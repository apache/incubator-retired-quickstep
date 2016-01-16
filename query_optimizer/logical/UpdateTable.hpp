/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_UPDATETABLE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_UPDATETABLE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class UpdateTable;
typedef std::shared_ptr<const UpdateTable> UpdateTablePtr;

/**
 * @brief Updates a relation with new assignments to some tuples.
 */
class UpdateTable : public Logical {
 public:
  LogicalType getLogicalType() const override { return LogicalType::kUpdateTable; }

  std::string getName() const override { return "UpdateTable"; }

  /**
   * @return The logical node that creates the table to be updated.
   */
  const LogicalPtr& input() const { return input_; }

  /**
   * @return The attributes to be assigned values to.
   */
  const std::vector<expressions::AttributeReferencePtr>& assignees() const {
    return assignees_;
  }

  /**
   * @return The expressions for which the values are assigned.
   */
  const std::vector<expressions::ScalarPtr>& assignment_expressions() const {
    return assignment_expressions_;
  }

  /**
   * @return The predicate to select tuples to be updated.
   */
  const expressions::PredicatePtr& predicate() const { return predicate_; }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override;

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return std::vector<expressions::AttributeReferencePtr>();
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override;

  /**
   * @brief Creates an UpdateTable logical node that represents an operation to
   *        update a table with given assignments.
   *
   * @param input The input gives the table to be updated.
   * @param assignees The attribute list to assign values to.
   * @param assignment_expressions The expressions that gives the values to be
   *                               assigned.
   * @param predicate The predicate to select tuples to be updated.
   * @return An immutable UpdateTable node.
   */
  static UpdateTablePtr Create(
      const LogicalPtr &input,
      const std::vector<expressions::AttributeReferencePtr> &assignees,
      const std::vector<expressions::ScalarPtr> &assignment_expressions,
      const expressions::PredicatePtr &predicate) {
    return UpdateTablePtr(
        new UpdateTable(input, assignees, assignment_expressions, predicate));
  }

  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

 private:
  UpdateTable(const LogicalPtr &input,
              const std::vector<expressions::AttributeReferencePtr> &assignees,
              const std::vector<expressions::ScalarPtr> &assignment_expressions,
              const expressions::PredicatePtr &predicate)
      : input_(input),
        assignees_(assignees),
        assignment_expressions_(assignment_expressions),
        predicate_(predicate) {
    addChild(input);
  }

  LogicalPtr input_;
  // The attributes to be assigned values to.
  std::vector<expressions::AttributeReferencePtr> assignees_;
  // The expressions for which the values are assigned. Has 1:1 matching with
  // <assignees_>.
  std::vector<expressions::ScalarPtr> assignment_expressions_;
  expressions::PredicatePtr predicate_;

  DISALLOW_COPY_AND_ASSIGN(UpdateTable);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_UPDATETABLE_HPP_ */
