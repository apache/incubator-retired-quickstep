/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include "storage/PackedRowStoreTupleStorageSubBlock.hpp"

#include <cstddef>
#include <cstring>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "storage/PackedRowStoreValueAccessor.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageErrors.hpp"
#include "storage/SubBlockTypeRegistry.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/Tuple.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"

using std::vector;
using std::memcpy;
using std::size_t;

namespace quickstep {

QUICKSTEP_REGISTER_TUPLE_STORE(PackedRowStoreTupleStorageSubBlock, PACKED_ROW_STORE);

PackedRowStoreTupleStorageSubBlock::PackedRowStoreTupleStorageSubBlock(
    const CatalogRelationSchema &relation,
    const TupleStorageSubBlockDescription &description,
    const bool new_block,
    void *sub_block_memory,
    const std::size_t sub_block_memory_size)
    : TupleStorageSubBlock(relation,
                           description,
                           new_block,
                           sub_block_memory,
                           sub_block_memory_size),
      header_(static_cast<PackedRowStoreHeader*>(sub_block_memory)),
      null_bitmap_bytes_(0) {
  if (!DescriptionIsValid(relation_, description_)) {
    FATAL_ERROR("Attempted to construct a PackedRowStoreTupleStorageSubBlock from an invalid description.");
  }

  if (sub_block_memory_size < sizeof(PackedRowStoreHeader)) {
    throw BlockMemoryTooSmall("PackedRowStoreTupleStorageSubBlock", sub_block_memory_size);
  }

  if (relation_.hasNullableAttributes()) {
    // Compute on the order of bits to account for bits in null_bitmap_.
    tuple_id row_capacity = ((sub_block_memory_size_ - sizeof(PackedRowStoreHeader)) << 3)
                            / ((relation.getFixedByteLength() << 3) + relation.numNullableAttributes());
    null_bitmap_bytes_ = BitVector<false>::BytesNeeded(row_capacity * relation.numNullableAttributes());

    if (sub_block_memory_size < sizeof(PackedRowStoreHeader) + null_bitmap_bytes_) {
      if (relation_.getFixedByteLength() == 0) {
        // Special case: relation consists entirely of NullType attributes.
        row_capacity = BitVector<false>::MaxCapacityForBytes(
                           sub_block_memory_size - sizeof(PackedRowStoreHeader))
                       / relation.numNullableAttributes();
        null_bitmap_bytes_ = sub_block_memory_size - sizeof(PackedRowStoreHeader);
      } else {
        throw BlockMemoryTooSmall("PackedRowStoreTupleStorageSubBlock", sub_block_memory_size);
      }
    }

    null_bitmap_.reset(new BitVector<false>(static_cast<char*>(sub_block_memory_)
                                                + sizeof(PackedRowStoreHeader),
                                            row_capacity * relation.numNullableAttributes()));
    tuple_storage_ = static_cast<char*>(sub_block_memory_)
                         + sizeof(PackedRowStoreHeader)
                         + null_bitmap_bytes_;
  } else {
    tuple_storage_ = static_cast<char*>(sub_block_memory_)
                         + sizeof(PackedRowStoreHeader);
  }

  if (new_block) {
    header_->num_tuples = 0;
    if (relation_.hasNullableAttributes()) {
      null_bitmap_->clear();
    }
  }
}

bool PackedRowStoreTupleStorageSubBlock::DescriptionIsValid(
    const CatalogRelationSchema &relation,
    const TupleStorageSubBlockDescription &description) {
  // Make sure description is initialized and specifies PackedRowStore.
  if (!description.IsInitialized()) {
    return false;
  }
  if (description.sub_block_type() != TupleStorageSubBlockDescription::PACKED_ROW_STORE) {
    return false;
  }

  // Make sure relation is not variable-length.
  if (relation.isVariableLength()) {
    return false;
  }

  return true;
}

std::size_t PackedRowStoreTupleStorageSubBlock::EstimateBytesPerTuple(
    const CatalogRelationSchema &relation,
    const TupleStorageSubBlockDescription &description) {
  DEBUG_ASSERT(DescriptionIsValid(relation, description));

  // NOTE(chasseur): We round-up the number of bytes needed in the NULL bitmap
  // to avoid estimating 0 bytes needed for a relation with less than 8
  // attributes which are all NullType.
  return relation.getFixedByteLength()
         + ((relation.numNullableAttributes() + 7) >> 3);
}

tuple_id PackedRowStoreTupleStorageSubBlock::bulkInsertTuples(ValueAccessor *accessor) {
  const tuple_id original_num_tuples = header_->num_tuples;
  char *dest_addr = static_cast<char*>(tuple_storage_)
                      + header_->num_tuples * relation_.getFixedByteLength();
  const unsigned num_nullable_attrs = relation_.numNullableAttributes();

  InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    const std::size_t num_attrs = relation_.size();
    // We keep track of the total size of a tuple in the case that we do a
    // 'fast path' insertion later in the code.
    std::size_t attrs_total_size = 0;
    // Create a vector containing the maximum sizes of the to-be extracted
    // attributes.
    // NOTE(Marc): This is an optimization so that we need not use an iterator
    // object in the following inner loops.
    std::vector<std::size_t> attrs_max_size;
    for (CatalogRelationSchema::const_iterator attr_it = relation_.begin();
         attr_it != relation_.end();
         ++attr_it) {
      attrs_max_size.push_back(attr_it->getType().maximumByteLength());
      attrs_total_size += attrs_max_size.back();
    }

    if (num_nullable_attrs != 0) {
      while (this->hasSpaceToInsert<true>(1) && accessor->next()) {
        for (std::size_t curr_attr = 0; curr_attr < num_attrs; ++curr_attr) {
          const std::size_t attr_size = attrs_max_size[curr_attr];
          const attribute_id nullable_idx = relation_.getNullableAttributeIndex(curr_attr);
          // If this attribute is nullable, check for a returned null value.
          if (nullable_idx != kInvalidCatalogId) {
            const void *attr_value
                = accessor->template getUntypedValue<true>(curr_attr);
            if (attr_value == nullptr) {
              null_bitmap_->setBit(
                  header_->num_tuples * num_nullable_attrs + nullable_idx,
                  true);
            } else {
              memcpy(dest_addr, attr_value, attr_size);
            }
          } else {
            memcpy(dest_addr,
                   accessor->template getUntypedValue<false>(curr_attr),
                   attr_size);
          }
          dest_addr += attr_size;
        }
        ++(header_->num_tuples);
      }
    } else {
      // If the accessor is from a packed row store, we can optimize the
      // memcpy by avoiding iterating over each attribute.
      const bool fast_copy =
          (accessor->getImplementationType() ==
              ValueAccessor::Implementation::kCompressedPackedRowStore);
      while (this->hasSpaceToInsert<false>(1) && accessor->next()) {
        if (fast_copy) {
          memcpy(dest_addr,
                 accessor->template getUntypedValue<false>(0),
                 attrs_total_size);
        } else {
          for (std::size_t curr_attr = 0; curr_attr < num_attrs; ++curr_attr) {
            const std::size_t attr_size = attrs_max_size[curr_attr];
            memcpy(dest_addr,
                   accessor->template getUntypedValue<false>(curr_attr),
                   attr_size);
            dest_addr += attr_size;
          }
        }
        ++(header_->num_tuples);
      }
    }
  });

  return header_->num_tuples - original_num_tuples;
}

tuple_id PackedRowStoreTupleStorageSubBlock::bulkInsertTuplesWithRemappedAttributes(
    const std::vector<attribute_id> &attribute_map,
    ValueAccessor *accessor) {
  DEBUG_ASSERT(attribute_map.size() == relation_.size());

  const tuple_id original_num_tuples = header_->num_tuples;
  char *dest_addr = static_cast<char*>(tuple_storage_)
                      + header_->num_tuples * relation_.getFixedByteLength();
  const unsigned num_nullable_attrs = relation_.numNullableAttributes();

  InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    const std::size_t num_attrs = relation_.size();
    // Create a vector containing the maximum sizes of the to-be extracted
    // attributes.
    // NOTE(Marc): This is an optimization so that we need not use an iterator
    // object in the following inner loops.
    std::vector<std::size_t> attrs_max_size;
    for (CatalogRelationSchema::const_iterator attr_it = relation_.begin();
         attr_it != relation_.end();
         ++attr_it) {
      attrs_max_size.push_back(attr_it->getType().maximumByteLength());
    }

    if (num_nullable_attrs != 0) {
      while (this->hasSpaceToInsert<true>(1) && accessor->next()) {
        for (std::size_t curr_attr = 0; curr_attr < num_attrs; ++curr_attr) {
          const std::size_t attr_size = attrs_max_size[curr_attr];
          const attribute_id nullable_idx = relation_.getNullableAttributeIndex(curr_attr);
          // If this attribute is nullable, check for a returned null value.
          if (nullable_idx != kInvalidCatalogId) {
            const void *attr_value
                = accessor->template getUntypedValue<true>(attribute_map[curr_attr]);
            if (attr_value == nullptr) {
              null_bitmap_->setBit(
                  header_->num_tuples * num_nullable_attrs + nullable_idx,
                  true);
            } else {
              memcpy(dest_addr, attr_value, attr_size);
            }
          } else {
            memcpy(dest_addr,
                   accessor->template getUntypedValue<false>(attribute_map[curr_attr]),
                   attr_size);
          }
          dest_addr += attr_size;
        }
        ++(header_->num_tuples);
      }
    } else {
      while (this->hasSpaceToInsert<false>(1) && accessor->next()) {
        for (std::size_t curr_attr = 0; curr_attr < num_attrs; ++curr_attr) {
          const std::size_t attr_size = attrs_max_size[curr_attr];
          memcpy(dest_addr,
                 accessor->template getUntypedValue<false>(attribute_map[curr_attr]),
                 attr_size);
          dest_addr += attr_size;
        }
        ++(header_->num_tuples);
      }
    }
  });

  return header_->num_tuples - original_num_tuples;
}

const void* PackedRowStoreTupleStorageSubBlock::getAttributeValue(
    const tuple_id tuple,
    const attribute_id attr) const {
  DEBUG_ASSERT(hasTupleWithID(tuple));
  DEBUG_ASSERT(relation_.hasAttributeWithId(attr));

  const int nullable_idx = relation_.getNullableAttributeIndex(attr);
  if ((nullable_idx != -1)
      && null_bitmap_->getBit(tuple * relation_.numNullableAttributes() + nullable_idx)) {
    return nullptr;
  }

  return static_cast<char*>(tuple_storage_)                // Start of actual tuple storage.
         + (tuple * relation_.getFixedByteLength())        // Tuples prior to 'tuple'.
         + relation_.getFixedLengthAttributeOffset(attr);  // Attribute offset within tuple.
}

TypedValue PackedRowStoreTupleStorageSubBlock::getAttributeValueTyped(
    const tuple_id tuple,
    const attribute_id attr) const {
  const Type &attr_type = relation_.getAttributeById(attr)->getType();
  const void *untyped_value = getAttributeValue(tuple, attr);
  return (untyped_value == nullptr)
      ? attr_type.makeNullValue()
      : attr_type.makeValue(untyped_value, attr_type.maximumByteLength());
}

ValueAccessor* PackedRowStoreTupleStorageSubBlock::createValueAccessor(
    const TupleIdSequence *sequence) const {
  PackedRowStoreValueAccessor *base_accessor
      = new PackedRowStoreValueAccessor(relation_,
                                        relation_,
                                        header_->num_tuples,
                                        tuple_storage_,
                                        null_bitmap_.get());
  if (sequence == nullptr) {
    return base_accessor;
  } else {
    return new TupleIdSequenceAdapterValueAccessor<PackedRowStoreValueAccessor>(
        base_accessor,
        *sequence);
  }
}

void PackedRowStoreTupleStorageSubBlock::setAttributeValueInPlaceTyped(const tuple_id tuple,
                                                                       const attribute_id attr,
                                                                       const TypedValue &value) {
  DEBUG_ASSERT(hasTupleWithID(tuple));
  DEBUG_ASSERT(relation_.hasAttributeWithId(attr));
  DEBUG_ASSERT(value.isPlausibleInstanceOf(relation_.getAttributeById(attr)->getType().getSignature()));

  const int nullable_idx = relation_.getNullableAttributeIndex(attr);
  if (nullable_idx != -1) {
    if (value.isNull()) {
      null_bitmap_->setBit(tuple * relation_.numNullableAttributes() + nullable_idx, true);
      return;
    } else {
      null_bitmap_->setBit(tuple * relation_.numNullableAttributes() + nullable_idx, false);
    }
  }

  char *base_addr = static_cast<char*>(tuple_storage_)                // Start of actual tuple storage.
                    + (tuple * relation_.getFixedByteLength())        // Tuples prior to 'tuple'.
                    + relation_.getFixedLengthAttributeOffset(attr);  // Attribute offset within tuple.

  value.copyInto(base_addr);
}

bool PackedRowStoreTupleStorageSubBlock::deleteTuple(const tuple_id tuple) {
  DEBUG_ASSERT(hasTupleWithID(tuple));

  if (tuple == header_->num_tuples - 1) {
    // If deleting the last tuple, simply truncate.
    --(header_->num_tuples);
    if (null_bitmap_.get() != nullptr) {
      null_bitmap_->setBitRange(tuple * relation_.numNullableAttributes(),
                                relation_.numNullableAttributes(),
                                false);
    }
    return false;
  } else {
    const size_t tuple_length = relation_.getFixedByteLength();

    char *dest_addr = static_cast<char*>(tuple_storage_)  // Start of actual tuple storage.
                      + (tuple * tuple_length);           // Prior tuples.
    char *src_addr = dest_addr + tuple_length;  // Start of subsequent tuples.
    const size_t copy_bytes = (header_->num_tuples - tuple - 1) * tuple_length;  // Bytes in subsequent tuples.
    memmove(dest_addr, src_addr, copy_bytes);

    if (null_bitmap_.get() != nullptr) {
      null_bitmap_->shiftTailForward(tuple * relation_.numNullableAttributes(),
                                     relation_.numNullableAttributes());
    }

    --(header_->num_tuples);

    return true;
  }
}

bool PackedRowStoreTupleStorageSubBlock::bulkDeleteTuples(TupleIdSequence *tuples) {
  if (tuples->empty()) {
    // Nothing to do.
    return false;
  }

  const tuple_id front = tuples->front();
  const tuple_id back = tuples->back();
  const tuple_id num_tuples = tuples->numTuples();

  if ((back == header_->num_tuples - 1)
       && (back - front == num_tuples - 1)) {
    // Just truncate the back.
    header_->num_tuples = front;
    if (null_bitmap_.get() != nullptr) {
      null_bitmap_->setBitRange(header_->num_tuples * relation_.numNullableAttributes(),
                                num_tuples * relation_.numNullableAttributes(),
                                false);
    }
    return false;
  }

  // Pack the non-deleted tuples.
  const size_t tuple_length = relation_.getFixedByteLength();
  tuple_id dest_tid = front;
  tuple_id src_tid = dest_tid;

  TupleIdSequence::const_iterator it = tuples->begin();
  for (tuple_id current_id = front;
       current_id < header_->num_tuples;
       ++current_id, ++src_tid) {
    if (current_id == *it) {
      // Don't copy a deleted tuple.

      if (null_bitmap_.get() != nullptr) {
        // Erase the deleted tuple's entries in the null bitmap.
        null_bitmap_->shiftTailForward(dest_tid * relation_.numNullableAttributes(),
                                       relation_.numNullableAttributes());
      }

      ++it;
      if (it == tuples->end()) {
        // No more to delete, so copy all the remaining tuples in one go.
        memmove(static_cast<char*>(tuple_storage_) + dest_tid * tuple_length,
                static_cast<char*>(tuple_storage_) + (src_tid + 1) * tuple_length,
                (header_->num_tuples - back - 1) * tuple_length);
        break;
      }
    } else {
      // Copy the next tuple to the packed region.
      memmove(static_cast<char*>(tuple_storage_) + dest_tid * tuple_length,
              static_cast<char*>(tuple_storage_) + src_tid * tuple_length,
              tuple_length);
      ++dest_tid;
    }
  }

  header_->num_tuples -= static_cast<tuple_id>(num_tuples);

  return true;
}

template <bool nullable_attrs>
bool PackedRowStoreTupleStorageSubBlock::hasSpaceToInsert(const tuple_id num_tuples) const {
  if (sizeof(PackedRowStoreHeader)
          + null_bitmap_bytes_
          + (header_->num_tuples + num_tuples) * relation_.getFixedByteLength()
      <= sub_block_memory_size_) {
    if (nullable_attrs) {
      return static_cast<std::size_t>(header_->num_tuples + num_tuples) < null_bitmap_->size();
    } else {
      return true;
    }
  } else {
    return false;
  }
}

// Make sure both versions get compiled in.
template bool PackedRowStoreTupleStorageSubBlock::hasSpaceToInsert<false>(
    const tuple_id num_tuples) const;
template bool PackedRowStoreTupleStorageSubBlock::hasSpaceToInsert<true>(
    const tuple_id num_tuples) const;

template <bool nullable_attrs>
TupleStorageSubBlock::InsertResult PackedRowStoreTupleStorageSubBlock::insertTupleImpl(
    const Tuple &tuple) {
#ifdef QUICKSTEP_DEBUG
  paranoidInsertTypeCheck(tuple);
#endif
  if (!hasSpaceToInsert<nullable_attrs>(1)) {
    return InsertResult(-1, false);
  }

  char *base_addr = static_cast<char*>(tuple_storage_)                       // Start of actual tuple-storage region.
                    + header_->num_tuples * relation_.getFixedByteLength();  // Existing tuples.

  Tuple::const_iterator value_it = tuple.begin();
  CatalogRelationSchema::const_iterator attr_it = relation_.begin();

  while (value_it != tuple.end()) {
    if (nullable_attrs) {
      const int nullable_idx = relation_.getNullableAttributeIndex(attr_it->getID());
      if ((nullable_idx != -1) && value_it->isNull()) {
        null_bitmap_->setBit(header_->num_tuples * relation_.numNullableAttributes()
                                 + nullable_idx,
                             true);
      } else {
        value_it->copyInto(base_addr);
      }
    } else {
      value_it->copyInto(base_addr);
    }

    base_addr += attr_it->getType().maximumByteLength();

    ++value_it;
    ++attr_it;
  }

  ++(header_->num_tuples);

  return InsertResult(header_->num_tuples - 1, false);
}

}  // namespace quickstep
