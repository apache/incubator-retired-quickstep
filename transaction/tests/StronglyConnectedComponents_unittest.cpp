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

#include "transaction/StronglyConnectedComponents.hpp"

#include <vector>

#include "transaction/DirectedGraph.hpp"
#include "transaction/Transaction.hpp"

#include "gtest/gtest.h"

namespace quickstep {

namespace transaction {

class StronglyConnectedComponentsTestWithOneNode : public testing::Test {
 protected:
  using NID = DirectedGraph::node_id;

  StronglyConnectedComponentsTestWithOneNode() {
    wait_for_graph = std::make_unique<DirectedGraph>();
    tid1 = new TransactionId(1);
    nid1 = wait_for_graph->addNodeUnchecked(tid1);

    scc = std::make_unique<StronglyConnectedComponents>(*wait_for_graph);
    scc->findStronglyConnectedComponents();
  }

  std::unique_ptr<DirectedGraph> wait_for_graph;
  std::unique_ptr<StronglyConnectedComponents> scc;

  TransactionId *tid1;

  NID nid1;

  std::uint64_t total_components;
};

class StronglyConnectedComponentsTestWithTwoNodesCycle : public testing::Test {
 protected:
  using NID = DirectedGraph::node_id;

  StronglyConnectedComponentsTestWithTwoNodesCycle() {
    wait_for_graph = std::make_unique<DirectedGraph>();
    tid1 = new TransactionId(1);
    tid2 = new TransactionId(2);

    nid1 = wait_for_graph->addNodeUnchecked(tid1);
    nid2 = wait_for_graph->addNodeUnchecked(tid2);

    wait_for_graph->addEdge(nid1, nid2);
    wait_for_graph->addEdge(nid2, nid1);

    scc = std::make_unique<StronglyConnectedComponents>(*wait_for_graph);
    scc->findStronglyConnectedComponents();
  }

  std::unique_ptr<DirectedGraph> wait_for_graph;
  std::unique_ptr<StronglyConnectedComponents> scc;

  TransactionId *tid1;
  TransactionId *tid2;

  NID nid1;
  NID nid2;

  std::uint64_t total_components;
};

class StronglyConnectedComponentsTest : public testing::Test {
 protected:
  using NID = DirectedGraph::node_id;

  StronglyConnectedComponentsTest() {
    // Creates a graph with predefined structure.
    wait_for_graph = std::make_unique<DirectedGraph>();
    tid1 = new TransactionId(1);
    tid2 = new TransactionId(2);
    tid3 = new TransactionId(3);
    tid4 = new TransactionId(4);
    tid5 = new TransactionId(5);
    tid6 = new TransactionId(6);
    tid7 = new TransactionId(7);
    tid8 = new TransactionId(8);
    tid9 = new TransactionId(9);
    tid10 = new TransactionId(10);
    tid11 = new TransactionId(11);
    tid12 = new TransactionId(12);

    nid1 = wait_for_graph->addNodeUnchecked(tid1);
    nid2 = wait_for_graph->addNodeUnchecked(tid2);
    nid3 = wait_for_graph->addNodeUnchecked(tid3);
    nid4 = wait_for_graph->addNodeUnchecked(tid4);
    nid5 = wait_for_graph->addNodeUnchecked(tid5);
    nid6 = wait_for_graph->addNodeUnchecked(tid6);
    nid7 = wait_for_graph->addNodeUnchecked(tid7);
    nid8 = wait_for_graph->addNodeUnchecked(tid8);
    nid9 = wait_for_graph->addNodeUnchecked(tid9);
    nid10 = wait_for_graph->addNodeUnchecked(tid10);
    nid11 = wait_for_graph->addNodeUnchecked(tid11);
    nid12 = wait_for_graph->addNodeUnchecked(tid12);

    wait_for_graph->addEdge(nid1, nid2);

    wait_for_graph->addEdge(nid2, nid3);
    wait_for_graph->addEdge(nid2, nid4);
    wait_for_graph->addEdge(nid2, nid5);

    wait_for_graph->addEdge(nid3, nid6);

    wait_for_graph->addEdge(nid4, nid5);
    wait_for_graph->addEdge(nid4, nid7);

    wait_for_graph->addEdge(nid5, nid2);
    wait_for_graph->addEdge(nid5, nid6);
    wait_for_graph->addEdge(nid5, nid7);

    wait_for_graph->addEdge(nid6, nid3);
    wait_for_graph->addEdge(nid6, nid8);

    wait_for_graph->addEdge(nid7, nid8);
    wait_for_graph->addEdge(nid7, nid10);

    wait_for_graph->addEdge(nid8, nid7);

    wait_for_graph->addEdge(nid9, nid7);

    wait_for_graph->addEdge(nid10, nid9);
    wait_for_graph->addEdge(nid10, nid11);

    wait_for_graph->addEdge(nid11, nid12);

    wait_for_graph->addEdge(nid12, nid10);

    scc = std::make_unique<StronglyConnectedComponents>(*wait_for_graph);
    scc->findStronglyConnectedComponents();
  }

  std::unique_ptr<DirectedGraph> wait_for_graph;
  std::unique_ptr<StronglyConnectedComponents> scc;

  TransactionId *tid1;
  TransactionId *tid2;
  TransactionId *tid3;
  TransactionId *tid4;
  TransactionId *tid5;
  TransactionId *tid6;
  TransactionId *tid7;
  TransactionId *tid8;
  TransactionId *tid9;
  TransactionId *tid10;
  TransactionId *tid11;
  TransactionId *tid12;

  NID nid1;
  NID nid2;
  NID nid3;
  NID nid4;
  NID nid5;
  NID nid6;
  NID nid7;
  NID nid8;
  NID nid9;
  NID nid10;
  NID nid11;
  NID nid12;

  std::uint64_t nid1_component;
  std::uint64_t nid2_component;
  std::uint64_t nid3_component;
  std::uint64_t nid4_component;
  std::uint64_t nid5_component;
  std::uint64_t nid6_component;
  std::uint64_t nid7_component;
  std::uint64_t nid8_component;
  std::uint64_t nid9_component;
  std::uint64_t nid10_component;
  std::uint64_t nid11_component;
  std::uint64_t nid12_component;

  std::uint64_t total_components;
};

TEST_F(StronglyConnectedComponentsTestWithOneNode, TotalNumberOfComponents) {
  total_components = scc->getTotalComponents();

  EXPECT_EQ(1u, total_components);
}

TEST_F(StronglyConnectedComponentsTestWithOneNode, GetComponentId) {
  std::uint64_t nid1_component = scc->getComponentId(nid1);
  EXPECT_EQ(0u, nid1_component);
}


TEST_F(StronglyConnectedComponentsTestWithOneNode, GetComponentsMapping) {
  std::unordered_map<std::uint64_t, std::vector<NID>> mapping
      = scc->getComponentMapping();
  std::vector<NID> component_no_0 = mapping[0];
  EXPECT_EQ(1u, component_no_0.size());
}

TEST_F(StronglyConnectedComponentsTestWithTwoNodesCycle, TotalNumberOfComponents) {
  total_components = scc->getTotalComponents();

  EXPECT_EQ(1u, total_components);
}

TEST_F(StronglyConnectedComponentsTestWithTwoNodesCycle, GetComponentId) {
  EXPECT_EQ(0u, scc->getComponentId(nid1));
  EXPECT_EQ(0u, scc->getComponentId(nid2));
}

TEST_F(StronglyConnectedComponentsTestWithTwoNodesCycle, GetComponentsMapping) {
  std::unordered_map<std::uint64_t, std::vector<NID>>
      mapping = scc->getComponentMapping();
  std::vector<NID> component_no_0 = mapping[0];
  EXPECT_EQ(2u, component_no_0.size());
}

TEST_F(StronglyConnectedComponentsTest, TotalNumberOfComponents) {
  total_components = scc->getTotalComponents();
  EXPECT_EQ(4u, total_components);
}

TEST_F(StronglyConnectedComponentsTest, GetComponentId) {
  nid1_component = scc->getComponentId(nid1);
  nid2_component = scc->getComponentId(nid2);
  nid3_component = scc->getComponentId(nid3);
  nid4_component = scc->getComponentId(nid4);
  nid5_component = scc->getComponentId(nid5);
  nid6_component = scc->getComponentId(nid6);
  nid7_component = scc->getComponentId(nid7);
  nid8_component = scc->getComponentId(nid8);
  nid9_component = scc->getComponentId(nid9);
  nid10_component = scc->getComponentId(nid10);
  nid11_component = scc->getComponentId(nid11);
  nid12_component = scc->getComponentId(nid12);

  EXPECT_EQ(3u, nid1_component);

  EXPECT_EQ(2u, nid2_component);
  EXPECT_EQ(2u, nid4_component);
  EXPECT_EQ(2u, nid5_component);

  EXPECT_EQ(1u, nid3_component);
  EXPECT_EQ(1u, nid6_component);

  EXPECT_EQ(0u, nid7_component);
  EXPECT_EQ(0u, nid8_component);
  EXPECT_EQ(0u, nid9_component);
  EXPECT_EQ(0u, nid10_component);
  EXPECT_EQ(0u, nid11_component);
  EXPECT_EQ(0u, nid12_component);
}

TEST_F(StronglyConnectedComponentsTest, GetComponentsMapping) {
  std::unordered_map<std::uint64_t, std::vector<NID>>
      mapping = scc->getComponentMapping();

  std::vector<NID> component_no_0 = mapping[0];
  std::vector<NID> component_no_1 = mapping[1];
  std::vector<NID> component_no_2 = mapping[2];
  std::vector<NID> component_no_3 = mapping[3];

  EXPECT_EQ(6u, component_no_0.size());
  EXPECT_EQ(2u, component_no_1.size());
  EXPECT_EQ(3u, component_no_2.size());
  EXPECT_EQ(1u, component_no_3.size());
}

}  // namespace transaction

}  // namespace quickstep
