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
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/predicate/ComparisonPredicate.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "expressions/scalar/ScalarLiteral.hpp"
#include "storage/BasicColumnStoreTupleStorageSubBlock.hpp"
#include "storage/StorageBlockLayout.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageConstants.hpp"
#include "storage/StorageErrors.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "storage/tests/TupleStorePredicateUtil.hpp"
#include "types/CharType.hpp"
#include "types/DoubleType.hpp"
#include "types/IntType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypedValue.hpp"
#include "types/TypeID.hpp"
#include "types/VarCharType.hpp"
#include "types/containers/Tuple.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "types/operations/comparisons/ComparisonUtil.hpp"
#include "utility/BitVector.hpp"
#include "utility/ScopedBuffer.hpp"

using std::size_t;
using std::string;
using std::vector;

namespace quickstep {

class BasicColumnStoreTupleStorageSubBlockTest : public ::testing::TestWithParam<bool> {
 protected:
  static const size_t kSubBlockSize = 0x100000;  // 1 MB.

  virtual void SetUp() {
    // Create a sample relation with a variety of attribute types.
    relation_.reset(new CatalogRelation(NULL, "TestRelation"));

    CatalogAttribute *int_attr = new CatalogAttribute(relation_.get(),
                                                      "int_attr",
                                                      TypeFactory::GetType(kInt, GetParam()));
    ASSERT_EQ(0, relation_->addAttribute(int_attr));

    CatalogAttribute *double_attr = new CatalogAttribute(relation_.get(),
                                                         "double_attr",
                                                         TypeFactory::GetType(kDouble, GetParam()));
    ASSERT_EQ(1, relation_->addAttribute(double_attr));

    CatalogAttribute *narrow_char_attr = new CatalogAttribute(relation_.get(),
                                                              "narrow_char_attr",
                                                              TypeFactory::GetType(kChar, 4, GetParam()));
    ASSERT_EQ(2, relation_->addAttribute(narrow_char_attr));

    CatalogAttribute *wide_char_attr = new CatalogAttribute(relation_.get(),
                                                            "wide_char_attr",
                                                            TypeFactory::GetType(kChar, 32, GetParam()));
    ASSERT_EQ(3, relation_->addAttribute(wide_char_attr));

    // Records the 'base_value' of a tuple used in createSampleTuple.
    CatalogAttribute *base_value_attr = new CatalogAttribute(relation_.get(),
                                                             "base_value",
                                                             TypeFactory::GetType(kInt, false));
    ASSERT_EQ(4, relation_->addAttribute(base_value_attr));

    // Don't initialize the block yet. Different tests will use different sort
    // columns.
    tuple_store_memory_.reset();
    tuple_store_description_.reset();
    tuple_store_.reset();
  }

  void createBlock(const attribute_id sort_column, const size_t block_memory_size) {
    tuple_store_memory_.reset(block_memory_size);

    tuple_store_description_.reset(new TupleStorageSubBlockDescription());
    tuple_store_description_->set_sub_block_type(TupleStorageSubBlockDescription::BASIC_COLUMN_STORE);
    if (sort_column != kInvalidCatalogId) {
      tuple_store_description_->SetExtension(
          BasicColumnStoreTupleStorageSubBlockDescription::sort_attribute_id,
          sort_column);
    }

    tuple_store_.reset(new BasicColumnStoreTupleStorageSubBlock(*relation_,
                                                                *tuple_store_description_,
                                                                true,
                                                                tuple_store_memory_.get(),
                                                                block_memory_size));
  }

  // Caller takes ownership of new heap-created Tuple.
  Tuple* createSampleTuple(const int base_value) const {
    std::vector<TypedValue> attrs;

    // int_attr
    if (GetParam() && (base_value % 8 == 0)) {
      // Throw in a NULL integer for every eighth value.
      attrs.emplace_back(kInt);
    } else {
      attrs.emplace_back(base_value);
    }

    // double_attr
    if (GetParam() && (base_value % 8 == 2)) {
      // NULL very eighth value.
      attrs.emplace_back(kDouble);
    } else {
      attrs.emplace_back(static_cast<double>(0.25 * base_value));
    }

    // narrow_char_attr
    if (GetParam() && (base_value % 8 == 4)) {
      // NULL very eighth value.
      attrs.emplace_back(CharType::InstanceNullable(4).makeNullValue());
    } else {
      std::ostringstream char_buffer;
      char_buffer << base_value;
      std::string string_literal(char_buffer.str());
      attrs.emplace_back(CharType::InstanceNonNullable(4).makeValue(
          string_literal.c_str(),
          string_literal.size() > 3 ? 4
                                    : string_literal.size() + 1));
      attrs.back().ensureNotReference();
    }

    // wide_char_attr
    if (GetParam() && (base_value % 8 == 6)) {
      // NULL very eighth value.
      attrs.emplace_back(CharType::InstanceNullable(32).makeNullValue());
    } else {
      std::ostringstream char_buffer;
      char_buffer << "Here are some numbers: " << base_value;
      std::string string_literal(char_buffer.str());
      attrs.emplace_back(CharType::InstanceNonNullable(32).makeValue(
          string_literal.c_str(),
          string_literal.size() + 1));
      attrs.back().ensureNotReference();
    }

    // base_value
    attrs.emplace_back(base_value);
    return new Tuple(std::move(attrs));
  }

  // Make a tuple out of the surrogate values used to sort nulls. Caller takes
  // ownership of new heap-created Tuple.
  Tuple* createTupleWithLastValues() const {
    std::vector<TypedValue> attrs;

    attrs.emplace_back(GetLastValueForType(TypeFactory::GetType(kInt, GetParam())));
    attrs.emplace_back(GetLastValueForType(TypeFactory::GetType(kDouble, GetParam())));
    attrs.emplace_back(GetLastValueForType(TypeFactory::GetType(kChar, 4, GetParam())));
    attrs.emplace_back(GetLastValueForType(TypeFactory::GetType(kChar, 32, GetParam())));
    attrs.emplace_back(-1);

    return new Tuple(std::move(attrs));
  }

  int computeRowCapacity() const {
    int initial_estimate = ((kSubBlockSize - 2 * sizeof(tuple_id)) << 3)
                           / ((relation_->getFixedByteLength() << 3) + relation_->numNullableAttributes());
    return (kSubBlockSize
                - (2 * sizeof(tuple_id))
                - (relation_->numNullableAttributes() * BitVector<false>::BytesNeeded(initial_estimate)))
           / relation_->getFixedByteLength();
  }

  int computeNullsInSortColumn(const attribute_id sort_column_id) {
    if (sort_column_id == kInvalidCatalogId || !GetParam()) {
      return 0;
    }

    int row_capacity = computeRowCapacity();
    int full_strides = (row_capacity - 2) / 8;
    int stride_remainder = (row_capacity - 2) % 8;
    switch (sort_column_id) {
      case 0:
        return full_strides + (stride_remainder > 0 ? 1 : 0);
      case 1:
        return full_strides + (stride_remainder > 2 ? 1 : 0);
      case 2:
        return full_strides + (stride_remainder > 4 ? 1 : 0);
      case 3:
        return full_strides + (stride_remainder > 6 ? 1 : 0);
      default:
        return 0;
    }
  }

  void fillBlockWithSampleData(const bool random_order, const bool ad_hoc) {
    std::unique_ptr<Tuple> current_tuple;
    if (random_order) {
      std::vector<int> base_values;
      for (int i = 0; i < computeRowCapacity() - 2; ++i) {
        base_values.push_back(i);
      }
      std::random_shuffle(base_values.begin(), base_values.end());

      if (ad_hoc) {
        // First, insert a tuple with special "last" values.
        TupleStorageSubBlock::InsertResult result(-1, false);
        current_tuple.reset(createTupleWithLastValues());
        result = tuple_store_->insertTuple(*current_tuple);
        EXPECT_EQ(0, result.inserted_id);
        EXPECT_FALSE(result.ids_mutated);
        int inserted = 1;

        // Regular tuples.
        for (const int base_value : base_values) {
          current_tuple.reset(createSampleTuple(base_value));
          result = tuple_store_->insertTuple(*current_tuple);
          EXPECT_GE(result.inserted_id, 0);
          ++inserted;
          if (result.inserted_id == (inserted - 1)) {
            EXPECT_FALSE(result.ids_mutated);
          } else {
            EXPECT_TRUE(result.ids_mutated);
          }
        }

        // Another "last" tuple.
        current_tuple.reset(createTupleWithLastValues());
        result = tuple_store_->insertTuple(*current_tuple);
        EXPECT_GE(result.inserted_id, 0);
        ++inserted;
        if (result.inserted_id == (inserted - 1)) {
          EXPECT_FALSE(result.ids_mutated);
        } else {
          EXPECT_TRUE(result.ids_mutated);
        }

        // No more space.
        result = tuple_store_->insertTuple(*current_tuple);
        EXPECT_EQ(-1, result.inserted_id);
      } else {
        current_tuple.reset(createTupleWithLastValues());
        EXPECT_TRUE(tuple_store_->insertTupleInBatch(*current_tuple));

        for (const int base_value : base_values) {
          current_tuple.reset(createSampleTuple(base_value));
          EXPECT_TRUE(tuple_store_->insertTupleInBatch(*current_tuple));
        }

        current_tuple.reset(createTupleWithLastValues());
        EXPECT_TRUE(tuple_store_->insertTupleInBatch(*current_tuple));

        // No more space.
        EXPECT_FALSE(tuple_store_->insertTupleInBatch(*current_tuple));

        tuple_store_->rebuild();
      }
    } else {
      // Fixed order.
      if (ad_hoc) {
        TupleStorageSubBlock::InsertResult result(-1, false);
        current_tuple.reset(createTupleWithLastValues());
        result = tuple_store_->insertTuple(*current_tuple);
        EXPECT_EQ(0, result.inserted_id);
        EXPECT_FALSE(result.ids_mutated);
        int inserted = 1;

        for (int base_value = 0; base_value < computeRowCapacity() - 2; ++base_value) {
          current_tuple.reset(createSampleTuple(base_value));
          result = tuple_store_->insertTuple(*current_tuple);
          EXPECT_GE(result.inserted_id, 0);
          ++inserted;
          if (result.inserted_id == (inserted - 1)) {
            EXPECT_FALSE(result.ids_mutated);
          } else {
            EXPECT_TRUE(result.ids_mutated);
          }
        }

        current_tuple.reset(createTupleWithLastValues());
        result = tuple_store_->insertTuple(*current_tuple);
        EXPECT_GE(result.inserted_id, 0);
        ++inserted;
        if (result.inserted_id == (inserted - 1)) {
          EXPECT_FALSE(result.ids_mutated);
        } else {
          EXPECT_TRUE(result.ids_mutated);
        }

        // No more space.
        result = tuple_store_->insertTuple(*current_tuple);
        EXPECT_EQ(-1, result.inserted_id);
      } else {
        current_tuple.reset(createTupleWithLastValues());
        EXPECT_TRUE(tuple_store_->insertTupleInBatch(*current_tuple));

        for (int base_value = 0; base_value < computeRowCapacity() - 2; ++base_value) {
          current_tuple.reset(createSampleTuple(base_value));
          EXPECT_TRUE(tuple_store_->insertTupleInBatch(*current_tuple));
        }

        current_tuple.reset(createTupleWithLastValues());
        EXPECT_TRUE(tuple_store_->insertTupleInBatch(*current_tuple));

        // No more space.
        EXPECT_FALSE(tuple_store_->insertTupleInBatch(*current_tuple));

        tuple_store_->rebuild();
      }
    }
  }

  void checkBlockValues(const attribute_id sort_attribute_id,
                        const size_t regular_tuples_deleted,
                        const size_t last_tuples_deleted) const {
    EXPECT_EQ(computeRowCapacity() - regular_tuples_deleted - last_tuples_deleted,
              static_cast<std::size_t>(tuple_store_->getMaxTupleID() + 1));

    std::unique_ptr<UncheckedComparator> sort_attribute_comparator;
    if (sort_attribute_id != kInvalidCatalogId) {
      const Type &sort_attribute_type =
          relation_->getAttributeById(sort_attribute_id)->getType();
      sort_attribute_comparator.reset(
          ComparisonFactory::GetComparison(ComparisonID::kLessOrEqual)
              .makeUncheckedComparatorForTypes(sort_attribute_type,
                                               sort_attribute_type));
    }

    std::vector<bool> existence_check_vector;
    existence_check_vector.resize(computeRowCapacity() - 2, false);
    size_t num_last_tuples = 0;
    size_t num_regular_tuples = 0;

    std::unique_ptr<TypedValue> previous_sort_attribute_value;
    for (tuple_id tid = 0;
         tid <= tuple_store_->getMaxTupleID();
         ++tid) {
      EXPECT_TRUE(tuple_store_->hasTupleWithID(tid));

      const int base_value = *static_cast<const int*>(tuple_store_->getAttributeValue(tid, 4));
      ASSERT_LT(base_value, computeRowCapacity() - 2);
      std::unique_ptr<Tuple> comparison_tuple;
      if (base_value == -1) {
        ++num_last_tuples;
        comparison_tuple.reset(createTupleWithLastValues());
      } else {
        ++num_regular_tuples;
        EXPECT_FALSE(existence_check_vector[base_value]);
        existence_check_vector[base_value] = true;
        comparison_tuple.reset(createSampleTuple(base_value));
      }

      // Check attribute values.
      for (const CatalogAttribute &attr : *relation_) {
        const void *untyped_value = tuple_store_->getAttributeValue(tid, attr.getID());
        TypedValue typed_value = tuple_store_->getAttributeValueTyped(tid, attr.getID());

        EXPECT_TRUE(typed_value.isPlausibleInstanceOf(attr.getType().getSignature()));
        if (comparison_tuple->getAttributeValue(attr.getID()).isNull()) {
          EXPECT_EQ(nullptr, untyped_value);
          EXPECT_TRUE(typed_value.isNull());
        } else {
          EXPECT_TRUE(CheckUntypedValuesEqual(attr.getType(),
                                              comparison_tuple->getAttributeValue(attr.getID()).getDataPtr(),
                                              untyped_value));
          EXPECT_TRUE(CheckUntypedValuesEqual(attr.getType(),
                                              comparison_tuple->getAttributeValue(attr.getID()).getDataPtr(),
                                              typed_value.getDataPtr()));
        }
      }

      // Check ordering if block is sorted.
      if (sort_attribute_id != kInvalidCatalogId) {
        TypedValue sort_attribute_value =
            tuple_store_->getAttributeValueTyped(tid, sort_attribute_id);
        if (previous_sort_attribute_value) {
          if (previous_sort_attribute_value->isNull()) {
            EXPECT_TRUE(sort_attribute_value.isNull());
          } else if (!sort_attribute_value.isNull()) {
            EXPECT_TRUE(sort_attribute_comparator->compareTypedValues(
                *previous_sort_attribute_value,
                sort_attribute_value));
          }
        }
        previous_sort_attribute_value.reset(new TypedValue(sort_attribute_value));
      }
    }

    EXPECT_EQ(2 - last_tuples_deleted, num_last_tuples);
    EXPECT_EQ(computeRowCapacity() - 2 - regular_tuples_deleted,
              num_regular_tuples);
  }

  void runDeleteTest(const attribute_id sort_attribute_id) {
    createBlock(sort_attribute_id, kSubBlockSize);
    fillBlockWithSampleData(true, false);

    int row_capacity = computeRowCapacity();

    // Delete last tuple.
    EXPECT_FALSE(tuple_store_->deleteTuple(row_capacity - 1));

    // Delete first tuple.
    EXPECT_TRUE(tuple_store_->deleteTuple(0));

    // Delete a sequence of tuples.
    TupleIdSequence delete_sequence(tuple_store_->getMaxTupleID() + 1);
    for (tuple_id tid = 0;
         tid < row_capacity - computeNullsInSortColumn(sort_attribute_id) - 2 - 2;
         tid += 5) {
      // Some non-null values.
      delete_sequence.set(tid, true);
    }
    // Some nulls, if any exist.
    for (tuple_id tid = row_capacity - computeNullsInSortColumn(sort_attribute_id) + 1;
         tid < row_capacity - 2;
         tid += 5) {
      delete_sequence.set(tid, true);
    }
    // One of the special "last" values.
    if (sort_attribute_id != kInvalidCatalogId && GetParam()) {
      delete_sequence.set(row_capacity - computeNullsInSortColumn(sort_attribute_id) - 2, true);
    }

    EXPECT_TRUE(tuple_store_->bulkDeleteTuples(&delete_sequence));

    if (sort_attribute_id == kInvalidCatalogId) {
      // There is no special treatment of "last" values without sorting.
      checkBlockValues(sort_attribute_id, delete_sequence.numTuples(), 2);
    } else {
      checkBlockValues(sort_attribute_id, delete_sequence.numTuples() + 2 - 1, 1);
    }
  }

  // Create a ComparisonPredicate of the form "attribute comp literal".
  template <typename AttributeType>
  Predicate* generateNumericComparisonPredicate(const ComparisonID comp,
                                                const attribute_id attribute,
                                                const typename AttributeType::cpptype literal) const {
    ScalarAttribute *scalar_attribute = new ScalarAttribute(*relation_->getAttributeById(attribute));
    ScalarLiteral *scalar_literal
        = new ScalarLiteral(AttributeType::InstanceNonNullable().makeValue(&literal),
                            AttributeType::InstanceNonNullable());
    return new ComparisonPredicate(ComparisonFactory::GetComparison(comp), scalar_attribute, scalar_literal);
  }

  Predicate* generateStringComparisonPredicate(const ComparisonID comp,
                                               const attribute_id attribute,
                                               const string &literal) const {
    ScalarAttribute *scalar_attribute = new ScalarAttribute(*relation_->getAttributeById(attribute));
    ScalarLiteral *scalar_literal = new ScalarLiteral(
        VarCharType::InstanceNonNullable(literal.size()).makeValue(
            literal.c_str(),
            literal.size() + 1).ensureNotReference(),
        VarCharType::InstanceNonNullable(literal.size()));
    return new ComparisonPredicate(ComparisonFactory::GetComparison(comp),
                                   scalar_attribute,
                                   scalar_literal);
  }

  template <typename ValueType>
  inline static bool matchExpected(const ComparisonID comp,
                                   const ValueType &left,
                                   const ValueType &right) {
    switch (comp) {
      case ComparisonID::kEqual:
        return (left == right);
      case ComparisonID::kNotEqual:
        return (left != right);
      case ComparisonID::kLess:
        return (left < right);
      case ComparisonID::kLessOrEqual:
        return (left <= right);
      case ComparisonID::kGreater:
        return (left > right);
      case ComparisonID::kGreaterOrEqual:
        return (left >= right);
      default:
        FATAL_ERROR("Unrecognized ComparisonID");
    }
  }

  void checkPredicateIntAttr(const ComparisonID comp) const {
    int comparison_literal = computeRowCapacity() >> 1;

    std::unique_ptr<Predicate> predicate(
        generateNumericComparisonPredicate<IntType>(comp,
                                                    0,
                                                    comparison_literal));
    std::unique_ptr<TupleIdSequence> matches(
        TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));

    for (tuple_id tid = 0;
         tid < static_cast<tuple_id>(matches->length());
         ++tid) {
      int insert_order_value = *static_cast<const int*>(tuple_store_->getAttributeValue(tid, 4));
      if (GetParam() && (insert_order_value % 8 == 0)) {
        // Value is NULL.
        EXPECT_FALSE(matches->get(tid));
      } else if (insert_order_value == -1) {
        // Special last value.
        if (matchExpected(comp,
                          GetLastValueForType(TypeFactory::GetType(kInt, GetParam())).getLiteral<int>(),
                          comparison_literal)) {
          EXPECT_TRUE(matches->get(tid));
        } else {
          EXPECT_FALSE(matches->get(tid));
        }
      } else {
        if (matchExpected(comp,
                          insert_order_value,
                          comparison_literal)) {
          EXPECT_TRUE(matches->get(tid));
        } else {
          EXPECT_FALSE(matches->get(tid));
        }
      }
    }
  }

  void checkPredicateDoubleAttr(const ComparisonID comp) const {
    double comparison_literal = (computeRowCapacity() >> 1) * 0.25;

    std::unique_ptr<Predicate> predicate(
        generateNumericComparisonPredicate<DoubleType>(comp,
                                                       1,
                                                       comparison_literal));
    std::unique_ptr<TupleIdSequence> matches(
        TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));

    for (tuple_id tid = 0;
         tid < static_cast<tuple_id>(matches->length());
         ++tid) {
      int insert_order_value = *static_cast<const int*>(tuple_store_->getAttributeValue(tid, 4));
      if (GetParam() && (insert_order_value % 8 == 2)) {
        // Value is NULL.
        EXPECT_FALSE(matches->get(tid));
      } else if (insert_order_value == -1) {
        // Special last value.
        if (matchExpected(comp,
                          GetLastValueForType(TypeFactory::GetType(kDouble, GetParam())).getLiteral<double>(),
                          comparison_literal)) {
          EXPECT_TRUE(matches->get(tid));
        } else {
          EXPECT_FALSE(matches->get(tid));
        }
      } else {
        if (matchExpected(comp,
                          insert_order_value * 0.25,
                          comparison_literal)) {
          EXPECT_TRUE(matches->get(tid));
        } else {
          EXPECT_FALSE(matches->get(tid));
        }
      }
    }
  }

  void checkPredicateNarrowCharAttr(const ComparisonID comp) const {
    string comparison_literal = "500";

    std::unique_ptr<Predicate> predicate(
        generateStringComparisonPredicate(comp,
                                          2,
                                          comparison_literal));
    std::unique_ptr<TupleIdSequence> matches(
        TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));

    for (tuple_id tid = 0;
         tid < static_cast<tuple_id>(matches->length());
         ++tid) {
      int insert_order_value = *static_cast<const int*>(tuple_store_->getAttributeValue(tid, 4));
      if (GetParam() && (insert_order_value % 8 == 4)) {
        // Value is NULL.
        EXPECT_FALSE(matches->get(tid));
      } else if (insert_order_value == -1) {
        // Special last value.
        if (matchExpected(comp,
                          std::string(static_cast<const char*>(
                              GetLastValueForType(TypeFactory::GetType(kChar, 4, GetParam())).getDataPtr())),
                          comparison_literal)) {
          EXPECT_TRUE(matches->get(tid));
        } else {
          EXPECT_FALSE(matches->get(tid));
        }
      } else {
        std::ostringstream char_buffer;
        char_buffer << insert_order_value;
        std::string string_literal(char_buffer.str());
        if (string_literal.size() > 4) {
          string_literal.resize(4);
        }
        if (matchExpected(comp,
                          string_literal,
                          comparison_literal)) {
          EXPECT_TRUE(matches->get(tid));
        } else {
          EXPECT_FALSE(matches->get(tid));
        }
      }
    }
  }

  void checkPredicateWideCharAttr(const ComparisonID comp) const {
    string comparison_literal = "Here are some numbers: 5000";

    std::unique_ptr<Predicate> predicate(
        generateStringComparisonPredicate(comp,
                                          3,
                                          comparison_literal));
    std::unique_ptr<TupleIdSequence> matches(
        TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));

    for (tuple_id tid = 0;
         tid < static_cast<tuple_id>(matches->length());
         ++tid) {
      int insert_order_value = *static_cast<const int*>(tuple_store_->getAttributeValue(tid, 4));
      if (GetParam() && (insert_order_value % 8 == 6)) {
        // Value is NULL.
        EXPECT_FALSE(matches->get(tid));
      } else if (insert_order_value == -1) {
        // Special last value.
        if (matchExpected(comp,
                          std::string(static_cast<const char*>(
                              GetLastValueForType(TypeFactory::GetType(kChar, 32, GetParam())).getDataPtr())),
                          comparison_literal)) {
          EXPECT_TRUE(matches->get(tid));
        } else {
          EXPECT_FALSE(matches->get(tid));
        }
      } else {
        std::ostringstream char_buffer;
        char_buffer << "Here are some numbers: " << insert_order_value;
        std::string string_literal(char_buffer.str());
        if (matchExpected(comp,
                          string_literal,
                          comparison_literal)) {
          EXPECT_TRUE(matches->get(tid));
        } else {
          EXPECT_FALSE(matches->get(tid));
        }
      }
    }
  }

  void runCheckPredicateTest(const attribute_id sort_attribute_id) {
    createBlock(sort_attribute_id, kSubBlockSize);
    fillBlockWithSampleData(true, false);

    checkPredicateIntAttr(ComparisonID::kEqual);
    checkPredicateIntAttr(ComparisonID::kNotEqual);
    checkPredicateIntAttr(ComparisonID::kLess);
    checkPredicateIntAttr(ComparisonID::kLessOrEqual);
    checkPredicateIntAttr(ComparisonID::kGreater);
    checkPredicateIntAttr(ComparisonID::kGreaterOrEqual);

    checkPredicateDoubleAttr(ComparisonID::kEqual);
    checkPredicateDoubleAttr(ComparisonID::kNotEqual);
    checkPredicateDoubleAttr(ComparisonID::kLess);
    checkPredicateDoubleAttr(ComparisonID::kLessOrEqual);
    checkPredicateDoubleAttr(ComparisonID::kGreater);
    checkPredicateDoubleAttr(ComparisonID::kGreaterOrEqual);

    checkPredicateNarrowCharAttr(ComparisonID::kEqual);
    checkPredicateNarrowCharAttr(ComparisonID::kNotEqual);
    checkPredicateNarrowCharAttr(ComparisonID::kLess);
    checkPredicateNarrowCharAttr(ComparisonID::kLessOrEqual);
    checkPredicateNarrowCharAttr(ComparisonID::kGreater);
    checkPredicateNarrowCharAttr(ComparisonID::kGreaterOrEqual);

    checkPredicateWideCharAttr(ComparisonID::kEqual);
    checkPredicateWideCharAttr(ComparisonID::kNotEqual);
    checkPredicateWideCharAttr(ComparisonID::kLess);
    checkPredicateWideCharAttr(ComparisonID::kLessOrEqual);
    checkPredicateWideCharAttr(ComparisonID::kGreater);
    checkPredicateWideCharAttr(ComparisonID::kGreaterOrEqual);
  }

  void runSetAttributeValueInPlaceTypedTest(const attribute_id sort_attribute_id) {
    createBlock(sort_attribute_id, kSubBlockSize);
    fillBlockWithSampleData(true, false);

    const tuple_id target_tid = tuple_store_->getMaxTupleID() >> 1;

    TypedValue new_int_value(-42);
    TypedValue new_double_value(static_cast<double>(-4.2));
    const char new_narrow_char_lit[4] = "foo";
    TypedValue new_narrow_char_value(kChar, new_narrow_char_lit, 4);
    const char new_wide_char_lit[32] = "I am not a number!";
    TypedValue new_wide_char_value(kChar, new_wide_char_lit, 32);

    std::unordered_map<attribute_id, TypedValue> proposed_values;
    if (sort_attribute_id != kInvalidCatalogId) {
      switch (sort_attribute_id) {
        case 0:
          proposed_values.emplace(0, new_int_value);
          break;
        case 1:
          proposed_values.emplace(1, new_double_value);
          break;
        case 2:
          proposed_values.emplace(2, new_narrow_char_value);
          break;
        case 3:
          proposed_values.emplace(3, new_wide_char_value);
          break;
      }

      // Can't overwrite a sort column value in-place.
      EXPECT_FALSE(tuple_store_->canSetAttributeValuesInPlaceTyped(
          target_tid, proposed_values));
    }

    // Other column values should be OK.
    proposed_values.clear();
    proposed_values.emplace(0, new_int_value);
    proposed_values.emplace(1, new_double_value);
    proposed_values.emplace(2, new_narrow_char_value);
    proposed_values.emplace(3, new_wide_char_value);
    if (sort_attribute_id != kInvalidCatalogId) {
      proposed_values.erase(sort_attribute_id);
    }
    EXPECT_TRUE(tuple_store_->canSetAttributeValuesInPlaceTyped(target_tid, proposed_values));

    // Actually set values.
    for (const auto &update_pair : proposed_values) {
      tuple_store_->setAttributeValueInPlaceTyped(target_tid, update_pair.first, update_pair.second);
    }

    if (sort_attribute_id != 0) {
      ASSERT_FALSE(tuple_store_->getAttributeValueTyped(target_tid, 0).isNull());
      EXPECT_EQ(new_int_value.getLiteral<int>(),
                tuple_store_->getAttributeValueTyped(target_tid, 0).getLiteral<int>());
    }
    if (sort_attribute_id != 1) {
      ASSERT_FALSE(tuple_store_->getAttributeValueTyped(target_tid, 1).isNull());
      EXPECT_EQ(new_double_value.getLiteral<double>(),
                tuple_store_->getAttributeValueTyped(target_tid, 1).getLiteral<double>());
    }
    if (sort_attribute_id != 2) {
      ASSERT_FALSE(tuple_store_->getAttributeValueTyped(target_tid, 2).isNull());
      EXPECT_STREQ(static_cast<const char*>(new_narrow_char_value.getDataPtr()),
                   static_cast<const char*>(tuple_store_->getAttributeValueTyped(target_tid, 2).getDataPtr()));
    }
    if (sort_attribute_id != 3) {
      ASSERT_FALSE(tuple_store_->getAttributeValueTyped(target_tid, 3).isNull());
      EXPECT_STREQ(static_cast<const char*>(new_wide_char_value.getDataPtr()),
                   static_cast<const char*>(tuple_store_->getAttributeValueTyped(target_tid, 3).getDataPtr()));
    }

    if (GetParam()) {
      // Set NULL values.
      for (const auto &update_pair : proposed_values) {
        tuple_store_->setAttributeValueInPlaceTyped(target_tid,
                                                    update_pair.first,
                                                    TypedValue(update_pair.second.getTypeID()));
      }

      for (attribute_id attr_id = 0; attr_id < 4; ++attr_id) {
        if (attr_id != sort_attribute_id) {
          EXPECT_TRUE(tuple_store_->getAttributeValueTyped(target_tid, attr_id).isNull());
        }
      }

      // Set back to non-null values.
      for (const auto &update_pair : proposed_values) {
        tuple_store_->setAttributeValueInPlaceTyped(target_tid, update_pair.first, update_pair.second);
      }

      if (sort_attribute_id != 0) {
        ASSERT_FALSE(tuple_store_->getAttributeValueTyped(target_tid, 0).isNull());
        EXPECT_EQ(new_int_value.getLiteral<int>(),
                  tuple_store_->getAttributeValueTyped(target_tid, 0).getLiteral<int>());
      }
      if (sort_attribute_id != 1) {
        ASSERT_FALSE(tuple_store_->getAttributeValueTyped(target_tid, 1).isNull());
        EXPECT_EQ(new_double_value.getLiteral<double>(),
                  tuple_store_->getAttributeValueTyped(target_tid, 1).getLiteral<double>());
      }
      if (sort_attribute_id != 2) {
        ASSERT_FALSE(tuple_store_->getAttributeValueTyped(target_tid, 2).isNull());
        EXPECT_STREQ(static_cast<const char*>(new_narrow_char_value.getDataPtr()),
                    static_cast<const char*>(tuple_store_->getAttributeValueTyped(target_tid, 2).getDataPtr()));
      }
      if (sort_attribute_id != 3) {
        ASSERT_FALSE(tuple_store_->getAttributeValueTyped(target_tid, 3).isNull());
        EXPECT_STREQ(static_cast<const char*>(new_wide_char_value.getDataPtr()),
                    static_cast<const char*>(tuple_store_->getAttributeValueTyped(target_tid, 3).getDataPtr()));
      }
    }
  }

  std::unique_ptr<CatalogRelation> relation_;
  ScopedBuffer tuple_store_memory_;
  std::unique_ptr<TupleStorageSubBlockDescription> tuple_store_description_;
  std::unique_ptr<BasicColumnStoreTupleStorageSubBlock> tuple_store_;
};

typedef BasicColumnStoreTupleStorageSubBlockTest BasicColumnStoreTupleStorageSubBlockDeathTest;

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, DescriptionIsValidTest) {
  // The descriptions we use for the other tests should be valid.
  for (const CatalogAttribute &attr : *relation_) {
    tuple_store_description_.reset(
        new TupleStorageSubBlockDescription());
    tuple_store_description_->set_sub_block_type(
        TupleStorageSubBlockDescription::BASIC_COLUMN_STORE);
    tuple_store_description_->SetExtension(
        BasicColumnStoreTupleStorageSubBlockDescription::sort_attribute_id,
        attr.getID());
    EXPECT_TRUE(BasicColumnStoreTupleStorageSubBlock::DescriptionIsValid(
        *relation_,
        *tuple_store_description_));
  }

  // Also check a description that doesn't specify a sort column.
  tuple_store_description_.reset(new TupleStorageSubBlockDescription());
  tuple_store_description_->set_sub_block_type(
      TupleStorageSubBlockDescription::BASIC_COLUMN_STORE);
  EXPECT_TRUE(BasicColumnStoreTupleStorageSubBlock::DescriptionIsValid(
      *relation_,
      *tuple_store_description_));

  // An uninitialized description is not valid.
  tuple_store_description_.reset(new TupleStorageSubBlockDescription());
  EXPECT_FALSE(BasicColumnStoreTupleStorageSubBlock::DescriptionIsValid(
                   *relation_,
                   *tuple_store_description_));

  // A description that specifies the wrong sub_block_type is not valid.
  tuple_store_description_->set_sub_block_type(
      TupleStorageSubBlockDescription::SPLIT_ROW_STORE);
  EXPECT_FALSE(BasicColumnStoreTupleStorageSubBlock::DescriptionIsValid(
                   *relation_,
                   *tuple_store_description_));

  // A relation with a variable-length attribute can't be used with this block type.
  std::unique_ptr<CatalogRelation> variable_length_relation(
      new CatalogRelation(nullptr, "variable_length_relation"));
  CatalogAttribute *variable_length_attribute = new CatalogAttribute(
      variable_length_relation.get(),
      "variable_length_attr",
      TypeFactory::GetType(kVarChar, 20, false));
  ASSERT_EQ(0, variable_length_relation->addAttribute(variable_length_attribute));
  tuple_store_description_.reset(new TupleStorageSubBlockDescription());
  tuple_store_description_->set_sub_block_type(
      TupleStorageSubBlockDescription::BASIC_COLUMN_STORE);
  tuple_store_description_->SetExtension(
      BasicColumnStoreTupleStorageSubBlockDescription::sort_attribute_id,
      0);
  EXPECT_FALSE(BasicColumnStoreTupleStorageSubBlock::DescriptionIsValid(*variable_length_relation,
                                                                        *tuple_store_description_));
}

TEST_P(BasicColumnStoreTupleStorageSubBlockDeathTest, ConstructWithInvalidDescriptionTest) {
  EXPECT_DEATH(createBlock(10, kSubBlockSize), "");
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, BlockTooSmallTest) {
  EXPECT_THROW(createBlock(0, 32), BlockMemoryTooSmall);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, InsertWithNoSortColumnTest) {
  // Non-random, batch insert.
  createBlock(kInvalidCatalogId, kSubBlockSize);
  fillBlockWithSampleData(false, false);

  // Non-random, ad-hoc insert.
  createBlock(kInvalidCatalogId, kSubBlockSize);
  fillBlockWithSampleData(false, true);

  // Random order, batch insert.
  createBlock(kInvalidCatalogId, kSubBlockSize);
  fillBlockWithSampleData(true, false);

  // Random order, ad-hoc insert.
  createBlock(kInvalidCatalogId, kSubBlockSize);
  fillBlockWithSampleData(true, true);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, InsertWithIntSortColumnTest) {
  // Non-random, batch insert.
  createBlock(0, kSubBlockSize);
  fillBlockWithSampleData(false, false);

  // Non-random, ad-hoc insert.
  createBlock(0, kSubBlockSize);
  fillBlockWithSampleData(false, true);

  // Random order, batch insert.
  createBlock(0, kSubBlockSize);
  fillBlockWithSampleData(true, false);

  // Random order, ad-hoc insert.
  createBlock(0, kSubBlockSize);
  fillBlockWithSampleData(true, true);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, InsertWithDoubleSortColumnTest) {
  // Non-random, batch insert.
  createBlock(1, kSubBlockSize);
  fillBlockWithSampleData(false, false);

  // Non-random, ad-hoc insert.
  createBlock(1, kSubBlockSize);
  fillBlockWithSampleData(false, true);

  // Random order, batch insert.
  createBlock(1, kSubBlockSize);
  fillBlockWithSampleData(true, false);

  // Random order, ad-hoc insert.
  createBlock(1, kSubBlockSize);
  fillBlockWithSampleData(true, true);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, InsertWithNarrowCharSortColumnTest) {
  // Non-random, batch insert.
  createBlock(2, kSubBlockSize);
  fillBlockWithSampleData(false, false);

  // Non-random, ad-hoc insert.
  createBlock(2, kSubBlockSize);
  fillBlockWithSampleData(false, true);

  // Random order, batch insert.
  createBlock(2, kSubBlockSize);
  fillBlockWithSampleData(true, false);

  // Random order, ad-hoc insert.
  createBlock(2, kSubBlockSize);
  fillBlockWithSampleData(true, true);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, InsertWithWideCharSortColumnTest) {
  // Non-random, batch insert.
  createBlock(3, kSubBlockSize);
  fillBlockWithSampleData(false, false);

  // Non-random, ad-hoc insert.
  createBlock(3, kSubBlockSize);
  fillBlockWithSampleData(false, true);

  // Random order, batch insert.
  createBlock(3, kSubBlockSize);
  fillBlockWithSampleData(true, false);

  // Random order, ad-hoc insert.
  createBlock(3, kSubBlockSize);
  fillBlockWithSampleData(true, true);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, GetValuesWithNoSortColumnTest) {
  createBlock(kInvalidCatalogId, kSubBlockSize);
  fillBlockWithSampleData(true, false);
  checkBlockValues(kInvalidCatalogId, 0, 0);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, GetValuesWithIntSortColumnTest) {
  createBlock(0, kSubBlockSize);
  fillBlockWithSampleData(true, false);
  checkBlockValues(0, 0, 0);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, GetValuesWithDoubleSortColumnTest) {
  createBlock(1, kSubBlockSize);
  fillBlockWithSampleData(true, false);
  checkBlockValues(1, 0, 0);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, GetValuesWithNarrowCharSortColumnTest) {
  createBlock(2, kSubBlockSize);
  fillBlockWithSampleData(true, false);
  checkBlockValues(2, 0, 0);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, GetValuesWithWideCharSortColumnTest) {
  createBlock(3, kSubBlockSize);
  fillBlockWithSampleData(true, false);
  checkBlockValues(3, 0, 0);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, DeleteWithNoSortColumnTest) {
  runDeleteTest(kInvalidCatalogId);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, DeleteWithIntSortColumnTest) {
  runDeleteTest(0);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, DeleteWithDoubleSortColumnTest) {
  runDeleteTest(1);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, DeleteWithNarrowCharSortColumnTest) {
  runDeleteTest(2);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, DeleteWithWideCharSortColumnTest) {
  runDeleteTest(3);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, GetMatchesForPredicateWithNoSortColumnTest) {
  runCheckPredicateTest(kInvalidCatalogId);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, GetMatchesForPredicateWithIntSortColumnTest) {
  runCheckPredicateTest(0);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, GetMatchesForPredicateWithDoubleSortColumnTest) {
  runCheckPredicateTest(1);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, GetMatchesForPredicateWithNarrowCharSortColumnTest) {
  runCheckPredicateTest(2);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, GetMatchesForPredicateWithWideCharSortColumnTest) {
  runCheckPredicateTest(3);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, SetAttributeValueInPlaceTypedWithNoSortColumnTest) {
  runSetAttributeValueInPlaceTypedTest(kInvalidCatalogId);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, SetAttributeValueInPlaceTypedWithIntSortColumnTest) {
  runSetAttributeValueInPlaceTypedTest(0);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, SetAttributeValueInPlaceTypedWithDoubleSortColumnTest) {
  runSetAttributeValueInPlaceTypedTest(1);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, SetAttributeValueInPlaceTypedWithNarrowCharSortColumnTest) {
  runSetAttributeValueInPlaceTypedTest(2);
}

TEST_P(BasicColumnStoreTupleStorageSubBlockTest, SetAttributeValueInPlaceTypedWithWideCharSortColumnTest) {
  runSetAttributeValueInPlaceTypedTest(3);
}

TEST(BasicColumnStoreTupleStorageSubBlockNullTypeTest, NullTypeTest) {
  // Set up a relation with an int attribute (need a sortable attribute to use
  // BasicColumnStore) and a single NullType attribute.
  CatalogRelation test_relation(nullptr, "TestRelation");
  CatalogAttribute *int_attr = new CatalogAttribute(&test_relation,
                                                    "int_attr",
                                                    TypeFactory::GetType(kInt, false));
  ASSERT_EQ(0, test_relation.addAttribute(int_attr));
  CatalogAttribute *nulltype_attr = new CatalogAttribute(&test_relation,
                                                         "nulltype_attr",
                                                         TypeFactory::GetType(kNullType, true));
  ASSERT_EQ(1, test_relation.addAttribute(nulltype_attr));

  // Set up a minimal StorageBlockLayoutDescription.
  StorageBlockLayoutDescription layout_desc;
  layout_desc.set_num_slots(1);
  layout_desc.mutable_tuple_store_description()->set_sub_block_type(
      TupleStorageSubBlockDescription::BASIC_COLUMN_STORE);
  layout_desc.mutable_tuple_store_description()->SetExtension(
      BasicColumnStoreTupleStorageSubBlockDescription::sort_attribute_id,
      0);

  // Check that the description is considered valid.
  EXPECT_TRUE(StorageBlockLayout::DescriptionIsValid(test_relation, layout_desc));

  StorageBlockLayout layout(test_relation, layout_desc);

  // Construct an actual PackedRowStoreTupleStorageSubBlock.
  ScopedBuffer tuple_store_memory(kSlotSizeBytes);
  BasicColumnStoreTupleStorageSubBlock tuple_store(test_relation,
                                                   layout_desc.tuple_store_description(),
                                                   true,
                                                   tuple_store_memory.get(),
                                                   kSlotSizeBytes);

  // Insert some NullType values.
  std::vector<TypedValue> attr_values;
  attr_values.emplace_back(0);
  attr_values.emplace_back(kNullType);
  Tuple tuple(std::move(attr_values));

  for (tuple_id tid = 0; tid < 100; ++tid) {
    tuple_store.insertTuple(tuple);
  }

  EXPECT_EQ(100, tuple_store.numTuples());

  // Read out values.
  for (tuple_id tid = 0; tid < 100; ++tid) {
    ASSERT_TRUE(tuple_store.hasTupleWithID(tid));
    EXPECT_EQ(nullptr, tuple_store.getAttributeValue(tid, 1));

    TypedValue value = tuple_store.getAttributeValueTyped(tid, 1);
    EXPECT_TRUE(value.isNull());
    EXPECT_EQ(kNullType, value.getTypeID());
  }

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

  // Read out the remaining undeleted values.
  for (tuple_id tid = 0; tid < 95; ++tid) {
    ASSERT_TRUE(tuple_store.hasTupleWithID(tid));
    EXPECT_EQ(nullptr, tuple_store.getAttributeValue(tid, 1));

    TypedValue value = tuple_store.getAttributeValueTyped(tid, 1);
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
                        BasicColumnStoreTupleStorageSubBlockTest,
                        ::testing::Bool(),);  // NOLINT(whitespace/comma)

INSTANTIATE_TEST_CASE_P(WithAndWithoutNullableAttributes,
                        BasicColumnStoreTupleStorageSubBlockDeathTest,
                        ::testing::Bool(),);  // NOLINT(whitespace/comma)

}  // namespace quickstep
