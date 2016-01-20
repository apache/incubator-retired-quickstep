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

#ifndef QUICKSTEP_QUERY_EXECUTION_WORKER_HPP_
#define QUICKSTEP_QUERY_EXECUTION_WORKER_HPP_

#include <cstddef>
#include <memory>

#include "query_execution/QueryExecutionTypedefs.hpp"
#include "threading/Thread.hpp"
#include "utility/Macros.hpp"

#include "tmb/message_bus.h"

namespace quickstep {

class CatalogDatabase;
class QueryContext;
class StorageManager;

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
   * @param worker_id The unique ID of this worker thread.
   * @param query_context The QueryContext that stores query execution states.
   * @param bus A pointer to the TMB.
   * @param catalog_database The catalog database where this query is executed.
   * @param storage_manager The StorageManager to use.
   * @param cpu_id The ID of the CPU to which this worker thread can be pinned.
   *
   * @note If cpu_id is not specified, worker thread can be possibly moved
   *       around on different CPUs by the OS.
   **/
  Worker(std::size_t worker_id,
         const std::unique_ptr<QueryContext> &query_context,
         MessageBus *bus,
         CatalogDatabase *catalog_database,
         StorageManager *storage_manager,
         int cpu_id = -1)
      : worker_id_(worker_id),
        query_context_(query_context),
        bus_(bus),
        catalog_database_(catalog_database),
        storage_manager_(storage_manager),
        cpu_id_(cpu_id) {
    DEBUG_ASSERT(bus_ != nullptr);
    worker_client_id_ = bus_->Connect();

    bus_->RegisterClientAsSender(worker_client_id_, kWorkOrderCompleteMessage);
    bus_->RegisterClientAsSender(worker_client_id_,
                                 kRebuildWorkOrderCompleteMessage);
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
  const std::size_t worker_id_;
  const std::unique_ptr<QueryContext> &query_context_;
  MessageBus *bus_;

  CatalogDatabase *catalog_database_;
  StorageManager *storage_manager_;

  const int cpu_id_;
  client_id worker_client_id_;

  DISALLOW_COPY_AND_ASSIGN(Worker);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_WORKER_HPP_
