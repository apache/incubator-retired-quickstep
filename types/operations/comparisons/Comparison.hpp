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

#ifndef QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_COMPARISON_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_COMPARISON_HPP_

#include <type_traits>

#include "catalog/CatalogTypedefs.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/Operation.hpp"
#include "types/operations/Operation.pb.h"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class ColumnVector;
class TupleIdSequence;
class Type;
class ValueAccessor;

/** \addtogroup Types
 *  @{
 */

/**
 * @brief An comparator which can be used to quickly compare data items WITHOUT
 *        checking their types.
 **/
class UncheckedComparator {
 public:
  /**
   * @brief Virtual destructor.
   **/
  virtual ~UncheckedComparator() {
  }

  /**
   * @brief Compare two TypedValues without type-checking.
   *
   * @param left The left TypedValue to compare.
   * @param right The right TypedValue to compare.
   * @return Whether the comparison is true for the given TypedValues.
   **/
  virtual bool compareTypedValues(const TypedValue &left, const TypedValue &right) const = 0;

  /**
   * @brief Compare data items without type-checking via pointers.
   *
   * @param left The left data item to compare.
   * @param right The right data item to compare.
   * @return Whether the comparison is true for the given data items.
   **/
  virtual bool compareDataPtrs(const void *left, const void *right) const = 0;

  /**
   * @brief Compare a TypedValue with a raw data pointer without
   *        type-checking.
   * @note See also compareDataPtrWithTypedValue().
   *
   * @param left The left TypedValue to compare.
   * @param right The right data pointer to compare.
   * @return Whether the comparison is true for the given data items.
   **/
  virtual bool compareTypedValueWithDataPtr(const TypedValue &left,
                                            const void *right) const = 0;

  /**
   * @brief Compare a raw data pointer with a TypedValue without
   *        type-checking.
   * @note See also compareTypedValueWithDataPtr().
   *
   * @param left The left data pointer to compare.
   * @param right The right TypedValue to compare.
   * @return Whether the comparison is true for the given data items.
   **/
  virtual bool compareDataPtrWithTypedValue(const void *left,
                                            const TypedValue &right) const = 0;

  /**
   * @brief Apply to two equally-sized vectors of values without type-checking.
   *
   * @param left The left argument ColumnVector to apply to.
   * @param right The right argument ColumnVector to apply to.
   * @param filter An optional TupleIdSequence indicating which tuples the
   *        comparison should actually be checked for. Tuples not in filter
   *        will not be checked and will not appear in the result.
   * @param existence_bitmap An optional TupleIdSequence indicating which
   *        tuple IDs actually exist in a TupleStorageSubBlock that the left
   *        and right ColumnVectors were generated from (may be needed to
   *        correctly set tuple IDs in the result, as ColumnVectors do not
   *        have any "holes" in their sequence of values, but some
   *        TupleStorageSubBlock implementations may have holes in their
   *        sequence of tuple IDs).
   * @return A TupleIdSequence indicating matches for the comparison.
   **/
  virtual TupleIdSequence* compareColumnVectors(
      const ColumnVector &left,
      const ColumnVector &right,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const;

  /**
   * @brief Apply to a vector of values on the left with a fixed static value
   *        on the right.
   *
   * @param left The left argument ColumnVector to apply to.
   * @param right The right static value to apply to.
   * @param filter An optional TupleIdSequence indicating which tuples the
   *        comparison should actually be checked for. Tuples not in filter
   *        will not be checked and will not appear in the result.
   * @param existence_bitmap An optional TupleIdSequence indicating which
   *        tuple IDs actually exist in a TupleStorageSubBlock that the left
   *        ColumnVector was generated from (may be needed to correctly set
   *        tuple IDs in the result, as ColumnVectors do not have any "holes"
   *        in their sequence of values, but some TupleStorageSubBlock
   *        implementations may have holes in their sequence of tuple IDs).
   * @return A TupleIdSequence indicating matches for the comparison.
   **/
  virtual TupleIdSequence* compareColumnVectorAndStaticValue(
      const ColumnVector &left,
      const TypedValue &right,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const;

  /**
   * @brief Apply to a fixed static value on the left with a vector of values
   *        on the right.
   *
   * @param left The left static value to apply to.
   * @param right The right argument ColumnVector to apply to.
   * @param filter An optional TupleIdSequence indicating which tuples the
   *        comparison should actually be checked for. Tuples not in filter
   *        will not be checked and will not appear in the result.
   * @param existence_bitmap An optional TupleIdSequence indicating which
   *        tuple IDs actually exist in a TupleStorageSubBlock that the right
   *        ColumnVector was generated from (may be needed to correctly set
   *        tuple IDs in the result, as ColumnVectors do not have any "holes"
   *        in their sequence of values, but some TupleStorageSubBlock
   *        implementations may have holes in their sequence of tuple IDs).
   * @return A TupleIdSequence indicating matches for the comparison.
   **/
  virtual TupleIdSequence* compareStaticValueAndColumnVector(
      const TypedValue &left,
      const ColumnVector &right,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const;

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  /**
   * @brief Apply to two attributes of all tuples in a ValueAccessor.
   *
   * @param accessor The ValueAccessor to apply to.
   * @param left_id The attribute ID of the left argument in accessor.
   * @param right_id The attribute ID of the right argument in accessor.
   * @param filter An optional TupleIdSequence indicating which tuples the
   *        comparison should actually be checked for. Tuples not in filter
   *        will not be checked and will not appear in the result.
   * @return A TupleIdSequence indicating matches for the comparison.
   **/
  virtual TupleIdSequence* compareSingleValueAccessor(
      ValueAccessor *accessor,
      const attribute_id left_id,
      const attribute_id right_id,
      const TupleIdSequence *filter) const;

  /**
   * @brief Apply to an attribute of all tuples in a ValueAccessor on the left
   *        with a fixed static value on the right.
   *
   * @param left_accessor The ValueAccessor to apply to.
   * @param left_id The attribute ID of the left argument in accessor.
   * @param right The right static value to apply to.
   * @param filter An optional TupleIdSequence indicating which tuples the
   *        comparison should actually be checked for. Tuples not in filter
   *        will not be checked and will not appear in the result.
   * @return A TupleIdSequence indicating matches for the comparison.
   **/
  virtual TupleIdSequence* compareValueAccessorAndStaticValue(
      ValueAccessor *left_accessor,
      const attribute_id left_id,
      const TypedValue &right,
      const TupleIdSequence *filter) const;

  /**
   * @brief Apply to a fixed static value on the left and an attribute of all
   *        tuples in a ValueAccessor on the right.
   *
   * @param left The left static value to apply to.
   * @param right_accessor The ValueAccessor to apply to.
   * @param right_id The attribute ID of the right argument in accessor.
   * @param filter An optional TupleIdSequence indicating which tuples the
   *        comparison should actually be checked for. Tuples not in filter
   *        will not be checked and will not appear in the result.
   * @return A TupleIdSequence indicating matches for the comparison.
   **/
  virtual TupleIdSequence* compareStaticValueAndValueAccessor(
      const TypedValue &left,
      ValueAccessor *right_accessor,
      const attribute_id right_id,
      const TupleIdSequence *filter) const;

  /**
   * @brief Apply to a vector of values on the left and an attribute of all
   *        tuples in a ValueAccessor on the right.
   *
   * @param left The left argument ColumnVector to apply to.
   * @param right_accessor The ValueAccessor to apply to.
   * @param right_id The attribute ID of the right argument in accessor.
   * @param filter An optional TupleIdSequence indicating which tuples the
   *        comparison should actually be checked for. Tuples not in filter
   *        will not be checked and will not appear in the result.
   * @param existence_bitmap An optional TupleIdSequence indicating which
   *        tuple IDs actually exist in a TupleStorageSubBlock that the left
   *        ColumnVector was generated from (may be needed to correctly set
   *        tuple IDs in the result, as ColumnVectors do not have any "holes"
   *        in their sequence of values, but some TupleStorageSubBlock
   *        implementations may have holes in their sequence of tuple IDs).
   * @return A TupleIdSequence indicating matches for the comparison.
   **/
  virtual TupleIdSequence* compareColumnVectorAndValueAccessor(
      const ColumnVector &left,
      ValueAccessor *right_accessor,
      const attribute_id right_id,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const;

  /**
   * @brief Apply to an attribute of all tuples in a ValueAccessor on the keft
   *        and a vector of values on the right.
   *
   * @param left_accessor The ValueAccessor to apply to.
   * @param left_id The attribute ID of the left argument in accessor.
   * @param right The right argument ColumnVector to apply to.
   * @param filter An optional TupleIdSequence indicating which tuples the
   *        comparison should actually be checked for. Tuples not in filter
   *        will not be checked and will not appear in the result.
   * @param existence_bitmap An optional TupleIdSequence indicating which
   *        tuple IDs actually exist in a TupleStorageSubBlock that the right
   *        ColumnVector was generated from (may be needed to correctly set
   *        tuple IDs in the result, as ColumnVectors do not have any "holes"
   *        in their sequence of values, but some TupleStorageSubBlock
   *        implementations may have holes in their sequence of tuple IDs).
   * @return A TupleIdSequence indicating matches for the comparison.
   **/
  virtual TupleIdSequence* compareValueAccessorAndColumnVector(
      ValueAccessor *left_accessor,
      const attribute_id left_id,
      const ColumnVector &right,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const;

  /**
   * @brief Apply the comparator to column of value accessor and current value
   * overwriting the current value with column value when the comparison
   * succeeds.  This is useful for computing the minimum/maximum of a column
   * vector and current minimum/maximum value. Essentially, it does the
   * following:
   *    for (value in column_values) {
   *      if (value COMP current) {
   *        current = value;
   *      }
   *    }
   * Note that COMP is not a typical comparison with respect to NULL. If current
   * is NULL, then value COMP current is true in the above psuedocode. See
   * implementation for details.
   *
   * @param current Current value (or initial value). The running current value
   *        is always the right operand. Note that this can be a NULL value,
   *        even if the comparison is instantiated for non-nullable attributes.
   * @param accessor Value accessor to access column to accumulate/apply
   *        comparator on. The column value is always the left operand. NULL
   *        values in column are skipped.
   * @param value_accessor_id Attribute ID in value accessor of the column to
   *        accumulate on.
   **/
  virtual TypedValue accumulateValueAccessor(
      const TypedValue &current,
      ValueAccessor *accessor,
      const attribute_id value_accessor_id) const;
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

  /**
   * @brief Apply the comparator to column vector and current value overwriting
   * the current value with column vector value when the comparison succeeds.
   * This is useful for computing the minimum/maximum of a column vector and
   * current minimum/maximum value. Essentially, it does the following:
   *    for (value in column_values) {
   *      if (value COMP current) {
   *        current = value;
   *      }
   *    }
   * Note that COMP is not a typical comparison with respect to NULL. If current
   * is NULL, then value COMP current is true in the above psuedocode. See
   * implementation for details.
   *
   * @param current Current value (or initial value). The running current value
   *        is always the right operand. Note that this can be a NULL value,
   *        even if the comparison is instantiated for non-nullable attributes.
   * @param column_vector Column vector to accumulate/apply comparator on. The
   *        column value is always the left operand. NULL values in column are
   *        skipped.
   **/
  virtual TypedValue accumulateColumnVector(
      const TypedValue &current,
      const ColumnVector &column_vector) const;

 protected:
  UncheckedComparator() {
  }

  /**
   * @brief Default implementation of the compareColumnVectors method.
   **/
  template <bool left_nullable, bool right_nullable>
  inline TupleIdSequence* compareColumnVectorsDefaultImpl(
      const ColumnVector &left,
      const ColumnVector &right,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const;

  /**
   * @brief Default implementation of the compareColumnVectorAndStaticValue method.
   **/
  template <bool left_nullable, bool right_nullable>
  inline TupleIdSequence* compareColumnVectorAndStaticValueDefaultImpl(
      const ColumnVector &left,
      const TypedValue &right,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const;

  /**
   * @brief Default implementation of the compareStaticValueAndColumnVector method.
   **/
  template <bool left_nullable, bool right_nullable>
  inline TupleIdSequence* compareStaticValueAndColumnVectorDefaultImpl(
      const TypedValue &left,
      const ColumnVector &right,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const;

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  /**
   * @brief Default implementation of the compareSingleValueAccessor method.
   **/
  template <bool left_nullable, bool right_nullable>
  inline TupleIdSequence* compareSingleValueAccessorDefaultImpl(
      ValueAccessor *accessor,
      const attribute_id left_id,
      const attribute_id right_id,
      const TupleIdSequence *filter) const;

  /**
   * @brief Default implementation of the compareValueAccessorAndStaticValue method.
   **/
  template <bool left_nullable, bool right_nullable>
  inline TupleIdSequence* compareValueAccessorAndStaticValueDefaultImpl(
      ValueAccessor *left_accessor,
      const attribute_id left_id,
      const TypedValue &right,
      const TupleIdSequence *filter) const;

  /**
   * @brief Default implementation of the compareStaticValueAndValueAccessor method.
   **/
  template <bool left_nullable, bool right_nullable>
  inline TupleIdSequence* compareStaticValueAndValueAccessorDefaultImpl(
      const TypedValue &left,
      ValueAccessor *right_accessor,
      const attribute_id right_id,
      const TupleIdSequence *filter) const;

  /**
   * @brief Default implementation of the compareColumnVectorAndValueAccessor method.
   **/
  template <bool left_nullable, bool right_nullable>
  inline TupleIdSequence* compareColumnVectorAndValueAccessorDefaultImpl(
      const ColumnVector &left,
      ValueAccessor *right_accessor,
      const attribute_id right_id,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const;

  /**
   * @brief Default implementation of the compareValueAccessorAndColumnVector method.
   **/
  template <bool left_nullable, bool right_nullable>
  inline TupleIdSequence* compareValueAccessorAndColumnVectorDefaultImpl(
      ValueAccessor *left_accessor,
      const attribute_id left_id,
      const ColumnVector &right,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const;

  /**
   * @brief Default implementation of the accumulateValueAccessor method.
   **/
  template <bool value_nullable>
  inline TypedValue accumulateValueAccessorDefaultImpl(
      const TypedValue &current,
      ValueAccessor *accessor,
      const attribute_id value_accessor_id) const;
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

  /**
   * @brief Default implementation of the accumulateColumnVector method.
   **/
  template <bool value_nullable>
  inline TypedValue accumulateColumnVectorDefaultImpl(
      const TypedValue &current,
      const ColumnVector &column_vector) const;
};

/**
 * @brief A lightweight wrapper for an UncheckedComparator which implements
 *        a functor interface that is compatible with STL algorithms.
 **/
template <typename UncheckedComparatorT>
class STLUncheckedComparatorWrapper {
 public:
  typedef UncheckedComparatorT comparator_type;

  /**
   * @brief Constructor.
   *
   * @param comparator An UncheckedComparator to wrap which is created, owned,
   *        and managed by the caller.
   **/
  explicit STLUncheckedComparatorWrapper(const UncheckedComparatorT &comparator)
      : internal_comparator_(comparator) {
  }

  /**
   * @brief Compare two data items without type-checking via pointers.
   *        Overloads the call operator for compatibility with STL algorithms.
   *
   * @param left The left data pointer to compare.
   * @param right The right data pointer to compare.
   * @return Whether the comparison is true for the given data items.
   **/
  inline bool operator() (const void *left, const void *right) const {
    return internal_comparator_.compareDataPtrsInl(left, right);
  }

  inline bool operator() (const TypedValue &left, const TypedValue &right) const {
    return internal_comparator_.compareTypedValuesInl(left, right);
  }

 private:
  UncheckedComparatorT internal_comparator_;
};

// Specialization for base UncheckedComparator. This version will make virtual
// method calls.
template <>
class STLUncheckedComparatorWrapper<UncheckedComparator> {
 public:
  typedef UncheckedComparator comparator_type;

  explicit STLUncheckedComparatorWrapper(const UncheckedComparator &comparator)
      : internal_comparator_(comparator) {
    DEV_WARNING("Creating STLUncheckedComparatorWrapper for abstract base "
                "class UncheckedComparator. This will not be able to inline "
                "comparisons.");
  }

  inline bool operator() (const void *left, const void *right) const {
    return internal_comparator_.compareDataPtrs(left, right);
  }

  inline bool operator() (const TypedValue &left, const TypedValue &right) const {
    return internal_comparator_.compareTypedValues(left, right);
  }

 private:
  const UncheckedComparator &internal_comparator_;
};


/**
 * @brief An operation which compares two typed values and returns a bool.
 * @note Comparing NULL with any value always results in false
 *       (even NULL = NULL is false). This is not SQL-standard 3-value-logic,
 *       and it should probably be fixed at some point.
 **/
class Comparison : public Operation {
 public:
  /**
   * @brief Generate a serialized Protocol Buffer representation of
   *        this Comparison.
   *
   * @return The serialized Protocol Buffer representation of this Comparison.
   **/
  serialization::Comparison getProto() const;

  /**
   * @brief Determine the ID of this Comparison.
   *
   * @return The ID of this Comparison.
   **/
  inline ComparisonID getComparisonID() const {
    return comparison_id_;
  }


  /**
   * @brief Whether this comparison is one of the 6 basic comparisons
   *        (e.g. =, <>, <, <=, >, >=).
   *
   * @note There is a tight coupling between basic comparisons and subblock
   *       indices. This method can be used to prevent non-basic comparisons
   *       from entering subblock indexing related logic.
   *
   * @return Whether this is a basic comparison.
   */
  bool isBasicComparison() const {
    return (comparison_id_ == ComparisonID::kEqual
            || comparison_id_ == ComparisonID::kNotEqual
            || comparison_id_ == ComparisonID::kLess
            || comparison_id_ == ComparisonID::kLessOrEqual
            || comparison_id_ == ComparisonID::kGreater
            || comparison_id_ == ComparisonID::kGreaterOrEqual);
  }

  /**
   * @brief Determine whether two Types can be compared by this Comparison.
   *
   * @param left The first Type to check.
   * @param right The second Type to check.
   * @return Whether the specified types can be compared by this Comparison.
   **/
  virtual bool canCompareTypes(const Type &left, const Type &right) const = 0;

  /**
   * @brief Similar to canCompareTypes(), but either Type may be NULL to
   *        indicate an unknown (but presumed nullable) Type.
   * @note Unknown Types are presumed to be nullable, since an unknown Type
   *       only arises from a NULL literal appearing in a query without
   *       sufficient context to resolve it to a particular Type.
   * @note If either argument is NULL (i.e. signifying a NULL of unknown type),
   *       the comparison is assumed to always be false. When proper 3-valued
   *       logic is implemented, the assumed result should be unknown.
   *
   * @param left_type The left argument Type for this Comparison, or NULL if
   *        the Type is not known (i.e. there is a NULL value with an
   *        unresolved Type in the query).
   * @param right_type The right argument Type for this Comparison, or NULL if
   *        the Type is not known (i.e. there is a NULL value with an
   *        unresolved Type in the query).
   * @return true if this comparison can be applied based on the known argument
   *         types.
   **/
  virtual bool canComparePartialTypes(const Type *left_type,
                                      const Type *right_type) const = 0;

  /**
   * @brief Compare two TypedValues.
   * @warning It is an error to call this method if the types of the values are
   *          not comparable. If in doubt, check canCompareTypes() first.
   *
   * @param left The left TypedValue to compare.
   * @param left_type The Type that left belongs to.
   * @param right The right TypedValue to compare.
   * @param right_type The Type that right belongs to.
   * @return Whether this comparison is true for the given TypedValues.
   **/
  virtual bool compareTypedValuesChecked(const TypedValue &left,
                                         const Type &left_type,
                                         const TypedValue &right,
                                         const Type &right_type) const = 0;

  /**
   * @brief Create an UncheckedComparator which can compare items of the
   *        specified types.
   * @warning The resulting UncheckedComparator performs no type-checking
   *          whatsoever. Nonetheless, it is useful in situations where many
   *          data items of the same, known type are to be compared (for
   *          example, over many tuples in the same table).
   *
   * @param left The left Type to compare.
   * @param right The right Type to compare.
   * @exception OperationInapplicableToType This Comparison is not applicable
   *            to either left or right.
   * @return An UncheckedComparator which applies this Comparison to the
   *         specified Types.
   **/
  virtual UncheckedComparator* makeUncheckedComparatorForTypes(const Type &left,
                                                               const Type &right) const = 0;

 protected:
  explicit Comparison(const ComparisonID comparison_id)
      : Operation(Operation::kComparison,
                  kComparisonNames[
                      static_cast<typename std::underlying_type<ComparisonID>::type>(comparison_id)],
                  kComparisonShortNames[
                      static_cast<typename std::underlying_type<ComparisonID>::type>(comparison_id)]),
        comparison_id_(comparison_id) {
  }

 private:
  const ComparisonID comparison_id_;

  DISALLOW_COPY_AND_ASSIGN(Comparison);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_COMPARISON_HPP_
