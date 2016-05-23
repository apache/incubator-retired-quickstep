/**
 *   Copyright 2016 Pivotal Software, Inc.
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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
#include <exception>
#include <memory>
#include <new>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "expressions/predicate/ComparisonPredicate.hpp"
#include "expressions/predicate/PredicateCost.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageConstants.hpp"
#include "storage/SubBlockTypeRegistry.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "types/IntervalLit.hpp"
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
#include "utility/PtrVector.hpp"

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
using std::exception;

namespace quickstep {

QUICKSTEP_REGISTER_INDEX(SMAIndexSubBlock, SMA);

typedef sma_internal::EntryReference EntryReference;
typedef sma_internal::SMAPredicate SMAPredicate;
typedef sma_internal::SMAHeader SMAHeader;
typedef sma_internal::SMAEntry SMAEntry;
typedef sma_internal::Selectivity Selectivity;

namespace sma_internal {

/**
 * @return Selectivity of an equals predicate.
 */
Selectivity getSelectivityForEquals(const TypedValue &literal,
                                    const TypedValue &min,
                                    const TypedValue &max,
                                    const UncheckedComparator *less_comparator) {
  if (less_comparator->compareTypedValues(literal, min) ||
      less_comparator->compareTypedValues(max, literal)) {
    return Selectivity::kNone;
  }
  return Selectivity::kSome;
}

/**
 * @return Selectivity of a less than predicate.
 */
Selectivity getSelectivityForLess(const TypedValue &literal,
                                  const TypedValue &min,
                                  const TypedValue &max,
                                  const UncheckedComparator *less_comparator,
                                  const UncheckedComparator *equals_comparator) {
  if (less_comparator->compareTypedValues(max, literal)) {
    return Selectivity::kAll;
  } else if (less_comparator->compareTypedValues(literal, min) ||
             equals_comparator->compareTypedValues(literal, min)) {
    return Selectivity::kNone;
  }
  return Selectivity::kSome;
}

/**
 * @return Selectivity of a less than equals predicate.
 */
Selectivity getSelectivityForLessOrEquals(const TypedValue &literal,
                                          const TypedValue &min,
                                          const TypedValue &max,
                                          const UncheckedComparator *less_comparator,
                                          const UncheckedComparator *equals_comparator) {
  if (less_comparator->compareTypedValues(max, literal) ||
      equals_comparator->compareTypedValues(max, literal)) {
    return Selectivity::kAll;
  } else if (less_comparator->compareTypedValues(literal, min)) {
    return Selectivity::kNone;
  }
  return Selectivity::kSome;
}

/**
 * @return Selectivity of a greater than predicate.
 */
Selectivity getSelectivityForGreater(const TypedValue &literal,
                                     const TypedValue &min,
                                     const TypedValue &max,
                                     const UncheckedComparator *less_comparator,
                                     const UncheckedComparator *equals_comparator) {
  if (less_comparator->compareTypedValues(literal, min)) {
    return Selectivity::kAll;
  } else if (less_comparator->compareTypedValues(max, literal) ||
             equals_comparator->compareTypedValues(max, literal)) {
    return Selectivity::kNone;
  }
  return Selectivity::kSome;
}

/**
 * @return Selectivity of a greater than or equals predicate.
 */
Selectivity getSelectivityForGreaterOrEquals(const TypedValue &literal,
                                             const TypedValue &min,
                                             const TypedValue &max,
                                             const UncheckedComparator *less_comparator,
                                             const UncheckedComparator *equals_comparator) {
  if (less_comparator->compareTypedValues(literal, min) ||
      equals_comparator->compareTypedValues(literal, min)) {
    return Selectivity::kAll;
  } else if (less_comparator->compareTypedValues(max, literal)) {
    return Selectivity::kNone;
  }
  return Selectivity::kSome;
}

Selectivity getSelectivity(const TypedValue &literal,
                           const ComparisonID comparison,
                           const TypedValue &min,
                           const TypedValue &max,
                           const UncheckedComparator *less_comparator,
                           const UncheckedComparator *equals_comparator) {
  switch (comparison) {
    case ComparisonID::kEqual:
      return getSelectivityForEquals(literal, min, max, less_comparator);
    case ComparisonID::kLess:
      return getSelectivityForLess(literal, min, max, less_comparator, equals_comparator);
    case ComparisonID::kLessOrEqual:
      return getSelectivityForLessOrEquals(literal, min, max, less_comparator, equals_comparator);
    case ComparisonID::kGreater:
      return getSelectivityForGreater(literal, min, max, less_comparator, equals_comparator);
    case ComparisonID::kGreaterOrEqual:
      return getSelectivityForGreaterOrEquals(literal, min, max, less_comparator, equals_comparator);
    default:
      return Selectivity::kUnknown;
  }
}

SMAPredicate* SMAPredicate::ExtractSMAPredicate(const ComparisonPredicate &predicate) {
  // Check that the predicate is contains an attribute and a literal value.
  if (predicate.getLeftOperand().hasStaticValue() &&
      predicate.getRightOperand().getDataSource() == Scalar::kAttribute) {
    return new SMAPredicate(
         static_cast<const ScalarAttribute&>(predicate.getRightOperand()).getAttribute().getID(),
         flipComparisonID(predicate.getComparison().getComparisonID()),
         predicate.getLeftOperand().getStaticValue().makeReferenceToThis());
  } else if (predicate.getRightOperand().hasStaticValue() &&
      predicate.getLeftOperand().getDataSource() == Scalar::kAttribute) {
    return new SMAPredicate(
        static_cast<const ScalarAttribute&>(predicate.getLeftOperand()).getAttribute().getID(),
        predicate.getComparison().getComparisonID(),
        predicate.getRightOperand().getStaticValue().makeReferenceToThis());
  }
  // Predicate is improper form, so return nullptr.
  return nullptr;
}

/**
 * @brief Summation will promote lower precision types to higher precision types.
 *
 * @param type The type id of the type we wish to sum over.
 * @return The higher precision sum typeid. Returns kNullType to indicate that the
 *         given type cannot be summed (in lieu of a better sigil).
 */
TypeID getTypeForSum(TypeID type) {
  switch (type) {
    case kInt:
    case kLong:
      return kLong;
    case kFloat:
    case kDouble:
      return kDouble;
    default:
      return kNullType;
  }
}

/**
 * @param type A type id.
 * @return True if the type can be summed.
 */
bool canApplySumToType(TypeID type) {
  return getTypeForSum(type) != kNullType;
}

/**
 * @brief Sets the given entry's sum to zero using the correct promoted type
 *        of the sum. Does nothing if the entry's type cannot be summed.
 * @note We ignore clearing any old data held in the previous sum because Double
 *       and Long types are always represented inline.
 *
 * @param entry A pointer to the entry to modify.
 */
void initializeTypedValueForSum(SMAEntry *entry) {
  TypeID sum_type = getTypeForSum(entry->type_id);
  if (sum_type == kLong) {
    new (&entry->sum_aggregate) TypedValue(static_cast<std::int64_t>(0));
  } else if (sum_type == kDouble) {
    new (&entry->sum_aggregate) TypedValue(static_cast<double>(0.0));
  } else {
    DCHECK(false) << "initializeTypedValueForSum called with unsummable type.";
  }
}

/**
 * @brief Correctly zeroes the SMA entry for each possible type.
 *
 * @param entry The entry for which the min/max references will be zeroed out.
 */
void initializeTypedValueForMinMax(SMAEntry *entry) {
  TypedValue *min = &(entry->min_entry_ref.value);
  TypedValue *max = &(entry->max_entry_ref.value);
  switch (entry->type_id) {
    case kInt: {
      new (min) TypedValue(static_cast<int>(0));
      new (max) TypedValue(static_cast<int>(0));
      return;
    }
    case kLong: {
      new (min) TypedValue(static_cast<std::int64_t>(0));
      new (min) TypedValue(static_cast<std::int64_t>(0));
      return;
    }
    case kFloat: {
      new (min) TypedValue(static_cast<float>(0));
      new (max) TypedValue(static_cast<float>(0));
    }
    case kDouble: {
      new (min) TypedValue(static_cast<double>(0.0));
      new (max) TypedValue(static_cast<double>(0.0));
      return;
    }
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
      CHECK(false)
          << "SMA Index was created on an unindexable type: "
          << entry->type_id;
    }
  }
}

}  // namespace sma_internal


SMAIndexSubBlock::SMAIndexSubBlock(const TupleStorageSubBlock &tuple_store,
                                   const IndexSubBlockDescription &description,
                                   const bool is_new_block,
                                   void *sub_block_memory,
                                   const std::size_t sub_block_memory_size)
    : IndexSubBlock(tuple_store,
                    description,
                    is_new_block,
                    sub_block_memory,
                    sub_block_memory_size),
      header_(nullptr),
      entries_(nullptr),
      total_attributes_(tuple_store.getRelation().size()),
      attribute_to_entry_(new int[tuple_store.getRelation().size()]),
      num_indexed_attributes_(description.indexed_attribute_ids_size()),
      initialized_(false) {
  CHECK(DescriptionIsValid(relation_, description_))
      << "Attempted to construct an SMAIndexSubBlock from an invalid description.";

  CHECK((sizeof(sma_internal::SMAHeader)
            + (num_indexed_attributes_ * sizeof(SMAEntry))) <= sub_block_memory_size_)
      << "Attempted to create SMAIndexSubBlock without enough space allocated.";

  header_ = reinterpret_cast<SMAHeader*>(sub_block_memory_);
  entries_ = reinterpret_cast<SMAEntry*>(header_ + 1);

  // Set each attribute to invalid.
  for (std::size_t i = 0; i < total_attributes_; ++i) {
    attribute_to_entry_.get()[i] = -1;
  }

  if (is_new_block) {
    header_->index_consistent = false;
  }

  // Set comparators to null.
  less_comparisons_.getInternalVectorMutable()->assign(kNumTypeIDs, nullptr);
  equal_comparisons_.getInternalVectorMutable()->assign(kNumTypeIDs, nullptr);
  add_operations_.getInternalVectorMutable()->assign(kNumTypeIDs, nullptr);

  // Iterate through each attribute which is being indexed.
  for (std::size_t indexed_attribute_num = 0;
       indexed_attribute_num < num_indexed_attributes_;
       ++indexed_attribute_num) {
    const attribute_id attribute = description_.indexed_attribute_ids(indexed_attribute_num);

    const CatalogAttribute *catalog_attribute
        = tuple_store_.getRelation().getAttributeById(attribute);
    CHECK(catalog_attribute != nullptr) << "Indexed attribute did not exist.";

    const Type &attribute_type = catalog_attribute->getType();
    SMAEntry *entry = entries_ + indexed_attribute_num;

    // Initialize the operator map.
    if (sma_internal::canApplySumToType(attribute_type.getTypeID())) {
      TypeID attr_typeid = attribute_type.getTypeID();
      TypeID attr_sum_typeid = sma_internal::getTypeForSum(attr_typeid);
      if (add_operations_.elementIsNullAt(attr_typeid)) {
        add_operations_.replaceElement(attr_typeid,
          BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd)
              .makeUncheckedBinaryOperatorForTypes(TypeFactory::GetType(attr_typeid),
                                                   TypeFactory::GetType(attr_sum_typeid)));
      }
    }

    // Initialize comparison map. Maps attribute type to comparison function.
    int attr_typeid = static_cast<int>(attribute_type.getTypeID());
    if (less_comparisons_.elementIsNullAt(attr_typeid)) {
      DCHECK(equal_comparisons_.elementIsNullAt(attr_typeid));
      less_comparisons_.replaceElement(attr_typeid,
          ComparisonFactory::GetComparison(ComparisonID::kLess)
              .makeUncheckedComparatorForTypes(attribute_type, attribute_type));
      equal_comparisons_.replaceElement(attr_typeid,
          ComparisonFactory::GetComparison(ComparisonID::kEqual)
              .makeUncheckedComparatorForTypes(attribute_type, attribute_type));
    }

    // Map the attribute's id to its entry's index.
    attribute_to_entry_.get()[attribute] = indexed_attribute_num;

    if (!header_->index_consistent) {
      resetEntry(entry, attribute, attribute_type);
    } else {
      // If the data held by min/max is out of line, we must retrieve it as we initialize.
      if (!TypedValue::RepresentedInline(entry->type_id)) {
        // If the data held by min/max is out of line (i.e. a variable
        // attribute which is stored on the heap or in a variable length storage
        // area), then we must store store our own copy of the typed value by
        // calling 'ensureNotReference'.
        if (entry->min_entry_ref.valid) {
          new (&entry->min_entry_ref.value) TypedValue();
          entry->min_entry_ref.value = tuple_store
              .getAttributeValueTyped(entry->min_entry_ref.entry_ref_tuple, entry->attribute);
          entry->min_entry_ref.value.ensureNotReference();
        }
        if (entry->max_entry_ref.valid) {
          new (&entry->max_entry_ref.value) TypedValue();
          entry->max_entry_ref.value = tuple_store
              .getAttributeValueTyped(entry->max_entry_ref.entry_ref_tuple, entry->attribute);
          entry->max_entry_ref.value.ensureNotReference();
        }
      }
    }
  }
  initialized_ = true;
}

SMAIndexSubBlock::~SMAIndexSubBlock() {
  // Any typed values which have out of line data must be cleared.
  freeOutOfLineData();
}

void SMAIndexSubBlock::resetEntry(SMAEntry *entry,
                                  const attribute_id attr_id,
                                  const Type &attribute_type) {
  entry->attribute = attr_id;
  entry->type_id = attribute_type.getTypeID();

  // Clearing min/max will free any out of line data.
  if (entry->min_entry_ref.valid) {
    entry->min_entry_ref.value.clear();
    entry->min_entry_ref.valid = false;
  }
  if (entry->max_entry_ref.valid) {
    entry->max_entry_ref.value.clear();
    entry->max_entry_ref.valid = false;
  }
  if (sma_internal::canApplySumToType(entry->type_id)) {
    sma_internal::initializeTypedValueForSum(entry);
  }
}

void SMAIndexSubBlock::resetAllEntries() {
  freeOutOfLineData();

  for (std::size_t indexed_attribute_num = 0;
       indexed_attribute_num < num_indexed_attributes_;
       ++indexed_attribute_num) {
    const attribute_id attribute = description_.indexed_attribute_ids(indexed_attribute_num);

    const Type &attribute_type = tuple_store_.getRelation().getAttributeById(attribute)->getType();
    resetEntry(entries_ + indexed_attribute_num, attribute, attribute_type);
  }
}

void SMAIndexSubBlock::freeOutOfLineData() {
  // For each entry, clear the min and max typed values (frees out of line data).
  for (std::size_t indexed_attribute_num = 0;
       indexed_attribute_num < num_indexed_attributes_;
       ++indexed_attribute_num) {
    SMAEntry &entry = entries_[indexed_attribute_num];
    if (!TypedValue::RepresentedInline(entry.type_id)) {
      if (entry.min_entry_ref.valid) {
        entry.min_entry_ref.value.clear();
      }
      if (entry.max_entry_ref.valid) {
        entry.max_entry_ref.value.clear();
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
  if (description.indexed_attribute_ids_size() == 0) {
    return false;
  }
  // Check that all key attributes exist.
  for (int indexed_attribute_num = 0;
       indexed_attribute_num < description.indexed_attribute_ids_size();
       ++indexed_attribute_num) {
    const attribute_id indexed_attribute_id = description.indexed_attribute_ids(indexed_attribute_num);
    if (!relation.hasAttributeWithId(indexed_attribute_id)) {
      return false;
    }
  }
  return true;
}

std::size_t SMAIndexSubBlock::EstimateBytesPerTuple(
    const CatalogRelationSchema &relation,
    const IndexSubBlockDescription &description) {
  // TODO(marc): Returning 1 almost always gives too much space. An enhancement
  // would be to inform the storage subblock that this index uses space w.r.t.
  // # of attributes being indexed, not tuples.
  return 1;
}

std::size_t SMAIndexSubBlock::EstimateBytesPerBlock(
    const CatalogRelationSchema &relation,
    const IndexSubBlockDescription &description) {
  return kZeroSize;
}

bool SMAIndexSubBlock::bulkAddEntries(const TupleIdSequence &tuples) {
  DCHECK(initialized_);
  if (header_->index_consistent) {
    for (const tuple_id &tuple : tuples) {
      addEntry(tuple);
    }
  }
  return true;  // There will always be space for the entry.
}

void SMAIndexSubBlock::bulkRemoveEntries(const TupleIdSequence &tuples) {
  header_->index_consistent = false;
}

bool SMAIndexSubBlock::addEntry(const tuple_id tuple) {
  DCHECK(initialized_);
  if (header_->index_consistent) {
    addTuple(tuple);
  }

  // There will always be space to insert the entry,
  // even if we did not call addTuple().
  return true;
}

void SMAIndexSubBlock::removeEntry(const tuple_id tuple) {
  header_->index_consistent = false;
}

bool SMAIndexSubBlock::rebuild() {
  resetAllEntries();
  header_->count_aggregate = 0;
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
  header_->index_consistent = true;
  return true;
}

void SMAIndexSubBlock::addTuple(tuple_id tuple) {
  for (std::size_t index = 0; index < num_indexed_attributes_; ++index) {
    SMAEntry *entry = entries_ + index;
    TypedValue tuple_value = tuple_store_.getAttributeValueTyped(tuple, entry->attribute);

    // TODO(marc) There should be a seperate COUNT tallied for attributes
    //            which can have null values. This is because the COUNT
    //            stored for the SMA currently counts all rows, regardless
    //            of NULL.

    // Ignore all nulls.
    if (tuple_value.isNull()) {
      continue;
    }

    if (sma_internal::canApplySumToType(entry->type_id)) {
      entry->sum_aggregate = add_operations_[entry->type_id].applyToTypedValues(tuple_value, entry->sum_aggregate);
    }

    // If the entries are valid, we can do comparison, otherwise, we skip the
    // comparison and set the value.
    if (!entry->min_entry_ref.valid) {
      memset(&entry->min_entry_ref.value, 0, sizeof(TypedValue));
      entry->min_entry_ref.value = tuple_value;
      entry->min_entry_ref.value.ensureNotReference();
      entry->min_entry_ref.entry_ref_tuple = tuple;
      entry->min_entry_ref.valid = true;
    } else {
      if (less_comparisons_[entry->type_id].compareTypedValues(tuple_value, entry->min_entry_ref.value)) {
        entry->min_entry_ref.value = tuple_value;
        entry->min_entry_ref.value.ensureNotReference();
        entry->min_entry_ref.entry_ref_tuple = tuple;
      }
    }

    if (!entry->max_entry_ref.valid) {
      memset(&entry->max_entry_ref.value, 0, sizeof(TypedValue));
      entry->max_entry_ref.value = tuple_value;
      entry->max_entry_ref.value.ensureNotReference();
      entry->max_entry_ref.entry_ref_tuple = tuple;
      entry->max_entry_ref.valid = true;
    } else {
      if (less_comparisons_[entry->type_id].compareTypedValues(entry->max_entry_ref.value, tuple_value)) {
        entry->max_entry_ref.value = tuple_value;
        entry->max_entry_ref.value.ensureNotReference();
        entry->max_entry_ref.entry_ref_tuple = tuple;
      }
    }
  }

  header_->count_aggregate++;
}

Selectivity SMAIndexSubBlock::getSelectivityForPredicate(const ComparisonPredicate &predicate) const {
  if (!header_->index_consistent) {
    return Selectivity::kUnknown;
  }

  std::unique_ptr<SMAPredicate> sma_predicate(SMAPredicate::ExtractSMAPredicate(predicate));
  // The predicate did not contain a static value to compare against.
  if (!sma_predicate) {
    return Selectivity::kUnknown;
  }

  const SMAEntry *entry = getEntryChecked(sma_predicate->attribute);

  // The attribute wasn't indexed.
  if (entry == nullptr || !entry->min_entry_ref.valid || !entry->max_entry_ref.valid) {
    return Selectivity::kUnknown;
  }

  // Check that the type of the comparison is the same or can be coerced the
  // type of the underlying attribute.
  if (sma_predicate->literal.getTypeID() != entry->type_id) {
    // Try to coerce the literal type to the entry type.
    // It may have to account for variable length attributes.
    int literal_length = -1;
    if (TypeFactory::TypeRequiresLengthParameter(sma_predicate->literal.getTypeID())) {
      literal_length = sma_predicate->literal.getAsciiStringLength();
    }

    const Type &literal_type = literal_length == -1 ?
        TypeFactory::GetType(sma_predicate->literal.getTypeID(), false) :
        TypeFactory::GetType(sma_predicate->literal.getTypeID(), static_cast<std::size_t>(literal_length), false);
    const Type &attribute_type = literal_length == -1 ?
        TypeFactory::GetType(entry->type_id, false) :
        TypeFactory::GetType(entry->type_id, static_cast<std::size_t>(literal_length), false);
    if (attribute_type.isSafelyCoercibleFrom(literal_type)) {
      // Convert the literal's type inside the predicate.
      SMAPredicate *replacement = new SMAPredicate(
          sma_predicate->attribute,
          sma_predicate->comparison,
          attribute_type.coerceValue(sma_predicate->literal, literal_type));
      sma_predicate.reset(replacement);
    } else {
      // The literal type cannot be converted, so do not evaluate with the SMA.
      return Selectivity::kUnknown;
    }
  }

  return sma_internal::getSelectivity(
    sma_predicate->literal,
    sma_predicate->comparison,
    entry->min_entry_ref.value,
    entry->max_entry_ref.value,
    &less_comparisons_[entry->type_id],
    &equal_comparisons_[entry->type_id]);
}

predicate_cost_t SMAIndexSubBlock::estimatePredicateEvaluationCost(
    const ComparisonPredicate &predicate) const {
  DCHECK(initialized_);

  // Check that at least one of the operands has a static value.
  Selectivity selectivity = getSelectivityForPredicate(predicate);
  if (selectivity == Selectivity::kAll || selectivity == Selectivity::kNone) {
    return predicate_cost::kConstantTime;
  }
  return predicate_cost::kInfinite;
}

TupleIdSequence* SMAIndexSubBlock::getMatchesForPredicate(
    const ComparisonPredicate &predicate,
    const TupleIdSequence *filter) const {
  if (filter != nullptr) {
    LOG(FATAL) << "SMAIndex cannot evaluate filters.";
  }

  Selectivity selectivity = getSelectivityForPredicate(predicate);
  if (selectivity == Selectivity::kAll) {
    TupleIdSequence* tidseq = new TupleIdSequence(tuple_store_.numTuples());

    // Set all existing tuples to true, selected.
    if (tuple_store_.isPacked()) {
      tidseq->setRange(0, tuple_store_.numTuples(), true);
    } else {
      for (tuple_id tid = 0; tid <= tuple_store_.getMaxTupleID(); ++tid) {
        if (tuple_store_.hasTupleWithID(tid)) {
          tidseq->set(tid, true);
        }
      }
    }
    return tidseq;
  } else if (selectivity == Selectivity::kNone) {
    // A new tuple ID sequence is initialized to false for all values.
    return new TupleIdSequence(tuple_store_.numTuples());
  }
  LOG(FATAL) << "SMAIndex failed to evaluate predicate. The SMA should not have been used";
  return nullptr;
}

bool SMAIndexSubBlock::hasEntryForAttribute(attribute_id attribute) const {
  return total_attributes_ > static_cast<std::size_t>(attribute) &&
      attribute_to_entry_.get()[attribute] != -1;
}

}  // namespace quickstep
