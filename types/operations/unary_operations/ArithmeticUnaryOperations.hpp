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

#ifndef QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_ARITHMETIC_UNARY_OPERATIONS_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_ARITHMETIC_UNARY_OPERATIONS_HPP_

#include "types/TypedValue.hpp"
#include "types/operations/unary_operations/UnaryOperation.hpp"
#include "types/operations/unary_operations/UnaryOperationID.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class Type;

/** \addtogroup Types
 *  @{
 */

/**
 * @brief A UnaryOperation which applies to and yields numeric values.
 **/
class ArithmeticUnaryOperation : public UnaryOperation {
 public:
  bool canApplyToType(const Type &type) const override;

  const Type* resultTypeForArgumentType(const Type &type) const override;

  const Type* pushDownTypeHint(const Type *type_hint) const override;

 protected:
  explicit ArithmeticUnaryOperation(const UnaryOperationID operation_id)
      : UnaryOperation(operation_id) {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ArithmeticUnaryOperation);
};

/**
 * @brief The UnaryOperation for negation.
 **/
class NegateUnaryOperation : public ArithmeticUnaryOperation {
 public:
  /**
   * @brief Get a reference to the singleton instance of this Operation.
   *
   * @return A reference to the singleton instance of this Operation.
   **/
  static const NegateUnaryOperation& Instance() {
    static NegateUnaryOperation instance;
    return instance;
  }

  const Type* fixedNullableResultType() const override {
    return nullptr;
  }

  bool resultTypeIsPlausible(const Type &result_type) const override;

  TypedValue applyToChecked(const TypedValue &argument,
                            const Type &argument_type) const override;

  UncheckedUnaryOperator* makeUncheckedUnaryOperatorForType(const Type &type) const override;

 private:
  NegateUnaryOperation()
      : ArithmeticUnaryOperation(UnaryOperationID::kNegate) {
  }

  DISALLOW_COPY_AND_ASSIGN(NegateUnaryOperation);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_ARITHMETIC_UNARY_OPERATIONS_HPP_
