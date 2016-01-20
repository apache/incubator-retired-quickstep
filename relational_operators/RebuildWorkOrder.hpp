/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_REBUILD_WORKORDER_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_REBUILD_WORKORDER_HPP_

#include <cstddef>
#include <ostream>
#include <utility>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/ForemanMessage.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlock.hpp"
#include "threading/ThreadIDBasedMap.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogDatabase;
class QueryContext;
class StorageManager;

/** \addtogroup RelationalOperators
 *  @{
 */

class RebuildWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param block_ref A MutableBlockReference to the storage block which needs
   *        to be rebuilt.
   * @param input_operator_index The index of the relational operator in the
   *        query plan DAG that produced the output block.
   * @param input_relation_id The ID of the CatalogRelation to which the given
   *        storage block belongs to.
   * @param foreman_input_queue A pointer to the Foreman's input queue.
   **/
  RebuildWorkOrder(MutableBlockReference &&block_ref,
                   const std::size_t input_operator_index,
                   const relation_id input_relation_id,
                   const client_id foreman_client_id,
                   MessageBus *bus)
      : block_ref_(std::move(block_ref)),
        input_operator_index_(input_operator_index),
        input_relation_id_(input_relation_id),
        foreman_client_id_(foreman_client_id),
        bus_(bus) {}

  ~RebuildWorkOrder() {}

  void execute(QueryContext *query_context,
               CatalogDatabase *catalog_database,
               StorageManager *storage_manager) {
    if (!block_ref_->rebuild()) {
      LOG_WARNING("Rebuilding of StorageBlock with ID: "
                  << block_ref_->getID() << " invalidated one or more "
                  "IndexSubBlocks.");
    }
    ForemanMessage message(ForemanMessage::DataPipelineMessage(
        input_operator_index_,
        block_ref_->getID(),
        input_relation_id_));

    TaggedMessage foreman_tagged_msg;

    foreman_tagged_msg.set_message(
        &message,
        sizeof(message),
        kDataPipelineMessage);

    // Refer to InsertDestination::sendBlockFilledMessage for the rationale
    // behind using the ClientIDMap map.
    QueryExecutionUtil::SendTMBMessage(bus_,
                                       ClientIDMap::Instance()->getValue(),
                                       foreman_client_id_,
                                       std::move(foreman_tagged_msg));
  }

 private:
  MutableBlockReference block_ref_;
  const std::size_t input_operator_index_;
  const relation_id input_relation_id_;
  const client_id foreman_client_id_;

  MessageBus *bus_;

  DISALLOW_COPY_AND_ASSIGN(RebuildWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_REBUILD_WORKORDER_HPP_
