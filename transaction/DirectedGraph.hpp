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

#ifndef QUICKSTEP_TRANSACTION_DIRECTED_GRAPH_HPP_
#define QUICKSTEP_TRANSACTION_DIRECTED_GRAPH_HPP_

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <unordered_set>
#include <vector>

#include "transaction/Transaction.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace transaction {

/** \addtogroup Transaction
 *  @{
 */

/**
 * @brief Class for representing a directed graph. Vertices are transaction
 *        ids, edges are wait-for relations.
 **/
class DirectedGraph {
 public:
  typedef std::uint64_t node_id;

  /**
   * @brief Default constructor
   **/
  DirectedGraph() {}

  /**
   * @brief Adds a new node to the graph with the given transaction id.
   *        It does not check whether the transaction id is valid or not.
   *
   * @param transaction_id_payload Transaction id that will be contained
   *        in the node.
   * @return Id of the newly created node.
   **/
  inline
  node_id addNodeUnchecked(const transaction_id transaction_id_payload) {
    nodes_.emplace_back(transaction_id_payload);
    return nodes_.size() - 1;
  }

  /**
   * @brief Adds a new node to the graph with the given transaction id.
   *        It checks whether the transaction id is valid or not.
   *
   * @param transaction_id_payload Transaction id that will be contained
   *        in the node.
   * @return Id of the newly created node.
   **/
  node_id addNodeCheckExists(const transaction_id transaction_id_payload) {
    for (const auto &node : nodes_) {
      CHECK(transaction_id_payload != node.getData());
    }

    nodes_.emplace_back(transaction_id_payload);
    return nodes_.size() - 1;
  }

  /**
   * @brief Adds an edge between nodes. It does not check whether the
   *        parameters are valid node ids.
   * @warning Does not check arguments are legit. It may cause
   *          out of range errors.
   *
   * @param fromNode The node that edge is originated.
   * @param toNode The node that edge is ended.
   **/
  void addEdgeUnchecked(node_id from_node, node_id to_node) {
    nodes_[from_node].addOutgoingEdge(to_node);
  }

  /**
   * @brief Adds an edge between nodes. It checks whether the
   *        parameters are valid node ids.
   *
   * @param fromNode The node that edge is orginated.
   * @param toNode The node that edge is ended.
   **/
  void addEdgeCheckExists(node_id from_node, node_id to_node) {
    CHECK(from_node < getNumNodes() && to_node < getNumNodes());
    nodes_[from_node].addOutgoingEdge(to_node);
  }

  /**
   * @brief Check whether there is a directed edge.
   * @warning When parameters are not valid ids, it may cause
   *          out of range errors.
   *
   * @param fromNode Id of the node that edge is originated from.
   * @param toNode Id of the node that edge is ended.
   * @return True if there is an edge, false otherwise.
   **/
  bool hasEdge(node_id from_node, node_id to_node) const {
    DCHECK(from_node < getNumNodes() && to_node < getNumNodes());
    return nodes_[from_node].hasOutgoingEdge(to_node);
  }

  /**
   * @brief Get data (transaction id) contained in the node.
   *
   * @param node Id of the node that the data is got from.
   * @return Id of the transaction that this node contains.
   **/
  transaction_id getDataFromNode(node_id node) const {
    DCHECK(node < getNumNodes());
    return nodes_[node].getData();
  }

  /**
   * @brief Calculate how many nodes the graph has.
   *
   * @return The number of nodes the graph has.
   **/
  std::size_t getNumNodes() const {
    return nodes_.size();
  }

  /**
   * @brief Gives the node ids that this node has edges to.
   *
   * @param id Id of the corresponding node.
   * @return Vector of node ids that id has edges to.
   **/
  inline std::vector<node_id> getAdjacentNodes(node_id id) const {
    return nodes_[id].getOutgoingEdges();
  }

 private:
  // Class for representing a graph node.
  class DirectedGraphNode {
   public:
    explicit DirectedGraphNode(const transaction_id payload)
      : transaction_id_payload_(payload) {}

    void addOutgoingEdge(node_id to_node) {
      outgoing_edges_.insert(to_node);
    }

    bool hasOutgoingEdge(node_id to_node) const {
      return outgoing_edges_.count(to_node) == 1;
    }

    std::vector<node_id> getOutgoingEdges() const {
      // TODO(hakan): Benchmark this version and the alternative which the
      //              function returns const reference and the uniqueness
      //              is imposed in the outgoing_edges_ as a vector.
      std::vector<node_id> result;
      std::copy(outgoing_edges_.begin(), outgoing_edges_.end(),
                std::back_inserter(result));
      return result;
    }

    transaction_id getData() const {
      return transaction_id_payload_;
    }

   private:
    const transaction_id transaction_id_payload_;

    // Endpoint nodes of outgoing edges originated from this node.
    std::unordered_set<node_id> outgoing_edges_;
  };

  // The list of nodes that are created. NodeId is the index of that
  // node in this buffer.
  std::vector<DirectedGraphNode> nodes_;

  DISALLOW_COPY_AND_ASSIGN(DirectedGraph);
};

/** @} */

}  // namespace transaction
}  // namespace quickstep

#endif  // QUICKSTEP_TRANSACTION_DIRECTED_GRAPH_HPP_
