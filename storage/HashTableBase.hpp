/**
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

#ifndef QUICKSTEP_STORAGE_HASH_TABLE_BASE_HPP_
#define QUICKSTEP_STORAGE_HASH_TABLE_BASE_HPP_

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
  kSeparateChaining
};

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
  virtual ~HashTableBase() {
  }

 protected:
  HashTableBase() {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(HashTableBase);
};

typedef HashTableBase<true, false, true, false> AggregationStateHashTableBase;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_HASH_TABLE_BASE_HPP_
