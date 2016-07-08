/**
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

#include <string>

#include "types/DecimalLit.hpp"
#include "types/DecimalType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypedValue.hpp"

#include "gtest/gtest.h"

namespace quickstep {

TEST(DecimalTypeTest, PrintValueTest) {
  const Type &decimal_type = TypeFactory::GetType(kDecimal);

  // Try integer version.
  DecimalLit integer_decimal = DecimalLit(36509);
  TypedValue value_for_integer_decimal(integer_decimal);
  EXPECT_EQ(std::string("36509.00"),
            decimal_type.printValueToString(value_for_integer_decimal));

  // Try double version.
  DecimalLit double_decimal = DecimalLit(36509.65);
  TypedValue value_for_double_decimal(double_decimal);
  EXPECT_EQ(std::string("36509.65"),
            decimal_type.printValueToString(value_for_double_decimal));

  // Try truncation of double version.
  DecimalLit double_decimal_truncated = DecimalLit(36509.6526762);
  TypedValue value_for_double_decimal_truncated(double_decimal_truncated);
  EXPECT_EQ(std::string("36509.65"),
            decimal_type.printValueToString(
                value_for_double_decimal_truncated));

  // Test that number is truncated, not rounded.
  double_decimal_truncated = DecimalLit(36509.6599999);
  TypedValue value_for_double_decimal_truncated_other(double_decimal_truncated);
  EXPECT_EQ(std::string("36509.65"),
            decimal_type.printValueToString(
                value_for_double_decimal_truncated));
}

TEST(DecimalTypeTest, DecimalLitOperationsTest) {
  const Type &decimal_type = TypeFactory::GetType(kDecimal);

  DecimalLit decimal_a(560.35);
  DecimalLit decimal_b(439.65);

  EXPECT_EQ(std::string("1000.00"),
            decimal_type.printValueToString(TypedValue(decimal_a + decimal_b)));

  EXPECT_EQ(std::string("120.70"),
            decimal_type.printValueToString(TypedValue(decimal_a - decimal_b)));

  EXPECT_EQ(std::string("246357.87"),
            decimal_type.printValueToString(TypedValue(decimal_a * decimal_b)));

  EXPECT_EQ(std::string("1.27"),
            decimal_type.printValueToString(TypedValue(decimal_a / decimal_b)));

  EXPECT_EQ(std::string("120.70"),
            decimal_type.printValueToString(TypedValue(decimal_a % decimal_b)));
}

}  // namespace quickstep
