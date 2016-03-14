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
    case BinaryOperationID::kModulo:
      proto.set_operation_id(serialization::BinaryOperation::MODULO);
      break;
    default:
      FATAL_ERROR("Unrecognized BinaryOperationID");
  }

  return proto;
}

}  // namespace quickstep
