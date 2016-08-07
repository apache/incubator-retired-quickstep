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

#include "types/DateType.hpp"
#include "types/DatetimeLit.hpp"
#include "types/NullType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/tests/TypeTest_common.hpp"
#include "utility/MemStream.hpp"

#include "gtest/gtest.h"

namespace quickstep {

TEST(DateTypeTest, PrintValueTest) {
  const Type &date_type = TypeFactory::GetType(kDate);

  // Skynet becomes self-aware at 2:14 AM on August 29, 1997.
  TypedValue judgement_day_value(DateLit::Create(1997, 8, 29));
  EXPECT_EQ(std::string("1997-08-29"),
            date_type.printValueToString(judgement_day_value));

  // Check extended formatting for years far in the past.
  TypedValue dawn_age_value(DateLit::Create(-18017, 4, 13));
  EXPECT_EQ(std::string("-18017-04-13"),
            date_type.printValueToString(dawn_age_value));
}

namespace {

static const std::size_t kPaddedLength = 200;

void CheckPrintValueToFile(const DateLit literal_value,
                           const std::string &expected_string) {
  // Test writing to FILE* with a MemStream.
  MemStream memstream;

  const Type &date_type = TypeFactory::GetType(kDate);
  const TypedValue date_value = date_type.makeValue(&literal_value);

  // First, check printing without any padding
  date_type.printValueToFile(date_value, memstream.file());
  EXPECT_STREQ(expected_string.c_str(), memstream.str());
  memstream.reset();

  // Specifying padding less than the length needed to print the value should
  // have no effect.
  date_type.printValueToFile(date_value, memstream.file(), 1);
  EXPECT_STREQ(expected_string.c_str(), memstream.str());
  memstream.reset();

  // Test padding up to a specified length.
  std::string padded(kPaddedLength - expected_string.size(), ' ');
  padded.append(expected_string);
  date_type.printValueToFile(date_value, memstream.file(), kPaddedLength);
  EXPECT_STREQ(padded.c_str(), memstream.str());
  memstream.reset();
}

}  // namespace

TEST(DateTypeTest, PrintValueToFileTest) {
  CheckPrintValueToFile(DateLit::Create(1997, 8, 29), "1997-08-29");

  CheckPrintValueToFile(DateLit::Create(-18017, 4, 13), "-18017-04-13");
}

TEST(DateTypeTest, ParseValueFromStringTest) {
  // Parse the same value in a bunch of different valid format variations.
  type_test::CheckSuccessfulParseLiteralValueFromString<DateType>(
      DateLit::Create(1997, 8, 29),
      "1997-08-29");
  type_test::CheckSuccessfulParseLiteralValueFromString<DateType>(
      DateLit::Create(1997, 8, 29),
      "+1997-08-29");
  type_test::CheckSuccessfulParseLiteralValueFromString<DateType>(
      DateLit::Create(1997, 8, 29),
      "+001997-08-29");

  type_test::CheckSuccessfulParseLiteralValueFromString<DateType>(
      DateLit::Create(1871, 3, 18),
      "1871-03-18");
  type_test::CheckSuccessfulParseLiteralValueFromString<DateType>(
      DateLit::Create(1871, 3, 18),
      "+1871-03-18");
  type_test::CheckSuccessfulParseLiteralValueFromString<DateType>(
      DateLit::Create(1871, 3, 18),
      "+001871-03-18");

  // Parse extended format with negative year.
  type_test::CheckSuccessfulParseLiteralValueFromString<DateType>(
      DateLit::Create(-18017, 4, 13),
      "-18017-04-13");

  // Test some parses that we expect to fail.
  const Type &date_type = TypeFactory::GetType(kDate);
  TypedValue value;
  EXPECT_FALSE(date_type.parseValueFromString("foo", &value));
  EXPECT_FALSE(date_type.parseValueFromString("42", &value));
  EXPECT_FALSE(date_type.parseValueFromString("1997-08-29 ", &value));
  EXPECT_FALSE(date_type.parseValueFromString(" 1997-08-29 ", &value));
  EXPECT_FALSE(date_type.parseValueFromString("1997-08-29.", &value));
  EXPECT_FALSE(date_type.parseValueFromString("1997-08-29q", &value));
  EXPECT_FALSE(date_type.parseValueFromString("1997-08-29.q", &value));
  EXPECT_FALSE(date_type.parseValueFromString("1e3-08-29", &value));
  EXPECT_FALSE(date_type.parseValueFromString("--18017-04-13", &value));
  EXPECT_FALSE(date_type.parseValueFromString("1997-08--29 ", &value));
  EXPECT_FALSE(date_type.parseValueFromString("1997-08--2 ", &value));
}

TEST(DateTypeTest, IsCoercibleFromTest) {
  type_test::CheckIsCoercibleFrom(DateType::InstanceNonNullable(),
                                  {kDate});
}

TEST(DateTypeTest, IsSafelyCoercibleFromTest) {
  type_test::CheckIsSafelyCoercibleFrom(DateType::InstanceNonNullable(),
                                        {kDate});
}

TEST(DateTypeTest, CoerceValueTest) {
  // The epoch.
  type_test::CheckLiteralCoerce<DateType, DateType>(DateLit::Create(1970, 1, 1));

  type_test::CheckNullLiteralCoerce<DateType, DateType>();
  type_test::CheckNullLiteralCoerce<DateType, NullType>();
}

}  // namespace quickstep
