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

#ifndef QUICKSTEP_TRANSACTION_STRONGLY_CONNECTED_COMPONENTS_HPP_
#define QUICKSTEP_TRANSACTION_STRONGLY_CONNECTED_COMPONENTS_HPP_

#include <cstddef>
#include <cstdint>
#include <stack>
#include <unordered_map>
#include <vector>

#include "transaction/DirectedGraph.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace transaction {

/** \addtogroup Transaction
 *  @{
 */

/**
 * @brief Class for applying Tarjan's strongly connected components algorithm
 *        to a directed graph. The class finds and stores information to
 *        identify components and nodes-to-component mapping. For all nodes
 *        v, w in a strongly connected component, there is a path from v to w,
 *        and a path from w to v.
 **/
class StronglyConnectedComponents {
 public:
  /**
   * @brief Constructor.
   *
   * @param directed_graph The directed graph which the algorithm runs.
   **/
  explicit StronglyConnectedComponents(const DirectedGraph &directed_graph);

  /**
   * @brief Getter for the component id of the node.
   *
   * @param node_id Id of the node in the graph.
   * @return Id of the component to which the given node belongs.
   **/
  inline std::uint64_t getComponentId(DirectedGraph::node_id node_id) const {
    return component_ids_[node_id];
  }

  /**
   * @brief Getter for total number of strongly connected components.
   * @warning This method should be called after findStronglyConnectedComponents().
   *
   * @return Total number of strongly connected components.
   **/
  inline std::size_t getTotalComponents() const {
    return no_of_components_;
  }

  /**
   * @brief Gets a component id to list of node ids mapping.
   *
   * @return Mapping of component ids to list of nodes.
   **/
  std::unordered_map<std::uint64_t, std::vector<DirectedGraph::node_id>> getComponentMapping() const;

 private:
  // Enumarates all strongly connected components, and stores node-to-component
  // mapping in the graph.
  void findStronglyConnectedComponents();

  // Applies DFS to the node whic has id v.
  void depthFirstSearch(DirectedGraph::node_id v);

  // Const reference to the graph.
  const DirectedGraph &directed_graph_;

  // is_marked[v] == true if depth first search traversed the node with id v.
  std::vector<bool> is_marked_;

  // component_ids_[v] == i if strongly connected component
  // which the node with id v belongs to, has id i.
  std::vector<std::uint64_t> component_ids_;

  // low_ids_[v] == i if lowest traversal id seen by the node
  // with id v equals i.
  std::vector<std::uint64_t> low_ids_;

  // Keeps track of the nodes in the current traversal of the graph.
  std::stack<DirectedGraph::node_id> traversal_stack_;

  // Keeps track of the current index number of depth first search traversal.
  std::uint64_t preorder_counter_;

  // Keeps track of the current number of the strongly connected component.
  std::uint64_t no_of_components_;

  DISALLOW_COPY_AND_ASSIGN(StronglyConnectedComponents);
};

/** @} */

}  // namespace transaction
}  // namespace quickstep

#endif  // QUICKSTEP_TRANSACTION_STRONGLY_CONNECTED_COMPONENTS_HPP_
