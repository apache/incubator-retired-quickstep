/**
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include <atomic>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "catalog/CatalogConfig.h"
#include "storage/EvictionPolicy.hpp"
#include "storage/StorageBlob.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageConstants.hpp"
#include "storage/StorageManager.hpp"
#include "utility/ShardedLockManager.hpp"

#ifdef QUICKSTEP_HAVE_LIBNUMA
#include <numa.h>
#endif

namespace quickstep {

TEST(StorageManagerTest, NUMAAgnosticBlobTest) {
  std::unique_ptr<StorageManager> storage_manager;
  static constexpr std::size_t kNumSlots = 10;
  storage_manager.reset(new StorageManager("temp_storage"));
  // Create kNumSlots * kSlotSizeBytes of memory.
  block_id blob_id = storage_manager->createBlob(kNumSlots);
  MutableBlobReference blob_obj = storage_manager->getBlobMutable(blob_id);
  char *blob_memory = static_cast<char*>(blob_obj->getMemoryMutable());
  // Write some contents into the blob's memory.
  for (std::size_t i = 0; i < kNumSlots * kSlotSizeBytes; ++i) {
    blob_memory[i] = static_cast<char>(i);
  }

  // Dereference the blob.
  blob_obj.release();

  BlobReference new_blob_obj = storage_manager->getBlob(blob_id);
  const char *new_blob_memory = static_cast<const char*>(new_blob_obj->getMemory());
  // Read the contents of the blob and verify if they
  // match with what was written previously.
  for (std::size_t i = 0; i < kNumSlots * kSlotSizeBytes; ++i) {
    EXPECT_EQ(static_cast<char>(i), new_blob_memory[i]);
  }
}

#ifdef QUICKSTEP_HAVE_LIBNUMA
TEST(StorageManagerTest, NUMAAwareBlobTest) {
  std::unique_ptr<StorageManager> storage_manager;
  static constexpr std::size_t kNumSlots = 10;
  storage_manager.reset(new StorageManager("temp_storage"));
  const std::size_t num_numa_nodes = numa_num_configured_nodes();

  block_id blob_id;
  MutableBlobReference blob_obj;
  char* blob_memory;
  BlobReference new_blob_obj;
  const char* new_blob_memory;

  for (std::size_t numa_node = 0; numa_node < num_numa_nodes; ++numa_node) {
    blob_id = storage_manager->createBlob(kNumSlots, numa_node);
    blob_obj =
        storage_manager->getBlobMutable(blob_id, numa_node);
    blob_memory =
        static_cast<char*>(blob_obj->getMemoryMutable());

    // Write some contents into the memory.
    for (std::size_t i = 0; i < kNumSlots * kSlotSizeBytes; ++i) {
      blob_memory[i] = static_cast<char>(i);
    }

    // Dereference the blob.
    blob_obj.release();

    new_blob_obj =
        storage_manager->getBlob(blob_id, numa_node);
    new_blob_memory =
        static_cast<const char*>(new_blob_obj->getMemory());
    // Read the contents of the blob on the same NUMA node on which the blob was
    // created and verify if they match with what we wrote into the blob.
    for (std::size_t i = 0; i < kNumSlots * kSlotSizeBytes; ++i) {
      EXPECT_EQ(static_cast<char>(i), new_blob_memory[i]);
    }
  }
}

TEST(StorageManagerTest, DifferentNUMANodeBlobTest) {
  std::unique_ptr<StorageManager> storage_manager;
  static constexpr std::size_t kNumSlots = 10;
  storage_manager.reset(new StorageManager("temp_storage"));
  const std::size_t num_numa_nodes = numa_num_configured_nodes();

  block_id blob_id;
  MutableBlobReference blob_obj;
  char* blob_memory;
  BlobReference new_blob_obj;
  const char* new_blob_memory;
  std::size_t new_numa_node = 0;

  for (std::size_t numa_node = 0; numa_node < num_numa_nodes; ++numa_node) {
    blob_id = storage_manager->createBlob(kNumSlots, numa_node);
    blob_obj =
        storage_manager->getBlobMutable(blob_id, numa_node);
    blob_memory =
        static_cast<char*>(blob_obj->getMemoryMutable());

    // Write some contents into the memory.
    for (std::size_t i = 0; i < kNumSlots * kSlotSizeBytes; ++i) {
      blob_memory[i] = static_cast<char>(i);
    }

    // Dereference the blob.
    blob_obj.release();

    new_numa_node = (numa_node + 1) % num_numa_nodes;

    new_blob_obj =
        storage_manager->getBlob(blob_id, new_numa_node);
    new_blob_memory =
        static_cast<const char*>(new_blob_obj->getMemory());
    // Read the contents of the blob by giving a different NUMA node hint and
    // verify if we still read the same blob that we actually wrote to.
    for (std::size_t i = 0; i < kNumSlots * kSlotSizeBytes; ++i) {
      EXPECT_EQ(static_cast<char>(i), new_blob_memory[i]);
    }
  }
}

TEST(StorageManagerTest, DifferentNUMANodeBlobTestWithEviction) {
  EvictionPolicy *eviction_policy = LRUKEvictionPolicyFactory::ConstructLRUKEvictionPolicy(
          2, std::chrono::seconds(100));
  EvictionPolicy::Status status;
  static constexpr std::size_t kNumSlots = 10;
  const block_id_domain block_domain = 1000;
  // Set the max_memory_usage to 4 GB.
  const size_t max_memory_usage = 2000;
  std::unique_ptr<StorageManager> storage_manager;
  storage_manager.reset(
      new StorageManager("temp_storage", block_domain, max_memory_usage, eviction_policy));

  const std::size_t num_numa_nodes = numa_num_configured_nodes();

  block_id blob_id;
  MutableBlobReference blob_obj;
  char* blob_memory;
  BlobReference new_blob_obj;
  const char* new_blob_memory;
  std::size_t new_numa_node = 0;

  for (std::size_t numa_node = 0; numa_node < num_numa_nodes; ++numa_node) {
    blob_id = storage_manager->createBlob(kNumSlots, numa_node);
    blob_obj =
        storage_manager->getBlobMutable(blob_id, numa_node);
    blob_memory =
        static_cast<char*>(blob_obj->getMemoryMutable());

    // Write some contents into the memory.
    for (std::size_t i = 0; i < kNumSlots * kSlotSizeBytes; ++i) {
      blob_memory[i] = static_cast<char>(i);
    }

    // Dereference the blob.
    blob_obj.release();

    // Choose a blob for eviction.
    status = storage_manager->eviction_policy_->chooseBlockToEvict(&blob_id);
    ASSERT_EQ(EvictionPolicy::Status::kOk, status);
    // Save the blob to disk.
    storage_manager->saveBlockOrBlob(blob_id, true);
    // Evict the blob from the buffer pool.
    storage_manager->evictBlockOrBlob(blob_id);
    // Inform the eviction policy that this blob has been evicted.
    storage_manager->eviction_policy_->blockEvicted(blob_id);

    new_numa_node = (numa_node + 1) % num_numa_nodes;

    new_blob_obj =
        storage_manager->getBlob(blob_id, new_numa_node);
    new_blob_memory =
        static_cast<const char*>(new_blob_obj->getMemory());

    // Read the contents of the blob by giving a different NUMA node hint and
    // verify if we still read the same blob that we actually wrote to.
    for (std::size_t i = 0; i < kNumSlots * kSlotSizeBytes; ++i) {
      EXPECT_EQ(static_cast<char>(i), new_blob_memory[i]);
    }
  }
}
#endif  // QUICKSTEP_HAVE_LIBNUMA

// Trigger an eviction from the same shard in StorageManager's
// ShardedLockManager while attempting to load a blob. Previously, a bug
// existed that caused a self-deadlock in such situations. This test reproduces
// the issue and validates the fix.
TEST(StorageManagerTest, EvictFromSameShardTest) {
  // Set up a StorageManager with a soft memory limit of only one slot.
  StorageManager storage_manager("eviction_test_storage", 1);

  // Create a blob.
  const block_id blob_a_id = storage_manager.createBlob(1);

  // Blob "a" is now memory-resident in StorageManager, but has a reference
  // count of zero.
  EXPECT_TRUE(storage_manager.blockOrBlobIsLoaded(blob_a_id));
  EXPECT_EQ(kSlotSizeBytes, storage_manager.getMemorySize());

  // Manually alter 'block_index_' inside 'storage_manager' so that the next
  // block_id generated will be in the same shard as 'blob_id_a'.
  storage_manager.block_index_.fetch_add(StorageManager::kLockManagerNumShards - 1);

  // Create another blob and verify that it is in the same shard.
  const block_id blob_b_id = storage_manager.createBlob(1);
  EXPECT_EQ(storage_manager.lock_manager_.get(blob_a_id),
            storage_manager.lock_manager_.get(blob_b_id));

  // Creating a second blob should have triggered an eviction that kicked
  // blob A out.
  EXPECT_FALSE(storage_manager.blockOrBlobIsLoaded(blob_a_id));
  EXPECT_TRUE(storage_manager.blockOrBlobIsLoaded(blob_b_id));
  EXPECT_EQ(kSlotSizeBytes, storage_manager.getMemorySize());

  // Try and get a reference to blob A. Blob A must be reloaded from disk.
  // This will trigger an eviction of blob B. This is the point where the
  // self-deadlock bug could be observed.
  BlobReference blob_a_ref = storage_manager.getBlob(blob_a_id);

  // Reaching this point means we have not self-deadlocked. Now clean up.
  blob_a_ref.release();
  storage_manager.deleteBlockOrBlobFile(blob_a_id);
  storage_manager.deleteBlockOrBlobFile(blob_b_id);
}

}  // namespace quickstep
