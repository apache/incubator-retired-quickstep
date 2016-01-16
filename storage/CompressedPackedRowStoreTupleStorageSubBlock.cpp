/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "storage/CompressedPackedRowStoreTupleStorageSubBlock.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <memory>
#include <unordered_set>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "compression/CompressionDictionaryLite.hpp"
#include "expressions/predicate/ComparisonPredicate.hpp"
#include "expressions/predicate/PredicateCost.hpp"
#include "storage/CompressedPackedRowStoreValueAccessor.hpp"
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

using std::equal_to;
using std::greater;
using std::less_equal;
using std::memmove;
using std::not_equal_to;
using std::pair;
using std::size_t;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;

namespace quickstep {

QUICKSTEP_REGISTER_TUPLE_STORE(CompressedPackedRowStoreTupleStorageSubBlock, COMPRESSED_PACKED_ROW_STORE);

CompressedPackedRowStoreTupleStorageSubBlock::CompressedPackedRowStoreTupleStorageSubBlock(
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
      num_uncompressed_attributes_with_nulls_(0) {
  if (!DescriptionIsValid(relation_, description_)) {
    FATAL_ERROR("Attempted to construct a CompressedPackedRowStoreTupleStorageSubBlock "
                "from an invalid description.");
  }

  if ((!new_block) && (*static_cast<tuple_id*>(sub_block_memory_) != 0)) {
    initialize();
  }
}

bool CompressedPackedRowStoreTupleStorageSubBlock::DescriptionIsValid(
    const CatalogRelationSchema &relation,
    const TupleStorageSubBlockDescription &description) {
  // Make sure description is initialized and specifies
  // CompressedPackedRowStore.
  if (!description.IsInitialized()) {
    return false;
  }
  if (description.sub_block_type() != TupleStorageSubBlockDescription::COMPRESSED_PACKED_ROW_STORE) {
    return false;
  }

  // Make sure all the specified compressed attributes exist and can be ordered
  // by LessComparison.
  const Comparison &less_comparison = ComparisonFactory::GetComparison(ComparisonID::kLess);
  std::unordered_set<attribute_id> compressed_variable_length_attributes;
  for (int compressed_attribute_num = 0;
       compressed_attribute_num < description.ExtensionSize(
           CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id);
       ++compressed_attribute_num) {
    attribute_id compressed_attribute_id = description.GetExtension(
        CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id,
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
std::size_t CompressedPackedRowStoreTupleStorageSubBlock::EstimateBytesPerTuple(
    const CatalogRelationSchema &relation,
    const TupleStorageSubBlockDescription &description) {
  DEBUG_ASSERT(DescriptionIsValid(relation, description));

  std::unordered_set<attribute_id> compressed_attributes;
  for (int compressed_attribute_num = 0;
       compressed_attribute_num < description.ExtensionSize(
           CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id);
       ++compressed_attribute_num) {
    compressed_attributes.insert(description.GetExtension(
        CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id,
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

  // NOTE(chasseur): We round-up the number of bytes needed in the NULL bitmap
  // to avoid estimating 0 bytes needed for a relation with less than 8
  // attributes which are all NullType. We estimate the bytes needed in the
  // NULL bitmap based on the number of nullable attributes in the base
  // relation (not only those that are not specified as compressed in
  // 'description') because if compression does not actually save any space,
  // then such attributes will be stored uncompressed and still require space
  // in the NULL bitmap.
  return total_size + ((relation.numNullableAttributes() + 7) >> 3);
}

const void* CompressedPackedRowStoreTupleStorageSubBlock::getAttributeValue(
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

TypedValue CompressedPackedRowStoreTupleStorageSubBlock::getAttributeValueTyped(
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

ValueAccessor* CompressedPackedRowStoreTupleStorageSubBlock::createValueAccessor(
    const TupleIdSequence *sequence) const {
  CompressedPackedRowStoreValueAccessor *base_accessor
      = new CompressedPackedRowStoreValueAccessor(relation_,
                                                  relation_,
                                                  *static_cast<const tuple_id*>(sub_block_memory_),
                                                  tuple_length_bytes_,
                                                  attribute_offsets_,
                                                  compression_info_,
                                                  dictionary_coded_attributes_,
                                                  truncated_attributes_,
                                                  truncated_attribute_is_int_,
                                                  dictionaries_,
                                                  tuple_storage_,
                                                  null_bitmap_.get(),
                                                  num_uncompressed_attributes_with_nulls_,
                                                  null_bitmap_attribute_offsets_);
  if (sequence == nullptr) {
    return base_accessor;
  } else {
    return new TupleIdSequenceAdapterValueAccessor<CompressedPackedRowStoreValueAccessor>(
        base_accessor,
        *sequence);
  }
}

bool CompressedPackedRowStoreTupleStorageSubBlock::deleteTuple(const tuple_id tuple) {
  DEBUG_ASSERT(hasTupleWithID(tuple));

  if (tuple == *static_cast<const tuple_id*>(sub_block_memory_) - 1) {
    // Simply truncate if only removing the last tuple.
    --(*static_cast<tuple_id*>(sub_block_memory_));
    if (null_bitmap_.get() != nullptr) {
      null_bitmap_->setBitRange(tuple * num_uncompressed_attributes_with_nulls_,
                                num_uncompressed_attributes_with_nulls_,
                                false);
    }
    return false;
  } else {
    // Shift subsequent tuples forward.
    memmove(static_cast<char*>(tuple_storage_) + tuple * tuple_length_bytes_,
            static_cast<const char*>(tuple_storage_) + (tuple + 1) * tuple_length_bytes_,
            (*static_cast<const tuple_id*>(sub_block_memory_) - tuple - 1) * tuple_length_bytes_);
    if (null_bitmap_.get() != nullptr) {
      null_bitmap_->shiftTailForward(tuple * num_uncompressed_attributes_with_nulls_,
                                     num_uncompressed_attributes_with_nulls_);
    }
    --(*static_cast<tuple_id*>(sub_block_memory_));
    return true;
  }
}

bool CompressedPackedRowStoreTupleStorageSubBlock::bulkDeleteTuples(TupleIdSequence *tuples) {
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
    if (null_bitmap_.get() != nullptr) {
      null_bitmap_->setBitRange(
          *static_cast<tuple_id*>(sub_block_memory_) * num_uncompressed_attributes_with_nulls_,
          num_tuples * num_uncompressed_attributes_with_nulls_,
          false);
    }
    return false;
  }

  // Pack the non-deleted tuples.
  tuple_id dest_tid = front;
  tuple_id src_tid = dest_tid;

  TupleIdSequence::const_iterator it = tuples->begin();
  for (tuple_id current_id = front;
       current_id < *static_cast<const tuple_id*>(sub_block_memory_);
       ++current_id, ++src_tid) {
    if (current_id == *it) {
      // Don't copy a deleted tuple.

      if (null_bitmap_.get() != nullptr) {
        // Erase the deleted tuple's entries in the null bitmap.
        null_bitmap_->shiftTailForward(dest_tid * num_uncompressed_attributes_with_nulls_,
                                       num_uncompressed_attributes_with_nulls_);
      }

      ++it;
      if (it == tuples->end()) {
        // No more to delete, so copy all the remaining tuples in one go.
        memmove(static_cast<char*>(tuple_storage_) + dest_tid * tuple_length_bytes_,
                static_cast<char*>(tuple_storage_) + (src_tid + 1) * tuple_length_bytes_,
                (*static_cast<const tuple_id*>(sub_block_memory_) - back - 1) * tuple_length_bytes_);
        break;
      }
    } else {
      // Copy the next tuple to the packed region.
      memmove(static_cast<char*>(tuple_storage_) + dest_tid * tuple_length_bytes_,
              static_cast<char*>(tuple_storage_) + src_tid * tuple_length_bytes_,
              tuple_length_bytes_);
      ++dest_tid;
    }
  }

  *static_cast<tuple_id*>(sub_block_memory_) -= num_tuples;

  return true;
}

predicate_cost_t CompressedPackedRowStoreTupleStorageSubBlock::estimatePredicateEvaluationCost(
    const ComparisonPredicate &predicate) const {
  if (predicate.isAttributeLiteralComparisonPredicate()) {
    std::pair<bool, attribute_id> comparison_attr = predicate.getAttributeFromAttributeLiteralComparison();
    if (dictionary_coded_attributes_[comparison_attr.second]
        || truncated_attributes_[comparison_attr.second]) {
      return predicate_cost::kCompressedRowScan;
    }
  }
  return predicate_cost::kRowScan;
}

void CompressedPackedRowStoreTupleStorageSubBlock::rebuild() {
  if (builder_.get() != nullptr) {
    builder_->buildCompressedPackedRowStoreTupleStorageSubBlock(sub_block_memory_);
    builder_.reset();
    initialize();
  }
}

std::uint32_t CompressedPackedRowStoreTupleStorageSubBlock::compressedGetCode(
    const tuple_id tid,
    const attribute_id attr_id) const {
  DEBUG_ASSERT(hasTupleWithID(tid));
  DEBUG_ASSERT((dictionary_coded_attributes_[attr_id]) || (truncated_attributes_[attr_id]));
  const void *code_location = static_cast<const char*>(tuple_storage_)
                              + tid * tuple_length_bytes_
                              + attribute_offsets_[attr_id];
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
                  << " in CompressedPackedRowStoreTupleStorageSubBlock::compressedGetCodeInl()");
  }
}

TupleIdSequence* CompressedPackedRowStoreTupleStorageSubBlock::getEqualCodes(
    const attribute_id attr_id,
    const std::uint32_t code,
    const TupleIdSequence *filter) const {
  return getCodesSatisfyingComparison<equal_to>(attr_id, code, filter);
}

TupleIdSequence* CompressedPackedRowStoreTupleStorageSubBlock::getNotEqualCodes(
    const attribute_id attr_id,
    const std::uint32_t code,
    const TupleIdSequence *filter) const {
  return getCodesSatisfyingComparison<not_equal_to>(attr_id, code, filter);
}

TupleIdSequence* CompressedPackedRowStoreTupleStorageSubBlock::getNotEqualCodesExcludingNull(
    const attribute_id attr_id,
    const std::uint32_t code,
    const std::uint32_t null_code,
    const TupleIdSequence *filter) const {
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
  static constexpr bool short_circuit = true;
#else
  static constexpr bool short_circuit = false;
#endif
  TupleIdSequence *matches = new TupleIdSequence(*static_cast<const tuple_id*>(sub_block_memory_));
  const char *attr_location = static_cast<const char*>(tuple_storage_)
                              + attribute_offsets_[attr_id];
  if (!short_circuit || (filter == nullptr)) {
    switch (compression_info_.attribute_size(attr_id)) {
      case 1:
        for (tuple_id tid = 0;
             tid < *static_cast<const tuple_id*>(sub_block_memory_);
             ++tid, attr_location += tuple_length_bytes_) {
          if ((code != *reinterpret_cast<const uint8_t*>(attr_location))
              && (null_code != *reinterpret_cast<const uint8_t*>(attr_location))) {
            matches->set(tid, true);
          }
        }
        break;
      case 2:
        for (tuple_id tid = 0;
             tid < *static_cast<const tuple_id*>(sub_block_memory_);
             ++tid, attr_location += tuple_length_bytes_) {
          if ((code != *reinterpret_cast<const uint16_t*>(attr_location))
              && (null_code != *reinterpret_cast<const uint16_t*>(attr_location))) {
            matches->set(tid, true);
          }
        }
        break;
      case 4:
        for (tuple_id tid = 0;
             tid < *static_cast<const tuple_id*>(sub_block_memory_);
             ++tid, attr_location += tuple_length_bytes_) {
          if ((code != *reinterpret_cast<const uint32_t*>(attr_location))
              && (null_code != *reinterpret_cast<const uint32_t*>(attr_location))) {
            matches->set(tid, true);
          }
        }
        break;
      default:
        FATAL_ERROR("Unexpected byte-length (not 1, 2, or 4) for compressed "
                    "attribute ID " << attr_id
                    << " in CompressedPackedRowStoreTupleStorageSubBlock::getNotEqualCodesExcludingNull()");
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
          const void *local_attr_location = attr_location + (*filter_it) * tuple_length_bytes_;
          if ((code != *reinterpret_cast<const uint8_t*>(local_attr_location))
              && (null_code != *reinterpret_cast<const uint8_t*>(local_attr_location))) {
            matches->set(*filter_it, true);
          }
        }
        break;
      case 2:
        for (TupleIdSequence::const_iterator filter_it = filter->begin();
             filter_it != filter->end();
             ++filter_it) {
          const void *local_attr_location = attr_location + (*filter_it) * tuple_length_bytes_;
          if ((code != *reinterpret_cast<const uint16_t*>(local_attr_location))
              && (null_code != *reinterpret_cast<const uint16_t*>(local_attr_location))) {
            matches->set(*filter_it, true);
          }
        }
        break;
      case 4:
        for (TupleIdSequence::const_iterator filter_it = filter->begin();
             filter_it != filter->end();
             ++filter_it) {
          const void *local_attr_location = attr_location + (*filter_it) * tuple_length_bytes_;
          if ((code != *reinterpret_cast<const uint32_t*>(local_attr_location))
              && (null_code != *reinterpret_cast<const uint32_t*>(local_attr_location))) {
            matches->set(*filter_it, true);
          }
        }
        break;
      default:
        FATAL_ERROR("Unexpected byte-length (not 1, 2, or 4) for compressed "
                    "attribute ID " << attr_id
                    << " in CompressedPackedRowStoreTupleStorageSubBlock::getNotEqualCodesExcludingNull()");
    }
  }
  return matches;
}

TupleIdSequence* CompressedPackedRowStoreTupleStorageSubBlock::getLessCodes(
    const attribute_id attr_id,
    const std::uint32_t code,
    const TupleIdSequence *filter) const {
  return getCodesSatisfyingComparison<greater>(attr_id, code, filter);
}

TupleIdSequence* CompressedPackedRowStoreTupleStorageSubBlock::getGreaterOrEqualCodes(
    const attribute_id attr_id,
    const std::uint32_t code,
    const TupleIdSequence *filter) const {
  return getCodesSatisfyingComparison<less_equal>(attr_id, code, filter);
}

TupleIdSequence* CompressedPackedRowStoreTupleStorageSubBlock::getCodesInRange(
    const attribute_id attr_id,
    const std::pair<std::uint32_t, std::uint32_t> range,
    const TupleIdSequence *filter) const {
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
  static constexpr bool short_circuit = true;
#else
  static constexpr bool short_circuit = false;
#endif
  TupleIdSequence *matches = new TupleIdSequence(*static_cast<const tuple_id*>(sub_block_memory_));
  const char *attr_location = static_cast<const char*>(tuple_storage_)
                              + attribute_offsets_[attr_id];
  if (!short_circuit || (filter == nullptr)) {
    switch (compression_info_.attribute_size(attr_id)) {
      case 1:
        for (tuple_id tid = 0;
             tid < *static_cast<const tuple_id*>(sub_block_memory_);
             ++tid, attr_location += tuple_length_bytes_) {
          if (range.first <= (*reinterpret_cast<const uint8_t*>(attr_location))
              && (*reinterpret_cast<const uint8_t*>(attr_location) < range.second)) {
            matches->set(tid, true);
          }
        }
        break;
      case 2:
        for (tuple_id tid = 0;
             tid < *static_cast<const tuple_id*>(sub_block_memory_);
             ++tid, attr_location += tuple_length_bytes_) {
          if (range.first <= (*reinterpret_cast<const uint16_t*>(attr_location))
              && (*reinterpret_cast<const uint16_t*>(attr_location) < range.second)) {
            matches->set(tid, true);
          }
        }
        break;
      case 4:
        for (tuple_id tid = 0;
             tid < *static_cast<const tuple_id*>(sub_block_memory_);
             ++tid, attr_location += tuple_length_bytes_) {
          if (range.first <= (*reinterpret_cast<const uint32_t*>(attr_location))
              && (*reinterpret_cast<const uint32_t*>(attr_location) < range.second)) {
            matches->set(tid, true);
          }
        }
        break;
      default:
        FATAL_ERROR("Unexpected byte-length (not 1, 2, or 4) for compressed "
                    "attribute ID " << attr_id
                    << " in CompressedPackedRowStoreTupleStorageSubBlock::getCodesInRange()");
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
          const void *local_attr_location = attr_location + (*filter_it) * tuple_length_bytes_;
          if (range.first <= (*reinterpret_cast<const uint8_t*>(local_attr_location))
              && (*reinterpret_cast<const uint8_t*>(local_attr_location) < range.second)) {
            matches->set(*filter_it, true);
          }
        }
        break;
      case 2:
        for (TupleIdSequence::const_iterator filter_it = filter->begin();
             filter_it != filter->end();
             ++filter_it) {
          const void *local_attr_location = attr_location + (*filter_it) * tuple_length_bytes_;
          if (range.first <= (*reinterpret_cast<const uint16_t*>(local_attr_location))
              && (*reinterpret_cast<const uint16_t*>(local_attr_location) < range.second)) {
            matches->set(*filter_it, true);
          }
        }
        break;
      case 4:
        for (TupleIdSequence::const_iterator filter_it = filter->begin();
             filter_it != filter->end();
             ++filter_it) {
          const void *local_attr_location = attr_location + (*filter_it) * tuple_length_bytes_;
          if (range.first <= (*reinterpret_cast<const uint32_t*>(local_attr_location))
              && (*reinterpret_cast<const uint32_t*>(local_attr_location) < range.second)) {
            matches->set(*filter_it, true);
          }
        }
        break;
      default:
        FATAL_ERROR("Unexpected byte-length (not 1, 2, or 4) for compressed "
                    "attribute ID " << attr_id
                    << " in CompressedPackedRowStoreTupleStorageSubBlock::getCodesInRange()");
    }
  }
  return matches;
}

void CompressedPackedRowStoreTupleStorageSubBlock::initialize() {
  void *memory_position = initializeCommon();
  if (compression_info_.null_bitmap_bits() > 0) {
    null_bitmap_.reset(new BitVector<false>(memory_position,
                                            compression_info_.null_bitmap_bits()));
    memory_position = static_cast<char*>(memory_position)
                      + BitVector<false>::BytesNeeded(compression_info_.null_bitmap_bits());
  }
  tuple_storage_ = memory_position;

  tuple_length_bytes_ = 0;
  attribute_offsets_.resize(relation_.getMaxAttributeId() + 1, 0);
  null_bitmap_attribute_offsets_.resize(relation_.getMaxAttributeId() + 1, 0);

  for (CatalogRelationSchema::const_iterator attr_it = relation_.begin();
       attr_it != relation_.end();
       ++attr_it) {
    attribute_offsets_[attr_it->getID()] = tuple_length_bytes_;
    tuple_length_bytes_ += compression_info_.attribute_size(attr_it->getID());

    if (compression_info_.uncompressed_attribute_has_nulls(attr_it->getID())) {
      null_bitmap_attribute_offsets_[attr_it->getID()]
          = num_uncompressed_attributes_with_nulls_;
      ++num_uncompressed_attributes_with_nulls_;
    }
  }
}

template <template <typename T> class comparison_functor>
TupleIdSequence* CompressedPackedRowStoreTupleStorageSubBlock::getCodesSatisfyingComparison(
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
  const char *attr_location = static_cast<const char*>(tuple_storage_)
                              + attribute_offsets_[attr_id];
  if (!short_circuit || (filter == nullptr)) {
    switch (compression_info_.attribute_size(attr_id)) {
      case 1:
        for (tuple_id tid = 0;
             tid < *static_cast<const tuple_id*>(sub_block_memory_);
             ++tid, attr_location += tuple_length_bytes_) {
          if (comp(code, *reinterpret_cast<const uint8_t*>(attr_location))) {
            matches->set(tid, true);
          }
        }
        break;
      case 2:
        for (tuple_id tid = 0;
             tid < *static_cast<const tuple_id*>(sub_block_memory_);
             ++tid, attr_location += tuple_length_bytes_) {
          if (comp(code, *reinterpret_cast<const uint16_t*>(attr_location))) {
            matches->set(tid, true);
          }
        }
        break;
      case 4:
        for (tuple_id tid = 0;
             tid < *static_cast<const tuple_id*>(sub_block_memory_);
             ++tid, attr_location += tuple_length_bytes_) {
          if (comp(code, *reinterpret_cast<const uint32_t*>(attr_location))) {
            matches->set(tid, true);
          }
        }
        break;
      default:
        FATAL_ERROR("Unexpected byte-length (not 1, 2, or 4) for compressed "
                    "attribute ID " << attr_id
                    << " in CompressedPackedRowStoreTupleStorageSubBlock::getCodesSatisfyingComparison()");
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
          const void *local_attr_location = attr_location + (*filter_it) * tuple_length_bytes_;
          if (comp(code, *reinterpret_cast<const uint8_t*>(local_attr_location))) {
            matches->set(*filter_it, true);
          }
        }
        break;
      case 2:
        for (TupleIdSequence::const_iterator filter_it = filter->begin();
             filter_it != filter->end();
             ++filter_it) {
          const void *local_attr_location = attr_location + (*filter_it) * tuple_length_bytes_;
          if (comp(code, *reinterpret_cast<const uint16_t*>(local_attr_location))) {
            matches->set(*filter_it, true);
          }
        }
        break;
      case 4:
        for (TupleIdSequence::const_iterator filter_it = filter->begin();
             filter_it != filter->end();
             ++filter_it) {
          const void *local_attr_location = attr_location + (*filter_it) * tuple_length_bytes_;
          if (comp(code, *reinterpret_cast<const uint32_t*>(local_attr_location))) {
            matches->set(*filter_it, true);
          }
        }
        break;
      default:
        FATAL_ERROR("Unexpected byte-length (not 1, 2, or 4) for compressed "
                    "attribute ID " << attr_id
                    << " in CompressedPackedRowStoreTupleStorageSubBlock::getCodesSatisfyingComparison()");
    }
  }
  return matches;
}

}  // namespace quickstep
