/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#include "query_optimizer/rules/GenerateJoins.hpp"

#include <unordered_map>
#include <utility>
#include <vector>

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ComparisonExpression.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/logical/Filter.hpp"
#include "query_optimizer/logical/HashJoin.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/MultiwayCartesianJoin.hpp"
#include "query_optimizer/logical/NestedLoopsJoin.hpp"
#include "query_optimizer/logical/PatternMatcher.hpp"
#include "query_optimizer/rules/Rule.hpp"
#include "query_optimizer/rules/RuleHelper.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/VectorUtil.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace L = ::quickstep::optimizer::logical;

typedef std::unordered_map<E::AttributeReferencePtr,
                           L::LogicalPtr,
                           E::NamedExpressionHash,
                           E::NamedExpressionEqual> AttributeToLogicalMap;

namespace {

// Info for a hash join predicate.
struct HashJoinPredicateInfo {
  HashJoinPredicateInfo(const E::AttributeReferencePtr &left_attribute_in,
                        const E::AttributeReferencePtr &right_attribute_in)
      : left_join_attributes({left_attribute_in}),
        right_join_attributes({right_attribute_in}) {}

  std::vector<E::AttributeReferencePtr> left_join_attributes;
  std::vector<E::AttributeReferencePtr> right_join_attributes;
};

typedef std::unordered_map<std::pair<L::LogicalPtr, L::LogicalPtr>,
                           HashJoinPredicateInfo*> HashJoinPredicateInfoMap;

// Info for a non-hash join predicate.
struct NonHashJoinPredicateInfo {
  explicit NonHashJoinPredicateInfo(const E::PredicatePtr &predicate_in)
      : predicate(predicate_in),
        referenced_attributes(predicate->getReferencedAttributes()) {}

  // Returns true if the predicate is a binary predicate and returns the two
  // referenced logical nodes in <updated_referenced_logical_list>.
  // If the predicate involves only a single logical node, returns the node in
  // <updated_referenced_logical_list>. Otherwise, <updated_referenced_logical_list>
  // is empty.
  bool isBinaryJoin(
      const AttributeToLogicalMap &attribute_to_logical_map,
      std::vector<L::LogicalPtr> *updated_referenced_logical_list) const {
    for (const E::AttributeReferencePtr &referenced_attribute :
         referenced_attributes) {
      const AttributeToLogicalMap::const_iterator found_it =
          attribute_to_logical_map.find(referenced_attribute);
      DCHECK(found_it != attribute_to_logical_map.end());
      if (AppendToVectorIfNotPresent(found_it->second,
                                     updated_referenced_logical_list) &&
          updated_referenced_logical_list->size() > 2u) {
        updated_referenced_logical_list->clear();
        return false;
      }
    }
    return updated_referenced_logical_list->size() == 2u;
  }

  const E::PredicatePtr predicate;
  const std::vector<E::AttributeReferencePtr> referenced_attributes;
};

// Maps the output attributes of <logical_operator> to <logical_operator>.
void MapAttributeToLogical(const L::LogicalPtr &logical_operator,
                           AttributeToLogicalMap *attribute_to_logical_map) {
  for (const E::AttributeReferencePtr &output_attribute :
       logical_operator->getOutputAttributes()) {
    (*attribute_to_logical_map)[output_attribute] = logical_operator;
  }
}

// Generates n-1 NestedLoopsJoin with an empty predicate to combine all n
// logical nodes in <inputs>.
template <class Container>
L::LogicalPtr MaybeGenerateNestedLoopsCartesianJoin(const Container &inputs) {
  typename Container::const_iterator non_joined_logical_it = inputs.begin();
  L::LogicalPtr output = *non_joined_logical_it;
  ++non_joined_logical_it;
  while (non_joined_logical_it != inputs.end()) {
    output = L::NestedLoopsJoin::Create(output,
                                        *non_joined_logical_it,
                                        E::PredicatePtr());
    ++non_joined_logical_it;
  }
  return output;
}

// Classifies the predicates in <predicates> into hash join predicates and
// non-hash join predicates.
void ClassifyPredicates(
    const std::vector<E::PredicatePtr> &predicates,
    const AttributeToLogicalMap &attribute_to_logical_map,
    std::vector<std::unique_ptr<const HashJoinPredicateInfo>> *hash_join_predicates,
    std::vector<std::unique_ptr<const NonHashJoinPredicateInfo>> *non_hash_join_predicates) {
  DCHECK(hash_join_predicates->empty());
  DCHECK(non_hash_join_predicates->empty());

  HashJoinPredicateInfoMap hash_join_predicate_map;
  for (const E::PredicatePtr &predicate : predicates) {
    E::ComparisonExpressionPtr comparison_predicate;
    bool is_hash_join_predicate = false;

    // Determine whether it is a hash join predicate which should be an equality
    // predicate between two attribute references.
    if (E::SomeComparisonExpression::MatchesWithConditionalCast(predicate,
                                                                &comparison_predicate)) {
      E::AttributeReferencePtr left_attribute_reference;
      E::AttributeReferencePtr right_attribute_reference;
      if (comparison_predicate->isEqualityComparisonPredicate() &&
          E::SomeAttributeReference::MatchesWithConditionalCast(comparison_predicate->left(),
                                                                &left_attribute_reference) &&
          E::SomeAttributeReference::MatchesWithConditionalCast(comparison_predicate->right(),
                                                                &right_attribute_reference)) {
        AttributeToLogicalMap::const_iterator left_logical_it =
            attribute_to_logical_map.find(left_attribute_reference);
        AttributeToLogicalMap::const_iterator right_logical_it =
            attribute_to_logical_map.find(right_attribute_reference);

        if (left_logical_it->second != right_logical_it->second) {
          // Update join attributes if we have added the logical pair.
          const std::pair<L::LogicalPtr, L::LogicalPtr> logical_pair(left_logical_it->second,
                                                                     right_logical_it->second);
          const HashJoinPredicateInfoMap::iterator predicate_it =
              hash_join_predicate_map.find(logical_pair);
          if (predicate_it != hash_join_predicate_map.end()) {
            predicate_it->second->left_join_attributes.emplace_back(left_attribute_reference);
            predicate_it->second->right_join_attributes.emplace_back(right_attribute_reference);
          } else {
            const std::pair<L::LogicalPtr, L::LogicalPtr> reversed_logical_pair(right_logical_it->second,
                                                                                left_logical_it->second);
            const HashJoinPredicateInfoMap::iterator reversed_predicate_it =
                hash_join_predicate_map.find(reversed_logical_pair);
            if (reversed_predicate_it != hash_join_predicate_map.end()) {
              reversed_predicate_it->second->left_join_attributes.emplace_back(right_attribute_reference);
              reversed_predicate_it->second->right_join_attributes.emplace_back(left_attribute_reference);
            } else {
              // Create a new logical pair.
              std::unique_ptr<HashJoinPredicateInfo> hash_join_predicate(
                  new HashJoinPredicateInfo(left_attribute_reference,
                                            right_attribute_reference));
              hash_join_predicate_map.emplace(
                  std::piecewise_construct,
                  std::forward_as_tuple(left_logical_it->second,
                                        right_logical_it->second),
                  std::forward_as_tuple(hash_join_predicate.get()));
              hash_join_predicates->emplace_back(hash_join_predicate.release());
            }
          }
          is_hash_join_predicate = true;
        }
      }
    }

    // Add it to <non_hash_join_predicates> if it is not a hash join predicate.
    if (!is_hash_join_predicate) {
      non_hash_join_predicates->emplace_back(
          new NonHashJoinPredicateInfo(predicate));
    }
  }
}

// Removes <left_logical> and <right_logical> from <logical_vec> and adds
// <logical_join> into it.
void UpdateLogicalVectors(const L::LogicalPtr &left_logical,
                          const L::LogicalPtr &right_logical,
                          const L::LogicalPtr &logical_join,
                          std::vector<L::LogicalPtr> *logical_vec) {
  int num_removed_logical = 0;
  for (std::vector<L::LogicalPtr>::iterator logical_it =
           logical_vec->begin();
       logical_it != logical_vec->end();) {
    if (*logical_it == left_logical || *logical_it == right_logical) {
      logical_it = logical_vec->erase(logical_it);
      ++num_removed_logical;
      if (num_removed_logical == 2) {
        break;
      }
    } else {
      ++logical_it;
    }
  }
  logical_vec->push_back(logical_join);
}

// Creates a nested loops join between <left_logical> and <right_logical>,
// updates the vector of unjoined logical nodes <unjoined_logical_vec>
// and the map from attributes to unjoined logical nodes <attribute_logical_map>.
// Note that we do not pass references for the first two arguments because they
// may come from the fourth argument which is subject to change.
void AddNestedLoopsJoin(const L::LogicalPtr left_logical,
                        const L::LogicalPtr right_logical,
                        const E::PredicatePtr &predicate,
                        std::vector<L::LogicalPtr> *unjoined_logical_vec,
                        AttributeToLogicalMap *attribute_logical_map) {
  const L::LogicalPtr join_logical =
      L::NestedLoopsJoin::Create(left_logical, right_logical, predicate);
  UpdateLogicalVectors(left_logical, right_logical, join_logical,
                       unjoined_logical_vec);
  MapAttributeToLogical(join_logical, attribute_logical_map);
}

}  // namespace

L::LogicalPtr GenerateJoins::applyToNode(const L::LogicalPtr &input) {
  L::FilterPtr filter;
  L::MultiwayCartesianJoinPtr cartesian_join;

  // Merge filter predicates with cartesian joins to generate NestedLoopsJoin or
  // HashJoin.
  // TODO(qzeng): Maybe pull up MultiwayCartesianJoin first if there are
  // multiple MultiwayCartesianJoin and generate joins holistically for
  // all MultiwayCartesianJoin nodes. It have some benefits when tables in
  // the nested queries can only be hash joined with outer query tables.
  // However the case can be deemed as rare and it is low priority.
  if (L::SomeFilter::MatchesWithConditionalCast(input, &filter) &&
      L::SomeMultiwayCartesianJoin::MatchesWithConditionalCast(filter->input(), &cartesian_join)) {
    const std::vector<L::LogicalPtr> join_inputs = cartesian_join->children();
    // Logical nodes that have not participated in a join.
    std::vector<L::LogicalPtr> non_joined_logical_list(join_inputs.begin(),
                                                       join_inputs.end());

    // Map from each attribute to the logical node that outputs the attribute.
    AttributeToLogicalMap attribute_to_logical_map;
    // Populate <attribute_to_logical_map>.
    for (const L::LogicalPtr &join_input : join_inputs) {
      MapAttributeToLogical(join_input, &attribute_to_logical_map);
    }

    std::vector<std::unique_ptr<const HashJoinPredicateInfo>>
        hash_join_predicates;
    std::vector<std::unique_ptr<const NonHashJoinPredicateInfo>>
        non_hash_join_predicates;
    // Classify the predicates in Filter to either hash join predicates or non
    // hash join predicates.
    ClassifyPredicates(GetConjunctivePredicates(filter->filter_predicate()),
                       attribute_to_logical_map,
                       &hash_join_predicates,
                       &non_hash_join_predicates);

    // The predicates that are not used in any joins.
    std::vector<E::PredicatePtr> filter_predicates;

    // First, create an inner HashJoin for each hash join predicate in the order
    // as specified in the query.
    for (const std::unique_ptr<const HashJoinPredicateInfo> &
             hash_join_predicate_info : hash_join_predicates) {
      const L::LogicalPtr left_logical =
          attribute_to_logical_map[hash_join_predicate_info->left_join_attributes[0]];
      const L::LogicalPtr right_logical =
          attribute_to_logical_map[hash_join_predicate_info->right_join_attributes[0]];
      DCHECK(left_logical != nullptr);
      DCHECK(right_logical != nullptr);

      if (left_logical != right_logical) {
        L::LogicalPtr logical_join = L::HashJoin::Create(
            left_logical,
            right_logical,
            hash_join_predicate_info->left_join_attributes,
            hash_join_predicate_info->right_join_attributes,
            nullptr /* residual_predicate */,
            L::HashJoin::JoinType::kInnerJoin);
        UpdateLogicalVectors(left_logical,
                             right_logical,
                             logical_join,
                             &non_joined_logical_list);
        MapAttributeToLogical(logical_join, &attribute_to_logical_map);
      } else {
        std::vector<E::PredicatePtr> equality_predicates;
        for (std::vector<E::AttributeReferencePtr>::size_type attr_idx = 0;
             attr_idx < hash_join_predicate_info->left_join_attributes.size();
             ++attr_idx) {
          filter_predicates.emplace_back(
              E::ComparisonExpression::Create(ComparisonFactory::GetComparison(ComparisonID::kEqual),
                                              hash_join_predicate_info->left_join_attributes[attr_idx],
                                              hash_join_predicate_info->right_join_attributes[attr_idx]));
        }
      }
    }

    // Second, add NestedLoopsJoin until there is no join predicates left. The
    // loop is guaranteed to be terminated, since each iteration will eliminate
    // at least one predicate.
    while (!non_hash_join_predicates.empty()) {
      bool has_new_join = false;
      std::vector<std::unique_ptr<const NonHashJoinPredicateInfo>>
          new_non_hash_join_predicates;
      for (std::unique_ptr<const NonHashJoinPredicateInfo> &non_hash_join_predicate : non_hash_join_predicates) {
        std::vector<L::LogicalPtr> referenced_logical_list;

        // 1. If the join predicate is binary, join the two referenced logical
        //    nodes.
        // 2. If the join predicate involves more than two logical nodes, keep
        //    it in <new_non_hash_join_predicates> for consideration in the next iteration.
        // 3. If the join predicate can be applied to exactly one logical node,
        //    add it to unused_predicates which will be later added into a Filter.
        if (non_hash_join_predicate->isBinaryJoin(attribute_to_logical_map,
                                                  &referenced_logical_list)) {
          AddNestedLoopsJoin(
              referenced_logical_list[0],
              referenced_logical_list[1],
              non_hash_join_predicate->predicate,
              &non_joined_logical_list,
              &attribute_to_logical_map);
          has_new_join = true;
        } else if (!referenced_logical_list.empty()) {
          DCHECK_EQ(referenced_logical_list.size(), 1u);
          filter_predicates.push_back(non_hash_join_predicate->predicate);
        } else {
          new_non_hash_join_predicates.emplace_back(
              non_hash_join_predicate.release());
        }
      }

      // Update non_hash_join_predicates with new_non_hash_join_predicate for
      // use in the next iteration.
      non_hash_join_predicates.clear();
      for (std::unique_ptr<const NonHashJoinPredicateInfo> &new_non_hash_join_predicate :
           new_non_hash_join_predicates) {
        non_hash_join_predicates.emplace_back(
            new_non_hash_join_predicate.release());
      }

      // If we did not find a binary join predicate, join the first two logical
      // nodes in <non_joined_logical_list>.
      if (!has_new_join && non_joined_logical_list.size() >= 2u) {
        AddNestedLoopsJoin(non_joined_logical_list[0],
                           non_joined_logical_list[1],
                           E::PredicatePtr(),
                           &non_joined_logical_list,
                           &attribute_to_logical_map);
      }
    }

    DCHECK(non_hash_join_predicates.empty());
    // Assert that there must be one remaining logical node.
    DCHECK(!non_joined_logical_list.empty());

    // Cross product all remaining logical nodes.
    L::LogicalPtr output = MaybeGenerateNestedLoopsCartesianJoin(non_joined_logical_list);

    if (!filter_predicates.empty()) {
      output = L::Filter::Create(output, filter_predicates);
    }

    LOG_APPLYING_RULE(input, output);
    return output;
  } else if (L::SomeMultiwayCartesianJoin::MatchesWithConditionalCast(input, &cartesian_join)) {
    // If there is no filter upon the cartesian join node, convert it to nested
    // loop cartesian joins.
    L::LogicalPtr output = MaybeGenerateNestedLoopsCartesianJoin(cartesian_join->operands());
    LOG_APPLYING_RULE(input, output);
    return output;
  }

  LOG_IGNORING_RULE(input);
  return input;
}

}  // namespace optimizer
}  // namespace quickstep
