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

#include "types/operations/unary_operations/CastOperation.hpp"

#include <algorithm>
#include <map>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

#include "types/CharType.hpp"
#include "types/DoubleType.hpp"
#include "types/FloatType.hpp"
#include "types/GenericValue.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/MetaType.hpp"
#include "types/Type.hpp"
#include "types/TypeUtil.hpp"
#include "types/TypedValue.hpp"
#include "types/VarCharType.hpp"
#include "types/operations/unary_operations/CastFunctorOverloads.hpp"
#include "types/operations/unary_operations/UnaryOperationSynthesizer.hpp"
#include "types/port/strnlen.hpp"
#include "utility/EqualsAnyConstant.hpp"
#include "utility/StringUtil.hpp"
#include "utility/meta/Common.hpp"

namespace quickstep {

namespace {

template <typename SourceType, typename TargetType>
UncheckedUnaryOperator* MakeUncheckedCastOperatorConstructorSpec(
    const SourceType &source_type,
    const TargetType &target_type,
    decltype(new CastFunctor<SourceType, TargetType>()) * = 0) {
  return new UncheckedUnaryOperatorSynthesizer<
      CastFunctor<SourceType, TargetType>>(
          source_type, target_type);
}

template <typename SourceType, typename TargetType>
UncheckedUnaryOperator* MakeUncheckedCastOperatorConstructorSpec(
    const SourceType &source_type,
    const TargetType &target_type,
    decltype(new CastFunctor<SourceType, TargetType>(source_type, target_type)) * = 0) {
  return new UncheckedUnaryOperatorSynthesizer<
      CastFunctor<SourceType, TargetType>>(
          source_type, target_type, source_type, target_type);
}

template <typename SourceType, typename TargetType>
UncheckedUnaryOperator* MakeUncheckedCastOperator(
    const Type &source_type,
    const Type &target_type,
    std::enable_if_t<meta::IsCompleteType<CastFunctor<SourceType, TargetType>>::value> * = 0) {
  return MakeUncheckedCastOperatorConstructorSpec(
      static_cast<const SourceType&>(source_type),
      static_cast<const TargetType&>(target_type));
}

template <typename SourceType, typename TargetType>
UncheckedUnaryOperator* MakeUncheckedCastOperator(
    const Type &source_type,
    const Type &target_type,
    std::enable_if_t<!meta::IsCompleteType<CastFunctor<SourceType, TargetType>>::value> * = 0) {
  LOG(FATAL) << "Unsupported cast from type " << source_type.getName()
             << " to type " << target_type.getName();
}

}  // namespace

const Type* CastOperation::ExtractTargetType(
    const Type &type,
    const std::vector<TypedValue> &static_arguments) {
  if (static_arguments.size() != 1) {
    return nullptr;
  }
  const GenericValue meta_type_value =
      GenericValue::CreateWithTypedValue(MetaType::InstanceNonNullable(),
                                         static_arguments.front());
  if (meta_type_value.isNull() || meta_type_value.getTypeID() != kMetaType) {
    return nullptr;
  }
  return meta_type_value.getLiteral<kMetaType>();
}

bool CastOperation::canApplyTo(
    const Type &type,
    const std::vector<TypedValue> &static_arguments,
    std::string *message) const {
  const Type *target_type = ExtractTargetType(type, static_arguments);
  if (target_type == nullptr) {
    *message = "Invalid target type";
    return false;
  }
  if (!target_type->isCoercibleFrom(type)) {
    *message = "Unsupported cast from " + type.getName() +
               " to " + target_type->getName();
    return false;
  }
  return true;
}

const Type* CastOperation::getResultType(
    const Type &type,
    const std::vector<TypedValue> &static_arguments) const {
  const Type *target_type = ExtractTargetType(type, static_arguments);
  DCHECK(target_type != nullptr);
  return target_type;
}

UncheckedUnaryOperator* CastOperation::makeUncheckedUnaryOperator(
    const Type &type,
    const std::vector<TypedValue> &static_arguments) const {
  const Type *result_type = getResultType(type, static_arguments);
  DCHECK(result_type != nullptr);

  const TypeID source_type_id = type.getTypeID();
  const TypeID target_type_id = result_type->getTypeID();

  return InvokeOnTypeID(
      source_type_id,
      [&](auto source) -> UncheckedUnaryOperator* {
    return InvokeOnTypeID(
        target_type_id,
        [&](auto target) -> UncheckedUnaryOperator* {
      return MakeUncheckedCastOperator<
          typename TypeIDTrait<decltype(source)::value>::TypeClass,
          typename TypeIDTrait<decltype(target)::value>::TypeClass>(type, *result_type);
    });
  });
}

}  // namespace quickstep
