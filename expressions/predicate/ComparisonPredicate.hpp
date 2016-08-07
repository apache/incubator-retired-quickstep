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

#ifndef QUICKSTEP_EXPRESSIONS_PREDICATE_COMPARISON_PREDICATE_HPP_
#define QUICKSTEP_EXPRESSIONS_PREDICATE_COMPARISON_PREDICATE_HPP_

#include <memory>
#include <utility>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class TupleIdSequence;
class ValueAccessor;

struct SubBlocksReference;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief A Predicate which is a comparison of two scalar values.
 **/
class ComparisonPredicate : public Predicate {
 public:
  /**
   * @brief Constructor
   *
   * @param Comparison The comparison operation to be performed.
   * @param left_operand The left argument of the comparison, becomes owned by
   *        this ComparisonPredicate.
   * @param right_operand The right argument of the comparison, becomes owned
   *        by this ComparisonPredicate.
   **/
  ComparisonPredicate(const Comparison &comparison,
                      Scalar *left_operand,
                      Scalar *right_operand);

  ~ComparisonPredicate() override {
  }

  serialization::Predicate getProto() const override;

  Predicate* clone() const override;

  PredicateType getPredicateType() const override {
    return kComparison;
  }

  bool isAttributeLiteralComparisonPredicate() const override;

  bool matchesForSingleTuple(const ValueAccessor &accessor,
                             const tuple_id tuple) const override;

  bool matchesForJoinedTuples(
      const ValueAccessor &left_accessor,
      const relation_id left_relation_id,
      const tuple_id left_tuple_id,
      const ValueAccessor &right_accessor,
      const relation_id right_relation_id,
      const tuple_id right_tuple_id) const override;

  TupleIdSequence* getAllMatches(ValueAccessor *accessor,
                                 const SubBlocksReference *sub_blocks_ref,
                                 const TupleIdSequence *filter,
                                 const TupleIdSequence *existence_map) const override;

  bool hasStaticResult() const override {
    return (fast_comparator_.get() == nullptr);
  }

  bool getStaticResult() const override;

  /**
   * @brief Get the comparison operation for this predicate.
   *
   * @return This predicate's comparison.
   **/
  const Comparison& getComparison() const {
    return comparison_;
  }

  /**
   * @brief Get the left operand of this comparison.
   *
   * @return This comparison's left operand.
   **/
  const Scalar& getLeftOperand() const {
    DCHECK(left_operand_.get() != nullptr);
    return *left_operand_;
  }

  /**
   * @brief Get the right operand of this comparison.
   *
   * @return This comparison's right operand.
   **/
  const Scalar& getRightOperand() const {
    DCHECK(right_operand_.get() != nullptr);
    return *right_operand_;
  }

  /**
   * @brief For an attribute-literal comparison, determine whether the
   *        attribute is on the left and what its attribute_id is.
   * @warning This is only usable if isAttributeLiteralComparisonPredicate() is
   *          true.
   *
   * @return A pair: first is true if attribute is on the left, false if on the
   *         right. second is the ID of the attribute.
   **/
  std::pair<bool, attribute_id> getAttributeFromAttributeLiteralComparison() const;

 private:
  const Comparison &comparison_;
  std::unique_ptr<Scalar> left_operand_;
  std::unique_ptr<Scalar> right_operand_;
  bool static_result_;
  std::unique_ptr<UncheckedComparator> fast_comparator_;

  void initHelper(bool own_children);

  DISALLOW_COPY_AND_ASSIGN(ComparisonPredicate);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_PREDICATE_COMPARISON_PREDICATE_HPP_
