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

#include "relational_operators/SelectOperator.hpp"

#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/ValueAccessor.hpp"
#include "utility/lip_filter/LIPFilterAdaptiveProber.hpp"
#include "utility/lip_filter/LIPFilterUtil.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

class Predicate;

bool SelectOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  DCHECK(query_context != nullptr);

  const Predicate *predicate =
      query_context->getPredicate(predicate_index_);
  const std::vector<std::unique_ptr<const Scalar>> *selection =
      simple_projection_
          ? nullptr
          : &query_context->getScalarGroup(selection_index_);
  InsertDestination *output_destination =
      query_context->getInsertDestination(output_destination_index_);

  if (input_relation_is_stored_) {
    if (started_) {
      return true;
    }

    for (std::size_t part_id = 0; part_id < num_partitions_; ++part_id) {
      for (const block_id input_block_id : input_relation_block_ids_[part_id]) {
        numa_node_id numa_node = 0;
#ifdef QUICKSTEP_HAVE_LIBNUMA
        if (input_relation_.hasNUMAPlacementScheme()) {
          numa_node = placement_scheme_->getNUMANodeForBlock(input_block_id);
        }
#endif  // QUICKSTEP_HAVE_LIBNUMA
        container->addNormalWorkOrder(
            new SelectWorkOrder(query_id_, input_relation_, part_id, input_block_id, predicate, simple_projection_,
                                simple_selection_, selection, output_destination, storage_manager,
                                CreateLIPFilterAdaptiveProberHelper(lip_deployment_index_, query_context), numa_node),
            op_index_);
      }
    }
    started_ = true;
    return true;
  } else {
    for (std::size_t part_id = 0; part_id < num_partitions_; ++part_id) {
      while (num_workorders_generated_[part_id] < input_relation_block_ids_[part_id].size()) {
        const block_id block = input_relation_block_ids_[part_id][num_workorders_generated_[part_id]];

        numa_node_id numa_node = 0;
#ifdef QUICKSTEP_HAVE_LIBNUMA
        if (input_relation_.hasNUMAPlacementScheme()) {
          numa_node = placement_scheme_->getNUMANodeForBlock(block);
        }
#endif  // QUICKSTEP_HAVE_LIBNUMA
        container->addNormalWorkOrder(
            new SelectWorkOrder(query_id_, input_relation_, part_id, block, predicate, simple_projection_,
                                simple_selection_, selection, output_destination, storage_manager,
                                CreateLIPFilterAdaptiveProberHelper(lip_deployment_index_, query_context), numa_node),
            op_index_);
        ++num_workorders_generated_[part_id];
      }
    }
    return done_feeding_input_relation_;
  }
}

bool SelectOperator::getAllWorkOrderProtos(WorkOrderProtosContainer *container) {
  if (input_relation_is_stored_) {
    if (started_) {
      return true;
    }

    for (std::size_t part_id = 0; part_id < num_partitions_; ++part_id) {
      for (const block_id input_block_id : input_relation_block_ids_[part_id]) {
        container->addWorkOrderProto(createWorkOrderProto(part_id, input_block_id), op_index_);
      }
    }
    started_ = true;
    return true;
  } else {
    for (std::size_t part_id = 0; part_id < num_partitions_; ++part_id) {
      while (num_workorders_generated_[part_id] < input_relation_block_ids_[part_id].size()) {
        container->addWorkOrderProto(
            createWorkOrderProto(part_id, input_relation_block_ids_[part_id][num_workorders_generated_[part_id]]),
            op_index_);
        ++num_workorders_generated_[part_id];
      }
    }
    return done_feeding_input_relation_;
  }
}

serialization::WorkOrder* SelectOperator::createWorkOrderProto(const partition_id part_id, const block_id block) {
  serialization::WorkOrder *proto = new serialization::WorkOrder;
  proto->set_work_order_type(serialization::SELECT);
  proto->set_query_id(query_id_);

  proto->SetExtension(serialization::SelectWorkOrder::relation_id, input_relation_.getID());
  proto->SetExtension(serialization::SelectWorkOrder::insert_destination_index, output_destination_index_);
  proto->SetExtension(serialization::SelectWorkOrder::predicate_index, predicate_index_);
  proto->SetExtension(serialization::SelectWorkOrder::partition_id, part_id);
  proto->SetExtension(serialization::SelectWorkOrder::block_id, block);
  proto->SetExtension(serialization::SelectWorkOrder::simple_projection, simple_projection_);
  if (simple_projection_) {
    for (const attribute_id attr_id : simple_selection_) {
      proto->AddExtension(serialization::SelectWorkOrder::simple_selection, attr_id);
    }
  }
  proto->SetExtension(serialization::SelectWorkOrder::selection_index, selection_index_);
  proto->SetExtension(serialization::SelectWorkOrder::lip_deployment_index, lip_deployment_index_);

  for (const QueryContext::lip_filter_id lip_filter_index : lip_filter_indexes_) {
    proto->AddExtension(serialization::SelectWorkOrder::lip_filter_indexes, lip_filter_index);
  }

  return proto;
}

void SelectWorkOrder::execute() {
  output_destination_->setInputPartitionId(partition_id_);

  BlockReference block(
      storage_manager_->getBlock(input_block_id_, input_relation_, getPreferredNUMANodes()[0]));

  // NOTE(jianqiao): For SSB and TPCH queries, experiments show that it is almost
  // always better to apply the predicate BEFORE the LIPFilters. But as a future
  // work this ordering may even be adaptive.
  std::unique_ptr<TupleIdSequence> predicate_matches;
  if (predicate_ != nullptr) {
    predicate_matches.reset(
        block->getMatchesForPredicate(predicate_));
  }

  std::unique_ptr<TupleIdSequence> matches;
  if (lip_filter_adaptive_prober_ != nullptr) {
    std::unique_ptr<ValueAccessor> accessor(
        block->getTupleStorageSubBlock().createValueAccessor(predicate_matches.get()));
    matches.reset(
        lip_filter_adaptive_prober_->filterValueAccessor(accessor.get()));
  } else {
    matches.reset(predicate_matches.release());
  }

  if (simple_projection_) {
    block->selectSimple(simple_selection_,
                        matches.get(),
                        output_destination_);
  } else {
    block->select(*DCHECK_NOTNULL(selection_),
                  matches.get(),
                  output_destination_);
  }
}

}  // namespace quickstep
