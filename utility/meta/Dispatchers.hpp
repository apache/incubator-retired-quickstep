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

#ifndef QUICKSTEP_UTILITY_META_DISPATCHERS_HPP_
#define QUICKSTEP_UTILITY_META_DISPATCHERS_HPP_

#include "utility/meta/Common.hpp"

namespace quickstep {
namespace meta {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief A helper function for bool branched template specialization.
 *
 * Usage example:
 * --
 * bool c1 = true, c2 = false;
 *
 * InvokeOnBools(
 *     c1, c2,
 *     [&](auto c1, auto c2) -> SomeBaseClass* {
 *   using T1 = decltype(c1);  // T1 == std::true_type
 *   using T2 = decltype(c2);  // T2 == std::false_type
 *
 *   constexpr bool cv1 = T1::value;  // cv1 == true
 *   constexpr bool cv2 = T2::value;  // cv2 == false
 *
 *   SomeFunction<cv1, cv2>(...);
 *   return new SomeClass<cv1, cv2>(...);
 * });
 * --
 */
template <typename ...ArgTypes>
inline auto InvokeOnBools(ArgTypes ...args);


namespace internal {

/**
 * @brief Invoke the functor with the compile-time bool values wrapped as
 *        integral_constant types.
 */
template <typename FunctorT, bool ...bool_values>
inline auto InvokeOnBoolsInner(const FunctorT &functor) {
  return functor(std::integral_constant<bool, bool_values>()...);
}

/**
 * @brief Recursive dispatching.
 */
template <typename FunctorT, bool ...bool_values, typename ...Bools>
inline auto InvokeOnBoolsInner(const FunctorT &functor,
                               const bool tparam,
                               const Bools ...rest_params) {
  if (tparam) {
    return InvokeOnBoolsInner<FunctorT, bool_values..., true>(
        functor, rest_params...);
  } else {
    return InvokeOnBoolsInner<FunctorT, bool_values..., false>(
        functor, rest_params...);
  }
}

/**
 * @brief Move the functor to the first position in argument list.
 */
template <std::size_t last, std::size_t ...i, typename TupleT>
inline auto InvokeOnBoolsInner(TupleT &&args, IntegerSequence<i...> &&indices) {
  return InvokeOnBoolsInner(std::get<last>(std::forward<TupleT>(args)),
                            std::get<i>(std::forward<TupleT>(args))...);
}

}  // namespace internal

template <typename ...ArgTypes>
inline auto InvokeOnBools(ArgTypes ...args) {
  constexpr std::size_t last = sizeof...(args) - 1;
  return internal::InvokeOnBoolsInner<last>(std::forward_as_tuple(args...),
                                            typename MakeSequence<last>::type());
}

/** @} */

}  // namespace meta
}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_META_DISPATCHERS_HPP_
