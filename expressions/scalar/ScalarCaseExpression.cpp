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

#include "expressions/scalar/ScalarCaseExpression.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/Type.hpp"
#include "types/Type.pb.h"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"

#include "glog/logging.h"

namespace quickstep {

ScalarCaseExpression::ScalarCaseExpression(
    const Type &result_type,
    std::vector<std::unique_ptr<Predicate>> &&when_predicates,
    std::vector<std::unique_ptr<Scalar>> &&result_expressions,
    Scalar *else_result_expression)
    : Scalar(result_type),
      when_predicates_(std::move(when_predicates)),
      result_expressions_(std::move(result_expressions)),
      else_result_expression_(else_result_expression),
      fixed_result_expression_(nullptr),
      has_static_value_(false) {
  DCHECK_EQ(when_predicates_.size(), result_expressions_.size());
  DCHECK(else_result_expression_ != nullptr);

#ifdef QUICKSTEP_DEBUG
  for (const std::unique_ptr<Scalar> &result_expr : result_expressions_) {
    DCHECK(result_expr->getType().isSubsumedBy(type_));
  }
#endif  // QUICKSTEP_DEBUG

  DCHECK(else_result_expression_->getType().isSubsumedBy(type_));

  // Resolve case branch statically if possible.
  bool static_case_branch = true;
  for (std::vector<std::unique_ptr<Predicate>>::size_type case_idx = 0;
       case_idx < when_predicates_.size();
       ++case_idx) {
    if (when_predicates_[case_idx]->hasStaticResult()) {
      if (when_predicates_[case_idx]->getStaticResult()) {
        fixed_result_expression_ = result_expressions_[case_idx].get();
        break;
      }
    } else {
      static_case_branch = false;
    }
  }

  if (static_case_branch && (fixed_result_expression_ == nullptr)) {
    fixed_result_expression_ = else_result_expression_.get();
  }

  if (fixed_result_expression_ != nullptr) {
    if (fixed_result_expression_->hasStaticValue()) {
      has_static_value_ = true;
      static_value_ = fixed_result_expression_->getStaticValue();
    } else {
      // CASE always goes to the same branch, but its value is not static.
    }
  }
}

serialization::Scalar ScalarCaseExpression::getProto() const {
  serialization::Scalar proto;
  proto.set_data_source(serialization::Scalar::CASE_EXPRESSION);
  proto.MutableExtension(serialization::ScalarCaseExpression::result_type)
      ->CopyFrom(type_.getProto());
  for (const std::unique_ptr<Predicate> &when_pred : when_predicates_) {
    proto.AddExtension(serialization::ScalarCaseExpression::when_predicate)
      ->CopyFrom(when_pred->getProto());
  }
  for (const std::unique_ptr<Scalar> &result_expr : result_expressions_) {
    proto.AddExtension(serialization::ScalarCaseExpression::result_expression)
      ->CopyFrom(result_expr->getProto());
  }
  proto.MutableExtension(serialization::ScalarCaseExpression::else_result_expression)
      ->CopyFrom(else_result_expression_->getProto());

  return proto;
}

Scalar* ScalarCaseExpression::clone() const {
  std::vector<std::unique_ptr<Predicate>> when_predicate_clones;
  when_predicate_clones.reserve(when_predicates_.size());
  for (const std::unique_ptr<Predicate> &when_pred : when_predicates_) {
    when_predicate_clones.emplace_back(when_pred->clone());
  }

  std::vector<std::unique_ptr<Scalar>> result_expression_clones;
  result_expression_clones.reserve(result_expressions_.size());
  for (const std::unique_ptr<Scalar> &result_expr : result_expressions_) {
    result_expression_clones.emplace_back(result_expr->clone());
  }

  return new ScalarCaseExpression(type_,
                                  std::move(when_predicate_clones),
                                  std::move(result_expression_clones),
                                  else_result_expression_->clone());
}

TypedValue ScalarCaseExpression::getValueForSingleTuple(
    const ValueAccessor &accessor,
    const tuple_id tuple) const {
  if (has_static_value_) {
    return static_value_.makeReferenceToThis();
  } else if (fixed_result_expression_ != nullptr) {
    return fixed_result_expression_->getValueForSingleTuple(accessor, tuple);
  } else {
    for (std::vector<std::unique_ptr<Predicate>>::size_type case_idx = 0;
         case_idx < when_predicates_.size();
         ++case_idx) {
      if (when_predicates_[case_idx]->matchesForSingleTuple(accessor, tuple)) {
        return result_expressions_[case_idx]->getValueForSingleTuple(accessor, tuple);
      }
    }
    return else_result_expression_->getValueForSingleTuple(accessor, tuple);
  }
}

TypedValue ScalarCaseExpression::getValueForJoinedTuples(
    const ValueAccessor &left_accessor,
    const relation_id left_relation_id,
    const tuple_id left_tuple_id,
    const ValueAccessor &right_accessor,
    const relation_id right_relation_id,
    const tuple_id right_tuple_id) const {
  if (has_static_value_) {
    return static_value_.makeReferenceToThis();
  } else if (fixed_result_expression_ != nullptr) {
    return fixed_result_expression_->getValueForJoinedTuples(left_accessor,
                                                             left_relation_id,
                                                             left_tuple_id,
                                                             right_accessor,
                                                             right_relation_id,
                                                             right_tuple_id);
  } else {
    for (std::vector<std::unique_ptr<Predicate>>::size_type case_idx = 0;
         case_idx < when_predicates_.size();
         ++case_idx) {
      if (when_predicates_[case_idx]->matchesForJoinedTuples(left_accessor,
                                                             left_relation_id,
                                                             left_tuple_id,
                                                             right_accessor,
                                                             right_relation_id,
                                                             right_tuple_id)) {
        return result_expressions_[case_idx]->getValueForJoinedTuples(
            left_accessor,
            left_relation_id,
            left_tuple_id,
            right_accessor,
            right_relation_id,
            right_tuple_id);
      }
    }
    return else_result_expression_->getValueForJoinedTuples(
        left_accessor,
        left_relation_id,
        left_tuple_id,
        right_accessor,
        right_relation_id,
        right_tuple_id);
  }
}

ColumnVectorPtr ScalarCaseExpression::getAllValues(
    ValueAccessor *accessor,
    const SubBlocksReference *sub_blocks_ref,
    ColumnVectorCache *cv_cache) const {
  return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
      accessor,
      [&](auto *accessor) -> ColumnVectorPtr {  // NOLINT(build/c++11)
    if (has_static_value_) {
      return ColumnVectorPtr(
          ColumnVector::MakeVectorOfValue(type_,
                                          static_value_,
                                          accessor->getNumTuples()));
    } else if (fixed_result_expression_ != nullptr) {
      return fixed_result_expression_->getAllValues(
          accessor, sub_blocks_ref, cv_cache);
    }

    const TupleIdSequence *accessor_sequence = accessor->getTupleIdSequence();

    // Initially set '*else_matches' to cover all tuples from the
    // ValueAccessor.
    std::unique_ptr<TupleIdSequence> else_matches;
    if (accessor_sequence != nullptr) {
      else_matches.reset(new TupleIdSequence(accessor_sequence->length()));
      else_matches->assignFrom(*accessor_sequence);
    } else {
      else_matches.reset(new TupleIdSequence(accessor->getEndPosition()));
      else_matches->setRange(0, else_matches->length(), true);
    }

    // Generate a TupleIdSequence of matches for each branch in the CASE.
    std::vector<std::unique_ptr<TupleIdSequence>> case_matches;
    for (std::vector<std::unique_ptr<Predicate>>::size_type case_idx = 0;
         case_idx < when_predicates_.size();
         ++case_idx) {
      if (else_matches->empty()) {
        break;
      }

      case_matches.emplace_back(when_predicates_[case_idx]->getAllMatches(
          accessor,
          sub_blocks_ref,
          else_matches.get(),
          accessor_sequence));
      else_matches->intersectWithComplement(*case_matches.back());
    }

    // Generate a ColumnVector of all the values for each case.
    std::vector<ColumnVectorPtr> case_results;
    for (std::vector<std::unique_ptr<TupleIdSequence>>::size_type case_idx = 0;
         case_idx < case_matches.size();
         ++case_idx) {
      std::unique_ptr<ValueAccessor> case_accessor(
          accessor->createSharedTupleIdSequenceAdapter(*case_matches[case_idx]));
      case_results.emplace_back(
          result_expressions_[case_idx]->getAllValues(
              case_accessor.get(), sub_blocks_ref, cv_cache));
    }

    ColumnVectorPtr else_results;
    if (!else_matches->empty()) {
      std::unique_ptr<ValueAccessor> else_accessor(
          accessor->createSharedTupleIdSequenceAdapter(*else_matches));
      else_results = else_result_expression_->getAllValues(
          else_accessor.get(), sub_blocks_ref, cv_cache);
    }

    // Multiplex per-case results into a single ColumnVector with values in the
    // correct positions.
    return this->multiplexColumnVectors(
        accessor->getNumTuples(),
        accessor_sequence,
        case_matches,
        *else_matches,
        case_results,
        else_results);
  });
}

ColumnVectorPtr ScalarCaseExpression::getAllValuesForJoin(
    const relation_id left_relation_id,
    ValueAccessor *left_accessor,
    const relation_id right_relation_id,
    ValueAccessor *right_accessor,
    const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids,
    ColumnVectorCache *cv_cache) const {
  // Slice 'joined_tuple_ids' apart by case.
  //
  // NOTE(chasseur): We use TupleIdSequence to keep track of the positions in
  // 'joined_tuple_ids' that match for a particular case. This is a bit hacky,
  // since TupleIdSequence is intended to represent tuple IDs in one block.
  // All we're really using it for is to multiplex results into the right
  // place.
  //
  // TODO(chasseur): Currently case predicates are evaluated tuple-at-a-time
  // here (just like in a NestedLoopsJoin). If and when we implement vectorized
  // evaluation of nested-loops predicates (or just filtration of joined IDs),
  // we should use that here.
  TupleIdSequence else_positions(joined_tuple_ids.size());
  else_positions.setRange(0, joined_tuple_ids.size(), true);

  std::vector<std::unique_ptr<TupleIdSequence>> case_positions;
  std::vector<std::vector<std::pair<tuple_id, tuple_id>>> case_matches;
  for (std::vector<std::unique_ptr<Predicate>>::size_type case_idx = 0;
       case_idx < when_predicates_.size();
       ++case_idx) {
    if (else_positions.empty()) {
      break;
    }

    TupleIdSequence *current_case_positions = new TupleIdSequence(joined_tuple_ids.size());
    case_positions.emplace_back(current_case_positions);

    case_matches.resize(case_matches.size() + 1);
    std::vector<std::pair<tuple_id, tuple_id>> &current_case_matches = case_matches.back();

    const Predicate &case_predicate = *when_predicates_[case_idx];
    for (tuple_id pos : else_positions) {
      const std::pair<tuple_id, tuple_id> check_pair = joined_tuple_ids[pos];
      if (case_predicate.matchesForJoinedTuples(*left_accessor,
                                                left_relation_id,
                                                check_pair.first,
                                                *right_accessor,
                                                right_relation_id,
                                                check_pair.second)) {
        current_case_positions->set(pos);
        current_case_matches.emplace_back(check_pair);
      }
    }

    else_positions.intersectWithComplement(*current_case_positions);
  }

  // Generate a ColumnVector of all the values for each case.
  std::vector<ColumnVectorPtr> case_results;
  for (std::vector<std::vector<std::pair<tuple_id, tuple_id>>>::size_type case_idx = 0;
       case_idx < case_matches.size();
       ++case_idx) {
    case_results.emplace_back(result_expressions_[case_idx]->getAllValuesForJoin(
        left_relation_id,
        left_accessor,
        right_relation_id,
        right_accessor,
        case_matches[case_idx],
        cv_cache));
  }

  ColumnVectorPtr else_results;
  if (!else_positions.empty()) {
    std::vector<std::pair<tuple_id, tuple_id>> else_matches;
    for (tuple_id pos : else_positions) {
      else_matches.emplace_back(joined_tuple_ids[pos]);
    }

    else_results = else_result_expression_->getAllValuesForJoin(
        left_relation_id,
        left_accessor,
        right_relation_id,
        right_accessor,
        else_matches,
        cv_cache);
  }

  // Multiplex per-case results into a single ColumnVector with values in the
  // correct positions.
  return multiplexColumnVectors(
      joined_tuple_ids.size(),
      nullptr,
      case_positions,
      else_positions,
      case_results,
      else_results);
}

void ScalarCaseExpression::MultiplexNativeColumnVector(
    const TupleIdSequence *source_sequence,
    const TupleIdSequence &case_matches,
    const NativeColumnVector &case_result,
    NativeColumnVector *output) {
  if (source_sequence == nullptr) {
    if (case_result.typeIsNullable()) {
      TupleIdSequence::const_iterator output_pos_it = case_matches.begin();
      for (std::size_t input_pos = 0;
           input_pos < case_result.size();
           ++input_pos, ++output_pos_it) {
        const void *value = case_result.getUntypedValue<true>(input_pos);
        if (value == nullptr) {
          output->positionalWriteNullValue(*output_pos_it);
        } else {
          output->positionalWriteUntypedValue(*output_pos_it, value);
        }
      }
    } else {
      TupleIdSequence::const_iterator output_pos_it = case_matches.begin();
      for (std::size_t input_pos = 0;
           input_pos < case_result.size();
           ++input_pos, ++output_pos_it) {
        output->positionalWriteUntypedValue(*output_pos_it,
                                            case_result.getUntypedValue<false>(input_pos));
      }
    }
  } else {
    if (case_result.typeIsNullable()) {
      std::size_t input_pos = 0;
      TupleIdSequence::const_iterator source_sequence_it = source_sequence->begin();
      for (std::size_t output_pos = 0;
           output_pos < output->size();
           ++output_pos, ++source_sequence_it) {
        if (case_matches.get(*source_sequence_it)) {
          const void *value = case_result.getUntypedValue<true>(input_pos++);
          if (value == nullptr) {
            output->positionalWriteNullValue(output_pos);
          } else {
            output->positionalWriteUntypedValue(output_pos, value);
          }
        }
      }
    } else {
      std::size_t input_pos = 0;
      TupleIdSequence::const_iterator source_sequence_it = source_sequence->begin();
      for (std::size_t output_pos = 0;
           output_pos < output->size();
           ++output_pos, ++source_sequence_it) {
        if (case_matches.get(*source_sequence_it)) {
          output->positionalWriteUntypedValue(output_pos,
                                              case_result.getUntypedValue<false>(input_pos++));
        }
      }
    }
  }
}

void ScalarCaseExpression::MultiplexIndirectColumnVector(
    const TupleIdSequence *source_sequence,
    const TupleIdSequence &case_matches,
    const IndirectColumnVector &case_result,
    IndirectColumnVector *output) {
  if (source_sequence == nullptr) {
    TupleIdSequence::const_iterator output_pos_it = case_matches.begin();
    for (std::size_t input_pos = 0;
         input_pos < case_result.size();
         ++input_pos, ++output_pos_it) {
      output->positionalWriteTypedValue(*output_pos_it,
                                        case_result.getTypedValue(input_pos));
    }
  } else {
    std::size_t input_pos = 0;
    TupleIdSequence::const_iterator source_sequence_it = source_sequence->begin();
    for (std::size_t output_pos = 0;
         output_pos < output->size();
         ++output_pos, ++source_sequence_it) {
      if (case_matches.get(*source_sequence_it)) {
        output->positionalWriteTypedValue(output_pos,
                                          case_result.getTypedValue(input_pos++));
      }
    }
  }
}

ColumnVectorPtr ScalarCaseExpression::multiplexColumnVectors(
    const std::size_t output_size,
    const TupleIdSequence *source_sequence,
    const std::vector<std::unique_ptr<TupleIdSequence>> &case_matches,
    const TupleIdSequence &else_matches,
    const std::vector<ColumnVectorPtr> &case_results,
    const ColumnVectorPtr &else_result) const {
  DCHECK_EQ(case_matches.size(), case_results.size());

  if (NativeColumnVector::UsableForType(type_)) {
    std::unique_ptr<NativeColumnVector> native_result(
        new NativeColumnVector(type_, output_size));
    native_result->prepareForPositionalWrites();

    for (std::vector<std::unique_ptr<TupleIdSequence>>::size_type case_idx = 0;
         case_idx < case_matches.size();
         ++case_idx) {
      DCHECK(case_results[case_idx]->isNative());
      if (!case_matches[case_idx]->empty()) {
        MultiplexNativeColumnVector(
            source_sequence,
            *case_matches[case_idx],
            static_cast<const NativeColumnVector&>(*case_results[case_idx]),
            native_result.get());
      }
    }

    if (else_result != nullptr) {
      DCHECK(else_result->isNative());
      DCHECK(!else_matches.empty());
      MultiplexNativeColumnVector(source_sequence,
                                  else_matches,
                                  static_cast<const NativeColumnVector&>(*else_result),
                                  native_result.get());
    }

    return ColumnVectorPtr(native_result.release());
  } else {
    std::unique_ptr<IndirectColumnVector> indirect_result(
        new IndirectColumnVector(type_, output_size));
    indirect_result->prepareForPositionalWrites();

    for (std::vector<std::unique_ptr<TupleIdSequence>>::size_type case_idx = 0;
         case_idx < case_matches.size();
         ++case_idx) {
      DCHECK(!case_results[case_idx]->isNative());
      if (!case_matches[case_idx]->empty()) {
        MultiplexIndirectColumnVector(
            source_sequence,
            *case_matches[case_idx],
            static_cast<const IndirectColumnVector&>(*case_results[case_idx]),
            indirect_result.get());
      }
    }

    if (else_result != nullptr) {
      DCHECK(!else_result->isNative());
      DCHECK(!else_matches.empty());
      MultiplexIndirectColumnVector(source_sequence,
                                    else_matches,
                                    static_cast<const IndirectColumnVector&>(*else_result),
                                    indirect_result.get());
    }

    return ColumnVectorPtr(indirect_result.release());
  }
}

void ScalarCaseExpression::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const Expression*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const Expression*>> *container_child_fields) const {
  Scalar::getFieldStringItems(inline_field_names,
                              inline_field_values,
                              non_container_child_field_names,
                              non_container_child_fields,
                              container_child_field_names,
                              container_child_fields);

  if (has_static_value_) {
    inline_field_names->emplace_back("static_value");
    if (static_value_.isNull()) {
      inline_field_values->emplace_back("NULL");
    } else {
      inline_field_values->emplace_back(type_.printValueToString(static_value_));
    }
  }

  container_child_field_names->emplace_back("when_predicates");
  container_child_fields->emplace_back();
  for (const auto &predicate : when_predicates_) {
    container_child_fields->back().emplace_back(predicate.get());
  }

  container_child_field_names->emplace_back("result_expressions");
  container_child_fields->emplace_back();
  for (const auto &expression : result_expressions_) {
    container_child_fields->back().emplace_back(expression.get());
  }

  if (else_result_expression_ != nullptr) {
    non_container_child_field_names->emplace_back("else_result_expression");
    non_container_child_fields->emplace_back(else_result_expression_.get());
  }
}

}  // namespace quickstep
