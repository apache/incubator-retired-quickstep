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

#include "storage/CompressedTupleStorageSubBlock.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <utility>
#include <vector>

#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "compression/CompressionDictionary.hpp"
#include "compression/CompressionDictionaryLite.hpp"
#include "expressions/predicate/ComparisonPredicate.hpp"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "storage/CompressedBlockBuilder.hpp"
#include "storage/CompressedStoreUtil.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageErrors.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/Tuple.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/Macros.hpp"

using std::ceil;
using std::floor;
using std::int64_t;
using std::numeric_limits;
using std::pair;
using std::size_t;
using std::uint32_t;

namespace quickstep {

CompressedTupleStorageSubBlock::CompressedTupleStorageSubBlock(
    const CatalogRelationSchema &relation,
    const TupleStorageSubBlockDescription &description,
    const bool new_block,
    void *sub_block_memory,
    const std::size_t sub_block_memory_size)
    : TupleStorageSubBlock(relation,
                           description,
                           new_block,
                           sub_block_memory,
                           sub_block_memory_size) {
  if (new_block) {
    if (sub_block_memory_size_ < sizeof(tuple_id) + sizeof(int)) {
      throw BlockMemoryTooSmall("CompressedTupleStorageSubBlock",
                                sub_block_memory_size_);
    }

    *static_cast<tuple_id*>(sub_block_memory_) = 0;
    builder_.reset(new CompressedBlockBuilder(relation_, description_, sub_block_memory_size_));
    if (builder_->getMinimumRequiredBlockSize() > sub_block_memory_size_) {
      throw BlockMemoryTooSmall("CompressedTupleStorageSubBlock",
                                sub_block_memory_size_);
    }
  } else {
    if (sub_block_memory_size_ < sizeof(tuple_id) + sizeof(int)) {
      throw MalformedBlock();
    }
    if (*reinterpret_cast<const int*>(static_cast<const char*>(sub_block_memory_) + sizeof(tuple_id)) <= 0) {
      throw MalformedBlock();
    }
    if (*reinterpret_cast<const int*>(static_cast<const char*>(sub_block_memory_) + sizeof(tuple_id))
        + sizeof(int) + sizeof(tuple_id)
        > sub_block_memory_size_) {
      throw MalformedBlock();
    }

    if (*static_cast<tuple_id*>(sub_block_memory_) == 0) {
      builder_.reset(new CompressedBlockBuilder(relation_, description_, sub_block_memory_size_));
      if (builder_->getMinimumRequiredBlockSize() > sub_block_memory_size_) {
        throw MalformedBlock();
      }
    }
  }
}

std::int64_t CompressedTupleStorageSubBlock::GetEffectiveLiteralValueForComparisonWithTruncatedAttribute(
    const ComparisonID comp,
    const TypedValue &right_literal,
    const Type &right_literal_type) {
  return CompressedAttributePredicateTransformer::GetEffectiveLiteralValueForComparisonWithTruncatedAttribute(
      comp, right_literal, right_literal_type);
}

bool CompressedTupleStorageSubBlock::insertTupleInBatch(
    const Tuple &tuple) {
#ifdef QUICKSTEP_DEBUG
  paranoidInsertTypeCheck(tuple);
#endif

  if (builder_.get() == nullptr) {
    return false;
  }

  return builder_->addTuple(tuple);
}

tuple_id CompressedTupleStorageSubBlock::bulkInsertTuples(
    ValueAccessor *accessor) {
  tuple_id inserted_count = 0;
  InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    while (accessor->next()) {
      if (!builder_->takeTuple(accessor->getTuple())) {
        accessor->previous();
        break;
      }
      ++inserted_count;
    }
  });
  return inserted_count;
}

tuple_id CompressedTupleStorageSubBlock::bulkInsertTuplesWithRemappedAttributes(
    const std::vector<attribute_id> &attribute_map,
    ValueAccessor *accessor) {
  tuple_id inserted_count = 0;
  InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    while (accessor->next()) {
      if (!builder_->takeTuple(accessor->getTupleWithAttributes(attribute_map))) {
        accessor->previous();
        break;
      }
      ++inserted_count;
    }
  });
  return inserted_count;
}

TupleIdSequence* CompressedTupleStorageSubBlock::getMatchesForPredicate(
    const ComparisonPredicate &predicate,
    const TupleIdSequence *filter) const {
  DEBUG_ASSERT(builder_.get() == nullptr);

  // Determine if the predicate is a comparison of a compressed attribute with
  // a literal.
  if (predicate.isAttributeLiteralComparisonPredicate()) {
    const CatalogAttribute *comparison_attribute = NULL;
    if (predicate.getLeftOperand().hasStaticValue()) {
      DEBUG_ASSERT(predicate.getRightOperand().getDataSource() == Scalar::kAttribute);
      comparison_attribute
          = &(static_cast<const ScalarAttribute&>(predicate.getRightOperand()).getAttribute());
    } else {
      DEBUG_ASSERT(predicate.getLeftOperand().getDataSource() == Scalar::kAttribute);
      comparison_attribute
          = &(static_cast<const ScalarAttribute&>(predicate.getLeftOperand()).getAttribute());
    }
    const attribute_id comparison_attribute_id = comparison_attribute->getID();

    DEBUG_ASSERT(comparison_attribute->getParent().getID() == relation_.getID());
    if (dictionary_coded_attributes_[comparison_attribute_id]
        || truncated_attributes_[comparison_attribute_id]) {
      const CompressionDictionary *dictionary = nullptr;
      if (dictionary_coded_attributes_[comparison_attribute_id]) {
        dictionary = static_cast<const CompressionDictionary*>(
            dictionaries_.find(comparison_attribute_id)->second);
      }
      PredicateTransformResult result
          = CompressedAttributePredicateTransformer::TransformPredicateOnCompressedAttribute(
              relation_,
              predicate,
              dictionary,
              GetMaxTruncatedValue(compression_info_.attribute_size(comparison_attribute_id)));

      pair<uint32_t, uint32_t> match_range;
      switch (result.type) {
        case PredicateTransformResultType::kAll:
          if (filter == nullptr) {
            return getExistenceMap();
          } else {
            TupleIdSequence *filter_copy = new TupleIdSequence(filter->length());
            filter_copy->assignFrom(*filter);
            return filter_copy;
          }
          // Pass through to base version to get all tuples.
          return TupleStorageSubBlock::getMatchesForPredicate(predicate, filter);
        case PredicateTransformResultType::kNone:
          // No matches.
          return new TupleIdSequence(*static_cast<const tuple_id*>(sub_block_memory_));
        case PredicateTransformResultType::kBasicComparison:
          switch (result.comp) {
            case ComparisonID::kEqual:
              return getEqualCodes(comparison_attribute_id, result.first_literal, filter);
            case ComparisonID::kNotEqual:
              if (result.exclude_nulls) {
                return getNotEqualCodesExcludingNull(comparison_attribute_id,
                                                     result.first_literal,
                                                     result.second_literal,
                                                     filter);
              } else {
                return getNotEqualCodes(comparison_attribute_id,
                                        result.first_literal,
                                        filter);
              }
            case ComparisonID::kLess:
              return getLessCodes(comparison_attribute_id, result.first_literal, filter);
            case ComparisonID::kGreaterOrEqual:
              return getGreaterOrEqualCodes(comparison_attribute_id,
                                            result.first_literal,
                                            filter);
            default:
              FATAL_ERROR("Unexpected ComparisonID in CompressedTupleStorageSubBlock::"
                          "getMatchesForPredicate()");
          }
        case PredicateTransformResultType::kRangeComparison:
          match_range.first = result.first_literal;
          match_range.second = result.second_literal;
          return getCodesInRange(comparison_attribute_id, match_range, filter);
        default:
          FATAL_ERROR("Unexpected PredicateTransformResultType in CompressedTupleStorageSubBlock::"
                      "getMatchesForPredicate()");
      }
    } else {
      FATAL_ERROR("Called CompressedTupleStorageSubBlock::getMatchesForPredicate() "
                  "with a predicate that can only be evaluated with a simple scan.");
    }
  } else {
    FATAL_ERROR("Called CompressedTupleStorageSubBlock::getMatchesForPredicate() "
                "with a predicate that can only be evaluated with a simple scan.");
  }
}

bool CompressedTupleStorageSubBlock::compressedComparisonIsAlwaysTrueForTruncatedAttribute(
    const ComparisonID comp,
    const attribute_id left_attr_id,
    const TypedValue &right_literal,
    const Type &right_literal_type) const {
  DEBUG_ASSERT(truncated_attributes_[left_attr_id]);

  return CompressedAttributePredicateTransformer::CompressedComparisonIsAlwaysTrueForTruncatedAttribute(
      comp,
      right_literal,
      right_literal_type,
      GetMaxTruncatedValue(compression_info_.attribute_size(left_attr_id)));
}

bool CompressedTupleStorageSubBlock::compressedComparisonIsAlwaysFalseForTruncatedAttribute(
    const ComparisonID comp,
    const attribute_id left_attr_id,
    const TypedValue &right_literal,
    const Type &right_literal_type) const {
  DEBUG_ASSERT(truncated_attributes_[left_attr_id]);

  return CompressedAttributePredicateTransformer::CompressedComparisonIsAlwaysFalseForTruncatedAttribute(
      comp,
      right_literal,
      right_literal_type,
      GetMaxTruncatedValue(compression_info_.attribute_size(left_attr_id)));
}

void* CompressedTupleStorageSubBlock::initializeCommon() {
  if (!compression_info_.ParseFromArray(
      static_cast<const char*>(sub_block_memory_) + sizeof(tuple_id) + sizeof(int),
      *reinterpret_cast<const int*>(static_cast<const char*>(sub_block_memory_) + sizeof(tuple_id)))) {
    throw MalformedBlock();
  }

  if ((relation_.getMaxAttributeId() + 1 != compression_info_.attribute_size_size())
      || (relation_.getMaxAttributeId() + 1 != compression_info_.dictionary_size_size())) {
    throw MalformedBlock();
  }

  dictionary_coded_attributes_.resize(relation_.getMaxAttributeId() + 1, false);
  truncated_attributes_.resize(relation_.getMaxAttributeId() + 1, false);
  truncated_attribute_is_int_.resize(relation_.getMaxAttributeId() + 1, false);
  size_t dictionary_offset =
      sizeof(tuple_id) + sizeof(int)
      + *reinterpret_cast<const int*>(static_cast<const char*>(sub_block_memory_) + sizeof(tuple_id));
  for (CatalogRelationSchema::const_iterator attr_it = relation_.begin();
       attr_it != relation_.end();
       ++attr_it) {
    const Type &attr_type = attr_it->getType();
    if (attr_type.isVariableLength()) {
      if (compression_info_.dictionary_size(attr_it->getID()) == 0) {
        throw MalformedBlock();
      }
    }

    if (compression_info_.dictionary_size(attr_it->getID()) > 0) {
      dictionaries_.insert(
          attr_it->getID(),
          new CompressionDictionary(
              attr_type,
              static_cast<const char*>(sub_block_memory_) + dictionary_offset,
              compression_info_.dictionary_size(attr_it->getID())));

      dictionary_coded_attributes_[attr_it->getID()] = true;
      dictionary_offset += compression_info_.dictionary_size(attr_it->getID());
    } else if (compression_info_.attribute_size(attr_it->getID())
               != attr_type.maximumByteLength()) {
      switch (attr_type.getTypeID()) {
        case kInt:
        case kLong:
          switch (compression_info_.attribute_size(attr_it->getID())) {
            case 1:
            case 2:
            case 4:
              truncated_attributes_[attr_it->getID()] = true;
              truncated_attribute_is_int_[attr_it->getID()] = (attr_type.getTypeID() == kInt);
              break;
            default:
              throw MalformedBlock();
          }
          break;
        default:
          throw MalformedBlock();
      }
    }
  }

  return static_cast<char*>(sub_block_memory_) + dictionary_offset;
}

}  // namespace quickstep
