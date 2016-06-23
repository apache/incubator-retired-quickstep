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

#ifndef QUICKSTEP_STORAGE_PACKED_ROW_STORE_TUPLE_STORAGE_SUB_BLOCK_HPP_
#define QUICKSTEP_STORAGE_PACKED_ROW_STORE_TUPLE_STORAGE_SUB_BLOCK_HPP_

#include <memory>
#include <unordered_map>
#include <vector>

#include "expressions/predicate/PredicateCost.hpp"
#include "storage/SubBlockTypeRegistryMacros.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "types/TypedValue.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogRelationSchema;
class ComparisonPredicate;
class TupleStorageSubBlockDescription;
class ValueAccessor;

QUICKSTEP_DECLARE_SUB_BLOCK_TYPE_REGISTERED(PackedRowStoreTupleStorageSubBlock);

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief An implementation of TupleStorageSubBlock as a packed row-store (i.e.
 *        an array of fixed-length values with no holes).
 * @warning This implementation does NOT support variable-length attributes. It
 *          is an error to attempt to construct a
 *          PackedRowStoreTupleStorageSubBlock for a relation with any
 *          variable-length attributes.
 **/
class PackedRowStoreTupleStorageSubBlock: public TupleStorageSubBlock {
 public:
  PackedRowStoreTupleStorageSubBlock(const CatalogRelationSchema &relation,
                                     const TupleStorageSubBlockDescription &description,
                                     const bool new_block,
                                     void *sub_block_memory,
                                     const std::size_t sub_block_memory_size);

  ~PackedRowStoreTupleStorageSubBlock() override {
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
    return kPackedRowStore;
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

  InsertResult insertTuple(const Tuple &tuple) override {
    if (null_bitmap_.get() == nullptr) {
      return insertTupleImpl<false>(tuple);
    } else {
      return insertTupleImpl<true>(tuple);
    }
  }

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
      const std::unordered_map<attribute_id, TypedValue> &new_values) const override {
    return true;
  }

  void setAttributeValueInPlaceTyped(const tuple_id tuple,
                                     const attribute_id attr,
                                     const TypedValue &value) override;

  bool deleteTuple(const tuple_id tuple) override;
  bool bulkDeleteTuples(TupleIdSequence *tuples) override;

  predicate_cost_t estimatePredicateEvaluationCost(
      const ComparisonPredicate &predicate) const override {
    return predicate_cost::kRowScan;
  }

  void rebuild() override {
  }

  bool isInsertOrderPreserving() const override {
    return true;
  }

 private:
  struct PackedRowStoreHeader {
    tuple_id num_tuples;
  };

  // If 'nullable_attrs' is true, extra branches are enabled to deal with NULLs
  // and set bits in '*null_bitmap_' appropriately. If no attributes of the
  // relation are nullable, then 'nullable_attrs' is false and this function
  // has a simpler and faster implementation with no NULL-checking.
  template <bool nullable_attrs>
  InsertResult insertTupleImpl(const Tuple &tuple);

  // Similar to insertTupleImpl(), the 'nullable_attrs' template parameter
  // distinguishes between a version of this function that includes a check for
  // space in '*null_bitmap_' and a simpler version that skips the check for
  // relations that have no nullable attributes (and therefore no NULL-bitmap).
  template <bool nullable_attrs>
  bool hasSpaceToInsert(const tuple_id num_tuples) const;
  
  // Helper function for bulkInsertTuplesWithRemappedAttributes which is
  // templated on the 'nullable_attrs' flag (similar in semantics to its use in
  // the above functions). 
  template <bool nullable_attrs>
  tuple_id bulkInsertTuplesWithRemappedAttributesHelper(
      const std::vector<attribute_id> &attribute_map, ValueAccessor *accessor);

  PackedRowStoreHeader *header_;
  std::unique_ptr<BitVector<false>> null_bitmap_;
  std::size_t null_bitmap_bytes_;
  void *tuple_storage_;

  DISALLOW_COPY_AND_ASSIGN(PackedRowStoreTupleStorageSubBlock);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_PACKED_ROW_STORE_TUPLE_STORAGE_SUB_BLOCK_HPP_
