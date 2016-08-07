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
#include <limits>
#include <memory>
#include <string>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "expressions/ExpressionFactories.hpp"
#include "expressions/predicate/ComparisonPredicate.hpp"
#include "expressions/predicate/ConjunctionPredicate.hpp"
#include "expressions/predicate/DisjunctionPredicate.hpp"
#include "expressions/predicate/NegationPredicate.hpp"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/predicate/PredicateWithList.hpp"
#include "expressions/predicate/TrivialPredicates.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "expressions/scalar/ScalarBinaryExpression.hpp"
#include "expressions/scalar/ScalarLiteral.hpp"
#include "expressions/scalar/ScalarUnaryExpression.hpp"
#include "types/DatetimeLit.hpp"
#include "types/IntervalLit.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "types/operations/unary_operations/UnaryOperation.hpp"
#include "types/operations/unary_operations/UnaryOperationID.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrList.hpp"

#include "gtest/gtest.h"

using std::int64_t;
using std::numeric_limits;
using std::size_t;
using std::string;
using std::unique_ptr;

namespace quickstep {

class PredicateTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    database_.reset(new CatalogDatabase(NULL, "database_"));

    rel_numeric_ = createCatalogRelation("rel_numeric_");
    attr_int_ = createCatalogAttribute(rel_numeric_, "attr_int", TypeFactory::GetType(kInt));
    attr_long_ = createCatalogAttribute(rel_numeric_, "attr_long", TypeFactory::GetType(kLong));
    attr_float_ = createCatalogAttribute(rel_numeric_, "attr_float", TypeFactory::GetType(kFloat));
    attr_double_ = createCatalogAttribute(rel_numeric_, "attr_double", TypeFactory::GetType(kDouble));

    rel_date_ = createCatalogRelation("rel_date_");
    attr_datetime_ = createCatalogAttribute(rel_date_, "attr_datetime", TypeFactory::GetType(kDatetime));
    attr_datetime_interval_ =
        createCatalogAttribute(rel_date_, "attr_datetime_interval", TypeFactory::GetType(kDatetimeInterval));
    attr_year_month_interval_ =
        createCatalogAttribute(rel_date_, "attr_year_month_interval", TypeFactory::GetType(kYearMonthInterval));
  }

  void checkComparisonPredicateSerialization(const Comparison &comparison) {
    checkPredicateSerialization(ComparisonPredicate(comparison, createScalarInt(9), createScalarInt(-1)));
    checkPredicateSerialization(
        ComparisonPredicate(comparison, createScalarInt(0), createScalarLong(static_cast<int64_t>(9))));
    checkPredicateSerialization(
        ComparisonPredicate(comparison, createScalarInt(4), createScalarFloat(static_cast<float>(1.2))));
    checkPredicateSerialization(
        ComparisonPredicate(comparison, createScalarInt(7), createScalarDouble(static_cast<double>(3.14))));

    checkPredicateSerialization(
        ComparisonPredicate(comparison, createScalarLong(static_cast<int64_t>(-10)), createScalarInt(8)));
    checkPredicateSerialization(
        ComparisonPredicate(comparison,
                            createScalarLong(static_cast<int64_t>(9)),
                            createScalarLong(static_cast<int64_t>(-9))));
    checkPredicateSerialization(
        ComparisonPredicate(comparison,
                            createScalarLong(static_cast<int64_t>(-9)),
                            createScalarFloat(static_cast<float>(1.2))));
    checkPredicateSerialization(
        ComparisonPredicate(comparison,
                            createScalarLong(static_cast<int64_t>(9)),
                            createScalarDouble(static_cast<double>(3.14))));

    checkPredicateSerialization(
        ComparisonPredicate(comparison, createScalarFloat(static_cast<float>(1.2)), createScalarInt(-1)));
    checkPredicateSerialization(
        ComparisonPredicate(comparison,
                            createScalarFloat(static_cast<float>(1.2)),
                            createScalarLong(static_cast<int64_t>(-9))));
    checkPredicateSerialization(
        ComparisonPredicate(comparison,
                            createScalarFloat(static_cast<float>(1.2)),
                            createScalarFloat(static_cast<float>(-3.6))));
    checkPredicateSerialization(
        ComparisonPredicate(comparison,
                            createScalarFloat(static_cast<float>(-1.2)),
                            createScalarDouble(static_cast<double>(3.14))));

    checkPredicateSerialization(
        ComparisonPredicate(comparison, createScalarDouble(static_cast<double>(3.14)), createScalarInt(6)));
    checkPredicateSerialization(
        ComparisonPredicate(comparison,
                            createScalarDouble(static_cast<double>(3.14)),
                            createScalarLong(static_cast<int64_t>(-9))));
    checkPredicateSerialization(
        ComparisonPredicate(comparison,
                            createScalarDouble(static_cast<double>(-3.14)),
                            createScalarFloat(static_cast<float>(1.2))));
    checkPredicateSerialization(
        ComparisonPredicate(comparison,
                            createScalarDouble(static_cast<double>(3.14)),
                            createScalarDouble(static_cast<double>(-3.14))));

    checkPredicateSerialization(
        ComparisonPredicate(comparison,
                            createScalarDatetime(numeric_limits<int64_t>::max()),
                            createScalarNull(TypeFactory::GetType(kDatetime,
                                                                  true /* nullable */))));

    checkPredicateSerialization(
        ComparisonPredicate(comparison,
                            createScalarDatetimeInterval(numeric_limits<int64_t>::min()),
                            createScalarNull(TypeFactory::GetType(kDatetimeInterval,
                                                                  true /* nullable */))));

    checkPredicateSerialization(
        ComparisonPredicate(comparison,
                            createScalarYearMonthInterval(numeric_limits<int64_t>::max()),
                            createScalarNull(TypeFactory::GetType(kYearMonthInterval,
                                                                  true /* nullable */))));
  }

  void checkPredicateSerialization(const Predicate &predicate) {
    unique_ptr<Predicate> predicate_from_proto(
        PredicateFactory::ReconstructFromProto(predicate.getProto(), *database_));
    comparePredicate(predicate, *predicate_from_proto);
  }

  CatalogRelation* createCatalogRelation(const std::string &name) {
    return database_->getRelationByIdMutable(database_->addRelation(new CatalogRelation(NULL, name)));
  }

  const CatalogAttribute* createCatalogAttribute(CatalogRelation* rel, const std::string &name, const Type &type) {
    rel->addAttribute(new CatalogAttribute(NULL, name, type));
    return rel->getAttributeByName(name);
  }

  static Scalar* createScalarNull(const Type &type) {
    return new ScalarLiteral(type.makeNullValue(), type);
  }

  static Scalar* createScalarInt(int int_literal) {
    return new ScalarLiteral(TypedValue(int_literal),
                             TypeFactory::GetType(kInt));
  }

  static Scalar* createScalarLong(int64_t long_literal) {
    return new ScalarLiteral(TypedValue(long_literal),
                             TypeFactory::GetType(kLong));
  }

  static Scalar* createScalarFloat(float float_literal) {
    return new ScalarLiteral(TypedValue(float_literal),
                             TypeFactory::GetType(kFloat));
  }

  static Scalar* createScalarDouble(double double_literal) {
    return new ScalarLiteral(TypedValue(double_literal),
                             TypeFactory::GetType(kDouble));
  }

  static Scalar* createScalarDatetime(int64_t tick) {
    DatetimeLit datetime;
    datetime.ticks = tick;
    return new ScalarLiteral(TypedValue(datetime),
                             TypeFactory::GetType(kDatetime));
  }

  static Scalar* createScalarDatetimeInterval(int64_t tick) {
    DatetimeIntervalLit datetime_interval;
    datetime_interval.interval_ticks = tick;
    return new ScalarLiteral(TypedValue(datetime_interval),
                             TypeFactory::GetType(kDatetimeInterval));
  }

  static Scalar* createScalarYearMonthInterval(int64_t month) {
    YearMonthIntervalLit year_month_interval;
    year_month_interval.months = month;
    return new ScalarLiteral(TypedValue(year_month_interval),
                             TypeFactory::GetType(kYearMonthInterval));
  }

  static void comparePredicatePtrList(const PtrList<Predicate> &expected, const PtrList<Predicate> &checked) {
    ASSERT_EQ(expected.size(), checked.size());

    for (PtrList<Predicate>::const_iterator it_expected = expected.begin(), it_checked = checked.begin();
         it_expected != expected.end();
         ++it_expected, ++it_checked) {
      comparePredicate(*it_expected, *it_checked);
    }
  }

  static void comparePredicate(const Predicate &expected, const Predicate &checked) {
    ASSERT_EQ(expected.getPredicateType(), checked.getPredicateType());

    switch (expected.getPredicateType()) {
      case Predicate::kTrue:  // Fall through.
      case Predicate::kFalse: {
        break;
      }
      case Predicate::kComparison: {
        EXPECT_TRUE(static_cast<const ComparisonPredicate&>(expected).getComparison().equals(
                    static_cast<const ComparisonPredicate&>(checked).getComparison()));

        compareScalar(static_cast<const ComparisonPredicate&>(expected).getLeftOperand(),
                      static_cast<const ComparisonPredicate&>(checked).getLeftOperand());

        compareScalar(static_cast<const ComparisonPredicate&>(expected).getRightOperand(),
                      static_cast<const ComparisonPredicate&>(checked).getRightOperand());
        break;
      }
      case Predicate::kNegation: {
        EXPECT_EQ(expected.hasStaticResult(), checked.hasStaticResult());
        if (expected.hasStaticResult()) {
          EXPECT_EQ(expected.getStaticResult(), checked.getStaticResult());
        }

        comparePredicate(*static_cast<const NegationPredicate&>(expected).operand_,
                         *static_cast<const NegationPredicate&>(checked).operand_);
        break;
      }
      case Predicate::kConjunction:  // Fall through.
      case Predicate::kDisjunction: {
        EXPECT_EQ(expected.hasStaticResult(), checked.hasStaticResult());
        if (expected.hasStaticResult()) {
          EXPECT_EQ(expected.getStaticResult(), checked.getStaticResult());
        }

        comparePredicatePtrList(static_cast<const PredicateWithList&>(expected).static_operand_list_,
                                static_cast<const PredicateWithList&>(checked).static_operand_list_);
        comparePredicatePtrList(static_cast<const PredicateWithList&>(expected).dynamic_operand_list_,
                                static_cast<const PredicateWithList&>(checked).dynamic_operand_list_);
        break;
      }
      default:
        FATAL_ERROR("checked is not a valid PredicateType in comparePredicate");
    }
  }

  static void compareScalar(const Scalar &expected, const Scalar &checked) {
    ASSERT_EQ(expected.getDataSource(), checked.getDataSource());
    ASSERT_TRUE(expected.getType().equals(checked.getType()));

    switch (expected.getDataSource()) {
      case Scalar::kLiteral: {
        compareTypedValue(expected.getType(), expected.getStaticValue(), checked.getStaticValue());
        break;
      }
      case Scalar::kAttribute: {
        compareCatalogAttribute(static_cast<const ScalarAttribute&>(expected).getAttribute(),
                                static_cast<const ScalarAttribute&>(checked).getAttribute());
        break;
      }
      case Scalar::kUnaryExpression: {
        EXPECT_EQ(expected.hasStaticValue(), checked.hasStaticValue());
        if (expected.hasStaticValue()) {
          compareTypedValue(expected.getType(), expected.getStaticValue(), checked.getStaticValue());
        }

        EXPECT_EQ(static_cast<const ScalarUnaryExpression&>(expected).operation_.getUnaryOperationID(),
                  static_cast<const ScalarUnaryExpression&>(checked).operation_.getUnaryOperationID());
        compareScalar(*(static_cast<const ScalarUnaryExpression&>(expected).operand_),
                      *(static_cast<const ScalarUnaryExpression&>(checked).operand_));
        break;
      }
      case Scalar::kBinaryExpression: {
        EXPECT_EQ(expected.hasStaticValue(), checked.hasStaticValue());
        if (expected.hasStaticValue()) {
          compareTypedValue(expected.getType(), expected.getStaticValue(), checked.getStaticValue());
        }

        EXPECT_EQ(static_cast<const ScalarBinaryExpression&>(expected).operation_.getBinaryOperationID(),
                  static_cast<const ScalarBinaryExpression&>(checked).operation_.getBinaryOperationID());
        compareScalar(*(static_cast<const ScalarBinaryExpression&>(expected).left_operand_),
                      *(static_cast<const ScalarBinaryExpression&>(checked).left_operand_));
        compareScalar(*(static_cast<const ScalarBinaryExpression&>(expected).right_operand_),
                      *(static_cast<const ScalarBinaryExpression&>(checked).right_operand_));
        break;
      }
      default:
        FATAL_ERROR("checked is not a valid Scalar in compareScalar");
    }
  }

  static void compareTypedValue(const Type &type,
                                const TypedValue &expected,
                                const TypedValue &checked) {
    ASSERT_EQ(expected.isNull(), checked.isNull());

    if (expected.isNull()) {
      return;
    }

    switch (type.getTypeID()) {
      case kInt:
        EXPECT_EQ(expected.getLiteral<int>(), checked.getLiteral<int>());
        break;
      case kLong:
        EXPECT_EQ(expected.getLiteral<int64_t>(), checked.getLiteral<int64_t>());
        break;
      case kFloat:
        EXPECT_FLOAT_EQ(expected.getLiteral<float>(), checked.getLiteral<float>());
        break;
      case kDouble:
        EXPECT_DOUBLE_EQ(expected.getLiteral<double>(), checked.getLiteral<double>());
        break;
      case kDatetime:
        EXPECT_EQ(expected.getLiteral<DatetimeLit>(), checked.getLiteral<DatetimeLit>());
        break;
      case kDatetimeInterval:
        EXPECT_EQ(expected.getLiteral<DatetimeIntervalLit>(), checked.getLiteral<DatetimeIntervalLit>());
        break;
      case kYearMonthInterval:
        EXPECT_EQ(expected.getLiteral<YearMonthIntervalLit>(), checked.getLiteral<YearMonthIntervalLit>());
        break;
      case kChar:  // Fall through.
      case kVarChar:
        EXPECT_EQ(expected.getDataSize(), checked.getDataSize());
        EXPECT_EQ(expected.getAsciiStringLength(), checked.getAsciiStringLength());
        EXPECT_STREQ(static_cast<const char*>(expected.getDataPtr()),
                     static_cast<const char*>(checked.getDataPtr()));
        break;
      default:
        FATAL_ERROR("Unrecognized Type in compareTypedValue");
    }
  }

  static void compareCatalogAttribute(const CatalogAttribute &expected, const CatalogAttribute &checked) {
    EXPECT_EQ(expected.getID(), checked.getID());
    EXPECT_EQ(expected.getName(), checked.getName());
    EXPECT_TRUE(expected.getType().equals(checked.getType()));
  }

  unique_ptr<CatalogDatabase> database_;
  // Both pointers below are owned by database_.
  CatalogRelation *rel_numeric_, *rel_date_;
  // All pointers below are owned by relations.
  const CatalogAttribute *attr_int_, *attr_long_, *attr_float_, *attr_double_,
      *attr_datetime_, *attr_datetime_interval_, *attr_year_month_interval_;
};

TEST_F(PredicateTest, TrivialPredicateSerializationTest) {
  checkPredicateSerialization(TruePredicate());
  checkPredicateSerialization(FalsePredicate());
}

TEST_F(PredicateTest, NegationPredicateSerializationTest) {
  checkPredicateSerialization(NegationPredicate(new TruePredicate()));
  checkPredicateSerialization(NegationPredicate(new FalsePredicate()));

  checkPredicateSerialization(
      NegationPredicate(new ComparisonPredicate(ComparisonFactory::GetComparison(ComparisonID::kEqual),
                                                new ScalarAttribute(*attr_int_),
                                                createScalarInt(-1))));
  checkPredicateSerialization(
      NegationPredicate(new ComparisonPredicate(ComparisonFactory::GetComparison(ComparisonID::kNotEqual),
                                                new ScalarAttribute(*attr_long_),
                                                createScalarLong(numeric_limits<int64_t>::max()))));
  checkPredicateSerialization(
      NegationPredicate(new ComparisonPredicate(ComparisonFactory::GetComparison(ComparisonID::kLess),
                                                new ScalarAttribute(*attr_float_),
                                                createScalarFloat(numeric_limits<float>::max()))));
  checkPredicateSerialization(
      NegationPredicate(new ComparisonPredicate(ComparisonFactory::GetComparison(ComparisonID::kLessOrEqual),
                                                new ScalarAttribute(*attr_double_),
                                                createScalarDouble(numeric_limits<double>::max()))));
  checkPredicateSerialization(
      NegationPredicate(new ComparisonPredicate(ComparisonFactory::GetComparison(ComparisonID::kGreater),
                                                new ScalarAttribute(*attr_datetime_),
                                                createScalarDatetime(numeric_limits<int64_t>::min()))));
  checkPredicateSerialization(
      NegationPredicate(new ComparisonPredicate(ComparisonFactory::GetComparison(ComparisonID::kGreaterOrEqual),
                                                new ScalarAttribute(*attr_datetime_interval_),
                                                createScalarDatetimeInterval(numeric_limits<int64_t>::min()))));
}

TEST_F(PredicateTest, ComparisonPredicateSerializationTest) {
  checkComparisonPredicateSerialization(ComparisonFactory::GetComparison(ComparisonID::kEqual));
  checkComparisonPredicateSerialization(ComparisonFactory::GetComparison(ComparisonID::kNotEqual));
  checkComparisonPredicateSerialization(ComparisonFactory::GetComparison(ComparisonID::kLess));
  checkComparisonPredicateSerialization(ComparisonFactory::GetComparison(ComparisonID::kLessOrEqual));
  checkComparisonPredicateSerialization(ComparisonFactory::GetComparison(ComparisonID::kGreater));
  checkComparisonPredicateSerialization(ComparisonFactory::GetComparison(ComparisonID::kGreaterOrEqual));
}

TEST_F(PredicateTest, ConjunctionPredicateSerializationTest) {
  ConjunctionPredicate predicate;
  predicate.addPredicate(new TruePredicate());
  predicate.addPredicate(NegationPredicate::NegatePredicate(new FalsePredicate()));
  predicate.addPredicate(NegationPredicate::NegatePredicate(
      new ComparisonPredicate(ComparisonFactory::GetComparison(ComparisonID::kGreaterOrEqual),
                              new ScalarAttribute(*attr_year_month_interval_),
                              createScalarYearMonthInterval(numeric_limits<int64_t>::min()))));

  checkPredicateSerialization(predicate);
}

TEST_F(PredicateTest, DisjunctionPredicateSerializationTest) {
  DisjunctionPredicate predicate;
  predicate.addPredicate(new FalsePredicate());
  predicate.addPredicate(NegationPredicate::NegatePredicate(new TruePredicate()));
  predicate.addPredicate(NegationPredicate::NegatePredicate(
      new ComparisonPredicate(ComparisonFactory::GetComparison(ComparisonID::kLess),
                              new ScalarAttribute(*attr_year_month_interval_),
                              createScalarYearMonthInterval(numeric_limits<int64_t>::max()))));

  checkPredicateSerialization(predicate);
}

}  // namespace quickstep
