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

#define __STDC_FORMAT_MACROS

#include "storage/FileManagerPosix.hpp"

#include <fcntl.h>
#include <glob.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <cerrno>
#include <cinttypes>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string>

#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageConstants.hpp"
#include "storage/StorageErrors.hpp"
#include "utility/Macros.hpp"
#include "utility/StringUtil.hpp"

#include "glog/logging.h"

using std::size_t;
using std::sscanf;
using std::strerror;
using std::string;

namespace quickstep {

block_id_counter FileManagerPosix::getMaxUsedBlockCounter(const block_id_domain block_domain) const {
  const string block_domain_str(ToZeroPaddedString(block_domain, kBlockIdDomainLengthInDigits));

  string glob_pattern(storage_path_);
  glob_pattern.append("qsblk_");
  glob_pattern.append(block_domain_str);
  glob_pattern.append("_*.qsb");

  glob_t glob_result;
  glob(glob_pattern.c_str(), 0, nullptr, &glob_result);

  string filename_pattern(storage_path_);
  filename_pattern.append("qsblk_");
  filename_pattern.append(block_domain_str);
  filename_pattern.append("_%");
  filename_pattern.append(SCNu64);
  filename_pattern.append(".qsb");

  block_id_counter counter_max = 0, counter;
  if (glob_result.gl_pathc > 0
      && sscanf(glob_result.gl_pathv[glob_result.gl_pathc - 1], filename_pattern.c_str(), &counter) == 1
      && counter > counter_max) {
    counter_max = counter;
  }

  globfree(&glob_result);
  return counter_max;
}

size_t FileManagerPosix::numSlots(const block_id block) const {
  string filename(blockFilename(block));

  struct stat file_stat;
  if (stat(filename.c_str(), &file_stat) == -1) {
    if (errno != ENOENT) {
      LOG_WARNING("Failed to retrieve info about file " << filename << " with error: " << strerror(errno));
    }
    return 0;
  }

  if ((file_stat.st_size % kSlotSizeBytes) != 0) {
    LOG(ERROR) << "Size of file " << filename.c_str() << " was not integral.";
    throw CorruptPersistentStorage();
  }

  return static_cast<size_t>(file_stat.st_size) / kSlotSizeBytes;
}

bool FileManagerPosix::deleteBlockOrBlob(const block_id block) {
  string filename(blockFilename(block));

  if ((unlink(filename.c_str()) == 0) || (errno == ENOENT)) {
    return true;
  } else {
    LOG_WARNING("Failed to delete file " << filename << " with error: " << strerror(errno));
    return false;
  }
}

bool FileManagerPosix::readBlockOrBlob(const block_id block,
                                       void *buffer,
                                       const std::size_t length) {
  DEBUG_ASSERT(buffer);
  DEBUG_ASSERT(length % kSlotSizeBytes == 0);

  string filename(blockFilename(block));

  int fd = open(filename.c_str(), O_RDONLY);
  if (fd == -1) {
    LOG_WARNING("Failed to open file " << filename << " with error: " << strerror(errno));
    return false;
  }

  size_t bytes_total = 0;
  while (bytes_total < length) {
    ssize_t bytes = read(fd, static_cast<char*>(buffer) + bytes_total, length - bytes_total);
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

  if (close(fd) != 0) {
    LOG_WARNING("Failed to close file " << filename << " with error: " << strerror(errno));
  }

  return (bytes_total == length);
}

bool FileManagerPosix::writeBlockOrBlob(const block_id block,
                                        const void *buffer,
                                        const std::size_t length) {
  DEBUG_ASSERT(buffer);
  DEBUG_ASSERT(length % kSlotSizeBytes == 0);

  string filename(blockFilename(block));

  int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    LOG_WARNING("Failed to open file " << filename << " with error: " << strerror(errno));
    return false;
  }

  size_t bytes_total = 0;
  while (bytes_total < length) {
    ssize_t bytes = write(fd, static_cast<const char*>(buffer) + bytes_total, length - bytes_total);
    if (bytes > 0) {
      bytes_total += bytes;
    } else if (bytes == -1 && errno != EINTR) {
      LOG_WARNING("Failed to write file " << filename << " with error: " << strerror(errno));
      break;
    }
  }

  if (fsync(fd) != 0) {
    LOG_WARNING("Failed to sync file " << filename << " with error: " << strerror(errno));
  }

  if (close(fd) != 0) {
    LOG_WARNING("Failed to close file " << filename << " with error: " << strerror(errno));
  }

  return (bytes_total == length);
}

}  // namespace quickstep
