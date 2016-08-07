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

#ifndef QUICKSTEP_STORAGE_COMPRESSED_PACKED_ROW_STORE_TUPLE_STORAGE_SUB_BLOCK_HPP_
#define QUICKSTEP_STORAGE_COMPRESSED_PACKED_ROW_STORE_TUPLE_STORAGE_SUB_BLOCK_HPP_

#include <cstddef>
#include <cstdint>
#include <memory>
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

namespace quickstep {

class CatalogRelationSchema;
class ComparisonPredicate;
class TupleStorageSubBlockDescription;
class ValueAccessor;

QUICKSTEP_DECLARE_SUB_BLOCK_TYPE_REGISTERED(CompressedPackedRowStoreTupleStorageSubBlock);

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief An implementation of TupleStorageSubBlock as a packed row store with
 *        optional column compression (dictionary or truncation) and no holes.
 * @warning This implementation does support variable-length attributes, but
 *          they must all be compressed (specified with compressed_attribute_id
 *          in the TupleStorageSubBlockDescription).
 **/
class CompressedPackedRowStoreTupleStorageSubBlock : public CompressedTupleStorageSubBlock {
 public:
  CompressedPackedRowStoreTupleStorageSubBlock(
      const CatalogRelationSchema &relation,
      const TupleStorageSubBlockDescription &description,
      const bool new_block,
      void *sub_block_memory,
      const std::size_t sub_block_memory_size);

  ~CompressedPackedRowStoreTupleStorageSubBlock() override {
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

  TupleStorageSubBlockType getTupleStorageSubBlockType() const override {
    return kCompressedPackedRowStore;
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

  void rebuild() override;

  bool isInsertOrderPreserving() const override {
    return true;
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
        && null_bitmap_->getBit(tid * num_uncompressed_attributes_with_nulls_
                                + null_bitmap_attribute_offsets_[attr_id])) {
      return nullptr;
    } else {
      return static_cast<const char*>(tuple_storage_)
             + tid * tuple_length_bytes_
             + attribute_offsets_[attr_id];
    }
  }

  // Initialize this sub-block's runtime data structures after the physical
  // block has been built.
  void initialize();

  // Note: order of application is
  // comparison_functor(literal_code, attribute_code).
  template <template <typename T> class comparison_functor>
  TupleIdSequence* getCodesSatisfyingComparison(const attribute_id attr_id,
                                                const std::uint32_t code,
                                                const TupleIdSequence *filter) const;

  void *tuple_storage_;
  std::size_t tuple_length_bytes_;
  std::vector<std::size_t> attribute_offsets_;

  // 'null_bitmap_' is only used for uncompressed attributes that actually
  // contain nulls. CompressionDictionary handles nulls for compressed columns.
  std::unique_ptr<BitVector<false>> null_bitmap_;
  attribute_id num_uncompressed_attributes_with_nulls_;
  std::vector<attribute_id> null_bitmap_attribute_offsets_;

  DISALLOW_COPY_AND_ASSIGN(CompressedPackedRowStoreTupleStorageSubBlock);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_COMPRESSED_PACKED_ROW_STORE_TUPLE_STORAGE_SUB_BLOCK_HPP_
