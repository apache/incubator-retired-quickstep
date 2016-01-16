/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "expressions/predicate/Predicate.hpp"

#include "storage/TupleIdSequence.hpp"
#include "storage/ValueAccessor.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

const char *Predicate::kPredicateTypeNames[] = {
  "True",
  "False",
  "Comparison",
  "Negation",
  "Conjunction",
  "Disjunction"
};

bool Predicate::getStaticResult() const {
  FATAL_ERROR("Called getStaticResult() on a predicate which has no static result");
}

TupleIdSequence* Predicate::GenerateSequenceForStaticResult(
    ValueAccessor *accessor,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_map,
    const bool static_result) {
  TupleIdSequence *result = new TupleIdSequence(accessor->getEndPositionVirtual());
  if (static_result) {
    if (filter != nullptr) {
      // '*filter' will always be a subset of '*existence_map'.
      result->assignFrom(*filter);
    } else if (existence_map != nullptr) {
      result->assignFrom(*existence_map);
    } else {
      result->setRange(0, result->length(), true);
    }
  }
  return result;
}

}  // namespace quickstep
