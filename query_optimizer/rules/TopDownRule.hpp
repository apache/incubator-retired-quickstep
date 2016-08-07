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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_TOP_DOWN_RULE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_TOP_DOWN_RULE_HPP_

#include <memory>
#include <vector>

#include "query_optimizer/rules/Rule.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

/** \addtogroup OptimizerRules
 *  @{
 */

/**
 * @brief Base class for a rule that performs a top-down traverse on a tree.
 */
template <class TreeType>
class TopDownRule : public Rule<TreeType> {
 public:
  typedef std::shared_ptr<const TreeType> TreeNodePtr;

  /**
   * @brief Constructor.
   */
  TopDownRule() {}

  /**
   * @brief Destructor.
   */
  ~TopDownRule() override {}

  TreeNodePtr apply(const TreeNodePtr &tree) override {
    DCHECK(tree != nullptr);
    TreeNodePtr new_tree;

    new_tree = applyToNode(tree);
    DCHECK(new_tree != nullptr);

    // TODO(qzeng): We assume that the ordering of the children is irrelevant,
    // but this may not be true for TopLevelPlan if we implement some rules
    // (e.g. outer-join elimination) that may change the output attributes of
    // a query plan. For a top-down traverse, the least dependent shared
    // subplan should be the last one to be processed, while it should be
    // visited first for a bottom-up traverse.
    std::vector<TreeNodePtr> new_children;
    bool has_changed_children = false;
    const std::vector<TreeNodePtr> &current_children = new_tree->children();
    for (const TreeNodePtr &current_child : current_children) {
      TreeNodePtr new_child;
      new_child = apply(current_child);
      if (current_child != new_child && !has_changed_children) {
        has_changed_children = true;
      }
      new_children.push_back(new_child);
    }

    if (has_changed_children) {
      new_tree = new_tree->copyWithNewChildren(new_children);
    }

    return new_tree;
  }

 protected:
  /**
   * @brief Applies the rule to a single node, not the entire tree.
   *
   * @param node The node to apply the rule to.
   * @param The updated node by the rule.
   */
  virtual TreeNodePtr applyToNode(const TreeNodePtr &node) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(TopDownRule);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_RULES_TOP_DOWN_RULE_HPP_ */
