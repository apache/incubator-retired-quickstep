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

#include "transaction/CycleDetector.hpp"

#include <cstdint>
#include <memory>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "transaction/DirectedGraph.hpp"
#include "transaction/StronglyConnectedComponents.hpp"

namespace quickstep {
namespace transaction {

CycleDetector::CycleDetector(DirectedGraph *wait_for_graph)
    : wait_for_graph_(wait_for_graph),
      strongly_connected_components_(
          std::make_unique<StronglyConnectedComponents>(*wait_for_graph)) {
}

std::vector<DirectedGraph::node_id>
CycleDetector::chooseVictimsToBreakCycle() const {
  std::vector<DirectedGraph::node_id> nodes_to_kill;
  const std::unordered_map<std::uint64_t, std::vector<DirectedGraph::node_id>>
      component_mapping = strongly_connected_components_->getComponentMapping();
  for (const auto &entry : component_mapping) {
    // One node means no cycle.
    if (entry.second.size() == 1) {
      continue;
    }
    const std::vector<DirectedGraph::node_id> nodes =
        chooseVictimsInComponent(entry.second);
    nodes_to_kill.insert(nodes_to_kill.end(), nodes.begin(), nodes.end());
  }
  return nodes_to_kill;
}

std::vector<DirectedGraph::node_id> CycleDetector::chooseVictimsInComponent(
    const std::vector<DirectedGraph::node_id> &nodes) const {
  std::vector<DirectedGraph::node_id> targets;
  // Convert it to set to ensure defensively that the elements are unique.
  std::unordered_set<DirectedGraph::node_id> nodes_set(nodes.begin(),
                                                       nodes.end());

  while (true) {
    if (!hasCycle(nodes_set)) {
      break;
    }
    // Connected component still has a cycle, therefore choose a
    // victim and keep trying to remove nodes until there is no cycle.
    const DirectedGraph::node_id victim = chooseVictim(nodes_set);
    // Remove the victim node from the connected component.
    nodes_set.erase(victim);
    // Removed node is a victim now.
    targets.push_back(victim);
  }
  return targets;
}

bool CycleDetector::hasCycle(
    const std::unordered_set<DirectedGraph::node_id> &nodes) const {
  // Keeps track of the nodes the algorithms visited.
  std::unordered_set<DirectedGraph::node_id> visited;
  for (const DirectedGraph::node_id node_id : nodes) {
    // If it is visited, then pass to the next one.
    if (visited.count(node_id) == 1) {
      continue;
    }
    // Save the backtracking information.
    std::stack<DirectedGraph::node_id> to_visit;
    // Mark this id as "to be visited".
    to_visit.push(node_id);
    // Start to visit nodes until it is done.
    while (!to_visit.empty()) {
      const DirectedGraph::node_id current_node = to_visit.top();
      to_visit.pop();
      // Mark the node coming from stack as "visited".
      visited.insert(current_node);
      // For all adjacent nodes of this "visited" node,
      const std::vector<DirectedGraph::node_id> adjacents
          = wait_for_graph_->getAdjacentNodes(current_node);
      for (const DirectedGraph::node_id adjacent : adjacents) {
        if (visited.count(adjacent) == 1) {
          // If this adjacent node is a node we already visited, then
          // there is a cycle.
          return true;
        } else if (nodes.count(adjacent) == 1 && visited.count(adjacent) == 0) {
          // Otherwise, if it is a node that we did not visit before
          // mark this nodes as "to be visited".
          to_visit.push(adjacent);
        }
      }
    }
  }
  // If we have already visited all nodes and could not find a cycle,
  // then we should return "no cycle" result.
  return false;
}

}  // namespace transaction
}  // namespace quickstep
