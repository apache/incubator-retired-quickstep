/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#include "catalog/CatalogRelation.hpp"

#include <memory>
#include <string>
#include <vector>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogConfig.h"
#include "catalog/CatalogRelationSchema.hpp"

#ifdef QUICKSTEP_HAVE_LIBNUMA
#include "catalog/NUMAPlacementScheme.hpp"
#endif

#include "catalog/PartitionScheme.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "threading/SpinSharedMutex.hpp"
#include "utility/PtrVector.hpp"

#include "glog/logging.h"

namespace quickstep {

class Type;

bool CatalogRelation::ProtoIsValid(const serialization::CatalogRelationSchema &proto) {
  // Check that proto is fully initialized.
  if (!CatalogRelationSchema::ProtoIsValid(proto) ||
      !proto.HasExtension(serialization::CatalogRelation::default_layout)) {
    return false;
  }

  // Check if the proto of the relation has a partition scheme.
  if (proto.HasExtension(serialization::CatalogRelation::partition_scheme)) {
    // Check if the partition scheme protobuf is valid.
    if (!PartitionScheme::ProtoIsValid(
            proto.GetExtension(serialization::CatalogRelation::partition_scheme))) {
      return false;
    }

#ifdef QUICKSTEP_HAVE_LIBNUMA
    // Check if the proto of the relation has a NUMA placement scheme.
    if (proto.HasExtension(serialization::CatalogRelation::placement_scheme)) {
      // Check if the NUMA placement scheme protobuf is valid.
      if (!NUMAPlacementScheme::ProtoIsValid(
              proto.GetExtension(serialization::CatalogRelation::placement_scheme))) {
        return false;
      }
    }
#endif
  }

  // Catalog Relation's proto is valid.
  return true;
}

CatalogRelation::CatalogRelation(const serialization::CatalogRelationSchema &proto)
    : CatalogRelationSchema(proto),
      default_layout_(nullptr) {
  DCHECK(ProtoIsValid(proto))
      << "Attempted to create CatalogRelation from an invalid proto description:\n"
      << proto.DebugString();

  // Deserializing the partition scheme for the relation.
  // This should be done after the attributes are added and before the
  // blocks of the relation are added.
  if (proto.HasExtension(serialization::CatalogRelation::partition_scheme)) {
    const serialization::PartitionScheme &proto_partition_scheme =
        proto.GetExtension(serialization::CatalogRelation::partition_scheme);

    const attribute_id partition_attribute_id = proto_partition_scheme.header().partition_attribute_id();
    DCHECK(hasAttributeWithId(partition_attribute_id));
    const Type &attr_type = attr_vec_[partition_attribute_id].getType();

    setPartitionScheme(PartitionScheme::ReconstructFromProto(proto_partition_scheme, attr_type));

    // Deserializing the NUMA placement scheme for the relation.
#ifdef QUICKSTEP_HAVE_LIBNUMA
    if (proto.HasExtension(serialization::CatalogRelation::placement_scheme)) {
      setNUMAPlacementScheme(
          NUMAPlacementScheme::ReconstructFromProto(
              proto.GetExtension(serialization::CatalogRelation::placement_scheme),
              proto_partition_scheme.header().num_partitions()));
    }
#endif
  }

  const StorageBlockLayoutDescription &proto_default_layout =
      proto.GetExtension(serialization::CatalogRelation::default_layout);
  DCHECK(StorageBlockLayout::DescriptionIsValid(*this, proto_default_layout));

  // Deserializing the index scheme defined for the relation, if any.
  if (proto.HasExtension(serialization::CatalogRelation::index_scheme)) {
    index_scheme_.reset(
        IndexScheme::ReconstructFromProto(proto.GetExtension(serialization::CatalogRelation::index_scheme)));
    // Ensure that indices present in the block layout are the same as in the index scheme.
    const std::size_t num_indices_expected = index_scheme_->getNumIndices();
    const std::size_t num_indices_checked = proto_default_layout.index_description_size();
    DCHECK_EQ(num_indices_expected, num_indices_checked);
    for (std::size_t i = 0; i < num_indices_checked; ++i) {
      const IndexSubBlockDescription &description_checked = proto_default_layout.index_description(i);
      DCHECK(index_scheme_->hasIndexWithDescription(description_checked))
          << "Block layout defines some indices not present in the catalog";
    }
  }

  // Deserializing the blocks of the relation.
  for (int i = 0; i < proto.ExtensionSize(serialization::CatalogRelation::blocks); ++i) {
    blocks_.emplace_back(static_cast<block_id>(proto.GetExtension(serialization::CatalogRelation::blocks, i)));
  }

  default_layout_.reset(new StorageBlockLayout(*this, proto_default_layout));
  num_tuples_ = proto.GetExtension(serialization::CatalogRelation::num_tuples);
}

serialization::CatalogRelationSchema CatalogRelation::getProto() const {
  serialization::CatalogRelationSchema proto;

  proto.set_relation_id(id_);
  proto.set_name(name_);
  proto.set_temporary(temporary_);
  proto.SetExtension(serialization::CatalogRelation::num_tuples, num_tuples_);

  for (PtrVector<CatalogAttribute, true>::const_iterator it = attr_vec_.begin();
       it != attr_vec_.end();
       ++it) {
    if (it.isNull()) {
      proto.add_attributes();
    } else {
      proto.add_attributes()->MergeFrom(it->getProto());
    }
  }

  proto.MutableExtension(serialization::CatalogRelation::default_layout)
      ->MergeFrom(getDefaultStorageBlockLayout().getDescription());

  {
    SpinSharedMutexSharedLock<false> lock(blocks_mutex_);
    for (const block_id block : blocks_) {
      proto.AddExtension(serialization::CatalogRelation::blocks, block);
    }
  }

  if (hasIndexScheme()) {
    proto.MutableExtension(serialization::CatalogRelation::index_scheme)->MergeFrom(index_scheme_->getProto());
  }

  // The partition scheme of the relation is stored in the protocol buffer format.
  if (hasPartitionScheme()) {
    proto.MutableExtension(serialization::CatalogRelation::partition_scheme)
        ->MergeFrom(partition_scheme_->getProto());
#ifdef QUICKSTEP_HAVE_LIBNUMA
    if (hasNUMAPlacementScheme()) {
      proto.MutableExtension(serialization::CatalogRelation::placement_scheme)
          ->MergeFrom(placement_scheme_->getProto());
    }
#endif
  }

  return proto;
}

void CatalogRelation::setNumTuples(std::size_t num_tuples) {
  num_tuples_ = num_tuples;
}

void CatalogRelation::setPartitionScheme(PartitionScheme* partition_scheme) {
  DCHECK_EQ(0u, size_blocks());
  partition_scheme_.reset(partition_scheme);
}

void CatalogRelation::setDefaultStorageBlockLayout(StorageBlockLayout *default_layout) {
  DCHECK(&(default_layout->getRelation()) == this);
  default_layout_.reset(default_layout);
}

const StorageBlockLayout& CatalogRelation::getDefaultStorageBlockLayout() const {
  if (default_layout_ == nullptr) {
    default_layout_.reset(StorageBlockLayout::GenerateDefaultLayout(*this, isVariableLength()));
  }

  return *default_layout_;
}

}  // namespace quickstep
