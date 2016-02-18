/**
 *   Copyright 2016 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_TABLE_GENERATOR_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_TABLE_GENERATOR_OPERATOR_HPP_

#include <cstddef>
#include <memory>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogDatabase;
class StorageManager;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief TODO
 **/
class TableGeneratorOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor
   *
   **/
  TableGeneratorOperator(const CatalogRelation &output_relation,
                         const QueryContext::insert_destination_id output_destination_index,
                         const QueryContext::generator_function_id generator_function_index)
      : output_relation_(output_relation),
        output_destination_index_(output_destination_index),
        generator_function_index_(generator_function_index),
        started_(false) {
  }

  ~TableGeneratorOperator() override {}

  bool getAllWorkOrders(WorkOrdersContainer *container) override;

  void feedInputBlock(const block_id input_block_id, const relation_id input_relation_id) override {
  }

  void feedInputBlocks(const relation_id rel_id, std::vector<block_id> *partially_filled_blocks) override {
  }

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
   **/
  TableGeneratorWorkOrder(const QueryContext::insert_destination_id output_destination_index,
                          const QueryContext::generator_function_id generator_function_index)
      : output_destination_index_(output_destination_index),
        generator_function_index_(generator_function_index) {
  }

  ~TableGeneratorWorkOrder() override {}

  void execute(QueryContext *query_context,
               CatalogDatabase *catalog_database,
               StorageManager *storage_manager) override;

 private:
  const QueryContext::insert_destination_id output_destination_index_;
  const QueryContext::generator_function_id generator_function_index_;

  DISALLOW_COPY_AND_ASSIGN(TableGeneratorWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_TABLE_GENERATOR_OPERATOR_HPP_
