/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_STORAGE_STORAGE_BLOB_HPP_
#define QUICKSTEP_STORAGE_STORAGE_BLOB_HPP_

#include <cstddef>

#include "storage/CountedReference.hpp"
#include "storage/StorageBlockBase.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class StorageManager;

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief A simple "blob" of memory allocated from the StorageManager pool.
 *        Clients use this as simply an array of bytes that happens to reside
 *        in memory managed by StorageManager and can be saved/loaded to disk
 *        by the StorageManager as needed.
 * @note Because StorageBlobs may be relocated or saved/loaded to disk,
 *       permanent "addresses" of data structures inside a StorageBlob should
 *       be stored as offsets relative to the start of the StorageBlob, not as
 *       absolute pointers.
 **/
class StorageBlob : public StorageBlockBase {
 public:
  /**
   * @brief Constructor.
   *
   * @param id The unique ID of this StorageBlob among all blocks and blobs.
   * @param blob_memory The memory slot to use for this StorageBlob's contents.
   * @param blob_memory_size The size of the memory at blob_memory.
   * @param initially_dirty true if the blob should initially be marked dirty
   *        (i.e. for a newly created block that has never been written to
   *        disk), false if block should initially be marked clean (i.e. for a
   *        blob that has been reloaded from disk).
   **/
  StorageBlob(const block_id id,
              void *blob_memory,
              const std::size_t blob_memory_size,
              const bool initially_dirty)
      : StorageBlockBase(id, blob_memory, blob_memory_size) {
    dirty_ = initially_dirty;
  }

  ~StorageBlob() override {
  }

  bool isBlob() const override {
    return true;
  }

  /**
   * @brief Get this blob's size.
   *
   * @return This blob's size in bytes.
   **/
  inline std::size_t size() const {
    return block_memory_size_;
  }

  /**
   * @brief Get a const pointer to this StorageBlob's underlying memory.
   * @warning Do NOT use a const_cast to remove the const-ness of the pointer
   *          returned by this method. Instead, use getMemoryMutable() below.
   *
   * @return A const pointer to this StorageBlob's memory.
   **/
  inline const void* getMemory() const {
    return block_memory_;
  }

  /**
   * @brief Get a mutable pointer to this StorageBlob's underlying memory.
   *
   * @return A mutable pointer to this StorageBlob's memory.
   **/
  inline void* getMemoryMutable() {
    dirty_ = true;
    return block_memory_;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(StorageBlob);
};

/**
 * @brief A managed reference to a mutable blob.
 **/
using MutableBlobReference = CountedReference<StorageBlob>;

/**
 * @brief A managed reference to an immutable blob.
 **/
using BlobReference = CountedReference<const StorageBlob>;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_STORAGE_BLOB_HPP_
