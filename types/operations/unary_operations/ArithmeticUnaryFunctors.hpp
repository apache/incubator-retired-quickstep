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

#ifndef QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_ARITHMETIC_UNARY_FUNCTORS_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_ARITHMETIC_UNARY_FUNCTORS_HPP_

#include <string>

#include "types/DatetimeIntervalType.hpp"
#include "types/DoubleType.hpp"
#include "types/FloatType.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/YearMonthIntervalType.hpp"
#include "types/operations/unary_operations/UnaryOperationWrapper.hpp"
#include "types/operations/utility/OperationSynthesizeUtil.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

template <typename T>
struct NegateFunctor : public UnaryFunctor<T, T> {
  inline typename T::cpptype apply(const typename T::cpptype &argument) const {
    return -argument;
  }
  inline static std::string GetName() {
    return "-";
  }
};

template <typename ArgumentT>
struct SgnFunctor : public UnaryFunctor<ArgumentT, IntType> {
  inline int apply(const typename ArgumentT::cpptype &argument) const {
    return (argument > 0) - (argument < 0);
  }
  inline static std::string GetName() {
    return "Sgn";
  }
};

using ArithmeticUnaryFunctorPack = FunctorPack<
// negate
  NegateFunctor<IntType>,
  NegateFunctor<LongType>,
  NegateFunctor<FloatType>,
  NegateFunctor<DoubleType>,
  NegateFunctor<DatetimeIntervalType>,
  NegateFunctor<YearMonthIntervalType>,

// sgn (Sign of a numeric value)
  SgnFunctor<IntType>,
  SgnFunctor<LongType>,
  SgnFunctor<FloatType>,
  SgnFunctor<DoubleType>
>;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_ARITHMETIC_UNARY_FUNCTORS_HPP_
