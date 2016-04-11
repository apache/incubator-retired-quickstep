/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#include "storage/StorageBlockLayout.hpp"

#include <cstring>
#include <string>
#include <vector>

#include "glog/logging.h"

#include "catalog/CatalogRelationSchema.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageConstants.hpp"
#include "storage/StorageErrors.hpp"
#include "storage/SubBlockTypeRegistry.hpp"
#include "utility/Macros.hpp"

using std::size_t;
using std::string;
using std::strlen;
using std::vector;

namespace quickstep {

StorageBlockLayout::StorageBlockLayout(const CatalogRelationSchema &relation,
                                       const StorageBlockLayoutDescription &proto)
    : relation_(relation),
      estimated_bytes_per_tuple_(0) {
  layout_description_.Clear();
  layout_description_.CopyFrom(proto);

  finalize();
}

// TODO(chasseur): Align sub-blocks to cache line boundaries.
void StorageBlockLayout::finalize() {
  CHECK(DescriptionIsValid(relation_, layout_description_))
      << "Called StorageBlockLayout::finalize() with incomplete or invalid layout.";

  // Reset the header and copy the layout from this StorageBlockLayout.
  block_header_.Clear();
  block_header_.mutable_layout()->CopyFrom(layout_description_);

  // Temporarily set all sub-block sizes to zero, and set all indices as
  // consistent.
  block_header_.set_tuple_store_size(0);
  for (int index_num = 0;
       index_num < layout_description_.index_description_size();
       ++index_num) {
    block_header_.add_index_size(0);
    block_header_.add_index_consistent(true);
  }

  DEBUG_ASSERT(block_header_.IsInitialized());

  size_t header_size = getBlockHeaderSize();
  if (header_size > layout_description_.num_slots() * kSlotSizeBytes) {
    throw BlockMemoryTooSmall("StorageBlockLayout", layout_description_.num_slots() * kSlotSizeBytes);
  }

  // Assign block space to sub-blocks in proportion to the estimated number of
  // bytes needed for each tuple in each sub-block.
  estimated_bytes_per_tuple_ = 0;

  // Bytes for tuple store.
  const TupleStorageSubBlockDescription &tuple_store_description
      = layout_description_.tuple_store_description();
  const size_t tuple_store_size_factor = SubBlockTypeRegistry::EstimateBytesPerTupleForTupleStore(
      relation_,
      tuple_store_description);
  estimated_bytes_per_tuple_ += tuple_store_size_factor;

  // Bytes for each index.
  vector<size_t> index_size_factors;
  index_size_factors.reserve(layout_description_.index_description_size());
  for (int index_num = 0;
       index_num < layout_description_.index_description_size();
       ++index_num) {
    const IndexSubBlockDescription &index_description = layout_description_.index_description(index_num);
    const size_t index_size_factor = SubBlockTypeRegistry::EstimateBytesPerTupleForIndex(
        relation_,
        index_description);
    index_size_factors.push_back(index_size_factor);
    estimated_bytes_per_tuple_ += index_size_factor;
  }

  if (estimated_bytes_per_tuple_ == 0) {
    LOG(WARNING)
        << "Estimated zero bytes per tuple when finalizing a "
        << "StorageBlockLayout for relation \"" << relation_.getName()
        << "\" (relation_id: " << relation_.getID()
        << "). Adjusting to 1 byte.";
    estimated_bytes_per_tuple_ = 1;
  }

  size_t allocated_sub_block_space = 0;
  size_t sub_block_space = (layout_description_.num_slots() * kSlotSizeBytes) - header_size;

  for (int index_num = 0;
       index_num < layout_description_.index_description_size();
       ++index_num) {
    size_t index_size = 0;
    if (index_size_factors[index_num] != kZeroSize) {
      // Estimated size to be consumed was specified per tuple.
      index_size = (sub_block_space * index_size_factors[index_num]) / estimated_bytes_per_tuple_;
    } else {
      // Some indices define total size per block, instead of defining size per tuple.
      const IndexSubBlockDescription &index_description = layout_description_.index_description(index_num);
      index_size = SubBlockTypeRegistry::EstimateBytesPerBlockForIndex(relation_, index_description);
    }
    block_header_.set_index_size(index_num, index_size);
    allocated_sub_block_space += index_size;
  }

  block_header_.set_tuple_store_size(sub_block_space - allocated_sub_block_space);

  DEBUG_ASSERT(block_header_.IsInitialized());
  DEBUG_ASSERT(header_size == getBlockHeaderSize());
}

void StorageBlockLayout::copyHeaderTo(void *dest) const {
  DEBUG_ASSERT(DescriptionIsValid(relation_, layout_description_));
  DEBUG_ASSERT(block_header_.IsInitialized());

  *static_cast<int*>(dest) = block_header_.ByteSize();
  if (!block_header_.SerializeToArray(static_cast<char*>(dest) + sizeof(int),
                                      block_header_.ByteSize())) {
    FATAL_ERROR("Failed to do binary serialization of StorageBlockHeader in StorageBlockLayout::copyHeaderTo()");
  }
}

StorageBlockLayout* StorageBlockLayout::GenerateDefaultLayout(const CatalogRelationSchema &relation,
                                                              const bool relation_variable_length) {
  StorageBlockLayout *layout = new StorageBlockLayout(relation);

  StorageBlockLayoutDescription *description = layout->getDescriptionMutable();
  description->set_num_slots(1);

  TupleStorageSubBlockDescription *tuple_store_description = description->mutable_tuple_store_description();
  if (relation_variable_length) {
    tuple_store_description->set_sub_block_type(TupleStorageSubBlockDescription::SPLIT_ROW_STORE);
  } else {
    tuple_store_description->set_sub_block_type(TupleStorageSubBlockDescription::PACKED_ROW_STORE);
  }

  // See if we can add a dummy index. Will confirm that the module idea works.
  IndexSubBlockDescription *dummy_desc = description->add_index_description();
  dummy_desc->set_sub_block_type(IndexSubBlockDescription::BITWEAVING_V);
  dummy_desc->add_indexed_attribute_ids(0);

  layout->finalize();
  return layout;
}

bool StorageBlockLayout::DescriptionIsValid(const CatalogRelationSchema &relation,
                                            const StorageBlockLayoutDescription &description) {
  return SubBlockTypeRegistry::LayoutDescriptionIsValid(relation, description);
}

std::size_t StorageBlockLayout::estimateTuplesPerBlock() const {
  DEBUG_ASSERT(block_header_.IsInitialized());
  return ((layout_description_.num_slots() * kSlotSizeBytes) - getBlockHeaderSize())
         / estimated_bytes_per_tuple_;
}

}  // namespace quickstep
