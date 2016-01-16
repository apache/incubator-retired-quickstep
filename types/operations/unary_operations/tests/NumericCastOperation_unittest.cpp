/**
 * This file copyright (c) 2015, Pivotal Software Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/operations/unary_operations/NumericCastOperation.hpp"
#include "types/operations/unary_operations/UnaryOperation.hpp"

#include "gtest/gtest.h"

namespace quickstep {

void CheckFixedNullableResultType(const Type &cast_type) {
  const UnaryOperation &op = NumericCastOperation::Instance(cast_type);
  const Type *fixed_result_type = op.fixedNullableResultType();
  ASSERT_NE(fixed_result_type, nullptr);
  EXPECT_TRUE(cast_type.getNullableVersion().equals(*fixed_result_type));
}

TEST(NumericCastOperationTest, FixedNullableResultTypeTest) {
  CheckFixedNullableResultType(TypeFactory::GetType(kInt, false));
  CheckFixedNullableResultType(TypeFactory::GetType(kInt, true));
  CheckFixedNullableResultType(TypeFactory::GetType(kLong, false));
  CheckFixedNullableResultType(TypeFactory::GetType(kLong, true));
  CheckFixedNullableResultType(TypeFactory::GetType(kFloat, false));
  CheckFixedNullableResultType(TypeFactory::GetType(kFloat, true));
  CheckFixedNullableResultType(TypeFactory::GetType(kDouble, false));
  CheckFixedNullableResultType(TypeFactory::GetType(kDouble, true));
}

void CheckResultTypeIsPlausible(const Type &cast_type) {
  const UnaryOperation &op = NumericCastOperation::Instance(cast_type);
  EXPECT_TRUE(op.resultTypeIsPlausible(cast_type));
  EXPECT_TRUE(op.resultTypeIsPlausible(cast_type.getNullableVersion()));
  if (cast_type.isNullable()) {
    EXPECT_FALSE(op.resultTypeIsPlausible(cast_type.getNonNullableVersion()));
  }

  // Check some other type.
  if (cast_type.getTypeID() == kInt) {
    EXPECT_FALSE(op.resultTypeIsPlausible(TypeFactory::GetType(kLong, false)));
    EXPECT_FALSE(op.resultTypeIsPlausible(TypeFactory::GetType(kLong, true)));
  } else {
    EXPECT_FALSE(op.resultTypeIsPlausible(TypeFactory::GetType(kInt, false)));
    EXPECT_FALSE(op.resultTypeIsPlausible(TypeFactory::GetType(kInt, true)));
  }
}

TEST(NumericCastOperationTest, ResultTypeIsPlausibleTest) {
  CheckResultTypeIsPlausible(TypeFactory::GetType(kInt, false));
  CheckResultTypeIsPlausible(TypeFactory::GetType(kInt, true));
  CheckResultTypeIsPlausible(TypeFactory::GetType(kLong, false));
  CheckResultTypeIsPlausible(TypeFactory::GetType(kLong, true));
  CheckResultTypeIsPlausible(TypeFactory::GetType(kFloat, false));
  CheckResultTypeIsPlausible(TypeFactory::GetType(kFloat, true));
  CheckResultTypeIsPlausible(TypeFactory::GetType(kDouble, false));
  CheckResultTypeIsPlausible(TypeFactory::GetType(kDouble, true));
}

void CheckPushDownTypeHint(const Type &cast_type) {
  const UnaryOperation &op = NumericCastOperation::Instance(cast_type);

  // No matter the result hint, the argument hint is always the target type for
  // the cast.
  const Type *argument_hint = op.pushDownTypeHint(nullptr);
  ASSERT_NE(argument_hint, nullptr);
  EXPECT_TRUE(cast_type.equals(*argument_hint));

  argument_hint = op.pushDownTypeHint(&TypeFactory::GetType(kInt, false));
  ASSERT_NE(argument_hint, nullptr);
  EXPECT_TRUE(cast_type.equals(*argument_hint));
  argument_hint = op.pushDownTypeHint(&TypeFactory::GetType(kInt, true));
  ASSERT_NE(argument_hint, nullptr);
  EXPECT_TRUE(cast_type.equals(*argument_hint));
  argument_hint = op.pushDownTypeHint(&TypeFactory::GetType(kLong, false));
  ASSERT_NE(argument_hint, nullptr);
  EXPECT_TRUE(cast_type.equals(*argument_hint));
  argument_hint = op.pushDownTypeHint(&TypeFactory::GetType(kLong, true));
  ASSERT_NE(argument_hint, nullptr);
  EXPECT_TRUE(cast_type.equals(*argument_hint));
  argument_hint = op.pushDownTypeHint(&TypeFactory::GetType(kFloat, false));
  ASSERT_NE(argument_hint, nullptr);
  EXPECT_TRUE(cast_type.equals(*argument_hint));
  argument_hint = op.pushDownTypeHint(&TypeFactory::GetType(kFloat, true));
  ASSERT_NE(argument_hint, nullptr);
  EXPECT_TRUE(cast_type.equals(*argument_hint));
  argument_hint = op.pushDownTypeHint(&TypeFactory::GetType(kDouble, false));
  ASSERT_NE(argument_hint, nullptr);
  EXPECT_TRUE(cast_type.equals(*argument_hint));
  argument_hint = op.pushDownTypeHint(&TypeFactory::GetType(kDouble, true));
  ASSERT_NE(argument_hint, nullptr);
  EXPECT_TRUE(cast_type.equals(*argument_hint));
}

TEST(NumericCastOperationTest, PushDownTypeHintTest) {
  CheckPushDownTypeHint(TypeFactory::GetType(kInt, false));
  CheckPushDownTypeHint(TypeFactory::GetType(kInt, true));
  CheckPushDownTypeHint(TypeFactory::GetType(kLong, false));
  CheckPushDownTypeHint(TypeFactory::GetType(kLong, true));
  CheckPushDownTypeHint(TypeFactory::GetType(kFloat, false));
  CheckPushDownTypeHint(TypeFactory::GetType(kFloat, true));
  CheckPushDownTypeHint(TypeFactory::GetType(kDouble, false));
  CheckPushDownTypeHint(TypeFactory::GetType(kDouble, true));
}

}  // namespace quickstep
