/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_VALIDATOR_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_VALIDATOR_HPP_

#include <memory>
#include <vector>

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

/** \addtogroup QueryOptimizer
 *  @{
 */

/**
 * @brief Validate that the plan does not have a dangling attribute reference.
 *
 * @param plan The plan to be validated.
 */
template <class PlanType>
static void Validate(const std::shared_ptr<const PlanType> &plan) {
  if (plan->getNumChildren() > 0) {
    const std::vector<expressions::AttributeReferencePtr>
        referenced_attributes = plan->getReferencedAttributes();
    std::vector<expressions::AttributeReferencePtr> visible_attributes;
    for (const std::shared_ptr<const PlanType> &child : plan->children()) {
      Validate<PlanType>(child);
      const std::vector<expressions::AttributeReferencePtr> input_attributes =
          child->getOutputAttributes();
      visible_attributes.insert(visible_attributes.end(),
                                input_attributes.begin(),
                                input_attributes.end());
    }
    for (const expressions::AttributeReferencePtr &referenced_attribute :
         referenced_attributes) {
      CHECK(expressions::ContainsExpression(visible_attributes,
                                            referenced_attribute))
          << "\nDangling attribute reference "
          << referenced_attribute->getShortString() << " in the plan:\n"
          << plan->toString();
    }
  }
}

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_VALIDATOR_HPP_ */
