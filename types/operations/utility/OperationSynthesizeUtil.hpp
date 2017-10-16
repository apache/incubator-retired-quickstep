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

#ifndef QUICKSTEP_TYPES_OPERATIONS_UTILITY_OPERATION_SYNTHESIZE_UTIL_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_UTILITY_OPERATION_SYNTHESIZE_UTIL_HPP_

#include <cstddef>
#include <list>
#include <memory>
#include <string>
#include <type_traits>

#include "catalog/CatalogTypedefs.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "utility/Macros.hpp"
#include "utility/meta/TypeList.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

template <typename FunctorT, typename ...SpecArgs>
struct FunctorSpecializer {
  template <bool specialize = (sizeof...(SpecArgs) != 0),
            typename EnableT = void>
  struct Implementation;

  typedef Implementation<> type;
};

template <typename FunctorT, typename ...SpecArgs>
template <bool specialize>
struct FunctorSpecializer<FunctorT, SpecArgs...>
    ::Implementation<specialize, std::enable_if_t<specialize>> {
  template <typename ...FuncArgs>
  inline static auto Invoke(const FunctorT &functor, FuncArgs &&...args) {
    return functor.template apply<SpecArgs...>(std::forward<FuncArgs>(args)...);
  }
  typedef FunctorT FunctorType;
};

template <typename FunctorT, typename ...SpecArgs>
template <bool specialize>
struct FunctorSpecializer<FunctorT, SpecArgs...>
    ::Implementation<specialize, std::enable_if_t<!specialize>> {
  template <typename ...FuncArgs>
  inline static auto Invoke(const FunctorT &functor, FuncArgs &&...args) {
    return functor.apply(std::forward<FuncArgs>(args)...);
  }
  typedef FunctorT FunctorType;
};

template <typename ColumnVectorT>
class ColumnVectorAccessor {
 public:
  constexpr static bool kIsGenericColumnVectorAccessor =
      (ColumnVectorT::kImplementation == ColumnVector::kGeneric);

  inline explicit ColumnVectorAccessor(const ColumnVectorT &column_vector)
      : column_vector_(column_vector),
        length_(column_vector.size()) {}

  inline void beginIteration() {
    pos_ = static_cast<std::size_t>(-1);
  }

  inline bool next() {
    return (++pos_) < length_;
  }

  inline std::size_t getNumTuples() const {
    return length_;
  }

  template <bool check_null>
  inline const void* getUntypedValue(const attribute_id) const {
    return column_vector_.template getUntypedValue<check_null>(pos_);
  }

  inline TypedValue getTypedValue(const attribute_id) const {
    return column_vector_.getTypedValue(pos_);
  }

  template <bool check_null, bool generic>
  inline typename std::enable_if_t<generic, ColumnVectorT>::cpptype
      getLiteralValue() const {
    return column_vector_.template getLiteralValue<check_null>(pos_);
  }

 private:
  const ColumnVectorT &column_vector_;
  const std::size_t length_;
  std::size_t pos_;

  DISALLOW_COPY_AND_ASSIGN(ColumnVectorAccessor);
};

template <typename FuncSpec, typename T, typename EnableT = void>
struct OperationCodegen;

template <typename FuncSpec, typename T>
struct OperationCodegen<FuncSpec, T,
                        std::enable_if_t<T::kMemoryLayout == kCxxInlinePod>> {
  using ValueType = T;
  using ColumnVectorType = NativeColumnVector;
  using FunctorSpecializer = FuncSpec;

  using NativeType = typename T::cpptype;
  using NativeTypeConst = const NativeType;
  using NativeTypeConstRef = const NativeType&;
  using NativeTypeConstPtr = const NativeType*;

  template <typename ArgumentGen>
  inline static TypedValue ApplyUnaryTypedValue(
      typename ArgumentGen::NativeTypeConstRef argument,
      const Type &result_type,
      const typename FuncSpec::FunctorType &functor) {
    return TypedValue(FuncSpec::Invoke(functor, argument));
  }

  template <typename ArgumentGen>
  inline static void ApplyUnaryColumnVector(
      const typename ArgumentGen::NativeTypeConstRef argument,
      const typename FuncSpec::FunctorType &functor,
      ColumnVectorType *cv) {
    *static_cast<NativeType *>(cv->getPtrForDirectWrite()) =
        FuncSpec::Invoke(functor, argument);
  }

  template <typename LeftGen, typename RightGen>
  inline static TypedValue ApplyBinaryTypedValue(
      typename LeftGen::NativeTypeConstRef left,
      typename RightGen::NativeTypeConstRef right,
      const Type &result_type,
      const typename FuncSpec::FunctorType &functor) {
    return TypedValue(FuncSpec::Invoke(functor, left, right));
  }

  template <typename LeftGen, typename RightGen>
  inline static void ApplyBinaryColumnVector(
      const typename LeftGen::NativeTypeConstRef left,
      const typename RightGen::NativeTypeConstRef right,
      const typename FuncSpec::FunctorType &functor,
      ColumnVectorType *cv) {
    *static_cast<NativeType *>(cv->getPtrForDirectWrite()) =
        FuncSpec::Invoke(functor, left, right);
  }

  template <bool nullable, typename AccessorT>
  inline static NativeTypeConstPtr GetValuePtr(const AccessorT *accessor,
                                               const attribute_id attr_id,
                                               const Type &, void *) {
    return static_cast<NativeTypeConstPtr>(
        accessor->template getUntypedValue<nullable>(attr_id));
  }

  inline static bool IsNull(const NativeType *value) {
    return value == nullptr;
  }

  // Dereference: NativeTypeConstPtr& -> const NativeType&
  inline static const NativeType& Dereference(const NativeType *value) {
    return *value;
  }

  inline static const NativeType ToNativeValueConst(const TypedValue &value,
                                                    const Type &) {
    return value.getLiteral<NativeType>();
  }
};

template <typename FuncSpec, typename T>
struct OperationCodegen<FuncSpec, T,
                        std::enable_if_t<T::kMemoryLayout == kParInlinePod>> {
  using ValueType = T;
  using ColumnVectorType = NativeColumnVector;
  using FunctorSpecializer = FuncSpec;

  using NativeType = const void*;
  using NativeTypeConst = const void*;
  using NativeTypeConstRef = const void*;
  using NativeTypeConstPtr = const void*;

  template <typename ArgumentGen>
  inline static TypedValue ApplyUnaryTypedValue(
      typename ArgumentGen::NativeTypeConstRef argument,
      const Type &result_type,
      const typename FuncSpec::FunctorType &functor) {
    void *result = std::malloc(result_type.maximumByteLength());
    FuncSpec::Invoke(functor, argument, result);
    return TypedValue::CreateWithOwnedData(T::kStaticTypeID,
                                           result,
                                           result_type.maximumByteLength());
  }

  template <typename ArgumentGen>
  inline static void ApplyUnaryColumnVector(
      const typename ArgumentGen::NativeTypeConstRef argument,
      const typename FuncSpec::FunctorType &functor,
      ColumnVectorType *cv) {
    FuncSpec::Invoke(functor, argument, cv->getPtrForDirectWrite());
  }

  template <typename LeftGen, typename RightGen>
  inline static TypedValue ApplyBinaryTypedValue(
      typename LeftGen::NativeTypeConstRef left,
      typename RightGen::NativeTypeConstRef right,
      const Type &result_type,
      const typename FuncSpec::FunctorType &functor) {
    void *result = std::malloc(result_type.maximumByteLength());
    FuncSpec::Invoke(functor, left, right, result);
    return TypedValue::CreateWithOwnedData(T::kStaticTypeID,
                                           result,
                                           result_type.maximumByteLength());
  }

  template <typename LeftGen, typename RightGen>
  inline static void ApplyBinaryColumnVector(
      const typename LeftGen::NativeTypeConstRef left,
      const typename RightGen::NativeTypeConstRef right,
      const typename FuncSpec::FunctorType &functor,
      ColumnVectorType *cv) {
    FuncSpec::Invoke(functor, left, right, cv->getPtrForDirectWrite());
  }

  // TODO(refactor-type): Use ColumnAccessor to improve performance.
  template <bool nullable, typename AccessorT>
  inline static NativeTypeConstPtr GetValuePtr(const AccessorT *accessor,
                                               const attribute_id attr_id,
                                               const Type &, void *) {
    return accessor->template getUntypedValue<nullable>(attr_id);
  }

  inline static bool IsNull(const void *value) {
    return value == nullptr;
  }

  // Dereference: NativeTypeConstPtr& -> const NativeType&
  inline static const void* Dereference(const void *value) {
    return value;
  }

  inline static const void* ToNativeValueConst(const TypedValue &value,
                                               const Type &) {
    return value.getDataPtr();
  }
};

template <typename FuncSpec, typename T>
struct OperationCodegen<FuncSpec, T,
                        std::enable_if_t<T::kMemoryLayout == kParOutOfLinePod>> {
  using ValueType = T;
  using ColumnVectorType = IndirectColumnVector;
  using FunctorSpecializer = FuncSpec;

  using NativeType = TypedValue;
  using NativeTypeConst = const TypedValue;
  using NativeTypeConstRef = const TypedValue&;
  using NativeTypeConstPtr = const TypedValue;

  template <typename ArgumentGen>
  inline static TypedValue ApplyUnaryTypedValue(
      typename ArgumentGen::NativeTypeConstRef argument,
      const Type &result_type,
      const typename FuncSpec::FunctorType &functor) {
    return FuncSpec::Invoke(functor, argument);
  }

  template <typename ArgumentGen>
  inline static void ApplyUnaryColumnVector(
      const typename ArgumentGen::NativeTypeConstRef argument,
      const typename FuncSpec::FunctorType &functor,
      ColumnVectorType *cv) {
    cv->appendTypedValue(FuncSpec::Invoke(functor, argument));
  }

  template <typename LeftGen, typename RightGen>
  inline static TypedValue ApplyBinaryTypedValue(
      typename LeftGen::NativeTypeConstRef left,
      typename RightGen::NativeTypeConstRef right,
      const Type &result_type,
      const typename FuncSpec::FunctorType &functor) {
    return FuncSpec::Invoke(functor, left, right);
  }

  template <typename LeftGen, typename RightGen>
  inline static void ApplyBinaryColumnVector(
      const typename LeftGen::NativeTypeConstRef left,
      const typename RightGen::NativeTypeConstRef right,
      const typename FuncSpec::FunctorType &functor,
      ColumnVectorType *cv) {
    cv->appendTypedValue(FuncSpec::Invoke(functor, left, right));
  }

  template <bool nullable, typename AccessorT>
  inline static NativeTypeConstPtr GetValuePtr(const AccessorT *accessor,
                                               const attribute_id attr_id,
                                               const Type &, void *) {
    return accessor->getTypedValue(attr_id);
  }

  inline static bool IsNull(NativeTypeConstPtr &value) {
    return value.isNull();
  }

  // Dereference: NativeTypeConstPtr& -> const NativeType&
  inline static const NativeType& Dereference(NativeTypeConstPtr &value) {
    return value;
  }

  inline static const NativeType& ToNativeValueConst(const TypedValue &value,
                                                     const Type &) {
    return value;
  }
};


// TODO(refactor-type): Remove this.
namespace internal {

template <typename T>
std::enable_if_t<T::kIsGenericColumnVectorAccessor,
                 std::true_type> IsGenericColumnVectorAccessorImpl(int);
template <typename>
std::false_type IsGenericColumnVectorAccessorImpl(...);

}  // namespace internal

template <typename T>
using IsGenericColumnVectorAccessor =
    decltype(internal::IsGenericColumnVectorAccessorImpl<T>());

template <typename FuncSpec, typename T>
struct OperationCodegen<FuncSpec, T,
                        std::enable_if_t<T::kMemoryLayout == kCxxGeneric>> {
  using ValueType = T;
  using ColumnVectorType = GenericColumnVector<T>;
  using FunctorSpecializer = FuncSpec;

  using NativeType = typename T::cpptype;
  using NativeTypeConst = const NativeType;
  using NativeTypeConstRef = const NativeType&;
  using NativeTypeConstPtr = const NativeType*;

  template <typename ArgumentGen, typename ResultType>
  inline static TypedValue ApplyUnaryTypedValue(
      typename ArgumentGen::NativeTypeConstRef argument,
      const ResultType &result_type,
      const typename FuncSpec::FunctorType &functor) {
    const NativeType result = FuncSpec::Invoke(functor, argument);
    return result_type.marshallValue(&result);
  }

  template <typename ArgumentGen>
  inline static void ApplyUnaryColumnVector(
      const typename ArgumentGen::NativeTypeConstRef argument,
      const typename FuncSpec::FunctorType &functor,
      ColumnVectorType *cv) {
    cv->appendLiteralValue(FuncSpec::Invoke(functor, argument));
  }

  template <typename LeftGen, typename RightGen, typename ResultType>
  inline static TypedValue ApplyBinaryTypedValue(
      typename LeftGen::NativeTypeConstRef left,
      typename RightGen::NativeTypeConstRef right,
      const ResultType &result_type,
      const typename FuncSpec::FunctorType &functor) {
    const NativeType result = FuncSpec::Invoke(functor, left, right);
    return result_type.marshallValue(&result);
  }

  template <typename LeftGen, typename RightGen>
  inline static void ApplyBinaryColumnVector(
      const typename LeftGen::NativeTypeConstRef left,
      const typename RightGen::NativeTypeConstRef right,
      const typename FuncSpec::FunctorType &functor,
      ColumnVectorType *cv) {
    cv->appendLiteralValue(FuncSpec::Invoke(functor, left, right));
  }

  // TODO(refactor-type): Use ColumnAccessor to handle the more general case.
  template <bool nullable, typename AccessorT>
  inline static NativeTypeConstPtr GetValuePtr(
      const AccessorT *accessor,
      const attribute_id attr_id,
      const Type &, void *,
      std::enable_if_t<IsGenericColumnVectorAccessor<AccessorT>::value> * = 0) {
    return &accessor->template getLiteralValue<nullable, true>();
  }

  // TODO(refactor-type): Use ColumnAccessor to handle the more general case.
  template <bool nullable, typename AccessorT>
  inline static NativeTypeConstPtr GetValuePtr(
      const AccessorT *accessor,
      const attribute_id attr_id,
      const ValueType &value_type,
      std::unique_ptr<NativeType> *cache,
      std::enable_if_t<!IsGenericColumnVectorAccessor<AccessorT>::value> * = 0) {
    static_assert(std::is_same<NativeType, typename ValueType::cpptype>::value,
                  "Unexpected value type in OperationCodegen::GetValuePtr for "
                  "CxxGeneric types.");
    cache->reset(static_cast<NativeType*>(
        value_type.unmarshallTypedValue(accessor->getTypedValue(attr_id))));
    return cache->get();
  }

  inline static bool IsNull(NativeTypeConstPtr &value) {
    return value == nullptr;
  }

  // Dereference: NativeTypeConstPtr& -> const NativeType&
  inline static const NativeType& Dereference(NativeTypeConstPtr &value) {
    return *value;
  }

  inline static const NativeType ToNativeValueConst(const TypedValue &value,
                                                    const ValueType &value_type) {
    // TODO(refactor-type): Improve performance.
    std::unique_ptr<NativeType> cache(
        static_cast<NativeType*>(value_type.unmarshallTypedValue(value)));
    return *cache;
  }
};


template <typename ...FunctorTypes>
struct FunctorPack {
  template <typename Dispatcher>
  inline static std::list<OperationPtr> GenerateOperations() {
    std::vector<std::list<OperationPtr>> op_list_groups =
        { Dispatcher::template Generate<FunctorTypes>()... };

    std::list<OperationPtr> operations;
    for (std::list<OperationPtr> &op_list : op_list_groups) {
      operations.splice(operations.end(), std::move(op_list));
    }
    return operations;
  }
};

struct OperationPack {
  virtual std::vector<OperationPtr> generateOperations() = 0;
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_OPERATION_UTILITY_SYNTHESIZE_UTIL_HPP_
