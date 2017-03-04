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

#ifndef QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_DATE_EXTRACT_OPERATION_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_DATE_EXTRACT_OPERATION_HPP_

#include <cstdint>
#include <string>

#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/unary_operations/UnaryOperation.hpp"
#include "utility/Macros.hpp"
#include "utility/StringUtil.hpp"

namespace quickstep {

enum class DateExtractUnit {
  kYear = 0,
  kMonth,
  kDay,
  kHour,
  kMinute,
  kSecond,
  kInvalid
};

/**
 * @brief UnaryOperation for Extract.
 */
class DateExtractOperation : public UnaryOperation {
 public:
  DateExtractOperation() {}

  std::string getName() const override {
    return "DateExtract";
  }

  std::string getShortName() const override {
    return "DateExtract";
  }

  std::vector<OperationSignaturePtr> getSignatures() const override {
    const std::vector<TypeID> unit_carrier = { kVarChar };
    return {
        OperationSignature::Create(getName(), {kDate}, unit_carrier),
        OperationSignature::Create(getName(), {kDatetime}, unit_carrier)
    };
  }

  bool canApplyTo(const Type &type,
                  const std::vector<TypedValue> &static_arguments,
                  std::string *message) const override {
    DCHECK(type.getTypeID() == kDate || type.getTypeID() == kDatetime);
    DCHECK_EQ(1u, static_arguments.size());

    const DateExtractUnit unit = parseUnit(static_arguments.front());
    switch (unit) {
      case DateExtractUnit::kYear:  // Fall through
      case DateExtractUnit::kMonth:
      case DateExtractUnit::kDay:
        return true;
      case DateExtractUnit::kHour:  // Fall through
      case DateExtractUnit::kMinute:
      case DateExtractUnit::kSecond:
        if (type.getTypeID() == kDate) {
          *message = "Invalid extraction unit for argument of DATE type";
        } else {
          return true;
        }
      default:
        *message = "Invalid extraction unit for DateExtract";
        return false;
    }
  }

  const Type* getResultType(
      const Type &type,
      const std::vector<TypedValue> &static_arguments) const override {
    DCHECK(UnaryOperation::canApplyTo(type, static_arguments));
    if (type.getTypeID() == kDatetime) {
      return &LongType::Instance(type.isNullable());
    } else {
      DCHECK_EQ(kDate, type.getTypeID());
      return &IntType::Instance(type.isNullable());
    }
  }

  UncheckedUnaryOperator* makeUncheckedUnaryOperator(
      const Type &type,
      const std::vector<TypedValue> &static_arguments) const override;

 private:
  static DateExtractUnit parseUnit(const TypedValue &unit_arg) {
    DCHECK(unit_arg.getTypeID() == kVarChar);
    const std::string unit_str =
        ToLower(std::string(static_cast<const char*>(unit_arg.getOutOfLineData())));

    auto it = kNameToUnitMap.find(unit_str);
    if (it != kNameToUnitMap.end()) {
      return it->second;
    } else {
      return DateExtractUnit::kInvalid;
    }
  }

  static const std::map<std::string, DateExtractUnit> kNameToUnitMap;

  DISALLOW_COPY_AND_ASSIGN(DateExtractOperation);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_DATE_EXTRACT_OPERATION_HPP_ */
