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
#include <cstdint>
#include <limits>
#include <string>

#include "types/LongType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/tests/TypeTest_common.hpp"
#include "utility/MemStream.hpp"

#include "gtest/gtest.h"

namespace quickstep {

namespace {

void CheckPrintValue(const std::int64_t literal_value) {
  const Type &long_type = TypeFactory::GetType(kLong);
  const TypedValue long_value = long_type.makeValue(&literal_value);
  EXPECT_EQ(std::to_string(literal_value), long_type.printValueToString(long_value));
}

}  // namespace

TEST(LongTypeTest, PrintValueTest) {
  CheckPrintValue(INT64_C(0));
  CheckPrintValue(INT64_C(123));
  CheckPrintValue(INT64_C(-123));
  CheckPrintValue(std::numeric_limits<std::int64_t>::max());
  CheckPrintValue(std::numeric_limits<std::int64_t>::min());
}

namespace {

static const std::size_t kPaddedLength = 200;

void CheckPrintValueToFile(const std::int64_t literal_value) {
  std::string value_as_string = std::to_string(literal_value);

  // Test writing to FILE* with a MemStream.
  MemStream memstream;

  const Type &long_type = TypeFactory::GetType(kLong);
  const TypedValue long_value = long_type.makeValue(&literal_value);

  // First, check printing without any padding
  long_type.printValueToFile(long_value, memstream.file());
  EXPECT_STREQ(value_as_string.c_str(), memstream.str());
  memstream.reset();

  // Specifying padding less than the length needed to print the value should
  // have no effect.
  long_type.printValueToFile(long_value, memstream.file(), 1);
  EXPECT_STREQ(value_as_string.c_str(), memstream.str());
  memstream.reset();

  // Test padding up to a specified length.
  std::string padded(kPaddedLength - value_as_string.size(), ' ');
  padded.append(value_as_string);
  long_type.printValueToFile(long_value, memstream.file(), kPaddedLength);
  EXPECT_STREQ(padded.c_str(), memstream.str());
  memstream.reset();
}

}  // namespace

TEST(LongTypeTest, PrintValueToFileTest) {
  CheckPrintValueToFile(INT64_C(0));
  CheckPrintValueToFile(INT64_C(123));
  CheckPrintValueToFile(INT64_C(-123));
  CheckPrintValueToFile(std::numeric_limits<std::int64_t>::max());
  CheckPrintValueToFile(std::numeric_limits<std::int64_t>::min());
}

TEST(LongTypeTest, ParseValueFromStringTest) {
  type_test::CheckSuccessfulParseLiteralValueFromString<LongType>(INT64_C(0), "0");
  type_test::CheckSuccessfulParseLiteralValueFromString<LongType>(INT64_C(0), "000");
  type_test::CheckSuccessfulParseLiteralValueFromString<LongType>(INT64_C(123), "123");
  type_test::CheckSuccessfulParseLiteralValueFromString<LongType>(INT64_C(123), "0123");
  type_test::CheckSuccessfulParseLiteralValueFromString<LongType>(INT64_C(123), "+123");
  type_test::CheckSuccessfulParseLiteralValueFromString<LongType>(INT64_C(-123), "-123");
  type_test::CheckSuccessfulParseLiteralValueFromString<LongType>(INT64_C(-123), "-0123");
  type_test::CheckSuccessfulParseLiteralValueFromString<LongType>(
      std::numeric_limits<std::int64_t>::max(),
      std::to_string(std::numeric_limits<std::int64_t>::max()));
  type_test::CheckSuccessfulParseLiteralValueFromString<LongType>(
      std::numeric_limits<std::int64_t>::min(),
      std::to_string(std::numeric_limits<std::int64_t>::min()));

  // Check some parses that we expect to fail.
  const Type &long_type = TypeFactory::GetType(kLong);
  TypedValue value;
  EXPECT_FALSE(long_type.parseValueFromString("foo", &value));
  EXPECT_FALSE(long_type.parseValueFromString("123l", &value));
  EXPECT_FALSE(long_type.parseValueFromString("12.34", &value));
  EXPECT_FALSE(long_type.parseValueFromString("1e2", &value));
  EXPECT_FALSE(long_type.parseValueFromString("0 ", &value));
  EXPECT_FALSE(long_type.parseValueFromString(" 0 ", &value));
  EXPECT_FALSE(long_type.parseValueFromString("0x100", &value));
}

TEST(LongTypeTest, IsCoercibleFromTest) {
  type_test::CheckIsCoercibleFrom(LongType::InstanceNonNullable(),
                                  {kInt, kLong, kFloat, kDouble});
}

TEST(LongTypeTest, IsSafelyCoercibleFromTest) {
  type_test::CheckIsSafelyCoercibleFrom(LongType::InstanceNonNullable(),
                                        {kInt, kLong});
}

TEST(LongTypeTest, CoerceValueTest) {
  type_test::CheckNumericCoerce<LongType>();
}

}  // namespace quickstep
