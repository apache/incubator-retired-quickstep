/**
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

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "catalog/Catalog.hpp"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryContext.pb.h"
#include "query_optimizer/ExecutionHeuristics.hpp"
#include "query_optimizer/QueryPlan.hpp"
#include "relational_operators/BuildHashOperator.hpp"
#include "relational_operators/HashJoinOperator.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace quickstep {
namespace optimizer {

namespace {
constexpr std::size_t kQueryId = 0;
}

class ExecutionHeuristicsTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    db_ = cat_.getDatabaseByIdMutable(cat_.addDatabase(new CatalogDatabase(nullptr, "db")));
    execution_heuristics_.reset(new ExecutionHeuristics());
    query_plan_.reset(new QueryPlan());
    query_context_proto_.reset(new serialization::QueryContext());
  }

  CatalogRelation* createCatalogRelation(const std::string &name, bool temporary = false) {
    return db_->getRelationByIdMutable(db_->addRelation(new CatalogRelation(nullptr, name, -1, temporary)));
  }

  void addDummyHashJoinInfo(ExecutionHeuristics *execution_heuristics,
                            const QueryPlan::DAGNodeIndex build_operator_index,
                            const QueryPlan::DAGNodeIndex join_operator_index,
                            const CatalogRelation *build_relation,
                            const CatalogRelation *probe_relation,
                            const attribute_id build_attribute_id,
                            const attribute_id probe_attribute_id,
                            const QueryContext::join_hash_table_id join_hash_table_id) {
    std::vector<attribute_id> build_attribute_ids(1, build_attribute_id);
    std::vector<attribute_id> probe_attribute_ids(1, probe_attribute_id);
    execution_heuristics->addHashJoinInfo(build_operator_index,
                                          join_operator_index,
                                          build_relation,
                                          probe_relation,
                                          std::move(build_attribute_ids),
                                          std::move(probe_attribute_ids),
                                          join_hash_table_id,
                                          build_relation->estimateTupleCardinality());
  }

  QueryPlan::DAGNodeIndex createDummyBuildHashOperator(QueryPlan *query_plan,
                                                       const CatalogRelation *build_relation,
                                                       const attribute_id build_attribute_id,
                                                       const QueryContext::join_hash_table_id join_hash_table_index) {
    std::vector<attribute_id> build_attribute_ids;
    build_attribute_ids.push_back(build_attribute_id);
    QueryPlan::DAGNodeIndex build_operator_index =
        query_plan->addRelationalOperator(new BuildHashOperator(kQueryId,
                                                                *build_relation,
                                                                true,
                                                                build_attribute_ids,
                                                                false,
                                                                join_hash_table_index));
    return build_operator_index;
  }

  QueryPlan::DAGNodeIndex createDummyHashJoinOperator(QueryPlan *query_plan,
                                                      const CatalogRelation *build_relation,
                                                      const CatalogRelation *probe_relation,
                                                      const attribute_id probe_attribute_id,
                                                      const QueryContext::join_hash_table_id join_hash_table_index) {
    std::vector<attribute_id> probe_attribute_ids;
    probe_attribute_ids.push_back(probe_attribute_id);
    QueryPlan::DAGNodeIndex join_operator_index =
        query_plan->addRelationalOperator(
            new HashJoinOperator(kQueryId,
                                 *build_relation,
                                 *probe_relation,
                                 true,
                                 probe_attribute_ids,
                                 false,
                                 *probe_relation,
                                 0,
                                 join_hash_table_index,
                                 0,
                                 0));
    return join_operator_index;
  }

  Catalog cat_;
  CatalogDatabase *db_;  // db_ is owned by cat_.
  std::unique_ptr<QueryPlan> query_plan_;
  std::unique_ptr<serialization::QueryContext> query_context_proto_;
  std::unique_ptr<ExecutionHeuristics> execution_heuristics_;
};

TEST_F(ExecutionHeuristicsTest, HashJoinOptimizedTest) {
  // This test case creates three hash joins, all of which are being probed on the same relation.
  // Since the probe are being made on the same relation, ExecutionHeuristics should optimize
  // these hash joins using bloom filters.

  const CatalogRelation *build_relation_1 = createCatalogRelation("build_relation_1");
  const CatalogRelation *build_relation_2 = createCatalogRelation("build_relation_2");
  const CatalogRelation *build_relation_3 = createCatalogRelation("build_relation_3");
  const CatalogRelation *probe_relation_1 = createCatalogRelation("probe_relation_1");

  const attribute_id build_attribute_id_1 = 0;
  const attribute_id build_attribute_id_2 = 0;
  const attribute_id build_attribute_id_3 = 0;
  const attribute_id probe_attribute_id_1 = 1;
  const attribute_id probe_attribute_id_2 = 2;
  const attribute_id probe_attribute_id_3 = 3;

  const QueryContext::join_hash_table_id join_hash_table_index_1 = 0;
  const QueryContext::join_hash_table_id join_hash_table_index_2 = 1;
  const QueryContext::join_hash_table_id join_hash_table_index_3 = 2;
  query_context_proto_->add_join_hash_tables();
  query_context_proto_->add_join_hash_tables();
  query_context_proto_->add_join_hash_tables();

  const QueryPlan::DAGNodeIndex build_operator_index_1 = createDummyBuildHashOperator(query_plan_.get(),
                                                                                      build_relation_1,
                                                                                      build_attribute_id_1,
                                                                                      join_hash_table_index_1);
  const QueryPlan::DAGNodeIndex probe_operator_index_1 = createDummyHashJoinOperator(query_plan_.get(),
                                                                                     build_relation_1,
                                                                                     probe_relation_1,
                                                                                     probe_attribute_id_1,
                                                                                     join_hash_table_index_1);
  const QueryPlan::DAGNodeIndex build_operator_index_2 = createDummyBuildHashOperator(query_plan_.get(),
                                                                                      build_relation_2,
                                                                                      build_attribute_id_2,
                                                                                      join_hash_table_index_2);
  const QueryPlan::DAGNodeIndex probe_operator_index_2 = createDummyHashJoinOperator(query_plan_.get(),
                                                                                     build_relation_2,
                                                                                     probe_relation_1,
                                                                                     probe_attribute_id_2,
                                                                                     join_hash_table_index_2);
  const QueryPlan::DAGNodeIndex build_operator_index_3 = createDummyBuildHashOperator(query_plan_.get(),
                                                                                      build_relation_3,
                                                                                      build_attribute_id_3,
                                                                                      join_hash_table_index_3);
  const QueryPlan::DAGNodeIndex probe_operator_index_3 = createDummyHashJoinOperator(query_plan_.get(),
                                                                                     build_relation_3,
                                                                                     probe_relation_1,
                                                                                     probe_attribute_id_3,
                                                                                     join_hash_table_index_3);

  addDummyHashJoinInfo(execution_heuristics_.get(),
                       build_operator_index_1,
                       probe_operator_index_1,
                       build_relation_1,
                       probe_relation_1,
                       build_attribute_id_1,
                       probe_attribute_id_1,
                       join_hash_table_index_1);
  addDummyHashJoinInfo(execution_heuristics_.get(),
                       build_operator_index_2,
                       probe_operator_index_2,
                       build_relation_2,
                       probe_relation_1,
                       build_attribute_id_2,
                       probe_attribute_id_2,
                       join_hash_table_index_2);
  addDummyHashJoinInfo(execution_heuristics_.get(),
                       build_operator_index_3,
                       probe_operator_index_3,
                       build_relation_3,
                       probe_relation_1,
                       build_attribute_id_3,
                       probe_attribute_id_3,
                       join_hash_table_index_3);

  execution_heuristics_->optimizeExecutionPlan(query_plan_.get(), query_context_proto_.get());

  // Test whether correct number of bloom filters were added.
  EXPECT_EQ(1, query_context_proto_->join_hash_tables(0).build_side_bloom_filter_id_size());
  EXPECT_EQ(1, query_context_proto_->join_hash_tables(1).build_side_bloom_filter_id_size());
  EXPECT_EQ(1, query_context_proto_->join_hash_tables(2).build_side_bloom_filter_id_size());
  EXPECT_EQ(3, query_context_proto_->join_hash_tables(0).probe_side_bloom_filters_size());

  // Test that the DAG was modified correctly or not.
  // Probe operator 1 should have now build operator 1 and build operator 2 added as dependencies.
  auto const probe_node_dependencies = query_plan_->getQueryPlanDAG().getDependencies(probe_operator_index_1);
  EXPECT_EQ(1u, probe_node_dependencies.count(build_operator_index_2));
  EXPECT_EQ(1u, probe_node_dependencies.count(build_operator_index_3));
}

TEST_F(ExecutionHeuristicsTest, HashJoinNotOptimizedTest) {
  // This test case creates three hash joins, all of which are being probed on different relations.
  // Since the probe are being made on the different relations, ExecutionHeuristics should optimize
  // these hash joins using bloom filters.

  const CatalogRelation *build_relation_1 = createCatalogRelation("build_relation_1");
  const CatalogRelation *build_relation_2 = createCatalogRelation("build_relation_2");
  const CatalogRelation *build_relation_3 = createCatalogRelation("build_relation_3");
  const CatalogRelation *probe_relation_1 = createCatalogRelation("probe_relation_1");
  const CatalogRelation *probe_relation_2 = createCatalogRelation("probe_relation_2");
  const CatalogRelation *probe_relation_3 = createCatalogRelation("probe_relation_3");

  const attribute_id build_attribute_id_1 = 0;
  const attribute_id build_attribute_id_2 = 0;
  const attribute_id build_attribute_id_3 = 0;
  const attribute_id probe_attribute_id_1 = 1;
  const attribute_id probe_attribute_id_2 = 2;
  const attribute_id probe_attribute_id_3 = 3;

  const QueryContext::join_hash_table_id join_hash_table_index_1 = 0;
  const QueryContext::join_hash_table_id join_hash_table_index_2 = 1;
  const QueryContext::join_hash_table_id join_hash_table_index_3 = 2;
  query_context_proto_->add_join_hash_tables();
  query_context_proto_->add_join_hash_tables();
  query_context_proto_->add_join_hash_tables();

  const QueryPlan::DAGNodeIndex build_operator_index_1 = createDummyBuildHashOperator(query_plan_.get(),
                                                                                      build_relation_1,
                                                                                      build_attribute_id_1,
                                                                                      join_hash_table_index_1);
  const QueryPlan::DAGNodeIndex probe_operator_index_1 = createDummyHashJoinOperator(query_plan_.get(),
                                                                                     build_relation_1,
                                                                                     probe_relation_1,
                                                                                     probe_attribute_id_1,
                                                                                     join_hash_table_index_1);
  const QueryPlan::DAGNodeIndex build_operator_index_2 = createDummyBuildHashOperator(query_plan_.get(),
                                                                                      build_relation_2,
                                                                                      build_attribute_id_2,
                                                                                      join_hash_table_index_2);
  const QueryPlan::DAGNodeIndex probe_operator_index_2 = createDummyHashJoinOperator(query_plan_.get(),
                                                                                     build_relation_2,
                                                                                     probe_relation_2,
                                                                                     probe_attribute_id_2,
                                                                                     join_hash_table_index_2);
  const QueryPlan::DAGNodeIndex build_operator_index_3 = createDummyBuildHashOperator(query_plan_.get(),
                                                                                      build_relation_3,
                                                                                      build_attribute_id_3,
                                                                                      join_hash_table_index_3);
  const QueryPlan::DAGNodeIndex probe_operator_index_3 = createDummyHashJoinOperator(query_plan_.get(),
                                                                                     build_relation_3,
                                                                                     probe_relation_3,
                                                                                     probe_attribute_id_3,
                                                                                     join_hash_table_index_3);

  addDummyHashJoinInfo(execution_heuristics_.get(),
                       build_operator_index_1,
                       probe_operator_index_1,
                       build_relation_1,
                       probe_relation_1,
                       build_attribute_id_1,
                       probe_attribute_id_1,
                       join_hash_table_index_1);
  addDummyHashJoinInfo(execution_heuristics_.get(),
                       build_operator_index_2,
                       probe_operator_index_2,
                       build_relation_2,
                       probe_relation_2,
                       build_attribute_id_2,
                       probe_attribute_id_2,
                       join_hash_table_index_2);
  addDummyHashJoinInfo(execution_heuristics_.get(),
                       build_operator_index_3,
                       probe_operator_index_3,
                       build_relation_3,
                       probe_relation_3,
                       build_attribute_id_3,
                       probe_attribute_id_3,
                       join_hash_table_index_3);

  execution_heuristics_->optimizeExecutionPlan(query_plan_.get(), query_context_proto_.get());

  // Test that no bloom filters were added.
  EXPECT_EQ(0, query_context_proto_->join_hash_tables(0).build_side_bloom_filter_id_size());
  EXPECT_EQ(0, query_context_proto_->join_hash_tables(1).build_side_bloom_filter_id_size());
  EXPECT_EQ(0, query_context_proto_->join_hash_tables(2).build_side_bloom_filter_id_size());
  EXPECT_EQ(0, query_context_proto_->join_hash_tables(0).probe_side_bloom_filters_size());

  // Test that the DAG was not modified at all.
  // Probe operator 1 should not have build operator 1 and build operator 2 added as dependencies.
  auto probe_node_dependencies = query_plan_->getQueryPlanDAG().getDependencies(probe_operator_index_1);
  EXPECT_EQ(0u, probe_node_dependencies.count(build_operator_index_2));
  EXPECT_EQ(0u, probe_node_dependencies.count(build_operator_index_3));
}

}  // namespace optimizer
}  // namespace quickstep
