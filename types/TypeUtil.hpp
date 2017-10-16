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

#ifndef QUICKSTEP_TYPES_TYPE_UTIL_HPP_
#define QUICKSTEP_TYPES_TYPE_UTIL_HPP_

#include <type_traits>

#include "types/ArrayType.hpp"
#include "types/BoolType.hpp"
#include "types/CharType.hpp"
#include "types/DateType.hpp"
#include "types/DatetimeIntervalType.hpp"
#include "types/DatetimeType.hpp"
#include "types/DoubleType.hpp"
#include "types/FloatType.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/MetaType-decl.hpp"
#include "types/NullType.hpp"
#include "types/TextType.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypeRegistrar.hpp"
#include "types/VarCharType.hpp"
#include "types/YearMonthIntervalType.hpp"
#include "utility/Macros.hpp"
#include "utility/meta/TypeList.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

namespace internal {

// TODO(refactor-type): Organize selector predicates, refactor the old design.
template <MemoryLayout layout>
struct MemoryLayoutSelector {
  template <typename TypeIDConstant>
  struct type {
    static constexpr bool value =
        TypeIDTrait<TypeIDConstant::value>::kMemoryLayout == layout;
  };
};

}  // namespace internal


class TypeUtil {
 public:
  template <MemoryLayout layout>
  using TypeIDSequenceMemoryLayout =
      TypeIDSequenceAll::bind_to<meta::TypeList>
                       ::filter<internal::MemoryLayoutSelector<kCxxInlinePod>::type>
                       ::as_sequence<TypeID>;

  static MemoryLayout GetMemoryLayout(const TypeID type_id) {
    return InvokeOnTypeID(
        type_id,
        [&](auto tid) -> MemoryLayout {  // NOLINT(build/c++11)
      return TypeIDTrait<decltype(tid)::value>::kMemoryLayout;
    });
  }

  template <MemoryLayout layout>
  static std::vector<TypeID> GetTypeIDVectorOfMemoryLayout(){
    return TypeIDSequenceMemoryLayout<layout>::Instantiate<std::vector<TypeID>>();
  }

  static bool IsParameterizedPod(const TypeID type_id) {
    return InvokeOnTypeID(
        type_id,
        [&](auto tid) -> bool {  // NOLINT(build/c++11)
      return TypeIDTrait<decltype(tid)::value>::kIsParPod;
    });
  }

 private:
  TypeUtil() {}

  DISALLOW_COPY_AND_ASSIGN(TypeUtil);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_TYPE_UTIL_HPP_
