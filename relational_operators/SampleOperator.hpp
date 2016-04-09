/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_SAMPLE_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_SAMPLE_OPERATOR_HPP_

#include <memory>
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

class CatalogDatabase;
class CatalogRelationSchema;
class InsertDestination;
class StorageManager;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which performs sampling at the block or tuple level over a relation.
 **/
class SampleOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor for SampleOperator  with the sampling percentage and type of sampling.
   *
   * @param input_relation The relation to perform sampling over.
   * @param output_relation The output relation.
   * @param output_destination_index The index of the InsertDestination in the
   *        QueryContext to insert the sampling results.
   * @param input_relation_is_stored If input_relation is a stored relation and
   *        is fully available to the operator before it can start generating
   *        workorders.
   * @param is_block_sample Flag indicating whether the sample type is block or tuple.
   * @param percentage The percentage of data to be sampled.
   *
   **/
  SampleOperator(const CatalogRelation &input_relation,
                 const CatalogRelationSchema &output_relation,
                 const QueryContext::insert_destination_id output_destination_index,
                 const bool input_relation_is_stored,
                 const bool is_block_sample,
                 const int percentage)
      : input_relation_(input_relation),
        output_relation_(output_relation),
        output_destination_index_(output_destination_index),
        input_relation_is_stored_(input_relation_is_stored),
        is_block_sample_(is_block_sample),
        percentage_(percentage),
        input_relation_block_ids_(input_relation_is_stored ? input_relation.getBlocksSnapshot()
                                                           : std::vector<block_id>()),
        num_workorders_generated_(0),
        started_(false) {}

  ~SampleOperator() override {}

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

  void feedInputBlock(const block_id input_block_id, const relation_id input_relation_id) override {
    input_relation_block_ids_.push_back(input_block_id);
  }

  void feedInputBlocks(const relation_id rel_id, std::vector<block_id> *partially_filled_blocks) override {
    input_relation_block_ids_.insert(input_relation_block_ids_.end(),
                                     partially_filled_blocks->begin(),
                                     partially_filled_blocks->end());
  }

  QueryContext::insert_destination_id getInsertDestinationID() const override {
    return output_destination_index_;
  }

  const relation_id getOutputRelationID() const override {
    return output_relation_.getID();
  }

 private:
  const CatalogRelation &input_relation_;
  const CatalogRelationSchema &output_relation_;
  const QueryContext::insert_destination_id output_destination_index_;
  const bool input_relation_is_stored_;
  const bool is_block_sample_;
  const int percentage_;
  std::vector<block_id> input_relation_block_ids_;

  // A single workorder is generated for each block of input relation.
  std::vector<block_id>::size_type num_workorders_generated_;
  bool started_;

  DISALLOW_COPY_AND_ASSIGN(SampleOperator);
};

/**
 * @brief A WorkOrder produced by SampleOperator.
 **/
class SampleWorkOrder : public WorkOrder {
 public:
  SampleWorkOrder(const CatalogRelationSchema &input_relation,
                  const block_id input_block_id,
                  const bool is_block_sample,
                  const int percentage,
                  InsertDestination *output_destination,
                  StorageManager *storage_manager)
      : input_relation_(input_relation),
        input_block_id_(input_block_id),
        is_block_sample_(is_block_sample),
        percentage_(percentage),
        output_destination_(DCHECK_NOTNULL(output_destination)),
        storage_manager_(DCHECK_NOTNULL(storage_manager)) {}

  ~SampleWorkOrder() override {}

  /**
   * @exception TupleTooLargeForBlock A tuple produced by this selection was
   *            too large to insert into an empty block provided by
   *            _op->_outputDestination. Select may be partially complete
   *            (with some tuples inserted into the destination) when this
   *            exception is thrown, causing potential inconsistency.
   **/
  void execute() override;

 private:
  const CatalogRelationSchema &input_relation_;
  const block_id input_block_id_;

  const bool is_block_sample_;
  const int percentage_;

  InsertDestination *output_destination_;
  StorageManager *storage_manager_;

  DISALLOW_COPY_AND_ASSIGN(SampleWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_SAMPLE_OPERATOR_HPP_
