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

#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "query_optimizer/cost_model/StarSchemaSimpleCostModel.hpp"
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
 * @brief TODO
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
          estimated_num_output_attributes(estimated_num_output_attributes_in),
          is_aggregation(is_aggregation_in) {
    }

    const std::size_t table_info_id;
    physical::PhysicalPtr table;
    std::size_t estimated_cardinality;
    double estimated_selectivity;
    std::size_t estimated_num_output_attributes;
    bool is_aggregation;
  };

  struct JoinPair {
    JoinPair(TableInfo *probe_in,
             TableInfo *build_in,
             const bool build_side_unique_in)
        : probe(probe_in),
          build(build_in),
          build_side_unique(build_side_unique_in) {
    }

    inline bool isBetterThan(const JoinPair &rhs) const {
      const auto &lhs = *this;

      if (lhs.build_side_unique != rhs.build_side_unique) {
        return lhs.build_side_unique;
      }

      const bool lhs_has_large_output =
          lhs.build->estimated_num_output_attributes
              + lhs.probe->estimated_num_output_attributes > 5;
      const bool rhs_has_large_output =
          rhs.build->estimated_num_output_attributes
              + rhs.probe->estimated_num_output_attributes > 5;
      if (lhs_has_large_output || rhs_has_large_output) {
        if (lhs_has_large_output != rhs_has_large_output) {
          return rhs_has_large_output;
        }
        double lhs_selectivity =
            lhs.build->estimated_selectivity * lhs.probe->estimated_selectivity;
        double rhs_selectivity =
            rhs.build->estimated_selectivity * rhs.probe->estimated_selectivity;
        if (lhs_selectivity != rhs_selectivity) {
          return lhs_selectivity < rhs_selectivity;
        }
      }

      const bool lhs_has_small_build =
          !lhs_has_large_output && lhs.build->estimated_cardinality < 0x100;
      const bool rhs_has_small_build =
          !rhs_has_large_output && rhs.build->estimated_cardinality < 0x100;
      if (lhs_has_small_build != rhs_has_small_build) {
        return lhs_has_small_build;
      }

      if (lhs.probe->is_aggregation != rhs.probe->is_aggregation) {
        return lhs.probe->is_aggregation;
      }

      if (lhs.probe->estimated_cardinality != rhs.probe->estimated_cardinality) {
        return lhs.probe->estimated_cardinality < rhs.probe->estimated_cardinality;
      }
      if (lhs.build->estimated_selectivity != rhs.build->estimated_selectivity) {
        return lhs.build->estimated_selectivity < rhs.build->estimated_selectivity;
      }
      if (lhs.build->estimated_cardinality != rhs.build->estimated_cardinality) {
        return lhs.build->estimated_cardinality < rhs.build->estimated_cardinality;
      }
      if (lhs.probe->table != rhs.probe->table) {
        return lhs.probe->table < rhs.probe->table;
      } else {
        return lhs.build->table < rhs.build->table;
      }
    }

    TableInfo *probe;
    TableInfo *build;
    const bool build_side_unique;
  };

  physical::PhysicalPtr applyInternal(const physical::PhysicalPtr &input,
                                      JoinGroupInfo *paret_join_group);

  physical::PhysicalPtr generatePlan(
      const JoinGroupInfo &join_group_info,
      const expressions::PredicatePtr &residual_predicate,
      const std::vector<expressions::NamedExpressionPtr> &project_expressions);

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
