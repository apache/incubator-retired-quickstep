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

#include "query_optimizer/rules/Partition.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>

#include "expressions/aggregation/AggregateFunction.hpp"
#include "expressions/aggregation/AggregateFunctionFactory.hpp"
#include "expressions/aggregation/AggregationID.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/cost_model/StarSchemaSimpleCostModel.hpp"
#include "query_optimizer/expressions/AggregateFunction.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/BinaryExpression.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/PartitionSchemeHeader.hpp"
#include "query_optimizer/physical/PatternMatcher.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "utility/Cast.hpp"
#include "utility/EqualsAnyConstant.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

using std::get;
using std::make_unique;
using std::move;
using std::size_t;
using std::static_pointer_cast;
using std::unordered_set;
using std::vector;

namespace quickstep {
namespace optimizer {

namespace E = expressions;
namespace P = physical;

static bool ValidateNumRepartitions(const char *flagname, std::uint64_t value) {
  return value > 1u;
}
DEFINE_uint64(num_repartitions, 4, "Number of repartitions for a join.");
static const volatile bool num_repartitions_dummy
    = gflags::RegisterFlagValidator(&FLAGS_num_repartitions, &ValidateNumRepartitions);

void Partition::init(const P::PhysicalPtr &input) {
  P::TopLevelPlanPtr top_level_plan;
  CHECK(P::SomeTopLevelPlan::MatchesWithConditionalCast(input, &top_level_plan));

  cost_model_ = make_unique<cost::StarSchemaSimpleCostModel>(top_level_plan->shared_subplans());
}

namespace {

bool needsSelection(const P::PhysicalType physical_type) {
  return QUICKSTEP_EQUALS_ANY_CONSTANT(physical_type,
                                       P::PhysicalType::kSharedSubplanReference,
                                       P::PhysicalType::kSort,
                                       P::PhysicalType::kTableReference,
                                       P::PhysicalType::kUnionAll);
}

P::PhysicalPtr Repartition(const P::PhysicalPtr &node, P::PartitionSchemeHeader *repartition_scheme_header) {
  if (needsSelection(node->getPhysicalType())) {
    // Add a Selection node.
    return P::Selection::Create(node,
                                CastSharedPtrVector<E::NamedExpression>(node->getOutputAttributes()),
                                nullptr /* filter_predicate */, repartition_scheme_header);
  } else {
    // Overwrite the output partition scheme header of the node.
    return node->copyWithNewOutputPartitionSchemeHeader(repartition_scheme_header);
  }
}

P::PhysicalPtr HashRepartition(const P::PhysicalPtr &node,
                               const vector<E::AttributeReferencePtr> &repartition_attributes,
                               const size_t num_repartitions) {
  P::PartitionSchemeHeader::PartitionExprIds repartition_expr_ids;
  for (const E::AttributeReferencePtr &attr : repartition_attributes) {
    repartition_expr_ids.push_back({ attr->id() });
  }
  auto repartition_scheme_header = make_unique<P::PartitionSchemeHeader>(
      P::PartitionSchemeHeader::PartitionType::kHash, num_repartitions, move(repartition_expr_ids));

  return Repartition(node, repartition_scheme_header.release());
}

E::AliasPtr GetReaggregateExpression(const E::AliasPtr &aggr_alias) {
  E::ExpressionPtr aggr = aggr_alias->expression();

  E::AggregateFunctionPtr aggr_fn;
  CHECK(E::SomeAggregateFunction::MatchesWithConditionalCast(aggr, &aggr_fn))
      << aggr->toString();

  AggregationID reaggr_id;
  switch (aggr_fn->getAggregate().getAggregationID()) {
    case AggregationID::kCount:
    case AggregationID::kSum: {
      reaggr_id = AggregationID::kSum;
      break;
    }
    case AggregationID::kMax: {
      reaggr_id = AggregationID::kMax;
      break;
    }
    case AggregationID::kMin: {
      reaggr_id = AggregationID::kMin;
      break;
    }
    default:
      LOG(FATAL) << "Unsupported aggregation id for re-aggregate";
  }

  const E::AggregateFunctionPtr reaggr =
      E::AggregateFunction::Create(AggregateFunctionFactory::Get(reaggr_id),
                                   { E::ToRef(aggr_alias) },
                                   aggr_fn->is_vector_aggregate(),
                                   aggr_fn->is_distinct());

  return E::Alias::Create(aggr_alias->id(),
                          reaggr,
                          aggr_alias->attribute_name(),
                          aggr_alias->attribute_alias(),
                          aggr_alias->relation_name());
}

P::PhysicalPtr applyToNestedLoopsJoin(const P::NestedLoopsJoinPtr &nested_loops_join,
                                      P::PhysicalPtr left, P::PhysicalPtr right) {
  const P::PartitionSchemeHeader *left_partition_scheme_header =
      left->getOutputPartitionSchemeHeader();
  const std::size_t num_partitions =
      left_partition_scheme_header ? left_partition_scheme_header->num_partitions : FLAGS_num_repartitions;
  auto left_repartition_scheme_header = make_unique<P::PartitionSchemeHeader>(
      P::PartitionSchemeHeader::PartitionType::kRandom, num_partitions, P::PartitionSchemeHeader::PartitionExprIds());

  switch (left->getPhysicalType()) {
    case P::PhysicalType::kTableReference:
      if (left_partition_scheme_header) {
        break;
      }
      // Fall through.
    case P::PhysicalType::kSharedSubplanReference:
    case P::PhysicalType::kSort:
    case P::PhysicalType::kUnionAll:
      // Add a Selection node.
      left = P::Selection::Create(left,
                                  CastSharedPtrVector<E::NamedExpression>(left->getOutputAttributes()),
                                  nullptr /* filter_predicate */, left_repartition_scheme_header.release());
      break;
    default: {
      if (!left_partition_scheme_header) {
        left = left->copyWithNewOutputPartitionSchemeHeader(left_repartition_scheme_header.release());
      }
    }
  }

  const P::PartitionSchemeHeader *right_partition_scheme_header =
      right->getOutputPartitionSchemeHeader();
  if (right_partition_scheme_header && right_partition_scheme_header->num_partitions != num_partitions) {
    auto right_repartition_scheme_header = make_unique<P::PartitionSchemeHeader>(
        right_partition_scheme_header->partition_type, num_partitions,
        P::PartitionSchemeHeader::PartitionExprIds(right_partition_scheme_header->partition_expr_ids));
    if (needsSelection(right->getPhysicalType())) {
      // Add a Selection node.
      right = P::Selection::Create(right,
                                   CastSharedPtrVector<E::NamedExpression>(right->getOutputAttributes()),
                                   nullptr /* filter_predicate */, right_repartition_scheme_header.release());
    } else {
      right = right->copyWithNewOutputPartitionSchemeHeader(right_repartition_scheme_header.release());
    }
  }

  unordered_set<E::ExprId> project_expr_ids;
  for (const E::AttributeReferencePtr &project_expression : nested_loops_join->getOutputAttributes()) {
    project_expr_ids.insert(project_expression->id());
  }

  left_partition_scheme_header = left->getOutputPartitionSchemeHeader();
  const auto &left_partition_expr_ids = left_partition_scheme_header->partition_expr_ids;
  P::PartitionSchemeHeader::PartitionExprIds output_partition_expr_ids;
  for (const auto &equivalent_expr_ids : left_partition_expr_ids) {
    P::PartitionSchemeHeader::EquivalentPartitionExprIds output_equivalent_partition_expr_ids;
    for (const E::ExprId expr_id : equivalent_expr_ids) {
      if (project_expr_ids.find(expr_id) != project_expr_ids.end()) {
        output_equivalent_partition_expr_ids.insert(expr_id);
      }
    }

    if (!output_equivalent_partition_expr_ids.empty()) {
      output_partition_expr_ids.push_back(move(output_equivalent_partition_expr_ids));
    }
  }

  std::unique_ptr<P::PartitionSchemeHeader> output_partition_scheme_header;
  if (left_partition_expr_ids != output_partition_expr_ids) {
    output_partition_scheme_header = make_unique<P::PartitionSchemeHeader>(
        left_partition_scheme_header->partition_type, num_partitions, move(output_partition_expr_ids));
  } else {
    output_partition_scheme_header = make_unique<P::PartitionSchemeHeader>(*left_partition_scheme_header);
  }

  return P::NestedLoopsJoin::Create(left, right,
                                    nested_loops_join->join_predicate(),
                                    nested_loops_join->project_expressions(),
                                    output_partition_scheme_header.release());
}

}  // namespace

P::PhysicalPtr Partition::applyToNode(const P::PhysicalPtr &node) {
  switch (node->getPhysicalType()) {
    case P::PhysicalType::kAggregate: {
      const P::AggregatePtr aggregate = static_pointer_cast<const P::Aggregate>(node);

      const P::PhysicalPtr input = aggregate->input();
      const P::PartitionSchemeHeader *input_partition_scheme_header =
          input->getOutputPartitionSchemeHeader();

      if (!input_partition_scheme_header) {
        break;
      }

      std::unique_ptr<P::PartitionSchemeHeader> output_partition_scheme_header;
      const vector<E::NamedExpressionPtr> &grouping_expressions = aggregate->grouping_expressions();

      unordered_set<E::ExprId> grouping_expr_ids;
      for (const E::NamedExpressionPtr &grouping_expression : grouping_expressions) {
        grouping_expr_ids.insert(grouping_expression->id());
      }

      if (!grouping_expressions.empty()) {
        if (input_partition_scheme_header->reusablePartitionScheme(grouping_expr_ids)) {
          // We do not need to reaggregate iff the list of partition attributes is
          // a subset of the group by list.
          P::PartitionSchemeHeader::PartitionExprIds output_partition_expr_ids;
          for (const E::ExprId grouping_expr_id : grouping_expr_ids) {
            output_partition_expr_ids.push_back({ grouping_expr_id });
          }

          output_partition_scheme_header = make_unique<P::PartitionSchemeHeader>(
              P::PartitionSchemeHeader::PartitionType::kHash,
              input_partition_scheme_header->num_partitions,
              move(output_partition_expr_ids));

          return aggregate->copyWithNewOutputPartitionSchemeHeader(output_partition_scheme_header.release());
        }
      }

      const vector<E::AliasPtr> &aggregate_expressions = aggregate->aggregate_expressions();
      const E::PredicatePtr &filter_predicate = aggregate->filter_predicate();

      vector<E::AliasPtr> partial_aggregate_expressions;
      vector<E::AttributeReferencePtr> non_recompute_aggregate_expressions;
      // tuple<Avg, Sum, Count>.
      vector<std::tuple<E::AliasPtr, E::AttributeReferencePtr, E::AttributeReferencePtr>> avg_recompute_expressions;
      for (const E::AliasPtr &aggregate_expression : aggregate_expressions) {
        E::AggregateFunctionPtr aggr_func;
        CHECK(E::SomeAggregateFunction::MatchesWithConditionalCast(aggregate_expression->expression(), &aggr_func));

        bool uses_partial_aggregate = false;
        if (aggr_func->is_distinct()) {
          const vector<E::AttributeReferencePtr> distinct_referenced_attributes =
              aggr_func->getReferencedAttributes();
          DCHECK_EQ(1u, distinct_referenced_attributes.size());

          if (grouping_expr_ids.find(distinct_referenced_attributes.front()->id()) == grouping_expr_ids.end()) {
            // Create a new aggregate whose input has no partitions.
            return P::Aggregate::Create(Repartition(input, nullptr), grouping_expressions, aggregate_expressions,
                                        filter_predicate);
          }

          uses_partial_aggregate = true;
        } else if (aggr_func->getAggregate().getAggregationID() != AggregationID::kAvg) {
          uses_partial_aggregate = true;
        }

        if (uses_partial_aggregate) {
          partial_aggregate_expressions.push_back(aggregate_expression);
          non_recompute_aggregate_expressions.push_back(E::ToRef(aggregate_expression));

          continue;
        }

        DCHECK(aggr_func->getAggregate().getAggregationID() == AggregationID::kAvg);
        const auto &arguments = aggr_func->getArguments();
        DCHECK_EQ(1u, arguments.size());

        // Sum
        const E::AggregateFunctionPtr sum_expr =
            E::AggregateFunction::Create(AggregateFunctionFactory::Get(AggregationID::kSum),
                                         arguments,
                                         aggr_func->is_vector_aggregate(),
                                         aggr_func->is_distinct());
        partial_aggregate_expressions.push_back(
            E::Alias::Create(optimizer_context_->nextExprId(),
                             sum_expr,
                             aggregate_expression->attribute_name(),
                             aggregate_expression->attribute_alias(),
                             aggregate_expression->relation_name()));
        const E::AttributeReferencePtr sum_attr = E::ToRef(partial_aggregate_expressions.back());

        // Count
        const E::AggregateFunctionPtr count_expr =
            E::AggregateFunction::Create(AggregateFunctionFactory::Get(AggregationID::kCount),
                                         arguments,
                                         aggr_func->is_vector_aggregate(),
                                         aggr_func->is_distinct());
        partial_aggregate_expressions.push_back(
            E::Alias::Create(optimizer_context_->nextExprId(),
                             count_expr,
                             aggregate_expression->attribute_name(),
                             aggregate_expression->attribute_alias(),
                             aggregate_expression->relation_name()));
        avg_recompute_expressions.emplace_back(aggregate_expression, sum_attr,
                                               E::ToRef(partial_aggregate_expressions.back()));
      }

      if (!grouping_expressions.empty()) {
        P::PartitionSchemeHeader::PartitionExprIds output_partition_expr_ids;
        for (const E::NamedExpressionPtr &grouping_expression : grouping_expressions) {
          output_partition_expr_ids.push_back({ grouping_expression->id() });
        }
        output_partition_scheme_header = make_unique<P::PartitionSchemeHeader>(
            P::PartitionSchemeHeader::PartitionType::kHash,
            input_partition_scheme_header->num_partitions,
            move(output_partition_expr_ids));
      }
      const P::PhysicalPtr partial_aggregate =
          avg_recompute_expressions.empty()
              ? aggregate->copyWithNewOutputPartitionSchemeHeader(output_partition_scheme_header.release())
              : P::Aggregate::Create(input, grouping_expressions, partial_aggregate_expressions,
                                     filter_predicate, output_partition_scheme_header.release());

      vector<E::AliasPtr> reaggregate_expressions;
      for (const auto &aggregate_expr : partial_aggregate_expressions) {
        reaggregate_expressions.push_back(GetReaggregateExpression(aggregate_expr));
      }

      if (!grouping_expressions.empty()) {
        P::PartitionSchemeHeader::PartitionExprIds output_partition_expr_ids;
        for (const E::NamedExpressionPtr &grouping_expression : grouping_expressions) {
          output_partition_expr_ids.push_back({ grouping_expression->id() });
        }
        output_partition_scheme_header = make_unique<P::PartitionSchemeHeader>(
            P::PartitionSchemeHeader::PartitionType::kHash,
            input_partition_scheme_header->num_partitions,
            move(output_partition_expr_ids));
      }
      const P::AggregatePtr reaggregate =
          P::Aggregate::Create(partial_aggregate, grouping_expressions, reaggregate_expressions,
                               nullptr /* filter_predicate */, output_partition_scheme_header.release());

      if (avg_recompute_expressions.empty()) {
        return reaggregate;
      }

      vector<E::NamedExpressionPtr> project_expressions;
      for (const auto &grouping_expr : grouping_expressions) {
        project_expressions.emplace_back(E::ToRef(grouping_expr));
      }

      for (const E::AttributeReferencePtr &non_recompute_aggregate_expression : non_recompute_aggregate_expressions) {
        project_expressions.emplace_back(non_recompute_aggregate_expression);
      }
      for (const auto &avg_recompute_expression : avg_recompute_expressions) {
        const auto &avg_expr = get<0>(avg_recompute_expression);
        // Obtain AVG by evaluating SUM/COUNT in Selection.
        const BinaryOperation &divide_op =
            BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kDivide);
        const E::BinaryExpressionPtr new_avg_expr =
            E::BinaryExpression::Create(divide_op,
                                        get<1>(avg_recompute_expression),
                                        get<2>(avg_recompute_expression));
        project_expressions.emplace_back(
            E::Alias::Create(avg_expr->id(),
                             new_avg_expr,
                             avg_expr->attribute_name(),
                             avg_expr->attribute_alias(),
                             avg_expr->relation_name()));
      }

      if (!grouping_expressions.empty()) {
        output_partition_scheme_header =
            make_unique<P::PartitionSchemeHeader>(*reaggregate->getOutputPartitionSchemeHeader());
      }
      return P::Selection::Create(reaggregate, project_expressions, nullptr /* filter_predicate */,
                                  output_partition_scheme_header.release());
    }
    case P::PhysicalType::kHashJoin: {
      const P::HashJoinPtr hash_join = static_pointer_cast<const P::HashJoin>(node);

      P::PhysicalPtr left = hash_join->left();
      const P::PartitionSchemeHeader *left_partition_scheme_header =
          left->getOutputPartitionSchemeHeader();

      P::PhysicalPtr right = hash_join->right();
      const P::PartitionSchemeHeader *right_partition_scheme_header =
          right->getOutputPartitionSchemeHeader();

      if (!left_partition_scheme_header && !right_partition_scheme_header) {
        break;
      }

      const auto &left_join_attributes = hash_join->left_join_attributes();
      const auto &right_join_attributes = hash_join->right_join_attributes();

      bool left_needs_repartition = false;
      bool right_needs_repartition = false;
      size_t num_partitions = 1u;

      needsRepartitionForHashJoin(left_partition_scheme_header, left_join_attributes,
                                  right_partition_scheme_header, right_join_attributes,
                                  &left_needs_repartition, &right_needs_repartition, &num_partitions);
      // Hash repartition.
      if (left_needs_repartition) {
        left = HashRepartition(left, left_join_attributes, num_partitions);
      }

      if (right_needs_repartition) {
        right = HashRepartition(right, right_join_attributes, num_partitions);
      }

      unordered_set<E::ExprId> project_expr_ids;
      for (const E::AttributeReferencePtr &project_expression : hash_join->getOutputAttributes()) {
        project_expr_ids.insert(project_expression->id());
      }

      P::PartitionSchemeHeader::PartitionExprIds output_repartition_expr_ids;
      for (size_t i = 0; i < left_join_attributes.size(); ++i) {
        const E::ExprId left_join_id = left_join_attributes[i]->id();
        const E::ExprId right_join_id = right_join_attributes[i]->id();

        output_repartition_expr_ids.emplace_back();

        if (project_expr_ids.count(left_join_id)) {
          output_repartition_expr_ids.back().insert(left_join_id);
        }

        if (project_expr_ids.count(right_join_id)) {
          output_repartition_expr_ids.back().insert(right_join_id);
        }

        if (output_repartition_expr_ids.back().empty()) {
          // Some partition attribute will be projected out, so we use
          // the input partition id as the output partition id.
          output_repartition_expr_ids.clear();
          break;
        }
      }
      auto output_partition_scheme_header = make_unique<P::PartitionSchemeHeader>(
          P::PartitionSchemeHeader::PartitionType::kHash, num_partitions, move(output_repartition_expr_ids));
      if (left_needs_repartition || right_needs_repartition) {
        return P::HashJoin::Create(left, right, left_join_attributes, right_join_attributes,
                                   hash_join->residual_predicate(),
                                   hash_join->project_expressions(),
                                   hash_join->join_type(),
                                   output_partition_scheme_header.release());
      } else if (left_partition_scheme_header) {
        return hash_join->copyWithNewOutputPartitionSchemeHeader(output_partition_scheme_header.release());
      }
      break;
    }
    case P::PhysicalType::kNestedLoopsJoin: {
      const P::NestedLoopsJoinPtr nested_loops_join = static_pointer_cast<const P::NestedLoopsJoin>(node);

      P::PhysicalPtr left = nested_loops_join->left();
      P::PhysicalPtr right = nested_loops_join->right();
      if (!left->getOutputPartitionSchemeHeader() && !right->getOutputPartitionSchemeHeader()) {
        break;
      }

      // Left (larger) side becames RandomPartition, and the right (smaller) side for broadcast join.
      if (cost_model_->estimateCardinality(left) > cost_model_->estimateCardinality(right)) {
        return applyToNestedLoopsJoin(nested_loops_join, left, right);
      } else {
        return applyToNestedLoopsJoin(nested_loops_join, right, left);
      }
    }
    case P::PhysicalType::kSelection: {
      const P::SelectionPtr selection = static_pointer_cast<const P::Selection>(node);

      const P::PartitionSchemeHeader *input_partition_scheme_header =
          selection->input()->getOutputPartitionSchemeHeader();
      if (!input_partition_scheme_header) {
        break;
      }

      if (input_partition_scheme_header->isHashPartition()) {
        unordered_set<E::ExprId> project_expr_ids;
        for (const E::AttributeReferencePtr &project_expression : selection->getOutputAttributes()) {
          project_expr_ids.insert(project_expression->id());
        }

        const auto &input_partition_expr_ids = input_partition_scheme_header->partition_expr_ids;
        P::PartitionSchemeHeader::PartitionExprIds output_partition_expr_ids;
        for (const auto &equivalent_expr_ids : input_partition_expr_ids) {
          P::PartitionSchemeHeader::EquivalentPartitionExprIds output_equivalent_partition_expr_ids;
          for (const E::ExprId expr_id : equivalent_expr_ids) {
            if (project_expr_ids.find(expr_id) != project_expr_ids.end()) {
              output_equivalent_partition_expr_ids.insert(expr_id);
            }
          }

          if (!output_equivalent_partition_expr_ids.empty()) {
            output_partition_expr_ids.push_back(move(output_equivalent_partition_expr_ids));
          }
        }

        if (input_partition_expr_ids != output_partition_expr_ids) {
          auto output_partition_scheme_header = make_unique<P::PartitionSchemeHeader>(
              P::PartitionSchemeHeader::PartitionType::kHash,
              input_partition_scheme_header->num_partitions,
              move(output_partition_expr_ids));
          return selection->copyWithNewOutputPartitionSchemeHeader(output_partition_scheme_header.release());
        }
      }
      break;
    }
    default:
      break;
  }
  return node;
}

void Partition::needsRepartitionForHashJoin(
    const P::PartitionSchemeHeader *left_partition_scheme_header,
    const vector<E::AttributeReferencePtr> &left_join_attributes,
    const P::PartitionSchemeHeader *right_partition_scheme_header,
    const vector<E::AttributeReferencePtr> &right_join_attributes,
    bool *left_needs_repartition,
    bool *right_needs_repartition,
    size_t *num_partitions) {
  DCHECK(left_partition_scheme_header || right_partition_scheme_header);

  *left_needs_repartition = false;
  *num_partitions = 1u;

  if (left_partition_scheme_header) {
    *num_partitions = left_partition_scheme_header->num_partitions;

    // Need to repartition unless the partition attributes are as the same as
    // the join attributes.
    *left_needs_repartition = true;
    if (left_partition_scheme_header->isHashPartition()) {
      unordered_set<E::ExprId> left_join_expr_ids;
      for (const E::AttributeReferencePtr &attr : left_join_attributes) {
        left_join_expr_ids.insert(attr->id());
      }

      if (left_partition_scheme_header->reusablePartitionScheme(left_join_expr_ids)) {
        *left_needs_repartition = false;
        *num_partitions = left_partition_scheme_header->num_partitions;
      }
    }
  } else if (right_partition_scheme_header) {
    *left_needs_repartition = true;
    *num_partitions = right_partition_scheme_header->num_partitions;
  }

  *right_needs_repartition = false;
  if (right_partition_scheme_header) {
    // Need to repartition unless the partition attributes are as the same as
    // the join attributes.
    *right_needs_repartition = true;
    if (right_partition_scheme_header->isHashPartition()) {
      unordered_set<E::ExprId> right_join_expr_ids;
      for (const E::AttributeReferencePtr &attr : right_join_attributes) {
        right_join_expr_ids.insert(attr->id());
      }

      if (right_partition_scheme_header->reusablePartitionScheme(right_join_expr_ids) &&
          (*left_needs_repartition || *num_partitions == right_partition_scheme_header->num_partitions)) {
        *right_needs_repartition = false;
        *num_partitions = right_partition_scheme_header->num_partitions;
      }
    }
  } else if (*left_needs_repartition) {
    // TODO(quickstep-team): use a cost model to choose the broadcast hash join
    // or repartitioned hash join if the right side has no partitions while the
    // left side needs to repartition. For now, we always use the latter.
    *right_needs_repartition = true;
  }

  if (*right_needs_repartition && *left_needs_repartition) {
    *num_partitions = FLAGS_num_repartitions;
  }

  DCHECK_NE(1u, *num_partitions);
}

}  // namespace optimizer
}  // namespace quickstep
