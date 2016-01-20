/**
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

#ifndef QUICKSTEP_UTILITY_MEMSTREAM_HPP_
#define QUICKSTEP_UTILITY_MEMSTREAM_HPP_

#include <cstddef>
#include <cstdio>
#include <cstdlib>

#include "utility/Macros.hpp"
#include "utility/UtilityConfig.h"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief Abstract interface for a FILE* stream backed by an in-memory buffer
 *        (or a simulation thereof). Intended to simplify testing of I/O
 *        routines that operate on FILE* streams.
 **/
class MemStreamInterface {
 public:
  /**
   * @brief Virtual destructor.
   **/
  virtual ~MemStreamInterface() {
  }

  /**
   * @return A writable FILE* stream.
   **/
  virtual FILE* file() = 0;

  /**
   * @return All the bytes that have been written to the FILE* stream so far as
   *         a c-string.
   **/
  virtual const char* str() = 0;

  /**
   * @brief Reset the stream and buffer managed by this object to a clean,
   *        empty state, as though it had just been constructed.
   **/
  virtual void reset() = 0;

 protected:
  MemStreamInterface() {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(MemStreamInterface);
};

#ifdef QUICKSTEP_HAVE_OPEN_MEMSTREAM

/**
 * @brief MemStream implementation using POSIX.1-2008 open_memstream()
 *        function.
 **/
class MemStreamPosix final : public MemStreamInterface {
 public:
  MemStreamPosix()
      : stream_(nullptr),
        filebuf_(nullptr),
        filebuf_size_(0) {
    stream_ = open_memstream(&filebuf_, &filebuf_size_);
    CHECK_NOTNULL(stream_);
  }

  ~MemStreamPosix() final {
    CHECK_EQ(0, std::fclose(stream_));
    std::free(filebuf_);
  }

  FILE* file() final {
    return stream_;
  }

  const char* str() final {
    CHECK_EQ(0, std::fflush(stream_));
    return filebuf_;
  }

  void reset() final {
    CHECK_EQ(0, std::fclose(stream_));
    std::free(filebuf_);
    filebuf_ = nullptr;
    filebuf_size_ = 0;
    stream_ = open_memstream(&filebuf_, &filebuf_size_);
    CHECK_NOTNULL(stream_);
  }

 private:
  FILE *stream_;
  char *filebuf_;
  std::size_t filebuf_size_;

  DISALLOW_COPY_AND_ASSIGN(MemStreamPosix);
};

typedef MemStreamPosix MemStream;

#else

/**
 * @brief MemStream simulated by a standard C tmpfile.
 **/
class MemStreamStdio final : public MemStreamInterface {
 public:
  MemStreamStdio()
      : stream_(std::tmpfile()),
        buffer_(nullptr) {
    CHECK_NOTNULL(stream_);
  }

  ~MemStreamStdio() final {
    CHECK_EQ(0, std::fclose(stream_));
    if (buffer_ != nullptr) {
      std::free(buffer_);
    }
  }

  FILE* file() final {
    return stream_;
  }

  const char* str() final {
    if (buffer_ != nullptr) {
      std::free(buffer_);
    }

    CHECK_EQ(0, std::fflush(stream_));
    long int filepos = std::ftell(stream_);  // NOLINT(runtime/int)
    CHECK_GE(filepos, 0);
    buffer_ = static_cast<char*>(std::malloc(filepos + 1));

    std::rewind(stream_);
    std::size_t bytes_read = std::fread(buffer_, 1, filepos, stream_);
    CHECK_EQ(filepos, bytes_read);
    CHECK_EQ(filepos, std::ftell(stream_));

    buffer_[filepos] = '\0';

    return buffer_;
  }

  void reset() final {
    CHECK_EQ(0, std::fclose(stream_));
    if (buffer_ != nullptr) {
      std::free(buffer_);
      buffer_ = nullptr;
    }

    stream_ = std::tmpfile();
    CHECK_NOTNULL(stream_);
  }

 private:
  FILE *stream_;
  char *buffer_;

  DISALLOW_COPY_AND_ASSIGN(MemStreamStdio);
};

typedef MemStreamStdio MemStream;

#endif

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_MEMSTREAM_HPP_
