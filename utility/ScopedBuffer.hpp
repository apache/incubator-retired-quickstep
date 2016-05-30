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

#ifndef QUICKSTEP_UTILITY_SCOPED_BUFFER_HPP_
#define QUICKSTEP_UTILITY_SCOPED_BUFFER_HPP_

#include <cstddef>
#include <cstdlib>

#include "utility/Alignment.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief A class which manages an untyped buffer of heap memory which is freed
 *        when it goes out of scope.
 **/
class ScopedBuffer {
 public:
  /**
   * @brief Constructor which allocates a new chunk of memory of the specified
   *        size.
   *
   * @param alloc_size The number of bytes of memory to allocate.
   * @param initialize If true, initialize all the bytes of the allocated memory to 0.
   **/
  explicit ScopedBuffer(const std::size_t alloc_size, bool initialize = true) {
    internal_ptr_ = std::malloc(alloc_size);
    if (initialize) {
      memset(internal_ptr_, 0x0, alloc_size);
    }
  }

  /**
   * @brief Constructor which allocates a new chunk of memory of the specified
   *        size with the specified alignment.
   *
   * @param alloc_size The number of bytes of memory to allocate.
   * @param alloc_alignment The alignment of the memory to allocate.
   * @param initialize If true, initialize all the bytes of the allocated memory to 0.
   **/
  ScopedBuffer(const std::size_t alloc_size,
               const std::size_t alloc_alignment,
               const bool initialize = true) {
    internal_ptr_ = malloc_with_alignment(alloc_size, alloc_alignment);
    if (initialize) {
      memset(internal_ptr_, 0x0, alloc_size);
    }
  }

  /**
   * @brief Constructor which takes ownership of an existing chunk of memory.
   * @warning memory MUST be allocated with malloc(), not with new or on the
   *          stack.
   *
   * @param memory The memory to take ownership of.
   **/
  explicit ScopedBuffer(void *memory = nullptr)
      : internal_ptr_(memory) {
  }

  /**
   * @brief Move constructor.
   *
   * @param orig The ScopedBuffer to move from, taking ownership of its memory.
   **/
  ScopedBuffer(ScopedBuffer &&orig)
      : internal_ptr_(orig.internal_ptr_) {
    orig.internal_ptr_ = nullptr;
  }

  /**
   * @brief Destructor which frees the memory held in this buffer.
   **/
  ~ScopedBuffer() {
    if (internal_ptr_ != nullptr) {
      std::free(internal_ptr_);
    }
  }

  /**
   * @brief Move-assignment operator.
   *
   * @param rhs The ScopedBuffer to move from, taking ownership of its memory.
   **/
  ScopedBuffer& operator=(ScopedBuffer &&rhs) {
    if (internal_ptr_ != nullptr) {
      std::free(internal_ptr_);
    }
    internal_ptr_ = rhs.internal_ptr_;
    rhs.internal_ptr_ = nullptr;

    return *this;
  }

  /**
   * @brief Free the buffer memory and create a new buffer of the specified
   *        size.
   *
   * @param alloc_size The number of bytes of memory to allocate.
   * @param initialize If true, initialize all the bytes of the allocated memory to 0.
   **/
  void reset(const std::size_t alloc_size, const bool initialize = true) {
    if (internal_ptr_ != nullptr) {
      std::free(internal_ptr_);
    }
    internal_ptr_ = std::malloc(alloc_size);
    if (initialize) {
      memset(internal_ptr_, 0x0, alloc_size);
    }
  }

  /**
   * @brief Free the buffer memory and take ownership of the specified chunk of
   *        memory.
   * @warning memory MUST be allocated with malloc(), not with new or on the
   *          stack.
   *
   * @param memory The memory to take ownership of.
   **/
  void reset(void *memory = nullptr) {
    if (internal_ptr_ != NULL) {
      std::free(internal_ptr_);
    }
    internal_ptr_ = memory;
  }

  /**
   * @brief Resize this ScopedBuffer, growing or shrinking it and preserving
   *        its contents.
   * @warning This method may relocate the ScopedBuffer in memory. Any pointers
   *          that were obtained by calling get() may become invalidated and
   *          should be reset after calling this method.
   *
   * @param new_alloc_size The new desired size, in bytes, for this
   *        ScopedBuffer.
   **/
  void resize(const std::size_t new_alloc_size) {
    DCHECK(internal_ptr_ != nullptr);
    if (new_alloc_size == 0) {
      reset();
    } else {
      internal_ptr_ = std::realloc(internal_ptr_, new_alloc_size);
    }
  }

  /**
   * @brief Release ownership of the memory owned by this ScopedBuffer, and
   *        return a pointer to it.
   * @warning The caller becomes responsible for managing the memory returned
   *          by this method, and should free it when it is no longer in use to
   *          avoid memory leaks.
   *
   * @return A pointer to the memory previously owned by this ScopedBuffer.
   *         NULL if empty() was true before the call.
   **/
  void* release() {
    void *memory = internal_ptr_;
    internal_ptr_ = nullptr;
    return memory;
  }

  /**
   * @brief Check whether this ScopedBuffer is empty, i.e. whether it currently
   *        owns any memory.
   *
   * @return Whether this ScopedBuffer is empty.
   **/
  inline bool empty() const {
    return internal_ptr_ == nullptr;
  }

  /**
   * @brief Get a pointer to the memory owned by this ScopedBuffer.
   * @warning Do not call free() on the memory returned by this method. Use
   *          reset() or delete the ScopedBuffer instead.
   *
   * @return A pointer the memory managed by this ScopedBuffer.
   **/
  inline void* get() const {
    return internal_ptr_;
  }

 private:
  void *internal_ptr_;

  DISALLOW_COPY_AND_ASSIGN(ScopedBuffer);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_SCOPED_BUFFER_HPP_
