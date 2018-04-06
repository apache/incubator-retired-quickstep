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

#ifndef QUICKSTEP_STORAGE_DATA_PROVIDER_THREAD_HPP_
#define QUICKSTEP_STORAGE_DATA_PROVIDER_THREAD_HPP_

#include <cstddef>

#include "query_execution/QueryExecutionTypedefs.hpp"
#include "storage/StorageConfig.h"
#include "threading/Thread.hpp"
#include "utility/Macros.hpp"

#include "tmb/native_net_client_message_bus.h"

namespace quickstep {

class CatalogDatabase;
class CatalogRelation;
class QueryProcessor;
class StorageManager;

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief A thread that provides access to query results (e.g. storage blocks),
 *        to Quickstep clients.
 **/
class DataProviderThread : public Thread {
 public:
  /**
   * @brief Constructor.
   *
   * @param database The CatalogDatabase.
   * @param query_processor The QueryProcessor object.
   * @param storage_manager The storage manager.
   * @param bus The TMB.
   * @param cpu_id The ID of the CPU to affinitize this thread to, or -1 to
   *        indicate no affinity.
   **/
  DataProviderThread(CatalogDatabase *database,
                     QueryProcessor *query_processor,
                     StorageManager *storage_manager,
                     tmb::MessageBus *bus,
                     const int cpu_id = -1)
      : database_(database),
        query_processor_(query_processor),
        storage_manager_(storage_manager),
        bus_(bus),
        cpu_id_(cpu_id),
        dpt_client_id_(bus_->Connect()) {
    bus_->RegisterClientAsReceiver(dpt_client_id_, kPoisonMessage);
    bus_->RegisterClientAsReceiver(dpt_client_id_, kBlockRequestMessage);
    bus_->RegisterClientAsReceiver(dpt_client_id_, kFinishReadingRelationMessage);
    bus_->RegisterClientAsSender(dpt_client_id_, kBlockResponseMessage);
  }

  ~DataProviderThread() override {
  }

  tmb::client_id getTMBClientID() const {
    return dpt_client_id_;
  }

 protected:
  void run() override;

 private:
  CatalogDatabase *database_;
  QueryProcessor *query_processor_;
  StorageManager *storage_manager_;
  tmb::MessageBus *bus_;

  const int cpu_id_;

  tmb::client_id dpt_client_id_;

  DISALLOW_COPY_AND_ASSIGN(DataProviderThread);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_DATA_PROVIDER_THREAD_HPP_
