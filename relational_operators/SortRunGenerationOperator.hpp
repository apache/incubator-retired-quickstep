/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_SORT_RUN_GENERATION_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_SORT_RUN_GENERATION_OPERATOR_HPP_

#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class CatalogDatabase;
class StorageManager;
class WorkOrdersContainer;

/**
 * \defgroup Sort Sorting
 * \ingroup RelationalOperators
 * Sorting Quickstep is implemented as a two-stage (or two operator)
 * pipeline. The first stage is \ref SortRunGeneration, where we create a list
 * of runs. Each run contains tuples in the sorted order. In the second stage
 * (\ref SortMergeRun), we merge the list of runs in the a single list of run.
 */

/**
 * \defgroup SortRunGeneration Sorted Run Generation
 * \ingroup Sort
 * We implement a simple version of sorted run generation, where we sort tuples
 * in each input block, and write each sorted block (as a run) to be consumed by
 * the next stage (\ref SortMergeRun).
 */

/**
 * \addtogroup SortRunGeneration
 * @{
 */

/**
 * @brief An operator which sorts blocks of a relation.
 **/
class SortRunGenerationOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor for sorting tuples in blocks based on the sort
   * configuration and writing to output destination.
   *
   * @param input_relation The relation to generate sorted runs of.
   * @param output_relation_id The id of the output relation.
   * @param output_destination_index The index of the InsertDestination in the
   *        QueryContext to store the sorted blocks of runs.
   * @param sort_config Sort configuration specifying ORDER BY, ordering and
   *                    null ordering. The operator makes a copy of the
   *                    configuration.
   * @param input_relation_is_stored Does the input relation contain the blocks
   *                                 to sort. If \c false, the blocks are
   *                                 streamed.
   **/
  SortRunGenerationOperator(const CatalogRelation &input_relation,
                            const relation_id output_relation_id,
                            const QueryContext::insert_destination_id output_destination_index,
                            const QueryContext::sort_config_id sort_config_index,
                            bool input_relation_is_stored)
      : input_relation_(input_relation),
        output_relation_id_(output_relation_id),
        output_destination_index_(output_destination_index),
        sort_config_index_(sort_config_index),
        input_relation_block_ids_(input_relation_is_stored ? input_relation.getBlocksSnapshot()
                                                           : std::vector<block_id>()),
        num_workorders_generated_(0),
        started_(false),
        input_relation_is_stored_(input_relation_is_stored) {}

  ~SortRunGenerationOperator() {}

  bool getAllWorkOrders(WorkOrdersContainer *container) override;

  void feedInputBlock(const block_id input_block_id, const relation_id input_relation_id) override {
    DCHECK(input_relation_id == input_relation_.getID());
    input_relation_block_ids_.push_back(input_block_id);
  }

  void feedInputBlocks(const relation_id rel_id, std::vector<block_id> *partially_filled_blocks) override {
    DCHECK(rel_id == input_relation_.getID());
    input_relation_block_ids_.insert(input_relation_block_ids_.end(),
                                     partially_filled_blocks->begin(),
                                     partially_filled_blocks->end());
  }

  QueryContext::insert_destination_id getInsertDestinationID() const override {
    return output_destination_index_;
  }

  const relation_id getOutputRelationID() const override {
    return output_relation_id_;
  }

 private:
  const CatalogRelation &input_relation_;

  const relation_id output_relation_id_;
  const QueryContext::insert_destination_id output_destination_index_;

  const QueryContext::sort_config_id sort_config_index_;
  std::vector<block_id> input_relation_block_ids_;
  std::vector<block_id>::size_type num_workorders_generated_;

  bool started_;
  const bool input_relation_is_stored_;

  DISALLOW_COPY_AND_ASSIGN(SortRunGenerationOperator);
};

class SortRunGenerationWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param input_relation_id The id of the relation to generate sorted runs of.
   * @param output_destination_index The index of the InsertDestination in the
   *        QueryContext to store the sorted blocks of runs.
   * @param sort_config_index The index of the Sort configuration in
   *        QueryContext specifying ORDER BY, ordering and null ordering.
   * @param input_block_id The block id.
   **/
  SortRunGenerationWorkOrder(const relation_id input_relation_id,
                             const QueryContext::insert_destination_id output_destination_index,
                             const QueryContext::sort_config_id sort_config_index,
                             const block_id input_block_id)
      : input_relation_id_(input_relation_id),
        output_destination_index_(output_destination_index),
        sort_config_index_(sort_config_index),
        input_block_id_(input_block_id) {}

  ~SortRunGenerationWorkOrder() {}

  void execute(QueryContext *query_context,
               CatalogDatabase *catalog_database,
               StorageManager *storage_manager) override;

 private:
  const relation_id input_relation_id_;
  const QueryContext::insert_destination_id output_destination_index_;
  const QueryContext::sort_config_id sort_config_index_;

  const block_id input_block_id_;

  friend class SortRunGenerationOperator;

  DISALLOW_COPY_AND_ASSIGN(SortRunGenerationWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_SORT_RUN_GENERATION_OPERATOR_HPP_
