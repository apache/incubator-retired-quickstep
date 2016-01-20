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

#include "types/operations/unary_operations/UnaryOperationFactory.hpp"

#include <string>

#include "types/TypeFactory.hpp"
#include "types/operations/Operation.pb.h"
#include "types/operations/unary_operations/ArithmeticUnaryOperations.hpp"
#include "types/operations/unary_operations/NumericCastOperation.hpp"
#include "types/operations/unary_operations/DateExtractOperation.hpp"
#include "types/operations/unary_operations/UnaryOperationID.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

const UnaryOperation& UnaryOperationFactory::GetUnaryOperation(const UnaryOperationID id) {
  switch (id) {
    case UnaryOperationID::kNegate:
      return NegateUnaryOperation::Instance();
    case UnaryOperationID::kCast:
      FATAL_ERROR("Getting a CastOperation through GetUnaryOperation is not supported");
    case UnaryOperationID::kDateExtract:
      FATAL_ERROR("Getting a DateExtractOperation through GetUnaryOperation is not supported");
    default:
      FATAL_ERROR("Unknown UnaryOperationID");
  }
}

bool UnaryOperationFactory::ProtoIsValid(const serialization::UnaryOperation &proto) {
  // Check that UnaryOperation is fully initialized.
  if (!proto.IsInitialized()) {
    return false;
  }

  // Check that the operation_id is a valid UnaryOperation.
  if (!proto.UnaryOperationID_IsValid(proto.operation_id())) {
    return false;
  }

  switch (proto.operation_id()) {
    case serialization::UnaryOperation::NEGATE:
      return true;
    case serialization::UnaryOperation::CAST:
      return proto.HasExtension(serialization::CastOperation::target_type)
          && TypeFactory::ProtoIsValid(proto.GetExtension(serialization::CastOperation::target_type));
    case serialization::UnaryOperation::DATE_EXTRACT:
      return proto.HasExtension(serialization::DateExtractOperation::unit)
          && DateExtractOperation_Unit_IsValid(proto.GetExtension(serialization::DateExtractOperation::unit));
    default:
      return false;
  }
}

const UnaryOperation& UnaryOperationFactory::ReconstructFromProto(
    const serialization::UnaryOperation &proto) {
  DCHECK(ProtoIsValid(proto))
      << "Attempted to create UnaryOperation from an invalid proto description:\n"
      << proto.DebugString();

  switch (proto.operation_id()) {
    case serialization::UnaryOperation::NEGATE:
      return GetUnaryOperation(UnaryOperationID::kNegate);
    case serialization::UnaryOperation::CAST:
      return NumericCastOperation::Instance(
          TypeFactory::ReconstructFromProto(
              proto.GetExtension(
                  serialization::CastOperation::target_type)));
    case serialization::UnaryOperation::DATE_EXTRACT:
      switch (proto.GetExtension(serialization::DateExtractOperation::unit)) {
        case serialization::DateExtractOperation::YEAR:
          return DateExtractOperation::Instance(DateExtractUnit::kYear);
        case serialization::DateExtractOperation::MONTH:
          return DateExtractOperation::Instance(DateExtractUnit::kMonth);
        case serialization::DateExtractOperation::DAY:
          return DateExtractOperation::Instance(DateExtractUnit::kDay);
        case serialization::DateExtractOperation::HOUR:
          return DateExtractOperation::Instance(DateExtractUnit::kHour);
        case serialization::DateExtractOperation::MINUTE:
          return DateExtractOperation::Instance(DateExtractUnit::kMinute);
        case serialization::DateExtractOperation::SECOND:
          return DateExtractOperation::Instance(DateExtractUnit::kSecond);
        default:
          FATAL_ERROR("Unrecognized DateExtractOperation unit in UnaryOperation::ReconstructFromProto");
      }
    default:
      FATAL_ERROR("Unrecognized UnaryOperationID in UnaryOperation::ReconstructFromProto");
  }
}

}  // namespace quickstep
