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

#include "types/FloatType.hpp"
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
std::string FloatAsString(const float literal_value) {
  char print_buffer[100];
  int written = snprintf(print_buffer,
                         sizeof(print_buffer),
                         "%.*g",
                         std::numeric_limits<float>::max_digits10,
                         literal_value);
  CHECK_GE(written, 0);
  CHECK_LT(static_cast<std::size_t>(written), sizeof(print_buffer));
  return std::string(print_buffer);
}

void CheckPrintValue(const float literal_value) {
  const Type &float_type = TypeFactory::GetType(kFloat);
  const TypedValue float_value = float_type.makeValue(&literal_value);
  EXPECT_EQ(FloatAsString(literal_value), float_type.printValueToString(float_value));
}

}  // namespace

TEST(FloatTypeTest, PrintValueTest) {
  // Floats without a fractional part.
  CheckPrintValue(0.0f);
  CheckPrintValue(123.0f);
  CheckPrintValue(-123.0f);

  // Try with a fractional part.
  CheckPrintValue(123.456f);
  CheckPrintValue(-123.456f);

  // Try with large and small exponents.
  CheckPrintValue(8.2575e20f);
  CheckPrintValue(-8.2575e20f);
  CheckPrintValue(8.2575e-20f);
  CheckPrintValue(-8.2575e-20f);

  // Various limits: largest and smallest normalized positive values, smallest
  // denormalized nonzero value, and largest negative value.
  CheckPrintValue(std::numeric_limits<float>::max());
  CheckPrintValue(std::numeric_limits<float>::min());
  CheckPrintValue(std::numeric_limits<float>::denorm_min());
  CheckPrintValue(std::numeric_limits<float>::lowest());
}

namespace {

static const std::size_t kPaddedLength = 200;

void CheckPrintValueToFile(const float literal_value) {
  std::string value_as_string = FloatAsString(literal_value);

  // Test writing to FILE* with a MemStream.
  MemStream memstream;

  const Type &float_type = TypeFactory::GetType(kFloat);
  const TypedValue float_value = float_type.makeValue(&literal_value);

  // First, check printing without any padding
  float_type.printValueToFile(float_value, memstream.file());
  EXPECT_STREQ(value_as_string.c_str(), memstream.str());
  memstream.reset();

  // Specifying padding less than the length needed to print the value should
  // have no effect.
  float_type.printValueToFile(float_value, memstream.file(), 1);
  EXPECT_STREQ(value_as_string.c_str(), memstream.str());
  memstream.reset();

  // Test padding up to a specified length.
  std::string padded(kPaddedLength - value_as_string.size(), ' ');
  padded.append(value_as_string);
  float_type.printValueToFile(float_value, memstream.file(), kPaddedLength);
  EXPECT_STREQ(padded.c_str(), memstream.str());
  memstream.reset();
}

}  // namespace

TEST(FloatTypeTest, PrintValueToFileTest) {
  // Floats without a fractional part.
  CheckPrintValueToFile(0.0f);
  CheckPrintValueToFile(123.0f);
  CheckPrintValueToFile(-123.0f);

  // Try with a fractional part.
  CheckPrintValueToFile(123.456f);
  CheckPrintValueToFile(-123.456f);

  // Try with large and small exponents.
  CheckPrintValueToFile(8.2575e20f);
  CheckPrintValueToFile(-8.2575e20f);
  CheckPrintValueToFile(8.2575e-20f);
  CheckPrintValueToFile(-8.2575e-20f);

  // Various limits: largest and smallest normalized positive values, smallest
  // denormalized nonzero value, and largest negative value.
  CheckPrintValueToFile(std::numeric_limits<float>::max());
  CheckPrintValueToFile(std::numeric_limits<float>::min());
  CheckPrintValueToFile(std::numeric_limits<float>::denorm_min());
  CheckPrintValueToFile(std::numeric_limits<float>::lowest());
}

TEST(FloatTypeTest, ParseValueFromStringTest) {
  // Floats without a fractional part.
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(0.0f, "0");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(0.0f, "000");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(123.0f, "123");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(123.0f, "0123");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(123.0f, "+123");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(-123.0f, "-123");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(-123.0f, "-0123");

  // With a fractional part.
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(123.456f, "123.456");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(-123.456f, "-123.456");

  // With large and small exponents.
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(8.2575e20f, "8.2575e20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(-8.2575e20f, "-8.2575e20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(8.2575e-20f, "8.2575e-20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(-8.2575e-20f, "-8.2575e-20");

  // With exponent but no decimal.
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(8.0e20f, "8e20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(-8.0e20f, "-8e20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(8.0e-20f, "8e-20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(-8.0e-20f, "-8e-20");

  // Various float limits.
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(
      std::numeric_limits<float>::max(),
      FloatAsString(std::numeric_limits<float>::max()));
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(
      std::numeric_limits<float>::min(),
      FloatAsString(std::numeric_limits<float>::min()));
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(
      std::numeric_limits<float>::denorm_min(),
      FloatAsString(std::numeric_limits<float>::denorm_min()));
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(
      std::numeric_limits<float>::lowest(),
      FloatAsString(std::numeric_limits<float>::lowest()));

  // Slightly oddball but still acceptable formatting.
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(42.0f, "42.");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(-42.0f, "-42.");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(0.42f, ".42");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(-0.42f, "-.42");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(8.0e20f, "8.e20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(-8.0e20f, "-8.e20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(8.0e-20f, "8.e-20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(-8.0e-20f, "-8.e-20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(1.23e22f, "123e20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(-1.23e22f, "-123e20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(1.23e-18f, "123e-20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(-1.23e-18f, "-123e-20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(1.234e22f, "123.4e20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(-1.234e22f, "-123.4e20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(1.234e-18f, "123.4e-20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(-1.234e-18f, "-123.4e-20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(1.23e19f, ".123e20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(-1.23e19f, "-.123e20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(1.23e-21f, ".123e-20");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(-1.23e-21f, "-.123e-20");

  // Hexadecimal floating point is also fine.
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(256.0f, "0x100");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(0.125f, "0x1p-3");
  type_test::CheckSuccessfulParseLiteralValueFromString<FloatType>(-0.125f, "-0x1p-3");

  // Check some parses that we expect to fail.
  const Type &float_type = TypeFactory::GetType(kFloat);
  TypedValue value;
  EXPECT_FALSE(float_type.parseValueFromString("foo", &value));
  EXPECT_FALSE(float_type.parseValueFromString("123l", &value));
  EXPECT_FALSE(float_type.parseValueFromString("12.34f", &value));
  EXPECT_FALSE(float_type.parseValueFromString("1e2f", &value));
  EXPECT_FALSE(float_type.parseValueFromString("0 ", &value));
  EXPECT_FALSE(float_type.parseValueFromString(" 0 ", &value));
  EXPECT_FALSE(float_type.parseValueFromString("e", &value));
  EXPECT_FALSE(float_type.parseValueFromString(".", &value));
  EXPECT_FALSE(float_type.parseValueFromString(".e", &value));
  EXPECT_FALSE(float_type.parseValueFromString("e1", &value));
}

TEST(FloatTypeTest, IsCoercibleFromTest) {
  type_test::CheckIsCoercibleFrom(FloatType::InstanceNonNullable(),
                                  {kInt, kLong, kFloat, kDouble});
}

TEST(FloatTypeTest, IsSafelyCoercibleFromTest) {
  type_test::CheckIsSafelyCoercibleFrom(FloatType::InstanceNonNullable(),
                                        {kInt, kFloat});
}

TEST(FloatTypeTest, CoerceValueTest) {
  type_test::CheckNumericCoerce<FloatType>();
}

}  // namespace quickstep
