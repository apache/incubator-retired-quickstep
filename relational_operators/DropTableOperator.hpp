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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_DROP_TABLE_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_DROP_TABLE_OPERATOR_HPP_

#include <cstddef>
#include <string>
#include <utility>
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

class CatalogDatabase;
class CatalogDatabaseLite;
class CatalogRelation;
class QueryContext;
class StorageManager;
class WorkOrderProtosContainer;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which drops a relation.
 **/
class DropTableOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor.
   *
   * @param query_id The ID of the query to which this operator belongs.
   * @param relation The relation to drop.
   * @param database The databse where to drop \c relation.
   * @param only_drop_blocks If true, only drop the blocks belonging to \c
   *        relation, but leave \c relation in \c database.
   **/
  DropTableOperator(const std::size_t query_id,
                    const CatalogRelation &relation,
                    CatalogDatabase *database,
                    const bool only_drop_blocks = false)
      : RelationalOperator(query_id),
        relation_(relation),
        database_(database),
        only_drop_blocks_(only_drop_blocks),
        work_generated_(false) {}

  ~DropTableOperator() override {}

  std::string getName() const override {
    return "DropTableOperator";
  }

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

  bool getAllWorkOrderProtos(WorkOrderProtosContainer *container) override;

  void actionOnCompletion() override;

 private:
  const CatalogRelation &relation_;
  CatalogDatabase *database_;
  const bool only_drop_blocks_;

  bool work_generated_;

  DISALLOW_COPY_AND_ASSIGN(DropTableOperator);
};

/**
 * @brief A WorkOrder produced by DropTableOperator.
 **/
class DropTableWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param query_id The ID of the query to which this operator belongs.
   * @param blocks The blocks to drop.
   * @param storage_manager The StorageManager to use.
   * @param rel_id The relation id to drop.
   * @param catalog_database_cache The CatalogDatabaseCache in the distributed
   *        version.
   **/
  DropTableWorkOrder(const std::size_t query_id,
                     std::vector<block_id> &&blocks,
                     StorageManager *storage_manager,
                     const relation_id rel_id = kInvalidCatalogId,
                     CatalogDatabaseLite *catalog_database_cache = nullptr)
      : WorkOrder(query_id),
        blocks_(std::move(blocks)),
        storage_manager_(DCHECK_NOTNULL(storage_manager)),
        rel_id_(rel_id),
        catalog_database_cache_(catalog_database_cache) {}

  ~DropTableWorkOrder() override {}

  void execute() override;

 private:
  const std::vector<block_id> blocks_;
  StorageManager *storage_manager_;

  // Used in the distributed version.
  const relation_id rel_id_;
  CatalogDatabaseLite *catalog_database_cache_;

  DISALLOW_COPY_AND_ASSIGN(DropTableWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_DROP_TABLE_OPERATOR_HPP_
