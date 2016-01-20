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

#include "storage/EvictionPolicy.hpp"

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageConstants.hpp"
#include "threading/Mutex.hpp"
#include "threading/SpinMutex.hpp"
#include "threading/SpinSharedMutex.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::time_point;
using std::chrono::microseconds;

namespace quickstep {

EvictionPolicy::Status EvictAnyBlockEvictionPolicy::chooseBlockToEvict(block_id* block) {
  MutexLock lock(mutex_);
  if (nonreferenced_blocks_.empty()) {
    return Status::kBlockNotFound;
  }
  std::unordered_set<block_id>::const_iterator it = nonreferenced_blocks_.begin();
  *block = *it;
  ref_counts_.erase(*it);
  nonreferenced_blocks_.erase(it);
  return Status::kOk;
}

void EvictAnyBlockEvictionPolicy::blockCreated(const block_id block) {
  MutexLock lock(mutex_);
  ref_counts_[block] = 0;
  nonreferenced_blocks_.insert(block);
}

void EvictAnyBlockEvictionPolicy::blockEvicted(const block_id block) {
  MutexLock lock(mutex_);
  ref_counts_.erase(block);
  nonreferenced_blocks_.erase(block);
}

void EvictAnyBlockEvictionPolicy::blockReferenced(const block_id block) {
  MutexLock lock(mutex_);
  if (ref_counts_[block]++ == 0) {
    // Since the ref count was 0, the block might be in nonreferenced_blocks_
    nonreferenced_blocks_.erase(block);
  }
}

void EvictAnyBlockEvictionPolicy::blockUnreferenced(const block_id block) {
  MutexLock lock(mutex_);
  DEBUG_ASSERT(ref_counts_.find(block) != ref_counts_.end());
  DEBUG_ASSERT(ref_counts_[block] > 0);
  if (--ref_counts_[block] == 0) {
    nonreferenced_blocks_.insert(block);
  }
}

unsigned int EvictAnyBlockEvictionPolicy::getRefCount(const block_id block) {
  MutexLock lock(mutex_);
  DEBUG_ASSERT(ref_counts_.find(block) != ref_counts_.end());
  return ref_counts_[block];
}

EvictionPolicy::Status UniformRandomEvictionPolicy::chooseBlockToEvict(block_id* block) {
  MutexLock lock(mutex_);
  if (nonreferenced_blocks_.empty()) {
    return Status::kBlockNotFound;
  }
  std::uniform_int_distribution<> rng(0, nonreferenced_blocks_.size() - 1);
  std::vector<block_id>::iterator block_it = nonreferenced_blocks_.begin() + rng(gen_);
  *block = *block_it;
  ref_counts_.erase(*block);
  nonreferenced_blocks_.erase(block_it);
  return Status::kOk;
}

void UniformRandomEvictionPolicy::blockCreated(const block_id block) {
  MutexLock lock(mutex_);
  ref_counts_[block] = 0;
  nonreferenced_blocks_.push_back(block);
}

void UniformRandomEvictionPolicy::blockEvicted(const block_id block) {
  MutexLock lock(mutex_);
  if (ref_counts_.erase(block) != 0) {
    // The only way a block could be in nonreferenced_blocks_ is if it were
    // first referenced and then later unreferenced. Thus, we only need to
    // check nonreferenced_blocks_ if it had an entry in ref_counts_.
    // Further, if it had an entry in ref_counts_, it must have an entry in
    // nonreferenced_blocks_, unless it was evicted with a non-zero ref count,
    // which is an error.
    std::vector<block_id>::iterator it = std::find(nonreferenced_blocks_.begin(), nonreferenced_blocks_.end(), block);
    DEBUG_ASSERT(it != nonreferenced_blocks_.end());
    nonreferenced_blocks_.erase(it);
  }
}

void UniformRandomEvictionPolicy::blockReferenced(const block_id block) {
  MutexLock lock(mutex_);
  std::unordered_map<block_id, unsigned int>::iterator it = ref_counts_.find(block);
  if (it == ref_counts_.end()) {
    ref_counts_[block] = 1;
  } else if (++(it->second) == 1) {
    // If we already had a ref count of 0 for the block, then it must be in nonreferenced_blocks_, since that could
    // only happen if the block were referenced and then unreferenced back to 0.
    std::vector<block_id>::iterator it = std::find(nonreferenced_blocks_.begin(), nonreferenced_blocks_.end(), block);
    DEBUG_ASSERT(it != nonreferenced_blocks_.end());
    nonreferenced_blocks_.erase(it);
  }
}

void UniformRandomEvictionPolicy::blockUnreferenced(const block_id block) {
  MutexLock lock(mutex_);
  DEBUG_ASSERT(ref_counts_.find(block) != ref_counts_.end());
  DEBUG_ASSERT(ref_counts_[block] > 0);
  if (--ref_counts_[block] == 0) {
    nonreferenced_blocks_.push_back(block);
  }
}

unsigned int UniformRandomEvictionPolicy::getRefCount(const block_id block) {
  MutexLock lock(mutex_);
  DEBUG_ASSERT(ref_counts_.find(block) != ref_counts_.end());
  return ref_counts_[block];
}

/**
 * @brief This class provides the implementation of the LRU-k eviction
 *        policy. It is a template parameterized on K, the number of references to keep
 *        for each block.
 **/
template<std::size_t K>
class LRUKEvictionPolicyImpl : public EvictionPolicy {
 public:
  explicit LRUKEvictionPolicyImpl(const microseconds correlated_reference_period);
  ~LRUKEvictionPolicyImpl() override { }
  EvictionPolicy::Status chooseBlockToEvict(block_id* block) override;
  void blockCreated(const block_id block) override;
  void blockEvicted(const block_id block) override;
  void blockReferenced(const block_id block) override;
  void blockUnreferenced(const block_id block) override;
  void blockDeleted(const block_id block) override;
  unsigned int getRefCount(const block_id block) override;

 private:
  struct BlockInfo {
    /**
     * Array of the last K references. ref_list_[0] refers to the most recent
     * access, ref_list_[1] to the second most recent, and so on. This only
     * records references that did not take place within the same Correlated
     * Reference Period. These times are drawn from
     * std::chrono::high_resolution_clock.
     **/
    std::array<time_point<high_resolution_clock>, K> ref_list_;

    /**
     * Time of the last reference regardless of whether it occurred within the
     * Correlated Reference Period of the previous reference.
     */
    time_point<high_resolution_clock> last_reference_;

    /**
     * How many references are outstanding on this block.
     */
    std::atomic<unsigned int> ref_count_;

    /**
     * Whether or not this block is currently in memory.
     */
    std::atomic<bool> in_memory_;

    /**
     * A mutex protecting ref_list_ and last_reference_.
     */
    mutable SpinSharedMutex<false> time_mutex_;

    BlockInfo()
        : ref_count_(0), in_memory_(false) {
      ref_list_.fill(time_point<high_resolution_clock>::min());
    }

    /**
     * @brief Increments the reference count for a block, marks the block as
     *        in memory, and records a new reference time.
     *
     * @param correlated_reference_period The correlated reference period of
     *        the parent LRU-K eviction policy.
     * @return True if reference count was zero before this call (meaning the
     *         caller should erase this block from nonreferenced_blocks_).
     **/
    bool blockReferencedHelper(const microseconds correlated_reference_period) {
      time_point<high_resolution_clock> time = high_resolution_clock::now();
      SpinSharedMutexExclusiveLock<false> lock(time_mutex_);
      if (in_memory_.load()) {
        // Block was already in buffer pool.
        if (time - last_reference_ > correlated_reference_period) {
          addReferenceTime(time, true);
        }
      } else {
        // Block was just brought into the buffer pool, possibly after an
        // eviction.
        addReferenceTime(time, false);
        in_memory_.store(true);
      }
      last_reference_ = time;
      return (ref_count_++ == 0);
    }

   private:
    /**
     * @brief Adds a new time as the most recent non-correlated reference. Does
     *        not update last_reference_. Also adjusts the previous reference
     *        times by moving them up
     * @param time The new time to add.
     **/
    inline void addReferenceTime(const time_point<high_resolution_clock> &time,
                                 const bool currently_in_memory) {
      // Move existing reference times back while also adjusting them for the
      // correlated reference period of this block
      if (currently_in_memory) {
        const microseconds corrl_period = duration_cast<microseconds>(last_reference_ - ref_list_[0]);
        for (int i = ref_list_.size() - 1; i > 0; --i) {
          ref_list_[i] = ref_list_[i - 1] + corrl_period;
        }
      } else {
        for (int i = ref_list_.size() - 1; i > 0; --i) {
          ref_list_[i] = ref_list_[i - 1];
        }
      }
      // Now add the new reference to the front
      ref_list_[0] = time;
    }

    DISALLOW_COPY_AND_ASSIGN(BlockInfo);
  };

  /**
   * @brief Compares two BlockInfo objects A and B as follows:
   *          (1) If the time since A's last reference is greater than the correlated reference period and B's is not,
   *              A < B
   *          (2) Otherwise, A < B iff A's Kth most recent reference < B's Kth most recent reference.
   * @return true iff lhs < rhs
   */
  class BlockInfoComparator {
   public:
    const time_point<high_resolution_clock> time_;
    const microseconds                      correlated_reference_period_;

    BlockInfoComparator(
        const time_point<high_resolution_clock> time,
        const microseconds correlated_reference_period)
        : time_(time), correlated_reference_period_(correlated_reference_period) { }

    inline bool operator()(const BlockInfo& lhs, const BlockInfo& rhs) {
      time_point<high_resolution_clock> lhs_last, rhs_last, lhs_next_last, rhs_next_last;
      {
        SpinSharedMutexSharedLock<false> lhs_lock(lhs.time_mutex_);
        lhs_last = lhs.last_reference_;
        lhs_next_last = lhs.ref_list_[K - 1];
      }
      {
        SpinSharedMutexSharedLock<false> rhs_lock(rhs.time_mutex_);
        rhs_last = rhs.last_reference_;
        rhs_next_last = rhs.ref_list_[K - 1];
      }

      bool lhs_past_corrl_period = time_ - lhs_last > correlated_reference_period_;
      bool rhs_past_corrl_period = time_ - rhs_last > correlated_reference_period_;
      if (lhs_past_corrl_period == rhs_past_corrl_period) {
        return lhs_next_last < rhs_next_last;
      } else {
        return lhs_past_corrl_period;
      }
    }
  };

  std::unordered_map<block_id, BlockInfo> blocks_;
  alignas(kCacheLineBytes) SpinSharedMutex<false> blocks_mutex_;
  const microseconds correlated_reference_period_;
  std::unordered_set<block_id> nonreferenced_blocks_;
  SpinMutex nonreferenced_blocks_mutex_;

#ifdef QUICKSTEP_DEBUG
  /**
   * @brief Prints the contents of the LRUKEvictionPolicyImpl. Use this
   *        strictly for debugging purposes. 
   *        WARNING: This method is not safe as it does not acquire
   *                 any mutexes on the internal state variable, so use it 
   *                 primarily for debugging purposes.
   *
   * @param print_map If true, print the eviction map. 
   * @param print_nr_list If true, print the list of non referenced blocks.
   *
   */
  void printState(const bool print_map, const bool print_nr_list) const {
    if (print_map) {
      LOG(INFO) << "Eviction Policy Map (" << blocks_.size() << " entries: ";

      for (const auto& it : blocks_) {
        LOG(INFO) << "[" << it.first                         // print the block number
                  << ", " << it.second.ref_count_ << "] ";   // and the ref count
      }

      LOG(INFO) << "\n";
    }

    if (print_nr_list) {
      // Print the non-referenced block list
      LOG(INFO) << "Non referenced block list is: ";

      for (const block_id nonref_block : nonreferenced_blocks_) {
        LOG(INFO) << nonref_block << "; ";
      }

      LOG(INFO) << "\n";
    }
  }
#endif

  DISALLOW_COPY_AND_ASSIGN(LRUKEvictionPolicyImpl);
};

template<std::size_t K>
LRUKEvictionPolicyImpl<K>::LRUKEvictionPolicyImpl(
    const microseconds correlated_reference_period)
    : correlated_reference_period_(correlated_reference_period) { }

template<std::size_t K>
EvictionPolicy::Status LRUKEvictionPolicyImpl<K>::chooseBlockToEvict(block_id* block) {
  const BlockInfo *smallest_blockinfo = nullptr;
  BlockInfoComparator cmp(high_resolution_clock::now(), correlated_reference_period_);

  SpinSharedMutexSharedLock<false> blocks_read_lock(blocks_mutex_);
  SpinMutexLock nonreferenced_blocks_lock(nonreferenced_blocks_mutex_);
  block_id ret;
  {
    for (const block_id nonref_block : nonreferenced_blocks_) {
      const BlockInfo &blockinfo = blocks_[nonref_block];
      if (blockinfo.in_memory_.load()) {
        if ((smallest_blockinfo == nullptr || cmp(blockinfo, *smallest_blockinfo))) {
          smallest_blockinfo = &blockinfo;
          ret = nonref_block;
        }
      }
    }
  }

  if (smallest_blockinfo == nullptr) {
    return Status::kBlockNotFound;
  } else {
    *block = ret;

#ifdef QUICKSTEP_DEBUG
    if (blocks_[*block].ref_count_ != 0) {
      LOG(INFO) << "Block " << *block
                << " has nonzero reference count. Printing state...\n";
      printState(true, true);
    }
#endif
    DEBUG_ASSERT(blocks_[*block].ref_count_ == 0);
    blocks_[*block].in_memory_.store(false);
    nonreferenced_blocks_.erase(*block);
    return Status::kOk;
  }
}

template<std::size_t K>
void LRUKEvictionPolicyImpl<K>::blockCreated(const block_id block) {
  SpinSharedMutexExclusiveLock<false> blocks_write_lock(blocks_mutex_);
  SpinMutexLock nonreferenced_blocks_lock(nonreferenced_blocks_mutex_);

  // 'blocks_[block]' will be implicitly default-constructed below. Set
  // 'in_memory_' to true initially.
  blocks_[block].in_memory_.store(true);

  // Block is initially nonreferenced.
  nonreferenced_blocks_.insert(block);
}

template<std::size_t K>
void LRUKEvictionPolicyImpl<K>::blockEvicted(const block_id block) {
  SpinSharedMutexSharedLock<false> blocks_read_lock(blocks_mutex_);
  SpinMutexLock nonreferenced_blocks_lock(nonreferenced_blocks_mutex_);
  DEBUG_ASSERT(blocks_[block].ref_count_ == 0);
  blocks_[block].in_memory_.store(false);
  nonreferenced_blocks_.erase(block);
}

template<std::size_t K>
void LRUKEvictionPolicyImpl<K>::blockReferenced(const block_id block) {
  bool block_known = false;
  {
    SpinSharedMutexSharedLock<false> read_lock(blocks_mutex_);
    typename std::unordered_map<block_id, BlockInfo>::iterator it = blocks_.find(block);
    if (it != blocks_.end()) {
      block_known = true;

      // Need to protect the access to "ref_count", so grab the mutex.
      // Note we need to grab this outside the loop as "blockReferencedHelper"
      // changes the "ref_count."
      SpinMutexLock nonreferenced_blocks_lock(nonreferenced_blocks_mutex_);
      if (it->second.blockReferencedHelper(correlated_reference_period_)) {
        nonreferenced_blocks_.erase(block);
      }
    }
  }
  if (!block_known) {
    SpinSharedMutexExclusiveLock<false> blocks_write_lock(blocks_mutex_);

    // As above, need to protect access to ref_count, so grab the mutex.
    // Note we need to grab this outside the loop as "blockReferencedHelper"
    // changes the "ref_count."
    SpinMutexLock nonreferenced_blocks_lock(nonreferenced_blocks_mutex_);
    if (blocks_[block].blockReferencedHelper(correlated_reference_period_)) {
      nonreferenced_blocks_.erase(block);
    }
  }
}

template<std::size_t K>
void LRUKEvictionPolicyImpl<K>::blockUnreferenced(const block_id block) {
  SpinSharedMutexSharedLock<false> read_lock(blocks_mutex_);
  DEBUG_ASSERT(blocks_.find(block) != blocks_.end());
  DEBUG_ASSERT(blocks_[block].ref_count_ > 0);
  BlockInfo& blockinfo = blocks_[block];

  // Need to protect the access to "ref_count", so grab the mutex.
  SpinMutexLock nonreferenced_blocks_lock(nonreferenced_blocks_mutex_);
  if (--blockinfo.ref_count_ == 0) {
    nonreferenced_blocks_.insert(block);
  }
}

template<std::size_t K>
void LRUKEvictionPolicyImpl<K>::blockDeleted(const block_id block) {
  SpinSharedMutexExclusiveLock<false> write_lock(blocks_mutex_);
  SpinMutexLock nonreferenced_blocks_lock(nonreferenced_blocks_mutex_);
  blocks_.erase(block);
  nonreferenced_blocks_.erase(block);
}

template<std::size_t K>
unsigned int LRUKEvictionPolicyImpl<K>::getRefCount(const block_id block) {
  SpinSharedMutexSharedLock<false> read_lock(blocks_mutex_);
  DEBUG_ASSERT(blocks_.find(block) != blocks_.end());
  return blocks_[block].ref_count_.load();
}

EvictionPolicy* LRUKEvictionPolicyFactory::ConstructLRUKEvictionPolicy(
    const size_t k,
    const microseconds correlated_reference_period) {
  switch (k) {
    case 1:
      return new LRUKEvictionPolicyImpl<1>(
        correlated_reference_period);
      break;
    case 2:
      return new LRUKEvictionPolicyImpl<2>(
        correlated_reference_period);
      break;
    case 3:
      return new LRUKEvictionPolicyImpl<3>(
        correlated_reference_period);
      break;
    default:
      FATAL_ERROR("LRU-k only supports k = 1, 2, or 3");
  }
}

}  // namespace quickstep
