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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_SAVE_BLOCKS_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_SAVE_BLOCKS_OPERATOR_HPP_

#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

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

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id foreman_client_id,
                        tmb::MessageBus *bus) override;

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
   * @param storage_manager The StorageManager to use.
   **/
  SaveBlocksWorkOrder(const block_id save_block_id,
                      const bool force,
                      StorageManager *storage_manager)
      : save_block_id_(save_block_id),
        force_(force),
        storage_manager_(DCHECK_NOTNULL(storage_manager)) {}

  ~SaveBlocksWorkOrder() override {}

  /**
   * @exception BlockNotFoundInMemory The block could not be found in the
   *            memory managed by the StorageManager.
   * @exception UnableToOpenFile The block's on-disk storage file couldn't be
   *            opened for writing.
   * @exception FileWriteError An IO error occurred while writing the block's
   *            on-disk storage file.
   **/
  void execute() override;

 private:
  const block_id save_block_id_;
  const bool force_;

  StorageManager *storage_manager_;

  DISALLOW_COPY_AND_ASSIGN(SaveBlocksWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_SAVE_BLOCKS_OPERATOR_HPP_
