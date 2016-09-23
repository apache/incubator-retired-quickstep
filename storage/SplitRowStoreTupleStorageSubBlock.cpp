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

namespace {

template <typename ValueAccessorT, bool nullable_attrs>
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

template <typename ValueAccessorT, bool nullable_attrs>
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

// Information related to inserting a single attribute into a table
struct AttributeInsertInfo {
  AttributeInsertInfo(bool is_variable,
                      bool is_nullable,
                      size_t fixed_length_offset,
                      size_t fixed_length_size,
                      size_t var_length_offset)
    : is_variable_(is_variable),
      is_nullable_(is_nullable),
      fixed_length_offset_(fixed_length_offset),
      fixed_length_size_(fixed_length_size),
      var_length_offset_(var_length_offset) {}

  bool is_variable_;
  bool is_nullable_;

  std::size_t fixed_length_offset_;
  std::size_t fixed_length_size_;
  std::size_t var_length_offset_;
};

// Information related to inserting a relation into a table
struct RelationInsertInfo {
  RelationInsertInfo(const CatalogRelationSchema &relation, std::size_t tuple_slot_size)
    : num_attrs_(relation.size()),
      num_nullable_attrs_(relation.numNullableAttributes()),
      max_tuple_size_bytes_(tuple_slot_size + relation.getMaximumVariableByteLength()),
      fixed_len_offset_(BitVector<true>::BytesNeeded(num_nullable_attrs_)),
      var_len_offset_(fixed_len_offset_ + relation.getFixedByteLength()),
      attribute_info_() {
    attribute_id accessor_attr_id = 0;
    for (CatalogRelationSchema::const_iterator attr_it = relation.begin();
         attr_it != relation.end();
         ++attr_it, ++accessor_attr_id) {
      DCHECK_EQ(accessor_attr_id, attr_it->getID());

      const int variable_idx = relation.getVariableLengthAttributeIndex(accessor_attr_id);
      const bool is_variable_len = variable_idx != -1;
      const bool is_nullable = relation.getNullableAttributeIndex(accessor_attr_id) != -1;

      if (is_variable_len) {
        attribute_info_.push_back(
          new AttributeInsertInfo(is_variable_len,
                                  is_nullable,
                                  0,
                                  0,
                                  relation.getVariableLengthAttributeIndex(accessor_attr_id)));
      } else {
        attribute_info_.push_back(
          new AttributeInsertInfo(is_variable_len,
                                  is_nullable,
                                  relation.getFixedLengthAttributeOffset(accessor_attr_id),
                                  relation.getAttributeById(accessor_attr_id)->getType().maximumByteLength(),
                                  -1));
      }
    }
  }

  ~RelationInsertInfo() {
    for (attribute_id attr_info_idx = 0;
        attr_info_idx < static_cast<attribute_id>(attribute_info_.size());
        ++attr_info_idx) {
      delete attribute_info_[attr_info_idx];
    }
  }

  std::size_t num_attrs_;
  std::size_t num_nullable_attrs_;
  std::size_t max_tuple_size_bytes_;

  // byte offset from the beginning of a tuple to the first fixed length attribute
  std::uint32_t fixed_len_offset_;
  // byte offset from the beginning of a tuple to the first variable length offset/length pair
  std::uint32_t var_len_offset_;

  std::vector<AttributeInsertInfo*> attribute_info_;
};


}  // namespace

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
      header_(static_cast<Header*>(sub_block_memory)) {
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
  return bulkInsertTuplesWithRemappedAttributes(simple_remap, accessor);
}

tuple_id SplitRowStoreTupleStorageSubBlock::bulkInsertTuplesWithRemappedAttributes(
    const std::vector<attribute_id> &attribute_map,
    ValueAccessor *accessor) {
  DCHECK_EQ(relation_.size(), attribute_map.size());
  const tuple_id original_num_tuples = header_->num_tuples;

  RelationInsertInfo insert_info(relation_, tuple_slot_bytes_);

  // only append to the end of the block
  tuple_id pos = header_->num_tuples;
  // the minimum number of tuples possible to append
  int append_lower_bound = 0;
  void *tuple_slot = static_cast<char *>(tuple_storage_) + pos * tuple_slot_bytes_;
  BitVector<true> tuple_null_bitmap(tuple_slot, insert_info.num_nullable_attrs_);

  // Start writing variable-length data at the beginning of the newly allocated range.
  std::size_t current_variable_position = tuple_storage_bytes_ - header_->variable_length_bytes_allocated;

  InvokeOnAnyValueAccessor(
    accessor,
    [&](auto *accessor) -> void {  // NOLINT(build/c++11
      while (accessor->next()) {
        // Recalculate the lower bound for number of tuples to append.
        if (append_lower_bound == 0) {
          std::size_t remaining_storage_bytes = tuple_storage_bytes_ -
                                          (header_->variable_length_bytes_allocated +
                                          ((header_->max_tid + 1) * tuple_slot_bytes_));
          DCHECK_LE(static_cast<std::size_t>(0), remaining_storage_bytes);
          append_lower_bound = remaining_storage_bytes/insert_info.max_tuple_size_bytes_;

          // We have run out of space to append.
          // TODO(marc) We may want to go into a 'fill in the gaps' mode here if there are deleted tuples.
          // TODO(marc) We may also want to check the available space against the actual tuples size versus the
          //            estimated maximum size.
         if (append_lower_bound == 0) {
            accessor->previous();
            break;
          }
        }

        tuple_null_bitmap.setMemory(tuple_slot);
        tuple_null_bitmap.clear();
        char *fixed_length_attr_storage = static_cast<char *>(tuple_slot) + insert_info.fixed_len_offset_;
        std::uint32_t *variable_length_info_array =
          reinterpret_cast<std::uint32_t *>(static_cast<char *>(tuple_slot) + insert_info.var_len_offset_);


        std::uint32_t current_null_idx = 0;
        for (attribute_id accessor_attr_id = 0;
             accessor_attr_id < static_cast<attribute_id>(insert_info.num_attrs_);
             ++accessor_attr_id) {
          const AttributeInsertInfo &attribute_insert_info = *insert_info.attribute_info_[accessor_attr_id];
          bool nullable = attribute_insert_info.is_nullable_;
          bool variable = attribute_insert_info.is_variable_;

          if (!nullable && !variable) {
            DCHECK_EQ(-1, relation_.getNullableAttributeIndex(accessor_attr_id));

            const void *attr_value = accessor->template getUntypedValue<false>(attribute_map[accessor_attr_id]);
            std::memcpy(fixed_length_attr_storage + attribute_insert_info.fixed_length_offset_,
                        attr_value,
                        attribute_insert_info.fixed_length_size_);
          } else if (nullable && !variable) {
            DCHECK_EQ(relation_.getNullableAttributeIndex(accessor_attr_id), static_cast<int>(current_null_idx));

            const void *attr_value = accessor->getUntypedValue(attribute_map[accessor_attr_id]);
            if (attr_value == nullptr) {
              tuple_null_bitmap.setBit(current_null_idx, true);
            } else {
              std::memcpy(fixed_length_attr_storage + attribute_insert_info.fixed_length_offset_,
                          attr_value,
                          attribute_insert_info.fixed_length_size_);
            }
            current_null_idx++;
          } else if (!nullable && variable) {
            // TODO(marc) this TypedValue can be replaced by a call for a pointer + length once that is implemented
            // in ValueAccessor
            TypedValue attr_value(accessor->getTypedValue(attribute_map[accessor_attr_id]));

            DCHECK_EQ(-1, relation_.getNullableAttributeIndex(accessor_attr_id));
            DCHECK_EQ(static_cast<int>(attribute_insert_info.var_length_offset_),
                      relation_.getVariableLengthAttributeIndex(accessor_attr_id));
            DCHECK(!attr_value.isNull());

            const std::size_t attr_size = attr_value.getDataSize();

            DCHECK_GE(current_variable_position, attr_size);

            current_variable_position -= attr_size;
            const int var_len_info_idx = attribute_insert_info.var_length_offset_ * 2;
            variable_length_info_array[var_len_info_idx] = current_variable_position;
            variable_length_info_array[var_len_info_idx + 1] = attr_size;
            std::memcpy(static_cast<char *>(tuple_storage_) + current_variable_position,
                        attr_value.getDataPtr(),
                        attr_size);

            header_->variable_length_bytes_allocated += attr_size;
          } else {  // nullable, variable length
            DCHECK_EQ(static_cast<int>(current_null_idx), relation_.getNullableAttributeIndex(accessor_attr_id));

            TypedValue attr_value(accessor->getTypedValue(attribute_map[accessor_attr_id]));
            if (attr_value.isNull()) {
              tuple_null_bitmap.setBit(current_null_idx, true);
            } else {
              DCHECK_EQ(relation_.getVariableLengthAttributeIndex(accessor_attr_id),
                        static_cast<int>(attribute_insert_info.var_length_offset_));

              const std::size_t attr_size = attr_value.getDataSize();

              DCHECK_GE(current_variable_position, attr_size);

              current_variable_position -= attr_size;
              const int var_len_info_idx = attribute_insert_info.var_length_offset_ * 2;
              variable_length_info_array[var_len_info_idx] = current_variable_position;
              variable_length_info_array[var_len_info_idx + 1] = attr_size;
              std::memcpy(static_cast<char *>(tuple_storage_) + current_variable_position,
                          attr_value.getDataPtr(),
                          attr_size);
              header_->variable_length_bytes_allocated += attr_size;
            }
            current_null_idx++;
          }
        }

        occupancy_bitmap_->setBit(pos, true);
        header_->num_tuples += 1;
        header_->max_tid = pos;
        append_lower_bound -= 1;
        pos += 1;

        tuple_slot = static_cast<char *>(tuple_slot) + tuple_slot_bytes_;
        DCHECK_EQ(static_cast<char *>(tuple_storage_) + pos * tuple_slot_bytes_, static_cast<char *>(tuple_slot));
      }
    });
  return header_->num_tuples - original_num_tuples;
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

}  // namespace quickstep
