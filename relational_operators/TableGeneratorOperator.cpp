/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *   University of Wisconsin—Madison.
 *   Copyright 2016 Pivotal Software, Inc.
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

#include "relational_operators/TableGeneratorOperator.hpp"

#include "expressions/table_generator/GeneratorFunctionHandle.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "storage/InsertDestination.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

bool TableGeneratorOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  if (!started_) {
    DCHECK(query_context != nullptr);

    // Currently the generator function is not abstracted to be parallelizable,
    // so just produce one work order.
    container->addNormalWorkOrder(
        new TableGeneratorWorkOrder(
            query_context->getGeneratorFunctionHandle(
                generator_function_index_),
            query_id_,
            query_context->getInsertDestination(output_destination_index_)),
        op_index_);
    started_ = true;
  }
  return started_;
}

void TableGeneratorWorkOrder::execute() {
  ColumnVectorsValueAccessor temp_result;
  function_handle_.populateColumns(&temp_result);

  output_destination_->bulkInsertTuples(&temp_result);
}

}  // namespace quickstep
