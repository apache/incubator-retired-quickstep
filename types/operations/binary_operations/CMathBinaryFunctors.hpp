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

#ifndef QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_CMATH_BINARY_FUNCTORS_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_CMATH_BINARY_FUNCTORS_HPP_

#include <cmath>
#include <string>

#include "types/DoubleType.hpp"
#include "types/FloatType.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/operations/binary_operations/BinaryOperationSynthesizer.hpp"
#include "types/operations/utility/OperationSynthesizeUtil.hpp"
#include "utility/meta/Common.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

template <typename LeftT, typename RightT, typename ResultT,
          typename ResultT::cpptype f(typename LeftT::cpptype,
                                      typename RightT::cpptype),
          typename FunctorNameT>
struct CMathBinaryFunctorWrapper {
  struct Implemenation : public BinaryFunctor<LeftT, RightT, ResultT> {
    inline typename ResultT::cpptype apply(
        const typename LeftT::cpptype &left,
        const typename RightT::cpptype &right) const {
      return f(left, right);
    }
    inline static std::string GetName() {
      return FunctorNameT::ToString();
    }
  };

  typedef Implemenation type;
};

template <typename LeftT, typename RightT, typename ResultT,
          typename ResultT::cpptype f(typename LeftT::cpptype,
                                      typename RightT::cpptype),
          typename FunctorNameT>
using CMathBinaryFunctor =
    typename CMathBinaryFunctorWrapper<LeftT, RightT, ResultT, f, FunctorNameT>::type;

using CMathBinaryFunctorPack = FunctorPack<
// pow
    CMathBinaryFunctor<FloatType, FloatType, FloatType,
                       std::pow, meta::StringLiteral<'p','o','w'>>,
    CMathBinaryFunctor<DoubleType, DoubleType, DoubleType,
                       std::pow, meta::StringLiteral<'p','o','w'>>
>;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_CMATH_BINARY_FUNCTORS_HPP_
