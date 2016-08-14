/**
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#ifndef QUICKSTEP_QUERY_EXECUTION_FOREMAN_DISTRIBUTED_HPP_
#define QUICKSTEP_QUERY_EXECUTION_FOREMAN_DISTRIBUTED_HPP_

#include <cstddef>
#include <cstdio>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/ForemanBase.hpp"
#include "query_execution/PolicyEnforcerDistributed.hpp"
#include "query_execution/ShiftbossDirectory.hpp"
#include "utility/Macros.hpp"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class CatalogDatabaseLite;

namespace serialization { class WorkOrderMessage; }

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief The Foreman receives queries from the main thread, messages from the
 *        policy enforcer and dispatches the work to Shiftbosses. It also
 *        receives work completion messages from Shiftbosses.
 **/
class ForemanDistributed final : public ForemanBase {
 public:
  /**
   * @brief Constructor.
   *
   * @param bus A pointer to the TMB.
   * @param catalog_database The catalog database where this query is executed.
   * @param cpu_id The ID of the CPU to which the Foreman thread can be pinned.
   * @param profile_individual_workorders Whether every workorder's execution
   *        be profiled or not.
   *
   * @note If cpu_id is not specified, Foreman thread can be possibly moved
   *       around on different CPUs by the OS.
  **/
  ForemanDistributed(tmb::MessageBus *bus,
                     CatalogDatabaseLite *catalog_database,
                     const int cpu_id = -1,
                     const bool profile_individual_workorders = false);

  ~ForemanDistributed() override {}

  /**
   * @brief Print the results of profiling individual work orders for a given
   *        query.
   *
   * TODO(harshad) - Add the name of the operator to the output.
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
   * @brief Dispatch schedulable WorkOrders, wrapped in WorkOrderMessages to the
   *        worker threads.
   *
   * @param messages The messages to be dispatched.
   **/
  void dispatchWorkOrderMessages(
      const std::vector<std::unique_ptr<serialization::WorkOrderMessage>> &messages);

  /**
   * @brief Send the given message to the specified worker.
   *
   * @param worker_index The logical index of the recipient worker in
   *        ShiftbossDirectory.
   * @param proto The WorkOrderMessage to be sent.
   **/
  void sendWorkOrderMessage(const std::size_t worker_index,
                            const serialization::WorkOrderMessage &proto);

  void processShiftbossRegistrationMessage(const tmb::client_id shiftboss_client_id,
                                            const std::size_t work_order_capacity);

  void processSaveQueryResultResponseMessage(const tmb::client_id cli_id,
                                             const relation_id result_relation_id);

  /**
   * @brief Check if we can collect new messages from the PolicyEnforcer.
   *
   * @param message_type The type of the last received message.
   **/
  bool canCollectNewMessages(const tmb::message_type_id message_type);

  ShiftbossDirectory shiftboss_directory_;

  CatalogDatabaseLite *catalog_database_;

  std::unique_ptr<PolicyEnforcerDistributed> policy_enforcer_;

  DISALLOW_COPY_AND_ASSIGN(ForemanDistributed);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_FOREMAN_DISTRIBUTED_HPP_
