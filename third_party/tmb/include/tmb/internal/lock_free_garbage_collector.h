// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_INTERNAL_LOCK_FREE_GARBAGE_COLLECTOR_H_
#define TMB_INTERNAL_LOCK_FREE_GARBAGE_COLLECTOR_H_

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <limits>
#include <memory>
#include <utility>

#include "tmb/internal/lock_free_stack.h"

namespace tmb {
namespace internal {

/**
 * @brief A fixed-size lock-free hash table with overflow list. Allows
 *        insertion of pointers according to user-supplied keys. When the
 *        LockFreeGarbageCollector is destroyed, all inserted pointers are
 *        also deleted. Inserted items may also be explicitly deleted before
 *        then by calling Delete().
 *
 * @param T The type of objects to garbage-collected.
 * @param num_buckets The number of buckets in the internal lock-free hash
 *        table. A power of two is a good choice, since it allows for faster
 *        arithmetic based on shifts. On the other hand, if there are
 *        congruences or skew in keys, a prime number may be a better choice.
 * @param bucket_size The number of slots for pointers in each bucket. Up to
 *        this many pointers can be stored in the same bucket (because of hash
 *        collisions) before the global overflow list is used.
 **/
template <typename T,
          std::size_t num_buckets = 128,
          std::size_t bucket_size = 4>
class LockFreeGarbageCollector {
 public:
  class DeletionToken {
   public:
    inline DeletionToken() {
      ref_.hash_table_index = std::numeric_limits<std::size_t>::max();
    }

   private:
    inline explicit DeletionToken(const std::size_t hash_table_index) {
      ref_.hash_table_index = (hash_table_index << 1) | 0x1;
    }

    inline explicit DeletionToken(std::unique_ptr<T> *overflow_entry) {
      ref_.overflow_entry = overflow_entry;
    }

    inline bool InHashTable() const {
      return (ref_.hash_table_index & 0x1);
    }

    inline std::size_t HashTableIndex() const {
      return ref_.hash_table_index >> 1;
    }

    union {
      std::size_t hash_table_index;
      std::unique_ptr<T> *overflow_entry;
    } ref_;

    friend class LockFreeGarbageCollector;
  };

  LockFreeGarbageCollector() {
    std::fill(hash_table_, hash_table_ + (num_buckets * bucket_size), nullptr);
  }

  ~LockFreeGarbageCollector() {
    for (const std::atomic<T*> &atom : hash_table_) {
      delete atom.load(std::memory_order_relaxed);
    }
  }

  /**
   * @brief Insert an item into the garbage collector.
   * @warning The same item must not be inserted twice, as this can lead to
   *          double-free errors.
   *
   * @param key The "hash" for the item. This may be generated using a general
   *        purpose hash function like std::hash() or CityHash, or it can be
   *        some application-specific value, or even a random number. The key
   *        is used purely to decide which bucket to store the pointer in, and
   *        since the key is not used for lookups, there is no need for the
   *        same value to always hash to the same key. Instead, it is desirable
   *        (though not strictly necessary) for the space of keys to have a
   *        relatively uniform distribution modulo num_buckets. A good
   *        high-entropy hash function like CityHash will achieve this, as will
   *        a random number generator.
   * @param item A pointer to an item to be garbage collected.
   * @return A token which can be used to explicitly delete item with a call to
   *         Delete().
   **/
  DeletionToken Insert(const std::size_t key, T* item) {
    // First, attempt to atomically insert '*item' into the hash table.
    for (std::size_t internal_key = (key % num_buckets) * bucket_size;
         internal_key < ((key % num_buckets) + 1) * bucket_size;
         ++internal_key) {
      T* buf = nullptr;
      if (hash_table_[internal_key].compare_exchange_strong(
              buf,
              item,
              std::memory_order_relaxed)) {
        return DeletionToken(internal_key);
      }
    }

    // If all the bucket slots are already occupied, put '*item' on the
    // overflow list.
    return DeletionToken(overflow_.Emplace(item));
  }

  /**
   * @brief Explicitly delete a previously inserted item.
   * @note This method allows reclaiming memory early when the caller knows
   *       that the item is no longer needed.
   * @warning The same item must not be deleted twice, as this can cause
   *          memory leaks and double-frees.
   *
   * @param deletion_token The token returned by a call to Insert().
   **/
  void Delete(const DeletionToken &deletion_token) {
    if (deletion_token.InHashTable()) {
      std::size_t hash_table_index = deletion_token.HashTableIndex();
      if (hash_table_index > num_buckets * bucket_size) {
        return;
      }
      T* item = hash_table_[hash_table_index].exchange(
          nullptr,
          std::memory_order_relaxed);
      delete item;
    } else {
      deletion_token.ref_.overflow_entry->reset(nullptr);
      // TODO(chasseur): Also remove node. This is hard to do in a purely
      // lock-free fashion. Hardware transactional memory would help, but it is
      // not yet widely available.
    }
  }

 private:
  std::atomic<T*> hash_table_[num_buckets * bucket_size];
  LockFreeStack<std::unique_ptr<T>> overflow_;

  // Disallow copy and assign.
  LockFreeGarbageCollector(const LockFreeGarbageCollector &orig) = delete;
  LockFreeGarbageCollector& operator=(const LockFreeGarbageCollector &rhs)
      = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_LOCK_FREE_GARBAGE_COLLECTOR_H_
