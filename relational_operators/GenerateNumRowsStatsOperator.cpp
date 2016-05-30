/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include "relational_operators/GenerateNumRowsStatsOperator.hpp"

#include <memory>

#include "catalog/CatalogRelation.hpp"
#include "cli/PrintToScreen.hpp"

#include "tmb/id_typedefs.h"

namespace quickstep {

bool GenerateNumRowsStatsOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  std::size_t num_tuples =
      PrintToScreen::GetNumTuplesInRelation(*relation_, storage_manager);
  relation_->getStatisticsMutable()->setNumTuples(num_tuples);
  return true;
}

}  // namespace quickstep

