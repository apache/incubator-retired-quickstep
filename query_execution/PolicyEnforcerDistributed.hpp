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

#ifndef QUICKSTEP_QUERY_EXECUTION_POLICY_ENFORCER_DISTRIBUTED_HPP_
#define QUICKSTEP_QUERY_EXECUTION_POLICY_ENFORCER_DISTRIBUTED_HPP_

#include <cstddef>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/PolicyEnforcerBase.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryManagerBase.hpp"
#include "query_execution/ShiftbossDirectory.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb {
class MessageBus;
class TaggedMessage;
}

namespace quickstep {

class BlockLocator;
class CatalogDatabaseLite;
class CatalogRelation;
class QueryProcessor;
class StorageManager;

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
   * @param query_processor The QueryProcessor to save catalog upon the query
   *        completion.
   * @param storage_manager The StorageManager to use.
   * @param shiftboss_directory The ShiftbossDirectory to manage Shiftbosses.
   * @param bus The TMB.
   **/
  PolicyEnforcerDistributed(const tmb::client_id foreman_client_id,
                            CatalogDatabaseLite *catalog_database,
                            QueryProcessor *query_processor,
                            StorageManager *storage_manager,
                            ShiftbossDirectory *shiftboss_directory,
                            tmb::MessageBus *bus)
      : PolicyEnforcerBase(catalog_database),
        foreman_client_id_(foreman_client_id),
        query_processor_(query_processor),
        storage_manager_(storage_manager),
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
   * @param work_order_proto_messages The work order messages to be dispatched.
   **/
  void getWorkOrderProtoMessages(
      std::vector<std::unique_ptr<serialization::WorkOrderMessage>> *work_order_proto_messages);

  /**
   * @brief Process the initiate rebuild work order response message.
   *
   * @param tagged_message The message.
   **/
  void processInitiateRebuildResponseMessage(const tmb::TaggedMessage &tagged_message);

  /**
   * @brief Whether the query should be executed on one Shiftboss.
   *
   * @param query_id The query id.
   *
   * @return Whether the query should be executed on one Shiftboss.
   **/
  bool isSingleNodeQuery(const std::size_t query_id) const {
    const auto cit = admitted_queries_.find(query_id);
    DCHECK(cit != admitted_queries_.end());
    return cit->second->query_handle()->is_single_node_query();
  }

  /**
   * @brief Get or set the index of Shiftboss for an Aggregation related
   * WorkOrder. If it is the first Aggregation on <aggr_state_index>,
   * <shiftboss_index> will be set based on block locality if found,
   * otherwise <next_shiftboss_index_to_schedule>.
   * Otherwise, <shiftboss_index> will be set to the index of the Shiftboss that
   * has executed the first Aggregation.
   *
   * @param query_id The query id.
   * @param aggr_state_index The Hash Table for the Aggregation.
   * @param block_locator The BlockLocator to use.
   * @param block The block id to feed BlockLocator for the locality info.
   * @param next_shiftboss_index The index of Shiftboss to schedule a next WorkOrder.
   * @param shiftboss_index The index of Shiftboss to schedule the WorkOrder.
   **/
  void getShiftbossIndexForAggregation(
      const std::size_t query_id,
      const QueryContext::aggregation_state_id aggr_state_index,
      const BlockLocator &block_locator,
      const block_id block,
      const std::size_t next_shiftboss_index_to_schedule,
      std::size_t *shiftboss_index);

  /**
   * @brief Get or set the index of Shiftboss for a HashJoin related WorkOrder.
   * If it is the first BuildHash on <join_hash_table_index, part_id>,
   * <shiftboss_index> will be set to block locality if found,
   * otherwise <next_shiftboss_index_to_schedule>.
   * Otherwise, <shiftboss_index> will be set to the index of the Shiftboss that
   * has executed the first BuildHash.
   *
   * @param query_id The query id.
   * @param join_hash_table_index The Hash Table for the Join.
   * @param part_id The partition ID.
   * @param block_locator The BlockLocator to use.
   * @param block The block id to feed BlockLocator for the locality info.
   * @param next_shiftboss_index The index of Shiftboss to schedule a next WorkOrder.
   * @param shiftboss_index The index of Shiftboss to schedule the WorkOrder.
   **/
  void getShiftbossIndexForHashJoin(
      const std::size_t query_id,
      const QueryContext::join_hash_table_id join_hash_table_index,
      const partition_id part_id,
      const BlockLocator &block_locator,
      const block_id block,
      const std::size_t next_shiftboss_index_to_schedule,
      std::size_t *shiftboss_index);

 private:
  void decrementNumQueuedWorkOrders(const serialization::WorkOrderCompletionMessage &proto) override {
    shiftboss_directory_->decrementNumQueuedWorkOrders(proto.shiftboss_index());
  }

  void onQueryCompletion(QueryManagerBase *query_manager) override;

  void initiateQueryInShiftboss(QueryHandle *query_handle);

  void processAnalyzeQueryResult(const tmb::client_id cli_id,
                                 const CatalogRelation *query_result_relation,
                                 const QueryHandle::AnalyzeQueryInfo *analyze_query_info);

  const tmb::client_id foreman_client_id_;

  QueryProcessor *query_processor_;
  StorageManager *storage_manager_;
  ShiftbossDirectory *shiftboss_directory_;

  tmb::MessageBus *bus_;

  std::unordered_map<tmb::client_id, std::vector<relation_id>> completed_analyze_relations_;

  DISALLOW_COPY_AND_ASSIGN(PolicyEnforcerDistributed);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_POLICY_ENFORCER_DISTRIBUTED_HPP_
