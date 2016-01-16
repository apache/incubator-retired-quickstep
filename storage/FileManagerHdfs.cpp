/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#define __STDC_FORMAT_MACROS

#include "storage/FileManagerHdfs.hpp"

#include <hdfs/hdfs.h>

#include <cerrno>
#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageConstants.hpp"
#include "storage/StorageErrors.hpp"
#include "utility/Macros.hpp"
#include "utility/StringUtil.hpp"

#include "gflags/gflags.h"

using std::size_t;
using std::sscanf;
using std::strerror;
using std::string;

namespace quickstep {

DEFINE_string(hdfs_namenode_host, "localhost", "Host name or IP address of HDFS namenode.");

static bool ValidateHdfsNameNodePort(const char *flagname,
                                     std::int32_t value) {
  if (value > 0 && value < 65536) {
    return true;
  } else {
    std::fprintf(stderr, "--%s must be between 1 and 65535 (inclusive)\n", flagname);
    return false;
  }
}
DEFINE_int32(hdfs_namenode_port, 9000, "Port of HDFS namenode.");
static const bool hdfs_namenode_port_dummy
    = gflags::RegisterFlagValidator(&FLAGS_hdfs_namenode_port, &ValidateHdfsNameNodePort);

static bool ValidateHdfsNumReplications(const char *flagname,
                                        std::int32_t value) {
  // The default dfs.namenode.replication.min is 1, while the default
  // dfs.replication.max is 512.
  if (value > 0) {
    return true;
  } else {
    std::fprintf(stderr, "--%s must be at least 1\n", flagname);
    return false;
  }
}
DEFINE_int32(hdfs_num_replications, 1, "Number of HDFS file replications.");
static const bool hdfs_num_replications_dummy
    = gflags::RegisterFlagValidator(&FLAGS_hdfs_num_replications, &ValidateHdfsNumReplications);

FileManagerHdfs::FileManagerHdfs(const string &storage_path)
    : FileManager(storage_path) {
  DEBUG_ASSERT(hdfs_namenode_port_dummy);
  DEBUG_ASSERT(hdfs_num_replications_dummy);

  struct hdfsBuilder *builder = hdfsNewBuilder();
  hdfsBuilderSetNameNode(builder, FLAGS_hdfs_namenode_host.c_str());
  hdfsBuilderSetNameNodePort(builder, FLAGS_hdfs_namenode_port);
  // hdfsBuilderConnect releases builder.
  hdfs_ = hdfsBuilderConnect(builder);
  DEBUG_ASSERT(hdfs_ != nullptr);
}

FileManagerHdfs::~FileManagerHdfs() {
  int status = hdfsDisconnect(hdfs_);
  DEBUG_ASSERT(status == 0);
}

block_id_counter FileManagerHdfs::getMaxUsedBlockCounter(const block_id_domain block_domain) const {
  int num_files = 0;
  hdfsFileInfo *file_infos = hdfsListDirectory(hdfs_, storage_path_.c_str(), &num_files);
  if (file_infos == nullptr) {
    if (errno != ENOENT) {
      LOG_WARNING("Failed to list file info with error: " << strerror(errno));
    }
    return 0;
  }

  string filename_pattern("/qsblk_");
  filename_pattern.append(ToZeroPaddedString(block_domain, kBlockIdDomainLengthInDigits));
  filename_pattern.append("_%");
  filename_pattern.append(SCNu64);
  filename_pattern.append(".qsb");

  block_id_counter counter_max = 0, counter;
  for (int i = 0; i < num_files; ++i) {
    // NOTE(zuyu): mName looks like
    // "/user/<username>/<storage_path_>/qsblk_<block_domain>_[0-9]*.qsb".
    const char *filename = std::strrchr(file_infos[i].mName, '/');
    if (filename != nullptr
        && sscanf(filename, filename_pattern.c_str(), &counter) == 1
        && counter > counter_max) {
      counter_max = counter;
    }
  }

  hdfsFreeFileInfo(file_infos, num_files);

  return counter_max;
}

size_t FileManagerHdfs::numSlots(const block_id block) const {
  string filename(blockFilename(block));

  hdfsFileInfo *file_info = hdfsGetPathInfo(hdfs_, filename.c_str());
  if (file_info == nullptr) {
    if (errno != ENOENT) {
      LOG_WARNING("Failed to get size of file " << filename << " with error: " << strerror(errno));
    }
    return 0;
  }

  size_t file_size = file_info->mSize;
  hdfsFreeFileInfo(file_info, 1);

  if ((file_size % kSlotSizeBytes) != 0) {
    throw CorruptPersistentStorage();
  }

  return file_size / kSlotSizeBytes;
}

bool FileManagerHdfs::deleteBlockOrBlob(const block_id block) {
  string filename(blockFilename(block));

  if ((hdfsDelete(hdfs_, filename.c_str(), 0) == 0) || (errno == ENOENT)) {
    return true;
  } else {
    LOG_WARNING("Failed to delete file " << filename << " with error: " << strerror(errno));
    return false;
  }
}

bool FileManagerHdfs::readBlockOrBlob(const block_id block,
                                      void *buffer,
                                      const size_t length) {
  DEBUG_ASSERT(buffer);
  DEBUG_ASSERT(length % kSlotSizeBytes == 0);

  string filename(blockFilename(block));

  hdfsFile file_handle = hdfsOpenFile(hdfs_,
                                      filename.c_str(),
                                      O_RDONLY,
                                      kSlotSizeBytes,
                                      FLAGS_hdfs_num_replications,
                                      kSlotSizeBytes);
  if (file_handle == nullptr) {
    LOG_WARNING("Failed to open file " << filename << " with error: " << strerror(errno));
    return false;
  }

  size_t bytes_total = 0;
  while (bytes_total < length) {
    tSize bytes = hdfsRead(hdfs_, file_handle, static_cast<char*>(buffer) + bytes_total, length - bytes_total);
    if (bytes > 0) {
      bytes_total += bytes;
    } else if (bytes == -1) {
      if (errno != EINTR) {
        LOG_WARNING("Failed to read file " << filename << " with error: " << strerror(errno));
        break;
      }
    } else {
      LOG_WARNING("Failed to read file " << filename << " since EOF was reached unexpectedly");
      break;
    }
  }

  if (hdfsCloseFile(hdfs_, file_handle) != 0) {
    LOG_WARNING("Failed to close file " << filename << " with error: " << strerror(errno));
  }

  return (bytes_total == length);
}

bool FileManagerHdfs::writeBlockOrBlob(const block_id block,
                                       const void *buffer,
                                       const size_t length) {
  DEBUG_ASSERT(buffer);
  DEBUG_ASSERT(length % kSlotSizeBytes == 0);

  string filename(blockFilename(block));

  hdfsFile file_handle = hdfsOpenFile(hdfs_,
                                      filename.c_str(),
                                      O_WRONLY,
                                      kSlotSizeBytes,
                                      FLAGS_hdfs_num_replications,
                                      kSlotSizeBytes);
  if (file_handle == nullptr) {
    LOG_WARNING("Failed to open file " << filename << " with error: " << strerror(errno));
    return false;
  }

  size_t bytes_total = 0;
  while (bytes_total < length) {
    tSize bytes = hdfsWrite(hdfs_, file_handle, static_cast<const char*>(buffer) + bytes_total, length - bytes_total);
    if (bytes > 0) {
      bytes_total += bytes;
    } else if (bytes == -1) {
      LOG_WARNING("Failed to write file " << filename << " with error: " << strerror(errno));
      break;
    }
  }

  if (hdfsSync(hdfs_, file_handle) != 0) {
    LOG_WARNING("Failed to sync file " << filename << " with error: " << strerror(errno));
  }

  if (hdfsCloseFile(hdfs_, file_handle) != 0) {
    LOG_WARNING("Failed to close file " << filename << " with error: " << strerror(errno));
  }

  return (bytes_total == length);
}

}  // namespace quickstep
