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
