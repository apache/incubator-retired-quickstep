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

#include "relational_operators/MonitoredTextScanOperator.hpp"

#include "glog/logging.h"

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/TextScanOperator.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlob.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/Tuple.hpp"
#include "utility/Glob.hpp"

namespace quickstep {

bool MonitoredTextScanOperator::getAllWorkOrders(
 WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id foreman_client_id,
    tmb::MessageBus *bus) 
{
  VLOG(3) << "MonitoredTextScanOperator::getAllWorkOrders() called.";
  if (!done_monitoring_) {
    const std::vector<std::string> files = utility::file::GlobExpand(file_pattern_); 
    InsertDestination *output_destination =
      query_context->getInsertDestination(output_destination_index_);
    for (const auto &file : files) {
      // Check if the file was already added.
      if (added_files_.find(file) == added_files_.end()) {
        container->addNormalWorkOrder(
            new TextScanWorkOrder(file,
              field_terminator_,
              process_escape_sequences_,
             output_destination,
              storage_manager),
            op_index_);
        added_files_.emplace(file);
      }
    }
  }
  return done_monitoring_;
}

}  // namespace quickstep
