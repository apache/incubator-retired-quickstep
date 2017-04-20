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

#include "query_optimizer/rules/ReuseAggregateExpressions.hpp"

#include <cstddef>
#include <list>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include "expressions/aggregation/AggregateFunction.hpp"
#include "expressions/aggregation/AggregateFunctionFactory.hpp"
#include "expressions/aggregation/AggregationID.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/expressions/AggregateFunction.hpp"
#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/BinaryExpression.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/PatternMatcher.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "utility/HashError.hpp"

#include "gflags/gflags.h"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

DEFINE_uint64(reuse_aggregate_group_size_threshold, 1000u,
              "The threshold on estimated number of groups for an aggregation "
              "below which the ReuseAggregateExpressions optimization will be "
              "performed.");

DEFINE_double(reuse_aggregate_ratio_threshold, 0.3,
              "The threshold on the ratio of (# of eliminable columns) to "
              "(# of original columns) for an aggregation above which the "
              "ReuseAggregateExpressions optimization will be performed.");

namespace E = ::quickstep::optimizer::expressions;
namespace P = ::quickstep::optimizer::physical;

void ReuseAggregateExpressions::init(const P::PhysicalPtr &input) {
  DCHECK(input->getPhysicalType() == P::PhysicalType::kTopLevelPlan);

  // Initialize cost model.
  const P::TopLevelPlanPtr top_level_plan =
     std::static_pointer_cast<const P::TopLevelPlan>(input);
  cost_model_.reset(
      new cost::StarSchemaSimpleCostModel(top_level_plan->shared_subplans()));
}

P::PhysicalPtr ReuseAggregateExpressions::applyToNode(
    const P::PhysicalPtr &input) {
  P::AggregatePtr aggregate;
  if (!P::SomeAggregate::MatchesWithConditionalCast(input, &aggregate)) {
    return input;
  }

  const std::vector<E::AliasPtr> &agg_exprs = aggregate->aggregate_expressions();

  // Maps aggregated expressions to AggregationID + positions.
  //
  // For example,
  // --
  //   SELECT SUM(x+1), AVG(x+1), SUM(x+1), COUNT(*), SUM(y) FROM s;
  // --
  // will generate *agg_expr_info* as
  // --
  // {
  //   x+1: { kSum: [0, 2], kAvg: [1] },
  //   y: { kSum: [4] },
  // }
  // --
  // and COUNT(*) will be recorded inside *count_star_info*.
  std::unordered_map<E::ScalarPtr,
                     std::map<AggregationID, std::vector<std::size_t>>,
                     ScalarHash, ScalarEqual> agg_expr_info;
  std::list<std::size_t> count_star_info;

  for (std::size_t i = 0; i < agg_exprs.size(); ++i) {
    DCHECK(agg_exprs[i]->expression()->getExpressionType()
               == E::ExpressionType::kAggregateFunction);
    const E::AggregateFunctionPtr agg_expr =
        std::static_pointer_cast<const E::AggregateFunction>(
            agg_exprs[i]->expression());

    // Skip DISTINCT aggregations.
    if (agg_expr->is_distinct()) {
      continue;
    }

    const AggregationID agg_id = agg_expr->getAggregate().getAggregationID();
    const std::vector<E::ScalarPtr> &arguments = agg_expr->getArguments();

    // Currently we only consider aggregate functions with 0 or 1 argument.
    if (agg_id == AggregationID::kCount) {
      if (arguments.empty()) {
        count_star_info.emplace_front(i);
        continue;
      } else if (!arguments.front()->getValueType().isNullable()) {
        // For COUNT(x) where x is not null, we view it as a more general COUNT(*).
        count_star_info.emplace_back(i);
        continue;
      }
    }
    if (arguments.size() == 1) {
      try {
        agg_expr_info[arguments.front()][agg_id].emplace_back(i);
      } catch (const HashNotSupported &e) {
        continue;
      }
    }
  }

  // Now for each aggregate expression, figure out whether we can avoid the
  // computation by reusing other aggregate expression's result.
  std::vector<std::unique_ptr<AggregateReference>> agg_refs(agg_exprs.size());

  constexpr std::size_t kInvalidRef = static_cast<std::size_t>(-1);
  std::size_t count_star_ref;

  // Check whether COUNT(*) is available.
  if (count_star_info.empty()) {
    count_star_ref = kInvalidRef;
  } else {
    auto it = count_star_info.begin();
    count_star_ref = *it;

    for (++it; it != count_star_info.end(); ++it) {
      agg_refs[*it].reset(new AggregateReference(count_star_ref));
    }
  }

  // Iterate through agg_expr_info to find all transformation opportunities,
  // and record them into agg_refs.
  for (const auto &it : agg_expr_info) {
    const auto &ref_map = it.second;

    // First, check whether AVG can be reduced to SUM/COUNT.
    bool is_avg_processed = false;

    const auto avg_it = ref_map.find(AggregationID::kAvg);
    if (avg_it != ref_map.end()) {
      std::size_t count_ref = kInvalidRef;

      // To reduce AVG to SUM/COUNT, we need a COUNT available.
      // TODO(jianqiao): We may even add a new COUNT(*) aggregation if it is not
      // there. E.g. when there are at least two AVG(...) aggregate functions.
      if (it.first->getValueType().isNullable()) {
        const auto count_it = ref_map.find(AggregationID::kCount);
        if (count_it != ref_map.end()) {
          DCHECK(!count_it->second.empty());
          count_ref = count_it->second.front();
        }
      } else {
        count_ref = count_star_ref;
      }

      if (count_ref != kInvalidRef) {
        // It is done if there is an existing SUM. Otherwise we strength-reduce
        // the current AVG to SUM.
        const auto sum_it = ref_map.find(AggregationID::kSum);
        const std::size_t sum_ref =
            sum_it == ref_map.end() ? kInvalidRef : sum_it->second.front();

        for (const std::size_t idx : avg_it->second) {
          agg_refs[idx].reset(new AggregateReference(sum_ref, count_ref));
        }
        is_avg_processed = true;
      }
    }

    // Then, identify duplicate aggregate expressions.
    for (const auto &ref_it : ref_map) {
      if (ref_it.first == AggregationID::kAvg && is_avg_processed) {
        continue;
      }
      const auto &indices = ref_it.second;
      DCHECK(!indices.empty());
      const std::size_t ref = indices.front();
      for (std::size_t i = 1; i < indices.size(); ++i) {
        agg_refs[indices[i]].reset(new AggregateReference(ref));
      }
    }
  }

  // Count the number of eliminable aggregate expressions.
  std::size_t num_eliminable = 0;
  for (const auto &agg_ref : agg_refs) {
    if (agg_ref != nullptr) {
      ++num_eliminable;
    }
  }

  if (num_eliminable == 0) {
    return input;
  }

  // Now we need to make a decision since it is not always benefitial to perform
  // the transformation. Currently we employ a simple heuristic that if either
  // (1) The estimated number of groups for this Aggregate node is smaller than
  //     the specified FLAGS_reuse_aggregate_group_size_threshold.
  // or
  // (2) The ratio of (# of eliminable columns) to (# of original columns) is
  //     greater than the specified FLAGS_reuse_aggregate_ratio_threshold.
  // then we perform the transformation.
  const bool is_group_size_condition_satisfied =
      cost_model_->estimateNumGroupsForAggregate(aggregate)
          < FLAGS_reuse_aggregate_group_size_threshold;
  const bool is_ratio_condition_satisfied =
      static_cast<double>(num_eliminable) / agg_exprs.size()
          > FLAGS_reuse_aggregate_ratio_threshold;

  if (!is_group_size_condition_satisfied && !is_ratio_condition_satisfied) {
    return input;
  }

  // Now we transform the original Aggregate to a reduced Aggregate + a wrapping
  // Selection.

  // Aggregate expressions for the new Aggregate.
  std::vector<E::AliasPtr> new_agg_exprs;

  // Project expressions for the new Selection.
  std::vector<E::NamedExpressionPtr> new_select_exprs;

  for (const auto &grouping_expr : aggregate->grouping_expressions()) {
    new_select_exprs.emplace_back(E::ToRef(grouping_expr));
  }

  const std::vector<E::AttributeReferencePtr> agg_attrs = E::ToRefVector(agg_exprs);

  for (std::size_t i = 0; i < agg_refs.size(); ++i) {
    const auto &agg_ref = agg_refs[i];
    const E::AliasPtr &agg_expr = agg_exprs[i];

    if (agg_ref == nullptr) {
      // Case 1: this aggregate expression can not be eliminated.
      new_agg_exprs.emplace_back(agg_expr);
      new_select_exprs.emplace_back(
          E::AttributeReference::Create(agg_expr->id(),
                                        agg_expr->attribute_name(),
                                        agg_expr->attribute_alias(),
                                        agg_expr->relation_name(),
                                        agg_expr->getValueType(),
                                        E::AttributeReferenceScope::kLocal));
    } else {
      switch (agg_ref->kind) {
        // Case 2.1: this aggregate expression can be eliminated.
        case AggregateReference::kDirect: {
          new_select_exprs.emplace_back(
              E::Alias::Create(agg_expr->id(),
                               agg_attrs[agg_ref->first_ref],
                               agg_expr->attribute_name(),
                               agg_expr->attribute_alias(),
                               agg_expr->relation_name()));
          break;
        }
        // Case 2.2: this aggregate expression is an AVG.
        case AggregateReference::kAvg: {
          E::AttributeReferencePtr sum_attr;
          if (agg_ref->first_ref == kInvalidRef) {
            // Case 2.2.1: If there is no existing SUM, we need to convert this
            //             AVG to SUM.
            const E::AggregateFunctionPtr avg_expr =
                std::static_pointer_cast<const E::AggregateFunction>(agg_expr->expression());

            const AggregateFunction &sum_func =
                AggregateFunctionFactory::Get(AggregationID::kSum);
            const E::AggregateFunctionPtr sum_expr =
                E::AggregateFunction::Create(sum_func,
                                             avg_expr->getArguments(),
                                             avg_expr->is_vector_aggregate(),
                                             avg_expr->is_distinct());
            new_agg_exprs.emplace_back(
                E::Alias::Create(optimizer_context_->nextExprId(),
                                 sum_expr,
                                 agg_expr->attribute_name(),
                                 agg_expr->attribute_alias(),
                                 agg_expr->relation_name()));

            sum_attr = E::ToRef(new_agg_exprs.back());
          } else {
            // Case 2.2.2: If there is a SUM somewhere, we just eliminate this
            //             AVG and use the result from that SUM.
            sum_attr = agg_attrs[agg_ref->first_ref];
          }

          // Obtain AVG by evaluating SUM/COUNT in Selection.
          const BinaryOperation &divide_op =
              BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kDivide);
          const E::BinaryExpressionPtr avg_expr =
              E::BinaryExpression::Create(divide_op,
                                          sum_attr,
                                          agg_attrs[agg_ref->second_ref]);
          new_select_exprs.emplace_back(
              E::Alias::Create(agg_expr->id(),
                               avg_expr,
                               agg_expr->attribute_name(),
                               agg_expr->attribute_alias(),
                               agg_expr->relation_name()));
        }
      }
    }
  }

  // Construct the reduced Aggregate.
  const P::AggregatePtr new_aggregate =
      P::Aggregate::Create(aggregate->input(),
                           aggregate->grouping_expressions(),
                           new_agg_exprs,
                           aggregate->filter_predicate());

  // Construct the wrapping Selection.
  return P::Selection::Create(new_aggregate, new_select_exprs, nullptr);
}

}  // namespace optimizer
}  // namespace quickstep
