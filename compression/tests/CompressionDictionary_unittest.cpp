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
#include <string>
#include <utility>
#include <vector>

#include "compression/CompressionDictionary.hpp"
#include "compression/CompressionDictionaryBuilder.hpp"
#include "types/CharType.hpp"
#include "types/DoubleType.hpp"
#include "types/IntType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/VarCharType.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/ScopedBuffer.hpp"

#include "gtest/gtest.h"

using std::numeric_limits;
using std::pair;
using std::size_t;
using std::sort;
using std::string;
using std::strlen;
using std::uint8_t;
using std::uint32_t;
using std::vector;

namespace quickstep {

class CompressionDictionaryTest : public ::testing::TestWithParam<bool> {
 protected:
  static const uint32_t kNumSampleValues = 24;
  static const int kSampleInts[kNumSampleValues];
  static const char *kSampleStrings[kNumSampleValues];

  static const size_t kVarCharSizeParameter = 32;

  static uint8_t bitsNeededForCodes(int num_codes) {
    if (num_codes == 0) {
      return 0;
    }
    for (uint8_t num_bits = 1; num_bits < 32; ++num_bits) {
      if ((1 << num_bits) >= num_codes) {
        return num_bits;
      }
    }

    return 32;
  }

  void setupBuilderForType(const Type &type) {
    dictionary_type_ = &type;
    builder_.reset(new CompressionDictionaryBuilder(type));
  }

  void setupSampleIntDictionary() {
    const IntType &int_type = IntType::Instance(GetParam());
    setupBuilderForType(int_type);

    for (uint32_t sample_idx = 0; sample_idx < kNumSampleValues; ++sample_idx) {
      TypedValue current_value = int_type.makeValue(&(kSampleInts[sample_idx]));
      ASSERT_TRUE(builder_->insertEntry(current_value));
    }
    // Also throw in a null value if we are testing a nullable type.
    if (GetParam()) {
      ASSERT_TRUE(builder_->insertEntry(TypedValue(kInt)));
    }
    ASSERT_EQ(kNumSampleValues + GetParam(), builder_->numberOfEntries());

    // Build the physical dictionary.
    dictionary_memory_.reset(builder_->dictionarySizeBytes());
    builder_->buildDictionary(dictionary_memory_.get());

    // Construct the dictionary object.
    dictionary_.reset(new CompressionDictionary(int_type,
                                                dictionary_memory_.get(),
                                                builder_->dictionarySizeBytes()));

    // Delete the builder, we don't need it anymore.
    builder_.reset();
  }

  void setupSampleVarCharDictionary() {
    const VarCharType &varchar_type = VarCharType::Instance(kVarCharSizeParameter, GetParam());
    setupBuilderForType(varchar_type);

    for (uint32_t sample_idx = 0; sample_idx < kNumSampleValues; ++sample_idx) {
      TypedValue current_value = varchar_type.makeValue(kSampleStrings[sample_idx],
                                                        strlen(kSampleStrings[sample_idx]) + 1);
      ASSERT_TRUE(builder_->insertEntry(current_value));
    }
    // Also throw in a null value if we are testing a nullable type.
    if (GetParam()) {
      ASSERT_TRUE(builder_->insertEntry(TypedValue(kVarChar)));
    }
    ASSERT_EQ(kNumSampleValues + GetParam(), builder_->numberOfEntries());

    // Build the physical dictionary.
    dictionary_memory_.reset(builder_->dictionarySizeBytes());
    builder_->buildDictionary(dictionary_memory_.get());

    // Construct the dictionary object.
    dictionary_.reset(new CompressionDictionary(varchar_type,
                                                dictionary_memory_.get(),
                                                builder_->dictionarySizeBytes()));

    // Delete the builder, we don't need it anymore.
    builder_.reset();
  }

  const Type *dictionary_type_;
  std::unique_ptr<CompressionDictionaryBuilder> builder_;
  std::unique_ptr<CompressionDictionary> dictionary_;
  ScopedBuffer dictionary_memory_;
};

const uint32_t CompressionDictionaryTest::kNumSampleValues;

const int CompressionDictionaryTest::kSampleInts[]
    = { 0, 5, -5, 1, 2, 3, -1000, 1000, -1, -2, -3, 500,
        42, 55, -12, 85, -1234, 92, 43, 41, -6464, 50, 9999, -10};

const char *CompressionDictionaryTest::kSampleStrings[] = {
  "Twilight Sparkle",
  "Rarity",
  "Pinkie Pie",
  "Applejack",
  "Rainbow Dash",
  "Fluttershy",
  "Celestia",
  "Luna",
  "Cadence",
  "Applebloom",
  "Scootaloo",
  "Sweetie Belle",
  "Zecora",
  "Big Macintosh",
  "Granny Smith",
  "Shining Armor",
  "Cheerilee",
  "Diamond Tiara",
  "Silver Spoon",
  "Photo Finish",
  "DJ Pon-3",
  "Snips",
  "Snails",
  "Derpy Hooves"
};

const size_t CompressionDictionaryTest::kVarCharSizeParameter;

typedef CompressionDictionaryTest CompressionDictionaryDeathTest;

TEST_P(CompressionDictionaryDeathTest, LimitCodesNotEqualTest) {
  setupSampleIntDictionary();
  int value = 0;
  EXPECT_DEATH(dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kNotEqual, &value), "");
}

// Some tests are only run for debug builds, which have assertions turned on.
#ifdef QUICKSTEP_DEBUG

TEST_P(CompressionDictionaryDeathTest, UndoInsertOnEmptyBuilderTest) {
  setupBuilderForType(TypeFactory::GetType(kInt));
  EXPECT_DEATH(builder_->undoLastInsert(), "");
}

TEST_P(CompressionDictionaryDeathTest, BuilderInsertWrongTypeTest) {
  setupBuilderForType(TypeFactory::GetType(kInt));

  double double_literal = 12.34;
  TypedValue value = TypeFactory::GetType(kDouble).makeValue(&double_literal);
  EXPECT_DEATH(builder_->insertEntry(value), "");
}

TEST_P(CompressionDictionaryDeathTest, BuilderInsertWrongTypeByReferenceTest) {
  setupBuilderForType(TypeFactory::GetType(kInt));

  double double_literal = 12.34;
  TypedValue value = TypeFactory::GetType(kDouble).makeValue(&double_literal);
  EXPECT_DEATH(builder_->insertEntryByReference(value), "");
}

TEST_P(CompressionDictionaryDeathTest, BuilderContainsWrongTypeTest) {
  setupBuilderForType(TypeFactory::GetType(kInt));

  double double_literal = 12.34;
  TypedValue value = TypeFactory::GetType(kDouble).makeValue(&double_literal);
  EXPECT_DEATH(builder_->containsValue(value), "");
}

TEST_P(CompressionDictionaryDeathTest, GetValueForNonexistentCodeTest) {
  setupSampleIntDictionary();

  EXPECT_DEATH(dictionary_->getUntypedValueForCode(kNumSampleValues + GetParam()), "");
}

#endif  // QUICKSTEP_DEBUG

TEST_P(CompressionDictionaryTest, IntKeyBuildTest) {
  const IntType &int_type = IntType::Instance(GetParam());
  TypedValue current_value;

  // Construct a CompressionDictionaryBuilder and check its initial state.
  setupBuilderForType(int_type);
  EXPECT_EQ(0u, builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(0), builder_->codeLengthBits());
  EXPECT_EQ(2 * sizeof(uint32_t), builder_->dictionarySizeBytes());

  // If the type is nullable, try inserting a null first.
  if (GetParam()) {
    current_value = TypedValue(kInt);
    EXPECT_TRUE(builder_->insertEntry(current_value));
    EXPECT_TRUE(builder_->containsValue(current_value));
    EXPECT_EQ(1u, builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(1), builder_->codeLengthBits());
    EXPECT_EQ(2 * sizeof(uint32_t), builder_->dictionarySizeBytes());

    // Roll back to zero entries.
    builder_->undoLastInsert();
    EXPECT_FALSE(builder_->containsValue(current_value));
    EXPECT_EQ(0u, builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(0), builder_->codeLengthBits());
    EXPECT_EQ(2 * sizeof(uint32_t), builder_->dictionarySizeBytes());
  }

  // Add a value.
  current_value = TypedValue(kSampleInts[0]);
  EXPECT_TRUE(builder_->insertEntry(current_value));
  EXPECT_TRUE(builder_->containsValue(current_value));
  EXPECT_EQ(1u, builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(1), builder_->codeLengthBits());
  EXPECT_EQ(2 * sizeof(uint32_t) + int_type.maximumByteLength(), builder_->dictionarySizeBytes());

  // Roll back to zero entries.
  builder_->undoLastInsert();
  EXPECT_FALSE(builder_->containsValue(current_value));
  EXPECT_EQ(0u, builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(0), builder_->codeLengthBits());
  EXPECT_EQ(2 * sizeof(uint32_t), builder_->dictionarySizeBytes());

  // Re-add the value.
  EXPECT_TRUE(builder_->insertEntry(current_value));
  EXPECT_TRUE(builder_->containsValue(current_value));
  EXPECT_EQ(1u, builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(1), builder_->codeLengthBits());
  EXPECT_EQ(2 * sizeof(uint32_t) + int_type.maximumByteLength(), builder_->dictionarySizeBytes());

  // If the type is nullable, also try inserting a null with a real value
  // present.
  if (GetParam()) {
    current_value = TypedValue(kInt);
    EXPECT_TRUE(builder_->insertEntry(current_value));
    EXPECT_TRUE(builder_->containsValue(current_value));
    EXPECT_EQ(2u, builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(2), builder_->codeLengthBits());
    EXPECT_EQ(2 * sizeof(uint32_t) + int_type.maximumByteLength(), builder_->dictionarySizeBytes());

    // Roll back insert of null.
    builder_->undoLastInsert();
    EXPECT_FALSE(builder_->containsValue(current_value));
    EXPECT_EQ(1u, builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(1), builder_->codeLengthBits());
    EXPECT_EQ(2 * sizeof(uint32_t) + int_type.maximumByteLength(), builder_->dictionarySizeBytes());

    // Finally, re-insert the null.
    EXPECT_TRUE(builder_->insertEntry(current_value));
    EXPECT_TRUE(builder_->containsValue(current_value));
    EXPECT_EQ(2u, builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(2), builder_->codeLengthBits());
    EXPECT_EQ(2 * sizeof(uint32_t) + int_type.maximumByteLength(), builder_->dictionarySizeBytes());
  }

  // Add a second value.
  current_value = TypedValue(kSampleInts[1]);
  EXPECT_TRUE(builder_->insertEntry(current_value));
  EXPECT_TRUE(builder_->containsValue(current_value));
  EXPECT_EQ(2u + GetParam(), builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(2 + GetParam()), builder_->codeLengthBits());
  EXPECT_EQ(2 * sizeof(uint32_t) + 2 * int_type.maximumByteLength(),
            builder_->dictionarySizeBytes());

  // Add a third value.
  current_value = TypedValue(kSampleInts[2]);
  EXPECT_TRUE(builder_->insertEntry(current_value));
  EXPECT_TRUE(builder_->containsValue(current_value));
  EXPECT_EQ(3u + GetParam(), builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(3 + GetParam()), builder_->codeLengthBits());
  EXPECT_EQ(2 * sizeof(uint32_t) + 3 * int_type.maximumByteLength(), builder_->dictionarySizeBytes());

  // Roll it back (this is the last time we will check this, we just need to
  // cover all the possible paths where the code length may shrink).
  builder_->undoLastInsert();
  EXPECT_FALSE(builder_->containsValue(current_value));
  EXPECT_EQ(2u + GetParam(), builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(2 + GetParam()), builder_->codeLengthBits());
  EXPECT_EQ(2 * sizeof(uint32_t) + 2 * int_type.maximumByteLength(), builder_->dictionarySizeBytes());

  // Insert the rest of the sample values.
  for (uint32_t sample_idx = 2; sample_idx < kNumSampleValues; ++sample_idx) {
    current_value = TypedValue(kSampleInts[sample_idx]);
    EXPECT_TRUE(builder_->insertEntry(current_value));
    EXPECT_TRUE(builder_->containsValue(current_value));
    EXPECT_EQ(sample_idx + 1 + GetParam(), builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(sample_idx + 1 + GetParam()), builder_->codeLengthBits());
    EXPECT_EQ(2 * sizeof(uint32_t) + (sample_idx + 1) * int_type.maximumByteLength(),
              builder_->dictionarySizeBytes());
  }

  // Attempt to insert a duplicate entry.
  current_value = TypedValue(kSampleInts[kNumSampleValues >> 1]);
  EXPECT_FALSE(builder_->insertEntry(current_value));
  EXPECT_TRUE(builder_->containsValue(current_value));
  EXPECT_EQ(kNumSampleValues + GetParam(), builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(kNumSampleValues + GetParam()), builder_->codeLengthBits());
  EXPECT_EQ(2 * sizeof(uint32_t) + kNumSampleValues * int_type.maximumByteLength(),
            builder_->dictionarySizeBytes());

  // If type is nullable, attempt to insert a duplicate null.
  if (GetParam()) {
    current_value = TypedValue(kInt);
    EXPECT_FALSE(builder_->insertEntry(current_value));
    EXPECT_TRUE(builder_->containsValue(current_value));
    EXPECT_EQ(kNumSampleValues + GetParam(), builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(kNumSampleValues + GetParam()), builder_->codeLengthBits());
    EXPECT_EQ(2 * sizeof(uint32_t) + kNumSampleValues * int_type.maximumByteLength(),
              builder_->dictionarySizeBytes());
  }

  // Check for the presence of a nonexistent key.
  current_value = TypedValue(7777);
  EXPECT_FALSE(builder_->containsValue(current_value));

  // Actually build the physical dictionary.
  dictionary_memory_.reset(builder_->dictionarySizeBytes());
  builder_->buildDictionary(dictionary_memory_.get());

  // Check the contents of the physical dictionary.
  vector<int> sorted_sample_ints(kSampleInts, kSampleInts + kNumSampleValues);
  sort(sorted_sample_ints.begin(), sorted_sample_ints.end());
  const int *value_array = reinterpret_cast<const int*>(
      static_cast<const char*>(dictionary_memory_.get()) + 2 * sizeof(uint32_t));

  EXPECT_EQ(kNumSampleValues, *static_cast<const uint32_t*>(dictionary_memory_.get()));
  if (GetParam()) {
    EXPECT_EQ(kNumSampleValues,
              *(static_cast<const uint32_t*>(dictionary_memory_.get()) + 1));
  } else {
    EXPECT_EQ(numeric_limits<uint32_t>::max(),
              *(static_cast<const uint32_t*>(dictionary_memory_.get()) + 1));
  }
  for (uint32_t sample_idx = 0; sample_idx < kNumSampleValues; ++sample_idx) {
    EXPECT_EQ(sorted_sample_ints[sample_idx], value_array[sample_idx]);
  }
}

// Same test as above, but insert values via insertEntryByReference().
TEST_P(CompressionDictionaryTest, IntKeyBuildInsertByReferenceTest) {
  const IntType &int_type = IntType::Instance(GetParam());
  std::vector<TypedValue> values;
  TypedValue other_value;
  TypedValue null_value(kInt);

  // Construct a CompressionDictionaryBuilder and check its initial state.
  setupBuilderForType(int_type);
  EXPECT_EQ(0u, builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(0), builder_->codeLengthBits());
  EXPECT_EQ(2 * sizeof(uint32_t), builder_->dictionarySizeBytes());

  // If the type is nullable, try inserting a null first.
  if (GetParam()) {
    EXPECT_TRUE(builder_->insertEntryByReference(null_value));
    EXPECT_TRUE(builder_->containsValue(null_value));
    EXPECT_EQ(1u, builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(1), builder_->codeLengthBits());
    EXPECT_EQ(2 * sizeof(uint32_t), builder_->dictionarySizeBytes());

    // Roll back to zero entries.
    builder_->undoLastInsert();
    EXPECT_FALSE(builder_->containsValue(null_value));
    EXPECT_EQ(0u, builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(0), builder_->codeLengthBits());
    EXPECT_EQ(2 * sizeof(uint32_t), builder_->dictionarySizeBytes());
  }

  // Add a value.
  values.push_back(TypedValue(kSampleInts[0]));
  EXPECT_TRUE(builder_->insertEntryByReference(values.back()));
  EXPECT_TRUE(builder_->containsValue(values.back()));
  EXPECT_EQ(1u, builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(1), builder_->codeLengthBits());
  EXPECT_EQ(2 * sizeof(uint32_t) + int_type.maximumByteLength(), builder_->dictionarySizeBytes());

  // Roll back to zero entries.
  builder_->undoLastInsert();
  EXPECT_FALSE(builder_->containsValue(values.back()));
  EXPECT_EQ(0u, builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(0), builder_->codeLengthBits());
  EXPECT_EQ(2 * sizeof(uint32_t), builder_->dictionarySizeBytes());

  // Re-add the value.
  EXPECT_TRUE(builder_->insertEntryByReference(values.back()));
  EXPECT_TRUE(builder_->containsValue(values.back()));
  EXPECT_EQ(1u, builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(1), builder_->codeLengthBits());
  EXPECT_EQ(2 * sizeof(uint32_t) + int_type.maximumByteLength(), builder_->dictionarySizeBytes());

  // If the type is nullable, also try inserting a null with a real value
  // present.
  if (GetParam()) {
    EXPECT_TRUE(builder_->insertEntryByReference(null_value));
    EXPECT_TRUE(builder_->containsValue(null_value));
    EXPECT_EQ(2u, builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(2), builder_->codeLengthBits());
    EXPECT_EQ(2 * sizeof(uint32_t) + int_type.maximumByteLength(), builder_->dictionarySizeBytes());

    // Roll back insert of null.
    builder_->undoLastInsert();
    EXPECT_FALSE(builder_->containsValue(null_value));
    EXPECT_EQ(1u, builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(1), builder_->codeLengthBits());
    EXPECT_EQ(2 * sizeof(uint32_t) + int_type.maximumByteLength(), builder_->dictionarySizeBytes());

    // Finally, re-insert the null.
    EXPECT_TRUE(builder_->insertEntryByReference(null_value));
    EXPECT_TRUE(builder_->containsValue(null_value));
    EXPECT_EQ(2u, builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(2), builder_->codeLengthBits());
    EXPECT_EQ(2 * sizeof(uint32_t) + int_type.maximumByteLength(), builder_->dictionarySizeBytes());
  }

  // Add a second value.
  values.push_back(TypedValue(kSampleInts[1]));
  EXPECT_TRUE(builder_->insertEntryByReference(values.back()));
  // Check against a different but equal value.
  other_value = TypedValue(kSampleInts[1]);
  EXPECT_TRUE(builder_->containsValue(other_value));
  EXPECT_EQ(2u + GetParam(), builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(2 + GetParam()), builder_->codeLengthBits());
  EXPECT_EQ(2 * sizeof(uint32_t) + 2 * int_type.maximumByteLength(), builder_->dictionarySizeBytes());

  // Add a third value.
  values.push_back(TypedValue(kSampleInts[2]));
  EXPECT_TRUE(builder_->insertEntryByReference(values.back()));
  EXPECT_TRUE(builder_->containsValue(values.back()));
  EXPECT_EQ(3u + GetParam(), builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(3 + GetParam()), builder_->codeLengthBits());
  EXPECT_EQ(2 * sizeof(uint32_t) + 3 * int_type.maximumByteLength(), builder_->dictionarySizeBytes());

  // Roll it back (this is the last time we will check this, we just need to
  // cover all the possible paths where the code length may shrink).
  builder_->undoLastInsert();
  EXPECT_FALSE(builder_->containsValue(values.back()));
  EXPECT_EQ(2u + GetParam(), builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(2 + GetParam()), builder_->codeLengthBits());
  EXPECT_EQ(2 * sizeof(uint32_t) + 2 * int_type.maximumByteLength(), builder_->dictionarySizeBytes());

  // Insert the rest of the sample values.
  for (uint32_t sample_idx = 2; sample_idx < kNumSampleValues; ++sample_idx) {
    values.push_back(TypedValue(kSampleInts[sample_idx]));
    EXPECT_TRUE(builder_->insertEntryByReference(values.back()));
    EXPECT_TRUE(builder_->containsValue(values.back()));
    EXPECT_EQ(sample_idx + 1 + GetParam(), builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(sample_idx + 1 + GetParam()), builder_->codeLengthBits());
    EXPECT_EQ(2 * sizeof(uint32_t) + (sample_idx + 1) * int_type.maximumByteLength(),
              builder_->dictionarySizeBytes());
  }

  // Attempt to insert a duplicate entry.
  other_value = TypedValue(kSampleInts[kNumSampleValues >> 1]);
  EXPECT_FALSE(builder_->insertEntryByReference(other_value));
  EXPECT_TRUE(builder_->containsValue(other_value));
  EXPECT_EQ(kNumSampleValues + GetParam(), builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(kNumSampleValues + GetParam()), builder_->codeLengthBits());
  EXPECT_EQ(2 * sizeof(uint32_t) + kNumSampleValues * int_type.maximumByteLength(),
            builder_->dictionarySizeBytes());

  // If type is nullable, attempt to insert a duplicate null.
  if (GetParam()) {
    EXPECT_FALSE(builder_->insertEntryByReference(null_value));
    EXPECT_TRUE(builder_->containsValue(null_value));
    EXPECT_EQ(kNumSampleValues + GetParam(), builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(kNumSampleValues + GetParam()), builder_->codeLengthBits());
    EXPECT_EQ(2 * sizeof(uint32_t) + kNumSampleValues * int_type.maximumByteLength(),
              builder_->dictionarySizeBytes());
  }

  // Check for the presence of a nonexistent key.
  other_value = TypedValue(7777);
  EXPECT_FALSE(builder_->containsValue(other_value));

  // Actually build the physical dictionary.
  dictionary_memory_.reset(builder_->dictionarySizeBytes());
  builder_->buildDictionary(dictionary_memory_.get());

  // Check the contents of the physical dictionary.
  vector<int> sorted_sample_ints(kSampleInts, kSampleInts + kNumSampleValues);
  sort(sorted_sample_ints.begin(), sorted_sample_ints.end());
  const int *value_array = reinterpret_cast<const int*>(
      static_cast<const char*>(dictionary_memory_.get()) + 2 * sizeof(uint32_t));

  EXPECT_EQ(kNumSampleValues, *static_cast<const uint32_t*>(dictionary_memory_.get()));
  if (GetParam()) {
    EXPECT_EQ(kNumSampleValues,
              *(static_cast<const uint32_t*>(dictionary_memory_.get()) + 1));
  } else {
    EXPECT_EQ(numeric_limits<uint32_t>::max(),
              *(static_cast<const uint32_t*>(dictionary_memory_.get()) + 1));
  }
  for (uint32_t sample_idx = 0; sample_idx < kNumSampleValues; ++sample_idx) {
    EXPECT_EQ(sorted_sample_ints[sample_idx], value_array[sample_idx]);
  }
}

TEST_P(CompressionDictionaryTest, IntKeyInfoTest) {
  setupSampleIntDictionary();
  EXPECT_EQ(kNumSampleValues + GetParam(), dictionary_->numberOfCodes());
  EXPECT_EQ(bitsNeededForCodes(kNumSampleValues + GetParam()), dictionary_->codeLengthBits());
}

TEST_P(CompressionDictionaryTest, IntKeyGetCodeUntypedTest) {
  setupSampleIntDictionary();

  vector<int> sorted_sample_ints(kSampleInts, kSampleInts + kNumSampleValues);
  sort(sorted_sample_ints.begin(), sorted_sample_ints.end());

  // Check all the codes in the dictionary.
  for (uint32_t sample_idx = 0; sample_idx < kNumSampleValues; ++sample_idx) {
    EXPECT_EQ(sample_idx, dictionary_->getCodeForUntypedValue(&(sorted_sample_ints[sample_idx])));
  }

  if (GetParam()) {
    EXPECT_EQ(kNumSampleValues, dictionary_->getCodeForUntypedValue(nullptr));
  }

  // Check a value that isn't in the dictionary.
  int other_value = 7777;
  EXPECT_EQ(kNumSampleValues + GetParam(), dictionary_->getCodeForUntypedValue(&other_value));
}

TEST_P(CompressionDictionaryTest, IntKeyGetCodeTypedTest) {
  setupSampleIntDictionary();

  vector<int> sorted_sample_ints(kSampleInts, kSampleInts + kNumSampleValues);
  sort(sorted_sample_ints.begin(), sorted_sample_ints.end());

  // Check all the codes in the dictionary.
  for (uint32_t sample_idx = 0; sample_idx < kNumSampleValues; ++sample_idx) {
    TypedValue value(sorted_sample_ints[sample_idx]);
    EXPECT_EQ(sample_idx, dictionary_->getCodeForTypedValue(value, *dictionary_type_));
  }

  if (GetParam()) {
    EXPECT_EQ(kNumSampleValues,
              dictionary_->getCodeForTypedValue(TypedValue(kInt), *dictionary_type_));
  }

  // Check a value that isn't in the dictionary.
  TypedValue other_value(7777);
  EXPECT_EQ(kNumSampleValues + GetParam(),
            dictionary_->getCodeForTypedValue(other_value, *dictionary_type_));
}

// Same as above, but use DoubleType literals.
TEST_P(CompressionDictionaryTest, IntKeyGetCodeDifferentTypeTest) {
  setupSampleIntDictionary();

  vector<int> sorted_sample_ints(kSampleInts, kSampleInts + kNumSampleValues);
  sort(sorted_sample_ints.begin(), sorted_sample_ints.end());

  // Check all the codes in the dictionary.
  for (uint32_t sample_idx = 0; sample_idx < kNumSampleValues; ++sample_idx) {
    TypedValue value(static_cast<double>(sorted_sample_ints[sample_idx]));
    EXPECT_EQ(sample_idx,
              dictionary_->getCodeForTypedValue(value, DoubleType::Instance(false)));
  }

  if (GetParam()) {
    EXPECT_EQ(kNumSampleValues,
              dictionary_->getCodeForTypedValue(TypedValue(kDouble), DoubleType::Instance(true)));
  }

  // Check a value that isn't in the dictionary.
  TypedValue other_value(static_cast<double>(5.5));
  EXPECT_EQ(kNumSampleValues + GetParam(),
            dictionary_->getCodeForTypedValue(other_value, DoubleType::Instance(false)));
}

TEST_P(CompressionDictionaryTest, IntKeyGetValueUntypedTest) {
  setupSampleIntDictionary();

  vector<int> sorted_sample_ints(kSampleInts, kSampleInts + kNumSampleValues);
  sort(sorted_sample_ints.begin(), sorted_sample_ints.end());

  for (uint8_t code = 0; code < kNumSampleValues; ++code) {
    const int *value_ptr = static_cast<const int*>(dictionary_->getUntypedValueForByteCode(code));
    EXPECT_EQ(sorted_sample_ints[code], *value_ptr);
    value_ptr = static_cast<const int*>(dictionary_->getUntypedValueForShortCode(code));
    EXPECT_EQ(sorted_sample_ints[code], *value_ptr);
    value_ptr = static_cast<const int*>(dictionary_->getUntypedValueForCode(code));
    EXPECT_EQ(sorted_sample_ints[code], *value_ptr);
  }

  if (GetParam()) {
    const void *value_ptr = dictionary_->getUntypedValueForByteCode(kNumSampleValues);
    EXPECT_EQ(nullptr, value_ptr);
    value_ptr = dictionary_->getUntypedValueForShortCode(kNumSampleValues);
    EXPECT_EQ(nullptr, value_ptr);
    value_ptr = dictionary_->getUntypedValueForCode(kNumSampleValues);
    EXPECT_EQ(nullptr, value_ptr);
  }
}

TEST_P(CompressionDictionaryTest, IntKeyGetValueTypedTest) {
  setupSampleIntDictionary();

  vector<int> sorted_sample_ints(kSampleInts, kSampleInts + kNumSampleValues);
  sort(sorted_sample_ints.begin(), sorted_sample_ints.end());

  const IntType &int_type = IntType::Instance(GetParam());
  const Comparison &equal_comp = ComparisonFactory::GetComparison(ComparisonID::kEqual);
  for (uint8_t code = 0; code < kNumSampleValues; ++code) {
    TypedValue expected_value(sorted_sample_ints[code]);

    TypedValue value = dictionary_->getTypedValueForByteCode(code);
    EXPECT_TRUE(equal_comp.compareTypedValuesChecked(expected_value, int_type,
                                                     value, int_type));
    value = dictionary_->getTypedValueForShortCode(code);
    EXPECT_TRUE(equal_comp.compareTypedValuesChecked(expected_value, int_type,
                                                     value, int_type));
    value = dictionary_->getTypedValueForCode(code);
    EXPECT_TRUE(equal_comp.compareTypedValuesChecked(expected_value, int_type,
                                                     value, int_type));
  }

  if (GetParam()) {
    TypedValue value = dictionary_->getTypedValueForByteCode(kNumSampleValues);
    EXPECT_TRUE(value.isNull());
    EXPECT_EQ(kInt, value.getTypeID());
    value = dictionary_->getTypedValueForShortCode(kNumSampleValues);
    EXPECT_TRUE(value.isNull());
    EXPECT_EQ(kInt, value.getTypeID());
    value = dictionary_->getTypedValueForCode(kNumSampleValues);
    EXPECT_TRUE(value.isNull());
    EXPECT_EQ(kInt, value.getTypeID());
  }
}

TEST_P(CompressionDictionaryTest, IntKeyBoundCodesUntypedTest) {
  setupSampleIntDictionary();

  vector<int> sorted_sample_ints(kSampleInts, kSampleInts + kNumSampleValues);
  sort(sorted_sample_ints.begin(), sorted_sample_ints.end());

  // Check bounds for all the values which are actually in the dictionary.
  for (uint32_t sorted_value_idx = 0;
       sorted_value_idx < kNumSampleValues;
       ++sorted_value_idx) {
    EXPECT_EQ(sorted_value_idx,
              dictionary_->getLowerBoundCodeForUntypedValue(&(sorted_sample_ints[sorted_value_idx])));
    EXPECT_EQ(sorted_value_idx + 1,
              dictionary_->getUpperBoundCodeForUntypedValue(&(sorted_sample_ints[sorted_value_idx])));
  }

  // Check a value less than the least value in the dictionary.
  int other_value = sorted_sample_ints.front() - 10;
  EXPECT_EQ(0u, dictionary_->getLowerBoundCodeForUntypedValue(&other_value));
  EXPECT_EQ(0u, dictionary_->getUpperBoundCodeForUntypedValue(&other_value));

  // Check a value larger than the greatest value in the dictionary.
  other_value = sorted_sample_ints.back() + 10;
  EXPECT_EQ(kNumSampleValues + GetParam(), dictionary_->getLowerBoundCodeForUntypedValue(&other_value));
  // Note: with nulls, the upper bound can actually be less than the lower
  // bound due to an implementation quirk.
  EXPECT_EQ(kNumSampleValues, dictionary_->getUpperBoundCodeForUntypedValue(&other_value));

  // Split the difference between the last couple of values.
  other_value = (sorted_sample_ints.back() + *(sorted_sample_ints.end() - 2)) / 2;
  EXPECT_EQ(kNumSampleValues - 1, dictionary_->getLowerBoundCodeForUntypedValue(&other_value));
  EXPECT_EQ(kNumSampleValues - 1, dictionary_->getUpperBoundCodeForUntypedValue(&other_value));
}

TEST_P(CompressionDictionaryTest, IntKeyBoundCodesTypedTest) {
  setupSampleIntDictionary();

  vector<int> sorted_sample_ints(kSampleInts, kSampleInts + kNumSampleValues);
  sort(sorted_sample_ints.begin(), sorted_sample_ints.end());

  TypedValue value;
  // Check bounds for all the values which are actually in the dictionary.
  for (uint32_t sorted_value_idx = 0;
       sorted_value_idx < kNumSampleValues;
       ++sorted_value_idx) {
    value = TypedValue(sorted_sample_ints[sorted_value_idx]);
    EXPECT_EQ(sorted_value_idx,
              dictionary_->getLowerBoundCodeForTypedValue(value, *dictionary_type_));
    EXPECT_EQ(sorted_value_idx + 1,
              dictionary_->getUpperBoundCodeForTypedValue(value, *dictionary_type_));
  }

  // Check a value less than the least value in the dictionary.
  value = TypedValue(sorted_sample_ints.front() - 10);
  EXPECT_EQ(0u, dictionary_->getLowerBoundCodeForTypedValue(value, *dictionary_type_));
  EXPECT_EQ(0u, dictionary_->getUpperBoundCodeForTypedValue(value, *dictionary_type_));

  // Check a value larger than the greatest value in the dictionary.
  value = TypedValue(sorted_sample_ints.back() + 10);
  EXPECT_EQ(kNumSampleValues + GetParam(),
            dictionary_->getLowerBoundCodeForTypedValue(value, *dictionary_type_));
  // Note: with nulls, the upper bound can actually be less than the lower
  // bound due to an implementation quirk.
  EXPECT_EQ(kNumSampleValues,
            dictionary_->getUpperBoundCodeForTypedValue(value, *dictionary_type_));

  // Split the difference between the last couple of values.
  value = TypedValue((sorted_sample_ints.back() + *(sorted_sample_ints.end() - 2)) / 2);
  EXPECT_EQ(kNumSampleValues - 1,
            dictionary_->getLowerBoundCodeForTypedValue(value, *dictionary_type_));
  EXPECT_EQ(kNumSampleValues - 1,
            dictionary_->getUpperBoundCodeForTypedValue(value, *dictionary_type_));
}

// Same as a above, but use DoubleType literals.
TEST_P(CompressionDictionaryTest, IntKeyBoundCodesDifferentTypeTest) {
  setupSampleIntDictionary();

  const Type &value_type = DoubleType::Instance(GetParam());

  vector<int> sorted_sample_ints(kSampleInts, kSampleInts + kNumSampleValues);
  sort(sorted_sample_ints.begin(), sorted_sample_ints.end());

  TypedValue value;
  // Check bounds for all the values which are actually in the dictionary.
  for (uint32_t sorted_value_idx = 0;
       sorted_value_idx < kNumSampleValues;
       ++sorted_value_idx) {
    value = TypedValue(static_cast<double>(sorted_sample_ints[sorted_value_idx]));
    EXPECT_EQ(sorted_value_idx,
              dictionary_->getLowerBoundCodeForTypedValue(value, value_type));
    EXPECT_EQ(sorted_value_idx + 1,
              dictionary_->getUpperBoundCodeForTypedValue(value, value_type));
  }

  // Check a value less than the least value in the dictionary.
  value = TypedValue(static_cast<double>(sorted_sample_ints.front()) - 0.5);
  EXPECT_EQ(0u, dictionary_->getLowerBoundCodeForTypedValue(value, value_type));
  EXPECT_EQ(0u, dictionary_->getUpperBoundCodeForTypedValue(value, value_type));

  // Check a value larger than the greatest value in the dictionary.
  value = TypedValue(static_cast<double>(sorted_sample_ints.back()) + 0.5);
  EXPECT_EQ(kNumSampleValues + GetParam(),
            dictionary_->getLowerBoundCodeForTypedValue(value, value_type));
  // Note: with nulls, the upper bound can actually be less than the lower
  // bound due to an implementation quirk.
  EXPECT_EQ(kNumSampleValues,
            dictionary_->getUpperBoundCodeForTypedValue(value, value_type));

  // Split the difference between the last couple of values.
  value = TypedValue(static_cast<double>((sorted_sample_ints.back() + *(sorted_sample_ints.end() - 2)) / 2.0));
  EXPECT_EQ(kNumSampleValues - 1,
            dictionary_->getLowerBoundCodeForTypedValue(value, value_type));
  EXPECT_EQ(kNumSampleValues - 1,
            dictionary_->getUpperBoundCodeForTypedValue(value, value_type));
}

TEST_P(CompressionDictionaryTest, IntKeyLimitCodesForComparisonUntypedTest) {
  setupSampleIntDictionary();

  vector<int> sorted_sample_ints(kSampleInts, kSampleInts + kNumSampleValues);
  sort(sorted_sample_ints.begin(), sorted_sample_ints.end());

  pair<uint32_t, uint32_t> limits;
  for (uint32_t sorted_value_idx = 0;
       sorted_value_idx < kNumSampleValues;
       ++sorted_value_idx) {
    limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kEqual,
                                                            &(sorted_sample_ints[sorted_value_idx]));
    EXPECT_EQ(sorted_value_idx, limits.first);
    EXPECT_EQ(sorted_value_idx + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kLess,
                                                            &(sorted_sample_ints[sorted_value_idx]));
    EXPECT_EQ(0u, limits.first);
    EXPECT_EQ(sorted_value_idx, limits.second);

    limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kLessOrEqual,
                                                            &(sorted_sample_ints[sorted_value_idx]));
    EXPECT_EQ(0u, limits.first);
    EXPECT_EQ(sorted_value_idx + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kGreater,
                                                            &(sorted_sample_ints[sorted_value_idx]));
    EXPECT_EQ(sorted_value_idx + 1, limits.first);
    EXPECT_EQ(kNumSampleValues, limits.second);

    limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kGreaterOrEqual,
                                                            &(sorted_sample_ints[sorted_value_idx]));
    EXPECT_EQ(sorted_value_idx, limits.first);
    EXPECT_EQ(kNumSampleValues, limits.second);
  }

  // Check a value smaller than all the values in the dictionary.
  int small_int = sorted_sample_ints.front() - 1;
  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kEqual, &small_int);
  EXPECT_GE(limits.first, limits.second);

  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kLess, &small_int);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(0u, limits.second);

  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kLessOrEqual, &small_int);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(0u, limits.second);

  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kGreater, &small_int);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kGreaterOrEqual, &small_int);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  // Check a value larger than all the values in the dictionary.
  int big_int = sorted_sample_ints.back() + 1;
  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kEqual, &big_int);
  EXPECT_GE(limits.first, limits.second);

  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kLess, &big_int);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kLessOrEqual, &big_int);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kGreater, &big_int);
  EXPECT_GE(limits.first, limits.second);

  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kGreaterOrEqual, &big_int);
  EXPECT_GE(limits.first, limits.second);

  if (GetParam()) {
    // Any comparison with null has no matches.
    limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kEqual, nullptr);
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kLess, nullptr);
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kLessOrEqual, nullptr);
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kGreater, nullptr);
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kGreaterOrEqual, nullptr);
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);
  }

  // Check kEqual for a nonexistent value.
  int other_value = 777;
  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kEqual, &other_value);
  EXPECT_EQ(limits.first, limits.second);
}

TEST_P(CompressionDictionaryTest, IntKeyLimitCodesForComparisonTypedTest) {
  setupSampleIntDictionary();

  vector<int> sorted_sample_ints(kSampleInts, kSampleInts + kNumSampleValues);
  sort(sorted_sample_ints.begin(), sorted_sample_ints.end());

  pair<uint32_t, uint32_t> limits;
  for (uint32_t sorted_value_idx = 0;
       sorted_value_idx < kNumSampleValues;
       ++sorted_value_idx) {
    TypedValue value(sorted_sample_ints[sorted_value_idx]);
    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kEqual,
                                                          value,
                                                          *dictionary_type_);
    EXPECT_EQ(sorted_value_idx, limits.first);
    EXPECT_EQ(sorted_value_idx + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLess,
                                                          value,
                                                          *dictionary_type_);
    EXPECT_EQ(0u, limits.first);
    EXPECT_EQ(sorted_value_idx, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLessOrEqual,
                                                          value,
                                                          *dictionary_type_);
    EXPECT_EQ(0u, limits.first);
    EXPECT_EQ(sorted_value_idx + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreater,
                                                          value,
                                                          *dictionary_type_);
    EXPECT_EQ(sorted_value_idx + 1, limits.first);
    EXPECT_EQ(kNumSampleValues, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreaterOrEqual,
                                                          value,
                                                          *dictionary_type_);
    EXPECT_EQ(sorted_value_idx, limits.first);
    EXPECT_EQ(kNumSampleValues, limits.second);
  }

  // Check a value smaller than all the values in the dictionary.
  TypedValue small_int_value(sorted_sample_ints.front() - 1);
  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kEqual,
                                                        small_int_value,
                                                        *dictionary_type_);
  EXPECT_GE(limits.first, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLess,
                                                        small_int_value,
                                                        *dictionary_type_);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(0u, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLessOrEqual,
                                                        small_int_value,
                                                        *dictionary_type_);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(0u, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreater,
                                                        small_int_value,
                                                        *dictionary_type_);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreaterOrEqual,
                                                        small_int_value,
                                                        *dictionary_type_);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  // Check a value larger than all the values in the dictionary.
  TypedValue big_int_value(sorted_sample_ints.back() + 1);
  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kEqual,
                                                        big_int_value,
                                                        *dictionary_type_);
  EXPECT_GE(limits.first, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLess,
                                                        big_int_value,
                                                        *dictionary_type_);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLessOrEqual,
                                                        big_int_value,
                                                        *dictionary_type_);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreater,
                                                        big_int_value,
                                                        *dictionary_type_);
  EXPECT_GE(limits.first, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreaterOrEqual,
                                                        big_int_value,
                                                        *dictionary_type_);
  EXPECT_GE(limits.first, limits.second);

  if (GetParam()) {
    // Any comparison with null has no matches.
    TypedValue null_value(kInt);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kEqual,
                                                          null_value,
                                                          *dictionary_type_);
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLess,
                                                          null_value,
                                                          *dictionary_type_);
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLessOrEqual,
                                                          null_value,
                                                          *dictionary_type_);
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreater,
                                                          null_value,
                                                          *dictionary_type_);
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreaterOrEqual,
                                                          null_value,
                                                          *dictionary_type_);
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);
  }

  // Check kEqual for a nonexistent value.
  TypedValue other_value(777);
  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kEqual,
                                                        other_value,
                                                        *dictionary_type_);
  EXPECT_EQ(limits.first, limits.second);
}

// Same as a above, but use DoubleType literals.
TEST_P(CompressionDictionaryTest, IntKeyLimitCodesForComparisonDifferentTypeTest) {
  setupSampleIntDictionary();
  const Type &literal_type = DoubleType::Instance(GetParam());

  vector<int> sorted_sample_ints(kSampleInts, kSampleInts + kNumSampleValues);
  sort(sorted_sample_ints.begin(), sorted_sample_ints.end());

  pair<uint32_t, uint32_t> limits;
  for (uint32_t sorted_value_idx = 0;
       sorted_value_idx < kNumSampleValues;
       ++sorted_value_idx) {
    TypedValue value(static_cast<double>(sorted_sample_ints[sorted_value_idx]));
    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kEqual,
                                                          value,
                                                          literal_type);
    EXPECT_EQ(sorted_value_idx, limits.first);
    EXPECT_EQ(sorted_value_idx + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLess,
                                                          value,
                                                          literal_type);
    EXPECT_EQ(0u, limits.first);
    EXPECT_EQ(sorted_value_idx, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLessOrEqual,
                                                          value,
                                                          literal_type);
    EXPECT_EQ(0u, limits.first);
    EXPECT_EQ(sorted_value_idx + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreater,
                                                          value,
                                                          literal_type);
    EXPECT_EQ(sorted_value_idx + 1, limits.first);
    EXPECT_EQ(kNumSampleValues, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreaterOrEqual,
                                                          value,
                                                          literal_type);
    EXPECT_EQ(sorted_value_idx, limits.first);
    EXPECT_EQ(kNumSampleValues, limits.second);
  }

  // Check a value smaller than all the values in the dictionary.
  TypedValue small_double_value(static_cast<double>(sorted_sample_ints.front()) - 0.5);
  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kEqual,
                                                        small_double_value,
                                                        literal_type);
  EXPECT_GE(limits.first, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLess,
                                                        small_double_value,
                                                        literal_type);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(0u, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLessOrEqual,
                                                        small_double_value,
                                                        literal_type);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(0u, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreater,
                                                        small_double_value,
                                                        literal_type);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreaterOrEqual,
                                                        small_double_value,
                                                        literal_type);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  // Check a value larger than all the values in the dictionary.
  TypedValue big_double_value(static_cast<double>(sorted_sample_ints.back()) + 0.5);
  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kEqual,
                                                        big_double_value,
                                                        literal_type);
  EXPECT_GE(limits.first, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLess,
                                                        big_double_value,
                                                        literal_type);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLessOrEqual,
                                                        big_double_value,
                                                        literal_type);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreater,
                                                        big_double_value,
                                                        literal_type);
  EXPECT_GE(limits.first, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreaterOrEqual,
                                                        big_double_value,
                                                        literal_type);
  EXPECT_GE(limits.first, limits.second);

  if (GetParam()) {
    // Any comparison with null has no matches.
    TypedValue null_value(kDouble);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kEqual,
                                                          null_value,
                                                          literal_type);
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLess,
                                                          null_value,
                                                          literal_type);
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLessOrEqual,
                                                          null_value,
                                                          literal_type);
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreater,
                                                          null_value,
                                                          literal_type);
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreaterOrEqual,
                                                          null_value,
                                                          literal_type);
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);
  }

  // Check kEqual for a nonexistent value.
  TypedValue other_value(static_cast<double>(5.5));
  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kEqual,
                                                        other_value,
                                                        literal_type);
  EXPECT_EQ(limits.first, limits.second);
}

TEST_P(CompressionDictionaryTest, VarCharKeyBuildTest) {
  const VarCharType &varchar_type = VarCharType::Instance(kVarCharSizeParameter, GetParam());
  TypedValue current_value;
  size_t total_key_length = 0;

  // Construct a CompressionDictionaryBuilder and check its initial state.
  setupBuilderForType(varchar_type);
  EXPECT_EQ(0u, builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(0), builder_->codeLengthBits());
  EXPECT_EQ(sizeof(uint32_t) * 2 + total_key_length, builder_->dictionarySizeBytes());

  // If the type is nullable, try inserting a null first.
  if (GetParam()) {
    current_value = TypedValue(kVarChar);
    EXPECT_TRUE(builder_->insertEntry(current_value));
    EXPECT_TRUE(builder_->containsValue(current_value));
    EXPECT_EQ(1u, builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(1), builder_->codeLengthBits());
    EXPECT_EQ(2 * sizeof(uint32_t) + total_key_length, builder_->dictionarySizeBytes());

    // Roll back to zero entries.
    builder_->undoLastInsert();
    EXPECT_FALSE(builder_->containsValue(current_value));
    EXPECT_EQ(0u, builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(0), builder_->codeLengthBits());
    EXPECT_EQ(2 * sizeof(uint32_t) + total_key_length, builder_->dictionarySizeBytes());
  }

  // Add a value.
  current_value = varchar_type.makeValue(kSampleStrings[0], strlen(kSampleStrings[0]) + 1);
  current_value.ensureNotReference();
  total_key_length += current_value.getDataSize();
  EXPECT_TRUE(builder_->insertEntry(current_value));
  EXPECT_TRUE(builder_->containsValue(current_value));
  EXPECT_EQ(1u, builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(1), builder_->codeLengthBits());
  EXPECT_EQ(sizeof(uint32_t) * 3 + total_key_length, builder_->dictionarySizeBytes());

  // Roll back to zero entries.
  total_key_length -= current_value.getDataSize();
  builder_->undoLastInsert();
  EXPECT_FALSE(builder_->containsValue(current_value));
  EXPECT_EQ(0u, builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(0), builder_->codeLengthBits());
  EXPECT_EQ(sizeof(uint32_t) * 2 + total_key_length, builder_->dictionarySizeBytes());

  // Re-add the value.
  total_key_length += current_value.getDataSize();
  EXPECT_TRUE(builder_->insertEntry(current_value));
  EXPECT_TRUE(builder_->containsValue(current_value));
  EXPECT_EQ(1u, builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(1), builder_->codeLengthBits());
  EXPECT_EQ(sizeof(uint32_t) * 3 + total_key_length, builder_->dictionarySizeBytes());

  // If the type is nullable, also try inserting a null with a real value
  // present.
  if (GetParam()) {
    current_value = TypedValue(kVarChar);
    EXPECT_TRUE(builder_->insertEntry(current_value));
    EXPECT_TRUE(builder_->containsValue(current_value));
    EXPECT_EQ(2u, builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(2), builder_->codeLengthBits());
    EXPECT_EQ(3 * sizeof(uint32_t) + total_key_length, builder_->dictionarySizeBytes());

    // Roll back insert of null.
    builder_->undoLastInsert();
    EXPECT_FALSE(builder_->containsValue(current_value));
    EXPECT_EQ(1u, builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(1), builder_->codeLengthBits());
    EXPECT_EQ(3 * sizeof(uint32_t) + total_key_length, builder_->dictionarySizeBytes());

    // Finally, re-insert the null.
    EXPECT_TRUE(builder_->insertEntry(current_value));
    EXPECT_TRUE(builder_->containsValue(current_value));
    EXPECT_EQ(2u, builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(2), builder_->codeLengthBits());
    EXPECT_EQ(3 * sizeof(uint32_t) + total_key_length, builder_->dictionarySizeBytes());
  }

  // Add a reference value.
  current_value = varchar_type.makeValue(kSampleStrings[1], strlen(kSampleStrings[1]) + 1);
  EXPECT_FALSE(current_value.ownsOutOfLineData());
  total_key_length += current_value.getDataSize();
  EXPECT_TRUE(builder_->insertEntry(current_value));
  // Check against a different but equal value.
  current_value = varchar_type.makeValue(kSampleStrings[1], strlen(kSampleStrings[1]) + 1);
  current_value.ensureNotReference();
  EXPECT_TRUE(current_value.ownsOutOfLineData());
  EXPECT_TRUE(builder_->containsValue(current_value));
  EXPECT_EQ(2u + GetParam(), builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(2 + GetParam()), builder_->codeLengthBits());
  EXPECT_EQ(sizeof(uint32_t) * 4 + total_key_length, builder_->dictionarySizeBytes());

  // Roll it back.
  total_key_length -= current_value.getDataSize();
  builder_->undoLastInsert();
  EXPECT_FALSE(builder_->containsValue(current_value));
  EXPECT_EQ(1u + GetParam(), builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(1 + GetParam()), builder_->codeLengthBits());
  EXPECT_EQ(sizeof(uint32_t) * 3 + total_key_length, builder_->dictionarySizeBytes());

  // Re-add it.
  current_value = varchar_type.makeValue(kSampleStrings[1], strlen(kSampleStrings[1]) + 1);
  EXPECT_FALSE(current_value.ownsOutOfLineData());
  total_key_length += current_value.getDataSize();
  EXPECT_TRUE(builder_->insertEntry(current_value));
  EXPECT_TRUE(builder_->containsValue(current_value));
  EXPECT_EQ(2u + GetParam(), builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(2 + GetParam()), builder_->codeLengthBits());
  EXPECT_EQ(sizeof(uint32_t) * 4 + total_key_length, builder_->dictionarySizeBytes());

  // Add a third value.
  current_value = varchar_type.makeValue(kSampleStrings[2], strlen(kSampleStrings[2]) + 1);
  current_value.ensureNotReference();
  total_key_length += current_value.getDataSize();
  EXPECT_TRUE(builder_->insertEntry(current_value));
  EXPECT_TRUE(builder_->containsValue(current_value));
  EXPECT_EQ(3u + GetParam(), builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(3 + GetParam()), builder_->codeLengthBits());
  EXPECT_EQ(sizeof(uint32_t) * 5 + total_key_length, builder_->dictionarySizeBytes());

  // Roll it back (this is the last time we will check this, we just need to
  // cover all the possible paths where the code length may shrink).
  builder_->undoLastInsert();
  total_key_length -= current_value.getDataSize();
  EXPECT_FALSE(builder_->containsValue(current_value));
  EXPECT_EQ(2u + GetParam(), builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(2 + GetParam()), builder_->codeLengthBits());
  EXPECT_EQ(sizeof(uint32_t) * 4 + total_key_length, builder_->dictionarySizeBytes());

  // Insert the rest of the sample values.
  for (uint32_t sample_idx = 2; sample_idx < kNumSampleValues; ++sample_idx) {
    current_value = varchar_type.makeValue(kSampleStrings[sample_idx],
                                           strlen(kSampleStrings[sample_idx]) + 1);
    total_key_length += current_value.getDataSize();
    EXPECT_TRUE(builder_->insertEntry(current_value));
    EXPECT_TRUE(builder_->containsValue(current_value));
    EXPECT_EQ(sample_idx + 1 + GetParam(), builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(sample_idx + 1 + GetParam()), builder_->codeLengthBits());
    EXPECT_EQ(sizeof(uint32_t) * (sample_idx + 3) + total_key_length,
              builder_->dictionarySizeBytes());
  }

  // Attempt to insert a duplicate entry.
  current_value = varchar_type.makeValue(kSampleStrings[kNumSampleValues >> 1],
                                         strlen(kSampleStrings[kNumSampleValues >> 1]) + 1);
  EXPECT_FALSE(builder_->insertEntry(current_value));
  EXPECT_TRUE(builder_->containsValue(current_value));
  EXPECT_EQ(kNumSampleValues + GetParam(), builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(kNumSampleValues + GetParam()), builder_->codeLengthBits());
  EXPECT_EQ(sizeof(uint32_t) * (kNumSampleValues + 2) + total_key_length,
            builder_->dictionarySizeBytes());

  // If type is nullable, attempt to insert a duplicate null.
  if (GetParam()) {
    current_value = TypedValue(kVarChar);
    EXPECT_FALSE(builder_->insertEntry(current_value));
    EXPECT_TRUE(builder_->containsValue(current_value));
    EXPECT_EQ(kNumSampleValues + GetParam(), builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(kNumSampleValues + GetParam()), builder_->codeLengthBits());
    EXPECT_EQ(sizeof(uint32_t) * (kNumSampleValues + 2) + total_key_length,
              builder_->dictionarySizeBytes());
  }

  // Check for the presence of a nonexistent key.
  current_value = varchar_type.makeValue("foo", 4);
  EXPECT_FALSE(builder_->containsValue(current_value));

  // Actually build the physical dictionary.
  dictionary_memory_.reset(builder_->dictionarySizeBytes());
  builder_->buildDictionary(dictionary_memory_.get());

  // Check the contents of the physical dictionary.
  vector<string> sorted_sample_strings(kSampleStrings, kSampleStrings + kNumSampleValues);
  sort(sorted_sample_strings.begin(), sorted_sample_strings.end());

  const uint32_t *offset_array = static_cast<const uint32_t*>(dictionary_memory_.get()) + 2;
  const char *value_memory =
      static_cast<const char*>(dictionary_memory_.get())
      + (*static_cast<const uint32_t*>(dictionary_memory_.get()) + 2) * sizeof(uint32_t);

  EXPECT_EQ(kNumSampleValues, *static_cast<const uint32_t*>(dictionary_memory_.get()));
  if (GetParam()) {
    EXPECT_EQ(kNumSampleValues,
              *(static_cast<const uint32_t*>(dictionary_memory_.get()) + 1));
  } else {
    EXPECT_EQ(numeric_limits<uint32_t>::max(),
              *(static_cast<const uint32_t*>(dictionary_memory_.get()) + 1));
  }
  for (uint32_t sample_idx = 0; sample_idx < kNumSampleValues; ++sample_idx) {
    EXPECT_STREQ(sorted_sample_strings[sample_idx].c_str(),
                 value_memory + offset_array[sample_idx]);
  }
}

// Same test as above, but insert values via insertEntryByReference().
TEST_P(CompressionDictionaryTest, VarCharKeyBuildInsertByReferenceTest) {
  const VarCharType &varchar_type = VarCharType::Instance(kVarCharSizeParameter, GetParam());
  std::vector<TypedValue> values;
  TypedValue other_value;
  size_t total_key_length = 0;

  // Construct a CompressionDictionaryBuilder and check its initial state.
  setupBuilderForType(varchar_type);
  EXPECT_EQ(0u, builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(0), builder_->codeLengthBits());
  EXPECT_EQ(sizeof(uint32_t) * 2 + total_key_length, builder_->dictionarySizeBytes());

  // If the type is nullable, try inserting a null first.
  if (GetParam()) {
    TypedValue null_value = TypedValue(kVarChar);
    EXPECT_TRUE(builder_->insertEntryByReference(null_value));
    EXPECT_TRUE(builder_->containsValue(null_value));
    EXPECT_EQ(1u, builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(1), builder_->codeLengthBits());
    EXPECT_EQ(2 * sizeof(uint32_t) + total_key_length, builder_->dictionarySizeBytes());

    // Roll back to zero entries.
    builder_->undoLastInsert();
    EXPECT_FALSE(builder_->containsValue(null_value));
    EXPECT_EQ(0u, builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(0), builder_->codeLengthBits());
    EXPECT_EQ(2 * sizeof(uint32_t) + total_key_length, builder_->dictionarySizeBytes());
  }

  // Add a value.
  values.push_back(varchar_type.makeValue(kSampleStrings[0], strlen(kSampleStrings[0]) + 1));
  values.back().ensureNotReference();
  total_key_length += values.back().getDataSize();
  EXPECT_TRUE(builder_->insertEntryByReference(values.back()));
  EXPECT_TRUE(builder_->containsValue(values.back()));
  EXPECT_EQ(1u, builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(1), builder_->codeLengthBits());
  EXPECT_EQ(sizeof(uint32_t) * 3 + total_key_length, builder_->dictionarySizeBytes());

  // Roll back to zero entries.
  total_key_length -= values.back().getDataSize();
  builder_->undoLastInsert();
  EXPECT_FALSE(builder_->containsValue(values.back()));
  EXPECT_EQ(0u, builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(0), builder_->codeLengthBits());
  EXPECT_EQ(sizeof(uint32_t) * 2 + total_key_length, builder_->dictionarySizeBytes());

  // Re-add the value.
  total_key_length += values.back().getDataSize();
  EXPECT_TRUE(builder_->insertEntryByReference(values.back()));
  EXPECT_TRUE(builder_->containsValue(values.back()));
  EXPECT_EQ(1u, builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(1), builder_->codeLengthBits());
  EXPECT_EQ(sizeof(uint32_t) * 3 + total_key_length, builder_->dictionarySizeBytes());

  // If the type is nullable, also try inserting a null with a real value
  // present.
  if (GetParam()) {
    TypedValue null_value = TypedValue(kVarChar);
    EXPECT_TRUE(builder_->insertEntryByReference(null_value));
    EXPECT_TRUE(builder_->containsValue(null_value));
    EXPECT_EQ(2u, builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(2), builder_->codeLengthBits());
    EXPECT_EQ(3 * sizeof(uint32_t) + total_key_length, builder_->dictionarySizeBytes());

    // Roll back insert of null.
    builder_->undoLastInsert();
    EXPECT_FALSE(builder_->containsValue(null_value));
    EXPECT_EQ(1u, builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(1), builder_->codeLengthBits());
    EXPECT_EQ(3 * sizeof(uint32_t) + total_key_length, builder_->dictionarySizeBytes());

    // Finally, re-insert the null.
    EXPECT_TRUE(builder_->insertEntryByReference(null_value));
    EXPECT_TRUE(builder_->containsValue(null_value));
    EXPECT_EQ(2u, builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(2), builder_->codeLengthBits());
    EXPECT_EQ(3 * sizeof(uint32_t) + total_key_length, builder_->dictionarySizeBytes());
  }

  // Add a reference value.
  values.push_back(varchar_type.makeValue(kSampleStrings[1], strlen(kSampleStrings[1]) + 1));
  total_key_length += values.back().getDataSize();
  EXPECT_TRUE(builder_->insertEntryByReference(values.back()));
  // Check against a different but equal value.
  other_value = varchar_type.makeValue(kSampleStrings[1], strlen(kSampleStrings[1]) + 1);
  other_value.ensureNotReference();
  EXPECT_TRUE(builder_->containsValue(other_value));
  EXPECT_EQ(2u + GetParam(), builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(2 + GetParam()), builder_->codeLengthBits());
  EXPECT_EQ(sizeof(uint32_t) * 4 + total_key_length, builder_->dictionarySizeBytes());

  // Roll it back.
  total_key_length -= values.back().getDataSize();
  builder_->undoLastInsert();
  EXPECT_FALSE(builder_->containsValue(values.back()));
  EXPECT_EQ(1u + GetParam(), builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(1 + GetParam()), builder_->codeLengthBits());
  EXPECT_EQ(sizeof(uint32_t) * 3 + total_key_length, builder_->dictionarySizeBytes());

  // Re-add it.
  total_key_length += values.back().getDataSize();
  EXPECT_TRUE(builder_->insertEntryByReference(values.back()));
  EXPECT_TRUE(builder_->containsValue(values.back()));
  EXPECT_EQ(2u + GetParam(), builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(2 + GetParam()), builder_->codeLengthBits());
  EXPECT_EQ(sizeof(uint32_t) * 4 + total_key_length, builder_->dictionarySizeBytes());

  // Add a third value.
  values.push_back(varchar_type.makeValue(kSampleStrings[2], strlen(kSampleStrings[2]) + 1));
  total_key_length += values.back().getDataSize();
  EXPECT_TRUE(builder_->insertEntryByReference(values.back()));
  EXPECT_TRUE(builder_->containsValue(values.back()));
  EXPECT_EQ(3u + GetParam(), builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(3 + GetParam()), builder_->codeLengthBits());
  EXPECT_EQ(sizeof(uint32_t) * 5 + total_key_length, builder_->dictionarySizeBytes());

  // Roll it back (this is the last time we will check this, we just need to
  // cover all the possible paths where the code length may shrink).
  builder_->undoLastInsert();
  total_key_length -= values.back().getDataSize();
  EXPECT_FALSE(builder_->containsValue(values.back()));
  EXPECT_EQ(2u + GetParam(), builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(2 + GetParam()), builder_->codeLengthBits());
  EXPECT_EQ(sizeof(uint32_t) * 4 + total_key_length, builder_->dictionarySizeBytes());

  // Insert the rest of the sample values.
  for (uint32_t sample_idx = 2; sample_idx < kNumSampleValues; ++sample_idx) {
    values.push_back(varchar_type.makeValue(kSampleStrings[sample_idx],
                                            strlen(kSampleStrings[sample_idx]) + 1));
    total_key_length += values.back().getDataSize();
    EXPECT_TRUE(builder_->insertEntryByReference(values.back()));
    EXPECT_TRUE(builder_->containsValue(values.back()));
    EXPECT_EQ(sample_idx + 1 + GetParam(), builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(sample_idx + 1 + GetParam()), builder_->codeLengthBits());
    EXPECT_EQ(sizeof(uint32_t) * (sample_idx + 3) + total_key_length,
              builder_->dictionarySizeBytes());
  }

  // Attempt to insert a duplicate entry.
  other_value = varchar_type.makeValue(kSampleStrings[kNumSampleValues >> 1],
                                       strlen(kSampleStrings[kNumSampleValues >> 1]) + 1);
  EXPECT_FALSE(builder_->insertEntryByReference(other_value));
  EXPECT_TRUE(builder_->containsValue(other_value));
  EXPECT_EQ(kNumSampleValues + GetParam(), builder_->numberOfEntries());
  EXPECT_EQ(bitsNeededForCodes(kNumSampleValues + GetParam()), builder_->codeLengthBits());
  EXPECT_EQ(sizeof(uint32_t) * (kNumSampleValues + 2) + total_key_length,
            builder_->dictionarySizeBytes());

  // If type is nullable, attempt to insert a duplicate null.
  if (GetParam()) {
    TypedValue null_value = TypedValue(kVarChar);
    EXPECT_FALSE(builder_->insertEntryByReference(null_value));
    EXPECT_TRUE(builder_->containsValue(null_value));
    EXPECT_EQ(kNumSampleValues + GetParam(), builder_->numberOfEntries());
    EXPECT_EQ(bitsNeededForCodes(kNumSampleValues + GetParam()), builder_->codeLengthBits());
    EXPECT_EQ(sizeof(uint32_t) * (kNumSampleValues + 2) + total_key_length,
              builder_->dictionarySizeBytes());
  }

  // Check for the presence of a nonexistent key.
  other_value = varchar_type.makeValue("foo", 4);
  EXPECT_FALSE(builder_->containsValue(other_value));

  // Actually build the physical dictionary.
  dictionary_memory_.reset(builder_->dictionarySizeBytes());
  builder_->buildDictionary(dictionary_memory_.get());

  // Check the contents of the physical dictionary.
  vector<string> sorted_sample_strings(kSampleStrings, kSampleStrings + kNumSampleValues);
  sort(sorted_sample_strings.begin(), sorted_sample_strings.end());

  const uint32_t *offset_array = static_cast<const uint32_t*>(dictionary_memory_.get()) + 2;
  const char *value_memory =
      static_cast<const char*>(dictionary_memory_.get())
      + (*static_cast<const uint32_t*>(dictionary_memory_.get()) + 2) * sizeof(uint32_t);

  EXPECT_EQ(kNumSampleValues, *static_cast<const uint32_t*>(dictionary_memory_.get()));
  if (GetParam()) {
    EXPECT_EQ(kNumSampleValues,
              *(static_cast<const uint32_t*>(dictionary_memory_.get()) + 1));
  } else {
    EXPECT_EQ(numeric_limits<uint32_t>::max(),
              *(static_cast<const uint32_t*>(dictionary_memory_.get()) + 1));
  }
  for (uint32_t sample_idx = 0; sample_idx < kNumSampleValues; ++sample_idx) {
    EXPECT_STREQ(sorted_sample_strings[sample_idx].c_str(),
                 value_memory + offset_array[sample_idx]);
  }
}

TEST_P(CompressionDictionaryTest, VarCharKeyInfoTest) {
  setupSampleVarCharDictionary();
  EXPECT_EQ(kNumSampleValues + GetParam(), dictionary_->numberOfCodes());
  EXPECT_EQ(bitsNeededForCodes(kNumSampleValues + GetParam()), dictionary_->codeLengthBits());
}

TEST_P(CompressionDictionaryTest, VarCharKeyGetCodeUntypedTest) {
  setupSampleVarCharDictionary();

  vector<string> sorted_sample_strings(kSampleStrings, kSampleStrings + kNumSampleValues);
  sort(sorted_sample_strings.begin(), sorted_sample_strings.end());

  // Check all the codes in the dictionary.
  for (uint32_t sample_idx = 0; sample_idx < kNumSampleValues; ++sample_idx) {
    EXPECT_EQ(sample_idx,
              dictionary_->getCodeForUntypedValue(sorted_sample_strings[sample_idx].c_str()));
  }

  if (GetParam()) {
    EXPECT_EQ(kNumSampleValues, dictionary_->getCodeForUntypedValue(nullptr));
  }

  // Check a value that isn't in the dictionary.
  EXPECT_EQ(kNumSampleValues + GetParam(), dictionary_->getCodeForUntypedValue("foo"));
}

TEST_P(CompressionDictionaryTest, VarCharKeyGetCodeTypedTest) {
  setupSampleVarCharDictionary();

  vector<string> sorted_sample_strings(kSampleStrings, kSampleStrings + kNumSampleValues);
  sort(sorted_sample_strings.begin(), sorted_sample_strings.end());

  // Check all the codes in the dictionary.
  const VarCharType &varchar_type = VarCharType::InstanceNonNullable(kVarCharSizeParameter);
  for (uint32_t sample_idx = 0; sample_idx < kNumSampleValues; ++sample_idx) {
    TypedValue value = varchar_type.makeValue(sorted_sample_strings[sample_idx].c_str(),
                                              sorted_sample_strings[sample_idx].size() + 1);
    EXPECT_EQ(sample_idx, dictionary_->getCodeForTypedValue(value, varchar_type));
  }

  if (GetParam()) {
    EXPECT_EQ(kNumSampleValues,
              dictionary_->getCodeForTypedValue(TypedValue(kVarChar), *dictionary_type_));
  }

  // Check a value that isn't in the dictionary.
  TypedValue other_value = varchar_type.makeValue("foo", 4);
  EXPECT_EQ(kNumSampleValues + GetParam(),
            dictionary_->getCodeForTypedValue(other_value, varchar_type));
}

// Same as above, but use CharType literals.
TEST_P(CompressionDictionaryTest, VarCharKeyGetCodeDifferentTypeTest) {
  setupSampleVarCharDictionary();

  vector<string> sorted_sample_strings(kSampleStrings, kSampleStrings + kNumSampleValues);
  sort(sorted_sample_strings.begin(), sorted_sample_strings.end());

  // Check all the codes in the dictionary.
  const CharType &char_type = CharType::InstanceNonNullable(kVarCharSizeParameter + 10);
  for (uint32_t sample_idx = 0; sample_idx < kNumSampleValues; ++sample_idx) {
    TypedValue value = char_type.makeValue(sorted_sample_strings[sample_idx].c_str(),
                                           sorted_sample_strings[sample_idx].size() + 1);
    EXPECT_EQ(sample_idx, dictionary_->getCodeForTypedValue(value, char_type));
  }

  if (GetParam()) {
    EXPECT_EQ(kNumSampleValues,
              dictionary_->getCodeForTypedValue(TypedValue(kChar),
                                                CharType::InstanceNullable(kVarCharSizeParameter)));
  }

  // Check a value that isn't in the dictionary.
  TypedValue other_value = char_type.makeValue("foo", 4);
  EXPECT_EQ(kNumSampleValues + GetParam(),
            dictionary_->getCodeForTypedValue(other_value, char_type));
}

TEST_P(CompressionDictionaryTest, VarCharKeyGetValueUntypedTest) {
  setupSampleVarCharDictionary();

  vector<string> sorted_sample_strings(kSampleStrings, kSampleStrings + kNumSampleValues);
  sort(sorted_sample_strings.begin(), sorted_sample_strings.end());

  for (uint8_t code = 0; code < kNumSampleValues; ++code) {
    const char *value_ptr = static_cast<const char*>(dictionary_->getUntypedValueForByteCode(code));
    EXPECT_STREQ(sorted_sample_strings[code].c_str(), value_ptr);
    value_ptr = static_cast<const char*>(dictionary_->getUntypedValueForShortCode(code));
    EXPECT_STREQ(sorted_sample_strings[code].c_str(), value_ptr);
    value_ptr = static_cast<const char*>(dictionary_->getUntypedValueForCode(code));
    EXPECT_STREQ(sorted_sample_strings[code].c_str(), value_ptr);
  }

  if (GetParam()) {
    const void *value_ptr = dictionary_->getUntypedValueForByteCode(kNumSampleValues);
    EXPECT_EQ(nullptr, value_ptr);
    value_ptr = dictionary_->getUntypedValueForShortCode(kNumSampleValues);
    EXPECT_EQ(nullptr, value_ptr);
    value_ptr = dictionary_->getUntypedValueForCode(kNumSampleValues);
    EXPECT_EQ(nullptr, value_ptr);
  }
}

TEST_P(CompressionDictionaryTest, VarCharKeyGetValueTypedTest) {
  setupSampleVarCharDictionary();

  vector<string> sorted_sample_strings(kSampleStrings, kSampleStrings + kNumSampleValues);
  sort(sorted_sample_strings.begin(), sorted_sample_strings.end());

  const VarCharType &varchar_type = VarCharType::Instance(kVarCharSizeParameter, GetParam());
  const Comparison &equal_comp = ComparisonFactory::GetComparison(ComparisonID::kEqual);
  for (uint8_t code = 0; code < kNumSampleValues; ++code) {
    TypedValue expected_value = varchar_type.makeValue(sorted_sample_strings[code].c_str(),
                                                       sorted_sample_strings[code].size());

    TypedValue value = dictionary_->getTypedValueForByteCode(code);
    EXPECT_TRUE(equal_comp.compareTypedValuesChecked(expected_value, varchar_type,
                                                     value, varchar_type));
    value = dictionary_->getTypedValueForShortCode(code);
    EXPECT_TRUE(equal_comp.compareTypedValuesChecked(expected_value, varchar_type,
                                                     value, varchar_type));
    value = dictionary_->getTypedValueForCode(code);
    EXPECT_TRUE(equal_comp.compareTypedValuesChecked(expected_value, varchar_type,
                                                     value, varchar_type));
  }

  if (GetParam()) {
    TypedValue value = dictionary_->getTypedValueForByteCode(kNumSampleValues);
    EXPECT_TRUE(value.isNull());
    EXPECT_EQ(kVarChar, value.getTypeID());
    value = dictionary_->getTypedValueForShortCode(kNumSampleValues);
    EXPECT_TRUE(value.isNull());
    EXPECT_EQ(kVarChar, value.getTypeID());
    value = dictionary_->getTypedValueForCode(kNumSampleValues);
    EXPECT_TRUE(value.isNull());
    EXPECT_EQ(kVarChar, value.getTypeID());
  }
}

TEST_P(CompressionDictionaryTest, VarCharKeyBoundCodesUntypedTest) {
  setupSampleVarCharDictionary();

  vector<string> sorted_sample_strings(kSampleStrings, kSampleStrings + kNumSampleValues);
  sort(sorted_sample_strings.begin(), sorted_sample_strings.end());

  // Check bounds for all the values which are actually in the dictionary.
  for (uint32_t sorted_value_idx = 0;
       sorted_value_idx < kNumSampleValues;
       ++sorted_value_idx) {
    EXPECT_EQ(sorted_value_idx,
              dictionary_->getLowerBoundCodeForUntypedValue(sorted_sample_strings[sorted_value_idx].c_str()));
    EXPECT_EQ(sorted_value_idx + 1,
              dictionary_->getUpperBoundCodeForUntypedValue(sorted_sample_strings[sorted_value_idx].c_str()));
  }

  // Check a value less than the least value in the dictionary.
  string other_value = sorted_sample_strings.front();
  other_value.resize(other_value.size() - 1);
  EXPECT_EQ(0u, dictionary_->getLowerBoundCodeForUntypedValue(other_value.c_str()));
  EXPECT_EQ(0u, dictionary_->getUpperBoundCodeForUntypedValue(other_value.c_str()));

  // Check a value larger than the greatest value in the dictionary.
  other_value = sorted_sample_strings.back();
  other_value.push_back('z');
  EXPECT_EQ(kNumSampleValues + GetParam(),
            dictionary_->getLowerBoundCodeForUntypedValue(other_value.c_str()));
  // Note: with nulls, the upper bound can actually be less than the lower
  // bound due to an implementation quirk.
  EXPECT_EQ(kNumSampleValues,
            dictionary_->getUpperBoundCodeForUntypedValue(other_value.c_str()));

  // Check a value between the last two.
  other_value = *(sorted_sample_strings.end() - 2);
  other_value.push_back('z');
  EXPECT_EQ(kNumSampleValues - 1, dictionary_->getLowerBoundCodeForUntypedValue(other_value.c_str()));
  EXPECT_EQ(kNumSampleValues - 1, dictionary_->getUpperBoundCodeForUntypedValue(other_value.c_str()));
}

TEST_P(CompressionDictionaryTest, VarCharKeyBoundCodesTypedTest) {
  setupSampleVarCharDictionary();

  vector<string> sorted_sample_strings(kSampleStrings, kSampleStrings + kNumSampleValues);
  sort(sorted_sample_strings.begin(), sorted_sample_strings.end());

  const VarCharType &varchar_type = VarCharType::InstanceNonNullable(kVarCharSizeParameter);
  TypedValue value;
  // Check bounds for all the values which are actually in the dictionary.
  for (uint32_t sorted_value_idx = 0;
       sorted_value_idx < kNumSampleValues;
       ++sorted_value_idx) {
    value = varchar_type.makeValue(sorted_sample_strings[sorted_value_idx].c_str(),
                                   sorted_sample_strings[sorted_value_idx].size() + 1);
    EXPECT_EQ(sorted_value_idx,
              dictionary_->getLowerBoundCodeForTypedValue(value, varchar_type));
    EXPECT_EQ(sorted_value_idx + 1,
              dictionary_->getUpperBoundCodeForTypedValue(value, varchar_type));
  }

  // Check a value less than the least value in the dictionary.
  string other_str = sorted_sample_strings.front();
  other_str.resize(other_str.size() - 1);
  value = varchar_type.makeValue(other_str.c_str(), other_str.size() + 1);
  EXPECT_EQ(0u, dictionary_->getLowerBoundCodeForTypedValue(value, varchar_type));
  EXPECT_EQ(0u, dictionary_->getUpperBoundCodeForTypedValue(value, varchar_type));

  // Check a value larger than the greatest value in the dictionary.
  other_str = sorted_sample_strings.back();
  other_str.push_back('z');
  value = varchar_type.makeValue(other_str.c_str(), other_str.size() + 1);
  EXPECT_EQ(kNumSampleValues + GetParam(),
            dictionary_->getLowerBoundCodeForTypedValue(value, varchar_type));
  // Note: with nulls, the upper bound can actually be less than the lower
  // bound due to an implementation quirk.
  EXPECT_EQ(kNumSampleValues,
            dictionary_->getUpperBoundCodeForTypedValue(value, varchar_type));

  // Check a value between the last two.
  other_str = *(sorted_sample_strings.end() - 2);
  other_str.push_back('z');
  value = varchar_type.makeValue(other_str.c_str(), other_str.size() + 1);
  EXPECT_EQ(kNumSampleValues - 1,
            dictionary_->getLowerBoundCodeForTypedValue(value, varchar_type));
  EXPECT_EQ(kNumSampleValues - 1,
            dictionary_->getUpperBoundCodeForTypedValue(value, varchar_type));
}

// Same as above, but use CharType literals.
TEST_P(CompressionDictionaryTest, VarCharKeyBoundCodesDifferentTypeTest) {
  setupSampleVarCharDictionary();

  vector<string> sorted_sample_strings(kSampleStrings, kSampleStrings + kNumSampleValues);
  sort(sorted_sample_strings.begin(), sorted_sample_strings.end());

  const CharType &char_type = CharType::InstanceNonNullable(kVarCharSizeParameter + 10);
  TypedValue value;
  // Check bounds for all the values which are actually in the dictionary.
  for (uint32_t sorted_value_idx = 0;
       sorted_value_idx < kNumSampleValues;
       ++sorted_value_idx) {
    value = char_type.makeValue(sorted_sample_strings[sorted_value_idx].c_str(),
                                sorted_sample_strings[sorted_value_idx].size() + 1);
    EXPECT_EQ(sorted_value_idx,
              dictionary_->getLowerBoundCodeForTypedValue(value, char_type));
    EXPECT_EQ(sorted_value_idx + 1,
              dictionary_->getUpperBoundCodeForTypedValue(value, char_type));
  }

  // Check a value less than the least value in the dictionary.
  string other_str = sorted_sample_strings.front();
  other_str.resize(other_str.size() - 1);
  value = char_type.makeValue(other_str.c_str(), other_str.size() + 1);
  EXPECT_EQ(0u, dictionary_->getLowerBoundCodeForTypedValue(value, char_type));
  EXPECT_EQ(0u, dictionary_->getUpperBoundCodeForTypedValue(value, char_type));

  // Check a value larger than the greatest value in the dictionary.
  other_str = sorted_sample_strings.back();
  other_str.push_back('z');
  value = char_type.makeValue(other_str.c_str(), other_str.size() + 1);
  EXPECT_EQ(kNumSampleValues + GetParam(),
            dictionary_->getLowerBoundCodeForTypedValue(value, char_type));
  // Note: with nulls, the upper bound can actually be less than the lower
  // bound due to an implementation quirk.
  EXPECT_EQ(kNumSampleValues,
            dictionary_->getUpperBoundCodeForTypedValue(value, char_type));

  // Check a value between the last two.
  other_str = *(sorted_sample_strings.end() - 2);
  other_str.push_back('z');
  value = char_type.makeValue(other_str.c_str(), other_str.size() + 1);
  EXPECT_EQ(kNumSampleValues - 1,
            dictionary_->getLowerBoundCodeForTypedValue(value, char_type));
  EXPECT_EQ(kNumSampleValues - 1,
            dictionary_->getUpperBoundCodeForTypedValue(value, char_type));
}

TEST_P(CompressionDictionaryTest, VarCharKeyLimitCodesForComparisonUntypedTest) {
  setupSampleVarCharDictionary();

  vector<string> sorted_sample_strings(kSampleStrings, kSampleStrings + kNumSampleValues);
  sort(sorted_sample_strings.begin(), sorted_sample_strings.end());

  pair<uint32_t, uint32_t> limits;
  for (uint32_t sorted_value_idx = 0;
       sorted_value_idx < kNumSampleValues;
       ++sorted_value_idx) {
    limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kEqual,
                                                            sorted_sample_strings[sorted_value_idx].c_str());
    EXPECT_EQ(sorted_value_idx, limits.first);
    EXPECT_EQ(sorted_value_idx + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kLess,
                                                            sorted_sample_strings[sorted_value_idx].c_str());
    EXPECT_EQ(0u, limits.first);
    EXPECT_EQ(sorted_value_idx, limits.second);

    limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kLessOrEqual,
                                                            sorted_sample_strings[sorted_value_idx].c_str());
    EXPECT_EQ(0u, limits.first);
    EXPECT_EQ(sorted_value_idx + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kGreater,
                                                            sorted_sample_strings[sorted_value_idx].c_str());
    EXPECT_EQ(sorted_value_idx + 1, limits.first);
    EXPECT_EQ(kNumSampleValues, limits.second);

    limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kGreaterOrEqual,
                                                            sorted_sample_strings[sorted_value_idx].c_str());
    EXPECT_EQ(sorted_value_idx, limits.first);
    EXPECT_EQ(kNumSampleValues, limits.second);
  }

  // Check a string that is ordered before everything in the dictionary.
  std::string before_string(sorted_sample_strings.front());
  before_string.pop_back();
  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kEqual, before_string.c_str());
  EXPECT_GE(limits.first, limits.second);

  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kLess, before_string.c_str());
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(0u, limits.second);

  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kLessOrEqual, before_string.c_str());
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(0u, limits.second);

  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kGreater, before_string.c_str());
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kGreaterOrEqual, before_string.c_str());
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  // Check a string that is ordered after everything in the dictionary.
  std::string after_string(sorted_sample_strings.back());
  after_string.push_back('z');
  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kEqual, after_string.c_str());
  EXPECT_GE(limits.first, limits.second);

  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kLess, after_string.c_str());
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kLessOrEqual, after_string.c_str());
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kGreater, after_string.c_str());
  EXPECT_GE(limits.first, limits.second);

  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kGreaterOrEqual, after_string.c_str());
  EXPECT_GE(limits.first, limits.second);

  if (GetParam()) {
    // Any comparison with null has no matches.
    limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kEqual, nullptr);
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kLess, nullptr);
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kLessOrEqual, nullptr);
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kGreater, nullptr);
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kGreaterOrEqual, nullptr);
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);
  }

  // Check kEqual for a nonexistent value.
  limits = dictionary_->getLimitCodesForComparisonUntyped(ComparisonID::kEqual, "foo");
  EXPECT_EQ(limits.first, limits.second);
}

TEST_P(CompressionDictionaryTest, VarCharKeyLimitCodesForComparisonTypedTest) {
  setupSampleVarCharDictionary();

  vector<string> sorted_sample_strings(kSampleStrings, kSampleStrings + kNumSampleValues);
  sort(sorted_sample_strings.begin(), sorted_sample_strings.end());

  const VarCharType &varchar_type = VarCharType::InstanceNonNullable(kVarCharSizeParameter);
  TypedValue value;

  pair<uint32_t, uint32_t> limits;
  for (uint32_t sorted_value_idx = 0;
       sorted_value_idx < kNumSampleValues;
       ++sorted_value_idx) {
    value = varchar_type.makeValue(sorted_sample_strings[sorted_value_idx].c_str(),
                                   sorted_sample_strings[sorted_value_idx].size() + 1);
    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kEqual,
                                                          value,
                                                          varchar_type);
    EXPECT_EQ(sorted_value_idx, limits.first);
    EXPECT_EQ(sorted_value_idx + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLess,
                                                          value,
                                                          varchar_type);
    EXPECT_EQ(0u, limits.first);
    EXPECT_EQ(sorted_value_idx, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLessOrEqual,
                                                          value,
                                                          varchar_type);
    EXPECT_EQ(0u, limits.first);
    EXPECT_EQ(sorted_value_idx + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreater,
                                                          value,
                                                          varchar_type);
    EXPECT_EQ(sorted_value_idx + 1, limits.first);
    EXPECT_EQ(kNumSampleValues, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreaterOrEqual,
                                                          value,
                                                          varchar_type);
    EXPECT_EQ(sorted_value_idx, limits.first);
    EXPECT_EQ(kNumSampleValues, limits.second);
  }

  // Check a string that is ordered before everything in the dictionary.
  std::string before_string(sorted_sample_strings.front());
  before_string.pop_back();
  value = varchar_type.makeValue(before_string.c_str(), before_string.size() + 1);
  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kEqual,
                                                        value,
                                                        varchar_type);
  EXPECT_GE(limits.first, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLess,
                                                        value,
                                                        varchar_type);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(0u, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLessOrEqual,
                                                        value,
                                                        varchar_type);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(0u, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreater,
                                                        value,
                                                        varchar_type);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreaterOrEqual,
                                                        value,
                                                        varchar_type);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  // Check a string that is ordered after everything in the dictionary.
  std::string after_string(sorted_sample_strings.back());
  after_string.push_back('z');
  value = varchar_type.makeValue(after_string.c_str(), after_string.size() + 1);
  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kEqual,
                                                        value,
                                                        varchar_type);
  EXPECT_GE(limits.first, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLess,
                                                        value,
                                                        varchar_type);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLessOrEqual,
                                                        value,
                                                        varchar_type);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreater,
                                                        value,
                                                        varchar_type);
  EXPECT_GE(limits.first, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreaterOrEqual,
                                                        value,
                                                        varchar_type);
  EXPECT_GE(limits.first, limits.second);

  if (GetParam()) {
    // Any comparison with null has no matches.
    TypedValue null_value(kVarChar);

    limits = dictionary_->getLimitCodesForComparisonTyped(
        ComparisonID::kEqual,
        null_value,
        VarCharType::InstanceNullable(kVarCharSizeParameter));
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(
        ComparisonID::kLess,
        null_value,
        VarCharType::InstanceNullable(kVarCharSizeParameter));
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(
        ComparisonID::kLessOrEqual,
        null_value,
        VarCharType::InstanceNullable(kVarCharSizeParameter));
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(
        ComparisonID::kGreater,
        null_value,
        VarCharType::InstanceNullable(kVarCharSizeParameter));
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(
        ComparisonID::kGreaterOrEqual,
        null_value,
        VarCharType::InstanceNullable(kVarCharSizeParameter));
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);
  }

  // Check kEqual for a nonexistent value.
  value = varchar_type.makeValue("foo", 4);
  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kEqual,
                                                        value,
                                                        varchar_type);
  EXPECT_EQ(limits.first, limits.second);
}

// Same as above, but use CharType literals.
TEST_P(CompressionDictionaryTest, VarCharKeyLimitCodesForComparisonDifferentTypeTest) {
  setupSampleVarCharDictionary();

  vector<string> sorted_sample_strings(kSampleStrings, kSampleStrings + kNumSampleValues);
  sort(sorted_sample_strings.begin(), sorted_sample_strings.end());

  const CharType &char_type = CharType::InstanceNonNullable(kVarCharSizeParameter + 10);
  TypedValue value;

  pair<uint32_t, uint32_t> limits;
  for (uint32_t sorted_value_idx = 0;
       sorted_value_idx < kNumSampleValues;
       ++sorted_value_idx) {
    value = char_type.makeValue(sorted_sample_strings[sorted_value_idx].c_str(),
                                sorted_sample_strings[sorted_value_idx].size() + 1);
    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kEqual,
                                                          value,
                                                          char_type);
    EXPECT_EQ(sorted_value_idx, limits.first);
    EXPECT_EQ(sorted_value_idx + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLess,
                                                          value,
                                                          char_type);
    EXPECT_EQ(0u, limits.first);
    EXPECT_EQ(sorted_value_idx, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLessOrEqual,
                                                          value,
                                                          char_type);
    EXPECT_EQ(0u, limits.first);
    EXPECT_EQ(sorted_value_idx + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreater,
                                                          value,
                                                          char_type);
    EXPECT_EQ(sorted_value_idx + 1, limits.first);
    EXPECT_EQ(kNumSampleValues, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreaterOrEqual,
                                                          value,
                                                          char_type);
    EXPECT_EQ(sorted_value_idx, limits.first);
    EXPECT_EQ(kNumSampleValues, limits.second);
  }

  // Check a string that is ordered before everything in the dictionary.
  std::string before_string(sorted_sample_strings.front());
  before_string.pop_back();
  value = char_type.makeValue(before_string.c_str(), before_string.size() + 1);
  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kEqual,
                                                        value,
                                                        char_type);
  EXPECT_GE(limits.first, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLess,
                                                        value,
                                                        char_type);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(0u, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLessOrEqual,
                                                        value,
                                                        char_type);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(0u, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreater,
                                                        value,
                                                        char_type);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreaterOrEqual,
                                                        value,
                                                        char_type);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  // Check a string that is ordered after everything in the dictionary.
  std::string after_string(sorted_sample_strings.back());
  after_string.push_back('z');
  value = char_type.makeValue(after_string.c_str(), after_string.size() + 1);
  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kEqual,
                                                        value,
                                                        char_type);
  EXPECT_GE(limits.first, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLess,
                                                        value,
                                                        char_type);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kLessOrEqual,
                                                        value,
                                                        char_type);
  EXPECT_EQ(0u, limits.first);
  EXPECT_EQ(kNumSampleValues, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreater,
                                                        value,
                                                        char_type);
  EXPECT_GE(limits.first, limits.second);

  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kGreaterOrEqual,
                                                        value,
                                                        char_type);
  EXPECT_GE(limits.first, limits.second);

  if (GetParam()) {
    // Any comparison with null has no matches.
    TypedValue null_value(kChar);

    limits = dictionary_->getLimitCodesForComparisonTyped(
        ComparisonID::kEqual,
        null_value,
        CharType::InstanceNullable(kVarCharSizeParameter));
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(
        ComparisonID::kLess,
        null_value,
        CharType::InstanceNullable(kVarCharSizeParameter));
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(
        ComparisonID::kLessOrEqual,
        null_value,
        CharType::InstanceNullable(kVarCharSizeParameter));
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(
        ComparisonID::kGreater,
        null_value,
        CharType::InstanceNullable(kVarCharSizeParameter));
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);

    limits = dictionary_->getLimitCodesForComparisonTyped(
        ComparisonID::kGreaterOrEqual,
        null_value,
        CharType::InstanceNullable(kVarCharSizeParameter));
    EXPECT_EQ(kNumSampleValues + 1, limits.first);
    EXPECT_EQ(kNumSampleValues + 1, limits.second);
  }

  // Check kEqual for a nonexistent value.
  value = char_type.makeValue("foo", 4);
  limits = dictionary_->getLimitCodesForComparisonTyped(ComparisonID::kEqual,
                                                        value,
                                                        char_type);
  EXPECT_EQ(limits.first, limits.second);
}

// Note: INSTANTIATE_TEST_CASE_P has variadic arguments part. If the variable argument part
//       is empty, C++11 standard says it should produce a warning. A warning is converted
//       to an error since we use -Werror as a compiler parameter. It causes Travis to build.
//       This is the reason that we must give an empty string argument as a last parameter
//       to supress warning that clang gives.
INSTANTIATE_TEST_CASE_P(WithAndWithoutNullValue,
                        CompressionDictionaryTest,
                        ::testing::Bool(),);  // NOLINT(whitespace/comma)

INSTANTIATE_TEST_CASE_P(WithAndWithoutNullValue,
                        CompressionDictionaryDeathTest,
                        ::testing::Bool(),);  // NOLINT(whitespace/comma)

}  // namespace quickstep
