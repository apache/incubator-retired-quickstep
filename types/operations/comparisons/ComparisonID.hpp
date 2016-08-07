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

#ifndef QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_COMPARISON_ID_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_COMPARISON_ID_HPP_

#include <type_traits>

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief Concrete Comparisons.
 **/
enum class ComparisonID {
  kEqual = 0,
  kNotEqual,
  kLess,
  kLessOrEqual,
  kGreater,
  kGreaterOrEqual,
  kLike,
  kNotLike,
  kRegexMatch,
  kNotRegexMatch,
  kNumComparisonIDs  // Not a real ComparisonID, exists for counting purposes.
};

/**
 * @brief Names of comparisons in the same order as BinaryOperationID.
 **/
extern const char *kComparisonNames[
    static_cast<typename std::underlying_type<ComparisonID>::type>(
        ComparisonID::kNumComparisonIDs)];

/**
 * @brief Short names (i.e. mathematical symbols) of comparisons in the same
 *        order as BinaryOperationID.
 **/
extern const char *kComparisonShortNames[
    static_cast<typename std::underlying_type<ComparisonID>::type>(
        ComparisonID::kNumComparisonIDs)];

/**
 * @brief Flips a comparison.
 *
 * As in greater than flips to less than, less than flips to greater than, and
 * similarly for greater/less than or equals. Notice that flipping equals
 * results in equals, same for not equals.
 *
 * @param comparison The Id of a comparison to flip.
 * @return The flipped comparison id.
 */
inline ComparisonID flipComparisonID(const ComparisonID comparison) {
  switch (comparison) {
    case ComparisonID::kLess:
      return ComparisonID::kGreater;
    case ComparisonID::kLessOrEqual:
      return ComparisonID::kGreaterOrEqual;
    case ComparisonID::kGreater:
      return ComparisonID::kLess;
    case ComparisonID::kGreaterOrEqual:
      return ComparisonID::kLessOrEqual;
    case ComparisonID::kLike:
    case ComparisonID::kNotLike:
    case ComparisonID::kRegexMatch:
    case ComparisonID::kNotRegexMatch:
      LOG(FATAL) << "Cannot flip pattern matching comparison.";
    default:
      return comparison;
  }
}

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_COMPARISON_ID_HPP_
