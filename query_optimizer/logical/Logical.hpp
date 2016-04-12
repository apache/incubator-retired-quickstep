/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_LOGICAL_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_LOGICAL_HPP_

#include <memory>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class Logical;
typedef std::shared_ptr<const Logical> LogicalPtr;

/**
 * @brief Base class for all logical operator/node.
 */
class Logical : public OptimizerTree<Logical>,
                public std::enable_shared_from_this<const Logical> {
 public:
  /**
   * @brief Destructor.
   */
  ~Logical() {}

  /**
   * @return The type of this logical operator.
   */
  virtual LogicalType getLogicalType() const = 0;

  /**
   * @return The references to the output attributes produced by this logical
   * operator.
   */
  virtual std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const = 0;

  /**
   * @return The references to the input attributes used in this logical
   * operator.
   */
  virtual std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const = 0;

  /**
   * @brief Make a copy of this logical node with input expressions replaced by
   *        input_expression.
   *
   * @return A copy of this logical node with the new input expressions.
   */
  virtual LogicalPtr copyWithNewInputExpressions(
      const std::vector<expressions::ExpressionPtr> &input_expressions) const {
    // Default implementation: return a shared reference to "this" when there is
    // no change.
    DCHECK(input_expressions.empty());
    return shared_from_this();
  }

  /**
   * @return The input expressions in this Logical.
   */
  const std::vector<expressions::ExpressionPtr>& input_expressions() const {
    return input_expressions_;
  }

  /**
   * @return The input attributes, which are the union set of the output attributes of
   *         all children.
   */
  std::vector<expressions::AttributeReferencePtr> getInputAttributes() const {
    std::vector<expressions::AttributeReferencePtr> input_attributes;
    for (const LogicalPtr &child : children()) {
      const std::vector<expressions::AttributeReferencePtr> child_output_attributes =
          child->getOutputAttributes();
      input_attributes.insert(input_attributes.end(),
                              child_output_attributes.begin(),
                              child_output_attributes.end());
    }
    return input_attributes;
  }

 protected:
  /**
   * @brief Constructor.
   */
  Logical() {}

  template <class ExpressionClass>
  void addInputExpressions(const std::vector<ExpressionClass> &new_input_expressions) {
    input_expressions_.insert(input_expressions_.end(),
                              new_input_expressions.begin(),
                              new_input_expressions.end());
  }

  void addInputExpression(const expressions::ExpressionPtr &new_input_expression) {
    input_expressions_.emplace_back(new_input_expression);
  }

  std::vector<expressions::ExpressionPtr> input_expressions_;

 private:
  DISALLOW_COPY_AND_ASSIGN(Logical);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_LOGICAL_HPP_ */
