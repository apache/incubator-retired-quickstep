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

#ifndef QUICKSTEP_UTILITY_META_TYPE_LIST_HPP_
#define QUICKSTEP_UTILITY_META_TYPE_LIST_HPP_

#include "utility/meta/Common.hpp"
#include "utility/meta/TypeListMetaFunctions.hpp"

namespace quickstep {
namespace meta {

/** \addtogroup Utility
 *  @{
 */

template <typename ...Ts>
class TypeList;

template <typename ...Ts>
class TypeListCommon {
 private:
  template <typename ...Tail> struct AppendHelper {
    using type = TypeList<Ts..., Tail...>;
  };

 public:
  static constexpr std::size_t length = sizeof...(Ts);

  using type = TypeList<Ts...>;

  template <template <typename ...> class Host>
  using bind_to = Host<Ts...>;

  template <std::size_t ...pos>
  using at = typename internal::ElementAtImpl<
      TypeList<Ts...>, TypeList<std::integral_constant<std::size_t, pos>...>>::type;

  template <typename T>
  using push_front = TypeList<T, Ts...>;

  template <typename T>
  using push_back = TypeList<Ts..., T>;

  template <typename T>
  using contains = EqualsAny<T, Ts...>;

  template <typename ...DumbT>
  using unique = typename internal::UniqueImpl<TypeList<>, TypeList<Ts...>, DumbT...>::type;

  template <typename TL>
  using append = typename TL::template bind_to<AppendHelper>::type;

  template <typename TL>
  using cartesian_product = typename internal::CartesianProductImpl<TypeList<Ts...>, TL>::type;

  template <typename Subtrahend>
  using subtract = typename internal::SubtractImpl<TypeList<>, TypeList<Ts...>, Subtrahend>::type;

  template <template <typename ...> class Op>
  using map = TypeList<typename Op<Ts>::type...>;

  template <template <typename ...> class Op>
  using flatmap = typename internal::FlatmapImpl<TypeList<>, TypeList<Ts...>, Op>::type;

  template <template <typename ...> class Op>
  using filter = typename internal::FilterImpl<TypeList<>, TypeList<Ts...>, Op>::type;

  template <template <typename ...> class Op>
  using filtermap = typename internal::FiltermapImpl<TypeList<>, TypeList<Ts...>, Op>::type;

  template <typename ...DumbT>
  using flatten_once = typename internal::FlattenOnceImpl<TypeList<>, TypeList<Ts...>, DumbT...>::type;

  template <typename TL>
  using zip = typename internal::ZipImpl<TypeList<>, TypeList<Ts...>, TL>::type;

  template <typename TL, template <typename ...> class Op>
  using zip_with = typename internal::ZipWithImpl<TypeList<>, TypeList<Ts...>, TL, Op>::type;

  template <typename T>
  using as_sequence = typename internal::AsSequenceImpl<T, Ts...>::type;
};

template <typename ...Ts>
class TypeList : public TypeListCommon<Ts...> {
 private:
  template <typename Head, typename ...Tail>
  struct HeadTailHelper {
    using head = Head;
    using tail = TypeList<Tail...>;
  };

 public:
  using head = typename HeadTailHelper<Ts...>::head;
  using tail = typename HeadTailHelper<Ts...>::tail;
};

template <>
class TypeList<> : public TypeListCommon<> {
};

/** @} */

}  // namespace meta
}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_META_TYPE_LIST_HPP_
