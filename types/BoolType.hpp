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

#ifndef QUICKSTEP_TYPES_BOOL_TYPE_HPP_
#define QUICKSTEP_TYPES_BOOL_TYPE_HPP_

#include <array>
#include <cstdio>
#include <limits>
#include <string>

#include "types/NumericSuperType.hpp"
#include "types/TypeID.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class Type;
class TypedValue;

/** \addtogroup Types
 *  @{
 */

/**
 * @brief A type representing a 8-bit bool.
 **/
class BoolType final : public NumericSuperType<kBool> {
 public:
  int getPrintWidth() const override {
    // "true" or "false"
    return 5;
  }

  std::string printValueToString(const UntypedLiteral *value) const override;

  void printValueToFile(const UntypedLiteral *value,
                        FILE *file,
                        const int padding = 0) const override;

  bool parseTypedValueFromString(const std::string &value_string,
                                 TypedValue *value) const override;

 private:
  explicit BoolType(const bool nullable)
      : NumericSuperType<kBool>(nullable) {}

  QUICKSTEP_SYNTHESIZE_TYPE(BoolType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_BOOL_TYPE_HPP_
