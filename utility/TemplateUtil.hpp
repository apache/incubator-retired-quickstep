/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *   University of Wisconsin—Madison.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
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

/**
 * @brief Represents a compile-time sequence of integers.
 *
 * Seq is defined here for C++11 compatibility. For C++14 and above,
 * std::integer_sequence can be used to achieve the same functionality.
 *
 * TODO(jianqiao): directly use std::integer_sequence if having C++14 support.
 */
template<std::size_t ...>
struct Sequence {};

/**
 * @brief The helper class for creating Seq. GenerateSequence<N>::type is equivalent to
 *        Sequence<1,2,...,N>.
 *
 * GenSeq is defined here for C++11 compatibility. For C++14 and above,
 * std::make_index_sequence can be used to achieve the same functionality.
 *
 * TODO(jianqiao): directly use std::make_index_sequence if having C++14 support.
 */
template<std::size_t N, std::size_t ...S>
struct GenerateSequence : GenerateSequence<N-1, N-1, S...> {};

template<std::size_t ...S>
struct GenerateSequence<0, S...> {
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
                                                    GenerateSequence<i...> &&indices) {
  return new T<bool_values...>(std::get<i>(std::forward<Tuple>(args))...);
}

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
  // Here we generate a compile-time index sequence (i.e. typename GenSeq<n_args>::type())
  // for the tuple, so that the tuple can be unpacked as a sequence of constructor
  // parameters in CreateBoolInstantiatedInstanceInner.
  constexpr std::size_t n_args = std::tuple_size<Tuple>::value;
  return CreateBoolInstantiatedInstanceInner<T, ReturnT, bool_values...>(
      std::forward<Tuple>(args), typename GenerateSequence<n_args>::type());
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

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_TEMPLATE_UTIL_HPP_
