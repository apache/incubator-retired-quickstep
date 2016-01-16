/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
