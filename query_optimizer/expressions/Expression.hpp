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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_EXPRESSION_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_EXPRESSION_HPP_

#include <memory>
#include <map>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "types/Type.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

class AttributeReference;
class Expression;

typedef std::shared_ptr<const Expression> ExpressionPtr;

/**
 * @brief Base class for all expressions in the query optimizer.
 */
class Expression : public OptimizerTree<Expression> {
 public:
  /**
   * @brief Destructor.
   */
  ~Expression() override {}

  /**
   * @brief Returns the expression type.
   *
   * @return ExpressionType of this expression.
   */
  virtual ExpressionType getExpressionType() const = 0;

  /**
   * @brief Returns the type of the output.
   *
   * @return The type of the output.
   */
  virtual const Type& getValueType() const = 0;

  /**
   * @brief Returns AttributeReferences referenced by this expression.
   *
   * @return A vector of AttributeReference referenced by this expression.
   */
  virtual std::vector<std::shared_ptr<const AttributeReference>> getReferencedAttributes() const = 0;

  /**
   * @return True if this expression is constant.
   */
  virtual bool isConstant() const = 0;

 protected:
  Expression() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(Expression);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_EXPRESSION_HPP_ */
