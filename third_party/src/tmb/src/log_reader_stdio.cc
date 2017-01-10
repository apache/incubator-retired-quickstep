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

#include "tmb/internal/log_reader_stdio.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "tmb/internal/crc32.h"
#include "tmb/internal/log_record_header.h"
#include "tmb/internal/log_read_status.h"
#include "tmb/internal/logging_constants.h"

namespace tmb {
namespace internal {

bool LogReaderStdio::Open() {
  file_ = std::fopen(filename_.c_str(), "rb");
  return (file_ != nullptr);
}

bool LogReaderStdio::Close() {
  if (!IsOpen()) {
    return true;
  }

  FreeBuffer();
  last_good_record_end_ = 0;

  const int status = std::fclose(file_);
  file_ = nullptr;
  return (status == 0);
}

bool LogReaderStdio::Reset() {
  assert(IsOpen());

  FreeBuffer();
  std::rewind(file_);
  last_good_record_end_ = 0;
  return true;
}

LogReadStatus LogReaderStdio::ReadNextRecord() {
  assert(IsOpen());

  FreeBuffer();

  LogRecordHeader header;
  std::size_t bytes_read = std::fread(&header, 1, sizeof(header), file_);
  if (bytes_read != sizeof(header)) {
    if (std::feof(file_)) {
      return (bytes_read == 0) ? LogReadStatus::kEndOfFile
                               : LogReadStatus::kTruncatedRecord;
    } else {
      return LogReadStatus::kIoError;
    }
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
    int status = std::fseek(file_, header.length, SEEK_CUR);
    if (status != 0) {
      if (std::feof(file_)) {
        return LogReadStatus::kOversizeRecord;
      } else {
        return LogReadStatus::kIoError;
      }
    } else {
      return LogReadStatus::kOversizeRecord;
    }
  }

  current_buffer_ = std::malloc(header.length);
  current_buffer_length_ = header.length;

  bytes_read = std::fread(current_buffer_, 1, current_buffer_length_, file_);
  if (bytes_read != current_buffer_length_) {
    FreeBuffer();
    if (std::feof(file_)) {
      return LogReadStatus::kTruncatedRecord;
    } else {
      return LogReadStatus::kIoError;
    }
  }

  const std::uint32_t crc32_check = ComputeCrc32(current_buffer_,
                                                 current_buffer_length_);
  if (crc32_check != header.crc32) {
    FreeBuffer();
    return LogReadStatus::kChecksumFailed;
  }

  last_good_record_end_ = std::ftell(file_);
  return LogReadStatus::kOK;
}

}  // namespace internal
}  // namespace tmb
