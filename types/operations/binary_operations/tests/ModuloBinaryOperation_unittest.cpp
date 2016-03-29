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
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/binary_operations/ModuloBinaryOperation.hpp"
#include "types/operations/binary_operations/tests/BinaryOperationTestUtil.hpp"

#include "gtest/gtest.h"

namespace quickstep {

TEST(ModuloBinaryOperationTest, ResultTypeForPartialArgumentTypesTest) {
  // If both argument types are unknown, the result is unknown.
  EXPECT_EQ(nullptr,
            ModuloBinaryOperation::Instance().resultTypeForPartialArgumentTypes(nullptr, nullptr));

  // Double has highest precedence of numeric types, and dividing an interval
  // type by a numeric type yields the same interval type.
  for (const TypeID type_id : {kDouble, kDatetimeInterval, kYearMonthInterval}) {
    const Type *result_type = ModuloBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
        &TypeFactory::GetType(type_id, false),
        nullptr);
    ASSERT_NE(result_type, nullptr);
    EXPECT_TRUE(TypeFactory::GetType(type_id, true).equals(*result_type));

    result_type = ModuloBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
        &TypeFactory::GetType(type_id, true),
        nullptr);
    ASSERT_NE(result_type, nullptr);
    EXPECT_TRUE(TypeFactory::GetType(type_id, true).equals(*result_type));
  }

  // Other types for left argument are ambiguous or inapplicable.
  for (const TypeID type_id : {kInt, kLong, kFloat, kDatetime}) {
    EXPECT_EQ(nullptr,
              ModuloBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  &TypeFactory::GetType(type_id, false),
                  nullptr));
    EXPECT_EQ(nullptr,
              ModuloBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  &TypeFactory::GetType(type_id, true),
                  nullptr));
  }
  for (const TypeID type_id : {kChar, kVarChar}) {
    EXPECT_EQ(nullptr,
              ModuloBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  &TypeFactory::GetType(type_id, 10, false),
                  nullptr));
    EXPECT_EQ(nullptr,
              ModuloBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  &TypeFactory::GetType(type_id, 10, true),
                  nullptr));
  }

  // Can't determine a return type from just the divisor (only kInt and kLong types
  // are allowed as the divisor, but the result may be a numeric type OR an
  // interval type).
  for (const TypeID type_id
       : {kInt, kLong, kFloat, kDouble, kDatetime, kDatetimeInterval, kYearMonthInterval}) {
    EXPECT_EQ(nullptr,
              ModuloBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  nullptr,
                  &TypeFactory::GetType(type_id, false)));
    EXPECT_EQ(nullptr,
              ModuloBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  nullptr,
                  &TypeFactory::GetType(type_id, true)));
  }
  for (const TypeID type_id : {kChar, kVarChar}) {
    EXPECT_EQ(nullptr,
              ModuloBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  nullptr,
                  &TypeFactory::GetType(type_id, 10, false)));
    EXPECT_EQ(nullptr,
              ModuloBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  nullptr,
                  &TypeFactory::GetType(type_id, 10, true)));
  }
}

TEST(ModuloBinaryOperationTest, PartialTypeSignatureIsPlausibleTest) {
  // --------------------------------------------------------------------------
  // All 3 Types unknown.
  EXPECT_TRUE(ModuloBinaryOperation::Instance().partialTypeSignatureIsPlausible(
      nullptr, nullptr, nullptr));

  // --------------------------------------------------------------------------
  // Result type known, both argument types unknown.
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndUnknownArguments(
      ModuloBinaryOperation::Instance(),
      {kInt, kLong, kFloat, kDouble, kDatetimeInterval, kYearMonthInterval});

  // --------------------------------------------------------------------------
  // Result type unknown, left argument type known.
  BinaryOperationTestUtil::CheckPlausibilityWithUnknownResultAndSingleKnownArgument(
      ModuloBinaryOperation::Instance(),
      {kInt, kLong, kFloat, kDouble, kDatetimeInterval, kYearMonthInterval},
      true,
      false);

  // --------------------------------------------------------------------------
  // Result type unknown, right argument type known.
  BinaryOperationTestUtil::CheckPlausibilityWithUnknownResultAndSingleKnownArgument(
      ModuloBinaryOperation::Instance(),
      {kInt, kLong, kFloat, kDouble},
      false,
      true);

  // --------------------------------------------------------------------------
  // Result type and left argument type known.
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      ModuloBinaryOperation::Instance(), kInt, {kInt}, true, false);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      ModuloBinaryOperation::Instance(), kLong, {kInt, kLong}, true, false);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      ModuloBinaryOperation::Instance(), kFloat, {kInt, kFloat}, true, false);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      ModuloBinaryOperation::Instance(), kDouble, {kInt, kLong, kFloat, kDouble}, true, false);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      ModuloBinaryOperation::Instance(), kDatetime, {}, true, false);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      ModuloBinaryOperation::Instance(), kDatetimeInterval, {kDatetimeInterval}, true, false);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      ModuloBinaryOperation::Instance(), kYearMonthInterval, {kYearMonthInterval}, true, false);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      ModuloBinaryOperation::Instance(), kChar, {}, true, false);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      ModuloBinaryOperation::Instance(), kVarChar, {}, true, false);

  // --------------------------------------------------------------------------
  // Result type and right argument type known.
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      ModuloBinaryOperation::Instance(), kInt, {kInt}, false, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      ModuloBinaryOperation::Instance(), kLong, {kInt, kLong}, false, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      ModuloBinaryOperation::Instance(), kFloat, {kInt, kFloat}, false, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      ModuloBinaryOperation::Instance(), kDouble, {kInt, kLong, kFloat, kDouble}, false, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      ModuloBinaryOperation::Instance(), kDatetime, {}, false, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      ModuloBinaryOperation::Instance(),
      kDatetimeInterval, {kInt, kLong, kFloat, kDouble}, false, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      ModuloBinaryOperation::Instance(),
      kYearMonthInterval, {kInt, kLong, kFloat, kDouble}, false, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      ModuloBinaryOperation::Instance(), kChar, {}, false, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      ModuloBinaryOperation::Instance(), kVarChar, {}, false, true);
}

TEST(ModuloBinaryOperationTest, PushDownTypeHintTest) {
  const Type *result_type = nullptr;
  std::pair<const Type*, const Type*> hints
      = ModuloBinaryOperation::Instance().pushDownTypeHint(result_type);
  EXPECT_EQ(nullptr, hints.first);
  EXPECT_EQ(nullptr, hints.second);

  // For any numeric result type, hints for arguments should be the same type.
  for (const TypeID type_id : {kInt, kLong, kFloat, kDouble}) {
    result_type = &TypeFactory::GetType(type_id, false);
    hints = ModuloBinaryOperation::Instance().pushDownTypeHint(result_type);
    ASSERT_NE(hints.first, nullptr);
    ASSERT_NE(hints.second, nullptr);
    EXPECT_TRUE(result_type->equals(*hints.first));
    EXPECT_TRUE(result_type->equals(*hints.second));

    result_type = &TypeFactory::GetType(type_id, true);
    hints = ModuloBinaryOperation::Instance().pushDownTypeHint(result_type);
    ASSERT_NE(hints.first, nullptr);
    ASSERT_NE(hints.second, nullptr);
    EXPECT_TRUE(result_type->equals(*hints.first));
    EXPECT_TRUE(result_type->equals(*hints.second));
  }

  // A result of an interval type means that the left argument should be the
  // same interval type and the right could be any numeric type.
  for (const TypeID type_id : {kDatetimeInterval, kYearMonthInterval}) {
    result_type = &TypeFactory::GetType(type_id, false);
    hints = ModuloBinaryOperation::Instance().pushDownTypeHint(result_type);
    ASSERT_NE(hints.first, nullptr);
    EXPECT_TRUE(result_type->equals(*hints.first));
    EXPECT_EQ(nullptr, hints.second);

    result_type = &TypeFactory::GetType(type_id, true);
    hints = ModuloBinaryOperation::Instance().pushDownTypeHint(result_type);
    ASSERT_NE(hints.first, nullptr);
    EXPECT_TRUE(result_type->equals(*hints.first));
    EXPECT_EQ(nullptr, hints.second);
  }

  // Division can't produce a Datetime or a string.
  result_type = &TypeFactory::GetType(kDatetime, false);
  hints = ModuloBinaryOperation::Instance().pushDownTypeHint(result_type);
  EXPECT_EQ(nullptr, hints.first);
  EXPECT_EQ(nullptr, hints.second);

  result_type = &TypeFactory::GetType(kDatetime, true);
  hints = ModuloBinaryOperation::Instance().pushDownTypeHint(result_type);
  EXPECT_EQ(nullptr, hints.first);
  EXPECT_EQ(nullptr, hints.second);

  for (const TypeID type_id : {kChar, kVarChar}) {
    result_type = &TypeFactory::GetType(type_id, 10, false);
    hints = ModuloBinaryOperation::Instance().pushDownTypeHint(result_type);
    EXPECT_EQ(nullptr, hints.first);
    EXPECT_EQ(nullptr, hints.second);

    result_type = &TypeFactory::GetType(type_id, 10, true);
    hints = ModuloBinaryOperation::Instance().pushDownTypeHint(result_type);
    EXPECT_EQ(nullptr, hints.first);
    EXPECT_EQ(nullptr, hints.second);
  }
}

}  // namespace quickstep
