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

#include "expressions/predicate/ConjunctionPredicate.hpp"

#include <memory>

#include "expressions/Expressions.pb.h"
#include "expressions/predicate/Predicate.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/ValueAccessor.hpp"
#include "utility/PtrList.hpp"

namespace quickstep {

serialization::Predicate ConjunctionPredicate::getProto() const {
  serialization::Predicate proto;
  proto.set_predicate_type(serialization::Predicate::CONJUNCTION);

  for (PtrList<Predicate>::const_iterator it = static_operand_list_.begin();
       it != static_operand_list_.end();
       ++it) {
    proto.AddExtension(serialization::PredicateWithList::operands)->CopyFrom(it->getProto());
  }

  for (PtrList<Predicate>::const_iterator it = dynamic_operand_list_.begin();
       it != dynamic_operand_list_.end();
       ++it) {
    proto.AddExtension(serialization::PredicateWithList::operands)->CopyFrom(it->getProto());
  }

  return proto;
}

Predicate* ConjunctionPredicate::clone() const {
  ConjunctionPredicate *clone = new ConjunctionPredicate();

  for (PtrList<Predicate>::const_iterator it = static_operand_list_.begin();
       it != static_operand_list_.end();
       ++it) {
    clone->addPredicate(it->clone());
  }

  for (PtrList<Predicate>::const_iterator it = dynamic_operand_list_.begin();
       it != dynamic_operand_list_.end();
       ++it) {
    clone->addPredicate(it->clone());
  }

  return clone;
}

bool ConjunctionPredicate::matchesForSingleTuple(const ValueAccessor &accessor,
                                                 const tuple_id tuple) const {
  if (has_static_result_) {
    return static_result_;
  } else {
    for (PtrList<Predicate>::const_iterator it = dynamic_operand_list_.begin();
         it != dynamic_operand_list_.end();
         ++it) {
      if (!it->matchesForSingleTuple(accessor, tuple)) {
        return false;
      }
    }

    return true;
  }
}

bool ConjunctionPredicate::matchesForJoinedTuples(
    const ValueAccessor &left_accessor,
    const relation_id left_relation_id,
    const tuple_id left_tuple_id,
    const ValueAccessor &right_accessor,
    const relation_id right_relation_id,
    const tuple_id right_tuple_id) const {
  if (has_static_result_) {
    return static_result_;
  } else {
    for (PtrList<Predicate>::const_iterator it = dynamic_operand_list_.begin();
         it != dynamic_operand_list_.end();
         ++it) {
      if (!it->matchesForJoinedTuples(left_accessor,
                                      left_relation_id,
                                      left_tuple_id,
                                      right_accessor,
                                      right_relation_id,
                                      right_tuple_id)) {
        return false;
      }
    }

    return true;
  }
}

TupleIdSequence* ConjunctionPredicate::getAllMatches(
    ValueAccessor *accessor,
    const SubBlocksReference *sub_blocks_ref,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_map) const {
  if (has_static_result_) {
    return GenerateSequenceForStaticResult(accessor, filter, existence_map, static_result_);
  } else {
    tuple_id end_pos = accessor->getEndPositionVirtual();

    TupleIdSequence *current_filter = new TupleIdSequence(end_pos);
    if (filter != nullptr) {
      current_filter->assignFrom(*filter);
    } else if (existence_map != nullptr) {
      current_filter->assignFrom(*existence_map);
    } else {
      current_filter->setRange(0, end_pos, true);
    }

    for (PtrList<Predicate>::const_iterator it = dynamic_operand_list_.begin();
         it != dynamic_operand_list_.end();
         ++it) {
      if (current_filter->empty()) {
        break;
      }
      std::unique_ptr<TupleIdSequence> operand_result(it->getAllMatches(accessor,
                                                                        sub_blocks_ref,
                                                                        current_filter,
                                                                        existence_map));
      current_filter->intersectWith(*operand_result);
    }

    return current_filter;
  }
}

void ConjunctionPredicate::addPredicate(Predicate *operand) {
  if (operand->getPredicateType() == kConjunction) {
    ConjunctionPredicate *conjunction_operand = static_cast<ConjunctionPredicate*>(operand);

    PtrList<Predicate>::iterator checkpos;
    if (static_operand_list_.empty()) {
      static_operand_list_.splice(static_operand_list_.end(), conjunction_operand->static_operand_list_);
      checkpos = static_operand_list_.begin();
    } else {
      checkpos = static_operand_list_.end();
      --checkpos;
      static_operand_list_.splice(static_operand_list_.end(), conjunction_operand->static_operand_list_);
      ++checkpos;
    }

    while (checkpos != static_operand_list_.end()) {
      processStaticOperand(*checkpos);
      ++checkpos;
    }

    if (!conjunction_operand->dynamic_operand_list_.empty()) {
      dynamic_operand_list_.splice(dynamic_operand_list_.end(), conjunction_operand->dynamic_operand_list_);
      processDynamicOperand();
    }

    delete conjunction_operand;
  } else {
    if (operand->hasStaticResult()) {
      static_operand_list_.push_back(operand);
      processStaticOperand(*operand);
    } else {
      dynamic_operand_list_.push_back(operand);
      processDynamicOperand();
    }
  }
}

void ConjunctionPredicate::processStaticOperand(const Predicate &operand) {
  if (!operand.getStaticResult()) {
    has_static_result_ = true;
    static_result_ = false;
  }
}

void ConjunctionPredicate::processDynamicOperand() {
  if (has_static_result_) {
    if (static_result_) {
      has_static_result_ = false;
    }
  }
}

}  // namespace quickstep
