/**
 *   Copyright 2016 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_STORAGE_SMA_INDEX_SUB_BLOCK_HPP_
#define QUICKSTEP_STORAGE_SMA_INDEX_SUB_BLOCK_HPP_

#include <cstddef>
#include <exception>
#include <vector>
#include <iostream>
#include <string>

#include "catalog/CatalogAttribute.hpp"
#include "expressions/aggregation/AggregationHandleSum.hpp"
#include "expressions/predicate/PredicateCost.hpp"
#include "storage/IndexSubBlock.hpp"
#include "storage/StorageConstants.hpp"
#include "storage/SubBlockTypeRegistryMacros.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"

using std::string; 

namespace quickstep {

class SMAIndexSubBlock;

QUICKSTEP_DECLARE_SUB_BLOCK_TYPE_REGISTERED(SMAIndexSubBlock);

namespace smaindex_internal {

/**
 * @brief A simple holding struct for a comparison predicate.
 * @details The selectivity enum indicates if the SMA has been used to solve the
 *  predicate and if so, what is the selectivity over the block.
 */
struct SMAPredicate {
  /**
   * @brief Describes how many tuples a predicate will select from a storage
   * block. 
   */
  enum class Selectivity {
    kAll,
    kSome,
    kNone,
    kUnknown,
    kUnsolved
  };

  /**
   * @brief Constructor. 
   * @param attribute Catalog attribute to be compared on the left.
   * @param comparisonid A Comparison Id.
   * @param literal A literal typed value for the right side of the comparison.
   */
  SMAPredicate(const CatalogAttribute& attribute,
               const ComparisonID comparisonid,
               const TypedValue literal) 
                   : attribute_(attribute),
                     comparisonid_(comparisonid),
                     literal_(literal),
                     selectivity_(kUnsolved) { };

  /**
   * @brief Extracts a comparison predicate into an SMAPredicate.
   * @param predicate A comparison of the form 
   *  {attribute} {comparison} {literal} or
   *  {literal} {comparison} {attribute}
   * @return An SMAPredicate which the caller must manage.
   */
  static SMAPredicate* Create(const ComparisonPredicate& predicate);

  const CatalogAttribute& attribute_;
  const ComparisonID comparisonid_;
  const TypedValue literal_;
  Selectivity selectivity_;
};

/**
 * @brief Refers to a specific entry (tuple's attribute value). Used for min, 
 *  max entries.
 */
class EntryReference {
 public:
  EntryReference(const tuple_id tuple = -1) : tid_(-1), value_() {  }

  ~EntryReference() { }

  inline const TypedValue& getValue() const {
    return value_;
  }

  inline tuple_id getTupleID() const {
    return tid_;
  }

  bool isSet() const {
    return tid_ != -1;
  }

  void set(tuple_id id, const TypedValue & value);

  void reset() {
    tid_ = -1;
  }

private:
  tuple_id tid_;
  TypedValue value_;
};

/**
 * @brief An SMAEntry will be created for each attribute which is indexed under
 *  the SMAIndexSubBlock.
 */
class SMAEntry {
 public:
  /**
   * @brief Constructor.
   * @param attribute_id The ID of the attribute which is being indexed.
   * @param attribute_type The type of the attibute being indexed.
   */
  SMAEntry(const attribute_id attribute_id, const Type& attribute_type) 
              : attr_id_(attribute_id),
                min_(),
                max_(),
                sum_(AggregationStateSum::SumZero(attribute_type)),
                requires_rebuild_(true) { }

  ~SMAEntry() { }

  /**
   * @brief Used in place of the constructor where raw memory is cast into an 
   *  SMAEntry.
   * @param new_block if this is a new SMA index or one being deserialized
   * @param attr_id ID of attribute which this entry summarizes
   * @param storage_block where the corresponding tuples are stored
   */
  void initialize(bool new_block, 
                  const attribute_id attr_id,
                  const TupleStorageSubBlock& tuple_store);

  /**
   * @brief Resets the entry to an initialized state. 
   * @details This is called prior to a rebuild.
   */
  void reset();

  attribute_id getAttrID() const {
    return attr_id_;
  }

  const EntryReference& getMinEntryReference() const {
    return min_;
  }

  const EntryReference& getMaxEntryReference() const {
    return max_;
  }

  const std::uint64_t getCount() const {
    return count_;
  }

  const TypedValue& getSum() const {
    return sum_;
  }

  /**
   * @brief Updates the aggregates based on the values of the new tuple.
   * @param tid The id of the new tuple.
   * @param tuple_store A reference to the storage block containing the tuple.
   */
  void insertUpdate(const tuple_id tid, const TupleStorageSubBlock& tuple_store);

  /**
   * @brief Attempts to update the SMAEntry on a removal. Some types of removals
   *  will require a rebuild (scan of storage_block).
   * @param tid The ID of the tuple which was removed.
   * @param tuple_store The tuple store containing the removed tuple.
   */
  void removeUpdate(const tuple_id tid, const TupleStorageSubBlock& tuple_store);

  /**
   * @brief Given a predicate, uses the min and max entries to determine if all,
   *  none, or some of the tuples in the storage block will be selected. This is
   *  used as a helper method by the SMAIndexSubBlock. 
   * @param predicate An SMAPredicate to solve.
   */
  void solvePredicate(SMAPredicate& predicate) const;

  bool getRequiresRebuild() const {
    return requires_rebuild_;
  }

  void setRequiresRebuild(bool rebuild) {
    requires_rebuild_ = rebuild;
  }

 private:
  void resetSum();

  static const Comparison& getLess();

  static const Comparison& getEqual();

  attribute_id attr_id_;
  TypeID attr_typeID_;
  EntryReference min_;
  EntryReference max_;
  TypedValue sum_;  // This is possibly higher precision than attribute type
                    // (ex. float->double)
  std::uint64_t count_;
  // fast operators for updating sum
  std::unique_ptr<UncheckedBinaryOperator> sub_operator_;
  std::unique_ptr<UncheckedBinaryOperator> add_operator_;
  // fast comparators
  std::unique_ptr<UncheckedComparator> less_comparison_;
  std::unique_ptr<UncheckedComparator> equal_comparison_;
  bool requires_rebuild_;
};

}  // namespace smaindex_internal


/**
 * Small Materialized Aggregate SubBlock
 *
 * Keeps account of several types of aggregate functions per Block. Currently
 * supports min, max.
 */
class SMAIndexSubBlock : public IndexSubBlock {
public:
  SMAIndexSubBlock( const TupleStorageSubBlock &tuple_store,
                    const IndexSubBlockDescription &description,
                    const bool new_block,
                    void *sub_block_memory,
                    const std::size_t sub_block_memory_size);

  ~SMAIndexSubBlock() { }

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
  static bool DescriptionIsValid( const CatalogRelationSchema &relation,
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
  static std::size_t EstimateBytesPerTuple( const CatalogRelationSchema &relation,
                                            const IndexSubBlockDescription &description);

  IndexSubBlockType getIndexSubBlockType() const override {
    return kSMA;
  }

  bool supportsAdHocAdd() const override{
    return initialized_;
  }

  /**
   * @brief Cannot remove unless we rebuild the entire index.
   *
   * @return false, does not support.
   */
  bool supportsAdHocRemove() const override {
    return false;
  }

  /**
   * @param tuple The new tuple added.
   * @return true always. There's no reason we should ever run out of space once
   *         this index has been successfully created
   */
  bool addEntry(const tuple_id tuple) override;

  void removeEntry(const tuple_id tuple) override;

  bool bulkAddEntries(const TupleIdSequence &tuples) override;

  void bulkRemoveEntries(const TupleIdSequence &tuples) override;

  /**
   * The SMA index will detect if one of the following cases is true:
   * 1) Complete match: all the tuples in this subblock will match the predicate
   * 2) Empty match: none of the tuples will match
   * 3) Partial match: some of the tuples may match
   * If there is a partial match, the SMA index is of no use. However, in a
   * Complete or Empty match, the SMA index can speed up the selection process
   * and should be used.
   *
   * @param predicate - Simple predicate too be evaluated.
   *
   * @return Cost associated with the type of match. Empty matches are constant,
   * partial matches require a scan, and complete matches require something less
   * than a regular scan (because we don't need to do the comparison operation
   * for each tuple) but is still linear with the number of tuples.
   */
  predicate_cost_t estimatePredicateEvaluationCost(
                                  const ComparisonPredicate &predicate) const override;

  /**
   * Calling this method on the SMA index implies that we are not going to do a
   * scan for some tuple matches. As in, the SMA index will either return an
   * empty set of tuple ids, or a set of tuple ids which is the entire set of
   * all tuple ids in the storage subblock.
   * @note Currently this version only supports simple comparisons of a literal
   *       value with a non-composite key.
   **/
  TupleIdSequence* getMatchesForPredicate(const ComparisonPredicate &predicate,
                                          const TupleIdSequence *filter) const override;


  bool rebuild() override;

  /**
   * @brief Returns if the index is consistent. Rebuilding will ensure this returns true
   * @return true if consistent
   */
  bool requiresRebuild() const;

  /**
   * Given an attribute, quickly check to see if the SMA index contains an
   * entry for it.
   * @param attribute - The ID of the attribute to check.
   * @return true if this index contains an entry.
   */
  bool hasEntryForAttribute(attribute_id attribute) const;

  /**
   * @brief Returns the corresponding SMAEntry from a given attribute ID
   * @param a_id AttributeID of the indexed attribute
   * @return a pointer to the SMAEntry stored in this index's block memory.
   *         \c nullptr if the attribute is not indexed.
   */
  const smaindex_internal::SMAEntry* getEntry(attribute_id attribute) const;

  /**
   * @brief Returns the number of tuples, the aggregate COUNT, of the storage SubBlock.
   * 
   * @return Number of tuples in the SubBlock
   */
  unsigned getCount() const;

private:

  void resetAggregates();

  /**
   * @brief Determines how much of the tuple store a predicate will select.
   * 
   * @param predicate The predicate to evaluate.
   * 
   * @return a solved SMA predicate which the caller must manage
   */
  smaindex_internal::SMAPredicate* solvePredicate(const ComparisonPredicate& predicate) const;


  int num_entries_;
  std::vector<attribute_id> attr_ids_;
  std::map<attribute_id, unsigned> attr_to_index_;
  bool initialized_;

  std::unique_ptr<smaindex_internal::SMAPredicate> last_predicate_;

  friend class SMAIndexSubBlockTest;

  DISALLOW_COPY_AND_ASSIGN(SMAIndexSubBlock);

};
} /* namespace quickstep */

#endif /* QUICKSTEP_STORAGE_SMA_INDEX_SUB_BLOCK_HPP_ */
