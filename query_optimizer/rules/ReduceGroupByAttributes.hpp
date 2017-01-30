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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_REDUCE_GROUP_BY_ATTRIBUTES_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_REDUCE_GROUP_BY_ATTRIBUTES_HPP_

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "query_optimizer/cost_model/StarSchemaSimpleCostModel.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/rules/Rule.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

class OptimizerContext;

/**
 * @brief Rule that applies to a physical plan to reduce the number of group-by
 *        attributes for Aggregate nodes (to improve performance) by pulling
 *        joins up the aggregations.
 *
 * For example, let R be a relation with PRIMARY KEY x and attributes y, z. Let
 * S be a relation with FOREIGN KEY u refering to R(x) and attribute v. Then the
 * optimization rule will transform the physical plan:
 *   Aggregate(
 *     [input relation]: HashJoin(
 *                         [probe relation]: S
 *                         [build relation]: R
 *                         [join expression]: S.u = R.x
 *                         [project attributes]: v, x, y, z
 *                       )
 *     [aggregate expression]: SUM(v) AS sum_v
 *     [group-by attributes]: x, y, z
 *   )
 *
 * into:
 *   HashJoin(
 *     [probe relation]: Aggregate(
 *                         [input relation]: S
 *                         [aggregate expression]: SUM(v) AS sum_v
 *                         [group-by attribute]: u
 *                       ) AS T
 *     [build relation]: R
 *     [join expression]: T.u = R.x
 *     [project attributes]: sum_v, x, y, z
 *   )
 */
class ReduceGroupByAttributes : public Rule<physical::Physical> {
 public:
  /**
   * @brief Constructor.
   *
   * @param optimizer_context The optimizer context.
   */
  explicit ReduceGroupByAttributes(OptimizerContext *optimizer_context)
      : optimizer_context_(optimizer_context) {}

  ~ReduceGroupByAttributes() override {}

  std::string getName() const override {
    return "ReduceGroupByAttributes";
  }

  physical::PhysicalPtr apply(const physical::PhysicalPtr &input) override;

 private:
  struct AttributeInfo {
    AttributeInfo(const expressions::AttributeReferencePtr &attribute_in,
                  const bool is_unique_in,
                  const bool is_fixed_length_in,
                  const std::size_t maximum_size_in)
        : attribute(attribute_in),
          is_unique(is_unique_in),
          is_fixed_length(is_fixed_length_in),
          maximum_size(maximum_size_in) {}

    // In the situation that there are multiple attributes that can serve as the
    // group-by key, we define an ordering based on aggregation performance (e.g.
    // it is faster to do aggregation with a fix-length attribute as the group-by
    // key than with a variable-length attribute).
    inline static bool IsBetterThan(const AttributeInfo *lhs,
                                    const AttributeInfo *rhs) {
      if (lhs->is_unique != rhs->is_unique) {
        return lhs->is_unique;
      }
      if (lhs->is_fixed_length != rhs->is_fixed_length) {
        return lhs->is_fixed_length;
      }
      if (lhs->maximum_size != rhs->maximum_size) {
        return lhs->maximum_size < rhs->maximum_size;
      }
      return lhs->attribute->id() < rhs->attribute->id();
    }

    const expressions::AttributeReferencePtr attribute;
    const bool is_unique;
    const bool is_fixed_length;
    const std::size_t maximum_size;
  };

  physical::PhysicalPtr applyInternal(const physical::PhysicalPtr &input);
  physical::PhysicalPtr applyToNode(const physical::PhysicalPtr &input);

  OptimizerContext *optimizer_context_;
  std::unique_ptr<cost::StarSchemaSimpleCostModel> cost_model_;

  // Maps an attribute's id to the TableReference that generates the attribute.
  std::unordered_map<expressions::ExprId,
                     std::pair<physical::TableReferencePtr,
                               expressions::AttributeReferencePtr>> source_;

  DISALLOW_COPY_AND_ASSIGN(ReduceGroupByAttributes);
};

}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_RULES_REDUCE_GROUP_BY_ATTRIBUTES_HPP_
