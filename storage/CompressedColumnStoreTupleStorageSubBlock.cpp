/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#include "storage/CompressedColumnStoreTupleStorageSubBlock.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <limits>
#include <unordered_set>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "compression/CompressionDictionaryLite.hpp"
#include "expressions/predicate/ComparisonPredicate.hpp"
#include "expressions/predicate/PredicateCost.hpp"
#include "storage/ColumnStoreUtil.hpp"
#include "storage/CompressedColumnStoreValueAccessor.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/SubBlockTypeRegistry.hpp"
#include "storage/TupleIdSequence.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"

using std::equal_to;
using std::greater;
using std::less_equal;
using std::lower_bound;
using std::memmove;
using std::not_equal_to;
using std::numeric_limits;
using std::pair;
using std::size_t;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;

using quickstep::column_store_util::SortColumnPredicateEvaluator;

namespace quickstep {

QUICKSTEP_REGISTER_TUPLE_STORE(CompressedColumnStoreTupleStorageSubBlock, COMPRESSED_COLUMN_STORE);

CompressedColumnStoreTupleStorageSubBlock::CompressedColumnStoreTupleStorageSubBlock(
    const CatalogRelationSchema &relation,
    const TupleStorageSubBlockDescription &description,
    const bool new_block,
    void *sub_block_memory,
    const std::size_t sub_block_memory_size)
    : CompressedTupleStorageSubBlock(relation,
                                     description,
                                     new_block,
                                     sub_block_memory,
                                     sub_block_memory_size),
      uncompressed_nulls_in_sort_column_(0) {
  if (!DescriptionIsValid(relation_, description_)) {
    FATAL_ERROR("Attempted to construct a CompressedColumnStoreTupleStorageSubBlock "
                "from an invalid description.");
  }

  sort_column_id_ = description_.GetExtension(
      CompressedColumnStoreTupleStorageSubBlockDescription::sort_attribute_id);

  if ((!new_block) && (*static_cast<tuple_id*>(sub_block_memory_) != 0)) {
    initialize();
  }
}

bool CompressedColumnStoreTupleStorageSubBlock::DescriptionIsValid(
    const CatalogRelationSchema &relation,
    const TupleStorageSubBlockDescription &description) {
  // Make sure description is initialized and specifies CompressedColumnStore.
  if (!description.IsInitialized()) {
    return false;
  }
  if (description.sub_block_type() != TupleStorageSubBlockDescription::COMPRESSED_COLUMN_STORE) {
    return false;
  }

  const Comparison &less_comparison = ComparisonFactory::GetComparison(ComparisonID::kLess);

  // Make sure the specified sort attribute exists and can be ordered by
  // LessComparison.
  if (!description.HasExtension(CompressedColumnStoreTupleStorageSubBlockDescription::sort_attribute_id)) {
    return false;
  }
  attribute_id sort_attribute_id = description.GetExtension(
      CompressedColumnStoreTupleStorageSubBlockDescription::sort_attribute_id);
  if (!relation.hasAttributeWithId(sort_attribute_id)) {
    return false;
  }
  const Type &sort_attr_type = relation.getAttributeById(sort_attribute_id)->getType();
  if (!less_comparison.canCompareTypes(sort_attr_type, sort_attr_type)) {
    return false;
  }

  // Make sure all the specified compressed attributes exist and can be ordered
  // by LessComparison.
  std::unordered_set<attribute_id> compressed_variable_length_attributes;
  for (int compressed_attribute_num = 0;
       compressed_attribute_num < description.ExtensionSize(
           CompressedColumnStoreTupleStorageSubBlockDescription::compressed_attribute_id);
       ++compressed_attribute_num) {
    attribute_id compressed_attribute_id = description.GetExtension(
        CompressedColumnStoreTupleStorageSubBlockDescription::compressed_attribute_id,
        compressed_attribute_num);
    if (!relation.hasAttributeWithId(compressed_attribute_id)) {
      return false;
    }
    const Type &attr_type = relation.getAttributeById(compressed_attribute_id)->getType();
    if (!less_comparison.canCompareTypes(attr_type, attr_type)) {
      return false;
    }
    if (attr_type.isVariableLength()) {
      compressed_variable_length_attributes.insert(compressed_attribute_id);
    }
  }

  // If the relation has variable-length attributes, make sure they are all
  // compressed.
  if (relation.isVariableLength()) {
    for (CatalogRelationSchema::const_iterator attr_it = relation.begin();
         attr_it != relation.end();
         ++attr_it) {
      if (attr_it->getType().isVariableLength()) {
        if (compressed_variable_length_attributes.find(attr_it->getID())
            == compressed_variable_length_attributes.end()) {
          return false;
        }
      }
    }
  }

  return true;
}

// TODO(chasseur): Make this heuristic better.
std::size_t CompressedColumnStoreTupleStorageSubBlock::EstimateBytesPerTuple(
    const CatalogRelationSchema &relation,
    const TupleStorageSubBlockDescription &description) {
  DEBUG_ASSERT(DescriptionIsValid(relation, description));

  std::unordered_set<attribute_id> compressed_attributes;
  for (int compressed_attribute_num = 0;
       compressed_attribute_num < description.ExtensionSize(
           CompressedColumnStoreTupleStorageSubBlockDescription::compressed_attribute_id);
       ++compressed_attribute_num) {
    compressed_attributes.insert(description.GetExtension(
        CompressedColumnStoreTupleStorageSubBlockDescription::compressed_attribute_id,
        compressed_attribute_num));
  }

  size_t total_size = 0;
  for (CatalogRelationSchema::const_iterator attr_it = relation.begin();
       attr_it != relation.end();
       ++attr_it) {
    if (compressed_attributes.find(attr_it->getID()) == compressed_attributes.end()) {
      total_size += attr_it->getType().estimateAverageByteLength();
    } else {
      // For compressed attributes, estimate 1/3 space.
      total_size += attr_it->getType().estimateAverageByteLength() / 3;
    }
  }

  // NOTE(chasseur): We round-up the number of bytes needed in the NULL bitmaps
  // to avoid estimating 0 bytes needed for a relation with less than 8
  // attributes which are all NullType. We estimate the bytes needed in the
  // NULL bitmaps based on the number of nullable attributes in the base
  // relation (not only those that are not specified as compressed in
  // 'description') because if compression does not actually save any space,
  // then such attributes will be stored uncompressed and still require space
  // in the NULL bitmaps.
  return total_size + ((relation.numNullableAttributes() + 7) >> 3);
}

const void* CompressedColumnStoreTupleStorageSubBlock::getAttributeValue(
    const tuple_id tuple,
    const attribute_id attr) const {
  DEBUG_ASSERT(hasTupleWithID(tuple));
  DEBUG_ASSERT(supportsUntypedGetAttributeValue(attr));

  if (dictionary_coded_attributes_[attr]) {
    return dictionaries_.atUnchecked(attr).getUntypedValueForCode<true>(
        compressedGetCode(tuple, attr));
  } else {
    return getAttributePtr<true>(tuple, attr);
  }
}

TypedValue CompressedColumnStoreTupleStorageSubBlock::getAttributeValueTyped(
    const tuple_id tuple,
    const attribute_id attr) const {
  DEBUG_ASSERT(hasTupleWithID(tuple));

  if (dictionary_coded_attributes_[attr]) {
    return dictionaries_.atUnchecked(attr).getTypedValueForCode(
        compressedGetCode(tuple, attr));
  } else if (truncated_attributes_[attr]) {
    if (truncated_attribute_is_int_[attr]) {
      return TypedValue(static_cast<int>(compressedGetCode(tuple, attr)));
    } else {
      return TypedValue(static_cast<std::int64_t>(compressedGetCode(tuple, attr)));
    }
  } else {
    const Type &attr_type = relation_.getAttributeById(attr)->getType();
    const void *untyped_value = getAttributePtr(tuple, attr);
    return (untyped_value == nullptr)
        ? attr_type.makeNullValue()
        : attr_type.makeValue(untyped_value, attr_type.maximumByteLength());
  }
}

ValueAccessor* CompressedColumnStoreTupleStorageSubBlock::createValueAccessor(
    const TupleIdSequence *sequence) const {
  CompressedColumnStoreValueAccessor *base_accessor
      = new CompressedColumnStoreValueAccessor(relation_,
                                               relation_,
                                               *static_cast<const tuple_id*>(sub_block_memory_),
                                               compression_info_,
                                               dictionary_coded_attributes_,
                                               truncated_attributes_,
                                               truncated_attribute_is_int_,
                                               dictionaries_,
                                               column_stripes_,
                                               uncompressed_column_null_bitmaps_);
  if (sequence == nullptr) {
    return base_accessor;
  } else {
    return new TupleIdSequenceAdapterValueAccessor<CompressedColumnStoreValueAccessor>(
        base_accessor,
        *sequence);
  }
}

bool CompressedColumnStoreTupleStorageSubBlock::deleteTuple(const tuple_id tuple) {
  DEBUG_ASSERT(hasTupleWithID(tuple));

  if (compression_info_.uncompressed_attribute_has_nulls(sort_column_id_)
      && uncompressed_column_null_bitmaps_[sort_column_id_].getBit(tuple)) {
    --uncompressed_nulls_in_sort_column_;
  }

  if (tuple == *static_cast<const tuple_id*>(sub_block_memory_) - 1) {
    --(*static_cast<tuple_id*>(sub_block_memory_));
    // Clear any null bits for the tuple.
    if (compression_info_.null_bitmap_bits() > 0) {
      for (PtrVector<BitVector<false>, true>::iterator it
               = uncompressed_column_null_bitmaps_.begin();
           it != uncompressed_column_null_bitmaps_.end();
           ++it) {
        if (!it.isNull()) {
          it->setBit(*static_cast<const tuple_id*>(sub_block_memory_), false);
        }
      }
    }
    return false;
  } else {
    // Shift subsequent tuples forward.
    shiftTuples(tuple, tuple + 1, *static_cast<const tuple_id*>(sub_block_memory_) - tuple - 1);
    shiftUncompressedNullBitmaps(tuple, -1);
    --(*static_cast<tuple_id*>(sub_block_memory_));
    return true;
  }
}

bool CompressedColumnStoreTupleStorageSubBlock::bulkDeleteTuples(TupleIdSequence *tuples) {
  if (tuples->empty()) {
    // Nothing to do.
    return false;
  }

  const tuple_id front = tuples->front();
  const tuple_id back = tuples->back();
  const tuple_id num_tuples = tuples->numTuples();

  if ((back == *static_cast<const tuple_id*>(sub_block_memory_) - 1)
       && (back - front == num_tuples - 1)) {
    // Just truncate the back.
    *static_cast<tuple_id*>(sub_block_memory_) = front;

    // Clear any null bits.
    if (compression_info_.null_bitmap_bits() > 0) {
      for (PtrVector<BitVector<false>, true>::iterator it
               = uncompressed_column_null_bitmaps_.begin();
           it != uncompressed_column_null_bitmaps_.end();
           ++it) {
        if (!it.isNull()) {
          it->setBitRange(*static_cast<const tuple_id*>(sub_block_memory_),
                          num_tuples,
                          false);
        }
      }
    }

    return false;
  }

  // Pack the non-deleted tuples.
  tuple_id dest_position = front;
  tuple_id src_tuple = dest_position;
  TupleIdSequence::const_iterator it = tuples->begin();
  tuple_id tail_shifted_distance = 0;
  for (tuple_id current_id = front;
       current_id < *static_cast<const tuple_id*>(sub_block_memory_);
       ++current_id, ++src_tuple) {
    if (current_id == *it) {
      // Don't copy a deleted tuple.
      shiftUncompressedNullBitmaps(*it - tail_shifted_distance, -1);
      ++tail_shifted_distance;
      ++it;
      if (it == tuples->end()) {
        // No more to delete, so copy all the remaining tuples in one go.
        shiftTuples(dest_position,
                    src_tuple + 1,
                    *static_cast<const tuple_id*>(sub_block_memory_) - back - 1);
        break;
      }
    } else {
      // Shift the next tuple forward.
      shiftTuples(dest_position, src_tuple, 1);
      ++dest_position;
    }
  }

  *static_cast<tuple_id*>(sub_block_memory_) -= num_tuples;

  return true;
}

predicate_cost_t CompressedColumnStoreTupleStorageSubBlock::estimatePredicateEvaluationCost(
    const ComparisonPredicate &predicate) const {
  if (predicate.isAttributeLiteralComparisonPredicate()) {
    std::pair<bool, attribute_id> comparison_attr = predicate.getAttributeFromAttributeLiteralComparison();
    if (comparison_attr.second == sort_column_id_) {
      return predicate_cost::kBinarySearch;
    } else if (dictionary_coded_attributes_[comparison_attr.second]
                || truncated_attributes_[comparison_attr.second]) {
      return predicate_cost::kCompressedColumnScan;
    }
  }
  return predicate_cost::kColumnScan;
}

TupleIdSequence* CompressedColumnStoreTupleStorageSubBlock::getMatchesForPredicate(
    const ComparisonPredicate &predicate,
    const TupleIdSequence *filter) const {
  if (dictionary_coded_attributes_[sort_column_id_] || truncated_attributes_[sort_column_id_]) {
    // NOTE(chasseur): The version from CompressedTupleStorageSubBlock will in
    // turn call getEqualCodes(), getNotEqualCodes(), or getCodesInRange() as
    // necessary for this block, which will use a fast binary search if
    // evaluating a predicate on the sort column.
    return CompressedTupleStorageSubBlock::getMatchesForPredicate(predicate, filter);
  } else {
    TupleIdSequence *matches = SortColumnPredicateEvaluator::EvaluatePredicateForUncompressedSortColumn(
        predicate,
        relation_,
        sort_column_id_,
        column_stripes_[sort_column_id_],
        *static_cast<const tuple_id*>(sub_block_memory_) - uncompressed_nulls_in_sort_column_);
    if (matches == nullptr) {
      // TODO(chasseur): There is considerable duplication of effort in
      // SortColumnPredicateEvaluator::EvaluatePredicateForUncompressedSortColumn()
      // and CompressedTupleStorageSubBlock::getMatchesForPredicate() which we
      // should try to eliminate.
      return CompressedTupleStorageSubBlock::getMatchesForPredicate(predicate, filter);
    } else {
      if (filter != nullptr) {
        matches->intersectWith(*filter);
      }
      return matches;
    }
  }
}

void CompressedColumnStoreTupleStorageSubBlock::rebuild() {
  if (builder_.get() != nullptr) {
    builder_->buildCompressedColumnStoreTupleStorageSubBlock(sub_block_memory_);
    builder_.reset();
    initialize();
  }
}

std::uint32_t CompressedColumnStoreTupleStorageSubBlock::compressedGetCode(
    const tuple_id tid,
    const attribute_id attr_id) const {
  DEBUG_ASSERT(hasTupleWithID(tid));
  DEBUG_ASSERT((dictionary_coded_attributes_[attr_id]) || (truncated_attributes_[attr_id]));
  const void *code_location = getAttributePtr<false>(tid, attr_id);
  switch (compression_info_.attribute_size(attr_id)) {
    case 1:
      return *static_cast<const std::uint8_t*>(code_location);
    case 2:
      return *static_cast<const std::uint16_t*>(code_location);
    case 4:
      return *static_cast<const std::uint32_t*>(code_location);
    default:
      FATAL_ERROR("Unexpected byte-length (not 1, 2, or 4) for compressed "
                  "attribute ID " << attr_id
                  << " in CompressedColumnStoreTupleStorageSubBlock::compressedGetCode()");
  }
}

TupleIdSequence* CompressedColumnStoreTupleStorageSubBlock::getEqualCodes(
    const attribute_id attr_id,
    const std::uint32_t code,
    const TupleIdSequence *filter) const {
  if (attr_id == sort_column_id_) {
    // Special (fast) case: do a binary search of the sort column.
    pair<uint32_t, uint32_t> code_range(code, code + 1);

    // Adjust the upper limit if doing so can avoid an extra binary search.
    if (dictionary_coded_attributes_[attr_id]) {
      if (code_range.second == compressedGetDictionary(attr_id).numberOfCodes()) {
        code_range.second = numeric_limits<uint32_t>::max();
      }
    } else if (code_range.first == GetMaxTruncatedValue(compression_info_.attribute_size(attr_id))) {
      code_range.second = numeric_limits<uint32_t>::max();
    }

    pair<tuple_id, tuple_id> tuple_range = getCompressedSortColumnRange(code_range);
    TupleIdSequence *matches = new TupleIdSequence(*static_cast<const tuple_id*>(sub_block_memory_));
    matches->setRange(tuple_range.first, tuple_range.second - tuple_range.first, true);
    if (filter != nullptr) {
      matches->intersectWith(*filter);
    }
    return matches;
  } else {
    return getCodesSatisfyingComparison<equal_to>(attr_id, code, filter);
  }
}

TupleIdSequence* CompressedColumnStoreTupleStorageSubBlock::getNotEqualCodes(
    const attribute_id attr_id,
    const std::uint32_t code,
    const TupleIdSequence *filter) const {
  if (attr_id == sort_column_id_) {
    // Special (fast) case: do a binary search of the sort column.
    pair<uint32_t, uint32_t> code_range(code, code + 1);

    // Adjust the upper limit if doing so can avoid an extra binary search.
    if (dictionary_coded_attributes_[attr_id]) {
      if (code_range.second == compressedGetDictionary(attr_id).numberOfCodes()) {
        code_range.second = numeric_limits<uint32_t>::max();
      }
    } else if (code_range.first == GetMaxTruncatedValue(compression_info_.attribute_size(attr_id))) {
      code_range.second = numeric_limits<uint32_t>::max();
    }

    pair<tuple_id, tuple_id> tuple_range = getCompressedSortColumnRange(code_range);

    // We searched for the range of equal codes, so return its complement.
    TupleIdSequence *matches = new TupleIdSequence(*static_cast<const tuple_id*>(sub_block_memory_));
    matches->setRange(0, tuple_range.first, true);
    matches->setRange(tuple_range.second,
                      *static_cast<const tuple_id*>(sub_block_memory_) - tuple_range.second,
                      true);
    if (filter != nullptr) {
      matches->intersectWith(*filter);
    }
    return matches;
  } else {
    return getCodesSatisfyingComparison<not_equal_to>(attr_id, code, filter);
  }
}

TupleIdSequence* CompressedColumnStoreTupleStorageSubBlock::getNotEqualCodesExcludingNull(
    const attribute_id attr_id,
    const std::uint32_t code,
    const std::uint32_t null_code,
    const TupleIdSequence *filter) const {
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
  static constexpr bool short_circuit = true;
#else
  static constexpr bool short_circuit = false;
#endif
  if (attr_id == sort_column_id_) {
    // Special (fast) case: do a binary search of the sort column.
    pair<uint32_t, uint32_t> code_range(code, code + 1);

    // Adjust the upper limit if doing so can avoid an extra binary search.
    if (dictionary_coded_attributes_[attr_id]) {
      if (code_range.second == compressedGetDictionary(attr_id).numberOfCodes()) {
        code_range.second = numeric_limits<uint32_t>::max();
      }
    } else if (code_range.first == GetMaxTruncatedValue(compression_info_.attribute_size(attr_id))) {
      code_range.second = numeric_limits<uint32_t>::max();
    }

    pair<tuple_id, tuple_id> tuple_range = getCompressedSortColumnRange(code_range);

    // Search for the beginning of nulls in the sort column.
    pair<uint32_t, uint32_t> null_range(null_code, numeric_limits<uint32_t>::max());
    pair<tuple_id, tuple_id> limit_range = getCompressedSortColumnRange(null_range);

    // We searched for the range of equal codes, so return its complement.
    TupleIdSequence *matches = new TupleIdSequence(*static_cast<const tuple_id*>(sub_block_memory_));
    matches->setRange(0, tuple_range.first, true);
    matches->setRange(tuple_range.second, limit_range.first - tuple_range.second, true);
    if (filter != nullptr) {
      matches->intersectWith(*filter);
    }
    return matches;
  } else {
    TupleIdSequence *matches = new TupleIdSequence(*static_cast<const tuple_id*>(sub_block_memory_));
    const void *attr_stripe = column_stripes_[attr_id];
    if (!short_circuit || (filter == nullptr)) {
      switch (compression_info_.attribute_size(attr_id)) {
        case 1:
          for (tuple_id tid = 0;
               tid < *static_cast<const tuple_id*>(sub_block_memory_);
               ++tid) {
            if ((code != static_cast<const uint8_t*>(attr_stripe)[tid])
                && (null_code != static_cast<const uint8_t*>(attr_stripe)[tid])) {
              matches->set(tid, true);
            }
          }
          break;
        case 2:
          for (tuple_id tid = 0;
               tid < *static_cast<const tuple_id*>(sub_block_memory_);
               ++tid) {
            if ((code != static_cast<const uint16_t*>(attr_stripe)[tid])
                && (null_code != static_cast<const uint16_t*>(attr_stripe)[tid])) {
              matches->set(tid, true);
            }
          }
          break;
        case 4:
          for (tuple_id tid = 0;
               tid <= *static_cast<const tuple_id*>(sub_block_memory_);
               ++tid) {
            if ((code != static_cast<const uint16_t*>(attr_stripe)[tid])
                && (null_code != static_cast<const uint16_t*>(attr_stripe)[tid])) {
              matches->set(tid, true);
            }
          }
          break;
        default:
          FATAL_ERROR("Unexpected byte-length (not 1, 2, or 4) for compressed "
                      "attribute ID " << attr_id
                      << " in CompressedColumnStoreTupleStorageSubBlock::getNotEqualCodesExcludingNull()");
      }
      if (filter != nullptr) {
        matches->intersectWith(*filter);
      }
    } else {
      switch (compression_info_.attribute_size(attr_id)) {
        case 1:
          for (TupleIdSequence::const_iterator filter_it = filter->begin();
               filter_it != filter->end();
               ++filter_it) {
            if ((code != static_cast<const uint8_t*>(attr_stripe)[*filter_it])
                && (null_code != static_cast<const uint8_t*>(attr_stripe)[*filter_it])) {
              matches->set(*filter_it, true);
            }
          }
          break;
        case 2:
          for (TupleIdSequence::const_iterator filter_it = filter->begin();
               filter_it != filter->end();
               ++filter_it) {
            if ((code != static_cast<const uint16_t*>(attr_stripe)[*filter_it])
                && (null_code != static_cast<const uint16_t*>(attr_stripe)[*filter_it])) {
              matches->set(*filter_it, true);
            }
          }
          break;
        case 4:
          for (TupleIdSequence::const_iterator filter_it = filter->begin();
               filter_it != filter->end();
               ++filter_it) {
            if ((code != static_cast<const uint16_t*>(attr_stripe)[*filter_it])
                && (null_code != static_cast<const uint16_t*>(attr_stripe)[*filter_it])) {
              matches->set(*filter_it, true);
            }
          }
          break;
        default:
          FATAL_ERROR("Unexpected byte-length (not 1, 2, or 4) for compressed "
                      "attribute ID " << attr_id
                      << " in CompressedColumnStoreTupleStorageSubBlock::getNotEqualCodesExcludingNull()");
      }
    }
    return matches;
  }
}

TupleIdSequence* CompressedColumnStoreTupleStorageSubBlock::getLessCodes(
    const attribute_id attr_id,
    const std::uint32_t code,
    const TupleIdSequence *filter) const {
  if (attr_id == sort_column_id_) {
    // Special (fast) case: do a binary search of the sort column.
    TupleIdSequence *matches = new TupleIdSequence(*static_cast<const tuple_id*>(sub_block_memory_));
    pair<tuple_id, tuple_id> tuple_range
        = getCompressedSortColumnRange(pair<uint32_t, uint32_t>(0, code));
    matches->setRange(tuple_range.first, tuple_range.second - tuple_range.first, true);
    if (filter != nullptr) {
      matches->intersectWith(*filter);
    }
    return matches;
  } else {
    return getCodesSatisfyingComparison<greater>(attr_id, code, filter);
  }
}

TupleIdSequence* CompressedColumnStoreTupleStorageSubBlock::getGreaterOrEqualCodes(
    const attribute_id attr_id,
    const std::uint32_t code,
    const TupleIdSequence *filter) const {
  if (attr_id == sort_column_id_) {
    // Special (fast) case: do a binary search of the sort column.
    TupleIdSequence *matches = new TupleIdSequence(*static_cast<const tuple_id*>(sub_block_memory_));
    pair<tuple_id, tuple_id> tuple_range
        = getCompressedSortColumnRange(pair<uint32_t, uint32_t>(code, numeric_limits<uint32_t>::max()));
    matches->setRange(tuple_range.first, tuple_range.second - tuple_range.first, true);
    if (filter != nullptr) {
      matches->intersectWith(*filter);
    }
    return matches;
  } else {
    return getCodesSatisfyingComparison<less_equal>(attr_id, code, filter);
  }
}

TupleIdSequence* CompressedColumnStoreTupleStorageSubBlock::getCodesInRange(
    const attribute_id attr_id,
    const std::pair<std::uint32_t, std::uint32_t> range,
    const TupleIdSequence *filter) const {
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
  static constexpr bool short_circuit = true;
#else
  static constexpr bool short_circuit = false;
#endif
  TupleIdSequence *matches = new TupleIdSequence(*static_cast<const tuple_id*>(sub_block_memory_));
  if (attr_id == sort_column_id_) {
    // Special (fast) case: do a binary search of the sort column.
    pair<tuple_id, tuple_id> tuple_range = getCompressedSortColumnRange(range);
    matches->setRange(tuple_range.first, tuple_range.second - tuple_range.first, true);
    if (filter != nullptr) {
      matches->intersectWith(*filter);
    }
  } else {
    const void *attr_stripe = column_stripes_[attr_id];
    if (!short_circuit || (filter == nullptr)) {
      switch (compression_info_.attribute_size(attr_id)) {
        case 1:
          for (tuple_id tid = 0;
               tid < *static_cast<const tuple_id*>(sub_block_memory_);
               ++tid) {
            if (range.first <= (static_cast<const uint8_t*>(attr_stripe)[tid])
                && (static_cast<const uint8_t*>(attr_stripe)[tid] < range.second)) {
              matches->set(tid, true);
            }
          }
          break;
        case 2:
          for (tuple_id tid = 0;
               tid < *static_cast<const tuple_id*>(sub_block_memory_);
               ++tid) {
            if (range.first <= (static_cast<const uint16_t*>(attr_stripe)[tid])
                && (static_cast<const uint16_t*>(attr_stripe)[tid] < range.second)) {
              matches->set(tid, true);
            }
          }
          break;
        case 4:
          for (tuple_id tid = 0;
               tid < *static_cast<const tuple_id*>(sub_block_memory_);
               ++tid) {
            if (range.first <= (static_cast<const uint32_t*>(attr_stripe)[tid])
                && (static_cast<const uint32_t*>(attr_stripe)[tid] < range.second)) {
              matches->set(tid, true);
            }
          }
          break;
        default:
          FATAL_ERROR("Unexpected byte-length (not 1, 2, or 4) for compressed "
                      "attribute ID " << attr_id
                      << " in CompressedColumnStoreTupleStorageSubBlock::getCodesInRange()");
      }
      if (filter != nullptr) {
        matches->intersectWith(*filter);
      }
    } else {
      switch (compression_info_.attribute_size(attr_id)) {
        case 1:
          for (TupleIdSequence::const_iterator filter_it = filter->begin();
               filter_it != filter->end();
               ++filter_it) {
            if (range.first <= (static_cast<const uint8_t*>(attr_stripe)[*filter_it])
                && (static_cast<const uint8_t*>(attr_stripe)[*filter_it] < range.second)) {
              matches->set(*filter_it, true);
            }
          }
          break;
        case 2:
          for (TupleIdSequence::const_iterator filter_it = filter->begin();
               filter_it != filter->end();
               ++filter_it) {
            if (range.first <= (static_cast<const uint16_t*>(attr_stripe)[*filter_it])
                && (static_cast<const uint16_t*>(attr_stripe)[*filter_it] < range.second)) {
              matches->set(*filter_it, true);
            }
          }
          break;
        case 4:
          for (TupleIdSequence::const_iterator filter_it = filter->begin();
               filter_it != filter->end();
               ++filter_it) {
            if (range.first <= (static_cast<const uint32_t*>(attr_stripe)[*filter_it])
                && (static_cast<const uint32_t*>(attr_stripe)[*filter_it] < range.second)) {
              matches->set(*filter_it, true);
            }
          }
          break;
        default:
          FATAL_ERROR("Unexpected byte-length (not 1, 2, or 4) for compressed "
                      "attribute ID " << attr_id
                      << " in CompressedColumnStoreTupleStorageSubBlock::getCodesInRange()");
      }
    }
  }

  return matches;
}

void CompressedColumnStoreTupleStorageSubBlock::initialize() {
  void *memory_position = initializeCommon();
  if (compression_info_.null_bitmap_bits() > 0) {
    for (attribute_id attr_id = 0;
         attr_id < compression_info_.uncompressed_attribute_has_nulls_size();
         ++attr_id) {
      if (compression_info_.uncompressed_attribute_has_nulls(attr_id)) {
        uncompressed_column_null_bitmaps_.push_back(
            new BitVector<false>(memory_position,
                                 compression_info_.null_bitmap_bits()));
        memory_position = static_cast<char*>(memory_position)
                          + BitVector<false>::BytesNeeded(compression_info_.null_bitmap_bits());

        if (attr_id == sort_column_id_) {
          uncompressed_nulls_in_sort_column_
              = uncompressed_column_null_bitmaps_.back().onesCount();
        }
      } else {
        uncompressed_column_null_bitmaps_.push_back(nullptr);
      }
    }
  }

  size_t tuple_length = 0;
  for (CatalogRelationSchema::const_iterator attr_it = relation_.begin();
       attr_it != relation_.end();
       ++attr_it) {
    tuple_length += compression_info_.attribute_size(attr_it->getID());
  }

  size_t max_num_tuples = (static_cast<const char*>(sub_block_memory_)
                          + sub_block_memory_size_ - static_cast<const char*>(memory_position))
                          / tuple_length;

  column_stripes_.resize(relation_.getMaxAttributeId() + 1, nullptr);

  for (CatalogRelationSchema::const_iterator attr_it = relation_.begin();
       attr_it != relation_.end();
       ++attr_it) {
    column_stripes_[attr_it->getID()] = memory_position;
    memory_position = static_cast<char*>(memory_position)
                      + max_num_tuples * compression_info_.attribute_size(attr_it->getID());
  }
}

void CompressedColumnStoreTupleStorageSubBlock::shiftTuples(
    const tuple_id dest_position,
    const tuple_id src_tuple,
    const tuple_id num_tuples) {
  for (attribute_id attr_id = 0;
       attr_id < compression_info_.attribute_size_size();
       ++attr_id) {
    size_t attr_length = compression_info_.attribute_size(attr_id);
    if (attr_length > 0) {
      memmove(static_cast<char*>(column_stripes_[attr_id]) + dest_position * attr_length,
              static_cast<const char*>(column_stripes_[attr_id]) + src_tuple * attr_length,
              attr_length * num_tuples);
    }
  }
}

void CompressedColumnStoreTupleStorageSubBlock::shiftUncompressedNullBitmaps(
    const tuple_id from_position,
    const tuple_id distance) {
  if (compression_info_.null_bitmap_bits() > 0) {
    for (PtrVector<BitVector<false>, true>::size_type idx = 0;
         idx < uncompressed_column_null_bitmaps_.size();
         ++idx) {
      if (!uncompressed_column_null_bitmaps_.elementIsNull(idx)) {
        if (distance < 0) {
          uncompressed_column_null_bitmaps_[idx].shiftTailForward(from_position, -distance);
        } else {
          uncompressed_column_null_bitmaps_[idx].shiftTailBackward(from_position, distance);
        }
      }
    }
  }
}

std::pair<tuple_id, tuple_id> CompressedColumnStoreTupleStorageSubBlock::getCompressedSortColumnRange(
    const std::pair<std::uint32_t, std::uint32_t> code_range) const {
  DEBUG_ASSERT(dictionary_coded_attributes_[sort_column_id_] || truncated_attributes_[sort_column_id_]);

  const void *attr_stripe = column_stripes_[sort_column_id_];
  pair<tuple_id, tuple_id> tuple_range;
  if (code_range.first == 0) {
    tuple_range.first = 0;
  } else {
    switch (compression_info_.attribute_size(sort_column_id_)) {
      case 1:
        tuple_range.first = lower_bound(static_cast<const uint8_t*>(attr_stripe),
                                        static_cast<const uint8_t*>(attr_stripe)
                                            + *static_cast<const tuple_id*>(sub_block_memory_),
                                        code_range.first)
                            - static_cast<const uint8_t*>(attr_stripe);
        break;
      case 2:
        tuple_range.first = lower_bound(static_cast<const uint16_t*>(attr_stripe),
                                        static_cast<const uint16_t*>(attr_stripe)
                                            + *static_cast<const tuple_id*>(sub_block_memory_),
                                        code_range.first)
                            - static_cast<const uint16_t*>(attr_stripe);
        break;
      case 4:
        tuple_range.first = lower_bound(static_cast<const uint32_t*>(attr_stripe),
                                        static_cast<const uint32_t*>(attr_stripe)
                                            + *static_cast<const tuple_id*>(sub_block_memory_),
                                        code_range.first)
                            - static_cast<const uint32_t*>(attr_stripe);
        break;
      default:
        FATAL_ERROR("Unexpected byte-length (not 1, 2, or 4) for compressed "
                    "attribute ID " << sort_column_id_
                    << " in CompressedColumnStoreTupleStorageSubBlock::getCompressedSortColumnRange()");
    }
  }

  if (code_range.second == numeric_limits<uint32_t>::max()) {
    tuple_range.second = *static_cast<const tuple_id*>(sub_block_memory_);
  } else {
    switch (compression_info_.attribute_size(sort_column_id_)) {
      case 1:
        tuple_range.second = lower_bound(static_cast<const uint8_t*>(attr_stripe),
                                         static_cast<const uint8_t*>(attr_stripe)
                                             + *static_cast<const tuple_id*>(sub_block_memory_),
                                         code_range.second)
                             - static_cast<const uint8_t*>(attr_stripe);
        break;
      case 2:
        tuple_range.second = lower_bound(static_cast<const uint16_t*>(attr_stripe),
                                         static_cast<const uint16_t*>(attr_stripe)
                                             + *static_cast<const tuple_id*>(sub_block_memory_),
                                         code_range.second)
                             - static_cast<const uint16_t*>(attr_stripe);
        break;
      case 4:
        tuple_range.second = lower_bound(static_cast<const uint32_t*>(attr_stripe),
                                         static_cast<const uint32_t*>(attr_stripe)
                                             + *static_cast<const tuple_id*>(sub_block_memory_),
                                         code_range.second)
                             - static_cast<const uint32_t*>(attr_stripe);
        break;
      default:
        FATAL_ERROR("Unexpected byte-length (not 1, 2, or 4) for compressed "
                    "attribute ID " << sort_column_id_
                    << " in CompressedColumnStoreTupleStorageSubBlock::getCompressedSortColumnRange()");
    }
  }

  return tuple_range;
}

template <template <typename T> class comparison_functor>
TupleIdSequence* CompressedColumnStoreTupleStorageSubBlock::getCodesSatisfyingComparison(
    const attribute_id attr_id,
    const std::uint32_t code,
    const TupleIdSequence *filter) const {
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
  static constexpr bool short_circuit = true;
#else
  static constexpr bool short_circuit = false;
#endif
  comparison_functor<uint32_t> comp;
  TupleIdSequence *matches = new TupleIdSequence(*static_cast<const tuple_id*>(sub_block_memory_));
  const void *attr_stripe = column_stripes_[attr_id];
  if (!short_circuit || (filter == nullptr)) {
    switch (compression_info_.attribute_size(attr_id)) {
      case 1:
        for (tuple_id tid = 0;
             tid < *static_cast<const tuple_id*>(sub_block_memory_);
             ++tid) {
          if (comp(code, static_cast<const uint8_t*>(attr_stripe)[tid])) {
            matches->set(tid, true);
          }
        }
        break;
      case 2:
        for (tuple_id tid = 0;
             tid < *static_cast<const tuple_id*>(sub_block_memory_);
             ++tid) {
          if (comp(code, static_cast<const uint16_t*>(attr_stripe)[tid])) {
            matches->set(tid, true);
          }
        }
        break;
      case 4:
        for (tuple_id tid = 0;
             tid < *static_cast<const tuple_id*>(sub_block_memory_);
             ++tid) {
          if (comp(code, static_cast<const uint32_t*>(attr_stripe)[tid])) {
            matches->set(tid, true);
          }
        }
        break;
      default:
        FATAL_ERROR("Unexpected byte-length (not 1, 2, or 4) for compressed "
                    "attribute ID " << attr_id
                    << " in CompressedColumnStoreTupleStorageSubBlock::getCodesSatisfyingComparison()");
    }
    if (filter != nullptr) {
      matches->intersectWith(*filter);
    }
  } else {
    switch (compression_info_.attribute_size(attr_id)) {
      case 1:
        for (TupleIdSequence::const_iterator filter_it = filter->begin();
             filter_it != filter->end();
             ++filter_it) {
          if (comp(code, static_cast<const uint8_t*>(attr_stripe)[*filter_it])) {
            matches->set(*filter_it, true);
          }
        }
        break;
      case 2:
        for (TupleIdSequence::const_iterator filter_it = filter->begin();
             filter_it != filter->end();
             ++filter_it) {
          if (comp(code, static_cast<const uint16_t*>(attr_stripe)[*filter_it])) {
            matches->set(*filter_it, true);
          }
        }
        break;
      case 4:
        for (TupleIdSequence::const_iterator filter_it = filter->begin();
             filter_it != filter->end();
             ++filter_it) {
          if (comp(code, static_cast<const uint32_t*>(attr_stripe)[*filter_it])) {
            matches->set(*filter_it, true);
          }
        }
        break;
      default:
        FATAL_ERROR("Unexpected byte-length (not 1, 2, or 4) for compressed "
                    "attribute ID " << attr_id
                    << " in CompressedColumnStoreTupleStorageSubBlock::getCodesSatisfyingComparison()");
    }
  }

  return matches;
}

}  // namespace quickstep
