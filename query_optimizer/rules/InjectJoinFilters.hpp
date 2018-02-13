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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_INJECT_JOIN_FILTERS_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_INJECT_JOIN_FILTERS_HPP_

#include <cstdint>
#include <memory>
#include <string>

#include "query_optimizer/cost_model/StarSchemaSimpleCostModel.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/physical/LIPFilterConfiguration.hpp"
#include "query_optimizer/physical/FilterJoin.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/rules/Rule.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

/** \addtogroup OptimizerRules
 *  @{
 */

/**
 * @brief Rule that applies to a physical plan to transform HashJoin nodes into
 *        FilterJoin nodes.
 * 
 * This is an optimization that strength-reduces HashJoins to FilterJoins
 * (implemented as LIPFilters attached to some anchoring operators where the
 * filters get applied). Briefly speaking, the idea is that in the case that
 * (1) the join attribute has consecutive integer values bounded in a reasonably
 * small range AND (2) the output attributes are all from the probe-side table,
 * we can eliminate the HashJoin by building a BitVector on the build-side
 * attribute and using the BitVector to filter the probe-side table.
 */
class InjectJoinFilters : public Rule<physical::Physical> {
 public:
  /**
   * @brief Constructor.
   */
  InjectJoinFilters() {}

  ~InjectJoinFilters() override {}

  std::string getName() const override {
    return "TransformFilterJoins";
  }

  physical::PhysicalPtr apply(const physical::PhysicalPtr &input) override;

 private:
  // Check whether a HashJoin can be transformed into a FilterJoin.
  bool isTransformable(const physical::HashJoinPtr &hash_join) const;

  // Transform applicable HashJoin nodes into FilterJoin nodes.
  physical::PhysicalPtr transformHashJoinToFilters(
      const physical::PhysicalPtr &input) const;

  // Push down FilterJoin nodes to be evaluated early.
  physical::PhysicalPtr pushDownFilters(const physical::PhysicalPtr &input) const;

  // Add Selection node, if necessary, for anchoring the LIP filters built by
  // FilterJoin nodes.
  physical::PhysicalPtr addFilterAnchors(const physical::PhysicalPtr &input,
                                         const bool ancestor_can_anchor_filter) const;

  // Setup lip_filter_configuration_ with the transformed plan tree.
  void concretizeAsLIPFilters(const physical::PhysicalPtr &input,
                              const physical::PhysicalPtr &anchor_node) const;

  physical::PhysicalPtr pushDownFiltersInternal(
      const physical::PhysicalPtr &probe_child,
      const physical::PhysicalPtr &build_child,
      const physical::FilterJoinPtr &filter_join) const;

  bool findExactMinMaxValuesForAttributeHelper(
      const physical::PhysicalPtr &physical_plan,
      const expressions::AttributeReferencePtr &attribute,
      std::int64_t *min_cpp_value,
      std::int64_t *max_cpp_value) const;

  physical::PhysicalPtr wrapSelection(const physical::PhysicalPtr &input) const;

  std::unique_ptr<cost::StarSchemaSimpleCostModel> cost_model_;
  std::unique_ptr<physical::LIPFilterConfiguration> lip_filter_configuration_;

  // TODO(jianqiao): Add this threshold as a gflag.
  // Note that 1G bits = 128MB
  static constexpr std::int64_t kMaxFilterSize = 1000000000L;

  DISALLOW_COPY_AND_ASSIGN(InjectJoinFilters);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_RULES_INJECT_JOIN_FILTERS_HPP_
