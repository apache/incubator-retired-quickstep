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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_BUILD_TRANSITIVE_CLOSURE_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_BUILD_TRANSITIVE_CLOSURE_OPERATOR_HPP_

#include <cstddef>
#include <string>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/TransitiveClosureState.hpp"
#include "utility/BlockIDStream.hpp"
#include "utility/Macros.hpp"
#include "utility/Range.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class CatalogRelationSchema;
class StorageManager;
class WorkOrderProtosContainer;
class WorkOrdersContainer;
class ValueAccessor;

/** \addtogroup RelationalOperators
 *  @{
 */

class BuildTransitiveClosureOperator : public RelationalOperator {
 public:
  BuildTransitiveClosureOperator(const std::size_t query_id,
                                 const std::size_t transitive_closure_context_index,
                                 const CatalogRelation &start_relation,
                                 const CatalogRelation &edge_relation,
                                 const bool start_relation_is_stored,
                                 const bool edge_relation_is_stored,
                                 const attribute_id start_attr_id,
                                 const attribute_id source_attr_id,
                                 const attribute_id destination_attr_id)
      : RelationalOperator(query_id, 1u),
        transitive_closure_context_index_(transitive_closure_context_index),
        start_relation_(start_relation),
        edge_relation_(edge_relation),
        start_attr_id_(start_attr_id),
        source_attr_id_(source_attr_id),
        destination_attr_id_(destination_attr_id),
        start_relation_block_stream_(start_relation,
                                     start_relation_is_stored,
                                     done_feeding_input_relation_),
        edge_relation_block_stream_(edge_relation,
                                    edge_relation_is_stored,
                                    done_feeding_input_relation_) {
  }

  ~BuildTransitiveClosureOperator() override {}

  OperatorType getOperatorType() const override {
    return kBuildTransitiveClosure;
  }

  std::string getName() const override {
    return "BuildTransitiveClosureOperator";
  }

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

  bool getAllWorkOrderProtos(WorkOrderProtosContainer *container) override;

  void feedInputBlock(const block_id input_block_id,
                      const relation_id input_relation_id,
                      const partition_id part_id) override {
    if (input_relation_id == edge_relation_.getID()) {
      edge_relation_block_stream_.append(input_block_id);
    } else {
      DCHECK_EQ(start_relation_.getID(), input_relation_id);
      start_relation_block_stream_.append(input_block_id);
    }
  }

 private:
  bool doneWorkOrderGeneration() const {
    return start_relation_block_stream_.isEndOfStream()
        && edge_relation_block_stream_.isEndOfStream();
  }

  const std::size_t transitive_closure_context_index_;

  const CatalogRelation &start_relation_;
  const CatalogRelation &edge_relation_;

  const attribute_id start_attr_id_;
  const attribute_id source_attr_id_;
  const attribute_id destination_attr_id_;

  BlockIDStream start_relation_block_stream_;
  BlockIDStream edge_relation_block_stream_;

  DISALLOW_COPY_AND_ASSIGN(BuildTransitiveClosureOperator);
};

class BuildTransitiveClosureWorkOrder : public WorkOrder {
 public:
  BuildTransitiveClosureWorkOrder(const std::size_t query_id,
                                  const CatalogRelationSchema &input_relation,
                                  const bool is_start_relation,
                                  const block_id block,
                                  const attribute_id start_attr_id,
                                  const attribute_id source_attr_id,
                                  const attribute_id destination_attr_id,
                                  TransitiveClosureState *state,
                                  StorageManager *storage_manager)
      : WorkOrder(query_id, 1u),
        input_relation_(input_relation),
        is_start_relation_(is_start_relation),
        block_(block),
        start_attr_id_(start_attr_id),
        source_attr_id_(source_attr_id),
        destination_attr_id_(destination_attr_id),
        state_(state),
        storage_manager_(storage_manager) {
  }

  ~BuildTransitiveClosureWorkOrder() override {}

  void execute() override;

 private:
  void buildStartRelation(ValueAccessor *accessor);
  void buildEdgeRelation(ValueAccessor *accessor);

  const CatalogRelationSchema &input_relation_;
  const bool is_start_relation_;
  const block_id block_;

  const attribute_id start_attr_id_;
  const attribute_id source_attr_id_;
  const attribute_id destination_attr_id_;

  TransitiveClosureState *state_;
  StorageManager *storage_manager_;

  DISALLOW_COPY_AND_ASSIGN(BuildTransitiveClosureWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_BUILD_TRANSITIVE_CLOSURE_OPERATOR_HPP_
