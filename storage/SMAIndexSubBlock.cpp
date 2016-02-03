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
typedef sma_internal::SMAEntry SMAEntry;

namespace sma_internal {

/**
 * @return A caller-mananged TypedValue for sum aggregation. Promotes types to a 
 *         higher precision type. For example int->int64. Returns nullptr if the
 *         given type cannot be used for summation. For example, Char.
 */
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
      initialized_(false),
      requires_rebuild_(true),
      sub_operators_(),
      add_operators_(),
      less_comparisons_(),
      equal_comparisons_() {
  CHECK(DescriptionIsValid(relation_, description_))
      << "Attempted to construct an SMAIndexSubBlock from an invalid description.";

  int num_indexed_attributes = description.ExtensionSize(SMAIndexSubBlockDescription::indexed_attribute_id);

  CHECK((sizeof(std::uint32_t) + (num_indexed_attributes * sizeof(SMAEntry))) <= sub_block_memory_size_)
      << "Attempted to create SMAIndexSubBlock without enough space allocated.";

  // The first entry is after the entry for count.
  SMAEntry *first_entry = reinterpret_cast<SMAEntry*>((char*)sub_block_memory_ + sizeof(std::uint32_t));

  // Iterate through each attribute which is being indexed.
  for (int indexed_attribute_num = 0;
       indexed_attribute_num < num_indexed_attributes;
       ++indexed_attribute_num) {
    const attribute_id attribute = description_.GetExtension(
        SMAIndexSubBlockDescription::indexed_attribute_id,
        indexed_attribute_num);

    const Type &attribute_type = tuple_store_.getRelation().getAttributeById(attribute)->getType();

    SMAEntry *entry = first_entry + indexed_attribute_num;
    initializeEntry(entry, new_block, attribute, attribute_type);

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
}

void SMAIndexSubBlock::initializeEntry(SMAEntry *entry,
                                       bool new_block,
                                       attribute_id attribute,
                                       const Type &attribute_type) {
  if (new_block) {
    entry->attribute_ = attribute;
    entry->type_ = attribute_type.getTypeID();
    entry->min_entry_.valid_ = false;
    entry->max_entry_.valid_ = false;
    sma_internal::setTypedValueForSum(entry);
  }
}

bool SMAIndexSubBlock::DescriptionIsValid(const CatalogRelationSchema & relation,
                                          const IndexSubBlockDescription & description) {
  if (!description.IsInitialized()) {
   return false;
  }
  if (description.sub_block_type() != IndexSubBlockDescription::SMA) {
   return false;
  }
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
  }
  return true;
}

bool SMAIndexSubBlock::bulkAddEntries(const TupleIdSequence & tuples) {
  return true;
}

void SMAIndexSubBlock::bulkRemoveEntries(const TupleIdSequence &tuples) { }

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
  return true;
}

void SMAIndexSubBlock::removeEntry(const tuple_id tuple) { }

TupleIdSequence* SMAIndexSubBlock::getMatchesForPredicate(
    const ComparisonPredicate &predicate,
    const TupleIdSequence *filter) const {
  return new TupleIdSequence(tuple_store_.getMaxTupleID() + 1);
}

bool SMAIndexSubBlock::rebuild() {
  return true;
}

bool SMAIndexSubBlock::requiresRebuild() const {
  return true;
}

}  // namespace quickstep
