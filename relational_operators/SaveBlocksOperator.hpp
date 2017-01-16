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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_SAVE_BLOCKS_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_SAVE_BLOCKS_OPERATOR_HPP_

#include <cstddef>
#include <string>
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

class CatalogRelation;
class QueryContext;
class StorageManager;
class WorkOrderProtosContainer;
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
   * @param query_id The ID of the query to which this operator belongs.
   * @param relation The relation whose blocks will be saved.
   * @param force If true, force writing of all blocks to disk, otherwise only
   *        write dirty blocks.
   **/
  SaveBlocksOperator(const std::size_t query_id,
                     CatalogRelation *relation,
                     const bool force = false)
      : RelationalOperator(query_id),
        force_(force),
        relation_(relation),
        num_workorders_generated_(0) {}

  ~SaveBlocksOperator() override {}

  std::string getName() const override {
    return "SaveBlocksOperator";
  }

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

  bool getAllWorkOrderProtos(WorkOrderProtosContainer *container) override;

  void feedInputBlock(const block_id input_block_id, const relation_id input_relation_id,
                      const partition_id part_id) override {
    destination_block_ids_.push_back(input_block_id);
  }

  void updateCatalogOnCompletion() override;

 private:
  const bool force_;

  CatalogRelation *relation_;
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
   * @param query_id The ID of the query to which this operator belongs.
   * @param save_block_id The id of the block to save.
   * @param force If true, force writing of all blocks to disk, otherwise only
   *        write dirty blocks.
   * @param storage_manager The StorageManager to use.
   **/
  SaveBlocksWorkOrder(const std::size_t query_id,
                      const block_id save_block_id,
                      const bool force,
                      StorageManager *storage_manager)
      : WorkOrder(query_id),
        save_block_id_(save_block_id),
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
