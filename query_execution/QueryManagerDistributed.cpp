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

#include "query_execution/QueryManagerDistributed.hpp"

#include <cstddef>
#include <cstdlib>
#include <memory>
#include <utility>

#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/ShiftbossDirectory.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "utility/DAG.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

using std::free;
using std::malloc;
using std::move;
using std::size_t;
using std::unique_ptr;

namespace quickstep {

QueryManagerDistributed::QueryManagerDistributed(QueryHandle *query_handle,
                                                 ShiftbossDirectory *shiftbosses,
                                                 const tmb::client_id foreman_client_id,
                                                 tmb::MessageBus *bus)
    : QueryManagerBase(query_handle),
      shiftbosses_(shiftbosses),
      foreman_client_id_(foreman_client_id),
      bus_(bus),
      normal_workorder_protos_container_(
          new WorkOrderProtosContainer(num_operators_in_dag_)) {
  // Collect all the workorders from all the relational operators in the DAG.
  for (dag_node_index index = 0; index < num_operators_in_dag_; ++index) {
    if (checkAllBlockingDependenciesMet(index)) {
      query_dag_->getNodePayloadMutable(index)->informAllBlockingDependenciesMet();
      processOperator(index, false);
    }
  }
}

serialization::WorkOrderMessage* QueryManagerDistributed::getNextWorkOrderMessage(
    const dag_node_index start_operator_index) {
  // Default policy: Operator with lowest index first.
  size_t num_operators_checked = 0;
  for (dag_node_index index = start_operator_index;
       num_operators_checked < num_operators_in_dag_;
       index = (index + 1) % num_operators_in_dag_, ++num_operators_checked) {
    if (query_exec_state_->hasExecutionFinished(index)) {
      continue;
    }
    unique_ptr<serialization::WorkOrder> work_order_proto(
        normal_workorder_protos_container_->getWorkOrderProto(index));
    if (work_order_proto != nullptr) {
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
  bool generated_new_workorder_protos = false;
  if (!query_exec_state_->hasDoneGenerationWorkOrders(index)) {
    // Do not fetch any work units until all blocking dependencies are met.
    // The releational operator is not aware of blocking dependencies for
    // uncorrelated scalar queries.
    if (!checkAllBlockingDependenciesMet(index)) {
      return false;
    }
    const size_t num_pending_workorder_protos_before =
        normal_workorder_protos_container_->getNumWorkOrderProtos(index);
    const bool done_generation =
        query_dag_->getNodePayloadMutable(index)
            ->getAllWorkOrderProtos(normal_workorder_protos_container_.get());
    if (done_generation) {
      query_exec_state_->setDoneGenerationWorkOrders(index);
    }

    // TODO(shoban): It would be a good check to see if operator is making
    // useful progress, i.e., the operator either generates work orders to
    // execute or still has pending work orders executing. However, this will not
    // work if Foreman polls operators without feeding data. This check can be
    // enabled, if Foreman is refactored to call getAllWorkOrders() only when
    // pending work orders are completed or new input blocks feed.

    generated_new_workorder_protos =
        (num_pending_workorder_protos_before <
         normal_workorder_protos_container_->getNumWorkOrderProtos(index));
  }
  return generated_new_workorder_protos;
}

bool QueryManagerDistributed::initiateRebuild(const dag_node_index index) {
  DCHECK(checkRebuildRequired(index));
  DCHECK(!checkRebuildInitiated(index));

  const RelationalOperator &op = query_dag_->getNodePayload(index);
  DCHECK_NE(op.getInsertDestinationID(), QueryContext::kInvalidInsertDestinationId);

  serialization::InitiateRebuildMessage proto;
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

  LOG(INFO) << "ForemanDistributed sent InitiateRebuildMessage (typed '" << kInitiateRebuildMessage
            << "') to Shiftboss";
  // TODO(zuyu): Multiple workers support.
  QueryExecutionUtil::SendTMBMessage(bus_,
                                     foreman_client_id_,
                                     shiftbosses_->getClientId(0),
                                     move(tagged_msg));

  // The negative value indicates that the number of rebuild work orders is to be
  // determined.
  query_exec_state_->setRebuildStatus(index, -1, true);

  // Wait for Shiftbosses to report the number of rebuild work orders.
  return false;
}

}  // namespace quickstep
