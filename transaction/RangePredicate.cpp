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

#include "transaction/CycleDetector.hpp"

#include <cstdint>
#include <memory>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "transaction/Predicate.hpp"
#include "transaction/EqualityPredicate.hpp"
#include "transaction/RangePredicate.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/FloatType.hpp"
#include "types/operations/comparisons/EqualComparison.hpp"
#include "types/operations/comparisons/LessComparison.hpp"
#include "types/operations/comparisons/LessOrEqualComparison.hpp"
#include "types/operations/comparisons/GreaterComparison.hpp"
#include "types/operations/comparisons/GreaterOrEqualComparison.hpp"

namespace quickstep {
namespace transaction {

RangePredicate::RangePredicate(const Type* targetType,
  const TypedValue* smallValue,
  const TypedValue* largeValue,
  RangeType rangeType):
rangeType(rangeType), targetType(targetType), smallValue(smallValue), largeValue(largeValue)
{
  type = Range;

  lessThanComp = &quickstep::LessComparison::Instance();
  lessThanEqComp = &quickstep::LessOrEqualComparison::Instance();
  greaterThanComp = &quickstep::GreaterComparison::Instance();
  greaterThanEqComp = &quickstep::GreaterOrEqualComparison::Instance();
}

RangePredicate::~RangePredicate(){
}

bool RangePredicate::intersect(const Predicate& predicate) const{
  switch(predicate.type){
    case Equality: {
      const EqualityPredicate& eqPredicate = dynamic_cast<const EqualityPredicate &>(predicate);
      if(targetType->getTypeID() != eqPredicate.targetType->getTypeID()){
        return false; // TODO: throw proper exception here
      }
      if(rangeType == Inclusive){
        return greaterThanEqComp->compareTypedValuesChecked(*eqPredicate.targetValue, *eqPredicate.targetType, *smallValue, *targetType)
          && lessThanEqComp->compareTypedValuesChecked(*eqPredicate.targetValue, *eqPredicate.targetType, *largeValue, *targetType);
      }
      else if(rangeType == NonInclusive){
        return greaterThanComp->compareTypedValuesChecked(*eqPredicate.targetValue, *eqPredicate.targetType, *smallValue, *targetType)
          && lessThanComp->compareTypedValuesChecked(*eqPredicate.targetValue, *eqPredicate.targetType, *largeValue, *targetType);
      }
      else if(rangeType == LeftInclusive){
        return greaterThanEqComp->compareTypedValuesChecked(*eqPredicate.targetValue, *eqPredicate.targetType, *smallValue, *targetType)
          && lessThanComp->compareTypedValuesChecked(*eqPredicate.targetValue, *eqPredicate.targetType, *largeValue, *targetType);
      }
      else {  // right inclusive
        return greaterThanComp->compareTypedValuesChecked(*eqPredicate.targetValue, *eqPredicate.targetType, *smallValue, *targetType)
          && lessThanEqComp->compareTypedValuesChecked(*eqPredicate.targetValue, *eqPredicate.targetType, *largeValue, *targetType);
      }
    }
    case Range: {
      return false;
    }
    default: {
      return false;
    }
  }
}

}  // namespace transaction
}  // namespace quickstep
