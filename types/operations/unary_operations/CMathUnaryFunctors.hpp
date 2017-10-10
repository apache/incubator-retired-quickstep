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

#ifndef QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_CMATH_UNARY_FUNCTORS_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_CMATH_UNARY_FUNCTORS_HPP_

#include <cmath>
#include <string>

#include "types/DoubleType.hpp"
#include "types/FloatType.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/operations/unary_operations/UnaryOperationSynthesizer.hpp"
#include "types/operations/utility/OperationSynthesizeUtil.hpp"
#include "utility/meta/Common.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

template <typename ArgumentT, typename ResultT,
          typename ResultT::cpptype f(typename ArgumentT::cpptype),
          typename FunctorNameT>
struct CMathUnaryFunctorWrapper {
  struct Implemenation : public UnaryFunctor<ArgumentT, ResultT> {
    inline typename ResultT::cpptype apply(
        const typename ArgumentT::cpptype &argument) const {
      return f(argument);
    }
    inline static std::string GetName() {
      return FunctorNameT::ToString();
    }
  };

  typedef Implemenation type;
};

template <typename ArgumentT, typename ResultT,
          typename ResultT::cpptype f(typename ArgumentT::cpptype),
          typename FunctorNameT>
using CMathUnaryFunctor =
    typename CMathUnaryFunctorWrapper<ArgumentT, ResultT, f, FunctorNameT>::type;

inline std::int64_t CMathRound(const float arg) {
  return std::llround(arg);
}
inline std::int64_t CMathRound(const double arg) {
  return std::llround(arg);
}

using CMathUnaryFunctorPack = FunctorPack<
// abs
    CMathUnaryFunctor<IntType, IntType,
                      std::abs, meta::StringLiteral<'a','b','s'>>,
    CMathUnaryFunctor<LongType, LongType,
                      std::abs, meta::StringLiteral<'a','b','s'>>,
    CMathUnaryFunctor<FloatType, FloatType,
                      std::fabs, meta::StringLiteral<'a','b','s'>>,
    CMathUnaryFunctor<DoubleType, DoubleType,
                      std::fabs, meta::StringLiteral<'a','b','s'>>,
// sqrt
    CMathUnaryFunctor<FloatType, FloatType,
                      std::sqrt, meta::StringLiteral<'s','q','r','t'>>,
    CMathUnaryFunctor<DoubleType, DoubleType,
                      std::sqrt, meta::StringLiteral<'s','q','r','t'>>,
// exp
    CMathUnaryFunctor<FloatType, FloatType,
                      std::exp, meta::StringLiteral<'e','x','p'>>,
    CMathUnaryFunctor<DoubleType, DoubleType,
                      std::exp, meta::StringLiteral<'e','x','p'>>,
// log
    CMathUnaryFunctor<FloatType, FloatType,
                      std::log, meta::StringLiteral<'l','o','g'>>,
    CMathUnaryFunctor<DoubleType, DoubleType,
                      std::log, meta::StringLiteral<'l','o','g'>>,
// ceil
    CMathUnaryFunctor<FloatType, FloatType,
                      std::ceil, meta::StringLiteral<'c','e','i','l'>>,
    CMathUnaryFunctor<DoubleType, DoubleType,
                      std::ceil, meta::StringLiteral<'c','e','i','l'>>,
// floor
    CMathUnaryFunctor<FloatType, FloatType,
                      std::floor, meta::StringLiteral<'f','l','o','o','r'>>,
    CMathUnaryFunctor<DoubleType, DoubleType,
                      std::floor, meta::StringLiteral<'f','l','o','o','r'>>,
// round
    CMathUnaryFunctor<FloatType, LongType,
                      CMathRound, meta::StringLiteral<'r','o','u','n','d'>>,
    CMathUnaryFunctor<DoubleType, LongType,
                      CMathRound, meta::StringLiteral<'r','o','u','n','d'>>
>;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_CMATH_UNARY_FUNCTORS_HPP_
