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

#include "types/operations/binary_operations/BinaryOperationFactory.hpp"

#include <string>

#include "types/operations/Operation.pb.h"
#include "types/operations/binary_operations/AddBinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "types/operations/binary_operations/DivideBinaryOperation.hpp"
#include "types/operations/binary_operations/ModuloBinaryOperation.hpp"
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
    case BinaryOperationID::kModulo:
      return ModuloBinaryOperation::Instance();
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
    case serialization::BinaryOperation::MODULO:
      return GetBinaryOperation(BinaryOperationID::kModulo);
    default:
      FATAL_ERROR("Unrecognized BinaryOperationID in "
                  "BinaryOperationFactory::ReconstructFromProto");
  }
}

}  // namespace quickstep
