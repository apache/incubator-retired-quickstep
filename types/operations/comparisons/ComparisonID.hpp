/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_COMPARISON_ID_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_COMPARISON_ID_HPP_

#include <type_traits>

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

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_COMPARISON_ID_HPP_
