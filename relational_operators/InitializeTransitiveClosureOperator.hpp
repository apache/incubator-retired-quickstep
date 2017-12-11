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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_INITIALIZE_TRANSITIVE_CLOSURE_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_INITIALIZE_TRANSITIVE_CLOSURE_OPERATOR_HPP_

#include <cstddef>
#include <string>
#include <vector>

#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/TransitiveClosureState.hpp"
#include "utility/Macros.hpp"
#include "utility/Range.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class StorageManager;
class WorkOrderProtosContainer;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

class InitializeTransitiveClosureOperator : public RelationalOperator {
 public:
  InitializeTransitiveClosureOperator(const std::size_t query_id,
                                      const std::size_t transitive_closure_context_index)
      : RelationalOperator(query_id, 1u),
        transitive_closure_context_index_(transitive_closure_context_index),
        started_(false) {
  }

  ~InitializeTransitiveClosureOperator() override {}

  OperatorType getOperatorType() const override {
    return kInitializeTransitiveClosure;
  }

  std::string getName() const override {
    return "InitializeTransitiveClosureOperator";
  }

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

  bool getAllWorkOrderProtos(WorkOrderProtosContainer *container) override;

 private:
  const std::size_t transitive_closure_context_index_;
  bool started_;

  DISALLOW_COPY_AND_ASSIGN(InitializeTransitiveClosureOperator);
};

class InitializeTransitiveClosureWorkOrder : public WorkOrder {
 public:
  InitializeTransitiveClosureWorkOrder(const std::size_t query_id,
                                       const Range &range,
                                       TransitiveClosureState *state)
      : WorkOrder(query_id, 1u),
        range_(range),
        state_(state) {}

  ~InitializeTransitiveClosureWorkOrder() override {}

  void execute() override;

 private:
  const Range range_;
  TransitiveClosureState *state_;

  DISALLOW_COPY_AND_ASSIGN(InitializeTransitiveClosureWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_INITIALIZE_TRANSITIVE_CLOSURE_OPERATOR_HPP_
