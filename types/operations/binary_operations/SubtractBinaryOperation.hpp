/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_SUBTRACT_BINARY_OPERATION_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_SUBTRACT_BINARY_OPERATION_HPP_

#include <utility>

#include "types/TypedValue.hpp"
#include "types/operations/binary_operations/ArithmeticBinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class Type;
class UncheckedBinaryOperator;

/** \addtogroup Types
 *  @{
 */

/**
 * @brief The BinaryOperation for subtraction.
 *
 * @note SubtractBinaryOperation is not commutative: the left argument is the
 *       minuend and the right argument is the subtrahend.
 **/
class SubtractBinaryOperation : public ArithmeticBinaryOperation {
 public:
  /**
   * @brief Get a reference to the singleton instance of this Operation.
   *
   * @return A reference to the singleton instance of this Operation.
   **/
  static const SubtractBinaryOperation& Instance() {
    static SubtractBinaryOperation instance;
    return instance;
  }

  bool canApplyToTypes(const Type &left,
                       const Type &right) const override;

  const Type* resultTypeForArgumentTypes(const Type &left,
                                         const Type &right) const override;

  const Type* resultTypeForPartialArgumentTypes(const Type *left,
                                                const Type *right) const override;

  bool partialTypeSignatureIsPlausible(const Type *result_type,
                                       const Type *left_argument_type,
                                       const Type *right_argument_type) const override;

  std::pair<const Type*, const Type*> pushDownTypeHint(
      const Type *result_type_hint) const override;

  TypedValue applyToChecked(const TypedValue &left,
                            const Type &left_type,
                            const TypedValue &right,
                            const Type &right_type) const override;

  UncheckedBinaryOperator *makeUncheckedBinaryOperatorForTypes(const Type &left,
                                                               const Type &right) const override;

 private:
  SubtractBinaryOperation()
      : ArithmeticBinaryOperation(BinaryOperationID::kSubtract) {
  }

  DISALLOW_COPY_AND_ASSIGN(SubtractBinaryOperation);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_SUBTRACT_BINARY_OPERATION_HPP_
