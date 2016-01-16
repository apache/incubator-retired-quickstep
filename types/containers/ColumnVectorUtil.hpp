/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
