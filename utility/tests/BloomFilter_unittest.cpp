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

#include <cstdint>
#include <memory>
#include <vector>

#include "utility/BloomFilter.hpp"

#include "gtest/gtest.h"


namespace quickstep {

class BloomFilterTest : public ::testing::Test {
 public:
  static const std::uint64_t kBloomFilterSeed = 0xA5A5A5A55A5A5A5AULL;
  static const std::uint32_t kNumberOfHashFunctions = 20;
  static const std::uint32_t kBloomFilterSize = 100000;  // in bytes.
};

TEST_F(BloomFilterTest, BloomFilterInsertTest) {
  // This test inserts an element into a known bloom filter size
  // with given number of hash functions and initial seed,
  // and tests whether the expected bits are set or not.
  std::unique_ptr<std::uint8_t> bit_array;
  std::unique_ptr<BloomFilter> bloom_filter;

  const std::uint32_t num_hashes = 2;
  const std::uint32_t bloom_filter_size = 1;

  bit_array.reset(new std::uint8_t[bloom_filter_size]);
  bloom_filter.reset(new BloomFilter(kBloomFilterSeed,
                                     num_hashes,
                                     bloom_filter_size,
                                     bit_array.get(),
                                     false));

  const std::uint8_t data = 7u;
  const std::uint8_t expected_bit_array = 136u;
  bloom_filter->insert(&data, 1);
  EXPECT_EQ(*bit_array, expected_bit_array);
}

TEST_F(BloomFilterTest, BloomFilterContainsTest) {
  std::unique_ptr<std::uint8_t> bit_array;
  std::unique_ptr<BloomFilter> bloom_filter;

  bit_array.reset(new std::uint8_t[kBloomFilterSize]);
  bloom_filter.reset(new BloomFilter(kBloomFilterSeed,
                                     kNumberOfHashFunctions,
                                     kBloomFilterSize,
                                     bit_array.get(),
                                     false));

  // Insert a set of values in the bloom filter.
  const std::uint32_t data[] = { 4, 60, 100, 9999 };
  const std::vector<std::uint32_t> data_vector(data, data + sizeof(data) / sizeof(data[0]));
  for (const std::uint32_t &value : data_vector) {
    bloom_filter->insert(reinterpret_cast<const uint8_t*>(&value), sizeof(std::uint32_t));
  }

  // Test the values, which were inserted, are present in the bloom filter.
  for (const std::uint32_t &value : data_vector) {
    EXPECT_TRUE(bloom_filter->contains(reinterpret_cast<const uint8_t*>(&value), sizeof(std::uint32_t)));
  }

  // Test the values, which were not inserted in the bloom filter, are not present.
  const std::uint32_t missing[] = { 5, 99, 999, 11111 };
  const std::vector<std::uint32_t> missing_vector(missing, missing + sizeof(missing) / sizeof(missing[0]));
  for (const std::uint32_t &value : missing_vector) {
    EXPECT_FALSE(bloom_filter->contains(reinterpret_cast<const uint8_t*>(&value), sizeof(std::uint32_t)));
  }
}

}  // namespace quickstep
