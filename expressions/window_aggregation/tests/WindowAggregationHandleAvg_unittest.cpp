/**
 *   Licensed to the Apache Software Foundation (ASF) under one
 *   or more contributor license agreements.  See the NOTICE file
 *   distributed with this work for additional information
 *   regarding copyright ownership.  The ASF licenses this file
 *   to you under the Apache License, Version 2.0 (the
 *   "License"); you may not use this file except in compliance
 *   with the License.  You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 *   Unless required by applicable law or agreed to in writing,
 *   software distributed under the License is distributed on an
 *   "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *   KIND, either express or implied.  See the License for the
 *   specific language governing permissions and limitations
 *   under the License.
 *   limitations under the License.
 **/

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/window_aggregation/WindowAggregateFunction.hpp"
#include "expressions/window_aggregation/WindowAggregateFunctionFactory.hpp"
#include "expressions/window_aggregation/WindowAggregationHandle.hpp"
#include "expressions/window_aggregation/WindowAggregationHandleAvg.hpp"
#include "expressions/window_aggregation/WindowAggregationID.hpp"
#include "storage/ValueAccessor.hpp"
#include "types/CharType.hpp"
#include "types/DateOperatorOverloads.hpp"
#include "types/DatetimeIntervalType.hpp"
#include "types/DoubleType.hpp"
#include "types/FloatType.hpp"
#include "types/IntType.hpp"
#include "types/IntervalLit.hpp"
#include "types/LongType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/VarCharType.hpp"
#include "types/YearMonthIntervalType.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"

#include "gtest/gtest.h"

namespace quickstep {

namespace {

  constexpr int kNumTuples = 100;
  constexpr int kNumTuplesPerPartition = 8;
  constexpr int kNullInterval = 25;
  constexpr int kNumPreceding = 2;
  constexpr int kNumFollowing = 2;

}  // namespace

// Attribute value could be null if set true.
class WindowAggregationHandleAvgTest : public::testing::Test {
 protected:
  // Handle initialization.
  void initializeHandle(const Type &argument_type) {
    const WindowAggregateFunction &function =
        WindowAggregateFunctionFactory::Get(WindowAggregationID::kAvg);
    std::vector<const Type*> partition_key_types(1, &TypeFactory::GetType(kInt, false));
    handle_avg_.reset(function.createHandle(std::vector<const Type*>(1, &argument_type),
                                            std::move(partition_key_types)));
  }

  // Test canApplyToTypes().
  static bool CanApplyToTypesTest(TypeID typeID) {
    const Type &type = (typeID == kChar || typeID == kVarChar) ?
        TypeFactory::GetType(typeID, static_cast<std::size_t>(10)) :
        TypeFactory::GetType(typeID);

    return WindowAggregateFunctionFactory::Get(WindowAggregationID::kAvg).canApplyToTypes(
        std::vector<const Type*>(1, &type));
  }

  // Test resultTypeForArgumentTypes().
  static bool ResultTypeForArgumentTypesTest(TypeID input_type_id,
                                             TypeID output_type_id) {
    const Type *result_type
        = WindowAggregateFunctionFactory::Get(WindowAggregationID::kAvg).resultTypeForArgumentTypes(
            std::vector<const Type*>(1, &TypeFactory::GetType(input_type_id)));
    return (result_type->getTypeID() == output_type_id);
  }

  template <typename CppType>
  static void CheckAvgValues(
      const std::vector<CppType*> &expected,
      const ColumnVector *actual) {
    EXPECT_TRUE(actual->isNative());
    const NativeColumnVector *native = static_cast<const NativeColumnVector*>(actual);

    EXPECT_EQ(expected.size(), native->size());
    for (std::size_t i = 0; i < expected.size(); ++i) {
      if (expected[i] == nullptr) {
        EXPECT_TRUE(native->getTypedValue(i).isNull());
      } else {
        EXPECT_EQ(*expected[i], native->getTypedValue(i).getLiteral<CppType>());
      }
    }
  }

  // Static templated methods for set a meaningful value to data types.
  template <typename CppType>
  static void SetDataType(int value, CppType *data) {
    *data = value;
  }

  template <typename GenericType, typename OutputType = DoubleType>
  void checkWindowAggregationAvgGeneric() {
    const GenericType &type = GenericType::Instance(true);
    initializeHandle(type);

    // Create argument, partition key and cpptype vectors.
    std::vector<typename GenericType::cpptype*> argument_cpp_vector;
    argument_cpp_vector.reserve(kNumTuples);
    ColumnVector *argument_type_vector =
        createArgumentGeneric<GenericType>(&argument_cpp_vector);
    NativeColumnVector *partition_key_vector =
        new NativeColumnVector(IntType::InstanceNonNullable(), kNumTuples + 2);

    for (int i = 0; i < kNumTuples; ++i) {
      partition_key_vector->appendTypedValue(TypedValue(i / kNumTuplesPerPartition));
    }

    // Create tuple ValueAccessor.
    ColumnVectorsValueAccessor *tuple_accessor = new ColumnVectorsValueAccessor();
    tuple_accessor->addColumn(partition_key_vector);
    tuple_accessor->addColumn(argument_type_vector);

    // Test UNBOUNDED PRECEDING AND CURRENT ROW.
    checkAccumulate<GenericType, OutputType>(tuple_accessor,
                                             argument_type_vector,
                                             argument_cpp_vector);
    // Test kNumPreceding PRECEDING AND kNumFollowing FOLLOWING.
    checkSlidingWindow<GenericType, OutputType>(tuple_accessor,
                                                argument_type_vector,
                                                argument_cpp_vector);
  }

  template <typename GenericType>
  ColumnVector *createArgumentGeneric(
      std::vector<typename GenericType::cpptype*> *argument_cpp_vector) {
    const GenericType &type = GenericType::Instance(true);
    NativeColumnVector *column = new NativeColumnVector(type, kNumTuples);

    for (int i = 0; i < kNumTuples; ++i) {
      // Insert a NULL every kNullInterval tuples.
      if (i % kNullInterval == 0) {
        argument_cpp_vector->push_back(nullptr);
        column->appendTypedValue(type.makeNullValue());
        continue;
      }

      typename GenericType::cpptype *val = new typename GenericType::cpptype;

      if (type.getTypeID() == kInt || type.getTypeID() == kLong) {
        SetDataType(i - 10, val);
      } else {
        SetDataType(static_cast<float>(i - 10) / 10, val);
      }

      column->appendTypedValue(type.makeValue(val));
      argument_cpp_vector->push_back(val);
    }

    return column;
  }

  template <typename GenericType, typename OutputType>
  void checkAccumulate(ColumnVectorsValueAccessor *tuple_accessor,
                       ColumnVector *argument_type_vector,
                       const std::vector<typename GenericType::cpptype*> &argument_cpp_vector) {
    std::vector<ColumnVector*> arguments;
    arguments.push_back(argument_type_vector);
    // The partition key index is 0.
    std::vector<attribute_id> partition_key(1, 0);

    ColumnVector *result =
        handle_avg_->calculate(tuple_accessor,
                               std::move(arguments),
                               partition_key,
                               true  /* is_row */,
                               -1  /* num_preceding: UNBOUNDED PRECEDING */,
                               0  /* num_following: CURRENT ROW */);

    // Get the cpptype result.
    std::vector<typename OutputType::cpptype*> result_cpp_vector;
    typename GenericType::cpptype sum;
    int count;
    for (std::size_t i = 0; i < argument_cpp_vector.size(); ++i) {
      // Start of new partition
      if (i % kNumTuplesPerPartition == 0) {
        SetDataType(0, &sum);
        count = 0;
      }

      typename GenericType::cpptype *value = argument_cpp_vector[i];
      if (value != nullptr) {
        sum += *value;
        count++;
      }

      if (count == 0) {
        result_cpp_vector.push_back(nullptr);
      } else {
        typename OutputType::cpptype *result_cpp_value =
            new typename OutputType::cpptype;
        *result_cpp_value = static_cast<typename OutputType::cpptype>(sum) / count;
        result_cpp_vector.push_back(result_cpp_value);
      }
    }

    CheckAvgValues(result_cpp_vector, result);
  }

  template <typename GenericType, typename OutputType>
  void checkSlidingWindow(ColumnVectorsValueAccessor *tuple_accessor,
                          ColumnVector *argument_type_vector,
                          const std::vector<typename GenericType::cpptype*> &argument_cpp_vector) {
    std::vector<ColumnVector*> arguments;
    arguments.push_back(argument_type_vector);
    // The partition key index is 0.
    std::vector<attribute_id> partition_key(1, 0);

    ColumnVector *result =
        handle_avg_->calculate(tuple_accessor,
                               std::move(arguments),
                               partition_key,
                               true  /* is_row */,
                               kNumPreceding,
                               kNumFollowing);

    // Get the cpptype result.
    // For each value, calculate all surrounding values in the window.
    std::vector<typename OutputType::cpptype*> result_cpp_vector;

    for (std::size_t i = 0; i < argument_cpp_vector.size(); ++i) {
      typename GenericType::cpptype sum;
      SetDataType(0, &sum);
      int count = 0;

      if (argument_cpp_vector[i] != nullptr) {
        sum += *argument_cpp_vector[i];
        count++;
      }

      for (std::size_t precede = 1; precede <= kNumPreceding; ++precede) {
        // Not the same partition.
        if (i / kNumTuplesPerPartition != (i - precede) / kNumTuplesPerPartition ||
            i < precede) {
          break;
        }

        if (argument_cpp_vector[i - precede] != nullptr) {
          sum += *argument_cpp_vector[i - precede];
          count++;
        }
      }

      for (int follow = 1; follow <= kNumPreceding; ++follow) {
        // Not the same partition.
        if (i / kNumTuplesPerPartition != (i + follow) / kNumTuplesPerPartition ||
            i + follow >= kNumTuples) {
          break;
        }

        if (argument_cpp_vector[i + follow] != nullptr) {
          sum += *argument_cpp_vector[i + follow];
          count++;
        }
      }

      if (count == 0) {
        result_cpp_vector.push_back(nullptr);
      } else {
        typename OutputType::cpptype *result_cpp_value =
            new typename OutputType::cpptype;
        *result_cpp_value = static_cast<typename OutputType::cpptype>(sum) / count;
        result_cpp_vector.push_back(result_cpp_value);
      }
    }

    CheckAvgValues(result_cpp_vector, result);
  }

  std::unique_ptr<WindowAggregationHandle> handle_avg_;
};

template <>
void WindowAggregationHandleAvgTest::CheckAvgValues<double>(
    const std::vector<double*> &expected,
    const ColumnVector *actual) {
  EXPECT_TRUE(actual->isNative());
  const NativeColumnVector *native = static_cast<const NativeColumnVector*>(actual);

  EXPECT_EQ(expected.size(), native->size());
  for (std::size_t i = 0; i < expected.size(); ++i) {
    if (expected[i] == nullptr) {
      EXPECT_TRUE(native->getTypedValue(i).isNull());
    } else {
      EXPECT_EQ(*expected[i], native->getTypedValue(i).getLiteral<double>());
    }
  }
}

template <>
void WindowAggregationHandleAvgTest::SetDataType<DatetimeIntervalLit>(
    int value, DatetimeIntervalLit *data) {
  data->interval_ticks = value;
}

template <>
void WindowAggregationHandleAvgTest::SetDataType<YearMonthIntervalLit>(
    int value, YearMonthIntervalLit *data) {
  data->months = value;
}

typedef WindowAggregationHandleAvgTest WindowAggregationHandleAvgDeathTest;

TEST_F(WindowAggregationHandleAvgTest, IntTypeTest) {
  checkWindowAggregationAvgGeneric<IntType>();
}

TEST_F(WindowAggregationHandleAvgTest, LongTypeTest) {
  checkWindowAggregationAvgGeneric<LongType>();
}

TEST_F(WindowAggregationHandleAvgTest, FloatTypeTest) {
  checkWindowAggregationAvgGeneric<FloatType>();
}

TEST_F(WindowAggregationHandleAvgTest, DoubleTypeTest) {
  checkWindowAggregationAvgGeneric<DoubleType>();
}

TEST_F(WindowAggregationHandleAvgTest, DatetimeIntervalTypeTest) {
  checkWindowAggregationAvgGeneric<DatetimeIntervalType, DatetimeIntervalType>();
}

TEST_F(WindowAggregationHandleAvgTest, YearMonthIntervalTypeTest) {
  checkWindowAggregationAvgGeneric<YearMonthIntervalType, YearMonthIntervalType>();
}

#ifdef QUICKSTEP_DEBUG
TEST_F(WindowAggregationHandleAvgDeathTest, CharTypeTest) {
  const Type &type = CharType::Instance(true, 10);
  EXPECT_DEATH(initializeHandle(type), "");
}

TEST_F(WindowAggregationHandleAvgDeathTest, VarTypeTest) {
  const Type &type = VarCharType::Instance(true, 10);
  EXPECT_DEATH(initializeHandle(type), "");
}
#endif

TEST_F(WindowAggregationHandleAvgDeathTest, canApplyToTypeTest) {
  EXPECT_TRUE(CanApplyToTypesTest(kInt));
  EXPECT_TRUE(CanApplyToTypesTest(kLong));
  EXPECT_TRUE(CanApplyToTypesTest(kFloat));
  EXPECT_TRUE(CanApplyToTypesTest(kDouble));
  EXPECT_FALSE(CanApplyToTypesTest(kChar));
  EXPECT_FALSE(CanApplyToTypesTest(kVarChar));
  EXPECT_FALSE(CanApplyToTypesTest(kDatetime));
  EXPECT_TRUE(CanApplyToTypesTest(kDatetimeInterval));
  EXPECT_TRUE(CanApplyToTypesTest(kYearMonthInterval));
}

TEST_F(WindowAggregationHandleAvgDeathTest, ResultTypeForArgumentTypeTest) {
  EXPECT_TRUE(ResultTypeForArgumentTypesTest(kInt, kDouble));
  EXPECT_TRUE(ResultTypeForArgumentTypesTest(kLong, kDouble));
  EXPECT_TRUE(ResultTypeForArgumentTypesTest(kFloat, kDouble));
  EXPECT_TRUE(ResultTypeForArgumentTypesTest(kDouble, kDouble));
  EXPECT_TRUE(ResultTypeForArgumentTypesTest(kDatetimeInterval, kDatetimeInterval));
  EXPECT_TRUE(ResultTypeForArgumentTypesTest(kYearMonthInterval, kYearMonthInterval));
}

}  // namespace quickstep
