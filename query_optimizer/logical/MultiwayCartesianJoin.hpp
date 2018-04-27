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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_MULTIWAY_CARTESIAN_JOIN_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_MULTIWAY_CARTESIAN_JOIN_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "query_optimizer/logical/Join.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class MultiwayCartesianJoin;
typedef std::shared_ptr<const MultiwayCartesianJoin> MultiwayCartesianJoinPtr;

/**
 * @brief Produces cartesian product of all input relations. This is used in the
 *        initial and intermediate logical plan only, and is replaced by other
 *        join operators in the final logical plan.
 */
class MultiwayCartesianJoin : public Join {
 public:
  LogicalType getLogicalType() const override {
    return LogicalType::kMultiwayCartesianJoin;
  }

  std::string getName() const override { return "MultiwayCartesianJoin"; }

  /**
   * @return The input operators.
   */
  const std::vector<LogicalPtr>& operands() const { return operands_; }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override;

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override;

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return {};
  }

  /**
   * @brief Creates a MultiwayCartesianJoin with \p operands as input operators.
   *
   * @param operands Input operators.
   * @return An immutable MultiwayCartesianJoin.
   */
  static MultiwayCartesianJoinPtr Create(
      const std::vector<LogicalPtr> &operands) {
    DCHECK_GT(operands.size(), 1u);
    return MultiwayCartesianJoinPtr(new MultiwayCartesianJoin(operands));
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
  explicit MultiwayCartesianJoin(const std::vector<LogicalPtr> &operands)
      : operands_(operands) {
    for (const LogicalPtr &operand : operands) {
      addChild(operand);
    }
  }

  const std::vector<LogicalPtr> operands_;

  DISALLOW_COPY_AND_ASSIGN(MultiwayCartesianJoin);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_MULTIWAY_CARTESIAN_JOIN_HPP_ */
