/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "types/operations/comparisons/LessComparison.hpp"

#include <functional>

#include "types/operations/comparisons/AsciiStringComparators.hpp"
#include "types/operations/comparisons/AsciiStringComparators-inl.hpp"
#include "types/operations/comparisons/BasicComparison.hpp"
#include "types/operations/comparisons/LiteralComparators.hpp"
#include "types/operations/comparisons/LiteralComparators-inl.hpp"

namespace quickstep {

bool LessComparison::compareTypedValuesChecked(const TypedValue &left,
                                               const Type &left_type,
                                               const TypedValue &right,
                                               const Type &right_type) const {
  return compareTypedValuesCheckedHelper<std::less>(left, left_type,
                                                    right, right_type);
}

UncheckedComparator* LessComparison::makeUncheckedComparatorForTypes(const Type &left,
                                                                     const Type &right) const {
  return makeUncheckedComparatorForTypesHelper<LessLiteralUncheckedComparator,
                                               LessAsciiStringUncheckedComparator>(left, right);
}

}  // namespace quickstep
