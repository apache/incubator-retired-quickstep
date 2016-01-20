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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_PROJECT_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_PROJECT_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class Project;
typedef std::shared_ptr<const Project> ProjectPtr;

/**
 * @brief Project operator that computes the values of a list of expressions for
 *        each input tuple and outputs them as a new relation.
 */
class Project : public Logical {
 public:
  LogicalType getLogicalType() const override { return LogicalType::kProject; }

  std::string getName() const override { return "Project"; }

  /**
   * @return The expressions to be evaluated by the Project.
   */
  const std::vector<expressions::NamedExpressionPtr>& project_expressions() const {
    return project_expressions_;
  }

  /**
   * @return The input operator.
   */
  const LogicalPtr& input() const { return input_; }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override;

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override;

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override;

  /**
   * @brief Creates a Project that computes \p project_expressions on the
   *        relation produced by \p input.
   *
   * @param input The input operator to this Project.
   * @param project_expressions The project expressions.
   * @return An immutable Project.
   */
  static ProjectPtr Create(
      const LogicalPtr &input,
      const std::vector<expressions::NamedExpressionPtr> &project_expressions) {
    return ProjectPtr(new Project(input, project_expressions));
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
  Project(
      const LogicalPtr &input,
      const std::vector<expressions::NamedExpressionPtr> &project_expressions)
      : input_(input), project_expressions_(project_expressions) {
    addChild(input);
  }

  LogicalPtr input_;
  std::vector<expressions::NamedExpressionPtr> project_expressions_;

  DISALLOW_COPY_AND_ASSIGN(Project);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_PROJECT_HPP_ */
