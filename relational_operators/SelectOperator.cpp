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

#include "relational_operators/SelectOperator.hpp"

#include <memory>
#include <vector>

#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"

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
    if (!started_) {
      if (input_relation_.hasPartitionScheme()) {
        const PartitionScheme &part_scheme =
            input_relation_.getPartitionScheme();
        int num_partitions = part_scheme.getPartitionSchemeHeader().getNumPartitions();
        for (int part_id = 0; part_id < num_partitions; ++part_id) {
          for (const block_id input_block_id :
               input_relation_block_ids_in_partition_[part_id]) {
            container->addNormalWorkOrder(
                new SelectWorkOrder(
                    input_relation_,
                    input_block_id,
                    predicate,
                    simple_projection_,
                    simple_selection_,
                    selection,
                    output_destination,
                    storage_manager,
                    placement_scheme_.getNUMANodeForBlock(input_block_id)),
                op_index_);
          }
        }
      } else {
        for (const block_id input_block_id : input_relation_block_ids_) {
          container->addNormalWorkOrder(
              new SelectWorkOrder(input_relation_,
                                  input_block_id,
                                  predicate,
                                  simple_projection_,
                                  simple_selection_,
                                  selection,
                                  output_destination,
                                  storage_manager),
              op_index_);
        }
      }
      started_ = true;
    }
    return started_;
  } else {
    if (input_relation_.hasPartitionScheme()) {
      const PartitionScheme &part_scheme =
          input_relation_.getPartitionScheme();
      int num_partitions = part_scheme.getPartitionSchemeHeader().getNumPartitions();
      for (int part_id = 0; part_id < num_partitions; ++part_id) {
        while (num_workorders_generated_in_partition_[part_id] <
               input_relation_block_ids_in_partition_[part_id].size()) {
          container->addNormalWorkOrder(
              new SelectWorkOrder(
                  input_relation_,
                  input_relation_block_ids_in_partition_
                      [part_id]
                      [num_workorders_generated_in_partition_[part_id]],
                  predicate,
                  simple_projection_,
                  simple_selection_,
                  selection,
                  output_destination,
                  storage_manager,
                  placement_scheme_.getNUMANodeForBlock(
                    input_relation_block_ids_in_partition_[part_id][num_workorders_generated_in_partition_[part_id]])),
              op_index_);
          ++num_workorders_generated_in_partition_[part_id];
        }
      }
    } else {
      while (num_workorders_generated_ < input_relation_block_ids_.size()) {
        container->addNormalWorkOrder(
            new SelectWorkOrder(
                input_relation_,
                input_relation_block_ids_[num_workorders_generated_],
                predicate,
                simple_projection_,
                simple_selection_,
                selection,
                output_destination,
                storage_manager),
            op_index_);
        ++num_workorders_generated_;
      }
    }
    return done_feeding_input_relation_;
  }
}

void SelectWorkOrder::execute() {
  BlockReference block(
      storage_manager_->getBlock(input_block_id_, input_relation_, getPreferredNUMANodes()[0]));

  if (simple_projection_) {
    block->selectSimple(simple_selection_,
                        predicate_,
                        output_destination_);
  } else {
    block->select(*DCHECK_NOTNULL(selection_),
                  predicate_,
                  output_destination_);
  }
}

}  // namespace quickstep
