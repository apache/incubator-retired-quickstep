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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_REUSE_AGGREGATE_EXPRESSIONS_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_REUSE_AGGREGATE_EXPRESSIONS_HPP_

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/cost_model/StarSchemaSimpleCostModel.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/rules/BottomUpRule.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

class OptimizerContext;

/** \addtogroup OptimizerRules
 *  @{
 */

/**
 * @brief Rule that applies to a physical plan to eliminate duplicate aggregate
 *        expressions and convert AVG to SUM/COUNT if appropriate.
 *
 * This rule rewrites Aggregate to Selection + Aggregate to eliminate duplicates
 * and strength-reduce AVG functions. E.g.
 * --
 *   SELECT SUM(x), SUM(y), SUM(x) * 2, AVG(y), COUNT(*)
 *   FROM s;
 * --
 * will be rewritten as (assume y is not null)
 * --
 *   SELECT sum_x, sum_y, sum_x * 2, sum_y / cnt, cnt
 *   FROM (
 *     SELECT SUM(x) AS sum_x, SUM(y) AS sum_y, COUNT(*) AS cnt
 *   ) t;
 * --
 *
 * Meanwhile, note that currently it is not free-of-cost to "re-project" the
 * columns. So it may not worth doing the transformation in some situations.
 * E.g. it may actually slow down the query to rewrite
 * --
 *   SELECT SUM(a), SUM(b), SUM(c), SUM(d), SUM(a)
 *   FROM s
 *   GROUP BY x;
 * --
 * as
 * --
 *   SELECT sum_a, sum_b, sum_c, sum_d, sum_a
 *   FROM (
 *     SELECT SUM(a) AS sum_a, SUM(b) AS sum_b, SUM(c) AS sum_c, SUM(d) AS sum_d
 *     FROM s
 *     GROUP BY x;
 *   ) t;
 * --
 * if the number of distinct values of attribute x is large -- in this case, we
 * avoid one duplicate computation of SUM(a), but introduce 5 extra expensive
 * column copying with the outer Selection.
 *
 * To address the above problem, currently we employ a simple heuristic that if
 * either
 * (1) The estimated number of groups for this Aggregate node is smaller than
 *     the specified FLAGS_reuse_aggregate_group_size_threshold.
 * or
 * (2) The ratio of (# of eliminable columns) to (# of original columns) is
 *     greater than the specified FLAGS_reuse_aggregate_ratio_threshold.
 * then we perform the transformation.
 */
class ReuseAggregateExpressions : public BottomUpRule<physical::Physical> {
 public:
  /**
   * @brief Constructor.
   *
   * @param optimizer_context The optimizer context.
   */
  explicit ReuseAggregateExpressions(OptimizerContext *optimizer_context)
      : optimizer_context_(optimizer_context) {}

  ~ReuseAggregateExpressions() override {}

  std::string getName() const override {
    return "ReuseAggregateExpressions";
  }

 protected:
  void init(const physical::PhysicalPtr &input) override;

  physical::PhysicalPtr applyToNode(const physical::PhysicalPtr &input) override;

 private:
  struct ScalarHash {
    inline std::size_t operator()(const expressions::ScalarPtr &scalar) const {
      return scalar->hash();
    }
  };

  struct ScalarEqual {
    inline bool operator()(const expressions::ScalarPtr &lhs,
                           const expressions::ScalarPtr &rhs) const {
      return lhs->equals(rhs);
    }
  };

  // This data structure indicates for each aggregate expression whether the
  // expression can be eliminated by refering to another identical expression,
  // or can be strength-reduced from AVG to SUM.
  struct AggregateReference {
    enum Kind {
      kDirect = 0,
      kAvg
    };

    explicit AggregateReference(const std::size_t ref)
        : kind(kDirect), first_ref(ref), second_ref(0) {}

    AggregateReference(const std::size_t sum_ref, const std::size_t count_ref)
        : kind(kAvg), first_ref(sum_ref), second_ref(count_ref) {}

    const Kind kind;
    const std::size_t first_ref;
    const std::size_t second_ref;
  };

  OptimizerContext *optimizer_context_;

  std::unique_ptr<cost::StarSchemaSimpleCostModel> cost_model_;

  DISALLOW_COPY_AND_ASSIGN(ReuseAggregateExpressions);
};

}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_RULES_REUSE_AGGREGATE_EXPRESSIONS_HPP_
