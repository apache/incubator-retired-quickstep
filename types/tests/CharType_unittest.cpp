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

#include <cstddef>
#include <cstring>
#include <string>

#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/tests/TypeTest_common.hpp"
#include "utility/MemStream.hpp"

#include "gtest/gtest.h"

using std::strlen;
using std::strncmp;

namespace quickstep {

namespace {

static const char kSampleStringShort[] = "foo";

static const char kSampleStringLong[] =
    "In theory there is no difference between theory and practice. In "
    "practice there is.";

static const std::size_t kPaddedLength = 200;

}  // namespace

TEST(CharTypeTest, PrintValueTest) {
  const Type &short_char_type = TypeFactory::GetType(
      kChar,
      strlen(kSampleStringShort));
  const TypedValue short_value = short_char_type.makeValue(
      kSampleStringShort,
      strlen(kSampleStringShort));
  EXPECT_STREQ(kSampleStringShort, short_char_type.printValueToString(short_value).c_str());

  const Type &short_plusone_char_type = TypeFactory::GetType(
      kChar,
      strlen(kSampleStringShort) + 1);
  const TypedValue short_value_nullterm = short_plusone_char_type.makeValue(
      kSampleStringShort,
      strlen(kSampleStringShort) + 1);
  EXPECT_STREQ(kSampleStringShort, short_plusone_char_type.printValueToString(short_value_nullterm).c_str());

  const Type &long_char_type = TypeFactory::GetType(
      kChar,
      strlen(kSampleStringLong));
  const TypedValue long_value = long_char_type.makeValue(
      kSampleStringLong,
      strlen(kSampleStringLong));
  EXPECT_STREQ(kSampleStringLong, long_char_type.printValueToString(long_value).c_str());

  const Type &long_plusone_char_type = TypeFactory::GetType(
      kChar,
      strlen(kSampleStringLong) + 1);
  const TypedValue long_value_nullterm = long_plusone_char_type.makeValue(
      kSampleStringLong,
      strlen(kSampleStringLong) + 1);
  EXPECT_STREQ(kSampleStringLong,
               long_plusone_char_type.printValueToString(long_value_nullterm).c_str());
}

TEST(CharTypeTest, PrintValueToFileTest) {
  // Test writing to FILE* with a MemStream.
  MemStream memstream;

  // First, check printing without any padding.
  const Type &short_char_type = TypeFactory::GetType(
      kChar,
      strlen(kSampleStringShort));
  const TypedValue short_value = short_char_type.makeValue(
      kSampleStringShort,
      strlen(kSampleStringShort));
  short_char_type.printValueToFile(short_value, memstream.file());
  EXPECT_STREQ(kSampleStringShort, memstream.str());
  memstream.reset();

  const Type &short_plusone_char_type = TypeFactory::GetType(
      kChar,
      strlen(kSampleStringShort) + 1);
  const TypedValue short_value_nullterm = short_plusone_char_type.makeValue(
      kSampleStringShort,
      strlen(kSampleStringShort) + 1);
  short_plusone_char_type.printValueToFile(short_value_nullterm, memstream.file());
  EXPECT_STREQ(kSampleStringShort, memstream.str());
  memstream.reset();

  const Type &long_char_type = TypeFactory::GetType(
      kChar,
      strlen(kSampleStringLong));
  const TypedValue long_value = long_char_type.makeValue(
      kSampleStringLong,
      strlen(kSampleStringLong));
  long_char_type.printValueToFile(long_value, memstream.file());
  EXPECT_STREQ(kSampleStringLong, memstream.str());
  memstream.reset();

  const Type &long_plusone_char_type = TypeFactory::GetType(
      kChar,
      strlen(kSampleStringLong) + 1);
  const TypedValue long_value_nullterm = long_plusone_char_type.makeValue(
      kSampleStringLong,
      strlen(kSampleStringLong) + 1);
  long_plusone_char_type.printValueToFile(long_value_nullterm, memstream.file());
  EXPECT_STREQ(kSampleStringLong, memstream.str());
  memstream.reset();

  // Specifying padding less than the length of the string should have no
  // effect.
  short_char_type.printValueToFile(short_value, memstream.file(), 1);
  EXPECT_STREQ(kSampleStringShort, memstream.str());
  memstream.reset();

  short_plusone_char_type.printValueToFile(short_value_nullterm, memstream.file(), 1);
  EXPECT_STREQ(kSampleStringShort, memstream.str());
  memstream.reset();

  long_char_type.printValueToFile(long_value, memstream.file(), 1);
  EXPECT_STREQ(kSampleStringLong, memstream.str());
  memstream.reset();

  long_plusone_char_type.printValueToFile(long_value_nullterm, memstream.file(), 1);
  EXPECT_STREQ(kSampleStringLong, memstream.str());
  memstream.reset();

  // Test padding up to a specified length.
  std::string padded_short(kPaddedLength - strlen(kSampleStringShort), ' ');
  padded_short.append(kSampleStringShort);

  std::string padded_long(kPaddedLength - strlen(kSampleStringLong), ' ');
  padded_long.append(kSampleStringLong);

  short_char_type.printValueToFile(short_value, memstream.file(), kPaddedLength);
  EXPECT_STREQ(padded_short.c_str(), memstream.str());
  memstream.reset();

  short_plusone_char_type.printValueToFile(short_value_nullterm, memstream.file(), kPaddedLength);
  EXPECT_STREQ(padded_short.c_str(), memstream.str());
  memstream.reset();

  long_char_type.printValueToFile(long_value, memstream.file(), kPaddedLength);
  EXPECT_STREQ(padded_long.c_str(), memstream.str());
  memstream.reset();

  long_plusone_char_type.printValueToFile(long_value_nullterm, memstream.file(), kPaddedLength);
  EXPECT_STREQ(padded_long.c_str(), memstream.str());
  memstream.reset();
}

namespace {

void CheckParseForString(const std::string &sample_string) {
  const Type &char_type
      = TypeFactory::GetType(kChar, sample_string.length(), false);
  const Type &shorter_char_type
      = TypeFactory::GetType(kChar, sample_string.length() - 1, false);
  const Type &longer_char_type
      = TypeFactory::GetType(kChar, sample_string.length() + 10, false);

  TypedValue parsed_value;

  EXPECT_FALSE(shorter_char_type.parseValueFromString(sample_string, &parsed_value));

  // No null-terminator if CHAR type is exactly big enough for the string.
  ASSERT_TRUE(char_type.parseValueFromString(sample_string, &parsed_value));
  EXPECT_EQ(sample_string.length(), parsed_value.getDataSize());
  EXPECT_EQ(0, strncmp(sample_string.c_str(),
                       static_cast<const char*>(parsed_value.getDataPtr()),
                       sample_string.length()));

  ASSERT_TRUE(longer_char_type.parseValueFromString(sample_string, &parsed_value));
  EXPECT_EQ(sample_string.length() + 1, parsed_value.getDataSize());
  EXPECT_STREQ(sample_string.c_str(), static_cast<const char*>(parsed_value.getDataPtr()));
}

}  // namespace

TEST(CharTypeTest, ParseValueFromStringTest) {
  CheckParseForString(kSampleStringShort);
  CheckParseForString(kSampleStringLong);
}

TEST(CharTypeTest, IsCoercibleFromTest) {
  // Check CHAR types of different lengths.
  type_test::CheckStringTypeIsCoercibleFrom(kChar, 1);
  type_test::CheckStringTypeIsCoercibleFrom(kChar, 10);
  type_test::CheckStringTypeIsCoercibleFrom(kChar, 100);
  type_test::CheckStringTypeIsCoercibleFrom(kChar, 1000);
}

TEST(CharTypeTest, IsSafelyCoercibleFromTest) {
  // Check CHAR types of different lengths.
  type_test::CheckStringTypeIsSafelyCoercibleFrom(kChar, 1);
  type_test::CheckStringTypeIsSafelyCoercibleFrom(kChar, 10);
  type_test::CheckStringTypeIsSafelyCoercibleFrom(kChar, 100);
  type_test::CheckStringTypeIsSafelyCoercibleFrom(kChar, 1000);
}

TEST(CharTypeTest, CoerceValueTest) {
  type_test::CheckStringCoerceFromCharWithoutNullTerminator(kChar, kSampleStringShort);
  type_test::CheckStringCoerceWithNullTerminator(kChar, kChar, kSampleStringShort);
  type_test::CheckStringCoerceWithNullTerminator(kChar, kVarChar, kSampleStringShort);

  type_test::CheckStringCoerceFromCharWithoutNullTerminator(kChar, kSampleStringLong);
  type_test::CheckStringCoerceWithNullTerminator(kChar, kChar, kSampleStringLong);
  type_test::CheckStringCoerceWithNullTerminator(kChar, kVarChar, kSampleStringLong);

  // Also check coercibility of NULL values.
  const Type &char_type = TypeFactory::GetType(kChar, 10, true);
  TypedValue char_null = char_type.makeNullValue();

  const Type &varchar_type = TypeFactory::GetType(kVarChar, 10, true);
  TypedValue varchar_null = varchar_type.makeNullValue();

  const Type &null_type = TypeFactory::GetType(kNullType, true);
  TypedValue nulltype_null = null_type.makeNullValue();

  TypedValue coerced_null = char_type.coerceValue(char_null, char_type);
  EXPECT_EQ(kChar, coerced_null.getTypeID());
  EXPECT_TRUE(coerced_null.isNull());

  coerced_null = char_type.coerceValue(varchar_null, varchar_type);
  EXPECT_EQ(kChar, coerced_null.getTypeID());
  EXPECT_TRUE(coerced_null.isNull());

  coerced_null = char_type.coerceValue(nulltype_null, null_type);
  EXPECT_EQ(kChar, coerced_null.getTypeID());
  EXPECT_TRUE(coerced_null.isNull());
}

}  // namespace quickstep
