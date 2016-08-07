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

#include <cstddef>
#include <cstdio>
#include <limits>
#include <string>

#include "types/DoubleType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/tests/TypeTest_common.hpp"
#include "utility/MemStream.hpp"

#include "glog/logging.h"

#include "gtest/gtest.h"

// NetBSD's libc has snprintf, but it doesn't show up in the std namespace for
// C++.
#ifndef __NetBSD__
using std::snprintf;
#endif

namespace quickstep {

namespace {

// We use this custom function instead of std::to_string(), because
// std::to_string() only prints up to 6 decimal digits.
std::string DoubleAsString(const double literal_value) {
  char print_buffer[100];
  int written = snprintf(print_buffer,
                         sizeof(print_buffer),
                         "%.*g",
                         std::numeric_limits<double>::max_digits10,
                         literal_value);
  CHECK_GE(written, 0);
  CHECK_LT(static_cast<std::size_t>(written), sizeof(print_buffer));
  return std::string(print_buffer);
}

void CheckPrintValue(const double literal_value) {
  const Type &double_type = TypeFactory::GetType(kDouble);
  const TypedValue double_value = double_type.makeValue(&literal_value);
  EXPECT_EQ(DoubleAsString(literal_value), double_type.printValueToString(double_value));
}

}  // namespace

TEST(DoubleTypeTest, PrintValueTest) {
  // Doubles without a fractional part.
  CheckPrintValue(0.0);
  CheckPrintValue(123.0);
  CheckPrintValue(-123.0);

  // Try with a fractional part.
  CheckPrintValue(123.456789);
  CheckPrintValue(-123.456789);

  // Try with large and small exponents.
  CheckPrintValue(8.2575e200);
  CheckPrintValue(-8.2575e200);
  CheckPrintValue(8.2575e-200);
  CheckPrintValue(-8.2575e-200);

  // Various limits: largest and smallest normalized positive values, smallest
  // denormalized nonzero value, and largest negative value.
  CheckPrintValue(std::numeric_limits<double>::max());
  CheckPrintValue(std::numeric_limits<double>::min());
  CheckPrintValue(std::numeric_limits<double>::denorm_min());
  CheckPrintValue(std::numeric_limits<double>::lowest());
}

namespace {

static const std::size_t kPaddedLength = 200;

void CheckPrintValueToFile(const double literal_value) {
  std::string value_as_string = DoubleAsString(literal_value);

  // Test writing to FILE* with a MemStream.
  MemStream memstream;

  const Type &double_type = TypeFactory::GetType(kDouble);
  const TypedValue double_value = double_type.makeValue(&literal_value);

  // First, check printing without any padding
  double_type.printValueToFile(double_value, memstream.file());
  EXPECT_STREQ(value_as_string.c_str(), memstream.str());
  memstream.reset();

  // Specifying padding less than the length needed to print the value should
  // have no effect.
  double_type.printValueToFile(double_value, memstream.file(), 1);
  EXPECT_STREQ(value_as_string.c_str(), memstream.str());
  memstream.reset();

  // Test padding up to a specified length.
  std::string padded(kPaddedLength - value_as_string.size(), ' ');
  padded.append(value_as_string);
  double_type.printValueToFile(double_value, memstream.file(), kPaddedLength);
  EXPECT_STREQ(padded.c_str(), memstream.str());
  memstream.reset();
}

}  // namespace

TEST(DoubleTypeTest, PrintValueToFileTest) {
  // Doubles without a fractional part.
  CheckPrintValueToFile(0.0);
  CheckPrintValueToFile(123.0);
  CheckPrintValueToFile(-123.0);

  // Try with a fractional part.
  CheckPrintValueToFile(123.456789);
  CheckPrintValueToFile(-123.456789);

  // Try with large and small exponents.
  CheckPrintValueToFile(8.2575e200);
  CheckPrintValueToFile(-8.2575e200);
  CheckPrintValueToFile(8.2575e-200);
  CheckPrintValueToFile(-8.2575e-200);

  // Various limits: largest and smallest normalized positive values, smallest
  // denormalized nonzero value, and largest negative value.
  CheckPrintValueToFile(std::numeric_limits<double>::max());
  CheckPrintValueToFile(std::numeric_limits<double>::min());
  CheckPrintValueToFile(std::numeric_limits<double>::denorm_min());
  CheckPrintValueToFile(std::numeric_limits<double>::lowest());
}

TEST(DoubleTypeTest, ParseValueFromStringTest) {
  // Doubles without a fractional part.
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(0.0, "0");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(0.0, "000");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(123.0, "123");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(123.0, "0123");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(123.0, "+123");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(-123.0, "-123");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(-123.0, "-0123");

  // With a fractional part.
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(123.456, "123.456");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(-123.456, "-123.456");

  // With large and small exponents.
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(8.2575e200, "8.2575e200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(-8.2575e200, "-8.2575e200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(8.2575e-200, "8.2575e-200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(-8.2575e-200, "-8.2575e-200");

  // With exponent but no decimal.
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(8.0e200, "8e200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(-8.0e200, "-8e200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(8.0e-200, "8e-200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(-8.0e-200, "-8e-200");

  // Various double limits.
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(
      std::numeric_limits<double>::max(),
      DoubleAsString(std::numeric_limits<double>::max()));
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(
      std::numeric_limits<double>::min(),
      DoubleAsString(std::numeric_limits<double>::min()));
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(
      std::numeric_limits<double>::denorm_min(),
      DoubleAsString(std::numeric_limits<double>::denorm_min()));
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(
      std::numeric_limits<double>::lowest(),
      DoubleAsString(std::numeric_limits<double>::lowest()));

  // Slightly oddball but still acceptable formatting.
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(42.0, "42.");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(-42.0, "-42.");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(0.42, ".42");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(-0.42, "-.42");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(8.0e200, "8.e200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(-8.0e200, "-8.e200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(8.0e-200, "8.e-200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(-8.0e-200, "-8.e-200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(1.23e202, "123e200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(-1.23e202, "-123e200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(1.23e-198, "123e-200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(-1.23e-198, "-123e-200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(1.234e202, "123.4e200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(-1.234e202, "-123.4e200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(1.234e-198, "123.4e-200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(-1.234e-198, "-123.4e-200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(1.23e199, ".123e200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(-1.23e199, "-.123e200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(1.23e-201, ".123e-200");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(-1.23e-201, "-.123e-200");

  // Hexadecimal floating point is also fine.
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(256.0, "0x100");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(0.125, "0x1p-3");
  type_test::CheckSuccessfulParseLiteralValueFromString<DoubleType>(-0.125, "-0x1p-3");

  // Check some parses that we expect to fail.
  const Type &double_type = TypeFactory::GetType(kDouble);
  TypedValue value;
  EXPECT_FALSE(double_type.parseValueFromString("foo", &value));
  EXPECT_FALSE(double_type.parseValueFromString("123l", &value));
  EXPECT_FALSE(double_type.parseValueFromString("12.34f", &value));
  EXPECT_FALSE(double_type.parseValueFromString("1e2f", &value));
  EXPECT_FALSE(double_type.parseValueFromString("0 ", &value));
  EXPECT_FALSE(double_type.parseValueFromString(" 0 ", &value));
  EXPECT_FALSE(double_type.parseValueFromString("e", &value));
  EXPECT_FALSE(double_type.parseValueFromString(".", &value));
  EXPECT_FALSE(double_type.parseValueFromString(".e", &value));
  EXPECT_FALSE(double_type.parseValueFromString("e1", &value));
}

TEST(DoubleTypeTest, IsCoercibleFromTest) {
  type_test::CheckIsCoercibleFrom(DoubleType::InstanceNonNullable(),
                                  {kInt, kLong, kFloat, kDouble});
}

TEST(DoubleTypeTest, IsSafelyCoercibleFromTest) {
  type_test::CheckIsSafelyCoercibleFrom(DoubleType::InstanceNonNullable(),
                                        {kInt, kLong, kFloat, kDouble});
}

TEST(DoubleTypeTest, CoerceValueTest) {
  type_test::CheckNumericCoerce<DoubleType>();
}

}  // namespace quickstep
