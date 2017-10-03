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

#include "types/ArrayType.hpp"

#include <cstdlib>
#include <string>

#include "types/Type.pb.h"
#include "types/TypeID.hpp"

#include "glog/logging.h"

namespace quickstep {

std::string ArrayType::getName() const {
  std::string name("Array(");
  name.append(element_type_.getName());
  name.push_back(')');
  if (nullable_) {
    name.append(" NULL");
  }
  return name;
}

bool ArrayType::checkValuesEqual(const UntypedLiteral *lhs,
                                 const UntypedLiteral *rhs,
                                 const Type &rhs_type) const {
  if (!equals(rhs_type)) {
    return false;
  }
  const ArrayLiteral &lhs_array = castValueToLiteral(lhs);
  const ArrayLiteral &rhs_array = castValueToLiteral(rhs);
  if (lhs_array.size() != rhs_array.size()) {
    return false;
  }
  for (std::size_t i = 0; i < lhs_array.size(); ++i) {
    if (!element_type_.checkValuesEqual(lhs_array.at(i), rhs_array.at(i))) {
      return false;
    }
  }
  return true;
}

TypedValue ArrayType::marshallValue(const UntypedLiteral *value) const {
  const ArrayLiteral &array = *static_cast<const ArrayLiteral*>(value);
  serialization::ArrayLiteral proto;
  for (const auto &element : array) {
    // TODO(refactor-type): Improve performance.
    TypedValue value = element_type_.marshallValue(element);
    proto.add_data(value.getDataPtr(), value.getDataSize());
  }
  const std::size_t data_size = proto.ByteSize();
  void *data = std::malloc(data_size);
  proto.SerializeToArray(data, data_size);
  return TypedValue::CreateWithOwnedData(kArray, data, data_size);
}

UntypedLiteral* ArrayType::unmarshallValue(const void *data,
                                           const std::size_t data_size) const {
  std::unique_ptr<ArrayLiteral> array = std::make_unique<ArrayLiteral>();
  serialization::ArrayLiteral proto;
  proto.ParseFromArray(data, data_size);
  for (int i = 0; i < proto.data_size(); ++i) {
    const std::string &element_data = proto.data(i);
    array->emplace_back(
        element_type_.unmarshallValue(element_data.c_str(), element_data.size()));
  }
  return array.release();
}

std::string ArrayType::printValueToString(const UntypedLiteral *value) const {
  DCHECK(value != nullptr);

  const std::vector<UntypedLiteral*> &literals = castValueToLiteral(value);
  std::string ret = "{";
  if (!literals.empty()) {
    ret.append(element_type_.printValueToString(literals.front()));
    for (std::size_t i = 1; i < literals.size(); ++i) {
      ret.append(",");
      ret.append(element_type_.printValueToString(literals.at(i)));
    }
  }
  ret.append("}");
  return ret;
}

}  // namespace quickstep
