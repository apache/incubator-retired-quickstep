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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_GENERATE_NUM_ROWS_STATS_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_GENERATE_NUM_ROWS_STATS_OPERATOR_HPP_

#include <memory>

#include "catalog/CatalogRelation.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class CatalogRelation;
class QueryContext;
class StorageManager;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator that gets the number of rows after loading a relation.
 **/
class GenerateNumRowsStatsOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor.
   *
   * @param relation The relation to get the number of rows from.
   *                 This GenNumRowStatsOperator owns relation until
   *                 the WorkOrder it produces is successfully executed.
   **/
  explicit GenerateNumRowsStatsOperator(CatalogRelation *relation)
      : relation_(relation) {}
  ~GenerateNumRowsStatsOperator() override {}

  /**
   * @note no WorkOrder is generated for this operator.
   **/
  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

 private:
  CatalogRelation *relation_;

  DISALLOW_COPY_AND_ASSIGN(GenerateNumRowsStatsOperator);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_GENERATE_NUM_ROWS_STATS_OPERATOR_HPP_
