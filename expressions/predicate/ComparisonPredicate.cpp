/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "expressions/predicate/ComparisonPredicate.hpp"

#include <cstddef>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/predicate/PredicateCost.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "storage/IndexSubBlock.hpp"
#include "storage/SubBlocksReference.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "storage/ValueAccessor.hpp"
#include "types/Type.hpp"
#include "types/TypeErrors.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/operations/Operation.pb.h"
#include "types/operations/comparisons/Comparison.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"

#include "glog/logging.h"

namespace quickstep {

class TupleIdSequence;

ComparisonPredicate::ComparisonPredicate(const Comparison &comparison,
                                         Scalar *left_operand,
                                         Scalar *right_operand)
    : comparison_(comparison),
      left_operand_(left_operand),
      right_operand_(right_operand) {
  initHelper(false);
}

serialization::Predicate ComparisonPredicate::getProto() const {
  serialization::Predicate proto;
  proto.set_predicate_type(serialization::Predicate::COMPARISON);
  proto.MutableExtension(serialization::ComparisonPredicate::comparison)->CopyFrom(comparison_.getProto());
  proto.MutableExtension(serialization::ComparisonPredicate::left_operand)->CopyFrom(left_operand_->getProto());
  proto.MutableExtension(serialization::ComparisonPredicate::right_operand)->CopyFrom(right_operand_->getProto());

  return proto;
}

Predicate* ComparisonPredicate::clone() const {
  return new ComparisonPredicate(comparison_,
                                 left_operand_->clone(),
                                 right_operand_->clone());
}

bool ComparisonPredicate::isAttributeLiteralComparisonPredicate() const {
  return (left_operand_->hasStaticValue() && (right_operand_->getDataSource() == Scalar::kAttribute))
         || (right_operand_->hasStaticValue() && (left_operand_->getDataSource() == Scalar::kAttribute));
}

bool ComparisonPredicate::matchesForSingleTuple(const ValueAccessor &accessor,
                                                const tuple_id tuple) const {
  if (fast_comparator_.get() == nullptr) {
    return static_result_;
  } else {
    return fast_comparator_->compareTypedValues(left_operand_->getValueForSingleTuple(accessor, tuple),
                                                right_operand_->getValueForSingleTuple(accessor, tuple));
  }
}

bool ComparisonPredicate::matchesForJoinedTuples(
    const ValueAccessor &left_accessor,
    const relation_id left_relation_id,
    const tuple_id left_tuple_id,
    const ValueAccessor &right_accessor,
    const relation_id right_relation_id,
    const tuple_id right_tuple_id) const {
  if (fast_comparator_.get() == nullptr) {
    return static_result_;
  } else {
    return fast_comparator_->compareTypedValues(
        left_operand_->getValueForJoinedTuples(left_accessor,
                                               left_relation_id,
                                               left_tuple_id,
                                               right_accessor,
                                               right_relation_id,
                                               right_tuple_id),
        right_operand_->getValueForJoinedTuples(left_accessor,
                                                left_relation_id,
                                                left_tuple_id,
                                                right_accessor,
                                                right_relation_id,
                                                right_tuple_id));
  }
}

TupleIdSequence* ComparisonPredicate::getAllMatches(
    ValueAccessor *accessor,
    const SubBlocksReference *sub_blocks_ref,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_map) const {
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
  static constexpr bool short_circuit = true;
#else
  static constexpr bool short_circuit = false;
#endif

  if (fast_comparator_.get() == nullptr) {
    return GenerateSequenceForStaticResult(accessor, filter, existence_map, static_result_);
  } else if (sub_blocks_ref != nullptr) {
    // Try to find a method faster than a simple scan to evaluate this
    // comparison.
    std::size_t fastest_subblock = std::numeric_limits<std::size_t>::max();
    predicate_cost_t lowest_cost
        = sub_blocks_ref->tuple_store.estimatePredicateEvaluationCost(*this);
    for (std::size_t index_id = 0; index_id < sub_blocks_ref->indices.size(); ++index_id) {
      if (sub_blocks_ref->indices_consistent[index_id]) {
        const predicate_cost_t index_cost
            = sub_blocks_ref->indices[index_id].estimatePredicateEvaluationCost(*this);
        if (index_cost < lowest_cost) {
          fastest_subblock = index_id;
          lowest_cost = index_cost;
        }
      }
    }
    if (fastest_subblock != std::numeric_limits<std::size_t>::max()) {
      return sub_blocks_ref->indices[fastest_subblock].getMatchesForPredicate(*this, filter);
    } else if (!PredicateCostIsSimpleScan(lowest_cost)) {
      return sub_blocks_ref->tuple_store.getMatchesForPredicate(*this, filter);
    }
  }

  // When short-circuiting is turned on, we should only evaluate the comparison
  // for tuples that are set in '*filter'. We might need to generate a
  // ColumnVector of values for the operand on either side of the comparison
  // with a call to Scalar::getAllValues() before we evaluate the comparison,
  // but we only actually care about the operand's values for the tuples that
  // are set in '*filter' (evaluating the Scalar's value for other tuples would
  // be a waste of memory and computation). Therefore, we set up a
  // ValueAccessor here that only iterates over the tuples in '*filter', and we
  // pass it in to Scalar::getAllValues() calls below so that we only generate
  // those values that we actually need to check. We use '*filter' in place of
  // the existence bitmap in such cases, because the filter effectively IS the
  // existence bitmap for '*short_circuit_adapter' (this way, the tuple IDs
  // don't get screwed up and still correspond to the original "physical" IDs
  // in the block).
  std::unique_ptr<ValueAccessor> short_circuit_adapter;
  if (short_circuit && (filter != nullptr)) {
    short_circuit_adapter.reset(accessor->createSharedTupleIdSequenceAdapterVirtual(*filter));
  }

  // NOTE(chasseur): We don't check if both operations have a static value,
  // because if they did then this comparison would have a static result
  // which is handled above.
  if (left_operand_->hasStaticValue()) {
#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
    const attribute_id right_operand_attr_id
        = right_operand_->getAttributeIdForValueAccessor();
    if (right_operand_attr_id != -1) {
      return fast_comparator_->compareStaticValueAndValueAccessor(
          left_operand_->getStaticValue(),
          accessor,
          right_operand_attr_id,
          filter);
    }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

    if (short_circuit_adapter) {
      std::unique_ptr<ColumnVector> right_values(right_operand_->getAllValues(
          short_circuit_adapter.get(),
          sub_blocks_ref));
      return fast_comparator_->compareStaticValueAndColumnVector(
          left_operand_->getStaticValue(),
          *right_values,
          nullptr,
          filter);
    } else {
      std::unique_ptr<ColumnVector> right_values(right_operand_->getAllValues(
          accessor,
          sub_blocks_ref));
      return fast_comparator_->compareStaticValueAndColumnVector(
          left_operand_->getStaticValue(),
          *right_values,
          filter,
          existence_map);
    }
  } else if (right_operand_->hasStaticValue()) {
#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
    const attribute_id left_operand_attr_id
        = left_operand_->getAttributeIdForValueAccessor();
    if (left_operand_attr_id != -1) {
      return fast_comparator_->compareValueAccessorAndStaticValue(
          accessor,
          left_operand_attr_id,
          right_operand_->getStaticValue(),
          filter);
    }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

    if (short_circuit_adapter) {
      std::unique_ptr<ColumnVector> left_values(left_operand_->getAllValues(
          short_circuit_adapter.get(),
          sub_blocks_ref));
      return fast_comparator_->compareColumnVectorAndStaticValue(
          *left_values,
          right_operand_->getStaticValue(),
          nullptr,
          filter);
    } else {
      std::unique_ptr<ColumnVector> left_values(left_operand_->getAllValues(
          accessor,
          sub_blocks_ref));
      return fast_comparator_->compareColumnVectorAndStaticValue(
          *left_values,
          right_operand_->getStaticValue(),
          filter,
          existence_map);
    }
  } else {
#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
    const attribute_id left_operand_attr_id
        = left_operand_->getAttributeIdForValueAccessor();
    const attribute_id right_operand_attr_id
        = right_operand_->getAttributeIdForValueAccessor();

    if (left_operand_attr_id != -1) {
      if (right_operand_attr_id != -1) {
        return fast_comparator_->compareSingleValueAccessor(accessor,
                                                            left_operand_attr_id,
                                                            right_operand_attr_id,
                                                            filter);
      } else {
        if (short_circuit_adapter) {
          std::unique_ptr<ColumnVector> right_values(right_operand_->getAllValues(
              short_circuit_adapter.get(),
              sub_blocks_ref));
          return fast_comparator_->compareValueAccessorAndColumnVector(
              short_circuit_adapter.get(),
              left_operand_attr_id,
              *right_values,
              nullptr,
              filter);
        } else {
          std::unique_ptr<ColumnVector> right_values(right_operand_->getAllValues(
              accessor,
              sub_blocks_ref));
          return fast_comparator_->compareValueAccessorAndColumnVector(accessor,
                                                                       left_operand_attr_id,
                                                                       *right_values,
                                                                       filter,
                                                                       existence_map);
        }
      }
    } else if (right_operand_attr_id != -1) {
      if (short_circuit_adapter) {
        std::unique_ptr<ColumnVector> left_values(left_operand_->getAllValues(
            short_circuit_adapter.get(),
            sub_blocks_ref));
        return fast_comparator_->compareColumnVectorAndValueAccessor(
            *left_values,
            short_circuit_adapter.get(),
            right_operand_attr_id,
            nullptr,
            filter);
      } else {
        std::unique_ptr<ColumnVector> left_values(left_operand_->getAllValues(
            accessor,
            sub_blocks_ref));
        return fast_comparator_->compareColumnVectorAndValueAccessor(*left_values,
                                                                     accessor,
                                                                     right_operand_attr_id,
                                                                     filter,
                                                                     existence_map);
      }
    }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

    if (short_circuit_adapter) {
      std::unique_ptr<ColumnVector> left_values(left_operand_->getAllValues(
          short_circuit_adapter.get(),
          sub_blocks_ref));
      std::unique_ptr<ColumnVector> right_values(right_operand_->getAllValues(
          short_circuit_adapter.get(),
          sub_blocks_ref));
      return fast_comparator_->compareColumnVectors(*left_values,
                                                    *right_values,
                                                    nullptr,
                                                    filter);
    } else {
      std::unique_ptr<ColumnVector> left_values(left_operand_->getAllValues(
          accessor,
          sub_blocks_ref));
      std::unique_ptr<ColumnVector> right_values(right_operand_->getAllValues(
          accessor,
          sub_blocks_ref));
      return fast_comparator_->compareColumnVectors(*left_values,
                                                    *right_values,
                                                    filter,
                                                    existence_map);
    }
  }
}

bool ComparisonPredicate::getStaticResult() const {
  if (fast_comparator_.get() == nullptr) {
    return static_result_;
  } else {
    FATAL_ERROR("Called getStaticResult() on a predicate which has no static result");
  }
}

std::pair<bool, attribute_id> ComparisonPredicate::getAttributeFromAttributeLiteralComparison() const {
  DCHECK(isAttributeLiteralComparisonPredicate());
  if (left_operand_->hasStaticValue()) {
    DCHECK_EQ(Scalar::kAttribute, right_operand_->getDataSource());
    return std::pair<bool, attribute_id>(
        false,
        static_cast<const ScalarAttribute&>(*right_operand_).getAttribute().getID());
  } else {
    DCHECK_EQ(Scalar::kAttribute, left_operand_->getDataSource());
    return std::pair<bool, attribute_id>(
        true,
        static_cast<const ScalarAttribute&>(*left_operand_).getAttribute().getID());
  }
}

void ComparisonPredicate::initHelper(bool own_children) {
  if (comparison_.canCompareTypes(left_operand_->getType(), right_operand_->getType())) {
    if (left_operand_->hasStaticValue() && right_operand_->hasStaticValue()) {
      static_result_ = comparison_.compareTypedValuesChecked(left_operand_->getStaticValue(),
                                                             left_operand_->getType(),
                                                             right_operand_->getStaticValue(),
                                                             right_operand_->getType());
    } else {
      fast_comparator_.reset(comparison_.makeUncheckedComparatorForTypes(left_operand_->getType(),
                                                                         right_operand_->getType()));
    }
  } else {
    const Type &left_operand_type = left_operand_->getType();
    const Type &right_operand_type = right_operand_->getType();
    if (!own_children) {
      left_operand_.release();
      right_operand_.release();
    }
    throw OperationInapplicableToType(comparison_.getName(),
                                      2,
                                      left_operand_type.getName().c_str(),
                                      right_operand_type.getName().c_str());
  }
}

}  // namespace quickstep
