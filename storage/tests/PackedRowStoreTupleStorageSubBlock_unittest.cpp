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

#include <cstring>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelation.hpp"
#include "storage/PackedRowStoreTupleStorageSubBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageConstants.hpp"
#include "storage/StorageErrors.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "types/CharType.hpp"
#include "types/DoubleType.hpp"
#include "types/IntType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypedValue.hpp"
#include "types/TypeID.hpp"
#include "types/containers/Tuple.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/BitVector.hpp"
#include "utility/ScopedBuffer.hpp"

using std::make_pair;
using std::ostringstream;
using std::pair;

namespace quickstep {

class PackedRowStoreTupleStorageSubBlockTest : public ::testing::TestWithParam<bool> {
 protected:
  static const size_t kSubBlockSize = 0x100000;  // 1 MB
  static const size_t kTupleLength = 24;

  virtual void SetUp() {
    // Create a sample relation with a variety of attribute types.
    relation_.reset(new CatalogRelation(nullptr, "TestRelation"));

    // An integer.
    CatalogAttribute *current_attr = new CatalogAttribute(relation_.get(),
                                                          "int_attr",
                                                          TypeFactory::GetType(kInt, GetParam()));
    ASSERT_EQ(0, relation_->addAttribute(current_attr));
    eq_comp_int_.reset(ComparisonFactory::GetComparison(ComparisonID::kEqual).makeUncheckedComparatorForTypes(
        current_attr->getType(),
        current_attr->getType()));

    // A double.
    current_attr = new CatalogAttribute(relation_.get(),
                                        "double_attr",
                                        TypeFactory::GetType(kDouble, GetParam()));
    ASSERT_EQ(1, relation_->addAttribute(current_attr));
    eq_comp_double_.reset(ComparisonFactory::GetComparison(ComparisonID::kEqual).makeUncheckedComparatorForTypes(
        current_attr->getType(),
        current_attr->getType()));

    // A string.
    current_attr = new CatalogAttribute(relation_.get(),
                                        "char_attr",
                                        TypeFactory::GetType(kChar, 12, GetParam()));
    ASSERT_EQ(2, relation_->addAttribute(current_attr));
    eq_comp_char_.reset(ComparisonFactory::GetComparison(ComparisonID::kEqual).makeUncheckedComparatorForTypes(
        current_attr->getType(),
        current_attr->getType()));

    tuple_store_description_.reset(new TupleStorageSubBlockDescription());
    tuple_store_description_->set_sub_block_type(TupleStorageSubBlockDescription::PACKED_ROW_STORE);

    // Don't initialize the block yet. Different tests will use different
    // params.
    tuple_store_memory_.reset();
    tuple_store_.reset();
  }

  void initializeNewBlock(const size_t block_size) {
    tuple_store_memory_.reset(block_size);
    tuple_store_.reset(new PackedRowStoreTupleStorageSubBlock(*relation_,
                                                              *tuple_store_description_,
                                                              true,
                                                              tuple_store_memory_.get(),
                                                              block_size));
  }

  int computeRowCapacity() {
    if (GetParam()) {
      // Estimate using the same heuristic as
      // PackedRowStoreTupleStorageSubBlock's constructor.
      int row_capacity = ((kSubBlockSize - sizeof(tuple_id)) << 3)
                         / ((kTupleLength << 3) + relation_->numNullableAttributes());
      const size_t null_bitmap_bytes_ = BitVector<false>::BytesNeeded(
          row_capacity * relation_->numNullableAttributes());
      return static_cast<int>(kSubBlockSize - sizeof(tuple_id) - null_bitmap_bytes_) / kTupleLength;
    } else {
      return static_cast<int>(kSubBlockSize - sizeof(tuple_id)) / kTupleLength;
    }
  }

  // Caller takes ownership of new heap-created Tuple.
  Tuple* createSampleTuple(const int base_value) const {
    std::vector<TypedValue> attrs;

    // int_attr
    if (GetParam() && (base_value % 6 == 0)) {
      // Throw in a NULL integer for every sixth value.
      attrs.emplace_back(kInt);
    } else {
      attrs.emplace_back(base_value);
    }

    // double_attr
    if (GetParam() && (base_value % 6 == 2)) {
      // NULL very sixth value.
      attrs.emplace_back(kDouble);
    } else {
      attrs.emplace_back(static_cast<double>(0.25 * base_value));
    }

    // char_attr
    if (GetParam() && (base_value % 6 == 4)) {
      // NULL very sixth value.
      attrs.emplace_back(CharType::InstanceNullable(12).makeNullValue());
    } else {
      ostringstream char_buffer;
      char_buffer << base_value;
      std::string string_literal(char_buffer.str());
      attrs.emplace_back(CharType::InstanceNonNullable(12).makeValue(string_literal.c_str(),
                                                                     string_literal.size() + 1));
      attrs.back().ensureNotReference();
    }

    return new Tuple(std::move(attrs));
  }

  void fillBlockWithSampleData() {
    tuple_id current_tid = 0;
    std::unique_ptr<Tuple> current_tuple(createSampleTuple(current_tid));
    while (tuple_store_->insertTupleInBatch(*current_tuple)) {
      ++current_tid;
      current_tuple.reset(createSampleTuple(current_tid));
    }

    tuple_store_->rebuild();
  }

  void checkTupleValuesUntyped(const tuple_id tid,
                               const int base_value) {
    ASSERT_TRUE(tuple_store_->hasTupleWithID(tid));
    ASSERT_TRUE(tuple_store_->supportsUntypedGetAttributeValue(0));
    ASSERT_TRUE(tuple_store_->supportsUntypedGetAttributeValue(1));
    ASSERT_TRUE(tuple_store_->supportsUntypedGetAttributeValue(2));

    std::unique_ptr<Tuple> comp_tuple(createSampleTuple(base_value));

    if (comp_tuple->getAttributeValue(0).isNull()) {
      assert(nullptr == tuple_store_->getAttributeValue(tid, 0));
      // NULL comparisons are always false (for now).
      EXPECT_FALSE(eq_comp_int_->compareDataPtrs(nullptr,
                                                 tuple_store_->getAttributeValue(tid, 0)));
    } else {
      assert(eq_comp_int_->compareDataPtrs(comp_tuple->getAttributeValue(0).getDataPtr(),
                                           tuple_store_->getAttributeValue(tid, 0)));
    }

    if (comp_tuple->getAttributeValue(1).isNull()) {
      EXPECT_EQ(nullptr, tuple_store_->getAttributeValue(tid, 1));
      // NULL comparisons are always false (for now).
      EXPECT_FALSE(eq_comp_double_->compareDataPtrs(nullptr,
                                                    tuple_store_->getAttributeValue(tid, 1)));
    } else {
      EXPECT_TRUE(eq_comp_double_->compareDataPtrs(comp_tuple->getAttributeValue(1).getDataPtr(),
                                                   tuple_store_->getAttributeValue(tid, 1)));
    }

    if (comp_tuple->getAttributeValue(2).isNull()) {
      EXPECT_EQ(nullptr, tuple_store_->getAttributeValue(tid, 2));
      // NULL comparisons are always false (for now).
      EXPECT_FALSE(eq_comp_char_->compareDataPtrs(nullptr,
                                                  tuple_store_->getAttributeValue(tid, 2)));
    } else {
      EXPECT_TRUE(eq_comp_char_->compareDataPtrs(comp_tuple->getAttributeValue(2).getDataPtr(),
                                                 tuple_store_->getAttributeValue(tid, 2)));
    }
  }

  void checkTupleValuesTyped(const tuple_id tid,
                             const int base_value) {
    ASSERT_TRUE(tuple_store_->hasTupleWithID(tid));

    std::unique_ptr<Tuple> comp_tuple(createSampleTuple(base_value));

    if (comp_tuple->getAttributeValue(0).isNull()) {
      EXPECT_TRUE(tuple_store_->getAttributeValueTyped(tid, 0).isNull());
      // NULL comparisons are always false (for now).
      EXPECT_FALSE(eq_comp_int_->compareTypedValues(comp_tuple->getAttributeValue(0),
                                                    tuple_store_->getAttributeValueTyped(tid, 0)));
    } else {
      EXPECT_TRUE(eq_comp_int_->compareTypedValues(comp_tuple->getAttributeValue(0),
                                                   tuple_store_->getAttributeValueTyped(tid, 0)));
    }

    if (comp_tuple->getAttributeValue(1).isNull()) {
      EXPECT_TRUE(tuple_store_->getAttributeValueTyped(tid, 1).isNull());
      // NULL comparisons are always false (for now).
      EXPECT_FALSE(eq_comp_double_->compareTypedValues(comp_tuple->getAttributeValue(1),
                                                       tuple_store_->getAttributeValueTyped(tid, 1)));
    } else {
      EXPECT_TRUE(eq_comp_double_->compareTypedValues(comp_tuple->getAttributeValue(1),
                                                      tuple_store_->getAttributeValueTyped(tid, 1)));
    }

    if (comp_tuple->getAttributeValue(2).isNull()) {
      EXPECT_TRUE(tuple_store_->getAttributeValueTyped(tid, 2).isNull());
      // NULL comparisons are always false (for now).
      EXPECT_FALSE(eq_comp_char_->compareTypedValues(comp_tuple->getAttributeValue(2),
                                                     tuple_store_->getAttributeValueTyped(tid, 2)));
    } else {
      EXPECT_TRUE(eq_comp_char_->compareTypedValues(comp_tuple->getAttributeValue(2),
                                                    tuple_store_->getAttributeValueTyped(tid, 2)));
    }
  }

  std::unique_ptr<CatalogRelation> relation_;
  ScopedBuffer tuple_store_memory_;
  std::unique_ptr<TupleStorageSubBlockDescription> tuple_store_description_;
  std::unique_ptr<PackedRowStoreTupleStorageSubBlock> tuple_store_;

  std::unique_ptr<UncheckedComparator> eq_comp_int_;
  std::unique_ptr<UncheckedComparator> eq_comp_double_;
  std::unique_ptr<UncheckedComparator> eq_comp_char_;
};

typedef PackedRowStoreTupleStorageSubBlockTest PackedRowStoreTupleStorageSubBlockDeathTest;

TEST_P(PackedRowStoreTupleStorageSubBlockTest, DescriptionIsValidTest) {
  // The descriptions we use for the other tests should be valid.
  EXPECT_TRUE(PackedRowStoreTupleStorageSubBlock::DescriptionIsValid(*relation_,
                                                                     *tuple_store_description_));

  // An uninitialized description is not valid.
  tuple_store_description_.reset(new TupleStorageSubBlockDescription());
  EXPECT_FALSE(PackedRowStoreTupleStorageSubBlock::DescriptionIsValid(*relation_,
                                                                      *tuple_store_description_));

  // A description that specifies the wrong sub_block_type is not valid.
  tuple_store_description_->set_sub_block_type(TupleStorageSubBlockDescription::BASIC_COLUMN_STORE);
  EXPECT_FALSE(PackedRowStoreTupleStorageSubBlock::DescriptionIsValid(*relation_,
                                                                      *tuple_store_description_));

  // A relation with a nullable attribute is OK.
  std::unique_ptr<CatalogRelation> nullable_relation(new CatalogRelation(NULL, "nullable_relation"));
  CatalogAttribute *nullable_attribute = new CatalogAttribute(nullable_relation.get(),
                                                              "nullable_attr",
                                                              TypeFactory::GetType(kInt, true));
  ASSERT_EQ(0, nullable_relation->addAttribute(nullable_attribute));
  tuple_store_description_.reset(new TupleStorageSubBlockDescription());
  tuple_store_description_->set_sub_block_type(TupleStorageSubBlockDescription::PACKED_ROW_STORE);
  EXPECT_TRUE(PackedRowStoreTupleStorageSubBlock::DescriptionIsValid(*nullable_relation,
                                                                     *tuple_store_description_));

  // A relation with a variable-length attribute can't be used with this block type.
  std::unique_ptr<CatalogRelation> variable_length_relation(new CatalogRelation(NULL, "variable_length_relation"));
  CatalogAttribute *variable_length_attribute = new CatalogAttribute(variable_length_relation.get(),
                                                                    "variable_length_attr",
                                                                     TypeFactory::GetType(kVarChar, 20, false));
  ASSERT_EQ(0, variable_length_relation->addAttribute(variable_length_attribute));
  EXPECT_FALSE(PackedRowStoreTupleStorageSubBlock::DescriptionIsValid(*variable_length_relation,
                                                                      *tuple_store_description_));
}

TEST_P(PackedRowStoreTupleStorageSubBlockDeathTest, ConstructWithInvalidDescriptionTest) {
  tuple_store_description_.reset(new TupleStorageSubBlockDescription());
  tuple_store_description_->set_sub_block_type(TupleStorageSubBlockDescription::BASIC_COLUMN_STORE);
  EXPECT_DEATH(initializeNewBlock(kSubBlockSize), "");
}

TEST_P(PackedRowStoreTupleStorageSubBlockTest, MemoryTooSmallTest) {
  // 1 byte short.
  EXPECT_THROW(initializeNewBlock(sizeof(tuple_id) - 1),
               BlockMemoryTooSmall);
}

TEST_P(PackedRowStoreTupleStorageSubBlockTest, InsertTest) {
  initializeNewBlock(kSubBlockSize);

  int row_capacity = computeRowCapacity();

  EXPECT_TRUE(tuple_store_->supportsAdHocInsert());
  EXPECT_TRUE(tuple_store_->adHocInsertIsEfficient());
  EXPECT_FALSE(tuple_store_->isCompressed());
  EXPECT_TRUE(tuple_store_->isEmpty());

  std::unique_ptr<Tuple> current_tuple;
  for (int tuple_num = 0;
       tuple_num < row_capacity;
       ++tuple_num) {
    current_tuple.reset(createSampleTuple(tuple_num));
    TupleStorageSubBlock::InsertResult result = tuple_store_->insertTuple(*current_tuple);
    ASSERT_EQ(tuple_num, result.inserted_id);
    ASSERT_FALSE(result.ids_mutated);

    EXPECT_FALSE(tuple_store_->isEmpty());
    EXPECT_TRUE(tuple_store_->isPacked());
    EXPECT_EQ(tuple_num, tuple_store_->getMaxTupleID());
    EXPECT_EQ(tuple_num + 1, tuple_store_->numTuples());
  }

  current_tuple.reset(createSampleTuple(0));
  TupleStorageSubBlock::InsertResult result = tuple_store_->insertTuple(*current_tuple);
  EXPECT_EQ(-1, result.inserted_id);
  EXPECT_FALSE(result.ids_mutated);

  EXPECT_TRUE(tuple_store_->isPacked());
  EXPECT_EQ(row_capacity - 1, tuple_store_->getMaxTupleID());
  EXPECT_EQ(row_capacity, tuple_store_->numTuples());
}

TEST_P(PackedRowStoreTupleStorageSubBlockTest, InsertInBatchTest) {
  initializeNewBlock(kSubBlockSize);

  int row_capacity = computeRowCapacity();

  EXPECT_TRUE(tuple_store_->supportsAdHocInsert());
  EXPECT_TRUE(tuple_store_->adHocInsertIsEfficient());
  EXPECT_FALSE(tuple_store_->isCompressed());
  EXPECT_TRUE(tuple_store_->isEmpty());

  std::unique_ptr<Tuple> current_tuple;
  for (int tuple_num = 0;
       tuple_num < row_capacity;
       ++tuple_num) {
    current_tuple.reset(createSampleTuple(tuple_num));
    EXPECT_TRUE(tuple_store_->insertTupleInBatch(*current_tuple));

    EXPECT_FALSE(tuple_store_->isEmpty());
    EXPECT_TRUE(tuple_store_->isPacked());
    EXPECT_EQ(tuple_num, tuple_store_->getMaxTupleID());
    EXPECT_EQ(tuple_num + 1, tuple_store_->numTuples());
  }

  current_tuple.reset(createSampleTuple(0));
  EXPECT_FALSE(tuple_store_->insertTupleInBatch(*current_tuple));

  EXPECT_TRUE(tuple_store_->isPacked());
  EXPECT_EQ(row_capacity - 1, tuple_store_->getMaxTupleID());
  EXPECT_EQ(row_capacity, tuple_store_->numTuples());
}

TEST_P(PackedRowStoreTupleStorageSubBlockTest, GetAttributeValueTest) {
  initializeNewBlock(kSubBlockSize);
  fillBlockWithSampleData();
  ASSERT_TRUE(tuple_store_->isPacked());

  for (tuple_id tid = 0;
       tid <= tuple_store_->getMaxTupleID();
       ++tid) {
    checkTupleValuesUntyped(tid, tid);
  }
}

TEST_P(PackedRowStoreTupleStorageSubBlockTest, GetAttributeValueTypedTest) {
  initializeNewBlock(kSubBlockSize);
  fillBlockWithSampleData();
  ASSERT_TRUE(tuple_store_->isPacked());

  for (tuple_id tid = 0;
       tid <= tuple_store_->getMaxTupleID();
       ++tid) {
    checkTupleValuesTyped(tid, tid);
  }
}

TEST_P(PackedRowStoreTupleStorageSubBlockTest, SetAttributeValueTypedTest) {
  initializeNewBlock(kSubBlockSize);
  fillBlockWithSampleData();
  ASSERT_TRUE(tuple_store_->isPacked());

  // Alter every 16th tuple.
  for (tuple_id tid = 0;
       tid <= tuple_store_->getMaxTupleID();
       tid += 16) {
    std::unique_ptr<Tuple> mod_tuple(createSampleTuple(-tid));

    std::unordered_map<attribute_id, TypedValue> new_values;
    new_values.insert(make_pair(0, mod_tuple->getAttributeValue(0)));
    new_values.insert(make_pair(1, mod_tuple->getAttributeValue(1)));
    new_values.insert(make_pair(2, mod_tuple->getAttributeValue(2)));
    ASSERT_TRUE(tuple_store_->canSetAttributeValuesInPlaceTyped(tid, new_values));

    tuple_store_->setAttributeValueInPlaceTyped(tid, 0, mod_tuple->getAttributeValue(0));
    tuple_store_->setAttributeValueInPlaceTyped(tid, 1, mod_tuple->getAttributeValue(1));
    tuple_store_->setAttributeValueInPlaceTyped(tid, 2, mod_tuple->getAttributeValue(2));
  }

  // Check all values.
  for (tuple_id tid = 0;
       tid <= tuple_store_->getMaxTupleID();
       ++tid) {
    if (tid & 0xF) {
      checkTupleValuesTyped(tid, tid);
    } else {
      checkTupleValuesTyped(tid, -tid);
    }
  }
}

TEST_P(PackedRowStoreTupleStorageSubBlockTest, DeleteTest) {
  initializeNewBlock(kSubBlockSize);
  fillBlockWithSampleData();
  ASSERT_TRUE(tuple_store_->isPacked());

  tuple_id original_num_tuples = tuple_store_->numTuples();

  // Delete the last tuple.
  EXPECT_FALSE(tuple_store_->deleteTuple(original_num_tuples - 1));

  // Delete the first tuple.
  EXPECT_TRUE(tuple_store_->deleteTuple(0));

  // Delete a sequence of tuples.
  TupleIdSequence delete_sequence(tuple_store_->getMaxTupleID() + 1);
  for (tuple_id tid = 63;
       tid <= tuple_store_->getMaxTupleID();
       tid += 64) {
    delete_sequence.set(tid, true);
  }
  EXPECT_TRUE(tuple_store_->bulkDeleteTuples(&delete_sequence));

  EXPECT_EQ(static_cast<tuple_id>(original_num_tuples - 2 - delete_sequence.numTuples()),
            tuple_store_->numTuples());

  tuple_id remaining_tid = 0;
  for (tuple_id original_tid = 0;
       original_tid < (original_num_tuples - 1);
       ++original_tid) {
    if (original_tid & 0x3F) {
      checkTupleValuesUntyped(remaining_tid, original_tid);
      ++remaining_tid;
    }
  }
}

TEST(PackedRowStoreTupleStorageSubBlockNullTypeTest, NullTypeTest) {
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
      TupleStorageSubBlockDescription::PACKED_ROW_STORE);

  // Check that the description is considered valid.
  EXPECT_TRUE(StorageBlockLayout::DescriptionIsValid(test_relation, layout_desc));

  StorageBlockLayout layout(test_relation, layout_desc);

  // Construct an actual PackedRowStoreTupleStorageSubBlock.
  ScopedBuffer tuple_store_memory(kSlotSizeBytes);
  PackedRowStoreTupleStorageSubBlock tuple_store(test_relation,
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

  EXPECT_TRUE(tuple_store.bulkDeleteTuples(&delete_sequence));
  EXPECT_EQ(95, tuple_store.numTuples());
  ASSERT_EQ(94, tuple_store.getMaxTupleID());

  // Read out values.
  for (tuple_id tid = 0; tid < 95; ++tid) {
    ASSERT_TRUE(tuple_store.hasTupleWithID(tid));
    EXPECT_EQ(nullptr, tuple_store.getAttributeValue(tid, 0));

    TypedValue value = tuple_store.getAttributeValueTyped(tid, 0);
    EXPECT_TRUE(value.isNull());
    EXPECT_EQ(kNullType, value.getTypeID());
  }
}

// Note: INSTANTIATE_TEST_CASE_P has variadic arguments part. If the variable argument part
//       is empty, C++11 standard says it should produce a warning. A warning is converted
//       to an error since we use -Werror as a compiler parameter. It causes Travis to build.
//       This is the reason that we must give an empty string argument as a last parameter
//       to supress warning that clang gives.
INSTANTIATE_TEST_CASE_P(WithAndWithoutNullableAttributes,
                        PackedRowStoreTupleStorageSubBlockTest,
                        ::testing::Bool(),);  // NOLINT(whitespace/comma)

INSTANTIATE_TEST_CASE_P(WithAndWithoutNullableAttributes,
                        PackedRowStoreTupleStorageSubBlockDeathTest,
                        ::testing::Bool(),);  // NOLINT(whitespace/comma)

}  // namespace quickstep
