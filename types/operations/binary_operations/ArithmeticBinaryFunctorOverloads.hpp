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

#ifndef QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_ARITHMETIC_BINARY_FUNCTOR_OVERLOADS_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_ARITHMETIC_BINARY_FUNCTOR_OVERLOADS_HPP_

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

#include "types/DateOperatorOverloads.hpp"
#include "utility/meta/Common.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

// We use these functors instead of the standard-library ones, because the
// standard-library functors in <functional> have to be instantiated for the
// most specific argument type, which would unnecessisarily introduce
// multiple copies of distinct template instantiations of operators.
template <typename LeftCppType, typename RightCppType, typename EnableT = void>
struct AddFunctorOverloads {
  inline auto operator() (const LeftCppType &left,
                          const RightCppType &right) const -> decltype(left + right) {
    return left + right;
  }
};

// NOTE(zuyu): The C++ compiler in general converts all integers to floats
//             when doing the following operations,
//             but we could like to return double instead.
template <>
struct AddFunctorOverloads<std::int64_t, float> {
  inline double operator() (const std::int64_t &left, const float &right) const {
    return static_cast<double>(left) + static_cast<double>(right);
  }
};

template <>
struct AddFunctorOverloads<float, std::int64_t> {
  inline double operator() (const float &left, const std::int64_t &right) const {
    return static_cast<double>(left) + static_cast<double>(right);
  }
};

template <typename LeftCppType, typename RightCppType, typename EnableT = void>
struct SubtractFunctorOverloads {
  inline auto operator() (const LeftCppType &left,
                          const RightCppType &right) const -> decltype(left - right) {
    return left - right;
  }
};

// NOTE(zuyu): The C++ compiler in general converts all integers to floats
//             when doing the following operations,
//             but we could like to return double instead.
template <>
struct SubtractFunctorOverloads<std::int64_t, float> {
  inline double operator() (const std::int64_t &left, const float &right) const {
    return static_cast<double>(left) - static_cast<double>(right);
  }
};

template <>
struct SubtractFunctorOverloads<float, std::int64_t> {
  inline double operator() (const float &left, const std::int64_t &right) const {
    return static_cast<double>(left) - static_cast<double>(right);
  }
};

template <typename LeftCppType, typename RightCppType, typename EnableT = void>
struct MultiplyFunctorOverloads {
  inline auto operator() (const LeftCppType &left,
                          const RightCppType &right) const -> decltype(left * right) {
    return left * right;
  }
};

// NOTE(zuyu): The C++ compiler in general converts all integers to floats
//             when doing the following operations,
//             but we could like to return double instead.
template <>
struct MultiplyFunctorOverloads<std::int64_t, float> {
  inline double operator() (const std::int64_t &left, const float &right) const {
    return static_cast<double>(left) * static_cast<double>(right);
  }
};

template <>
struct MultiplyFunctorOverloads<float, std::int64_t> {
  inline double operator() (const float &left, const std::int64_t &right) const {
    return static_cast<double>(left) * static_cast<double>(right);
  }
};

template <typename LeftCppType, typename RightCppType, typename EnableT = void>
struct DivideFunctorOverloads {
  inline auto operator() (const LeftCppType &left,
                          const RightCppType &right) const -> decltype(left / right) {
    return left / right;
  }
};

// NOTE(zuyu): The C++ compiler in general converts all integers to floats
//             when doing the following operations,
//             but we could like to return double instead.
template <>
struct DivideFunctorOverloads<std::int64_t, float> {
  inline double operator() (const std::int64_t &left, const float &right) const {
    return static_cast<double>(left) / static_cast<double>(right);
  }
};

template <>
struct DivideFunctorOverloads<float, std::int64_t> {
  inline double operator() (const float &left, const std::int64_t &right) const {
    return static_cast<double>(left) / static_cast<double>(right);
  }
};

template <typename LeftCppType, typename RightCppType, typename EnableT = void>
struct ModuloFunctorOverloads;

template <typename LeftCppType, typename RightCppType>
struct ModuloFunctorOverloads<
    LeftCppType, RightCppType,
    std::enable_if_t<meta::EqualsAny<LeftCppType, int, std::int64_t>::value &&
                     meta::EqualsAny<RightCppType, int, std::int64_t>::value>> {
  inline auto operator() (const LeftCppType &left,
                          const RightCppType &right) const -> decltype(left % right) {
    return left % right;
  }
};

// NOTE(jianqiao): The C++11 standard specifies the following type signatures for fmod:
// (1) (double, double) -> double
// (2) (float, float) -> float
// (3) (long double, long double) -> long double
// (3) (Arithmetic, Arithmetic) -> double
template <typename LeftCppType, typename RightCppType>
struct ModuloFunctorOverloads<
    LeftCppType, RightCppType,
    std::enable_if_t<meta::EqualsAny<LeftCppType, float, double>::value ||
                     meta::EqualsAny<RightCppType, float, double>::value>> {
  inline auto operator() (const LeftCppType &left,
                          const RightCppType &right) const -> decltype(std::fmod(left, right)) {
    return std::fmod(left, right);
  }
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_ARITHMETIC_BINARY_FUNCTOR_OVERLOADS_HPP_
