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

#ifndef QUICKSTEP_TYPES_ARRAY_TYPE_HPP_
#define QUICKSTEP_TYPES_ARRAY_TYPE_HPP_

#include <cstddef>
#include <string>

#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypeSynthesizer.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class TypedValue;

/** \addtogroup Types
 *  @{
 */

class ArrayType : public TypeSynthesizer<kArray> {
 public:
  int getPrintWidth() const override {
    return 16;
  }

  std::string printValueToString(const UntypedLiteral *value) const override;

  bool parseValueFromString(const std::string &value_string,
                            TypedValue *value) const override {
    return false;
  }

 private:
  ArrayType(const bool nullable, const std::vector<GenericValue> &parameters)
      : TypeSynthesizer<kArray>(nullable, 0, 0x1000, parameters),
        element_type_(ExtractType(parameters)) {
    // TODO(refactor-type): Possibly infinite maximum size.
    // TODO(refactor-type): Validate parameters.
  }

  static const Type& ExtractType(const std::vector<GenericValue> &parameters) {
    DCHECK_EQ(1u, parameters.size());
    const GenericValue &value = parameters.front();
    DCHECK(value.getType().getTypeID() == kMetaType);
    return *static_cast<const Type*>(value.getValue());
  }

  const Type &element_type_;

  QUICKSTEP_SYNTHESIZE_TYPE(ArrayType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_ARRAY_TYPE_HPP_
