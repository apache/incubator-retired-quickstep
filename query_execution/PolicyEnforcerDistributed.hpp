/**
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

#ifndef QUICKSTEP_QUERY_EXECUTION_POLICY_ENFORCER_DISTRIBUTED_HPP_
#define QUICKSTEP_QUERY_EXECUTION_POLICY_ENFORCER_DISTRIBUTED_HPP_

#include <cstddef>
#include <memory>
#include <vector>

#include "query_execution/PolicyEnforcerBase.hpp"
#include "query_execution/ShiftbossDirectory.hpp"
#include "utility/Macros.hpp"

#include "tmb/id_typedefs.h"

namespace tmb {
class MessageBus;
class TaggedMessage;
}

namespace quickstep {

class CatalogDatabaseLite;
class QueryHandle;

namespace serialization { class WorkOrderMessage; }

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief A class that ensures that a high level policy is maintained
 *        in sharing resources among concurrent queries.
 **/
class PolicyEnforcerDistributed final : public PolicyEnforcerBase {
 public:
  /**
   * @brief Constructor.
   *
   * @param foreman_client_id The TMB client ID of the Foreman.
   * @param catalog_database The CatalogDatabase used.
   * @param bus The TMB.
   **/
  PolicyEnforcerDistributed(const tmb::client_id foreman_client_id,
                            CatalogDatabaseLite *catalog_database,
                            ShiftbossDirectory *shiftboss_directory,
                            tmb::MessageBus *bus,
                            const bool profile_individual_workorders = false)
      : PolicyEnforcerBase(catalog_database, profile_individual_workorders),
        foreman_client_id_(foreman_client_id),
        shiftboss_directory_(shiftboss_directory),
        bus_(bus) {}

  /**
   * @brief Destructor.
   **/
  ~PolicyEnforcerDistributed() override {}

  bool admitQuery(QueryHandle *query_handle) override;

  /**
   * @brief Get work order messages to be dispatched. These messages come from
   *        the active queries.
   *
   * @param work_order_messages The work order messages to be dispatched.
   **/
  void getWorkOrderMessages(
      std::vector<std::unique_ptr<serialization::WorkOrderMessage>> *work_order_messages);

  /**
   * @brief Process the initiate rebuild work order response message.
   *
   * @param tagged_message The message.
   **/
  void processInitiateRebuildResponseMessage(const tmb::TaggedMessage &tagged_message);

 private:
  void decrementNumQueuedWorkOrders(const std::size_t shiftboss_index) override {
    shiftboss_directory_->decrementNumQueuedWorkOrders(shiftboss_index);
  }

  void onQueryCompletion(QueryHandle *query_handle) override;

  void initiateQueryInShiftboss(QueryHandle *query_handle);

  const tmb::client_id foreman_client_id_;

  ShiftbossDirectory *shiftboss_directory_;

  tmb::MessageBus *bus_;

  DISALLOW_COPY_AND_ASSIGN(PolicyEnforcerDistributed);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_POLICY_ENFORCER_DISTRIBUTED_HPP_
