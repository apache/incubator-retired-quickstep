/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_TYPES_TESTS_TYPE_TEST_COMMON_HPP_
#define QUICKSTEP_TYPES_TESTS_TYPE_TEST_COMMON_HPP_

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <limits>
#include <string>

#include "types/DoubleType.hpp"
#include "types/FloatType.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/NullType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"

#include "gtest/gtest.h"

namespace quickstep {

namespace type_test {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief Check that an implementation of Type::parseValueFromString() returns
 *        success and parses a string as expected.
 *
 * @param TypeT A type that is represented as a fixed-size literal.
 * @param expected_literal The literal value that the string should be parsed
 *        as.
 * @param value_string A string representation of expected_literal to attempt
 *        to parse.
 **/
template <typename TypeT>
void CheckSuccessfulParseLiteralValueFromString(
    const typename TypeT::cpptype expected_literal,
    const std::string &value_string) {
  const Type &type = TypeT::InstanceNonNullable();
  TypedValue value;
  ASSERT_TRUE(type.parseValueFromString(value_string, &value));
  EXPECT_EQ(expected_literal, value.getLiteral<typename TypeT::cpptype>());
}

/**
 * @brief Check that a given Type is coercible from a specified set of types,
 *        and not from others.
 *
 * @param target_type The Type to test the isCoercibleFrom() method for. This
 *        should be non-nullable, but this function will also test the nullable
 *        version.
 * @param expected_type A list of all the TypeIDs of Types that should be
 *        coercible to target_type.
 **/
void CheckIsCoercibleFrom(
    const Type &target_type,
    const std::initializer_list<TypeID> &expected_coercible) {
  const Type &target_type_nullable = target_type.getNullableVersion();

  for (const TypeID original_type_id : expected_coercible) {
    const Type &original_type = TypeFactory::TypeRequiresLengthParameter(original_type_id)
                                ? TypeFactory::GetType(original_type_id, 10, false)
                                : TypeFactory::GetType(original_type_id, false);
    const Type &original_type_nullable = original_type.getNullableVersion();

    EXPECT_TRUE(target_type.isCoercibleFrom(original_type));
    // Can't coerce from nullable to non-nullable.
    EXPECT_FALSE(target_type.isCoercibleFrom(original_type_nullable));

    EXPECT_TRUE(target_type_nullable.isCoercibleFrom(original_type));
    EXPECT_TRUE(target_type_nullable.isCoercibleFrom(original_type_nullable));
  }

  // NullType is coercible to any nullable type.
  EXPECT_FALSE(target_type.isCoercibleFrom(TypeFactory::GetType(kNullType, true)));
  EXPECT_TRUE(target_type_nullable.isCoercibleFrom(TypeFactory::GetType(kNullType, true)));

  // Can't coerce from other types.
  for (const TypeID original_type_id
       : {kInt, kLong, kFloat, kDouble, kChar, kVarChar,
          kDatetime, kDatetimeInterval, kYearMonthInterval}) {
    if (std::find(expected_coercible.begin(), expected_coercible.end(), original_type_id)
        == expected_coercible.end()) {
      const Type &original_type = TypeFactory::TypeRequiresLengthParameter(original_type_id)
                                  ? TypeFactory::GetType(original_type_id, 10, false)
                                  : TypeFactory::GetType(original_type_id, false);
      const Type &original_type_nullable = original_type.getNullableVersion();

      EXPECT_FALSE(target_type.isCoercibleFrom(original_type));
      EXPECT_FALSE(target_type.isCoercibleFrom(original_type_nullable));
      EXPECT_FALSE(target_type_nullable.isCoercibleFrom(original_type));
      EXPECT_FALSE(target_type_nullable.isCoercibleFrom(original_type_nullable));
    }
  }
}

/**
 * @brief Check that an ASCII string type (i.e. CharType or VarCharType) is
 *        coercible from exactly the appropriate types (i.e. from any other
 *        ASCII string type).
 *
 * @param target_type_id The TypeID of the string Type to test.
 * @param target_length The length parameter of the string Type to test.
 **/
void CheckStringTypeIsCoercibleFrom(const TypeID target_type_id,
                                    const std::size_t target_length) {
  const Type &target_type = TypeFactory::GetType(target_type_id, target_length, false);
  const Type &target_type_nullable = TypeFactory::GetType(target_type_id, target_length, true);

  // Other string types to test against.
  const Type &shorter_char = TypeFactory::GetType(kChar, target_length - 1, false);
  const Type &shorter_char_nullable = TypeFactory::GetType(kChar, target_length - 1, true);
  const Type &same_char = TypeFactory::GetType(kChar, target_length, false);
  const Type &same_char_nullable = TypeFactory::GetType(kChar, target_length, true);
  const Type &longer_char = TypeFactory::GetType(kChar, target_length + 1, false);
  const Type &longer_char_nullable = TypeFactory::GetType(kChar, target_length + 1, true);

  const Type &shorter_varchar = TypeFactory::GetType(kVarChar, target_length - 1, false);
  const Type &shorter_varchar_nullable = TypeFactory::GetType(kVarChar, target_length - 1, true);
  const Type &same_varchar = TypeFactory::GetType(kVarChar, target_length, false);
  const Type &same_varchar_nullable = TypeFactory::GetType(kVarChar, target_length, true);
  const Type &longer_varchar = TypeFactory::GetType(kVarChar, target_length + 1, false);
  const Type &longer_varchar_nullable = TypeFactory::GetType(kVarChar, target_length + 1, true);

  // A non-nullable ASCII string type is coercible from any other non-nullable
  // ASCII string type.
  EXPECT_TRUE(target_type.isCoercibleFrom(shorter_char));
  EXPECT_FALSE(target_type.isCoercibleFrom(shorter_char_nullable));
  EXPECT_TRUE(target_type.isCoercibleFrom(same_char));
  EXPECT_FALSE(target_type.isCoercibleFrom(same_char_nullable));
  EXPECT_TRUE(target_type.isCoercibleFrom(longer_char));
  EXPECT_FALSE(target_type.isCoercibleFrom(longer_char_nullable));

  EXPECT_TRUE(target_type.isCoercibleFrom(shorter_varchar));
  EXPECT_FALSE(target_type.isCoercibleFrom(shorter_varchar_nullable));
  EXPECT_TRUE(target_type.isCoercibleFrom(same_varchar));
  EXPECT_FALSE(target_type.isCoercibleFrom(same_varchar_nullable));
  EXPECT_TRUE(target_type.isCoercibleFrom(longer_varchar));
  EXPECT_FALSE(target_type.isCoercibleFrom(longer_varchar_nullable));

  // A nullable ASCII string type is coercible from any other ASCII string
  // type.
  EXPECT_TRUE(target_type_nullable.isCoercibleFrom(shorter_char));
  EXPECT_TRUE(target_type_nullable.isCoercibleFrom(shorter_char_nullable));
  EXPECT_TRUE(target_type_nullable.isCoercibleFrom(same_char));
  EXPECT_TRUE(target_type_nullable.isCoercibleFrom(same_char_nullable));
  EXPECT_TRUE(target_type_nullable.isCoercibleFrom(longer_char));
  EXPECT_TRUE(target_type_nullable.isCoercibleFrom(longer_char_nullable));

  EXPECT_TRUE(target_type_nullable.isCoercibleFrom(shorter_varchar));
  EXPECT_TRUE(target_type_nullable.isCoercibleFrom(shorter_varchar_nullable));
  EXPECT_TRUE(target_type_nullable.isCoercibleFrom(same_varchar));
  EXPECT_TRUE(target_type_nullable.isCoercibleFrom(same_varchar_nullable));
  EXPECT_TRUE(target_type_nullable.isCoercibleFrom(longer_varchar));
  EXPECT_TRUE(target_type_nullable.isCoercibleFrom(longer_varchar_nullable));

  // NullType can be coerced to any other nullable type.
  EXPECT_FALSE(target_type.isCoercibleFrom(TypeFactory::GetType(kNullType, true)));
  EXPECT_TRUE(target_type_nullable.isCoercibleFrom(TypeFactory::GetType(kNullType, true)));

  // Other types are not coercible to strings.
  for (const TypeID original_type_id
       : {kInt, kLong, kFloat, kDouble, kDatetime, kDatetimeInterval, kYearMonthInterval}) {
    const Type &original_type = TypeFactory::TypeRequiresLengthParameter(original_type_id)
                                ? TypeFactory::GetType(original_type_id, 10, false)
                                : TypeFactory::GetType(original_type_id, false);
    const Type &original_type_nullable = original_type.getNullableVersion();

    EXPECT_FALSE(target_type.isCoercibleFrom(original_type));
    EXPECT_FALSE(target_type.isCoercibleFrom(original_type_nullable));
    EXPECT_FALSE(target_type_nullable.isCoercibleFrom(original_type));
    EXPECT_FALSE(target_type_nullable.isCoercibleFrom(original_type_nullable));
  }
}

/**
 * @brief Check that a given Type is safely coercible from a specified set of
 *        types, and not from others.
 *
 * @param target_type The Type to test the isSafelyCoercibleFrom() method for.
 *        This should be non-nullable, but this function will also test the
 *        nullable version.
 * @param expected_type A list of all the TypeIDs of Types that should be
 *        safely coercible to target_type.
 **/
void CheckIsSafelyCoercibleFrom(
    const Type &target_type,
    const std::initializer_list<TypeID> &expected_coercible) {
  const Type &target_type_nullable = target_type.getNullableVersion();

  for (const TypeID original_type_id : expected_coercible) {
    const Type &original_type = TypeFactory::TypeRequiresLengthParameter(original_type_id)
                                ? TypeFactory::GetType(original_type_id, 10, false)
                                : TypeFactory::GetType(original_type_id, false);
    const Type &original_type_nullable = original_type.getNullableVersion();

    EXPECT_TRUE(target_type.isSafelyCoercibleFrom(original_type));
    // Can't coerce from nullable to non-nullable.
    EXPECT_FALSE(target_type.isSafelyCoercibleFrom(original_type_nullable));

    EXPECT_TRUE(target_type_nullable.isSafelyCoercibleFrom(original_type));
    EXPECT_TRUE(target_type_nullable.isSafelyCoercibleFrom(original_type_nullable));
  }

  // NullType is coercible to any nullable type.
  EXPECT_FALSE(target_type.isSafelyCoercibleFrom(TypeFactory::GetType(kNullType, true)));
  EXPECT_TRUE(target_type_nullable.isSafelyCoercibleFrom(TypeFactory::GetType(kNullType, true)));

  // Can't coerce from other types.
  for (const TypeID original_type_id
       : {kInt, kLong, kFloat, kDouble, kChar, kVarChar,
          kDatetime, kDatetimeInterval, kYearMonthInterval}) {
    if (std::find(expected_coercible.begin(), expected_coercible.end(), original_type_id)
        == expected_coercible.end()) {
      const Type &original_type = TypeFactory::TypeRequiresLengthParameter(original_type_id)
                                  ? TypeFactory::GetType(original_type_id, 10, false)
                                  : TypeFactory::GetType(original_type_id, false);
      const Type &original_type_nullable = original_type.getNullableVersion();

      EXPECT_FALSE(target_type.isSafelyCoercibleFrom(original_type));
      EXPECT_FALSE(target_type.isSafelyCoercibleFrom(original_type_nullable));
      EXPECT_FALSE(target_type_nullable.isSafelyCoercibleFrom(original_type));
      EXPECT_FALSE(target_type_nullable.isSafelyCoercibleFrom(original_type_nullable));
    }
  }
}

/**
 * @brief Check that an ASCII string type (i.e. CharType or VarCharType) is
 *        safely coercible from exactly the appropriate types (i.e. from any
 *        ASCII string type with the same or shorter length).
 *
 * @param target_type_id The TypeID of the string Type to test.
 * @param target_length The length parameter of the string Type to test.
 **/
void CheckStringTypeIsSafelyCoercibleFrom(const TypeID target_type_id,
                                          const std::size_t target_length) {
  const Type &target_type = TypeFactory::GetType(target_type_id, target_length, false);
  const Type &target_type_nullable = TypeFactory::GetType(target_type_id, target_length, true);

  // Other string types to test against.
  const Type &shorter_char = TypeFactory::GetType(kChar, target_length - 1, false);
  const Type &shorter_char_nullable = TypeFactory::GetType(kChar, target_length - 1, true);
  const Type &same_char = TypeFactory::GetType(kChar, target_length, false);
  const Type &same_char_nullable = TypeFactory::GetType(kChar, target_length, true);
  const Type &longer_char = TypeFactory::GetType(kChar, target_length + 1, false);
  const Type &longer_char_nullable = TypeFactory::GetType(kChar, target_length + 1, true);

  const Type &shorter_varchar = TypeFactory::GetType(kVarChar, target_length - 1, false);
  const Type &shorter_varchar_nullable = TypeFactory::GetType(kVarChar, target_length - 1, true);
  const Type &same_varchar = TypeFactory::GetType(kVarChar, target_length, false);
  const Type &same_varchar_nullable = TypeFactory::GetType(kVarChar, target_length, true);
  const Type &longer_varchar = TypeFactory::GetType(kVarChar, target_length + 1, false);
  const Type &longer_varchar_nullable = TypeFactory::GetType(kVarChar, target_length + 1, true);

  // A non-nullable ASCII string type is safely coercible from other
  // non-nullable ASCII string types with the same or lesser length.
  EXPECT_TRUE(target_type.isSafelyCoercibleFrom(shorter_char));
  EXPECT_FALSE(target_type.isSafelyCoercibleFrom(shorter_char_nullable));
  EXPECT_TRUE(target_type.isSafelyCoercibleFrom(same_char));
  EXPECT_FALSE(target_type.isSafelyCoercibleFrom(same_char_nullable));
  EXPECT_FALSE(target_type.isSafelyCoercibleFrom(longer_char));
  EXPECT_FALSE(target_type.isSafelyCoercibleFrom(longer_char_nullable));

  EXPECT_TRUE(target_type.isSafelyCoercibleFrom(shorter_varchar));
  EXPECT_FALSE(target_type.isSafelyCoercibleFrom(shorter_varchar_nullable));
  EXPECT_TRUE(target_type.isSafelyCoercibleFrom(same_varchar));
  EXPECT_FALSE(target_type.isSafelyCoercibleFrom(same_varchar_nullable));
  EXPECT_FALSE(target_type.isSafelyCoercibleFrom(longer_varchar));
  EXPECT_FALSE(target_type.isSafelyCoercibleFrom(longer_varchar_nullable));

  // A nullable ASCII string type is safely coercible from other ASCII string
  // types with the same or lesser length.
  EXPECT_TRUE(target_type_nullable.isSafelyCoercibleFrom(shorter_char));
  EXPECT_TRUE(target_type_nullable.isSafelyCoercibleFrom(shorter_char_nullable));
  EXPECT_TRUE(target_type_nullable.isSafelyCoercibleFrom(same_char));
  EXPECT_TRUE(target_type_nullable.isSafelyCoercibleFrom(same_char_nullable));
  EXPECT_FALSE(target_type_nullable.isSafelyCoercibleFrom(longer_char));
  EXPECT_FALSE(target_type_nullable.isSafelyCoercibleFrom(longer_char_nullable));

  EXPECT_TRUE(target_type_nullable.isSafelyCoercibleFrom(shorter_varchar));
  EXPECT_TRUE(target_type_nullable.isSafelyCoercibleFrom(shorter_varchar_nullable));
  EXPECT_TRUE(target_type_nullable.isSafelyCoercibleFrom(same_varchar));
  EXPECT_TRUE(target_type_nullable.isSafelyCoercibleFrom(same_varchar_nullable));
  EXPECT_FALSE(target_type_nullable.isSafelyCoercibleFrom(longer_varchar));
  EXPECT_FALSE(target_type_nullable.isSafelyCoercibleFrom(longer_varchar_nullable));

  // NullType can be safely coerced to any other nullable type.
  EXPECT_FALSE(target_type.isSafelyCoercibleFrom(TypeFactory::GetType(kNullType, true)));
  EXPECT_TRUE(target_type_nullable.isSafelyCoercibleFrom(TypeFactory::GetType(kNullType, true)));

  // Other types are not coercible to strings.
  for (const TypeID original_type_id
       : {kInt, kLong, kFloat, kDouble, kDatetime, kDatetimeInterval, kYearMonthInterval}) {
    const Type &original_type = TypeFactory::TypeRequiresLengthParameter(original_type_id)
                                ? TypeFactory::GetType(original_type_id, 10, false)
                                : TypeFactory::GetType(original_type_id, false);
    const Type &original_type_nullable = original_type.getNullableVersion();

    EXPECT_FALSE(target_type.isSafelyCoercibleFrom(original_type));
    EXPECT_FALSE(target_type.isSafelyCoercibleFrom(original_type_nullable));
    EXPECT_FALSE(target_type_nullable.isSafelyCoercibleFrom(original_type));
    EXPECT_FALSE(target_type_nullable.isSafelyCoercibleFrom(original_type_nullable));
  }
}

/**
 * @brief Check coercion between two types represented as in-line literal
 *        values.
 *
 * @param TargetType The Type to coerce to.
 * @param OriginalType The Type to coerce from.
 * @param literal The underlying literal value for the original value from
 *        OriginalType.
 **/
template <typename TargetType, typename OriginalType>
void CheckLiteralCoerce(const typename OriginalType::cpptype literal) {
  TypedValue value(literal);
  TypedValue coerced = TargetType::InstanceNonNullable().coerceValue(
      value,
      OriginalType::InstanceNonNullable());
  EXPECT_EQ(static_cast<typename TargetType::cpptype>(literal),
            coerced.getLiteral<typename TargetType::cpptype>());
}

/**
 * @brief Check coercion of a NULL value.
 *
 * @param TargetType The Type to coerce to.
 * @param OriginalType The Type to coerce from.
 **/
template <typename TargetType, typename OriginalType>
void CheckNullLiteralCoerce() {
  TypedValue value(OriginalType::InstanceNullable().makeNullValue());
  TypedValue coerced = TargetType::InstanceNullable().coerceValue(
      value,
      OriginalType::InstanceNullable());
  EXPECT_TRUE(coerced.isNull());
}

/**
 * @brief Run a battery of tests coercing values of various different numeric
 *        Types to a specified target Type.
 *
 * @param TargetType The Type to coerce to.
 **/
template <typename TargetType>
void CheckNumericCoerce() {
  CheckLiteralCoerce<TargetType, IntType>(0);
  CheckLiteralCoerce<TargetType, IntType>(123);
  CheckLiteralCoerce<TargetType, IntType>(-123);
  CheckLiteralCoerce<TargetType, IntType>(std::numeric_limits<int>::max());
  CheckLiteralCoerce<TargetType, IntType>(std::numeric_limits<int>::min());
  CheckNullLiteralCoerce<TargetType, IntType>();

  CheckLiteralCoerce<TargetType, LongType>(INT64_C(0));
  CheckLiteralCoerce<TargetType, LongType>(INT64_C(123));
  CheckLiteralCoerce<TargetType, LongType>(INT64_C(-123));
  CheckLiteralCoerce<TargetType, LongType>(std::numeric_limits<std::int64_t>::max());
  CheckLiteralCoerce<TargetType, LongType>(std::numeric_limits<std::int64_t>::min());
  CheckNullLiteralCoerce<TargetType, LongType>();

  CheckLiteralCoerce<TargetType, FloatType>(0.0f);
  CheckLiteralCoerce<TargetType, FloatType>(123.45f);
  CheckLiteralCoerce<TargetType, FloatType>(-123.45f);
  CheckLiteralCoerce<TargetType, FloatType>(std::numeric_limits<float>::max());
  CheckLiteralCoerce<TargetType, FloatType>(std::numeric_limits<float>::min());
  CheckNullLiteralCoerce<TargetType, FloatType>();

  CheckLiteralCoerce<TargetType, DoubleType>(0.0);
  CheckLiteralCoerce<TargetType, DoubleType>(123.45);
  CheckLiteralCoerce<TargetType, DoubleType>(-123.45);
  CheckLiteralCoerce<TargetType, DoubleType>(std::numeric_limits<double>::max());
  CheckLiteralCoerce<TargetType, DoubleType>(std::numeric_limits<double>::min());
  CheckNullLiteralCoerce<TargetType, DoubleType>();

  CheckNullLiteralCoerce<TargetType, NullType>();
}

/**
 * @brief Check coercion of CHAR literal values WITHOUT a null-terminator to
 *        a string Type.
 *
 * @param type_id The ID of the string type to coerce to.
 * @param literal The underlying string literal value to test coercion on.
 **/
void CheckStringCoerceFromCharWithoutNullTerminator(const TypeID type_id,
                                                    const std::string &literal) {
  const Type &target_type = TypeFactory::GetType(type_id, literal.size(), false);
  const Type &shorter_target_type = TypeFactory::GetType(type_id, literal.size() - 1, false);
  const Type &longer_target_type = TypeFactory::GetType(type_id, literal.size() + 10, false);

  const Type &char_type = TypeFactory::GetType(kChar, literal.size(), false);

  TypedValue char_value = char_type.makeValue(literal.c_str(), literal.size());
  TypedValue nonref_char_value = char_value;
  nonref_char_value.ensureNotReference();

  // Test coercion from a CHAR with exactly the right length (original value
  // has no null-terminator).
  TypedValue coerced = target_type.coerceValue(char_value, char_type);
  EXPECT_EQ(type_id, coerced.getTypeID());
  if (type_id == kChar) {
    // Still a reference to the same data.
    EXPECT_FALSE(coerced.ownsOutOfLineData());
    EXPECT_EQ(literal.size(), coerced.getDataSize());
    EXPECT_EQ(static_cast<const void*>(literal.c_str()), coerced.getDataPtr());
  } else {
    // Coercing to VARCHAR requires appending a null-terminator.
    EXPECT_TRUE(coerced.ownsOutOfLineData());
    EXPECT_EQ(literal.size() + 1, coerced.getDataSize());
    EXPECT_STREQ(literal.c_str(), static_cast<const char*>(coerced.getDataPtr()));
  }

  // Coercing from a literal makes a copy.
  TypedValue coerced_from_nonref = target_type.coerceValue(nonref_char_value, char_type);
  EXPECT_EQ(type_id, coerced_from_nonref.getTypeID());
  EXPECT_TRUE(coerced_from_nonref.ownsOutOfLineData());
  if (type_id == kChar) {
    ASSERT_EQ(literal.size(), coerced_from_nonref.getDataSize());
    EXPECT_EQ(0, std::strncmp(literal.c_str(),
                              static_cast<const char*>(coerced_from_nonref.getDataPtr()),
                              literal.size()));
  } else {
    EXPECT_EQ(literal.size() + 1, coerced_from_nonref.getDataSize());
    EXPECT_STREQ(literal.c_str(), static_cast<const char*>(coerced_from_nonref.getDataPtr()));
  }

  // Test shortening from a CHAR.
  TypedValue shortened = shorter_target_type.coerceValue(char_value, char_type);
  EXPECT_EQ(type_id, shortened.getTypeID());
  if (type_id == kChar) {
    // Still a reference to a shorter slice of the same data.
    EXPECT_FALSE(shortened.ownsOutOfLineData());
    EXPECT_EQ(literal.size() - 1, shortened.getDataSize());
    EXPECT_EQ(static_cast<const void*>(literal.c_str()), shortened.getDataPtr());
  } else {
    // End of string is trimmed and a null-terminator is appended.
    EXPECT_TRUE(shortened.ownsOutOfLineData());
    ASSERT_EQ(literal.size(), shortened.getDataSize());
    EXPECT_EQ(0, std::strncmp(literal.c_str(),
                              static_cast<const char*>(shortened.getDataPtr()),
                              literal.size() - 1));
    EXPECT_EQ('\0', static_cast<const char*>(shortened.getDataPtr())[literal.size() - 1]);
  }

  // Again, coercing from a literal makes a copy.
  TypedValue shortened_from_nonref
      = shorter_target_type.coerceValue(nonref_char_value, char_type);
  EXPECT_EQ(type_id, shortened_from_nonref.getTypeID());
  EXPECT_TRUE(shortened_from_nonref.ownsOutOfLineData());
  if (type_id == kChar) {
    ASSERT_EQ(literal.size() - 1, shortened_from_nonref.getDataSize());
    EXPECT_EQ(0, std::strncmp(literal.c_str(),
                              static_cast<const char*>(shortened_from_nonref.getDataPtr()),
                              literal.size() - 1));
  } else {
    EXPECT_EQ(literal.size(), shortened_from_nonref.getDataSize());
    EXPECT_EQ(0, std::strncmp(literal.c_str(),
                              static_cast<const char*>(shortened_from_nonref.getDataPtr()),
                              literal.size() - 1));
    EXPECT_EQ('\0', static_cast<const char*>(
                        shortened_from_nonref.getDataPtr())[literal.size() - 1]);
  }

  // Test lengthening from a CHAR. This always needs to make a copy and append
  // a null-terminator.
  TypedValue lengthened
      = longer_target_type.coerceValue(char_value, char_type);
  EXPECT_EQ(type_id, lengthened.getTypeID());
  EXPECT_TRUE(lengthened.ownsOutOfLineData());
  EXPECT_EQ(literal.size() + 1, lengthened.getDataSize());
  EXPECT_STREQ(literal.c_str(), static_cast<const char*>(lengthened.getDataPtr()));

  // Same deal if the original is itself a literal.
  TypedValue lengthened_from_nonref
      = longer_target_type.coerceValue(nonref_char_value, char_type);
  EXPECT_EQ(type_id, lengthened_from_nonref.getTypeID());
  EXPECT_TRUE(lengthened_from_nonref.ownsOutOfLineData());
  EXPECT_EQ(literal.size() + 1, lengthened_from_nonref.getDataSize());
  EXPECT_STREQ(literal.c_str(),
               static_cast<const char*>(lengthened_from_nonref.getDataPtr()));
}

/**
 * @brief Check coercion of string literal values WITH a null-terminator to a
 *        string Type.
 *
 * @param type_id The ID of the string type to coerce to.
 * @param original_type_id The ID of the string type to coerce from.
 * @param literal The underlying string literal value to test coercion on.
 **/
void CheckStringCoerceWithNullTerminator(const TypeID type_id,
                                         const TypeID original_type_id,
                                         const std::string &literal) {
  const Type &target_type = TypeFactory::GetType(type_id, literal.size(), false);
  const Type &shorter_target_type = TypeFactory::GetType(type_id, literal.size() - 1, false);
  const Type &longer_target_type = TypeFactory::GetType(type_id, literal.size() + 10, false);

  const Type &original_type = TypeFactory::GetType(
      original_type_id,
      original_type_id == kChar ? literal.size() + 1
                                : literal.size(),
      false);
  TypedValue value = original_type.makeValue(literal.c_str(), literal.size() + 1);
  TypedValue nonref_value = value;
  nonref_value.ensureNotReference();

  // Should just be a reference to the original data.
  TypedValue coerced = target_type.coerceValue(value, original_type);
  EXPECT_EQ(type_id, coerced.getTypeID());
  EXPECT_FALSE(coerced.ownsOutOfLineData());
  EXPECT_EQ(type_id == kChar ? literal.size() : literal.size() + 1,
            coerced.getDataSize());
  EXPECT_EQ(static_cast<const void*>(literal.c_str()), coerced.getDataPtr());

  // Coercing from a literal makes a copy.
  TypedValue coerced_from_nonref  = target_type.coerceValue(nonref_value, original_type);
  EXPECT_EQ(type_id, coerced_from_nonref.getTypeID());
  EXPECT_TRUE(coerced_from_nonref.ownsOutOfLineData());
  if (type_id == kChar) {
    ASSERT_EQ(literal.size(), coerced_from_nonref.getDataSize());
    EXPECT_EQ(0, std::strncmp(literal.c_str(),
                              static_cast<const char*>(coerced_from_nonref.getDataPtr()),
                              literal.size()));
  } else {
    EXPECT_EQ(literal.size() + 1, coerced_from_nonref.getDataSize());
    EXPECT_STREQ(literal.c_str(), static_cast<const char*>(coerced_from_nonref.getDataPtr()));
  }

  // Test shortening.
  TypedValue shortened = shorter_target_type.coerceValue(value, original_type);
  EXPECT_EQ(type_id, shortened.getTypeID());
  if (type_id == kChar) {
    // Still a reference to a shorter slice of the same data.
    EXPECT_FALSE(shortened.ownsOutOfLineData());
    EXPECT_EQ(literal.size() - 1, shortened.getDataSize());
    EXPECT_EQ(static_cast<const void*>(literal.c_str()), shortened.getDataPtr());
  } else {
    // Shortening to a VARCHAR requires making a shorter copy with a
    // null-terminator.
    EXPECT_TRUE(shortened.ownsOutOfLineData());
    ASSERT_EQ(literal.size(), shortened.getDataSize());
    EXPECT_EQ(0, std::strncmp(literal.c_str(),
                              static_cast<const char*>(shortened.getDataPtr()),
                              literal.size() - 1));
    EXPECT_EQ('\0', static_cast<const char*>(shortened.getDataPtr())[literal.size() - 1]);
  }

  // Again, coercing from a literal makes a copy.
  TypedValue shortened_from_nonref = shorter_target_type.coerceValue(nonref_value, original_type);
  EXPECT_EQ(type_id, shortened_from_nonref.getTypeID());
  EXPECT_TRUE(shortened_from_nonref.ownsOutOfLineData());
  if (type_id == kChar) {
    ASSERT_EQ(literal.size() - 1, shortened_from_nonref.getDataSize());
    EXPECT_EQ(0, std::strncmp(literal.c_str(),
                              static_cast<const char*>(shortened_from_nonref.getDataPtr()),
                              literal.size() - 1));
  } else {
    EXPECT_EQ(literal.size(), shortened_from_nonref.getDataSize());
    EXPECT_EQ(0, std::strncmp(literal.c_str(),
                              static_cast<const char*>(shortened_from_nonref.getDataPtr()),
                              literal.size() - 1));
    EXPECT_EQ('\0', static_cast<const char*>(
                        shortened_from_nonref.getDataPtr())[literal.size() - 1]);
  }

  // Lengthening from a null-terminated original should stay as a reference to
  // the original data.
  TypedValue lengthened = longer_target_type.coerceValue(value, original_type);
  EXPECT_EQ(type_id, lengthened.getTypeID());
  EXPECT_FALSE(lengthened.ownsOutOfLineData());
  EXPECT_EQ(literal.size() + 1, lengthened.getDataSize());
  EXPECT_EQ(static_cast<const void*>(literal.c_str()), lengthened.getDataPtr());

  // If the original is a literal, a copy is made.
  TypedValue lengthened_from_nonref = longer_target_type.coerceValue(nonref_value, original_type);
  EXPECT_EQ(type_id, lengthened_from_nonref.getTypeID());
  EXPECT_TRUE(lengthened_from_nonref.ownsOutOfLineData());
  EXPECT_EQ(literal.size() + 1, lengthened_from_nonref.getDataSize());
  EXPECT_STREQ(literal.c_str(),
               static_cast<const char*>(lengthened_from_nonref.getDataPtr()));
}

/** @} */

}  // namespace type_test
}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_TESTS_TYPE_TEST_COMMON_HPP_
