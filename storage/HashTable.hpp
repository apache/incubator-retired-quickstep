/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_STORAGE_HASH_TABLE_HPP_
#define QUICKSTEP_STORAGE_HASH_TABLE_HPP_

#include <atomic>
#include <cstddef>
#include <cstdlib>
#include <type_traits>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/HashTableBase.hpp"
#include "storage/StorageBlob.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageConstants.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TupleReference.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "threading/SpinSharedMutex.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "utility/HashPair.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief Codes which indicate the result of a call to put() or
 *        putCompositeKey().
 **/
enum class HashTablePutResult {
  kOK = 0,
  kDuplicateKey,
  kOutOfSpace
};

/**
 * @brief Base class for hash table.
 *
 * This class is templated so that the core hash-table logic can be reused in
 * different contexts requiring different value types and semantics (e.g.
 * hash-joins vs. hash-based grouping for aggregates vs. hash-based indices).
 * The base template defines the interface that HashTables provide to clients
 * and implements some common functionality for all HashTables. There a few
 * different (also templated) implementation classes that inherit from this
 * base class and have different physical layouts with different performance
 * characteristics. As of this writing, they are:
 *      1. LinearOpenAddressingHashTable - All keys/values are stored directly
 *         in a single array of buckets. Collisions are handled by simply
 *         advancing to the "next" adjacent bucket until an empty bucket is
 *         found. This implementation is vulnerable to performance degradation
 *         due to the formation of bucket chains when there are many duplicate
 *         and/or consecutive keys.
 *      2. SeparateChainingHashTable - Keys/values are stored in a separate
 *         region of memory from the base hash table slot array. Every bucket
 *         has a "next" pointer so that entries that collide (i.e. map to the
 *         same base slot) form chains of pointers with each other. Although
 *         this implementation has some extra indirection compared to
 *         LinearOpenAddressingHashTable, it does not have the same
 *         vulnerabilities to key skew, and it additionally supports a very
 *         efficient bucket-preallocation mechanism that minimizes cache
 *         coherency overhead when multiple threads are building a HashTable
 *         as part of a hash-join.
 *      3. SimpleScalarSeparateChainingHashTable - A simplified version of
 *         SeparateChainingHashTable that is only usable for single, scalar
 *         keys with a reversible hash function. This implementation exploits
 *         the reversible hash to avoid storing separate copies of keys at all,
 *         and to skip an extra key comparison when hash codes collide.
 *
 * @note If you need to create a HashTable and not just use it as a client, see
 *       HashTableFactory, which simplifies the process of creating a
 *       HashTable.
 *
 * @param ValueT The mapped value in this hash table. Must be
 *        copy-constructible. For a serializable hash table, ValueT must also
 *        be trivially copyable and trivially destructible (and beware of
 *        pointers to external memory).
 * @param resizable Whether this hash table is resizable (using memory from a
 *        StorageManager) or not (using a private, fixed memory allocation).
 * @param serializable If true, this hash table can safely be saved to and
 *        loaded from disk. If false, some out of band memory may be used (e.g.
 *        to store variable length keys).
 * @param force_key_copy If true, inserted keys are always copied into this
 *        HashTable's memory. If false, pointers to external values may be
 *        stored instead. force_key_copy should be true if the hash table will
 *        outlive the external key values which are inserted into it. Note that
 *        if serializable is true and force_key_copy is false, then relative
 *        offsets will be used instead of absolute pointers to keys, meaning
 *        that the pointed-to keys must be serialized and deserialized in
 *        exactly the same relative byte order (e.g. as part of the same
 *        StorageBlock), and keys must not change position relative to this
 *        HashTable (beware TupleStorageSubBlocks that may self-reorganize when
 *        modified). If serializable and resizable are both true, then
 *        force_key_copy must also be true.
 * @param allow_duplicate_keys If true, multiple values can be mapped to the
 *        same key. If false, one and only one value may be mapped.
 **/
template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
class HashTable : public HashTableBase<resizable,
                                       serializable,
                                       force_key_copy,
                                       allow_duplicate_keys> {
  static_assert(std::is_copy_constructible<ValueT>::value,
                "A HashTable can not be used with a Value type which is not "
                "copy-constructible.");

  static_assert((!serializable) || std::is_trivially_destructible<ValueT>::value,
                "A serializable HashTable can not be used with a Value type "
                "which is not trivially destructible.");

  static_assert(!(serializable && resizable && !force_key_copy),
                "A HashTable must have force_key_copy=true when serializable "
                "and resizable are both true.");

  // TODO(chasseur): GCC 4.8.3 doesn't yet implement
  // std::is_trivially_copyable. In the future, we should include a
  // static_assert that prevents a serializable HashTable from being used with
  // a ValueT which is not trivially copyable.

 public:
  // Shadow template parameters. This is useful for shared test harnesses.
  typedef ValueT value_type;
  static constexpr bool template_resizable = resizable;
  static constexpr bool template_serializable = serializable;
  static constexpr bool template_force_key_copy = force_key_copy;
  static constexpr bool template_allow_duplicate_keys = allow_duplicate_keys;

  // Some HashTable implementations (notably LinearOpenAddressingHashTable)
  // use a special hash code to represent an empty bucket, and another special
  // code to indicate that a bucket is currently being inserted into. For those
  // HashTables, this is a surrogate hash value for empty buckets. Keys which
  // actually hash to this value should have their hashes mutated (e.g. by
  // adding 1). We use zero, since we will often be using memory which is
  // already zeroed-out and this saves us the trouble of a memset. This has
  // some downside, as the hash function we use is the identity hash for
  // integers, and the integer 0 is common in many data sets and must be
  // adjusted (and will then spuriously collide with 1). Nevertheless, this
  // expense is outweighed by no longer having to memset large regions of
  // memory when initializing a HashTable.
  static constexpr unsigned char kEmptyHashByte = 0x0;
  static constexpr std::size_t kEmptyHash = 0x0;

  // A surrogate hash value for a bucket which is currently being inserted
  // into. As with kEmptyHash, keys which actually hash to this value should
  // have their hashes adjusted.
  static constexpr std::size_t kPendingHash = ~kEmptyHash;

  /**
   * @brief Virtual destructor.
   **/
  virtual ~HashTable() {
    if (resizable) {
      if (blob_.valid()) {
        if (serializable) {
          DEV_WARNING("Destroying a resizable serializable HashTable's underlying "
                      "StorageBlob.");
        }
        const block_id blob_id = blob_->getID();
        blob_.release();
        storage_manager_->deleteBlockOrBlobFile(blob_id);
      }
    }
  }

  /**
   * @brief Get the ID of the StorageBlob used to store a resizable HashTable.
   *
   * @warning This method must not be used for a non-resizable HashTable.
   *
   * @return The ID of the StorageBlob used to store this HashTable.
   **/
  inline block_id getBlobId() const {
    DEBUG_ASSERT(resizable);
    return blob_->getID();
  }

  /**
   * @brief Erase all entries in this hash table.
   *
   * @warning This method is not guaranteed to be threadsafe.
   **/
  virtual void clear() = 0;

  /**
   * @brief Add a new entry into the hash table.
   *
   * @warning The key must not be null.
   * @warning This method is threadsafe with regard to other calls to put(),
   *          putCompositeKey(), putValueAccessor(), and
   *          putValueAccessorCompositeKey(), but should not be used
   *          simultaneously with upsert(), upsertCompositeKey(),
   *          upsertValueAccessor(), or upsertValueAccessorCompositeKey().
   * @note This version is for single scalar keys, see also putCompositeKey().
   * @note If the hash table is (close to) full and resizable is true, this
   *       routine might result in rebuilding the entire hash table.
   *
   * @param key The key.
   * @param value The value payload.
   * @return HashTablePutResult::kOK if an entry was successfully inserted,
   *         HashTablePutResult::kDuplicateKey if allow_duplicate_keys is false
   *         and key was a duplicate, or HashTablePutResult::kOutOfSpace if
   *         resizable is false and storage space for the hash table has been
   *         exhausted.
   **/
  HashTablePutResult put(const TypedValue &key,
                         const ValueT &value);

  /**
   * @brief Add a new entry into the hash table (composite key version).
   *
   * @warning No component of the key may be null.
   * @warning This method is threadsafe with regard to other calls to put(),
   *          putCompositeKey(), putValueAccessor(), and
   *          putValueAccessorCompositeKey(), but should not be used
   *          simultaneously with upsert(), upsertCompositeKey(),
   *          upsertValueAccessor(), or upsertValueAccessorCompositeKey().
   * @note This version is for composite keys, see also put().
   * @note If the hash table is (close to) full and resizable is true, this
   *       routine might result in rebuilding the entire hash table.
   *
   * @param key The components of the key.
   * @param value The value payload.
   * @return HashTablePutResult::kOK if an entry was successfully inserted,
   *         HashTablePutResult::kDuplicateKey if allow_duplicate_keys is false
   *         and key was a duplicate, or HashTablePutResult::kOutOfSpace if
   *         resizable is false and storage space for the hash table has been
   *         exhausted.
   **/
  HashTablePutResult putCompositeKey(const std::vector<TypedValue> &key,
                                     const ValueT &value);

  /**
   * @brief Add (multiple) new entries into the hash table from a
   *        ValueAccessor.
   *
   * @warning This method is threadsafe with regard to other calls to put(),
   *          putCompositeKey(), putValueAccessor(), and
   *          putValueAccessorCompositeKey(), but should not be used
   *          simultaneously with upsert(), upsertCompositeKey(),
   *          upsertValueAccessor(), or upsertValueAccessorCompositeKey().
   * @note This version is for scalar keys, see also
   *       putValueAccessorCompositeKey().
   * @note If the hash table fills up while this call is in progress and
   *       resizable is true, this might result in rebuilding the entire hash
   *       table.
   *
   * @param accessor A ValueAccessor which will be used to access keys.
   *        beginIteration() should be called on accessor before calling this
   *        method.
   * @param key_attr_id The attribute ID of the keys to be read from accessor.
   * @param check_for_null_keys If true, each key will be checked to see if it
   *        is null before inserting it (null keys are skipped). This must be
   *        set to true if some of the keys that will be read from accessor may
   *        be null.
   * @param functor A pointer to a functor, which should provide a call
   *        operator that takes const ValueAccessor& as an argument (or better
   *        yet, a templated call operator which takes a const reference to
   *        some subclass of ValueAccessor as an argument) and returns either
   *        a ValueT or a reference to a ValueT. The functor should generate
   *        the appropriate mapped value for the current tuple the accessor is
   *        iterating on.
   * @return HashTablePutResult::kOK if all keys and generated values from
   *         accessor were successfully inserted.
   *         HashTablePutResult::kOutOfSpace is returned if this hash-table is
   *         non-resizable and ran out of space (note that some entries may
   *         still have been inserted, and accessor's iteration will be left on
   *         the first tuple which could not be inserted).
   *         HashTablePutResult::kDuplicateKey is returned if
   *         allow_duplicate_keys is false and a duplicate key is encountered
   *         (as with HashTablePutResult::kOutOfSpace, some entries may have
   *         been inserted, and accessor will be left on the tuple with a
   *         duplicate key).
   **/
  template <typename FunctorT>
  HashTablePutResult putValueAccessor(ValueAccessor *accessor,
                                      const attribute_id key_attr_id,
                                      const bool check_for_null_keys,
                                      FunctorT *functor);

  /**
   * @brief Add (multiple) new entries into the hash table from a
   *        ValueAccessor (composite key version).
   *
   * @warning This method is threadsafe with regard to other calls to put(),
   *          putCompositeKey(), putValueAccessor(), and
   *          putValueAccessorCompositeKey(), but should not be used
   *          simultaneously with upsert(), upsertCompositeKey(),
   *          upsertValueAccessor(), or upsertValueAccessorCompositeKey().
   * @note This version is for composite keys, see also putValueAccessor().
   * @note If the hash table fills up while this call is in progress and
   *       resizable is true, this might result in rebuilding the entire hash
   *       table.
   *
   * @param accessor A ValueAccessor which will be used to access keys.
   *        beginIteration() should be called on accessor before calling this
   *        method.
   * @param key_attr_ids The attribute IDs of each key component to be read
   *        from accessor.
   * @param check_for_null_keys If true, each key will be checked to see if it
   *        has a null component before inserting it (null keys are skipped).
   *        This must be set to true if some of the keys that will be read from
   *        accessor may be null.
   * @param functor A pointer to a functor, which should provide a call
   *        operator that takes const ValueAccessor& as an argument (or better
   *        yet, a templated call operator which takes a const reference to
   *        some subclass of ValueAccessor as an argument) and returns either
   *        a ValueT or a reference to a ValueT. The functor should generate
   *        the appropriate mapped value for the current tuple the accessor is
   *        iterating on.
   * @return HashTablePutResult::kOK if all keys and generated values from
   *         accessor were successfully inserted.
   *         HashTablePutResult::kOutOfSpace is returned if this hash-table is
   *         non-resizable and ran out of space (note that some entries may
   *         still have been inserted, and accessor's iteration will be left on
   *         the first tuple which could not be inserted).
   *         HashTablePutResult::kDuplicateKey is returned if
   *         allow_duplicate_keys is false and a duplicate key is encountered
   *         (as with HashTablePutResult::kOutOfSpace, some entries may have
   *         been inserted, and accessor will be left on the tuple with a
   *         duplicate key).
   **/
  template <typename FunctorT>
  HashTablePutResult putValueAccessorCompositeKey(
      ValueAccessor *accessor,
      const std::vector<attribute_id> &key_attr_ids,
      const bool check_for_null_keys,
      FunctorT *functor);

  /**
   * @brief Apply a functor to the value mapped to a key, first inserting a new
   *        value if one is not already present.
   *
   * @warning The key must not be null.
   * @warning This method is only usable if allow_duplicate_keys is false.
   * @warning This method is threadsafe with regard to other calls to upsert(),
   *          upsertCompositeKey(), upsertValueAccessor(), and
   *          upsertValueAccessorCompositeKey(), but should not be used
   *          simultaneously with put(), putCompositeKey(), putValueAccessor(),
   *          or putValueAccessorCompositeKey().
   * @warning The ValueT* pointer passed to functor's call operator is only
   *          guaranteed to be valid for the duration of the call. The functor
   *          should not store a copy of the pointer and assume that it remains
   *          valid.
   * @warning Although this method itself is threadsafe, the ValueT object
   *          accessed by functor is not guaranteed to be (although it is
   *          guaranteed that its initial insertion will be atomic). If it is
   *          possible for multiple threads to call upsert() with the same key
   *          at the same time, then their access to ValueT should be made
   *          threadsafe (e.g. with the use of atomic types, mutexes, or some
   *          other external synchronization).
   * @note This version is for single scalar keys, see also
   *       upsertCompositeKey().
   * @note If the hash table is (close to) full and resizable is true, this
   *       routine might result in rebuilding the entire hash table.
   *
   * @param key The key.
   * @param initial_value If there was not already a preexisting entry in this
   *        HashTable for the specified key, then the value will be initialized
   *        with a copy of initial_value. This parameter is ignored if a value
   *        is already present for key.
   * @param functor A pointer to a functor, which should provide a call
   *        operator which takes ValueT* as an argument. The call operator will
   *        be invoked once on the value corresponding to key (which may be
   *        newly inserted and default-constructed).
   * @return True on success, false if upsert failed because there was not
   *         enough space to insert a new entry in this HashTable.
   **/
  template <typename FunctorT>
  bool upsert(const TypedValue &key,
              const ValueT &initial_value,
              FunctorT *functor);

  /**
   * @brief Apply a functor to the value mapped to a key, first inserting a new
   *        value if one is not already present.
   *
   * @warning The key must not be null.
   * @warning This method is only usable if allow_duplicate_keys is false.
   * @warning This method is threadsafe with regard to other calls to upsert(),
   *          upsertCompositeKey(), upsertValueAccessor(), and
   *          upsertValueAccessorCompositeKey(), but should not be used
   *          simultaneously with put(), putCompositeKey(), putValueAccessor(),
   *          or putValueAccessorCompositeKey().
   * @warning The ValueT* pointer passed to functor's call operator is only
   *          guaranteed to be valid for the duration of the call. The functor
   *          should not store a copy of the pointer and assume that it remains
   *          valid.
   * @warning Although this method itself is threadsafe, the ValueT object
   *          accessed by functor is not guaranteed to be (although it is
   *          guaranteed that its initial insertion will be atomic). If it is
   *          possible for multiple threads to call upsertCompositeKey() with
   *          the same key at the same time, then their access to ValueT should
   *          be made threadsafe (e.g. with the use of atomic types, mutexes,
   *          or some other external synchronization).
   * @note This version is for composite keys, see also upsert().
   * @note If the hash table is (close to) full and resizable is true, this
   *       routine might result in rebuilding the entire hash table.
   *
   * @param key The key.
   * @param initial_value If there was not already a preexisting entry in this
   *        HashTable for the specified key, then the value will be initialized
   *        with a copy of initial_value. This parameter is ignored if a value
   *        is already present for key.
   * @param functor A pointer to a functor, which should provide a call
   *        operator which takes ValueT* as an argument. The call operator will
   *        be invoked once on the value corresponding to key (which may be
   *        newly inserted and default-constructed).
   * @return True on success, false if upsert failed because there was not
   *         enough space to insert a new entry in this HashTable.
   **/
  template <typename FunctorT>
  bool upsertCompositeKey(const std::vector<TypedValue> &key,
                          const ValueT &initial_value,
                          FunctorT *functor);

  /**
   * @brief Apply a functor to (multiple) entries in this hash table, with keys
   *        drawn from a ValueAccessor. New values are first inserted if not
   *        already present.
   *
   * @warning This method is only usable if allow_duplicate_keys is false.
   * @warning This method is threadsafe with regard to other calls to upsert(),
   *          upsertCompositeKey(), upsertValueAccessor(), and
   *          upsertValueAccessorCompositeKey(), but should not be used
   *          simultaneously with put(), putCompositeKey(), putValueAccessor(),
   *          or putValueAccessorCompositeKey().
   * @warning The ValueAccessor reference and ValueT* pointer passed to
   *          functor's call operator are only guaranteed to be valid for the
   *          duration of the call. The functor should not store a copy of
   *          these pointers and assume that they remain valid.
   * @warning Although this method itself is threadsafe, the ValueT object
   *          accessed by functor is not guaranteed to be (although it is
   *          guaranteed that its initial insertion will be atomic). If it is
   *          possible for multiple threads to call upsertValueAccessor() with
   *          the same key at the same time, then their access to ValueT should
   *          be made threadsafe (e.g. with the use of atomic types, mutexes,
   *          or some other external synchronization).
   * @note This version is for single scalar keys, see also
   *       upsertValueAccessorCompositeKey().
   * @note If the hash table is (close to) full and resizable is true, this
   *       routine might result in rebuilding the entire hash table.
   *
   * @param accessor A ValueAccessor which will be used to access keys.
   *        beginIteration() should be called on accessor before calling this
   *        method.
   * @param key_attr_id The attribute ID of the keys to be read from accessor.
   * @param check_for_null_keys If true, each key will be checked to see if it
   *        is null before upserting it (null keys are skipped). This must be
   *        set to true if some of the keys that will be read from accessor may
   *        be null.
   * @param functor A pointer to a functor, which should provide a call
   *        operator that takes two arguments: const ValueAccessor& (or better
   *        yet, a templated call operator which takes a const reference to
   *        some subclass of ValueAccessor as its first argument) and ValueT*.
   *        The call operator will be invoked once for every tuple with a
   *        non-null key in accessor.
   * @return True on success, false if upsert failed because there was not
   *         enough space to insert new entries for all the keys in accessor
   *         (note that some entries may still have been upserted, and
   *         accessor's iteration will be left on the first tuple which could
   *         not be inserted).
   **/
  template <typename FunctorT>
  bool upsertValueAccessor(ValueAccessor *accessor,
                           const attribute_id key_attr_id,
                           const bool check_for_null_keys,
                           const ValueT &initial_value,
                           FunctorT *functor);

  /**
   * @brief Apply a functor to (multiple) entries in this hash table, with keys
   *        drawn from a ValueAccessor. New values are first inserted if not
   *        already present. Composite key version.
   *
   * @warning This method is only usable if allow_duplicate_keys is false.
   * @warning This method is threadsafe with regard to other calls to upsert(),
   *          upsertCompositeKey(), upsertValueAccessor(), and
   *          upsertValueAccessorCompositeKey(), but should not be used
   *          simultaneously with put(), putCompositeKey(), putValueAccessor(),
   *          or putValueAccessorCompositeKey().
   * @warning The ValueAccessor reference and ValueT* pointer passed to
   *          functor's call operator are only guaranteed to be valid for the
   *          duration of the call. The functor should not store a copy of
   *          these pointers and assume that they remain valid.
   * @warning Although this method itself is threadsafe, the ValueT object
   *          accessed by functor is not guaranteed to be (although it is
   *          guaranteed that its initial insertion will be atomic). If it is
   *          possible for multiple threads to call upsertValueAccessor() with
   *          the same key at the same time, then their access to ValueT should
   *          be made threadsafe (e.g. with the use of atomic types, mutexes,
   *          or some other external synchronization).
   * @note This version is for composite keys, see also upsertValueAccessor().
   * @note If the hash table is (close to) full and resizable is true, this
   *       routine might result in rebuilding the entire hash table.
   *
   * @param accessor A ValueAccessor which will be used to access keys.
   *        beginIteration() should be called on accessor before calling this
   *        method.
   * @param key_attr_ids The attribute IDs of each key component to be read
   *        from accessor.
   * @param check_for_null_keys If true, each key will be checked to see if it
   *        is null before upserting it (null keys are skipped). This must be
   *        set to true if some of the keys that will be read from accessor may
   *        be null.
   * @param functor A pointer to a functor, which should provide a call
   *        operator that takes two arguments: const ValueAccessor& (or better
   *        yet, a templated call operator which takes a const reference to
   *        some subclass of ValueAccessor as its first argument) and ValueT*.
   *        The call operator will be invoked once for every tuple with a
   *        non-null key in accessor.
   * @return True on success, false if upsert failed because there was not
   *         enough space to insert new entries for all the keys in accessor
   *         (note that some entries may still have been upserted, and
   *         accessor's iteration will be left on the first tuple which could
   *         not be inserted).
   **/
  template <typename FunctorT>
  bool upsertValueAccessorCompositeKey(
      ValueAccessor *accessor,
      const std::vector<attribute_id> &key_attr_ids,
      const bool check_for_null_keys,
      const ValueT &initial_value,
      FunctorT *functor);

  /**
   * @brief Determine the number of entries (key-value pairs) contained in this
   *        HashTable.
   * @note For some HashTable implementations, this is O(1), but for others it
   *       may be O(n) where n is the number of buckets.
   *
   * @warning This method assumes that no concurrent calls to put(),
   *          putCompositeKey(), putValueAccessor(),
   *          putValueAccessorCompositeKey(), upsert(), upsertCompositeKey(),
   *          upsertValueAccessor(), or upsertValueAccessorCompositeKey() are
   *          taking place (i.e. that this HashTable is immutable for the
   *          duration of the call). Concurrent calls to getSingle(),
   *          getSingleCompositeKey(), getAll(), getAllCompositeKey(),
   *          getAllFromValueAccessor(), getAllFromValueAccessorCompositeKey(),
   *          forEach(), and forEachCompositeKey() are safe.
   *
   * @return The number of entries in this HashTable.
   **/
  virtual std::size_t numEntries() const = 0;

  /**
   * @brief Lookup a key against this hash table to find a matching entry.
   *
   * @warning Only usable with the hash table that does not allow duplicate
   *          keys.
   * @warning The key must not be null.
   * @warning This method assumes that no concurrent calls to put(),
   *          putCompositeKey(), putValueAccessor(),
   *          putValueAccessorCompositeKey(), upsert(), upsertCompositeKey(),
   *          upsertValueAccessor(), or upsertValueAccessorCompositeKey() are
   *          taking place (i.e. that this HashTable is immutable for the
   *          duration of the call and as long as the returned pointer may be
   *          dereferenced). Concurrent calls to getSingle(),
   *          getSingleCompositeKey(), getAll(), getAllCompositeKey(),
   *          getAllFromValueAccessor(), getAllFromValueAccessorCompositeKey(),
   *          forEach(), and forEachCompositeKey() are safe.
   * @note This version is for single scalar keys. See also
   *       getSingleCompositeKey().
   *
   * @param key The key to look up.
   * @return The value of a matched entry if a matching key is found.
   *         Otherwise, return NULL.
   **/
  virtual const ValueT* getSingle(const TypedValue &key) const = 0;

  /**
   * @brief Lookup a composite key against this hash table to find a matching
   *        entry.
   *
   * @warning Only usable with the hash table that does not allow duplicate
   *          keys.
   * @warning The key must not be null.
   * @warning This method assumes that no concurrent calls to put(),
   *          putCompositeKey(), putValueAccessor(),
   *          putValueAccessorCompositeKey(), upsert(), upsertCompositeKey(),
   *          upsertValueAccessor(), or upsertValueAccessorCompositeKey() are
   *          taking place (i.e. that this HashTable is immutable for the
   *          duration of the call and as long as the returned pointer may be
   *          dereferenced). Concurrent calls to getSingle(),
   *          getSingleCompositeKey(), getAll(), getAllCompositeKey(),
   *          getAllFromValueAccessor(), getAllFromValueAccessorCompositeKey(),
   *          forEach(), and forEachCompositeKey() are safe.
   * @note This version is for composite keys. See also getSingle().
   *
   * @param key The key to look up.
   * @return The value of a matched entry if a matching key is found.
   *         Otherwise, return NULL.
   **/
  virtual const ValueT* getSingleCompositeKey(const std::vector<TypedValue> &key) const = 0;

  /**
   * @brief Lookup a key against this hash table to find matching entries.
   *
   * @warning The key must not be null.
   * @warning This method assumes that no concurrent calls to put(),
   *          putCompositeKey(), putValueAccessor(),
   *          putValueAccessorCompositeKey(), upsert(), upsertCompositeKey(),
   *          upsertValueAccessor(), or upsertValueAccessorCompositeKey() are
   *          taking place (i.e. that this HashTable is immutable for the
   *          duration of the call and as long as the returned pointer may be
   *          dereferenced). Concurrent calls to getSingle(),
   *          getSingleCompositeKey(), getAll(), getAllCompositeKey(),
   *          getAllFromValueAccessor(), getAllFromValueAccessorCompositeKey(),
   *          forEach(), and forEachCompositeKey() are safe.
   * @note It is more efficient to call getSingle() if the hash table does not
   *       allow duplicate keys.
   * @note This version is for single scalar keys. See also
   *       getAllCompositeKey().
   *
   * @param key The key to look up.
   * @param values A vector to hold values of all matching entries. Matches
   *        will be appended to the vector.
   **/
  virtual void getAll(const TypedValue &key, std::vector<const ValueT*> *values) const = 0;

  /**
   * @brief Lookup a composite key against this hash table to find matching
   *        entries.
   *
   * @warning The key must not be null.
   * @warning This method assumes that no concurrent calls to put(),
   *          putCompositeKey(), putValueAccessor(),
   *          putValueAccessorCompositeKey(), upsert(), upsertCompositeKey(),
   *          upsertValueAccessor(), or upsertValueAccessorCompositeKey() are
   *          taking place (i.e. that this HashTable is immutable for the
   *          duration of the call and as long as the returned pointer may be
   *          dereferenced). Concurrent calls to getSingle(),
   *          getSingleCompositeKey(), getAll(), getAllCompositeKey(),
   *          getAllFromValueAccessor(), getAllFromValueAccessorCompositeKey(),
   *          forEach(), and forEachCompositeKey() are safe.
   * @note It is more efficient to call getSingleCompositeKey() if the hash
   *       table does not allow duplicate keys.
   * @note This version is for composite keys. See also getAll().
   *
   * @param key The key to look up.
   * @param values A vector to hold values of all matching entries. Matches
   *        will be appended to the vector.
   **/
  virtual void getAllCompositeKey(const std::vector<TypedValue> &key,
                                  std::vector<const ValueT*> *values) const = 0;

  /**
   * @brief Lookup (multiple) keys from a ValueAccessor and apply a functor to
   *        the matching values.
   *
   * @warning This method assumes that no concurrent calls to put(),
   *          putCompositeKey(), putValueAccessor(),
   *          putValueAccessorCompositeKey(), upsert(), upsertCompositeKey(),
   *          upsertValueAccessor(), or upsertValueAccessorCompositeKey() are
   *          taking place (i.e. that this HashTable is immutable for the
   *          duration of the call and as long as the returned pointer may be
   *          dereferenced). Concurrent calls to getSingle(),
   *          getSingleCompositeKey(), getAll(), getAllCompositeKey(),
   *          getAllFromValueAccessor(), getAllFromValueAccessorCompositeKey(),
   *          forEach(), and forEachCompositeKey() are safe.
   * @note This version is for single scalar keys. See also
   *       getAllFromValueAccessorCompositeKey().
   *
   * @param accessor A ValueAccessor which will be used to access keys.
   *        beginIteration() should be called on accessor before calling this
   *        method.
   * @param key_attr_id The attribute ID of the keys to be read from accessor.
   * @param check_for_null_keys If true, each key will be checked to see if it
   *        is null before looking it up (null keys are skipped). This must be
   *        set to true if some of the keys that will be read from accessor may
   *        be null.
   * @param functor A pointer to a functor, which should provide a call
   *        operator that takes 2 arguments: const ValueAccessor& (or better
   *        yet, a templated call operator which takes a const reference to
   *        some subclass of ValueAccessor as its first argument) and
   *        const ValueT&. The functor will be invoked once for each pair of a
   *        key taken from accessor and matching value.
   **/
  template <typename FunctorT>
  void getAllFromValueAccessor(ValueAccessor *accessor,
                               const attribute_id key_attr_id,
                               const bool check_for_null_keys,
                               FunctorT *functor) const;

  /**
   * @brief Lookup (multiple) keys from a ValueAccessor, apply a functor to
   *        the matching values and additionally call a hasMatch() function of
   *        the functor when the first match for a key is found.
   * @warning This method assumes that no concurrent calls to put(),
   *          putCompositeKey(), putValueAccessor(),
   *          putValueAccessorCompositeKey(), upsert(), upsertCompositeKey(),
   *          upsertValueAccessor(), or upsertValueAccessorCompositeKey() are
   *          taking place (i.e. that this HashTable is immutable for the
   *          duration of the call and as long as the returned pointer may be
   *          dereferenced). Concurrent calls to getSingle(),
   *          getSingleCompositeKey(), getAll(), getAllCompositeKey(),
   *          getAllFromValueAccessor(), getAllFromValueAccessorCompositeKey(),
   *          forEach(), and forEachCompositeKey() are safe.
   * @note This version is for single scalar keys. See also
   *       getAllFromValueAccessorCompositeKeyWithExtraWorkForFirstMatch().
   *
   * @param accessor A ValueAccessor which will be used to access keys.
   *        beginIteration() should be called on accessor before calling this
   *        method.
   * @param key_attr_id The attribute ID of the keys to be read from accessor.
   * @param check_for_null_keys If true, each key will be checked to see if it
   *        is null before looking it up (null keys are skipped). This must be
   *        set to true if some of the keys that will be read from accessor may
   *        be null.
   * @param functor A pointer to a functor, which should provide two functions:
   *        1) An operator that takes 2 arguments: const ValueAccessor& (or better
   *        yet, a templated call operator which takes a const reference to
   *        some subclass of ValueAccessor as its first argument) and
   *        const ValueT&. The operator will be invoked once for each pair of a
   *        key taken from accessor and matching value.
   *        2) A function hasMatch that takes 1 argument: const ValueAccessor&.
   *        The function will be called only once for a key from accessor when
   *        the first match is found.
   */
  template <typename FunctorT>
  void getAllFromValueAccessorWithExtraWorkForFirstMatch(
      ValueAccessor *accessor,
      const attribute_id key_attr_id,
      const bool check_for_null_keys,
      FunctorT *functor) const;

  /**
   * @brief Lookup (multiple) keys from a ValueAccessor, apply a functor to
   *        the matching values and additionally call a hasMatch() function of
   *        the functor when the first match for a key is found.  Composite key version.
   * @warning This method assumes that no concurrent calls to put(),
   *          putCompositeKey(), putValueAccessor(),
   *          putValueAccessorCompositeKey(), upsert(), upsertCompositeKey(),
   *          upsertValueAccessor(), or upsertValueAccessorCompositeKey() are
   *          taking place (i.e. that this HashTable is immutable for the
   *          duration of the call and as long as the returned pointer may be
   *          dereferenced). Concurrent calls to getSingle(),
   *          getSingleCompositeKey(), getAll(), getAllCompositeKey(),
   *          getAllFromValueAccessor(), getAllFromValueAccessorCompositeKey(),
   *          forEach(), and forEachCompositeKey() are safe..
   *
   * @param accessor A ValueAccessor which will be used to access keys.
   *        beginIteration() should be called on accessor before calling this
   *        method.
   * @param key_attr_id The attribute ID of the keys to be read from accessor.
   * @param check_for_null_keys If true, each key will be checked to see if it
   *        is null before looking it up (null keys are skipped). This must be
   *        set to true if some of the keys that will be read from accessor may
   *        be null.
   * @param functor A pointer to a functor, which should provide two functions:
   *        1) An operator that takes 2 arguments: const ValueAccessor& (or better
   *        yet, a templated call operator which takes a const reference to
   *        some subclass of ValueAccessor as its first argument) and
   *        const ValueT&. The operator will be invoked once for each pair of a
   *        key taken from accessor and matching value.
   *        2) A function hasMatch that takes 1 argument: const ValueAccessor&.
   *        The function will be called only once for a key from accessor when
   *        the first match is found.
   */
  template <typename FunctorT>
  void getAllFromValueAccessorCompositeKeyWithExtraWorkForFirstMatch(
      ValueAccessor *accessor,
      const std::vector<attribute_id> &key_attr_ids,
      const bool check_for_null_keys,
      FunctorT *functor) const;

  /**
   * @brief Lookup (multiple) keys from a ValueAccessor and apply a functor to
   *        the matching values. Composite key version.
   *
   * @warning This method assumes that no concurrent calls to put(),
   *          putCompositeKey(), putValueAccessor(),
   *          putValueAccessorCompositeKey(), upsert(), upsertCompositeKey(),
   *          upsertValueAccessor(), or upsertValueAccessorCompositeKey() are
   *          taking place (i.e. that this HashTable is immutable for the
   *          duration of the call and as long as the returned pointer may be
   *          dereferenced). Concurrent calls to getSingle(),
   *          getSingleCompositeKey(), getAll(), getAllCompositeKey(),
   *          getAllFromValueAccessor(), getAllFromValueAccessorCompositeKey(),
   *          forEach(), and forEachCompositeKey() are safe.
   * @note This version is for composite keys. See also
   *       getAllFromValueAccessor().
   *
   * @param accessor A ValueAccessor which will be used to access keys.
   *        beginIteration() should be called on accessor before calling this
   *        method.
   * @param key_attr_ids The attribute IDs of each key component to be read
   *        from accessor.
   * @param check_for_null_keys If true, each key will be checked to see if it
   *        has a null component before inserting it (null keys are skipped).
   *        This must be set to true if some of the keys that will be read from
   *        accessor may be null.
   * @param functor A pointer to a functor, which should provide a call
   *        operator that takes 2 arguments: const ValueAccessor& (or better
   *        yet, a templated call operator which takes a const reference to
   *        some subclass of ValueAccessor as its first argument) and
   *        const ValueT&. The functor will be invoked once for each pair of a
   *        key taken from accessor and matching value.
   **/
  template <typename FunctorT>
  void getAllFromValueAccessorCompositeKey(ValueAccessor *accessor,
                                           const std::vector<attribute_id> &key_attr_ids,
                                           const bool check_for_null_keys,
                                           FunctorT *functor) const;

  /**
   * @brief Apply the functor to each key with a match in the hash table.
   *
   * @param accessor A ValueAccessor which will be used to access keys.
   *        beginIteration() should be called on accessor before calling this
   *        method.
   * @param key_attr_id The attribute ID of the keys to be read from accessor.
   * @param check_for_null_keys If true, each key will be checked to see if it
   *        is null before looking it up (null keys are skipped). This must be
   *        set to true if some of the keys that will be read from accessor may
   *        be null.
   * @param functor A pointer to a functor which should provide an operator that
   *        takes 1 argument: const ValueAccessor&. The operator will be called
   *        only once for a key from accessor if there is a match.
   */
  template <typename FunctorT>
  void runOverKeysFromValueAccessorIfMatchFound(ValueAccessor *accessor,
                                                const attribute_id key_attr_id,
                                                const bool check_for_null_keys,
                                                FunctorT *functor) const {
    return runOverKeysFromValueAccessor<true>(accessor,
                                              key_attr_id,
                                              check_for_null_keys,
                                              functor);
  }

  /**
   * @brief Apply the functor to each key with a match in the hash table.
   *
   * @param accessor A ValueAccessor which will be used to access keys.
   *        beginIteration() should be called on accessor before calling this
   *        method.
   * @param key_attr_id The attribute ID of the keys to be read from accessor.
   * @param check_for_null_keys If true, each key will be checked to see if it
   *        is null before looking it up (null keys are skipped). This must be
   *        set to true if some of the keys that will be read from accessor may
   *        be null.
   * @param functor A pointer to a functor which should provide an operator that
   *        takes 1 argument: const ValueAccessor&. The operator will be called
   *        only once for a key from accessor if there is a match.
   */
  template <typename FunctorT>
  void runOverKeysFromValueAccessorIfMatchFoundCompositeKey(
      ValueAccessor *accessor,
      const std::vector<attribute_id> &key_attr_ids,
      const bool check_for_null_keys,
      FunctorT *functor) const {
    return runOverKeysFromValueAccessorCompositeKey<true>(accessor,
                                                          key_attr_ids,
                                                          check_for_null_keys,
                                                          functor);
  }

  /**
   * @brief Apply the functor to each key without a match in the hash table.
   *
   * @param accessor A ValueAccessor which will be used to access keys.
   *        beginIteration() should be called on accessor before calling this
   *        method.
   * @param key_attr_id The attribute ID of the keys to be read from accessor.
   * @param check_for_null_keys If true, each key will be checked to see if it
   *        is null before looking it up (null keys are skipped). This must be
   *        set to true if some of the keys that will be read from accessor may
   *        be null.
   * @param functor A pointer to a functor which should provide an operator that
   *        takes 1 argument: const ValueAccessor&. The operator will be called
   *        only once for a key from accessor if there is no match.
   */
  template <typename FunctorT>
  void runOverKeysFromValueAccessorIfMatchNotFound(
      ValueAccessor *accessor,
      const attribute_id key_attr_id,
      const bool check_for_null_keys,
      FunctorT *functor) const {
    return runOverKeysFromValueAccessor<false>(accessor,
                                               key_attr_id,
                                               check_for_null_keys,
                                               functor);
  }

  /**
   * @brief Apply the functor to each key without a match in the hash table.
   *
   * @param accessor A ValueAccessor which will be used to access keys.
   *        beginIteration() should be called on accessor before calling this
   *        method.
   * @param key_attr_id The attribute ID of the keys to be read from accessor.
   * @param check_for_null_keys If true, each key will be checked to see if it
   *        is null before looking it up (null keys are skipped). This must be
   *        set to true if some of the keys that will be read from accessor may
   *        be null.
   * @param functor A pointer to a functor which should provide an operator that
   *        takes 1 argument: const ValueAccessor&. The operator will be called
   *        only once for a key from accessor if there is no match.
   */
  template <typename FunctorT>
  void runOverKeysFromValueAccessorIfMatchNotFoundCompositeKey(
      ValueAccessor *accessor,
      const std::vector<attribute_id> &key_attr_ids,
      const bool check_for_null_keys,
      FunctorT *functor) const {
    return runOverKeysFromValueAccessorCompositeKey<false>(accessor,
                                                           key_attr_ids,
                                                           check_for_null_keys,
                                                           functor);
  }

  /**
   * @brief Apply a functor to each key, value pair in this hash table.
   *
   * @warning This method assumes that no concurrent calls to put(),
   *          putCompositeKey(), putValueAccessor(),
   *          putValueAccessorCompositeKey(), upsert(), upsertCompositeKey(),
   *          upsertValueAccessor(), or upsertValueAccessorCompositeKey() are
   *          taking place (i.e. that this HashTable is immutable for the
   *          duration of the call and as long as the returned pointer may be
   *          dereferenced). Concurrent calls to getSingle(),
   *          getSingleCompositeKey(), getAll(), getAllCompositeKey(),
   *          getAllFromValueAccessor(), getAllFromValueAccessorCompositeKey(),
   *          forEach(), and forEachCompositeKey() are safe.
   * @note This version is for single scalar keys. See also
   *       forEachCompositeKey().
   *
   * @param functor A pointer to a functor, which should provide a call
   *        operator which takes 2 arguments: const TypedValue&, const ValueT&.
   *        The call operator will be invoked once on each key, value pair in
   *        this hash table (note that if allow_duplicate_keys is true,
   *        the call may occur multiple times for the same key with different
   *        values).
   * @return The number of key-value pairs visited.
   **/
  template <typename FunctorT>
  std::size_t forEach(FunctorT *functor) const;

  /**
   * @brief Apply a functor to each key, value pair in this hash table.
   *
   * @warning This method assumes that no concurrent calls to put(),
   *          putCompositeKey(), putValueAccessor(),
   *          putValueAccessorCompositeKey(), upsert(), upsertCompositeKey(),
   *          upsertValueAccessor(), or upsertValueAccessorCompositeKey() are
   *          taking place (i.e. that this HashTable is immutable for the
   *          duration of the call and as long as the returned pointer may be
   *          dereferenced). Concurrent calls to getSingle(),
   *          getSingleCompositeKey(), getAll(), getAllCompositeKey(),
   *          getAllFromValueAccessor(), getAllFromValueAccessorCompositeKey(),
   *          forEach(), and forEachCompositeKey() are safe.
   * @note This version is for composite keys. See also forEach().
   *
   * @param functor A pointer to a functor, which should provide a call
   *        operator which takes 2 arguments: const std::vector<TypedValue>&,
   *        const ValueT&. The call operator will be invoked once on each key,
   *        value pair in this hash table (note that if allow_duplicate_keys is
   *        true, the call may occur multiple times for the same key with
   *        different values).
   * @return The number of key-value pairs visited.
   **/
  template <typename FunctorT>
  std::size_t forEachCompositeKey(FunctorT *functor) const;

 protected:
  /**
   * @brief Constructor for new resizable hash table.
   *
   * @param key_types A vector of one or more types (>1 indicates a composite
   *        key).
   * @param num_entries The estimated number of entries this hash table will
   *        hold.
   * @param storage_manager The StorageManager to use (a StorageBlob will be
   *        allocated to hold this hash table's contents).
   * @param adjust_hashes If true, the hash of a key should be modified by
   *        applying AdjustHash() so that it does not collide with one of the
   *        special values kEmptyHash or kPendingHash. If false, the hash is
   *        used as-is.
   * @param use_scalar_literal_hash If true, the key is a single scalar literal
   *        (non-composite) that it is safe to use the simplified hash function
   *        TypedValue::getHashScalarLiteral() on. If false, the generic
   *        TypedValue::getHash() method will be used.
   * @param preallocate_supported If true, this HashTable overrides
   *        preallocateForBulkInsert() to allow bulk-allocation of resources
   *        (i.e. buckets and variable-length key storage) in a single up-front
   *        pass when bulk-inserting entries. If false, resources are allocated
   *        on the fly for each entry.
   **/
  HashTable(const std::vector<const Type*> &key_types,
            const std::size_t num_entries,
            StorageManager *storage_manager,
            const bool adjust_hashes,
            const bool use_scalar_literal_hash,
            const bool preallocate_supported)
        : key_types_(key_types),
          scalar_key_inline_(true),
          key_inline_(nullptr),
          adjust_hashes_(adjust_hashes),
          use_scalar_literal_hash_(use_scalar_literal_hash),
          preallocate_supported_(preallocate_supported),
          storage_manager_(storage_manager),
          hash_table_memory_(nullptr),
          hash_table_memory_size_(0) {
    DEBUG_ASSERT(resizable);
  }

  /**
   * @brief Constructor for non-resizable hash table.
   *
   * @param key_types A vector of one or more types (>1 indicates a composite
   *        key).
   * @param hash_table_memory A pointer to memory to use for this hash table.
   * @param hash_table_memory_size The size of hash_table_memory in bytes.
   * @param new_hash_table If true, this hash table is being constructed for
   *        the first time and hash_table_memory will be cleared. If false,
   *        reload a pre-existing hash table.
   * @param hash_table_memory_zeroed If new_hash_table is true, setting this to
   *        true means that this HashTable will assume that hash_table_memory
   *        has already been zeroed-out (any newly-allocated block or blob
   *        memory from StorageManager is zeroed-out). If false, this HashTable
   *        will explicitly zero-fill its memory as neccessary. This parameter
   *        has no effect when new_hash_table is false.
   * @param adjust_hashes If true, the hash of a key should be modified by
   *        applying AdjustHash() so that it does not collide with one of the
   *        special values kEmptyHash or kPendingHash. If false, the hash is
   *        used as-is.
   * @param use_scalar_literal_hash If true, the key is a single scalar literal
   *        (non-composite) that it is safe to use the simplified hash function
   *        TypedValue::getHashScalarLiteral() on. If false, the generic
   *        TypedValue::getHash() method will be used.
   * @param preallocate_supported If true, this HashTable overrides
   *        preallocateForBulkInsert() to allow bulk-allocation of resources
   *        (i.e. buckets and variable-length key storage) in a single up-front
   *        pass when bulk-inserting entries. If false, resources are allocated
   *        on the fly for each entry.
   **/
  HashTable(const std::vector<const Type*> &key_types,
            void *hash_table_memory,
            const std::size_t hash_table_memory_size,
            const bool new_hash_table,
            const bool hash_table_memory_zeroed,
            const bool adjust_hashes,
            const bool use_scalar_literal_hash,
            const bool preallocate_supported)
      : key_types_(key_types),
        scalar_key_inline_(true),
        key_inline_(nullptr),
        adjust_hashes_(adjust_hashes),
        use_scalar_literal_hash_(use_scalar_literal_hash),
        preallocate_supported_(preallocate_supported),
        storage_manager_(nullptr),
        hash_table_memory_(hash_table_memory),
        hash_table_memory_size_(hash_table_memory_size) {
    DEBUG_ASSERT(!resizable);
  }

  // Adjust 'hash' so that it is not exactly equal to either of the special
  // values kEmptyHash or kPendingHash.
  inline constexpr static std::size_t AdjustHash(const std::size_t hash) {
    return hash + (hash == kEmptyHash) - (hash == kPendingHash);
  }

  // Set information about which key components are stored inline. This usually
  // comes from a HashTableKeyManager, and is set by the constructor of a
  // subclass of HashTable.
  inline void setKeyInline(const std::vector<bool> *key_inline) {
    scalar_key_inline_ = key_inline->front();
    key_inline_ = key_inline;
  }

  // Generate a hash for a composite key by hashing each component of 'key' and
  // mixing their bits with CombineHashes().
  inline std::size_t hashCompositeKey(const std::vector<TypedValue> &key) const;

  // If 'force_key_copy' is true and some part of a composite key is
  // variable-length, calculate the total number of bytes for variable-length
  // key components that need to be copied. Otherwise, return 0 to indicate
  // that no variable-length copy is required.
  inline std::size_t calculateVariableLengthCompositeKeyCopySize(
      const std::vector<TypedValue> &key) const;

  // Helpers for put. If this HashTable is resizable, 'resize_shared_mutex_'
  // should be locked in shared mode before calling either of these methods.
  virtual HashTablePutResult putInternal(const TypedValue &key,
                                         const std::size_t variable_key_size,
                                         const ValueT &value,
                                         HashTablePreallocationState *prealloc_state) = 0;
  virtual HashTablePutResult putCompositeKeyInternal(const std::vector<TypedValue> &key,
                                                     const std::size_t variable_key_size,
                                                     const ValueT &value,
                                                     HashTablePreallocationState *prealloc_state) = 0;

  // Helpers for upsert. Both return a pointer to the value corresponding to
  // 'key'. If this HashTable is resizable, 'resize_shared_mutex_' should be
  // locked in shared mode while calling and using the returned pointer. May
  // return NULL if there is not enough space to insert a new key, in which
  // case a resizable HashTable should release the 'resize_shared_mutex_' and
  // call resize(), then try again.
  virtual ValueT* upsertInternal(const TypedValue &key,
                                 const std::size_t variable_key_size,
                                 const ValueT &initial_value) = 0;
  virtual ValueT* upsertCompositeKeyInternal(const std::vector<TypedValue> &key,
                                             const std::size_t variable_key_size,
                                             const ValueT &initial_value) = 0;

  // Helpers for forEach. Each return true on success, false if no more entries
  // exist to iterate over. After a successful call, '*key' is overwritten with
  // the key of the next entry, '*value' points to the associated value, and
  // '*entry_num' is incremented to the next (implementation defined) entry to
  // check ('*entry_num' should initially be set to zero).
  virtual bool getNextEntry(TypedValue *key,
                            const ValueT **value,
                            std::size_t *entry_num) const = 0;
  virtual bool getNextEntryCompositeKey(std::vector<TypedValue> *key,
                                        const ValueT **value,
                                        std::size_t *entry_num) const = 0;

  // Helpers for getAllFromValueAccessor. Each return true on success, false if
  // no more entries exist for the specified key. After a successful call,
  // '*value' points to the associated value, and '*entry_num' is incremented
  // to the next (implementation defined) entry to check ('*entry_num' should
  // initially be set to zero).
  virtual bool getNextEntryForKey(const TypedValue &key,
                                  const std::size_t hash_code,
                                  const ValueT **value,
                                  std::size_t *entry_num) const = 0;
  virtual bool getNextEntryForCompositeKey(const std::vector<TypedValue> &key,
                                           const std::size_t hash_code,
                                           const ValueT **value,
                                           std::size_t *entry_num) const = 0;

  // Return true if key exists in the hash table.
  virtual bool hasKey(const TypedValue &key) const = 0;
  virtual bool hasCompositeKey(const std::vector<TypedValue> &key) const = 0;

  // For a resizable HashTable, grow to accomodate more entries. If
  // 'extra_buckets' is not zero, it may serve as a "hint" to implementations
  // that at least the requested number of extra buckets are required when
  // resizing (mainly used in putValueAccessor() and
  // putValueAccessorCompositeKey() when 'preallocate_supported_' is true).
  // Implementations are free to ignore 'extra_buckets'. If
  // 'extra_variable_storage' is not zero, implementations will attempt to
  // allocate at least enough additional variable-key storage space to
  // accomodate the number of bytes specified. 'retry_num' is intended ONLY for
  // when resize() recursively calls itself and should not be set to nonzero by
  // any other caller.
  virtual void resize(const std::size_t extra_buckets,
                      const std::size_t extra_variable_storage,
                      const std::size_t retry_num = 0) = 0;

  // In the case where 'allow_duplicate_keys' is true, it is possible to
  // pre-calculate the number of key-value entries and the amount of
  // variable-length key storage that will be needed to insert all the
  // entries from a ValueAccessor in putValueAccessor() or
  // putValueAccessorCompositeKey() before actually inserting anything. Some
  // HashTable implemetations (notably SeparateChainingHashTable) can achieve
  // better performance by ammortizing the cost of allocating certain resources
  // (buckets and variable-length key storage) in one up-front allocation. This
  // method is intended to support that. Returns true and fills in
  // '*prealloc_state' if pre-allocation was successful. Returns false if a
  // resize() is needed.
  virtual bool preallocateForBulkInsert(const std::size_t total_entries,
                                        const std::size_t total_variable_key_size,
                                        HashTablePreallocationState *prealloc_state) {
    FATAL_ERROR("Called HashTable::preallocateForBulkInsert() on a HashTable "
                "implementation that does not support preallocation.");
  }

  // Type(s) of keys.
  const std::vector<const Type*> key_types_;

  // Information about whether key components are stored inline or in a
  // separate variable-length storage region. This is usually determined by a
  // HashTableKeyManager and set by calling setKeyInline().
  bool scalar_key_inline_;
  const std::vector<bool> *key_inline_;

  // Whether hashes should be adjusted by AdjustHash() before being used.
  const bool adjust_hashes_;
  // Whether it is safe to use the simplified TypedValue::getHashScalarLiteral()
  // method instead of the generic TypedValue::getHash() method.
  const bool use_scalar_literal_hash_;
  // Whether preallocateForBulkInsert() is supported by this HashTable.
  const bool preallocate_supported_;

  // Used only when resizable is true:
  StorageManager *storage_manager_;
  MutableBlobReference blob_;
  // Locked in shared mode for most operations, exclusive mode during resize.
  // Not locked at all for non-resizable HashTables.
  alignas(kCacheLineBytes) SpinSharedMutex<true> resize_shared_mutex_;

  // Used only when resizable is false:
  void *hash_table_memory_;
  const std::size_t hash_table_memory_size_;

 private:
  // Assign '*key_vector' with the attribute values specified by 'key_attr_ids'
  // at the current position of 'accessor'. If 'check_for_null_keys' is true,
  // stops and returns true if any of the values is null, otherwise returns
  // false.
  template <typename ValueAccessorT>
  inline static bool GetCompositeKeyFromValueAccessor(
      const ValueAccessorT &accessor,
      const std::vector<attribute_id> &key_attr_ids,
      const bool check_for_null_keys,
      std::vector<TypedValue> *key_vector) {
    for (std::vector<attribute_id>::size_type key_idx = 0;
         key_idx < key_attr_ids.size();
         ++key_idx) {
      (*key_vector)[key_idx] = accessor.getTypedValue(key_attr_ids[key_idx]);
      if (check_for_null_keys && (*key_vector)[key_idx].isNull()) {
        return true;
      }
    }
    return false;
  }

  // If run_if_match_found is true, apply the functor to each key if a match is
  // found; otherwise, apply the functor if no match is found.
  template <bool run_if_match_found, typename FunctorT>
  void runOverKeysFromValueAccessor(ValueAccessor *accessor,
                                    const attribute_id key_attr_id,
                                    const bool check_for_null_keys,
                                    FunctorT *functor) const;

  template <bool run_if_match_found, typename FunctorT>
  void runOverKeysFromValueAccessorCompositeKey(
      ValueAccessor *accessor,
      const std::vector<attribute_id> &key_attr_ids,
      const bool check_for_null_keys,
      FunctorT *functor) const;

  // Method containing the actual logic implementing getAllFromValueAccessor().
  // Has extra template parameters that control behavior to avoid some
  // inner-loop branching.
  template <typename FunctorT,
            bool check_for_null_keys,
            bool adjust_hashes_template,
            bool use_scalar_literal_hash_template>
  void getAllFromValueAccessorImpl(ValueAccessor *accessor,
                                   const attribute_id key_attr_id,
                                   FunctorT *functor) const;

  DISALLOW_COPY_AND_ASSIGN(HashTable);
};

/**
 * @brief An instantiation of the HashTable template for use in hash joins.
 * @note This assumes that duplicate keys are allowed. If it is known a priori
 *       that there are no duplicate keys (e.g. because of primary key or
 *       unique constraints on a table), then using a version of HashTable with
 *       allow_duplicate_keys = false can be more efficient.
 * @warning This has force_key_copy = false. Therefore, any blocks which keys
 *          are inserted from should remain memory-resident and not be
 *          reorganized or rebuilt for the lifetime of this JoinHashTable. If
 *          that is not possible, then a HashTable with force_key_copy = true
 *          must be used instead.
 **/
typedef HashTable<TupleReference, true, false, false, true> JoinHashTable;

template <template <typename, bool, bool, bool, bool> class HashTableImpl>
using JoinHashTableImpl = HashTableImpl<TupleReference, true, false, false, true>;

/**
 * @brief An instantiation of the HashTable template for use in aggregations.
 * @note This has force_key_copy = true, so that we don't have dangling pointers
 * to blocks that are evicted.
 **/
template <typename ValueT>
using AggregationStateHashTable = HashTable<ValueT, true, false, true, false>;

template <template <typename, bool, bool, bool, bool> class HashTableImpl, typename ValueT>
using AggregationStateHashTableImpl = HashTableImpl<ValueT, true, false, true, false>;

/** @} */

// ----------------------------------------------------------------------------
// Implementations of template class methods follow.

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
HashTablePutResult HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::put(const TypedValue &key,
          const ValueT &value) {
  const std::size_t variable_size = (force_key_copy && !scalar_key_inline_) ? key.getDataSize()
                                                                            : 0;
  if (resizable) {
    HashTablePutResult result = HashTablePutResult::kOutOfSpace;
    while (result == HashTablePutResult::kOutOfSpace) {
      {
        SpinSharedMutexSharedLock<true> lock(resize_shared_mutex_);
        result = putInternal(key, variable_size, value, nullptr);
      }
      if (result == HashTablePutResult::kOutOfSpace) {
        resize(0, variable_size);
      }
    }
    return result;
  } else {
    return putInternal(key, variable_size, value, nullptr);
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
HashTablePutResult HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::putCompositeKey(const std::vector<TypedValue> &key,
                      const ValueT &value) {
  const std::size_t variable_size = calculateVariableLengthCompositeKeyCopySize(key);
  if (resizable) {
    HashTablePutResult result = HashTablePutResult::kOutOfSpace;
    while (result == HashTablePutResult::kOutOfSpace) {
      {
        SpinSharedMutexSharedLock<true> lock(resize_shared_mutex_);
        result = putCompositeKeyInternal(key, variable_size, value, nullptr);
      }
      if (result == HashTablePutResult::kOutOfSpace) {
        resize(0, variable_size);
      }
    }
    return result;
  } else {
    return putCompositeKeyInternal(key, variable_size, value, nullptr);
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
template <typename FunctorT>
HashTablePutResult HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::putValueAccessor(ValueAccessor *accessor,
                       const attribute_id key_attr_id,
                       const bool check_for_null_keys,
                       FunctorT *functor) {
  HashTablePutResult result = HashTablePutResult::kOutOfSpace;
  std::size_t variable_size;
  HashTablePreallocationState prealloc_state;
  bool using_prealloc = allow_duplicate_keys && preallocate_supported_;
  return InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> HashTablePutResult {  // NOLINT(build/c++11)
    if (using_prealloc) {
      std::size_t total_entries = 0;
      std::size_t total_variable_key_size = 0;
      if (check_for_null_keys || (force_key_copy && !scalar_key_inline_)) {
        // If we need to filter out nulls OR make variable copies, make a
        // prepass over the ValueAccessor.
        while (accessor->next()) {
          TypedValue key = accessor->getTypedValue(key_attr_id);
          if (check_for_null_keys && key.isNull()) {
            continue;
          }
          ++total_entries;
          total_variable_key_size += (force_key_copy && !scalar_key_inline_) ? key.getDataSize() : 0;
        }
        accessor->beginIteration();
      } else {
        total_entries = accessor->getNumTuples();
      }
      if (resizable) {
        bool prealloc_succeeded = false;
        while (!prealloc_succeeded) {
          {
            SpinSharedMutexSharedLock<true> lock(resize_shared_mutex_);
            prealloc_succeeded = this->preallocateForBulkInsert(total_entries,
                                                                total_variable_key_size,
                                                                &prealloc_state);
          }
          if (!prealloc_succeeded) {
            this->resize(total_entries, total_variable_key_size);
          }
        }
      } else {
        using_prealloc = this->preallocateForBulkInsert(total_entries,
                                                        total_variable_key_size,
                                                        &prealloc_state);
      }
    }
    if (resizable) {
      while (result == HashTablePutResult::kOutOfSpace) {
        {
          result = HashTablePutResult::kOK;
          SpinSharedMutexSharedLock<true> lock(resize_shared_mutex_);
          while (accessor->next()) {
            TypedValue key = accessor->getTypedValue(key_attr_id);
            if (check_for_null_keys && key.isNull()) {
              continue;
            }
            variable_size = (force_key_copy && !scalar_key_inline_) ? key.getDataSize() : 0;
            result = this->putInternal(key,
                                       variable_size,
                                       (*functor)(*accessor),
                                       using_prealloc ? &prealloc_state : nullptr);
            if (result == HashTablePutResult::kDuplicateKey) {
              DEBUG_ASSERT(!using_prealloc);
              return result;
            } else if (result == HashTablePutResult::kOutOfSpace) {
              DEBUG_ASSERT(!using_prealloc);
              break;
            }
          }
        }
        if (result == HashTablePutResult::kOutOfSpace) {
          this->resize(0, variable_size);
          accessor->previous();
        }
      }
    } else {
      while (accessor->next()) {
        TypedValue key = accessor->getTypedValue(key_attr_id);
        if (check_for_null_keys && key.isNull()) {
          continue;
        }
        variable_size = (force_key_copy && !scalar_key_inline_) ? key.getDataSize() : 0;
        result = this->putInternal(key,
                                   variable_size,
                                   (*functor)(*accessor),
                                   using_prealloc ? &prealloc_state : nullptr);
        if (result != HashTablePutResult::kOK) {
          return result;
        }
      }
    }

    return HashTablePutResult::kOK;
  });
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
template <typename FunctorT>
HashTablePutResult HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::putValueAccessorCompositeKey(ValueAccessor *accessor,
                                   const std::vector<attribute_id> &key_attr_ids,
                                   const bool check_for_null_keys,
                                   FunctorT *functor) {
  DEBUG_ASSERT(key_types_.size() == key_attr_ids.size());
  HashTablePutResult result = HashTablePutResult::kOutOfSpace;
  std::size_t variable_size;
  HashTablePreallocationState prealloc_state;
  bool using_prealloc = allow_duplicate_keys && preallocate_supported_;
  std::vector<TypedValue> key_vector;
  key_vector.resize(key_attr_ids.size());
  return InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> HashTablePutResult {  // NOLINT(build/c++11)
    if (using_prealloc) {
      std::size_t total_entries = 0;
      std::size_t total_variable_key_size = 0;
      if (check_for_null_keys || force_key_copy) {
        // If we need to filter out nulls OR make variable copies, make a
        // prepass over the ValueAccessor.
        while (accessor->next()) {
          if (this->GetCompositeKeyFromValueAccessor(*accessor,
                                                     key_attr_ids,
                                                     check_for_null_keys,
                                                     &key_vector)) {
            continue;
          }
          ++total_entries;
          total_variable_key_size += this->calculateVariableLengthCompositeKeyCopySize(key_vector);
        }
        accessor->beginIteration();
      } else {
        total_entries = accessor->getNumTuples();
      }
      if (resizable) {
        bool prealloc_succeeded = false;
        while (!prealloc_succeeded) {
          {
            SpinSharedMutexSharedLock<true> lock(resize_shared_mutex_);
            prealloc_succeeded = this->preallocateForBulkInsert(total_entries,
                                                                total_variable_key_size,
                                                                &prealloc_state);
          }
          if (!prealloc_succeeded) {
            this->resize(total_entries, total_variable_key_size);
          }
        }
      } else {
        using_prealloc = this->preallocateForBulkInsert(total_entries,
                                                        total_variable_key_size,
                                                        &prealloc_state);
      }
    }
    if (resizable) {
      while (result == HashTablePutResult::kOutOfSpace) {
        {
          result = HashTablePutResult::kOK;
          SpinSharedMutexSharedLock<true> lock(resize_shared_mutex_);
          while (accessor->next()) {
            if (this->GetCompositeKeyFromValueAccessor(*accessor,
                                                       key_attr_ids,
                                                       check_for_null_keys,
                                                       &key_vector)) {
              continue;
            }
            variable_size = this->calculateVariableLengthCompositeKeyCopySize(key_vector);
            result = this->putCompositeKeyInternal(key_vector,
                                                   variable_size,
                                                   (*functor)(*accessor),
                                                   using_prealloc ? &prealloc_state : nullptr);
            if (result == HashTablePutResult::kDuplicateKey) {
              DEBUG_ASSERT(!using_prealloc);
              return result;
            } else if (result == HashTablePutResult::kOutOfSpace) {
              DEBUG_ASSERT(!using_prealloc);
              break;
            }
          }
        }
        if (result == HashTablePutResult::kOutOfSpace) {
          this->resize(0, variable_size);
          accessor->previous();
        }
      }
    } else {
      while (accessor->next()) {
        if (this->GetCompositeKeyFromValueAccessor(*accessor,
                                                   key_attr_ids,
                                                   check_for_null_keys,
                                                   &key_vector)) {
          continue;
        }
        variable_size = this->calculateVariableLengthCompositeKeyCopySize(key_vector);
        result = this->putCompositeKeyInternal(key_vector,
                                               variable_size,
                                               (*functor)(*accessor),
                                               using_prealloc ? &prealloc_state : nullptr);
        if (result != HashTablePutResult::kOK) {
          return result;
        }
      }
    }

    return HashTablePutResult::kOK;
  });
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
template <typename FunctorT>
bool HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::upsert(const TypedValue &key,
             const ValueT &initial_value,
             FunctorT *functor) {
  DEBUG_ASSERT(!allow_duplicate_keys);
  const std::size_t variable_size = (force_key_copy && !scalar_key_inline_) ? key.getDataSize() : 0;
  if (resizable) {
    for (;;) {
      {
        SpinSharedMutexSharedLock<true> resize_lock(resize_shared_mutex_);
        ValueT *value = upsertInternal(key, variable_size, initial_value);
        if (value != nullptr) {
          (*functor)(value);
          return true;
        }
      }
      resize(0, force_key_copy && !scalar_key_inline_ ? key.getDataSize() : 0);
    }
  } else {
    ValueT *value = upsertInternal(key, variable_size, initial_value);
    if (value == nullptr) {
      return false;
    } else {
      (*functor)(value);
      return true;
    }
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
template <typename FunctorT>
bool HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::upsertCompositeKey(const std::vector<TypedValue> &key,
                         const ValueT &initial_value,
                         FunctorT *functor) {
  DEBUG_ASSERT(!allow_duplicate_keys);
  const std::size_t variable_size = calculateVariableLengthCompositeKeyCopySize(key);
  if (resizable) {
    for (;;) {
      {
        SpinSharedMutexSharedLock<true> resize_lock(resize_shared_mutex_);
        ValueT *value = upsertCompositeKeyInternal(key, variable_size, initial_value);
        if (value != nullptr) {
          (*functor)(value);
          return true;
        }
      }
      resize(0, variable_size);
    }
  } else {
    ValueT *value = upsertCompositeKeyInternal(key, variable_size, initial_value);
    if (value == nullptr) {
      return false;
    } else {
      (*functor)(value);
      return true;
    }
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
template <typename FunctorT>
bool HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::upsertValueAccessor(ValueAccessor *accessor,
                          const attribute_id key_attr_id,
                          const bool check_for_null_keys,
                          const ValueT &initial_value,
                          FunctorT *functor) {
  DEBUG_ASSERT(!allow_duplicate_keys);
  std::size_t variable_size;
  return InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> bool {  // NOLINT(build/c++11)
    if (resizable) {
      bool continuing = true;
      while (continuing) {
        {
          continuing = false;
          SpinSharedMutexSharedLock<true> lock(resize_shared_mutex_);
          while (accessor->next()) {
            TypedValue key = accessor->getTypedValue(key_attr_id);
            if (check_for_null_keys && key.isNull()) {
              continue;
            }
            variable_size = (force_key_copy && !scalar_key_inline_) ? key.getDataSize() : 0;
            ValueT *value = this->upsertInternal(key, variable_size, initial_value);
            if (value == nullptr) {
              continuing = true;
              break;
            } else {
              (*functor)(*accessor, value);
            }
          }
        }
        if (continuing) {
          this->resize(0, variable_size);
          accessor->previous();
        }
      }
    } else {
      while (accessor->next()) {
        TypedValue key = accessor->getTypedValue(key_attr_id);
        if (check_for_null_keys && key.isNull()) {
          continue;
        }
        variable_size = (force_key_copy && !scalar_key_inline_) ? key.getDataSize() : 0;
        ValueT *value = this->upsertInternal(key, variable_size, initial_value);
        if (value == nullptr) {
          return false;
        } else {
          (*functor)(*accessor, value);
        }
      }
    }

    return true;
  });
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
template <typename FunctorT>
bool HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::upsertValueAccessorCompositeKey(ValueAccessor *accessor,
                                      const std::vector<attribute_id> &key_attr_ids,
                                      const bool check_for_null_keys,
                                      const ValueT &initial_value,
                                      FunctorT *functor) {
  DEBUG_ASSERT(!allow_duplicate_keys);
  std::size_t variable_size;
  std::vector<TypedValue> key_vector;
  key_vector.resize(key_attr_ids.size());
  return InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> bool {  // NOLINT(build/c++11)
    if (resizable) {
      bool continuing = true;
      while (continuing) {
        {
          continuing = false;
          SpinSharedMutexSharedLock<true> lock(resize_shared_mutex_);
          while (accessor->next()) {
            if (this->GetCompositeKeyFromValueAccessor(*accessor,
                                                       key_attr_ids,
                                                       check_for_null_keys,
                                                       &key_vector)) {
              continue;
            }
            variable_size = this->calculateVariableLengthCompositeKeyCopySize(key_vector);
            ValueT *value = this->upsertCompositeKeyInternal(key_vector,
                                                             variable_size,
                                                             initial_value);
            if (value == nullptr) {
              continuing = true;
              break;
            } else {
              (*functor)(*accessor, value);
            }
          }
        }
        if (continuing) {
          this->resize(0, variable_size);
          accessor->previous();
        }
      }
    } else {
      while (accessor->next()) {
        if (this->GetCompositeKeyFromValueAccessor(*accessor,
                                                   key_attr_ids,
                                                   check_for_null_keys,
                                                   &key_vector)) {
          continue;
        }
        variable_size = this->calculateVariableLengthCompositeKeyCopySize(key_vector);
        ValueT *value = this->upsertCompositeKeyInternal(key_vector,
                                                         variable_size,
                                                         initial_value);
        if (value == nullptr) {
          return false;
        } else {
          (*functor)(*accessor, value);
        }
      }
    }

    return true;
  });
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
template <typename FunctorT>
void HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getAllFromValueAccessor(ValueAccessor *accessor,
                              const attribute_id key_attr_id,
                              const bool check_for_null_keys,
                              FunctorT *functor) const {
  // Pass through to method with additional template parameters for less
  // branching in inner loop.
  if (check_for_null_keys) {
    if (adjust_hashes_) {
      if (use_scalar_literal_hash_) {
        getAllFromValueAccessorImpl<FunctorT, true, true, true>(
            accessor, key_attr_id, functor);
      } else {
        getAllFromValueAccessorImpl<FunctorT, true, true, false>(
            accessor, key_attr_id, functor);
      }
    } else {
      if (use_scalar_literal_hash_) {
        getAllFromValueAccessorImpl<FunctorT, true, false, true>(
            accessor, key_attr_id, functor);
      } else {
        getAllFromValueAccessorImpl<FunctorT, true, false, false>(
            accessor, key_attr_id, functor);
      }
    }
  } else {
    if (adjust_hashes_) {
      if (use_scalar_literal_hash_) {
        getAllFromValueAccessorImpl<FunctorT, false, true, true>(
            accessor, key_attr_id, functor);
      } else {
        getAllFromValueAccessorImpl<FunctorT, false, true, false>(
            accessor, key_attr_id, functor);
      }
    } else {
      if (use_scalar_literal_hash_) {
        getAllFromValueAccessorImpl<FunctorT, false, false, true>(
            accessor, key_attr_id, functor);
      } else {
        getAllFromValueAccessorImpl<FunctorT, false, false, false>(
            accessor, key_attr_id, functor);
      }
    }
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
template <typename FunctorT>
void HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getAllFromValueAccessorCompositeKey(ValueAccessor *accessor,
                                          const std::vector<attribute_id> &key_attr_ids,
                                          const bool check_for_null_keys,
                                          FunctorT *functor) const {
  DEBUG_ASSERT(key_types_.size() == key_attr_ids.size());
  std::vector<TypedValue> key_vector;
  key_vector.resize(key_attr_ids.size());
  InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    while (accessor->next()) {
      bool null_key = false;
      for (std::vector<attribute_id>::size_type key_idx = 0;
           key_idx < key_types_.size();
           ++key_idx) {
        key_vector[key_idx] = accessor->getTypedValue(key_attr_ids[key_idx]);
        if (check_for_null_keys && key_vector[key_idx].isNull()) {
          null_key = true;
          break;
        }
      }
      if (null_key) {
        continue;
      }

      const std::size_t hash_code
          = adjust_hashes_ ? this->AdjustHash(this->hashCompositeKey(key_vector))
                           : this->hashCompositeKey(key_vector);
      std::size_t entry_num = 0;
      const ValueT *value;
      while (this->getNextEntryForCompositeKey(key_vector, hash_code, &value, &entry_num)) {
        (*functor)(*accessor, *value);
        if (!allow_duplicate_keys) {
          break;
        }
      }
    }
  });
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
template <typename FunctorT>
void HashTable<ValueT,
               resizable,
               serializable,
               force_key_copy,
               allow_duplicate_keys>::
    getAllFromValueAccessorWithExtraWorkForFirstMatch(
        ValueAccessor *accessor,
        const attribute_id key_attr_id,
        const bool check_for_null_keys,
        FunctorT *functor) const {
  InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    while (accessor->next()) {
      TypedValue key = accessor->getTypedValue(key_attr_id);
      if (check_for_null_keys && key.isNull()) {
        continue;
      }
      const std::size_t hash_code = adjust_hashes_ ? AdjustHash(key.getHash())
                                                   : key.getHash();
      std::size_t entry_num = 0;
      const ValueT *value;
      if (getNextEntryForKey(key, hash_code, &value, &entry_num)) {
        functor->hasMatch(*accessor);
        (*functor)(*accessor, *value);
        if (!allow_duplicate_keys) {
           continue;
         }
        while (getNextEntryForKey(key, hash_code, &value, &entry_num)) {
          (*functor)(*accessor, *value);
        }
      }
    }
  });  // NOLINT(whitespace/parens)
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
template <typename FunctorT>
void HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getAllFromValueAccessorCompositeKeyWithExtraWorkForFirstMatch(
        ValueAccessor *accessor,
        const std::vector<attribute_id> &key_attr_ids,
        const bool check_for_null_keys,
        FunctorT *functor) const {
  DEBUG_ASSERT(key_types_.size() == key_attr_ids.size());
  std::vector<TypedValue> key_vector;
  key_vector.resize(key_attr_ids.size());
  InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    while (accessor->next()) {
      bool null_key = false;
      for (std::vector<attribute_id>::size_type key_idx = 0;
           key_idx < key_types_.size();
           ++key_idx) {
        key_vector[key_idx] = accessor->getTypedValue(key_attr_ids[key_idx]);
        if (check_for_null_keys && key_vector[key_idx].isNull()) {
          null_key = true;
          break;
        }
      }
      if (null_key) {
        continue;
      }

      const std::size_t hash_code = adjust_hashes_ ? AdjustHash(hashCompositeKey(key_vector))
                                                   : hashCompositeKey(key_vector);
      std::size_t entry_num = 0;
      const ValueT *value;
      if (getNextEntryForCompositeKey(key_vector, hash_code, &value, &entry_num)) {
        functor->hasMatch(*accessor);
        (*functor)(*accessor, *value);
        if (!allow_duplicate_keys) {
          continue;
        }
        while (getNextEntryForCompositeKey(key_vector, hash_code, &value, &entry_num)) {
          (*functor)(*accessor, *value);
        }
      }
    }
  });  // NOLINT(whitespace/parens)
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
template <bool run_if_match_found, typename FunctorT>
void HashTable<ValueT,
               resizable,
               serializable,
               force_key_copy,
               allow_duplicate_keys>::
    runOverKeysFromValueAccessor(ValueAccessor *accessor,
                                 const attribute_id key_attr_id,
                                 const bool check_for_null_keys,
                                 FunctorT *functor) const {
  InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    while (accessor->next()) {
      TypedValue key = accessor->getTypedValue(key_attr_id);
      if (check_for_null_keys && key.isNull()) {
        if (!run_if_match_found) {
          (*functor)(*accessor);
          continue;
        }
      }
      if (run_if_match_found) {
        if (hasKey(key)) {
          (*functor)(*accessor);
        }
      } else {
        if (!hasKey(key)) {
          (*functor)(*accessor);
        }
      }
    }
  });  // NOLINT(whitespace/parens)
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
template <bool run_if_match_found, typename FunctorT>
void HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::runOverKeysFromValueAccessorCompositeKey(ValueAccessor *accessor,
                                               const std::vector<attribute_id> &key_attr_ids,
                                               const bool check_for_null_keys,
                                               FunctorT *functor) const {
  DEBUG_ASSERT(key_types_.size() == key_attr_ids.size());
  std::vector<TypedValue> key_vector;
  key_vector.resize(key_attr_ids.size());
  InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    while (accessor->next()) {
      bool null_key = false;
      for (std::vector<attribute_id>::size_type key_idx = 0;
           key_idx < key_types_.size();
           ++key_idx) {
        key_vector[key_idx] = accessor->getTypedValue(key_attr_ids[key_idx]);
        if (check_for_null_keys && key_vector[key_idx].isNull()) {
          null_key = true;
          break;
        }
      }
      if (null_key) {
        if (!run_if_match_found) {
          (*functor)(*accessor);
          continue;
        }
      }

      if (run_if_match_found) {
        if (hasCompositeKey(key_vector)) {
          (*functor)(*accessor);
        }
      } else if (!hasCompositeKey(key_vector)) {
        (*functor)(*accessor);
      }
    }
  });  // NOLINT(whitespace/parens)
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
template <typename FunctorT>
std::size_t HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::forEach(FunctorT *functor) const {
  std::size_t entries_visited = 0;
  std::size_t entry_num = 0;
  TypedValue key;
  const ValueT *value_ptr;
  while (getNextEntry(&key, &value_ptr, &entry_num)) {
    ++entries_visited;
    (*functor)(key, *value_ptr);
  }
  return entries_visited;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
template <typename FunctorT>
std::size_t HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::forEachCompositeKey(FunctorT *functor) const {
  std::size_t entries_visited = 0;
  std::size_t entry_num = 0;
  std::vector<TypedValue> key;
  const ValueT *value_ptr;
  while (getNextEntryCompositeKey(&key, &value_ptr, &entry_num)) {
    ++entries_visited;
    (*functor)(key, *value_ptr);
    key.clear();
  }
  return entries_visited;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
inline std::size_t HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::hashCompositeKey(const std::vector<TypedValue> &key) const {
  DEBUG_ASSERT(!key.empty());
  DEBUG_ASSERT(key.size() == key_types_.size());
  std::size_t hash = key.front().getHash();
  for (std::vector<TypedValue>::const_iterator key_it = key.begin() + 1;
       key_it != key.end();
       ++key_it) {
    hash = CombineHashes(hash, key_it->getHash());
  }
  return hash;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
inline std::size_t HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::calculateVariableLengthCompositeKeyCopySize(const std::vector<TypedValue> &key) const {
  DEBUG_ASSERT(!key.empty());
  DEBUG_ASSERT(key.size() == key_types_.size());
  if (force_key_copy) {
    std::size_t total = 0;
    for (std::vector<TypedValue>::size_type idx = 0;
         idx < key.size();
         ++idx) {
      if (!(*key_inline_)[idx]) {
        total += key[idx].getDataSize();
      }
    }
    return total;
  } else {
    return 0;
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
template <typename FunctorT,
          bool check_for_null_keys,
          bool adjust_hashes_template,
          bool use_scalar_literal_hash_template>
void HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getAllFromValueAccessorImpl(
        ValueAccessor *accessor,
        const attribute_id key_attr_id,
        FunctorT *functor) const {
  InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    while (accessor->next()) {
      TypedValue key = accessor->getTypedValue(key_attr_id);
      if (check_for_null_keys && key.isNull()) {
        continue;
      }
      const std::size_t true_hash = use_scalar_literal_hash_template ? key.getHashScalarLiteral()
                                                                     : key.getHash();
      const std::size_t adjusted_hash = adjust_hashes_template ? this->AdjustHash(true_hash)
                                                               : true_hash;
      std::size_t entry_num = 0;
      const ValueT *value;
      while (this->getNextEntryForKey(key, adjusted_hash, &value, &entry_num)) {
        (*functor)(*accessor, *value);
        if (!allow_duplicate_keys) {
          break;
        }
      }
    }
  });
}

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_HASH_TABLE_HPP_
