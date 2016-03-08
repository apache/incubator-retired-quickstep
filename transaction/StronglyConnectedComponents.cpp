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

namespace quickstep {

namespace transaction {

StronglyConnectedComponents::StronglyConnectedComponents(const DirectedGraph &directed_graph)
  : directed_graph_(directed_graph)
  , is_marked_(directed_graph_.count())
  , component_ids_(directed_graph_.count())
  , low_ids_(directed_graph_.count())
  , preorder_counter_(0)
  , no_of_components_(0) {
}

void StronglyConnectedComponents::findStronglyConnectedComponents() {
  for (DirectedGraph::node_id v = 0; v < directed_graph_.count(); ++v) {
    if (!is_marked_[v]) {
      depthFirstSearch(v);
    }
  }
}


void StronglyConnectedComponents::depthFirstSearch(DirectedGraph::node_id v) {
  // Mark this node.
  is_marked_[v] = true;

  // Set the low_id = preorder_counter and increment counter.
  // Right now low_id eqaual to the counter because the node
  // have not seen any other node.
  low_ids_[v] = preorder_counter_++;

  // Set min = counter = low_id.
  // It keeps track of the minimum traversal order the node has
  // seen.
  std::uint64_t min = low_ids_[v];

  // Push the node to the stack.
  stack_.push(v);

  // For every adjacent node w:
  // 1-) Apply DFS if w is not marked (recursively).
  // 2-) If low id of w is smaller than minimum number
  //     we have seen so far, update our minimum.
  for (DirectedGraph::node_id w : directed_graph_.getAdjacentNodes(v)) {
    if (!is_marked_[w]) {
      depthFirstSearch(w);
    }
    if (low_ids_[w] < min) {
      min = low_ids_[w];
    }
  }

  // If this node's low id is bigger than the minimum
  // traversal order we have seen so far, update the
  // minimum to reflect changes. That means also we captured
  // a cycle here. To keep invariant we have to keep node in
  // stack, therefore it returns.
  if (min < low_ids_[v]) {
    low_ids_[v] = min;
    return;
  }

  // Reaching here means:
  // - v.min >= v.low_id
  // v is the root of strongly connected component.
  DirectedGraph::node_id w;
  do {
    // Until we get our node, pop from stack.
    w = stack_.top();

    // Make component id of the node on the top
    // equals to current number of component counter.
    component_ids_[w] = no_of_components_;

    // Set the low id to the more than max number posssible
    // which is the (no_nodes + 1).
    low_ids_[w] = directed_graph_.count();
    stack_.pop();
  } while (w != v);

  // Increment current no of component counter.
  no_of_components_++;
}

std::uint64_t StronglyConnectedComponents::getComponentId(DirectedGraph::node_id node_id) const {
  return component_ids_[node_id];
}

std::uint64_t StronglyConnectedComponents::getTotalComponents() const {
  return no_of_components_;
}

std::unordered_map<std::uint64_t, std::vector<DirectedGraph::node_id>>
    StronglyConnectedComponents::getComponentMapping() const {
  std::unordered_map<std::uint64_t, std::vector<DirectedGraph::node_id>> component_mapping;
  for (std::uint64_t i = 0; i < component_ids_.size(); ++i) {
    component_mapping[component_ids_[i]].push_back(i);
  }
  return component_mapping;
}

}  // namespace transaction

}  // namespace quickstep
