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

#include "query_optimizer/rules/PruneColumns.hpp"

#include <vector>

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/physical/PatternMatcher.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/rules/Rule.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace P = ::quickstep::optimizer::physical;

P::PhysicalPtr PruneColumns::applyToNode(const P::PhysicalPtr &input) {
  if (input->getNumChildren() == 0 || P::SomeTopLevelPlan::Matches(input)) {
    LOG_IGNORING_RULE(input);
    return input;
  }

  const std::vector<E::AttributeReferencePtr> referenced_attributes_vector =
      input->getReferencedAttributes();
  const E::UnorderedNamedExpressionSet referenced_attributes_set(
      referenced_attributes_vector.begin(),
      referenced_attributes_vector.end());

  const std::vector<P::PhysicalPtr> &current_children = input->children();
  std::vector<P::PhysicalPtr> new_children = current_children;
  bool has_children_changed = false;
  for (std::vector<P::PhysicalPtr>::size_type i = 0; i < current_children.size(); ++i) {
    // Remove those project expressions that are not referenced in the parent
    // node.
    if (current_children[i]->maybeCopyWithPrunedExpressions(
            referenced_attributes_set, &new_children[i]) &&
        !has_children_changed) {
      has_children_changed = true;
    }
  }

  if (has_children_changed) {
    P::PhysicalPtr output = input->copyWithNewChildren(new_children);
    LOG_APPLYING_RULE(input, output);
    return output;
  } else {
    LOG_IGNORING_RULE(input);
    return input;
  }
}

}  // namespace optimizer
}  // namespace quickstep
