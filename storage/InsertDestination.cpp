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

#include "storage/InsertDestination.hpp"

#include <cstddef>
#include <cstdlib>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "catalog/PartitionSchemeHeader.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "storage/InsertDestination.pb.h"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "threading/SpinMutex.hpp"
#include "threading/ThreadIDBasedMap.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/Tuple.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

using std::free;
using std::malloc;
using std::move;
using std::vector;

namespace quickstep {

InsertDestination::InsertDestination(const InsertDestinationType insert_dest_type,
                                     const CatalogRelationSchema &relation,
                                     const StorageBlockLayout *layout,
                                     StorageManager *storage_manager,
                                     const std::size_t relational_op_index,
                                     const std::size_t query_id,
                                     const tmb::client_id scheduler_client_id,
                                     tmb::MessageBus *bus)
    : insert_dest_type_(insert_dest_type),
      thread_id_map_(*ClientIDMap::Instance()),
      storage_manager_(storage_manager),
      relation_(relation),
      layout_(layout),
      relational_op_index_(relational_op_index),
      query_id_(query_id),
      scheduler_client_id_(scheduler_client_id),
      bus_(DCHECK_NOTNULL(bus)) {
  if (layout_ == nullptr) {
    layout_.reset(StorageBlockLayout::GenerateDefaultLayout(relation, relation.isVariableLength()));
  }
}

InsertDestination* InsertDestination::ReconstructFromProto(
    const std::size_t query_id,
    const serialization::InsertDestination &proto,
    const CatalogRelationSchema &relation,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  DCHECK(ProtoIsValid(proto, relation));

  StorageBlockLayout *layout = nullptr;
  if (proto.has_layout()) {
    // InsertDestination ctor will own this layout.
    layout = new StorageBlockLayout(relation, proto.layout());
  }

  switch (proto.insert_destination_type()) {
    case serialization::InsertDestinationType::ALWAYS_CREATE_BLOCK: {
      return new AlwaysCreateBlockInsertDestination(relation,
                                                    layout,
                                                    storage_manager,
                                                    proto.relational_op_index(),
                                                    query_id,
                                                    scheduler_client_id,
                                                    bus);
    }
    case serialization::InsertDestinationType::BLOCK_POOL: {
      vector<block_id> blocks;
      for (int i = 0; i < proto.ExtensionSize(serialization::BlockPoolInsertDestination::blocks); ++i) {
        blocks.push_back(proto.GetExtension(serialization::BlockPoolInsertDestination::blocks, i));
      }

      return new BlockPoolInsertDestination(relation,
                                            layout,
                                            storage_manager,
                                            move(blocks),
                                            proto.relational_op_index(),
                                            query_id,
                                            scheduler_client_id,
                                            bus);
    }
    case serialization::InsertDestinationType::PARTITION_AWARE: {
      const serialization::PartitionScheme &proto_partition_scheme =
          proto.GetExtension(serialization::PartitionAwareInsertDestination::partition_scheme);

      vector<vector<block_id>> partitions;
      for (int partition_index = 0; partition_index < proto_partition_scheme.partitions_size(); ++partition_index) {
        vector<block_id> partition;
        const serialization::Partition &proto_partition = proto_partition_scheme.partitions(partition_index);
        for (int block_index = 0; block_index < proto_partition.blocks_size(); ++block_index) {
          partition.push_back(proto_partition.blocks(block_index));
        }
        partitions.push_back(move(partition));
      }

      return new PartitionAwareInsertDestination(
          PartitionSchemeHeader::ReconstructFromProto(proto_partition_scheme.header()),
          relation,
          layout,
          storage_manager,
          move(partitions),
          proto.relational_op_index(),
          query_id,
          scheduler_client_id,
          bus);
    }
    default: {
      LOG(FATAL) << "Unrecognized InsertDestinationType in proto";
    }
  }
}

bool InsertDestination::ProtoIsValid(const serialization::InsertDestination &proto,
                                     const CatalogRelationSchema &relation) {
  if (!proto.IsInitialized() ||
      !serialization::InsertDestinationType_IsValid(proto.insert_destination_type()) ||
      proto.relation_id() != relation.getID()) {
    return false;
  }

  if (proto.has_layout() &&
      !StorageBlockLayout::DescriptionIsValid(relation, proto.layout())) {
    return false;
  }

  return true;
}

void InsertDestination::insertTuple(const Tuple &tuple) {
  MutableBlockReference output_block = getBlockForInsertion();

  try {
    while (!output_block->insertTuple(tuple)) {
      returnBlock(std::move(output_block), true);
      output_block = getBlockForInsertion();
    }
  } catch (...) {
    returnBlock(std::move(output_block), false);
    throw;
  }

  returnBlock(std::move(output_block), false);
}

void InsertDestination::insertTupleInBatch(const Tuple &tuple) {
  MutableBlockReference output_block = getBlockForInsertion();

  try {
    while (!output_block->insertTupleInBatch(tuple)) {
      returnBlock(std::move(output_block), true);
      output_block = getBlockForInsertion();
    }
  } catch (...) {
    returnBlock(std::move(output_block), false);
    throw;
  }

  returnBlock(std::move(output_block), false);
}

void InsertDestination::bulkInsertTuples(ValueAccessor *accessor, bool always_mark_full) {
  InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    accessor->beginIteration();
    while (!accessor->iterationFinished()) {
      MutableBlockReference output_block = this->getBlockForInsertion();
      // FIXME(chasseur): Deal with TupleTooLargeForBlock exception.
      if (output_block->bulkInsertTuples(accessor) == 0) {
        // output_block is full.
        this->returnBlock(std::move(output_block), true);
      } else {
        // Bulk insert into output_block was successful. output_block
        // will be rebuilt when there won't be any more insertions to it.
        this->returnBlock(std::move(output_block),
                          always_mark_full || !accessor->iterationFinished());
      }
    }
  });
}

void InsertDestination::bulkInsertTuplesWithRemappedAttributes(
    const std::vector<attribute_id> &attribute_map,
    ValueAccessor *accessor,
    bool always_mark_full) {
  InvokeOnAnyValueAccessor(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    accessor->beginIteration();
    while (!accessor->iterationFinished()) {
      MutableBlockReference output_block = this->getBlockForInsertion();
      // FIXME(chasseur): Deal with TupleTooLargeForBlock exception.
      if (output_block->bulkInsertTuplesWithRemappedAttributes(
              attribute_map,
              accessor) == 0) {
        // output_block is full.
        this->returnBlock(std::move(output_block), true);
      } else {
        // Bulk insert into output_block was successful. output_block
        // will be rebuilt when there won't be any more insertions to it.
        this->returnBlock(std::move(output_block),
                          always_mark_full || !accessor->iterationFinished());
      }
    }
  });
}

// A common case that we can optimize away is when the attribute_map
// for an accessor only contains gaps. e.g. This happens for a join when
// there are no attributes selected from one side.
void removeGapOnlyAccessors(
  const std::vector<std::pair<ValueAccessor *, std::vector<attribute_id>>>* accessor_attribute_map,
  std::vector<std::pair<ValueAccessor *, const std::vector<attribute_id>>>* reduced_accessor_attribute_map) {
  for (std::size_t i = 0; i < accessor_attribute_map->size(); ++i) {
    bool all_gaps = true;
    for (const auto &attr : (*accessor_attribute_map)[i].second)
      if (attr != kInvalidCatalogId) {
        all_gaps = false;
        break;
      }
    if (all_gaps)
      continue;
    reduced_accessor_attribute_map->push_back((*accessor_attribute_map)[i]);
    (*accessor_attribute_map)[i].first->beginIterationVirtual();
  }
}

void InsertDestination::bulkInsertTuplesFromValueAccessors(
    const std::vector<std::pair<ValueAccessor *, std::vector<attribute_id>>> &accessor_attribute_map,
    bool always_mark_full) {
  // Handle pathological corner case where there are no accessors
  if (accessor_attribute_map.size() == 0)
    return;

  std::vector<std::pair<ValueAccessor *, const std::vector<attribute_id>>> reduced_accessor_attribute_map;
  removeGapOnlyAccessors(&accessor_attribute_map, &reduced_accessor_attribute_map);

  // We assume that all input accessors have the same number of tuples, so
  // the iterations finish together. Therefore, we can just check the first one.
  auto first_accessor = reduced_accessor_attribute_map[0].first;
  while (!first_accessor->iterationFinishedVirtual()) {
    tuple_id num_tuples_to_insert = kCatalogMaxID;
    tuple_id num_tuples_inserted = 0;
    MutableBlockReference output_block = this->getBlockForInsertion();

    // Now iterate through all the accessors and do one round of bulk-insertion
    // of partial tuples into the selected output_block.
    // While inserting from the first ValueAccessor, space is reserved for
    // all the columns including those coming from other ValueAccessors.
    // Thereafter, in a given round, we only insert the remaining columns of the
    // same tuples from the other ValueAccessors.
    for (auto &p : reduced_accessor_attribute_map) {
      ValueAccessor *accessor = p.first;
      const std::vector<attribute_id> &attribute_map = p.second;

      InvokeOnAnyValueAccessor(
          accessor,
          [&](auto *accessor) -> void {  // NOLINT(build/c++11)
            num_tuples_inserted = output_block->bulkInsertPartialTuples(
                attribute_map, accessor, num_tuples_to_insert);
      });

      if (accessor == first_accessor) {
        // Now we know how many full tuples can be inserted into this
        // output_block (viz. number of tuples inserted from first ValueAccessor).
        // We should only insert that many tuples from the remaining
        // ValueAccessors as well.
        num_tuples_to_insert = num_tuples_inserted;
      } else {
        // Since the bulk insertion of the first ValueAccessor should already
        // have reserved the space for all the other ValueAccessors' columns,
        // we must have been able to insert all the tuples we asked to insert.
        DCHECK_EQ(num_tuples_inserted, num_tuples_to_insert);
      }
    }

    // After one round of insertions, we have successfully inserted as many
    // tuples as possible into the output_block. Strictly speaking, it's
    // possible that there is more space for insertions because the size
    // estimation of variable length columns is conservative. But we will ignore
    // that case and proceed assuming that this output_block is full.

    // Update the header for output_block and then return it.
    output_block->bulkInsertPartialTuplesFinalize(num_tuples_inserted);
    const bool mark_full = always_mark_full
                           || !first_accessor->iterationFinishedVirtual();
    this->returnBlock(std::move(output_block), mark_full);
  }
}

void InsertDestination::insertTuplesFromVector(std::vector<Tuple>::const_iterator begin,
                                               std::vector<Tuple>::const_iterator end) {
  if (begin == end) {
    return;
  }

  MutableBlockReference dest_block = getBlockForInsertion();
  for (; begin != end; ++begin) {
    // FIXME(chasseur): Deal with TupleTooLargeForBlock exception.
    while (!dest_block->insertTupleInBatch(*begin)) {
      returnBlock(std::move(dest_block), true);
      dest_block = getBlockForInsertion();
    }
  }
  returnBlock(std::move(dest_block), false);
}

MutableBlockReference AlwaysCreateBlockInsertDestination::createNewBlock() {
  const block_id new_id = storage_manager_->createBlock(relation_, *layout_);

  // Notify Foreman to add the newly created block id in the master Catalog.
  serialization::CatalogRelationNewBlockMessage proto;
  proto.set_relation_id(relation_.getID());
  proto.set_block_id(new_id);
  proto.set_query_id(getQueryID());

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage tagged_msg(static_cast<const void *>(proto_bytes),
                           proto_length,
                           kCatalogRelationNewBlockMessage);
  free(proto_bytes);

  DLOG(INFO) << "AlwaysCreateBlockInsertDestination sent CatalogRelationNewBlockMessage to Scheduler with Client "
             << scheduler_client_id_;
  const tmb::MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(bus_,
                                         thread_id_map_.getValue(),
                                         scheduler_client_id_,
                                         move(tagged_msg));
  CHECK(send_status == tmb::MessageBus::SendStatus::kOK);

  return storage_manager_->getBlockMutable(new_id, relation_);
}

MutableBlockReference AlwaysCreateBlockInsertDestination::getBlockForInsertion() {
  SpinMutexLock lock(mutex_);
  return createNewBlock();
}

void AlwaysCreateBlockInsertDestination::returnBlock(MutableBlockReference &&block, const bool full) {
  {
    SpinMutexLock lock(mutex_);
    returned_block_ids_.push_back(block->getID());
  }
  if (!block->rebuild()) {
    LOG_WARNING("Rebuilding of StorageBlock with ID: " << block->getID() <<
                "invalidated one or more IndexSubBlocks.");
  }
  // Due to the nature of this InsertDestination, a block will always be
  // streamed no matter if it's full or not.
  sendBlockFilledMessage(block->getID());
}

MutableBlockReference BlockPoolInsertDestination::createNewBlock() {
  const block_id new_id = storage_manager_->createBlock(relation_, *layout_);

  // Notify Foreman to add the newly created block id in the master Catalog.
  serialization::CatalogRelationNewBlockMessage proto;
  proto.set_relation_id(relation_.getID());
  proto.set_block_id(new_id);
  proto.set_query_id(getQueryID());

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage tagged_msg(static_cast<const void *>(proto_bytes),
                           proto_length,
                           kCatalogRelationNewBlockMessage);
  free(proto_bytes);

  DLOG(INFO) << "BlockPoolInsertDestination sent CatalogRelationNewBlockMessage to Scheduler with Client "
             << scheduler_client_id_;
  const tmb::MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(bus_,
                                         thread_id_map_.getValue(),
                                         scheduler_client_id_,
                                         move(tagged_msg));
  CHECK(send_status == tmb::MessageBus::SendStatus::kOK);

  return storage_manager_->getBlockMutable(new_id, relation_);
}

void BlockPoolInsertDestination::getPartiallyFilledBlocks(std::vector<MutableBlockReference> *partial_blocks,
                                                          vector<partition_id> *part_ids) {
  SpinMutexLock lock(mutex_);
  for (std::vector<MutableBlockReference>::size_type i = 0; i < available_block_refs_.size(); ++i) {
    partial_blocks->push_back((std::move(available_block_refs_[i])));
    // No partition.
    part_ids->push_back(0u);
  }

  available_block_refs_.clear();
}

MutableBlockReference BlockPoolInsertDestination::getBlockForInsertion() {
  SpinMutexLock lock(mutex_);
  if (available_block_refs_.empty()) {
    if (available_block_ids_.empty()) {
      return createNewBlock();
    } else {
      const block_id id = available_block_ids_.back();
      available_block_ids_.pop_back();
      MutableBlockReference retval = storage_manager_->getBlockMutable(id, relation_);
      return retval;
    }
  } else {
    MutableBlockReference retval = std::move(available_block_refs_.back());
    available_block_refs_.pop_back();
    return retval;
  }
}

void BlockPoolInsertDestination::returnBlock(MutableBlockReference &&block, const bool full) {
  {
    SpinMutexLock lock(mutex_);
    if (full) {
      done_block_ids_.push_back(block->getID());
    } else {
      available_block_refs_.push_back(std::move(block));
      return;
    }
  }
  DEBUG_ASSERT(full);
  // If the block is full, rebuild before pipelining it.
  if (!block->rebuild()) {
    LOG_WARNING("Rebuilding of StorageBlock with ID: " << block->getID() <<
                "invalidated one or more IndexSubBlocks.");
  }
  // Note that the block will only be sent if it's full (true).
  sendBlockFilledMessage(block->getID());
}

std::vector<block_id> BlockPoolInsertDestination::getTouchedBlocksInternal() {
  for (std::vector<MutableBlockReference>::size_type i = 0; i < available_block_refs_.size(); ++i) {
    done_block_ids_.push_back(available_block_refs_[i]->getID());
  }
  available_block_refs_.clear();

  return done_block_ids_;
}

PartitionAwareInsertDestination::PartitionAwareInsertDestination(
    PartitionSchemeHeader *partition_scheme_header,
    const CatalogRelationSchema &relation,
    const StorageBlockLayout *layout,
    StorageManager *storage_manager,
    vector<vector<block_id>> &&partitions,
    const std::size_t relational_op_index,
    const std::size_t query_id,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus)
    : InsertDestination(InsertDestinationType::kPartitionAwareInsertDestination,
                        relation,
                        layout,
                        storage_manager,
                        relational_op_index,
                        query_id,
                        scheduler_client_id,
                        bus),
      partition_scheme_header_(DCHECK_NOTNULL(partition_scheme_header)),
      available_block_refs_(partition_scheme_header_->getNumPartitions()),
      available_block_ids_(move(partitions)),
      done_block_ids_(partition_scheme_header_->getNumPartitions()),
      mutexes_for_partition_(
          new SpinMutex[partition_scheme_header_->getNumPartitions()]) {}

MutableBlockReference PartitionAwareInsertDestination::createNewBlock() {
  FATAL_ERROR("PartitionAwareInsertDestination::createNewBlock needs a partition id as an argument.");
}

MutableBlockReference PartitionAwareInsertDestination::createNewBlockInPartition(const partition_id part_id) {
  DCHECK_LT(part_id, partition_scheme_header_->getNumPartitions());
  // Create a new block.
  const block_id new_id = storage_manager_->createBlock(relation_, *layout_);

  // Notify Foreman to add the newly created block id in the master Catalog.
  serialization::CatalogRelationNewBlockMessage proto;
  proto.set_relation_id(relation_.getID());
  proto.set_block_id(new_id);
  proto.set_partition_id(part_id);
  proto.set_query_id(getQueryID());

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage tagged_msg(static_cast<const void *>(proto_bytes),
                           proto_length,
                           kCatalogRelationNewBlockMessage);
  free(proto_bytes);

  DLOG(INFO) << "PartitionAwareInsertDestination sent CatalogRelationNewBlockMessage to Scheduler with Client "
             << scheduler_client_id_;
  const tmb::MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(bus_,
                                         thread_id_map_.getValue(),
                                         scheduler_client_id_,
                                         move(tagged_msg));
  CHECK(send_status == tmb::MessageBus::SendStatus::kOK);

  return storage_manager_->getBlockMutable(new_id, relation_);
}

std::vector<block_id> PartitionAwareInsertDestination::getTouchedBlocksInternal() {
  std::vector<block_id> all_partitions_done_block_ids;
  // Iterate through each partition and get all the touched blocks.
  for (std::size_t part_id = 0;
       part_id < partition_scheme_header_->getNumPartitions();
       ++part_id) {
    for (std::size_t i = 0; i < available_block_refs_[part_id].size(); ++i) {
      done_block_ids_[part_id].push_back(available_block_refs_[part_id][i]->getID());
    }
    available_block_refs_[part_id].clear();

    all_partitions_done_block_ids.insert(
        all_partitions_done_block_ids.end(), done_block_ids_[part_id].begin(), done_block_ids_[part_id].end());
    done_block_ids_[part_id].clear();
  }
  return all_partitions_done_block_ids;
}

PartitionSchemeHeader::PartitionAttributeIds PartitionAwareInsertDestination::getPartitioningAttributes() const {
  return partition_scheme_header_->getPartitionAttributeIds();
}

void PartitionAwareInsertDestination::insertTuple(const Tuple &tuple) {
  const partition_id part_id = getPartitionId(tuple);

  MutableBlockReference output_block = getBlockForInsertionInPartition(part_id);

  try {
    while (!output_block->insertTuple(tuple)) {
      returnBlockInPartition(std::move(output_block), true, part_id);
      output_block = getBlockForInsertionInPartition(part_id);
    }
  }
  catch (...) {
    returnBlockInPartition(std::move(output_block), false, part_id);
    throw;
  }

  returnBlockInPartition(std::move(output_block), false, part_id);
}

void PartitionAwareInsertDestination::insertTupleInBatch(const Tuple &tuple) {
  const partition_id part_id = getPartitionId(tuple);

  MutableBlockReference output_block = getBlockForInsertionInPartition(part_id);

  try {
    while (!output_block->insertTupleInBatch(tuple)) {
      returnBlockInPartition(std::move(output_block), true, part_id);
      output_block = getBlockForInsertionInPartition(part_id);
    }
  }
  catch (...) {
    returnBlockInPartition(std::move(output_block), false, part_id);
    throw;
  }

  returnBlockInPartition(std::move(output_block), false, part_id);
}

void PartitionAwareInsertDestination::bulkInsertTuples(ValueAccessor *accessor, bool always_mark_full) {
  const std::size_t num_partitions = partition_scheme_header_->getNumPartitions();

  InvokeOnAnyValueAccessor(
      accessor,
      [this,
       &always_mark_full,
       &num_partitions](auto *accessor) -> void {  // NOLINT(build/c++11)
    std::vector<std::unique_ptr<TupleIdSequence>> partition_membership(num_partitions);

    // Create a tuple-id sequence for each partition.
    for (std::size_t partition = 0; partition < num_partitions; ++partition) {
      partition_membership[partition] = std::make_unique<TupleIdSequence>(accessor->getEndPosition());
    }

    // Iterate over ValueAccessor for each tuple,
    // set a bit in the appropriate TupleIdSequence.
    accessor->beginIteration();
    this->setPartitionMembership(&partition_membership, accessor);

    // For each partition, create an adapter around Value Accessor and
    // TupleIdSequence.
    std::vector<std::unique_ptr<typename std::remove_pointer<
        decltype(accessor->createSharedTupleIdSequenceAdapter(*partition_membership.front()))>::type>> adapter;
    for (std::size_t partition = 0; partition < num_partitions; ++partition) {
      adapter.emplace_back(accessor->createSharedTupleIdSequenceAdapter(*partition_membership[partition]));
    }

    // Bulk-insert into a block belonging to the partition.
    for (std::size_t partition = 0; partition < num_partitions; ++partition) {
      adapter[partition]->beginIteration();
      while (!adapter[partition]->iterationFinished()) {
        MutableBlockReference output_block = this->getBlockForInsertionInPartition(partition);
        if (output_block->bulkInsertTuples(adapter[partition].get()) == 0) {
          this->returnBlockInPartition(std::move(output_block), true, partition);
        } else {
          // Bulk insert into output_block was successful. output_block
          // will be rebuilt when there won't be any more insertions to it.
          this->returnBlockInPartition(std::move(output_block),
                                       always_mark_full || !adapter[partition]->iterationFinished(),
                                       partition);
        }
      }
    }
  });
}

void PartitionAwareInsertDestination::bulkInsertTuplesWithRemappedAttributes(
    const std::vector<attribute_id> &attribute_map, ValueAccessor *accessor, bool always_mark_full) {
  const std::size_t num_partitions = partition_scheme_header_->getNumPartitions();

  InvokeOnAnyValueAccessor(
      accessor,
      [this,
       &attribute_map,
       &always_mark_full,
       &num_partitions](auto *accessor) -> void {  // NOLINT(build/c++11)
    std::vector<std::unique_ptr<TupleIdSequence>> partition_membership(num_partitions);

    // Create a tuple-id sequence for each partition.
    for (std::size_t partition = 0; partition < num_partitions; ++partition) {
      partition_membership[partition] = std::make_unique<TupleIdSequence>(accessor->getEndPosition());
    }

    // Iterate over ValueAccessor for each tuple,
    // set a bit in the appropriate TupleIdSequence.
    accessor->beginIteration();
    this->setPartitionMembership(&partition_membership, accessor);

    // For each partition, create an adapter around Value Accessor and
    // TupleIdSequence.
    std::vector<std::unique_ptr<typename std::remove_pointer<
        decltype(accessor->createSharedTupleIdSequenceAdapter(*partition_membership.front()))>::type>> adapter;
    for (std::size_t partition = 0; partition < num_partitions; ++partition) {
      adapter.emplace_back(accessor->createSharedTupleIdSequenceAdapter(*partition_membership[partition]));
    }

    // Bulk-insert into a block belonging to the partition.
    for (std::size_t partition = 0; partition < num_partitions; ++partition) {
      adapter[partition]->beginIteration();
      while (!adapter[partition]->iterationFinished()) {
        MutableBlockReference output_block = this->getBlockForInsertionInPartition(partition);
        if (output_block->bulkInsertTuplesWithRemappedAttributes(attribute_map, adapter[partition].get()) == 0) {
          this->returnBlockInPartition(std::move(output_block), true, partition);
        } else {
          // Bulk insert into output_block was successful. output_block
          // will be rebuilt when there won't be any more insertions to it.
          this->returnBlockInPartition(std::move(output_block),
                                       always_mark_full || !adapter[partition]->iterationFinished(),
                                       partition);
        }
      }
    }
  });
}

void PartitionAwareInsertDestination::insertTuplesFromVector(std::vector<Tuple>::const_iterator begin,
                                                             std::vector<Tuple>::const_iterator end) {
  if (begin == end) {
    return;
  }

  for (; begin != end; ++begin) {
    const partition_id part_id = getPartitionId(*begin);

    MutableBlockReference dest_block = getBlockForInsertionInPartition(part_id);
    // FIXME(chasseur): Deal with TupleTooLargeForBlock exception.
    while (!dest_block->insertTupleInBatch(*begin)) {
      returnBlockInPartition(std::move(dest_block), true, part_id);
      dest_block = getBlockForInsertionInPartition(part_id);
    }
    returnBlockInPartition(std::move(dest_block), false, part_id);
  }
}

MutableBlockReference PartitionAwareInsertDestination::getBlockForInsertionInPartition(const partition_id part_id) {
  DCHECK_LT(part_id, partition_scheme_header_->getNumPartitions());
  SpinMutexLock lock(mutexes_for_partition_[part_id]);
  if (available_block_refs_[part_id].empty()) {
    if (available_block_ids_[part_id].empty()) {
      return createNewBlockInPartition(part_id);
    } else {
      const block_id id = available_block_ids_[part_id].back();
      available_block_ids_[part_id].pop_back();
      MutableBlockReference retval = storage_manager_->getBlockMutable(id, relation_);
      return retval;
    }
  } else {
    MutableBlockReference retval = std::move(available_block_refs_[part_id].back());
    available_block_refs_[part_id].pop_back();
    return retval;
  }
}

void PartitionAwareInsertDestination::returnBlock(MutableBlockReference &&block, const bool full) {
  FATAL_ERROR("PartitionAwareInsertDestination::returnBlock needs a partition id as the third argument.");
}

void PartitionAwareInsertDestination::returnBlockInPartition(MutableBlockReference &&block,
                                                             const bool full,
                                                             const partition_id part_id) {
  DCHECK_LT(part_id, partition_scheme_header_->getNumPartitions());
  {
    SpinMutexLock lock(mutexes_for_partition_[part_id]);
    if (full) {
      done_block_ids_[part_id].push_back(block->getID());
    } else {
      available_block_refs_[part_id].push_back(std::move(block));
      return;
    }
  }
  DEBUG_ASSERT(full);
  // If the block is full, rebuild before pipelining it.
  if (!block->rebuild()) {
    LOG_WARNING("Rebuilding of StorageBlock with ID: " << block->getID()
                                                       << "invalidated one or more IndexSubBlocks.");
  }
  // Note that the block will only be sent if it's full (true).
  sendBlockFilledMessage(block->getID(), part_id);
}

}  // namespace quickstep
