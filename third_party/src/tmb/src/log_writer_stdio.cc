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

#include "tmb/internal/log_writer_stdio.h"

// Include stdio.h instead of cstdio, because on windows _fileno() only shows
// up in the former.
#include <stdio.h>

#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <mutex>  // NOLINT(build/c++11)
#include <string>

#include "tmb/internal/crc32.h"
#include "tmb/internal/log_record_header.h"
#include "tmb/internal/logging_constants.h"

#if defined(TMB_POSIX_FDATASYNC_SUPPORTED)
#include <unistd.h>
namespace {
inline bool SyncFile(FILE *file) {
  if (fflush(file) != 0) {
    return false;
  }

  return (fdatasync(fileno(file)) == 0);
}
}  // namespace

#elif defined(TMB_POSIX_FSYNC_SUPPORTED)
#include <unistd.h>
namespace {
inline bool SyncFile(FILE *file) {
  if (fflush(file) != 0) {
    return false;
  }

  return (fsync(fileno(file)) == 0);
}
}  // namespace

#elif defined(TMB_WINDOWS_FLUSHFILEBUFFERS_SUPPORTED)
#define WIN32_LEAN_AND_MEAN
#include <io.h>
#include <windows.h>
namespace {
inline bool SyncFile(FILE *file) {
  if (fflush(file) != 0) {
    return false;
  }

  return FlushFileBuffers(
      reinterpret_cast<HANDLE>(_get_osfhandle(_fileno(file))));
}
}  // namespace

#else
namespace {
inline bool SyncFile(FILE *file) {
  return (fflush(file) == 0);
}
}  // namespace

#endif

namespace tmb {
namespace internal {

bool LogWriterStdio::OpenReset() {
  file_ = fopen(filename_.c_str(), "wb");
  return (file_ != nullptr);
}

bool LogWriterStdio::OpenAtPosition(const std::size_t offset) {
  file_ = fopen(filename_.c_str(), "rb+");
  if (file_ == nullptr) {
    return false;
  }

  int status = fseek(file_, offset, SEEK_SET);
  if (status != 0) {
    return false;
  }

  const long int set_pos = ftell(file_);  // NOLINT(runtime/int)
  if (set_pos != offset) {
    return false;
  }

  return true;
}

bool LogWriterStdio::Close() {
  if (!IsOpen()) {
    return true;
  }

  const int status = fclose(file_);
  file_ = nullptr;
  return (status == 0);
}

bool LogWriterStdio::WriteRecord(const void *data,
                                 const std::size_t length,
                                 const bool sync) {
  assert(IsOpen());
  assert(data != nullptr);
  assert(length != 0);

  if (length > kMaxLogRecordSize) {
    return false;
  }

  LogRecordHeader header;
  header.length = length;
  header.crc32 = ComputeCrc32(data, length);

  {
    std::lock_guard<std::mutex> lock(write_mutex_);
    std::size_t written = fwrite(&header, sizeof(header), 1, file_);
    if (written != 1) {
      return false;
    }
    written = fwrite(data, length, 1, file_);
    if (written != 1) {
      return false;
    }
    if (sync) {
      if (!SyncFile(file_)) {
        return false;
      }
    }
  }

  return true;
}

bool LogWriterStdio::WriteMultipartRecord(
    std::initializer_list<std::pair<const void*, std::size_t>> parts,
    const bool sync) {
  assert(IsOpen());

  LogRecordHeader header;
  header.length = 0;
  header.crc32 = kInitialCrc32;

  for (const std::pair<const void*, std::size_t> &part : parts) {
    assert(part.first != nullptr);
    assert(part.second != 0);

    header.length += part.second;
    header.crc32 = UpdateCrc32(header.crc32, part.first, part.second);
  }
  header.crc32 = FinalizeCrc32(header.crc32);

  if (header.length > kMaxLogRecordSize) {
    return false;
  }

  {
    std::lock_guard<std::mutex> lock(write_mutex_);
    std::size_t written = fwrite(&header, sizeof(header), 1, file_);
    if (written != 1) {
      return false;
    }

    for (const std::pair<const void*, std::size_t> &part : parts) {
      written = fwrite(part.first, part.second, 1, file_);
      if (written != 1) {
        return false;
      }
    }

    if (sync) {
      if (!SyncFile(file_)) {
        return false;
      }
    }
  }

  return true;
}

bool LogWriterStdio::Sync() {
  std::lock_guard<std::mutex> lock(write_mutex_);
  return SyncFile(file_);
}

}  // namespace internal
}  // namespace tmb
