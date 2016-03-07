/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

bool CatalogRelation::ProtoIsValid(const serialization::CatalogRelation &proto) {
  // Check that proto is fully initialized.
  if (!proto.IsInitialized()) {
    return false;
  }

  for (int i = 0; i < proto.attributes_size(); ++i) {
    if (!CatalogAttribute::ProtoIsValid(proto.attributes(i))) {
      return false;
    }
  }

  // Check if the proto of the relation has a partition scheme.
  if (proto.has_partition_scheme()) {
    // Check if the partition scheme protobuf is valid.
    if (!PartitionScheme::ProtoIsValid(proto.partition_scheme())) {
      return false;
    }

#ifdef QUICKSTEP_HAVE_LIBNUMA
    // Check if the proto of the relation has a NUMA placement scheme.
    if (proto.has_placement_scheme()) {
      // Check if the NUMA placement scheme protobuf is valid.
      if (!NUMAPlacementScheme::ProtoIsValid(proto.placement_scheme())) {
        return false;
      }
    }
#endif
  }

  // Catalog Relation's proto is valid.
  return true;
}

CatalogRelation::CatalogRelation(const serialization::CatalogRelation &proto)
    : CatalogRelationSchema(nullptr, proto.name(), -1, proto.temporary()),
      default_layout_(nullptr) {
  DCHECK(ProtoIsValid(proto))
      << "Attempted to create CatalogRelation from an invalid proto description:\n"
      << proto.DebugString();

  // Deserializing the attributes for the relation.
  for (int i = 0; i < proto.attributes_size(); ++i) {
    if (proto.attributes(i).IsInitialized()) {
      addAttribute(new CatalogAttribute(proto.attributes(i)));
    } else {
      attr_vec_.push_back(nullptr);
    }
  }

  // Deserializing the index scheme defined for the relation, if any.
  if (proto.has_index_scheme()) {
    index_scheme_.reset(IndexScheme::ReconstructFromProto(proto.index_scheme()));
  }

  // Deserializing the partition scheme for the relation.
  // This should be done after the attributes are added and before the
  // blocks of the relation are added.
  if (proto.has_partition_scheme()) {
    const serialization::PartitionScheme &proto_partition_scheme = proto.partition_scheme();

    const attribute_id partition_attribute_id = proto_partition_scheme.header().partition_attribute_id();
    DCHECK(hasAttributeWithId(partition_attribute_id));
    const Type &attr_type = attr_vec_[partition_attribute_id].getType();

    setPartitionScheme(PartitionScheme::ReconstructFromProto(proto_partition_scheme, attr_type));

    // Deserializing the NUMA placement scheme for the relation.
#ifdef QUICKSTEP_HAVE_LIBNUMA
    if (proto.has_placement_scheme()) {
      setNUMAPlacementScheme(
          NUMAPlacementScheme::ReconstructFromProto(proto.placement_scheme(),
                                                    proto_partition_scheme.header().num_partitions()));
    }
#endif
  }

  // Deserializing the blocks of the relation.
  for (int i = 0; i < proto.blocks_size(); ++i) {
    blocks_.emplace_back(static_cast<block_id>(proto.blocks(i)));
  }

  DCHECK(StorageBlockLayout::DescriptionIsValid(*this, proto.default_layout()));

  default_layout_.reset(new StorageBlockLayout(*this, proto.default_layout()));
}

void CatalogRelation::setPartitionScheme(PartitionScheme* partition_scheme) {
  DCHECK_EQ(0u, size_blocks());
  partition_scheme_.reset(partition_scheme);
}


serialization::CatalogRelation CatalogRelation::getProto() const {
  serialization::CatalogRelation proto;

  proto.set_name(name_);
  proto.set_temporary(temporary_);
  proto.mutable_default_layout()->CopyFrom(getDefaultStorageBlockLayout().getDescription());

  {
    SpinSharedMutexSharedLock<false> lock(blocks_mutex_);
    for (const block_id block : blocks_) {
      proto.add_blocks(block);
    }
  }

  if (hasIndexScheme()) {
    proto.mutable_index_scheme()->CopyFrom(index_scheme_->getProto());
  }

  for (PtrVector<CatalogAttribute, true>::const_iterator it = attr_vec_.begin();
       it != attr_vec_.end();
       ++it) {
    if (it.isNull()) {
      proto.add_attributes();
    } else {
      proto.add_attributes()->CopyFrom(it->getProto());
    }
  }

  // The partition scheme of the relation is stored in the protocol buffer format.
  if (hasPartitionScheme()) {
    proto.mutable_partition_scheme()->CopyFrom(partition_scheme_->getProto());
  }

  return proto;
}

void CatalogRelation::setDefaultStorageBlockLayout(StorageBlockLayout *default_layout) {
  DCHECK(&(default_layout->getRelation()) == this);
  default_layout_.reset(default_layout);
}

const StorageBlockLayout& CatalogRelation::getDefaultStorageBlockLayout() const {
  if (default_layout_.get() == nullptr) {
    default_layout_.reset(StorageBlockLayout::GenerateDefaultLayout(*this, isVariableLength()));
  }

  return *default_layout_;
}

}  // namespace quickstep
