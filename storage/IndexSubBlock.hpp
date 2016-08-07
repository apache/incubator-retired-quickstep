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

#ifndef QUICKSTEP_STORAGE_INDEX_SUB_BLOCK_HPP_
#define QUICKSTEP_STORAGE_INDEX_SUB_BLOCK_HPP_

#include <cstddef>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/predicate/PredicateCost.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogRelationSchema;
class ComparisonPredicate;
class IndexSubBlockDescription;
class TupleIdSequence;

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief SubBlock which indexes tuples stored in a TupleStorageSubBlock
 *        (within the same StorageBlock).
 **/
class IndexSubBlock {
 public:
  /**
   * @brief Constructor.
   *
   * @param tuple_store The TupleStorageSubBlock whose contents are indexed by
   *                    this IndexSubBlock.
   * @param description A description containing any parameters needed to
   *        construct this SubBlock (e.g. what attributes to index on).
   *        Implementation-specific parameters are defined as extensions in
   *        StorageBlockLayout.proto.
   * @param new_block Whether this is a newly-created block.
   * @param sub_block_memory The memory slot to use for the block's contents.
   * @param sub_block_memory_size The size of the memory slot in bytes.
   * @exception BlockMemoryTooSmall This TupleStorageSubBlock hasn't been
   *            provided enough memory to store metadata.
   **/
  IndexSubBlock(const TupleStorageSubBlock &tuple_store,
                const IndexSubBlockDescription &description,
                const bool new_block,
                void *sub_block_memory,
                const std::size_t sub_block_memory_size)
                : sub_block_memory_(sub_block_memory),
                  sub_block_memory_size_(sub_block_memory_size),
                  relation_(tuple_store.getRelation()),
                  description_(description),
                  tuple_store_(tuple_store) {
  }

  /**
   * @brief Virtual destructor
   **/
  virtual ~IndexSubBlock() {
  }

  /**
   * @brief Identify the type of this IndexSubBlock.
   *
   * @return This IndexSubBlock's type.
   **/
  virtual IndexSubBlockType getIndexSubBlockType() const = 0;

  /**
   * @brief Determine whether this IndexSubBlock supports adding entries to an
   *        existing index via the addEntry() and bulkAddEntries() methods.
   * @note If this method returns false, then rebuild() must be called to add
   *       entries to this index.
   * @note It is still legal to call addEntry() or bulkAddEntries() if this
   *       method returns false, but those methods will always fail and return
   *       false.
   *
   * @return Whether addEntry() and bulkAddEntries() are usable with this
   *         IndexSubBlock.
   **/
  virtual bool supportsAdHocAdd() const = 0;

  /**
   * @brief Determine whether this IndexSubBlock supports removing entries from
   *        an existing index via the removeEntry() and bulkRemoveEntries()
   *        methods.
   * @note If this method returns false, then rebuild() must be called to
   *       remove entries from this index.
   * @warning It is an error to call removeEntry() or bulkRemoveEntries() if
   *          this method returns false.
   *
   * @return Whether removeEntry() and bulkRemoveEntries() are usable with this
   *         IndexSubBlock.
   **/
  virtual bool supportsAdHocRemove() const = 0;

  /**
   * @brief Add an entry to this index
   * @note Implementations should access the necessary attribute values via
   *       parent_'s TupleStorageSubBlock.
   *
   * @param tuple The ID of the tuple to index.
   * @return True if entry was successfully added, false if there was no space
   *         to add entry.
   **/
  virtual bool addEntry(const tuple_id tuple) = 0;

  /**
   * @brief Add multiple entries to this index in bulk.
   * @note As with addEntry(), implementations should access the necessary
   *       attribute values via parent_'s TupleStorageSubBlock.
   *
   * @param tuples The IDs of the tuples to index.
   * @return True if all entries were successfully added, false if not enough
   *         space.
   **/
  virtual bool bulkAddEntries(const TupleIdSequence &tuples) = 0;

  /**
   * @brief Remove an entry from this index.
   * @note Tuples are removed from indexes BEFORE the TupleStorageSubBlock, so
   *       it is safe to look up attribute values in parent_'s
   *       TupleStorageSubBlock to quickly locate entries in the index as
   *       necessary.
   *
   * @param tuple The ID of the tuple to remove from the index.
   **/
  virtual void removeEntry(const tuple_id tuple) = 0;

  /**
   * @brief Remove multiple entries from this index
   * @note Tuples are removed from indexes BEFORE the TupleStorageSubBlock, so
   *       it is safe to look up attribute values in parent_'s
   *       TupleStorageSubBlock to quickly locate entries in the index as
   *       necessary.
   *
   * @param tuples The IDs of the tuple to remove from the index.
   **/
  virtual void bulkRemoveEntries(const TupleIdSequence &tuples) = 0;

  virtual predicate_cost_t estimatePredicateEvaluationCost(
      const ComparisonPredicate &predicate) const = 0;

  /**
   * @brief Use this index to find tuples matching a particular predicate.
   * @warning This should only be called if estimatePredicateEvaluationCost()
   *          returns a value OTHER than kInfinite.
   *
   * @param predicate The predicate to match.
   * @param filter If non-NULL, then only tuple IDs which are set in the
   *        filter will be checked and possible returned in the result.
   * @return A TupleIdSequence which contains matching tuple IDs for predicate.
   **/
  virtual TupleIdSequence* getMatchesForPredicate(const ComparisonPredicate &predicate,
                                                  const TupleIdSequence *filter) const = 0;

  /**
   * @brief Rebuild this index from scratch.
   *
   * @return True if the index was successfully rebuilt, false if there was not
   *         enough space to index all the tuples in tuple_store_.
   **/
  virtual bool rebuild() = 0;

 protected:
  void *sub_block_memory_;
  const std::size_t sub_block_memory_size_;

  const CatalogRelationSchema &relation_;
  const IndexSubBlockDescription &description_;
  const TupleStorageSubBlock &tuple_store_;

 private:
  DISALLOW_COPY_AND_ASSIGN(IndexSubBlock);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_INDEX_SUB_BLOCK_HPP_
