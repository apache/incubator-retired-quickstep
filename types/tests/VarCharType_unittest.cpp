/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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

namespace quickstep {

namespace {

static const char kSampleStringShort[] = "foo";

static const char kSampleStringLong[] =
    "You should always go to other people's funerals; otherwise they won't "
    "come to yours.";

static const std::size_t kPaddedLength = 200;

}  // namespace

TEST(VarCharTypeTest, PrintValueTest) {
  const Type &short_varchar_type = TypeFactory::GetType(
      kVarChar,
      strlen(kSampleStringShort));
  const TypedValue short_value = short_varchar_type.makeValue(
      kSampleStringShort,
      strlen(kSampleStringShort) + 1);
  EXPECT_STREQ(kSampleStringShort, short_varchar_type.printValueToString(short_value).c_str());

  const Type &long_varchar_type = TypeFactory::GetType(
      kVarChar,
      strlen(kSampleStringLong));
  const TypedValue long_value = long_varchar_type.makeValue(
      kSampleStringLong,
      strlen(kSampleStringLong) + 1);
  EXPECT_STREQ(kSampleStringLong, long_varchar_type.printValueToString(long_value).c_str());
}

TEST(VarCharTypeTest, PrintValueToFileTest) {
  // Test writing to FILE* with a MemStream.
  MemStream memstream;

  // First, check printing without any padding.
  const Type &short_varchar_type = TypeFactory::GetType(
      kVarChar,
      strlen(kSampleStringShort));
  const TypedValue short_value = short_varchar_type.makeValue(
      kSampleStringShort,
      strlen(kSampleStringShort) + 1);
  short_varchar_type.printValueToFile(short_value, memstream.file());
  EXPECT_STREQ(kSampleStringShort, memstream.str());
  memstream.reset();

  const Type &long_varchar_type = TypeFactory::GetType(
      kVarChar,
      strlen(kSampleStringLong));
  const TypedValue long_value = long_varchar_type.makeValue(
      kSampleStringLong,
      strlen(kSampleStringLong) + 1);
  long_varchar_type.printValueToFile(long_value, memstream.file());
  EXPECT_STREQ(kSampleStringLong, memstream.str());
  memstream.reset();

  // Specifying padding less than the length of the string should have no
  // effect.
  short_varchar_type.printValueToFile(short_value, memstream.file(), 1);
  EXPECT_STREQ(kSampleStringShort, memstream.str());
  memstream.reset();

  long_varchar_type.printValueToFile(long_value, memstream.file(), 1);
  EXPECT_STREQ(kSampleStringLong, memstream.str());
  memstream.reset();

  // Test padding up to a specified length.
  std::string padded_short(kPaddedLength - strlen(kSampleStringShort), ' ');
  padded_short.append(kSampleStringShort);

  std::string padded_long(kPaddedLength - strlen(kSampleStringLong), ' ');
  padded_long.append(kSampleStringLong);

  short_varchar_type.printValueToFile(short_value, memstream.file(), kPaddedLength);
  EXPECT_STREQ(padded_short.c_str(), memstream.str());
  memstream.reset();

  long_varchar_type.printValueToFile(long_value, memstream.file(), kPaddedLength);
  EXPECT_STREQ(padded_long.c_str(), memstream.str());
  memstream.reset();
}

namespace {

void CheckParseForString(const std::string &sample_string) {
  const Type &varchar_type
      = TypeFactory::GetType(kVarChar, sample_string.length(), false);
  const Type &shorter_varchar_type
      = TypeFactory::GetType(kVarChar, sample_string.length() - 1, false);
  const Type &longer_varchar_type
      = TypeFactory::GetType(kVarChar, sample_string.length() + 10, false);

  TypedValue parsed_value;

  EXPECT_FALSE(shorter_varchar_type.parseValueFromString(sample_string, &parsed_value));

  ASSERT_TRUE(varchar_type.parseValueFromString(sample_string, &parsed_value));
  EXPECT_EQ(sample_string.length() + 1, parsed_value.getDataSize());
  EXPECT_STREQ(sample_string.c_str(), static_cast<const char*>(parsed_value.getDataPtr()));

  ASSERT_TRUE(longer_varchar_type.parseValueFromString(sample_string, &parsed_value));
  EXPECT_EQ(sample_string.length() + 1, parsed_value.getDataSize());
  EXPECT_STREQ(sample_string.c_str(), static_cast<const char*>(parsed_value.getDataPtr()));
}

}  // namespace

TEST(VarCharTypeTest, ParseValueFromStringTest) {
  CheckParseForString(kSampleStringShort);
  CheckParseForString(kSampleStringLong);
}

TEST(VarCharTypeTest, IsCoercibleFromTest) {
  // Check VARCHAR types of different lengths.
  type_test::CheckStringTypeIsCoercibleFrom(kVarChar, 1);
  type_test::CheckStringTypeIsCoercibleFrom(kVarChar, 10);
  type_test::CheckStringTypeIsCoercibleFrom(kVarChar, 100);
  type_test::CheckStringTypeIsCoercibleFrom(kVarChar, 1000);
}

TEST(VarCharTypeTest, IsSafelyCoercibleFromTest) {
  // Check VARCHAR types of different lengths.
  type_test::CheckStringTypeIsSafelyCoercibleFrom(kVarChar, 1);
  type_test::CheckStringTypeIsSafelyCoercibleFrom(kVarChar, 10);
  type_test::CheckStringTypeIsSafelyCoercibleFrom(kVarChar, 100);
  type_test::CheckStringTypeIsSafelyCoercibleFrom(kVarChar, 1000);
}

TEST(VarCharTypeTest, CoerceValueTest) {
  type_test::CheckStringCoerceFromCharWithoutNullTerminator(kVarChar, kSampleStringShort);
  type_test::CheckStringCoerceWithNullTerminator(kVarChar, kChar, kSampleStringShort);
  type_test::CheckStringCoerceWithNullTerminator(kVarChar, kVarChar, kSampleStringShort);

  type_test::CheckStringCoerceFromCharWithoutNullTerminator(kVarChar, kSampleStringLong);
  type_test::CheckStringCoerceWithNullTerminator(kVarChar, kChar, kSampleStringLong);
  type_test::CheckStringCoerceWithNullTerminator(kVarChar, kVarChar, kSampleStringLong);

  // Also check coercibility of NULL values.
  const Type &char_type = TypeFactory::GetType(kChar, 10, true);
  TypedValue char_null = char_type.makeNullValue();

  const Type &varchar_type = TypeFactory::GetType(kVarChar, 10, true);
  TypedValue varchar_null = varchar_type.makeNullValue();

  const Type &null_type = TypeFactory::GetType(kNullType, true);
  TypedValue nulltype_null = null_type.makeNullValue();

  TypedValue coerced_null = varchar_type.coerceValue(char_null, char_type);
  EXPECT_EQ(kVarChar, coerced_null.getTypeID());
  EXPECT_TRUE(coerced_null.isNull());

  coerced_null = varchar_type.coerceValue(varchar_null, varchar_type);
  EXPECT_EQ(kVarChar, coerced_null.getTypeID());
  EXPECT_TRUE(coerced_null.isNull());

  coerced_null = varchar_type.coerceValue(nulltype_null, null_type);
  EXPECT_EQ(kVarChar, coerced_null.getTypeID());
  EXPECT_TRUE(coerced_null.isNull());
}

}  // namespace quickstep
