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

#ifndef QUICKSTEP_QUERY_EXECUTION_WORKER_MESSAGE_HPP_
#define QUICKSTEP_QUERY_EXECUTION_WORKER_MESSAGE_HPP_

#include <cstddef>

namespace quickstep {

class WorkOrder;
/**
 * @brief The messages to be sent to the worker from Foreman.
 *
 * @note  This class is copyable.
 **/
class WorkerMessage {
 public:
  static constexpr int kInvalidRecipientIndexHint = -1;

  enum class WorkerMessageType {
    kRebuildWorkOrder = 0,
    kWorkOrder,
  };

  /**
   * @brief A static factory method for generating rebuild WorkOrder messages.
   *
   * @param rebuild_workorder The rebuild WorkOrder to be executed by the worker.
   * @param relational_op_index The index of the relational operator in the
   *        query plan DAG that generated the given rebuild WorkOrder.
   *
   * @return The constructed RebuildWorkOrderMessage.
   **/
  static WorkerMessage* RebuildWorkOrderMessage(WorkOrder *rebuild_workorder, const std::size_t relational_op_index) {
    return new WorkerMessage(rebuild_workorder,
                             relational_op_index,
                             WorkerMessageType::kRebuildWorkOrder);
  }

  /**
   * @brief A static factory method for generating WorkOrder messages.
   *
   * @param workorder The work order to be executed by the worker.
   * @param relational_op_index The index of the relational operator in the
   *                            query plan DAG that generated the given
   *                            workorder.
   *
   * @return The constructed WorkOrderMessage.
   **/
  static WorkerMessage* WorkOrderMessage(WorkOrder *workorder, const std::size_t relational_op_index) {
    return new WorkerMessage(workorder,
                             relational_op_index,
                             WorkerMessageType::kWorkOrder);
  }

  /**
   * @brief Destructor.
   **/
  ~WorkerMessage() {
  }

  /**
   * @brief Gets the work order to be executed by the worker.
   * @return A pointer to the work order which the worker should execute.
   **/
  inline WorkOrder* getWorkOrder() const {
    return work_unit_;
  }

  /**
   * @brief Get the index of the relational operator in the DAG that generated
   *        the workorder.
   **/
  inline std::size_t getRelationalOpIndex() const {
    return relational_op_index_;
  }

  /**
   * @brief Get the type of this WorkerMessage.
   **/
  inline WorkerMessageType getType() const {
    return type_;
  }

  /**
   * @brief Set a hint for the recipient worker thread.
   *
   * @param recipient_index_hint The hint i.e. the worker thread index.
   **/
  inline void setRecipientHint(const int recipient_index_hint) {
    recipient_index_hint_ = recipient_index_hint;
  }

  /**
   * @brief Get the hint for the recipient worker thread. The hint is invalid if
   *        it is kInvalidRecipientIndexHint.
   **/
  inline int getRecipientHint() const {
    return recipient_index_hint_;
  }

 private:
  /**
   * @brief Constructor.
   *
   * @param work_unit The work order to be executed by the worker.
   * @param relational_op_index The index of the relational operator in the
   *        query plan DAG that generated the given WorkOrder.
   * @param type Type of the WorkerMessage.
   **/
  WorkerMessage(WorkOrder *work_unit,
                const std::size_t relational_op_index,
                const WorkerMessageType type)
      : work_unit_(work_unit),
        relational_op_index_(relational_op_index),
        type_(type),
        recipient_index_hint_(kInvalidRecipientIndexHint) {}

  WorkOrder *work_unit_;
  const std::size_t relational_op_index_;
  const WorkerMessageType type_;
  int recipient_index_hint_;
};

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_WORKER_MESSAGE_HPP_
