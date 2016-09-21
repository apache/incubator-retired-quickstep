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

#ifndef QUICKSTEP_STORAGE_PACKED_ROW_STORE_VALUE_ACCESSOR_HPP_
#define QUICKSTEP_STORAGE_PACKED_ROW_STORE_VALUE_ACCESSOR_HPP_

#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/ValueAccessor.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class PackedRowStoreTupleStorageSubBlock;

class PackedRowStoreValueAccessorHelper {
 public:
  PackedRowStoreValueAccessorHelper(const CatalogRelationSchema &relation,
                                    const tuple_id num_tuples,
                                    const void *tuple_storage,
                                    const BitVector<false> *null_bitmap)
      : relation_(relation),
        num_tuples_(num_tuples),
        tuple_storage_(tuple_storage),
        null_bitmap_(null_bitmap) {
  }

  inline tuple_id numPackedTuples() const {
    return num_tuples_;
  }

  /**
   * @brief Returns whether this accessor has a fast strided ColumnAccessor available
   *        that can be used to optimize memory access in a tight loop iteration
   *        over the underlying storage block.
   *
   * @return true if fast ColumnAccessor is supported, otherwise false.
   */
  inline bool isColumnAccessorSupported() const {
    return true;
  }

  /**
   * @brief Get a pointer to a ColumnAccessor object that provides a fast strided memory
   *        access on the underlying storage block.
   * @note The ownership of the returned object lies with the caller.
   * @warning This method should only be called if isColumnAccessorSupported() method
   *          returned true. If ColumnAccessor is not supported this method will return a nullptr.
   *
   * @param current_tuple_position A constant reference to the tuple position in the containing
   *        ValueAccessor. This reference value is shared between the containing ValueAccessor &
   *        a ColumnAccessor. However, a ColumnAccessor *CANNOT* modify this tuple position.
   * @param attr_id The attribute id on which this ColumnAccessor will be created.
   *
   * @return A pointer to a ColumnAccessor object with specific properties set that can be used
   *         in a tight loop iterations over the underlying storage block.
   **/
  template <bool check_null = true>
  inline const ColumnAccessor<check_null>* getColumnAccessor(const tuple_id &current_tuple_position,
                                                             const attribute_id attr_id) const {
    DEBUG_ASSERT(relation_.hasAttributeWithId(attr_id));
    const void* base_location = static_cast<const char*>(tuple_storage_)
        + relation_.getFixedLengthAttributeOffset(attr_id);
    const std::size_t stride = relation_.getFixedByteLength();

    std::unique_ptr<ColumnAccessor<check_null>> column_accessor;
    if (check_null) {
      const int nullable_base = relation_.getNullableAttributeIndex(attr_id);
      const unsigned nullable_stride = relation_.numNullableAttributes();
      column_accessor.reset(new ColumnAccessor<check_null>(current_tuple_position,
                                                           num_tuples_,
                                                           base_location,
                                                           stride,
                                                           null_bitmap_,
                                                           nullable_base,
                                                           nullable_stride));
    } else {
      column_accessor.reset(new ColumnAccessor<check_null>(current_tuple_position,
                                                           num_tuples_,
                                                           base_location,
                                                           stride));
    }
    return column_accessor.release();
  }

  template <bool check_null>
  inline const void* getAttributeValue(const tuple_id tuple,
                                       const attribute_id attr) const {
    DEBUG_ASSERT(tuple < num_tuples_);
    DEBUG_ASSERT(relation_.hasAttributeWithId(attr));
    if (check_null) {
      const int nullable_idx = relation_.getNullableAttributeIndex(attr);
      if ((nullable_idx != -1)
          && null_bitmap_->getBit(tuple * relation_.numNullableAttributes() + nullable_idx)) {
        return nullptr;
      }
    }

    return static_cast<const char*>(tuple_storage_)          // Start of actual tuple storage.
           + (tuple * relation_.getFixedByteLength())        // Tuples prior to 'tuple'.
           + relation_.getFixedLengthAttributeOffset(attr);  // Attribute offset within tuple.
  }

  inline TypedValue getAttributeValueTyped(const tuple_id tuple,
                                           const attribute_id attr) const {
    const Type &attr_type = relation_.getAttributeById(attr)->getType();
    const void *untyped_value = getAttributeValue<true>(tuple, attr);
    return (untyped_value == nullptr)
        ? attr_type.makeNullValue()
        : attr_type.makeValue(untyped_value, attr_type.maximumByteLength());
  }

 private:
  const CatalogRelationSchema &relation_;
  const tuple_id num_tuples_;
  const void *tuple_storage_;
  const BitVector<false> *null_bitmap_;

  DISALLOW_COPY_AND_ASSIGN(PackedRowStoreValueAccessorHelper);
};

typedef PackedTupleStorageSubBlockValueAccessor<
    PackedRowStoreTupleStorageSubBlock,
    PackedRowStoreValueAccessorHelper,
    ValueAccessor::Implementation::kPackedRowStore>
        PackedRowStoreValueAccessor;

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_PACKED_ROW_STORE_VALUE_ACCESSOR_HPP_
