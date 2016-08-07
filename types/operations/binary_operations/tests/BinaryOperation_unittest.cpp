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

#include <cstdint>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include "types/DateOperatorOverloads.hpp"
#include "types/DatetimeLit.hpp"
#include "types/IntervalLit.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/Operation.pb.h"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"
#include "gtest/gtest.h"

using std::int64_t;
using std::numeric_limits;
using std::pair;
using std::unique_ptr;
using std::vector;

namespace quickstep {

class BinaryOperationTest : public ::testing::Test {
 protected:
  template <typename NumericType>
  static NumericType GetTypedValueAsNumeric(const TypedValue &value) {
    switch (value.getTypeID()) {
      case kInt:
        return value.getLiteral<int>();
      case kLong:
        return value.getLiteral<std::int64_t>();
      case kFloat:
        return value.getLiteral<float>();
      case kDouble:
        return value.getLiteral<double>();
      default:
        FATAL_ERROR("TypedValue does not appear to be numeric.");
    }
  }

  static bool TypedValueIsZero(const TypedValue &value) {
    switch (value.getTypeID()) {
      case kInt:
        return value.getLiteral<int>() == 0;
      case kLong:
        return value.getLiteral<std::int64_t>() == 0;
      case kFloat:
        return value.getLiteral<float>() == 0.0f;
      case kDouble:
        return value.getLiteral<double>() == 0.0;
      default:
        FATAL_ERROR("TypedValue does not appear to be numeric.");
    }
  }

  virtual void SetUp() {
    int_null_.reset(new TypedValue(kInt));
    int_zero_.reset(new TypedValue(0));
    int_positive_.reset(new TypedValue(12345));
    int_negative_.reset(new TypedValue(- int_positive_->getLiteral<int>()));
    int_max_.reset(new TypedValue(numeric_limits<int>::max()));
    int_min_.reset(new TypedValue(numeric_limits<int>::min()));

    numeric_typed_values_.emplace_back(int_null_.get(),
                                       &TypeFactory::GetType(kInt, true));
    numeric_typed_values_.emplace_back(int_zero_.get(),
                                       &TypeFactory::GetType(kInt, false));
    numeric_typed_values_.emplace_back(int_positive_.get(),
                                       &TypeFactory::GetType(kInt, false));
    numeric_typed_values_.emplace_back(int_negative_.get(),
                                       &TypeFactory::GetType(kInt, false));
    numeric_typed_values_.emplace_back(int_max_.get(),
                                       &TypeFactory::GetType(kInt, false));
    numeric_typed_values_.emplace_back(int_min_.get(),
                                       &TypeFactory::GetType(kInt, false));

    long_null_.reset(new TypedValue(kLong));
    long_zero_.reset(new TypedValue(static_cast<int64_t>(0)));
    long_positive_.reset(new TypedValue(static_cast<int64_t>(12345)));
    long_negative_.reset(new TypedValue(- long_positive_->getLiteral<int64_t>()));
    long_max_.reset(new TypedValue(numeric_limits<int64_t>::max()));
    long_min_.reset(new TypedValue(numeric_limits<int64_t>::min()));

    numeric_typed_values_.emplace_back(long_null_.get(),
                                       &TypeFactory::GetType(kLong, true));
    numeric_typed_values_.emplace_back(long_zero_.get(),
                                       &TypeFactory::GetType(kLong, false));
    numeric_typed_values_.emplace_back(long_positive_.get(),
                                       &TypeFactory::GetType(kLong, false));
    numeric_typed_values_.emplace_back(long_negative_.get(),
                                       &TypeFactory::GetType(kLong, false));
    numeric_typed_values_.emplace_back(long_max_.get(),
                                       &TypeFactory::GetType(kLong, false));
    numeric_typed_values_.emplace_back(long_min_.get(),
                                       &TypeFactory::GetType(kLong, false));

    float_null_.reset(new TypedValue(kFloat));
    float_zero_.reset(new TypedValue(static_cast<float>(0.0)));
    float_positive_.reset(new TypedValue(static_cast<float>(123.45)));
    float_negative_.reset(new TypedValue(- float_positive_->getLiteral<float>()));
    float_max_.reset(new TypedValue(numeric_limits<float>::max()));
    float_min_.reset(new TypedValue(numeric_limits<float>::min()));

    numeric_typed_values_.emplace_back(float_null_.get(),
                                       &TypeFactory::GetType(kFloat, true));
    numeric_typed_values_.emplace_back(float_zero_.get(),
                                       &TypeFactory::GetType(kFloat, false));
    numeric_typed_values_.emplace_back(float_positive_.get(),
                                       &TypeFactory::GetType(kFloat, false));
    numeric_typed_values_.emplace_back(float_negative_.get(),
                                       &TypeFactory::GetType(kFloat, false));
    numeric_typed_values_.emplace_back(float_max_.get(),
                                       &TypeFactory::GetType(kFloat, false));
    numeric_typed_values_.emplace_back(float_min_.get(),
                                       &TypeFactory::GetType(kFloat, false));

    double_null_.reset(new TypedValue(kDouble));
    double_zero_.reset(new TypedValue(static_cast<double>(0)));
    double_positive_.reset(new TypedValue(static_cast<double>(123.45)));
    double_negative_.reset(new TypedValue(- double_positive_->getLiteral<double>()));
    double_max_.reset(new TypedValue(numeric_limits<double>::max()));
    double_min_.reset(new TypedValue(numeric_limits<double>::min()));

    numeric_typed_values_.emplace_back(double_null_.get(),
                                       &TypeFactory::GetType(kDouble, true));
    numeric_typed_values_.emplace_back(double_zero_.get(),
                                       &TypeFactory::GetType(kDouble, false));
    numeric_typed_values_.emplace_back(double_positive_.get(),
                                       &TypeFactory::GetType(kDouble, false));
    numeric_typed_values_.emplace_back(double_negative_.get(),
                                       &TypeFactory::GetType(kDouble, false));
    numeric_typed_values_.emplace_back(double_max_.get(),
                                       &TypeFactory::GetType(kDouble, false));
    numeric_typed_values_.emplace_back(double_min_.get(),
                                       &TypeFactory::GetType(kDouble, false));

    datetime_null_.reset(new TypedValue(kDatetime));
    DatetimeLit datetime;
    datetime.ticks = 0;
    datetime_zero_.reset(new TypedValue(datetime));
    datetime.ticks = 12345678;
    datetime_positive_.reset(new TypedValue(datetime));
    datetime.ticks = -87654321;
    datetime_negative_.reset(new TypedValue(datetime));
    datetime.ticks = numeric_limits<int64_t>::max();
    datetime_max_.reset(new TypedValue(datetime));
    datetime.ticks = numeric_limits<int64_t>::min();
    datetime_min_.reset(new TypedValue(datetime));

    datetime_typed_values_.emplace_back(datetime_null_.get(),
                                        &TypeFactory::GetType(kDatetime, true));
    datetime_typed_values_.emplace_back(datetime_zero_.get(),
                                        &TypeFactory::GetType(kDatetime, false));
    datetime_typed_values_.emplace_back(datetime_positive_.get(),
                                        &TypeFactory::GetType(kDatetime, false));
    datetime_typed_values_.emplace_back(datetime_negative_.get(),
                                        &TypeFactory::GetType(kDatetime, false));
    datetime_typed_values_.emplace_back(datetime_max_.get(),
                                        &TypeFactory::GetType(kDatetime, false));
    datetime_typed_values_.emplace_back(datetime_min_.get(),
                                        &TypeFactory::GetType(kDatetime, false));

    datetime_interval_null_.reset(new TypedValue(kDatetimeInterval));
    DatetimeIntervalLit datetime_interval;
    datetime_interval.interval_ticks = 0;
    datetime_interval_zero_.reset(new TypedValue(datetime_interval));
    datetime_interval.interval_ticks = 87654321;
    datetime_interval_positive_.reset(new TypedValue(datetime_interval));
    datetime_interval.interval_ticks = -12345678;
    datetime_interval_negative_.reset(new TypedValue(datetime_interval));

    datetime_interval_typed_values_.emplace_back(datetime_interval_null_.get(),
                                                 &TypeFactory::GetType(kDatetimeInterval, true));
    datetime_interval_typed_values_.emplace_back(datetime_interval_zero_.get(),
                                                 &TypeFactory::GetType(kDatetimeInterval, false));
    datetime_interval_typed_values_.emplace_back(datetime_interval_positive_.get(),
                                                 &TypeFactory::GetType(kDatetimeInterval, false));
    datetime_interval_typed_values_.emplace_back(datetime_interval_negative_.get(),
                                                 &TypeFactory::GetType(kDatetimeInterval, false));

    year_month_interval_null_.reset(new TypedValue(kYearMonthInterval));
    YearMonthIntervalLit year_month_interval;
    year_month_interval.months = 0;
    year_month_interval_zero_.reset(new TypedValue(year_month_interval));
    year_month_interval.months = 12;  // 1 year forward.
    year_month_interval_positive_.reset(new TypedValue(year_month_interval));
    year_month_interval.months = -3;  // 3 months backward.
    year_month_interval_negative_.reset(new TypedValue(year_month_interval));

    year_month_interval_typed_values_.emplace_back(year_month_interval_null_.get(),
                                                   &TypeFactory::GetType(kYearMonthInterval, true));
    year_month_interval_typed_values_.emplace_back(year_month_interval_zero_.get(),
                                                   &TypeFactory::GetType(kYearMonthInterval, false));
    year_month_interval_typed_values_.emplace_back(year_month_interval_positive_.get(),
                                                   &TypeFactory::GetType(kYearMonthInterval, false));
    year_month_interval_typed_values_.emplace_back(year_month_interval_negative_.get(),
                                                   &TypeFactory::GetType(kYearMonthInterval, false));
  }

  void checkNumericBinaryOperation(const BinaryOperation &operation) {
    for (const pair<const TypedValue*, const Type*> &left_item : numeric_typed_values_) {
      for (const pair<const TypedValue*, const Type*> &right_item : numeric_typed_values_) {
        if ((operation.getBinaryOperationID() == BinaryOperationID::kDivide ||
             operation.getBinaryOperationID() == BinaryOperationID::kModulo) &&
            !right_item.first->isNull() &&
            TypedValueIsZero(*right_item.first)) {
          // Avoid Divide-by-zero errors.
          continue;
        }

        checkNumericBinaryOperationChecked(operation,
                                           *left_item.first,
                                           *left_item.second,
                                           *right_item.first,
                                           *right_item.second);
        checkNumericBinaryOperatorUnchecked(operation,
                                            *left_item.first,
                                            *left_item.second,
                                            *right_item.first,
                                            *right_item.second);
      }
    }
  }

  void checkDateAddBinaryOperators() {
    // Add DatetimeLit and DatetimeIntervalLit.
    DatetimeLit datetime_1995_08_01;
    datetime_1995_08_01.ticks = 807235200020000;  // 01 Aug 1995 00:00:00 GMT.

    DatetimeIntervalLit interval_20_days;
    interval_20_days.interval_ticks = 1728000000000;

    DatetimeLit datetime_1995_08_21;
    datetime_1995_08_21.ticks = 808963200020000;  // 21 Aug 1995 00:00:00 GMT.

    EXPECT_EQ(datetime_1995_08_21, datetime_1995_08_01 + interval_20_days);
    EXPECT_EQ(datetime_1995_08_21, interval_20_days + datetime_1995_08_01);

    // Add DatetimeIntervalLit and DatetimeIntervalLit.
    DatetimeIntervalLit interval_40_days;
    interval_40_days.interval_ticks = 3456000000000;
    EXPECT_EQ(interval_40_days, interval_20_days + interval_20_days);

    // Add DatetimeLit and YearMonthIntervalLit.
    DatetimeLit datetime_1997_01_01;
    datetime_1997_01_01.ticks = 852076800000300;  // 01 Jan 1997 00:00:00 GMT.

    YearMonthIntervalLit interval_1_year;
    interval_1_year.months = 12;

    DatetimeLit datetime_1998_01_01;
    datetime_1998_01_01.ticks = 883612800000300;  // 01 Jan 1998 00:00:00 GMT.

    EXPECT_EQ(datetime_1998_01_01, datetime_1997_01_01 + interval_1_year);
    EXPECT_EQ(datetime_1998_01_01, interval_1_year + datetime_1997_01_01);

    // Add YearMonthIntervalLit and YearMonthIntervalLit.
    YearMonthIntervalLit interval_2_years;
    interval_2_years.months = 24;

    EXPECT_EQ(interval_2_years, interval_1_year + interval_1_year);
  }

  void checkDatetimeAddBinaryOperation() {
    const BinaryOperation &operation(
        BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd));

    for (const pair<const TypedValue*, const Type*> &left_item : datetime_typed_values_) {
      for (const pair<const TypedValue*, const Type*> &right_item : datetime_interval_typed_values_) {
        TypedValue expected = checkDateAddBinaryOperationHelper(operation,
                                                                *left_item.first,
                                                                *left_item.second,
                                                                *right_item.first,
                                                                *right_item.second);
        checkDateBinaryOperationChecked<DatetimeLit>(operation,
                                                     *left_item.first,
                                                     *left_item.second,
                                                     *right_item.first,
                                                     *right_item.second,
                                                     expected);
        checkDateBinaryOperatorUnchecked<DatetimeLit>(operation,
                                                      *left_item.first,
                                                      *left_item.second,
                                                      *right_item.first,
                                                      *right_item.second,
                                                      expected);
      }

      for (const pair<const TypedValue*, const Type*> &right_item : year_month_interval_typed_values_) {
        TypedValue expected = checkDateAddBinaryOperationHelper(operation,
                                                                *left_item.first,
                                                                *left_item.second,
                                                                *right_item.first,
                                                                *right_item.second);
        checkDateBinaryOperationChecked<DatetimeLit>(operation,
                                                     *left_item.first,
                                                     *left_item.second,
                                                     *right_item.first,
                                                     *right_item.second,
                                                     expected);
        checkDateBinaryOperatorUnchecked<DatetimeLit>(operation,
                                                      *left_item.first,
                                                      *left_item.second,
                                                      *right_item.first,
                                                      *right_item.second,
                                                      expected);
      }
    }
  }

  void checkDatetimeIntervalAddBinaryOperation() {
    const BinaryOperation &operation(
        BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd));

    for (const pair<const TypedValue*, const Type*> &left_item : datetime_interval_typed_values_) {
      for (const pair<const TypedValue*, const Type*> &right_item : datetime_typed_values_) {
        TypedValue expected = checkDateAddBinaryOperationHelper(operation,
                                                                *left_item.first,
                                                                *left_item.second,
                                                                *right_item.first,
                                                                *right_item.second);
        checkDateBinaryOperationChecked<DatetimeLit>(operation,
                                                     *left_item.first,
                                                     *left_item.second,
                                                     *right_item.first,
                                                     *right_item.second,
                                                     expected);
        checkDateBinaryOperatorUnchecked<DatetimeLit>(operation,
                                                      *left_item.first,
                                                      *left_item.second,
                                                      *right_item.first,
                                                      *right_item.second,
                                                      expected);
      }

      for (const pair<const TypedValue*, const Type*> &right_item : datetime_interval_typed_values_) {
        TypedValue expected = checkDateAddBinaryOperationHelper(operation,
                                                                *left_item.first,
                                                                *left_item.second,
                                                                *right_item.first,
                                                                *right_item.second);
        checkDateBinaryOperationChecked<DatetimeIntervalLit>(operation,
                                                             *left_item.first,
                                                             *left_item.second,
                                                             *right_item.first,
                                                             *right_item.second,
                                                             expected);
        checkDateBinaryOperatorUnchecked<DatetimeIntervalLit>(operation,
                                                              *left_item.first,
                                                              *left_item.second,
                                                              *right_item.first,
                                                              *right_item.second,
                                                              expected);
      }
    }
  }

  void checkYearMonthIntervalAddBinaryOperation() {
    const BinaryOperation &operation(
        BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd));

    for (const pair<const TypedValue*, const Type*> &left_item : year_month_interval_typed_values_) {
      for (const pair<const TypedValue*, const Type*> &right_item : datetime_typed_values_) {
        TypedValue expected = checkDateAddBinaryOperationHelper(operation,
                                                                *left_item.first,
                                                                *left_item.second,
                                                                *right_item.first,
                                                                *right_item.second);
        checkDateBinaryOperationChecked<DatetimeLit>(operation,
                                                     *left_item.first,
                                                     *left_item.second,
                                                     *right_item.first,
                                                     *right_item.second,
                                                     expected);
        checkDateBinaryOperatorUnchecked<DatetimeLit>(operation,
                                                      *left_item.first,
                                                      *left_item.second,
                                                      *right_item.first,
                                                      *right_item.second,
                                                      expected);
      }

      for (const pair<const TypedValue*, const Type*> &right_item : year_month_interval_typed_values_) {
        TypedValue expected = checkDateAddBinaryOperationHelper(operation,
                                                                *left_item.first,
                                                                *left_item.second,
                                                                *right_item.first,
                                                                *right_item.second);
        checkDateBinaryOperationChecked<YearMonthIntervalLit>(operation,
                                                              *left_item.first,
                                                              *left_item.second,
                                                              *right_item.first,
                                                              *right_item.second,
                                                              expected);
        checkDateBinaryOperatorUnchecked<YearMonthIntervalLit>(operation,
                                                               *left_item.first,
                                                               *left_item.second,
                                                               *right_item.first,
                                                               *right_item.second,
                                                               expected);
      }
    }
  }

  void checkDateSubtractBinaryOperators() {
    // Subtract DatetimeLit and DatetimeLit.
    DatetimeLit datetime_1998_12_01;
    datetime_1998_12_01.ticks = 912470400600000;  // 01 Dec 1998 00:00:00 GMT.

    DatetimeIntervalLit interval_96_days;
    interval_96_days.interval_ticks = 8294400000000;

    DatetimeLit datetime_1998_8_27;
    datetime_1998_8_27.ticks = 904176000600000;  // 27 Aug 1998 00:00:00 GMT.

    EXPECT_EQ(interval_96_days, datetime_1998_12_01 - datetime_1998_8_27);

    // Subtract DatetimeIntervalLit and DatetimeIntervalLit.
    DatetimeIntervalLit interval_48_days;
    interval_48_days.interval_ticks = 4147200000000;

    EXPECT_EQ(interval_48_days, interval_96_days - interval_48_days);

    // Subtract DatetimeLit and YearMonthIntervalLit.
    DatetimeLit datetime_2014_12_26;
    datetime_2014_12_26.ticks = 1419552000000001;  // 26 Dec 2014 00:00:00 GMT.

    YearMonthIntervalLit interval_6_months;
    interval_6_months.months = 6;

    DatetimeLit datetime_2014_06_26;
    datetime_2014_06_26.ticks = 1403740800000001;  // 26 Jun 2014 00:00:00 GMT.

    EXPECT_EQ(datetime_2014_06_26, datetime_2014_12_26 - interval_6_months);

    // Subtract DatetimeLit and YearMonthIntervalLit.
    YearMonthIntervalLit interval_1_year;
    interval_1_year.months = 12;

    EXPECT_EQ(interval_6_months, interval_1_year - interval_6_months);
  }

  void checkDatetimeSubtractBinaryOperation() {
    const BinaryOperation &operation(
        BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kSubtract));

    for (const pair<const TypedValue*, const Type*> &left_item : datetime_typed_values_) {
      for (const pair<const TypedValue*, const Type*> &right_item : datetime_typed_values_) {
        TypedValue expected = checkDateSubtractBinaryOperationHelper(operation,
                                                                     *left_item.first,
                                                                     *left_item.second,
                                                                     *right_item.first,
                                                                     *right_item.second);
        checkDateBinaryOperationChecked<DatetimeIntervalLit>(operation,
                                                             *left_item.first,
                                                             *left_item.second,
                                                             *right_item.first,
                                                             *right_item.second,
                                                             expected);
        checkDateBinaryOperatorUnchecked<DatetimeIntervalLit>(operation,
                                                              *left_item.first,
                                                              *left_item.second,
                                                              *right_item.first,
                                                              *right_item.second,
                                                              expected);
      }

      for (const pair<const TypedValue*, const Type*> &right_item : datetime_interval_typed_values_) {
        TypedValue expected = checkDateSubtractBinaryOperationHelper(operation,
                                                                     *left_item.first,
                                                                     *left_item.second,
                                                                     *right_item.first,
                                                                     *right_item.second);
        checkDateBinaryOperationChecked<DatetimeLit>(operation,
                                                     *left_item.first,
                                                     *left_item.second,
                                                     *right_item.first,
                                                     *right_item.second,
                                                     expected);
        checkDateBinaryOperatorUnchecked<DatetimeLit>(operation,
                                                      *left_item.first,
                                                      *left_item.second,
                                                      *right_item.first,
                                                      *right_item.second,
                                                      expected);
      }

      for (const pair<const TypedValue*, const Type*> &right_item : year_month_interval_typed_values_) {
        TypedValue expected = checkDateSubtractBinaryOperationHelper(operation,
                                                                     *left_item.first,
                                                                     *left_item.second,
                                                                     *right_item.first,
                                                                     *right_item.second);
        checkDateBinaryOperationChecked<DatetimeLit>(operation,
                                                     *left_item.first,
                                                     *left_item.second,
                                                     *right_item.first,
                                                     *right_item.second,
                                                     expected);
        checkDateBinaryOperatorUnchecked<DatetimeLit>(operation,
                                                      *left_item.first,
                                                      *left_item.second,
                                                      *right_item.first,
                                                      *right_item.second,
                                                      expected);
      }
    }
  }

  void checkDatetimeIntervalSubtractBinaryOperation() {
    const BinaryOperation &operation(
        BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kSubtract));

    for (const pair<const TypedValue*, const Type*> &left_item : datetime_interval_typed_values_) {
      for (const pair<const TypedValue*, const Type*> &right_item : datetime_interval_typed_values_) {
        TypedValue expected = checkDateSubtractBinaryOperationHelper(operation,
                                                                     *left_item.first,
                                                                     *left_item.second,
                                                                     *right_item.first,
                                                                     *right_item.second);
        checkDateBinaryOperationChecked<DatetimeIntervalLit>(operation,
                                                             *left_item.first,
                                                             *left_item.second,
                                                             *right_item.first,
                                                             *right_item.second,
                                                             expected);
        checkDateBinaryOperatorUnchecked<DatetimeIntervalLit>(operation,
                                                              *left_item.first,
                                                              *left_item.second,
                                                              *right_item.first,
                                                              *right_item.second,
                                                              expected);
      }
    }
  }

  void checkYearMonthIntervalSubtractBinaryOperation() {
    const BinaryOperation &operation(
        BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kSubtract));

    for (const pair<const TypedValue*, const Type*> &left_item : year_month_interval_typed_values_) {
      for (const pair<const TypedValue*, const Type*> &right_item : year_month_interval_typed_values_) {
        TypedValue expected = checkDateSubtractBinaryOperationHelper(operation,
                                                                     *left_item.first,
                                                                     *left_item.second,
                                                                     *right_item.first,
                                                                     *right_item.second);
        checkDateBinaryOperationChecked<YearMonthIntervalLit>(operation,
                                                              *left_item.first,
                                                              *left_item.second,
                                                              *right_item.first,
                                                              *right_item.second,
                                                              expected);
        checkDateBinaryOperatorUnchecked<YearMonthIntervalLit>(operation,
                                                               *left_item.first,
                                                               *left_item.second,
                                                               *right_item.first,
                                                               *right_item.second,
                                                               expected);
      }
    }
  }

  void checkDateMultiplyBinaryOperators() {
    // DatetimeIntervalLit Multiplication.
    DatetimeIntervalLit interval_32_days;
    interval_32_days.interval_ticks = 2764800000000;

    DatetimeIntervalLit interval_96_days;
    interval_96_days.interval_ticks = 8294400000000;

    EXPECT_EQ(interval_96_days, interval_32_days * 3);
    EXPECT_EQ(interval_96_days, 3 * interval_32_days);

    // YearMonthIntervalLit Multiplication.
    YearMonthIntervalLit interval_6_months;
    interval_6_months.months = 6;

    YearMonthIntervalLit interval_2_years;
    interval_2_years.months = 24;

    EXPECT_EQ(interval_2_years, interval_6_months * 4);
    EXPECT_EQ(interval_2_years, 4 * interval_6_months);
  }

  void checkDatetimeIntervalMultiplyBinaryOperation() {
    const BinaryOperation &operation(
        BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kMultiply));

    for (const pair<const TypedValue*, const Type*> &left_item : datetime_interval_typed_values_) {
      for (const pair<const TypedValue*, const Type*> &right_item : numeric_typed_values_) {
        TypedValue expected = checkDateMultiplyBinaryOperationHelper(operation,
                                                                     *left_item.first,
                                                                     *left_item.second,
                                                                     *right_item.first,
                                                                     *right_item.second);
        checkDateBinaryOperationChecked<DatetimeIntervalLit>(operation,
                                                             *left_item.first,
                                                             *left_item.second,
                                                             *right_item.first,
                                                             *right_item.second,
                                                             expected);
        checkDateBinaryOperatorUnchecked<DatetimeIntervalLit>(operation,
                                                              *left_item.first,
                                                              *left_item.second,
                                                              *right_item.first,
                                                              *right_item.second,
                                                              expected);

        // Should also get same results with arguments reversed.
        checkDateBinaryOperationChecked<DatetimeIntervalLit>(operation,
                                                             *right_item.first,
                                                             *right_item.second,
                                                             *left_item.first,
                                                             *left_item.second,
                                                             expected);
        checkDateBinaryOperatorUnchecked<DatetimeIntervalLit>(operation,
                                                              *right_item.first,
                                                              *right_item.second,
                                                              *left_item.first,
                                                              *left_item.second,
                                                              expected);
      }
    }
  }

  void checkYearMonthIntervalMultiplyBinaryOperation() {
    const BinaryOperation &operation(
        BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kMultiply));

    for (const pair<const TypedValue*, const Type*> &left_item : year_month_interval_typed_values_) {
      for (const pair<const TypedValue*, const Type*> &right_item : numeric_typed_values_) {
        TypedValue expected = checkDateMultiplyBinaryOperationHelper(operation,
                                                                     *left_item.first,
                                                                     *left_item.second,
                                                                     *right_item.first,
                                                                     *right_item.second);
        checkDateBinaryOperationChecked<YearMonthIntervalLit>(operation,
                                                              *left_item.first,
                                                              *left_item.second,
                                                              *right_item.first,
                                                              *right_item.second,
                                                              expected);
        checkDateBinaryOperatorUnchecked<YearMonthIntervalLit>(operation,
                                                               *left_item.first,
                                                               *left_item.second,
                                                               *right_item.first,
                                                               *right_item.second,
                                                               expected);

        // Should also get same results with arguments reversed.
        checkDateBinaryOperationChecked<YearMonthIntervalLit>(operation,
                                                              *right_item.first,
                                                              *right_item.second,
                                                              *left_item.first,
                                                              *left_item.second,
                                                              expected);
        checkDateBinaryOperatorUnchecked<YearMonthIntervalLit>(operation,
                                                               *right_item.first,
                                                               *right_item.second,
                                                               *left_item.first,
                                                               *left_item.second,
                                                               expected);
      }
    }
  }

  void checkDateDivideBinaryOperators() {
    // DatetimeIntervalLit Division.
    DatetimeIntervalLit interval_96_days;
    interval_96_days.interval_ticks = 8294400000000;

    DatetimeIntervalLit interval_32_days;
    interval_32_days.interval_ticks = 2764800000000;

    EXPECT_EQ(interval_32_days, interval_96_days / 3);

    // YearMonthIntervalLit Division.
    YearMonthIntervalLit interval_2_years;
    interval_2_years.months = 24;

    YearMonthIntervalLit interval_6_months;
    interval_6_months.months = 6;

    EXPECT_EQ(interval_6_months, interval_2_years / 4);
  }

  void checkDatetimeIntervalDivideBinaryOperation() {
    const BinaryOperation &operation(
        BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kDivide));

    for (const pair<const TypedValue*, const Type*> &left_item : datetime_interval_typed_values_) {
      for (const pair<const TypedValue*, const Type*> &right_item : numeric_typed_values_) {
        if (!right_item.first->isNull() &&
            TypedValueIsZero(*right_item.first)) {
          // Avoid Divide-by-zero errors.
          continue;
        }

        TypedValue expected = checkDateDivideBinaryOperationHelper(operation,
                                                                   *left_item.first,
                                                                   *left_item.second,
                                                                   *right_item.first,
                                                                   *right_item.second);
        checkDateBinaryOperationChecked<DatetimeIntervalLit>(operation,
                                                             *left_item.first,
                                                             *left_item.second,
                                                             *right_item.first,
                                                             *right_item.second,
                                                             expected);
        checkDateBinaryOperatorUnchecked<DatetimeIntervalLit>(operation,
                                                              *left_item.first,
                                                              *left_item.second,
                                                              *right_item.first,
                                                              *right_item.second,
                                                              expected);
      }
    }
  }

  void checkYearMonthIntervalDivideBinaryOperation() {
    const BinaryOperation &operation(
        BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kDivide));

    for (const pair<const TypedValue*, const Type*> &left_item : year_month_interval_typed_values_) {
      for (const pair<const TypedValue*, const Type*> &right_item : numeric_typed_values_) {
        if (!right_item.first->isNull() &&
            TypedValueIsZero(*right_item.first)) {
          // Avoid Divide-by-zero errors.
          continue;
        }

        TypedValue expected = checkDateDivideBinaryOperationHelper(operation,
                                                                   *left_item.first,
                                                                   *left_item.second,
                                                                   *right_item.first,
                                                                   *right_item.second);
        checkDateBinaryOperationChecked<YearMonthIntervalLit>(operation,
                                                              *left_item.first,
                                                              *left_item.second,
                                                              *right_item.first,
                                                              *right_item.second,
                                                              expected);
        checkDateBinaryOperatorUnchecked<YearMonthIntervalLit>(operation,
                                                               *left_item.first,
                                                               *left_item.second,
                                                               *right_item.first,
                                                               *right_item.second,
                                                               expected);
      }
    }
  }

 private:
  void checkNumericBinaryOperationChecked(const BinaryOperation &operation,
                                          const TypedValue &left,
                                          const Type &left_type,
                                          const TypedValue &right,
                                          const Type &right_type) {
    if (left.isNull() || right.isNull()) {
      EXPECT_TRUE(operation.applyToChecked(left, left_type, right, right_type).isNull());
      return;
    }

    const Type* expected_type =
        operation.resultTypeForArgumentTypes(left_type, right_type);
    ASSERT_NE(expected_type, nullptr);

    switch (expected_type->getTypeID()) {
      case kInt:
        checkNumericBinaryOperationCheckedHelper<int>(operation,
                                                      left,
                                                      left_type,
                                                      right,
                                                      right_type);
        break;
      case kLong:
        checkNumericBinaryOperationCheckedHelper<int64_t>(operation,
                                                          left,
                                                          left_type,
                                                          right,
                                                          right_type);
        break;
      case kFloat:
        checkNumericBinaryOperationCheckedHelper<float>(operation,
                                                        left,
                                                        left_type,
                                                        right,
                                                        right_type);
        break;
      case kDouble:
        checkNumericBinaryOperationCheckedHelper<double>(operation,
                                                         left,
                                                         left_type,
                                                         right,
                                                         right_type);
        break;
      default:
        FATAL_ERROR("Unsupported result type from two operands.");
    }
  }

  template <typename NumericType>
  inline void checkNumericBinaryOperationCheckedHelper(const BinaryOperation &operation,
                                                       const TypedValue &left,
                                                       const Type &left_type,
                                                       const TypedValue &right,
                                                       const Type &right_type) {
    NumericType expected = checkNumericBinaryOperationHelper<NumericType>(operation, left, right);
    EXPECT_EQ(expected, operation.applyToChecked(left, left_type,
                                                 right, right_type).getLiteral<NumericType>());
  }

  void checkNumericBinaryOperatorUnchecked(const BinaryOperation &operation,
                                           const TypedValue &left,
                                           const Type &left_type,
                                           const TypedValue &right,
                                           const Type &right_type) {
    unique_ptr<UncheckedBinaryOperator> binary_operator(
        operation.makeUncheckedBinaryOperatorForTypes(left_type,
                                                      right_type));

    if (left.isNull() || right.isNull()) {
      EXPECT_TRUE(binary_operator->applyToTypedValues(left, right).isNull());
      EXPECT_TRUE(binary_operator->applyToDataPtrs(
          left.isNull() ? nullptr : left.getDataPtr(),
          right.isNull() ? nullptr : right.getDataPtr()).isNull());
      return;
    }

    const Type *expected_type =
        operation.resultTypeForArgumentTypes(left_type, right_type);
    ASSERT_NE(expected_type, nullptr);

    switch (expected_type->getTypeID()) {
      case kInt:
        checkNumericBinaryOperatorUncheckedHelper<int>(operation, *binary_operator, left, right);
        break;
      case kLong:
        checkNumericBinaryOperatorUncheckedHelper<int64_t>(operation, *binary_operator, left, right);
        break;
      case kFloat:
        checkNumericBinaryOperatorUncheckedHelper<float>(operation, *binary_operator, left, right);
        break;
      case kDouble:
        checkNumericBinaryOperatorUncheckedHelper<double>(operation, *binary_operator, left, right);
        break;
      default:
        FATAL_ERROR("Unsupported result types from two operands.");
    }
  }

  template <typename NumericType>
  inline void checkNumericBinaryOperatorUncheckedHelper(const BinaryOperation &operation,
                                                        const UncheckedBinaryOperator &binary_operator,
                                                        const TypedValue &left,
                                                        const TypedValue &right) {
    NumericType expected = checkNumericBinaryOperationHelper<NumericType>(operation, left, right);
    EXPECT_EQ(expected, binary_operator.applyToTypedValues(left, right).getLiteral<NumericType>());
    EXPECT_EQ(expected,
              binary_operator.applyToDataPtrs(left.getDataPtr(),
                                              right.getDataPtr()).getLiteral<NumericType>());
  }

  template <typename NumericType>
  inline NumericType checkNumericBinaryOperationHelper(const BinaryOperation &operation,
                                                       const TypedValue &left,
                                                       const TypedValue &right) {
    const NumericType left_num = GetTypedValueAsNumeric<NumericType>(left);
    const NumericType right_num = GetTypedValueAsNumeric<NumericType>(right);
    switch (operation.getBinaryOperationID()) {
      case BinaryOperationID::kAdd:
        return left_num + right_num;
      case BinaryOperationID::kSubtract:
        return left_num - right_num;
      case BinaryOperationID::kMultiply:
        return left_num * right_num;
      case BinaryOperationID::kDivide:
        CHECK_NE(right_num, static_cast<NumericType>(0));
        return left_num / right_num;
      default:
        FATAL_ERROR("Unsupported numeric binary operation.");
    }
  }

  inline TypedValue checkDateAddBinaryOperationHelper(const BinaryOperation &operation,
                                                      const TypedValue &left,
                                                      const Type &left_type,
                                                      const TypedValue &right,
                                                      const Type &right_type) {
    if (left.isNull() || right.isNull()) {
      return operation.resultTypeForArgumentTypes(left_type, right_type)->makeNullValue();
    }

    switch (left_type.getTypeID()) {
      case kDatetime:
        switch (right_type.getTypeID()) {
          case kDatetimeInterval:
            return TypedValue(left.getLiteral<DatetimeLit>() + right.getLiteral<DatetimeIntervalLit>());
          case kYearMonthInterval:
            return TypedValue(left.getLiteral<DatetimeLit>() + right.getLiteral<YearMonthIntervalLit>());
          default:
            break;
        }
        break;
      case kDatetimeInterval:
        switch (right_type.getTypeID()) {
          case kDatetime:
            return TypedValue(left.getLiteral<DatetimeIntervalLit>() + right.getLiteral<DatetimeLit>());
          case kDatetimeInterval:
            return TypedValue(left.getLiteral<DatetimeIntervalLit>() + right.getLiteral<DatetimeIntervalLit>());
          default:
            break;
        }
        break;
      case kYearMonthInterval:
        switch (right_type.getTypeID()) {
          case kDatetime:
            return TypedValue(left.getLiteral<YearMonthIntervalLit>() + right.getLiteral<DatetimeLit>());
          case kYearMonthInterval:
            return TypedValue(left.getLiteral<YearMonthIntervalLit>() + right.getLiteral<YearMonthIntervalLit>());
          default:
            break;
        }
        break;
      default:
        break;
    }

    FATAL_ERROR("Unsupported add between two operands.");
  }

  inline TypedValue checkDateSubtractBinaryOperationHelper(const BinaryOperation &operation,
                                                           const TypedValue &left,
                                                           const Type &left_type,
                                                           const TypedValue &right,
                                                           const Type &right_type) {
    if (left.isNull() || right.isNull()) {
      return operation.resultTypeForArgumentTypes(left_type, right_type)->makeNullValue();
    }

    switch (left_type.getTypeID()) {
      case kDatetime:
        switch (right_type.getTypeID()) {
          case kDatetime:
            return TypedValue(left.getLiteral<DatetimeLit>() - right.getLiteral<DatetimeLit>());
          case kDatetimeInterval:
            return TypedValue(left.getLiteral<DatetimeLit>() - right.getLiteral<DatetimeIntervalLit>());
          case kYearMonthInterval:
            return TypedValue(left.getLiteral<DatetimeLit>() - right.getLiteral<YearMonthIntervalLit>());
          default:
            break;
        }
        break;
      case kDatetimeInterval:
        switch (right_type.getTypeID()) {
          case kDatetimeInterval:
            return TypedValue(left.getLiteral<DatetimeIntervalLit>() - right.getLiteral<DatetimeIntervalLit>());
          default:
            break;
        }
        break;
      case kYearMonthInterval:
        switch (right_type.getTypeID()) {
          case kYearMonthInterval:
            return TypedValue(left.getLiteral<YearMonthIntervalLit>() - right.getLiteral<YearMonthIntervalLit>());
          default:
            break;
        }
        break;
      default:
        break;
    }

    FATAL_ERROR("Unsupported subtraction between two operands.");
  }

  inline TypedValue checkDateMultiplyBinaryOperationHelper(const BinaryOperation &operation,
                                                           const TypedValue &left,
                                                           const Type &left_type,
                                                           const TypedValue &right,
                                                           const Type &right_type) {
    if (left.isNull() || right.isNull()) {
      return operation.resultTypeForArgumentTypes(left_type, right_type)->makeNullValue();
    }

    switch (left_type.getTypeID()) {
      case kInt:
        switch (right_type.getTypeID()) {
          case kDatetimeInterval:
            return TypedValue(right.getLiteral<DatetimeIntervalLit>() * left.getLiteral<int>());
          case kYearMonthInterval:
            return TypedValue(right.getLiteral<YearMonthIntervalLit>() * left.getLiteral<int>());
          default:
            break;
        }
        break;
      case kLong:
        switch (right_type.getTypeID()) {
          case kDatetimeInterval:
            return TypedValue(right.getLiteral<DatetimeIntervalLit>() * left.getLiteral<int64_t>());
          case kYearMonthInterval:
            return TypedValue(right.getLiteral<YearMonthIntervalLit>() * left.getLiteral<int64_t>());
          default:
            break;
        }
        break;
      case kFloat:
        switch (right_type.getTypeID()) {
          case kDatetimeInterval:
            return TypedValue(right.getLiteral<DatetimeIntervalLit>() * left.getLiteral<float>());
          case kYearMonthInterval:
            return TypedValue(right.getLiteral<YearMonthIntervalLit>() * left.getLiteral<float>());
          default:
            break;
        }
        break;
      case kDouble:
        switch (right_type.getTypeID()) {
          case kDatetimeInterval:
            return TypedValue(right.getLiteral<DatetimeIntervalLit>() * left.getLiteral<double>());
          case kYearMonthInterval:
            return TypedValue(right.getLiteral<YearMonthIntervalLit>() * left.getLiteral<double>());
          default:
            break;
        }
        break;
      case kDatetimeInterval:
        switch (right_type.getTypeID()) {
          case kInt:
            return TypedValue(left.getLiteral<DatetimeIntervalLit>() * right.getLiteral<int>());
          case kLong:
            return TypedValue(left.getLiteral<DatetimeIntervalLit>() * right.getLiteral<int64_t>());
          case kFloat:
            return TypedValue(left.getLiteral<DatetimeIntervalLit>() * right.getLiteral<float>());
          case kDouble:
            return TypedValue(left.getLiteral<DatetimeIntervalLit>() * right.getLiteral<double>());
          default:
            break;
        }
        break;
      case kYearMonthInterval:
        switch (right_type.getTypeID()) {
          case kInt:
            return TypedValue(left.getLiteral<YearMonthIntervalLit>() * right.getLiteral<int>());
          case kLong:
            return TypedValue(left.getLiteral<YearMonthIntervalLit>() * right.getLiteral<int64_t>());
          case kFloat:
            return TypedValue(left.getLiteral<YearMonthIntervalLit>() * right.getLiteral<float>());
          case kDouble:
            return TypedValue(left.getLiteral<YearMonthIntervalLit>() * right.getLiteral<double>());
          default:
            break;
        }
        break;
      default:
        break;
    }

    FATAL_ERROR("Unsupported multiplication between two operands.");
  }

  inline TypedValue checkDateDivideBinaryOperationHelper(const BinaryOperation &operation,
                                                         const TypedValue &left,
                                                         const Type &left_type,
                                                         const TypedValue &right,
                                                         const Type &right_type) {
    if (left.isNull() || right.isNull()) {
      return operation.resultTypeForArgumentTypes(left_type, right_type)->makeNullValue();
    }

    switch (left_type.getTypeID()) {
      case kDatetimeInterval:
        switch (right_type.getTypeID()) {
          case kInt:
            return TypedValue(left.getLiteral<DatetimeIntervalLit>() / right.getLiteral<int>());
          case kLong:
            return TypedValue(left.getLiteral<DatetimeIntervalLit>() / right.getLiteral<int64_t>());
          case kFloat:
            return TypedValue(left.getLiteral<DatetimeIntervalLit>() / right.getLiteral<float>());
          case kDouble:
            return TypedValue(left.getLiteral<DatetimeIntervalLit>() / right.getLiteral<double>());
          default:
            break;
        }
        break;
      case kYearMonthInterval:
        switch (right_type.getTypeID()) {
          case kInt:
            return TypedValue(left.getLiteral<YearMonthIntervalLit>() / right.getLiteral<int>());
          case kLong:
            return TypedValue(left.getLiteral<YearMonthIntervalLit>() / right.getLiteral<int64_t>());
          case kFloat:
            return TypedValue(left.getLiteral<YearMonthIntervalLit>() / right.getLiteral<float>());
          case kDouble:
            return TypedValue(left.getLiteral<YearMonthIntervalLit>() / right.getLiteral<double>());
          default:
            break;
        }
        break;
      default:
        break;
    }

    FATAL_ERROR("Unsupported division between two operands.");
  }

  template <typename DateType>
  void checkDateBinaryOperationChecked(const BinaryOperation &operation,
                                       const TypedValue &left,
                                       const Type &left_type,
                                       const TypedValue &right,
                                       const Type &right_type,
                                       const TypedValue &expected) {
    if (left.isNull() || right.isNull()) {
      EXPECT_TRUE(operation.applyToChecked(left, left_type, right, right_type).isNull());
      return;
    }

    TypedValue checked(operation.applyToChecked(left, left_type, right, right_type));
    EXPECT_EQ(expected.getLiteral<DateType>(),
              checked.getLiteral<DateType>());
  }

  template <typename DateType>
  void checkDateBinaryOperatorUnchecked(const BinaryOperation &operation,
                                        const TypedValue &left,
                                        const Type &left_type,
                                        const TypedValue &right,
                                        const Type &right_type,
                                        const TypedValue &expected) {
    unique_ptr<UncheckedBinaryOperator> binary_operator(
        operation.makeUncheckedBinaryOperatorForTypes(left_type, right_type));

    if (left.isNull() || right.isNull()) {
      EXPECT_TRUE(binary_operator->applyToTypedValues(left, right).isNull());
      EXPECT_TRUE(binary_operator->applyToDataPtrs(
          left.isNull() ? nullptr : left.getDataPtr(),
          right.isNull() ? nullptr : right.getDataPtr()).isNull());
      return;
    }

    TypedValue checked(binary_operator->applyToTypedValues(left, right));
    EXPECT_EQ(expected.getLiteral<DateType>(),
              checked.getLiteral<DateType>());

    TypedValue checked_from_date_ptr(binary_operator->applyToDataPtrs(left.getDataPtr(),
                                                                      right.getDataPtr()));
    EXPECT_EQ(expected.getLiteral<DateType>(),
              checked_from_date_ptr.getLiteral<DateType>());
  }

  unique_ptr<TypedValue>
      int_null_, int_zero_, int_positive_, int_negative_, int_max_, int_min_,
      long_null_, long_zero_, long_positive_, long_negative_, long_max_, long_min_,
      float_null_, float_zero_, float_positive_, float_negative_, float_max_, float_min_,
      double_null_, double_zero_, double_positive_, double_negative_, double_max_, double_min_,
      datetime_null_, datetime_zero_, datetime_positive_, datetime_negative_, datetime_max_, datetime_min_,
      datetime_interval_null_, datetime_interval_zero_, datetime_interval_positive_, datetime_interval_negative_,
      year_month_interval_null_, year_month_interval_zero_,
      year_month_interval_positive_, year_month_interval_negative_;

  vector<pair<const TypedValue*, const Type*>>
      numeric_typed_values_, datetime_typed_values_,
      datetime_interval_typed_values_, year_month_interval_typed_values_;
};

TEST_F(BinaryOperationTest, AddBinaryOperationTest) {
  checkNumericBinaryOperation(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd));
  checkDateAddBinaryOperators();
  checkDatetimeAddBinaryOperation();
  checkDatetimeIntervalAddBinaryOperation();
  checkYearMonthIntervalAddBinaryOperation();
}

TEST_F(BinaryOperationTest, SubtractBinaryOperationTest) {
  checkNumericBinaryOperation(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kSubtract));
  checkDateSubtractBinaryOperators();
  checkDatetimeSubtractBinaryOperation();
  checkDatetimeIntervalSubtractBinaryOperation();
  checkYearMonthIntervalSubtractBinaryOperation();
}

TEST_F(BinaryOperationTest, MultiplyBinaryOperationTest) {
  checkNumericBinaryOperation(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kMultiply));
  checkDateMultiplyBinaryOperators();
  // NOTE(zuyu): No multiplication operations for the Datetime type.
  checkDatetimeIntervalMultiplyBinaryOperation();
  checkYearMonthIntervalMultiplyBinaryOperation();
}

TEST_F(BinaryOperationTest, DivideBinaryOperationTest) {
  checkNumericBinaryOperation(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kDivide));
  checkDateDivideBinaryOperators();
  // NOTE(zuyu): No division operations for the Datetime type.
  checkDatetimeIntervalDivideBinaryOperation();
  checkYearMonthIntervalDivideBinaryOperation();
}


void CheckBinaryOperationSerialization(const BinaryOperation &operation) {
  serialization::BinaryOperation proto = operation.getProto();
  switch (operation.getBinaryOperationID()) {
    case BinaryOperationID::kAdd:
      EXPECT_EQ(serialization::BinaryOperation::ADD, proto.operation_id());
      break;
    case BinaryOperationID::kSubtract:
      EXPECT_EQ(serialization::BinaryOperation::SUBTRACT, proto.operation_id());
      break;
    case BinaryOperationID::kMultiply:
      EXPECT_EQ(serialization::BinaryOperation::MULTIPLY, proto.operation_id());
      break;
    case BinaryOperationID::kDivide:
      EXPECT_EQ(serialization::BinaryOperation::DIVIDE, proto.operation_id());
      break;
    case BinaryOperationID::kModulo:
      EXPECT_EQ(serialization::BinaryOperation::MODULO, proto.operation_id());
      break;
    default:
      FATAL_ERROR("operation is an unknown BinaryOperation in CheckBinaryOperationSerialization");
  }

  EXPECT_TRUE(operation.equals(BinaryOperationFactory::ReconstructFromProto(proto)));
}

TEST(BinaryOperationSerializationTest, BinaryOperationProtoSerializationTest) {
  CheckBinaryOperationSerialization(BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd));

  CheckBinaryOperationSerialization(BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kSubtract));

  CheckBinaryOperationSerialization(BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kMultiply));

  CheckBinaryOperationSerialization(BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kDivide));
}

}  // namespace quickstep
