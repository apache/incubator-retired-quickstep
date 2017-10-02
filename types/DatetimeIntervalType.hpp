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

#ifndef QUICKSTEP_TYPES_DATETIME_INTERVAL_TYPE_HPP_
#define QUICKSTEP_TYPES_DATETIME_INTERVAL_TYPE_HPP_

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>

#include "types/IntervalLit.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypeSynthesizer.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief A type representing the datetime interval.
 **/
class DatetimeIntervalType : public TypeSynthesizer<kDatetimeInterval> {
 public:
  int getPrintWidth() const override {
    return DatetimeIntervalLit::kPrintingChars;
  }

  std::string printValueToString(const UntypedLiteral *value) const override;

  TypedValue makeZeroValue() const override {
    return TypedValue(DatetimeIntervalLit{0});
  }

  bool parseValueFromString(const std::string &value_string,
                            TypedValue *value) const override;

 private:
  explicit DatetimeIntervalType(const bool nullable)
      : TypeSynthesizer<kDatetimeInterval>(nullable) {}

  QUICKSTEP_SYNTHESIZE_TYPE(DatetimeIntervalType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_DATETIME_INTERVAL_TYPE_HPP_
