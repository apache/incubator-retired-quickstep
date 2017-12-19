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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_INSERT_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_INSERT_OPERATOR_HPP_

#include <cstddef>
#include <string>
#include <memory>
#include <utility>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "types/containers/Tuple.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class InsertDestination;
class StorageManager;
class WorkOrderProtosContainer;
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
   * @param query_id The ID of the query to which this operator belongs.
   * @param output_relation_id The output relation.
   * @param output_destination_index The index of the InsertDestination in the
   *        QueryContext to insert the tuple.
   * @param tuple_index The index of the tuple to insert in the QueryContext.
   **/
  InsertOperator(
      const std::size_t query_id,
      const CatalogRelation &output_relation,
      const QueryContext::insert_destination_id output_destination_index,
      const std::vector<QueryContext::tuple_id> &tuple_indexes)
      : RelationalOperator(query_id, 1u, false, output_relation.getNumPartitions()),
        output_relation_(output_relation),
        output_destination_index_(output_destination_index),
        tuple_indexes_(tuple_indexes),
        work_generated_(false) {}

  ~InsertOperator() override {}

  OperatorType getOperatorType() const override {
    return kInsert;
  }

  std::string getName() const override {
    return "InsertOperator";
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
  const std::vector<QueryContext::tuple_id> tuple_indexes_;
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
   * @note InsertWorkOrder takes ownership of \c tuple.
   *
   * @param query_id The ID of the query to which this WorkOrder belongs.
   * @param output_destination The InsertDestination to insert the tuple.
   * @param tuple The tuple to insert.
   **/
  InsertWorkOrder(const std::size_t query_id,
                  InsertDestination *output_destination,
                  std::vector<std::unique_ptr<Tuple>> &&tuples)
      : WorkOrder(query_id),
        output_destination_(DCHECK_NOTNULL(output_destination)),
        tuples_(std::move(tuples)) {}

  ~InsertWorkOrder() override {}

  /**
   * @exception TupleTooLargeForBlock The tuple was too large to insert into an
   *            empty block provided by output_destination_index_ in query_context.
   **/
  void execute() override;

 private:
  InsertDestination *output_destination_;
  std::vector<std::unique_ptr<Tuple>> tuples_;

  DISALLOW_COPY_AND_ASSIGN(InsertWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_INSERT_OPERATOR_HPP_
