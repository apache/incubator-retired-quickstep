/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#include "storage/StorageBlock.hpp"

#include <climits>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/aggregation/AggregationHandle.hpp"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "storage/BasicColumnStoreTupleStorageSubBlock.hpp"
#include "storage/BloomFilterIndexSubBlock.hpp"
#include "storage/CSBTreeIndexSubBlock.hpp"
#include "storage/CompressedColumnStoreTupleStorageSubBlock.hpp"
#include "storage/CompressedPackedRowStoreTupleStorageSubBlock.hpp"
#include "storage/CountedReference.hpp"
#include "storage/HashTableBase.hpp"
#include "storage/IndexSubBlock.hpp"
#include "storage/InsertDestinationInterface.hpp"
#include "storage/PackedRowStoreTupleStorageSubBlock.hpp"
#include "storage/SMAIndexSubBlock.hpp"
#include "storage/SplitRowStoreTupleStorageSubBlock.hpp"
#include "storage/StorageBlockBase.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageConfig.h"
#include "storage/StorageErrors.hpp"
#include "storage/SubBlocksReference.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "types/containers/Tuple.hpp"
#include "types/operations/comparisons/ComparisonUtil.hpp"
#include "utility/BloomFilter.hpp"
#include "utility/BloomFilterAdapter.hpp"
#include "utility/Macros.hpp"

#include "gflags/gflags.h"

#include "glog/logging.h"

#ifdef QUICKSTEP_HAVE_BITWEAVING
#include "storage/bitweaving/BitWeavingIndexSubBlock.hpp"
#include "storage/bitweaving/BitWeavingHIndexSubBlock.hpp"
#include "storage/bitweaving/BitWeavingVIndexSubBlock.hpp"
#endif

using std::make_pair;
using std::pair;
using std::size_t;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

namespace quickstep {

DECLARE_int64(bloom_adapter_batch_size);

class Type;

StorageBlock::StorageBlock(const CatalogRelationSchema &relation,
                           const block_id id,
                           const StorageBlockLayout &layout,
                           const bool new_block,
                           void *block_memory,
                           const std::size_t block_memory_size)
    : StorageBlockBase(id, block_memory, block_memory_size),
      all_indices_consistent_(true),
      all_indices_inconsistent_(false),
      relation_(relation) {
  if (new_block) {
    if (block_memory_size_ < layout.getBlockHeaderSize()) {
      throw BlockMemoryTooSmall("StorageBlock", block_memory_size_);
    }

    layout.copyHeaderTo(block_memory_);
    DEBUG_ASSERT(*static_cast<const int*>(block_memory_) > 0);

    if (!block_header_.ParseFromArray(static_cast<char*>(block_memory_) + sizeof(int),
                                      *static_cast<const int*>(block_memory_))) {
      FATAL_ERROR("A StorageBlockLayout created a malformed StorageBlockHeader.");
    }

    // We mark a newly-created block as dirty, so that in the rare case that a
    // block is evicted before anything is inserted into it, we still write it
    // (and the header plus any sub-block specific fixed data structures) back
    // to disk.
    dirty_ = true;

    DEBUG_ASSERT(block_header_.IsInitialized());
    DEBUG_ASSERT(StorageBlockLayout::DescriptionIsValid(relation_, block_header_.layout()));
    DEBUG_ASSERT(block_header_.index_size_size() == block_header_.layout().index_description_size());
    DEBUG_ASSERT(block_header_.index_size_size() == block_header_.index_consistent_size());
  } else {
    if (block_memory_size < sizeof(int)) {
      throw MalformedBlock();
    }
    if (*static_cast<const int*>(block_memory_) <= 0) {
      throw MalformedBlock();
    }
    if (*static_cast<const int*>(block_memory_) + sizeof(int) > block_memory_size_) {
      throw MalformedBlock();
    }

    if (!block_header_.ParseFromArray(static_cast<char*>(block_memory_) + sizeof(int),
                                      *static_cast<const int*>(block_memory_))) {
      throw MalformedBlock();
    }
    if (!block_header_.IsInitialized()) {
      throw MalformedBlock();
    }
    if (!StorageBlockLayout::DescriptionIsValid(relation_, block_header_.layout())) {
      throw MalformedBlock();
    }
    if (block_header_.index_size_size() != block_header_.layout().index_description_size()) {
      throw MalformedBlock();
    }
    if (block_header_.index_size_size() != block_header_.index_consistent_size()) {
      throw MalformedBlock();
    }
  }

  size_t block_size_from_metadata = *static_cast<const int*>(block_memory_) + sizeof(int);
  block_size_from_metadata += block_header_.tuple_store_size();
  for (int index_num = 0;
       index_num < block_header_.index_size_size();
       ++index_num) {
    block_size_from_metadata += block_header_.index_size(index_num);
  }

  if (block_size_from_metadata > block_memory_size_) {
    throw MalformedBlock();
  } else if (block_size_from_metadata < block_memory_size_) {
    // WARNING: this isn't strictly an error, but it does indicate that there
    // is unallocated space in the block.
  }

  char *sub_block_address = static_cast<char*>(block_memory_)
                            + sizeof(int)
                            + *static_cast<const int*>(block_memory_);
  tuple_store_.reset(CreateTupleStorageSubBlock(
      relation_,
      block_header_.layout().tuple_store_description(),
      new_block,
      sub_block_address,
      block_header_.tuple_store_size()));
  sub_block_address += block_header_.tuple_store_size();
  ad_hoc_insert_supported_ = tuple_store_->supportsAdHocInsert();
  ad_hoc_insert_efficient_ = tuple_store_->adHocInsertIsEfficient();

  if (block_header_.index_size_size() > 0) {
    all_indices_inconsistent_ = true;
  }
  for (int index_num = 0;
       index_num < block_header_.index_size_size();
       ++index_num) {
    indices_.push_back(CreateIndexSubBlock(*tuple_store_,
                                           block_header_.layout().index_description(index_num),
                                           new_block, sub_block_address,
                                           block_header_.index_size(index_num)));
    sub_block_address += block_header_.index_size(index_num);
    if (!indices_.back().supportsAdHocAdd()) {
      ad_hoc_insert_efficient_ = false;
    }

    if (block_header_.index_consistent(index_num)) {
      indices_consistent_.push_back(true);
      all_indices_inconsistent_ = false;
    } else {
      indices_consistent_.push_back(false);
      all_indices_consistent_ = false;
    }
  }
}

bool StorageBlock::insertTuple(const Tuple &tuple) {
  if (!ad_hoc_insert_supported_) {
    return false;
  }

  const bool empty_before = tuple_store_->isEmpty();

  TupleStorageSubBlock::InsertResult tuple_store_insert_result = tuple_store_->insertTuple(tuple);
  if (tuple_store_insert_result.inserted_id < 0) {
    DEBUG_ASSERT(tuple_store_insert_result.ids_mutated == false);
    if (empty_before) {
      throw TupleTooLargeForBlock(tuple.getByteSize());
    } else {
      return false;
    }
  }

  bool update_succeeded = true;

  if (tuple_store_insert_result.ids_mutated) {
    update_succeeded = rebuildIndexes(true);
    if (!update_succeeded) {
      tuple_store_->deleteTuple(tuple_store_insert_result.inserted_id);
      if (!rebuildIndexes(true)) {
        // It should always be possible to rebuild an index with the tuples
        // which it originally contained.
        FATAL_ERROR("Rebuilding an IndexSubBlock failed after removing tuples.");
      }
    }
  } else {
    update_succeeded = insertEntryInIndexes(tuple_store_insert_result.inserted_id);
  }

  if (update_succeeded) {
    dirty_ = true;
    return true;
  } else {
    if (empty_before) {
      throw TupleTooLargeForBlock(tuple.getByteSize());
    } else {
      return false;
    }
  }
}

bool StorageBlock::insertTupleInBatch(const Tuple &tuple) {
  if (tuple_store_->insertTupleInBatch(tuple)) {
    invalidateAllIndexes();
    dirty_ = true;
    return true;
  } else {
    if (tuple_store_->isEmpty()) {
      throw TupleTooLargeForBlock(tuple.getByteSize());
    } else {
      return false;
    }
  }
}

tuple_id StorageBlock::bulkInsertTuples(ValueAccessor *accessor) {
  const tuple_id num_inserted = tuple_store_->bulkInsertTuples(accessor);
  if (num_inserted != 0) {
    invalidateAllIndexes();
    dirty_ = true;
  } else if (tuple_store_->isEmpty()) {
    if (!accessor->iterationFinishedVirtual()) {
      throw TupleTooLargeForBlock(0);
    }
  }
  return num_inserted;
}

tuple_id StorageBlock::bulkInsertTuplesWithRemappedAttributes(
    const std::vector<attribute_id> &attribute_map,
    ValueAccessor *accessor) {
  const tuple_id num_inserted
      = tuple_store_->bulkInsertTuplesWithRemappedAttributes(attribute_map,
                                                             accessor);
  if (num_inserted != 0) {
    invalidateAllIndexes();
    dirty_ = true;
  } else if (tuple_store_->isEmpty()) {
    if (!accessor->iterationFinishedVirtual()) {
      throw TupleTooLargeForBlock(0);
    }
  }
  return num_inserted;
}

void StorageBlock::sample(const bool is_block_sample,
                          const int percentage,
                          InsertDestinationInterface *destination) const {
  std::unique_ptr<ValueAccessor> accessor;
  // Bulk insert if the sampling method is block sample or if the sample
  // percent is 100
  if (is_block_sample || percentage == 100) {
    accessor.reset(tuple_store_->createValueAccessor());
    destination->bulkInsertTuples(accessor.get());
  } else {
    int actual_percentage = percentage;
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::unique_ptr<TupleIdSequence> sequence;
    // Get the tuple id sequence
    sequence.reset(tuple_store_->getExistenceMap());
    // Initialize a new TupleIdSequence from the existing sequence
    // but with a default bit vector of all zeros
    std::unique_ptr<TupleIdSequence> sequence_mask(
        new TupleIdSequence(sequence->length()));
    std::unordered_map<std::size_t, std::size_t> tuple_index_mapping;
    bool invert_bits = false;

    // If we have to set more number of 1 in the TupleIdSequence
    // calculate the number of 0's to be set(corresponding to the
    // rows that will not be picked) and set those rows to 1.
    if (percentage > 50) {
      actual_percentage = 100 - percentage;
      invert_bits = true;
    }
    const std::size_t num_tuples = sequence->length() * actual_percentage / 100;

    // Total possible tuple id space (N) is given by the length of
    // TupleIdSequence
    // Generate a random number between 0 and N
    for (std::size_t n = 0; n < num_tuples; n++) {
        std::uniform_real_distribution<> distribution(0, sequence->length() - (n + 1));
        std::size_t random_number = distribution(generator);
        sequence_mask->set(tuple_index_mapping.find(random_number) ==
                                   tuple_index_mapping.end()
                               ? random_number
                               : tuple_index_mapping[random_number],
                           true);
        tuple_index_mapping[random_number] = sequence->length() - (n + 1);
    }

    // Since the bits set correspond to actual zeros invert the bitvector
    if (invert_bits) {
      sequence_mask->invert();
    }
    accessor.reset(tuple_store_->createValueAccessor(sequence_mask.get()));
    destination->bulkInsertTuples(accessor.get());
  }
}

void StorageBlock::select(const vector<unique_ptr<const Scalar>> &selection,
                          const Predicate *predicate,
                          const std::vector<const BloomFilter *> &bloom_filters,
                          const std::vector<attribute_id> &bloom_filter_attribute_ids,
                          InsertDestinationInterface *destination) const {
  ColumnVectorsValueAccessor temp_result;
  {
    SubBlocksReference sub_blocks_ref(*tuple_store_,
                                      indices_,
                                      indices_consistent_);

    std::unique_ptr<TupleIdSequence> matches;
    std::unique_ptr<ValueAccessor> accessor;

    if (bloom_filters.size() > 0) {
      const std::size_t num_tuples = tuple_store_->numTuples();
      matches.reset(new TupleIdSequence(num_tuples));
//      std::cerr << "Before: " << num_tuples << "\n";
      accessor.reset(tuple_store_->createValueAccessor());
      InvokeOnAnyValueAccessor(
          accessor.get(),
          [&](auto *accessor) -> void {  // NOLINT(build/c++11)
        std::vector<std::size_t> attr_size_vector;
        attr_size_vector.reserve(bloom_filter_attribute_ids.size());
        for (const auto &attr : bloom_filter_attribute_ids) {
          auto val_and_size =
              accessor->template getUntypedValueAndByteLengthAtAbsolutePosition<false>(0, attr);
          attr_size_vector.emplace_back(val_and_size.second);
        }

        std::unique_ptr<BloomFilterAdapter> bloom_filter_adapter;
        bloom_filter_adapter.reset(new BloomFilterAdapter(
            bloom_filters, bloom_filter_attribute_ids, attr_size_vector));

        std::uint32_t batch_size_try = FLAGS_bloom_adapter_batch_size;
        std::uint32_t num_tuples_left = accessor->getNumTuples();
        std::vector<tuple_id> batch(num_tuples_left);

        do {
          std::uint32_t batch_size =
              batch_size_try < num_tuples_left ? batch_size_try : num_tuples_left;
          for (std::size_t i = 0; i < batch_size; ++i) {
            accessor->next();
            batch.push_back(accessor->getCurrentPosition());
          }

          std::size_t num_hits =
              bloom_filter_adapter->bulkProbe<true>(accessor, batch, batch_size);
          for (std::size_t t = 0; t < num_hits; ++t){
            matches->set(batch[t], true);
          }

          batch.clear();
          num_tuples_left -= batch_size;
          batch_size_try = batch_size * 2;
        } while (num_tuples_left > 0);
      });
//      std::cerr << "After: " << matches->numTuples() << "\n";
    }

    if (predicate == nullptr) {
      accessor.reset(tuple_store_->createValueAccessor(matches.get()));
    } else {
      auto *new_matches = getMatchesForPredicate(predicate, matches.get());
      matches.reset(new_matches);
      accessor.reset(tuple_store_->createValueAccessor(matches.get()));
    }

    for (vector<unique_ptr<const Scalar>>::const_iterator selection_cit = selection.begin();
         selection_cit != selection.end();
         ++selection_cit) {
      // TODO(chasseur): Can probably elide some copies for parts of the
      // selection that are ScalarAttribute or ScalarLiteral.
      temp_result.addColumn((*selection_cit)->getAllValues(accessor.get(), &sub_blocks_ref));
    }
  }

  destination->bulkInsertTuples(&temp_result);
}

void StorageBlock::selectSimple(const std::vector<attribute_id> &selection,
                                const Predicate *predicate,
                                const std::vector<const BloomFilter *> &bloom_filters,
                                const std::vector<attribute_id> &bloom_filter_attribute_ids,
                                InsertDestinationInterface *destination) const {
  std::unique_ptr<ValueAccessor> accessor;
  std::unique_ptr<TupleIdSequence> matches;

  if (bloom_filters.size() > 0) {
    const std::size_t num_tuples = tuple_store_->numTuples();
    matches.reset(new TupleIdSequence(num_tuples));
//    std::cerr << "Before: " << num_tuples << "\n";
    accessor.reset(tuple_store_->createValueAccessor());
    InvokeOnAnyValueAccessor(
        accessor.get(),
        [&](auto *accessor) -> void {  // NOLINT(build/c++11)
      std::vector<std::size_t> attr_size_vector;
      attr_size_vector.reserve(bloom_filter_attribute_ids.size());
      for (const auto &attr : bloom_filter_attribute_ids) {
        auto val_and_size =
            accessor->template getUntypedValueAndByteLengthAtAbsolutePosition<false>(0, attr);
        attr_size_vector.emplace_back(val_and_size.second);
      }

      std::unique_ptr<BloomFilterAdapter> bloom_filter_adapter;
      bloom_filter_adapter.reset(new BloomFilterAdapter(
          bloom_filters, bloom_filter_attribute_ids, attr_size_vector));

      std::uint32_t batch_size_try = FLAGS_bloom_adapter_batch_size;
      std::uint32_t num_tuples_left = accessor->getNumTuples();
      std::vector<tuple_id> batch(num_tuples_left);

      do {
        std::uint32_t batch_size =
            batch_size_try < num_tuples_left ? batch_size_try : num_tuples_left;
        for (std::size_t i = 0; i < batch_size; ++i) {
          accessor->next();
          batch.push_back(accessor->getCurrentPosition());
        }

        std::size_t num_hits =
            bloom_filter_adapter->bulkProbe<true>(accessor, batch, batch_size);
        for (std::size_t t = 0; t < num_hits; ++t){
          matches->set(batch[t], true);
        }

        batch.clear();
        num_tuples_left -= batch_size;
        batch_size_try = batch_size * 2;
      } while (num_tuples_left > 0);
    });
//    std::cerr << "After: " << matches->numTuples() << "\n";
  }

  if (predicate == nullptr) {
    accessor.reset(tuple_store_->createValueAccessor(matches.get()));
  } else {
    auto *new_matches = getMatchesForPredicate(predicate, matches.get());
    matches.reset(new_matches);
    accessor.reset(tuple_store_->createValueAccessor(matches.get()));
  }

  destination->bulkInsertTuplesWithRemappedAttributes(selection,
                                                      accessor.get());
}

AggregationState* StorageBlock::aggregate(
    const AggregationHandle &handle,
    const std::vector<std::unique_ptr<const Scalar>> &arguments,
    const std::vector<attribute_id> *arguments_as_attributes,
    std::unique_ptr<TupleIdSequence> *reuse_matches) const {
#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  // If all the arguments to this aggregate are plain relation attributes,
  // aggregate directly on a ValueAccessor from this block to avoid a copy.
  if ((arguments_as_attributes != nullptr) && (!arguments_as_attributes->empty())) {
    DCHECK_EQ(arguments.size(), arguments_as_attributes->size())
        << "Mismatch between number of arguments and number of attribute_ids";
    return aggregateHelperValueAccessor(handle, *arguments_as_attributes, reuse_matches->get());
  }
  // TODO(shoban): We may want to optimize for ScalarLiteral here.
#endif

  // Call aggregateHelperColumnVector() to materialize each argument as a
  // ColumnVector, then aggregate over those.
  return aggregateHelperColumnVector(handle, arguments, reuse_matches->get());
}

void StorageBlock::aggregateGroupBy(
    const AggregationHandle &handle,
    const std::vector<std::unique_ptr<const Scalar>> &arguments,
    const std::vector<std::unique_ptr<const Scalar>> &group_by,
    AggregationStateHashTableBase *hash_table,
    std::unique_ptr<TupleIdSequence> *reuse_matches,
    std::vector<std::unique_ptr<ColumnVector>> *reuse_group_by_vectors) const {
  DCHECK_GT(group_by.size(), 0u)
      << "Called aggregateGroupBy() with zero GROUP BY expressions";

  SubBlocksReference sub_blocks_ref(*tuple_store_,
                                    indices_,
                                    indices_consistent_);

  // IDs of 'arguments' as attributes in the ValueAccessor we create below.
  std::vector<attribute_id> argument_ids;

  // IDs of GROUP BY key element(s) in the ValueAccessor we create below.
  std::vector<attribute_id> key_ids;

  // An intermediate ValueAccessor that stores the materialized 'arguments' for
  // this aggregate, as well as the GROUP BY expression values.
  ColumnVectorsValueAccessor temp_result;
  {
    std::unique_ptr<ValueAccessor> accessor;
    if (reuse_matches) {
      // Create a filtered ValueAccessor that only iterates over predicate
      // matches.
      accessor.reset(tuple_store_->createValueAccessor(reuse_matches->get()));
    } else {
      // Create a ValueAccessor that iterates over all tuples in this block
      accessor.reset(tuple_store_->createValueAccessor());
    }

    attribute_id attr_id = 0;

    // First, put GROUP BY keys into 'temp_result'.
    if (reuse_group_by_vectors->empty()) {
      // Compute GROUP BY values from group_by Scalars, and store them in
      // reuse_group_by_vectors for reuse by other aggregates on this same
      // block.
      reuse_group_by_vectors->reserve(group_by.size());
      for (const std::unique_ptr<const Scalar> &group_by_element : group_by) {
        reuse_group_by_vectors->emplace_back(
            group_by_element->getAllValues(accessor.get(), &sub_blocks_ref));
        temp_result.addColumn(reuse_group_by_vectors->back().get(), false);
        key_ids.push_back(attr_id++);
      }
    } else {
      // Reuse precomputed GROUP BY values from reuse_group_by_vectors.
      DCHECK_EQ(group_by.size(), reuse_group_by_vectors->size())
          << "Wrong number of reuse_group_by_vectors";
      for (const std::unique_ptr<ColumnVector> &reuse_cv : *reuse_group_by_vectors) {
        temp_result.addColumn(reuse_cv.get(), false);
        key_ids.push_back(attr_id++);
      }
    }

    // Compute argument vectors and add them to 'temp_result'.
    for (const std::unique_ptr<const Scalar> &argument : arguments) {
      temp_result.addColumn(argument->getAllValues(accessor.get(), &sub_blocks_ref));
      argument_ids.push_back(attr_id++);
    }
  }

  // Actually do aggregation into '*hash_table'.
  handle.aggregateValueAccessorIntoHashTable(&temp_result,
                                             argument_ids,
                                             key_ids,
                                             hash_table);
}

void StorageBlock::aggregateDistinct(
    const AggregationHandle &handle,
    const std::vector<std::unique_ptr<const Scalar>> &arguments,
    const std::vector<attribute_id> *arguments_as_attributes,
    const std::vector<std::unique_ptr<const Scalar>> &group_by,
    AggregationStateHashTableBase *distinctify_hash_table,
    std::unique_ptr<TupleIdSequence> *reuse_matches,
    std::vector<std::unique_ptr<ColumnVector>> *reuse_group_by_vectors) const {
  DCHECK_GT(arguments.size(), 0u)
      << "Called aggregateDistinct() with zero argument expressions";
  DCHECK((group_by.size() == 0 || reuse_group_by_vectors != nullptr));

  std::vector<attribute_id> key_ids;

  // An intermediate ValueAccessor that stores the materialized 'arguments' for
  // this aggregate, as well as the GROUP BY expression values.
  ColumnVectorsValueAccessor temp_result;
  {
    std::unique_ptr<ValueAccessor> accessor;
    if (reuse_matches) {
      // Create a filtered ValueAccessor that only iterates over predicate
      // matches.
      accessor.reset(tuple_store_->createValueAccessor(reuse_matches->get()));
    } else {
      // Create a ValueAccessor that iterates over all tuples in this block
      accessor.reset(tuple_store_->createValueAccessor());
    }

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
    // If all the arguments to this aggregate are plain relation attributes,
    // aggregate directly on a ValueAccessor from this block to avoid a copy.
    if ((arguments_as_attributes != nullptr) && (!arguments_as_attributes->empty())) {
      DCHECK_EQ(arguments.size(), arguments_as_attributes->size())
          << "Mismatch between number of arguments and number of attribute_ids";
      DCHECK_EQ(group_by.size(), 0u);
      handle.insertValueAccessorIntoDistinctifyHashTable(
          accessor.get(), *arguments_as_attributes, distinctify_hash_table);
      return;
    }
#endif

    SubBlocksReference sub_blocks_ref(*tuple_store_,
                                      indices_,
                                      indices_consistent_);
    attribute_id attr_id = 0;

    if (!group_by.empty()) {
      // Put GROUP BY keys into 'temp_result'.
      if (reuse_group_by_vectors->empty()) {
        // Compute GROUP BY values from group_by Scalars, and store them in
        // reuse_group_by_vectors for reuse by other aggregates on this same
        // block.
        reuse_group_by_vectors->reserve(group_by.size());
        for (const std::unique_ptr<const Scalar> &group_by_element : group_by) {
          reuse_group_by_vectors->emplace_back(
              group_by_element->getAllValues(accessor.get(), &sub_blocks_ref));
          temp_result.addColumn(reuse_group_by_vectors->back().get(), false);
          key_ids.push_back(attr_id++);
        }
      } else {
        // Reuse precomputed GROUP BY values from reuse_group_by_vectors.
        DCHECK_EQ(group_by.size(), reuse_group_by_vectors->size())
            << "Wrong number of reuse_group_by_vectors";
        for (const std::unique_ptr<ColumnVector> &reuse_cv : *reuse_group_by_vectors) {
          temp_result.addColumn(reuse_cv.get(), false);
          key_ids.push_back(attr_id++);
        }
      }
    }
    // Compute argument vectors and add them to 'temp_result'.
    for (const std::unique_ptr<const Scalar> &argument : arguments) {
      temp_result.addColumn(argument->getAllValues(accessor.get(), &sub_blocks_ref));
      key_ids.push_back(attr_id++);
    }
  }

  handle.insertValueAccessorIntoDistinctifyHashTable(
      &temp_result, key_ids, distinctify_hash_table);
}


// TODO(chasseur): Vectorization for updates.
StorageBlock::UpdateResult StorageBlock::update(
    const unordered_map<attribute_id, unique_ptr<const Scalar>> &assignments,
    const Predicate *predicate,
    InsertDestinationInterface *relocation_destination) {
  if (relation_.getID() != relocation_destination->getRelation().getID()) {
    FATAL_ERROR("StorageBlock::update() called with a relocation_destination "
                "that does not belong to the same relation.");
  }

  UpdateResult retval;
  // TODO(chasseur): Be smarter and only update indexes that need to be updated.
  std::unique_ptr<TupleIdSequence> matches(getMatchesForPredicate(predicate));

  // If nothing matches the predicate, return immediately.
  if (matches->empty()) {
    retval.indices_consistent = all_indices_consistent_;
    retval.relocation_destination_used = false;
    return retval;
  }

  // Remove index entries for tuples which will be updated.
  bool rebuild_some_indices = false;
  for (PtrVector<IndexSubBlock>::iterator it = indices_.begin();
       it != indices_.end();
       ++it) {
    if (it->supportsAdHocRemove()) {
      it->bulkRemoveEntries(*matches);
    } else {
      rebuild_some_indices = true;
    }
  }

  // To be safe, relocate ALL tuples if the relation is partitioned and we are
  // updating the partitioning attribute.
  const bool relocate_all =
      assignments.find(relocation_destination->getPartitioningAttribute()) != assignments.end();

  // IDs of tuples which should be re-added to indices.
  TupleIdSequence in_place_ids(tuple_store_->getMaxTupleID() + 1);
  // IDs of tuples which must be reinserted.
  TupleIdSequence relocate_ids(tuple_store_->getMaxTupleID() + 1);
  std::vector<Tuple> relocation_buffer;

  std::unique_ptr<ValueAccessor> accessor(tuple_store_->createValueAccessor());
  for (TupleIdSequence::const_iterator match_it = matches->begin();
       match_it != matches->end();
       ++match_it) {
    // Generate a map of updated values according to 'assignments'.
    std::unique_ptr<std::unordered_map<attribute_id, TypedValue>>
        updated_values(generateUpdatedValues(*accessor, *match_it, assignments));

    // Determine if the tuple can be modified in-place without deleting and
    // reinserting.
    if (!relocate_all
        && tuple_store_->canSetAttributeValuesInPlaceTyped(*match_it, *updated_values)) {
      // Update attribute values in place.
      for (std::unordered_map<attribute_id, TypedValue>::const_iterator update_it
               = updated_values->begin();
           update_it != updated_values->end();
           ++update_it) {
        tuple_store_->setAttributeValueInPlaceTyped(*match_it, update_it->first, update_it->second);
      }

      in_place_ids.set(*match_it, true);
    } else {
      // Make a copy of the tuple with the updated values.
      std::vector<TypedValue> updated_tuple_values;
      for (CatalogRelationSchema::const_iterator attr_it = relation_.begin();
           attr_it != relation_.end();
           ++attr_it) {
        std::unordered_map<attribute_id, TypedValue>::iterator update_it
            = updated_values->find(attr_it->getID());
        if (update_it == updated_values->end()) {
          updated_tuple_values.emplace_back(tuple_store_->getAttributeValueTyped(*match_it, attr_it->getID()));
          updated_tuple_values.back().ensureNotReference();
        } else {
          updated_tuple_values.emplace_back(std::move(update_it->second));
        }
      }

      relocation_buffer.emplace_back(std::move(updated_tuple_values));
      relocate_ids.set(*match_it, true);
    }
  }

  bool rebuild_all = false;
  retval.indices_consistent = all_indices_consistent_;
  retval.relocation_destination_used = false;
  if (!relocate_ids.empty()) {
    // Delete relocated tuples.
    if (tuple_store_->bulkDeleteTuples(&relocate_ids)) {
      rebuild_all = true;
    }

    if (relocate_all) {
      // Immediately bulk-insert into InsertDestination if required by
      // partitioning.
      retval.relocation_destination_used = true;
      relocation_destination->insertTuplesFromVector(relocation_buffer.begin(),
                                                     relocation_buffer.end());
    } else {
      // Reinsert into this block until space is exhausted.
      for (std::vector<Tuple>::const_iterator copy_it = relocation_buffer.begin();
          copy_it != relocation_buffer.end();
          ++copy_it) {
        if (rebuild_all || (!tuple_store_->adHocInsertIsEfficient())) {
          // If we must rebuild anyway, we might as well use the fast insert path.
          if (tuple_store_->insertTupleInBatch(*copy_it)) {
            rebuild_all = true;
          } else {
            retval.relocation_destination_used = true;
            relocation_destination->insertTuplesFromVector(copy_it, relocation_buffer.end());
            break;
          }
        } else {
          const TupleStorageSubBlock::InsertResult reinsert_result
              = tuple_store_->insertTuple(*copy_it);
          if (reinsert_result.inserted_id < 0) {
            DCHECK(!reinsert_result.ids_mutated);
            retval.relocation_destination_used = true;
            relocation_destination->insertTuplesFromVector(copy_it, relocation_buffer.end());
            break;
          }

          if (reinsert_result.ids_mutated) {
            rebuild_all = true;
          } else {
            // Only bother adding 'reinsert_id' to 'in_place_ids' if not rebuilding.
            in_place_ids.set(reinsert_result.inserted_id, true);
          }
        }
      }
    }
  }

  // TODO(chasseur): Consider doing a partial rollback when an index rebuild
  // fails.
  if (rebuild_all) {
    tuple_store_->rebuild();

    // Rebuild indexes.
    retval.indices_consistent = rebuildIndexes(false);
  } else if (!indices_.empty()) {
    all_indices_inconsistent_ = true;
    int index_num = 0;
    for (PtrVector<IndexSubBlock>::iterator it = indices_.begin();
         it != indices_.end();
         ++it, ++index_num) {
      bool add_successful;
      if (it->supportsAdHocAdd() && ((!rebuild_some_indices) || it->supportsAdHocRemove())) {
        add_successful = it->bulkAddEntries(in_place_ids);
#ifdef QUICKSTEP_REBUILD_INDEX_ON_UPDATE_OVERFLOW
        if (!add_successful) {
          add_successful = it->rebuild();
        }
#endif
      } else {
        add_successful = it->rebuild();
      }
      if (add_successful) {
        block_header_.set_index_consistent(index_num, true);
        indices_consistent_[index_num] = true;
        all_indices_inconsistent_ = false;
      } else {
        block_header_.set_index_consistent(index_num, false);
        indices_consistent_[index_num] = false;
        all_indices_consistent_ = false;
      }
    }

    updateHeader();
    retval.indices_consistent = all_indices_consistent_;
  }

  dirty_ = true;
  return retval;
}

namespace {

// Helper class to sort tuple.
//
// TODO(shoban): Refine this to use values directly instead of pointing to
// memory in storage block for better cache locality.
class SortReference {
 public:
  SortReference(const void *value, const tuple_id tuple) : tuple_(tuple), value_(value) {}

  inline const void* getDataPtr() const {
    return value_;
  }

  inline tuple_id getTupleID() const {
    return tuple_;
  }

 private:
  tuple_id tuple_;
  const void *value_;
};

}  // namespace

void StorageBlock::sortColumn(bool use_input_sequence,
                              const Scalar &sort_attribute,
                              bool sort_is_ascending,
                              bool null_first,
                              OrderedTupleIdSequence *sorted_sequence) const {
  const attribute_id sort_attr_id = sort_attribute.getAttributeIdForValueAccessor();
  DCHECK_NE(sort_attr_id, -1)
      << "Attempted to sort a StorageBlock on a sort attribute that is not "
      << "actually a ScalarAttribute.";

  const Type &sort_attr_type = sort_attribute.getType();

  std::unique_ptr<ValueAccessor> accessor;
  std::vector<SortReference> refs;
  OrderedTupleIdSequence nulls;

  refs.reserve(use_input_sequence ? sorted_sequence->size() : getTupleStorageSubBlock().numTuples());

  // TODO(shoban): Refer the TODO in sortColumnHelperValueAccessor() for
  // optimizing for NULL checking.
  ValueAccessor *all_accessor = tuple_store_->createValueAccessor(nullptr);
  InvokeOnValueAccessorNotAdapter(
      all_accessor,
      [&sort_attr_id,
       &use_input_sequence,
       &nulls,
       &refs,
       &accessor,
       &sorted_sequence](auto *all_accessor) -> void {  // NOLINT(build/c++11)
    if (use_input_sequence) {
      auto *seq_value_accessor = new OrderedTupleIdSequenceAdapterValueAccessor<
          typename std::remove_reference<decltype(*all_accessor)>::type>(
              all_accessor, *sorted_sequence);
      accessor.reset(seq_value_accessor);
      seq_value_accessor->beginIteration();
      while (seq_value_accessor->next()) {
        const void *value = seq_value_accessor->getUntypedValue(sort_attr_id);
        if (value) {
          refs.emplace_back(value, seq_value_accessor->getCurrentPosition());
        } else {
          nulls.emplace_back(seq_value_accessor->getCurrentPosition());
        }
      }
    } else {
      accessor.reset(all_accessor);
      while (all_accessor->next()) {
        const void *value = all_accessor->getUntypedValue(sort_attr_id);
        if (value) {
          refs.emplace_back(value, all_accessor->getCurrentPosition());
        } else {
          nulls.emplace_back(all_accessor->getCurrentPosition());
        }
      }
    }
  });

  if (use_input_sequence) {
    if (sort_is_ascending) {
      StableSortWrappedValues<SortReference, vector<SortReference>::iterator>(
          sort_attr_type, refs.begin(), refs.end());
    } else {
      // Use reverse iterators to sort in descending order.
      StableSortWrappedValues<SortReference, vector<SortReference>::reverse_iterator>(
          sort_attr_type, refs.rbegin(), refs.rend());
    }
  } else {
    if (sort_is_ascending) {
      SortWrappedValues<SortReference, vector<SortReference>::iterator>(
          sort_attr_type, refs.begin(), refs.end());
    } else {
      // Use reverse iterators to sort in descending order.
      SortWrappedValues<SortReference, vector<SortReference>::reverse_iterator>(
          sort_attr_type, refs.rbegin(), refs.rend());
    }
  }

  sorted_sequence->clear();
  if (null_first) {
    sorted_sequence->insert(sorted_sequence->end(), nulls.begin(), nulls.end());
  }
  for (const SortReference &ref : refs) {
    sorted_sequence->emplace_back(ref.getTupleID());
  }
  if (!null_first) {
    sorted_sequence->insert(sorted_sequence->end(), nulls.begin(), nulls.end());
  }
}

void StorageBlock::sort(const PtrVector<Scalar> &order_by,  // NOLINT(build/include_what_you_use)
                        const std::vector<bool> &sort_is_ascending,
                        const std::vector<bool> &null_first,
                        OrderedTupleIdSequence *sorted_sequence,
                        InsertDestinationInterface *output_destination) const {
  // TODO(shoban): We, currently, use a scheme where we stable sort the
  // tuple-id-sequence from last to first columns in ORDER BY clause to produce
  // the final sorted sequence. We could on the other hand sort from first to
  // last columns in ORDER BY clause, where the subsequent column's sort can be
  // broken down into sort of smaller ranges of tuple-id-sequences for which the
  // previous columns have the same value. This can have several pros and cons
  // the method used. Average-case asymptotics is definitely better in the
  // later. Need to do an analysis of the two methods.

  DEBUG_ASSERT(order_by.size() == sort_is_ascending.size());
  DEBUG_ASSERT(order_by.size() == null_first.size());
  DEBUG_ASSERT(order_by.size() > 0);

  // TODO(shoban): We should use reverse_iterator in conjunction with rbegin()
  // and rend() for better readability, if PtrVector supports it.
  PtrVector<Scalar>::const_iterator order_it = order_by.end();
  std::vector<bool>::const_iterator sort_is_ascending_it = sort_is_ascending.end();
  std::vector<bool>::const_iterator null_first_it = null_first.end();
  --order_it;
  --sort_is_ascending_it;
  --null_first_it;

  // Reserve capacity once. std::vector::clear() does not change capacity.
  sorted_sequence->reserve(getTupleStorageSubBlock().numTuples());

  // Sort the last column based on TupleIdSequence.
  // We can use regular sort on the last column, since there no inherent order
  // prior to this. std::sort() is an in-place impementation, whereas
  // std::stable_sort() uses extra memory. Hence, std::sort() is slight better
  // than std::stable_sort().
  sortColumn(false, *order_it, *sort_is_ascending_it, *null_first_it, sorted_sequence);

  while (order_it != order_by.begin()) {
    --order_it;
    --sort_is_ascending_it;
    --null_first_it;

    // Sort the other columns in reverse order based on OrderedTupleIdSequence
    // to maintain the previous sorted order.
    sortColumn(true, *order_it, *sort_is_ascending_it, *null_first_it, sorted_sequence);
  }

  // Write to output destination.
  if (output_destination) {
    std::unique_ptr<ValueAccessor> accessor(tuple_store_->createValueAccessor(nullptr));
    std::unique_ptr<ValueAccessor> ordered_accessor;
    InvokeOnValueAccessorNotAdapter(
        accessor.get(),
        [&](auto *accessor) -> void {  // NOLINT(build/c++11)
      ordered_accessor.reset(
          accessor->createSharedOrderedTupleIdSequenceAdapter(*sorted_sequence));
    });

    // TODO(quickstep-team): This might depending on the block layouts write one
    // or more sorted blocks for each input blocks. It would be useful to
    // remember the list of sorted blocks as an initial run for the merge phase
    // of sorting.
    output_destination->bulkInsertTuples(ordered_accessor.get(), true);
  }
}

void StorageBlock::deleteTuples(const Predicate *predicate) {
  std::unique_ptr<TupleIdSequence> matches(getMatchesForPredicate(predicate));

  if (!matches->empty()) {
    // First, remove entries from indices that support ad-hoc removal.
    bool rebuild_some_indices = false;
    for (PtrVector<IndexSubBlock>::iterator it = indices_.begin(); it != indices_.end(); ++it) {
      if (it->supportsAdHocRemove()) {
        it->bulkRemoveEntries(*matches);
      } else {
        rebuild_some_indices = true;
      }
    }

    // Delete tuples from the TupleStorageSubBlock.
    if (tuple_store_->bulkDeleteTuples(matches.get())) {
      // If the tuple-ID sequence was mutated, rebuild all indices.
      if (!rebuildIndexes(true)) {
        FATAL_ERROR("Rebuilding an IndexSubBlock failed after removing tuples.");
      }
    } else if (rebuild_some_indices) {
      // Rebuild any remaining indices that don't support ad-hoc removal.
      for (PtrVector<IndexSubBlock>::iterator it = indices_.begin(); it != indices_.end(); ++it) {
        if (!it->supportsAdHocRemove()) {
          if (!it->rebuild()) {
            FATAL_ERROR("Rebuilding an IndexSubBlock failed after removing tuples.");
          }
        }
      }
    }

    dirty_ = true;
  }
}

TupleStorageSubBlock* StorageBlock::CreateTupleStorageSubBlock(
    const CatalogRelationSchema &relation,
    const TupleStorageSubBlockDescription &description,
    const bool new_block,
    void *sub_block_memory,
    const std::size_t sub_block_memory_size) {
  DEBUG_ASSERT(description.IsInitialized());
  switch (description.sub_block_type()) {
    case TupleStorageSubBlockDescription::PACKED_ROW_STORE:
      return new PackedRowStoreTupleStorageSubBlock(relation,
                                                    description,
                                                    new_block,
                                                    sub_block_memory,
                                                    sub_block_memory_size);
    case TupleStorageSubBlockDescription::BASIC_COLUMN_STORE:
      return new BasicColumnStoreTupleStorageSubBlock(relation,
                                                      description,
                                                      new_block,
                                                      sub_block_memory,
                                                      sub_block_memory_size);
    case TupleStorageSubBlockDescription::COMPRESSED_PACKED_ROW_STORE:
      return new CompressedPackedRowStoreTupleStorageSubBlock(relation,
                                                              description,
                                                              new_block,
                                                              sub_block_memory,
                                                              sub_block_memory_size);
    case TupleStorageSubBlockDescription::COMPRESSED_COLUMN_STORE:
      return new CompressedColumnStoreTupleStorageSubBlock(relation,
                                                           description,
                                                           new_block,
                                                           sub_block_memory,
                                                           sub_block_memory_size);
    case TupleStorageSubBlockDescription::SPLIT_ROW_STORE:
      return new SplitRowStoreTupleStorageSubBlock(relation,
                                                   description,
                                                   new_block,
                                                   sub_block_memory,
                                                   sub_block_memory_size);
    default:
      if (new_block) {
        FATAL_ERROR("A StorageBlockLayout provided an unknown TupleStorageSubBlockType.");
      } else {
        throw MalformedBlock();
      }
  }
}

IndexSubBlock* StorageBlock::CreateIndexSubBlock(
    const TupleStorageSubBlock &tuple_store,
    const IndexSubBlockDescription &description,
    const bool new_block,
    void *sub_block_memory,
    const std::size_t sub_block_memory_size) {
  DEBUG_ASSERT(description.IsInitialized());
  switch (description.sub_block_type()) {
    case IndexSubBlockDescription::BLOOM_FILTER:
      return new BloomFilterIndexSubBlock(tuple_store,
                                          description,
                                          new_block,
                                          sub_block_memory,
                                          sub_block_memory_size);
    case IndexSubBlockDescription::CSB_TREE:
      return new CSBTreeIndexSubBlock(tuple_store,
                                      description,
                                      new_block,
                                      sub_block_memory,
                                      sub_block_memory_size);
    case IndexSubBlockDescription::SMA:
      return new SMAIndexSubBlock(tuple_store,
                                  description,
                                  new_block,
                                  sub_block_memory,
                                  sub_block_memory_size);
#ifdef QUICKSTEP_HAVE_BITWEAVING
    case IndexSubBlockDescription::BITWEAVING_V:
      return new BitWeavingVIndexSubBlock(tuple_store,
                                          description,
                                          new_block,
                                          sub_block_memory,
                                          sub_block_memory_size);
    case IndexSubBlockDescription::BITWEAVING_H:
      return new BitWeavingHIndexSubBlock(tuple_store,
                                          description,
                                          new_block,
                                          sub_block_memory,
                                          sub_block_memory_size);
#else
    case IndexSubBlockDescription::BITWEAVING_V:  // Fall through.
    case IndexSubBlockDescription::BITWEAVING_H:
      LOG(FATAL) << "Attempted to create a block with a bitweaving index "
                 << "but Quickstep was not compiled with bitweaving.";
#endif
    default:
      if (new_block) {
        FATAL_ERROR("A StorageBlockLayout provided an unknown IndexBlockType.");
      } else {
        throw MalformedBlock();
      }
  }
}

bool StorageBlock::insertEntryInIndexes(const tuple_id new_tuple) {
  DEBUG_ASSERT(ad_hoc_insert_supported_);
  DEBUG_ASSERT(new_tuple >= 0);
  DEBUG_ASSERT(all_indices_consistent_);

  for (PtrVector<IndexSubBlock>::iterator it = indices_.begin();
       it != indices_.end();
       ++it) {
    bool entry_added;
    if (it->supportsAdHocAdd()) {
      entry_added = it->addEntry(new_tuple);
    } else {
      entry_added = it->rebuild();
    }
    if (!entry_added) {
      // Roll back if index is full.
      //
      // TODO(chasseur): What about fragmented indexes, where rebuilding could allow success?
      bool rebuild_some_indices = false;
      for (PtrVector<IndexSubBlock>::iterator fixer_it = indices_.begin();
           fixer_it != it;
           ++fixer_it) {
        // Do ad-hoc removal for those indices which support it. Those that
        // don't are rebuilt after the entry is removed from the tuple_store_
        // below.
        if (fixer_it->supportsAdHocRemove()) {
          fixer_it->removeEntry(new_tuple);
        } else {
          rebuild_some_indices = true;
        }
      }

      if (tuple_store_->deleteTuple(new_tuple)) {
        // The tuple-ID sequence was mutated, so rebuild all indices.
        if (!rebuildIndexes(true)) {
          FATAL_ERROR("Rebuilding an IndexSubBlock failed after removing tuples.");
        }
      } else if (rebuild_some_indices) {
        // Rebuild those indices that were modified that don't support ad-hoc
        // removal.
        for (PtrVector<IndexSubBlock>::iterator fixer_it = indices_.begin();
             fixer_it != it;
             ++fixer_it) {
          if (!fixer_it->supportsAdHocRemove()) {
            if (!fixer_it->rebuild()) {
              // It should always be possible to rebuild an index with the
              // tuples which it originally contained.
              FATAL_ERROR("Rebuilding an IndexSubBlock failed after removing tuples.");
            }
          }
        }
      }

      return false;
    }
  }

  return true;
}

bool StorageBlock::bulkInsertEntriesInIndexes(TupleIdSequence *new_tuples,
                                              const bool roll_back_on_failure) {
  DEBUG_ASSERT(ad_hoc_insert_supported_);
  DEBUG_ASSERT(all_indices_consistent_);

  // If 'roll_back_on_failure' is false, we will allow some indices to become
  // inconsistent.
  if ((!indices_.empty()) && (!roll_back_on_failure)) {
    all_indices_inconsistent_ = true;
  }

  int index_num = 0;
  for (PtrVector<IndexSubBlock>::iterator it = indices_.begin();
       it != indices_.end();
       ++it, ++index_num) {
    bool entries_added;
    if (it->supportsAdHocAdd()) {
      entries_added = it->bulkAddEntries(*new_tuples);
    } else {
      entries_added = it->rebuild();
    }
    if (!entries_added) {
      if (roll_back_on_failure) {
        // Roll back if index full.
        bool rebuild_some_indices = false;
        for (PtrVector<IndexSubBlock>::iterator fixer_it = indices_.begin();
             fixer_it != it;
             ++fixer_it) {
          // Do ad-hoc removal for those indices which support it. Those that
          // don't are rebuilt after the entries are removed from the
          // tuple_store_ below.
          if (fixer_it->supportsAdHocRemove()) {
            fixer_it->bulkRemoveEntries(*new_tuples);
          } else {
            rebuild_some_indices = true;
          }
        }

        if (tuple_store_->bulkDeleteTuples(new_tuples)) {
          // The tuple-ID sequence was mutated, so rebuild all indices.
          if (!rebuildIndexes(true)) {
            FATAL_ERROR("Rebuilding an IndexSubBlock failed after removing tuples.");
          }
        } else if (rebuild_some_indices) {
          // Rebuild those indices that were modified that don't support ad-hoc
          // removal.
          for (PtrVector<IndexSubBlock>::iterator fixer_it = indices_.begin();
               fixer_it != it;
               ++fixer_it) {
            if (!fixer_it->supportsAdHocRemove()) {
              if (!fixer_it->rebuild()) {
                // It should always be possible to rebuild an index with the
                // tuples which it originally contained.
                FATAL_ERROR("Rebuilding an IndexSubBlock failed after removing tuples.");
              }
            }
          }
        }

        return false;
      } else {
        block_header_.set_index_consistent(index_num, false);
        indices_consistent_[index_num] = false;
        all_indices_consistent_ = false;
      }
    } else if (!roll_back_on_failure) {
      all_indices_inconsistent_ = false;
    }
  }

  if (!all_indices_consistent_) {
    updateHeader();
  }
  return all_indices_consistent_;
}

bool StorageBlock::rebuildIndexes(bool short_circuit) {
  if (indices_.empty()) {
    return true;
  }

  all_indices_consistent_ = true;
  all_indices_inconsistent_ = true;

  int index_num = 0;
  for (PtrVector<IndexSubBlock>::iterator it = indices_.begin();
       it != indices_.end();
       ++it, ++index_num) {
    if (it->rebuild()) {
      all_indices_inconsistent_ = false;
      indices_consistent_[index_num] = true;
      block_header_.set_index_consistent(index_num, true);
    } else {
      all_indices_consistent_ = false;
      indices_consistent_[index_num] = false;
      block_header_.set_index_consistent(index_num, false);
      if (short_circuit) {
        return false;
      }
    }
  }
  updateHeader();

  return all_indices_consistent_;
}

TupleIdSequence* StorageBlock::getMatchesForPredicate(const Predicate *predicate,
                                                      const TupleIdSequence *filter) const {
  if (predicate == nullptr) {
    TupleIdSequence *sequence = tuple_store_->getExistenceMap();
    if (filter != nullptr) {
      sequence->intersectWith(*filter);
    }
    return sequence;
  }

  std::unique_ptr<ValueAccessor> value_accessor(tuple_store_->createValueAccessor());
  std::unique_ptr<TupleIdSequence> existence_map;
  if (!tuple_store_->isPacked()) {
    existence_map.reset(tuple_store_->getExistenceMap());
  }
  SubBlocksReference sub_blocks_ref(*tuple_store_,
                                    indices_,
                                    indices_consistent_);
  return predicate->getAllMatches(value_accessor.get(),
                                  &sub_blocks_ref,
                                  filter,
                                  existence_map.get());
}

std::unordered_map<attribute_id, TypedValue>* StorageBlock::generateUpdatedValues(
    const ValueAccessor &accessor,
    const tuple_id tuple,
    const unordered_map<attribute_id, unique_ptr<const Scalar>> &assignments) const {
  std::unordered_map<attribute_id, TypedValue> *update_map
      = new std::unordered_map<attribute_id, TypedValue>();

  for (unordered_map<attribute_id, unique_ptr<const Scalar>>::const_iterator it = assignments.begin();
       it != assignments.end();
       ++it) {
    pair<std::unordered_map<attribute_id, TypedValue>::iterator, bool> insert_result
        = update_map->emplace(it->first,
                              it->second->getValueForSingleTuple(accessor, tuple));
    DCHECK(insert_result.second);
    insert_result.first->second.ensureNotReference();
  }

  return update_map;
}

AggregationState* StorageBlock::aggregateHelperColumnVector(
    const AggregationHandle &handle,
    const std::vector<std::unique_ptr<const Scalar>> &arguments,
    const TupleIdSequence *matches) const {
  if (arguments.empty()) {
    // Special case. This is a nullary aggregate (i.e. COUNT(*)).
    return handle.accumulateNullary(matches == nullptr ? tuple_store_->numTuples()
                                                       : matches->size());
  } else {
    // Set up a ValueAccessor that will be used when materializing argument
    // values below (possibly filtered based on the '*matches' to a filter
    // predicate).
    std::unique_ptr<ValueAccessor> accessor;
    if (matches == nullptr) {
      accessor.reset(tuple_store_->createValueAccessor());
    } else {
      accessor.reset(tuple_store_->createValueAccessor(matches));
    }

    SubBlocksReference sub_blocks_ref(*tuple_store_,
                                      indices_,
                                      indices_consistent_);

    // Materialize each argument's values for this block as a ColumnVector.
    std::vector<std::unique_ptr<ColumnVector>> column_vectors;
    for (const std::unique_ptr<const Scalar> &argument : arguments) {
      column_vectors.emplace_back(argument->getAllValues(accessor.get(), &sub_blocks_ref));
    }

    // Have the AggregationHandle actually do the aggregation.
    return handle.accumulateColumnVectors(column_vectors);
  }
}

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
AggregationState* StorageBlock::aggregateHelperValueAccessor(
    const AggregationHandle &handle,
    const std::vector<attribute_id> &argument_ids,
    const TupleIdSequence *matches) const {
  // Set up a ValueAccessor to aggregate over (possibly filtered based on the
  // '*matches' to a filter predicate).
  std::unique_ptr<ValueAccessor> accessor;
  if (matches == nullptr) {
    accessor.reset(tuple_store_->createValueAccessor());
  } else {
    accessor.reset(tuple_store_->createValueAccessor(matches));
  }

  // Have the AggregationHandle actually do the aggregation.
  return handle.accumulateValueAccessor(
      accessor.get(),
      argument_ids);
}
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

void StorageBlock::updateHeader() {
  DEBUG_ASSERT(*static_cast<const int*>(block_memory_) == block_header_.ByteSize());

  if (!block_header_.SerializeToArray(static_cast<char*>(block_memory_) + sizeof(int),
                                      block_header_.ByteSize())) {
    FATAL_ERROR("Failed to do binary serialization of StorageBlockHeader in StorageBlock::updateHeader()");
  }
}

void StorageBlock::invalidateAllIndexes() {
  if ((!indices_.empty()) && (!all_indices_inconsistent_)) {
    for (unsigned int index_num = 0;
         index_num < indices_.size();
         ++index_num) {
      indices_consistent_[index_num] = false;
      block_header_.set_index_consistent(index_num, false);
    }
    all_indices_consistent_ = false;
    all_indices_inconsistent_ = true;

    updateHeader();
  }
}

const std::size_t StorageBlock::getNumTuples() const {
  DCHECK(tuple_store_ != nullptr);
  return tuple_store_->numTuples();
}

}  // namespace quickstep
