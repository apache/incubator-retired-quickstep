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

#include "query_optimizer/rules/FuseHashSelect.hpp"

#include <algorithm>
#include <cstddef>
#include <unordered_set>
#include <vector>

#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/PatternMatcher.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "query_optimizer/physical/Selection.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace P = ::quickstep::optimizer::physical;

P::PhysicalPtr FuseHashSelect::applyToNode(
    const P::PhysicalPtr &node) {
  P::HashJoinPtr hash_join;

  // Check to see if the join is a hash join, if the hash join is an inner
  // join.. We also check that there are no partitions.
  // TODO(dbacon): Add support for other join types.

  if ((!P::SomeHashJoin::MatchesWithConditionalCast(node, &hash_join)) ||
      hash_join->getOutputPartitionSchemeHeader() != nullptr) {
    return node;
  }

  // Get the join attributes from the build side.

  P::PhysicalPtr right_child = hash_join->right();
  const std::vector<E::AttributeReferencePtr> &right_join_attributes =
      hash_join->right_join_attributes();
  E::PredicatePtr build_predicate = nullptr;

  // Check that the build side is a Selection and that the join attributes match up.
  // If so, set the new hash join build side to the Selection input and the build predicate
  // to the selection's filter.

  P::SelectionPtr selection;
  if (P::SomeSelection::MatchesWithConditionalCast(right_child, &selection)) {
    if (E::SubsetOfExpressions(right_join_attributes,
                               selection->input()->getOutputAttributes())) {
      right_child = selection->input();
      build_predicate = selection->filter_predicate();
    }
  }

  return P::HashJoin::Create(hash_join->left(),
                             right_child,
                             hash_join->left_join_attributes(),
                             right_join_attributes,
                             hash_join->residual_predicate(),
                             build_predicate,
                             hash_join->project_expressions(),
                             hash_join->join_type(),
                             hash_join->hasRepartition(),
                             hash_join->cloneOutputPartitionSchemeHeader());
}

}  // namespace optimizer
}  // namespace quickstep
