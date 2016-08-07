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

#ifndef QUICKSTEP_QUERY_EXECUTION_FOREMAN_HPP_
#define QUICKSTEP_QUERY_EXECUTION_FOREMAN_HPP_

#include <cstddef>
#include <cstdio>
#include <memory>
#include <vector>

#include "query_execution/ForemanBase.hpp"
#include "query_execution/PolicyEnforcerSingleNode.hpp"
#include "utility/Macros.hpp"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class CatalogDatabaseLite;
class StorageManager;
class WorkerDirectory;
class WorkerMessage;

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief The Foreman receives queries from the main thread, messages from the
 *        policy enforcer and dispatches the work to worker threads. It also
 *        receives work completion messages from workers.
 **/
class ForemanSingleNode final : public ForemanBase {
 public:
  /**
   * @brief Constructor.
   *
   * @param main_thread_client_id The TMB client ID of the main thread.
   * @param worker_directory The worker directory.
   * @param bus A pointer to the TMB.
   * @param catalog_database The catalog database where this query is executed.
   * @param storage_manager The StorageManager to use.
   * @param cpu_id The ID of the CPU to which the Foreman thread can be pinned.
   * @param num_numa_nodes The number of NUMA nodes in the system.
   * @param profile_individual_workorders Whether every workorder's execution
   *        be profiled or not.
   *
   * @note If cpu_id is not specified, Foreman thread can be possibly moved
   *       around on different CPUs by the OS.
  **/
  ForemanSingleNode(const tmb::client_id main_thread_client_id,
          WorkerDirectory *worker_directory,
          tmb::MessageBus *bus,
          CatalogDatabaseLite *catalog_database,
          StorageManager *storage_manager,
          const int cpu_id = -1,
          const std::size_t num_numa_nodes = 1,
          const bool profile_individual_workorders = false);

  ~ForemanSingleNode() override {}


  /**
   * @brief Get the results of profiling individual work orders for a given
   *        query.
   *
   * @param query_id The ID of the query for which the results are to be printed.
   * @return A vector of tuples, each being a single profiling entry.
   **/
  const std::vector<WorkOrderTimeEntry>& getWorkOrderProfilingResults(
      const std::size_t query_id) const;

  /**
   * @brief Print the results of profiling individual work orders for a given
   *        query.
   *
   * TODO(harshad) - Add the name of the operator to the output.
   * TODO(harshad) - Add the CPU core ID of the operator to the output. This
   * will require modifying the WorkerDirectory to remember worker affinities.
   * Until then, the users can refer to the worker_affinities provided to the
   * cli to infer the CPU core ID where a given worker is pinned.
   *
   * @param query_id The ID of the query for which the results are to be printed.
   * @param out The file stream.
   **/
  void printWorkOrderProfilingResults(const std::size_t query_id,
                                      std::FILE *out) const;

 protected:
  void run() override;

 private:
  /**
   * @brief Dispatch schedulable WorkOrders, wrapped in WorkerMessages to the
   *        worker threads.
   *
   * @param messages The messages to be dispatched.
   **/
  void dispatchWorkerMessages(
      const std::vector<std::unique_ptr<WorkerMessage>> &messages);

  /**
   * @brief Send the given message to the specified worker.
   *
   * @param worker_thread_index The logical index of the recipient worker thread
   *        in WorkerDirectory.
   * @param message The WorkerMessage to be sent.
   **/
  void sendWorkerMessage(const std::size_t worker_thread_index,
                         const WorkerMessage &message);

  /**
   * @brief Check if we can collect new messages from the PolicyEnforcer.
   *
   * @param message_type The type of the last received message.
   **/
  bool canCollectNewMessages(const tmb::message_type_id message_type);

  const tmb::client_id main_thread_client_id_;

  WorkerDirectory *worker_directory_;

  CatalogDatabaseLite *catalog_database_;
  StorageManager *storage_manager_;

  std::unique_ptr<PolicyEnforcerSingleNode> policy_enforcer_;

  DISALLOW_COPY_AND_ASSIGN(ForemanSingleNode);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_FOREMAN_HPP_
