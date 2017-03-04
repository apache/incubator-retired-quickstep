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

#include "types/TypedValue.hpp"

#include <cstddef>
#include <cstring>

#include "types/DatetimeLit.hpp"
#include "types/IntervalLit.hpp"
#include "types/Type.pb.h"
#include "types/TypeID.hpp"
#include "types/TypedValue.pb.h"
#include "utility/Macros.hpp"

#include "glog/logging.h"

using std::memchr;
using std::size_t;

namespace quickstep {

bool TypedValue::isPlausibleInstanceOf(const TypeSignature type) const {
  TypeID type_id = getTypeID();
  if (type_id != type.id) {
    return false;
  }

  if (isNull()) {
    return type.nullable;
  }

  switch (type_id) {
    case kBool:
    case kInt:
    case kLong:
    case kFloat:
    case kDouble:
    case kDate:
    case kDatetime:
    case kDatetimeInterval:
    case kYearMonthInterval:
      return true;
    case kChar: {
      size_t data_size = getDataSize();
      if (data_size == type.length) {
        return true;
      } else if (data_size < type.length) {
        // If the data is shorter than the max size for a CHAR type, make sure
        // it is null-terminated.
        return (memchr(value_union_.out_of_line_data, '\0', data_size) != nullptr);
      } else {
        return false;
      }
    }
    case kVarChar:
      return getDataSize() <= (type.length + 1);
    default: {
      size_t data_size = getDataSize();
      return (data_size <= type.length);
    }
  }
}

serialization::TypedValue TypedValue::getProto() const {
  serialization::TypedValue proto;

  // NOTE(chasseur): To represent a NULL value, only the 'type_id' field of the
  // proto is filled in, and all the optional value fields are omitted.
  proto.mutable_type_id()->CopyFrom(TypeIDFactory::GetProto(getTypeID()));
  switch (getTypeID()) {
    case kBool:
      if (!isNull()) {
        proto.set_int_value(getLiteral<bool>());
      }
      break;
    case kInt:
      if (!isNull()) {
        proto.set_int_value(getLiteral<int>());
      }
      break;
    case kLong:
      if (!isNull()) {
        proto.set_long_value(getLiteral<std::int64_t>());
      }
      break;
    case kFloat:
      if (!isNull()) {
        proto.set_float_value(getLiteral<float>());
      }
      break;
    case kDouble:
      if (!isNull()) {
        proto.set_double_value(getLiteral<double>());
      }
      break;
    case kDate:
      if (!isNull()) {
        serialization::TypedValue::DateLit *literal_date_proto = proto.mutable_date_value();
        literal_date_proto->set_year(value_union_.date_value.year);
        literal_date_proto->set_month(value_union_.date_value.month);
        literal_date_proto->set_day(value_union_.date_value.day);
      }
      break;
    case kDatetime:
      if (!isNull()) {
        proto.set_datetime_value(value_union_.datetime_value.ticks);
      }
      break;
    case kDatetimeInterval:
      if (!isNull()) {
        proto.set_datetime_interval_value(value_union_.datetime_interval_value.interval_ticks);
      }
      break;
    case kYearMonthInterval:
      if (!isNull()) {
        proto.set_year_month_interval_value(value_union_.year_month_interval_value.months);
      }
      break;
    case kChar:
      if (!isNull()) {
        proto.set_out_of_line_data(static_cast<const char*>(getOutOfLineData()), getDataSize());
      }
      break;
    case kVarChar:
      if (!isNull()) {
        proto.set_out_of_line_data(static_cast<const char*>(getOutOfLineData()), getDataSize());
      }
      break;
    case kNullType:
      DCHECK(isNull());
      break;
    default:
      FATAL_ERROR("Unrecognized TypeID in TypedValue::getProto");
  }

  return proto;
}

bool TypedValue::ProtoIsValid(const serialization::TypedValue &proto) {
  return proto.IsInitialized();
}

TypedValue TypedValue::ReconstructFromProto(const serialization::TypedValue &proto) {
  DCHECK(ProtoIsValid(proto))
      << "Attempted to create TypedValue from an invalid proto description:\n"
      << proto.DebugString();

  const TypeID type_id = TypeIDFactory::ReconstructFromProto(proto.type_id());
  switch (type_id) {
    case kBool:
      return proto.has_bool_value() ?
          TypedValue(static_cast<bool>(proto.bool_value())) :
          TypedValue(kBool);
    case kInt:
      return proto.has_int_value() ?
          TypedValue(static_cast<int>(proto.int_value())) :
          TypedValue(kInt);
    case kLong:
      return proto.has_long_value() ?
          TypedValue(static_cast<std::int64_t>(proto.long_value())) :
          TypedValue(kLong);
    case kFloat:
      return proto.has_float_value() ?
          TypedValue(static_cast<float>(proto.float_value())) :
          TypedValue(kFloat);
    case kDouble:
      return proto.has_double_value() ?
          TypedValue(static_cast<double>(proto.double_value())) :
          TypedValue(kDouble);
    case kDate:
      if (proto.has_date_value()) {
        return TypedValue(DateLit::Create(proto.date_value().year(),
                                          proto.date_value().month(),
                                          proto.date_value().day()));
      } else {
        return TypedValue(kDate);
      }
    case kDatetime:
      if (proto.has_datetime_value()) {
        DatetimeLit datetime;
        datetime.ticks = proto.datetime_value();
        return TypedValue(datetime);
      } else {
        return TypedValue(kDatetime);
      }
    case kDatetimeInterval:
      if (proto.has_datetime_interval_value()) {
        DatetimeIntervalLit interval;
        interval.interval_ticks = proto.datetime_interval_value();
        return TypedValue(interval);
      } else {
        return TypedValue(kDatetimeInterval);
      }
    case kYearMonthInterval:
      if (proto.has_year_month_interval_value()) {
        YearMonthIntervalLit interval;
        interval.months = proto.year_month_interval_value();
        return TypedValue(interval);
      } else {
        return TypedValue(kYearMonthInterval);
      }
    case kChar:
      return proto.has_out_of_line_data() ?
          TypedValue(kChar,
                     static_cast<const void*>(proto.out_of_line_data().c_str()),
                     proto.out_of_line_data().size()).ensureNotReference() :
          TypedValue(kChar);
    case kVarChar:
      return proto.has_out_of_line_data() ?
          TypedValue(kVarChar,
                     static_cast<const void*>(proto.out_of_line_data().c_str()),
                     proto.out_of_line_data().size()).ensureNotReference() :
          TypedValue(kVarChar);
    case kNullType:
      return TypedValue(kNullType);
    default:
      FATAL_ERROR("Unrecognized TypeID in TypedValue::ReconstructFromProto");
  }
}

}  // namespace quickstep
