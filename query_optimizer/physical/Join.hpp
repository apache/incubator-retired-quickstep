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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_JOIN_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_JOIN_HPP_

#include <memory>
#include <vector>

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

class Join;
typedef std::shared_ptr<const Join> JoinPtr;

struct PartitionSchemeHeader;

/**
 * @brief Base class for physical join nodes.
 */
class Join : public Physical {
 public:
  /**
   * @brief Destructor.
   */
  ~Join() override {}

  /**
   * @brief Gets the project expressions.
   *
   * @return The list of project expressions.
   */
  const std::vector<expressions::NamedExpressionPtr>& project_expressions() const {
    return project_expressions_;
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return expressions::ToRefVector(project_expressions_);
  }

 protected:
  /**
   * @brief Constructor.
   *
   * @param project_expressions The project expressions.
   * @param partition_scheme_header The optional output partition scheme header.
   */
  explicit Join(
      const std::vector<expressions::NamedExpressionPtr>& project_expressions,
      PartitionSchemeHeader *partition_scheme_header = nullptr)
      : Physical(partition_scheme_header),
        project_expressions_(project_expressions) {}

 private:
  std::vector<expressions::NamedExpressionPtr> project_expressions_;

  DISALLOW_COPY_AND_ASSIGN(Join);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_JOIN_HPP_ */
