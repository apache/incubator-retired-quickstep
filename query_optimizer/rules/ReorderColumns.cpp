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

#include "query_optimizer/rules/ReorderColumns.hpp"

#include <algorithm>
#include <cstddef>
#include <limits>
#include <unordered_map>
#include <vector>

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
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

P::PhysicalPtr ReorderColumns::apply(const P::PhysicalPtr &input) {
  DCHECK(input->getPhysicalType() == P::PhysicalType::kTopLevelPlan);

  return applyInternal(input, true);
}

P::PhysicalPtr ReorderColumns::applyInternal(const P::PhysicalPtr &input,
                                             const bool lock_ordering) {
  // We have to guarantee that the top level ordering of the columns remain
  // unchanged so that the output columns are ordered as specified by the user.
  // So here we use the flag "lock_ordering" to skip the first transformable
  // node (i.e. the first Selection or HashJoin).
  const bool is_not_transformable = !IsTransformable(input);
  const bool skip_transform = lock_ordering || is_not_transformable;

  if (skip_transform) {
    std::vector<P::PhysicalPtr> new_children;
    for (const P::PhysicalPtr &child : input->children()) {
      bool child_lock_ordering = lock_ordering && is_not_transformable;
      if (child->getPhysicalType() == P::PhysicalType::kUnionAll) {
        child_lock_ordering = true;
      }
      new_children.emplace_back(applyInternal(child, child_lock_ordering));
    }

    if (new_children != input->children()) {
      return input->copyWithNewChildren(new_children);
    } else {
      return input;
    }
  }

  // Collect the maximal chain of transformable nodes.
  std::vector<P::PhysicalPtr> nodes;
  for (P::PhysicalPtr node = input; IsTransformable(node); node = node->children().front()) {
    nodes.emplace_back(node);
  }
  // Arrange the nodes with bottom-up order.
  std::reverse(nodes.begin(), nodes.end());

  // A greedy algorithm that reorders the output attributes based on the GEN/KILL
  // intervals. This algorithm works well with SSB/TPCH queries and is not likely
  // to make the plans worse for whatever queries.
  //
  // Here is a brief explanation of the three data structure base/gen/kill.
  //   (1) base: maps each attribute's id to its position in the BASE relation's
  //             output attributes. Note that the base relation is the child
  //             relation of nodes[0].
  //   (2) gen:  maps each attribute's id to the MINIMUM index i such that the
  //             attribute is among nodes[i]'s output attributes. I.e. node i
  //             GENERATEs the attribute.
  //   (3) kill: maps each attribute's id to the MAXIMUM index i such that the
  //             attribute is among nodes[i]'s output attributes. I.e. node i+1
  //             KILLs the attribute.
  std::unordered_map<E::ExprId, std::size_t> base, gen, kill;

  const P::PhysicalPtr base_node =
      applyInternal(nodes.front()->children().front(), false);
  const std::vector<E::AttributeReferencePtr> base_attrs =
      base_node->getOutputAttributes();
  for (std::size_t i = 0; i < base_attrs.size(); ++i) {
    base.emplace(base_attrs[i]->id(), i);
  }

  for (std::size_t i = 0; i < nodes.size(); ++i) {
    for (const auto &attr : nodes[i]->getOutputAttributes()) {
      const E::ExprId attr_id = attr->id();
      if (gen.find(attr_id) == gen.end()) {
        gen.emplace(attr_id, i);
      }
      kill[attr_id] = i;
    }
  }

  // TODO(jianqiao): implement this comparator as a standalone and well-documented
  // struct.
  const auto comparator = [&gen, &kill, &base](const E::NamedExpressionPtr &lhs,
                                               const E::NamedExpressionPtr &rhs) -> bool {
    const E::ExprId lhs_id = lhs->id();
    const E::ExprId rhs_id = rhs->id();

    // Sort the attributes first by GEN location.
    const std::size_t lhs_gen = gen.at(lhs_id);
    const std::size_t rhs_gen = gen.at(rhs_id);
    if (lhs_gen != rhs_gen) {
      return lhs_gen < rhs_gen;
    }

    // Then by KILL location.
    const std::size_t lhs_kill = kill.at(lhs_id);
    const std::size_t rhs_kill = kill.at(rhs_id);
    if (lhs_kill != rhs_kill) {
      return lhs_kill < rhs_kill;
    }

    // Finally by the ordering in the base relaton.
    const auto lhs_base_it = base.find(lhs_id);
    const auto rhs_base_it = base.find(rhs_id);
    const std::size_t lhs_base =
        lhs_base_it == base.end() ? std::numeric_limits<std::size_t>::max()
                                  : lhs_base_it->second;
    const std::size_t rhs_base =
        rhs_base_it == base.end() ? std::numeric_limits<std::size_t>::max()
                                  : rhs_base_it->second;
    if (lhs_base != rhs_base) {
      return lhs_base < rhs_base;
    }

    return lhs_id < rhs_id;
  };

  P::PhysicalPtr output = base_node;

  for (const auto &node : nodes) {
    std::vector<E::NamedExpressionPtr> project_expressions;
    switch (node->getPhysicalType()) {
      case P::PhysicalType::kHashJoin: {
        project_expressions =
            std::static_pointer_cast<const P::HashJoin>(node)->project_expressions();
        break;
      }
      case P::PhysicalType::kSelection: {
        project_expressions =
            std::static_pointer_cast<const P::Selection>(node)->project_expressions();
        break;
      }
      default:
        LOG(FATAL) << "Unsupported physical type";
    }

    std::sort(project_expressions.begin(), project_expressions.end(), comparator);

    switch (node->getPhysicalType()) {
      case P::PhysicalType::kHashJoin: {
        const P::HashJoinPtr old_node =
            std::static_pointer_cast<const P::HashJoin>(node);
        output = P::HashJoin::Create(output,
                                     applyInternal(old_node->right(), false),
                                     old_node->left_join_attributes(),
                                     old_node->right_join_attributes(),
                                     old_node->residual_predicate(),
                                     project_expressions,
                                     old_node->join_type());
        break;
      }
      case P::PhysicalType::kSelection: {
        const P::SelectionPtr old_node =
            std::static_pointer_cast<const P::Selection>(node);
        output = P::Selection::Create(output,
                                      project_expressions,
                                      old_node->filter_predicate(),
                                      output->cloneOutputPartitionSchemeHeader());
        break;
      }
      default:
        LOG(FATAL) << "Unsupported physical type";
    }
  }

  return output;
}

bool ReorderColumns::IsTransformable(const physical::PhysicalPtr &input) {
  switch (input->getPhysicalType()) {
    case P::PhysicalType::kHashJoin:  // Fall through
    case P::PhysicalType::kSelection:
      return true;
    default:
      return false;
  }
}

}  // namespace optimizer
}  // namespace quickstep
