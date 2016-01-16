/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include <cstddef>
#include <string>

#include "types/IntervalLit.hpp"
#include "types/NullType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/YearMonthIntervalType.hpp"
#include "types/tests/TypeTest_common.hpp"
#include "utility/MemStream.hpp"

#include "gtest/gtest.h"

namespace quickstep {

namespace {

static const YearMonthIntervalLit kSampleIntervals[] = {
    {0}, {1}, {2},
    {12}, {13}, {14},
    {24}, {25}, {26}};

static const char *kSampleStrings[] = {
    "0 mons", "1 mon", "2 mons",
    "1 year", "1 year 1 mon", "1 year 2 mons",
    "2 years", "2 years 1 mon", "2 years 2 mons"};

}  // namespace

TEST(YearMonthIntervalTypeTest, PrintValueTest) {
  const Type &yearmonth_interval_type = TypeFactory::GetType(kYearMonthInterval);
  for (std::size_t sample_idx = 0;
       sample_idx < (sizeof(kSampleIntervals) / sizeof(kSampleIntervals[0]));
       ++sample_idx) {
    EXPECT_EQ(std::string(kSampleStrings[sample_idx]),
              yearmonth_interval_type.printValueToString(TypedValue(kSampleIntervals[sample_idx])));

    // Also check the negative version of the same interval.
    if (kSampleIntervals[sample_idx].months != 0) {
      std::string negative_string("-");
      negative_string.append(kSampleStrings[sample_idx]);
      EXPECT_EQ(negative_string,
                yearmonth_interval_type.printValueToString(TypedValue(- kSampleIntervals[sample_idx])));
    }
  }
}

namespace {

static const std::size_t kPaddedLength = 200;

void CheckPrintValueToFile(const YearMonthIntervalLit literal_value,
                           const std::string &expected_string) {
  // Test writing to FILE* with a MemStream.
  MemStream memstream;

  const Type &yearmonth_interval_type = TypeFactory::GetType(kYearMonthInterval);
  const TypedValue yearmonth_interval_value
      = yearmonth_interval_type.makeValue(&literal_value);

  // First, check printing without any padding
  yearmonth_interval_type.printValueToFile(yearmonth_interval_value, memstream.file());
  EXPECT_STREQ(expected_string.c_str(), memstream.str());
  memstream.reset();

  // Specifying padding less than the length needed to print the value should
  // have no effect.
  yearmonth_interval_type.printValueToFile(yearmonth_interval_value, memstream.file(), 1);
  EXPECT_STREQ(expected_string.c_str(), memstream.str());
  memstream.reset();

  // Test padding up to a specified length.
  std::string padded(kPaddedLength - expected_string.size(), ' ');
  padded.append(expected_string);
  yearmonth_interval_type.printValueToFile(yearmonth_interval_value,
                                           memstream.file(),
                                           kPaddedLength);
  EXPECT_STREQ(padded.c_str(), memstream.str());
  memstream.reset();
}

}  // namespace

TEST(YearMonthIntervalTypeTest, PrintValueToFileTest) {
  for (std::size_t sample_idx = 0;
       sample_idx < (sizeof(kSampleIntervals) / sizeof(kSampleIntervals[0]));
       ++sample_idx) {
    CheckPrintValueToFile(kSampleIntervals[sample_idx],
                          kSampleStrings[sample_idx]);

    // Also check the negative version of the same interval.
    if (kSampleIntervals[sample_idx].months != 0) {
      std::string negative_string("-");
      negative_string.append(kSampleStrings[sample_idx]);
      CheckPrintValueToFile(- kSampleIntervals[sample_idx],
                            negative_string);
    }
  }
}

namespace {

static const YearMonthIntervalLit kAdditionalParseSampleIntervals[] = {
    {0},
    {0},
    {0},
    {12 * 12 + 6},
    {123},
    {123},
    {123},
    {123},
    {123 * 12},
    {123 * 12},
    {123 * 12},
    {123 * 10 * 12},
    {123 * 10 * 12},
    {123 * 100 * 12},
    {123 * 100 * 12},
    {123 * 1000 * 12},
    {123 * 1000 * 12}
};

static const char *kAdditionalParseSampleStrings[] = {
    "0 years 0 mons",
    "0 years",
    "0 mons",
    "12 y 6 month",
    "123 mon",
    "123 mons",
    "123 mONth",
    "123 months",
    "123 y",
    "123 year",
    "123 years",
    "123 deCaDe",
    "123 decades",
    "123 century",
    "123 centuries",
    "123 millennium",
    "123 millennia"
};

}  // namespace

TEST(YearMonthIntervalTypeTest, ParseValueFromStringTest) {
  // Test various valid strings.
  for (std::size_t sample_idx = 0;
       sample_idx < (sizeof(kSampleIntervals) / sizeof(kSampleIntervals[0]));
       ++sample_idx) {
      type_test::CheckSuccessfulParseLiteralValueFromString<YearMonthIntervalType>(
          kSampleIntervals[sample_idx], kSampleStrings[sample_idx]);

      // Also check the negative version of the same interval.
      std::string negative_string("-");
      negative_string.append(kSampleStrings[sample_idx]);
      type_test::CheckSuccessfulParseLiteralValueFromString<YearMonthIntervalType>(
          - kSampleIntervals[sample_idx], negative_string);
  }

  // Test additional strings that are still valid for parsing but aren't part
  // of the narrower canonical print format.
  for (std::size_t sample_idx = 0;
       sample_idx < (sizeof(kAdditionalParseSampleIntervals)
                     / sizeof(kAdditionalParseSampleIntervals[0]));
       ++sample_idx) {
      type_test::CheckSuccessfulParseLiteralValueFromString<YearMonthIntervalType>(
          kAdditionalParseSampleIntervals[sample_idx],
          kAdditionalParseSampleStrings[sample_idx]);

      // Also check the negative version of the same interval.
      std::string negative_string("-");
      negative_string.append(kAdditionalParseSampleStrings[sample_idx]);
      type_test::CheckSuccessfulParseLiteralValueFromString<YearMonthIntervalType>(
          - kAdditionalParseSampleIntervals[sample_idx], negative_string);
  }

  // Test some strings with quote marks as allowed by SQL.
  type_test::CheckSuccessfulParseLiteralValueFromString<YearMonthIntervalType>(
      YearMonthIntervalLit{123},
      "'123 mons'");
  type_test::CheckSuccessfulParseLiteralValueFromString<YearMonthIntervalType>(
      YearMonthIntervalLit{123},
      "'123' mons");
  type_test::CheckSuccessfulParseLiteralValueFromString<YearMonthIntervalType>(
      YearMonthIntervalLit{-123},
      "'-123 mons'");
  type_test::CheckSuccessfulParseLiteralValueFromString<YearMonthIntervalType>(
      YearMonthIntervalLit{-123},
      "'-123' mons");

  // Test some parses that we expect to fail.
  const Type &yearmonth_interval_type = TypeFactory::GetType(kYearMonthInterval);
  TypedValue value;
  EXPECT_FALSE(yearmonth_interval_type.parseValueFromString("foo", &value));
  EXPECT_FALSE(yearmonth_interval_type.parseValueFromString("42", &value));
  EXPECT_FALSE(yearmonth_interval_type.parseValueFromString("--1 month", &value));
  EXPECT_FALSE(yearmonth_interval_type.parseValueFromString("1 year -1 month", &value));
  EXPECT_FALSE(yearmonth_interval_type.parseValueFromString("-1 year -1 month", &value));
  EXPECT_FALSE(yearmonth_interval_type.parseValueFromString("1 day", &value));
  EXPECT_FALSE(yearmonth_interval_type.parseValueFromString("1 century 1 month", &value));
  EXPECT_FALSE(yearmonth_interval_type.parseValueFromString("1 year 1 month ", &value));
  EXPECT_FALSE(yearmonth_interval_type.parseValueFromString(" 1 year 1 month ", &value));
  EXPECT_FALSE(yearmonth_interval_type.parseValueFromString("2 millenniums", &value));
  EXPECT_FALSE(yearmonth_interval_type.parseValueFromString("1 month ", &value));
  EXPECT_FALSE(yearmonth_interval_type.parseValueFromString(" 1 month ", &value));
  EXPECT_FALSE(yearmonth_interval_type.parseValueFromString("'123' 'mons'", &value));
  EXPECT_FALSE(yearmonth_interval_type.parseValueFromString("123 'mons'", &value));
  EXPECT_FALSE(yearmonth_interval_type.parseValueFromString("-'123 mons'", &value));
  EXPECT_FALSE(yearmonth_interval_type.parseValueFromString("-'123' mons", &value));
}

TEST(YearMonthIntervalTypeTest, IsCoercibleFromTest) {
  type_test::CheckIsCoercibleFrom(YearMonthIntervalType::InstanceNonNullable(),
                                  {kYearMonthInterval});
}

TEST(YearMonthIntervalTypeTest, IsSafelyCoercibleFromTest) {
  type_test::CheckIsSafelyCoercibleFrom(YearMonthIntervalType::InstanceNonNullable(),
                                        {kYearMonthInterval});
}

TEST(YearMonthIntervalTypeTest, CoerceValueTest) {
  // No change.
  type_test::CheckLiteralCoerce<YearMonthIntervalType, YearMonthIntervalType>(
      YearMonthIntervalLit{0});

  // 123 months (10 years, 3 months) forward.
  type_test::CheckLiteralCoerce<YearMonthIntervalType, YearMonthIntervalType>(
      YearMonthIntervalLit{123});

  // 123 months (10 years, 3 months) backward.
  type_test::CheckLiteralCoerce<YearMonthIntervalType, YearMonthIntervalType>(
      YearMonthIntervalLit{-123});

  type_test::CheckNullLiteralCoerce<YearMonthIntervalType, YearMonthIntervalType>();
  type_test::CheckNullLiteralCoerce<YearMonthIntervalType, NullType>();
}

}  // namespace quickstep
