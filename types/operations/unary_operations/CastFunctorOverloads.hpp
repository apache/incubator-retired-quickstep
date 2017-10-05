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

#include "types/TypeRegistrar.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/unary_operations/UnaryOperationWrapper.hpp"
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

template <typename SourceType>
struct CastFunctor<SourceType, CharType,
                   std::enable_if_t<SourceType::kStaticSuperTypeID == SuperTypeID::kNumeric>>
    : public UnaryFunctor<SourceType, CharType> {
  explicit CastFunctor(const SourceType &source_type_in,
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
struct CastFunctor<SourceType, VarCharType,
                   std::enable_if_t<SourceType::kStaticSuperTypeID == SuperTypeID::kNumeric>>
    : public UnaryFunctor<SourceType, VarCharType> {
  explicit CastFunctor(const SourceType &source_type_in,
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

//template <typename SourceType>
//struct CastFunctor<SourceType, TextType>
//    : public UnaryFunctor<SourceType, TextType> {
//  explicit CastFunctor(const SourceType &source_type_in,
//                       const TextType &target_type_in)
//      : source_type(source_type_in) {}
//  inline std::string apply(const typename SourceType::cpptype &argument) const {
//    return source_type.printValueToString(&argument);
//  }
//  const SourceType &source_type;
//};
//
//
//// ----------------------------------------------------------------------------
//// Implementations of ascii string to numeric casts.
//
//template <typename T>
//T CastStringToNumericOverload(const char *str);
//
//template <>
//bool CastStringToNumericOverload(const char *str) {
//  return ToLower(str) == "true";
//}
//template <>
//int CastStringToNumericOverload(const char *str) {
//  return std::atoi(str);
//}
//template <>
//float CastStringToNumericOverload(const char *str) {
//  return static_cast<float>(std::atof(str));
//}
//template <>
//std::int64_t CastStringToNumericOverload(const char *str) {
//  return std::atoll(str);
//}
//template <>
//double CastStringToNumericOverload(const char *str) {
//  return std::atof(str);
//}
//
//template <typename TargetType>
//struct CastFunctor<CharType, TargetType,
//                   std::enable_if_t<NumericTypes::contains<TargetType>::value>>
//    : public UnaryFunctor<CharType, TargetType> {
//  explicit CastFunctor(const CharType &source_type_in,
//                       const TargetType &target_type_in)
//      : max_string_length(source_type_in.getStringLength()) {}
//  inline typename TargetType::cpptype apply(const void *argument) const {
//    const char *str = static_cast<const char*>(argument);
//    const std::string value(str, strnlen(str, max_string_length));
//    return CastStringToNumericOverload<typename TargetType::cpptype>(value.c_str());
//  }
//  const std::size_t max_string_length;
//};
//
//
//template <typename TargetType>
//struct CastFunctor<VarCharType, TargetType,
//                   std::enable_if_t<NumericTypes::contains<TargetType>::value>>
//    : public UnaryFunctor<VarCharType, TargetType> {
//  explicit CastFunctor(const VarCharType &source_type_in,
//                       const TargetType &target_type_in)
//      : max_string_length(source_type_in.getStringLength()) {}
//  inline typename TargetType::cpptype apply(const TypedValue &argument) const {
//    return CastStringToNumericOverload<typename TargetType::cpptype>(
//        static_cast<const char*>(argument.getDataPtr()));
//  }
//  const std::size_t max_string_length;
//};


/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_CAST_FUNCTOR_OVERLOADS_HPP_
