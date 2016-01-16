/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_DROP_TABLE_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_DROP_TABLE_OPERATOR_HPP_

#include "catalog/CatalogTypedefs.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogDatabase;
class CatalogRelation;
class QueryContext;
class StorageManager;
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
   * @param rel_id The id of the relation to drop.
   * @param only_drop_blocks If true, only drop the blocks belonging to
   *        relation, but leave relation in the database.
   **/
  explicit DropTableOperator(const relation_id rel_id,
                             const bool only_drop_blocks = false)
      : rel_id_(rel_id),
        only_drop_blocks_(only_drop_blocks),
        work_generated_(false) {}

  ~DropTableOperator() override {}

  bool getAllWorkOrders(WorkOrdersContainer *container) override;

 private:
  const relation_id rel_id_;
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
   * @param rel_id The id of the relation to drop.
   * @param only_drop_blocks If true, only drop the blocks belonging to
   *        relation, but leave relation in the database.
   **/
  explicit DropTableWorkOrder(const relation_id rel_id,
                              const bool only_drop_blocks)
      : rel_id_(rel_id), only_drop_blocks_(only_drop_blocks) {}

  ~DropTableWorkOrder() override {}

  void execute(QueryContext *query_context,
               CatalogDatabase *catalog_database,
               StorageManager *storage_manager) override;

 private:
  const relation_id rel_id_;
  const bool only_drop_blocks_;

  DISALLOW_COPY_AND_ASSIGN(DropTableWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_DROP_TABLE_OPERATOR_HPP_
