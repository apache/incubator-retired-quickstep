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

#ifndef QUICKSTEP_UTILITY_TEMPLATE_UTIL_HPP_
#define QUICKSTEP_UTILITY_TEMPLATE_UTIL_HPP_

#include <cstddef>
#include <tuple>
#include <utility>

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

namespace template_util_inner {

/**
 * @brief Represents a compile-time sequence of integers.
 *
 * Sequence is defined here for C++11 compatibility. For C++14 and above,
 * std::integer_sequence can be used to achieve the same functionality.
 *
 * TODO(jianqiao): directly use std::integer_sequence if having C++14 support.
 */
template<std::size_t ...>
struct Sequence {};

/**
 * @brief The helper class for creating Sequence. MakeSequence<N>::type is
 *        equivalent to Sequence<1,2,...,N>.
 *
 * MakeSequence is defined here for C++11 compatibility. For C++14 and above,
 * std::make_index_sequence can be used to achieve the same functionality.
 *
 * TODO(jianqiao): directly use std::make_index_sequence if having C++14 support.
 */
template<std::size_t N, std::size_t ...S>
struct MakeSequence : MakeSequence<N-1, N-1, S...> {};

template<std::size_t ...S>
struct MakeSequence<0, S...> {
  typedef Sequence<S...> type;
};

/**
 * @brief Final step of CreateBoolInstantiatedInstance. Now all bool_values are
 *        ready. Instantiate the template and create (i.e. new) an instance.
 */
template <template <bool ...> class T, class ReturnT,
          bool ...bool_values, std::size_t ...i,
          typename Tuple>
inline ReturnT* CreateBoolInstantiatedInstanceInner(Tuple &&args,
                                                    Sequence<i...> &&indices) {
  return new T<bool_values...>(std::get<i>(std::forward<Tuple>(args))...);
}

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
inline auto InvokeOnBoolsInner(TupleT &&args, Sequence<i...> &&indices) {
  return InvokeOnBoolsInner(std::get<last>(std::forward<TupleT>(args)),
                            std::get<i>(std::forward<TupleT>(args))...);
}

}  // namespace template_util_inner

/**
 * @brief Edge case of the recursive CreateBoolInstantiatedInstance function
 *        when all bool variables have been branched and replaced with compile-time
 *        bool constants.
 */
template <template <bool ...> class T, class ReturnT,
          bool ...bool_values,
          typename Tuple>
inline ReturnT* CreateBoolInstantiatedInstance(Tuple &&args) {
  // Note that the constructor arguments have been forwarded as a tuple (args).
  // Here we generate a compile-time index sequence (i.e. typename MakeSequence<n_args>::type())
  // for the tuple, so that the tuple can be unpacked as a sequence of constructor
  // parameters in CreateBoolInstantiatedInstanceInner.
  constexpr std::size_t n_args = std::tuple_size<Tuple>::value;
  return template_util_inner::CreateBoolInstantiatedInstanceInner<
      T, ReturnT, bool_values...>(
          std::forward<Tuple>(args),
          typename template_util_inner::MakeSequence<n_args>::type());
}

/**
 * @brief A helper function for creating bool branched templates.
 *
 * The scenario for using this helper function is that, suppose we have a class
 * where all template parameters are bools:
 * --
 * template <bool c1, bool c2, bool c3>
 * class SomeClass : public BaseClass {
 *   // This simple function will be invoked in computationally-intensive loops.
 *   inline SomeType someSimpleFunction(...) {
 *     if (c1) {
 *       doSomeThing1();
 *     }
 *     if (c2) {
 *       doSomeThing2();
 *     }
 *     if (c3) {
 *       doSomeThing3();
 *     }
 *   }
 * };
 * --
 * Typically, this bool-paramterized template is for performance consideration.
 * That is, we would like to make a copy of code for each configuration of bool
 * values, so that there will be no branchings in someSimpleFunction().
 *
 * The problem is that, to conditionally instantiate the template, given bool
 * variables c1, c2, c3, we have to do something like this:
 * --
 * if (c1) {
 *   if (c2) {
 *     if (c3) {
 *       return new SomeClass<true, true, true>(some_args...);
 *     } else {
 *       return new SomeClass<true, true, false>(some_args...);
 *     }
 *   } else {
 *     if (c3) {
 *       return new SomeClass<true, false, true>(some_args...);
 *     } else {
 *       return new SomeClass<true, false, false>(some_args...);
 *     }
 * } else {
 *   ...
 * }
 * --
 * Then there will be power(2,N) branches if the template has N bool parameters,
 * making it tedious to do the instantiating.
 *
 * Now, this helper function can achieve the branched instantiation in one
 * statement as:
 * --
 * return CreateBoolInstantiatedInstance<SomeClass,BaseClass>(
 *     std::forward_as_tuple(some_args...), c1, c2, c3);
 * --
 */
template <template <bool ...> class T, class ReturnT,
          bool ...bool_values, typename ...Bools,
          typename Tuple>
inline ReturnT* CreateBoolInstantiatedInstance(Tuple &&args,
                                               const bool tparam,
                                               const Bools ...rest_tparams) {
  if (tparam) {
    return CreateBoolInstantiatedInstance<T, ReturnT, bool_values..., true>(
        std::forward<Tuple>(args), rest_tparams...);
  } else {
    return CreateBoolInstantiatedInstance<T, ReturnT, bool_values..., false>(
        std::forward<Tuple>(args), rest_tparams...);
  }
}

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
inline auto InvokeOnBools(ArgTypes ...args) {
  constexpr std::size_t last = sizeof...(args) - 1;
  return template_util_inner::InvokeOnBoolsInner<last>(
      std::forward_as_tuple(args...),
      typename template_util_inner::MakeSequence<last>::type());
}

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_TEMPLATE_UTIL_HPP_
