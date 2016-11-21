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

#ifndef QUICKSTEP_VIZ_VIZ_ANALYZER_HPP_
#define QUICKSTEP_VIZ_VIZ_ANALYZER_HPP_

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "utility/Macros.hpp"
#include "viz/VizObject.hpp"

#include "glog/logging.h"

namespace quickstep {

class CatalogAttribute;
class CatalogDatabase;
class CatalogRelation;
class QueryPlan;
class StorageManager;

namespace viz {

/** \addtogroup Viz
 *  @{
 */

class VizAnalyzer : public VizObject {
 public:
  VizAnalyzer(const optimizer::physical::PhysicalPtr &physical_plan,
              const QueryPlan *execution_plan,
              const std::vector<WorkOrderTimeEntry> *profiling_stats,
              const CatalogRelation *query_result_relation,
              const CatalogDatabase *catalog_database,
              StorageManager *storage_manager);

  std::string getName() const override {
    return "VizAnalyzer";
  }

  inline const optimizer::physical::PhysicalPtr& getQueryPlan() const {
    return physical_plan_;
  }

  const QueryPlan* getExecutionPlan() const {
    return execution_plan_;
  }

  const std::vector<WorkOrderTimeEntry>* getProfilingStats() const {
    return profiling_stats_;
  }

  const CatalogRelation *getRelation() const {
    return query_result_relation_;
  }

  const CatalogDatabase *getDatabase() const {
    return catalog_database_;
  }

  StorageManager *getStorageManager() const  {
    return storage_manager_;
  }

  bool findGroupByAttributes(
      std::vector<attribute_id> *group_by_attribute_ids) const;

  bool isTime(const attribute_id attr_id,
              std::string *format) const;

 private:
  bool findGroupByAttributesInternal(
      const optimizer::physical::PhysicalPtr &physical_plan,
      std::vector<optimizer::expressions::AttributeReferencePtr> *group_by_attributes) const;

  bool findGenExpression(const optimizer::physical::PhysicalPtr &physical_plan,
                         const optimizer::expressions::ExprId expr_id,
                         optimizer::expressions::ExpressionPtr *expr) const;

  bool isExtractedTime(const optimizer::physical::PhysicalPtr &physical_plan,
                       const optimizer::expressions::ExprId expr_id,
                       std::string *format) const;

  const optimizer::physical::PhysicalPtr physical_plan_;
  const QueryPlan *execution_plan_;
  const std::vector<WorkOrderTimeEntry> *profiling_stats_;
  const CatalogRelation *query_result_relation_;
  const CatalogDatabase *catalog_database_;
  StorageManager *storage_manager_;

  std::unordered_map<
      attribute_id, optimizer::expressions::AttributeReferencePtr>
          catalog_to_physical_attribute_map;

  std::unordered_map<
      optimizer::expressions::ExprId, const CatalogAttribute *>
          physical_to_catalog_attribute_map;

  DISALLOW_COPY_AND_ASSIGN(VizAnalyzer);
};

/** @} */

}  // namespace viz
}  // namespace quickstep

#endif  // QUICKSTEP_VIZ_VIZ_ANALYZER_HPP_
