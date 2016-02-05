/**
 *   Copyright 2016 Pivotal Software, Inc.
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

#include "storage/SMAIndexSubBlock.hpp"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <utility>
#include <exception>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "compression/CompressionDictionary.hpp"
#include "expressions/predicate/ComparisonPredicate.hpp"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/predicate/PredicateCost.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "storage/CompressedTupleStorageSubBlock.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageConstants.hpp"
#include "storage/StorageErrors.hpp"
#include "storage/SubBlockTypeRegistry.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "types/operations/comparisons/ComparisonUtil.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"
#include "utility/ScopedBuffer.hpp"

#include "glog/logging.h"

using std::memcpy;
using std::memmove;
using std::pair;
using std::size_t;
using std::sort;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::vector;
using std::map;
using std::exception;

namespace quickstep {

QUICKSTEP_REGISTER_INDEX(SMAIndexSubBlock, SMA);

typedef sma_internal::EntryReference EntryReference;
//typedef sma_internal::SMAPredicate SMAPredicate;
typedef sma_internal::SMAHeader SMAHeader;
typedef sma_internal::SMAEntry SMAEntry;

namespace sma_internal {

void setTypedValueForSum(SMAEntry *entry) {
  TypedValue *value = &(entry->sum_);
  switch (entry->type_) {
    case kInt:
    case kLong:
      new (value) TypedValue(static_cast<std::int64_t>(0));
      return;
    case kFloat:
    case kDouble:
      new (value) TypedValue(static_cast<double>(0.0));
      return;
    case kDatetimeInterval: {
      DatetimeIntervalLit zero;
      zero.interval_ticks = 0;
      new (value) TypedValue(zero);
      return;
    }
    case kYearMonthInterval: {
      YearMonthIntervalLit zero;
      zero.months = 0;
      new (value) TypedValue(zero);
      return;
    }
    default: {
      CHECK(false) << "SMA Index was created on an unindexable type.";
    }
  }
}

void setTypedValueForMinMax(SMAEntry *entry) {
  TypedValue *min = &(entry->min_entry_.value_);
  TypedValue *max = &(entry->max_entry_.value_);
  switch (entry->type_) {
    case kInt:
      new (min) TypedValue(static_cast<int>(0));
      new (max) TypedValue(static_cast<int>(0));
      return;
    case kLong:
      new (min) TypedValue(static_cast<std::int64_t>(0));
      new (min) TypedValue(static_cast<std::int64_t>(0));
      return;
    case kFloat:
      new (min) TypedValue(static_cast<float>(0));
      new (max) TypedValue(static_cast<float>(0));
    case kDouble:
      new (min) TypedValue(static_cast<double>(0.0));
      new (max) TypedValue(static_cast<double>(0.0));
      return;
    case kDatetimeInterval: {
      DatetimeIntervalLit zero;
      zero.interval_ticks = 0;
      new (min) TypedValue(zero);
      new (max) TypedValue(zero);
      return;
    }
    case kYearMonthInterval: {
      YearMonthIntervalLit zero;
      zero.months = 0;
      new (min) TypedValue(zero);
      new (max) TypedValue(zero);
      return;
    }
    default: {
      CHECK(false) << "SMA Index was created on an unindexable type.";
    }
  }
}

} // namespace sma_internal


SMAIndexSubBlock::SMAIndexSubBlock(const TupleStorageSubBlock &tuple_store,
                                   const IndexSubBlockDescription &description,
                                   const bool new_block,
                                   void *sub_block_memory,
                                   const std::size_t sub_block_memory_size)
    : IndexSubBlock(tuple_store,
                    description,
                    new_block,
                    sub_block_memory,
                    sub_block_memory_size),
      header_(nullptr),
      entries_(nullptr),
      indexed_attributes_(0),
      initialized_(false),
      sub_operators_(),
      add_operators_(),
      less_comparisons_(),
      equal_comparisons_() {
  CHECK(DescriptionIsValid(relation_, description_))
      << "Attempted to construct an SMAIndexSubBlock from an invalid description.";

  indexed_attributes_ = description.ExtensionSize(SMAIndexSubBlockDescription::indexed_attribute_id);

  CHECK((sizeof(sma_internal::SMAHeader) + (indexed_attributes_ * sizeof(SMAEntry))) <= sub_block_memory_size_)
      << "Attempted to create SMAIndexSubBlock without enough space allocated.";

  header_ = reinterpret_cast<SMAHeader*>(sub_block_memory_);
  entries_ = reinterpret_cast<SMAEntry*>(header_ + 1);

  // Iterate through each attribute which is being indexed.
  for (int indexed_attribute_num = 0;
       indexed_attribute_num < indexed_attributes_;
       ++indexed_attribute_num) {
    const attribute_id attribute = description_.GetExtension(
        SMAIndexSubBlockDescription::indexed_attribute_id,
        indexed_attribute_num);

    const Type &attribute_type = tuple_store_.getRelation().getAttributeById(attribute)->getType();
    SMAEntry *entry = entries_ + indexed_attribute_num;
    resetEntry(entry, attribute, attribute_type);

    // Initialize the operators.
    // TODO(marc): Attributes that share the same type can share operators. Making
    // this change will save space.
    TypeID sum_type = entry->sum_.getTypeID();
    sub_operators_.push_back(
        BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kSubtract)
            .makeUncheckedBinaryOperatorForTypes(TypeFactory::GetType(sum_type), attribute_type));
    add_operators_.push_back(
        BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd)
            .makeUncheckedBinaryOperatorForTypes(TypeFactory::GetType(sum_type), attribute_type));
    less_comparisons_.push_back(
        ComparisonFactory::GetComparison(ComparisonID::kLess)
            .makeUncheckedComparatorForTypes(attribute_type, attribute_type));
    equal_comparisons_.push_back(
        ComparisonFactory::GetComparison(ComparisonID::kEqual)
            .makeUncheckedComparatorForTypes(attribute_type, attribute_type));

    // Map the attribute's id to its entry's index.
    attribute_to_entry_.insert({attribute, indexed_attribute_num});
  }

  if (new_block) {
    header_->consistent_ = false;
  }
}

void SMAIndexSubBlock::resetEntry(SMAEntry *entry,
                                  attribute_id attribute,
                                  const Type &attribute_type) {
  entry->attribute_ = attribute;
  entry->type_ = attribute_type.getTypeID();
  entry->min_entry_.valid_ = false;
  entry->max_entry_.valid_ = false;
  setTypedValueForMinMax(entry);
  sma_internal::setTypedValueForSum(entry);
}

void SMAIndexSubBlock::resetEntries() {
  for (int indexed_attribute_num = 0;
       indexed_attribute_num < indexed_attributes_;
       ++indexed_attribute_num) {
    const attribute_id attribute = description_.GetExtension(
        SMAIndexSubBlockDescription::indexed_attribute_id,
        indexed_attribute_num);

    const Type &attribute_type = tuple_store_.getRelation().getAttributeById(attribute)->getType();

    resetEntry(entries_ + indexed_attribute_num, attribute, attribute_type);
  }
}

bool SMAIndexSubBlock::DescriptionIsValid(const CatalogRelationSchema &relation,
                                          const IndexSubBlockDescription &description) {
  if (!description.IsInitialized()) {
    return false;
  }
  if (description.sub_block_type() != IndexSubBlockDescription::SMA) {
    return false;
  }
  // Must have at least one indexed attribute.
  if(description.ExtensionSize(SMAIndexSubBlockDescription::indexed_attribute_id) == 0){
    return false;
  }
  // Check that all key attributes exist and are fixed-length.
  for (int indexed_attribute_num = 0;
       indexed_attribute_num < description.ExtensionSize(SMAIndexSubBlockDescription::indexed_attribute_id);
       ++indexed_attribute_num) {
    attribute_id indexed_attribute_id = description.GetExtension(
        SMAIndexSubBlockDescription::indexed_attribute_id, indexed_attribute_num);
    if (!relation.hasAttributeWithId(indexed_attribute_id)) {
      return false;
    }
    const Type &attr_type = relation.getAttributeById(indexed_attribute_id)->getType();
    if (attr_type.isVariableLength()) {
      return false;
    }
    if (!TypedValue::RepresentedInline(attr_type.getTypeID())) {
      return false;
    }
  }
  return true;
}

bool SMAIndexSubBlock::bulkAddEntries(const TupleIdSequence &tuples) {
  header_->consistent_ = false;
  return true; // There will always be space for the entry.
}

void SMAIndexSubBlock::bulkRemoveEntries(const TupleIdSequence &tuples) {
  header_->consistent_ = false;
}

predicate_cost_t SMAIndexSubBlock::estimatePredicateEvaluationCost(
    const ComparisonPredicate &predicate) const {
  return predicate_cost::kInfinite;
}

std::size_t SMAIndexSubBlock::EstimateBytesPerTuple(
    const CatalogRelationSchema &relation,
    const IndexSubBlockDescription &description) {
  // TODO(marc): Returning 1 almost always gives too much space. An enhancement
  // would be to inform the storage subblock that this index uses space w.r.t. 
  // # of attributes being indexed, not tuples.
  return 1;
}

bool SMAIndexSubBlock::addEntry(const tuple_id tuple) {
  header_->consistent_ = false;
  return true; // There will always be space to insert the entry.
}

void SMAIndexSubBlock::removeEntry(const tuple_id tuple) {
  header_->consistent_ = false;
}

TupleIdSequence* SMAIndexSubBlock::getMatchesForPredicate(
    const ComparisonPredicate &predicate,
    const TupleIdSequence *filter) const {
  return new TupleIdSequence(tuple_store_.getMaxTupleID() + 1);
}

bool SMAIndexSubBlock::rebuild() {
  resetEntries();
  header_->count_ = 0;
  if (tuple_store_.isPacked()) {
    for (tuple_id tid = 0; tid <= tuple_store_.getMaxTupleID(); ++tid) {
      addTuple(tid);
    }
  } else {
    for (tuple_id tid = 0; tid <= tuple_store_.getMaxTupleID(); ++tid) {
      if (tuple_store_.hasTupleWithID(tid)) {
        addTuple(tid);
      }
    }
  }
  header_->consistent_ = true;
  return true;
}

void SMAIndexSubBlock::addTuple(tuple_id tuple) {
  for (int index = 0; index < indexed_attributes_; ++index) {
    SMAEntry &entry = entries_[index];
    TypedValue tuple_value = tuple_store_.getAttributeValueTyped(tuple, entry.attribute_);

    if (tuple_value.isNull()) {
      continue;
    }

    entry.sum_ = add_operators_[index].applyToTypedValues(entry.sum_, tuple_value);

    if (!entry.min_entry_.valid_) {
      memcpy(&entry.min_entry_.value_, &tuple_value, sizeof(TypedValue));
      //entry.min_entry_.value_ = tuple_value;
      entry.min_entry_.tuple_ = tuple;
      entry.min_entry_.valid_ = true;
    } else {
      if (less_comparisons_[index].compareTypedValues(tuple_value, entry.min_entry_.value_)) {
        entry.min_entry_.value_ = tuple_value;
        entry.min_entry_.tuple_ = tuple;
      }
    }

    if (!entry.max_entry_.valid_) {
      memcpy(&entry.max_entry_.value_, &tuple_value, sizeof(TypedValue));
      //entry.max_entry_.value_ = tuple_value;
      entry.max_entry_.tuple_ = tuple;
      entry.max_entry_.valid_ = true;
    } else {
      if (less_comparisons_[index].compareTypedValues(entry.max_entry_.value_, tuple_value)) {
        entry.max_entry_.value_ = tuple_value;
        entry.max_entry_.tuple_ = tuple;
      }
    }
  }
  header_->count_++;
}

bool SMAIndexSubBlock::requiresRebuild() const {
  return !header_->consistent_;
}

}  // namespace quickstep
