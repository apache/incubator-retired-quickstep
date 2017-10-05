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

#ifndef QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_ASCII_STRING_UNARY_FUNCTORS_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_ASCII_STRING_UNARY_FUNCTORS_HPP_

#include <cctype>
#include <cstring>
#include <string>

#include "types/CharType.hpp"
#include "types/IntType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/VarCharType.hpp"
#include "types/operations/unary_operations/UnaryOperationWrapper.hpp"
#include "types/operations/utility/OperationSynthesizeUtil.hpp"
#include "types/port/strnlen.hpp"
#include "utility/meta/Common.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

template <typename ArgumentT>
struct AsciiStringLengthFunctor : public UnaryFunctor<ArgumentT, IntType> {
  explicit AsciiStringLengthFunctor(const ArgumentT &argument_type)
      : max_string_length_(argument_type.getStringLength()) {}
  inline int apply(const void *argument) const {
    return strnlen(static_cast<const char*>(argument), max_string_length_);
  }
  inline int apply(const TypedValue &argument) const {
    DCHECK(argument.getTypeID() == kVarChar);
    return std::strlen(static_cast<const char*>(argument.getOutOfLineData()));
  }
  inline static std::string GetName() {
    return "length";
  }
  const std::size_t max_string_length_;
};

template <typename ArgumentT, int transform(int), typename FunctorNameT>
struct AsciiStringTranformFunctor : public UnaryFunctor<ArgumentT, ArgumentT> {
  explicit AsciiStringTranformFunctor(const ArgumentT &argument_type)
      : max_string_length_(argument_type.getStringLength()) {}
  inline void apply(const void *argument, void *result) const {
    DCHECK(ArgumentT::kStaticTypeID == kChar);
    const char *argument_str = static_cast<const char*>(argument);
    char *result_str = static_cast<char*>(result);
    for (std::size_t i = 0; i < max_string_length_; ++i) {
      if ((result_str[i] = transform(argument_str[i])) == 0) {
        break;
      }
    }
  }
  inline TypedValue apply(const TypedValue &argument) const {
    DCHECK(argument.getTypeID() == kVarChar);
    const char *argument_str = static_cast<const char*>(argument.getOutOfLineData());
    const std::size_t length = argument.getDataSize();
    char *buf = static_cast<char*>(std::malloc(length));

    for (std::size_t i = 0; i < length; ++i) {
      buf[i] = transform(argument_str[i]);
    }
    return TypedValue::CreateWithOwnedData(kVarChar, buf, length);
  }
  inline static std::string GetName() {
    return FunctorNameT::ToString();
  }
  inline static const Type* GetResultType(const Type &argument_type) {
    DCHECK(argument_type.getTypeID() == ArgumentT::kStaticTypeID);
    return &argument_type;
  }
  const std::size_t max_string_length_;
};

template <typename ArgumentT>
using AsciiStringToLowerCaseFunctor =
    AsciiStringTranformFunctor<ArgumentT, std::tolower,
                               meta::StringLiteral<'t', 'o', 'l', 'o', 'w', 'e', 'r'>>;

template <typename ArgumentT>
using AsciiStringToUpperCaseFunctor =
    AsciiStringTranformFunctor<ArgumentT, std::toupper,
                               meta::StringLiteral<'t', 'o', 'u', 'p', 'p', 'e', 'r'>>;

using AsciiStringUnaryFunctorPack = FunctorPack<
// length
    AsciiStringLengthFunctor<CharType>,
    AsciiStringLengthFunctor<VarCharType>,
// tolower
    AsciiStringToLowerCaseFunctor<CharType>,
    AsciiStringToLowerCaseFunctor<VarCharType>,
// toupper
    AsciiStringToUpperCaseFunctor<CharType>,
    AsciiStringToUpperCaseFunctor<VarCharType>
>;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_ASCII_STRING_UNARY_FUNCTORS_HPP_
