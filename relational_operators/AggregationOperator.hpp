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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_AGGREGATION_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_AGGREGATION_OPERATOR_HPP_

#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class AggregationOperationState;
class StorageManager;
class WorkOrderProtosContainer;
class WorkOrdersContainer;

namespace serialization { class WorkOrder; }

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which performs aggregation over a relation.
 **/
class AggregationOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor for aggregating with arbitrary expressions in projection
   *        list.
   *
   * @param input_relation The relation to perform aggregation over.
   * @param input_relation_is_stored If input_relation is a stored relation and
   *        is fully available to the operator before it can start generating
   *        workorders.
   * @param aggr_state_index The index of the AggregationState in QueryContext.
   **/
  AggregationOperator(const CatalogRelation &input_relation,
                      bool input_relation_is_stored,
                      const QueryContext::aggregation_state_id aggr_state_index)
      : input_relation_is_stored_(input_relation_is_stored),
        input_relation_block_ids_(input_relation_is_stored ? input_relation.getBlocksSnapshot()
                                                           : std::vector<block_id>()),
        aggr_state_index_(aggr_state_index),
        num_workorders_generated_(0),
        started_(false) {}

  ~AggregationOperator() override {}

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id foreman_client_id,
                        const tmb::client_id agent_client_id,
                        tmb::MessageBus *bus) override;

  bool getAllWorkOrderProtos(WorkOrderProtosContainer *container) override;

  void feedInputBlock(const block_id input_block_id, const relation_id input_relation_id) override {
    input_relation_block_ids_.push_back(input_block_id);
  }

  void feedInputBlocks(const relation_id rel_id, std::vector<block_id> *partially_filled_blocks) override {
    input_relation_block_ids_.insert(input_relation_block_ids_.end(),
                                     partially_filled_blocks->begin(),
                                     partially_filled_blocks->end());
  }

 private:
  /**
   * @brief Create Work Order proto.
   *
   * @param block The block id used in the Work Order.
   **/
  serialization::WorkOrder* createWorkOrderProto(const block_id block);

  const bool input_relation_is_stored_;
  std::vector<block_id> input_relation_block_ids_;
  const QueryContext::aggregation_state_id aggr_state_index_;

  std::vector<block_id>::size_type num_workorders_generated_;
  bool started_;

  DISALLOW_COPY_AND_ASSIGN(AggregationOperator);
};

/**
 * @brief A WorkOrder produced by AggregationOperator.
 **/
class AggregationWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor
   *
   * @param input_block_id The block id.
   * @param state The AggregationState to use.
   **/
  AggregationWorkOrder(const block_id input_block_id,
                       AggregationOperationState *state)
      : input_block_id_(input_block_id),
        state_(DCHECK_NOTNULL(state)) {}

  ~AggregationWorkOrder() override {}

  void execute() override;

 private:
  const block_id input_block_id_;
  AggregationOperationState *state_;

  DISALLOW_COPY_AND_ASSIGN(AggregationWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_AGGREGATION_OPERATOR_HPP_
