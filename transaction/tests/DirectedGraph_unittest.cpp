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

TEST(DirectedGraphTest, AddNode) {
  DirectedGraph wait_for_graph;
  TransactionId *tid3 = new TransactionId(3);
  TransactionId *tid4 = new TransactionId(4);
  TransactionId *tid5 = new TransactionId(5);
  TransactionId *tid6 = new TransactionId(6);

  EXPECT_EQ(0u, wait_for_graph.count());

  wait_for_graph.addNodeUnchecked(tid3);

  EXPECT_EQ(1u, wait_for_graph.count());

  wait_for_graph.addNodeUnchecked(tid4);

  EXPECT_EQ(2u, wait_for_graph.count());

  wait_for_graph.addNodeUnchecked(tid5);
  wait_for_graph.addNodeUnchecked(tid6);

  EXPECT_EQ(4u, wait_for_graph.count());
}

TEST(DirectedGraphTest, AddEdge) {
  DirectedGraph wait_for_graph;
  TransactionId *tid3 = new TransactionId(3);
  TransactionId *tid4 = new TransactionId(4);
  TransactionId *tid5 = new TransactionId(5);
  TransactionId *tid6 = new TransactionId(6);

  using NID = DirectedGraph::node_id;
  NID nid3 = wait_for_graph.addNodeUnchecked(tid3);
  NID nid6 = wait_for_graph.addNodeUnchecked(tid6);
  NID nid4 = wait_for_graph.addNodeUnchecked(tid4);
  NID nid5 = wait_for_graph.addNodeUnchecked(tid5);

  wait_for_graph.addEdgeUnchecked(nid3, nid5);
  wait_for_graph.addEdgeUnchecked(nid6, nid4);
  wait_for_graph.addEdgeUnchecked(nid3, nid6);
  wait_for_graph.addEdgeUnchecked(nid4, nid6);

  EXPECT_TRUE(wait_for_graph.hasEdge(nid3, nid5));
  EXPECT_TRUE(wait_for_graph.hasEdge(nid6, nid4));
  EXPECT_TRUE(wait_for_graph.hasEdge(nid4, nid6));
  EXPECT_TRUE(wait_for_graph.hasEdge(nid3, nid6));

  EXPECT_FALSE(wait_for_graph.hasEdge(nid5, nid3));
  EXPECT_FALSE(wait_for_graph.hasEdge(nid6, nid3));
  EXPECT_FALSE(wait_for_graph.hasEdge(nid4, nid5));
}

TEST(DirectedGraphTest, GetAdjacentNodes) {
  DirectedGraph wait_for_graph;
  TransactionId *tid3 = new TransactionId(3);
  TransactionId *tid4 = new TransactionId(4);
  TransactionId *tid5 = new TransactionId(5);
  TransactionId *tid6 = new TransactionId(6);

  using NID = DirectedGraph::node_id;
  NID nid3 = wait_for_graph.addNodeUnchecked(tid3);
  NID nid6 = wait_for_graph.addNodeUnchecked(tid6);
  NID nid4 = wait_for_graph.addNodeUnchecked(tid4);
  NID nid5 = wait_for_graph.addNodeUnchecked(tid5);

  std::vector<NID> result1 = wait_for_graph.getAdjacentNodes(nid3);
  EXPECT_EQ(0u, result1.size());

  wait_for_graph.addEdgeUnchecked(nid3, nid4);
  wait_for_graph.addEdgeUnchecked(nid3, nid5);

  std::vector<NID> result2 = wait_for_graph.getAdjacentNodes(nid3);
  EXPECT_EQ(2u, result2.size());

  wait_for_graph.addEdgeUnchecked(nid3, nid6);
  std::vector<NID> result3 = wait_for_graph.getAdjacentNodes(nid3);
  EXPECT_EQ(3u, result3.size());
}

}  // namespace transaction

}  // namespace quickstep
