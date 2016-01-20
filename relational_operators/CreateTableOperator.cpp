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
