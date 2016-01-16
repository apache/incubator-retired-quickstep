/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "types/operations/unary_operations/UnaryOperation.hpp"

#include "types/operations/Operation.pb.h"
#include "types/operations/unary_operations/UnaryOperationID.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

serialization::UnaryOperation UnaryOperation::getProto() const {
  serialization::UnaryOperation proto;
  switch (operation_id_) {
    case UnaryOperationID::kNegate:
      proto.set_operation_id(serialization::UnaryOperation::NEGATE);
      break;
    case UnaryOperationID::kCast:
      FATAL_ERROR("Must use the overridden NumericCastOperation::getProto");
    case UnaryOperationID::kDateExtract:
      FATAL_ERROR("Must use the overridden DateExtractOperation::getProto");
    default:
      FATAL_ERROR("Unrecognized UnaryOperationID in UnaryOperation::getProto");
  }

  return proto;
}

}  // namespace quickstep
