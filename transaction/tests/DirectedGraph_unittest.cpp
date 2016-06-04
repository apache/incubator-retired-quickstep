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

#include "transaction/DirectedGraph.hpp"

#include <vector>

#include "transaction/Transaction.hpp"

#include "gtest/gtest.h"

namespace quickstep {
namespace transaction {

class DirectedGraphTest : public ::testing::Test {
 protected:
  DirectedGraphTest()
      : tid3_(3),
        tid4_(4),
        tid5_(5),
        tid6_(6) {
  }

  DirectedGraph wait_for_graph_;
  transaction_id tid3_;
  transaction_id tid4_;
  transaction_id tid5_;
  transaction_id tid6_;
};

TEST_F(DirectedGraphTest, AddNode) {
  // The nodes are not added yet, total no of nodesshould be zero.
  EXPECT_EQ(0u, wait_for_graph_.getNumNodes());

  wait_for_graph_.addNodeUnchecked(tid3_);

  // One node is added.
  EXPECT_EQ(1u, wait_for_graph_.getNumNodes());

  wait_for_graph_.addNodeUnchecked(tid4_);

  // Another node is added.
  EXPECT_EQ(2u, wait_for_graph_.getNumNodes());

  wait_for_graph_.addNodeUnchecked(tid5_);
  wait_for_graph_.addNodeUnchecked(tid6_);

  // Total no of nodes should be 4 right now.
  EXPECT_EQ(4u, wait_for_graph_.getNumNodes());
}

TEST_F(DirectedGraphTest, AddEdge) {
  DirectedGraph::node_id nid3 = wait_for_graph_.addNodeUnchecked(tid3_);
  DirectedGraph::node_id nid6 = wait_for_graph_.addNodeUnchecked(tid6_);
  DirectedGraph::node_id nid4 = wait_for_graph_.addNodeUnchecked(tid4_);
  DirectedGraph::node_id nid5 = wait_for_graph_.addNodeUnchecked(tid5_);

  // Add edges.
  wait_for_graph_.addEdgeUnchecked(nid3, nid5);
  wait_for_graph_.addEdgeUnchecked(nid6, nid4);
  wait_for_graph_.addEdgeUnchecked(nid3, nid6);
  wait_for_graph_.addEdgeUnchecked(nid4, nid6);

  // Check whether the edges are already there.
  EXPECT_TRUE(wait_for_graph_.hasEdge(nid3, nid5));
  EXPECT_TRUE(wait_for_graph_.hasEdge(nid6, nid4));
  EXPECT_TRUE(wait_for_graph_.hasEdge(nid4, nid6));
  EXPECT_TRUE(wait_for_graph_.hasEdge(nid3, nid6));

  // Check non-existent edges.
  EXPECT_FALSE(wait_for_graph_.hasEdge(nid5, nid3));
  EXPECT_FALSE(wait_for_graph_.hasEdge(nid6, nid3));
  EXPECT_FALSE(wait_for_graph_.hasEdge(nid4, nid5));
}

TEST_F(DirectedGraphTest, GetAdjacentNodes) {
  // Add 4 disconnected nodes to the graph.
  DirectedGraph::node_id nid3 = wait_for_graph_.addNodeUnchecked(tid3_);
  DirectedGraph::node_id nid6 = wait_for_graph_.addNodeUnchecked(tid6_);
  DirectedGraph::node_id nid4 = wait_for_graph_.addNodeUnchecked(tid4_);
  DirectedGraph::node_id nid5 = wait_for_graph_.addNodeUnchecked(tid5_);

  std::vector<DirectedGraph::node_id> result1 = wait_for_graph_.getAdjacentNodes(nid3);
  // nid3 has no edge to other nodes.
  EXPECT_EQ(0u, result1.size());

  // Now nid3 has outgoing edge to nid4 and nid5.
  wait_for_graph_.addEdgeUnchecked(nid3, nid4);
  wait_for_graph_.addEdgeUnchecked(nid3, nid5);

  std::vector<DirectedGraph::node_id> result2 = wait_for_graph_.getAdjacentNodes(nid3);
  // Therefore, number of outgoing edges from nid3 is 2.
  EXPECT_EQ(2u, result2.size());

  // Add an edge from nid3 to nid6.
  wait_for_graph_.addEdgeUnchecked(nid3, nid6);
  std::vector<DirectedGraph::node_id> result3 = wait_for_graph_.getAdjacentNodes(nid3);

  // Now there are 3 outgoing edges.
  EXPECT_EQ(3u, result3.size());

  // Again add edge from nid3 to nid6.
  wait_for_graph_.addEdgeUnchecked(nid3, nid6);
  std::vector<DirectedGraph::node_id> result4 = wait_for_graph_.getAdjacentNodes(nid3);
  // Since we have already add same edge before, number of edges are still 3.
  EXPECT_EQ(3u, result4.size());
}

}  // namespace transaction
}  // namespace quickstep
