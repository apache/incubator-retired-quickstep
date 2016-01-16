/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "types/operations/binary_operations/BinaryOperation.hpp"

#include "types/operations/Operation.pb.h"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

serialization::BinaryOperation BinaryOperation::getProto() const {
  serialization::BinaryOperation proto;
  switch (operation_id_) {
    case BinaryOperationID::kAdd:
      proto.set_operation_id(serialization::BinaryOperation::ADD);
      break;
    case BinaryOperationID::kSubtract:
      proto.set_operation_id(serialization::BinaryOperation::SUBTRACT);
      break;
    case BinaryOperationID::kMultiply:
      proto.set_operation_id(serialization::BinaryOperation::MULTIPLY);
      break;
    case BinaryOperationID::kDivide:
      proto.set_operation_id(serialization::BinaryOperation::DIVIDE);
      break;
    default:
      FATAL_ERROR("Unrecognized BinaryOperationID");
  }

  return proto;
}

}  // namespace quickstep
