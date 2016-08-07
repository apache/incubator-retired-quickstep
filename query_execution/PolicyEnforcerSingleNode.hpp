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

#ifndef QUICKSTEP_QUERY_EXECUTION_POLICY_ENFORCER_SINGLE_NODE_HPP_
#define QUICKSTEP_QUERY_EXECUTION_POLICY_ENFORCER_SINGLE_NODE_HPP_

#include <cstddef>
#include <memory>
#include <vector>

#include "query_execution/PolicyEnforcerBase.hpp"
#include "utility/Macros.hpp"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class CatalogDatabaseLite;
class QueryHandle;
class StorageManager;
class WorkerDirectory;
class WorkerMessage;

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief A class that ensures that a high level policy is maintained
 *        in sharing resources among concurrent queries.
 **/
class PolicyEnforcerSingleNode final : public PolicyEnforcerBase {
 public:
  /**
   * @brief Constructor.
   *
   * @param foreman_client_id The TMB client ID of the Foreman.
   * @param num_numa_nodes Number of NUMA nodes used by the system.
   * @param catalog_database The CatalogDatabase used.
   * @param storage_manager The StorageManager used.
   * @param bus The TMB.
   **/
  PolicyEnforcerSingleNode(const tmb::client_id foreman_client_id,
                           const std::size_t num_numa_nodes,
                           CatalogDatabaseLite *catalog_database,
                           StorageManager *storage_manager,
                           WorkerDirectory *worker_directory,
                           tmb::MessageBus *bus,
                           const bool profile_individual_workorders = false)
      : PolicyEnforcerBase(catalog_database, profile_individual_workorders),
        foreman_client_id_(foreman_client_id),
        num_numa_nodes_(num_numa_nodes),
        storage_manager_(storage_manager),
        worker_directory_(worker_directory),
        bus_(bus) {}

  /**
   * @brief Destructor.
   **/
  ~PolicyEnforcerSingleNode() override {}

  bool admitQuery(QueryHandle *query_handle) override;

  /**
   * @brief Get worker messages to be dispatched. These worker messages come
   *        from the active queries.
   *
   * @param worker_messages The worker messages to be dispatched.
   **/
  void getWorkerMessages(
      std::vector<std::unique_ptr<WorkerMessage>> *worker_messages);

 private:
  void decrementNumQueuedWorkOrders(const std::size_t worker_index) override;

  const tmb::client_id foreman_client_id_;
  const std::size_t num_numa_nodes_;

  StorageManager *storage_manager_;
  WorkerDirectory *worker_directory_;

  tmb::MessageBus *bus_;

  DISALLOW_COPY_AND_ASSIGN(PolicyEnforcerSingleNode);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_POLICY_ENFORCER_SINGLE_NODE_HPP_
