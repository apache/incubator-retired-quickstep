/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

#include <memory>

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
   * @param output_relation_id The output relation.
   * @param output_destination_index The index of the InsertDestination in the
   *        QueryContext to insert the tuple.
   * @param tuple_index The index of the tuple to insert in the QueryContext.
   **/
  InsertOperator(const CatalogRelation &output_relation,
                 const QueryContext::insert_destination_id output_destination_index,
                 const QueryContext::tuple_id tuple_index)
      : output_relation_(output_relation),
        output_destination_index_(output_destination_index),
        tuple_index_(tuple_index),
        work_generated_(false) {}

  ~InsertOperator() override {}

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
   * @note InsertWorkOrder takes ownership of \c tuple.
   *
   * @param output_destination The InsertDestination to insert the tuple.
   * @param tuple The tuple to insert.
   **/
  InsertWorkOrder(InsertDestination *output_destination,
                  Tuple *tuple)
      : output_destination_(DCHECK_NOTNULL(output_destination)),
        tuple_(DCHECK_NOTNULL(tuple)) {}

  ~InsertWorkOrder() override {}

  /**
   * @exception TupleTooLargeForBlock The tuple was too large to insert into an
   *            empty block provided by output_destination_index_ in query_context.
   **/
  void execute() override;

 private:
  InsertDestination *output_destination_;
  std::unique_ptr<Tuple> tuple_;

  DISALLOW_COPY_AND_ASSIGN(InsertWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_INSERT_OPERATOR_HPP_
