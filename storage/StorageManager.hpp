/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_STORAGE_STORAGE_MANAGER_HPP_
#define QUICKSTEP_STORAGE_STORAGE_MANAGER_HPP_

#include <atomic>
#include <chrono>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "storage/CountedReference.hpp"
#include "storage/EvictionPolicy.hpp"
#include "storage/FileManager.hpp"
#include "storage/StorageBlob.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageConfig.h"
#include "storage/StorageConstants.hpp"
#include "threading/SpinSharedMutex.hpp"
#include "utility/Macros.hpp"
#include "utility/ShardedLockManager.hpp"

#include "gflags/gflags.h"
#include "gtest/gtest_prod.h"

namespace quickstep {

DECLARE_int32(block_domain);
DECLARE_uint64(buffer_pool_slots);

#ifdef QUICKSTEP_HAVE_FILE_MANAGER_HDFS
DECLARE_bool(use_hdfs);
#endif

class CatalogRelation;
class CatalogRelationSchema;
class StorageBlockLayout;

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief A class which manages block storage in memory and is responsible for
 *        creating, saving, and loading StorageBlock and StorageBlob instances.
 **/
class StorageManager {
 public:
  /**
   * @brief Constructor.
   * @param storage_path The filesystem directory where blocks have persistent
   *        storage. This should end with a path-separator character.
   * @exception CorruptPersistentStorage The storage directory layout is not
   *            in the expected format.
   */
  explicit StorageManager(const std::string &storage_path)
      : StorageManager(storage_path,
                       FLAGS_block_domain,
                       FLAGS_buffer_pool_slots,
                       LRUKEvictionPolicyFactory::ConstructLRUKEvictionPolicy(
                           2,
                           std::chrono::milliseconds(200))) {
  }

  /**
   * @brief Constructor.
   * @param storage_path The filesystem directory where blocks have persistent
   *        storage.
   * @param max_memory_usage The maximum amount of memory that the storage
   *                         manager should use for cached blocks in slots. If
   *                         an block is requested that is not currently in
   *                         memory and there are already max_memory_usage slots
   *                         in use in memory, then the storage manager will
   *                         attempt to evict enough blocks to make room for the
   *                         requested block; if it cannot evict enough blocks,
   *                         it will fetch the requested block anyway,
   *                         temporarily going over the memory limit.
   * @exception CorruptPersistentStorage The storage directory layout is not
   *            in the expected format.
   **/
  StorageManager(const std::string &storage_path,
                 const size_t max_memory_usage)
      : StorageManager(storage_path,
                       FLAGS_block_domain,
                       max_memory_usage,
                       LRUKEvictionPolicyFactory::ConstructLRUKEvictionPolicy(
                           2,
                           std::chrono::milliseconds(200))) {
  }

  /**
   * @brief Constructor.
   * @param storage_path The filesystem directory where blocks have persistent
   *        storage.
   * @param block_domain The unique block domain.
   * @param max_memory_usage The maximum amount of memory that the storage
   *                         manager should use for cached blocks in slots. If
   *                         an block is requested that is not currently in
   *                         memory and there are already max_memory_usage slots
   *                         in use in memory, then the storage manager will
   *                         attempt to evict enough blocks to make room for the
   *                         requested block; if it cannot evict enough blocks,
   *                         it will fetch the requested block anyway,
   *                         temporarily going over the memory limit.
   * @param eviction_policy The eviction policy that the storage manager should
   *                        use to manage the cache. The storage manager takes
   *                        ownership of *eviction_policy.
   * @exception CorruptPersistentStorage The storage directory layout is not
   *            in the expected format.
   **/
  StorageManager(const std::string &storage_path,
                 const block_id_domain block_domain,
                 const size_t max_memory_usage,
                 EvictionPolicy *eviction_policy);

  /**
   * @brief Destructor which also destroys all managed blocks.
   **/
  ~StorageManager();

  /**
   * @brief Determine the number of slots needed to store the specified number
   *        of bytes.
   * @note The specified number of slots may include some "extra" bytes, modulo
   *       the size of a slot in the memory pool.
   * @note This is mainly intended to help create StorageBlobs of the
   *       appropriate size. StorageBlocks have internal structures and
   *       metadata which require additional storage beyond the "raw" bytes for
   *       tuples.
   *
   * @param bytes The desired number of bytes.
   * @return The number of slots needed to store bytes.
   **/
  static std::size_t SlotsNeededForBytes(const std::size_t bytes) {
    return (bytes + kSlotSizeBytes - 1) / kSlotSizeBytes;
  }

  /**
   * @brief Determine the size of the memory pool managed by this
   *        StorageManager.
   * @note This is provided for informational purposes and provides a snapshot
   *       of the memory pool size at the time it is called. The memory pool
   *       may grow as needed during a call to createBlock() or loadBlock().
   *
   * @return The amount of allocated memory managed by this StorageManager in
   *         bytes.
   **/
  std::size_t getMemorySize() const {
    return kSlotSizeBytes * total_memory_usage_;
  }

  /**
   * @brief Create a new empty block.
   *
   * @param relation The relation which the new block will belong to (you must
   *                 also call addBlock() on the relation).
   * @param layout The StorageBlockLayout to use for the new block. If NULL,
   *               the default layout from relation will be used.
   * @param numa_node The NUMA node on which the block should be created. The
   *                  default value is -1 and it means that the Catalog
   *                  Relation has no NUMAPlacementScheme associated with it
   *                  and hence the block will be created as per the OS policy.
   * @return The id of the newly-created block.
   **/
  block_id createBlock(const CatalogRelation &relation,
                       const StorageBlockLayout *layout,
                       const int numa_node = -1);

  /**
   * @brief Create a new StorageBlob. The blob memory will initially be
   *        zero-filled.
   *
   * @param num_slots The size of the StorageBlob in slots. Must not exceed
   *        kAllocationChunkSizeSlots.
   * @param numa_node The NUMA node on which the blob should be created. The
   *                  default value is -1 and it means that the blob will be
   *                  created as per the default OS policy.
   * @return The id of the newly-created blob.
   **/
  block_id createBlob(const std::size_t num_slots, const int numa_node = -1);

  /**
   * @brief Check whether a StorageBlock or StorageBlob is loaded into memory.
   * @note This is provided for informational purposes and determines if the
   *       specified block is loaded at the moment the method is called. It is
   *       possible for the block to be loaded by a concurrent thread
   *       immediately after this method returns.
   *
   * @param block The id of the block.
   * @return Whether the block with the specified id is in memory.
   **/
  bool blockOrBlobIsLoaded(const block_id block) const;

  /**
   * @brief Save a block or blob in memory to the persistent storage.
   *
   * @param block The id of the block or blob to save.
   * @param force Force the block to the persistent storage, even if it is not
   *        dirty (by default, only actually write dirty blocks to the
   *        persistent storage).
   * @exception UnableToOpenFile The block's persistent storage file couldn't
   *            be opened for writing.
   * @exception FileWriteError An IO error occurred while writing the block's
   *            persistent storage file.
   *
   * @return False if the block is not found in the memory. True if the block is
   *         successfully saved to the persistent storage OR the block is clean
   *         and force is false.
   **/
  bool saveBlockOrBlob(const block_id block, const bool force = false);

  /**
   * @brief Delete a block or blob's file in the persistent storage. The block
   *        is automatically evicted.
   *
   * @param block The id of the block whose file will be deleted.
   **/
  void deleteBlockOrBlobFile(const block_id block);

  /**
   * @brief Get a block. If the block is not in memory it will be loaded.
   *
   * @param block The id of the block to get.
   * @param relation The Catalog Relation this block belongs to.
   * @param numa_node The NUMA node for placing this block. If the block is
   *                  already present in the buffer pool, this is ignored.
   *                  If set to -1, the default OS memory-allocation policy
   *                  will be used.
   * @return The block with the given id.
   * @exception OutOfMemory The system has run out of memory.
   **/
  BlockReference getBlock(const block_id block,
                          const CatalogRelationSchema &relation,
                          const int numa_node = -1) {
    return BlockReference(getBlockInternal(block, relation, numa_node));
  }

  /**
   * @brief Get a mutable pointer to a block. If the block is not in memory it
   *        will be loaded.
   *
   * @param block The id of the block to get.
   * @param relation The Catalog Relation this block belongs to.
   * @param numa_node The NUMA node for placing this block. If the block is
   *                  already present in the buffer pool, this is ignored.
   *                  If set to -1, the default OS memory-allocation policy
   *                  will be used.
   * @return The block with the given id.
   * @exception OutOfMemory The system has run out of memory.
   **/
  MutableBlockReference getBlockMutable(const block_id block,
                                        const CatalogRelationSchema &relation,
                                        const int numa_node = -1) {
    return getBlockInternal(block, relation, numa_node);
  }

  /**
   * @brief Get a blob. If the block is not in memory it will be loaded.
   *
   * @param block The id of the blob to get.
   * @param numa_node The NUMA node for placing this blob. If the blob is
   *                  already present in the buffer pool, this is ignored.
   *                  If set to -1, the default OS memory-allocation policy
   *                  will be used.
   * @return The blob with the given id.
   * @exception OutOfMemory The system has run out of memory.
   **/
  BlobReference getBlob(const block_id blob, const int numa_node = -1) {
    return BlobReference(getBlobInternal(blob, numa_node));
  }

  /**
   * @brief Get a mutable pointer to a blob. If the block is not in memory it
   *        will be loaded.
   *
   * @param block The id of the blob to get.
   * @param numa_node The NUMA node for placing this blob. If the blob is
   *                  already present in the buffer pool, this is ignored.
   *                  If set to -1, the default OS memory-allocation policy
   *                  will be used.
   * @return The blob with the given id.
   * @exception OutOfMemory The system has run out of memory.
   **/
  MutableBlobReference getBlobMutable(const block_id blob,
                                      const int numa_node = -1) {
    return getBlobInternal(blob, numa_node);
  }

  /**
   * @brief Check if a block or blob is loaded in memory AND is dirty.
   *
   * @param block The id of the block or blob.
   * @return True if it's both loaded and dirty, false otherwise.
   **/
  bool blockOrBlobIsLoadedAndDirty(const block_id block);

 private:
  struct BlockHandle {
    void *block_memory;
    std::size_t block_memory_size;  // size of block_memory in slots
    StorageBlockBase *block;
  };

  // Helper for createBlock() and createBlob(). Allocates a block ID and memory
  // slots for a new StorageBlock or StorageBlob. Returns the allocated ID and
  // writes the allocated slot range into 'handle->slot_index_low' and
  // 'handle->slot_index_high'.
  block_id allocateNewBlockOrBlob(const std::size_t num_slots,
                                  BlockHandle *handle,
                                  const int numa_node);

  // Helper for loadBlock() and loadBlob(). Allocates memory slots for the
  // loaded block or blob and reads raw data from the persistent storage into
  // memory. Returns a partially-constructed BlockHandle which has
  // 'slot_index_low' and 'slot_index_high' properly set to indicate the memory
  // slots which raw data has been read into, but for which a StorageBlob or
  // StorageBlock object has not yet been constructed.
  BlockHandle loadBlockOrBlob(const block_id block, const int numa_node);

  // Helper for loadBlock() and loadBlob(). Inserts an entry (block, handle)
  // into 'blocks_'. If there is already an entry in 'blocks_' for 'block',
  // deletes the blob or block in 'handle', frees the slots which it occupied.
  void insertBlockHandleAfterLoad(const block_id block,
                                  const BlockHandle &handle);

  // Allocate a buffer to hold the specified number of slots. The memory
  // returned will be zeroed-out, and mapped using large pages if the system
  // supports it.
  // Note if the last parameter "locked_block_id" is set to something other than
  // "kInvalidBlockId," then it means that the caller has acquired
  // a lock in the sharded lock manager for that block. Thus, if a block needs
  // to be evicted by the EvictionPolicy in the "makeRoomForBlock" call, and
  // if the block to be evicted happens to hash to the same entry in the
  // sharded lock manager, then the Eviction policy needs to pick a different
  // block for eviction.
  // The key point is that if "locked_block_id" is not "kInvalidBlockId," then
  // the caller of allocateSlots, e.g. loadBlock, will have acquired a lock
  // in the sharded lock manager for the block "locked_block_id."
  void* allocateSlots(const std::size_t num_slots,
                      const int numa_node,
                      // const block_id locked_block_id = kInvalidBlockId);
                      const block_id locked_block_id);

  // Deallocate a buffer allocated by allocateSlots(). This must be used
  // instead of free(), because the underlying implementation of
  // allocateSlots() may use mmap instead of malloc.
  void deallocateSlots(void *slots,
                       const std::size_t num_slots);

  /**
   * @brief Evict a block or blob from memory.
   * @note The block is NOT automatically saved, so call saveBlock() first if
   *       necessary.
   *
   * @param block The id of the block to evict.
   * @exception BlockNotFoundInMemory The block with the specified id is not
   *            in memory.
   **/
  void evictBlockOrBlob(const block_id block);

  /**
   * @brief Do the work that is common to getBlock and getBlockMutable, namely,
   *        actually loading the block if necessary.
   * @param block    The block to be acquired.
   * @param relation The relation the block is a part of.
   * @param numa_node The NUMA node where the block is to be loaded.
   * @return A CountedReference to the block.
   * @exception OutOfMemory The system has run out of memory.
   */
  MutableBlockReference getBlockInternal(const block_id block,
                                         const CatalogRelationSchema &relation,
                                         const int numa_node);

  /**
   * @brief Do the work that is common to getBlob and getBlobMutable, namely,
   *        actually loading the block if necessary.
   * @param  blob The blob to be acquired.
   * @param numa_node The NUMA node where the blob is to be loaded.
   * @return A CountedReference to the blob.
   * @exception OutOfMemory The system has run out of memory.
   */
  MutableBlobReference getBlobInternal(const block_id blob,
                                       const int numa_node);

  /**
   * @brief Evict blocks until there is enough space for a new block of the
   *        requested size.
   *
   * @param slots Number of slots to make room for.
   * @param locked_block_id Reference to the block id for which room is being made.
   *                        The parent has a lock in the sharded lock manager for the 
   *                        "locked_block_id,"  so need to pass this through to the 
   *                        EvictionPolicy to avoid a deadlock if the block that is
   *                        being cleared out hashes to the same hash entry.
   */
  void makeRoomForBlock(const size_t slots,
                        const block_id locked_block_id);

  /**
   * @brief Load a block from the persistent storage into memory.
   *
   * @param block The id of the block to load.
   * @param relation The relation which the block belongs to.
   * @param numa_node The NUMA node where the block is to be loaded.
   * @return The StorageBlock that was loaded.
   * @exception BlockNotFoundInPersistentStorage The block with the specified
   *            id can't be found on the persistent storage.
   * @exception CorruptPersistentStorage The storage directory layout is not
   *            in the expected format.
   * @exception OutOfMemory The system has run out of memory.
   * @exception UnableToOpenFile The block's persistent storage file couldn't be
   *            opened for reading.
   **/
  StorageBlock* loadBlock(const block_id block,
                          const CatalogRelationSchema &relation,
                          const int numa_node);

  /**
   * @brief Load a StorageBlob from the persistent storage into memory.
   *
   * @param blob The ID of the blob to load.
   * @param numa_node The NUMA node where the blob is to be loaded.
   *
   * @exception BlockNotFoundInPersistentStorage The block with the specified
   *            id can't be found on the persistent storage.
   * @exception CorruptPersistentStorage The storage directory layout is not
   *            in the expected format.
   * @exception OutOfMemory The system has run out of memory.
   * @exception UnableToOpenFile The block's persistent storage file couldn't be
   *            opened for reading.
   **/
  StorageBlob* loadBlob(const block_id blob, const int numa_node);

  // File system path where block files are stored. Fixed when StorageManager
  // is created.
  const std::string storage_path_;

  // The current memory usage of all storage blocks and blobs in slots.
  std::atomic<size_t> total_memory_usage_;
  // The maximum allowed memory usage of all storage blocks and blobs in slots.
  const size_t max_memory_usage_;

  std::unique_ptr<EvictionPolicy> eviction_policy_;

  std::unique_ptr<FileManager> file_manager_;

  // Used to generate unique IDs in allocateNewBlockOrBlob().
  std::atomic<block_id> block_index_;

  // Directory of in-memory blocks. Read by blockIsLoaded(), saveBlock(),
  // getBlock()/getBlockMutable() and blockOrBlobIsLoadedAndDirty().
  // Modified by createBlock(), loadBlock(), and evictBlock().
  //
  // TODO(chasseur): Look into concurrent map implementations that allow
  // finer-grained locking.
  std::unordered_map<block_id, BlockHandle> blocks_;
  alignas(kCacheLineBytes) mutable SpinSharedMutex<false> blocks_shared_mutex_;

  // This lock manager is used with the following contract:
  //   (1) A block cannot be evicted unless an exclusive lock is held on its
  //       lock shard.
  //   (2) If it is not safe to evict a block, then either that block's
  //       reference count is greater than 0 or a shared lock is held on the
  //       block's lock shard.
  static constexpr std::size_t kLockManagerNumShards = 256;
  ShardedLockManager<block_id, kLockManagerNumShards, SpinSharedMutex<false>> lock_manager_;

  FRIEND_TEST(StorageManagerTest, DifferentNUMANodeBlobTestWithEviction);
  FRIEND_TEST(StorageManagerTest, EvictFromSameShardTest);

  DISALLOW_COPY_AND_ASSIGN(StorageManager);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_STORAGE_MANAGER_HPP_
