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
#include "types/operations/binary_operations/MultiplyBinaryOperation.hpp"
#include "types/operations/binary_operations/tests/BinaryOperationTestUtil.hpp"

#include "gtest/gtest.h"

namespace quickstep {

TEST(MultiplyBinaryOperationTest, ResultTypeForPartialArgumentTypesTest) {
  // If both argument types are unknown, the result is unknown.
  EXPECT_EQ(nullptr,
            MultiplyBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                nullptr, nullptr));

  // If one of the arguments is an interval, then the other argument may be any
  // numeric type and the result is an interval of the same type.
  for (const TypeID interval_type_id : {kDatetimeInterval, kYearMonthInterval}) {
      const Type *result_type
          = MultiplyBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                nullptr,
                &TypeFactory::GetType(interval_type_id, false));
      ASSERT_NE(result_type, nullptr);
      EXPECT_TRUE(TypeFactory::GetType(interval_type_id, true).equals(*result_type));

      result_type
          = MultiplyBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                nullptr,
                &TypeFactory::GetType(interval_type_id, true));
      ASSERT_NE(result_type, nullptr);
      EXPECT_TRUE(TypeFactory::GetType(interval_type_id, true).equals(*result_type));

      result_type
          = MultiplyBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                &TypeFactory::GetType(interval_type_id, false),
                nullptr);
      ASSERT_NE(result_type, nullptr);
      EXPECT_TRUE(TypeFactory::GetType(interval_type_id, true).equals(*result_type));

      result_type
          = MultiplyBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                &TypeFactory::GetType(interval_type_id, true),
                nullptr);
      ASSERT_NE(result_type, nullptr);
      EXPECT_TRUE(TypeFactory::GetType(interval_type_id, true).equals(*result_type));
  }

  // Any other argument type is either inapplicable or yields an ambiguous
  // result.
  for (const TypeID type_id
       : {kInt, kLong, kFloat, kDouble, kDatetime}) {
    EXPECT_EQ(nullptr,
              MultiplyBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  nullptr,
                  &TypeFactory::GetType(type_id, false)));
    EXPECT_EQ(nullptr,
              MultiplyBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  nullptr,
                  &TypeFactory::GetType(type_id, true)));
    EXPECT_EQ(nullptr,
              MultiplyBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  &TypeFactory::GetType(type_id, false),
                  nullptr));
    EXPECT_EQ(nullptr,
              MultiplyBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  &TypeFactory::GetType(type_id, true),
                  nullptr));
  }

  for (const TypeID type_id : {kChar, kVarChar}) {
    EXPECT_EQ(nullptr,
              MultiplyBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  nullptr,
                  &TypeFactory::GetType(type_id, 10, false)));
    EXPECT_EQ(nullptr,
              MultiplyBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  nullptr,
                  &TypeFactory::GetType(type_id, 10, true)));
    EXPECT_EQ(nullptr,
              MultiplyBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  &TypeFactory::GetType(type_id, 10, false),
                  nullptr));
    EXPECT_EQ(nullptr,
              MultiplyBinaryOperation::Instance().resultTypeForPartialArgumentTypes(
                  &TypeFactory::GetType(type_id, 10, true),
                  nullptr));
  }
}

TEST(MultiplyBinaryOperationTest, PartialTypeSignatureIsPlausibleTest) {
  // --------------------------------------------------------------------------
  // All 3 Types unknown.
  EXPECT_TRUE(MultiplyBinaryOperation::Instance().partialTypeSignatureIsPlausible(
      nullptr, nullptr, nullptr));

  // --------------------------------------------------------------------------
  // Result type known, both argument types unknown.
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndUnknownArguments(
      MultiplyBinaryOperation::Instance(),
      {kInt, kLong, kFloat, kDouble, kDatetimeInterval, kYearMonthInterval});

  // --------------------------------------------------------------------------
  // Result type unknown, one argument type known.
  BinaryOperationTestUtil::CheckPlausibilityWithUnknownResultAndSingleKnownArgument(
      MultiplyBinaryOperation::Instance(),
      {kInt, kLong, kFloat, kDouble, kDatetimeInterval, kYearMonthInterval},
      true,
      true);

  // --------------------------------------------------------------------------
  // Result type and one argument type known.
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      MultiplyBinaryOperation::Instance(), kInt, {kInt}, true, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      MultiplyBinaryOperation::Instance(), kLong, {kInt, kLong}, true, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      MultiplyBinaryOperation::Instance(), kFloat, {kInt, kFloat}, true, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      MultiplyBinaryOperation::Instance(), kDouble, {kInt, kLong, kFloat, kDouble}, true, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      MultiplyBinaryOperation::Instance(), kDatetime, {}, true, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      MultiplyBinaryOperation::Instance(),
      kDatetimeInterval, {kInt, kLong, kFloat, kDouble, kDatetimeInterval}, true, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      MultiplyBinaryOperation::Instance(),
      kYearMonthInterval, {kInt, kLong, kFloat, kDouble, kYearMonthInterval}, true, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      MultiplyBinaryOperation::Instance(), kChar, {}, true, true);
  BinaryOperationTestUtil::CheckPlausibilityWithKnownResultAndSingleArgument(
      MultiplyBinaryOperation::Instance(), kVarChar, {}, true, true);
}

TEST(MultiplyBinaryOperationTest, PushDownTypeHintTest) {
  const Type *result_type = nullptr;
  std::pair<const Type*, const Type*> hints
      = MultiplyBinaryOperation::Instance().pushDownTypeHint(result_type);
  EXPECT_EQ(nullptr, hints.first);
  EXPECT_EQ(nullptr, hints.second);

  for (const TypeID type_id : {kInt, kLong, kFloat, kDouble}) {
    result_type = &TypeFactory::GetType(type_id, false);
    hints = MultiplyBinaryOperation::Instance().pushDownTypeHint(result_type);
    ASSERT_NE(hints.first, nullptr);
    ASSERT_NE(hints.second, nullptr);
    EXPECT_TRUE(result_type->equals(*hints.first));
    EXPECT_TRUE(result_type->equals(*hints.second));

    result_type = &TypeFactory::GetType(type_id, true);
    hints = MultiplyBinaryOperation::Instance().pushDownTypeHint(result_type);
    ASSERT_NE(hints.first, nullptr);
    ASSERT_NE(hints.second, nullptr);
    EXPECT_TRUE(result_type->equals(*hints.first));
    EXPECT_TRUE(result_type->equals(*hints.second));
  }

  // A hint of either interval type is ambiguous, since one argument should be
  // an interval of the same type and the other other should be some numeric
  // type. Datetime is not applicable.
  for (const TypeID type_id : {kDatetime, kDatetimeInterval, kYearMonthInterval}) {
    result_type = &TypeFactory::GetType(type_id, false);
    hints = MultiplyBinaryOperation::Instance().pushDownTypeHint(result_type);
    EXPECT_EQ(nullptr, hints.first);
    EXPECT_EQ(nullptr, hints.second);

    result_type = &TypeFactory::GetType(type_id, true);
    hints = MultiplyBinaryOperation::Instance().pushDownTypeHint(result_type);
    EXPECT_EQ(nullptr, hints.first);
    EXPECT_EQ(nullptr, hints.second);
  }

  for (const TypeID string_type_id : {kChar, kVarChar}) {
    result_type = &TypeFactory::GetType(string_type_id, 10, false);
    hints = MultiplyBinaryOperation::Instance().pushDownTypeHint(result_type);
    EXPECT_EQ(nullptr, hints.first);
    EXPECT_EQ(nullptr, hints.second);

    result_type = &TypeFactory::GetType(string_type_id, 10, true);
    hints = MultiplyBinaryOperation::Instance().pushDownTypeHint(result_type);
    EXPECT_EQ(nullptr, hints.first);
    EXPECT_EQ(nullptr, hints.second);
  }
}

}  // namespace quickstep
