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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_WINDOWAGGREGATE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_WINDOWAGGREGATE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/ScalarLiteral.hpp"
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
 * @brief WindowAggregate operator that computes aggregate expressions for each
 *        combination of the values of grouping expressions.
 */
class WindowAggregate : public Logical {
 public:
  LogicalType getLogicalType() const override { return LogicalType::kWindowAggregate; }

  std::string getName() const override { return "WindowAggregate"; }

  /**
   * @return The input logical node.
   */
  const LogicalPtr& input() const { return input_; }

  /**
   * @return Aggregate expressions.
   */
  const std::vector<expressions::AliasPtr>& aggregate_expressions() const {
    return aggregate_expressions_;
  }

  /**
   * @return Window attribute.
   */
  const expressions::AttributeReferencePtr& window_attribute() const {
    return window_attribute_;
  }

  /**
   * @return Grouping expressions.
   */
  const std::vector<expressions::NamedExpressionPtr>& grouping_expressions() const {
    return grouping_expressions_;
  }

  /**
   * @return Window duration.
   */
  const expressions::ScalarLiteralPtr& window_duration() const {
    return window_duration_;
  }

  /**
   * @return Emit duration.
   */
  const expressions::ScalarLiteralPtr& emit_duration() const {
    return emit_duration_;
  }

  /**
   * @return Aging duration.
   */
  const expressions::ScalarLiteralPtr& age_duration() const { return age_duration_; }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override;

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override;

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override;

  /**
   * @brief Creates an WindowAggregate logical node.
   *
   * @param input The input node.
   * @param window_attribute The window attribute.
   * @param window_duration The window duration.
   * @param emit_duration The emit duration.
   * @param age_duration The aging duration.
   * @param grouping_expressions The grouping expressions.
   * @param aggregate_expressions The aggregate expressions.
   * @return An immutable WindowAggregate node.
   */
  static WindowAggregatePtr Create(
      LogicalPtr input,
      const expressions::AttributeReferencePtr &window_attribute,
      expressions::ScalarLiteralPtr window_duration,
      expressions::ScalarLiteralPtr emit_duration,
      expressions::ScalarLiteralPtr age_duration,
      const std::vector<expressions::NamedExpressionPtr> &grouping_expressions,
      const std::vector<expressions::AliasPtr> &aggregate_expressions) {
    return WindowAggregatePtr(new WindowAggregate(input,
                                                  window_attribute,
                                                  window_duration,
                                                  emit_duration,
                                                  age_duration,
                                                  grouping_expressions,
                                                  aggregate_expressions));
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
  WindowAggregate(
      LogicalPtr input,
      const expressions::AttributeReferencePtr &window_attribute,
      expressions::ScalarLiteralPtr window_duration,
      expressions::ScalarLiteralPtr emit_duration,
      expressions::ScalarLiteralPtr age_duration,
      const std::vector<expressions::NamedExpressionPtr> &grouping_expressions,
      const std::vector<expressions::AliasPtr> &aggregate_expressions)
      : input_(input),
        window_attribute_(window_attribute),
        window_duration_(window_duration),
        emit_duration_(emit_duration),
        age_duration_(age_duration),
        grouping_expressions_(grouping_expressions),
        aggregate_expressions_(aggregate_expressions) {
    addChild(input_);
  }

  LogicalPtr input_;
  expressions::AttributeReferencePtr window_attribute_;
  expressions::ScalarLiteralPtr window_duration_;
  expressions::ScalarLiteralPtr emit_duration_;
  expressions::ScalarLiteralPtr age_duration_;
  std::vector<expressions::NamedExpressionPtr> grouping_expressions_;
  std::vector<expressions::AliasPtr> aggregate_expressions_;

  DISALLOW_COPY_AND_ASSIGN(WindowAggregate);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_WINDOWAGGREGATE_HPP_ */
