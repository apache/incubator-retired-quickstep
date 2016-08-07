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

#include "query_optimizer/rules/PushDownFilter.hpp"

#include <cstddef>
#include <vector>

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/logical/Filter.hpp"
#include "query_optimizer/logical/HashJoin.hpp"
#include "query_optimizer/logical/PatternMatcher.hpp"
#include "query_optimizer/rules/Rule.hpp"
#include "query_optimizer/rules/RuleHelper.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace L = ::quickstep::optimizer::logical;

L::LogicalPtr PushDownFilter::applyToNode(const L::LogicalPtr &input) {
  L::FilterPtr filter;
  bool applied = false;

  // The rule is only applied to Filters.
  if (L::SomeFilter::MatchesWithConditionalCast(input, &filter)) {
    const std::vector<E::PredicatePtr> conjunction_items =
        GetConjunctivePredicates(filter->filter_predicate());
    // Predicates that cannot be pushed down.
    std::vector<E::PredicatePtr> unchanged_predicates;

    // We consider if the filter predicates can be pushed under the input of the
    // Filter.
    const L::LogicalPtr &input = filter->input();
    const std::vector<L::LogicalPtr> &input_children = input->children();

    // Store the predicates that can be pushed down to be upon each child node
    // of the Filter input.
    std::vector<std::vector<E::PredicatePtr>> predicates_to_be_pushed(
        input_children.size());
    if (!input_children.empty()) {
      std::size_t last_input_index = input_children.size();

      // Cannot push down a Filter down the right child of LeftOuterJoin.
      L::HashJoinPtr hash_join;
      if (L::SomeHashJoin::MatchesWithConditionalCast(input, &hash_join) &&
          hash_join->join_type() == L::HashJoin::JoinType::kLeftOuterJoin) {
        DCHECK_EQ(2u, input_children.size());
        last_input_index = 1u;
      }

      for (const E::PredicatePtr &conjuntion_item : conjunction_items) {
        bool can_be_pushed = false;
        const std::vector<E::AttributeReferencePtr> referenced_attributes =
            conjuntion_item->getReferencedAttributes();

        // A predicate can be pushed down only if the set of attributes
        // referenced by the predicate is a subset of output attributes
        // of a child.
        for (std::vector<L::LogicalPtr>::size_type input_index = 0;
             input_index < last_input_index;
             ++input_index) {
          if (SubsetOfExpressions(
                  referenced_attributes,
                  input_children[input_index]->getOutputAttributes())) {
            predicates_to_be_pushed[input_index].push_back(conjuntion_item);
            can_be_pushed = true;
            break;
          }
        }

        if (!can_be_pushed) {
          unchanged_predicates.push_back(conjuntion_item);
        } else if (!applied) {
          applied = true;
        }
      }

      // Create a filter upon a child if there is any filter that can be pushed
      // down along the path between the child and the parent.
      std::vector<L::LogicalPtr> new_input_children;
      for (std::vector<L::LogicalPtr>::size_type input_index = 0;
           input_index < input_children.size();
           ++input_index) {
        if (predicates_to_be_pushed[input_index].empty()) {
          new_input_children.push_back(input_children[input_index]);
        } else {
          new_input_children.push_back(
              L::Filter::Create(input_children[input_index],
                                predicates_to_be_pushed[input_index]));
        }
      }

      // Replace the child nodes with the new ones to get a new input to the
      // Filter.
      if (applied) {
        L::LogicalPtr new_input =
            input->copyWithNewChildren(new_input_children);
        L::LogicalPtr output;

        // If there are no remaining predicates, we need to remove the current
        // Filter node and the output is the result of the rule being applied
        // to the input of the Filter.
        if (unchanged_predicates.empty()) {
          output = applyToNode(new_input);
        } else {
          output = L::Filter::Create(new_input, unchanged_predicates);
        }
        LOG_APPLYING_RULE(input, output);
        return output;
      }
    }
  }

  LOG_IGNORING_RULE(input);
  return input;
}

}  // namespace optimizer
}  // namespace quickstep
