/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "storage/BasicColumnStoreTupleStorageSubBlock.hpp"

#include <cstddef>
#include <cstring>
#include <unordered_map>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/predicate/ComparisonPredicate.hpp"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/predicate/PredicateCost.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "storage/BasicColumnStoreValueAccessor.hpp"
#include "storage/ColumnStoreUtil.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageErrors.hpp"
#include "storage/SubBlockTypeRegistry.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/Tuple.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "types/operations/comparisons/ComparisonUtil.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrMap.hpp"
#include "utility/PtrVector.hpp"
#include "utility/ScopedBuffer.hpp"

using std::memcpy;
using std::memmove;
using std::size_t;
using std::vector;

using quickstep::column_store_util::ColumnStripeIterator;
using quickstep::column_store_util::SortColumnPredicateEvaluator;

namespace quickstep {

QUICKSTEP_REGISTER_TUPLE_STORE(BasicColumnStoreTupleStorageSubBlock, BASIC_COLUMN_STORE);

// Hide this helper in an anonymous namespace:
namespace {

class SortColumnValueReference {
 public:
  SortColumnValueReference(const void *value, const tuple_id tuple)
      : value_(value), tuple_(tuple) {
  }

  explicit SortColumnValueReference(const void *value)
      : value_(value), tuple_(-1) {
  }

  inline const void* getDataPtr() const {
    return value_;
  }

  inline tuple_id getTupleID() const {
    return tuple_;
  }

 private:
  const void *value_;
  tuple_id tuple_;
};

}  // anonymous namespace


BasicColumnStoreTupleStorageSubBlock::BasicColumnStoreTupleStorageSubBlock(
    const CatalogRelationSchema &relation,
    const TupleStorageSubBlockDescription &description,
    const bool new_block,
    void *sub_block_memory,
    const std::size_t sub_block_memory_size)
    : TupleStorageSubBlock(relation,
                           description,
                           new_block,
                           sub_block_memory,
                           sub_block_memory_size),
      sorted_(true),
      header_(static_cast<BasicColumnStoreHeader*>(sub_block_memory)) {
  if (!DescriptionIsValid(relation_, description_)) {
    FATAL_ERROR("Attempted to construct a BasicColumnStoreTupleStorageSubBlock from an invalid description.");
  }

  sort_column_id_ = description_.GetExtension(BasicColumnStoreTupleStorageSubBlockDescription::sort_attribute_id);
  sort_column_type_ = &(relation_.getAttributeById(sort_column_id_)->getType());

  if (sub_block_memory_size < sizeof(BasicColumnStoreHeader)) {
    throw BlockMemoryTooSmall("BasicColumnStoreTupleStorageSubBlock", sub_block_memory_size_);
  }

  // Determine the amount of tuples this sub-block can hold. Compute on the
  // order of bits to account for null bitmap storage.
  max_tuples_ = ((sub_block_memory_size_ - sizeof(BasicColumnStoreHeader)) << 3)
                / ((relation_.getFixedByteLength() << 3) + relation_.numNullableAttributes());
  if (max_tuples_ == 0) {
    throw BlockMemoryTooSmall("BasicColumnStoreTupleStorageSubBlock", sub_block_memory_size_);
  }

  // BitVector's storage requirements "round up" to sizeof(size_t), so now redo
  // the calculation accurately.
  std::size_t bitmap_required_bytes = BitVector<false>::BytesNeeded(max_tuples_);
  max_tuples_ = (sub_block_memory_size_
                     - sizeof(BasicColumnStoreHeader)
                     - (relation_.numNullableAttributes() * bitmap_required_bytes))
                / relation_.getFixedByteLength();
  if (max_tuples_ == 0) {
    throw BlockMemoryTooSmall("BasicColumnStoreTupleStorageSubBlock", sub_block_memory_size_);
  }
  bitmap_required_bytes = BitVector<false>::BytesNeeded(max_tuples_);

  // Allocate memory for this sub-block's structures, starting with the header.
  char *memory_location = static_cast<char*>(sub_block_memory_) + sizeof(BasicColumnStoreHeader);

  // Per-column NULL bitmaps.
  for (attribute_id attr_id = 0;
       attr_id <= relation_.getMaxAttributeId();
       ++attr_id) {
    if (relation_.hasAttributeWithId(attr_id)
        && relation_.getAttributeById(attr_id)->getType().isNullable()) {
      column_null_bitmaps_.push_back(
          new BitVector<false>(memory_location, max_tuples_));
      if (new_block) {
        column_null_bitmaps_.back().clear();
      }
      memory_location += bitmap_required_bytes;
    } else {
      column_null_bitmaps_.push_back(nullptr);
    }
  }

  // Column stripes.
  column_stripes_.resize(relation_.getMaxAttributeId() +  1, nullptr);
  for (const CatalogAttribute &attr : relation_) {
    column_stripes_[attr.getID()] = memory_location;
    memory_location += max_tuples_ * attr.getType().maximumByteLength();
  }

  DEBUG_ASSERT(memory_location
               <= static_cast<const char*>(sub_block_memory_) + sub_block_memory_size_);

  if (new_block) {
    header_->num_tuples = 0;
    header_->nulls_in_sort_column = 0;
  }
}

bool BasicColumnStoreTupleStorageSubBlock::DescriptionIsValid(
    const CatalogRelationSchema &relation,
    const TupleStorageSubBlockDescription &description) {
  // Make sure description is initialized and specifies BasicColumnStore.
  if (!description.IsInitialized()) {
    return false;
  }
  if (description.sub_block_type() != TupleStorageSubBlockDescription::BASIC_COLUMN_STORE) {
    return false;
  }
  // Make sure a sort_attribute_id is specified.
  if (!description.HasExtension(BasicColumnStoreTupleStorageSubBlockDescription::sort_attribute_id)) {
    return false;
  }

  // Make sure relation is not variable-length.
  if (relation.isVariableLength()) {
    return false;
  }

  // Check that the specified sort attribute exists and can be ordered by LessComparison.
  attribute_id sort_attribute_id = description.GetExtension(
      BasicColumnStoreTupleStorageSubBlockDescription::sort_attribute_id);
  if (!relation.hasAttributeWithId(sort_attribute_id)) {
    return false;
  }
  const Type &sort_attribute_type = relation.getAttributeById(sort_attribute_id)->getType();
  if (!ComparisonFactory::GetComparison(ComparisonID::kLess).canCompareTypes(sort_attribute_type,
                                                                             sort_attribute_type)) {
    return false;
  }

  return true;
}

std::size_t BasicColumnStoreTupleStorageSubBlock::EstimateBytesPerTuple(
    const CatalogRelationSchema &relation,
    const TupleStorageSubBlockDescription &description) {
  DEBUG_ASSERT(DescriptionIsValid(relation, description));

  // NOTE(chasseur): We round-up the number of bytes needed in the NULL bitmaps
  // to avoid estimating 0 bytes needed for a relation with less than 8
  // attributes which are all NullType.
  return relation.getFixedByteLength()
         + ((relation.numNullableAttributes() + 7) >> 3);
}

TupleStorageSubBlock::InsertResult BasicColumnStoreTupleStorageSubBlock::insertTuple(
    const Tuple &tuple) {
#ifdef QUICKSTEP_DEBUG
  paranoidInsertTypeCheck(tuple);
#endif
  if (!hasSpaceToInsert(1)) {
    return InsertResult(-1, false);
  }

  tuple_id insert_position = header_->num_tuples;
  // If sort column is NULL, skip the search and put the new tuple at the end
  // of everything else.
  if (!tuple.getAttributeValue(sort_column_id_).isNull()) {
    // Binary search for the appropriate insert location.
    ColumnStripeIterator begin_it(column_stripes_[sort_column_id_],
                                  relation_.getAttributeById(sort_column_id_)->getType().maximumByteLength(),
                                  0);
    ColumnStripeIterator end_it(column_stripes_[sort_column_id_],
                                relation_.getAttributeById(sort_column_id_)->getType().maximumByteLength(),
                                header_->num_tuples - header_->nulls_in_sort_column);
    insert_position = GetBoundForUntypedValue<ColumnStripeIterator, UpperBoundFunctor>(
        *sort_column_type_,
        begin_it,
        end_it,
        tuple.getAttributeValue(sort_column_id_).getDataPtr()).getTuplePosition();
  }

  InsertResult retval(insert_position, insert_position != header_->num_tuples);
  insertTupleAtPosition(tuple, insert_position);

  return retval;
}

bool BasicColumnStoreTupleStorageSubBlock::insertTupleInBatch(const Tuple &tuple) {
#ifdef QUICKSTEP_DEBUG
  paranoidInsertTypeCheck(tuple);
#endif
  if (!hasSpaceToInsert(1)) {
    return false;
  }

  insertTupleAtPosition(tuple, header_->num_tuples);
  sorted_ = false;
  return true;
}

tuple_id BasicColumnStoreTupleStorageSubBlock::bulkInsertTuples(ValueAccessor *accessor) {
  const tuple_id original_num_tuples = header_->num_tuples;

  InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    if (relation_.hasNullableAttributes()) {
      while (this->hasSpaceToInsert(1) && accessor->next()) {
        attribute_id accessor_attr_id = 0;
        // TODO(chasseur): Column-wise copy is probably preferable to
        // tuple-at-a-time, but will require some API changes.
        for (CatalogRelationSchema::const_iterator attr_it = relation_.begin();
             attr_it != relation_.end();
             ++attr_it) {
          const attribute_id attr_id = attr_it->getID();
          const std::size_t attr_size = attr_it->getType().maximumByteLength();
          if (attr_it->getType().isNullable()) {
            const void *attr_value
                = accessor->template getUntypedValue<true>(accessor_attr_id);
            if (attr_value == nullptr) {
              column_null_bitmaps_[attr_id].setBit(header_->num_tuples, true);
            } else {
              memcpy(static_cast<char*>(column_stripes_[attr_id])
                         + header_->num_tuples * attr_size,
                     attr_value,
                     attr_size);
            }
          } else {
            memcpy(static_cast<char*>(column_stripes_[attr_id])
                       + header_->num_tuples * attr_size,
                   accessor->template getUntypedValue<false>(accessor_attr_id),
                   attr_size);
          }
          ++accessor_attr_id;
        }
        ++(header_->num_tuples);
      }
    } else {
      while (this->hasSpaceToInsert(1) && accessor->next()) {
        attribute_id accessor_attr_id = 0;
        for (CatalogRelationSchema::const_iterator attr_it = relation_.begin();
             attr_it != relation_.end();
             ++attr_it) {
          const std::size_t attr_size = attr_it->getType().maximumByteLength();
          memcpy(static_cast<char*>(column_stripes_[attr_it->getID()])
                     + header_->num_tuples * attr_size,
                 accessor->template getUntypedValue<false>(accessor_attr_id),
                 attr_size);
          ++accessor_attr_id;
        }
        ++(header_->num_tuples);
      }
    }
  });

  const tuple_id num_inserted = header_->num_tuples - original_num_tuples;
  sorted_ = sorted_ && (num_inserted == 0);
  return num_inserted;
}

tuple_id BasicColumnStoreTupleStorageSubBlock::bulkInsertTuplesWithRemappedAttributes(
    const std::vector<attribute_id> &attribute_map,
    ValueAccessor *accessor) {
  DEBUG_ASSERT(attribute_map.size() == relation_.size());
  const tuple_id original_num_tuples = header_->num_tuples;

  InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    if (relation_.hasNullableAttributes()) {
      while (this->hasSpaceToInsert(1) && accessor->next()) {
        std::vector<attribute_id>::const_iterator attribute_map_it = attribute_map.begin();
        // TODO(chasseur): Column-wise copy is probably preferable to
        // tuple-at-a-time, but will require some API changes.
        for (CatalogRelationSchema::const_iterator attr_it = relation_.begin();
             attr_it != relation_.end();
             ++attr_it) {
          const attribute_id attr_id = attr_it->getID();
          const std::size_t attr_size = attr_it->getType().maximumByteLength();
          if (attr_it->getType().isNullable()) {
            const void *attr_value
                = accessor->template getUntypedValue<true>(*attribute_map_it);
            if (attr_value == nullptr) {
              column_null_bitmaps_[attr_id].setBit(header_->num_tuples, true);
            } else {
              memcpy(static_cast<char*>(column_stripes_[attr_id])
                         + header_->num_tuples * attr_size,
                     attr_value,
                     attr_size);
            }
          } else {
            memcpy(static_cast<char*>(column_stripes_[attr_id])
                       + header_->num_tuples * attr_size,
                   accessor->template getUntypedValue<false>(*attribute_map_it),
                   attr_size);
          }
          ++attribute_map_it;
        }
        ++(header_->num_tuples);
      }
    } else {
      while (this->hasSpaceToInsert(1) && accessor->next()) {
        std::vector<attribute_id>::const_iterator attribute_map_it = attribute_map.begin();
        for (CatalogRelationSchema::const_iterator attr_it = relation_.begin();
             attr_it != relation_.end();
             ++attr_it) {
          const std::size_t attr_size = attr_it->getType().maximumByteLength();
          memcpy(static_cast<char*>(column_stripes_[attr_it->getID()])
                     + header_->num_tuples * attr_size,
                 accessor->template getUntypedValue<false>(*attribute_map_it),
                 attr_size);
          ++attribute_map_it;
        }
        ++(header_->num_tuples);
      }
    }
  });

  const tuple_id num_inserted = header_->num_tuples - original_num_tuples;
  sorted_ = sorted_ && (num_inserted == 0);
  return num_inserted;
}

const void* BasicColumnStoreTupleStorageSubBlock::getAttributeValue(
    const tuple_id tuple,
    const attribute_id attr) const {
  DEBUG_ASSERT(hasTupleWithID(tuple));
  DEBUG_ASSERT(relation_.hasAttributeWithId(attr));

  if ((!column_null_bitmaps_.elementIsNull(attr))
      && column_null_bitmaps_[attr].getBit(tuple)) {
    return nullptr;
  }

  return static_cast<const char*>(column_stripes_[attr])
         + (tuple * relation_.getAttributeById(attr)->getType().maximumByteLength());
}

TypedValue BasicColumnStoreTupleStorageSubBlock::getAttributeValueTyped(
    const tuple_id tuple,
    const attribute_id attr) const {
  const Type &attr_type = relation_.getAttributeById(attr)->getType();
  const void *untyped_value = getAttributeValue(tuple, attr);
  return (untyped_value == nullptr)
      ? attr_type.makeNullValue()
      : attr_type.makeValue(untyped_value, attr_type.maximumByteLength());
}

ValueAccessor* BasicColumnStoreTupleStorageSubBlock::createValueAccessor(
    const TupleIdSequence *sequence) const {
  BasicColumnStoreValueAccessor *base_accessor
      = new BasicColumnStoreValueAccessor(relation_,
                                          relation_,
                                          header_->num_tuples,
                                          column_stripes_,
                                          column_null_bitmaps_);
  if (sequence == nullptr) {
    return base_accessor;
  } else {
    return new TupleIdSequenceAdapterValueAccessor<BasicColumnStoreValueAccessor>(
        base_accessor,
        *sequence);
  }
}

bool BasicColumnStoreTupleStorageSubBlock::canSetAttributeValuesInPlaceTyped(
    const tuple_id tuple,
    const std::unordered_map<attribute_id, TypedValue> &new_values) const {
  DEBUG_ASSERT(hasTupleWithID(tuple));
  for (std::unordered_map<attribute_id, TypedValue>::const_iterator it
           = new_values.begin();
       it != new_values.end();
       ++it) {
    DEBUG_ASSERT(relation_.hasAttributeWithId(it->first));
    // TODO(chasseur): Could check if the new value for sort column would
    // remain in the same position in the stripe.
    if (it->first == sort_column_id_) {
      return false;
    }
  }
  return true;
}

void BasicColumnStoreTupleStorageSubBlock::setAttributeValueInPlaceTyped(
    const tuple_id tuple,
    const attribute_id attr,
    const TypedValue &value) {
  DEBUG_ASSERT(attr != sort_column_id_);

  const Type &attr_type = relation_.getAttributeById(attr)->getType();
  void *value_position = static_cast<char*>(column_stripes_[attr])
                         + tuple * attr_type.maximumByteLength();
  if (!column_null_bitmaps_.elementIsNull(attr)) {
    if (value.isNull()) {
      column_null_bitmaps_[attr].setBit(tuple, true);
      return;
    } else {
      column_null_bitmaps_[attr].setBit(tuple, false);
    }
  }

  value.copyInto(value_position);
}

bool BasicColumnStoreTupleStorageSubBlock::deleteTuple(const tuple_id tuple) {
  DEBUG_ASSERT(hasTupleWithID(tuple));

  if (!column_null_bitmaps_.elementIsNull(sort_column_id_)) {
    if (column_null_bitmaps_[sort_column_id_].getBit(tuple)) {
      --(header_->nulls_in_sort_column);
    }
  }

  if (tuple == header_->num_tuples - 1) {
    // If deleting the last tuple, simply truncate.
    --(header_->num_tuples);

    // Clear any null bits for the tuple.
    for (PtrVector<BitVector<false>, true>::iterator it = column_null_bitmaps_.begin();
         it != column_null_bitmaps_.end();
         ++it) {
      if (!it.isNull()) {
        it->setBit(header_->num_tuples, false);
      }
    }

    return false;
  } else {
    // Pack each column stripe.
    shiftTuples(tuple, tuple + 1, header_->num_tuples - tuple - 1);
    shiftNullBitmaps(tuple, -1);
    --(header_->num_tuples);
    return true;
  }
}

bool BasicColumnStoreTupleStorageSubBlock::bulkDeleteTuples(TupleIdSequence *tuples) {
  if (tuples->empty()) {
    // Nothing to do.
    return false;
  }

  const tuple_id front = tuples->front();
  const tuple_id back = tuples->back();
  const tuple_id num_tuples = tuples->numTuples();

  if ((back == header_->num_tuples - 1)
       && (back - front == num_tuples - 1)) {
    // Just truncate the back.
    header_->num_tuples = front;

    // Clear any null bits.
    for (PtrVector<BitVector<false>, true>::iterator it = column_null_bitmaps_.begin();
         it != column_null_bitmaps_.end();
         ++it) {
      if (!it.isNull()) {
        it->setBitRange(header_->num_tuples, num_tuples, false);
      }
    }

    return false;
  }

  // Pack the non-deleted tuples.
  tuple_id dest_position = front;
  tuple_id src_tuple = dest_position;
  TupleIdSequence::const_iterator it = tuples->begin();
  tuple_id tail_shifted_distance = 0;
  for (tuple_id current_id = tuples->front();
       current_id < header_->num_tuples;
       ++current_id, ++src_tuple) {
    if (current_id == *it) {
      // Don't copy a deleted tuple.
      shiftNullBitmaps(*it - tail_shifted_distance, -1);
      ++tail_shifted_distance;
      ++it;
      if (it == tuples->end()) {
        // No more to delete, so copy all the remaining tuples in one go.
        shiftTuples(dest_position,
                    src_tuple + 1,
                    header_->num_tuples - back - 1);
        break;
      }
    } else {
      // Shift the next tuple forward.
      shiftTuples(dest_position, src_tuple, 1);
      ++dest_position;
    }
  }

  header_->num_tuples -= static_cast<tuple_id>(num_tuples);

  return true;
}

predicate_cost_t BasicColumnStoreTupleStorageSubBlock::estimatePredicateEvaluationCost(
    const ComparisonPredicate &predicate) const {
  if (predicate.isAttributeLiteralComparisonPredicate()) {
    std::pair<bool, attribute_id> comparison_attr = predicate.getAttributeFromAttributeLiteralComparison();
    if (comparison_attr.second == sort_column_id_) {
      return predicate_cost::kBinarySearch;
    }
  }
  return predicate_cost::kColumnScan;
}

TupleIdSequence* BasicColumnStoreTupleStorageSubBlock::getMatchesForPredicate(
    const ComparisonPredicate &predicate,
    const TupleIdSequence *filter) const {
  TupleIdSequence *matches = SortColumnPredicateEvaluator::EvaluatePredicateForUncompressedSortColumn(
      predicate,
      relation_,
      sort_column_id_,
      column_stripes_[sort_column_id_],
      header_->num_tuples - header_->nulls_in_sort_column);

  if (matches == nullptr) {
    FATAL_ERROR("Called BasicColumnStoreTupleStorageSubBlock::getMatchesForPredicate() "
                "with a predicate that can only be evaluated with a scan.");
  } else {
    if (filter != nullptr) {
      matches->intersectWith(*filter);
    }
    return matches;
  }
}

void BasicColumnStoreTupleStorageSubBlock::insertTupleAtPosition(
    const Tuple &tuple,
    const tuple_id position) {
  DEBUG_ASSERT(hasSpaceToInsert(1));
  DEBUG_ASSERT(position >= 0);
  DEBUG_ASSERT(position < max_tuples_);

  if (position != header_->num_tuples) {
    // If not inserting in the last position, shift subsequent tuples back.
    shiftTuples(position + 1, position, header_->num_tuples - position);
    shiftNullBitmaps(position, 1);
  }

  // Copy attribute values into place in the column stripes.
  Tuple::const_iterator value_it = tuple.begin();
  CatalogRelationSchema::const_iterator attr_it = relation_.begin();

  while (value_it != tuple.end()) {
    const attribute_id attr_id = attr_it->getID();
    const Type &attr_type = attr_it->getType();
    if (value_it->isNull()) {
      column_null_bitmaps_[attr_id].setBit(position, true);
      if (attr_id == sort_column_id_) {
        ++(header_->nulls_in_sort_column);

        // Copy in a special value that always compares last.
        GetLastValueForType(attr_type).copyInto(
            static_cast<char*>(column_stripes_[attr_id])
                + position * attr_type.maximumByteLength());
      }
    } else {
      value_it->copyInto(static_cast<char*>(column_stripes_[attr_id])
                         + position * attr_type.maximumByteLength());
    }

    ++value_it;
    ++attr_it;
  }

  ++(header_->num_tuples);
}

void BasicColumnStoreTupleStorageSubBlock::shiftTuples(
    const tuple_id dest_position,
    const tuple_id src_tuple,
    const tuple_id num_tuples) {
  for (CatalogRelationSchema::const_iterator attr_it = relation_.begin();
       attr_it != relation_.end();
       ++attr_it) {
    size_t attr_length = attr_it->getType().maximumByteLength();
    memmove(static_cast<char*>(column_stripes_[attr_it->getID()]) + dest_position * attr_length,
            static_cast<const char*>(column_stripes_[attr_it->getID()]) + src_tuple * attr_length,
            attr_length * num_tuples);
  }
}

void BasicColumnStoreTupleStorageSubBlock::shiftNullBitmaps(
    const tuple_id from_position,
    const tuple_id distance) {
  if (relation_.hasNullableAttributes()) {
    for (PtrVector<BitVector<false>, true>::size_type idx = 0;
         idx < column_null_bitmaps_.size();
         ++idx) {
      if (!column_null_bitmaps_.elementIsNull(idx)) {
        if (distance < 0) {
          column_null_bitmaps_[idx].shiftTailForward(from_position, -distance);
        } else {
          column_null_bitmaps_[idx].shiftTailBackward(from_position, distance);
        }
      }
    }
  }
}

// TODO(chasseur): This implementation uses out-of-band memory up to the
// total size of tuples contained in this sub-block. It could be done with
// less memory, although the implementation would be more complex.
bool BasicColumnStoreTupleStorageSubBlock::rebuildInternal() {
  const tuple_id num_tuples = header_->num_tuples;
  // Immediately return if 1 or 0 tuples.
  if (num_tuples <= 1) {
    sorted_ = true;
    return false;
  }

  // Determine the properly-sorted order of tuples.
  vector<SortColumnValueReference> sort_column_values;
  const char *sort_value_position
      = static_cast<const char*>(column_stripes_[sort_column_id_]);
  const std::size_t sort_value_size = sort_column_type_->maximumByteLength();
  for (tuple_id tid = 0; tid < num_tuples; ++tid) {
    // NOTE(chasseur): The insert methods put a special last-comparing
    // substitute value for NULLs into the sorted column stripe.
    sort_column_values.emplace_back(sort_value_position, tid);
    sort_value_position += sort_value_size;
  }
  SortWrappedValues<SortColumnValueReference, vector<SortColumnValueReference>::iterator>(
      *sort_column_type_,
      sort_column_values.begin(),
      sort_column_values.end());

  if (header_->nulls_in_sort_column > 0) {
    // If any NULL values exist in the sort column, make sure they are ordered
    // after all the "real" values which compare as the same.
    vector<SortColumnValueReference>::iterator tail_it =
        GetBoundForWrappedValues<SortColumnValueReference,
                                 vector<SortColumnValueReference>::iterator,
                                 LowerBoundFunctor>(
            *sort_column_type_,
            sort_column_values.begin(),
            sort_column_values.end(),
            GetLastValueForType(*sort_column_type_).getDataPtr());

    if (sort_column_values.end() - tail_it > header_->nulls_in_sort_column) {
      vector<SortColumnValueReference> non_nulls;
      vector<SortColumnValueReference> true_nulls;
      for (vector<SortColumnValueReference>::const_iterator tail_sort_it = tail_it;
           tail_sort_it != sort_column_values.end();
           ++tail_sort_it) {
        if (column_null_bitmaps_[sort_column_id_].getBit(tail_sort_it->getTupleID())) {
          true_nulls.push_back(*tail_sort_it);
        } else {
          non_nulls.push_back(*tail_sort_it);
        }
      }

      sort_column_values.erase(tail_it, sort_column_values.end());
      sort_column_values.insert(sort_column_values.end(),
                                non_nulls.begin(),
                                non_nulls.end());
      sort_column_values.insert(sort_column_values.end(),
                                true_nulls.begin(),
                                true_nulls.end());
    }
  }

  // If a prefix of the total order of tuples is already sorted, don't bother
  // copying it around.
  tuple_id ordered_prefix_tuples = 0;
  for (vector<SortColumnValueReference>::const_iterator it = sort_column_values.begin();
       it != sort_column_values.end();
       ++it) {
    if (it->getTupleID() != ordered_prefix_tuples) {
      break;
    }
    ++ordered_prefix_tuples;
  }

  if (ordered_prefix_tuples == num_tuples) {
    // Already sorted.
    sorted_ = true;
    return false;
  }

  // Allocate buffers for each resorted column stripe which can hold exactly as
  // many values as needed.
  PtrVector<ScopedBuffer, true> column_stripe_buffers;
  PtrVector<ScopedBuffer, true> null_bitmap_buffers;
  PtrVector<BitVector<false>, true> new_null_bitmaps;

  const std::size_t bitmap_required_bytes = BitVector<false>::BytesNeeded(max_tuples_);
  char *bitmap_location = static_cast<char*>(sub_block_memory_)
                          + sizeof(BasicColumnStoreHeader);

  for (attribute_id stripe_id = 0; stripe_id <= relation_.getMaxAttributeId(); ++stripe_id) {
    if (relation_.hasAttributeWithId(stripe_id)) {
      column_stripe_buffers.push_back(
          new ScopedBuffer((num_tuples - ordered_prefix_tuples)
                           * relation_.getAttributeById(stripe_id)->getType().maximumByteLength()));
      if (column_null_bitmaps_.elementIsNull(stripe_id)) {
        null_bitmap_buffers.push_back(nullptr);
        new_null_bitmaps.push_back(nullptr);
      } else {
        // Make a copy of the null bitmap for this column.
        null_bitmap_buffers.push_back(new ScopedBuffer(bitmap_required_bytes));
        new_null_bitmaps.push_back(new BitVector<false>(null_bitmap_buffers.back().get(), max_tuples_));
        bitmap_location += bitmap_required_bytes;

        // Clear out the tail, which will be reorganized.
        if (ordered_prefix_tuples > 0) {
          memcpy(null_bitmap_buffers.back().get(), bitmap_location, bitmap_required_bytes);
          new_null_bitmaps.back().setBitRange(ordered_prefix_tuples,
                                              num_tuples - ordered_prefix_tuples,
                                              false);
        } else {
          new_null_bitmaps.back().clear();
        }
      }
    } else {
      column_stripe_buffers.push_back(nullptr);
      null_bitmap_buffers.push_back(nullptr);
      new_null_bitmaps.push_back(nullptr);
    }
  }

  // Copy attribute values into the column stripe buffers in properly-sorted
  // order.
  for (CatalogRelationSchema::const_iterator attr_it = relation_.begin();
       attr_it != relation_.end();
       ++attr_it) {
    const size_t attr_length = attr_it->getType().maximumByteLength();
    const attribute_id attr_id = attr_it->getID();
    for (tuple_id unordered_tuple_num = 0;
         unordered_tuple_num < num_tuples - ordered_prefix_tuples;
         ++unordered_tuple_num) {
      memcpy(static_cast<char*>(column_stripe_buffers[attr_id].get())
                 + unordered_tuple_num * attr_length,
             static_cast<char*>(column_stripes_[attr_id]) +
                 sort_column_values[ordered_prefix_tuples + unordered_tuple_num].getTupleID() * attr_length,
             attr_length);
    }

    if (!new_null_bitmaps.elementIsNull(attr_id)) {
      for (tuple_id tuple_num = ordered_prefix_tuples;
           tuple_num < num_tuples;
           ++tuple_num) {
        new_null_bitmaps[attr_id].setBit(
            tuple_num,
            column_null_bitmaps_[attr_id].getBit(sort_column_values[tuple_num].getTupleID()));
      }
    }
  }

  // Overwrite the unsorted tails of the column stripes in this block with the
  // sorted values from the buffers. Also copy back any null bitmaps.
  bitmap_location = static_cast<char*>(sub_block_memory_)
                    + sizeof(BasicColumnStoreHeader);
  for (CatalogRelationSchema::const_iterator attr_it = relation_.begin();
       attr_it != relation_.end();
       ++attr_it) {
    size_t attr_length = attr_it->getType().maximumByteLength();
    memcpy(static_cast<char*>(column_stripes_[attr_it->getID()]) + ordered_prefix_tuples * attr_length,
           column_stripe_buffers[attr_it->getID()].get(),
           (num_tuples - ordered_prefix_tuples) * attr_length);

    if (!null_bitmap_buffers.elementIsNull(attr_it->getID())) {
      std::memcpy(bitmap_location,
                  null_bitmap_buffers[attr_it->getID()].get(),
                  bitmap_required_bytes);
      bitmap_location += bitmap_required_bytes;
    }
  }

  sorted_ = true;
  return true;
}

}  // namespace quickstep
