/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#include "storage/SplitRowStoreTupleStorageSubBlock.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <utility>
#include <vector>

#include "catalog/CatalogRelationSchema.hpp"
#include "storage/SplitRowStoreValueAccessor.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageErrors.hpp"
#include "storage/SubBlockTypeRegistry.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/TypedValue.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"
#include "utility/ScopedBuffer.hpp"

namespace quickstep {

QUICKSTEP_REGISTER_TUPLE_STORE(SplitRowStoreTupleStorageSubBlock, SPLIT_ROW_STORE);

using splitrow_internal::CopyGroupList;
using splitrow_internal::ContiguousAttrs;
using splitrow_internal::NullableAttr;
using splitrow_internal::VarLenAttr;

const std::size_t SplitRowStoreTupleStorageSubBlock::kVarLenSlotSize = sizeof(std::uint32_t) * 2;

namespace {

  template<typename ValueAccessorT, bool nullable_attrs>
  inline std::size_t CalculateVariableSize(
    const CatalogRelationSchema &relation,
    const ValueAccessorT &accessor) {
    std::size_t total_size = 0;
    attribute_id accessor_attr_id = 0;
    for (CatalogRelationSchema::const_iterator attr_it = relation.begin();
         attr_it != relation.end();
         ++attr_it, ++accessor_attr_id) {
      if (!attr_it->getType().isVariableLength()) {
        continue;
      }

      TypedValue value(accessor.getTypedValue(accessor_attr_id));
      if (nullable_attrs && value.isNull()) {
        continue;
      }
      total_size += value.getDataSize();
    }
    return total_size;
  }

  template<typename ValueAccessorT, bool nullable_attrs>
  inline std::size_t CalculateVariableSizeWithRemappedAttributes(
    const CatalogRelationSchema &relation,
    const ValueAccessorT &accessor,
    const std::vector<attribute_id> &attribute_map) {
    std::size_t total_size = 0;
    std::vector<attribute_id>::const_iterator attr_map_it = attribute_map.begin();
    for (CatalogRelationSchema::const_iterator attr_it = relation.begin();
         attr_it != relation.end();
         ++attr_it, ++attr_map_it) {
      if (!attr_it->getType().isVariableLength()) {
        continue;
      }

      TypedValue value(accessor.getTypedValue(*attr_map_it));
      if (nullable_attrs && value.isNull()) {
        continue;
      }
      total_size += value.getDataSize();
    }
    return total_size;
  }

}  // anonymous namespace

SplitRowStoreTupleStorageSubBlock::SplitRowStoreTupleStorageSubBlock(
    const CatalogRelationSchema &relation,
    const TupleStorageSubBlockDescription &description,
    const bool new_block,
    void *sub_block_memory,
    const std::size_t sub_block_memory_size)
    : TupleStorageSubBlock(relation,
                           description,
                           new_block,
                           sub_block_memory,
                           sub_block_memory_size),
      header_(static_cast<Header*>(sub_block_memory)),
      num_null_attrs_(0),
      num_fixed_attrs_(0),
      num_var_attrs_(0) {
  if (!DescriptionIsValid(relation_, description_)) {
    FATAL_ERROR("Attempted to construct a SplitRowStoreTupleStorageSubBlock from an invalid description.");
  }

  if (sub_block_memory_size < sizeof(Header)) {
    throw BlockMemoryTooSmall("SplitRowStoreTupleStorageSubBlock", sub_block_memory_size);
  }

  // Slot format is a bitmap of null attributes, followed by fixed-length
  // attribute storage, followed by references to variable-length attribute
  // values (each is a pair of uint32_ts indicating the offset in the
  // tuple-storage region and the size of the value).
  per_tuple_null_bitmap_bytes_ = BitVector<true>::BytesNeeded(relation.numNullableAttributes());
  tuple_slot_bytes_ = per_tuple_null_bitmap_bytes_
                      + relation.getFixedByteLength()
                      + relation.numVariableLengthAttributes() * (sizeof(std::uint32_t) * 2);

  // Size the occupancy bitmap by calculating the maximum tuples that can fit
  // assuming the bare-minimum per tuple storage is used (no variable-length
  // storage used for nullable variable-length attributes, otherwise the
  // minimum possible.
  //
  // TODO(chasseur): This will slightly over-size the occupancy bitmap, since
  // it doesn't take into account bytes used to store the bitmap itself. Adding
  // some generic size-estimation functions to BitVector might be a good idea.
  std::size_t max_num_tuples
      = (sub_block_memory_size_ - sizeof(Header))
        / (tuple_slot_bytes_ + relation_.getMinimumVariableByteLengthExcludingNullable());

  occupancy_bitmap_bytes_ = BitVector<false>::BytesNeeded(max_num_tuples);
  if (sub_block_memory_size < occupancy_bitmap_bytes_ + sizeof(Header)) {
    throw BlockMemoryTooSmall("SplitRowStoreTupleStorageSubBlock", sub_block_memory_size);
  }
  occupancy_bitmap_.reset(
      new BitVector<false>(static_cast<char*>(sub_block_memory_) + sizeof(Header),
                           max_num_tuples));

  tuple_storage_ = static_cast<char*>(sub_block_memory_)
                   + sizeof(Header) + occupancy_bitmap_bytes_;
  tuple_storage_bytes_ = sub_block_memory_size_ - (sizeof(Header) + occupancy_bitmap_bytes_);

  // Some accounting information for bulk inserts.
  for (attribute_id attr_id = 0;
       attr_id < static_cast<attribute_id>(relation.size());
       ++attr_id) {
    const Type& attr_type = relation.getAttributeById(attr_id)->getType();
    if (attr_type.isVariableLength()) {
      fixed_len_attr_sizes_.push_back(kInvalidAttributeID);
      num_var_attrs_++;
    } else {
      fixed_len_attr_sizes_.push_back(attr_type.maximumByteLength());
      num_fixed_attrs_++;
    }
    num_null_attrs_ += attr_type.isNullable();
  }

  if (new_block) {
    // Only need to initialize these fields, the rest of the block will be
    // zeroed-out by the StorageManager.
    header_->max_tid = -1;
    header_->variable_length_storage_compact = true;
  }
}

bool SplitRowStoreTupleStorageSubBlock::DescriptionIsValid(
    const CatalogRelationSchema &relation,
    const TupleStorageSubBlockDescription &description) {
  // Make sure description is initialized and specifies SplitRowStore.
  if (!description.IsInitialized()) {
    return false;
  }
  if (description.sub_block_type() != TupleStorageSubBlockDescription::SPLIT_ROW_STORE) {
    return false;
  }

  return true;
}

std::size_t SplitRowStoreTupleStorageSubBlock::EstimateBytesPerTuple(
    const CatalogRelationSchema &relation,
    const TupleStorageSubBlockDescription &description) {
  DEBUG_ASSERT(DescriptionIsValid(relation, description));

  return relation.getFixedByteLength()                                           // Fixed-length attrs
         + BitVector<true>::BytesNeeded(relation.numNullableAttributes())        // Null bitmap
         + relation.numVariableLengthAttributes() * (sizeof(std::uint32_t) * 2)  // Variable-length ref
         + relation.getEstimatedVariableByteLength();                            // Variable-length attrs
}

TupleStorageSubBlock::InsertResult SplitRowStoreTupleStorageSubBlock::insertTuple(
    const Tuple &tuple) {
  if (relation_.hasNullableAttributes()) {
    if (relation_.isVariableLength()) {
      return insertTupleImpl<true, true>(tuple);
    } else {
      return insertTupleImpl<true, false>(tuple);
    }
  } else {
    if (relation_.isVariableLength()) {
      return insertTupleImpl<false, true>(tuple);
    } else {
      return insertTupleImpl<false, false>(tuple);
    }
  }
}

tuple_id SplitRowStoreTupleStorageSubBlock::bulkInsertTuples(ValueAccessor *accessor) {
  std::vector<attribute_id> simple_remap;
  for (attribute_id attr_id = 0;
      attr_id < static_cast<attribute_id>(relation_.size());
      ++attr_id) {
    simple_remap.push_back(attr_id);
  }
  return bulkInsertDispatcher(simple_remap, accessor, kCatalogMaxID, true);
}

tuple_id SplitRowStoreTupleStorageSubBlock::bulkInsertPartialTuples(
  const std::vector<attribute_id> &attribute_map,
  ValueAccessor *accessor,
  const tuple_id max_num_tuples_to_insert) {
  return bulkInsertDispatcher(attribute_map, accessor, max_num_tuples_to_insert, false);
}

tuple_id SplitRowStoreTupleStorageSubBlock::bulkInsertDispatcher(
  const std::vector<attribute_id> &attribute_map,
  ValueAccessor *accessor,
  tuple_id max_num_tuples_to_insert,
  bool finalize) {
  const bool fill_to_capacity = max_num_tuples_to_insert == kCatalogMaxID;

  CopyGroupList copy_groups;
  getCopyGroupsForAttributeMap(attribute_map, &copy_groups);
  auto impl = accessor->getImplementationType();
  const bool is_rowstore_source =
    (impl == ValueAccessor::Implementation::kPackedRowStore ||
     impl == ValueAccessor::Implementation::kSplitRowStore);
  if (is_rowstore_source) {
    copy_groups.merge_contiguous();
  }

  const bool copy_nulls = copy_groups.nullable_attrs_.size() > 0;
  const bool copy_varlen = copy_groups.varlen_attrs_.size() > 0;

  if (fill_to_capacity) {
    if (relation_.hasNullableAttributes()) {
      // TODO(marc) This is an annoying gotcha: the insertion loop assumes the null
      // bitmaps are zero'd for a fresh insert. We could clear the bit map on each tuple
      // iteration, but that'd be costlier.
      std::int64_t remaining_bytes = tuple_storage_bytes_ -
                                     (header_->variable_length_bytes_allocated +
                                      (header_->num_tuples * tuple_slot_bytes_));
      memset(static_cast<char *>(tuple_storage_) + header_->num_tuples * tuple_slot_bytes_, 0x0, remaining_bytes);
    }
  }

  tuple_id num_inserted = 0;
  if (max_num_tuples_to_insert == kCatalogMaxID) {
    max_num_tuples_to_insert = getInsertLowerBound();
  }
  if (copy_varlen) {
    if (copy_nulls) {
      if (fill_to_capacity) {
        num_inserted = bulkInsertPartialTuplesImpl<true, true, true>(copy_groups, accessor,
                                                                     max_num_tuples_to_insert);
      } else {
        num_inserted = bulkInsertPartialTuplesImpl<true, true, false>(copy_groups, accessor,
                                                                      max_num_tuples_to_insert);
      }
    } else {
      if (fill_to_capacity) {
        num_inserted = bulkInsertPartialTuplesImpl<false, true, true>(copy_groups, accessor,
                                                                      max_num_tuples_to_insert);
      } else {
        num_inserted = bulkInsertPartialTuplesImpl<false, true, false>(copy_groups, accessor,
                                                                       max_num_tuples_to_insert);
      }
    }
  } else {
    if (copy_nulls) {
      num_inserted = bulkInsertPartialTuplesImpl<true, false, false>(copy_groups, accessor, max_num_tuples_to_insert);
    } else {
      num_inserted = bulkInsertPartialTuplesImpl<false, false, false>(copy_groups, accessor, max_num_tuples_to_insert);
    }
  }

  if (finalize) {
    bulkInsertPartialTuplesFinalize(num_inserted);
  }
  return num_inserted;
}

// copy_nulls is true if the incoming attributes include at least one nullable attribute
// copy_varlen is true if the incoming attributes include at least one varlen attribute
template<bool copy_nulls, bool copy_varlen, bool fill_to_capacity>
tuple_id SplitRowStoreTupleStorageSubBlock::bulkInsertPartialTuplesImpl(
  const CopyGroupList &copy_groups,
  ValueAccessor *accessor,
  std::size_t max_num_tuples_to_insert) {
  std::size_t num_tuples_inserted = 0;

  // We only append to the end of the block to cut down on complexity.
  char *tuple_slot = static_cast<char *>(tuple_storage_) +  header_->num_tuples * tuple_slot_bytes_;

  std::uint32_t varlen_heap_offset = tuple_storage_bytes_ - header_->variable_length_bytes_allocated;
  std::uint32_t varlen_heap_offset_orig = varlen_heap_offset;

  BitVector<true> tuple_null_bitmap(tuple_slot, num_null_attrs_);
  char *fixed_len_cursor = tuple_slot + BitVector<true>::BytesNeeded(num_null_attrs_);



  std::size_t storage_available = tuple_storage_bytes_ -
                                    (header_->variable_length_bytes_allocated +
                                     header_->num_tuples * tuple_slot_bytes_);

  // The number of bytes that must be reserved per tuple inserted due to gaps.
  std::size_t varlen_reserve = relation_.getMaximumVariableByteLength();
  if (fill_to_capacity) {
    for (std::size_t vattr_idx = 0; vattr_idx < copy_groups.varlen_attrs_.size(); vattr_idx++) {
      varlen_reserve -= relation_.getAttributeById(
        copy_groups.varlen_attrs_[vattr_idx].dst_attr_id_)->getType().maximumByteLength();
    }
    DCHECK_GE(relation_.getMaximumVariableByteLength(), varlen_reserve);
  }

  InvokeOnAnyValueAccessor(
    accessor,
    [&](auto *accessor) -> void {  // NOLINT(build/c++11
      do {
        const std::size_t num_c_attr = copy_groups.contiguous_attrs_.size();
        const std::size_t num_n_attr = copy_groups.nullable_attrs_.size();
        const std::size_t num_v_attr = copy_groups.varlen_attrs_.size();

        const std::size_t nullmap_size = BitVector<true>::BytesNeeded(num_null_attrs_);

        while (num_tuples_inserted < max_num_tuples_to_insert && accessor->next()) {
          for (std::size_t cattr_idx = 0; cattr_idx < num_c_attr; cattr_idx++) {
            const ContiguousAttrs &cattr = copy_groups.contiguous_attrs_[cattr_idx];
            fixed_len_cursor += cattr.bytes_to_advance_;
            const void *attr_value = accessor->template getUntypedValue<false>(cattr.src_attr_id_);
            std::memcpy(fixed_len_cursor, attr_value, cattr.bytes_to_copy_);
          }

          if (copy_nulls) {
            tuple_null_bitmap.setMemory(tuple_slot);
            for (std::size_t nattr_idx = 0; nattr_idx < num_n_attr; nattr_idx++) {
              const NullableAttr &nattr = copy_groups.nullable_attrs_[nattr_idx];
              const void *attr_value = accessor->template getUntypedValue<true>(nattr.src_attr_id_);
              if (attr_value == nullptr) {
                tuple_null_bitmap.setBit(nattr.nullable_attr_idx_, true);
              }
            }
          }

          if (copy_varlen) {
            for (std::size_t vattr_idx = 0; vattr_idx < num_v_attr; vattr_idx++) {
              const VarLenAttr &vattr = copy_groups.varlen_attrs_[vattr_idx];
              fixed_len_cursor += vattr.bytes_to_advance_;
              // Typed value is necessary as we need the length.
              const TypedValue &attr_value = accessor->template getTypedValue(vattr.src_attr_id_);
              if (attr_value.isNull()) {
                continue;
              }
              const std::size_t attr_size = attr_value.getDataSize();
              varlen_heap_offset -= attr_size;
              std::memcpy(static_cast<char *>(tuple_storage_) + varlen_heap_offset, attr_value.getDataPtr(),
                          attr_size);
              reinterpret_cast<std::uint32_t *>(fixed_len_cursor)[0] = varlen_heap_offset;
              reinterpret_cast<std::uint32_t *>(fixed_len_cursor)[1] = static_cast<std::uint32_t>(attr_size);
            }
          }
          tuple_slot += tuple_slot_bytes_;
          fixed_len_cursor = tuple_slot + nullmap_size;
          num_tuples_inserted++;
        }
        if (fill_to_capacity) {
          std::int64_t remaining_storage_after_inserts = storage_available -
                                                         (num_tuples_inserted * (tuple_slot_bytes_ + varlen_reserve) +
                                                          (varlen_heap_offset_orig - varlen_heap_offset));
          DCHECK_LE(0, remaining_storage_after_inserts);
          std::size_t additional_tuples_insert =
            remaining_storage_after_inserts / (tuple_slot_bytes_ + this->relation_.getMaximumByteLength());
          // We want to avoid a situation where we have several short insert iterations
          // near the end of an insertion cycle.
          if (additional_tuples_insert > this->getInsertLowerBoundThreshold()) {
            max_num_tuples_to_insert += additional_tuples_insert;
          }
        }
      } while (fill_to_capacity && !accessor->iterationFinishedVirtual() &&
               num_tuples_inserted < max_num_tuples_to_insert);
    });

  if (copy_varlen) {
    header_->variable_length_bytes_allocated += (varlen_heap_offset_orig - varlen_heap_offset);
  }

  return num_tuples_inserted;
}

void SplitRowStoreTupleStorageSubBlock::bulkInsertPartialTuplesFinalize(
    const tuple_id num_tuples_inserted) {
  occupancy_bitmap_->setBitRange(header_->max_tid + 1, num_tuples_inserted, true);
  header_->num_tuples += num_tuples_inserted;
  header_->max_tid += num_tuples_inserted;
}

std::size_t SplitRowStoreTupleStorageSubBlock::getInsertLowerBound() const {
  const std::size_t remaining_storage_bytes = tuple_storage_bytes_ -
                                              (header_->variable_length_bytes_allocated +
                                               ((header_->max_tid + 1) * tuple_slot_bytes_));
  const std::size_t tuple_max_size = tuple_slot_bytes_ + relation_.getMaximumVariableByteLength();
  return remaining_storage_bytes / tuple_max_size;
}

tuple_id SplitRowStoreTupleStorageSubBlock::bulkInsertTuplesWithRemappedAttributes(
    const std::vector<attribute_id> &attribute_map,
    ValueAccessor *accessor) {
  DCHECK_EQ(relation_.size(), attribute_map.size());
  return bulkInsertDispatcher(attribute_map, accessor, kCatalogMaxID, true);
}

const void* SplitRowStoreTupleStorageSubBlock::getAttributeValue(
    const tuple_id tuple,
    const attribute_id attr) const {
  DEBUG_ASSERT(occupancy_bitmap_->getBit(tuple));
  DEBUG_ASSERT(relation_.hasAttributeWithId(attr));
  const char *tuple_slot = static_cast<const char*>(tuple_storage_)
                           + tuple_slot_bytes_ * tuple;

  const int nullable_idx = relation_.getNullableAttributeIndex(attr);
  if (nullable_idx != -1) {
    // const_cast is safe here. We will only be using read-only methods of
    // BitVector.
    BitVector<true> tuple_null_bitmap(const_cast<void*>(static_cast<const void*>(tuple_slot)),
                                      relation_.numNullableAttributes());
    if (tuple_null_bitmap.getBit(nullable_idx)) {
      return nullptr;
    }
  }

  const int variable_length_idx = relation_.getVariableLengthAttributeIndex(attr);
  if (variable_length_idx == -1) {
    // Fixed-length, stored in-line in slot.
    return tuple_slot
           + per_tuple_null_bitmap_bytes_
           + relation_.getFixedLengthAttributeOffset(attr);
  } else {
    // Variable-length, stored at back of block.
    const std::uint32_t pos = *reinterpret_cast<const std::uint32_t*>(
        tuple_slot + per_tuple_null_bitmap_bytes_
                   + relation_.getFixedByteLength()
                   + variable_length_idx * 2 * sizeof(std::uint32_t));
    return static_cast<const char*>(tuple_storage_) + pos;
  }
}

TypedValue SplitRowStoreTupleStorageSubBlock::getAttributeValueTyped(
    const tuple_id tuple,
    const attribute_id attr) const {
  DEBUG_ASSERT(occupancy_bitmap_->getBit(tuple));
  DEBUG_ASSERT(relation_.hasAttributeWithId(attr));
  const Type &attr_type = relation_.getAttributeById(attr)->getType();
  const char *tuple_slot = static_cast<const char*>(tuple_storage_)
                           + tuple_slot_bytes_ * tuple;
  const int nullable_idx = relation_.getNullableAttributeIndex(attr);
  if (nullable_idx != -1) {
    // const_cast is safe here. We will only be using read-only methods of
    // BitVector.
    BitVector<true> tuple_null_bitmap(const_cast<void*>(static_cast<const void*>(tuple_slot)),
                                      relation_.numNullableAttributes());
    if (tuple_null_bitmap.getBit(nullable_idx)) {
      return attr_type.makeNullValue();
    }
  }

  const int variable_length_idx = relation_.getVariableLengthAttributeIndex(attr);
  if (variable_length_idx == -1) {
    // Fixed-length, stored in-line in slot.
    return attr_type.makeValue(
        tuple_slot + per_tuple_null_bitmap_bytes_ + relation_.getFixedLengthAttributeOffset(attr),
        attr_type.maximumByteLength());
  } else {
    // Variable-length, stored at back of block.
    const std::uint32_t *pos_ptr = reinterpret_cast<const std::uint32_t*>(
        tuple_slot + per_tuple_null_bitmap_bytes_
                   + relation_.getFixedByteLength()
                   + variable_length_idx * 2 * sizeof(std::uint32_t));
    return attr_type.makeValue(static_cast<const char*>(tuple_storage_) + pos_ptr[0],
                               pos_ptr[1]);
  }
}

ValueAccessor* SplitRowStoreTupleStorageSubBlock::createValueAccessor(
    const TupleIdSequence *sequence) const {
  SplitRowStoreValueAccessor *base_accessor
      = new SplitRowStoreValueAccessor(relation_,
                                       header_->num_tuples,
                                       header_->max_tid,
                                       *occupancy_bitmap_,
                                       tuple_storage_,
                                       tuple_slot_bytes_,
                                       per_tuple_null_bitmap_bytes_);
  if (sequence == nullptr) {
    return base_accessor;
  } else {
    return new TupleIdSequenceAdapterValueAccessor<SplitRowStoreValueAccessor>(
        base_accessor,
        *sequence);
  }
}

bool SplitRowStoreTupleStorageSubBlock::canSetAttributeValuesInPlaceTyped(
    const tuple_id tuple,
    const std::unordered_map<attribute_id, TypedValue> &new_values) const {
  DEBUG_ASSERT(hasTupleWithID(tuple));

  if (!relation_.isVariableLength()) {
    return true;
  }

  const void *tuple_slot = static_cast<const char*>(tuple_storage_) + tuple * tuple_slot_bytes_;
  // const_cast is safe. Only read-only methods of BitVector are used here.
  BitVector<true> tuple_null_bitmap(const_cast<void*>(tuple_slot),
                                    relation_.numNullableAttributes());
  const std::uint32_t *variable_length_info_array = reinterpret_cast<const std::uint32_t*>(
      static_cast<const char*>(tuple_slot)
          + per_tuple_null_bitmap_bytes_
          + relation_.getFixedByteLength());

  std::size_t extra_variable_bytes = 0;
  for (const std::pair<const attribute_id, TypedValue> &update_pair : new_values) {
    int nullable_idx = relation_.getNullableAttributeIndex(update_pair.first);
    bool original_null = false;
    if (nullable_idx != -1) {
      if (update_pair.second.isNull()) {
        // If new attribute value is NULL, no extra storage is needed.
        continue;
      }
      original_null = tuple_null_bitmap.getBit(nullable_idx);
    }
    int variable_idx = relation_.getVariableLengthAttributeIndex(update_pair.first);
    if (variable_idx == -1) {
      // If attribute is fixed-length, it can be overwritten inline in the slot.
      continue;
    }
    const std::size_t update_size = update_pair.second.getDataSize();
    if (original_null
        || (update_size > variable_length_info_array[(variable_idx << 1) + 1])) {
      // If value's size is less than or equal to the existing value, we can
      // overwrite it in place. We only need to allocate more storage if it is
      // larger.
      extra_variable_bytes += update_size;
    }
  }

  if ((header_->max_tid + 1) * tuple_slot_bytes_
          + header_->variable_length_bytes_allocated
          + extra_variable_bytes
      > tuple_storage_bytes_) {
    return false;
  }

  return true;
}

void SplitRowStoreTupleStorageSubBlock::setAttributeValueInPlaceTyped(
    const tuple_id tuple,
    const attribute_id attr,
    const TypedValue &value) {
  DEBUG_ASSERT(hasTupleWithID(tuple));
  DEBUG_ASSERT(relation_.hasAttributeWithId(attr));

  void *tuple_slot = static_cast<char*>(tuple_storage_) + tuple * tuple_slot_bytes_;
  const int nullable_idx = relation_.getNullableAttributeIndex(attr);
  bool original_null = false;
  if (nullable_idx != -1) {
    // Set bit in null bitmap.
    BitVector<true> tuple_null_bitmap(tuple_slot, relation_.numNullableAttributes());
    original_null = tuple_null_bitmap.getBit(nullable_idx);
    if (value.isNull()) {
      tuple_null_bitmap.setBit(nullable_idx, true);
      return;
    } else {
      tuple_null_bitmap.setBit(nullable_idx, false);
    }
  }
  DEBUG_ASSERT(!value.isNull());

  const int variable_length_idx = relation_.getVariableLengthAttributeIndex(attr);
  if (variable_length_idx == -1) {
    // Copy fixed-length value into inline position in slot.
    value.copyInto(static_cast<char*>(tuple_slot)
                   + per_tuple_null_bitmap_bytes_
                   + relation_.getFixedLengthAttributeOffset(attr));
  } else {
    const std::size_t value_size = value.getDataSize();
    std::uint32_t *variable_length_info_array = reinterpret_cast<std::uint32_t*>(
        static_cast<char*>(tuple_slot)
            + per_tuple_null_bitmap_bytes_
            + relation_.getFixedByteLength());
    if (original_null
        || (value_size > variable_length_info_array[(variable_length_idx << 1) + 1])) {
      // Allocate more space.
      DEBUG_ASSERT((header_->max_tid + 1) * tuple_slot_bytes_
                       + header_->variable_length_bytes_allocated
                       + value_size
                   <= tuple_storage_bytes_);

      header_->variable_length_bytes_allocated += value_size;

      // Point to the newly-allocated location.
      variable_length_info_array[variable_length_idx << 1]
          = tuple_storage_bytes_ - header_->variable_length_bytes_allocated;
      header_->variable_length_storage_compact = false;
    } else if (value_size < variable_length_info_array[(variable_length_idx << 1) + 1]) {
      // If we are overwriting in place and using less than exactly the same
      // number of bytes as the original value, storage is no longer compact.
      header_->variable_length_storage_compact = false;
    }
    // Actually copy the value into variable-length storage.
    value.copyInto(static_cast<char*>(tuple_storage_)
                   + variable_length_info_array[variable_length_idx << 1]);
    // Update the size slot.
    variable_length_info_array[(variable_length_idx << 1) + 1] = value_size;
  }
}

bool SplitRowStoreTupleStorageSubBlock::deleteTuple(const tuple_id tuple) {
  DEBUG_ASSERT(hasTupleWithID(tuple));
  occupancy_bitmap_->setBit(tuple, false);
  --(header_->num_tuples);
  if (header_->num_tuples) {
    if (tuple == header_->max_tid) {
      header_->max_tid = occupancy_bitmap_->lastOne();
      // Since 'header_->num_tuples' is nonzero, there should be at least one
      // bit still set in '*occupancy_bitmap_'.
      DEBUG_ASSERT(static_cast<std::size_t>(header_->max_tid) != occupancy_bitmap_->size());
    }
    header_->variable_length_storage_compact = false;
  } else {
    header_->max_tid = -1;
    header_->variable_length_bytes_allocated = 0;
    header_->variable_length_storage_compact = true;
  }

  return false;
}

bool SplitRowStoreTupleStorageSubBlock::bulkDeleteTuples(TupleIdSequence *tuples) {
  occupancy_bitmap_->unsetFrom(tuples->getInternalBitVector());
  header_->num_tuples = occupancy_bitmap_->onesCount();
  if (header_->num_tuples) {
    header_->max_tid = occupancy_bitmap_->lastOne();
    header_->variable_length_storage_compact = false;
  } else {
    header_->max_tid = -1;
    header_->variable_length_bytes_allocated = 0;
    header_->variable_length_storage_compact = true;
  }
  return false;
}

OrderedTupleIdSequence* SplitRowStoreTupleStorageSubBlock::getExistenceList() const {
  if (isPacked()) {
    return TupleStorageSubBlock::getExistenceList();
  }

  OrderedTupleIdSequence *existence_list = new OrderedTupleIdSequence();
  existence_list->reserve(header_->num_tuples);
  for (std::size_t pos = occupancy_bitmap_->firstOne();
       pos < occupancy_bitmap_->size();
       occupancy_bitmap_->firstOne(pos + 1)) {
    existence_list->push_back(pos);
  }
  return existence_list;
}

void SplitRowStoreTupleStorageSubBlock::rebuild() {
  // First, pack all tuple slots in the front of the block.
  if (!isPacked()) {
    // The packing algorithm scans for holes starting at the beginning of the
    // slot array and fills them with tuples taken from the back. This
    // minimizes the total number of moves.
    std::size_t dest = occupancy_bitmap_->firstZero();
    std::size_t src = occupancy_bitmap_->lastOne(header_->max_tid + 1);
    while (dest < static_cast<std::size_t>(header_->num_tuples)) {
      std::memcpy(static_cast<char*>(tuple_storage_) + dest * tuple_slot_bytes_,
                  static_cast<char*>(tuple_storage_) + src * tuple_slot_bytes_,
                  tuple_slot_bytes_);
      dest = occupancy_bitmap_->firstZero(dest + 1);
      src = occupancy_bitmap_->lastOne(src);
    }
    // Update header and reset occupancy bitmap.
    header_->max_tid = header_->num_tuples - 1;
    occupancy_bitmap_->clear();
    occupancy_bitmap_->setBitRange(0, header_->num_tuples, true);
  }

  if (relation_.isVariableLength()
      && !header_->variable_length_storage_compact
      && (header_->variable_length_bytes_allocated != 0)) {
    // Some variable-length storage is used, but it has holes.
    // Allocate a temporary buffer to store packed variable-length values.
    ScopedBuffer packed_buffer(header_->variable_length_bytes_allocated);
    std::size_t buffer_pos = 0;
    for (tuple_id tid = 0; tid < header_->num_tuples; ++tid) {
      void *tuple_slot = static_cast<char*>(tuple_storage_) + tid * tuple_slot_bytes_;
      BitVector<true> tuple_null_bitmap(tuple_slot, relation_.numNullableAttributes());
      std::uint32_t *variable_length_info_array = reinterpret_cast<std::uint32_t*>(
          static_cast<char*>(tuple_slot)
              + per_tuple_null_bitmap_bytes_
              + relation_.getFixedByteLength());

      for (CatalogRelationSchema::const_iterator attr_it = relation_.begin();
           attr_it != relation_.end();
           ++attr_it) {
        const int nullable_idx = relation_.getNullableAttributeIndex(attr_it->getID());
        if ((nullable_idx != -1) && tuple_null_bitmap.getBit(nullable_idx)) {
          // Nothing to copy for null values.
          continue;
        }
        const int variable_idx = relation_.getVariableLengthAttributeIndex(attr_it->getID());
        if (variable_idx != -1) {
          // Copy into the temporary buffer.
          std::memcpy(static_cast<char*>(packed_buffer.get()) + buffer_pos,
                      static_cast<char*>(tuple_storage_)
                          + variable_length_info_array[variable_idx << 1],
                      variable_length_info_array[(variable_idx << 1) + 1]);
          // Temporarily reset the offset entry in the slot to the offset in
          // the temp buffer (will be corrected in a second pass below).
          variable_length_info_array[variable_idx << 1] = buffer_pos;
          buffer_pos += variable_length_info_array[(variable_idx << 1) + 1];
        }
      }
    }

    if (buffer_pos != 0) {
      // Copy temporary packing buffer back into block.
      const std::uint32_t variable_start = tuple_storage_bytes_ - buffer_pos;
      std::memcpy(static_cast<char*>(tuple_storage_) + variable_start,
                  packed_buffer.get(),
                  buffer_pos);
      // Correct offsets of variable-length attribute storage to point to the
      // actual location in the block.
      for (tuple_id tid = 0; tid < header_->num_tuples; ++tid) {
        void *tuple_slot = static_cast<char*>(tuple_storage_) + tid * tuple_slot_bytes_;
        std::uint32_t *variable_length_info_array = reinterpret_cast<std::uint32_t*>(
            static_cast<char*>(tuple_slot)
                + per_tuple_null_bitmap_bytes_
                + relation_.getFixedByteLength());

        // Note that we skip null checks, as doing arithmetic on meaningless
        // offsets for null values is harmless and avoids a branch.
        for (std::size_t variable_idx = 0;
             variable_idx < relation_.numVariableLengthAttributes();
             ++variable_idx) {
          variable_length_info_array[variable_idx << 1] += variable_start;
        }
      }
    }

    header_->variable_length_bytes_allocated = buffer_pos;
  }

  header_->variable_length_storage_compact = true;
}

template <bool nullable_attrs, bool variable_length_attrs>
TupleStorageSubBlock::InsertResult SplitRowStoreTupleStorageSubBlock::insertTupleImpl(
    const Tuple &tuple) {
  tuple_id pos = isPacked() ? header_->num_tuples
                            : occupancy_bitmap_->firstZero();
  if ((pos + 1) * tuple_slot_bytes_ + header_->variable_length_bytes_allocated > tuple_storage_bytes_) {
    // Early check: if tuple would cause us to run out of space without even
    // counting variable length storage, fail immediately.
    return InsertResult(-1, false);
  }

  std::size_t variable_length_value_pos = 0;

  // Calculate the required variable-length storage.
  if (variable_length_attrs) {
    std::size_t total_variable_length_bytes = 0;
    Tuple::const_iterator value_it = tuple.begin();
    CatalogRelationSchema::const_iterator attr_it = relation_.begin();
    for (; value_it != tuple.end(); ++value_it, ++attr_it) {
      if (nullable_attrs && value_it->isNull()) {
        continue;
      } else if (attr_it->getType().isVariableLength()) {
        total_variable_length_bytes += value_it->getDataSize();
      }
    }

    if (!spaceToInsert(pos, total_variable_length_bytes)) {
      return InsertResult(-1, false);
    }

    // Allocate variable-length storage.
    header_->variable_length_bytes_allocated += total_variable_length_bytes;
    variable_length_value_pos = tuple_storage_bytes_ - header_->variable_length_bytes_allocated;
  }

  // Locate the slot and its sub-structures.
  void *tuple_slot = static_cast<char*>(tuple_storage_) + pos * tuple_slot_bytes_;
  BitVector<true> tuple_null_bitmap(tuple_slot,
                                    relation_.numNullableAttributes());
  tuple_null_bitmap.clear();
  char *fixed_length_attr_storage = static_cast<char*>(tuple_slot) + per_tuple_null_bitmap_bytes_;
  std::uint32_t *variable_length_info_array
      = variable_length_attrs ? reinterpret_cast<std::uint32_t*>(
                                    fixed_length_attr_storage + relation_.getFixedByteLength())
                              : nullptr;

  Tuple::const_iterator value_it = tuple.begin();
  CatalogRelationSchema::const_iterator attr_it = relation_.begin();
  for (; value_it != tuple.end(); ++value_it, ++attr_it) {
    const attribute_id attr_id = attr_it->getID();
    if (nullable_attrs) {
      // Set null bit if needed.
      const int nullable_idx = relation_.getNullableAttributeIndex(attr_id);
      if ((nullable_idx != -1) && value_it->isNull()) {
        tuple_null_bitmap.setBit(nullable_idx, true);
        continue;
      }
    }
    if (variable_length_attrs) {
      const int variable_length_idx = relation_.getVariableLengthAttributeIndex(attr_id);
      if (variable_length_idx != -1) {
        // If attribute is variable-length, point to it and record its size
        // in the slot, then copy it into the variable-length storage region.
        const std::size_t data_size = value_it->getDataSize();

        variable_length_info_array[variable_length_idx << 1] = variable_length_value_pos;
        variable_length_info_array[(variable_length_idx << 1) + 1] = data_size;

        value_it->copyInto(static_cast<char*>(tuple_storage_) + variable_length_value_pos);
        variable_length_value_pos += data_size;
        continue;
      }
    }
    // Not null or variable-length, so copy directly into slot.
    value_it->copyInto(fixed_length_attr_storage + relation_.getFixedLengthAttributeOffset(attr_id));
  }

  ++(header_->num_tuples);
  if (pos > header_->max_tid) {
    header_->max_tid = pos;
  }
  occupancy_bitmap_->setBit(pos, true);

  return InsertResult(pos, false);
}

// Copy groups are used by insert algorithms to efficiently copy attributes from a
// variety of source schemas with some matching attributes in the destination (this) store.
// SplitRow has 3 distinct zones which define a physical tuple:
//    [null_bitmap] [fixed_length_zone] [var_len_pairs]
// When we do our insert algorithm, we first copy over fixed length attributes. Since there
// can be gaps, and reorderings in the source schema, we need to know:
//    * Where to copy the src attr into (ie offset from start of fixed_len_zone)
//    * How many bytes to copy
//    * Which src attr we are copying
// When copying fixed length attributes, we calculate the offset into our tuple, do a memcpy for
// the length of the data with the src attribute.
//
// Copying variable length attributes pairs is similar. Note that there is a heap at the end of
// the SplitRow for actual data and the tuple contains pairs of (heap offset, length). Having to
// copy varlen into the heap is the main difference from copying fixed length.
void SplitRowStoreTupleStorageSubBlock::getCopyGroupsForAttributeMap(
  const std::vector<attribute_id> &attribute_map,
  CopyGroupList *copy_groups) {
  DCHECK_EQ(attribute_map.size(), relation_.size());

  attribute_id num_attrs = attribute_map.size();

  std::size_t contig_adv = 0;
  std::size_t varlen_adv = 0;
  for (attribute_id attr_id = 0; attr_id < num_attrs; ++attr_id) {
    attribute_id src_attr = attribute_map[attr_id];

    // Attribute doesn't exist in src.
    if (src_attr == kInvalidCatalogId) {
      // create a placeholder for now
      if (relation_.getVariableLengthAttributeIndex(attr_id) == -1) {
        // fixed len
        contig_adv += fixed_len_attr_sizes_[attr_id];
      } else {
        // var len
        varlen_adv += kVarLenSlotSize;
      }
      continue;
    }

    // Attribute exists in src.
    if (relation_.getVariableLengthAttributeIndex(attr_id) == -1) {
      // fixed len
      copy_groups->contiguous_attrs_.push_back(
        ContiguousAttrs(src_attr, fixed_len_attr_sizes_[attr_id], contig_adv));
      contig_adv = fixed_len_attr_sizes_[attr_id];
    } else {
      // var len
      copy_groups->varlen_attrs_.push_back(VarLenAttr(src_attr, attr_id, varlen_adv));
      varlen_adv = SplitRowStoreTupleStorageSubBlock::kVarLenSlotSize;
    }

    if (relation_.getNullableAttributeIndex(attr_id) != -1) {
      copy_groups->nullable_attrs_.push_back(
        NullableAttr(src_attr, relation_.getNullableAttributeIndex(attr_id)));
    }
  }
  // This will point us to the beginning of the varlen zone.
  if (copy_groups->varlen_attrs_.size() > 0) {
    copy_groups->varlen_attrs_[0].bytes_to_advance_ += contig_adv;
  }
}

}  // namespace quickstep
