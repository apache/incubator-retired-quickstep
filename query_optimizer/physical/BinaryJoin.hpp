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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_BINARY_JOIN_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_BINARY_JOIN_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/physical/Join.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

class BinaryJoin;
typedef std::shared_ptr<const BinaryJoin> BinaryJoinPtr;

/**
 * @brief Base class for binary join nodes.
 */
class BinaryJoin : public Join {
 public:
  /**
   * @brief Destructor.
   */
  ~BinaryJoin() override {}

  /**
   * @return The left operand.
   */
  const PhysicalPtr& left() const { return left_; }

  /**
   * @return The right operand.
   */
  const PhysicalPtr& right() const { return right_; }

 protected:
  /**
   * @brief Constructor.
   *
   * @param left The left operand.
   * @param right The right operand.
   * @param project_expressions The project expressions.
   */
  BinaryJoin(const PhysicalPtr &left,
             const PhysicalPtr &right,
             const std::vector<expressions::NamedExpressionPtr> &project_expressions)
      : Join(project_expressions),
        left_(left),
        right_(right) {
    addChild(left_);
    addChild(right_);
  }

  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

 private:
  PhysicalPtr left_;
  PhysicalPtr right_;

  DISALLOW_COPY_AND_ASSIGN(BinaryJoin);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_BINARY_JOIN_HPP_ */
