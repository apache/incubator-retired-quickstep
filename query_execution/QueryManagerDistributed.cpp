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

#include "query_execution/QueryManagerDistributed.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryContext.pb.h"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "utility/DAG.hpp"
#include "utility/lip_filter/LIPFilter.pb.h"

#include "glog/logging.h"

#include "tmb/address.h"
#include "tmb/id_typedefs.h"
#include "tmb/tagged_message.h"

using std::free;
using std::malloc;
using std::move;
using std::size_t;
using std::unique_ptr;
using std::unordered_set;
using std::vector;

namespace quickstep {

QueryManagerDistributed::QueryManagerDistributed(QueryHandle *query_handle,
                                                 const tmb::client_id foreman_client_id,
                                                 const std::size_t num_shiftbosses,
                                                 tmb::Address &&shiftboss_addresses,  // NOLINT(whitespace/operators)
                                                 tmb::MessageBus *bus)
    : QueryManagerBase(query_handle),
      foreman_client_id_(foreman_client_id),
      num_shiftbosses_(num_shiftbosses),
      shiftboss_addresses_(move(shiftboss_addresses)),
      bus_(bus),
      normal_workorder_protos_container_(
          new WorkOrderProtosContainer(num_operators_in_dag_)) {
  // Collect all the workorders from all the non-blocking relational operators in the DAG.
  for (const dag_node_index index : non_dependent_operators_) {
    if (!fetchNormalWorkOrders(index)) {
      if (checkRebuildRequired(index)) {
        initiateRebuild(index);
      } else {
        markOperatorFinished(index);
      }
    }
  }

  const serialization::QueryContext &query_context_proto = query_handle->getQueryContextProto();
  for (int i = 0; i < query_context_proto.aggregation_states_size(); ++i) {
    shiftboss_indexes_for_aggrs_.push_back(
        vector<size_t>(query_context_proto.aggregation_states(i).num_partitions(), kInvalidShiftbossIndex));
  }

  for (int i = 0; i < query_context_proto.join_hash_tables_size(); ++i) {
    shiftboss_indexes_for_hash_joins_.push_back(
        vector<size_t>(query_context_proto.join_hash_tables(i).num_partitions(), kInvalidShiftbossIndex));
  }

  for (int i = 0; i < query_context_proto.num_partitions_for_nested_loops_joins_size(); ++i) {
    shiftboss_indexes_for_nested_loops_joins_.push_back(
        vector<size_t>(query_context_proto.num_partitions_for_nested_loops_joins(i), kInvalidShiftbossIndex));
  }

  computeLipFilterEquivalenceClasses(query_context_proto);

  const size_t num_partitions_for_lips = query_context_proto.num_partitions_for_lips();
  for (const LipFilterGroupIndex lip_filter_groups_index : lip_filter_groups_indexes_) {
    shiftboss_indexes_for_lip_filter_groups_.emplace(lip_filter_groups_index,
                                                     vector<size_t>(num_partitions_for_lips, kInvalidShiftbossIndex));
  }
}

void QueryManagerDistributed::computeLipFilterEquivalenceClasses(
    const serialization::QueryContext &query_context_proto) {
  static constexpr LipFilterGroupIndex kInvalidLipFilterGroupIndex = static_cast<LipFilterGroupIndex>(-1);
  lip_filter_groups_indexes_.resize(query_context_proto.lip_filters_size(), kInvalidLipFilterGroupIndex);

  std::unordered_map<LipFilterGroupIndex, std::unordered_set<QueryContext::lip_filter_id>> lip_filter_groups;
  for (int i = 0; i < query_context_proto.lip_filter_deployments_size(); ++i) {
    const serialization::LIPFilterDeployment &lip_filter_deployment = query_context_proto.lip_filter_deployments(i);

    unordered_set<QueryContext::lip_filter_id> lip_filter_ids;
    for (int j = 0; j < lip_filter_deployment.build_entries_size(); ++j) {
      lip_filter_ids.insert(lip_filter_deployment.build_entries(j).lip_filter_id());
    }

    for (int j = 0; j < lip_filter_deployment.probe_entries_size(); ++j) {
      lip_filter_ids.insert(lip_filter_deployment.probe_entries(j).lip_filter_id());
    }

    LipFilterGroupIndex min_lip_filter_groups_index = kInvalidLipFilterGroupIndex;
    unordered_set<LipFilterGroupIndex> lip_filter_groups_index_candidates;

    for (const QueryContext::lip_filter_id lip_filter_index : lip_filter_ids) {
      const LipFilterGroupIndex lip_filter_groups_index = lip_filter_groups_indexes_[lip_filter_index];
      if (lip_filter_groups_index != kInvalidLipFilterGroupIndex) {
        if (min_lip_filter_groups_index == kInvalidLipFilterGroupIndex) {
          min_lip_filter_groups_index = lip_filter_groups_index;
        } else if (min_lip_filter_groups_index != lip_filter_groups_index) {
          lip_filter_groups_index_candidates.insert(std::max(lip_filter_groups_index, min_lip_filter_groups_index));
          min_lip_filter_groups_index = std::min(lip_filter_groups_index, min_lip_filter_groups_index);
        }
      }
    }

    if (min_lip_filter_groups_index == kInvalidLipFilterGroupIndex) {
      const LipFilterGroupIndex lip_filter_groups_index = lip_filter_groups.size();
      for (const QueryContext::lip_filter_id lip_filter_index : lip_filter_ids) {
        lip_filter_groups_indexes_[lip_filter_index] = lip_filter_groups_index;
      }

      lip_filter_groups.emplace(lip_filter_groups_index, move(lip_filter_ids));
    } else {
      for (const QueryContext::lip_filter_id lip_filter_index : lip_filter_ids) {
        lip_filter_groups_indexes_[lip_filter_index] = min_lip_filter_groups_index;
      }

      lip_filter_groups[min_lip_filter_groups_index].insert(lip_filter_ids.begin(), lip_filter_ids.end());

      for (const LipFilterGroupIndex lip_filter_groups_index : lip_filter_groups_index_candidates) {
        lip_filter_groups.erase(lip_filter_groups_index);
      }
    }
  }
}

serialization::WorkOrderMessage* QueryManagerDistributed::getNextWorkOrderMessage() {
  // Default policy: Operator with lowest index first.
  for (dag_node_index index = 0u; index < num_operators_in_dag_; ++index) {
    if (query_exec_state_->hasExecutionFinished(index)) {
      continue;
    }
    unique_ptr<serialization::WorkOrder> work_order_proto(
        normal_workorder_protos_container_->getWorkOrderProto(index));
    if (work_order_proto) {
      query_exec_state_->incrementNumQueuedWorkOrders(index);

      unique_ptr<serialization::WorkOrderMessage> message_proto(new serialization::WorkOrderMessage);
      message_proto->set_query_id(query_id_);
      message_proto->set_operator_index(index);
      message_proto->mutable_work_order()->MergeFrom(*work_order_proto);

      return message_proto.release();
    }
  }
  // No normal WorkOrder protos available right now.
  return nullptr;
}

bool QueryManagerDistributed::fetchNormalWorkOrders(const dag_node_index index) {
  // Do not fetch any work units until all blocking dependencies are met.
  // The releational operator is not aware of blocking dependencies for
  // uncorrelated scalar queries.
  DCHECK(checkAllBlockingDependenciesMet(index));
  DCHECK(!query_exec_state_->hasDoneGenerationWorkOrders(index));

  const size_t num_pending_workorder_protos_before =
      normal_workorder_protos_container_->getNumWorkOrderProtos(index);
  const bool done_generation =
      query_dag_->getNodePayloadMutable(index)
          ->getAllWorkOrderProtos(normal_workorder_protos_container_.get());
  if (done_generation) {
    query_exec_state_->setDoneGenerationWorkOrders(index);
  }

  return (num_pending_workorder_protos_before < normal_workorder_protos_container_->getNumWorkOrderProtos(index));
}

void QueryManagerDistributed::processInitiateRebuildResponseMessage(const dag_node_index op_index,
                                                                    const std::size_t num_rebuild_work_orders,
                                                                    const std::size_t shiftboss_index) {
  query_exec_state_->updateRebuildStatus(op_index, num_rebuild_work_orders, shiftboss_index);

  if (query_exec_state_->hasRebuildFinished(op_index, num_shiftbosses_)) {
    // No needs for rebuilds, or the rebuild has finished.
    markOperatorFinished(op_index);
  }

  // Wait for the rebuild work orders to finish.
}

bool QueryManagerDistributed::initiateRebuild(const dag_node_index index) {
  DCHECK(checkRebuildRequired(index));
  DCHECK(!checkRebuildInitiated(index));

  const RelationalOperator &op = query_dag_->getNodePayload(index);
  DCHECK_NE(op.getInsertDestinationID(), QueryContext::kInvalidInsertDestinationId);

  serialization::InitiateRebuildMessage proto;
  proto.set_query_id(query_id_);
  proto.set_operator_index(index);
  proto.set_insert_destination_index(op.getInsertDestinationID());
  proto.set_relation_id(op.getOutputRelationID());

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage tagged_msg(static_cast<const void *>(proto_bytes),
                           proto_length,
                           kInitiateRebuildMessage);
  free(proto_bytes);

  DLOG(INFO) << "ForemanDistributed sent InitiateRebuildMessage to all Shiftbosses";
  QueryExecutionUtil::BroadcastMessage(foreman_client_id_,
                                       shiftboss_addresses_,
                                       move(tagged_msg),
                                       bus_);

  query_exec_state_->setRebuildStatus(index, 0, true);

  // Wait for Shiftbosses to report the number of rebuild work orders.
  return false;
}

}  // namespace quickstep
