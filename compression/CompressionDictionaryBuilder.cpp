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

#include "compression/CompressionDictionaryBuilder.hpp"

#include <cstddef>
#include <cstdint>
#include <limits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/comparisons/ComparisonUtil.hpp"

#include "glog/logging.h"

using std::numeric_limits;
using std::pair;
using std::uint32_t;
using std::unordered_map;

namespace quickstep {

namespace {

inline void CopyFixedLengthValuesToLocation(const std::vector<TypedValue> &values,
                                            const std::size_t type_length,
                                            char *location) {
  for (const TypedValue &value : values) {
    value.copyInto(location);
    location += type_length;
  }
}

inline void CopyVariableLengthValuesToLocation(const std::vector<TypedValue> &values,
                                               char *location) {
  uint32_t *offset_array_ptr = reinterpret_cast<uint32_t*>(location);
  char *values_location = static_cast<char*>(location)
                          + values.size() * sizeof(uint32_t);
  uint32_t value_offset = 0;
  for (const TypedValue &value : values) {
    *offset_array_ptr = value_offset;
    value.copyInto(values_location + value_offset);

    ++offset_array_ptr;
    value_offset += value.getDataSize();
  }
}

}  // namespace

CompressionDictionaryBuilder::CompressionDictionaryBuilder(const Type &type)
    : type_(type),
      type_is_nullable_(type.isNullable()),
      fixed_type_length_(type.isVariableLength() ? 0 : type.maximumByteLength()),
      last_inserted_iterator_(value_set_.end()),
      num_values_(0),
      code_length_bits_(0),
      total_value_size_(0),
      null_value_present_(false),
      last_insert_was_null_(false),
      may_contain_out_of_line_literal_(false),
      built_(false) {
}

void CompressionDictionaryBuilder::buildDictionary(void *location) {
  *static_cast<uint32_t*>(location) = num_values_ - null_value_present_;
  *(static_cast<uint32_t*>(location) + 1)
      = null_value_present_ ? num_values_ - 1
                            : std::numeric_limits<uint32_t>::max();

  char *copy_location = static_cast<char*>(location) + 2 * sizeof(uint32_t);

  std::vector<TypedValue> sorted_values;
  sorted_values.reserve(value_set_.size());
  if (may_contain_out_of_line_literal_ && !TypedValue::RepresentedInline(type_.getTypeID())) {
    // Avoid an intermediate copy of out-of-line data.
    for (const pair<const TypedValue, uint32_t> &value_pair : value_set_) {
      sorted_values.emplace_back(value_pair.first.makeReferenceToThis());
    }
  } else {
    for (const pair<const TypedValue, uint32_t> &value_pair : value_set_) {
      sorted_values.emplace_back(value_pair.first);
    }
  }
  SortValues(type_, sorted_values.begin(), sorted_values.end());

  // Update 'value_set_' with the actual codes used for each value so that
  // getCodeForValue() can do fast lookups.
  for (uint32_t code = 0; code < sorted_values.size(); ++code) {
    value_set_.find(sorted_values[code])->second = code;
  }

  if (fixed_type_length_) {
    CopyFixedLengthValuesToLocation(sorted_values, fixed_type_length_, copy_location);
  } else {
    CopyVariableLengthValuesToLocation(sorted_values, copy_location);
  }

  built_ = true;
}

bool CompressionDictionaryBuilder::insertEntryInternal(const TypedValue &value,
                                                       bool by_reference) {
  DCHECK(!built_);
  DCHECK(value.isPlausibleInstanceOf(type_.getSignature()));

  if (type_is_nullable_ && value.isNull()) {
    last_insert_was_null_ = !null_value_present_;
    null_value_present_ = true;
    if (last_insert_was_null_) {
      ++num_values_;
      if ((code_length_bits_ == 0) || (num_values_ == (1u << code_length_bits_) + 1)) {
        ++code_length_bits_;
      }
    }
    return last_insert_was_null_;
  }

  DCHECK(!value.isNull());
  if (num_values_ == numeric_limits<uint32_t>::max()) {
    LOG(FATAL) << "Attempted to insert a value into a CompressionDictionaryBuilder which "
               << "would cause it to overflow the limit of "
               << numeric_limits<uint32_t>::max() << " entries.";
  }

  pair<unordered_map<TypedValue,
                     uint32_t,
                     TypedValueHasher,
                     TypedValueEqualChecker>::iterator,
       bool> insertion_result;
  if (by_reference) {
    insertion_result = value_set_.emplace(value.makeReferenceToThis(),
                                          numeric_limits<uint32_t>::max());
  } else {
    insertion_result = value_set_.emplace(value,
                                          numeric_limits<uint32_t>::max());
  }
  if (!insertion_result.second) {
    return false;
  }
  if (!by_reference) {
    // const_cast here is safe: TypedValue will be modified in-place and
    // hash/compare the same.
    const_cast<TypedValue&>(insertion_result.first->first).ensureNotReference();
  }
  last_inserted_iterator_ = insertion_result.first;
  const std::size_t value_length = (fixed_type_length_ == 0) ? insertion_result.first->first.getDataSize()
                                                             : fixed_type_length_;

  // Update state.
  last_insert_was_null_ = false;
  ++num_values_;
  if ((code_length_bits_ == 0) || (num_values_ == (1u << code_length_bits_) + 1)) {
    ++code_length_bits_;
  }
  total_value_size_ += value_length;
  if (total_value_size_ > numeric_limits<uint32_t>::max()) {
    LOG(FATAL) << "Attempted to insert a value into a CompressionDictionaryBuilder which would "
               << "overflow the limit of "
               << numeric_limits<uint32_t>::max() << " total bytes.";
  }

  return true;
}

void CompressionDictionaryBuilder::undoLastInsert() {
  DCHECK(!built_);
  if (last_insert_was_null_) {
    null_value_present_ = false;
    last_insert_was_null_ = false;
  } else {
    DCHECK(last_inserted_iterator_ != value_set_.end());
    total_value_size_ -= (fixed_type_length_ == 0) ? last_inserted_iterator_->first.getDataSize()
                                                   : fixed_type_length_;
    value_set_.erase(last_inserted_iterator_);
    last_inserted_iterator_ = value_set_.end();
  }

  // Update state.
  --num_values_;
  if (num_values_ == 0) {
    code_length_bits_ = 0;
  } else if ((code_length_bits_ > 1) && (num_values_ == (1u << (code_length_bits_ - 1)))) {
    --code_length_bits_;
  }
}

}  // namespace quickstep
