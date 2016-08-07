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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_NESTED_LOOPS_JOIN_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_NESTED_LOOPS_JOIN_HPP_

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/physical/BinaryJoin.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

class NestedLoopsJoin;
typedef std::shared_ptr<const NestedLoopsJoin> NestedLoopsJoinPtr;

/**
 * @brief Physical nested loops join node.
 */
class NestedLoopsJoin : public BinaryJoin {
 public:
  PhysicalType getPhysicalType() const override {
    return PhysicalType::kNestedLoopsJoin;
  }

  std::string getName() const override { return "NestedLoopsJoin"; }

  /**
   * @return The join predicate.
   */
  const expressions::PredicatePtr& join_predicate() const {
    return join_predicate_;
  }

  PhysicalPtr copyWithNewChildren(
      const std::vector<PhysicalPtr> &new_children) const override {
    DCHECK_EQ(children().size(), new_children.size());
    return Create(new_children[0],
                  new_children[1],
                  join_predicate_,
                  project_expressions());
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override;

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const override;

  /**
   * @brief Creates a NestedLoopsJoin.
   *
   * @param left The left operand.
   * @param right The right operand.
   * @param join_predicate The join predicate.
   * @param project_expressions The project expressions.
   * @return An immutable NestedLoopsJoin.
   */
  static NestedLoopsJoinPtr Create(
      const PhysicalPtr &left,
      const PhysicalPtr &right,
      const expressions::PredicatePtr &join_predicate,
      const std::vector<expressions::NamedExpressionPtr> &project_expressions) {
    return NestedLoopsJoinPtr(
        new NestedLoopsJoin(left, right, join_predicate, project_expressions));
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
  NestedLoopsJoin(
      const PhysicalPtr &left,
      const PhysicalPtr &right,
      const expressions::PredicatePtr &join_predicate,
      const std::vector<expressions::NamedExpressionPtr> &project_expressions)
      : BinaryJoin(left, right, project_expressions),
        join_predicate_(join_predicate) {
    DCHECK(join_predicate_ != nullptr);
  }

  expressions::PredicatePtr join_predicate_;

  DISALLOW_COPY_AND_ASSIGN(NestedLoopsJoin);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_NESTED_LOOPS_JOIN_HPP_ */
