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

#include "query_execution/PolicyEnforcerDistributed.hpp"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogRelation.hpp"
#include "query_execution/QueryContext.pb.h"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionState.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/QueryManagerBase.hpp"
#include "query_execution/QueryManagerDistributed.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "query_optimizer/QueryProcessor.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/ExecutionDAGVisualizer.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

#include "tmb/address.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

using std::free;
using std::malloc;
using std::move;
using std::size_t;
using std::unique_ptr;
using std::vector;

using tmb::TaggedMessage;

namespace quickstep {

DECLARE_bool(visualize_execution_dag);

namespace S = serialization;

DEFINE_uint64(max_msgs_per_dispatch_round, 20, "Maximum number of messages that"
              " can be allocated in a single round of dispatch of messages to"
              " the workers.");

void PolicyEnforcerDistributed::getWorkOrderProtoMessages(
    vector<unique_ptr<S::WorkOrderMessage>> *work_order_proto_messages) {
  // Iterate over admitted queries until either there are no more
  // messages available, or the maximum number of messages have
  // been collected.
  DCHECK(work_order_proto_messages->empty());
  // TODO(harshad) - Make this function generic enough so that it
  // works well when multiple queries are getting executed.
  if (admitted_queries_.empty()) {
    if (waiting_queries_.empty()) {
      LOG(WARNING) << "Requesting WorkOrderProtoMessages when no query is running";
      return;
    } else {
      // Admit the earliest waiting query.
      QueryHandle *new_query = waiting_queries_.front();
      waiting_queries_.pop();
      admitQuery(new_query);
    }
  }

  const std::size_t per_query_share =
      FLAGS_max_msgs_per_dispatch_round / admitted_queries_.size();
  DCHECK_GT(per_query_share, 0u);

  vector<std::size_t> finished_queries_ids;

  for (const auto &admitted_query_info : admitted_queries_) {
    QueryManagerBase *curr_query_manager = admitted_query_info.second.get();
    DCHECK(curr_query_manager != nullptr);
    std::size_t messages_collected_curr_query = 0;
    while (messages_collected_curr_query < per_query_share) {
      S::WorkOrderMessage *next_work_order_message =
          static_cast<QueryManagerDistributed*>(curr_query_manager)->getNextWorkOrderMessage(0);
      if (next_work_order_message != nullptr) {
        ++messages_collected_curr_query;
        work_order_proto_messages->push_back(unique_ptr<S::WorkOrderMessage>(next_work_order_message));
      } else {
        // No more work ordes from the current query at this time.
        // Check if the query's execution is over.
        if (curr_query_manager->getQueryExecutionState().hasQueryExecutionFinished()) {
          // If the query has been executed, remove it.
          finished_queries_ids.push_back(admitted_query_info.first);
        }
        break;
      }
    }
  }
  for (const std::size_t finished_qid : finished_queries_ids) {
    onQueryCompletion(admitted_queries_[finished_qid].get());
    removeQuery(finished_qid);
  }
}

bool PolicyEnforcerDistributed::admitQuery(QueryHandle *query_handle) {
  if (admitted_queries_.size() >= PolicyEnforcerBase::kMaxConcurrentQueries) {
    // This query will have to wait.
    waiting_queries_.push(query_handle);
    return false;
  }

  const std::size_t query_id = query_handle->query_id();
  if (admitted_queries_.find(query_id) != admitted_queries_.end()) {
    LOG(ERROR) << "Query with the same ID " << query_id << " exists";
    return false;
  }

  // Ok to admit the query.
  // NOTE(zuyu): Should call before constructing a 'QueryManager'.
  // Otherwise, an InitiateRebuildMessage may be sent before 'QueryContext'
  // initializes.
  initiateQueryInShiftboss(query_handle);

  const std::size_t num_shiftbosses = shiftboss_directory_->size();

  tmb::Address shiftboss_addresses;
  for (std::size_t i = 0; i < num_shiftbosses; ++i) {
    shiftboss_addresses.AddRecipient(shiftboss_directory_->getClientId(i));
  }

  // Query with the same ID not present, ok to admit.
  admitted_queries_.emplace(query_id,
                            std::make_unique<QueryManagerDistributed>(
                                query_handle, foreman_client_id_, num_shiftbosses, move(shiftboss_addresses), bus_));
  return true;
}

void PolicyEnforcerDistributed::processInitiateRebuildResponseMessage(const tmb::TaggedMessage &tagged_message) {
  S::InitiateRebuildResponseMessage proto;
  CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

  const std::size_t query_id = proto.query_id();
  DCHECK(admitted_queries_.find(query_id) != admitted_queries_.end());

  QueryManagerDistributed *query_manager = static_cast<QueryManagerDistributed*>(admitted_queries_[query_id].get());

  const std::size_t num_rebuild_work_orders = proto.num_rebuild_work_orders();
  const size_t shiftboss_index = proto.shiftboss_index();
  query_manager->processInitiateRebuildResponseMessage(
      proto.operator_index(), num_rebuild_work_orders, shiftboss_index);
  shiftboss_directory_->addNumQueuedWorkOrders(shiftboss_index, num_rebuild_work_orders);

  if (query_manager->getQueryExecutionState().hasQueryExecutionFinished()) {
    onQueryCompletion(query_manager);
    removeQuery(query_id);
  }
}

void PolicyEnforcerDistributed::getShiftbossIndexForAggregation(
    const std::size_t query_id,
    const QueryContext::aggregation_state_id aggr_state_index,
    const std::size_t next_shiftboss_index_to_schedule,
    std::size_t *shiftboss_index) {
  DCHECK(admitted_queries_.find(query_id) != admitted_queries_.end());
  QueryManagerDistributed *query_manager = static_cast<QueryManagerDistributed*>(admitted_queries_[query_id].get());
  query_manager->getShiftbossIndexForAggregation(aggr_state_index,
                                                 next_shiftboss_index_to_schedule,
                                                 shiftboss_index);
}

void PolicyEnforcerDistributed::getShiftbossIndexForHashJoin(
    const std::size_t query_id,
    const QueryContext::join_hash_table_id join_hash_table_index,
    const partition_id part_id,
    const std::size_t next_shiftboss_index_to_schedule,
    std::size_t *shiftboss_index) {
  DCHECK(admitted_queries_.find(query_id) != admitted_queries_.end());
  QueryManagerDistributed *query_manager = static_cast<QueryManagerDistributed*>(admitted_queries_[query_id].get());
  query_manager->getShiftbossIndexForHashJoin(join_hash_table_index,
                                              part_id,
                                              next_shiftboss_index_to_schedule,
                                              shiftboss_index);
}

void PolicyEnforcerDistributed::initiateQueryInShiftboss(QueryHandle *query_handle) {
  S::QueryInitiateMessage proto;
  proto.set_query_id(query_handle->query_id());
  proto.mutable_catalog_database_cache()->MergeFrom(query_handle->getCatalogDatabaseCacheProto());
  proto.mutable_query_context()->MergeFrom(query_handle->getQueryContextProto());

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage message(static_cast<const void*>(proto_bytes),
                        proto_length,
                        kQueryInitiateMessage);
  free(proto_bytes);

  // TODO(quickstep-team): Dynamically scale-up/down Shiftbosses.
  tmb::Address shiftboss_addresses;
  for (std::size_t i = 0; i < shiftboss_directory_->size(); ++i) {
    shiftboss_addresses.AddRecipient(shiftboss_directory_->getClientId(i));
  }

  DLOG(INFO) << "PolicyEnforcerDistributed sent QueryInitiateMessage (typed '" << kQueryInitiateMessage
             << "') to all Shiftbosses";
  QueryExecutionUtil::BroadcastMessage(foreman_client_id_,
                                       shiftboss_addresses,
                                       move(message),
                                       bus_);
}

void PolicyEnforcerDistributed::onQueryCompletion(QueryManagerBase *query_manager) {
  const QueryHandle *query_handle = query_manager->query_handle();

  const CatalogRelation *query_result = query_handle->getQueryResultRelation();
  const tmb::client_id cli_id = query_handle->getClientId();
  const std::size_t query_id = query_handle->query_id();

  if (FLAGS_visualize_execution_dag && hasProfilingResults(query_id)) {
    ExecutionDAGVisualizer* dag_visualizer = query_manager->dag_visualizer();
    dag_visualizer->bindProfilingStats(getProfilingResults(query_id));

    std::ostringstream dot_filename;
    dot_filename << query_id << ".dot";

    FILE *fp = std::fopen(dot_filename.str().c_str(), "w");
    CHECK_NOTNULL(fp);
    const std::string dot_file_content(dag_visualizer->toDOT());
    const std::size_t dot_file_length = dot_file_content.length();

    CHECK_EQ(dot_file_length,
             std::fwrite(dot_file_content.c_str(), sizeof(char), dot_file_length, fp));

    std::fclose(fp);
  }

  // TODO(quickstep-team): Dynamically scale-up/down Shiftbosses.
  tmb::Address shiftboss_addresses;
  for (std::size_t i = 0; i < shiftboss_directory_->size(); ++i) {
    shiftboss_addresses.AddRecipient(shiftboss_directory_->getClientId(i));
  }

  if (query_result == nullptr) {
    if (query_processor_) {
      query_processor_->saveCatalog();
    }

    // Clean up query execution states, i.e., QueryContext, in Shiftbosses.
    serialization::QueryTeardownMessage proto;
    proto.set_query_id(query_id);

    const size_t proto_length = proto.ByteSize();
    char *proto_bytes = static_cast<char*>(malloc(proto_length));
    CHECK(proto.SerializeToArray(proto_bytes, proto_length));

    TaggedMessage message(static_cast<const void*>(proto_bytes),
                          proto_length,
                          kQueryTeardownMessage);
    free(proto_bytes);

    DLOG(INFO) << "PolicyEnforcerDistributed sent QueryTeardownMessage (typed '" << kQueryTeardownMessage
               << "') to all Shiftbosses";
    QueryExecutionUtil::BroadcastMessage(foreman_client_id_,
                                         shiftboss_addresses,
                                         move(message),
                                         bus_);

    TaggedMessage cli_message(kQueryExecutionSuccessMessage);

    // Notify the CLI query execution successfully.
    DLOG(INFO) << "PolicyEnforcerDistributed sent QueryExecutionSuccessMessage (typed '"
               << kQueryExecutionSuccessMessage
               << "') to CLI with TMB client id " << cli_id;
    const tmb::MessageBus::SendStatus send_status =
        QueryExecutionUtil::SendTMBMessage(bus_,
                                           foreman_client_id_,
                                           cli_id,
                                           move(cli_message));
    CHECK(send_status == tmb::MessageBus::SendStatus::kOK);
    return;
  }

  // NOTE(zuyu): SaveQueryResultMessage implicitly triggers QueryTeardown in Shiftboss.
  S::SaveQueryResultMessage proto;
  proto.set_query_id(query_id);
  proto.set_relation_id(query_result->getID());

  const vector<block_id> blocks(query_result->getBlocksSnapshot());
  for (const block_id block : blocks) {
    proto.add_blocks(block);
  }

  proto.set_cli_id(cli_id);

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage message(static_cast<const void*>(proto_bytes),
                        proto_length,
                        kSaveQueryResultMessage);
  free(proto_bytes);

  // TODO(quickstep-team): Dynamically scale-up/down Shiftbosses.
  DLOG(INFO) << "PolicyEnforcerDistributed sent SaveQueryResultMessage (typed '" << kSaveQueryResultMessage
             << "') to all Shiftbosses";
  QueryExecutionUtil::BroadcastMessage(foreman_client_id_,
                                       shiftboss_addresses,
                                       move(message),
                                       bus_);
}

}  // namespace quickstep
