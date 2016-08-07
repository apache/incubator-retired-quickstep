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

#include "types/operations/unary_operations/SubstringOperation.hpp"

#include <algorithm>
#include <tuple>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/containers/ColumnVectorUtil.hpp"
#include "types/operations/Operation.pb.h"
#include "types/port/strnlen.hpp"
#include "utility/TemplateUtil.hpp"

#include "glog/logging.h"

namespace quickstep {

serialization::UnaryOperation SubstringOperation::getProto() const {
  serialization::UnaryOperation proto;
  proto.set_operation_id(serialization::UnaryOperation::SUBSTRING);
  proto.SetExtension(serialization::SubstringOperation::start_position,
                     start_position_);
  proto.SetExtension(serialization::SubstringOperation::substring_length,
                     substring_length_);
  return proto;
}

UncheckedUnaryOperator* SubstringOperation::makeUncheckedUnaryOperatorForType(
    const Type &type) const {
  DCHECK(type.getSuperTypeID() == Type::kAsciiString);

  const std::size_t input_maximum_length =
      static_cast<const AsciiStringSuperType&>(type).getStringLength();
  const bool input_null_terminated = (type.getTypeID() == TypeID::kVarChar);

  const Type *result_type = resultTypeForArgumentType(type);
  DCHECK(result_type != nullptr);

  return CreateBoolInstantiatedInstance<SubstringUncheckedOperator, UncheckedUnaryOperator>(
      std::forward_as_tuple(start_position_,
                            computeMaximumSubstringLength(type),
                            input_maximum_length,
                            *result_type),
      input_null_terminated, type.isNullable());
}

template <bool null_terminated, bool input_nullable>
inline void SubstringUncheckedOperator<null_terminated, input_nullable>
    ::computeSubstring(const char *input,
                       char *output) const {
  std::size_t string_length =
      (null_terminated ? strlen(input) : strnlen(input, maximum_input_length_));

  if (start_position_ >= string_length) {
    *output = '\0';
    return;
  }

  const std::size_t actual_substring_length =
      std::min(string_length - start_position_, substring_length_);
  std::memcpy(output, input + start_position_, actual_substring_length);

  if (actual_substring_length < substring_length_) {
    output[actual_substring_length] = '\0';
  }
}

template <bool null_terminated, bool input_nullable>
TypedValue SubstringUncheckedOperator<null_terminated,
                                      input_nullable>
    ::applyToTypedValue(const TypedValue& argument) const {
  if (input_nullable && argument.isNull()) {
    return TypedValue(result_type_.getTypeID());
  }

  char *output_ptr = static_cast<char*>(std::malloc(substring_length_));
  computeSubstring(static_cast<const char*>(argument.getOutOfLineData()),
                   output_ptr);

  return TypedValue::CreateWithOwnedData(result_type_.getTypeID(),
                                         output_ptr,
                                         substring_length_);
}

template <bool null_terminated, bool input_nullable>
TypedValue SubstringUncheckedOperator<null_terminated,
                                      input_nullable>
    ::applyToDataPtr(const void *argument) const {
  if (input_nullable && argument == nullptr) {
    return TypedValue(result_type_.getTypeID());
  }

  char *output_ptr = static_cast<char*>(std::malloc(substring_length_));
  computeSubstring(static_cast<const char*>(argument),
                   output_ptr);

  return TypedValue::CreateWithOwnedData(result_type_.getTypeID(),
                                         output_ptr,
                                         substring_length_);
}

template <bool null_terminated, bool input_nullable>
ColumnVector* SubstringUncheckedOperator<null_terminated,
                                         input_nullable>
    ::applyToColumnVector(const ColumnVector &argument) const {
  return InvokeOnColumnVector(
      argument,
      [&](const auto &column_vector) -> ColumnVector* {  // NOLINT(build/c++11)
    NativeColumnVector *result =
        new NativeColumnVector(result_type_, column_vector.size());

    for (std::size_t cv_pos = 0;
         cv_pos < column_vector.size();
         ++cv_pos) {
      const char *input_ptr = static_cast<const char *>(
          column_vector.template getUntypedValue<input_nullable>(cv_pos));

      if (input_nullable && input_ptr == nullptr) {
        result->appendNullValue();
      } else {
        this->computeSubstring(input_ptr,
                               static_cast<char *>(result->getPtrForDirectWrite()));
      }
    }
    return result;
  });
}

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
template <bool null_terminated, bool input_nullable>
ColumnVector* SubstringUncheckedOperator<null_terminated,
                                         input_nullable>
    ::applyToValueAccessor(ValueAccessor *accessor,
                           const attribute_id argument_attr_id) const {
  return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
      accessor,
      [&](auto *accessor) -> ColumnVector* {  // NOLINT(build/c++11)
    NativeColumnVector *result =
        new NativeColumnVector(result_type_, accessor->getNumTuples());

    accessor->beginIteration();
    while (accessor->next()) {
      const char *input_ptr = static_cast<const char *>(
          accessor->template getUntypedValue<input_nullable>(argument_attr_id));

      if (input_nullable && (input_ptr == nullptr)) {
        result->appendNullValue();
      } else {
        this->computeSubstring(input_ptr,
                               static_cast<char *>(result->getPtrForDirectWrite()));
      }
    }
    return result;
  });
}
#endif

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN
template <bool null_terminated, bool input_nullable>
ColumnVector* SubstringUncheckedOperator<null_terminated,
                                         input_nullable>
    ::applyToValueAccessorForJoin(
        ValueAccessor *accessor,
        const bool use_left_relation,
        const attribute_id argument_attr_id,
        const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const {
  return InvokeOnValueAccessorNotAdapter(
      accessor,
      [&](auto *accessor) -> ColumnVector* {  // NOLINT(build/c++11)
    NativeColumnVector *result =
        new NativeColumnVector(result_type_, accessor->getNumTuples());

    for (const std::pair<tuple_id, tuple_id> &joined_pair : joined_tuple_ids) {
      const char *input_ptr = static_cast<const char *>(
          accessor->template getUntypedValueAtAbsolutePosition<input_nullable>(
              argument_attr_id,
              use_left_relation ? joined_pair.first : joined_pair.second));

      if (input_nullable && input_ptr == nullptr) {
        result->appendNullValue();
      } else {
        this->computeSubstring(input_ptr,
                               static_cast<char *>(result->getPtrForDirectWrite()));
      }
    }
    return result;
  });
}
#endif

}  // namespace quickstep
