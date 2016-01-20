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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_INSERT_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_INSERT_OPERATOR_HPP_

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogDatabase;
class StorageManager;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which inserts a single tuple into a relation.
 **/
class InsertOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor.
   *
   * @param output_relation_id The id of the output relation.
   * @param output_destination_index The index of the InsertDestination in the
   *        QueryContext to insert the tuple.
   * @param tuple_index The index of the tuple to insert in the QueryContext.
   **/
  InsertOperator(const relation_id output_relation_id,
                 const QueryContext::insert_destination_id output_destination_index,
                 const QueryContext::tuple_id tuple_index)
      : output_relation_id_(output_relation_id),
        output_destination_index_(output_destination_index),
        tuple_index_(tuple_index),
        work_generated_(false) {}

  ~InsertOperator() override {}

  bool getAllWorkOrders(WorkOrdersContainer *container) override;

  QueryContext::insert_destination_id getInsertDestinationID() const override {
    return output_destination_index_;
  }

  const relation_id getOutputRelationID() const override {
    return output_relation_id_;
  }

 private:
  const relation_id output_relation_id_;
  const QueryContext::insert_destination_id output_destination_index_;
  const QueryContext::tuple_id tuple_index_;
  bool work_generated_;

  DISALLOW_COPY_AND_ASSIGN(InsertOperator);
};

/**
 * @brief A WorkOrder produced by InsertOperator.
 **/
class InsertWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param output_destination_index The index of the InsertDestination in the
   *        QueryContext to insert the tuple.
   * @param tuple_index The index of the tuple to insert in the QueryContext.
   **/
  InsertWorkOrder(
      const QueryContext::insert_destination_id output_destination_index,
      const QueryContext::tuple_id tuple_index)
      : output_destination_index_(output_destination_index),
        tuple_index_(tuple_index) {}

  ~InsertWorkOrder() override {}

  /**
   * @exception TupleTooLargeForBlock The tuple was too large to insert into an
   *            empty block provided by output_destination_index_ in query_context.
   **/
  void execute(QueryContext *query_context,
               CatalogDatabase *catalog_database,
               StorageManager *storage_manager) override;

 private:
  const QueryContext::insert_destination_id output_destination_index_;
  const QueryContext::tuple_id tuple_index_;

  DISALLOW_COPY_AND_ASSIGN(InsertWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_INSERT_OPERATOR_HPP_
