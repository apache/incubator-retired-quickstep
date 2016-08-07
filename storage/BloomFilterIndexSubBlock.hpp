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

#ifndef QUICKSTEP_STORAGE_BLOOM_FILTER_INDEX_SUB_BLOCK_HPP_
#define QUICKSTEP_STORAGE_BLOOM_FILTER_INDEX_SUB_BLOCK_HPP_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "expressions/predicate/PredicateCost.hpp"
#include "storage/IndexSubBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/SubBlockTypeRegistryMacros.hpp"
#include "utility/BloomFilter.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogRelationSchema;
class ComparisonPredicate;
class IndexSubBlockDescription;
class TupleIdSequence;
class TupleStorageSubBlock;

QUICKSTEP_DECLARE_SUB_BLOCK_TYPE_REGISTERED(BloomFilterIndexSubBlock);

/** \addtogroup Storage
*  @{
*/

/**
* @brief An IndexSubBlock which implements a Bloom Filter index.
* @note This IndexSubBlock supports both fixed-length attributes, and the
*          variable length attributes.
**/
class BloomFilterIndexSubBlock : public IndexSubBlock {
 public:
  /**
   * @brief An enum describing the various selecitivity factors provided by
   *        a BloomFilter index.
   * @note A kSelectivityNone will correspond to a BloomFilter miss (true negative),
   *       when the data is certainly guaranteed to be not present in the StorageBlock.
   *       A kSelectivityAll, kSelectivityNone will correspond to a BloomFilter hit
   *       when the data may or may not be present in the StorageBlock.
   **/
  enum class BloomFilterSelectivity {
    kSelectivityUnknown = 0,
    kSelectivityAll,
    kSelectivityNone
  };

  /**
   * @brief A random seed to initialize the bloom filter hash functions.
   **/
  static const std::uint64_t kBloomFilterSeed = 0xA5A5A5A55A5A5A5AULL;

  BloomFilterIndexSubBlock(const TupleStorageSubBlock &tuple_store,
                           const IndexSubBlockDescription &description,
                           const bool new_block,
                           void *sub_block_memory,
                           const std::size_t sub_block_memory_size);

  ~BloomFilterIndexSubBlock() override;

  /**
  * @brief Determine whether an IndexSubBlockDescription is valid for this
  *        type of IndexSubBlock.
  *
  * @param relation The relation an index described by description would
  *        belong to.
  * @param description A description of the parameters for this type of
  *        IndexSubBlock, which will be checked for validity.
  * @return Whether description is well-formed and valid for this type of
  *         IndexSubBlock belonging to relation (i.e. whether an IndexSubBlock
  *         of this type, belonging to relation, can be constructed according
  *         to description).
  **/
  static bool DescriptionIsValid(const CatalogRelationSchema &relation,
                                const IndexSubBlockDescription &description);

  /**
  * @brief Estimate the average number of bytes (including any applicable
  *        overhead) used to index a single tuple in this type of
  *        IndexSubBlock. Used by StorageBlockLayout::finalize() to divide
  *        block memory amongst sub-blocks.
  * @warning description must be valid. DescriptionIsValid() should be called
  *          first if necessary.
  *
  * @param relation The relation tuples belong to.
  * @param description A description of the parameters for this type of
  *        IndexSubBlock.
  * @return The average/ammortized number of bytes used to index a single
  *         tuple of relation in an IndexSubBlock of this type described by
  *         description.
  **/
  static std::size_t EstimateBytesPerTuple(const CatalogRelationSchema &relation,
                                          const IndexSubBlockDescription &description);

  /**
   * @brief Estimate the total number of bytes (including any applicable
   *        overhead) occupied by this IndexSubBlock within the StorageBlock.
   *        This function is to be used by those indicies whose occupied size
   *        in the block does not depend on the number of tuples being indexed.
   * @warning description must be valid. DescriptionIsValid() should be called
   *          first if necessary.
   * @note This function will be invoked by StorageBlockLayout::finalize()
   *       if and only when EstimateBytesPerTuple() returns a zero size.
   *
   * @param relation The relation tuples belong to.
   * @param description A description of the parameters for this type of
   *        IndexSubBlock.
   * @return The total number of bytes occupied by this IndexSubBlock within
   *         the StorageBlock.
   **/
  static std::size_t EstimateBytesPerBlock(const CatalogRelationSchema &relation,
                                           const IndexSubBlockDescription &description);

  IndexSubBlockType getIndexSubBlockType() const override {
    return kBloomFilter;
  }

  bool supportsAdHocAdd() const override {
    return true;
  }

  bool supportsAdHocRemove() const override {
    return false;
  }

  bool addEntry(const tuple_id tuple) override;

  bool bulkAddEntries(const TupleIdSequence &tuples) override;

  void removeEntry(const tuple_id tuple) override;

  void bulkRemoveEntries(const TupleIdSequence &tuples) override;

  predicate_cost_t estimatePredicateEvaluationCost(
     const ComparisonPredicate &predicate) const override;

  /**
  * @note Currently this version only supports predicates with
  *       equality comparison of a static literal value with a non-composite key.
  **/
  TupleIdSequence* getMatchesForPredicate(const ComparisonPredicate &predicate,
                                          const TupleIdSequence *filter) const override;

  bool rebuild() override;

  /**
   * @brief Get the selectivity provided by the index for a given predicate.
   *        There are only two selectivity levels returned by this function-
   *        either a zero(none) selectivity for bloom filter miss
   *        or a one(all) selectivity for bloom filter hit.
   *        A none selectivity predicate will return no tuples, whereas
   *        an all selectivity predicate will possibly return all tuples.
   *
   * @param predicate The comparison predicate to evaluate selectivity for.
   * @return An enum describing the selectivity factor.
   **/
  BloomFilterSelectivity getSelectivityForPredicate(const ComparisonPredicate &predicate) const;

 private:
  bool is_initialized_;
  bool is_consistent_;
  const std::uint64_t random_seed_;
  const std::uint64_t bit_array_size_in_bytes_;
  std::vector<attribute_id> indexed_attribute_ids_;
  std::unique_ptr<unsigned char> bit_array_;
  std::unique_ptr<BloomFilter> bloom_filter_;

  friend class BloomFilterIndexSubBlockTest;

  DISALLOW_COPY_AND_ASSIGN(BloomFilterIndexSubBlock);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_BLOOM_FILTER_INDEX_SUB_BLOCK_HPP_
