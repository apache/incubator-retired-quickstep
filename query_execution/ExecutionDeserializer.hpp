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

#ifndef QUICKSTEP_QUERY_EXECUTION_EXECUTION_DESERIALIZER_HPP
#define QUICKSTEP_QUERY_EXECUTION_EXECUTION_DESERIALIZER_HPP

#include "catalog/CatalogDatabase.hpp"
#include "query_optimizer/QueryPlan.hpp"
#include "relational_operators/AggregationOperator.hpp"
#include "relational_operators/BuildAggregationExistenceMapOperator.hpp"
#include "relational_operators/BuildHashOperator.hpp"
#include "relational_operators/BuildLIPFilterOperator.hpp"
#include "relational_operators/CreateIndexOperator.hpp"
#include "relational_operators/CreateTableOperator.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/DeleteOperator.hpp"
#include "relational_operators/DestroyAggregationStateOperator.hpp"
#include "relational_operators/DestroyHashOperator.hpp"
#include "relational_operators/DropTableOperator.hpp"
#include "relational_operators/FinalizeAggregationOperator.hpp"
#include "relational_operators/HashJoinOperator.hpp"
#include "relational_operators/InitializeAggregationOperator.hpp"
#include "relational_operators/InsertOperator.hpp"
#include "relational_operators/NestedLoopsJoinOperator.hpp"
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

namespace quickstep {
namespace optimizer {

namespace S = ::quickstep::serialization;

class ExecutionDeserializer {
 public:
  ExecutionDeserializer(const S::QueryPlan &query_plan, CatalogDatabase *database)
    : query_plan_proto_(query_plan),
      database_(*database) {
  }

  void deserializePlan();

 private:
  void deserializeInternal(const S::Vertex &relational_operator);

  void deserializeAggregation(const S::AggregationOperator &aggregation_operator);
  void deserializeBuildAggregationExistenceMap(const S::BuildAggregationExistenceMapOperator &op);
  void deserializeBuildHash(const S::BuildHashOperator &build_hash_operator);
  void deserializeBuildLIPFilter(const S::BuildLIPFilterOperator &build_lip_filter_operator);
  void deserializeCreateIndex(const S::CreateIndexOperator &create_index_operator);
  void deserializeCreateTable(const S::CreateTableOperator &create_table_operator);
  void deserializeDelete(const S::DeleteOperator &delete_operator);
  void deserializeDestroyAggregationState(const S::DestroyAggregationStateOperator &destroy_aggregation_state_operator);
  void deserializeDestroyHash(const S::DestroyHashOperator &destroy_hash_operator);
  void deserializeDropTable(const S::DropTableOperator &drop_table_operator);
  void deserializeFinalizeAggregation(const S::FinalizeAggregationOperator &finalize_aggregation_operator);
  void deserializeHashJoin(const S::HashJoinOperator &hash_join_operator);
  void deserializeInitializeAggregation(const S::InitializeAggregationOperator &initialize_aggregation_operator);
  void deserializeInsert(const S::InsertOperator &insert_operator);
  void deserializeNestedLoopsJoin(const S::NestedLoopsJoinOperator &nested_loops_join_operator);
  void deserializeSample(const S::SampleOperator &sample_operator);
  void deserializeSaveBlocks(const S::SaveBlocksOperator &save_blocks_operator);
  void deserializeSelect(const S::SelectOperator &select_operator);
  void deserializeSortMergeRun(const S::SortMergeRunOperator &sort_merge_run_operator);
  void deserializeSortRunGeneration(const S::SortRunGenerationOperator &sort_run_generation_operator);
  void deserializeTableGenerator(const S::TableGeneratorOperator &table_generator_operator);
  void deserializeTextScan(const S::TextScanOperator &text_scan_operator);
  void deserializeUpdate(const S::UpdateOperator &update_operator);
  void deserializeWindowAggregation(const S::WindowAggregationOperator &window_aggregation_operator);

  const S::QueryPlan &query_plan_proto_;
  CatalogDatabase &database_;
  
  QueryPlan query_plan;
};

}
}

#endif //QUICKSTEP_EXECUTIONDESERIALIZER_HPP
