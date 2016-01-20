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

#ifndef QUICKSTEP_STORAGE_TESTS_HASHTABLE_UNITTEST_COMMON_HPP_
#define QUICKSTEP_STORAGE_TESTS_HASHTABLE_UNITTEST_COMMON_HPP_

#include <algorithm>
#include <atomic>
#include <bitset>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "catalog/CatalogTypedefs.hpp"
#include "storage/HashTable.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageConstants.hpp"
#include "storage/StorageManager.hpp"
#include "threading/Thread.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "utility/Macros.hpp"
#include "utility/ScopedBuffer.hpp"

// NetBSD's libc has atoll, but it doesn't show up in the std namespace for
// C++.
#ifndef __NetBSD__
using std::atoll;
#endif

namespace quickstep {

constexpr std::int64_t kNumSampleKeys = 1024;
// Not exactly aligned to word size so we can make sure there are no alignment
// problems for arbitrary-length types.
constexpr std::size_t kStringLengthParam = 18;
constexpr std::size_t kNumConcurrentThreads = 8;

class TestHashPayload {
 public:
  explicit TestHashPayload(const int payload)
      : internal_int_(payload) {
  }

  TestHashPayload(const TestHashPayload &orig)
      : internal_int_(orig.internal_int_.load()) {
  }

  std::atomic<int>* accessInternalInt() {
    return &internal_int_;
  }

  int loadInternalInt() const {
    return internal_int_.load(std::memory_order_relaxed);
  }

 private:
  std::atomic<int> internal_int_;
};

class NonTriviallyDestructibleTestHashPayload {
 public:
  NonTriviallyDestructibleTestHashPayload()
      : destruct_count_(nullptr) {
  }

  explicit NonTriviallyDestructibleTestHashPayload(std::atomic<int> *destruct_count)
      : destruct_count_(destruct_count) {
  }

  void clearDestructCountPtr() {
    destruct_count_ = nullptr;
  }

  ~NonTriviallyDestructibleTestHashPayload() {
    if (destruct_count_ != nullptr) {
      destruct_count_->fetch_add(1, std::memory_order_relaxed);
    }
  }

 private:
  std::atomic<int> *destruct_count_;
};

// Insertion functor that creates a TestHashPayload containing the tuple_id for
// each tuple visited in a ValueAccessor.
class TestInserter {
 public:
  template <typename ValueAccessorT>
  TestHashPayload operator()(const ValueAccessorT &accessor) {
    return TestHashPayload(accessor.getCurrentPosition());
  }
};

// Simple upserter that just atomically increments a TestHashPayload's value.
class TestUpserter {
 public:
  TestUpserter()
      : call_count_(0) {
  }

  void operator()(TestHashPayload *value) {
    ++call_count_;
    value->accessInternalInt()->fetch_add(1, std::memory_order_relaxed);
  }

  template <typename ValueAccessorT>
  void operator()(const ValueAccessorT &accessor, TestHashPayload *value) {
    ++call_count_;
    value->accessInternalInt()->fetch_add(1, std::memory_order_relaxed);
  }

  std::size_t call_count() const {
    return call_count_;
  }

 private:
  std::size_t call_count_;
};

// Visitor to test HashTable::forEach() and HashTable::forEachCompositeKey().
// For each key-value pair, extracts and remembers the key's original seed and
// the value stored in the TestHashPayload which it maps to.
class TestVisitor {
 public:
  TestVisitor()
      : test_key_type_id_(kNumTypeIDs) {
  }

  explicit TestVisitor(const TypeID test_key_type_id)
      : test_key_type_id_(test_key_type_id) {
  }

  void operator()(const TypedValue &key, const TestHashPayload &payload) {
    internal_map_.emplace(extractScalarSeed(key), payload.loadInternalInt());
  }

  void operator()(const std::vector<TypedValue> &key, const TestHashPayload &payload) {
    internal_map_.emplace(extractCompositeSeed(key), payload.loadInternalInt());
  }

  template <typename ValueAccessorT>
  void operator()(const ValueAccessorT &accessor, const TestHashPayload &payload) {
    std::vector<TypedValue> composite_key;
    composite_key.emplace_back(accessor.getTypedValue(0));
    composite_key.emplace_back(accessor.getTypedValue(1));
    composite_key.emplace_back(accessor.getTypedValue(2));

    internal_map_.emplace(extractCompositeSeed(composite_key),
                          payload.loadInternalInt());
  }

  const std::unordered_multimap<std::int64_t, int>& internal_map() const {
    return internal_map_;
  }

 private:
  std::int64_t extractScalarSeed(const TypedValue &key) {
    switch (test_key_type_id_) {
      case TypeID::kLong: {
        EXPECT_EQ(TypeID::kLong, key.getTypeID());
        return key.getLiteral<std::int64_t>();
      }
      case TypeID::kChar: {
        EXPECT_EQ(TypeID::kChar, key.getTypeID());
        return atoll(static_cast<const char*>(key.getDataPtr()));
      }
      case TypeID::kVarChar: {
        EXPECT_EQ(TypeID::kVarChar, key.getTypeID());
        return atoll(static_cast<const char*>(key.getDataPtr()));
      }
      default:
        FATAL_ERROR("TypeID not covered\n");
    }
  }

  std::int64_t extractCompositeSeed(const std::vector<TypedValue> &key) {
    EXPECT_EQ(3u, key.size());

    EXPECT_EQ(TypeID::kLong, key[0].getTypeID());
    const std::int64_t seed = key[0].getLiteral<std::int64_t>();

    EXPECT_EQ(TypeID::kChar, key[1].getTypeID());
    EXPECT_EQ(seed, atoll(static_cast<const char*>(key[1].getDataPtr())));

    EXPECT_EQ(TypeID::kVarChar, key[2].getTypeID());
    EXPECT_EQ(seed, atoll(static_cast<const char*>(key[2].getDataPtr())));

    return seed;
  }

  const TypeID test_key_type_id_;
  std::unordered_multimap<std::int64_t, int> internal_map_;
};

template <typename HashTableImpl>
class HashTableTest : public ::testing::Test {
 protected:
  class InserterThread : public Thread {
   public:
    InserterThread(const int my_id,
                   const std::vector<TypedValue> &keys,
                   HashTableImpl *hash_table)
        : my_id_(my_id),
          keys_(keys),
          hash_table_(hash_table),
          insert_succeeded_(keys.size(), false) {
      // Randomize insert order.
      for (std::vector<TypedValue>::size_type i = 0;
           i < keys_.size();
           ++i) {
        insert_order_.push_back(i);
      }
      std::random_shuffle(insert_order_.begin(), insert_order_.end());
    }

    int getID() const {
      return my_id_;
    }

    const std::vector<bool>& insert_succeeded() const {
      return insert_succeeded_;
    }

   protected:
    void run() override {
      for (const std::vector<TypedValue>::size_type i : insert_order_) {
        TestHashPayload value(i * kNumSampleKeys + my_id_);
        insert_succeeded_[i] = hash_table_->put(keys_[i], value) == HashTablePutResult::kOK;
      }
    }

   private:
    const int my_id_;
    const std::vector<TypedValue> &keys_;
    HashTableImpl *hash_table_;
    std::vector<std::vector<TypedValue>::size_type> insert_order_;
    std::vector<bool> insert_succeeded_;
  };

  class CompositeKeyInserterThread : public Thread {
   public:
    CompositeKeyInserterThread(const int my_id,
                               const std::vector<std::vector<TypedValue>> &keys,
                               HashTableImpl *hash_table)
        : my_id_(my_id),
          keys_(keys),
          hash_table_(hash_table),
          insert_succeeded_(keys.size(), false) {
      // Randomize insert order.
      for (std::vector<TypedValue>::size_type i = 0;
           i < keys_.size();
           ++i) {
        insert_order_.push_back(i);
      }
      std::random_shuffle(insert_order_.begin(), insert_order_.end());
    }

    int getID() const {
      return my_id_;
    }

    const std::vector<bool>& insert_succeeded() const {
      return insert_succeeded_;
    }

   protected:
    void run() override {
      for (const std::vector<std::vector<TypedValue>>::size_type i : insert_order_) {
        TestHashPayload value(i * kNumSampleKeys + my_id_);
        insert_succeeded_[i]
            = hash_table_->putCompositeKey(keys_[i], value) == HashTablePutResult::kOK;
      }
    }

   private:
    const int my_id_;
    const std::vector<std::vector<TypedValue>> &keys_;
    HashTableImpl *hash_table_;
    std::vector<std::vector<std::vector<TypedValue>>::size_type> insert_order_;
    std::vector<bool> insert_succeeded_;
  };

  virtual void SetUp() {
    storage_manager_.reset(new StorageManager("./test_data/"));
  }

  static std::vector<const Type*> SetupScalarKeyType(const TypeID type_id) {
    std::vector<const Type*> key_type_vec;
    switch (type_id) {
      case kLong:
        key_type_vec.push_back(&TypeFactory::GetType(kLong, false));
        break;
      case kChar:
        key_type_vec.push_back(&TypeFactory::GetType(kChar, kStringLengthParam, false));
        break;
      case kVarChar:
        key_type_vec.push_back(&TypeFactory::GetType(kVarChar, kStringLengthParam, false));
        break;
      default:
        FATAL_ERROR("TypeID not covered\n");
    }
    return key_type_vec;
  }

  static std::vector<const Type*> SetupCompositeKeyType() {
    std::vector<const Type*> key_type_vec;
    key_type_vec.push_back(&TypeFactory::GetType(kLong, false));
    key_type_vec.push_back(&TypeFactory::GetType(kChar, kStringLengthParam, false));
    key_type_vec.push_back(&TypeFactory::GetType(kVarChar, kStringLengthParam, false));
    return key_type_vec;
  }

  void createResizableHashTable(const std::vector<const Type*> &key_types,
                                const std::size_t num_entries) {
    hash_table_.reset(new HashTableImpl(key_types,
                                        num_entries,
                                        storage_manager_.get()));
  }

  void createNonresizableHashTable(const std::vector<const Type*> &key_types,
                                   const std::size_t buffer_size,
                                   const std::size_t offset_in_buffer) {
    assert(offset_in_buffer < buffer_size);
    memory_buffer_.reset(new ScopedBuffer(buffer_size, kCacheLineBytes));
    hash_table_.reset(new HashTableImpl(
        key_types,
        static_cast<char*>(memory_buffer_->get()) + offset_in_buffer,
        buffer_size - offset_in_buffer,
        true,
        false));
  }

  void createHashTableSimple(const std::vector<const Type*> &key_types,
                             const std::size_t num_entries) {
    if (HashTableImpl::template_resizable) {
      createResizableHashTable(key_types, num_entries);
    } else {
      // Allocate space for 'num_entries' generously.
      std::size_t key_size = 0;
      for (const Type *type : key_types) {
        key_size += type->maximumByteLength();
      }
      createNonresizableHashTable(
          key_types,
          256 + num_entries * 8 * (key_size + sizeof(typename HashTableImpl::value_type)),
          0);
    }
  }

  TypedValue createLongKey(const std::int64_t seed) const {
    return TypedValue(seed);
  }

  TypedValue createCharKey(const std::int64_t seed) {
    std::ostringstream stream;
    stream << seed;
    external_strings_.emplace_back(stream.str().size() + 1);
    std::memcpy(external_strings_.back().get(),
                stream.str().c_str(),
                stream.str().size() + 1);
    return TypeFactory::GetType(kChar, kStringLengthParam, false).makeValue(external_strings_.back().get(),
                                                                     stream.str().size() + 1);
  }

  TypedValue createVarCharKey(const std::int64_t seed) {
    std::ostringstream stream;
    stream << seed;
    external_strings_.emplace_back(stream.str().size() + 1);
    std::memcpy(external_strings_.back().get(),
                stream.str().c_str(),
                stream.str().size() + 1);
    return TypeFactory::GetType(kVarChar, kStringLengthParam, false).makeValue(external_strings_.back().get(),
                                                                        stream.str().size() + 1);
  }

  TypedValue createScalarKey(const TypeID test_key_type_id,
                             const std::int64_t seed) {
    switch (test_key_type_id) {
      case TypeID::kLong:
        return createLongKey(seed);
      case TypeID::kChar:
        return createCharKey(seed);
      case TypeID::kVarChar:
        return createVarCharKey(seed);
      default:
        FATAL_ERROR("TypeID not covered\n");
    }
  }

  std::vector<TypedValue> createCompositeKey(const std::int64_t seed) {
    std::vector<TypedValue> key;
    key.emplace_back(createLongKey(seed));
    key.emplace_back(createCharKey(seed));
    key.emplace_back(createVarCharKey(seed));
    return key;
  }

  ColumnVectorsValueAccessor* createSampleValueAccessor() {
    // Create a ColumnVectorsValueAccessor to simulate a table with 3 columns:
    // Long, Char, & VarChar.
    ColumnVectorsValueAccessor *accessor = new ColumnVectorsValueAccessor();

    NativeColumnVector *long_column
        = new NativeColumnVector(TypeFactory::GetType(kLong, true),
                                 kNumSampleKeys + 1);
    NativeColumnVector *char_column
        = new NativeColumnVector(TypeFactory::GetType(kChar, kStringLengthParam, true),
                                 kNumSampleKeys + 1);
    IndirectColumnVector *varchar_column
        = new IndirectColumnVector(TypeFactory::GetType(kVarChar, kStringLengthParam, true),
                                   kNumSampleKeys + 1);

    for (std::int64_t i = 0; i < kNumSampleKeys / 2; ++i) {
      long_column->appendTypedValue(createScalarKey(kLong, i * (i + 1)));
      char_column->appendTypedValue(createScalarKey(kChar, i * (i + 1)));
      varchar_column->appendTypedValue(createScalarKey(kVarChar, i * (i + 1)));
    }

    // Put some nulls in the middle of each column.
    long_column->appendTypedValue(TypedValue(kLong));
    char_column->appendTypedValue(TypedValue(kChar));
    varchar_column->appendTypedValue(TypedValue(kVarChar));

    // Finish filling the columns.
    for (std::int64_t i = kNumSampleKeys / 2;
         i < kNumSampleKeys;
         ++i) {
      long_column->appendTypedValue(createScalarKey(kLong, i * (i + 1)));
      char_column->appendTypedValue(createScalarKey(kChar, i * (i + 1)));
      varchar_column->appendTypedValue(createScalarKey(kVarChar, i * (i + 1)));
    }

    accessor->addColumn(long_column);
    accessor->addColumn(char_column);
    accessor->addColumn(varchar_column);

    return accessor;
  }

  void runScalarKeyPutAndGetTest(const TypeID test_key_type_id) {
    std::vector<const Type*> key_types = SetupScalarKeyType(test_key_type_id);
    createHashTableSimple(key_types, kNumSampleKeys);

    // Insert some key/value pairs.
    for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
      TypedValue key(createScalarKey(test_key_type_id, i * (i + 1)));
      TestHashPayload value(i);
      EXPECT_EQ(HashTablePutResult::kOK, hash_table_->put(key, value));
    }

    // Read them back out.
    for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
      TypedValue key(createScalarKey(test_key_type_id, i * (i + 1)));
      std::vector<TypedValue> key_vec(1, key);

      // Use getSingle() if duplicate keys are not allowed.
      if (!HashTableImpl::template_allow_duplicate_keys) {
        const TestHashPayload *value = hash_table_->getSingle(key);
        ASSERT_NE(nullptr, value);
        EXPECT_EQ(i, value->loadInternalInt());

        value = hash_table_->getSingleCompositeKey(key_vec);
        ASSERT_NE(nullptr, value);
        EXPECT_EQ(i, value->loadInternalInt());
      }

      // Test getAll() in all cases.
      std::vector<const TestHashPayload*> matches;
      hash_table_->getAll(key, &matches);
      ASSERT_EQ(1u, matches.size());
      EXPECT_EQ(i, matches.front()->loadInternalInt());

      matches.clear();
      hash_table_->getAllCompositeKey(key_vec, &matches);
      ASSERT_EQ(1u, matches.size());
      EXPECT_EQ(i, matches.front()->loadInternalInt());
    }

    // Check a nonexistent key.
    TypedValue nonexistent_key(createScalarKey(test_key_type_id,
                                               kNumSampleKeys * kNumSampleKeys * kNumSampleKeys));
    if (!HashTableImpl::template_allow_duplicate_keys) {
      const TestHashPayload *nonexistent_value = hash_table_->getSingle(nonexistent_key);
      EXPECT_EQ(nullptr, nonexistent_value);
    }
    std::vector<const TestHashPayload*> nonexistent_matches;
    hash_table_->getAll(nonexistent_key, &nonexistent_matches);
    EXPECT_TRUE(nonexistent_matches.empty());

    // Also test forEach().
    TestVisitor visitor(test_key_type_id);
    EXPECT_EQ(static_cast<std::size_t>(kNumSampleKeys),
              hash_table_->forEach(&visitor));
    EXPECT_EQ(static_cast<std::size_t>(kNumSampleKeys),
              visitor.internal_map().size());
    for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
      ASSERT_EQ(1u, visitor.internal_map().count(i * (i + 1)));
      EXPECT_EQ(i, visitor.internal_map().find(i * (i + 1))->second);
    }
  }

  void runDuplicateScalarKeysPutAndGetTest(const TypeID test_key_type_id) {
    std::vector<const Type*> key_types = SetupScalarKeyType(test_key_type_id);
    createHashTableSimple(key_types, kNumSampleKeys * 2);

    // Attempt to insert 1-3 values for each distinct key.
    std::size_t total_inserted = 0;
    for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
      TypedValue key(createScalarKey(test_key_type_id, i * (i + 1)));
      TestHashPayload first_value(i * kNumSampleKeys);
      EXPECT_EQ(HashTablePutResult::kOK, hash_table_->put(key, first_value));
      ++total_inserted;

      for (int extra_value_num = 1;
           extra_value_num <= i % 3;
           ++extra_value_num) {
        TestHashPayload additional_value(i * kNumSampleKeys + extra_value_num);
        if (HashTableImpl::template_allow_duplicate_keys) {
          EXPECT_EQ(HashTablePutResult::kOK, hash_table_->put(key, additional_value));
          ++total_inserted;
        } else {
          EXPECT_EQ(HashTablePutResult::kDuplicateKey, hash_table_->put(key, additional_value));
        }
      }
    }

    // Read them back out.
    for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
      TypedValue key(createScalarKey(test_key_type_id, i * (i + 1)));
      std::vector<TypedValue> key_vec(1, key);

      std::vector<const TestHashPayload*> matches;
      hash_table_->getAll(key, &matches);
      if (HashTableImpl::template_allow_duplicate_keys) {
        EXPECT_EQ(static_cast<std::size_t>((i % 3) + 1), matches.size());
        std::bitset<3> value_found;
        for (const TestHashPayload *match : matches) {
          ASSERT_LE(match->loadInternalInt() - i * kNumSampleKeys, 3);
          EXPECT_FALSE(value_found[match->loadInternalInt() - i * kNumSampleKeys]);
          value_found.set(match->loadInternalInt() - i * kNumSampleKeys, true);
        }
        EXPECT_TRUE(value_found[0]);
        if (i % 3 > 0) {
          EXPECT_TRUE(value_found[1]);
        } else {
          EXPECT_FALSE(value_found[1]);
        }
        if (i % 3 > 1) {
          EXPECT_TRUE(value_found[2]);
        } else {
          EXPECT_FALSE(value_found[2]);
        }
      } else {
        ASSERT_EQ(1u, matches.size());
        EXPECT_EQ(i * kNumSampleKeys,
                  matches.front()->loadInternalInt());
      }
    }

    // Check a nonexistent key.
    TypedValue nonexistent_key(createScalarKey(test_key_type_id,
                                               kNumSampleKeys * kNumSampleKeys * kNumSampleKeys));
    std::vector<const TestHashPayload*> nonexistent_matches;
    hash_table_->getAll(nonexistent_key, &nonexistent_matches);
    EXPECT_TRUE(nonexistent_matches.empty());

    // Also test forEach().
    TestVisitor visitor(test_key_type_id);
    EXPECT_EQ(total_inserted, hash_table_->forEach(&visitor));
    EXPECT_EQ(total_inserted, visitor.internal_map().size());
    for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
      if (HashTableImpl::template_allow_duplicate_keys) {
        ASSERT_EQ(static_cast<std::size_t>(1 + (i % 3)),
                  visitor.internal_map().count(i * (i + 1)));
        std::bitset<3> value_found;
        std::pair<std::unordered_multimap<std::int64_t, int>::const_iterator,
                  std::unordered_multimap<std::int64_t, int>::const_iterator> match_range
            = visitor.internal_map().equal_range(i * (i + 1));
        for (std::unordered_multimap<std::int64_t, int>::const_iterator match_it = match_range.first;
             match_it != match_range.second;
             ++match_it) {
          ASSERT_LE(match_it->second - i * kNumSampleKeys, 3);
          EXPECT_FALSE(value_found[match_it->second - i * kNumSampleKeys]);
          value_found.set(match_it->second - i * kNumSampleKeys, true);
        }

        EXPECT_TRUE(value_found[0]);
        if (i % 3 > 0) {
          EXPECT_TRUE(value_found[1]);
        } else {
          EXPECT_FALSE(value_found[1]);
        }
        if (i % 3 > 1) {
          EXPECT_TRUE(value_found[2]);
        } else {
          EXPECT_FALSE(value_found[2]);
        }
      } else {
        ASSERT_EQ(1u, visitor.internal_map().count(i * (i + 1)));
        EXPECT_EQ(i * kNumSampleKeys,
                  visitor.internal_map().find(i * (i + 1))->second);
      }
    }
  }

  void runScalarKeyPutAndGetFromValueAccessorTest(const TypeID test_key_type_id) {
    std::vector<const Type*> key_types = SetupScalarKeyType(test_key_type_id);
    createHashTableSimple(key_types, kNumSampleKeys);

    std::unique_ptr<ColumnVectorsValueAccessor> accessor(createSampleValueAccessor());

    attribute_id key_attr_id;
    switch (test_key_type_id) {
      case kLong:
        key_attr_id = 0;
        break;
      case kChar:
        key_attr_id = 1;
        break;
      case kVarChar:
        key_attr_id = 2;
        break;
      default:
        FATAL_ERROR("TypeID not covered\n");
    }

    // Vectorized insert, skipping over null keys.
    TestInserter inserter;
    EXPECT_EQ(HashTablePutResult::kOK, hash_table_->putValueAccessor(accessor.get(),
                                                                     key_attr_id,
                                                                     true,
                                                                     &inserter));

    // Read values back out.
    for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
      TypedValue key(createScalarKey(test_key_type_id, i * (i + 1)));
      std::vector<const TestHashPayload*> matches;
      hash_table_->getAll(key, &matches);
      ASSERT_EQ(1u, matches.size());
      EXPECT_EQ(i + (i >= kNumSampleKeys / 2),
                matches.front()->loadInternalInt());
    }

    // Also read the values out using the vectorized interface.
    accessor->beginIteration();
    TestVisitor visitor;
    hash_table_->getAllFromValueAccessor(accessor.get(),
                                         key_attr_id,
                                         true,
                                         &visitor);
    EXPECT_EQ(static_cast<std::size_t>(kNumSampleKeys),
              visitor.internal_map().size());
    for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
      ASSERT_EQ(1u, visitor.internal_map().count(i * (i + 1)));
      EXPECT_EQ(i + (i >= kNumSampleKeys / 2),
                visitor.internal_map().find(i * (i + 1))->second);
    }
  }

  void runScalarKeyClearTest(const TypeID test_key_type_id) {
    std::vector<const Type*> key_types = SetupScalarKeyType(test_key_type_id);
    createHashTableSimple(key_types, kNumSampleKeys);

    // Insert some key/value pairs.
    for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
      TypedValue key(createScalarKey(test_key_type_id, i * (i + 1)));
      TestHashPayload value(i);
      EXPECT_EQ(HashTablePutResult::kOK, hash_table_->put(key, value));
    }

    // Clear out the HashTable.
    hash_table_->clear();

    // Make sure entries are cleared out.
    for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
      TypedValue key(createScalarKey(test_key_type_id, i * (i + 1)));

      std::vector<const TestHashPayload*> matches;
      hash_table_->getAll(key, &matches);
      EXPECT_TRUE(matches.empty());
    }

    // Also make sure that forEach() sees an empty HashTable.
    TestVisitor visitor(test_key_type_id);
    EXPECT_EQ(0u, hash_table_->forEach(&visitor));
    EXPECT_EQ(0u, visitor.internal_map().size());
  }

  void runScalarKeyResizeTest(const TypeID test_key_type_id) {
    // Try to trigger a resize by substantially under-provisioning storage.
    std::vector<const Type*> key_types = SetupScalarKeyType(test_key_type_id);
    createResizableHashTable(key_types, 1);

    const block_id original_blob_id = hash_table_->getBlobId();

    // Insert at least enough values to exhaust the space in the smallest
    // possible StorageBlob.
    for (std::int64_t i = 0;
         i < static_cast<std::int64_t>(
                 kSlotSizeBytes / (sizeof(std::size_t) + sizeof(TestHashPayload)));
         ++i) {
      TypedValue key(createScalarKey(test_key_type_id, i * (i + 1)));
      TestHashPayload value(i);
      EXPECT_EQ(HashTablePutResult::kOK, hash_table_->put(key, value));
    }

    // If a resize occured, the block ID should be different now.
    EXPECT_NE(original_blob_id, hash_table_->getBlobId());

    // Read out the values.
    for (std::int64_t i = 0; i < 2 * kNumSampleKeys; ++i) {
      TypedValue key(createScalarKey(test_key_type_id, i * (i + 1)));

      std::vector<const TestHashPayload*> matches;
      hash_table_->getAll(key, &matches);
      ASSERT_EQ(1u, matches.size());
      EXPECT_EQ(i, matches.front()->loadInternalInt());
    }
  }

  void runScalarKeyExhaustSpaceTest(const TypeID test_key_type_id) {
    std::vector<const Type*> key_types = SetupScalarKeyType(test_key_type_id);

    // Under-provision storage and insert entries until we run out of space.
    const std::size_t buffer_size
        = kNumSampleKeys * (key_types.front()->maximumByteLength() + sizeof(typename HashTableImpl::value_type))
          / 4;
    createNonresizableHashTable(key_types, buffer_size, 0);

    std::int64_t i;
    for (i = 0; i < kNumSampleKeys; ++i) {
      TypedValue key(createScalarKey(test_key_type_id, i * (i + 1)));
      TestHashPayload value(i);
      HashTablePutResult result = hash_table_->put(key, value);
      if (result != HashTablePutResult::kOK) {
        EXPECT_EQ(HashTablePutResult::kOutOfSpace, result);
        break;
      }
    }

    EXPECT_LT(i, kNumSampleKeys);

    // Check the entries that we were able to insert.
    for (std::int64_t j = 0; j < i; ++j) {
      TypedValue key(createScalarKey(test_key_type_id, j * (j + 1)));

      std::vector<const TestHashPayload*> matches;
      hash_table_->getAll(key, &matches);
      ASSERT_EQ(1u, matches.size());
      EXPECT_EQ(j, matches.front()->loadInternalInt());
    }

    // Check that the entry we failed to insert is not present.
    TypedValue failed_key(createScalarKey(test_key_type_id, i * (i + 1)));
    std::vector<const TestHashPayload*> failed_matches;
    hash_table_->getAll(failed_key, &failed_matches);
    EXPECT_TRUE(failed_matches.empty());
  }

  void runScalarKeyUpsertTest(const TypeID test_key_type_id) {
    std::vector<const Type*> key_types = SetupScalarKeyType(test_key_type_id);
    createHashTableSimple(key_types, kNumSampleKeys);

    // Upsert some key/value pairs.
    TestHashPayload initial_value(42);
    TestUpserter upserter;
    for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
      TypedValue key(createScalarKey(test_key_type_id, i * (i + 1)));
      EXPECT_TRUE(hash_table_->upsert(key, initial_value, &upserter));
    }
    EXPECT_EQ(static_cast<std::size_t>(kNumSampleKeys), upserter.call_count());

    // Visit every other value a second time.
    for (std::int64_t i = 0; i < kNumSampleKeys; i += 2) {
      TypedValue key(createScalarKey(test_key_type_id, i * (i + 1)));
      EXPECT_TRUE(hash_table_->upsert(key, initial_value, &upserter));
    }
    EXPECT_EQ(static_cast<std::size_t>(kNumSampleKeys + (kNumSampleKeys / 2)),
              upserter.call_count());

    // Read out the values and check that they are as expected (initial value
    // for each was set to 42, and each call to TestUpserter increments by 1).
    for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
      TypedValue key(createScalarKey(test_key_type_id, i * (i + 1)));

      const TestHashPayload *value = hash_table_->getSingle(key);
      ASSERT_NE(nullptr, value);
      EXPECT_EQ(44 - (i % 2), value->loadInternalInt());
    }
  }

  void runScalarKeyUpsertValueAccessorTest(const TypeID test_key_type_id) {
    std::vector<const Type*> key_types = SetupScalarKeyType(test_key_type_id);
    createHashTableSimple(key_types, kNumSampleKeys);

    std::unique_ptr<ColumnVectorsValueAccessor> accessor(createSampleValueAccessor());

    attribute_id key_attr_id;
    switch (test_key_type_id) {
      case kLong:
        key_attr_id = 0;
        break;
      case kChar:
        key_attr_id = 1;
        break;
      case kVarChar:
        key_attr_id = 2;
        break;
      default:
        FATAL_ERROR("TypeID not covered\n");
    }

    // Vectorized upsert, skipping over null keys.
    TestHashPayload initial_value(42);
    TestUpserter upserter;
    EXPECT_TRUE(hash_table_->upsertValueAccessor(accessor.get(),
                                                 key_attr_id,
                                                 true,
                                                 initial_value,
                                                 &upserter));
    EXPECT_EQ(static_cast<std::size_t>(kNumSampleKeys), upserter.call_count());

    // Read values out and check that they are as expected.
    for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
      TypedValue key(createScalarKey(test_key_type_id, i * (i + 1)));
      std::vector<const TestHashPayload*> matches;
      hash_table_->getAll(key, &matches);
      ASSERT_EQ(1u, matches.size());
      EXPECT_EQ(43, matches.front()->loadInternalInt());
    }
  }

  void runScalarKeySerializationTest(const TypeID test_key_type_id) {
    std::vector<const Type*> key_types = SetupScalarKeyType(test_key_type_id);
    // Set up a shared memory buffer which will hold both the external keys
    // and the hash table.
    const std::size_t buffer_size
        = kNumSampleKeys * key_types.front()->maximumByteLength()
          + 8 * kNumSampleKeys * (key_types.front()->maximumByteLength() + sizeof(typename HashTableImpl::value_type));
    createNonresizableHashTable(key_types,
                                buffer_size,
                                kNumSampleKeys * key_types.front()->maximumByteLength());

    // Create external keys in the first part of the buffer.
    char *key_ptr = static_cast<char*>(memory_buffer_->get());
    std::vector<std::size_t> key_sizes;
    for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
      TypedValue key(createScalarKey(test_key_type_id, i * (i + 1)));
      key.copyInto(key_ptr);
      key_sizes.push_back(key.getDataSize());
      key_ptr += key.getDataSize();
    }

    // Now actually insert.
    key_ptr = static_cast<char*>(memory_buffer_->get());
    for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
      TypedValue key(key_types.front()->makeValue(key_ptr, key_sizes[i]));
      TestHashPayload value(i);
      EXPECT_EQ(HashTablePutResult::kOK, hash_table_->put(key, value));
      key_ptr += key_sizes[i];
    }

    // Destroy the HashTable object and copy the underlying memory.
    hash_table_.reset(nullptr);
    ScopedBuffer moved_buffer(buffer_size, kCacheLineBytes);
    std::memcpy(moved_buffer.get(), memory_buffer_->get(), buffer_size);
    memory_buffer_.reset(nullptr);

    // Recreate the HashTable at the new location.
    hash_table_.reset(new HashTableImpl(
        key_types,
        static_cast<char*>(moved_buffer.get()) + kNumSampleKeys * key_types.front()->maximumByteLength(),
        buffer_size - kNumSampleKeys * key_types.front()->maximumByteLength(),
        false,
        false));

    // If duplicate keys are not allowed, make sure we can't insert a
    // duplicate.
    if (!HashTableImpl::template_allow_duplicate_keys) {
      TypedValue key(key_types.front()->makeValue(moved_buffer.get(), key_sizes[0]));
      TestHashPayload value(123);
      EXPECT_EQ(HashTablePutResult::kDuplicateKey, hash_table_->put(key, value));
    }

    // Read the inserted values back out.
    for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
      TypedValue key(createScalarKey(test_key_type_id, i * (i + 1)));

      // Test getAll() in all cases.
      std::vector<const TestHashPayload*> matches;
      hash_table_->getAll(key, &matches);
      ASSERT_EQ(1u, matches.size());
      EXPECT_EQ(i, matches.front()->loadInternalInt());
    }

    // Check a nonexistent key.
    TypedValue nonexistent_key(createScalarKey(test_key_type_id,
                                               kNumSampleKeys * kNumSampleKeys * kNumSampleKeys));
    std::vector<const TestHashPayload*> nonexistent_matches;
    hash_table_->getAll(nonexistent_key, &nonexistent_matches);
    EXPECT_TRUE(nonexistent_matches.empty());
  }

  void runScalarKeyThreadedPutTest(const TypeID test_key_type_id,
                                   const bool test_resize) {
    std::vector<const Type*> key_types = SetupScalarKeyType(test_key_type_id);
    if (HashTableImpl::template_allow_duplicate_keys) {
      if (test_resize) {
        createHashTableSimple(key_types, 1);
      } else {
        createHashTableSimple(key_types, kNumSampleKeys * kNumConcurrentThreads);
      }
    } else {
      if (test_resize) {
        createHashTableSimple(key_types, 1);
      } else {
        createHashTableSimple(key_types, kNumSampleKeys);
      }
    }

    // If testing multithreaded resize, record the original ID of the
    // underlying blob.
    block_id original_blob_id = 0;
    if (test_resize) {
      original_blob_id = hash_table_->getBlobId();
    }

    // Setup keys ahead of time.
    const std::size_t num_keys = test_resize
        ? (kSlotSizeBytes / (sizeof(std::size_t) + sizeof(TestHashPayload)))
            / (HashTableImpl::template_allow_duplicate_keys ? kNumConcurrentThreads : 1)
        : kNumSampleKeys;
    std::vector<TypedValue> keys;
    for (std::int64_t i = 0; i < static_cast<std::int64_t>(num_keys); ++i) {
      keys.emplace_back(createScalarKey(test_key_type_id, i * (i + 1)));
    }

    // Setup threads.
    std::vector<std::unique_ptr<InserterThread>> threads;
    for (std::size_t thread_num = 0; thread_num < kNumConcurrentThreads; ++thread_num) {
      threads.emplace_back(new InserterThread(thread_num, keys, hash_table_.get()));
    }

    // Run threads and join them.
    for (std::unique_ptr<InserterThread> &thread : threads) {
      thread->start();
    }
    for (std::unique_ptr<InserterThread> &thread : threads) {
      thread->join();
    }

    // If a resize occured, the block ID should be different now.
    if (test_resize) {
      EXPECT_NE(original_blob_id, hash_table_->getBlobId());
    }

    // Check that everything was inserted properly.
    for (std::int64_t i = 0; i < static_cast<std::int64_t>(num_keys); ++i) {
      std::size_t successful_inserts = 0;
      for (const std::unique_ptr<InserterThread> &thread : threads) {
        if (thread->insert_succeeded()[i]) {
          ++successful_inserts;
        }
      }
      if (HashTableImpl::template_allow_duplicate_keys) {
        EXPECT_EQ(kNumConcurrentThreads, successful_inserts);
      } else {
        EXPECT_EQ(1u, successful_inserts);
      }

      TypedValue key(createScalarKey(test_key_type_id, i * (i + 1)));

      std::vector<const TestHashPayload*> matches;
      hash_table_->getAll(key, &matches);
      if (HashTableImpl::template_allow_duplicate_keys) {
        EXPECT_EQ(kNumConcurrentThreads, matches.size());
        std::vector<bool> found_value(kNumConcurrentThreads, false);
        for (const TestHashPayload *payload : matches) {
          const int payload_value = payload->loadInternalInt();
          ASSERT_GE(payload_value, i * kNumSampleKeys);
          ASSERT_LT(payload_value,
                    i * kNumSampleKeys + static_cast<std::int64_t>(kNumConcurrentThreads));
          EXPECT_FALSE(found_value[payload_value - i * kNumSampleKeys]);
          found_value[payload_value - i * kNumSampleKeys] = true;
        }
        for (const bool found : found_value) {
          EXPECT_TRUE(found);
        }
      } else {
        ASSERT_EQ(1u, matches.size());
        for (const std::unique_ptr<InserterThread> &thread : threads) {
          if (thread->insert_succeeded()[i]) {
            EXPECT_EQ(i * kNumSampleKeys + thread->getID(),
                      matches.front()->loadInternalInt());
          }
        }
      }
    }
  }

  void runCompositeKeyThreadedPutTest(const bool test_resize) {
    std::vector<const Type*> key_types = SetupCompositeKeyType();
    if (HashTableImpl::template_allow_duplicate_keys) {
      if (test_resize) {
        createHashTableSimple(key_types, 1);
      } else {
        createHashTableSimple(key_types, kNumSampleKeys * kNumConcurrentThreads);
      }
    } else {
      if (test_resize) {
        createHashTableSimple(key_types, 1);
      } else {
        createHashTableSimple(key_types, kNumSampleKeys);
      }
    }

    // If testing multithreaded resize, record the original ID of the
    // underlying blob.
    block_id original_blob_id = 0;
    if (test_resize) {
      original_blob_id = hash_table_->getBlobId();
    }

    // Setup keys ahead of time.
    const std::size_t num_keys = test_resize
        ? (kSlotSizeBytes / (sizeof(std::size_t) + sizeof(TestHashPayload)))
            / (HashTableImpl::template_allow_duplicate_keys ? kNumConcurrentThreads : 1)
        : kNumSampleKeys;
    std::vector<std::vector<TypedValue>> keys;
    for (std::int64_t i = 0; i < static_cast<std::int64_t>(num_keys); ++i) {
      keys.emplace_back(createCompositeKey(i * (i + 1)));
    }

    // Setup threads.
    std::vector<std::unique_ptr<CompositeKeyInserterThread>> threads;
    for (std::size_t thread_num = 0; thread_num < kNumConcurrentThreads; ++thread_num) {
      threads.emplace_back(new CompositeKeyInserterThread(thread_num, keys, hash_table_.get()));
    }

    // Run threads and join them.
    for (std::unique_ptr<CompositeKeyInserterThread> &thread : threads) {
      thread->start();
    }
    for (std::unique_ptr<CompositeKeyInserterThread> &thread : threads) {
      thread->join();
    }

    // If a resize occured, the block ID should be different now.
    if (test_resize) {
      EXPECT_NE(original_blob_id, hash_table_->getBlobId());
    }

    // Check that everything was inserted properly.
    for (std::int64_t i = 0; i < static_cast<std::int64_t>(num_keys); ++i) {
      std::size_t successful_inserts = 0;
      for (const std::unique_ptr<CompositeKeyInserterThread> &thread : threads) {
        if (thread->insert_succeeded()[i]) {
          ++successful_inserts;
        }
      }
      if (HashTableImpl::template_allow_duplicate_keys) {
        EXPECT_EQ(kNumConcurrentThreads, successful_inserts);
      } else {
        EXPECT_EQ(1u, successful_inserts);
      }

      std::vector<TypedValue> key(createCompositeKey(i * (i + 1)));

      std::vector<const TestHashPayload*> matches;
      hash_table_->getAllCompositeKey(key, &matches);
      if (HashTableImpl::template_allow_duplicate_keys) {
        EXPECT_EQ(kNumConcurrentThreads, matches.size());
        std::vector<bool> found_value(kNumConcurrentThreads, false);
        for (const TestHashPayload *payload : matches) {
          const int payload_value = payload->loadInternalInt();
          ASSERT_GE(payload_value, i * kNumSampleKeys);
          ASSERT_LT(payload_value,
                    i * kNumSampleKeys + static_cast<std::int64_t>(kNumConcurrentThreads));
          EXPECT_FALSE(found_value[payload_value - i * kNumSampleKeys]);
          found_value[payload_value - i * kNumSampleKeys] = true;
        }
        for (const bool found : found_value) {
          EXPECT_TRUE(found);
        }
      } else {
        ASSERT_EQ(1u, matches.size());
        for (const std::unique_ptr<CompositeKeyInserterThread> &thread : threads) {
          if (thread->insert_succeeded()[i]) {
            EXPECT_EQ(i * kNumSampleKeys + thread->getID(),
                      matches.front()->loadInternalInt());
          }
        }
      }
    }
  }

  std::unique_ptr<StorageManager> storage_manager_;
  std::unique_ptr<ScopedBuffer> memory_buffer_;
  std::unique_ptr<HashTableImpl> hash_table_;
  std::vector<ScopedBuffer> external_strings_;
};
TYPED_TEST_CASE_P(HashTableTest);

template <typename HashTableImpl> using HashTableTestLongOnly
    = HashTableTest<HashTableImpl>;
TYPED_TEST_CASE_P(HashTableTestLongOnly);

// Alias for tests which only apply to resizable hash tables.
template <typename HashTableImpl> using ResizableHashTableTest
    = HashTableTest<HashTableImpl>;
TYPED_TEST_CASE_P(ResizableHashTableTest);

template <typename HashTableImpl> using ResizableHashTableTestLongOnly
    = HashTableTest<HashTableImpl>;
TYPED_TEST_CASE_P(ResizableHashTableTestLongOnly);

// Alias for tests which only apply to non-resizable hash tables.
template <typename HashTableImpl> using NonResizableHashTableTest
    = HashTableTest<HashTableImpl>;
TYPED_TEST_CASE_P(NonResizableHashTableTest);

template <typename HashTableImpl> using NonResizableHashTableTestLongOnly
    = HashTableTest<HashTableImpl>;
TYPED_TEST_CASE_P(NonResizableHashTableTestLongOnly);

// Alias for tests which only apply to hash tables which do not allow duplicate
// keys.
template <typename HashTableImpl> using DuplicateKeysForbiddenHashTableTest
    = HashTableTest<HashTableImpl>;
TYPED_TEST_CASE_P(DuplicateKeysForbiddenHashTableTest);

template <typename HashTableImpl> using DuplicateKeysForbiddenHashTableTestLongOnly
    = HashTableTest<HashTableImpl>;
TYPED_TEST_CASE_P(DuplicateKeysForbiddenHashTableTestLongOnly);

// Alias for tests which only apply to hash tables which are fixed-size and
// serializable.
template <typename HashTableImpl> using FixedSizeSerializableHashTableTest
    = HashTableTest<HashTableImpl>;
TYPED_TEST_CASE_P(FixedSizeSerializableHashTableTest);

template <typename HashTableImpl> using FixedSizeSerializableHashTableTestLongOnly
    = HashTableTest<HashTableImpl>;
TYPED_TEST_CASE_P(FixedSizeSerializableHashTableTestLongOnly);

// We use a separate test fixture here, since the regular HashTableTest assumes
// that HashTableImpl::value_type is TestHashPayload.
template <typename HashTableImpl>
class NonTriviallyDestructibleValueHashTableTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    storage_manager_.reset(new StorageManager("./test_data/"));
  }

  static std::vector<const Type*> SetupScalarKeyType(const TypeID type_id) {
    std::vector<const Type*> key_type_vec;
    switch (type_id) {
      case kLong:
        key_type_vec.push_back(&TypeFactory::GetType(kLong, false));
        break;
      case kChar:
        key_type_vec.push_back(&TypeFactory::GetType(kChar, kStringLengthParam, false));
        break;
      case kVarChar:
        key_type_vec.push_back(&TypeFactory::GetType(kVarChar, kStringLengthParam, false));
        break;
      default:
        FATAL_ERROR("TypeID not covered\n");
    }
    return key_type_vec;
  }

  static std::vector<const Type*> SetupCompositeKeyType() {
    std::vector<const Type*> key_type_vec;
    key_type_vec.push_back(&TypeFactory::GetType(kLong, false));
    key_type_vec.push_back(&TypeFactory::GetType(kChar, kStringLengthParam, false));
    key_type_vec.push_back(&TypeFactory::GetType(kVarChar, kStringLengthParam, false));
    return key_type_vec;
  }

  void createResizableHashTable(const std::vector<const Type*> &key_types,
                                const std::size_t num_entries) {
    hash_table_.reset(new HashTableImpl(key_types,
                                        num_entries,
                                        storage_manager_.get()));
  }

  void createNonresizableHashTable(const std::vector<const Type*> &key_types,
                                   const std::size_t buffer_size,
                                   const std::size_t offset_in_buffer) {
    assert(offset_in_buffer < buffer_size);
    memory_buffer_.reset(new ScopedBuffer(buffer_size, kCacheLineBytes));
    hash_table_.reset(new HashTableImpl(
        key_types,
        static_cast<char*>(memory_buffer_->get()) + offset_in_buffer,
        buffer_size - offset_in_buffer,
        true,
        false));
  }

  void createHashTableSimple(const std::vector<const Type*> &key_types,
                             const std::size_t num_entries) {
    if (HashTableImpl::template_resizable) {
      createResizableHashTable(key_types, num_entries);
    } else {
      // Allocate space for 'num_entries' generously.
      std::size_t key_size = 0;
      for (const Type *type : key_types) {
        key_size += type->maximumByteLength();
      }
      createNonresizableHashTable(
          key_types,
          num_entries * 8 * (key_size + sizeof(typename HashTableImpl::value_type)),
          0);
    }
  }

  TypedValue createLongKey(const std::int64_t seed) const {
    return TypedValue(seed);
  }

  TypedValue createCharKey(const std::int64_t seed) {
    std::ostringstream stream;
    stream << seed;
    external_strings_.emplace_back(stream.str().size() + 1);
    std::memcpy(external_strings_.back().get(),
                stream.str().c_str(),
                stream.str().size() + 1);
    return TypeFactory::GetType(kChar, kStringLengthParam, false).makeValue(external_strings_.back().get(),
                                                                     stream.str().size() + 1);
  }

  TypedValue createVarCharKey(const std::int64_t seed) {
    std::ostringstream stream;
    stream << seed;
    external_strings_.emplace_back(stream.str().size() + 1);
    std::memcpy(external_strings_.back().get(),
                stream.str().c_str(),
                stream.str().size() + 1);
    return TypeFactory::GetType(kVarChar, kStringLengthParam, false).makeValue(external_strings_.back().get(),
                                                                        stream.str().size() + 1);
  }

  TypedValue createScalarKey(const TypeID test_key_type_id,
                             const std::int64_t seed) {
    switch (test_key_type_id) {
      case TypeID::kLong:
        return createLongKey(seed);
      case TypeID::kChar:
        return createCharKey(seed);
      case TypeID::kVarChar:
        return createVarCharKey(seed);
      default:
        FATAL_ERROR("TypeID not covered\n");
    }
  }

  std::vector<TypedValue> createCompositeKey(const std::int64_t seed) {
    std::vector<TypedValue> key;
    key.emplace_back(createLongKey(seed));
    key.emplace_back(createCharKey(seed));
    key.emplace_back(createVarCharKey(seed));
    return key;
  }

  void runScalarKeyDestroyValueTest(const TypeID test_key_type_id) {
    std::vector<const Type*> key_types = SetupScalarKeyType(test_key_type_id);
    createHashTableSimple(key_types, kNumSampleKeys);

    std::atomic<int> destruct_count(0);

    // Insert some key/value pairs.
    for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
      TypedValue key(createScalarKey(test_key_type_id, i * (i + 1)));
      NonTriviallyDestructibleTestHashPayload value(&destruct_count);
      EXPECT_EQ(HashTablePutResult::kOK, hash_table_->put(key, value));
      value.clearDestructCountPtr();
    }

    // Destroy the hash table and check that the destructors of inserted values
    // were invoked.
    hash_table_.reset(nullptr);
    EXPECT_EQ(kNumSampleKeys,
              destruct_count.load(std::memory_order_relaxed));
  }

  std::unique_ptr<StorageManager> storage_manager_;
  std::unique_ptr<ScopedBuffer> memory_buffer_;
  std::unique_ptr<HashTableImpl> hash_table_;
  std::vector<ScopedBuffer> external_strings_;
};
TYPED_TEST_CASE_P(NonTriviallyDestructibleValueHashTableTest);

template <typename HashTableImpl> using NonTriviallyDestructibleValueHashTableTestLongOnly
    = NonTriviallyDestructibleValueHashTableTest<HashTableImpl>;
TYPED_TEST_CASE_P(NonTriviallyDestructibleValueHashTableTestLongOnly);

TYPED_TEST_P(HashTableTestLongOnly, LongKeyPutAndGetTest) {
  this->runScalarKeyPutAndGetTest(TypeID::kLong);
}

TYPED_TEST_P(HashTableTest, CharKeyPutAndGetTest) {
  this->runScalarKeyPutAndGetTest(TypeID::kChar);
}

TYPED_TEST_P(HashTableTest, VarCharKeyPutAndGetTest) {
  this->runScalarKeyPutAndGetTest(TypeID::kVarChar);
}

TYPED_TEST_P(HashTableTest, CompositeKeyPutAndGetTest) {
  std::vector<const Type*> key_types = this->SetupCompositeKeyType();
  this->createHashTableSimple(key_types, kNumSampleKeys);

  // Insert some key/value pairs.
  for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
    std::vector<TypedValue> key(this->createCompositeKey(i * (i + 1)));
    TestHashPayload value(i);
    EXPECT_EQ(HashTablePutResult::kOK, this->hash_table_->putCompositeKey(key, value));
  }

  // Read them back out.
  for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
    std::vector<TypedValue> key(this->createCompositeKey(i * (i + 1)));
    // Use getSingleCompositeKey() if duplicate keys are not allowed.
    if (!TypeParam::template_allow_duplicate_keys) {
      const TestHashPayload *value = this->hash_table_->getSingleCompositeKey(key);
      ASSERT_NE(nullptr, value);
      EXPECT_EQ(i, value->loadInternalInt());
    }

    // Test getAllCompositeKey() in all cases.
    std::vector<const TestHashPayload*> matches;
    this->hash_table_->getAllCompositeKey(key, &matches);
    ASSERT_EQ(1u, matches.size());
    EXPECT_EQ(i, matches.front()->loadInternalInt());
  }

  // Check a nonexistent key.
  std::vector<TypedValue> nonexistent_key(
      this->createCompositeKey(kNumSampleKeys * kNumSampleKeys * kNumSampleKeys));
  if (!TypeParam::template_allow_duplicate_keys) {
    const TestHashPayload *nonexistent_value
        = this->hash_table_->getSingleCompositeKey(nonexistent_key);
    EXPECT_EQ(nullptr, nonexistent_value);
  }
  std::vector<const TestHashPayload*> nonexistent_matches;
  this->hash_table_->getAllCompositeKey(nonexistent_key, &nonexistent_matches);
  EXPECT_TRUE(nonexistent_matches.empty());

  // Check a nonexistent key that only differs from an existing key in one
  // component.
  nonexistent_key.clear();
  nonexistent_key.emplace_back(this->createLongKey(0));
  nonexistent_key.emplace_back(this->createCharKey(0));
  nonexistent_key.emplace_back(this->createVarCharKey(1));
  if (!TypeParam::template_allow_duplicate_keys) {
    const TestHashPayload *nonexistent_value
        = this->hash_table_->getSingleCompositeKey(nonexistent_key);
    EXPECT_EQ(nullptr, nonexistent_value);
  }
  nonexistent_matches.clear();
  this->hash_table_->getAllCompositeKey(nonexistent_key, &nonexistent_matches);
  EXPECT_TRUE(nonexistent_matches.empty());

  // Also test forEach().
  TestVisitor visitor;
  EXPECT_EQ(static_cast<std::size_t>(kNumSampleKeys),
            this->hash_table_->forEachCompositeKey(&visitor));
  EXPECT_EQ(static_cast<std::size_t>(kNumSampleKeys),
            visitor.internal_map().size());
  for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
    ASSERT_EQ(1u, visitor.internal_map().count(i * (i + 1)));
    EXPECT_EQ(i, visitor.internal_map().find(i * (i + 1))->second);
  }
}

TYPED_TEST_P(HashTableTest, SpecialHashesPutAndGetTest) {
  std::vector<const Type*> key_types = this->SetupScalarKeyType(kLong);
  this->createHashTableSimple(key_types, 4);

  // Insert Long values that hash to "special" reserved values in HashTable.
  TypedValue empty_hash_key(
      this->createScalarKey(kLong, static_cast<std::int64_t>(TypeParam::kEmptyHash)));
  TestHashPayload empty_hash_value(42);
  EXPECT_EQ(HashTablePutResult::kOK, this->hash_table_->put(empty_hash_key, empty_hash_value));

  TypedValue pending_hash_key(
      this->createScalarKey(kLong, static_cast<std::int64_t>(TypeParam::kPendingHash)));
  TestHashPayload pending_hash_value(85);
  EXPECT_EQ(HashTablePutResult::kOK, this->hash_table_->put(pending_hash_key, pending_hash_value));

  // Also try values whose natural hashes are the same as the adjusted hashes
  // of the values above (causing spurious hash collisions).
  TypedValue adjusted_empty_hash_key(
      this->createScalarKey(kLong, static_cast<std::int64_t>(TypeParam::kEmptyHash + 1)));
  TestHashPayload adjusted_empty_hash_value(123);
  EXPECT_EQ(HashTablePutResult::kOK, this->hash_table_->put(adjusted_empty_hash_key,
                                                            adjusted_empty_hash_value));

  TypedValue adjusted_pending_hash_key(
      this->createScalarKey(kLong, static_cast<std::int64_t>(TypeParam::kPendingHash - 1)));
  TestHashPayload adjusted_pending_hash_value(456);
  EXPECT_EQ(HashTablePutResult::kOK, this->hash_table_->put(adjusted_pending_hash_key,
                                                            adjusted_pending_hash_value));

  // Read values back out.
  std::vector<const TestHashPayload*> values;
  this->hash_table_->getAll(empty_hash_key, &values);
  ASSERT_EQ(1u, values.size());
  EXPECT_EQ(42, values.front()->loadInternalInt());
  values.clear();

  this->hash_table_->getAll(pending_hash_key, &values);
  ASSERT_EQ(1u, values.size());
  EXPECT_EQ(85, values.front()->loadInternalInt());
  values.clear();

  this->hash_table_->getAll(adjusted_empty_hash_key, &values);
  ASSERT_EQ(1u, values.size());
  EXPECT_EQ(123, values.front()->loadInternalInt());
  values.clear();

  this->hash_table_->getAll(adjusted_pending_hash_key, &values);
  ASSERT_EQ(1u, values.size());
  EXPECT_EQ(456, values.front()->loadInternalInt());
  values.clear();
}

TYPED_TEST_P(HashTableTestLongOnly, LongKeyPutDuplicateKeysTest) {
  this->runDuplicateScalarKeysPutAndGetTest(TypeID::kLong);
}

TYPED_TEST_P(HashTableTest, CharKeyPutDuplicateKeysTest) {
  this->runDuplicateScalarKeysPutAndGetTest(TypeID::kChar);
}

TYPED_TEST_P(HashTableTest, VarCharKeyPutDuplicateKeysTest) {
  this->runDuplicateScalarKeysPutAndGetTest(TypeID::kVarChar);
}

TYPED_TEST_P(HashTableTest, CompositeKeyPutDuplicateKeysTest) {
  std::vector<const Type*> key_types = this->SetupCompositeKeyType();
  this->createHashTableSimple(key_types, kNumSampleKeys * 2);

  // Attempt to insert 1-3 values for each distinct key.
  std::size_t total_inserted = 0;
  for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
    std::vector<TypedValue> key(this->createCompositeKey(i * (i + 1)));
    TestHashPayload first_value(i * kNumSampleKeys);
    EXPECT_EQ(HashTablePutResult::kOK,
              this->hash_table_->putCompositeKey(key, first_value));
    ++total_inserted;

    for (int extra_value_num = 1;
         extra_value_num <= i % 3;
         ++extra_value_num) {
      TestHashPayload additional_value(i * kNumSampleKeys + extra_value_num);
      if (TypeParam::template_allow_duplicate_keys) {
        EXPECT_EQ(HashTablePutResult::kOK,
                  this->hash_table_->putCompositeKey(key, additional_value));
        ++total_inserted;
      } else {
        EXPECT_EQ(HashTablePutResult::kDuplicateKey,
                  this->hash_table_->putCompositeKey(key, additional_value));
      }
    }
  }

  // Read them back out.
  for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
    std::vector<TypedValue> key(this->createCompositeKey(i * (i + 1)));
    std::vector<const TestHashPayload*> matches;
    this->hash_table_->getAllCompositeKey(key, &matches);
    if (TypeParam::template_allow_duplicate_keys) {
      EXPECT_EQ(static_cast<std::size_t>((i % 3) + 1), matches.size());
      std::bitset<3> value_found;
      for (const TestHashPayload *match : matches) {
        ASSERT_LE(match->loadInternalInt() - i * kNumSampleKeys, 3);
        EXPECT_FALSE(value_found[match->loadInternalInt() - i * kNumSampleKeys]);
        value_found.set(match->loadInternalInt() - i * kNumSampleKeys, true);
      }
      EXPECT_TRUE(value_found[0]);
      if (i % 3 > 0) {
        EXPECT_TRUE(value_found[1]);
      } else {
        EXPECT_FALSE(value_found[1]);
      }
      if (i % 3 > 1) {
        EXPECT_TRUE(value_found[2]);
      } else {
        EXPECT_FALSE(value_found[2]);
      }
    } else {
      ASSERT_EQ(1u, matches.size());
      EXPECT_EQ(i * kNumSampleKeys,
                matches.front()->loadInternalInt());
    }
  }

  // Check a nonexistent key.
  std::vector<TypedValue> nonexistent_key(
      this->createCompositeKey(kNumSampleKeys * kNumSampleKeys * kNumSampleKeys));
  std::vector<const TestHashPayload*> nonexistent_matches;
  this->hash_table_->getAllCompositeKey(nonexistent_key, &nonexistent_matches);
  EXPECT_TRUE(nonexistent_matches.empty());

  // Check a nonexistent key that only differs from an existing key in one
  // component.
  nonexistent_key.clear();
  nonexistent_key.emplace_back(this->createLongKey(0));
  nonexistent_key.emplace_back(this->createCharKey(0));
  nonexistent_key.emplace_back(this->createVarCharKey(1));
  if (!TypeParam::template_allow_duplicate_keys) {
    const TestHashPayload *nonexistent_value
        = this->hash_table_->getSingleCompositeKey(nonexistent_key);
    EXPECT_EQ(nullptr, nonexistent_value);
  }
  nonexistent_matches.clear();
  this->hash_table_->getAllCompositeKey(nonexistent_key, &nonexistent_matches);
  EXPECT_TRUE(nonexistent_matches.empty());

  // Also test forEach().
  TestVisitor visitor;
  EXPECT_EQ(total_inserted, this->hash_table_->forEachCompositeKey(&visitor));
  EXPECT_EQ(total_inserted, visitor.internal_map().size());
  for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
    if (TypeParam::template_allow_duplicate_keys) {
      ASSERT_EQ(static_cast<std::size_t>(1 + (i % 3)),
                visitor.internal_map().count(i * (i + 1)));
      std::bitset<3> value_found;
      std::pair<std::unordered_multimap<std::int64_t, int>::const_iterator,
                std::unordered_multimap<std::int64_t, int>::const_iterator> match_range
          = visitor.internal_map().equal_range(i * (i + 1));
      for (std::unordered_multimap<std::int64_t, int>::const_iterator match_it = match_range.first;
           match_it != match_range.second;
           ++match_it) {
        ASSERT_LE(match_it->second - i * kNumSampleKeys, 3);
        EXPECT_FALSE(value_found[match_it->second - i * kNumSampleKeys]);
        value_found.set(match_it->second - i * kNumSampleKeys, true);
      }

      EXPECT_TRUE(value_found[0]);
      if (i % 3 > 0) {
        EXPECT_TRUE(value_found[1]);
      } else {
        EXPECT_FALSE(value_found[1]);
      }
      if (i % 3 > 1) {
        EXPECT_TRUE(value_found[2]);
      } else {
        EXPECT_FALSE(value_found[2]);
      }
    } else {
      ASSERT_EQ(1u, visitor.internal_map().count(i * (i + 1)));
      EXPECT_EQ(i * kNumSampleKeys,
                visitor.internal_map().find(i * (i + 1))->second);
    }
  }
}

TYPED_TEST_P(HashTableTestLongOnly, LongKeyPutAndGetFromValueAccessorTest) {
  this->runScalarKeyPutAndGetFromValueAccessorTest(TypeID::kLong);
}

TYPED_TEST_P(HashTableTest, CharKeyPutAndGetFromValueAccessorTest) {
  this->runScalarKeyPutAndGetFromValueAccessorTest(TypeID::kChar);
}

TYPED_TEST_P(HashTableTest, VarCharKeyPutAndGetFromValueAccessorTest) {
  this->runScalarKeyPutAndGetFromValueAccessorTest(TypeID::kVarChar);
}

TYPED_TEST_P(HashTableTest, CompositeKeyPutAndGetFromValueAccessorTest) {
  std::vector<const Type*> key_types = this->SetupCompositeKeyType();
  this->createHashTableSimple(key_types, kNumSampleKeys);

  std::unique_ptr<ColumnVectorsValueAccessor> accessor(this->createSampleValueAccessor());

  std::vector<attribute_id> key_attr_ids;
  key_attr_ids.push_back(0);
  key_attr_ids.push_back(1);
  key_attr_ids.push_back(2);

  // Vectorized insert, skipping over null keys.
  TestInserter inserter;
  EXPECT_EQ(HashTablePutResult::kOK,
            this->hash_table_->putValueAccessorCompositeKey(accessor.get(),
                                                            key_attr_ids,
                                                            true,
                                                            &inserter));

  // Read values back out.
  for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
    std::vector<TypedValue> key(this->createCompositeKey(i * (i + 1)));
    std::vector<const TestHashPayload*> matches;
    this->hash_table_->getAllCompositeKey(key, &matches);
    ASSERT_EQ(1u, matches.size());
    EXPECT_EQ(i + (i >= kNumSampleKeys / 2),
              matches.front()->loadInternalInt());
  }

  // Also read the values out using the vectorized interface.
  accessor->beginIteration();
  TestVisitor visitor;
  this->hash_table_->getAllFromValueAccessorCompositeKey(accessor.get(),
                                                         key_attr_ids,
                                                         true,
                                                         &visitor);
  EXPECT_EQ(static_cast<std::size_t>(kNumSampleKeys), visitor.internal_map().size());
  for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
    ASSERT_EQ(1u, visitor.internal_map().count(i * (i + 1)));
    EXPECT_EQ(i + (i >= kNumSampleKeys / 2),
              visitor.internal_map().find(i * (i + 1))->second);
  }
}

TYPED_TEST_P(HashTableTestLongOnly, LongKeyThreadedPutTest) {
  this->runScalarKeyThreadedPutTest(TypeID::kLong, false);
}

TYPED_TEST_P(HashTableTest, CharKeyThreadedPutTest) {
  this->runScalarKeyThreadedPutTest(TypeID::kChar, false);
}

TYPED_TEST_P(HashTableTest, VarCharKeyThreadedPutTest) {
  this->runScalarKeyThreadedPutTest(TypeID::kVarChar, false);
}

TYPED_TEST_P(HashTableTest, CompositeKeyThreadedPutTest) {
  this->runCompositeKeyThreadedPutTest(false);
}

TYPED_TEST_P(HashTableTestLongOnly, LongKeyClearTest) {
  this->runScalarKeyClearTest(TypeID::kLong);
}

TYPED_TEST_P(HashTableTest, CharKeyClearTest) {
  this->runScalarKeyClearTest(TypeID::kChar);
}

TYPED_TEST_P(HashTableTest, VarCharKeyClearTest) {
  this->runScalarKeyClearTest(TypeID::kVarChar);
}

TYPED_TEST_P(HashTableTest, CompositeKeyClearTest) {
  std::vector<const Type*> key_types = this->SetupCompositeKeyType();
  this->createHashTableSimple(key_types, kNumSampleKeys);

  // Insert some key/value pairs.
  for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
    std::vector<TypedValue> key(this->createCompositeKey(i * (i + 1)));
    TestHashPayload value(i);
    EXPECT_EQ(HashTablePutResult::kOK, this->hash_table_->putCompositeKey(key, value));
  }

  // Clear out the HashTable.
  this->hash_table_->clear();

  // Make sure entries are cleared out.
  for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
    std::vector<TypedValue> key(this->createCompositeKey(i * (i + 1)));

    std::vector<const TestHashPayload*> matches;
    this->hash_table_->getAllCompositeKey(key, &matches);
    EXPECT_TRUE(matches.empty());
  }

  // Also make sure that forEach() sees an empty HashTable.
  TestVisitor visitor;
  EXPECT_EQ(0u, this->hash_table_->forEachCompositeKey(&visitor));
  EXPECT_EQ(0u, visitor.internal_map().size());
}

TYPED_TEST_P(ResizableHashTableTestLongOnly, LongKeyResizeTest) {
  this->runScalarKeyResizeTest(TypeID::kLong);
}

TYPED_TEST_P(ResizableHashTableTest, CharKeyResizeTest) {
  this->runScalarKeyResizeTest(TypeID::kChar);
}

TYPED_TEST_P(ResizableHashTableTest, VarCharKeyResizeTest) {
  this->runScalarKeyResizeTest(TypeID::kVarChar);
}

TYPED_TEST_P(ResizableHashTableTest, CompositeKeyResizeTest) {
  // Try to trigger a resize by substantially under-provisioning storage.
  std::vector<const Type*> key_types = this->SetupCompositeKeyType();
  this->createResizableHashTable(key_types, 1);

  const block_id original_blob_id = this->hash_table_->getBlobId();

  for (std::int64_t i = 0;
       i < static_cast<std::int64_t>(
               kSlotSizeBytes / (sizeof(std::size_t) + sizeof(TestHashPayload)));
       ++i) {
    std::vector<TypedValue> key(this->createCompositeKey(i * (i + 1)));
    TestHashPayload value(i);
    EXPECT_EQ(HashTablePutResult::kOK, this->hash_table_->putCompositeKey(key, value));
  }

  // If a resize occured, the block ID should be different now.
  EXPECT_NE(original_blob_id, this->hash_table_->getBlobId());

  // Read out the values.
  for (std::int64_t i = 0; i < 2 * kNumSampleKeys; ++i) {
    std::vector<TypedValue> key(this->createCompositeKey(i * (i + 1)));

    std::vector<const TestHashPayload*> matches;
    this->hash_table_->getAllCompositeKey(key, &matches);
    ASSERT_EQ(1u, matches.size());
    EXPECT_EQ(i, matches.front()->loadInternalInt());
  }
}

TYPED_TEST_P(ResizableHashTableTestLongOnly, LongKeyThreadedResizeTest) {
  this->runScalarKeyThreadedPutTest(TypeID::kLong, true);
}

TYPED_TEST_P(ResizableHashTableTest, CharKeyThreadedResizeTest) {
  this->runScalarKeyThreadedPutTest(TypeID::kChar, true);
}

TYPED_TEST_P(ResizableHashTableTest, VarCharKeyThreadedResizeTest) {
  this->runScalarKeyThreadedPutTest(TypeID::kVarChar, true);
}

TYPED_TEST_P(ResizableHashTableTest, CompositeKeyThreadedResizeTest) {
  this->runCompositeKeyThreadedPutTest(true);
}

TYPED_TEST_P(NonResizableHashTableTestLongOnly, LongKeyExhaustSpaceTest) {
  this->runScalarKeyExhaustSpaceTest(TypeID::kLong);
}

TYPED_TEST_P(NonResizableHashTableTest, CharKeyExhaustSpaceTest) {
  this->runScalarKeyExhaustSpaceTest(TypeID::kChar);
}

TYPED_TEST_P(NonResizableHashTableTest, VarCharKeyExhaustSpaceTest) {
  this->runScalarKeyExhaustSpaceTest(TypeID::kVarChar);
}

TYPED_TEST_P(NonResizableHashTableTest, CompositeKeyExhaustSpaceTest) {
  std::vector<const Type*> key_types = this->SetupCompositeKeyType();

  // Under-provision storage and insert entries until we run out of space.
  const std::size_t buffer_size
      = kNumSampleKeys * (key_types.front()->maximumByteLength() + sizeof(typename TypeParam::value_type))
        / 4;
  this->createNonresizableHashTable(key_types, buffer_size, 0);

  std::int64_t i;
  for (i = 0; i < kNumSampleKeys; ++i) {
    std::vector<TypedValue> key(this->createCompositeKey(i * (i + 1)));
    TestHashPayload value(i);
    HashTablePutResult result = this->hash_table_->putCompositeKey(key, value);
    if (result != HashTablePutResult::kOK) {
      EXPECT_EQ(HashTablePutResult::kOutOfSpace, result);
      break;
    }
  }

  EXPECT_LT(i, kNumSampleKeys);

  // Check the entries that we were able to insert.
  for (std::int64_t j = 0; j < i; ++j) {
    std::vector<TypedValue> key(this->createCompositeKey(j * (j + 1)));

    std::vector<const TestHashPayload*> matches;
    this->hash_table_->getAllCompositeKey(key, &matches);
    ASSERT_EQ(1u, matches.size());
    EXPECT_EQ(j, matches.front()->loadInternalInt());
  }

  // Check that the entry we failed to insert is not present.
  std::vector<TypedValue> failed_key(this->createCompositeKey(i * (i + 1)));
  std::vector<const TestHashPayload*> failed_matches;
  this->hash_table_->getAllCompositeKey(failed_key, &failed_matches);
  EXPECT_TRUE(failed_matches.empty());
}

TYPED_TEST_P(DuplicateKeysForbiddenHashTableTestLongOnly, LongKeyUpsertTest) {
  this->runScalarKeyUpsertTest(TypeID::kLong);
}

TYPED_TEST_P(DuplicateKeysForbiddenHashTableTest, CharKeyUpsertTest) {
  this->runScalarKeyUpsertTest(TypeID::kChar);
}

TYPED_TEST_P(DuplicateKeysForbiddenHashTableTest, VarCharKeyUpsertTest) {
  this->runScalarKeyUpsertTest(TypeID::kVarChar);
}

TYPED_TEST_P(DuplicateKeysForbiddenHashTableTest, CompositeKeyUpsertTest) {
  std::vector<const Type*> key_types = this->SetupCompositeKeyType();
  this->createHashTableSimple(key_types, kNumSampleKeys);

  // Upsert some key/value pairs.
  TestHashPayload initial_value(42);
  TestUpserter upserter;
  for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
    std::vector<TypedValue> key(this->createCompositeKey(i * (i + 1)));
    EXPECT_TRUE(this->hash_table_->upsertCompositeKey(key, initial_value, &upserter));
  }
  EXPECT_EQ(static_cast<std::size_t>(kNumSampleKeys), upserter.call_count());

  // Visit every other value a second time.
  for (std::int64_t i = 0; i < kNumSampleKeys; i += 2) {
    std::vector<TypedValue> key(this->createCompositeKey(i * (i + 1)));
    EXPECT_TRUE(this->hash_table_->upsertCompositeKey(key, initial_value, &upserter));
  }
  EXPECT_EQ(static_cast<std::size_t>(kNumSampleKeys + (kNumSampleKeys / 2)),
            upserter.call_count());

  // Read out the values and check that they are as expected (initial value
  // for each was set to 42, and each call to TestUpserter increments by 1).
  for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
    std::vector<TypedValue> key(this->createCompositeKey(i * (i + 1)));

    const TestHashPayload *value = this->hash_table_->getSingleCompositeKey(key);
    ASSERT_NE(nullptr, value);
    EXPECT_EQ(44 - (i % 2), value->loadInternalInt());
  }
}

TYPED_TEST_P(DuplicateKeysForbiddenHashTableTestLongOnly, LongKeyUpsertValueAccessorTest) {
  this->runScalarKeyUpsertValueAccessorTest(TypeID::kLong);
}

TYPED_TEST_P(DuplicateKeysForbiddenHashTableTest, CharKeyUpsertValueAccessorTest) {
  this->runScalarKeyUpsertValueAccessorTest(TypeID::kChar);
}

TYPED_TEST_P(DuplicateKeysForbiddenHashTableTest, VarCharKeyUpsertValueAccessorTest) {
  this->runScalarKeyUpsertValueAccessorTest(TypeID::kVarChar);
}

TYPED_TEST_P(DuplicateKeysForbiddenHashTableTest, CompositeKeyUpsertValueAccessorTest) {
  std::vector<const Type*> key_types = this->SetupCompositeKeyType();
  this->createHashTableSimple(key_types, kNumSampleKeys);

  std::unique_ptr<ColumnVectorsValueAccessor> accessor(this->createSampleValueAccessor());

  std::vector<attribute_id> key_attr_ids;
  key_attr_ids.push_back(0);
  key_attr_ids.push_back(1);
  key_attr_ids.push_back(2);

  // Vectorized upsert, skipping over null keys.
  TestHashPayload initial_value(42);
  TestUpserter upserter;
  EXPECT_TRUE(this->hash_table_->upsertValueAccessorCompositeKey(accessor.get(),
                                                                 key_attr_ids,
                                                                 true,
                                                                 initial_value,
                                                                 &upserter));
  EXPECT_EQ(static_cast<std::size_t>(kNumSampleKeys), upserter.call_count());

  // Read values out and check that they are as expected.
  for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
    std::vector<TypedValue> key(this->createCompositeKey(i * (i + 1)));
    std::vector<const TestHashPayload*> matches;
    this->hash_table_->getAllCompositeKey(key, &matches);
    ASSERT_EQ(1u, matches.size());
    EXPECT_EQ(43, matches.front()->loadInternalInt());
  }
}

TYPED_TEST_P(FixedSizeSerializableHashTableTestLongOnly, LongKeySerializationTest) {
  this->runScalarKeySerializationTest(TypeID::kLong);
}

TYPED_TEST_P(FixedSizeSerializableHashTableTest, CharKeySerializationTest) {
  this->runScalarKeySerializationTest(TypeID::kChar);
}

TYPED_TEST_P(FixedSizeSerializableHashTableTest, VarCharKeySerializationTest) {
  this->runScalarKeySerializationTest(TypeID::kVarChar);
}

TYPED_TEST_P(FixedSizeSerializableHashTableTest, CompositeKeySerializationTest) {
  std::vector<const Type*> key_types = this->SetupCompositeKeyType();
  std::size_t maximum_key_size = 0;
  for (const Type *type : key_types) {
    maximum_key_size += type->maximumByteLength();
  }
  // Set up a shared memory buffer which will hold both the external keys and
  // the hash table.
  const std::size_t buffer_size
      = kNumSampleKeys * maximum_key_size
        + 8 * kNumSampleKeys * (maximum_key_size + sizeof(typename TypeParam::value_type));
  this->createNonresizableHashTable(key_types,
                                    buffer_size,
                                    kNumSampleKeys * maximum_key_size);

  // Create external keys in the first part of the buffer.
  char *key_ptr = static_cast<char*>(this->memory_buffer_->get());
  std::vector<std::vector<std::size_t>> key_sizes;
  for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
    std::vector<TypedValue> key(this->createCompositeKey(i * (i + 1)));
    std::vector<std::size_t> component_sizes;
    for (const TypedValue &key_component : key) {
      key_component.copyInto(key_ptr);
      component_sizes.push_back(key_component.getDataSize());
      key_ptr += key_component.getDataSize();
    }
    key_sizes.emplace_back(component_sizes);
  }

  // Now actually insert.
  key_ptr = static_cast<char*>(this->memory_buffer_->get());
  for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
    std::vector<TypedValue> key;
    for (std::size_t component_idx = 0;
         component_idx < key_types.size();
         ++component_idx) {
      key.emplace_back(key_types[component_idx]->makeValue(key_ptr, key_sizes[i][component_idx]));
      key_ptr += key_sizes[i][component_idx];
    }
    TestHashPayload value(i);
    EXPECT_EQ(HashTablePutResult::kOK, this->hash_table_->putCompositeKey(key, value));
  }

  // Destroy the HashTable object and copy the underlying memory.
  this->hash_table_.reset(nullptr);
  ScopedBuffer moved_buffer(buffer_size, kCacheLineBytes);
  std::memcpy(moved_buffer.get(), this->memory_buffer_->get(), buffer_size);
  this->memory_buffer_.reset(nullptr);

  // Recreate the HashTable at the new location.
  this->hash_table_.reset(new TypeParam(
      key_types,
      static_cast<char*>(moved_buffer.get()) + kNumSampleKeys * maximum_key_size,
      buffer_size - kNumSampleKeys * maximum_key_size,
      false,
      false));

  // If duplicate keys are not allowed, make sure we can't insert a
  // duplicate.
  if (!TypeParam::template_allow_duplicate_keys) {
    std::vector<TypedValue> key;
    key_ptr = static_cast<char*>(moved_buffer.get());
    for (std::size_t component_idx = 0;
         component_idx < key_types.size();
         ++component_idx) {
      key.emplace_back(key_types[component_idx]->makeValue(key_ptr, key_sizes[0][component_idx]));
      key_ptr += key_sizes[0][component_idx];
    }
    TestHashPayload value(123);
    EXPECT_EQ(HashTablePutResult::kDuplicateKey, this->hash_table_->putCompositeKey(key, value));
  }

  // Read the inserted values back out.
  for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
    std::vector<TypedValue> key(this->createCompositeKey(i * (i + 1)));

    // Test getAll() in all cases.
    std::vector<const TestHashPayload*> matches;
    this->hash_table_->getAllCompositeKey(key, &matches);
    ASSERT_EQ(1u, matches.size());
    EXPECT_EQ(i, matches.front()->loadInternalInt());
  }

  // Check a nonexistent key.
  std::vector<TypedValue> nonexistent_key(
      this->createCompositeKey(kNumSampleKeys * kNumSampleKeys * kNumSampleKeys));
  std::vector<const TestHashPayload*> nonexistent_matches;
  this->hash_table_->getAllCompositeKey(nonexistent_key, &nonexistent_matches);
  EXPECT_TRUE(nonexistent_matches.empty());
}

TYPED_TEST_P(NonTriviallyDestructibleValueHashTableTestLongOnly, LongKeyDestroyValueTest) {
  this->runScalarKeyDestroyValueTest(TypeID::kLong);
}

TYPED_TEST_P(NonTriviallyDestructibleValueHashTableTest, CharKeyDestroyValueTest) {
  this->runScalarKeyDestroyValueTest(TypeID::kChar);
}

TYPED_TEST_P(NonTriviallyDestructibleValueHashTableTest, VarCharKeyDestroyValueTest) {
  this->runScalarKeyDestroyValueTest(TypeID::kVarChar);
}

TYPED_TEST_P(NonTriviallyDestructibleValueHashTableTest, CompositeKeyDestroyValueTest) {
  std::vector<const Type*> key_types = this->SetupCompositeKeyType();
  this->createHashTableSimple(key_types, kNumSampleKeys);

  std::atomic<int> destruct_count(0);

  // Insert some key/value pairs.
  for (std::int64_t i = 0; i < kNumSampleKeys; ++i) {
    std::vector<TypedValue> key(this->createCompositeKey(i * (i + 1)));
    NonTriviallyDestructibleTestHashPayload value(&destruct_count);
    EXPECT_EQ(HashTablePutResult::kOK, this->hash_table_->putCompositeKey(key, value));
    value.clearDestructCountPtr();
  }

  // Destroy the hash table and check that the destructors of inserted values
  // were invoked.
  this->hash_table_.reset(nullptr);
  EXPECT_EQ(kNumSampleKeys, destruct_count.load(std::memory_order_relaxed));
}

REGISTER_TYPED_TEST_CASE_P(HashTableTestLongOnly,
                           LongKeyPutAndGetTest,
                           LongKeyPutDuplicateKeysTest,
                           LongKeyPutAndGetFromValueAccessorTest,
                           LongKeyThreadedPutTest,
                           LongKeyClearTest);

REGISTER_TYPED_TEST_CASE_P(HashTableTest,
                           CharKeyPutAndGetTest,
                           VarCharKeyPutAndGetTest,
                           CompositeKeyPutAndGetTest,
                           SpecialHashesPutAndGetTest,
                           CharKeyPutDuplicateKeysTest,
                           VarCharKeyPutDuplicateKeysTest,
                           CompositeKeyPutDuplicateKeysTest,
                           CharKeyPutAndGetFromValueAccessorTest,
                           VarCharKeyPutAndGetFromValueAccessorTest,
                           CompositeKeyPutAndGetFromValueAccessorTest,
                           CharKeyThreadedPutTest,
                           VarCharKeyThreadedPutTest,
                           CompositeKeyThreadedPutTest,
                           CharKeyClearTest,
                           VarCharKeyClearTest,
                           CompositeKeyClearTest);

REGISTER_TYPED_TEST_CASE_P(ResizableHashTableTestLongOnly,
                           LongKeyResizeTest,
                           LongKeyThreadedResizeTest);

REGISTER_TYPED_TEST_CASE_P(ResizableHashTableTest,
                           CharKeyResizeTest,
                           VarCharKeyResizeTest,
                           CompositeKeyResizeTest,
                           CharKeyThreadedResizeTest,
                           VarCharKeyThreadedResizeTest,
                           CompositeKeyThreadedResizeTest);

REGISTER_TYPED_TEST_CASE_P(NonResizableHashTableTestLongOnly,
                           LongKeyExhaustSpaceTest);

REGISTER_TYPED_TEST_CASE_P(NonResizableHashTableTest,
                           CharKeyExhaustSpaceTest,
                           VarCharKeyExhaustSpaceTest,
                           CompositeKeyExhaustSpaceTest);

REGISTER_TYPED_TEST_CASE_P(DuplicateKeysForbiddenHashTableTestLongOnly,
                           LongKeyUpsertTest,
                           LongKeyUpsertValueAccessorTest);

REGISTER_TYPED_TEST_CASE_P(DuplicateKeysForbiddenHashTableTest,
                           CharKeyUpsertTest,
                           VarCharKeyUpsertTest,
                           CompositeKeyUpsertTest,
                           CharKeyUpsertValueAccessorTest,
                           VarCharKeyUpsertValueAccessorTest,
                           CompositeKeyUpsertValueAccessorTest);

REGISTER_TYPED_TEST_CASE_P(FixedSizeSerializableHashTableTestLongOnly,
                           LongKeySerializationTest);

REGISTER_TYPED_TEST_CASE_P(FixedSizeSerializableHashTableTest,
                           CharKeySerializationTest,
                           VarCharKeySerializationTest,
                           CompositeKeySerializationTest);

REGISTER_TYPED_TEST_CASE_P(NonTriviallyDestructibleValueHashTableTestLongOnly,
                           LongKeyDestroyValueTest);

REGISTER_TYPED_TEST_CASE_P(NonTriviallyDestructibleValueHashTableTest,
                           CharKeyDestroyValueTest,
                           VarCharKeyDestroyValueTest,
                           CompositeKeyDestroyValueTest);

}  // namespace quickstep

// Macro to automatically instantiate tests for all the different flavors of
// the specificied HashTable implementation template.
#define QUICKSTEP_TEST_HASHTABLE_IMPL(HashTableImpl)                        \
namespace quickstep {                                                       \
                                                                            \
typedef ::testing::Types<                                                   \
    HashTableImpl<TestHashPayload, false, false, false, false>,             \
    HashTableImpl<TestHashPayload, false, false, false, true>,              \
    HashTableImpl<TestHashPayload, false, false, true, false>,              \
    HashTableImpl<TestHashPayload, false, false, true, true>,               \
    HashTableImpl<TestHashPayload, false, true, false, false>,              \
    HashTableImpl<TestHashPayload, false, true, false, true>,               \
    HashTableImpl<TestHashPayload, false, true, true, false>,               \
    HashTableImpl<TestHashPayload, false, true, true, true>,                \
    HashTableImpl<TestHashPayload, true, false, false, false>,              \
    HashTableImpl<TestHashPayload, true, false, false, true>,               \
    HashTableImpl<TestHashPayload, true, false, true, false>,               \
    HashTableImpl<TestHashPayload, true, false, true, true>,                \
    HashTableImpl<TestHashPayload, true, true, true, false>,                \
    HashTableImpl<TestHashPayload, true, true, true, true>>                 \
        _CommonTestTypes;                                                   \
INSTANTIATE_TYPED_TEST_CASE_P(HashTableImpl,                                \
                              HashTableTestLongOnly,                        \
                              _CommonTestTypes);                            \
INSTANTIATE_TYPED_TEST_CASE_P(HashTableImpl,                                \
                              HashTableTest,                                \
                              _CommonTestTypes);                            \
                                                                            \
typedef ::testing::Types<                                                   \
    HashTableImpl<TestHashPayload, true, false, false, false>,              \
    HashTableImpl<TestHashPayload, true, false, false, true>,               \
    HashTableImpl<TestHashPayload, true, false, true, false>,               \
    HashTableImpl<TestHashPayload, true, false, true, true>,                \
    HashTableImpl<TestHashPayload, true, true, true, false>,                \
    HashTableImpl<TestHashPayload, true, true, true, true>>                 \
        _ResizableTestTypes;                                                \
INSTANTIATE_TYPED_TEST_CASE_P(HashTableImpl,                                \
                              ResizableHashTableTestLongOnly,               \
                              _ResizableTestTypes);                         \
INSTANTIATE_TYPED_TEST_CASE_P(HashTableImpl,                                \
                              ResizableHashTableTest,                       \
                              _ResizableTestTypes);                         \
                                                                            \
typedef ::testing::Types<                                                   \
    HashTableImpl<TestHashPayload, false, false, false, false>,             \
    HashTableImpl<TestHashPayload, false, false, false, true>,              \
    HashTableImpl<TestHashPayload, false, false, true, false>,              \
    HashTableImpl<TestHashPayload, false, false, true, true>,               \
    HashTableImpl<TestHashPayload, false, true, false, false>,              \
    HashTableImpl<TestHashPayload, false, true, false, true>,               \
    HashTableImpl<TestHashPayload, false, true, true, false>,               \
    HashTableImpl<TestHashPayload, false, true, true, true>>                \
        _NonResizableTestTypes;                                             \
INSTANTIATE_TYPED_TEST_CASE_P(HashTableImpl,                                \
                              NonResizableHashTableTestLongOnly,            \
                              _NonResizableTestTypes);                      \
INSTANTIATE_TYPED_TEST_CASE_P(HashTableImpl,                                \
                              NonResizableHashTableTest,                    \
                              _NonResizableTestTypes);                      \
                                                                            \
typedef ::testing::Types<                                                   \
    HashTableImpl<TestHashPayload, false, false, false, false>,             \
    HashTableImpl<TestHashPayload, false, false, true, false>,              \
    HashTableImpl<TestHashPayload, false, true, false, false>,              \
    HashTableImpl<TestHashPayload, false, true, true, false>,               \
    HashTableImpl<TestHashPayload, true, false, false, false>,              \
    HashTableImpl<TestHashPayload, true, false, true, false>,               \
    HashTableImpl<TestHashPayload, true, true, true, false>>                \
        _DuplicateKeysForbiddenTypes;                                       \
INSTANTIATE_TYPED_TEST_CASE_P(HashTableImpl,                                \
                              DuplicateKeysForbiddenHashTableTestLongOnly,  \
                              _DuplicateKeysForbiddenTypes);                \
INSTANTIATE_TYPED_TEST_CASE_P(HashTableImpl,                                \
                              DuplicateKeysForbiddenHashTableTest,          \
                              _DuplicateKeysForbiddenTypes);                \
                                                                            \
typedef ::testing::Types<                                                   \
    HashTableImpl<TestHashPayload, false, true, false, false>,              \
    HashTableImpl<TestHashPayload, false, true, false, true>,               \
    HashTableImpl<TestHashPayload, false, true, true, false>,               \
    HashTableImpl<TestHashPayload, false, true, true, true>>                \
        _FixedSizeSerializableTestTypes;                                    \
INSTANTIATE_TYPED_TEST_CASE_P(HashTableImpl,                                \
                              FixedSizeSerializableHashTableTestLongOnly,   \
                              _FixedSizeSerializableTestTypes);             \
INSTANTIATE_TYPED_TEST_CASE_P(HashTableImpl,                                \
                              FixedSizeSerializableHashTableTest,           \
                              _FixedSizeSerializableTestTypes);             \
                                                                            \
typedef ::testing::Types<                                                   \
    HashTableImpl<NonTriviallyDestructibleTestHashPayload,                  \
                  false, false, false, false>,                              \
    HashTableImpl<NonTriviallyDestructibleTestHashPayload,                  \
                  false, false, false, true>,                               \
    HashTableImpl<NonTriviallyDestructibleTestHashPayload,                  \
                  false, false, true, false>,                               \
    HashTableImpl<NonTriviallyDestructibleTestHashPayload,                  \
                  false, false, true, true>,                                \
    HashTableImpl<NonTriviallyDestructibleTestHashPayload,                  \
                  true, false, false, false>,                               \
    HashTableImpl<NonTriviallyDestructibleTestHashPayload,                  \
                  true, false, false, true>,                                \
    HashTableImpl<NonTriviallyDestructibleTestHashPayload,                  \
                  true, false, true, false>,                                \
    HashTableImpl<NonTriviallyDestructibleTestHashPayload,                  \
                  true, false, true, true>>                                 \
        _NonTriviallyDestructibleNonSerializableTestTypes;                  \
INSTANTIATE_TYPED_TEST_CASE_P(                                              \
    HashTableImpl,                                                          \
    NonTriviallyDestructibleValueHashTableTestLongOnly,                     \
    _NonTriviallyDestructibleNonSerializableTestTypes);                     \
INSTANTIATE_TYPED_TEST_CASE_P(                                              \
    HashTableImpl,                                                          \
    NonTriviallyDestructibleValueHashTableTest,                             \
    _NonTriviallyDestructibleNonSerializableTestTypes);                     \
                                                                            \
}  /* namespace quickstep */                                                \
struct quickstep_hashtable_test_dummy  /* NOLINT(build/class) */

// Similar to QUICKSTEP_TEST_HASHTABLE_IMPL(), but only instantiates test cases
// that use LONG keys.
#define QUICKSTEP_TEST_HASHTABLE_IMPL_LONG_ONLY(HashTableImpl)              \
namespace quickstep {                                                       \
                                                                            \
typedef ::testing::Types<                                                   \
    HashTableImpl<TestHashPayload, false, false, false, false>,             \
    HashTableImpl<TestHashPayload, false, false, false, true>,              \
    HashTableImpl<TestHashPayload, false, false, true, false>,              \
    HashTableImpl<TestHashPayload, false, false, true, true>,               \
    HashTableImpl<TestHashPayload, false, true, false, false>,              \
    HashTableImpl<TestHashPayload, false, true, false, true>,               \
    HashTableImpl<TestHashPayload, false, true, true, false>,               \
    HashTableImpl<TestHashPayload, false, true, true, true>,                \
    HashTableImpl<TestHashPayload, true, false, false, false>,              \
    HashTableImpl<TestHashPayload, true, false, false, true>,               \
    HashTableImpl<TestHashPayload, true, false, true, false>,               \
    HashTableImpl<TestHashPayload, true, false, true, true>,                \
    HashTableImpl<TestHashPayload, true, true, true, false>,                \
    HashTableImpl<TestHashPayload, true, true, true, true>>                 \
        _CommonTestTypes;                                                   \
INSTANTIATE_TYPED_TEST_CASE_P(HashTableImpl,                                \
                              HashTableTestLongOnly,                        \
                              _CommonTestTypes);                            \
                                                                            \
typedef ::testing::Types<                                                   \
    HashTableImpl<TestHashPayload, true, false, false, false>,              \
    HashTableImpl<TestHashPayload, true, false, false, true>,               \
    HashTableImpl<TestHashPayload, true, false, true, false>,               \
    HashTableImpl<TestHashPayload, true, false, true, true>,                \
    HashTableImpl<TestHashPayload, true, true, true, false>,                \
    HashTableImpl<TestHashPayload, true, true, true, true>>                 \
        _ResizableTestTypes;                                                \
INSTANTIATE_TYPED_TEST_CASE_P(HashTableImpl,                                \
                              ResizableHashTableTestLongOnly,               \
                              _ResizableTestTypes);                         \
                                                                            \
typedef ::testing::Types<                                                   \
    HashTableImpl<TestHashPayload, false, false, false, false>,             \
    HashTableImpl<TestHashPayload, false, false, false, true>,              \
    HashTableImpl<TestHashPayload, false, false, true, false>,              \
    HashTableImpl<TestHashPayload, false, false, true, true>,               \
    HashTableImpl<TestHashPayload, false, true, false, false>,              \
    HashTableImpl<TestHashPayload, false, true, false, true>,               \
    HashTableImpl<TestHashPayload, false, true, true, false>,               \
    HashTableImpl<TestHashPayload, false, true, true, true>>                \
        _NonResizableTestTypes;                                             \
INSTANTIATE_TYPED_TEST_CASE_P(HashTableImpl,                                \
                              NonResizableHashTableTestLongOnly,            \
                              _NonResizableTestTypes);                      \
                                                                            \
typedef ::testing::Types<                                                   \
    HashTableImpl<TestHashPayload, false, false, false, false>,             \
    HashTableImpl<TestHashPayload, false, false, true, false>,              \
    HashTableImpl<TestHashPayload, false, true, false, false>,              \
    HashTableImpl<TestHashPayload, false, true, true, false>,               \
    HashTableImpl<TestHashPayload, true, false, false, false>,              \
    HashTableImpl<TestHashPayload, true, false, true, false>,               \
    HashTableImpl<TestHashPayload, true, true, true, false>>                \
        _DuplicateKeysForbiddenTypes;                                       \
INSTANTIATE_TYPED_TEST_CASE_P(HashTableImpl,                                \
                              DuplicateKeysForbiddenHashTableTestLongOnly,  \
                              _DuplicateKeysForbiddenTypes);                \
                                                                            \
typedef ::testing::Types<                                                   \
    HashTableImpl<TestHashPayload, false, true, false, false>,              \
    HashTableImpl<TestHashPayload, false, true, false, true>,               \
    HashTableImpl<TestHashPayload, false, true, true, false>,               \
    HashTableImpl<TestHashPayload, false, true, true, true>>                \
        _FixedSizeSerializableTestTypes;                                    \
INSTANTIATE_TYPED_TEST_CASE_P(HashTableImpl,                                \
                              FixedSizeSerializableHashTableTestLongOnly,   \
                              _FixedSizeSerializableTestTypes);             \
                                                                            \
typedef ::testing::Types<                                                   \
    HashTableImpl<NonTriviallyDestructibleTestHashPayload,                  \
                  false, false, false, false>,                              \
    HashTableImpl<NonTriviallyDestructibleTestHashPayload,                  \
                  false, false, false, true>,                               \
    HashTableImpl<NonTriviallyDestructibleTestHashPayload,                  \
                  false, false, true, false>,                               \
    HashTableImpl<NonTriviallyDestructibleTestHashPayload,                  \
                  false, false, true, true>,                                \
    HashTableImpl<NonTriviallyDestructibleTestHashPayload,                  \
                  true, false, false, false>,                               \
    HashTableImpl<NonTriviallyDestructibleTestHashPayload,                  \
                  true, false, false, true>,                                \
    HashTableImpl<NonTriviallyDestructibleTestHashPayload,                  \
                  true, false, true, false>,                                \
    HashTableImpl<NonTriviallyDestructibleTestHashPayload,                  \
                  true, false, true, true>>                                 \
        _NonTriviallyDestructibleNonSerializableTestTypes;                  \
INSTANTIATE_TYPED_TEST_CASE_P(                                              \
    HashTableImpl,                                                          \
    NonTriviallyDestructibleValueHashTableTestLongOnly,                     \
    _NonTriviallyDestructibleNonSerializableTestTypes);                     \
                                                                            \
}  /* namespace quickstep */                                                \
struct quickstep_hashtable_test_dummy  /* NOLINT(build/class) */

#endif  // QUICKSTEP_STORAGE_TESTS_HASHTABLE_UNITTEST_COMMON_HPP_
