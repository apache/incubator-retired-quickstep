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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_CROSS_REFERENCE_COALESCE_AGGREGATE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_CROSS_REFERENCE_COALESCE_AGGREGATE_HPP_

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace physical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class CrossReferenceCoalesceAggregate;
typedef std::shared_ptr<const CrossReferenceCoalesceAggregate> CrossReferenceCoalesceAggregatePtr;

/**
 * @brief A physical node that fuses a HashJoin with an Aggregate to enable
 *        fast-path execution.
 *
 * Below we briefly describe the semantics of this physical node.
 *
 * Let L be a table with PRIMARY KEY u. Let R be a table with FOREIGN KEY x
 * referring to L(u). Then CrossReferenceCoalesceAggregate represents a common
 * class of analytical queries that
 * - For each u in L, COUNT/SUM the records in R that correspond to u (i.e.
 *   those records satisfying R.x = L.u).
 *   In the case that there is no record for u in R, use 0 as the result value.
 *
 * And we have the mapping:
 *   L -> left_child_
 *   R -> right_child_
 *   u -> left_join_attributes_
 *   x -> right_join_attributes_
 *   COUNT/SUM -> aggregate_expressions_
 */
class CrossReferenceCoalesceAggregate : public Physical {
 public:
  PhysicalType getPhysicalType() const override {
    return PhysicalType::kCrossReferenceCoalesceAggregate;
  }

  std::string getName() const override {
    return "CrossReferenceCoalesceAggregate";
  }

  /**
   * @return The left physical child.
   */
  const PhysicalPtr& left_child() const {
    return left_child_;
  }

  /**
   * @return The right physical child.
   */
  const PhysicalPtr& right_child() const {
    return right_child_;
  }

  /**
   * @return The left join attributes.
   */
  const std::vector<expressions::AttributeReferencePtr>& left_join_attributes() const {
    return left_join_attributes_;
  }

  /**
   * @return The right join attributes.
   */
  const std::vector<expressions::AttributeReferencePtr>& right_join_attributes() const {
    return right_join_attributes_;
  }

  /**
   * @return The predicate to be applied to the right child before aggregation.
   */
  const expressions::PredicatePtr& right_filter_predicate() const {
    return right_filter_predicate_;
  }

  /**
   * @return Aggregate expressions.
   */
  const std::vector<expressions::AliasPtr>& aggregate_expressions() const {
    return aggregate_expressions_;
  }

  /**
   * @return The maximum possible value of the group-by keys when mapped to
   *         integer.
   */
  std::size_t group_by_key_value_range() const {
    return group_by_key_value_range_;
  }

  PhysicalPtr copyWithNewChildren(
      const std::vector<PhysicalPtr> &new_children) const override {
    DCHECK_EQ(getNumChildren(), new_children.size());
    return Create(new_children[0],
                  new_children[1],
                  left_join_attributes_,
                  right_join_attributes_,
                  right_filter_predicate_,
                  aggregate_expressions_,
                  group_by_key_value_range_);
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override;

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override;

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const override {
    return false;
  }

  /**
   * @brief Creates a physical CrossReferenceCoalesceAggregate.
   *
   * @param left_child The left child.
   * @param right_child The right child.
   * @param left_join_attributes The join attributes of the left child.
   * @param right_join_attributes The join attributes of the right child.
   * @param right_filter_predicate Optional filtering predicate evaluated on
   *        the left child before aggregation.
   * @param aggregate_expressions The aggregate expressions.
   * @param group_by_key_value_range The maximum possible value of the group-by
   *        keys when mapped to integer.
   * @return An immutable physical CrossReferenceCoalesceAggregate.
   */
  static CrossReferenceCoalesceAggregatePtr Create(
      const PhysicalPtr &left_child,
      const PhysicalPtr &right_child,
      const std::vector<expressions::AttributeReferencePtr> &left_join_attributes,
      const std::vector<expressions::AttributeReferencePtr> &right_join_attributes,
      const expressions::PredicatePtr &right_filter_predicate,
      const std::vector<expressions::AliasPtr> &aggregate_expressions,
      const std::size_t group_by_key_value_range) {
    return CrossReferenceCoalesceAggregatePtr(
        new CrossReferenceCoalesceAggregate(left_child,
                                            right_child,
                                            left_join_attributes,
                                            right_join_attributes,
                                            right_filter_predicate,
                                            aggregate_expressions,
                                            group_by_key_value_range));
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

 private:
  CrossReferenceCoalesceAggregate(
      const PhysicalPtr &left_child,
      const PhysicalPtr &right_child,
      const std::vector<expressions::AttributeReferencePtr> &left_join_attributes,
      const std::vector<expressions::AttributeReferencePtr> &right_join_attributes,
      const expressions::PredicatePtr &right_filter_predicate,
      const std::vector<expressions::AliasPtr> &aggregate_expressions,
      const std::size_t group_by_key_value_range)
      : left_child_(left_child),
        right_child_(right_child),
        left_join_attributes_(left_join_attributes),
        right_join_attributes_(right_join_attributes),
        right_filter_predicate_(right_filter_predicate),
        aggregate_expressions_(aggregate_expressions),
        group_by_key_value_range_(group_by_key_value_range) {
    addChild(left_child_);
    addChild(right_child_);
  }

  // TODO(jianqiao): For the left child, support filter predicate fusing and
  // attachment of LIPFilters.
  const PhysicalPtr left_child_;
  const PhysicalPtr right_child_;
  const std::vector<expressions::AttributeReferencePtr> left_join_attributes_;
  const std::vector<expressions::AttributeReferencePtr> right_join_attributes_;
  const expressions::PredicatePtr right_filter_predicate_;
  const std::vector<expressions::AliasPtr> aggregate_expressions_;
  const std::size_t group_by_key_value_range_;

  DISALLOW_COPY_AND_ASSIGN(CrossReferenceCoalesceAggregate);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_CROSS_REFERENCE_COALESCE_AGGREGATE_HPP_
