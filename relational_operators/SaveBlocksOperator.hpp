/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_SAVE_BLOCKS_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_SAVE_BLOCKS_OPERATOR_HPP_

#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogDatabase;
class QueryContext;
class StorageManager;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which saves blocks to disk.
 **/
class SaveBlocksOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor for saving only modified blocks in a relation.
   *
   * @param force If true, force writing of all blocks to disk, otherwise only
   *        write dirty blocks.
   **/
  explicit SaveBlocksOperator(bool force = false)
      : force_(force),
        num_workorders_generated_(0) {}

  ~SaveBlocksOperator() override {}

  bool getAllWorkOrders(WorkOrdersContainer *container) override;

  void feedInputBlock(const block_id input_block_id, const relation_id input_relation_id) override;

  void feedInputBlocks(const relation_id rel_id, std::vector<block_id> *partially_filled_blocks) override {
    for (std::vector<block_id>::const_iterator it = partially_filled_blocks->begin();
         it != partially_filled_blocks->end();
         ++it) {
      feedInputBlock(*it, rel_id);
    }
  }

 private:
  const bool force_;

  std::vector<block_id> destination_block_ids_;

  std::vector<block_id>::size_type num_workorders_generated_;

  DISALLOW_COPY_AND_ASSIGN(SaveBlocksOperator);
};

/**
 * @brief A WorkOrder produced by SaveBlocksOperator.
 **/
class SaveBlocksWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param save_block_id The id of the block to save.
   * @param force If true, force writing of all blocks to disk, otherwise only
   *        write dirty blocks.
   **/
  SaveBlocksWorkOrder(const block_id save_block_id, const bool force)
      : save_block_id_(save_block_id), force_(force) {}

  ~SaveBlocksWorkOrder() override {}

  /**
   * @exception BlockNotFoundInMemory The block could not be found in the
   *            memory managed by the StorageManager.
   * @exception UnableToOpenFile The block's on-disk storage file couldn't be
   *            opened for writing.
   * @exception FileWriteError An IO error occurred while writing the block's
   *            on-disk storage file.
   **/
  void execute(QueryContext *query_context,
               CatalogDatabase *catalog_database,
               StorageManager *storage_manager) override;

 private:
  const block_id save_block_id_;
  const bool force_;

  DISALLOW_COPY_AND_ASSIGN(SaveBlocksWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_SAVE_BLOCKS_OPERATOR_HPP_
