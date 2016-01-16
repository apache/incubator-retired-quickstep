/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_AGGREGATE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_AGGREGATE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class Aggregate;
typedef std::shared_ptr<const Aggregate> AggregatePtr;

/**
 * @brief Aggregate operator that computes aggregate expressions for each combination
 *        of the values of grouping expressions.
 */
class Aggregate : public Logical {
 public:
  LogicalType getLogicalType() const override { return LogicalType::kAggregate; }

  std::string getName() const override { return "Aggregate"; }

  /**
   * @return The input logical node.
   */
  const LogicalPtr& input() const { return input_; }

  /**
   * @return Grouping expressions.
   */
  const std::vector<expressions::NamedExpressionPtr>& grouping_expressions() const {
    return grouping_expressions_;
  }

  /**
   * @return Aggregate expressions.
   */
  const std::vector<expressions::AliasPtr>& aggregate_expressions() const {
    return aggregate_expressions_;
  }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override;

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override;

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override;

  /**
   * @brief Creates an Aggregate logical node.
   *
   * @param input The input node.
   * @param aggregate_expressions The aggregate expressions.
   * @param grouping_expressions The grouping expressions.
   * @return An immutable Aggregate node.
   */
  static AggregatePtr Create(
      LogicalPtr input,
      const std::vector<expressions::NamedExpressionPtr> &grouping_expressions,
      const std::vector<expressions::AliasPtr> &aggregate_expressions) {
    return AggregatePtr(
        new Aggregate(input, grouping_expressions, aggregate_expressions));
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
  Aggregate(LogicalPtr input,
            const std::vector<expressions::NamedExpressionPtr> &grouping_expressions,
            const std::vector<expressions::AliasPtr> &aggregate_expressions)
      : input_(input),
        grouping_expressions_(grouping_expressions),
        aggregate_expressions_(aggregate_expressions) {
    addChild(input_);
  }

  LogicalPtr input_;
  std::vector<expressions::NamedExpressionPtr> grouping_expressions_;
  std::vector<expressions::AliasPtr> aggregate_expressions_;

  DISALLOW_COPY_AND_ASSIGN(Aggregate);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_AGGREGATE_HPP_ */
