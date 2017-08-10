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

#ifndef QUICKSTEP_QUERY_EXECUTION_WORK_ORDER_SELECTION_POLICY_HPP_
#define QUICKSTEP_QUERY_EXECUTION_WORK_ORDER_SELECTION_POLICY_HPP_

#include <cstddef>
#include <stack>
#include <queue>

#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief Base class for a policy to select work orders for query execution.
 **/
class WorkOrderSelectionPolicy {
 public:
  /**
   * @brief Whether there is an available work order for execution.
   *
   * @return True if a work order is available. Otherwise, false.
   **/
  virtual bool hasWorkOrder() const = 0;

  /**
   * @brief Add work order.
   *
   * @param operator_index The operator index for added work order.
   **/
  virtual void addWorkOrder(const std::size_t operator_index) = 0;

  /**
   * @brief Choose the operator index for next workorder execution based on the policy.
   *
   * @return The operator index chosen for next workorder execution.
   **/
  virtual std::size_t getOperatorIndexForNextWorkOrder() = 0;

 protected:
  /**
   * @brief Constructor.
   **/
  WorkOrderSelectionPolicy() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(WorkOrderSelectionPolicy);
};

/**
 * @brief Choose the next work order in a first-in-first-out manner.
 **/
class FifoWorkOrderSelectionPolicy final : public WorkOrderSelectionPolicy {
 public:
  /**
   * @brief Constructor.
   **/
  FifoWorkOrderSelectionPolicy() = default;

  bool hasWorkOrder() const override {
    return !work_orders_.empty();
  }

  void addWorkOrder(const std::size_t operator_index) override {
    work_orders_.push(operator_index);
  }

  std::size_t getOperatorIndexForNextWorkOrder() override {
    DCHECK(hasWorkOrder());
    const std::size_t operator_index = work_orders_.front();
    work_orders_.pop();

    return operator_index;
  }

 private:
  std::queue<std::size_t> work_orders_;

  DISALLOW_COPY_AND_ASSIGN(FifoWorkOrderSelectionPolicy);
};

/**
 * @brief Choose the next work order in a last-in-first-out manner.
 **/
class LifoWorkOrderSelectionPolicy final : public WorkOrderSelectionPolicy {
 public:
  /**
   * @brief Constructor.
   **/
  LifoWorkOrderSelectionPolicy() = default;

  bool hasWorkOrder() const override {
    return !work_orders_.empty();
  }

  void addWorkOrder(const std::size_t operator_index) override {
    work_orders_.push(operator_index);
  }

  std::size_t getOperatorIndexForNextWorkOrder() override {
    DCHECK(hasWorkOrder());
    const std::size_t operator_index = work_orders_.top();
    work_orders_.pop();

    return operator_index;
  }

 private:
  std::stack<std::size_t> work_orders_;

  DISALLOW_COPY_AND_ASSIGN(LifoWorkOrderSelectionPolicy);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_WORK_ORDER_SELECTION_POLICY_HPP_
