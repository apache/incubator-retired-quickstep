/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_STORAGE_STORAGE_BLOCK_HPP_
#define QUICKSTEP_STORAGE_STORAGE_BLOCK_HPP_

#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/CountedReference.hpp"
#include "storage/HashTableBase.hpp"
#include "storage/IndexSubBlock.hpp"
#include "storage/StorageBlockBase.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/TupleIdSequence.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"

namespace quickstep {

class AggregationHandle;
class AggregationState;
class CatalogRelationSchema;
class ColumnVector;
class InsertDestinationInterface;
class Predicate;
class Scalar;
class StorageBlockLayout;
class Tuple;
class TypedValue;
class ValueAccessor;

namespace storage_explorer {
class BlockBasedQueryExecutor;
}

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief Top-level storage block, which contains exactly one
 *        TupleStorageSubBlock and any number of IndexSubBlocks.
 **/
class StorageBlock : public StorageBlockBase {
 public:
  /**
   * @brief The return value of a call to update().
   **/
  struct UpdateResult {
    /**
     * @brief Whether this StorageBlock's IndexSubBlocks remain consistent
     *        after the call to update().
     **/
    bool indices_consistent;

    /**
     * @brief Whether some tuples were moved to relocation_destination.
     **/
    bool relocation_destination_used;
  };

  /**
   * @brief Constructor.
   *
   * @param relation The CatalogRelationSchema this block belongs to.
   * @param id The ID of this block.
   * @param layout The StorageBlockLayout to use for this block. This is used
   *        ONLY for a newly-created block, and is ignored if new_block is
   *        false. If unsure, just use relation.getDefaultStorageBlockLayout().
   * @param new_block Whether this is a newly-created block.
   * @param block_memory The memory slot to use for the block's contents.
   * @param block_memory_size The size of the memory slot in bytes.
   * @exception MalformedBlock new_block is false and the provided block_memory
   *            appears corrupted.
   * @exception BlockMemoryTooSmall This StorageBlock or one of its subblocks
   *            hasn't been provided enough memory to store metadata.
   **/
  StorageBlock(const CatalogRelationSchema &relation,
               const block_id id,
               const StorageBlockLayout &layout,
               const bool new_block,
               void *block_memory,
               const std::size_t block_memory_size);

  /**
   * @brief Destructor.
   **/
  ~StorageBlock() override {}

  bool isBlob() const override {
    return false;
  }

  /**
   * @brief Determine whether this StorageBlock supports ad-hoc insertion of
   *        individual tuples via the insertTuple() method.
   * @note If this method returns false, then tuples can only be inserted via
   *       insertTupleInBatch(), which should be followed by a call to
   *       rebuild() when a batch is fully inserted.
   * @note Even if this method returns false, it is still legal to call
   *       insertTuple(), although it will always fail to actually insert.
   *
   * @return Whether the insertTuple() can be used on this StorageBlock.
   **/
  bool supportsAdHocInsert() const {
    return ad_hoc_insert_supported_;
  }

  /**
   * @brief Determine whether inserting tuples one-at-a-time via the
   *        insertTuple() method is efficient (i.e. has reasonable time and
   *        space costs and does not require expensive reorganization of other
   *        tuples or rebuilding indices).
   *
   * @return Whether insertTuple() is efficient for this StorageBlock.
   **/
  bool adHocInsertIsEfficient() const {
    return ad_hoc_insert_efficient_;
  }

  /**
   * @brief Determine whether the IndexSubBlocks in this StorageBlock (if any)
   *        are up-to-date and consistent with the complete contents of the
   *        TupleStorageSubBlock.
   * @note Indices are usually kept consistent, except during batch-insertion
   *       using calls to the insertTupleInBatch() method, in which case
   *       indices are inconsistent until rebuild() is called.
   * @warning If insufficient space is allocated for IndexSubBlocks, it is
   *          possible for indices to become inconsistent in a block which is
   *          used in a destination for select(), selectSimple(), or update().
   *          It is also possible for a StorageBlock which update() is called
   *          on to have its indices become inconsistent in some unusual edge
   *          cases. In all such cases, the TupleStorageSubBlock will remain
   *          consistent and accessible.
   *
   * @return Whether all IndexSubBlocks in this StorageBlock are consistent.
   **/
  bool indicesAreConsistent() const {
    return all_indices_consistent_;
  }

  /**
   * @brief Get the CatalogRelationSchema this block belongs to.
   *
   * @return The relation this block belongs to.
   **/
  const CatalogRelationSchema& getRelation() const {
    return relation_;
  }

  /**
   * @brief Get this block's TupleStorageSubBlock.
   *
   * @return This block's TupleStorageSubBlock.
   **/
  inline const TupleStorageSubBlock& getTupleStorageSubBlock() const {
    return *tuple_store_;
  }

  inline std::size_t numIndexSubBlocks() const {
    return indices_.size();
  }

  inline bool indexIsConsistent(const std::size_t index_id) const {
    DEBUG_ASSERT(index_id < indices_consistent_.size());
    return indices_consistent_[index_id];
  }

  /**
   * @brief Get one of this block's IndexSubBlocks.
   *
   * @param index_id The ID of the IndexSubBlock. This is simply a serial
   *        counter for the IndexSubBlocks inside this block, and does not
   *        necessarily correspond to the attribute_id(s) of attributes that
   *        are covered by the index.
   * @return The specified IndexSubBlock in this block.
   **/
  inline const IndexSubBlock& getIndexSubBlock(const std::size_t index_id) const {
    DEBUG_ASSERT(index_id < indices_.size());
    return indices_[index_id];
  }

  /**
   * @brief Insert a single tuple into this block.
   *
   * @param tuple The tuple to insert.
   * @return true if the tuple was successfully inserted, false if insertion
   *         failed (e.g. because of not enough space).
   * @exception TupleTooLargeForBlock Even though this block was initially
   *            empty, the tuple was too large to insert. Only thrown if block
   *            is initially empty, otherwise failure to insert simply returns
   *            false.
   **/
  bool insertTuple(const Tuple &tuple);

  /**
   * @brief Insert a single tuple into this block as part of a batch.
   * @warning A tuple inserted via this method may be placed in an "incorrect"
   *          or sub-optimal location in this block's TupleStorageSubBlock, and
   *          the IndexSubBlocks in this block are not updated to account for
   *          the new tuple. rebuild() MUST be called on this block after calls
   *          to this method to put the block back into a consistent state.
   * @warning Depending on the relative sizes of sub-blocks allocated by this
   *          block's StorageBlockLayout, it is possible to over-fill a block
   *          with more tuples than can be stored in its indexes when rebuild()
   *          is called.
   *
   * @param tuple The tuple to insert.
   * @return true if the tuple was successfully inserted, false if insertion
   *         failed (e.g. because of not enough space).
   * @exception TupleTooLargeForBlock Even though this block was initially
   *            empty, the tuple was too large to insert. Only thrown if block
   *            is initially empty, otherwise failure to insert simply returns
   *            false.
   **/
  bool insertTupleInBatch(const Tuple &tuple);

  /**
   * @brief Insert as many tuples as possible from a ValueAccessor (all of the
   *        tuples accessible or as many as will fit in this StorageBlock) as a
   *        single batch.
   *
   * @warning Tuples inserted via this method may be placed in an "incorrect"
   *          or sub-optimal location in this block's TupleStorageSubBlock, and
   *          the IndexSubBlocks in this block are not updated to account for
   *          new tuples. rebuild() MUST be called on this block after calls
   *          to this method to put the block back into a consistent state.
   * @warning Depending on the relative sizes of sub-blocks allocated by this
   *          block's StorageBlockLayout, it is possible to over-fill a block
   *          with more tuples than can be stored in its indexes when rebuild()
   *          is called.
   *
   * @param accessor A ValueAccessor to insert tuples from. This method assumes
   *        that the attributes in the ValueAccessor are in exactly the same
   *        order (and the same type as) the attributes of this StorageBlock's
   *        relation. The accessor's iteration will be advanced to the first
   *        non-inserted tuple or, if all accessible tuples were inserted in
   *        this block, to the end position.
   * @return The number of tuples inserted from accessor.
   **/
  tuple_id bulkInsertTuples(ValueAccessor *accessor);

  /**
   * @brief Insert as many tuples as possible from a ValueAccessor (all of the
   *        tuples accessible or as many as will fit in this StorageBlock) as a
   *        single batch. This version remaps attribute IDs from the
   *        ValueAccessor to the correct order of attributes for this
   *        StorageBlock's relation.
   *
   * @warning Tuples inserted via this method may be placed in an "incorrect"
   *          or sub-optimal location in this block's TupleStorageSubBlock, and
   *          the IndexSubBlocks in this block are not updated to account for
   *          new tuples. rebuild() MUST be called on this block after calls
   *          to this method to put the block back into a consistent state.
   * @warning Depending on the relative sizes of sub-blocks allocated by this
   *          block's StorageBlockLayout, it is possible to over-fill a block
   *          with more tuples than can be stored in its indexes when rebuild()
   *          is called.
   *
   * @param attribute_map A vector which maps the attributes of this
   *        StorageBlock's relation (in order with no gaps) to the
   *        corresponding attributes which should be read from accessor.
   * @param accessor A ValueAccessor to insert tuples from. The accessor's
   *        iteration will be advanced to the first non-inserted tuple or, if
   *        all accessible tuples were inserted in this block, to the end
   *        position.
   * @return The number of tuples inserted from accessor.
   **/
  tuple_id bulkInsertTuplesWithRemappedAttributes(
      const std::vector<attribute_id> &attribute_map,
      ValueAccessor *accessor);

  /**
   * @brief Perform a SELECT query on this StorageBlock.
   *
   * @param selection A list of scalars, which will be evaluated to obtain
   *        attribute values for each result tuple.
   * @param predicate A predicate for selection. NULL indicates that all tuples
   *        should be matched.
   * @param destination Where to insert the tuples resulting from the SELECT
   *        query.
   * @exception TupleTooLargeForBlock A tuple produced by this selection was
   *            too large to insert into an empty block provided by
   *            destination. Selection may be partially complete (with some
   *            tuples inserted into destination) when this exception is
   *            thrown, causing potential inconsistency.
   *
   **/
  void select(const std::vector<std::unique_ptr<const Scalar>> &selection,
              const Predicate *predicate,
              InsertDestinationInterface *destination) const;

  /**
   * @brief Perform a simple SELECT query on this StorageBlock which only
   *        projects attributes and does not evaluate expressions.
   *
   * @param destination Where to insert the tuples resulting from the SELECT
   *        query.
   * @param selection The attributes to project.
   * @param predicate A predicate for selection. NULL indicates that all tuples
   *        should be matched.
   * @exception TupleTooLargeForBlock A tuple produced by this selection was
   *            too large to insert into an empty block provided by
   *            destination. Selection may be partially complete (with some
   *            tuples inserted into destination) when this exception is
   *            thrown, causing potential inconsistency.
   *
   * @return true if selection completed with no issues, false if one or more
   *         of the blocks provided by '*destination' was left with
   *         an inconsistent IndexSubBlock (see indicesAreConsistent()).
   **/
  void selectSimple(const std::vector<attribute_id> &selection,
                    const Predicate *predicate,
                    InsertDestinationInterface *destination) const;

  /**
   * @brief Perform non GROUP BY aggregation on the tuples in the this storage
   *        block, returning the aggregated result (for this block) in an
   *        AggregationState.
   *
   * @param handle Aggregation handle that will be used to compute aggregate.
   * @param arguments The arguments of the aggregate function as expressions.
   * @param arguments_as_attributes If non-NULL, indicates a valid attribute_id
   *        for each of the elements in arguments, and is used to elide a copy.
   *        Has no effect if NULL, or if VECTOR_COPY_ELISION_LEVEL is NONE.
   * @param predicate A predicate for selection. nullptr indicates that all
   *        tuples should be aggregated on.
   * @param reuse_matches This parameter is used to store and reuse tuple-id
   *        sequence of matches pre-computed in an earlier invocations to
   *        aggregate(). \c reuse_matches is never \c nullptr for ease of use.
   *        Current invocation of aggregate() will reuse TupleIdSequence if
   *        passed, otherwise compute a TupleIdSequence based on \c predicate
   *        and store in \c reuse_matches. We use std::unique_ptr for each of
   *        use, since the caller will not have to selective free.
   *
   * For example, see this relevant pseudo-C++ code:
   * \code
   * std::unique_ptr<TupleIdSequence> matches;
   * for each aggregate {
   *   block.aggregate(..., &matches);
   * }
   * \endcode
   *
   * @return Aggregated state for this block in the form of an
   *         AggregationState. AggregationHandle::mergeStates() can be called
   *         to merge with states from other blocks, and
   *         AggregationHandle::finalize() can be used to generate a final
   *         result.
   **/
  AggregationState* aggregate(
      const AggregationHandle &handle,
      const std::vector<std::unique_ptr<const Scalar>> &arguments,
      const std::vector<attribute_id> *arguments_as_attributes,
      const Predicate *predicate,
      std::unique_ptr<TupleIdSequence> *reuse_matches) const;

  /**
   * @brief Perform GROUP BY aggregation on the tuples in the this storage
   *        block.
   *
   * @param handle Aggregation handle to compute aggregates with.
   * @param arguments The arguments to the aggregation function as Scalars.
   * @param group_by The list of GROUP BY attributes/expressions. The tuples in
   *        this storage block are grouped by these attributes before
   *        aggregation.
   * @param predicate A predicate for selection. nullptr indicates that all
   *        tuples should be aggregated on.
   * @param hash_table Hash table to store aggregation state mapped based on
   *        GROUP BY value list (defined by \c group_by).
   * @param reuse_matches This parameter is used to store and reuse tuple-id
   *        sequence of matches pre-computed in an earlier invocations of
   *        aggregateGroupBy(). \c reuse_matches is never \c nullptr for ease of
   *        use.  Current invocation of aggregateGroupBy() will reuse
   *        TupleIdSequence if passed, otherwise computes a TupleIdSequence based
   *        on \c predicate and stores in \c reuse_matches. We use
   *        std::unique_ptr for each of use, since the caller will not have to
   *        selective free.
   * @param reuse_group_by_vectors This parameter is used to store and reuse
   *        GROUP BY attribute vectors pre-computed in an earlier invocation of
   *        aggregateGroupBy(). \c reuse_group_by_vectors is never \c nullptr
   *        for ease of use. Current invocation of aggregateGroupBy() will reuse
   *        ColumnVectors if non-empty, otherwise computes ColumnVectors based
   *        on \c group_by and stores them in \c reuse_group_by_vectors.
   *
   * For sample usage of aggregateGroupBy, see this relevant pseudo-C++ code:
   * \code
   * std::unique_ptr<TupleIdSequence> matches;
   * std::vector<std::unique_ptr<ColumnVector>> group_by_vectors;
   * for each aggregate {
   *   block.aggregateGroupBy(..., &matches, &group_by_vectors);
   * }
   * \endcode
   **/
  /*
   * TODO(shoban): Currently, we use ColumnVectorsValueAccessor to compute
   * temporary result for Scalars of aggregation attributes and GROUP BY
   * attributes.  We will have to support specifying aggregation and GROUP BY
   * attributes as std::vector<attribute_id> (like in selectSimple()) for fast
   * path when there are no expressions specified in the query.
   */
  void aggregateGroupBy(const AggregationHandle &handle,
                        const std::vector<std::unique_ptr<const Scalar>> &arguments,
                        const std::vector<std::unique_ptr<const Scalar>> &group_by,
                        const Predicate *predicate,
                        AggregationStateHashTableBase *hash_table,
                        std::unique_ptr<TupleIdSequence> *reuse_matches,
                        std::vector<std::unique_ptr<ColumnVector>>
                            *reuse_group_by_vectors) const;

  /**
   * @brief Perform an UPDATE query over the tuples in this StorageBlock.
   * @warning In some edge cases, calling this method may cause IndexSubBlocks
   *          in this block to become inconsistent (the TupleStorageSubBlock
   *          will remain consistent and well-formed, however). Also see
   *          UpdateResult and indicesAreConsistent().
   *
   * @param assignments A map of attribute_ids to Scalars which should be
   *        evaluated to get the new value for the corresponding attribute.
   * @param predicate A predicate for the update. NULL indicates that all
   *        tuples should be matched.
   * @param relocation_destination Any tuples that can not be updated in-place
   *        will be removed from this block and inserted into a block provided
   *        by relocation_destination.
   * @return A structure which indicates whether this block's indices remain
   *         consistent, whether relocation_destination was used, and whether
   *         blocks provided by relocation_destination have consistent indices.
   **/
  UpdateResult update(const std::unordered_map<attribute_id, std::unique_ptr<const Scalar>> &assignments,
                      const Predicate *predicate,
                      InsertDestinationInterface *relocation_destination);

  /**
   * @brief Sort the tuples in this storage block and return a sequence of
   *        tuple-ids. Note that this method does not reorder the tuples in the
   *        storage block based on the sorted sequence.
   *
   * @param order_by List of Scalars to sort the block by. This corresponds to
   *        list of ORDER BY attribute list. These must all be
   *        ScalarAttributes.
   * @param sort_is_ascending List of boolean corresponding to each Scalar in \c
   *        order_by. The boolean indicates if the Scalar is sorted in
   *        ascending (true) or descending (false).
   * @param null_first List of boolean corresponding to each Scalar in \c
   *        order_by. The boolean indicates if the NULLs appear at the beginning
   *        (true) or end (false).
   * @param sorted_sequence Output that holds the sorted tuple-id sequence.
   * @param output_destination Destination to write the sorted tuples output
   *        to. This argument can be nullptr, in which case no output is
   *        written.
   */
  void sort(const PtrVector<Scalar> &order_by,  // NOLINT(build/include_what_you_use)
            const std::vector<bool> &sort_is_ascending,
            const std::vector<bool> &null_first,
            OrderedTupleIdSequence *sorted_sequence,
            InsertDestinationInterface *output_destination) const;

  /**
   * @brief Delete tuples (optionally matching a predicate) from this
   *        StorageBlock.
   *
   * @param predicate Delete tuples matching predicate (NULL indicates all
   *        tuples should be deleted).
   **/
  void deleteTuples(const Predicate *predicate);

  /**
   * @brief Rebuild all SubBlocks in this StorageBlock, compacting storage
   *        and reordering tuples where applicable and rebuilding indexes from
   *        scratch.
   * @note This method may use an unbounded amount of out-of-band memory.
   * @note Even when rebuilding fails, the TupleStorageSubBlock will be
   *       consistent, and all tuples can be accessed via
   *       getTupleStorageSubBlock().
   *
   * @return True if rebuilding succeeded, false if one of the IndexSubBlocks
   *         ran out of space.
   **/
  bool rebuild() {
    tuple_store_->rebuild();
    return rebuildIndexes(false);
  }

 private:
  static TupleStorageSubBlock* CreateTupleStorageSubBlock(
      const CatalogRelationSchema &relation,
      const TupleStorageSubBlockDescription &description,
      const bool new_block,
      void *sub_block_memory,
      const std::size_t sub_block_memory_size);

  static IndexSubBlock* CreateIndexSubBlock(
      const TupleStorageSubBlock &tuple_store,
      const IndexSubBlockDescription &description,
      const bool new_block,
      void *sub_block_memory,
      const std::size_t sub_block_memory_size);

  // Attempt to add an entry for 'new_tuple' to all of the IndexSubBlocks in
  // this StorageBlock. Returns true if entries were successfully added, false
  // otherwise. Removes 'new_tuple' from the TupleStorageSubBlock if entries
  // could not be added.
  bool insertEntryInIndexes(const tuple_id new_tuple);

  // Attempt to add entries for '*new_tuples' to all of the IndexSubBlocks in
  // this StorageBlock. Returns true if entries were successfully added, false
  // otherwise. If 'roll_back_on_failure' is true, removes 'new_tuples' from
  // the TupleStorageSubBlock and any successfully-modified IndexSubBlocks if
  // entries could not be added to any index. If 'roll_back_on_failure' is
  // false, and entries could not be added to an index, this method will not
  // modify the TupleStorageSubBlock and will still attempt to add entries to
  // all possible IndexSubBlocks.
  bool bulkInsertEntriesInIndexes(TupleIdSequence *new_tuples,
                                  const bool roll_back_on_failure);

  // Rebuild all IndexSubBlocks in this StorageBlock. Returns true if all
  // indexes were successfully rebuilt, false if any failed. If 'short_circuit'
  // is true, immediately stops and returns when an IndexSubBlock fails to
  // rebuild, without rebuilding any subsequent IndexSubBlocks or updating this
  // StorageBlock's header.
  bool rebuildIndexes(bool short_circuit);

  TupleIdSequence* getMatchesForPredicate(const Predicate *predicate) const;

  std::unordered_map<attribute_id, TypedValue>* generateUpdatedValues(
      const ValueAccessor &accessor,
      const tuple_id tuple,
      const std::unordered_map<attribute_id, std::unique_ptr<const Scalar>> &assignments) const;

  AggregationState* aggregateHelperColumnVector(
      const AggregationHandle &handle,
      const std::vector<std::unique_ptr<const Scalar>> &arguments,
      const TupleIdSequence *matches) const;

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  AggregationState* aggregateHelperValueAccessor(
      const AggregationHandle &handle,
      const std::vector<attribute_id> &argument_ids,
      const TupleIdSequence *matches) const;
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

  // Sort the tuples in storage block based on `sort_attribute'. If
  // `use_input_sequence' is set, we assume a pre-existing order of tuple-id
  // sequence specified by `sorted_sequence' and use stable sort to maintain
  // that order. Otherwise, we use all the tuples in the block and identify
  // no-existing order of tuples. The output sorted sequence is stored in
  // `sorted_sequence'. (Note that it serves as input too when
  // `use_input_sequence' is set.)
  void sortColumn(bool use_input_sequence,
                  const Scalar &sort_attribute,
                  bool sort_is_ascending,
                  bool null_first,
                  OrderedTupleIdSequence *sorted_sequence) const;

  void updateHeader();
  void invalidateAllIndexes();

  StorageBlockHeader block_header_;
  // Exactly corresponds to repeated field index_consistent in StorageBlockHeader:
  std::vector<bool> indices_consistent_;
  bool all_indices_consistent_;
  bool all_indices_inconsistent_;

  const CatalogRelationSchema &relation_;

  std::unique_ptr<TupleStorageSubBlock> tuple_store_;
  PtrVector<IndexSubBlock> indices_;

  bool ad_hoc_insert_supported_;
  bool ad_hoc_insert_efficient_;

  friend class storage_explorer::BlockBasedQueryExecutor;

  DISALLOW_COPY_AND_ASSIGN(StorageBlock);
};

/**
 * @brief A managed reference to a mutable block.
 **/
using MutableBlockReference = CountedReference<StorageBlock>;

/**
 * @brief A managed reference to an immutable block.
 **/
using BlockReference = CountedReference<const StorageBlock>;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_STORAGE_BLOCK_HPP_
