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

#include "types/operations/comparisons/PatternMatchingComparison.hpp"

#include <cstddef>
#include <memory>
#include <tuple>
#include <type_traits>

#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "types/operations/comparisons/PatternMatchingComparators.hpp"
#include "types/operations/comparisons/PatternMatchingComparators-inl.hpp"
#include "utility/meta/Dispatchers.hpp"

#include "glog/logging.h"

namespace quickstep {

const PatternMatchingComparison& PatternMatchingComparison::Instance(const ComparisonID sub_type) {
  // Create a static instance for each sub-type of pattern matching comparison.
  switch (sub_type) {
    case ComparisonID::kLike: {
      static PatternMatchingComparison instance(ComparisonID::kLike);
      return instance;
    }
    case ComparisonID::kNotLike: {
      static PatternMatchingComparison instance(ComparisonID::kNotLike);
      return instance;
    }
    case ComparisonID::kRegexMatch: {
      static PatternMatchingComparison instance(ComparisonID::kRegexMatch);
      return instance;
    }
    case ComparisonID::kNotRegexMatch: {
      static PatternMatchingComparison instance(ComparisonID::kNotRegexMatch);
      return instance;
    }
    default:
      LOG(FATAL) << "Invalid ComparisonID: "
                 << kComparisonNames[static_cast<typename std::underlying_type<ComparisonID>::type>(sub_type)]
                 << " for PatternMatchinComparison::Instance()";
  }

  QUICKSTEP_UNREACHABLE();
}

bool PatternMatchingComparison::canCompareTypes(const Type &left,
                                                const Type &right) const {
  // Pattern matching comparison can only apply to CHAR or VARCHAR types.
  return ((left.getTypeID() == TypeID::kChar || left.getTypeID() == TypeID::kVarChar)
          && (right.getTypeID() == TypeID::kChar || right.getTypeID() == TypeID::kVarChar));
}

bool PatternMatchingComparison::canComparePartialTypes(const Type *left_type,
                                                       const Type *right_type) const {
  return false;
}

bool PatternMatchingComparison::compareTypedValuesChecked(const TypedValue &left,
                                                          const Type &left_type,
                                                          const TypedValue &right,
                                                          const Type &right_type) const {
  std::unique_ptr<UncheckedComparator> comparator(
      makeUncheckedComparatorForTypes(left_type, right_type));
  return comparator->compareTypedValues(left, right);
}

UncheckedComparator* PatternMatchingComparison::makeUncheckedComparatorForTypes(const Type &left,
                                                                                const Type &right) const {
  DCHECK(canCompareTypes(left, right));

  // Configure parameters for the pattern matching comparator.
  const bool left_null_terminated = (left.getTypeID() == TypeID::kVarChar);
  const bool right_null_terminated = (right.getTypeID() == TypeID::kVarChar);

  const std::size_t left_max_length = left.maximumByteLength() - (left_null_terminated ? 1 : 0);
  const std::size_t right_max_length = right.maximumByteLength() - (right_null_terminated ? 1 : 0);

  bool is_like_pattern;
  bool is_negation;
  const ComparisonID sub_type = getComparisonID();
  switch (sub_type) {
    case ComparisonID::kLike:
      is_like_pattern = true;
      is_negation = false;
      break;
    case ComparisonID::kNotLike:
      is_like_pattern = true;
      is_negation = true;
      break;
    case ComparisonID::kRegexMatch:
      is_like_pattern = false;
      is_negation = false;
      break;
    case ComparisonID::kNotRegexMatch:
      is_like_pattern = false;
      is_negation = true;
      break;
    default:
      LOG(FATAL) << "Unhandled ComparisonID: "
                 << kComparisonNames[static_cast<typename std::underlying_type<ComparisonID>::type>(sub_type)]
                 << " in PatternMatchinComparison::makeUncheckedComparatorForTypes()";
  }

  return meta::InvokeOnBools(
      is_like_pattern, is_negation,
      left.isNullable(), right.isNullable(),
      [&](auto is_like_pattern,  // NOLINT(build/c++11)
          auto is_negation,
          auto is_left_nullable,
          auto is_right_nullable) -> UncheckedComparator* {
    return new PatternMatchingUncheckedComparator<
        decltype(is_like_pattern)::value,
        decltype(is_negation)::value,
        decltype(is_left_nullable)::value,
        decltype(is_right_nullable)::value>(left_max_length, right_max_length);
  });
}

}  // namespace quickstep
