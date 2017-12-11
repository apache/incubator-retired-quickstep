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

#include "relational_operators/BuildTransitiveClosureOperator.hpp"

#include <memory>
#include <cstddef>

#include "cli/Flags.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TransitiveClosureState.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "utility/Range.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

bool BuildTransitiveClosureOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  DCHECK(query_context != nullptr);

  if (doneWorkOrderGeneration()) {
    return true;
  }

  TransitiveClosureState *state =
      query_context->getTransitiveClosureState(transitive_closure_context_index_);

  while (start_relation_block_stream_.hasNext()) {
    container->addNormalWorkOrder(
        new BuildTransitiveClosureWorkOrder(query_id_,
                                            start_relation_,
                                            true /* is_start_relation */,
                                            start_relation_block_stream_.getNext(),
                                            start_attr_id_,
                                            source_attr_id_,
                                            destination_attr_id_,
                                            state,
                                            storage_manager),
        op_index_);
  }

  while (edge_relation_block_stream_.hasNext()) {
    container->addNormalWorkOrder(
        new BuildTransitiveClosureWorkOrder(query_id_,
                                            edge_relation_,
                                            false /* is_start_relation */,
                                            edge_relation_block_stream_.getNext(),
                                            start_attr_id_,
                                            source_attr_id_,
                                            destination_attr_id_,
                                            state,
                                            storage_manager),
        op_index_);
  }

  return doneWorkOrderGeneration();
}

bool BuildTransitiveClosureOperator::getAllWorkOrderProtos(
    WorkOrderProtosContainer *container)  {
  LOG(FATAL) << "Not supported";
}

void BuildTransitiveClosureWorkOrder::execute() {
  BlockReference block(storage_manager_->getBlock(block_, input_relation_));

  std::unique_ptr<ValueAccessor> accessor(
      block->getTupleStorageSubBlock().createValueAccessor());

  if (is_start_relation_) {
    buildStartRelation(accessor.get());
  } else {
    buildEdgeRelation(accessor.get());
  }
}

void BuildTransitiveClosureWorkOrder::buildStartRelation(ValueAccessor *accessor) {
  InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> void {
    while (accessor->next()) {
      const void *value = accessor->getUntypedValue(this->start_attr_id_);
      this->state_->addStart(*static_cast<const int*>(value));
    }
  });
}

void BuildTransitiveClosureWorkOrder::buildEdgeRelation(ValueAccessor *accessor) {
  InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> void {
    while (accessor->next()) {
      const void *source = accessor->getUntypedValue(this->source_attr_id_);
      const void *destination = accessor->getUntypedValue(this->destination_attr_id_);
      this->state_->addEdge(*static_cast<const int*>(source),
                            *static_cast<const int*>(destination));
    }
  });
}

}  // namespace quickstep

