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

#include "types/TypeID.hpp"

#include "types/Type.pb.h"

#include "glog/logging.h"

namespace quickstep {

const char *kTypeNames[] = {
  "Int",
  "Long",
  "Float",
  "Double",
  "Char",
  "VarChar",
  "Date",
  "Datetime",
  "DatetimeInterval",
  "YearMonthInterval",
  "NullType"
};

TypeID DeserializeTypeID(const serialization::Type::TypeID type_id_proto) {
  switch (type_id_proto) {
    case serialization::Type::INT:
      return kInt;
    case serialization::Type::LONG:
      return kLong;
    case serialization::Type::FLOAT:
      return kFloat;
    case serialization::Type::DOUBLE:
      return kDouble;
    case serialization::Type::CHAR:
      return kChar;
    case serialization::Type::VAR_CHAR:
      return kVarChar;
    case serialization::Type::DATE:
      return kDate;
    case serialization::Type::DATETIME:
      return kDatetime;
    case serialization::Type::DATETIME_INTERVAL:
      return kDatetimeInterval;
    case serialization::Type::YEAR_MONTH_INTERVAL:
      return kYearMonthInterval;
    case serialization::Type::NULL_TYPE:
      return kNullType;
    default:
      LOG(FATAL) << "Unrecognized TypeID in DeserializeTypeID";
  }
}

serialization::Type::TypeID SerializeTypeID(const TypeID type_id) {
  switch (type_id) {
    case kInt:
      return serialization::Type::INT;
    case kLong:
      return serialization::Type::LONG;
    case kFloat:
      return serialization::Type::FLOAT;
    case kDouble:
      return serialization::Type::DOUBLE;
    case kChar:
      return serialization::Type::CHAR;
    case kVarChar:
      return serialization::Type::VAR_CHAR;
    case kDate:
      return serialization::Type::DATE;
    case kDatetime:
      return serialization::Type::DATETIME;
    case kDatetimeInterval:
      return serialization::Type::DATETIME_INTERVAL;
    case kYearMonthInterval:
      return serialization::Type::YEAR_MONTH_INTERVAL;
    case kNullType:
      return serialization::Type::NULL_TYPE;
    default:
      LOG(FATAL) << "Unrecognized TypeID in SerializeTypeID";
  }
}

}  // namespace quickstep
