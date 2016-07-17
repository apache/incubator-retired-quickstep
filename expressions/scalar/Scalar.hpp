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

#ifndef QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_HPP_
#define QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_HPP_

#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/Expressions.pb.h"
#include "storage/StorageBlockInfo.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogAttribute;
class ColumnVector;
class Type;
class ValueAccessor;

struct SubBlocksReference;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief Base class for anything which evaluates to a Scalar value.
 **/
class Scalar {
 public:
  /**
   * @brief The possible provenance of Scalar values.
   **/
  enum ScalarDataSource {
    kLiteral = 0,
    kAttribute,
    kUnaryExpression,
    kBinaryExpression,
    kCaseExpression,
    kNumScalarDataSources  // Not a real ScalarDataSource, exists for counting purposes.
  };

  /**
   * @brief Strings naming the elements in ScalarDataSource.
   * @note String literals are defined out-of-line in Scalar.cpp
   **/
  static const char *kScalarDataSourceNames[kNumScalarDataSources];

  /**
   * @brief Virtual destructor.
   **/
  virtual ~Scalar() {
  }

  /**
   * @brief Serialize this scalar in Protocol Buffer form.
   *
   * @return The Protocol Buffer representation of this scalar object.
   **/
  virtual serialization::Scalar getProto() const = 0;

  /**
   * @brief Make a deep copy of this Scalar.
   *
   * @return A cloned copy of this Scalar.
   **/
  virtual Scalar* clone() const = 0;

  /**
   * @brief Get the type of scalar value represented.
   *
   * @return The Type of the Scalar.
   **/
  inline const Type& getType() const {
    return type_;
  }

  /**
   * @brief Get the provenance of this scalar value.
   *
   * @return The source of this Scalar's data.
   **/
  virtual ScalarDataSource getDataSource() const = 0;

  /**
   * @brief Get this Scalar's value for the given tuple from a ValueAccessor.
   * @note This only works for Scalars which can be evaluated for a single
   *       table. Use getValueForJoinedTuples() where necessary.
   *
   * @param accessor The ValueAccessor that will be used to access the tuple to
   *        evaluate this Scalar for.
   * @param tuple The ID of the tuple in accessor (the absolute position) to
   *        evaluate this Scalar for.
   * @return The value of this scalar for the given tuple in the given
   *         TupleStorageSubBlock.
   **/
  virtual TypedValue getValueForSingleTuple(const ValueAccessor &accessor,
                                            const tuple_id tuple) const = 0;

  /**
   * @brief Get this Scalar's value for the given pair of joined tuples.
   *
   * @param left_accessor The ValueAccessor that the first of the joined
   *        tuples can be read from (this does NOT necessarily correspond to
   *        the left operand of a binary operation).
   * @param left_relation_id The ID of the relation that left_tuple_store
   *        belongs to.
   * @param left_tuple_id The ID of the tuple in left_tuple_store to evaluate
   *        this Scalar for.
   * @param right_accessor The ValueAccessor that the second of the joined
   *        tuples can be read from (this does NOT necessarily correspond to
   *        the right operand of a binary operation).
   * @param right_relation_id The ID of the relation that right_tuple_store
   *        belongs to.
   * @param right_tuple_id The ID of the tuple in right_tuple_store to evaluate
   *        this Scalar for.
   * @return The value of this scalar for the given tuples.
   **/
  virtual TypedValue getValueForJoinedTuples(
      const ValueAccessor &left_accessor,
      const relation_id left_relation_id,
      const tuple_id left_tuple_id,
      const ValueAccessor &right_accessor,
      const relation_id right_relation_id,
      const tuple_id right_tuple_id) const = 0;

  /**
   * @brief Determine whether this Scalar's value is static (i.e. whether it is
   *        the same regardless of tuple).
   *
   * @return Whether this Scalar's value is static.
   **/
  virtual bool hasStaticValue() const {
    return false;
  }

  /**
   * @brief Get this Scalar's static value.
   * @warning hasStaticValue() should be called first to check whether a static
   *          value actually exists.
   *
   * @return This Scalar's static value.
   **/
  virtual const TypedValue& getStaticValue() const;

  /**
   * @brief If it is possible to get this Scalar's values directly from a
   *        ValueAccessor, return the attribute_id that should be used to get
   *        values.
   *
   * @return The attribute_id of this Scalar's values using a ValueAccessor,
   *         or -1 if values can not be obtained directly from a ValueAccessor.
   **/
  virtual attribute_id getAttributeIdForValueAccessor() const {
    return -1;
  }

  /**
   * @brief If it is possible to get this Scalar's values directly from a
   *        ValueAccessor, return the ID of the relation that such a
   *        ValueAccessor should belong to.
   *
   * @return The relation_id for ValueAccessors that can directly produce this
   *         Scalar's values, or -1 if values can not be obtained directly from
   *         a ValueAccessor.
   **/
  virtual relation_id getRelationIdForValueAccessor() const {
    return -1;
  }

  /**
   * @brief Get this Scalar's values for all tuples accesible via a
   *        ValueAccessor.
   *
   * @param accessor A ValueAccessor which this Scalar will be evaluated over.
   * @param sub_blocks_ref If non-NULL, contains references to the
   *        TupleStorageSubBlock that produced accessor and any IndexSubBlocks
   *        in the same StorageBlock. These sub-blocks may be used for
   *        fast-path (non-scan) evaluation of Predicates embedded in this
   *        scalar (e.g. WHEN predicates in ScalarCaseExpression). May be NULL,
   *        in which case scan-based evaluation is always used.
   * @return A ColumnVector of this Scalar's values for each tuple accesible
   *         via accessor.
   **/
  virtual ColumnVector* getAllValues(ValueAccessor *accessor,
                                     const SubBlocksReference *sub_blocks_ref) const = 0;

  /**
   * @brief Get this Scalar's value for all specified joined tuples from two
   *        ValueAccessors.
   *
   * @param left_relation_id The ID of the left relation in the join.
   * @param left_accessor A ValueAccessor which will be used to access tuples
   *        from the left relation.
   * @param right_relation_id The ID of the right relation in the join.
   * @param right_accessor A ValueAccessor which will be used to access tuples
   *        from the right relation.
   * @param joined_tuple_ids A series of pairs of tuple ids from the left and
   *        right relations that will be joined.
   * @return A ColumnVector of this Scalar's values for all the joined tuples
   *         specified by joined_tuple_ids.
   **/
  virtual ColumnVector* getAllValuesForJoin(
      const relation_id left_relation_id,
      ValueAccessor *left_accessor,
      const relation_id right_relation_id,
      ValueAccessor *right_accessor,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const = 0;

  /**
   * @brief Get a vector of all attributes that this Scalar depends on.
   *
   * @param Output parameter: vector to push the dependency attributes into.
   */
  virtual void getDependencyAttributes(
      std::vector<const CatalogAttribute*> attrs) const = 0;

 protected:
  explicit Scalar(const Type &type) : type_(type) {
  }

  const Type &type_;

 private:
  DISALLOW_COPY_AND_ASSIGN(Scalar);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_HPP_
