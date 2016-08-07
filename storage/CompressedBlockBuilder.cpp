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

#include "storage/CompressedBlockBuilder.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "compression/CompressionDictionaryBuilder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/TypeID.hpp"
#include "types/containers/Tuple.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonUtil.hpp"
#include "utility/BitManipulation.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"

using std::numeric_limits;
using std::pair;
using std::sort;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::vector;

namespace quickstep {

namespace {

// Very simple helper function to truncate an integral (either INT or LONG) in
// 'value' down to a code. Depending on the actual code length needed, the
// returned code may be further truncated down to 8 or 16 bits.
inline uint32_t TruncateIntegralValueToCode(const TypedValue &value,
                                            const bool value_is_long) {
  if (value_is_long) {
    return value.getLiteral<std::int64_t>();
  } else {
    return value.getLiteral<int>();
  }
}

}  // namespace

CompressedBlockBuilder::CompressedBlockBuilder(
    const CatalogRelationSchema &relation,
    const TupleStorageSubBlockDescription &description,
    const std::size_t block_size)
    : relation_(relation),
      block_size_(block_size),
      sort_attribute_id_(-1),
      sort_attribute_nullable_(false) {
  std::unordered_set<attribute_id> compressed_attribute_ids;

  if (description.sub_block_type() == TupleStorageSubBlockDescription::COMPRESSED_PACKED_ROW_STORE) {
    for (int compressed_attr_num = 0;
         compressed_attr_num < description.ExtensionSize(
             CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id);
         ++compressed_attr_num) {
      compressed_attribute_ids.insert(
          description.GetExtension(CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id,
                                   compressed_attr_num));
    }
  } else if (description.sub_block_type() == TupleStorageSubBlockDescription::COMPRESSED_COLUMN_STORE) {
    if (!description.HasExtension(CompressedColumnStoreTupleStorageSubBlockDescription::sort_attribute_id)) {
      FATAL_ERROR("Attempted to create a CompressedBlockBuilder with a "
                  "TupleStorageSubBlockDescription that specified a "
                  "sub_block_type of COMPRESSED_COLUMN_STORE, but did not "
                  "specify a sort_attribute_id.");
    }
    sort_attribute_id_ = description.GetExtension(
        CompressedColumnStoreTupleStorageSubBlockDescription::sort_attribute_id);
    sort_attribute_nullable_
        = relation_.getAttributeById(sort_attribute_id_)->getType().isNullable();

    for (int compressed_attr_num = 0;
         compressed_attr_num < description.ExtensionSize(
             CompressedColumnStoreTupleStorageSubBlockDescription::compressed_attribute_id);
         ++compressed_attr_num) {
      compressed_attribute_ids.insert(
          description.GetExtension(CompressedColumnStoreTupleStorageSubBlockDescription::compressed_attribute_id,
                                   compressed_attr_num));
    }
  } else {
    FATAL_ERROR("Attempted to create a CompressedBlockBuilder with a "
                "TupleStorageSubBlockDescription that did not specify a "
                "compressed sub_block_type.");
  }

  for (attribute_id attr_num = 0;
       attr_num <= relation.getMaxAttributeId();
       ++attr_num) {
    compression_info_.add_attribute_size(0);
    compression_info_.add_dictionary_size(0);
    compression_info_.add_uncompressed_attribute_has_nulls(false);

    if (relation_.hasAttributeWithId(attr_num)) {
      const Type &attr_type = relation_.getAttributeById(attr_num)->getType();

      if (compressed_attribute_ids.find(attr_num) != compressed_attribute_ids.end()) {
        dictionary_builders_.insert(attr_num, new CompressionDictionaryBuilder(attr_type));
        if ((attr_type.getTypeID() == kInt) || (attr_type.getTypeID() == kLong)) {
          maximum_integers_.insert(pair<attribute_id, const TypedValue*>(attr_num,
                                                                         nullptr));
        }
      }

      if (attr_type.isNullable()) {
        null_present_.emplace(attr_num, false);
      }
    }
  }

  compression_info_.set_null_bitmap_bits(0);
}

bool CompressedBlockBuilder::addTuple(const Tuple &tuple) {
  Tuple *candidate_tuple = tuple.clone();
  candidate_tuple->ensureLiteral();
  return addTupleInternal(candidate_tuple);
}

void CompressedBlockBuilder::buildCompressedPackedRowStoreTupleStorageSubBlock(void *sub_block_memory) {
  DEBUG_ASSERT(sort_attribute_id_ == -1);
  DEBUG_ASSERT(computeRequiredStorage(tuples_.size()) <= block_size_);

  // Build header and any dictionaries, and initialize null bitmap if needed.
  char *data_ptr = static_cast<char*>(sub_block_memory)
                   + buildTupleStorageSubBlockHeader(sub_block_memory);

  std::unique_ptr<BitVector<false>> null_bitmap;
  std::unordered_map<attribute_id, size_t> attribute_null_bitmap_offsets;
  size_t uncompressed_attributes_with_nulls = 0;
  if (compression_info_.null_bitmap_bits() > 0) {
    const size_t null_bitmap_bytes = BitVector<false>::BytesNeeded(
        compression_info_.null_bitmap_bits());
    null_bitmap.reset(new BitVector<false>(data_ptr - null_bitmap_bytes,
                                           compression_info_.null_bitmap_bits()));
    for (attribute_id attr_id = 0;
         attr_id <= relation_.getMaxAttributeId();
         ++attr_id) {
      if (compression_info_.uncompressed_attribute_has_nulls(attr_id)) {
        attribute_null_bitmap_offsets[attr_id] = uncompressed_attributes_with_nulls++;
      }
    }
  }

  for (PtrVector<Tuple>::size_type tuple_idx = 0;
       tuple_idx < tuples_.size();
       ++tuple_idx) {
    const Tuple &tuple = tuples_[tuple_idx];
    for (CatalogRelationSchema::const_iterator attr_it = relation_.begin();
         attr_it != relation_.end();
         ++attr_it) {
      const attribute_id attr_id = attr_it->getID();
      if (compression_info_.dictionary_size(attr_id) > 0) {
        // Attribute is dictionary-compressed.
        PtrMap<attribute_id, CompressionDictionaryBuilder>::const_iterator
            builder_it = dictionary_builders_.find(attr_id);
        DEBUG_ASSERT(builder_it != dictionary_builders_.end());
        switch (compression_info_.attribute_size(attr_id)) {
          case 1:
            *reinterpret_cast<uint8_t*>(data_ptr)
                = builder_it->second->getCodeForValue(tuple.getAttributeValue(attr_id));
            break;
          case 2:
            *reinterpret_cast<uint16_t*>(data_ptr)
                = builder_it->second->getCodeForValue(tuple.getAttributeValue(attr_id));
            break;
          case 4:
            *reinterpret_cast<uint32_t*>(data_ptr)
                = builder_it->second->getCodeForValue(tuple.getAttributeValue(attr_id));
            break;
          default:
            FATAL_ERROR("Dictionary-compressed type had non power-of-two length in "
                        "CompressedBlockBuilder::buildCompressedPackedRowStoreTupleStorageSubBlock()");
        }
      } else if (compression_info_.attribute_size(attr_id)
                 != attr_it->getType().maximumByteLength()) {
        // Attribute is compressed by truncation.
        switch (compression_info_.attribute_size(attr_id)) {
          case 1:
            *reinterpret_cast<uint8_t*>(data_ptr)
                = TruncateIntegralValueToCode(tuple.getAttributeValue(attr_id),
                                              attr_it->getType().getTypeID() == kLong);
            break;
          case 2:
            *reinterpret_cast<uint16_t*>(data_ptr)
                = TruncateIntegralValueToCode(tuple.getAttributeValue(attr_id),
                                              attr_it->getType().getTypeID() == kLong);
            break;
          case 4:
            *reinterpret_cast<uint32_t*>(data_ptr)
                = TruncateIntegralValueToCode(tuple.getAttributeValue(attr_id),
                                              attr_it->getType().getTypeID() == kLong);
            break;
          default:
            FATAL_ERROR("Truncation-compressed type had non power-of-two length in "
                        "CompressedBlockBuilder::buildCompressedPackedRowStoreupleStorageSubBlock()");
        }
      } else {
        // Attribute is uncompressed.
        if (tuple.getAttributeValue(attr_id).isNull()) {
          null_bitmap->setBit(tuple_idx * uncompressed_attributes_with_nulls
                                  + attribute_null_bitmap_offsets[attr_id],
                              true);
        } else {
          tuple.getAttributeValue(attr_id).copyInto(data_ptr);
        }
      }
      data_ptr += compression_info_.attribute_size(attr_id);
    }
  }
}

void CompressedBlockBuilder::buildCompressedColumnStoreTupleStorageSubBlock(void *sub_block_memory) {
  DEBUG_ASSERT(computeRequiredStorage(tuples_.size()) <= block_size_);

  // Sort tuples according to values of the sort attribute.
  const Type &sort_attribute_type = relation_.getAttributeById(sort_attribute_id_)->getType();
  if (sort_attribute_nullable_) {
    SortTuples(sort_attribute_id_,
               sort_attribute_type.getNonNullableVersion(),
               tuples_with_non_null_sort_value_.begin(),
               tuples_with_non_null_sort_value_.end());
    *(tuples_.getInternalVectorMutable()) = std::move(tuples_with_non_null_sort_value_);
    tuples_.getInternalVectorMutable()->insert(tuples_.getInternalVectorMutable()->end(),
                                               tuples_with_null_sort_value_.begin(),
                                               tuples_with_null_sort_value_.end());
  } else {
    SortTuples(sort_attribute_id_,
               sort_attribute_type,
               tuples_.getInternalVectorMutable()->begin(),
               tuples_.getInternalVectorMutable()->end());
  }

  char *current_stripe = static_cast<char*>(sub_block_memory)
                         + buildTupleStorageSubBlockHeader(sub_block_memory);

  PtrMap<attribute_id, BitVector<false>> null_bitmaps;
  buildNullBitmapMap(sub_block_memory, &null_bitmaps);

  size_t tuple_size = 0;
  size_t total_dictionary_size = 0;
  for (CatalogRelationSchema::const_iterator attr_it = relation_.begin();
       attr_it != relation_.end();
       ++attr_it) {
    tuple_size += compression_info_.attribute_size(attr_it->getID());
    total_dictionary_size += compression_info_.dictionary_size(attr_it->getID());
  }
  DEBUG_ASSERT(tuple_size > 0);
  size_t max_tuples = (block_size_
                      - (sizeof(tuple_id) + sizeof(int)
                         + compression_info_.ByteSize()
                         + total_dictionary_size
                         + null_bitmaps.size()
                           * BitVector<false>::BytesNeeded(compression_info_.null_bitmap_bits())))
                      / tuple_size;

  for (CatalogRelationSchema::const_iterator attr_it = relation_.begin();
       attr_it != relation_.end();
       ++attr_it) {
    if (compression_info_.dictionary_size(attr_it->getID()) > 0) {
      // Attribute is dictionary-compressed.
      PtrMap<attribute_id, CompressionDictionaryBuilder>::const_iterator
          builder_it = dictionary_builders_.find(attr_it->getID());
      DEBUG_ASSERT(builder_it != dictionary_builders_.end());
      buildDictionaryCompressedColumnStripe(attr_it->getID(),
                                            *(builder_it->second),
                                            current_stripe);
    } else if (compression_info_.attribute_size(attr_it->getID())
               != attr_it->getType().maximumByteLength()) {
      // Attribute is truncation-compressed.
      buildTruncationCompressedColumnStripe(attr_it->getID(),
                                            attr_it->getType().getTypeID() == kLong,
                                            current_stripe);
    } else {
      // Attribute is uncompressed.
      if (compression_info_.uncompressed_attribute_has_nulls(attr_it->getID())) {
        buildNullableUncompressedColumnStripe(attr_it->getID(),
                                              current_stripe,
                                              null_bitmaps.find(attr_it->getID())->second);
      } else {
        buildUncompressedColumnStripe(attr_it->getID(),
                                      current_stripe);
      }
    }

    current_stripe += max_tuples * compression_info_.attribute_size(attr_it->getID());
  }
}

bool CompressedBlockBuilder::addTupleInternal(Tuple *candidate_tuple) {
  DEBUG_ASSERT(candidate_tuple->size() == relation_.size());

  // Ensure that the tuple is the owner of its values.
  candidate_tuple->ensureLiteral();

  // Modify dictionaries and maximum integers to reflect the new tuple's
  // values. Keep track of what has changed in case a rollback is needed.
  vector<CompressionDictionaryBuilder*> modified_dictionaries;
  std::unordered_map<attribute_id, const TypedValue*> previous_maximum_integers;
  std::unordered_map<attribute_id, bool> previous_null_present(null_present_);

  CatalogRelationSchema::const_iterator attr_it = relation_.begin();
  Tuple::const_iterator value_it = candidate_tuple->begin();
  while (attr_it != relation_.end()) {
    attribute_id attr_id = attr_it->getID();

    PtrMap<attribute_id, CompressionDictionaryBuilder>::iterator
        dictionary_it = dictionary_builders_.find(attr_id);
    if (dictionary_it != dictionary_builders_.end()) {
      if (dictionary_it->second->insertEntryByReference(*value_it)) {
        modified_dictionaries.push_back(dictionary_it->second);
      }
    }

    if (value_it->isNull()) {
      DEBUG_ASSERT(attr_it->getType().isNullable());
      DEBUG_ASSERT(null_present_.find(attr_id) != null_present_.end());
      null_present_[attr_id] = true;
    }

    std::unordered_map<attribute_id, const TypedValue*>::iterator max_int_it
        = maximum_integers_.find(attr_id);
    if (max_int_it != maximum_integers_.end()) {
      // If the new value is null or less than zero, we can't truncate it.
      //
      // TODO(chasseur): With some effort, we could efficiently encode small
      // negative integers using zigzag encoding (although this would
      // complicate predicate evaluation). We could also use a null bitmap in
      // conjunction with truncation to deal with null values.
      bool value_is_negative_or_null = value_it->isNull();
      if (!value_is_negative_or_null) {
        switch (attr_it->getType().getTypeID()) {
          case kInt:
            value_is_negative_or_null = value_it->getLiteral<int>() < 0;
            break;
          case kLong:
            value_is_negative_or_null = value_it->getLiteral<std::int64_t>() < 0;
            break;
          default:
            FATAL_ERROR("Non-integer type encountered in the maximum_integers_ "
                        "map of a CompressedBlockBuilder.");
        }
      }

      if (value_is_negative_or_null) {
        previous_maximum_integers.emplace(attr_id, max_int_it->second);
        // Stop counting maximum, since we can no longer compress by truncating.
        maximum_integers_.erase(max_int_it);
      } else if (max_int_it->second == nullptr) {
        // If there was no maximum before, then this is the first value and
        // automatically becomes the maximum.
        previous_maximum_integers.emplace(attr_id, nullptr);
        max_int_it->second = &(*value_it);
      } else {
        // Compare with the previous maximum and update as necessary.
        bool new_maximum;
        switch (attr_it->getType().getTypeID()) {
          case kInt:
            new_maximum = max_int_it->second->getLiteral<int>()
                          < value_it->getLiteral<int>();
            break;
          case kLong:
            new_maximum = max_int_it->second->getLiteral<std::int64_t>()
                          < value_it->getLiteral<std::int64_t>();
            break;
          default:
            FATAL_ERROR("Non-integer type encountered in the maximum_integers_ map "
                        "of a CompressedBlockBuilder.");
        }

        if (new_maximum) {
          previous_maximum_integers.emplace(attr_id, max_int_it->second);
          max_int_it->second = &(*value_it);
        }
      }
    }

    ++attr_it;
    ++value_it;
  }

  if (computeRequiredStorage(tuples_.size() + 1) > block_size_) {
    rollbackLastInsert(modified_dictionaries,
                       previous_maximum_integers,
                       std::move(previous_null_present));
    delete candidate_tuple;
    return false;
  } else {
    if (sort_attribute_nullable_) {
      if (candidate_tuple->getAttributeValue(sort_attribute_id_).isNull()) {
        tuples_with_null_sort_value_.push_back(candidate_tuple);
      } else {
        tuples_with_non_null_sort_value_.push_back(candidate_tuple);
      }
    }
    tuples_.push_back(candidate_tuple);
    return true;
  }
}

std::size_t CompressedBlockBuilder::computeRequiredStorage(const std::size_t num_tuples) const {
  // Start with the size of the header.
  size_t required_storage = compression_info_.ByteSize() + sizeof(int) + sizeof(tuple_id);

  // Add required storage attribute-by-attribute.
  size_t uncompressed_attributes_with_nulls = 0;
  for (CatalogRelationSchema::const_iterator attr_it = relation_.begin();
       attr_it != relation_.end();
       ++attr_it) {
    PtrMap<attribute_id, CompressionDictionaryBuilder>::const_iterator
        dictionary_it = dictionary_builders_.find(attr_it->getID());
    if (dictionary_it == dictionary_builders_.end()) {
      // This attribute is not compressed.
      required_storage += num_tuples * attr_it->getType().maximumByteLength();

      if (attr_it->getType().isNullable() && null_present_.find(attr_it->getID())->second) {
        ++uncompressed_attributes_with_nulls;
      }
    } else if (attr_it->getType().isVariableLength()) {
      // Variable-length types MUST use dictionary compression.
      required_storage += dictionary_it->second->dictionarySizeBytes()
                          + num_tuples * dictionary_it->second->codeLengthPaddedBytes();
    } else {
      // Calculate the number of bytes needed to store all values when
      // truncating (if possible) or just storing values uncompressed.
      size_t truncated_bytes = num_tuples * computeTruncatedByteLengthForAttribute(attr_it->getID());

      // Compute the number of extra bytes that would be needed for a bitmap of
      // uncompressed nulls.
      //
      // TODO(chasseur): This is precisely accurate for CompressedColumnStore,
      // but may slightly overestimate required storage for
      // CompressedPackedRowStore, which uses one shared null bitmap in
      // row-major order, and therefore only "rounds up" to the nearest size_t
      // increment *once*, rather than once-per-column. Thus, there may be
      // some marginal edge cases for CompressedPackedRowStore where this
      // calculation is slightly biased in favor of compression.
      size_t null_bitmap_bytes = 0;
      if (attr_it->getType().isNullable() && null_present_.find(attr_it->getID())->second) {
        null_bitmap_bytes = BitVector<false>::BytesNeeded(num_tuples);
      }

      // Calculate the total number of bytes (including storage for the
      // dictionary itself) needed to store all values with dictionary
      // compression.
      size_t dictionary_bytes = dictionary_it->second->dictionarySizeBytes()
                                + num_tuples * dictionary_it->second->codeLengthPaddedBytes();

      // Choose the method that uses space most efficiently.
      if (truncated_bytes + null_bitmap_bytes < dictionary_bytes) {
        required_storage += truncated_bytes;
        if (null_bitmap_bytes != 0) {
          ++uncompressed_attributes_with_nulls;
        }
      } else {
        required_storage += dictionary_bytes;
      }
    }
  }

  // Add space for null bitmap(s) for uncompressed attributes, if any.
  if (uncompressed_attributes_with_nulls > 0) {
    if (sort_attribute_id_ == -1) {
      // CompressedPackedRowStore uses a single shared row-major null bitmap.
      required_storage += BitVector<false>::BytesNeeded(num_tuples * uncompressed_attributes_with_nulls);
    } else {
      // CompressedColumnStore uses separate per-column null bitmaps.
      required_storage += uncompressed_attributes_with_nulls
                          * BitVector<false>::BytesNeeded(num_tuples);
    }
  }
  return required_storage;
}

std::size_t CompressedBlockBuilder::computeTruncatedByteLengthForAttribute(
    const attribute_id attr_id) const {
  DEBUG_ASSERT(relation_.hasAttributeWithId(attr_id));

  size_t truncated_bytes = relation_.getAttributeById(attr_id)->getType().maximumByteLength();

  // Can't truncate if null values are present.
  std::unordered_map<attribute_id, bool>::const_iterator null_present_it
      = null_present_.find(attr_id);
  if ((null_present_it != null_present_.end()) && null_present_it->second) {
    return truncated_bytes;
  }

  std::unordered_map<attribute_id, const TypedValue*>::const_iterator max_int_it
      = maximum_integers_.find(attr_id);
  if ((max_int_it != maximum_integers_.end() && (max_int_it->second != nullptr))) {
    unsigned int leading_zero_bits;
    switch (max_int_it->second->getTypeID()) {
      case kInt:
        DEBUG_ASSERT(max_int_it->second->getLiteral<int>() >= 0);
        if (max_int_it->second->getLiteral<int>()) {
          leading_zero_bits = leading_zero_count<uint32_t>(
              static_cast<uint32_t>(max_int_it->second->getLiteral<int>()));
        } else {
          leading_zero_bits = 32;
        }
        break;
      case kLong:
        DEBUG_ASSERT(max_int_it->second->getLiteral<std::int64_t>() >= 0);
        if (max_int_it->second->getLiteral<std::int64_t>()) {
          // Due to a quirk in predicate evaluation on truncated values,
          // we shouldn't store UINT32_MAX truncated.
          if (max_int_it->second->getLiteral<std::int64_t>() == numeric_limits<uint32_t>::max()) {
            return truncated_bytes;
          }
          leading_zero_bits = leading_zero_count<uint64_t>(
              static_cast<uint64_t>(max_int_it->second->getLiteral<std::int64_t>()));
        } else {
          leading_zero_bits = 64;
        }
        break;
      default:
        FATAL_ERROR("Non-integer type encountered in the maximum_integers_ map "
                    "of a CompressedBlockBuilder.");
    }

    unsigned int needed_bits = (truncated_bytes << 3) - leading_zero_bits;
    if (needed_bits < 9) {
      truncated_bytes = 1;
    } else if (needed_bits < 17) {
      truncated_bytes = 2;
    } else if (needed_bits < 33) {
      truncated_bytes = 4;
    }
  }

  return truncated_bytes;
}

void CompressedBlockBuilder::rollbackLastInsert(
    const std::vector<CompressionDictionaryBuilder*> &modified_dictionaries,
    const std::unordered_map<attribute_id, const TypedValue*> &previous_maximum_integers,
    std::unordered_map<attribute_id, bool> &&previous_null_present) {
  for (vector<CompressionDictionaryBuilder*>::const_iterator dictionary_it = modified_dictionaries.begin();
       dictionary_it != modified_dictionaries.end();
       ++dictionary_it) {
    (*dictionary_it)->undoLastInsert();
  }

  for (std::unordered_map<attribute_id, const TypedValue*>::const_iterator previous_max_it
           = previous_maximum_integers.begin();
       previous_max_it != previous_maximum_integers.end();
       ++previous_max_it) {
    maximum_integers_[previous_max_it->first] = previous_max_it->second;
  }

  null_present_ = std::move(previous_null_present);
}

std::size_t CompressedBlockBuilder::buildTupleStorageSubBlockHeader(void *sub_block_memory) {
  // Build up the CompressedBlockInfo.
  size_t uncompressed_attributes_with_nulls = 0;
  for (CatalogRelationSchema::const_iterator attr_it = relation_.begin();
       attr_it != relation_.end();
       ++attr_it) {
    const attribute_id attr_id = attr_it->getID();
    const Type &attr_type = attr_it->getType();
    const bool attr_has_nulls = attr_type.isNullable()
                                && null_present_[attr_id];
    PtrMap<attribute_id, CompressionDictionaryBuilder>::const_iterator dictionary_it
        = dictionary_builders_.find(attr_id);
    if (dictionary_it == dictionary_builders_.end()) {
      // This attribute is not compressed.
      compression_info_.set_attribute_size(attr_id,
                                           attr_type.maximumByteLength());
      compression_info_.set_dictionary_size(attr_id, 0);

      compression_info_.set_uncompressed_attribute_has_nulls(attr_id,
                                                             attr_has_nulls);
      uncompressed_attributes_with_nulls += attr_has_nulls;
    } else if (attr_type.isVariableLength()) {
      // Variable-length types MUST use dictionary compression.
      compression_info_.set_attribute_size(attr_id,
                                           dictionary_it->second->codeLengthPaddedBytes());
      compression_info_.set_dictionary_size(attr_id,
                                            dictionary_it->second->dictionarySizeBytes());
      compression_info_.set_uncompressed_attribute_has_nulls(attr_id, false);
    } else {
      // Calculate the number of bytes needed to store all values when
      // truncating (if possible) or just storing values uncompressed.
      size_t truncated_bytes = tuples_.size() * computeTruncatedByteLengthForAttribute(attr_id);

      // Compute the number of extra bytes that would be needed for a bitmap of
      // uncompressed nulls.
      //
      // NOTE(chasseur): See note in computeRequiredStorage() about edge cases
      // with CompressedPackedRowStore that are slightly biased in favor of
      // compression.
      size_t null_bitmap_bytes = attr_has_nulls ? BitVector<false>::BytesNeeded(tuples_.size())
                                                : 0;

      // Calculate the total number of bytes (including storage for the
      // dictionary itself) needed to store all values with dictionary
      // compression.
      size_t dictionary_bytes = dictionary_it->second->dictionarySizeBytes()
                                + tuples_.size() * dictionary_it->second->codeLengthPaddedBytes();

      // Choose the method that uses space most efficiently.
      if (truncated_bytes + null_bitmap_bytes < dictionary_bytes) {
        compression_info_.set_attribute_size(attr_id,
                                             computeTruncatedByteLengthForAttribute(attr_id));
        compression_info_.set_dictionary_size(attr_id, 0);
        compression_info_.set_uncompressed_attribute_has_nulls(attr_id, attr_has_nulls);
        uncompressed_attributes_with_nulls += attr_has_nulls;
      } else {
        compression_info_.set_attribute_size(attr_id,
                                             dictionary_it->second->codeLengthPaddedBytes());
        compression_info_.set_dictionary_size(attr_id,
                                              dictionary_it->second->dictionarySizeBytes());
        compression_info_.set_uncompressed_attribute_has_nulls(attr_id, false);
      }
    }
  }

  if (uncompressed_attributes_with_nulls > 0) {
    if (sort_attribute_id_ == -1) {
      // CompressedPackedRowStore uses a single shared null bitmap.
      compression_info_.set_null_bitmap_bits(
          uncompressed_attributes_with_nulls * tuples_.size());
    } else {
      // CompressedColumnStore uses per-column null bitmaps.
      compression_info_.set_null_bitmap_bits(tuples_.size());
    }
  } else {
    compression_info_.set_null_bitmap_bits(0);
  }

  // Record the number of tuples.
  *static_cast<tuple_id*>(sub_block_memory) = tuples_.size();

  // Serialize the compression info.
  *reinterpret_cast<int*>(static_cast<char*>(sub_block_memory) + sizeof(tuple_id))
      = compression_info_.ByteSize();
  if (!compression_info_.SerializeToArray(static_cast<char*>(sub_block_memory) + sizeof(tuple_id) + sizeof(int),
                                          compression_info_.ByteSize())) {
    FATAL_ERROR("Failed to do binary serialization of CompressedBlockInfo in "
                "CompressedBlockBuilder::buildTupleStorageSubBlockHeader");
  }

  // Build the physical dictionaries.
  size_t memory_offset = sizeof(tuple_id) + sizeof(int) + compression_info_.ByteSize();
  for (attribute_id attr_id = 0;
       attr_id <= relation_.getMaxAttributeId();
       ++attr_id) {
    if (compression_info_.dictionary_size(attr_id) > 0) {
      dictionary_builders_.find(attr_id)->second->buildDictionary(
          static_cast<char*>(sub_block_memory) + memory_offset);
      memory_offset += compression_info_.dictionary_size(attr_id);
    }
  }

  // Initialize null bitmap(s), if any.
  if (uncompressed_attributes_with_nulls > 0) {
    if (sort_attribute_id_ == -1) {
      // CompressedPackedRowStore uses a single shared null bitmap.
      BitVector<false> null_bitmap(static_cast<char*>(sub_block_memory) + memory_offset,
                                   compression_info_.null_bitmap_bits());
      null_bitmap.clear();

      memory_offset += BitVector<false>::BytesNeeded(compression_info_.null_bitmap_bits());
    } else {
      // CompressedColumnStore uses per-column null bitmaps.
      for (attribute_id attr_id = 0;
           attr_id <= relation_.getMaxAttributeId();
           ++attr_id) {
        if (compression_info_.uncompressed_attribute_has_nulls(attr_id)) {
          BitVector<false> null_bitmap(static_cast<char*>(sub_block_memory) + memory_offset,
                                       compression_info_.null_bitmap_bits());
          null_bitmap.clear();

          memory_offset += BitVector<false>::BytesNeeded(compression_info_.null_bitmap_bits());
        }
      }
    }
  }

  return memory_offset;
}

void CompressedBlockBuilder::buildNullBitmapMap(
    void *sub_block_memory,
    PtrMap<attribute_id, BitVector<false>> *null_bitmaps) const {
  if (compression_info_.null_bitmap_bits() == 0) {
    return;
  }

  char *memory_position = static_cast<char*>(sub_block_memory)
                          + sizeof(int) + sizeof(tuple_id) + compression_info_.ByteSize();

  // Advance past all the dictionaries.
  for (attribute_id attr_id = 0;
       attr_id < compression_info_.dictionary_size_size();
       ++attr_id) {
    memory_position += compression_info_.dictionary_size(attr_id);
  }

  for (attribute_id attr_id = 0;
       attr_id < compression_info_.dictionary_size_size();
       ++attr_id) {
    if (compression_info_.uncompressed_attribute_has_nulls(attr_id)) {
      null_bitmaps->insert(attr_id,
                           new BitVector<false>(memory_position,
                                                compression_info_.null_bitmap_bits()));
      memory_position += BitVector<false>::BytesNeeded(compression_info_.null_bitmap_bits());
    }
  }
}

void CompressedBlockBuilder::buildDictionaryCompressedColumnStripe(
    const attribute_id attr_id,
    const CompressionDictionaryBuilder &builder,
    void *stripe_location) const {
  switch (compression_info_.attribute_size(attr_id)) {
    case 1:
      for (size_t tuple_num = 0;
           tuple_num < tuples_.size();
           ++tuple_num) {
        reinterpret_cast<uint8_t*>(stripe_location)[tuple_num]
            = builder.getCodeForValue(tuples_[tuple_num].getAttributeValue(attr_id));
      }
      break;
    case 2:
      for (size_t tuple_num = 0;
           tuple_num < tuples_.size();
           ++tuple_num) {
        reinterpret_cast<uint16_t*>(stripe_location)[tuple_num]
            = builder.getCodeForValue(tuples_[tuple_num].getAttributeValue(attr_id));
      }
      break;
    case 4:
      for (size_t tuple_num = 0;
           tuple_num < tuples_.size();
           ++tuple_num) {
        reinterpret_cast<uint32_t*>(stripe_location)[tuple_num]
            = builder.getCodeForValue(tuples_[tuple_num].getAttributeValue(attr_id));
      }
      break;
    default:
      FATAL_ERROR("Dictionary-compressed type had non power-of-two length in "
                   "CompressedBlockBuilder::buildDictionaryCompressedColumnStripe()");
  }
}

void CompressedBlockBuilder::buildTruncationCompressedColumnStripe(
    const attribute_id attr_id,
    const bool attr_is_long,
    void *stripe_location) const {
  switch (compression_info_.attribute_size(attr_id)) {
    case 1:
      for (size_t tuple_num = 0;
           tuple_num < tuples_.size();
           ++tuple_num) {
        reinterpret_cast<uint8_t*>(stripe_location)[tuple_num]
            = TruncateIntegralValueToCode(tuples_[tuple_num].getAttributeValue(attr_id),
                                          attr_is_long);
      }
      break;
    case 2:
      for (size_t tuple_num = 0;
           tuple_num < tuples_.size();
           ++tuple_num) {
        reinterpret_cast<uint16_t*>(stripe_location)[tuple_num]
            = TruncateIntegralValueToCode(tuples_[tuple_num].getAttributeValue(attr_id),
                                          attr_is_long);
      }
      break;
    case 4:
      for (size_t tuple_num = 0;
           tuple_num < tuples_.size();
           ++tuple_num) {
        reinterpret_cast<uint32_t*>(stripe_location)[tuple_num]
            = TruncateIntegralValueToCode(tuples_[tuple_num].getAttributeValue(attr_id),
                                          attr_is_long);
      }
      break;
    default:
      FATAL_ERROR("Truncation-compressed type had non power-of-two length in "
                   "CompressedBlockBuilder::buildTruncationCompressedColumnStripe()");
  }
}

void CompressedBlockBuilder::buildUncompressedColumnStripe(
    const attribute_id attr_id,
    void *stripe_location) const {
  char *value_location = static_cast<char*>(stripe_location);
  const size_t value_length = compression_info_.attribute_size(attr_id);
  for (PtrVector<Tuple>::const_iterator tuple_it = tuples_.begin();
       tuple_it != tuples_.end();
       ++tuple_it) {
    tuple_it->getAttributeValue(attr_id).copyInto(value_location);
    value_location += value_length;
  }
}

void CompressedBlockBuilder::buildNullableUncompressedColumnStripe(
    const attribute_id attr_id,
    void *stripe_location,
    BitVector<false> *null_bitmap) const {
  const size_t value_length = compression_info_.attribute_size(attr_id);
  if (value_length == 0) {
    for (PtrVector<Tuple>::size_type tuple_idx = 0;
         tuple_idx < tuples_.size();
         ++tuple_idx) {
      const TypedValue &value = tuples_[tuple_idx].getAttributeValue(attr_id);
      if (value.isNull()) {
        null_bitmap->setBit(tuple_idx, true);
      }
    }
  } else {
    char *value_location = static_cast<char*>(stripe_location);
    TypedValue last_value = GetLastValueForType(relation_.getAttributeById(attr_id)->getType());
    for (PtrVector<Tuple>::size_type tuple_idx = 0;
         tuple_idx < tuples_.size();
         ++tuple_idx) {
      const TypedValue &value = tuples_[tuple_idx].getAttributeValue(attr_id);
      if (value.isNull()) {
        null_bitmap->setBit(tuple_idx, true);
        last_value.copyInto(value_location);
      } else {
        value.copyInto(value_location);
      }
      value_location += value_length;
    }
  }
}

}  // namespace quickstep
