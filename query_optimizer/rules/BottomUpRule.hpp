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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_BOTTOM_UP_RULE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_BOTTOM_UP_RULE_HPP_

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
 * @brief Base class for a rule that performs a bottom-up traverse on a tree.
 */
template <class TreeType>
class BottomUpRule : public Rule<TreeType> {
 public:
  typedef std::shared_ptr<const TreeType> TreeNodePtr;

  /**
   * @brief Constructor.
   */
  BottomUpRule() {}

  /**
   * @brief Destructor.
   */
  ~BottomUpRule() override {}

  TreeNodePtr apply(const TreeNodePtr &tree) override {
    DCHECK(tree != nullptr);

    std::vector<std::shared_ptr<const TreeType>> new_children;
    bool has_changed_children = false;
    for (const std::shared_ptr<const TreeType> &child : tree->children()) {
      std::shared_ptr<const TreeType> new_child = apply(child);
      if (child != new_child && !has_changed_children) {
        has_changed_children = true;
      }
      new_children.push_back(new_child);
    }

    if (has_changed_children) {
      return applyToNode(tree->copyWithNewChildren(new_children));
    } else {
      return applyToNode(tree);
    }
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
  DISALLOW_COPY_AND_ASSIGN(BottomUpRule);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_RULES_BOTTOM_UP_RULE_HPP_ */
