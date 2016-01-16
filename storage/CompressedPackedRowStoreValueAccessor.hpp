/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_STORAGE_COMPRESSED_PACKED_ROW_STORE_VALUE_ACCESSOR_HPP_
#define QUICKSTEP_STORAGE_COMPRESSED_PACKED_ROW_STORE_VALUE_ACCESSOR_HPP_

#include <cstddef>
#include <cstdint>
#include <vector>

#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "compression/CompressionDictionaryLite.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/ValueAccessor.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrMap.hpp"

namespace quickstep {

class CompressedPackedRowStoreTupleStorageSubBlock;

class CompressedPackedRowStoreValueAccessorHelper {
 public:
  CompressedPackedRowStoreValueAccessorHelper(
      const CatalogRelationSchema &relation,
      const tuple_id num_tuples,
      const std::size_t tuple_length_bytes,
      const std::vector<std::size_t> &attribute_offsets,
      const CompressedBlockInfo &compression_info,
      const std::vector<bool> &dictionary_coded_attributes,
      const std::vector<bool> &truncated_attributes,
      const std::vector<bool> &truncated_attribute_is_int,
      const PtrMap<attribute_id, CompressionDictionaryLite> &dictionaries,
      const void *tuple_storage,
      const BitVector<false> *null_bitmap,
      const attribute_id num_uncompressed_attributes_with_nulls,
      const std::vector<attribute_id> &null_bitmap_attribute_offsets)
      : relation_(relation),
        num_tuples_(num_tuples),
        tuple_length_bytes_(tuple_length_bytes),
        attribute_offsets_(attribute_offsets),
        compression_info_(compression_info),
        dictionary_coded_attributes_(dictionary_coded_attributes),
        truncated_attributes_(truncated_attributes),
        truncated_attribute_is_int_(truncated_attribute_is_int),
        dictionaries_(dictionaries),
        tuple_storage_(tuple_storage),
        null_bitmap_(null_bitmap),
        num_uncompressed_attributes_with_nulls_(num_uncompressed_attributes_with_nulls),
        null_bitmap_attribute_offsets_(null_bitmap_attribute_offsets) {
  }

  inline tuple_id numPackedTuples() const {
    return num_tuples_;
  }

  template <bool check_null>
  inline const void* getAttributeValue(const tuple_id tuple,
                                       const attribute_id attr) const {
    if (dictionary_coded_attributes_[attr]) {
      return dictionaries_.atUnchecked(attr).getUntypedValueForCode<check_null>(
          getCode(tuple, attr));
    } else if (truncated_attributes_[attr]) {
      if (truncated_attribute_is_int_[attr]) {
        int_buffer_ = getCode(tuple, attr);
        return &int_buffer_;
      } else {
        long_buffer_ = getCode(tuple, attr);
        return &long_buffer_;
      }
    } else {
      return getAttributePtr<check_null>(tuple, attr);
    }
  }

  inline TypedValue getAttributeValueTyped(const tuple_id tuple,
                                           const attribute_id attr) const {
    if (dictionary_coded_attributes_[attr]) {
      return dictionaries_.atUnchecked(attr).getTypedValueForCode(
          getCode(tuple, attr));
    } else if (truncated_attributes_[attr]) {
      if (truncated_attribute_is_int_[attr]) {
        return TypedValue(static_cast<int>(getCode(tuple, attr)));
      } else {
        return TypedValue(static_cast<std::int64_t>(getCode(tuple, attr)));
      }
    } else {
      const Type &attr_type = relation_.getAttributeById(attr)->getType();
      const void *untyped_value = getAttributePtr<true>(tuple, attr);
      return (untyped_value == nullptr)
          ? attr_type.makeNullValue()
          : attr_type.makeValue(untyped_value, attr_type.maximumByteLength());
    }
  }

 private:
  template <bool check_null>
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

  inline std::uint32_t getCode(const tuple_id tid,
                               const attribute_id attr_id) const {
    const void *code_location = getAttributePtr<false>(tid, attr_id);
    switch (compression_info_.attribute_size(attr_id)) {
      case 1:
        return *static_cast<const std::uint8_t*>(code_location);
      case 2:
        return *static_cast<const std::uint16_t*>(code_location);
      case 4:
        return *static_cast<const std::uint32_t*>(code_location);
      default:
        FATAL_ERROR("Unexpected byte-length (not 1, 2, or 4) for compressed "
                    "attribute ID " << attr_id
                    << " in CompressedPackedRowStoreValueAccessorHelper::getCode()");
    }
  }

  const CatalogRelationSchema &relation_;

  const tuple_id num_tuples_;
  const std::size_t tuple_length_bytes_;
  const std::vector<std::size_t> &attribute_offsets_;

  const CompressedBlockInfo &compression_info_;
  const std::vector<bool> &dictionary_coded_attributes_;
  const std::vector<bool> &truncated_attributes_;
  const std::vector<bool> &truncated_attribute_is_int_;
  const PtrMap<attribute_id, CompressionDictionaryLite> &dictionaries_;

  const void *tuple_storage_;

  const BitVector<false> *null_bitmap_;
  const attribute_id num_uncompressed_attributes_with_nulls_;
  const std::vector<attribute_id> &null_bitmap_attribute_offsets_;

  mutable int int_buffer_;
  mutable std::int64_t long_buffer_;

  DISALLOW_COPY_AND_ASSIGN(CompressedPackedRowStoreValueAccessorHelper);
};

typedef PackedTupleStorageSubBlockValueAccessor<
    CompressedPackedRowStoreTupleStorageSubBlock,
    CompressedPackedRowStoreValueAccessorHelper,
    ValueAccessor::Implementation::kCompressedPackedRowStore>
        CompressedPackedRowStoreValueAccessor;

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_COMPRESSED_PACKED_ROW_STORE_VALUE_ACCESSOR_HPP_
