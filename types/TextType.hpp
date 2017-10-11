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

#ifndef QUICKSTEP_TYPES_TEXT_TYPE_HPP_
#define QUICKSTEP_TYPES_TEXT_TYPE_HPP_

#include <cstddef>
#include <cstdio>
#include <string>

#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypeSynthesizer.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

class TextType final : public TypeSynthesizer<kText> {
 public:
  int getPrintWidth() const override {
    return 32;
  }

  bool isSafelyCoercibleFrom(const Type &original_type) const override;

  bool checkValuesEqual(const UntypedLiteral *lhs,
                        const UntypedLiteral *rhs,
                        const Type &rhs_type) const override;

  TypedValue marshallValue(const UntypedLiteral *value) const override;

  UntypedLiteral* unmarshallValue(const void *data,
                                  const std::size_t length) const override;

  std::string printValueToString(const UntypedLiteral *value) const override;

  bool parseTypedValueFromString(const std::string &value_string,
                                 TypedValue *value) const override {
    return false;
  }

 private:
  TextType(const bool nullable)
      : TypeSynthesizer<kText>(nullable, 0, 0x1000) {
    // TODO(refactor-type): Possibly infinite maximum size.
  }

  QUICKSTEP_SYNTHESIZE_TYPE(TextType);
};


}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_TEXT_TYPE_HPP_
