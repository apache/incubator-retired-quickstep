/**
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

#include "storage/BloomFilterIndexSubBlock.hpp"

#include <cstdint>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/predicate/ComparisonPredicate.hpp"
#include "expressions/predicate/PredicateCost.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "expressions/scalar/ScalarLiteral.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageConstants.hpp"
#include "storage/SubBlockTypeRegistry.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/BloomFilter.hpp"

#include "glog/logging.h"

namespace quickstep {

QUICKSTEP_REGISTER_INDEX(BloomFilterIndexSubBlock, BLOOM_FILTER);

BloomFilterIndexSubBlock::BloomFilterIndexSubBlock(const TupleStorageSubBlock &tuple_store,
                                                   const IndexSubBlockDescription &description,
                                                   const bool is_new_block,
                                                   void *sub_block_memory,
                                                   const std::size_t sub_block_memory_size)
    : IndexSubBlock(tuple_store,
                    description,
                    is_new_block,
                    sub_block_memory,
                    sub_block_memory_size),
      is_initialized_(false),
      is_consistent_(false),
      random_seed_(kBloomFilterSeed),
      bit_array_size_in_bytes_(description.GetExtension(
                                   BloomFilterIndexSubBlockDescription::bloom_filter_size)) {
  CHECK(DescriptionIsValid(relation_, description_))
      << "Attempted to construct an BloomFilterIndexSubBlock from an invalid description.";

  // Store the attribute ids that are being indexed.
  indexed_attribute_ids_.reserve(description.indexed_attribute_ids_size());
  for (int i = 0; i < description.indexed_attribute_ids_size(); ++i) {
    indexed_attribute_ids_.push_back(description.indexed_attribute_ids(i));
  }

  // Make the bit_array_ point to sub_block_memory.
  bit_array_.reset(static_cast<std::uint8_t*>(sub_block_memory));

  bool is_bloom_filter_initialized = !is_new_block;
  const std::uint32_t salt_count = description.GetExtension(BloomFilterIndexSubBlockDescription::number_of_hashes);

  // Initialize the bloom_filter_ data structure to operate on bit_array.
  bloom_filter_.reset(new BloomFilter(random_seed_,
                                      salt_count,
                                      bit_array_size_in_bytes_,
                                      bit_array_.get(),
                                      is_bloom_filter_initialized));
  is_initialized_ = true;
  is_consistent_ = true;
}

BloomFilterIndexSubBlock::~BloomFilterIndexSubBlock() {
  bit_array_.release();  // De-allocation of bit_array_ is handled by StorageBlock.
}

bool BloomFilterIndexSubBlock::DescriptionIsValid(const CatalogRelationSchema &relation,
                                         const IndexSubBlockDescription &description) {
  if (!description.IsInitialized()) {
    return false;
  }

  if (description.sub_block_type() != IndexSubBlockDescription::BLOOM_FILTER) {
    return false;
  }

  // Check that all the specified index attributes are valid.
  for (int i = 0; i < description.indexed_attribute_ids_size(); ++i) {
    const attribute_id indexed_attribute_id = description.indexed_attribute_ids(i);
    if (!relation.hasAttributeWithId(indexed_attribute_id)) {
      return false;
    }
  }

  return true;
}

std::size_t BloomFilterIndexSubBlock::EstimateBytesPerTuple(const CatalogRelationSchema &relation,
                                                            const IndexSubBlockDescription &description) {
  DCHECK(DescriptionIsValid(relation, description));
  // Note: Returning zero here causes EstimteBytesPerBlock() to be invoked for size computation.
  return kZeroSize;
}

std::size_t BloomFilterIndexSubBlock::EstimateBytesPerBlock(const CatalogRelationSchema &relation,
                                                            const IndexSubBlockDescription &description) {
  // Note: This function is only invoked when EstimateBytesPerTuple() returns zero.
  DCHECK(DescriptionIsValid(relation, description));
  return description.GetExtension(BloomFilterIndexSubBlockDescription::bloom_filter_size);
}

bool BloomFilterIndexSubBlock::addEntry(const tuple_id tuple) {
  DCHECK(is_initialized_);
  if (!is_consistent_) {
    return false;
  }
  if (!tuple_store_.hasTupleWithID(tuple)) {
    return false;
  }
  for (std::size_t i = 0; i < indexed_attribute_ids_.size(); ++i) {
    const attribute_id indexed_attribute_id = indexed_attribute_ids_[i];
    const TypedValue tuple_value = tuple_store_.getAttributeValueTyped(tuple, indexed_attribute_id);
    const std::uint8_t *data = static_cast<const std::uint8_t*>(tuple_value.getDataPtr());
    const std::size_t data_size = tuple_value.getDataSize();
    if (!tuple_value.isNull()) {
      bloom_filter_->insert(data, data_size);
    }
  }
  return true;
}

bool BloomFilterIndexSubBlock::bulkAddEntries(const TupleIdSequence &tuples) {
  DCHECK(is_initialized_);
  if (!is_consistent_) {
    return false;
  }
  bool didAllSucceed = true;
  for (const tuple_id &tuple : tuples) {
    didAllSucceed = didAllSucceed && addEntry(tuple);
  }
  return didAllSucceed;
}

void BloomFilterIndexSubBlock::removeEntry(const tuple_id tuple) {
  is_consistent_ = false;
}

void BloomFilterIndexSubBlock::bulkRemoveEntries(const TupleIdSequence &tuples) {
  is_consistent_ = false;
}

predicate_cost_t BloomFilterIndexSubBlock::estimatePredicateEvaluationCost(
    const ComparisonPredicate &predicate) const {
  DCHECK(is_initialized_);
  BloomFilterSelectivity selectivity = getSelectivityForPredicate(predicate);
  // Note: A Bloomfilter index is only useful when it gives a zero selectivity
  //       in which case a block can be skipped entirely.
  if (selectivity == BloomFilterSelectivity::kSelectivityNone) {
    // Return minimum cost so that this block can be skipped altogether.
    return predicate_cost::kConstantTime;
  } else {
    // Return maximum cost because this is as worse as doing a full scan of the block.
    return predicate_cost::kInfinite;
  }
}

TupleIdSequence* BloomFilterIndexSubBlock::getMatchesForPredicate(
    const ComparisonPredicate &predicate,
    const TupleIdSequence *filter) const {
  DCHECK(is_initialized_);
  if (filter != nullptr) {
    LOG(FATAL) << "BloomFilterIndex does not support filter evaluation with predicate.";
  }
  BloomFilterSelectivity selectivity = getSelectivityForPredicate(predicate);
  if (selectivity == BloomFilterSelectivity::kSelectivityNone) {
    // This path is invoked when there is a BloomFilter miss (true negative)
    // and a BloomFilter index allows to skip this block entirely.
    // A new tuple ID sequence is initialized to false for all values.
    return new TupleIdSequence(tuple_store_.numTuples());
  } else if (selectivity == BloomFilterSelectivity::kSelectivityAll) {
    // This path is invoked when there is a BloomFilter hit (possibly false positive)
    // and a BloomFilter index does not provide much information.
    // Note: If estimatePredicateEvaluationCost() is used by optimizer to evaluate
    //       the cost of using BloomFilter index for a BloomFilter hit,
    //       getMatchesForPredicate() will not be called in the first place itself
    //       and this path will never be taken.
    TupleIdSequence* tuple_sequence = new TupleIdSequence(tuple_store_.numTuples());
    // Set all existing tuples to true, selected.
    if (tuple_store_.isPacked()) {
      tuple_sequence->setRange(0, tuple_store_.numTuples(), true);
    } else {
      for (tuple_id tid = 0; tid <= tuple_store_.getMaxTupleID(); ++tid) {
        if (tuple_store_.hasTupleWithID(tid)) {
          tuple_sequence->set(tid, true);
        }
      }
    }
    return tuple_sequence;
  } else {
    LOG(FATAL) << "BloomFilterIndex should never have been called.";
    return nullptr;
  }
}

BloomFilterIndexSubBlock::BloomFilterSelectivity
    BloomFilterIndexSubBlock::getSelectivityForPredicate(const ComparisonPredicate &predicate) const {
  DCHECK(is_initialized_);
  if (!is_consistent_) {
    return BloomFilterSelectivity::kSelectivityUnknown;
  }
  // Bloom filter index can be currently only used to evaluate
  // equality matching comparison predicates with static values as the right operand.
  if (predicate.getComparison().getComparisonID() == ComparisonID::kEqual
      && predicate.getRightOperand().hasStaticValue()) {
    const ScalarAttribute *left_operand = static_cast<const ScalarAttribute*>(&predicate.getLeftOperand());
    const attribute_id attr_id_checked = left_operand->getAttribute().getID();

    // Check if the specified attribute in the predicate is actually indexed or not.
    bool is_attribute_indexed = false;
    for (const attribute_id &attr_id_expected : indexed_attribute_ids_) {
      if (attr_id_expected == attr_id_checked) {
        is_attribute_indexed = true;
        break;
      }
    }
    if (!is_attribute_indexed) {
      // Specified attribute not indexed, this index provides no useful information.
      return BloomFilterSelectivity::kSelectivityUnknown;
    }

    const ScalarLiteral *right_operand = static_cast<const ScalarLiteral*>(&predicate.getRightOperand());
    const std::uint8_t *test_data = static_cast<const std::uint8_t*>(right_operand->getStaticValue().getDataPtr());
    const std::size_t test_data_size = right_operand->getStaticValue().getDataSize();
    bool is_contained = bloom_filter_->contains(test_data, test_data_size);

    if (is_contained) {
      // Bloom filter hit. Can be a false positive, a scan is required here.
      return BloomFilterSelectivity::kSelectivityAll;
    } else {
      // Bloom filter miss. Definitely a true negative. Selectivity is zero.
      return BloomFilterSelectivity::kSelectivityNone;
    }
  }
  return BloomFilterSelectivity::kSelectivityUnknown;
}

bool BloomFilterIndexSubBlock::rebuild() {
  DCHECK(is_initialized_);
  bloom_filter_->reset();
  bool didSucceed = true;
  if (tuple_store_.isPacked()) {
    for (tuple_id tid = 0; didSucceed && tid <= tuple_store_.getMaxTupleID(); ++tid) {
      didSucceed = addEntry(tid);
    }
  } else {
    for (tuple_id tid = 0; didSucceed && tid <= tuple_store_.getMaxTupleID(); ++tid) {
      if (tuple_store_.hasTupleWithID(tid)) {
        didSucceed = addEntry(tid);
      }
    }
  }
  is_consistent_ = true;
  return didSucceed;
}

}  // namespace quickstep
