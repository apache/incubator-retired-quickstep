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

#ifndef QUICKSTEP_UTILITY_DISJOINT_TREE_FOREST_HPP_
#define QUICKSTEP_UTILITY_DISJOINT_TREE_FOREST_HPP_

#include <algorithm>
#include <cstddef>
#include <unordered_map>
#include <vector>

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief Disjoint sets implemented with tree data structures so that the
 *        union/find operations have nearly O(1) time complexity.
 */
template <typename ElementT,
          class MapperT = std::unordered_map<ElementT, std::size_t>>
class DisjointTreeForest {
 public:
  /**
   * @brief Whether the given element is in a subset.
   *
   * @param element The element.
   * @return True if the element is in a subset.
   */
  bool hasElement(const ElementT &element) const {
    return elements_map_.find(element) != elements_map_.end();
  }

  /**
   * @brief If the given element is not in any subset yet, make a singleton
   *        subset for it. Otherwise do nothing.
   *
   * @param element The element.
   */
  void makeSet(const ElementT &element) {
    if (!hasElement(element)) {
      std::size_t loc = nodes_.size();
      nodes_.emplace_back(0, loc);
      elements_map_.emplace(element, loc);
    }
  }

  /**
   * @brief Find the subset id for the given element.
   *
   * @param element The element.
   */
  std::size_t find(const ElementT &element) {
    DCHECK(hasElement(element));

    const std::size_t node_id = elements_map_.at(element);
    std::size_t root_id = node_id;
    std::size_t parent_id;
    while ((parent_id = nodes_[root_id].parent) != root_id) {
      root_id = parent_id;
    }
    compress_path(node_id, root_id);
    return root_id;
  }

  /**
   * @brief Union the two subsets that the two given elements belong to.
   *
   * @param element1 The first element.
   * @param element2 The second element.
   */
  void merge(const ElementT &element1, const ElementT &element2) {
    std::size_t root_id1 = find(element1);
    std::size_t root_id2 = find(element2);
    if (root_id1 != root_id2) {
      Node &n1 = nodes_[root_id1];
      Node &n2 = nodes_[root_id2];
      if (n1.rank > n2.rank) {
        n2.parent = root_id1;
      } else if (n1.rank < n2.rank) {
        n1.parent = root_id2;
      } else {
        n1.parent = root_id2;
        n2.rank += 1;
      }
    }
  }

  /**
   * @brief Whether the two given elements are in the same subset.
   *
   * @param element1 The first element.
   * @param element2 The second element.
   * @return True if the twos elements are in the same subset, false otherwise.
   */
  bool isConnected(const ElementT &element1, const ElementT &element2) {
    return find(element1) == find(element2);
  }

 private:
  struct Node {
    Node(const std::size_t rank_in, const std::size_t parent_in)
        : rank(rank_in), parent(parent_in) {
    }
    std::size_t rank;
    std::size_t parent;
  };

  inline void compress_path(const std::size_t leaf_node_id,
                            const std::size_t root_node_id) {
    std::size_t node_id = leaf_node_id;
    std::size_t max_rank = 0;
    while (node_id != root_node_id) {
      const Node &node = nodes_[node_id];
      max_rank = std::max(max_rank, node.rank);

      const std::size_t parent_id = node.parent;
      nodes_[node_id].parent = root_node_id;
      node_id = parent_id;
    }
    nodes_[root_node_id].rank = max_rank + 1;
  }

  std::vector<Node> nodes_;
  MapperT elements_map_;
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_DISJOINT_TREE_FOREST_HPP_
