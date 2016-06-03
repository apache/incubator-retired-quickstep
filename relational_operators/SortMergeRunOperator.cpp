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

#include "relational_operators/SortMergeRunOperator.hpp"

#include <cstddef>
#include <cstdlib>
#include <utility>
#include <vector>

#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/SortMergeRunOperator.pb.h"
#include "relational_operators/SortMergeRunOperatorHelpers.hpp"
#include "threading/ThreadIDBasedMap.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

class InsertDestination;

using merge_run_operator::Run;
using merge_run_operator::RunMerger;
using merge_run_operator::MergeTree;

bool SortMergeRunOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  if (input_relation_is_stored_) {
    // Input blocks (or runs) are from base relation. Only possible when base
    // relation is stored sorted.
    if (!started_) {
      // Initialize merge tree completely, since all input runs are known.
      merge_tree_.initializeTree(input_relation_block_ids_.size());
      started_ = true;
      initializeInputRuns();
    }
  } else {
    // Input blocks (or runs) are pipelined from the sorted run generation
    // operator.
    if (!started_ && !input_stream_done_) {
      // Initialize merge tree for first pipeline mode.
      merge_tree_.initializeForPipeline();
      started_ = true;
      initializeInputRuns();
    }
  }
  // Generate runs from merge tree.
  return generateWorkOrders(container, query_context, storage_manager, scheduler_client_id, bus);
}

WorkOrder *SortMergeRunOperator::createWorkOrder(
    merge_run_operator::MergeTree::MergeJob *job,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  DCHECK(!job->runs.empty());
  DCHECK(query_context != nullptr);

  InsertDestination *output_destination =
      query_context->getInsertDestination(job->is_final_level
                                              ? output_destination_index_
                                              : run_block_destination_index_);

  // Create a work order from the merge job from merge tree.
  return new SortMergeRunWorkOrder(
      query_id_,
      query_context->getSortConfig(sort_config_index_),
      job->level > 0 ? run_relation_ : input_relation_,
      std::move(job->runs),
      top_k_,
      job->level,
      output_destination,
      storage_manager,
      op_index_,
      scheduler_client_id,
      bus);
}

bool SortMergeRunOperator::generateWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  std::vector<MergeTree::MergeJob> jobs;

  // Get merge jobs from merge tree.
  bool done_generating = merge_tree_.getMergeJobs(&jobs);

  for (std::vector<MergeTree::MergeJob>::size_type job_id = 0;
       job_id != jobs.size();
       ++job_id) {
    // Add work order for each merge job.
    container->addNormalWorkOrder(createWorkOrder(&jobs[job_id],
                                                  query_context,
                                                  storage_manager,
                                                  scheduler_client_id,
                                                  bus),
                                  op_index_);
  }

  return done_generating;
}

void SortMergeRunOperator::initializeInputRuns() {
  DCHECK(started_);
  if (input_relation_is_stored_) {
    // Input blocks are from base relation; add all blocks.
    merge_tree_.addInputBlocks(input_relation_block_ids_);
  } else {
    // Input blocks are pipelined; add only newly available blocks.
    if (num_input_workorders_generated_ < input_relation_block_ids_.size()) {
      std::vector<block_id> new_blocks(
          input_relation_block_ids_.begin() + num_input_workorders_generated_,
          input_relation_block_ids_.end());
      merge_tree_.addInputBlocks(new_blocks);
      num_input_workorders_generated_ += new_blocks.size();
    }
  }
}

void SortMergeRunOperator::doneFeedingInputBlocks(const relation_id input_relation_id) {
  if (input_relation_is_stored_) {
    return;
  }

  // Now we know all the input blocks; compute the merge tree.
  merge_tree_.initializeTree(input_relation_block_ids_.size());

  // Initialize runs for the final time.
  initializeInputRuns();

  // If the final merge was already scheduled, fix it to write to correct output
  // destinaton.
  merge_tree_.checkAndFixFinalMerge();
}

namespace {

/**
 * @brief Object to store the output run of a merge step with methods to
 * serialize and deserialize from its corresponding protobuf.
 **/
class SortMergeRunOutput {
 public:
  /**
   * @brief Constructor with move semantics for creating the object from a
   * vector of block_ids.
   *
   * @param merge_level Level of the current merge.
   * @param blocks Rvalue of the vector of blocks in the output.
   **/
  SortMergeRunOutput(std::size_t merge_level, std::vector<block_id> &&blocks)
      : merge_level_(merge_level), blocks_(std::move(blocks)) {}

  /**
   * @brief Constructor from protobuf object.
   *
   * @param proto Protobuf of merged run.
   **/
  explicit SortMergeRunOutput(serialization::SortMergeRunOutput &proto)
      : merge_level_(static_cast<std::size_t>(proto.merge_level())) {
    blocks_.reserve(proto.blocks_size());
    for (const auto block : proto.blocks()) {
      blocks_.emplace_back(block);
    }
  }

  /**
   * @brief Method to serialize the object.
   *
   * @return Pair with pointer serialized blob and size of the blob.
   **/
  std::pair<void *, std::size_t> serialize() const {
    serialization::SortMergeRunOutput msg;
    msg.set_merge_level(merge_level_);
    for (const block_id block : blocks_) {
      msg.add_blocks(block);
    }

    const std::size_t msg_size = msg.ByteSize();
    char *msg_bytes = static_cast<char *>(std::malloc(msg_size));
    CHECK(msg.SerializeToArray(msg_bytes, msg_size));
    return std::make_pair(msg_bytes, msg_size);
  }

  /**
   * @brief Get merge level.
   *
   * @return Merge level.
   **/
  std::size_t getMergeLevel() const { return merge_level_; }

  /**
   * @brief Get mutable pointer to the blocks in the run.
   *
   * @return Pointer to vector of block_ids in the run.
   **/
  std::vector<block_id>* getBlocksMutable() { return &blocks_; }

 private:
  std::size_t merge_level_;
  std::vector<block_id> blocks_;
};

}  // namespace

void SortMergeRunOperator::receiveFeedbackMessage(
    const WorkOrder::FeedbackMessage &msg) {
  CHECK(SortMergeRunOperator::kRunOutputMessage == msg.type());

  // Deserialize completion message from merge run work order.
  serialization::SortMergeRunOutput run_output_proto;
  CHECK(run_output_proto.ParseFromArray(msg.payload(), msg.payload_size()));

  // Write the output run to merge tree.
  SortMergeRunOutput run_output(run_output_proto);
  merge_tree_.writeOutputRun(run_output.getMergeLevel(),
                             run_output.getBlocksMutable());
}

void SortMergeRunWorkOrder::execute() {
  // Merge input runs.
  merge_run_operator::RunMerger run_merger(sort_config_,
                                           std::move(input_runs_),
                                           top_k_,
                                           run_relation_,
                                           output_destination_,
                                           merge_level_,
                                           storage_manager_);
  run_merger.doMerge();

  // Serialize completion message with output run.
  SortMergeRunOutput output(run_merger.getMergeLevel(),
                            std::move(*run_merger.getOutputRunMutable()));
  std::pair<void *, std::size_t> serialized_output = output.serialize();

  // Send completion message to operator.
  FeedbackMessage msg(SortMergeRunOperator::kRunOutputMessage,
                      operator_index_,
                      serialized_output.first,
                      serialized_output.second);
  SendFeedbackMessage(
      bus_, ClientIDMap::Instance()->getValue(), scheduler_client_id_, msg);
}

}  // namespace quickstep
