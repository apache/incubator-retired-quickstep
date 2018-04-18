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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_UNION_ALL_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_UNION_ALL_HPP_

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "utility/Cast.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

class UnionAll;
typedef std::shared_ptr<const UnionAll> UnionAllPtr;

/**
 * @brief Perform Union All operation on query results.
 */
class UnionAll : public Physical {
 public:
  ~UnionAll() override {}

  PhysicalType getPhysicalType() const override {
    return PhysicalType::kUnionAll;
  }

  std::string getName() const override {
    return "UnionAll";
  }

  /**
   * @return The operands for the UnionALl operator.
   */
  const std::vector<PhysicalPtr>& operands() const {
    return operands_;
  }

  PhysicalPtr copyWithNewChildren(
      const std::vector<PhysicalPtr> &new_children) const override {
    return Create(new_children, project_attributes_);
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return project_attributes_;
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    std::vector<expressions::AttributeReferencePtr> referenced_attributes;
    for (const PhysicalPtr &operand : operands_) {
      const std::vector<expressions::AttributeReferencePtr> reference =
          operand->getOutputAttributes();
      referenced_attributes.insert(referenced_attributes.end(),
                                   reference.begin(),
                                   reference.end());
    }
    return referenced_attributes;
  }

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const override {
    std::vector<std::size_t> expression_index;
    for (std::size_t i = 0; i < project_attributes_.size(); ++i) {
      if (referenced_expressions.find(project_attributes_[i]) != referenced_expressions.end()) {
        expression_index.push_back(i);
      }
    }

    if (expression_index.size() == project_attributes_.size()) {
      return false;
    }

    std::vector<PhysicalPtr> new_operands;
    for (std::size_t i = 0; i < operands_.size(); ++i) {
      // Currently only prune when all children are select.
      if (operands_[i]->getPhysicalType() != PhysicalType::kSelection) {
        return false;
      }

      const auto child_output_attrs = operands_[i]->getOutputAttributes();
      expressions::UnorderedNamedExpressionSet child_referenced_attrs;
      for (const std::size_t idx : expression_index) {
        child_referenced_attrs.emplace(child_output_attrs[idx]);
      }
      PhysicalPtr new_operand;
      if (!operands_[i]->maybeCopyWithPrunedExpressions(
              child_referenced_attrs, &new_operand)) {
          return false;
      }
      DCHECK_EQ(expression_index.size(), new_operand->getOutputAttributes().size());
      new_operands.push_back(new_operand);
    }
    std::vector<expressions::AttributeReferencePtr> new_project_attributes;
    for (const std::size_t idx : expression_index) {
      new_project_attributes.emplace_back(project_attributes_[idx]);
    }
    *output = Create(new_operands, new_project_attributes);
    return true;
  }

  const std::vector<expressions::AttributeReferencePtr>& project_attributes() const {
    return project_attributes_;
  }

  /**
   * @brief Creates the physical node of UnionAll.
   *
   * @param operands The children physical nodes of UnionAll.
   * @param project_attributes The project attributes of this UnionAll.
   * @return An immutable UnionAll node.
   */
  static UnionAllPtr Create(
      const std::vector<PhysicalPtr> &operands,
      const std::vector<expressions::AttributeReferencePtr> &project_attributes) {
    return UnionAllPtr(
        new UnionAll(operands, project_attributes));
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override {
    container_child_field_names->emplace_back("operands");
    container_child_fields->emplace_back(
        CastSharedPtrVector<OptimizerTreeBase>(operands_));

    container_child_field_names->emplace_back("project_attributes");
    container_child_fields->emplace_back(
        CastSharedPtrVector<OptimizerTreeBase>(project_attributes_));
  }

 private:
  UnionAll(const std::vector<PhysicalPtr> &operands,
           const std::vector<expressions::AttributeReferencePtr> &project_attributes)
      : operands_(operands),
        project_attributes_(project_attributes) {
    for (const PhysicalPtr &operand : operands) {
      addChild(operand);
    }
  }

  const std::vector<PhysicalPtr> operands_;
  const std::vector<expressions::AttributeReferencePtr> project_attributes_;

  DISALLOW_COPY_AND_ASSIGN(UnionAll);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_UNION_ALL_HPP_
