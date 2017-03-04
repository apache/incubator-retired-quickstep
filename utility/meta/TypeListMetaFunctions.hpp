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

#ifndef QUICKSTEP_UTILITY_META_TYPE_LIST_META_FUNCTIONS_HPP_
#define QUICKSTEP_UTILITY_META_TYPE_LIST_META_FUNCTIONS_HPP_

#include "utility/meta/Common.hpp"

namespace quickstep {
namespace meta {

/** \addtogroup Utility
 *  @{
 */

template <typename ...Ts>
class TypeList;

namespace internal {

template <typename TL, typename PosTL, typename Enable = void>
struct ElementAtImpl;

template <typename TL, typename PosTL>
struct ElementAtImpl<TL, PosTL,
                     std::enable_if_t<PosTL::length == 0>> {
  using type = TL;
};

template <typename TL, typename PosTL>
struct ElementAtImpl<TL, PosTL,
                     std::enable_if_t<PosTL::length != 0>>
    : ElementAtImpl<typename std::tuple_element<
                        PosTL::head::value,
                        typename TL::template bind_to<std::tuple>>::type,
                    typename PosTL::tail> {};


template <typename Out, typename Rest, typename Enable = void>
struct UniqueImpl;

template <typename Out, typename Rest>
struct UniqueImpl<Out, Rest,
                  std::enable_if_t<Rest::length == 0>> {
  using type = Out;
};

template <typename Out, typename Rest>
struct UniqueImpl<Out, Rest,
                  std::enable_if_t<Out::template contains<typename Rest::head>::value>>
    : UniqueImpl<Out, typename Rest::tail> {};

template <typename Out, typename Rest>
struct UniqueImpl<Out, Rest,
                  std::enable_if_t<!Out::template contains<typename Rest::head>::value>>
    : UniqueImpl<typename Out::template push_back<typename Rest::head>,
                 typename Rest::tail> {};


template <typename Out, typename Rest, typename Subtrahend, typename Enable = void>
struct SubtractImpl;

template <typename Out, typename Rest, typename Subtrahend>
struct SubtractImpl<Out, Rest, Subtrahend,
                    std::enable_if_t<Rest::length == 0>> {
  using type = Out;
};

template <typename Out, typename Rest, typename Subtrahend>
struct SubtractImpl<Out, Rest, Subtrahend,
                    std::enable_if_t<Subtrahend::template contains<
                        typename Rest::head>::value>>
    : SubtractImpl<Out, typename Rest::tail, Subtrahend> {};

template <typename Out, typename Rest, typename Subtrahend>
struct SubtractImpl<Out, Rest, Subtrahend,
                    std::enable_if_t<!Subtrahend::template contains<
                        typename Rest::head>::value>>
    : SubtractImpl<typename Out::template push_back<typename Rest::head>,
                   typename Rest::tail, Subtrahend> {};


template <typename LeftTL, typename RightTL>
struct CartesianProductImpl {
  template <typename LeftT>
  struct LeftHelper {
    template <typename RightT>
    struct RightHelper {
      using type = TypeList<LeftT, RightT>;
    };
    using type = typename RightTL::template map<RightHelper>;
  };
  using type = typename LeftTL::template flatmap<LeftHelper>;
};


template <typename Out, typename Rest, template <typename ...> class Op,
          typename Enable = void>
struct FlatmapImpl;

template <typename Out, typename Rest, template <typename ...> class Op>
struct FlatmapImpl<Out, Rest, Op,
                   std::enable_if_t<Rest::length == 0>> {
  using type = Out;
};

template <typename Out, typename Rest, template <typename ...> class Op>
struct FlatmapImpl<Out, Rest, Op,
                   std::enable_if_t<Rest::length != 0>>
    : FlatmapImpl<typename Out::template append<typename Op<typename Rest::head>::type>,
                  typename Rest::tail, Op> {};


template <typename Out, typename Rest, template <typename ...> class Op,
          typename Enable = void>
struct FilterImpl;

template <typename Out, typename Rest, template <typename ...> class Op>
struct FilterImpl<Out, Rest, Op,
                  std::enable_if_t<Rest::length == 0>> {
  using type = Out;
};

template <typename Out, typename Rest, template <typename ...> class Op>
struct FilterImpl<Out, Rest, Op,
                  std::enable_if_t<Op<typename Rest::head>::value>>
    : FilterImpl<typename Out::template push_back<typename Rest::head>,
                 typename Rest::tail, Op> {};

template <typename Out, typename Rest, template <typename ...> class Op>
struct FilterImpl<Out, Rest, Op,
                  std::enable_if_t<!Op<typename Rest::head>::value>>
    : FilterImpl<Out, typename Rest::tail, Op> {};


template <typename Out, typename Rest, template <typename ...> class Op,
          typename Enable = void>
struct FiltermapImpl;

template <typename Out, typename Rest, template <typename ...> class Op>
struct FiltermapImpl<Out, Rest, Op,
                     std::enable_if_t<Rest::length == 0>> {
  using type = Out;
};

template <typename Out, typename Rest, template <typename ...> class Op>
struct FiltermapImpl<Out, Rest, Op,
                     std::enable_if_t<Rest::length != 0 &&
                                      IsTrait<Op<typename Rest::head>>::value>>
    : FiltermapImpl<typename Out::template push_back<typename Op<typename Rest::head>::type>,
                    typename Rest::tail, Op> {};

template <typename Out, typename Rest, template <typename ...> class Op>
struct FiltermapImpl<Out, Rest, Op,
                     std::enable_if_t<Rest::length != 0 &&
                                      !IsTrait<Op<typename Rest::head>>::value>>
    : FiltermapImpl<Out, typename Rest::tail, Op> {};


template <typename Out, typename Rest, typename Enable = void>
struct FlattenOnceImpl;

template <typename Out, typename Rest>
struct FlattenOnceImpl<Out, Rest,
                       std::enable_if_t<Rest::length == 0>> {
  using type = Out;
};

template <typename Out, typename Rest>
struct FlattenOnceImpl<Out, Rest,
                       std::enable_if_t<Rest::length != 0>>
    : FlattenOnceImpl<typename Out::template append<typename Rest::head>,
                      typename Rest::tail> {};


template <typename Out, typename RestL, typename RestR, typename Enable = void>
struct ZipImpl;

template <typename Out, typename RestL, typename RestR>
struct ZipImpl<Out, RestL, RestR,
               std::enable_if_t<RestL::length == 0 || RestR::length == 0>> {
  static_assert(RestL::length == 0 && RestR::length == 0,
                "Zip failed: TypeLists have unequal lengths");
  using type = Out;
};

template <typename Out, typename RestL, typename RestR>
struct ZipImpl<Out, RestL, RestR,
               std::enable_if_t<RestL::length != 0 && RestR::length != 0>>
    : ZipImpl<typename Out::template push_back<
                  TypeList<typename RestL::head, typename RestR::head>>,
              typename RestL::tail, typename RestR::tail> {};


template <typename Out, typename RestL, typename RestR,
          template <typename ...> class Op, typename Enable = void>
struct ZipWithImpl;

template <typename Out, typename RestL, typename RestR,
          template <typename ...> class Op>
struct ZipWithImpl<Out, RestL, RestR, Op,
                   std::enable_if_t<RestL::length == 0 || RestR::length == 0>> {
  static_assert(RestL::length == 0 && RestR::length == 0,
                "ZipWith failed: TypeLists have unequal lengths");
  using type = Out;
};

template <typename Out, typename RestL, typename RestR,
          template <typename ...> class Op>
struct ZipWithImpl<Out, RestL, RestR, Op,
                   std::enable_if_t<RestL::length != 0 && RestR::length != 0>>
    : ZipWithImpl<typename Out::template push_back<
                      typename Op<typename RestL::head, typename RestR::head>::type>,
                  typename RestL::tail, typename RestR::tail, Op> {};


template <typename T, typename ...Ts>
struct AsSequenceImpl {
  using type = Sequence<T, static_cast<T>(Ts::value)...>;
};

}  // namespace internal

/** @} */

}  // namespace meta
}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_META_TYPE_LIST_META_FUNCTIONS_HPP_
