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

#ifndef QUICKSTEP_STORAGE_COMPRESSED_BLOCK_BUILDER_HPP_
#define QUICKSTEP_STORAGE_COMPRESSED_BLOCK_BUILDER_HPP_

#include <cstddef>
#include <unordered_map>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "compression/CompressionDictionaryBuilder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "types/containers/Tuple.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrMap.hpp"
#include "utility/PtrVector.hpp"

namespace quickstep {

class CatalogRelationSchema;
class TypedValue;

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief A helper class which temporarily stores tuples during batch-insertion
 *        and builds the physical contents of either a
 *        CompressedPackedRowStoreTupleStorageSubBlock or a
 *        CompressedColumnStoreTupleStorageSubBlock, automatically selecting
 *        most efficient coding for each compressed column (dictionary coding,
 *        truncation, or none).
 **/
class CompressedBlockBuilder {
 public:
  /**
   * @brief Constructor.
   *
   * @param relation The relation which the compressed block being built
   *        belongs to.
   * @param description The description for the TupleStorageSubBlock being
   *        built. The description must be valid and specify either
   *        COMPRESSED_PACKED_ROW_STORE or COMPRESSED_COLUMN_STORE as the
   *        sub_block_type.
   * @param block_size The size in bytes of the TupleStorageSubBlock being
   *        built.
   **/
  CompressedBlockBuilder(const CatalogRelationSchema &relation,
                         const TupleStorageSubBlockDescription &description,
                         const std::size_t block_size);

  /**
   * @brief Destructor. A CompressedBlockBuilder should be deleted after the
   *        block is done being built to free any temporary memory.
   **/
  ~CompressedBlockBuilder() {
  }

  /**
   * @brief Check if an attribute may be compressed in the TupleStorageSubBlock
   *        ultimately build by this CompressedBlockBuilder.
   * @note Even if this method returns true, the attribute specified might
   *       still be uncompressed when the block is built if compression fails
   *       to save any space.
   *
   * @param attr_id The ID of the attribute to check for possible compression.
   * @return true if the attribute specified by attr_id might be compressed
   *         in a TupleStorageSubBlock built by this CompressedBlockBuilder,
   *         false if no compression will be attempted.
   **/
  bool attributeMayBeCompressed(const attribute_id attr_id) const {
    return dictionary_builders_.find(attr_id) != dictionary_builders_.end();
  }

  /**
   * @brief Add a Tuple to the block being built.
   *
   * @param tuple The Tuple to add. It will be copied.
   *
   * @return True if the tuple was successfully added, false if attempting to
   *         add the tuple failed because there would not be enough space to
   *         store it in the block being constructed. Typically,
   *         buildCompressedPackedRowStoreTupleStorageSubBlock() or
   *         buildCompressedColumnStoreTupleStorageSubBlock() would be called
   *         to actually construct the physical block after the first time this
   *         method returns false.
   **/
  bool addTuple(const Tuple &tuple);

  /**
   * @brief Similar to addTuple(), but takes ownership of an existing Tuple
   *        without copying it.
   * @note Even if this method returns false, tuple will still be deleted by
   *       this CompressedBlockBuilder.
   *
   * @param tuple The Tuple to add.
   * @return True if tuple was successfully added, false if out of space. See
   *         addTuple() for additional details.
   **/
  bool takeTuple(Tuple *tuple) {
    return addTupleInternal(tuple);
  }

  /**
   * @brief Get the number of Tuples held by this CompressedBlockBuilder for
   *        eventual inclusion in the compressed block being built.
   *
   * @return The number of tuples in this CompressedBlockBuilder.
   **/
  inline std::size_t numTuples() const {
    return tuples_.size();
  }

  /**
   * @brief Get the bare-minimum number of bytes needed to store just the
   *        metadata of a compressed block under construction, without any
   *        actual tuples.
   *
   * @return The minimum required memory for any TupleStorageSubBlock built
   *         by this CompressedBlockBuilder.
   **/
  inline std::size_t getMinimumRequiredBlockSize() const {
    return sizeof(tuple_id) + sizeof(int) + compression_info_.ByteSize();
  }

  /**
   * @brief Build a physical CompressedPackedRowStoreTupleStorageSubBlock with
   *        the tuples in this CompressedStorageSubBlock, automatically using
   *        the most-efficient compression method for each compressed column.
   *
   * @param sub_block_memory The physical memory location where the block will
   *        be stored. Must be at least as large as the block_size given to the
   *        constructor.
   **/
  void buildCompressedPackedRowStoreTupleStorageSubBlock(void *sub_block_memory);

  /**
   * @brief Build a physical CompressedColumnStoreTupleStorageSubBlock with
   *        the tuples in this CompressedStorageSubBlock, automatically using
   *        the most-efficient compression method for each compressed column.
   *
   * @param sub_block_memory The physical memory location where the block will
   *        be stored. Must be at least as large as the block_size given to the
   *        constructor.
   **/
  void buildCompressedColumnStoreTupleStorageSubBlock(void *sub_block_memory);

 private:
  bool addTupleInternal(Tuple *candidate_tuple);

  std::size_t computeRequiredStorage(const std::size_t num_tuples) const;
  std::size_t computeTruncatedByteLengthForAttribute(const attribute_id attr_id) const;

  void rollbackLastInsert(
      const std::vector<CompressionDictionaryBuilder*> &modified_dictionaries,
      const std::unordered_map<attribute_id, const TypedValue*> &previous_maximum_integers,
      std::unordered_map<attribute_id, bool> &&previous_null_present);

  std::size_t buildTupleStorageSubBlockHeader(void *sub_block_memory);
  void buildNullBitmapMap(void *sub_block_memory,
                          PtrMap<attribute_id, BitVector<false>> *null_bitmaps) const;

  void buildDictionaryCompressedColumnStripe(const attribute_id attr_id,
                                             const CompressionDictionaryBuilder &builder,
                                             void *stripe_location) const;
  void buildTruncationCompressedColumnStripe(const attribute_id attr_id,
                                             const bool attr_is_long,
                                             void *stripe_location) const;
  void buildUncompressedColumnStripe(const attribute_id attr_id,
                                     void *stripe_location) const;
  void buildNullableUncompressedColumnStripe(const attribute_id attr_id,
                                             void *stripe_location,
                                             BitVector<false> *null_bitmap) const;

  const CatalogRelationSchema &relation_;
  const std::size_t block_size_;
  // Real value for CompressedColumnStore, -1 for CompressedPackedRowStore:
  attribute_id sort_attribute_id_;

  PtrVector<Tuple> tuples_;

  bool sort_attribute_nullable_;
  std::vector<Tuple*> tuples_with_non_null_sort_value_;
  std::vector<Tuple*> tuples_with_null_sort_value_;

  CompressedBlockInfo compression_info_;
  PtrMap<attribute_id, CompressionDictionaryBuilder> dictionary_builders_;
  std::unordered_map<attribute_id, const TypedValue*> maximum_integers_;
  std::unordered_map<attribute_id, bool> null_present_;

  DISALLOW_COPY_AND_ASSIGN(CompressedBlockBuilder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_COMPRESSED_BLOCK_BUILDER_HPP_
