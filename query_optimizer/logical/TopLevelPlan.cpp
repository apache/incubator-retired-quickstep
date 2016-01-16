/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/logical/TopLevelPlan.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "utility/Cast.hpp"

namespace quickstep {
namespace optimizer {
namespace logical {

namespace E = ::quickstep::optimizer::expressions;

void TopLevelPlan::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  non_container_child_field_names->push_back("plan");
  non_container_child_fields->push_back(plan_);

  if (!shared_subplans_.empty()) {
    container_child_field_names->push_back("shared_subplans");
    container_child_fields->push_back(
        CastSharedPtrVector<OptimizerTreeBase>(shared_subplans_));
  }

  container_child_field_names->push_back("output_attributes");
  container_child_fields->push_back(
      CastSharedPtrVector<OptimizerTreeBase>(plan_->getOutputAttributes()));
}

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep
