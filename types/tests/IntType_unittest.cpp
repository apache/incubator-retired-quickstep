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
#include <limits>
#include <string>

#include "types/IntType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/tests/TypeTest_common.hpp"
#include "utility/MemStream.hpp"

#include "gtest/gtest.h"

namespace quickstep {

namespace {

void CheckPrintValue(const int literal_value) {
  const Type &int_type = TypeFactory::GetType(kInt);
  const TypedValue int_value = int_type.makeValue(&literal_value);
  EXPECT_EQ(std::to_string(literal_value), int_type.printValueToString(int_value));
}

}  // namespace

TEST(IntTypeTest, PrintValueTest) {
  CheckPrintValue(0);
  CheckPrintValue(123);
  CheckPrintValue(-123);
  CheckPrintValue(std::numeric_limits<int>::max());
  CheckPrintValue(std::numeric_limits<int>::min());
}

namespace {

static const std::size_t kPaddedLength = 200;

void CheckPrintValueToFile(const int literal_value) {
  std::string value_as_string = std::to_string(literal_value);

  // Test writing to FILE* with a MemStream.
  MemStream memstream;

  const Type &int_type = TypeFactory::GetType(kInt);
  const TypedValue int_value = int_type.makeValue(&literal_value);

  // First, check printing without any padding
  int_type.printValueToFile(int_value, memstream.file());
  EXPECT_STREQ(value_as_string.c_str(), memstream.str());
  memstream.reset();

  // Specifying padding less than the length needed to print the value should
  // have no effect.
  int_type.printValueToFile(int_value, memstream.file(), 1);
  EXPECT_STREQ(value_as_string.c_str(), memstream.str());
  memstream.reset();

  // Test padding up to a specified length.
  std::string padded(kPaddedLength - value_as_string.size(), ' ');
  padded.append(value_as_string);
  int_type.printValueToFile(int_value, memstream.file(), kPaddedLength);
  EXPECT_STREQ(padded.c_str(), memstream.str());
  memstream.reset();
}

}  // namespace

TEST(IntTypeTest, PrintValueToFileTest) {
  CheckPrintValueToFile(0);
  CheckPrintValueToFile(123);
  CheckPrintValueToFile(-123);
  CheckPrintValueToFile(std::numeric_limits<int>::max());
  CheckPrintValueToFile(std::numeric_limits<int>::min());
}

TEST(IntTypeTest, ParseValueFromStringTest) {
  type_test::CheckSuccessfulParseLiteralValueFromString<IntType>(0, "0");
  type_test::CheckSuccessfulParseLiteralValueFromString<IntType>(0, "000");
  type_test::CheckSuccessfulParseLiteralValueFromString<IntType>(123, "123");
  type_test::CheckSuccessfulParseLiteralValueFromString<IntType>(123, "0123");
  type_test::CheckSuccessfulParseLiteralValueFromString<IntType>(123, "+123");
  type_test::CheckSuccessfulParseLiteralValueFromString<IntType>(-123, "-123");
  type_test::CheckSuccessfulParseLiteralValueFromString<IntType>(-123, "-0123");
  type_test::CheckSuccessfulParseLiteralValueFromString<IntType>(
      std::numeric_limits<int>::max(),
      std::to_string(std::numeric_limits<int>::max()));
  type_test::CheckSuccessfulParseLiteralValueFromString<IntType>(
      std::numeric_limits<int>::min(),
      std::to_string(std::numeric_limits<int>::min()));

  // Check some parses that we expect to fail.
  const Type &int_type = TypeFactory::GetType(kInt);
  TypedValue value;
  EXPECT_FALSE(int_type.parseValueFromString("foo", &value));
  EXPECT_FALSE(int_type.parseValueFromString("12.34", &value));
  EXPECT_FALSE(int_type.parseValueFromString("1e2", &value));
  EXPECT_FALSE(int_type.parseValueFromString("0 ", &value));
  EXPECT_FALSE(int_type.parseValueFromString(" 0 ", &value));
  EXPECT_FALSE(int_type.parseValueFromString("0x100", &value));
}

TEST(IntTypeTest, IsCoercibleFromTest) {
  type_test::CheckIsCoercibleFrom(IntType::InstanceNonNullable(),
                                  {kInt, kLong, kFloat, kDouble});
}

TEST(IntTypeTest, IsSafelyCoercibleFromTest) {
  type_test::CheckIsSafelyCoercibleFrom(IntType::InstanceNonNullable(),
                                        {kInt});
}

TEST(IntTypeTest, CoerceValueTest) {
  type_test::CheckNumericCoerce<IntType>();
}

}  // namespace quickstep
