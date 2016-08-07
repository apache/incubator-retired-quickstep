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

// TODO(chasseur): Trim down duplicated code in this file using more
// templating.

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "types/DatetimeLit.hpp"
#include "types/IntervalLit.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/TypedValue.pb.h"
#include "utility/Macros.hpp"
#include "utility/ScopedBuffer.hpp"

#include "gtest/gtest.h"

using std::numeric_limits;
using std::vector;

namespace quickstep {

namespace {

static const char kSampleStringShort[] = "foo";

static const char kSampleStringLong[] =
    "Space is big. You just won't believe how vastly, hugely, mind-bogglingly "
    "big it is. I mean, you may think it's a long way down the road to the "
    "chemist's, but that's just peanuts to space.";

}  // namespace

template <typename CppType>
void CheckNumericMetadata(const TypedValue &value, const TypeID expected_type_id) {
  EXPECT_FALSE(value.isNull());
  EXPECT_FALSE(value.isReference());
  EXPECT_FALSE(value.ownsOutOfLineData());
  EXPECT_EQ(expected_type_id, value.getTypeID());
  EXPECT_EQ(sizeof(CppType), value.getDataSize());

  EXPECT_TRUE(value.isPlausibleInstanceOf(TypeFactory::GetType(expected_type_id).getSignature()));
  for (int type_id_int = 0;
       type_id_int < static_cast<int>(kNumTypeIDs);
       ++type_id_int) {
    TypeID type_id = static_cast<TypeID>(type_id_int);
    if (type_id != expected_type_id) {
      if (TypeFactory::TypeRequiresLengthParameter(type_id)) {
        EXPECT_FALSE(value.isPlausibleInstanceOf(TypeFactory::GetType(type_id, 10, false).getSignature()));
      } else if (type_id == kNullType) {
        EXPECT_FALSE(value.isPlausibleInstanceOf(TypeFactory::GetType(type_id, true).getSignature()));
      } else {
        EXPECT_FALSE(value.isPlausibleInstanceOf(TypeFactory::GetType(type_id, false).getSignature()));
      }
    }
  }
}

void CheckNumericMetadataForNull(const TypedValue &value, const TypeID expected_type_id) {
  EXPECT_TRUE(value.isNull());
  EXPECT_FALSE(value.isReference());
  EXPECT_FALSE(value.ownsOutOfLineData());
  EXPECT_EQ(expected_type_id, value.getTypeID());

  EXPECT_TRUE(value.isPlausibleInstanceOf(TypeFactory::GetType(expected_type_id, true).getSignature()));
  EXPECT_FALSE(value.isPlausibleInstanceOf(TypeFactory::GetType(expected_type_id, false).getSignature()));
  for (int type_id_int = 0;
       type_id_int < static_cast<int>(kNumTypeIDs);
       ++type_id_int) {
    TypeID type_id = static_cast<TypeID>(type_id_int);
    if (type_id != expected_type_id) {
      if (TypeFactory::TypeRequiresLengthParameter(type_id)) {
        EXPECT_FALSE(value.isPlausibleInstanceOf(TypeFactory::GetType(type_id, 10, false).getSignature()));
        EXPECT_FALSE(value.isPlausibleInstanceOf(TypeFactory::GetType(type_id, 10, true).getSignature()));
      } else {
        if (type_id != kNullType) {
          EXPECT_FALSE(value.isPlausibleInstanceOf(TypeFactory::GetType(type_id, false).getSignature()));
        }
        EXPECT_FALSE(value.isPlausibleInstanceOf(TypeFactory::GetType(type_id, true).getSignature()));
      }
    }
  }
}

TEST(TypedValueTest, NumericMetadataTest) {
  TypedValue int_zero(0);
  CheckNumericMetadata<int>(int_zero, kInt);
  TypedValue int_max(numeric_limits<int>::max());
  CheckNumericMetadata<int>(int_max, kInt);
  TypedValue int_min(numeric_limits<int>::min());
  CheckNumericMetadata<int>(int_min, kInt);
  TypedValue int_null(kInt);
  CheckNumericMetadataForNull(int_null, kInt);

  TypedValue long_zero(static_cast<std::int64_t>(0));
  CheckNumericMetadata<std::int64_t>(long_zero, kLong);
  TypedValue long_max(numeric_limits<std::int64_t>::max());
  CheckNumericMetadata<std::int64_t>(long_max, kLong);
  TypedValue long_min(numeric_limits<std::int64_t>::min());
  CheckNumericMetadata<std::int64_t>(long_min, kLong);
  TypedValue long_null(kLong);
  CheckNumericMetadataForNull(long_null, kLong);

  TypedValue float_zero(static_cast<float>(0.0));
  CheckNumericMetadata<float>(float_zero, kFloat);
  TypedValue float_positive(static_cast<float>(123.45));
  CheckNumericMetadata<float>(float_positive, kFloat);
  TypedValue float_negative(static_cast<float>(-123.45));
  CheckNumericMetadata<float>(float_negative, kFloat);
  TypedValue float_max(numeric_limits<float>::max());
  CheckNumericMetadata<float>(float_max, kFloat);
  TypedValue float_min(numeric_limits<float>::min());
  CheckNumericMetadata<float>(float_min, kFloat);
  TypedValue float_null(kFloat);
  CheckNumericMetadataForNull(float_null, kFloat);

  TypedValue double_zero(static_cast<double>(0.0));
  CheckNumericMetadata<double>(double_zero, kDouble);
  TypedValue double_positive(static_cast<double>(123.45));
  CheckNumericMetadata<double>(double_positive, kDouble);
  TypedValue double_negative(static_cast<double>(-123.45));
  CheckNumericMetadata<double>(double_negative, kDouble);
  TypedValue double_max(numeric_limits<double>::max());
  CheckNumericMetadata<double>(double_max, kDouble);
  TypedValue double_min(numeric_limits<double>::min());
  CheckNumericMetadata<double>(double_min, kDouble);
  TypedValue double_null(kDouble);
  CheckNumericMetadataForNull(double_null, kDouble);
}

void CheckStringMetadata(const std::string &string_literal) {
  TypedValue char_ref(kChar,
                      string_literal.c_str(),
                      string_literal.size());
  EXPECT_TRUE(char_ref.isReference());
  EXPECT_FALSE(char_ref.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref.getTypeID());
  EXPECT_EQ(string_literal.size(), char_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref.getAsciiStringLength());

  TypedValue char_lit(kChar,
                      string_literal.c_str(),
                      string_literal.size());
  char_lit.ensureNotReference();
  EXPECT_FALSE(char_lit.isReference());
  EXPECT_TRUE(char_lit.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit.getTypeID());
  EXPECT_EQ(string_literal.size(), char_lit.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit.getAsciiStringLength());

  TypedValue char_ref_with_nullterm(kChar,
                                    string_literal.c_str(),
                                    string_literal.size() + 1);
  EXPECT_TRUE(char_ref_with_nullterm.isReference());
  EXPECT_FALSE(char_ref_with_nullterm.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref_with_nullterm.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, char_ref_with_nullterm.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref_with_nullterm.getAsciiStringLength());

  TypedValue char_lit_with_nullterm(kChar,
                                    string_literal.c_str(),
                                    string_literal.size() + 1);
  char_lit_with_nullterm.ensureNotReference();
  EXPECT_FALSE(char_lit_with_nullterm.isReference());
  EXPECT_TRUE(char_lit_with_nullterm.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit_with_nullterm.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, char_lit_with_nullterm.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit_with_nullterm.getAsciiStringLength());

  ScopedBuffer extended_char_buffer(string_literal.size() + 5);
  std::memset(extended_char_buffer.get(), 0, string_literal.size() + 5);
  std::memcpy(extended_char_buffer.get(),
              string_literal.c_str(),
              string_literal.size());
  TypedValue char_ref_extended(kChar,
                               extended_char_buffer.get(),
                               string_literal.size() + 5);
  EXPECT_TRUE(char_ref_extended.isReference());
  EXPECT_FALSE(char_ref_extended.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref_extended.getTypeID());
  EXPECT_EQ(string_literal.size() + 5, char_ref_extended.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref_extended.getAsciiStringLength());

  TypedValue char_lit_extended(kChar,
                               extended_char_buffer.get(),
                               string_literal.size() + 5);
  char_lit_extended.ensureNotReference();
  EXPECT_FALSE(char_lit_extended.isReference());
  EXPECT_TRUE(char_lit_extended.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit_extended.getTypeID());
  EXPECT_EQ(string_literal.size() + 5, char_lit_extended.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit_extended.getAsciiStringLength());

  TypedValue varchar_ref(kVarChar,
                         string_literal.c_str(),
                         string_literal.size() + 1);
  EXPECT_TRUE(varchar_ref.isReference());
  EXPECT_FALSE(varchar_ref.ownsOutOfLineData());
  EXPECT_EQ(kVarChar, varchar_ref.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, varchar_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), varchar_ref.getAsciiStringLength());

  TypedValue varchar_lit(kVarChar,
                         string_literal.c_str(),
                         string_literal.size() + 1);
  varchar_lit.ensureNotReference();
  EXPECT_FALSE(varchar_lit.isReference());
  EXPECT_TRUE(varchar_lit.ownsOutOfLineData());
  EXPECT_EQ(kVarChar, varchar_lit.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, varchar_lit.getDataSize());
  EXPECT_EQ(string_literal.size(), varchar_lit.getAsciiStringLength());
}

void CheckStringMetadataForNull(const TypedValue &value,
                                const TypeID expected_type_id) {
  EXPECT_TRUE(value.isNull());
  EXPECT_FALSE(value.isReference());
  EXPECT_FALSE(value.ownsOutOfLineData());
  EXPECT_EQ(expected_type_id, value.getTypeID());

  EXPECT_TRUE(value.isPlausibleInstanceOf(TypeFactory::GetType(expected_type_id, 1, true).getSignature()));
  EXPECT_TRUE(value.isPlausibleInstanceOf(TypeFactory::GetType(expected_type_id, 100, true).getSignature()));
  EXPECT_FALSE(value.isPlausibleInstanceOf(TypeFactory::GetType(expected_type_id, 1, false).getSignature()));
  EXPECT_FALSE(value.isPlausibleInstanceOf(TypeFactory::GetType(expected_type_id, 100, false).getSignature()));

  for (int type_id_int = 0;
       type_id_int < static_cast<int>(kNumTypeIDs);
       ++type_id_int) {
    TypeID type_id = static_cast<TypeID>(type_id_int);
    if (type_id != expected_type_id) {
      if (TypeFactory::TypeRequiresLengthParameter(type_id)) {
        EXPECT_FALSE(value.isPlausibleInstanceOf(TypeFactory::GetType(type_id, 10, false).getSignature()));
        EXPECT_FALSE(value.isPlausibleInstanceOf(TypeFactory::GetType(type_id, 10, true).getSignature()));
      } else {
        if (type_id != kNullType) {
          EXPECT_FALSE(value.isPlausibleInstanceOf(TypeFactory::GetType(type_id, false).getSignature()));
        }
        EXPECT_FALSE(value.isPlausibleInstanceOf(TypeFactory::GetType(type_id, true).getSignature()));
      }
    }
  }
}

TEST(TypedValueTest, StringMetadataTest) {
  CheckStringMetadata(kSampleStringShort);
  CheckStringMetadata(kSampleStringLong);

  TypedValue char_null(kChar);
  CheckStringMetadataForNull(char_null, kChar);

  TypedValue varchar_null(kVarChar);
  CheckStringMetadataForNull(varchar_null, kVarChar);
}

template <typename NumericType>
void CheckNumericData(const TypedValue &value, const NumericType expected) {
  EXPECT_EQ(expected, value.getLiteral<NumericType>());

  const NumericType *data_ptr = static_cast<const NumericType*>(value.getDataPtr());
  ASSERT_NE(data_ptr, nullptr);
  EXPECT_EQ(expected, *data_ptr);

  NumericType stack_var;
  value.copyInto(&stack_var);
  EXPECT_EQ(expected, stack_var);
}

template <typename NumericType>
void CheckNumericDataBasic(const NumericType literal) {
  TypedValue value(literal);
  CheckNumericData<NumericType>(value, literal);
}

TEST(TypedValueTest, NumericValueTest) {
  CheckNumericDataBasic<int>(0);
  CheckNumericDataBasic<int>(123);
  CheckNumericDataBasic<int>(-123);
  CheckNumericDataBasic<int>(numeric_limits<int>::max());
  CheckNumericDataBasic<int>(numeric_limits<int>::min());

  CheckNumericDataBasic<std::int64_t>(0);
  CheckNumericDataBasic<std::int64_t>(123);
  CheckNumericDataBasic<std::int64_t>(-123);
  CheckNumericDataBasic<std::int64_t>(numeric_limits<std::int64_t>::max());
  CheckNumericDataBasic<std::int64_t>(numeric_limits<std::int64_t>::min());

  CheckNumericDataBasic<float>(0.0);
  CheckNumericDataBasic<float>(123.45);
  CheckNumericDataBasic<float>(-123.45);
  CheckNumericDataBasic<float>(numeric_limits<float>::max());
  CheckNumericDataBasic<float>(numeric_limits<float>::min());

  CheckNumericDataBasic<double>(0.0);
  CheckNumericDataBasic<double>(123.45);
  CheckNumericDataBasic<double>(-123.45);
  CheckNumericDataBasic<double>(numeric_limits<double>::max());
  CheckNumericDataBasic<double>(numeric_limits<double>::min());
}

void CheckStringData(const TypedValue &value,
                     const std::string &expected,
                     const bool expect_null_terminator) {
  EXPECT_EQ(value.getDataPtr(), value.getOutOfLineData());

  if (expect_null_terminator) {
    EXPECT_STREQ(expected.c_str(), static_cast<const char*>(value.getDataPtr()));
  } else {
    EXPECT_EQ(0, std::strncmp(expected.c_str(),
                              static_cast<const char*>(value.getDataPtr()),
                              expected.size()));
  }

  ScopedBuffer str_buffer(value.getDataSize());
  value.copyInto(str_buffer.get());
  if (expect_null_terminator) {
    EXPECT_STREQ(expected.c_str(), static_cast<const char*>(str_buffer.get()));
  } else {
    EXPECT_EQ(0, std::strncmp(expected.c_str(),
                              static_cast<const char*>(str_buffer.get()),
                              expected.size()));
  }
}

void CheckStringDataBasic(const std::string &string_literal) {
  TypedValue char_ref(kChar,
                      string_literal.c_str(),
                      string_literal.size());
  CheckStringData(char_ref, string_literal, false);

  TypedValue char_lit(kChar,
                      string_literal.c_str(),
                      string_literal.size());
  char_lit.ensureNotReference();
  CheckStringData(char_lit, string_literal, false);

  TypedValue char_ref_with_nullterm(kChar,
                                    string_literal.c_str(),
                                    string_literal.size() + 1);
  CheckStringData(char_ref_with_nullterm, string_literal, true);

  TypedValue char_lit_with_nullterm(kChar,
                                    string_literal.c_str(),
                                    string_literal.size() + 1);
  char_lit_with_nullterm.ensureNotReference();
  CheckStringData(char_lit_with_nullterm, string_literal, true);

  ScopedBuffer extended_char_buffer(string_literal.size() + 5);
  std::memset(extended_char_buffer.get(), 0, string_literal.size() + 5);
  std::memcpy(extended_char_buffer.get(),
              string_literal.c_str(),
              string_literal.size());
  TypedValue char_ref_extended(kChar,
                               extended_char_buffer.get(),
                               string_literal.size() + 5);
  CheckStringData(char_ref_extended, string_literal, true);

  TypedValue char_lit_extended(kChar,
                               extended_char_buffer.get(),
                               string_literal.size() + 5);
  char_lit_extended.ensureNotReference();
  CheckStringData(char_lit_extended, string_literal, true);

  TypedValue varchar_ref(kVarChar,
                         string_literal.c_str(),
                         string_literal.size() + 1);
  CheckStringData(varchar_ref, string_literal, true);

  TypedValue varchar_lit(kVarChar,
                         string_literal.c_str(),
                         string_literal.size() + 1);
  varchar_lit.ensureNotReference();
  CheckStringData(varchar_lit, string_literal, true);
}

TEST(TypedValueTest, StringValueTest) {
  CheckStringDataBasic(kSampleStringShort);
  CheckStringDataBasic(kSampleStringLong);
}

template <typename NumericType>
void CheckNumericCopy(const NumericType literal) {
  TypedValue value(literal);
  TypedValue value_copy(value);

  CheckNumericMetadata<NumericType>(value_copy, value.getTypeID());
  CheckNumericData<NumericType>(value_copy, literal);
}

void CheckNumericNullCopy(const TypeID type_id) {
  TypedValue numeric_null(type_id);
  TypedValue numeric_null_copy(numeric_null);
  CheckNumericMetadataForNull(numeric_null_copy, type_id);
}

TEST(TypedValueTest, NumericCopyTest) {
  CheckNumericCopy<int>(0);
  CheckNumericCopy<int>(123);
  CheckNumericCopy<int>(-123);
  CheckNumericCopy<int>(numeric_limits<int>::max());
  CheckNumericCopy<int>(numeric_limits<int>::min());
  CheckNumericNullCopy(kInt);

  CheckNumericCopy<std::int64_t>(0);
  CheckNumericCopy<std::int64_t>(123);
  CheckNumericCopy<std::int64_t>(-123);
  CheckNumericCopy<std::int64_t>(numeric_limits<std::int64_t>::max());
  CheckNumericCopy<std::int64_t>(numeric_limits<std::int64_t>::min());
  CheckNumericNullCopy(kLong);

  CheckNumericCopy<float>(0.0);
  CheckNumericCopy<float>(123.45);
  CheckNumericCopy<float>(-123.45);
  CheckNumericCopy<float>(numeric_limits<float>::max());
  CheckNumericCopy<float>(numeric_limits<float>::min());
  CheckNumericNullCopy(kFloat);

  CheckNumericCopy<double>(0.0);
  CheckNumericCopy<double>(123.45);
  CheckNumericCopy<double>(-123.45);
  CheckNumericCopy<double>(numeric_limits<double>::max());
  CheckNumericCopy<double>(numeric_limits<double>::min());
  CheckNumericNullCopy(kDouble);
}

void CheckStringCopy(const std::string &string_literal) {
  TypedValue char_ref_orig(kChar,
                           string_literal.c_str(),
                           string_literal.size());
  TypedValue char_ref(char_ref_orig);
  // Check metadata, then data.
  EXPECT_TRUE(char_ref.isReference());
  EXPECT_FALSE(char_ref.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref.getTypeID());
  EXPECT_EQ(string_literal.size(), char_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref.getAsciiStringLength());
  CheckStringData(char_ref, string_literal, false);

  TypedValue char_lit_orig(kChar,
                           string_literal.c_str(),
                           string_literal.size());
  char_lit_orig.ensureNotReference();
  TypedValue char_lit(char_lit_orig);
  EXPECT_FALSE(char_lit.isReference());
  EXPECT_TRUE(char_lit.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit.getTypeID());
  EXPECT_EQ(string_literal.size(), char_lit.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit.getAsciiStringLength());
  CheckStringData(char_lit, string_literal, false);

  TypedValue char_ref_with_nullterm_orig(kChar,
                                         string_literal.c_str(),
                                         string_literal.size() + 1);
  TypedValue char_ref_with_nullterm(char_ref_with_nullterm_orig);
  EXPECT_TRUE(char_ref_with_nullterm.isReference());
  EXPECT_FALSE(char_ref_with_nullterm.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref_with_nullterm.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, char_ref_with_nullterm.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref_with_nullterm.getAsciiStringLength());
  CheckStringData(char_ref_with_nullterm, string_literal, true);

  TypedValue char_lit_with_nullterm_orig(kChar,
                                         string_literal.c_str(),
                                         string_literal.size() + 1);
  char_lit_with_nullterm_orig.ensureNotReference();
  TypedValue char_lit_with_nullterm(char_lit_with_nullterm_orig);
  EXPECT_FALSE(char_lit_with_nullterm.isReference());
  EXPECT_TRUE(char_lit_with_nullterm.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit_with_nullterm.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, char_lit_with_nullterm.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit_with_nullterm.getAsciiStringLength());
  CheckStringData(char_lit_with_nullterm, string_literal, true);

  ScopedBuffer extended_char_buffer(string_literal.size() + 5);
  std::memset(extended_char_buffer.get(), 0, string_literal.size() + 5);
  std::memcpy(extended_char_buffer.get(),
              string_literal.c_str(),
              string_literal.size());
  TypedValue char_ref_extended_orig(kChar,
                                    extended_char_buffer.get(),
                                    string_literal.size() + 5);
  TypedValue char_ref_extended(char_ref_extended_orig);
  EXPECT_TRUE(char_ref_extended.isReference());
  EXPECT_FALSE(char_ref_extended.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref_extended.getTypeID());
  EXPECT_EQ(string_literal.size() + 5, char_ref_extended.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref_extended.getAsciiStringLength());
  CheckStringData(char_ref_extended, string_literal, true);

  TypedValue char_lit_extended_orig(kChar,
                                    extended_char_buffer.get(),
                                    string_literal.size() + 5);
  char_lit_extended_orig.ensureNotReference();
  TypedValue char_lit_extended(char_lit_extended_orig);
  EXPECT_FALSE(char_lit_extended.isReference());
  EXPECT_TRUE(char_lit_extended.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit_extended.getTypeID());
  EXPECT_EQ(string_literal.size() + 5, char_lit_extended.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit_extended.getAsciiStringLength());
  CheckStringData(char_lit_extended, string_literal, true);

  TypedValue varchar_ref_orig(kVarChar,
                              string_literal.c_str(),
                              string_literal.size() + 1);
  TypedValue varchar_ref(varchar_ref_orig);
  EXPECT_TRUE(varchar_ref.isReference());
  EXPECT_FALSE(varchar_ref.ownsOutOfLineData());
  EXPECT_EQ(kVarChar, varchar_ref.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, varchar_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), varchar_ref.getAsciiStringLength());
  CheckStringData(varchar_ref, string_literal, true);

  TypedValue varchar_lit_orig(kVarChar,
                              string_literal.c_str(),
                              string_literal.size() + 1);
  varchar_lit_orig.ensureNotReference();
  TypedValue varchar_lit(varchar_lit_orig);
  EXPECT_FALSE(varchar_lit.isReference());
  EXPECT_TRUE(varchar_lit.ownsOutOfLineData());
  EXPECT_EQ(kVarChar, varchar_lit.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, varchar_lit.getDataSize());
  EXPECT_EQ(string_literal.size(), varchar_lit.getAsciiStringLength());
  CheckStringData(varchar_lit, string_literal, true);
}

void CheckStringNullCopy(const TypeID type_id) {
  TypedValue string_null(type_id);
  TypedValue string_null_copy(string_null);
  CheckStringMetadataForNull(string_null_copy, type_id);
}

TEST(TypedValueTest, StringCopyTest) {
  CheckStringCopy(kSampleStringShort);
  CheckStringCopy(kSampleStringLong);

  CheckStringNullCopy(kChar);
  CheckStringNullCopy(kVarChar);
}

template <typename NumericType>
void CheckNumericAssign(const NumericType literal) {
  TypedValue value_assigned;
  TypedValue value(literal);
  value_assigned = value;

  CheckNumericMetadata<NumericType>(value_assigned, value.getTypeID());
  CheckNumericData<NumericType>(value_assigned, literal);

  // Also check self-assignment.
  value_assigned = value_assigned;
  CheckNumericMetadata<NumericType>(value_assigned, value.getTypeID());
  CheckNumericData<NumericType>(value_assigned, literal);
}

void CheckNumericNullAssign(const TypeID type_id) {
  TypedValue value_assigned;
  TypedValue numeric_null(type_id);
  value_assigned = numeric_null;
  CheckNumericMetadataForNull(value_assigned, type_id);

  // Also check self-assignment.
  value_assigned = value_assigned;
  CheckNumericMetadataForNull(value_assigned, type_id);
}

TEST(TypedValueTest, NumericAssignTest) {
  CheckNumericAssign<int>(0);
  CheckNumericAssign<int>(123);
  CheckNumericAssign<int>(-123);
  CheckNumericAssign<int>(numeric_limits<int>::max());
  CheckNumericAssign<int>(numeric_limits<int>::min());
  CheckNumericNullAssign(kInt);

  CheckNumericAssign<std::int64_t>(0);
  CheckNumericAssign<std::int64_t>(123);
  CheckNumericAssign<std::int64_t>(-123);
  CheckNumericAssign<std::int64_t>(numeric_limits<std::int64_t>::max());
  CheckNumericAssign<std::int64_t>(numeric_limits<std::int64_t>::min());
  CheckNumericNullAssign(kLong);

  CheckNumericAssign<float>(0.0);
  CheckNumericAssign<float>(123.45);
  CheckNumericAssign<float>(-123.45);
  CheckNumericAssign<float>(numeric_limits<float>::max());
  CheckNumericAssign<float>(numeric_limits<float>::min());
  CheckNumericNullAssign(kFloat);

  CheckNumericAssign<double>(0.0);
  CheckNumericAssign<double>(123.45);
  CheckNumericAssign<double>(-123.45);
  CheckNumericAssign<double>(numeric_limits<double>::max());
  CheckNumericAssign<double>(numeric_limits<double>::min());
  CheckNumericNullAssign(kDouble);
}

void CheckStringAssign(const std::string &string_literal) {
  TypedValue char_ref_orig(kChar,
                           string_literal.c_str(),
                           string_literal.size());
  TypedValue char_ref = char_ref_orig;
  // Check metadata, then data.
  EXPECT_TRUE(char_ref.isReference());
  EXPECT_FALSE(char_ref.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref.getTypeID());
  EXPECT_EQ(string_literal.size(), char_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref.getAsciiStringLength());
  CheckStringData(char_ref, string_literal, false);
  // Self-assign and check again.
  char_ref = char_ref;
  EXPECT_TRUE(char_ref.isReference());
  EXPECT_FALSE(char_ref.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref.getTypeID());
  EXPECT_EQ(string_literal.size(), char_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref.getAsciiStringLength());
  CheckStringData(char_ref, string_literal, false);

  TypedValue char_lit_orig(kChar,
                           string_literal.c_str(),
                           string_literal.size());
  char_lit_orig.ensureNotReference();
  TypedValue char_lit = char_lit_orig;
  EXPECT_FALSE(char_lit.isReference());
  EXPECT_TRUE(char_lit.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit.getTypeID());
  EXPECT_EQ(string_literal.size(), char_lit.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit.getAsciiStringLength());
  CheckStringData(char_lit, string_literal, false);
  // Self-assign.
  char_lit = char_lit;
  EXPECT_FALSE(char_lit.isReference());
  EXPECT_TRUE(char_lit.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit.getTypeID());
  EXPECT_EQ(string_literal.size(), char_lit.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit.getAsciiStringLength());
  CheckStringData(char_lit, string_literal, false);

  TypedValue char_ref_with_nullterm_orig(kChar,
                                         string_literal.c_str(),
                                         string_literal.size() + 1);
  TypedValue char_ref_with_nullterm = char_ref_with_nullterm_orig;
  EXPECT_TRUE(char_ref_with_nullterm.isReference());
  EXPECT_FALSE(char_ref_with_nullterm.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref_with_nullterm.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, char_ref_with_nullterm.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref_with_nullterm.getAsciiStringLength());
  CheckStringData(char_ref_with_nullterm, string_literal, true);
  // Self-assign.
  char_ref_with_nullterm = char_ref_with_nullterm;
  EXPECT_TRUE(char_ref_with_nullterm.isReference());
  EXPECT_FALSE(char_ref_with_nullterm.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref_with_nullterm.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, char_ref_with_nullterm.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref_with_nullterm.getAsciiStringLength());
  CheckStringData(char_ref_with_nullterm, string_literal, true);

  TypedValue char_lit_with_nullterm_orig(kChar,
                                         string_literal.c_str(),
                                         string_literal.size() + 1);
  char_lit_with_nullterm_orig.ensureNotReference();
  TypedValue char_lit_with_nullterm = char_lit_with_nullterm_orig;
  EXPECT_FALSE(char_lit_with_nullterm.isReference());
  EXPECT_TRUE(char_lit_with_nullterm.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit_with_nullterm.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, char_lit_with_nullterm.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit_with_nullterm.getAsciiStringLength());
  CheckStringData(char_lit_with_nullterm, string_literal, true);
  // Self-assign.
  char_lit_with_nullterm = char_lit_with_nullterm;
  EXPECT_FALSE(char_lit_with_nullterm.isReference());
  EXPECT_TRUE(char_lit_with_nullterm.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit_with_nullterm.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, char_lit_with_nullterm.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit_with_nullterm.getAsciiStringLength());
  CheckStringData(char_lit_with_nullterm, string_literal, true);

  ScopedBuffer extended_char_buffer(string_literal.size() + 5);
  std::memset(extended_char_buffer.get(), 0, string_literal.size() + 5);
  std::memcpy(extended_char_buffer.get(),
              string_literal.c_str(),
              string_literal.size());
  TypedValue char_ref_extended_orig(kChar,
                                    extended_char_buffer.get(),
                                    string_literal.size() + 5);
  TypedValue char_ref_extended = char_ref_extended_orig;
  EXPECT_TRUE(char_ref_extended.isReference());
  EXPECT_FALSE(char_ref_extended.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref_extended.getTypeID());
  EXPECT_EQ(string_literal.size() + 5, char_ref_extended.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref_extended.getAsciiStringLength());
  CheckStringData(char_ref_extended, string_literal, true);
  // Self-assign.
  char_ref_extended = char_ref_extended;
  EXPECT_TRUE(char_ref_extended.isReference());
  EXPECT_FALSE(char_ref_extended.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref_extended.getTypeID());
  EXPECT_EQ(string_literal.size() + 5, char_ref_extended.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref_extended.getAsciiStringLength());
  CheckStringData(char_ref_extended, string_literal, true);

  TypedValue char_lit_extended_orig(kChar,
                                    extended_char_buffer.get(),
                                    string_literal.size() + 5);
  char_lit_extended_orig.ensureNotReference();
  TypedValue char_lit_extended = char_lit_extended_orig;
  EXPECT_FALSE(char_lit_extended.isReference());
  EXPECT_TRUE(char_lit_extended.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit_extended.getTypeID());
  EXPECT_EQ(string_literal.size() + 5, char_lit_extended.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit_extended.getAsciiStringLength());
  CheckStringData(char_lit_extended, string_literal, true);
  // Self-assign.
  char_lit_extended = char_lit_extended;
  EXPECT_FALSE(char_lit_extended.isReference());
  EXPECT_TRUE(char_lit_extended.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit_extended.getTypeID());
  EXPECT_EQ(string_literal.size() + 5, char_lit_extended.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit_extended.getAsciiStringLength());
  CheckStringData(char_lit_extended, string_literal, true);

  TypedValue varchar_ref_orig(kVarChar,
                              string_literal.c_str(),
                              string_literal.size() + 1);
  TypedValue varchar_ref = varchar_ref_orig;
  EXPECT_TRUE(varchar_ref.isReference());
  EXPECT_FALSE(varchar_ref.ownsOutOfLineData());
  EXPECT_EQ(kVarChar, varchar_ref.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, varchar_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), varchar_ref.getAsciiStringLength());
  CheckStringData(varchar_ref, string_literal, true);
  // Self-assign.
  varchar_ref = varchar_ref;
  EXPECT_TRUE(varchar_ref.isReference());
  EXPECT_FALSE(varchar_ref.ownsOutOfLineData());
  EXPECT_EQ(kVarChar, varchar_ref.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, varchar_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), varchar_ref.getAsciiStringLength());
  CheckStringData(varchar_ref, string_literal, true);

  TypedValue varchar_lit_orig(kVarChar,
                              string_literal.c_str(),
                              string_literal.size() + 1);
  varchar_lit_orig.ensureNotReference();
  TypedValue varchar_lit = varchar_lit_orig;
  EXPECT_FALSE(varchar_lit.isReference());
  EXPECT_TRUE(varchar_lit.ownsOutOfLineData());
  EXPECT_EQ(kVarChar, varchar_lit.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, varchar_lit.getDataSize());
  EXPECT_EQ(string_literal.size(), varchar_lit.getAsciiStringLength());
  CheckStringData(varchar_lit, string_literal, true);
  // Self-assign.
  varchar_lit = varchar_lit;
  EXPECT_FALSE(varchar_lit.isReference());
  EXPECT_TRUE(varchar_lit.ownsOutOfLineData());
  EXPECT_EQ(kVarChar, varchar_lit.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, varchar_lit.getDataSize());
  EXPECT_EQ(string_literal.size(), varchar_lit.getAsciiStringLength());
  CheckStringData(varchar_lit, string_literal, true);
}

void CheckStringNullAssign(const TypeID type_id) {
  TypedValue value_assigned;
  TypedValue string_null(type_id);
  value_assigned = string_null;
  CheckStringMetadataForNull(value_assigned, type_id);

  // Also check self-assignment.
  value_assigned = value_assigned;
  CheckStringMetadataForNull(value_assigned, type_id);
}

TEST(TypedValueTest, StringAssignTest) {
  CheckStringAssign(kSampleStringShort);
  CheckStringAssign(kSampleStringLong);

  CheckStringNullAssign(kChar);
  CheckStringNullAssign(kVarChar);
}

template <typename NumericType>
void CheckNumericMakeReference(const NumericType literal) {
  TypedValue value(literal);
  // NOTE: For numeric types, this will actually just make a copy, not a
  // real reference.
  TypedValue value_reference = value.makeReferenceToThis();

  CheckNumericMetadata<NumericType>(value_reference, value.getTypeID());
  CheckNumericData<NumericType>(value_reference, literal);
}

void CheckNumericMakeReferenceToNull(const TypeID type_id) {
  TypedValue null_value(type_id);
  // NOTE: Making a reference to a null actually just makes a copy, not a real
  // reference.
  TypedValue value_reference = null_value.makeReferenceToThis();

  CheckNumericMetadataForNull(value_reference, type_id);
}

TEST(TypedValueTest, NumericMakeReferenceTest) {
  CheckNumericMakeReference<int>(0);
  CheckNumericMakeReference<int>(123);
  CheckNumericMakeReference<int>(-123);
  CheckNumericMakeReference<int>(numeric_limits<int>::max());
  CheckNumericMakeReference<int>(numeric_limits<int>::min());
  CheckNumericMakeReferenceToNull(kInt);

  CheckNumericMakeReference<std::int64_t>(0);
  CheckNumericMakeReference<std::int64_t>(123);
  CheckNumericMakeReference<std::int64_t>(-123);
  CheckNumericMakeReference<std::int64_t>(numeric_limits<std::int64_t>::max());
  CheckNumericMakeReference<std::int64_t>(numeric_limits<std::int64_t>::min());
  CheckNumericMakeReferenceToNull(kLong);

  CheckNumericMakeReference<float>(0.0);
  CheckNumericMakeReference<float>(123.45);
  CheckNumericMakeReference<float>(-123.45);
  CheckNumericMakeReference<float>(numeric_limits<float>::max());
  CheckNumericMakeReference<float>(numeric_limits<float>::min());
  CheckNumericMakeReferenceToNull(kFloat);

  CheckNumericMakeReference<double>(0.0);
  CheckNumericMakeReference<double>(123.45);
  CheckNumericMakeReference<double>(-123.45);
  CheckNumericMakeReference<double>(numeric_limits<double>::max());
  CheckNumericMakeReference<double>(numeric_limits<double>::min());
  CheckNumericMakeReferenceToNull(kDouble);
}

void CheckStringMakeReference(const std::string &string_literal) {
  TypedValue char_ref(kChar,
                      string_literal.c_str(),
                      string_literal.size());
  TypedValue char_ref_ref = char_ref.makeReferenceToThis();
  EXPECT_EQ(string_literal.c_str(), char_ref_ref.getDataPtr());
  // Check metadata, then data.
  EXPECT_TRUE(char_ref_ref.isReference());
  EXPECT_FALSE(char_ref_ref.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref_ref.getTypeID());
  EXPECT_EQ(string_literal.size(), char_ref_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref_ref.getAsciiStringLength());
  CheckStringData(char_ref_ref, string_literal, false);

  TypedValue char_lit(kChar,
                      string_literal.c_str(),
                      string_literal.size());
  char_lit.ensureNotReference();
  EXPECT_NE(string_literal.c_str(), char_lit.getDataPtr());
  TypedValue char_lit_ref = char_lit.makeReferenceToThis();
  EXPECT_EQ(char_lit.getDataPtr(), char_lit_ref.getDataPtr());
  EXPECT_TRUE(char_lit_ref.isReference());
  EXPECT_FALSE(char_lit_ref.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit_ref.getTypeID());
  EXPECT_EQ(string_literal.size(), char_lit_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit_ref.getAsciiStringLength());
  CheckStringData(char_lit_ref, string_literal, false);

  TypedValue char_ref_with_nullterm(kChar,
                                    string_literal.c_str(),
                                    string_literal.size() + 1);
  TypedValue char_ref_with_nullterm_ref = char_ref_with_nullterm.makeReferenceToThis();
  EXPECT_EQ(string_literal.c_str(), char_ref_with_nullterm_ref.getDataPtr());
  EXPECT_TRUE(char_ref_with_nullterm_ref.isReference());
  EXPECT_FALSE(char_ref_with_nullterm_ref.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref_with_nullterm_ref.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, char_ref_with_nullterm_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref_with_nullterm_ref.getAsciiStringLength());
  CheckStringData(char_ref_with_nullterm_ref, string_literal, true);

  TypedValue char_lit_with_nullterm(kChar,
                                    string_literal.c_str(),
                                    string_literal.size() + 1);
  char_lit_with_nullterm.ensureNotReference();
  EXPECT_NE(string_literal.c_str(), char_lit_with_nullterm.getDataPtr());
  TypedValue char_lit_with_nullterm_ref = char_lit_with_nullterm.makeReferenceToThis();
  EXPECT_EQ(char_lit_with_nullterm.getDataPtr(), char_lit_with_nullterm_ref.getDataPtr());
  EXPECT_TRUE(char_lit_with_nullterm_ref.isReference());
  EXPECT_FALSE(char_lit_with_nullterm_ref.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit_with_nullterm_ref.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, char_lit_with_nullterm_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit_with_nullterm_ref.getAsciiStringLength());
  CheckStringData(char_lit_with_nullterm_ref, string_literal, true);

  ScopedBuffer extended_char_buffer(string_literal.size() + 5);
  std::memset(extended_char_buffer.get(), 0, string_literal.size() + 5);
  std::memcpy(extended_char_buffer.get(),
              string_literal.c_str(),
              string_literal.size());
  TypedValue char_ref_extended(kChar,
                               extended_char_buffer.get(),
                               string_literal.size() + 5);
  TypedValue char_ref_extended_ref = char_ref_extended.makeReferenceToThis();
  EXPECT_EQ(extended_char_buffer.get(), char_ref_extended_ref.getDataPtr());
  EXPECT_TRUE(char_ref_extended_ref.isReference());
  EXPECT_FALSE(char_ref_extended_ref.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref_extended_ref.getTypeID());
  EXPECT_EQ(string_literal.size() + 5, char_ref_extended_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref_extended_ref.getAsciiStringLength());
  CheckStringData(char_ref_extended_ref, string_literal, true);

  TypedValue char_lit_extended(kChar,
                               extended_char_buffer.get(),
                               string_literal.size() + 5);
  char_lit_extended.ensureNotReference();
  EXPECT_NE(string_literal.c_str(), char_lit_extended.getDataPtr());
  TypedValue char_lit_extended_ref = char_lit_extended.makeReferenceToThis();
  EXPECT_EQ(char_lit_extended.getDataPtr(), char_lit_extended_ref.getDataPtr());
  EXPECT_TRUE(char_lit_extended_ref.isReference());
  EXPECT_FALSE(char_lit_extended_ref.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit_extended_ref.getTypeID());
  EXPECT_EQ(string_literal.size() + 5, char_lit_extended_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit_extended_ref.getAsciiStringLength());
  CheckStringData(char_lit_extended_ref, string_literal, true);

  TypedValue varchar_ref(kVarChar,
                         string_literal.c_str(),
                         string_literal.size() + 1);
  TypedValue varchar_ref_ref = varchar_ref.makeReferenceToThis();
  EXPECT_EQ(string_literal.c_str(), varchar_ref_ref.getDataPtr());
  EXPECT_TRUE(varchar_ref_ref.isReference());
  EXPECT_FALSE(varchar_ref_ref.ownsOutOfLineData());
  EXPECT_EQ(kVarChar, varchar_ref_ref.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, varchar_ref_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), varchar_ref_ref.getAsciiStringLength());
  CheckStringData(varchar_ref_ref, string_literal, true);

  TypedValue varchar_lit(kVarChar,
                         string_literal.c_str(),
                         string_literal.size() + 1);
  varchar_lit.ensureNotReference();
  EXPECT_NE(string_literal.c_str(), varchar_lit.getDataPtr());
  TypedValue varchar_lit_ref = varchar_lit.makeReferenceToThis();
  EXPECT_EQ(varchar_lit.getDataPtr(), varchar_lit_ref.getDataPtr());
  EXPECT_TRUE(varchar_lit_ref.isReference());
  EXPECT_FALSE(varchar_lit_ref.ownsOutOfLineData());
  EXPECT_EQ(kVarChar, varchar_lit_ref.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, varchar_lit_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), varchar_lit_ref.getAsciiStringLength());
  CheckStringData(varchar_lit_ref, string_literal, true);
}

void CheckStringMakeReferenceToNull(const TypeID type_id) {
  TypedValue null_value(type_id);
  // NOTE: Making a reference to a null actually just makes a copy, not a real
  // reference.
  TypedValue value_reference = null_value.makeReferenceToThis();

  CheckStringMetadataForNull(value_reference, type_id);
}

TEST(TypedValueTest, StringMakeReferenceTest) {
  CheckStringMakeReference(kSampleStringShort);
  CheckStringMakeReference(kSampleStringLong);

  CheckStringMakeReferenceToNull(kChar);
  CheckStringMakeReferenceToNull(kVarChar);
}

template <typename NumericType>
void CheckNumericMove(const NumericType literal) {
  TypedValue value(literal);
  TypeID expected_type_id = value.getTypeID();
  TypedValue value_moved(std::move(value));

  // Check that the original has been cleared out.
  EXPECT_EQ(kInt, value.getTypeID());
  EXPECT_FALSE(value.isNull());
  EXPECT_FALSE(value.ownsOutOfLineData());

  CheckNumericMetadata<NumericType>(value_moved, expected_type_id);
  CheckNumericData<NumericType>(value_moved, literal);
}

void CheckNumericNullMove(const TypeID type_id) {
  TypedValue numeric_null(type_id);
  TypedValue numeric_null_moved(std::move(numeric_null));

  // Check that the original has been cleared out.
  EXPECT_EQ(kInt, numeric_null.getTypeID());
  EXPECT_FALSE(numeric_null.isNull());
  EXPECT_FALSE(numeric_null.ownsOutOfLineData());

  CheckNumericMetadataForNull(numeric_null_moved, type_id);
}

TEST(TypedValueTest, NumericMoveTest) {
  CheckNumericMove<int>(0);
  CheckNumericMove<int>(123);
  CheckNumericMove<int>(-123);
  CheckNumericMove<int>(numeric_limits<int>::max());
  CheckNumericMove<int>(numeric_limits<int>::min());
  CheckNumericNullMove(kInt);

  CheckNumericMove<std::int64_t>(0);
  CheckNumericMove<std::int64_t>(123);
  CheckNumericMove<std::int64_t>(-123);
  CheckNumericMove<std::int64_t>(numeric_limits<std::int64_t>::max());
  CheckNumericMove<std::int64_t>(numeric_limits<std::int64_t>::min());
  CheckNumericNullMove(kLong);

  CheckNumericMove<float>(0.0);
  CheckNumericMove<float>(123.45);
  CheckNumericMove<float>(-123.45);
  CheckNumericMove<float>(numeric_limits<float>::max());
  CheckNumericMove<float>(numeric_limits<float>::min());
  CheckNumericNullMove(kFloat);

  CheckNumericMove<double>(0.0);
  CheckNumericMove<double>(123.45);
  CheckNumericMove<double>(-123.45);
  CheckNumericMove<double>(numeric_limits<double>::max());
  CheckNumericMove<double>(numeric_limits<double>::min());
  CheckNumericNullMove(kDouble);
}

void CheckStringMove(const std::string &string_literal) {
  TypedValue char_ref_orig(kChar,
                           string_literal.c_str(),
                           string_literal.size());
  TypedValue char_ref(std::move(char_ref_orig));
  // Check that original has been cleared out.
  EXPECT_EQ(kInt, char_ref_orig.getTypeID());
  EXPECT_FALSE(char_ref_orig.isNull());
  EXPECT_FALSE(char_ref_orig.ownsOutOfLineData());
  // Check metadata, then data.
  EXPECT_TRUE(char_ref.isReference());
  EXPECT_FALSE(char_ref.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref.getTypeID());
  EXPECT_EQ(string_literal.size(), char_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref.getAsciiStringLength());
  CheckStringData(char_ref, string_literal, false);

  TypedValue char_lit_orig(kChar,
                           string_literal.c_str(),
                           string_literal.size());
  char_lit_orig.ensureNotReference();
  TypedValue char_lit(std::move(char_lit_orig));
  EXPECT_EQ(kInt, char_lit_orig.getTypeID());
  EXPECT_FALSE(char_lit_orig.isNull());
  EXPECT_FALSE(char_lit_orig.ownsOutOfLineData());
  EXPECT_FALSE(char_lit.isReference());
  EXPECT_TRUE(char_lit.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit.getTypeID());
  EXPECT_EQ(string_literal.size(), char_lit.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit.getAsciiStringLength());
  CheckStringData(char_lit, string_literal, false);

  TypedValue char_ref_with_nullterm_orig(kChar,
                                         string_literal.c_str(),
                                         string_literal.size() + 1);
  TypedValue char_ref_with_nullterm(std::move(char_ref_with_nullterm_orig));
  EXPECT_EQ(kInt, char_ref_with_nullterm_orig.getTypeID());
  EXPECT_FALSE(char_ref_with_nullterm_orig.isNull());
  EXPECT_FALSE(char_ref_with_nullterm_orig.ownsOutOfLineData());
  EXPECT_TRUE(char_ref_with_nullterm.isReference());
  EXPECT_FALSE(char_ref_with_nullterm.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref_with_nullterm.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, char_ref_with_nullterm.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref_with_nullterm.getAsciiStringLength());
  CheckStringData(char_ref_with_nullterm, string_literal, true);

  TypedValue char_lit_with_nullterm_orig(kChar,
                                         string_literal.c_str(),
                                         string_literal.size() + 1);
  char_lit_with_nullterm_orig.ensureNotReference();
  TypedValue char_lit_with_nullterm(std::move(char_lit_with_nullterm_orig));
  EXPECT_EQ(kInt, char_lit_with_nullterm_orig.getTypeID());
  EXPECT_FALSE(char_lit_with_nullterm_orig.isNull());
  EXPECT_FALSE(char_lit_with_nullterm_orig.ownsOutOfLineData());
  EXPECT_FALSE(char_lit_with_nullterm.isReference());
  EXPECT_TRUE(char_lit_with_nullterm.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit_with_nullterm.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, char_lit_with_nullterm.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit_with_nullterm.getAsciiStringLength());
  CheckStringData(char_lit_with_nullterm, string_literal, true);

  ScopedBuffer extended_char_buffer(string_literal.size() + 5);
  std::memset(extended_char_buffer.get(), 0, string_literal.size() + 5);
  std::memcpy(extended_char_buffer.get(),
              string_literal.c_str(),
              string_literal.size());
  TypedValue char_ref_extended_orig(kChar,
                                    extended_char_buffer.get(),
                                    string_literal.size() + 5);
  TypedValue char_ref_extended(std::move(char_ref_extended_orig));
  EXPECT_EQ(kInt, char_ref_extended_orig.getTypeID());
  EXPECT_FALSE(char_ref_extended_orig.isNull());
  EXPECT_FALSE(char_ref_extended_orig.ownsOutOfLineData());
  EXPECT_TRUE(char_ref_extended.isReference());
  EXPECT_FALSE(char_ref_extended.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref_extended.getTypeID());
  EXPECT_EQ(string_literal.size() + 5, char_ref_extended.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref_extended.getAsciiStringLength());
  CheckStringData(char_ref_extended, string_literal, true);

  TypedValue char_lit_extended_orig(kChar,
                                    extended_char_buffer.get(),
                                    string_literal.size() + 5);
  char_lit_extended_orig.ensureNotReference();
  TypedValue char_lit_extended(std::move(char_lit_extended_orig));
  EXPECT_EQ(kInt, char_lit_extended_orig.getTypeID());
  EXPECT_FALSE(char_lit_extended_orig.isNull());
  EXPECT_FALSE(char_lit_extended_orig.ownsOutOfLineData());
  EXPECT_FALSE(char_lit_extended.isReference());
  EXPECT_TRUE(char_lit_extended.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit_extended.getTypeID());
  EXPECT_EQ(string_literal.size() + 5, char_lit_extended.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit_extended.getAsciiStringLength());
  CheckStringData(char_lit_extended, string_literal, true);

  TypedValue varchar_ref_orig(kVarChar,
                              string_literal.c_str(),
                              string_literal.size() + 1);
  TypedValue varchar_ref(std::move(varchar_ref_orig));
  EXPECT_EQ(kInt, varchar_ref_orig.getTypeID());
  EXPECT_FALSE(varchar_ref_orig.isNull());
  EXPECT_FALSE(varchar_ref_orig.ownsOutOfLineData());
  EXPECT_TRUE(varchar_ref.isReference());
  EXPECT_FALSE(varchar_ref.ownsOutOfLineData());
  EXPECT_EQ(kVarChar, varchar_ref.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, varchar_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), varchar_ref.getAsciiStringLength());
  CheckStringData(varchar_ref, string_literal, true);

  TypedValue varchar_lit_orig(kVarChar,
                              string_literal.c_str(),
                              string_literal.size() + 1);
  varchar_lit_orig.ensureNotReference();
  TypedValue varchar_lit(std::move(varchar_lit_orig));
  EXPECT_EQ(kInt, varchar_lit_orig.getTypeID());
  EXPECT_FALSE(varchar_lit_orig.isNull());
  EXPECT_FALSE(varchar_lit_orig.ownsOutOfLineData());
  EXPECT_FALSE(varchar_lit.isReference());
  EXPECT_TRUE(varchar_lit.ownsOutOfLineData());
  EXPECT_EQ(kVarChar, varchar_lit.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, varchar_lit.getDataSize());
  EXPECT_EQ(string_literal.size(), varchar_lit.getAsciiStringLength());
  CheckStringData(varchar_lit, string_literal, true);
}

void CheckStringNullMove(const TypeID type_id) {
  TypedValue string_null(type_id);
  TypedValue string_null_moved(std::move(string_null));

  // Check that the original has been cleared out.
  EXPECT_EQ(kInt, string_null.getTypeID());
  EXPECT_FALSE(string_null.isNull());
  EXPECT_FALSE(string_null.ownsOutOfLineData());

  CheckStringMetadataForNull(string_null_moved, type_id);
}

TEST(TypedValueTest, StringMoveTest) {
  CheckStringMove(kSampleStringShort);
  CheckStringMove(kSampleStringLong);

  CheckStringNullMove(kChar);
  CheckStringNullMove(kVarChar);
}

template <typename NumericType>
void CheckNumericMoveAssign(const NumericType literal) {
  TypedValue value(literal);
  TypeID expected_type_id = value.getTypeID();
  TypedValue value_moved = std::move(value);

  // Check that the original has been cleared out.
  EXPECT_EQ(kInt, value.getTypeID());
  EXPECT_FALSE(value.isNull());
  EXPECT_FALSE(value.ownsOutOfLineData());

  CheckNumericMetadata<NumericType>(value_moved, expected_type_id);
  CheckNumericData<NumericType>(value_moved, literal);

  // Check self-move-assignment.
  value_moved = std::move(value_moved);
  CheckNumericMetadata<NumericType>(value_moved, expected_type_id);
  CheckNumericData<NumericType>(value_moved, literal);
}

void CheckNumericNullMoveAssign(const TypeID type_id) {
  TypedValue value_moved;
  TypedValue numeric_null(type_id);
  value_moved = std::move(numeric_null);

  // Check that the original has been cleared out.
  EXPECT_EQ(kInt, numeric_null.getTypeID());
  EXPECT_FALSE(numeric_null.isNull());
  EXPECT_FALSE(numeric_null.ownsOutOfLineData());

  CheckNumericMetadataForNull(value_moved, type_id);

  // Also check self-move-assignment.
  value_moved = std::move(value_moved);
  CheckNumericMetadataForNull(value_moved, type_id);
}

TEST(TypedValueTest, NumericMoveAssignTest) {
  CheckNumericMoveAssign<int>(0);
  CheckNumericMoveAssign<int>(123);
  CheckNumericMoveAssign<int>(-123);
  CheckNumericMoveAssign<int>(numeric_limits<int>::max());
  CheckNumericMoveAssign<int>(numeric_limits<int>::min());
  CheckNumericNullMoveAssign(kInt);

  CheckNumericMoveAssign<std::int64_t>(0);
  CheckNumericMoveAssign<std::int64_t>(123);
  CheckNumericMoveAssign<std::int64_t>(-123);
  CheckNumericMoveAssign<std::int64_t>(numeric_limits<std::int64_t>::max());
  CheckNumericMoveAssign<std::int64_t>(numeric_limits<std::int64_t>::min());
  CheckNumericNullMoveAssign(kLong);

  CheckNumericMoveAssign<float>(0.0);
  CheckNumericMoveAssign<float>(123.45);
  CheckNumericMoveAssign<float>(-123.45);
  CheckNumericMoveAssign<float>(numeric_limits<float>::max());
  CheckNumericMoveAssign<float>(numeric_limits<float>::min());
  CheckNumericNullMoveAssign(kFloat);

  CheckNumericMoveAssign<double>(0.0);
  CheckNumericMoveAssign<double>(123.45);
  CheckNumericMoveAssign<double>(-123.45);
  CheckNumericMoveAssign<double>(numeric_limits<double>::max());
  CheckNumericMoveAssign<double>(numeric_limits<double>::min());
  CheckNumericNullMoveAssign(kDouble);
}

void CheckStringMoveAssign(const std::string &string_literal) {
  TypedValue char_ref_orig(kChar,
                           string_literal.c_str(),
                           string_literal.size());
  TypedValue char_ref = std::move(char_ref_orig);
  // Check that original has been cleared out.
  EXPECT_EQ(kInt, char_ref_orig.getTypeID());
  EXPECT_FALSE(char_ref_orig.isNull());
  EXPECT_FALSE(char_ref_orig.ownsOutOfLineData());
  // Check metadata, then data.
  EXPECT_TRUE(char_ref.isReference());
  EXPECT_FALSE(char_ref.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref.getTypeID());
  EXPECT_EQ(string_literal.size(), char_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref.getAsciiStringLength());
  CheckStringData(char_ref, string_literal, false);
  // Check self-move-assignment.
  char_ref = std::move(char_ref);
  EXPECT_TRUE(char_ref.isReference());
  EXPECT_FALSE(char_ref.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref.getTypeID());
  EXPECT_EQ(string_literal.size(), char_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref.getAsciiStringLength());
  CheckStringData(char_ref, string_literal, false);

  TypedValue char_lit_orig(kChar,
                           string_literal.c_str(),
                           string_literal.size());
  char_lit_orig.ensureNotReference();
  TypedValue char_lit = std::move(char_lit_orig);
  EXPECT_EQ(kInt, char_lit_orig.getTypeID());
  EXPECT_FALSE(char_lit_orig.isNull());
  EXPECT_FALSE(char_lit_orig.ownsOutOfLineData());
  EXPECT_FALSE(char_lit.isReference());
  EXPECT_TRUE(char_lit.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit.getTypeID());
  EXPECT_EQ(string_literal.size(), char_lit.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit.getAsciiStringLength());
  CheckStringData(char_lit, string_literal, false);
  // Self-move-assignment.
  char_lit = std::move(char_lit);
  EXPECT_FALSE(char_lit.isReference());
  EXPECT_TRUE(char_lit.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit.getTypeID());
  EXPECT_EQ(string_literal.size(), char_lit.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit.getAsciiStringLength());
  CheckStringData(char_lit, string_literal, false);


  TypedValue char_ref_with_nullterm_orig(kChar,
                                         string_literal.c_str(),
                                         string_literal.size() + 1);
  TypedValue char_ref_with_nullterm = std::move(char_ref_with_nullterm_orig);
  EXPECT_EQ(kInt, char_ref_with_nullterm_orig.getTypeID());
  EXPECT_FALSE(char_ref_with_nullterm_orig.isNull());
  EXPECT_FALSE(char_ref_with_nullterm_orig.ownsOutOfLineData());
  EXPECT_TRUE(char_ref_with_nullterm.isReference());
  EXPECT_FALSE(char_ref_with_nullterm.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref_with_nullterm.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, char_ref_with_nullterm.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref_with_nullterm.getAsciiStringLength());
  CheckStringData(char_ref_with_nullterm, string_literal, true);
  // Self-move-assignment.
  char_ref_with_nullterm = std::move(char_ref_with_nullterm);
  EXPECT_TRUE(char_ref_with_nullterm.isReference());
  EXPECT_FALSE(char_ref_with_nullterm.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref_with_nullterm.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, char_ref_with_nullterm.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref_with_nullterm.getAsciiStringLength());
  CheckStringData(char_ref_with_nullterm, string_literal, true);

  TypedValue char_lit_with_nullterm_orig(kChar,
                                         string_literal.c_str(),
                                         string_literal.size() + 1);
  char_lit_with_nullterm_orig.ensureNotReference();
  TypedValue char_lit_with_nullterm = std::move(char_lit_with_nullterm_orig);
  EXPECT_EQ(kInt, char_lit_with_nullterm_orig.getTypeID());
  EXPECT_FALSE(char_lit_with_nullterm_orig.isNull());
  EXPECT_FALSE(char_lit_with_nullterm_orig.ownsOutOfLineData());
  EXPECT_FALSE(char_lit_with_nullterm.isReference());
  EXPECT_TRUE(char_lit_with_nullterm.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit_with_nullterm.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, char_lit_with_nullterm.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit_with_nullterm.getAsciiStringLength());
  CheckStringData(char_lit_with_nullterm, string_literal, true);
  // Self-move-assignment.
  char_lit_with_nullterm = std::move(char_lit_with_nullterm);
  EXPECT_FALSE(char_lit_with_nullterm.isReference());
  EXPECT_TRUE(char_lit_with_nullterm.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit_with_nullterm.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, char_lit_with_nullterm.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit_with_nullterm.getAsciiStringLength());
  CheckStringData(char_lit_with_nullterm, string_literal, true);


  ScopedBuffer extended_char_buffer(string_literal.size() + 5);
  std::memset(extended_char_buffer.get(), 0, string_literal.size() + 5);
  std::memcpy(extended_char_buffer.get(),
              string_literal.c_str(),
              string_literal.size());
  TypedValue char_ref_extended_orig(kChar,
                                    extended_char_buffer.get(),
                                    string_literal.size() + 5);
  TypedValue char_ref_extended = std::move(char_ref_extended_orig);
  EXPECT_EQ(kInt, char_ref_extended_orig.getTypeID());
  EXPECT_FALSE(char_ref_extended_orig.isNull());
  EXPECT_FALSE(char_ref_extended_orig.ownsOutOfLineData());
  EXPECT_TRUE(char_ref_extended.isReference());
  EXPECT_FALSE(char_ref_extended.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref_extended.getTypeID());
  EXPECT_EQ(string_literal.size() + 5, char_ref_extended.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref_extended.getAsciiStringLength());
  CheckStringData(char_ref_extended, string_literal, true);
  // Self-move-assignment.
  char_ref_extended = std::move(char_ref_extended);
  EXPECT_TRUE(char_ref_extended.isReference());
  EXPECT_FALSE(char_ref_extended.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_ref_extended.getTypeID());
  EXPECT_EQ(string_literal.size() + 5, char_ref_extended.getDataSize());
  EXPECT_EQ(string_literal.size(), char_ref_extended.getAsciiStringLength());
  CheckStringData(char_ref_extended, string_literal, true);


  TypedValue char_lit_extended_orig(kChar,
                                    extended_char_buffer.get(),
                                    string_literal.size() + 5);
  char_lit_extended_orig.ensureNotReference();
  TypedValue char_lit_extended = std::move(char_lit_extended_orig);
  EXPECT_EQ(kInt, char_lit_extended_orig.getTypeID());
  EXPECT_FALSE(char_lit_extended_orig.isNull());
  EXPECT_FALSE(char_lit_extended_orig.ownsOutOfLineData());
  EXPECT_FALSE(char_lit_extended.isReference());
  EXPECT_TRUE(char_lit_extended.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit_extended.getTypeID());
  EXPECT_EQ(string_literal.size() + 5, char_lit_extended.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit_extended.getAsciiStringLength());
  CheckStringData(char_lit_extended, string_literal, true);
  // Self-move-assignment.
  char_lit_extended = std::move(char_lit_extended);
  EXPECT_FALSE(char_lit_extended.isReference());
  EXPECT_TRUE(char_lit_extended.ownsOutOfLineData());
  EXPECT_EQ(kChar, char_lit_extended.getTypeID());
  EXPECT_EQ(string_literal.size() + 5, char_lit_extended.getDataSize());
  EXPECT_EQ(string_literal.size(), char_lit_extended.getAsciiStringLength());
  CheckStringData(char_lit_extended, string_literal, true);


  TypedValue varchar_ref_orig(kVarChar,
                              string_literal.c_str(),
                              string_literal.size() + 1);
  TypedValue varchar_ref = std::move(varchar_ref_orig);
  EXPECT_EQ(kInt, varchar_ref_orig.getTypeID());
  EXPECT_FALSE(varchar_ref_orig.isNull());
  EXPECT_FALSE(varchar_ref_orig.ownsOutOfLineData());
  EXPECT_TRUE(varchar_ref.isReference());
  EXPECT_FALSE(varchar_ref.ownsOutOfLineData());
  EXPECT_EQ(kVarChar, varchar_ref.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, varchar_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), varchar_ref.getAsciiStringLength());
  CheckStringData(varchar_ref, string_literal, true);
  // Self-move-assignment.
  varchar_ref = std::move(varchar_ref);
  EXPECT_TRUE(varchar_ref.isReference());
  EXPECT_FALSE(varchar_ref.ownsOutOfLineData());
  EXPECT_EQ(kVarChar, varchar_ref.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, varchar_ref.getDataSize());
  EXPECT_EQ(string_literal.size(), varchar_ref.getAsciiStringLength());
  CheckStringData(varchar_ref, string_literal, true);


  TypedValue varchar_lit_orig(kVarChar,
                              string_literal.c_str(),
                              string_literal.size() + 1);
  varchar_lit_orig.ensureNotReference();
  TypedValue varchar_lit = std::move(varchar_lit_orig);
  EXPECT_EQ(kInt, varchar_lit_orig.getTypeID());
  EXPECT_FALSE(varchar_lit_orig.isNull());
  EXPECT_FALSE(varchar_lit_orig.ownsOutOfLineData());
  EXPECT_FALSE(varchar_lit.isReference());
  EXPECT_TRUE(varchar_lit.ownsOutOfLineData());
  EXPECT_EQ(kVarChar, varchar_lit.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, varchar_lit.getDataSize());
  EXPECT_EQ(string_literal.size(), varchar_lit.getAsciiStringLength());
  CheckStringData(varchar_lit, string_literal, true);
  // Self-move-assignment.
  varchar_lit = std::move(varchar_lit);
  EXPECT_FALSE(varchar_lit.isReference());
  EXPECT_TRUE(varchar_lit.ownsOutOfLineData());
  EXPECT_EQ(kVarChar, varchar_lit.getTypeID());
  EXPECT_EQ(string_literal.size() + 1, varchar_lit.getDataSize());
  EXPECT_EQ(string_literal.size(), varchar_lit.getAsciiStringLength());
  CheckStringData(varchar_lit, string_literal, true);
}

void CheckStringNullMoveAssign(const TypeID type_id) {
  TypedValue value_moved;
  TypedValue string_null(type_id);
  value_moved = std::move(string_null);

  // Check that the original has been cleared out.
  EXPECT_EQ(kInt, string_null.getTypeID());
  EXPECT_FALSE(string_null.isNull());
  EXPECT_FALSE(string_null.ownsOutOfLineData());

  CheckStringMetadataForNull(value_moved, type_id);

  // Also check self-move-assignment.
  value_moved = std::move(value_moved);
  CheckStringMetadataForNull(value_moved, type_id);
}

TEST(TypedValueTest, StringMoveAssignTest) {
  CheckStringMoveAssign(kSampleStringShort);
  CheckStringMoveAssign(kSampleStringLong);

  CheckStringNullMoveAssign(kChar);
  CheckStringNullMoveAssign(kVarChar);
}

void CheckHashEquivalence(const vector<TypedValue> &values) {
  for (vector<TypedValue>::const_iterator value_it = values.begin();
       value_it != values.end();
       ++value_it) {
    std::size_t original_hash = value_it->getHash();
    TypedValue value_ref = value_it->makeReferenceToThis();
    EXPECT_EQ(original_hash, value_ref.getHash());
  }
}

void CheckHashCollisions(const vector<TypedValue> &values) {
  vector<std::size_t> hashes;
  for (vector<TypedValue>::const_iterator value_it = values.begin();
       value_it != values.end();
       ++value_it) {
    hashes.push_back(value_it->getHash());
  }

  for (vector<std::size_t>::const_iterator hash_it = hashes.begin();
       hash_it != hashes.end();
       ++hash_it) {
    for (vector<std::size_t>::const_iterator compare_it = hashes.begin();
         compare_it != hashes.end();
         ++compare_it) {
      if (hash_it != compare_it) {
        EXPECT_NE(*hash_it, *compare_it);
      }
    }
  }
}

template <typename ScalarLiteralType>
void CheckHashReversal(const vector<TypedValue> &values) {
  for (const TypedValue &original_value : values) {
    const std::size_t hash = original_value.getHash();
    TypedValue reconstructed(original_value.getTypeID(), hash);
    EXPECT_EQ(original_value.getLiteral<ScalarLiteralType>(),
              reconstructed.getLiteral<ScalarLiteralType>());
  }
}

template <typename NumericType>
void CheckNumericHash() {
  vector<TypedValue> values;
  values.push_back(TypedValue(static_cast<NumericType>(0)));
  values.push_back(TypedValue(static_cast<NumericType>(1)));
  values.push_back(TypedValue(static_cast<NumericType>(2)));
  values.push_back(TypedValue(static_cast<NumericType>(-1)));
  values.push_back(TypedValue(static_cast<NumericType>(-2)));
  values.push_back(TypedValue(static_cast<NumericType>(123.45)));
  values.push_back(TypedValue(static_cast<NumericType>(-123.45)));
  values.push_back(TypedValue(numeric_limits<NumericType>::max()));
  values.push_back(TypedValue(numeric_limits<NumericType>::min()));

  CheckHashEquivalence(values);

  if (sizeof(NumericType) <= sizeof(std::size_t)) {
    CheckHashReversal<NumericType>(values);
  }

  // On 32-bit platforms, INT64_MIN and INT64_MAX can collide with hashes for
  // 0 and -1.
  if (std::is_same<NumericType, std::int64_t>::value
      && (sizeof(std::size_t) < 8)) {
    values.resize(values.size() - 2);
  }
  CheckHashCollisions(values);
}

TEST(TypedValueTest, HashTest) {
  CheckNumericHash<int>();
  CheckNumericHash<std::int64_t>();
  CheckNumericHash<float>();
  CheckNumericHash<double>();

  vector<TypedValue> char_values;
  char_values.push_back(TypedValue(kChar,
                                   kSampleStringShort,
                                   sizeof(kSampleStringShort) - 1));
  char_values.push_back(TypedValue(kChar,
                                   kSampleStringLong,
                                   sizeof(kSampleStringLong) - 1));
  CheckHashEquivalence(char_values);
  CheckHashCollisions(char_values);

  vector<TypedValue> varchar_values;
  varchar_values.push_back(TypedValue(kVarChar,
                                      kSampleStringShort,
                                      sizeof(kSampleStringShort)));
  varchar_values.push_back(TypedValue(kVarChar,
                                      kSampleStringLong,
                                      sizeof(kSampleStringLong)));
  CheckHashEquivalence(varchar_values);
  CheckHashCollisions(varchar_values);
}

// Check that different representations of the same string have the same hash.
void CheckStringHashEquality(const std::string &literal) {
  TypedValue char_ref(kChar,
                      literal.c_str(),
                      literal.size());
  std::size_t string_hash = char_ref.getHash();

  TypedValue char_lit(char_ref);
  char_lit.ensureNotReference();
  EXPECT_EQ(string_hash, char_lit.getHash());

  TypedValue char_ref_with_nullterm(kChar,
                                    literal.c_str(),
                                    literal.size() + 1);
  EXPECT_EQ(string_hash, char_ref_with_nullterm.getHash());

  TypedValue char_lit_with_nullterm(char_ref_with_nullterm);
  char_lit_with_nullterm.ensureNotReference();
  EXPECT_EQ(string_hash, char_lit_with_nullterm.getHash());

  ScopedBuffer extended_char_buffer(literal.size() + 10);
  std::memcpy(extended_char_buffer.get(),
              literal.c_str(),
              literal.size() + 1);
  TypedValue char_ref_extended(kChar,
                               extended_char_buffer.get(),
                               literal.size() + 10);
  EXPECT_EQ(string_hash, char_ref_extended.getHash());

  TypedValue char_lit_extended(char_ref_extended);
  char_lit_extended.ensureNotReference();
  EXPECT_EQ(string_hash, char_lit_extended.getHash());

  TypedValue varchar_ref(kVarChar,
                         literal.c_str(),
                         literal.size() + 1);
  EXPECT_EQ(string_hash, varchar_ref.getHash());

  TypedValue varchar_lit(varchar_ref);
  varchar_lit.ensureNotReference();
  EXPECT_EQ(string_hash, varchar_lit.getHash());
}

TEST(TypedValueTest, StringHashEqualityTest) {
  CheckStringHashEquality(kSampleStringShort);
  CheckStringHashEquality(kSampleStringLong);
}

// Check that a value is equivalent to the "default" value produced by the
// zero-argument constructor.
void CheckValueIsDefault(const TypedValue &value) {
  EXPECT_FALSE(value.isNull());
  EXPECT_FALSE(value.isReference());
  EXPECT_FALSE(value.ownsOutOfLineData());
  EXPECT_EQ(kInt, value.getTypeID());
  EXPECT_EQ(sizeof(int), value.getDataSize());
  EXPECT_EQ(0, value.getLiteral<int>());
}

TEST(TypedValueTest, ClearTest) {
  TypedValue int_value(42);
  int_value.clear();
  CheckValueIsDefault(int_value);

  TypedValue long_value(static_cast<std::int64_t>(42));
  long_value.clear();
  CheckValueIsDefault(long_value);

  TypedValue float_value(-12.5);
  float_value.clear();
  CheckValueIsDefault(float_value);

  TypedValue double_value(static_cast<double>(123.45));
  double_value.clear();
  CheckValueIsDefault(double_value);

  TypedValue char_ref(kChar,
                      kSampleStringShort,
                      std::strlen(kSampleStringShort));
  char_ref.clear();
  CheckValueIsDefault(char_ref);

  TypedValue char_literal(kChar,
                          kSampleStringShort,
                          std::strlen(kSampleStringShort));
  char_literal.ensureNotReference();
  char_literal.clear();
  CheckValueIsDefault(char_literal);

  TypedValue varchar_ref(kVarChar,
                         kSampleStringShort,
                         std::strlen(kSampleStringShort) + 1);
  varchar_ref.clear();
  CheckValueIsDefault(varchar_ref);

  TypedValue varchar_literal(kVarChar,
                             kSampleStringShort,
                             std::strlen(kSampleStringShort) + 1);
  varchar_literal.ensureNotReference();
  varchar_literal.clear();
  CheckValueIsDefault(varchar_literal);

  // Also check a variety of nulls.
  for (TypeID type_id = static_cast<TypeID>(0);
       type_id < kNumTypeIDs;
       type_id = static_cast<TypeID>(type_id + 1)) {
    TypedValue null_value(type_id);
    null_value.clear();
    CheckValueIsDefault(null_value);
  }
}

TEST(TypedValueTest, LowLevelMemoryOpsTest) {
  TypedValue value_array[6];

  // Zero-out array.
  std::memset(value_array, 0, sizeof(TypedValue) * 5);
  CheckValueIsDefault(value_array[0]);
  CheckValueIsDefault(value_array[1]);
  CheckValueIsDefault(value_array[2]);
  CheckValueIsDefault(value_array[3]);
  CheckValueIsDefault(value_array[4]);
  CheckValueIsDefault(value_array[5]);

  // Fill in some values.
  value_array[0] = TypedValue(42);
  value_array[1] = TypedValue(static_cast<float>(12.5));
  value_array[2] = TypedValue(kChar,
                              kSampleStringShort,
                              std::strlen(kSampleStringShort));
  value_array[3] = TypedValue(kVarChar,
                              kSampleStringLong,
                              std::strlen(kSampleStringLong) + 1);
  value_array[4] = TypedValue(static_cast<std::int64_t>(-12345));
  value_array[5] = TypedValue(kDouble);

  // memcpy values.
  TypedValue other_value;

  std::memcpy(&other_value, value_array, sizeof(TypedValue));
  EXPECT_EQ(kInt, other_value.getTypeID());
  EXPECT_EQ(42, other_value.getLiteral<int>());

  std::memcpy(&other_value, value_array + 1, sizeof(TypedValue));
  EXPECT_EQ(kFloat, other_value.getTypeID());
  EXPECT_EQ(12.5, other_value.getLiteral<float>());

  std::memcpy(&other_value, value_array + 2, sizeof(TypedValue));
  EXPECT_EQ(kChar, other_value.getTypeID());
  ASSERT_EQ(std::strlen(kSampleStringShort), other_value.getDataSize());
  EXPECT_EQ(0, std::memcmp(kSampleStringShort, other_value.getDataPtr(), other_value.getDataSize()));

  std::memcpy(&other_value, value_array + 3, sizeof(TypedValue));
  EXPECT_EQ(kVarChar, other_value.getTypeID());
  EXPECT_EQ(std::strlen(kSampleStringLong) + 1, other_value.getDataSize());
  EXPECT_STREQ(kSampleStringLong, static_cast<const char*>(other_value.getDataPtr()));

  std::memcpy(&other_value, value_array + 4, sizeof(TypedValue));
  EXPECT_EQ(kLong, other_value.getTypeID());
  EXPECT_EQ(-12345, other_value.getLiteral<std::int64_t>());

  std::memcpy(&other_value, value_array + 5, sizeof(TypedValue));
  EXPECT_EQ(kDouble, other_value.getTypeID());
  EXPECT_TRUE(other_value.isNull());

  // memmove to shift values inside array.
  std::memmove(value_array, value_array + 1, 5 * sizeof(TypedValue));

  EXPECT_EQ(kFloat, value_array[0].getTypeID());
  EXPECT_EQ(12.5, value_array[0].getLiteral<float>());

  EXPECT_EQ(kChar, value_array[1].getTypeID());
  ASSERT_EQ(std::strlen(kSampleStringShort), value_array[1].getDataSize());
  EXPECT_EQ(0, std::memcmp(kSampleStringShort, value_array[1].getDataPtr(), value_array[1].getDataSize()));

  EXPECT_EQ(kVarChar, value_array[2].getTypeID());
  EXPECT_EQ(std::strlen(kSampleStringLong) + 1, value_array[2].getDataSize());
  EXPECT_STREQ(kSampleStringLong, static_cast<const char*>(value_array[2].getDataPtr()));

  EXPECT_EQ(kLong, value_array[3].getTypeID());
  EXPECT_EQ(-12345, value_array[3].getLiteral<std::int64_t>());

  EXPECT_EQ(kDouble, value_array[4].getTypeID());
  EXPECT_TRUE(value_array[4].isNull());

  EXPECT_EQ(kDouble, value_array[5].getTypeID());
  EXPECT_TRUE(value_array[5].isNull());
}

void CheckNumericSerialization(const TypedValue &value) {
  TypedValue value_from_proto = TypedValue::ReconstructFromProto(value.getProto());
  EXPECT_EQ(value.isNull(), value_from_proto.isNull());
  EXPECT_EQ(value.getTypeID(), value_from_proto.getTypeID());

  if (value.isNull()) {
    return;
  }

  switch (value.getTypeID()) {
    case kInt: {
      EXPECT_EQ(value.getLiteral<int>(), value_from_proto.getLiteral<int>());
      break;
    }
    case kLong: {
      EXPECT_EQ(value.getLiteral<int64_t>(), value_from_proto.getLiteral<int64_t>());
      break;
    }
    case kFloat: {
      EXPECT_EQ(value.getLiteral<float>(), value_from_proto.getLiteral<float>());
      break;
    }
    case kDouble: {
      EXPECT_EQ(value.getLiteral<double>(), value_from_proto.getLiteral<double>());
      break;
    }
    default:
      FATAL_ERROR("value is not a numeric TypedValue in CheckNumericSerialization");
  }
}

TEST(TypedValueTest, NumericSerializationTest) {
  TypedValue int_zero(0);
  CheckNumericSerialization(int_zero);
  TypedValue int_max(numeric_limits<int>::max());
  CheckNumericSerialization(int_max);
  TypedValue int_min(numeric_limits<int>::min());
  CheckNumericSerialization(int_min);
  TypedValue int_null(kInt);
  CheckNumericSerialization(int_null);

  TypedValue long_zero(static_cast<std::int64_t>(0));
  CheckNumericSerialization(long_zero);
  TypedValue long_max(numeric_limits<std::int64_t>::max());
  CheckNumericSerialization(long_max);
  TypedValue long_min(numeric_limits<std::int64_t>::min());
  CheckNumericSerialization(long_min);
  TypedValue long_null(kLong);
  CheckNumericSerialization(long_null);

  TypedValue float_zero(static_cast<float>(0.0));
  CheckNumericSerialization(float_zero);
  TypedValue float_positive(static_cast<float>(123.45));
  CheckNumericSerialization(float_positive);
  TypedValue float_negative(static_cast<float>(-123.45));
  CheckNumericSerialization(float_negative);
  TypedValue float_max(numeric_limits<float>::max());
  CheckNumericSerialization(float_max);
  TypedValue float_min(numeric_limits<float>::min());
  CheckNumericSerialization(float_min);
  TypedValue float_null(kFloat);
  CheckNumericSerialization(float_null);

  TypedValue double_zero(static_cast<double>(0.0));
  CheckNumericSerialization(double_zero);
  TypedValue double_positive(static_cast<double>(123.45));
  CheckNumericSerialization(double_positive);
  TypedValue double_negative(static_cast<double>(-123.45));
  CheckNumericSerialization(double_negative);
  TypedValue double_max(numeric_limits<double>::max());
  CheckNumericSerialization(double_max);
  TypedValue double_min(numeric_limits<double>::min());
  CheckNumericSerialization(double_min);
  TypedValue double_null(kDouble);
  CheckNumericSerialization(double_null);
}

void CheckStringProto(const TypedValue &value, const std::string &expected, const bool expect_null_terminator) {
  TypedValue value_from_proto = TypedValue::ReconstructFromProto(value.getProto());
  ASSERT_FALSE(value.isNull());
  ASSERT_FALSE(value_from_proto.isNull());
  EXPECT_EQ(value.getTypeID(), value_from_proto.getTypeID());
  EXPECT_EQ(value.getDataSize(), value_from_proto.getDataSize());
  EXPECT_EQ(value.getAsciiStringLength(), value_from_proto.getAsciiStringLength());

  switch (value.getTypeID()) {
    case kChar: {
      if (expect_null_terminator) {
        EXPECT_STREQ(expected.c_str(), static_cast<const char*>(value_from_proto.getDataPtr()));
      } else {
        EXPECT_EQ(0, std::strncmp(expected.c_str(),
                                  static_cast<const char*>(value_from_proto.getDataPtr()),
                                  expected.size()));
      }
      break;
    }
    case kVarChar: {
      EXPECT_STREQ(expected.c_str(), static_cast<const char*>(value_from_proto.getDataPtr()));
      break;
    }
    default:
      FATAL_ERROR("value is not a string TypedValue in CheckStringProto");
  }
}

void CheckStringSerialization(const std::string &string_literal) {
  TypedValue char_ref(kChar, string_literal.c_str(), string_literal.size());
  CheckStringProto(char_ref, string_literal, false);

  TypedValue char_lit(kChar, string_literal.c_str(), string_literal.size());
  char_lit.ensureNotReference();
  CheckStringProto(char_lit, string_literal, false);

  TypedValue char_ref_with_nullterm(kChar, string_literal.c_str(), string_literal.size() + 1);
  CheckStringProto(char_ref_with_nullterm, string_literal, true);

  TypedValue char_lit_with_nullterm(kChar, string_literal.c_str(), string_literal.size() + 1);
  char_lit_with_nullterm.ensureNotReference();
  CheckStringProto(char_lit_with_nullterm, string_literal, true);

  ScopedBuffer extended_char_buffer(string_literal.size() + 5);
  std::memset(extended_char_buffer.get(), 0, string_literal.size() + 5);
  std::memcpy(extended_char_buffer.get(), string_literal.c_str(), string_literal.size());
  TypedValue char_ref_extended(kChar, extended_char_buffer.get(), string_literal.size() + 5);
  CheckStringProto(char_ref_extended, string_literal, true);

  TypedValue char_lit_extended(kChar, extended_char_buffer.get(), string_literal.size() + 5);
  char_lit_extended.ensureNotReference();
  CheckStringProto(char_lit_extended, string_literal, true);

  TypedValue varchar_ref(kVarChar, string_literal.c_str(), string_literal.size() + 1);
  CheckStringProto(varchar_ref, string_literal, true);

  TypedValue varchar_lit(kVarChar, string_literal.c_str(), string_literal.size() + 1);
  varchar_lit.ensureNotReference();
  CheckStringProto(varchar_lit, string_literal, true);
}

TEST(TypedValueTest, StringSerializationTest) {
  CheckStringSerialization(kSampleStringShort);
  CheckStringSerialization(kSampleStringLong);
}

void CheckNullStringSerialization(const TypedValue &value) {
  TypedValue value_from_proto = TypedValue::ReconstructFromProto(value.getProto());
  EXPECT_EQ(value.isNull(), value_from_proto.isNull());
  EXPECT_EQ(value.getTypeID(), value_from_proto.getTypeID());
}

TEST(TypedValueTest, NullStringSerializationTest) {
  TypedValue char_null(kChar);
  CheckNullStringSerialization(char_null);
  TypedValue var_char_null(kVarChar);
  CheckNullStringSerialization(var_char_null);
}

class DateTypedValueTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    // Datetime Type.
    DatetimeLit datetime;
    datetime.ticks = 0;
    datetime_zero.reset(new TypedValue(datetime));

    datetime.ticks = numeric_limits<std::int64_t>::max();
    datetime_max.reset(new TypedValue(datetime));

    datetime.ticks = numeric_limits<std::int64_t>::min();
    datetime_min.reset(new TypedValue(datetime));

    datetime_null.reset(new TypedValue(kDatetime));

    // DatetimeInterval Type.
    DatetimeIntervalLit datetime_interval;
    datetime_interval.interval_ticks = 0;
    datetime_interval_zero.reset(new TypedValue(datetime_interval));

    datetime_interval.interval_ticks = numeric_limits<std::int64_t>::max();
    datetime_interval_max.reset(new TypedValue(datetime_interval));

    datetime_interval.interval_ticks = numeric_limits<std::int64_t>::min();
    datetime_interval_min.reset(new TypedValue(datetime_interval));

    datetime_interval_null.reset(new TypedValue(kDatetimeInterval));

    // YearMonthInterval Type.
    YearMonthIntervalLit year_month_interval;
    year_month_interval.months = 0;
    year_month_interval_zero.reset(new TypedValue(year_month_interval));

    year_month_interval.months = numeric_limits<std::int64_t>::max();
    year_month_interval_max.reset(new TypedValue(year_month_interval));

    year_month_interval.months = numeric_limits<std::int64_t>::min();
    year_month_interval_min.reset(new TypedValue(year_month_interval));

    year_month_interval_null.reset(new TypedValue(kYearMonthInterval));
  }

  void CheckDateTypeMetadata(const TypedValue &value, TypeID expected_type_id) {
    EXPECT_FALSE(value.isNull());
    EXPECT_FALSE(value.isReference());
    EXPECT_FALSE(value.ownsOutOfLineData());
    EXPECT_EQ(expected_type_id, value.getTypeID());

    EXPECT_TRUE(value.isPlausibleInstanceOf(TypeFactory::GetType(expected_type_id).getSignature()));
    for (int type_id_int = 0;
         type_id_int < static_cast<int>(kNumTypeIDs);
         ++type_id_int) {
      TypeID type_id = static_cast<TypeID>(type_id_int);
      if (type_id != expected_type_id) {
        if (TypeFactory::TypeRequiresLengthParameter(type_id)) {
          EXPECT_FALSE(value.isPlausibleInstanceOf(TypeFactory::GetType(type_id, 10, false).getSignature()));
        } else if (type_id == kNullType) {
          EXPECT_FALSE(value.isPlausibleInstanceOf(TypeFactory::GetType(type_id, true).getSignature()));
        } else {
          EXPECT_FALSE(value.isPlausibleInstanceOf(TypeFactory::GetType(type_id, false).getSignature()));
        }
      }
    }
  }

  void CheckNullableDateTypeMetadata(const TypedValue &value, TypeID expected_type_id) {
    EXPECT_FALSE(value.isReference());
    EXPECT_FALSE(value.ownsOutOfLineData());
    EXPECT_EQ(expected_type_id, value.getTypeID());

    EXPECT_TRUE(value.isPlausibleInstanceOf(TypeFactory::GetType(expected_type_id, true).getSignature()));
    for (int type_id_int = 0;
         type_id_int < static_cast<int>(kNumTypeIDs);
         ++type_id_int) {
      TypeID type_id = static_cast<TypeID>(type_id_int);
      if (type_id != expected_type_id) {
        if (TypeFactory::TypeRequiresLengthParameter(type_id)) {
          EXPECT_FALSE(value.isPlausibleInstanceOf(TypeFactory::GetType(type_id, 10, true).getSignature()));
        } else {
          EXPECT_FALSE(value.isPlausibleInstanceOf(TypeFactory::GetType(type_id, true).getSignature()));
        }
      }
    }
  }

  void CheckDateTypeSerialization(const TypedValue &value) {
    TypedValue value_from_proto = TypedValue::ReconstructFromProto(value.getProto());
    EXPECT_EQ(value.isNull(), value_from_proto.isNull());
    EXPECT_EQ(value.getTypeID(), value_from_proto.getTypeID());

    if (value.isNull()) {
      return;
    }

    switch (value.getTypeID()) {
      case kDatetime: {
        EXPECT_EQ(value.getLiteral<DatetimeLit>(), value_from_proto.getLiteral<DatetimeLit>());
        break;
      }
      case kDatetimeInterval: {
        EXPECT_EQ(value.getLiteral<DatetimeIntervalLit>(), value_from_proto.getLiteral<DatetimeIntervalLit>());
        break;
      }
      case kYearMonthInterval: {
        EXPECT_EQ(value.getLiteral<YearMonthIntervalLit>(), value_from_proto.getLiteral<YearMonthIntervalLit>());
        break;
      }
      default:
        FATAL_ERROR("value is not a date TypedValue in CheckDateTypeSerialization");
    }
  }

  std::unique_ptr<TypedValue>
      datetime_zero, datetime_min, datetime_max, datetime_null,
      datetime_interval_zero, datetime_interval_min, datetime_interval_max, datetime_interval_null,
      year_month_interval_zero, year_month_interval_min, year_month_interval_max, year_month_interval_null;
};

TEST_F(DateTypedValueTest, MetadataTest) {
  CheckDateTypeMetadata(*datetime_zero, kDatetime);
  CheckDateTypeMetadata(*datetime_max, kDatetime);
  CheckDateTypeMetadata(*datetime_min, kDatetime);

  CheckDateTypeMetadata(*datetime_interval_zero, kDatetimeInterval);
  CheckDateTypeMetadata(*datetime_interval_max, kDatetimeInterval);
  CheckDateTypeMetadata(*datetime_interval_min, kDatetimeInterval);

  CheckDateTypeMetadata(*year_month_interval_zero, kYearMonthInterval);
  CheckDateTypeMetadata(*year_month_interval_max, kYearMonthInterval);
  CheckDateTypeMetadata(*year_month_interval_min, kYearMonthInterval);
}

TEST_F(DateTypedValueTest, NullableMetadataTest) {
  CheckNullableDateTypeMetadata(*datetime_null, kDatetime);
  CheckNullableDateTypeMetadata(*datetime_interval_null, kDatetimeInterval);
  CheckNullableDateTypeMetadata(*year_month_interval_null, kYearMonthInterval);
}

TEST_F(DateTypedValueTest, SerializationTest) {
  CheckDateTypeSerialization(*datetime_zero);
  CheckDateTypeSerialization(*datetime_max);
  CheckDateTypeSerialization(*datetime_min);
  CheckDateTypeSerialization(*datetime_null);

  CheckDateTypeSerialization(*datetime_interval_zero);
  CheckDateTypeSerialization(*datetime_interval_max);
  CheckDateTypeSerialization(*datetime_interval_min);
  CheckDateTypeSerialization(*datetime_interval_null);

  CheckDateTypeSerialization(*year_month_interval_zero);
  CheckDateTypeSerialization(*year_month_interval_max);
  CheckDateTypeSerialization(*year_month_interval_min);
  CheckDateTypeSerialization(*year_month_interval_null);
}

TEST_F(DateTypedValueTest, DateExtractTest) {
  DatetimeLit datetime;
  datetime.ticks = 1431359664 * DatetimeLit::kTicksPerSecond;  // Mon, 11 May 2015 15:54:24 GMT.

  EXPECT_EQ(2015, datetime.yearField());
  EXPECT_EQ(5, datetime.monthField());
  EXPECT_EQ(11, datetime.dayField());
  EXPECT_EQ(15, datetime.hourField());
  EXPECT_EQ(54, datetime.minuteField());
  EXPECT_EQ(24, datetime.secondField());
}

}  // namespace quickstep
