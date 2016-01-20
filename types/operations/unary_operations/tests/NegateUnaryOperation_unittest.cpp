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

#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/operations/unary_operations/ArithmeticUnaryOperations.hpp"
#include "types/operations/unary_operations/UnaryOperation.hpp"

#include "gtest/gtest.h"

namespace quickstep {

TEST(NegateUnaryOperationTest, FixedNullableResultTypeTest) {
  EXPECT_EQ(nullptr, NegateUnaryOperation::Instance().fixedNullableResultType());
}

TEST(NegateUnaryOperationTest, ResultTypeIsPlausibleTest) {
  // Any numeric result type is plausible.
  EXPECT_TRUE(NegateUnaryOperation::Instance().resultTypeIsPlausible(
      TypeFactory::GetType(kInt, false)));
  EXPECT_TRUE(NegateUnaryOperation::Instance().resultTypeIsPlausible(
      TypeFactory::GetType(kInt, true)));
  EXPECT_TRUE(NegateUnaryOperation::Instance().resultTypeIsPlausible(
      TypeFactory::GetType(kLong, false)));
  EXPECT_TRUE(NegateUnaryOperation::Instance().resultTypeIsPlausible(
      TypeFactory::GetType(kLong, true)));
  EXPECT_TRUE(NegateUnaryOperation::Instance().resultTypeIsPlausible(
      TypeFactory::GetType(kFloat, false)));
  EXPECT_TRUE(NegateUnaryOperation::Instance().resultTypeIsPlausible(
      TypeFactory::GetType(kFloat, true)));
  EXPECT_TRUE(NegateUnaryOperation::Instance().resultTypeIsPlausible(
      TypeFactory::GetType(kDouble, false)));
  EXPECT_TRUE(NegateUnaryOperation::Instance().resultTypeIsPlausible(
      TypeFactory::GetType(kDouble, true)));

  // Interval types are also plausible.
  EXPECT_TRUE(NegateUnaryOperation::Instance().resultTypeIsPlausible(
      TypeFactory::GetType(kDatetimeInterval, false)));
  EXPECT_TRUE(NegateUnaryOperation::Instance().resultTypeIsPlausible(
      TypeFactory::GetType(kDatetimeInterval, true)));
  EXPECT_TRUE(NegateUnaryOperation::Instance().resultTypeIsPlausible(
      TypeFactory::GetType(kYearMonthInterval, false)));
  EXPECT_TRUE(NegateUnaryOperation::Instance().resultTypeIsPlausible(
      TypeFactory::GetType(kYearMonthInterval, true)));

  // Other types can not be the result of a negation.
  EXPECT_FALSE(NegateUnaryOperation::Instance().resultTypeIsPlausible(
      TypeFactory::GetType(kDatetime, false)));
  EXPECT_FALSE(NegateUnaryOperation::Instance().resultTypeIsPlausible(
      TypeFactory::GetType(kDatetime, true)));
  EXPECT_FALSE(NegateUnaryOperation::Instance().resultTypeIsPlausible(
      TypeFactory::GetType(kChar, 10, false)));
  EXPECT_FALSE(NegateUnaryOperation::Instance().resultTypeIsPlausible(
      TypeFactory::GetType(kChar, 10, true)));
  EXPECT_FALSE(NegateUnaryOperation::Instance().resultTypeIsPlausible(
      TypeFactory::GetType(kVarChar, 10, false)));
  EXPECT_FALSE(NegateUnaryOperation::Instance().resultTypeIsPlausible(
      TypeFactory::GetType(kVarChar, 10, true)));
}

void CheckArgumentHintSameAsResultHint(const Type *result_hint) {
  const Type *argument_hint
      = NegateUnaryOperation::Instance().pushDownTypeHint(result_hint);

  if (result_hint != nullptr) {
    ASSERT_NE(argument_hint, nullptr);
    EXPECT_TRUE(result_hint->equals(*argument_hint));
  } else {
    EXPECT_EQ(nullptr, argument_hint);
  }
}

TEST(NegateUnaryOperationTest, PushDownTypeHintTest) {
  // For any applicable type, the argument and result type for negation are the
  // same.
  CheckArgumentHintSameAsResultHint(nullptr);
  CheckArgumentHintSameAsResultHint(&TypeFactory::GetType(kInt, false));
  CheckArgumentHintSameAsResultHint(&TypeFactory::GetType(kInt, true));
  CheckArgumentHintSameAsResultHint(&TypeFactory::GetType(kLong, false));
  CheckArgumentHintSameAsResultHint(&TypeFactory::GetType(kLong, true));
  CheckArgumentHintSameAsResultHint(&TypeFactory::GetType(kFloat, false));
  CheckArgumentHintSameAsResultHint(&TypeFactory::GetType(kFloat, true));
  CheckArgumentHintSameAsResultHint(&TypeFactory::GetType(kDouble, false));
  CheckArgumentHintSameAsResultHint(&TypeFactory::GetType(kDouble, true));
  CheckArgumentHintSameAsResultHint(&TypeFactory::GetType(kDatetimeInterval, false));
  CheckArgumentHintSameAsResultHint(&TypeFactory::GetType(kDatetimeInterval, true));

  // Expect NULL if not applicable.
  EXPECT_EQ(nullptr,
            NegateUnaryOperation::Instance().pushDownTypeHint(
                &TypeFactory::GetType(kDatetime, false)));
  EXPECT_EQ(nullptr,
            NegateUnaryOperation::Instance().pushDownTypeHint(
                &TypeFactory::GetType(kDatetime, true)));
  EXPECT_EQ(nullptr,
            NegateUnaryOperation::Instance().pushDownTypeHint(
                &TypeFactory::GetType(kChar, 10, false)));
  EXPECT_EQ(nullptr,
            NegateUnaryOperation::Instance().pushDownTypeHint(
                &TypeFactory::GetType(kChar, 10, true)));
  EXPECT_EQ(nullptr,
            NegateUnaryOperation::Instance().pushDownTypeHint(
                &TypeFactory::GetType(kVarChar, 10, false)));
  EXPECT_EQ(nullptr,
            NegateUnaryOperation::Instance().pushDownTypeHint(
                &TypeFactory::GetType(kVarChar, 10, true)));
}

}  // namespace quickstep
