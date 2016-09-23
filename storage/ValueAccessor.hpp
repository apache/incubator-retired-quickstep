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

#ifndef QUICKSTEP_STORAGE_VALUE_ACCESSOR_HPP_
#define QUICKSTEP_STORAGE_VALUE_ACCESSOR_HPP_

#include <algorithm>
#include <cstdint>
#include <limits>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/TupleIdSequence.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/Tuple.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class TupleStorageSubBlock;

template <bool check_null = true>
class ColumnAccessor;

// TODO(chasseur): Iteration on ValueAccessors is row-at-a-time, but in some
// cases column-wise data movement may be more efficient.

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief Base class for accessors which provide inline-able methods allowing
 *        direct access to attribute values in TupleStorageSubBlocks or
 *        ColumnVectors.
 * @note This base class provides several virtual methods named with the suffix
 *       -Virtual that constitute a common interface that all ValueAccessors
 *       support. Concrete implementation subclasses of ValueAccessor all
 *       provide non-virtual inline versions of these methods without the
 *       -Virtual suffix. The general recommendation is that if client code
 *       only needs to make one or two calls to methods of a ValueAccessor, it
 *       can just use the virtual methods and avoid generating many code
 *       branches for different types of ValueAccessors. If, on the other hand,
 *       methods of a ValueAccessor will be called many times repeatedly (e.g.
 *       in an inner loop), it is recommended to downcast a ValueAccessor to
 *       its concrete type and use the inline methods. The file
 *       ValueAccessorUtil.hpp provides macros to do this automatically.
 **/
class ValueAccessor {
 public:
  /**
   * @brief Enum with cases for different subclasses of ValueAccessor.
   **/
  enum class Implementation {
    // Accessors for values inside a TupleStorageSubBlock:
    kBasicColumnStore,
    kCompressedColumnStore,
    kCompressedPackedRowStore,
    kPackedRowStore,
    kSplitRowStore,
    // Accessor for a group of ColumnVectors:
    kColumnVectors
  };

  ValueAccessor() {
  }

  virtual ~ValueAccessor() {
  }

  /**
   * @brief Determine the concrete type of this ValueAccessor.
   * @note See also isTupleIdSequenceAdapter() and
   *       isOrderedTupleIdSequenceAdapter(), which check if a templated
   *       adapter is being used.
   *
   * @return The implementation type of this ValueAccessor.
   **/
  virtual Implementation getImplementationType() const = 0;

  /**
   * @brief Determine if this ValueAccessor is a
   *        TupleIdSequenceAdapterValueAccessor.
   *
   * @return true if this is a TupleIdSequenceAdapterValueAccessor, false
   *         otherwise.
   **/
  virtual bool isTupleIdSequenceAdapter() const = 0;

  /**
   * @brief Determine if this ValueAccessor is an
   *        OrderedTupleIdSequenceAdapterValueAccessor.
   *
   * @return true if thhis is an OrderedTupleIdSequenceAdapterValueAccessor,
   *         false otherwise.
   **/
  virtual bool isOrderedTupleIdSequenceAdapter() const = 0;

  /**
   * @brief Start statefully iterating over tuples from this ValueAccessor.
   * @note Conceptually, this method sets the "position" of the ValueAccessor
   *       to one-before the start of tuples. nextVirtual()/next() should be
   *       called before attempting to read values.
   **/
  virtual void beginIterationVirtual() = 0;

  /**
   * @brief Determine whether the stateful iteration over tuples in this
   *        ValueAccessor is finished (i.e. there are no more tuples to iterate
   *        over).
   *
   * @return true if iteration is finished (i.e. a subsequent call to
   *         nextVirtual()/next() will return false). Note that this returns
   *         true if iteration is past-the-end of tuples OR on the very last
   *         tuple.
   **/
  virtual bool iterationFinishedVirtual() const = 0;

  /**
   * @brief Advance stateful iteration to the next tuple from this
   *        ValueAccessor.
   *
   * @return true if advanced to another tuple, false if there are no more
   *         tuples.
   **/
  virtual bool nextVirtual() = 0;

  /**
   * @brief Rewind stateful iteration by one tuple (i.e. undo the last call to
   *        nextVirtual()/next()).
   **/
  virtual void previousVirtual() = 0;

  /**
   * @brief Get the current absolute position of stateful iteration on this
   *        ValueAccessor (i.e. the tuple_id of the corresponding tuple in a
   *        TupleStorageSubBlock).
   *
   * @return The absolute position of the current tuple in stateful iteration.
   **/
  virtual tuple_id getCurrentPositionVirtual() const = 0;

  /**
   * @brief Get the "end" absolute position of tuples accessible via this
   *        ValueAccessor (i.e. the absolute position of the last tuple plus
   *        one).
   *
   * @return The end position of tuples accessible via this ValueAccessor.
   **/
  virtual tuple_id getEndPositionVirtual() const = 0;

  /**
   * @brief Get the actual number of tuples accessible via this ValueAccessor.
   * @note This is different from getEndPositionVirtual()/getEndPosition(),
   *       because there may be gaps in the sequence of tuple positions that a
   *       ValueAccessor iterates over (e.g. because a
   *       TupleIdSequenceAdapterValueAccessor is being used, or because the
   *       base TupleStorageSubBlock (e.g. SplitRowStoreTupleStorageSubBlock)
   *       allows empty tuple slots).
   *
   * @return The actual number of tuples accessible via this ValueAccessor.
   **/
  virtual tuple_id getNumTuplesVirtual() const = 0;

  /**
   * @brief Returns whether this accessor has a fast strided ColumnAccessor available
   *        that can be used to optimize memory access in a tight loop iteration
   *        over the underlying storage block. Specific derived classes should override
   *        this method if they support ColumnAccessor.
   *
   * @return true if fast ColumnAccessor is supported, otherwise false.
   */
  virtual inline bool isColumnAccessorSupported() const {
    return false;
  }

  /**
   * @brief Get a pointer to an untyped value for the current tuple in stateful
   *        iteration.
   * @note The inline version of this method provided by subclasses,
   *       getUntypedValue(), takes an optional bool template parameter
   *       indicating whether checks for null-values should be performed. If it
   *       is known that the Type for an attribute is not nullable, using false
   *       for that template parameter may result in a faster version of the
   *       method that avoids a branch to check for nulls.
   * @warning This method should only be called if the previous call to
   *          nextVirtual()/next() return true.
   *
   * @param attr_id The attribute_id indicating which attribute's value to get
   *        from the current tuple.
   * @return An untyped pointer to the attribute value specified by attr_id for
   *         the current tuple.
   **/
  virtual const void* getUntypedValueVirtual(const attribute_id attr_id) const = 0;

  /**
   * @brief Get a TypedValue for the current tuple in stateful iteration.
   * @warning This method should only be called if the previous call to
   *          nextVirtual()/next() return true.
   *
   * @param attr_id The attribute_id indicating which attribute's value to get
   *        from the current tuple.
   * @return The attribute value specified by attr_id for the current tuple.
   **/
  virtual TypedValue getTypedValueVirtual(const attribute_id attr_id) const = 0;

  /**
   * @brief Get a pointer to an untyped value for the tuple at an explicitly
   *        specified absolute position.
   * @note The inline version of this method provided by subclasses,
   *       getUntypedValueAtAbsolutePosition(), takes an optional bool template
   *       parameter indicating whether checks for null-values should be
   *       performed. If it is known that the Type for an attribute is not
   *       nullable, using false for that template parameter may result in a
   *       faster version of the method that avoids a branch to check for
   *       nulls.
   * @warning It is the caller's responsbility to ensure that there actually is
   *          a tuple at the specified position.
   *
   * @param attr_id The attribute_id indicating which attribute's value to get.
   * @param tid The absolute position of the tuple to get a value from.
   * @return An untyped pointer to the attribute value specified by attr_id in
   *         the tuple at the position specified by tid.
   **/
  virtual const void* getUntypedValueAtAbsolutePositionVirtual(
      const attribute_id attr_id,
      const tuple_id tid) const = 0;

  /**
   * @brief Get a TypedValue for the tuple at an explicitly specified absolute
   *        position.
   * @warning It is the caller's responsbility to ensure that there actually is
   *          a tuple at the specified position.
   *
   * @param attr_id The attribute_id indicating which attribute's value to get.
   * @param tid The absolute position of the tuple to get a value from.
   * @return The attribute value specified by attr_id in the tuple at the
   *         position specified by tid.
   **/
  virtual TypedValue getTypedValueAtAbsolutePositionVirtual(
      const attribute_id attr_id,
      const tuple_id tid) const = 0;

  /**
   * @brief Create a Tuple object containing all the attribute values for the
   *        current tuple in stateful iteration.
   * @warning This method should only be called if the previous call to
   *          nextVirtual()/next() return true.
   *
   * @return A new Tuple object (caller takes ownership) containing all the
   *         attribute values for the current tuple in stateful iteration.
   **/
  virtual Tuple* getTupleVirtual() const = 0;

  /**
   * @brief Create a Tuple object containing values of explicitly-specified
   *        attributes in a specified order for the current tuple in stateful
   *        iteration.
   * @warning This method should only be called if the previous call to
   *          nextVirtual()/next() return true.
   *
   * @param attributes The IDs of attributes, in the desired order, to fill
   *        the returned Tuple with.
   * @return A new Tuple object (caller takes ownership) containing the
   *         specified attribute values for the current tuple in stateful
   *         iteration.
   **/
  virtual Tuple* getTupleWithAttributesVirtual(
      const std::vector<attribute_id> &attributes) const = 0;

  /**
   * @brief Create a new TupleIdSequenceAdapterValueAccessor that wraps this
   *        ValueAccessor.
   * @warning The newly-created adapter does NOT take ownership of this
   *          ValueAccessor nor the provided TupleIdSequence. Both must remain
   *          valid so long as the adapter will be used.
   *
   * @param id_sequence A TupleIdSequence specifying some subset of the tuples
   *        for this ValueAccessor that the adapter will iterate over.
   * @return A new TupleIdSequenceAdapterValueAccessor that will iterate over
   *         only the tuples specified in id_sequence.
   **/
  virtual ValueAccessor* createSharedTupleIdSequenceAdapterVirtual(
      const TupleIdSequence &id_sequence) = 0;

  /**
   * @brief Get a TupleIdSequence indicating which positions this ValueAccessor
   *        is iterating over.
   *
   * @return A pointer to a TupleIdSequence that indicates which absolute
   *         positions this ValueAccessor iterates over. Returns NULL if there
   *         are no gaps in the iteration sequence (i.e. iteration goes from
   *         position 0 to getEndPositionVirtual() - 1 without skipping
   *         anything).
   **/
  virtual const TupleIdSequence* getTupleIdSequenceVirtual() const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(ValueAccessor);
};

template <typename InternalValueAccessorType>
class OrderedTupleIdSequenceAdapterValueAccessor;

/**
 * @brief Adapter for another ValueAccessor which only iterates over tuples in
 *        a TupleIdSequence.
 **/
template <typename InternalValueAccessorType>
class TupleIdSequenceAdapterValueAccessor : public ValueAccessor {
 public:
  TupleIdSequenceAdapterValueAccessor(InternalValueAccessorType *accessor,
                                      const TupleIdSequence &id_sequence,
                                      const bool take_ownership_of_accessor = true)
      : accessor_(accessor),
        owned_accessor_(take_ownership_of_accessor ? accessor : nullptr),
        id_sequence_(id_sequence),
        current_position_(id_sequence.before_begin()) {
  }

  ~TupleIdSequenceAdapterValueAccessor() override {
  }

  Implementation getImplementationType() const override {
    return accessor_->getImplementationType();
  }

  bool isTupleIdSequenceAdapter() const override {
    return true;
  }

  bool isOrderedTupleIdSequenceAdapter() const override {
    return false;
  }

  inline void beginIteration() {
    current_position_ = id_sequence_.before_begin();
  }

  inline bool iterationFinished() const {
    TupleIdSequence::const_iterator next_position = current_position_;
    ++next_position;
    return (current_position_ == id_sequence_.end())
           || (next_position == id_sequence_.end());
  }

  inline bool next() {
    ++current_position_;
    return current_position_ != id_sequence_.end();
  }

  inline void previous() {
    --current_position_;
  }

  inline tuple_id getCurrentPosition() const {
    return *current_position_;
  }

  inline tuple_id getEndPosition() const {
    return id_sequence_.length();
  }

  inline tuple_id getNumTuples() const {
    return id_sequence_.numTuples();
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
    // Column Accessors are currently unsupported for this accessor, hence nullptr.
    return nullptr;
  }

  template <bool check_null = true>
  inline const void* getUntypedValue(const attribute_id attr_id) const {
    return accessor_->template getUntypedValueAtAbsolutePosition<check_null>(attr_id, *current_position_);
  }

  inline TypedValue getTypedValue(const attribute_id attr_id) const {
    return accessor_->getTypedValueAtAbsolutePosition(attr_id, *current_position_);
  }

  // Pass-through.
  template <bool check_null = true>
  inline const void* getUntypedValueAtAbsolutePosition(const attribute_id attr_id,
                                                       const tuple_id tid) const {
    return accessor_->template getUntypedValueAtAbsolutePosition<check_null>(attr_id, tid);
  }

  // Pass-through.
  inline TypedValue getTypedValueAtAbsolutePosition(const attribute_id attr_id,
                                                    const tuple_id tid) const {
    return accessor_->getTypedValueAtAbsolutePosition(attr_id, tid);
  }

  inline Tuple* getTuple() const {
    return accessor_->getTupleAtAbsolutePosition(*current_position_);
  }

  inline Tuple* getTupleWithAttributes(const std::vector<attribute_id> &attributes) const {
    return accessor_->getTupleAtAbsolutePositionWithAttributes(*current_position_, attributes);
  }

  inline TupleIdSequenceAdapterValueAccessor<InternalValueAccessorType> *createSharedTupleIdSequenceAdapter(
      const TupleIdSequence &id_sequence) {
    return new TupleIdSequenceAdapterValueAccessor<InternalValueAccessorType>(accessor_, id_sequence, false);
  }

  inline OrderedTupleIdSequenceAdapterValueAccessor<InternalValueAccessorType>
      *createSharedOrderedTupleIdSequenceAdapter(const OrderedTupleIdSequence &id_sequence) {
    return new OrderedTupleIdSequenceAdapterValueAccessor<InternalValueAccessorType>(accessor_, id_sequence, false);
  }

  inline const TupleIdSequence* getTupleIdSequence() const {
    return &id_sequence_;
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
  InternalValueAccessorType *accessor_;
  std::unique_ptr<InternalValueAccessorType> owned_accessor_;
  const TupleIdSequence &id_sequence_;
  TupleIdSequence::const_iterator current_position_;

  DISALLOW_COPY_AND_ASSIGN(TupleIdSequenceAdapterValueAccessor);
};

/**
 * @brief Adapter for another ValueAccessor which only iterates over tuples in
 *        an OrderedTupleIdSequence.
 **/
template <typename InternalValueAccessorType>
class OrderedTupleIdSequenceAdapterValueAccessor : public ValueAccessor {
 public:
  OrderedTupleIdSequenceAdapterValueAccessor(InternalValueAccessorType *accessor,
                                             const OrderedTupleIdSequence &id_sequence,
                                             const bool take_ownership_of_accessor = true)
      : accessor_(accessor),
        owned_accessor_(take_ownership_of_accessor ? accessor : nullptr),
        id_sequence_(id_sequence),
        current_position_(std::numeric_limits<OrderedTupleIdSequence::size_type>::max()) {
    static_assert(std::numeric_limits<OrderedTupleIdSequence::size_type>::is_modulo,
                  "size_type does not support modulo overflow.");
  }

  ~OrderedTupleIdSequenceAdapterValueAccessor() {
  }

  Implementation getImplementationType() const override {
    return accessor_->getImplementationType();
  }

  bool isTupleIdSequenceAdapter() const override {
    return false;
  }

  bool isOrderedTupleIdSequenceAdapter() const override {
    return true;
  }

  inline void beginIteration() {
    current_position_ = std::numeric_limits<OrderedTupleIdSequence::size_type>::max();
  }

  inline bool iterationFinished() const {
    return current_position_ + 1 >= id_sequence_.size();
  }

  inline bool next() {
    ++current_position_;
    return current_position_ != id_sequence_.size();
  }

  inline void previous() {
    --current_position_;
  }

  inline tuple_id getCurrentPosition() const {
    return id_sequence_[current_position_];
  }

  inline tuple_id getEndPosition() const {
    OrderedTupleIdSequence::const_iterator max_it = std::max_element(id_sequence_.begin(), id_sequence_.end());
    return (max_it != id_sequence_.end() ? *max_it : 0) + 1;
  }

  inline tuple_id getNumTuples() const {
    return id_sequence_.size();
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
    // Column Accessors are currently unsupported for this accessor, hence nullptr.
    return nullptr;
  }

  template <bool check_null = true>
  inline const void* getUntypedValue(const attribute_id attr_id) const {
    return accessor_->template getUntypedValueAtAbsolutePosition<check_null>(attr_id,
                                                                             id_sequence_[current_position_]);
  }

  inline TypedValue getTypedValue(const attribute_id attr_id) const {
    return accessor_->getTypedValueAtAbsolutePosition(attr_id, id_sequence_[current_position_]);
  }

  template <bool check_null = true>
  inline const void* getUntypedValueAtAbsolutePosition(const attribute_id attr_id,
                                                       const tuple_id tid) const {
    FATAL_ERROR("getUntypedValueAtAbsolutePosition() not implemented in "
                "OrderedTupleIdSequenceAdapterValueAccessor");
  }

  inline TypedValue getTypedValueAtAbsolutePosition(const attribute_id attr_id,
                                                    const tuple_id tid) const {
    FATAL_ERROR("getTypedValueAtAbsolutePosition() not implemented in "
                "OrderedTupleIdSequenceAdapterValueAccessor");
  }

  inline Tuple* getTuple() const {
    return accessor_->getTupleAtAbsolutePosition(id_sequence_[current_position_]);
  }

  inline Tuple* getTupleWithAttributes(const std::vector<attribute_id> &attributes) const {
    return accessor_->getTupleAtAbsolutePositionWithAttributes(id_sequence_[current_position_], attributes);
  }

  inline TupleIdSequenceAdapterValueAccessor<InternalValueAccessorType>*
      createSharedTupleIdSequenceAdapter(const TupleIdSequence &id_sequence) {
    return new TupleIdSequenceAdapterValueAccessor<InternalValueAccessorType>(accessor_,
                                                                              id_sequence,
                                                                              false);
  }

  inline OrderedTupleIdSequenceAdapterValueAccessor<InternalValueAccessorType>
      *createSharedOrderedTupleIdSequenceAdapter(const OrderedTupleIdSequence &id_sequence) {
    return new OrderedTupleIdSequenceAdapterValueAccessor<InternalValueAccessorType>(accessor_, id_sequence, false);
  }

  inline const TupleIdSequence* getTupleIdSequence() const {
    FATAL_ERROR("getTupleIdSequence() not implemented in "
                "OrderedTupleIdSequenceAdapterValueAccessor");
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
  InternalValueAccessorType *accessor_;
  std::unique_ptr<InternalValueAccessorType> owned_accessor_;
  const OrderedTupleIdSequence &id_sequence_;
  OrderedTupleIdSequence::size_type current_position_;

  DISALLOW_COPY_AND_ASSIGN(OrderedTupleIdSequenceAdapterValueAccessor);
};

/**
 * @brief Templated implementation of ValueAccessor for any packed
 *        TupleStorageSubBlock.
 **/
template <typename TupleStorageSubBlockT,
          typename HelperT,
          ValueAccessor::Implementation impl_type>
class PackedTupleStorageSubBlockValueAccessor : public ValueAccessor {
 public:
  ~PackedTupleStorageSubBlockValueAccessor() override {
  }

  Implementation getImplementationType() const override {
    return impl_type;
  }

  bool isTupleIdSequenceAdapter() const override {
    return false;
  }

  bool isOrderedTupleIdSequenceAdapter() const override {
    return false;
  }

  inline void beginIteration() {
    current_tuple_ = -1;
  }

  inline bool iterationFinished() const {
    return current_tuple_ + 1 >= helper_.numPackedTuples();
  }

  inline bool next() {
    ++current_tuple_;
    return (current_tuple_ < helper_.numPackedTuples());
  }

  inline void previous() {
    --current_tuple_;
  }

  inline tuple_id getCurrentPosition() const {
    return current_tuple_;
  }

  inline tuple_id getEndPosition() const {
    return helper_.numPackedTuples();
  }

  inline tuple_id getNumTuples() const {
    return helper_.numPackedTuples();
  }

  template <bool check_null = true>
  inline const void* getUntypedValue(const attribute_id attr_id) const {
    return getUntypedValueAtAbsolutePosition<check_null>(attr_id, current_tuple_);
  }

  inline TypedValue getTypedValue(const attribute_id attr_id) const {
    return getTypedValueAtAbsolutePosition(attr_id, current_tuple_);
  }

  inline bool isColumnAccessorSupported() const override {
    return helper_.isColumnAccessorSupported();
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
    return helper_.template getColumnAccessor<check_null>(current_tuple_, attr_id);
  }

  template <bool check_null = true>
  inline const void* getUntypedValueAtAbsolutePosition(const attribute_id attr_id,
                                                       const tuple_id tid) const {
    return helper_.template getAttributeValue<check_null>(tid, attr_id);
  }

  inline TypedValue getTypedValueAtAbsolutePosition(const attribute_id attr_id,
                                                    const tuple_id tid) const {
    return helper_.getAttributeValueTyped(tid, attr_id);
  }

  inline Tuple* getTuple() const {
    return getTupleAtAbsolutePosition(current_tuple_);
  }

  inline Tuple* getTupleWithAttributes(const std::vector<attribute_id> &attributes) const {
    return getTupleAtAbsolutePositionWithAttributes(current_tuple_, attributes);
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

  inline TupleIdSequenceAdapterValueAccessor<PackedTupleStorageSubBlockValueAccessor>*
      createSharedTupleIdSequenceAdapter(const TupleIdSequence &id_sequence) {
    return new TupleIdSequenceAdapterValueAccessor<PackedTupleStorageSubBlockValueAccessor>(
        this,
        id_sequence,
        false);
  }

  inline OrderedTupleIdSequenceAdapterValueAccessor<PackedTupleStorageSubBlockValueAccessor>*
      createSharedOrderedTupleIdSequenceAdapter(const OrderedTupleIdSequence &id_sequence) {
    return new OrderedTupleIdSequenceAdapterValueAccessor<PackedTupleStorageSubBlockValueAccessor>(
        this,
        id_sequence,
        false);
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
  template <typename... HelperArgs>
  PackedTupleStorageSubBlockValueAccessor(
      const CatalogRelationSchema &relation,
      HelperArgs&&... helper_args)
      : relation_(relation),
        helper_(std::forward<HelperArgs>(helper_args)...),
        current_tuple_(-1) {
  }

  const CatalogRelationSchema &relation_;
  HelperT helper_;
  tuple_id current_tuple_;

  friend TupleStorageSubBlockT;

  DISALLOW_COPY_AND_ASSIGN(PackedTupleStorageSubBlockValueAccessor);
};


/**
 * @brief ColumnAccessor is a helper template class that is used to optimize memory
 *        access patterns for a ValueAccessor when it is used in a tight loop
 *        to extract values for a given attribute from a given storage block.
 **/
template <bool check_null>
class ColumnAccessor {
 public:
  /**
   * @brief Constructor.
   *
   * @param current_tuple_position A constant reference to the tuple position in the containing
   *        ValueAccessor. This reference value is shared between the containing ValueAccessor &
   *        a ColumnAccessor. However, a ColumnAccessor *CANNOT* modify this tuple position.
   * @param num_tuples Number of tuples for this block.
   * @param base_address The starting address in memory for the first column value.
   * @param stride The memory offset at which other column values will be found.
   * @param null_bitmap The bitmap that will be referred in case of nullable attributes.
   * @param nullable_base The starting index for the first nullable attribute in the bitmap.
   *        Note that setting this value to -1 will essentially cause null checks to always
   *        return false.
   * @param nullable_stride The offset at which null bits will be found for
   *        different attribute values.
   **/
  ColumnAccessor(const tuple_id &current_tuple_position,
                 const std::size_t num_tuples,
                 const void *base_address,
                 const std::size_t stride,
                 const BitVector<false> *null_bitmap = nullptr,
                 const int nullable_base = -1,
                 const unsigned nullable_stride = 0)
      : current_tuple_position_(current_tuple_position),
        num_tuples_(num_tuples),
        base_address_(base_address),
        stride_(stride),
        null_bitmap_(null_bitmap),
        nullable_base_(nullable_base),
        nullable_stride_(nullable_stride) {
  }

  /**
   * @brief Get a pointer to an untyped value for the current tuple in stateful
   *        iteration over the given column.
   *
   * @return An untyped pointer to the attribute value for the current tuple.
   **/
  inline const void* getUntypedValue() const {
    DCHECK(current_tuple_position_ < num_tuples_);
    if (check_null) {
      DCHECK(null_bitmap_ != nullptr);
      if ((nullable_base_ != -1)
          && null_bitmap_->getBit(current_tuple_position_ * nullable_stride_ + nullable_base_)) {
        return nullptr;
      }
    }
    return static_cast<const char*>(base_address_) + current_tuple_position_ * stride_;
  }

 private:
  const tuple_id &current_tuple_position_;
  const tuple_id num_tuples_;
  const void *base_address_;
  const std::size_t stride_;
  const BitVector<false> *null_bitmap_;
  const int nullable_base_;
  const unsigned nullable_stride_;

  DISALLOW_COPY_AND_ASSIGN(ColumnAccessor);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_VALUE_ACCESSOR_HPP_
