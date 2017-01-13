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

#ifndef QUICKSTEP_STORAGE_SPLIT_ROW_STORE_VALUE_ACCESSOR_HPP_
#define QUICKSTEP_STORAGE_SPLIT_ROW_STORE_VALUE_ACCESSOR_HPP_

#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/ValueAccessor.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/Tuple.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class TupleIdSequence;
class TupleStorageSubBlock;

class SplitRowStoreValueAccessor : public ValueAccessor {
 public:
  ~SplitRowStoreValueAccessor() override {
  }

  Implementation getImplementationType() const override {
    return Implementation::kSplitRowStore;
  }

  bool isTupleIdSequenceAdapter() const override {
    return false;
  }

  bool isOrderedTupleIdSequenceAdapter() const override {
    return false;
  }

  inline void beginIteration() {
    current_position_ = std::numeric_limits<std::size_t>::max();
  }

  inline bool iterationFinished() const {
    return (current_position_ == std::numeric_limits<std::size_t>::max())
           ? (num_tuples_ == 0)
           : (current_position_ >= static_cast<std::size_t>(max_tid_));
  }

  inline bool next() {
    ++current_position_;
    if (current_position_ >= static_cast<std::size_t>((max_tid_ + 1))) {
      return false;
    }
    current_position_ = occupancy_bitmap_.firstOne(current_position_);
    return current_position_ <= static_cast<std::size_t>(max_tid_);
  }

  inline void previous() {
    current_position_ = occupancy_bitmap_.lastOne(current_position_);
    if (current_position_ == occupancy_bitmap_.size()) {
      current_position_ = std::numeric_limits<std::size_t>::max();
    }
  }

  inline tuple_id getCurrentPosition() const {
    return current_position_;
  }

  inline tuple_id getEndPosition() const {
    return max_tid_ + 1;
  }

  inline tuple_id getNumTuples() const {
    return num_tuples_;
  }

  /**
   * @brief Get a pointer to a ColumnAccessor object that provides a fast strided memory
   *        access on the underlying storage block.
   * @note The ownership of the returned object lies with the caller.
   * @warning This method should only be called if isColumnAccessorSupported() method
   *          returned true. If ColumnAccessor is not supported this method will return a nullptr.
   *
   * @param attr_id The attribute id on which this ColumnAccessor will be created.
   *
   * @return A pointer to a ColumnAccessor object with specific properties set that can be used
   *         in a tight loop iterations over the underlying storage block.
   **/
  template <bool check_null = true>
  inline const ColumnAccessor<check_null>* getColumnAccessor(const attribute_id attr_id) const {
    // Column Accessors are currently unsupported for this value accessor, hence nullptr.
    return nullptr;
  }

  template <bool check_null = true>
  inline const void* getUntypedValue(const attribute_id attr_id) const {
    return getUntypedValueAtAbsolutePosition<check_null>(attr_id, current_position_);
  }

  inline TypedValue getTypedValue(const attribute_id attr_id) const {
    return getTypedValueAtAbsolutePosition(attr_id, current_position_);
  }

  template <bool check_null = true>
  inline const void* getUntypedValueAtAbsolutePosition(const attribute_id attr_id,
                                                       const tuple_id tid) const {
    DEBUG_ASSERT(occupancy_bitmap_.getBit(tid));
    DEBUG_ASSERT(relation_.hasAttributeWithId(attr_id));
    const char *tuple_slot = static_cast<const char*>(tuple_storage_)
                             + tuple_slot_bytes_ * tid;
    if (check_null) {
      const int nullable_idx = relation_.getNullableAttributeIndex(attr_id);
      if (nullable_idx != -1) {
        // const_cast is safe here. We will only be using read-only methods of
        // BitVector.
        BitVector<true> tuple_null_bitmap(const_cast<void*>(static_cast<const void*>(tuple_slot)),
                                          relation_.numNullableAttributes());
        if (tuple_null_bitmap.getBit(nullable_idx)) {
          return nullptr;
        }
      }
    }

    const int variable_length_idx = relation_.getVariableLengthAttributeIndex(attr_id);
    if (variable_length_idx == -1) {
      // Fixed-length, stored in-line in slot.
      return tuple_slot
             + per_tuple_null_bitmap_bytes_
             + relation_.getFixedLengthAttributeOffset(attr_id);
    } else {
      // Variable-length, stored at back of block.
      const std::uint32_t pos = *reinterpret_cast<const std::uint32_t*>(
          tuple_slot + per_tuple_null_bitmap_bytes_
                     + relation_.getFixedByteLength()
                     + variable_length_idx * 2 * sizeof(std::uint32_t));
      return static_cast<const char*>(tuple_storage_) + pos;
    }
  }

  inline TypedValue getTypedValueAtAbsolutePosition(const attribute_id attr_id,
                                                    const tuple_id tid) const {
    DEBUG_ASSERT(occupancy_bitmap_.getBit(tid));
    DEBUG_ASSERT(relation_.hasAttributeWithId(attr_id));
    const Type &attr_type = relation_.getAttributeById(attr_id)->getType();
    const char *tuple_slot = static_cast<const char*>(tuple_storage_)
                             + tuple_slot_bytes_ * tid;
    const int nullable_idx = relation_.getNullableAttributeIndex(attr_id);
    if (nullable_idx != -1) {
      // const_cast is safe here. We will only be using read-only methods of
      // BitVector.
      BitVector<true> tuple_null_bitmap(const_cast<void*>(static_cast<const void*>(tuple_slot)),
                                        relation_.numNullableAttributes());
      if (tuple_null_bitmap.getBit(nullable_idx)) {
        return attr_type.makeNullValue();
      }
    }

    const int variable_length_idx = relation_.getVariableLengthAttributeIndex(attr_id);
    if (variable_length_idx == -1) {
      // Fixed-length, stored in-line in slot.
      return attr_type.makeValue(
          tuple_slot + per_tuple_null_bitmap_bytes_ + relation_.getFixedLengthAttributeOffset(attr_id),
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

  inline Tuple* getTuple() const {
    return getTupleAtAbsolutePosition(current_position_);
  }

  inline Tuple* getTupleWithAttributes(const std::vector<attribute_id> &attributes) const {
    return getTupleAtAbsolutePositionWithAttributes(current_position_, attributes);
  }

  inline Tuple* getTupleAtAbsolutePosition(const tuple_id tid) const {
    std::vector<TypedValue> values;
    if (relation_.gapsInAttributeSequence()) {
      for (const CatalogAttribute &attr : relation_) {
        values.emplace_back(getTypedValueAtAbsolutePosition(attr.getID(), tid));
      }
    } else {
      const attribute_id max_attr_id = relation_.getMaxAttributeId();
      for (attribute_id attr_id = 0;
           attr_id <= max_attr_id;
           ++attr_id) {
        values.emplace_back(getTypedValueAtAbsolutePosition(attr_id, tid));
      }
    }
    return new Tuple(std::move(values));
  }

  inline Tuple* getTupleAtAbsolutePositionWithAttributes(
      const tuple_id tid,
      const std::vector<attribute_id> &attributes) const {
    std::vector<TypedValue> values;
    for (const attribute_id attr_id : attributes) {
      values.emplace_back(getTypedValueAtAbsolutePosition(attr_id, tid));
    }
    return new Tuple(std::move(values));
  }

  inline TupleIdSequenceAdapterValueAccessor<SplitRowStoreValueAccessor>*
      createSharedTupleIdSequenceAdapter(const TupleIdSequence &id_sequence) {
    return new TupleIdSequenceAdapterValueAccessor<SplitRowStoreValueAccessor>(
        this,
        id_sequence,
        false);
  }

  inline OrderedTupleIdSequenceAdapterValueAccessor<SplitRowStoreValueAccessor>
      *createSharedOrderedTupleIdSequenceAdapter(const OrderedTupleIdSequence &id_sequence) {
    return new OrderedTupleIdSequenceAdapterValueAccessor<SplitRowStoreValueAccessor>(this, id_sequence, false);
  }

  inline const TupleIdSequence* getTupleIdSequence() const {
    if (num_tuples_ == (max_tid_ + 1)) {
      return nullptr;
    } else {
      if (!tuple_id_sequence_) {
        tuple_id_sequence_.reset(new TupleIdSequence(max_tid_ + 1, occupancy_bitmap_));
      }
      return tuple_id_sequence_.get();
    }
  }

  // Implementation of virtuals that pass-through to the inline
  // implementations.
  void beginIterationVirtual() override {
    beginIteration();
  }

  bool iterationFinishedVirtual() const override {
    return iterationFinished();
  }

  bool nextVirtual() override {
    return next();
  }

  void previousVirtual() override {
    previous();
  }

  tuple_id getCurrentPositionVirtual() const override {
    return getCurrentPosition();
  }

  tuple_id getEndPositionVirtual() const override {
    return getEndPosition();
  }

  tuple_id getNumTuplesVirtual() const override {
    return getNumTuples();
  }

  const void* getUntypedValueVirtual(const attribute_id attr_id) const override {
    return getUntypedValue(attr_id);
  }

  TypedValue getTypedValueVirtual(const attribute_id attr_id) const override {
    return getTypedValue(attr_id);
  }

  const void* getUntypedValueAtAbsolutePositionVirtual(
      const attribute_id attr_id,
      const tuple_id tid) const override {
    return getUntypedValueAtAbsolutePosition(attr_id, tid);
  }

  TypedValue getTypedValueAtAbsolutePositionVirtual(
      const attribute_id attr_id,
      const tuple_id tid) const override {
    return getTypedValueAtAbsolutePosition(attr_id, tid);
  }

  Tuple* getTupleVirtual() const override {
    return getTuple();
  }

  Tuple* getTupleWithAttributesVirtual(
      const std::vector<attribute_id> &attributes) const override {
    return getTupleWithAttributes(attributes);
  }

  ValueAccessor* createSharedTupleIdSequenceAdapterVirtual(
      const TupleIdSequence &id_sequence) override {
    return createSharedTupleIdSequenceAdapter(id_sequence);
  }

  ValueAccessor* createSharedOrderedTupleIdSequenceAdapterVirtual(
      const OrderedTupleIdSequence &id_sequence) override {
    return createSharedOrderedTupleIdSequenceAdapter(id_sequence);
  }

  const TupleIdSequence* getTupleIdSequenceVirtual() const override {
    return getTupleIdSequence();
  }

 private:
  SplitRowStoreValueAccessor(const CatalogRelationSchema &relation,
                             const tuple_id num_tuples,
                             const tuple_id max_tid,
                             const BitVector<false> &occupancy_bitmap,
                             const void *tuple_storage,
                             const std::size_t tuple_slot_bytes,
                             const std::size_t per_tuple_null_bitmap_bytes)
      : relation_(relation),
        num_tuples_(num_tuples),
        max_tid_(max_tid),
        occupancy_bitmap_(occupancy_bitmap),
        tuple_storage_(tuple_storage),
        tuple_slot_bytes_(tuple_slot_bytes),
        per_tuple_null_bitmap_bytes_(per_tuple_null_bitmap_bytes),
        current_position_(std::numeric_limits<std::size_t>::max()) {
  }

  const CatalogRelationSchema &relation_;
  const tuple_id num_tuples_;
  const tuple_id max_tid_;
  const BitVector<false> &occupancy_bitmap_;
  const void *tuple_storage_;
  const std::size_t tuple_slot_bytes_;
  const std::size_t per_tuple_null_bitmap_bytes_;

  std::size_t current_position_;

  // Initialized from 'occupancy_bitmap_' on-demand.
  mutable std::unique_ptr<TupleIdSequence> tuple_id_sequence_;

  friend class SplitRowStoreTupleStorageSubBlock;

  DISALLOW_COPY_AND_ASSIGN(SplitRowStoreValueAccessor);
};

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_SPLIT_ROW_STORE_VALUE_ACCESSOR_HPP_
