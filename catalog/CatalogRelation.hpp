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

#ifndef QUICKSTEP_CATALOG_CATALOG_RELATION_HPP_
#define QUICKSTEP_CATALOG_CATALOG_RELATION_HPP_

#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogConfig.h"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogRelationStatistics.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "catalog/IndexScheme.hpp"

#ifdef QUICKSTEP_HAVE_LIBNUMA
#include "catalog/NUMAPlacementScheme.hpp"
#endif  // QUICKSTEP_HAVE_LIBNUMA

#include "catalog/PartitionScheme.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.hpp"
#include "storage/StorageConstants.hpp"
#include "threading/Mutex.hpp"
#include "threading/SharedMutex.hpp"
#include "threading/SpinSharedMutex.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogDatabase;

/** \addtogroup Catalog
 *  @{
 */

/**
 * @brief A relation in a database. This augments the schema information in
 *        CatalogRelationSchema with information about physical StorageBlocks
 *        and their layouts.
 **/
class CatalogRelation : public CatalogRelationSchema {
 public:
  typedef CatalogRelationSchema::size_type size_type;
  typedef CatalogRelationSchema::const_iterator const_iterator;

  typedef std::vector<block_id>::size_type size_type_blocks;

  /**
   * @brief Create a new relation.
   *
   * @param parent The database this relation belongs to.
   * @param name This relation's name.
   * @param id This relation's ID (defaults to -1, which means invalid/unset).
   * @param temporary Whether this relation is temporary (stores an
   *        intermediate result during query processing).
   **/
  CatalogRelation(CatalogDatabase* parent,
                  const std::string &name,
                  const relation_id id = -1,
                  bool temporary = false)
      : CatalogRelationSchema(parent, name, id, temporary),
        default_layout_(nullptr),
        num_partitions_(1u),
        statistics_(new CatalogRelationStatistics()) {
  }

  /**
   * @brief Reconstruct a relation from its serialized Protocol Buffer form.
   *
   * @param proto The Protocol Buffer serialization of a relation,
   *        previously produced by getProto().
   **/
  explicit CatalogRelation(const serialization::CatalogRelationSchema &proto);

  /**
   * @brief Destructor which recursively destroys children.
   **/
  ~CatalogRelation() override {
  }

  serialization::CatalogRelationSchema getProto() const override;

  /**
   * @brief Check if a partition scheme is available for the relation.
   *
   * @return True if the relation has a partition scheme, false otherwise.
   **/
  bool hasPartitionScheme() const {
    return partition_scheme_ != nullptr;
  }

  /**
   * @brief Get the partition scheme of the catalog relation.
   *
   * @return A const pointer to the partition scheme of the relation.
   **/
  const PartitionScheme* getPartitionScheme() const {
    return partition_scheme_.get();
  }

  /**
   * @brief Set the partition scheme for the catalog relation.
   * @warning This method should be called only once when a relation is
   *          is first created.
   *
   * @param partition_scheme The partition scheme object for a relation, which
   *        becomes owned by this relation.
   **/
  void setPartitionScheme(PartitionScheme* partition_scheme);

  /**
   * @brief Get a mutable partition scheme of the relation.
   *
   * @return A pointer to the partition scheme.
   **/
  PartitionScheme* getPartitionSchemeMutable() {
    return partition_scheme_.get();
  }

  /**
   * @brief Get the number of partitions for the relation.
   *
   * @return The number of partitions the relation is partitioned into.
   **/
  std::size_t getNumPartitions() const {
    return num_partitions_;
  }

  /**
   * @brief Check if a NUMA placement scheme is available for the relation.
   *
   * @return True if the relation has a NUMA placement scheme, false otherwise.
   **/
  bool hasNUMAPlacementScheme() const {
#ifdef QUICKSTEP_HAVE_LIBNUMA
    return placement_scheme_ != nullptr;
#else
    return false;
#endif  // QUICKSTEP_HAVE_LIBNUMA
  }

#ifdef QUICKSTEP_HAVE_LIBNUMA
  /**
   * @brief Get the NUMA placement scheme of the catalog relation.
   * @warning This is only safe if hasNUMAPlacementScheme() is true.
   *
   * @return A const reference to the NUMA placement scheme of the relation.
   **/
  const NUMAPlacementScheme& getNUMAPlacementScheme() const {
    return *placement_scheme_;
  }

  /**
   * @brief Get a mutable NUMA placement scheme of the relation.
   *
   * @return A pointer to the NUMA placement scheme.
   **/
  NUMAPlacementScheme* getNUMAPlacementSchemeMutable() {
    return placement_scheme_.get();
  }

  /**
   * @brief Get the NUMA placement scheme of the relation.
   *
   * @return A pointer to a const NUMA placement scheme.
   **/
  const NUMAPlacementScheme* getNUMAPlacementSchemePtr() const {
    return placement_scheme_.get();
  }

  /**
   * @brief Set the NUMA placement scheme for the catalog relation.
   *
   * @param placement_scheme The NUMA placement scheme object for the relation,
   *        which becomes owned by this relation.
   **/
  void setNUMAPlacementScheme(NUMAPlacementScheme *placement_scheme)  {
    placement_scheme_.reset(placement_scheme);
  }
#endif  // QUICKSTEP_HAVE_LIBNUMA

  /**
   * @brief Check if an index scheme is available for the relation.
   *
   * @return True if the relation has a index scheme, false otherwise.
   **/
  bool hasIndexScheme() const {
    return index_scheme_ != nullptr;
  }

  /**
   * @brief Get the index scheme of the catalog relation.
   * @warning This is only safe if hasIndexScheme() is true.
   *
   * @return A const reference to the index scheme of the relation.
   **/
  const IndexScheme& getIndexScheme() const {
    return *index_scheme_;
  }

  /**
   * @brief Get a mutable index scheme of the relation.
   *
   * @return A pointer to the index scheme.
   **/
  IndexScheme* getIndexSchemeMutable() {
    return index_scheme_.get();
  }

  /**
   * @brief Register a StorageBlock as belonging to this relation.
   * @note Blocks are ordered in the same order they are added (preserving order
   *       for full-table sorts).
   *
   * @param block the ID of the block to add.
   **/
  void addBlock(const block_id block) {
    SpinSharedMutexExclusiveLock<false> lock(blocks_mutex_);
    blocks_.emplace_back(block);
  }

  /**
   * @brief Remove a StorageBlock from this relation (idempotent).
   *
   * @param block the ID of the block to remove.
   **/
  void removeBlock(const block_id block) {
    SpinSharedMutexExclusiveLock<false> lock(blocks_mutex_);
    std::vector<block_id>::iterator it = std::find(blocks_.begin(), blocks_.end(), block);
    if (it != blocks_.end()) {
      blocks_.erase(it);
    }
  }

  /**
   * @brief Remove all StorageBlocks from this relation.
   **/
  void clearBlocks() {
    SpinSharedMutexExclusiveLock<false> lock(blocks_mutex_);
    blocks_.clear();
  }

  /**
   * @brief Check whether an index with the given exists or not.
   *
   * @param index_name Name of the index to be checked.
   * @return Whether the index exists or not.
   **/
  bool hasIndexWithName(const std::string &index_name) const {
    SpinSharedMutexSharedLock<false> lock(index_scheme_mutex_);
    return index_scheme_ && index_scheme_->hasIndexWithName(index_name);
  }

  /**
   * @brief Check whether an index with the given description
   *        containing the same attribute id and index type
   *        exists or not in the index map.
   *
   * @param index_descripton Index Description to check against.
   * @return Whether a similar index description was already defined or not.
   **/
  bool hasIndexWithDescription(const IndexSubBlockDescription &index_description) const {
    SpinSharedMutexSharedLock<false> lock(index_scheme_mutex_);
    return index_scheme_ && index_scheme_->hasIndexWithDescription(index_description);
  }

  /**
   * @brief Add an index to the index_map.
   *
   * @param index_name Name of the index to be added.
   * @param index_description Corresponding description of the index.
   * @return Whether the index was added successfully or not
   **/
  bool addIndex(const std::string &index_name,
                IndexSubBlockDescription &&index_description) {  // NOLINT(whitespace/operators)
    SpinSharedMutexExclusiveLock<false> lock(index_scheme_mutex_);
    // Create an index_scheme, if it does not exist.
    if (index_scheme_ == nullptr) {
      index_scheme_.reset(new IndexScheme());
    }

    // Verify that index with the given name does not exist.
    if (index_scheme_->hasIndexWithName(index_name)) {
      return false;
    }

    // Verify that index with similar description does not exist.
    if (index_scheme_->hasIndexWithDescription(index_description)) {
      return false;
    }

    // Verify that the CatalogRelation has a valid layout.
    // This check is also required for some unit tests on catalog.
    if (default_layout_ == nullptr) {
      // Calling this function initializes the default_layout_.
      getDefaultStorageBlockLayout();
    }

    StorageBlockLayoutDescription *layout = default_layout_->getDescriptionMutable();
    layout->add_index_description()->MergeFrom(index_description);
    default_layout_->finalize();

    // Update the index_scheme.
    index_scheme_->addIndexMapEntry(index_name, std::move(index_description));

    return true;  // Index added successfully, lock released.
  }

  /**
   * @brief Check whether a serialization::CatalogRelation is fully-formed and
   *        all parts are valid.
   *
   * @param proto A serialized Protocol Buffer representation of a relation,
   *        originally generated by getProto().
   * @return Whether proto is fully-formed and valid.
   **/
  static bool ProtoIsValid(const serialization::CatalogRelationSchema &proto);

  /**
   * @brief Get the number of child blocks.
   *
   * @return The number of child blocks.
   **/
  inline size_type_blocks size_blocks() const {
    SpinSharedMutexSharedLock<false> lock(blocks_mutex_);
    return blocks_.size();
  }

  /**
   * @brief Get the current set of blocks belonging to this relation.
   *
   * @return The block_ids of blocks belonging to this relation at the moment
   *         this method is called.
   **/
  inline std::vector<block_id> getBlocksSnapshot() const {
    SpinSharedMutexSharedLock<false> lock(blocks_mutex_);
    return std::vector<block_id>(blocks_.begin(), blocks_.end());
  }

  /**
   * @brief Set the default StorageBlockLayout for this relation.
   * @note Deletes the previous default layout, if any.
   *
   * @param default_layout The new default StorageBlockLayout for this
   *        relation, which becomes owned by this relation.
   **/
  void setDefaultStorageBlockLayout(StorageBlockLayout *default_layout);

  /**
   * @brief Get this relation's default StorageBlockLayout.
   * @note If no default has been set via setDefaultStorageBlockLayout(), then
   *       one is created with StorageBlockLayout::generateDefaultLayout().
   *
   * @return The default StorageBlockLayout for this relation.
   **/
  const StorageBlockLayout& getDefaultStorageBlockLayout() const;

  /**
   * @brief Estimate the number of tuples in this relation.
   * @warning This is currently implemented as a very dumb heuristic. Estimates
   *          should be considered a rough "best guess" only.
   *
   * @return The estimated number of tuples in this relation.
   **/
  std::size_t estimateTupleCardinality() const {
    return size_blocks()
           * getDefaultStorageBlockLayout().estimateTuplesPerBlock();
  }

  /**
   * @brief Get an immutable reference to the statistics of this catalog relation.
   *
   * @return A reference to the statistics of this catalog relation.
   */
  const CatalogRelationStatistics& getStatistics() const {
    return *statistics_;
  }

  /**
   * @brief Get a mutable pointer to the statistics of this catalog relation.
   *
   * @return A pointer to the statistics of this catalog relation.
   */
  CatalogRelationStatistics* getStatisticsMutable() {
    return statistics_.get();
  }

  /**
   * @brief Get the size of this relation in bytes.
   *
   * @note The output signifies the amount of memory allocated for this
   *       relation. The true memory footprint of this relation could be lower
   *       than the output of this method.
   **/
  inline std::size_t getRelationSizeBytes() const {
    SpinSharedMutexSharedLock<false> lock(blocks_mutex_);
    return blocks_.size() *
           getDefaultStorageBlockLayout().getDescription().num_slots() *
           kSlotSizeBytes;
  }

 private:
  // A list of blocks belonged to the relation.
  std::vector<block_id> blocks_;
  // The mutex used to protect 'blocks_' from concurrent accesses.
  mutable SpinSharedMutex<false> blocks_mutex_;

  // The default layout for newly-created blocks.
  mutable std::unique_ptr<StorageBlockLayout> default_layout_;

  // Partition Scheme associated with the Catalog Relation.
  // A relation may or may not have a Partition Scheme
  // assosiated with it.
  std::unique_ptr<PartitionScheme> partition_scheme_;
  std::size_t num_partitions_;

  // Index scheme associated with this relation.
  // Defines a set of indices defined for this relation.
  std::unique_ptr<IndexScheme> index_scheme_;
  // Mutex for locking the index scheme.
  alignas(kCacheLineBytes) mutable SpinSharedMutex<false> index_scheme_mutex_;

  std::unique_ptr<CatalogRelationStatistics> statistics_;

#ifdef QUICKSTEP_HAVE_LIBNUMA
  // NUMA placement scheme object which has the mapping between the partitions
  // of the relation and the NUMA nodes/sockets. It also maintains a mapping
  // between the blocks of the relation and the NUMA nodes..
  std::unique_ptr<NUMAPlacementScheme> placement_scheme_;
#endif  // QUICKSTEP_HAVE_LIBNUMA

  friend class CatalogTest;

  DISALLOW_COPY_AND_ASSIGN(CatalogRelation);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CATALOG_CATALOG_RELATION_HPP_
