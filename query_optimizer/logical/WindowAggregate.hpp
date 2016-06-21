/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_WINDOW_AGGREGATE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_WINDOW_AGGREGATE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class WindowAggregate;
typedef std::shared_ptr<const WindowAggregate> WindowAggregatePtr;

/**
 * @brief Window aggregate operator that computes window aggregate expressions.
 */
class WindowAggregate : public Logical {
 public:
  LogicalType getLogicalType() const override {
    return LogicalType::kWindowAggregate;
  }

  std::string getName() const override { return "WindowAggregate"; }

  /**
   * @return The input logical node.
   */
  const LogicalPtr& input() const { return input_; }

  /**
   * @return PARTITION BY expressions.
   */
  const expressions::AliasPtr window_aggregate_expression() const {
    return window_aggregate_expression_;
  }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override;

  LogicalPtr copyWithNewInputExpressions(
      const std::vector<expressions::ExpressionPtr> &input_expressions) const override;

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override;

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override;

  /**
   * @brief Creates an Aggregate logical node.
   *
   * @param input The input node.
   * @param window_aggregate_expression The window aggregate expression.
   * @return An immutable WindowAggregate node.
   */
  static WindowAggregatePtr Create(
      const LogicalPtr &input,
      const expressions::AliasPtr &window_aggregate_expression) {
    return WindowAggregatePtr(new WindowAggregate(input, window_aggregate_expression));
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

 private:
  WindowAggregate(const LogicalPtr &input,
                  const expressions::AliasPtr &window_aggregate_expression)
      : input_(input),
        window_aggregate_expression_(window_aggregate_expression) {
    addChild(input_);
    addInputExpression(window_aggregate_expression_);
  }

  const LogicalPtr input_;
  const expressions::AliasPtr window_aggregate_expression_;

  DISALLOW_COPY_AND_ASSIGN(WindowAggregate);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_WINDOW_AGGREGATE_HPP_
