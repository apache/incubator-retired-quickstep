/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include "query_execution/PolicyEnforcerBase.hpp"

#include <cstddef>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/PartitionScheme.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionState.hpp"
#include "query_execution/QueryManagerBase.hpp"
#include "query_optimizer/QueryOptimizerConfig.h"  // For QUICKSTEP_DISTRIBUTED.
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"

#include "glog/logging.h"

namespace quickstep {

void PolicyEnforcerBase::processMessage(const TaggedMessage &tagged_message) {
  std::size_t query_id;
  QueryManagerBase::dag_node_index op_index;

  switch (tagged_message.message_type()) {
    case kWorkOrderCompleteMessage: {
      serialization::NormalWorkOrderCompletionMessage proto;
      // Note: This proto message contains the time it took to execute the
      // WorkOrder. It can be accessed in this scope.
      CHECK(proto.ParseFromArray(tagged_message.message(),
                                 tagged_message.message_bytes()));
      decrementNumQueuedWorkOrders(proto.worker_thread_index());

      if (profile_individual_workorders_) {
        recordTimeForWorkOrder(proto);
      }

      query_id = proto.query_id();
      DCHECK(admitted_queries_.find(query_id) != admitted_queries_.end());

      op_index = proto.operator_index();
      admitted_queries_[query_id]->processWorkOrderCompleteMessage(op_index);
      break;
    }
#ifdef QUICKSTEP_DISTRIBUTED
    case kInitiateRebuildResponseMessage: {
      serialization::InitiateRebuildResponseMessage proto;
      CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

      query_id = proto.query_id();
      DCHECK(admitted_queries_.find(query_id) != admitted_queries_.end());

      op_index = proto.operator_index();
      const std::size_t num_rebuild_work_orders = proto.num_rebuild_work_orders();

      // Check if new work orders are available.
      admitted_queries_[query_id]->processInitiateRebuildResponseMessage(op_index, num_rebuild_work_orders);
      incrementNumQueuedWorkOrders(proto.shiftboss_index(), num_rebuild_work_orders);
      break;
    }
#endif  // QUICKSTEP_DISTRIBUTED
    case kRebuildWorkOrderCompleteMessage: {
      serialization::RebuildWorkOrderCompletionMessage proto;
      // Note: This proto message contains the time it took to execute the
      // rebuild WorkOrder. It can be accessed in this scope.
      CHECK(proto.ParseFromArray(tagged_message.message(),
                                 tagged_message.message_bytes()));
      decrementNumQueuedWorkOrders(proto.worker_thread_index());

      query_id = proto.query_id();
      DCHECK(admitted_queries_.find(query_id) != admitted_queries_.end());

      op_index = proto.operator_index();
      admitted_queries_[query_id]->processRebuildWorkOrderCompleteMessage(op_index);
      break;
    }
    case kCatalogRelationNewBlockMessage: {
      serialization::CatalogRelationNewBlockMessage proto;
      CHECK(proto.ParseFromArray(tagged_message.message(),
                                 tagged_message.message_bytes()));

      const block_id block = proto.block_id();

      CatalogRelation *relation =
          static_cast<CatalogDatabase*>(catalog_database_)->getRelationByIdMutable(proto.relation_id());
      relation->addBlock(block);

      if (proto.has_partition_id()) {
        relation->getPartitionSchemeMutable()->addBlockToPartition(
            proto.partition_id(), block);
      }
      return;
    }
    case kDataPipelineMessage: {
      serialization::DataPipelineMessage proto;
      CHECK(proto.ParseFromArray(tagged_message.message(),
                                 tagged_message.message_bytes()));
      query_id = proto.query_id();
      DCHECK(admitted_queries_.find(query_id) != admitted_queries_.end());

      op_index = proto.operator_index();
      admitted_queries_[query_id]->processDataPipelineMessage(
          op_index, proto.block_id(), proto.relation_id());
      break;
    }
    case kWorkOrdersAvailableMessage: {
      serialization::WorkOrdersAvailableMessage proto;
      CHECK(proto.ParseFromArray(tagged_message.message(),
                                 tagged_message.message_bytes()));
      query_id = proto.query_id();
      DCHECK(admitted_queries_.find(query_id) != admitted_queries_.end());

      op_index = proto.operator_index();

      // Check if new work orders are available.
      admitted_queries_[query_id]->fetchNormalWorkOrders(op_index);
      break;
    }
    case kWorkOrderFeedbackMessage: {
      WorkOrder::FeedbackMessage msg(
          const_cast<void *>(tagged_message.message()),
          tagged_message.message_bytes());
      query_id = msg.header().query_id;
      DCHECK(admitted_queries_.find(query_id) != admitted_queries_.end());

      op_index = msg.header().rel_op_index;
      admitted_queries_[query_id]->processFeedbackMessage(op_index, msg);
      break;
    }
    default:
      LOG(FATAL) << "Unknown message type found in PolicyEnforcer";
  }
  if (admitted_queries_[query_id]->queryStatus(op_index) ==
          QueryManagerBase::QueryStatusCode::kQueryExecuted) {
    removeQuery(query_id);
    if (!waiting_queries_.empty()) {
      // Admit the earliest waiting query.
      QueryHandle *new_query = waiting_queries_.front();
      waiting_queries_.pop();
      admitQuery(new_query);
    }
  }
}

void PolicyEnforcerBase::removeQuery(const std::size_t query_id) {
  DCHECK(admitted_queries_.find(query_id) != admitted_queries_.end());
  if (!admitted_queries_[query_id]->getQueryExecutionState().hasQueryExecutionFinished()) {
    LOG(WARNING) << "Removing query with ID " << query_id
                 << " that hasn't finished its execution";
  }
  admitted_queries_.erase(query_id);
}

bool PolicyEnforcerBase::admitQueries(
    const std::vector<QueryHandle*> &query_handles) {
  for (QueryHandle *curr_query : query_handles) {
    if (!admitQuery(curr_query)) {
      return false;
    }
  }
  return true;
}

void PolicyEnforcerBase::recordTimeForWorkOrder(
    const serialization::NormalWorkOrderCompletionMessage &proto) {
  const std::size_t query_id = proto.query_id();
  if (workorder_time_recorder_.find(query_id) == workorder_time_recorder_.end()) {
    workorder_time_recorder_[query_id];
  }
  workorder_time_recorder_[query_id].emplace_back(
      proto.worker_thread_index(),
      proto.operator_index(),
      proto.execution_time_in_microseconds());
}

}  // namespace quickstep
