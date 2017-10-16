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

#ifndef QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_SUBSTRING_OPERATION_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_SUBSTRING_OPERATION_HPP_

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/CharType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/VarCharType.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/containers/ColumnVectorUtil.hpp"
#include "types/operations/unary_operations/UnaryOperation.hpp"
#include "types/port/strnlen.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class ColumnVector;
class ValueAccessor;

/**
 * @brief Operation that extracts a number of characters from a string
 *        at a given starting position.
 */
class SubstringOperation : public UnaryOperation {
 public:
  SubstringOperation() {}

  std::string getName() const override {
    return "Substring";
  }

  std::string getShortName() const override {
    return "Substring";
  }

  std::vector<OperationSignaturePtr> getSignatures() const override {
    return {
        OperationSignature::Create(getName(), {kChar}, {kLong, kLong}),
        OperationSignature::Create(getName(), {kVarChar}, {kLong, kLong})
    };
  }

  bool canApplyTo(const Type &type,
                  const std::vector<TypedValue> &static_arguments,
                  std::string *message) const override {
    DCHECK(type.getTypeID() == kChar || type.getTypeID() == kVarChar);
    DCHECK(!static_arguments.empty() && static_arguments[0].getTypeID() == kLong);
    DCHECK(static_arguments.size() <= 2);

    if (static_arguments[0].getLiteral<std::int64_t>() <= 0) {
      *message = "The start position must be greater than 0";
      return false;
    }

    if (static_arguments.size() == 2) {
      DCHECK(static_arguments[1].getTypeID() == kLong);
      if (static_arguments[1].getLiteral<std::int64_t>() <= 0) {
        *message = "The substring length must be greater than 0";
        return false;
      }
    }

    return true;
  }

  const Type* getResultType(
      const Type &type,
      const std::vector<TypedValue> &static_arguments) const override {
    DCHECK(UnaryOperation::canApplyTo(type, static_arguments));

    std::size_t start_position;
    std::size_t substring_length;
    ExtractStaticArguments(static_arguments, &start_position, &substring_length);

    return &TypeFactory::GetType(TypeID::kChar,
                                 ComputeMaximumSubstringLength(type, start_position, substring_length),
                                 type.isNullable());
  }

  UncheckedUnaryOperator* makeUncheckedUnaryOperator(
      const Type &type,
      const std::vector<TypedValue> &static_arguments) const override;

 private:
  inline static void ExtractStaticArguments(
      const std::vector<TypedValue> &static_arguments,
      std::size_t *start_position,
      std::size_t *substring_length) {
    DCHECK_LE(1u, static_arguments.size());
    DCHECK_GE(2u, static_arguments.size());

    DCHECK(static_arguments[0].getTypeID() == kLong);
    *start_position =
        static_cast<std::size_t>(static_arguments[0].getLiteral<std::int64_t>() - 1);

    DCHECK(static_arguments.size() < 2u || static_arguments[1].getTypeID() == kLong);
    *substring_length =
        static_arguments.size() < 2u
            ? std::numeric_limits<std::size_t>::max()
            : static_cast<std::size_t>(static_arguments[1].getLiteral<std::int64_t>());
  }

  /**
   * @brief Compute an upper bound for the substring length regarding the input
   *        type and the substring_length_ field.
   *
   * @param type The type of the input, must be either CharType or VarCharType.
   */
  inline static std::size_t ComputeMaximumSubstringLength(
      const Type& type,
      const std::size_t start_position,
      const std::size_t substring_length) {
    DCHECK(type.getTypeID() == kChar || type.getTypeID() == kVarChar);

    const std::size_t input_maximum_length =
        type.getTypeID() == kChar
            ? static_cast<const CharType&>(type).getStringLength()
            : static_cast<const VarCharType&>(type).getStringLength();

    // Substring result should have length no greater than the minimum of
    // (1) the input string length subtract the start position, and
    // (2) the specified substring length.
    return std::min(input_maximum_length - start_position, substring_length);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(SubstringOperation);
};

template <bool null_terminated, bool input_nullable>
class SubstringUncheckedOperator : public UncheckedUnaryOperator {
 public:
  SubstringUncheckedOperator(const std::size_t start_position,
                             const std::size_t substring_length,
                             const std::size_t maximum_input_length,
                             const Type &result_type)
      : start_position_(start_position),
        substring_length_(substring_length),
        maximum_input_length_(maximum_input_length),
        result_type_(result_type) {
  }

  TypedValue applyToTypedValue(const TypedValue& argument) const override;

  ColumnVector* applyToColumnVector(const ColumnVector &argument) const override;

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  ColumnVector* applyToValueAccessor(ValueAccessor *accessor,
                                     const attribute_id argument_attr_id) const override;
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN
  ColumnVector* applyToValueAccessorForJoin(
      ValueAccessor *accessor,
      const bool use_left_relation,
      const attribute_id argument_attr_id,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const override;
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN

 private:
  inline void computeSubstring(const char *input, char *output) const;

  const std::size_t start_position_;
  const std::size_t substring_length_;
  const std::size_t maximum_input_length_;
  const Type &result_type_;

  DISALLOW_COPY_AND_ASSIGN(SubstringUncheckedOperator);
};

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

}  // namespace quickstep

#endif /* QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_SUBSTRING_OPERATION_HPP_ */
