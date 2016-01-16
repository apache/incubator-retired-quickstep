/**
 * This file copyright (c) 2015, Pivotal Software Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include <initializer_list>
#include <utility>

#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/binary_operations/SubtractBinaryOperation.hpp"
#include "types/operations/binary_operations/tests/BinaryOperationTestUtil.hpp"

#include "gtest/gtest.h"

namespace quickstep {

TEST(SubtractBinaryOperationTest, ResultTypeForPartialArgumentTypesTest) {
  // If both argument types are unknown, the result is unknown.
  EXPECT_EQ(nullptr,
            SubtractBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                nullptr, nullptr));

  // If one of the arguments is a Double, the result is Double (since it has
  // the highest precedence of the numeric types).
  const Type *result_type = SubtractBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
      nullptr,
      &TypeFactory::GetType(kDouble, false));
  ASSERT_NE(result_type, nullptr);
  EXPECT_TRUE(TypeFactory::GetType(kDouble, true).equals(*result_type));

  result_type = SubtractBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
      nullptr,
      &TypeFactory::GetType(kDouble, true));
  ASSERT_NE(result_type, nullptr);
  EXPECT_TRUE(TypeFactory::GetType(kDouble, true).equals(*result_type));

  result_type = SubtractBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
      &TypeFactory::GetType(kDouble, false),
      nullptr);
  ASSERT_NE(result_type, nullptr);
  EXPECT_TRUE(TypeFactory::GetType(kDouble, true).equals(*result_type));

  result_type = SubtractBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
      &TypeFactory::GetType(kDouble, true),
      nullptr);
  ASSERT_NE(result_type, nullptr);
  EXPECT_TRUE(TypeFactory::GetType(kDouble, true).equals(*result_type));

  // If the left argument (the minuend) is an interval, then the subtrahend and
  // the result must also be the same kind of interval.
  result_type = SubtractBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
      &TypeFactory::GetType(kDatetimeInterval, false),
      nullptr);
  ASSERT_NE(result_type, nullptr);
  EXPECT_TRUE(TypeFactory::GetType(kDatetimeInterval, true).equals(*result_type));

  result_type = SubtractBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
      &TypeFactory::GetType(kDatetimeInterval, true),
      nullptr);
  ASSERT_NE(result_type, nullptr);
  EXPECT_TRUE(TypeFactory::GetType(kDatetimeInterval, true).equals(*result_type));

  result_type = SubtractBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
      &TypeFactory::GetType(kYearMonthInterval, false),
      nullptr);
  ASSERT_NE(result_type, nullptr);
  EXPECT_TRUE(TypeFactory::GetType(kYearMonthInterval, true).equals(*result_type));

  result_type = SubtractBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
      &TypeFactory::GetType(kYearMonthInterval, true),
      nullptr);
  ASSERT_NE(result_type, nullptr);
  EXPECT_TRUE(TypeFactory::GetType(kYearMonthInterval, true).equals(*result_type));

  // If the right argument (the subtrahend) is a Datetime, then the minuend
  // must also be a Datetime and the result is an interval.
  result_type = SubtractBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
      nullptr,
      &TypeFactory::GetType(kDatetime, false));
  ASSERT_NE(result_type, nullptr);
  EXPECT_TRUE(TypeFactory::GetType(kDatetimeInterval, true).equals(*result_type));

  result_type = SubtractBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
      nullptr,
      &TypeFactory::GetType(kDatetime, true));
  ASSERT_NE(result_type, nullptr);
  EXPECT_TRUE(TypeFactory::GetType(kDatetimeInterval, true).equals(*result_type));

  // Other argument types give ambiguous or inapplicable results.
  for (const TypeID left_type_id : {kInt, kLong, kFloat, kDatetime, kChar, kVarChar}) {
    const Type *left_type = TypeFactory::TypeRequiresLengthParameter(left_type_id)
                            ? &TypeFactory::GetType(left_type_id, 10, false)
                            : &TypeFactory::GetType(left_type_id, false);
    EXPECT_EQ(nullptr,
              SubtractBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  left_type,
                  nullptr));
    EXPECT_EQ(nullptr,
              SubtractBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  &left_type->getNullableVersion(),
                  nullptr));
  }

  for (const TypeID right_type_id
       : {kInt, kLong, kFloat, kDatetimeInterval, kYearMonthInterval, kChar, kVarChar}) {
    const Type *right_type = TypeFactory::TypeRequiresLengthParameter(right_type_id)
                             ? &TypeFactory::GetType(right_type_id, 10, false)
                             : &TypeFactory::GetType(right_type_id, false);
    EXPECT_EQ(nullptr,
              SubtractBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  nullptr,
                  right_type));
    EXPECT_EQ(nullptr,
              SubtractBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  nullptr,
                  &right_type->getNullableVersion()));
  }
}

TEST(SubtractBinaryOperationTest, PartialTypeSignatureIsPlausibleTest) {
  // --------------------------------------------------------------------------
  // All 3 Types unknown.
  EXPECT_TRUE(SubtractBinaryOperation::Instance().partialTypeSignatureIsPlausible(
      nullptr, nullptr, nullptr));

  // --------------------------------------------------------------------------
  // Result type known, both argument types unknown.
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndUnknownArguments(
      SubtractBinaryOperation::Instance(),
      {kInt, kLong, kFloat, kDouble, kDatetime, kDatetimeInterval, kYearMonthInterval});

  // --------------------------------------------------------------------------
  // Result type unknown, one argument type known.
  BinaryOperationTestUtil::CheckPlausibilityWithUnknownResultAndSingleKnownArgument(
      SubtractBinaryOperation::Instance(),
      {kInt, kLong, kFloat, kDouble, kDatetime, kDatetimeInterval, kYearMonthInterval},
      true,
      true);

  // --------------------------------------------------------------------------
  // Result type and left argument type known.
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      SubtractBinaryOperation::Instance(), kInt, {kInt}, true, false);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      SubtractBinaryOperation::Instance(), kLong, {kInt, kLong}, true, false);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      SubtractBinaryOperation::Instance(), kFloat, {kInt, kFloat}, true, false);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      SubtractBinaryOperation::Instance(), kDouble, {kInt, kLong, kFloat, kDouble}, true, false);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      SubtractBinaryOperation::Instance(), kDatetime, {kDatetime}, true, false);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      SubtractBinaryOperation::Instance(),
      kDatetimeInterval, {kDatetime, kDatetimeInterval}, true, false);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      SubtractBinaryOperation::Instance(), kYearMonthInterval, {kYearMonthInterval}, true, false);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      SubtractBinaryOperation::Instance(), kChar, {}, true, false);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      SubtractBinaryOperation::Instance(), kVarChar, {}, true, false);

  // --------------------------------------------------------------------------
  // Result type and right argument type known.
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      SubtractBinaryOperation::Instance(), kInt, {kInt}, false, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      SubtractBinaryOperation::Instance(), kLong, {kInt, kLong}, false, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      SubtractBinaryOperation::Instance(), kFloat, {kInt, kFloat}, false, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      SubtractBinaryOperation::Instance(), kDouble, {kInt, kLong, kFloat, kDouble}, false, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      SubtractBinaryOperation::Instance(),
      kDatetime, {kDatetimeInterval, kYearMonthInterval}, false, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      SubtractBinaryOperation::Instance(),
      kDatetimeInterval, {kDatetime, kDatetimeInterval}, false, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      SubtractBinaryOperation::Instance(), kYearMonthInterval, {kYearMonthInterval}, false, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      SubtractBinaryOperation::Instance(), kChar, {}, false, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      SubtractBinaryOperation::Instance(), kVarChar, {}, false, true);
}

TEST(SubtractBinaryOperationTest, PushDownTypeHintTest) {
  const Type *result_type = nullptr;
  std::pair<const Type*, const Type*> hints
      = SubtractBinaryOperation::Instance().pushDownTypeHint(result_type);
  EXPECT_EQ(nullptr, hints.first);
  EXPECT_EQ(nullptr, hints.second);

  for (const TypeID type_id
       : {kInt, kLong, kFloat, kDouble, kYearMonthInterval}) {
    result_type = &TypeFactory::GetType(type_id, false);
    hints = SubtractBinaryOperation::Instance().pushDownTypeHint(result_type);
    ASSERT_NE(hints.first, nullptr);
    ASSERT_NE(hints.second, nullptr);
    EXPECT_TRUE(result_type->equals(*hints.first));
    EXPECT_TRUE(result_type->equals(*hints.second));

    result_type = &TypeFactory::GetType(type_id, true);
    hints = SubtractBinaryOperation::Instance().pushDownTypeHint(result_type);
    ASSERT_NE(hints.first, nullptr);
    ASSERT_NE(hints.second, nullptr);
    EXPECT_TRUE(result_type->equals(*hints.first));
    EXPECT_TRUE(result_type->equals(*hints.second));
  }

  // A hint of Datetime means the left argument should be Datetime and the
  // right could be either interval type.
  result_type = &TypeFactory::GetType(kDatetime, false);
  hints = SubtractBinaryOperation::Instance().pushDownTypeHint(result_type);
  ASSERT_NE(hints.first, nullptr);
  EXPECT_TRUE(result_type->equals(*hints.first));
  EXPECT_EQ(nullptr, hints.second);

  result_type = &TypeFactory::GetType(kDatetime, true);
  hints = SubtractBinaryOperation::Instance().pushDownTypeHint(result_type);
  ASSERT_NE(hints.first, nullptr);
  EXPECT_TRUE(result_type->equals(*hints.first));
  EXPECT_EQ(nullptr, hints.second);

  // A hint of DatetimeInterval is ambiguous, because the arguments could be
  // a pair of DateTimes or a pair of DatetimeIntervals.
  result_type = &TypeFactory::GetType(kDatetimeInterval, false);
  hints = SubtractBinaryOperation::Instance().pushDownTypeHint(result_type);
  EXPECT_EQ(nullptr, hints.first);
  EXPECT_EQ(nullptr, hints.second);

  result_type = &TypeFactory::GetType(kDatetimeInterval, true);
  hints = SubtractBinaryOperation::Instance().pushDownTypeHint(result_type);
  EXPECT_EQ(nullptr, hints.first);
  EXPECT_EQ(nullptr, hints.second);

  for (const TypeID string_type_id : {kChar, kVarChar}) {
    result_type = &TypeFactory::GetType(string_type_id, 10, false);
    hints = SubtractBinaryOperation::Instance().pushDownTypeHint(result_type);
    EXPECT_EQ(nullptr, hints.first);
    EXPECT_EQ(nullptr, hints.second);

    result_type = &TypeFactory::GetType(string_type_id, 10, true);
    hints = SubtractBinaryOperation::Instance().pushDownTypeHint(result_type);
    EXPECT_EQ(nullptr, hints.first);
    EXPECT_EQ(nullptr, hints.second);
  }
}

}  // namespace quickstep
