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

#include "query_optimizer/rules/FuseGeneralHash.hpp"

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
#include "query_optimizer/physical/TableReference.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace P = ::quickstep::optimizer::physical;

P::PhysicalPtr FuseGeneralHash::applyToNode(
    const P::PhysicalPtr &node) {
  P::HashJoinPtr hash_join;

  // Check to see if the join is a hash join, if the hash join is an inner
  // join, it has a null build predicate, and that there are no partitions.
  // TODO(dbacon): Add support for other join types.

  if ((!P::SomeHashJoin::MatchesWithConditionalCast(node, &hash_join)) ||
      hash_join->getName() != "HashJoin" ||
      hash_join->build_predicate() != nullptr ||
      hash_join->getOutputPartitionSchemeHeader() != nullptr) {
    return node;
  }

  // Test if the probe side of the first hash join is also a hash join that fits
  // the above requirements.

  P::PhysicalPtr right_child = hash_join->right();
  P::PhysicalPtr left_child = hash_join->left();
  P::HashJoinPtr second_hash;

  if ((!P::SomeHashJoin::MatchesWithConditionalCast(left_child, &second_hash)) ||
      second_hash->getName() != "HashJoin" ||
      second_hash->build_predicate() != nullptr ||
      second_hash->getOutputPartitionSchemeHeader() != nullptr) {
    return node;
  }

  // Check that the build side of the first hash join is a Selection or table reference.

  P::SelectionPtr selection;
  P::TableReferencePtr tableRef;
  if (!P::SomeSelection::MatchesWithConditionalCast(right_child, &selection)) {
    if(!P::SomeTableReference::MatchesWithConditionalCast(right_child, &tableRef)) {
      return node;
    }
  }

  P::PhysicalPtr second_right_child = second_hash->right();

  // Check that the build side of the second hash join is a Selection or table reference.

  P::SelectionPtr second_selection;
  P::TableReferencePtr second_tableRef;
  if (!P::SomeSelection::MatchesWithConditionalCast(second_right_child, &second_selection)) {
    if(!P::SomeTableReference::MatchesWithConditionalCast(second_right_child, &second_tableRef)) {
      return node;
    }
  }

  P::PhysicalPtr second_left_child = second_hash->left();

  // Check that the probe side of the second hash join is a Selection or table reference.

  P::SelectionPtr third_selection;
  P::TableReferencePtr third_tableRef;
  if(!P::SomeSelection::MatchesWithConditionalCast(second_left_child, &third_selection)) {
    if(!P::SomeTableReference::MatchesWithConditionalCast(second_left_child, &third_tableRef)) {
      return node;
    }
  }


  return P::GeneralizedHashJoin::Create(second_hash->left(),
                             second_hash->right(),
                             hash_join->right(),
                             second_hash->left_join_attributes(),
                             second_hash->right_join_attributes(),
                             hash_join->left_join_attributes(),
                             hash_join->right_join_attributes(),
                             second_hash->residual_predicate(),
                             hash_join->residual_predicate(),
                             hash_join->build_predicate(),
                             hash_join->project_expressions(),
                             physical::HashJoin::JoinType::kGeneralizedInnerJoin,
                             hash_join->hasRepartition(),
                             hash_join->cloneOutputPartitionSchemeHeader());
}

}  // namespace optimizer
}  // namespace quickstep
