/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "types/operations/comparisons/ComparisonID.hpp"

namespace quickstep {

const char *kComparisonNames[] = {
  "Equal",
  "NotEqual",
  "Less",
  "LessOrEqual",
  "Greater",
  "GreaterOrEqual"
};

const char *kComparisonShortNames[] = {
  "=",
  "!=",
  "<",
  "<=",
  ">",
  ">="
};

}  // namespace quickstep
