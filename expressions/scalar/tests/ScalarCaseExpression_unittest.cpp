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
#include <cstdio>
#include <cstring>
#include <memory>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "expressions/predicate/ComparisonPredicate.hpp"
#include "expressions/predicate/ConjunctionPredicate.hpp"
#include "expressions/predicate/DisjunctionPredicate.hpp"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "expressions/scalar/ScalarBinaryExpression.hpp"
#include "expressions/scalar/ScalarCaseExpression.hpp"
#include "expressions/scalar/ScalarLiteral.hpp"
#include "expressions/scalar/ScalarUnaryExpression.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/ValueAccessor.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "types/operations/binary_operations/BinaryOperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "types/operations/unary_operations/UnaryOperationFactory.hpp"
#include "types/operations/unary_operations/UnaryOperationID.hpp"

#include "gtest/gtest.h"

// NetBSD's libc has snprintf(), but it doesn't show up in the std namespace
// for C++.
#ifndef __NetBSD__
using std::snprintf;
#endif

namespace quickstep {

class Type;

class ScalarCaseExpressionTest : public ::testing::Test {
 protected:
  static constexpr std::size_t kNumSampleTuples = 1000;

  virtual void SetUp() {
    sample_relation_.reset(new CatalogRelation(nullptr, "sample", 0));
    sample_relation_->addAttribute(new CatalogAttribute(
        sample_relation_.get(),
        "int_attr",
        TypeFactory::GetType(kInt, true)));
    sample_relation_->addAttribute(new CatalogAttribute(
        sample_relation_.get(),
        "double_attr",
        TypeFactory::GetType(kDouble, true)));
    sample_relation_->addAttribute(new CatalogAttribute(
        sample_relation_.get(),
        "varchar_attr",
        TypeFactory::GetType(kVarChar, 20, true)));

    std::unique_ptr<NativeColumnVector> int_column(
        new NativeColumnVector(TypeFactory::GetType(kInt, true), kNumSampleTuples));
    std::unique_ptr<NativeColumnVector> double_column(
        new NativeColumnVector(TypeFactory::GetType(kDouble, true), kNumSampleTuples));
    std::unique_ptr<IndirectColumnVector> varchar_column(
        new IndirectColumnVector(TypeFactory::GetType(kVarChar, 20, true), kNumSampleTuples));

    for (std::size_t tuple_num = 0; tuple_num < kNumSampleTuples; ++tuple_num) {
      if (tuple_num % 10 == 0) {
        int_column->appendNullValue();
      } else {
        int int_value = tuple_num;
        int_column->appendUntypedValue(&int_value);
      }

      if (tuple_num % 10 == 1) {
        double_column->appendNullValue();
      } else {
        double double_value = -static_cast<double>(tuple_num);
        double_column->appendUntypedValue(&double_value);
      }

      if (tuple_num % 10 == 2) {
        TypedValue varchar_value(kVarChar);
        varchar_column->appendTypedValue(std::move(varchar_value));
      } else {
        char varchar_buffer[20];
        int written = snprintf(varchar_buffer,
                               sizeof(varchar_buffer),
                               "aa%05dzz",
                               static_cast<int>(tuple_num));
        ASSERT_LT(written, 20);
        TypedValue varchar_value(kVarChar, varchar_buffer, std::strlen(varchar_buffer) + 1);
        varchar_value.ensureNotReference();
        varchar_column->appendTypedValue(std::move(varchar_value));
      }
    }

    sample_data_value_accessor_.addColumn(int_column.release());
    sample_data_value_accessor_.addColumn(double_column.release());
    sample_data_value_accessor_.addColumn(varchar_column.release());
  }

  std::unique_ptr<CatalogRelation> sample_relation_;
  ColumnVectorsValueAccessor sample_data_value_accessor_;
};

constexpr std::size_t ScalarCaseExpressionTest::kNumSampleTuples;

// Test a CASE expression that always goes to the same branch.
TEST_F(ScalarCaseExpressionTest, StaticBranchTest) {
  std::vector<std::unique_ptr<Predicate>> when_predicates;
  std::vector<std::unique_ptr<Scalar>> result_expressions;

  const Type &int_type = TypeFactory::GetType(kInt);

  // WHEN 1 > 2 THEN int_attr
  when_predicates.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kGreater),
      new ScalarLiteral(TypedValue(static_cast<int>(1)), int_type),
      new ScalarLiteral(TypedValue(static_cast<int>(2)), int_type)));
  result_expressions.emplace_back(
      new ScalarAttribute(*sample_relation_->getAttributeById(0)));

  // WHEN 2 > 1 THEN 42
  when_predicates.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kGreater),
      new ScalarLiteral(TypedValue(static_cast<int>(2)), int_type),
      new ScalarLiteral(TypedValue(static_cast<int>(1)), int_type)));
  result_expressions.emplace_back(
      new ScalarLiteral(TypedValue(static_cast<int>(42)), int_type));

  // WHEN int_attr > 50 THEN -int_attr
  when_predicates.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kGreater),
      new ScalarAttribute(*sample_relation_->getAttributeById(0)),
      new ScalarLiteral(TypedValue(static_cast<int>(50)), int_type)));
  result_expressions.emplace_back(new ScalarUnaryExpression(
      UnaryOperationFactory::GetUnaryOperation(UnaryOperationID::kNegate),
      new ScalarAttribute(*sample_relation_->getAttributeById(0))));

  // ELSE 123
  ScalarCaseExpression case_expr(
      TypeFactory::GetType(kInt, true),
      std::move(when_predicates),
      std::move(result_expressions),
      new ScalarLiteral(TypedValue(static_cast<int>(123)), int_type));

  ASSERT_TRUE(case_expr.hasStaticValue());
  ASSERT_EQ(kInt, case_expr.getStaticValue().getTypeID());
  ASSERT_FALSE(case_expr.getStaticValue().isNull());
  EXPECT_EQ(42, case_expr.getStaticValue().getLiteral<int>());
}

TEST_F(ScalarCaseExpressionTest, BasicComparisonAndLiteralTest) {
  static const char kFirstLawString[]
      = "A robot may not injure a human being or, through inaction, allow a "
        "human being to come to harm.";
  static const char kSecondLawString[]
      = "A robot must obey orders given it by human beings except where such "
        "orders would conflict with the First Law.";
  static const char kThirdLawString[]
      = "A robot must protect its own existence as long as such protection "
        "does not conflict with the First or Second Law.";

  std::vector<std::unique_ptr<Predicate>> when_predicates;
  std::vector<std::unique_ptr<Scalar>> result_expressions;

  const Type &int_type = TypeFactory::GetType(kInt);
  const Type &double_type = TypeFactory::GetType(kDouble);
  // Maximum length of 160 chars is longer than is needed for any of the
  // sample strings.
  const Type &varchar_type = TypeFactory::GetType(kVarChar, 160, false);

  // WHEN int_attr < [kNumSampleTuples / 2] THEN [kFirstLawString]
  when_predicates.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kLess),
      new ScalarAttribute(*sample_relation_->getAttributeById(0)),
      new ScalarLiteral(TypedValue(static_cast<int>(kNumSampleTuples / 2)), int_type)));
  result_expressions.emplace_back(new ScalarLiteral(
      TypedValue(kVarChar, kFirstLawString, std::strlen(kFirstLawString) + 1),
      varchar_type));

  // WHEN double_attr > [-kNumSampleTuples * 3.0 / 4.0] THEN [kSecondLawString]
  when_predicates.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kGreater),
      new ScalarAttribute(*sample_relation_->getAttributeById(1)),
      new ScalarLiteral(TypedValue(-static_cast<double>(kNumSampleTuples) * 3.0 / 4.0), double_type)));
  result_expressions.emplace_back(new ScalarLiteral(
      TypedValue(kVarChar, kSecondLawString, std::strlen(kSecondLawString) + 1),
      varchar_type));

  // ELSE [kThirdLawString]
  ScalarCaseExpression case_expr(
      TypeFactory::GetType(kVarChar, 160, false),
      std::move(when_predicates),
      std::move(result_expressions),
      new ScalarLiteral(TypedValue(kVarChar, kThirdLawString, std::strlen(kThirdLawString) + 1),
                        varchar_type));

  ColumnVectorPtr result_cv(
      case_expr.getAllValues(&sample_data_value_accessor_, nullptr, nullptr));
  ASSERT_FALSE(result_cv->isNative());
  const IndirectColumnVector &indirect_result_cv
      = static_cast<const IndirectColumnVector&>(*result_cv);
  EXPECT_EQ(kNumSampleTuples, indirect_result_cv.size());

  for (std::size_t tuple_num = 0;
       tuple_num < indirect_result_cv.size();
       ++tuple_num) {
    if ((tuple_num % 10 != 0) && (tuple_num < kNumSampleTuples / 2)) {
      EXPECT_STREQ(kFirstLawString,
                   static_cast<const char*>(indirect_result_cv.getUntypedValue(tuple_num)));
    } else if ((tuple_num % 10 != 1) && (tuple_num < kNumSampleTuples * 3 / 4)) {
      EXPECT_STREQ(kSecondLawString,
                   static_cast<const char*>(indirect_result_cv.getUntypedValue(tuple_num)));
    } else {
      EXPECT_STREQ(kThirdLawString,
                   static_cast<const char*>(indirect_result_cv.getUntypedValue(tuple_num)));
    }
  }
}

// Same as above, but use a TupleIdSequenceAdapterValueAccessor that filters
// the input.
TEST_F(ScalarCaseExpressionTest, BasicComparisonAndLiteralWithFilteredInputTest) {
  // Filter out every seventh tuple.
  TupleIdSequence filter_sequence(kNumSampleTuples);
  std::size_t num_filtered_tuples = 0;
  for (std::size_t tuple_num = 0;
       tuple_num < kNumSampleTuples;
       ++tuple_num) {
    if (tuple_num % 7 != 0) {
      filter_sequence.set(tuple_num, true);
      ++num_filtered_tuples;
    }
  }

  std::unique_ptr<ValueAccessor> filtered_accessor(
      sample_data_value_accessor_.createSharedTupleIdSequenceAdapter(filter_sequence));

  static const char kFirstLawString[]
      = "A robot may not injure a human being or, through inaction, allow a "
        "human being to come to harm.";
  static const char kSecondLawString[]
      = "A robot must obey orders given it by human beings except where such "
        "orders would conflict with the First Law.";
  static const char kThirdLawString[]
      = "A robot must protect its own existence as long as such protection "
        "does not conflict with the First or Second Law.";

  std::vector<std::unique_ptr<Predicate>> when_predicates;
  std::vector<std::unique_ptr<Scalar>> result_expressions;

  const Type &int_type = TypeFactory::GetType(kInt);
  const Type &double_type = TypeFactory::GetType(kDouble);
  const Type &varchar_type = TypeFactory::GetType(kVarChar, 160, false);

  // WHEN int_attr < [kNumSampleTuples / 2] THEN [kFirstLawString]
  when_predicates.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kLess),
      new ScalarAttribute(*sample_relation_->getAttributeById(0)),
      new ScalarLiteral(TypedValue(static_cast<int>(kNumSampleTuples / 2)), int_type)));
  result_expressions.emplace_back(new ScalarLiteral(
      TypedValue(kVarChar, kFirstLawString, std::strlen(kFirstLawString) + 1),
      varchar_type));

  // WHEN double_attr > [kNumSampleTuples * 3.0 / 4.0] THEN [kSecondLawString]
  when_predicates.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kGreater),
      new ScalarAttribute(*sample_relation_->getAttributeById(1)),
      new ScalarLiteral(TypedValue(-static_cast<double>(kNumSampleTuples) * 3.0 / 4.0),
                        double_type)));
  result_expressions.emplace_back(new ScalarLiteral(
      TypedValue(kVarChar, kSecondLawString, std::strlen(kSecondLawString) + 1),
      varchar_type));

  // ELSE [kThirdLawString]
  ScalarCaseExpression case_expr(
      TypeFactory::GetType(kVarChar, 160, false),
      std::move(when_predicates),
      std::move(result_expressions),
      new ScalarLiteral(TypedValue(kVarChar, kThirdLawString, std::strlen(kThirdLawString) + 1),
                        varchar_type));

  ColumnVectorPtr result_cv(
      case_expr.getAllValues(filtered_accessor.get(),
                             nullptr /* sub_blocks_ref */,
                             nullptr /* cv_cache */));
  ASSERT_FALSE(result_cv->isNative());
  const IndirectColumnVector &indirect_result_cv
      = static_cast<const IndirectColumnVector&>(*result_cv);
  EXPECT_EQ(num_filtered_tuples, indirect_result_cv.size());

  std::size_t original_tuple_num = 0;
  for (std::size_t tuple_num = 0;
       tuple_num < indirect_result_cv.size();
       ++tuple_num, ++original_tuple_num) {
    if (original_tuple_num % 7 == 0) {
      ++original_tuple_num;
    }

    if ((original_tuple_num % 10 != 0) && (original_tuple_num < kNumSampleTuples / 2)) {
      EXPECT_STREQ(kFirstLawString,
                   static_cast<const char*>(indirect_result_cv.getUntypedValue(tuple_num)));
    } else if ((original_tuple_num % 10 != 1) && (original_tuple_num < kNumSampleTuples * 3 / 4)) {
      EXPECT_STREQ(kSecondLawString,
                   static_cast<const char*>(indirect_result_cv.getUntypedValue(tuple_num)));
    } else {
      EXPECT_STREQ(kThirdLawString,
                   static_cast<const char*>(indirect_result_cv.getUntypedValue(tuple_num)));
    }
  }
}

// If a tuple matches multiple WHEN clauses, get the value from the first one.
TEST_F(ScalarCaseExpressionTest, WhenClauseOrderTest) {
  static const char kFirstLawString[]
      = "A robot may not injure a human being or, through inaction, allow a "
        "human being to come to harm.";
  static const char kSecondLawString[]
      = "A robot must obey orders given it by human beings except where such "
        "orders would conflict with the First Law.";
  static const char kThirdLawString[]
      = "A robot must protect its own existence as long as such protection "
        "does not conflict with the First or Second Law.";

  std::vector<std::unique_ptr<Predicate>> when_predicates;
  std::vector<std::unique_ptr<Scalar>> result_expressions;

  const Type &int_type = TypeFactory::GetType(kInt);
  const Type &varchar_type = TypeFactory::GetType(kVarChar, 160, false);

  // WHEN int_attr < 500 THEN [kFirstLawString]
  when_predicates.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kLess),
      new ScalarAttribute(*sample_relation_->getAttributeById(0)),
      new ScalarLiteral(TypedValue(static_cast<int>(500)), int_type)));
  result_expressions.emplace_back(new ScalarLiteral(
      TypedValue(kVarChar, kFirstLawString, std::strlen(kFirstLawString) + 1),
      varchar_type));

  // WHEN int_attr < 750 THEN [kSecondLawString]
  when_predicates.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kLess),
      new ScalarAttribute(*sample_relation_->getAttributeById(0)),
      new ScalarLiteral(TypedValue(static_cast<int>(750)), int_type)));
  result_expressions.emplace_back(new ScalarLiteral(
      TypedValue(kVarChar, kSecondLawString, std::strlen(kSecondLawString) + 1),
      varchar_type));

  // ELSE [kThirdLawString]
  ScalarCaseExpression case_expr(
      TypeFactory::GetType(kVarChar, 160, false),
      std::move(when_predicates),
      std::move(result_expressions),
      new ScalarLiteral(TypedValue(kVarChar, kThirdLawString, std::strlen(kThirdLawString) + 1),
                        varchar_type));

  ColumnVectorPtr result_cv(
      case_expr.getAllValues(&sample_data_value_accessor_,
                             nullptr /* sub_blocks_ref */,
                             nullptr /* cv_cache */));
  ASSERT_FALSE(result_cv->isNative());
  const IndirectColumnVector &indirect_result_cv
      = static_cast<const IndirectColumnVector&>(*result_cv);
  EXPECT_EQ(kNumSampleTuples, indirect_result_cv.size());

  for (std::size_t tuple_num = 0;
       tuple_num < indirect_result_cv.size();
       ++tuple_num) {
    if ((tuple_num % 10 != 0) && (tuple_num < 500)) {
      EXPECT_STREQ(kFirstLawString,
                   static_cast<const char*>(indirect_result_cv.getUntypedValue(tuple_num)));
    } else if ((tuple_num % 10 != 0) && (tuple_num < 750)) {
      EXPECT_STREQ(kSecondLawString,
                   static_cast<const char*>(indirect_result_cv.getUntypedValue(tuple_num)));
    } else {
      EXPECT_STREQ(kThirdLawString,
                   static_cast<const char*>(indirect_result_cv.getUntypedValue(tuple_num)));
    }
  }
}

TEST_F(ScalarCaseExpressionTest, ComplexConjunctionAndCalculatedExpressionTest) {
  const Type &int_type = TypeFactory::GetType(kInt);
  const Type &varchar_type = TypeFactory::GetType(kVarChar, 20, false);

  std::vector<std::unique_ptr<Predicate>> when_predicates;
  std::vector<std::unique_ptr<Scalar>> result_expressions;

  // WHEN (int_attr * 2) + double_attr < 500 AND varchar_attr > 'aa002'
  // THEN -int_attr
  static const char kStringLit1[] = "aa002";
  ConjunctionPredicate *first_case_predicate = new ConjunctionPredicate();
  first_case_predicate->addPredicate(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kLess),
      new ScalarBinaryExpression(
          BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd),
          new ScalarBinaryExpression(
              BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kMultiply),
              new ScalarAttribute(*sample_relation_->getAttributeById(0)),
              new ScalarLiteral(TypedValue(2), int_type)),
          new ScalarAttribute(*sample_relation_->getAttributeById(1))),
      new ScalarLiteral(TypedValue(static_cast<int>(kNumSampleTuples / 2)), int_type)));
  first_case_predicate->addPredicate(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kGreater),
      new ScalarAttribute(*sample_relation_->getAttributeById(2)),
      new ScalarLiteral(TypedValue(kVarChar, kStringLit1, std::strlen(kStringLit1) + 1), varchar_type)));
  when_predicates.emplace_back(first_case_predicate);

  result_expressions.emplace_back(new ScalarUnaryExpression(
      UnaryOperationFactory::GetUnaryOperation(UnaryOperationID::kNegate),
      new ScalarAttribute(*sample_relation_->getAttributeById(0))));

  // WHEN int_attr >= 50 AND (CASE WHEN varchar_attr < 'aa007' THEN 1 ELSE -1) > 0
  // THEN int_attr + 10
  static const char kStringLit2[] = "aa007";
  ConjunctionPredicate *second_case_predicate = new ConjunctionPredicate();
  second_case_predicate->addPredicate(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kGreaterOrEqual),
      new ScalarAttribute(*sample_relation_->getAttributeById(0)),
      new ScalarLiteral(TypedValue(50), int_type)));
  std::vector<std::unique_ptr<Predicate>> subcase_preds;
  subcase_preds.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kLess),
      new ScalarAttribute(*sample_relation_->getAttributeById(2)),
      new ScalarLiteral(TypedValue(kVarChar, kStringLit2, std::strlen(kStringLit2) + 1), varchar_type)));
  std::vector<std::unique_ptr<Scalar>> subcase_results;
  subcase_results.emplace_back(new ScalarLiteral(TypedValue(1), int_type));
  second_case_predicate->addPredicate(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kGreater),
      new ScalarCaseExpression(
          TypeFactory::GetType(kInt, false),
          std::move(subcase_preds),
          std::move(subcase_results),
          new ScalarLiteral(TypedValue(-1), int_type)),
      new ScalarLiteral(TypedValue(0), int_type)));
  when_predicates.emplace_back(second_case_predicate);

  result_expressions.emplace_back(new ScalarBinaryExpression(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd),
      new ScalarAttribute(*sample_relation_->getAttributeById(0)),
      new ScalarLiteral(TypedValue(10), int_type)));

  // ELSE 5 * int_attr
  ScalarCaseExpression case_expr(
      TypeFactory::GetType(kInt, true),
      std::move(when_predicates),
      std::move(result_expressions),
      new ScalarBinaryExpression(
          BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kMultiply),
          new ScalarLiteral(TypedValue(5), int_type),
          new ScalarAttribute(*sample_relation_->getAttributeById(0))));

  ColumnVectorPtr result_cv(
      case_expr.getAllValues(&sample_data_value_accessor_,
                             nullptr /* sub_blocks_ref */,
                             nullptr /* cv_cache */));
  ASSERT_TRUE(result_cv->isNative());
  const NativeColumnVector &native_result_cv
      = static_cast<const NativeColumnVector&>(*result_cv);
  EXPECT_EQ(kNumSampleTuples, native_result_cv.size());

  for (std::size_t tuple_num = 0;
       tuple_num < native_result_cv.size();
       ++tuple_num) {
    if ((tuple_num % 10 != 0)
        && (tuple_num % 10 != 1)
        && (tuple_num % 10 != 2)
        && (tuple_num < 500)
        && (tuple_num >= 200)) {
      ASSERT_NE(nullptr, native_result_cv.getUntypedValue(tuple_num));
      EXPECT_EQ(-static_cast<int>(tuple_num),
                *static_cast<const int*>(native_result_cv.getUntypedValue(tuple_num)));
    } else if ((tuple_num % 10 != 0)
               && (tuple_num % 10 != 2)
               && (tuple_num >= 50 && tuple_num < 700)) {
      ASSERT_NE(nullptr, native_result_cv.getUntypedValue(tuple_num));
      EXPECT_EQ(static_cast<int>(tuple_num) + 10,
                *static_cast<const int*>(native_result_cv.getUntypedValue(tuple_num)));
    } else if (tuple_num % 10 == 0) {
      EXPECT_EQ(nullptr, native_result_cv.getUntypedValue(tuple_num));
    } else {
      ASSERT_NE(nullptr, native_result_cv.getUntypedValue(tuple_num));
      EXPECT_EQ(static_cast<int>(tuple_num) * 5,
                *static_cast<const int*>(native_result_cv.getUntypedValue(tuple_num)));
    }
  }
}

// Same as above, but use a TupleIdSequenceAdapterValueAccessor that filters
// the input.
TEST_F(ScalarCaseExpressionTest,
       ComplexConjunctionAndCalculatedExpressionWithFilteredInputTest) {
  // Filter out every seventh tuple.
  TupleIdSequence filter_sequence(kNumSampleTuples);
  std::size_t num_filtered_tuples = 0;
  for (std::size_t tuple_num = 0;
       tuple_num < kNumSampleTuples;
       ++tuple_num) {
    if (tuple_num % 7 != 0) {
      filter_sequence.set(tuple_num, true);
      ++num_filtered_tuples;
    }
  }

  std::unique_ptr<ValueAccessor> filtered_accessor(
      sample_data_value_accessor_.createSharedTupleIdSequenceAdapter(filter_sequence));

  std::vector<std::unique_ptr<Predicate>> when_predicates;
  std::vector<std::unique_ptr<Scalar>> result_expressions;

  const Type &int_type = TypeFactory::GetType(kInt);
  const Type &varchar_type = TypeFactory::GetType(kVarChar, 20, false);

  // WHEN (int_attr * 2) + double_attr < 500 AND varchar_attr > 'aa002'
  // THEN -int_attr
  static const char kStringLit1[] = "aa002";
  ConjunctionPredicate *first_case_predicate = new ConjunctionPredicate();
  first_case_predicate->addPredicate(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kLess),
      new ScalarBinaryExpression(
          BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd),
          new ScalarBinaryExpression(
              BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kMultiply),
              new ScalarAttribute(*sample_relation_->getAttributeById(0)),
              new ScalarLiteral(TypedValue(2), int_type)),
          new ScalarAttribute(*sample_relation_->getAttributeById(1))),
      new ScalarLiteral(TypedValue(static_cast<int>(kNumSampleTuples / 2)), int_type)));
  first_case_predicate->addPredicate(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kGreater),
      new ScalarAttribute(*sample_relation_->getAttributeById(2)),
      new ScalarLiteral(TypedValue(kVarChar, kStringLit1, std::strlen(kStringLit1) + 1), varchar_type)));
  when_predicates.emplace_back(first_case_predicate);

  result_expressions.emplace_back(new ScalarUnaryExpression(
      UnaryOperationFactory::GetUnaryOperation(UnaryOperationID::kNegate),
      new ScalarAttribute(*sample_relation_->getAttributeById(0))));

  // WHEN int_attr >= 50 AND (CASE WHEN varchar_attr < 'aa007' THEN 1 ELSE -1) > 0
  // THEN int_attr + 10
  static const char kStringLit2[] = "aa007";
  ConjunctionPredicate *second_case_predicate = new ConjunctionPredicate();
  second_case_predicate->addPredicate(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kGreaterOrEqual),
      new ScalarAttribute(*sample_relation_->getAttributeById(0)),
      new ScalarLiteral(TypedValue(50), int_type)));
  std::vector<std::unique_ptr<Predicate>> subcase_preds;
  subcase_preds.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kLess),
      new ScalarAttribute(*sample_relation_->getAttributeById(2)),
      new ScalarLiteral(TypedValue(kVarChar, kStringLit2, std::strlen(kStringLit2) + 1), varchar_type)));
  std::vector<std::unique_ptr<Scalar>> subcase_results;
  subcase_results.emplace_back(new ScalarLiteral(TypedValue(1), int_type));
  second_case_predicate->addPredicate(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kGreater),
      new ScalarCaseExpression(
          TypeFactory::GetType(kInt, false),
          std::move(subcase_preds),
          std::move(subcase_results),
          new ScalarLiteral(TypedValue(-1), int_type)),
      new ScalarLiteral(TypedValue(0), int_type)));
  when_predicates.emplace_back(second_case_predicate);

  result_expressions.emplace_back(new ScalarBinaryExpression(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd),
      new ScalarAttribute(*sample_relation_->getAttributeById(0)),
      new ScalarLiteral(TypedValue(10), int_type)));

  // ELSE 5 * int_attr
  ScalarCaseExpression case_expr(
      TypeFactory::GetType(kInt, true),
      std::move(when_predicates),
      std::move(result_expressions),
      new ScalarBinaryExpression(
          BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kMultiply),
          new ScalarLiteral(TypedValue(5), int_type),
          new ScalarAttribute(*sample_relation_->getAttributeById(0))));

  ColumnVectorPtr result_cv(
      case_expr.getAllValues(filtered_accessor.get(),
                             nullptr /* sub_blocks_ref */,
                             nullptr /* cv_cache */));
  ASSERT_TRUE(result_cv->isNative());
  const NativeColumnVector &native_result_cv
      = static_cast<const NativeColumnVector&>(*result_cv);
  EXPECT_EQ(num_filtered_tuples, native_result_cv.size());

  std::size_t original_tuple_num = 0;
  for (std::size_t tuple_num = 0;
       tuple_num < native_result_cv.size();
       ++tuple_num, ++original_tuple_num) {
    if (original_tuple_num % 7 == 0) {
      ++original_tuple_num;
    }

    if ((original_tuple_num % 10 != 0)
        && (original_tuple_num % 10 != 1)
        && (original_tuple_num % 10 != 2)
        && (original_tuple_num < 500)
        && (original_tuple_num >= 200)) {
      ASSERT_NE(nullptr, native_result_cv.getUntypedValue(tuple_num));
      EXPECT_EQ(-static_cast<int>(original_tuple_num),
                *static_cast<const int*>(native_result_cv.getUntypedValue(tuple_num)));
    } else if ((original_tuple_num % 10 != 0)
               && (original_tuple_num % 10 != 2)
               && (original_tuple_num >= 50 && original_tuple_num < 700)) {
      ASSERT_NE(nullptr, native_result_cv.getUntypedValue(tuple_num));
      EXPECT_EQ(static_cast<int>(original_tuple_num) + 10,
                *static_cast<const int*>(native_result_cv.getUntypedValue(tuple_num)));
    } else if (original_tuple_num % 10 == 0) {
      EXPECT_EQ(nullptr, native_result_cv.getUntypedValue(tuple_num));
    } else {
      ASSERT_NE(nullptr, native_result_cv.getUntypedValue(tuple_num));
      EXPECT_EQ(static_cast<int>(original_tuple_num) * 5,
                *static_cast<const int*>(native_result_cv.getUntypedValue(tuple_num)));
    }
  }
}

TEST_F(ScalarCaseExpressionTest, ComplexDisjunctionAndCalculatedExpressionTest) {
  const Type &int_type = TypeFactory::GetType(kInt);
  const Type &varchar_type = TypeFactory::GetType(kVarChar, 20, false);

  std::vector<std::unique_ptr<Predicate>> when_predicates;
  std::vector<std::unique_ptr<Scalar>> result_expressions;

  // WHEN (int_attr * 2) + double_attr > 750 OR varchar_attr < 'aa002'
  // THEN -int_attr
  static const char kStringLit1[] = "aa002";
  DisjunctionPredicate *first_case_predicate = new DisjunctionPredicate();
  first_case_predicate->addPredicate(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kGreater),
      new ScalarBinaryExpression(
          BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd),
          new ScalarBinaryExpression(
              BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kMultiply),
              new ScalarAttribute(*sample_relation_->getAttributeById(0)),
              new ScalarLiteral(TypedValue(2), int_type)),
          new ScalarAttribute(*sample_relation_->getAttributeById(1))),
      new ScalarLiteral(TypedValue(750), int_type)));
  first_case_predicate->addPredicate(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kLess),
      new ScalarAttribute(*sample_relation_->getAttributeById(2)),
      new ScalarLiteral(TypedValue(kVarChar, kStringLit1, std::strlen(kStringLit1) + 1), varchar_type)));
  when_predicates.emplace_back(first_case_predicate);

  result_expressions.emplace_back(new ScalarUnaryExpression(
      UnaryOperationFactory::GetUnaryOperation(UnaryOperationID::kNegate),
      new ScalarAttribute(*sample_relation_->getAttributeById(0))));

  // WHEN int_attr < 350 OR (CASE WHEN varchar_attr < 'aa006' THEN 1 ELSE -1) < 0
  // THEN int_attr + 10
  static const char kStringLit2[] = "aa006";
  DisjunctionPredicate *second_case_predicate = new DisjunctionPredicate();
  second_case_predicate->addPredicate(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kLess),
      new ScalarAttribute(*sample_relation_->getAttributeById(0)),
      new ScalarLiteral(TypedValue(350), int_type)));
  std::vector<std::unique_ptr<Predicate>> subcase_preds;
  subcase_preds.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kLess),
      new ScalarAttribute(*sample_relation_->getAttributeById(2)),
      new ScalarLiteral(TypedValue(kVarChar, kStringLit2, std::strlen(kStringLit2) + 1), varchar_type)));
  std::vector<std::unique_ptr<Scalar>> subcase_results;
  subcase_results.emplace_back(new ScalarLiteral(TypedValue(1), int_type));
  second_case_predicate->addPredicate(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kLess),
      new ScalarCaseExpression(
          TypeFactory::GetType(kInt, false),
          std::move(subcase_preds),
          std::move(subcase_results),
          new ScalarLiteral(TypedValue(-1), int_type)),
      new ScalarLiteral(TypedValue(0), int_type)));
  when_predicates.emplace_back(second_case_predicate);

  result_expressions.emplace_back(new ScalarBinaryExpression(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd),
      new ScalarAttribute(*sample_relation_->getAttributeById(0)),
      new ScalarLiteral(TypedValue(10), int_type)));

  // ELSE 5 * int_attr
  ScalarCaseExpression case_expr(
      TypeFactory::GetType(kInt, true),
      std::move(when_predicates),
      std::move(result_expressions),
      new ScalarBinaryExpression(
          BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kMultiply),
          new ScalarLiteral(TypedValue(5), int_type),
          new ScalarAttribute(*sample_relation_->getAttributeById(0))));

  ColumnVectorPtr result_cv(
      case_expr.getAllValues(&sample_data_value_accessor_,
                             nullptr /* sub_blocks_ref */,
                             nullptr /* cv_cache */));
  ASSERT_TRUE(result_cv->isNative());
  const NativeColumnVector &native_result_cv
      = static_cast<const NativeColumnVector&>(*result_cv);
  EXPECT_EQ(kNumSampleTuples, native_result_cv.size());

  for (std::size_t tuple_num = 0;
       tuple_num < native_result_cv.size();
       ++tuple_num) {
    if (tuple_num % 10 == 0) {
      EXPECT_EQ(nullptr, native_result_cv.getUntypedValue(tuple_num));
    } else if (((tuple_num % 10 != 1) && (tuple_num > 750))
               || ((tuple_num % 10 != 2) && (tuple_num < 200))) {
      ASSERT_NE(nullptr, native_result_cv.getUntypedValue(tuple_num));
      EXPECT_EQ(-static_cast<int>(tuple_num),
                *static_cast<const int*>(native_result_cv.getUntypedValue(tuple_num)));
    } else if ((tuple_num < 350)
               || (tuple_num % 10 == 2)
               || ((tuple_num % 10 != 2) && (tuple_num >= 600))) {
      ASSERT_NE(nullptr, native_result_cv.getUntypedValue(tuple_num));
      EXPECT_EQ(static_cast<int>(tuple_num) + 10,
                *static_cast<const int*>(native_result_cv.getUntypedValue(tuple_num)));
    } else {
      ASSERT_NE(nullptr, native_result_cv.getUntypedValue(tuple_num));
      EXPECT_EQ(static_cast<int>(tuple_num) * 5,
                *static_cast<const int*>(native_result_cv.getUntypedValue(tuple_num)));
    }
  }
}

// Same as above, but use a TupleIdSequenceAdapterValueAccessor that filters
// the input.
TEST_F(ScalarCaseExpressionTest,
       ComplexDisjunctionAndCalculatedExpressionWithFilteredInputTest) {
  const Type &int_type = TypeFactory::GetType(kInt);
  const Type &varchar_type = TypeFactory::GetType(kVarChar, 20, false);

  // Filter out every seventh tuple.
  TupleIdSequence filter_sequence(kNumSampleTuples);
  std::size_t num_filtered_tuples = 0;
  for (std::size_t tuple_num = 0;
       tuple_num < kNumSampleTuples;
       ++tuple_num) {
    if (tuple_num % 7 != 0) {
      filter_sequence.set(tuple_num, true);
      ++num_filtered_tuples;
    }
  }

  std::unique_ptr<ValueAccessor> filtered_accessor(
      sample_data_value_accessor_.createSharedTupleIdSequenceAdapter(filter_sequence));

  std::vector<std::unique_ptr<Predicate>> when_predicates;
  std::vector<std::unique_ptr<Scalar>> result_expressions;

  // WHEN (int_attr * 2) + double_attr > 750 OR varchar_attr < 'aa002'
  // THEN -int_attr
  static const char kStringLit1[] = "aa002";
  DisjunctionPredicate *first_case_predicate = new DisjunctionPredicate();
  first_case_predicate->addPredicate(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kGreater),
      new ScalarBinaryExpression(
          BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd),
          new ScalarBinaryExpression(
              BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kMultiply),
              new ScalarAttribute(*sample_relation_->getAttributeById(0)),
              new ScalarLiteral(TypedValue(2), int_type)),
          new ScalarAttribute(*sample_relation_->getAttributeById(1))),
      new ScalarLiteral(TypedValue(750), int_type)));
  first_case_predicate->addPredicate(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kLess),
      new ScalarAttribute(*sample_relation_->getAttributeById(2)),
      new ScalarLiteral(TypedValue(kVarChar, kStringLit1, std::strlen(kStringLit1) + 1), varchar_type)));
  when_predicates.emplace_back(first_case_predicate);

  result_expressions.emplace_back(new ScalarUnaryExpression(
      UnaryOperationFactory::GetUnaryOperation(UnaryOperationID::kNegate),
      new ScalarAttribute(*sample_relation_->getAttributeById(0))));

  // WHEN int_attr < 350 OR (CASE WHEN varchar_attr < 'aa006' THEN 1 ELSE -1) < 0
  // THEN int_attr + 10
  static const char kStringLit2[] = "aa006";
  DisjunctionPredicate *second_case_predicate = new DisjunctionPredicate();
  second_case_predicate->addPredicate(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kLess),
      new ScalarAttribute(*sample_relation_->getAttributeById(0)),
      new ScalarLiteral(TypedValue(350), int_type)));
  std::vector<std::unique_ptr<Predicate>> subcase_preds;
  subcase_preds.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kLess),
      new ScalarAttribute(*sample_relation_->getAttributeById(2)),
      new ScalarLiteral(TypedValue(kVarChar, kStringLit2, std::strlen(kStringLit2) + 1), varchar_type)));
  std::vector<std::unique_ptr<Scalar>> subcase_results;
  subcase_results.emplace_back(new ScalarLiteral(TypedValue(1), int_type));
  second_case_predicate->addPredicate(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kLess),
      new ScalarCaseExpression(
          TypeFactory::GetType(kInt, false),
          std::move(subcase_preds),
          std::move(subcase_results),
          new ScalarLiteral(TypedValue(-1), int_type)),
      new ScalarLiteral(TypedValue(0), int_type)));
  when_predicates.emplace_back(second_case_predicate);

  result_expressions.emplace_back(new ScalarBinaryExpression(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd),
      new ScalarAttribute(*sample_relation_->getAttributeById(0)),
      new ScalarLiteral(TypedValue(10), int_type)));

  // ELSE 5 * int_attr
  ScalarCaseExpression case_expr(
      TypeFactory::GetType(kInt, true),
      std::move(when_predicates),
      std::move(result_expressions),
      new ScalarBinaryExpression(
          BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kMultiply),
          new ScalarLiteral(TypedValue(5), int_type),
          new ScalarAttribute(*sample_relation_->getAttributeById(0))));

  ColumnVectorPtr result_cv(
      case_expr.getAllValues(filtered_accessor.get(),
                             nullptr /* sub_blocks_ref */,
                             nullptr /* cv_cache */));
  ASSERT_TRUE(result_cv->isNative());
  const NativeColumnVector &native_result_cv
      = static_cast<const NativeColumnVector&>(*result_cv);
  EXPECT_EQ(num_filtered_tuples, native_result_cv.size());

  std::size_t original_tuple_num = 0;
  for (std::size_t tuple_num = 0;
       tuple_num < native_result_cv.size();
       ++tuple_num, ++original_tuple_num) {
    if (original_tuple_num % 7 == 0) {
      ++original_tuple_num;
    }

    if (original_tuple_num % 10 == 0) {
      EXPECT_EQ(nullptr, native_result_cv.getUntypedValue(tuple_num));
    } else if (((original_tuple_num % 10 != 1) && (original_tuple_num > 750))
               || ((original_tuple_num % 10 != 2) && (original_tuple_num < 200))) {
      ASSERT_NE(nullptr, native_result_cv.getUntypedValue(tuple_num));
      EXPECT_EQ(-static_cast<int>(original_tuple_num),
                *static_cast<const int*>(native_result_cv.getUntypedValue(tuple_num)));
    } else if ((original_tuple_num < 350)
               || (original_tuple_num % 10 == 2)
               || ((original_tuple_num % 10 != 2) && (original_tuple_num >= 600))) {
      ASSERT_NE(nullptr, native_result_cv.getUntypedValue(tuple_num));
      EXPECT_EQ(static_cast<int>(original_tuple_num) + 10,
                *static_cast<const int*>(native_result_cv.getUntypedValue(tuple_num)));
    } else {
      ASSERT_NE(nullptr, native_result_cv.getUntypedValue(tuple_num));
      EXPECT_EQ(static_cast<int>(original_tuple_num) * 5,
                *static_cast<const int*>(native_result_cv.getUntypedValue(tuple_num)));
    }
  }
}

// Test CASE evaluation over joins, which that always goes to the same branch
// on a constant.
TEST_F(ScalarCaseExpressionTest, JoinStaticBranchConstantTest) {
  // Simulate a join with another relation.
  CatalogRelation other_relation(nullptr, "other", 1);
  other_relation.addAttribute(new CatalogAttribute(&other_relation,
                                                   "other_double",
                                                   TypeFactory::GetType(kDouble, false)));
  other_relation.addAttribute(new CatalogAttribute(&other_relation,
                                                   "other_int",
                                                   TypeFactory::GetType(kInt, false)));

  static const double kOtherDoubleValues[] = {-250.0, -750.0};
  std::unique_ptr<NativeColumnVector> other_double_column(
      new NativeColumnVector(TypeFactory::GetType(kDouble, false), 2));
  other_double_column->appendUntypedValue(kOtherDoubleValues);
  other_double_column->appendUntypedValue(kOtherDoubleValues + 1);

  static const int kOtherIntValues[] = {10, -1};
  std::unique_ptr<NativeColumnVector> other_int_column(
      new NativeColumnVector(TypeFactory::GetType(kInt, false), 2));
  other_int_column->appendUntypedValue(kOtherIntValues);
  other_int_column->appendUntypedValue(kOtherIntValues + 1);

  ColumnVectorsValueAccessor other_accessor;
  other_accessor.addColumn(other_double_column.release());
  other_accessor.addColumn(other_int_column.release());

  const Type &int_type = TypeFactory::GetType(kInt);

  // Setup expression.
  std::vector<std::unique_ptr<Predicate>> when_predicates;
  std::vector<std::unique_ptr<Scalar>> result_expressions;

  // WHEN 1 > 2 THEN int_attr + other_int
  when_predicates.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kGreater),
      new ScalarLiteral(TypedValue(static_cast<int>(1)), int_type),
      new ScalarLiteral(TypedValue(static_cast<int>(2)), int_type)));
  result_expressions.emplace_back(new ScalarBinaryExpression(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd),
      new ScalarAttribute(*sample_relation_->getAttributeById(0)),
      new ScalarAttribute(*other_relation.getAttributeById(1))));

  const int kConstant = 72;
  // WHEN 1 < 2 THEN kConstant
  when_predicates.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kLess),
      new ScalarLiteral(TypedValue(static_cast<int>(1)), int_type),
      new ScalarLiteral(TypedValue(static_cast<int>(2)), int_type)));
  result_expressions.emplace_back(
      new ScalarLiteral(TypedValue(kConstant), int_type));

  // WHEN double_attr = other_double THEN 0
  when_predicates.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kEqual),
      new ScalarAttribute(*sample_relation_->getAttributeById(1)),
      new ScalarAttribute(*other_relation.getAttributeById(0))));
  result_expressions.emplace_back(new ScalarLiteral(TypedValue(0), TypeFactory::GetType(kInt)));

  const Type &int_nullable_type = TypeFactory::GetType(kInt, true);

  // ELSE NULL
  ScalarCaseExpression case_expr(
      int_nullable_type,
      std::move(when_predicates),
      std::move(result_expressions),
      new ScalarLiteral(TypedValue(kInt), int_nullable_type));

  // Create a list of joined tuple-id pairs (just the cross-product of tuples).
  std::vector<std::pair<tuple_id, tuple_id>> joined_tuple_ids;
  for (std::size_t tuple_num = 0; tuple_num < kNumSampleTuples; ++tuple_num) {
    joined_tuple_ids.emplace_back(tuple_num, 0);
    joined_tuple_ids.emplace_back(tuple_num, 1);
  }

  ColumnVectorPtr result_cv(case_expr.getAllValuesForJoin(
      0,
      &sample_data_value_accessor_,
      1,
      &other_accessor,
      joined_tuple_ids,
      nullptr /* cv_cache */));
  ASSERT_TRUE(result_cv->isNative());
  const NativeColumnVector &native_result_cv
      = static_cast<const NativeColumnVector&>(*result_cv);
  EXPECT_EQ(kNumSampleTuples * 2, native_result_cv.size());

  for (std::size_t result_num = 0;
       result_num < native_result_cv.size();
       ++result_num) {
    EXPECT_EQ(kConstant,
              *static_cast<const int*>(native_result_cv.getUntypedValue(result_num)));
  }
}

// Test CASE evaluation over joins, which that always goes to the same branch
// of ScalarAttribute.
TEST_F(ScalarCaseExpressionTest, JoinStaticBranchOnScalarAttributeTest) {
  // Simulate a join with another relation.
  CatalogRelation other_relation(nullptr, "other", 1);
  other_relation.addAttribute(new CatalogAttribute(&other_relation,
                                                   "other_double",
                                                   TypeFactory::GetType(kDouble, false)));
  other_relation.addAttribute(new CatalogAttribute(&other_relation,
                                                   "other_int",
                                                   TypeFactory::GetType(kInt, false)));

  static const double kOtherDoubleValues[] = {-250.0, -750.0};
  std::unique_ptr<NativeColumnVector> other_double_column(
      new NativeColumnVector(TypeFactory::GetType(kDouble, false), 2));
  other_double_column->appendUntypedValue(kOtherDoubleValues);
  other_double_column->appendUntypedValue(kOtherDoubleValues + 1);

  static const int kOtherIntValues[] = {10, -1};
  std::unique_ptr<NativeColumnVector> other_int_column(
      new NativeColumnVector(TypeFactory::GetType(kInt, false), 2));
  other_int_column->appendUntypedValue(kOtherIntValues);
  other_int_column->appendUntypedValue(kOtherIntValues + 1);

  ColumnVectorsValueAccessor other_accessor;
  other_accessor.addColumn(other_double_column.release());
  other_accessor.addColumn(other_int_column.release());

  const Type &int_type = TypeFactory::GetType(kInt);

  // Setup expression.
  std::vector<std::unique_ptr<Predicate>> when_predicates;
  std::vector<std::unique_ptr<Scalar>> result_expressions;

  // WHEN 1 > 2 THEN int_attr + other_int
  when_predicates.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kGreater),
      new ScalarLiteral(TypedValue(static_cast<int>(1)), int_type),
      new ScalarLiteral(TypedValue(static_cast<int>(2)), int_type)));
  result_expressions.emplace_back(new ScalarBinaryExpression(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd),
      new ScalarAttribute(*sample_relation_->getAttributeById(0)),
      new ScalarAttribute(*other_relation.getAttributeById(1))));

  // WHEN 1 < 2 THEN int_attr
  when_predicates.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kLess),
      new ScalarLiteral(TypedValue(static_cast<int>(1)), int_type),
      new ScalarLiteral(TypedValue(static_cast<int>(2)), int_type)));
  result_expressions.emplace_back(
      new ScalarAttribute(*sample_relation_->getAttributeById(0)));

  // WHEN double_attr = other_double THEN 0
  when_predicates.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kEqual),
      new ScalarAttribute(*sample_relation_->getAttributeById(1)),
      new ScalarAttribute(*other_relation.getAttributeById(0))));
  result_expressions.emplace_back(new ScalarLiteral(TypedValue(0), TypeFactory::GetType(kInt)));

  const Type &int_nullable_type = TypeFactory::GetType(kInt, true);

  // ELSE NULL
  ScalarCaseExpression case_expr(
      int_nullable_type,
      std::move(when_predicates),
      std::move(result_expressions),
      new ScalarLiteral(TypedValue(kInt), int_nullable_type));

  // Create a list of joined tuple-id pairs (just the cross-product of tuples).
  std::vector<std::pair<tuple_id, tuple_id>> joined_tuple_ids;
  for (std::size_t tuple_num = 0; tuple_num < kNumSampleTuples; ++tuple_num) {
    joined_tuple_ids.emplace_back(tuple_num, 0);
    joined_tuple_ids.emplace_back(tuple_num, 1);
  }

  ColumnVectorPtr result_cv(case_expr.getAllValuesForJoin(
      0,
      &sample_data_value_accessor_,
      1,
      &other_accessor,
      joined_tuple_ids,
      nullptr /* cv_cache */));
  ASSERT_TRUE(result_cv->isNative());
  const NativeColumnVector &native_result_cv
      = static_cast<const NativeColumnVector&>(*result_cv);
  EXPECT_EQ(kNumSampleTuples * 2, native_result_cv.size());

  for (std::size_t result_num = 0;
       result_num < native_result_cv.size();
       ++result_num) {
    // For convenience, calculate expected tuple values here.
    const bool sample_int_null = ((result_num >> 1) % 10 == 0);
    const int sample_int = result_num >> 1;

    if (sample_int_null) {
      EXPECT_EQ(nullptr, native_result_cv.getUntypedValue(result_num));
    } else {
      ASSERT_NE(nullptr, native_result_cv.getUntypedValue(result_num));
      EXPECT_EQ(sample_int,
                *static_cast<const int*>(native_result_cv.getUntypedValue(result_num)));
    }
  }
}

// Test CASE evaluation over joins, which that always goes to the same branch
// of ScalarBinaryExpression.
TEST_F(ScalarCaseExpressionTest, JoinStaticBranchTest) {
  // Simulate a join with another relation.
  CatalogRelation other_relation(nullptr, "other", 1);
  other_relation.addAttribute(new CatalogAttribute(&other_relation,
                                                   "other_double",
                                                   TypeFactory::GetType(kDouble, false)));
  other_relation.addAttribute(new CatalogAttribute(&other_relation,
                                                   "other_int",
                                                   TypeFactory::GetType(kInt, false)));

  static const double kOtherDoubleValues[] = {-250.0, -750.0};
  std::unique_ptr<NativeColumnVector> other_double_column(
      new NativeColumnVector(TypeFactory::GetType(kDouble, false), 2));
  other_double_column->appendUntypedValue(kOtherDoubleValues);
  other_double_column->appendUntypedValue(kOtherDoubleValues + 1);

  static const int kOtherIntValues[] = {10, -1};
  std::unique_ptr<NativeColumnVector> other_int_column(
      new NativeColumnVector(TypeFactory::GetType(kInt, false), 2));
  other_int_column->appendUntypedValue(kOtherIntValues);
  other_int_column->appendUntypedValue(kOtherIntValues + 1);

  ColumnVectorsValueAccessor other_accessor;
  other_accessor.addColumn(other_double_column.release());
  other_accessor.addColumn(other_int_column.release());

  const Type &int_type = TypeFactory::GetType(kInt);

  // Setup expression.
  std::vector<std::unique_ptr<Predicate>> when_predicates;
  std::vector<std::unique_ptr<Scalar>> result_expressions;

  // WHEN 1 > 2 THEN int_attr + other_int
  when_predicates.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kGreater),
      new ScalarLiteral(TypedValue(static_cast<int>(1)), int_type),
      new ScalarLiteral(TypedValue(static_cast<int>(2)), int_type)));
  result_expressions.emplace_back(new ScalarBinaryExpression(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd),
      new ScalarAttribute(*sample_relation_->getAttributeById(0)),
      new ScalarAttribute(*other_relation.getAttributeById(1))));

  // WHEN 1 < 2 THEN int_attr * other_int
  when_predicates.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kLess),
      new ScalarLiteral(TypedValue(static_cast<int>(1)), int_type),
      new ScalarLiteral(TypedValue(static_cast<int>(2)), int_type)));
  result_expressions.emplace_back(new ScalarBinaryExpression(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kMultiply),
      new ScalarAttribute(*sample_relation_->getAttributeById(0)),
      new ScalarAttribute(*other_relation.getAttributeById(1))));

  // WHEN double_attr = other_double THEN 0
  when_predicates.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kEqual),
      new ScalarAttribute(*sample_relation_->getAttributeById(1)),
      new ScalarAttribute(*other_relation.getAttributeById(0))));
  result_expressions.emplace_back(new ScalarLiteral(TypedValue(0), TypeFactory::GetType(kInt)));

  const Type &int_nullable_type = TypeFactory::GetType(kInt, true);

  // ELSE NULL
  ScalarCaseExpression case_expr(
      int_nullable_type,
      std::move(when_predicates),
      std::move(result_expressions),
      new ScalarLiteral(TypedValue(kInt), int_nullable_type));

  // Create a list of joined tuple-id pairs (just the cross-product of tuples).
  std::vector<std::pair<tuple_id, tuple_id>> joined_tuple_ids;
  for (std::size_t tuple_num = 0; tuple_num < kNumSampleTuples; ++tuple_num) {
    joined_tuple_ids.emplace_back(tuple_num, 0);
    joined_tuple_ids.emplace_back(tuple_num, 1);
  }

  ColumnVectorPtr result_cv(case_expr.getAllValuesForJoin(
      0,
      &sample_data_value_accessor_,
      1,
      &other_accessor,
      joined_tuple_ids,
      nullptr /* cv_cache */));
  ASSERT_TRUE(result_cv->isNative());
  const NativeColumnVector &native_result_cv
      = static_cast<const NativeColumnVector&>(*result_cv);
  EXPECT_EQ(kNumSampleTuples * 2, native_result_cv.size());

  for (std::size_t result_num = 0;
       result_num < native_result_cv.size();
       ++result_num) {
    // For convenience, calculate expected tuple values here.
    const bool sample_int_null = ((result_num >> 1) % 10 == 0);
    const int sample_int = result_num >> 1;
    const int other_int = kOtherIntValues[result_num & 0x1];

    if (sample_int_null) {
      EXPECT_EQ(nullptr, native_result_cv.getUntypedValue(result_num));
    } else {
      ASSERT_NE(nullptr, native_result_cv.getUntypedValue(result_num));
      EXPECT_EQ(sample_int * other_int,
                *static_cast<const int*>(native_result_cv.getUntypedValue(result_num)));
    }
  }
}

// Test CASE evaluation over joins, with both WHEN predicates and THEN
// expressions referencing attributes in both relations.
TEST_F(ScalarCaseExpressionTest, JoinTest) {
  // Simulate a join with another relation.
  CatalogRelation other_relation(nullptr, "other", 1);
  other_relation.addAttribute(new CatalogAttribute(&other_relation,
                                                   "other_double",
                                                   TypeFactory::GetType(kDouble, false)));
  other_relation.addAttribute(new CatalogAttribute(&other_relation,
                                                   "other_int",
                                                   TypeFactory::GetType(kInt, false)));

  static const double kOtherDoubleValues[] = {-250.0, -750.0};
  std::unique_ptr<NativeColumnVector> other_double_column(
      new NativeColumnVector(TypeFactory::GetType(kDouble, false), 2));
  other_double_column->appendUntypedValue(kOtherDoubleValues);
  other_double_column->appendUntypedValue(kOtherDoubleValues + 1);

  static const int kOtherIntValues[] = {10, -1};
  std::unique_ptr<NativeColumnVector> other_int_column(
      new NativeColumnVector(TypeFactory::GetType(kInt, false), 2));
  other_int_column->appendUntypedValue(kOtherIntValues);
  other_int_column->appendUntypedValue(kOtherIntValues + 1);

  ColumnVectorsValueAccessor other_accessor;
  other_accessor.addColumn(other_double_column.release());
  other_accessor.addColumn(other_int_column.release());

  // Setup expression.
  std::vector<std::unique_ptr<Predicate>> when_predicates;
  std::vector<std::unique_ptr<Scalar>> result_expressions;

  // WHEN double_attr > other_double THEN int_attr + other_int
  when_predicates.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kGreater),
      new ScalarAttribute(*sample_relation_->getAttributeById(1)),
      new ScalarAttribute(*other_relation.getAttributeById(0))));
  result_expressions.emplace_back(new ScalarBinaryExpression(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd),
      new ScalarAttribute(*sample_relation_->getAttributeById(0)),
      new ScalarAttribute(*other_relation.getAttributeById(1))));

  // WHEN double_attr < other_double THEN int_attr * other_int
  when_predicates.emplace_back(new ComparisonPredicate(
      ComparisonFactory::GetComparison(ComparisonID::kLess),
      new ScalarAttribute(*sample_relation_->getAttributeById(1)),
      new ScalarAttribute(*other_relation.getAttributeById(0))));
  result_expressions.emplace_back(new ScalarBinaryExpression(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kMultiply),
      new ScalarAttribute(*sample_relation_->getAttributeById(0)),
      new ScalarAttribute(*other_relation.getAttributeById(1))));

  // ELSE 0
  ScalarCaseExpression case_expr(
      TypeFactory::GetType(kInt, true),
      std::move(when_predicates),
      std::move(result_expressions),
      new ScalarLiteral(TypedValue(0), TypeFactory::GetType(kInt)));

  // Create a list of joined tuple-id pairs (just the cross-product of tuples).
  std::vector<std::pair<tuple_id, tuple_id>> joined_tuple_ids;
  for (std::size_t tuple_num = 0; tuple_num < kNumSampleTuples; ++tuple_num) {
    joined_tuple_ids.emplace_back(tuple_num, 0);
    joined_tuple_ids.emplace_back(tuple_num, 1);
  }

  ColumnVectorPtr result_cv(case_expr.getAllValuesForJoin(
      0,
      &sample_data_value_accessor_,
      1,
      &other_accessor,
      joined_tuple_ids,
      nullptr /* cv_cache */));
  ASSERT_TRUE(result_cv->isNative());
  const NativeColumnVector &native_result_cv
      = static_cast<const NativeColumnVector&>(*result_cv);
  EXPECT_EQ(kNumSampleTuples * 2, native_result_cv.size());

  for (std::size_t result_num = 0;
       result_num < native_result_cv.size();
       ++result_num) {
    // For convenience, calculate expected tuple values here.
    const bool sample_int_null = ((result_num >> 1) % 10 == 0);
    const int sample_int = result_num >> 1;
    const bool sample_double_null = ((result_num >> 1) % 10 == 1);
    const double sample_double = -static_cast<double>(result_num >> 1);
    const int other_int = kOtherIntValues[result_num & 0x1];
    const double other_double = kOtherDoubleValues[result_num & 0x1];

    if (sample_double_null) {
      ASSERT_NE(nullptr, native_result_cv.getUntypedValue(result_num));
      EXPECT_EQ(0, *static_cast<const int*>(native_result_cv.getUntypedValue(result_num)));
    } else if (sample_double > other_double) {
      if (sample_int_null) {
        EXPECT_EQ(nullptr, native_result_cv.getUntypedValue(result_num));
      } else {
        ASSERT_NE(nullptr, native_result_cv.getUntypedValue(result_num));
        EXPECT_EQ(sample_int + other_int,
                  *static_cast<const int*>(native_result_cv.getUntypedValue(result_num)));
      }
    } else if (sample_double < other_double) {
      if (sample_int_null) {
        EXPECT_EQ(nullptr, native_result_cv.getUntypedValue(result_num));
      } else {
        ASSERT_NE(nullptr, native_result_cv.getUntypedValue(result_num));
        EXPECT_EQ(sample_int * other_int,
                  *static_cast<const int*>(native_result_cv.getUntypedValue(result_num)));
      }
    } else {
      ASSERT_NE(nullptr, native_result_cv.getUntypedValue(result_num));
      EXPECT_EQ(0, *static_cast<const int*>(native_result_cv.getUntypedValue(result_num)));
    }
  }
}

}  // namespace quickstep
