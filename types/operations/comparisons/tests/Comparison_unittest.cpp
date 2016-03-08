/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
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

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "types/DatetimeLit.hpp"
#include "types/IntervalLit.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/Operation.pb.h"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/Macros.hpp"
#include "utility/ScopedBuffer.hpp"

using std::int64_t;
using std::min;
using std::numeric_limits;
using std::pair;
using std::string;
using std::strlen;
using std::unique_ptr;
using std::vector;

namespace quickstep {

namespace {

static const char kSampleStringShort[] = "foo";

static const char kSampleStringLong[] =
    "Space is big. You just won't believe how vastly, hugely, mind-bogglingly "
    "big it is. I mean, you may think it's a long way down the road to the "
    "chemist's, but that's just peanuts to space.";

}  // namespace

class ComparisonTest : public ::testing::Test {
 protected:
  typedef vector<const TypedValue *>::const_iterator const_iterator;

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

    char_null_.reset(new TypedValue(kChar));
    varchar_null_.reset(new TypedValue(kVarChar));

    string_typed_values_.emplace_back(char_null_.get(),
                                      &TypeFactory::GetType(kChar, 10, true));
    string_typed_values_.emplace_back(varchar_null_.get(),
                                      &TypeFactory::GetType(kVarChar, 10, true));

    const std::size_t string_short_length = strlen(kSampleStringShort);
    char_ref_short_.reset(new TypedValue(kChar, kSampleStringShort, string_short_length));
    char_lit_short_.reset(new TypedValue(kChar, kSampleStringShort, string_short_length));
    char_lit_short_->ensureNotReference();

    char_ref_null_terminated_short_.reset(new TypedValue(kChar, kSampleStringShort, string_short_length + 1));
    char_lit_null_terminated_short_.reset(new TypedValue(kChar, kSampleStringShort, string_short_length + 1));
    char_lit_null_terminated_short_->ensureNotReference();

    extended_short_char_buffer_.reset(new ScopedBuffer(strlen(kSampleStringShort) + 5));
    std::memset(extended_short_char_buffer_->get(), 0, string_short_length + 5);
    std::memcpy(extended_short_char_buffer_->get(), kSampleStringShort, string_short_length);

    char_ref_extended_short_.reset(new TypedValue(kChar, extended_short_char_buffer_->get(), string_short_length + 5));

    char_lit_extended_short_.reset(new TypedValue(kChar, extended_short_char_buffer_->get(), string_short_length + 5));
    char_lit_extended_short_->ensureNotReference();

    varchar_ref_short_.reset(new TypedValue(kVarChar, kSampleStringShort, string_short_length + 1));
    varchar_lit_short_.reset(new TypedValue(kVarChar, kSampleStringShort, string_short_length + 1));
    varchar_lit_short_->ensureNotReference();

    string_typed_values_.emplace_back(
        char_ref_short_.get(),
        &TypeFactory::GetType(kChar,
                              char_ref_short_->getAsciiStringLength(),
                              false));
    string_typed_values_.emplace_back(
        char_lit_short_.get(),
        &TypeFactory::GetType(kChar,
                              char_lit_short_->getAsciiStringLength(),
                              false));
    string_typed_values_.emplace_back(
        char_ref_null_terminated_short_.get(),
        &TypeFactory::GetType(kChar,
                              char_ref_null_terminated_short_->getAsciiStringLength(),
                              false));
    string_typed_values_.emplace_back(
        char_lit_null_terminated_short_.get(),
        &TypeFactory::GetType(kChar,
                              char_lit_null_terminated_short_->getAsciiStringLength(),
                              false));
    string_typed_values_.emplace_back(
        char_ref_extended_short_.get(),
        &TypeFactory::GetType(kChar,
                              char_ref_extended_short_->getAsciiStringLength(),
                              false));
    string_typed_values_.emplace_back(
        char_lit_extended_short_.get(),
        &TypeFactory::GetType(kChar,
                              char_lit_extended_short_->getAsciiStringLength(),
                              false));
    string_typed_values_.emplace_back(
        varchar_ref_short_.get(),
        &TypeFactory::GetType(kVarChar,
                              varchar_ref_short_->getAsciiStringLength(),
                              false));
    string_typed_values_.emplace_back(
        varchar_lit_short_.get(),
        &TypeFactory::GetType(kVarChar,
                              varchar_lit_short_->getAsciiStringLength(),
                              false));

    const std::size_t string_long_length = strlen(kSampleStringLong);
    char_ref_long_.reset(new TypedValue(kChar, kSampleStringLong, string_long_length));
    char_lit_long_.reset(new TypedValue(kChar, kSampleStringLong, string_long_length));
    char_lit_long_->ensureNotReference();

    char_ref_null_terminated_long_.reset(new TypedValue(kChar, kSampleStringLong, string_long_length + 1));
    char_lit_null_terminated_long_.reset(new TypedValue(kChar, kSampleStringLong, string_long_length + 1));
    char_lit_null_terminated_long_->ensureNotReference();

    extended_long_char_buffer_.reset(new ScopedBuffer(strlen(kSampleStringLong) + 5));
    std::memset(extended_long_char_buffer_->get(), 0, string_long_length + 5);
    std::memcpy(extended_long_char_buffer_->get(), kSampleStringLong, string_long_length);

    char_ref_extended_long_.reset(new TypedValue(kChar, extended_long_char_buffer_->get(), string_long_length + 5));

    char_lit_extended_long_.reset(new TypedValue(kChar, extended_long_char_buffer_->get(), string_long_length + 5));
    char_lit_extended_long_->ensureNotReference();

    varchar_ref_long_.reset(new TypedValue(kVarChar, kSampleStringLong, string_long_length + 1));
    varchar_lit_long_.reset(new TypedValue(kVarChar, kSampleStringLong, string_long_length + 1));
    varchar_lit_long_->ensureNotReference();

    string_typed_values_.emplace_back(
        char_ref_long_.get(),
        &TypeFactory::GetType(kChar,
                              char_ref_long_->getAsciiStringLength(),
                              false));
    string_typed_values_.emplace_back(
        char_lit_long_.get(),
        &TypeFactory::GetType(kChar,
                              char_lit_long_->getAsciiStringLength(),
                              false));
    string_typed_values_.emplace_back(
        char_ref_null_terminated_long_.get(),
        &TypeFactory::GetType(kChar,
                              char_ref_null_terminated_long_->getAsciiStringLength(),
                              false));
    string_typed_values_.emplace_back(
        char_lit_null_terminated_long_.get(),
        &TypeFactory::GetType(kChar,
                              char_lit_null_terminated_long_->getAsciiStringLength(),
                              false));
    string_typed_values_.emplace_back(
        char_ref_extended_long_.get(),
        &TypeFactory::GetType(kChar,
                              char_ref_extended_long_->getAsciiStringLength(),
                              false));
    string_typed_values_.emplace_back(
        char_lit_extended_long_.get(),
        &TypeFactory::GetType(kChar,
                              char_lit_extended_long_->getAsciiStringLength(),
                              false));
    string_typed_values_.emplace_back(
        varchar_ref_long_.get(),
        &TypeFactory::GetType(kVarChar,
                              varchar_ref_long_->getAsciiStringLength(),
                              false));
    string_typed_values_.emplace_back(
        varchar_lit_long_.get(),
        &TypeFactory::GetType(kVarChar,
                              varchar_lit_long_->getAsciiStringLength(),
                              false));
  }

  void checkComparison(const Comparison &comparison) {
    checkNumericComparison(comparison);
    checkStringComparison(comparison);
    checkDatetimeComparison(comparison);
    checkDatetimeIntervalComparison(comparison);
    checkYearMonthIntervalComparison(comparison);
  }

 private:
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

  void checkNumericComparison(const Comparison &comparison) {
    for (const std::pair<const TypedValue*, const Type*> &left_item : numeric_typed_values_) {
      for (const std::pair<const TypedValue*, const Type*> &right_item : numeric_typed_values_) {
        checkNumericComparisonChecked(comparison,
                                      *left_item.first, *left_item.second,
                                      *right_item.first, *right_item.second);
        checkNumericComparisonUnchecked(comparison,
                                        *left_item.first, *left_item.second,
                                        *right_item.first, *right_item.second);
      }
    }
  }

  void checkNumericComparisonChecked(const Comparison &comparison,
                                     const TypedValue &left,
                                     const Type &left_type,
                                     const TypedValue &right,
                                     const Type &right_type) {
    if (left.isNull() || right.isNull()) {
      EXPECT_FALSE(comparison.compareTypedValuesChecked(left, left_type,
                                                        right, right_type));
      return;
    }

    const Type *expected_type = TypeFactory::GetUnifyingType(left_type,
                                                             right_type);
    ASSERT_NE(expected_type, nullptr);

    switch (expected_type->getTypeID()) {
      case kInt:
        checkNumericComparisonCheckedHelper<int>(comparison,
                                                 left, left_type,
                                                 right, right_type);
        break;
      case kLong:
        checkNumericComparisonCheckedHelper<int64_t>(comparison,
                                                     left, left_type,
                                                     right, right_type);
        break;
      case kFloat:
        checkNumericComparisonCheckedHelper<float>(comparison,
                                                   left, left_type,
                                                   right, right_type);
        break;
      case kDouble:
        checkNumericComparisonCheckedHelper<double>(comparison,
                                                    left, left_type,
                                                    right, right_type);
        break;
      default:
        FATAL_ERROR("Unsupported result type for the comparison.");
    }
  }

  template <typename NumericType>
  inline void checkNumericComparisonCheckedHelper(const Comparison &comparison,
                                                  const TypedValue &left,
                                                  const Type &left_type,
                                                  const TypedValue &right,
                                                  const Type &right_type) {
    if (left.isNull() || right.isNull()) {
      EXPECT_FALSE(comparison.compareTypedValuesChecked(left, left_type,
                                                        right, right_type));
      return;
    }

    bool expected = checkNumericComparisonHelper<NumericType>(comparison, left, right);
    EXPECT_EQ(expected, comparison.compareTypedValuesChecked(left, left_type,
                                                             right, right_type));
  }

  void checkNumericComparisonUnchecked(const Comparison &comparison,
                                       const TypedValue &left,
                                       const Type &left_type,
                                       const TypedValue &right,
                                       const Type &right_type) {
    unique_ptr<UncheckedComparator> comparator(
        comparison.makeUncheckedComparatorForTypes(left_type,
                                                   right_type));

    if (left.isNull() || right.isNull()) {
      EXPECT_FALSE(comparator->compareTypedValues(left, right));
      EXPECT_FALSE(comparator->compareDataPtrs(
          left.isNull() ? nullptr : left.getDataPtr(),
          right.isNull() ? nullptr : right.getDataPtr()));
      return;
    }

    const Type *expected_type = TypeFactory::GetUnifyingType(left_type,
                                                             right_type);
    ASSERT_NE(expected_type, nullptr);

    switch (expected_type->getTypeID()) {
      case kInt:
        checkNumericComparisonUncheckedHelper<int>(comparison, *comparator,
                                                   left, left_type,
                                                   right, right_type);
        break;
      case kLong:
        checkNumericComparisonUncheckedHelper<int64_t>(comparison, *comparator,
                                                       left, left_type,
                                                       right, right_type);
        break;
      case kFloat:
        checkNumericComparisonUncheckedHelper<float>(comparison, *comparator,
                                                     left, left_type,
                                                     right, right_type);
        break;
      case kDouble:
        checkNumericComparisonUncheckedHelper<double>(comparison, *comparator,
                                                      left, left_type,
                                                      right, right_type);
        break;
      default:
        FATAL_ERROR("Unsupported result types for the comparison.");
    }
  }

  template <typename NumericType>
  inline void checkNumericComparisonUncheckedHelper(const Comparison &comparison,
                                                    const UncheckedComparator &comparator,
                                                    const TypedValue &left,
                                                    const Type &left_type,
                                                    const TypedValue &right,
                                                    const Type &right_type) {
    bool expected = checkNumericComparisonHelper<NumericType>(comparison, left, right);
    EXPECT_EQ(expected, comparator.compareTypedValues(left, right));
    EXPECT_EQ(expected, comparator.compareDataPtrs(left.getDataPtr(), right.getDataPtr()));
  }

  template <typename NumericType>
  inline bool checkNumericComparisonHelper(const Comparison &comparison,
                                           const TypedValue &left,
                                           const TypedValue &right) {
    const NumericType left_num = GetTypedValueAsNumeric<NumericType>(left);
    const NumericType right_num = GetTypedValueAsNumeric<NumericType>(right);

    switch (comparison.getComparisonID()) {
      case ComparisonID::kEqual:
        return left_num == right_num;
      case ComparisonID::kNotEqual:
        return left_num != right_num;
      case ComparisonID::kLess:
        return left_num < right_num;
      case ComparisonID::kLessOrEqual:
        return left_num <= right_num;
      case ComparisonID::kGreater:
        return left_num > right_num;
      case ComparisonID::kGreaterOrEqual:
        return left_num >= right_num;
      default:
        FATAL_ERROR("Unsupported comparison type.");
    }
  }

  void checkDatetimeComparison(const Comparison &comparison) {
    for (const std::pair<const TypedValue*, const Type*> &left_item : datetime_typed_values_) {
      for (const std::pair<const TypedValue*, const Type*> &right_item : datetime_typed_values_) {
        checkDateComparisonChecked<DatetimeLit>(comparison,
                                                *left_item.first, *left_item.second,
                                                *right_item.first, *right_item.second);
        checkDateComparisonUnchecked<DatetimeLit>(comparison,
                                                  *left_item.first, *left_item.second,
                                                  *right_item.first, *right_item.second);
      }
    }
  }

  void checkDatetimeIntervalComparison(const Comparison &comparison) {
    for (const std::pair<const TypedValue*, const Type*> &left_item
         : datetime_interval_typed_values_) {
      for (const std::pair<const TypedValue*, const Type*> &right_item
           : datetime_interval_typed_values_) {
        checkDateComparisonChecked<DatetimeIntervalLit>(comparison,
                                                        *left_item.first, *left_item.second,
                                                        *right_item.first, *right_item.second);
        checkDateComparisonUnchecked<DatetimeIntervalLit>(comparison,
                                                          *left_item.first, *left_item.second,
                                                          *right_item.first, *right_item.second);
      }
    }
  }

  void checkYearMonthIntervalComparison(const Comparison &comparison) {
    for (const std::pair<const TypedValue*, const Type*> &left_item
         : year_month_interval_typed_values_) {
      for (const std::pair<const TypedValue*, const Type*> &right_item
           : year_month_interval_typed_values_) {
        checkDateComparisonChecked<YearMonthIntervalLit>(comparison,
                                                         *left_item.first, *left_item.second,
                                                         *right_item.first, *right_item.second);
        checkDateComparisonUnchecked<YearMonthIntervalLit>(comparison,
                                                           *left_item.first, *left_item.second,
                                                           *right_item.first, *right_item.second);
      }
    }
  }

  template <typename DateType>
  void checkDateComparisonChecked(const Comparison &comparison,
                                  const TypedValue &left,
                                  const Type &left_type,
                                  const TypedValue &right,
                                  const Type &right_type) {
    if (left.isNull() || right.isNull()) {
      EXPECT_FALSE(comparison.compareTypedValuesChecked(left, left_type,
                                                        right, right_type));
      return;
    }

    bool expected = checkDateComparisonHelper<DateType>(comparison, left, right);
    EXPECT_EQ(expected, comparison.compareTypedValuesChecked(left, left_type,
                                                             right, right_type));
  }

  template <typename DateType>
  void checkDateComparisonUnchecked(const Comparison &comparison,
                                    const TypedValue &left,
                                    const Type &left_type,
                                    const TypedValue &right,
                                    const Type &right_type) {
    unique_ptr<UncheckedComparator> comparator(
        comparison.makeUncheckedComparatorForTypes(left_type,
                                                   right_type));

    if (left.isNull() || right.isNull()) {
      EXPECT_FALSE(comparator->compareTypedValues(left, right));
      EXPECT_FALSE(comparator->compareDataPtrs(
          left.isNull() ? nullptr : left.getDataPtr(),
          right.isNull() ? nullptr : right.getDataPtr()));
      return;
    }

    bool expected = checkDateComparisonHelper<DateType>(comparison, left, right);
    EXPECT_EQ(expected, comparator->compareTypedValues(left, right));
    EXPECT_EQ(expected, comparator->compareDataPtrs(left.getDataPtr(), right.getDataPtr()));
  }

  template <typename DateType>
  inline bool checkDateComparisonHelper(const Comparison &comparison,
                                        const TypedValue &left,
                                        const TypedValue &right) {
    switch (comparison.getComparisonID()) {
      case ComparisonID::kEqual:
        return left.getLiteral<DateType>() == right.getLiteral<DateType>();
      case ComparisonID::kNotEqual:
        return left.getLiteral<DateType>() != right.getLiteral<DateType>();
      case ComparisonID::kLess:
        return left.getLiteral<DateType>() < right.getLiteral<DateType>();
      case ComparisonID::kLessOrEqual:
        return left.getLiteral<DateType>() <= right.getLiteral<DateType>();
      case ComparisonID::kGreater:
        return left.getLiteral<DateType>() > right.getLiteral<DateType>();
      case ComparisonID::kGreaterOrEqual:
        return left.getLiteral<DateType>() >= right.getLiteral<DateType>();
      default:
        FATAL_ERROR("Unsupported comparison type.");
    }
  }

  void checkStringComparison(const Comparison &comparison) {
    for (const std::pair<const TypedValue*, const Type*> &left_item : string_typed_values_) {
      for (const std::pair<const TypedValue*, const Type*> &right_item : string_typed_values_) {
        checkStringComparisonChecked(comparison,
                                     *left_item.first, *left_item.second,
                                     *right_item.first, *right_item.second);
        checkStringComparisonUnchecked(comparison,
                                       *left_item.first, *left_item.second,
                                       *right_item.first, *right_item.second);
      }
    }
  }

  void checkStringComparisonChecked(const Comparison &comparison,
                                    const TypedValue &left,
                                    const Type &left_type,
                                    const TypedValue &right,
                                    const Type &right_type) {
    if (left.isNull() || right.isNull()) {
      EXPECT_FALSE(comparison.compareTypedValuesChecked(left, left_type,
                                                        right, right_type));
      return;
    }

    bool expected = checkStringComparisonHelper(comparison, left, right);
    EXPECT_EQ(expected, comparison.compareTypedValuesChecked(left, left_type,
                                                             right, right_type));
  }

  void checkStringComparisonUnchecked(const Comparison &comparison,
                                      const TypedValue &left,
                                      const Type &left_type,
                                      const TypedValue &right,
                                      const Type &right_type) {
    unique_ptr<UncheckedComparator> comparator(
        comparison.makeUncheckedComparatorForTypes(left_type,
                                                   right_type));

    if (left.isNull() || right.isNull()) {
      EXPECT_FALSE(comparator->compareTypedValues(left, right));
      EXPECT_FALSE(comparator->compareDataPtrs(
          left.isNull() ? nullptr : left.getDataPtr(),
          right.isNull() ? nullptr : right.getDataPtr()));
      return;
    }

    bool expected = checkStringComparisonHelper(comparison, left, right);
    EXPECT_EQ(expected, comparator->compareTypedValues(left, right));
    EXPECT_EQ(expected, comparator->compareDataPtrs(left.getDataPtr(), right.getDataPtr()));
  }

  inline bool checkStringComparisonHelper(const Comparison &comparison,
                                          const TypedValue &left,
                                          const TypedValue &right) {
    const std::size_t left_length = left.getAsciiStringLength();
    const std::size_t right_length = right.getAsciiStringLength();
    const int compare_result =
        strncmp(static_cast<const char*>(left.getOutOfLineData()),
                static_cast<const char*>(right.getOutOfLineData()),
                min(left_length, right_length));

    switch (comparison.getComparisonID()) {
      case ComparisonID::kEqual:
        return compare_result == 0 && left_length == right_length;
      case ComparisonID::kNotEqual:
        return !(compare_result == 0 && left_length == right_length);
      case ComparisonID::kLess:
        return compare_result < 0 || (compare_result == 0 && left_length < right_length);
      case ComparisonID::kLessOrEqual:
        return compare_result < 0 || (compare_result == 0 && left_length <= right_length);
      case ComparisonID::kGreater:
        return compare_result > 0 || (compare_result == 0 && left_length > right_length);
      case ComparisonID::kGreaterOrEqual:
        return compare_result > 0 || (compare_result == 0 && left_length >= right_length);
      default:
        FATAL_ERROR("Unsupported comparison type.");
    }
  }

  unique_ptr<TypedValue>
      int_null_, int_zero_, int_positive_, int_negative_, int_max_, int_min_,
      long_null_, long_zero_, long_positive_, long_negative_, long_max_, long_min_,
      float_null_, float_zero_, float_positive_, float_negative_, float_max_, float_min_,
      double_null_, double_zero_, double_positive_, double_negative_, double_max_, double_min_,
      char_null_, char_ref_short_, char_lit_short_, char_ref_long_, char_lit_long_,
      char_ref_null_terminated_short_, char_lit_null_terminated_short_,
      char_ref_null_terminated_long_, char_lit_null_terminated_long_,
      char_ref_extended_short_, char_lit_extended_short_,
      char_ref_extended_long_, char_lit_extended_long_,
      varchar_null_, varchar_ref_short_, varchar_lit_short_, varchar_ref_long_, varchar_lit_long_,
      datetime_null_, datetime_zero_, datetime_positive_, datetime_negative_, datetime_max_, datetime_min_,
      datetime_interval_null_, datetime_interval_zero_, datetime_interval_positive_, datetime_interval_negative_,
      year_month_interval_null_, year_month_interval_zero_,
      year_month_interval_positive_, year_month_interval_negative_;

  unique_ptr<ScopedBuffer> extended_short_char_buffer_, extended_long_char_buffer_;

  vector<pair<const TypedValue*, const Type*>>
      numeric_typed_values_, datetime_typed_values_, datetime_interval_typed_values_,
      year_month_interval_typed_values_, string_typed_values_;
};

TEST_F(ComparisonTest, ComparisonTests) {
  checkComparison(ComparisonFactory::GetComparison(ComparisonID::kEqual));
  checkComparison(ComparisonFactory::GetComparison(ComparisonID::kNotEqual));
  checkComparison(ComparisonFactory::GetComparison(ComparisonID::kLess));
  checkComparison(ComparisonFactory::GetComparison(ComparisonID::kLessOrEqual));
  checkComparison(ComparisonFactory::GetComparison(ComparisonID::kGreater));
  checkComparison(ComparisonFactory::GetComparison(ComparisonID::kGreaterOrEqual));
}

void CheckComparisonSerialization(const Comparison &comparison) {
  serialization::Comparison proto = comparison.getProto();
  switch (comparison.getComparisonID()) {
    case ComparisonID::kEqual:
      EXPECT_EQ(serialization::Comparison::EQUAL, proto.comparison_id());
      break;
    case ComparisonID::kNotEqual:
      EXPECT_EQ(serialization::Comparison::NOT_EQUAL, proto.comparison_id());
      break;
    case ComparisonID::kLess:
      EXPECT_EQ(serialization::Comparison::LESS, proto.comparison_id());
      break;
    case ComparisonID::kLessOrEqual:
      EXPECT_EQ(serialization::Comparison::LESS_OR_EQUAL, proto.comparison_id());
      break;
    case ComparisonID::kGreater:
      EXPECT_EQ(serialization::Comparison::GREATER, proto.comparison_id());
      break;
    case ComparisonID::kGreaterOrEqual:
      EXPECT_EQ(serialization::Comparison::GREATER_OR_EQUAL, proto.comparison_id());
      break;
    case ComparisonID::kLike:
      EXPECT_EQ(serialization::Comparison::LIKE, proto.comparison_id());
      break;
    case ComparisonID::kNotLike:
      EXPECT_EQ(serialization::Comparison::NOT_LIKE, proto.comparison_id());
      break;
    case ComparisonID::kRegexMatch:
      EXPECT_EQ(serialization::Comparison::REGEX_MATCH, proto.comparison_id());
      break;
    case ComparisonID::kNotRegexMatch:
      EXPECT_EQ(serialization::Comparison::NOT_REGEX_MATCH, proto.comparison_id());
      break;

    default:
      FATAL_ERROR("operation is an unknown Comparison in CheckComparisonSerialization");
  }

  EXPECT_TRUE(comparison.equals(ComparisonFactory::ReconstructFromProto(proto)));
}

TEST_F(ComparisonTest, ComparisonProtoSerializationTest) {
  CheckComparisonSerialization(ComparisonFactory::GetComparison(ComparisonID::kEqual));
  CheckComparisonSerialization(ComparisonFactory::GetComparison(ComparisonID::kNotEqual));
  CheckComparisonSerialization(ComparisonFactory::GetComparison(ComparisonID::kLess));
  CheckComparisonSerialization(ComparisonFactory::GetComparison(ComparisonID::kLessOrEqual));
  CheckComparisonSerialization(ComparisonFactory::GetComparison(ComparisonID::kGreater));
  CheckComparisonSerialization(ComparisonFactory::GetComparison(ComparisonID::kGreaterOrEqual));
  CheckComparisonSerialization(ComparisonFactory::GetComparison(ComparisonID::kLike));
  CheckComparisonSerialization(ComparisonFactory::GetComparison(ComparisonID::kNotLike));
  CheckComparisonSerialization(ComparisonFactory::GetComparison(ComparisonID::kRegexMatch));
  CheckComparisonSerialization(ComparisonFactory::GetComparison(ComparisonID::kNotRegexMatch));
}

// The 6 basic comparisons all have the same applicability to types. This
// general-purpose function checks Comparison::canComparePartialTypes() for any
// of them.
void CheckBasicComparisonApplicableToPartialTypes(const Comparison &comparison) {
  // Assume applicable when neither side of the comparison is known.
  EXPECT_TRUE(comparison.canComparePartialTypes(nullptr, nullptr));

  // If only one argument is known, any orderable Type is OK.
  for (const TypeID type_id
       : {kInt, kLong, kFloat, kDouble, kDatetime, kDatetimeInterval, kYearMonthInterval}) {
    EXPECT_TRUE(comparison.canComparePartialTypes(
        &TypeFactory::GetType(type_id, false), nullptr));
    EXPECT_TRUE(comparison.canComparePartialTypes(
        &TypeFactory::GetType(type_id, true), nullptr));
    EXPECT_TRUE(comparison.canComparePartialTypes(
        nullptr, &TypeFactory::GetType(type_id, false)));
    EXPECT_TRUE(comparison.canComparePartialTypes(
        nullptr, &TypeFactory::GetType(type_id, true)));
  }

  for (const TypeID string_type_id : {kChar, kVarChar}) {
    EXPECT_TRUE(comparison.canComparePartialTypes(
        &TypeFactory::GetType(string_type_id, 10, false), nullptr));
    EXPECT_TRUE(comparison.canComparePartialTypes(
        &TypeFactory::GetType(string_type_id, 10, true), nullptr));
    EXPECT_TRUE(comparison.canComparePartialTypes(
        nullptr, &TypeFactory::GetType(string_type_id, 10, false)));
    EXPECT_TRUE(comparison.canComparePartialTypes(
        nullptr, &TypeFactory::GetType(string_type_id, 10, true)));
  }

  // If both types are known, expect the same thing as canCompareTypes().
  for (const TypeID first_type_id
       : {kInt, kLong, kFloat, kDouble, kDatetime, kDatetimeInterval, kYearMonthInterval}) {
    const Type &first_type = TypeFactory::GetType(first_type_id, false);
    const Type &first_type_nullable = TypeFactory::GetType(first_type_id, true);

    for (const TypeID second_type_id
         : {kInt, kLong, kFloat, kDouble, kDatetime, kDatetimeInterval, kYearMonthInterval}) {
      const Type &second_type = TypeFactory::GetType(second_type_id, false);
      const Type &second_type_nullable = TypeFactory::GetType(second_type_id, true);
      EXPECT_EQ(comparison.canCompareTypes(first_type, second_type),
                comparison.canComparePartialTypes(&first_type, &second_type));
      EXPECT_EQ(comparison.canCompareTypes(first_type, second_type_nullable),
                comparison.canComparePartialTypes(&first_type, &second_type_nullable));
      EXPECT_EQ(comparison.canCompareTypes(first_type_nullable, second_type),
                comparison.canComparePartialTypes(&first_type_nullable, &second_type));
      EXPECT_EQ(comparison.canCompareTypes(first_type_nullable, second_type_nullable),
                comparison.canComparePartialTypes(&first_type_nullable, &second_type_nullable));
    }

    for (const TypeID second_type_id : {kChar, kVarChar}) {
      const Type &second_type = TypeFactory::GetType(second_type_id, 10, false);
      const Type &second_type_nullable = TypeFactory::GetType(second_type_id, 10, true);
      EXPECT_EQ(comparison.canCompareTypes(first_type, second_type),
                comparison.canComparePartialTypes(&first_type, &second_type));
      EXPECT_EQ(comparison.canCompareTypes(first_type, second_type_nullable),
                comparison.canComparePartialTypes(&first_type, &second_type_nullable));
      EXPECT_EQ(comparison.canCompareTypes(first_type_nullable, second_type),
                comparison.canComparePartialTypes(&first_type_nullable, &second_type));
      EXPECT_EQ(comparison.canCompareTypes(first_type_nullable, second_type_nullable),
                comparison.canComparePartialTypes(&first_type_nullable, &second_type_nullable));
    }
  }

  for (const TypeID first_type_id : {kChar, kVarChar}) {
    const Type &first_type = TypeFactory::GetType(first_type_id, 10, false);
    const Type &first_type_nullable = TypeFactory::GetType(first_type_id, 10, true);

    for (const TypeID second_type_id
         : {kInt, kLong, kFloat, kDouble, kDatetime, kDatetimeInterval, kYearMonthInterval}) {
      const Type &second_type = TypeFactory::GetType(second_type_id, false);
      const Type &second_type_nullable = TypeFactory::GetType(second_type_id, true);
      EXPECT_EQ(comparison.canCompareTypes(first_type, second_type),
                comparison.canComparePartialTypes(&first_type, &second_type));
      EXPECT_EQ(comparison.canCompareTypes(first_type, second_type_nullable),
                comparison.canComparePartialTypes(&first_type, &second_type_nullable));
      EXPECT_EQ(comparison.canCompareTypes(first_type_nullable, second_type),
                comparison.canComparePartialTypes(&first_type_nullable, &second_type));
      EXPECT_EQ(comparison.canCompareTypes(first_type_nullable, second_type_nullable),
                comparison.canComparePartialTypes(&first_type_nullable, &second_type_nullable));
    }

    for (const TypeID second_type_id : {kChar, kVarChar}) {
      const Type &second_type = TypeFactory::GetType(second_type_id, 10, false);
      const Type &second_type_nullable = TypeFactory::GetType(second_type_id, 10, true);
      EXPECT_EQ(comparison.canCompareTypes(first_type, second_type),
                comparison.canComparePartialTypes(&first_type, &second_type));
      EXPECT_EQ(comparison.canCompareTypes(first_type, second_type_nullable),
                comparison.canComparePartialTypes(&first_type, &second_type_nullable));
      EXPECT_EQ(comparison.canCompareTypes(first_type_nullable, second_type),
                comparison.canComparePartialTypes(&first_type_nullable, &second_type));
      EXPECT_EQ(comparison.canCompareTypes(first_type_nullable, second_type_nullable),
                comparison.canComparePartialTypes(&first_type_nullable, &second_type_nullable));
    }
  }
}

TEST_F(ComparisonTest, CanComparePartialTypesTest) {
  CheckBasicComparisonApplicableToPartialTypes(
      ComparisonFactory::GetComparison(ComparisonID::kEqual));
  CheckBasicComparisonApplicableToPartialTypes(
      ComparisonFactory::GetComparison(ComparisonID::kNotEqual));
  CheckBasicComparisonApplicableToPartialTypes(
      ComparisonFactory::GetComparison(ComparisonID::kLess));
  CheckBasicComparisonApplicableToPartialTypes(
      ComparisonFactory::GetComparison(ComparisonID::kLessOrEqual));
  CheckBasicComparisonApplicableToPartialTypes(
      ComparisonFactory::GetComparison(ComparisonID::kGreater));
  CheckBasicComparisonApplicableToPartialTypes(
      ComparisonFactory::GetComparison(ComparisonID::kGreaterOrEqual));
}

TEST_F(ComparisonTest, PatternMatchingComparisonTest) {
  const Comparison &like_comp =
      ComparisonFactory::GetComparison(ComparisonID::kLike);
  const Comparison &not_like_comp =
      ComparisonFactory::GetComparison(ComparisonID::kNotLike);
  const Comparison &regex_comp =
      ComparisonFactory::GetComparison(ComparisonID::kRegexMatch);
  const Comparison &not_regex_comp =
      ComparisonFactory::GetComparison(ComparisonID::kRegexMatch);

  // Check canCompareTypes method
  for (const TypeID tid1 : {kChar, kVarChar}) {
    const Type &t1 = TypeFactory::GetType(tid1, 10, false);
    for (const TypeID tid2 : {kChar, kVarChar}) {
      const Type &t2 = TypeFactory::GetType(tid2, 10, false);
      EXPECT_TRUE(like_comp.canCompareTypes(t1, t2));
      EXPECT_TRUE(not_like_comp.canCompareTypes(t1, t2));
      EXPECT_TRUE(regex_comp.canCompareTypes(t1, t2));
      EXPECT_TRUE(not_regex_comp.canCompareTypes(t1, t2));
    }
  }

  string text = "abc123\nXYZ\\n 127.0.0.1 @xxx.com";
  vector<string> matched_like_patterns = {
      "%",
      "abc%",
      "%127.0.0.1%",
      "%@x_x_c%",
      "%\n%\\% @%",
      "abc%_%_3%" };
  vector<string> not_matched_like_patterns = {
      "abc",
      "%127__0.0.1%",
      "abc%_%_2",
      "%\\nXYZ%" };
  vector<string> matched_regex_patterns = {
      ".*\\d+.*",
      "abc[^x]+xxx\\..*",
      ".*\\d+\\.\\d+\\.\\d+\\.\\d+.*",
      ".*\n[a-zA-Z]{3}.*" };
  vector<string> not_matched_regex_patterns = {
      ".*\\d{4}.*",
      ".*XYZ\n .*",
      ".*(\\d+\\.)*" };

  for (const TypeID tid1 : {kChar, kVarChar}) {
    const Type &t1 = TypeFactory::GetType(tid1, 32, false);
    const TypedValue text_value(tid1, text.c_str(), text.size() + 1);
    for (const TypeID tid2 : {kChar, kVarChar}) {
      const Type &t2 = TypeFactory::GetType(tid2, 32, false);

      for (const string &sp : matched_like_patterns) {
        const TypedValue pattern_value(tid2, sp.c_str(), sp.size() + 1);
        EXPECT_TRUE(like_comp.compareTypedValuesChecked(text_value, t1, pattern_value, t2));
        EXPECT_FALSE(not_like_comp.compareTypedValuesChecked(text_value, t1, pattern_value, t2));
      }

      for (const string &sp : not_matched_like_patterns) {
        const TypedValue pattern_value(tid2, sp.c_str(), sp.size() + 1);
        EXPECT_FALSE(like_comp.compareTypedValuesChecked(text_value, t1, pattern_value, t2));
        EXPECT_TRUE(not_like_comp.compareTypedValuesChecked(text_value, t1, pattern_value, t2));
      }

      for (const string &sp : matched_regex_patterns) {
        const TypedValue pattern_value(tid2, sp.c_str(), sp.size() + 1);
        EXPECT_TRUE(regex_comp.compareTypedValuesChecked(text_value, t1, pattern_value, t2));
        EXPECT_FALSE(not_regex_comp.compareTypedValuesChecked(text_value, t1, pattern_value, t2));
      }

      for (const string &sp : not_matched_regex_patterns) {
        const TypedValue pattern_value(tid2, sp.c_str(), sp.size() + 1);
        EXPECT_FALSE(regex_comp.compareTypedValuesChecked(text_value, t1, pattern_value, t2));
        EXPECT_TRUE(not_regex_comp.compareTypedValuesChecked(text_value, t1, pattern_value, t2));
      }
    }
  }
}

}  // namespace quickstep
