/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "catalog/PartitionScheme.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/EqualComparison.hpp"

#include "gtest/gtest.h"

namespace quickstep {

TEST(PartitionSchemeTest, IntegerHashPartitionSchemeTest) {
  std::unique_ptr<PartitionScheme> partition_scheme;
  std::size_t num_partitions = 4;
  partition_scheme.reset(new HashPartitionScheme(num_partitions, 0));
  EXPECT_EQ(num_partitions, partition_scheme->getNumPartitions());
  EXPECT_EQ(0, partition_scheme->getPartitionAttributeId());
  const int kSampleInts[] = {
      0, 1, 2, 3, 400, 501, 64783970, 78437883, -2784627};
  for (int i = 0; i < 9; ++i) {
    // Check if the partition id returned by the partition scheme for
    // an integer is the same as the hash of the integer modulus the number
    // of partitions.
    EXPECT_EQ(TypedValue(kSampleInts[i]).getHash() % num_partitions,
              partition_scheme->getPartitionId(TypedValue(kSampleInts[i])));
  }
}

TEST(PartitionSchemeTest, LongHashPartitionSchemeTest) {
  std::unique_ptr<PartitionScheme> partition_scheme;
  std::size_t num_partitions = 8;
  partition_scheme.reset(new HashPartitionScheme(num_partitions, 0));
  EXPECT_EQ(num_partitions, partition_scheme->getNumPartitions());
  EXPECT_EQ(0, partition_scheme->getPartitionAttributeId());
  const int64_t kSampleLongs[] = {INT64_C(10),
                                  INT64_C(100),
                                  INT64_C(1025),
                                  INT64_C(9876543),
                                  INT64_C(-89234758937573987)};
  // Check if the partition id returned by the partition scheme for
  // a long is the same as the hash of the long number modulus the number
  // of partitions.
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(TypedValue(kSampleLongs[i]).getHash() % num_partitions,
              partition_scheme->getPartitionId(TypedValue(kSampleLongs[i])));
  }
}

TEST(PartitionSchemeTest, FloatHashPartitionSchemeTest) {
  std::unique_ptr<PartitionScheme> partition_scheme;
  std::size_t num_partitions = 5;
  partition_scheme.reset(new HashPartitionScheme(num_partitions, 0));
  EXPECT_EQ(num_partitions, partition_scheme->getNumPartitions());
  EXPECT_EQ(0, partition_scheme->getPartitionAttributeId());
  const float kSampleFloats[] = {
      285728.895680f, 924005.4989f, -8973494.37438f};
  // Check if the partition id returned by the partition scheme for
  // a float is the same as the hash of the floating point number modulus
  // the number of partitions.
  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(TypedValue(kSampleFloats[i]).getHash() % num_partitions,
              partition_scheme->getPartitionId(TypedValue(kSampleFloats[i])));
  }
}

TEST(PartitionSchemeTest, DoubleHashPartitionSchemeTest) {
  std::unique_ptr<PartitionScheme> partition_scheme;
  std::size_t num_partitions = 6;
  partition_scheme.reset(new HashPartitionScheme(num_partitions, 0));
  EXPECT_EQ(num_partitions, partition_scheme->getNumPartitions());
  EXPECT_EQ(0, partition_scheme->getPartitionAttributeId());
  const double kSampleDoubles[] = {
      1.0378, 763624.46343453, -87238497384.3187431894713};
  // Check if the partition id returned by the partition scheme for
  // a double is the same as the hash of the double modulus the number
  // of partitions.
  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(
        TypedValue(kSampleDoubles[i]).getHash() % num_partitions,
        partition_scheme->getPartitionId(TypedValue(kSampleDoubles[i])));
  }
}

TEST(PartitionSchemeTest, CharacterHashPartitionSchemeTest) {
  std::unique_ptr<PartitionScheme> partition_scheme;
  std::size_t num_partitions = 7;
  partition_scheme.reset(new HashPartitionScheme(num_partitions, 0));
  EXPECT_EQ(num_partitions, partition_scheme->getNumPartitions());
  EXPECT_EQ(0, partition_scheme->getPartitionAttributeId());
  static const char *kSampleStrings[] = {
      "a", "gerald", "ram", "3081289", "=42?", "+-/*&^%", "hello_world"};
  // Check if the partition id returned by the partition scheme for
  // characters is the same as the hash of the characters modulus the number
  // of partitions.
  for (int i = 0; i < 7; ++i) {
    EXPECT_EQ(
        TypedValue(
            kChar, kSampleStrings[i], std::strlen(kSampleStrings[i]) + 1)
                .getHash() %
            num_partitions,
        partition_scheme->getPartitionId(TypedValue(
            kChar, kSampleStrings[i], std::strlen(kSampleStrings[i]) + 1)));
  }
}

TEST(PartitionSchemeTest, VarCharHashPartitionSchemeTest) {
  std::unique_ptr<PartitionScheme> partition_scheme;
  std::size_t num_partitions = 7;
  partition_scheme.reset(new HashPartitionScheme(num_partitions, 0));
  EXPECT_EQ(num_partitions, partition_scheme->getNumPartitions());
  EXPECT_EQ(0, partition_scheme->getPartitionAttributeId());
  static const char *kSampleStrings[] = {
      "hello", "world", "1234567", "!@#$^&*", "pa345+="};
  // Check if the partition id returned by the partition scheme for
  // a variable length string is the same as the hash of the variable length string
  // modulus the number of partitions.
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(
        TypedValue(
            kVarChar, kSampleStrings[i], std::strlen(kSampleStrings[i]) + 1)
                .getHash() %
            num_partitions,
        partition_scheme->getPartitionId(
            TypedValue(kVarChar,
                       kSampleStrings[i],
                       std::strlen(kSampleStrings[i]) + 1)));
  }
}

TEST(PartitionSchemeTest, IntegerRangePartitionSchemeTest) {
  std::unique_ptr<PartitionScheme> partition_scheme;
  std::vector<TypedValue> partition_range(3);
  // Partition boundaries are 0, 10, 20.
  // Last partition can hold upto infinity.
  // First partition can hold from -infinity to -1.
  for (int i = 0; i < 3; ++i) {
    partition_range[i] = TypedValue(i * 10);
  }
  partition_scheme.reset(new RangePartitionScheme(
      TypeFactory::GetType(kInt), 4, 0, partition_range));
  EXPECT_EQ(4u, partition_scheme->getNumPartitions());
  // Check if the partition id returned by the Range Partition Scheme for
  // integers is the same as the partition id into which it is supposed to
  // be based on the partition boundaries that we have defined.
  EXPECT_EQ(0, partition_scheme->getPartitionAttributeId());
  EXPECT_EQ(1u, partition_scheme->getPartitionId(TypedValue(0)));
  EXPECT_EQ(2u, partition_scheme->getPartitionId(TypedValue(10)));
  EXPECT_EQ(3u, partition_scheme->getPartitionId(TypedValue(20)));
  EXPECT_EQ(3u, partition_scheme->getPartitionId(TypedValue(30)));
  EXPECT_EQ(0u, partition_scheme->getPartitionId(TypedValue(-4)));
  EXPECT_EQ(2u, partition_scheme->getPartitionId(TypedValue(15)));
  EXPECT_EQ(1u, partition_scheme->getPartitionId(TypedValue(6)));
  EXPECT_EQ(0u, partition_scheme->getPartitionId(TypedValue(-70)));
  EXPECT_EQ(3u, partition_scheme->getPartitionId(TypedValue(1000)));
  EXPECT_EQ(3u, partition_scheme->getPartitionId(TypedValue(20000)));
}

TEST(PartitionSchemeTest, LongRangePartitionSchemeTest) {
  std::unique_ptr<PartitionScheme> partition_scheme;
  std::vector<TypedValue> partition_range(3);
  // Partition boundaries are 0, 10000, 20000, 30000
  for (int i = 0; i < 3; ++i) {
    partition_range[i] = TypedValue(i * INT64_C(10000));
  }
  partition_scheme.reset(new RangePartitionScheme(
      TypeFactory::GetType(kLong), 4, 0, partition_range));

  EXPECT_EQ(4u, partition_scheme->getNumPartitions());
  EXPECT_EQ(0, partition_scheme->getPartitionAttributeId());
  // Check if the partition id returned by the Range Partition Scheme for
  // long numbers is the same as the partition id into which it is supposed to
  // be based on the partition boundaries that we have defined.
  EXPECT_EQ(1u, partition_scheme->getPartitionId(TypedValue(INT64_C(0))));
  EXPECT_EQ(2u, partition_scheme->getPartitionId(TypedValue(INT64_C(13456))));
  EXPECT_EQ(3u, partition_scheme->getPartitionId(TypedValue(INT64_C(20000))));
  EXPECT_EQ(3u, partition_scheme->getPartitionId(TypedValue(INT64_C(300123))));
  EXPECT_EQ(0u,
            partition_scheme->getPartitionId(TypedValue(INT64_C(-400000))));
  EXPECT_EQ(2u, partition_scheme->getPartitionId(TypedValue(INT64_C(15123))));
  EXPECT_EQ(1u, partition_scheme->getPartitionId(TypedValue(INT64_C(6012))));
  EXPECT_EQ(0u,
            partition_scheme->getPartitionId(TypedValue(INT64_C(-7000000))));
}

TEST(PartitionSchemeTest, FloatRangePartitionSchemeTest) {
  std::unique_ptr<PartitionScheme> partition_scheme;
  std::vector<TypedValue> partition_range(3);
  // Partition boundaries are 0.0, 10.0, 20.0
  for (int i = 0; i < 3; ++i) {
    partition_range[i] = TypedValue(i * 10.0f);
  }
  partition_scheme.reset(new RangePartitionScheme(
      TypeFactory::GetType(kFloat), 4, 0, partition_range));
  EXPECT_EQ(4u, partition_scheme->getNumPartitions());
  EXPECT_EQ(0, partition_scheme->getPartitionAttributeId());
  // Check if the partition id returned by the Range Partition Scheme for
  // floats is the same as the partition id into which it is supposed to
  // be based on the partition boundaries that we have defined.
  EXPECT_EQ(1u, partition_scheme->getPartitionId(TypedValue(0.1f)));
  EXPECT_EQ(2u, partition_scheme->getPartitionId(TypedValue(10.00000000f)));
  EXPECT_EQ(3u, partition_scheme->getPartitionId(TypedValue(20.23f)));
  EXPECT_EQ(3u, partition_scheme->getPartitionId(TypedValue(30.56f)));
  EXPECT_EQ(0u, partition_scheme->getPartitionId(TypedValue(-4.5f)));
  EXPECT_EQ(2u, partition_scheme->getPartitionId(TypedValue(15.034f)));
  EXPECT_EQ(1u, partition_scheme->getPartitionId(TypedValue(6.987f)));
  EXPECT_EQ(0u, partition_scheme->getPartitionId(TypedValue(-70.384f)));
}

TEST(PartitionSchemeTest, DoubleRangePartitionSchemeTest) {
  std::unique_ptr<PartitionScheme> partition_scheme;
  std::vector<TypedValue> partition_range(3);
  // Partition boundaries are 0.00000, 10.00000, 20.00000
  for (int i = 0; i < 3; ++i) {
    partition_range[i] = TypedValue(i * 10.00000);
  }
  partition_scheme.reset(new RangePartitionScheme(
      TypeFactory::GetType(kDouble), 4, 0, partition_range));
  EXPECT_EQ(4u, partition_scheme->getNumPartitions());
  EXPECT_EQ(0, partition_scheme->getPartitionAttributeId());
  // Check if the partition id returned by the Range Partition Scheme for
  // doubles is the same as the partition id into which it is supposed to
  // be based on the partition boundaries that we have defined.
  EXPECT_EQ(1u, partition_scheme->getPartitionId(TypedValue(0.1897438974)));
  EXPECT_EQ(2u,
            partition_scheme->getPartitionId(TypedValue(10.00000000287489)));
  EXPECT_EQ(3u,
            partition_scheme->getPartitionId(TypedValue(20.23249859403750)));
  EXPECT_EQ(3u, partition_scheme->getPartitionId(TypedValue(30.567866347563)));
  EXPECT_EQ(0u,
            partition_scheme->getPartitionId(TypedValue(-4.57583978935689)));
  EXPECT_EQ(2u,
            partition_scheme->getPartitionId(TypedValue(15.034248758978936)));
  EXPECT_EQ(1u, partition_scheme->getPartitionId(TypedValue(6.98792489)));
  EXPECT_EQ(
      0u, partition_scheme->getPartitionId(TypedValue(-70.38454985893768738)));
}

TEST(PartitionSchemeTest, CharacterRangePartitionSchemeTest) {
  std::unique_ptr<PartitionScheme> partition_scheme;
  std::vector<TypedValue> partition_range(3);
  // Partition boundaries are the following 3 characters.
  static const char *kRangeBoundaryStrings[] = {"don", "hippo", "pattasu"};
  for (int i = 0; i < 3; ++i) {
    partition_range[i] =
        TypedValue(kChar,
                   kRangeBoundaryStrings[i],
                   std::strlen(kRangeBoundaryStrings[i]) + 1);
  }
  partition_scheme.reset(new RangePartitionScheme(
      TypeFactory::GetType(kChar, 20, false), 4, 0, partition_range));
  EXPECT_EQ(4u, partition_scheme->getNumPartitions());
  EXPECT_EQ(0, partition_scheme->getPartitionAttributeId());
  static const char *kSampleStrings[] = {"amma",
                                         "ganesh",
                                         "e",
                                         "imo",
                                         "master",
                                         "pathetic",
                                         "turing",
                                         "wentao",
                                         "dog",
                                         "zebra"};
  partition_id kExpectedPartitions[] = {0, 1, 1, 2, 2, 2, 3, 3, 0, 3};
  // Check if the partition id returned by the Range Partition Scheme for
  // characters is the same as the partition id into which it is supposed to
  // be based on the partition boundaries that we have defined.
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(
        kExpectedPartitions[i],
        partition_scheme->getPartitionId(TypedValue(
            kChar, kSampleStrings[i], std::strlen(kSampleStrings[i]) + 1)));
  }
}

TEST(PartitionSchemeTest, VarCharRangePartitionSchemeTest) {
  std::unique_ptr<PartitionScheme> partition_scheme;
  std::vector<TypedValue> partition_range(3);
  // Partition boundaries are the following 3 strings.
  static const char *kRangeBoundaryStrings[] = {
      "elephant", "jamaica", "zorgonz"};
  for (int i = 0; i < 3; ++i) {
    partition_range[i] =
        TypedValue(kVarChar,
                   kRangeBoundaryStrings[i],
                   std::strlen(kRangeBoundaryStrings[i]) + 1);
  }

  partition_scheme.reset(new RangePartitionScheme(
      TypeFactory::GetType(kVarChar, 20, false), 4, 0, partition_range));
  EXPECT_EQ(4u, partition_scheme->getNumPartitions());
  EXPECT_EQ(0, partition_scheme->getPartitionAttributeId());
  static const char *kSampleStrings[] = {"apple",
                                         "halloween",
                                         "mango",
                                         "turkey",
                                         "elephant",
                                         "sathyam",
                                         "zyxw",
                                         "zorgonz"};
  const partition_id kExpectedPartitions[] = {0, 1, 2, 2, 1, 2, 3, 3};
  // Check if the partition id returned by the Range Partition Scheme for
  // variable length strings is the same as the partition id into which it
  // is supposed to be based on the partition boundaries that we have defined.
  for (int i = 0; i < 8; ++i) {
    EXPECT_EQ(kExpectedPartitions[i],
              partition_scheme->getPartitionId(
                  TypedValue(kVarChar,
                             kSampleStrings[i],
                             std::strlen(kSampleStrings[i]) + 1)));
  }
}

TEST(PartitionSchemeTest, AddBlocksToPartitionTest) {
  std::unique_ptr<PartitionScheme> partition_scheme;
  partition_scheme.reset(new HashPartitionScheme(4, 0));
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

  EXPECT_EQ(4u, partition_scheme->getNumPartitions());
  EXPECT_EQ(0, partition_scheme->getPartitionAttributeId());

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
  std::unique_ptr<PartitionScheme> partition_scheme;
  partition_scheme.reset(new HashPartitionScheme(4, 0));
  for (int i = 0; i < 10; ++i) {
    partition_scheme->addBlockToPartition(i, i % 4);
  }

  EXPECT_EQ(4u, partition_scheme->getNumPartitions());
  EXPECT_EQ(0, partition_scheme->getPartitionAttributeId());
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
  std::unique_ptr<PartitionScheme> part_scheme;
  std::size_t num_partitions = 4;
  part_scheme.reset(new HashPartitionScheme(num_partitions, 0));
  // Add some blocks to each partition.
  for (int i = 0; i < 10; ++i) {
    part_scheme->addBlockToPartition(i, i % num_partitions);
  }
  std::unique_ptr<PartitionScheme> part_scheme_from_proto;
  part_scheme_from_proto.reset(
      HashPartitionScheme::DeserializePartitionScheme(
          part_scheme->getProto(), TypeFactory::GetType(kInt)));
  // Check number of partitions
  EXPECT_EQ(part_scheme->getNumPartitions(),
            part_scheme_from_proto->getNumPartitions());
  // Check the partition attribute id
  EXPECT_EQ(part_scheme->getPartitionAttributeId(),
            part_scheme_from_proto->getPartitionAttributeId());
  // Check the partition type
  EXPECT_EQ(part_scheme->getPartitionType(),
            part_scheme_from_proto->getPartitionType());
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

TEST(PartitionSchemeTest, CheckRangePartitionSchemeSerialization) {
  std::unique_ptr<PartitionScheme> part_scheme;
  std::size_t num_partitions = 4;
  std::vector<TypedValue> partition_range(num_partitions - 1);
  // Partition boundaries are 0, 10, 20.
  // Last partition can hold upto infinity.
  // First partition can hold from -infinity to -1.
  for (std::size_t i = 0; i < num_partitions - 1; ++i) {
    partition_range[i] = TypedValue(static_cast<int>(i * 10));
  }
  part_scheme.reset(new RangePartitionScheme(
      TypeFactory::GetType(kInt), num_partitions, 0, partition_range));
  for (int i = 0; i < 10; ++i) {
    part_scheme->addBlockToPartition(i * 5, i % num_partitions);
  }
  std::unique_ptr<PartitionScheme> part_scheme_from_proto;

  part_scheme_from_proto.reset(
      RangePartitionScheme::DeserializePartitionScheme(
          part_scheme->getProto(), TypeFactory::GetType(kInt)));
  // Check number of partitions
  EXPECT_EQ(part_scheme->getNumPartitions(),
            part_scheme_from_proto->getNumPartitions());

  // Check the partition attribute id
  EXPECT_EQ(part_scheme->getPartitionAttributeId(),
            part_scheme_from_proto->getPartitionAttributeId());

  // Check the partition type
  EXPECT_EQ(part_scheme->getPartitionType(),
            part_scheme_from_proto->getPartitionType());

  // Check the partition range boundaries' size.
  std::vector<TypedValue> range_boundaries_part_scheme =
      static_cast<RangePartitionScheme *>(part_scheme.get())
          ->getPartitionRangeBoundaries();
  std::vector<TypedValue> range_boundaries_part_scheme_from_proto =
      static_cast<RangePartitionScheme *>(part_scheme_from_proto.get())
          ->getPartitionRangeBoundaries();
  EXPECT_EQ(range_boundaries_part_scheme.size(),
            range_boundaries_part_scheme_from_proto.size());

  // Check the partition range boundaries' values.
  const Comparison &equal_comparison_op(EqualComparison::Instance());
  std::unique_ptr<UncheckedComparator> equal_unchecked_comparator;
  equal_unchecked_comparator.reset(
      equal_comparison_op.makeUncheckedComparatorForTypes(
          TypeFactory::GetType(kInt), TypeFactory::GetType(kInt)));
  for (std::size_t i = 0; i < num_partitions - 1; ++i) {
    EXPECT_TRUE(equal_unchecked_comparator->compareTypedValues(
        range_boundaries_part_scheme[i],
        range_boundaries_part_scheme_from_proto[i]));
  }

  // Check the blocks in each partition from both the Partition Scheme's
  // C++ object and protocol buffer.
  for (partition_id part_id = 0; part_id < num_partitions; ++part_id) {
    std::vector<block_id> blocks_in_part_scheme =
        part_scheme->getBlocksInPartition(part_id);
    std::vector<block_id> blocks_in_part_scheme_from_proto =
        part_scheme_from_proto->getBlocksInPartition(part_id);
    std::sort(blocks_in_part_scheme.begin(), blocks_in_part_scheme.end());
    std::sort(blocks_in_part_scheme_from_proto.begin(),
              blocks_in_part_scheme_from_proto.end());
    EXPECT_EQ(blocks_in_part_scheme, blocks_in_part_scheme_from_proto);
  }
}

TEST(PartitionSchemeTest, CheckBlocksInPartitionTest) {
  std::unique_ptr<PartitionScheme> partition_scheme;
  constexpr std::size_t kNumBlocks = 10;
  constexpr std::size_t kNumPartitions = 4;
  constexpr attribute_id kPartitioningAttribute = 0;
  // Create a partition scheme object.
  partition_scheme.reset(
      new HashPartitionScheme(kNumPartitions, kPartitioningAttribute));
  // Add blocks to different partitions.
  for (std::size_t block_id = 0; block_id < kNumBlocks; ++block_id) {
    partition_scheme->addBlockToPartition(block_id,
                                          block_id % kNumPartitions);
  }

  // Check the number of partitions and the partitioning attribute.
  EXPECT_EQ(kNumPartitions, partition_scheme->getNumPartitions());
  EXPECT_EQ(kPartitioningAttribute,
            partition_scheme->getPartitionAttributeId());

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
