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

#include <cstdint>
#include <limits>
#include <memory>
#include <vector>

#include "types/DatetimeLit.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/Operation.pb.h"
#include "types/operations/unary_operations/DateExtractOperation.hpp"
#include "types/operations/unary_operations/UnaryOperation.hpp"
#include "types/operations/unary_operations/UnaryOperationFactory.hpp"
#include "utility/EqualsAnyConstant.hpp"

#include "gtest/gtest.h"

using std::int64_t;
using std::numeric_limits;
using std::unique_ptr;
using std::vector;

namespace quickstep {

class DateExtractOperationTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    datetime_literal_.ticks = 1431359664 * DatetimeLit::kTicksPerSecond;  // Mon, 11 May 2015 15:54:24 GMT.
    datetime_.reset(new TypedValue(datetime_literal_));
    datetime_null_.reset(new TypedValue(kDatetime));

    date_literal_ = DateLit::Create(2015, 05, 11);  // 11 May 2015.
    date_.reset(new TypedValue(date_literal_));
    date_null_.reset(new TypedValue(kDate));
  }

  void checkDateExtractOperationSerialization(const DateExtractOperation &operation) {
    serialization::UnaryOperation proto = operation.getProto();
    EXPECT_EQ(serialization::UnaryOperation::DATE_EXTRACT, proto.operation_id());
    EXPECT_TRUE(UnaryOperationFactory::ProtoIsValid(proto));
    EXPECT_TRUE(operation.equals(UnaryOperationFactory::ReconstructFromProto(proto)));
  }

  void checkDatetimeExtract(int64_t expected, const DateExtractUnit unit) {
    const DateExtractOperation &operation = DateExtractOperation::Instance(unit);
    checkDateExtractOperationSerialization(operation);

    EXPECT_EQ(expected,
              operation.applyToChecked(*datetime_,
                                       TypeFactory::GetType(kDatetime, true)).getLiteral<int64_t>());
    EXPECT_TRUE(operation.applyToChecked(*datetime_null_,
                                         TypeFactory::GetType(kDatetime, true)).isNull());

    unchecked_operator_.reset(
        operation.makeUncheckedUnaryOperatorForType(TypeFactory::GetType(kDatetime, true)));
    EXPECT_EQ(expected, unchecked_operator_->applyToTypedValue(*datetime_).getLiteral<int64_t>());

    EXPECT_TRUE(unchecked_operator_->applyToTypedValue(*datetime_null_).isNull());
  }

  void checkDateExtract(int32_t expected, const DateExtractUnit unit) {
    const DateExtractOperation &operation = DateExtractOperation::Instance(unit);
    checkDateExtractOperationSerialization(operation);

    EXPECT_EQ(expected,
              operation.applyToChecked(*date_,
                                       TypeFactory::GetType(kDate, true)).getLiteral<int32_t>());
    EXPECT_TRUE(operation.applyToChecked(*date_null_,
                                         TypeFactory::GetType(kDate, true)).isNull());

    unchecked_operator_.reset(
        operation.makeUncheckedUnaryOperatorForType(TypeFactory::GetType(kDate, true)));
    EXPECT_EQ(expected, unchecked_operator_->applyToTypedValue(*date_).getLiteral<int32_t>());

    EXPECT_TRUE(unchecked_operator_->applyToTypedValue(*date_null_).isNull());
  }

  static void CheckFixedNullableResultTypeForField(const DateExtractUnit field) {
    const Type *fixed_result_type
        = DateExtractOperation::Instance(field).fixedNullableResultType();
    ASSERT_EQ(fixed_result_type, nullptr);
  }

  static void CheckResultTypeIsPlausibleForField(const DateExtractUnit field) {
    // Long and Int are plausible.
    EXPECT_TRUE(DateExtractOperation::Instance(field).resultTypeIsPlausible(
        TypeFactory::GetType(kLong, false)));
    EXPECT_TRUE(DateExtractOperation::Instance(field).resultTypeIsPlausible(
        TypeFactory::GetType(kLong, true)));
    EXPECT_TRUE(DateExtractOperation::Instance(field).resultTypeIsPlausible(
        TypeFactory::GetType(kInt, false)));
    EXPECT_TRUE(DateExtractOperation::Instance(field).resultTypeIsPlausible(
        TypeFactory::GetType(kInt, true)));

    for (const TypeID type_id
         : {kFloat, kDouble, kDatetime, kDatetimeInterval, kYearMonthInterval}) {
      EXPECT_FALSE(DateExtractOperation::Instance(field).resultTypeIsPlausible(
          TypeFactory::GetType(type_id, false)));
      EXPECT_FALSE(DateExtractOperation::Instance(field).resultTypeIsPlausible(
          TypeFactory::GetType(type_id, true)));
    }

    EXPECT_FALSE(DateExtractOperation::Instance(field).resultTypeIsPlausible(
        TypeFactory::GetType(kChar, 10, false)));
    EXPECT_FALSE(DateExtractOperation::Instance(field).resultTypeIsPlausible(
        TypeFactory::GetType(kChar, 10, true)));
    EXPECT_FALSE(DateExtractOperation::Instance(field).resultTypeIsPlausible(
        TypeFactory::GetType(kVarChar, 10, false)));
    EXPECT_FALSE(DateExtractOperation::Instance(field).resultTypeIsPlausible(
        TypeFactory::GetType(kVarChar, 10, true)));
  }

  static void CheckPushDownTypeHintForField(const DateExtractUnit field) {
    const UnaryOperation &op = DateExtractOperation::Instance(field);

    // For nullable types.
    const Type *argument_hint
        = op.pushDownTypeHint(&TypeFactory::GetType(kLong, true));
    if ((field == DateExtractUnit::kMonth) || (field == DateExtractUnit::kYear)) {
      // If field is either Year or Month, the argument is NULL.
      ASSERT_EQ(argument_hint, nullptr);
    } else if (QUICKSTEP_EQUALS_ANY_CONSTANT(field,
                                             DateExtractUnit::kDay,
                                             DateExtractUnit::kHour,
                                             DateExtractUnit::kMinute,
                                             DateExtractUnit::kSecond)) {
      // Otherwise, if hint is Long, then the argument is a Datetime with the
      // same nullability.
      ASSERT_NE(argument_hint, nullptr);
      EXPECT_TRUE(TypeFactory::GetType(kDatetime, true).equals(*argument_hint));
    }

    // For non-nullable types.
    argument_hint = op.pushDownTypeHint(&TypeFactory::GetType(kLong, false));
    if ((field == DateExtractUnit::kMonth) || (field == DateExtractUnit::kYear)) {
      // If field is either Year or Month, the argument is NULL.
      ASSERT_EQ(argument_hint, nullptr);
    } else {
      // Otherwise, if hint is Long, then the argument is a Datetime with the
      // same nullability.
      ASSERT_NE(argument_hint, nullptr);
      EXPECT_TRUE(TypeFactory::GetType(kDatetime, false).equals(*argument_hint));
    }

    // Anything else can't be returned, so there is no hint for argument type.
    EXPECT_EQ(nullptr, op.pushDownTypeHint(nullptr));

    for (const TypeID type_id
         : {kInt, kFloat, kDouble, kDatetime, kDatetimeInterval, kYearMonthInterval}) {
      EXPECT_EQ(nullptr, op.pushDownTypeHint(&TypeFactory::GetType(type_id, false)));
      EXPECT_EQ(nullptr, op.pushDownTypeHint(&TypeFactory::GetType(type_id, true)));
    }

    EXPECT_EQ(nullptr, op.pushDownTypeHint(&TypeFactory::GetType(kChar, 10, false)));
    EXPECT_EQ(nullptr, op.pushDownTypeHint(&TypeFactory::GetType(kChar, 10, true)));
    EXPECT_EQ(nullptr, op.pushDownTypeHint(&TypeFactory::GetType(kVarChar, 10, false)));
    EXPECT_EQ(nullptr, op.pushDownTypeHint(&TypeFactory::GetType(kVarChar, 10, true)));
  }

  DatetimeLit datetime_literal_;
  DateLit date_literal_;
  unique_ptr<TypedValue> datetime_, datetime_null_;
  unique_ptr<TypedValue> date_, date_null_;

  unique_ptr<UncheckedUnaryOperator> unchecked_operator_;
};

TEST_F(DateExtractOperationTest, DateTimeExtractUnaryOperationTest) {
  checkDatetimeExtract(datetime_literal_.yearField(), DateExtractUnit::kYear);
  checkDatetimeExtract(datetime_literal_.monthField(), DateExtractUnit::kMonth);
  checkDatetimeExtract(datetime_literal_.dayField(), DateExtractUnit::kDay);
  checkDatetimeExtract(datetime_literal_.hourField(), DateExtractUnit::kHour);
  checkDatetimeExtract(datetime_literal_.minuteField(), DateExtractUnit::kMinute);
  checkDatetimeExtract(datetime_literal_.secondField(), DateExtractUnit::kSecond);
}

TEST_F(DateExtractOperationTest, DateExtractUnaryOperationTest) {
  checkDateExtract(date_literal_.yearField(), DateExtractUnit::kYear);
  checkDateExtract(date_literal_.monthField(), DateExtractUnit::kMonth);
}

TEST_F(DateExtractOperationTest, FixedNullableResultTypeTest) {
  CheckFixedNullableResultTypeForField(DateExtractUnit::kYear);
  CheckFixedNullableResultTypeForField(DateExtractUnit::kMonth);
  CheckFixedNullableResultTypeForField(DateExtractUnit::kDay);
  CheckFixedNullableResultTypeForField(DateExtractUnit::kHour);
  CheckFixedNullableResultTypeForField(DateExtractUnit::kMinute);
  CheckFixedNullableResultTypeForField(DateExtractUnit::kSecond);
}

TEST_F(DateExtractOperationTest, ResultTypeIsPlausibleTest) {
  CheckResultTypeIsPlausibleForField(DateExtractUnit::kYear);
  CheckResultTypeIsPlausibleForField(DateExtractUnit::kMonth);
  CheckResultTypeIsPlausibleForField(DateExtractUnit::kDay);
  CheckResultTypeIsPlausibleForField(DateExtractUnit::kHour);
  CheckResultTypeIsPlausibleForField(DateExtractUnit::kMinute);
  CheckResultTypeIsPlausibleForField(DateExtractUnit::kSecond);
}

TEST_F(DateExtractOperationTest, PushDownTypeHintTest) {
  CheckPushDownTypeHintForField(DateExtractUnit::kYear);
  CheckPushDownTypeHintForField(DateExtractUnit::kMonth);
  CheckPushDownTypeHintForField(DateExtractUnit::kDay);
  CheckPushDownTypeHintForField(DateExtractUnit::kHour);
  CheckPushDownTypeHintForField(DateExtractUnit::kMinute);
  CheckPushDownTypeHintForField(DateExtractUnit::kSecond);
}

}  // namespace quickstep
