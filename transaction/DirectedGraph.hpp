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

#ifndef QUICKSTEP_TRANSACTION_DIRECTED_GRAPH_HPP_
#define QUICKSTEP_TRANSACTION_DIRECTED_GRAPH_HPP_

#include <algorithm>
#include <memory>
#include <stack>
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
   * @warning Pointer ownership will pass to the graph, therefore it
   *          should not be deleted.
   *
   * @param data Pointer to the transaction id that will be contained
   *        in the node.
   * @return Id of the newly created node.
   **/
  inline node_id addNodeUnchecked(transaction_id *data) {
    nodes_.emplace_back(data);
    return nodes_.size() - 1;
  }

  /**
   * @brief Adds a new node to the graph with the given transaction id.
   *        It checks whether the transaction id is valid or not.
   * @warning Pointer ownership will pass to the graph, therefore it
   *          should not be deleted.
   *
   * @param data Pointer to the transaction id that will be contained
   *        in the node.
   * @return Id of the newly created node.
   **/
  inline node_id addNodeCheckExists(transaction_id *data) {
    for (std::vector<DirectedGraphNode>::const_iterator
           it = nodes_.cbegin(); it != nodes_.cend(); ++it) {
      CHECK(*data != it->getData());
    }
    nodes_.emplace_back(data);
    return nodes_.size() - 1;
  }

  /**
   * @brief Adds an edge between nodes. It does not check whether the
   *        parameters are valid node ids.
   * @warning Does not check arguments are legit. It may cause
   *          out of range errors.
   *
   * @param fromNode The node that edge is orginated.
   * @param toNode The node that edge is ended.
   **/
  inline void addEdgeUnchecked(node_id from_node, node_id to_node) {
    nodes_[from_node].addOutgoingEdge(to_node);
  }

  /**
   * @brief Adds an edge between nodes. It checks whether the
   *        parameters are valid node ids.
   *
   * @param fromNode The node that edge is orginated.
   * @param toNode The node that edge is ended.
   **/
  inline void addEdgeCheckExists(node_id from_node, node_id to_node) {
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
  inline bool hasEdge(node_id from_node, node_id to_node) const {
    DCHECK(from_node < getNumNodes() && to_node < getNumNodes());
    return nodes_[from_node].hasOutgoingEdge(to_node);
  }

  /**
   * @brief Get data (transaction id) contained in the node.
   * @warning Does not check index validity.
   *
   * @param node Id of the node that the data is got from.
   * @return Id of the transaction that this node contains.
   **/
  inline transaction_id getDataFromNode(node_id node) const {
    return nodes_[node].getData();
  }

  /**
   * @brief Calculate how many nodes the graph has.
   *
   * @return The number of nodes the graph has.
   **/
  inline std::size_t getNumNodes() const {
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
    explicit DirectedGraphNode(transaction_id *data)
      : data_(data) {}

    inline void addOutgoingEdge(node_id to_node) {
      outgoing_edges_.insert(to_node);
    }

    inline bool hasOutgoingEdge(node_id to_node) const {
      return outgoing_edges_.count(to_node) == 1;
    }

    inline std::vector<node_id> getOutgoingEdges() const {
      // TODO(hakan): Benchmark this version and the alternative which the
      //              function returns const reference and the uniqueness
      //              is imposed in the outgoing_edges_ as a vector.
      std::vector<node_id> result;
      std::copy(outgoing_edges_.begin(), outgoing_edges_.end(),
                std::back_inserter(result));
      return result;
    }

    inline transaction_id getData() const {
      return *(data_.get());
    }

   private:
    // Owner pointer to transaction id.
    std::unique_ptr<transaction_id> data_;

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
