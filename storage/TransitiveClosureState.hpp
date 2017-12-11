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

#ifndef QUICKSTEP_STORAGE_TRANSITIVE_CLOSURE_STATE_HPP_
#define QUICKSTEP_STORAGE_TRANSITIVE_CLOSURE_STATE_HPP_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "utility/BarrieredReadWriteConcurrentBitVector.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Storage
 *  @{
 */

class TransitiveClosureState {
 public:
  explicit TransitiveClosureState(const int range)
      : range_(range),
        edges_(range_) {
  }

  int range() const {
    return range_;
  }

  void initializeStart() {
    DCHECK(starts_ == nullptr);
    starts_ = std::make_unique<BarrieredReadWriteConcurrentBitVector>(range_);
  }

  void initializeEdgeComponent(const int source) {
    DCHECK(edges_[source] == nullptr);
    edges_[source] = std::make_unique<BarrieredReadWriteConcurrentBitVector>(range_);
  }

  inline void addStart(const int value) {
    if (value < range_) {
      starts_->setBit(value);
    }
  }

  inline void addEdge(const int source, const int destination) {
    DCHECK_LT(source, range_);
    DCHECK_LT(destination, range_);
    edges_[source]->setBit(destination);
  }

 private:
  const int range_;
  std::unique_ptr<BarrieredReadWriteConcurrentBitVector> starts_;
  std::vector<std::unique_ptr<BarrieredReadWriteConcurrentBitVector>> edges_;
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_TRANSITIVE_CLOSURE_STATE_HPP_
