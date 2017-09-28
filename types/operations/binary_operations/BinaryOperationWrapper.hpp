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

#ifndef QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_BINARY_OPERATION_WRAPPER_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_BINARY_OPERATION_WRAPPER_HPP_

#include <cstddef>
#include <list>
#include <string>
#include <tuple>
#include <type_traits>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/operations/OperationSignature.hpp"
#include "types/operations/OperationUtil.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "utility/Macros.hpp"
#include "utility/meta/Common.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

template <typename LeftArgumentT, typename RightArgumentT, typename ResultT>
struct BinaryFunctor {
  typedef LeftArgumentT LeftArgumentType;
  typedef RightArgumentT RightArgumentType;
  typedef ResultT ResultType;

  static constexpr Operation
      ::OperationSuperTypeID kOperationSuperTypeID = Operation::kBinaryOperation;
};

template <typename FunctorT, typename ...SpecArgs>
class UncheckedBinaryOperatorWrapperCodegen : public UncheckedBinaryOperator {
 public:
  template <typename ...ConstructorArgs>
  UncheckedBinaryOperatorWrapperCodegen(const Type &left_type,
                                        const Type &right_type,
                                        const Type &result_type,
                                        ConstructorArgs &&...args)
      : functor_(std::forward<ConstructorArgs>(args)...),
        impl_(functor_, left_type, right_type, result_type) {}

  TypedValue applyToTypedValues(const TypedValue &left,
                                const TypedValue &right) const override {
    return impl_.applyToTypedValues(left, right);
  }

  ColumnVector* applyToColumnVectors(const ColumnVector &left,
                                     const ColumnVector &right) const override {
    using LeftCVT = typename LeftGen::ColumnVectorType;
    DCHECK_EQ(left.isNative(), LeftCVT::kNative);
    using LeftAccessorT = ColumnVectorValueAccessor<LeftCVT>;
    LeftAccessorT left_accessor(static_cast<const LeftCVT&>(left));

    using RightCVT = typename RightGen::ColumnVectorType;
    DCHECK_EQ(right.isNative(), RightCVT::kNative);
    using RightAccessorT = ColumnVectorValueAccessor<RightCVT>;
    RightAccessorT right_accessor(static_cast<const RightCVT&>(right));

    const std::size_t num_tuples = left_accessor.getNumTuples();
    DCHECK_EQ(num_tuples, right_accessor.getNumTuples());

    return impl_.applyToValueAccessors(num_tuples,
                                       &left_accessor, kInvalidAttributeID,
                                       &right_accessor, kInvalidAttributeID);
  }

  ColumnVector* applyToColumnVectorAndStaticValue(
      const ColumnVector &left,
      const TypedValue &right) const override {
    using LeftCVT = typename LeftGen::ColumnVectorType;
    DCHECK_EQ(left.isNative(), LeftCVT::kNative);

    using LeftAccessorT = ColumnVectorValueAccessor<LeftCVT>;
    LeftAccessorT accessor(static_cast<const LeftCVT&>(left));
    return impl_.applyToValueAccessorAndStaticValue(&accessor, 0, right);
  }

  ColumnVector* applyToStaticValueAndColumnVector(
      const TypedValue &left,
      const ColumnVector &right) const override {
    using RightCVT = typename RightGen::ColumnVectorType;
    DCHECK_EQ(right.isNative(), RightCVT::kNative);

    using RightAccessorT = ColumnVectorValueAccessor<RightCVT>;
    RightAccessorT accessor(static_cast<const RightCVT&>(right));
    return impl_.applyToStaticValueAndValueAccessor(left, &accessor, 0);
  }

  ColumnVector* applyToSingleValueAccessor(
      ValueAccessor *accessor,
      const attribute_id left_id,
      const attribute_id right_id) const override {
    return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
        accessor,
        [&](auto *accessor) -> ColumnVector* {  // NOLINT(build/c++11)
      return impl_.applyToSingleValueAccessor(accessor, left_id, right_id);
    });
  }

  ColumnVector* applyToValueAccessorAndStaticValue(
      ValueAccessor *left_accessor,
      const attribute_id left_id,
      const TypedValue &right) const override {
    return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
        left_accessor,
        [&](auto *accessor) -> ColumnVector* {  // NOLINT(build/c++11)
      return impl_.applyToValueAccessorAndStaticValue(accessor, left_id, right);
    });
  }

  ColumnVector* applyToStaticValueAndValueAccessor(
      const TypedValue &left,
      ValueAccessor *right_accessor,
      const attribute_id right_id) const override {
    return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
        right_accessor,
        [&](auto *accessor) -> ColumnVector* {  // NOLINT(build/c++11)
      return impl_.applyToStaticValueAndValueAccessor(left, accessor, right_id);
    });
  }

  ColumnVector* applyToColumnVectorAndValueAccessor(
      const ColumnVector &left,
      ValueAccessor *right_accessor,
      const attribute_id right_id) const override {
    using LeftCVT = typename LeftGen::ColumnVectorType;
    DCHECK_EQ(left.isNative(), LeftCVT::kNative);
    using LeftAccessorT = ColumnVectorValueAccessor<LeftCVT>;
    LeftAccessorT left_accessor(static_cast<const LeftCVT&>(left));

    return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
        right_accessor,
        [&](auto *right_accessor) -> ColumnVector* {  // NOLINT(build/c++11)
    const std::size_t num_tuples = left_accessor.getNumTuples();
    DCHECK_EQ(num_tuples, right_accessor->getNumTuples());

    return impl_.applyToValueAccessors(num_tuples,
                                       &left_accessor, kInvalidAttributeID,
                                       right_accessor, right_id);
    });
  }

  ColumnVector* applyToValueAccessorAndColumnVector(
      ValueAccessor *left_accessor,
      const attribute_id left_id,
      const ColumnVector &right) const override {
    using RightCVT = typename RightGen::ColumnVectorType;
    DCHECK_EQ(right.isNative(), RightCVT::kNative);
    using RightAccessorT = ColumnVectorValueAccessor<RightCVT>;
    RightAccessorT right_accessor(static_cast<const RightCVT&>(right));

    return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
        left_accessor,
        [&](auto *left_accessor) -> ColumnVector* {  // NOLINT(build/c++11)
      const std::size_t num_tuples = right_accessor.getNumTuples();
      DCHECK_EQ(num_tuples, left_accessor->getNumTuples());

      return impl_.applyToValueAccessors(num_tuples,
                                         left_accessor, left_id,
                                         &right_accessor, kInvalidAttributeID);
    });
  }

  TypedValue accumulateColumnVector(
      const TypedValue &current,
      const ColumnVector &column_vector,
      std::size_t *num_tuples_applied) const override {
    constexpr bool is_supported =
        LeftType::kStaticTypeID == ResultType::kStaticTypeID &&
        LeftType::kMemoryLayout == kCxxNativePod &&
        std::is_copy_assignable<typename LeftType::cpptype>::value;

    using RightCVT = typename RightGen::ColumnVectorType;
    DCHECK_EQ(column_vector.isNative(), RightCVT::kNative);
    using RightAccessorT = ColumnVectorValueAccessor<RightCVT>;
    RightAccessorT accessor(static_cast<const RightCVT&>(column_vector));

    return impl_.template accumulateValueAccessor<is_supported>(
        current,
        &accessor,
        kInvalidAttributeID,
        num_tuples_applied);
  }

  TypedValue accumulateValueAccessor(
      const TypedValue &current,
      ValueAccessor *accessor,
      const attribute_id value_accessor_id,
      std::size_t *num_tuples_applied) const override {
    constexpr bool is_supported =
        LeftType::kStaticTypeID == ResultType::kStaticTypeID &&
        LeftType::kMemoryLayout == kCxxNativePod &&
        std::is_copy_assignable<typename LeftType::cpptype>::value;

    return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
        accessor,
        [&](auto *accessor) -> TypedValue {  // NOLINT(build/c++11)
      return impl_.template accumulateValueAccessor<is_supported>(
          current,
          accessor,
          value_accessor_id,
          num_tuples_applied);
    });
  }

 private:
  using LeftType = typename FunctorT::LeftArgumentType;
  using RightType = typename FunctorT::RightArgumentType;
  using ResultType = typename FunctorT::ResultType;

  using FuncSpec = typename FunctorSpecializer<FunctorT, SpecArgs...>::type;
  using LeftGen = Codegen<FuncSpec, LeftType>;
  using RightGen = Codegen<FuncSpec, RightType>;
  using ResultGen = Codegen<FuncSpec, ResultType>;

  template <bool left_nullable, bool right_nullable>
  struct Implementation;

  const FunctorT functor_;
  const Implementation<false, false> impl_;

  DISALLOW_COPY_AND_ASSIGN(UncheckedBinaryOperatorWrapperCodegen);
};

template <typename FunctorT, typename ...SpecArgs>
template <bool left_nullable, bool right_nullable>
struct UncheckedBinaryOperatorWrapperCodegen<FunctorT, SpecArgs...>
    ::Implementation {
  Implementation(const FunctorT &functor_in,
                 const Type &left_type_in,
                 const Type &right_type_in,
                 const Type &result_type_in)
      : functor(functor_in),
        left_type(left_type_in),
        right_type(right_type_in),
        result_type(result_type_in) {}

  inline TypedValue applyToTypedValues(const TypedValue &left,
                                       const TypedValue &right) const {
    if ((left_nullable && left.isNull()) || (right_nullable && right.isNull())) {
      return TypedValue(ResultType::kStaticTypeID);
    }

    return ResultGen::template ApplyBinaryTypedValue<LeftGen, RightGen>(
        LeftGen::ToNativeValueConst(left),
        RightGen::ToNativeValueConst(right),
        result_type,
        functor);
  }

  template <typename AccessorT>
  inline ColumnVector* applyToValueAccessorAndStaticValue(
      AccessorT *accessor,
      const attribute_id attr_id,
      const TypedValue &static_value) const {
    using AccessorGen = LeftGen;
    using StaticValueGen = RightGen;

    constexpr bool accessor_nullable = left_nullable;
    constexpr bool static_value_nullable = right_nullable;

    using ResultCVT = typename ResultGen::ColumnVectorType;
    ResultCVT *result_cv = new ResultCVT(result_type, accessor->getNumTuples());

    if (static_value_nullable && static_value.isNull()) {
      result_cv->fillWithNulls();
      return result_cv;
    }

    typename StaticValueGen::NativeTypeConst literal =
        StaticValueGen::ToNativeValueConst(static_value);
    accessor->beginIteration();
    while (accessor->next()) {
      typename AccessorGen::NativeTypeConstPtr arg_value =
          AccessorGen::template GetValuePtr<
              accessor_nullable, AccessorT>(accessor, attr_id);
      if (accessor_nullable && AccessorGen::IsNull(arg_value)) {
        result_cv->appendNullValue();
      } else {
        ResultGen::template ApplyBinaryColumnVector<LeftGen, RightGen>(
            AccessorGen::Dereference(arg_value), literal, functor, result_cv);
      }
    }
    return result_cv;
  }

  template <typename AccessorT>
  inline ColumnVector* applyToStaticValueAndValueAccessor(
      const TypedValue &static_value,
      AccessorT *accessor,
      const attribute_id attr_id) const {
    using AccessorGen = RightGen;
    using StaticValueGen = LeftGen;

    constexpr bool accessor_nullable = right_nullable;
    constexpr bool static_value_nullable = left_nullable;

    using ResultCVT = typename ResultGen::ColumnVectorType;
    ResultCVT *result_cv = new ResultCVT(result_type, accessor->getNumTuples());

    if (static_value_nullable && static_value.isNull()) {
      result_cv->fillWithNulls();
      return result_cv;
    }

    typename StaticValueGen::NativeTypeConst literal =
        StaticValueGen::ToNativeValueConst(static_value);
    accessor->beginIteration();
    while (accessor->next()) {
      typename AccessorGen::NativeTypeConstPtr arg_value =
          AccessorGen::template GetValuePtr<
              accessor_nullable, AccessorT>(accessor, attr_id);
      if (accessor_nullable && AccessorGen::IsNull(arg_value)) {
        result_cv->appendNullValue();
      } else {
        ResultGen::template ApplyBinaryColumnVector<LeftGen, RightGen>(
            literal, AccessorGen::Dereference(arg_value), functor, result_cv);
      }
    }
    return result_cv;
  }

  template <typename AccessorT>
  inline ColumnVector* applyToSingleValueAccessor(
      AccessorT *accessor,
      const attribute_id left_id,
      const attribute_id right_id) const {
    using ResultCVT = typename ResultGen::ColumnVectorType;
    ResultCVT *result_cv = new ResultCVT(result_type, accessor->getNumTuples());

    accessor->beginIteration();
    while (accessor->next()) {
      typename LeftGen::NativeTypeConstPtr left_value =
          LeftGen::template GetValuePtr<
              left_nullable, AccessorT>(accessor, left_id);
      if (left_nullable && LeftGen::IsNull(left_value)) {
        result_cv->appendNullValue();
        continue;
      }
      typename RightGen::NativeTypeConstPtr right_value =
          RightGen::template GetValuePtr<
              right_nullable, AccessorT>(accessor, right_id);
      if (right_nullable && RightGen::IsNull(right_value)) {
        result_cv->appendNullValue();
        continue;
      }
      ResultGen::template ApplyBinaryColumnVector<LeftGen, RightGen>(
          LeftGen::Dereference(left_value),
          RightGen::Dereference(right_value),
          functor,
          result_cv);
    }
    return result_cv;
  }

  template <typename LeftAccessorT, typename RightAccessorT>
  inline ColumnVector* applyToValueAccessors(const std::size_t num_tuples,
                                             LeftAccessorT *left_accessor,
                                             const attribute_id left_id,
                                             RightAccessorT *right_accessor,
                                             const attribute_id right_id) const {
    DCHECK_EQ(num_tuples, left_accessor->getNumTuples());
    DCHECK_EQ(num_tuples, right_accessor->getNumTuples());

    using ResultCVT = typename ResultGen::ColumnVectorType;
    ResultCVT *result_cv = new ResultCVT(result_type, num_tuples);

    left_accessor->beginIteration();
    right_accessor->beginIteration();
    while (left_accessor->next()) {
      right_accessor->next();
      typename LeftGen::NativeTypeConstPtr left_value =
          LeftGen::template GetValuePtr<
              left_nullable, LeftAccessorT>(left_accessor, left_id);
      if (left_nullable && LeftGen::IsNull(left_value)) {
        result_cv->appendNullValue();
        continue;
      }
      typename RightGen::NativeTypeConstPtr right_value =
          RightGen::template GetValuePtr<
              right_nullable, RightAccessorT>(right_accessor, right_id);
      if (right_nullable && RightGen::IsNull(right_value)) {
        result_cv->appendNullValue();
        continue;
      }
      ResultGen::template ApplyBinaryColumnVector<LeftGen, RightGen>(
          LeftGen::Dereference(left_value),
          RightGen::Dereference(right_value),
          functor,
          result_cv);
    }
    return result_cv;
  }

  template <bool supported, typename AccessorT>
  inline TypedValue accumulateValueAccessor(const TypedValue &current,
                                            AccessorT *accessor,
                                            const attribute_id attr_id,
                                            std::size_t *num_tuples_applied,
                                            std::enable_if_t<supported>* = 0) const {
    DCHECK(num_tuples_applied);

    *num_tuples_applied = 0;
    if (left_nullable && current.isNull()) {
      return result_type.makeNullValue();
    }

    using LeftCppType = typename LeftType::cpptype;
    using ResultCppType = typename ResultType::cpptype;
    ResultCppType accumulated = current.getLiteral<LeftCppType>();

    accessor->beginIteration();
    while (accessor->next()) {
      typename RightGen::NativeTypeConstPtr right_value =
          RightGen::template GetValuePtr<
              right_nullable, AccessorT>(accessor, attr_id);
      if (right_nullable && RightGen::IsNull(right_value)) {
        continue;
      }

      accumulated =
          ResultGen::FunctorSpecializer::Invoke(
              functor, accumulated, RightGen::Dereference(right_value));
      ++(*num_tuples_applied);
    }

    return TypedValue(accumulated);
  }

  template <bool supported, typename AccessorT>
  inline TypedValue accumulateValueAccessor(const TypedValue &current,
                                            AccessorT *accessor,
                                            const attribute_id attr_id,
                                            std::size_t *num_tuples_applied,
                                            std::enable_if_t<!supported>* = 0) const {
    LOG(FATAL) << "Unimplemented method UncheckedBinaryOperatorWrapperCodegen"
               << "::accumulateValueAccessor() because ResultType and LeftType "
               << "are not same or not native types.";
  }

  const FunctorT &functor;
  const Type &left_type;
  const Type &right_type;
  const Type &result_type;
};

template <typename FunctorT>
class BinaryOperationWrapper : public BinaryOperation {
 public:
  BinaryOperationWrapper()
      : BinaryOperation(),
        operation_name_(FunctorT::GetName()) {}

  std::string getName() const override {
    return operation_name_;
  }

  std::string getShortName() const override {
    return getName();
  }

  std::vector<OperationSignaturePtr> getSignatures() const override {
    return {
        OperationSignature::Create(
            getName(), {LeftType::kStaticTypeID, RightType::kStaticTypeID}, 0)
    };
  }

  bool canApplyTo(const Type &left,
                  const Type &right,
                  const std::vector<TypedValue> &static_arguments,
                  std::string *message) const override {
    DCHECK(left.getTypeID() == LeftType::kStaticTypeID);
    DCHECK(right.getTypeID() == RightType::kStaticTypeID);
    DCHECK(static_arguments.empty());
    return true;
  }

  const Type* getResultType(
      const Type &left,
      const Type &right,
      const std::vector<TypedValue> &static_arguments) const override {
    DCHECK(left.getTypeID() == LeftType::kStaticTypeID);
    DCHECK(right.getTypeID() == RightType::kStaticTypeID);
    DCHECK(static_arguments.empty());
    return getResultTypeImpl<ResultType::kIsParameterizedPod>(
        left, right, static_arguments);
  }

  UncheckedBinaryOperator* makeUncheckedBinaryOperator(
      const Type &left,
      const Type &right,
      const std::vector<TypedValue> &static_arguments) const override {
    DCHECK(left.getTypeID() == LeftType::kStaticTypeID);
    DCHECK(right.getTypeID() == RightType::kStaticTypeID);
    DCHECK(static_arguments.empty());
    return makeUncheckedBinaryOperatorImpl<
        std::is_default_constructible<FunctorT>::value>(
            left, right, static_arguments);
  }

 private:
  using LeftType = typename FunctorT::LeftArgumentType;
  using RightType = typename FunctorT::RightArgumentType;
  using ResultType = typename FunctorT::ResultType;

  template <bool functor_use_default_constructor>
  inline UncheckedBinaryOperator* makeUncheckedBinaryOperatorImpl(
      const Type &left,
      const Type &right,
      const std::vector<TypedValue> &static_arguments,
      std::enable_if_t<functor_use_default_constructor>* = 0) const {
    return new UncheckedBinaryOperatorWrapperCodegen<FunctorT>(
        left, right, *getResultType(left, right, static_arguments));
  }

  template <bool functor_use_default_constructor>
  inline UncheckedBinaryOperator* makeUncheckedBinaryOperatorImpl(
      const Type &left,
      const Type &right,
      const std::vector<TypedValue> &static_arguments,
      std::enable_if_t<!functor_use_default_constructor>* = 0) const {
    return new UncheckedBinaryOperatorWrapperCodegen<FunctorT>(
        left, right, *getResultType(left, right, static_arguments),
        static_cast<const LeftType&>(left),
        static_cast<const RightType&>(right));
  }

  template <bool result_type_has_parameter>
  inline const Type* getResultTypeImpl(
      const Type &left,
      const Type &right,
      const std::vector<TypedValue> &static_arguments,
      std::enable_if_t<!result_type_has_parameter>* = 0) const {
    return &TypeFactory::GetType(
        ResultType::kStaticTypeID,
        left.isNullable() || right.isNullable());
  }

  template <bool result_type_has_parameter>
  inline const Type* getResultTypeImpl(
      const Type &left,
      const Type &right,
      const std::vector<TypedValue> &static_arguments,
      std::enable_if_t<result_type_has_parameter>* = 0) const {
    return FunctorT::GetResultType(left, right);
  }

  const std::string operation_name_;

  DISALLOW_COPY_AND_ASSIGN(BinaryOperationWrapper);
};

template <typename LeftPack, typename RightPack,
          template <typename LeftT,
                    typename RightT,
                    typename ResultT> class FunctorT,
          template <typename LeftT,
                    typename RightT> class ResultGenerator>
struct BinaryFunctorCrossProductPack {
  template <std::size_t l, std::size_t r>
  inline static OperationPtr GenerateInner() {
    using LeftType = std::tuple_element_t<l, LeftPack>;
    using RightType = std::tuple_element_t<r, RightPack>;
    using ResultType = typename ResultGenerator<LeftType, RightType>::type;

    return std::make_shared<
        const BinaryOperationWrapper<
            FunctorT<LeftType, RightType, ResultType>>>();
  }

  template <std::size_t l, std::size_t ...Rs>
  inline static std::list<OperationPtr> GenerateRightHelper() {
    return { GenerateInner<l, Rs>()... };
  }

  template <std::size_t ...Ls, std::size_t ...Rs>
  inline static std::vector<std::list<OperationPtr>> GenerateLeftHelper(
      meta::IntegerSequence<Ls...> &&l_seq, meta::IntegerSequence<Rs...> &&r_seq) {
    return { GenerateRightHelper<Ls, Rs...>()... };
  }

  template <typename Dispatcher>
  inline static std::list<OperationPtr> GenerateOperations() {
    std::vector<std::list<OperationPtr>> op_list_groups =
        GenerateLeftHelper(typename meta::MakeSequence<std::tuple_size<LeftPack>::value>::type(),
                           typename meta::MakeSequence<std::tuple_size<RightPack>::value>::type());

    std::list<OperationPtr> operations;
    for (std::list<OperationPtr> &op_list : op_list_groups) {
      operations.splice(operations.end(), std::move(op_list));
    }
    return operations;
  }
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_UNARY_OPERATION_WRAPPER_HPP_
