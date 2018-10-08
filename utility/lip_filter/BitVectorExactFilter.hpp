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

#ifndef QUICKSTEP_UTILITY_LIP_FILTER_BIT_VECTOR_EXACT_FILTER_HPP_
#define QUICKSTEP_UTILITY_LIP_FILTER_BIT_VECTOR_EXACT_FILTER_HPP_

#include <cstddef>
#include <cstdint>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/Type.hpp"
#include "utility/BarrieredReadWriteConcurrentBitVector.hpp"
#include "utility/Macros.hpp"
#include "utility/lip_filter/LIPFilter.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief A LIP filter that tests the EXACT memberships of elements, i.e. there
 *        will be neither false positives nor false negatives. The implementation
 *        is to simply reinterpret_cast a value's byte stream into the specified
 *        CppType as the underlying bit vector's index. Therefore, to use this
 *        filter, the corresponding LIP attribute's values must be bounded in a
 *        reasonably small integer range.
 */
template <typename CppType, bool is_anti_filter>
class BitVectorExactFilter : public LIPFilter {
 public:
  /**
   * @brief Constructor.
   *
   * @param min_value The minimum possible value for this filter to set.
   * @param max_value The maximum possible value for this filter to set.
   */
  explicit BitVectorExactFilter(const std::int64_t min_value,
                                const std::int64_t max_value)
      : LIPFilter(LIPFilterType::kBitVectorExactFilter),
        min_value_(static_cast<CppType>(min_value)),
        max_value_(static_cast<CppType>(max_value)),
        bit_vector_(max_value - min_value + 1) {
    DCHECK_EQ(min_value, static_cast<std::int64_t>(min_value_));
    DCHECK_EQ(max_value, static_cast<std::int64_t>(max_value_));
    DCHECK_GE(max_value_, min_value_);
  }

  ~BitVectorExactFilter() override {};

  void insertValueAccessor(ValueAccessor *accessor,
                           const attribute_id attr_id,
                           const Type *attr_type) override {
    InvokeOnAnyValueAccessor(
        accessor,
        [&](auto *accessor) -> void {  // NOLINT(build/c++11)
      if (attr_type->isNullable()) {
        this->insertValueAccessorInternal<true>(accessor, attr_id);
      } else {
        this->insertValueAccessorInternal<false>(accessor, attr_id);
      }
    });
  }

  std::size_t filterBatch(ValueAccessor *accessor,
                          const attribute_id attr_id,
                          const bool is_attr_nullable,
                          std::vector<tuple_id> *batch,
                          const std::size_t batch_size) const override {
    DCHECK(batch != nullptr);
    DCHECK_LE(batch_size, batch->size());

    return InvokeOnAnyValueAccessor(
        accessor,
        [&](auto *accessor) -> std::size_t {  // NOLINT(build/c++11)
      if (is_attr_nullable) {
        return this->filterBatchInternal<true>(accessor, attr_id, batch, batch_size);
      } else {
        return this->filterBatchInternal<false>(accessor, attr_id, batch, batch_size);
      }
    });
  }

 private:
  /**
   * @brief Iterate through the accessor and hash values into the internal bit
   *        array.
   */
  template <bool is_attr_nullable, typename ValueAccessorT>
  inline void insertValueAccessorInternal(ValueAccessorT *accessor,
                                          const attribute_id attr_id) {
    accessor->beginIteration();
    while (accessor->next()) {
      const void *value = accessor->template getUntypedValue<is_attr_nullable>(attr_id);
      if (!is_attr_nullable || value != nullptr) {
        insert(value);
      }
    }
  }

  /**
   * @brief Filter the given batch of tuples from a ValueAccessor. Write the
   *        tuple ids which survive in the filtering back to \p batch.
   */
  template <bool is_attr_nullable, typename ValueAccessorT>
  inline std::size_t filterBatchInternal(const ValueAccessorT *accessor,
                                         const attribute_id attr_id,
                                         std::vector<tuple_id> *batch,
                                         const std::size_t batch_size) const {
    std::size_t out_size = 0;
    for (std::size_t i = 0; i < batch_size; ++i) {
      const tuple_id tid = batch->at(i);
      const void *value =
          accessor->template getUntypedValueAtAbsolutePosition(attr_id, tid);
      if (is_attr_nullable && value == nullptr) {
        continue;
      }
      if (contains(value)) {
        batch->at(out_size) = tid;
        ++out_size;
      }
    }
    return out_size;
  }

  /**
   * @brief Inserts a given value into the exact filter.
   */
  inline void insert(const void *key_begin) {
    const CppType value = *reinterpret_cast<const CppType *>(key_begin);
    DCHECK_GE(value, min_value_);
    DCHECK_LE(value, max_value_);

    bit_vector_.setBit(value - min_value_);
  }

  /**
   * @brief Test membership of a given value in the exact filter.
   */
  inline bool contains(const void *key_begin) const {
    const CppType value = *reinterpret_cast<const CppType *>(key_begin);
    if (value < min_value_ || value > max_value_) {
      return is_anti_filter;
    }

    const bool is_bit_set = bit_vector_.getBit(value - min_value_);

    if (is_anti_filter) {
      return !is_bit_set;
    } else {
      return is_bit_set;
    }
  }

  const CppType min_value_;
  const CppType max_value_;
  BarrieredReadWriteConcurrentBitVector bit_vector_;

  DISALLOW_COPY_AND_ASSIGN(BitVectorExactFilter);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_LIP_FILTER_BIT_VECTOR_EXACT_FILTER_HPP_
