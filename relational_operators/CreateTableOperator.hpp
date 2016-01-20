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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_CREATE_TABLE_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_CREATE_TABLE_OPERATOR_HPP_

#include <memory>

#include "catalog/CatalogRelation.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogDatabase;
class QueryContext;
class StorageManager;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which creates a relation.
 **/
class CreateTableOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor.
   *
   * @param relation The relation to add. This CreateTableOperator owns
   *        relation until the WorkOrder it produces is successfully executed,
   *        at which point it is owned by database.
   **/
  explicit CreateTableOperator(CatalogRelation *relation)
      : relation_(relation),
        work_generated_(false) {}

  ~CreateTableOperator() override {}

  bool getAllWorkOrders(WorkOrdersContainer *container) override;

 private:
  std::unique_ptr<CatalogRelation> relation_;
  bool work_generated_;

  DISALLOW_COPY_AND_ASSIGN(CreateTableOperator);
};

/**
 * @brief A WorkOrder produced by CreateTableOperator.
 **/
class CreateTableWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param relation The relation to add. This class does NOT take ownership
   *        of the passed-in pointer, and it must remain valid until after
   *        execute() is called, at which point it is owned by catalog_database
   *        in execute().
   **/
  explicit CreateTableWorkOrder(CatalogRelation *relation)
      : relation_(relation) {}

  ~CreateTableWorkOrder() override {}

  /**
   * @exception RelationNameCollision A relation with the same name is already
   *            present in the database.
   **/
  void execute(QueryContext *query_context,
               CatalogDatabase *catalog_database,
               StorageManager *storage_manager) override;

 private:
  std::unique_ptr<CatalogRelation> relation_;

  DISALLOW_COPY_AND_ASSIGN(CreateTableWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_CREATE_TABLE_OPERATOR_HPP_
