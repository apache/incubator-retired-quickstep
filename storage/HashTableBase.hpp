/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
