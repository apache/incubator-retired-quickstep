/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_INSERT_TUPLE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_INSERT_TUPLE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ScalarLiteral.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class InsertTuple;
typedef std::shared_ptr<const InsertTuple> InsertTuplePtr;

/**
 * @brief Inserts a tuple with <column_values_> into a relation.
 */
class InsertTuple : public Logical {
 public:
  LogicalType getLogicalType() const override { return LogicalType::kInsertTuple; }

  std::string getName() const override { return "InsertTuple"; }

  /**
   * @return The input that produces the relation to insert the tuple to.
   */
  const LogicalPtr& input() const { return input_; }

  /**
   * @return Column values to be used to compose a new tuple.
   */
  const std::vector<expressions::ScalarLiteralPtr>& column_values() const {
    return column_values_;
  }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override {
    DCHECK_EQ(getNumChildren(), new_children.size());
    return Create(new_children[0], column_values_);
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return {};
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return {};
  }

  /**
   * @brief Creates an InsertTuple logical node.
   *
   * @param input The input produces the relation to insert the tuple to.
   * @param column_values The column values of the tuple to be inserted.
   * @return An immutable InsertTuple node.
   */
  static InsertTuplePtr Create(
      const LogicalPtr &input,
      const std::vector<expressions::ScalarLiteralPtr> &column_values) {
    return InsertTuplePtr(new InsertTuple(input, column_values));
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
  InsertTuple(const LogicalPtr &input,
              const std::vector<expressions::ScalarLiteralPtr> &column_values)
      : input_(input), column_values_(column_values) {
    addChild(input_);
  }

  LogicalPtr input_;
  std::vector<expressions::ScalarLiteralPtr> column_values_;

  DISALLOW_COPY_AND_ASSIGN(InsertTuple);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_INSERT_TUPLE_HPP_ */
