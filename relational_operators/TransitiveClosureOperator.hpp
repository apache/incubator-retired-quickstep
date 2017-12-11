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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_TRANSITIVE_CLOSURE_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_TRANSITIVE_CLOSURE_OPERATOR_HPP_

#include <cstddef>
#include <string>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/TransitiveClosureState.hpp"
#include "types/containers/ColumnVector.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"
#include "utility/Range.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class NativeColumnVector;
class InsertDestination;
class StorageManager;
class WorkOrderProtosContainer;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

class TransitiveClosureOperator : public RelationalOperator {
 public:
  TransitiveClosureOperator(const std::size_t query_id,
                            const std::size_t transitive_closure_context_index,
                            const CatalogRelation &output_relation,
                            const QueryContext::insert_destination_id output_destination_index)
      : RelationalOperator(query_id, 1u),
        transitive_closure_context_index_(transitive_closure_context_index),
        output_relation_(output_relation),
        output_destination_index_(output_destination_index),
        started_(false) {
  }

  ~TransitiveClosureOperator() override {}

  OperatorType getOperatorType() const override {
    return kTransitiveClosure;
  }

  std::string getName() const override {
    return "TransitiveClosureOperator";
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
  const std::size_t transitive_closure_context_index_;
  const CatalogRelation &output_relation_;
  const QueryContext::insert_destination_id output_destination_index_;

  bool started_;

  DISALLOW_COPY_AND_ASSIGN(TransitiveClosureOperator);
};

class TransitiveClosureWorkOrder : public WorkOrder {
 public:
  TransitiveClosureWorkOrder(const std::size_t query_id,
                             const Range &interval,
                             TransitiveClosureState *state,
                             InsertDestination *output_destination)
      : WorkOrder(query_id, 1u),
        interval_(interval),
        range_(state->range()),
        state_(state),
        output_destination_(output_destination) {}

  ~TransitiveClosureWorkOrder() override {}

  void execute() override;

 private:
  void evaluateSingleSource(const int start,
                            std::vector<int> *delta,
                            BitVector<false> *next,
                            BitVector<false> *result) const;

  void bulkInsert(const ColumnVectorPtr &src_cv, const ColumnVectorPtr &dst_cv);

  const Range interval_;
  const int range_;
  TransitiveClosureState *state_;
  InsertDestination *output_destination_;

  DISALLOW_COPY_AND_ASSIGN(TransitiveClosureWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_TRANSITIVE_CLOSURE_OPERATOR_HPP_
