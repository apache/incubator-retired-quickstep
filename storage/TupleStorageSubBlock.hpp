/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_STORAGE_TUPLE_STORAGE_SUB_BLOCK_HPP_
#define QUICKSTEP_STORAGE_TUPLE_STORAGE_SUB_BLOCK_HPP_

#include <cstddef>
#include <unordered_map>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/predicate/PredicateCost.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/TupleIdSequence.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class CatalogRelationSchema;
class ComparisonPredicate;
class Tuple;
class TupleStorageSubBlockDescription;
class ValueAccessor;

/** \addtogroup Storage
 *  @{
 */

// TODO(chasseur): Abstract more common safety checks into base class.
/**
 * @brief SubBlock which stores complete tuples.
 **/
class TupleStorageSubBlock {
 public:
  /**
   * @brief Structure describing the result of an insertion of a single tuple.
   **/
  struct InsertResult {
    InsertResult(const tuple_id inserted_id_arg, const bool ids_mutated_arg)
        : inserted_id(inserted_id_arg),
          ids_mutated(ids_mutated_arg) {
    }

    /**
     * @brief The ID of the inserted tuple, or -1 if unable to insert.
     **/
    tuple_id inserted_id;

    /**
     * @brief True if other tuples in the TupleStorageSubBlock had their ids
     *        mutated (requiring that indexes be rebuilt), false otherwise.
     **/
    bool ids_mutated;
  };

  /**
   * @brief Constructor.
   *
   * @param relation The CatalogRelationSchema which this SubBlock belongs to.
   * @param description A description containing any parameters needed to
   *        construct this SubBlock. Implementation-specific parameters are
   *        defined as extensions in StorageBlockLayout.proto.
   * @param new_block Whether this is a newly-created block.
   * @param sub_block_memory The memory slot to use for the block's contents.
   * @param sub_block_memory_size The size of the memory slot in bytes.
   * @exception BlockMemoryTooSmall This TupleStorageSubBlock hasn't been
   *            provided enough memory to store metadata.
   **/
  TupleStorageSubBlock(const CatalogRelationSchema &relation,
                       const TupleStorageSubBlockDescription &description,
                       const bool new_block,
                       void *sub_block_memory,
                       const std::size_t sub_block_memory_size)
                       : relation_(relation),
                         description_(description),
                         sub_block_memory_(sub_block_memory),
                         sub_block_memory_size_(sub_block_memory_size) {
  }

  /**
   * @brief Virtual destructor.
   **/
  virtual ~TupleStorageSubBlock() {
  }

  /**
   * @brief Get the CatalogRelationSchema this TupleStorageSubBlock belongs to.
   *
   * @return The relation this TupleStorageSubBlock belongs to.
   **/
  inline const CatalogRelationSchema& getRelation() const {
    return relation_;
  }

  /**
   * @brief Identify the type of this TupleStorageSubBlock.
   *
   * @return This TupleStorageSubBlock's type.
   **/
  virtual TupleStorageSubBlockType getTupleStorageSubBlockType() const = 0;

  /**
   * @brief Determine whether this SubBlock supports the getAttributeValue()
   *        method to get an untyped pointer to a value for a particular
   *        attribute.
   *
   * @param attr The ID of the attribute which getAttributeValue() would be
   *             used with.
   * @return Whether the getAttributeValue() method can be used on this
   *         SubBlock with the specified attr.
   **/
  virtual bool supportsUntypedGetAttributeValue(const attribute_id attr) const = 0;

  /**
   * @brief Determine whether this SubBlock supports ad-hoc insertion of
   *        individual tuples via the insertTuple() method.
   * @note If this method returns false, then tuples can only be inserted via
   *       insertTupleInBatch(), which should be followed by a call to
   *       rebuild() when a batch is fully inserted.
   * @note Even if this method returns false, it is still legal to call
   *       insertTuple(), although it will always fail to actually insert.
   *
   * @return Whether the insertTuple() can be used on this SubBlock.
   **/
  virtual bool supportsAdHocInsert() const = 0;

  /**
   * @brief Determine whether inserting tuples one-at-a-time via the
   *        insertTuple() method is efficient (i.e. has constant time and space
   *        costs and does not require expensive reorganization of other tuples
   *        in this SubBlock).
   *
   * @return Whether insertTuple() is efficient for this SubBlock.
   **/
  virtual bool adHocInsertIsEfficient() const = 0;

  /**
   * @brief Determine whether this block has any tuples in it.
   *
   * @return True if this SubBlock is empty, false otherwise.
   **/
  virtual bool isEmpty() const = 0;

  /**
   * @brief Determine whether this block is packed, i.e. there are no holes in
   *        the tuple-id sequence.
   *
   * @return True if this SubBlock is packed, false otherwise.
   **/
  virtual bool isPacked() const = 0;

  /**
   * @brief Get the highest tuple-id of a valid tuple in this SubBlock.
   *
   * @return The highest tuple-id of a tuple stored in this SubBlock (-1 if
   *         this SubBlock is empty).
   **/
  virtual tuple_id getMaxTupleID() const = 0;

  /**
   * @brief Get the number of tuples contained in this SubBlock.
   * @note This method returns tuple_id for ease of comparison with other
   *       tuple_id values referring to tuples in this block. The return value
   *       will always be non-negative, though.
   * @note The default implementation is O(1) for packed TupleStorageSubBlocks,
   *       but TupleStorageSubBlock implementations which may be non-packed
   *       should override this where possible, as the default version of this
   *       method is O(N) when the SubBlock is non-packed.
   *
   * @return The number of tuples contained in this TupleStorageSubBlock.
   **/
  virtual tuple_id numTuples() const;

  /**
   * @brief Determine whether a tuple with the given id exists in this
   *        SubBlock.
   *
   * @param tuple The ID to check.
   * @return True if tuple exists, false if it does not.
   **/
  virtual bool hasTupleWithID(const tuple_id tuple) const = 0;

  /**
   * @brief Insert a single tuple into this TupleStorageSubBlock.
   *
   * @param tuple The tuple to insert, whose values must be in the correct
   *        order.
   * @return The result of the insertion.
   **/
  virtual InsertResult insertTuple(const Tuple &tuple) = 0;

  /**
   * @brief Insert a single tuple as part of a batch.
   * @note This method is intended to allow a large number of tuples to be
   *       loaded without incurring the full cost of maintaining a "clean"
   *       internal block structure. Once a batch of tuples have been inserted,
   *       rebuild() should be called to put this TupleStorageSubBlock into a
   *       consistent state.
   * @warning The inserted tuple may be placed in an "incorrect" or sub-optimal
   *          location in this TupleStorageSubBlock. The only methods which are
   *          safe to call between bulkInsertTuples() and rebuild() are
   *          insertTupleInBatch(), bulkInsertTuples(), and
   *          bulkInsertTuplesWithRemappedAttributes().
   *
   * @param tuple The tuple to insert, whose values must be in the correct
   *        order.
   * @return True if the insertion was successful, false if out of space.
   **/
  virtual bool insertTupleInBatch(const Tuple &tuple) = 0;

  /**
   * @brief Insert as many tuples as possible from a ValueAccessor (all of the
   *        tuples accessible or as many as will fit in this
   *        TupleStorageSubBlock) as a single batch.
   *
   * @warning The inserted tuples may be placed in an "incorrect" or
   *          sub-optimal locations in this TupleStorageSubBlock. The only
   *          methods which are safe to call between bulkInsertTuples() and
   *          rebuild() are insertTupleInBatch(), bulkInsertTuples(), and
   *          bulkInsertTuplesWithRemappedAttributes().
   *
   * @param accessor A ValueAccessor to insert tuples from. This method assumes
   *        that the attributes in the ValueAccessor are in exactly the same
   *        order (and the same type as) the attributes of this
   *        TupleStorageStorageBlock's relation. The accessor's iteration will
   *        be advanced to the first non-inserted tuple or, if all accessible
   *        tuples were inserted in this sub-block, to the end position.
   * @return The number of tuples inserted from accessor.
   **/
  virtual tuple_id bulkInsertTuples(ValueAccessor *accessor) = 0;

  /**
   * @brief Insert as many tuples as possible from a ValueAccessor (all of the
   *        tuples accessible or as many as will fit in this
   *        TupleStorageSubBlock) as a single batch. This version remaps
   *        attribute IDs from the ValueAccessor to the correct order of
   *        attributes for this StorageBlock's relation.
   *
   * @warning The inserted tuples may be placed in an "incorrect" or
   *          sub-optimal locations in this TupleStorageSubBlock. The only
   *          methods which are safe to call between bulkInsertTuples() and
   *          rebuild() are insertTupleInBatch(), bulkInsertTuples(), and
   *          bulkInsertTuplesWithRemappedAttributes().
   *
   * @param attribute_map A vector which maps the attributes of this
   *        TupleStorageSubBlock's relation (in order with no gaps) to the
   *        corresponding attributes which should be read from accessor.
   * @param accessor A ValueAccessor to insert tuples from. The accessor's
   *        iteration will be advanced to the first non-inserted tuple or, if
   *        all accessible tuples were inserted in this sub-block, to the end
   *        position.
   * @return The number of tuples inserted from accessor.
   **/
  virtual tuple_id bulkInsertTuplesWithRemappedAttributes(
      const std::vector<attribute_id> &attribute_map,
      ValueAccessor *accessor) = 0;

  /**
   * @brief Get the (untyped) value of an attribute in a tuple in this buffer.
   * @warning This method may not be supported for all implementations of
   *          TupleStorageSubBlock. supportsUntypedGetAttributeValue() MUST be
   *          called first to determine if this method is usable.
   * @warning For debug builds, an assertion checks whether the specified tuple
   *          actually exists. In general, this method should only be called
   *          for tuples which are known to exist (from hasTupleWithID(),
   *          isPacked() and getMaxTupleID(), or presence in an index).
   *
   * @param tuple The desired tuple in this SubBlock.
   * @param attr The attribute id of the desired attribute.
   * @return An untyped pointer to the value of the specified attribute in the
   *         specified tuple.
   **/
  virtual const void* getAttributeValue(const tuple_id tuple, const attribute_id attr) const = 0;

  /**
   * @brief Get the value of the specified attribute of the specified tuple as
   *        a TypedValue.
   * @warning For debug builds, an assertion checks whether the specified tuple
   *          actually exists. In general, this method should only be called
   *          for tuples which are known to exist (from hasTupleWithID(),
   *          isPacked() and getMaxTupleID(), or presence in an index).
   *
   * @param tuple The desired tuple in this SubBlock.
   * @param attr The attribute id of the desired attribute.
   * @return The data as a TypedValue.
   **/
  virtual TypedValue getAttributeValueTyped(const tuple_id tuple, const attribute_id attr) const = 0;

  /**
   * @brief Create a ValueAccessor object that can read attribute values from
   *        within this TupleStorageSubBlock.
   *
   * @param sequence If non-null, the ValueAccessor returned with only iterate
   *        over the tuples in sequence. Note that the returned ValueAccessor
   *        will only be valid so long as sequence exists.
   * @return A ValueAccessor object which can iterate over and read values from
   *         this TupleStorageSubBlock.
   **/
  virtual ValueAccessor* createValueAccessor(const TupleIdSequence *sequence = nullptr) const = 0;

  /**
   * @brief Determine whether it is possible to update some attribute values
   *        in-place without relocating the tuple.
   *
   * @param tuple The desired tuple in this SubBlock.
   * @param new_values A map of attribute IDs to corresponding new (typed)
   *        values.
   * @return True if an in-place update for all values in new_values is
   *         possible, false otherwise.
   **/
  virtual bool canSetAttributeValuesInPlaceTyped(
      const tuple_id tuple,
      const std::unordered_map<attribute_id, TypedValue> &new_values) const = 0;

  /**
   * @brief Set (update) the value of an attribute in an existing tuple.
   * @warning For debug builds, an assertion checks whether the specified tuple
   *          actually exists. In general, this method should only be called
   *          for tuples which are known to exist (from hasTupleWithID(),
   *          isPacked() and getMaxTupleID(), or presence in an index).
   * @warning Sometimes it is not possible to update an attribute value
   *          in-place, and the tuple must be relocated. ALWAYS call
   *          canSetAttributeValuesInPlaceTyped() first to check whether this
   *          method is safe to use.
   *
   * @param tuple The desired tuple in this SubBlock.
   * @param attr The attribute id of the desired attribute.
   * @param value A TypedValue containing the new value for the attribute.
   **/
  virtual void setAttributeValueInPlaceTyped(const tuple_id tuple,
                                             const attribute_id attr,
                                             const TypedValue &value) = 0;

  /**
   * @brief Delete a single tuple from this TupleStorageSubBlock.
   * @warning For debug builds, an assertion checks whether the specified tuple
   *          actually exists. In general, this method should only be called
   *          for tuples which are known to exist (from hasTupleWithID(),
   *          isPacked() and getMaxTupleID(), or presence in an index).
   * @warning Always check the return value of this call to see whether indexes
   *          must be totally rebuilt.
   *
   * @param tuple The tuple to delete.
   * @return True if other tuples have had their ids mutated (requiring indexes
   *         to be rebuilt), false if other tuple IDs are stable.
   **/
  virtual bool deleteTuple(const tuple_id tuple) = 0;

  /**
   * @brief Delete multiple tuples from this TupleStorageSubBlock.
   * @warning For debug builds, an assertion checks whether the specified
   *          tuples actually exist. In general, this method should only be
   *          called for tuples which are known to exist (from
   *          hasTupleWithID(), isPacked() and getMaxTupleID(), or presence
   *          in an index).
   * @warning Always check the return value of this call to see whether indexes
   *          must be totally rebuilt.
   *
   * @param tuples A sequence of tuple IDs which indicate the tuples to be
   *        deleted.
   * @return True if other tuples have had their ids mutated (requiring
   *         indexes to be rebuilt), false if other tuple IDs are stable.
   **/
  virtual bool bulkDeleteTuples(TupleIdSequence *tuples) = 0;

  /**
   * @brief Estimate the cost of evaluating a ComparisonPredicate on this
   *        TupleStorageStorageBlock.
   *
   * @param predicate The predicate to be evaluated.
   * @return A cost estimate for evaluating the predicate using this
   *         TupleStorageSubBlock's getMatchesForPredicate() method. Lower
   *         values indicate a cheaper/faster estimated cost.
   **/
  virtual predicate_cost_t estimatePredicateEvaluationCost(
      const ComparisonPredicate &predicate) const = 0;

  /**
   * @brief Get the IDs of tuples in this SubBlock which match a given
   *        ComparisonPredicate using a "fast-path" technique specific to this
   *        TupleStorageStorageBlock.
   * @warning This should only be called if estimatePredicateEvaluationCost()
   *          returns a value OTHER than kInfinite, kRowScan, or kColumnScan
   *          for the predicate.
   *
   * @param predicate The predicate to match.
   * @param filter If non-NULL, then only tuple IDs which are set in the
   *        filter will be checked (all others will be assumed to be false).
   * @return The IDs of tuples matching the specified predicate.
   **/
  virtual TupleIdSequence* getMatchesForPredicate(const ComparisonPredicate &predicate,
                                                  const TupleIdSequence *filter) const {
    LOG(FATAL) << "Called TupleStorageSubBlock::getMatchesForPredicate() on a "
               << "TupleStorageStorageBlock that does not provide any non-scan "
               << "method for evaluating predicates.";
  }

  /**
   * @brief Get the IDs of all tuples which exist in this SubBlock.
   * @note As with numTuples(), the default implementation of this method is
   *       O(1) for packed blocks, but O(n) for non-packed blocks. New
   *       TupleStorageSubBlock implementations which are not packed should
   *       override this with a faster version if possible.
   *
   * @return The IDs of all tuples which exist in this SubBlock.
   **/
  virtual TupleIdSequence* getExistenceMap() const;

  /**
   * @brief Get the IDs of all tuples which exist in this SubBlock.
   * @note The default implementation of this method is O(n) for both packed and
   *       non-packed blocks. However, with packed with packed blocks there is
   *       not branching in the tight loop.
   *
   * @return The list of IDs of all tuples which exist in this SubBlock.
   **/
  virtual OrderedTupleIdSequence* getExistenceList() const;

  /**
   * @brief Rebuild this TupleStorageSubBlock, compacting storage and
   *        reordering tuples where applicable.
   * @note This method may use an unbounded amount of out-of-band memory.
   **/
  virtual void rebuild() = 0;

  /**
   * @brief Check if this TupleStorageSubBlock is compressed, i.e. whether it
   *        can safely be cast to CompressedTupleStorageSubBlock.
   *
   * @return true if this is a CompressedTupleStorageSubBlock, false otherwise.
   **/
  virtual bool isCompressed() const {
    return false;
  }

  /**
   * @brief Check if the instance of tuple storage sub-block order preserving.
   *        This is useful in merging of sorted runs, where insertion order
   *        should be preserved.
   *
   * @return \c true if this tuple storage sub-block is order preserving.
   **/
  virtual bool isInsertOrderPreserving() const = 0;

 protected:
  /**
   * @brief In debug builds, performs assertions to make sure that the values
   *        in tuple can be inserted into this TupleStorageSubBlock.
   *
   * @param tuple The tuple to check.
   **/
  void paranoidInsertTypeCheck(const Tuple &tuple);

  const CatalogRelationSchema &relation_;
  const TupleStorageSubBlockDescription &description_;

  void *sub_block_memory_;
  const std::size_t sub_block_memory_size_;

 private:
  DISALLOW_COPY_AND_ASSIGN(TupleStorageSubBlock);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_TUPLE_STORAGE_SUB_BLOCK_HPP_
