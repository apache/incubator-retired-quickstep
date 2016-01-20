/**
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

#include <initializer_list>
#include <utility>

#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/operations/binary_operations/AddBinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/binary_operations/tests/BinaryOperationTestUtil.hpp"

#include "gtest/gtest.h"

namespace quickstep {

TEST(AddBinaryOperationTest, ResultTypeForPartialArgumentTypesTest) {
  // If both argument types are unknown, the result is unknown.
  EXPECT_EQ(nullptr,
            AddBinaryOperation::Instance().resultTypeForPartialArgumentTypes(nullptr, nullptr));

  // If one of the arguments is a Double, the result is Double (since it has
  // the highest precedence of the numeric types).
  const Type *result_type = AddBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
      nullptr,
      &TypeFactory::GetType(kDouble, false));
  ASSERT_NE(result_type, nullptr);
  EXPECT_TRUE(TypeFactory::GetType(kDouble, true).equals(*result_type));

  result_type = AddBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
      nullptr,
      &TypeFactory::GetType(kDouble, true));
  ASSERT_NE(result_type, nullptr);
  EXPECT_TRUE(TypeFactory::GetType(kDouble, true).equals(*result_type));

  result_type = AddBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
      &TypeFactory::GetType(kDouble, false),
      nullptr);
  ASSERT_NE(result_type, nullptr);
  EXPECT_TRUE(TypeFactory::GetType(kDouble, true).equals(*result_type));

  result_type = AddBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
      &TypeFactory::GetType(kDouble, true),
      nullptr);
  ASSERT_NE(result_type, nullptr);
  EXPECT_TRUE(TypeFactory::GetType(kDouble, true).equals(*result_type));

  // If one of the arguments is a Datetime, then it can be added with either
  // interval type and will always yield a Datetime.
  result_type = AddBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
      nullptr,
      &TypeFactory::GetType(kDatetime, false));
  ASSERT_NE(result_type, nullptr);
  EXPECT_TRUE(TypeFactory::GetType(kDatetime, true).equals(*result_type));

  result_type = AddBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
      nullptr,
      &TypeFactory::GetType(kDatetime, true));
  ASSERT_NE(result_type, nullptr);
  EXPECT_TRUE(TypeFactory::GetType(kDatetime, true).equals(*result_type));

  result_type = AddBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
      &TypeFactory::GetType(kDatetime, false),
      nullptr);
  ASSERT_NE(result_type, nullptr);
  EXPECT_TRUE(TypeFactory::GetType(kDatetime, true).equals(*result_type));

  result_type = AddBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
      &TypeFactory::GetType(kDatetime, true),
      nullptr);
  ASSERT_NE(result_type, nullptr);
  EXPECT_TRUE(TypeFactory::GetType(kDatetime, true).equals(*result_type));

  // Any other argument type is either inapplicable or yields an ambiguous
  // result.
  for (const TypeID type_id
       : {kInt, kLong, kFloat, kDatetimeInterval, kYearMonthInterval}) {
    EXPECT_EQ(nullptr,
              AddBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  nullptr,
                  &TypeFactory::GetType(type_id, false)));
    EXPECT_EQ(nullptr,
              AddBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  nullptr,
                  &TypeFactory::GetType(type_id, true)));
    EXPECT_EQ(nullptr,
              AddBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  &TypeFactory::GetType(type_id, false),
                  nullptr));
    EXPECT_EQ(nullptr,
              AddBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  &TypeFactory::GetType(type_id, true),
                  nullptr));
  }

  for (const TypeID type_id : {kChar, kVarChar}) {
    EXPECT_EQ(nullptr,
              AddBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  nullptr,
                  &TypeFactory::GetType(type_id, 10, false)));
    EXPECT_EQ(nullptr,
              AddBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  nullptr,
                  &TypeFactory::GetType(type_id, 10, true)));
    EXPECT_EQ(nullptr,
              AddBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  &TypeFactory::GetType(type_id, 10, false),
                  nullptr));
    EXPECT_EQ(nullptr,
              AddBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  &TypeFactory::GetType(type_id, 10, true),
                  nullptr));
  }
}

TEST(AddBinaryOperationTest, PartialTypeSignatureIsPlausibleTest) {
  // --------------------------------------------------------------------------
  // All 3 Types unknown.
  EXPECT_TRUE(AddBinaryOperation::Instance().partialTypeSignatureIsPlausible(
      nullptr, nullptr, nullptr));

  // --------------------------------------------------------------------------
  // Result type known, both argument types unknown.

  // When both arguments are unknown, any nullable result type that addition
  // might possibly return is plausible.
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndUnknownArguments(
      AddBinaryOperation::Instance(),
      {kInt, kLong, kFloat, kDouble, kDatetime, kDatetimeInterval, kYearMonthInterval});

  // --------------------------------------------------------------------------
  // Result type unknown, one argument type known.
  BinaryOperationTestUtil::CheckPlausibilityWithUnknownResultAndSingleKnownArgument(
      AddBinaryOperation::Instance(),
      {kInt, kLong, kFloat, kDouble, kDatetime, kDatetimeInterval, kYearMonthInterval},
      true,
      true);

  // --------------------------------------------------------------------------
  // Result type and one argument type known.
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      AddBinaryOperation::Instance(), kInt, {kInt}, true, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      AddBinaryOperation::Instance(), kLong, {kInt, kLong}, true, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      AddBinaryOperation::Instance(), kFloat, {kInt, kFloat}, true, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      AddBinaryOperation::Instance(), kDouble, {kInt, kLong, kFloat, kDouble}, true, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      AddBinaryOperation::Instance(),
      kDatetime, {kDatetime, kDatetimeInterval, kYearMonthInterval}, true, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      AddBinaryOperation::Instance(), kDatetimeInterval, {kDatetimeInterval}, true, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      AddBinaryOperation::Instance(), kYearMonthInterval, {kYearMonthInterval}, true, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      AddBinaryOperation::Instance(), kChar, {}, true, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      AddBinaryOperation::Instance(), kVarChar, {}, true, true);
}

TEST(AddBinaryOperationTest, PushDownTypeHintTest) {
  const Type *result_type = nullptr;
  std::pair<const Type*, const Type*> hints
      = AddBinaryOperation::Instance().pushDownTypeHint(result_type);
  EXPECT_EQ(nullptr, hints.first);
  EXPECT_EQ(nullptr, hints.second);

  for (const TypeID type_id
       : {kInt, kLong, kFloat, kDouble, kDatetimeInterval, kYearMonthInterval}) {
    result_type = &TypeFactory::GetType(type_id, false);
    hints = AddBinaryOperation::Instance().pushDownTypeHint(result_type);
    ASSERT_NE(hints.first, nullptr);
    ASSERT_NE(hints.second, nullptr);
    EXPECT_TRUE(result_type->equals(*hints.first));
    EXPECT_TRUE(result_type->equals(*hints.second));

    result_type = &TypeFactory::GetType(type_id, true);
    hints = AddBinaryOperation::Instance().pushDownTypeHint(result_type);
    ASSERT_NE(hints.first, nullptr);
    ASSERT_NE(hints.second, nullptr);
    EXPECT_TRUE(result_type->equals(*hints.first));
    EXPECT_TRUE(result_type->equals(*hints.second));
  }

  // A hint of Datetime is ambiguous, because one argument should be Datetime
  // and the other should be some interval type.
  result_type = &TypeFactory::GetType(kDatetime, false);
  hints = AddBinaryOperation::Instance().pushDownTypeHint(result_type);
  EXPECT_EQ(nullptr, hints.first);
  EXPECT_EQ(nullptr, hints.second);

  result_type = &TypeFactory::GetType(kDatetime, true);
  hints = AddBinaryOperation::Instance().pushDownTypeHint(result_type);
  EXPECT_EQ(nullptr, hints.first);
  EXPECT_EQ(nullptr, hints.second);

  for (const TypeID string_type_id : {kChar, kVarChar}) {
    result_type = &TypeFactory::GetType(string_type_id, 10, false);
    hints = AddBinaryOperation::Instance().pushDownTypeHint(result_type);
    EXPECT_EQ(nullptr, hints.first);
    EXPECT_EQ(nullptr, hints.second);

    result_type = &TypeFactory::GetType(string_type_id, 10, true);
    hints = AddBinaryOperation::Instance().pushDownTypeHint(result_type);
    EXPECT_EQ(nullptr, hints.first);
    EXPECT_EQ(nullptr, hints.second);
  }
}

}  // namespace quickstep
