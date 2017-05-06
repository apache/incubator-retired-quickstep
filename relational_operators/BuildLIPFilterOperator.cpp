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

#include "relational_operators/BuildLIPFilterOperator.hpp"

#include <memory>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "storage/ValueAccessor.hpp"
#include "utility/lip_filter/LIPFilterAdaptiveProber.hpp"
#include "utility/lip_filter/LIPFilterBuilder.hpp"
#include "utility/lip_filter/LIPFilterUtil.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

bool BuildLIPFilterOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  DCHECK(query_context != nullptr);

  const Predicate *build_side_predicate =
      query_context->getPredicate(build_side_predicate_index_);

  if (input_relation_is_stored_) {
    if (!started_) {
      for (const block_id input_block_id : input_relation_block_ids_) {
        container->addNormalWorkOrder(
            new BuildLIPFilterWorkOrder(
                query_id_,
                input_relation_,
                input_block_id,
                build_side_predicate,
                storage_manager,
                CreateLIPFilterAdaptiveProberHelper(lip_deployment_index_, query_context),
                CreateLIPFilterBuilderHelper(lip_deployment_index_, query_context)),
            op_index_);
      }
      started_ = true;
    }
    return true;
  } else {
    while (num_workorders_generated_ < input_relation_block_ids_.size()) {
      container->addNormalWorkOrder(
          new BuildLIPFilterWorkOrder(
              query_id_,
              input_relation_,
              input_relation_block_ids_[num_workorders_generated_],
              build_side_predicate,
              storage_manager,
              CreateLIPFilterAdaptiveProberHelper(lip_deployment_index_, query_context),
              CreateLIPFilterBuilderHelper(lip_deployment_index_, query_context)),
          op_index_);
      ++num_workorders_generated_;
    }
    return done_feeding_input_relation_;
  }
}

bool BuildLIPFilterOperator::getAllWorkOrderProtos(WorkOrderProtosContainer *container) {
  if (input_relation_is_stored_) {
    if (!started_) {
      for (const block_id block : input_relation_block_ids_) {
        container->addWorkOrderProto(createWorkOrderProto(block), op_index_);
      }
      started_ = true;
    }
    return true;
  } else {
    while (num_workorders_generated_ < input_relation_block_ids_.size()) {
      container->addWorkOrderProto(
          createWorkOrderProto(input_relation_block_ids_[num_workorders_generated_]),
          op_index_);
      ++num_workorders_generated_;
    }
    return done_feeding_input_relation_;
  }
}

serialization::WorkOrder* BuildLIPFilterOperator::createWorkOrderProto(const block_id block) {
  serialization::WorkOrder *proto = new serialization::WorkOrder;
  proto->set_work_order_type(serialization::BUILD_LIP_FILTER);
  proto->set_query_id(query_id_);

  proto->SetExtension(serialization::BuildLIPFilterWorkOrder::relation_id, input_relation_.getID());
  proto->SetExtension(serialization::BuildLIPFilterWorkOrder::build_block_id, block);
  proto->SetExtension(serialization::BuildLIPFilterWorkOrder::build_side_predicate_index,
                      build_side_predicate_index_);
  proto->SetExtension(serialization::BuildLIPFilterWorkOrder::lip_deployment_index, lip_deployment_index_);

  for (const QueryContext::lip_filter_id lip_filter_index : lip_filter_indexes_) {
    proto->AddExtension(serialization::BuildLIPFilterWorkOrder::lip_filter_indexes, lip_filter_index);
  }

  return proto;
}

void BuildLIPFilterWorkOrder::execute() {
  BlockReference block(
      storage_manager_->getBlock(build_block_id_, input_relation_));

  // Apply the predicate first.
  std::unique_ptr<TupleIdSequence> predicate_matches;
  if (build_side_predicate_ != nullptr) {
    predicate_matches.reset(block->getMatchesForPredicate(build_side_predicate_));
  }

  std::unique_ptr<ValueAccessor> accessor(
      block->getTupleStorageSubBlock().createValueAccessor(predicate_matches.get()));

  if (lip_filter_adaptive_prober_ != nullptr) {
    // Probe the LIP filters if there are any. Note that the LIP filters to be
    // probed are for filtering the input relation. They are distinct from the
    // target LIP filters we are building.
    std::unique_ptr<TupleIdSequence> matches(
        lip_filter_adaptive_prober_->filterValueAccessor(accessor.get()));
    std::unique_ptr<ValueAccessor> filtered_accessor(
        accessor->createSharedTupleIdSequenceAdapterVirtual(*matches));

    lip_filter_builder_->insertValueAccessor(filtered_accessor.get());
  } else {
    lip_filter_builder_->insertValueAccessor(accessor.get());
  }
}

}  // namespace quickstep
