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

#include "types/operations/unary_operations/DateExtractOperation.hpp"

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

#include "types/DateType.hpp"
#include "types/DatetimeLit.hpp"
#include "types/DatetimeType.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/unary_operations/UnaryOperationSynthesizer.hpp"

#include "glog/logging.h"

namespace quickstep {

struct DateExtractFunctor : public UnaryFunctor<DateType, IntType> {
  template <typename DateExtractUnitT>
  inline int apply(const DateLit &argument) const {
    switch (DateExtractUnitT::value) {
      case DateExtractUnit::kYear:
        return argument.yearField();
      case DateExtractUnit::kMonth:
        return argument.monthField();
      case DateExtractUnit::kDay:
        return argument.dayField();
      default:
        DLOG(FATAL) << "Unsupported DateExtractUnit in DateExtractFunctor::apply";
    }
  }
};

struct DatetimeExtractFunctor : public UnaryFunctor<DatetimeType, IntType> {
  template <typename DateExtractUnitT>
  inline std::int64_t apply(const DatetimeLit &argument) const {
    switch (DateExtractUnitT::value) {
      case DateExtractUnit::kYear:
        return argument.yearField();
      case DateExtractUnit::kMonth:
        return argument.monthField();
      case DateExtractUnit::kDay:
        return argument.dayField();
      case DateExtractUnit::kHour:
        return argument.hourField();
      case DateExtractUnit::kMinute:
        return argument.minuteField();
      case DateExtractUnit::kSecond:
        return argument.secondField();
      default:
        DLOG(FATAL) << "Unsupported DateExtractUnit in DatetimeExtractFunctor::apply";
    }
  }
};

const std::map<std::string, DateExtractUnit> DateExtractOperation::kNameToUnitMap = {
    { "year",   DateExtractUnit::kYear },
    { "month",  DateExtractUnit::kMonth },
    { "day",    DateExtractUnit::kDay },
    { "hour",   DateExtractUnit::kHour },
    { "minute", DateExtractUnit::kMinute },
    { "second", DateExtractUnit::kSecond }
};

UncheckedUnaryOperator* DateExtractOperation::makeUncheckedUnaryOperator(
    const Type &type,
    const std::vector<TypedValue> &static_arguments) const {
  DCHECK_EQ(1u, static_arguments.size());

  const DateExtractUnit unit = parseUnit(static_arguments.front());
  const Type *result_type = getResultType(type, static_arguments);

  if (type.getTypeID() == kDate) {
    switch (unit) {
      case DateExtractUnit::kYear:
        return new UncheckedUnaryOperatorSynthesizer<
             DateExtractFunctor,
             std::integral_constant<DateExtractUnit, DateExtractUnit::kYear>>(
                 type, *result_type);
      case DateExtractUnit::kMonth:
        return new UncheckedUnaryOperatorSynthesizer<
             DateExtractFunctor,
             std::integral_constant<DateExtractUnit, DateExtractUnit::kMonth>>(
                 type, *result_type);
      case DateExtractUnit::kDay:
        return new UncheckedUnaryOperatorSynthesizer<
             DateExtractFunctor,
             std::integral_constant<DateExtractUnit, DateExtractUnit::kDay>>(
                 type, *result_type);
      default:
        LOG(FATAL) << "Unsupported DateExtractUnit for DateType in "
                   << "DateExtractOperation::makeUncheckedUnaryOperator";
    }
  } else {
    switch (unit) {
      case DateExtractUnit::kYear:
        return new UncheckedUnaryOperatorSynthesizer<
             DatetimeExtractFunctor,
             std::integral_constant<DateExtractUnit, DateExtractUnit::kYear>>(
                 type, *result_type);
      case DateExtractUnit::kMonth:
        return new UncheckedUnaryOperatorSynthesizer<
             DatetimeExtractFunctor,
             std::integral_constant<DateExtractUnit, DateExtractUnit::kMonth>>(
                 type, *result_type);
      case DateExtractUnit::kDay:
        return new UncheckedUnaryOperatorSynthesizer<
             DatetimeExtractFunctor,
             std::integral_constant<DateExtractUnit, DateExtractUnit::kDay>>(
                 type, *result_type);
      case DateExtractUnit::kHour:
        return new UncheckedUnaryOperatorSynthesizer<
             DatetimeExtractFunctor,
             std::integral_constant<DateExtractUnit, DateExtractUnit::kHour>>(
                 type, *result_type);
      case DateExtractUnit::kMinute:
        return new UncheckedUnaryOperatorSynthesizer<
             DatetimeExtractFunctor,
             std::integral_constant<DateExtractUnit, DateExtractUnit::kMinute>>(
                 type, *result_type);
      case DateExtractUnit::kSecond:
        return new UncheckedUnaryOperatorSynthesizer<
             DatetimeExtractFunctor,
             std::integral_constant<DateExtractUnit, DateExtractUnit::kSecond>>(
                 type, *result_type);
      default:
        LOG(FATAL) << "Unsupported DateExtractUnit for DatetimeType in "
                   << "DateExtractOperation::makeUncheckedUnaryOperator";
    }
  }
}

}  // namespace quickstep
