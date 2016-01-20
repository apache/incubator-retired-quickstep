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
#include "types/operations/Operation.pb.h"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class ColumnVector;
class Type;
class ValueAccessor;

/** \addtogroup Types
 *  @{
 */

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
   * @brief Apply to two data items via pointers without type-checking.
   *
   * @param left The left argument data item to apply to.
   * @param right The right argument data item to apply to.
   * @return The literal result of the operation.
   **/
  virtual TypedValue applyToDataPtrs(const void *left,
                                     const void *right) const = 0;

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

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN
  virtual ColumnVector* applyToValueAccessorAndStaticValueForJoin(
      ValueAccessor *left_accessor,
      const bool left_accessor_is_left_relation,
      const attribute_id left_id,
      const TypedValue &right,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const = 0;

  virtual ColumnVector* applyToStaticValueAndValueAccessorForJoin(
      const TypedValue &left,
      ValueAccessor *right_accessor,
      const bool right_accessor_is_left_relation,
      const attribute_id right_id,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const = 0;

  virtual ColumnVector* applyToColumnVectorAndValueAccessorForJoin(
      const ColumnVector &left,
      ValueAccessor *right_accessor,
      const bool right_accessor_is_left_relation,
      const attribute_id right_id,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const = 0;

  virtual ColumnVector* applyToValueAccessorAndColumnVectorForJoin(
      ValueAccessor *left_accessor,
      const bool left_accessor_is_left_relation,
      const attribute_id left_id,
      const ColumnVector &right,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const = 0;
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN_WITH_BINARY_EXPRESSIONS
  virtual ColumnVector* applyToValueAccessorsForJoin(
      ValueAccessor *left_accessor,
      const bool left_accessor_is_left_relation,
      const attribute_id left_id,
      ValueAccessor *right_accessor,
      const bool right_accessor_is_left_relation,
      const attribute_id right_id,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const = 0;
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN_WITH_BINARY_EXPRESSIONS

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
  /**
   * @brief Generate a serialized Protocol Buffer representation of
   *        this BinaryOperation.
   *
   * @return The serialized Protocol Buffer representation of
   *         this BinaryOperation.
   **/
  serialization::BinaryOperation getProto() const;

  /**
   * @brief Determine the ID of this BinaryOperation
   *
   * @return The ID of this BinaryOperation
   **/
  inline BinaryOperationID getBinaryOperationID() const {
    return operation_id_;
  }

  /**
   * @brief Get a human-readable name for this BinaryOperation.
   *
   * @return A human-readable name for this BinaryOperation.
   **/
  virtual std::string getName() const {
    return kBinaryOperationNames[static_cast<std::size_t>(operation_id_)];
  }

  /**
   * @brief Get a human-readable short name (e.g. "+", "-", "*", "/") for this
   *        BinaryOperation.
   *
   * @return A human-readable short name for this BinaryOperation.
   **/
  virtual std::string getShortName() const {
    return kBinaryOperationShortNames[static_cast<std::size_t>(operation_id_)];
  }

  /**
   * @brief Determine whether this BinaryOperation can apply to the specified
   *        Types.
   * @note When the Types that an operator can apply to are changed,
   *       its definition in the function catalog of the query optimizer
   *       needs to be modified accordingly.
   *
   * @param left The left argument Type to check.
   * @param right The right argument Type to check.
   * @return Whether this BinaryOperation can apply to left and right.
   **/
  virtual bool canApplyToTypes(const Type &left, const Type &right) const = 0;

  /**
   * @brief Determine the Type of the result from applying this BinaryOperation
   *        to arguments of the specified Types.
   *
   * @param left The left argument Type to check.
   * @param right The right argument Type to check.
   * @return The Type of the result from applying this BinaryOperation to left
   *         and right (NULL if not applicable).
   **/
  virtual const Type* resultTypeForArgumentTypes(const Type &left, const Type &right) const = 0;

  /**
   * @brief Similar to resultTypeForArgumentTypes(), but allows either or both
   *        argument types to be NULL to indicate an unknown (but presumed
   *        nullable) type. If the return type can be unambiguously determined
   *        based on the incomplete information about the argument types, that
   *        will be returned. Otherwise, NULL will be returned.
   * @note This method returns NULL in cases where the return Type is
   *       ambiguous, and also in cases where this BinaryOperation can not
   *       possibly be applied to argument(s) of one of the known types left or
   *       right. See also partialTypeSignatureIsPlausible(), which will return
   *       true in the former case (ambiguous result type) but false in the
   *       latter (can not apply to specfied argument types).
   *
   * @param left The left argument Type, or NULL if unknown (but presumed
   *        nullable).
   * @param right The right argument Type, or NULL if unknown (but presumed
   *        nullable).
   * @return The result Type for the given partial argument type information,
   *         if such a result Type can be unambiguously determined. Otherwise
   *         NULL.
   **/
  virtual const Type* resultTypeForPartialArgumentTypes(const Type *left,
                                                        const Type *right) const = 0;

  /**
   * @brief Determine if an incomplete type signature is plausible for this
   *        BinaryOperation. Specifically, checks if for each unknown Type in
   *        the partial Type signature, there is some concrete Type that could
   *        be substituted such that the overall signature becomes valid.
   *
   * @param result_type The result Type for this BinaryOperation, or NULL if
   *        unknown.
   * @param left_argument_type The Type for the left argument to this
   *        BinaryOperation, or NULL if unknown (but presumed nullable).
   * @param right_argument_type The Type for the right argument to this
   *        BinaryOperation, or NULL if unknown (but presumed nullable).
   * @return true if the specified partial Type signature is plausible.
   **/
  virtual bool partialTypeSignatureIsPlausible(const Type *result_type,
                                               const Type *left_argument_type,
                                               const Type *right_argument_type) const = 0;

  // TODO(chasseur): This hinting interface is currently rather rudimentary,
  // and we would like it to be more flexible to capture the full semantics of
  // binary operations. Three issues immediately come to mind:
  //
  //     1. We should be able to use a category of Types or multiple Types as
  //        a hint. For example, if we hint that the result of
  //        DivideBinaryOperation should be DatetimeIntervalType, we know that
  //        the hint for the left argument should be DatetimeIntervalType, but
  //        the hint for the right argument can be any numeric type.
  //     2. We should know something about whether or not a BinaryOperation is
  //        commutative with respect to its argument types. For example,
  //        AddBinaryOperation is commutative. If we hint that its result
  //        should be DatetimeType, then one of its arguments should be
  //        DatetimeType and the other should be one of the interval types, but
  //        which is left and which is right does not matter (although it does
  //        matter that there is one of each).
  //     3. We may want to use "sideways" hinting, where we tentatively resolve
  //        the types for arguments and note where we encounter "strong"
  //        information about what a type should be (i.e. from the catalog or
  //        an explicitly-typed literal in the parser), then using that to help
  //        hint "weak" guesses about typing for the other argument.
  //
  // These issues can be difficult to solve in general, and it may not be worth
  // doing in the current optimizer if another more sophisticated optimizer
  // frontend is to be added in the future.

  /**
   * @brief Get a pair of "hint" Types for the arguments to this
   *        BinaryOperation based on a hint for this BinaryOperation's result
   *        type. If possible, returns pointers to Types that, when given to
   *        this BinaryOperation as arguments, yield values of the desired type
   *        (i.e. calling resultTypeForArgumentTypes() on the returned types
   *        should return the original type_hint).
   *
   * This method is designed to help the query optimizer's Resolver determine
   * the type of literal values that appear in SQL queries. The Resolver
   * propagates known Type information (e.g. from the Catalog) top-down through
   * parsed expression trees, eventually using a Type hint at a leaf
   * ParseLiteralValue node in the tree to concretize that literal value as the
   * appropriate Type. This is especially important for NULL literals that
   * appear in SQL queries, because a NULL by itself does not convey any type
   * information and could be an instance of any nullable type. Hints generated
   * by this method are to be treated as hints only, not as hard or
   * authoritative requirements. After resolving literals, the Resolver
   * propagates known concrete Types bottom-up through the tree and explicitly
   * checks canApplyToTypes() for correctness.
   *
   * @note In some cases, either or both arguments could take on more than one
   *       possible Type and this BinaryOperation would still yield the
   *       specified result_type_hint (e.g. for arithmetic operations, if
   *       result_type_hint is DoubleType, then the arguments could both be
   *       DoubleType, or DoubleType and a different numeric Type, or even
   *       LongType and FloatType). In such cases, the most precise suitable
   *       Type is used as the hint.
   * @note In some cases, the choice of argument Type hints for a given
   *       result_type_hint is ambiguous and there is no single precise Type
   *       that can represent values of any of the possible argument types
   *       (e.g. an addition returning Datetime could be
   *       Datetime + DatetimeInterval, DatetimeInterval + Datetime,
   *       Datetime + YearMonthInterval, or YearMonthInterval + Datetime, and
   *       none of these Types is coercible to the others or to a common more
   *       precise type). In such cases, NULL is used to represent an ambiguous
   *       or unknown hint.
   *
   * @param result_type_hint A hint about what Type the result of this
   *        BinaryOperation should have. May be NULL to indicate no preference.
   * @return A pair of type hints for the left and right arguments to this
   *         BinaryOperation based on type_hint, or NULL if no suitable Type
   *         exists or if the hint is ambiguous.
   **/
  virtual std::pair<const Type*, const Type*> pushDownTypeHint(
      const Type *result_type_hint) const = 0;

  /**
   * @brief Apply this BinaryOperation to two TypedValues.
   * @warning It is an error to call this method if this BinaryOperation can
   *          not be applied to arguments of the specified types. If in doubt,
   *          check canApplyToTypes() first.
   *
   * @param left The left argument TypedValue to apply to.
   * @param left_type The Type that left belongs to.
   * @param right The right argument TypedValue to apply to.
   * @param right_type The Type that right belongs to.
   * @return The literal result of the operation.
   **/
  virtual TypedValue applyToChecked(const TypedValue &left,
                                    const Type &left_type,
                                    const TypedValue &right,
                                    const Type &right_type) const = 0;

  /**
   * @brief Create an UncheckedBinaryOperator which can apply to items of the
   *        specified types.
   * @warning The resulting UncheckedBinaryOperator performs no type-checking
   *          whatsoever. Nonetheless, it is useful in situations where many
   *          data items of the same, known types are to be operated on (for
   *          example, over many tuples in the same table).
   *
   * @param left The left argument Type to compare.
   * @param right The right argument Type to compare.
   * @return An UncheckedBinaryOperator which applies this BinaryOperation to
   *         the specified Types.
   * @exception OperationInapplicableToType This BinaryOperation is not
   *            applicable to either left or right.
   **/
  virtual UncheckedBinaryOperator* makeUncheckedBinaryOperatorForTypes(const Type &left,
                                                                       const Type &right) const = 0;

 protected:
  explicit BinaryOperation(const BinaryOperationID operation_id)
      : Operation(Operation::kBinaryOperation,
                  kBinaryOperationNames[
                      static_cast<typename std::underlying_type<BinaryOperationID>::type>(operation_id)],
                  kBinaryOperationShortNames[
                      static_cast<typename std::underlying_type<BinaryOperationID>::type>(operation_id)]),
        operation_id_(operation_id) {
  }

  const BinaryOperationID operation_id_;

 private:
  DISALLOW_COPY_AND_ASSIGN(BinaryOperation);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_BINARY_OPERATION_HPP_
