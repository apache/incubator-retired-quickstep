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

#ifndef TMB_INTERNAL_THREADSAFE_SET_H_
#define TMB_INTERNAL_THREADSAFE_SET_H_

#include <algorithm>
#include <unordered_set>
#include <utility>
#include <vector>

#include "tmb/internal/rcu.h"

namespace tmb {
namespace internal {

/**
 * @brief A thin threadsafe wrapper for std::unordered_set.
 **/
template <typename T>
class ThreadsafeSet {
 public:
  ThreadsafeSet()
      : internal_set_(new std::unordered_set<T>()) {
  }

  explicit ThreadsafeSet(std::unordered_set<T> *internal_set)
      : internal_set_(internal_set) {
  }

  bool insert(const T &value) {
    typename RCU<std::unordered_set<T>>::WriteHandle write_handle
        = internal_set_.GetWriteHandle();
    bool success = write_handle->insert(value).second;
    if (success) {
      write_handle.Commit();
    }
    return success;
  }

  bool insert(T &&value) {  // NOLINT(build/c++11)
    typename RCU<std::unordered_set<T>>::WriteHandle write_handle
        = internal_set_.GetWriteHandle();
    bool success = write_handle->insert(std::move(value)).second;
    if (success) {
      write_handle.Commit();
    }
    return success;
  }

  bool erase(const T &value) {
    typename RCU<std::unordered_set<T>>::WriteHandle write_handle
        = internal_set_.GetWriteHandle();
    bool success = (write_handle->erase(value) != 0);
    if (success) {
      write_handle.Commit();
    }
    return success;
  }

  bool empty() const {
    typename RCU<std::unordered_set<T>>::ReadHandle read_handle
        = internal_set_.GetReadHandle();
    return read_handle->empty();
  }

  bool contains(const T &value) const {
    typename RCU<std::unordered_set<T>>::ReadHandle read_handle
        = internal_set_.GetReadHandle();
    return read_handle->find(value) != read_handle->end();
  }

  template <typename Function>
  Function for_each(Function &&fn) const {  // NOLINT(build/c++11)
    typename RCU<std::unordered_set<T>>::ReadHandle read_handle
        = internal_set_.GetReadHandle();
    return std::for_each(read_handle->begin(),
                         read_handle->end(),
                         std::forward<Function>(fn));
  }

  template <typename UnaryPredicate>
  bool any_of(UnaryPredicate &&pred) const {  // NOLINT(build/c++11)
    typename RCU<std::unordered_set<T>>::ReadHandle read_handle
        = internal_set_.GetReadHandle();
    return std::any_of(
        read_handle->begin(),
        read_handle->end(),
        std::forward<UnaryPredicate>(pred));
  }

  void CopyIntoVector(std::vector<T> *vec) const {
    typename RCU<std::unordered_set<T>>::ReadHandle read_handle
        = internal_set_.GetReadHandle();
    vec->reserve(vec->size() + read_handle->size());
    vec->insert(vec->end(), read_handle->begin(), read_handle->end());
  }

 private:
  RCU<std::unordered_set<T>> internal_set_;

  // Disallow copy and assign:
  ThreadsafeSet(const ThreadsafeSet &orig) = delete;
  ThreadsafeSet& operator=(const ThreadsafeSet &rhs) = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_THREADSAFE_SET_H_
