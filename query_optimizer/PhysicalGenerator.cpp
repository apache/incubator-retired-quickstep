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

#include "query_optimizer/PhysicalGenerator.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

#include "query_optimizer/Validator.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/rules/AttachLIPFilters.hpp"
#include "query_optimizer/rules/CollapseSelection.hpp"
#include "query_optimizer/rules/ExtractCommonSubexpression.hpp"
#include "query_optimizer/rules/FuseAggregateJoin.hpp"
#include "query_optimizer/rules/InjectJoinFilters.hpp"
#include "query_optimizer/rules/Partition.hpp"
#include "query_optimizer/rules/PruneColumns.hpp"
#include "query_optimizer/rules/PushDownLowCostDisjunctivePredicate.hpp"
#include "query_optimizer/rules/ReduceGroupByAttributes.hpp"
#include "query_optimizer/rules/ReorderColumns.hpp"
#include "query_optimizer/rules/ReuseAggregateExpressions.hpp"
#include "query_optimizer/rules/StarSchemaHashJoinOrderOptimization.hpp"
#include "query_optimizer/rules/SwapProbeBuild.hpp"
#include "query_optimizer/strategy/Aggregate.hpp"
#include "query_optimizer/strategy/Join.hpp"
#include "query_optimizer/strategy/OneToOne.hpp"
#include "query_optimizer/strategy/Selection.hpp"
#include "query_optimizer/strategy/Strategy.hpp"
#include "utility/PlanVisualizer.hpp"

#include "gflags/gflags.h"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

DEFINE_bool(reorder_columns, true,
            "Adjust the ordering of intermediate relations' columns to improve "
            "copy performance.");

DEFINE_bool(reorder_hash_joins, true,
            "If true, apply hash join order optimization to each group of hash "
            "joins. The optimization applies a greedy algorithm to favor smaller "
            "cardinality and selective tables to be joined first, which is suitable "
            "for queries on star-schema tables.");

DEFINE_bool(use_partition_rule, true,
            "If true, apply an optimization to support partitioned inputs. The "
            "optimization may add additional Selection for repartitioning.");

DEFINE_bool(use_filter_joins, true,
            "If true, apply an optimization that strength-reduces HashJoins to "
            "FilterJoins (implemented as LIPFilters attached to some anchoring "
            "operators. Briefly speaking, in the case that the join attribute has "
            "consecutive integer values bounded in a reasonably small range, we "
            "build a BitVector on the build-side attribute and use the BitVector "
            "to filter the probe side table.");

DEFINE_bool(use_lip_filters, true,
            "If true, use LIP (Lookahead Information Passing) filters to accelerate "
            "query processing. LIP filters are effective for queries on star schema "
            "tables (e.g. the SSB benchmark) and snowflake schema tables (e.g. the "
            "TPC-H benchmark).");

DEFINE_bool(visualize_plan, false,
            "If true, visualize the final physical plan into a graph in DOT format "
            "(DOT is a plain text graph description language). Then print the "
            "generated graph through stderr.");

namespace L = ::quickstep::optimizer::logical;
namespace P = ::quickstep::optimizer::physical;
namespace S = ::quickstep::optimizer::strategy;

void PhysicalGenerator::createStrategies() {
  // Note that the ordering matters.
  // The strategy that can create better plans should be inserted first.
  strategies_.emplace_back(new S::Join(this));
  strategies_.emplace_back(new S::Aggregate(this));
  strategies_.emplace_back(new S::Selection(this));
  strategies_.emplace_back(new S::OneToOne(this));
}

P::PhysicalPtr PhysicalGenerator::generatePlan(
    const L::LogicalPtr &logical_plan) {
  physical_plan_ = generateInitialPlan(logical_plan);
  return optimizePlan();
}

P::PhysicalPtr PhysicalGenerator::generateInitialPlan(
    const L::LogicalPtr &logical_plan) {
  // Choose the first generated physical plan as the best one.
  P::PhysicalPtr physical_plan;
  for (std::unique_ptr<S::Strategy> &strategy : strategies_) {
    DVLOG(5) << "Apply strategy " << strategy->getName() << " to "
             << logical_plan->getShortString();
    if (strategy->generatePlan(logical_plan, &physical_plan)) {
      DVLOG(5) << "Result:\n" << physical_plan->toString();
      break;
    }
  }
  CHECK(physical_plan != nullptr) << "No strategy for a logical plan:\n"
                                  << logical_plan->toString();

  // Memorize the created physical plan.
  setBestPhysicalForLogical(logical_plan, physical_plan);
  return physical_plan;
}

P::PhysicalPtr PhysicalGenerator::optimizePlan() {
  std::vector<std::unique_ptr<Rule<P::Physical>>> rules;
  rules.emplace_back(new PruneColumns());

  rules.emplace_back(new PushDownLowCostDisjunctivePredicate());

  rules.emplace_back(new ReduceGroupByAttributes(optimizer_context_));

  if (FLAGS_reorder_hash_joins) {
    rules.emplace_back(new StarSchemaHashJoinOrderOptimization());
    rules.emplace_back(new PruneColumns());
  } else {
    rules.emplace_back(new SwapProbeBuild());
  }

  if (FLAGS_reorder_columns) {
    // NOTE(jianqiao): This optimization relies on the fact that the intermediate
    // relations all have SPLIT_ROW_STORE layouts. If this fact gets changed, the
    // optimization algorithm may need to be updated and the performance impact
    // should be re-evaluated.
    rules.emplace_back(new ReorderColumns());
  }

  // This optimization pass eliminates duplicate aggregates and converts AVG to
  // SUM/COUNT if appropriate. Note that this optimization needs to be done before
  // ExtractCommonSubexpression.
  rules.emplace_back(new ReuseAggregateExpressions(optimizer_context_));

  rules.emplace_back(new FuseAggregateJoin());

  // Some of the optimization passes (e.g. PushDownLowCostDisjunctivePredicate
  // and ReuseAggregateExpressions) might add extra Selection nodes and extra
  // projection columns for their convenience. So we collapse Selection nodes
  // and prune unnecessary columns here.
  rules.emplace_back(new CollapseSelection());
  rules.emplace_back(new PruneColumns());

  // This optimization pass identifies common subexpressions and wraps them with
  // CommonSubexpression nodes, where identical CommonSubexpression nodes share
  // a same unique integer ID. Later in the backend we use memoization tables to
  // memorize the result column vectors for each ID so that each group has its
  // common subexpression evaluated only once.
  rules.emplace_back(new ExtractCommonSubexpression(optimizer_context_));

  // This optimization pass may add additional Selection for repartitions, and
  // set output PartitionSchemeHeader in a Physical Plan node, when needed.
  if (FLAGS_use_partition_rule) {
    rules.push_back(std::make_unique<Partition>(optimizer_context_));
  }

  // NOTE(jianqiao): Adding rules after InjectJoinFilters (or AttachLIPFilters)
  // requires extra handling of LIPFilterConfiguration for transformed nodes.
  // So currently it is suggested that all the new rules be placed before this
  // point.
  if (FLAGS_use_filter_joins) {
    rules.emplace_back(new InjectJoinFilters());
  }

  if (FLAGS_use_lip_filters) {
    rules.emplace_back(new AttachLIPFilters());
  }

  for (std::unique_ptr<Rule<P::Physical>> &rule : rules) {
    physical_plan_ = rule->apply(physical_plan_);
    DVLOG(5) << "After applying rule " << rule->getName() << ":\n"
             << physical_plan_->toString();
  }

  DVLOG(4) << "Optimized physical plan:\n" << physical_plan_->toString();

  if (FLAGS_visualize_plan) {
    quickstep::PlanVisualizer plan_visualizer;
    std::cerr << "\n" << plan_visualizer.visualize(physical_plan_) << "\n";
  }

#ifdef QUICKSTEP_DEBUG
  Validate(physical_plan_);
#endif
  return physical_plan_;
}

void PhysicalGenerator::setBestPhysicalForLogical(
    const L::LogicalPtr &logical_plan, const P::PhysicalPtr &physical_plan) {
  DCHECK(logical_plan != nullptr);
  DCHECK(physical_plan != nullptr);
  logical_to_physical_map_.emplace(logical_plan, physical_plan);
}

P::PhysicalPtr PhysicalGenerator::createOrGetPhysicalFromLogical(
    const L::LogicalPtr &logical_plan) {
  DCHECK(logical_plan != nullptr);
  // Check if we have created the physical plan.
  const std::unordered_map<L::LogicalPtr, P::PhysicalPtr>::const_iterator it =
      logical_to_physical_map_.find(logical_plan);
  if (it != logical_to_physical_map_.end()) {
    return it->second;
  }

  return generateInitialPlan(logical_plan);
}

}  // namespace optimizer
}  // namespace quickstep
