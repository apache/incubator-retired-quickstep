/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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
#include "expressions/aggregation/AggregateFunction.hpp"
#include "expressions/aggregation/AggregateFunctionFactory.hpp"
#include "expressions/aggregation/AggregationHandle.hpp"
#include "expressions/aggregation/AggregationID.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "storage/StorageManager.hpp"
#include "storage/WindowAggregationOperationState.pb.h"

#include "glog/logging.h"

namespace quickstep {

WindowAggregationOperationState::WindowAggregationOperationState(
    const CatalogRelationSchema &input_relation,
    const AggregateFunction *window_aggregate_function,
    std::vector<std::unique_ptr<const Scalar>> &&arguments,
    std::vector<std::unique_ptr<const Scalar>> &&partition_by_attributes,
    const bool is_row,
    const std::int64_t num_preceding,
    const std::int64_t num_following,
    StorageManager *storage_manager)
    : input_relation_(input_relation),
      arguments_(std::move(arguments)),
      partition_by_attributes_(std::move(partition_by_attributes)),
      is_row_(is_row),
      num_preceding_(num_preceding),
      num_following_(num_following),
      storage_manager_(storage_manager) {
  // Get the Types of this window aggregate's arguments so that we can create an
  // AggregationHandle.
  // TODO(Shixuan): Next step: New handles for window aggregation function.
  std::vector<const Type*> argument_types;
  for (const std::unique_ptr<const Scalar> &argument : arguments_) {
    argument_types.emplace_back(&argument->getType());
  }

  // Check if window aggregate function could apply to the arguments.
  DCHECK(window_aggregate_function->canApplyToTypes(argument_types));

  // Create the handle and initial state.
  window_aggregation_handle_.reset(
      window_aggregate_function->createHandle(argument_types));
  window_aggregation_state_.reset(
      window_aggregation_handle_->createInitialState());

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  // See if all of this window aggregate's arguments are attributes in the input
  // relation. If so, remember the attribute IDs so that we can do copy elision
  // when actually performing the window aggregation.
  arguments_as_attributes_.reserve(arguments_.size());
  for (const std::unique_ptr<const Scalar> &argument : arguments_) {
    const attribute_id argument_id = argument->getAttributeIdForValueAccessor();
    if (argument_id == -1) {
      arguments_as_attributes_.clear();
      break;
    } else {
      DCHECK_EQ(input_relation_.getID(), argument->getRelationIdForValueAccessor());
      arguments_as_attributes_.push_back(argument_id);
    }
  }
#endif
}

WindowAggregationOperationState* WindowAggregationOperationState::ReconstructFromProto(
    const serialization::WindowAggregationOperationState &proto,
    const CatalogDatabaseLite &database,
    StorageManager *storage_manager) {
  DCHECK(ProtoIsValid(proto, database));

  // Rebuild contructor arguments from their representation in 'proto'.
  const AggregateFunction *aggregate_function
      = &AggregateFunctionFactory::ReconstructFromProto(proto.function());

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

  const bool is_row = proto.is_row();
  const std::int64_t num_preceding = proto.num_preceding();
  const std::int64_t num_following = proto.num_following();

  return new WindowAggregationOperationState(database.getRelationSchemaById(proto.relation_id()),
                                             aggregate_function,
                                             std::move(arguments),
                                             std::move(partition_by_attributes),
                                             is_row,
                                             num_preceding,
                                             num_following,
                                             storage_manager);
}

bool WindowAggregationOperationState::ProtoIsValid(const serialization::WindowAggregationOperationState &proto,
                                                   const CatalogDatabaseLite &database) {
  if (!proto.IsInitialized() ||
      !database.hasRelationWithId(proto.relation_id())) {
    return false;
  }

  if (!AggregateFunctionFactory::ProtoIsValid(proto.function())) {
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

  if (proto.num_preceding() < -1 || proto.num_following() < -1) {
    return false;
  }

  return true;
}

}  // namespace quickstep
