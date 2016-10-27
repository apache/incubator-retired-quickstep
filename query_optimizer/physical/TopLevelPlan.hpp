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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_TOP_LEVEL_PLAN_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_TOP_LEVEL_PLAN_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/physical/LIPFilterConfiguration.hpp"
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

class TopLevelPlan;
typedef std::shared_ptr<const TopLevelPlan> TopLevelPlanPtr;

/**
 * @brief Root node of a physical plan tree. It contains the complete plan for
 *        a query and shared subplans referenced used in multiple places in
 *        the query plan.
 */
class TopLevelPlan : public Physical {
 public:
  PhysicalType getPhysicalType() const override {
    return PhysicalType::kTopLevelPlan;
  }

  std::string getName() const override { return "TopLevelPlan"; }

  /**
   * @return The complete query plan.
   */
  const PhysicalPtr& plan() const { return plan_; }

  /**
   * @return The vector of the shared subplans.
   */
  const std::vector<PhysicalPtr>& shared_subplans() const {
    return shared_subplans_;
  }

  /**
   * @return Map from the expression ID of an attribute reference to the
   *         uncorrelated subquery that produces the attribute.
   */
  const std::unordered_map<expressions::ExprId, int>& uncorrelated_subquery_map() const {
    return uncorrelated_subquery_map_;
  }

  /**
   * @brief Gets a shared subquery given by the position.
   *
   * @param index The position of the shared subplan to be returned.
   * @return The shared subplan at the index \p index in <shared_subplans_>.
   */
  const PhysicalPtr& shared_subplan_at(int index) const {
    return shared_subplans_[index];
  }

  /**
   * @brief Creates a copy of the TopLevelPlan with lip_filter_configuration_
   *        replaced by \p new_lip_filter_configuration.
   *
   * @param new_lip_filter_configuration The new lip_filter_configuration to be
   *        substituted for the existing one.
   * @return A copy of this TopLevelPlan with the new lip_filter_configuration.
   */
  TopLevelPlanPtr copyWithLIPFilterConfiguration(
      const LIPFilterConfigurationPtr &new_lip_filter_configuration) const {
    return TopLevelPlan::Create(plan_,
                                shared_subplans_,
                                uncorrelated_subquery_map_,
                                new_lip_filter_configuration);
  }

  /**
   * @return The LIPFilter configuration information for the overall query plan.
   */
  const LIPFilterConfigurationPtr& lip_filter_configuration() const {
    return lip_filter_configuration_;
  }

  PhysicalPtr copyWithNewChildren(
      const std::vector<PhysicalPtr> &new_children) const override {
    DCHECK_EQ(getNumChildren(), new_children.size());
    // The first child in the new_children is the main plan, and the remaining
    // children are shared subplans referenced in the main plan.
    std::vector<PhysicalPtr> new_shared_subplans;
    if (new_children.size() > 1u) {
      new_shared_subplans.insert(new_shared_subplans.end(),
                                 ++new_children.begin(),
                                 new_children.end());
    }
    return TopLevelPlan::Create(new_children[0],
                                new_shared_subplans,
                                uncorrelated_subquery_map_,
                                lip_filter_configuration_);
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return plan()->getOutputAttributes();
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return {};
  }

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const override {
    // TopLevelPlan does not have any project expressions.
    return false;
  }

  /**
   * @brief Creates a TopLevelPlan.
   *
   * @param plan The query plan.
   * @param shared_subplans The subplans referenced in the main input plan.
   * @param uncorrelated_subquery_map Map from the expression ID of an attribute
   *        reference to the uncorrelated subquery that produces the attribute.
   * @param lip_filter_configuration The LIPFilter configuration information
   *        for the overall query plan.
   * @return An immutable TopLevelPlan.
   */
  static TopLevelPlanPtr Create(
      const PhysicalPtr &plan,
      const std::vector<PhysicalPtr> &shared_subplans = {},
      const std::unordered_map<expressions::ExprId, int> &uncorrelated_subquery_map
          = std::unordered_map<expressions::ExprId, int>(),
      const LIPFilterConfigurationPtr &lip_filter_configuration = nullptr) {
    return TopLevelPlanPtr(new TopLevelPlan(plan,
                                            shared_subplans,
                                            uncorrelated_subquery_map,
                                            lip_filter_configuration));
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
  TopLevelPlan(const PhysicalPtr &plan,
               const std::vector<PhysicalPtr> &shared_subplans,
               const std::unordered_map<expressions::ExprId, int> &uncorrelated_subquery_map,
               const LIPFilterConfigurationPtr &lip_filter_configuration)
      : plan_(plan),
        shared_subplans_(shared_subplans),
        uncorrelated_subquery_map_(uncorrelated_subquery_map),
        lip_filter_configuration_(lip_filter_configuration) {
    addChild(plan);
    for (const PhysicalPtr &shared_subplan : shared_subplans) {
      addChild(shared_subplan);
    }
  }

  PhysicalPtr plan_;
  // Stored in the topological ordering based on dependencies.
  std::vector<PhysicalPtr> shared_subplans_;
  std::unordered_map<expressions::ExprId, int> uncorrelated_subquery_map_;
  LIPFilterConfigurationPtr lip_filter_configuration_;

  DISALLOW_COPY_AND_ASSIGN(TopLevelPlan);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUERY_PLANNER_PHYSICAL_TOPLEVEL_PLAN_HPP_ */
