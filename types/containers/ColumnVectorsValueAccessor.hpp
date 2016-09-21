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

#ifndef QUICKSTEP_TYPES_CONTAINERS_COLUMN_VECTORS_VALUE_ACCESSOR_HPP_
#define QUICKSTEP_TYPES_CONTAINERS_COLUMN_VECTORS_VALUE_ACCESSOR_HPP_

#include <cstddef>
#include <limits>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/ValueAccessor.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/containers/Tuple.hpp"
#include "utility/Macros.hpp"
#include "utility/ScopedDeleter.hpp"

#include "glog/logging.h"

namespace quickstep {

class TupleIdSequence;

/**
 * @brief Implementation of ValueAccessor as a group of equal-length
 *        ColumnVectors (one per "attribute").
 **/
class ColumnVectorsValueAccessor : public ValueAccessor {
 public:
  ColumnVectorsValueAccessor()
      : column_length_(0),
        current_position_(std::numeric_limits<std::size_t>::max()) {
  }

  ~ColumnVectorsValueAccessor() override {
  }

  Implementation getImplementationType() const override {
    return Implementation::kColumnVectors;
  }

  bool isTupleIdSequenceAdapter() const override {
    return false;
  }

  bool isOrderedTupleIdSequenceAdapter() const override {
    return false;
  }

  /**
   * @brief Adds a column vector as an attribute to the value-accessor.
   *
   * @param column Column vector added to this value-accessor.
   * @param owns Whether this value-accessor owns this column vector. If true,
   *             this value-accessor is responsible for freeing this column
   *             vector.
   **/
  void addColumn(ColumnVector *column, const bool owns = true) {
    // If this is not the first column to be added, make sure it is the same
    // length as the others.
    DCHECK(columns_.empty()
           || (column->isNative()
               ? (static_cast<const NativeColumnVector*>(column)->size() == column_length_)
               : (static_cast<const IndirectColumnVector*>(column)->size() == column_length_)));
    columns_.push_back(column);
    column_native_.push_back(column->isNative());
    if (owns) {
      deleter_.addObject(column);
    }
    column_length_
        = column->isNative()
          ? static_cast<const NativeColumnVector*>(column)->size()
          : static_cast<const IndirectColumnVector*>(column)->size();
  }

  inline void beginIteration() {
    current_position_ = std::numeric_limits<std::size_t>::max();
  }

  inline bool iterationFinished() const {
    return current_position_ + 1 >= column_length_;
  }

  inline bool next() {
    ++current_position_;
    return current_position_ < column_length_;
  }

  inline void previous() {
    --current_position_;
  }

  inline tuple_id getCurrentPosition() const {
    return current_position_;
  }

  inline tuple_id getEndPosition() const {
    return column_length_;
  }

  inline tuple_id getNumTuples() const {
    return column_length_;
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
    DCHECK(attributeIdInRange(attr_id));
    DCHECK(tupleIdInRange(tid));
    if (column_native_[attr_id]) {
      return static_cast<const NativeColumnVector&>(*columns_[attr_id]).getUntypedValue<check_null>(tid);
    } else {
      return static_cast<const IndirectColumnVector&>(*columns_[attr_id]).getUntypedValue<check_null>(tid);
    }
  }

  inline TypedValue getTypedValueAtAbsolutePosition(const attribute_id attr_id,
                                                    const tuple_id tid) const {
    DCHECK(attributeIdInRange(attr_id));
    DCHECK(tupleIdInRange(tid));
    if (column_native_[attr_id]) {
      return static_cast<const NativeColumnVector&>(*columns_[attr_id])
          .getTypedValue(tid)
          .makeReferenceToThis();
    } else {
      return static_cast<const IndirectColumnVector&>(*columns_[attr_id])
          .getTypedValue(tid)
          .makeReferenceToThis();
    }
  }

  inline Tuple* getTuple() const {
    return getTupleAtAbsolutePosition(current_position_);
  }

  inline Tuple* getTupleWithAttributes(const std::vector<attribute_id> &attributes) const {
    return getTupleAtAbsolutePositionWithAttributes(current_position_, attributes);
  }

  inline Tuple* getTupleAtAbsolutePosition(const tuple_id tid) const {
    DCHECK(tupleIdInRange(tid));
    std::vector<TypedValue> values;
    for (attribute_id attr_id = 0;
         attr_id < static_cast<attribute_id>(columns_.size());
         ++attr_id) {
      values.emplace_back(getTypedValueAtAbsolutePosition(attr_id, tid));
    }
    return new Tuple(std::move(values));
  }

  inline Tuple* getTupleAtAbsolutePositionWithAttributes(
      const tuple_id tid,
      const std::vector<attribute_id> &attributes) const {
    DCHECK(tupleIdInRange(tid));
    std::vector<TypedValue> values;
    for (const attribute_id attr_id : attributes) {
      values.emplace_back(getTypedValueAtAbsolutePosition(attr_id, tid));
    }
    return new Tuple(std::move(values));
  }

  inline TupleIdSequenceAdapterValueAccessor<ColumnVectorsValueAccessor>*
      createSharedTupleIdSequenceAdapter(const TupleIdSequence &id_sequence) {
    return new TupleIdSequenceAdapterValueAccessor<ColumnVectorsValueAccessor>(
        this,
        id_sequence,
        false);
  }

  inline OrderedTupleIdSequenceAdapterValueAccessor<ColumnVectorsValueAccessor>*
  createSharedOrderedTupleIdSequenceAdapter(const OrderedTupleIdSequence& id_sequence) {
    return new OrderedTupleIdSequenceAdapterValueAccessor<ColumnVectorsValueAccessor>(this, id_sequence, false);
  }

  inline const TupleIdSequence* getTupleIdSequence() const {
    return nullptr;
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

  const TupleIdSequence* getTupleIdSequenceVirtual() const override {
    return getTupleIdSequence();
  }

 private:
  inline bool tupleIdInRange(const tuple_id tid) const {
    return (tid >= 0) && (static_cast<std::size_t>(tid) < column_length_);
  }

  inline bool attributeIdInRange(const attribute_id attr_id) const {
    return (attr_id >= 0)
           && (static_cast<std::vector<ColumnVector*>::size_type>(attr_id) < columns_.size());
  }

  std::vector<ColumnVector*> columns_;
  std::vector<bool> column_native_;
  std::size_t column_length_;
  std::size_t current_position_;
  ScopedDeleter deleter_;

  DISALLOW_COPY_AND_ASSIGN(ColumnVectorsValueAccessor);
};

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_CONTAINERS_COLUMN_VECTORS_VALUE_ACCESSOR_HPP_
