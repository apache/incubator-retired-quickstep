/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

class Type;

InsertDestination::InsertDestination(const CatalogRelationSchema &relation,
                                     const StorageBlockLayout *layout,
                                     StorageManager *storage_manager,
                                     const std::size_t relational_op_index,
                                     const tmb::client_id scheduler_client_id,
                                     tmb::MessageBus *bus)
    : thread_id_map_(*ClientIDMap::Instance()),
      storage_manager_(storage_manager),
      relation_(relation),
      layout_(layout),
      relational_op_index_(relational_op_index),
      scheduler_client_id_(scheduler_client_id),
      bus_(DCHECK_NOTNULL(bus)) {
  if (layout_ == nullptr) {
    layout_.reset(StorageBlockLayout::GenerateDefaultLayout(relation, relation.isVariableLength()));
  }
}

InsertDestination* InsertDestination::ReconstructFromProto(const serialization::InsertDestination &proto,
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

      const serialization::PartitionSchemeHeader &proto_partition_scheme_header = proto_partition_scheme.header();
      const Type &attr_type =
          relation.getAttributeById(proto_partition_scheme_header.partition_attribute_id())->getType();
      return new PartitionAwareInsertDestination(
          PartitionSchemeHeader::ReconstructFromProto(proto_partition_scheme_header, attr_type),
          relation,
          layout,
          storage_manager,
          move(partitions),
          proto.relational_op_index(),
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

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage tagged_msg(static_cast<const void *>(proto_bytes),
                           proto_length,
                           kCatalogRelationNewBlockMessage);
  free(proto_bytes);

  const tmb::MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(bus_,
                                         thread_id_map_.getValue(),
                                         scheduler_client_id_,
                                         move(tagged_msg));
  CHECK(send_status == tmb::MessageBus::SendStatus::kOK)
      << "CatalogRelationNewBlockMessage could not be sent from InsertDestination to Foreman.";

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

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage tagged_msg(static_cast<const void *>(proto_bytes),
                           proto_length,
                           kCatalogRelationNewBlockMessage);
  free(proto_bytes);

  const tmb::MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(bus_,
                                         thread_id_map_.getValue(),
                                         scheduler_client_id_,
                                         move(tagged_msg));
  CHECK(send_status == tmb::MessageBus::SendStatus::kOK)
      << "CatalogRelationNewBlockMessage could not be sent from InsertDestination to Foreman.";

  return storage_manager_->getBlockMutable(new_id, relation_);
}

void BlockPoolInsertDestination::getPartiallyFilledBlocks(std::vector<MutableBlockReference> *partial_blocks) {
  SpinMutexLock lock(mutex_);
  for (auto it = available_block_refs_.begin(); it != available_block_refs_.end(); ++it) {
    for (std::vector<MutableBlockReference>::size_type i = 0; i < it->second.size(); ++i) {
      partial_blocks->push_back((std::move(it->second[i])));
    }
    it->second.clear();
  }
  available_block_refs_.clear();
}

MutableBlockReference BlockPoolInsertDestination::getBlockForInsertion() {
  // Find out the thread which made the request.
  {
    SpinMutexLock lock(mutex_);
    const tmb::client_id worker_thread_client_id = thread_id_map_.getValue();
    if (available_block_refs_.find(worker_thread_client_id) == available_block_refs_.end()) {
      // Entry for the worker not present.
      available_block_refs_[worker_thread_client_id];
      available_block_ids_[worker_thread_client_id];
      return createNewBlock();
    }
    if (available_block_refs_[worker_thread_client_id].empty()) {
      // No available refs
      if (!available_block_ids_[worker_thread_client_id].empty()) {
        // Block IDs are available.
        const block_id id = available_block_ids_[worker_thread_client_id].back();
        available_block_ids_[worker_thread_client_id].pop_back();
        MutableBlockReference retval = storage_manager_->getBlockMutable(id, relation_);
        return retval;
      }
    } else {
      // Some block refs available.
      MutableBlockReference retval = std::move(available_block_refs_[worker_thread_client_id].back());
      available_block_refs_[worker_thread_client_id].pop_back();
      return retval;
    }
  }
  return createNewBlock();
}

void BlockPoolInsertDestination::returnBlock(MutableBlockReference &&block, const bool full) {
  {
    SpinMutexLock lock(mutex_);
    if (full) {
      done_block_ids_.push_back(block->getID());
    } else {
      const tmb::client_id worker_thread_client_id = thread_id_map_.getValue();
      available_block_refs_[worker_thread_client_id].push_back(std::move(block));
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

const std::vector<block_id>& BlockPoolInsertDestination::getTouchedBlocksInternal() {
  for (auto it = available_block_refs_.begin(); it != available_block_refs_.end(); ++it) {
    for (std::vector<MutableBlockReference>::size_type i = 0; i < it->second.size(); ++i) {
      done_block_ids_.push_back((it->second)[i]->getID());
    }
    it->second.clear();
  }
  available_block_refs_.clear();

  return done_block_ids_;
}

PartitionAwareInsertDestination::PartitionAwareInsertDestination(PartitionSchemeHeader *partition_scheme_header,
                                                                 const CatalogRelationSchema &relation,
                                                                 const StorageBlockLayout *layout,
                                                                 StorageManager *storage_manager,
                                                                 vector<vector<block_id>> &&partitions,
                                                                 const std::size_t relational_op_index,
                                                                 const tmb::client_id scheduler_client_id,
                                                                 tmb::MessageBus *bus)
    : InsertDestination(relation, layout, storage_manager, relational_op_index, scheduler_client_id, bus),
      partition_scheme_header_(DCHECK_NOTNULL(partition_scheme_header)),
      available_block_refs_(partition_scheme_header_->getNumPartitions()),
      available_block_ids_(move(partitions)),
      done_block_ids_(partition_scheme_header_->getNumPartitions()),
      mutexes_for_partition_(new SpinMutex[partition_scheme_header_->getNumPartitions()]) {
}

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

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage tagged_msg(static_cast<const void *>(proto_bytes),
                           proto_length,
                           kCatalogRelationNewBlockMessage);
  free(proto_bytes);

  const tmb::MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(bus_,
                                         thread_id_map_.getValue(),
                                         scheduler_client_id_,
                                         move(tagged_msg));
  CHECK(send_status == tmb::MessageBus::SendStatus::kOK)
      << "CatalogRelationNewBlockMessage could not be sent from InsertDestination to Foreman.";

  return storage_manager_->getBlockMutable(new_id, relation_);
}

const std::vector<block_id>& PartitionAwareInsertDestination::getTouchedBlocksInternal() {
  // Iterate through each partition and get all the touched blocks.
  for (std::size_t part_id = 0;
       part_id < partition_scheme_header_->getNumPartitions();
       ++part_id) {
    done_block_ids_[part_id] = getTouchedBlocksInternalInPartition(part_id);
    all_partitions_done_block_ids_.insert(
        all_partitions_done_block_ids_.end(), done_block_ids_[part_id].begin(), done_block_ids_[part_id].end());
    done_block_ids_[part_id].clear();
  }
  return all_partitions_done_block_ids_;
}

const std::vector<block_id>& PartitionAwareInsertDestination::getTouchedBlocksInternalInPartition(
    partition_id part_id) {
  for (std::vector<MutableBlockReference>::size_type i = 0; i < available_block_refs_[part_id].size(); ++i) {
    done_block_ids_[part_id].push_back(available_block_refs_[part_id][i]->getID());
  }
  available_block_refs_[part_id].clear();

  return done_block_ids_[part_id];
}

attribute_id PartitionAwareInsertDestination::getPartitioningAttribute() const {
  return partition_scheme_header_->getPartitionAttributeId();
}

void PartitionAwareInsertDestination::insertTuple(const Tuple &tuple) {
  const partition_id part_id =
      partition_scheme_header_->getPartitionId(
          tuple.getAttributeValue(partition_scheme_header_->getPartitionAttributeId()));

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
  const partition_id part_id =
      partition_scheme_header_->getPartitionId(
          tuple.getAttributeValue(partition_scheme_header_->getPartitionAttributeId()));

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
  const attribute_id partition_attribute_id = partition_scheme_header_->getPartitionAttributeId();

  InvokeOnAnyValueAccessor(
      accessor,
      [this,
       &partition_attribute_id,
       &always_mark_full,
       &num_partitions](auto *accessor) -> void {  // NOLINT(build/c++11)
    std::vector<std::unique_ptr<TupleIdSequence>> partition_membership;
    partition_membership.resize(num_partitions);

    // Create a tuple-id sequence for each partition.
    for (std::size_t partition = 0; partition < num_partitions; ++partition) {
      partition_membership[partition].reset(new TupleIdSequence(accessor->getEndPosition()));
    }

    // Iterate over ValueAccessor for each tuple,
    // set a bit in the appropriate TupleIdSequence.
    accessor->beginIteration();
    while (accessor->next()) {
      TypedValue attr_val = accessor->getTypedValue(partition_attribute_id);
      partition_membership[partition_scheme_header_->getPartitionId(attr_val)]
          ->set(accessor->getCurrentPosition(), true);
    }

    // For each partition, create an adapter around Value Accessor and
    // TupleIdSequence.
    std::vector<std::unique_ptr<typename std::remove_pointer<
        decltype(accessor->createSharedTupleIdSequenceAdapter(*partition_membership.front()))>::type>> adapter;
    adapter.resize(num_partitions);
    for (std::size_t partition = 0; partition < num_partitions; ++partition) {
      adapter[partition].reset(accessor->createSharedTupleIdSequenceAdapter(*partition_membership[partition]));
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
  const attribute_id partition_attribute_id = partition_scheme_header_->getPartitionAttributeId();

  InvokeOnAnyValueAccessor(
      accessor,
      [this,
       &partition_attribute_id,
       &attribute_map,
       &always_mark_full,
       &num_partitions](auto *accessor) -> void {  // NOLINT(build/c++11)
    std::vector<std::unique_ptr<TupleIdSequence>> partition_membership;
    partition_membership.resize(num_partitions);

    // Create a tuple-id sequence for each partition.
    for (std::size_t partition = 0; partition < num_partitions; ++partition) {
      partition_membership[partition].reset(new TupleIdSequence(accessor->getEndPosition()));
    }

    // Iterate over ValueAccessor for each tuple,
    // set a bit in the appropriate TupleIdSequence.
    accessor->beginIteration();
    while (accessor->next()) {
      TypedValue attr_val = accessor->getTypedValue(attribute_map[partition_attribute_id]);
      partition_membership[partition_scheme_header_->getPartitionId(attr_val)]
          ->set(accessor->getCurrentPosition(), true);
    }

    // For each partition, create an adapter around Value Accessor and
    // TupleIdSequence.
    std::vector<std::unique_ptr<typename std::remove_pointer<
        decltype(accessor->createSharedTupleIdSequenceAdapter(*partition_membership.front()))>::type>> adapter;
    adapter.resize(num_partitions);
    for (std::size_t partition = 0; partition < num_partitions; ++partition) {
      adapter[partition].reset(accessor->createSharedTupleIdSequenceAdapter(*partition_membership[partition]));
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

  const attribute_id partition_attribute_id = partition_scheme_header_->getPartitionAttributeId();
  for (; begin != end; ++begin) {
    const partition_id part_id =
        partition_scheme_header_->getPartitionId(begin->getAttributeValue(partition_attribute_id));
    MutableBlockReference dest_block = getBlockForInsertionInPartition(part_id);
    // FIXME(chasseur): Deal with TupleTooLargeForBlock exception.
    while (!dest_block->insertTupleInBatch(*begin)) {
      returnBlockInPartition(std::move(dest_block), true, part_id);
      dest_block = getBlockForInsertionInPartition(part_id);
    }
    returnBlockInPartition(std::move(dest_block), false, part_id);
  }
}

MutableBlockReference PartitionAwareInsertDestination::getBlockForInsertion() {
  FATAL_ERROR("PartitionAwareInsertDestination::getBlockForInsertion needs a partition id as an argument.");
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
  sendBlockFilledMessage(block->getID());
}

}  // namespace quickstep
