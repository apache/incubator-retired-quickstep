/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#include "types/operations/comparisons/GreaterComparison.hpp"

#include <functional>

#include "types/operations/comparisons/AsciiStringComparators.hpp"
#include "types/operations/comparisons/AsciiStringComparators-inl.hpp"
#include "types/operations/comparisons/BasicComparison.hpp"
#include "types/operations/comparisons/LiteralComparators.hpp"
#include "types/operations/comparisons/LiteralComparators-inl.hpp"

namespace quickstep {

bool GreaterComparison::compareTypedValuesChecked(const TypedValue &left,
                                                  const Type &left_type,
                                                  const TypedValue &right,
                                                  const Type &right_type) const {
  return compareTypedValuesCheckedHelper<std::greater>(left, left_type,
                                                       right, right_type);
}

UncheckedComparator* GreaterComparison::makeUncheckedComparatorForTypes(const Type &left,
                                                                        const Type &right) const {
  return makeUncheckedComparatorForTypesHelper<GreaterLiteralUncheckedComparator,
                                               GreaterAsciiStringUncheckedComparator>(left, right);
}

}  // namespace quickstep
