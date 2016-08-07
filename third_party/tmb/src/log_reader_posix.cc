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

#include "tmb/internal/log_reader_posix.h"

#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>

#include "tmb/internal/crc32.h"
#include "tmb/internal/log_record_header.h"
#include "tmb/internal/log_read_status.h"
#include "tmb/internal/logging_constants.h"

namespace tmb {
namespace internal {

namespace {

inline LogReadStatus ReadFromFd(const int fd,
                                void *data,
                                const std::size_t length) {
  std::size_t bytes_total = 0;
  while (bytes_total < length) {
    ssize_t bytes = read(fd,
                         static_cast<char*>(data) + bytes_total,
                         length - bytes_total);
    if (bytes > 0) {
      bytes_total += bytes;
    } else if (bytes == -1 && errno != EINTR) {
      return LogReadStatus::kIoError;
    } else {
      if (bytes_total == 0) {
        return LogReadStatus::kEndOfFile;
      } else {
        return LogReadStatus::kTruncatedRecord;
      }
    }
  }

  return LogReadStatus::kOK;
}

}  // namespace

bool LogReaderPosix::Open() {
  fd_ = open(filename_.c_str(), O_RDONLY);
  return (fd_ >= 0);
}

bool LogReaderPosix::Close() {
  if (!IsOpen()) {
    return true;
  }

  FreeBuffer();
  last_good_record_end_ = 0;

  const int status = close(fd_);
  fd_ = -1;
  return (status == 0);
}

bool LogReaderPosix::Reset() {
  assert(IsOpen());

  FreeBuffer();

  off_t reset_pos = lseek(fd_, 0, SEEK_SET);
  if (reset_pos != 0) {
    return false;
  }

  last_good_record_end_ = 0;
  return true;
}

LogReadStatus LogReaderPosix::ReadNextRecord() {
  assert(IsOpen());

  FreeBuffer();

  LogRecordHeader header;
  LogReadStatus status = ReadFromFd(fd_, &header, sizeof(header));
  if (status != LogReadStatus::kOK) {
    return status;
  }

  if (header.length == 0) {
    // The file may actually extend far beyond this point because of "chunky"
    // allocation, but this is the end of actual log records.
    return LogReadStatus::kEndOfFile;
  }

  if (header.length > kMaxLogRecordSize) {
    // Record is beyond size limit (probably garbage bytes). Nevertheless, try
    // and fast-forward to where the next record would start if this header
    // were correct. If this seeks beyond the end of the file, subsequent reads
    // will return kEndOfFile.
    off_t newpos = lseek(fd_, header.length, SEEK_CUR);
    if (newpos == static_cast<off_t>(-1)) {
      if ((errno == EINVAL) || (errno == EOVERFLOW)) {
        // Likely tried to seek ridiculously far ahead. Try again, this time
        // just going to the end of the file.
        off_t original_pos = lseek(fd_, 0, SEEK_CUR);
        newpos = lseek(fd_, 0, SEEK_END);
        if (newpos == static_cast<off_t>(-1)) {
          return LogReadStatus::kIoError;
        } else if (newpos > original_pos + static_cast<off_t>(header.length)) {
          // Seeking to the end of the file put us past where the original
          // attempt at seeking should have put us. Wierd!
          return LogReadStatus::kIoError;
        } else {
          return LogReadStatus::kOversizeRecord;
        }
      } else {
        return LogReadStatus::kIoError;
      }
    } else {
      return LogReadStatus::kOversizeRecord;
    }
  }

  current_buffer_ = std::malloc(header.length);
  current_buffer_length_ = header.length;

  status = ReadFromFd(fd_, current_buffer_, current_buffer_length_);
  if (status == LogReadStatus::kEndOfFile) {
    FreeBuffer();
    return LogReadStatus::kTruncatedRecord;
  } else if (status != LogReadStatus::kOK) {
    FreeBuffer();
    return status;
  }

  const std::uint32_t crc32_check = ComputeCrc32(current_buffer_,
                                                 current_buffer_length_);
  if (crc32_check != header.crc32) {
    FreeBuffer();
    return LogReadStatus::kChecksumFailed;
  }

  last_good_record_end_ = lseek(fd_, 0, SEEK_CUR);
  return LogReadStatus::kOK;
}

}  // namespace internal
}  // namespace tmb
