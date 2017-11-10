/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#ifndef QUICKSTEP_TRANSACTION_RANGE_PREDICATE_HPP_
#define QUICKSTEP_TRANSACTION_RANGE_PREDICATE_HPP_

#include <memory>
#include <unordered_set>
#include <vector>
#include "transaction/Predicate.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/EqualComparison.hpp"
#include "types/operations/comparisons/LessComparison.hpp"
#include "types/operations/comparisons/LessOrEqualComparison.hpp"
#include "types/operations/comparisons/GreaterComparison.hpp"
#include "types/operations/comparisons/GreaterOrEqualComparison.hpp"

namespace quickstep {
namespace transaction {

class RangePredicate : public Predicate
{
private:

  const LessComparison* lessThanComp;
  const LessOrEqualComparison* lessThanEqComp;
  const GreaterComparison* greaterThanComp;
  const GreaterOrEqualComparison* greaterThanEqComp;


public:
  enum RangeType {
    Inclusive,
    NonInclusive,
    LeftInclusive,
    RightInclusive
  };
  const RangeType rangeType;
  const Type *targetType;
  const TypedValue *smallValue;
  const TypedValue *largeValue;
  RangePredicate(const Type* targetType, const TypedValue* smallValue, const TypedValue* largeValue, RangeType rangeType);
  ~RangePredicate();
  bool intersect(const Predicate& predicate) const override;
};

}
}

#endif
