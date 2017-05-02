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

#include "query_optimizer/ExecutionSerializer.hpp"

#include <cstddef>

#include "query_optimizer/QueryPlan.hpp"
#include "relational_operators/AggregationOperator.hpp"
#include "relational_operators/BuildAggregationExistenceMapOperator.hpp"
#include "relational_operators/BuildHashOperator.hpp"
#include "relational_operators/BuildLIPFilterOperator.hpp"
#include "relational_operators/CreateIndexOperator.hpp"
#include "relational_operators/CreateTableOperator.hpp"
#include "relational_operators/DeleteOperator.hpp"
#include "relational_operators/DestroyAggregationStateOperator.hpp"
#include "relational_operators/DestroyHashOperator.hpp"
#include "relational_operators/DropTableOperator.hpp"
#include "relational_operators/FinalizeAggregationOperator.hpp"
#include "relational_operators/HashJoinOperator.hpp"
#include "relational_operators/InitializeAggregationOperator.hpp"
#include "relational_operators/InsertOperator.hpp"
#include "relational_operators/NestedLoopsJoinOperator.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/SampleOperator.hpp"
#include "relational_operators/SaveBlocksOperator.hpp"
#include "relational_operators/SelectOperator.hpp"
#include "relational_operators/SortMergeRunOperator.hpp"
#include "relational_operators/SortRunGenerationOperator.hpp"
#include "relational_operators/TableGeneratorOperator.hpp"
#include "relational_operators/TextScanOperator.hpp"
#include "relational_operators/UpdateOperator.hpp"
#include "relational_operators/WindowAggregationOperator.hpp"
#include "query_optimizer/QueryPlan.pb.h"
#include "relational_operators/Operator.pb.h"

namespace quickstep {
namespace optimizer {

namespace S = ::quickstep::serialization;

void ExecutionSerializer::serializePlan(const QueryPlan &query_plan) {
  // TODO(hakan): Fix the problem with const DAG.
  const auto &dag = query_plan.getQueryPlanDAG();
  for (std::size_t i = 0; i < dag.size(); ++i) {
    const auto &edges = dag.getDependencies(i);
    S::OutgoingEdges *outgoing_edges = query_plan_proto_.add_outgoing();
    for (const auto &edge : edges) {
      S::Edge *edge_proto = outgoing_edges->add_edges();
      edge_proto->set_outgoing_id(edge);
      edge_proto->set_payload(dag.getLinkMetadata(i, edge));
    }
    const auto &relational_operator = dag.getNodePayload(i);
    serializeInternal(relational_operator);
  }
}

void ExecutionSerializer::serializeInternal(const RelationalOperator &relational_operator) {
  switch (relational_operator.getOperatorType()) {
  case RelationalOperator::OperatorType::kAggregation:
    return serializeAggregation(
        static_cast<const AggregationOperator&>(relational_operator));
  case RelationalOperator::kBuildAggregationExistenceMap:
    return serializeBuildAggregationExistenceMap(
        static_cast<const BuildAggregationExistenceMapOperator&>(relational_operator));
  case RelationalOperator::kBuildHash:
    return serializeBuildHash(
        static_cast<const BuildHashOperator&>(relational_operator));
  case RelationalOperator::kBuildLIPFilter:
    return serializeBuildLIPFilter(
        static_cast<const BuildLIPFilterOperator&>(relational_operator));
  case RelationalOperator::kCreateIndex:
    return serializeCreateIndex(
        static_cast<const CreateIndexOperator&>(relational_operator));
  case RelationalOperator::kCreateTable:
    return serializeCreateTable(
        static_cast<const CreateTableOperator&>(relational_operator));
  case RelationalOperator::kDelete:
    return serializeDelete(
        static_cast<const DeleteOperator&>(relational_operator));
  case RelationalOperator::kDestroyAggregationState:
    return serializeDestroyAggregationState(
        static_cast<const DestroyAggregationStateOperator&>(relational_operator));
  case RelationalOperator::kDestroyHash:
    return serializeDestroyHash(
        static_cast<const DestroyHashOperator&>(relational_operator));
  case RelationalOperator::kDropTable:
    return serializeDropTable(
        static_cast<const DropTableOperator&>(relational_operator));
  case RelationalOperator::kFinalizeAggregation:
    return serializeFinalizeAggregation(
        static_cast<const FinalizeAggregationOperator&>(relational_operator));
  case RelationalOperator::kInitializeAggregation:
    return serializeInitializeAggregation(
        static_cast<const InitializeAggregationOperator&>(relational_operator));
  case RelationalOperator::kInsert:
    return serializeInsert(
        static_cast<const InsertOperator&>(relational_operator));
  case RelationalOperator::kInnerJoin:
  case RelationalOperator::kLeftAntiJoin:
  case RelationalOperator::kLeftOuterJoin:
  case RelationalOperator::kLeftSemiJoin:
    return serializeHashJoin(
        static_cast<const HashJoinOperator&>(relational_operator));
  case RelationalOperator::kNestedLoopsJoin:
    return serializeNestedLoopsJoin(
        static_cast<const NestedLoopsJoinOperator&>(relational_operator));
  case RelationalOperator::kSample:
    return serializeSample(
        static_cast<const SampleOperator&>(relational_operator));
  case RelationalOperator::kSaveBlocks:
    return serializeSaveBlocks(
        static_cast<const SaveBlocksOperator&>(relational_operator));
  case RelationalOperator::kSelect:
    return serializeSelect(
        static_cast<const SelectOperator&>(relational_operator));
  case RelationalOperator::kSortMergeRun:
    return serializeSortMergeRun(
        static_cast<const SortMergeRunOperator&>(relational_operator));
  case RelationalOperator::kSortRunGeneration:
    return serializeSortRunGeneration(
        static_cast<const SortRunGenerationOperator&>(relational_operator));
  case RelationalOperator::kTableGenerator:
    return serializeTableGenerator(
        static_cast<const TableGeneratorOperator&>(relational_operator));
  case RelationalOperator::kTextScan:
    return serializeTextScan(
        static_cast<const TextScanOperator&>(relational_operator));
  case RelationalOperator::kUpdate:
    return serializeUpdate(
        static_cast<const UpdateOperator&>(relational_operator));
  case RelationalOperator::kWindowAggregation:
    return serializeWindowAggregation(
        static_cast<const WindowAggregationOperator&>(relational_operator));
  case RelationalOperator::kMockOperator:
    break;
  case RelationalOperator::kUnionAll:break;
  }
}

void ExecutionSerializer::serializeAggregation(const AggregationOperator &aggregation_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_AGGREGATION);

  serialization::AggregationOperator *aggregation = vertex->mutable_aggregation_operator();
  aggregation->set_relation_id(aggregation_operator.input_relation().getID());
  aggregation->set_relation_is_stored(!aggregation_operator.input_relation().isTemporary());
  aggregation->set_aggr_state_index(aggregation_operator.getAggregationStateIndex());
}

void ExecutionSerializer::serializeBuildAggregationExistenceMap(const BuildAggregationExistenceMapOperator &op) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_BUILD_AGGREGATION_EXISTENCE_MAP);

  serialization::BuildAggregationExistenceMapOperator *build_aggregation_existence_map_work_order = vertex->mutable_build_aggregation_existence_map_operator();
  build_aggregation_existence_map_work_order->set_relation_id(op.input_relation().getID());
  build_aggregation_existence_map_work_order->set_attribute_id(op.getBuildAttributeID());
  build_aggregation_existence_map_work_order->set_relation_is_stored(!op.input_relation().isTemporary());
  build_aggregation_existence_map_work_order->set_aggr_state_index(op.getAggregationStateIndex());
}

void ExecutionSerializer::serializeBuildHash(const BuildHashOperator &build_hash_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_BUILD_HASH);

  serialization::BuildHashOperator *build_hash = vertex->mutable_build_hash_operator();
  build_hash->set_relation_id(build_hash_operator.input_relation().getID());
  build_hash->set_relation_is_stored(!build_hash_operator.input_relation().isTemporary());
  for (const auto &join_key : build_hash_operator.getJoinKeyAttributes()) {
    build_hash->add_join_key_attributes(join_key);
  }
  build_hash->set_any_join_key_nullable(build_hash_operator.anyJoinKeyNullable());
  build_hash->set_num_partitions(build_hash_operator.getNumberOfPartitions());
  build_hash->set_hash_table_index(build_hash_operator.getHashTableIndex());
}

void ExecutionSerializer::serializeBuildLIPFilter(
    const BuildLIPFilterOperator &build_lip_filter_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_BUILD_LIP_FILTER);

  serialization::BuildLIPFilterOperator *build_lip_filter = vertex->mutable_build_lip_filter_operator();
  build_lip_filter->set_relation_id(build_lip_filter_operator.input_relation().getID());
  build_lip_filter->set_relation_is_stored(!build_lip_filter_operator.input_relation().isTemporary());
  build_lip_filter->set_build_side_predicate_index(build_lip_filter_operator.getPredicateIndex());
}

void ExecutionSerializer::serializeCreateIndex(
    const CreateIndexOperator &create_index_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_CREATE_INDEX);

  serialization::CreateIndexOperator *create_index = vertex->mutable_create_index_operator();
  create_index->set_relation_id(create_index_operator.getRelation().getID());
  IndexSubBlockDescription *index_description = create_index->mutable_index_description();
  index_description->CopyFrom(create_index_operator.getIndexDescription());
  create_index->set_index_name(create_index_operator.getIndexName());
}

void ExecutionSerializer::serializeCreateTable(const CreateTableOperator &create_table_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_CREATE_TABLE);

  serialization::CreateTableOperator *create_table = vertex->mutable_create_table_operator();
  // TODO(hakan): Add database id serialization.
  serialization::CatalogRelationSchema *catalog_relation = create_table->mutable_relation_catalog();
  catalog_relation->CopyFrom(create_table_operator.getRelation().getProto());
}

void ExecutionSerializer::serializeDelete(const DeleteOperator &delete_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_DELETE);

  serialization::DeleteOperator *delete_op = vertex->mutable_delete_operator();
  delete_op->set_relation_id(delete_operator.getOutputRelationID());
  delete_op->set_predicate_index(delete_operator.getPredicateIndex());
  delete_op->set_relation_is_stored(!delete_operator.getRelation().isTemporary());

}
void ExecutionSerializer::serializeDestroyAggregationState(
    const DestroyAggregationStateOperator &destroy_aggregation_state_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_DESTROY_AGGREGATION_STATE);

  serialization::DestroyAggregationStateOperator *destroy_aggregation
      = vertex->mutable_destroy_aggregation_state_operator();
  destroy_aggregation->set_aggr_state_index(
      destroy_aggregation_state_operator.getAggregationStateIndex());
}
void ExecutionSerializer::serializeDestroyHash(const DestroyHashOperator &destroy_hash_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_DESTROY_HASH);

  serialization::DestroyHashOperator *destroy_hash = vertex->mutable_destroy_hash_operator();
  destroy_hash->set_hash_table_index(destroy_hash_operator.getHashTableIndex());
  destroy_hash->set_build_num_partitions(destroy_hash_operator.getNumPartitions());
}
void ExecutionSerializer::serializeDropTable(const DropTableOperator &drop_table_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_DROP_TABLE);

  serialization::DropTableOperator *drop_table = vertex->mutable_drop_table_operator();
  // TODO(hakan): Add database id serialization.
  drop_table->set_relation_id(drop_table_operator.getRelationID());
  drop_table->set_only_drop_blocks(drop_table_operator.onlyDropBlocks());
}

void ExecutionSerializer::serializeFinalizeAggregation(
    const FinalizeAggregationOperator &finalize_aggregation_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_FINALIZE_AGGREGATION);

  serialization::FinalizeAggregationOperator *finalize_aggregation
      = vertex->mutable_finalize_aggregation_operator();
  finalize_aggregation->set_aggr_state_index(finalize_aggregation_operator.getAggregationStateIndex());
  finalize_aggregation->set_output_relation_id(finalize_aggregation_operator.getOutputRelationID());
  finalize_aggregation->set_output_destination_index(finalize_aggregation_operator.getInsertDestinationID());
}

void ExecutionSerializer::serializeHashJoin(const HashJoinOperator &finalize_hash_join) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_HASH_JOIN);

  serialization::HashJoinOperator *hash_join = vertex->mutable_hash_join_operator();
  hash_join->set_build_relation_id(finalize_hash_join.build_relation().getID());
  hash_join->set_probe_relation_id(finalize_hash_join.probe_relation().getID());
  for (const auto &join_key_attr : finalize_hash_join.getJoinKeyAttributes()) {
    hash_join->add_join_key_attributes(join_key_attr);
  }
  hash_join->set_any_join_key_attributes_nullable(finalize_hash_join.anyJoinKeyNullable());
  hash_join->set_build_num_partitions(finalize_hash_join.getBuildNumPartitions());
  hash_join->set_output_relation_id(finalize_hash_join.getOutputRelationID());
  hash_join->set_output_destionation_index(finalize_hash_join.getInsertDestinationID());
  hash_join->set_hash_table_index(finalize_hash_join.getJoinHashTableIndex());
  hash_join->set_residiual_predicate_index(finalize_hash_join.getResidualPredicateIndex());
  hash_join->set_selection_index(finalize_hash_join.getSelectionIndex());
  for (const auto &is_selection_on_build : finalize_hash_join.getSelectionsOnBuild()) {
    hash_join->add_is_selection_on_build(is_selection_on_build);
  }

  switch (finalize_hash_join.getOperatorType()) {
  case RelationalOperator::kInnerJoin: {
    hash_join->set_join_type(S::HashJoinOperator_JoinType_LEFT_INNER);
    break;
  }
  case RelationalOperator::kLeftSemiJoin: {
    hash_join->set_join_type(S::HashJoinOperator_JoinType_LEFT_SEMI);
    break;
  }
  case RelationalOperator::kLeftAntiJoin: {
    hash_join->set_join_type(S::HashJoinOperator_JoinType_LEFT_ANTI);
  }
  case RelationalOperator::kLeftOuterJoin: {
    hash_join->set_join_type(S::HashJoinOperator_JoinType_LEFT_OUTER);
    break;
  }
  default: {
    // TODO(hakan): Write exception log message.
  }
  }
}
void ExecutionSerializer::serializeInitializeAggregation(
    const InitializeAggregationOperator &initialize_aggregation_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_INITIALIZE_AGGREGATION);

  serialization::InitializeAggregationOperator *initialize_aggregation
      = vertex->mutable_initialize_aggregation_operator();
  initialize_aggregation->set_aggr_state_index(
      initialize_aggregation_operator.getAggregationStateIndex());
}

void ExecutionSerializer::serializeInsert(const InsertOperator &insert_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_INSERT);

  serialization::InsertOperator *insert = vertex->mutable_insert_operator();
  insert->set_output_relation_id(insert_operator.getOutputRelationID());
  insert->set_output_destination_index(insert_operator.getInsertDestinationID());
  insert->set_tuple_index(insert_operator.getTupleIndex());
}

void ExecutionSerializer::serializeNestedLoopsJoin(const NestedLoopsJoinOperator &nested_loops_join_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_NESTED_LOOP_JOIN);

  serialization::NestedLoopsJoinOperator *nested_loops_join = vertex->mutable_nested_loops_join_operator();
  nested_loops_join->set_left_input_relation_id(nested_loops_join_operator.getLeftInputRelation().getID());
  nested_loops_join->set_right_input_relation_id(nested_loops_join_operator.getRightInputRelation().getID());
  nested_loops_join->set_output_relation_id(nested_loops_join_operator.getOutputRelationID());
  nested_loops_join->set_output_destination_index(nested_loops_join_operator.getInsertDestinationID());
  nested_loops_join->set_join_predicate_index(nested_loops_join_operator.getJoinPredicateIndex());
  nested_loops_join->set_selection_index(nested_loops_join_operator.getSelectionIndex());
  nested_loops_join->set_left_relation_is_stored(!nested_loops_join_operator.getLeftInputRelation().isTemporary());
  nested_loops_join->set_right_relation_is_stored(!nested_loops_join_operator.getRightInputRelation().isTemporary());
}

void ExecutionSerializer::serializeSample(const SampleOperator &sample_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_SAMPLE);

  serialization::SampleOperator *sample = vertex->mutable_sample_operator();
  sample->set_input_relation_id(sample_operator.getIntputRelation().getID());
  sample->set_output_relation_id(sample_operator.getOutputRelationID());
  sample->set_output_destination_index(sample_operator.getInsertDestinationID());
  sample->set_input_relation_is_stored(!sample_operator.getIntputRelation().isTemporary());
  sample->set_is_block_sample(sample_operator.isBlockSample());
  sample->set_percentage(sample_operator.getPercentage());
}

void ExecutionSerializer::serializeSaveBlocks(const SaveBlocksOperator &save_blocks_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_SAVE_BLOCKS);

  serialization::SaveBlocksOperator *save_blocks = vertex->mutable_save_blocks_operator();
  serialization::CatalogRelation *relation_catalog = save_blocks->mutable_relation();
  relation_catalog->CopyFrom(save_blocks_operator.getRelation().getProto());
  save_blocks->set_force(save_blocks_operator.isForced());
}

void ExecutionSerializer::serializeSelect(const SelectOperator &select_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_SELECT);

  serialization::SelectOperator *select = vertex->mutable_select_operator();
  select->set_input_relation_id(select_operator.input_relation().getID());
  select->set_output_relation_id(select_operator.getOutputRelationID());
  select->set_output_destination_index(select_operator.getInsertDestinationID());
  select->set_predicate_index(select_operator.getPredicateIndex());
  select->set_input_relation_is_stored(!select_operator.input_relation().isTemporary());
  select->set_num_partitions(select_operator.getNumPartitions());
  for (const auto &attribute : select_operator.getSelectionAttributes()) {
    select->add_selection_attributes(attribute);
  }
}

void ExecutionSerializer::serializeSortMergeRun(const SortMergeRunOperator &sort_merge_run_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_SORT_MERGE_RUN);

  serialization::SortMergeRunOperator *sort_merge_run = vertex->mutable_sort_merge_run_operator();
  sort_merge_run->set_input_relation_id(sort_merge_run_operator.getInputRelation().getID());
  sort_merge_run->set_output_relation_id(sort_merge_run_operator.getOutputRelationID());
  sort_merge_run->set_output_destination_index(sort_merge_run_operator.getInsertDestinationID());
  sort_merge_run->set_run_relation_id(sort_merge_run_operator.getRunRelation().getID());
  sort_merge_run->set_run_block_destination_index(sort_merge_run_operator.getRunBlockDestinationIndex());
  sort_merge_run->set_sort_config_index(sort_merge_run_operator.getSortConfigIndex());
  sort_merge_run->set_merge_factor(sort_merge_run_operator.getMergeFactor());
  sort_merge_run->set_top_k(sort_merge_run_operator.getTopK());
  sort_merge_run->set_is_input_stored(!sort_merge_run_operator.getInputRelation().isTemporary());
}

void ExecutionSerializer::serializeSortRunGeneration(const SortRunGenerationOperator &sort_run_generation_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_SORT_RUN_GENERATION);

  serialization::SortRunGenerationOperator *sort_run_generation = vertex->mutable_sort_run_generation_operator();
  sort_run_generation->set_input_relation_id(sort_run_generation_operator.getInputRelation().getID());
  sort_run_generation->set_output_relation_id(sort_run_generation_operator.getOutputRelationID());
  sort_run_generation->set_output_destination_index(sort_run_generation_operator.getInsertDestinationID());
  sort_run_generation->set_sort_config_index(sort_run_generation_operator.getSortConfigIndex());
  sort_run_generation->set_is_input_stored(!sort_run_generation_operator.getInputRelation().isTemporary());
}

void ExecutionSerializer::serializeTableGenerator(const TableGeneratorOperator &table_generator_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_TABLE_GENERATOR);

  serialization::TableGeneratorOperator *table_generator = vertex->mutable_table_generator_operator();
  table_generator->set_output_relation_id(table_generator_operator.getOutputRelationID());
  table_generator->set_output_destination_index(table_generator_operator.getInsertDestinationID());
  table_generator->set_generator_function_index(table_generator_operator.getGeneratorFunctionIndex());
}

void ExecutionSerializer::serializeTextScan(const TextScanOperator &text_scan_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_TEXT_SCAN);

  serialization::TextScanOperator *text_scan = vertex->mutable_text_scan_operator();
  text_scan->set_output_relation_id(text_scan_operator.getOutputRelationID());
  text_scan->set_output_destination_index(text_scan_operator.getInsertDestinationID());
  text_scan->set_file_pattern(text_scan_operator.getFilePattern());
  text_scan->set_field_terminator(text_scan_operator.getFieldTerminator());
  text_scan->set_process_escape_sequences(text_scan_operator.getProcessEscapeSequences());
}

void ExecutionSerializer::serializeUpdate(const UpdateOperator &update_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_UPDATE);

  serialization::UpdateOperator *update = vertex->mutable_update_operator();
  update->set_predicate_index(update_operator.getPredicateIndex());
  update->set_relation_id(update_operator.getOutputRelationID());
  update->set_relocation_destionation_index(update_operator.getInsertDestinationID());
  update->set_update_group_index(update_operator.getUpdateGroupIndex());
}

void ExecutionSerializer::serializeWindowAggregation(const WindowAggregationOperator &window_aggregation_operator) {
  serialization::Vertex *vertex = query_plan_proto_.add_vertex();
  vertex->set_operator_type(S::OP_WINDOW_AGGREGATION);

  serialization::WindowAggregationOperator *window_aggregation
      = vertex->mutable_window_aggregation_operator();
  window_aggregation->set_output_relation_id(
      window_aggregation_operator.getOutputRelationID());
  window_aggregation->set_output_destination_index(
      window_aggregation_operator.getInsertDestinationID());
  window_aggregation->set_input_relation_id(
      window_aggregation_operator.getInputRelation().getID());
  window_aggregation->set_window_aggregation_state_index(
      window_aggregation_operator.getWindowAggregationStateIndex());
}

}
}