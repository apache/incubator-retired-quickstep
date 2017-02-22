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

#ifndef QUICKSTEP_UTILITY_BOOL_VECTOR_HPP_
#define QUICKSTEP_UTILITY_BOOL_VECTOR_HPP_

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>

#include "threading/SpinMutex.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

class BoolVector {
 public:
  BoolVector(void *memory_location,
             const std::size_t length,
             const bool initialize)
      : owned_(false),
        length_(length),
        data_array_(static_cast<bool *>(memory_location)) {
    DCHECK_GT(length, 0u);
    DCHECK(data_array_ != nullptr);

    if (initialize) {
      clear();
    }
  }

  explicit BoolVector(const std::size_t length)
      : owned_(true),
        length_(length),
        data_array_(static_cast<bool *>(std::malloc(sizeof(bool) * length))) {
    DCHECK_GT(length, 0u);
    clear();
  }

  ~BoolVector() {
    if (owned_) {
      std::free(data_array_);
    }
  }

  inline void clear() {
    std::memset(data_array_, 0, sizeof(bool) * length_);
  }

  inline void set(const std::size_t loc) {
    data_array_[loc] = true;
  }

  inline bool get(const std::size_t loc) const {
    return data_array_[loc];
  }

  inline void unionWith(const BoolVector &other,
                        const std::size_t start_position,
                        const std::size_t end_position) const {
    for (std::size_t loc = start_position; loc < end_position; ++loc) {
      data_array_[loc] |= other.data_array_[loc];
    }
  }

  inline std::size_t onesCountInRange(const std::size_t start_position,
                                      const std::size_t end_position) const {
    DCHECK_LE(start_position, end_position);
    DCHECK_LT(start_position, length_);
    DCHECK_LE(end_position, length_);

    std::size_t count = 0;
    for (std::size_t i = start_position; i < end_position; ++i) {
      count += data_array_[i];
    }
    return count;
  }

 private:
  const bool owned_;
  const std::size_t length_;
  bool *data_array_;

  DISALLOW_COPY_AND_ASSIGN(BoolVector);
};

class BoolVectorPool {
 public:
  explicit BoolVectorPool(const std::size_t vector_length)
      : vector_length_(vector_length) {}

  BoolVector* checkOut() {
    {
      SpinMutexLock lock(mutex_);
      if (!pool_.empty()) {
        BoolVector *ret = pool_.back().release();
        pool_.pop_back();
        return ret;
      }
    }
    return new BoolVector(vector_length_);
  }

  void checkIn(BoolVector *bool_vector) {
    SpinMutexLock lock(mutex_);
    pool_.emplace_back(bool_vector);
  }

  std::vector<std::unique_ptr<BoolVector>>& getAll() {
    return pool_;
  }

  const std::vector<std::unique_ptr<BoolVector>>& getAll() const {
    return pool_;
  }

 private:
  const std::size_t vector_length_;

  SpinMutex mutex_;
  std::vector<std::unique_ptr<BoolVector>> pool_;

  DISALLOW_COPY_AND_ASSIGN(BoolVectorPool);
};

class BarrieredReadWriteConcurrentBoolVector {
 public:
  BarrieredReadWriteConcurrentBoolVector(void *memory_location,
                                         const std::size_t length,
                                         const bool initialize)
      : owned_(false),
        length_(length),
        data_array_(static_cast<DataType *>(memory_location)) {
    DCHECK_GT(length, 0u);
    DCHECK(data_array_ != nullptr);

    if (initialize) {
      clear();
    }
  }

  explicit BarrieredReadWriteConcurrentBoolVector(const std::size_t length)
      : owned_(true),
        length_(length),
        data_array_(static_cast<DataType *>(std::malloc(BytesNeeded(length)))) {
    DCHECK_GT(length, 0u);
    clear();
  }

  ~BarrieredReadWriteConcurrentBoolVector() {
    if (owned_) {
      std::free(data_array_);
    }
  }

  inline static std::size_t BytesNeeded(const std::size_t length) {
    return kDataSize * length;
  }

  inline void clear() {
    std::memset(data_array_, 0, BytesNeeded(length_));
  }

  inline void set(const std::size_t loc) {
    data_array_[loc].store(true, std::memory_order_relaxed);
  }

  inline bool get(const std::size_t loc) const {
    return data_array_[loc].load(std::memory_order_relaxed);
  }

  inline std::size_t onesCountInRange(const std::size_t start_position,
                                      const std::size_t end_position) const {
    DCHECK_LE(start_position, end_position);
    DCHECK_LT(start_position, length_);
    DCHECK_LE(end_position, length_);

    std::size_t count = 0;
    for (std::size_t i = start_position; i < end_position; ++i) {
      count += data_array_[i].load(std::memory_order_relaxed);
    }
    return count;
  }

 private:
  typedef std::atomic<bool> DataType;
  static constexpr std::size_t kDataSize = sizeof(DataType);

  const bool owned_;
  const std::size_t length_;
  DataType *data_array_;

  DISALLOW_COPY_AND_ASSIGN(BarrieredReadWriteConcurrentBoolVector);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_BOOL_VECTOR_HPP_
