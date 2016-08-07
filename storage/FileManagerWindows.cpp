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

#define __STDC_FORMAT_MACROS

#include "storage/FileManagerWindows.hpp"

#define WIN32_LEAN_AND_MEAN
#include <io.h>
#include <stdio.h>
#include <windows.h>

#include <cerrno>
#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageConstants.hpp"
#include "storage/StorageErrors.hpp"
#include "utility/StringUtil.hpp"

#include "glog/logging.h"

using std::size_t;
using std::strerror;
using std::string;
using std::uint64_t;

namespace quickstep {

block_id_counter FileManagerWindows::getMaxUsedBlockCounter(const block_id_domain block_domain) const {
  const string block_domain_str(ToZeroPaddedString(block_domain, kBlockIdDomainLengthInDigits));

  string glob_pattern(storage_path_);
  glob_pattern.append("qsblk_");
  glob_pattern.append(block_domain_str);
  glob_pattern.append("_*.qsb");

  WIN32_FIND_DATA find_data;
  HANDLE find_handle = INVALID_HANDLE_VALUE;
  DWORD error_code = 0;

  find_handle = FindFirstFile(glob_pattern.c_str(), &find_data);
  if (find_handle == INVALID_HANDLE_VALUE) {
    error_code = GetLastError();
    if (error_code != ERROR_FILE_NOT_FOUND) {
      LOG(ERROR) << "Failed to retrieve blockfiles with error_code: " << error_code;
    }
    return 0;
  }

  string filename_pattern("qsblk_");
  filename_pattern.append(block_domain_str);
  filename_pattern.append("_%");
  filename_pattern.append(SCNu64);
  filename_pattern.append(".qsb");

  block_id_counter counter_max = 0, counter;
  do {
    if (sscanf(find_data.cFileName, filename_pattern.c_str(), &counter) == 1 &&
        counter > counter_max) {
      counter_max = counter;
    }
  } while (FindNextFile(find_handle, &find_data) != 0);

  error_code = GetLastError();
  if (error_code != ERROR_NO_MORE_FILES) {
    LOG(ERROR) << "Failed to FindNextFile with error_code: " << error_code;
  }

  if (FindClose(find_handle) == 0) {
    LOG(ERROR) << "Failed to close the file with error_code: " << GetLastError();
  }

  return counter_max;
}

size_t FileManagerWindows::numSlots(const block_id block) const {
  const string filename(blockFilename(block));
  WIN32_FILE_ATTRIBUTE_DATA file_stat;

  if (!GetFileAttributesEx(filename.c_str(), GetFileExInfoStandard, &file_stat)) {
    DWORD error_code = GetLastError();
    if (error_code != ERROR_FILE_NOT_FOUND) {
      LOG(ERROR) << "Failed to retrieve info about file " << filename << " with error_code: " << error_code;
    }
    return 0;
  }

  uint64_t file_size = (static_cast<uint64_t>(file_stat.nFileSizeHigh) << 32) | file_stat.nFileSizeLow;

  if ((file_size % kSlotSizeBytes) != 0) {
    throw CorruptPersistentStorage();
  }

  return static_cast<size_t>(file_size / kSlotSizeBytes);
}

bool FileManagerWindows::deleteBlockOrBlob(const block_id block) {
  const string filename(blockFilename(block));

  if ((DeleteFile(filename.c_str()) != 0) || (GetLastError() == ERROR_FILE_NOT_FOUND)) {
    return true;
  } else {
    LOG(ERROR) << "Failed to delete file " << filename << " with error_code: " << GetLastError();
  }
}

bool FileManagerWindows::readBlockOrBlob(const block_id block,
                                         void *buffer,
                                         const size_t length) {
  DCHECK(buffer != nullptr);
  DCHECK_EQ(0u, length % kSlotSizeBytes);

  const string filename(blockFilename(block));

  FILE *file = fopen(filename.c_str(), "rb");
  if (file == nullptr) {
    // Note: On most, but not all, library implementations, the errno variable
    //       is set to a system-specific error code on failure.
    LOG(ERROR) << "Failed to open file " << filename << " with error: " << strerror(errno);
    return false;
  }

  const size_t bytes = std::fread(buffer, sizeof(char), length, file);
  const bool result_is_ok = (bytes == length);
  if (!result_is_ok) {
    if (std::feof(file)) {
      LOG(ERROR) << "Failed to read file " << filename << " since EOF was reached unexpectedly";
    } else {
      LOG(ERROR) << "Failed to read file " << filename << " with error: " << strerror(ferror(file));
      clearerr(file);
    }
  }

  if (fclose(file)) {
    // Note: fclose does not set errno on failure.
    LOG(ERROR) << "Failed to close file " << filename;
  }

  return result_is_ok;
}

bool FileManagerWindows::writeBlockOrBlob(const block_id block,
                                          const void *buffer,
                                          const size_t length) {
  DCHECK(buffer != nullptr);
  DCHECK_EQ(0u, length % kSlotSizeBytes);

  const string filename(blockFilename(block));

  FILE *file = fopen(filename.c_str(), "wb");
  if (file == nullptr) {
    // Note: On most, but not all, library implementations, the errno variable
    //       is set to a system-specific error code on failure.
    LOG(ERROR) << "Failed to open file " << filename << " with error: " << strerror(errno);
    return false;
  }

  const size_t bytes = std::fwrite(buffer, sizeof(char), length, file);
  const bool result_is_ok = (bytes == length);
  if (!result_is_ok) {
    LOG(ERROR) << "Failed to write file " << filename << " with error: " << strerror(ferror(file));
    clearerr(file);
  }

  if (fflush(file)) {
    LOG(ERROR) << "Failed to flush file " << filename << " with error: " << strerror(ferror(file));
  }

  if (!FlushFileBuffers(reinterpret_cast<HANDLE>(_get_osfhandle(_fileno(file))))) {
    LOG(ERROR) << "Failed to re-flush file " << filename << " with error: " << strerror(ferror(file));
  }

  if (fclose(file)) {
    // Note: fclose does not set errno on failure.
    LOG(ERROR) << "Failed to close file " << filename;
  }

  return result_is_ok;
}

}  // namespace quickstep
