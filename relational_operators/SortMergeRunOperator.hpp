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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_SORT_MERGE_RUN_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_SORT_MERGE_RUN_OPERATOR_HPP_

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/SortMergeRunOperatorHelpers.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"
#include "utility/SortConfiguration.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class CatalogRelationSchema;
class InsertDestination;
class StorageManager;
class WorkOrderProtosContainer;
class WorkOrdersContainer;

namespace serialization { class WorkOrder; }

/**
 * @defgroup SortMergeRun Merging Sorted Runs
 * @ingroup Sort
 * In this phase, we merge the runs generated in the previous phase (\ref
 * SortRunGeneration). We perform the run merge \c k runs at a time (i.e., k-way
 * merge implemented as \ref RunMerger). Using this k-way merge as a building
 * block, we create a merge tree (\ref MergeTree) of k-way merges to eventually
 * produce a single sorted run.
 */

/** @addtogroup SortMergeRun
 *  @{
 */

/**
 * @brief An operator which merges sort blocks and writes a sorted relation.
 **/
class SortMergeRunOperator : public RelationalOperator {
 public:
  enum FeedbackMessageType : WorkOrder::FeedbackMessageType {
      kRunOutputMessage,
  };

  /**
   * @brief Constructor for merging sorted runs to generate a sorted relation.
   *
   * @param query_id The ID of the query to which this operator belongs.
   * @param input_relation The relation to merge sorted blocks.
   * @param output_relation The output relation.
   * @param output_destination_index The index of the InsertDestination in the
   *        QueryContext to store the sorted blocks in.
   * @param run_relation The temporary relation used to store intermediate runs
   *                     of blocks.
   * @param run_block_destination_index The index of the InsertDestination in
   *        the QueryContext to store the intermediate blocks in the merging
   *        process.
   * @param sort_config_index The index of the Sort configuration in
   *        QueryContext.
   * @param merge_factor Merge factor of this operator.
   * @param top_k Only return the first \c top_k results. Return all results if
   *              \c top_k is 0.
   * @param input_relation_is_stored Boolean to indicate is input relation is
   *                                 stored or streamed.
   **/
  SortMergeRunOperator(
      const std::size_t query_id,
      const CatalogRelation &input_relation,
      const CatalogRelation &output_relation,
      const QueryContext::insert_destination_id output_destination_index,
      const CatalogRelation &run_relation,
      const QueryContext::insert_destination_id run_block_destination_index,
      const QueryContext::sort_config_id sort_config_index,
      const std::size_t merge_factor,
      const std::size_t top_k,
      const bool input_relation_is_stored)
      : RelationalOperator(query_id),
        input_relation_(input_relation),
        output_relation_(output_relation),
        output_destination_index_(output_destination_index),
        sort_config_index_(sort_config_index),
        merge_factor_(merge_factor),
        top_k_(top_k),
        merge_tree_(merge_factor_),
        input_relation_block_ids_(input_relation_is_stored
                                      ? input_relation.getBlocksSnapshot()
                                      : std::vector<block_id>()),
        num_input_workorders_generated_(0),
        run_relation_(run_relation),
        run_block_destination_index_(run_block_destination_index),
        input_relation_is_stored_(input_relation_is_stored),
        input_stream_done_(input_relation_is_stored),
        started_(false) {
    DCHECK_GT(merge_factor_, 1u);
  }

  /**
   * @brief Destructor.
   **/
  ~SortMergeRunOperator() {}

  std::string getName() const override {
    return "SortMergeRunOperator";
  }

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

  bool getAllWorkOrderProtos(WorkOrderProtosContainer *container) override;

  void feedInputBlock(const block_id input_block_id, const relation_id input_relation_id,
                      const partition_id part_id) override {
    input_relation_block_ids_.push_back(input_block_id);
    if (started_) {
      initializeInputRuns();
    }
  }

  void doneFeedingInputBlocks(const relation_id input_relation_id) override;

  void receiveFeedbackMessage(const WorkOrder::FeedbackMessage &msg) override;

  QueryContext::insert_destination_id getInsertDestinationID() const override {
    return output_destination_index_;
  }

  const relation_id getOutputRelationID() const override {
    return output_relation_.getID();
  }

 private:
  // Initialize runs for the first pass. This needs to be called every time new
  // blocks are input to the operator in the case of pipelined input.
  void initializeInputRuns();

  // Generate work orders for the current state of operator.
  bool generateWorkOrders(WorkOrdersContainer *container,
                          QueryContext *query_context,
                          StorageManager *storage_manager,
                          const tmb::client_id scheduler_client_id,
                          tmb::MessageBus *bus);

  // Create a merge work order for the given merge job.
  WorkOrder *createWorkOrder(merge_run_operator::MergeTree::MergeJob *job,
                             QueryContext *query_context,
                             StorageManager *storage_manager,
                             const tmb::client_id scheduler_client_id,
                             tmb::MessageBus *bus);

  /**
   * @brief Create Work Order proto.
   *
   * @param job The merge job.
   **/
  serialization::WorkOrder* createWorkOrderProto(merge_run_operator::MergeTree::MergeJob *job);

  const CatalogRelation &input_relation_;

  const CatalogRelation &output_relation_;
  const QueryContext::insert_destination_id output_destination_index_;

  const QueryContext::sort_config_id sort_config_index_;
  const std::size_t merge_factor_;
  const std::size_t top_k_;
  merge_run_operator::MergeTree merge_tree_;  // Merge tree for sorting.

  std::vector<block_id> input_relation_block_ids_;
  std::vector<block_id>::size_type num_input_workorders_generated_;

  const CatalogRelation &run_relation_;
  const QueryContext::insert_destination_id run_block_destination_index_;

  const bool input_relation_is_stored_;
  const bool input_stream_done_;

  bool started_;

  DISALLOW_COPY_AND_ASSIGN(SortMergeRunOperator);
};

/**
 * @brief A work order to merge sorted input runs into a single sorted output
 * run.
 */
class SortMergeRunWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param query_id The ID of the query to which this WorkOrder belongs.
   * @param sort_config The Sort configuration.
   * @param run_relation The relation to which the run blocks belong to.
   * @param input_runs Input runs to merge.
   * @param top_k If non-zero will merge only \c top_k tuples.
   * @param merge_level Merge level in the merge tree.
   * @param output_destination The InsertDestination to create new blocks.
   * @param storage_manager The StorageManager to use.
   * @param operator_index Merge-run operator index to send feedback messages
   *                       to.
   * @param scheduler_client_id The TMB client ID of the scheduler thread.
   * @param bus TMB to send the feedback message on.
   **/
  SortMergeRunWorkOrder(
      const std::size_t query_id,
      const SortConfiguration &sort_config,
      const CatalogRelationSchema &run_relation,
      std::vector<merge_run_operator::Run> &&input_runs,
      const std::size_t top_k,
      const std::size_t merge_level,
      InsertDestination *output_destination,
      StorageManager *storage_manager,
      const std::size_t operator_index,
      const tmb::client_id scheduler_client_id,
      MessageBus *bus)
      : WorkOrder(query_id),
        sort_config_(sort_config),
        run_relation_(run_relation),
        input_runs_(std::move(input_runs)),
        top_k_(top_k),
        merge_level_(merge_level),
        output_destination_(DCHECK_NOTNULL(output_destination)),
        storage_manager_(DCHECK_NOTNULL(storage_manager)),
        operator_index_(operator_index),
        scheduler_client_id_(scheduler_client_id),
        bus_(DCHECK_NOTNULL(bus)) {
    DCHECK(sort_config_.isValid());
  }

  ~SortMergeRunWorkOrder() {}

  void execute() override;

 private:
  const SortConfiguration &sort_config_;
  const CatalogRelationSchema &run_relation_;
  std::vector<merge_run_operator::Run> input_runs_;
  const std::size_t top_k_;
  const std::size_t merge_level_;

  InsertDestination *output_destination_;
  StorageManager *storage_manager_;

  const std::size_t operator_index_;
  const tmb::client_id scheduler_client_id_;
  MessageBus *bus_;

  friend class SortMergeRunOperator;

  DISALLOW_COPY_AND_ASSIGN(SortMergeRunWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_SORT_MERGE_RUN_OPERATOR_HPP_
