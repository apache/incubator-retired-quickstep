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
#include <utility>
#include <vector>

#include "utility/DAG.hpp"

#include "gtest/gtest.h"

using std::find;
using std::pair;
using std::vector;

namespace quickstep {

class DummyPayload {
 public:
  explicit DummyPayload(const int payload_value)
      : payload_value_(payload_value) {
  }

  inline int getPayloadValue() const {
    return payload_value_;
  }

 private:
  int payload_value_;
};

TEST(DAGTest, AddNodeTest) {
  const int kPayloadCount = 300;
  DAG<DummyPayload, int> dag_;

  // Create a DAG with 300 nodes and test if the index assignment is correct.
  for (int index = 0; index < kPayloadCount; ++index) {
    EXPECT_EQ(static_cast<std::size_t>(index),
              dag_.createNode(new DummyPayload(index)));
  }
}

TEST(DAGTest, PayloadTest) {
  const int kPayloadCount = 300;
  DAG<DummyPayload, int> dag_;

  // Create a DAG with 300 nodes and test if the payload is returned correctly.
  for (int index = 0; index < kPayloadCount; ++index) {
    EXPECT_EQ(static_cast<std::size_t>(index),
              dag_.createNode(new DummyPayload(index + 5)));
  }

  // If all indices are correct, check the payload.
  for (int index = 0; index < kPayloadCount; ++index) {
    EXPECT_EQ(index + 5, dag_.getNodePayload(index).getPayloadValue());
    EXPECT_EQ(index + 5, dag_.getNodePayloadMutable(index)->getPayloadValue());
  }
}

TEST(DAGTest, SelfCycleTest) {
  DAG<DummyPayload, int> dag_;

  // No cycle in empty DAG.
  EXPECT_FALSE(dag_.hasCycle());

  ASSERT_EQ(0u, dag_.createNode(new DummyPayload(34)));

  // No cycle with single node.
  EXPECT_FALSE(dag_.hasCycle());

  ASSERT_EQ(1u, dag_.createNode(new DummyPayload(44)));

  dag_.createLink(0, 1, 1);

  // This should test createLink function too.
  EXPECT_FALSE(dag_.hasCycle());

  dag_.createLink(0, 0, 1);

  EXPECT_TRUE(dag_.hasCycle());
}

TEST(DAGTest, TwoNodesCycleTest) {
  DAG<DummyPayload, int> dag_;

  // Two nodes with cycle.
  ASSERT_EQ(0u, dag_.createNode(new DummyPayload(2)));
  ASSERT_EQ(1u, dag_.createNode(new DummyPayload(3)));

  dag_.createLink(0, 1, 2);

  EXPECT_FALSE(dag_.hasCycle());

  dag_.createLink(1, 0, 3);

  EXPECT_TRUE(dag_.hasCycle());
}

TEST(DAGTest, CycleExistenceTest) {
  const int kNodeSize = 5;
  DAG<DummyPayload, int> dag_;

  for (int node_index = 0; node_index < kNodeSize; ++node_index) {
    ASSERT_EQ(static_cast<std::size_t>(node_index),
              dag_.createNode(new DummyPayload(node_index)));
  }

  /*
   * 0 1
   * \ /
   *  2
   * / \
   * 3->4
   *
   */

  dag_.createLink(0, 2, 3);
  dag_.createLink(1, 2, 4);
  dag_.createLink(2, 3, 3);
  dag_.createLink(2, 4, 4);
  dag_.createLink(3, 4, 5);

  EXPECT_FALSE(dag_.hasCycle());

  dag_.createLink(4, 3, 1);
  EXPECT_TRUE(dag_.hasCycle());
}

TEST(DAGTest, FiveNodesCycleTest) {
  const int kNodeSize = 5;
  DAG<DummyPayload, int> dag_;

  for (int node_index = 0; node_index < kNodeSize; ++node_index) {
    ASSERT_EQ(static_cast<std::size_t>(node_index),
              dag_.createNode(new DummyPayload(node_index)));
  }

  /*
   * 1->2->3
   * ^     |
   * |     v
   * 0 <-- 4
   *
   */

  dag_.createLink(0, 1, 0);
  dag_.createLink(1, 2, 0);
  dag_.createLink(2, 3, 0);
  dag_.createLink(3, 4, 0);
  dag_.createLink(4, 0, 1);

  EXPECT_TRUE(dag_.hasCycle());

  dag_.createLink(3, 0, 3);

  EXPECT_TRUE(dag_.hasCycle());
}

TEST(DAGTest, DisconnectedComponentsNoCycleTest) {
  const int kNodeSize = 5;
  DAG<DummyPayload, int> dag_;

  // A graph with 5 nodes. Nodes 1, 2, 3 and 4 are put in one component. Node 0
  // is in another component. Both the components are acyclic.
  for (int node_index = 0; node_index < kNodeSize; ++node_index) {
    ASSERT_EQ(static_cast<std::size_t>(node_index),
              dag_.createNode(new DummyPayload(node_index)));
  }

  // All 5 nodes are disconnected.
  EXPECT_FALSE(dag_.hasCycle());

  dag_.createLink(1, 2, 3);
  dag_.createLink(2, 3, 2);
  dag_.createLink(3, 4, 1);

  // Node 0 is in a separate component.
  EXPECT_FALSE(dag_.hasCycle());
}

TEST(DAGTest, DisconnectedComponentsWithCycleTest) {
  const int kNodeSize = 6;
  DAG<DummyPayload, int> dag_;

  // A graph with 6 nodes. Nodes 1, 2 and 3 are put in one component. Nodes 0,
  // 4 and 5 are in another component. Both the components have cycle within
  // themselves.
  for (int node_index = 0; node_index < kNodeSize; ++node_index) {
    ASSERT_EQ(static_cast<std::size_t>(node_index),
              dag_.createNode(new DummyPayload(node_index)));
  }

  dag_.createLink(1, 2, 4);
  dag_.createLink(2, 3, 5);
  dag_.createLink(3, 2, 2);

  // Component with nodes 1, 2 and 3 is cyclic.
  EXPECT_TRUE(dag_.hasCycle());

  dag_.createLink(0, 4, 3);

  EXPECT_TRUE(dag_.hasCycle());

  dag_.createLink(4, 5, 2);
  // Make the second component with nodes 0, 4 and 5 cyclic.
  dag_.createLink(5, 0, 4);

  EXPECT_TRUE(dag_.hasCycle());
}

TEST(DAGTest, SixNodeStagesTest) {
  const int kNodeSize = 6;
  DAG<DummyPayload, int> dag_;

  for (int node_index = 0; node_index < kNodeSize; ++node_index) {
    ASSERT_EQ(static_cast<std::size_t>(node_index),
              dag_.createNode(new DummyPayload(node_index)));
  }

  /*
   * 0 1  2
   * \ /  /
   *  3  /
   *   \/
   *   4
   *   |
   *   5
   */

  dag_.createLink(0, 3, 0);
  dag_.createLink(1, 3, 1);
  dag_.createLink(2, 4, 2);
  dag_.createLink(3, 4, 2);
  dag_.createLink(4, 5, 1);

  vector<pair<vector<DAG<DummyPayload, int>::size_type_nodes>,
      vector<DAG<DummyPayload, int>::size_type_nodes> > > stages = dag_.computeStageSequence();

  ASSERT_EQ(4u, stages.size());
  // stages[0]
  EXPECT_NE(find(stages[0].first.begin(), stages[0].first.end(), 0), stages[0].first.end());
  EXPECT_NE(find(stages[0].first.begin(), stages[0].first.end(), 1), stages[0].first.end());
  EXPECT_NE(find(stages[0].first.begin(), stages[0].first.end(), 2), stages[0].first.end());
  // Check if there's no other element in stages[0].first than the ones we've
  // tested.
  EXPECT_EQ(3u, stages[0].first.size());

  EXPECT_NE(find(stages[0].second.begin(), stages[0].second.end(), 0), stages[0].second.end());
  EXPECT_NE(find(stages[0].second.begin(), stages[0].second.end(), 1), stages[0].second.end());
  EXPECT_EQ(2u, stages[0].second.size());

  // stages[1]
  EXPECT_NE(find(stages[1].first.begin(), stages[1].first.end(), 3), stages[1].first.end());
  EXPECT_EQ(1u, stages[1].first.size());

  EXPECT_NE(find(stages[1].second.begin(), stages[1].second.end(), 2), stages[1].second.end());
  EXPECT_NE(find(stages[1].second.begin(), stages[1].second.end(), 3), stages[1].second.end());
  EXPECT_EQ(2u, stages[1].second.size());

  // stages[2]
  EXPECT_NE(find(stages[2].first.begin(), stages[2].first.end(), 4), stages[2].first.end());
  EXPECT_EQ(1u, stages[2].first.size());

  EXPECT_NE(find(stages[2].second.begin(), stages[2].second.end(), 4), stages[2].second.end());
  EXPECT_EQ(1u, stages[2].second.size());

  // stages[3]
  EXPECT_NE(find(stages[3].first.begin(), stages[3].first.end(), 5), stages[3].first.end());
  EXPECT_EQ(1u, stages[3].first.size());

  EXPECT_NE(find(stages[3].second.begin(), stages[3].second.end(), 5), stages[3].second.end());
  EXPECT_EQ(1u, stages[3].second.size());
}

TEST(DAGTest, DiamondNoCycleStagesTest) {
  const int kNodeSize = 5;
  DAG<DummyPayload, int> dag_;

  for (int node_index = 0; node_index < kNodeSize; ++node_index) {
    ASSERT_EQ(static_cast<std::size_t>(node_index),
              dag_.createNode(new DummyPayload(node_index)));
  }

  /*
   *    0
   *   / \
   *  v   v
   *  1   2
   *   \ /
   *    v
   *    3
   *    |
   *    v
   *    4
   *
   */

  dag_.createLink(0, 1, 2);
  dag_.createLink(0, 2, 1);
  dag_.createLink(1, 3, 1);
  dag_.createLink(2, 3, 1);
  dag_.createLink(3, 4, 1);

  ASSERT_FALSE(dag_.hasCycle());

  vector<pair<vector<DAG<DummyPayload, int>::size_type_nodes>,
      vector<DAG<DummyPayload, int>::size_type_nodes> > > stages = dag_.computeStageSequence();

  ASSERT_EQ(4u, stages.size());

  // stages[0]
  EXPECT_NE(find(stages[0].first.begin(), stages[0].first.end(), 0), stages[0].first.end());
  EXPECT_EQ(1u, stages[0].first.size());

  EXPECT_NE(find(stages[0].second.begin(), stages[0].second.end(), 0), stages[0].second.end());
  EXPECT_EQ(1u, stages[0].second.size());

  // stages[1]
  EXPECT_NE(find(stages[1].first.begin(), stages[1].first.end(), 1), stages[1].first.end());
  EXPECT_NE(find(stages[1].first.begin(), stages[1].first.end(), 2), stages[1].first.end());
  EXPECT_EQ(2u, stages[1].first.size());

  EXPECT_NE(find(stages[1].second.begin(), stages[1].second.end(), 1), stages[1].second.end());
  EXPECT_NE(find(stages[1].second.begin(), stages[1].second.end(), 2), stages[1].second.end());
  EXPECT_EQ(2u, stages[1].second.size());

  // stages[2]
  EXPECT_NE(find(stages[2].first.begin(), stages[2].first.end(), 3), stages[2].first.end());
  EXPECT_EQ(1u, stages[2].first.size());

  EXPECT_NE(find(stages[2].second.begin(), stages[2].second.end(), 3), stages[2].second.end());
  EXPECT_EQ(1u, stages[2].second.size());
  // stages[3]
  EXPECT_NE(find(stages[3].first.begin(), stages[3].first.end(), 4), stages[3].first.end());
  EXPECT_EQ(1u, stages[3].first.size());

  EXPECT_NE(find(stages[3].second.begin(), stages[3].second.end(), 4), stages[3].second.end());
  EXPECT_EQ(1u, stages[3].second.size());
}

TEST(DAGTest, DiamondCycleExistenceTest) {
  const int kNodeSize = 5;
  DAG<DummyPayload, int> dag_;

  // A cycle is present within the diamond shape
  for (int node_index = 0; node_index < kNodeSize; ++node_index) {
    ASSERT_EQ(static_cast<std::size_t>(node_index),
              dag_.createNode(new DummyPayload(node_index)));
  }

  /*
   *     0
   *    / ^
   *   v   \
   *   1    2
   *   \   /
   *    v /
   *     3
   *     |
   *     v
   *     4
   *
   */

  dag_.createLink(0, 1, 1);
  dag_.createLink(1, 3, 2);
  dag_.createLink(3, 2, 1);
  dag_.createLink(3, 4, 2);
  dag_.createLink(2, 0, 1);

  ASSERT_TRUE(dag_.hasCycle());
}

TEST(DAGTest, DisconnectedComponentsNoCycleStagesTest) {
  const int kNodeSize = 5;
  DAG<DummyPayload, int> dag_;

  // A graph with 5 nodes. Nodes 1, 2, 3 and 4 are put in one component. Node 0
  // is in another component. Both the components are acyclic. As node 0 has no
  // dependencies, it can be processed in any stage, starting from 0 till 4.
  for (int node_index = 0; node_index < kNodeSize; ++node_index) {
    ASSERT_EQ(static_cast<std::size_t>(node_index),
              dag_.createNode(new DummyPayload(node_index)));
  }

  dag_.createLink(1, 2, 0);
  dag_.createLink(2, 3, 1);
  dag_.createLink(3, 4, 2);

  EXPECT_FALSE(dag_.hasCycle());

  vector<pair<vector<DAG<DummyPayload, int>::size_type_nodes>,
      vector<DAG<DummyPayload, int>::size_type_nodes> > > stages = dag_.computeStageSequence();

  ASSERT_EQ(4u, stages.size());

  // stages[0] Node 0 can begin execution here.
  EXPECT_NE(find(stages[0].first.begin(), stages[0].first.end(), 0), stages[0].first.end());
  EXPECT_NE(find(stages[0].first.begin(), stages[0].first.end(), 1), stages[0].first.end());
  EXPECT_EQ(2u, stages[0].first.size());

  EXPECT_NE(find(stages[0].second.begin(), stages[0].second.end(), 1), stages[0].second.end());
  EXPECT_EQ(1u, stages[0].second.size());

  // stages[1]
  EXPECT_NE(find(stages[1].first.begin(), stages[1].first.end(), 2), stages[1].first.end());
  EXPECT_EQ(1u, stages[1].first.size());

  EXPECT_NE(find(stages[1].second.begin(), stages[1].second.end(), 2), stages[1].second.end());
  EXPECT_EQ(1u, stages[1].second.size());

  // stages[2]
  EXPECT_NE(find(stages[2].first.begin(), stages[2].first.end(), 3), stages[2].first.end());
  EXPECT_EQ(1u, stages[2].first.size());

  EXPECT_NE(find(stages[2].second.begin(), stages[2].second.end(), 3), stages[2].second.end());
  EXPECT_EQ(1u, stages[2].second.size());

  // stages[3] Node 0 must finish execution here.
  EXPECT_NE(find(stages[3].first.begin(), stages[3].first.end(), 4), stages[3].first.end());
  EXPECT_EQ(1u, stages[3].first.size());

  EXPECT_NE(find(stages[3].second.begin(), stages[3].second.end(), 0), stages[3].second.end());
  EXPECT_NE(find(stages[3].second.begin(), stages[3].second.end(), 4), stages[3].second.end());
  EXPECT_EQ(2u, stages[3].second.size());
}

TEST(DAGTest, LinkMetadataIntTest) {
  DAG<DummyPayload, int> dag_;
  const int kNodeSize = 2;

  for (int index = 0; index < kNodeSize; ++index) {
    EXPECT_EQ(static_cast<std::size_t>(index),
              dag_.createNode(new DummyPayload(index)));
  }

  dag_.createLink(0, 1, 0);
  EXPECT_EQ(0, dag_.getLinkMetadata(0, 1));
  dag_.createLink(1, 0, 1);
  EXPECT_EQ(1, dag_.getLinkMetadata(1, 0));

  // Change the weight of the link 0->1
  dag_.setLinkMetadata(0, 1, 2);
  EXPECT_EQ(2, dag_.getLinkMetadata(0, 1));
}

TEST(DAGTest, LinkMetadataBoolTest) {
  DAG<DummyPayload, bool> dag_;
  const int kNodeSize = 2;

  for (int index = 0; index < kNodeSize; ++index) {
    EXPECT_EQ(static_cast<std::size_t>(index),
              dag_.createNode(new DummyPayload(index)));
  }

  dag_.createLink(0, 1, false);
  EXPECT_FALSE(dag_.getLinkMetadata(0, 1));
  dag_.createLink(1, 0, false);
  EXPECT_FALSE(dag_.getLinkMetadata(1, 0));

  // Change the weight of the link 0->1
  dag_.setLinkMetadata(0, 1, true);
  EXPECT_TRUE(dag_.getLinkMetadata(0, 1));

  // Make sure link weight 1->0 is not affected.
  EXPECT_FALSE(dag_.getLinkMetadata(1, 0));

  // Change the weight of the link 0->1 again
  dag_.setLinkMetadata(0, 1, false);
  EXPECT_FALSE(dag_.getLinkMetadata(0, 1));

  // Make sure link weight 1->0 is not affected.
  EXPECT_FALSE(dag_.getLinkMetadata(1, 0));
}

#ifdef QUICKSTEP_DEBUG
TEST(DAGDeathTest, SixNodeStagesCycleTest) {
  const int kNodeSize = 6;
  DAG<DummyPayload, int> dag_;

  for (int node_index = 0; node_index < kNodeSize; ++node_index) {
    ASSERT_EQ(static_cast<std::size_t>(node_index),
              dag_.createNode(new DummyPayload(node_index)));
  }

  /*
   * 0 1  2
   * \ /  /
   *  3  /
   *   \/
   *   4
   *   |
   *   5
   */

  dag_.createLink(0, 3, 0);
  dag_.createLink(1, 3, 1);
  dag_.createLink(2, 4, 0);
  dag_.createLink(3, 4, 1);
  dag_.createLink(4, 5, 0);

  vector<pair<vector<DAG<DummyPayload, int>::size_type_nodes>,
      vector<DAG<DummyPayload, int>::size_type_nodes> > > stages = dag_.computeStageSequence();

  ASSERT_EQ(4u, stages.size());
  // stages[0]
  EXPECT_NE(find(stages[0].first.begin(), stages[0].first.end(), 0), stages[0].first.end());
  EXPECT_NE(find(stages[0].first.begin(), stages[0].first.end(), 1), stages[0].first.end());
  EXPECT_NE(find(stages[0].first.begin(), stages[0].first.end(), 2), stages[0].first.end());
  // Check if there's no other element in stages[0].first than the ones we've
  // tested.
  EXPECT_EQ(3u, stages[0].first.size());

  EXPECT_NE(find(stages[0].second.begin(), stages[0].second.end(), 0), stages[0].second.end());
  EXPECT_NE(find(stages[0].second.begin(), stages[0].second.end(), 1), stages[0].second.end());
  EXPECT_EQ(2u, stages[0].second.size());

  // stages[1]
  EXPECT_NE(find(stages[1].first.begin(), stages[1].first.end(), 3), stages[1].first.end());
  EXPECT_EQ(1u, stages[1].first.size());

  EXPECT_NE(find(stages[1].second.begin(), stages[1].second.end(), 2), stages[1].second.end());
  EXPECT_NE(find(stages[1].second.begin(), stages[1].second.end(), 3), stages[1].second.end());
  EXPECT_EQ(2u, stages[1].second.size());

  // stages[2]
  EXPECT_NE(find(stages[2].first.begin(), stages[2].first.end(), 4), stages[2].first.end());
  EXPECT_EQ(1u, stages[2].first.size());

  EXPECT_NE(find(stages[2].second.begin(), stages[2].second.end(), 4), stages[2].second.end());
  EXPECT_EQ(1u, stages[2].second.size());

  // stages[3]
  EXPECT_NE(find(stages[3].first.begin(), stages[3].first.end(), 5), stages[3].first.end());
  EXPECT_EQ(1u, stages[3].first.size());

  EXPECT_NE(find(stages[3].second.begin(), stages[3].second.end(), 5), stages[3].second.end());
  EXPECT_EQ(1u, stages[3].second.size());

  dag_.createLink(5, 4, 3);

  // computeStageSequence goes in an infinite loop when graph has cycle
  // DCHECK(!hasCycle()) should abort the execution
  EXPECT_DEATH(dag_.computeStageSequence(), "");
}
#endif

}  // namespace quickstep
