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

#include "relational_operators/InitializeTransitiveClosureOperator.hpp"

#include <algorithm>
#include <cstddef>

#include "cli/Flags.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/TransitiveClosureState.hpp"
#include "utility/Range.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

bool InitializeTransitiveClosureOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  if (started_) {
    return true;
  }
  started_ = true;

  TransitiveClosureState *state =
      query_context->getTransitiveClosureState(transitive_closure_context_index_);

  if (state->range() == 0) {
    return true;
  }

  constexpr std::size_t kMinBatchSize = 1024ul * 1024ul * 4ul;
  const std::size_t range = state->range();
  const std::size_t num_batches =
      std::max(1ul, std::min(range * range / kMinBatchSize,
                             static_cast<std::size_t>(FLAGS_num_workers)));

  const RangeSplitter splitter =
      RangeSplitter::CreateWithNumPartitions(0, range, num_batches);

  for (std::size_t i = 0; i < splitter.getNumPartitions(); ++i) {
    container->addNormalWorkOrder(
        new InitializeTransitiveClosureWorkOrder(query_id_,
                                                 splitter.getPartition(i),
                                                 state),
        op_index_);
  }

  return true;
}

bool InitializeTransitiveClosureOperator::getAllWorkOrderProtos(
    WorkOrderProtosContainer *container)  {
  LOG(FATAL) << "Not supported";
}

void InitializeTransitiveClosureWorkOrder::execute() {
  if (range_.begin() == 0) {
    state_->initializeStart();
  }
  for (std::size_t i = range_.begin(); i < range_.end(); ++i) {
    state_->initializeEdgeComponent(i);
  }
}

}  // namespace quickstep
