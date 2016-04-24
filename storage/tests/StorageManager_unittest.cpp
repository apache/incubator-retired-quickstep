/**
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

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "glog/logging.h"
#include "gtest/gtest.h"

#include "catalog/CatalogConfig.h"
#include "storage/EvictionPolicy.hpp"
#include "storage/StorageBlob.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageConstants.hpp"
#include "storage/StorageManager.hpp"
#include "threading/Thread.hpp"
#include "utility/PtrVector.hpp"
#include "utility/ShardedLockManager.hpp"

#ifdef QUICKSTEP_HAVE_LIBNUMA
#include <numa.h>
#endif

namespace quickstep {

namespace storage_manager_test_internal {

static int client_id = 0;

static const int NUM_BLOBS_PER_CLIENT = 10;
static const int BLOB_SIZE_SLOTS = 1;
static const int CLIENT_CYCLES = 1000;

class StorageClient : public Thread {
 public:
  StorageClient(StorageManager &storage_manager)
      : storage_manager_(storage_manager),
        id_(client_id++) {}

  void run() {
    createBlobs();
    for (int i = 0; i < CLIENT_CYCLES; ++i) {
      cycle();
      if (id_ == 1) {
        std::ostringstream msg_stream;
        msg_stream << "Thread " << id_ << " completed cycle " << i << "\n";
        LOG(INFO) << msg_stream.str();
      }
    }
    deleteBlobs();
  }

 private:
  void createBlobs() {
    for (int i = 0; i < NUM_BLOBS_PER_CLIENT; ++i) {
      block_id new_blob = storage_manager_.createBlob(BLOB_SIZE_SLOTS);
      blobs_.push_back(new_blob);
      blobs_status_[new_blob] = false;
    }
  }

  void cycle() {
    // Shuffle the vector of references and ids.
    std::random_shuffle(blob_refs_.begin(), blob_refs_.end());
    std::random_shuffle(blobs_.begin(), blobs_.end());

    // Release half of the checked out blobs.
    while (blob_refs_.size() > NUM_BLOBS_PER_CLIENT/4) {
      // TODO(marc): Blob may be marked dirty at this point.
      blobs_status_[blob_refs_.back()->getID()] = false;
      blob_refs_.pop_back();
    }

    // Checkout blobs until we reach the threshold.
    int blob_index = 0;
    while (blob_refs_.size() < NUM_BLOBS_PER_CLIENT/2) {
      CHECK(blob_index < blobs_.size());

      // See if we have not checked out this blob. If not, make a reference.
      const block_id candidate_blob = blobs_[blob_index];
      if (!blobs_status_[candidate_blob]) {
        blob_refs_.push_back(storage_manager_.getBlobMutable(candidate_blob));
        blobs_status_[candidate_blob] = true;
      }
      blob_index++;
    }
  }

  void deleteBlobs() {
    // Go through all the blobs and dereference them.
    while(blob_refs_.size() > 0) {
      blobs_status_[blob_refs_.back()->getID()] = false;
      blob_refs_.pop_back();
    }

    // Ensure everything has been checked in.
    for (const auto& id_status : blobs_status_) {
      CHECK(id_status.second == false);
    }

    // Delete all the blob files.
    for (block_id bid : blobs_) {
     storage_manager_.deleteBlockOrBlobFile(bid);
    }
  }

  StorageManager &storage_manager_;

  // Blobs created and owned by this client. 
  std::vector<block_id> blobs_;

  // A value of true means that we have checked out the block.
  std::unordered_map<block_id, bool> blobs_status_;

  // Blob references by this client.
  std::vector<MutableBlobReference> blob_refs_;

  int id_;
};

}  // namespace storage_manager_test_internal

using namespace storage_manager_test_internal;

// Create a large number of threads which concurrently access the StorageManager,
// trying to force a bad interleaving. Test is meant to stress the storage manager
// but does not expose all possible interleavings.
TEST(StorageManagerTest, BruteForceDeadLockTest) {
  // Init StorageManager.
  std::unique_ptr<StorageManager> storage_manager;
  // Use a small number of slots.
  storage_manager.reset(new StorageManager("temp_storage", 32));
  
  // Init some threads.
  const int num_clients = 20;
  PtrVector<StorageClient> clients;
  for (int i = 0; i < num_clients; ++i) {
    clients.push_back(new StorageClient(*storage_manager));
  }

  // Start all threads.
  for (int i = 0; i < num_clients; ++i) {
    clients[i].start();
  }

  // Wait for all threads to finish.
  for (int i = 0; i < num_clients; ++i) {
    clients[i].join();
  }
}

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

}  // namespace quickstep
