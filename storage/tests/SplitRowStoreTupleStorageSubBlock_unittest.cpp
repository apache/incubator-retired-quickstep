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

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "storage/SplitRowStoreTupleStorageSubBlock.hpp"
#include "storage/SplitRowStoreValueAccessor.hpp"
#include "storage/StorageBlockLayout.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageConstants.hpp"
#include "storage/StorageErrors.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "storage/ValueAccessor.hpp"
#include "types/CharType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/VarCharType.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "types/containers/Tuple.hpp"
#include "utility/BitVector.hpp"
#include "utility/EqualsAnyConstant.hpp"
#include "utility/ScopedBuffer.hpp"

// snprintf() is available using NetBSD's libc, but doesn't show up in the std
// namespace for C++.
#ifndef __NetBSD__
using std::snprintf;
#endif

namespace quickstep {

using splitrow_internal::CopyGroupList;
using splitrow_internal::ContiguousAttrs;
using splitrow_internal::NullableAttr;
using splitrow_internal::VarLenAttr;

namespace {

// Used to set up a value-parameterized test with certain features for
// attribute types.
enum class AttributeTypeFeatures {
  kNone,
  kNullable,
  kVariableLength,
  kNullableAndVariableLength
};

}  // namespace

class SplitRowStoreTupleStorageSubBlockTest
    : public ::testing::TestWithParam<AttributeTypeFeatures> {
 public:
  static const std::size_t kSubBlockSize = 0x100000;  // 1 MB
  static const std::size_t kVarLenSize = 26;

 protected:
  virtual void SetUp() {
    // Create a sample relation with a variety of attribute types.
    relation_.reset(new CatalogRelation(nullptr, "TestRelation"));

    // An integer.
    CatalogAttribute *current_attr = new CatalogAttribute(
        relation_.get(),
        "int_attr",
        TypeFactory::GetType(kInt, testNullable()));
    ASSERT_EQ(0, relation_->addAttribute(current_attr));

    // A double.
    current_attr = new CatalogAttribute(
        relation_.get(),
        "double_attr",
        TypeFactory::GetType(kDouble, testNullable()));
    ASSERT_EQ(1, relation_->addAttribute(current_attr));

    // A (possibly variable-length) string.
    current_attr = new CatalogAttribute(
        relation_.get(),
        "string_attr",
        TypeFactory::GetType(testVariableLength() ? kVarChar : kChar,
                             kVarLenSize,
                             testNullable()));
    ASSERT_EQ(2, relation_->addAttribute(current_attr));

    tuple_store_description_.reset(new TupleStorageSubBlockDescription());
    tuple_store_description_->set_sub_block_type(TupleStorageSubBlockDescription::SPLIT_ROW_STORE);

    // Initialize the actual block.
    tuple_store_memory_.reset(kSubBlockSize);
    std::memset(tuple_store_memory_.get(), 0x0, kSubBlockSize);
    tuple_store_.reset(new SplitRowStoreTupleStorageSubBlock(*relation_,
                                                             *tuple_store_description_,
                                                             true,
                                                             tuple_store_memory_.get(),
                                                             kSubBlockSize));
  }

  inline bool testNullable() const {
    switch (GetParam()) {
      case AttributeTypeFeatures::kNullable:
      case AttributeTypeFeatures::kNullableAndVariableLength:
        return true;
      default:
        return false;
    }
  }

  inline bool testVariableLength() const {
    switch (GetParam()) {
      case AttributeTypeFeatures::kVariableLength:
      case AttributeTypeFeatures::kNullableAndVariableLength:
        return true;
      default:
        return false;
    }
  }

  std::size_t getTupleSlotSize() const {
    return tuple_store_->tuple_slot_bytes_;
  }

  std::size_t getTupleStorageSize() const {
    return tuple_store_->tuple_storage_bytes_;
  }

  std::size_t getTupleInsertLowerBound() const {
    return tuple_store_->getInsertLowerBound();
  }

  std::size_t getInsertLowerBoundThreshold() const {
    return tuple_store_->getInsertLowerBoundThreshold();
  }

  Tuple createSampleTuple(const int base_value) const {
    std::vector<TypedValue> attribute_values;

    // int_attr
    if (testNullable() && (base_value % 6 == 0)) {
      // Throw in a NULL integer for every sixth value.
      attribute_values.emplace_back(kInt);
    } else {
      attribute_values.emplace_back(base_value);
    }

    // double_attr
    if (testNullable() && (base_value % 6 == 2)) {
      // NULL very sixth value.
      attribute_values.emplace_back(kDouble);
    } else {
      attribute_values.emplace_back(static_cast<double>(0.25 * base_value));
    }

    // string_attr
    if (testNullable() && (base_value % 6 == 4)) {
      // NULL very sixth value.
      attribute_values.emplace_back(testVariableLength() ? kVarChar : kChar);
    } else {
      char string_buffer[13];
      int written = snprintf(string_buffer, sizeof(string_buffer), "%d", base_value);
      if (testVariableLength()) {
        attribute_values.emplace_back((VarCharType::InstanceNonNullable(kVarLenSize).makeValue(string_buffer,
                                                                                      written + 1)));
      } else {
        attribute_values.emplace_back((CharType::InstanceNonNullable(kVarLenSize).makeValue(string_buffer,
                                                                                   written + 1)));
      }
      attribute_values.back().ensureNotReference();
    }

    return Tuple(std::move(attribute_values));
  }

  void fillBlockWithSampleData() {
    tuple_id current_tid = 0;
    Tuple current_tuple(createSampleTuple(current_tid));
    while (tuple_store_->insertTupleInBatch(current_tuple)) {
      ++current_tid;
      current_tuple = createSampleTuple(current_tid);
    }

    tuple_store_->rebuild();
  }

  void getCopyGroupsForAttributeMap(const std::vector<attribute_id> &attribute_map,
                                    CopyGroupList *copy_groups) {
    tuple_store_->getCopyGroupsForAttributeMap(attribute_map, copy_groups);
  }

  void checkTupleValuesUntyped(const tuple_id tid,
                               const int base_value) {
    ASSERT_TRUE(tuple_store_->hasTupleWithID(tid));
    ASSERT_TRUE(tuple_store_->supportsUntypedGetAttributeValue(0));
    ASSERT_TRUE(tuple_store_->supportsUntypedGetAttributeValue(1));
    ASSERT_TRUE(tuple_store_->supportsUntypedGetAttributeValue(2));

    Tuple comp_tuple(createSampleTuple(base_value));

    if (comp_tuple.getAttributeValue(0).isNull()) {
      EXPECT_EQ(nullptr, tuple_store_->getAttributeValue(tid, 0));
    } else {
      ASSERT_NE(nullptr, tuple_store_->getAttributeValue(tid, 0));
      EXPECT_EQ(comp_tuple.getAttributeValue(0).getLiteral<int>(),
                *static_cast<const int*>(tuple_store_->getAttributeValue(tid, 0)));
    }

    if (comp_tuple.getAttributeValue(1).isNull()) {
      EXPECT_EQ(nullptr, tuple_store_->getAttributeValue(tid, 1));
    } else {
      ASSERT_NE(nullptr, tuple_store_->getAttributeValue(tid, 1));
      EXPECT_EQ(comp_tuple.getAttributeValue(1).getLiteral<double>(),
                *static_cast<const double*>(tuple_store_->getAttributeValue(tid, 1)));
    }

    if (comp_tuple.getAttributeValue(2).isNull()) {
      EXPECT_EQ(nullptr, tuple_store_->getAttributeValue(tid, 2));
    } else {
      ASSERT_NE(nullptr, tuple_store_->getAttributeValue(tid, 2));
      EXPECT_STREQ(static_cast<const char*>(comp_tuple.getAttributeValue(2).getDataPtr()),
                   static_cast<const char*>(tuple_store_->getAttributeValue(tid, 2)));
    }
  }

  void checkTupleValuesTyped(const tuple_id tid,
                             const int base_value) {
    ASSERT_TRUE(tuple_store_->hasTupleWithID(tid));

    Tuple comp_tuple(createSampleTuple(base_value));

    TypedValue int_attr_value = tuple_store_->getAttributeValueTyped(tid, 0);
    if (comp_tuple.getAttributeValue(0).isNull()) {
      EXPECT_TRUE(int_attr_value.isNull());
    } else {
      EXPECT_EQ(comp_tuple.getAttributeValue(0).getLiteral<int>(),
                int_attr_value.getLiteral<int>());
    }

    TypedValue double_attr_value = tuple_store_->getAttributeValueTyped(tid, 1);
    if (comp_tuple.getAttributeValue(1).isNull()) {
      EXPECT_TRUE(double_attr_value.isNull());
    } else {
      EXPECT_EQ(comp_tuple.getAttributeValue(1).getLiteral<double>(),
                double_attr_value.getLiteral<double>());
    }

    TypedValue string_attr_value = tuple_store_->getAttributeValueTyped(tid, 2);
    if (comp_tuple.getAttributeValue(2).isNull()) {
      EXPECT_TRUE(string_attr_value.isNull());
    } else {
      EXPECT_STREQ(static_cast<const char*>(comp_tuple.getAttributeValue(2).getDataPtr()),
                   static_cast<const char*>(string_attr_value.getDataPtr()));
    }
  }

  std::unique_ptr<CatalogRelation> relation_;
  std::unique_ptr<TupleStorageSubBlockDescription> tuple_store_description_;
  ScopedBuffer tuple_store_memory_;
  std::unique_ptr<SplitRowStoreTupleStorageSubBlock> tuple_store_;
};
typedef SplitRowStoreTupleStorageSubBlockTest SplitRowStoreTupleStorageSubBlockDeathTest;

class SplitRowWrapper {
 public:
  enum AttrType {
    kInt = 0,
    kDouble,
    kString,
    kNumAttrTypes
  };

  /**
   * Builds a catalog relation given a list of attributes.
   *
   * @param attribute_ordering The ordering of the attributes in the represented relation. Attribute #1 is an
   *                integer attribute, #2 is a double, and #3 is a string.
   * @param contains_nullable If the relation contains nullable attributes.
   * @param contains_varlen If the relation contains variable length attributes.
   * @return A caller-owned catalog relation.
   */
  static CatalogRelation *
  GetRelationFromAttributeList(const std::vector<attribute_id> &attribute_ordering, bool contains_nullable,
                               bool contains_varlen) {
    // Create a unique name.
    std::string rel_name("TempRelation");
    for (auto attr_itr = attribute_ordering.begin();
         attr_itr != attribute_ordering.end();
         ++attr_itr) {
      rel_name += "_" + std::to_string(*attr_itr);
    }
    CatalogRelation *relation = new CatalogRelation(nullptr, rel_name.c_str());

    std::vector<int> attr_counts(AttrType::kNumAttrTypes);
    std::string attr_name;
    for (auto attr_itr = attribute_ordering.begin();
         attr_itr != attribute_ordering.end();
         ++attr_itr) {
      switch (*attr_itr) {
        case AttrType::kInt:
          // An integer.
          attr_name = "int_attr_" + std::to_string(attr_counts[AttrType::kInt]);
          relation->addAttribute(new CatalogAttribute(
            relation,
            attr_name.c_str(),
            TypeFactory::GetType(TypeID::kInt, contains_nullable)));
          attr_counts[AttrType::kInt]++;
          break;
        case AttrType::kDouble:
          // A double.
          attr_name = "double_attr_" + std::to_string(attr_counts[AttrType::kDouble]);
          relation->addAttribute(new CatalogAttribute(
            relation,
            attr_name.c_str(),
            TypeFactory::GetType(TypeID::kDouble, contains_nullable)));
          attr_counts[AttrType::kDouble]++;
          break;
        case AttrType::kString:
          // A (possibly variable-length) string.
          attr_name = "string_attr_" + std::to_string(attr_counts[AttrType::kString]);
          relation->addAttribute(new CatalogAttribute(
            relation,
            attr_name.c_str(),
            TypeFactory::GetType(contains_varlen ? TypeID::kVarChar : TypeID::kChar,
                                 SplitRowStoreTupleStorageSubBlockTest::kVarLenSize,
                                 contains_nullable)));
          attr_counts[AttrType::kString]++;
          break;
        default:
          LOG(FATAL) << "Unknown type was specified in SplitRowWrapper.";
          break;
      }
    }
    return relation;
  }

  /**
   * A wrapper for an empty SplitRowstore.
   *
   * @param attribute_ordering The ordering of the attributes in the represented relation. Attribute #1 is an
   *                integer attribute, #2 is a double, and #3 is a string.
   * @param contains_nullable If the relation contains nullable attributes.
   * @param contains_varlen If the relation contains variable length attributes.
   */
  SplitRowWrapper(const std::vector<attribute_id> &attribute_ordering, bool contains_nullable, bool contains_varlen)
    : contains_nullable_(contains_nullable),
      contains_varlen_(contains_varlen) {
    initialize(attribute_ordering);
  }

  SplitRowWrapper(bool contains_nullable, bool contains_varlen)
    : contains_nullable_(contains_nullable),
      contains_varlen_(contains_varlen) {
    // Make a clone of the Test Block type using the 3 basic attributes.
    std::vector<attribute_id> attrs;
    for (attribute_id attr = 0; attr < 3; ++attr) {
      attrs.push_back(attr);
    }
    initialize(attrs);
  }

  SplitRowStoreTupleStorageSubBlock *operator->() {
    return tuple_store_.get();
  }

  const bool contains_nullable_;
  const bool contains_varlen_;

  std::unique_ptr<CatalogRelation> relation_;
  std::unique_ptr<TupleStorageSubBlockDescription> tuple_store_description_;
  ScopedBuffer tuple_store_memory_;
  std::unique_ptr<SplitRowStoreTupleStorageSubBlock> tuple_store_;

 private:
  void initialize(const std::vector<attribute_id> &attribute_ordering) {
    // Create a sample relation with a variety of attribute types.
    relation_.reset(GetRelationFromAttributeList(attribute_ordering, contains_nullable_, contains_varlen_));

    tuple_store_description_.reset(new TupleStorageSubBlockDescription());
    tuple_store_description_->set_sub_block_type(TupleStorageSubBlockDescription::SPLIT_ROW_STORE);

    // Initialize the actual block.
    tuple_store_memory_.reset(SplitRowStoreTupleStorageSubBlockTest::kSubBlockSize);
    std::memset(tuple_store_memory_.get(), 0x0, SplitRowStoreTupleStorageSubBlockTest::kSubBlockSize);
    tuple_store_.reset(new SplitRowStoreTupleStorageSubBlock(*relation_,
                                                             *tuple_store_description_,
                                                             true,
                                                             tuple_store_memory_.get(),
                                                             SplitRowStoreTupleStorageSubBlockTest::kSubBlockSize));
  }
};

TEST_P(SplitRowStoreTupleStorageSubBlockTest, DescriptionIsValidTest) {
  // The descriptions we use for the other tests (which includes nullable and
  // variable-length attributes) should be valid.
  EXPECT_TRUE(SplitRowStoreTupleStorageSubBlock::DescriptionIsValid(*relation_,
                                                                    *tuple_store_description_));

  // An uninitialized description is not valid.
  tuple_store_description_.reset(new TupleStorageSubBlockDescription());
  EXPECT_FALSE(SplitRowStoreTupleStorageSubBlock::DescriptionIsValid(*relation_,
                                                                     *tuple_store_description_));

  // A description that specifies the wrong sub_block_type is not valid.
  tuple_store_description_->set_sub_block_type(TupleStorageSubBlockDescription::BASIC_COLUMN_STORE);
  EXPECT_FALSE(SplitRowStoreTupleStorageSubBlock::DescriptionIsValid(*relation_,
                                                                     *tuple_store_description_));
}

TEST_P(SplitRowStoreTupleStorageSubBlockDeathTest, ConstructWithInvalidDescriptionTest) {
  tuple_store_.reset(nullptr);
  tuple_store_memory_.reset(kSubBlockSize);

  tuple_store_description_.reset(new TupleStorageSubBlockDescription());
  tuple_store_description_->set_sub_block_type(TupleStorageSubBlockDescription::BASIC_COLUMN_STORE);
  EXPECT_DEATH(tuple_store_.reset(new SplitRowStoreTupleStorageSubBlock(
                   *relation_,
                   *tuple_store_description_,
                   true,
                   tuple_store_memory_.get(),
                   kSubBlockSize)),
               "");
}

TEST_P(SplitRowStoreTupleStorageSubBlockTest, MemoryTooSmallTest) {
  // 1 byte short.
  tuple_store_.reset(nullptr);
  tuple_store_memory_.reset(4);  // 4 bytes is too small for header.
  EXPECT_THROW(tuple_store_.reset(new SplitRowStoreTupleStorageSubBlock(
                   *relation_,
                   *tuple_store_description_,
                   true,
                   tuple_store_memory_.get(),
                   4)),
               BlockMemoryTooSmall);
}

TEST_P(SplitRowStoreTupleStorageSubBlockTest, InitializeTest) {
  // We expect the header to be placed at the beginning of the block's memory.
  EXPECT_EQ(tuple_store_memory_.get(), tuple_store_->header_);

  // Check the Header's initial contents.
  EXPECT_EQ(0, tuple_store_->header_->num_tuples);
  EXPECT_EQ(-1, tuple_store_->header_->max_tid);
  EXPECT_EQ(0u, tuple_store_->header_->variable_length_bytes_allocated);
  EXPECT_TRUE(tuple_store_->header_->variable_length_storage_compact);

  // Check size of per-tuple null bitmaps.
  if (testNullable()) {
    EXPECT_EQ(BitVector<true>::BytesNeeded(3), tuple_store_->per_tuple_null_bitmap_bytes_);
  } else {
    EXPECT_EQ(0u, tuple_store_->per_tuple_null_bitmap_bytes_);
  }

  // Calculate the size of a tuple slot.
  const Type &int_attr_type = relation_->getAttributeById(0)->getType();
  const Type &double_attr_type = relation_->getAttributeById(1)->getType();
  const Type &string_attr_type = relation_->getAttributeById(2)->getType();
  const std::size_t tuple_slot_size
      = (testNullable() ? BitVector<true>::BytesNeeded(3) : 0)  // Null bitmap
        + int_attr_type.maximumByteLength()                     // int_attr
        + double_attr_type.maximumByteLength()                  // double_attr
        + (testVariableLength()
               ? 2 * sizeof(std::uint32_t)                      // string_attr offset, length
               : string_attr_type.maximumByteLength());         // string_attr inline storage
  EXPECT_EQ(tuple_slot_size, tuple_store_->tuple_slot_bytes_);

  // Calculate the maximum capacity for tuples and check that the occupancy
  // bitmap is properly set up.
  const std::size_t max_tuple_capacity
      = (kSubBlockSize - sizeof(SplitRowStoreTupleStorageSubBlock::Header))
        / (tuple_slot_size
           + (GetParam() == AttributeTypeFeatures::kVariableLength
              ? string_attr_type.minimumByteLength() : 0));  // Take into account minimum variable string.
  EXPECT_EQ(BitVector<false>::BytesNeeded(max_tuple_capacity),
            tuple_store_->occupancy_bitmap_bytes_);
  EXPECT_EQ(max_tuple_capacity, tuple_store_->occupancy_bitmap_->size());

  // Check that memory for actual tuple storage is located and sized correctly.
  EXPECT_EQ(kSubBlockSize
                - sizeof(SplitRowStoreTupleStorageSubBlock::Header)
                - BitVector<false>::BytesNeeded(max_tuple_capacity),
            tuple_store_->tuple_storage_bytes_);
  EXPECT_EQ(static_cast<const char*>(tuple_store_memory_.get())
                + sizeof(SplitRowStoreTupleStorageSubBlock::Header)
                + BitVector<false>::BytesNeeded(max_tuple_capacity),
            tuple_store_->tuple_storage_);
}

TEST_P(SplitRowStoreTupleStorageSubBlockTest, InsertTest) {
  const std::size_t tuple_storage_size = getTupleStorageSize();
  const std::size_t tuple_slot_size = getTupleSlotSize();
  std::size_t storage_used = 0;
  int current_tuple_idx = 0;
  for (;;) {
    Tuple current_tuple(createSampleTuple(current_tuple_idx));
    const std::size_t current_tuple_storage_bytes
        = tuple_slot_size
          + (testVariableLength() ? (current_tuple.getAttributeValue(2).isNull() ?
                                     0 : current_tuple.getAttributeValue(2).getDataSize())
                                  : 0);
    TupleStorageSubBlock::InsertResult result = tuple_store_->insertTuple(current_tuple);
    if (storage_used + current_tuple_storage_bytes <= tuple_storage_size) {
      EXPECT_EQ(current_tuple_idx, result.inserted_id);
      EXPECT_FALSE(result.ids_mutated);

      EXPECT_FALSE(tuple_store_->isEmpty());
      EXPECT_TRUE(tuple_store_->isPacked());
      EXPECT_EQ(current_tuple_idx, tuple_store_->getMaxTupleID());
      EXPECT_EQ(current_tuple_idx + 1, tuple_store_->numTuples());
      checkTupleValuesUntyped(current_tuple_idx, current_tuple_idx);

      storage_used += current_tuple_storage_bytes;
    } else {
      EXPECT_EQ(-1, result.inserted_id);
      EXPECT_FALSE(result.ids_mutated);

      EXPECT_FALSE(tuple_store_->isEmpty());
      EXPECT_TRUE(tuple_store_->isPacked());
      EXPECT_EQ(current_tuple_idx - 1, tuple_store_->getMaxTupleID());
      EXPECT_EQ(current_tuple_idx, tuple_store_->numTuples());

      break;
    }
    ++current_tuple_idx;
  }
}

TEST_P(SplitRowStoreTupleStorageSubBlockTest, InsertInBatchTest) {
  const std::size_t tuple_storage_size = getTupleStorageSize();
  const std::size_t tuple_slot_size = getTupleSlotSize();
  std::size_t storage_used = 0;
  int current_tuple_idx = 0;
  for (;;) {
    Tuple current_tuple(createSampleTuple(current_tuple_idx));
    const std::size_t current_tuple_storage_bytes
        = tuple_slot_size
          + (testVariableLength() ? (current_tuple.getAttributeValue(2).isNull() ?
                                     0 : current_tuple.getAttributeValue(2).getDataSize())
                                  : 0);
    if (storage_used + current_tuple_storage_bytes <= tuple_storage_size) {
      EXPECT_TRUE(tuple_store_->insertTupleInBatch(current_tuple));

      EXPECT_FALSE(tuple_store_->isEmpty());
      EXPECT_TRUE(tuple_store_->isPacked());
      EXPECT_EQ(current_tuple_idx, tuple_store_->getMaxTupleID());
      EXPECT_EQ(current_tuple_idx + 1, tuple_store_->numTuples());

      storage_used += current_tuple_storage_bytes;
    } else {
      EXPECT_FALSE(tuple_store_->insertTupleInBatch(current_tuple));

      EXPECT_FALSE(tuple_store_->isEmpty());
      EXPECT_TRUE(tuple_store_->isPacked());
      EXPECT_EQ(current_tuple_idx - 1, tuple_store_->getMaxTupleID());
      EXPECT_EQ(current_tuple_idx, tuple_store_->numTuples());

      break;
    }
    ++current_tuple_idx;
  }

  tuple_store_->rebuild();

  EXPECT_TRUE(tuple_store_->isPacked());
  EXPECT_EQ(current_tuple_idx - 1, tuple_store_->getMaxTupleID());
  EXPECT_EQ(current_tuple_idx, tuple_store_->numTuples());
  for (tuple_id check_tid = 0;
       check_tid <= tuple_store_->getMaxTupleID();
       ++check_tid) {
    checkTupleValuesUntyped(check_tid, check_tid);
  }
}

TEST_P(SplitRowStoreTupleStorageSubBlockTest, BulkInsertTest) {
  // Build up a ColumnVectorsValueAccessor to bulk-insert from. We'll reserve
  // enough space for the maximum possible number of tuples in the block, even
  // though we won't use all of it if testVariableLength() is true.
  const std::size_t max_tuple_capacity = getTupleStorageSize() / getTupleSlotSize();

  NativeColumnVector *int_vector = new NativeColumnVector(
    relation_->getAttributeById(0)->getType(),
    max_tuple_capacity);
  NativeColumnVector *double_vector = new NativeColumnVector(
    relation_->getAttributeById(1)->getType(),
    max_tuple_capacity);
  ColumnVector *string_vector = testVariableLength() ?
                                static_cast<ColumnVector*>(new IndirectColumnVector(
                                  relation_->getAttributeById(2)->getType(),
                                  max_tuple_capacity))
                                                     : static_cast<ColumnVector*>(new NativeColumnVector(
      relation_->getAttributeById(2)->getType(),
      max_tuple_capacity));

  std::size_t storage_used = 0;
  int current_tuple_idx = 0;
  for (;;) {
    Tuple current_tuple(createSampleTuple(current_tuple_idx));
    const std::size_t current_tuple_storage_bytes
      = getTupleSlotSize()
        + (testVariableLength() ? (current_tuple.getAttributeValue(2).isNull() ?
                                   0 : current_tuple.getAttributeValue(2).getDataSize())
                                : 0);
    if (storage_used + current_tuple_storage_bytes <= getTupleStorageSize()) {
      int_vector->appendTypedValue(current_tuple.getAttributeValue(0));
      double_vector->appendTypedValue(current_tuple.getAttributeValue(1));
      if (testVariableLength()) {
        static_cast<IndirectColumnVector*>(string_vector)
          ->appendTypedValue(current_tuple.getAttributeValue(2));
      } else {
        static_cast<NativeColumnVector*>(string_vector)
          ->appendTypedValue(current_tuple.getAttributeValue(2));
      }

      storage_used += current_tuple_storage_bytes;
      ++current_tuple_idx;
    } else {
      break;
    }
  }

  ColumnVectorsValueAccessor accessor;
  accessor.addColumn(int_vector);
  accessor.addColumn(double_vector);
  accessor.addColumn(string_vector);

  // Actually do the bulk-insert.
  accessor.beginIteration();
  tuple_id num_inserted = tuple_store_->bulkInsertTuples(&accessor);
  if (!testVariableLength()) {
    EXPECT_EQ(current_tuple_idx, num_inserted);
    ASSERT_TRUE(accessor.iterationFinished());
    // Shouldn't be able to insert any more tuples.
    accessor.beginIteration();
    tuple_id num_inserted_second_round = tuple_store_->bulkInsertTuples(&accessor);
    ASSERT_EQ(0, num_inserted_second_round);
  }

  tuple_store_->rebuild();
  EXPECT_EQ(num_inserted, tuple_store_->numTuples());
  EXPECT_EQ(num_inserted - 1, tuple_store_->getMaxTupleID());

  // Check the inserted values.
  ASSERT_TRUE(tuple_store_->isPacked());
  for (tuple_id tid = 0;
       tid <= tuple_store_->getMaxTupleID();
       ++tid) {
    checkTupleValuesUntyped(tid, tid);
  }
}

TEST_P(SplitRowStoreTupleStorageSubBlockTest, PartialBulkInsertTest) {
  // Build up a ColumnVectorsValueAccessor to bulk-insert from. We'll reserve
  // enough space for the maximum possible number of tuples in the block, even
  // though we won't use all of it if testVariableLength() is true.
  const std::size_t max_tuple_capacity = getTupleStorageSize() / getTupleSlotSize();

  NativeColumnVector *int_vector = new NativeColumnVector(
    relation_->getAttributeById(0)->getType(),
    max_tuple_capacity);
  NativeColumnVector *double_vector = new NativeColumnVector(
    relation_->getAttributeById(1)->getType(),
    max_tuple_capacity);
  ColumnVector *string_vector = testVariableLength() ?
                                static_cast<ColumnVector *>(new IndirectColumnVector(
                                  relation_->getAttributeById(2)->getType(),
                                  max_tuple_capacity))
                                                     : static_cast<ColumnVector *>(new NativeColumnVector(
      relation_->getAttributeById(2)->getType(),
      max_tuple_capacity));

  const int max_tuples_insert = 1000;
  for (int tuple_idx = 0; tuple_idx < max_tuples_insert; ++tuple_idx) {
    Tuple current_tuple(createSampleTuple(tuple_idx));
    int_vector->appendTypedValue(current_tuple.getAttributeValue(0));
    double_vector->appendTypedValue(current_tuple.getAttributeValue(1));
    if (testVariableLength()) {
      static_cast<IndirectColumnVector *>(string_vector)
        ->appendTypedValue(current_tuple.getAttributeValue(2));
    } else {
      static_cast<NativeColumnVector *>(string_vector)
        ->appendTypedValue(current_tuple.getAttributeValue(2));
    }
  }

  std::vector<attribute_id> attr_map_pt1 = {kInvalidCatalogId, 0, kInvalidCatalogId};
  std::vector<attribute_id> attr_map_pt2 = {0, kInvalidCatalogId, 1};

  ColumnVectorsValueAccessor accessor_pt1;
  accessor_pt1.addColumn(double_vector);

  ColumnVectorsValueAccessor accessor_pt2;
  accessor_pt2.addColumn(int_vector);
  accessor_pt2.addColumn(string_vector);


  // Actually do the bulk-insert.
  accessor_pt1.beginIteration();
  const tuple_id num_inserted_pt1 = tuple_store_->bulkInsertPartialTuples(attr_map_pt1, &accessor_pt1, kCatalogMaxID);
  ASSERT_GT(num_inserted_pt1, 0);
  const tuple_id num_inserted_pt2 = tuple_store_->bulkInsertPartialTuples(attr_map_pt2, &accessor_pt2,
                                                                          num_inserted_pt1);
  ASSERT_EQ(num_inserted_pt1, num_inserted_pt2);

  tuple_store_->bulkInsertPartialTuplesFinalize(num_inserted_pt1);
  ASSERT_EQ(max_tuples_insert, tuple_store_->getMaxTupleID() + 1);
  ASSERT_EQ(num_inserted_pt1, tuple_store_->getMaxTupleID() + 1);
  EXPECT_TRUE(accessor_pt2.iterationFinished());

  tuple_store_->rebuild();

  // Should be the same order as if we inserted them serially.
  ASSERT_TRUE(tuple_store_->isPacked());
  for (tuple_id tid = 0;
       tid <= tuple_store_->getMaxTupleID();
       ++tid) {
    checkTupleValuesUntyped(tid, tid);
  }
}

TEST_P(SplitRowStoreTupleStorageSubBlockTest, GetCopyGroupsForAttributeMapTest) {
  const bool nullable_attrs = testNullable();
  std::vector<attribute_id> relation_attrs = {
    SplitRowWrapper::AttrType::kInt,
    SplitRowWrapper::AttrType::kInt,
    SplitRowWrapper::AttrType::kInt,
    SplitRowWrapper::AttrType::kString,
    SplitRowWrapper::AttrType::kString,
    SplitRowWrapper::AttrType::kString};
  SplitRowWrapper dst_store(relation_attrs, nullable_attrs, testVariableLength());
  std::vector<attribute_id> attr_map = { kInvalidCatalogId, 0, 1, kInvalidCatalogId, 2, 1 };
  CopyGroupList copy_groups;
  dst_store->getCopyGroupsForAttributeMap(attr_map, &copy_groups);

  std::vector<ContiguousAttrs>& contiguous_attrs = copy_groups.contiguous_attrs_;
  std::vector<VarLenAttr>& varlen_attrs = copy_groups.varlen_attrs_;

  const std::size_t size_of_string = dst_store->getRelation().getAttributeById(3)->getType().maximumByteLength();

  // Fixed length attributes.
  EXPECT_EQ(0, contiguous_attrs[0].src_attr_id_);
  EXPECT_EQ(4, contiguous_attrs[0].bytes_to_advance_);
  EXPECT_EQ(4, contiguous_attrs[0].bytes_to_copy_);

  EXPECT_EQ(1, contiguous_attrs[1].src_attr_id_);
  EXPECT_EQ(4, contiguous_attrs[1].bytes_to_advance_);
  EXPECT_EQ(4, contiguous_attrs[1].bytes_to_copy_);

  if (testVariableLength()) {
    ASSERT_EQ(2, contiguous_attrs.size());
    ASSERT_EQ(2, varlen_attrs.size());

    EXPECT_EQ(2, varlen_attrs[0].src_attr_id_);
    EXPECT_EQ(sizeof(int) + SplitRowStoreTupleStorageSubBlock::kVarLenSlotSize, varlen_attrs[0].bytes_to_advance_);

    EXPECT_EQ(1, varlen_attrs[1].src_attr_id_);
    EXPECT_EQ(SplitRowStoreTupleStorageSubBlock::kVarLenSlotSize, varlen_attrs[1].bytes_to_advance_);

  } else {
    ASSERT_EQ(4, copy_groups.contiguous_attrs_.size());
    ASSERT_EQ(0, copy_groups.varlen_attrs_.size());

    EXPECT_EQ(2, contiguous_attrs[2].src_attr_id_);
    EXPECT_EQ(4 + size_of_string, contiguous_attrs[2].bytes_to_advance_);
    EXPECT_EQ(size_of_string, contiguous_attrs[2].bytes_to_copy_);
  }

  int null_count =  copy_groups.nullable_attrs_.size();
  if (testNullable()) {
    // The relation contains 6 nullable attributes, but only 3 are inserted.
    EXPECT_EQ(4, null_count);
  } else {
    EXPECT_EQ(0, null_count);
  }

  // test that merging works.
  copy_groups.merge_contiguous();
  EXPECT_EQ(0, contiguous_attrs[0].src_attr_id_);
  EXPECT_EQ(4, contiguous_attrs[0].bytes_to_advance_);

  if (testVariableLength()) {
    EXPECT_EQ(1, contiguous_attrs.size());
    EXPECT_EQ(sizeof(int) * 2 + SplitRowStoreTupleStorageSubBlock::kVarLenSlotSize,
              varlen_attrs[0].bytes_to_advance_);
  } else {
    EXPECT_EQ(3, contiguous_attrs.size());
    EXPECT_EQ(8, contiguous_attrs[0].bytes_to_copy_);
    EXPECT_EQ(8 + size_of_string, contiguous_attrs[1].bytes_to_advance_);
  }
}

TEST_P(SplitRowStoreTupleStorageSubBlockTest, BulkInsertWithRemappedAttributesTest) {
  // This is similar to the above test, but we will reverse the order of the
  // ColumnVectors in the ColumnVectorsValueAccessor and remap them back to the
  // correct order.

  // Build up a ColumnVectorsValueAccessor to bulk-insert from. We'll reserve
  // enough space for the maximum possible number of tuples in the block, even
  // though we won't use all of it if testVariableLength() is true.
  const std::size_t max_tuple_capacity = getTupleStorageSize() / getTupleSlotSize();

  NativeColumnVector *int_vector = new NativeColumnVector(
      relation_->getAttributeById(0)->getType(),
      max_tuple_capacity);
  NativeColumnVector *double_vector = new NativeColumnVector(
      relation_->getAttributeById(1)->getType(),
      max_tuple_capacity);
  ColumnVector *string_vector = testVariableLength() ?
      static_cast<ColumnVector*>(new IndirectColumnVector(
          relation_->getAttributeById(2)->getType(),
          max_tuple_capacity))
      : static_cast<ColumnVector*>(new NativeColumnVector(
          relation_->getAttributeById(2)->getType(),
          max_tuple_capacity));

  std::size_t storage_used = 0;
  int current_tuple_idx = 0;
  std::size_t tuple_max_size = relation_->getMaximumByteLength();
  std::size_t tuple_slot_size = getTupleSlotSize();
  for (;;) {
    Tuple current_tuple(createSampleTuple(current_tuple_idx));
    if ((getTupleStorageSize() - storage_used) / tuple_max_size > 0) {
      int_vector->appendTypedValue(current_tuple.getAttributeValue(0));
      double_vector->appendTypedValue(current_tuple.getAttributeValue(1));
      if (testVariableLength()) {
        static_cast<IndirectColumnVector*>(string_vector)
          ->appendTypedValue(current_tuple.getAttributeValue(2));
      } else {
        static_cast<NativeColumnVector*>(string_vector)
          ->appendTypedValue(current_tuple.getAttributeValue(2));
      }

      storage_used += tuple_slot_size;
      if (testVariableLength() && !current_tuple.getAttributeValue(2).isNull()) {
        storage_used += current_tuple.getAttributeValue(2).getDataSize();
      }

      ++current_tuple_idx;
    } else {
      break;
    }
  }

  ColumnVectorsValueAccessor accessor;
  accessor.addColumn(string_vector);
  accessor.addColumn(double_vector);
  accessor.addColumn(int_vector);

  std::vector<attribute_id> attribute_map;
  attribute_map.push_back(2);
  attribute_map.push_back(1);
  attribute_map.push_back(0);

  // Actually do the bulk-insert.
  accessor.beginIteration();
  tuple_id num_inserted = tuple_store_->bulkInsertTuplesWithRemappedAttributes(attribute_map, &accessor);
  if (!testVariableLength()) {
    EXPECT_EQ(current_tuple_idx, num_inserted);
    ASSERT_TRUE(accessor.iterationFinished());
    // Shouldn't be able to insert any more tuples.
    accessor.beginIteration();
    tuple_id num_inserted_second_round = tuple_store_->bulkInsertTuplesWithRemappedAttributes(attribute_map, &accessor);
    ASSERT_EQ(0, num_inserted_second_round);
  }

  tuple_store_->rebuild();
  EXPECT_EQ(num_inserted, tuple_store_->numTuples());
  EXPECT_EQ(num_inserted - 1, tuple_store_->getMaxTupleID());

  // Check the inserted values.
  ASSERT_TRUE(tuple_store_->isPacked());
  for (tuple_id tid = 0;
       tid <= tuple_store_->getMaxTupleID();
       ++tid) {
    checkTupleValuesUntyped(tid, tid);
  }
}

TEST_P(SplitRowStoreTupleStorageSubBlockTest, GetAttributeValueTest) {
  fillBlockWithSampleData();
  ASSERT_TRUE(tuple_store_->isPacked());

  for (tuple_id tid = 0;
       tid <= tuple_store_->getMaxTupleID();
       ++tid) {
    checkTupleValuesUntyped(tid, tid);
  }
}

TEST_P(SplitRowStoreTupleStorageSubBlockTest, GetAttributeValueTypedTest) {
  fillBlockWithSampleData();
  ASSERT_TRUE(tuple_store_->isPacked());

  for (tuple_id tid = 0;
       tid <= tuple_store_->getMaxTupleID();
       ++tid) {
    checkTupleValuesTyped(tid, tid);
  }
}

TEST_P(SplitRowStoreTupleStorageSubBlockTest, SplitRowToSplitRowTest) {
  // Test insertion of data from a SplitRow to a SplitRow with no reordering.
  fillBlockWithSampleData();
  std::vector<attribute_id> relation_attrs = {
    SplitRowWrapper::AttrType::kInt,
    SplitRowWrapper::AttrType::kDouble,
    SplitRowWrapper::AttrType::kDouble,
    SplitRowWrapper::AttrType::kInt,
    SplitRowWrapper::AttrType::kString,
    SplitRowWrapper::AttrType::kString,
    SplitRowWrapper::AttrType::kString};
  SplitRowWrapper dst_store(relation_attrs, testNullable(), testVariableLength());

  std::vector<attribute_id> attribute_map = {0, kInvalidCatalogId, 1, 0, 2, kInvalidCatalogId, 2};

  std::unique_ptr<ValueAccessor> accessor(tuple_store_->createValueAccessor());
  ASSERT_EQ(ValueAccessor::Implementation::kSplitRowStore,
            accessor->getImplementationType());
  ASSERT_FALSE(accessor->isTupleIdSequenceAdapter());

  SplitRowStoreValueAccessor &cast_accessor = static_cast<SplitRowStoreValueAccessor &>(*accessor);
  std::size_t num_inserted = dst_store->bulkInsertPartialTuples(attribute_map, &cast_accessor, kCatalogMaxID);
  attribute_map = {kInvalidCatalogId, 1, kInvalidCatalogId, kInvalidCatalogId, kInvalidCatalogId, 2, kInvalidCatalogId};
  cast_accessor.beginIteration();
  dst_store->bulkInsertPartialTuples(attribute_map, &cast_accessor, num_inserted);
  dst_store->bulkInsertPartialTuplesFinalize(num_inserted);

  EXPECT_EQ(num_inserted - 1, dst_store->getMaxTupleID());
  // The inserted relation should hold roughly 1/3 the tuples of the src. The more varlen
  // attributes, the fewer the relation will accept due to how it estimates.
  EXPECT_LT(0.15 * tuple_store_->getMaxTupleID(), dst_store->getMaxTupleID());
  EXPECT_GT(0.5 * tuple_store_->getMaxTupleID(), dst_store->getMaxTupleID());

  attribute_map = {0, 1, 4};
  for (tuple_id tid = 0; tid < dst_store->getMaxTupleID(); ++tid) {
    for (attribute_id aid = 0; aid < tuple_store_->getRelation().getMaxAttributeId(); ++aid) {
      const TypedValue &dst_value = dst_store->getAttributeValueTyped(tid, attribute_map[aid]);
      const TypedValue &src_value = tuple_store_->getAttributeValueTyped(tid, aid);
      if (src_value.isNull() || dst_value.isNull()) {
        EXPECT_TRUE(src_value.isNull() && dst_value.isNull());
      } else {
        EXPECT_TRUE(src_value.fastEqualCheck(dst_value));
      }
    }
  }
}

TEST_P(SplitRowStoreTupleStorageSubBlockTest, ValueAccessorTest) {
  fillBlockWithSampleData();

  // Delete a tuple so that we can check that iteration skips over holes.
  EXPECT_FALSE(tuple_store_->deleteTuple(42));

  std::unique_ptr<ValueAccessor> accessor(tuple_store_->createValueAccessor());
  ASSERT_EQ(ValueAccessor::Implementation::kSplitRowStore,
            accessor->getImplementationType());
  ASSERT_FALSE(accessor->isTupleIdSequenceAdapter());

  SplitRowStoreValueAccessor &cast_accessor
      = static_cast<SplitRowStoreValueAccessor&>(*accessor);

  cast_accessor.beginIteration();
  for (tuple_id expected_tid = 0;
       expected_tid <= tuple_store_->getMaxTupleID();
       expected_tid += (expected_tid == 41) ? 2 : 1) {
    EXPECT_FALSE(cast_accessor.iterationFinished());
    ASSERT_TRUE(cast_accessor.next());
    EXPECT_EQ(expected_tid, cast_accessor.getCurrentPosition());

    Tuple comp_tuple(createSampleTuple(expected_tid));

    TypedValue int_attr_value = cast_accessor.getTypedValue(0);
    if (comp_tuple.getAttributeValue(0).isNull()) {
      EXPECT_TRUE(int_attr_value.isNull());
    } else {
      EXPECT_EQ(comp_tuple.getAttributeValue(0).getLiteral<int>(),
                int_attr_value.getLiteral<int>());
    }

    TypedValue double_attr_value = cast_accessor.getTypedValue(1);
    if (comp_tuple.getAttributeValue(1).isNull()) {
      EXPECT_TRUE(double_attr_value.isNull());
    } else {
      EXPECT_EQ(comp_tuple.getAttributeValue(1).getLiteral<double>(),
                double_attr_value.getLiteral<double>());
    }

    TypedValue string_attr_value = cast_accessor.getTypedValue(2);
    if (comp_tuple.getAttributeValue(2).isNull()) {
      EXPECT_TRUE(string_attr_value.isNull());
    } else {
      EXPECT_STREQ(static_cast<const char*>(comp_tuple.getAttributeValue(2).getDataPtr()),
                   static_cast<const char*>(string_attr_value.getDataPtr()));
    }
  }

  EXPECT_TRUE(cast_accessor.iterationFinished());
  EXPECT_FALSE(cast_accessor.next());
}

TEST_P(SplitRowStoreTupleStorageSubBlockTest, SetAttributeValueTypedTest) {
  fillBlockWithSampleData();
  ASSERT_TRUE(tuple_store_->isPacked());

  // Alter every 16th tuple.
  for (tuple_id tid = 6;
       tid <= tuple_store_->getMaxTupleID();
       tid += 16) {
    Tuple mod_tuple(createSampleTuple(tid - 6));

    std::unordered_map<attribute_id, TypedValue> new_values;
    new_values.emplace(0, mod_tuple.getAttributeValue(0));
    new_values.emplace(1, mod_tuple.getAttributeValue(1));
    new_values.emplace(2, mod_tuple.getAttributeValue(2));
    ASSERT_TRUE(tuple_store_->canSetAttributeValuesInPlaceTyped(tid, new_values));

    tuple_store_->setAttributeValueInPlaceTyped(tid, 0, mod_tuple.getAttributeValue(0));
    tuple_store_->setAttributeValueInPlaceTyped(tid, 1, mod_tuple.getAttributeValue(1));
    tuple_store_->setAttributeValueInPlaceTyped(tid, 2, mod_tuple.getAttributeValue(2));
  }

  // Check all values.
  for (tuple_id tid = 0;
       tid <= tuple_store_->getMaxTupleID();
       ++tid) {
    if ((tid - 6) % 16 == 0) {
      checkTupleValuesUntyped(tid, tid - 6);
    } else {
      checkTupleValuesUntyped(tid, tid);
    }
  }

  if (testVariableLength()) {
    // It's also OK to replace a variable-length value with a shorter value, or
    // with null.
    std::unordered_map<attribute_id, TypedValue> variable_new_values;
    variable_new_values.emplace(2, VarCharType::InstanceNonNullable(kVarLenSize).makeValue("x", 2));
    ASSERT_TRUE(tuple_store_->canSetAttributeValuesInPlaceTyped(33, variable_new_values));
    tuple_store_->setAttributeValueInPlaceTyped(33, 2, variable_new_values[2]);
    EXPECT_STREQ("x", static_cast<const char*>(tuple_store_->getAttributeValue(33, 2)));

    if (testNullable()) {
      variable_new_values[2] = TypedValue(kVarChar);
      ASSERT_TRUE(tuple_store_->canSetAttributeValuesInPlaceTyped(33, variable_new_values));
      tuple_store_->setAttributeValueInPlaceTyped(33, 2, variable_new_values[2]);
      EXPECT_EQ(nullptr, tuple_store_->getAttributeValue(33, 2));
    }

    // Reset with empty block to test updating with a longer variable-length
    // value when there is variable-length storage available.
    tuple_store_.reset(nullptr);
    tuple_store_memory_.reset(kSubBlockSize);
    std::memset(tuple_store_memory_.get(), 0x0, kSubBlockSize);
    tuple_store_.reset(new SplitRowStoreTupleStorageSubBlock(*relation_,
                                                             *tuple_store_description_,
                                                             true,
                                                             tuple_store_memory_.get(),
                                                             kSubBlockSize));

    EXPECT_TRUE(tuple_store_->insertTupleInBatch(createSampleTuple(0)));
    tuple_store_->rebuild();

    variable_new_values[2] = VarCharType::InstanceNonNullable(kVarLenSize).makeValue("hello world", 12);
    ASSERT_TRUE(tuple_store_->canSetAttributeValuesInPlaceTyped(0, variable_new_values));
    tuple_store_->setAttributeValueInPlaceTyped(0, 2, variable_new_values[2]);
    EXPECT_STREQ("hello world", static_cast<const char*>(tuple_store_->getAttributeValue(0, 2)));
  }
}


TEST_P(SplitRowStoreTupleStorageSubBlockTest, DeleteAndRebuildTest) {
  fillBlockWithSampleData();
  ASSERT_TRUE(tuple_store_->isPacked());

  const tuple_id original_num_tuples = tuple_store_->numTuples();
  const tuple_id original_max_tid = tuple_store_->getMaxTupleID();

  // Delete the first tuple.
  EXPECT_FALSE(tuple_store_->deleteTuple(0));

  // Delete a sequence of tuples.
  TupleIdSequence delete_sequence(tuple_store_->getMaxTupleID() + 1);
  for (tuple_id tid = 64;
       tid <= tuple_store_->getMaxTupleID();
       tid += 64) {
    delete_sequence.set(tid, true);
  }
  EXPECT_FALSE(tuple_store_->bulkDeleteTuples(&delete_sequence));

  EXPECT_EQ(static_cast<tuple_id>(original_num_tuples - 1 - delete_sequence.numTuples()),
            tuple_store_->numTuples());
  EXPECT_EQ(original_max_tid, tuple_store_->getMaxTupleID());

  for (tuple_id tid = 0;
       tid <= tuple_store_->getMaxTupleID();
       ++tid) {
    if (tid % 64 == 0) {
      EXPECT_FALSE(tuple_store_->hasTupleWithID(tid));
    } else {
      ASSERT_TRUE(tuple_store_->hasTupleWithID(tid));
      checkTupleValuesUntyped(tid, tid);
    }
  }

  TupleStorageSubBlock::InsertResult result(-1, false);
  tuple_id tuples_reinserted = 0;

  // If we're testing variable-length attributes, but NOT nulls, then there's
  // no way to make a new tuple that doesn't use up at least 1 more byte of
  // variable-length storage.
  if (GetParam() != AttributeTypeFeatures::kVariableLength) {
    // After deleting, we can insert tuples in the "holes" left behind, as long
    // as variable-length storage doesn't run into tuple slots.
    std::vector<TypedValue> reinsert_attr_values;
    reinsert_attr_values.emplace_back(-42);
    reinsert_attr_values.emplace_back(static_cast<double>(-0.125));
    // Use a NULL string so that we won't require any more variable-length
    // storage.
    if (testNullable()) {
      reinsert_attr_values.emplace_back(testVariableLength() ? kVarChar : kChar);
    } else {
      reinsert_attr_values.emplace_back(
          CharType::InstanceNonNullable(kVarLenSize).makeValue("foo", 4));
      reinsert_attr_values.back().ensureNotReference();
    }
    Tuple reinsert_tuple(std::move(reinsert_attr_values));

    result = tuple_store_->insertTuple(reinsert_tuple);
    EXPECT_EQ(0, result.inserted_id);
    EXPECT_FALSE(result.ids_mutated);
    ++tuples_reinserted;

    // Insert again, going to the next available position.
    result = tuple_store_->insertTuple(reinsert_tuple);
    EXPECT_EQ(64, result.inserted_id);
    EXPECT_FALSE(result.ids_mutated);
    ++tuples_reinserted;
  }

  Tuple extra_variable_tuple((std::vector<TypedValue>()));
  const char kExtraVarCharValue[] = "abcdefghijklmnopqrstuvwxyz";
  if (testVariableLength()) {
    // Try to insert a tuple that uses more than the available variable-length
    // storage.
    std::vector<TypedValue> extra_variable_attr_values;
    extra_variable_attr_values.emplace_back(-123);
    extra_variable_attr_values.emplace_back(static_cast<double>(-100.5));
    extra_variable_attr_values.emplace_back((VarCharType::InstanceNonNullable(kVarLenSize).makeValue(
        kExtraVarCharValue,
        27)));
    extra_variable_tuple = Tuple(std::move(extra_variable_attr_values));

    result = tuple_store_->insertTuple(extra_variable_tuple);
    EXPECT_EQ(-1, result.inserted_id);
    EXPECT_FALSE(result.ids_mutated);
  }

  // Rebuild the block.
  tuple_store_->rebuild();

  if (testVariableLength()) {
    // Storage has now been compacted, so insert should be able to succeed.
    result = tuple_store_->insertTuple(extra_variable_tuple);
    EXPECT_EQ(static_cast<tuple_id>(
                  original_num_tuples + tuples_reinserted - delete_sequence.numTuples() - 1),
              result.inserted_id);
    EXPECT_FALSE(result.ids_mutated);
    ++tuples_reinserted;
  }

  // Check info post-rebuild.
  EXPECT_TRUE(tuple_store_->isPacked());
  EXPECT_EQ(static_cast<tuple_id>(
                original_num_tuples + tuples_reinserted - delete_sequence.numTuples() - 2),
            tuple_store_->getMaxTupleID());
  EXPECT_EQ(static_cast<tuple_id>(
                original_num_tuples + tuples_reinserted - delete_sequence.numTuples() - 1),
            tuple_store_->numTuples());

  // Check values in rebuilt block. The compaction algorithm works by scanning
  // for "holes" in the slot array, and filling them by copying filled slots
  // from the back of the array.
  tuple_id back_copied_tid = original_max_tid;
  for (tuple_id tid = 0;
       tid < tuple_store_->getMaxTupleID();  // We will check the max tuple below.
       ++tid) {
    if ((GetParam() != AttributeTypeFeatures::kVariableLength) && ((tid == 0) || (tid == 64))) {
      ASSERT_NE(nullptr, tuple_store_->getAttributeValue(tid, 0));
      EXPECT_EQ(-42, *static_cast<const int*>(tuple_store_->getAttributeValue(tid, 0)));

      ASSERT_NE(nullptr, tuple_store_->getAttributeValue(tid, 1));
      EXPECT_EQ(static_cast<double>(-0.125),
                *static_cast<const double*>(tuple_store_->getAttributeValue(tid, 1)));

      if (testNullable()) {
        EXPECT_EQ(nullptr, tuple_store_->getAttributeValue(tid, 2));
      } else {
        EXPECT_STREQ("foo", static_cast<const char*>(tuple_store_->getAttributeValue(tid, 2)));
      }
    } else if (tid % 64 == 0) {
      checkTupleValuesUntyped(tid, back_copied_tid);
      --back_copied_tid;
      if (back_copied_tid % 64 == 0) {
        --back_copied_tid;
      }
    } else {
      checkTupleValuesUntyped(tid, tid);
    }
  }

  // Check the tuple we inserted after rebuilding.
  if (testVariableLength()) {
    ASSERT_NE(nullptr, tuple_store_->getAttributeValue(result.inserted_id, 0));
    EXPECT_EQ(-123,
              *static_cast<const int*>(tuple_store_->getAttributeValue(result.inserted_id, 0)));

    ASSERT_NE(nullptr, tuple_store_->getAttributeValue(result.inserted_id, 1));
    EXPECT_EQ(static_cast<double>(-100.5),
              *static_cast<const double*>(tuple_store_->getAttributeValue(result.inserted_id, 1)));

    ASSERT_NE(nullptr, tuple_store_->getAttributeValue(result.inserted_id, 2));
    EXPECT_STREQ(kExtraVarCharValue,
                 static_cast<const char*>(tuple_store_->getAttributeValue(result.inserted_id, 2)));
  }
}

TEST(SplitRowStoreTupleStorageSubBlockNullTypeTest, NullTypeTest) {
  // Set up a relation with a single NullType attribute.
  CatalogRelation test_relation(nullptr, "TestRelation");
  CatalogAttribute *nulltype_attr = new CatalogAttribute(&test_relation,
                                                         "nulltype_attr",
                                                         TypeFactory::GetType(kNullType, true));
  ASSERT_EQ(0, test_relation.addAttribute(nulltype_attr));

  // Set up a minimal StorageBlockLayoutDescription.
  StorageBlockLayoutDescription layout_desc;
  layout_desc.set_num_slots(1);
  layout_desc.mutable_tuple_store_description()->set_sub_block_type(
      TupleStorageSubBlockDescription::SPLIT_ROW_STORE);

  // Check that the description is considered valid.
  EXPECT_TRUE(StorageBlockLayout::DescriptionIsValid(test_relation, layout_desc));

  StorageBlockLayout layout(test_relation, layout_desc);

  // Construct an actual SplitRowStoreTupleStorageSubBlock.
  ScopedBuffer tuple_store_memory(kSlotSizeBytes);
  SplitRowStoreTupleStorageSubBlock tuple_store(test_relation,
                                                layout_desc.tuple_store_description(),
                                                true,
                                                tuple_store_memory.get(),
                                                kSlotSizeBytes);

  // Insert some NullType values.
  std::vector<TypedValue> attr_values;
  attr_values.emplace_back(kNullType);
  Tuple tuple(std::move(attr_values));

  for (tuple_id tid = 0; tid < 100; ++tid) {
    tuple_store.insertTuple(tuple);
  }

  EXPECT_EQ(100, tuple_store.numTuples());

  // Delete some values.
  TupleIdSequence delete_sequence(100);
  delete_sequence.set(5, true);
  delete_sequence.set(25, true);
  delete_sequence.set(45, true);
  delete_sequence.set(65, true);
  delete_sequence.set(85, true);

  EXPECT_FALSE(tuple_store.bulkDeleteTuples(&delete_sequence));
  EXPECT_EQ(95, tuple_store.numTuples());
  ASSERT_EQ(99, tuple_store.getMaxTupleID());

  // Read out values.
  for (tuple_id tid = 0; tid < 100; ++tid) {
    if (QUICKSTEP_EQUALS_ANY_CONSTANT(tid, 5, 25, 45, 65, 85)) {
      EXPECT_FALSE(tuple_store.hasTupleWithID(tid));
    } else {
      ASSERT_TRUE(tuple_store.hasTupleWithID(tid));
      EXPECT_EQ(nullptr, tuple_store.getAttributeValue(tid, 0));

      TypedValue value = tuple_store.getAttributeValueTyped(tid, 0);
      EXPECT_TRUE(value.isNull());
      EXPECT_EQ(kNullType, value.getTypeID());
    }
  }
}

// Note: INSTANTIATE_TEST_CASE_P has variadic arguments part. If the variable argument part
//       is empty, C++11 standard says it should produce a warning. A warning is converted
//       to an error since we use -Werror as a compiler parameter. It causes Travis to build.
//       This is the reason that we must give an empty string argument as a last parameter
//       to supress warning that clang gives.
INSTANTIATE_TEST_CASE_P(
    AttributeTypeFeatures,
    SplitRowStoreTupleStorageSubBlockTest,
    ::testing::Values(
        AttributeTypeFeatures::kNone,
        AttributeTypeFeatures::kNullable,
        AttributeTypeFeatures::kVariableLength,
        AttributeTypeFeatures::
            kNullableAndVariableLength),);  // NOLINT(whitespace/comma)

}  // namespace quickstep
