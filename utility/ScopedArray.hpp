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

#ifndef QUICKSTEP_UTILITY_SCOPED_ARRAY_HPP_
#define QUICKSTEP_UTILITY_SCOPED_ARRAY_HPP_

#include <cstddef>
#include <utility>

#include "utility/ScopedBuffer.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

template <typename T>
class ScopedArray {
 public:
  explicit ScopedArray(const std::size_t length, const bool initialize = false)
      : buffer_(length * sizeof(T), initialize) {}

  explicit ScopedArray(T *data = nullptr)
      : buffer_(data) {}

  explicit ScopedArray(ScopedArray &&orig)
      : buffer_(std::move(orig.buffer_)) {}

  inline void reset(const std::size_t length, const bool initialize = false) {
    buffer_.reset(length * sizeof(T), initialize);
  }

  inline void reset(T *data = nullptr) {
    buffer_.reset(data);
  }

  inline ScopedArray& operator=(ScopedArray &&rhs) {
    buffer_ = std::move(rhs.buffer_);
    return *this;
  }

  inline T* get() const {
    return static_cast<T*>(buffer_.get());
  }

  inline T* release() {
    return static_cast<T*>(buffer_.release());
  }

  inline T* operator->() const {
    return get();
  }

  inline T& operator[](const std::size_t index) const {
    return get()[index];
  }

 private:
  ScopedBuffer buffer_;
};

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_SCOPED_ARRAY_HPP_
