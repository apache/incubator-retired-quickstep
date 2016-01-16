/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_NOT_EQUAL_COMPARISON_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_NOT_EQUAL_COMPARISON_HPP_

#include "types/operations/comparisons/BasicComparison.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class Type;
class TypedValue;
class UncheckedComparator;

/** \addtogroup Types
 *  @{
 */

/**
 * @brief The not-equal Comparison.
 **/
class NotEqualComparison : public BasicComparison {
 public:
  /**
   * @brief Get a reference to the singleton instance of this Operation.
   *
   * @return A reference to the singleton instance of this Operation.
   **/
  static const NotEqualComparison& Instance() {
    static NotEqualComparison instance;
    return instance;
  }

  bool compareTypedValuesChecked(const TypedValue &left,
                                 const Type &left_type,
                                 const TypedValue &right,
                                 const Type &right_type) const override;

  UncheckedComparator* makeUncheckedComparatorForTypes(const Type &left,
                                                       const Type &right) const override;

 private:
  NotEqualComparison() : BasicComparison(ComparisonID::kNotEqual) {
  }

  DISALLOW_COPY_AND_ASSIGN(NotEqualComparison);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_NOT_EQUAL_COMPARISON_HPP_
