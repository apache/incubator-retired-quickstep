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

/**
 * @brief Summation will promote lower precision types to higher precision types.
 *
 * @return The higher precision sum typeid. Returns kNullType to indicate that the
 *         given type cannot be summed.
 */
inline TypeID sumType(TypeID type) {
  switch (type) {
    case kInt:
    case kLong:
      return kLong;
    case kFloat:
    case kDouble:
      return kDouble;
    default: {
      return kNullType;
    }
  }
}

/**
 * @return True if the type can be summed.
 */
inline bool canSum(TypeID type) {
  return type == kInt || type == kLong || type == kFloat || type == kDouble;
}

/**
 * @brief Sets the given entry's sum to zero using the correct promoted type
 *        of the sum. Does nothing if the entry's type cannot be summed.
 * @note We ignore clearing any old data held in the previous sum because Double
 *       and Long types are always represented inline.
 */
inline void setTypedValueForSum(SMAEntry *entry) {
  TypeID sum_type = sumType(entry->type_);
  if (sum_type == kLong) {
    new (&entry->sum_) TypedValue(static_cast<std::int64_t>(0));
  } else if (sum_type == kDouble) {
    new (&entry->sum_) TypedValue(static_cast<double>(0.0));
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
      add_operations_(),
      less_comparisons_(),
      equal_comparisons_() {
  CHECK(DescriptionIsValid(relation_, description_))
      << "Attempted to construct an SMAIndexSubBlock from an invalid description.";

  indexed_attributes_ = description.ExtensionSize(SMAIndexSubBlockDescription::indexed_attribute_id);

  CHECK((sizeof(sma_internal::SMAHeader) + (indexed_attributes_ * sizeof(SMAEntry))) <= sub_block_memory_size_)
      << "Attempted to create SMAIndexSubBlock without enough space allocated.";

  header_ = reinterpret_cast<SMAHeader*>(sub_block_memory_);
  entries_ = reinterpret_cast<SMAEntry*>(header_ + 1);

  if (new_block) {
    header_->consistent_ = false;
  }

  // Iterate through each attribute which is being indexed.
  for (int indexed_attribute_num = 0;
       indexed_attribute_num < indexed_attributes_;
       ++indexed_attribute_num) {
    const attribute_id attribute = description_.GetExtension(
        SMAIndexSubBlockDescription::indexed_attribute_id,
        indexed_attribute_num);

    const Type &attribute_type = tuple_store_.getRelation().getAttributeById(attribute)->getType();
    SMAEntry *entry = entries_ + indexed_attribute_num;

    // Initialize the operator map.
    if (sma_internal::canSum(attribute_type.getTypeID())) {
      TypeID attr_sum_type = sma_internal::sumType(attribute_type.getTypeID());
      if (add_operations_.find(static_cast<int>(attr_sum_type)) == add_operations_.end()) {
        add_operations_.insert({
          static_cast<int>(attribute_type.getTypeID()),
          BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd)
              .makeUncheckedBinaryOperatorForTypes(attribute_type, TypeFactory::GetType(attr_sum_type))});
      }
    }

    // Initialize comparison map. Maps attribute type to comparison function.
    int attr_typeid = static_cast<int>(attribute_type.getTypeID());
    if (less_comparisons_.find(attr_typeid) == less_comparisons_.end()) {
      less_comparisons_.insert({attr_typeid,
          ComparisonFactory::GetComparison(ComparisonID::kLess)
              .makeUncheckedComparatorForTypes(attribute_type, attribute_type)});
      equal_comparisons_.insert({attr_typeid,
          ComparisonFactory::GetComparison(ComparisonID::kEqual)
              .makeUncheckedComparatorForTypes(attribute_type, attribute_type)});
    }

    // Map the attribute's id to its entry's index.
    attribute_to_entry_.insert({attribute, indexed_attribute_num});

    if (!header_->consistent_) {
      resetEntry(entry, attribute, attribute_type);
    } else {
      // If the data held by min/max is out of line, we must retrieve it.
      if (!TypedValue::RepresentedInline(entry->type_)) {

        // First, set to 0 so that we don't try to free invalid memory on copy.
        // Next, copy from the tuple store. This will copy and give us ownership of out of line data.
        if (entry->min_entry_.valid_) {
          std::cout << "set min\n";
          new (&entry->min_entry_.value_) TypedValue();
          entry->min_entry_.value_ = tuple_store
              .getAttributeValueTyped(entry->min_entry_.tuple_, entry->attribute_);
          std::cout << "typeid: " << entry->min_entry_.value_.getTypeID() << "\n";
        }
        if (entry->max_entry_.valid_) {
          std::cout << "set max\n";
          new (&entry->max_entry_.value_) TypedValue();
          entry->max_entry_.value_ = tuple_store
              .getAttributeValueTyped(entry->max_entry_.tuple_, entry->attribute_);
          std::cout << "typeid: " << entry->max_entry_.value_.getTypeID() << "\n";
        }
      }
    }
  }
}

SMAIndexSubBlock::~SMAIndexSubBlock() {
  // Any typed values which have out of line data must be cleared.
  freeOutOfLineData();

  // Delete owned comparators and operations.
  for (auto pair : less_comparisons_) {
    delete pair.second;
  }
  for (auto pair : equal_comparisons_) {
    delete pair.second;
  }
  for (auto pair : add_operations_) {
    delete pair.second;
  }
}

void SMAIndexSubBlock::resetEntry(SMAEntry *entry,
                                  attribute_id attribute,
                                  const Type &attribute_type) {
  entry->attribute_ = attribute;
  entry->type_ = attribute_type.getTypeID();
  if (entry->min_entry_.valid_) {
    entry->min_entry_.value_.clear();
    entry->min_entry_.valid_ = false;
  }
  if (entry->max_entry_.valid_) {
    entry->max_entry_.value_.clear();
    entry->max_entry_.valid_ = false;
  }
  sma_internal::setTypedValueForSum(entry);
}

void SMAIndexSubBlock::resetEntries() {
  freeOutOfLineData();

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

void SMAIndexSubBlock::freeOutOfLineData() {
  for (int indexed_attribute_num = 0;
       indexed_attribute_num < indexed_attributes_;
       ++indexed_attribute_num) {
    SMAEntry &entry = entries_[indexed_attribute_num];
    if (!TypedValue::RepresentedInline(entry.type_)) {
      if (entry.min_entry_.valid_) {
        entry.min_entry_.value_.clear();
      }
      if (entry.max_entry_.valid_) {
        entry.max_entry_.value_.clear();
      }
    }
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
    SMAEntry *entry = entries_ + index;
    TypedValue tuple_value = tuple_store_.getAttributeValueTyped(tuple, entry->attribute_);

    if (tuple_value.isNull()) {
      continue;
    }

    if (sma_internal::canSum(entry->type_)) {
      entry->sum_ = add_operations_
                       .at(static_cast<int>(entry->type_))
                            ->applyToTypedValues(tuple_value, entry->sum_);
    }

    if (!entry->min_entry_.valid_) {
      entry->min_entry_.value_ = tuple_value;
      entry->min_entry_.value_.ensureNotReference();
      entry->min_entry_.tuple_ = tuple;
      entry->min_entry_.valid_ = true;
    } else {
      if (less_comparisons_.at(entry->type_)->compareTypedValues(tuple_value, entry->min_entry_.value_)) {
        entry->min_entry_.value_ = tuple_value;
        entry->min_entry_.value_.ensureNotReference();
        entry->min_entry_.tuple_ = tuple;
      }
    }

    if (!entry->max_entry_.valid_) {
      entry->max_entry_.value_ = tuple_value;
      entry->max_entry_.value_.ensureNotReference();
      entry->max_entry_.tuple_ = tuple;
      entry->max_entry_.valid_ = true;
    } else {
      if (less_comparisons_.at(entry->type_)->compareTypedValues(entry->max_entry_.value_, tuple_value)) {
        entry->max_entry_.value_ = tuple_value;
        entry->max_entry_.value_.ensureNotReference();
        entry->max_entry_.tuple_ = tuple;
      }
    }
  }

  header_->count_++;
}

bool SMAIndexSubBlock::requiresRebuild() const {
  return !header_->consistent_;
}

bool SMAIndexSubBlock::hasEntryForAttribute(attribute_id attribute) const {
  return attribute_to_entry_.find(attribute) != attribute_to_entry_.end();
}

}  // namespace quickstep
