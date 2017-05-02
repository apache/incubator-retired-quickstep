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

#include <vector>

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
  const CatalogRelation *catalog = database_.getRelationById(relation);
  const bool is_relation_stored = aggregation_operator.relation_is_stored();
  QueryContext::aggregation_state_id aggregation_state_index = aggregation_operator.aggr_state_index();


  const AggregationOperator* aggregation
      = new AggregationOperator(query_id,
                                *catalog,
                                is_relation_stored,
                                aggregation_state_index);
  query_plan.addRelationalOperator(aggregation);
}

void ExecutionDeserializer::deserializeBuildAggregationExistenceMap(const S::BuildAggregationExistenceMapOperator &op) {
  const std::size_t query_id = 1;
  const relation_id relation = op.relation_id();
  const CatalogRelation *catalog = database_.getRelationById(relation);
  const attribute_id build_attribute = op.attribute_id();
  const bool is_relation_stored = op.relation_is_stored();
  const QueryContext::aggregation_state_id aggregation_state_index = op.aggr_state_index();

  const BuildAggregationExistenceMapOperator* build_aggregation_existence_map
      = new BuildAggregationExistenceMapOperator(query_id,
                                                 *catalog,
                                                 build_attribute,
                                                 is_relation_stored,
                                                 aggregation_state_index);
  query_plan.addRelationalOperator(build_aggregation_existence_map);
}

void ExecutionDeserializer::deserializeBuildHash(const S::BuildHashOperator &build_hash_operator) {
  const std::size_t query_id = 1;
  const relation_id relation = build_hash_operator.relation_id();
  const CatalogRelation *catalog = database_.getRelationById(relation);
  const is_input_relation_stored = build_hash_operator.relation_is_stored();
  std::vector<attribute_id> join_key_attributes;
  join_key_attributes.reserve(build_hash_operator.join_key_attributes_size());
  for (std::size_t i = 0; i < build_hash_operator.join_key_attributes_size(); ++i) {
    join_key_attributes.push_back(build_hash_operator.join_key_attributes(i));
  }
  const bool any_join_key_attributes_nullable = build_hash_operator.any_join_key_nullable();
  const std::size_t num_partitions = build_hash_operator.num_partitions();
  const QueryContext::join_hash_table_id join_hash_table_index = build_hash_operator.hash_table_index();

  const BuildHashOperator* build_hash
      = new BuildHashOperator(query_id,
                              *catalog,
                              is_input_relation_stored,
                              join_key_attributes,
                              any_join_key_attributes_nullable,
                              num_partitions,
                              join_hash_table_index);

  query_plan.addRelationalOperator(build_hash);
}

void ExecutionDeserializer::deserializeBuildLIPFilter(const S::BuildLIPFilterOperator &build_lip_filter_operator) {
  const std::size_t query_id = 1;
  const relation_id relation = build_lip_filter_operator.relation_id();
  const CatalogRelation *catalog =  database_.getRelationById(relation);
  const bool is_input_relation_stored = build_lip_filter_operator.relation_is_stored();
  const QueryContext::predicate_id build_side_predicate_index
      = build_lip_filter_operator.build_side_predicate_index();

  const BuildLIPFilterOperator* build_lip_filter
      = new BuildLIPFilterOperator(query_id,
                                   *catalog,
                                   build_side_predicate_index,
                                   is_input_relation_stored);

  query_plan.addRelationalOperator(build_lip_filter);
}

void ExecutionDeserializer::deserializeCreateIndex(const S::CreateIndexOperator &create_index_operator) {
  const std::size_t query_id = 1;
  const relation_id relation = create_index_operator.relation_id();
  CatalogRelation *catalog = database_.getRelationByIdMutable(relation);
  const std::string index_name = create_index_operator.index_name();
  IndexSubBlockDescription description = create_index_operator.index_description();

  const CreateIndexOperator* create_index
      = new CreateIndexOperator(query_id,
                                catalog,
                                index_name,
                                std::move(description));

  query_plan.addRelationalOperator(create_index);
}

void ExecutionDeserializer::deserializeCreateTable(const S::CreateTableOperator &create_table_operator) {
  const std::size_t query_id = 1;
  const serialization::CatalogRelationSchema &catalog_relation = create_table_operator.relation_catalog();
  CatalogRelation *relation = new CatalogRelation(catalog_relation);

  const CreateTableOperator* create_table
      = new CreateTableOperator(query_id,
                                relation,
                                &database_);

  query_plan.addRelationalOperator(create_table);
}

void ExecutionDeserializer::deserializeDelete(const S::DeleteOperator &delete_operator) {
  const std::size_t query_id = 1;
  const relation_id relation = delete_operator.relation_id();
  const CatalogRelation *catalog_relation = database_.getRelationById(relation);
  const bool is_relation_stored = delete_operator.relation_is_stored();
  const QueryContext::predicate_id predicate_index = delete_operator.predicate_index();

  const DeleteOperator* delete_op
      = new DeleteOperator(query_id,
                           *catalog_relation,
                           predicate_index,
                           is_relation_stored);

  query_plan.addRelationalOperator(delete_op);
}

void ExecutionDeserializer::deserializeDestroyAggregationState(
    const S::DestroyAggregationStateOperator &destroy_aggregation_state_operator) {
  const std::size_t query_id = 1;
  const QueryContext::aggregation_state_id aggregation_state_index
      = destroy_aggregation_state_operator.aggr_state_index();

  const DestroyAggregationStateOperator* destroy_aggregation_state
      = new DestroyAggregationStateOperator(query_id, aggregation_state_index);

  query_plan.addRelationalOperator(destroy_aggregation_state);
}

void ExecutionDeserializer::deserializeDestroyHash(
    const S::DestroyHashOperator &destroy_hash_operator) {
  const std::size_t query_id = 1;
  const std::size_t build_num_partitions = destroy_hash_operator.build_num_partitions();
  const QueryContext::join_hash_table_id hash_table_index = destroy_hash_operator.hash_table_index();

  const DestroyHashOperator* destroy_hash
      = new DestroyHashOperator(query_id,
                                build_num_partitions,
                                hash_table_index);

  query_plan.addRelationalOperator(destroy_hash);
}

void ExecutionDeserializer::deserializeDropTable(
    const S::DropTableOperator &drop_table_operator) {
  const std::size_t query_id = 1;
  const relation_id relation = drop_table_operator.relation_id();
  const CatalogRelation *catalog_relation = database_.getRelationById(relation);
  const bool only_drop_blocks = drop_table_operator.only_drop_blocks();

  const DropTableOperator* drop_table
      = new DropTableOperator(query_id,
                              *catalog_relation,
                              &database_,
                              only_drop_blocks);

  query_plan.addRelationalOperator(drop_table);
}

void ExecutionDeserializer::deserializeFinalizeAggregation(
    const S::FinalizeAggregationOperator &finalize_aggregation_operator) {
  const std::size_t query_id = 1;
  const QueryContext::aggregation_state_id aggregation_state_index = finalize_aggregation_operator.aggr_state_index();
  const QueryContext::insert_destination_id output_index = finalize_aggregation_operator.output_destination_index();
  const relation_id output_relation = finalize_aggregation_operator.output_relation_id();
  const CatalogRelation *output_catalog = database_.getRelationById(output_relation);

  const FinalizeAggregationOperator* finalize_aggregation
      = new FinalizeAggregationOperator(query_id,
                                        aggregation_state_index,
                                        *output_catalog,
                                        output_index);
  query_plan.addRelationalOperator(finalize_aggregation);
}

void ExecutionDeserializer::deserializeHashJoin(const S::HashJoinOperator &hash_join_operator) {
  const std::size_t query_id = 1;
  const relation_id build_relation = hash_join_operator.build_relation_id();
  const relation_id probe_relation = hash_join_operator.probe_relation_id();
  const CatalogRelation *build_catalog = database_.getRelationById(build_relation);
  const CatalogRelation *probe_catalog = database_.getRelationById(probe_relation);
  const bool probe_relation_is_stored = hash_join_operator.is_probe_relation_stored();

  std::vector<attribute_id> join_attributes;
  for (const auto &attribute : hash_join_operator.join_key_attributes()) {
    join_attributes.push_back(attribute);
  }
  const bool any_join_key_nullable = hash_join_operator.any_join_key_attributes_nullable();
  const std::size_t build_num_partitions = hash_join_operator.build_num_partitions();

  const relation_id output_relation = hash_join_operator.output_relation_id();
  const CatalogRelation *output_catalog = database_.getRelationById(output_relation);

  const QueryContext::insert_destination_id output_destination_index
      = hash_join_operator.output_destionation_index();

  const QueryContext::join_hash_table_id hash_table_index = hash_join_operator.hash_table_index();
  const QueryContext::predicate_id residual_predicate_index = hash_join_operator.residiual_predicate_index();
  const QueryContext::scalar_group_id selection_index = hash_join_operator.selection_index();

  std::unique_ptr<std::vector<bool>> is_selection_on_build = std::make_unique<std::vector<bool>>();
  for (const auto &selection_on_build : hash_join_operator.is_selection_on_build()) {
    is_selection_on_build->push_back(selection_on_build);
  }

  HashJoinOperator::JoinType join_type;
  switch (hash_join_operator.join_type()) {
  case serialization::HashJoinOperator_JoinType_LEFT_INNER:
    join_type = HashJoinOperator::JoinType::kInnerJoin;
    break;
  case serialization::HashJoinOperator_JoinType_LEFT_SEMI:
    join_type = HashJoinOperator::JoinType::kLeftSemiJoin;
    break;
  case serialization::HashJoinOperator_JoinType_LEFT_ANTI:
    join_type = HashJoinOperator::JoinType::kLeftAntiJoin;
    break;
  case serialization::HashJoinOperator_JoinType_LEFT_OUTER:
    join_type = HashJoinOperator::JoinType::kLeftOuterJoin;
    break;
  default:
    join_type = HashJoinOperator::JoinType::kInnerJoin;
  }

  const HashJoinOperator *hash_join
      = new HashJoinOperator(query_id,
                             *build_catalog,
                             *probe_catalog,
                             probe_relation_is_stored,
                             join_attributes,
                             any_join_key_nullable,
                             build_num_partitions,
                             *output_catalog,
                             output_destination_index,
                             hash_table_index,
                             residual_predicate_index,selection_index,
                             is_selection_on_build.release(),
                             join_type);

  query_plan.addRelationalOperator(hash_join);
}

void ExecutionDeserializer::deserializeInitializeAggregation(
    const S::InitializeAggregationOperator &initialize_aggregation_operator) {
  const std::size_t query_id = 1;
  const QueryContext::aggregation_state_id aggregation_state_index = initialize_aggregation_operator.aggr_state_index();
  const InitializeAggregationOperator *initialize_aggregation
      = new InitializeAggregationOperator(query_id, aggregation_state_index);

  query_plan.addRelationalOperator(initialize_aggregation);
}

void ExecutionDeserializer::deserializeInsert(const S::InsertOperator &insert_operator) {
  const std::size_t query_id = 1;

  const CatalogRelation *output_catalog = database_.getRelationById(insert_operator.output_relation_id());
  const QueryContext::insert_destination_id output_index = insert_operator.output_destination_index();
  const QueryContext::tuple_id tuple_index = insert_operator.tuple_index();

  const InsertOperator *insert = new InsertOperator(query_id, *output_catalog, output_index,  tuple_index);

  query_plan.addRelationalOperator(insert);
}

void ExecutionDeserializer::deserializeNestedLoopsJoin(const S::NestedLoopsJoinOperator &nested_loops_join_operator) {
  const std::size_t query_id = 1;

  const CatalogRelation *output_catalog = database_.getRelationById(nested_loops_join_operator.output_relation_id());
  const CatalogRelation *left_catalog = database_.getRelationById(nested_loops_join_operator.left_input_relation_id());
  const CatalogRelation *right_catalog = database_.getRelationById(nested_loops_join_operator.right_input_relation_id());

  const QueryContext::insert_destination_id output_index = nested_loops_join_operator.output_destination_index();
  const QueryContext::predicate_id join_predicate_index = nested_loops_join_operator.join_predicate_index();
  const QueryContext::scalar_group_id selection_index = nested_loops_join_operator.selection_index();
  const bool left_is_stored = nested_loops_join_operator.left_relation_is_stored();
  const bool right_is_stored = nested_loops_join_operator.right_relation_is_stored();

  const NestedLoopsJoinOperator *nested_loops_join
      = new NestedLoopsJoinOperator(query_id,
                                    *left_catalog,
                                    *right_catalog,
                                    *output_catalog,
                                    output_index,
                                    join_predicate_index,
                                    selection_index,
                                    left_is_stored,
                                    right_is_stored);

  query_plan.addRelationalOperator(nested_loops_join);
}

void ExecutionDeserializer::deserializeSample(const S::SampleOperator &sample_operator) {
  const std::size_t query_id = 1;
  const CatalogRelation *input_catalog = database_.getRelationById(sample_operator.input_relation_id());
  const CatalogRelation *output_catalog = database_.getRelationById(sample_operator.output_relation_id());
  const QueryContext::insert_destination_id output_index = sample_operator.output_destination_index();
  const bool is_input_stored = sample_operator.input_relation_is_stored();
  const bool is_block_sample = sample_operator.is_block_sample();
  const int percentage = sample_operator.percentage();

  const SampleOperator *sample
      = new SampleOperator(query_id,
                           *input_catalog,
                           *output_catalog,
                           output_index,
                           is_input_stored,
                           is_block_sample,
                           percentage);

  query_plan.addRelationalOperator(sample);
}

void ExecutionDeserializer::deserializeSaveBlocks(const S::SaveBlocksOperator &save_blocks_operator) {
  const std::size_t query_id = 1;

  std::unique_ptr<CatalogRelation> catalog = std::make_unique<CatalogRelation>(save_blocks_operator.relation());
  const bool force = save_blocks_operator.force();

  const SaveBlocksOperator *save_blocks = new SaveBlocksOperator(query_id, catalog.release(), force);
  query_plan.addRelationalOperator(save_blocks);
}

void ExecutionDeserializer::deserializeSelect(const S::SelectOperator &select_operator) {
  const std::size_t query_id = 1;
  const CatalogRelation *input = database_.getRelationById(select_operator.input_relation_id());
  const CatalogRelation *output = database_.getRelationById(select_operator.output_relation_id());

  const QueryContext::insert_destination_id output_destionation_index = select_operator.output_destination_index();
  const bool is_input_stored = select_operator.input_relation_is_stored();
  const QueryContext::predicate_id predicate_index = select_operator.predicate_index();

  const std::size_t num_partitions = select_operator.num_partitions();
  std::vector<attribute_id> *selections = new std::vector<attribute_id>;
  for (const auto &selection : select_operator.selection_attributes()) {
    selections->push_back(selection);
  }


  const SelectOperator *select
      = (selections->size() == 1) ? new SelectOperator(query_id,
                                                       *input,
                                                       *output,
                                                       output_destionation_index,
                                                       predicate_index,
                                                       selections->[0],
                                                       is_input_stored,
                                                       num_partitions)
                                  : new SelectOperator(query_id,
                                                       *input,
                                                       *output,
                                                       output_destionation_index,
                                                       predicate_index,
                                                       std::move(*selections),
                                                       is_input_stored,
                                                       num_partitions);

  query_plan.addRelationalOperator(select);
}

void ExecutionDeserializer::deserializeSortMergeRun(const S::SortMergeRunOperator &sort_merge_run_operator) {
  const std::size_t query_id = 1;

  const CatalogRelation *input = database_.getRelationById(sort_merge_run_operator.input_relation_id());
  const CatalogRelation *output = database_.getRelationById(sort_merge_run_operator.output_relation_id());

  const QueryContext::insert_destination_id output_destination_index
      = sort_merge_run_operator.output_destination_index();

  const CatalogRelation *run = database_.getRelationById(sort_merge_run_operator.run_relation_id());
  const QueryContext::insert_destination_id run_destination_index
      = sort_merge_run_operator.run_block_destination_index();

  const QueryContext::sort_config_id sort_config_index = sort_merge_run_operator.sort_config_index();
  const std::uint64_t merge_factor = sort_merge_run_operator.merge_factor();
  const std::uint64_t top_k = sort_merge_run_operator.top_k();
  const bool input_relation_is_stored = sort_merge_run_operator.is_input_stored();

  const SortMergeRunOperator *sort_merge_run
      = new SortMergeRunOperator(query_id,
                                 *input,
                                 *output,
                                 output_destination_index,
                                 *run,
                                 run_destination_index,
                                 sort_config_index,
                                 merge_factor, top_k,
                                 input_relation_is_stored);

  query_plan.addRelationalOperator(sort_merge_run);
}

void ExecutionDeserializer::deserializeSortRunGeneration(const S::SortRunGenerationOperator &sort_run_generation_operator) {
  const std::size_t query_id = 1;
  const CatalogRelation *input = database_.getRelationById(sort_run_generation_operator.input_relation_id());
  const CatalogRelation *output = database_.getRelationById(sort_run_generation_operator.output_relation_id());

  const QueryContext::insert_destination_id output_destination_index
      = sort_run_generation_operator.output_destination_index();

  const QueryContext::sort_config_id sort_config_index = sort_run_generation_operator.sort_config_index();
  const bool input_relation_is_stored = sort_run_generation_operator.is_input_stored();

  const SortRunGenerationOperator *sort_run_generation
      = new SortRunGenerationOperator(query_id,
                                      *input,
                                      *output,
                                      output_destination_index,
                                      sort_config_index,
                                      input_relation_is_stored);

  query_plan.addRelationalOperator(sort_run_generation);
}

void ExecutionDeserializer::deserializeTableGenerator(const S::TableGeneratorOperator &table_generator_operator) {
  const std::size_t query_id = 1;

  const CatalogRelation *output = database_.getRelationById(table_generator_operator.output_relation_id());
  const QueryContext::insert_destination_id output_destination_index
      = table_generator_operator.output_destination_index();
  const QueryContext::generator_function_id generator_function_index
      = table_generator_operator.generator_function_index();

  const TableGeneratorOperator *table_generator
      = new TableGeneratorOperator(query_id,
                                   *output,
                                   output_destination_index,
                                   generator_function_index);

  query_plan.addRelationalOperator(table_generator);
}

void ExecutionDeserializer::deserializeTextScan(const S::TextScanOperator &text_scan_operator) {
  const std::size_t query_id = 1;

  const std::string file_pattern = text_scan_operator.file_pattern();
  const char field_terminator = text_scan_operator.field_terminator();
  const bool process_escape_sequences = text_scan_operator.process_escape_sequences();

  const CatalogRelation *output = database_.getRelationById(text_scan_operator.output_relation_id());
  const QueryContext::insert_destination_id output_index = text_scan_operator.output_destination_index();

  const TextScanOperator *text_scan
      = new TextScanOperator(query_id,
                             file_pattern,
                             field_terminator,
                             process_escape_sequences,
                             *output,
                             output_index);

  query_plan.addRelationalOperator(text_scan);
}

void ExecutionDeserializer::deserializeUpdate(const S::UpdateOperator &update_operator) {
  const std::size_t query_id = 1;

  const CatalogRelation *relation = database_.getRelationById(update_operator.relation_id());
  const QueryContext::insert_destination_id relocation_destination_index
      = update_operator.relocation_destionation_index();
  const QueryContext::predicate_id predicate_index = update_operator.predicate_index();
  const QueryContext::update_group_id update_group_index = update_operator.update_group_index();

  const UpdateOperator *update = new UpdateOperator(query_id,
                                                    *relation,
                                                    relocation_destination_index,
                                                    predicate_index,
                                                    update_group_index);
  query_plan.addRelationalOperator(update);
}

void ExecutionDeserializer::deserializeWindowAggregation(
    const S::WindowAggregationOperator &window_aggregation_operator) {
  const std::size_t query_id = 1;

  const CatalogRelation *input_relation
      = database_.getRelationById(window_aggregation_operator.input_relation_id());
  const CatalogRelation *output_relation
      = database_.getRelationById(window_aggregation_operator.output_relation_id());
  const QueryContext::window_aggregation_state_id window_aggregation_state_index
      = window_aggregation_operator.window_aggregation_state_index();
  const QueryContext::insert_destination_id output_destination_index
      = window_aggregation_operator.output_destination_index();

  const WindowAggregationOperator *window_aggregation
      = new WindowAggregationOperator(query_id,
                                      *input_relation,
                                      *output_relation,
                                      window_aggregation_state_index,
                                      output_destination_index);
  
  query_plan.addRelationalOperator(window_aggregation);
}

}
}