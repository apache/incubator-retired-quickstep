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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_REBUILD_WORKORDER_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_REBUILD_WORKORDER_HPP_

#include <cstddef>
#include <cstdlib>
#include <ostream>
#include <utility>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlock.hpp"
#include "threading/ThreadIDBasedMap.hpp"
#include "utility/Macros.hpp"

#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup RelationalOperators
 *  @{
 */

class RebuildWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param query_id The ID of the query to which this RebuildWorkOrder belongs.
   * @param block_ref A MutableBlockReference to the storage block which needs
   *        to be rebuilt.
   * @param input_operator_index The index of the relational operator in the
   *        query plan DAG that produced the output block.
   * @param input_relation_id The ID of the CatalogRelation to which the given
   *        storage block belongs to.
   * @param scheduler_client_id The TMB client ID of the scheduler thread.
   * @param bus A pointer to the TMB.
   **/
  RebuildWorkOrder(
      const std::size_t query_id,
      MutableBlockReference &&block_ref,  // NOLINT(whitespace/operators)
      const std::size_t input_operator_index,
      const relation_id input_relation_id,
      const client_id scheduler_client_id,
      MessageBus *bus)
      : WorkOrder(query_id),
        block_ref_(std::move(block_ref)),
        input_operator_index_(input_operator_index),
        input_relation_id_(input_relation_id),
        scheduler_client_id_(scheduler_client_id),
        bus_(bus) {}

  ~RebuildWorkOrder() {}

  void execute() {
    if (!block_ref_->rebuild()) {
      LOG_WARNING("Rebuilding of StorageBlock with ID: "
                  << block_ref_->getID() << " invalidated one or more "
                  "IndexSubBlocks.");
    }

    serialization::DataPipelineMessage proto;
    proto.set_operator_index(input_operator_index_);
    proto.set_block_id(block_ref_->getID());
    proto.set_relation_id(input_relation_id_);
    proto.set_query_id(query_id_);

    // NOTE(zuyu): Using the heap memory to serialize proto as a c-like string.
    const std::size_t proto_length = proto.ByteSize();
    char *proto_bytes = static_cast<char*>(std::malloc(proto_length));
    CHECK(proto.SerializeToArray(proto_bytes, proto_length));

    tmb::TaggedMessage tagged_message(static_cast<const void *>(proto_bytes),
                                      proto_length,
                                      kDataPipelineMessage);
    std::free(proto_bytes);

    // Refer to InsertDestination::sendBlockFilledMessage for the rationale
    // behind using the ClientIDMap map.
    const tmb::MessageBus::SendStatus send_status =
        QueryExecutionUtil::SendTMBMessage(bus_,
                                           ClientIDMap::Instance()->getValue(),
                                           scheduler_client_id_,
                                           std::move(tagged_message));
    CHECK(send_status == tmb::MessageBus::SendStatus::kOK) << "Message could "
        " not be sent from thread with TMB client ID " <<
        ClientIDMap::Instance()->getValue() << " to Foreman with TMB client ID "
        << scheduler_client_id_;
  }

 private:
  MutableBlockReference block_ref_;
  const std::size_t input_operator_index_;
  const relation_id input_relation_id_;
  const client_id scheduler_client_id_;

  MessageBus *bus_;

  DISALLOW_COPY_AND_ASSIGN(RebuildWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_REBUILD_WORKORDER_HPP_
