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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_STAR_SCHEMA_HASH_JOIN_ORDER_OPTIMIZATION_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_STAR_SCHEMA_HASH_JOIN_ORDER_OPTIMIZATION_HPP_

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "query_optimizer/cost_model/StarSchemaSimpleCostModel.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/rules/Rule.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

/** \addtogroup OptimizerRules
 *  @{
 */

/**
 * @brief Rule that applies to a physical plan to optimize hash join orders.
 *
 * This optimization applies a greedy algorithm to favor smaller cardinality
 * and selective tables to be joined first, which is suitable for queries on
 * star-schema or snowflake-schema tables.
 */
class StarSchemaHashJoinOrderOptimization : public Rule<physical::Physical> {
 public:
  StarSchemaHashJoinOrderOptimization() {}

  ~StarSchemaHashJoinOrderOptimization() override {}

  std::string getName() const override {
    return "StarSchemaHashJoinOrderOptimization";
  }

  physical::PhysicalPtr apply(const physical::PhysicalPtr &input) override;

 private:
  /**
   * @brief A group of tables to form a hash join tree.
   */
  struct JoinGroupInfo {
    std::unordered_set<expressions::ExprId> referenced_attributes;
    std::vector<physical::PhysicalPtr> tables;
    std::vector<std::pair<expressions::ExprId, expressions::ExprId>> join_attribute_pairs;
  };

  /**
   * @brief Auxiliary information of a table for the optimizer.
   */
  struct TableInfo {
    TableInfo(const std::size_t table_info_id_in,
              const physical::PhysicalPtr &table_in,
              const std::size_t estimated_cardinality_in,
              const double estimated_selectivity_in,
              const std::size_t estimated_num_output_attributes_in,
              const bool is_aggregation_in)
        : table_info_id(table_info_id_in),
          table(table_in),
          estimated_cardinality(estimated_cardinality_in),
          estimated_selectivity(estimated_selectivity_in),
          estimated_num_output_attributes(estimated_num_output_attributes_in) {
    }

    const std::size_t table_info_id;
    physical::PhysicalPtr table;
    std::size_t estimated_cardinality;
    double estimated_selectivity;
    std::size_t estimated_num_output_attributes;
  };

  struct JoinPair {
    JoinPair(TableInfo *probe_in,
             TableInfo *build_in,
             const bool build_side_unique_in,
             const std::size_t num_join_attributes_in)
        : probe(probe_in),
          build(build_in),
          build_side_unique(build_side_unique_in),
          num_join_attributes(num_join_attributes_in) {
    }

    inline bool isBetterThan(const JoinPair &rhs) const {
      const auto &lhs = *this;
      const TableInfo &lhs_probe = *(lhs.probe);
      const TableInfo &lhs_build = *(lhs.build);

      const TableInfo &rhs_probe = *(rhs.probe);
      const TableInfo &rhs_build = *(rhs.build);

      // Avoid carrying too many output attributes all the way through a long
      // chain of hash joins.
      const bool lhs_has_large_output =
          lhs_build.estimated_num_output_attributes
              + lhs_probe.estimated_num_output_attributes > 5;
      const bool rhs_has_large_output =
          rhs_build.estimated_num_output_attributes
              + rhs_probe.estimated_num_output_attributes > 5;
      if (lhs_has_large_output != rhs_has_large_output) {
        return rhs_has_large_output;
      }

      // Prefer foreign-key primary-key style hash joins.
      if (lhs.build_side_unique != rhs.build_side_unique) {
        return lhs.build_side_unique;
      }

      // Prefer hash joins where the build side table has no output attributes.
      const bool lhs_build_has_no_output = lhs_build.estimated_num_output_attributes == 0u;
      const bool rhs_build_has_no_output = rhs_build.estimated_num_output_attributes == 0u;
      if (lhs_build_has_no_output != rhs_build_has_no_output) {
        return lhs_build_has_no_output;
      }

      // Prefer hash joins where the build side table is small.
      const bool lhs_has_small_build = lhs_build.estimated_cardinality < 0x100;
      const bool rhs_has_small_build = rhs_build.estimated_cardinality < 0x100;
      if (lhs_has_small_build != rhs_has_small_build) {
        return lhs_has_small_build;
      }

      // Prefer hash joins where the probe side table is small. This is effective
      // for TPCH style (snowflake schema) queries, with the help of LIPFilters.
      if (lhs_probe.estimated_cardinality != rhs_probe.estimated_cardinality) {
        return lhs_probe.estimated_cardinality < rhs_probe.estimated_cardinality;
      }

      // Prefer build side tables with better selectivity. This is effective
      // for SSB style queries.
      if (lhs_build.estimated_selectivity != rhs_build.estimated_selectivity) {
        return lhs_build.estimated_selectivity < rhs_build.estimated_selectivity;
      }

      // Residual rules that help provide a total order.
      if (lhs_build.estimated_cardinality != rhs_build.estimated_cardinality) {
        return lhs_build.estimated_cardinality < rhs_build.estimated_cardinality;
      }

      const std::size_t lhs_probe_table_info_id = lhs_probe.table_info_id;
      const std::size_t rhs_probe_table_info_id = rhs_probe.table_info_id;
      if (lhs_probe_table_info_id != rhs_probe_table_info_id) {
        return lhs_probe_table_info_id < rhs_probe_table_info_id;
      } else {
        return lhs_build.table_info_id < rhs_build.table_info_id;
      }
    }

    TableInfo *probe;
    TableInfo *build;
    const bool build_side_unique;
    const std::size_t num_join_attributes;
  };

  physical::PhysicalPtr applyInternal(const physical::PhysicalPtr &input,
                                      JoinGroupInfo *paret_join_group);

  physical::PhysicalPtr generatePlan(
      const JoinGroupInfo &join_group_info,
      const expressions::PredicatePtr &residual_predicate,
      const std::vector<expressions::NamedExpressionPtr> &project_expressions);

  std::size_t getEstimatedNumGroups(const physical::PhysicalPtr &input);

  static std::size_t CountSharedAttributes(
      const std::unordered_set<expressions::ExprId> &attr_set1,
      const std::vector<expressions::AttributeReferencePtr> &attr_set2);

  std::unique_ptr<cost::StarSchemaSimpleCostModel> cost_model_;

  DISALLOW_COPY_AND_ASSIGN(StarSchemaHashJoinOrderOptimization);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_RULES_STAR_SCHEMA_HASH_JOIN_ORDER_OPTIMIZATION_HPP_ */
