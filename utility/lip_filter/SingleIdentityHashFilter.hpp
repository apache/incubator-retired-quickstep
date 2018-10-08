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

#ifndef QUICKSTEP_UTILITY_LIP_FILTER_SINGLE_IDENTITY_HASH_FILTER_HPP_
#define QUICKSTEP_UTILITY_LIP_FILTER_SINGLE_IDENTITY_HASH_FILTER_HPP_

#include <cstddef>
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
 * @brief Specialized bloom filter that uses only one hash function. The hash
 *        function is an "identity" function that it simply reinterpret_casts
 *        a value's byte stream into the specified CppType as the value's hash
 *        code.
 *
 * @note SingleIdentityHashFilter is most effective when applied to fixed-length
 *       integer values. It cannot be applied to variable-length values unless
 *       the corresponding value Type has its minimumByteLength() greater than
 *       sizeof(CppType).
 */
template <typename CppType>
class SingleIdentityHashFilter : public LIPFilter {
 public:
  /**
   * @brief Constructor.
   *
   * @param filter_cardinality The cardinality of this hash filter.
   */
  explicit SingleIdentityHashFilter(const std::size_t filter_cardinality)
      : LIPFilter(LIPFilterType::kSingleIdentityHashFilter),
        filter_cardinality_(filter_cardinality),
        bit_vector_(filter_cardinality) {
    DCHECK_GE(filter_cardinality, 1u);
  }

  ~SingleIdentityHashFilter() override {};

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
   * @brief Round up bit_size to multiples of 8.
   */
  inline static std::size_t GetByteSize(const std::size_t bit_size) {
    return (bit_size + 7) / 8;
  }

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
   * @brief Inserts a given value into the hash filter.
   */
  inline void insert(const void *key_begin) {
    const CppType hash =
        *reinterpret_cast<const CppType *>(key_begin) % filter_cardinality_;
    bit_vector_.setBit(hash);
  }

  /**
   * @brief Test membership of a given value in the hash filter.
   *        If true is returned, then a value may or may not be present in the hash filter.
   *        If false is returned, a value is certainly not present in the hash filter.
   */
  inline bool contains(const void *key_begin) const {
    const CppType hash =
        *reinterpret_cast<const CppType *>(key_begin) % filter_cardinality_;
    return bit_vector_.getBit(hash);
  }

  std::size_t filter_cardinality_;
  BarrieredReadWriteConcurrentBitVector bit_vector_;

  DISALLOW_COPY_AND_ASSIGN(SingleIdentityHashFilter);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_LIP_FILTER_SINGLE_IDENTITY_HASH_FILTER_HPP_
