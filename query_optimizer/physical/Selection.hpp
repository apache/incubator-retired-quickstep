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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_SELECTION_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_SELECTION_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/LogicalAnd.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

class Selection;
typedef std::shared_ptr<const Selection> SelectionPtr;

/**
 * @brief Physical node that evaluates a predicate for each input tuple,
 *        computes and outputs the values of project expressions if the
 *        predicate evaluates to true.
 */
class Selection : public Physical {
 public:
  PhysicalType getPhysicalType() const override { return PhysicalType::kSelection; }

  std::string getName() const override { return "Selection"; }

  /**
   * @return Filter predicate to evaluate during scanning the input.
   */
  inline const expressions::PredicatePtr& filter_predicate() const {
    return filter_predicate_;
  }

  /**
   * @return Project expressions to compute.
   */
  inline const std::vector<expressions::NamedExpressionPtr>& project_expressions() const {
    return project_expressions_;
  }

  /**
   * @return Input physical node.
   */
  inline const PhysicalPtr& input() const { return children()[0]; }

  bool isSimpleSelection() const;

  PhysicalPtr copyWithNewChildren(
      const std::vector<PhysicalPtr> &new_children) const override;

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override;

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override;

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_attributes,
      PhysicalPtr *output) const override;

  bool impliesUniqueAttributes(
      const std::vector<expressions::AttributeReferencePtr> &attributes) const override;

  const BloomFilterConfig &bloom_filter_config() const {
    return bloom_filter_config_;
  }

  /**
   * @brief Creates a Selection.
   *
   * @param input The input node.
   * @param project_expressions The project expressions.
   * @param filter_predicate The filter predicate. Can be NULL.
   * @return An immutable Selection.
   */
  static SelectionPtr Create(
      const PhysicalPtr &input,
      const std::vector<expressions::NamedExpressionPtr> &project_expressions,
      const expressions::PredicatePtr &filter_predicate,
      const BloomFilterConfig bloom_filter_config = BloomFilterConfig()) {
    return SelectionPtr(
        new Selection(input, project_expressions, filter_predicate, bloom_filter_config));
  }

  /**
   * @brief Creates a conjunctive predicate with \p filter_predicates
   *        as operands, and creates a Selection with the predicate.
   *
   * @param input The input node.
   * @param project_expressions The project expressions.
   * @param filter_predicates A list of filter predicates.
   *                          The predicate in the created Selection is
   *                          a conjunction of them.
   * @return An immutable Selection.
   */
  static SelectionPtr CreateWithConjuction(
      const PhysicalPtr &input,
      const std::vector<expressions::NamedExpressionPtr> &project_expressions,
      const std::vector<expressions::PredicatePtr> &filter_predicates) {
    if (filter_predicates.size() > 1u) {
      return Create(input, project_expressions,
                    expressions::LogicalAnd::Create(filter_predicates));
    } else if (filter_predicates.size() == 1u) {
      return Create(input, project_expressions, filter_predicates[0]);
    } else {
      return Create(input, project_expressions, expressions::PredicatePtr());
    }
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
  Selection(
      const PhysicalPtr &input,
      const std::vector<expressions::NamedExpressionPtr> &project_expressions,
      const expressions::PredicatePtr &filter_predicate,
      const BloomFilterConfig &bloom_filter_config)
      : project_expressions_(project_expressions),
        filter_predicate_(filter_predicate),
        bloom_filter_config_(bloom_filter_config) {
    addChild(input);
  }

  std::vector<expressions::NamedExpressionPtr> project_expressions_;
  // Can be NULL. If NULL, the filter predicate is treated as the literal true.
  expressions::PredicatePtr filter_predicate_;
  BloomFilterConfig bloom_filter_config_;

  DISALLOW_COPY_AND_ASSIGN(Selection);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_SELECTION_HPP_ */
