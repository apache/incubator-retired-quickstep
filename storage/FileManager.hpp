/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_STORAGE_FILE_MANAGER_HPP_
#define QUICKSTEP_STORAGE_FILE_MANAGER_HPP_

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>
#include <utility>

#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"
#include "utility/StringUtil.hpp"

namespace quickstep {

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief A class which manages file I/Os between memory and
 *        the persistent storage.
 **/
class FileManager {
 public:
  /**
   * @brief Constructor.
   *
   * @param storage_path the filesystem directory where blocks have persistent
   *        storage. It is expected to end with a path separator (e.g., '\\'
   *        for Windows or '/' for POSIX systems).
   * @param block_domain The domain of a block id.
   **/
  explicit FileManager(const std::string &storage_path)
      : storage_path_(storage_path) { }

  /**
   * @brief Virtual destructor.
   **/
  virtual ~FileManager() { }

  /**
   * @brief Get a block or blob's relative filename, which uses storage_path_
   *        as a path prefix.
   *
   * @param block The id of the block or blob.
   * @return The relative filename for the given id.
   **/
  virtual std::string blockFilename(const block_id block) const {
    std::string filepath(storage_path_);
    filepath.append("qsblk_");
    filepath.append(ToZeroPaddedString(BlockIdUtil::Domain(block), kBlockIdDomainLengthInDigits));
    filepath.append("_");
    filepath.append(ToZeroPaddedString(BlockIdUtil::Counter(block), kBlockIdCounterLengthInDigits));
    filepath.append(".qsb");

    return filepath;
  }

  /**
   * @brief Get the highest block counter in the persistent storage.
   * @exception CorruptPersistentStorage The storage directory layout is not
   *            in the expected format.
   *
   * @param domain The domain of a block or blob.
   * @return The highest block-id in the persistent storage. Return 0 if no
   *         block files found for the current domain.
   **/
  virtual block_id_counter getMaxUsedBlockCounter(const block_id_domain block_domain) const = 0;

  /**
   * @brief Get the size of a block or blob in slots.
   *
   * @param block The id of the block or blob.
   * @exception CorruptPersistentStorage The storage directory layout is not
   *            in the expected format.
   *
   * @return The number of slots for the given id (0 if it doesn't
   *         exist).
   **/
  virtual std::size_t numSlots(const block_id block) const = 0;

  /**
   * @brief Delete a block or blob's file.
   *
   * @param block The id of the block or blob to delete.
   *
   * @return False if the block exists but is failed to delete. True
   *         if the block is successfully deleted OR is not found.
   **/
  virtual bool deleteBlockOrBlob(const block_id block) = 0;

  /**
   * @brief Read a block or blob's file from the persistent storage.
   *
   * @param block The id of the block or blob to read.
   * @param buffer The data contents of the block or blob to read.
   * @param length The number of bytes to read. It should be multiple of
   *        kSlotSizeBytes.
   *
   * @return False if the block is not found in the persistent storage OR
   *         fails to load. True if the block is successfully loaded to memory.
   **/
  virtual bool readBlockOrBlob(const block_id block, void *buffer, const std::size_t length) = 0;

  /**
   * @brief Write a block or blob in memory to the persistent storage.
   *
   * @param block The id of the block or blob to write.
   * @param buffer The data content of the block or blob to write.
   * @param length The number of bytes to write. It should be multiple of
   *        kSlotSizeBytes.
   *
   * @return False if the block fails to write to the persistent storage. True
   *         if the block is written successfully.
   **/
  virtual bool writeBlockOrBlob(const block_id block, const void *buffer, const std::size_t length) = 0;

 protected:
  // File system path where block files are stored. Fixed when FileManager
  // is created.
  const std::string storage_path_;

 private:
  DISALLOW_COPY_AND_ASSIGN(FileManager);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_FILE_MANAGER_HPP_
