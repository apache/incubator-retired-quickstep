/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
 *   Copyright 2016 Pivotal Software, Inc.
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

#include "relational_operators/SampleOperator.hpp"

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

bool SampleOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id foreman_client_id,
    const tmb::client_id agent_client_id,
    tmb::MessageBus *bus) {
  DCHECK(query_context != nullptr);

  InsertDestination *output_destination =
      query_context->getInsertDestination(output_destination_index_);
  std::random_device random_device;
  std::mt19937 generator(random_device());
  std::uniform_real_distribution<> distribution(0, 1);
  const double probability = static_cast<double>(percentage_) / 100;
  if (input_relation_is_stored_) {
    if (!started_) {
      // If the sampling is by block choose blocks randomly
      if (is_block_sample_) {
        for (const block_id input_block_id : input_relation_block_ids_) {
          if (distribution(generator) <= probability) {
            container->addNormalWorkOrder(
                new SampleWorkOrder(input_relation_,
                                    input_block_id,
                                    is_block_sample_,
                                    percentage_,
                                    output_destination,
                                    storage_manager),
                op_index_);
          }
        }
      } else {
        // Add all the blocks for tuple sampling which would handle
        // the sampling from each block
        for (const block_id input_block_id : input_relation_block_ids_) {
          container->addNormalWorkOrder(
              new SampleWorkOrder(input_relation_,
                                  input_block_id,
                                  is_block_sample_,
                                  percentage_,
                                  output_destination,
                                  storage_manager),
              op_index_);
        }
      }
      started_ = true;
    }
    return started_;
  } else {
      if (is_block_sample_) {
          while (num_workorders_generated_ < input_relation_block_ids_.size()) {
            if (distribution(generator) <= probability) {
              container->addNormalWorkOrder(
                  new SampleWorkOrder(input_relation_,
                                      input_relation_block_ids_[num_workorders_generated_],
                                      is_block_sample_,
                                      percentage_,
                                      output_destination,
                                      storage_manager),
                  op_index_);
            ++num_workorders_generated_;
          }
        }
      } else  {
        while (num_workorders_generated_ < input_relation_block_ids_.size()) {
          container->addNormalWorkOrder(
              new SampleWorkOrder(input_relation_,
                                  input_relation_block_ids_[num_workorders_generated_],
                                  is_block_sample_,
                                  percentage_, output_destination,
                                  storage_manager),
              op_index_);
          ++num_workorders_generated_;
        }
      }
    return done_feeding_input_relation_;
  }
}
void SampleWorkOrder::execute() {
  BlockReference block(
      storage_manager_->getBlock(input_block_id_, input_relation_));

  block->sample(is_block_sample_, percentage_, output_destination_);
}

}  // namespace quickstep
