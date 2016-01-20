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

#ifndef QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_CASE_EXPRESSION_HPP_
#define QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_CASE_EXPRESSION_HPP_

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class ColumnVector;
class IndirectColumnVector;
class NativeColumnVector;
class TupleIdSequence;
class Type;
class ValueAccessor;

struct SubBlocksReference;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief A SQL CASE expression, which consists of one or more Predicates from
 *        WHEN clauses that map to Scalars which are evaluated conditionally
 *        for the first matching WHEN predicate. Also contains a Scalar for the
 *        ELSE case where no WHEN predicate matches.
 * @note This class implements the most general form of a SQL case expression,
 *       a "searched CASE" expression. For other forms of the CASE expression
 *       (e.g. "simple CASE") the optimizer is responsible for resolving into
 *       this general form.
 **/
class ScalarCaseExpression : public Scalar {
 public:
  /**
   * @brief Constructor.
   *
   * @param result_type The Type for values produced by this expression. Must
   *        subsume the types of all result_expressions and
   *        else_result_expression.
   * @param when_predicates A vector of Predicates, one for each WHEN clause,
   *        in descending order of priority. Will be moved-from.
   * @param result_expressions A vector of Scalars, each of which is a THEN
   *        expression corresponding to the WHEN clause in the same position in
   *        when_predicates. Will be moved-from.
   * @param else_result_expression A Scalar whose value will be used when none
   *        of the when_predicates match. This is a required parameter. SQL
   *        allows the ELSE clause of a CASE expression to be ommitted, in
   *        which case it defaults to NULL. For compliance with the SQL
   *        standard, an explicit ScalarLiteral with a NULL value should be
   *        supplied here for such cases.
   **/
  ScalarCaseExpression(const Type &result_type,
                       std::vector<std::unique_ptr<Predicate>> &&when_predicates,
                       std::vector<std::unique_ptr<Scalar>> &&result_expressions,
                       Scalar *else_result_expression);

  ~ScalarCaseExpression() override {
  }

  serialization::Scalar getProto() const override;

  Scalar* clone() const override;

  ScalarDataSource getDataSource() const override {
    return kCaseExpression;
  }

  TypedValue getValueForSingleTuple(const ValueAccessor &accessor,
                                    const tuple_id tuple) const override;

  TypedValue getValueForJoinedTuples(
      const ValueAccessor &left_accessor,
      const relation_id left_relation_id,
      const tuple_id left_tuple_id,
      const ValueAccessor &right_accessor,
      const relation_id right_relation_id,
      const tuple_id right_tuple_id) const override;

  bool hasStaticValue() const override {
    return has_static_value_;
  }

  const TypedValue& getStaticValue() const override {
    DCHECK(has_static_value_);
    return static_value_;
  }

  attribute_id getAttributeIdForValueAccessor() const override {
    if (fixed_result_expression_ != nullptr) {
      return fixed_result_expression_->getAttributeIdForValueAccessor();
    } else {
      return -1;
    }
  }

  relation_id getRelationIdForValueAccessor() const override {
    if (fixed_result_expression_ != nullptr) {
      return fixed_result_expression_->getRelationIdForValueAccessor();
    } else {
      return -1;
    }
  }

  ColumnVector* getAllValues(ValueAccessor *accessor,
                             const SubBlocksReference *sub_blocks_ref) const override;

  ColumnVector* getAllValuesForJoin(
      const relation_id left_relation_id,
      ValueAccessor *left_accessor,
      const relation_id right_relation_id,
      ValueAccessor *right_accessor,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const override;

 private:
  // Merge the values in the NativeColumnVector 'case_result' into '*output' at
  // the positions specified by 'case_matches'. If '*source_sequence' is
  // non-NULL, it indicates which positions actually have tuples in the input,
  // otherwise it is assumed that there are no holes in the input.
  static void MultiplexNativeColumnVector(
      const TupleIdSequence *source_sequence,
      const TupleIdSequence &case_matches,
      const NativeColumnVector &case_result,
      NativeColumnVector *output);

  // Same as MultiplexNativeColumnVector(), but works on IndirectColumnVectors
  // instead of NativeColumnVectors.
  static void MultiplexIndirectColumnVector(
      const TupleIdSequence *source_sequence,
      const TupleIdSequence &case_matches,
      IndirectColumnVector *case_result,
      IndirectColumnVector *output);

  // Create and return a new ColumnVector by multiplexing the ColumnVectors
  // containing results for individual CASE branches at the appropriate
  // positions. 'output_size' is the total number of values in the output.
  // '*source_sequence' indicates which positions actually have tuples in the
  // input (if NULL, it is assumed there are no holes in the input).
  // 'case_matches' are the sequences of tuple_ids matching each WHEN clause in
  // order. 'else_matches' indicates the tuple_ids that did not match any of
  // the explicit WHEN clauses. Similarly, '*case_results' are the values
  // generated for the tuples matching each WHEN clause, and '*else_results'
  // are the values generated for the ELSE tuples.
  ColumnVector* multiplexColumnVectors(
      const std::size_t output_size,
      const TupleIdSequence *source_sequence,
      const std::vector<std::unique_ptr<TupleIdSequence>> &case_matches,
      const TupleIdSequence &else_matches,
      std::vector<std::unique_ptr<ColumnVector>> *case_results,
      ColumnVector *else_result) const;

  std::vector<std::unique_ptr<Predicate>> when_predicates_;
  std::vector<std::unique_ptr<Scalar>> result_expressions_;
  std::unique_ptr<Scalar> else_result_expression_;

  // If the CASE always evaluates to the same branch, this points to the result
  // expression for that branch. Note that this is different from having a
  // static value, because the result expression itself can have a
  // tuple-dependent value.
  const Scalar *fixed_result_expression_;

  bool has_static_value_;
  TypedValue static_value_;

  DISALLOW_COPY_AND_ASSIGN(ScalarCaseExpression);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_CASE_EXPRESSION_HPP_
