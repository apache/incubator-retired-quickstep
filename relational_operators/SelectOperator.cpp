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

#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "utility/EventProfiler.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

class Predicate;

void SelectOperator::addWorkOrders(WorkOrdersContainer *container,
                                   StorageManager *storage_manager,
                                   const Predicate *predicate,
                                   const std::vector<const BloomFilter *> &bloom_filters,
                                   const std::vector<attribute_id> &bloom_filter_attribute_ids,
                                   const std::vector<std::unique_ptr<const Scalar>> *selection,
                                   InsertDestination *output_destination) {
  if (input_relation_is_stored_) {
    for (const block_id input_block_id : input_relation_block_ids_) {
      container->addNormalWorkOrder(new SelectWorkOrder(query_id_,
                                                        input_relation_,
                                                        input_block_id,
                                                        predicate,
                                                        bloom_filters,
                                                        bloom_filter_attribute_ids,
                                                        simple_projection_,
                                                        simple_selection_,
                                                        selection,
                                                        output_destination,
                                                        storage_manager,
                                                        0,
                                                        op_index_),
                                    op_index_);
    }
  } else {
    while (num_workorders_generated_ < input_relation_block_ids_.size()) {
      container->addNormalWorkOrder(
          new SelectWorkOrder(
              query_id_,
              input_relation_,
              input_relation_block_ids_[num_workorders_generated_],
              predicate,
              bloom_filters,
              bloom_filter_attribute_ids,
              simple_projection_,
              simple_selection_,
              selection,
              output_destination,
              storage_manager,
              0,
              op_index_),
          op_index_);
      ++num_workorders_generated_;
    }
  }
}

#ifdef QUICKSTEP_HAVE_LIBNUMA
void SelectOperator::addPartitionAwareWorkOrders(WorkOrdersContainer *container,
                                                 StorageManager *storage_manager,
                                                 const Predicate *predicate,
                                                 const std::vector<const BloomFilter *> &bloom_filters,
                                                 const std::vector<attribute_id> &bloom_filter_attribute_ids,
                                                 const std::vector<std::unique_ptr<const Scalar>> *selection,
                                                 InsertDestination *output_destination) {
  DCHECK(placement_scheme_ != nullptr);
  const std::size_t num_partitions = input_relation_.getPartitionScheme().getPartitionSchemeHeader().getNumPartitions();
  if (input_relation_is_stored_) {
    for (std::size_t part_id = 0; part_id < num_partitions; ++part_id) {
      for (const block_id input_block_id :
           input_relation_block_ids_in_partition_[part_id]) {
        container->addNormalWorkOrder(
            new SelectWorkOrder(
                query_id_,
                input_relation_,
                input_block_id,
                predicate,
                bloom_filters,
                bloom_filter_attribute_ids,
                simple_projection_,
                simple_selection_,
                selection,
                output_destination,
                storage_manager,
                placement_scheme_->getNUMANodeForBlock(input_block_id),
                op_index_),
            op_index_);
      }
    }
  } else {
    for (std::size_t part_id = 0; part_id < num_partitions; ++part_id) {
      while (num_workorders_generated_in_partition_[part_id] <
             input_relation_block_ids_in_partition_[part_id].size()) {
        block_id block_in_partition
            = input_relation_block_ids_in_partition_[part_id][num_workorders_generated_in_partition_[part_id]];
        container->addNormalWorkOrder(
            new SelectWorkOrder(
                query_id_,
                input_relation_,
                block_in_partition,
                predicate,
                bloom_filters,
                bloom_filter_attribute_ids,
                simple_projection_,
                simple_selection_,
                selection,
                output_destination,
                storage_manager,
                placement_scheme_->getNUMANodeForBlock(block_in_partition),
                op_index_),
            op_index_);
        ++num_workorders_generated_in_partition_[part_id];
      }
    }
  }
}
#endif

bool SelectOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  DCHECK(query_context != nullptr);

  if (bloom_filters_ == nullptr) {
    bloom_filters_.reset(new std::vector<const BloomFilter*>());
    for (const auto bloom_filter_id : bloom_filter_ids_) {
      // Add the pointer to the probe bloom filter within the list of probe bloom filters to use.
      bloom_filters_->emplace_back(
          query_context->getBloomFilter(bloom_filter_id));
    }
  }

  const Predicate *predicate =
      query_context->getPredicate(predicate_index_);
  const std::vector<std::unique_ptr<const Scalar>> *selection =
      simple_projection_
          ? nullptr
          : &query_context->getScalarGroup(selection_index_);
  InsertDestination *output_destination =
      query_context->getInsertDestination(output_destination_index_);

  if (input_relation_is_stored_) {
    if (!started_) {
      if (input_relation_.hasPartitionScheme()) {
#ifdef QUICKSTEP_HAVE_LIBNUMA
        if (input_relation_.hasNUMAPlacementScheme()) {
          addPartitionAwareWorkOrders(container,
                                      storage_manager,
                                      predicate,
                                      *bloom_filters_,
                                      bloom_filter_attribute_ids_,
                                      selection,
                                      output_destination);
        }
#endif
      } else {
        addWorkOrders(container,
                      storage_manager,
                      predicate,
                      *bloom_filters_,
                      bloom_filter_attribute_ids_,
                      selection,
                      output_destination);
      }
      started_ = true;
    }
    return started_;
  } else {
    if (input_relation_.hasPartitionScheme()) {
#ifdef QUICKSTEP_HAVE_LIBNUMA
        if (input_relation_.hasNUMAPlacementScheme()) {
          addPartitionAwareWorkOrders(container,
                                      storage_manager,
                                      predicate,
                                      *bloom_filters_,
                                      bloom_filter_attribute_ids_,
                                      selection,
                                      output_destination);
        }
#endif
    } else {
        addWorkOrders(container,
                      storage_manager,
                      predicate,
                      *bloom_filters_,
                      bloom_filter_attribute_ids_,
                      selection,
                      output_destination);
    }
    return done_feeding_input_relation_;
  }
}

bool SelectOperator::getAllWorkOrderProtos(WorkOrderProtosContainer *container) {
  if (input_relation_is_stored_) {
    if (!started_) {
      for (const block_id input_block_id : input_relation_block_ids_) {
        container->addWorkOrderProto(createWorkOrderProto(input_block_id), op_index_);
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

serialization::WorkOrder* SelectOperator::createWorkOrderProto(const block_id block) {
  serialization::WorkOrder *proto = new serialization::WorkOrder;
  proto->set_work_order_type(serialization::SELECT);
  proto->set_query_id(query_id_);

  proto->SetExtension(serialization::SelectWorkOrder::relation_id, input_relation_.getID());
  proto->SetExtension(serialization::SelectWorkOrder::insert_destination_index, output_destination_index_);
  proto->SetExtension(serialization::SelectWorkOrder::predicate_index, predicate_index_);
  proto->SetExtension(serialization::SelectWorkOrder::block_id, block);
  proto->SetExtension(serialization::SelectWorkOrder::simple_projection, simple_projection_);
  if (simple_projection_) {
    for (const attribute_id attr_id : simple_selection_) {
      proto->AddExtension(serialization::SelectWorkOrder::simple_selection, attr_id);
    }
  }
  proto->SetExtension(serialization::SelectWorkOrder::selection_index, selection_index_);

  return proto;
}


void SelectWorkOrder::execute() {
//  auto *container = simple_profiler.getContainer();
//  container->setContext(getOperatorIndex());
//  auto *line = container->getEventLine(0);
//  line->emplace_back();
  BlockReference block(
      storage_manager_->getBlock(input_block_id_, input_relation_, getPreferredNUMANodes()[0]));

  if (simple_projection_) {
    block->selectSimple(simple_selection_,
                        predicate_,
                        bloom_filters_,
                        bloom_filter_attribute_ids_,
                        output_destination_);
  } else {
    block->select(*DCHECK_NOTNULL(selection_),
                  predicate_,
                  bloom_filters_,
                  bloom_filter_attribute_ids_,
                  output_destination_);
  }
//  line->back().endEvent();
//  line->back().setPayload(getOperatorIndex(), 0);
}

}  // namespace quickstep
