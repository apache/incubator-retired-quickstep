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

#ifndef QUICKSTEP_VIZ_VIZ_HELPER_HPP_
#define QUICKSTEP_VIZ_VIZ_HELPER_HPP_

#include <vector>

#include "query_execution/QueryExecutionTypedefs.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogDatabase;
class CatalogRelation;
class ParseStatement;
class QueryPlan;
class StorageManager;

namespace viz {

/** \addtogroup Viz
 *  @{
 */

class VizHelper {
 public:
  static void Visualize(const ParseStatement &parse_statement,
                        const QueryPlan *execution_plan,
                        const std::vector<WorkOrderTimeEntry> *profiling_stats,
                        const CatalogRelation *query_result_relation,
                        const CatalogDatabase *catalog_database,
                        StorageManager *storage_manager);

 private:
  VizHelper() {}

  DISALLOW_COPY_AND_ASSIGN(VizHelper);
};

/** @} */

}  // namespace viz
}  // namespace quickstep

#endif  // QUICKSTEP_VIZ_VIZ_HELPER_HPP_
