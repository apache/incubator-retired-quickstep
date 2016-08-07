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

#ifndef QUICKSTEP_STORAGE_COMPRESSED_COLUMN_STORE_TUPLE_STORAGE_SUB_BLOCK_HPP_
#define QUICKSTEP_STORAGE_COMPRESSED_COLUMN_STORE_TUPLE_STORAGE_SUB_BLOCK_HPP_

#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/predicate/PredicateCost.hpp"
#include "storage/CompressedTupleStorageSubBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/SubBlockTypeRegistryMacros.hpp"
#include "types/TypedValue.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"

namespace quickstep {

class CatalogRelationSchema;
class ComparisonPredicate;
class ValueAccessor;

QUICKSTEP_DECLARE_SUB_BLOCK_TYPE_REGISTERED(CompressedColumnStoreTupleStorageSubBlock);

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief An implementation of TupleStorageSubBlock as a column store with a
 *        single sort column, optional column compression (dictionary or
 *        truncation), and no holes.
 * @warning This implementation does support variable-length attributes, but
 *          they must all be compressed (specified with compressed_attribute_id
 *          in the TupleStorageSubBlockDescription).
 **/
class CompressedColumnStoreTupleStorageSubBlock : public CompressedTupleStorageSubBlock {
 public:
  CompressedColumnStoreTupleStorageSubBlock(
      const CatalogRelationSchema &relation,
      const TupleStorageSubBlockDescription &description,
      const bool new_block,
      void *sub_block_memory,
      const std::size_t sub_block_memory_size);

  ~CompressedColumnStoreTupleStorageSubBlock() override {
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

  inline TypedValue getAttributeValueTypedInl(const tuple_id tuple,
                                              const attribute_id attr) const;

  TupleStorageSubBlockType getTupleStorageSubBlockType() const override {
    return kCompressedColumnStore;
  }

  const void* getAttributeValue(const tuple_id tuple,
                                const attribute_id attr) const override;

  TypedValue getAttributeValueTyped(const tuple_id tuple,
                                    const attribute_id attr) const override;

  ValueAccessor* createValueAccessor(
      const TupleIdSequence *sequence = nullptr) const override;

  bool deleteTuple(const tuple_id tuple) override;
  bool bulkDeleteTuples(TupleIdSequence *tuples) override;

  predicate_cost_t estimatePredicateEvaluationCost(
      const ComparisonPredicate &predicate) const override;

  // This override can quickly evaluate comparisons between the sort column
  // and a literal value.
  TupleIdSequence* getMatchesForPredicate(const ComparisonPredicate &predicate,
                                          const TupleIdSequence *filter) const override;

  void rebuild() override;

  bool isInsertOrderPreserving() const override {
    // Rebuild sorts tuples based on sort column.
    return false;
  }

  std::uint32_t compressedGetCode(const tuple_id tid,
                                  const attribute_id attr_id) const override;

 protected:
  TupleIdSequence* getEqualCodes(const attribute_id attr_id,
                                 const std::uint32_t code,
                                 const TupleIdSequence *filter) const override;

  TupleIdSequence* getNotEqualCodes(const attribute_id attr_id,
                                    const std::uint32_t code,
                                    const TupleIdSequence *filter) const override;

  TupleIdSequence* getNotEqualCodesExcludingNull(const attribute_id attr_id,
                                                 const std::uint32_t code,
                                                 const std::uint32_t null_code,
                                                 const TupleIdSequence *filter) const override;

  TupleIdSequence* getLessCodes(const attribute_id attr_id,
                                const std::uint32_t code,
                                const TupleIdSequence *filter) const override;

  TupleIdSequence* getGreaterOrEqualCodes(const attribute_id attr_id,
                                          const std::uint32_t code,
                                          const TupleIdSequence *filter) const override;

  TupleIdSequence* getCodesInRange(const attribute_id attr_id,
                                   const std::pair<std::uint32_t, std::uint32_t> range,
                                   const TupleIdSequence *filter) const override;

 private:
  // Returns a pointer to an attribute's physical storage (the value itself if
  // uncompressed, OR the compressed code if compressed), or NULL if
  // 'check_null' is true and the attribute is null.
  template <bool check_null = true>
  inline const void* getAttributePtr(const tuple_id tid,
                                     const attribute_id attr_id) const {
    if (check_null
        && compression_info_.uncompressed_attribute_has_nulls(attr_id)
        && uncompressed_column_null_bitmaps_[attr_id].getBit(tid)) {
      return nullptr;
    } else {
      return static_cast<const char*>(column_stripes_[attr_id])
             + tid * compression_info_.attribute_size(attr_id);
    }
  }

  // Initialize this sub-block's runtime data structures after the physical
  // block has been built.
  void initialize();

  // Move 'num_tuples' values in each column from 'src_tuple' to
  // 'dest_position'.
  void shiftTuples(const tuple_id dest_position,
                   const tuple_id src_tuple,
                   const tuple_id num_tuples);

  // Shift the tails of 'uncompressed_column_null_bitmaps_' (starting from
  // 'from_position') by 'distance' bits (forward shift if 'distance' is
  // negative, backward shift if positive).
  void shiftUncompressedNullBitmaps(const tuple_id from_position,
                                    const tuple_id distance);

  std::pair<tuple_id, tuple_id> getCompressedSortColumnRange(
      const std::pair<std::uint32_t, std::uint32_t> code_range) const;

  // Note: order of application is
  // comparison_functor(literal_code, attribute_code).
  template <template <typename T> class comparison_functor>
  TupleIdSequence* getCodesSatisfyingComparison(const attribute_id attr_id,
                                                const std::uint32_t code,
                                                const TupleIdSequence *filter) const;

  attribute_id sort_column_id_;
  tuple_id uncompressed_nulls_in_sort_column_;

  std::vector<void*> column_stripes_;
  PtrVector<BitVector<false>, true> uncompressed_column_null_bitmaps_;

  DISALLOW_COPY_AND_ASSIGN(CompressedColumnStoreTupleStorageSubBlock);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_COMPRESSED_COLUMN_STORE_TUPLE_STORAGE_SUB_BLOCK_HPP_
