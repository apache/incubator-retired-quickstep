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

namespace splitrow_internal {
// A CopyGroup contains information about ane run of attributes in the source
// ValueAccessor that can be copied into the output block. The
// getCopyGroupsForAttributeMap function below takes an attribute map for a source
// and converts it into a sequence of runs. The goal is to minimize the number
// of memcpy calls and address calculations that occur during bulk insertion.
// Contiguous attributes from a rowstore source can be merged into a single copy group.
//
// A single ContiguousAttrs CopyGroup consists of contiguous attributes, nullable
// or not. "Contiguous" here means that their attribute IDs are successive in both
// the source and destination relations.
//
// A NullAttr refers to exactly one nullable attribute. Nullable columns are
// represented using fixed length inline data as well as a null bitmap.
// In a particular tuple, if the attribute has a null value, the inline data
// has no meaning. So it is safe to copy it or not. We use this fact to merge
// runs together aggressively, i.e., a ContiguousAttrs group may include a
// nullable attribute. However, we also create a NullableAttr in that case in
// order to check the null bitmap.
//
// A gap is a run of destination (output) attributes that don't come from a
// particular source. This occurs during bulkInsertPartialTuples. They must be
// skipped during the insert (not copied over). They are indicated by a
// kInvalidCatalogId in the attribute map. For efficiency, the gap size
// is merged into the bytes_to_advance_ of previous ContiguousAttrs copy group.
// For gaps at the start of the attribute map, we just create a ContiguousAttrs
// copy group with 0 bytes to copy and dummy (0) source attribute id.
//
// eg. For 4B integer attrs, from a row store source,
// if the input attribute_map is {-1,0,5,6,7,-1,2,4,9,10,-1}
// with input/output attributes 4 and 7 being nullable,
// we will create the following ContiguousAttrs copy groups
//
//  ----------------------------------------------------
//  |src_id_      |bytes_to_advance_| bytes_to_copy_   |
//  |-------------|-----------------|------------------|
//  |            0|                4|                 4|
//  |            5|                4|                12|
//  |            2|               16|                 4|
//  |            4|                4|                 4|
//  |            9|                4|                 8|
//  ----------------------------------------------------
// and two NullableAttrs with src_attr_id_ set to 4 and 7.
//
// In this example, we do 6 memcpy calls and 6 address calculations
// as well as 2 bitvector lookups for each tuple. A naive copy algorithm
// would do 11 memcpy calls and address calculations, along with the
// bitvector lookups, not to mention the schema lookups,
// all interspersed in a complex loop with lots of branches.
//
// If the source was a column store, then we can't merge contiguous
// attributes (or gaps). So we would have 11 ContigousAttrs copy groups with
// three of them having bytes_to_copy = 0 (corresponding to the gaps) and
// the rest having bytes_to_copy_ = 4.
//
// SplitRowStore supports variable length attributes. Since the layout of the
// tuple is like: [null bitmap][fixed length attributes][variable length offsets]
// we do all the variable length copies after the fixed length copies.
//
struct CopyGroup {
  attribute_id src_attr_id_;  // The attr_id of starting input attribute for run.

  explicit CopyGroup(const attribute_id source_attr_id)
    : src_attr_id_(source_attr_id) {}
};

struct ContiguousAttrs : public CopyGroup {
  std::size_t bytes_to_advance_;  // Number of bytes to advance destination ptr
                                  // to get to the location where we copy THIS attribute.
  std::size_t bytes_to_copy_;     // Number of bytes to copy from source.

  ContiguousAttrs(
    const attribute_id source_attr_id,
    const std::size_t bytes_to_copy,
    const std::size_t bytes_to_advance)
    : CopyGroup(source_attr_id),
      bytes_to_advance_(bytes_to_advance),
      bytes_to_copy_(bytes_to_copy) { }
};

struct VarLenAttr : public CopyGroup {
  std::size_t bytes_to_advance_;
  attribute_id dst_attr_id_;
  VarLenAttr(const attribute_id source_attr_id,
             const attribute_id dst_attr_id,
             const std::size_t bytes_to_advance)
    : CopyGroup(source_attr_id),
      bytes_to_advance_(bytes_to_advance),
      dst_attr_id_(dst_attr_id) {}
};

struct NullableAttr : public CopyGroup {
  int nullable_attr_idx_;  // index into null bitmap

  NullableAttr(attribute_id source_attr_id_,
               int nullable_attr_idx)
    : CopyGroup(source_attr_id_),
      nullable_attr_idx_(nullable_attr_idx) {}
};

struct CopyGroupList {
  CopyGroupList()
    : contiguous_attrs_(),
      nullable_attrs_(),
      varlen_attrs_() {}

  /**
   * @brief Attributes which are exactly sequential are merged to a single copy.
   */
  void merge_contiguous() {
    if (contiguous_attrs_.size() < 2) {
      return;
    }

    int add_to_advance = 0;
    for (std::size_t idx = 1; idx < contiguous_attrs_.size(); ++idx) {
      ContiguousAttrs *current_attr = &contiguous_attrs_[idx];
      ContiguousAttrs *previous_attr = &contiguous_attrs_[idx - 1];
      if (add_to_advance > 0) {
        current_attr->bytes_to_advance_ += add_to_advance;
        add_to_advance = 0;
      }
      // The merge step:
      if (previous_attr->src_attr_id_ + 1 == current_attr->src_attr_id_ &&
            previous_attr->bytes_to_copy_ == current_attr->bytes_to_advance_) {
        previous_attr->bytes_to_copy_ += current_attr->bytes_to_copy_;
        add_to_advance += current_attr->bytes_to_advance_;
        contiguous_attrs_.erase(contiguous_attrs_.begin() + idx);
        idx--;
      }
    }

    if (varlen_attrs_.size() > 0) {
      varlen_attrs_[0].bytes_to_advance_ += add_to_advance;
    }
  }

  std::vector<ContiguousAttrs> contiguous_attrs_;
  std::vector<NullableAttr> nullable_attrs_;
  std::vector<VarLenAttr> varlen_attrs_;
};

}  // namespace splitrow_internal

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
  static const std::size_t kVarLenSlotSize;

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

  tuple_id bulkInsertPartialTuples(
    const std::vector<attribute_id> &attribute_map,
    ValueAccessor *accessor,
    const tuple_id max_num_tuples_to_insert);

  void bulkInsertPartialTuplesFinalize(const tuple_id num_tuples_inserted);

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

  template<bool copy_nulls, bool copy_varlen, bool fill_to_capacity>
  tuple_id bulkInsertPartialTuplesImpl(
    const splitrow_internal::CopyGroupList &copy_groups,
    ValueAccessor *accessor,
    std::size_t max_num_tuples_to_insert);

  tuple_id bulkInsertDispatcher(
    const std::vector<attribute_id> &attribute_map,
    ValueAccessor *accessor,
    tuple_id max_num_tuples_to_insert,
    bool finalize);

  void getCopyGroupsForAttributeMap(
    const std::vector<attribute_id> &attribute_map,
    splitrow_internal::CopyGroupList *copy_groups);

  std::size_t getInsertLowerBound() const;

  // When varlen attributes are bulk inserted, the difference between the maximum
  // possible size and the actual size of the tuples will cause an underestimate of
  // the number of tuples we can insert. This threshold puts a limit on the number
  // of tuples to attempt to insert. A smaller number will give more rounds of insertion
  // and a more-packed block, but at the cost of insertion speed.
  std::size_t getInsertLowerBoundThreshold() const {
    return 10;
  }

  Header *header_;

  std::unique_ptr<BitVector<false>> occupancy_bitmap_;
  std::size_t occupancy_bitmap_bytes_;

  void *tuple_storage_;
  std::size_t tuple_storage_bytes_;
  std::size_t tuple_slot_bytes_;
  std::vector<std::size_t> fixed_len_attr_sizes_;

  std::size_t num_null_attrs_;
  std::size_t num_fixed_attrs_;
  std::size_t num_var_attrs_;

  std::size_t per_tuple_null_bitmap_bytes_;

  friend class SplitRowStoreTupleStorageSubBlockTest;
  friend class SplitRowStoreValueAccessor;
  FRIEND_TEST(SplitRowStoreTupleStorageSubBlockTest, InitializeTest);
  FRIEND_TEST(SplitRowStoreTupleStorageSubBlockTest, GetCopyGroupsForAttributeMapTest);

  DISALLOW_COPY_AND_ASSIGN(SplitRowStoreTupleStorageSubBlock);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_PACKED_ROW_STORE_TUPLE_STORAGE_SUB_BLOCK_HPP_
