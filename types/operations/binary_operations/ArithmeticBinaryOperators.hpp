/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#ifndef QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_ARITHMETIC_BINARY_OPERATORS_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_ARITHMETIC_BINARY_OPERATORS_HPP_

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageBlockInfo.hpp"

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

// We use these functors instead of the standard-library ones, because the
// standard-library functors in <functional> have to be instantiated for the
// most specific argument type, which would unnecessisarily introduce
// multiple copies of distinct template instantiations of operators.
template <typename LeftArgument, typename RightArgument> struct AddFunctor {
  inline auto operator() (const LeftArgument &left, const RightArgument &right) const -> decltype(left + right) {
    return left + right;
  }
};

// NOTE(zuyu): The C++ compiler in general converts all integers to floats
//             when doing the following operations,
//             but we could like to return double instead.
template <>
struct AddFunctor<std::int64_t, float> {
  inline double operator() (const std::int64_t &left, const float &right) const {
    return static_cast<double>(left) + static_cast<double>(right);
  }
};

template <>
struct AddFunctor<float, std::int64_t> {
  inline double operator() (const float &left, const std::int64_t &right) const {
    return static_cast<double>(left) + static_cast<double>(right);
  }
};

template <typename LeftArgument, typename RightArgument> struct SubtractFunctor {
  inline auto operator() (const LeftArgument &left, const RightArgument &right) const -> decltype(left - right) {
    return left - right;
  }
};

// NOTE(zuyu): The C++ compiler in general converts all integers to floats
//             when doing the following operations,
//             but we could like to return double instead.
template <>
struct SubtractFunctor<std::int64_t, float> {
  inline double operator() (const std::int64_t &left, const float &right) const {
    return static_cast<double>(left) - static_cast<double>(right);
  }
};

template <>
struct SubtractFunctor<float, std::int64_t> {
  inline double operator() (const float &left, const std::int64_t &right) const {
    return static_cast<double>(left) - static_cast<double>(right);
  }
};

template <typename LeftArgument, typename RightArgument> struct MultiplyFunctor {
  inline auto operator() (const LeftArgument &left, const RightArgument &right) const -> decltype(left * right) {
    return left * right;
  }
};

// NOTE(zuyu): The C++ compiler in general converts all integers to floats
//             when doing the following operations,
//             but we could like to return double instead.
template <>
struct MultiplyFunctor<std::int64_t, float> {
  inline double operator() (const std::int64_t &left, const float &right) const {
    return static_cast<double>(left) * static_cast<double>(right);
  }
};

template <>
struct MultiplyFunctor<float, std::int64_t> {
  inline double operator() (const float &left, const std::int64_t &right) const {
    return static_cast<double>(left) * static_cast<double>(right);
  }
};

template <typename LeftArgument, typename RightArgument> struct DivideFunctor {
  inline auto operator() (const LeftArgument &left, const RightArgument &right) const -> decltype(left / right) {
    return left / right;
  }
};

// NOTE(zuyu): The C++ compiler in general converts all integers to floats
//             when doing the following operations,
//             but we could like to return double instead.
template <>
struct DivideFunctor<std::int64_t, float> {
  inline double operator() (const std::int64_t &left, const float &right) const {
    return static_cast<double>(left) / static_cast<double>(right);
  }
};

template <>
struct DivideFunctor<float, std::int64_t> {
  inline double operator() (const float &left, const std::int64_t &right) const {
    return static_cast<double>(left) / static_cast<double>(right);
  }
};

template <typename LeftArgument, typename RightArgument> struct IntegerModuloFunctor {
  inline auto operator() (const LeftArgument &left, const RightArgument &right) const -> decltype(left % right) {
    return left % right;
  }
};

// NOTE(jianqiao): The C++11 standard specifies the following type signatures for fmod:
// (1) (double, double) -> double
// (2) (float, float) -> float
// (3) (long double, long double) -> long double
// (3) (Arithmetic, Arithmetic) -> double
template <typename LeftArgument, typename RightArgument> struct FloatModuloFunctor {
  inline auto operator() (const LeftArgument &left, const RightArgument &right) const
      -> decltype(std::fmod(left, right)) {
    return std::fmod(left, right);
  }
};

template <template <typename LeftCppType, typename RightCppType> class OpFunctor,
          typename ResultType,
          typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
class ArithmeticUncheckedBinaryOperator : public UncheckedBinaryOperator {
 public:
  ArithmeticUncheckedBinaryOperator() = default;
  ArithmeticUncheckedBinaryOperator(const ArithmeticUncheckedBinaryOperator &orig) = default;
  ~ArithmeticUncheckedBinaryOperator() = default;

  inline TypedValue applyToTypedValues(const TypedValue &left,
                                       const TypedValue &right) const override {
    return applyToTypedValuesInl(left, right);
  }

  // NOTE(chasseur): This inline version does NOT override a virtual in the
  // base class. g++ (and probably other compilers) will not inline a method
  // that overrides a virtual, so we use this instead when we cast to a
  // specific subclass so that we can actually inline the call.
  inline TypedValue applyToTypedValuesInl(const TypedValue &left,
                                          const TypedValue &right) const {
    if ((left_nullable && left.isNull()) || (right_nullable && right.isNull())) {
      return TypedValue(ResultType::kStaticTypeID);
    }
    return TypedValue(op_functor_(left.getLiteral<LeftCppType>(),
                                  right.getLiteral<RightCppType>()));
  }

  inline TypedValue applyToDataPtrs(const void *left, const void *right) const override {
    return applyToDataPtrsInl(left, right);
  }

  // See above note about inlines.
  inline TypedValue applyToDataPtrsInl(const void *left, const void *right) const {
    if ((left_nullable && (left == nullptr)) || (right_nullable && (right == nullptr))) {
      return TypedValue(ResultType::kStaticTypeID);
    }

    return TypedValue(op_functor_(*static_cast<const LeftCppType*>(left),
                                  *static_cast<const RightCppType*>(right)));
  }

  ColumnVector* applyToColumnVectors(
      const ColumnVector &left,
      const ColumnVector &right) const override {
    DCHECK(NativeColumnVector::UsableForType(
        ResultType::Instance(left_nullable || right_nullable)));
    // All arithmetic types (numbers, datetime, and intervals) are usable with
    // NativeColumnVector, so 'left' and 'right' should always be native.
    DCHECK(left.isNative());
    DCHECK(right.isNative());

    const NativeColumnVector &left_native = static_cast<const NativeColumnVector&>(left);
    const NativeColumnVector &right_native = static_cast<const NativeColumnVector&>(right);

    DCHECK_EQ(left_native.size(), right_native.size());
    NativeColumnVector *result = new NativeColumnVector(
        ResultType::Instance(left_nullable || right_nullable),
        left_native.size());
    for (std::size_t pos = 0;
         pos < left_native.size();
         ++pos) {
      const LeftCppType *left_value
          = static_cast<const LeftCppType*>(
              left_native.getUntypedValue<left_nullable>(pos));
      if (left_nullable && (left_value == nullptr)) {
        result->appendNullValue();
        continue;
      }
      const RightCppType *right_value
          = static_cast<const RightCppType*>(
              right_native.getUntypedValue<right_nullable>(pos));
      if (right_nullable && (right_value == nullptr)) {
        result->appendNullValue();
        continue;
      }
      *static_cast<typename ResultType::cpptype*>(result->getPtrForDirectWrite())
          = op_functor_(*left_value, *right_value);
    }
    return result;
  }

  ColumnVector* applyToColumnVectorAndStaticValue(
      const ColumnVector &left,
      const TypedValue &right) const override {
    return applyToColumnVectorAndStaticValueHelper<true>(left, right);
  }

  ColumnVector* applyToStaticValueAndColumnVector(
      const TypedValue &left,
      const ColumnVector &right) const override {
    return applyToColumnVectorAndStaticValueHelper<false>(right, left);
  }

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  ColumnVector* applyToSingleValueAccessor(
      ValueAccessor *accessor,
      const attribute_id left_id,
      const attribute_id right_id) const override {
    DCHECK(NativeColumnVector::UsableForType(
        ResultType::Instance(left_nullable || right_nullable)));
    return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
        accessor,
        [&](auto *accessor) -> ColumnVector* {  // NOLINT(build/c++11)
      NativeColumnVector *result = new NativeColumnVector(
          ResultType::Instance(left_nullable || right_nullable),
          accessor->getNumTuples());
      accessor->beginIteration();
      while (accessor->next()) {
        const LeftCppType *left_value = static_cast<const LeftCppType*>(
            accessor->template getUntypedValue<left_nullable>(left_id));
        if (left_nullable && (left_value == nullptr)) {
          result->appendNullValue();
          continue;
        }
        const RightCppType *right_value = static_cast<const RightCppType*>(
            accessor->template getUntypedValue<right_nullable>(right_id));
        if (right_nullable && (right_value == nullptr)) {
          result->appendNullValue();
          continue;
        }
        *static_cast<typename ResultType::cpptype*>(result->getPtrForDirectWrite())
            = op_functor_(*left_value, *right_value);
      }
      return result;
    });
  }

  ColumnVector* applyToValueAccessorAndStaticValue(
      ValueAccessor *left_accessor,
      const attribute_id left_id,
      const TypedValue &right) const override {
    return applyToValueAccessorAndStaticValueHelper<true>(left_accessor, left_id, right);
  }

  ColumnVector* applyToStaticValueAndValueAccessor(
      const TypedValue &left,
      ValueAccessor *right_accessor,
      const attribute_id right_id) const override {
    return applyToValueAccessorAndStaticValueHelper<false>(right_accessor, right_id, left);
  }

  ColumnVector* applyToColumnVectorAndValueAccessor(
      const ColumnVector &left,
      ValueAccessor *right_accessor,
      const attribute_id right_id) const override {
    return applyToColumnVectorAndValueAccessorHelper<true>(left, right_accessor, right_id);
  }

  ColumnVector* applyToValueAccessorAndColumnVector(
      ValueAccessor *left_accessor,
      const attribute_id left_id,
      const ColumnVector &right) const override {
    return applyToColumnVectorAndValueAccessorHelper<false>(right, left_accessor, left_id);
  }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN
  ColumnVector* applyToValueAccessorAndStaticValueForJoin(
      ValueAccessor *left_accessor,
      const bool left_accessor_is_left_relation,
      const attribute_id left_id,
      const TypedValue &right,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const override {
    return applyToValueAccessorAndStaticValueForJoinHelper<true>(left_accessor,
                                                                 left_accessor_is_left_relation,
                                                                 left_id,
                                                                 right,
                                                                 joined_tuple_ids);
  }

  ColumnVector* applyToStaticValueAndValueAccessorForJoin(
      const TypedValue &left,
      ValueAccessor *right_accessor,
      const bool right_accessor_is_left_relation,
      const attribute_id right_id,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const override {
    return applyToValueAccessorAndStaticValueForJoinHelper<false>(right_accessor,
                                                                  right_accessor_is_left_relation,
                                                                  right_id,
                                                                  left,
                                                                  joined_tuple_ids);
  }

  ColumnVector* applyToColumnVectorAndValueAccessorForJoin(
      const ColumnVector &left,
      ValueAccessor *right_accessor,
      const bool right_accessor_is_left_relation,
      const attribute_id right_id,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const override {
    return applyToColumnVectorAndValueAccessorForJoinHelper<true>(left,
                                                                  right_accessor,
                                                                  right_accessor_is_left_relation,
                                                                  right_id,
                                                                  joined_tuple_ids);
  }

  ColumnVector* applyToValueAccessorAndColumnVectorForJoin(
      ValueAccessor *left_accessor,
      const bool left_accessor_is_left_relation,
      const attribute_id left_id,
      const ColumnVector &right,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const override {
    return applyToColumnVectorAndValueAccessorForJoinHelper<false>(right,
                                                                   left_accessor,
                                                                   left_accessor_is_left_relation,
                                                                   left_id,
                                                                   joined_tuple_ids);
  }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN_WITH_BINARY_EXPRESSIONS
  ColumnVector* applyToValueAccessorsForJoin(
      ValueAccessor *left_accessor,
      const bool left_accessor_is_left_relation,
      const attribute_id left_id,
      ValueAccessor *right_accessor,
      const bool right_accessor_is_left_relation,
      const attribute_id right_id,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const override {
    DCHECK(NativeColumnVector::UsableForType(
        ResultType::Instance(left_nullable || right_nullable)));
    NativeColumnVector *result = new NativeColumnVector(
        ResultType::Instance(left_nullable || right_nullable),
        joined_tuple_ids.size());
    InvokeOnValueAccessorNotAdapter(
        left_accessor,
        [&](auto *left_accessor) -> void {  // NOLINT(build/c++11)
      InvokeOnValueAccessorNotAdapter(
          right_accessor,
          [&](auto *right_accessor) -> void {  // NOLINT(build/c++11)
        for (const std::pair<tuple_id, tuple_id> &joined_pair : joined_tuple_ids) {
          const LeftCppType *left_value = static_cast<const LeftCppType*>(
              left_accessor->template getUntypedValueAtAbsolutePosition<left_nullable>(
                  left_id,
                  left_accessor_is_left_relation ? joined_pair.first
                                                 : joined_pair.second));
          if (left_nullable && (left_value == nullptr)) {
            result->appendNullValue();
            continue;
          }
          const RightCppType *right_value = static_cast<const RightCppType*>(
              right_accessor->template getUntypedValueAtAbsolutePosition<right_nullable>(
                  right_id,
                  right_accessor_is_left_relation ? joined_pair.first
                                                  : joined_pair.second));
          if (right_nullable && (right_value == nullptr)) {
            result->appendNullValue();
            continue;
          }
          *static_cast<typename ResultType::cpptype*>(result->getPtrForDirectWrite())
              = op_functor_(*left_value, *right_value);
        }
      });
    });
    return result;
  }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN_WITH_BINARY_EXPRESSIONS

  TypedValue accumulateColumnVector(
      const TypedValue &current,
      const ColumnVector &column_vector,
      std::size_t *num_tuples_applied) const override {
    return accumulateColumnVectorHelper(
        current,
        column_vector,
        num_tuples_applied,
        std::is_same<LeftCppType, typename ResultType::cpptype>());
  }

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  TypedValue accumulateValueAccessor(
      const TypedValue &current,
      ValueAccessor *accessor,
      const attribute_id value_accessor_id,
      std::size_t *num_tuples_applied) const override {
    return accumulateValueAccessorHelper(
        current,
        accessor,
        value_accessor_id,
        num_tuples_applied,
        std::is_same<LeftCppType, typename ResultType::cpptype>());
  }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

 private:
  template <bool column_vector_on_left>
  ColumnVector* applyToColumnVectorAndStaticValueHelper(
      const ColumnVector &column_vector,
      const TypedValue &static_value) const {
    typedef typename std::conditional<column_vector_on_left,
                                      RightCppType,
                                      LeftCppType>::type
        StaticValueCppType;

    constexpr bool cv_nullable = column_vector_on_left ? left_nullable : right_nullable;
    constexpr bool static_value_nullable = column_vector_on_left ? right_nullable : left_nullable;

    DCHECK(column_vector.isNative());
    const NativeColumnVector &native_column_vector = static_cast<const NativeColumnVector&>(column_vector);

    DCHECK(NativeColumnVector::UsableForType(
        ResultType::Instance(left_nullable || right_nullable)));

    NativeColumnVector *result = new NativeColumnVector(
        ResultType::Instance(left_nullable || right_nullable),
        native_column_vector.size());
    if (static_value_nullable && static_value.isNull()) {
      result->fillWithNulls();
      return result;
    }
    const StaticValueCppType literal = static_value.getLiteral<StaticValueCppType>();
    for (std::size_t pos = 0;
         pos < native_column_vector.size();
         ++pos) {
      const void* cv_value = native_column_vector.getUntypedValue<cv_nullable>(pos);
      if (cv_nullable && (cv_value == nullptr)) {
        result->appendNullValue();
      } else {
        *static_cast<typename ResultType::cpptype*>(result->getPtrForDirectWrite())
            = castAndApply<column_vector_on_left>(cv_value, &literal);
      }
    }
    return result;
  }

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  template <bool value_accessor_on_left>
  ColumnVector* applyToValueAccessorAndStaticValueHelper(
      ValueAccessor *value_accessor,
      const attribute_id value_accessor_attr_id,
      const TypedValue &static_value) const {
    DCHECK(NativeColumnVector::UsableForType(
        ResultType::Instance(left_nullable || right_nullable)));

    return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
        value_accessor,
        [&](auto *value_accessor) -> ColumnVector* {  // NOLINT(build/c++11)
      typedef typename std::conditional<value_accessor_on_left,
                                        RightCppType,
                                        LeftCppType>::type
          StaticValueCppType;

      constexpr bool va_nullable = value_accessor_on_left ? left_nullable : right_nullable;
      constexpr bool static_value_nullable = value_accessor_on_left ? right_nullable : left_nullable;

      NativeColumnVector *result = new NativeColumnVector(
          ResultType::Instance(left_nullable || right_nullable),
          value_accessor->getNumTuples());
      if (static_value_nullable && static_value.isNull()) {
        result->fillWithNulls();
        return result;
      }
      const StaticValueCppType literal = static_value.getLiteral<StaticValueCppType>();
      value_accessor->beginIteration();
      while (value_accessor->next()) {
        const void* va_value
            = value_accessor->template getUntypedValue<va_nullable>(value_accessor_attr_id);
        if (va_nullable && (va_value == nullptr)) {
          result->appendNullValue();
        } else {
          *static_cast<typename ResultType::cpptype*>(result->getPtrForDirectWrite())
              = this->castAndApply<value_accessor_on_left>(va_value, &literal);
        }
      }
      return result;
    });
  }

  template <bool column_vector_on_left>
  ColumnVector* applyToColumnVectorAndValueAccessorHelper(
      const ColumnVector &column_vector,
      ValueAccessor *value_accessor,
      const attribute_id value_accessor_attr_id) const {
    DCHECK(column_vector.isNative());
    const NativeColumnVector &native_column_vector = static_cast<const NativeColumnVector&>(column_vector);

    DCHECK(NativeColumnVector::UsableForType(
        ResultType::Instance(left_nullable || right_nullable)));
    return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
        value_accessor,
        [&](auto *value_accessor) -> ColumnVector* {  // NOLINT(build/c++11)
      constexpr bool cv_nullable = column_vector_on_left ? left_nullable : right_nullable;
      constexpr bool va_nullable = column_vector_on_left ? right_nullable : left_nullable;

      DCHECK_EQ(native_column_vector.size(),
                static_cast<std::size_t>(value_accessor->getNumTuples()));
      NativeColumnVector *result = new NativeColumnVector(
          ResultType::Instance(left_nullable || right_nullable),
          native_column_vector.size());
      std::size_t cv_pos = 0;
      value_accessor->beginIteration();
      while (value_accessor->next()) {
        const void *cv_value = native_column_vector.getUntypedValue<cv_nullable>(cv_pos);
        if (cv_nullable && (cv_value == nullptr)) {
          result->appendNullValue();
          ++cv_pos;
          continue;
        }
        const void *va_value
            = value_accessor->template getUntypedValue<va_nullable>(value_accessor_attr_id);
        if (va_nullable && (va_value == nullptr)) {
          result->appendNullValue();
          ++cv_pos;
          continue;
        }
        *static_cast<typename ResultType::cpptype*>(result->getPtrForDirectWrite())
            = this->castAndApply<column_vector_on_left>(cv_value, va_value);
        ++cv_pos;
      }
      return result;
    });
  }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN
  template <bool value_accessor_on_left>
  ColumnVector* applyToValueAccessorAndStaticValueForJoinHelper(
      ValueAccessor *value_accessor,
      const bool accessor_is_left_relation,
      const attribute_id value_accessor_attr_id,
      const TypedValue &static_value,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const {
    typedef typename std::conditional<value_accessor_on_left,
                                      RightCppType,
                                      LeftCppType>::type
        StaticValueCppType;

    constexpr bool static_value_nullable = value_accessor_on_left ? right_nullable : left_nullable;

    DCHECK(NativeColumnVector::UsableForType(
        ResultType::Instance(left_nullable || right_nullable)));
    NativeColumnVector *result = new NativeColumnVector(
        ResultType::Instance(left_nullable || right_nullable),
        joined_tuple_ids.size());
    if (static_value_nullable && static_value.isNull()) {
      result->fillWithNulls();
      return result;
    }

    const StaticValueCppType literal = static_value.getLiteral<StaticValueCppType>();

    InvokeOnValueAccessorNotAdapter(
        value_accessor,
        [&](auto *value_accessor) -> void {  // NOLINT(build/c++11)
      constexpr bool va_nullable = value_accessor_on_left ? left_nullable : right_nullable;

      for (const std::pair<tuple_id, tuple_id> &joined_pair : joined_tuple_ids) {
        const void* va_value
            = value_accessor->template getUntypedValueAtAbsolutePosition<va_nullable>(
                value_accessor_attr_id,
                accessor_is_left_relation ? joined_pair.first : joined_pair.second);
        if (va_nullable && (va_value == nullptr)) {
          result->appendNullValue();
        } else {
          *static_cast<typename ResultType::cpptype*>(result->getPtrForDirectWrite())
              = this->castAndApply<value_accessor_on_left>(va_value, &literal);
        }
      }
    });
    return result;
  }

  template <bool column_vector_on_left>
  ColumnVector* applyToColumnVectorAndValueAccessorForJoinHelper(
      const ColumnVector &column_vector,
      ValueAccessor *value_accessor,
      const bool accessor_is_left_relation,
      const attribute_id value_accessor_attr_id,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const {
    DCHECK(column_vector.isNative());
    const NativeColumnVector &native_column_vector = static_cast<const NativeColumnVector&>(column_vector);
    DCHECK_EQ(native_column_vector.size(), joined_tuple_ids.size());

    DCHECK(NativeColumnVector::UsableForType(
        ResultType::Instance(left_nullable || right_nullable)));
    NativeColumnVector *result = new NativeColumnVector(
        ResultType::Instance(left_nullable || right_nullable),
        native_column_vector.size());
    InvokeOnValueAccessorNotAdapter(
        value_accessor,
        [&](auto *value_accessor) -> void {  // NOLINT(build/c++11)
      constexpr bool cv_nullable = column_vector_on_left ? left_nullable : right_nullable;
      constexpr bool va_nullable = column_vector_on_left ? right_nullable : left_nullable;

      for (std::size_t pos = 0; pos < native_column_vector.size(); ++pos) {
        const void *cv_value = native_column_vector.getUntypedValue<cv_nullable>(pos);
        if (cv_nullable && (cv_value == nullptr)) {
          result->appendNullValue();
          continue;
        }
        const void *va_value
            = value_accessor->template getUntypedValueAtAbsolutePosition<va_nullable>(
                value_accessor_attr_id,
                accessor_is_left_relation ? joined_tuple_ids[pos].first
                                          : joined_tuple_ids[pos].second);
        if (va_nullable && (va_value == nullptr)) {
          result->appendNullValue();
          continue;
        }
        *static_cast<typename ResultType::cpptype*>(result->getPtrForDirectWrite())
            = this->castAndApply<column_vector_on_left>(cv_value, va_value);
      }
    });
    return result;
  }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN

  // Actual implementation of accumulator.
  TypedValue accumulateColumnVectorHelper(
      const TypedValue &current,
      const ColumnVector &column_vector,
      std::size_t *num_tuples_applied,
      std::true_type) const {
    DCHECK(num_tuples_applied);

    *num_tuples_applied = 0;
    if (left_nullable && current.isNull()) {
      return ResultType::Instance(left_nullable).makeNullValue();
    }

    LeftCppType accumulated = current.getLiteral<LeftCppType>();

    DCHECK(column_vector.isNative());
    const NativeColumnVector &native_column_vector = static_cast<const NativeColumnVector&>(column_vector);
    for (std::size_t pos = 0;
         pos < native_column_vector.size();
         ++pos) {
      const RightCppType *value = static_cast<const RightCppType *>(
        native_column_vector.getUntypedValue<right_nullable>(pos));
      if ((right_nullable && value) || !right_nullable) {
        accumulated = op_functor_(accumulated, *value);
        ++(*num_tuples_applied);
      }
    }

    // Note ResultType::cpptype and LeftCppType are same here.
    return TypedValue(accumulated);
  }

  // Unimplemented version of accumlator for use when the result type and left
  // operand type are not same.
  TypedValue accumulateColumnVectorHelper(
      const TypedValue &current,
      const ColumnVector &column_vector,
      std::size_t *num_tuples_applied,
      std::false_type) const {
    FATAL_ERROR("Unimplemented method UncheckedBinaryOperator::accumulateColumnVectorHelper() "
        "because ResultType::cpptype and LeftCppType are not same.");
  }

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  // Actual implementation of accumlator.
  TypedValue accumulateValueAccessorHelper(
      const TypedValue &current,
      ValueAccessor *accessor,
      const attribute_id value_accessor_id,
      std::size_t *num_tuples_applied,
      std::true_type) const {
    DCHECK(num_tuples_applied);

    *num_tuples_applied = 0;
    if (left_nullable && current.isNull()) {
      return ResultType::Instance(left_nullable).makeNullValue();
    }

    LeftCppType accumulated = current.getLiteral<LeftCppType>();
    InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
        accessor,
        [&](auto *accessor) -> void {  // NOLINT(build/c++11)
      accessor->beginIteration();
      while (accessor->next()) {
        const RightCppType *value = static_cast<const RightCppType *>(
          accessor->template getUntypedValue<right_nullable>(value_accessor_id));
        if ((right_nullable && value) || !right_nullable) {
          accumulated = op_functor_(accumulated, *value);
          ++(*num_tuples_applied);
        }
      }
    });

    // Note ResultType::cpptype and LeftCppType are same here.
    return TypedValue(accumulated);
  }

  // Unimplemented version of accumlator for use when the result type and left
  // operand type are not same.
  TypedValue accumulateValueAccessorHelper(
      const TypedValue &current,
      ValueAccessor *accessor,
      const attribute_id value_accessor_id,
      std::size_t *num_tuples_applied,
      std::false_type) const {
    FATAL_ERROR("Unimplemented method UncheckedBinaryOperator::accumulateValueAccessorHelper() "
        "because ResultType::cpptype and LeftCppType are not same.");
  }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

  template <bool arguments_in_order>
  inline typename ResultType::cpptype castAndApply(const void *left, const void *right) const {
    return op_functor_(*static_cast<const LeftCppType*>(arguments_in_order ? left : right),
                       *static_cast<const RightCppType*>(arguments_in_order ? right : left));
  }

  OpFunctor<LeftCppType, RightCppType> op_functor_;
};

/**
 * @brief The UncheckedBinaryOperator for addition.
 **/
template <typename ResultType,
          typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
using AddArithmeticUncheckedBinaryOperator
    = ArithmeticUncheckedBinaryOperator<AddFunctor,
                                        ResultType,
                                        LeftCppType, left_nullable,
                                        RightCppType, right_nullable>;

/**
 * @brief The UncheckedBinaryOperator for subtraction.
 **/
template <typename ResultType,
          typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
using SubtractArithmeticUncheckedBinaryOperator
    = ArithmeticUncheckedBinaryOperator<SubtractFunctor,
                                        ResultType,
                                        LeftCppType, left_nullable,
                                        RightCppType, right_nullable>;

/**
 * @brief The UncheckedBinaryOperator for multiplication.
 **/
template <typename ResultType,
          typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
using MultiplyArithmeticUncheckedBinaryOperator
    = ArithmeticUncheckedBinaryOperator<MultiplyFunctor,
                                        ResultType,
                                        LeftCppType, left_nullable,
                                        RightCppType, right_nullable>;

/**
 * @brief The UncheckedBinaryOperator for division.
 **/
template <typename ResultType,
          typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
using DivideArithmeticUncheckedBinaryOperator
    = ArithmeticUncheckedBinaryOperator<DivideFunctor,
                                        ResultType,
                                        LeftCppType, left_nullable,
                                        RightCppType, right_nullable>;

/**
 * @brief The UncheckedBinaryOperator for integer modulo.
 **/
template <typename ResultType,
          typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
using IntegerModuloArithmeticUncheckedBinaryOperator
    = ArithmeticUncheckedBinaryOperator<IntegerModuloFunctor,
                                        ResultType,
                                        LeftCppType, left_nullable,
                                        RightCppType, right_nullable>;

/**
 * @brief The UncheckedBinaryOperator for real number modulo.
 **/
template <typename ResultType,
          typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
using FloatModuloArithmeticUncheckedBinaryOperator
    = ArithmeticUncheckedBinaryOperator<FloatModuloFunctor,
                                        ResultType,
                                        LeftCppType, left_nullable,
                                        RightCppType, right_nullable>;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_ARITHMETIC_BINARY_OPERATORS_HPP_
