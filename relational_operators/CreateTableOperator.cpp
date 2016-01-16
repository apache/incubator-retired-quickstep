/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "relational_operators/CreateTableOperator.hpp"

#include <memory>

#include "catalog/CatalogDatabase.hpp"
#include "query_execution/WorkOrdersContainer.hpp"

#include "glog/logging.h"

namespace quickstep {

bool CreateTableOperator::getAllWorkOrders(WorkOrdersContainer *container) {
  if (!work_generated_) {
    work_generated_ = true;
    container->addNormalWorkOrder(
        new CreateTableWorkOrder(relation_.release()),
        op_index_);
  }
  return work_generated_;
}

void CreateTableWorkOrder::execute(QueryContext *query_context,
                                   CatalogDatabase *catalog_database,
                                   StorageManager *storage_manager) {
  DCHECK(catalog_database != nullptr);
  catalog_database->addRelation(relation_.release());
}

}  // namespace quickstep
