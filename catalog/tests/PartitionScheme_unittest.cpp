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

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "catalog/PartitionScheme.hpp"
#include "catalog/PartitionSchemeHeader.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/EqualComparison.hpp"
#include "utility/CompositeHash.hpp"

#include "gtest/gtest.h"

using std::make_unique;
using std::move;
using std::size_t;
using std::vector;

namespace quickstep {

TEST(PartitionSchemeHeaderTest, IntegerHashPartitionSchemeHeaderTest) {
  const std::size_t num_partitions = 4;
  std::unique_ptr<PartitionSchemeHeader> partition_scheme_header(
      new HashPartitionSchemeHeader(num_partitions, { 0 }, { kInt }));
  EXPECT_EQ(num_partitions, partition_scheme_header->getNumPartitions());
  EXPECT_EQ(0, partition_scheme_header->getPartitionAttributeIds().front());
  const int kSampleInts[] = {
      0, 1, 2, 3, 400, 501, 64783970, 78437883, -2784627};
  const size_t num_ints = sizeof(kSampleInts) / sizeof(kSampleInts[0]);
  for (size_t i = 0; i < num_ints; ++i) {
    // Check if the partition id returned by the partition scheme for
    // an integer is the same as the hash of the integer modulus the number
    // of partitions.
    EXPECT_EQ(TypedValue(kSampleInts[i]).getHash() % num_partitions,
              partition_scheme_header->getPartitionId({ TypedValue(kSampleInts[i]) }));
  }
}

TEST(PartitionSchemeHeaderTest, LongHashPartitionSchemeHeaderTest) {
  const std::size_t num_partitions = 8;
  std::unique_ptr<PartitionSchemeHeader> partition_scheme_header(
      new HashPartitionSchemeHeader(num_partitions, { 0 }, { kLong }));
  EXPECT_EQ(num_partitions, partition_scheme_header->getNumPartitions());
  EXPECT_EQ(0, partition_scheme_header->getPartitionAttributeIds().front());
  const std::int64_t kSampleLongs[] = {INT64_C(10),
                                  INT64_C(100),
                                  INT64_C(1025),
                                  INT64_C(9876543),
                                  INT64_C(-89234758937573987)};
  const size_t num_longs = sizeof(kSampleLongs) / sizeof(kSampleLongs[0]);
  // Check if the partition id returned by the partition scheme for
  // a long is the same as the hash of the long number modulus the number
  // of partitions.
  for (size_t i = 0; i < num_longs; ++i) {
    EXPECT_EQ(TypedValue(kSampleLongs[i]).getHash() % num_partitions,
              partition_scheme_header->getPartitionId({ TypedValue(kSampleLongs[i]) }));
  }
}

TEST(PartitionSchemeHeaderTest, FloatHashPartitionSchemeHeaderTest) {
  const std::size_t num_partitions = 5;
  std::unique_ptr<PartitionSchemeHeader> partition_scheme_header(
      new HashPartitionSchemeHeader(num_partitions, { 0 }, { kFloat }));
  EXPECT_EQ(num_partitions, partition_scheme_header->getNumPartitions());
  EXPECT_EQ(0, partition_scheme_header->getPartitionAttributeIds().front());
  const float kSampleFloats[] = {
      285728.895680f, 924005.4989f, -8973494.37438f};
  const size_t num_floats = sizeof(kSampleFloats) / sizeof(kSampleFloats[0]);
  // Check if the partition id returned by the partition scheme for
  // a float is the same as the hash of the floating point number modulus
  // the number of partitions.
  for (size_t i = 0; i < num_floats; ++i) {
    EXPECT_EQ(TypedValue(kSampleFloats[i]).getHash() % num_partitions,
              partition_scheme_header->getPartitionId({ TypedValue(kSampleFloats[i]) }));
  }
}

TEST(PartitionSchemeHeaderTest, DoubleHashPartitionSchemeHeaderTest) {
  const std::size_t num_partitions = 6;
  std::unique_ptr<PartitionSchemeHeader> partition_scheme_header(
      new HashPartitionSchemeHeader(num_partitions, { 0 }, { kDouble }));
  EXPECT_EQ(num_partitions, partition_scheme_header->getNumPartitions());
  EXPECT_EQ(0, partition_scheme_header->getPartitionAttributeIds().front());
  const double kSampleDoubles[] = {
      1.0378, 763624.46343453, -87238497384.3187431894713};
  const size_t num_doubles = sizeof(kSampleDoubles) / sizeof(kSampleDoubles[0]);
  // Check if the partition id returned by the partition scheme for
  // a double is the same as the hash of the double modulus the number
  // of partitions.
  for (size_t i = 0; i < num_doubles; ++i) {
    EXPECT_EQ(
        TypedValue(kSampleDoubles[i]).getHash() % num_partitions,
        partition_scheme_header->getPartitionId({ TypedValue(kSampleDoubles[i]) }));
  }
}

TEST(PartitionSchemeHeaderTest, CharacterHashPartitionSchemeHeaderTest) {
  const std::size_t num_partitions = 7;
  std::unique_ptr<PartitionSchemeHeader> partition_scheme_header(
      new HashPartitionSchemeHeader(num_partitions, { 0 }, { kChar }, { 20 }));
  EXPECT_EQ(num_partitions, partition_scheme_header->getNumPartitions());
  EXPECT_EQ(0, partition_scheme_header->getPartitionAttributeIds().front());
  const char *kSampleStrings[] = {
      "a", "gerald", "ram", "3081289", "=42?", "+-/*&^%", "hello_world"};
  const size_t num_strings = sizeof(kSampleStrings) / sizeof(kSampleStrings[0]);
  // Check if the partition id returned by the partition scheme for
  // characters is the same as the hash of the characters modulus the number
  // of partitions.
  for (size_t i = 0; i < num_strings; ++i) {
    EXPECT_EQ(
        TypedValue(
            kChar, kSampleStrings[i], std::strlen(kSampleStrings[i]) + 1)
                .getHash() %
            num_partitions,
        partition_scheme_header->getPartitionId({ TypedValue(
            kChar, kSampleStrings[i], std::strlen(kSampleStrings[i]) + 1) }));
  }
}

TEST(PartitionSchemeHeaderTest, VarCharHashPartitionSchemeHeaderTest) {
  const std::size_t num_partitions = 7;
  std::unique_ptr<PartitionSchemeHeader> partition_scheme_header(
      new HashPartitionSchemeHeader(num_partitions, { 0 }, { kVarChar }));
  EXPECT_EQ(num_partitions, partition_scheme_header->getNumPartitions());
  EXPECT_EQ(0, partition_scheme_header->getPartitionAttributeIds().front());
  const char *kSampleStrings[] = {
      "hello", "world", "1234567", "!@#$^&*", "pa345+="};
  const size_t num_strings = sizeof(kSampleStrings) / sizeof(kSampleStrings[0]);
  // Check if the partition id returned by the partition scheme for
  // a variable length string is the same as the hash of the variable length string
  // modulus the number of partitions.
  for (size_t i = 0; i < num_strings; ++i) {
    EXPECT_EQ(
        TypedValue(
            kVarChar, kSampleStrings[i], std::strlen(kSampleStrings[i]) + 1)
                .getHash() %
            num_partitions,
        partition_scheme_header->getPartitionId(
            { TypedValue(kVarChar, kSampleStrings[i], std::strlen(kSampleStrings[i]) + 1) }));
  }
}

TEST(PartitionSchemeHeaderTest, MultiAttributeHashPartitionSchemeHeaderTest) {
  const std::size_t num_partitions = 4;
  constexpr attribute_id kPartitioningFirstAttribute = 0;
  constexpr attribute_id kPartitioningLastAttribute = 2;
  std::unique_ptr<PartitionSchemeHeader> partition_scheme_header(
      new HashPartitionSchemeHeader(num_partitions, { kPartitioningFirstAttribute, kPartitioningLastAttribute },
                                    { kInt, kDouble }));
  EXPECT_EQ(num_partitions, partition_scheme_header->getNumPartitions());
  EXPECT_EQ(kPartitioningFirstAttribute, partition_scheme_header->getPartitionAttributeIds().front());
  EXPECT_EQ(kPartitioningLastAttribute, partition_scheme_header->getPartitionAttributeIds().back());
  const int kSampleInts[] = {
      0, 78437883, -2784627};
  const double kSampleDoubles[] = {
      1.0378, 763624.46343453, -87238497384.3187431894713};
  const size_t num_ints = sizeof(kSampleInts) / sizeof(kSampleInts[0]);
  for (size_t i = 0; i < num_ints; ++i) {
    const PartitionSchemeHeader::PartitionValues values =
        { TypedValue(kSampleInts[i]), TypedValue(kSampleDoubles[i]) };
    // Check if the partition id returned by the partition scheme for
    // an integer is the same as the hash of the integer modulus the number
    // of partitions.
    EXPECT_EQ(HashCompositeKey(values) % num_partitions,
              partition_scheme_header->getPartitionId(values));
  }
}

TEST(PartitionSchemeHeaderTest, IntegerRangePartitionSchemeHeaderTest) {
  vector<PartitionSchemeHeader::PartitionValues> partition_ranges;
  // Partition boundaries are 0, 10, 20.
  // Last partition can hold upto infinity.
  // First partition can hold from -infinity to -1.
  for (int i = 0; i < 3; ++i) {
    partition_ranges.push_back({ TypedValue(i * 10) });
  }
  std::unique_ptr<PartitionSchemeHeader> partition_scheme_header(
      new RangePartitionSchemeHeader(4, { 0 }, { &TypeFactory::GetType(kInt) }, move(partition_ranges)));
  EXPECT_EQ(4u, partition_scheme_header->getNumPartitions());
  // Check if the partition id returned by the Range Partition Scheme for
  // integers is the same as the partition id into which it is supposed to
  // be based on the partition boundaries that we have defined.
  EXPECT_EQ(0, partition_scheme_header->getPartitionAttributeIds().front());

  EXPECT_EQ(1u, partition_scheme_header->getPartitionId({ TypedValue(0) }));
  EXPECT_EQ(2u, partition_scheme_header->getPartitionId({ TypedValue(10) }));
  EXPECT_EQ(3u, partition_scheme_header->getPartitionId({ TypedValue(20) }));
  EXPECT_EQ(3u, partition_scheme_header->getPartitionId({ TypedValue(30) }));
  EXPECT_EQ(0u, partition_scheme_header->getPartitionId({ TypedValue(-4) }));
  EXPECT_EQ(2u, partition_scheme_header->getPartitionId({ TypedValue(15) }));
  EXPECT_EQ(1u, partition_scheme_header->getPartitionId({ TypedValue(6) }));
  EXPECT_EQ(0u, partition_scheme_header->getPartitionId({ TypedValue(-70) }));
  EXPECT_EQ(3u, partition_scheme_header->getPartitionId({ TypedValue(1000) }));
  EXPECT_EQ(3u, partition_scheme_header->getPartitionId({ TypedValue(20000) }));
}

TEST(PartitionSchemeHeaderTest, LongRangePartitionSchemeHeaderTest) {
  vector<PartitionSchemeHeader::PartitionValues> partition_ranges;
  // Partition boundaries are 0, 10000, 20000, 30000
  for (int i = 0; i < 3; ++i) {
    partition_ranges.push_back({ TypedValue(i * INT64_C(10000)) });
  }
  std::unique_ptr<PartitionSchemeHeader> partition_scheme_header(
      new RangePartitionSchemeHeader(4, { 0 }, { &TypeFactory::GetType(kLong) }, move(partition_ranges)));

  EXPECT_EQ(4u, partition_scheme_header->getNumPartitions());
  EXPECT_EQ(0, partition_scheme_header->getPartitionAttributeIds().front());
  // Check if the partition id returned by the Range Partition Scheme for
  // long numbers is the same as the partition id into which it is supposed to
  // be based on the partition boundaries that we have defined.
  EXPECT_EQ(1u, partition_scheme_header->getPartitionId({ TypedValue(INT64_C(0)) }));
  EXPECT_EQ(2u, partition_scheme_header->getPartitionId({ TypedValue(INT64_C(13456)) }));
  EXPECT_EQ(3u, partition_scheme_header->getPartitionId({ TypedValue(INT64_C(20000)) }));
  EXPECT_EQ(3u, partition_scheme_header->getPartitionId({ TypedValue(INT64_C(300123)) }));
  EXPECT_EQ(0u,
            partition_scheme_header->getPartitionId({ TypedValue(INT64_C(-400000)) }));
  EXPECT_EQ(2u, partition_scheme_header->getPartitionId({ TypedValue(INT64_C(15123)) }));
  EXPECT_EQ(1u, partition_scheme_header->getPartitionId({ TypedValue(INT64_C(6012)) }));
  EXPECT_EQ(0u,
            partition_scheme_header->getPartitionId({ TypedValue(INT64_C(-7000000)) }));
}

TEST(PartitionSchemeHeaderTest, FloatRangePartitionSchemeHeaderTest) {
  vector<PartitionSchemeHeader::PartitionValues> partition_ranges;
  // Partition boundaries are 0.0, 10.0, 20.0
  for (int i = 0; i < 3; ++i) {
    partition_ranges.push_back({ TypedValue(i * 10.0f) });
  }
  std::unique_ptr<PartitionSchemeHeader> partition_scheme_header(
      new RangePartitionSchemeHeader(4, { 0 }, { &TypeFactory::GetType(kFloat) }, move(partition_ranges)));
  EXPECT_EQ(4u, partition_scheme_header->getNumPartitions());
  EXPECT_EQ(0, partition_scheme_header->getPartitionAttributeIds().front());
  // Check if the partition id returned by the Range Partition Scheme for
  // floats is the same as the partition id into which it is supposed to
  // be based on the partition boundaries that we have defined.
  EXPECT_EQ(1u, partition_scheme_header->getPartitionId({ TypedValue(0.1f) }));
  EXPECT_EQ(2u, partition_scheme_header->getPartitionId({ TypedValue(10.00000000f) }));
  EXPECT_EQ(3u, partition_scheme_header->getPartitionId({ TypedValue(20.23f) }));
  EXPECT_EQ(3u, partition_scheme_header->getPartitionId({ TypedValue(30.56f) }));
  EXPECT_EQ(0u, partition_scheme_header->getPartitionId({ TypedValue(-4.5f) }));
  EXPECT_EQ(2u, partition_scheme_header->getPartitionId({ TypedValue(15.034f) }));
  EXPECT_EQ(1u, partition_scheme_header->getPartitionId({ TypedValue(6.987f) }));
  EXPECT_EQ(0u, partition_scheme_header->getPartitionId({ TypedValue(-70.384f) }));
}

TEST(PartitionSchemeHeaderTest, DoubleRangePartitionSchemeHeaderTest) {
  vector<PartitionSchemeHeader::PartitionValues> partition_ranges;
  // Partition boundaries are 0.00000, 10.00000, 20.00000
  for (int i = 0; i < 3; ++i) {
    partition_ranges.push_back({ TypedValue(i * 10.00000) });
  }
  std::unique_ptr<PartitionSchemeHeader> partition_scheme_header(
      new RangePartitionSchemeHeader(4, { 0 }, { &TypeFactory::GetType(kDouble) }, move(partition_ranges)));
  EXPECT_EQ(4u, partition_scheme_header->getNumPartitions());
  EXPECT_EQ(0, partition_scheme_header->getPartitionAttributeIds().front());
  // Check if the partition id returned by the Range Partition Scheme for
  // doubles is the same as the partition id into which it is supposed to
  // be based on the partition boundaries that we have defined.
  EXPECT_EQ(1u, partition_scheme_header->getPartitionId({ TypedValue(0.1897438974) }));
  EXPECT_EQ(2u,
            partition_scheme_header->getPartitionId({ TypedValue(10.00000000287489) }));
  EXPECT_EQ(3u,
            partition_scheme_header->getPartitionId({ TypedValue(20.23249859403750) }));
  EXPECT_EQ(3u, partition_scheme_header->getPartitionId({ TypedValue(30.567866347563) }));
  EXPECT_EQ(0u,
            partition_scheme_header->getPartitionId({ TypedValue(-4.57583978935689) }));
  EXPECT_EQ(2u,
            partition_scheme_header->getPartitionId({ TypedValue(15.034248758978936) }));
  EXPECT_EQ(1u, partition_scheme_header->getPartitionId({ TypedValue(6.98792489) }));
  EXPECT_EQ(
      0u, partition_scheme_header->getPartitionId({ TypedValue(-70.38454985893768738) }));
}

TEST(PartitionSchemeHeaderTest, CharacterRangePartitionSchemeHeaderTest) {
  vector<PartitionSchemeHeader::PartitionValues> partition_ranges;
  // Partition boundaries are the following 3 characters.
  const char *kRangeBoundaryStrings[] = {"don", "hippo", "pattasu"};
  const size_t num_boundaries = sizeof(kRangeBoundaryStrings) / sizeof(kRangeBoundaryStrings[0]);
  for (size_t i = 0; i < num_boundaries; ++i) {
    partition_ranges.push_back(
        { TypedValue(kChar, kRangeBoundaryStrings[i], std::strlen(kRangeBoundaryStrings[i]) + 1) });
  }
  std::unique_ptr<PartitionSchemeHeader> partition_scheme_header(
      new RangePartitionSchemeHeader(4, { 0 }, { &TypeFactory::GetType(kChar, 20, false) }, move(partition_ranges)));
  EXPECT_EQ(4u, partition_scheme_header->getNumPartitions());
  EXPECT_EQ(0, partition_scheme_header->getPartitionAttributeIds().front());
  const char *kSampleStrings[] = {"amma",
                                  "ganesh",
                                  "e",
                                  "imo",
                                  "master",
                                  "pathetic",
                                  "turing",
                                  "wentao",
                                  "dog",
                                  "zebra"};
  const partition_id kExpectedPartitions[] = {0, 1, 1, 2, 2, 2, 3, 3, 0, 3};
  const size_t num_strings = sizeof(kExpectedPartitions) / sizeof(kExpectedPartitions[0]);
  // Check if the partition id returned by the Range Partition Scheme for
  // characters is the same as the partition id into which it is supposed to
  // be based on the partition boundaries that we have defined.
  for (size_t i = 0; i < num_strings; ++i) {
    EXPECT_EQ(
        kExpectedPartitions[i],
        partition_scheme_header->getPartitionId({ TypedValue(
            kChar, kSampleStrings[i], std::strlen(kSampleStrings[i]) + 1) }));
  }
}

TEST(PartitionSchemeHeaderTest, VarCharRangePartitionSchemeHeaderTest) {
  vector<PartitionSchemeHeader::PartitionValues> partition_ranges;
  // Partition boundaries are the following 3 strings.
  const char *kRangeBoundaryStrings[] = { "elephant", "jamaica", "zorgonz"};
  const size_t num_boundaries = sizeof(kRangeBoundaryStrings) / sizeof(kRangeBoundaryStrings[0]);
  for (size_t i = 0; i < num_boundaries; ++i) {
    partition_ranges.push_back(
        { TypedValue(kVarChar, kRangeBoundaryStrings[i], std::strlen(kRangeBoundaryStrings[i]) + 1) });
  }

  std::unique_ptr<PartitionSchemeHeader> partition_scheme_header(
      new RangePartitionSchemeHeader(4, { 0 }, { &TypeFactory::GetType(kVarChar, 20, false) }, move(partition_ranges)));
  EXPECT_EQ(4u, partition_scheme_header->getNumPartitions());
  EXPECT_EQ(0, partition_scheme_header->getPartitionAttributeIds().front());
  const char *kSampleStrings[] = {"apple",
                                  "halloween",
                                  "mango",
                                  "turkey",
                                  "elephant",
                                  "sathyam",
                                  "zyxw",
                                  "zorgonz"};
  const partition_id kExpectedPartitions[] = {0, 1, 2, 2, 1, 2, 3, 3};
  const size_t num_strings = sizeof(kExpectedPartitions) / sizeof(kExpectedPartitions[0]);
  // Check if the partition id returned by the Range Partition Scheme for
  // variable length strings is the same as the partition id into which it
  // is supposed to be based on the partition boundaries that we have defined.
  for (size_t i = 0; i < num_strings; ++i) {
    EXPECT_EQ(kExpectedPartitions[i],
              partition_scheme_header->getPartitionId(
                  { TypedValue(kVarChar, kSampleStrings[i], std::strlen(kSampleStrings[i]) + 1) }));
  }
}

TEST(PartitionSchemeHeaderTest, MultiAttributeRangePartitionSchemeHeaderTest) {
  vector<PartitionSchemeHeader::PartitionValues> partition_ranges;
  // Partition boundaries are { 0, 0.00000 }, { 10, 10.0000 }, { 20, 20.00000 }
  // Last partition can hold upto infinity.
  // First partition can hold from { -infinity, -infinity } to { 0, -2^(-1074) }.
  for (int i = 0; i < 3; ++i) {
    partition_ranges.push_back({ TypedValue(i * 10), TypedValue(i * 10.00000) });
  }

  constexpr attribute_id kPartitioningFirstAttribute = 0;
  constexpr attribute_id kPartitioningLastAttribute = 2;
  std::unique_ptr<PartitionSchemeHeader> partition_scheme_header(
      new RangePartitionSchemeHeader(4, { kPartitioningFirstAttribute, kPartitioningLastAttribute },
                                     { &TypeFactory::GetType(kInt), &TypeFactory::GetType(kDouble) },
                                     move(partition_ranges)));
  EXPECT_EQ(4u, partition_scheme_header->getNumPartitions());
  EXPECT_EQ(kPartitioningFirstAttribute, partition_scheme_header->getPartitionAttributeIds().front());
  EXPECT_EQ(kPartitioningLastAttribute, partition_scheme_header->getPartitionAttributeIds().back());

  // Check if the partition id returned by the Range Partition Scheme for
  // { int, double } is the same as the partition id into which it is supposed
  // to be based on the partition boundaries that we have defined.
  EXPECT_EQ(0u, partition_scheme_header->getPartitionId({ TypedValue(-70), TypedValue(30.567866347563) }));
  EXPECT_EQ(0u, partition_scheme_header->getPartitionId({ TypedValue(0),   TypedValue(-4.57583978935689) }));
  EXPECT_EQ(1u, partition_scheme_header->getPartitionId({ TypedValue(6),   TypedValue(15.034248758978936) }));
  EXPECT_EQ(2u, partition_scheme_header->getPartitionId({ TypedValue(10),  TypedValue(10.00000000287489) }));
  EXPECT_EQ(3u, partition_scheme_header->getPartitionId({ TypedValue(20),  TypedValue(20.23249859403750) }));
  EXPECT_EQ(3u, partition_scheme_header->getPartitionId({ TypedValue(300), TypedValue(-70.38454985893768738) }));
}

TEST(PartitionSchemeTest, AddBlocksToPartitionTest) {
  std::unique_ptr<PartitionScheme> partition_scheme(
      new PartitionScheme(new HashPartitionSchemeHeader(4, { 0 }, { kInt })));
  for (int i = 0; i < 10; ++i) {
    partition_scheme->addBlockToPartition(i, i % 4);
  }

  // Compute blocks in each partition.
  const std::vector<block_id> blocks_in_partition_zero =
      partition_scheme->getBlocksInPartition(0);
  const std::vector<block_id> blocks_in_partition_one =
      partition_scheme->getBlocksInPartition(1);
  const std::vector<block_id> blocks_in_partition_two =
      partition_scheme->getBlocksInPartition(2);
  const std::vector<block_id> blocks_in_partition_three =
      partition_scheme->getBlocksInPartition(3);

  EXPECT_EQ(4u, partition_scheme->getPartitionSchemeHeader().getNumPartitions());
  EXPECT_EQ(0, partition_scheme->getPartitionSchemeHeader().getPartitionAttributeIds().front());

  // Check if the blocks are present in the partitions that we
  // expect them to be based on where we inserted them.
  EXPECT_NE(blocks_in_partition_zero.end(),
            std::find(blocks_in_partition_zero.begin(),
                      blocks_in_partition_zero.end(),
                      0));
  EXPECT_NE(blocks_in_partition_zero.end(),
            std::find(blocks_in_partition_zero.begin(),
                      blocks_in_partition_zero.end(),
                      4));
  EXPECT_NE(blocks_in_partition_zero.end(),
            std::find(blocks_in_partition_zero.begin(),
                      blocks_in_partition_zero.end(),
                      8));
  EXPECT_NE(
      blocks_in_partition_one.end(),
      std::find(
          blocks_in_partition_one.begin(), blocks_in_partition_one.end(), 1));
  EXPECT_NE(
      blocks_in_partition_one.end(),
      std::find(
          blocks_in_partition_one.begin(), blocks_in_partition_one.end(), 5));
  EXPECT_NE(
      blocks_in_partition_one.end(),
      std::find(
          blocks_in_partition_one.begin(), blocks_in_partition_one.end(), 9));
  EXPECT_NE(
      blocks_in_partition_two.end(),
      std::find(
          blocks_in_partition_two.begin(), blocks_in_partition_two.end(), 2));
  EXPECT_NE(
      blocks_in_partition_two.end(),
      std::find(
          blocks_in_partition_two.begin(), blocks_in_partition_two.end(), 6));
  EXPECT_NE(blocks_in_partition_three.end(),
            std::find(blocks_in_partition_three.begin(),
                      blocks_in_partition_three.end(),
                      3));
  EXPECT_NE(blocks_in_partition_three.end(),
            std::find(blocks_in_partition_three.begin(),
                      blocks_in_partition_three.end(),
                      7));
  // Check if the number of blocks in a partition are as expected.
  EXPECT_EQ(3u, blocks_in_partition_zero.size());
  EXPECT_EQ(3u, blocks_in_partition_one.size());
  EXPECT_EQ(2u, blocks_in_partition_two.size());
  EXPECT_EQ(2u, blocks_in_partition_three.size());
}

TEST(PartitionSchemeTest, RemoveBlocksFromPartitionTest) {
  std::unique_ptr<PartitionScheme> partition_scheme(
      new PartitionScheme(new HashPartitionSchemeHeader(4, { 0 }, { kInt })));
  for (int i = 0; i < 10; ++i) {
    partition_scheme->addBlockToPartition(i, i % 4);
  }

  EXPECT_EQ(4u, partition_scheme->getPartitionSchemeHeader().getNumPartitions());
  EXPECT_EQ(0, partition_scheme->getPartitionSchemeHeader().getPartitionAttributeIds().front());
  // remove block 0 from partition 0
  partition_scheme->removeBlockFromPartition(0, 0);
  const std::vector<block_id> blocks_in_partition_zero =
      partition_scheme->getBlocksInPartition(0);
  // check if block 0 is removed from partition 0
  EXPECT_EQ(blocks_in_partition_zero.end(),
            std::find(blocks_in_partition_zero.begin(),
                      blocks_in_partition_zero.end(),
                      0));
  // Check if block 4 is still present in partition zero.
  EXPECT_NE(blocks_in_partition_zero.end(),
            std::find(blocks_in_partition_zero.begin(),
                      blocks_in_partition_zero.end(),
                      4));
  // Check if block 8 is still present in partition zero.
  EXPECT_NE(blocks_in_partition_zero.end(),
            std::find(blocks_in_partition_zero.begin(),
                      blocks_in_partition_zero.end(),
                      8));
  EXPECT_EQ(2u, blocks_in_partition_zero.size());

  // remove block 5 from partition 1
  partition_scheme->removeBlockFromPartition(5, 1);
  const std::vector<block_id> blocks_in_partition_one =
      partition_scheme->getBlocksInPartition(1);
  EXPECT_NE(
      blocks_in_partition_one.end(),
      std::find(
          blocks_in_partition_one.begin(), blocks_in_partition_one.end(), 1));
  // check if block 5 is not present in partition 1
  EXPECT_EQ(
      blocks_in_partition_one.end(),
      std::find(
          blocks_in_partition_one.begin(), blocks_in_partition_one.end(), 5));
  // Check if block 9 is still present in partition one.
  EXPECT_NE(
      blocks_in_partition_one.end(),
      std::find(
          blocks_in_partition_one.begin(), blocks_in_partition_one.end(), 9));
  EXPECT_EQ(2u, blocks_in_partition_one.size());

  // remove block 2 from partition 2
  partition_scheme->removeBlockFromPartition(2, 2);
  const std::vector<block_id> blocks_in_partition_two =
      partition_scheme->getBlocksInPartition(2);
  // check if block 2 is removed from partition 2
  EXPECT_EQ(
      blocks_in_partition_two.end(),
      std::find(
          blocks_in_partition_two.begin(), blocks_in_partition_two.end(), 2));
  // Check if block 6 is still present in partition two.
  EXPECT_NE(
      blocks_in_partition_two.end(),
      std::find(
          blocks_in_partition_two.begin(), blocks_in_partition_two.end(), 6));
  EXPECT_EQ(1u, blocks_in_partition_two.size());

  // remove block 7 from partition 3
  partition_scheme->removeBlockFromPartition(7, 3);
  const std::vector<block_id> blocks_in_partition_three =
      partition_scheme->getBlocksInPartition(3);
  // Check if block 3 is still present in partition three.
  EXPECT_NE(blocks_in_partition_three.end(),
            std::find(blocks_in_partition_three.begin(),
                      blocks_in_partition_three.end(),
                      3));
  // check if block 7 is removed from partition 3
  EXPECT_EQ(blocks_in_partition_three.end(),
            std::find(blocks_in_partition_three.begin(),
                      blocks_in_partition_three.end(),
                      7));
  EXPECT_EQ(1u, blocks_in_partition_three.size());
}

TEST(PartitionSchemeTest, CheckHashPartitionSchemeSerialization) {
  const std::size_t num_partitions = 4;
  std::unique_ptr<PartitionScheme> part_scheme(
      new PartitionScheme(new HashPartitionSchemeHeader(num_partitions, { 0 }, { kInt })));
  // Add some blocks to each partition.
  for (int i = 0; i < 10; ++i) {
    part_scheme->addBlockToPartition(i, i % num_partitions);
  }
  std::unique_ptr<PartitionScheme> part_scheme_from_proto;
  part_scheme_from_proto.reset(
      PartitionScheme::ReconstructFromProto(part_scheme->getProto()));

  const PartitionSchemeHeader &header = part_scheme->getPartitionSchemeHeader();
  const PartitionSchemeHeader &header_from_proto = part_scheme_from_proto->getPartitionSchemeHeader();

  // Check the partition type
  EXPECT_EQ(header.getPartitionType(),
            header_from_proto.getPartitionType());
  // Check number of partitions
  EXPECT_EQ(header.getNumPartitions(),
            header_from_proto.getNumPartitions());
  // Check the partition attribute id
  EXPECT_EQ(header.getPartitionAttributeIds(),
            header_from_proto.getPartitionAttributeIds());
  // Check the block in each partition
  for (partition_id part_id = 0; part_id < num_partitions; ++part_id) {
    // Collect the blocks from C++ Partition Scheme object.
    std::vector<block_id> blocks_in_part_scheme =
        part_scheme->getBlocksInPartition(part_id);
    // Collect the blocks from Partition Scheme's protocol buffer.
    std::vector<block_id> blocks_in_part_scheme_from_proto =
        part_scheme_from_proto->getBlocksInPartition(part_id);
    // Sort both these vector of block ids so that we can compare them.
    std::sort(blocks_in_part_scheme.begin(), blocks_in_part_scheme.end());
    std::sort(blocks_in_part_scheme_from_proto.begin(),
              blocks_in_part_scheme_from_proto.end());
    // Compare the two sorted lists to check if they are equal.
    EXPECT_EQ(blocks_in_part_scheme, blocks_in_part_scheme_from_proto);
  }
}

TEST(PartitionSchemeTest, CheckRandomPartitionSchemeSerialization) {
  const std::size_t num_partitions = 4;
  auto part_scheme = make_unique<PartitionScheme>(new RandomPartitionSchemeHeader(num_partitions));
  // Add some blocks to each partition.
  for (int i = 0; i < 10; ++i) {
    part_scheme->addBlockToPartition(i, i % num_partitions);
  }
  std::unique_ptr<PartitionScheme> part_scheme_from_proto;
  part_scheme_from_proto.reset(
      PartitionScheme::ReconstructFromProto(part_scheme->getProto()));

  const PartitionSchemeHeader &header = part_scheme->getPartitionSchemeHeader();
  const PartitionSchemeHeader &header_from_proto = part_scheme_from_proto->getPartitionSchemeHeader();

  // Check the partition type
  EXPECT_EQ(header.getPartitionType(),
            header_from_proto.getPartitionType());
  // Check number of partitions
  EXPECT_EQ(header.getNumPartitions(),
            header_from_proto.getNumPartitions());
  // Check the partition attribute id
  EXPECT_EQ(header.getPartitionAttributeIds(),
            header_from_proto.getPartitionAttributeIds());
  // Check the block in each partition
  for (partition_id part_id = 0; part_id < num_partitions; ++part_id) {
    // Collect the blocks from C++ Partition Scheme object.
    std::vector<block_id> blocks_in_part_scheme =
        part_scheme->getBlocksInPartition(part_id);
    // Collect the blocks from Partition Scheme's protocol buffer.
    std::vector<block_id> blocks_in_part_scheme_from_proto =
        part_scheme_from_proto->getBlocksInPartition(part_id);
    // Sort both these vector of block ids so that we can compare them.
    std::sort(blocks_in_part_scheme.begin(), blocks_in_part_scheme.end());
    std::sort(blocks_in_part_scheme_from_proto.begin(),
              blocks_in_part_scheme_from_proto.end());
    // Compare the two sorted lists to check if they are equal.
    EXPECT_EQ(blocks_in_part_scheme, blocks_in_part_scheme_from_proto);
  }
}

// TODO(quickstep-team): Add back CheckRangePartitionSchemeSerialization test
// due to QUICKSTEP-86.

TEST(PartitionSchemeTest, CheckBlocksInPartitionTest) {
  constexpr std::size_t kNumBlocks = 10;
  constexpr std::size_t kNumPartitions = 4;
  constexpr attribute_id kPartitioningAttribute = 0;
  // Create a partition scheme object.
  auto partition_scheme = make_unique<PartitionScheme>(
      new HashPartitionSchemeHeader(kNumPartitions, { kPartitioningAttribute }, { kInt }));
  // Add blocks to different partitions.
  for (std::size_t block_id = 0; block_id < kNumBlocks; ++block_id) {
    partition_scheme->addBlockToPartition(block_id,
                                          block_id % kNumPartitions);
  }

  const PartitionSchemeHeader &header = partition_scheme->getPartitionSchemeHeader();

  // Check the number of partitions and the partitioning attribute.
  EXPECT_EQ(kNumPartitions, header.getNumPartitions());
  EXPECT_EQ(kPartitioningAttribute, header.getPartitionAttributeIds().front());

  // Check if the blocks are correctly assigned to its partitions.
  EXPECT_EQ(0u, partition_scheme->getPartitionForBlock(0));
  EXPECT_EQ(1u, partition_scheme->getPartitionForBlock(1));
  EXPECT_EQ(2u, partition_scheme->getPartitionForBlock(2));
  EXPECT_EQ(3u, partition_scheme->getPartitionForBlock(3));
  EXPECT_EQ(0u, partition_scheme->getPartitionForBlock(4));
  EXPECT_EQ(1u, partition_scheme->getPartitionForBlock(5));
  EXPECT_EQ(2u, partition_scheme->getPartitionForBlock(6));
  EXPECT_EQ(3u, partition_scheme->getPartitionForBlock(7));
  EXPECT_EQ(0u, partition_scheme->getPartitionForBlock(8));
  EXPECT_EQ(1u, partition_scheme->getPartitionForBlock(9));

  // Block that is not present in any partition.
  EXPECT_EQ(std::numeric_limits<partition_id>::max(),
            partition_scheme->getPartitionForBlock(100));
}

}  // namespace quickstep
