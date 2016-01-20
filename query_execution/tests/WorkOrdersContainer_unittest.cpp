/**
 *   Copyright 2015 Pivotal Software, Inc.
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

#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "utility/PtrVector.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogDatabase;
class QueryContext;
class StorageManager;

class MockNUMAWorkOrder : public WorkOrder {
 public:
  MockNUMAWorkOrder(const int id, const std::vector<int> &numa_nodes)
      : id_(id) {
    for (int numa_node : numa_nodes) {
      preferred_numa_nodes_.push_back(numa_node);
    }
  }

  const int getID() const {
    return id_;
  }

  void execute(QueryContext *query_context,
               CatalogDatabase *catalog_database,
               StorageManager *storage_manager) override {}

 private:
  const int id_;

  DISALLOW_COPY_AND_ASSIGN(MockNUMAWorkOrder);
};

// Note: In the tests, we create a local WorkOrder object (on stack) and use its
// address as a pointer while inserting to the container.
// The real usage of the API works as follows - we create a WorkOrder using "new"
// (on heap) and pass the pointer to the container. When a WorkOrder is
// retrieved from the container, its pointer is removed from the container and
// the caller is responsible to delete the actual WorkOrder object.

TEST(WorkOrdersContainerTest, ZeroNUMANodesTest) {
  // A container for one operator and no NUMA nodes.
  WorkOrdersContainer w(1, 0);

  EXPECT_EQ(0u, w.getNumNormalWorkOrders(0));
  EXPECT_EQ(0u, w.getNumRebuildWorkOrders(0));
  EXPECT_FALSE(w.hasNormalWorkOrder(0));
  EXPECT_FALSE(w.hasRebuildWorkOrder(0));
  EXPECT_EQ(nullptr, w.getNormalWorkOrder(0));
  EXPECT_EQ(nullptr, w.getRebuildWorkOrder(0));
}

TEST(WorkOrdersContainerTest, ZeroNUMANodesAddWorkOrderTest) {
  // Add one normal and rebuild workorder each for one operator DAG. Test if
  // they get inserted and retrieved correctly.
  std::vector<int> numa_node_ids;
  // A container for one operator and no NUMA nodes.
  WorkOrdersContainer w(1, 0);

  EXPECT_EQ(0u, w.getNumNormalWorkOrders(0));
  EXPECT_EQ(0u, w.getNumRebuildWorkOrders(0));
  EXPECT_FALSE(w.hasNormalWorkOrder(0));
  EXPECT_FALSE(w.hasRebuildWorkOrder(0));

  // Create a NUMA agnostic normal WorkOrder.
  MockNUMAWorkOrder wu(0, numa_node_ids);
  w.addNormalWorkOrder(&wu, 0);

  // Expect the normal WorkOrder count to be 1.
  EXPECT_EQ(1u, w.getNumNormalWorkOrders(0));
  EXPECT_EQ(0u, w.getNumRebuildWorkOrders(0));
  EXPECT_TRUE(w.hasNormalWorkOrder(0));
  EXPECT_FALSE(w.hasRebuildWorkOrder(0));

  // Create a NUMA agnostic rebuild WorkOrder.
  MockNUMAWorkOrder wu1(1, numa_node_ids);
  w.addRebuildWorkOrder(&wu1, 0);

  // Expect the normal WorkOrder count to be 1.
  EXPECT_EQ(1u, w.getNumNormalWorkOrders(0));
  EXPECT_EQ(1u, w.getNumRebuildWorkOrders(0));
  EXPECT_TRUE(w.hasNormalWorkOrder(0));
  EXPECT_TRUE(w.hasRebuildWorkOrder(0));

  // Check if we retrieve the same WorkOrders.
  WorkOrder *returned_wu = w.getNormalWorkOrder(0);
  ASSERT_TRUE(returned_wu != nullptr);
  EXPECT_EQ(wu.getID(), static_cast<MockNUMAWorkOrder*>(returned_wu)->getID());

  WorkOrder *returned_rebuild_wu = w.getRebuildWorkOrder(0);
  ASSERT_TRUE(returned_rebuild_wu != nullptr);
  EXPECT_EQ(wu1.getID(),
            static_cast<MockNUMAWorkOrder *>(returned_rebuild_wu)->getID());

  // Container should be empty now.
  EXPECT_EQ(0u, w.getNumNormalWorkOrders(0));
  EXPECT_EQ(0u, w.getNumRebuildWorkOrders(0));
  EXPECT_FALSE(w.hasNormalWorkOrder(0));
  EXPECT_FALSE(w.hasRebuildWorkOrder(0));
  EXPECT_EQ(nullptr, w.getNormalWorkOrder(0));
  EXPECT_EQ(nullptr, w.getRebuildWorkOrder(0));
}

TEST(WorkOrdersContainerTest, ZeroNUMANodesMultipleWorkOrdersTest) {
  // Add multiple normal and rebuild workorders each for one operator DAG. Test
  // if they get inserted and retrieved correctly and the order of retrieval.
  // A container for one operator and no NUMA nodes.
  std::vector<int> numa_node_ids;
  WorkOrdersContainer w(1, 0);

  EXPECT_EQ(0u, w.getNumNormalWorkOrders(0));
  EXPECT_EQ(0u, w.getNumRebuildWorkOrders(0));
  EXPECT_FALSE(w.hasNormalWorkOrder(0));
  EXPECT_FALSE(w.hasRebuildWorkOrder(0));

  PtrVector<MockNUMAWorkOrder> normal_workorders;
  PtrVector<MockNUMAWorkOrder> rebuild_workorders;
  const std::size_t kNumWorkOrders = 100;

  // Push the mock WorkOrders in the vectors and the container.
  for (std::size_t i = 0; i < kNumWorkOrders; ++i) {
    EXPECT_EQ(i, w.getNumNormalWorkOrders(0));
    EXPECT_EQ(i, w.getNumRebuildWorkOrders(0));

    normal_workorders.push_back(new MockNUMAWorkOrder(i, numa_node_ids));
    rebuild_workorders.push_back(
        new MockNUMAWorkOrder(kNumWorkOrders + i, numa_node_ids));

    w.addNormalWorkOrder(&(normal_workorders.back()), 0);
    w.addRebuildWorkOrder(&(rebuild_workorders.back()), 0);
  }

  // Expect the normal WorkOrder count to be kNumWorkOrders.
  EXPECT_EQ(kNumWorkOrders, w.getNumNormalWorkOrders(0));
  EXPECT_EQ(kNumWorkOrders, w.getNumRebuildWorkOrders(0));
  EXPECT_TRUE(w.hasNormalWorkOrder(0));
  EXPECT_TRUE(w.hasRebuildWorkOrder(0));

  // Retrieve the WorkOrders and check the order of retrieval.
  for (std::size_t i = 0; i < kNumWorkOrders; ++i) {
    EXPECT_EQ(kNumWorkOrders - i, w.getNumNormalWorkOrders(0));
    EXPECT_EQ(kNumWorkOrders - i, w.getNumRebuildWorkOrders(0));
    WorkOrder *returned_wu = w.getNormalWorkOrder(0);
    ASSERT_TRUE(returned_wu != nullptr);
    EXPECT_EQ(static_cast<int>(i), static_cast<MockNUMAWorkOrder *>(returned_wu)->getID());
    WorkOrder *returned_rebuild_wu = w.getRebuildWorkOrder(0);
    ASSERT_TRUE(returned_wu != nullptr);
    EXPECT_EQ(static_cast<int>(kNumWorkOrders + i),
              static_cast<MockNUMAWorkOrder *>(returned_rebuild_wu)->getID());
  }

  // Container should be empty now.
  EXPECT_EQ(0u, w.getNumNormalWorkOrders(0));
  EXPECT_EQ(0u, w.getNumRebuildWorkOrders(0));
  EXPECT_FALSE(w.hasNormalWorkOrder(0));
  EXPECT_FALSE(w.hasRebuildWorkOrder(0));
  EXPECT_EQ(nullptr, w.getNormalWorkOrder(0));
  EXPECT_EQ(nullptr, w.getRebuildWorkOrder(0));
}

TEST(WorkOrdersContainerTest, MultipleNUMANodesTest) {
  // Create WorkOrders belonging to different NUMA nodes and test if they get
  // inserted and retrieved correctly.
  std::vector<int> numa_node_ids;
  // The sequence of NUMA nodes isn't contiguous.
  numa_node_ids.push_back(4);
  numa_node_ids.push_back(7);
  numa_node_ids.push_back(0);
  const std::size_t kNUMANodes =
      1 +
      *std::max_element(numa_node_ids.begin(), numa_node_ids.end());

  const std::size_t kNUMANodesUsed = numa_node_ids.size();

  // A container for one operator and kNUMANodes.
  WorkOrdersContainer w(1, kNUMANodes);

  for (std::size_t i = 0; i < kNUMANodesUsed; ++i) {
    std::vector<int> curr_numa_node;
    curr_numa_node.push_back(numa_node_ids[i]);
  }

  PtrVector<MockNUMAWorkOrder> normal_workorders;
  PtrVector<MockNUMAWorkOrder> rebuild_workorders;

  for (std::size_t i = 0; i < kNUMANodesUsed; ++i) {
    // Create a vector consisting of the current NUMA node as its element.
    std::vector<int> curr_numa_node;
    curr_numa_node.push_back(numa_node_ids[i]);

    // Create normal and rebuild WorkOrders belonging to exactly one NUMA node.
    normal_workorders.push_back(new MockNUMAWorkOrder(i, curr_numa_node));
    rebuild_workorders.push_back(
        new MockNUMAWorkOrder(kNUMANodes + i, curr_numa_node));

    w.addNormalWorkOrder(&(normal_workorders.back()), 0);
    w.addRebuildWorkOrder(&(rebuild_workorders.back()), 0);

    // For each NUMA node, check the count of WorkOrders.
    for (std::size_t j = 0; j < i; ++j) {
      EXPECT_EQ(1u, w.getNumNormalWorkOrdersForNUMANode(0, numa_node_ids[j]));
      EXPECT_EQ(1u, w.getNumRebuildWorkOrdersForNUMANode(0, numa_node_ids[j]));
    }
  }

  // Retrieve the WorkOrders.
  for (std::size_t i = 0; i < kNUMANodesUsed; ++i) {
    // For each NUMA node, check the count of WorkOrders.
    for (std::size_t j = 0; j < kNUMANodesUsed; ++j) {
      if (j >= i) {
        // We haven't retrieved the workorders for this NUMA node yet.
        EXPECT_EQ(1u, w.getNumNormalWorkOrdersForNUMANode(0, numa_node_ids[j]));
        EXPECT_EQ(1u, w.getNumRebuildWorkOrdersForNUMANode(0, numa_node_ids[j]));
      } else {
        // We already retrieved the workorders for this NUMA node.
        EXPECT_EQ(0u, w.getNumNormalWorkOrdersForNUMANode(0, numa_node_ids[j]));
        EXPECT_EQ(0u, w.getNumRebuildWorkOrdersForNUMANode(0, numa_node_ids[j]));
      }
    }

    // Retrieve the workorders for this NUMA node.
    WorkOrder *returned_wu = w.getNormalWorkOrderForNUMANode(0, numa_node_ids[i]);
    ASSERT_TRUE(returned_wu != nullptr);
    EXPECT_EQ(normal_workorders[i].getID(),
              static_cast<MockNUMAWorkOrder *>(returned_wu)->getID());

    WorkOrder *returned_rebuild_wu =
        w.getRebuildWorkOrderForNUMANode(0, numa_node_ids[i]);
    ASSERT_TRUE(returned_rebuild_wu != nullptr);
    EXPECT_EQ(rebuild_workorders[i].getID(),
              static_cast<MockNUMAWorkOrder *>(returned_rebuild_wu)->getID());
  }

  // No workorder should be left for this operator on any NUMA node.
  for (const int numa_node : numa_node_ids) {
    EXPECT_FALSE(w.hasNormalWorkOrderForNUMANode(0, numa_node));
    EXPECT_FALSE(w.hasRebuildWorkOrderForNUMANode(0, numa_node));
  }

  // No workorder should be left for this operator.
  EXPECT_FALSE(w.hasNormalWorkOrder(0));
  EXPECT_FALSE(w.hasRebuildWorkOrder(0));
}

TEST(WorkOrdersContainerTest, AllTypesWorkOrdersTest) {
  // For a given operator create three types of WorkOrders
  // 1. NUMA agnostic.
  // 2. WorkOrder with single NUMA node.
  // 3. WorkOrder with multiple NUMA nodes.
  std::vector<int> numa_nodes;

  // Create a WorkOrder with no NUMA node.
  MockNUMAWorkOrder no_numa_wu(0, numa_nodes);
  EXPECT_TRUE(no_numa_wu.getPreferredNUMANodes().empty());

  // Add one NUMA node.
  numa_nodes.push_back(0);

  // Create a WorkOrder with exactly one NUMA node.
  MockNUMAWorkOrder one_numa_wu(1, numa_nodes);
  EXPECT_EQ(0, one_numa_wu.getPreferredNUMANodes().front());

  // Add another NUMA node.
  numa_nodes.push_back(3);

  // Create a WorkOrder with more than one NUMA node.
  MockNUMAWorkOrder multiple_numa_wu(2, numa_nodes);
  for (std::size_t i = 0; i < numa_nodes.size(); ++i) {
    EXPECT_EQ(numa_nodes[i], multiple_numa_wu.getPreferredNUMANodes().at(i));
  }

  const std::size_t kNUMANodes =
      1 + *std::max_element(numa_nodes.begin(), numa_nodes.end());
  const std::size_t kNUMANodesUsed = numa_nodes.size();

  // Create the container.
  WorkOrdersContainer w(1, kNUMANodes);

  w.addNormalWorkOrder(&multiple_numa_wu, 0);

  for (std::size_t i = 0; i < kNUMANodesUsed; ++i) {
    // Check the count of per NUMA node workorders.
    EXPECT_EQ(1u, w.getNumNormalWorkOrdersForNUMANode(0, numa_nodes[i]));
  }

  // Expect 1 normal workorder in total for this operator.
  EXPECT_EQ(1u, w.getNumNormalWorkOrders(0));
  EXPECT_EQ(0u, w.getNumRebuildWorkOrders(0));

  // Add the WorkOrder with no NUMA node.
  w.addNormalWorkOrder(&no_numa_wu, 0);

  for (std::size_t i = 0; i < kNUMANodesUsed; ++i) {
    // Check the count of per NUMA node workorders.
    EXPECT_EQ(1u, w.getNumNormalWorkOrdersForNUMANode(0, numa_nodes[i]));
  }

  // Expect 2 normal workorders in total for this operator.
  EXPECT_EQ(2u, w.getNumNormalWorkOrders(0));
  EXPECT_EQ(0u, w.getNumRebuildWorkOrders(0));

  // Add the WorkOrder with exactly one NUMA node.
  w.addNormalWorkOrder(&one_numa_wu, 0);

  EXPECT_EQ(2u, w.getNumNormalWorkOrdersForNUMANode(0, numa_nodes[0]));
  EXPECT_EQ(1u, w.getNumNormalWorkOrdersForNUMANode(0, numa_nodes[1]));

  // Expect 3 normal workorders in total for this operator.
  EXPECT_EQ(3u, w.getNumNormalWorkOrders(0));
  EXPECT_EQ(0u, w.getNumRebuildWorkOrders(0));

  // Retrieve the workorders for NUMA node = numa_nodes[0]
  MockNUMAWorkOrder *observed_wu = static_cast<MockNUMAWorkOrder *>(
      w.getNormalWorkOrderForNUMANode(0, numa_nodes[0]));
  ASSERT_TRUE(observed_wu != nullptr);

  EXPECT_EQ(one_numa_wu.getPreferredNUMANodes().front(),
            observed_wu->getPreferredNUMANodes().front());
  EXPECT_EQ(one_numa_wu.getID(), observed_wu->getID());

  EXPECT_EQ(1u, w.getNumNormalWorkOrdersForNUMANode(0, numa_nodes[0]));
  EXPECT_EQ(1u, w.getNumNormalWorkOrdersForNUMANode(0, numa_nodes[1]));

  // Expect 2 normal workorders in total for this operator.
  EXPECT_EQ(2u, w.getNumNormalWorkOrders(0));
  EXPECT_EQ(0u, w.getNumRebuildWorkOrders(0));

  // Retrieve the non NUMA workorder.
  WorkOrder *observed_non_numa_wu = w.getNormalWorkOrder(0);
  ASSERT_TRUE(observed_non_numa_wu != nullptr);
  EXPECT_EQ(no_numa_wu.getID(),
            static_cast<MockNUMAWorkOrder *>(observed_non_numa_wu)->getID());

  EXPECT_EQ(1u, w.getNumNormalWorkOrdersForNUMANode(0, numa_nodes[0]));
  EXPECT_EQ(1u, w.getNumNormalWorkOrdersForNUMANode(0, numa_nodes[1]));

  // Expect 1 normal workorders in total for this operator.
  EXPECT_EQ(1u, w.getNumNormalWorkOrders(0));
  EXPECT_EQ(0u, w.getNumRebuildWorkOrders(0));

  // Retrieve the workorder with multiple NUMA nodes.
  MockNUMAWorkOrder *observed_wu_multiple_numa_nodes =
      static_cast<MockNUMAWorkOrder *>(w.getNormalWorkOrder(0));
  ASSERT_TRUE(observed_wu_multiple_numa_nodes != nullptr);
  EXPECT_EQ(multiple_numa_wu.getID(), observed_wu_multiple_numa_nodes->getID());

  std::vector<int> observed_numa_nodes(
      observed_wu_multiple_numa_nodes->getPreferredNUMANodes());
  // Look up the expected numa nodes in the observed_numa_nodes vector.
  EXPECT_TRUE(std::find(observed_numa_nodes.begin(), observed_numa_nodes.end(),
                        numa_nodes[0]) != observed_numa_nodes.end());
  EXPECT_TRUE(std::find(observed_numa_nodes.begin(), observed_numa_nodes.end(),
                        numa_nodes[1]) != observed_numa_nodes.end());

  EXPECT_EQ(0u, w.getNumNormalWorkOrdersForNUMANode(0, numa_nodes[0]));
  EXPECT_EQ(0u, w.getNumNormalWorkOrdersForNUMANode(0, numa_nodes[1]));

  // Expect no normal workorders in total for this operator.
  EXPECT_EQ(0u, w.getNumNormalWorkOrders(0));
  EXPECT_EQ(0u, w.getNumRebuildWorkOrders(0));
}

TEST(WorkOrdersContainerTest, MultipleOperatorsNormalWUTest) {
  // Generate workorders for multiple operators and test their insertion and
  // retrieval. Use the IDs of the WorkOrders to verify the correctness.

  // Create the NUMA node vectors.
  std::vector<int> numa_node_ids;

  const std::size_t kNumOperators = 100;

  // For each operator create normal workorders with no NUMA node.
  PtrVector<MockNUMAWorkOrder> normal_workorders_no_numa;
  std::vector<int> normal_workorders_no_numa_ids;
  // Set of workorder IDs = {0, ... kNumOperators - 1}
  for (std::size_t i = 0; i < kNumOperators; ++i) {
    normal_workorders_no_numa_ids.push_back(i);
    normal_workorders_no_numa.push_back(new MockNUMAWorkOrder(
        normal_workorders_no_numa_ids.back(), numa_node_ids));
  }

  // Insert one NUMA node.
  numa_node_ids.push_back(0);

  // For each operator create normal workorders with one NUMA node.
  PtrVector<MockNUMAWorkOrder> normal_workorders_one_numa;
  std::vector<int> normal_workorders_one_numa_ids;
  // Set of workorder IDs = {kNumOperators, .. , 2*kNumOperators - 1}
  for (std::size_t i = 0; i < kNumOperators; ++i) {
    normal_workorders_one_numa_ids.push_back(kNumOperators + i);
    normal_workorders_one_numa.push_back(new MockNUMAWorkOrder(
        normal_workorders_one_numa_ids.back(), numa_node_ids));
  }

  // Insert another NUMA node.
  numa_node_ids.push_back(1);

  // For each operator create normal workorders with more than one NUMA node.
  PtrVector<MockNUMAWorkOrder> normal_workorders_multiple_numa;
  std::vector<int> normal_workorders_multiple_numa_ids;
  // Set of workorder IDs = {2*kNumOperators, .. , 3*kNumOperators - 1}
  for (std::size_t i = 0; i < kNumOperators; ++i) {
    normal_workorders_multiple_numa_ids.push_back(2*kNumOperators + i);
    normal_workorders_multiple_numa.push_back(new MockNUMAWorkOrder(
        normal_workorders_multiple_numa_ids.back(), numa_node_ids));
  }

  // TODO(harshad) : Design a test in which the number of NUMA nodes is
  // configurable.
  const std::size_t kNUMANodes = numa_node_ids.size();

  // Create the container.
  WorkOrdersContainer w(kNumOperators, kNUMANodes);

  std::vector<std::size_t> operator_ids;
  for (std::size_t i = 0; i < kNumOperators; ++i) {
    operator_ids.push_back(i);
  }

  // Randomize the operator IDs.
  std::random_shuffle(operator_ids.begin(), operator_ids.end());

  // Insert the workorders.
  for (std::size_t outer_op = 0; outer_op < kNumOperators; ++outer_op) {
    const std::size_t curr_operator_id = operator_ids[outer_op];
    // Check the count of WorkOrders per operator.
    for (std::size_t inner_op = 0; inner_op < kNumOperators; ++inner_op) {
      if (inner_op < outer_op) {
        // WorkOrders inserted already.
        EXPECT_EQ(3u, w.getNumNormalWorkOrders(operator_ids[inner_op]));
        EXPECT_TRUE(w.hasNormalWorkOrder(operator_ids[inner_op]));
        EXPECT_EQ(0u, w.getNumRebuildWorkOrders(operator_ids[inner_op]));
        EXPECT_FALSE(w.hasRebuildWorkOrder(operator_ids[inner_op]));
        // Number of WorkOrders : NUMA Node
        // 2 : 0
        // 1 : 1
        for (int numa_node_used = kNUMANodes - 1; numa_node_used >= 0;
             --numa_node_used) {
          EXPECT_EQ(kNUMANodes - numa_node_used,
                    w.getNumNormalWorkOrdersForNUMANode(
                        operator_ids[inner_op], numa_node_ids[numa_node_used]));
          EXPECT_TRUE(w.hasNormalWorkOrderForNUMANode(
              operator_ids[inner_op], numa_node_ids[numa_node_used]));
        }
      } else {
        // WorkOrders yet to be inserted.
        EXPECT_EQ(0u, w.getNumNormalWorkOrders(operator_ids[inner_op]));
        EXPECT_FALSE(w.hasNormalWorkOrder(operator_ids[inner_op]));
        EXPECT_EQ(0u, w.getNumRebuildWorkOrders(operator_ids[inner_op]));
        EXPECT_FALSE(w.hasRebuildWorkOrder(operator_ids[inner_op]));
        for (int numa_node_used = kNUMANodes - 1; numa_node_used >= 0;
             --numa_node_used) {
          EXPECT_EQ(
              0u, w.getNumNormalWorkOrdersForNUMANode(
                     operator_ids[inner_op], numa_node_ids[numa_node_used]));
          EXPECT_FALSE(w.hasNormalWorkOrderForNUMANode(
              operator_ids[inner_op], numa_node_ids[numa_node_used]));
        }
      }
    }
    // Insert the workorder with multiple NUMA nodes.
    w.addNormalWorkOrder(&(normal_workorders_multiple_numa[curr_operator_id]),
                        curr_operator_id);
    // Insert the workorder with no NUMA node.
    w.addNormalWorkOrder(&(normal_workorders_no_numa[curr_operator_id]),
                        curr_operator_id);
    // Insert the workorder with one NUMA node.
    w.addNormalWorkOrder(&(normal_workorders_one_numa[curr_operator_id]),
                        curr_operator_id);
  }

  // Randomize the operator IDs again and retrieve the WorkOrders.
  std::random_shuffle(operator_ids.begin(), operator_ids.end());
  for (std::size_t outer_op = 0; outer_op < kNumOperators; ++outer_op) {
    const std::size_t curr_operator_id = operator_ids[outer_op];
    // Check if the existing WorkOrder counts are correct.
    for (std::size_t inner_op = 0; inner_op < kNumOperators; ++inner_op) {
      if (inner_op >= outer_op) {
        // WorkOrders not yet retrieved.
        EXPECT_EQ(3u, w.getNumNormalWorkOrders(operator_ids[inner_op]));
        EXPECT_TRUE(w.hasNormalWorkOrder(operator_ids[inner_op]));
        EXPECT_EQ(0u, w.getNumRebuildWorkOrders(operator_ids[inner_op]));
        EXPECT_FALSE(w.hasRebuildWorkOrder(operator_ids[inner_op]));
        // Number of WorkOrders : NUMA Node
        // 2 : 0
        // 1 : 1
        for (int numa_node_used = kNUMANodes - 1; numa_node_used >= 0;
             --numa_node_used) {
          EXPECT_EQ(
              kNUMANodes - numa_node_used,
              w.getNumNormalWorkOrdersForNUMANode(
                  operator_ids[inner_op], numa_node_ids[numa_node_used]));
          EXPECT_TRUE(w.hasNormalWorkOrderForNUMANode(
              operator_ids[inner_op], numa_node_ids[numa_node_used]));
        }
      } else {
        // WorkOrders already retrieved.
        EXPECT_EQ(0u, w.getNumNormalWorkOrders(operator_ids[inner_op]));
        EXPECT_FALSE(w.hasNormalWorkOrder(operator_ids[inner_op]));
        EXPECT_EQ(0u, w.getNumRebuildWorkOrders(operator_ids[inner_op]));
        EXPECT_FALSE(w.hasRebuildWorkOrder(operator_ids[inner_op]));
        for (int numa_node_used = kNUMANodes - 1; numa_node_used >= 0;
             --numa_node_used) {
          EXPECT_EQ(
              0u, w.getNumNormalWorkOrdersForNUMANode(
                     operator_ids[inner_op], numa_node_ids[numa_node_used]));
          EXPECT_FALSE(w.hasNormalWorkOrderForNUMANode(
              operator_ids[inner_op], numa_node_ids[numa_node_used]));
        }
      }
    }
    // There is a workorder with exactly one NUMA node, get that node id.
    const std::size_t single_numa_node_id = numa_node_ids.front();
    // There is a workorder with more than one NUMA node, get that node id
    // (which is different than the node id above).
    const std::size_t multiple_numa_node_id = numa_node_ids.back();

    // Retrieve a single NUMA node workorder.
    MockNUMAWorkOrder *observed_wu_single_numa =
        static_cast<MockNUMAWorkOrder *>(w.getNormalWorkOrderForNUMANode(
            curr_operator_id, single_numa_node_id));
    ASSERT_TRUE(observed_wu_single_numa != nullptr);

    // Verify if the workorder ID is correct.
    const int expected_workorder_id_single_numa =
        normal_workorders_one_numa_ids[curr_operator_id];
    EXPECT_EQ(expected_workorder_id_single_numa,
              observed_wu_single_numa->getID());

    // Retrieve a multiple NUMA node workorder.
    MockNUMAWorkOrder *observed_wu_multiple_numa =
        static_cast<MockNUMAWorkOrder *>(w.getNormalWorkOrderForNUMANode(
            curr_operator_id, multiple_numa_node_id));
    ASSERT_TRUE(observed_wu_multiple_numa != nullptr);

    // Verify if the workorder ID is correct.
    const int expected_workorder_id_multiple_numa =
        normal_workorders_multiple_numa_ids[curr_operator_id];

    EXPECT_EQ(expected_workorder_id_multiple_numa,
              observed_wu_multiple_numa->getID());

    // Retrieve a no NUMA node workorder.
    MockNUMAWorkOrder *observed_wu_no_numa =
        static_cast<MockNUMAWorkOrder *>(w.getNormalWorkOrder(curr_operator_id));
    ASSERT_TRUE(observed_wu_no_numa != nullptr);

    // Verify if the workorder ID is correct.
    const int expected_workorder_id_no_numa =
        normal_workorders_no_numa_ids[curr_operator_id];

    EXPECT_EQ(expected_workorder_id_no_numa, observed_wu_no_numa->getID());
  }
}

TEST(WorkOrdersContainerTest, MultipleOperatorsRebuildWUTest) {
  // This test is exactly similar to previous one, except that this test checks
  // rebuild workorders related interface.

  // Create the NUMA node vectors.
  std::vector<int> numa_node_ids;

  const std::size_t kNumOperators = 100;

  // For each operator create rebuild workorders with no NUMA node.
  PtrVector<MockNUMAWorkOrder> rebuild_workorders_no_numa;
  std::vector<int> rebuild_workorders_no_numa_ids;
  // Set of workorder IDs = {0, ... kNumOperators - 1}
  for (std::size_t i = 0; i < kNumOperators; ++i) {
    rebuild_workorders_no_numa_ids.push_back(i);
    rebuild_workorders_no_numa.push_back(new MockNUMAWorkOrder(
        rebuild_workorders_no_numa_ids.back(), numa_node_ids));
  }

  // Insert one NUMA node.
  numa_node_ids.push_back(0);

  // For each operator create rebuild workorders with one NUMA node.
  PtrVector<MockNUMAWorkOrder> rebuild_workorders_one_numa;
  std::vector<int> rebuild_workorders_one_numa_ids;
  // Set of workorder IDs = {kNumOperators, .. , 2*kNumOperators - 1}
  for (std::size_t i = 0; i < kNumOperators; ++i) {
    rebuild_workorders_one_numa_ids.push_back(kNumOperators + i);
    rebuild_workorders_one_numa.push_back(new MockNUMAWorkOrder(
        rebuild_workorders_one_numa_ids.back(), numa_node_ids));
  }

  // Insert another NUMA node.
  numa_node_ids.push_back(1);

  // For each operator create rebuild workorders with more than one NUMA node.
  PtrVector<MockNUMAWorkOrder> rebuild_workorders_multiple_numa;
  std::vector<int> rebuild_workorders_multiple_numa_ids;
  // Set of workorder IDs = {2*kNumOperators, .. , 3*kNumOperators - 1}
  for (std::size_t i = 0; i < kNumOperators; ++i) {
    rebuild_workorders_multiple_numa_ids.push_back(2*kNumOperators + i);
    rebuild_workorders_multiple_numa.push_back(new MockNUMAWorkOrder(
        rebuild_workorders_multiple_numa_ids.back(), numa_node_ids));
  }

  // TODO(harshad) : Design a test in which the number of NUMA nodes is
  // configurable.
  const std::size_t kNUMANodes = numa_node_ids.size();

  // Create the container.
  WorkOrdersContainer w(kNumOperators, kNUMANodes);

  std::vector<std::size_t> operator_ids;
  for (std::size_t i = 0; i < kNumOperators; ++i) {
    operator_ids.push_back(i);
  }

  // Randomize the operator IDs.
  std::random_shuffle(operator_ids.begin(), operator_ids.end());

  // Insert the workorders.
  for (std::size_t outer_op = 0; outer_op < kNumOperators; ++outer_op) {
    const std::size_t curr_operator_id = operator_ids[outer_op];
    // Check the count of WorkOrders per operator.
    for (std::size_t inner_op = 0; inner_op < kNumOperators; ++inner_op) {
      if (inner_op < outer_op) {
        // WorkOrders inserted already.
        EXPECT_EQ(3u, w.getNumRebuildWorkOrders(operator_ids[inner_op]));
        EXPECT_TRUE(w.hasRebuildWorkOrder(operator_ids[inner_op]));
        EXPECT_EQ(0u, w.getNumNormalWorkOrders(operator_ids[inner_op]));
        EXPECT_FALSE(w.hasNormalWorkOrder(operator_ids[inner_op]));
        // Number of WorkOrders : NUMA Node
        // 2 : 0
        // 1 : 1
        for (int numa_node_used = kNUMANodes - 1; numa_node_used >= 0;
             --numa_node_used) {
          EXPECT_EQ(
              kNUMANodes - numa_node_used,
              w.getNumRebuildWorkOrdersForNUMANode(
                  operator_ids[inner_op], numa_node_ids[numa_node_used]));
          EXPECT_TRUE(w.hasRebuildWorkOrderForNUMANode(
              operator_ids[inner_op], numa_node_ids[numa_node_used]));
        }
      } else {
        // WorkOrders yet to be inserted.
        EXPECT_EQ(0u, w.getNumRebuildWorkOrders(operator_ids[inner_op]));
        EXPECT_FALSE(w.hasRebuildWorkOrder(operator_ids[inner_op]));
        EXPECT_EQ(0u, w.getNumNormalWorkOrders(operator_ids[inner_op]));
        EXPECT_FALSE(w.hasNormalWorkOrder(operator_ids[inner_op]));
        for (int numa_node_used = kNUMANodes - 1; numa_node_used >= 0;
             --numa_node_used) {
          EXPECT_EQ(
              0u, w.getNumRebuildWorkOrdersForNUMANode(
                     operator_ids[inner_op], numa_node_ids[numa_node_used]));
          EXPECT_FALSE(w.hasRebuildWorkOrderForNUMANode(
              operator_ids[inner_op], numa_node_ids[numa_node_used]));
        }
      }
    }
    // Insert the workorder with multiple NUMA nodes.
    w.addRebuildWorkOrder(&(rebuild_workorders_multiple_numa[curr_operator_id]),
                         curr_operator_id);
    // Insert the workorder with no NUMA node.
    w.addRebuildWorkOrder(&(rebuild_workorders_no_numa[curr_operator_id]),
                         curr_operator_id);
    // Insert the workorder with one NUMA node.
    w.addRebuildWorkOrder(&(rebuild_workorders_one_numa[curr_operator_id]),
                         curr_operator_id);
  }

  // Randomize the operator IDs again and retrieve the WorkOrders.
  std::random_shuffle(operator_ids.begin(), operator_ids.end());
  for (std::size_t outer_op = 0; outer_op < kNumOperators; ++outer_op) {
    const std::size_t curr_operator_id = operator_ids[outer_op];
    // Check if the existing WorkOrder counts are correct.
    for (std::size_t inner_op = 0; inner_op < kNumOperators; ++inner_op) {
      if (inner_op >= outer_op) {
        // WorkOrders not yet retrieved.
        EXPECT_EQ(3u, w.getNumRebuildWorkOrders(operator_ids[inner_op]));
        EXPECT_TRUE(w.hasRebuildWorkOrder(operator_ids[inner_op]));
        EXPECT_EQ(0u, w.getNumNormalWorkOrders(operator_ids[inner_op]));
        EXPECT_FALSE(w.hasNormalWorkOrder(operator_ids[inner_op]));
        // Number of WorkOrders : NUMA Node
        // 2 : 0
        // 1 : 1
        for (int numa_node_used = kNUMANodes - 1; numa_node_used >= 0;
             --numa_node_used) {
          EXPECT_EQ(
              kNUMANodes - numa_node_used,
              w.getNumRebuildWorkOrdersForNUMANode(
                  operator_ids[inner_op], numa_node_ids[numa_node_used]));
          EXPECT_TRUE(w.hasRebuildWorkOrderForNUMANode(
              operator_ids[inner_op], numa_node_ids[numa_node_used]));
        }
      } else {
        // WorkOrders already retrieved.
        EXPECT_EQ(0u, w.getNumRebuildWorkOrders(operator_ids[inner_op]));
        EXPECT_FALSE(w.hasRebuildWorkOrder(operator_ids[inner_op]));
        EXPECT_EQ(0u, w.getNumNormalWorkOrders(operator_ids[inner_op]));
        EXPECT_FALSE(w.hasNormalWorkOrder(operator_ids[inner_op]));
        for (int numa_node_used = kNUMANodes - 1; numa_node_used >= 0;
             --numa_node_used) {
          EXPECT_EQ(
              0u, w.getNumRebuildWorkOrdersForNUMANode(
                     operator_ids[inner_op], numa_node_ids[numa_node_used]));
          EXPECT_FALSE(w.hasRebuildWorkOrderForNUMANode(
              operator_ids[inner_op], numa_node_ids[numa_node_used]));
        }
      }
    }
    // There is a workorder with exactly one NUMA node, get that node id.
    const std::size_t single_numa_node_id = numa_node_ids.front();
    // There is a workorder with more than one NUMA node, get that node id
    // (which is different than the node id above).
    const std::size_t multiple_numa_node_id = numa_node_ids.back();

    // Retrieve a single NUMA node workorder.
    MockNUMAWorkOrder *observed_wu_single_numa =
        static_cast<MockNUMAWorkOrder *>(w.getRebuildWorkOrderForNUMANode(
            curr_operator_id, single_numa_node_id));
    ASSERT_TRUE(observed_wu_single_numa != nullptr);

    // Verify if the workorder ID is correct.
    const int expected_workorder_id_single_numa =
        rebuild_workorders_one_numa_ids[curr_operator_id];
    EXPECT_EQ(expected_workorder_id_single_numa,
              observed_wu_single_numa->getID());

    // Retrieve a multiple NUMA node workorder.
    MockNUMAWorkOrder *observed_wu_multiple_numa =
        static_cast<MockNUMAWorkOrder *>(w.getRebuildWorkOrderForNUMANode(
            curr_operator_id, multiple_numa_node_id));
    ASSERT_TRUE(observed_wu_multiple_numa != nullptr);

    // Verify if the workorder ID is correct.
    const int expected_workorder_id_multiple_numa =
        rebuild_workorders_multiple_numa_ids[curr_operator_id];

    EXPECT_EQ(expected_workorder_id_multiple_numa,
              observed_wu_multiple_numa->getID());

    // Retrieve a no NUMA node workorder.
    MockNUMAWorkOrder *observed_wu_no_numa =
        static_cast<MockNUMAWorkOrder *>(w.getRebuildWorkOrder(curr_operator_id));

    // Verify if the workorder ID is correct.
    const int expected_workorder_id_no_numa =
        rebuild_workorders_no_numa_ids[curr_operator_id];

    EXPECT_EQ(expected_workorder_id_no_numa, observed_wu_no_numa->getID());
  }
}

TEST(WorkOrdersContainerTest, RetrievalOrderTest) {
  // Create only two kinds of workorders -
  // 1. WorkOrder with exactly one NUMA node as input.
  // 2. WorkOrder with more than one NUMA node as input.
  // Vary the parameter prefer_single_NUMA_node and verify the behavior.
  std::vector<int> numa_node_ids;
  numa_node_ids.push_back(0);
  const std::size_t kNumWorkOrdersPerType = 100;

  WorkOrdersContainer w(1, 2);

  std::vector<int> single_numa_node_workorder_ids;
  std::vector<int> multiple_numa_node_workorder_ids;

  PtrVector<MockNUMAWorkOrder> single_numa_node_workorders;
  PtrVector<MockNUMAWorkOrder> multiple_numa_node_workorders;

  // Insert WorkOrders with exactly one NUMA node and store the WorkOrder IDs.
  for (std::size_t wu_num = 0; wu_num < kNumWorkOrdersPerType; ++wu_num) {
    w.addNormalWorkOrder(new MockNUMAWorkOrder(wu_num, numa_node_ids), 0);
    single_numa_node_workorder_ids.push_back(wu_num);
  }

  numa_node_ids.push_back(1);

  // Insert WorkOrders with more than one NUMA node and store the WorkOrder IDs.
  for (std::size_t wu_num = 0; wu_num < kNumWorkOrdersPerType; ++wu_num) {
    w.addNormalWorkOrder(
        new MockNUMAWorkOrder(wu_num + kNumWorkOrdersPerType, numa_node_ids),
        0);
    multiple_numa_node_workorder_ids.push_back(kNumWorkOrdersPerType + wu_num);
  }

  // Create a vector of booleans of size kNumWorkOrdersPerType * 2
  std::vector<bool> retrieval_order;
  for (std::size_t i = 0; i < kNumWorkOrdersPerType; ++i) {
    retrieval_order.push_back(true);
    retrieval_order.push_back(false);
  }

  // Randomize the order of retrieval.
  std::random_shuffle(retrieval_order.begin(), retrieval_order.end());

  std::vector<int>::iterator single_numa_it =
      single_numa_node_workorder_ids.begin();
  std::vector<int>::iterator multiple_numa_it =
      multiple_numa_node_workorder_ids.begin();

  for (bool prefer_single_NUMA_node : retrieval_order) {
    // Retrieve the WorkOrder.
    MockNUMAWorkOrder *observed_wu = static_cast<MockNUMAWorkOrder *>(
        w.getNormalWorkOrder(0, prefer_single_NUMA_node));
    ASSERT_TRUE(observed_wu != nullptr);
    if (prefer_single_NUMA_node) {
      EXPECT_EQ(*single_numa_it, observed_wu->getID());
      EXPECT_EQ(1u, observed_wu->getPreferredNUMANodes().size());
      ++single_numa_it;
    } else {
      EXPECT_EQ(*multiple_numa_it, observed_wu->getID());
      EXPECT_EQ(2u, observed_wu->getPreferredNUMANodes().size());
      ++multiple_numa_it;
    }
  }
  DEBUG_ASSERT(single_numa_it == single_numa_node_workorder_ids.end());
  DEBUG_ASSERT(multiple_numa_it == multiple_numa_node_workorder_ids.end());

  // Expect no more workorders.
  EXPECT_EQ(nullptr, w.getNormalWorkOrder(0, true));
  EXPECT_EQ(nullptr, w.getNormalWorkOrder(0, false));
}

}  // namespace quickstep
