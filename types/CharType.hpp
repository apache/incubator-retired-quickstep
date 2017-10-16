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

#ifndef QUICKSTEP_TYPES_CHAR_TYPE_HPP_
#define QUICKSTEP_TYPES_CHAR_TYPE_HPP_

#include <cstddef>
#include <cstdio>
#include <string>

#include "types/AsciiStringSuperType.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief A type representing a fixed-size ASCII string.
 *
 * @note A string that is exactly the maximum length of a CharType is
 *       represented WITHOUT a null-terminator character. Any strings shorter
 *       than the maximum length will have a null-terminator.
 **/
class CharType final : public AsciiStringSuperType<kChar> {
 public:
  bool isSafelyCoercibleFrom(const Type &original_type) const override;

  std::string getName() const override;

  int getPrintWidth() const override {
    return length_;
  }

  std::size_t hashValue(const UntypedLiteral *value) const override;

  bool checkValuesEqual(const UntypedLiteral *lhs,
                        const UntypedLiteral *rhs,
                        const Type &rhs_type) const override;

  UntypedLiteral* cloneValue(const UntypedLiteral *value) const override;

  TypedValue marshallValue(const UntypedLiteral *value) const override;

  UntypedLiteral* unmarshallValue(const void *data,
                                  const std::size_t length) const override;

  std::string printValueToString(const UntypedLiteral *value) const override;

  void printValueToFile(const UntypedLiteral *value,
                        FILE *file,
                        const int padding = 0) const override;

  bool parseTypedValueFromString(const std::string &value_string,
                                 TypedValue *value) const override;

  TypedValue coerceTypedValue(const TypedValue &original_value,
                              const Type &original_type) const override;

 private:
  CharType(const bool nullable, const std::size_t length)
      : AsciiStringSuperType<kChar>(nullable, length, length, length) {}

  QUICKSTEP_SYNTHESIZE_TYPE(CharType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_CHAR_TYPE_HPP_
