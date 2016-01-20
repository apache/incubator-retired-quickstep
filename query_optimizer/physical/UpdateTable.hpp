/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_UPDATETABLE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_UPDATETABLE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

class UpdateTable;
typedef std::shared_ptr<const UpdateTable> UpdateTablePtr;

/**
 * @brief Updates a relation with new assignments to some tuples.
 */
class UpdateTable : public Physical {
 public:
  PhysicalType getPhysicalType() const override {
    return PhysicalType::kUpdateTable;
  }

  std::string getName() const override { return "UpdateTable"; }

  /**
   * @return The logical node that creates the table to be updated.
   */
  const PhysicalPtr& input() const { return input_; }

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

  PhysicalPtr copyWithNewChildren(
      const std::vector<PhysicalPtr> &new_children) const override {
    DCHECK_EQ(getNumChildren(), new_children.size());
    return Create(new_children[0], assignees_, assignment_expressions_,
                  predicate_);
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return std::vector<expressions::AttributeReferencePtr>();
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override;

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const override {
    return false;
  }

  /**
   * @brief Creates an UpdateTable physical node that represents an operation to
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
      const PhysicalPtr &input,
      const std::vector<expressions::AttributeReferencePtr> &assignees,
      const std::vector<expressions::ScalarPtr> &assignment_expressions,
      const expressions::PredicatePtr &predicate) {
    return UpdateTablePtr(
        new UpdateTable(input, assignees, assignment_expressions, predicate));
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
  UpdateTable(const PhysicalPtr &input,
              const std::vector<expressions::AttributeReferencePtr> &assignees,
              const std::vector<expressions::ScalarPtr> &assignment_expressions,
              const expressions::PredicatePtr &predicate)
      : input_(input),
        assignees_(assignees),
        assignment_expressions_(assignment_expressions),
        predicate_(predicate) {
    addChild(input);
  }

  PhysicalPtr input_;
  // The attributes to be assigned values to.
  std::vector<expressions::AttributeReferencePtr> assignees_;
  // The expressions for which the values are assigned. Has 1:1 matching with
  // <assignees_>.
  std::vector<expressions::ScalarPtr> assignment_expressions_;
  expressions::PredicatePtr predicate_;

  DISALLOW_COPY_AND_ASSIGN(UpdateTable);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_UPDATETABLE_HPP_ */
