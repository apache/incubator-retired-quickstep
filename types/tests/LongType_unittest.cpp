/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
  CheckPrintValue(0l);
  CheckPrintValue(123l);
  CheckPrintValue(-123l);
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
  CheckPrintValueToFile(0l);
  CheckPrintValueToFile(123l);
  CheckPrintValueToFile(-123l);
  CheckPrintValueToFile(std::numeric_limits<std::int64_t>::max());
  CheckPrintValueToFile(std::numeric_limits<std::int64_t>::min());
}

TEST(LongTypeTest, ParseValueFromStringTest) {
  type_test::CheckSuccessfulParseLiteralValueFromString<LongType>(0l, "0");
  type_test::CheckSuccessfulParseLiteralValueFromString<LongType>(0l, "000");
  type_test::CheckSuccessfulParseLiteralValueFromString<LongType>(123l, "123");
  type_test::CheckSuccessfulParseLiteralValueFromString<LongType>(123l, "0123");
  type_test::CheckSuccessfulParseLiteralValueFromString<LongType>(123l, "+123");
  type_test::CheckSuccessfulParseLiteralValueFromString<LongType>(-123l, "-123");
  type_test::CheckSuccessfulParseLiteralValueFromString<LongType>(-123l, "-0123");
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
