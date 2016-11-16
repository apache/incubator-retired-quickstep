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

#include "viz/VizHelper.hpp"

#include <memory>
#include <vector>

#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_optimizer/LogicalGenerator.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/PhysicalGenerator.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "viz/VizAnalyzer.hpp"
#include "viz/VizContext.hpp"
#include "viz/VizEngine.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace viz {

namespace O = ::quickstep::optimizer;
namespace P = ::quickstep::optimizer::physical;

void VizHelper::Visualize(const ParseStatement &parse_statement,
                          const QueryPlan *execution_plan,
                          const std::vector<WorkOrderTimeEntry> *profiling_stats,
                          const CatalogRelation *query_result_relation,
                          const CatalogDatabase *catalog_database,
                          StorageManager *storage_manager) {
  // TODO(jianqiao): give some output for empty relation.
  if (query_result_relation == nullptr) {
    return;
  }

  O::OptimizerContext optimizer_context;
  O::LogicalGenerator logical_generator(&optimizer_context);
  O::PhysicalGenerator physical_generator;

  const P::PhysicalPtr physical_plan =
      physical_generator.generatePlan(
          logical_generator.generatePlan(*catalog_database, parse_statement));

  std::unique_ptr<VizContext> context_base(new VizContext());
  context_base->set("VizAnalyzer",
                    new VizAnalyzer(physical_plan,
                                    execution_plan,
                                    profiling_stats,
                                    query_result_relation,
                                    catalog_database,
                                    storage_manager));

  VizEngine viz_engine(VizContextPtr(context_base.release()));
  viz_engine.execute();
}


}  // namespace viz
}  // namespace quickstep
