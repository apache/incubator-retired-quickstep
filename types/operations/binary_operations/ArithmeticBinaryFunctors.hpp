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

#ifndef QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_ARITHMETIC_BINARY_FUNCTORS_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_ARITHMETIC_BINARY_FUNCTORS_HPP_

#include <string>
#include <tuple>

#include "types/DateType.hpp"
#include "types/DatetimeIntervalType.hpp"
#include "types/DatetimeLit.hpp"
#include "types/DatetimeType.hpp"
#include "types/IntervalLit.hpp"
#include "types/NumericTypeUnifier.hpp"
#include "types/Type.hpp"
#include "types/TypeErrors.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/YearMonthIntervalType.hpp"
#include "types/operations/binary_operations/ArithmeticBinaryFunctorOverloads.hpp"
#include "types/operations/binary_operations/BinaryOperationWrapper.hpp"
#include "utility/meta/Common.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

template <typename LeftT, typename RightT, typename ResultT,
          template <typename LeftCppType,
                    typename RightCppType,
                    typename EnableT = void> class FunctorOverload,
          typename FunctorName>
struct ArithmeticBinaryFunctor : public BinaryFunctor<LeftT, RightT, ResultT> {
  ArithmeticBinaryFunctor() : spec() {}
  inline typename ResultT::cpptype apply(const typename LeftT::cpptype &left,
                                         const typename RightT::cpptype &right) const {
    return spec(left, right);
  }
  inline static std::string GetName() {
    return FunctorName::ToString();
  }
  const FunctorOverload<typename LeftT::cpptype,
                        typename RightT::cpptype> spec;
};

template <typename LeftT, typename RightT, typename ResultT>
using AddFunctor = ArithmeticBinaryFunctor<LeftT, RightT, ResultT,
                                           AddFunctorOverloads,
                                           meta::StringLiteral<'+'>>;

template <typename LeftT, typename RightT, typename ResultT>
using SubtractFunctor = ArithmeticBinaryFunctor<LeftT, RightT, ResultT,
                                                SubtractFunctorOverloads,
                                                meta::StringLiteral<'-'>>;

template <typename LeftT, typename RightT, typename ResultT>
using MultiplyFunctor = ArithmeticBinaryFunctor<LeftT, RightT, ResultT,
                                                MultiplyFunctorOverloads,
                                                meta::StringLiteral<'*'>>;

template <typename LeftT, typename RightT, typename ResultT>
using DivideFunctor = ArithmeticBinaryFunctor<LeftT, RightT, ResultT,
                                              DivideFunctorOverloads,
                                              meta::StringLiteral<'/'>>;

template <typename LeftT, typename RightT, typename ResultT>
using ModuloFunctor = ArithmeticBinaryFunctor<LeftT, RightT, ResultT,
                                              ModuloFunctorOverloads,
                                              meta::StringLiteral<'%'>>;

// ----------------------------------------------------------------------------
// Packs of functors:

using AddBinaryFunctorPack = FunctorPack<
// Numeric
    BinaryFunctorCrossProductPack<
        std::tuple<IntType, LongType, FloatType, DoubleType>,
        std::tuple<IntType, LongType, FloatType, DoubleType>,
        AddFunctor, NumericTypeUnifier>,
// Date
    AddFunctor<DateType, YearMonthIntervalType, DateType>,
    AddFunctor<YearMonthIntervalType, DateType, DateType>,
// Datetime
    AddFunctor<DatetimeType, DatetimeIntervalType, DatetimeType>,
    AddFunctor<DatetimeType, YearMonthIntervalType, DatetimeType>,
    AddFunctor<DatetimeIntervalType, DatetimeType, DatetimeType>,
    AddFunctor<YearMonthIntervalType, DatetimeType, DatetimeType>,
// DatetimeInterval
    AddFunctor<DatetimeIntervalType, DatetimeIntervalType, DatetimeIntervalType>,
// YearMonthInterval
    AddFunctor<YearMonthIntervalType, YearMonthIntervalType, YearMonthIntervalType>
>;

using SubtractBinaryFunctorPack = FunctorPack<
// Numeric
    BinaryFunctorCrossProductPack<
        std::tuple<IntType, LongType, FloatType, DoubleType>,
        std::tuple<IntType, LongType, FloatType, DoubleType>,
        SubtractFunctor, NumericTypeUnifier>,
// Date
    SubtractFunctor<DateType, YearMonthIntervalType, DateType>,
    // TODO(quickstep-team):
    // Implement SubtractFunctor<DateType, DateType, YearMonthIntervalType>,
// Datetime
    SubtractFunctor<DatetimeType, DatetimeIntervalType, DatetimeType>,
    SubtractFunctor<DatetimeType, YearMonthIntervalType, DatetimeType>,
    SubtractFunctor<DatetimeType, DatetimeType, DatetimeIntervalType>,
// DatetimeInterval
    SubtractFunctor<DatetimeIntervalType, DatetimeIntervalType, DatetimeIntervalType>,
// YearMonthInterval
    SubtractFunctor<YearMonthIntervalType, YearMonthIntervalType, YearMonthIntervalType>
>;

using MultiplyBinaryFunctorPack = FunctorPack<
// Numeric
    BinaryFunctorCrossProductPack<
        std::tuple<IntType, LongType, FloatType, DoubleType>,
        std::tuple<IntType, LongType, FloatType, DoubleType>,
        MultiplyFunctor, NumericTypeUnifier>,
// DatetimeInterval and YearMonthInterval
    BinaryFunctorCrossProductPack<
        std::tuple<DatetimeIntervalType, YearMonthIntervalType>,
        std::tuple<IntType, LongType, FloatType, DoubleType>,
        MultiplyFunctor, meta::PairSelectorLeft>,
    BinaryFunctorCrossProductPack<
        std::tuple<IntType, LongType, FloatType, DoubleType>,
        std::tuple<DatetimeIntervalType, YearMonthIntervalType>,
        MultiplyFunctor, meta::PairSelectorRight>
>;

using DivideBinaryFunctorPack = FunctorPack<
// Numeric
    BinaryFunctorCrossProductPack<
        std::tuple<IntType, LongType, FloatType, DoubleType>,
        std::tuple<IntType, LongType, FloatType, DoubleType>,
        DivideFunctor, NumericTypeUnifier>,
// DatetimeInterval and YearMonthInterval
    BinaryFunctorCrossProductPack<
        std::tuple<DatetimeIntervalType, YearMonthIntervalType>,
        std::tuple<IntType, LongType, FloatType, DoubleType>,
        DivideFunctor, meta::PairSelectorLeft>
>;

using ModuloBinaryFunctorPack = FunctorPack<
// Numeric
    BinaryFunctorCrossProductPack<
        std::tuple<IntType, LongType, FloatType, DoubleType>,
        std::tuple<IntType, LongType, FloatType, DoubleType>,
        ModuloFunctor, NumericTypeUnifier>
>;

using ArithmeticBinaryFunctorPack = FunctorPack<
    AddBinaryFunctorPack,
    SubtractBinaryFunctorPack,
    MultiplyBinaryFunctorPack,
    DivideBinaryFunctorPack,
    ModuloBinaryFunctorPack
>;

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_ARITHMETIC_BINARY_FUNCTORS_HPP_
