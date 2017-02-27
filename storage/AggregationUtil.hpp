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

#ifndef QUICKSTEP_STORAGE_AGGREGATION_UTIL_HPP_
#define QUICKSTEP_STORAGE_AGGREGATION_UTIL_HPP_

#include <type_traits>

#include "expressions/aggregation/AggregationID.hpp"
#include "expressions/aggregation/AggFunc.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorMultiplexer.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/TypeID.hpp"
#include "types/Type.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Storage
 *  @{
 */

template <typename T>
using remove_const_reference_t = std::remove_const_t<std::remove_reference_t<T>>;

template <typename FunctorT>
inline auto InvokeOnKeyType(const Type &type,
                            const FunctorT &functor) {
  switch (type.getTypeID()) {
    case TypeID::kInt:
      return functor(static_cast<const IntType&>(type));
    case TypeID::kLong:
      return functor(static_cast<const LongType&>(type));
    default:
      LOG(FATAL) << "Not supported";
  }
}

template <typename FunctorT>
inline auto InvokeOnType(const Type &type,
                         const FunctorT &functor) {
  switch (type.getTypeID()) {
    case TypeID::kInt:
      return functor(static_cast<const IntType&>(type));
    case TypeID::kLong:
      return functor(static_cast<const LongType&>(type));
    case TypeID::kFloat:
      return functor(static_cast<const FloatType&>(type));
    case TypeID::kDouble:
      return functor(static_cast<const DoubleType&>(type));
    default:
      LOG(FATAL) << "Not supported";
  }
}

template <typename FunctorT>
inline auto InvokeOnBool(const bool &val,
                         const FunctorT &functor) {
  if (val) {
    return functor(std::true_type());
  } else {
    return functor(std::false_type());
  }
}

template <typename FunctorT>
inline auto InvokeOnBools(const bool &val1,
                          const bool &val2,
                          const FunctorT &functor) {
  if (val1) {
    if (val2) {
      return functor(std::true_type(), std::true_type());
    } else {
      return functor(std::true_type(), std::false_type());
    }
  } else {
    if (val2) {
      return functor(std::false_type(), std::true_type());
    } else {
      return functor(std::false_type(), std::false_type());
    }
  }
}

template <typename FunctorT>
inline auto InvokeOnAggFunc(const AggregationID &agg_id,
                            const FunctorT &functor) {
  switch (agg_id) {
    case AggregationID::kSum: {
      return functor(Sum());
    }
    default:
      LOG(FATAL) << "Not supported";
  }
}

template <typename FunctorT>
inline auto InvokeIf(const std::true_type &val,
                     const FunctorT &functor) {
  return functor();
}

template <typename FunctorT>
inline void InvokeIf(const std::false_type &val,
                     const FunctorT &functor) {
}

//template <typename FunctorT>
//inline void InvokeOnAggFuncIfApplicableToArgType(
//    const AggregationID &agg_id,
//    const Type &arg_type,
//    const FunctorT &functor) {
//  InvokeOnAggFunc(
//      agg_id,
//      [&](const auto &agg_func) -> void {
//    InvokeOnType(
//        arg_type,
//        [&](const auto &arg_type) -> void {
//      using AggFuncT = std::remove_reference_t<decltype(agg_func)>;
//      using ArgT = remove_const_reference_t<decltype(arg_type)>;
//
//      InvokeIf(
//          typename AggFuncT::template HasAtomicImpl<ArgT>(),
//          [&]() -> void {
//        functor(agg_func, arg_type);
//      });
//    });
//  });
//}

template <typename FunctorT>
inline void InvokeOnAggFuncWithArgType(
    const AggregationID &agg_id,
    const Type &arg_type,
    const FunctorT &functor) {
  InvokeOnAggFunc(
      agg_id,
      [&](const auto &agg_func) -> void {
    InvokeOnType(
        arg_type,
        [&](const auto &arg_type) -> void {
      functor(agg_func, arg_type);
    });
  });
}

template <typename FunctorT>
inline auto InvokeOnTwoAccessors(
    const ValueAccessorMultiplexer &accessor_mux,
    const ValueAccessorSource &first_source,
    const ValueAccessorSource &second_source,
    const FunctorT &functor) {
  ValueAccessor *base_accessor = accessor_mux.getBaseAccessor();
  ColumnVectorsValueAccessor *derived_accessor =
      static_cast<ColumnVectorsValueAccessor *>(accessor_mux.getDerivedAccessor());

  InvokeOnAnyValueAccessor(
      base_accessor,
      [&](auto *accessor) {
    if (first_source == ValueAccessorSource::kBase) {
      if (second_source == ValueAccessorSource::kBase) {
        return functor(std::false_type(), accessor, accessor);
      } else {
        return functor(std::true_type(), accessor, derived_accessor);
      }
    } else {
      if (second_source == ValueAccessorSource::kBase) {
        return functor(std::true_type(), derived_accessor, accessor);
      } else {
        return functor(std::false_type(), derived_accessor, derived_accessor);
      }
    }
  });
}

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_AGGREGATION_UTIL_HPP_
