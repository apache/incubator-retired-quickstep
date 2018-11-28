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

#ifndef QUICKSTEP_STORAGE_BASIC_COLUMN_STORE_TUPLE_STORAGE_SUB_BLOCK_HPP_
#define QUICKSTEP_STORAGE_BASIC_COLUMN_STORE_TUPLE_STORAGE_SUB_BLOCK_HPP_

#include <unordered_map>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/predicate/PredicateCost.hpp"
#include "storage/SubBlockTypeRegistryMacros.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "types/TypedValue.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"

namespace quickstep {

template <typename K, typename V> class PtrMap;

class CatalogRelationSchema;
class ComparisonPredicate;
class TupleStorageSubBlockDescription;
class Type;
class ValueAccessor;

QUICKSTEP_DECLARE_SUB_BLOCK_TYPE_REGISTERED(BasicColumnStoreTupleStorageSubBlock);

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief An implementation of TupleStorageSubBlock as a simple column store
 *        with a single sort column and no compression or holes.
 * @warning This implementation does NOT support variable-length attributes.
 *          It is an error to attempt to construct a
 *          BasicColumnStoreTupleStorageSubBlock for a relation with any
 *          variable-length attributes.
 **/
class BasicColumnStoreTupleStorageSubBlock : public TupleStorageSubBlock {
 public:
  BasicColumnStoreTupleStorageSubBlock(const CatalogRelationSchema &relation,
                                       const TupleStorageSubBlockDescription &description,
                                       const bool new_block,
                                       void *sub_block_memory,
                                       const std::size_t sub_block_memory_size);

  ~BasicColumnStoreTupleStorageSubBlock() override {
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
    // Ad-hoc insert is only efficient if it doesn't require resorting the
    // whole block.
    return !sort_specified_;
  }

  TupleStorageSubBlockType getTupleStorageSubBlockType() const override {
    return kBasicColumnStore;
  }

  bool isEmpty() const override {
    return (header_->num_tuples == 0);
  }

  bool isPacked() const override {
    return true;
  }

  tuple_id getMaxTupleID() const override {
    return header_->num_tuples - 1;
  }

  bool hasTupleWithID(const tuple_id tuple) const override {
    return ((tuple >=0) && (tuple < header_->num_tuples));
  }

  InsertResult insertTuple(const Tuple &tuple) override;

  bool insertTupleInBatch(const Tuple &tuple) override;

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

  predicate_cost_t estimatePredicateEvaluationCost(
      const ComparisonPredicate &predicate) const override;

  // This override can quickly evaluate comparisons between the sort column
  // and a literal value.
  TupleIdSequence* getMatchesForPredicate(const ComparisonPredicate &predicate,
                                          const TupleIdSequence *filter) const override;

  void rebuild() override {
    if (sort_specified_ && !sorted_) {
      rebuildInternal();
    }
  }

  bool isInsertOrderPreserving() const override {
    // Rebuild reorders inserts based on sort column if any is specified.
    return !sort_specified_;
  }

 private:
  struct BasicColumnStoreHeader {
    tuple_id num_tuples;
    tuple_id nulls_in_sort_column;
  };

  bool hasSpaceToInsert(const tuple_id num_tuples) const {
    return (num_tuples <= max_tuples_ - header_->num_tuples);
  }

  // Copy attribute values from 'tuple' into the appropriate column stripes
  // at the offset specified by 'position'. If 'position' is not at the current
  // end of tuples in this block, subsequent tuples are shifted back to make
  // room for the new tuple.
  void insertTupleAtPosition(const Tuple &tuple,
                             const tuple_id position);

  // Move 'num_tuples' values in each column from 'src_tuple' to
  // 'dest_position'. This should usually be paired with a call to
  // shiftNullBitmaps().
  void shiftTuples(const tuple_id dest_position,
                   const tuple_id src_tuple,
                   const tuple_id num_tuples);

  // Shift the tails of 'column_null_bitmaps_' (starting from 'from_position')
  // by 'distance' bits (forward shift if 'distance' is negative, backward
  // shift if positive).
  void shiftNullBitmaps(const tuple_id from_position,
                        const tuple_id distance);

  // Sort all columns according to ascending order of values in the sort
  // column. Returns true if any reordering occured. This should only be called
  // when 'sort_specified_' is true, otherwise the block is always "built".
  bool rebuildInternal();

  tuple_id max_tuples_;
  bool sort_specified_, sorted_;

  attribute_id sort_column_id_;
  const Type *sort_column_type_;

  BasicColumnStoreHeader *header_;
  std::vector<void*> column_stripes_;
  PtrVector<BitVector<false>, true> column_null_bitmaps_;

  DISALLOW_COPY_AND_ASSIGN(BasicColumnStoreTupleStorageSubBlock);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_BASIC_COLUMN_STORE_TUPLE_STORAGE_SUB_BLOCK_HPP_
