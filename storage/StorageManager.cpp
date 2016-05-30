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

// This is included before other files so that we can conditionally determine
// what else to include.
#include "catalog/CatalogConfig.h"
#include "storage/StorageConfig.h"

// Define feature test macros to enable large page support for mmap.
#if defined(QUICKSTEP_HAVE_MMAP_LINUX_HUGETLB) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#elif defined(QUICKSTEP_HAVE_MMAP_BSD_SUPERPAGE) && !defined(_BSD_SOURCE)
#define _BSD_SOURCE
#endif

#include "storage/StorageManager.hpp"

#if defined(QUICKSTEP_HAVE_MMAP_LINUX_HUGETLB) \
    || defined(QUICKSTEP_HAVE_MMAP_BSD_SUPERPAGE) \
    || defined(QUICKSTEP_HAVE_MMAP_PLAIN)
#include <sys/mman.h>
#endif

#ifdef QUICKSTEP_HAVE_LIBNUMA
#include <numa.h>
#include <numaif.h>
#endif

#include <atomic>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "storage/CountedReference.hpp"
#include "storage/EvictionPolicy.hpp"
#include "storage/FileManagerLocal.hpp"
#include "storage/StorageBlob.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockBase.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageConstants.hpp"
#include "storage/StorageErrors.hpp"
#include "threading/SpinSharedMutex.hpp"
#include "utility/Alignment.hpp"
#include "utility/CalculateInstalledMemory.hpp"

#ifdef QUICKSTEP_HAVE_FILE_MANAGER_HDFS
#include "storage/FileManagerHdfs.hpp"
#endif

#include "gflags/gflags.h"
#include "glog/logging.h"

using std::free;
using std::int32_t;
using std::memset;
using std::size_t;
using std::string;
using std::vector;

namespace quickstep {

static bool ValidateBlockDomain(const char *flagname,
                                int32_t value) {
  if (value <= 0 || value > static_cast<int32_t>(kMaxDomain)) {
    std::fprintf(stderr, "--%s must be nonzero and no greater than %hu\n", flagname, kMaxDomain);
    return false;
  } else {
    return true;
  }
}
DEFINE_int32(block_domain, 1,
             "The unique domain for a distributed Quickstep instance.");
static const volatile bool block_domain_dummy
    = gflags::RegisterFlagValidator(&FLAGS_block_domain, &ValidateBlockDomain);

/**
 * @brief Set or validate the buffer pool slots. When automatically picking a
 *        default value, check if the system is "small" or "large." Set the
 *        buffer pool space to 70% of the installed main memory for small
 *        and 80% otherwise.
 *        This method follows the signature that is set by the gflags module.
 * @param flagname The name of the buffer pool flag.
 * @param value The value of this flag from the command line, or default (0)
 * @return True if the value was set to a legimate value, false otherwise.
 *         Currently this method aims to always find some legitimate value,
 *         and never returns false.
 **/
static bool SetOrValidateBufferPoolSlots(const char *flagname,
                                         std::uint64_t value) {
  if (value != 0) {
    // TODO(jmp): Check if this value is safe and warn the user if it is not.
    return true;  // User supplied value is > 0 and we simply use that value.
  }

  // Need to automatically pick the buffer pool size.
  std::uint64_t total_memory;
  if (utility::system::calculateTotalMemoryInBytes(&total_memory)) {
    // Detected the total installed memory. Now set the buffer pool size
    //   based on whether the system is large or small.
    if (total_memory/kAGigaByte < kLargeMemorySystemThresholdInGB) {
      // This is a "small" system. Leave a litte more memory for others.
      FLAGS_buffer_pool_slots
          = (total_memory*kPercentageToGrabForSmallSystems)/(kSlotSizeBytes*100);
    } else {
      // This is a "large" system. Grab nearly all of the installed memory.
      FLAGS_buffer_pool_slots
          = (total_memory*kPercentageToGrabForLargeSystems)/(kSlotSizeBytes*100);
    }
    return true;
  }

  // Could not calculate the installed memory. Use a default value of 1k slots.
  LOG(INFO) << "Unable to determine an appropriate buffer pool size. "
            << "Using a default value of 2GB.\n";
  FLAGS_buffer_pool_slots = kDefaultBufferPoolSizeInSlots;
  return true;
}

DEFINE_uint64(buffer_pool_slots, 0,
              "By default the value is 0 and the system automatically sets the "
              "buffer pool size/slots at 70-80% of the total installed memory. "
              "The user can also explicity define the number of slots. "
              "The units for this variable is the number of 2-megabyte slots "
              "that is allocated in the buffer pool. This is a \"soft\" limit: "
              "the buffer pool may temporarily grow larger than this size "
              "if the buffer manager is unable to evict enough unreferenced "
              "blocks to make room for a new allocation.");
static const volatile bool buffer_pool_slots_dummy
    = gflags::RegisterFlagValidator(&FLAGS_buffer_pool_slots, &SetOrValidateBufferPoolSlots);

#ifdef QUICKSTEP_HAVE_FILE_MANAGER_HDFS
DEFINE_bool(use_hdfs, false, "Use HDFS as the persistent storage, instead of the local disk.");
#endif

StorageManager::StorageManager(
  const std::string &storage_path,
  const block_id_domain block_domain,
  const size_t max_memory_usage,
  EvictionPolicy *eviction_policy)
    : storage_path_(storage_path),
      total_memory_usage_(0),
      max_memory_usage_(max_memory_usage),
      eviction_policy_(eviction_policy) {
#ifdef QUICKSTEP_HAVE_FILE_MANAGER_HDFS
  if (FLAGS_use_hdfs) {
    file_manager_.reset(new FileManagerHdfs(storage_path));
  } else {
    file_manager_.reset(new FileManagerLocal(storage_path));
  }
#else
  file_manager_.reset(new FileManagerLocal(storage_path));
#endif

  block_index_ = BlockIdUtil::GetBlockId(block_domain, file_manager_->getMaxUsedBlockCounter(block_domain));
}

StorageManager::~StorageManager() {
  for (std::unordered_map<block_id, BlockHandle>::iterator it = blocks_.begin();
       it != blocks_.end();
       ++it) {
    if (it->second.block->isDirty()) {
      LOG(WARNING) << "Block with ID " << BlockIdUtil::ToString(it->first)
                   << " is dirty during StorageManager shutdown";
    }
    delete it->second.block;
    deallocateSlots(it->second.block_memory, it->second.block_memory_size);
  }
}

block_id StorageManager::createBlock(const CatalogRelationSchema &relation,
                                     const StorageBlockLayout &layout,
                                     const int numa_node) {
  const size_t num_slots = layout.getDescription().num_slots();

  BlockHandle new_block_handle;
  const block_id new_block_id =
      allocateNewBlockOrBlob(num_slots, &new_block_handle, numa_node);

  new_block_handle.block = new StorageBlock(relation,
                                            new_block_id,
                                            layout,
                                            true,
                                            new_block_handle.block_memory,
                                            kSlotSizeBytes * num_slots);

  {
    // Lock 'blocks_shared_mutex_' as briefly as possible to insert an entry
    // for the new block, after the block has already been constructed.
    SpinSharedMutexExclusiveLock<false> write_lock(blocks_shared_mutex_);

    // Because block IDs are generated by atomically incrementing block_index_,
    // there should never be collisions.
    DEBUG_ASSERT(blocks_.find(new_block_id) == blocks_.end());
    blocks_[new_block_id] = new_block_handle;
  }

  // Make '*eviction_policy_' aware of the new block's existence.
  eviction_policy_->blockCreated(new_block_id);

  return new_block_id;
}

block_id StorageManager::createBlob(const std::size_t num_slots,
                                    int numa_node) {
  BlockHandle new_block_handle;
  block_id new_block_id =
      allocateNewBlockOrBlob(num_slots, &new_block_handle, numa_node);
  void *new_block_mem = new_block_handle.block_memory;

  new_block_handle.block = new StorageBlob(
      new_block_id, new_block_mem, kSlotSizeBytes * num_slots, true);

  {
    // Lock 'blocks_shared_mutex_' as briefly as possible to insert an entry
    // for the new block, after the block has already been constructed.
    SpinSharedMutexExclusiveLock<false> write_lock(blocks_shared_mutex_);

    // Because block IDs are generated by atomically incrementing block_index_,
    // there should never be collisions.
    DEBUG_ASSERT(blocks_.find(new_block_id) == blocks_.end());
    blocks_[new_block_id] = new_block_handle;
  }

  // Make '*eviction_policy_' aware of the new blob's existence.
  eviction_policy_->blockCreated(new_block_id);

  return new_block_id;
}

bool StorageManager::blockOrBlobIsLoaded(const block_id block) const {
  SpinSharedMutexSharedLock<false> read_lock(blocks_shared_mutex_);
  if (blocks_.find(block) == blocks_.end()) {
    return false;
  } else {
    return true;
  }
}

StorageBlock* StorageManager::loadBlock(const block_id block,
                                        const CatalogRelationSchema &relation,
                                        const int numa_node) {
  BlockHandle handle = loadBlockOrBlob(block, numa_node);
  handle.block = new StorageBlock(
      relation,
      block,
      StorageBlockLayout(relation),
      false,
      handle.block_memory,
      handle.block_memory_size * kSlotSizeBytes);
  insertBlockHandleAfterLoad(block, handle);
  return static_cast<StorageBlock*>(handle.block);
}

StorageBlob* StorageManager::loadBlob(const block_id blob,
                                      const int numa_node) {
  BlockHandle handle = loadBlockOrBlob(blob, numa_node);
  handle.block = new StorageBlob(blob,
                                 handle.block_memory,
                                 handle.block_memory_size * kSlotSizeBytes,
                                 false);
  insertBlockHandleAfterLoad(blob, handle);
  return static_cast<StorageBlob*>(handle.block);
}

bool StorageManager::saveBlockOrBlob(const block_id block, const bool force) {
  // TODO(chasseur): This lock is held for the entire duration of this call
  // (including I/O), but really we only need to prevent the eviction of the
  // particular entry in 'blocks_' for the specified 'block'. If and when we
  // switch blocks_ to something with more fine-grained locking, this should
  // be revisited.
  SpinSharedMutexSharedLock<false> read_lock(blocks_shared_mutex_);

  std::unordered_map<block_id, BlockHandle>::iterator block_it = blocks_.find(block);
  if (block_it == blocks_.end()) {
    return false;
  }

  if (!(force || block_it->second.block->isDirty())) {
    return true;
  }

  bool res = file_manager_->writeBlockOrBlob(block,
                                             block_it->second.block_memory,
                                             kSlotSizeBytes * (block_it->second.block_memory_size));
  if (res) {
    block_it->second.block->markClean();
  }

  return res;
}

void StorageManager::evictBlockOrBlob(const block_id block) {
  BlockHandle handle;
  {
    SpinSharedMutexExclusiveLock<false> write_lock(blocks_shared_mutex_);

    std::unordered_map<block_id, BlockHandle>::iterator block_it = blocks_.find(block);
    if (block_it == blocks_.end()) {
      throw BlockNotFoundInMemory();
    }
    handle = block_it->second;
    blocks_.erase(block_it);
  }

  delete handle.block;
  deallocateSlots(handle.block_memory, handle.block_memory_size);
}

void StorageManager::deleteBlockOrBlobFile(const block_id block) {
  bool need_to_evict = false;
  {
    SpinSharedMutexSharedLock<false> read_lock(blocks_shared_mutex_);
    if (blocks_.find(block) != blocks_.end()) {
      need_to_evict = true;
    }
  }
  if (need_to_evict) {
    evictBlockOrBlob(block);
    eviction_policy_->blockEvicted(block);
  }

  const bool status = file_manager_->deleteBlockOrBlob(block);
  CHECK(status) << "Failed to delete block from persistent storage: " << block;

  eviction_policy_->blockDeleted(block);
}

block_id StorageManager::allocateNewBlockOrBlob(const std::size_t num_slots,
                                                BlockHandle *handle,
                                                const int numa_node) {
  DEBUG_ASSERT(num_slots > 0);
  DEBUG_ASSERT(handle != nullptr);

  handle->block_memory = allocateSlots(num_slots, numa_node);
  handle->block_memory_size = num_slots;

  return ++block_index_;
}

StorageManager::BlockHandle StorageManager::loadBlockOrBlob(
    const block_id block, const int numa_node) {
  // The caller of this function holds an exclusive lock on this block/blob's
  // mutex in the lock manager. The caller has ensured that the block is not
  // already loaded before this function gets called.
  size_t num_slots = file_manager_->numSlots(block);
  DEBUG_ASSERT(num_slots != 0);
  void *block_buffer = allocateSlots(num_slots, numa_node);

  const bool status = file_manager_->readBlockOrBlob(block, block_buffer, kSlotSizeBytes * num_slots);
  CHECK(status) << "Failed to read block from persistent storage: " << block;

  BlockHandle loaded_handle;
  loaded_handle.block_memory = block_buffer;
  loaded_handle.block_memory_size = num_slots;

  return loaded_handle;
}

void StorageManager::insertBlockHandleAfterLoad(const block_id block,
                                                const BlockHandle &handle) {
  SpinSharedMutexExclusiveLock<false> lock(blocks_shared_mutex_);
  DEBUG_ASSERT(blocks_.find(block) == blocks_.end());
  blocks_[block] = handle;
}

void* StorageManager::allocateSlots(const std::size_t num_slots,
                                    const int numa_node) {
#if defined(QUICKSTEP_HAVE_MMAP_LINUX_HUGETLB)
  static constexpr int kLargePageMmapFlags
      = MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB;
#elif defined(QUICKSTEP_HAVE_MMAP_BSD_SUPERPAGE)
  static constexpr int kLargePageMmapFlags
      = MAP_PRIVATE | MAP_ANONYMOUS | MAP_ALIGNED_SUPER;
#endif

  makeRoomForBlockOrBlob(num_slots);
  void *slots = nullptr;

#if defined(QUICKSTEP_HAVE_MMAP_LINUX_HUGETLB) || defined(QUICKSTEP_HAVE_MMAP_BSD_SUPERPAGE)
  slots = mmap(nullptr,
               num_slots * kSlotSizeBytes,
               PROT_READ | PROT_WRITE,
               kLargePageMmapFlags,
               -1, 0);

  // Fallback to regular mmap() if large page allocation failed. Even on
  // systems with large page support, large page allocation may fail if the
  // user running the executable is not a member of hugetlb_shm_group on Linux,
  // or if all the reserved hugepages are already in use.
  if (slots == MAP_FAILED) {
    slots = mmap(nullptr,
                 num_slots * kSlotSizeBytes,
                 PROT_READ | PROT_WRITE,
                 MAP_PRIVATE | MAP_ANONYMOUS,
                 -1, 0);
  }
  if (slots == MAP_FAILED) {
    slots = nullptr;
  }
#elif defined(QUICKSTEP_HAVE_MMAP_PLAIN)
  slots = mmap(nullptr,
               num_slots * kSlotSizeBytes,
               PROT_READ | PROT_WRITE,
               MAP_PRIVATE | MAP_ANONYMOUS,
               -1, 0);
  if (slots == MAP_FAILED) {
    slots = nullptr;
  }
#else
  slots = malloc_with_alignment(num_slots * kSlotSizeBytes,
                                kCacheLineBytes);
  if (slots != nullptr) {
    memset(slots, 0x0, num_slots * kSlotSizeBytes);
  }
#endif

  if (slots == nullptr) {
    throw OutOfMemory();
  }

#if defined(QUICKSTEP_HAVE_LIBNUMA)
  if (numa_node != -1) {
    DEBUG_ASSERT(numa_node < numa_num_configured_nodes());
    struct bitmask *numa_node_bitmask = numa_allocate_nodemask();
    // numa_node can be 0 through n-1, where n is the num of NUMA nodes.
    numa_bitmask_setbit(numa_node_bitmask, numa_node);
    long mbind_status = mbind(slots,  // NOLINT(runtime/int)
                              num_slots * kSlotSizeBytes,
                              MPOL_PREFERRED,
                              numa_node_bitmask->maskp,
                              numa_node_bitmask->size,
                              0);
    numa_free_nodemask(numa_node_bitmask);
    if (mbind_status == -1) {
      LOG(WARNING) << "mbind() failed with errno " << errno << " ("
                   << std::strerror(errno) << ")";
    }
  }
#endif  // QUICKSTEP_HAVE_LIBNUMA

  total_memory_usage_ += num_slots;
  return slots;
}

void StorageManager::deallocateSlots(void *slots, const std::size_t num_slots) {
#if defined(QUICKSTEP_HAVE_MMAP_LINUX_HUGETLB) \
    || defined(QUICKSTEP_HAVE_MMAP_BSD_SUPERPAGE) \
    || defined(QUICKSTEP_HAVE_MMAP_PLAIN)
  DO_AND_DEBUG_ASSERT_ZERO(munmap(slots, num_slots * kSlotSizeBytes));
#else
  free(slots);
#endif
  total_memory_usage_ -= num_slots;
}

MutableBlockReference StorageManager::getBlockInternal(
    const block_id block,
    const CatalogRelationSchema &relation,
    const int numa_node) {
  MutableBlockReference ret;
  {
    SpinSharedMutexSharedLock<false> eviction_lock(*lock_manager_.get(block));
    SpinSharedMutexSharedLock<false> read_lock(blocks_shared_mutex_);
    std::unordered_map<block_id, BlockHandle>::iterator it = blocks_.find(block);
    if (it != blocks_.end()) {
      DEBUG_ASSERT(!it->second.block->isBlob());
      ret = MutableBlockReference(static_cast<StorageBlock*>(it->second.block), eviction_policy_.get());
    }
  }
  // To be safe, release the block's shard after 'eviction_lock' destructs.
  lock_manager_.release(block);

  if (ret.valid()) {
    return ret;
  }

  // Note that there is no way for the block to be evicted between the call to
  // loadBlock and the call to EvictionPolicy::blockReferenced from
  // MutableBlockReference's constructor; this is because EvictionPolicy
  // doesn't know about the block until blockReferenced is called, so
  // chooseBlockToEvict shouldn't return the block.
  do {
    SpinSharedMutexExclusiveLock<false> io_lock(*lock_manager_.get(block));
    {
      // Check one more time if the block got loaded in memory by someone else.
      SpinSharedMutexSharedLock<false> read_lock(blocks_shared_mutex_);
      std::unordered_map<block_id, BlockHandle>::iterator it = blocks_.find(block);
      if (it != blocks_.end()) {
        DEBUG_ASSERT(!it->second.block->isBlob());
        ret = MutableBlockReference(static_cast<StorageBlock*>(it->second.block), eviction_policy_.get());
        break;
      }
    }
    // No other thread loaded the block before us.
    ret = MutableBlockReference(loadBlock(block, relation, numa_node), eviction_policy_.get());
  } while (false);
  // To be safe, release the block's shard after 'io_lock' destructs.
  lock_manager_.release(block);

  return ret;
}

MutableBlobReference StorageManager::getBlobInternal(const block_id blob,
                                                     const int numa_node) {
  MutableBlobReference ret;
  {
    SpinSharedMutexSharedLock<false> eviction_lock(*lock_manager_.get(blob));
    SpinSharedMutexSharedLock<false> read_lock(blocks_shared_mutex_);
    std::unordered_map<block_id, BlockHandle>::iterator it = blocks_.find(blob);
    if (it != blocks_.end()) {
      DEBUG_ASSERT(it->second.block->isBlob());
      ret = MutableBlobReference(static_cast<StorageBlob*>(it->second.block), eviction_policy_.get());
    }
  }
  // To be safe, release the blob's shard after 'eviction_lock' destructs.
  lock_manager_.release(blob);

  if (ret.valid()) {
    return ret;
  }

  do {
    SpinSharedMutexExclusiveLock<false> io_lock(*lock_manager_.get(blob));
    // Note that there is no way for the block to be evicted between the call to
    // loadBlob and the call to EvictionPolicy::blockReferenced from
    // MutableBlobReference's constructor; this is because EvictionPolicy
    // doesn't know about the blob until blockReferenced is called, so
    // chooseBlockToEvict shouldn't return the blob.
    {
      SpinSharedMutexSharedLock<false> read_lock(blocks_shared_mutex_);
      std::unordered_map<block_id, BlockHandle>::iterator it = blocks_.find(blob);
      if (it != blocks_.end()) {
        DEBUG_ASSERT(it->second.block->isBlob());
        ret = MutableBlobReference(static_cast<StorageBlob*>(it->second.block), eviction_policy_.get());
        break;
      }
    }
    // No other thread loaded the blob before us.
    ret = MutableBlobReference(loadBlob(blob, numa_node), eviction_policy_.get());
  } while (false);
  // To be safe, release the blob's shard after 'io_lock' destructs.
  lock_manager_.release(blob);

  return ret;
}

void StorageManager::makeRoomForBlockOrBlob(const size_t slots) {
  block_id block_to_evict;
  while (total_memory_usage_ + slots > max_memory_usage_) {
    const EvictionPolicy::Status status = eviction_policy_->chooseBlockToEvict(&block_to_evict);
    if (status != EvictionPolicy::Status::kOk) {
      // If status was not ok, then we must not have been able to evict enough
      // blocks; therefore, we return anyway, temporarily going over the memory
      // limit.
      break;
    }

    bool has_collision = false;
    SpinSharedMutexExclusiveLock<false> eviction_lock(*lock_manager_.get(block_to_evict, &has_collision));
    if (has_collision) {
      // We have a collision in the shared lock manager, where some callers
      // of this function (i.e., getBlockInternal or getBlobInternal) has
      // acquired an exclusive lock, and we are trying to evict a block that
      // hashes to the same location. This will cause a deadlock.

      // For now simply treat this situation as the case where there is not
      // enough memory and we temporarily go over the memory limit.
      break;
    }

    {
      SpinSharedMutexSharedLock<false> read_lock(blocks_shared_mutex_);
      if (blocks_.find(block_to_evict) == blocks_.end()) {
        // another thread must have jumped in and evicted it before us

        // NOTE(zuyu): It is ok to release the shard for a block or blob,
        // before 'eviction_lock' destructs, because we will never encounter a
        // self-deadlock in a single thread, and in multiple-thread case some
        // thread will block but not deadlock if there is a shard collision.
        lock_manager_.release(block_to_evict);
        continue;
      }
    }
    if (eviction_policy_->getRefCount(block_to_evict) > 0) {
      // Someone sneaked in and referenced the block before we could evict it.

      // NOTE(zuyu): It is ok to release the shard for a block or blob, before
      // before 'eviction_lock' destructs, because we will never encounter a
      // self-deadlock in a single thread, and in multiple-thread case some
      // thread will block but not deadlock if there is a shard collision.
      lock_manager_.release(block_to_evict);
      continue;
    }
    if (saveBlockOrBlob(block_to_evict)) {
      evictBlockOrBlob(block_to_evict);
    }  // else : Someone sneaked in and evicted the block before we could.

    // NOTE(zuyu): It is ok to release the shard for a block or blob, before
    // before 'eviction_lock' destructs, because we will never encounter a
    // self-deadlock in a single thread, and in multiple-thread case some
    // thread will block but not deadlock if there is a shard collision.
    lock_manager_.release(block_to_evict);
  }
}

bool StorageManager::blockOrBlobIsLoadedAndDirty(const block_id block) {
  SpinSharedMutexSharedLock<false> read_lock(blocks_shared_mutex_);

  std::unordered_map<block_id, BlockHandle>::iterator block_it = blocks_.find(block);
  if (block_it != blocks_.end()) {
    return block_it->second.block->isDirty();
  }
  return false;
}

}  // namespace quickstep
