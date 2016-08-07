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

#ifndef QUICKSTEP_TYPES_CONTAINERS_COLUMN_VECTOR_UTIL_HPP_
#define QUICKSTEP_TYPES_CONTAINERS_COLUMN_VECTOR_UTIL_HPP_

#include "types/containers/ColumnVector.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief Invoke a generic functor (or lambda) on a ColumnVector that is
 *        downcast to its actual concrete type.
 *
 * @param column_vector A ColumnVector that will be downcast to its concrete
 *        type so that inline methods can be used.
 * @param functor A generic functor or lambda that has a templated call
 *        operator taking a single argument. The call operator will be invoked
 *        with the downcast column_vector as its argument.
 * @return The return value of functor's call operator applied to the downcast
 *         column_vector.
 **/
template <typename FunctorT>
auto InvokeOnColumnVector(const ColumnVector &column_vector,
                          const FunctorT &functor) {
  if (column_vector.isNative()) {
    return functor(static_cast<const NativeColumnVector&>(column_vector));
  } else {
    return functor(static_cast<const IndirectColumnVector&>(column_vector));
  }
}

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_CONTAINERS_COLUMN_VECTOR_UTIL_HPP_
