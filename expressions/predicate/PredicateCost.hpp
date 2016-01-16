/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
static constexpr predicate_cost_t kBinarySearch = 1;
static constexpr predicate_cost_t kTreeSearch = 2;
static constexpr predicate_cost_t kCompressedColumnScan = 3;
static constexpr predicate_cost_t kColumnScan = 4;
static constexpr predicate_cost_t kCompressedRowScan = 5;
static constexpr predicate_cost_t kRowScan = 6;
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
