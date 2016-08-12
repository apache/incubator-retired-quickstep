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

#include <algorithm>
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

// Unnamed namespace for helper functions that are implementation details and
// need not be exposed in the interface of the class (i.e., in the *.hpp file).
// 
// The first helper function here is used to provide an optimized bulk insertion path
// from RowStore to RowStore blocks, where contiguous attributes are copied
// together. For uniformity, there is another helper function that provides
// semantically identical `runs` for other input layouts as well.
namespace {
enum RunType {
  kContiguousAttributes,
  kNullableAttribute,
  kGap,
};


// struct Run contains information about ane run of attributes in the source
// ValueAccessor that can be copied into the output block. The
// getRunsForAttributeMap function below takes an attribute map for a source
// and converts it into a sequence of runs. The goal is to minimize the number
// of memcpy calls and address that occur during bulk insertion.
// Contiguous attributes from a rowstore source are merged into a single run.
//
// A ContiguousAttrsRun consists of contiguous attributes, nullable or not.
// "Contiguous" here means that their attribute IDs are successive in both
// the source and destination relations.
//
// A NullAttrRun refers to a nullable attribute. Nullable columns are
// represented using fixed length inline data as well as a null bitmap.
// In a particular tuple, if the attribute has a null value, the inline data
// has no meaning. So it is safe to copy it or not. We use this fact to merge
// runs together aggressively, i.e., a ContiguousAttrsRun may include a nullable
// attribute. However, we also create a NullAttrRun in that case (with 0 bytes
// to copy) in order to check the null bitmap.
//
// A GapRun is a run of destination (output) attributes that don't come from a
// particular source. This occurs during bulkInsertPartialTuples. They must be
// skipped during the insert (not copied over). They are indicated by a
// kInvalidCatalogId in the attribute map. Note that for efficiency, a GapRun
// is only created when it is at the start. Otherwise, the gap size is merged
// into the bytes_to_advance_ of previous run.
//
// eg. For 4B integer attrs, from a row store source,
// if the input attribute_map is {-1,0,5,6,7,-1,2,4,9,10}
// with input/output attributes 4 and 7 being nullable,
// we will create the following runs. (Note that the order may be different.)
//
//  ------------------------------------------------------------------------
//  |         run_type_ |source_attr_id_| bytes_to_copy_| bytes_to_advance_|
//  |-------------------|---------------|---------------|------------------|
//  |            GapRun |             -1|              0|                 4|
//  | ContigousAttrsRun |              0|              4|                 4|
//  | ContigousAttrsRun |              5|             12|                12|
//  |   NullableAttrRun |              7|              0|                 0|
//  | ContigousAttrsRun |              2|              4|                 4|
//  |   NullableAttrRun |              4|              4|                 4|
//  | ContigousAttrsRun |              9|              8|                 8|
//  ------------------------------------------------------------------------
// In this example, there are only 5 memcpy calls and 6 address calculations
// needed, even though there are 10 attributes in the attribute map.
//
struct Run {
  RunType run_type_;             // Type of run 
  attribute_id source_attr_id_;  // attr_id of starting input attribute for run
  std::size_t bytes_to_copy_;    // Number of bytes to copy from source
  std::size_t bytes_to_advance_; // Number of bytes to advance destination ptr
  int nullable_attr_idx_;        // For NullableAttrRun, index into null bitmap

  Run(const RunType run_type,
      const attribute_id mapped_attr_id,
      const std::size_t bytes_to_copy,
      const std::size_t bytes_to_advance,
      const int nullable_attr_idx)
      : run_type_(run_type),
        source_attr_id_(mapped_attr_id),
        bytes_to_copy_(bytes_to_copy),
        bytes_to_advance_(bytes_to_advance),
        nullable_attr_idx_(nullable_attr_idx) {};

  static Run ContiguousAttrsRun(
      const std::vector<std::size_t> &attrs_max_size,
      const attribute_id start_attr_id,
      const attribute_id num_contiguous_attrs,
      const attribute_id num_gap_attrs) {
    // Accumulate number of bytes to copy for contiguous attrs
    std::size_t bytes_to_copy = 0;
    for (attribute_id i = 0; i < num_contiguous_attrs; i++) {
      bytes_to_copy += attrs_max_size[start_attr_id + i];
    }

    // Accumulate number of bytes to skip for gap attrs
    std::size_t bytes_to_advance = bytes_to_copy;
    const std::size_t gap_start_attr_id = start_attr_id + num_contiguous_attrs;
    for (attribute_id i = 0; i < num_gap_attrs; i++) {
      bytes_to_advance += attrs_max_size[gap_start_attr_id + i];
    }

    return Run(kContiguousAttributes,
               start_attr_id,
               bytes_to_copy,
               bytes_to_advance,
               kInvalidCatalogId);
  }

  static Run NullableAttrRun(
      const std::vector<std::size_t> &attrs_max_size,
      const attribute_id attr_id,
      const int nullable_attr_idx,
      const attribute_id num_gap_attrs) {
    std::size_t bytes_to_copy = attrs_max_size[attr_id];

    // Accumulate number of bytes to skip for gap attrs
    std::size_t bytes_to_advance = bytes_to_copy;
    const std::size_t gap_start_attr_id = attr_id + 1;
    for (attribute_id i = 0; i < num_gap_attrs; i++) {
      bytes_to_advance += attrs_max_size[gap_start_attr_id + i];
    }

    return Run(kNullableAttribute,
               attr_id,
               bytes_to_copy,
               bytes_to_advance,
               nullable_attr_idx);
  }

  static Run GapRun(
      const std::vector<std::size_t> &attrs_max_size,
      const attribute_id num_gap_attrs) {
    std::size_t bytes_to_copy = 0;

    // Accumulate number of bytes to skip for gap attrs
    std::size_t bytes_to_advance = bytes_to_copy;
    for (attribute_id i = 0; i < num_gap_attrs; i++) {
      bytes_to_advance += attrs_max_size[i];
    }

    return Run(kGap,
               kInvalidCatalogId,
               bytes_to_copy,
               bytes_to_advance,
               kInvalidCatalogId);
    
  }
};

template <bool has_nullable_attrs>
bool isNullable(const CatalogRelationSchema &relation,
                const attribute_id attr_id,
                int &null_idx) {
  if (!has_nullable_attrs)
    return false;
  null_idx = relation.getNullableAttributeIndex(attr_id);
  return null_idx != kInvalidCatalogId;
}


// This helper function examines the schema of the input and output blocks
// and determines runs of attributes that can be copied at once.
// has_nullable_attrs: Check and break runs when there are nullable attributes.
//                     Caller should set based on relation schema.
// has_gaps: Check and break runs when there are gaps.
//           Caller should set this when there is more than one source ValueAccessor.
// merge_contiguous_attrs: Successive attribute IDs are merged into one run.
//                         Caller should set this when source is a row store.
template <bool has_nullable_attrs, bool has_gaps, bool merge_contiguous_attrs>
void getRunsForAttributeMap(
    const CatalogRelationSchema &relation,
    const std::vector<attribute_id> &attribute_map,
    const std::vector<std::size_t> &attrs_max_size,
    std::vector<Run> &runs) {
  attribute_id num_attrs = attribute_map.size();
  std::size_t curr_attr = 0;

  // First handle a starting GapRun
  if (has_gaps) {
    while (curr_attr < num_attrs && attribute_map[curr_attr] == kInvalidCatalogId)
      ++curr_attr;
    if (curr_attr > 0)
      runs.push_back(Run::GapRun(attrs_max_size, curr_attr));
  }

  // Starting with curr_attr set to the first non-gap attribute,
  // scan attribute_map to find contiguous runs.
  while (curr_attr < num_attrs) {
    std::size_t run_start = curr_attr;
    int null_idx = kInvalidCatalogId;
    if (!isNullable<has_nullable_attrs>(relation, curr_attr, null_idx)) {
      ++curr_attr;
      // Start of this run is non-nullable, so it is a ContiguousAttrsRun.
      // Find all following non-nullable contiguous attributes
      if (merge_contiguous_attrs) {
        while (curr_attr < num_attrs
             && attribute_map[curr_attr] == 1 + attribute_map[curr_attr-1]
             && !isNullable<has_nullable_attrs>(relation, curr_attr, null_idx))
          ++curr_attr;
      }
      // curr_attr should now be 1 beyond list of contiguous attributes to merge
      const attribute_id num_contiguous_attrs = curr_attr - run_start;

      // Now find how many gap attributes are next.
      const attribute_id gap_start = curr_attr;
      if (has_gaps) {
        while (curr_attr < num_attrs
               && attribute_map[curr_attr] == kInvalidCatalogId)
          ++curr_attr;
      }
      const attribute_id num_gap_attrs = curr_attr - gap_start;
      runs.push_back(Run::ContiguousAttrsRun(
          attrs_max_size, run_start, num_contiguous_attrs, num_gap_attrs));
    }
    else {
      // Start of this run is nullable, so it is a NullableAttrRun.
      // Find how many gap attributes are next.
      ++curr_attr;
      const attribute_id gap_start = curr_attr;
      if (has_gaps) {
        while (curr_attr < num_attrs
              && attribute_map[curr_attr] == kInvalidCatalogId)
          ++curr_attr;
      }
      attribute_id num_gap_attrs = curr_attr - gap_start;
      runs.push_back(Run::NullableAttrRun(
          attrs_max_size, run_start, null_idx, num_gap_attrs));
    }
  }
}

} // end Unnamed Namespace

template <bool has_nullable_attrs,
          bool has_gaps,
          bool merge_contiguous_attrs>
tuple_id PackedRowStoreTupleStorageSubBlock::bulkInsertTuplesHelper(
    const std::vector<attribute_id> &attribute_map,
    ValueAccessor *accessor,
    const tuple_id max_num_tuples_to_insert) {
  DEBUG_ASSERT(attribute_map.size() == relation_.size());

  tuple_id num_tuples_inserted = 0;
  char *dest_addr = static_cast<char *>(tuple_storage_) +
                    header_->num_tuples * relation_.getFixedByteLength();
  const unsigned num_nullable_attrs = relation_.numNullableAttributes();
  const std::vector<std::size_t> &attrs_max_size =
      relation_.getMaximumAttributeByteLengths();

  std::vector<Run> runs;
  runs.reserve(attribute_map.size());
  getRunsForAttributeMap<has_nullable_attrs, has_gaps, merge_contiguous_attrs>
      (relation_, attribute_map, attrs_max_size, runs);

  InvokeOnAnyValueAccessor(
    accessor,
    [&] (auto *accessor) -> void {  // NOLINT(build/c++11)

      // Inner lambda inserts one tuple.
      // Defining a lambda here reduces code duplication. It can't be defined
      // as a separate function because getUntypedValue is only a member of
      // the downcasted ValueAccessor subtypes, not the base ValueAccessor type.
      // So the inner lambda must be nested inside the outer lambda. 
      auto insertOneTupleUsingRuns =
          [&]() -> void {
        accessor->next();
        for (auto &run : runs) {  // NOLINT(build/c++11)
          if (run.run_type_ == RunType::kContiguousAttributes) {
            // It's a run with one or more non-nullable attributes. Copy data.
            memcpy(dest_addr,
                   accessor->template getUntypedValue<false>(run.source_attr_id_),
                   run.bytes_to_copy_);
            dest_addr += run.bytes_to_advance_;
          } else if (has_nullable_attrs
                     && run.run_type_ == RunType::kNullableAttribute){
            // It's a nullable attribute. Does it have null value?
            const void *attr_value =
            accessor->template getUntypedValue<true>(run.source_attr_id_);
            if (attr_value != nullptr) {
              memcpy(dest_addr, attr_value, run.bytes_to_copy_);
            } else {
              this->null_bitmap_->setBit(
                  this->header_->num_tuples * num_nullable_attrs
                  + run.nullable_attr_idx_,
                  true);
            }
            // In either case, increment dest_addr (leaving blank space in
            // case of null values)
            dest_addr += run.bytes_to_advance_;
          } else if (has_gaps
                     && run.run_type_ == RunType::kGap) {
            // It's a gap. Simply skip the bytes.
            dest_addr += run.bytes_to_advance_;
          }
        }
        ++num_tuples_inserted;
      }; // end (inner) lambda: insertOneTupleUsingRuns

      const tuple_id num_tuples_to_insert = std::min(
          estimateNumTuplesInsertable<has_nullable_attrs>(),
          max_num_tuples_to_insert);
      while(num_tuples_inserted < num_tuples_to_insert
            && !accessor->iterationFinished())
        insertOneTupleUsingRuns();
    });  // End (outer) lambda: argument to InvokeOnAnyValueAccessor

  if (!has_gaps)
    header_->num_tuples += num_tuples_inserted;
  return num_tuples_inserted;
}

template <bool has_gaps> tuple_id
PackedRowStoreTupleStorageSubBlock::bulkInsertTuplesDispatcher(
    const std::vector<attribute_id> &attribute_map,
    ValueAccessor *accessor,
    const tuple_id max_num_tuples_to_insert) {
  const bool has_nullable_attrs = (relation_.numNullableAttributes() > 0);
  auto impl = accessor->getImplementationType();
  const bool is_rowstore_source =
      (impl == ValueAccessor::Implementation::kPackedRowStore ||
       impl == ValueAccessor::Implementation::kSplitRowStore);

  // bool has_gaps = false;
  // for (auto &i : attribute_map)
  //   if (i == -1) {
  //     has_gaps = true;
  //     break;
  //   }

  if (has_nullable_attrs) {
      if (is_rowstore_source)
        return bulkInsertTuplesHelper<1,has_gaps,1>(
            attribute_map, accessor, max_num_tuples_to_insert);
      else
        return bulkInsertTuplesHelper<1,has_gaps,0>(
            attribute_map, accessor, max_num_tuples_to_insert);
    }
    else {
      if (is_rowstore_source)
        return bulkInsertTuplesHelper<0,has_gaps,1>(
            attribute_map, accessor, max_num_tuples_to_insert);
      else
        return bulkInsertTuplesHelper<0,has_gaps,0>(
            attribute_map, accessor, max_num_tuples_to_insert);
    }
}

tuple_id
PackedRowStoreTupleStorageSubBlock::bulkInsertTuples(ValueAccessor *accessor) {
  // Create a dummy attribute map and call bulkInsertTuples.
  std::vector<attribute_id> attribute_map;
  const attribute_id num_attrs = relation_.size();
  attribute_map.reserve(num_attrs);
  for (attribute_id i = 0; i < num_attrs; ++i)
    attribute_map.push_back(i);

  return bulkInsertTuplesWithRemappedAttributes(attribute_map, accessor);
}

tuple_id
PackedRowStoreTupleStorageSubBlock::bulkInsertTuplesWithRemappedAttributes(
    const std::vector<attribute_id> &attribute_map, ValueAccessor *accessor) {
  // This function does not permit an attribute_map containing gaps.
  return bulkInsertTuplesDispatcher<false>(
      attribute_map, accessor, kCatalogMaxID);
}

tuple_id PackedRowStoreTupleStorageSubBlock::bulkInsertPartialTuples(
    const std::vector<attribute_id> &attribute_map,
    ValueAccessor *accessor,
    const tuple_id max_num_tuples_to_insert) {
  return bulkInsertTuplesDispatcher<true>(
      attribute_map, accessor, max_num_tuples_to_insert);
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
tuple_id PackedRowStoreTupleStorageSubBlock::estimateNumTuplesInsertable() const{
  std::size_t tuple_size = relation_.getFixedByteLength();
  std::size_t remaining_subblock_bytes = sub_block_memory_size_
                                         - sizeof(PackedRowStoreHeader)
                                         - null_bitmap_bytes_
                                         - header_->num_tuples * tuple_size;
  tuple_id est_num_tuples = remaining_subblock_bytes/tuple_size;
  if (nullable_attrs) {
    tuple_id remaining_null_bitmap_bits = null_bitmap_->size()
                                          - header_->num_tuples;
    return std::min(est_num_tuples, remaining_null_bitmap_bits);
  } 
  return est_num_tuples;
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
