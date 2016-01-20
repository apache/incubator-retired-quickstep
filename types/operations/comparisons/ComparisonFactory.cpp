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

#include "types/operations/comparisons/ComparisonFactory.hpp"

#include <string>

#include "types/operations/Operation.pb.h"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "types/operations/comparisons/EqualComparison.hpp"
#include "types/operations/comparisons/GreaterComparison.hpp"
#include "types/operations/comparisons/GreaterOrEqualComparison.hpp"
#include "types/operations/comparisons/LessComparison.hpp"
#include "types/operations/comparisons/LessOrEqualComparison.hpp"
#include "types/operations/comparisons/NotEqualComparison.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

const Comparison& ComparisonFactory::GetComparison(const ComparisonID id) {
  switch (id) {
    case ComparisonID::kEqual:
      return EqualComparison::Instance();
    case ComparisonID::kNotEqual:
      return NotEqualComparison::Instance();
    case ComparisonID::kLess:
      return LessComparison::Instance();
    case ComparisonID::kLessOrEqual:
      return LessOrEqualComparison::Instance();
    case ComparisonID::kGreater:
      return GreaterComparison::Instance();
    case ComparisonID::kGreaterOrEqual:
      return GreaterOrEqualComparison::Instance();
    default:
      break;  // Prevent compiler from complaining about unhandled case.
  }
  FATAL_ERROR("Unknown ComparisonID");
}

bool ComparisonFactory::ProtoIsValid(const serialization::Comparison &proto) {
  // Check that Comparison is fully initialized.
  if (!proto.IsInitialized()) {
    return false;
  }

  // Check that the comparison_id is a valid Comparison.
  if (!proto.ComparisonID_IsValid(proto.comparison_id())) {
    return false;
  }

  return true;
}

const Comparison& ComparisonFactory::ReconstructFromProto(const serialization::Comparison &proto) {
  DCHECK(ProtoIsValid(proto))
      << "Attempted to create Comparison from an invalid proto description:\n"
      << proto.DebugString();

  switch (proto.comparison_id()) {
    case serialization::Comparison::EQUAL:
      return GetComparison(ComparisonID::kEqual);
    case serialization::Comparison::NOT_EQUAL:
      return GetComparison(ComparisonID::kNotEqual);
    case serialization::Comparison::LESS:
      return GetComparison(ComparisonID::kLess);
    case serialization::Comparison::LESS_OR_EQUAL:
      return GetComparison(ComparisonID::kLessOrEqual);
    case serialization::Comparison::GREATER:
      return GetComparison(ComparisonID::kGreater);
    case serialization::Comparison::GREATER_OR_EQUAL:
      return GetComparison(ComparisonID::kGreaterOrEqual);
    default:
      FATAL_ERROR("Unrecognized ComparisonID in ComparisonFactory::ReconstructFromProto");
  }
}

}  // namespace quickstep
