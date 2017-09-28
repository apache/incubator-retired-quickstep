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

#include <cstdint>
#include <type_traits>

#include "types/DatetimeLit.hpp"
#include "types/IntervalLit.hpp"
#include "types/NullLit.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypeIDSelectors.hpp"
#include "utility/meta/Common.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

template <TypeID type_id>
struct TypeIDTrait;

#define REGISTER_TYPE(type_class, type_id, super_type_id, memory_layout, cpp_type) \
  class type_class; \
  template <> struct TypeIDTrait<type_id> { \
    typedef type_class TypeClass; \
    typedef cpp_type cpptype; \
    static constexpr TypeID kStaticTypeID = type_id; \
    static constexpr Type::SuperTypeID kStaticSuperTypeID = super_type_id; \
    static constexpr MemoryLayout kMemoryLayout = memory_layout; \
    static constexpr bool kIsParameterizedPod = \
        (memory_layout == kParNativePod || memory_layout == kParIndirectPod); \
  };

REGISTER_TYPE(BoolType, kBool, \
              Type::kNumeric, kCxxNativePod, bool);
REGISTER_TYPE(IntType, kInt, \
              Type::kNumeric, kCxxNativePod, int);
REGISTER_TYPE(LongType, kLong, \
              Type::kNumeric, kCxxNativePod, std::int64_t);
REGISTER_TYPE(FloatType, kFloat, \
              Type::kNumeric, kCxxNativePod, float);
REGISTER_TYPE(DoubleType, kDouble, \
              Type::kNumeric, kCxxNativePod, double);
REGISTER_TYPE(DateType, kDate, \
              Type::kOther, kCxxNativePod, DateLit);
REGISTER_TYPE(DatetimeType, kDatetime, \
              Type::kOther, kCxxNativePod, DatetimeLit);
REGISTER_TYPE(DatetimeIntervalType, kDatetimeInterval, \
              Type::kOther, kCxxNativePod, DatetimeIntervalLit);
REGISTER_TYPE(YearMonthIntervalType, kYearMonthInterval, \
              Type::kOther, kCxxNativePod, YearMonthIntervalLit);
REGISTER_TYPE(CharType, kChar, \
              Type::kAsciiString, kParNativePod, void);
REGISTER_TYPE(VarCharType, kVarChar, \
              Type::kAsciiString, kParIndirectPod, void);
REGISTER_TYPE(NullType, kNullType, \
              Type::kOther, kCxxNativePod, NullLit);

#undef REGISTER_TYPE

using TypeIDSequenceAll =
    meta::MakeSequence<static_cast<std::size_t>(kNumTypeIDs)>
        ::type::template cast_to<TypeID>;

template <typename Selector = TypeIDSelectorAll, typename FunctorT>
auto InvokeOnTypeID(const TypeID type_id, const FunctorT &functor);

namespace internal {

template <int l, int r, typename Selector, typename FunctorT>
inline auto InvokeOnTypeIDInner(const int value,
                                const FunctorT &functor) {
  DCHECK_LE(l, r);
  if (l == r) {
    constexpr TypeID type_id = static_cast<TypeID>(r);
    return Selector::template Implementation<
        std::integral_constant<TypeID, type_id>, FunctorT>::Invoke(functor);
  }
  constexpr int m = (l + r) >> 1;
  if (value <= m) {
    return InvokeOnTypeIDInner<l, m, Selector, FunctorT>(value, functor);
  } else {
    return InvokeOnTypeIDInner<m+1, r, Selector, FunctorT>(value, functor);
  }
}

}  // namespace internal

template <typename Selector, typename FunctorT>
auto InvokeOnTypeID(const TypeID type_id,
                    const FunctorT &functor) {
  return internal::InvokeOnTypeIDInner<0, static_cast<int>(kNumTypeIDs)-1,
                                       Selector, FunctorT>(
      static_cast<int>(type_id), functor);
}

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_TYPE_REGISTRAR_HPP_
