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

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <vector>

#include "utility/BitVector.hpp"

#include "gtest/gtest.h"

using std::free;
using std::malloc;
using std::size_t;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::vector;

namespace quickstep {

template <typename BitVectorType>
class BitVectorTest : public ::testing::Test {
 protected:
  virtual void TearDown() {
    for (vector<void*>::iterator it = allocated_chunks_.begin();
         it != allocated_chunks_.end();
         ++it) {
      free(*it);
    }
    allocated_chunks_.clear();
  }

  BitVectorType* createBitVector(const size_t bit_size) {
    void *bytes = malloc(BitVectorType::BytesNeeded(bit_size));
    allocated_chunks_.push_back(bytes);
    return new BitVectorType(bytes, bit_size);
  }

  void runClearTest(const size_t bit_size) {
    std::unique_ptr<BitVectorType> bit_vector(createBitVector(bit_size));
    bit_vector->clear();
    for (size_t i = 0; i < bit_size; ++i) {
      EXPECT_FALSE(bit_vector->getBit(i));
    }
    EXPECT_EQ(0u, bit_vector->onesCount());
    EXPECT_EQ(0u, bit_vector->firstZero());
    EXPECT_EQ(bit_size / 2, bit_vector->firstZero(bit_size / 2));
    EXPECT_EQ(bit_size - 1, bit_vector->lastZero());
    EXPECT_EQ((bit_size / 2) - 1, bit_vector->lastZero(bit_size / 2));
    EXPECT_EQ(bit_size, bit_vector->firstOne());
    EXPECT_EQ(bit_size, bit_vector->firstOne(bit_size / 2));
    EXPECT_EQ(bit_size, bit_vector->lastOne());
    EXPECT_EQ(bit_size, bit_vector->lastOne(bit_size / 2));
  }

  void runFlipBitsTest(const size_t bit_size) {
    std::unique_ptr<BitVectorType> bit_vector(createBitVector(bit_size));
    bit_vector->clear();

    bit_vector->setBit(0, true);
    bit_vector->setBit(bit_size / 2, true);
    bit_vector->setBit(bit_size - 1, true);
    bit_vector->flipBits();

    for (size_t i = 0; i < bit_size; ++i) {
      if ((i == 0) || (i == bit_size / 2) || (i == bit_size - 1)) {
        EXPECT_FALSE(bit_vector->getBit(i));
      } else {
        EXPECT_TRUE(bit_vector->getBit(i));
      }
    }

    EXPECT_EQ(bit_size - 3, bit_vector->onesCount());

    EXPECT_EQ(0u, bit_vector->firstZero());
    EXPECT_EQ(bit_size / 2, bit_vector->firstZero(1));
    EXPECT_EQ(bit_size - 1, bit_vector->firstZero((bit_size / 2) + 1));

    EXPECT_EQ(1u, bit_vector->firstOne());
    EXPECT_EQ(2u, bit_vector->firstOne(2));
    EXPECT_EQ((bit_size / 2) + 1, bit_vector->firstOne(bit_size / 2));
    EXPECT_EQ(bit_size, bit_vector->firstOne(bit_size - 1));

    EXPECT_EQ(bit_size - 1, bit_vector->lastZero());
    EXPECT_EQ(bit_size / 2, bit_vector->lastZero(bit_size - 1));
    EXPECT_EQ(0u, bit_vector->lastZero(bit_size / 2));

    EXPECT_EQ(bit_size - 2, bit_vector->lastOne());
    EXPECT_EQ(bit_size - 3, bit_vector->lastOne(bit_size - 2));
    EXPECT_EQ((bit_size / 2) - 1, bit_vector->lastOne(bit_size / 2));
    EXPECT_EQ(bit_size, bit_vector->lastOne(1));
  }

  void runIntersectWithTest(const size_t bit_size) {
    std::unique_ptr<BitVectorType> bit_vector(createBitVector(bit_size));
    bit_vector->clear();
    bit_vector->setBit(0, true);
    bit_vector->setBit(bit_size / 2, true);
    bit_vector->setBit(bit_size - 1, true);

    std::unique_ptr<BitVectorType> other_bit_vector(createBitVector(bit_size));
    other_bit_vector->clear();
    other_bit_vector->setBit(1, true);
    other_bit_vector->setBit(bit_size / 2, true);
    other_bit_vector->setBit(bit_size - 2, true);

    bit_vector->intersectWith(*other_bit_vector);

    for (size_t i = 0; i < bit_size; ++i) {
      if (i == bit_size / 2) {
        EXPECT_TRUE(bit_vector->getBit(i));
      } else {
        EXPECT_FALSE(bit_vector->getBit(i));
      }
    }

    // '*other_bit_vector' should be unmodified
    for (size_t i = 0; i < bit_size; ++i) {
      if ((i == 1) || (i == bit_size / 2) || (i == bit_size - 2)) {
        EXPECT_TRUE(other_bit_vector->getBit(i));
      } else {
        EXPECT_FALSE(other_bit_vector->getBit(i));
      }
    }
  }

  void runUnionWithTest(const size_t bit_size) {
    std::unique_ptr<BitVectorType> bit_vector(createBitVector(bit_size));
    bit_vector->clear();
    bit_vector->setBit(0, true);
    bit_vector->setBit(bit_size / 2, true);
    bit_vector->setBit(bit_size - 1, true);

    std::unique_ptr<BitVectorType> other_bit_vector(createBitVector(bit_size));
    other_bit_vector->clear();
    other_bit_vector->setBit(1, true);
    other_bit_vector->setBit(bit_size / 2, true);
    other_bit_vector->setBit(bit_size - 2, true);

    bit_vector->unionWith(*other_bit_vector);

    for (size_t i = 0; i < bit_size; ++i) {
      if ((i == 0) || (i == 1) || (i == bit_size / 2)
          || (i == bit_size - 2) || (i == bit_size - 1)) {
        EXPECT_TRUE(bit_vector->getBit(i));
      } else {
        EXPECT_FALSE(bit_vector->getBit(i));
      }
    }

    // '*other_bit_vector' should be unmodified
    for (size_t i = 0; i < bit_size; ++i) {
      if ((i == 1) || (i == bit_size / 2) || (i == bit_size - 2)) {
        EXPECT_TRUE(other_bit_vector->getBit(i));
      } else {
        EXPECT_FALSE(other_bit_vector->getBit(i));
      }
    }
  }

  void runZeroLengthTest(BitVectorType *bit_vector) {
    BitVectorType other_zero_length_bit_vector(0);

    // Check initial metadata.
    CheckZeroLengthMetaData(*bit_vector);

    // Do some no-op modifying operations and recheck metadata after each.
    bit_vector->clear();
    CheckZeroLengthMetaData(*bit_vector);
    bit_vector->flipBits();
    CheckZeroLengthMetaData(*bit_vector);

    // Some compilers may aggressively propagate constants through inlined
    // functions (especially for RELEASE builds), eventually resulting in a
    // call to std::memset() with a constant zero-length parameter
    // (effectively a no-op). Some C standard library implementations
    // (including glibc) have memset declared with an attribute to warn about a
    // constant zero for the length parameter. Storing the zero-length constant
    // in a variable marked volatile prevents constant propagation and
    // supresses warnings for memset() when we intentionally test
    // BitVector::setBitRange() with length 0.
    const volatile std::size_t kZeroLen = 0;
    bit_vector->setBitRange(0, kZeroLen, false);
    CheckZeroLengthMetaData(*bit_vector);
    bit_vector->setBitRange(0, kZeroLen, true);
    CheckZeroLengthMetaData(*bit_vector);
    bit_vector->assignFrom(other_zero_length_bit_vector);
    CheckZeroLengthMetaData(*bit_vector);
    bit_vector->intersectWith(other_zero_length_bit_vector);
    CheckZeroLengthMetaData(*bit_vector);
    bit_vector->unionWith(other_zero_length_bit_vector);
    CheckZeroLengthMetaData(*bit_vector);
  }

  static void CheckZeroLengthMetaData(const BitVectorType &bit_vector) {
    EXPECT_EQ(0u, bit_vector.size());
    EXPECT_FALSE(bit_vector.any());
    EXPECT_TRUE(bit_vector.all());
    EXPECT_EQ(0u, bit_vector.onesCount());
    EXPECT_EQ(0u, bit_vector.firstOne());
    EXPECT_EQ(0u, bit_vector.firstZero());
    EXPECT_EQ(0u, bit_vector.lastOne());
    EXPECT_EQ(0u, bit_vector.lastZero());
  }

  static const size_t kSmallBitSize = 8;
  static const size_t kBigBitSize = 160;
  static const size_t kBiggerBitSize = 1024;
  vector<void*> allocated_chunks_;
};

typedef ::testing::Types<BitVector<false>, BitVector<true>> TestTypes;
TYPED_TEST_CASE(BitVectorTest, TestTypes);

// Static const members must be defined out-of-line to work with EXPECT_*
// macros.
template <typename BitVectorType> const size_t BitVectorTest<BitVectorType>::kSmallBitSize;
template <typename BitVectorType> const size_t BitVectorTest<BitVectorType>::kBigBitSize;
template <typename BitVectorType> const size_t BitVectorTest<BitVectorType>::kBiggerBitSize;

TYPED_TEST(BitVectorTest, BytesNeededTest) {
  if (TypeParam::kShortVersionEnabled) {
    EXPECT_EQ(sizeof(uint8_t), TypeParam::BytesNeeded(7));
    EXPECT_EQ(sizeof(uint8_t), TypeParam::BytesNeeded(8));
    EXPECT_EQ(sizeof(uint16_t), TypeParam::BytesNeeded(9));
    EXPECT_EQ(sizeof(uint16_t), TypeParam::BytesNeeded(15));
    EXPECT_EQ(sizeof(uint16_t), TypeParam::BytesNeeded(16));
    EXPECT_EQ(sizeof(uint32_t), TypeParam::BytesNeeded(17));
    EXPECT_EQ(sizeof(uint32_t), TypeParam::BytesNeeded(31));
    EXPECT_EQ(sizeof(uint32_t), TypeParam::BytesNeeded(32));
  } else {
    EXPECT_EQ(sizeof(size_t), TypeParam::BytesNeeded(7));
    EXPECT_EQ(sizeof(size_t), TypeParam::BytesNeeded(8));
    EXPECT_EQ(sizeof(size_t), TypeParam::BytesNeeded(9));
    EXPECT_EQ(sizeof(size_t), TypeParam::BytesNeeded(15));
    EXPECT_EQ(sizeof(size_t), TypeParam::BytesNeeded(16));
    EXPECT_EQ(sizeof(size_t), TypeParam::BytesNeeded(17));
    EXPECT_EQ(sizeof(size_t), TypeParam::BytesNeeded(31));
    EXPECT_EQ(sizeof(size_t), TypeParam::BytesNeeded(32));
  }

  if (sizeof(size_t) == 4) {
    // 32-bit build.
    EXPECT_EQ(sizeof(size_t) * 2, TypeParam::BytesNeeded(33));
    EXPECT_EQ(sizeof(size_t) * 2, TypeParam::BytesNeeded(63));
    EXPECT_EQ(sizeof(size_t) * 2, TypeParam::BytesNeeded(64));
    EXPECT_EQ(sizeof(size_t) * 3, TypeParam::BytesNeeded(65));
  } else if (sizeof(size_t) == 8) {
    // 64-bit build.
    EXPECT_EQ(sizeof(size_t), TypeParam::BytesNeeded(33));
    EXPECT_EQ(sizeof(size_t), TypeParam::BytesNeeded(63));
    EXPECT_EQ(sizeof(size_t), TypeParam::BytesNeeded(64));
    EXPECT_EQ(sizeof(size_t) * 2, TypeParam::BytesNeeded(65));
  } else {
    // The exotic far future.
  }
}

TYPED_TEST(BitVectorTest, ByteClearTest) {
  TestFixture::runClearTest(7);
  TestFixture::runClearTest(8);
}

TYPED_TEST(BitVectorTest, ShortClearTest) {
  TestFixture::runClearTest(9);
  TestFixture::runClearTest(15);
  TestFixture::runClearTest(16);
}

TYPED_TEST(BitVectorTest, WordClearTest) {
  TestFixture::runClearTest(17);
  TestFixture::runClearTest(31);
  TestFixture::runClearTest(32);
}

TYPED_TEST(BitVectorTest, RegularClearTest) {
  TestFixture::runClearTest(33);
  TestFixture::runClearTest(63);
  TestFixture::runClearTest(64);
  TestFixture::runClearTest(65);
  TestFixture::runClearTest(200);
}

TYPED_TEST(BitVectorTest, SetAndGetTest) {
  std::unique_ptr<TypeParam> small_bit_vector(this->createBitVector(TestFixture::kSmallBitSize));
  small_bit_vector->clear();

  small_bit_vector->setBit(2, true);
  small_bit_vector->setBit(3, true);
  small_bit_vector->setBit(4, true);
  small_bit_vector->setBit(3, false);

  for (size_t i = 0; i < TestFixture::kSmallBitSize; ++i) {
    if ((i == 2) || (i == 4)) {
      EXPECT_TRUE(small_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(small_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(2u, small_bit_vector->onesCount());
  EXPECT_EQ(0u, small_bit_vector->firstZero());
  EXPECT_EQ(1u, small_bit_vector->firstZero(1));
  EXPECT_EQ(3u, small_bit_vector->firstZero(2));
  EXPECT_EQ(2u, small_bit_vector->firstOne());
  EXPECT_EQ(2u, small_bit_vector->firstOne(2));
  EXPECT_EQ(4u, small_bit_vector->firstOne(3));
  EXPECT_EQ(TestFixture::kSmallBitSize - 1, small_bit_vector->lastZero());
  EXPECT_EQ(0u, small_bit_vector->lastZero(1));
  EXPECT_EQ(1u, small_bit_vector->lastZero(3));
  EXPECT_EQ(3u, small_bit_vector->lastZero(4));
  EXPECT_EQ(4u, small_bit_vector->lastOne());
  EXPECT_EQ(TestFixture::kSmallBitSize, small_bit_vector->lastOne(2));
  EXPECT_EQ(2u, small_bit_vector->lastOne(3));

  // Set all the bits.
  for (size_t i = 0; i < TestFixture::kSmallBitSize; ++i) {
    small_bit_vector->setBit(i, true);
  }
  EXPECT_EQ(TestFixture::kSmallBitSize, small_bit_vector->onesCount());
  EXPECT_EQ(TestFixture::kSmallBitSize, small_bit_vector->firstZero());
  EXPECT_EQ(0u, small_bit_vector->firstOne());
  EXPECT_EQ(TestFixture::kSmallBitSize / 2,
            small_bit_vector->firstOne(TestFixture::kSmallBitSize / 2));
  EXPECT_EQ(TestFixture::kSmallBitSize, small_bit_vector->lastZero());
  EXPECT_EQ(TestFixture::kSmallBitSize - 1, small_bit_vector->lastOne());
  EXPECT_EQ((TestFixture::kSmallBitSize / 2) - 1,
            small_bit_vector->lastOne(TestFixture::kSmallBitSize / 2));
  for (size_t i = 0; i < TestFixture::kSmallBitSize; ++i) {
    EXPECT_TRUE(small_bit_vector->getBit(i));
    EXPECT_EQ(i, small_bit_vector->firstOne(i));
    if (i > 0) {
      EXPECT_EQ(i - 1, small_bit_vector->lastOne(i));
    }
  }


  std::unique_ptr<TypeParam> big_bit_vector(this->createBitVector(TestFixture::kBigBitSize));
  big_bit_vector->clear();

  // Set some bits, particularly around potential boundaries between size_t strides.
  big_bit_vector->setBit(0);
  big_bit_vector->setBit(5);
  big_bit_vector->setBit(31);
  big_bit_vector->setBit(32);
  big_bit_vector->setBit(63);
  big_bit_vector->setBit(64);
  big_bit_vector->setBit(127);
  big_bit_vector->setBit(128);

  for (size_t i = 0; i < TestFixture::kBigBitSize; ++i) {
    if ((i == 0) || (i == 5) || (i == 31) || (i == 32) || (i == 63) || (i == 64) || (i == 127) || (i == 128)) {
      EXPECT_TRUE(big_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(big_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(8u, big_bit_vector->onesCount());
  EXPECT_EQ(1u, big_bit_vector->firstZero());
  EXPECT_EQ(2u, big_bit_vector->firstZero(2));
  EXPECT_EQ(0u, big_bit_vector->firstOne());
  EXPECT_EQ(5u, big_bit_vector->firstOne(1));
  EXPECT_EQ(TestFixture::kBigBitSize - 1, big_bit_vector->lastZero());
  EXPECT_EQ(126u, big_bit_vector->lastZero(129));
  EXPECT_EQ(TestFixture::kBigBitSize, big_bit_vector->lastZero(1));
  EXPECT_EQ(128u, big_bit_vector->lastOne());
  EXPECT_EQ(127u, big_bit_vector->lastOne(128));
  EXPECT_EQ(64u, big_bit_vector->lastOne(127));

  // Unset bits and make sure things still look right.
  // TODO(chasseur): More checks for lastZero() and lastOne().
  big_bit_vector->setBit(0, false);
  EXPECT_EQ(7u, big_bit_vector->onesCount());
  EXPECT_EQ(0u, big_bit_vector->firstZero());
  EXPECT_EQ(1u, big_bit_vector->firstZero(1));
  EXPECT_EQ(6u, big_bit_vector->firstZero(5));
  EXPECT_EQ(5u, big_bit_vector->firstOne());
  EXPECT_EQ(31u, big_bit_vector->firstOne(6));

  big_bit_vector->setBit(5, false);
  EXPECT_EQ(6u, big_bit_vector->onesCount());
  EXPECT_EQ(0u, big_bit_vector->firstZero());
  EXPECT_EQ(1u, big_bit_vector->firstZero(1));
  EXPECT_EQ(33u, big_bit_vector->firstZero(31));
  EXPECT_EQ(31u, big_bit_vector->firstOne());
  EXPECT_EQ(32u, big_bit_vector->firstOne(32));

  big_bit_vector->setBit(31, false);
  EXPECT_EQ(5u, big_bit_vector->onesCount());
  EXPECT_EQ(0u, big_bit_vector->firstZero());
  EXPECT_EQ(1u, big_bit_vector->firstZero(1));
  EXPECT_EQ(33u, big_bit_vector->firstZero(32));
  EXPECT_EQ(32u, big_bit_vector->firstOne());
  EXPECT_EQ(63u, big_bit_vector->firstOne(33));

  big_bit_vector->setBit(32, false);
  EXPECT_EQ(4u, big_bit_vector->onesCount());
  EXPECT_EQ(0u, big_bit_vector->firstZero());
  EXPECT_EQ(1u, big_bit_vector->firstZero(1));
  EXPECT_EQ(65u, big_bit_vector->firstZero(63));
  EXPECT_EQ(63u, big_bit_vector->firstOne());
  EXPECT_EQ(64u, big_bit_vector->firstOne(64));

  big_bit_vector->setBit(63, false);
  EXPECT_EQ(3u, big_bit_vector->onesCount());
  EXPECT_EQ(0u, big_bit_vector->firstZero());
  EXPECT_EQ(1u, big_bit_vector->firstZero(1));
  EXPECT_EQ(65u, big_bit_vector->firstZero(64));
  EXPECT_EQ(64u, big_bit_vector->firstOne());
  EXPECT_EQ(127u, big_bit_vector->firstOne(65));

  big_bit_vector->setBit(64, false);
  EXPECT_EQ(2u, big_bit_vector->onesCount());
  EXPECT_EQ(0u, big_bit_vector->firstZero());
  EXPECT_EQ(1u, big_bit_vector->firstZero(1));
  EXPECT_EQ(129u, big_bit_vector->firstZero(127));
  EXPECT_EQ(127u, big_bit_vector->firstOne());
  EXPECT_EQ(128u, big_bit_vector->firstOne(128));

  big_bit_vector->setBit(127, false);
  EXPECT_EQ(1u, big_bit_vector->onesCount());
  EXPECT_EQ(0u, big_bit_vector->firstZero());
  EXPECT_EQ(1u, big_bit_vector->firstZero(1));
  EXPECT_EQ(129u, big_bit_vector->firstZero(128));
  EXPECT_EQ(128u, big_bit_vector->firstOne());
  EXPECT_EQ(TestFixture::kBigBitSize, big_bit_vector->firstOne(129));

  big_bit_vector->setBit(128, false);
  EXPECT_EQ(0u, big_bit_vector->onesCount());
  EXPECT_EQ(0u, big_bit_vector->firstZero());
  EXPECT_EQ(TestFixture::kBigBitSize, big_bit_vector->firstOne());

  for (size_t i = 0; i < TestFixture::kBigBitSize; ++i) {
    EXPECT_FALSE(big_bit_vector->getBit(i));
  }

  // Set all the bits.
  for (size_t i = 0; i < TestFixture::kBigBitSize; ++i) {
    big_bit_vector->setBit(i);
  }
  EXPECT_EQ(TestFixture::kBigBitSize, big_bit_vector->onesCount());
  EXPECT_EQ(TestFixture::kBigBitSize, big_bit_vector->firstZero());
  EXPECT_EQ(TestFixture::kBigBitSize, big_bit_vector->firstZero(TestFixture::kBigBitSize / 2));
  EXPECT_EQ(0u, big_bit_vector->firstOne());
  for (size_t i = 0; i < TestFixture::kBigBitSize; ++i) {
    EXPECT_TRUE(big_bit_vector->getBit(i));
    EXPECT_EQ(i, big_bit_vector->firstOne(i));
    EXPECT_EQ(TestFixture::kBigBitSize, big_bit_vector->firstZero(i));
  }
}

TYPED_TEST(BitVectorTest, AssignFromTest) {
  std::unique_ptr<TypeParam> small_bit_vector(this->createBitVector(TestFixture::kSmallBitSize));
  small_bit_vector->clear();

  small_bit_vector->setBit(2, true);
  small_bit_vector->setBit(4, true);

  std::unique_ptr<TypeParam> small_bit_vector_copy(this->createBitVector(TestFixture::kSmallBitSize));
  small_bit_vector_copy->assignFrom(*small_bit_vector);
  for (size_t i = 0; i < TestFixture::kSmallBitSize; ++i) {
    if ((i == 2) || (i == 4)) {
      EXPECT_TRUE(small_bit_vector_copy->getBit(i));
    } else {
      EXPECT_FALSE(small_bit_vector_copy->getBit(i));
    }
  }

  std::unique_ptr<TypeParam> big_bit_vector(this->createBitVector(TestFixture::kBigBitSize));
  big_bit_vector->clear();

  big_bit_vector->setBit(2);
  big_bit_vector->setBit(4);
  big_bit_vector->setBit(42);
  big_bit_vector->setBit(77);

  std::unique_ptr<TypeParam> big_bit_vector_copy(this->createBitVector(TestFixture::kBigBitSize));
  big_bit_vector_copy->assignFrom(*big_bit_vector);
  for (size_t i = 0; i < TestFixture::kBigBitSize; ++i) {
    if ((i == 2) || (i == 4) || (i == 42) || (i == 77)) {
      EXPECT_TRUE(big_bit_vector_copy->getBit(i));
    } else {
      EXPECT_FALSE(big_bit_vector_copy->getBit(i));
    }
  }
}

TYPED_TEST(BitVectorTest, ByteFlipBitsTest) {
  TestFixture::runFlipBitsTest(7);
  TestFixture::runFlipBitsTest(8);
}

TYPED_TEST(BitVectorTest, ShortFlipBitsTest) {
  TestFixture::runFlipBitsTest(9);
  TestFixture::runFlipBitsTest(15);
  TestFixture::runFlipBitsTest(16);
}

TYPED_TEST(BitVectorTest, WordFlipBitsTest) {
  TestFixture::runFlipBitsTest(17);
  TestFixture::runFlipBitsTest(31);
  TestFixture::runFlipBitsTest(32);
}

TYPED_TEST(BitVectorTest, RegularFlipBitsTest) {
  TestFixture::runFlipBitsTest(33);
  TestFixture::runFlipBitsTest(63);
  TestFixture::runFlipBitsTest(64);
  TestFixture::runFlipBitsTest(65);
  TestFixture::runFlipBitsTest(200);
}

TYPED_TEST(BitVectorTest, ByteIntersectWithTest) {
  TestFixture::runIntersectWithTest(7);
  TestFixture::runIntersectWithTest(8);
}

TYPED_TEST(BitVectorTest, ShortIntersectWithTest) {
  TestFixture::runIntersectWithTest(9);
  TestFixture::runIntersectWithTest(15);
  TestFixture::runIntersectWithTest(16);
}

TYPED_TEST(BitVectorTest, WordIntersectWithTest) {
  TestFixture::runIntersectWithTest(17);
  TestFixture::runIntersectWithTest(31);
  TestFixture::runIntersectWithTest(32);
}

TYPED_TEST(BitVectorTest, RegularIntersectWithTest) {
  TestFixture::runIntersectWithTest(33);
  TestFixture::runIntersectWithTest(63);
  TestFixture::runIntersectWithTest(64);
  TestFixture::runIntersectWithTest(65);
  TestFixture::runIntersectWithTest(200);
}

TYPED_TEST(BitVectorTest, ByteUnionWithTest) {
  TestFixture::runUnionWithTest(7);
  TestFixture::runUnionWithTest(8);
}

TYPED_TEST(BitVectorTest, ShortUnionWithTest) {
  TestFixture::runUnionWithTest(9);
  TestFixture::runUnionWithTest(15);
  TestFixture::runUnionWithTest(16);
}

TYPED_TEST(BitVectorTest, WordUnionWithTest) {
  TestFixture::runUnionWithTest(17);
  TestFixture::runUnionWithTest(31);
  TestFixture::runUnionWithTest(32);
}

TYPED_TEST(BitVectorTest, RegularUnionWithTest) {
  TestFixture::runUnionWithTest(33);
  TestFixture::runUnionWithTest(63);
  TestFixture::runUnionWithTest(64);
  TestFixture::runUnionWithTest(65);
  TestFixture::runUnionWithTest(200);
}

TYPED_TEST(BitVectorTest, SetBitRangeTest) {
  std::unique_ptr<TypeParam> small_bit_vector(this->createBitVector(TestFixture::kSmallBitSize));
  small_bit_vector->clear();

  small_bit_vector->setBitRange(2, 4, true);
  small_bit_vector->setBitRange(3, 2, false);

  for (size_t i = 0; i < TestFixture::kSmallBitSize; ++i) {
    if ((i == 2) || (i == 5)) {
      EXPECT_TRUE(small_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(small_bit_vector->getBit(i));
    }
  }

  std::unique_ptr<TypeParam> bigger_bit_vector(this->createBitVector(TestFixture::kBiggerBitSize));
  bigger_bit_vector->clear();

  // Set with exact size_t boundaries.
  bigger_bit_vector->setBitRange(128, 512, true);
  bigger_bit_vector->setBitRange(192, 64, false);
  for (size_t i = 0; i < TestFixture::kBiggerBitSize; ++i) {
    if (((i >= 128) && (i < 192)) || ((i >= 192 + 64) && (i < 128 + 512))) {
      EXPECT_TRUE(bigger_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(bigger_bit_vector->getBit(i));
    }
  }

  // Set with boundaries that don't align with size_t strides.
  bigger_bit_vector->clear();
  bigger_bit_vector->setBitRange(111, 222, true);
  bigger_bit_vector->setBitRange(123, 55, false);
  for (size_t i = 0; i < TestFixture::kBiggerBitSize; ++i) {
    if (((i >= 111) && (i < 123)) || ((i >= 123 + 55) && (i < 111 + 222))) {
      EXPECT_TRUE(bigger_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(bigger_bit_vector->getBit(i));
    }
  }

  // Set with boundaries all inside the same size_t stride.
  bigger_bit_vector->clear();
  bigger_bit_vector->setBitRange(130, 20, true);
  bigger_bit_vector->setBitRange(135, 5, false);
  for (size_t i = 0; i < TestFixture::kBiggerBitSize; ++i) {
    if (((i >= 130) && (i < 135)) || ((i >= 135 + 5) && (i < 130 + 20))) {
      EXPECT_TRUE(bigger_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(bigger_bit_vector->getBit(i));
    }
  }
}

TYPED_TEST(BitVectorTest, SetAndGetBitWordTest) {
  std::unique_ptr<TypeParam> bigger_bit_vector(this->createBitVector(TestFixture::kBiggerBitSize));
  bigger_bit_vector->clear();

  // Set with exact size_t boundaries.
  const std::size_t bits_in_word = sizeof(std::size_t) << 3;
  const std::size_t word_mask = static_cast<std::size_t>(-1);
  const std::size_t last_word_id = (TestFixture::kBiggerBitSize - 1) / bits_in_word;

  // This test should work on both 32bit architecture and 64-bit architecture.
  bigger_bit_vector->setBitWord(2, static_cast<std::size_t>(1) << ((bits_in_word - 1) - 3));
  bigger_bit_vector->setBitWord(6, ~(static_cast<std::size_t>(1) << ((bits_in_word - 1) - 11)));
  bigger_bit_vector->setBitWord(last_word_id, word_mask);
  for (size_t i = 0; i < TestFixture::kBiggerBitSize; ++i) {
    if (i == 2 * bits_in_word + 3
        || (i >= 6 * bits_in_word && i < 7 * bits_in_word && i != 6 * bits_in_word + 11)
        || i >= last_word_id * bits_in_word) {
      EXPECT_TRUE(bigger_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(bigger_bit_vector->getBit(i));
    }
  }
  for (size_t word_id = 0; word_id <= last_word_id; ++word_id) {
    if (word_id == 2) {
      EXPECT_EQ(static_cast<std::size_t>(1) << ((bits_in_word - 1) - 3),
                bigger_bit_vector->getBitWord(word_id));
    } else if (word_id == 6) {
      EXPECT_EQ(~(static_cast<std::size_t>(1) << ((bits_in_word - 1) - 11)),
                bigger_bit_vector->getBitWord(word_id));
    } else if (word_id == last_word_id) {
      size_t last_word = word_mask
          << ((last_word_id + 1) * bits_in_word - TestFixture::kBiggerBitSize);
      EXPECT_EQ(last_word, bigger_bit_vector->getBitWord(word_id));
    } else {
      EXPECT_EQ(0u, bigger_bit_vector->getBitWord(word_id));
    }
  }
}

TYPED_TEST(BitVectorTest, ShiftTailForwardTest) {
  std::unique_ptr<TypeParam> small_bit_vector(this->createBitVector(TestFixture::kSmallBitSize));
  small_bit_vector->clear();

  small_bit_vector->setBit(2, true);
  small_bit_vector->setBit(4, true);
  small_bit_vector->setBit(6, true);

  small_bit_vector->shiftTailForward(3, 3);

  for (size_t i = 0; i < TestFixture::kSmallBitSize; ++i) {
    if ((i == 2) || (i == 3)) {
      EXPECT_TRUE(small_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(small_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(2u, small_bit_vector->onesCount());

  std::unique_ptr<TypeParam> big_bit_vector(this->createBitVector(TestFixture::kBigBitSize));
  big_bit_vector->clear();

  big_bit_vector->setBit(11);
  big_bit_vector->setBit(13);
  big_bit_vector->setBit(27);
  big_bit_vector->setBit(42);
  big_bit_vector->setBit(84);
  big_bit_vector->setBit(88);
  big_bit_vector->setBit(91);
  big_bit_vector->setBit(123);

  big_bit_vector->shiftTailForward(15, 70);
  for (size_t i = 0; i < TestFixture::kBigBitSize; ++i) {
    if ((i == 11) || (i == 13) || (i == 88 - 70) || (i == 91 - 70) || (i == 123 - 70)) {
      EXPECT_TRUE(big_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(big_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(5u, big_bit_vector->onesCount());

  // Also try a relatively small shift within the same word.
  big_bit_vector->clear();
  big_bit_vector->setBit(11);
  big_bit_vector->setBit(13);
  big_bit_vector->setBit(27);
  big_bit_vector->setBit(42);
  big_bit_vector->setBit(45);
  big_bit_vector->setBit(51);
  big_bit_vector->setBit(84);
  big_bit_vector->setBit(88);
  big_bit_vector->setBit(91);
  big_bit_vector->setBit(123);

  big_bit_vector->shiftTailForward(43, 7);
  for (size_t i = 0; i < TestFixture::kBigBitSize; ++i) {
    if ((i == 11) || (i == 13) || (i == 27) || (i == 42) || (i == 51 - 7)
        || (i == 84 - 7)  || (i == 88 - 7)  || (i == 91 - 7)  || (i == 123 - 7)) {
      EXPECT_TRUE(big_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(big_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(9u, big_bit_vector->onesCount());

  // Align the shift distance to size_t.
  big_bit_vector->clear();
  big_bit_vector->setBit(11);
  big_bit_vector->setBit(13);
  big_bit_vector->setBit(27);
  big_bit_vector->setBit(42);
  big_bit_vector->setBit(45);
  big_bit_vector->setBit(51);
  big_bit_vector->setBit(84);
  big_bit_vector->setBit(88);
  big_bit_vector->setBit(91);
  big_bit_vector->setBit(123);
  big_bit_vector->setBit(128);
  big_bit_vector->setBit(137);

  big_bit_vector->shiftTailForward(43, 64);
  for (size_t i = 0; i < TestFixture::kBigBitSize; ++i) {
    if ((i == 11) || (i == 13) || (i == 27) || (i == 42)
        || (i == 123 - 64) || (i == 128 - 64) || (i == 137 - 64)) {
      EXPECT_TRUE(big_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(big_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(7u, big_bit_vector->onesCount());

  // Exactly align both the tail start and the shift distance to size_t.
  big_bit_vector->clear();
  big_bit_vector->setBit(11);
  big_bit_vector->setBit(13);
  big_bit_vector->setBit(27);
  big_bit_vector->setBit(42);
  big_bit_vector->setBit(45);
  big_bit_vector->setBit(51);
  big_bit_vector->setBit(84);
  big_bit_vector->setBit(88);
  big_bit_vector->setBit(91);
  big_bit_vector->setBit(123);
  big_bit_vector->setBit(128);
  big_bit_vector->setBit(137);

  big_bit_vector->shiftTailForward(64, 64);
  for (size_t i = 0; i < TestFixture::kBigBitSize; ++i) {
    if ((i == 11) || (i == 13) || (i == 27) || (i == 42) || (i == 45)
        || (i == 51) || (i == 128 - 64)  || (i == 137 - 64)) {
      EXPECT_TRUE(big_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(big_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(8u, big_bit_vector->onesCount());

  std::unique_ptr<TypeParam> bigger_bit_vector(this->createBitVector(TestFixture::kBiggerBitSize));
  bigger_bit_vector->clear();
  bigger_bit_vector->flipBits();

  bigger_bit_vector->shiftTailForward(123, 456);
  for (size_t i = 0; i < TestFixture::kBiggerBitSize; ++i) {
    if (i < TestFixture::kBiggerBitSize - 456) {
      EXPECT_TRUE(bigger_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(bigger_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(TestFixture::kBiggerBitSize - 456, bigger_bit_vector->onesCount());

  // Also try a nicely-aligned shift.
  bigger_bit_vector->clear();
  bigger_bit_vector->flipBits();

  bigger_bit_vector->shiftTailForward(123, 512);
  for (size_t i = 0; i < TestFixture::kBiggerBitSize; ++i) {
    if (i < TestFixture::kBiggerBitSize - 512) {
      EXPECT_TRUE(bigger_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(bigger_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(TestFixture::kBiggerBitSize - 512, bigger_bit_vector->onesCount());

  // Even nicer, align the tail start as well.
  bigger_bit_vector->clear();
  bigger_bit_vector->flipBits();

  bigger_bit_vector->shiftTailForward(128, 512);
  for (size_t i = 0; i < TestFixture::kBiggerBitSize; ++i) {
    if (i < TestFixture::kBiggerBitSize - 512) {
      EXPECT_TRUE(bigger_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(bigger_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(TestFixture::kBiggerBitSize - 512, bigger_bit_vector->onesCount());

  // Finally, do a shift so large that it just zeroes out the entire tail.
  bigger_bit_vector->clear();
  bigger_bit_vector->flipBits();

  bigger_bit_vector->shiftTailForward(567, 1000);
  for (size_t i = 0; i < TestFixture::kBiggerBitSize; ++i) {
    if (i < 567) {
      EXPECT_TRUE(bigger_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(bigger_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(567u, bigger_bit_vector->onesCount());
}

TYPED_TEST(BitVectorTest, ShiftTailBackwardTest) {
  std::unique_ptr<TypeParam> small_bit_vector(this->createBitVector(TestFixture::kSmallBitSize));
  small_bit_vector->clear();

  small_bit_vector->setBit(2, true);
  small_bit_vector->setBit(4, true);
  small_bit_vector->setBit(6, true);

  small_bit_vector->shiftTailBackward(3, 3);

  for (size_t i = 0; i < TestFixture::kSmallBitSize; ++i) {
    if ((i == 2) || (i == 7)) {
      EXPECT_TRUE(small_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(small_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(2u, small_bit_vector->onesCount());

  std::unique_ptr<TypeParam> big_bit_vector(this->createBitVector(TestFixture::kBigBitSize));
  big_bit_vector->clear();

  big_bit_vector->setBit(11);
  big_bit_vector->setBit(13);
  big_bit_vector->setBit(27);
  big_bit_vector->setBit(42);
  big_bit_vector->setBit(84);
  big_bit_vector->setBit(88);
  big_bit_vector->setBit(91);
  big_bit_vector->setBit(123);

  big_bit_vector->shiftTailBackward(15, 70);
  for (size_t i = 0; i < TestFixture::kBigBitSize; ++i) {
    if ((i == 11) || (i == 13) || (i == 27 + 70)
        || (i == 42 + 70) || (i == 88 + 70) || (i == 84 + 70)) {
      EXPECT_TRUE(big_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(big_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(6u, big_bit_vector->onesCount());

  // Also try a relatively small shift within the same word.
  big_bit_vector->clear();
  big_bit_vector->setBit(11);
  big_bit_vector->setBit(13);
  big_bit_vector->setBit(27);
  big_bit_vector->setBit(42);
  big_bit_vector->setBit(45);
  big_bit_vector->setBit(51);
  big_bit_vector->setBit(84);
  big_bit_vector->setBit(88);
  big_bit_vector->setBit(91);
  big_bit_vector->setBit(123);

  big_bit_vector->shiftTailBackward(43, 7);
  for (size_t i = 0; i < TestFixture::kBigBitSize; ++i) {
    if ((i == 11) || (i == 13) || (i == 27) || (i == 42) || (i == 45 + 7) || (i == 51 + 7)
        || (i == 84 + 7)  || (i == 88 + 7)  || (i == 91 + 7)  || (i == 123 + 7)) {
      EXPECT_TRUE(big_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(big_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(10u, big_bit_vector->onesCount());

  // Align the shift distance to size_t.
  big_bit_vector->clear();
  big_bit_vector->setBit(11);
  big_bit_vector->setBit(13);
  big_bit_vector->setBit(27);
  big_bit_vector->setBit(42);
  big_bit_vector->setBit(45);
  big_bit_vector->setBit(51);
  big_bit_vector->setBit(84);
  big_bit_vector->setBit(88);
  big_bit_vector->setBit(91);
  big_bit_vector->setBit(123);
  big_bit_vector->setBit(128);
  big_bit_vector->setBit(137);

  big_bit_vector->shiftTailBackward(43, 64);
  for (size_t i = 0; i < TestFixture::kBigBitSize; ++i) {
    if ((i == 11) || (i == 13) || (i == 27) || (i == 42)
        || (i == 45 + 64) || (i == 51 + 64) || (i == 84 + 64)
        || (i == 88 + 64) || (i == 91 + 64)) {
      EXPECT_TRUE(big_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(big_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(9u, big_bit_vector->onesCount());

  // Exactly align both the tail start and the shift distance to size_t.
  big_bit_vector->clear();
  big_bit_vector->setBit(11);
  big_bit_vector->setBit(13);
  big_bit_vector->setBit(27);
  big_bit_vector->setBit(42);
  big_bit_vector->setBit(45);
  big_bit_vector->setBit(51);
  big_bit_vector->setBit(84);
  big_bit_vector->setBit(88);
  big_bit_vector->setBit(91);
  big_bit_vector->setBit(123);
  big_bit_vector->setBit(128);
  big_bit_vector->setBit(137);

  big_bit_vector->shiftTailBackward(64, 64);
  for (size_t i = 0; i < TestFixture::kBigBitSize; ++i) {
    if ((i == 11) || (i == 13) || (i == 27) || (i == 42) || (i == 45)
        || (i == 51) || (i == 84 + 64)  || (i == 88 + 64) || (i == 91 + 64)) {
      EXPECT_TRUE(big_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(big_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(9u, big_bit_vector->onesCount());

  std::unique_ptr<TypeParam> bigger_bit_vector(this->createBitVector(TestFixture::kBiggerBitSize));
  bigger_bit_vector->clear();
  bigger_bit_vector->flipBits();

  bigger_bit_vector->shiftTailBackward(123, 456);
  for (size_t i = 0; i < TestFixture::kBiggerBitSize; ++i) {
    if ((i < 123) || (i >= 123 + 456)) {
      EXPECT_TRUE(bigger_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(bigger_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(TestFixture::kBiggerBitSize - 456, bigger_bit_vector->onesCount());

  // Also try a nicely-aligned shift.
  bigger_bit_vector->clear();
  bigger_bit_vector->flipBits();

  bigger_bit_vector->shiftTailBackward(123, 512);
  for (size_t i = 0; i < TestFixture::kBiggerBitSize; ++i) {
    if ((i < 123) || (i >= 123 + 512)) {
      EXPECT_TRUE(bigger_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(bigger_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(TestFixture::kBiggerBitSize - 512, bigger_bit_vector->onesCount());

  // Even nicer, align the tail start as well.
  bigger_bit_vector->clear();
  bigger_bit_vector->flipBits();

  bigger_bit_vector->shiftTailBackward(128, 512);
  for (size_t i = 0; i < TestFixture::kBiggerBitSize; ++i) {
    if ((i < 128) || (i >= 128 + 512)) {
      EXPECT_TRUE(bigger_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(bigger_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(TestFixture::kBiggerBitSize - 512, bigger_bit_vector->onesCount());

  // Finally, do a shift so large that it just zeroes out the entire tail.
  bigger_bit_vector->clear();
  bigger_bit_vector->flipBits();

  bigger_bit_vector->shiftTailBackward(567, 1000);
  for (size_t i = 0; i < TestFixture::kBiggerBitSize; ++i) {
    if (i < 567) {
      EXPECT_TRUE(bigger_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(bigger_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(567u, bigger_bit_vector->onesCount());
}

TYPED_TEST(BitVectorTest, RebindTest) {
  std::unique_ptr<TypeParam> big_bit_vector(this->createBitVector(TestFixture::kBigBitSize));
  big_bit_vector->clear();

  // Set some bits, particularly around potential boundaries between size_t
  // strides.
  big_bit_vector->setBit(0);
  big_bit_vector->setBit(5);
  big_bit_vector->setBit(31);
  big_bit_vector->setBit(32);
  big_bit_vector->setBit(63);
  big_bit_vector->setBit(64);
  big_bit_vector->setBit(127);
  big_bit_vector->setBit(128);

  // Destroy the old BitVector and create a new one bound to the same memory.
  ASSERT_EQ(1u, this->allocated_chunks_.size());
  big_bit_vector.reset(new TypeParam(this->allocated_chunks_.back(), TestFixture::kBigBitSize));

  // Make sure bits set in the original BitVector instance are set in the
  // rebound instance.
  for (size_t i = 0; i < TestFixture::kBigBitSize; ++i) {
    if ((i == 0) || (i == 5) || (i == 31) || (i == 32) || (i == 63) || (i == 64) || (i == 127) || (i == 128)) {
      EXPECT_TRUE(big_bit_vector->getBit(i));
    } else {
      EXPECT_FALSE(big_bit_vector->getBit(i));
    }
  }
  EXPECT_EQ(8u, big_bit_vector->onesCount());
  EXPECT_EQ(1u, big_bit_vector->firstZero());
  EXPECT_EQ(2u, big_bit_vector->firstZero(2));
  EXPECT_EQ(0u, big_bit_vector->firstOne());
  EXPECT_EQ(5u, big_bit_vector->firstOne(1));
  EXPECT_EQ(TestFixture::kBigBitSize - 1, big_bit_vector->lastZero());
  EXPECT_EQ(126u, big_bit_vector->lastZero(129));
  EXPECT_EQ(TestFixture::kBigBitSize, big_bit_vector->lastZero(1));
  EXPECT_EQ(128u, big_bit_vector->lastOne());
  EXPECT_EQ(127u, big_bit_vector->lastOne(128));
  EXPECT_EQ(64u, big_bit_vector->lastOne(127));
}

TYPED_TEST(BitVectorTest, AnyTest) {
  std::unique_ptr<TypeParam> small_bit_vector(this->createBitVector(TestFixture::kSmallBitSize));

  small_bit_vector->clear();
  EXPECT_FALSE(small_bit_vector->any());

  small_bit_vector->setBit(0, true);
  EXPECT_TRUE(small_bit_vector->any());
  small_bit_vector->setBit(0, false);
  EXPECT_FALSE(small_bit_vector->any());

  small_bit_vector->setBit(TestFixture::kSmallBitSize - 1, true);
  EXPECT_TRUE(small_bit_vector->any());
  small_bit_vector->setBit(TestFixture::kSmallBitSize - 1, false);
  EXPECT_FALSE(small_bit_vector->any());

  small_bit_vector->setBit(TestFixture::kSmallBitSize / 2, true);
  EXPECT_TRUE(small_bit_vector->any());
  small_bit_vector->setBit(TestFixture::kSmallBitSize / 2 + 1, true);
  EXPECT_TRUE(small_bit_vector->any());
  small_bit_vector->setBit(TestFixture::kSmallBitSize / 2, false);
  EXPECT_TRUE(small_bit_vector->any());
  small_bit_vector->setBit(TestFixture::kSmallBitSize / 2 + 1, false);
  EXPECT_FALSE(small_bit_vector->any());


  std::unique_ptr<TypeParam> big_bit_vector(this->createBitVector(TestFixture::kBigBitSize));

  big_bit_vector->clear();
  EXPECT_FALSE(big_bit_vector->any());

  big_bit_vector->setBit(0);
  EXPECT_TRUE(big_bit_vector->any());
  big_bit_vector->setBit(0, false);
  EXPECT_FALSE(big_bit_vector->any());

  big_bit_vector->setBit(TestFixture::kBigBitSize - 1);
  EXPECT_TRUE(big_bit_vector->any());
  big_bit_vector->setBit(TestFixture::kBigBitSize - 1, false);
  EXPECT_FALSE(big_bit_vector->any());

  big_bit_vector->setBit(TestFixture::kBigBitSize / 2);
  EXPECT_TRUE(big_bit_vector->any());
  big_bit_vector->setBit(TestFixture::kBigBitSize / 2 + 1);
  EXPECT_TRUE(big_bit_vector->any());
  big_bit_vector->setBit(TestFixture::kBigBitSize / 2, false);
  EXPECT_TRUE(big_bit_vector->any());
  big_bit_vector->setBit(TestFixture::kBigBitSize / 2 + 1, false);
  EXPECT_FALSE(big_bit_vector->any());
}

TYPED_TEST(BitVectorTest, AllTest) {
  std::unique_ptr<TypeParam> small_bit_vector(this->createBitVector(TestFixture::kSmallBitSize));

  small_bit_vector->clear();
  EXPECT_FALSE(small_bit_vector->all());
  small_bit_vector->flipBits();
  EXPECT_TRUE(small_bit_vector->all());

  small_bit_vector->setBit(0, false);
  EXPECT_FALSE(small_bit_vector->all());
  small_bit_vector->setBit(0, true);
  EXPECT_TRUE(small_bit_vector->all());

  small_bit_vector->setBit(TestFixture::kSmallBitSize - 1, false);
  EXPECT_FALSE(small_bit_vector->all());
  small_bit_vector->setBit(TestFixture::kSmallBitSize - 1, true);
  EXPECT_TRUE(small_bit_vector->all());

  small_bit_vector->setBit(TestFixture::kSmallBitSize / 2, false);
  EXPECT_FALSE(small_bit_vector->all());
  small_bit_vector->setBit(TestFixture::kSmallBitSize / 2 + 1, false);
  EXPECT_FALSE(small_bit_vector->all());
  small_bit_vector->setBit(TestFixture::kSmallBitSize / 2, true);
  EXPECT_FALSE(small_bit_vector->all());
  small_bit_vector->setBit(TestFixture::kSmallBitSize / 2 + 1, true);
  EXPECT_TRUE(small_bit_vector->all());


  std::unique_ptr<TypeParam> big_bit_vector(this->createBitVector(TestFixture::kBigBitSize));

  big_bit_vector->clear();
  EXPECT_FALSE(big_bit_vector->all());
  big_bit_vector->flipBits();
  EXPECT_TRUE(big_bit_vector->all());

  big_bit_vector->setBit(0, false);
  EXPECT_FALSE(big_bit_vector->all());
  big_bit_vector->setBit(0);
  EXPECT_TRUE(big_bit_vector->all());

  big_bit_vector->setBit(TestFixture::kBigBitSize - 1, false);
  EXPECT_FALSE(big_bit_vector->all());
  big_bit_vector->setBit(TestFixture::kBigBitSize - 1);
  EXPECT_TRUE(big_bit_vector->all());

  big_bit_vector->setBit(TestFixture::kBigBitSize / 2, false);
  EXPECT_FALSE(big_bit_vector->all());
  big_bit_vector->setBit(TestFixture::kBigBitSize / 2 + 1, false);
  EXPECT_FALSE(big_bit_vector->all());
  big_bit_vector->setBit(TestFixture::kBigBitSize / 2);
  EXPECT_FALSE(big_bit_vector->all());
  big_bit_vector->setBit(TestFixture::kBigBitSize / 2 + 1);
  EXPECT_TRUE(big_bit_vector->all());
}

TYPED_TEST(BitVectorTest, ZeroLengthTest) {
  EXPECT_EQ(0u, TypeParam::BytesNeeded(0));

  // Test a BitVector that owns its own memory.
  TypeParam owning_zero_length_bit_vector(0);
  this->runZeroLengthTest(&owning_zero_length_bit_vector);

  // Test a BitVector that does not own its own memory.
  unsigned buffer = 0xDEADBEEF;
  TypeParam non_owning_zero_length_bit_vector(&buffer, 0);
  this->runZeroLengthTest(&owning_zero_length_bit_vector);
  // Make sure the provided memory location is unmodified.
  EXPECT_EQ(0xDEADBEEF, buffer);
}

}  // namespace quickstep
