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

#ifndef QUICKSTEP_TYPES_TYPE_ID_SELECTORS_HPP_
#define QUICKSTEP_TYPES_TYPE_ID_SELECTORS_HPP_

#include <type_traits>

#include "types/TypeID.hpp"
#include "types/TypeRegistrar.hpp"
#include "utility/meta/Common.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

struct TypeIDSelectorAll;

struct TypeIDSelectorNumeric;

struct TypeIDSelectorParameterized;

struct TypeIDSelectorNonParameterized;

template <TypeID ...candidates>
struct TypeIDSelector;


// Forward declaration
template <TypeID type_id>
struct TypeIDTrait;

struct TypeIDSelectorAll {
  template <typename TypeIDConstant, typename FunctorT, typename EnableT = void>
  struct Implementation {
    inline static auto Invoke(const FunctorT &functor) {
      return functor(TypeIDConstant());
    }
  };
};

struct TypeIDSelectorNumeric {
  template <typename TypeIDConstant, typename FunctorT, typename EnableT = void>
  struct Implementation {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
    inline static auto Invoke(const FunctorT &functor)
        -> decltype(functor(TypeIDConstant())) {
      DLOG(FATAL) << "Unexpected TypeID: "
                  << kTypeNames[static_cast<int>(TypeIDConstant::value)];
    }
#pragma GCC diagnostic pop
  };
};

template <typename TypeIDConstant, typename FunctorT>
struct TypeIDSelectorNumeric::Implementation<
    TypeIDConstant, FunctorT,
    std::enable_if_t<TypeIDTrait<TypeIDConstant::value>
                         ::kStaticSuperTypeID == SuperTypeID::kNumeric>> {
  inline static auto Invoke(const FunctorT &functor) {
    return functor(TypeIDConstant());
  }
};

template <TypeID ...candidates>
struct TypeIDSelector {
  template <typename TypeIDConstant, typename FunctorT, typename EnableT = void>
  struct Implementation {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
    inline static auto Invoke(const FunctorT &functor)
        -> decltype(functor(TypeIDConstant())) {
      DLOG(FATAL) << "Unexpected TypeID: "
                  << kTypeNames[static_cast<int>(TypeIDConstant::value)];
    }
#pragma GCC diagnostic pop
  };
};

template <TypeID ...candidates>
template <typename TypeIDConstant, typename FunctorT>
struct TypeIDSelector<candidates...>::Implementation<
    TypeIDConstant, FunctorT,
    std::enable_if_t<
        meta::EqualsAny<TypeIDConstant,
                        std::integral_constant<TypeID, candidates>...>::value>> {
  inline static auto Invoke(const FunctorT &functor) {
    return functor(TypeIDConstant());
  }
};

template <MemoryLayout ...candidates>
struct TypeIDSelectorMemoryLayout {
  template <typename TypeIDConstant, typename FunctorT, typename EnableT = void>
  struct Implementation {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
    inline static auto Invoke(const FunctorT &functor)
        -> decltype(functor(TypeIDConstant())) {
      DLOG(FATAL) << "Unexpected TypeID: "
                  << kTypeNames[static_cast<int>(TypeIDConstant::value)];
    }
#pragma GCC diagnostic pop
  };
};

template <MemoryLayout ...candidates>
template <typename TypeIDConstant, typename FunctorT>
struct TypeIDSelectorMemoryLayout<candidates...>::Implementation<
    TypeIDConstant, FunctorT,
    std::enable_if_t<
        meta::EqualsAny<
            std::integral_constant<MemoryLayout,
                                   TypeIDTrait<TypeIDConstant::value>::kMemoryLayout>,
            std::integral_constant<MemoryLayout, candidates>...>::value>> {
  inline static auto Invoke(const FunctorT &functor) {
    return functor(TypeIDConstant());
  }
};

//namespace internal {
//
//template <bool require_parameterized>
//struct TypeIDSelectorParameterizedHelper {
//  template <typename TypeIDConstant, typename FunctorT, typename EnableT = void>
//  struct Implementation {
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wreturn-type"
//    inline static auto Invoke(const FunctorT &functor)
//        -> decltype(functor(TypeIDConstant())) {
//      DLOG(FATAL) << "Unexpected TypeID: "
//                  << kTypeNames[static_cast<int>(TypeIDConstant::value)];
//    }
//#pragma GCC diagnostic pop
//  };
//};
//
//template <bool require_non_parameterized>
//template <typename TypeIDConstant, typename FunctorT>
//struct TypeIDSelectorParameterizedHelper<require_non_parameterized>::Implementation<
//    TypeIDConstant, FunctorT,
//    std::enable_if_t<TypeIDTrait<TypeIDConstant::value>::kIsParPod
//                         ^ require_non_parameterized>> {
//  inline static auto Invoke(const FunctorT &functor) {
//    return functor(TypeIDConstant());
//  }
//};
//
//}  // namespace internal

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_TYPE_ID_SELECTORS_HPP_
