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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_SHARED_SUBPLAN_REFERENCE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_SHARED_SUBPLAN_REFERENCE_HPP_

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
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

class SharedSubplanReference;
typedef std::shared_ptr<const SharedSubplanReference> SharedSubplanReferencePtr;

/**
 * @breif A reference to a shared subplan stored in the TopLevelPlan node.
 */
class SharedSubplanReference : public physical::Physical {
 public:
  PhysicalType getPhysicalType() const override {
    return PhysicalType::kSharedSubplanReference;
  }

  std::string getName() const override {
    return "SharedSubplanReference";
  }

  /**
   * @return The ID of the shared subplan.
   */
  int subplan_id() const {
    return subplan_id_;
  }

  /**
   * @return The output attributes of the shared subplan.
   */
  const std::vector<expressions::AttributeReferencePtr>& output_attributes() const {
    return output_attributes_;
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return output_attributes_;
  }

  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

  PhysicalPtr copyWithNewChildren(const std::vector<PhysicalPtr> &new_children) const override {
    DCHECK(new_children.empty());
    return Create(subplan_id_, output_attributes_);
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return output_attributes_;
  }

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const override {
    return false;
  }

  /**
   * @brief Creates a SharedSubplanReference.
   *
   * @param subplan_id The ID of the shared subplan, which is the index of
   *        the referenced shared subplan in <shared_subplans_> of the TopLevelPlan.
   * @param output_attributes The output attributes of the shared subplan.
   * @return An immutable SharedSubplanReference.
   */
  static SharedSubplanReferencePtr Create(int subplan_id,
                                          const std::vector<expressions::AttributeReferencePtr> &output_attributes) {
    return SharedSubplanReferencePtr(new SharedSubplanReference(subplan_id, output_attributes));
  }

 private:
  SharedSubplanReference(int subplan_id,
                         const std::vector<expressions::AttributeReferencePtr> &output_attributes)
    : subplan_id_(subplan_id),
      output_attributes_(output_attributes) {}

  int subplan_id_;
  std::vector<expressions::AttributeReferencePtr> output_attributes_;

  DISALLOW_COPY_AND_ASSIGN(SharedSubplanReference);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_SHARED_SUBPLAN_REFERENCE_HPP_ */
