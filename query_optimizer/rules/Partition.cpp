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
#include <unordered_set>
#include <utility>
#include <vector>

#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/cost_model/StarSchemaSimpleCostModel.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/PartitionSchemeHeader.hpp"
#include "query_optimizer/physical/PatternMatcher.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"
#include "utility/Cast.hpp"
#include "utility/EqualsAnyConstant.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

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

P::PhysicalPtr Repartition(const P::PhysicalPtr &node, const vector<E::AttributeReferencePtr> &join_attributes,
                           const size_t num_partitions) {
  P::PartitionSchemeHeader::PartitionExprIds repartition_expr_ids;
  for (const E::AttributeReferencePtr &attr : join_attributes) {
    repartition_expr_ids.push_back({ attr->id() });
  }
  auto repartition_scheme_header = make_unique<P::PartitionSchemeHeader>(
      P::PartitionSchemeHeader::PartitionType::kHash, num_partitions, move(repartition_expr_ids));

  if (needsSelection(node->getPhysicalType())) {
    // Add a Selection node.
    return P::Selection::Create(node,
                                CastSharedPtrVector<E::NamedExpression>(node->getOutputAttributes()),
                                nullptr /* filter_predicate */, repartition_scheme_header.release());
  } else {
    // Overwrite the output partition scheme header of the node.
    return node->copyWithNewOutputPartitionSchemeHeader(repartition_scheme_header.release());
  }
}

}  // namespace

P::PhysicalPtr Partition::applyToNode(const P::PhysicalPtr &node) {
  // Will be used for aggregations.
  (void) optimizer_context_;

  switch (node->getPhysicalType()) {
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
      // Repartition.
      if (left_needs_repartition) {
        left = Repartition(left, left_join_attributes, num_partitions);
      }

      if (right_needs_repartition) {
        right = Repartition(right, right_join_attributes, num_partitions);
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
    case P::PhysicalType::kSelection: {
      const P::SelectionPtr selection = static_pointer_cast<const P::Selection>(node);

      const P::PartitionSchemeHeader *input_partition_scheme_header =
          selection->input()->getOutputPartitionSchemeHeader();
      if (input_partition_scheme_header && input_partition_scheme_header->isHashPartition()) {
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
