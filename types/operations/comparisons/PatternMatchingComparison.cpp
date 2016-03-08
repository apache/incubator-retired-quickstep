/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *   University of Wisconsin—Madison.
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

#include "types/operations/comparisons/PatternMatchingComparison.hpp"

#include <memory>
#include <utility>

#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "types/operations/comparisons/PatternMatchingComparators.hpp"
#include "types/operations/comparisons/PatternMatchingComparators-inl.hpp"
#include "utility/TemplateUtil.hpp"

#include "glog/logging.h"

namespace quickstep {

const PatternMatchingComparison& PatternMatchingComparison::Instance(ComparisonID sub_type) {
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
}

bool PatternMatchingComparison::canCompareTypes(const Type &left,
                                                const Type &right) const {
  if ((left.getTypeID() == TypeID::kChar || left.getTypeID() == TypeID::kVarChar)
      && (right.getTypeID() == TypeID::kChar || right.getTypeID() == TypeID::kVarChar)) {
    return true;
  }
  return false;
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

  bool is_like_pattern;
  bool is_negation;
  ComparisonID sub_type = getComparisonID();
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
    default:
      LOG(FATAL) << "Unhandled ComparisonID: "
                 << kComparisonNames[static_cast<typename std::underlying_type<ComparisonID>::type>(sub_type)]
                 << " in PatternMatchinComparison::makeUncheckedComparatorForTypes()";
  }

  bool left_null_terminated = (left.getTypeID() == TypeID::kVarChar);
  bool right_null_terminated = (right.getTypeID() == TypeID::kVarChar);

  size_t left_max_length = left.maximumByteLength() - (left_null_terminated ? 1 : 0);
  size_t right_max_length = right.maximumByteLength() - (right_null_terminated ? 1 : 0);

  return CreateBoolInstantiatedInstance<PatternMatchingUncheckedComparator, UncheckedComparator>(
      std::forward_as_tuple(left_max_length, right_max_length),
      is_like_pattern, is_negation,
      left.isNullable(), right.isNullable());
}


}  // namespace quickstep
