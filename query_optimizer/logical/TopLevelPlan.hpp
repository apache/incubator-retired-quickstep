/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_TOP_LEVEL_PLAN_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_TOP_LEVEL_PLAN_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
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
    return TopLevelPlan::Create(new_children[0], new_shared_subplans);
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
   * @return An immutable TopLevelPlan.
   */
  static TopLevelPlanPtr Create(const LogicalPtr &plan,
                                const std::vector<LogicalPtr> &shared_subplans = std::vector<LogicalPtr>()) {
    return TopLevelPlanPtr(new TopLevelPlan(plan, shared_subplans));
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
               const std::vector<LogicalPtr> &shared_subplans)
      : plan_(plan),
        shared_subplans_(shared_subplans) {
    addChild(plan);
    for (const LogicalPtr &shared_subplan : shared_subplans) {
      addChild(shared_subplan);
    }
  }

  LogicalPtr plan_;
  // Stored in the topological ordering based on dependencies.
  std::vector<LogicalPtr> shared_subplans_;

  DISALLOW_COPY_AND_ASSIGN(TopLevelPlan);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_TOP_LEVEL_PLAN_HPP_ */
