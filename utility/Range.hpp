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

#ifndef QUICKSTEP_UTILITY_RANGE_HPP_
#define QUICKSTEP_UTILITY_RANGE_HPP_

#include <algorithm>
#include <cstddef>
#include <limits>

#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class Range {
 public:
  Range(const std::size_t begin, const std::size_t end)
      : begin_(begin), end_(end) {
    DCHECK_LE(begin_, end_);
  }

  Range(const Range &range)
      : begin_(range.begin_), end_(range.end_) {
    DCHECK_LE(begin_, end_);
  }

  inline std::size_t begin() const {
    return begin_;
  }

  inline std::size_t end() const {
    return end_;
  }

  inline std::size_t size() const {
    return end_ - begin_;
  }

 private:
  const std::size_t begin_;
  const std::size_t end_;
};

class RangeSplitter {
 public:
  static RangeSplitter CreateWithPartitionLength(
      const std::size_t begin,
      const std::size_t end,
      const std::size_t min_partition_length,
      const std::size_t max_num_partitions = kMaxNumPartitions) {
    DCHECK_LE(begin, end);
    DCHECK_GT(min_partition_length, 0u);
    DCHECK_GT(max_num_partitions, 0u);

    const std::size_t range_length = end - begin;
    const std::size_t est_num_partitions = range_length / min_partition_length;

    const std::size_t num_partitions =
        std::max(1uL, std::min(est_num_partitions, max_num_partitions));
    const std::size_t partition_length = range_length / num_partitions;
    return RangeSplitter(begin, end, num_partitions, partition_length);
  }

  static RangeSplitter CreateWithPartitionLength(
      const Range &range,
      const std::size_t min_partition_length,
      const std::size_t max_num_partitions = kMaxNumPartitions) {
    return CreateWithPartitionLength(
        range.begin(), range.end(), min_partition_length, max_num_partitions);
  }

  static RangeSplitter CreateWithMinMaxPartitionLength(
      const std::size_t begin,
      const std::size_t end,
      const std::size_t min_partition_length,
      const std::size_t max_partition_length,
      const std::size_t ept_num_partitions) {
    DCHECK_LE(begin, end);
    DCHECK_LE(min_partition_length, max_partition_length);
    DCHECK_GT(min_partition_length, 0u);
    DCHECK_GT(max_partition_length, 0u);

    const std::size_t range_length = end - begin;
    const std::size_t ept_partition_length = range_length / ept_num_partitions;

    std::size_t partition_length;
    if (ept_partition_length < min_partition_length) {
      partition_length = min_partition_length;
    } else if (ept_partition_length > max_partition_length) {
      partition_length = max_partition_length;
    } else {
      partition_length = ept_partition_length;
    }

    const std::size_t num_partitions =
        std::max(1uL, range_length / partition_length);
    return RangeSplitter(begin, end, num_partitions, partition_length);
  }

  static RangeSplitter CreateWithMinMaxPartitionLength(
      const Range &range,
      const std::size_t min_partition_length,
      const std::size_t max_partition_length,
      const std::size_t ept_num_partitions) {
    return CreateWithMinMaxPartitionLength(
        range.begin(), range.end(),
        min_partition_length, max_partition_length,
        ept_num_partitions);
  }

  static RangeSplitter CreateWithNumPartitions(
      const std::size_t begin,
      const std::size_t end,
      const std::size_t num_partitions) {
    DCHECK_LE(begin, end);
    DCHECK_GT(num_partitions, 0u);

    const std::size_t partition_length = (end - begin) / num_partitions;
    return RangeSplitter(begin, end, num_partitions, partition_length);
  }

  static RangeSplitter CreateWithNumPartitions(
      const Range &range,
      const std::size_t num_partitions) {
    return CreateWithNumPartitions(range.begin(), range.end(), num_partitions);
  }

  RangeSplitter(const RangeSplitter &other)
      : begin_(other.begin_), end_(other.end_),
        num_partitions_(other.num_partitions_),
        partition_length_(other.partition_length_) {}

  inline std::size_t getNumPartitions() const {
    return num_partitions_;
  }

  inline Range getPartition(const std::size_t partition_id) const {
    DCHECK_LT(partition_id, num_partitions_);
    const std::size_t begin = begin_ + partition_length_ * partition_id;
    const std::size_t end =
        partition_id == num_partitions_ - 1
            ? end_
            : begin + partition_length_;
    return Range(begin, end);
  }

 private:
  RangeSplitter(const std::size_t begin,
                const std::size_t end,
                const std::size_t num_partitions,
                const std::size_t partition_length)
      : begin_(begin),
        end_(end),
        num_partitions_(num_partitions),
        partition_length_(partition_length) {
    DCHECK_LE(num_partitions_ * partition_length_, end_);
  }

  static constexpr std::size_t kMaxNumPartitions =
      std::numeric_limits<std::size_t>::max();

  const std::size_t begin_;
  const std::size_t end_;
  const std::size_t num_partitions_;
  const std::size_t partition_length_;
};

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_RANGE_HPP_
