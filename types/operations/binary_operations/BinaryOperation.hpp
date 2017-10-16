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

#ifndef QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_BINARY_OPERATION_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_BINARY_OPERATION_HPP_

#include <cstddef>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/Operation.hpp"
#include "types/operations/OperationSignature.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class ColumnVector;
class Type;
class ValueAccessor;

/** \addtogroup Types
 *  @{
 */

class BinaryOperation;
typedef std::shared_ptr<const BinaryOperation> BinaryOperationPtr;

/**
 * @brief A binary operator which can be quickly applied to data items WITHOUT
 *        checking their types.
 **/
class UncheckedBinaryOperator {
 public:
  /**
   * @brief Virtual destructor.
   **/
  virtual ~UncheckedBinaryOperator() {
  }

  /**
   * @brief Apply to two TypedValues without type-checking.
   *
   * @param left The left argument TypedValue to apply to.
   * @param right The right argument TypedValue to apply to.
   * @return The literal result of the operation.
   **/
  virtual TypedValue applyToTypedValues(const TypedValue &left,
                                        const TypedValue &right) const = 0;

  /**
   * @brief Apply to two equally-sized vectors of values without type-checking.
   *
   * @param left The left argument ColumnVector to apply to.
   * @param right The right argument ColumnVector to apply to.
   * @return A ColumnVector of literal results of the operation.
   **/
  virtual ColumnVector* applyToColumnVectors(
      const ColumnVector &left,
      const ColumnVector &right) const = 0;

  /**
   * @brief Apply to a vector of values on the left with a fixed static value
   *        on the right.
   *
   * @param left The left argument ColumnVector to apply to.
   * @param right The right static value to apply to.
   * @return A ColumnVector of literal results of the operation.
   **/
  virtual ColumnVector* applyToColumnVectorAndStaticValue(
      const ColumnVector &left,
      const TypedValue &right) const = 0;

  /**
   * @brief Apply to a fixed static value on the left with a vector of values
   *        on the right.
   *
   * @param left The left static value to apply to.
   * @param right The right argument ColumnVector to apply to.
   * @return A ColumnVector of literal results of the operation.
   **/
  virtual ColumnVector* applyToStaticValueAndColumnVector(
      const TypedValue &left,
      const ColumnVector &right) const = 0;

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  /**
   * @brief Apply to two attributes of all tuples in a ValueAccessor.
   *
   * @param accessor The ValueAccessor to apply to.
   * @param left_id The attribute ID of the left argument in accessor.
   * @param right_id The attribute ID of the right argument in accessor.
   * @return A ColumnVector of literal results of the operation.
   **/
  virtual ColumnVector* applyToSingleValueAccessor(
      ValueAccessor *accessor,
      const attribute_id left_id,
      const attribute_id right_id) const = 0;

  /**
   * @brief Apply to an attribute of all tuples in a ValueAccessor on the left
   *        with a fixed static value on the right.
   *
   * @param left_accessor The ValueAccessor to apply to.
   * @param left_id The attribute ID of the left argument in accessor.
   * @param right The right static value to apply to.
   * @return A ColumnVector of literal results of the operation.
   **/
  virtual ColumnVector* applyToValueAccessorAndStaticValue(
      ValueAccessor *left_accessor,
      const attribute_id left_id,
      const TypedValue &right) const = 0;

  /**
   * @brief Apply to a fixed static value on the left and an attribute of all
   *        tuples in a ValueAccessor on the right.
   *
   * @param left The left static value to apply to.
   * @param right_accessor The ValueAccessor to apply to.
   * @param right_id The attribute ID of the right argument in accessor.
   * @return A ColumnVector of literal results of the operation.
   **/
  virtual ColumnVector* applyToStaticValueAndValueAccessor(
      const TypedValue &left,
      ValueAccessor *right_accessor,
      const attribute_id right_id) const = 0;

  /**
   * @brief Apply to a vector of values on the left and an attribute of all
   *        tuples in a ValueAccessor on the right.
   *
   * @param left The left argument ColumnVector to apply to.
   * @param right_accessor The ValueAccessor to apply to.
   * @param right_id The attribute ID of the right argument in accessor.
   * @return A ColumnVector of literal results of the operation.
   **/
  virtual ColumnVector* applyToColumnVectorAndValueAccessor(
      const ColumnVector &left,
      ValueAccessor *right_accessor,
      const attribute_id right_id) const = 0;

  /**
   * @brief Apply to an attribute of all tuples in a ValueAccessor on the keft
   *        and a vector of values on the right.
   *
   * @param left_accessor The ValueAccessor to apply to.
   * @param left_id The attribute ID of the left argument in accessor.
   * @param right The right argument ColumnVector to apply to.
   * @return A ColumnVector of literal results of the operation.
   **/
  virtual ColumnVector* applyToValueAccessorAndColumnVector(
      ValueAccessor *left_accessor,
      const attribute_id left_id,
      const ColumnVector &right) const = 0;
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

  /**
   * @brief Apply binary operator on column vector accumulating on the current
   * value. This is useful for summing/multiplying the values of the column
   * vector returning the sum/product of that value with current value.
   * Essentially, this does the following:
   *    for (value in column_values) {
   *      if (value NOT NULL) {
   *        current = current OP value;
   *      }
   *    }
   *
   * @param current The current (or initial) value. The running current value
   *        is always the left operand.
   * @param column_vector Column vector to accumulate on. The column value is
   *        always the right operand. NULL values in the column are ignored.
   * @param num_tuples_applied An output argument to indicate the number of
   *        tuples in column vector where the operator was applied. NULL values
   *        are not counted. This is number of tuples in column vector in
   *        non-nullable column vectors, and number of tuples with non-NULL
   *        values in nullable column vectors.
   * @note Left type should be same as result type to use this method.
   **/
  virtual TypedValue accumulateColumnVector(
      const TypedValue &current,
      const ColumnVector &column_vector,
      std::size_t *num_tuples_applied) const = 0;

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  /**
   * @brief Apply binary operator on a column in value accessor accumulating on
   * the current value. This is useful for summing/multiplying the values of the
   * column returning the sum/product of that value with current value.
   * Essentially, this does the following:
   *    for (value in column_values) {
   *      if (value NOT NULL) {
   *        current = current OP value;
   *      }
   *    }
   *
   * @param current The current (or initial) value. The running current value
   *        is always the left operand.
   * @param accessor The value accessor to which the column accumulated on
   *        belongs to. The column value is always the right operand. NULL
   *        values in the column are ignored.
   * @param value_accessor_id The attribute ID in the accessor on which to
   *        accumulate.
   * @param num_tuples_applied An output argument to indicate the number of
   *        tuples in column where the operator was applied. NULL values are not
   *        counted. This is number of tuples in column in non-nullable columns,
   *        and number of tuples with non-NULL values in nullable columns.
   * @note Left type should be same as result type to use this method.
   **/
  virtual TypedValue accumulateValueAccessor(
      const TypedValue &current,
      ValueAccessor *accessor,
      const attribute_id value_accessor_id,
      std::size_t *num_tuples_applied) const = 0;
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

 protected:
  UncheckedBinaryOperator() {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(UncheckedBinaryOperator);
};

/**
 * @brief An operation which has two arguments and returns a single TypedValue.
 * @note When any argument of a BinaryOperation is a NULL value, the result is
 *       always NULL.
 **/
class BinaryOperation : public Operation {
 public:
  virtual std::string getName() const {
    return "NoName";
  }

  virtual std::string getShortName() const {
    return getName();
  }

  // TODO
  virtual bool isCommutative() const {
    return false;
  }

  virtual bool canApplyTo(const Type &left,
                          const Type &right,
                          const std::vector<TypedValue> &static_arguments,
                          std::string *message) const = 0;

  bool canApplyTo(const Type &left,
                  const Type &right,
                  const std::vector<TypedValue> &static_arguments = {}) const {
    std::string message;
    return canApplyTo(left, right, static_arguments, &message);
  }

  virtual const Type* getResultType(
      const Type &left,
      const Type &right,
      const std::vector<TypedValue> &static_arguments = {}) const = 0;

  virtual UncheckedBinaryOperator* makeUncheckedBinaryOperator(
      const Type &left,
      const Type &right,
      const std::vector<TypedValue> &static_arguments = {}) const = 0;

 protected:
  BinaryOperation()
      : Operation(Operation::kBinaryOperation) {}

 private:
  DISALLOW_COPY_AND_ASSIGN(BinaryOperation);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_BINARY_OPERATION_HPP_
