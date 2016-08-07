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

#include "relational_operators/CreateIndexOperator.hpp"

#include <utility>

#include "tmb/id_typedefs.h"

namespace quickstep {

bool CreateIndexOperator::getAllWorkOrders(WorkOrdersContainer *container,
                                           QueryContext *query_context,
                                           StorageManager *storage_manager,
                                           const tmb::client_id scheduler_client_id,
                                           tmb::MessageBus *bus) {
  return true;
}

void CreateIndexOperator::updateCatalogOnCompletion() {
  relation_->addIndex(index_name_, std::move(index_description_));
}

}  // namespace quickstep
