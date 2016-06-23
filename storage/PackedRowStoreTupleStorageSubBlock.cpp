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
      [this, &dest_addr, &num_nullable_attrs](auto *accessor) -> void {  // NOLINT(build/c++11)
    const std::size_t num_attrs = relation_.size();
    const std::vector<std::size_t> &attrs_max_size =
        relation_.getMaximumAttributeByteLengths();

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
      const std::size_t attrs_total_size = relation_.getMaximumByteLength();
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

// Unnamed namespace for helper functions that are implementation details and
// need not be exposed in the interface of the class (i.e., in the *.hpp file).
// 
// The first helper function here is used to provide an optimized bulk insertion path
// from RowStore to RowStore blocks, where contiguous attributes are copied
// together. For uniformity, there is another helper function that provides
// semantically identical `runs` for other layouts as well.
namespace {
enum RunType {
  kContiguousAttributes,
  kNullableAttribute,
};

struct RunInfo {
  RunType run_type_;
  attribute_id mapped_attr_id_;
  std::size_t size_;
  int nullable_attr_idx_;

  static const int invalid_nullable_attr_idx_ = -1;

  RunInfo(RunType run_type, attribute_id mapped_attr_id, std::size_t size,
          int nullable_attr_idx)
      : run_type_(run_type),
        mapped_attr_id_(mapped_attr_id),
        size_(size),
        nullable_attr_idx_(nullable_attr_idx) {}

  static RunInfo getContiguousAttrsRun(attribute_id mapped_attr_id,
                               std::size_t size) {
    return RunInfo(kContiguousAttributes, mapped_attr_id,
                   size, invalid_nullable_attr_idx_);
  }

  static RunInfo getNullableAttrRun(attribute_id mapped_attr_id, std::size_t size,
                            int nullable_attr_idx) {
    return RunInfo(kNullableAttribute, mapped_attr_id,
                   size, nullable_attr_idx);
  }

};

// This helper function examines the schema of the input and output blocks
// and determines runs of attributes that can be copied at once.
// 
// For the i-th run of contiguous non-nullable attributes in the attribute map,
// `runs[i]` will contain a RunInfo struct.
//   run_start is the starting (mapped) attribute id of the run, and
//   size is the total size (i.e., num bytes) of all attributes in the run.
//
// For 4B integer attrs, with attribute_map {0,2,5,6,7,3,4,10}
// `runs` will be {(0,4), (2,4), (5,12), (3,8), (10,4)}
//
// Nullable attributes also break runs, just like non-contiguous attributes.
// They are recorded in `runs` using a sentinel run size kNullAttrRunSize (-1).
// In the above example, if 6 was nullable,
// `runs` will be {(0,4), (2,4), (5,4), (6,-1), (7,4), (3,8), (10,4)}
void getRunsForRowLayoutHelper(
    const CatalogRelationSchema &relation,
    const std::vector<attribute_id> &attribute_map,
    const std::vector<std::size_t> &attrs_max_size,
    std::vector<RunInfo> &runs) {
  std::size_t num_attrs = attribute_map.size();
  std::size_t curr_run_size = 0;
  std::size_t curr_run_start = 0;
  for (std::size_t curr_attr = 0; curr_attr < num_attrs; ++curr_attr) {
    int null_idx = relation.getNullableAttributeIndex(curr_attr);
    if (null_idx == kInvalidCatalogId) {
      if (curr_run_size != 0) {
        if (attribute_map[curr_attr] == 1 + attribute_map[curr_attr - 1]) {
          // If curr_attr is non-nullable,
          // ... and if there is an ongoing run,
          // ... and if curr_attr is adjacent to the previous attribute,
          // ... then we continue the current run.
          curr_run_size += attrs_max_size[curr_attr];
        } else {
          // If curr_attr is non-nullable
          // ... and there is an ongoing run,
          // ... but curr_attr is not adjacent to the previous attribute,
          // ... then we end the current run,
          runs.push_back(RunInfo::getContiguousAttrsRun(
              attribute_map[curr_run_start], curr_run_size));

          // ... and begin a new run.
          curr_run_size = attrs_max_size[curr_attr];
          curr_run_start = curr_attr;
        }
      } else {
        // If curr_attr is non-nullable, ...
        // ... but there is no ongoing run,
        // ... then we begin a new run.
        curr_run_size = attrs_max_size[curr_attr];
        curr_run_start = curr_attr;
      }
    } else {
      if (curr_run_size != 0) {
        // If curr_attr is nullable,
        // ... and there is an ongoing run,
        // ... then we end the current run,
        runs.push_back(RunInfo::getContiguousAttrsRun(
            attribute_map[curr_run_start], curr_run_size));
        curr_run_size = 0;

        // ... and record nulability of curr_attr as below.
      } else {
        // If curr_attr is nullable,
        // ... and there is no ongoing run,
        // ... we just have to record the nullability of curr_attr as below.
        // (Nothing to do here.)
      }

      // We must record that curr_attr is nullable, by inserting it into `runs`
      runs.push_back(RunInfo::getNullableAttrRun(
          attribute_map[curr_attr], attrs_max_size[curr_attr], null_idx));
    }
  } // end for-loop on attributes in attribute_map

  if (curr_run_size != 0) {
    // If there is an ongoing run, then we end it. 
    runs.push_back(RunInfo::getContiguousAttrsRun(
        attribute_map[curr_run_start], curr_run_size));
  }
}

// (See comments for above function for context.)
// For other layouts (not Row Store), the input attributes may not be contiguous,
// even if they have successive attribute IDs. So we just create a dummy `runs`
// vector, with every attribute being a run of its size. Again, as above,
// nullable attributes are given a sentinel run size kNullAttrRunSize.
void getRunsForOtherLayoutHelper(
    const CatalogRelationSchema &relation,
    const std::vector<attribute_id> &attribute_map,
    const std::vector<std::size_t> &attrs_max_size,
    std::vector<RunInfo> &runs) {
  std::size_t num_attrs = attribute_map.size();
  for (std::size_t curr_attr = 0; curr_attr < num_attrs; ++curr_attr) {
    int null_idx = relation.getNullableAttributeIndex(curr_attr);
    if (null_idx == kInvalidCatalogId) {
      runs.push_back(RunInfo::getContiguousAttrsRun(
          attribute_map[curr_attr], attrs_max_size[curr_attr]));
    } else {
      runs.push_back(RunInfo::getNullableAttrRun(
          attribute_map[curr_attr], attrs_max_size[curr_attr], null_idx));
    }
  }

}
} // end Unnamed Namespace

template <bool nullable_attrs> tuple_id
PackedRowStoreTupleStorageSubBlock::bulkInsertTuplesWithRemappedAttributesHelper(
    const std::vector<attribute_id> &attribute_map, ValueAccessor *accessor) {
  DEBUG_ASSERT(attribute_map.size() == relation_.size());

  const tuple_id original_num_tuples = header_->num_tuples;
  char *dest_addr = static_cast<char *>(tuple_storage_) +
                    header_->num_tuples * relation_.getFixedByteLength();
  const unsigned num_nullable_attrs = relation_.numNullableAttributes();
  const std::vector<std::size_t> &attrs_max_size =
      relation_.getMaximumAttributeByteLengths();

  std::vector<RunInfo> runs;
  runs.reserve(attribute_map.size());

  // We have an optimized implementation for RowStore to RowStore bulk insert.
  // For other layouts, we just create dummy `runs` for uniformity.
  auto impl = accessor->getImplementationType();
  if (impl == ValueAccessor::Implementation::kPackedRowStore ||
      impl == ValueAccessor::Implementation::kSplitRowStore) {
    getRunsForRowLayoutHelper(relation_, attribute_map, attrs_max_size, runs);
  } else {
    getRunsForOtherLayoutHelper(relation_, attribute_map, attrs_max_size, runs);
  }

  InvokeOnAnyValueAccessor(
    accessor,
    [this, &num_nullable_attrs, &dest_addr, &runs, &attrs_max_size]
    (auto *accessor) -> void {  // NOLINT(build/c++11)

      // Inner lambda inserts one tuple.
      // Defining a lambda here reduces code duplication. It can't be defined
      // as a separate function because getUntypedValue is only a member of
      // the downcasted ValueAccessor subtypes, not the
      // base ValueAccessor type. So the inner lambda has to be nested inside
      // the outer lambda. 
      auto insertOneTupleUsingRuns =
          [this, &accessor, &runs, &dest_addr, num_nullable_attrs]() -> void {
        for (auto &run : runs) {
          if (!nullable_attrs
              || run.run_type_ == RunType::kContiguousAttributes) {
            memcpy(dest_addr,
                   accessor->template getUntypedValue<false>(run.mapped_attr_id_),
                   run.size_);
            dest_addr += run.size_;
          } else {
            // It's a nullable attribute. Does it have null value?
            const void *attr_value =
                accessor->template getUntypedValue<true>(run.mapped_attr_id_);
            if (attr_value != nullptr) {
              memcpy(dest_addr, attr_value, run.size_);
            } else {
              this->null_bitmap_->setBit(
                  this->header_->num_tuples * num_nullable_attrs
                      + run.nullable_attr_idx_,
                  true);
            }
            // In either case, increment dest_addr (leaving blank space in
            // case of null values)
            dest_addr += run.size_;
          }
        }
        ++(this->header_->num_tuples);
      }; // end (inner) lambda: insertOneTupleUsingRuns

      // Insert one tuple at a time into this subblock using the lambda fn above.
      // We have split the loop in two and inserted in batches in order to make
      // fewer calls to the somewhat expensive function hasSpaceToInsert.
      //
      // Note the order of the test conditions in the inner for-loop here. The
      // loop terminates either when i == kNumTuplesInBatch, in which case
      // accessor position has not been incremented past the last insert,
      // or when accessor->next() returns false, in which case all tuples have
      // been inserted. So there is no possibility of missing a tuple between the
      // two loops.
      const unsigned kNumTuplesInBatch = 1000;
      while (this->hasSpaceToInsert<nullable_attrs>(kNumTuplesInBatch)
             && !accessor->iterationFinished()) {
        for (unsigned i = 0; i < kNumTuplesInBatch && accessor->next(); ++i)
          insertOneTupleUsingRuns();
      }
      while (this->hasSpaceToInsert<nullable_attrs>(1) && accessor->next())
        insertOneTupleUsingRuns();
    });  // End (outer) lambda: argument to InvokeOnAnyValueAccessor

  return header_->num_tuples - original_num_tuples;
}

tuple_id
PackedRowStoreTupleStorageSubBlock::bulkInsertTuplesWithRemappedAttributes(
    const std::vector<attribute_id> &attribute_map, ValueAccessor *accessor) {
  const unsigned num_nullable_attrs = relation_.numNullableAttributes();
  if (num_nullable_attrs > 0)
    return bulkInsertTuplesWithRemappedAttributesHelper<true>(
        attribute_map, accessor);
  
  return bulkInsertTuplesWithRemappedAttributesHelper<false>(
        attribute_map, accessor);
}

const void *PackedRowStoreTupleStorageSubBlock::getAttributeValue(
    const tuple_id tuple, const attribute_id attr) const {
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
