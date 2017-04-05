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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_TABLE_GENERATOR_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_TABLE_GENERATOR_OPERATOR_HPP_

#include <string>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class GeneratorFunctionHandle;
class InsertDestination;
class StorageManager;
class WorkOrderProtosContainer;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which uses the given generator function to populate a
 *        table.
 **/
class TableGeneratorOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor
   *
   * @param query_id The ID of the query to which this operator belongs.
   * @param output_relation The output relation.
   * @param output_destination_index The index of the InsertDestination in the
   *        QueryContext to insert the generated output.
   * @param generator_function_index The index of the GeneratorFunctionHandle in
   *        the QueryContext.
   **/
  TableGeneratorOperator(
      const std::size_t query_id,
      const CatalogRelation &output_relation,
      const QueryContext::insert_destination_id output_destination_index,
      const QueryContext::generator_function_id generator_function_index)
      : RelationalOperator(query_id),
        output_relation_(output_relation),
        output_destination_index_(output_destination_index),
        generator_function_index_(generator_function_index),
        started_(false) {}

  ~TableGeneratorOperator() override {}

  OperatorType getOperatorType() const override {
    return kTableGenerator;
  }

  std::string getName() const override {
    return "TableGeneratorOperator";
  }

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

  bool getAllWorkOrderProtos(WorkOrderProtosContainer *container) override;

  QueryContext::insert_destination_id getInsertDestinationID() const override {
    return output_destination_index_;
  }

  const relation_id getOutputRelationID() const override {
    return output_relation_.getID();
  }

 private:
  const CatalogRelation &output_relation_;
  const QueryContext::insert_destination_id output_destination_index_;
  const QueryContext::generator_function_id generator_function_index_;

  bool started_;

  DISALLOW_COPY_AND_ASSIGN(TableGeneratorOperator);
};

/**
 * @brief A WorkOrder produced by TableGeneratorOperator.
 **/
class TableGeneratorWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param query_id The ID of the query to which this WorkOrder belongs.
   * @param generator_function The GeneratorFunctionHandle to use.
   * @param output_destination The InsertDestination to insert the generated
   *        output.
   **/
  TableGeneratorWorkOrder(const std::size_t query_id,
                          const GeneratorFunctionHandle &function_handle,
                          InsertDestination *output_destination)
      : WorkOrder(query_id),
        function_handle_(function_handle),
        output_destination_(DCHECK_NOTNULL(output_destination)) {}

  ~TableGeneratorWorkOrder() override {}

  void execute() override;

 private:
  const GeneratorFunctionHandle &function_handle_;
  InsertDestination *output_destination_;

  DISALLOW_COPY_AND_ASSIGN(TableGeneratorWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_TABLE_GENERATOR_OPERATOR_HPP_
