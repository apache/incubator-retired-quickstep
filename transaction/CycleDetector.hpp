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

#ifndef QUICKSTEP_TRANSACTION_CYCLE_DETECTOR_HPP_
#define QUICKSTEP_TRANSACTION_CYCLE_DETECTOR_HPP_

#include <memory>
#include <unordered_set>
#include <vector>

#include "transaction/DirectedGraph.hpp"
#include "transaction/StronglyConnectedComponents.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace transaction {

/** \addtogroup Transaction
 *  @{
 */

/**
 * @brief Class for running cycle detection algorithm on directed graph.
 */
class CycleDetector {
 public:
  /**
   * @brief Constructor for DirectedGraph.
   *
   * @param wait_for_graph Pointer to a directed wait-for graph.
   */
  explicit CycleDetector(DirectedGraph *wait_for_graph);

  /**
   * @brief Calculate which nodes should be killed to eliminate all cycles
   *        in the graph.
   *
   * @return Vector of node ids that should be killed to break all cycles.
   */
  std::vector<DirectedGraph::node_id> chooseVictimsToBreakCycle() const;

 private:
  std::vector<DirectedGraph::node_id> chooseVictimsInComponent(
      const std::vector<DirectedGraph::node_id> &nodes) const;

  inline DirectedGraph::node_id chooseVictim(
      const std::unordered_set<DirectedGraph::node_id> &nodes_set) const {
    // TODO(Hakan): This is very inefficient scheme, however in the
    //              future, we can use the transaction's priority
    //              as the victim selection parameter.
    return *(nodes_set.begin());
  }

  // Checks whether the nodes in the set make a cycle.
  bool hasCycle(const std::unordered_set<DirectedGraph::node_id> &within) const;

  DirectedGraph *wait_for_graph_;

  std::unique_ptr<StronglyConnectedComponents> strongly_connected_components_;

  DISALLOW_COPY_AND_ASSIGN(CycleDetector);
};

/** @} */

}  // namespace transaction
}  // namespace quickstep

#endif  // QUICKSTEP_TRANSACTION_CYCLE_DETECTOR_HPP_
