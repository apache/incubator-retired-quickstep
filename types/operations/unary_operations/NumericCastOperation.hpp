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

#ifndef QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_NUMERIC_CAST_OPERATION_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_NUMERIC_CAST_OPERATION_HPP_

#include <cstddef>
#include <string>
#include <utility>

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN
#include <vector>
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN

#include "catalog/CatalogTypedefs.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/DoubleType.hpp"
#include "types/FloatType.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/operations/Operation.pb.h"
#include "types/operations/unary_operations/UnaryOperation.hpp"
#include "types/operations/unary_operations/UnaryOperationID.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrMap.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

// TODO(quickstep-team): Support CAST on Datetime, YearMonthInterval, DatetimeInterval, VarChar, Char.
//                       Currently we only need cast operations on numeric values.

/**
 * @brief UncheckedUnaryOperator for CAST.
 */
template <class SourceType, bool source_nullability, class TargetType, bool target_nullability>
class UncheckedNumericCastOperator : public UncheckedUnaryOperator {
 public:
  UncheckedNumericCastOperator()
      : UncheckedUnaryOperator(),
        target_type_(TargetType::Instance(target_nullability)) {
  }

  TypedValue applyToTypedValue(const TypedValue &argument) const override {
    if (source_nullability && argument.isNull()) {
      return TypedValue(TargetType::kStaticTypeID);
    }

    return TypedValue(static_cast<typename TargetType::cpptype>(
        argument.getLiteral<typename SourceType::cpptype>()));
  }

  TypedValue applyToDataPtr(const void *argument) const override {
    if (source_nullability && argument == nullptr) {
      return TypedValue(TargetType::kStaticTypeID);
    }

    return TypedValue(
        static_cast<const typename TargetType::cpptype>(
            *static_cast<const typename SourceType::cpptype*>(argument)));
  }

  ColumnVector* applyToColumnVector(const ColumnVector &argument) const override {
    DCHECK(NativeColumnVector::UsableForType(target_type_));
    DCHECK(argument.isNative());
    const NativeColumnVector &native_argument = static_cast<const NativeColumnVector&>(argument);
    NativeColumnVector *result = new NativeColumnVector(
        target_type_,
        native_argument.size());
    for (std::size_t pos = 0;
         pos < native_argument.size();
         ++pos) {
      const typename SourceType::cpptype *scalar_arg
          = static_cast<const typename SourceType::cpptype*>(
              native_argument.getUntypedValue<source_nullability>(pos));
      if (source_nullability && (scalar_arg == nullptr)) {
        result->appendNullValue();
      } else {
        *static_cast<typename TargetType::cpptype*>(result->getPtrForDirectWrite())
            = static_cast<typename TargetType::cpptype>(*scalar_arg);
      }
    }
    return result;
  }

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  ColumnVector* applyToValueAccessor(ValueAccessor *accessor,
                                     const attribute_id argument_attr_id) const override {
    DCHECK(NativeColumnVector::UsableForType(target_type_));
    return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
        accessor,
        [&](auto *accessor) -> ColumnVector* {  // NOLINT(build/c++11)
      NativeColumnVector *result = new NativeColumnVector(
          target_type_,
          accessor->getNumTuples());
      accessor->beginIteration();
      while (accessor->next()) {
        const typename SourceType::cpptype *scalar_arg
            = static_cast<const typename SourceType::cpptype*>(
                accessor->template getUntypedValue<source_nullability>(argument_attr_id));
        if (source_nullability && (scalar_arg == nullptr)) {
          result->appendNullValue();
        } else {
          *static_cast<typename TargetType::cpptype*>(result->getPtrForDirectWrite())
              = static_cast<typename TargetType::cpptype>(*scalar_arg);
        }
      }
      return result;
    });
  }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN
  ColumnVector* applyToValueAccessorForJoin(
      ValueAccessor *accessor,
      const bool use_left_relation,
      const attribute_id argument_attr_id,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const override {
    DCHECK(NativeColumnVector::UsableForType(target_type_));
    NativeColumnVector *result = new NativeColumnVector(target_type_,
                                                        joined_tuple_ids.size());
    InvokeOnValueAccessorNotAdapter(
        accessor,
        [&](auto *accessor) -> void {  // NOLINT(build/c++11)
      for (const std::pair<tuple_id, tuple_id> &joined_pair : joined_tuple_ids) {
        const typename SourceType::cpptype *scalar_arg
            = static_cast<const typename SourceType::cpptype*>(
                accessor->template getUntypedValueAtAbsolutePosition<source_nullability>(
                    argument_attr_id,
                    use_left_relation ? joined_pair.first : joined_pair.second));
        if (source_nullability && (scalar_arg == nullptr)) {
          result->appendNullValue();
        } else {
          *static_cast<typename TargetType::cpptype*>(result->getPtrForDirectWrite())
              = static_cast<typename TargetType::cpptype>(*scalar_arg);
        }
      }
    });
    return result;
  }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN

 private:
  const Type &target_type_;

  DISALLOW_COPY_AND_ASSIGN(UncheckedNumericCastOperator);
};

/**
 * @brief UnaryOperation for CAST.
 */
class NumericCastOperation : public UnaryOperation {
 public:
  serialization::UnaryOperation getProto() const override {
    serialization::UnaryOperation proto;
    proto.set_operation_id(serialization::UnaryOperation::CAST);
    proto.MutableExtension(serialization::CastOperation::target_type)
        ->CopyFrom(getTargetType().getProto());

    return proto;
  }

  /**
   * @brief Get a reference to the singleton instance of this Operation.
   *
   * @param target_type The target type to coerce input values to.
   * @return A reference to the singleton instance of this Operation.
   **/
  static const NumericCastOperation& Instance(const Type &target_type) {
    static PtrMap<const Type*, NumericCastOperation> instance_map;
    PtrMap<const Type*, NumericCastOperation>::iterator instance_map_it =
        instance_map.find(&target_type);
    if (instance_map_it == instance_map.end()) {
      instance_map_it = instance_map.insert(&target_type,
                                            new NumericCastOperation(target_type)).first;
    }
    return *(instance_map_it->second);
  }

  /**
   * @return The target type for coercion.
   */
  const Type& getTargetType() const {
    return target_type_;
  }

  std::string getName() const override {
    return std::string(kUnaryOperationNames[static_cast<std::size_t>(operation_id_)])
        .append("(")
        .append(target_type_.getName())
        .append(")");
  }

  bool canApplyToType(const Type &type) const override {
    return target_type_.isCoercibleFrom(type);
  }

  const Type* resultTypeForArgumentType(const Type &type) const override {
    if (canApplyToType(type)) {
      return &target_type_;
    } else {
      return nullptr;
    }
  }

  const Type* fixedNullableResultType() const override {
    return &target_type_.getNullableVersion();
  }

  bool resultTypeIsPlausible(const Type &result_type) const override {
    return result_type.equals(target_type_)
           || result_type.equals(target_type_.getNullableVersion());
  }

  const Type* pushDownTypeHint(const Type *type_hint) const override {
    return &target_type_;
  }

  TypedValue applyToChecked(const TypedValue &argument,
                            const Type &argument_type) const override {
    return target_type_.coerceValue(argument, argument_type);
  }

  UncheckedUnaryOperator* makeUncheckedUnaryOperatorForType(const Type &type) const override {
    switch (type.getTypeID()) {
      case kInt:
        return makeUncheckedUnaryOperatorHelperForSourceNullability<IntType>(type);
      case kLong:
        return makeUncheckedUnaryOperatorHelperForSourceNullability<LongType>(type);
      case kFloat:
        return makeUncheckedUnaryOperatorHelperForSourceNullability<FloatType>(type);
      case kDouble:
        return makeUncheckedUnaryOperatorHelperForSourceNullability<DoubleType>(type);
      default:
        FATAL_ERROR("Unhandled type " << kTypeNames[type.getTypeID()]);
    }
  }

 private:
  explicit NumericCastOperation(const Type &target_type)
      : UnaryOperation(UnaryOperationID::kCast),
        target_type_(target_type) {}

  template <class SourceType>
  UncheckedUnaryOperator* makeUncheckedUnaryOperatorHelperForSourceNullability(const Type &type) const {
    if (type.isNullable()) {
      return makeUncheckedUnaryOperatorHelperForTargetType<SourceType, true>();
    } else {
      return makeUncheckedUnaryOperatorHelperForTargetType<SourceType, false>();
    }
  }

  template <class SourceType, bool source_nullability>
  UncheckedUnaryOperator* makeUncheckedUnaryOperatorHelperForTargetType() const {
    switch (target_type_.getTypeID()) {
      case kInt:
        return makeUncheckedUnaryOperatorHelperForTargetNullability<SourceType, source_nullability, IntType>();
      case kLong:
        return makeUncheckedUnaryOperatorHelperForTargetNullability<SourceType, source_nullability, LongType>();
      case kFloat:
        return makeUncheckedUnaryOperatorHelperForTargetNullability<SourceType, source_nullability, FloatType>();
      case kDouble:
        return makeUncheckedUnaryOperatorHelperForTargetNullability<SourceType, source_nullability, DoubleType>();
      default:
        FATAL_ERROR("Unhandled type " << kTypeNames[target_type_.getTypeID()]);
    }
  }

  template <class SourceType, bool source_nullability, class TargetType>
  UncheckedUnaryOperator* makeUncheckedUnaryOperatorHelperForTargetNullability() const {
    if (target_type_.isNullable()) {
      return new UncheckedNumericCastOperator<SourceType, source_nullability, TargetType, true>();
    } else {
      return new UncheckedNumericCastOperator<SourceType, source_nullability, TargetType, false>();
    }
  }

  const Type& target_type_;

  DISALLOW_COPY_AND_ASSIGN(NumericCastOperation);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_NUMERIC_CAST_OPERATION_HPP_
