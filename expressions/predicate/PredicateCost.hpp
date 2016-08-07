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

#ifndef QUICKSTEP_EXPRESSIONS_PREDICATE_PREDICATE_COST_HPP_
#define QUICKSTEP_EXPRESSIONS_PREDICATE_PREDICATE_COST_HPP_

#include <cstdint>
#include <limits>

namespace quickstep {

/**
 * @brief Represents a simple estimation of the cost to evaluate a
 *        ComparisonPredicate using a particular sub-block.
 **/
typedef std::uint8_t predicate_cost_t;

namespace predicate_cost {

// TODO(chasseur): For now, these static constants define an order of
// preference for different predicate-evaluation methods. In the future, we may
// want to use statistics to get more accurate cost estimation.
static constexpr predicate_cost_t kConstantTime = 0;
static constexpr predicate_cost_t kBitWeavingVScan = 1;
static constexpr predicate_cost_t kBitWeavingHScan = 2;
static constexpr predicate_cost_t kBinarySearch = 3;
static constexpr predicate_cost_t kTreeSearch = 4;
static constexpr predicate_cost_t kCompressedColumnScan = 5;
static constexpr predicate_cost_t kColumnScan = 6;
static constexpr predicate_cost_t kCompressedRowScan = 7;
static constexpr predicate_cost_t kRowScan = 8;
static constexpr predicate_cost_t kInfinite = std::numeric_limits<predicate_cost_t>::max();

}  // namespace predicate_cost

/**
 * @brief Determine if a predicate_cost_t represents a simple scan on a
 *        TupleStorageSubBlock.
 **/
inline bool PredicateCostIsSimpleScan(const predicate_cost_t cost) {
  return (cost == predicate_cost::kRowScan) || (cost == predicate_cost::kColumnScan);
}

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_PREDICATE_PREDICATE_COST_HPP_
