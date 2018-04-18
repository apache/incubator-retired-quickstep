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
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "types/operations/Operation.hpp"
#include "types/operations/Operation.pb.h"
#include "types/TypedValue.hpp"
#include "types/operations/unary_operations/UnaryOperationID.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class ColumnVector;
class Type;
class ValueAccessor;

/** \addtogroup Types
 *  @{
 */

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
   * @brief Apply to a data item via a pointer without type-checking.
   *
   * @param argument The data item to apply to.
   * @return The literal result of the operation.
   **/
  virtual TypedValue applyToDataPtr(const void *argument) const = 0;

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

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN
  /**
   * @brief Apply to an attribute of a list of joined tuples in a
   *        ValueAccessor.
   *
   * @param accessor The ValueAccessor to apply to.
   * @param use_left_relation If true, this UnaryOperation's argument is
   *        assumed to be taken from the left relation in the pairs of
   *        joined_tuple_ids. If false, the right relation.
   * @param argument_attr_id The attribute ID of the argument in accessor.
   * @param joined_tuple_ids A series of pairs of tuple ids from the left and
   *        right relations in a join.
   * @return A ColumnVector of literal results of the operation.
   **/
  virtual ColumnVector* applyToValueAccessorForJoin(
      ValueAccessor *accessor,
      const bool use_left_relation,
      const attribute_id argument_attr_id,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const = 0;
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN

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
   * @brief Generate a serialized Protocol Buffer representation of
   *        this UnaryOperation.
   *
   * @return The serialized Protocol Buffer representation of this UnaryOperation.
   **/
  virtual serialization::UnaryOperation getProto() const;

  /**
   * @brief Determine the ID of this UnaryOperation.
   *
   * @return The ID of this UnaryOperation.
   **/
  inline UnaryOperationID getUnaryOperationID() const {
    return operation_id_;
  }

  /**
   * @brief Get a human-readable name for this UnaryOperation.
   *
   * @return A human-readable name for this UnaryOperation.
   **/
  virtual std::string getName() const {
    return kUnaryOperationNames[static_cast<std::size_t>(operation_id_)];
  }

  /**
   * @brief Get a human-readable short name (e.g. "-") for this UnaryOperation.
   *
   * @return A human-readable short name for this BinaryOperation.
   **/
  virtual std::string getShortName() const {
    return kUnaryOperationShortNames[static_cast<std::size_t>(operation_id_)];
  }

  /**
   * @brief Determine whether this UnaryOperation can apply to the specified
   *        Type.
   *
   * @param type The argument Type to check.
   * @return Whether this UnaryOperation can apply to type.
   **/
  virtual bool canApplyToType(const Type &type) const = 0;

  /**
   * @brief Determine the Type of the result from applying this UnaryOperation
   *        to an argument of the specified Type.
   *
   * @param type The argument Type to check.
   * @return The Type of the result from applying this UnaryOperation to type
   *         (NULL if not applicable).
   **/
  virtual const Type* resultTypeForArgumentType(const Type &type) const = 0;

  /**
   * @brief If this UnaryOperation always yields the same Type (or if the ONLY
   *        difference between 2 possible return Types is nullability), return
   *        that Type, otherwise return NULL.
   *
   * @return The nullable version of this UnaryOperation's fixed result Type,
   *         if applicable.
   **/
  virtual const Type* fixedNullableResultType() const = 0;

  /**
   * @brief Check if a particular Type might possibly be returned by this
   *        UnaryOperation, assuming an appropriate argument type.
   * @note A nullable result type may be considered plausible even if a
   *       particular UnaryOperation never actually returns NULL values, so
   *       long as the non-nullable version of the type would otherwise be
   *       plausible.
   *
   * @param result_type Check whether this Type can possibly be returned by
   *        this UnaryOperation.
   * @return true if result_type can be returned by this UnaryOperation, false
   *         otherwise.
   **/
  virtual bool resultTypeIsPlausible(const Type &result_type) const = 0;

  /**
   * @brief Get a "hint" Type for the argument to this UnaryOperation based on
   *        a hint for this UnaryOperation's result type. If possible, returns
   *        a pointer to a Type that, when given to this UnaryOperation as an
   *        argument, yields values of the desired type (i.e. calling
   *        resultTypeForArgumentType() on the returned type should return the
   *        original type_hint).
   * @note In some cases (e.g. NumericCastOperation) there may be multiple
   *       types that can be used as arguments to this UnaryOperation that will
   *       all yield the desired type_hint. In such cases, this method will
   *       pick one Type based on its own implementation-specific preference.
   *
   * @param type_hint A hint about what Type the result of this UnaryOperation
   *        should have. May be NULL to indicate no preference.
   * @return A type hint for the argument to this UnaryOperation based on
   *         type_hint, or NULL if no suitable Type exists.
   **/
  virtual const Type* pushDownTypeHint(const Type *type_hint) const = 0;

  /**
   * @brief Apply this UnaryOperation to a TypedValue.
   * @warning It is an error to call this method if this UnaryOperation can not
   *          be applied to argument_type. If in doubt, check canApplyToType()
   *          first.
   *
   * @param argument The TypedValue to apply to.
   * @param argument_type The Type that argument belongs to.
   * @return The literal result of the operation.
   **/
  virtual TypedValue applyToChecked(const TypedValue &argument,
                                    const Type &argument_type) const = 0;

  /**
   * @brief Create an UncheckedUnaryOperator which can apply to items of the
   *        specified type.
   * @warning The resulting UncheckedUnaryOperator performs no type-checking
   *          whatsoever. Nonetheless, it is useful in situations where many
   *          data items of the same, known type are to be operated on (for
   *          example, over many tuples in the same relation).
   *
   * @param type The Type of argument to apply to.
   * @return An UncheckedUnaryOperator which applies this UnaryOperation to
   *         the specified Type.
   * @exception OperationInapplicableToType This UnaryOperation is not
   *            applicable to type.
   **/
  virtual UncheckedUnaryOperator* makeUncheckedUnaryOperatorForType(const Type &type) const = 0;

 protected:
  explicit UnaryOperation(const UnaryOperationID operation_id)
      : Operation(Operation::kUnaryOperation,
                  kUnaryOperationNames[
                      static_cast<typename std::underlying_type<UnaryOperationID>::type>(operation_id)],
                  kUnaryOperationShortNames[
                      static_cast<typename std::underlying_type<UnaryOperationID>::type>(operation_id)]),
        operation_id_(operation_id) {
  }

  const UnaryOperationID operation_id_;

 private:
  DISALLOW_COPY_AND_ASSIGN(UnaryOperation);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_UNARY_OPERATION_HPP_
