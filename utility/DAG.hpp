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

#ifndef QUICKSTEP_UTILITY_DAG_HPP_
#define QUICKSTEP_UTILITY_DAG_HPP_

#include <cstddef>
#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief A directed acyclic graph whose nodes each contain a payload. DAG owns
 *        all node payloads. The link metadata should be copyable.
 **/
template<class T, class LinkMetadataT>
class DAG {
 private:
  class DAGNode;

 public:
  typedef typename std::vector<DAGNode>::size_type size_type_nodes;
  typedef typename std::unordered_set<size_type_nodes>::const_iterator const_iterator_dependencies;
  typedef typename std::unordered_map<size_type_nodes, LinkMetadataT>::const_iterator const_iterator_dependents;

  /**
   * @brief Constructor.
   **/
  DAG() {
  }

  /**
   * @brief Returns the payload of the specified node.
   *
   * @param node_index The index of the specified node.
   *
   * @return A const reference to the payload of the specified node.
   **/
  inline const T& getNodePayload(const size_type_nodes node_index) const {
    DCHECK_LT(node_index, nodes_.size());
    return nodes_[node_index].getPayload();
  }

  /**
   * @brief Returns the (mutable) pointer to payload of the specified node.
   *
   * @param node_index The index of the specified node.
   *
   * @return A mutable Pointer to the payload of the specified node.
   **/
  inline T* getNodePayloadMutable(const size_type_nodes node_index) {
    DCHECK_LT(node_index, nodes_.size());
    return nodes_[node_index].getPayloadMutable();
  }

  /**
   * @brief Get the nodes dependent on the specified node.
   *
   * @param node_index The index of the specified node.
   *
   * @return An unordered set made of the indices of the dependents of the
   *         specified node.
   **/
  inline const std::unordered_map<size_type_nodes, LinkMetadataT>& getDependents(
      const size_type_nodes node_index) const {
    DCHECK_LT(node_index, nodes_.size());
    return nodes_[node_index].getDependents();
  }

  /**
   * @brief Get the dependencies of the specified node.
   *
   * @param node_index The index of the specified node.
   *
   * @return An unordered set made of the indices of the dependencies of the
   *         specified node.
   **/
  inline const std::unordered_set<size_type_nodes>& getDependencies(const size_type_nodes node_index) const {
    DCHECK_LT(node_index, nodes_.size());
    return nodes_[node_index].getDependencies();
  }

  /**
   * @brief Adds a node with the specified paylod to the DAG. DAG takes
   *        ownership of payload
   *
   * @param payload A pointer to the payload.
   *
   * @return The index of the new DAGNode in the DAG.
   **/
  size_type_nodes createNode(T *payload) {
    nodes_.emplace_back(payload);
    return nodes_.size() - 1;
  }

  /**
   * @brief Connects a dependent to dependancy and vice versa.
   *
   * @param dependency_index Index of the dependency node in the DAG.
   * @param dependent_index Index of the dependent node in the DAG.
   * @param link_metadata The metadata of the link between dependency_index
   *                    and dependent_index.
   **/
  void createLink(const size_type_nodes dependency_index,
                  const size_type_nodes dependent_index,
                  const LinkMetadataT &link_metadata) {
    DCHECK_LT(dependency_index, nodes_.size());
    DCHECK_LT(dependent_index, nodes_.size());
    nodes_[dependency_index].addDependent(dependent_index, link_metadata);
    nodes_[dependent_index].addDependency(dependency_index);
  }

  /**
   * @brief Set the metadata of the link between dependency and dependent node.
   *
   * @note As this is a directed graph, this method adds metadata to only a
   *       single directed edge.
   *
   * @note This method is meant to modify the metadata of an existing link
   *       between the two nodes.
   *
   * @param dependency_index Index of the dependency node in the DAG.
   * @param dependent_index Index of the dependent node in the DAG.
   * @param link_metadata The metadata of the link between dependency_index
   *                    and dependent_index.
   **/
  void setLinkMetadata(const size_type_nodes dependency_index,
                       const size_type_nodes dependent_index,
                       const LinkMetadataT &link_metadata) {
    nodes_[dependency_index].setLinkMetadata(dependent_index, link_metadata);
  }

  /**
   * @brief Get the metadata of the link between two nodes.
   *
   * @param dependency_index Index of the dependency node in the DAG.
   * @param dependent_index Index of the dependent node in the DAG.
   **/
  const LinkMetadataT& getLinkMetadata(const size_type_nodes dependency_index,
                                       const size_type_nodes dependent_index) {
    return nodes_[dependency_index].getLinkMetadata(dependent_index);
  }

  /**
   * @brief Divides the nodes into a sequence of stages where each stage is a
   *        pair of vectors of node indices.
   *
   * @return A pair of vectors of node indices. The first vector is the indices
   *         of the nodes that can start at the stage, the second is the
   *         indices of the nodes that must finish in (or before) the stage.
   **/
  const std::vector<std::pair<std::vector<size_type_nodes>,
      std::vector<size_type_nodes> > > computeStageSequence() const;

  /**
   * @brief Checks if there is a cycle in the directed graph.
   *
   * @return True if a cycle is present, false if cycle is absent.
   **/
  const bool hasCycle() const;

  /**
   * @brief Get the number of nodes in the DAG.
   **/
  const size_type_nodes size() const {
    return nodes_.size();
  }

  /**
   * @brief Get a begin iterator on the node IDs of the dependencies of the node
   *        at the given index.
   *
   * @param node_index The index of the given node.
   *
   * @return A const begin iterator on the list of node IDs of the dependencies
   *         of the node at the given index.
   **/
  inline const_iterator_dependencies begin_dependencies(size_type_nodes node_index) const {
    return nodes_[node_index].getDependencies().begin();
  }

  /**
   * @brief Get a begin iterator on the node IDs of the dependents. of the node
   *        at the given index.
   *
   * @param node_index The index of the given node.
   *
   * @return A const begin iterator on the list of node IDs of the dependents
   *         of the node at the given index.
   **/
  inline const_iterator_dependents begin_dependents(size_type_nodes node_index) const {
    return nodes_[node_index].getDependents().begin();
  }

  /**
   * @brief Get an end iterator on the node IDs of the dependencies of the node
   *        at the given index.
   *
   * @param node_index The index of the given node.
   *
   * @return A const end iterator on the list of node IDs of the dependencies of
   *         the node at the given index.
   **/
  inline const_iterator_dependencies end_dependencies(size_type_nodes node_index) const {
    return nodes_[node_index].getDependencies().end();
  }

  /**
   * @brief Get an end iterator on the node IDs of the dependents of the node
   *        at the given index.
   *
   * @param node_index The index of the given node.
   *
   * @return A const end iterator on the list of node IDs of the dependents of
   *         the node at the given index.
   **/
  inline const_iterator_dependents end_dependents(size_type_nodes node_index) const {
    return nodes_[node_index].getDependents().end();
  }

  /**
   * @brief Get a topologically sorted list of node IDs.
   **/
  std::vector<size_type_nodes> getTopologicalSorting() const;

 private:
  /**
   * @brief A node in the DAG which contains a payload. DAGNode owns its
   * payload.
   **/
  class DAGNode {
   public:
     /**
      * @brief Constructor.
      *
      * @param payload A data payload that the DAGNode owns.
      **/
    explicit DAGNode(T *payload)
        : payload_(payload) {
    }

    /**
     * @brief Default move constructor.
     **/
    DAGNode(DAGNode &&other) = default;

    /**
     * @brief Returns the set of dependencies on the node.
     *
     * @return The unordered set of dependencies.
     **/
    inline const std::unordered_set<size_type_nodes>& getDependencies() const {
      return dependencies_;
    }

    /** @brief Returns the set of dependents on the node.
     *
     * @return The unordered set of dependents.
     **/
    inline const std::unordered_map<size_type_nodes, LinkMetadataT>& getDependents() const {
      return dependents_with_metadata_;
    }

    /**
     * @brief Adds the index of dependent node in vector of nodes from DAG and
     *        adds the link metadata.
     *
     * @param node_index Index of dependent node in vector of nodes from DAG.
     * @param link_metadata The metadata of the link between this node and the
     *                      node at node_index.
     **/
     inline void addDependent(const size_type_nodes node_index, const LinkMetadataT &link_metadata) {
       DCHECK(dependents_with_metadata_.find(node_index) == dependents_with_metadata_.end());
       dependents_with_metadata_.emplace(node_index, link_metadata);
     }

    /**
     * @brief Set the metadata of the edge between this node and the node at
     *        node_index.
     *
     * @param node_index The index of the dependent node of this node.
     * @param link_metadata The new metadata of the edge.
     **/
    inline void setLinkMetadata(const size_type_nodes node_index, const LinkMetadataT &link_metadata) {
      DCHECK(dependents_with_metadata_.find(node_index) != dependents_with_metadata_.end());
      dependents_with_metadata_[node_index] = link_metadata;
    }

    /**
     * @brief Gets the metadata of the edge between this node and the node at
     *        node_index.
     *
     * @param node_index The index of the dependent node of this node.
     *
     * @return The metadata of the link.
     **/
    inline const LinkMetadataT& getLinkMetadata(const size_type_nodes node_index) {
      DCHECK(dependents_with_metadata_.find(node_index) != dependents_with_metadata_.end());
      return dependents_with_metadata_[node_index];
    }

    /**
     * @brief Adds the index of dependency node in vector of nodes from DAG.
     *
     * @param node_index Index of dependency node in vector of nodes from DAG.
     **/
    inline void addDependency(const size_type_nodes node_index) {
      dependencies_.insert(node_index);
    }

    /**
     * @brief Returns a pointer to the payload represented by the DAG node.
     *
     * @return Pointer to the payload represented by the DAG node.
     **/
    inline T* getPayloadMutable() {
      return payload_.get();
    }

    /**
     * @return A const reference to the payload.
     **/
    inline const T& getPayload() const {
      return *payload_;
    }

   private:
    std::unique_ptr<T> payload_;
    std::unordered_set<size_type_nodes> dependencies_;

    // Key: Dependent node's index.
    // Value: The metadata of the edge between this node and key.
    std::unordered_map<size_type_nodes, LinkMetadataT> dependents_with_metadata_;

    DISALLOW_COPY_AND_ASSIGN(DAGNode);
  };

  bool hasCycleHelper(const size_type_nodes node_index, std::vector<bool> *visited, std::vector<bool> *on_stack) const;

  std::vector<DAGNode> nodes_;

  DISALLOW_COPY_AND_ASSIGN(DAG);
};

template <class T, class LinkMetadataT>
const bool DAG<T, LinkMetadataT>::hasCycle() const {
  std::vector<bool> visited;
  std::vector<bool> on_stack;
  visited.resize(nodes_.size(), false);
  on_stack.resize(nodes_.size(), false);
  // Checking all nodes is required for disconnected components in DAG if any.
  for (DAG<T, LinkMetadataT>::size_type_nodes curr_node_index = 0; curr_node_index < nodes_.size(); ++curr_node_index) {
    if (hasCycleHelper(curr_node_index, &visited, &on_stack)) {
      return true;
    }
  }
  return false;
}

template <class T, class LinkMetadataT>
const std::vector<std::pair<std::vector<typename DAG<T, LinkMetadataT>::size_type_nodes>,
                  std::vector<typename DAG<T, LinkMetadataT>::size_type_nodes> > >
                  DAG<T, LinkMetadataT>::computeStageSequence() const {
  // The algorithm operates in two passes. First, it traverses the graph
  // structure backwards, starting at the nodes with no dependencies and
  // determines the finish-stage for each node. In the second pass, each node
  // is assigned a start stage, which is the maximum of the finish stages of
  // its dependencies, plus one (nodes with no dependencies get a start stage
  // of 0).
  DCHECK(!hasCycle());
  std::vector<std::size_t> node_finish_stage_offsets;  // negative offset from end of stages
  std::vector<std::size_t> node_start_stages;
  const std::size_t num_nodes = nodes_.size();

  node_start_stages.resize(num_nodes, 0);
  node_finish_stage_offsets.resize(num_nodes, 0);

  std::unordered_set<typename DAG<T, LinkMetadataT>::size_type_nodes> current_stage_finish;
  for (DAG<T, LinkMetadataT>::size_type_nodes node_index = 0;
       node_index < num_nodes;
       ++node_index) {
    if (nodes_[node_index].getDependents().empty()) {
      node_finish_stage_offsets[node_index] = 1;
      current_stage_finish.insert(node_index);
    }
  }

  std::size_t num_stages = 1;
  while (!current_stage_finish.empty()) {
    std::unordered_set<typename DAG<T, LinkMetadataT>::size_type_nodes> prior_stage_finish;
    ++num_stages;
    for (typename std::unordered_set<typename DAG<T, LinkMetadataT>::size_type_nodes>::const_iterator
             current_node_iterator = current_stage_finish.begin();
         current_node_iterator != current_stage_finish.end();
         ++current_node_iterator) {
      // This will sometimes overwrite an already set finish_stage_offset to
      // be even earlier. This is the correct behavior.
      const DAGNode &current_dag_node = nodes_[*current_node_iterator];
      for (typename std::unordered_set<typename DAG<T, LinkMetadataT>::size_type_nodes>::const_iterator
               dependency_node_iterator = current_dag_node.getDependencies().begin();
           dependency_node_iterator != current_dag_node.getDependencies().end();
           ++dependency_node_iterator) {
        node_finish_stage_offsets[*dependency_node_iterator] = num_stages;
        // Don't check the return value of insert. If it fails, that means
        // that this dependency_node is already in prior_stage_finish
        // because it's a dependency of another node in current_stage_finish.
        prior_stage_finish.insert(*dependency_node_iterator);
      }
    }
    current_stage_finish.swap(prior_stage_finish);
  }
  // The final iteration of the above loop doesn't actually make a new stage,
  // so adjust num_stages to be correct.
  --num_stages;

  for (DAG<T, LinkMetadataT>::size_type_nodes node_index = 0;
       node_index < num_nodes;
       ++node_index) {
    DAG<T, LinkMetadataT>::size_type_nodes node_start_stage = 0;
    const DAGNode &dag_node_at_index = nodes_[node_index];
    for (typename std::unordered_set<typename DAG<T, LinkMetadataT>::size_type_nodes>::const_iterator
             dependency_node_iterator = dag_node_at_index.getDependencies().begin();
         dependency_node_iterator != dag_node_at_index.getDependencies().end();
         ++dependency_node_iterator) {
      std::size_t candidate_start_stage = num_stages - node_finish_stage_offsets[*dependency_node_iterator] + 1;
      if (candidate_start_stage > node_start_stage) {
        node_start_stage = candidate_start_stage;
      }
    }
    node_start_stages[node_index] = node_start_stage;
  }

  std::vector<std::pair<std::vector<typename DAG<T, LinkMetadataT>::size_type_nodes>,
      std::vector<typename DAG<T, LinkMetadataT>::size_type_nodes> > > stages;

  stages.resize(num_stages);
  for (std::size_t node_index_pushed = 0;
       node_index_pushed < num_nodes;
       ++node_index_pushed) {
    stages[node_start_stages[node_index_pushed]].first.push_back(node_index_pushed);
    stages[num_stages - node_finish_stage_offsets[node_index_pushed]].second.push_back(node_index_pushed);
  }
  return stages;
}

template <class T, class LinkMetadataT>
bool DAG<T, LinkMetadataT>::hasCycleHelper(const typename DAG<T, LinkMetadataT>::size_type_nodes node_index,
                             std::vector<bool> *visited,
                             std::vector<bool> *on_stack) const {
  if (!(*visited)[node_index]) {  // Skip if already visited
    (*visited)[node_index] = true;
    (*on_stack)[node_index] = true;
    const DAG<T, LinkMetadataT>::DAGNode &dependent_node = nodes_[node_index];
    for (auto dependent_iterator = dependent_node.getDependents().begin();
         dependent_iterator != dependent_node.getDependents().end();
         ++dependent_iterator) {
      // For all unvisited dependents of the current node in the DAG check
      // for a cycle involving them by recursively calling hasCycleHelper.
      if ((!(*visited)[dependent_iterator->first] && hasCycleHelper(dependent_iterator->first, visited, on_stack))
          || (*on_stack)[dependent_iterator->first]) {
        return true;
      }
    }
  }
  (*on_stack)[node_index] = false;
  return false;
}

template <class T, class LinkMetadataT>
std::vector<typename DAG<T, LinkMetadataT>::size_type_nodes>
DAG<T, LinkMetadataT>::getTopologicalSorting() const {
  // As a clarification, if A->B then A is the dependency for B and B is dependent on A.
  // We implement "Kahn's algorithm" for the sorting.
  DCHECK(!hasCycle());
  // This list is going to be the topologically sorted output.
  std::unique_ptr<std::vector<typename DAG<T, LinkMetadataT>::size_type_nodes>>
      sorted_list(new std::vector<size_type_nodes>());
  sorted_list->reserve(this->size());
  // Key = node ID, value = # incoming edges for this node.
  // NOTE(harshad) - We modify the "values" in this map as we go along.
  std::unordered_map<typename DAG<T, LinkMetadataT>::size_type_nodes,
                     std::size_t> num_dependencies;
  std::queue<typename DAG<T, LinkMetadataT>::size_type_nodes> nodes_with_no_dependencies;
  // First store the nodes without any dependencies in a list.
  // Also remember the number of dependencies for each node in a map.
  for (auto node_id = 0u; node_id < this->size(); ++node_id) {
    if (nodes_[node_id].getDependencies().empty()) {
      nodes_with_no_dependencies.emplace(node_id);
    }
    num_dependencies[node_id] = nodes_[node_id].getDependencies().size();
  }
  // The algorithm begins now.
  while (!nodes_with_no_dependencies.empty()) {
    // For a node with no dependencies ...
    auto curr_node = nodes_with_no_dependencies.front();
    nodes_with_no_dependencies.pop();
    // Add the node to the sorted list.
    sorted_list->emplace_back(curr_node);
    auto dependents_of_curr_node = getDependents(curr_node);
    for (auto dependent_iterator : dependents_of_curr_node) {
      // For each dependent of the current node ...
      auto dependent_node_id = dependent_iterator.first;
      // Remove the incoming edge from curr_node.
      DCHECK_GE(num_dependencies[dependent_node_id], 1u);
      if (--num_dependencies[dependent_node_id] == 0) {
        // Now this node has no children.
        nodes_with_no_dependencies.emplace(dependent_node_id);
      }
    }
  }
  return *(sorted_list.release());
}

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_DAG_HPP_
