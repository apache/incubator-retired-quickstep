/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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

#include "gtest/gtest.h"

using std::int64_t;
using std::numeric_limits;
using std::unique_ptr;
using std::vector;

namespace quickstep {

class DateExtractOperationTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    dt_literal_.ticks = 1431359664 * DatetimeLit::kTicksPerSecond;  // Mon, 11 May 2015 15:54:24 GMT.
    datetime_.reset(new TypedValue(dt_literal_));

    datetime_null_.reset(new TypedValue(kDatetime));
  }

  void checkDateExtractOperationSerialization(const DateExtractOperation &operation) {
    serialization::UnaryOperation proto = operation.getProto();
    EXPECT_EQ(serialization::UnaryOperation::DATE_EXTRACT, proto.operation_id());
    EXPECT_TRUE(UnaryOperationFactory::ProtoIsValid(proto));
    EXPECT_TRUE(operation.equals(UnaryOperationFactory::ReconstructFromProto(proto)));
  }

  void checkDateExtract(int64_t expected, const DateExtractUnit unit) {
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

  static void CheckFixedNullableResultTypeForField(const DateExtractUnit field) {
    const Type *fixed_result_type
        = DateExtractOperation::Instance(field).fixedNullableResultType();
    ASSERT_NE(fixed_result_type, nullptr);
    EXPECT_TRUE(TypeFactory::GetType(kLong, true).equals(*fixed_result_type));
  }

  static void CheckResultTypeIsPlausibleForField(const DateExtractUnit field) {
    // Only Long is plausible.
    EXPECT_TRUE(DateExtractOperation::Instance(field).resultTypeIsPlausible(
        TypeFactory::GetType(kLong, false)));
    EXPECT_TRUE(DateExtractOperation::Instance(field).resultTypeIsPlausible(
        TypeFactory::GetType(kLong, true)));

    for (const TypeID type_id
         : {kInt, kFloat, kDouble, kDatetime, kDatetimeInterval, kYearMonthInterval}) {
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

    // If hint is Long, then the argument is a Datetime with the same
    // nullability.
    const Type *argument_hint
        = op.pushDownTypeHint(&TypeFactory::GetType(kLong, true));
    ASSERT_NE(argument_hint, nullptr);
    EXPECT_TRUE(TypeFactory::GetType(kDatetime, true).equals(*argument_hint));

    argument_hint = op.pushDownTypeHint(&TypeFactory::GetType(kLong, false));
    ASSERT_NE(argument_hint, nullptr);
    EXPECT_TRUE(TypeFactory::GetType(kDatetime, false).equals(*argument_hint));

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

  DatetimeLit dt_literal_;
  unique_ptr<TypedValue> datetime_, datetime_null_;

  unique_ptr<UncheckedUnaryOperator> unchecked_operator_;
};

TEST_F(DateExtractOperationTest, DateExtratUnaryOperationTest) {
  checkDateExtract(dt_literal_.yearField(), DateExtractUnit::kYear);
  checkDateExtract(dt_literal_.monthField(), DateExtractUnit::kMonth);
  checkDateExtract(dt_literal_.dayField(), DateExtractUnit::kDay);
  checkDateExtract(dt_literal_.hourField(), DateExtractUnit::kHour);
  checkDateExtract(dt_literal_.minuteField(), DateExtractUnit::kMinute);
  checkDateExtract(dt_literal_.secondField(), DateExtractUnit::kSecond);
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
