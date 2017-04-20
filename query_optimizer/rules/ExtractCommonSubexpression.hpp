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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_EXTRACT_COMMON_SUBEXPRESSION_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_EXTRACT_COMMON_SUBEXPRESSION_HPP_

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "query_optimizer/expressions/CommonSubexpression.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/rules/Rule.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

class OptimizerContext;

/** \addtogroup OptimizerRules
 *  @{
 */

/**
 * @brief Rule that applies to a physical plan to identify and label common
 *        subexpressions.
 *
 * @note This is essentially a logical optimization pass. However, we need some
 *       of the physical passes (e.g. ReuseAggregateExpressions) to be finalized
 *       before this one to maximize optimization opportunities.
 */
class ExtractCommonSubexpression : public Rule<physical::Physical> {
 public:
  /**
   * @brief Constructor.
   *
   * @param optimizer_context The optimizer context.
   */
  explicit ExtractCommonSubexpression(OptimizerContext *optimizer_context);

  ~ExtractCommonSubexpression() override {}

  std::string getName() const override {
    return "ExtractCommonSubexpression";
  }

  physical::PhysicalPtr apply(const physical::PhysicalPtr &input) override;

 private:
  physical::PhysicalPtr applyInternal(const physical::PhysicalPtr &input);

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

  // For memorizing whether a subexpression is hashable.
  using ScalarHashable = std::unordered_set<expressions::ScalarPtr>;

  // For counting the number of occurrences of each subexpression.
  using ScalarCounter =
      std::unordered_map<expressions::ScalarPtr, std::size_t, ScalarHash, ScalarEqual>;

  // For mapping each subexpression to its transformed version.
  using ScalarMap =
      std::unordered_map<expressions::ScalarPtr,
                         expressions::CommonSubexpressionPtr,
                         ScalarHash,
                         ScalarEqual>;

  std::vector<expressions::ExpressionPtr> transformExpressions(
      const std::vector<expressions::ExpressionPtr> &expressions);

  expressions::ExpressionPtr transformExpression(
      const expressions::ExpressionPtr &expression);

  // Traverse the expression tree and increase the count of each subexpression.
  bool visitAndCount(
      const expressions::ExpressionPtr &expression,
      ScalarCounter *counter,
      ScalarHashable *hashable);

  // Traverse the expression tree and transform subexpressions (to common
  // subexpressions) if applicable.
  expressions::ExpressionPtr visitAndTransform(
      const expressions::ExpressionPtr &expression,
      const std::size_t max_reference_count,
      const ScalarCounter &counter,
      const ScalarHashable &hashable,
      ScalarMap *substitution_map);

  template <typename ScalarSubclassT>
  static std::vector<expressions::ExpressionPtr> UpCast(
      const std::vector<std::shared_ptr<const ScalarSubclassT>> &expressions) {
    std::vector<expressions::ExpressionPtr> output;
    for (const auto &expr : expressions) {
      output.emplace_back(expr);
    }
    return output;
  }

  template <typename ScalarSubclassT>
  static std::vector<std::shared_ptr<const ScalarSubclassT>> DownCast(
      const std::vector<expressions::ExpressionPtr> &expressions) {
    std::vector<std::shared_ptr<const ScalarSubclassT>> output;
    for (const auto &expr : expressions) {
      output.emplace_back(std::static_pointer_cast<const ScalarSubclassT>(expr));
    }
    return output;
  }

  struct ExpressionTypeHash {
    using UnderT = std::underlying_type<expressions::ExpressionType>::type;

    inline std::size_t operator()(const expressions::ExpressionType &et) const {
      return std::hash<UnderT>()(static_cast<UnderT>(et));
    }
  };

  // Here we define that an expression type is homogeneous if at execution time
  // the result column vector of that expression has a one-to-one positional
  // correspondence with all the result column vectors from its child expressions.
  // E.g. aggregate functions and CASE expressions are not homogeneous.
  //
  // Being homogeneous enables common subexpressions to be hoisted through.
  // For example, consider the case
  // --
  //   (x * 2) + F(x * 2)
  // --
  // where F is some unary expression. Then if F is homogenous, we can mark the
  // two (x * 2) as common subexpressions. Otherwise, we cannot do that since
  // the two subexpressions will generate different result column vectors.
  std::unordered_set<expressions::ExpressionType,
                     ExpressionTypeHash> homogeneous_expression_types_;

  OptimizerContext *optimizer_context_;

  DISALLOW_COPY_AND_ASSIGN(ExtractCommonSubexpression);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_RULES_EXTRACT_COMMON_SUBEXPRESSION_HPP_
