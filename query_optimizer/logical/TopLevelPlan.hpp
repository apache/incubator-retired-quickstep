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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_TOP_LEVEL_PLAN_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_TOP_LEVEL_PLAN_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class TopLevelPlan;
typedef std::shared_ptr<const TopLevelPlan> TopLevelPlanPtr;

/**
 * @brief The root of a logical plan tree that contains the entire logical plan
 *        and shared subplans.
 */
class TopLevelPlan : public Logical {
 public:
  /**
   * @brief Destructor.
   */
  ~TopLevelPlan() override {}

  LogicalType getLogicalType() const override { return LogicalType::kTopLevelPlan; }

  std::string getName() const override { return "TopLevelPlan"; }

  /**
   * @return The input plan.
   */
  const LogicalPtr& plan() const { return plan_; }

  /**
   * @return The vector of the shared subplans.
   */
  const std::vector<LogicalPtr>& shared_subplans() const { return shared_subplans_; }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override {
    DCHECK_EQ(getNumChildren(), new_children.size());
    // The first is the main plan, and the remaining
    // children are shared subplans referenced in the main plan.
    std::vector<LogicalPtr> new_shared_subplans;
    if (new_children.size() > 1u) {
      new_shared_subplans.insert(new_shared_subplans.end(),
                                 ++new_children.begin(),
                                 new_children.end());
    }
    return Create(new_children[0], new_shared_subplans, uncorrelated_subquery_map_);
  }

  /**
   * @return Map from the expression ID of an attribute reference to the
   *         uncorrelated subquery that produces the attribute.
   */
  const std::unordered_map<expressions::ExprId, int>& uncorrelated_subquery_map() const {
    return uncorrelated_subquery_map_;
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return plan()->getOutputAttributes();
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return {};
  }

  /**
   * @brief Creates a TopLevelPlan.
   *
   * @param plan The input plan.
   * @param shared_subplans The subplans referenced in the main input plan.
   * @param uncorrelated_subquery_map Map from expression IDs to uncorrelated scalar subqueries.
   * @return An immutable TopLevelPlan.
   */
  static TopLevelPlanPtr Create(const LogicalPtr &plan,
                                const std::vector<LogicalPtr> &shared_subplans = std::vector<LogicalPtr>(),
                                const std::unordered_map<expressions::ExprId, int> &uncorrelated_subquery_map =
                                    std::unordered_map<expressions::ExprId, int>()) {
    return TopLevelPlanPtr(new TopLevelPlan(plan, shared_subplans, uncorrelated_subquery_map));
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
  TopLevelPlan(const LogicalPtr &plan,
               const std::vector<LogicalPtr> &shared_subplans,
               const std::unordered_map<expressions::ExprId, int> &uncorrelated_subquery_map)
      : plan_(plan),
        shared_subplans_(shared_subplans),
        uncorrelated_subquery_map_(uncorrelated_subquery_map) {
    addChild(plan);
    for (const LogicalPtr &shared_subplan : shared_subplans) {
      addChild(shared_subplan);
    }
  }

  LogicalPtr plan_;
  // Stored in the topological ordering based on dependencies.
  std::vector<LogicalPtr> shared_subplans_;

  std::unordered_map<expressions::ExprId, int> uncorrelated_subquery_map_;

  DISALLOW_COPY_AND_ASSIGN(TopLevelPlan);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_TOP_LEVEL_PLAN_HPP_ */
