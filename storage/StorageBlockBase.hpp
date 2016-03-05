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

#ifndef QUICKSTEP_STORAGE_STORAGE_BLOCK_BASE_HPP_
#define QUICKSTEP_STORAGE_STORAGE_BLOCK_BASE_HPP_

#include <atomic>
#include <cstddef>

#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/**
 * @brief Abstract base class which encompasses functionality common to
 *        StorageBlock and StorageBlob.
 **/
class StorageBlockBase {
 public:
  /**
   * @brief Virtual destructor.
   **/
  virtual ~StorageBlockBase() {
#ifdef QUICKSTEP_DEBUG
    CHECK_EQ(0, getRefCount())
        << "Nonzero ref_count_ when deleting block ("
        << BlockIdUtil::Domain(id_) << ", "
        << BlockIdUtil::Counter(id_) << ")";
#endif
  }

  /**
   * @brief Determine if this is a StorageBlob or StorageBlock.
   *
   * @return true if this is a StorageBlob, false if it is a StorageBlock.
   **/
  virtual bool isBlob() const = 0;

  /**
   * @brief Get this block's block_id.
   *
   * @return This block's ID.
   **/
  inline block_id getID() const {
    return id_;
  }

  /**
   * @brief Check whether this block is dirty (whether it has been changed
   *        since being written to disk).
   *
   * @return Whether the block is dirty.
   **/
  inline bool isDirty() const {
    return dirty_;
  }

  /**
   * @brief Clear the dirty bit for this block, marking it as clean.
   **/
  inline void markClean() {
    dirty_ = false;
  }

#ifdef QUICKSTEP_DEBUG
  /**
   * @brief Atomically increment the reference count for this StorageBlockBase.
   **/
  void ref() const {
    ++ref_count_;
  }

  /**
   * @brief Atomically decrement the reference count for this StorageBlockBase.
   **/
  void unref() const {
    CHECK_GE(--ref_count_, 0)
        << "unref() of block ("
        << BlockIdUtil::Domain(id_) << ", "
        << BlockIdUtil::Counter(id_) << ") "
        << "caused reference count to become negative.";
  }

  /**
   * @brief Atomically get the reference count of this block.
   * @return This block's reference count.
   **/
  int getRefCount() const {
    return ref_count_.load();
  }
#endif

 protected:
  StorageBlockBase(const block_id id,
                   void *block_memory,
                   const std::size_t block_memory_size)
      : id_(id),
        dirty_(false),
        block_memory_(block_memory),
        block_memory_size_(block_memory_size) {
#ifdef QUICKSTEP_DEBUG
    ref_count_ = 0;
#endif
  }

  const block_id id_;
  bool dirty_;

  void *block_memory_;
  const std::size_t block_memory_size_;

 private:
#ifdef QUICKSTEP_DEBUG
  mutable std::atomic_int ref_count_;
#endif

  DISALLOW_COPY_AND_ASSIGN(StorageBlockBase);
};

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_STORAGE_BLOCK_BASE_HPP_
