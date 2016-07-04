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

#ifndef QUICKSTEP_STORAGE_HASH_TABLE_BASE_HPP_
#define QUICKSTEP_STORAGE_HASH_TABLE_BASE_HPP_

#include <cstddef>
#include <vector>

#include "ValueAccessor.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief An enum with an entry for each full HashTable implementation template
 *        class. Used to specify the desired implementation when using
 *        HashTableFactory to create a HashTable.
 **/
enum class HashTableImplType {
  kLinearOpenAddressing,
  kSeparateChaining,
  kSimpleScalarSeparateChaining
};

/**
 * @brief Used internally by HashTables to keep track of pre-allocated
 *        resources used in putValueAccessor() and
 *        putValueAccessorCompositeKey().
 **/
struct HashTablePreallocationState {
  std::size_t bucket_position;
  std::size_t variable_length_key_position;
};

/**
 * @brief Codes which indicate the result of a call to put() or
 *        putCompositeKey().
 **/
enum class HashTablePutResult { kOK = 0, kDuplicateKey, kOutOfSpace };

/**
 * @brief An ultra-minimal base class that HashTables with different ValueT
 *        parameters inherit from. This allows for a bit more type-safety than
 *        just passing around void* pointers (although casting will still be
 *        required). See the HashTable template class for everything
 *        interesting.
 **/
template <bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
class HashTableBase {
 public:
  virtual ~HashTableBase() {}

  /**
   * TODO(harshad) We should get rid of this function from here. We are
   * postponing it because of the amount of work to be done is significant.
   * The steps are as follows:
   * 1. Replace AggregationStateHashTableBase occurence in HashTablePool to
   * the FastHashTable implementation (i.e. an implementation specialized for
   * aggregation).
   * 2. Remove createGroupByHashTable from the AggregationHandle* classes.
   * 3. Replace AggregationStateHashTableBase occurences in AggregationHandle*
   * clases to the FastHashTable implementation (i.e. an implementation
   * specialized for aggregation).
   * 4. Move this method to the FastHashTable class from here, so that it can
   * be called from the AggregationHandle* classes.
   *
   * Optionally, we can also remove the AggregationStateHashTableBase
   * specialization from this file.
   **/
  virtual bool upsertValueAccessorCompositeKeyFast(
      const std::vector<attribute_id> &argument,
      ValueAccessor *accessor,
      const std::vector<attribute_id> &key_attr_ids,
      const bool check_for_null_keys) {
    return false;
  }

 protected:
  HashTableBase() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(HashTableBase);
};

typedef HashTableBase<true, false, true, false> AggregationStateHashTableBase;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_HASH_TABLE_BASE_HPP_
