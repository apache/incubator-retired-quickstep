/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "expressions/predicate/DisjunctionPredicate.hpp"

#include <memory>

#include "expressions/Expressions.pb.h"
#include "expressions/predicate/Predicate.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/ValueAccessor.hpp"
#include "utility/PtrList.hpp"

namespace quickstep {

serialization::Predicate DisjunctionPredicate::getProto() const {
  serialization::Predicate proto;
  proto.set_predicate_type(serialization::Predicate::DISJUNCTION);

  for (PtrList<Predicate>::const_iterator it(static_operand_list_.begin());
       it != static_operand_list_.end();
       ++it) {
    proto.AddExtension(serialization::PredicateWithList::operands)->CopyFrom(it->getProto());
  }

  for (PtrList<Predicate>::const_iterator it(dynamic_operand_list_.begin());
       it != dynamic_operand_list_.end();
       ++it) {
    proto.AddExtension(serialization::PredicateWithList::operands)->CopyFrom(it->getProto());
  }

  return proto;
}

Predicate* DisjunctionPredicate::clone() const {
  DisjunctionPredicate *clone = new DisjunctionPredicate();

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

bool DisjunctionPredicate::matchesForSingleTuple(const ValueAccessor &accessor,
                                                 const tuple_id tuple) const {
  if (has_static_result_) {
    return static_result_;
  } else {
    for (PtrList<Predicate>::const_iterator it = dynamic_operand_list_.begin();
         it != dynamic_operand_list_.end();
         ++it) {
      if (it->matchesForSingleTuple(accessor, tuple)) {
        return true;
      }
    }

    return false;
  }
}

bool DisjunctionPredicate::matchesForJoinedTuples(
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
      if (it->matchesForJoinedTuples(left_accessor,
                                     left_relation_id,
                                     left_tuple_id,
                                     right_accessor,
                                     right_relation_id,
                                     right_tuple_id)) {
        return true;
      }
    }

    return false;
  }
}

TupleIdSequence* DisjunctionPredicate::getAllMatches(
    ValueAccessor *accessor,
    const SubBlocksReference *sub_blocks_ref,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_map) const {
  if (has_static_result_) {
    return GenerateSequenceForStaticResult(accessor, filter, existence_map, static_result_);
  } else {
    tuple_id end_pos = accessor->getEndPositionVirtual();
    TupleIdSequence *union_result = new TupleIdSequence(end_pos);

#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
    std::unique_ptr<TupleIdSequence> current_filter(new TupleIdSequence(end_pos));
    if (filter != nullptr) {
      current_filter->assignFrom(*filter);
    } else if (existence_map != nullptr) {
      current_filter->assignFrom(*existence_map);
    } else {
      current_filter->setRange(0, end_pos, true);
    }
#endif  // QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT

    for (PtrList<Predicate>::const_iterator it = dynamic_operand_list_.begin();
         it != dynamic_operand_list_.end();
         ++it) {
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
      std::unique_ptr<TupleIdSequence> operand_result(it->getAllMatches(accessor,
                                                                        sub_blocks_ref,
                                                                        current_filter.get(),
                                                                        existence_map));
#else
      std::unique_ptr<TupleIdSequence> operand_result(it->getAllMatches(accessor,
                                                                        sub_blocks_ref,
                                                                        filter,
                                                                        existence_map));
#endif  // QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
      union_result->unionWith(*operand_result);

#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
      // Don't bother checking tuples which are already known to match some
      // part of the union.
      operand_result->invert();
      current_filter->intersectWith(*operand_result);
#endif  // QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
    }

    return union_result;
  }
}

void DisjunctionPredicate::addPredicate(Predicate *operand) {
  if (operand->getPredicateType() == kDisjunction) {
    DisjunctionPredicate *disjunction_operand = static_cast<DisjunctionPredicate*>(operand);

    PtrList<Predicate>::iterator checkpos;
    if (static_operand_list_.empty()) {
      static_operand_list_.splice(static_operand_list_.end(), disjunction_operand->static_operand_list_);
      checkpos = static_operand_list_.begin();
    } else {
      checkpos = static_operand_list_.end();
      --checkpos;
      static_operand_list_.splice(static_operand_list_.end(), disjunction_operand->static_operand_list_);
      ++checkpos;
    }

    while (checkpos != static_operand_list_.end()) {
      processStaticOperand(*checkpos);
      ++checkpos;
    }

    if (!disjunction_operand->dynamic_operand_list_.empty()) {
      dynamic_operand_list_.splice(dynamic_operand_list_.end(), disjunction_operand->dynamic_operand_list_);
      processDynamicOperand();
    }

    delete disjunction_operand;
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

void DisjunctionPredicate::processStaticOperand(const Predicate &operand) {
  if (operand.getStaticResult()) {
    has_static_result_ = true;
    static_result_ = true;
  } else if (fresh_) {
    has_static_result_ = true;
    static_result_ = false;
  }

  fresh_ = false;
}

void DisjunctionPredicate::processDynamicOperand() {
  if (has_static_result_ && !static_result_) {
    has_static_result_ = false;
  } else if (fresh_) {
    has_static_result_ = false;
  }

  fresh_ = false;
}

}  // namespace quickstep
