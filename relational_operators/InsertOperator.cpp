/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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
