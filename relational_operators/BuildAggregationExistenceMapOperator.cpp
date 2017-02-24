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

#include "relational_operators/BuildAggregationExistenceMapOperator.hpp"

#include <memory>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/AggregationOperationState.hpp"
#include "storage/CollisionFreeVectorTable.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "utility/BarrieredReadWriteConcurrentBitVector.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

namespace {

template <typename CppType, bool is_attr_nullable>
void ExecuteBuild(const attribute_id attr_id,
                  ValueAccessor *accessor,
                  BarrieredReadWriteConcurrentBitVector *existence_map) {
  InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    accessor->beginIteration();
    while (accessor->next()) {
      const void *value = accessor->template getUntypedValue<is_attr_nullable>(attr_id);
      if (!is_attr_nullable || value != nullptr) {
        existence_map->setBit(*reinterpret_cast<const CppType *>(value));
      }
    }
  });
}

// Dispatch helper.
template <typename CppType>
void ExecuteHelper(const attribute_id attr_id,
                   const bool is_attr_nullable,
                   ValueAccessor *accessor,
                   BarrieredReadWriteConcurrentBitVector *existence_map)  {
  if (is_attr_nullable) {
    ExecuteBuild<CppType, true>(attr_id, accessor, existence_map);
  } else {
    ExecuteBuild<CppType, false>(attr_id, accessor, existence_map);
  }
}

}  // namespace

bool BuildAggregationExistenceMapOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  if (input_relation_is_stored_) {
    if (!started_) {
      for (const block_id input_block_id : input_relation_block_ids_) {
        container->addNormalWorkOrder(
            new BuildAggregationExistenceMapWorkOrder(
                query_id_,
                input_relation_,
                input_block_id,
                build_attribute_,
                query_context->getAggregationState(aggr_state_index_),
                storage_manager),
            op_index_);
      }
      started_ = true;
    }
    return true;
  } else {
    while (num_workorders_generated_ < input_relation_block_ids_.size()) {
      container->addNormalWorkOrder(
          new BuildAggregationExistenceMapWorkOrder(
                query_id_,
                input_relation_,
                input_relation_block_ids_[num_workorders_generated_],
                build_attribute_,
                query_context->getAggregationState(aggr_state_index_),
                storage_manager),
          op_index_);
      ++num_workorders_generated_;
    }
    return done_feeding_input_relation_;
  }
}

bool BuildAggregationExistenceMapOperator
    ::getAllWorkOrderProtos(WorkOrderProtosContainer *container) {
  if (input_relation_is_stored_) {
    if (!started_) {
      for (const block_id block : input_relation_block_ids_) {
        container->addWorkOrderProto(createWorkOrderProto(block), op_index_);
      }
      started_ = true;
    }
    return true;
  } else {
    while (num_workorders_generated_ < input_relation_block_ids_.size()) {
      container->addWorkOrderProto(
          createWorkOrderProto(input_relation_block_ids_[num_workorders_generated_]),
          op_index_);
      ++num_workorders_generated_;
    }
    return done_feeding_input_relation_;
  }
}

serialization::WorkOrder* BuildAggregationExistenceMapOperator
    ::createWorkOrderProto(const block_id block) {
  serialization::WorkOrder *proto = new serialization::WorkOrder;
  proto->set_work_order_type(serialization::BUILD_AGGREGATION_EXISTENCE_MAP);
  proto->set_query_id(query_id_);

  proto->SetExtension(serialization::BuildAggregationExistenceMapWorkOrder::relation_id,
                      input_relation_.getID());
  proto->SetExtension(serialization::BuildAggregationExistenceMapWorkOrder::build_block_id,
                      block);
  proto->SetExtension(serialization::BuildAggregationExistenceMapWorkOrder::build_attribute,
                      build_attribute_);
  proto->SetExtension(serialization::BuildAggregationExistenceMapWorkOrder::aggr_state_index,
                      aggr_state_index_);
  return proto;
}

void BuildAggregationExistenceMapWorkOrder::execute() {
  BlockReference block(
      storage_manager_->getBlock(build_block_id_, input_relation_));
  std::unique_ptr<ValueAccessor> accessor(
      block->getTupleStorageSubBlock().createValueAccessor());

  CollisionFreeVectorTable *aggregate_table =
      state_->getCollisionFreeVectorTable();
  DCHECK(aggregate_table != nullptr);

  BarrieredReadWriteConcurrentBitVector *existence_map =
      aggregate_table->getExistenceMap();

  const Type &attr_type =
      input_relation_.getAttributeById(build_attribute_)->getType();
  switch (attr_type.getTypeID()) {
    case TypeID::kInt:
      ExecuteHelper<int>(build_attribute_,
                         attr_type.isNullable(),
                         accessor.get(),
                         existence_map);
      return;
    case TypeID::kLong:
      ExecuteHelper<std::int64_t>(build_attribute_,
                                  attr_type.isNullable(),
                                  accessor.get(),
                                  existence_map);
      return;
    default:
      LOG(FATAL) << "Build attribute type not supported by "
                 << "BuildAggregationExistenceMapOperator: "
                 << attr_type.getName();
  }
}

}  // namespace quickstep
