/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include <algorithm>
#include <array>
#include <chrono>
#include <memory>
#include <thread>  // NOLINT(build/c++11)
#include <unordered_set>

#include "gtest/gtest.h"

#include "storage/EvictionPolicy.hpp"
#include "storage/StorageBlockInfo.hpp"

namespace quickstep {

class EvictionPolicyTest : public ::testing::Test {
 protected:
  enum class Config : unsigned char {
    kUniformRandom = 0,
    kLRU1,
    kLRU2,
    kLRU3,
    kEvictAnyBlock,
    kEnd
  };

  Config current_config_ = Config::kUniformRandom;

  std::unique_ptr<EvictionPolicy> eviction_policy_;

  void initEvictionPolicy() {
    switch (current_config_) {
      case Config::kUniformRandom:
        eviction_policy_.reset(new UniformRandomEvictionPolicy);
        break;
      case Config::kLRU1:
        eviction_policy_.reset(LRUKEvictionPolicyFactory::ConstructLRUKEvictionPolicy(1,
            std::chrono::seconds(100)));
        break;
      case Config::kLRU2:
        eviction_policy_.reset(LRUKEvictionPolicyFactory::ConstructLRUKEvictionPolicy(2,
            std::chrono::seconds(100)));
        break;
      case Config::kLRU3:
        eviction_policy_.reset(LRUKEvictionPolicyFactory::ConstructLRUKEvictionPolicy(3,
            std::chrono::seconds(100)));
        break;
      case Config::kEvictAnyBlock:
        eviction_policy_.reset(new EvictAnyBlockEvictionPolicy);
        break;
      case Config::kEnd:
        ASSERT_TRUE(false);
        break;
    }
  }

 public:
  EvictionPolicyTest() : ::testing::Test() {
    initEvictionPolicy();
  }

 protected:
  bool nextConfig() {
    current_config_ = static_cast<Config>(static_cast<unsigned char>(current_config_) + 1);
    if (current_config_ == Config::kEnd) {
      current_config_ = Config::kUniformRandom;
      return false;
    }
    initEvictionPolicy();
    return true;
  }
};

TEST_F(EvictionPolicyTest, DoesNotEvictReferencedBlocks) {
  EvictionPolicy::Status status;
  block_id block;

  do {
    // Simulate some access patterns
    std::unordered_set<block_id> blocks;
    for (int i = 0; i < 1000; ++i) {
      blocks.insert(BlockIdUtil::GetBlockId(0 /* domain */, i));
    }

    status = eviction_policy_->chooseBlockToEvict(&block);
    ASSERT_EQ(EvictionPolicy::Status::kBlockNotFound, status);

    // Make all blocks known to eviction_policy_
    for (int i = 0; i < 1000; ++i) {
      eviction_policy_->blockReferenced(BlockIdUtil::GetBlockId(0 /* domain */, i));
      eviction_policy_->blockUnreferenced(BlockIdUtil::GetBlockId(0 /* domain */, i));
    }

    // Reference every 10th one
    for (int i = 0; i < 1000; i += 10) {
      eviction_policy_->blockReferenced(BlockIdUtil::GetBlockId(0 /* domain */, i));
    }

    // Make sure they don't get chosen for eviction
    for (int i = 0; i < 900; ++i) {
      status = eviction_policy_->chooseBlockToEvict(&block);
      // Expect that status is okay and that it didn't evict a referenced block
      ASSERT_EQ(EvictionPolicy::Status::kOk, status);
      ASSERT_NE(0u, BlockIdUtil::Counter(block) % 10);
      blocks.erase(block);
    }

    // Make sure that we now always get an error. We just try 10 times.
    for (int i = 0; i < 10; ++i) {
      status = eviction_policy_->chooseBlockToEvict(&block);
      ASSERT_EQ(EvictionPolicy::Status::kBlockNotFound, status);
    }

    // Now unreference and evict the rest of the blocks
    for (int i = 0; i < 1000; i += 10) {
      eviction_policy_->blockUnreferenced(BlockIdUtil::GetBlockId(0 /* domain */, i));
      status = eviction_policy_->chooseBlockToEvict(&block);
      ASSERT_EQ(EvictionPolicy::Status::kOk, status);
      blocks.erase(block);
      ASSERT_EQ(static_cast<block_id_counter>(i), BlockIdUtil::Counter(block));
    }

    // Finally, ensure that EvictionPolicy doesn't believe in any non-existent blocks
    for (int i = 0; i < 10; ++i) {
      status = eviction_policy_->chooseBlockToEvict(&block);
      ASSERT_EQ(EvictionPolicy::Status::kBlockNotFound, status);
    }
  } while (nextConfig());
}

TEST(LRUKEvictionPolicyTest, LRU2IsCorrect) {
  std::array<std::chrono::milliseconds, 3> corrl_reference_periods = {{
      std::chrono::milliseconds(0),
      std::chrono::milliseconds(100),
      std::chrono::milliseconds(300)
  }};

  for (std::chrono::milliseconds corrl_reference_period : corrl_reference_periods) {
    // Create an array of block IDs from [1, 100] and shuffle it.
    std::array<block_id, 100> blocks;
    int n = 1;
    std::generate(blocks.begin(), blocks.end(), [&n]{ return BlockIdUtil::GetBlockId(0 /* domain */, n++); });
    std::random_shuffle(blocks.begin(), blocks.end());

    // Create an LRU-2 EvictionPolicy
    std::unique_ptr<EvictionPolicy> eviction_policy(
        LRUKEvictionPolicyFactory::ConstructLRUKEvictionPolicy(
            2,
            corrl_reference_period));

    // Reference all the blocks
    for (const block_id block : blocks) {
      eviction_policy->blockReferenced(block);
      eviction_policy->blockUnreferenced(block);
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Shuffle them again
    std::array<block_id, 100> shuffled_blocks = blocks;
    std::random_shuffle(shuffled_blocks.begin(), shuffled_blocks.end());

    // Ensure that the next set of references is at least corrl_reference_period after the first set.
    std::this_thread::sleep_for(corrl_reference_period);

    // Reference each block again in a random order
    for (const block_id block : shuffled_blocks) {
      eviction_policy->blockReferenced(block);
      eviction_policy->blockUnreferenced(block);
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Ensure that no block has been referenced in the last corrl_reference_period.
    std::this_thread::sleep_for(corrl_reference_period + std::chrono::milliseconds(500));

    // Assure that the blocks are evicted in descending order
    for (const block_id block : blocks) {
      block_id evicted_block;
      EvictionPolicy::Status status = eviction_policy->chooseBlockToEvict(&evicted_block);
      ASSERT_EQ(EvictionPolicy::Status::kOk, status);
      ASSERT_EQ(block, evicted_block);
    }
  }
}

}  // namespace quickstep
