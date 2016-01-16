/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include <cstddef>
#include <string>

#include "types/DatetimeLit.hpp"
#include "types/DatetimeType.hpp"
#include "types/NullType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/tests/TypeTest_common.hpp"
#include "utility/MemStream.hpp"

#include "gtest/gtest.h"

namespace quickstep {

TEST(DatetimeTypeTest, PrintValueTest) {
  const Type &datetime_type = TypeFactory::GetType(kDatetime);

  // Skynet becomes self-aware at 2:14 AM on August 29, 1997.
  DatetimeLit judgement_day;
  judgement_day.ticks = 872820840LL * DatetimeLit::kTicksPerSecond;
  TypedValue judgement_day_value(judgement_day);
  EXPECT_EQ(std::string("1997-08-29T02:14:00"),
            datetime_type.printValueToString(judgement_day_value));

  // Check with fractional seconds.
  judgement_day.ticks += 123;
  judgement_day_value = TypedValue(judgement_day);
  EXPECT_EQ(std::string("1997-08-29T02:14:00.000123"),
            datetime_type.printValueToString(judgement_day_value));

  // Check extended formatting for years far in the past.
  DatetimeLit dawn_age;
  dawn_age.ticks = -630720000000000000LL;
  TypedValue dawn_age_value(dawn_age);
  EXPECT_EQ(std::string("-18017-04-13T00:00:00"),
            datetime_type.printValueToString(dawn_age_value));
}

namespace {

static const std::size_t kPaddedLength = 200;

void CheckPrintValueToFile(const DatetimeLit literal_value,
                           const std::string &expected_string) {
  // Test writing to FILE* with a MemStream.
  MemStream memstream;

  const Type &datetime_type = TypeFactory::GetType(kDatetime);
  const TypedValue datetime_value = datetime_type.makeValue(&literal_value);

  // First, check printing without any padding
  datetime_type.printValueToFile(datetime_value, memstream.file());
  EXPECT_STREQ(expected_string.c_str(), memstream.str());
  memstream.reset();

  // Specifying padding less than the length needed to print the value should
  // have no effect.
  datetime_type.printValueToFile(datetime_value, memstream.file(), 1);
  EXPECT_STREQ(expected_string.c_str(), memstream.str());
  memstream.reset();

  // Test padding up to a specified length.
  std::string padded(kPaddedLength - expected_string.size(), ' ');
  padded.append(expected_string);
  datetime_type.printValueToFile(datetime_value, memstream.file(), kPaddedLength);
  EXPECT_STREQ(padded.c_str(), memstream.str());
  memstream.reset();
}

}  // namespace

TEST(DatetimeTypeTest, PrintValueToFileTest) {
  DatetimeLit judgement_day;
  judgement_day.ticks = 872820840LL * DatetimeLit::kTicksPerSecond;
  CheckPrintValueToFile(judgement_day, "1997-08-29T02:14:00");

  judgement_day.ticks += 123;
  CheckPrintValueToFile(judgement_day, "1997-08-29T02:14:00.000123");

  DatetimeLit dawn_age;
  dawn_age.ticks = -630720000000000000LL;
  CheckPrintValueToFile(dawn_age, "-18017-04-13T00:00:00");
}

TEST(DatetimeTypeTest, ParseValueFromStringTest) {
  // Parse the same value in a bunch of different valid format variations.
  type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeType>(
      DatetimeLit{872820840LL * DatetimeLit::kTicksPerSecond}, "1997-08-29T02:14:00");
  type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeType>(
      DatetimeLit{872820840LL * DatetimeLit::kTicksPerSecond}, "1997-08-29t02:14:00");
  type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeType>(
      DatetimeLit{872820840LL * DatetimeLit::kTicksPerSecond}, "1997-08-29 02:14:00");
  type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeType>(
      DatetimeLit{872820840LL * DatetimeLit::kTicksPerSecond}, "+1997-08-29T02:14:00");
  type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeType>(
      DatetimeLit{872820840LL * DatetimeLit::kTicksPerSecond}, "+001997-08-29T02:14:00");
  type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeType>(
      DatetimeLit{872820840LL * DatetimeLit::kTicksPerSecond}, "1997-08-29T02:14:00.0");
  type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeType>(
      DatetimeLit{872820840LL * DatetimeLit::kTicksPerSecond}, "1997-08-29T02:14:00.000");
  type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeType>(
      DatetimeLit{872820840LL * DatetimeLit::kTicksPerSecond}, "1997-08-29T02:14:00.000000");

  // Parse with fractional subseconds.
  type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeType>(
      DatetimeLit{872820840LL * DatetimeLit::kTicksPerSecond + 123}, "1997-08-29T02:14:00.000123");
  type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeType>(
      DatetimeLit{872820840LL * DatetimeLit::kTicksPerSecond + 1230}, "1997-08-29T02:14:00.00123");
  type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeType>(
      DatetimeLit{872820840LL * DatetimeLit::kTicksPerSecond + 123000}, "1997-08-29T02:14:00.123");

  // Parse a date without a time.
  type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeType>(
      DatetimeLit{-3117571200LL * DatetimeLit::kTicksPerSecond}, "1871-03-18");
  type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeType>(
      DatetimeLit{-3117571200LL * DatetimeLit::kTicksPerSecond}, "+1871-03-18");
  type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeType>(
      DatetimeLit{-3117571200LL * DatetimeLit::kTicksPerSecond}, "+001871-03-18");

  // Parse extended format with negative year.
  type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeType>(
      DatetimeLit{-630720000000000000LL}, "-18017-04-13T00:00:00");

  // Test some parses that we expect to fail.
  const Type &datetime_type = TypeFactory::GetType(kDatetime);
  TypedValue value;
  EXPECT_FALSE(datetime_type.parseValueFromString("foo", &value));
  EXPECT_FALSE(datetime_type.parseValueFromString("42", &value));
  EXPECT_FALSE(datetime_type.parseValueFromString("1997-08-29T02:14:00 ", &value));
  EXPECT_FALSE(datetime_type.parseValueFromString(" 1997-08-29T02:14:00 ", &value));
  EXPECT_FALSE(datetime_type.parseValueFromString("1997-08-29T02:14:00.", &value));
  EXPECT_FALSE(datetime_type.parseValueFromString("1997-08-29T02:14:00q", &value));
  EXPECT_FALSE(datetime_type.parseValueFromString("1997-08-29T02:14:00.q", &value));
  EXPECT_FALSE(datetime_type.parseValueFromString("1997-08-29-02:14:00", &value));
  EXPECT_FALSE(datetime_type.parseValueFromString("1e3-08-29T02:14:00", &value));
  EXPECT_FALSE(datetime_type.parseValueFromString("1997-08-29T02:14:00.0000001", &value));
  EXPECT_FALSE(datetime_type.parseValueFromString("--18017-04-13T00:00:00", &value));
  EXPECT_FALSE(datetime_type.parseValueFromString("1997-08--29 ", &value));
  EXPECT_FALSE(datetime_type.parseValueFromString("1997-08--2 ", &value));
  EXPECT_FALSE(datetime_type.parseValueFromString("1997-08-29T-01:00:00", &value));
  EXPECT_FALSE(datetime_type.parseValueFromString("1997-08-29T-1:00:00", &value));
}

TEST(DatetimeTypeTest, IsCoercibleFromTest) {
  type_test::CheckIsCoercibleFrom(DatetimeType::InstanceNonNullable(),
                                  {kDatetime});
}

TEST(DatetimeTypeTest, IsSafelyCoercibleFromTest) {
  type_test::CheckIsSafelyCoercibleFrom(DatetimeType::InstanceNonNullable(),
                                        {kDatetime});
}

TEST(DatetimeTypeTest, CoerceValueTest) {
  // The epoch.
  type_test::CheckLiteralCoerce<DatetimeType, DatetimeType>(DatetimeLit{0});

  // 123 ticks (microseconds) after the epoch.
  type_test::CheckLiteralCoerce<DatetimeType, DatetimeType>(DatetimeLit{123});

  // 123 ticks (microseconds) before the epoch.
  type_test::CheckLiteralCoerce<DatetimeType, DatetimeType>(DatetimeLit{-123});

  type_test::CheckNullLiteralCoerce<DatetimeType, DatetimeType>();
  type_test::CheckNullLiteralCoerce<DatetimeType, NullType>();
}

}  // namespace quickstep
