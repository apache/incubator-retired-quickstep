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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_INSERT_SELECTION_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_INSERT_SELECTION_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
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

class InsertSelection;
typedef std::shared_ptr<const InsertSelection> InsertSelectionPtr;

/**
 * @brief Inserts the results from a SELECT query into a relation.
 */
class InsertSelection : public Physical {
 public:
  PhysicalType getPhysicalType() const override {
    return PhysicalType::kInsertSelection;
  }

  std::string getName() const override { return "InsertSelection"; }

  /**
   * @return The destination relation to insert the selection to.
   */
  const PhysicalPtr& destination() const { return destination_; }

  /**
   * @return The selection to be inserted.
   */
  const PhysicalPtr& selection() const { return selection_; }

  PhysicalPtr copyWithNewChildren(
      const std::vector<PhysicalPtr> &new_children) const override {
    DCHECK_EQ(getNumChildren(), new_children.size());
    return Create(new_children[0], new_children[1]);
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return {};
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return selection_->getOutputAttributes();
  }

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const override {
    return false;
  }

  /**
   * @brief Creates an InsertSelection physical node.
   *
   * @param destination The destination relation to insert the selection to.
   * @param selection The selection to be inserted.
   * @return An immutable InsertSelection node.
   */
  static InsertSelectionPtr Create(
      const PhysicalPtr &destination,
      const PhysicalPtr &selection) {
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
  InsertSelection(const PhysicalPtr &destination,
                  const PhysicalPtr &selection)
      : destination_(destination),
        selection_(selection) {
    addChild(destination_);
    addChild(selection_);
  }

  const PhysicalPtr destination_;
  const PhysicalPtr selection_;

  DISALLOW_COPY_AND_ASSIGN(InsertSelection);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_INSERT_SELECTION_HPP_ */
