/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "expressions/predicate/NegationPredicate.hpp"

#include "expressions/Expressions.pb.h"
#include "expressions/predicate/Predicate.hpp"
#include "storage/TupleIdSequence.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class ValueAccessor;

serialization::Predicate NegationPredicate::getProto() const {
  serialization::Predicate proto;
  proto.set_predicate_type(serialization::Predicate::NEGATION);
  proto.MutableExtension(serialization::NegationPredicate::operand)->CopyFrom(operand_->getProto());

  return proto;
}

Predicate* NegationPredicate::clone() const {
  return new NegationPredicate(operand_->clone());
}

bool NegationPredicate::matchesForSingleTuple(const ValueAccessor &accessor,
                                              const tuple_id tuple) const {
  if (has_static_result_) {
    return static_result_;
  } else {
    return !(operand_->matchesForSingleTuple(accessor, tuple));
  }
}

bool NegationPredicate::matchesForJoinedTuples(
    const ValueAccessor &left_accessor,
    const relation_id left_relation_id,
    const tuple_id left_tuple_id,
    const ValueAccessor &right_accessor,
    const relation_id right_relation_id,
    const tuple_id right_tuple_id) const {
  if (has_static_result_) {
    return static_result_;
  } else {
    return !(operand_->matchesForJoinedTuples(left_accessor,
                                              left_relation_id,
                                              left_tuple_id,
                                              right_accessor,
                                              right_relation_id,
                                              right_tuple_id));
  }
}

TupleIdSequence* NegationPredicate::getAllMatches(
    ValueAccessor *accessor,
    const SubBlocksReference *sub_blocks_ref,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_map) const {
  if (has_static_result_) {
    return GenerateSequenceForStaticResult(accessor, filter, existence_map, static_result_);
  } else {
    TupleIdSequence *operand_matches = operand_->getAllMatches(accessor,
                                                               sub_blocks_ref,
                                                               filter,
                                                               existence_map);

    operand_matches->invert();
    if (filter != nullptr) {
      operand_matches->intersectWith(*filter);
    }
    if (existence_map != nullptr) {
      operand_matches->intersectWith(*existence_map);
    }
    return operand_matches;
  }
}

bool NegationPredicate::getStaticResult() const {
  if (has_static_result_) {
    return static_result_;
  } else {
    FATAL_ERROR("Called getStaticResult() on a predicate which has no static result");
  }
}

void NegationPredicate::initHelper() {
  if (operand_->hasStaticResult()) {
    has_static_result_ = true;
    static_result_ = !operand_->getStaticResult();
  } else {
    has_static_result_ = false;
  }
}

Predicate* NegationPredicate::NegatePredicate(Predicate *operand) {
  if (operand->getPredicateType() == kNegation) {
    NegationPredicate *negation_operand = static_cast<NegationPredicate*>(operand);
    Predicate *inner = negation_operand->operand_.release();
    delete negation_operand;
    return inner;
  } else {
    return new NegationPredicate(operand);
  }
}

}  // namespace quickstep
