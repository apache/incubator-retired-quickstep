/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "expressions/ExpressionFactories.hpp"
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
#include "types/operations/binary_operations/BinaryOperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "types/operations/unary_operations/NumericCastOperation.hpp"
#include "types/operations/unary_operations/UnaryOperation.hpp"
#include "types/operations/unary_operations/UnaryOperationFactory.hpp"
#include "types/operations/unary_operations/UnaryOperationID.hpp"
#include "utility/Macros.hpp"

using std::int64_t;
using std::numeric_limits;
using std::size_t;
using std::string;
using std::strlen;
using std::unique_ptr;

namespace quickstep {

namespace {

static const char kSampleStringShort[] = "foo";

static const char kSampleStringLong[] =
    "Space is big. You just won't believe how vastly, hugely, mind-bogglingly "
    "big it is. I mean, you may think it's a long way down the road to the "
    "chemist's, but that's just peanuts to space.";

}  // namespace

class ScalarTest : public ::testing::Test {
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

    rel_string_ = createCatalogRelation("rel_string_");
    size_t length = 12;
    attr_char_ = createCatalogAttribute(rel_string_, "attr_char", TypeFactory::GetType(kChar, length));
    attr_varchar_ = createCatalogAttribute(rel_string_, "attr_varchar", TypeFactory::GetType(kVarChar, length));
  }

  void checkScalarUnaryExpressionSerialization(const UnaryOperation &operation) {
    checkScalarSerialization(ScalarUnaryExpression(operation, createScalarInt(numeric_limits<int>::max())));
    checkScalarSerialization(ScalarUnaryExpression(operation, createScalarLong(numeric_limits<int64_t>::max())));
    checkScalarSerialization(ScalarUnaryExpression(operation, createScalarFloat(numeric_limits<float>::max())));
    checkScalarSerialization(ScalarUnaryExpression(operation, createScalarDouble(numeric_limits<double>::max())));
  }

  void checkScalarBinaryExpressionSerialization(const BinaryOperation &operation) {
    checkScalarSerialization(ScalarBinaryExpression(operation, createScalarInt(9), createScalarInt(-1)));
    checkScalarSerialization(
        ScalarBinaryExpression(operation, createScalarInt(0), createScalarLong(static_cast<int64_t>(9))));
    checkScalarSerialization(
        ScalarBinaryExpression(operation, createScalarInt(4), createScalarFloat(static_cast<float>(1.2))));
    checkScalarSerialization(
        ScalarBinaryExpression(operation, createScalarInt(7), createScalarDouble(static_cast<double>(3.14))));

    checkScalarSerialization(
        ScalarBinaryExpression(operation, createScalarLong(static_cast<int64_t>(-10)), createScalarInt(8)));
    checkScalarSerialization(
        ScalarBinaryExpression(operation,
                               createScalarLong(static_cast<int64_t>(9)),
                               createScalarLong(static_cast<int64_t>(-9))));
    checkScalarSerialization(
        ScalarBinaryExpression(operation,
                               createScalarLong(static_cast<int64_t>(-9)),
                               createScalarFloat(static_cast<float>(1.2))));
    checkScalarSerialization(
        ScalarBinaryExpression(operation,
                               createScalarLong(static_cast<int64_t>(9)),
                               createScalarDouble(static_cast<double>(3.14))));

    checkScalarSerialization(
        ScalarBinaryExpression(operation, createScalarFloat(static_cast<float>(1.2)), createScalarInt(-1)));
    checkScalarSerialization(
        ScalarBinaryExpression(operation,
                               createScalarFloat(static_cast<float>(1.2)),
                               createScalarLong(static_cast<int64_t>(-9))));
    checkScalarSerialization(
        ScalarBinaryExpression(operation,
                               createScalarFloat(static_cast<float>(1.2)),
                               createScalarFloat(static_cast<float>(-3.6))));
    checkScalarSerialization(
        ScalarBinaryExpression(operation,
                               createScalarFloat(static_cast<float>(-1.2)),
                               createScalarDouble(static_cast<double>(3.14))));

    checkScalarSerialization(
        ScalarBinaryExpression(operation, createScalarDouble(static_cast<double>(3.14)), createScalarInt(6)));
    checkScalarSerialization(
        ScalarBinaryExpression(operation,
                               createScalarDouble(static_cast<double>(3.14)),
                               createScalarLong(static_cast<int64_t>(-9))));
    checkScalarSerialization(
        ScalarBinaryExpression(operation,
                               createScalarDouble(static_cast<double>(-3.14)),
                               createScalarFloat(static_cast<float>(1.2))));
    checkScalarSerialization(
        ScalarBinaryExpression(operation,
                               createScalarDouble(static_cast<double>(3.14)),
                               createScalarDouble(static_cast<double>(-3.14))));
  }

  CatalogRelation* createCatalogRelation(const std::string &name) {
    return database_->getRelationByIdMutable(database_->addRelation(new CatalogRelation(NULL, name)));
  }

  const CatalogAttribute* createCatalogAttribute(CatalogRelation* rel, const std::string &name, const Type &type) {
    rel->addAttribute(new CatalogAttribute(NULL, name, type));
    return rel->getAttributeByName(name);
  }

  // All four pointers created below are owned by a Scalar subclass.
  static Scalar* createScalarInt(int int_literal) {
    return new ScalarLiteral(TypedValue(int_literal), TypeFactory::GetType(kInt));
  }

  static Scalar* createScalarLong(int64_t long_literal) {
    return new ScalarLiteral(TypedValue(long_literal), TypeFactory::GetType(kLong));
  }

  static Scalar* createScalarFloat(float float_literal) {
    return new ScalarLiteral(TypedValue(float_literal), TypeFactory::GetType(kFloat));
  }

  static Scalar* createScalarDouble(double double_literal) {
    return new ScalarLiteral(TypedValue(double_literal), TypeFactory::GetType(kDouble));
  }

  void checkScalarSerialization(const Scalar &scalar) {
    unique_ptr<Scalar> scalar_from_proto(ScalarFactory::ReconstructFromProto(scalar.getProto(), *database_));
    compareScalar(scalar, *scalar_from_proto);
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
  CatalogRelation *rel_numeric_, *rel_date_, *rel_string_;
  // All pointers below are owned by relations.
  const CatalogAttribute *attr_int_, *attr_long_, *attr_float_, *attr_double_,
      *attr_datetime_, *attr_datetime_interval_, *attr_year_month_interval_,
      *attr_char_, *attr_varchar_;
};

TEST_F(ScalarTest, ScalarLiteralSerializationTest) {
  checkScalarSerialization(ScalarLiteral(TypedValue(kInt),
                                         TypeFactory::GetType(kInt, true /* nullable */)));
  checkScalarSerialization(ScalarLiteral(TypedValue(4),
                                         TypeFactory::GetType(kInt, false /* nullable */)));

  checkScalarSerialization(ScalarLiteral(TypedValue(kLong),
                                         TypeFactory::GetType(kLong, true /* nullable */)));
  checkScalarSerialization(ScalarLiteral(TypedValue(static_cast<int64_t>(9)),
                                         TypeFactory::GetType(kLong, false /* nullable */)));

  checkScalarSerialization(ScalarLiteral(TypedValue(kFloat),
                                         TypeFactory::GetType(kFloat, true /* nullable */)));
  checkScalarSerialization(ScalarLiteral(TypedValue(static_cast<float>(1.2)),
                                         TypeFactory::GetType(kFloat, false /* nullable */)));

  checkScalarSerialization(ScalarLiteral(TypedValue(kDouble),
                                         TypeFactory::GetType(kDouble, true /* nullable */)));
  checkScalarSerialization(ScalarLiteral(TypedValue(static_cast<double>(1.3)),
                                         TypeFactory::GetType(kDouble, false /* nullable */)));

  checkScalarSerialization(ScalarLiteral(TypedValue(kChar),
                                         TypeFactory::GetType(kChar, 0, true /* nullable */)));
  checkScalarSerialization(ScalarLiteral(TypedValue(kChar, kSampleStringShort, strlen(kSampleStringShort)+5),
                                         TypeFactory::GetType(kChar,
                                                              strlen(kSampleStringShort) + 5,
                                                              false /* nullable */)));

  checkScalarSerialization(ScalarLiteral(TypedValue(kVarChar),
                                         TypeFactory::GetType(kVarChar, 0, true)));
  checkScalarSerialization(ScalarLiteral(TypedValue(kVarChar, kSampleStringLong, strlen(kSampleStringLong)+1),
                                         TypeFactory::GetType(kVarChar,
                                                              strlen(kSampleStringLong),
                                                              false /* nullable */)));

  checkScalarSerialization(ScalarLiteral(TypedValue(kDatetime),
                                         TypeFactory::GetType(kDatetime, true /* nullable */)));
  DatetimeLit datetime;
  datetime.ticks = static_cast<int64_t>(0xFFFFF);
  checkScalarSerialization(ScalarLiteral(TypedValue(datetime),
                                         TypeFactory::GetType(kDatetime, false /* nullable */)));

  checkScalarSerialization(ScalarLiteral(TypedValue(kDatetimeInterval),
                                         TypeFactory::GetType(kDatetimeInterval,
                                                              true /* nullable */)));
  DatetimeIntervalLit datetime_interval;
  datetime_interval.interval_ticks = static_cast<int64_t>(0xFF);
  checkScalarSerialization(ScalarLiteral(TypedValue(datetime_interval),
                                         TypeFactory::GetType(kDatetimeInterval,
                                                              false /* nullable */)));

  checkScalarSerialization(ScalarLiteral(TypedValue(kYearMonthInterval),
                                         TypeFactory::GetType(kYearMonthInterval,
                                                              true /* nullable */)));
  YearMonthIntervalLit year_month_interval;
  year_month_interval.months = static_cast<int64_t>(14);
  checkScalarSerialization(ScalarLiteral(TypedValue(year_month_interval),
                                         TypeFactory::GetType(kYearMonthInterval,
                                                              false /* nullable */)));
}

TEST_F(ScalarTest, ScalarAttributeSerializationTest) {
  checkScalarSerialization(ScalarAttribute(*attr_int_));
  checkScalarSerialization(ScalarAttribute(*attr_long_));
  checkScalarSerialization(ScalarAttribute(*attr_float_));
  checkScalarSerialization(ScalarAttribute(*attr_double_));

  checkScalarSerialization(ScalarAttribute(*attr_datetime_));
  checkScalarSerialization(ScalarAttribute(*attr_datetime_interval_));
  checkScalarSerialization(ScalarAttribute(*attr_year_month_interval_));

  checkScalarSerialization(ScalarAttribute(*attr_char_));
  checkScalarSerialization(ScalarAttribute(*attr_varchar_));
}

TEST_F(ScalarTest, ScalarUnaryExpressionSerializationTest) {
  checkScalarUnaryExpressionSerialization(UnaryOperationFactory::GetUnaryOperation(UnaryOperationID::kNegate));
  checkScalarUnaryExpressionSerialization(NumericCastOperation::Instance(TypeFactory::GetType(kDouble)));
}

TEST_F(ScalarTest, ScalarBinaryExpressionSerializationTest) {
  checkScalarBinaryExpressionSerialization(BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd));
  checkScalarBinaryExpressionSerialization(BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kSubtract));
  checkScalarBinaryExpressionSerialization(BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kMultiply));
  checkScalarBinaryExpressionSerialization(BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kDivide));
  checkScalarBinaryExpressionSerialization(BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kModulo));
}

}  // namespace quickstep
