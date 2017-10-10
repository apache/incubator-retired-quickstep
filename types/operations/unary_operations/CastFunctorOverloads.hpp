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

#ifndef QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_CAST_FUNCTOR_OVERLOADS_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_CAST_FUNCTOR_OVERLOADS_HPP_

#include <algorithm>
#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>

#include "types/GenericValue.hpp"
#include "types/TypeRegistrar.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/unary_operations/UnaryOperationSynthesizer.hpp"
#include "utility/meta/TypeList.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

template <typename SourceType, typename TargetType, typename Enable = void>
struct CastFunctor;

// ----------------------------------------------------------------------------
// Implementations of numeric to numeric casts.

using NumericTypes = meta::TypeList<BoolType, IntType, LongType, FloatType, DoubleType>;
using NumericCastOverloads = NumericTypes::cartesian_product<NumericTypes>;

template <typename SourceType, typename TargetType>
struct CastFunctor<
    SourceType, TargetType,
    std::enable_if_t<
        NumericCastOverloads::contains<meta::TypeList<SourceType, TargetType>>::value>>
    : public UnaryFunctor<SourceType, TargetType> {
  inline typename TargetType::cpptype apply(
      const typename SourceType::cpptype &argument) const {
    return static_cast<typename TargetType::cpptype>(argument);
  }
};


// ----------------------------------------------------------------------------
// Implementations of any type to ascii string casts.

using UnsupportedCastToAsciiStringTypes = meta::TypeList<NullType>;

// TODO(refactor-type): Better implementation of casting among ascii string types.

template <typename SourceType>
struct CastFunctor<
    SourceType, CharType,
    std::enable_if_t<!UnsupportedCastToAsciiStringTypes::contains<SourceType>::value>>
    : public UnaryFunctor<SourceType, CharType> {
  CastFunctor(const SourceType &source_type_in,
              const CharType &target_type_in)
      : source_type(source_type_in),
        max_string_length(target_type_in.getStringLength()) {}
  inline void apply(const typename SourceType::cpptype &argument, void *result) const {
    std::string str = source_type.printValueToString(&argument);
    const std::size_t str_len = str.length();
    if (str_len < max_string_length) {
      std::memcpy(result, str.c_str(), str_len);
      static_cast<char *>(result)[str_len] = 0;
    } else {
      std::memcpy(result, str.c_str(), max_string_length);
    }
  }
  const SourceType &source_type;
  const std::size_t max_string_length;
};

template <typename SourceType>
struct CastFunctor<
    SourceType, VarCharType,
    std::enable_if_t<!UnsupportedCastToAsciiStringTypes::contains<SourceType>::value>>
    : public UnaryFunctor<SourceType, VarCharType> {
  CastFunctor(const SourceType &source_type_in,
              const VarCharType &target_type_in)
      : source_type(source_type_in),
        max_string_length(target_type_in.getStringLength()) {}
  inline TypedValue apply(const typename SourceType::cpptype &argument) const {
    std::string str = source_type.printValueToString(&argument);
    const std::size_t len = std::min(str.length(), max_string_length);
    const std::size_t buf_len = len + 1;
    char *buf = static_cast<char *>(std::malloc(buf_len));
    std::memcpy(buf, str.c_str(), len);
    buf[len] = 0;
    return TypedValue::CreateWithOwnedData(kVarChar, buf, buf_len);
  }
  const SourceType &source_type;
  const std::size_t max_string_length;
};

template <typename SourceType>
struct CastFunctor<
    SourceType, TextType,
    std::enable_if_t<!UnsupportedCastToAsciiStringTypes::contains<SourceType>::value>>
    : public UnaryFunctor<SourceType, TextType> {
  CastFunctor(const SourceType &source_type_in,
              const TextType &target_type_in)
      : source_type(source_type_in) {}
  inline std::string apply(const typename SourceType::cpptype &argument) const {
    return source_type.printValueToString(&argument);
  }
  const SourceType &source_type;
};


//// ----------------------------------------------------------------------------
//// Implementations of ascii string to other types casts.

using UnsupportedParseFromAsciiStringTypes = meta::TypeList<NullType>;

// TODO(refactor-type): Implement parseTypedValueFromString for other non-numeric types.
// TODO(refactor-type): Possibly return NULL values.

template <typename TargetType>
struct CastFunctor<
    CharType, TargetType,
    std::enable_if_t<!UnsupportedParseFromAsciiStringTypes::contains<TargetType>::value &&
                     TargetType::kMemoryLayout == kCxxInlinePod>>
    : public UnaryFunctor<CharType, TargetType> {
  CastFunctor(const CharType &source_type_in,
              const TargetType &target_type_in)
      : max_string_length(source_type_in.getStringLength()),
        target_type(target_type_in) {}
  inline typename TargetType::cpptype apply(const void *argument) const {
    // TODO(refactor-type): Implement specialized parseValueFromString.
    const char *str = static_cast<const char*>(argument);
    const std::string value(str, strnlen(str, max_string_length));
    TypedValue result;
    target_type.parseTypedValueFromString(value, &result);
    return result.getLiteral<typename TargetType::cpptype>();
  }
  const std::size_t max_string_length;
  const TargetType &target_type;
};


template <typename TargetType>
struct CastFunctor<VarCharType, TargetType,
                   std::enable_if_t<NumericTypes::contains<TargetType>::value>>
    : public UnaryFunctor<VarCharType, TargetType> {
  CastFunctor(const VarCharType &source_type_in,
              const TargetType &target_type_in)
      : max_string_length(source_type_in.getStringLength()),
        target_type(target_type_in) {}
  inline typename TargetType::cpptype apply(const TypedValue &argument) const {
    const std::string value(static_cast<const char*>(argument.getOutOfLineData()));
    TypedValue result;
    target_type.parseTypedValueFromString(value, &result);
    return result.getLiteral<typename TargetType::cpptype>();
  }
  const std::size_t max_string_length;
  const Type &target_type;
};

template <typename TargetType>
struct CastFunctor<
    TextType, TargetType,
    std::enable_if_t<!UnsupportedParseFromAsciiStringTypes::contains<TargetType>::value &&
                     TargetType::kMemoryLayout == kCxxInlinePod>>
    : public UnaryFunctor<TextType, TargetType> {
  CastFunctor(const TextType &source_type_in,
              const TargetType &target_type_in)
      : target_type(target_type_in) {}
  inline typename TargetType::cpptype apply(const std::string &argument) const {
    // TODO(refactor-type): Implement specialized parseValueFromString.
    TypedValue result;
    target_type.parseTypedValueFromString(argument, &result);
    return result.getLiteral<typename TargetType::cpptype>();
  }
  const TargetType &target_type;
};


//// ----------------------------------------------------------------------------
//// Implementations of array to array casts.

template <>
struct CastFunctor<ArrayType, ArrayType>
    : public UnaryFunctor<ArrayType, ArrayType> {
  CastFunctor(const ArrayType &source_type_in,
              const ArrayType &target_type_in)
      : source_element_type(source_type_in.getElementType()),
        target_element_type(target_type_in.getElementType()) {}
  inline ArrayLit apply(const ArrayLit &argument) const {
    ArrayLit result(target_element_type);
    for (const auto *value : argument) {
      result.push_back(target_element_type.coerceValue(value, source_element_type));
    }
    return result;
  }
  const Type &source_element_type;
  const Type &target_element_type;
};


/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_CAST_FUNCTOR_OVERLOADS_HPP_
