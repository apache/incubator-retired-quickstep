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
    case UnaryOperationID::kSubstring:
      FATAL_ERROR("Must use the overridden SubstringOperation::getProto");
    default:
      FATAL_ERROR("Unrecognized UnaryOperationID in UnaryOperation::getProto");
  }

  return proto;
}

}  // namespace quickstep
