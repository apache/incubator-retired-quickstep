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

#include <algorithm>
#include <cstddef>
#include <memory>
#include <vector>

#include "gtest/gtest.h"

#include "query_execution/DAGAnalyzer.hpp"
#include "query_execution/tests/MockOperator.hpp"
#include "utility/DAG.hpp"

namespace quickstep {

TEST(DAGAnalyzerTest, ZeroNodeDAGTest) {
  std::unique_ptr<DAG<RelationalOperator, bool>> zero_node_dag;
  zero_node_dag.reset(new DAG<RelationalOperator, bool>());
  DAGAnalyzer analyzer(zero_node_dag.get());
  EXPECT_EQ(0u, analyzer.getNumPipelines());
}

TEST(DAGAnalyzerTest, OneNodeDAGTest) {
  std::unique_ptr<DAG<RelationalOperator, bool>> one_node_dag;
  one_node_dag.reset(new DAG<RelationalOperator, bool>());
  one_node_dag->createNode(new MockOperator(false, false));

  DAGAnalyzer analyzer(one_node_dag.get());
  EXPECT_EQ(1u, analyzer.getNumPipelines());
}

TEST(DAGAnalyzerTest, TwoNodesOnePipelineTest) {
  std::unique_ptr<DAG<RelationalOperator, bool>> dag;
  dag.reset(new DAG<RelationalOperator, bool>());
  const std::size_t n0 = dag->createNode(new MockOperator(false, false));
  const std::size_t n1 = dag->createNode(new MockOperator(false, false));

  dag->createLink(n0, n1, false);
  DAGAnalyzer analyzer(dag.get());
  EXPECT_EQ(1u, analyzer.getNumPipelines());
}

TEST(DAGAnalyzerTest, TwoNodesTwoPipelinesTest) {
  std::unique_ptr<DAG<RelationalOperator, bool>> dag;
  dag.reset(new DAG<RelationalOperator, bool>());
  const std::size_t n0 = dag->createNode(new MockOperator(false, false));
  const std::size_t n1 = dag->createNode(new MockOperator(false, false));

  dag->createLink(n0, n1, true);
  DAGAnalyzer analyzer(dag.get());
  EXPECT_EQ(2u, analyzer.getNumPipelines());
}

TEST(DAGAnalyzerTest, MultipleNodesOnePipelineTest) {
  std::unique_ptr<DAG<RelationalOperator, bool>> dag;
  // Create a long pipeline.
  const std::size_t kNumNodes = 100;
  dag.reset(new DAG<RelationalOperator, bool>());
  std::vector<std::size_t> nodes;
  nodes.resize(kNumNodes);
  // Insert first node.
  const std::size_t curr_node_id = dag->createNode(new MockOperator(false, false));
  nodes.push_back(curr_node_id);
  for (std::size_t i = 1; i < kNumNodes; ++i) {
    const std::size_t node_id = dag->createNode(new MockOperator(false, false));
    const std::size_t previous_node_id = nodes.back();
    dag->createLink(node_id, previous_node_id, false);
    nodes.push_back(node_id);
  }

  DAGAnalyzer analyzer(dag.get());
  EXPECT_EQ(1u, analyzer.getNumPipelines());
}

TEST(DAGAnalyzerTest, VShapedDAGTest) {
  // Test for three nodes in shape V.
  std::unique_ptr<DAG<RelationalOperator, bool>> dag;
  dag.reset(new DAG<RelationalOperator, bool>());
  const std::size_t n0 = dag->createNode(new MockOperator(false, false));
  const std::size_t n1 = dag->createNode(new MockOperator(false, false));
  const std::size_t n2 = dag->createNode(new MockOperator(false, false));

  dag->createLink(n0, n1, true);
  dag->createLink(n2, n1, true);
  DAGAnalyzer analyzer(dag.get());
  // Expect three pipelines, one for each node. n1 is not part of either n0 or
  // n1's pipeline, because it has more than one dependencies.
  EXPECT_EQ(3u, analyzer.getNumPipelines());
}

}  // namespace quickstep
