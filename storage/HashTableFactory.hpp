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

#ifndef QUICKSTEP_STORAGE_HASH_TABLE_FACTORY_HPP_
#define QUICKSTEP_STORAGE_HASH_TABLE_FACTORY_HPP_

#include <cstddef>
#include <string>
#include <vector>

#include "storage/CollisionFreeVectorTable.hpp"
#include "storage/HashTable.hpp"
#include "storage/HashTableBase.hpp"
#include "storage/HashTable.pb.h"
#include "storage/LinearOpenAddressingHashTable.hpp"
#include "storage/PackedPayloadHashTable.hpp"
#include "storage/SeparateChainingHashTable.hpp"
#include "storage/SimpleScalarSeparateChainingHashTable.hpp"
#include "storage/ThreadPrivateCompactKeyHashTable.hpp"
#include "storage/TupleReference.hpp"
#include "types/TypeFactory.hpp"
#include "utility/BloomFilter.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class StorageManager;
class Type;

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief Validation function for use with gflags. Checks that a string used to
 *        indicate a particular HashTable implementation is valid.
 *
 * @note This function does NOT consider "SimpleScalarSeparateChaining" to be
 *       a valid string, as that is considered a special case of
 *       "SeparateChaining" that may be chosen by the optimizer but not
 *       explicitly specified with command-line flags.
 *
 * @param flagname The name of the command-line flag to validate.
 * @param value The value of the flag.
 * @return true if value is valid (i.e. it is either "LinearOpenAddressing" or
 *         "SeparateChaining"), false otherwise.
 **/
bool ValidateHashTableImplTypeString(const char *flagname,
                                     const std::string &value);

/**
 * @brief Get a serialization::HashTableImplType from a human-readable string
 *        indicating the desired HashTable implementation.
 *
 * @warning It is an error to call this function with an invalid
 *          hash_table_impl_string. The string should be checked beforehand
 *          (e.g. by registering ValidateHashTableImplTypeString() as a
 *          flag-validator in gflags).
 *
 * @param hash_table_impl_string A human readable-string (probably from a
 *        command-line flag) indicating the desired HashTable implementation
 *        (currently either "LinearOpenAddressing" or "SeparateChaining").
 * @return The serialization::HashTableImplType corresponding to
 *         hash_table_impl_string.
 **/
serialization::HashTableImplType HashTableImplTypeProtoFromString(
    const std::string &hash_table_impl_string);

/**
 * @brief Attempt to convert a specified serialization::HashTableImplType to
 *        a simpler HashTable implementation if possible for given key Types.
 *
 * @note Currently, this function converts SEPARATE_CHAINING to
 *       SIMPLE_SCALAR_SEPARATE_CHAINING if there is a single scalar key with
 *       a reversible hash function (i.e. if
 *       SimpleScalarSeparateChainingHashTable is usable). It does nothing if
 *       the originally selected implementation is LINEAR_OPEN_ADDRESSING or if
 *       the requirements for the key are not met.
 *
 * @param proto_impl_type The proto form of the originally chosen HashTable
 *        implementation.
 * @param key_types The Types of keys to be used with the HashTable.
 * @return A serialization::HashTableImplType indicating a simplified form of
 *         the original proto_impl_type if possible, otherwise proto_impl_type
 *         unchanged.
 **/
serialization::HashTableImplType SimplifyHashTableImplTypeProto(
    const serialization::HashTableImplType proto_impl_type,
    const std::vector<const Type*> &key_types);

/**
 * @brief Convert a serialization::HashTableImplType to the equivalent
 *        HashTableImplType.
 *
 * @param proto_type A serialization::HashTableImplType to convert.
 * @return The HashTableImplType equivalent to proto_type.
 **/
inline HashTableImplType HashTableImplTypeFromProto(
    const serialization::HashTableImplType proto_type) {
  switch (proto_type) {
    case serialization::HashTableImplType::COLLISION_FREE_VECTOR:
      return HashTableImplType::kCollisionFreeVector;
    case serialization::HashTableImplType::LINEAR_OPEN_ADDRESSING:
      return HashTableImplType::kLinearOpenAddressing;
    case serialization::HashTableImplType::SEPARATE_CHAINING:
      return HashTableImplType::kSeparateChaining;
    case serialization::HashTableImplType::SIMPLE_SCALAR_SEPARATE_CHAINING:
      return HashTableImplType::kSimpleScalarSeparateChaining;
    case serialization::HashTableImplType::THREAD_PRIVATE_COMPACT_KEY:
      return HashTableImplType::kThreadPrivateCompactKey;
    default: {
      LOG(FATAL) << "Unrecognized serialization::HashTableImplType\n";
    }
  }
}

/**
 * @brief Templated all-static factory class that makes it easier to
 *        instantiate HashTables with the particular HashTable implementation
 *        chosen at runtime. All template parameters are exactly the same as
 *        those of HashTable.
 **/
template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
class HashTableFactory {
 public:
  /**
   * @brief Create a new resizable HashTable, with the type selected by
   *        hash_table_type. Other parameters are forwarded to the HashTable's
   *        constructor.
   *
   * @param hash_table_type The specific HashTable implementation that should
   *        be used.
   * @param key_types A vector of one or more types (>1 indicates a composite
   *        key). Forwarded as-is to the HashTable's constructor.
   * @param num_entries The estimated number of entries the HashTable will
   *        hold. Forwarded as-is to the HashTable's constructor.
   * @param storage_manager The StorageManager to use (a StorageBlob will be
   *        allocated to hold the HashTable's contents). Forwarded as-is to the
   *        HashTable's constructor.
   * @return A new resizable HashTable.
   **/
  static HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>*
      CreateResizable(const HashTableImplType hash_table_type,
                      const std::vector<const Type*> &key_types,
                      const std::size_t num_entries,
                      StorageManager *storage_manager) {
    DCHECK(resizable);

    switch (hash_table_type) {
      case HashTableImplType::kLinearOpenAddressing:
        return new LinearOpenAddressingHashTable<
            ValueT,
            resizable,
            serializable,
            force_key_copy,
            allow_duplicate_keys>(key_types, num_entries, storage_manager);
      case HashTableImplType::kSeparateChaining:
        return new SeparateChainingHashTable<
            ValueT,
            resizable,
            serializable,
            force_key_copy,
            allow_duplicate_keys>(key_types, num_entries, storage_manager);
      case HashTableImplType::kSimpleScalarSeparateChaining:
        return new SimpleScalarSeparateChainingHashTable<
            ValueT,
            resizable,
            serializable,
            force_key_copy,
            allow_duplicate_keys>(key_types, num_entries, storage_manager);
      default: {
        LOG(FATAL) << "Unrecognized HashTableImplType in HashTableFactory::createResizable()\n";
      }
    }
  }

  /**
   * @brief Create a new fixed-sized HashTable, with the type selected by
   *        hash_table_type. Other parameters are forwarded to the HashTables's
   *        constructor.
   *
   * @param hash_table_type The specific HashTable implementation that should
   *        be used.
   * @param key_types A vector of one or more types (>1 indicates a composite
   *        key). Forwarded as-is to the HashTable's constructor.
   * @param hash_table_memory A pointer to memory to use for the HashTable.
   *        Forwarded as-is to the HashTable's constructor.
   * @param hash_table_memory_size The size of hash_table_memory in bytes.
   *        Forwarded as-is to the HashTable's constructor.
   * @param new_hash_table If true, the HashTable is being constructed for the
   *        first time and hash_table_memory will be cleared. If false, reload
   *        a pre-existing HashTable. Forwarded as-is to the HashTable's
   *        constructor.
   * @param hash_table_memory_zeroed If new_hash_table is true, setting this to
   *        true means that the HashTable will assume that hash_table_memory
   *        has already been zeroed-out (any newly-allocated block or blob
   *        memory from StorageManager is zeroed-out). If false, the HashTable
   *        will explicitly zero-fill its memory as neccessary. This parameter
   *        has no effect when new_hash_table is false. Forwarded as-is to the
   *        HashTable's constructor.
   * @return A new (or reloaded) fixed-size HashTable.
   **/
  static HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>*
      CreateFixedSize(const HashTableImplType hash_table_type,
                      const std::vector<const Type*> &key_types,
                      void *hash_table_memory,
                      const std::size_t hash_table_memory_size,
                      const bool new_hash_table,
                      const bool hash_table_memory_zeroed) {
    DCHECK(!resizable);

    switch (hash_table_type) {
      case HashTableImplType::kLinearOpenAddressing:
        return new LinearOpenAddressingHashTable<
            ValueT,
            resizable,
            serializable,
            force_key_copy,
            allow_duplicate_keys>(key_types,
                                  hash_table_memory,
                                  hash_table_memory_size,
                                  new_hash_table,
                                  hash_table_memory_zeroed);
      case HashTableImplType::kSeparateChaining:
        return new SeparateChainingHashTable<
            ValueT,
            resizable,
            serializable,
            force_key_copy,
            allow_duplicate_keys>(key_types,
                                  hash_table_memory,
                                  hash_table_memory_size,
                                  new_hash_table,
                                  hash_table_memory_zeroed);
      case HashTableImplType::kSimpleScalarSeparateChaining:
        return new SimpleScalarSeparateChainingHashTable<
            ValueT,
            resizable,
            serializable,
            force_key_copy,
            allow_duplicate_keys>(key_types,
                                  hash_table_memory,
                                  hash_table_memory_size,
                                  new_hash_table,
                                  hash_table_memory_zeroed);
      default: {
        LOG(FATAL) << "Unrecognized HashTableImplType\n";
      }
    }
  }

  /**
   * @brief Check whether a serialization::HashTable describing a resizable
   *        HashTable is fully-formed and all parts are valid.
   *
   * @param proto A serialized Protocol Buffer description of a HashTable,
   *        originally generated by the optimizer.
   * @return Whether proto is fully-formed and valid.
   **/
  static bool ProtoIsValid(const serialization::HashTable &proto) {
    if (!proto.IsInitialized() ||
        !serialization::HashTableImplType_IsValid(
            proto.hash_table_impl_type())) {
      return false;
    }

    for (int i = 0; i < proto.key_types_size(); ++i) {
      if (!TypeFactory::ProtoIsValid(proto.key_types(i))) {
        return false;
      }
    }

    return true;
  }

  /**
   * @brief Create a new resizable HashTable according to a protobuf
   *        description.
   *
   * @param proto A protobuf description of a resizable HashTable.
   * @param storage_manager The StorageManager to use (a StorageBlob will be
   *        allocated to hold the HashTable's contents).
   * @return A new resizable HashTable with parameters specified by proto.
   **/
  static HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>*
      CreateResizableFromProto(const serialization::HashTable &proto,
                               StorageManager *storage_manager) {
    DCHECK(ProtoIsValid(proto))
        << "Attempted to create HashTable from invalid proto description:\n"
        << proto.DebugString();

    std::vector<const Type*> key_types;
    for (int i = 0; i < proto.key_types_size(); ++i) {
      key_types.emplace_back(&TypeFactory::ReconstructFromProto(proto.key_types(i)));
    }

    auto hash_table = CreateResizable(HashTableImplTypeFromProto(proto.hash_table_impl_type()),
                                      key_types,
                                      proto.estimated_num_entries(),
                                      storage_manager);
    return hash_table;
  }

 private:
  // Class is all-static and should not be instantiated.
  HashTableFactory();

  DISALLOW_COPY_AND_ASSIGN(HashTableFactory);
};

/**
 * @brief Convenient alias for a HashTableFactory that makes JoinHashTables.
 **/
typedef HashTableFactory<TupleReference, true, false, false, true>
    JoinHashTableFactory;

/**
 * @brief Factory class that makes it easier to instantiate aggregation state
 *        hash tables.
 **/
class AggregationStateHashTableFactory {
 public:
  /**
   * @brief Create a new aggregation state hash table, with the type selected by
   *        hash_table_type. Other parameters are forwarded to the hash table's
   *        constructor.
   *
   * @param hash_table_type The specific hash table implementation that should
   *        be used.
   * @param key_types A vector of one or more types (>1 indicates a composite
   *        key). Forwarded as-is to the hash table's constructor.
   * @param num_entries The estimated number of entries the hash table will
   *        hold. Forwarded as-is to the hash table's constructor.
   * @param storage_manager The StorageManager to use (a StorageBlob will be
   *        allocated to hold the hash table's contents). Forwarded as-is to the
   *        hash table constructor.
   * @return A new aggregation state hash table.
   **/
  static AggregationStateHashTableBase* CreateResizable(
      const HashTableImplType hash_table_type,
      const std::vector<const Type*> &key_types,
      const std::size_t num_entries,
      const std::vector<AggregationHandle *> &handles,
      StorageManager *storage_manager) {
    switch (hash_table_type) {
      case HashTableImplType::kCollisionFreeVector:
        DCHECK_EQ(1u, key_types.size());
        return new CollisionFreeVectorTable(
            key_types.front(), num_entries, handles, storage_manager);
      case HashTableImplType::kSeparateChaining:
        return new PackedPayloadHashTable(
            key_types, num_entries, handles, storage_manager);
      case HashTableImplType::kThreadPrivateCompactKey:
        return new ThreadPrivateCompactKeyHashTable(
            key_types, num_entries, handles, storage_manager);
      default: {
        LOG(FATAL) << "Unrecognized HashTableImplType in "
                   << "AggregationStateHashTableFactory::createResizable()";
      }
    }
  }

 private:
  // Class is all-static and should not be instantiated.
  AggregationStateHashTableFactory();

  DISALLOW_COPY_AND_ASSIGN(AggregationStateHashTableFactory);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_HASH_TABLE_FACTORY_HPP_
