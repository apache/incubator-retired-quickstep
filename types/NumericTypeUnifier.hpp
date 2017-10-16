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

#ifndef QUICKSTEP_TYPES_NUMERIC_TYPE_UNIFIER_HPP_
#define QUICKSTEP_TYPES_NUMERIC_TYPE_UNIFIER_HPP_

#include "types/NumericTypeSafeCoercibility.hpp"

namespace quickstep {

/**
 * @brief A traits template that resolves what the "unifying" Type of two
 *        built-in numeric Types (currently IntType, LongType, FloatType, and
 *        DoubleType) is at compile time.
 *
 * If both argument types are built-in numeric Types, then the struct will
 * contain a typedef called "type" that refers to the unifying Type class.
 *
 * @note A "unifying" Type is a type that both argument types can be safely
 *       coerced to. It is not necessarily one of the input types (for example,
 *       the unifying type for LongType and FloatType is DoubleType).
 * @note This provides a subset of the functionality of
 *       TypeFactory::GetUnifyingType(). It only covers numeric types, and does
 *       not do any handling of nullability. It does, however, allow static
 *       resolution at compile-time.
 *
 * @tparam LeftType The first Quickstep numeric Type class to unify.
 * @tparam RightType The second Quickstep numeric Type class to unify.
 **/
template <typename LeftType, typename RightType>
struct NumericTypeUnifier;


namespace internal {

template <typename LeftType, typename RightType, typename EnableT = void>
struct NumericTypeUnifierHelper;

template <typename LeftType, typename RightType>
struct NumericTypeUnifierHelper<
    LeftType, RightType,
    std::enable_if_t<NumericTypeSafeCoercibility<LeftType, RightType>::value>> {
  typedef RightType type;
};

template <typename LeftType, typename RightType>
struct NumericTypeUnifierHelper<
    LeftType, RightType,
    std::enable_if_t<!std::is_same<LeftType, RightType>::value &&
                     NumericTypeSafeCoercibility<RightType, LeftType>::value>> {
  typedef LeftType type;
};

// Explicit template specializations
template<>
struct NumericTypeUnifierHelper<LongType, FloatType> {
  typedef DoubleType type;
};

template<>
struct NumericTypeUnifierHelper<FloatType, LongType> {
  typedef DoubleType type;
};

}  // namespace internal

template <typename LeftType, typename RightType>
struct NumericTypeUnifier
    : internal::NumericTypeUnifierHelper<LeftType, RightType> {};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_NUMERIC_TYPE_UNIFIER_HPP_
