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

#ifndef QUICKSTEP_UTILITY_META_TRANSITIVE_CLOSURE_HPP_
#define QUICKSTEP_UTILITY_META_TRANSITIVE_CLOSURE_HPP_

#include "utility/meta/TypeList.hpp"

namespace quickstep {
namespace meta {

/** \addtogroup Meta
 *  @{
 */

template <typename Edges>
struct TransitiveClosure;


namespace internal {

template <typename TL, typename Enable = void>
struct EdgeMatcher {};

template <typename TL>
struct EdgeMatcher<
    TL,
    std::enable_if_t<std::is_same<typename TL::template at<0, 1>,
                                  typename TL::template at<1, 0>>::value>> {
  using type = TypeList<typename TL::template at<0, 0>,
                        typename TL::template at<1, 1>>;
};

template <typename LeftEdges, typename RightEdges>
struct JoinPath {
  using type = typename LeftEdges::template cartesian_product<RightEdges>
                                 ::template filtermap<EdgeMatcher>;
};

// Semi-naive
template <typename Out, typename WorkSet, typename Edges, typename Enable = void>
struct TransitiveClosureInner;

template <typename Out, typename WorkSet, typename Edges>
struct TransitiveClosureInner<Out, WorkSet, Edges,
                              std::enable_if_t<WorkSet::length == 0>> {
  using type = Out;
};

template <typename Out, typename WorkSet, typename Edges>
struct TransitiveClosureInner<Out, WorkSet, Edges,
                              std::enable_if_t<WorkSet::length != 0>>
    : TransitiveClosureInner<typename Out::template append<WorkSet>,
                             typename JoinPath<WorkSet, Edges>::type::template subtract<
                                 typename Out::template append<WorkSet>>::template unique<>,
                             Edges> {};

template <typename Edge>
struct TransitiveClosureInitializer {
  using type = TypeList<TypeList<typename Edge::head, typename Edge::head>,
                        TypeList<typename Edge::tail::head, typename Edge::tail::head>>;
};

template <typename Edges>
using TransitiveClosureHelper =
    typename TransitiveClosureInner<TypeList<>,
                                    typename Edges::template flatmap<
                                        TransitiveClosureInitializer>::template unique<>,
                                    Edges>::type;

}  // namespace internal

template <typename Edges>
struct TransitiveClosure : internal::TransitiveClosureHelper<Edges> {};

/** @} */

}  // namespace meta
}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_META_TRANSITIVE_CLOSURE_HPP_
