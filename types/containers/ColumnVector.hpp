/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_TYPES_CONTAINERS_COLUMN_VECTOR_HPP_
#define QUICKSTEP_TYPES_CONTAINERS_COLUMN_VECTOR_HPP_

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <utility>
#include <vector>

#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

// TODO(chasseur): Look into ways to allocate ColumnVector memory from the
// StorageManager.

/**
 * @brief A vector of values of the same type. Two implementations exist:
 *        NativeColumnVector (an array of fixed-size data elements) and
 *        IndirectColumnVector (a vector of TypedValues, which may point to
 *        variably-sized or out-of-line data).
 * @note This abstract base class is mostly useless by itself. Client code
 *       should check isNative() and then cast to either NativeColumnVector or
 *       IndirectColumnVector to use a ColumnVector.
 * @note To help control the combinatorial explosion of possible code paths due
 *       to different ColumnVector implementations, the convention is that
 *       NativeColumnVector will ALWAYS be used where possible (i.e. for any
 *       Type where NativeColumnVector::UsableForType() is true).
 *       IndirectColumnVector will ONLY be used for types where that method is
 *       false (i.e. only for variable-length types). Another important aspect
 *       of this convention is that memcpy/memmove may be used directly on
 *       fixed-size values from a NativeColumnVector (which is safe) while the
 *       TypedValues in an IndirectColumnVector may trim out-of-line storage
 *       for a CHAR(X) value, for instance, to a smaller number of bytes needed
 *       to store the string up to a null-terminator byte (in which case the
 *       TypedValue's copyInto() method is still safe, but using memcpy/memmove
 *       with the Type's actual length may not be).
 **/
class ColumnVector {
 public:
  /**
   * @brief Constructor.
   *
   * @param type The Type of values to hold.
   **/
  explicit ColumnVector(const Type &type)
      : type_(type) {
  }

  /**
   * @brief Virtual destructor.
   **/
  virtual ~ColumnVector() {
  }

  /**
   * @brief Create a ColumnVector consisting entirely of copies of a single
   *        value.
   *
   * @param value_type The type of the values to create.
   * @param value The literal value to fill the result ColumnVector with.
   * @param num_copies The number of copies of value to fill the result
   *        ColumnVector with.
   * @return A ColumnVector of the appropriate type consisting of num_copies
   *         copies of value.
   **/
  static ColumnVector* MakeVectorOfValue(
      const Type &value_type,
      const TypedValue &value,
      const std::size_t num_copies);

  /**
   * @brief Check whether this ColumnVector is a NativeColumnVector or an
   *        IndirectColumnVector.
   *
   * @return true if this is a NativeColumnVector, false if this is an
   *         IndirectColumnVector.
   **/
  virtual bool isNative() const = 0;

 protected:
  const Type &type_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ColumnVector);
};

/**
 * @brief An implementation of ColumnVector as an array of native values.
 **/
class NativeColumnVector : public ColumnVector {
 public:
  /**
   * @brief Constructor for a NativeColumnVector which owns its own array of
   *        values.
   *
   * @param type The Type of values to hold. Must be fixed-length.
   * @param reserved_length The maximum number of values that this
   *        NativeColumnVector will hold.
   **/
  NativeColumnVector(const Type &type, const std::size_t reserved_length)
      : ColumnVector(type),
        type_length_(type.maximumByteLength()),
        values_(std::malloc(type.maximumByteLength() * reserved_length)),
        reserved_length_(reserved_length),
        actual_length_(0u),
        null_bitmap_(type.isNullable() ? new BitVector<false>(reserved_length) : nullptr) {
    DCHECK(UsableForType(type_));
    if (null_bitmap_) {
      null_bitmap_->clear();
    }
  }

  /**
   * @brief Destructor. Frees value array if owned.
   **/
  ~NativeColumnVector() override {
    std::free(values_);
  }

  /**
   * @brief Determine if NativeColumnVector is usable for a particular Type.
   *
   * @param type A Type to check if whether NativeColumnVector is usable for.
   * @return true if NativeColumnVector can be used for type, false if
   *         IndirectColumnVector must be used instead.
   **/
  static bool UsableForType(const Type &type) {
    return !type.isVariableLength();
  }

  bool isNative() const override {
    return true;
  }

  /**
   * @brief Determine if this NativeColumnVector's Type is nullable.
   *
   * @return Whether this NativeColumnVector's Type is nullable.
   **/
  inline bool typeIsNullable() const {
    return null_bitmap_.get() != nullptr;
  }

  /**
   * @brief Get the number of values in this NativeColumnVector.
   *
   * @return The number of values in this NativeColumnVector.
   **/
  inline std::size_t size() const {
    return actual_length_;
  }

  /**
   * @brief Get an untyped pointer to a value in this NativeColumnVector.
   *
   * @param position The position of the value to get.
   * @return An untyped pointer to the value at position.
   **/
  template <bool check_null = true>
  inline const void* getUntypedValue(const std::size_t position) const {
    DCHECK_LT(position, actual_length_);
    return (check_null && null_bitmap_ && null_bitmap_->getBit(position))
        ? nullptr
        : static_cast<const char*>(values_) + (position * type_length_);
  }

  /**
   * @brief Get a value in this NativeColumnVector as a TypedValue.
   *
   * @param position The position of the value to get.
   * @return The value at position.
   **/
  inline TypedValue getTypedValue(const std::size_t position) const {
    DCHECK_LT(position, actual_length_);
    return (null_bitmap_ && null_bitmap_->getBit(position))
        ? type_.makeNullValue()
        : type_.makeValue(static_cast<const char*>(values_) + (position * type_length_),
                          type_length_);
  }

  /**
   * @brief Append an untyped value to this NativeColumnVector.
   * @warning Appending a new value must not cause the number of values in this
   *          NativeColumnVector to exceed the reserved length supplied to the
   *          constructor.
   * @warning Do not use this with NULL values. Use appendNullValue() instead.
   *
   * @param value A pointer to an untyped value to append to this
   *        NativeColumnVector.
   **/
  inline void appendUntypedValue(const void *value) {
    DCHECK_LT(actual_length_, reserved_length_);
    std::memcpy((static_cast<char*>(values_) + (actual_length_ * type_length_)),
                value,
                type_length_);
    ++actual_length_;
  }

  /**
   * @brief Append a NULL value to this NativeColumnVector.
   * @warning Appending a new value must not cause the number of values in this
   *          NativeColumnVector to exceed the reserved length supplied to the
   *          constructor.
   **/
  inline void appendNullValue() {
    DCHECK_LT(actual_length_, reserved_length_);
    DCHECK(null_bitmap_);
    null_bitmap_->setBit(actual_length_, true);
    ++actual_length_;
  }

  /**
   * @brief Append a TypedValue to this NativeColumnVector.
   *
   * @param value A value to append to this NativeColumnVector.
   **/
  inline void appendTypedValue(const TypedValue &value) {
    DCHECK_LT(actual_length_, reserved_length_);
    DCHECK(value.isPlausibleInstanceOf(type_.getSignature()));
    if (null_bitmap_ && value.isNull()) {
      null_bitmap_->setBit(actual_length_, true);
    } else {
      DCHECK(!value.isNull());
      value.copyInto(static_cast<char*>(values_) + (actual_length_ * type_length_));
    }
    ++actual_length_;
  }

  /**
   * @brief Get a pointer to internal storage that can be used to directly
   *        write an untyped value into the next position of this
   *        NativeColumnVector.
   * @warning Be very careful with the return pointer. Do not write beyond the
   *          Type's fixed size bytes.
   *
   * @return A pointer to the next value position in this NativeColumnVector.
   **/
  inline void* getPtrForDirectWrite() {
    DCHECK_LT(actual_length_, reserved_length_);
    return static_cast<char*>(values_) + (actual_length_++ * type_length_);
  }

  /**
   * @brief Fill this entire ColumnVector with NULLs.
   **/
  inline void fillWithNulls() {
    DCHECK(null_bitmap_);
    null_bitmap_->setBitRange(0, reserved_length_, true);
    actual_length_ = reserved_length_;
  }

  /**
   * @brief Fill this entire ColumnVector with copies of value.
   *
   * @param value A value to fill this ColumnVector with.
   **/
  inline void fillWithValue(const TypedValue &value) {
    DCHECK(value.isPlausibleInstanceOf(type_.getSignature()));
    if (value.isNull()) {
      fillWithNulls();
    } else {
      if (null_bitmap_) {
        null_bitmap_->clear();
      }
      for (std::size_t pos = 0;
           pos < reserved_length_;
           ++pos) {
        value.copyInto(static_cast<char*>(values_) + (pos * type_length_));
      }
      actual_length_ = reserved_length_;
    }
  }

  /**
   * @brief Prepare this NativeColumnVector for positional writes. Effectively
   *        "fills" this NativeColumnVector with uninitialized values that will
   *        be overwritten with calls to positionalWriteUntypedValue(),
   *        positionalWriteNullValue(), and/or positionalWriteTypedValue().
   * @warning Do NOT use positional writes in combination with appends.
   **/
  inline void prepareForPositionalWrites() {
    DCHECK_EQ(0u, actual_length_);
    actual_length_ = reserved_length_;
  }

  /**
   * @brief Overwrite the value at the specified position with the supplied
   *        untyped value.
   * @warning Do not use this with NULL values. Use positionalWriteNullValue()
   *          instead.
   * @warning You must call prepareForPositionalWrites() BEFORE calling this
   *          method.
   * @warning Do NOT use positional writes in combination with appends.
   * @warning It is intended that this and other positional write methods
   *          should be called exactly once for each position (if this is
   *          violated, NULLs may not be tracked properly).
   *
   * @param position The position of the value in this NativeColumnVector to
   *        overwrite.
   * @param value A pointer to an untyped value to write into this
   *        NativeColumnVector.
   **/
  inline void positionalWriteUntypedValue(const std::size_t position,
                                          const void *value) {
    DCHECK_LT(position, actual_length_);
    DCHECK(value != nullptr);
    std::memcpy((static_cast<char*>(values_) + (position * type_length_)),
                value,
                type_length_);
  }

  /**
   * @brief Overwrite the value at the specified position with a NULL value.
   * @warning You must call prepareForPositionalWrites() BEFORE calling this
   *          method.
   * @warning Do NOT use positional writes in combination with appends.
   * @warning It is intended that this and other positional write methods
   *          should be called exactly once for each position (if this is
   *          violated, NULLs may not be tracked properly).
   *
   * @param position The position of the value in this NativeColumnVector to
   *        overwrite with a NULL value.
   **/
  inline void positionalWriteNullValue(const std::size_t position) {
    DCHECK_LT(position, actual_length_);
    DCHECK(null_bitmap_);
    null_bitmap_->setBit(position, true);
  }

  /**
   * @brief Overwrite the value at the specified position with the supplied
   *        TypedValue.
   * @warning You must call prepareForPositionalWrites() BEFORE calling this
   *          method.
   * @warning Do NOT use positional writes in combination with appends.
   * @warning It is intended that this and other positional write methods
   *          should be called exactly once for each position (if this is
   *          violated, NULLs may not be tracked properly).
   *
   * @param position The position of the value in this NativeColumnVector to
   *        overwrite.
   * @param value A TypedValue to write into this NativeColumnVector.
   **/
  inline void positionalWriteTypedValue(const std::size_t position,
                                        const TypedValue &value) {
    DCHECK_LT(position, actual_length_);
    DCHECK(value.isPlausibleInstanceOf(type_.getSignature()));
    if (null_bitmap_ && value.isNull()) {
      null_bitmap_->setBit(position, true);
    } else {
      DCHECK(!value.isNull());
      value.copyInto(static_cast<char*>(values_) + (position * type_length_));
    }
  }

 private:
  const std::size_t type_length_;
  void *values_;
  const std::size_t reserved_length_;
  std::size_t actual_length_;
  std::unique_ptr<BitVector<false>> null_bitmap_;

  DISALLOW_COPY_AND_ASSIGN(NativeColumnVector);
};

/**
 * @brief An implementation of ColumnVector as a vector of TypedValues. Unlike
 *        NativeColumnVector, this can handle variable-length types.
 **/
class IndirectColumnVector : public ColumnVector {
 public:
  /**
   * @brief Constructor.
   *
   * @param type The Type of values to hold.
   * @param reserved_length The number of values to reserve space for.
   **/
  IndirectColumnVector(const Type &type, const std::size_t reserved_length)
      : ColumnVector(type),
        type_is_nullable_(type.isNullable()),
        reserved_length_(reserved_length) {
    values_.reserve(reserved_length);
  }

  /**
   * @brief Destructor.
   **/
  ~IndirectColumnVector() override {
  }

  bool isNative() const override {
    return false;
  }

  /**
   * @brief Determine if this IndirectColumnVector's Type is nullable.
   *
   * @return Whether this IndirectColumnVector's Type is nullable.
   **/
  inline bool typeIsNullable() const {
    return type_is_nullable_;
  }

  /**
   * @brief Get the number of values in this IndirectColumnVector.
   *
   * @return The number of values in this IndirectColumnVector.
   **/
  inline std::size_t size() const {
    return values_.size();
  }

  /**
   * @brief Get an untyped pointer to a value in this IndirectColumnVector.
   *
   * @param position The position of the value to get.
   * @return An untyped pointer to the value at position.
   **/
  template <bool check_null = true>
  inline const void* getUntypedValue(const std::size_t position) const {
    DCHECK_LT(position, values_.size());
    return (check_null && type_is_nullable_ && values_[position].isNull())
        ? nullptr
        : values_[position].getDataPtr();
  }

  /**
   * @brief Get a value in this IndirectColumnVector as a TypedValue.
   *
   * @param position The position of the value to get.
   * @return The value at position.
   **/
  inline const TypedValue& getTypedValue(const std::size_t position) const {
    DCHECK_LT(position, values_.size());
    return values_[position];
  }

  /**
   * @brief Destructively move a TypedValue out of this IndirectColumnVector.
   *
   * @param position The position of the value to get.
   * @return The value at position (original will be moved-from).
   **/
  inline TypedValue moveTypedValue(const std::size_t position) {
    DCHECK_LT(position, values_.size());
    return std::move(values_[position]);
  }

  /**
   * @brief Append a TypedValue to this IndirectColumnVector.
   *
   * @param value A value to append to this NativeColumnVector.
   **/
  inline void appendTypedValue(const TypedValue &value) {
    DCHECK(value.isPlausibleInstanceOf(type_.getSignature()));
    DCHECK_LT(values_.size(), reserved_length_);
    values_.emplace_back(value);
  }

  /**
   * @brief Append a TypedValue to this IndirectColumnVector.
   *
   * @param value A value to append to this NativeColumnVector.
   **/
  inline void appendTypedValue(TypedValue &&value) {
    DCHECK(value.isPlausibleInstanceOf(type_.getSignature()));
    DCHECK_LT(values_.size(), reserved_length_);
    values_.emplace_back(std::move(value));
  }

  /**
   * @brief Fill this entire ColumnVector with copies of value.
   *
   * @param value A value to fill this ColumnVector with.
   **/
  inline void fillWithValue(const TypedValue &value) {
    DCHECK(value.isPlausibleInstanceOf(type_.getSignature()));
    values_.assign(reserved_length_, value);
  }

  /**
   * @brief Prepare this IndirectColumnVector for positional writes.
   *        Effectively "fills" this IndirectColumnVector with uninitialized
   *        values that will be overwritten with calls to
   *        positionalWriteTypedValue().
   * @warning Do NOT use positional writes in combination with appends.
   **/
  inline void prepareForPositionalWrites() {
    DCHECK(values_.empty());
    values_.resize(reserved_length_);
  }

  /**
   * @brief Overwrite the value at the specified position with the supplied
   *        TypedValue.
   * @warning You must call prepareForPositionalWrites() BEFORE calling this
   *          method.
   * @warning Do NOT use positional writes in combination with appends.
   *
   * @param position The position of the value in this IndirectColumnVector to
   *        overwrite.
   * @param value A TypedValue to write into this IndirectColumnVector.
   **/
  inline void positionalWriteTypedValue(const std::size_t position,
                                        const TypedValue &value) {
    DCHECK(value.isPlausibleInstanceOf(type_.getSignature()));
    DCHECK_LT(position, values_.size());
    values_[position] = value;
  }

  /**
   * @brief Overwrite the value at the specified position with the supplied
   *        TypedValue.
   * @warning You must call prepareForPositionalWrites() BEFORE calling this
   *          method.
   * @warning Do NOT use positional writes in combination with appends.
   *
   * @param position The position of the value in this IndirectColumnVector to
   *        overwrite.
   * @param value A TypedValue to write into this IndirectColumnVector.
   **/
  inline void positionalWriteTypedValue(const std::size_t position,
                                        TypedValue &&value) {  // NOLINT(whitespace/operators)
    DCHECK(value.isPlausibleInstanceOf(type_.getSignature()));
    DCHECK_LT(position, values_.size());
    values_[position] = std::move(value);
  }

 private:
  const bool type_is_nullable_;
  const std::size_t reserved_length_;
  std::vector<TypedValue> values_;

  DISALLOW_COPY_AND_ASSIGN(IndirectColumnVector);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_CONTAINERS_COLUMN_VECTOR_HPP_
