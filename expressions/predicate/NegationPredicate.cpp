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

#include "expressions/predicate/NegationPredicate.hpp"

#include <string>
#include <vector>

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
    const tuple_id left_tuple_id,
    const ValueAccessor &right_accessor,
    const tuple_id right_tuple_id) const {
  if (has_static_result_) {
    return static_result_;
  } else {
    return !(operand_->matchesForJoinedTuples(left_accessor,
                                              left_tuple_id,
                                              right_accessor,
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

void NegationPredicate::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const Expression*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const Expression*>> *container_child_fields) const {
  Predicate::getFieldStringItems(inline_field_names,
                                 inline_field_values,
                                 non_container_child_field_names,
                                 non_container_child_fields,
                                 container_child_field_names,
                                 container_child_fields);

  non_container_child_field_names->emplace_back("operand");
  non_container_child_fields->emplace_back(operand_.get());
}

}  // namespace quickstep
