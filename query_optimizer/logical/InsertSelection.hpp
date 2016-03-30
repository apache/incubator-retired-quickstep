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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_INSERT_SELECTION_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_INSERT_SELECTION_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
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

class InsertSelection;
typedef std::shared_ptr<const InsertSelection> InsertSelectionPtr;

/**
 * @brief Inserts the results from a SELECT query into a relation.
 */
class InsertSelection : public Logical {
 public:
  LogicalType getLogicalType() const override { return LogicalType::kInsertSelection; }

  std::string getName() const override { return "InsertSelection"; }

  /**
   * @return The destination relation to insert the selection to.
   */
  const LogicalPtr& destination() const { return destination_; }

  /**
   * @return The selection to be inserted.
   */
  const LogicalPtr& selection() const { return selection_; }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override {
    DCHECK_EQ(getNumChildren(), new_children.size());
    return Create(new_children[0], new_children[1]);
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return {};
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return selection_->getOutputAttributes();
  }

  /**
   * @brief Creates an InsertSelection logical node.
   *
   * @param destination The destination relation to insert the selection to.
   * @param selection The selection to be inserted.
   * @return An immutable InsertSelection node.
   */
  static InsertSelectionPtr Create(
      const LogicalPtr &destination,
      const LogicalPtr &selection) {
    return InsertSelectionPtr(new InsertSelection(destination, selection));
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
  InsertSelection(const LogicalPtr &destination,
                  const LogicalPtr &selection)
      : destination_(destination),
        selection_(selection) {
    addChild(destination);
    addChild(selection);
  }

  LogicalPtr destination_;
  LogicalPtr selection_;

  DISALLOW_COPY_AND_ASSIGN(InsertSelection);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_INSERT_SELECTION_HPP_ */
