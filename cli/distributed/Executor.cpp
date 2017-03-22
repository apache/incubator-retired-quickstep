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

#include "cli/distributed/Executor.hpp"

#include <cstddef>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "cli/Flags.hpp"
#include "cli/InputParserUtil.hpp"
#include "query_execution/BlockLocatorUtil.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/Shiftboss.hpp"
#include "query_execution/Worker.hpp"
#include "query_execution/WorkerDirectory.hpp"
#include "storage/DataExchangerAsync.hpp"
#include "storage/StorageManager.hpp"

#include "tmb/id_typedefs.h"
#include "tmb/native_net_client_message_bus.h"
#include "tmb/pure_memory_message_bus.h"

#include "glog/logging.h"

using std::make_unique;
using std::size_t;
using std::vector;

using tmb::client_id;

namespace quickstep {

void Executor::init() {
  bus_local_.Initialize();

  executor_client_id_ = bus_.Connect();
  DLOG(INFO) << "Executor TMB Client ID: " << executor_client_id_;

  bus_.RegisterClientAsSender(executor_client_id_, kBlockDomainRegistrationMessage);
  bus_.RegisterClientAsReceiver(executor_client_id_, kBlockDomainRegistrationResponseMessage);

  // Parse the CPU affinities for workers and the preloader thread, if enabled
  // to warm up the buffer pool.
  const vector<int> worker_cpu_affinities =
      InputParserUtil::ParseWorkerAffinities(FLAGS_num_workers, FLAGS_worker_affinities);

  const vector<numa_node_id> worker_numa_nodes(FLAGS_num_workers, kAnyNUMANodeID);
  vector<client_id> worker_client_ids;

  for (int worker_thread_index = 0;
       worker_thread_index < FLAGS_num_workers;
       ++worker_thread_index) {
    workers_.push_back(make_unique<Worker>(worker_thread_index, &bus_local_,
                                           worker_cpu_affinities[worker_thread_index]));
    worker_client_ids.push_back(workers_.back()->getBusClientID());
  }

  worker_directory_ =
      make_unique<WorkerDirectory>(worker_client_ids.size(), worker_client_ids, worker_numa_nodes);

  client_id locator_client_id;
  storage_manager_ = make_unique<StorageManager>(
      FLAGS_storage_path,
      block_locator::getBlockDomain(data_exchanger_.network_address(), executor_client_id_, &locator_client_id, &bus_),
      locator_client_id, &bus_);

  data_exchanger_.set_storage_manager(storage_manager_.get());
  data_exchanger_.start();

  shiftboss_ =
      make_unique<Shiftboss>(&bus_, &bus_local_, storage_manager_.get(), worker_directory_.get(),
                             storage_manager_->hdfs());
  shiftboss_->start();

  for (const auto &worker : workers_) {
    worker->start();
  }
}

}  // namespace quickstep
