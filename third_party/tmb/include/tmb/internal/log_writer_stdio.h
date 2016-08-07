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

#ifndef TMB_INTERNAL_LOG_WRITER_STDIO_H_
#define TMB_INTERNAL_LOG_WRITER_STDIO_H_

#include <cstdio>
#include <initializer_list>
#include <mutex>  // NOLINT(build/c++11)
#include <string>
#include <utility>

#include "tmb/internal/log_writer_base.h"

namespace tmb {
namespace internal {

/**
 * @brief Writes records to a log file using stdio functions for non-POSIX
 *        systems.
 **/
class LogWriterStdio : public LogWriterBase {
 public:
  explicit LogWriterStdio(const std::string &filename)
      : filename_(filename),
        file_(nullptr) {
  }

  ~LogWriterStdio() override {
    if (IsOpen()) {
      Close();
    }
  }

  inline bool IsOpen() const {
    return file_ != nullptr;
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
  const std::string filename_;
  FILE *file_;
  std::mutex write_mutex_;

  // Disallow copy and assign:
  LogWriterStdio(const LogWriterStdio &orig) = delete;
  LogWriterStdio& operator=(const LogWriterStdio &rhs) = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_LOG_WRITER_STDIO_H_
