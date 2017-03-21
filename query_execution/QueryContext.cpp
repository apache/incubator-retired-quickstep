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

#include "query_execution/QueryContext.hpp"

#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "catalog/CatalogDatabaseLite.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/ExpressionFactories.hpp"
#include "expressions/table_generator/GeneratorFunction.pb.h"
#include "expressions/table_generator/GeneratorFunctionFactory.hpp"
#include "expressions/table_generator/GeneratorFunctionHandle.hpp"
#include "query_execution/QueryContext.pb.h"
#include "storage/AggregationOperationState.hpp"
#include "storage/HashTable.hpp"
#include "storage/HashTableFactory.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/InsertDestination.pb.h"
#include "types/TypedValue.hpp"
#include "types/containers/Tuple.hpp"
#include "utility/SortConfiguration.hpp"
#include "utility/lip_filter/LIPFilter.hpp"
#include "utility/lip_filter/LIPFilterDeployment.hpp"
#include "utility/lip_filter/LIPFilterFactory.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

using std::move;
using std::unique_ptr;
using std::vector;

namespace quickstep {

QueryContext::QueryContext(const serialization::QueryContext &proto,
                           const CatalogDatabaseLite &database,
                           StorageManager *storage_manager,
                           const tmb::client_id scheduler_client_id,
                           tmb::MessageBus *bus) {
  DCHECK(ProtoIsValid(proto, database))
      << "Attempted to create QueryContext from an invalid proto description:\n"
      << proto.DebugString();

  for (int i = 0; i < proto.aggregation_states_size(); ++i) {
    aggregation_states_.emplace_back(
        AggregationOperationState::ReconstructFromProto(proto.aggregation_states(i),
                                                        database,
                                                        storage_manager));
  }

  for (int i = 0; i < proto.generator_functions_size(); ++i) {
    const GeneratorFunctionHandle *func_handle =
        GeneratorFunctionFactory::Instance().reconstructFromProto(proto.generator_functions(i));
    DCHECK(func_handle != nullptr);
    generator_functions_.emplace_back(
        std::unique_ptr<const GeneratorFunctionHandle>(func_handle));
  }

  for (int i = 0; i < proto.join_hash_tables_size(); ++i) {
    PartitionedJoinHashTables partitioned_join_hash_tables;

    const serialization::QueryContext::HashTableContext &hash_table_context_proto = proto.join_hash_tables(i);
    for (std::uint64_t j = 0; j < hash_table_context_proto.num_partitions(); ++j) {
      partitioned_join_hash_tables.emplace_back(
          JoinHashTableFactory::CreateResizableFromProto(hash_table_context_proto.join_hash_table(), storage_manager));
    }

    join_hash_tables_.push_back(move(partitioned_join_hash_tables));
  }

  for (int i = 0; i < proto.insert_destinations_size(); ++i) {
    const serialization::InsertDestination &insert_destination_proto = proto.insert_destinations(i);
    insert_destinations_.emplace_back(InsertDestination::ReconstructFromProto(
        proto.query_id(),
        insert_destination_proto,
        database.getRelationSchemaById(insert_destination_proto.relation_id()),
        storage_manager,
        scheduler_client_id,
        bus));
  }

  for (int i = 0; i < proto.lip_filters_size(); ++i) {
    lip_filters_.emplace_back(
        std::unique_ptr<LIPFilter>(
            LIPFilterFactory::ReconstructFromProto(proto.lip_filters(i))));
  }

  for (int i = 0; i < proto.lip_filter_deployments_size(); ++i) {
    lip_deployments_.emplace_back(
        std::make_unique<LIPFilterDeployment>(
            proto.lip_filter_deployments(i), lip_filters_));
  }

  for (int i = 0; i < proto.predicates_size(); ++i) {
    predicates_.emplace_back(
        PredicateFactory::ReconstructFromProto(proto.predicates(i), database));
  }

  for (int i = 0; i < proto.scalar_groups_size(); ++i) {
    vector<unique_ptr<const Scalar>> scalar_group;

    const serialization::QueryContext::ScalarGroup &scalar_group_proto = proto.scalar_groups(i);
    for (int j = 0; j < scalar_group_proto.scalars_size(); ++j) {
      scalar_group.emplace_back(
          ScalarFactory::ReconstructFromProto(scalar_group_proto.scalars(j), database));
    }

    scalar_groups_.push_back(move(scalar_group));
  }

  for (int i = 0; i < proto.sort_configs_size(); ++i) {
    sort_configs_.emplace_back(
        SortConfiguration::ReconstructFromProto(proto.sort_configs(i), database));
  }

  for (int i = 0; i < proto.tuples_size(); ++i) {
    tuples_.emplace_back(Tuple::ReconstructFromProto(proto.tuples(i)));
  }

  for (int i = 0; i < proto.update_groups_size(); ++i) {
    const serialization::QueryContext::UpdateGroup &update_group_proto = proto.update_groups(i);

    std::unordered_map<attribute_id, std::unique_ptr<const Scalar>> update_group;
    for (int j = 0; j < update_group_proto.update_assignments_size(); ++j) {
      const serialization::QueryContext::UpdateGroup::UpdateAssignment &update_assignment_proto =
          update_group_proto.update_assignments(j);

      unique_ptr<const Scalar> scalar(
          ScalarFactory::ReconstructFromProto(update_assignment_proto.scalar(), database));

      update_group.emplace(update_assignment_proto.attribute_id(), move(scalar));
    }

    update_groups_.push_back(move(update_group));
  }

  for (int i = 0; i < proto.window_aggregation_states_size(); ++i) {
    window_aggregation_states_.emplace_back(
        WindowAggregationOperationState::ReconstructFromProto(proto.window_aggregation_states(i),
                                                              database,
                                                              storage_manager));
  }
}

bool QueryContext::ProtoIsValid(const serialization::QueryContext &proto,
                                const CatalogDatabaseLite &database) {
  for (int i = 0; i < proto.aggregation_states_size(); ++i) {
    if (!AggregationOperationState::ProtoIsValid(proto.aggregation_states(i), database)) {
      return false;
    }
  }

  // Each GeneratorFunctionHandle object is serialized as a function name with
  // a list of arguments. Here checks that the arguments are valid TypedValue's.
  for (int i = 0; i < proto.generator_functions_size(); ++i) {
    const serialization::GeneratorFunctionHandle &func_proto = proto.generator_functions(i);
    for (int j = 0; j < func_proto.args_size(); ++j) {
      if (!TypedValue::ProtoIsValid(func_proto.args(j))) {
        return false;
      }
    }
  }

  for (int i = 0; i < proto.join_hash_tables_size(); ++i) {
    if (!JoinHashTableFactory::ProtoIsValid(proto.join_hash_tables(i).join_hash_table())) {
      return false;
    }
  }

  for (int i = 0; i < proto.insert_destinations_size(); ++i) {
    const serialization::InsertDestination &insert_destination_proto = proto.insert_destinations(i);
    const relation_id rel_id = insert_destination_proto.relation_id();

    if (!database.hasRelationWithId(rel_id) ||
        !InsertDestination::ProtoIsValid(insert_destination_proto,
                                         database.getRelationSchemaById(rel_id))) {
      return false;
    }
  }

  for (int i = 0; i < proto.lip_filters_size(); ++i) {
    if (!LIPFilterFactory::ProtoIsValid(proto.lip_filters(i))) {
      return false;
    }
  }

  for (int i = 0; i < proto.lip_filter_deployments_size(); ++i) {
    if (!LIPFilterDeployment::ProtoIsValid(proto.lip_filter_deployments(i))) {
      return false;
    }
  }

  for (int i = 0; i < proto.predicates_size(); ++i) {
    if (!PredicateFactory::ProtoIsValid(proto.predicates(i), database)) {
      return false;
    }
  }

  for (int i = 0; i < proto.scalar_groups_size(); ++i) {
    const serialization::QueryContext::ScalarGroup &scalar_group_proto = proto.scalar_groups(i);
    for (int j = 0; j < scalar_group_proto.scalars_size(); ++j) {
      if (!ScalarFactory::ProtoIsValid(scalar_group_proto.scalars(j), database)) {
        return false;
      }
    }
  }

  for (int i = 0; i < proto.sort_configs_size(); ++i) {
    if (!SortConfiguration::ProtoIsValid(proto.sort_configs(i), database)) {
      return false;
    }
  }

  for (int i = 0; i < proto.tuples_size(); ++i) {
    if (!Tuple::ProtoIsValid(proto.tuples(i))) {
      return false;
    }
  }

  for (int i = 0; i < proto.update_groups_size(); ++i) {
    const serialization::QueryContext::UpdateGroup &update_group_proto = proto.update_groups(i);

    const relation_id rel_id = update_group_proto.relation_id();
    if (!database.hasRelationWithId(rel_id)) {
      return false;
    }
    const CatalogRelationSchema &rel = database.getRelationSchemaById(rel_id);

    for (int j = 0; j < update_group_proto.update_assignments_size(); ++j) {
      const serialization::QueryContext::UpdateGroup::UpdateAssignment &update_assignment_proto =
          update_group_proto.update_assignments(j);

      if (!rel.hasAttributeWithId(update_assignment_proto.attribute_id()) ||
          !ScalarFactory::ProtoIsValid(update_assignment_proto.scalar(), database)) {
        return false;
      }
    }
  }

  for (int i = 0; i < proto.window_aggregation_states_size(); ++i) {
    if (!WindowAggregationOperationState::ProtoIsValid(proto.window_aggregation_states(i),
                                                       database)) {
      return false;
    }
  }

  return proto.IsInitialized();
}

std::size_t QueryContext::getJoinHashTablesMemoryBytes() const {
  SpinSharedMutexSharedLock<false> lock(hash_tables_mutex_);
  std::size_t memory = 0;
  for (std::size_t hashtable_id = 0;
       hashtable_id < join_hash_tables_.size();
       ++hashtable_id) {
    for (std::size_t partition_num = 0;
         partition_num < join_hash_tables_[hashtable_id].size();
         ++partition_num) {
      if (join_hash_tables_[hashtable_id][partition_num] != nullptr) {
        memory += join_hash_tables_[hashtable_id][partition_num]
                      ->getHashTableMemorySizeBytes();
      }
    }
  }
  return memory;
}

std::size_t QueryContext::getAggregationStatesMemoryBytes() const {
  SpinSharedMutexSharedLock<false> lock(aggregation_states_mutex_);
  std::size_t memory = 0;
  for (std::size_t agg_state_id = 0;
       agg_state_id < aggregation_states_.size();
       ++agg_state_id) {
    if (aggregation_states_[agg_state_id] != nullptr) {
      memory += aggregation_states_[agg_state_id]->getMemoryConsumptionBytes();
    }
  }
  return memory;
}

void QueryContext::getTempRelationIDs(
    std::vector<relation_id> *temp_relation_ids) const {
  SpinSharedMutexSharedLock<false> lock(insert_destinations_mutex_);
  DCHECK(temp_relation_ids != nullptr);
  for (std::size_t id = 0; id < insert_destinations_.size(); ++id) {
    InsertDestination *curr_insert_dest = insert_destinations_[id].get();
    DCHECK(curr_insert_dest != nullptr);
    temp_relation_ids->emplace_back(curr_insert_dest->getRelation().getID());
  }
}

}  // namespace quickstep
