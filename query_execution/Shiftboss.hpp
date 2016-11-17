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

#ifndef QUICKSTEP_QUERY_EXECUTION_SHIFTBOSS_HPP_
#define QUICKSTEP_QUERY_EXECUTION_SHIFTBOSS_HPP_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <unordered_map>

#include "catalog/CatalogDatabaseCache.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/WorkerDirectory.hpp"
#include "threading/Thread.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/address.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"

namespace quickstep {

class StorageManager;

namespace serialization {
class CatalogDatabase;
class QueryContext;
}  // namespace serialization

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief The Shiftboss accepts workorder protos from shiftboss, and assigns
 *        the workorders to workers.
 **/
class Shiftboss : public Thread {
 public:
  /**
   * @brief Constructor.
   *
   * @param bus A pointer to the TMB.
   * @param storage_manager The StorageManager to use.
   * @param workers A pointer to the WorkerDirectory.
   * @param cpu_id The ID of the CPU to which the Shiftboss thread can be pinned.
   *
   * @note If cpu_id is not specified, Shiftboss thread can be possibly moved
   *       around on different CPUs by the OS.
  **/
  Shiftboss(tmb::MessageBus *bus,
            StorageManager *storage_manager,
            WorkerDirectory *workers,
            const int cpu_id = -1)
      : bus_(DCHECK_NOTNULL(bus)),
        storage_manager_(DCHECK_NOTNULL(storage_manager)),
        workers_(DCHECK_NOTNULL(workers)),
        cpu_id_(cpu_id),
        shiftboss_client_id_(tmb::kClientIdNone),
        foreman_client_id_(tmb::kClientIdNone),
        max_msgs_per_worker_(1),
        start_worker_index_(0u) {
    // Check to have at least one Worker.
    DCHECK_GT(workers->getNumWorkers(), 0u);

    shiftboss_client_id_ = bus_->Connect();
    LOG(INFO) << "Shiftboss TMB client ID: " << shiftboss_client_id_;
    DCHECK_NE(shiftboss_client_id_, tmb::kClientIdNone);

    // Messages between Foreman and Shiftboss.
    bus_->RegisterClientAsSender(shiftboss_client_id_, kShiftbossRegistrationMessage);
    bus_->RegisterClientAsReceiver(shiftboss_client_id_, kShiftbossRegistrationResponseMessage);

    bus_->RegisterClientAsReceiver(shiftboss_client_id_, kQueryInitiateMessage);
    bus_->RegisterClientAsSender(shiftboss_client_id_, kQueryInitiateResponseMessage);

    bus_->RegisterClientAsReceiver(shiftboss_client_id_, kInitiateRebuildMessage);
    bus_->RegisterClientAsSender(shiftboss_client_id_, kInitiateRebuildResponseMessage);

    bus_->RegisterClientAsReceiver(shiftboss_client_id_, kSaveQueryResultMessage);
    bus_->RegisterClientAsSender(shiftboss_client_id_, kSaveQueryResultResponseMessage);

    // Message sent to Worker.
    bus_->RegisterClientAsSender(shiftboss_client_id_, kRebuildWorkOrderMessage);

    // Forward the following message types from Foreman to Workers.
    bus_->RegisterClientAsReceiver(shiftboss_client_id_, kWorkOrderMessage);
    bus_->RegisterClientAsSender(shiftboss_client_id_, kWorkOrderMessage);

    // Forward the following message types from Workers to Foreman.
    bus_->RegisterClientAsReceiver(shiftboss_client_id_, kCatalogRelationNewBlockMessage);
    bus_->RegisterClientAsSender(shiftboss_client_id_, kCatalogRelationNewBlockMessage);

    bus_->RegisterClientAsReceiver(shiftboss_client_id_, kDataPipelineMessage);
    bus_->RegisterClientAsSender(shiftboss_client_id_, kDataPipelineMessage);

    bus_->RegisterClientAsReceiver(shiftboss_client_id_, kWorkOrderFeedbackMessage);
    bus_->RegisterClientAsSender(shiftboss_client_id_, kWorkOrderFeedbackMessage);

    bus_->RegisterClientAsReceiver(shiftboss_client_id_, kWorkOrderCompleteMessage);
    bus_->RegisterClientAsSender(shiftboss_client_id_, kWorkOrderCompleteMessage);

    bus_->RegisterClientAsReceiver(shiftboss_client_id_, kRebuildWorkOrderCompleteMessage);
    bus_->RegisterClientAsSender(shiftboss_client_id_, kRebuildWorkOrderCompleteMessage);

    // Clean up query execution states, i.e., QueryContext.
    bus_->RegisterClientAsReceiver(shiftboss_client_id_, kQueryTeardownMessage);

    // Stop itself.
    bus_->RegisterClientAsReceiver(shiftboss_client_id_, kPoisonMessage);
    // Stop all workers.
    bus_->RegisterClientAsSender(shiftboss_client_id_, kPoisonMessage);

    for (std::size_t i = 0; i < workers_->getNumWorkers(); ++i) {
      worker_addresses_.AddRecipient(workers_->getClientID(i));
    }

    registerWithForeman();
  }

  ~Shiftboss() override {
  }

  /**
   * @brief Get the TMB client ID of Shiftboss thread.
   *
   * @return TMB client ID of shiftboss thread.
   **/
  inline tmb::client_id getBusClientID() const {
    return shiftboss_client_id_;
  }

  /**
   * @brief Get the Work Order processing capacity of all Workers managed by
   *        Shiftboss during a single round of WorkOrder dispatch.
   **/
  inline std::size_t getWorkOrderCapacity() const {
    DCHECK_NE(max_msgs_per_worker_, 0u);
    return max_msgs_per_worker_ * workers_->getNumWorkers();
  }

  /**
   * @brief Get the Worker to assign WorkOrders for execution. Block to wait if
   *        all Workers have reached their capacity for queued WorkOrders.
   **/
  // TODO(zuyu): To achieve non-blocking, we need a queue to cache received
  // normal Work Order protos from Foreman and the generated rebuild Work Orders.
  inline std::size_t getSchedulableWorker();

  /**
   * @brief Set the maximum number of messages that should be allocated to each
   *        worker during a single round of WorkOrder dispatch.
   *
   * @param max_msgs_per_worker Maximum number of messages.
   **/
  inline void setMaxMessagesPerWorker(const std::size_t max_msgs_per_worker) {
    max_msgs_per_worker_ = max_msgs_per_worker;
  }

 protected:
  /**
   * @brief The shiftboss receives workorders, and based on the response it
   *        assigns workorders to workers.
   *
   * @note  The workers who get the messages from the Shiftboss execute and
   *        subsequently delete the WorkOrder contained in the message.
   **/
  void run() override;

 private:
  void registerWithForeman();

  void processShiftbossRegistrationResponseMessage();

  /**
   * @brief Process the Shiftboss initiate message and ack back.
   *
   * @param query_id The given query id.
   * @param catalog_database_cache_proto The proto used to update
   *        CatalogDatabaseCache.
   * @param query_context_proto The QueryContext proto.
   **/
  void processQueryInitiateMessage(const std::size_t query_id,
                                   const serialization::CatalogDatabase &catalog_database_cache_proto,
                                   const serialization::QueryContext &query_context_proto);

  /**
   * @brief Process the RebuildWorkOrder initiate message and ack back.
   *
   * @param query_id The ID of the query to which this RebuildWorkOrder initiate
   *        message belongs.
   * @param op_index The index of the operator for rebuild work orders.
   * @param dest_index The InsertDestination index in QueryContext to rebuild.
   * @param rel_id The relation that needs to generate rebuild work orders.
   **/
  void processInitiateRebuildMessage(const std::size_t query_id,
                                     const std::size_t op_index,
                                     const QueryContext::insert_destination_id dest_index,
                                     const relation_id rel_id);

  // TODO(zuyu): Use two buses for the message communication between Foreman and Shiftboss,
  // and Shiftboss and Worker thread pool.
  tmb::MessageBus *bus_;

  CatalogDatabaseCache database_cache_;
  StorageManager *storage_manager_;
  WorkerDirectory *workers_;

  // The ID of the CPU that the Shiftboss thread can optionally be pinned to.
  const int cpu_id_;

  tmb::client_id shiftboss_client_id_, foreman_client_id_;

  // Unique per Shiftboss instance.
  std::uint64_t shiftboss_index_;

  // TMB recipients for all workers managed by this Shiftboss.
  tmb::Address worker_addresses_;

  // During a single round of WorkOrder dispatch, a Worker should be allocated
  // at most these many WorkOrders.
  std::size_t max_msgs_per_worker_;

  // The worker index for scheduling Work Order.
  std::size_t start_worker_index_;

  // QueryContexts per query.
  std::unordered_map<std::size_t, std::unique_ptr<QueryContext>> query_contexts_;

  DISALLOW_COPY_AND_ASSIGN(Shiftboss);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_SHIFTBOSS_HPP_
