/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_DESTROY_HASH_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_DESTROY_HASH_OPERATOR_HPP_

#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogDatabase;
class StorageManager;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which destroys a shared hash table.
 **/
class DestroyHashOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor.
   *
   * @param hash_table_index The index of the JoinHashTable in QueryContext.
   **/
  explicit DestroyHashOperator(const QueryContext::join_hash_table_id hash_table_index)
      : hash_table_index_(hash_table_index),
        work_generated_(false) {}

  ~DestroyHashOperator() override {}

  bool getAllWorkOrders(WorkOrdersContainer *container) override;

 private:
  const QueryContext::join_hash_table_id hash_table_index_;
  bool work_generated_;

  DISALLOW_COPY_AND_ASSIGN(DestroyHashOperator);
};

/**
 * @brief A WorkOrder produced by DestroyHashOperator.
 **/
class DestroyHashWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param hash_table_index The index of the JoinHashTable in QueryContext.
   **/
  explicit DestroyHashWorkOrder(const QueryContext::join_hash_table_id hash_table_index)
      : hash_table_index_(hash_table_index) {}

  ~DestroyHashWorkOrder() override {}

  void execute(QueryContext *query_context,
               CatalogDatabase *catalog_database,
               StorageManager *storage_manager) override;

 private:
  const QueryContext::join_hash_table_id hash_table_index_;

  DISALLOW_COPY_AND_ASSIGN(DestroyHashWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_DESTROY_HASH_OPERATOR_HPP_
