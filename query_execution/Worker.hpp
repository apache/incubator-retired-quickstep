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

#ifndef QUICKSTEP_QUERY_EXECUTION_WORKER_HPP_
#define QUICKSTEP_QUERY_EXECUTION_WORKER_HPP_

#include <cstddef>
#include <cstdint>

#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_optimizer/QueryOptimizerConfig.h"  // For QUICKSTEP_DISTRIBUTED.
#include "threading/Thread.hpp"
#include "utility/Macros.hpp"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"

namespace tmb { class TaggedMessge; }

namespace quickstep {

namespace serialization { class WorkOrderCompletionMessage; }

/** \addtogroup QueryExecution
 *  @{
 */

 /**
  * @brief A worker thread executes WorkOrders which it receives from Foreman.
  **/
class Worker : public Thread {
 public:
  /**
   * @brief Constructor
   *
   * @param worker_thread_index The unique index of this worker thread in
   *        WorkerDirectory.
   * @param bus A pointer to the TMB.
   * @param cpu_id The ID of the CPU to which this worker thread can be pinned.
   *
   * @note If cpu_id is not specified, worker thread can be possibly moved
   *       around on different CPUs by the OS.
   **/
  Worker(const std::size_t worker_thread_index,
         MessageBus *bus,
         int cpu_id = -1)
      : worker_thread_index_(worker_thread_index),
        bus_(bus),
        cpu_id_(cpu_id) {
    DEBUG_ASSERT(bus_ != nullptr);
    worker_client_id_ = bus_->Connect();

    bus_->RegisterClientAsSender(worker_client_id_, kWorkOrderCompleteMessage);
    bus_->RegisterClientAsSender(worker_client_id_,
                                 kRebuildWorkOrderCompleteMessage);
    bus_->RegisterClientAsSender(worker_client_id_, kCatalogRelationNewBlockMessage);
    bus_->RegisterClientAsSender(worker_client_id_, kDataPipelineMessage);
    bus_->RegisterClientAsSender(worker_client_id_, kWorkOrderFeedbackMessage);

    bus_->RegisterClientAsReceiver(worker_client_id_, kWorkOrderMessage);
    bus_->RegisterClientAsReceiver(worker_client_id_, kRebuildWorkOrderMessage);
    bus_->RegisterClientAsReceiver(worker_client_id_, kPoisonMessage);

#ifdef QUICKSTEP_DISTRIBUTED
    bus_->RegisterClientAsReceiver(worker_client_id_, kShiftbossRegistrationResponseMessage);
#endif  // QUICKSTEP_DISTRIBUTED
  }

  ~Worker() override {}

  /**
   * @brief Get the TMB client ID of this worker.
   *
   * @return TMB client ID of this worker.
   **/
  inline client_id getBusClientID() const {
    return worker_client_id_;
  }

 protected:
  /**
   * @brief Receive workorder one at a time and execute it. After execution, send
   *        a completion message to the Foreman and delete the WorkOrder.
   *
   * @note  On receiving a poison message, the method terminates.
   **/
  void run() override;

 private:
  /**
   * @brief A helper method to execute the WorkOrder and construct a
   *        completion message.
   *
   * @note Right now a single helper method works for all message types.
   *       If different message types need to collect different statistics for
   *       the WorkOrder execution, we need to create different helper methods,
   *       one for each message type.
   *
   * @param tagged_message The TaggedMessage which consists of the WorkOrder.
   * @param proto The proto message to be sent.
   * @param is_rebuild_work_order Whether it is used for a RebuildWorkOrder.
   **/
  void executeWorkOrderHelper(const TaggedMessage &tagged_message,
                              serialization::WorkOrderCompletionMessage *proto,
                              const bool is_rebuild_work_order = false);

  /**
   * @brief A helper method to send the WorkOrder completion message.
   *
   * @param receiver The TMB client ID of the receiver.
   * @param proto The proto message to be sent.
   * @param message_type The ID of the type of the message being sent.
   **/
  void sendWorkOrderCompleteMessage(const tmb::client_id receiver,
                                    const serialization::WorkOrderCompletionMessage &proto,
                                    const message_type_id message_type);

  const std::size_t worker_thread_index_;
  MessageBus *bus_;

  const int cpu_id_;
  client_id worker_client_id_;

#ifdef QUICKSTEP_DISTRIBUTED
  std::size_t shiftboss_index_;
#endif  // QUICKSTEP_DISTRIBUTED

  DISALLOW_COPY_AND_ASSIGN(Worker);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_WORKER_HPP_
