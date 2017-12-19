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

#ifndef QUICKSTEP_STORAGE_COMPRESSED_TUPLE_STORAGE_SUB_BLOCK_HPP_
#define QUICKSTEP_STORAGE_COMPRESSED_TUPLE_STORAGE_SUB_BLOCK_HPP_

#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "compression/CompressionDictionary.hpp"
#include "compression/CompressionDictionaryLite.hpp"
#include "storage/CompressedBlockBuilder.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageErrors.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrMap.hpp"

namespace quickstep {

class CatalogRelationSchema;
class ComparisonPredicate;
class Tuple;
class TupleStorageSubBlockDescription;
class ValueAccessor;

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief Abstract base class which implements common functionality for
 *        CompressedPackedRowStoreTupleStorageSubBlock and
 *        CompressedColumnStoreTupleStorageSubBlock.
 **/
class CompressedTupleStorageSubBlock : public TupleStorageSubBlock {
 public:
  CompressedTupleStorageSubBlock(
      const CatalogRelationSchema &relation,
      const TupleStorageSubBlockDescription &description,
      const bool new_block,
      void *sub_block_memory,
      const std::size_t sub_block_memory_size);

  virtual ~CompressedTupleStorageSubBlock() {
  }

  /**
   * @brief Get a long value for a comparison of a truncated attribute with a
   *        literal TypedValue. If the literal is a Float or Double with a
   *        fractional part, the value will be adjusted so that the comparison
   *        will still evaluate correctly.
   *
   * @param comp The ID of the comparison being evaluated (the order is
   *        'attribute comp literal'). Must be kLess, kLessOrEqual, kGreater,
   *        or kGreaterOrEqual (kEqual and kNotEqual are not supported).
   * @param right_literal A literal of a numeric type (Int, Long, Float, or
   *        Double) to get the effective truncated value for.
   * @param right_literal_type The specific concrete Type that right_literal
   *        belongs to.
   * @return The value of right_literal as a long, adjusted as necessary so
   *         that the specified comparison still evaluates correctly.
   **/
  static std::int64_t GetEffectiveLiteralValueForComparisonWithTruncatedAttribute(
      const ComparisonID comp,
      const TypedValue &right_literal,
      const Type &right_literal_type);

  bool supportsUntypedGetAttributeValue(const attribute_id attr) const override {
    return !truncated_attributes_[attr];
  }

  bool supportsAdHocInsert() const override {
    return false;
  }

  bool adHocInsertIsEfficient() const override {
    return false;
  }

  bool isEmpty() const override {
    if (builder_.get() == nullptr) {
      return *static_cast<const tuple_id*>(sub_block_memory_) == 0;
    } else {
      return builder_->numTuples() == 0;
    }
  }

  bool isPacked() const override {
    return true;
  }

  tuple_id getMaxTupleID() const override {
    return *static_cast<const tuple_id*>(sub_block_memory_) - 1;
  }

  bool hasTupleWithID(const tuple_id tid) const override {
    return tid < *static_cast<const tuple_id*>(sub_block_memory_);
  }

  InsertResult insertTuple(const Tuple &tuple) override {
    return InsertResult(-1, false);
  }

  bool insertTupleInBatch(const Tuple &tuple) override;

  tuple_id bulkInsertTuples(ValueAccessor *accessor) override;
  tuple_id bulkInsertTuplesWithRemappedAttributes(
      const std::vector<attribute_id> &attribute_map,
      ValueAccessor *accessor) override;

  bool canSetAttributeValuesInPlaceTyped(
      const tuple_id tuple,
      const std::unordered_map<attribute_id, TypedValue> &new_values) const override {
    return false;
  }

  void setAttributeValueInPlaceTyped(const tuple_id tuple,
                                     const attribute_id attr,
                                     const TypedValue &value) override {
    FATAL_ERROR("Called CompressedTupleStorageSubBlock::setAttributeValueInPlaceTyped(), "
                "which is not supported.");
  }

  // This override can more efficiently evaluate comparisons between a
  // compressed attribute and a literal value.
  TupleIdSequence* getMatchesForPredicate(const ComparisonPredicate &predicate,
                                          const TupleIdSequence *filter) const override;

  bool isCompressed() const override {
    return true;
  }

  // NOTE(chasseur): The methods below, with the prefix "compressed", are
  // intended for use by IndexSubBlock implementations which are capable of
  // using compressed codes directly. They can be accessed by checking that
  // isCompressed() is true, then casting to CompressedTupleStorageSubBlock.

  /**
   * @brief Check if the physical block has been built and compression has been
   *        finalized.
   *
   * @return true if the physical block has been built by a call to rebuild(),
   *         false if building is still in progress.
   **/
  bool compressedBlockIsBuilt() const {
    return builder_.get() == nullptr;
  }

  /**
   * @brief Check if an attribute may be compressed in a block which is not yet
   *        built.
   * @warning This method can only be called if compressedBlockIsBuilt()
   *          returns false.
   * @note Even if this method returns true, the attribute specified might
   *       still be uncompressed when the block is built if compression fails.
   *
   * @param attr_id The ID of the attribute to check for possible compression.
   * @return true if the attribute specified by attr_id might be compressed
   *         when rebuild() is called, false if no compression will be
   *         attempted.
   **/
  bool compressedUnbuiltBlockAttributeMayBeCompressed(const attribute_id attr_id) const {
    DEBUG_ASSERT(builder_.get() != nullptr);
    return builder_->attributeMayBeCompressed(attr_id);
  }

  /**
   * @brief Check if an attribute is dictionary-compressed.
   * @warning This method can only be called if compressedBlockIsBuilt()
   *          returns true.
   *
   * @param attr_id The ID of the attribute to check for dictionary
   *        compression.
   * @return true if the attribute specified by attr_id is dictionary-
   *         compressed, false otherwise.
   **/
  inline bool compressedAttributeIsDictionaryCompressed(const attribute_id attr_id) const {
    DEBUG_ASSERT(builder_.get() == nullptr);
    return dictionary_coded_attributes_[attr_id];
  }

  /**
   * @brief Check if an attribute is trunctation-compressed.
   * @warning This method can only be called if compressedBlockIsBuilt()
   *          returns true.
   *
   * @param attr_id The ID of the attribute to check for truncation.
   * @return true if the attribute specified by attr_id is an Int or Long which
   *         is truncated, false otherwise.
   **/
  inline bool compressedAttributeIsTruncationCompressed(const attribute_id attr_id) const {
    DEBUG_ASSERT(builder_.get() == nullptr);
    return truncated_attributes_[attr_id];
  }

  /**
   * @brief Check whether a truncated attribute is an instance of IntType or
   *        LongType.
   * @warning This method can only be called if compressedBlockIsBuilt()
   *          returns true.
   * @warning It is an error to call this method for an attribute which is not
   *          truncated (check compressedAttributeIsTruncationCompressed()
   *          first).
   *
   * @param attr_id The ID of a truncated attribute to determine the type of.
   * @return true if the attribute specified by attr_id is an Int, false if it
   *         is a Long.
   **/
  inline bool compressedTruncatedAttributeIsInt(const attribute_id attr_id) const {
    DEBUG_ASSERT(builder_.get() == nullptr);
    DEBUG_ASSERT(truncated_attributes_[attr_id]);
    return truncated_attribute_is_int_[attr_id];
  }

  /**
   * @brief Determine the compressed byte-length of an attribute.
   * @warning This method can only be called if compressedBlockIsBuilt()
   *          returns true.
   *
   * @param attr_id The ID of the attribute to check the size of.
   * @return The compressed length of the attribute specified by attr_id in
   *         bytes (or the original byte-length if the attribute is
   *         uncompressed).
   **/
  std::size_t compressedGetCompressedAttributeSize(const attribute_id attr_id) const {
    DEBUG_ASSERT(builder_.get() == nullptr);
    return compression_info_.attribute_size(attr_id);
  }

  /**
   * @brief Get the CompressionDictionary used for compressing an attribute.
   * @warning compressedAttributeIsDictionaryCompressed() must return true for
   *          the specified attribute.
   *
   * @param attr_id The ID of the attribute to get the CompressionDictionary
   *        for.
   * @return The CompressionDictionary used to compress the attribute specified
   *         by attr_id.
   **/
  const CompressionDictionary& compressedGetDictionary(const attribute_id attr_id) const {
    DEBUG_ASSERT(builder_.get() == nullptr);
    PtrMap<attribute_id, CompressionDictionaryLite>::const_iterator dict_it
        = dictionaries_.find(attr_id);
    if (dict_it == dictionaries_.end()) {
      FATAL_ERROR("Called CompressedTupleStorageSubBlock::getCompressionDictionary() "
                  "for an attribute which is not dictionary-compressed.");
    } else {
      return static_cast<const CompressionDictionary&>(*(dict_it->second));
    }
  }

  /**
   * @brief Determine if a comparison must always be true for any possible
   *        value of a truncated attribute.
   * @warning compressedAttributeIsTruncationCompressed() must return true for
   *          the specified attribute.
   *
   * @param comp The comparison to evaluate.
   * @param left_attr_id The ID of the truncated attribute on the left side
   *        of the comparison.
   * @param right_literal The literal value on the right side of the
   *        comparison.
   * @param right_literal_type The specific concrete Type that right_literal
   *        belongs to.
   * @return true if 'left_attr_id comp right_literal' must always be true for
   *         all possible values of the attribute specified by left_attr_id.
   **/
  bool compressedComparisonIsAlwaysTrueForTruncatedAttribute(
      const ComparisonID comp,
      const attribute_id left_attr_id,
      const TypedValue &right_literal,
      const Type &right_literal_type) const;

  /**
   * @brief Determine if a comparison must always be false for any possible
   *        value of a truncated attribute.
   * @warning compressedAttributeIsTruncationCompressed() must return true for
   *          the specified attribute.
   *
   * @param comp The comparison to evaluate.
   * @param left_attr_id The ID of the truncated attribute on the left side
   *        of the comparison.
   * @param right_literal The literal value on the right side of the
   *        comparison.
   * @param right_literal_type The specific concrete Type that right_literal
   *        belongs to.
   * @return true if 'left_attr_id comp right_literal' must always be false for
   *         all possible values of the attribute specified by left_attr_id.
   **/
  bool compressedComparisonIsAlwaysFalseForTruncatedAttribute(
      const ComparisonID comp,
      const attribute_id left_attr_id,
      const TypedValue &right_literal,
      const Type &right_literal_type) const;

  /**
   * @brief Get the compressed code for the specified tuple and compressed
   *        attribute.
   * @warning The specified attribute must be compressed, i.e. either
   *          compressedAttributeIsDictionaryCompressed() or
   *          compressedAttributeIsTruncationCompressed() must be true for it.
   *
   * @param tid The ID of the desired tuple.
   * @param attr_id The ID of the compressed attribute to get the code for.
   **/
  virtual std::uint32_t compressedGetCode(const tuple_id tid,
                                          const attribute_id attr_id) const = 0;

 protected:
  inline static std::int64_t GetMaxTruncatedValue(const std::size_t byte_length) {
    switch (byte_length) {
      case 1:
        return std::numeric_limits<std::uint8_t>::max();
      case 2:
        return std::numeric_limits<std::uint16_t>::max();
      case 4:
        return std::numeric_limits<std::uint32_t>::max() - 1;
      default:
        FATAL_ERROR("Unexpected byte_length for truncated value in "
                    "CompressedTupleStorageSubBlock::GetMaxTruncatedValue()");
    }
  }

  void* initializeCommon();

  // Implementations of each of these methods, with the exception of
  // getNotEqualCodesExcludingNull(), do not do any filtering to exclude codes
  // that map to NULL in the appropriate attribute's CompressionDictionary. If
  // such codes exist, it is the caller's responsibility to call one of these
  // methods in such a way that the NULL code (which is always the
  // highest-numbered code in the dictionary) is excluded (e.g. by using
  // getCodesInRange() instead of getGreaterOrEqualCodes()).
  // evaluateOtherComparisonPredicateOnCompressedAttribute() is written to do
  // this correctly.
  virtual TupleIdSequence* getEqualCodes(const attribute_id attr_id,
                                         const std::uint32_t code,
                                         const TupleIdSequence *filter) const = 0;

  virtual TupleIdSequence* getNotEqualCodes(const attribute_id attr_id,
                                            const std::uint32_t code,
                                            const TupleIdSequence *filter) const = 0;

  virtual TupleIdSequence* getNotEqualCodesExcludingNull(const attribute_id attr_id,
                                                         const std::uint32_t code,
                                                         const std::uint32_t null_code,
                                                         const TupleIdSequence *filter) const = 0;

  virtual TupleIdSequence* getLessCodes(const attribute_id attr_id,
                                        const std::uint32_t code,
                                        const TupleIdSequence *filter) const = 0;

  virtual TupleIdSequence* getGreaterOrEqualCodes(const attribute_id attr_id,
                                                  const std::uint32_t code,
                                                  const TupleIdSequence *filter) const = 0;

  virtual TupleIdSequence* getCodesInRange(const attribute_id attr_id,
                                           const std::pair<std::uint32_t, std::uint32_t> range,
                                           const TupleIdSequence *filter) const = 0;

  std::unique_ptr<CompressedBlockBuilder> builder_;

  CompressedBlockInfo compression_info_;

  std::vector<bool> dictionary_coded_attributes_;
  std::vector<bool> truncated_attributes_;
  std::vector<bool> truncated_attribute_is_int_;  // If false, attribute is long.

  // Note that all dictionaries are actually instances of the derived class
  // CompressionDictionary, but we use the base class CompressionDictionaryLite
  // here because ValueAccessors only need the Lite interface and we don't want
  // to introduce an unnecessary dependency on ComparisonUtil.
  PtrMap<attribute_id, CompressionDictionaryLite> dictionaries_;

 private:
  DISALLOW_COPY_AND_ASSIGN(CompressedTupleStorageSubBlock);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_COMPRESSED_TUPLE_STORAGE_SUB_BLOCK_HPP_
