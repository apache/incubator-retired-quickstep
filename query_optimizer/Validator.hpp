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
