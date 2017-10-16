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

#ifndef QUICKSTEP_TYPES_NUMERIC_TYPE_SAFE_COERCIBILITY_HPP_
#define QUICKSTEP_TYPES_NUMERIC_TYPE_SAFE_COERCIBILITY_HPP_

#include "utility/meta/TMP.hpp"
#include "types/TypeRegistrar.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

template <typename LeftType, typename RightType>
using IsSafelyCoercible = meta::TypeList<LeftType, RightType>;

using NumericTypeSafeCoersionPartialOrder = meta::TypeList<
    IsSafelyCoercible<BoolType, IntType>,
    IsSafelyCoercible<IntType, FloatType>,
    IsSafelyCoercible<IntType, LongType>,
    IsSafelyCoercible<FloatType, DoubleType>,
    IsSafelyCoercible<LongType, DoubleType>
>;

using NumericTypeSafeCoersionClosure =
    meta::TransitiveClosure<NumericTypeSafeCoersionPartialOrder>;

template <typename LeftType, typename RightType>
struct NumericTypeSafeCoercibility {
  static constexpr bool value =
      NumericTypeSafeCoersionClosure::contains<
          IsSafelyCoercible<LeftType, RightType>>::value;
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_NUMERIC_TYPE_SAFE_COERCIBILITY_HPP_
