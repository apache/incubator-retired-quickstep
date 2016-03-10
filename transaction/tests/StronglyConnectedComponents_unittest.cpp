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

#include <cstdint>
#include <utility>
#include <vector>

#include "transaction/DirectedGraph.hpp"
#include "transaction/Transaction.hpp"

#include "gtest/gtest.h"

namespace quickstep {

namespace transaction {

class GraphConfiguration {
 public:
  GraphConfiguration(DirectedGraph *graph,
                     std::size_t no_transaction,
                     const std::vector<std::pair<transaction_id,
                                                 transaction_id>> &mapping)
    : graph_(graph) {
    for (std::size_t index = 0; index < no_transaction; ++index) {
      std::unique_ptr<transaction_id> tid =
        std::make_unique<transaction_id>(transaction_id(index));
      transaction_list_.push_back(*tid);
      DirectedGraph::node_id nid = graph->addNodeUnchecked(tid.release());
      node_id_list_.push_back(nid);
    }

    for (const std::pair<transaction_id, transaction_id> &edge : mapping) {
      transaction_id pending = edge.first;
      transaction_id owner = edge.second;
      graph_->addEdgeUnchecked(pending, owner);
    }
  }

  const std::vector<DirectedGraph::node_id>& getNodesList() const {
    return node_id_list_;
  }

 private:
  std::vector<transaction_id> transaction_list_;
  std::vector<DirectedGraph::node_id> node_id_list_;
  DirectedGraph *graph_;
};

class StronglyConnectedComponentsTestWithOneNode : public testing::Test {
 protected:
  StronglyConnectedComponentsTestWithOneNode() {
    wait_for_graph_ = std::make_unique<DirectedGraph>();
    std::vector<std::pair<transaction_id, transaction_id>> edge_mapping;

    // Configure the graph with just 1 node.
    graph_configuration_ =
      std::make_unique<GraphConfiguration>(wait_for_graph_.get(),
                                           1,
                                           edge_mapping);
    scc_ = std::make_unique<StronglyConnectedComponents>(*wait_for_graph_);
  }

  std::unique_ptr<GraphConfiguration> graph_configuration_;
  std::unique_ptr<DirectedGraph> wait_for_graph_;
  std::unique_ptr<StronglyConnectedComponents> scc_;
};

class StronglyConnectedComponentsTestWithTwoNodesCycle : public testing::Test {
 protected:
  StronglyConnectedComponentsTestWithTwoNodesCycle() {
    wait_for_graph_ = std::make_unique<DirectedGraph>();

    // Create 2 nodes with cycle.
    std::vector<std::pair<transaction_id, transaction_id>> edge_mapping =
      {{0, 1},
       {1, 0}};
    graph_configuration_ =
      std::make_unique<GraphConfiguration>(wait_for_graph_.get(),
                                           2,
                                           edge_mapping);

    // Run the SCC algorithm.
    scc_ = std::make_unique<StronglyConnectedComponents>(*wait_for_graph_);
  }

  std::unique_ptr<GraphConfiguration> graph_configuration_;
  std::unique_ptr<DirectedGraph> wait_for_graph_;
  std::unique_ptr<StronglyConnectedComponents> scc_;
};

class StronglyConnectedComponentsTest : public testing::Test {
 protected:
  StronglyConnectedComponentsTest() {
    // Prepare the graph.
    wait_for_graph_ = std::make_unique<DirectedGraph>();

    // Create edges.
    std::vector<std::pair<transaction_id, transaction_id>> edge_mapping =
      {{0, 1},
       {1, 2}, {1, 3}, {1, 4},
       {2, 5},
       {3, 4}, {3, 6},
       {4, 1}, {4, 5}, {4, 6},
       {5, 2}, {5, 7},
       {6, 7}, {6, 9},
       {7, 6},
       {8, 6},
       {9, 8}, {9, 10},
       {10, 11}, {11, 9}};

    // Configure the graph.
    graph_configuration_ =
      std::make_unique<GraphConfiguration>(wait_for_graph_.get(),
                                           12,
                                           edge_mapping);

    // Run the SCC algorithm.
    scc_ = std::make_unique<StronglyConnectedComponents>(*wait_for_graph_);
  }

  std::unique_ptr<GraphConfiguration> graph_configuration_;
  std::unique_ptr<DirectedGraph> wait_for_graph_;
  std::unique_ptr<StronglyConnectedComponents> scc_;
};

TEST_F(StronglyConnectedComponentsTestWithOneNode, TotalNumberOfComponents) {
  std::uint64_t total_components = scc_->getTotalComponents();

  EXPECT_EQ(1u, total_components);
}

TEST_F(StronglyConnectedComponentsTestWithOneNode, GetComponentId) {
  std::uint64_t nid0_component =
    scc_->getComponentId(graph_configuration_->getNodesList()[0]);

  EXPECT_EQ(0u, nid0_component);
}


TEST_F(StronglyConnectedComponentsTestWithOneNode, GetComponentsMapping) {
  std::unordered_map<std::uint64_t, std::vector<DirectedGraph::node_id>>
    mapping = scc_->getComponentMapping();
  std::vector<DirectedGraph::node_id> component_no_0 = mapping[0];

  EXPECT_EQ(1u, component_no_0.size());
}

TEST_F(StronglyConnectedComponentsTestWithTwoNodesCycle, TotalNumberOfComponents) {
  std::uint64_t total_components = scc_->getTotalComponents();

  EXPECT_EQ(1u, total_components);
}

TEST_F(StronglyConnectedComponentsTestWithTwoNodesCycle, GetComponentId) {
  DirectedGraph::node_id nid0 = DirectedGraph::node_id(0);
  DirectedGraph::node_id nid1 = DirectedGraph::node_id(1);

  // Since two nodes make a cycle, they should have same component id.
  EXPECT_EQ(0u, scc_->getComponentId(nid0));
  EXPECT_EQ(0u, scc_->getComponentId(nid1));
}

TEST_F(StronglyConnectedComponentsTestWithTwoNodesCycle, GetComponentsMapping) {
  std::unordered_map<std::uint64_t, std::vector<DirectedGraph::node_id>>
      mapping = scc_->getComponentMapping();
  std::vector<DirectedGraph::node_id> component_no_0 = mapping[0];

  // Since there are 2 nodes, and they are in the same SC component,
  // the component size is 2.
  EXPECT_EQ(2u, component_no_0.size());
}

TEST_F(StronglyConnectedComponentsTest, TotalNumberOfComponents) {
  std::size_t total_components = scc_->getTotalComponents();

  EXPECT_EQ(4u, total_components);
}

TEST_F(StronglyConnectedComponentsTest, GetComponentId) {
  std::vector<DirectedGraph::node_id> nid_list;
  for (DirectedGraph::node_id nid = 0;
       nid < wait_for_graph_->getNumNodes();
       ++nid) {
    nid_list.push_back(scc_->getComponentId(nid));
  }

  // Check the result are as expected.
  EXPECT_EQ(3u, nid_list[0]);

  EXPECT_EQ(2u, nid_list[1]);
  EXPECT_EQ(2u, nid_list[3]);
  EXPECT_EQ(2u, nid_list[4]);

  EXPECT_EQ(1u, nid_list[2]);
  EXPECT_EQ(1u, nid_list[5]);

  EXPECT_EQ(0u, nid_list[6]);
  EXPECT_EQ(0u, nid_list[7]);
  EXPECT_EQ(0u, nid_list[8]);
  EXPECT_EQ(0u, nid_list[9]);
  EXPECT_EQ(0u, nid_list[10]);
  EXPECT_EQ(0u, nid_list[11]);
}

TEST_F(StronglyConnectedComponentsTest, GetComponentsMapping) {
  std::unordered_map<std::uint64_t, std::vector<DirectedGraph::node_id>>
      mapping = scc_->getComponentMapping();

  // Check the components' size are OK.
  EXPECT_EQ(6u, mapping[0].size());
  EXPECT_EQ(2u, mapping[1].size());
  EXPECT_EQ(3u, mapping[2].size());
  EXPECT_EQ(1u, mapping[3].size());
}

}  // namespace transaction

}  // namespace quickstep
