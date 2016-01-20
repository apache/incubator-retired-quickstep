//   Copyright 2014-2015 Quickstep Technologies LLC.
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#ifndef TMB_INTERNAL_LOG_READER_POSIX_H_
#define TMB_INTERNAL_LOG_READER_POSIX_H_

#include <cstdlib>
#include <string>

#include "tmb/internal/log_read_status.h"
#include "tmb/internal/log_reader_base.h"

namespace tmb {
namespace internal {

/**
 * @brief Reads records from a log file using POSIX I/O syscalls.
 **/
class LogReaderPosix : public LogReaderBase {
 public:
  explicit LogReaderPosix(const std::string &filename)
      : filename_(filename),
        fd_(-1),
        current_buffer_(nullptr),
        current_buffer_length_(0),
        last_good_record_end_(0) {
  }

  ~LogReaderPosix() override {
    FreeBuffer();

    if (IsOpen()) {
      Close();
    }
  }

  inline bool IsOpen() const {
    return fd_ >= 0;
  }

  // Attempt to open log file for reading. Returns true on success, false
  // otherwise.
  bool Open();

  // Close the file. Also frees currently buffered record, if any, and
  // resets the last "good" end position.
  bool Close();

  // Rewind file to beginning. Also frees currently buffered record, if any,
  // and resets the last "good" end position.
  bool Reset();

  // Attempt to read the next record in order from the open log file.
  LogReadStatus ReadNextRecord();

  // Get a pointer to the "current" (i.e. most recently read) log record. Only
  // usable if that last call to ReadNextRecord() returned
  // LogReaderStatus::kOK.
  inline const void* CurrentRecord() const {
    return current_buffer_;
  }

  // Get the length of the "current" log record in bytes.
  inline std::size_t CurrentRecordLength() const {
    return current_buffer_length_;
  }

  // Get the position in the log file where the last "good" record that was
  // read ends.
  inline std::size_t LastGoodRecordEnd() const {
    return last_good_record_end_;
  }

 private:
  inline void FreeBuffer() {
    if (current_buffer_ != nullptr) {
      std::free(current_buffer_);
      current_buffer_ = nullptr;
      current_buffer_length_ = 0;
    }
  }

  const std::string filename_;
  int fd_;

  void *current_buffer_;
  std::size_t current_buffer_length_;

  std::size_t last_good_record_end_;

  // Disallow copy and assign:
  LogReaderPosix(const LogReaderPosix &orig) = delete;
  LogReaderPosix& operator=(const LogReaderPosix &rhs) = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_LOG_READER_POSIX_H_
