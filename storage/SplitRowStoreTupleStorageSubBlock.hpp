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

#ifndef QUICKSTEP_STORAGE_SPLIT_ROW_STORE_TUPLE_STORAGE_SUB_BLOCK_HPP_
#define QUICKSTEP_STORAGE_SPLIT_ROW_STORE_TUPLE_STORAGE_SUB_BLOCK_HPP_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include "expressions/predicate/PredicateCost.hpp"
#include "storage/SubBlockTypeRegistryMacros.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "types/TypedValue.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"

#include "gtest/gtest_prod.h"

namespace quickstep {

class ComparisonPredicate;
class Tuple;
class TupleStorageSubBlockDescription;
class ValueAccessor;

QUICKSTEP_DECLARE_SUB_BLOCK_TYPE_REGISTERED(SplitRowStoreTupleStorageSubBlock);

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief An implementation of TupleStorageSubBlock as a slotted row-store that
 *        splits storage between an array of slots for fixed-length attributes
 *        at the front of the block and a region of storage for variable-length
 *        attributes at the back of the block that both grow towards each
 *        other.
 * @note Deletions and updates can cause both the slot array and the
 *       variable-length storage region to become fragmented. Otherwise wasted
 *       storage can be reclaimed by calling rebuild().
 **/
class SplitRowStoreTupleStorageSubBlock: public TupleStorageSubBlock {
 public:
  SplitRowStoreTupleStorageSubBlock(const CatalogRelationSchema &relation,
                                    const TupleStorageSubBlockDescription &description,
                                    const bool new_block,
                                    void *sub_block_memory,
                                    const std::size_t sub_block_memory_size);

  ~SplitRowStoreTupleStorageSubBlock() override {
  }

  /**
   * @brief Determine whether a TupleStorageSubBlockDescription is valid for
   *        this type of TupleStorageSubBlock.
   *
   * @param relation The relation a tuple store described by description would
   *        belong to.
   * @param description A description of the parameters for this type of
   *        TupleStorageSubBlock, which will be checked for validity.
   * @return Whether description is well-formed and valid for this type of
   *         TupleStorageSubBlock belonging to relation (i.e. whether a
   *         TupleStorageSubBlock of this type, belonging to relation, can be
   *         constructed according to description).
   **/
  static bool DescriptionIsValid(const CatalogRelationSchema &relation,
                                 const TupleStorageSubBlockDescription &description);

  /**
   * @brief Estimate the average number of bytes (including any applicable
   *        overhead) used to store a single tuple in this type of
   *        TupleStorageSubBlock. Used by StorageBlockLayout::finalize() to
   *        divide block memory amongst sub-blocks.
   * @warning description must be valid. DescriptionIsValid() should be called
   *          first if necessary.
   *
   * @param relation The relation tuples belong to.
   * @param description A description of the parameters for this type of
   *        TupleStorageSubBlock.
   * @return The average/ammortized number of bytes used to store a single
   *         tuple of relation in a TupleStorageSubBlock of this type described
   *         by description.
   **/
  static std::size_t EstimateBytesPerTuple(const CatalogRelationSchema &relation,
                                           const TupleStorageSubBlockDescription &description);

  bool supportsUntypedGetAttributeValue(const attribute_id attr) const override {
    return true;
  }

  bool supportsAdHocInsert() const override {
    return true;
  }

  bool adHocInsertIsEfficient() const override {
    return true;
  }

  TupleStorageSubBlockType getTupleStorageSubBlockType() const override {
    return kSplitRowStore;
  }

  bool isEmpty() const override {
    return header_->num_tuples == 0;
  }

  bool isPacked() const override {
    return header_->num_tuples == header_->max_tid + 1;
  }

  tuple_id getMaxTupleID() const override {
    return header_->max_tid;
  }

  tuple_id numTuples() const override {
    return header_->num_tuples;
  }

  bool hasTupleWithID(const tuple_id tuple) const override {
    return (tuple >= 0)
           && (static_cast<std::size_t>(tuple) < occupancy_bitmap_->size())
           && occupancy_bitmap_->getBit(tuple);
  }

  InsertResult insertTuple(const Tuple &tuple) override;

  inline bool insertTupleInBatch(const Tuple &tuple) override {
    const InsertResult result = insertTuple(tuple);
    return (result.inserted_id >= 0);
  }

  tuple_id bulkInsertTuples(ValueAccessor *accessor) override;

  tuple_id bulkInsertTuplesWithRemappedAttributes(
      const std::vector<attribute_id> &attribute_map,
      ValueAccessor *accessor) override;

  const void* getAttributeValue(const tuple_id tuple,
                                const attribute_id attr) const override;

  TypedValue getAttributeValueTyped(const tuple_id tuple,
                                    const attribute_id attr) const override;

  ValueAccessor* createValueAccessor(
      const TupleIdSequence *sequence = nullptr) const override;

  bool canSetAttributeValuesInPlaceTyped(
      const tuple_id tuple,
      const std::unordered_map<attribute_id, TypedValue> &new_values) const override;

  void setAttributeValueInPlaceTyped(const tuple_id tuple,
                                     const attribute_id attr,
                                     const TypedValue &value) override;

  bool deleteTuple(const tuple_id tuple) override;

  bool bulkDeleteTuples(TupleIdSequence *tuples) override;

  TupleIdSequence* getExistenceMap() const override {
    return new TupleIdSequence(header_->max_tid + 1, *occupancy_bitmap_);
  }

  OrderedTupleIdSequence* getExistenceList() const override;

  predicate_cost_t estimatePredicateEvaluationCost(
      const ComparisonPredicate &predicate) const override {
    return predicate_cost::kRowScan;
  }

  void rebuild() override;

  bool isInsertOrderPreserving() const override {
    return true;
  }

 private:
  struct Header {
    tuple_id num_tuples;
    tuple_id max_tid;
    std::uint32_t variable_length_bytes_allocated;
    bool variable_length_storage_compact;
  };

  inline bool spaceToInsert(const tuple_id position,
                            const std::size_t variable_length_bytes) const {
    return (header_->max_tid + 1) * tuple_slot_bytes_                // Already allocated slots
           + (position == header_->max_tid + 1) * tuple_slot_bytes_  // An extra slot if needed
           + header_->variable_length_bytes_allocated                // Already allocated variable storage.
           + variable_length_bytes                                   // Extra needed variable storage.
         <= tuple_storage_bytes_;
  }

  template <bool nullable_attrs, bool variable_length_attrs>
  InsertResult insertTupleImpl(const Tuple &tuple);

  Header *header_;

  std::unique_ptr<BitVector<false>> occupancy_bitmap_;
  std::size_t occupancy_bitmap_bytes_;

  void *tuple_storage_;
  std::size_t tuple_storage_bytes_;
  std::size_t tuple_slot_bytes_;

  std::size_t per_tuple_null_bitmap_bytes_;

  friend class SplitRowStoreTupleStorageSubBlockTest;
  friend class SplitRowStoreValueAccessor;
  FRIEND_TEST(SplitRowStoreTupleStorageSubBlockTest, InitializeTest);

  DISALLOW_COPY_AND_ASSIGN(SplitRowStoreTupleStorageSubBlock);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_PACKED_ROW_STORE_TUPLE_STORAGE_SUB_BLOCK_HPP_
