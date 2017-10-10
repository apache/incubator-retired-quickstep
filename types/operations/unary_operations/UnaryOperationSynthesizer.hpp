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

#ifndef QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_UNARY_OPERATION_WRAPPER_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_UNARY_OPERATION_WRAPPER_HPP_

#include <cstddef>
#include <string>
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
#include "types/operations/unary_operations/UnaryOperation.hpp"
#include "types/operations/utility/OperationSynthesizeUtil.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

template <typename ArgumentT, typename ResultT>
struct UnaryFunctor {
  typedef ArgumentT ArgumentType;
  typedef ResultT ResultType;

  static constexpr Operation
      ::OperationSuperTypeID kOperationSuperTypeID = Operation::kUnaryOperation;
};

template <typename FunctorT, typename ...SpecArgs>
class UncheckedUnaryOperatorSynthesizer : public UncheckedUnaryOperator {
 public:
  template <typename ...FunctorArgs>
  UncheckedUnaryOperatorSynthesizer(const Type &argument_type,
                                    const Type &result_type,
                                    FunctorArgs &&...args)
      : functor_(std::forward<FunctorArgs>(args)...),
        impl_(functor_, argument_type, result_type) {
    DCHECK(argument_type.getTypeID() == ArgumentType::kStaticTypeID);
    DCHECK(result_type.getTypeID() == ResultType::kStaticTypeID);
  }

  TypedValue applyToTypedValue(const TypedValue &argument) const override {
    return impl_.applyToTypedValue(argument);
  }

  ColumnVector* applyToColumnVector(const ColumnVector &argument) const override {
    using ArgumentCVT = typename ArgumentGen::ColumnVectorType;
    DCHECK_EQ(argument.getImplementation(), ArgumentCVT::kImplementation);

    using ArgumentAccessorT = ColumnVectorAccessor<ArgumentCVT>;
    ArgumentAccessorT accessor(static_cast<const ArgumentCVT&>(argument));
    return impl_.applyToValueAccessor(&accessor, 0);
  }

  ColumnVector* applyToValueAccessor(ValueAccessor *accessor,
                                     const attribute_id attr_id) const override {
    return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
        accessor,
        [&](auto *accessor) -> ColumnVector* {  // NOLINT(build/c++11)
      return impl_.applyToValueAccessor(accessor, attr_id);
    });
  }

 private:
  using ArgumentType = typename FunctorT::ArgumentType;
  using ResultType = typename FunctorT::ResultType;

  using FuncSpec = typename FunctorSpecializer<FunctorT, SpecArgs...>::type;
  using ArgumentGen = OperationCodegen<FuncSpec, ArgumentType>;
  using ResultGen = OperationCodegen<FuncSpec, ResultType>;

  template <bool argument_nullable>
  struct Implementation;

  const FunctorT functor_;
  const Implementation<true> impl_;

  DISALLOW_COPY_AND_ASSIGN(UncheckedUnaryOperatorSynthesizer);
};

template <typename FunctorT, typename ...SpecArgs>
template <bool argument_nullable>
struct UncheckedUnaryOperatorSynthesizer<FunctorT, SpecArgs...>
    ::Implementation {
  Implementation(const FunctorT &functor_in,
                 const Type &argument_type_in,
                 const Type &result_type_in)
      : functor(functor_in),
        argument_type(static_cast<const ArgumentType&>(argument_type_in)),
        result_type(static_cast<const ResultType&>(result_type_in)) {}

  inline TypedValue applyToTypedValue(const TypedValue &argument) const {
    if (argument_nullable && argument.isNull()) {
      return TypedValue(ResultType::kStaticTypeID);
    }

    return ResultGen::template ApplyUnaryTypedValue<ArgumentGen>(
        ArgumentGen::ToNativeValueConst(argument, argument_type),
        result_type,
        functor);
  }

  template <typename AccessorT>
  inline ColumnVector* applyToValueAccessor(AccessorT *accessor,
                                            const attribute_id attr_id) const {
    using ResultCVT = typename ResultGen::ColumnVectorType;
    ResultCVT *result_cv = new ResultCVT(result_type, accessor->getNumTuples());

    std::unique_ptr<typename ArgumentGen::NativeType> value_cache;
    accessor->beginIteration();
    while (accessor->next()) {
      typename ArgumentGen::NativeTypeConstPtr arg_value =
          ArgumentGen::template GetValuePtr<argument_nullable, AccessorT>(
              accessor, attr_id, argument_type, &value_cache);

      if (argument_nullable && ArgumentGen::IsNull(arg_value)) {
        result_cv->appendNullValue();
      } else {
        ResultGen::template ApplyUnaryColumnVector<ArgumentGen>(
            ArgumentGen::Dereference(arg_value), functor, result_cv);
      }
    }
    return result_cv;
  }

  const FunctorT &functor;
  const ArgumentType &argument_type;
  const ResultType &result_type;
};

template <typename FunctorT>
class UnaryOperationSynthesizer : public UnaryOperation {
 public:
  UnaryOperationSynthesizer()
      : UnaryOperation(),
        operation_name_(FunctorT::GetName()) {}

  std::string getName() const override {
    return operation_name_;
  }

  std::string getShortName() const override {
    return getName();
  }

  std::vector<OperationSignaturePtr> getSignatures() const override {
    return {
        OperationSignature::Create(getName(), {ArgumentType::kStaticTypeID}, 0)
    };
  }

  bool canApplyTo(const Type &argument_type,
                  const std::vector<TypedValue> &static_arguments,
                  std::string *message) const override {
    DCHECK(argument_type.getTypeID() == ArgumentType::kStaticTypeID);
    DCHECK(static_arguments.empty());
    return true;
  }

  const Type* getResultType(
      const Type &argument_type,
      const std::vector<TypedValue> &static_arguments) const override {
    DCHECK(argument_type.getTypeID() == ArgumentType::kStaticTypeID);
    DCHECK(static_arguments.empty());
    return getResultTypeImpl<HasGetType<FunctorT>::value>(
        argument_type, static_arguments);
  }

  UncheckedUnaryOperator* makeUncheckedUnaryOperator(
      const Type &argument_type,
      const std::vector<TypedValue> &static_arguments) const override {
    DCHECK(argument_type.getTypeID() == ArgumentType::kStaticTypeID);
    DCHECK(static_arguments.empty());
    return makeUncheckedUnaryOperatorImpl<
        std::is_default_constructible<FunctorT>::value>(
            argument_type, static_arguments);
  }

 private:
  using ArgumentType = typename FunctorT::ArgumentType;
  using ResultType = typename FunctorT::ResultType;

  QUICKSTEP_TRAIT_HAS_STATIC_METHOD(HasGetType, GetType);

  template <bool functor_use_default_constructor>
  inline UncheckedUnaryOperator* makeUncheckedUnaryOperatorImpl(
      const Type &argument_type,
      const std::vector<TypedValue> &static_arguments,
      std::enable_if_t<functor_use_default_constructor> * = 0) const {
    return new UncheckedUnaryOperatorSynthesizer<FunctorT>(
        argument_type, *getResultType(argument_type, static_arguments));
  }

  template <bool functor_use_default_constructor>
  inline UncheckedUnaryOperator* makeUncheckedUnaryOperatorImpl(
      const Type &argument_type,
      const std::vector<TypedValue> &static_arguments,
      std::enable_if_t<!functor_use_default_constructor> * = 0) const {
    return new UncheckedUnaryOperatorSynthesizer<FunctorT>(
        argument_type, *getResultType(argument_type, static_arguments),
        static_cast<const ArgumentType&>(argument_type));
  }

  template <bool user_defined_get_type>
  inline const Type* getResultTypeImpl(
      const Type &argument_type,
      const std::vector<TypedValue> &static_arguments,
      std::enable_if_t<!user_defined_get_type &&
                       ResultType::kMemoryLayout == kCxxInlinePod> * = 0) const {
    return &TypeFactory::GetType(ResultType::kStaticTypeID,
                                 argument_type.isNullable());
  }

  template <bool user_defined_get_type>
  inline const Type* getResultTypeImpl(
      const Type &argument_type,
      const std::vector<TypedValue> &static_arguments,
      std::enable_if_t<!user_defined_get_type &&
                       ResultType::kMemoryLayout == kCxxGeneric> * = 0) const {
    return &TypeFactory::GetType(ResultType::kStaticTypeID,
                                 std::vector<GenericValue>(),
                                 argument_type.isNullable());
  }

  template <bool user_defined_get_type>
  inline const Type* getResultTypeImpl(
      const Type &argument_type,
      const std::vector<TypedValue> &static_arguments,
      std::enable_if_t<user_defined_get_type ||
                       ResultType::kMemoryLayout == kParInlinePod ||
                       ResultType::kMemoryLayout == kParOutOfLinePod> * = 0) const {
    // TODO(refactor-type): Specialize with regard to static arguments.
    return FunctorT::GetResultType(argument_type);
  }

  const std::string operation_name_;

  DISALLOW_COPY_AND_ASSIGN(UnaryOperationSynthesizer);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_UNARY_OPERATION_WRAPPER_HPP_
