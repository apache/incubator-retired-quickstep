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

#include "query_execution/ExecutionDeserializer.hpp"

namespace quickstep {
namespace optimizer {

void ExecutionDeserializer::deserializePlan() {
  const auto &vertices = query_plan_proto_.vertex();
  for (const auto &vertex : vertices) {
    deserializeInternal(vertex);
  }
}

void ExecutionDeserializer::deserializeInternal(const S::Vertex &vertex) {
  switch (vertex.operator_type()) {
  case serialization::OP_AGGREGATION:
    return deserializeAggregation(vertex.aggregation_operator());
  case serialization::OP_BUILD_AGGREGATION_EXISTENCE_MAP:
    return deserializeBuildAggregationExistenceMap(vertex.build_aggregation_existence_map_operator());
  case serialization::OP_BUILD_HASH:
    return deserializeBuildHash(vertex.build_hash_operator());
  case serialization::OP_BUILD_LIP_FILTER:
    return deserializeBuildLIPFilter(vertex.build_lip_filter_operator());
  case serialization::OP_CREATE_INDEX:
    return deserializeCreateIndex(vertex.create_index_operator());
  case serialization::OP_CREATE_TABLE:
    return deserializeCreateTable(vertex.create_table_operator());
  case serialization::OP_DELETE:
    return deserializeDelete(vertex.delete_operator());
  case serialization::OP_DESTROY_AGGREGATION_STATE:
    return deserializeDestroyAggregationState(vertex.destroy_aggregation_state_operator());
  case serialization::OP_DESTROY_HASH:
    return deserializeDestroyHash(vertex.destroy_hash_operator());
  case serialization::OP_DROP_TABLE:
    return deserializeDropTable(vertex.drop_table_operator());
  case serialization::OP_FINALIZE_AGGREGATION:
    return deserializeFinalizeAggregation(vertex.finalize_aggregation_operator());
  case serialization::OP_HASH_JOIN:
    return deserializeHashJoin(vertex.hash_join_operator());
  case serialization::OP_INITIALIZE_AGGREGATION:
    return deserializeInitializeAggregation(vertex.initialize_aggregation_operator());
  case serialization::OP_INSERT:
    return deserializeInsert(vertex.insert_operator());
  case serialization::OP_NESTED_LOOP_JOIN:
    return deserializeNestedLoopsJoin(vertex.nested_loops_join_operator());
  case serialization::OP_SAMPLE:
    return deserializeSample(vertex.sample_operator());
  case serialization::OP_SAVE_BLOCKS:
    return deserializeSaveBlocks(vertex.save_blocks_operator());
  case serialization::OP_SELECT:
    return deserializeSelect(vertex.select_operator());
  case serialization::OP_SORT_MERGE_RUN:
    return deserializeSortMergeRun(vertex.sort_merge_run_operator());
  case serialization::OP_SORT_RUN_GENERATION:
    return deserializeSortRunGeneration(vertex.sort_run_generation_operator());
  case serialization::OP_TABLE_GENERATOR:
    return deserializeTableGenerator(vertex.table_generator_operator());
  case serialization::OP_TEXT_SCAN:
    return deserializeTextScan(vertex.text_scan_operator());
  case serialization::OP_UPDATE:
    return deserializeUpdate(vertex.update_operator());
  case serialization::OP_WINDOW_AGGREGATION:
    return deserializeWindowAggregation(vertex.window_aggregation_operator());
  }
}

void ExecutionDeserializer::deserializeAggregation(const S::AggregationOperator &aggregation_operator) {
  const std::size_t query_id = 1;
  const relation_id relation = aggregation_operator.relation_id();
  const bool is_relation_stored = aggregation_operator.relation_is_stored();
  QueryContext::aggregation_state_id aggregation_state_index = aggregation_operator.aggr_state_index();

  std::unique_ptr<AggregationOperator> aggregation = std::make_unique(new AggregationOperator())
  query_plan.addRelationalOperator();
}

void ExecutionDeserializer::deserializeBuildAggregationExistenceMap(const S::BuildAggregationExistenceMapOperator &op) {

}

void ExecutionDeserializer::deserializeBuildHash(const S::BuildHashOperator &build_hash_operator) {

}

void ExecutionDeserializer::deserializeBuildLIPFilter(const S::BuildLIPFilterOperator &build_lip_filter_operator) {

}

void ExecutionDeserializer::deserializeCreateIndex(const S::CreateIndexOperator &create_index_operator) {

}

void ExecutionDeserializer::deserializeCreateTable(const S::CreateTableOperator &create_table_operator) {

}

void ExecutionDeserializer::deserializeDelete(const S::DeleteOperator &delete_operator) {

}

void ExecutionDeserializer::deserializeDestroyAggregationState(const S::DestroyAggregationStateOperator &destroy_aggregation_state_operator) {

}

void ExecutionDeserializer::deserializeDestroyHash(const S::DestroyHashOperator &destroy_hash_operator) {

}

void ExecutionDeserializer::deserializeDropTable(const S::DropTableOperator &drop_table_operator) {

}

void ExecutionDeserializer::deserializeFinalizeAggregation(const S::FinalizeAggregationOperator &finalize_aggregation_operator) {

}

void ExecutionDeserializer::deserializeHashJoin(const S::HashJoinOperator &finalize_hash_join) {

}

void ExecutionDeserializer::deserializeInitializeAggregation(const S::InitializeAggregationOperator &initialize_aggregation_operator) {

}

void ExecutionDeserializer::deserializeInsert(const S::InsertOperator &insert_operator) {

}

void ExecutionDeserializer::deserializeNestedLoopsJoin(const S::NestedLoopsJoinOperator &nested_loops_join_operator) {

}

void ExecutionDeserializer::deserializeSample(const S::SampleOperator &sample_operator) {

}

void ExecutionDeserializer::deserializeSaveBlocks(const S::SaveBlocksOperator &save_blocks_operator) {

}

void ExecutionDeserializer::deserializeSelect(const S::SelectOperator &select_operator) {

}

void ExecutionDeserializer::deserializeSortMergeRun(const S::SortMergeRunOperator &sort_merge_run_operator) {

}

void ExecutionDeserializer::deserializeSortRunGeneration(const S::SortRunGenerationOperator &sort_run_generation_operator) {

}

void ExecutionDeserializer::deserializeTableGenerator(const S::TableGeneratorOperator &table_generator_operator) {

}

void ExecutionDeserializer::deserializeTextScan(const S::TextScanOperator &text_scan_operator) {

}

void ExecutionDeserializer::deserializeUpdate(const S::UpdateOperator &update_operator) {

}

void ExecutionDeserializer::deserializeWindowAggregation(const S::WindowAggregationOperator &window_aggregation_operator) {

}

}
}