/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "types/operations/binary_operations/BinaryOperationFactory.hpp"

#include <string>

#include "types/operations/Operation.pb.h"
#include "types/operations/binary_operations/AddBinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "types/operations/binary_operations/DivideBinaryOperation.hpp"
#include "types/operations/binary_operations/MultiplyBinaryOperation.hpp"
#include "types/operations/binary_operations/SubtractBinaryOperation.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

const BinaryOperation& BinaryOperationFactory::GetBinaryOperation(const BinaryOperationID id) {
  switch (id) {
    case BinaryOperationID::kAdd:
      return AddBinaryOperation::Instance();
    case BinaryOperationID::kSubtract:
      return SubtractBinaryOperation::Instance();
    case BinaryOperationID::kMultiply:
      return MultiplyBinaryOperation::Instance();
    case BinaryOperationID::kDivide:
      return DivideBinaryOperation::Instance();
    default:
      break;  // Prevent compiler from complaining about unhandled case.
  }
  // Should never be reached
  FATAL_ERROR("Unrecognized BinaryOperationID");
}

bool BinaryOperationFactory::ProtoIsValid(const serialization::BinaryOperation &proto) {
  // Check that BinaryOperation is fully initialized.
  if (!proto.IsInitialized()) {
    return false;
  }

  // Check that the operation_id is a valid BinaryOperation.
  if (!proto.BinaryOperationID_IsValid(proto.operation_id())) {
    return false;
  }

  return true;
}

const BinaryOperation& BinaryOperationFactory::ReconstructFromProto(
    const serialization::BinaryOperation &proto) {
  DCHECK(ProtoIsValid(proto))
      << "Attempted to create BinaryOperation from an invalid proto description:\n"
      << proto.DebugString();

  switch (proto.operation_id()) {
    case serialization::BinaryOperation::ADD:
      return GetBinaryOperation(BinaryOperationID::kAdd);
    case serialization::BinaryOperation::SUBTRACT:
      return GetBinaryOperation(BinaryOperationID::kSubtract);
    case serialization::BinaryOperation::MULTIPLY:
      return GetBinaryOperation(BinaryOperationID::kMultiply);
    case serialization::BinaryOperation::DIVIDE:
      return GetBinaryOperation(BinaryOperationID::kDivide);
    default:
      FATAL_ERROR("Unrecognized BinaryOperationID in "
                  "BinaryOperationFactory::ReconstructFromProto");
  }
}

}  // namespace quickstep
