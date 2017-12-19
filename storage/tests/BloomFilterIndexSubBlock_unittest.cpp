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

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/predicate/ComparisonPredicate.hpp"
#include "expressions/predicate/PredicateCost.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "expressions/scalar/ScalarLiteral.hpp"
#include "storage/BloomFilterIndexSubBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/TupleIdSequence.hpp"
#include "storage/tests/MockTupleStorageSubBlock.hpp"
#include "types/CharType.hpp"
#include "types/DoubleType.hpp"
#include "types/FloatType.hpp"
#include "types/LongType.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/VarCharType.hpp"
#include "types/containers/Tuple.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/ScopedBuffer.hpp"

#include "glog/logging.h"
#include "gtest/gtest.h"

using std::int64_t;
using std::ostringstream;
using std::size_t;
using std::string;
using std::vector;

namespace quickstep {

class BloomFilterIndexSubBlockTest : public ::testing::Test {
 protected:
  static const size_t kIndexSubBlockSize = 0x100000;  // 100 KB
  static const int64_t kLongAttrNullValue = -55555;
  static const char kCharAttrNullValue[];

  virtual void SetUp() {
    // Create a sample relation with a variety of attribute types.
    relation_.reset(new CatalogRelation(NULL, "TestRelation"));

    CatalogAttribute *long_attr = new CatalogAttribute(relation_.get(),
                                                       "long_attr",
                                                       TypeFactory::GetType(kLong, false));
    ASSERT_EQ(0, relation_->addAttribute(long_attr));

    CatalogAttribute *nullable_long_attr = new CatalogAttribute(relation_.get(),
                                                                "nullable_long_attr",
                                                                TypeFactory::GetType(kLong, true));
    ASSERT_EQ(1, relation_->addAttribute(nullable_long_attr));

    CatalogAttribute *float_attr = new CatalogAttribute(relation_.get(),
                                                        "float_attr",
                                                        TypeFactory::GetType(kFloat, false));
    ASSERT_EQ(2, relation_->addAttribute(float_attr));

    CatalogAttribute *char_attr = new CatalogAttribute(relation_.get(),
                                                       "char_attr",
                                                       TypeFactory::GetType(kChar, 4, false));
    ASSERT_EQ(3, relation_->addAttribute(char_attr));

    CatalogAttribute *nullable_char_attr = new CatalogAttribute(relation_.get(),
                                                                "nullable_char_attr",
                                                                TypeFactory::GetType(kChar, 4, true));
    ASSERT_EQ(4, relation_->addAttribute(nullable_char_attr));

    CatalogAttribute *big_char_attr = new CatalogAttribute(relation_.get(),
                                                           "big_char_attr",
                                                           TypeFactory::GetType(kChar, 80, false));
    ASSERT_EQ(5, relation_->addAttribute(big_char_attr));

    CatalogAttribute *varchar_attr = new CatalogAttribute(relation_.get(),
                                                          "varchar_attr",
                                                          TypeFactory::GetType(kVarChar, 8, false));
    ASSERT_EQ(6, relation_->addAttribute(varchar_attr));

    // Create a MockTupleStorageSubBlock to hold tuples for testing.
    tuple_store_.reset(new MockTupleStorageSubBlock(*relation_));

    index_memory_.reset();
    index_description_.reset();
    index_.reset();
  }

  void createIndex(const vector<attribute_id> &indexed_attrs, const size_t index_memory_size) {
    // Make the IndexSubBlockDescription.
    index_description_.reset(new IndexSubBlockDescription());
    index_description_->set_sub_block_type(IndexSubBlockDescription::BLOOM_FILTER);
    for (std::size_t i = 0; i < indexed_attrs.size(); ++i) {
      index_description_->add_indexed_attribute_ids(indexed_attrs[i]);
    }
    index_memory_.reset(index_memory_size);
    index_.reset(new BloomFilterIndexSubBlock(*tuple_store_,
                                              *index_description_,
                                              true,
                                              index_memory_.get(),
                                              index_memory_size));
  }

  // Insert a tuple with the specified attribute values into tuple_store_.
  tuple_id insertTupleInTupleStore(const int64_t long_val,
                                   const int64_t nullable_long_val,
                                   const float float_val,
                                   const string &char_val,
                                   const string &nullable_char_val,
                                   const string &big_char_val,
                                   const string &varchar_val) {
    std::vector<TypedValue> attrs;

    attrs.emplace_back(LongType::InstanceNonNullable().makeValue(&long_val));

    if (nullable_long_val == kLongAttrNullValue) {
      attrs.emplace_back(LongType::InstanceNullable().makeNullValue());
    } else {
      attrs.emplace_back(LongType::InstanceNullable().makeValue(&nullable_long_val));
    }

    attrs.emplace_back(FloatType::InstanceNonNullable().makeValue(&float_val));

    attrs.emplace_back(CharType::InstanceNonNullable(4).makeValue(
        char_val.c_str(),
        char_val.size() >= 4 ? 4 : char_val.size() + 1).ensureNotReference());

    if (nullable_char_val == kCharAttrNullValue) {
      attrs.emplace_back(CharType::InstanceNullable(4).makeNullValue());
    } else {
      attrs.emplace_back(CharType::InstanceNonNullable(4).makeValue(
          nullable_char_val.c_str(),
          nullable_char_val.size() >= 4 ? 4 : nullable_char_val.size() + 1).ensureNotReference());
    }

    attrs.emplace_back(CharType::InstanceNonNullable(80).makeValue(
        big_char_val.c_str(),
        big_char_val.size() >= 80 ? 80 : big_char_val.size() + 1).ensureNotReference());

    TypedValue varchar_typed_value
        = VarCharType::InstanceNonNullable(varchar_val.size()).makeValue(
            varchar_val.c_str(),
            varchar_val.size() + 1);
    // Test strings are sometimes longer than 8 characters, so truncate if
    // needed.
    varchar_typed_value = VarCharType::InstanceNonNullable(8).coerceValue(
        varchar_typed_value,
        VarCharType::InstanceNonNullable(varchar_val.size()));
    varchar_typed_value.ensureNotReference();
    attrs.emplace_back(std::move(varchar_typed_value));

    // MockTupleStorageSubBlock takes ownership of new tuple passed in with
    // addTupleMock() method.
    return tuple_store_->addTupleMock(new Tuple(std::move(attrs)));
  }

  // Generate a sample tuple based on 'base_value' and insert in into
  // tuple_store_. The sample tuple will have long_attr equal to 'base_value',
  // float_attr equal to 0.25 * base_value, and each of char_attr,
  // big_char_attr, and varchar_attr equal to the string representation of
  // 'base_value' with 'string_suffix' appended on to it. If 'generate_nulls'
  // is true, then both nullable_long_attr and nullable_char_attr will be NULL,
  // otherwise nullable_long_attr will be equal to 'base_value' and
  // nullable_char_attr will be equal to the other string values. Returns the
  // tuple_id of the inserted tuple.
  tuple_id generateAndInsertTuple(const int64_t base_value,
                                  const bool generate_nulls,
                                  const string &string_suffix) {
    ostringstream string_value_buffer;
    string_value_buffer << base_value << string_suffix;
    if (generate_nulls) {
      return insertTupleInTupleStore(base_value,
                                     kLongAttrNullValue,
                                     0.25 * base_value,
                                     string_value_buffer.str(),
                                     kCharAttrNullValue,
                                     string_value_buffer.str(),
                                     string_value_buffer.str());
    } else {
      return insertTupleInTupleStore(base_value,
                                     base_value,
                                     0.25 * base_value,
                                     string_value_buffer.str(),
                                     string_value_buffer.str(),
                                     string_value_buffer.str(),
                                     string_value_buffer.str());
    }
  }

  // Create a ComparisonPredicate of the form "attribute comp literal".
  template <typename AttributeType>
  ComparisonPredicate* generateNumericComparisonPredicate(
      const ComparisonID comp,
      const attribute_id attribute,
      const typename AttributeType::cpptype literal) {
    ScalarAttribute *scalar_attribute = new ScalarAttribute(*relation_->getAttributeById(attribute));
    ScalarLiteral *scalar_literal
        = new ScalarLiteral(AttributeType::InstanceNonNullable().makeValue(&literal),
                            AttributeType::InstanceNonNullable());
    return new ComparisonPredicate(ComparisonFactory::GetComparison(comp), scalar_attribute, scalar_literal);
  }

  ComparisonPredicate* generateStringComparisonPredicate(
      const ComparisonID comp,
      const attribute_id attribute,
      const string &literal) {
    ScalarAttribute *scalar_attribute = new ScalarAttribute(*relation_->getAttributeById(attribute));
    ScalarLiteral *scalar_literal = new ScalarLiteral(
        VarCharType::InstanceNonNullable(literal.size()).makeValue(
            literal.c_str(),
            literal.size() + 1).ensureNotReference(),
        VarCharType::InstanceNonNullable(literal.size()));
    return new ComparisonPredicate(ComparisonFactory::GetComparison(comp), scalar_attribute, scalar_literal);
  }

  bool requiresRebuild() const {
    CHECK(index_) << "Bloom Filter index not yet built!";
    return !index_->is_consistent_;
  }

  std::unique_ptr<CatalogRelation> relation_;
  std::unique_ptr<MockTupleStorageSubBlock> tuple_store_;
  ScopedBuffer index_memory_;
  std::unique_ptr<IndexSubBlockDescription> index_description_;
  std::unique_ptr<BloomFilterIndexSubBlock> index_;
};

const char BloomFilterIndexSubBlockTest::kCharAttrNullValue[] = "_NULLSTRING";

TEST_F(BloomFilterIndexSubBlockTest, DescriptionIsValidTest) {
  std::unique_ptr<IndexSubBlockDescription> index_description_;
  // valid_attrs contains attribute ids which should register as
  // a valid DescriptionIsValid.
  vector<attribute_id> valid_attrs({ 0, 1, 2, 3, 4, 5, 6 });
  // Try to create an index on each of the attributes. Make sure that each
  // of the attributes which can be indexed on is marked valid.
  for (const CatalogAttribute &attr : *relation_) {
    index_description_.reset(new IndexSubBlockDescription());
    index_description_->set_sub_block_type(IndexSubBlockDescription::BLOOM_FILTER);
    index_description_->add_indexed_attribute_ids(attr.getID());
    if (std::find(valid_attrs.begin(), valid_attrs.end(), attr.getID()) != valid_attrs.end()) {
      EXPECT_TRUE(BloomFilterIndexSubBlock::DescriptionIsValid(*relation_, *index_description_))
          << "Expected attribute " << attr.getID() << " to be valid.";
    } else {
      EXPECT_FALSE(BloomFilterIndexSubBlock::DescriptionIsValid(*relation_, *index_description_))
          << "Expected attribute " << attr.getID() << " to be invalid.";
    }
  }
}

TEST_F(BloomFilterIndexSubBlockTest, TestConstructor) {
  // This test checks the correctness of the index's constructor and destructor.
  createIndex({0, 1, 2}, kIndexSubBlockSize);

  EXPECT_FALSE(requiresRebuild());

  // Reset will invoke the destructor for the index.
  index_.reset(nullptr);

  // Creating a block on the same memory should cause the index to retrieve
  // the previously written values.
  index_.reset(new BloomFilterIndexSubBlock(*tuple_store_,
                                            *index_description_,
                                            false,
                                            index_memory_.get(),
                                            kIndexSubBlockSize));
  EXPECT_FALSE(requiresRebuild());
}

TEST_F(BloomFilterIndexSubBlockTest, TestRebuild) {
  // This test checks whether the index is rebuilt correctly
  // after the index is marked inconsistent.
  const attribute_id indexed_attr = 0;
  createIndex({ indexed_attr }, kIndexSubBlockSize);
  EXPECT_FALSE(requiresRebuild());

  // Insert 1000 tuples in the index.
  const std::size_t num_tuples = 1000;
  for (std::size_t i = 0; i < num_tuples; ++i) {
    generateAndInsertTuple(i, false, "suffix");
  }

  // Deleting a tuple should mark the index as inconsistent.
  const tuple_id tuple_to_remove = 100;
  index_->removeEntry(tuple_to_remove);
  EXPECT_TRUE(requiresRebuild());

  // Rebuilding the index should mark it consistent.
  index_->rebuild();
  EXPECT_FALSE(requiresRebuild());
}

TEST_F(BloomFilterIndexSubBlockTest, TestGetSelectivity) {
  // This test checks the following conditions on selectivity:
  // 1. Selectivity for an inconsistent index should be kSelectivityUnknown,
  // 2. Selectivity for a false positive should be kSelectivityAll,
  // 3. Selectivity for a true negative should be kSelectivityNone.
  const attribute_id indexed_attr = 0;
  createIndex({ indexed_attr }, kIndexSubBlockSize);
  EXPECT_FALSE(requiresRebuild());

  const uint64_t indexed_value = 999;
  const uint64_t non_indexed_value = 9999;

  std::unique_ptr<ComparisonPredicate> indexed_predicate(
      generateNumericComparisonPredicate<LongType>(
          ComparisonID::kEqual, indexed_attr, indexed_value));

  std::unique_ptr<ComparisonPredicate> non_indexed_predicate(
      generateNumericComparisonPredicate<LongType>(
          ComparisonID::kEqual, indexed_attr, non_indexed_value));

  generateAndInsertTuple(indexed_value, false, "suffix");
  // Until the tuple is inserted into index, expect the selectivity to be none.
  EXPECT_EQ(BloomFilterIndexSubBlock::BloomFilterSelectivity::kSelectivityNone,
            index_->getSelectivityForPredicate(*indexed_predicate));

  index_->addEntry(0);
  // Once the tuple is inserted into index, expect the selectivity to be all.
  EXPECT_EQ(BloomFilterIndexSubBlock::BloomFilterSelectivity::kSelectivityAll,
            index_->getSelectivityForPredicate(*indexed_predicate));
  // A non-indexed value should not be present in the index.
  EXPECT_EQ(BloomFilterIndexSubBlock::BloomFilterSelectivity::kSelectivityNone,
            index_->getSelectivityForPredicate(*non_indexed_predicate));

  index_->removeEntry(0);
  // Removing an entry makes the index inconsistent and selectivity unknown.
  EXPECT_EQ(BloomFilterIndexSubBlock::BloomFilterSelectivity::kSelectivityUnknown,
            index_->getSelectivityForPredicate(*indexed_predicate));

  index_->rebuild();
  // Once rebuilt, the index is consistent again.
  EXPECT_EQ(BloomFilterIndexSubBlock::BloomFilterSelectivity::kSelectivityNone,
            index_->getSelectivityForPredicate(*indexed_predicate));
}

TEST_F(BloomFilterIndexSubBlockTest, TestEvaluatePredicateCost) {
  // This test checks the following conditions:
  // 1. Predicate cost for an inconsistent index should be infinite (maximum),
  // 2. Predicate cost for false positives should be infinite (maximum),
  // 3. Predicate cost for true negatives should be constant (minimum).
  const attribute_id indexed_attr = 0;
  createIndex({ indexed_attr }, kIndexSubBlockSize);
  EXPECT_FALSE(requiresRebuild());

  const uint64_t indexed_value = 999;
  const uint64_t non_indexed_value = 9999;

  std::unique_ptr<ComparisonPredicate> indexed_predicate(
      generateNumericComparisonPredicate<LongType>(
          ComparisonID::kEqual, indexed_attr, indexed_value));

  std::unique_ptr<ComparisonPredicate> non_indexed_predicate(
      generateNumericComparisonPredicate<LongType>(
          ComparisonID::kEqual, indexed_attr, non_indexed_value));

  generateAndInsertTuple(indexed_value, false, "suffix");
  // Until the tuple is inserted into index, expect the predicate cost to be constant.
  EXPECT_EQ(predicate_cost::kConstantTime,
            index_->estimatePredicateEvaluationCost(*indexed_predicate));

  index_->addEntry(0);
  // Once the tuple is inserted into index, expect the predicate cost to be infinite.
  EXPECT_EQ(predicate_cost::kInfinite,
            index_->estimatePredicateEvaluationCost(*indexed_predicate));
  // A non-indexed value should not be present in the index.
  EXPECT_EQ(predicate_cost::kConstantTime,
            index_->estimatePredicateEvaluationCost(*non_indexed_predicate));

  index_->removeEntry(0);
  // Removing an entry makes the index inconsistent and predicate cost infinite.
  EXPECT_EQ(predicate_cost::kInfinite,
            index_->estimatePredicateEvaluationCost(*indexed_predicate));

  index_->rebuild();
  // Once rebuilt, the index is consistent again.
  EXPECT_EQ(predicate_cost::kConstantTime,
            index_->estimatePredicateEvaluationCost(*indexed_predicate));
}

TEST_F(BloomFilterIndexSubBlockTest, TestGetMatchesForPredicate) {
  // This test checks the following conditions:
  // 1. All tuples are returned for a predicate with false positive match,
  // 2. No tuples are returned for a predicate with true negative match.
  const attribute_id indexed_attr = 0;
  createIndex({ indexed_attr }, kIndexSubBlockSize);
  EXPECT_FALSE(requiresRebuild());

  const uint64_t indexed_value = 999;
  const uint64_t non_indexed_value = 9999;

  std::unique_ptr<ComparisonPredicate> indexed_predicate(
      generateNumericComparisonPredicate<LongType>(
          ComparisonID::kEqual, indexed_attr, indexed_value));

  std::unique_ptr<ComparisonPredicate> non_indexed_predicate(
      generateNumericComparisonPredicate<LongType>(
          ComparisonID::kEqual, indexed_attr, non_indexed_value));

  generateAndInsertTuple(indexed_value, false, "suffix");
  index_->addEntry(0);

  std::unique_ptr<TupleIdSequence> result;
  // Expect all tuples to be returned.
  result.reset(index_->getMatchesForPredicate(*indexed_predicate, nullptr));
  EXPECT_EQ(static_cast<std::size_t>(tuple_store_->getMaxTupleID() + 1), result->length());
  EXPECT_EQ(static_cast<std::size_t>(tuple_store_->getMaxTupleID() + 1), result->numTuples());

  // Expect no tuples to be returned.
  result.reset(index_->getMatchesForPredicate(*non_indexed_predicate, nullptr));
  EXPECT_EQ(static_cast<std::size_t>(tuple_store_->getMaxTupleID() + 1), result->length());
  EXPECT_EQ(static_cast<std::size_t>(0), result->numTuples());
}

}  // namespace quickstep
