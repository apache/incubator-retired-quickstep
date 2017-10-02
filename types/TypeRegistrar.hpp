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

#ifndef QUICKSTEP_TYPES_TYPE_REGISTRAR_HPP_
#define QUICKSTEP_TYPES_TYPE_REGISTRAR_HPP_

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <vector>

#include "types/DatetimeLit.hpp"
#include "types/IntervalLit.hpp"
#include "types/NullLit.hpp"
#include "types/TypeID.hpp"
#include "types/TypeIDSelectors.hpp"
#include "utility/meta/Common.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

class Type;
class TypedValue;

using UntypedLiteral = void;

using ArrayLiteral = std::vector<UntypedLiteral*>;
using MetaTypeLiteral = const Type*;

template <TypeID type_id>
struct TypeIDTrait;

#define REGISTER_TYPE(type_class, type_id, super_type_id, memory_layout, cpp_type) \
  class type_class; \
  template <> struct TypeIDTrait<type_id> { \
    typedef type_class TypeClass; \
    typedef cpp_type cpptype; \
    static constexpr TypeID kStaticTypeID = type_id; \
    static constexpr SuperTypeID kStaticSuperTypeID = super_type_id; \
    static constexpr MemoryLayout kMemoryLayout = memory_layout; \
    static constexpr bool kIsParPod = \
        (memory_layout == kParInlinePod || memory_layout == kParOutOfLinePod); \
  };

REGISTER_TYPE(BoolType, kBool,
              SuperTypeID::kNumeric, kCxxInlinePod, bool);
REGISTER_TYPE(IntType, kInt,
              SuperTypeID::kNumeric, kCxxInlinePod, int);
REGISTER_TYPE(LongType, kLong,
              SuperTypeID::kNumeric, kCxxInlinePod, std::int64_t);
REGISTER_TYPE(FloatType, kFloat,
              SuperTypeID::kNumeric, kCxxInlinePod, float);
REGISTER_TYPE(DoubleType, kDouble,
              SuperTypeID::kNumeric, kCxxInlinePod, double);
REGISTER_TYPE(DateType, kDate,
              SuperTypeID::kOther, kCxxInlinePod, DateLit);
REGISTER_TYPE(DatetimeType, kDatetime,
              SuperTypeID::kOther, kCxxInlinePod, DatetimeLit);
REGISTER_TYPE(DatetimeIntervalType, kDatetimeInterval,
              SuperTypeID::kOther, kCxxInlinePod, DatetimeIntervalLit);
REGISTER_TYPE(YearMonthIntervalType, kYearMonthInterval,
              SuperTypeID::kOther, kCxxInlinePod, YearMonthIntervalLit);
REGISTER_TYPE(CharType, kChar,
              SuperTypeID::kAsciiString, kParInlinePod, TypedValue);
REGISTER_TYPE(VarCharType, kVarChar,
              SuperTypeID::kAsciiString, kParOutOfLinePod, TypedValue);
REGISTER_TYPE(ArrayType, kArray,
              SuperTypeID::kOther, kCxxGeneric, ArrayLiteral);
REGISTER_TYPE(MetaType, kMetaType,
              SuperTypeID::kOther, kCxxGeneric, MetaTypeLiteral);
REGISTER_TYPE(NullType, kNullType,
              SuperTypeID::kOther, kCxxInlinePod, NullLit);

#undef REGISTER_TYPE

using TypeIDSequenceAll =
    meta::MakeSequence<static_cast<std::size_t>(kNumTypeIDs)>
        ::type::template cast_to<TypeID>;

template <typename Selector = TypeIDSelectorAll, typename FunctorT>
auto InvokeOnTypeID(const TypeID type_id, const FunctorT &functor);

namespace internal {

template <int l, int r, typename Selector, typename FunctorT>
inline auto InvokeOnTypeIDInternal(const int value,
                                   const FunctorT &functor) {
  DCHECK_LE(l, r);
  if (l == r) {
    constexpr TypeID type_id = static_cast<TypeID>(r);
    return Selector::template Implementation<
        std::integral_constant<TypeID, type_id>, FunctorT>::Invoke(functor);
  }
  constexpr int m = (l + r) >> 1;
  if (value <= m) {
    return InvokeOnTypeIDInternal<l, m, Selector, FunctorT>(value, functor);
  } else {
    return InvokeOnTypeIDInternal<m+1, r, Selector, FunctorT>(value, functor);
  }
}

}  // namespace internal

template <typename Selector, typename FunctorT>
auto InvokeOnTypeID(const TypeID type_id,
                    const FunctorT &functor) {
  return internal::InvokeOnTypeIDInternal<0, static_cast<int>(kNumTypeIDs)-1,
                                          Selector, FunctorT>(
      static_cast<int>(type_id), functor);
}

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_TYPE_REGISTRAR_HPP_
