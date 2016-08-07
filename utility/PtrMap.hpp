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

#ifndef QUICKSTEP_UTILITY_PTR_MAP_HPP_
#define QUICKSTEP_UTILITY_PTR_MAP_HPP_

#include <stdexcept>
#include <unordered_map>
#include <utility>

#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief A map (unordered_map where possible) whose values are pointers which
 *        are automatically deleted when the map goes out of scope.
 * @note Iterators are regular iterators from the underlying map type, meaning
 *       that they are iterators of pair<const K, V*>.
 **/
template <typename K, typename V>
class PtrMap {
 public:
  typedef typename std::unordered_map<K, V*>::iterator iterator;
  typedef typename std::unordered_map<K, V*>::const_iterator const_iterator;
  typedef typename std::unordered_map<K, V*>::size_type size_type;

  PtrMap() {
  }

  ~PtrMap() {
    for (typename std::unordered_map<K, V*>::iterator it = internal_map_.begin();
         it != internal_map_.end();
         ++it) {
      delete it->second;
    }
  }

  inline bool empty() const {
    return internal_map_.empty();
  }

  inline size_type size() const {
    return internal_map_.size();
  }

  // Iterators.
  iterator begin() {
    return internal_map_.begin();
  }

  const_iterator begin() const {
    return internal_map_.begin();
  }

  iterator end() {
    return internal_map_.end();
  }

  const_iterator end() const {
    return internal_map_.end();
  }

  // Insertion.
  std::pair<iterator, bool> insert(const K &key, V *value) {
    return internal_map_.insert(std::pair<const K, V*>(key, value));
  }

  // Lookup.
  inline iterator find(const K &key) {
    return internal_map_.find(key);
  }

  inline const_iterator find(const K &key) const {
    return internal_map_.find(key);
  }

  // Element access.
  inline V& at(const K &key) {
    iterator it = find(key);
    if (it != end()) {
      return *(it->second);
    } else {
      throw std::out_of_range("PtrMap::at() with nonexistent key");
    }
  }

  inline const V& at(const K &key) const {
    const_iterator it = find(key);
    if (it != end()) {
      return *(it->second);
    } else {
      throw std::out_of_range("PtrMap::at() with nonexistent key");
    }
  }

  inline const V& atUnchecked(const K &key) const {
    return *(find(key)->second);
  }

  // Remove all entries from this map, but do not delete values (value pointers
  // should be retained and freed elsewhere to avoid leaking memory).
  void clearAndRelease() {
    internal_map_.clear();
  }

 private:
  std::unordered_map<K, V*> internal_map_;

  DISALLOW_COPY_AND_ASSIGN(PtrMap);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_PTR_MAP_HPP_
