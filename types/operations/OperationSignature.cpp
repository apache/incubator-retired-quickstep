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

#include "types/operations/OperationSignature.hpp"

#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>

#include "types/TypeID.hpp"
#include "types/Type.pb.h"
#include "types/operations/Operation.pb.h"

namespace quickstep {

serialization::OperationSignature OperationSignature::getProto() const {
  serialization::OperationSignature op_signature;

  op_signature.set_operation_name(operation_name_);
  for (const TypeID tid : argument_type_ids_) {
    op_signature.add_argument_type_ids()->CopyFrom(TypeIDFactory::GetProto(tid));
  }
  op_signature.set_num_static_arguments(
      static_cast<std::uint32_t>(num_static_arguments_));

  return op_signature;
}

OperationSignaturePtr OperationSignature::ReconstructFromProto(
    const serialization::OperationSignature &proto) {
  std::vector<TypeID> argument_type_ids;
  for (int i = 0; i < proto.argument_type_ids_size(); ++i) {
    argument_type_ids.emplace_back(
        TypeIDFactory::ReconstructFromProto(proto.argument_type_ids(i)));
  }

  return Create(proto.operation_name(),
                argument_type_ids,
                proto.num_static_arguments());
}

std::string OperationSignature::toString() const {
  const std::size_t num_regular_arguments =
      argument_type_ids_.size() - num_static_arguments_;

  std::string str;
  str.append(operation_name_);
  str.push_back('(');
  for (std::size_t i = 0; i < num_regular_arguments; ++i) {
    if (i != 0) {
      str.append(", ");
    }
    str.append(
        kTypeNames[static_cast<std::underlying_type_t<TypeID>>(
            argument_type_ids_[i])]);
  }
  if (num_static_arguments_ > 0) {
    str.append(", static(");
    for (std::size_t i = 0; i < num_static_arguments_; ++i) {
      if (i != 0) {
        str.append(", ");
      }
      str.append(
          kTypeNames[static_cast<std::underlying_type_t<TypeID>>(
              argument_type_ids_[i + num_regular_arguments])]);
    }
    str.push_back(')');
  }
  str.push_back(')');

  return str;
}

}  // namespace quickstep
