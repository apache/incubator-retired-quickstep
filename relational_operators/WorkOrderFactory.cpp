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

#include "relational_operators/WorkOrderFactory.hpp"

#include <memory>
#include <utility>
#include <vector>

#include "catalog/CatalogDatabaseLite.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/AggregationOperator.hpp"
#include "relational_operators/BuildHashOperator.hpp"
#include "relational_operators/DeleteOperator.hpp"
#include "relational_operators/DestroyHashOperator.hpp"
#include "relational_operators/DropTableOperator.hpp"
#include "relational_operators/FinalizeAggregationOperator.hpp"
#include "relational_operators/HashJoinOperator.hpp"
#include "relational_operators/InsertOperator.hpp"
#include "relational_operators/NestedLoopsJoinOperator.hpp"
#include "relational_operators/SampleOperator.hpp"
#include "relational_operators/SaveBlocksOperator.hpp"
#include "relational_operators/SelectOperator.hpp"
#include "relational_operators/SortMergeRunOperator.hpp"
#include "relational_operators/SortMergeRunOperator.pb.h"
#include "relational_operators/SortMergeRunOperatorHelpers.hpp"
#include "relational_operators/SortRunGenerationOperator.hpp"
#include "relational_operators/TableGeneratorOperator.hpp"
#include "relational_operators/TextScanOperator.hpp"
#include "relational_operators/UpdateOperator.hpp"
#include "relational_operators/WindowAggregationOperator.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/StorageBlockInfo.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

using std::move;
using std::vector;

namespace quickstep {

class InsertDestination;
class Predicate;
class Scalar;

WorkOrder* WorkOrderFactory::ReconstructFromProto(const serialization::WorkOrder &proto,
                                                  CatalogDatabaseLite *catalog_database,
                                                  QueryContext *query_context,
                                                  StorageManager *storage_manager,
                                                  const tmb::client_id shiftboss_client_id,
                                                  tmb::MessageBus *bus) {
  DCHECK(query_context != nullptr);
  DCHECK(ProtoIsValid(proto, *catalog_database, *query_context))
      << "Attempted to create WorkOrder from an invalid proto description:\n"
      << proto.DebugString();

  switch (proto.work_order_type()) {
    case serialization::AGGREGATION: {
      LOG(INFO) << "Creating AggregationWorkOrder";
      return new AggregationWorkOrder(
          proto.query_id(),
          proto.GetExtension(serialization::AggregationWorkOrder::block_id),
          query_context->getAggregationState(
              proto.GetExtension(serialization::AggregationWorkOrder::aggr_state_index)));
    }
    case serialization::BUILD_HASH: {
      LOG(INFO) << "Creating BuildHashWorkOrder";
      vector<attribute_id> join_key_attributes;
      for (int i = 0; i < proto.ExtensionSize(serialization::BuildHashWorkOrder::join_key_attributes); ++i) {
        join_key_attributes.push_back(
            proto.GetExtension(serialization::BuildHashWorkOrder::join_key_attributes, i));
      }

      return new BuildHashWorkOrder(
          proto.query_id(),
          catalog_database->getRelationSchemaById(
              proto.GetExtension(serialization::BuildHashWorkOrder::relation_id)),
          move(join_key_attributes),
          proto.GetExtension(serialization::BuildHashWorkOrder::any_join_key_attributes_nullable),
          proto.GetExtension(serialization::BuildHashWorkOrder::block_id),
          query_context->getJoinHashTable(
              proto.GetExtension(serialization::BuildHashWorkOrder::join_hash_table_index)),
          storage_manager);
    }
    case serialization::DELETE: {
      LOG(INFO) << "Creating DeleteWorkOrder";
      return new DeleteWorkOrder(
          proto.query_id(),
          catalog_database->getRelationSchemaById(
              proto.GetExtension(serialization::DeleteWorkOrder::relation_id)),
          proto.GetExtension(serialization::DeleteWorkOrder::block_id),
          query_context->getPredicate(
              proto.GetExtension(serialization::DeleteWorkOrder::predicate_index)),
          storage_manager,
          proto.GetExtension(serialization::DeleteWorkOrder::operator_index),
          shiftboss_client_id,
          bus);
    }
    case serialization::DESTROY_HASH: {
      LOG(INFO) << "Creating DestroyHashWorkOrder";
      return new DestroyHashWorkOrder(
          proto.query_id(),
          proto.GetExtension(
              serialization::DestroyHashWorkOrder::join_hash_table_index),
          query_context);
    }
    case serialization::DROP_TABLE: {
      LOG(INFO) << "Creating DropTableWorkOrder";
      vector<block_id> blocks;
      for (int i = 0; i < proto.ExtensionSize(serialization::DropTableWorkOrder::block_ids); ++i) {
        blocks.push_back(
            proto.GetExtension(serialization::DropTableWorkOrder::block_ids, i));
      }

      return new DropTableWorkOrder(
          proto.query_id(),
          move(blocks),
          storage_manager,
          proto.HasExtension(serialization::DropTableWorkOrder::relation_id)
              ? proto.GetExtension(serialization::DropTableWorkOrder::relation_id)
              : kInvalidCatalogId,
          catalog_database);
    }
    case serialization::FINALIZE_AGGREGATION: {
      LOG(INFO) << "Creating FinalizeAggregationWorkOrder";
      return new FinalizeAggregationWorkOrder(
          proto.query_id(),
          query_context->releaseAggregationState(proto.GetExtension(
              serialization::FinalizeAggregationWorkOrder::aggr_state_index)),
          query_context->getInsertDestination(
              proto.GetExtension(serialization::FinalizeAggregationWorkOrder::
                                     insert_destination_index)));
    }
    case serialization::HASH_JOIN: {
      const auto hash_join_work_order_type =
          proto.GetExtension(serialization::HashJoinWorkOrder::hash_join_work_order_type);

      const CatalogRelationSchema &build_relation =
          catalog_database->getRelationSchemaById(
              proto.GetExtension(serialization::HashJoinWorkOrder::build_relation_id));
      const CatalogRelationSchema &probe_relation =
          catalog_database->getRelationSchemaById(
              proto.GetExtension(serialization::HashJoinWorkOrder::probe_relation_id));

      vector<attribute_id> join_key_attributes;
      const int join_key_attributes_size =
          proto.ExtensionSize(serialization::HashJoinWorkOrder::join_key_attributes);
      for (int i = 0; i < join_key_attributes_size; ++i) {
        join_key_attributes.push_back(
            proto.GetExtension(serialization::HashJoinWorkOrder::join_key_attributes, i));
      }

      const bool any_join_key_attributes_nullable =
          proto.GetExtension(serialization::HashJoinWorkOrder::any_join_key_attributes_nullable);
      const block_id lookup_block_id =
          proto.GetExtension(serialization::HashJoinWorkOrder::block_id);

      const Predicate *residual_predicate = nullptr;
      if (hash_join_work_order_type != serialization::HashJoinWorkOrder::HASH_OUTER_JOIN) {
        residual_predicate =
            query_context->getPredicate(
                proto.GetExtension(serialization::HashJoinWorkOrder::residual_predicate_index));
      }

      const std::vector<std::unique_ptr<const Scalar>> &selection =
          query_context->getScalarGroup(
              proto.GetExtension(serialization::HashJoinWorkOrder::selection_index));
      const JoinHashTable &hash_table =
          *query_context->getJoinHashTable(
              proto.GetExtension(serialization::HashJoinWorkOrder::join_hash_table_index));
      InsertDestination *output_destination =
          query_context->getInsertDestination(
              proto.GetExtension(serialization::HashJoinWorkOrder::insert_destination_index));

      switch (hash_join_work_order_type) {
        case serialization::HashJoinWorkOrder::HASH_ANTI_JOIN: {
          LOG(INFO) << "Creating HashAntiJoinWorkOrder";
          return new HashAntiJoinWorkOrder(
              proto.query_id(),
              build_relation,
              probe_relation,
              move(join_key_attributes),
              any_join_key_attributes_nullable,
              lookup_block_id,
              residual_predicate,
              selection,
              hash_table,
              output_destination,
              storage_manager);
        }
        case serialization::HashJoinWorkOrder::HASH_INNER_JOIN: {
          LOG(INFO) << "Creating HashInnerJoinWorkOrder";
          return new HashInnerJoinWorkOrder(
              proto.query_id(),
              build_relation,
              probe_relation,
              move(join_key_attributes),
              any_join_key_attributes_nullable,
              lookup_block_id,
              residual_predicate,
              selection,
              hash_table,
              output_destination,
              storage_manager);
        }
        case serialization::HashJoinWorkOrder::HASH_OUTER_JOIN: {
          vector<bool> is_selection_on_build;
          const int is_selection_on_build_size =
              proto.ExtensionSize(serialization::HashJoinWorkOrder::is_selection_on_build);
          for (int i = 0; i < is_selection_on_build_size; ++i) {
            is_selection_on_build.push_back(
                proto.GetExtension(serialization::HashJoinWorkOrder::is_selection_on_build, i));
          }

          LOG(INFO) << "Creating HashOuterJoinWorkOrder";
          return new HashOuterJoinWorkOrder(
              proto.query_id(),
              build_relation,
              probe_relation,
              move(join_key_attributes),
              any_join_key_attributes_nullable,
              lookup_block_id,
              selection,
              move(is_selection_on_build),
              hash_table,
              output_destination,
              storage_manager);
        }
        case serialization::HashJoinWorkOrder::HASH_SEMI_JOIN: {
          LOG(INFO) << "Creating HashSemiJoinWorkOrder";
          return new HashSemiJoinWorkOrder(
              proto.query_id(),
              build_relation,
              probe_relation,
              move(join_key_attributes),
              any_join_key_attributes_nullable,
              lookup_block_id,
              residual_predicate,
              selection,
              hash_table,
              output_destination,
              storage_manager);
        }
        default:
          LOG(FATAL) << "Unknown HashJoinWorkOrder Type in WorkOrderFactory::ReconstructFromProto";
      }
    }
    case serialization::INSERT: {
      LOG(INFO) << "Creating InsertWorkOrder";
      return new InsertWorkOrder(
          proto.query_id(),
          query_context->getInsertDestination(
              proto.GetExtension(serialization::InsertWorkOrder::insert_destination_index)),
          query_context->releaseTuple(
              proto.GetExtension(serialization::InsertWorkOrder::tuple_index)));
    }
    case serialization::NESTED_LOOP_JOIN: {
      LOG(INFO) << "Creating NestedLoopsJoinWorkOrder";
      return new NestedLoopsJoinWorkOrder(
          proto.query_id(),
          catalog_database->getRelationSchemaById(
              proto.GetExtension(serialization::NestedLoopsJoinWorkOrder::left_relation_id)),
          catalog_database->getRelationSchemaById(
              proto.GetExtension(serialization::NestedLoopsJoinWorkOrder::right_relation_id)),
          proto.GetExtension(serialization::NestedLoopsJoinWorkOrder::left_block_id),
          proto.GetExtension(serialization::NestedLoopsJoinWorkOrder::right_block_id),
          query_context->getPredicate(
              proto.GetExtension(serialization::NestedLoopsJoinWorkOrder::join_predicate_index)),
          query_context->getScalarGroup(
              proto.GetExtension(serialization::NestedLoopsJoinWorkOrder::selection_index)),
          query_context->getInsertDestination(
              proto.GetExtension(serialization::NestedLoopsJoinWorkOrder::insert_destination_index)),
          storage_manager);
    }
    case serialization::SAMPLE: {
      LOG(INFO) << "Creating SampleWorkOrder";
      return new SampleWorkOrder(
          proto.query_id(),
          catalog_database->getRelationSchemaById(
              proto.GetExtension(serialization::SampleWorkOrder::relation_id)),
          proto.GetExtension(serialization::SampleWorkOrder::block_id),
          proto.GetExtension(serialization::SampleWorkOrder::is_block_sample),
          proto.GetExtension(serialization::SampleWorkOrder::percentage),
          query_context->getInsertDestination(
              proto.GetExtension(serialization::SampleWorkOrder::insert_destination_index)),
          storage_manager);
    }
    case serialization::SAVE_BLOCKS: {
      LOG(INFO) << "Creating SaveBlocksWorkOrder";
      return new SaveBlocksWorkOrder(
          proto.query_id(),
          proto.GetExtension(serialization::SaveBlocksWorkOrder::block_id),
          proto.GetExtension(serialization::SaveBlocksWorkOrder::force),
          storage_manager);
    }
    case serialization::SELECT: {
      LOG(INFO) << "Creating SelectWorkOrder";
      const bool simple_projection =
          proto.GetExtension(serialization::SelectWorkOrder::simple_projection);
      vector<attribute_id> simple_selection;
      for (int i = 0; i < proto.ExtensionSize(serialization::SelectWorkOrder::simple_selection); ++i) {
        simple_selection.push_back(
            proto.GetExtension(serialization::SelectWorkOrder::simple_selection, i));
      }

      return new SelectWorkOrder(
          proto.query_id(),
          catalog_database->getRelationSchemaById(
              proto.GetExtension(serialization::SelectWorkOrder::relation_id)),
          proto.GetExtension(serialization::SelectWorkOrder::block_id),
          query_context->getPredicate(
              proto.GetExtension(serialization::SelectWorkOrder::predicate_index)),
          simple_projection,
          move(simple_selection),
          simple_projection ? nullptr
                            : &query_context->getScalarGroup(
                                  proto.GetExtension(serialization::SelectWorkOrder::selection_index)),
          query_context->getInsertDestination(
              proto.GetExtension(serialization::SelectWorkOrder::insert_destination_index)),
          storage_manager);
    }
    case serialization::SORT_MERGE_RUN: {
      LOG(INFO) << "Creating SortMergeRunWorkOrder";
      vector<merge_run_operator::Run> runs;
      for (int i = 0; i < proto.ExtensionSize(serialization::SortMergeRunWorkOrder::runs); ++i) {
        merge_run_operator::Run run;
        const serialization::Run &run_proto =
            proto.GetExtension(serialization::SortMergeRunWorkOrder::runs, i);
        for (int j = 0; j < run_proto.blocks_size(); ++j) {
          run.push_back(run_proto.blocks(j));
        }
        runs.push_back(move(run));
      }

      return new SortMergeRunWorkOrder(
          proto.query_id(),
          query_context->getSortConfig(
              proto.GetExtension(serialization::SortMergeRunWorkOrder::sort_config_index)),
          catalog_database->getRelationSchemaById(
              proto.GetExtension(serialization::SortMergeRunWorkOrder::relation_id)),
          move(runs),
          proto.GetExtension(serialization::SortMergeRunWorkOrder::top_k),
          proto.GetExtension(serialization::SortMergeRunWorkOrder::merge_level),
          query_context->getInsertDestination(
              proto.GetExtension(serialization::SortMergeRunWorkOrder::insert_destination_index)),
          storage_manager,
          proto.GetExtension(serialization::SortMergeRunWorkOrder::operator_index),
          shiftboss_client_id,
          bus);
    }
    case serialization::SORT_RUN_GENERATION: {
      LOG(INFO) << "Creating SortRunGenerationWorkOrder";
      return new SortRunGenerationWorkOrder(
          proto.query_id(),
          catalog_database->getRelationSchemaById(
              proto.GetExtension(serialization::SortRunGenerationWorkOrder::relation_id)),
          proto.GetExtension(serialization::SortRunGenerationWorkOrder::block_id),
          query_context->getSortConfig(
              proto.GetExtension(serialization::SortRunGenerationWorkOrder::sort_config_index)),
          query_context->getInsertDestination(
              proto.GetExtension(serialization::SortRunGenerationWorkOrder::insert_destination_index)),
          storage_manager);
    }
    case serialization::TABLE_GENERATOR: {
      LOG(INFO) << "Creating SortRunGenerationWorkOrder";
      return new TableGeneratorWorkOrder(
          proto.query_id(),
          query_context->getGeneratorFunctionHandle(
              proto.GetExtension(serialization::TableGeneratorWorkOrder::generator_function_index)),
          query_context->getInsertDestination(
              proto.GetExtension(serialization::TableGeneratorWorkOrder::insert_destination_index)));
    }
    case serialization::TEXT_SCAN: {
      LOG(INFO) << "Creating TextScanWorkOrder";
      return new TextScanWorkOrder(
          proto.query_id(),
          proto.GetExtension(serialization::TextScanWorkOrder::filename),
          proto.GetExtension(serialization::TextScanWorkOrder::text_offset),
          proto.GetExtension(serialization::TextScanWorkOrder::text_segment_size),
          proto.GetExtension(serialization::TextScanWorkOrder::field_terminator),
          proto.GetExtension(serialization::TextScanWorkOrder::process_escape_sequences),
          query_context->getInsertDestination(
              proto.GetExtension(serialization::TextScanWorkOrder::insert_destination_index)));
    }
    case serialization::UPDATE: {
      LOG(INFO) << "Creating UpdateWorkOrder";
      return new UpdateWorkOrder(
          proto.query_id(),
          catalog_database->getRelationSchemaById(
              proto.GetExtension(serialization::UpdateWorkOrder::relation_id)),
          proto.GetExtension(serialization::UpdateWorkOrder::block_id),
          query_context->getPredicate(
              proto.GetExtension(serialization::UpdateWorkOrder::predicate_index)),
          query_context->getUpdateGroup(
              proto.GetExtension(serialization::UpdateWorkOrder::update_group_index)),
          query_context->getInsertDestination(
              proto.GetExtension(serialization::UpdateWorkOrder::insert_destination_index)),
          storage_manager,
          proto.GetExtension(serialization::UpdateWorkOrder::operator_index),
          shiftboss_client_id,
          bus);
    }
    case serialization::WINDOW_AGGREGATION: {
      LOG(INFO) << "Creating WindowAggregationWorkOrder";
      vector<block_id> blocks;
      for (int i = 0; i < proto.ExtensionSize(serialization::WindowAggregationWorkOrder::block_ids); ++i) {
        blocks.push_back(
            proto.GetExtension(serialization::WindowAggregationWorkOrder::block_ids, i));
      }

      return new WindowAggregationWorkOrder(
          proto.query_id(),
          query_context->getWindowAggregationState(
              proto.GetExtension(serialization::WindowAggregationWorkOrder::window_aggr_state_index)),
          move(blocks),
          query_context->getInsertDestination(
              proto.GetExtension(serialization::FinalizeAggregationWorkOrder::insert_destination_index)));
    }
    default:
      LOG(FATAL) << "Unknown WorkOrder Type in WorkOrderFactory::ReconstructFromProto";
  }
}

bool WorkOrderFactory::ProtoIsValid(const serialization::WorkOrder &proto,
                                    const CatalogDatabaseLite &catalog_database,
                                    const QueryContext &query_context) {
  if (!proto.IsInitialized()) {
    return false;
  }

  switch (proto.work_order_type()) {
    case serialization::AGGREGATION: {
      return proto.HasExtension(serialization::AggregationWorkOrder::block_id) &&
             proto.HasExtension(serialization::AggregationWorkOrder::aggr_state_index) &&
             query_context.isValidAggregationStateId(
                 proto.GetExtension(serialization::AggregationWorkOrder::aggr_state_index));
    }
    case serialization::BUILD_HASH: {
      if (!proto.HasExtension(serialization::BuildHashWorkOrder::relation_id)) {
        return false;
      }

      const relation_id rel_id = proto.GetExtension(serialization::BuildHashWorkOrder::relation_id);
      if (!catalog_database.hasRelationWithId(rel_id)) {
        return false;
      }

      const CatalogRelationSchema &relation = catalog_database.getRelationSchemaById(rel_id);
      for (int i = 0; i < proto.ExtensionSize(serialization::BuildHashWorkOrder::join_key_attributes); ++i) {
        if (!relation.hasAttributeWithId(
                proto.GetExtension(serialization::BuildHashWorkOrder::join_key_attributes, i))) {
          return false;
        }
      }

      return proto.HasExtension(serialization::BuildHashWorkOrder::any_join_key_attributes_nullable) &&
             proto.HasExtension(serialization::BuildHashWorkOrder::block_id) &&
             proto.HasExtension(serialization::BuildHashWorkOrder::join_hash_table_index) &&
             query_context.isValidJoinHashTableId(
                 proto.GetExtension(serialization::BuildHashWorkOrder::join_hash_table_index));
    }
    case serialization::DELETE: {
      return proto.HasExtension(serialization::DeleteWorkOrder::relation_id) &&
             catalog_database.hasRelationWithId(
                 proto.GetExtension(serialization::DeleteWorkOrder::relation_id)) &&
             proto.HasExtension(serialization::DeleteWorkOrder::predicate_index) &&
             query_context.isValidPredicate(
                 proto.GetExtension(serialization::DeleteWorkOrder::predicate_index)) &&
             proto.HasExtension(serialization::DeleteWorkOrder::block_id) &&
             proto.HasExtension(serialization::DeleteWorkOrder::operator_index);
    }
    case serialization::DESTROY_HASH: {
      return proto.HasExtension(serialization::DestroyHashWorkOrder::join_hash_table_index) &&
             query_context.isValidJoinHashTableId(
                 proto.GetExtension(serialization::DestroyHashWorkOrder::join_hash_table_index));
    }
    case serialization::DROP_TABLE: {
      return true;
    }
    case serialization::FINALIZE_AGGREGATION: {
      return proto.HasExtension(serialization::FinalizeAggregationWorkOrder::aggr_state_index) &&
             query_context.isValidAggregationStateId(
                 proto.GetExtension(serialization::FinalizeAggregationWorkOrder::aggr_state_index)) &&
             proto.HasExtension(serialization::FinalizeAggregationWorkOrder::insert_destination_index) &&
             query_context.isValidInsertDestinationId(
                 proto.GetExtension(serialization::FinalizeAggregationWorkOrder::insert_destination_index));
    }
    case serialization::HASH_JOIN: {
      if (!proto.HasExtension(serialization::HashJoinWorkOrder::hash_join_work_order_type)) {
        return false;
      }

      const auto hash_join_work_order_type =
          proto.GetExtension(serialization::HashJoinWorkOrder::hash_join_work_order_type);
      if (!serialization::HashJoinWorkOrder_HashJoinWorkOrderType_IsValid(hash_join_work_order_type)) {
        return false;
      }

      if (!proto.HasExtension(serialization::HashJoinWorkOrder::build_relation_id) ||
          !proto.HasExtension(serialization::HashJoinWorkOrder::probe_relation_id)) {
        return false;
      }

      const relation_id build_relation_id =
          proto.GetExtension(serialization::HashJoinWorkOrder::build_relation_id);
      if (!catalog_database.hasRelationWithId(build_relation_id)) {
        return false;
      }

      const relation_id probe_relation_id =
          proto.GetExtension(serialization::HashJoinWorkOrder::probe_relation_id);
      if (!catalog_database.hasRelationWithId(probe_relation_id)) {
        return false;
      }

      const CatalogRelationSchema &probe_relation = catalog_database.getRelationSchemaById(probe_relation_id);
      for (int i = 0; i < proto.ExtensionSize(serialization::HashJoinWorkOrder::join_key_attributes); ++i) {
        const attribute_id attr_id =
            proto.GetExtension(serialization::HashJoinWorkOrder::join_key_attributes, i);
        if (!probe_relation.hasAttributeWithId(attr_id)) {
          return false;
        }
      }

      if (hash_join_work_order_type == serialization::HashJoinWorkOrder::HASH_OUTER_JOIN) {
        if (!proto.HasExtension(serialization::HashJoinWorkOrder::is_selection_on_build)) {
          return false;
        }
      } else {
        if (!proto.HasExtension(serialization::HashJoinWorkOrder::residual_predicate_index) ||
            !query_context.isValidPredicate(
                 proto.GetExtension(serialization::HashJoinWorkOrder::residual_predicate_index))) {
          return false;
        }
      }

      return proto.HasExtension(serialization::HashJoinWorkOrder::any_join_key_attributes_nullable) &&
             proto.HasExtension(serialization::HashJoinWorkOrder::insert_destination_index) &&
             query_context.isValidInsertDestinationId(
                 proto.GetExtension(serialization::HashJoinWorkOrder::insert_destination_index)) &&
             proto.HasExtension(serialization::HashJoinWorkOrder::join_hash_table_index) &&
             query_context.isValidJoinHashTableId(
                 proto.GetExtension(serialization::HashJoinWorkOrder::join_hash_table_index)) &&
             proto.HasExtension(serialization::HashJoinWorkOrder::selection_index) &&
             query_context.isValidScalarGroupId(
                 proto.GetExtension(serialization::HashJoinWorkOrder::selection_index)) &&
             proto.HasExtension(serialization::HashJoinWorkOrder::block_id);
    }
    case serialization::INSERT: {
      return proto.HasExtension(serialization::InsertWorkOrder::insert_destination_index) &&
             query_context.isValidInsertDestinationId(
                 proto.GetExtension(serialization::InsertWorkOrder::insert_destination_index)) &&
             proto.HasExtension(serialization::InsertWorkOrder::tuple_index) &&
             query_context.isValidTupleId(
                 proto.GetExtension(serialization::InsertWorkOrder::tuple_index));
    }
    case serialization::NESTED_LOOP_JOIN: {
      if (!proto.HasExtension(serialization::NestedLoopsJoinWorkOrder::left_relation_id) ||
          !proto.HasExtension(serialization::NestedLoopsJoinWorkOrder::right_relation_id)) {
        return false;
      }

      const relation_id left_relation_id =
          proto.GetExtension(serialization::NestedLoopsJoinWorkOrder::left_relation_id);
      if (!catalog_database.hasRelationWithId(left_relation_id)) {
        return false;
      }

      const relation_id right_relation_id =
          proto.GetExtension(serialization::NestedLoopsJoinWorkOrder::right_relation_id);
      if (!catalog_database.hasRelationWithId(right_relation_id)) {
        return false;
      }

      return proto.HasExtension(serialization::NestedLoopsJoinWorkOrder::left_block_id) &&
             proto.HasExtension(serialization::NestedLoopsJoinWorkOrder::right_block_id) &&
             proto.HasExtension(serialization::NestedLoopsJoinWorkOrder::insert_destination_index) &&
             query_context.isValidInsertDestinationId(
                 proto.GetExtension(serialization::NestedLoopsJoinWorkOrder::insert_destination_index)) &&
             proto.HasExtension(serialization::NestedLoopsJoinWorkOrder::join_predicate_index) &&
             query_context.isValidPredicate(
                 proto.GetExtension(serialization::NestedLoopsJoinWorkOrder::join_predicate_index)) &&
             proto.HasExtension(serialization::NestedLoopsJoinWorkOrder::selection_index) &&
             query_context.isValidScalarGroupId(
                 proto.GetExtension(serialization::NestedLoopsJoinWorkOrder::selection_index));
    }
    case serialization::SAMPLE: {
      return catalog_database.hasRelationWithId(proto.GetExtension(serialization::SampleWorkOrder::relation_id)) &&
             proto.HasExtension(serialization::SampleWorkOrder::block_id) &&
             proto.HasExtension(serialization::SampleWorkOrder::is_block_sample) &&
             proto.HasExtension(serialization::SampleWorkOrder::percentage) &&
             proto.HasExtension(serialization::SampleWorkOrder::insert_destination_index);
    }
    case serialization::SAVE_BLOCKS: {
      return proto.HasExtension(serialization::SaveBlocksWorkOrder::block_id) &&
             proto.HasExtension(serialization::SaveBlocksWorkOrder::force);
    }
    case serialization::SELECT: {
      if (!proto.HasExtension(serialization::SelectWorkOrder::relation_id) ||
          !proto.HasExtension(serialization::SelectWorkOrder::simple_projection) ||
          !proto.HasExtension(serialization::SelectWorkOrder::selection_index)) {
        return false;
      }

      const relation_id rel_id = proto.GetExtension(serialization::SelectWorkOrder::relation_id);
      if (!catalog_database.hasRelationWithId(rel_id)) {
        return false;
      }

      const CatalogRelationSchema &relation = catalog_database.getRelationSchemaById(rel_id);
      for (int i = 0; i < proto.ExtensionSize(serialization::SelectWorkOrder::simple_selection); ++i) {
        if (!relation.hasAttributeWithId(
                 proto.GetExtension(serialization::SelectWorkOrder::simple_selection, i))) {
          return false;
        }
      }

      if (proto.GetExtension(serialization::SelectWorkOrder::simple_projection) ==
              query_context.isValidScalarGroupId(
                  proto.GetExtension(serialization::SelectWorkOrder::selection_index))) {
        return false;
      }

      return proto.HasExtension(serialization::SelectWorkOrder::insert_destination_index) &&
             query_context.isValidInsertDestinationId(
                 proto.GetExtension(serialization::SelectWorkOrder::insert_destination_index)) &&
             proto.HasExtension(serialization::SelectWorkOrder::predicate_index) &&
             query_context.isValidPredicate(
                 proto.GetExtension(serialization::SelectWorkOrder::predicate_index)) &&
             proto.HasExtension(serialization::SelectWorkOrder::block_id);
    }
    case serialization::SORT_MERGE_RUN: {
      // In Protobuf 2.6, proto.HasExtension does not work for the repeated
      // message field, but Protobuf 3.0 beta works.
      // TODO(zuyu): Validate serialization::SortMergeRunWorkOrder::runs.
      return proto.HasExtension(serialization::SortMergeRunWorkOrder::sort_config_index) &&
             query_context.isValidSortConfigId(
                 proto.GetExtension(serialization::SortMergeRunWorkOrder::sort_config_index)) &&
             proto.HasExtension(serialization::SortMergeRunWorkOrder::top_k) &&
             proto.HasExtension(serialization::SortMergeRunWorkOrder::merge_level) &&
             proto.HasExtension(serialization::SortMergeRunWorkOrder::relation_id) &&
             catalog_database.hasRelationWithId(
                 proto.GetExtension(serialization::SortMergeRunWorkOrder::relation_id)) &&
             proto.HasExtension(serialization::SortMergeRunWorkOrder::insert_destination_index) &&
             query_context.isValidInsertDestinationId(
                 proto.GetExtension(serialization::SortMergeRunWorkOrder::insert_destination_index)) &&
             proto.HasExtension(serialization::SortMergeRunWorkOrder::operator_index);
    }
    case serialization::SORT_RUN_GENERATION: {
      return proto.HasExtension(serialization::SortRunGenerationWorkOrder::relation_id) &&
             catalog_database.hasRelationWithId(
                 proto.GetExtension(serialization::SortRunGenerationWorkOrder::relation_id)) &&
             proto.HasExtension(serialization::SortRunGenerationWorkOrder::insert_destination_index) &&
             query_context.isValidInsertDestinationId(
                 proto.GetExtension(serialization::SortRunGenerationWorkOrder::insert_destination_index)) &&
             proto.HasExtension(serialization::SortRunGenerationWorkOrder::sort_config_index) &&
             query_context.isValidSortConfigId(
                 proto.GetExtension(serialization::SortRunGenerationWorkOrder::sort_config_index)) &&
             proto.HasExtension(serialization::SortRunGenerationWorkOrder::block_id);
    }
    case serialization::TABLE_GENERATOR: {
      return proto.HasExtension(serialization::TableGeneratorWorkOrder::generator_function_index) &&
             query_context.isValidGeneratorFunctionId(
                 proto.GetExtension(serialization::TableGeneratorWorkOrder::generator_function_index)) &&
             proto.HasExtension(serialization::TableGeneratorWorkOrder::insert_destination_index) &&
             query_context.isValidInsertDestinationId(
                 proto.GetExtension(serialization::TableGeneratorWorkOrder::insert_destination_index));
    }
    case serialization::TEXT_SCAN: {
      return proto.HasExtension(serialization::TextScanWorkOrder::filename) &&
             proto.HasExtension(serialization::TextScanWorkOrder::text_offset) &&
             proto.HasExtension(serialization::TextScanWorkOrder::text_segment_size) &&
             proto.HasExtension(serialization::TextScanWorkOrder::field_terminator) &&
             proto.HasExtension(serialization::TextScanWorkOrder::process_escape_sequences) &&
             proto.HasExtension(serialization::TextScanWorkOrder::insert_destination_index) &&
             query_context.isValidInsertDestinationId(
                 proto.GetExtension(serialization::TextScanWorkOrder::insert_destination_index));
    }
    case serialization::UPDATE: {
      return proto.HasExtension(serialization::UpdateWorkOrder::relation_id) &&
             catalog_database.hasRelationWithId(
                 proto.GetExtension(serialization::UpdateWorkOrder::relation_id)) &&
             proto.HasExtension(serialization::UpdateWorkOrder::insert_destination_index) &&
             query_context.isValidInsertDestinationId(
                 proto.GetExtension(serialization::UpdateWorkOrder::insert_destination_index)) &&
             proto.HasExtension(serialization::UpdateWorkOrder::predicate_index) &&
             query_context.isValidPredicate(
                 proto.GetExtension(serialization::UpdateWorkOrder::predicate_index)) &&
             proto.HasExtension(serialization::UpdateWorkOrder::update_group_index) &&
             query_context.isValidUpdateGroupId(
                 proto.GetExtension(serialization::UpdateWorkOrder::update_group_index)) &&
             proto.HasExtension(serialization::UpdateWorkOrder::operator_index) &&
             proto.HasExtension(serialization::UpdateWorkOrder::block_id);
    }
    case serialization::WINDOW_AGGREGATION: {
      return proto.HasExtension(serialization::WindowAggregationWorkOrder::window_aggr_state_index) &&
             query_context.isValidWindowAggregationStateId(
                 proto.GetExtension(serialization::WindowAggregationWorkOrder::window_aggr_state_index)) &&
             proto.HasExtension(serialization::WindowAggregationWorkOrder::insert_destination_index) &&
             query_context.isValidInsertDestinationId(
                 proto.GetExtension(serialization::WindowAggregationWorkOrder::insert_destination_index));
    }
    default:
      return false;
  }
}

}  // namespace quickstep
