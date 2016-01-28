/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

#include "relational_operators/UpdateOperator.hpp"

#include <cstddef>
#include <cstdlib>
#include <ostream>
#include <utility>

#include "catalog/CatalogDatabase.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "threading/ThreadIDBasedMap.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/tagged_message.h"

namespace quickstep {

bool UpdateOperator::getAllWorkOrders(WorkOrdersContainer *container) {
  if (blocking_dependencies_met_ && !started_) {
    for (const block_id input_block_id : input_blocks_) {
      container->addNormalWorkOrder(
          new UpdateWorkOrder(relation_.getID(),
                              relocation_destination_index_,
                              predicate_index_,
                              update_group_index_,
                              input_block_id,
                              op_index_,
                              foreman_client_id_,
                              bus_),
          op_index_);
    }
    started_ = true;
  }
  return started_;
}

void UpdateWorkOrder::execute(QueryContext *query_context,
                              CatalogDatabase *database,
                              StorageManager *storage_manager) {
  DCHECK(query_context != nullptr);
  DCHECK(database != nullptr);
  DCHECK(storage_manager != nullptr);

  MutableBlockReference block(
      storage_manager->getBlockMutable(input_block_id_,
                                       *database->getRelationById(rel_id_)));

  InsertDestination *relocation_destination =
      query_context->getInsertDestination(relocation_destination_index_);
  DCHECK(relocation_destination != nullptr);

  StorageBlock::UpdateResult result =
      block->update(query_context->getUpdateGroup(update_group_index_),
                    query_context->getPredicate(predicate_index_),
                    relocation_destination);

  if (!result.indices_consistent) {
    LOG_WARNING("An UPDATE caused one or more IndexSubBlocks in StorageBlock "
                << BlockIdUtil::ToString(input_block_id_) << " to become inconsistent "
                << "(not enough space to index all tuples in block).");
  }

  // TODO(harshad): Stream the block ID only if the predicate returned at least
  // one match in the StorageBlock.
  serialization::DataPipelineMessage proto;
  proto.set_operator_index(update_operator_index_);
  proto.set_block_id(input_block_id_);
  proto.set_relation_id(rel_id_);

  const std::size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(std::malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  tmb::TaggedMessage tagged_message(static_cast<const void *>(proto_bytes),
                                    proto_length,
                                    kDataPipelineMessage);
  std::free(proto_bytes);

  QueryExecutionUtil::SendTMBMessage(
      bus_,
      ClientIDMap::Instance()->getValue(),
      foreman_client_id_,
      std::move(tagged_message));
}

}  // namespace quickstep
