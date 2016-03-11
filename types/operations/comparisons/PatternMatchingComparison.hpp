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

#ifndef QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_PATTERN_MATCHING_COMPARISON_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_PATTERN_MATCHING_COMPARISON_HPP_

#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class Type;
class TypedValue;

/** \addtogroup Types
 *  @{
 */

/**
 * @brief The pattern matching Comparison.
 **/
class PatternMatchingComparison : public Comparison {
 public:
  /**
   * @brief Get a reference to the singleton instance of the specified pattern
   *        matching operation with regard to the ComparisonID.
   *
   * @param sub_type The specified type of the pattern matching, which can be
   *        kLike, kNotLike, kRegexMatch or kNotRegexMatch.
   * @return A reference to the singleton instance of the specified pattern
   *         matching operation.
   **/
  static const PatternMatchingComparison& Instance(const ComparisonID sub_type);

  bool canCompareTypes(const Type &left, const Type &right) const override;

  bool canComparePartialTypes(const Type *left_type,
                              const Type *right_type) const override;

  bool compareTypedValuesChecked(const TypedValue &left,
                                 const Type &left_type,
                                 const TypedValue &right,
                                 const Type &right_type) const override;

  UncheckedComparator* makeUncheckedComparatorForTypes(const Type &left,
                                                       const Type &right) const override;

 private:
  explicit PatternMatchingComparison(const ComparisonID sub_type)
      : Comparison(sub_type) {
  }

  DISALLOW_COPY_AND_ASSIGN(PatternMatchingComparison);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_PATTERN_MATCHING_COMPARISON_HPP_
