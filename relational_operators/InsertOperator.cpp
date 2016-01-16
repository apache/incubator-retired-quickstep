/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "relational_operators/InsertOperator.hpp"

#include <memory>

#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "storage/InsertDestination.hpp"
#include "types/containers/Tuple.hpp"

#include "glog/logging.h"

namespace quickstep {

bool InsertOperator::getAllWorkOrders(WorkOrdersContainer *container) {
  if (blocking_dependencies_met_ && !work_generated_) {
    work_generated_ = true;
    container->addNormalWorkOrder(
        new InsertWorkOrder(output_destination_index_, tuple_index_),
        op_index_);
  }
  return work_generated_;
}

void InsertWorkOrder::execute(QueryContext *query_context,
                              CatalogDatabase *catalog_database,
                              StorageManager *storage_manager) {
  DCHECK(query_context != nullptr);

  InsertDestination *output_destination =
      query_context->getInsertDestination(output_destination_index_);
  DCHECK(output_destination != nullptr);

  std::unique_ptr<Tuple> tuple(query_context->releaseTuple(tuple_index_));

  output_destination->insertTuple(*tuple);
}

}  // namespace quickstep
