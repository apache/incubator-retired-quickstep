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
#include <ctime>
#include <random>
#include <vector>

#include "types/port/timegm.hpp"

#include "glog/logging.h"

#include "gtest/gtest.h"

namespace quickstep {
namespace {

constexpr int kDaysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
constexpr int kDaysInMonthLeapYear[] = {
    31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

}  // namespace

// Test custom timegm() for few years around epoch.
TEST(timegmTest, FewYearsAroundEpoch) {
  struct tm tm = {};
  for (int year = 1960; year <= 1980; ++year) {
    for (int month = 1; month <= 12; ++month) {
      const int num_days = (year % 4 == 0) ? kDaysInMonthLeapYear[month - 1]
                                           : kDaysInMonth[month - 1];
      for (int day = 1; day <= num_days; ++day) {
        tm.tm_year = year - 1900;
        tm.tm_mon = month - 1;
        tm.tm_mday = day;
        struct tm tm_copy = tm;  // timegm can change struct tm.
        ASSERT_EQ(timegmWithPrecomputedOffset(&tm_copy), timegmCustom(&tm))
            << "Wrong seconds since epoch for " << year << '-' << month + 1
            << '-' << day;
      }
    }
  }
}

// Test custom timegm() for few 100 years around epoch.
TEST(timegmTest, Few100YearsAroundEpoch) {
  struct tm tm = {};
  const int num_samples = 20;
  int year = 1970 - num_samples * 50;
  for (int i = 0; i < num_samples; ++i, year += 100) {
    tm.tm_year = year - 1900;
    tm.tm_mon = 0;
    tm.tm_mday = 1;
    struct tm tm_copy = tm;  // timegm can change struct tm.
    const std::time_t expected = timegmSystemImplemented(&tm_copy);
    if (expected != -1) {
      ASSERT_EQ(expected, timegmCustom(&tm)) << "Wrong seconds since epoch for "
                                             << year << '-' << 1 << '-' << 1;
    } else {
      // NOTE: MacOSX timegm() implementation returns -1 for many years 970-1-1.
      LOG_FIRST_N(WARNING, 3) << "System-based timegm() returned -1 for "
                              << year << "-1-1.";
    }
  }
}

// Test custom timegm() for extended ISO8601 representation of years.
TEST(timegmTest, ExtendedISO8601) {
  struct tm tm = {};
  std::vector<int> years = {99999, 50000, 15000, 10000, 0, -10000, -15000, -50000, -99999};
  for (const int year : years) {
    tm.tm_year = year - 1900;
    tm.tm_mon = 0;
    tm.tm_mday = 1;
    struct tm tm_copy = tm;  // timegm can change struct tm.
    const std::time_t expected = timegmSystemImplemented(&tm_copy);
    if (expected != -1) {
      ASSERT_EQ(expected, timegmCustom(&tm)) << "Wrong seconds since epoch for "
                                             << year << '-' << 1 << '-' << 1;
    } else {
      // NOTE: MacOSX timegm() implementation returns -1 for many years 970-1-1.
      LOG_FIRST_N(WARNING, 3) << "System-based timegm() returned -1 for "
                              << year << "-1-1.";
    }
  }
}

// Fuzz test custom timegm(), and compare to system implementation.
TEST(timegmTest, FuzzTest) {
  struct tm tm = {};
  const std::size_t samples = 100000;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> year(-10000, 10000);
  std::uniform_int_distribution<> month(0, 11);
  std::uniform_int_distribution<> day(1, 28);
  std::uniform_int_distribution<> hours(0, 23);
  std::uniform_int_distribution<> min_sec(0, 59);

  for (std::size_t i = 0; i < samples; ++i) {
    tm.tm_year = year(gen) - 1970;
    tm.tm_mon = month(gen);
    tm.tm_mday = day(gen);
    tm.tm_hour = hours(gen);
    tm.tm_min = min_sec(gen);
    tm.tm_sec = min_sec(gen);

    struct tm tm_copy = tm;  // timegm can change struct tm.
    const std::time_t expected = timegmSystemImplemented(&tm_copy);
    if (expected != -1) {
      ASSERT_EQ(expected, timegmCustom(&tm))
          << "Wrong seconds since epoch for " << tm.tm_year + 1970 << '-'
          << tm.tm_mon + 1 << '-' << tm.tm_mday << ' ' << tm.tm_hour << ':'
          << tm.tm_min << ':' << tm.tm_sec;
    } else {
      // NOTE: MacOSX timegm() implementation returns -1 for many years 970-1-1.
      LOG_FIRST_N(WARNING, 3) << "System-based timegm() returned -1 for "
                              << tm.tm_year + 1970 << '-' << tm.tm_mon + 1
                              << '-' << tm.tm_mday << ' ' << tm.tm_hour << ':'
                              << tm.tm_min << ':' << tm.tm_sec;
    }
  }
}

}  // namespace quickstep
