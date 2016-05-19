/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
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
    std::vector<physical::PhysicalPtr> tables;
    std::vector<std::pair<expressions::ExprId, expressions::ExprId>> join_attribute_pairs;
  };

  /**
   * @brief Auxiliary information of a table for the optimizer.
   */
  struct TableInfo {
    TableInfo(const std::size_t in_table_info_id,
              const physical::PhysicalPtr &in_table,
              const std::size_t in_estimated_cardinality,
              const double in_estimated_selectivity)
        : table_info_id(in_table_info_id),
          table(in_table),
          estimated_cardinality(in_estimated_cardinality),
          estimated_selectivity(in_estimated_selectivity) {
    }

    const std::size_t table_info_id;
    physical::PhysicalPtr table;
    std::size_t estimated_cardinality;
    double estimated_selectivity;
    std::unordered_multimap<expressions::ExprId, expressions::ExprId> join_attribute_pairs;
    std::unordered_set<expressions::ExprId> joined_attribute_set;
  };

  /**
   * @brief Comparator that compares the join priorities between two tables.
   */
  struct TableInfoPtrLessComparator {
    inline bool operator() (const TableInfo *lhs, const TableInfo *rhs) {
      bool swapped = false;
      if (lhs->estimated_cardinality > rhs->estimated_cardinality) {
        std::swap(lhs, rhs);
        swapped = true;
      }

      if (lhs->estimated_selectivity < rhs->estimated_selectivity) {
        return !swapped;
      } else if (lhs->estimated_cardinality < 1000u &&
                 rhs->estimated_cardinality > 10000u &&
                 lhs->estimated_selectivity < rhs->estimated_selectivity * 1.5) {
        return !swapped;
      } else if (lhs->estimated_selectivity > rhs->estimated_selectivity) {
        return swapped;
      } else if (lhs->estimated_cardinality != rhs->estimated_cardinality) {
        return !swapped;
      } else {
        return swapped ^ (lhs->table < rhs->table);
      }
    }
  };

  physical::PhysicalPtr applyInternal(const physical::PhysicalPtr &input,
                                      JoinGroupInfo *paret_join_group);

  physical::PhysicalPtr generatePlan(
      const JoinGroupInfo &join_group_info,
      const expressions::PredicatePtr &residual_predicate,
      const std::vector<expressions::NamedExpressionPtr> &project_expressions);

  std::unique_ptr<cost::StarSchemaSimpleCostModel> cost_model_;

  DISALLOW_COPY_AND_ASSIGN(StarSchemaHashJoinOrderOptimization);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_RULES_STAR_SCHEMA_HASH_JOIN_ORDER_OPTIMIZATION_HPP_ */
