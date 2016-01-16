/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_STORAGE_FILE_MANAGER_HDFS_HPP_
#define QUICKSTEP_STORAGE_FILE_MANAGER_HDFS_HPP_

#include <hdfs/hdfs.h>

#include <cstddef>
#include <string>

#include "storage/FileManager.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

#include "gflags/gflags.h"

namespace quickstep {

DECLARE_string(hdfs_namenode_host);
DECLARE_int32(hdfs_namenode_port);
DECLARE_int32(hdfs_num_replications);

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief An implementation of FileManager for HDFS (the Hadoop Distributed
 *        File System) using libhdfs3.
 **/
class FileManagerHdfs : public FileManager {
 public:
  explicit FileManagerHdfs(const std::string &storage_path);

  ~FileManagerHdfs() override;

  bool deleteBlockOrBlob(const block_id block) override;

  bool readBlockOrBlob(const block_id block, void *buffer, const std::size_t length) override;

  bool writeBlockOrBlob(const block_id block, const void *buffer, const std::size_t length) override;

  std::size_t numSlots(const block_id block) const override;

  block_id_counter getMaxUsedBlockCounter(const block_id_domain block_domain) const override;

 private:
  // libhdfs3 has an API to release this pointer.
  hdfsFS hdfs_;

  DISALLOW_COPY_AND_ASSIGN(FileManagerHdfs);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_FILE_MANAGER_HDFS_HPP_
