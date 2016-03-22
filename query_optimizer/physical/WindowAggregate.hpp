/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_WINDOW_AGGREGATE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_WINDOW_AGGREGATE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/expressions/ScalarLiteral.hpp"
#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace physical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class WindowAggregate;
typedef std::shared_ptr<const WindowAggregate> WindowAggregatePtr;

/**
 * @brief WindowAggregate operator that computes aggregate expressions for each combination
 *        of the values of grouping expressions.
 */
class WindowAggregate : public Physical {
 public:
  PhysicalType getPhysicalType() const override { return PhysicalType::kWindowAggregate; }

  std::string getName() const override { return "WindowAggregate"; }

  /**
   * @return The input physical node.
   */
  const PhysicalPtr input() const { return input_; }

  /**
   * @return The window attribute.
   */
  const expressions::AttributeReferencePtr& window_attribute() const { return window_attribute_; }

  /**
   * @return The window duration/size.
   */
  const expressions::ScalarLiteralPtr& window_duration() const { return window_duration_; }

  /**
   * @return The emit duration.
   */
  const expressions::ScalarLiteralPtr& emit_duration() const { return emit_duration_; }

  /**
   * @return The aging duration.
   */
  const expressions::ScalarLiteralPtr& age_duration() const { return age_duration_; }

  /**
   * @return Grouping expressions.
   */
  inline const std::vector<expressions::NamedExpressionPtr>& grouping_expressions() const {
    return grouping_expressions_;
  }

  /**
   * @return WindowAggregate expressions.
   */
  const std::vector<expressions::AliasPtr>& aggregate_expressions() const {
    return aggregate_expressions_;
  }

  PhysicalPtr copyWithNewChildren(
      const std::vector<PhysicalPtr> &new_children) const override {
    DCHECK_EQ(getNumChildren(), new_children.size());
    return Create(new_children[0],
                  window_attribute_,
                  window_duration_,
                  emit_duration_,
                  age_duration_,
                  grouping_expressions_,
                  aggregate_expressions_);
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override;

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override;

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const override {
    // The project expressions for an WindowAggregate cannot be changed.
    return false;
  }

  /**
   * @brief Creates an WindowAggregate physical node.
   *
   * @param input The input node.
   * @param window_attribute The window attribute.
   * @param window_duration The window duration/size.
   * @param emit_duration The emit duration.
   * @param age_duration The age duration.
   * @param aggregate_expressions The aggregate expressions.
   * @param grouping_expressions The grouping expressions.
   * @return An immutable WindowAggregate node.
   */
  static WindowAggregatePtr Create(
      PhysicalPtr input,
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
      PhysicalPtr input,
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

  PhysicalPtr input_;
  expressions::AttributeReferencePtr window_attribute_;
  expressions::ScalarLiteralPtr window_duration_;
  expressions::ScalarLiteralPtr emit_duration_;
  expressions::ScalarLiteralPtr age_duration_;
  std::vector<expressions::NamedExpressionPtr> grouping_expressions_;
  std::vector<expressions::AliasPtr> aggregate_expressions_;

  DISALLOW_COPY_AND_ASSIGN(WindowAggregate);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_WINDOW_AGGREGATE_HPP_ */
