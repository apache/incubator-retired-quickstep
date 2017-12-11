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

#include "relational_operators/TransitiveClosureOperator.hpp"

#include <algorithm>
#include <cstddef>

#include "cli/Flags.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/TransitiveClosureState.hpp"
#include "types/IntType.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "utility/BitVector.hpp"
#include "utility/Range.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

bool TransitiveClosureOperator::getAllWorkOrders(
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

  InsertDestination *output_destination =
      query_context->getInsertDestination(output_destination_index_);

  const std::size_t num_batches = std::min(state->range(), FLAGS_num_workers * 2);
  const RangeSplitter splitter =
      RangeSplitter::CreateWithNumPartitions(0, state->range(), num_batches);

  for (std::size_t i = 0; i < splitter.getNumPartitions(); ++i) {
    container->addNormalWorkOrder(
        new TransitiveClosureWorkOrder(query_id_,
                                       splitter.getPartition(i),
                                       state,
                                       output_destination),
        op_index_);
  }

  return true;
}

bool TransitiveClosureOperator::getAllWorkOrderProtos(
    WorkOrderProtosContainer *container)  {
  LOG(FATAL) << "Not supported";
}

void TransitiveClosureWorkOrder::execute() {
  std::vector<int> delta;
  delta.reserve(range_);
  BitVector<false> next(range_, false);
  BitVector<false> result(range_, false);

  const int kBulkInsertBatchSize = std::max(0x10000, state_->range());

  std::shared_ptr<NativeColumnVector> src_cv =
      std::make_shared<NativeColumnVector>(IntType::InstanceNonNullable(),
                                           kBulkInsertBatchSize);
  std::shared_ptr<NativeColumnVector> dst_cv =
      std::make_shared<NativeColumnVector>(IntType::InstanceNonNullable(),
                                           kBulkInsertBatchSize);

  int total = 0;
  for (int src = interval_.begin(); src < interval_.end(); ++src) {
    if (state_->hasStart(src)) {
      // Evaluate single source transitive closure.
      evaluateSingleSource(src, &delta, &next, &result);

      const int num_values = result.onesCount();
      if (total + num_values > kBulkInsertBatchSize) {
        bulkInsert(src_cv, dst_cv);
        src_cv->clear();
        dst_cv->clear();
        total = 0;
      }

      std::size_t dst = -1;
      for (int i = 0; i < num_values; ++i) {
        *static_cast<int*>(src_cv->getPtrForDirectWrite()) = src;
        dst = result.firstOne(dst + 1);
        *static_cast<int*>(dst_cv->getPtrForDirectWrite()) = dst;
      }

      total += num_values;
    }
  }
  if (total > 0) {
    bulkInsert(src_cv, dst_cv);
  }
}

void TransitiveClosureWorkOrder::evaluateSingleSource(
    const int start,
    std::vector<int> *delta,
    BitVector<false> *next,
    BitVector<false> *result) const {
  delta->clear();
  delta->emplace_back(start);
  result->clear();

  while (!delta->empty()) {
    next->clear();
    for (const int source : *delta) {
      next->unionWith(state_->getEdgeData(source));
    }
    delta->clear();
    next->subtractTo(*result, delta);
    result->unionWith(*next);
  }
}

void TransitiveClosureWorkOrder::bulkInsert(const ColumnVectorPtr &src_cv,
                                            const ColumnVectorPtr &dst_cv) {
  ColumnVectorsValueAccessor columns;
  columns.addColumn(src_cv);
  columns.addColumn(dst_cv);
  output_destination_->bulkInsertTuples(&columns);
}

}  // namespace quickstep

