/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include "query_optimizer/rules/PushDownSemiAntiJoin.hpp"

#include <vector>

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/logical/HashJoin.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/PatternMatcher.hpp"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace L = ::quickstep::optimizer::logical;

L::LogicalPtr PushDownSemiAntiJoin::applyToNode(const L::LogicalPtr &input) {
  std::vector<L::LogicalPtr> new_children;
  bool has_changes = false;

  for (const L::LogicalPtr &child : input->children()) {
    L::HashJoinPtr hash_join;
    if (L::SomeHashJoin::MatchesWithConditionalCast(child, &hash_join)) {
      if (hash_join->join_type() == L::HashJoin::JoinType::kLeftSemiJoin ||
          hash_join->join_type() == L::HashJoin::JoinType::kLeftAntiJoin) {
        L::LogicalPtr new_child = pushDownSemiAntiJoin(hash_join);
        if (new_child != child) {
          has_changes = true;
        }
        new_children.push_back(new_child);
      }
    } else {
      new_children.push_back(child);
    }
  }

  if (has_changes) {
    return input->copyWithNewChildren(new_children);
  } else {
    return input;
  }
}

L::LogicalPtr PushDownSemiAntiJoin::pushDownSemiAntiJoin(
    const L::HashJoinPtr &semi_anti_join) {
  const L::LogicalPtr &left_input = semi_anti_join->left();
  std::vector<L::LogicalPtr> left_input_children = left_input->children();

  if (!left_input_children.empty()) {
    std::vector<L::LogicalPtr>::size_type last_input_index = left_input_children.size();
    // Cannot push down a Filter down the right child of LeftOuterJoin.
    L::HashJoinPtr hash_join;
    if (L::SomeHashJoin::MatchesWithConditionalCast(left_input, &hash_join) &&
        hash_join->join_type() == L::HashJoin::JoinType::kLeftOuterJoin) {
      DCHECK_EQ(2u, left_input_children.size());
      last_input_index = 1u;
    }

    std::vector<L::LogicalPtr>::size_type input_index = 0;
    while (input_index < last_input_index) {
      if (SubsetOfExpressions(semi_anti_join->left_join_attributes(),
                              left_input_children[input_index]->getOutputAttributes())) {
        break;
      }
      ++input_index;
    }

    if (input_index < last_input_index &&
        semi_anti_join->residual_predicate() != nullptr) {
      const std::vector<E::AttributeReferencePtr> input_child_output_attributes =
          left_input_children[input_index]->getOutputAttributes();
      const std::vector<E::AttributeReferencePtr> right_input_output_attributes =
          semi_anti_join->right()->getOutputAttributes();
      E::UnorderedAttributeSet visible_attribute_set(input_child_output_attributes.begin(),
                                                     input_child_output_attributes.end());
      visible_attribute_set.insert(right_input_output_attributes.begin(),
                                   right_input_output_attributes.end());

      const std::vector<E::AttributeReferencePtr> referenced_attrs_in_residual =
          E::GetAttributeReferencesWithinScope(
              semi_anti_join->residual_predicate()->getReferencedAttributes(),
              E::AttributeReferenceScope::kLocal);
      for (const E::AttributeReferencePtr &referenced_attr : referenced_attrs_in_residual) {
        if (visible_attribute_set.find(referenced_attr) == visible_attribute_set.end()) {
          input_index = last_input_index;
          break;
        }
      }
    }

    if (input_index < last_input_index) {
      left_input_children[input_index] =
          semi_anti_join->copyWithNewChildren({left_input_children[input_index],
                                               semi_anti_join->right()});
      return left_input->copyWithNewChildren(left_input_children);
    }
  }

  return semi_anti_join;
}

}  // namespace optimizer
}  // namespace quickstep
