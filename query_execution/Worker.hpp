/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_QUERY_EXECUTION_WORKER_HPP_
#define QUICKSTEP_QUERY_EXECUTION_WORKER_HPP_

#include <cstddef>

#include "query_execution/QueryExecutionTypedefs.hpp"
#include "threading/Thread.hpp"
#include "utility/Macros.hpp"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"

namespace quickstep {

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
    bus_->RegisterClientAsSender(worker_client_id_, kWorkOrdersAvailableMessage);
    bus_->RegisterClientAsSender(worker_client_id_, kWorkOrderFeedbackMessage);

    bus_->RegisterClientAsReceiver(worker_client_id_, kWorkOrderMessage);
    bus_->RegisterClientAsReceiver(worker_client_id_, kRebuildWorkOrderMessage);
    bus_->RegisterClientAsReceiver(worker_client_id_, kPoisonMessage);
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
   * @brief Send the response WorkOrder completion message.
   *
   * @param receiver The id of the TMB client which should receive the response.
   * @param op_index The index of the operator to which the WorkOrder belongs.
   * @param query_id The ID of the query which the WorkOrder belongs to.
   * @param is_rebuild_work_order True if it is a RebuildWorkOrder. Otherwise
   *        false.
   **/
  void sendWorkOrderCompleteMessage(const tmb::client_id receiver,
                                    const std::size_t op_index,
                                    const std::size_t query_id,
                                    const bool is_rebuild_work_order);

  const std::size_t worker_thread_index_;
  MessageBus *bus_;

  const int cpu_id_;
  client_id worker_client_id_;

  DISALLOW_COPY_AND_ASSIGN(Worker);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_WORKER_HPP_
