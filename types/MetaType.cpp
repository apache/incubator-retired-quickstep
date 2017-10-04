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

#include "types/MetaType.hpp"

#include <cstddef>
#include <string>

#include "types/Type.pb.h"
#include "types/TypeFactory-decl.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"

namespace quickstep {

bool MetaType::checkValuesEqual(const UntypedLiteral *lhs,
                                const UntypedLiteral *rhs,
                                const Type &rhs_type) const {
  if (!equals(rhs_type)) {
    return false;
  }
  return castValueToLiteral(lhs)->equals(*castValueToLiteral(rhs));
}

TypedValue MetaType::marshallValue(const UntypedLiteral *value) const {
  const Type *type = castValueToLiteral(value);
  serialization::Type proto = type->getProto();
  const std::size_t data_size = proto.ByteSize();
  void *data = std::malloc(data_size);
  proto.SerializeToArray(data, data_size);
  return TypedValue::CreateWithOwnedData(kMetaType, data, data_size);
}

UntypedLiteral* MetaType::unmarshallValue(const void *data,
                                          const std::size_t data_size) const {
  serialization::Type proto;
  proto.ParseFromArray(data, data_size);
  return new MetaTypeLiteral(&TypeFactory::ReconstructFromProto(proto));
}

std::string MetaType::printValueToString(const UntypedLiteral *value) const {
  DCHECK(value != nullptr);

  return castValueToLiteral(value)->getName();
}

}  // namespace quickstep
