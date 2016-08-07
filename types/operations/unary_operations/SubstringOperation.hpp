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
#include <cstdlib>
#include <cstring>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/Operation.pb.h"
#include "types/operations/unary_operations/UnaryOperation.hpp"
#include "types/operations/unary_operations/UnaryOperationID.hpp"
#include "utility/HashPair.hpp"
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
  /**
   * @brief Get a reference to the singleton instance of this Operation for
   *        the given (start_position, substring_length) pair.
   **/
  static const SubstringOperation& Instance(const std::size_t start_position,
                                            const std::size_t substring_length) {
    // TODO(jianqiao): This is a temporary solution that creates a new instance
    // for each distinct pair of start_position and substring_length arguments.
    // The number of instances may be unbounded if quickstep continuously accepts
    // queries that call SUBSTRING with different arguments. It still remains to
    // design a better long-term solution.
    const auto hash = [](const auto &pair) {
      return hash_combine_detail::HashCombiner<std::size_t>::CombineHashes(pair.first, pair.second);
    };
    static std::unordered_map<std::pair<std::size_t, std::size_t>,
                              std::unique_ptr<const SubstringOperation>,
                              decltype(hash)> instance_map(10, hash);

    const std::pair<std::size_t, std::size_t> key_pair =
        std::make_pair(start_position, substring_length);
    auto imit = instance_map.find(key_pair);
    if (imit != instance_map.end()) {
      return *imit->second;
    } else {
      const SubstringOperation *instance =
          new SubstringOperation(start_position, substring_length);
      instance_map.emplace(key_pair,
                           std::unique_ptr<const SubstringOperation>(instance));
      return *instance;
    }
  }

  serialization::UnaryOperation getProto() const override;

  bool canApplyToType(const Type &type) const override {
    return (type.getSuperTypeID() == Type::kAsciiString);
  }

  const Type *resultTypeForArgumentType(const Type &type) const override {
    if (type.getSuperTypeID() == Type::kAsciiString) {
      // Result is a Char string.
      return &TypeFactory::GetType(TypeID::kChar,
                                   computeMaximumSubstringLength(type),
                                   type.isNullable());
    }
    return nullptr;
  }

  const Type* fixedNullableResultType() const override {
    // Result type is not fixed (i.e. can have various lengths).
    return nullptr;
  }

  bool resultTypeIsPlausible(const Type &result_type) const override {
    // Result can be coerced to Char or VarChar.
    return (result_type.getSuperTypeID() == Type::kAsciiString);
  }

  const Type* pushDownTypeHint(const Type *type_hint) const override {
    // Input can only be a string, but we don't know the length.
    return nullptr;
  }

  TypedValue applyToChecked(const TypedValue &argument,
                            const Type &argument_type) const override {
    DCHECK(canApplyToType(argument_type));

    const Type *result_type = resultTypeForArgumentType(argument_type);
    DCHECK(result_type != nullptr);

    if (argument_type.isNullable() && argument.isNull()) {
      return result_type->makeNullValue();
    } else {
      const std::size_t result_length = computeMaximumSubstringLength(argument_type);
      char *output_ptr = static_cast<char*>(std::malloc(result_length));
      const char *input_ptr = static_cast<const char*>(argument.getOutOfLineData());

      const std::size_t string_length = argument.getAsciiStringLength();
      if (start_position_ >= string_length) {
        *output_ptr = '\0';
      } else {
        const std::size_t actual_substring_length =
            std::min(string_length - start_position_, substring_length_);
        std::memcpy(output_ptr, input_ptr + start_position_, actual_substring_length);
        if (actual_substring_length < result_length) {
          output_ptr[actual_substring_length] = '\0';
        }
      }

      return TypedValue::CreateWithOwnedData(result_type->getTypeID(),
                                             output_ptr,
                                             result_length);
    }
  }

  UncheckedUnaryOperator* makeUncheckedUnaryOperatorForType(const Type &type) const override;

 private:
  /**
   * @brief Constructor.
   *
   * @param input_type The data type of the input argument for substring.
   * @param start_position The 0-base starting position of the substring.
   * @param substring_length The substring length.
   */
  SubstringOperation(const std::size_t start_position,
                     const std::size_t substring_length)
      : UnaryOperation(UnaryOperationID::kSubstring),
        start_position_(start_position),
        substring_length_(substring_length) {
  }

  /**
   * @brief Compute an upper bound for the substring length regarding the input
   *        type and the substring_length_ field.
   *
   * @param type The type of the input, must be either CharType or VarCharType.
   */
  inline std::size_t computeMaximumSubstringLength(const Type& type) const {
      DCHECK(type.getSuperTypeID() == Type::kAsciiString);

      // Substring result should have length no greater than the minimum of
      // (1) the input string length subtract the start position, and
      // (2) the specified substring length.
     return std::min(static_cast<const AsciiStringSuperType&>(type).getStringLength() - start_position_,
                     substring_length_);
  }

  const std::size_t start_position_;
  const std::size_t substring_length_;

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

  TypedValue applyToDataPtr(const void *argument) const override;

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

}  // namespace quickstep

#endif /* QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_SUBSTRING_OPERATION_HPP_ */
