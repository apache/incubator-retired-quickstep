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

#ifndef QUICKSTEP_STORAGE_VALUE_ACCESSOR_UTIL_HPP_
#define QUICKSTEP_STORAGE_VALUE_ACCESSOR_UTIL_HPP_

#include <type_traits>

#include "storage/BasicColumnStoreValueAccessor.hpp"
#include "storage/CompressedColumnStoreValueAccessor.hpp"
#include "storage/CompressedPackedRowStoreValueAccessor.hpp"
#include "storage/PackedRowStoreValueAccessor.hpp"
#include "storage/SplitRowStoreValueAccessor.hpp"
#include "storage/ValueAccessor.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief Invoke a generic functor (or lambda) on a ValueAccessor (known not to
 *        be an adapter) that is downcast to its actual concrete type.
 * @note This is intended for cases where it is known that a ValueAccessor will
 *       never be a TupleIdSequenceAdapterValueAccessor or
 *       OrderedTupleIdSequenceAdapterValueAccessor. If in doubt, consider
 *       using InvokeOnValueAccessorMaybeTupleIdSequenceAdapter() or
 *       InvokeOnAnyValueAccessor() instead.
 *
 * @param accessor A pointer to a ValueAccessor that will be downcast to its
 *        concrete type so that inline methods can be used.
 * @param functor A generic functor or lambda that has a templated call
 *        operator taking a single argument. The call operator will be invoked
 *        with the downcast accessor (as a mutable pointer) as its argument.
 * @return The return value of functor's call operator applied to the downcast
 *         accessor pointer.
 **/
template <typename FunctorT>
auto InvokeOnValueAccessorNotAdapter(
    ValueAccessor *accessor,
    const FunctorT &functor) {
  DCHECK(!accessor->isTupleIdSequenceAdapter());
  DCHECK(!accessor->isOrderedTupleIdSequenceAdapter());

  switch (accessor->getImplementationType()) {
    case ValueAccessor::Implementation::kBasicColumnStore:
      return functor(static_cast<BasicColumnStoreValueAccessor*>(accessor));
    case ValueAccessor::Implementation::kCompressedColumnStore:
      return functor(static_cast<CompressedColumnStoreValueAccessor*>(accessor));
    case ValueAccessor::Implementation::kCompressedPackedRowStore:
      return functor(static_cast<CompressedPackedRowStoreValueAccessor*>(accessor));
    case ValueAccessor::Implementation::kPackedRowStore:
      return functor(static_cast<PackedRowStoreValueAccessor*>(accessor));
    case ValueAccessor::Implementation::kSplitRowStore:
      return functor(static_cast<SplitRowStoreValueAccessor*>(accessor));
    case ValueAccessor::Implementation::kColumnVectors:
      return functor(static_cast<ColumnVectorsValueAccessor*>(accessor));
    default: {
      LOG(FATAL) << "Unrecognized ValueAccessor::Implementation: "
                 << static_cast<std::underlying_type<ValueAccessor::Implementation>::type>(
                        accessor->getImplementationType());
    }
  }
}

/**
 * @brief Invoke a generic functor (or lambda) on a ValueAccessor (known to be
 *        a TupleIdSequenceAdapterValueAccessor) that is downcast to its actual
 *        concrete type.
 * @note This is intended for cases where it is known that a ValueAccessor is
 *       definitely a TupleIdSequenceAdapterValueAccessor. If in doubt,
 *       consinder using InvokeOnValueAccessorMaybeTupleIdSequenceAdapter() or
 *       InvokeOnAnyValueAccessor() instead.
 *
 * @param accessor A pointer to a ValueAccessor that will be downcast to its
 *        concrete type so that inline methods can be used.
 * @param functor A generic functor or lambda that has a templated call
 *        operator taking a single argument. The call operator will be invoked
 *        with the downcast accessor (as a mutable pointer) as its argument.
 * @return The return value of functor's call operator applied to the downcast
 *         accessor pointer.
 **/
template <typename FunctorT>
auto InvokeOnTupleIdSequenceAdapterValueAccessor(
    ValueAccessor *accessor,
    const FunctorT &functor) {
  DCHECK(accessor->isTupleIdSequenceAdapter());
  DCHECK(!accessor->isOrderedTupleIdSequenceAdapter());

  switch (accessor->getImplementationType()) {
    case ValueAccessor::Implementation::kBasicColumnStore:
      return functor(
          static_cast<TupleIdSequenceAdapterValueAccessor<BasicColumnStoreValueAccessor>*>(
              accessor));
    case ValueAccessor::Implementation::kCompressedColumnStore:
      return functor(
          static_cast<TupleIdSequenceAdapterValueAccessor<CompressedColumnStoreValueAccessor>*>(
              accessor));
    case ValueAccessor::Implementation::kCompressedPackedRowStore:
      return functor(
          static_cast<TupleIdSequenceAdapterValueAccessor<CompressedPackedRowStoreValueAccessor>*>(
              accessor));
    case ValueAccessor::Implementation::kPackedRowStore:
      return functor(
          static_cast<TupleIdSequenceAdapterValueAccessor<PackedRowStoreValueAccessor>*>(
              accessor));
    case ValueAccessor::Implementation::kSplitRowStore:
      return functor(
          static_cast<TupleIdSequenceAdapterValueAccessor<SplitRowStoreValueAccessor>*>(
              accessor));
    case ValueAccessor::Implementation::kColumnVectors:
      return functor(
          static_cast<TupleIdSequenceAdapterValueAccessor<ColumnVectorsValueAccessor>*>(
              accessor));
    default: {
      LOG(FATAL) << "Unrecognized ValueAccessor::Implementation: "
                 << static_cast<std::underlying_type<ValueAccessor::Implementation>::type>(
                        accessor->getImplementationType());
    }
  }
}

/**
 * @brief Invoke a generic functor (or lambda) on a ValueAccessor (known to be
 *        an OrderedTupleIdSequenceAdapterValueAccessor) that is downcast to
 *        its actual concrete type.
 * @note This is intended for cases where it is known that a ValueAccessor is
 *       definitely an OrderedTupleIdSequenceAdapterValueAccessor. If in doubt,
 *       consinder using InvokeOnAnyValueAccessor() instead.
 *
 * @param accessor A pointer to a ValueAccessor that will be downcast to its
 *        concrete type so that inline methods can be used.
 * @param functor A generic functor or lambda that has a templated call
 *        operator taking a single argument. The call operator will be invoked
 *        with the downcast accessor (as a mutable pointer) as its argument.
 * @return The return value of functor's call operator applied to the downcast
 *         accessor pointer.
 **/
template <typename FunctorT>
auto InvokeOnOrderedTupleIdSequenceAdapterValueAccessor(
    ValueAccessor *accessor,
    const FunctorT &functor) {
  DCHECK(!accessor->isTupleIdSequenceAdapter());
  DCHECK(accessor->isOrderedTupleIdSequenceAdapter());

  switch (accessor->getImplementationType()) {
    case ValueAccessor::Implementation::kBasicColumnStore:
      return functor(
          static_cast<OrderedTupleIdSequenceAdapterValueAccessor<BasicColumnStoreValueAccessor>*>(
              accessor));
    case ValueAccessor::Implementation::kCompressedColumnStore:
      return functor(
          static_cast<OrderedTupleIdSequenceAdapterValueAccessor<CompressedColumnStoreValueAccessor>*>(
              accessor));
    case ValueAccessor::Implementation::kCompressedPackedRowStore:
      return functor(
          static_cast<OrderedTupleIdSequenceAdapterValueAccessor<CompressedPackedRowStoreValueAccessor>*>(
              accessor));
    case ValueAccessor::Implementation::kPackedRowStore:
      return functor(
          static_cast<OrderedTupleIdSequenceAdapterValueAccessor<PackedRowStoreValueAccessor>*>(
              accessor));
    case ValueAccessor::Implementation::kSplitRowStore:
      return functor(
          static_cast<OrderedTupleIdSequenceAdapterValueAccessor<SplitRowStoreValueAccessor>*>(
              accessor));
    case ValueAccessor::Implementation::kColumnVectors:
      return functor(
          static_cast<OrderedTupleIdSequenceAdapterValueAccessor<ColumnVectorsValueAccessor>*>(
              accessor));
    default: {
      LOG(FATAL) << "Unrecognized ValueAccessor::Implementation: "
                 << static_cast<std::underlying_type<ValueAccessor::Implementation>::type>(
                        accessor->getImplementationType());
    }
  }
}

/**
 * @brief Invoke a generic functor (or lambda) on a ValueAccessor that is
 *        downcast to its actual concrete type.
 * @note This version of this function may be used with a non-adapter
 *       ValueAccessor OR a TupleIdSequenceAdapterValueAccessor. The only place
 *       where it is not safe to use is in the bulk-insert path for inserting
 *       tuples into StorageBlock(s) via an InsertDestination (in such cases,
 *       an OrderedTupleIdSequenceAdapterValueAccessor is also possible as we
 *       may be bulk-inserting tuples sorted from a block without an
 *       intermediate copy). For that code path, the completely generic
 *       InvokeOnAnyValueAccessor() is available.
 * @note If it is known that a ValueAccessor will never be an adapter, using
 *       InvokeOnValueAccessorNotAdapter() instead will result in more compact
 *       code that compiles faster.
 *
 * @param accessor A pointer to a ValueAccessor that will be downcast to its
 *        concrete type so that inline methods can be used.
 * @param functor A generic functor or lambda that has a templated call
 *        operator taking a single argument. The call operator will be invoked
 *        with the downcast accessor (as a mutable pointer) as its argument.
 * @return The return value of functor's call operator applied to the downcast
 *         accessor pointer.
 **/
template <typename FunctorT>
auto InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
    ValueAccessor *accessor,
    const FunctorT &functor) {
  if (accessor->isTupleIdSequenceAdapter()) {
    return InvokeOnTupleIdSequenceAdapterValueAccessor(accessor, functor);
  } else {
    return InvokeOnValueAccessorNotAdapter(accessor, functor);
  }
}

/**
 * @brief Invoke a generic functor (or lambda) on a ValueAccessor that is
 *        downcast to its actual concrete type.
 * @note This is the most general version of this function, and is suitable for
 *       use with ANY ValueAccessor. Other functions in this file are more
 *       specific and may be used when more a priori information about the
 *       ValueAccessor is known to generate more compact, faster-compiling
 *       code.
 * @note The only situation where ANY type of ValueAccessor is possible is
 *       bulk-inserting into StorageBlock(s) via an InsertDestination. An
 *       OrderedTupleIdSequenceAdapterValueAccessor will not be used in any
 *       other situation, so it is at least safe to use
 *       InvokeOnValueAccessorMaybeTupleIdSequenceAdapter() everywhere else.
 *       Depending on the context, it may also be safe to use
 *       InvokeOnValueAccessorNotAdapter() for even more compact code.
 *
 * @param accessor A pointer to a ValueAccessor that will be downcast to its
 *        concrete type so that inline methods can be used.
 * @param functor A generic functor or lambda that has a templated call
 *        operator taking a single argument. The call operator will be invoked
 *        with the downcast accessor (as a mutable pointer) as its argument.
 * @return The return value of functor's call operator applied to the downcast
 *         accessor pointer.
 **/
template <typename FunctorT>
auto InvokeOnAnyValueAccessor(
    ValueAccessor *accessor,
    const FunctorT &functor) {
  if (accessor->isTupleIdSequenceAdapter()) {
    return InvokeOnTupleIdSequenceAdapterValueAccessor(accessor, functor);
  } else if (accessor->isOrderedTupleIdSequenceAdapter()) {
    return InvokeOnOrderedTupleIdSequenceAdapterValueAccessor(accessor, functor);
  } else {
    return InvokeOnValueAccessorNotAdapter(accessor, functor);
  }
}

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_VALUE_ACCESSOR_UTIL_HPP_
