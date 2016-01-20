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

#include "tmb/internal/log_writer_posix.h"

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <mutex>  // NOLINT(build/c++11)
#include <string>
#include <utility>

#include "tmb/internal/crc32.h"
#include "tmb/internal/log_record_header.h"
#include "tmb/internal/logging_constants.h"

namespace {

inline bool SyncFD(const int fd) {
#if defined(TMB_POSIX_FDATASYNC_SUPPORTED)
  return (fdatasync(fd) == 0);
#elif defined(TMB_DARWIN_FFULLFSYNC_SUPPORTED)
  return (fcntl(fd, F_FULLFSYNC) != -1);
#else
  return (fsync(fd) == 0);
#endif
}

}  // namespace

namespace tmb {
namespace internal {

bool LogWriterPosix::OpenReset() {
  fd_ = open(filename_.c_str(),
             O_WRONLY | O_CREAT | O_TRUNC,
             S_IRUSR | S_IWUSR);
  write_allocation_.store(0, std::memory_order_relaxed);
  file_size_.store(0, std::memory_order_relaxed);
  return (fd_ >= 0);
}

bool LogWriterPosix::OpenAtPosition(const std::size_t offset) {
  fd_ = open(filename_.c_str(),
             O_WRONLY,
             S_IRUSR | S_IWUSR);
  if (fd_ < 0) {
    return false;
  }

  // Determine file's size.
  const off_t end_pos = lseek(fd_, 0, SEEK_END);
  if (end_pos == static_cast<off_t>(-1)) {
    return false;
  }

  if (end_pos < static_cast<off_t>(offset)) {
    // Desired offset is beyond the end of the file.
    return false;
  }
  file_size_.store(end_pos, std::memory_order_relaxed);

  const off_t set_pos = lseek(fd_, offset, SEEK_SET);
  if (set_pos != static_cast<off_t>(offset)) {
    return false;
  }
  write_allocation_.store(offset, std::memory_order_relaxed);

  return true;
}

bool LogWriterPosix::Close() {
  if (!IsOpen()) {
    return true;
  }

  const int status = close(fd_);
  fd_ = -1;
  return (status == 0);
}

bool LogWriterPosix::WriteRecord(const void *data,
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

  struct iovec segments[2];
  segments[0].iov_base = &header;
  segments[0].iov_len = sizeof(header);
  segments[1].iov_base = const_cast<void*>(data);
  segments[1].iov_len = length;

  if (!GrowFileIfNeeded(sizeof(header) + length)) {
    return false;
  }

  ssize_t written = writev(fd_, segments, 2);
  if (written != static_cast<ssize_t>(sizeof(header) + length)) {
    return false;
  }

  if (sync) {
    return SyncFD(fd_);
  }

  return true;
}

bool LogWriterPosix::WriteMultipartRecord(
    std::initializer_list<std::pair<const void*, std::size_t>> parts,
    const bool sync) {
  assert(IsOpen());

  if (parts.size() + 1 > IOV_MAX) {
    // Too many segments.
    return false;
  }

  LogRecordHeader header;
  header.length = 0;
  header.crc32 = kInitialCrc32;

  struct iovec *segments = new struct iovec[parts.size() + 1];
  segments[0].iov_base = &header;
  segments[0].iov_len = sizeof(header);

  std::initializer_list<std::pair<const void*, std::size_t>>::const_iterator
      it = parts.begin();
  for (std::size_t part_idx = 0;
       part_idx < parts.size();
       ++part_idx, ++it) {
    const std::pair<const void*, std::size_t> &part = *it;;
    assert(part.first != nullptr);
    assert(part.second != 0);

    header.length += part.second;
    header.crc32 = UpdateCrc32(header.crc32, part.first, part.second);

    segments[part_idx + 1].iov_base = const_cast<void*>(part.first);
    segments[part_idx + 1].iov_len = part.second;
  }
  header.crc32 = FinalizeCrc32(header.crc32);

  if (header.length > kMaxLogRecordSize) {
    delete[] segments;
    return false;
  }

  if (!GrowFileIfNeeded(sizeof(header) + header.length)) {
    delete[] segments;
    return false;
  }

  ssize_t written = writev(fd_, segments, parts.size() + 1);
  delete[] segments;

  if (written != static_cast<ssize_t>(sizeof(header) + header.length)) {
    return false;
  }

  if (sync) {
    return SyncFD(fd_);
  }

  return true;
}

bool LogWriterPosix::Sync() {
  return SyncFD(fd_);
}

bool LogWriterPosix::GrowFileIfNeeded(const std::size_t write_size) {
  const std::size_t write_end
      = write_allocation_.fetch_add(write_size, std::memory_order_relaxed)
        + write_size;
  if (write_end > file_size_.load(std::memory_order_relaxed)) {
    std::lock_guard<std::mutex> lock(resize_mutex_);
    std::size_t original_file_size = file_size_.load(
        std::memory_order_relaxed);
    if (write_end > original_file_size) {
      std::size_t new_file_size = ((write_end / kLogSegmentSize) + 1)
                                  * kLogSegmentSize;
      const int status = ftruncate(fd_, new_file_size);
      if (status == 0) {
        file_size_.store(new_file_size, std::memory_order_relaxed);
      }
      return (status == 0);
    }
  }

  return true;
}

}  // namespace internal
}  // namespace tmb
