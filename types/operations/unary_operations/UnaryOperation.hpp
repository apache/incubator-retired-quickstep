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

#ifndef QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_UNARY_OPERATION_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_UNARY_OPERATION_HPP_

#include <cstddef>
#include <string>
#include <type_traits>

#include "catalog/CatalogTypedefs.hpp"
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

class UnaryOperation;
typedef std::shared_ptr<const UnaryOperation> UnaryOperationPtr;

/**
 * @brief A unary operator which can be quickly applied to data items WITHOUT
 *        checking their type.
 **/
class UncheckedUnaryOperator {
 public:
  /**
   * @brief Virtual destructor.
   **/
  virtual ~UncheckedUnaryOperator() {
  }

  /**
   * @brief Apply to a TypedValue without type-checking.
   *
   * @param argument The TypedValue to apply to.
   * @return The literal result of the operation.
   **/
  virtual TypedValue applyToTypedValue(const TypedValue &argument) const = 0;

  /**
   * @brief Apply to a vector of values without type-checking.
   *
   * @param argument The argument ColumnVector to apply to.
   * @return A ColumnVector of literal results of the operation.
   **/
  virtual ColumnVector* applyToColumnVector(const ColumnVector &argument) const = 0;

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  /**
   * @brief Apply to an attribute of all tuples in a ValueAccessor.
   *
   * @param accessor The ValueAccessor to apply to.
   * @param argument_attr_id The attribute ID of the argument in accessor.
   * @return A ColumnVector of literal results of the operation.
   **/
  virtual ColumnVector* applyToValueAccessor(ValueAccessor *accessor,
                                             const attribute_id argument_attr_id) const = 0;
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

 protected:
  UncheckedUnaryOperator() {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(UncheckedUnaryOperator);
};

/**
 * @brief An operation which has a single argument and returns a single
 *        TypedValue.
 * @note When any UnaryOperation is applied to a NULL value, the result is
 *       always NULL.
 **/
class UnaryOperation : public Operation {
 public:
  /**
   * @brief Get a human-readable name for this UnaryOperation.
   *
   * @return A human-readable name for this UnaryOperation.
   **/
  virtual std::string getName() const = 0;

  /**
   * @brief Get a human-readable short name (e.g. "-") for this UnaryOperation.
   *
   * @return A human-readable short name for this BinaryOperation.
   **/
  virtual std::string getShortName() const {
    return getName();
  }

  virtual bool canApplyTo(const Type &argument_type,
                          const std::vector<TypedValue> &static_arguments,
                          std::string *message) const = 0;

  virtual const Type* getResultType(
      const Type &argument_type,
      const std::vector<TypedValue> &static_arguments) const = 0;

  virtual UncheckedUnaryOperator* makeUncheckedUnaryOperator(
      const Type &argument_type,
      const std::vector<TypedValue> &static_arguments) const = 0;

  bool canApplyTo(const Type &argument_type,
                  const std::vector<TypedValue> &static_arguments) const {
    std::string message;
    return canApplyTo(argument_type, static_arguments, &message);
  }

 protected:
  UnaryOperation()
      : Operation(Operation::kUnaryOperation) {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(UnaryOperation);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_UNARY_OPERATION_HPP_
