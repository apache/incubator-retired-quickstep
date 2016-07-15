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

#include "types/operations/comparisons/BasicComparison.hpp"

#include <cstddef>
#include <cstring>

#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "utility/EqualsAnyConstant.hpp"

#include "glog/logging.h"

using std::size_t;
using std::strcmp;
using std::strncmp;

namespace quickstep {

bool BasicComparison::canCompareTypes(const Type &left, const Type &right) const {
  switch (left.getTypeID()) {
    case kInt:
    case kLong:
    case kFloat:
    case kDouble: {
      switch (right.getTypeID()) {
        case kInt:
        case kLong:
        case kFloat:
        case kDouble:
          return true;
        default:
          return false;
      }
    }
    case kDate: {
      return right.getTypeID() == kDate;
    }
    case kDatetime: {
      return right.getTypeID() == kDatetime;
    }
    case kDatetimeInterval: {
      return right.getTypeID() == kDatetimeInterval;
    }
    case kYearMonthInterval: {
      return right.getTypeID() == kYearMonthInterval;
    }
    case kChar:
    case kVarChar: {
      switch (right.getTypeID()) {
        case kChar:
        case kVarChar:
          return true;
        default:
          return false;
      }
    }
    default:
      return false;
  }
}

bool BasicComparison::canComparePartialTypes(const Type *left_type,
                                             const Type *right_type) const {
  if ((left_type == nullptr) && (right_type == nullptr)) {
    return true;
  }

  if ((left_type != nullptr) && (right_type != nullptr)) {
    return canCompareTypes(*left_type, *right_type);
  }

  // The basic comparisons are all symmetric with respect to the types that
  // they may compare, so we determine comparability based on which type is
  // known, we don't particularly care if it is left or right.
  const Type *known_type = (left_type != nullptr) ? left_type : right_type;
  return QUICKSTEP_EQUALS_ANY_CONSTANT(known_type->getTypeID(),
                                       kInt, kLong, kFloat, kDouble,
                                       kDate, kDatetime, kDatetimeInterval, kYearMonthInterval,
                                       kChar, kVarChar);
}

int BasicComparison::strcmpHelper(const TypedValue &left, const TypedValue &right) const {
  DCHECK((left.getTypeID() == kChar) || (left.getTypeID() == kVarChar));
  DCHECK((right.getTypeID() == kChar) || (right.getTypeID() == kVarChar));
  if ((left.getTypeID() == kVarChar) && (right.getTypeID() == kVarChar)) {
    return strcmp(static_cast<const char*>(left.getOutOfLineData()),
                  static_cast<const char*>(right.getOutOfLineData()));
  } else {
    size_t left_length = left.getAsciiStringLength();
    size_t right_length = right.getAsciiStringLength();

    if (left_length < right_length) {
      int res = strncmp(static_cast<const char*>(left.getOutOfLineData()),
                        static_cast<const char*>(right.getOutOfLineData()),
                        left_length);
      if (res) {
        return res;
      } else {
        return -1;
      }
    } else if (left_length > right_length) {
      int res = strncmp(static_cast<const char*>(left.getOutOfLineData()),
                        static_cast<const char*>(right.getOutOfLineData()),
                        right_length);
      if (res) {
        return res;
      } else {
        return 1;
      }
    } else {
      return strncmp(static_cast<const char*>(left.getOutOfLineData()),
                     static_cast<const char*>(right.getOutOfLineData()),
                     left_length);
    }
  }
}

}  // namespace quickstep
