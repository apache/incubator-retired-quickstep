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

#include "storage/WindowAggregationOperationState.hpp"

#include <cstddef>
#include <cstdio>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogDatabaseLite.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/ExpressionFactories.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "expressions/window_aggregation/WindowAggregateFunction.hpp"
#include "expressions/window_aggregation/WindowAggregateFunctionFactory.hpp"
#include "expressions/window_aggregation/WindowAggregationHandle.hpp"
#include "expressions/window_aggregation/WindowAggregationID.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageManager.hpp"
#include "storage/SubBlocksReference.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "storage/WindowAggregationOperationState.pb.h"
#include "types/containers/ColumnVector.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "types/containers/ColumnVectorUtil.hpp"
#include "utility/ColumnVectorCache.hpp"

#include "glog/logging.h"

namespace quickstep {

WindowAggregationOperationState::WindowAggregationOperationState(
    const CatalogRelationSchema &input_relation,
    const WindowAggregateFunction *window_aggregate_function,
    std::vector<std::unique_ptr<const Scalar>> &&arguments,
    const std::vector<std::unique_ptr<const Scalar>> &partition_by_attributes,
    const std::vector<std::unique_ptr<const Scalar>> &order_by_attributes,
    const bool is_row,
    const std::int64_t num_preceding,
    const std::int64_t num_following,
    StorageManager *storage_manager)
    : input_relation_(input_relation),
      arguments_(std::move(arguments)),
      storage_manager_(storage_manager) {
  // Get the Types of this window aggregate's arguments so that we can create an
  // AggregationHandle.
  std::vector<const Type*> argument_types;
  for (const std::unique_ptr<const Scalar> &argument : arguments_) {
    argument_types.emplace_back(&argument->getType());
  }

  // Check if window aggregate function could apply to the arguments.
  DCHECK(window_aggregate_function->canApplyToTypes(argument_types));

  // Create the handle and initial state.
  window_aggregation_handle_.reset(
      window_aggregate_function->createHandle(argument_types,
                                              partition_by_attributes,
                                              order_by_attributes,
                                              is_row,
                                              num_preceding,
                                              num_following));
}

WindowAggregationOperationState* WindowAggregationOperationState::ReconstructFromProto(
    const serialization::WindowAggregationOperationState &proto,
    const CatalogDatabaseLite &database,
    StorageManager *storage_manager) {
  DCHECK(ProtoIsValid(proto, database));

  std::vector<std::unique_ptr<const Scalar>> arguments;
  arguments.reserve(proto.arguments_size());
  for (int argument_idx = 0; argument_idx < proto.arguments_size(); ++argument_idx) {
    arguments.emplace_back(ScalarFactory::ReconstructFromProto(
        proto.arguments(argument_idx),
        database));
  }

  std::vector<std::unique_ptr<const Scalar>> partition_by_attributes;
  for (int attribute_idx = 0;
       attribute_idx < proto.partition_by_attributes_size();
       ++attribute_idx) {
    partition_by_attributes.emplace_back(ScalarFactory::ReconstructFromProto(
        proto.partition_by_attributes(attribute_idx),
        database));
  }

  std::vector<std::unique_ptr<const Scalar>> order_by_attributes;
  for (int attribute_idx = 0;
       attribute_idx < proto.order_by_attributes_size();
       ++attribute_idx) {
    order_by_attributes.emplace_back(ScalarFactory::ReconstructFromProto(
        proto.order_by_attributes(attribute_idx),
        database));
  }

  const bool is_row = proto.is_row();
  const std::int64_t num_preceding = proto.num_preceding();
  const std::int64_t num_following = proto.num_following();

  return new WindowAggregationOperationState(database.getRelationSchemaById(proto.input_relation_id()),
                                             &WindowAggregateFunctionFactory::ReconstructFromProto(proto.function()),
                                             std::move(arguments),
                                             partition_by_attributes,
                                             order_by_attributes,
                                             is_row,
                                             num_preceding,
                                             num_following,
                                             storage_manager);
}

bool WindowAggregationOperationState::ProtoIsValid(const serialization::WindowAggregationOperationState &proto,
                                                   const CatalogDatabaseLite &database) {
  if (!proto.IsInitialized() ||
      !database.hasRelationWithId(proto.input_relation_id())) {
    return false;
  }

  if (!WindowAggregateFunctionFactory::ProtoIsValid(proto.function())) {
    return false;
  }

  // TODO(chasseur): We may also want to check that the specified
  // AggregateFunction is applicable to the specified arguments, but that
  // requires partial deserialization and may be too heavyweight for this
  // method.
  // TODO(Shixuan): The TODO for AggregateFunction could also be applied here.
  for (int argument_idx = 0;
       argument_idx < proto.arguments_size();
       ++argument_idx) {
    if (!ScalarFactory::ProtoIsValid(proto.arguments(argument_idx), database)) {
      return false;
    }
  }

  for (int attribute_idx = 0;
       attribute_idx < proto.partition_by_attributes_size();
       ++attribute_idx) {
    if (!ScalarFactory::ProtoIsValid(proto.partition_by_attributes(attribute_idx),
                                     database)) {
      return false;
    }
  }

  for (int attribute_idx = 0;
       attribute_idx < proto.order_by_attributes_size();
       ++attribute_idx) {
    if (!ScalarFactory::ProtoIsValid(proto.order_by_attributes(attribute_idx),
                                     database)) {
      return false;
    }
  }

  if (proto.num_preceding() < -1 || proto.num_following() < -1) {
    return false;
  }

  return true;
}

void WindowAggregationOperationState::windowAggregateBlocks(
    InsertDestination *output_destination,
    const std::vector<block_id> &block_ids) {
  // TODO(Shixuan): This is a quick fix for currently unsupported functions in
  // order to pass the query_optimizer test.
  if (window_aggregation_handle_.get() == nullptr) {
    std::cout << "The function will be supported in the near future :)\n";
    return;
  }

  // Get the total number of tuples.
  int num_tuples = 0;
  for (const block_id block_idx : block_ids) {
    num_tuples +=
        storage_manager_->getBlock(block_idx, input_relation_)->getNumTuples();
  }

  // Construct column vectors for attributes.
  std::vector<ColumnVector*> attribute_vecs;
  for (std::size_t attr_id = 0; attr_id < input_relation_.size(); ++attr_id) {
    const CatalogAttribute *attr = input_relation_.getAttributeById(attr_id);
    const Type &type = attr->getType();

    if (NativeColumnVector::UsableForType(type)) {
      attribute_vecs.push_back(new NativeColumnVector(type, num_tuples));
    } else {
      attribute_vecs.push_back(new IndirectColumnVector(type, num_tuples));
    }
  }

  // Construct column vectors for arguments.
  std::vector<ColumnVector*> argument_vecs;
  for (const std::unique_ptr<const Scalar> &argument : arguments_) {
    const Type &type = argument->getType();

    if (NativeColumnVector::UsableForType(type)) {
      argument_vecs.push_back(new NativeColumnVector(type, num_tuples));
    } else {
      argument_vecs.push_back(new IndirectColumnVector(type, num_tuples));
    }
  }

  // TODO(Shixuan): Add Support for Vector Copy Elision Selection.
  // Add tuples and arguments into ColumnVectors.
  for (const block_id block_idx : block_ids) {
    BlockReference block = storage_manager_->getBlock(block_idx, input_relation_);
    const TupleStorageSubBlock &tuple_block = block->getTupleStorageSubBlock();
    SubBlocksReference sub_block_ref(tuple_block,
                                     block->getIndices(),
                                     block->getIndicesConsistent());
    ValueAccessor *tuple_accessor = tuple_block.createValueAccessor();
    ColumnVectorsValueAccessor *argument_accessor = nullptr;
    if (!arguments_.empty()) {
      argument_accessor = new ColumnVectorsValueAccessor();
    }

    std::unique_ptr<ColumnVectorCache> cv_cache = std::make_unique<ColumnVectorCache>();
    for (const std::unique_ptr<const Scalar> &argument : arguments_) {
      argument_accessor->addColumn(argument->getAllValues(tuple_accessor,
                                                          &sub_block_ref,
                                                          cv_cache.get()));
    }

    // Release common subexpression cache as early as possible.
    cv_cache.reset();

    InvokeOnAnyValueAccessor(tuple_accessor,
                             [&] (auto *tuple_accessor) -> void {  // NOLINT(build/c++11)
      tuple_accessor->beginIteration();
      if (argument_accessor != nullptr) {
        argument_accessor->beginIteration();
      }

      while (tuple_accessor->next()) {
        if (argument_accessor != nullptr) {
          argument_accessor->next();
        }

        for (std::size_t attr_id = 0; attr_id < attribute_vecs.size(); ++attr_id) {
          ColumnVector *attr_vec = attribute_vecs[attr_id];
          if (attr_vec->isNative()) {
            static_cast<NativeColumnVector*>(attr_vec)->appendTypedValue(
                tuple_accessor->getTypedValue(attr_id));
          } else {
            static_cast<IndirectColumnVector*>(attr_vec)->appendTypedValue(
                tuple_accessor->getTypedValue(attr_id));
          }
        }

        for (std::size_t argument_idx = 0;
             argument_idx < argument_vecs.size();
             ++argument_idx) {
          ColumnVector *argument = argument_vecs[argument_idx];
          if (argument->isNative()) {
            static_cast<NativeColumnVector*>(argument)->appendTypedValue(
                argument_accessor->getTypedValue(argument_idx));
          } else {
            static_cast<IndirectColumnVector*>(argument)->appendTypedValue(
                argument_accessor->getTypedValue(argument_idx));
          }
        }
      }
    });
  }

  // Construct the value accessor for tuples in all blocks
  ColumnVectorsValueAccessor *all_blocks_accessor
      = new ColumnVectorsValueAccessor();
  for (ColumnVector *attr_vec : attribute_vecs) {
    all_blocks_accessor->addColumn(attr_vec);
  }

  // Do actual calculation in handle.
  ColumnVector *window_aggregates =
      window_aggregation_handle_->calculate(all_blocks_accessor,
                                            argument_vecs);

  all_blocks_accessor->addColumn(window_aggregates);
  output_destination->bulkInsertTuples(all_blocks_accessor);
}

}  // namespace quickstep
