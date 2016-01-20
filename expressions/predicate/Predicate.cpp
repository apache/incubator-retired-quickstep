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
