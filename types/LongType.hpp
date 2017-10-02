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

#ifndef QUICKSTEP_TYPES_LONG_TYPE_HPP_
#define QUICKSTEP_TYPES_LONG_TYPE_HPP_

#include <cstdint>
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
 * @brief A type representing a 64-bit integer.
 **/
class LongType : public NumericSuperType<kLong> {
 public:
  // Fully represented digits, single leading digit, and possible '-'
  // character.
  int getPrintWidth() const override {
    return std::numeric_limits<std::int64_t>::digits10 + 2;
  }

  std::string printValueToString(const UntypedLiteral *value) const override;

  void printValueToFile(const UntypedLiteral *value,
                        FILE *file,
                        const int padding = 0) const override;

  bool parseTypedValueFromString(const std::string &value_string,
                                 TypedValue *value) const override;

 private:
  explicit LongType(const bool nullable)
      : NumericSuperType<kLong>(nullable) {}

  QUICKSTEP_SYNTHESIZE_TYPE(LongType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_LONG_TYPE_HPP_
