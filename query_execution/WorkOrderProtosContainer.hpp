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

#ifndef QUICKSTEP_QUERY_EXECUTION_WORKORDER_PROTOS_CONTAINER_HPP_
#define QUICKSTEP_QUERY_EXECUTION_WORKORDER_PROTOS_CONTAINER_HPP_

#include <cstddef>
#include <memory>
#include <queue>
#include <vector>

#include "relational_operators/WorkOrder.pb.h"  // IWYU pragma: keep
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup QueryExecution
 *  @{
 */

 /**
  * @brief A container used in the distributed version to hold the normal
  *        (non-rebuild) WorkOrder protos for a given query.
  *
  * @note This container stays alive during the lifetime of the query.
  **/
class WorkOrderProtosContainer {
 public:
  /**
   * @brief Constructor
   *
   * @param num_operators Number of operators in the query DAG.
   **/
  explicit WorkOrderProtosContainer(const std::size_t num_operators)
      : num_operators_(num_operators),
        operator_containers_(num_operators_) {
    DCHECK_NE(num_operators_, 0u);
  }

  /**
   * @brief Destructor.
   *
   * @note If the query is executed normally, we should never encounter a
   *       situation where at the time of deletion the WorkOrderProtosContainer has
   *       pending WorkOrders.
   **/
  ~WorkOrderProtosContainer() {
    for (std::size_t op = 0; op < num_operators_; ++op) {
      if (hasWorkOrderProto(op)) {
        LOG(WARNING) << "Destroying a WorkOrderProtosContainer that still has pending WorkOrder protos.";
        break;
      }
    }
  }

  /**
   * @brief Check if there are some pending WorkOrders for the given operator.
   *
   * @param operator_index Index of the operator.
   *
   * @return If there are pending WorkOrders.
   **/
  bool hasWorkOrderProto(const std::size_t operator_index) const {
    DCHECK_LT(operator_index, num_operators_);
    return !operator_containers_[operator_index].empty();
  }

  /**
   * @brief Get a WorkOrder for a given operator.
   *
   * @param operator_index The index of the operator.
   *
   * @return Release a WorkOrder proto. If no WorkOrder proto is available,
   *         return nullptr.
   **/
  serialization::WorkOrder* getWorkOrderProto(const std::size_t operator_index) {
    DCHECK_LT(operator_index, num_operators_);

    if (operator_containers_[operator_index].empty()) {
      return nullptr;
    }

    serialization::WorkOrder *proto =
        operator_containers_[operator_index].front().release();
    operator_containers_[operator_index].pop();

    return proto;
  }

  /**
   * @brief Add a WorkOrder generated from a given
   *        operator.
   *
   * @param workorder A pointer to the WorkOrder to be added.
   * @param operator_index The index of the operator in the query DAG.
   **/
  void addWorkOrderProto(serialization::WorkOrder *proto,
                         const std::size_t operator_index) {
    DCHECK(proto != nullptr);
    DCHECK_LT(operator_index, num_operators_);

    operator_containers_[operator_index].emplace(
        std::unique_ptr<serialization::WorkOrder>(proto));
  }

  /**
   * @brief Get the number of all pending WorkOrders
   *        for a given operator.
   *
   * @param operator_index The index of the operator.
   *
   * @return The number of pending WorkOrders.
   **/
  std::size_t getNumWorkOrderProtos(const std::size_t operator_index) const {
    DCHECK_LT(operator_index, num_operators_);
    return operator_containers_[operator_index].size();
  }

 private:
  const std::size_t num_operators_;

  std::vector<std::queue<std::unique_ptr<serialization::WorkOrder>>> operator_containers_;

  DISALLOW_COPY_AND_ASSIGN(WorkOrderProtosContainer);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_WORKORDER_PROTOS_CONTAINER_HPP_
