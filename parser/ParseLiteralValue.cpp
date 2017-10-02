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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "parser/ParseLiteralValue.hpp"

#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <limits>
#include <string>
#include <utility>
#include <vector>

#include "types/DatetimeIntervalType.hpp"
#include "types/DoubleType.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/NullType.hpp"
#include "types/Type.hpp"
#include "types/VarCharType.hpp"
#include "types/YearMonthIntervalType.hpp"
#include "utility/SqlError.hpp"

#include "glog/logging.h"

namespace quickstep {

TypedValue NullParseLiteralValue::concretize(
    const Type *type_hint,
    const Type **concretized_type) const {
  if (type_hint == nullptr) {
    *concretized_type = &NullType::InstanceNullable();
  } else {
    *concretized_type = &(type_hint->getNullableVersion());
  }
  return (*concretized_type)->makeNullValue();
}

NumericParseLiteralValue::NumericParseLiteralValue(const int line_number,
                                                   const int column_number,
                                                   const char *numstring)
    : ParseLiteralValue(line_number, column_number),
      numeric_string_(numstring) {
  float_like_ = (numeric_string_.find('.') != std::string::npos)
                || (numeric_string_.find('e') != std::string::npos)
                || (numeric_string_.find('E') != std::string::npos);

  if (!float_like_) {
    CHECK_EQ(1, std::sscanf(numeric_string_.c_str(), "%" SCNd64, &long_value_))
        << "Failed to parse int64_t from numeric string \""
        << numeric_string_ << "\n";
  }
}

void NumericParseLiteralValue::prependMinus() {
  std::string minused("-");
  minused.append(numeric_string_);
  numeric_string_ = std::move(minused);

  if (!float_like_) {
    long_value_ = -long_value_;
  }
}

TypedValue NumericParseLiteralValue::concretize(
    const Type *type_hint,
    const Type **concretized_type) const {
  TypedValue parsed_value;
  if ((type_hint != nullptr)
      && (type_hint->getSuperTypeID() == SuperTypeID::kNumeric)
      && (type_hint->parseValueFromString(numeric_string_, &parsed_value))) {
    *concretized_type = &(type_hint->getNonNullableVersion());
    return parsed_value;
  }

  if (float_like_) {
    *concretized_type = &DoubleType::InstanceNonNullable();
    CHECK((*concretized_type)->parseValueFromString(numeric_string_, &parsed_value))
        << "Failed to parse double from numeric string \""
        << numeric_string_ << "\"";
    return parsed_value;
  } else {
    if ((long_value_ >= std::numeric_limits<int>::min())
        && (long_value_ <= std::numeric_limits<int>::max())) {
      *concretized_type = &IntType::InstanceNonNullable();
      return TypedValue(static_cast<int>(long_value_));
    } else {
      *concretized_type = &LongType::InstanceNonNullable();
      return TypedValue(long_value_);
    }
  }
}

void NumericParseLiteralValue::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  inline_field_names->push_back("numeric_string");
  inline_field_values->push_back(numeric_string_);

  inline_field_names->push_back("float_like");
  inline_field_values->push_back(float_like_ ? "true" : "false");
}

bool StringParseLiteralValue::ParseAmbiguousInterval(
    ParseString *value,
    StringParseLiteralValue **output) {
  *output = new StringParseLiteralValue(value,
                                        nullptr);  // Initially no explicit type.

  // Try parsing as DatetimeInterval.
  (*output)->explicit_type_ = &DatetimeIntervalType::InstanceNonNullable();
  if ((*output)->tryExplicitTypeParse()) {
    return true;
  }

  // Try parsing as YearMonthInterval.
  (*output)->explicit_type_ = &YearMonthIntervalType::InstanceNonNullable();
  if ((*output)->tryExplicitTypeParse()) {
    return true;
  }

  delete *output;
  *output = nullptr;
  return false;
}

TypedValue StringParseLiteralValue::concretize(const Type *type_hint,
                                               const Type **concretized_type) const {
  if (explicit_type_ != nullptr) {
    if ((type_hint != nullptr) && (type_hint->isSafelyCoercibleFrom(*explicit_type_))) {
      *concretized_type = type_hint;
      return type_hint->coerceValue(explicit_parsed_value_, *explicit_type_);
    } else {
      *concretized_type = explicit_type_;
      return explicit_parsed_value_;
    }
  } else {
    TypedValue parsed_value;
    if ((type_hint != nullptr)
        && (type_hint->parseValueFromString(value_->value(), &parsed_value))) {
      *concretized_type = &(type_hint->getNonNullableVersion());
      return parsed_value;
    } else {
      *concretized_type = &VarCharType::InstanceNonNullable(value_->value().length());
      CHECK((*concretized_type)->parseValueFromString(value_->value(), &parsed_value));
      return parsed_value;
    }
  }
}

std::string StringParseLiteralValue::generateName() const {
  std::string name;
  if (explicit_type_ != nullptr) {
    name.append(explicit_type_->getName());
    name.push_back('(');
  }
  name.push_back('\'');
  name.append(value_->value());
  name.push_back('\'');
  if (explicit_type_ != nullptr) {
    name.push_back(')');
  }
  return name;
}

bool StringParseLiteralValue::tryExplicitTypeParse() {
  DCHECK(explicit_type_ != nullptr);
  return explicit_type_->parseValueFromString(value_->value(), &explicit_parsed_value_);
}

void StringParseLiteralValue::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  inline_field_names->push_back("value");
  inline_field_values->push_back(value_->value());

  if (explicit_type_ != nullptr) {
    inline_field_names->push_back("explicit_type");
    inline_field_values->push_back(explicit_type_->getName());
  }
}

}  // namespace quickstep
