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

#ifndef TMB_INTERNAL_RCU_H_
#define TMB_INTERNAL_RCU_H_

#include <atomic>
#include <cassert>
#include <cstddef>
#include <memory>
#include <mutex>  // NOLINT(build/c++11)
#include <utility>

#include "tmb/internal/cache_info.h"

namespace tmb {
namespace internal {

/**
 * @brief An implementation of Read-Copy-Update (a form of multiversion
 *        concurrency control). Any number of readers can access a consistent
 *        read-only snapshot of the underlying data without any locking
 *        whatsoever. A single writer at a time may modify a copy of the data,
 *        then atomically commit its updates. Snapshots are reference-counted
 *        and automatically garbage collected.
 **/
template <typename T>
class RCU {
 private:
  class Version;

 public:
  typedef T value_type;

  // A handle allowing a client to access a read-only snapshot.
  class ReadHandle {
   public:
    // Default constructor creates an invalid ReadHandle.
    inline ReadHandle()
        : version_(nullptr),
          item_cached_(nullptr) {
    }

    // Copy constructor (increments reference count).
    inline ReadHandle(const ReadHandle &orig)
        : version_(orig.version_),
          item_cached_(orig.item_cached_) {
      if (version_ != nullptr) {
        version_->IncrementReferenceCount();
      }
    }

    // Move constructor.
    inline ReadHandle(ReadHandle &&orig)  // NOLINT(build/c++11)
        : version_(orig.version_),
          item_cached_(orig.item_cached_) {
      orig.version_ = nullptr;
      orig.item_cached_ = nullptr;
    }

    // Destructor (decrements reference count).
    inline ~ReadHandle() {
      if (version_ != nullptr) {
        version_->DecrementReferenceCount();
      }
    }

    // Copy-assignment (adjusts reference counts).
    inline ReadHandle& operator=(const ReadHandle &rhs) {
      if (this != &rhs) {
        if (version_ != nullptr) {
          version_->DecrementReferenceCount();
        }
        version_ = rhs.version_;
        item_cached_ = rhs.item_cached_;
        if (version_ != nullptr) {
          version_->IncrementReferenceCount();
        }
      }

      return *this;
    }

    // Move-assignment (adjusts reference counts).
    inline ReadHandle& operator=(ReadHandle &&rhs) {  // NOLINT(build/c++11)
      if (this != &rhs) {
        if (version_ != nullptr) {
          version_->DecrementReferenceCount();
        }
        version_ = rhs.version_;
        item_cached_ = rhs.item_cached_;
        rhs.version_ = nullptr;
        rhs.item_cached_ = nullptr;
      }

      return *this;
    }

    // Check if this handle is valid (points to a readable snapshot).
    inline bool valid() const {
      return (version_ != nullptr);
    }

    // Synonym for valid().
    inline explicit operator bool() const {
      return valid();
    }

    // Read the underlying data.
    inline const T* get() const {
      return item_cached_;
    }

    inline const T& operator*() const {
      assert(valid());
      return *item_cached_;
    }

    inline const T* operator->() const {
      return item_cached_;
    }

    // Invalidate this handle. If the handle will no longer be used, but won't
    // go out of scope for a while, this may allow garbage collection to happen
    // earlier.
    inline void release() {
      if (valid()) {
        version_->DecrementReferenceCount();
        version_ = nullptr;
        item_cached_ = nullptr;
      }
    }

   private:
    inline explicit ReadHandle(Version *version)
        : version_(version),
          item_cached_(version->item()) {
      version_->IncrementReferenceCount();
    }

    Version *version_;
    const T *item_cached_;

    friend class RCU;
  };

  // A handle allowing a client to modify a copy of the data without perturbing
  // readers. A valid WriteHandle implicitly holds a mutex, so it must NOT be
  // shared between threads.
  class WriteHandle {
   public:
    inline WriteHandle()
        : rcu_(nullptr),
          writable_copy_(nullptr) {
    }

    // Move constructor.
    inline WriteHandle(WriteHandle &&orig)  // NOLINT(build/c++11)
        : rcu_(orig.rcu_),
          writable_copy_(std::move(orig.writable_copy_)) {
      orig.rcu_ = nullptr;
    }

    // Destructor. Automatically aborts an uncommitted update.
    inline ~WriteHandle() {
      if (rcu_ != nullptr) {
        rcu_->write_mutex_.unlock();
      }
    }

    // Move-assignment. If this WriteHandle is valid before assignment, any
    // updates will be aborted.
    inline WriteHandle& operator=(WriteHandle &&rhs) {  // NOLINT(build/c++11)
      if (this != &rhs) {
        if (rcu_ != nullptr) {
          rcu_->write_mutex_.unlock();
        }

        rcu_ = rhs.rcu_;
        writable_copy_ = std::move(rhs.writable_copy_);

        rhs.rcu_ = nullptr;
      }

      return *this;
    }

    // Check if this handle is valid (contains a writable data copy).
    inline bool valid() const {
      return (rcu_ != nullptr);
    }

    // Synonym for valid().
    inline explicit operator bool() const {
      return valid();
    }

    // Get a mutable pointer to the underlying data copy.
    inline T* get() const {
      return writable_copy_.get();
    }

    inline T& operator*() const {
      assert(valid());
      return *writable_copy_;
    }

    inline T* operator->() const {
      return writable_copy_.get();
    }

    // Abort any changes and invalidate this WriteHandle.
    inline void Abort() {
      if (rcu_ != nullptr) {
        rcu_->write_mutex_.unlock();
      }
      rcu_ = nullptr;
      writable_copy_.reset(nullptr);
    }

    // Commit changes to the RCU and invalidate this WriteHandle.
    inline void Commit() {
      assert(valid());
      rcu_->CommitNewVersion(writable_copy_.release());
      rcu_ = nullptr;
    }

   private:
    // Must lock 'rcu->write_mutex_' before constructing.
    inline WriteHandle(RCU *rcu, T* writable_copy)
        : rcu_(rcu),
          writable_copy_(writable_copy) {
    }

    RCU *rcu_;
    std::unique_ptr<T> writable_copy_;

    friend class RCU;

    // Disallow copy and assign.
    WriteHandle(const WriteHandle &orig) = delete;
    WriteHandle& operator=(const WriteHandle &rhs) = delete;
  };

  // Constructor. Takes ownership of '*initial_item'.
  explicit RCU(T *initial_item)
      : current_version_(new Version(initial_item, 1)),
        readers_under_construction_(0) {
  }

  // Destructor. Note that any outstanding ReadHandles will actually remain
  // valid after the RCU which created them is destroyed.
  ~RCU() {
    current_version_.load(std::memory_order_relaxed)
        ->DecrementReferenceCount();
  }

  // Get a ReadHandle to the current snapshot. Lock-free and non-blocking.
  inline ReadHandle GetReadHandle() const {
    readers_under_construction_.fetch_add(1, std::memory_order_release);
    ReadHandle retval(current_version_.load(std::memory_order_relaxed));
    readers_under_construction_.fetch_sub(1, std::memory_order_release);
    return std::move(retval);
  }

  // Get a WriteHandle to do updates on the underlying data. Only one
  // outstanding WriteHandle may exist at a time, so this method may block
  // while other writers finish.
  inline WriteHandle GetWriteHandle() {
    write_mutex_.lock();
    return WriteHandle(
        this,
        new T(*(current_version_.load(std::memory_order_relaxed)->item())));
  }

  // Same as GetWriteHandle() above, but instead of copying the underlying
  // value in the RCU, initialize the WriteHandle's copy with the new value
  // specified, taking ownership of '*value'.
  inline WriteHandle GetWriteHandleWithValue(T *value) {
    write_mutex_.lock();
    return WriteHandle(this, value);
  }

 private:
  // A particular read-only version of T, with a reference count.
  class Version {
   public:
    inline Version(T *item, const std::size_t initial_reference_count)
        : item_(item),
          reference_count_(initial_reference_count) {
    }

    inline T* item() const {
      return item_;
    }

    inline void IncrementReferenceCount() {
      reference_count_.fetch_add(1, std::memory_order_relaxed);
    }

    // A Version automatically commits suicide when the reference count reaches
    // zero.
    inline void DecrementReferenceCount() {
      if (reference_count_.fetch_sub(1, std::memory_order_relaxed) == 1) {
        if (item_ != nullptr) {
          delete item_;
        }
        delete this;
      }
    }

   private:
    alignas(kCacheLineBytes) T *item_;
    alignas(kCacheLineBytes) std::atomic<std::size_t> reference_count_;

    // Disallow copy and assign.
    Version(const Version &orig) = delete;
    Version& operator=(const Version &rhs) = delete;
  };

  // Atomically swap in '*new_item' and release the write mutex. The old
  // version of the data will be garbage collected once no ReadHandle
  // references it (which may be immediately).
  inline void CommitNewVersion(T *new_item) {
    Version *new_version = new Version(new_item, 1);
    Version *old_version
        = current_version_.exchange(new_version, std::memory_order_relaxed);

    write_mutex_.unlock();

    // Spin until a moment when there are no ReadHandles actively being
    // constructed. We can then safely decrement the reference count and
    // possibly destroy '*old_version', because we are assured that there
    // is no bare pointer to '*old_version' that is not counted.
    while (readers_under_construction_.load(std::memory_order_acquire) != 0) {
    }
    old_version->DecrementReferenceCount();
  }

  alignas(kCacheLineBytes) std::atomic<Version*> current_version_;
  alignas(kCacheLineBytes) std::mutex write_mutex_;
  alignas(kCacheLineBytes) mutable std::atomic<std::size_t>
      readers_under_construction_;

  // Disallow copy and assign.
  RCU(const RCU &orig) = delete;
  RCU& operator=(const RCU &rhs) = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_RCU_H_
