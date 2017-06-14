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

#include "query_optimizer/rules/InjectJoinFilters.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

#include "query_optimizer/cost_model/StarSchemaSimpleCostModel.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/physical/LIPFilterConfiguration.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/FilterJoin.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/PatternMatcher.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"
#include "query_optimizer/rules/PruneColumns.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "utility/lip_filter/LIPFilter.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace P = ::quickstep::optimizer::physical;

P::PhysicalPtr InjectJoinFilters::apply(const P::PhysicalPtr &input) {
  DCHECK(input->getPhysicalType() == P::PhysicalType::kTopLevelPlan);

  const P::TopLevelPlanPtr top_level_plan =
     std::static_pointer_cast<const P::TopLevelPlan>(input);
  cost_model_.reset(
      new cost::StarSchemaSimpleCostModel(
          top_level_plan->shared_subplans()));
  lip_filter_configuration_.reset(new P::LIPFilterConfiguration());

  // Step 1. Transform applicable HashJoin nodes to FilterJoin nodes.
  P::PhysicalPtr output = transformHashJoinToFilters(input);

  // Step 2. Push down FilterJoin nodes to be evaluated early.
  output = pushDownFilters(output);

  // Step 3. Add Selection nodes for attaching the LIPFilters, if necessary.
  output = addFilterAnchors(output, false);

  // Step 4. Because of the pushdown of FilterJoin nodes, there are optimization
  // opportunities for projecting columns early.
  output = PruneColumns().apply(output);

  // Step 5. For each FilterJoin node, attach its corresponding LIPFilter to
  // proper nodes.
  concretizeAsLIPFilters(output, nullptr);

  if (!lip_filter_configuration_->getBuildInfoMap().empty() ||
      !lip_filter_configuration_->getProbeInfoMap().empty()) {
    output = std::static_pointer_cast<const P::TopLevelPlan>(output)
        ->copyWithLIPFilterConfiguration(
              P::LIPFilterConfigurationPtr(lip_filter_configuration_.release()));
  }

  return output;
}

bool InjectJoinFilters::isTransformable(
    const physical::HashJoinPtr &hash_join) const {
  // Conditions for replacing a HashJoin with a FilterJoin:

  // No residual predicate.
  if (hash_join->residual_predicate() != nullptr) {
    return false;
  }
  // Single attribute equi-join.
  if (hash_join->right_join_attributes().size() > 1) {
    return false;
  }
  // All the output attributes must be from the probe side.
  if (!E::SubsetOfExpressions(hash_join->getOutputAttributes(),
                              hash_join->left()->getOutputAttributes())) {
    return false;
  }
  switch (hash_join->join_type()) {
    case P::HashJoin::JoinType::kInnerJoin: {
      // In the case of inner join, the build side join attributes must be unique.
      if (!cost_model_->impliesUniqueAttributes(hash_join->right(),
                                                hash_join->right_join_attributes())) {
        return false;
      }
      break;
    }
    case P::HashJoin::JoinType::kLeftSemiJoin:  // Fall through
    case P::HashJoin::JoinType::kLeftAntiJoin:
      break;
    default:
      return false;
  }

  // The build side join attribute has integer type and its values are exactly
  // within a reasonable range.
  std::int64_t min_cpp_value;
  std::int64_t max_cpp_value;
  const bool has_exact_min_max_stats =
      findExactMinMaxValuesForAttributeHelper(hash_join->right(),
                                              hash_join->right_join_attributes().front(),
                                              &min_cpp_value,
                                              &max_cpp_value);
  if (!has_exact_min_max_stats) {
    return false;
  }

  // TODO(jianqiao): implement SimpleHashSetExactFilter to relax this requirement.
  // Note that 1G bits = 128MB.
  const std::int64_t value_range = max_cpp_value - min_cpp_value;
  DCHECK_GE(value_range, 0);
  if (value_range > kMaxFilterSize) {
    return false;
  }

  return true;
}

P::PhysicalPtr InjectJoinFilters::transformHashJoinToFilters(
    const P::PhysicalPtr &input) const {
  std::vector<P::PhysicalPtr> new_children;
  bool has_changed_children = false;
  for (const P::PhysicalPtr &child : input->children()) {
    const P::PhysicalPtr new_child = transformHashJoinToFilters(child);
    if (child != new_child && !has_changed_children) {
      has_changed_children = true;
    }
    new_children.push_back(new_child);
  }

  P::HashJoinPtr hash_join;
  if (P::SomeHashJoin::MatchesWithConditionalCast(input, &hash_join) &&
      isTransformable(hash_join)) {
    const bool is_anti_join =
        hash_join->join_type() == P::HashJoin::JoinType::kLeftAntiJoin;

    DCHECK_EQ(2u, new_children.size());
    P::PhysicalPtr build_child = new_children[1];
    E::PredicatePtr build_side_filter_predicate = nullptr;
    P::SelectionPtr selection;
    if (P::SomeSelection::MatchesWithConditionalCast(build_child, &selection) &&
        E::SubsetOfExpressions(hash_join->right_join_attributes(),
                               selection->input()->getOutputAttributes())) {
      build_child = selection->input();
      build_side_filter_predicate = selection->filter_predicate();
    }

    return P::FilterJoin::Create(new_children[0],
                                 build_child,
                                 hash_join->left_join_attributes(),
                                 hash_join->right_join_attributes(),
                                 hash_join->project_expressions(),
                                 build_side_filter_predicate,
                                 is_anti_join,
                                 hash_join->cloneOutputPartitionSchemeHeader());
  }

  if (has_changed_children) {
    return input->copyWithNewChildren(new_children);
  } else {
    return input;
  }
}

physical::PhysicalPtr InjectJoinFilters::pushDownFilters(
    const physical::PhysicalPtr &input) const {
  std::vector<P::PhysicalPtr> new_children;
  bool has_changed_children = false;
  for (const P::PhysicalPtr &child : input->children()) {
    const P::PhysicalPtr new_child = pushDownFilters(child);
    if (child != new_child && !has_changed_children) {
      has_changed_children = true;
    }
    new_children.push_back(new_child);
  }

  P::FilterJoinPtr filter_join;
  if (P::SomeFilterJoin::MatchesWithConditionalCast(input, &filter_join)) {
    DCHECK_EQ(2u, new_children.size());
    return pushDownFiltersInternal(
        new_children[0], new_children[1], filter_join);
  }

  if (has_changed_children) {
    return input->copyWithNewChildren(new_children);
  } else {
    return input;
  }
}

physical::PhysicalPtr InjectJoinFilters::pushDownFiltersInternal(
    const physical::PhysicalPtr &probe_child,
    const physical::PhysicalPtr &build_child,
    const physical::FilterJoinPtr &filter_join) const {
  switch (probe_child->getPhysicalType()) {
    case P::PhysicalType::kAggregate:  // Fall through
    case P::PhysicalType::kHashJoin:
    case P::PhysicalType::kSample:
    case P::PhysicalType::kSelection:
    case P::PhysicalType::kSort:
    case P::PhysicalType::kWindowAggregate: {
      DCHECK_GE(probe_child->getNumChildren(), 1u);
      const P::PhysicalPtr child = probe_child->children()[0];
      if (E::SubsetOfExpressions(filter_join->probe_attributes(),
                                 child->getOutputAttributes())) {
        const P::PhysicalPtr new_child =
            pushDownFiltersInternal(child, build_child, filter_join);
        if (new_child != child) {
          std::vector<P::PhysicalPtr> new_children = probe_child->children();
          new_children[0] = new_child;
          return probe_child->copyWithNewChildren(new_children);
        }
      }
    }
    default:
      break;
  }

  if (probe_child != filter_join->left()) {
    // TODO(jianqiao): may need to update probe_attributes.
    return P::FilterJoin::Create(probe_child,
                                 build_child,
                                 filter_join->probe_attributes(),
                                 filter_join->build_attributes(),
                                 E::ToNamedExpressions(probe_child->getOutputAttributes()),
                                 filter_join->build_side_filter_predicate(),
                                 filter_join->is_anti_join(),
                                 filter_join->cloneOutputPartitionSchemeHeader());
  } else {
    return filter_join;
  }
}


physical::PhysicalPtr InjectJoinFilters::addFilterAnchors(
    const physical::PhysicalPtr &input,
    const bool ancestor_can_anchor_filter) const {
  std::vector<P::PhysicalPtr> new_children;

  switch (input->getPhysicalType()) {
    case P::PhysicalType::kAggregate: {
      const P::AggregatePtr &aggregate =
          std::static_pointer_cast<const P::Aggregate>(input);
      new_children.emplace_back(
          addFilterAnchors(aggregate->input(), true));
      break;
    }
    case P::PhysicalType::kSelection: {
      const P::SelectionPtr &selection =
          std::static_pointer_cast<const P::Selection>(input);
      new_children.emplace_back(
          addFilterAnchors(selection->input(), true));
      break;
    }
    // NOTE(jianqiao): Some of the SSB/TPCH queries slow down significantly if
    // we attach converted filters to parent HashJoin nodes. E.g. one HashJoin +
    // one attached LIPFilter is slower than the original two HashJoins. This is
    // due to some implementation issues with the current HashJoinOperator. So
    // currently we disable the anchoring of filters to HashJoin nodes. That is,
    // in the case that a FilterJoin's parent node (or ancestor node, if there
    // is a chain of FilterJoins) is a HashJoin, we create an extra Selection
    // before the parent HashJoin as anchoring node to attach the filters. This
    // guarantees non-degrading performance.
    /*
    case P::PhysicalType::kHashJoin: {
      const P::HashJoinPtr &hash_join =
          std::static_pointer_cast<const P::HashJoin>(input);
      new_children.emplace_back(
          addFilterAnchors(hash_join->left(), true));
      new_children.emplace_back(
          addFilterAnchors(hash_join->right(), false));
      break;
    }
    */
    case P::PhysicalType::kFilterJoin: {
      const P::FilterJoinPtr &filter_join =
          std::static_pointer_cast<const P::FilterJoin>(input);
      new_children.emplace_back(
          addFilterAnchors(filter_join->left(), true));
      new_children.emplace_back(
          addFilterAnchors(filter_join->right(), true));
      break;
    }
    default: {
      for (const P::PhysicalPtr &child : input->children()) {
        new_children.emplace_back(addFilterAnchors(child, false));
      }
    }
  }

  DCHECK_EQ(new_children.size(), input->children().size());
  const P::PhysicalPtr output_with_new_children =
      new_children == input->children()
          ? input
          : input->copyWithNewChildren(new_children);

  if (input->getPhysicalType() == P::PhysicalType::kFilterJoin &&
      !ancestor_can_anchor_filter) {
    const P::FilterJoinPtr &filter_join =
        std::static_pointer_cast<const P::FilterJoin>(output_with_new_children);
    return P::Selection::Create(filter_join,
                                filter_join->project_expressions(),
                                nullptr,
                                filter_join->cloneOutputPartitionSchemeHeader());
  } else {
    return output_with_new_children;
  }
}

void InjectJoinFilters::concretizeAsLIPFilters(
    const P::PhysicalPtr &input,
    const P::PhysicalPtr &anchor_node) const {
  switch (input->getPhysicalType()) {
    case P::PhysicalType::kAggregate: {
      const P::AggregatePtr &aggregate =
          std::static_pointer_cast<const P::Aggregate>(input);
      concretizeAsLIPFilters(aggregate->input(), aggregate);
      break;
    }
    case P::PhysicalType::kSelection: {
      const P::SelectionPtr &selection =
          std::static_pointer_cast<const P::Selection>(input);
      concretizeAsLIPFilters(selection->input(), selection);
      break;
    }
    // Currently we disable the attachment of filters to HashJoin nodes. See the
    // comments in InjectJoinFilters::addFilterAnchors().
    /*
    case P::PhysicalType::kHashJoin: {
      const P::HashJoinPtr &hash_join =
          std::static_pointer_cast<const P::HashJoin>(input);
      concretizeAsLIPFilters(hash_join->left(), hash_join);
      concretizeAsLIPFilters(hash_join->right(), nullptr);
      break;
    }
    */
    case P::PhysicalType::kFilterJoin: {
      const P::FilterJoinPtr &filter_join =
          std::static_pointer_cast<const P::FilterJoin>(input);
      DCHECK_EQ(1u, filter_join->build_attributes().size());
      const E::AttributeReferencePtr &build_attr =
          filter_join->build_attributes().front();

      std::int64_t min_cpp_value;
      std::int64_t max_cpp_value;
      const bool has_exact_min_max_stats =
          findExactMinMaxValuesForAttributeHelper(filter_join,
                                                  build_attr,
                                                  &min_cpp_value,
                                                  &max_cpp_value);
      DCHECK(has_exact_min_max_stats);
      DCHECK_GE(max_cpp_value, min_cpp_value);
      DCHECK_LE(max_cpp_value - min_cpp_value, kMaxFilterSize);
      CHECK(anchor_node != nullptr);

      lip_filter_configuration_->addBuildInfo(
          P::BitVectorExactFilterBuildInfo::Create(build_attr,
                                                   min_cpp_value,
                                                   max_cpp_value,
                                                   filter_join->is_anti_join()),
          filter_join);
      lip_filter_configuration_->addProbeInfo(
          P::LIPFilterProbeInfo::Create(filter_join->probe_attributes().front(),
                                        build_attr,
                                        filter_join),
          anchor_node);

      concretizeAsLIPFilters(filter_join->left(), anchor_node);
      concretizeAsLIPFilters(filter_join->right(), filter_join);
      break;
    }
    default: {
      for (const P::PhysicalPtr &child : input->children()) {
        concretizeAsLIPFilters(child, nullptr);
      }
    }
  }
}

bool InjectJoinFilters::findExactMinMaxValuesForAttributeHelper(
    const physical::PhysicalPtr &physical_plan,
    const expressions::AttributeReferencePtr &attribute,
    std::int64_t *min_cpp_value,
    std::int64_t *max_cpp_value) const {
  bool min_value_is_exact;
  bool max_value_is_exact;

  const TypedValue min_value =
      cost_model_->findMinValueStat(physical_plan, attribute, &min_value_is_exact);
  const TypedValue max_value =
      cost_model_->findMaxValueStat(physical_plan, attribute, &max_value_is_exact);
  if (min_value.isNull() || max_value.isNull() ||
      (!min_value_is_exact) || (!max_value_is_exact)) {
    return false;
  }

  switch (attribute->getValueType().getTypeID()) {
    case TypeID::kInt: {
      *min_cpp_value = min_value.getLiteral<int>();
      *max_cpp_value = max_value.getLiteral<int>();
      return true;
    }
    case TypeID::kLong: {
      *min_cpp_value = min_value.getLiteral<std::int64_t>();
      *max_cpp_value = max_value.getLiteral<std::int64_t>();
      return true;
    }
    default:
      return false;
  }
}

}  // namespace optimizer
}  // namespace quickstep
