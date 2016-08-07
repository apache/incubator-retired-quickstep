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

#include "compression/CompressionDictionary.hpp"

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <limits>
#include <utility>

#include "compression/CompressionDictionaryLite.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "types/operations/comparisons/ComparisonUtil.hpp"
#include "types/operations/comparisons/EqualComparison.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

using std::numeric_limits;
using std::pair;
using std::size_t;
using std::uint32_t;

namespace quickstep {

namespace compression_dictionary_internal {

template <bool variable_length>
class CompressionDictionaryIterator : public std::iterator<std::random_access_iterator_tag, const void*> {
 public:
  typedef std::iterator<std::random_access_iterator_tag, const void*>::difference_type difference_type;

  CompressionDictionaryIterator(const CompressionDictionary &dictionary, const uint32_t code)
      : dictionary_(&dictionary),
        code_(code) {
  }

  CompressionDictionaryIterator()
      : dictionary_(nullptr),
        code_(0) {
  }

  CompressionDictionaryIterator(const CompressionDictionaryIterator &other)
      : dictionary_(other.dictionary_),
        code_(other.code_) {
  }

  CompressionDictionaryIterator& operator=(const CompressionDictionaryIterator& other) = default;

  // Comparisons.
  inline bool operator==(const CompressionDictionaryIterator& other) const {
    DCHECK_EQ(dictionary_, other.dictionary_);
    return code_ == other.code_;
  }

  inline bool operator!=(const CompressionDictionaryIterator& other) const {
    DCHECK_EQ(dictionary_, other.dictionary_);
    return code_ != other.code_;
  }

  inline bool operator<(const CompressionDictionaryIterator& other) const {
    DCHECK_EQ(dictionary_, other.dictionary_);
    return code_ < other.code_;
  }

  inline bool operator<=(const CompressionDictionaryIterator& other) const {
    DCHECK_EQ(dictionary_, other.dictionary_);
    return code_ <= other.code_;
  }

  inline bool operator>(const CompressionDictionaryIterator& other) const {
    DCHECK_EQ(dictionary_, other.dictionary_);
    return code_ > other.code_;
  }

  inline bool operator>=(const CompressionDictionaryIterator& other) const {
    DCHECK_EQ(dictionary_, other.dictionary_);
    return code_ >= other.code_;
  }

  // Increment/decrement.
  inline CompressionDictionaryIterator& operator++() {
    ++code_;
    return *this;
  }

  CompressionDictionaryIterator operator++(int) {
    CompressionDictionaryIterator result(*this);
    ++(*this);
    return result;
  }

  inline CompressionDictionaryIterator& operator--() {
    --code_;
    return *this;
  }

  CompressionDictionaryIterator operator--(int) {
    CompressionDictionaryIterator result(*this);
    --(*this);
    return result;
  }

  // Compound assignment.
  inline CompressionDictionaryIterator& operator+=(difference_type n) {
    code_ += n;
    return *this;
  }

  inline CompressionDictionaryIterator& operator-=(difference_type n) {
    code_ -= n;
    return *this;
  }

  // Note: + operator with difference_type on the left is not defined.
  CompressionDictionaryIterator operator+(difference_type n) const {
    return CompressionDictionaryIterator(dictionary_, code_ + n);
  }

  CompressionDictionaryIterator operator-(difference_type n) const {
    return CompressionDictionaryIterator(dictionary_, code_ - n);
  }

  difference_type operator-(const CompressionDictionaryIterator &other) const {
    DCHECK_EQ(dictionary_, other.dictionary_);
    return code_ - other.code_;
  }

  // Dereference.
  inline const void* operator*() const {
    DCHECK(dictionary_ != nullptr);
    if (variable_length) {
      return dictionary_->variableLengthGetUntypedValueHelper<std::uint32_t>(code_);
    } else {
      return dictionary_->fixedLengthGetUntypedValueHelper<std::uint32_t>(code_);
    }
  }

  inline const void** operator->() const {
    FATAL_ERROR("-> dereference operator unimplemented for CompressionDictionaryIterator.");
  }

  const void* operator[](difference_type n) const {
    DCHECK(dictionary_ != nullptr);
    if (variable_length) {
      return dictionary_->variableLengthGetUntypedValueHelper<std::uint32_t>(code_ + n);
    } else {
      return dictionary_->fixedLengthGetUntypedValueHelper<std::uint32_t>(code_ + n);
    }
  }

  uint32_t getCode() const {
    return code_;
  }

 private:
  const CompressionDictionary *dictionary_;
  uint32_t code_;
};

}  // namespace compression_dictionary_internal

using compression_dictionary_internal::CompressionDictionaryIterator;

uint32_t CompressionDictionary::getCodeForUntypedValue(const void *value) const {
  if (value == nullptr) {
    return getNullCode() == numeric_limits<uint32_t>::max() ? number_of_codes_including_null_
                                                            : getNullCode();
  }

  uint32_t candidate_code = getLowerBoundCodeForUntypedValue(value);
  if (candidate_code >= *static_cast<const uint32_t*>(dictionary_memory_)) {
    return number_of_codes_including_null_;
  }

  if (CheckUntypedValuesEqual(type_, value, getUntypedValueForCode(candidate_code))) {
    return candidate_code;
  } else {
    return number_of_codes_including_null_;
  }
}

std::pair<uint32_t, uint32_t> CompressionDictionary::getLimitCodesForComparisonUntyped(
    const ComparisonID comp,
    const void *value) const {
  if (value == nullptr) {
    return pair<uint32_t, uint32_t>(number_of_codes_including_null_,
                                    number_of_codes_including_null_);
  }

  pair<uint32_t, uint32_t> limit_codes;
  switch (comp) {
    case ComparisonID::kEqual:
      limit_codes.first = getCodeForUntypedValue(value);
      limit_codes.second = (limit_codes.first == number_of_codes_including_null_)
                           ? limit_codes.first
                           : limit_codes.first + 1;
      break;
    case ComparisonID::kNotEqual:
      LOG(FATAL) << "Called CompressionDictionary::getLimitCodesForComparisonUntyped() "
                 << "with comparison kNotEqual, which is not allowed.";
    case ComparisonID::kLess:
      limit_codes.first = 0;
      limit_codes.second = getLowerBoundCodeForUntypedValue(value, true);
      break;
    case ComparisonID::kLessOrEqual:
      limit_codes.first = 0;
      limit_codes.second = getUpperBoundCodeForUntypedValue(value);
      break;
    case ComparisonID::kGreater:
      limit_codes.first = getUpperBoundCodeForUntypedValue(value);
      limit_codes.second = *static_cast<const uint32_t*>(dictionary_memory_);
      break;
    case ComparisonID::kGreaterOrEqual:
      limit_codes.first = getLowerBoundCodeForUntypedValue(value);
      limit_codes.second = *static_cast<const uint32_t*>(dictionary_memory_);
      break;
    default:
      LOG(FATAL) << "Unknown comparison in CompressionDictionary::getLimitCodesForComparisonUntyped().";
  }

  return limit_codes;
}

std::uint32_t CompressionDictionary::getCodeForDifferentTypedValue(const TypedValue &value,
                                                                   const Type &value_type) const {
  if (value.isNull()) {
    return getNullCode() == numeric_limits<uint32_t>::max() ? number_of_codes_including_null_
                                                            : getNullCode();
  }

  uint32_t candidate_code = getLowerBoundCodeForDifferentTypedValue(value, value_type);
  if (candidate_code >= *static_cast<const uint32_t*>(dictionary_memory_)) {
    return candidate_code;
  }

  if (EqualComparison::Instance().compareTypedValuesChecked(
          value, value_type,
          getTypedValueForCode(candidate_code), type_)) {
    return candidate_code;
  } else {
    return number_of_codes_including_null_;
  }
}

std::pair<uint32_t, uint32_t> CompressionDictionary::getLimitCodesForComparisonDifferentTyped(
    const ComparisonID comp,
    const TypedValue &value,
    const Type &value_type) const {
  if (value.isNull()) {
    return pair<uint32_t, uint32_t>(number_of_codes_including_null_,
                                    number_of_codes_including_null_);
  }

  pair<uint32_t, uint32_t> limit_codes;
  switch (comp) {
    case ComparisonID::kEqual:
      limit_codes.first = getCodeForDifferentTypedValue(value, value_type);
      limit_codes.second = (limit_codes.first == number_of_codes_including_null_)
                           ? limit_codes.first
                           : limit_codes.first + 1;
      break;
    case ComparisonID::kNotEqual:
      LOG(FATAL) << "Called CompressionDictionary::getLimitCodesForComparisonTyped() "
                 <<  "with comparison kNotEqual, which is not allowed.";
    case ComparisonID::kLess:
      limit_codes.first = 0;
      limit_codes.second = getLowerBoundCodeForDifferentTypedValue(value, value_type, true);
      break;
    case ComparisonID::kLessOrEqual:
      limit_codes.first = 0;
      limit_codes.second = getUpperBoundCodeForDifferentTypedValue(value, value_type);
      break;
    case ComparisonID::kGreater:
      limit_codes.first = getUpperBoundCodeForDifferentTypedValue(value, value_type);
      limit_codes.second = *static_cast<const uint32_t*>(dictionary_memory_);
      break;
    case ComparisonID::kGreaterOrEqual:
      limit_codes.first = getLowerBoundCodeForDifferentTypedValue(value, value_type);
      limit_codes.second = *static_cast<const uint32_t*>(dictionary_memory_);
      break;
    default:
      LOG(FATAL) << "Unknown comparison in CompressionDictionary::getLimitCodesForComparisonTyped().";
  }

  return limit_codes;
}

uint32_t CompressionDictionary::getLowerBoundCodeForUntypedValue(const void *value,
                                                                 const bool ignore_null_code) const {
  uint32_t code;
  if (type_is_variable_length_) {
    CompressionDictionaryIterator<true> begin_it(*this, 0);
    CompressionDictionaryIterator<true> end_it(*this,
                                               *static_cast<const uint32_t*>(dictionary_memory_));
    code = GetBoundForUntypedValue<CompressionDictionaryIterator<true>, LowerBoundFunctor>(
        type_,
        begin_it,
        end_it,
        value).getCode();
  } else {
    CompressionDictionaryIterator<false> begin_it(*this, 0);
    CompressionDictionaryIterator<false> end_it(*this,
                                                *static_cast<const uint32_t*>(dictionary_memory_));
    code = GetBoundForUntypedValue<CompressionDictionaryIterator<false>, LowerBoundFunctor>(
        type_,
        begin_it,
        end_it,
        value).getCode();
  }

  return !ignore_null_code && (code == *static_cast<const uint32_t*>(dictionary_memory_))
             ? number_of_codes_including_null_
             : code;
}

uint32_t CompressionDictionary::getUpperBoundCodeForUntypedValue(const void *value) const {
  if (type_is_variable_length_) {
    CompressionDictionaryIterator<true> begin_it(*this, 0);
    CompressionDictionaryIterator<true> end_it(*this,
                                               *static_cast<const uint32_t*>(dictionary_memory_));
    return GetBoundForUntypedValue<CompressionDictionaryIterator<true>, UpperBoundFunctor>(
        type_,
        begin_it,
        end_it,
        value).getCode();
  } else {
    CompressionDictionaryIterator<false> begin_it(*this, 0);
    CompressionDictionaryIterator<false> end_it(*this,
                                                *static_cast<const uint32_t*>(dictionary_memory_));
    return GetBoundForUntypedValue<CompressionDictionaryIterator<false>, UpperBoundFunctor>(
        type_,
        begin_it,
        end_it,
        value).getCode();
  }
}

uint32_t CompressionDictionary::getLowerBoundCodeForDifferentTypedValue(
    const TypedValue &value,
    const Type &value_type,
    const bool ignore_null_code) const {
  uint32_t code;
  if (type_is_variable_length_) {
    CompressionDictionaryIterator<true> begin_it(*this, 0);
    CompressionDictionaryIterator<true> end_it(*this,
                                               *static_cast<const uint32_t*>(dictionary_memory_));
    code = GetBoundForDifferentTypedValue<CompressionDictionaryIterator<true>, LowerBoundFunctor>(
        type_,
        begin_it,
        end_it,
        value,
        value_type).getCode();
  } else {
    CompressionDictionaryIterator<false> begin_it(*this, 0);
    CompressionDictionaryIterator<false> end_it(*this,
                                                *static_cast<const uint32_t*>(dictionary_memory_));
    code = GetBoundForDifferentTypedValue<CompressionDictionaryIterator<false>, LowerBoundFunctor>(
        type_,
        begin_it,
        end_it,
        value,
        value_type).getCode();
  }

  return !ignore_null_code && (code == *static_cast<const uint32_t*>(dictionary_memory_))
             ? number_of_codes_including_null_
             : code;
}

uint32_t CompressionDictionary::getUpperBoundCodeForDifferentTypedValue(
    const TypedValue &value,
    const Type &value_type) const {
  if (type_is_variable_length_) {
    CompressionDictionaryIterator<true> begin_it(*this, 0);
    CompressionDictionaryIterator<true> end_it(*this,
                                               *static_cast<const uint32_t*>(dictionary_memory_));
    return GetBoundForDifferentTypedValue<CompressionDictionaryIterator<true>, UpperBoundFunctor>(
        type_,
        begin_it,
        end_it,
        value,
        value_type).getCode();
  } else {
    CompressionDictionaryIterator<false> begin_it(*this, 0);
    CompressionDictionaryIterator<false> end_it(*this,
                                                *static_cast<const uint32_t*>(dictionary_memory_));
    return GetBoundForDifferentTypedValue<CompressionDictionaryIterator<false>, UpperBoundFunctor>(
        type_,
        begin_it,
        end_it,
        value,
        value_type).getCode();
  }
}

}  // namespace quickstep
