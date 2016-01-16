/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "relational_operators/DestroyHashOperator.hpp"

#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrdersContainer.hpp"

#include "glog/logging.h"

namespace quickstep {

bool DestroyHashOperator::getAllWorkOrders(WorkOrdersContainer *container) {
  if (blocking_dependencies_met_ && !work_generated_) {
    work_generated_ = true;
    container->addNormalWorkOrder(new DestroyHashWorkOrder(hash_table_index_),
                                  op_index_);
  }
  return work_generated_;
}

void DestroyHashWorkOrder::execute(QueryContext *query_context,
                                   CatalogDatabase *catalog_database,
                                   StorageManager *storage_manager) {
  DCHECK(query_context != nullptr);
  query_context->destroyJoinHashTable(hash_table_index_);
}

}  // namespace quickstep
