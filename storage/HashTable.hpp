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
#include "types/operations/comparisons/ComparisonUtil.hpp"
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
 * @brief Used internally by HashTables to keep track of pre-allocated
 *        resources used in putValueAccessor() and
 *        putValueAccessorCompositeKey().
 **/
struct HashTablePreallocationState {
  std::size_t bucket_position;
  std::size_t variable_length_key_position;
};

/**
 * @brief Base class for hash table.
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

  // A surrogate hash value for empty buckets. Keys which actually hash to this
  // value should have their hashes mutated (e.g. by adding 1). We use zero,
  // since we will often be using memory which is already zeroed-out and this
  // saves us the trouble of a memset. This has some downside, as the std::hash
  // function we use is the identity hash for integers, and the integer 0 is
  // common in many data sets and must be adjusted (and will then spuriously
  // collide with 1). Nevertheless, this expense is outweighed by no longer
  // having to memset large regions of memory when initializing a HashTable.
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
   **/
  HashTable(const std::vector<const Type*> &key_types,
            const std::size_t num_entries,
            StorageManager *storage_manager,
            const std::size_t key_start_in_bucket,
            const bool adjust_hashes,
            const bool preallocate_supported)
        : key_types_(key_types),
          fixed_key_size_(0),
          estimated_variable_key_size_(0),
          adjust_hashes_(adjust_hashes),
          preallocate_supported_(preallocate_supported),
          storage_manager_(storage_manager),
          hash_table_memory_(nullptr),
          hash_table_memory_size_(0),
          next_variable_length_key_offset_(0) {
    DEBUG_ASSERT(resizable);
    initializeKeyInfo(key_start_in_bucket);
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
   **/
  HashTable(const std::vector<const Type*> &key_types,
            void *hash_table_memory,
            const std::size_t hash_table_memory_size,
            const bool new_hash_table,
            const bool hash_table_memory_zeroed,
            const std::size_t key_start_in_bucket,
            const bool adjust_hashes,
            const bool preallocate_supported)
      : key_types_(key_types),
        fixed_key_size_(0),
        estimated_variable_key_size_(0),
        adjust_hashes_(adjust_hashes),
        preallocate_supported_(preallocate_supported),
        storage_manager_(nullptr),
        hash_table_memory_(hash_table_memory),
        hash_table_memory_size_(hash_table_memory_size),
        next_variable_length_key_offset_(0) {
    DEBUG_ASSERT(!resizable);
    initializeKeyInfo(key_start_in_bucket);
  }

  // Adjust 'hash' so that it is not exactly equal to either of the special
  // values kEmptyHash or kPendingHash.
  inline constexpr static std::size_t AdjustHash(const std::size_t hash) {
    return hash + (hash == kEmptyHash) - (hash == kPendingHash);
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

  // Reserve 'required_bytes' of variable-length storage. This can be undone by
  // calling deallocateVariableLengthKeyStorage(). Note that to actually use
  // the allocated storage, 'next_variable_length_key_offset_' should be read
  // incremented by a fetch_add(). This is a no-op if 'required_bytes' is 0.
  // Returns true if allocation successful, false if not enough unreserved
  // space was available.
  inline bool allocateVariableLengthKeyStorage(const std::size_t required_bytes);

  // Release 'bytes' of variable-length storage reserved by a previous call to
  // allocateVariableLengthKeyStorage(), but never actually used.
  inline void deallocateVariableLengthKeyStorage(const std::size_t bytes);

  // Get a pointer to a component of a key (specified by 'component_idx', 0 for
  // single scalar keys) in '*bucket'.
  inline const void* getKeyComponent(const void *bucket,
                                     const std::size_t component_idx) const;

  // Similar to getKeyComponent(), but returns a TypedValue instead (also
  // determines the size in bytes of the pointed-to value in order to construct
  // the TypedValue).
  inline TypedValue getKeyComponentTyped(const void *bucket,
                                         const std::size_t component_idx) const;

  // Check whether 'key' is actually equal to the scalar key stored in
  // '*bucket'.
  inline bool scalarKeyCollisionCheck(const TypedValue &key,
                                      const void *bucket) const;

  // Check whether 'key' is actually equal to the composite key stored in
  // '*bucket'.
  inline bool compositeKeyCollisionCheck(const std::vector<TypedValue> &key,
                                         const void *bucket) const;

  // Write 'component', the key component at 'component_idx' (0 for a single
  // scalar key) into '*bucket'.
  inline void writeKeyComponentToBucket(const TypedValue &component,
                                        const std::size_t component_idx,
                                        void *bucket,
                                        HashTablePreallocationState *prealloc_state);

  // Information about keys.
  const std::vector<const Type*> key_types_;
  std::size_t fixed_key_size_;
  std::size_t estimated_variable_key_size_;
  std::vector<std::size_t> key_offsets_;
  std::vector<bool> key_inline_;

  // Whether hashes should be adjusted by AdjustHash() before being used.
  const bool adjust_hashes_;
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

  void *variable_length_key_storage_;
  std::size_t variable_length_key_storage_size_;
  std::atomic<std::size_t> *variable_length_bytes_allocated_;
  // This is separate from '*variable_length_bytes_allocated_', because we
  // allocate storage for variable-length keys up-front but defer actually
  // copying them until we find an empty bucket.
  alignas(kCacheLineBytes) std::atomic<std::size_t>
      next_variable_length_key_offset_;

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

  void initializeKeyInfo(const std::size_t key_start_in_bucket);

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
  const std::size_t variable_size = (force_key_copy && !key_inline_.front()) ? key.getDataSize()
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
      if (check_for_null_keys || (force_key_copy && !key_inline_.front())) {
        // If we need to filter out nulls OR make variable copies, make a
        // prepass over the ValueAccessor.
        while (accessor->next()) {
          TypedValue key = accessor->getTypedValue(key_attr_id);
          if (check_for_null_keys && key.isNull()) {
            continue;
          }
          ++total_entries;
          total_variable_key_size += (force_key_copy && !key_inline_.front()) ? key.getDataSize() : 0;
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
            variable_size = (force_key_copy && !key_inline_.front()) ? key.getDataSize() : 0;
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
        variable_size = (force_key_copy && !key_inline_.front()) ? key.getDataSize() : 0;
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
  const std::size_t variable_size = (force_key_copy && !key_inline_.front()) ? key.getDataSize() : 0;
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
      resize(0, force_key_copy && !key_inline_.front() ? key.getDataSize() : 0);
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
            variable_size = (force_key_copy && !key_inline_.front()) ? key.getDataSize() : 0;
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
        variable_size = (force_key_copy && !key_inline_.front()) ? key.getDataSize() : 0;
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
  InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    while (accessor->next()) {
      TypedValue key = accessor->getTypedValue(key_attr_id);
      if (check_for_null_keys && key.isNull()) {
        continue;
      }
      const std::size_t hash_code = adjust_hashes_ ? this->AdjustHash(key.getHash())
                                                   : key.getHash();
      std::size_t entry_num = 0;
      const ValueT *value;
      while (this->getNextEntryForKey(key, hash_code, &value, &entry_num)) {
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
      if (!key_inline_[idx]) {
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
inline bool HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::allocateVariableLengthKeyStorage(const std::size_t required_bytes) {
  if (required_bytes == 0) {
    return true;
  }

  const std::size_t original_variable_length_bytes_allocated
      = variable_length_bytes_allocated_->fetch_add(required_bytes,
                                                    std::memory_order_relaxed);
  if (original_variable_length_bytes_allocated + required_bytes
      > variable_length_key_storage_size_) {
    // Release the memory we tried to allocate.
    variable_length_bytes_allocated_->fetch_sub(required_bytes,
                                                std::memory_order_relaxed);
    return false;
  } else {
    return true;
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
inline void HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::deallocateVariableLengthKeyStorage(const std::size_t bytes) {
  if (bytes == 0) {
    return;
  }
  variable_length_bytes_allocated_->fetch_sub(bytes,
                                              std::memory_order_relaxed);
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
inline const void* HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getKeyComponent(const void *bucket,
                      const std::size_t component_idx) const {
  DEBUG_ASSERT(component_idx < key_offsets_.size());

  const void *base_key_ptr = static_cast<const char*>(bucket)
                             + key_offsets_[component_idx];
  if (key_inline_[component_idx]) {
    // Component is inline.
    return base_key_ptr;
  } else if (force_key_copy) {
    // Component is copied into variable-length region.
    DEBUG_ASSERT(key_types_[component_idx]->isVariableLength());
    return static_cast<const char*>(variable_length_key_storage_)
           + *static_cast<const std::size_t*>(base_key_ptr);
  } else if (serializable) {
    // Relative pointer.
    return static_cast<const char*>(base_key_ptr)
           + *static_cast<const std::ptrdiff_t*>(base_key_ptr);
  } else {
    // Absolute pointer.
    return *static_cast<const void* const*>(base_key_ptr);
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
inline TypedValue HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getKeyComponentTyped(const void *bucket,
                           const std::size_t component_idx) const {
  DEBUG_ASSERT(component_idx < key_offsets_.size());

  const Type &key_type = *(key_types_[component_idx]);
  const void *base_key_ptr = static_cast<const char*>(bucket)
                             + key_offsets_[component_idx];
  if (key_inline_[component_idx]) {
    // Component is inline.
    DEBUG_ASSERT(!key_type.isVariableLength());
    return key_type.makeValue(base_key_ptr,
                              key_type.maximumByteLength());
  } else if (force_key_copy) {
    // Component is copied into variable-length region.
    DEBUG_ASSERT(key_type.isVariableLength());
    return key_type.makeValue(static_cast<const char*>(variable_length_key_storage_)
                                  + *static_cast<const std::size_t*>(base_key_ptr),
                              *(static_cast<const std::size_t*>(base_key_ptr) + 1));
  } else if (serializable) {
    // Relative pointer.
    return key_type.makeValue(
        static_cast<const char*>(base_key_ptr)
            + *static_cast<const std::ptrdiff_t*>(base_key_ptr),
        key_type.isVariableLength() ? *(static_cast<const std::size_t*>(base_key_ptr) + 1)
                                    : key_type.maximumByteLength());
  } else {
    // Absolute pointer.
    return key_type.makeValue(
        *static_cast<const void* const*>(base_key_ptr),
        key_type.isVariableLength() ? *(static_cast<const std::size_t*>(base_key_ptr) + 1)
                                    : key_type.maximumByteLength());
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
inline bool HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::scalarKeyCollisionCheck(const TypedValue &key,
                              const void *bucket) const {
  return CheckUntypedValuesEqual(*key_types_.front(),
                                 key.getDataPtr(),
                                 getKeyComponent(bucket, 0));
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
inline bool HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::compositeKeyCollisionCheck(const std::vector<TypedValue> &key,
                                 const void *bucket) const {
  DEBUG_ASSERT(key.size() == key_types_.size());

  // Check key components one-by-one.
  for (std::size_t idx = 0;
       idx < key_types_.size();
       ++idx) {
    if (!CheckUntypedValuesEqual(*key_types_[idx],
                                 key[idx].getDataPtr(),
                                 getKeyComponent(bucket, idx))) {
      return false;
    }
  }

  return true;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
inline void HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::writeKeyComponentToBucket(const TypedValue &component,
                                const std::size_t component_idx,
                                void *bucket,
                                HashTablePreallocationState *prealloc_state) {
  void *key_ptr = static_cast<char*>(bucket) + key_offsets_[component_idx];
  if (key_inline_[component_idx]) {
    // Copy key component directly into bucket.
    component.copyInto(key_ptr);
  } else if (force_key_copy) {
    // Copy key into variable-length storage region and store its offset and
    // length.
    DEBUG_ASSERT(key_types_[component_idx]->isVariableLength());
    const std::size_t component_size = component.getDataSize();
    const std::size_t variable_length_key_pos
        = (prealloc_state == nullptr)
            ? next_variable_length_key_offset_.fetch_add(component_size,
                                                         std::memory_order_relaxed)
            : prealloc_state->variable_length_key_position;
    if (prealloc_state != nullptr) {
      prealloc_state->variable_length_key_position += component_size;
    }
    DEBUG_ASSERT(variable_length_key_pos + component_size
                 <= variable_length_key_storage_size_);

    component.copyInto(static_cast<char*>(variable_length_key_storage_)
                       + variable_length_key_pos);
    *static_cast<std::size_t*>(key_ptr) = variable_length_key_pos;
    *(static_cast<std::size_t*>(key_ptr) + 1) = component_size;
  } else if (serializable) {
    // Store a relative pointer. We assume that we are pointing to memory
    // that is serialized together with this LinearOpenAddressingHashTable,
    // i.e. as part of the same StorageBlock. See comments regarding the
    // 'force_key_copy' parameter in storage/HashTable.hpp for more details.
    DEBUG_ASSERT(component.isReference());
    *static_cast<std::ptrdiff_t*>(key_ptr) = static_cast<const char*>(component.getDataPtr())
                                             - static_cast<const char*>(key_ptr);
    if (key_types_[component_idx]->isVariableLength()) {
      // Also store length if variable-length.
      *(static_cast<std::size_t*>(key_ptr) + 1) = component.getDataSize();
    }
  } else {
    // Store an absolute pointer.
    *static_cast<const void**>(key_ptr) = component.getDataPtr();
    if (key_types_[component_idx]->isVariableLength()) {
      // Also store length if variable-length.
      *(static_cast<std::size_t*>(key_ptr) + 1) = component.getDataSize();
    }
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
void HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::initializeKeyInfo(const std::size_t key_start_in_bucket) {
  DEBUG_ASSERT(!key_types_.empty());

  // Make sure key info is not already initialized.
  DEBUG_ASSERT(fixed_key_size_ == 0);
  DEBUG_ASSERT(estimated_variable_key_size_ == 0);
  DEBUG_ASSERT(key_offsets_.empty());
  DEBUG_ASSERT(key_inline_.empty());

  for (const Type *subkey_type : key_types_) {
    key_offsets_.push_back(key_start_in_bucket + fixed_key_size_);

    if (force_key_copy) {
      if (subkey_type->isVariableLength()) {
        // An offset into the variable length storage region, paired with a length.
        fixed_key_size_ += sizeof(size_t) * 2;
        estimated_variable_key_size_ += subkey_type->estimateAverageByteLength();
        key_inline_.push_back(false);
      } else {
        fixed_key_size_ += subkey_type->maximumByteLength();
        key_inline_.push_back(true);
      }
    } else {
      constexpr std::size_t ptr_size = serializable ? sizeof(std::ptrdiff_t)
                                                    : sizeof(const void*);
      if ((!subkey_type->isVariableLength())
          && (subkey_type->maximumByteLength() <= ptr_size + sizeof(size_t))) {
        fixed_key_size_ += subkey_type->maximumByteLength();
        key_inline_.push_back(true);
      } else {
        // A pointer to external memory, paired with a length.
        fixed_key_size_ += ptr_size + sizeof(size_t);
        key_inline_.push_back(false);
      }
    }
  }
}

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_HASH_TABLE_HPP_
