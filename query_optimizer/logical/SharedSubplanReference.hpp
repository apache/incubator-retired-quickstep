/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_SHARED_SUBPLAN_REFERENCE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_SHARED_SUBPLAN_REFERENCE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class SharedSubplanReference;
typedef std::shared_ptr<const SharedSubplanReference> SharedSubplanReferencePtr;

/**
 * @brief A reference to a shared subplan stored in the TopLevelPlan node.
 */
class SharedSubplanReference : public Logical {
 public:
  LogicalType getLogicalType() const override {
    return LogicalType::kSharedSubplanReference;
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

  LogicalPtr copyWithNewChildren(const std::vector<LogicalPtr> &new_children) const override;

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return output_attributes_;
  }

  /**
   * @brief Creates a SharedSubplanReference.
   *
   * @param subplan_id The ID of the shared subplan, which is the index of the referenced
   *        shared subplan in <shared_subplans_> of the TopLevelPlan.
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

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_SHARED_SUBPLAN_REFERENCE_HPP_ */
