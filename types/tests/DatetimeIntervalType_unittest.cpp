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
#include <string>

#include "types/DatetimeIntervalType.hpp"
#include "types/IntervalLit.hpp"
#include "types/NullType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/tests/TypeTest_common.hpp"
#include "utility/MemStream.hpp"

#include "gtest/gtest.h"

namespace quickstep {

namespace {

static const DatetimeIntervalLit kSampleIntervals[] = {
    {35 * DatetimeIntervalLit::kTicksPerSecond},
    {35 * DatetimeIntervalLit::kTicksPerSecond + 244090},
    {39 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 35 * DatetimeIntervalLit::kTicksPerSecond
        + 244090},
    {1 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 39 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 35 * DatetimeIntervalLit::kTicksPerSecond
        + 244090},
    {24 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 39 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 35 * DatetimeIntervalLit::kTicksPerSecond
        + 244090},
    {48 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 39 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 35 * DatetimeIntervalLit::kTicksPerSecond
        + 244090}};

static const char *kSampleStrings[] = {
    "00:00:35",
    "00:00:35.244090",
    "00:39:35.244090",
    "01:39:35.244090",
    "1 day 00:39:35.244090",  // Exactly one Martian Sol.
    "2 days 00:39:35.244090"};

}  // namespace

TEST(DatetimeIntervalTypeTest, PrintValueTest) {
  const Type &datetime_interval_type = TypeFactory::GetType(kDatetimeInterval);
  for (std::size_t sample_idx = 0;
       sample_idx < (sizeof(kSampleIntervals) / sizeof(kSampleIntervals[0]));
       ++sample_idx) {
    EXPECT_EQ(std::string(kSampleStrings[sample_idx]),
              datetime_interval_type.printValueToString(TypedValue(kSampleIntervals[sample_idx])));

    // Also check the negative version of the same interval.
    std::string negative_string("-");
    negative_string.append(kSampleStrings[sample_idx]);
    EXPECT_EQ(negative_string,
              datetime_interval_type.printValueToString(TypedValue(- kSampleIntervals[sample_idx])));
  }
}

namespace {

static const std::size_t kPaddedLength = 200;

void CheckPrintValueToFile(const DatetimeIntervalLit literal_value,
                           const std::string &expected_string) {
  // Test writing to FILE* with a MemStream.
  MemStream memstream;

  const Type &datetime_interval_type = TypeFactory::GetType(kDatetimeInterval);
  const TypedValue datetime_interval_value
      = datetime_interval_type.makeValue(&literal_value);

  // First, check printing without any padding
  datetime_interval_type.printValueToFile(datetime_interval_value, memstream.file());
  EXPECT_STREQ(expected_string.c_str(), memstream.str());
  memstream.reset();

  // Specifying padding less than the length needed to print the value should
  // have no effect.
  datetime_interval_type.printValueToFile(datetime_interval_value, memstream.file(), 1);
  EXPECT_STREQ(expected_string.c_str(), memstream.str());
  memstream.reset();

  // Test padding up to a specified length.
  std::string padded(kPaddedLength - expected_string.size(), ' ');
  padded.append(expected_string);
  datetime_interval_type.printValueToFile(datetime_interval_value,
                                          memstream.file(),
                                          kPaddedLength);
  EXPECT_STREQ(padded.c_str(), memstream.str());
  memstream.reset();
}

}  // namespace

TEST(DatetimeIntervalTypeTest, PrintValueToFileTest) {
  for (std::size_t sample_idx = 0;
       sample_idx < (sizeof(kSampleIntervals) / sizeof(kSampleIntervals[0]));
       ++sample_idx) {
    CheckPrintValueToFile(kSampleIntervals[sample_idx],
                          kSampleStrings[sample_idx]);

    // Also check the negative version of the same interval.
    std::string negative_string("-");
    negative_string.append(kSampleStrings[sample_idx]);
    CheckPrintValueToFile(- kSampleIntervals[sample_idx],
                          negative_string);
  }
}

namespace {

static const DatetimeIntervalLit kAdditionalParseSampleIntervals[] = {
    {0},
    {1 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 23 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 45 * DatetimeIntervalLit::kTicksPerSecond},
    {1 * 24 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 1 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 23 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 45 * DatetimeIntervalLit::kTicksPerSecond
        + 678900},
    {1 * 24 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 1 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 23 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 45 * DatetimeIntervalLit::kTicksPerSecond
        + 678900},
    {1 * 24 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 1 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 23 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 45 * DatetimeIntervalLit::kTicksPerSecond
        + 678900},
    {1 * 24 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 1 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 23 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 45 * DatetimeIntervalLit::kTicksPerSecond
        + 678900},
    {123456 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 1 * 60 * DatetimeIntervalLit::kTicksPerSecond
        + 1 * DatetimeIntervalLit::kTicksPerSecond},
    {123},
    {123},
    {123},
    {123 * 1000},
    {123 * 1000},
    {123 * 1000},
    {123 * DatetimeIntervalLit::kTicksPerSecond},
    {123 * DatetimeIntervalLit::kTicksPerSecond},
    {123 * DatetimeIntervalLit::kTicksPerSecond},
    {123 * 60 * DatetimeIntervalLit::kTicksPerSecond},
    {123 * 60 * DatetimeIntervalLit::kTicksPerSecond},
    {123 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond},
    {123 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond},
    {123 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond},
    {123 * 24 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond},
    {123 * 24 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond},
    {123 * 24 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond},
    {123 * 7 * 24 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond},
    {123 * 7 * 24 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond},
    {123 * 7 * 24 * 60 * 60 * DatetimeIntervalLit::kTicksPerSecond}
};

static const char *kAdditionalParseSampleStrings[] = {
    "0 days 00:00:00",
    "0 days 01:23:45",
    "1 day 01:23:45.6789",
    "1 days 01:23:45.6789",
    "1 DaYs 01:23:45.6789",
    "1 dAy 01:23:45.6789",
    "123456:01:01",
    "123 us",
    "123 microsecond",
    "123 microseconds",
    "123 ms",
    "123 millisecond",
    "123 milliseconds",
    "123 s",
    "123 second",
    "123 seconds",
    "123 minute",
    "123 minutes",
    "123 h",
    "123 hour",
    "123 hours",
    "123 d",
    "123 day",
    "123 days",
    "123 w",
    "123 week",
    "123 weeks"
};

}  // namespace

TEST(DatetimeIntervalTypeTest, ParseValueFromStringTest) {
  // Test various "complex format" strings.
  for (std::size_t sample_idx = 0;
       sample_idx < (sizeof(kSampleIntervals) / sizeof(kSampleIntervals[0]));
       ++sample_idx) {
      type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeIntervalType>(
          kSampleIntervals[sample_idx], kSampleStrings[sample_idx]);

      // Also check the negative version of the same interval.
      std::string negative_string("-");
      negative_string.append(kSampleStrings[sample_idx]);
      type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeIntervalType>(
          - kSampleIntervals[sample_idx], negative_string);
  }

  // Test additional strings that are still valid for parsing but aren't part
  // of the narrower canonical print format.
  for (std::size_t sample_idx = 0;
       sample_idx < (sizeof(kAdditionalParseSampleIntervals)
                     / sizeof(kAdditionalParseSampleIntervals[0]));
       ++sample_idx) {
      type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeIntervalType>(
          kAdditionalParseSampleIntervals[sample_idx],
          kAdditionalParseSampleStrings[sample_idx]);

      // Also check the negative version of the same interval.
      std::string negative_string("-");
      negative_string.append(kAdditionalParseSampleStrings[sample_idx]);
      type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeIntervalType>(
          - kAdditionalParseSampleIntervals[sample_idx], negative_string);
  }

  // Test some strings with quote marks as allowed by SQL.
  type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeIntervalType>(
      DatetimeIntervalLit{123 * DatetimeIntervalLit::kTicksPerSecond},
      "'123 seconds'");
  type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeIntervalType>(
      DatetimeIntervalLit{123 * DatetimeIntervalLit::kTicksPerSecond},
      "'123' seconds");
  type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeIntervalType>(
      DatetimeIntervalLit{-123 * DatetimeIntervalLit::kTicksPerSecond},
      "'-123 seconds'");
  type_test::CheckSuccessfulParseLiteralValueFromString<DatetimeIntervalType>(
      DatetimeIntervalLit{-123 * DatetimeIntervalLit::kTicksPerSecond},
      "'-123' seconds");

  // Test some parses that we expect to fail.
  const Type &datetime_interval_type = TypeFactory::GetType(kDatetimeInterval);
  TypedValue value;
  EXPECT_FALSE(datetime_interval_type.parseValueFromString("foo", &value));
  EXPECT_FALSE(datetime_interval_type.parseValueFromString("42", &value));
  EXPECT_FALSE(datetime_interval_type.parseValueFromString("--01:35:00", &value));
  EXPECT_FALSE(datetime_interval_type.parseValueFromString("--1:35:00", &value));
  EXPECT_FALSE(datetime_interval_type.parseValueFromString("00:-35:00", &value));
  EXPECT_FALSE(datetime_interval_type.parseValueFromString("00:-3:00", &value));
  EXPECT_FALSE(datetime_interval_type.parseValueFromString("1 day -01:23:45", &value));
  EXPECT_FALSE(datetime_interval_type.parseValueFromString("2 dias", &value));
  EXPECT_FALSE(datetime_interval_type.parseValueFromString("1 month", &value));
  EXPECT_FALSE(datetime_interval_type.parseValueFromString("1 day 12 seconds", &value));
  EXPECT_FALSE(datetime_interval_type.parseValueFromString("1997-08-29T02:14:00", &value));
  EXPECT_FALSE(datetime_interval_type.parseValueFromString("01:23:45 ", &value));
  EXPECT_FALSE(datetime_interval_type.parseValueFromString(" 01:23:45 ", &value));
  EXPECT_FALSE(datetime_interval_type.parseValueFromString("1 second ", &value));
  EXPECT_FALSE(datetime_interval_type.parseValueFromString(" 1 second ", &value));
  EXPECT_FALSE(datetime_interval_type.parseValueFromString("'123' 'seconds'", &value));
  EXPECT_FALSE(datetime_interval_type.parseValueFromString("123 'seconds'", &value));
  EXPECT_FALSE(datetime_interval_type.parseValueFromString("-'123 seconds'", &value));
  EXPECT_FALSE(datetime_interval_type.parseValueFromString("-'123' seconds", &value));
}

TEST(DatetimeIntervalTypeTest, IsCoercibleFromTest) {
  type_test::CheckIsCoercibleFrom(DatetimeIntervalType::InstanceNonNullable(),
                                  {kDatetimeInterval});
}

TEST(DatetimeIntervalTypeTest, IsSafelyCoercibleFromTest) {
  type_test::CheckIsSafelyCoercibleFrom(DatetimeIntervalType::InstanceNonNullable(),
                                        {kDatetimeInterval});
}

TEST(DatetimeIntervalTypeTest, CoerceValueTest) {
  // No change.
  type_test::CheckLiteralCoerce<DatetimeIntervalType, DatetimeIntervalType>(
      DatetimeIntervalLit{0});

  // 123 ticks forward (microseconds).
  type_test::CheckLiteralCoerce<DatetimeIntervalType, DatetimeIntervalType>(
      DatetimeIntervalLit{123});

  // 123 ticks backward (microseconds).
  type_test::CheckLiteralCoerce<DatetimeIntervalType, DatetimeIntervalType>(
      DatetimeIntervalLit{-123});

  type_test::CheckNullLiteralCoerce<DatetimeIntervalType, DatetimeIntervalType>();
  type_test::CheckNullLiteralCoerce<DatetimeIntervalType, NullType>();
}

}  // namespace quickstep
