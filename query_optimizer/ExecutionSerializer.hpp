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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXECUTION_SERIALIZER_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXECUTION_SERIALIZER_HPP_

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

namespace quickstep {
namespace optimizer {

namespace S = quickstep::serialization;

/**
 * @brief
 */
class ExecutionSerializer {
 public:
  ExecutionSerializer() {
  }

  /**
   * @brief
   * @param query_plan
   */
  void serializePlan(const QueryPlan &query_plan);

private:
  void serializeInternal(const RelationalOperator &relational_operator);

  void serializeAggregation(const AggregationOperator &aggregation_operator);
  void serializeBuildAggregationExistenceMap(const BuildAggregationExistenceMapOperator &op);
  void serializeBuildHash(const BuildHashOperator &build_hash_operator);
  void serializeBuildLIPFilter(const BuildLIPFilterOperator &build_lip_filter_operator);
  void serializeCreateIndex(const CreateIndexOperator &create_index_operator);
  void serializeCreateTable(const CreateTableOperator &create_table_operator);
  void serializeDelete(const DeleteOperator &delete_operator);
  void serializeDestroyAggregationState(const DestroyAggregationStateOperator &destroy_aggregation_state_operator);
  void serializeDestroyHash(const DestroyHashOperator &destroy_hash_operator);
  void serializeDropTable(const DropTableOperator &drop_table_operator);
  void serializeFinalizeAggregation(const FinalizeAggregationOperator &finalize_aggregation_operator);
  void serializeHashJoin(const HashJoinOperator &finalize_hash_join);
  void serializeInitializeAggregation(const InitializeAggregationOperator &initialize_aggregation_operator);
  void serializeInsert(const InsertOperator &insert_operator);
  void serializeNestedLoopsJoin(const NestedLoopsJoinOperator &nested_loops_join_operator);
  void serializeSample(const SampleOperator &sample_operator);
  void serializeSaveBlocks(const SaveBlocksOperator &save_blocks_operator);
  void serializeSelect(const SelectOperator &select_operator);
  void serializeSortMergeRun(const SortMergeRunOperator &sort_merge_run_operator);
  void serializeSortRunGeneration(const SortRunGenerationOperator &sort_run_generation_operator);
  void serializeTableGenerator(const TableGeneratorOperator &table_generator_operator);
  void serializeTextScan(const TextScanOperator &text_scan_operator);
  void serializeUpdate(const UpdateOperator &update_operator);
  void serializeWindowAggregation(const WindowAggregationOperator &window_aggregation_operator);

  S::QueryPlan query_plan_proto_;
};

}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_EXECUTION_SERIALIZER_HPP_
