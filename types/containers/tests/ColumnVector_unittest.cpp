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

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <utility>
#include <vector>

#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypedValue.hpp"
#include "types/TypeID.hpp"
#include "types/containers/ColumnVector.hpp"

#include "gtest/gtest.h"

namespace quickstep {

TEST(ColumnVectorTest, NativeUsableForTypeTest) {
  EXPECT_TRUE(NativeColumnVector::UsableForType(TypeFactory::GetType(kInt, false)));
  EXPECT_TRUE(NativeColumnVector::UsableForType(TypeFactory::GetType(kInt, true)));
  EXPECT_TRUE(NativeColumnVector::UsableForType(TypeFactory::GetType(kLong, false)));
  EXPECT_TRUE(NativeColumnVector::UsableForType(TypeFactory::GetType(kLong, true)));
  EXPECT_TRUE(NativeColumnVector::UsableForType(TypeFactory::GetType(kFloat, false)));
  EXPECT_TRUE(NativeColumnVector::UsableForType(TypeFactory::GetType(kFloat, true)));
  EXPECT_TRUE(NativeColumnVector::UsableForType(TypeFactory::GetType(kDouble, false)));
  EXPECT_TRUE(NativeColumnVector::UsableForType(TypeFactory::GetType(kDouble, true)));
  EXPECT_TRUE(NativeColumnVector::UsableForType(TypeFactory::GetType(kChar, 20, false)));
  EXPECT_TRUE(NativeColumnVector::UsableForType(TypeFactory::GetType(kChar, 20, true)));
  EXPECT_FALSE(NativeColumnVector::UsableForType(TypeFactory::GetType(kVarChar, 20, false)));
  EXPECT_FALSE(NativeColumnVector::UsableForType(TypeFactory::GetType(kVarChar, 20, true)));
  EXPECT_TRUE(NativeColumnVector::UsableForType(TypeFactory::GetType(kNullType, true)));
}

TEST(ColumnVectorTest, NativeIntTest) {
  // Randomly shuffle a sequence of ints.
  std::vector<int> values;
  for (int i = -50; i < 50; ++i) {
    values.push_back(i);
  }
  std::random_shuffle(values.begin(), values.end());

  NativeColumnVector column_vector(TypeFactory::GetType(kInt, false), values.size());
  EXPECT_TRUE(column_vector.isNative());
  EXPECT_EQ(0u, column_vector.size());

  // Append values one-by-one, alternating between typed and untyped.
  for (std::size_t i = 0; i < values.size(); ++i) {
    if (i & 0x1) {
      column_vector.appendUntypedValue(&(values[i]));
    } else {
      column_vector.appendTypedValue(TypedValue(values[i]));
    }
    EXPECT_EQ(i + 1, column_vector.size());
  }

  // Check that values are as expected using both typed and untyped interface.
  for (std::size_t i = 0; i < values.size(); ++i) {
    EXPECT_EQ(values[i], *static_cast<const int*>(column_vector.getUntypedValue(i)));
    EXPECT_EQ(values[i], column_vector.getTypedValue(i).getLiteral<int>());
  }

  // Also try inserting in random order using positional writes.
  NativeColumnVector column_vector_positional(TypeFactory::GetType(kInt, false),
                                              values.size());
  column_vector_positional.prepareForPositionalWrites();
  EXPECT_EQ(values.size(), column_vector_positional.size());

  std::vector<std::size_t> insert_order;
  for (std::size_t i = 0; i < values.size(); ++i) {
    insert_order.push_back(i);
  }
  std::random_shuffle(insert_order.begin(), insert_order.end());
  for (const std::size_t pos : insert_order) {
    if (pos & 0x1) {
      column_vector_positional.positionalWriteUntypedValue(pos, &(values[pos]));
    } else {
      column_vector_positional.positionalWriteTypedValue(pos, TypedValue(values[pos]));
    }
  }

  EXPECT_EQ(values.size(), column_vector_positional.size());
  for (std::size_t i = 0; i < values.size(); ++i) {
    EXPECT_EQ(values[i], *static_cast<const int*>(column_vector_positional.getUntypedValue(i)));
    EXPECT_EQ(values[i], column_vector_positional.getTypedValue(i).getLiteral<int>());
  }

  // Reshuffle and overwrite values in-place in the same ColumnVector.
  std::random_shuffle(values.begin(), values.end());
  std::random_shuffle(insert_order.begin(), insert_order.end());
  for (const std::size_t pos : insert_order) {
    if (pos & 0x1) {
      column_vector_positional.positionalWriteUntypedValue(pos, &(values[pos]));
    } else {
      column_vector_positional.positionalWriteTypedValue(pos, TypedValue(values[pos]));
    }
  }

  EXPECT_EQ(values.size(), column_vector_positional.size());
  for (std::size_t i = 0; i < values.size(); ++i) {
    EXPECT_EQ(values[i], *static_cast<const int*>(column_vector_positional.getUntypedValue(i)));
    EXPECT_EQ(values[i], column_vector_positional.getTypedValue(i).getLiteral<int>());
  }
}

TEST(ColumnVectorTest, NativeNullableIntTest) {
  // Randomly shuffle a sequence of ints.
  std::vector<int> values;
  for (int i = -50; i < 50; ++i) {
    values.push_back(i);
  }
  std::random_shuffle(values.begin(), values.end());

  NativeColumnVector column_vector(TypeFactory::GetType(kInt, true), values.size());
  EXPECT_TRUE(column_vector.isNative());
  EXPECT_EQ(0u, column_vector.size());

  // Append values one-by-one, alternating between typed and untyped.
  for (std::size_t i = 0; i < values.size(); ++i) {
    // Use a NULL in place of every 5th value.
    if (i % 5 == 0) {
      if (i & 0x1) {
        column_vector.appendNullValue();
      } else {
        column_vector.appendTypedValue(TypedValue(kInt));
      }
    } else {
      if (i & 0x1) {
        column_vector.appendUntypedValue(&(values[i]));
      } else {
        column_vector.appendTypedValue(TypedValue(values[i]));
      }
    }
    EXPECT_EQ(i + 1, column_vector.size());
  }

  // Check that values are as expected using both typed and untyped interface.
  for (std::size_t i = 0; i < values.size(); ++i) {
    if (i % 5 == 0) {
      EXPECT_EQ(nullptr, column_vector.getUntypedValue(i));
      EXPECT_TRUE(column_vector.getTypedValue(i).isNull());
    } else {
      EXPECT_EQ(values[i], *static_cast<const int*>(column_vector.getUntypedValue(i)));
      EXPECT_EQ(values[i], column_vector.getTypedValue(i).getLiteral<int>());
    }
  }

  // Also try inserting in random order using positional writes.
  NativeColumnVector column_vector_positional(TypeFactory::GetType(kInt, true),
                                              values.size());
  column_vector_positional.prepareForPositionalWrites();
  EXPECT_EQ(values.size(), column_vector_positional.size());

  std::vector<std::size_t> insert_order;
  for (std::size_t i = 0; i < values.size(); ++i) {
    insert_order.push_back(i);
  }
  std::random_shuffle(insert_order.begin(), insert_order.end());
  for (const std::size_t pos : insert_order) {
    // Use a NULL in place of every 5th value.
    if (pos % 5 == 0) {
      if (pos & 0x1) {
        column_vector_positional.positionalWriteNullValue(pos);
      } else {
        column_vector_positional.positionalWriteTypedValue(pos, TypedValue(kInt));
      }
    } else {
      if (pos & 0x1) {
        column_vector_positional.positionalWriteUntypedValue(pos, &(values[pos]));
      } else {
        column_vector_positional.positionalWriteTypedValue(pos, TypedValue(values[pos]));
      }
    }
  }

  EXPECT_EQ(values.size(), column_vector_positional.size());
  for (std::size_t i = 0; i < values.size(); ++i) {
    if (i % 5 == 0) {
      EXPECT_EQ(nullptr, column_vector_positional.getUntypedValue(i));
      EXPECT_TRUE(column_vector_positional.getTypedValue(i).isNull());
    } else {
      EXPECT_EQ(values[i], *static_cast<const int*>(column_vector_positional.getUntypedValue(i)));
      EXPECT_EQ(values[i], column_vector_positional.getTypedValue(i).getLiteral<int>());
    }
  }

  // Reshuffle and overwrite values in-place in the same ColumnVector.
  std::random_shuffle(values.begin(), values.end());
  std::random_shuffle(insert_order.begin(), insert_order.end());
  for (const std::size_t pos : insert_order) {
    // Use a NULL in place of every 5th value.
    if (pos % 5 == 0) {
      if (pos & 0x1) {
        column_vector_positional.positionalWriteNullValue(pos);
      } else {
        column_vector_positional.positionalWriteTypedValue(pos, TypedValue(kInt));
      }
    } else {
      if (pos & 0x1) {
        column_vector_positional.positionalWriteUntypedValue(pos, &(values[pos]));
      } else {
        column_vector_positional.positionalWriteTypedValue(pos, TypedValue(values[pos]));
      }
    }
  }

  EXPECT_EQ(values.size(), column_vector_positional.size());
  for (std::size_t i = 0; i < values.size(); ++i) {
    if (i % 5 == 0) {
      EXPECT_EQ(nullptr, column_vector_positional.getUntypedValue(i));
      EXPECT_TRUE(column_vector_positional.getTypedValue(i).isNull());
    } else {
      EXPECT_EQ(values[i], *static_cast<const int*>(column_vector_positional.getUntypedValue(i)));
      EXPECT_EQ(values[i], column_vector_positional.getTypedValue(i).getLiteral<int>());
    }
  }
}

TEST(ColumnVectorTest, IndirectIntTest) {
  // Randomly shuffle a sequence of ints.
  std::vector<int> values;
  for (int i = -50; i < 50; ++i) {
    values.push_back(i);
  }
  std::random_shuffle(values.begin(), values.end());

  // Make an IndirectColumnVector for nullable ints.
  IndirectColumnVector column_vector(TypeFactory::GetType(kInt, true), values.size());
  EXPECT_FALSE(column_vector.isNative());
  EXPECT_EQ(0u, column_vector.size());

  for (std::size_t i = 0; i < values.size(); ++i) {
    TypedValue value(values[i]);
    // Use a NULL in place of every 5th value.
    if (i % 5 == 0) {
      value = TypedValue(kInt);
    }

    // Alternate between copy and move.
    if (i & 0x1) {
      column_vector.appendTypedValue(value);
    } else {
      column_vector.appendTypedValue(std::move(value));
    }
    EXPECT_EQ(i + 1, column_vector.size());
  }

  // Check that values are as expected using both typed and untyped interface.
  for (std::size_t i = 0; i < values.size(); ++i) {
    if (i % 5 == 0) {
      EXPECT_EQ(nullptr, column_vector.getUntypedValue(i));
      EXPECT_TRUE(column_vector.getTypedValue(i).isNull());
    } else {
      EXPECT_EQ(values[i], *static_cast<const int*>(column_vector.getUntypedValue(i)));
      EXPECT_EQ(values[i], column_vector.getTypedValue(i).getLiteral<int>());
    }
  }

  // Also try inserting in random order using positional writes.
  IndirectColumnVector column_vector_positional(TypeFactory::GetType(kInt, true),
                                              values.size());
  column_vector_positional.prepareForPositionalWrites();
  EXPECT_EQ(values.size(), column_vector_positional.size());

  std::vector<std::size_t> insert_order;
  for (std::size_t i = 0; i < values.size(); ++i) {
    insert_order.push_back(i);
  }
  std::random_shuffle(insert_order.begin(), insert_order.end());
  for (const std::size_t pos : insert_order) {
    TypedValue value(values[pos]);
    // Use a NULL in place of every 5th value.
    if (pos % 5 == 0) {
      value = TypedValue(kInt);
    }

    // Alternate between copy and move.
    if (pos & 0x1) {
      column_vector_positional.positionalWriteTypedValue(pos, value);
    } else {
      column_vector_positional.positionalWriteTypedValue(pos, std::move(value));
    }
  }

  EXPECT_EQ(values.size(), column_vector_positional.size());
  for (std::size_t i = 0; i < values.size(); ++i) {
    if (i % 5 == 0) {
      EXPECT_EQ(nullptr, column_vector_positional.getUntypedValue(i));
      EXPECT_TRUE(column_vector_positional.getTypedValue(i).isNull());
    } else {
      EXPECT_EQ(values[i], *static_cast<const int*>(column_vector_positional.getUntypedValue(i)));
      EXPECT_EQ(values[i], column_vector_positional.getTypedValue(i).getLiteral<int>());
    }
  }

  // Reshuffle and overwrite values in-place in the same ColumnVector.
  std::random_shuffle(values.begin(), values.end());
  std::random_shuffle(insert_order.begin(), insert_order.end());
  for (const std::size_t pos : insert_order) {
    TypedValue value(values[pos]);
    // Use a NULL in place of every 5th value.
    if (pos % 5 == 0) {
      value = TypedValue(kInt);
    }

    // Alternate between copy and move.
    if (pos & 0x1) {
      column_vector_positional.positionalWriteTypedValue(pos, value);
    } else {
      column_vector_positional.positionalWriteTypedValue(pos, std::move(value));
    }
  }

  EXPECT_EQ(values.size(), column_vector_positional.size());
  for (std::size_t i = 0; i < values.size(); ++i) {
    if (i % 5 == 0) {
      EXPECT_EQ(nullptr, column_vector_positional.getUntypedValue(i));
      EXPECT_TRUE(column_vector_positional.getTypedValue(i).isNull());
    } else {
      EXPECT_EQ(values[i], *static_cast<const int*>(column_vector_positional.getUntypedValue(i)));
      EXPECT_EQ(values[i], column_vector_positional.getTypedValue(i).getLiteral<int>());
    }
  }
}


const char *kSampleStrings[] = {
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

TEST(ColumnVectorTest, NativeCharTest) {
  // Make a contiguous array to store all strings.
  const std::size_t num_sample_strings = sizeof(kSampleStrings) / sizeof(kSampleStrings[0]);

  char *contiguous_strings = static_cast<char*>(std::malloc(20 * num_sample_strings));
  for (std::size_t i = 0; i < num_sample_strings; ++i) {
    std::strcpy(contiguous_strings + 20 * i, kSampleStrings[i]);  // NOLINT(runtime/printf)
  }

  NativeColumnVector column_vector(TypeFactory::GetType(kChar, 20, false), num_sample_strings);
  EXPECT_TRUE(column_vector.isNative());
  EXPECT_EQ(0u, column_vector.size());

  // Append values one-by-one, alternating between typed and untyped.
  for (std::size_t i = 0; i < num_sample_strings; ++i) {
    if (i & 0x1) {
      column_vector.appendUntypedValue(contiguous_strings + 20 * i);
    } else {
      if (i & 0x2) {
        column_vector.appendTypedValue(TypedValue(kChar, contiguous_strings + 20 * i, 20));
      } else {
        column_vector.appendTypedValue(TypedValue(kChar,
                                                  kSampleStrings[i],
                                                  std::strlen(kSampleStrings[i]) + 1));
      }
    }
    EXPECT_EQ(i + 1, column_vector.size());
  }

  // Check that values are as expected using both typed and untyped interface.
  for (std::size_t i = 0; i < num_sample_strings; ++i) {
    EXPECT_STREQ(kSampleStrings[i], static_cast<const char*>(column_vector.getUntypedValue(i)));
    EXPECT_EQ(std::strlen(kSampleStrings[i]),
              column_vector.getTypedValue(i).getAsciiStringLength());
    EXPECT_STREQ(kSampleStrings[i],
                 static_cast<const char*>(column_vector.getTypedValue(i).getDataPtr()));
  }

  // Also try inserting in random order using positional writes.
  NativeColumnVector column_vector_positional(TypeFactory::GetType(kChar, 20, false),
                                              num_sample_strings);
  column_vector_positional.prepareForPositionalWrites();
  EXPECT_EQ(num_sample_strings, column_vector_positional.size());

  std::vector<std::size_t> insert_order;
  for (std::size_t i = 0; i < num_sample_strings; ++i) {
    insert_order.push_back(i);
  }
  std::random_shuffle(insert_order.begin(), insert_order.end());
  for (const std::size_t pos : insert_order) {
    if (pos & 0x1) {
      column_vector_positional.positionalWriteUntypedValue(
          pos,
          contiguous_strings + 20 * pos);
    } else {
      if (pos & 0x2) {
        column_vector_positional.positionalWriteTypedValue(
            pos,
            TypedValue(kChar, contiguous_strings + 20 * pos, 20));
      } else {
        column_vector_positional.positionalWriteTypedValue(
            pos,
            TypedValue(kChar,
                       kSampleStrings[pos],
                       std::strlen(kSampleStrings[pos]) + 1));
      }
    }
  }

  EXPECT_EQ(num_sample_strings, column_vector_positional.size());
  for (std::size_t i = 0; i < num_sample_strings; ++i) {
    EXPECT_STREQ(kSampleStrings[i],
                 static_cast<const char*>(column_vector_positional.getUntypedValue(i)));
    EXPECT_EQ(std::strlen(kSampleStrings[i]),
              column_vector_positional.getTypedValue(i).getAsciiStringLength());
    EXPECT_STREQ(kSampleStrings[i],
                 static_cast<const char*>(column_vector_positional.getTypedValue(i).getDataPtr()));
  }

  std::free(contiguous_strings);
}

TEST(ColumnVectorTest, NativeNullableCharTest) {
  // Make a contiguous array to store all strings.
  const std::size_t num_sample_strings = sizeof(kSampleStrings) / sizeof(kSampleStrings[0]);

  char *contiguous_strings = static_cast<char*>(std::malloc(20 * num_sample_strings));
  for (std::size_t i = 0; i < num_sample_strings; ++i) {
    std::strcpy(contiguous_strings + 20 * i, kSampleStrings[i]);  // NOLINT(runtime/printf)
  }

  NativeColumnVector column_vector(TypeFactory::GetType(kChar, 20, true), num_sample_strings);
  EXPECT_TRUE(column_vector.isNative());
  EXPECT_EQ(0u, column_vector.size());

  // Append values one-by-one, alternating between typed and untyped.
  for (std::size_t i = 0; i < num_sample_strings; ++i) {
    // Use a NULL in place of every 5th value.
    if (i % 5 == 0) {
      if (i & 0x1) {
        column_vector.appendNullValue();
      } else {
        column_vector.appendTypedValue(TypedValue(kChar));
      }
    } else {
      if (i & 0x1) {
        column_vector.appendUntypedValue(contiguous_strings + 20 * i);
      } else {
        if (i & 0x2) {
          column_vector.appendTypedValue(TypedValue(kChar, contiguous_strings + 20 * i, 20));
        } else {
          column_vector.appendTypedValue(TypedValue(kChar,
                                                    kSampleStrings[i],
                                                    std::strlen(kSampleStrings[i]) + 1));
        }
      }
    }
    EXPECT_EQ(i + 1, column_vector.size());
  }

  // Check that values are as expected using both typed and untyped interface.
  for (std::size_t i = 0; i < num_sample_strings; ++i) {
    if (i % 5 == 0) {
      EXPECT_EQ(nullptr, column_vector.getUntypedValue(i));
      EXPECT_TRUE(column_vector.getTypedValue(i).isNull());
    } else {
      EXPECT_STREQ(kSampleStrings[i], static_cast<const char*>(column_vector.getUntypedValue(i)));
      EXPECT_EQ(std::strlen(kSampleStrings[i]),
                column_vector.getTypedValue(i).getAsciiStringLength());
      EXPECT_STREQ(kSampleStrings[i],
                   static_cast<const char*>(column_vector.getTypedValue(i).getDataPtr()));
    }
  }

  // Also try inserting in random order using positional writes.
  NativeColumnVector column_vector_positional(TypeFactory::GetType(kChar, 20, true),
                                              num_sample_strings);
  column_vector_positional.prepareForPositionalWrites();
  EXPECT_EQ(num_sample_strings, column_vector_positional.size());

  std::vector<std::size_t> insert_order;
  for (std::size_t i = 0; i < num_sample_strings; ++i) {
    insert_order.push_back(i);
  }
  std::random_shuffle(insert_order.begin(), insert_order.end());
  for (const std::size_t pos : insert_order) {
    // Use a NULL in place of every 5th value.
    if (pos % 5 == 0) {
      if (pos & 0x1) {
        column_vector_positional.positionalWriteNullValue(pos);
      } else {
        column_vector_positional.positionalWriteTypedValue(pos, TypedValue(kChar));
      }
    } else {
      if (pos & 0x1) {
        column_vector_positional.positionalWriteUntypedValue(pos, contiguous_strings + 20 * pos);
      } else {
        if (pos & 0x2) {
          column_vector_positional.positionalWriteTypedValue(
              pos,
              TypedValue(kChar, contiguous_strings + 20 * pos, 20));
        } else {
          column_vector_positional.positionalWriteTypedValue(
              pos,
              TypedValue(kChar,
                         kSampleStrings[pos],
                         std::strlen(kSampleStrings[pos]) + 1));
        }
      }
    }
  }

  EXPECT_EQ(num_sample_strings, column_vector_positional.size());
  for (std::size_t i = 0; i < num_sample_strings; ++i) {
    if (i % 5 == 0) {
      EXPECT_EQ(nullptr, column_vector_positional.getUntypedValue(i));
      EXPECT_TRUE(column_vector_positional.getTypedValue(i).isNull());
    } else {
      EXPECT_STREQ(kSampleStrings[i],
                   static_cast<const char*>(column_vector_positional.getUntypedValue(i)));
      EXPECT_EQ(std::strlen(kSampleStrings[i]),
                column_vector_positional.getTypedValue(i).getAsciiStringLength());
      EXPECT_STREQ(kSampleStrings[i],
                   static_cast<const char*>(column_vector_positional.getTypedValue(i).getDataPtr()));
    }
  }

  std::free(contiguous_strings);
}

TEST(ColumnVectorTest, IndirectCharTest) {
  const std::size_t num_sample_strings = sizeof(kSampleStrings) / sizeof(kSampleStrings[0]);

  // Make an IndirectColumnVector for nullable CHAR(20).
  IndirectColumnVector column_vector(TypeFactory::GetType(kChar, 20, true), num_sample_strings);
  EXPECT_FALSE(column_vector.isNative());
  EXPECT_EQ(0u, column_vector.size());

  for (std::size_t i = 0; i < num_sample_strings; ++i) {
    TypedValue value(kChar,
                     kSampleStrings[i],
                     std::strlen(kSampleStrings[i]) + 1);
    // Use a NULL in place of every 5th value.
    if (i % 5 == 0) {
      value = TypedValue(kChar);
    }
    // Make some values into literals.
    if (i & 0x2) {
      value.ensureNotReference();
    }

    // Alternate between copy and move.
    if (i & 0x1) {
      column_vector.appendTypedValue(value);
    } else {
      column_vector.appendTypedValue(std::move(value));
    }
    EXPECT_EQ(i + 1, column_vector.size());
  }

  // Check that values are as expected using both typed and untyped interface.
  for (std::size_t i = 0; i < num_sample_strings; ++i) {
    if (i % 5 == 0) {
      EXPECT_EQ(nullptr, column_vector.getUntypedValue(i));
      EXPECT_TRUE(column_vector.getTypedValue(i).isNull());
    } else {
      EXPECT_STREQ(kSampleStrings[i], static_cast<const char*>(column_vector.getUntypedValue(i)));
      EXPECT_EQ(std::strlen(kSampleStrings[i]),
                column_vector.getTypedValue(i).getAsciiStringLength());
      EXPECT_STREQ(kSampleStrings[i],
                   static_cast<const char*>(column_vector.getTypedValue(i).getDataPtr()));
    }
  }

  // Also try inserting in random order using positional writes.
  IndirectColumnVector column_vector_positional(TypeFactory::GetType(kChar, 20, true),
                                                num_sample_strings);
  column_vector_positional.prepareForPositionalWrites();
  EXPECT_EQ(num_sample_strings, column_vector_positional.size());

  std::vector<std::size_t> insert_order;
  for (std::size_t i = 0; i < num_sample_strings; ++i) {
    insert_order.push_back(i);
  }
  std::random_shuffle(insert_order.begin(), insert_order.end());
  for (const std::size_t pos : insert_order) {
    TypedValue value(kChar,
                     kSampleStrings[pos],
                     std::strlen(kSampleStrings[pos]) + 1);
    // Use a NULL in place of every 5th value.
    if (pos % 5 == 0) {
      value = TypedValue(kChar);
    }
    // Make some values into literals.
    if (pos & 0x2) {
      value.ensureNotReference();
    }

    // Alternate between copy and move.
    if (pos & 0x1) {
      column_vector_positional.positionalWriteTypedValue(pos, value);
    } else {
      column_vector_positional.positionalWriteTypedValue(pos, std::move(value));
    }
  }

  EXPECT_EQ(num_sample_strings, column_vector_positional.size());
  for (std::size_t i = 0; i < num_sample_strings; ++i) {
    if (i % 5 == 0) {
      EXPECT_EQ(nullptr, column_vector_positional.getUntypedValue(i));
      EXPECT_TRUE(column_vector_positional.getTypedValue(i).isNull());
    } else {
      EXPECT_STREQ(kSampleStrings[i],
                   static_cast<const char*>(column_vector_positional.getUntypedValue(i)));
      EXPECT_EQ(std::strlen(kSampleStrings[i]),
                column_vector_positional.getTypedValue(i).getAsciiStringLength());
      EXPECT_STREQ(kSampleStrings[i],
                   static_cast<const char*>(column_vector_positional.getTypedValue(i).getDataPtr()));
    }
  }
}

TEST(ColumnVectorTest, IndirectVarcharTest) {
  const std::size_t num_sample_strings = sizeof(kSampleStrings) / sizeof(kSampleStrings[0]);

  // Make an IndirectColumnVector for nullable VARCHAR(20).
  IndirectColumnVector column_vector(TypeFactory::GetType(kVarChar, 20, true), num_sample_strings);
  EXPECT_FALSE(column_vector.isNative());
  EXPECT_EQ(0u, column_vector.size());

  for (std::size_t i = 0; i < num_sample_strings; ++i) {
    TypedValue value(kVarChar,
                     kSampleStrings[i],
                     std::strlen(kSampleStrings[i]) + 1);
    // Use a NULL in place of every 5th value.
    if (i % 5 == 0) {
      value = TypedValue(kVarChar);
    }
    // Make some values into literals.
    if (i & 0x2) {
      value.ensureNotReference();
    }

    // Alternate between copy and move.
    if (i & 0x1) {
      column_vector.appendTypedValue(value);
    } else {
      column_vector.appendTypedValue(std::move(value));
    }
    EXPECT_EQ(i + 1, column_vector.size());
  }

  // Check that values are as expected using both typed and untyped interface.
  for (std::size_t i = 0; i < num_sample_strings; ++i) {
    if (i % 5 == 0) {
      EXPECT_EQ(nullptr, column_vector.getUntypedValue(i));
      EXPECT_TRUE(column_vector.getTypedValue(i).isNull());
    } else {
      EXPECT_STREQ(kSampleStrings[i], static_cast<const char*>(column_vector.getUntypedValue(i)));
      EXPECT_EQ(std::strlen(kSampleStrings[i]),
                column_vector.getTypedValue(i).getAsciiStringLength());
      EXPECT_STREQ(kSampleStrings[i],
                   static_cast<const char*>(column_vector.getTypedValue(i).getDataPtr()));
    }
  }

  // Also try inserting in random order using positional writes.
  IndirectColumnVector column_vector_positional(TypeFactory::GetType(kVarChar, 20, true),
                                                num_sample_strings);
  column_vector_positional.prepareForPositionalWrites();
  EXPECT_EQ(num_sample_strings, column_vector_positional.size());

  std::vector<std::size_t> insert_order;
  for (std::size_t i = 0; i < num_sample_strings; ++i) {
    insert_order.push_back(i);
  }
  std::random_shuffle(insert_order.begin(), insert_order.end());
  for (const std::size_t pos : insert_order) {
    TypedValue value(kVarChar,
                     kSampleStrings[pos],
                     std::strlen(kSampleStrings[pos]) + 1);
    // Use a NULL in place of every 5th value.
    if (pos % 5 == 0) {
      value = TypedValue(kVarChar);
    }
    // Make some values into literals.
    if (pos & 0x2) {
      value.ensureNotReference();
    }

    // Alternate between copy and move.
    if (pos & 0x1) {
      column_vector_positional.positionalWriteTypedValue(pos, value);
    } else {
      column_vector_positional.positionalWriteTypedValue(pos, std::move(value));
    }
  }

  EXPECT_EQ(num_sample_strings, column_vector_positional.size());
  for (std::size_t i = 0; i < num_sample_strings; ++i) {
    if (i % 5 == 0) {
      EXPECT_EQ(nullptr, column_vector_positional.getUntypedValue(i));
      EXPECT_TRUE(column_vector_positional.getTypedValue(i).isNull());
    } else {
      EXPECT_STREQ(kSampleStrings[i],
                   static_cast<const char*>(column_vector_positional.getUntypedValue(i)));
      EXPECT_EQ(std::strlen(kSampleStrings[i]),
                column_vector_positional.getTypedValue(i).getAsciiStringLength());
      EXPECT_STREQ(kSampleStrings[i],
                   static_cast<const char*>(column_vector_positional.getTypedValue(i).getDataPtr()));
    }
  }
}

TEST(ColumnVectorTest, NativeNullTypeTest) {
  const Type &null_type = TypeFactory::GetType(kNullType, true);
  NativeColumnVector column_vector(null_type, 100);

  for (std::size_t i = 0; i < 100; ++i) {
    if (i % 2) {
      column_vector.appendNullValue();
    } else {
      column_vector.appendTypedValue(null_type.makeNullValue());
    }
  }
  ASSERT_EQ(100u, column_vector.size());

  for (std::size_t i = 0; i < 100; ++i) {
    EXPECT_EQ(nullptr, column_vector.getUntypedValue(i));
    EXPECT_TRUE(column_vector.getTypedValue(i).isNull());
  }
}

TEST(ColumnVectorTest, IndirectNullTypeTest) {
  const Type &null_type = TypeFactory::GetType(kNullType, true);
  IndirectColumnVector column_vector(null_type, 100);

  for (std::size_t i = 0; i < 100; ++i) {
    column_vector.appendTypedValue(null_type.makeNullValue());
  }
  ASSERT_EQ(100u, column_vector.size());

  for (std::size_t i = 0; i < 100; ++i) {
    EXPECT_EQ(nullptr, column_vector.getUntypedValue(i));
    EXPECT_TRUE(column_vector.getTypedValue(i).isNull());
  }
}

}  // namespace quickstep
