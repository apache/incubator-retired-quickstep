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

#ifndef QUICKSTEP_STORAGE_EVICTION_POLICY_HPP_
#define QUICKSTEP_STORAGE_EVICTION_POLICY_HPP_

#include <chrono>
#include <cstddef>
#include <memory>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "storage/StorageBlockInfo.hpp"
#include "threading/Mutex.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class EvictionPolicy {
 public:
  enum class Status {
    kOk,
    kBlockNotFound
  };

  EvictionPolicy() { }

  virtual ~EvictionPolicy() { }

  /**
   * @brief Choose a block to evict. The policy is responsible for only choosing
   *        a block that is not referenced. The policy will assume that the
   *        block this function returns is eventually evicted.
   *
   * @param block Where the ID of the block should be stored.
   * @return Status::kOk if there was success; Status::kNoKnownBlocks if the
   *                     EvictionPolicy knows of no unreferenced blocks.
   **/
  virtual Status chooseBlockToEvict(block_id* block) = 0;

  /**
   * @brief Notify the EvictionPolicy that a block has been initially created
   *        in the buffer pool, but currently has a reference count of zero.
   *
   * @note We do not simply rely on blockReferenced() to inform the
   *       EvictionPolicy of a newly-created block on first reference, because
   *       it may be some time between the call to
   *       StorageManager::createBlock() or StorageManager::createBlob() and
   *       the first call to StorageManager::getBlock() (or similar) for the
   *       block. During this period, the block is memory-resident and taking
   *       up buffer pool space but it would be effectively invisible to the
   *       EvictionPolicy and ineligible for eviction. Thus, this method is
   *       called by StorageManager::createBlock() and
   *       StorageManager::createBlob() to inform the EvictionPolicy of a new
   *       block's existence before it is ever referenced.
   *
   * @param block The block that has been newly created.
   **/
  virtual void blockCreated(const block_id block) { }

  /**
   * @brief Notify the EvictionPolicy that a block has been evicted even though
   *        it wasn't returned by chooseBlockToEvict. By default, this is a
   *        no-op.
   *
   * @param block The block that has been evicted.
   **/
  virtual void blockEvicted(const block_id block) { }

  /**
   * @brief Notify the EvictionPolicy that a block has been referenced. By
   *        default, this is a no-op. It is called each time the block's
   *        reference count is incremented.
   *
   * @param block The block that has been referenced.
   **/
  virtual void blockReferenced(const block_id block) { }

  /**
   * @brief Notify the EvictionPolicy that a block has been unreferenced. By
   *        default, this is a no-op. It is called each time the block's
   *        reference count is decremented.
   * @param block The block that has been unreferenced.
   */
  virtual void blockUnreferenced(const block_id block) { }

  /**
   * @brief Notify the EvictionPolicy that a block has been deleted. By default,
   *        this is a no-op. This is useful because some eviction policies may
   *        keep information about a block even after it has been evicted. If a
   *        block is evicted and then deleted, you should still call
   *        blockEvicted before calling blockDeleted.
   * @param block The block that has been deleted.
   */
  virtual void blockDeleted(const block_id block) { }

  /**
   * @brief Get the reference count of block.
   *
   * @param block The block whose reference count to get.
   * @return The block's reference count.'
   **/
  virtual unsigned int getRefCount(const block_id block) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(EvictionPolicy);
};

/**
 * @brief Simply evicts the first block it finds that is unreferenced.
 **/
class EvictAnyBlockEvictionPolicy : public EvictionPolicy {
 public:
  EvictAnyBlockEvictionPolicy() { }
  ~EvictAnyBlockEvictionPolicy() override { }
  Status chooseBlockToEvict(block_id* block) override;
  void blockCreated(const block_id block) override;
  void blockEvicted(const block_id block) override;
  void blockReferenced(const block_id block) override;
  void blockUnreferenced(const block_id block) override;
  unsigned int getRefCount(const block_id block) override;

 private:
  // Maintain a reference count for each block
  std::unordered_map<block_id, unsigned int> ref_counts_;

  // For performance, keep track of all known non-referenced blocks
  std::unordered_set<block_id> nonreferenced_blocks_;

  // We just use a single mutex for simplicity. This class isn't really meant to
  // be used except in testing, so performance is not a goal.
  Mutex mutex_;

  DISALLOW_COPY_AND_ASSIGN(EvictAnyBlockEvictionPolicy);
};

/**
 * @brief Evicts non-referenced blocks with uniform probability.
 **/
class UniformRandomEvictionPolicy : public EvictionPolicy {
 public:
  UniformRandomEvictionPolicy() : gen_(rd_()) { }
  ~UniformRandomEvictionPolicy() override { }
  Status chooseBlockToEvict(block_id* block) override;
  void blockCreated(const block_id block) override;
  void blockEvicted(const block_id block) override;
  void blockReferenced(const block_id block) override;
  void blockUnreferenced(const block_id block) override;
  unsigned int getRefCount(const block_id block) override;

 private:
  // Maintain a reference count for each block
  std::unordered_map<block_id, unsigned int> ref_counts_;

  // Maintain a list of non-referenced blocks to simplify victim selection
  std::vector<block_id> nonreferenced_blocks_;

  // Used to randomize victim selection
  std::random_device rd_;
  std::default_random_engine gen_;

  // We just use a single mutex for simplicity. This class isn't really meant to
  // be used except in testing, so performance is not a goal.
  Mutex mutex_;

  DISALLOW_COPY_AND_ASSIGN(UniformRandomEvictionPolicy);
};

/**
 * @brief LRU-k eviction policy. Only supports k = 1, 2, or 3. If k is not one
 *        of these three options, it will be set to 2.
 **/
class LRUKEvictionPolicyFactory {
 public:
  /**
   * @brief Construct an LRU-k policy with the specified parameters.
   * @param k The policy will keep track of the last k references for each known
   *          block.
   * @param correlated_reference_period How far apart should two references be
   *                                    before they are considered to
   *                                    uncorrelated.
   */
  static EvictionPolicy* ConstructLRUKEvictionPolicy(
      const std::size_t k,
      const std::chrono::microseconds correlated_reference_period);

 private:
  DISALLOW_COPY_AND_ASSIGN(LRUKEvictionPolicyFactory);
};

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_EVICTION_POLICY_HPP_
