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

#ifndef TMB_INTERNAL_LOG_WRITER_POSIX_H_
#define TMB_INTERNAL_LOG_WRITER_POSIX_H_

#include <atomic>
#include <cstddef>
#include <initializer_list>
#include <mutex>  // NOLINT(build/c++11)
#include <string>
#include <utility>

#include "tmb/internal/cache_info.h"
#include "tmb/internal/log_writer_base.h"

namespace tmb {
namespace internal {

/**
 * @brief Writes records to a log file using POSIX I/O syscalls.
 **/
class LogWriterPosix : public LogWriterBase {
 public:
  explicit LogWriterPosix(const std::string &filename)
      : filename_(filename),
        fd_(-1) {
  }

  ~LogWriterPosix() override {
    if (IsOpen()) {
      Close();
    }
  }

  inline bool IsOpen() const {
    return fd_ >= 0;
  }

  // Open for writing, resetting the log and erasing any existing entries.
  bool OpenReset();

  // Open for writing, starting from 'offset' bytes into file.
  bool OpenAtPosition(const std::size_t offset);

  // Close the file.
  bool Close();

  // Write a single record ('length' bytes at '*data') to the log. If 'sync' is
  // true, record will be synchronously flushed to disk to guarantee that it is
  // recoverable.
  bool WriteRecord(const void *data,
                   const std::size_t length,
                   const bool sync);

  // Write a multipart record (consisting of 'parts', each of which is a
  // pointer to data and a length in bytes) to the log. If 'sync' is true,
  // record will be synchronously flushed to disk to guarantee that it is
  // recoverable.
  bool WriteMultipartRecord(
      std::initializer_list<std::pair<const void*, std::size_t>> parts,
      const bool sync);

  // Explicitly flush the log to disk so that all previously written records
  // will be guaranteed recoverable.
  bool Sync();

 private:
  // Extend file to the next kLogSegmentSize boundary beyond the current end of
  // the log plus 'write_size' bytes. Allocating large chunks of log file
  // infrequently means that flushing the log requires only fdatasync() (i.e.
  // flush file pages to disk) and not fsync() (update file metadata in
  // filesystem journal).
  bool GrowFileIfNeeded(const std::size_t write_size);

  const std::string filename_;
  int fd_;

  alignas(kCacheLineBytes) std::atomic<std::size_t> write_allocation_;
  alignas(kCacheLineBytes) std::atomic<std::size_t> file_size_;
  alignas(kCacheLineBytes) std::mutex resize_mutex_;

  // Disallow copy and assign:
  LogWriterPosix(const LogWriterPosix &orig) = delete;
  LogWriterPosix& operator=(const LogWriterPosix &rhs) = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_LOG_WRITER_POSIX_H_
