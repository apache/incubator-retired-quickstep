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

#ifndef QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_TESTS_BINARY_OPERATION_TEST_UTIL_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_TESTS_BINARY_OPERATION_TEST_UTIL_HPP_

#include <algorithm>
#include <initializer_list>

#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "utility/Macros.hpp"

#include "gtest/gtest.h"

namespace quickstep {

/**
 * @brief All-static utility class that provides reusable helper methods for
 *        testing some interfaces of BinaryOperation.
 **/
class BinaryOperationTestUtil {
 public:
  /**
   * @brief Run tests on BinaryOperation::partialTypeSignatureIsPlausible()
   *        with a known result type and unknown (NULL) argument types.
   *
   * @param op An implementation of BinaryOperation to test.
   * @param plausible_result_type_ids A list of TypeIDs indicating which Types
   *        are considered plausible for op to return.
   **/
  static void CheckPlausibilityWithKnownResultAndUnknownArguments(
      const BinaryOperation &op,
      const std::initializer_list<TypeID> &plausible_result_type_ids) {
    for (const TypeID result_type_id
         : {kInt, kLong, kFloat, kDouble, kChar, kVarChar, kDatetime,
            kDatetimeInterval, kYearMonthInterval}) {
      const Type *result_type = TypeFactory::TypeRequiresLengthParameter(result_type_id)
                                ? &TypeFactory::GetType(result_type_id, 10, false)
                                : &TypeFactory::GetType(result_type_id, false);
      const Type *result_type_nullable = &result_type->getNullableVersion();

      if (std::find(plausible_result_type_ids.begin(),
                    plausible_result_type_ids.end(),
                    result_type_id)
          == plausible_result_type_ids.end()) {
        EXPECT_FALSE(op.partialTypeSignatureIsPlausible(result_type_nullable, nullptr, nullptr));
      } else {
        EXPECT_TRUE(op.partialTypeSignatureIsPlausible(result_type_nullable, nullptr, nullptr));
      }

      // The non-nullable version of the result type is never considered
      // plausible, because the unknown argument types represent unresolved
      // NULL values.
      EXPECT_FALSE(op.partialTypeSignatureIsPlausible(result_type, nullptr, nullptr));
    }
  }

  /**
   * @brief Run tests on BinaryOperation::partialTypeSignatureIsPlausible()
   *        with an unknown result type and a single known argument type.
   *
   * @param op An implementation of BinaryOperation to test.
   * @param plausible_argument_type_ids A list of TypeIDs indicating which
   *        Types are considered plausible arguments (left or right) to op.
   * @param check_left_argument If true, check the plausibility of Types in the
   *        left argument position.
   * @param check_right_argument If true, check the plausibility of Types in
   *        the right argument position.
   **/
  static void CheckPlausibilityWithUnknownResultAndSingleKnownArgument(
      const BinaryOperation &op,
      const std::initializer_list<TypeID> &plausible_argument_type_ids,
      const bool check_left_argument,
      const bool check_right_argument) {
    for (const TypeID argument_type_id
         : {kInt, kLong, kFloat, kDouble, kChar, kVarChar, kDate, kDatetime,
            kDatetimeInterval, kYearMonthInterval}) {
      const Type *argument_type = TypeFactory::TypeRequiresLengthParameter(argument_type_id)
                                  ? &TypeFactory::GetType(argument_type_id, 10, false)
                                  : &TypeFactory::GetType(argument_type_id, false);
      const Type *argument_type_nullable = &argument_type->getNullableVersion();

      if (std::find(plausible_argument_type_ids.begin(),
                    plausible_argument_type_ids.end(),
                    argument_type_id)
          == plausible_argument_type_ids.end()) {
        if (check_left_argument) {
          EXPECT_FALSE(op.partialTypeSignatureIsPlausible(
              nullptr, argument_type, nullptr));
          EXPECT_FALSE(op.partialTypeSignatureIsPlausible(
              nullptr, argument_type_nullable, nullptr));
        }
        if (check_right_argument) {
          EXPECT_FALSE(op.partialTypeSignatureIsPlausible(
              nullptr, nullptr, argument_type));
          EXPECT_FALSE(op.partialTypeSignatureIsPlausible(
              nullptr, nullptr, argument_type_nullable));
        }
      } else {
        if (check_left_argument) {
          EXPECT_TRUE(op.partialTypeSignatureIsPlausible(
              nullptr, argument_type, nullptr));
          EXPECT_TRUE(op.partialTypeSignatureIsPlausible(
              nullptr, argument_type_nullable, nullptr));
        }
        if (check_right_argument) {
          EXPECT_TRUE(op.partialTypeSignatureIsPlausible(
              nullptr, nullptr, argument_type));
          EXPECT_TRUE(op.partialTypeSignatureIsPlausible(
              nullptr, nullptr, argument_type_nullable));
        }
      }
    }
  }

  /**
   * @brief Run tests on BinaryOperation::partialTypeSignatureIsPlausible()
   *        with an known result type and a single known argument type.
   *
   * @param op An implementation of BinaryOperation to test.
   * @param result_type_id A TypeID indicating the particular known result type
   *        to check.
   * @param plausible_argument_type_ids A list of TypeIDs indicating which
   *        Types are considered plausible arguments (left or right) to op with
   *        the result Type specified by result_type_id.
   * @param check_left_argument If true, check the plausibility of Types in the
   *        left argument position.
   * @param check_right_argument If true, check the plausibility of Types in
   *        the right argument position.
   **/
  static void CheckPlausibilityWithKnownResultAndSingleArgument(
      const BinaryOperation &op,
      const TypeID result_type_id,
      const std::initializer_list<TypeID> &plausible_argument_type_ids,
      const bool check_left_argument,
      const bool check_right_argument) {
    const Type *result_type = TypeFactory::TypeRequiresLengthParameter(result_type_id)
                              ? &TypeFactory::GetType(result_type_id, 10, false)
                              : &TypeFactory::GetType(result_type_id, false);
    const Type *result_type_nullable = &result_type->getNullableVersion();

    for (const TypeID argument_type_id
        : {kInt, kLong, kFloat, kDouble, kChar, kVarChar, kDate, kDatetime,
            kDatetimeInterval, kYearMonthInterval}) {
      const Type *argument_type = TypeFactory::TypeRequiresLengthParameter(argument_type_id)
                                  ? &TypeFactory::GetType(argument_type_id, 10, false)
                                  : &TypeFactory::GetType(argument_type_id, false);
      const Type *argument_type_nullable = &argument_type->getNullableVersion();

      if (std::find(plausible_argument_type_ids.begin(),
                    plausible_argument_type_ids.end(),
                    argument_type_id)
          == plausible_argument_type_ids.end()) {
        if (check_left_argument) {
          EXPECT_FALSE(op.partialTypeSignatureIsPlausible(
              result_type_nullable,
              argument_type,
              nullptr));
          EXPECT_FALSE(op.partialTypeSignatureIsPlausible(
              result_type_nullable,
              argument_type_nullable,
              nullptr));
        }
        if (check_right_argument) {
          EXPECT_FALSE(op.partialTypeSignatureIsPlausible(
              result_type_nullable,
              nullptr,
              argument_type));
          EXPECT_FALSE(op.partialTypeSignatureIsPlausible(
              result_type_nullable,
              nullptr,
              argument_type_nullable));
        }
      } else {
        if (check_left_argument) {
          EXPECT_TRUE(op.partialTypeSignatureIsPlausible(
              result_type_nullable,
              argument_type,
              nullptr));
          EXPECT_TRUE(op.partialTypeSignatureIsPlausible(
              result_type_nullable,
              argument_type_nullable,
              nullptr));
        }
        if (check_right_argument) {
          EXPECT_TRUE(op.partialTypeSignatureIsPlausible(
              result_type_nullable,
              nullptr,
              argument_type));
          EXPECT_TRUE(op.partialTypeSignatureIsPlausible(
              result_type_nullable,
              nullptr,
              argument_type_nullable));
        }
      }

      // No non-nullable result type is plausible when one of the argument types
      // is unknown.
      if (check_left_argument) {
        EXPECT_FALSE(op.partialTypeSignatureIsPlausible(
            result_type,
            argument_type,
            nullptr));
        EXPECT_FALSE(op.partialTypeSignatureIsPlausible(
            result_type,
            argument_type_nullable,
            nullptr));
      }
      if (check_right_argument) {
        EXPECT_FALSE(op.partialTypeSignatureIsPlausible(
            result_type,
            nullptr,
            argument_type));
        EXPECT_FALSE(op.partialTypeSignatureIsPlausible(
            result_type,
            nullptr,
            argument_type_nullable));
      }
    }
  }

 private:
  // Undefined default constructor. Class is all-static and should not be
  // instantiated.
  BinaryOperationTestUtil();

  DISALLOW_COPY_AND_ASSIGN(BinaryOperationTestUtil);
};

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_TESTS_BINARY_OPERATION_TEST_UTIL_HPP_
