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
#include <cstring>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/aggregation/AggregateFunction.hpp"
#include "expressions/aggregation/AggregateFunctionFactory.hpp"
#include "expressions/aggregation/AggregationHandle.hpp"
#include "expressions/aggregation/AggregationHandleMax.hpp"
#include "expressions/aggregation/AggregationID.hpp"
#include "storage/HashTableBase.hpp"
#include "storage/StorageManager.hpp"
#include "types/CharType.hpp"
#include "types/DatetimeIntervalType.hpp"
#include "types/DatetimeLit.hpp"
#include "types/DatetimeType.hpp"
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

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#endif

#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"

#include "gtest/gtest.h"

namespace quickstep {

class AggregationHandleMaxTest : public ::testing::Test {
 protected:
  static const int kNumSamples = 200;
  static const int kIterations = 5;

  // Helper method that calls AggregationHandleMax::iterateUnaryInl() to
  // aggregate 'value' into '*state'.
  void iterateHandle(AggregationState *state, const TypedValue &value) {
    static_cast<const AggregationHandleMax&>(*aggregation_handle_max_).iterateUnaryInl(
        static_cast<AggregationStateMax*>(state),
        value);
  }

  void initializeHandle(const Type &type) {
    aggregation_handle_max_.reset(
        AggregateFunctionFactory::Get(AggregationID::kMax).createHandle(
            std::vector<const Type*>(1, &type)));
    aggregation_handle_max_state_.reset(
        aggregation_handle_max_->createInitialState());
  }

  static bool ApplyToTypesTest(TypeID typeID) {
    const Type &type = (typeID == kChar || typeID == kVarChar) ?
        TypeFactory::GetType(typeID, static_cast<std::size_t>(10)) :
        TypeFactory::GetType(typeID);

    return AggregateFunctionFactory::Get(AggregationID::kMax).canApplyToTypes(
        std::vector<const Type*>(1, &type));
  }

  static bool ResultTypeForArgumentTypeTest(TypeID input_type_id,
                                            TypeID output_type_id) {
    const Type *result_type
        = AggregateFunctionFactory::Get(AggregationID::kMax).resultTypeForArgumentTypes(
            std::vector<const Type*>(1, &TypeFactory::GetType(input_type_id)));
    return (result_type->getTypeID() == output_type_id);
  }

  template <typename CppType>
  static void CheckMaxValue(
      CppType expected,
      const AggregationHandle &handle,
      const AggregationState &state) {
    EXPECT_EQ(expected, handle.finalize(state).getLiteral<CppType>());
  }

  static void CheckMaxString(
      const std::string &expected,
      const AggregationHandle &handle,
      const AggregationState &state) {
    TypedValue value = handle.finalize(state);

    ASSERT_EQ(expected.length(), value.getAsciiStringLength());
    EXPECT_EQ(0, std::strncmp(expected.c_str(),
                              static_cast<const char*>(value.getDataPtr()),
                              value.getAsciiStringLength()));
  }

  // Static templated method to initialize data types.
  template <typename CppType>
  void SetDataType(int value, CppType *data) {
    *data = value;
  }

  template <typename GenericType>
  void checkAggregationMaxGeneric() {
    const GenericType &type = GenericType::Instance(true);
    initializeHandle(type);
    EXPECT_TRUE(aggregation_handle_max_->finalize(*aggregation_handle_max_state_).isNull());

    typename GenericType::cpptype val;
    typename GenericType::cpptype max;
    SetDataType(0, &max);

    iterateHandle(aggregation_handle_max_state_.get(), type.makeNullValue());
    for (int i = 0; i < kIterations; ++i) {
      for (int j = 0; j < kNumSamples; ++j) {
        if (type.getTypeID() == kInt || type.getTypeID() == kLong) {
          SetDataType(i * kNumSamples + j - 10, &val);
        } else {
          SetDataType(static_cast<float>(i * kNumSamples + j - 10)/10, &val);
        }
        iterateHandle(aggregation_handle_max_state_.get(), type.makeValue(&val));
        if (max < val) {
          max = val;
        }
      }
    }
    iterateHandle(aggregation_handle_max_state_.get(), type.makeNullValue());
    CheckMaxValue<typename GenericType::cpptype>(max, *aggregation_handle_max_, *aggregation_handle_max_state_);

    // Test mergeStates().
    std::unique_ptr<AggregationState> merge_state(
        aggregation_handle_max_->createInitialState());
    aggregation_handle_max_->mergeStates(*merge_state,
                                         aggregation_handle_max_state_.get());

    iterateHandle(merge_state.get(), type.makeNullValue());
    for (int i = 0; i < kIterations; ++i) {
      for (int j = kNumSamples - 1; j >= 0; --j) {
        if (type.getTypeID() == kInt || type.getTypeID() == kLong) {
          SetDataType(i * kNumSamples + j - 20, &val);
        } else {
          SetDataType(static_cast<float>(i * kNumSamples + j - 20)/10, &val);
        }
        iterateHandle(merge_state.get(), type.makeValue(&val));
        if (max < val) {
          max = val;
        }
      }
    }
    aggregation_handle_max_->mergeStates(*merge_state,
                                         aggregation_handle_max_state_.get());
    CheckMaxValue<typename GenericType::cpptype>(
        max,
        *aggregation_handle_max_,
        *aggregation_handle_max_state_);
  }

  template <typename GenericType>
  ColumnVector *createColumnVectorGeneric(const Type &type, typename GenericType::cpptype *max) {
    NativeColumnVector *column = new NativeColumnVector(type, kIterations * kNumSamples + 3);

    typename GenericType::cpptype val;
    SetDataType(0, max);

    column->appendTypedValue(type.makeNullValue());
    for (int i = 0; i < kIterations; ++i) {
      for (int j = 0; j < kNumSamples; ++j) {
        if (type.getTypeID() == kInt || type.getTypeID() == kLong) {
          SetDataType(i * kNumSamples + j - 10, &val);
        } else {
          SetDataType(static_cast<float>(i * kNumSamples + j - 10)/10, &val);
        }
        column->appendTypedValue(type.makeValue(&val));
        if (*max < val) {
          *max = val;
        }
      }
      // One NULL in the middle.
      if (i == kIterations/2) {
        column->appendTypedValue(type.makeNullValue());
      }
    }
    column->appendTypedValue(type.makeNullValue());

    return column;
  }

  template <typename GenericType>
  void checkAggregationMaxGenericColumnVector() {
    const GenericType &type = GenericType::Instance(true);
    initializeHandle(type);
    EXPECT_TRUE(aggregation_handle_max_->finalize(*aggregation_handle_max_state_).isNull());

    typename GenericType::cpptype max;
    std::vector<std::unique_ptr<ColumnVector>> column_vectors;
    column_vectors.emplace_back(createColumnVectorGeneric<GenericType>(type, &max));

    std::unique_ptr<AggregationState> cv_state(
        aggregation_handle_max_->accumulateColumnVectors(column_vectors));

    // Test the state generated directly by accumulateColumnVectors(), and also
    // test after merging back.
    CheckMaxValue<typename GenericType::cpptype>(
        max,
        *aggregation_handle_max_,
        *cv_state);

    aggregation_handle_max_->mergeStates(*cv_state, aggregation_handle_max_state_.get());
    CheckMaxValue<typename GenericType::cpptype>(
        max,
        *aggregation_handle_max_,
        *aggregation_handle_max_state_);
  }

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  template <typename GenericType>
  void checkAggregationMaxGenericValueAccessor() {
    const GenericType &type = GenericType::Instance(true);
    initializeHandle(type);
    EXPECT_TRUE(aggregation_handle_max_->finalize(*aggregation_handle_max_state_).isNull());

    std::unique_ptr<ColumnVectorsValueAccessor> accessor(new ColumnVectorsValueAccessor());

    typename GenericType::cpptype max;
    accessor->addColumn(createColumnVectorGeneric<GenericType>(type, &max));

    std::unique_ptr<AggregationState> va_state(
        aggregation_handle_max_->accumulateValueAccessor(accessor.get(),
                                                         std::vector<attribute_id>(1, 0)));

    // Test the state generated directly by accumulateValueAccessor(), and also
    // test after merging back.
    CheckMaxValue<typename GenericType::cpptype>(
        max,
        *aggregation_handle_max_,
        *va_state);

    aggregation_handle_max_->mergeStates(*va_state, aggregation_handle_max_state_.get());
    CheckMaxValue<typename GenericType::cpptype>(
        max,
        *aggregation_handle_max_,
        *aggregation_handle_max_state_);
  }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

  template <typename StringType>
  void checkAggregationMaxString() {
    const StringType &type = StringType::Instance(10, true);
    initializeHandle(type);
    EXPECT_TRUE(aggregation_handle_max_->finalize(*aggregation_handle_max_state_).isNull());

    std::unique_ptr<UncheckedComparator> fast_comparator_;
    fast_comparator_.reset(ComparisonFactory::GetComparison(ComparisonID::kGreater)
                           .makeUncheckedComparatorForTypes(type, type));
    std::string string_literal;
    std::string max = "";
    int val;
    iterateHandle(aggregation_handle_max_state_.get(), type.makeNullValue());
    for (int i = 0; i < kIterations; ++i) {
      for (int j = 0; j < kNumSamples; ++j) {
        val = i * kNumSamples + j;
        std::ostringstream oss;
        oss << "max" << val;
        string_literal = oss.str();

        iterateHandle(
            aggregation_handle_max_state_.get(),
            type.makeValue(string_literal.c_str(),
                           string_literal.length() + 1).ensureNotReference());
        if (fast_comparator_->compareDataPtrs(string_literal.c_str(), max.c_str())) {
          max = string_literal;
        }
      }
    }
    iterateHandle(aggregation_handle_max_state_.get(), type.makeNullValue());
    CheckMaxString(max, *aggregation_handle_max_, *aggregation_handle_max_state_);

    // Test mergeStates().
    std::unique_ptr<AggregationState> merge_state(
        aggregation_handle_max_->createInitialState());
    aggregation_handle_max_->mergeStates(*merge_state,
                                         aggregation_handle_max_state_.get());

    iterateHandle(merge_state.get(), type.makeNullValue());
    for (int i = 0; i < kIterations; ++i) {
      for (int j = kNumSamples - 1; j >= 0; --j) {
        val = i * kNumSamples + j;
        std::ostringstream oss;
        oss << "test" << val;
        string_literal = oss.str();

        iterateHandle(
            merge_state.get(),
            type.makeValue(string_literal.c_str(),
                           string_literal.length() + 1).ensureNotReference());
        if (fast_comparator_->compareDataPtrs(string_literal.c_str(), max.c_str())) {
          max = string_literal;
        }
      }
    }
    aggregation_handle_max_->mergeStates(*merge_state,
                                         aggregation_handle_max_state_.get());
    CheckMaxString(max, *aggregation_handle_max_, *aggregation_handle_max_state_);
  }

  template <typename ColumnVectorType>
  ColumnVector *createColumnVectorString(const Type &type, std::string *max) {
    ColumnVectorType *column = new ColumnVectorType(type, kIterations * kNumSamples + 3);
    std::unique_ptr<UncheckedComparator> fast_comparator_;
    fast_comparator_.reset(ComparisonFactory::GetComparison(ComparisonID::kGreater)
                           .makeUncheckedComparatorForTypes(type, type));
    std::string string_literal;
    *max = "";
    int val;
    column->appendTypedValue(type.makeNullValue());
    for (int i = 0; i < kIterations; ++i) {
      for (int j = 0; j < kNumSamples; ++j) {
        val = i * kNumSamples + j;
        std::ostringstream oss;
        oss << "max" << val;
        string_literal = oss.str();

        column->appendTypedValue(type.makeValue(string_literal.c_str(), string_literal.length() + 1)
            .ensureNotReference());
        if (fast_comparator_->compareDataPtrs(string_literal.c_str(), max->c_str())) {
          *max = string_literal;
        }
      }
      // One NULL in the middle.
      if (i == kIterations/2) {
        column->appendTypedValue(type.makeNullValue());
      }
    }
    column->appendTypedValue(type.makeNullValue());

    return column;
  }

  template <typename StringType, typename ColumnVectorType>
  void checkAggregationMaxStringColumnVector() {
    const StringType &type = StringType::Instance(10, true);
    initializeHandle(type);
    EXPECT_TRUE(aggregation_handle_max_->finalize(*aggregation_handle_max_state_).isNull());

    std::string max;
    std::vector<std::unique_ptr<ColumnVector>> column_vectors;
    column_vectors.emplace_back(createColumnVectorString<ColumnVectorType>(type, &max));

    std::unique_ptr<AggregationState> cv_state(
        aggregation_handle_max_->accumulateColumnVectors(column_vectors));

    // Test the state generated directly by accumulateColumnVectors(), and also
    // test after merging back.
    CheckMaxString(max,
                   *aggregation_handle_max_,
                   *cv_state);

    aggregation_handle_max_->mergeStates(*cv_state, aggregation_handle_max_state_.get());
    CheckMaxString(max,
                   *aggregation_handle_max_,
                   *aggregation_handle_max_state_);
  }

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  template <typename StringType, typename ColumnVectorType>
  void checkAggregationMaxStringValueAccessor() {
    const StringType &type = StringType::Instance(10, true);
    initializeHandle(type);
    EXPECT_TRUE(aggregation_handle_max_->finalize(*aggregation_handle_max_state_).isNull());

    std::string max;
    std::unique_ptr<ColumnVectorsValueAccessor> accessor(new ColumnVectorsValueAccessor());
    accessor->addColumn(createColumnVectorString<ColumnVectorType>(type, &max));

    std::unique_ptr<AggregationState> va_state(
        aggregation_handle_max_->accumulateValueAccessor(accessor.get(),
                                                         std::vector<attribute_id>(1, 0)));

    // Test the state generated directly by accumulateValueAccessor(), and also
    // test after merging back.
    CheckMaxString(max,
                   *aggregation_handle_max_,
                   *va_state);

    aggregation_handle_max_->mergeStates(*va_state, aggregation_handle_max_state_.get());
    CheckMaxString(max,
                   *aggregation_handle_max_,
                   *aggregation_handle_max_state_);
  }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

  std::unique_ptr<AggregationHandle> aggregation_handle_max_;
  std::unique_ptr<AggregationState> aggregation_handle_max_state_;
  std::unique_ptr<StorageManager> storage_manager_;
};

template <>
void AggregationHandleMaxTest::CheckMaxValue<float>(
    float val,
    const AggregationHandle &handle,
    const AggregationState &state) {
  EXPECT_FLOAT_EQ(val, handle.finalize(state).getLiteral<float>());
}

template <>
void AggregationHandleMaxTest::CheckMaxValue<double>(
    double val,
    const AggregationHandle &handle,
    const AggregationState &state) {
  EXPECT_DOUBLE_EQ(val, handle.finalize(state).getLiteral<double>());
}

template <>
void AggregationHandleMaxTest::SetDataType<DatetimeLit>(int value, DatetimeLit *data) {
  data->ticks = value;
}

template <>
void AggregationHandleMaxTest::SetDataType<DatetimeIntervalLit>(int value, DatetimeIntervalLit *data) {
  data->interval_ticks = value;
}

template <>
void AggregationHandleMaxTest::SetDataType<YearMonthIntervalLit>(int value, YearMonthIntervalLit *data) {
  data->months = value;
}

typedef AggregationHandleMaxTest AggregationHandleMaxDeathTest;

TEST_F(AggregationHandleMaxTest, IntTypeTest) {
  checkAggregationMaxGeneric<IntType>();
}

TEST_F(AggregationHandleMaxTest, LongTypeTest) {
  checkAggregationMaxGeneric<LongType>();
}

TEST_F(AggregationHandleMaxTest, FloatTypeTest) {
  checkAggregationMaxGeneric<FloatType>();
}

TEST_F(AggregationHandleMaxTest, DoubleTypeTest) {
  checkAggregationMaxGeneric<DoubleType>();
}

TEST_F(AggregationHandleMaxTest, DatetimeType) {
  checkAggregationMaxGeneric<DatetimeType>();
}

TEST_F(AggregationHandleMaxTest, DatetimeIntervalType) {
  checkAggregationMaxGeneric<DatetimeIntervalType>();
}

TEST_F(AggregationHandleMaxTest, YearMonthIntervalType) {
  checkAggregationMaxGeneric<YearMonthIntervalType>();
}

TEST_F(AggregationHandleMaxTest, CharTypeTest) {
  checkAggregationMaxString<CharType>();
}

TEST_F(AggregationHandleMaxTest, VarCharTypeTest) {
  checkAggregationMaxString<VarCharType>();
}

TEST_F(AggregationHandleMaxTest, IntTypeColumnVectorTest) {
  checkAggregationMaxGenericColumnVector<IntType>();
}

TEST_F(AggregationHandleMaxTest, LongTypeColumnVectorTest) {
  checkAggregationMaxGenericColumnVector<LongType>();
}

TEST_F(AggregationHandleMaxTest, FloatTypeColumnVectorTest) {
  checkAggregationMaxGenericColumnVector<FloatType>();
}

TEST_F(AggregationHandleMaxTest, DoubleTypeColumnVectorTest) {
  checkAggregationMaxGenericColumnVector<DoubleType>();
}

TEST_F(AggregationHandleMaxTest, DatetimeTypeColumnVectorTest) {
  checkAggregationMaxGenericColumnVector<DatetimeType>();
}

TEST_F(AggregationHandleMaxTest, DatetimeIntervalTypeColumnVectorTest) {
  checkAggregationMaxGenericColumnVector<DatetimeIntervalType>();
}

TEST_F(AggregationHandleMaxTest, YearMonthIntervalTypeColumnVectorTest) {
  checkAggregationMaxGenericColumnVector<YearMonthIntervalType>();
}

TEST_F(AggregationHandleMaxTest, CharTypeColumnVectorTest) {
  checkAggregationMaxStringColumnVector<CharType, NativeColumnVector>();
}

TEST_F(AggregationHandleMaxTest, VarCharColumnVectorTypeTest) {
  checkAggregationMaxStringColumnVector<VarCharType, IndirectColumnVector>();
}

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
TEST_F(AggregationHandleMaxTest, IntTypeValueAccessorTest) {
  checkAggregationMaxGenericValueAccessor<IntType>();
}

TEST_F(AggregationHandleMaxTest, LongTypeValueAccessorTest) {
  checkAggregationMaxGenericValueAccessor<LongType>();
}

TEST_F(AggregationHandleMaxTest, FloatTypeValueAccessorTest) {
  checkAggregationMaxGenericValueAccessor<FloatType>();
}

TEST_F(AggregationHandleMaxTest, DoubleTypeValueAccessorTest) {
  checkAggregationMaxGenericValueAccessor<DoubleType>();
}

TEST_F(AggregationHandleMaxTest, DatetimeTypeValueAccessorTest) {
  checkAggregationMaxGenericValueAccessor<DatetimeType>();
}

TEST_F(AggregationHandleMaxTest, DatetimeIntervalTypeValueAccessorTest) {
  checkAggregationMaxGenericValueAccessor<DatetimeIntervalType>();
}

TEST_F(AggregationHandleMaxTest, YearMonthIntervalTypeValueAccessorTest) {
  checkAggregationMaxGenericValueAccessor<YearMonthIntervalType>();
}

TEST_F(AggregationHandleMaxTest, CharTypeValueAccessorTest) {
  checkAggregationMaxStringValueAccessor<CharType, NativeColumnVector>();
}

TEST_F(AggregationHandleMaxTest, VarCharValueAccessorTypeTest) {
  checkAggregationMaxStringValueAccessor<VarCharType, IndirectColumnVector>();
}
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

#ifdef QUICKSTEP_DEBUG
TEST_F(AggregationHandleMaxDeathTest, WrongTypeTest) {
  const Type &int_non_null_type = IntType::Instance(false);
  const Type &long_type = LongType::Instance(true);
  const Type &double_type = DoubleType::Instance(true);
  const Type &float_type = FloatType::Instance(true);
  const Type &char_type = CharType::Instance(true, 10);
  const Type &varchar_type = VarCharType::Instance(true, 10);

  initializeHandle(IntType::Instance(true));
  int int_val = 0;
  std::int64_t long_val = 0;
  double double_val = 0;
  float float_val = 0;

  // Passes.
  iterateHandle(aggregation_handle_max_state_.get(), int_non_null_type.makeValue(&int_val));

  EXPECT_DEATH(iterateHandle(aggregation_handle_max_state_.get(), long_type.makeValue(&long_val)), "");
  EXPECT_DEATH(iterateHandle(aggregation_handle_max_state_.get(), double_type.makeValue(&double_val)), "");
  EXPECT_DEATH(iterateHandle(aggregation_handle_max_state_.get(), float_type.makeValue(&float_val)), "");
  EXPECT_DEATH(iterateHandle(aggregation_handle_max_state_.get(), char_type.makeValue("asdf", 5)), "");
  EXPECT_DEATH(iterateHandle(aggregation_handle_max_state_.get(), varchar_type.makeValue("asdf", 5)), "");

  // Test mergeStates() with incorrectly typed handles.
  std::unique_ptr<AggregationHandle> aggregation_handle_max_long(
      AggregateFunctionFactory::Get(AggregationID::kMax).createHandle(
          std::vector<const Type*>(1, &long_type)));
  std::unique_ptr<AggregationState> aggregation_state_max_merge_long(
      aggregation_handle_max_long->createInitialState());
  static_cast<const AggregationHandleMax&>(*aggregation_handle_max_long).iterateUnaryInl(
      static_cast<AggregationStateMax*>(aggregation_state_max_merge_long.get()),
      long_type.makeValue(&long_val));
  EXPECT_DEATH(aggregation_handle_max_->mergeStates(*aggregation_state_max_merge_long,
                                                    aggregation_handle_max_state_.get()),
               "");

  std::unique_ptr<AggregationHandle> aggregation_handle_max_double(
      AggregateFunctionFactory::Get(AggregationID::kMax).createHandle(
          std::vector<const Type*>(1, &double_type)));
  std::unique_ptr<AggregationState> aggregation_state_max_merge_double(
      aggregation_handle_max_double->createInitialState());
  static_cast<const AggregationHandleMax&>(*aggregation_handle_max_double).iterateUnaryInl(
      static_cast<AggregationStateMax*>(aggregation_state_max_merge_double.get()),
      double_type.makeValue(&double_val));
  EXPECT_DEATH(aggregation_handle_max_->mergeStates(*aggregation_state_max_merge_double,
                                                    aggregation_handle_max_state_.get()),
               "");

  std::unique_ptr<AggregationHandle> aggregation_handle_max_float(
      AggregateFunctionFactory::Get(AggregationID::kMax).createHandle(
          std::vector<const Type*>(1, &float_type)));
  std::unique_ptr<AggregationState> aggregation_state_max_merge_float(
      aggregation_handle_max_float->createInitialState());
  static_cast<const AggregationHandleMax&>(*aggregation_handle_max_float).iterateUnaryInl(
      static_cast<AggregationStateMax*>(aggregation_state_max_merge_float.get()),
      float_type.makeValue(&float_val));
  EXPECT_DEATH(aggregation_handle_max_->mergeStates(*aggregation_state_max_merge_float,
                                                    aggregation_handle_max_state_.get()),
               "");
}
#endif

TEST_F(AggregationHandleMaxTest, canApplyToTypeTest) {
  EXPECT_TRUE(ApplyToTypesTest(kInt));
  EXPECT_TRUE(ApplyToTypesTest(kLong));
  EXPECT_TRUE(ApplyToTypesTest(kFloat));
  EXPECT_TRUE(ApplyToTypesTest(kDouble));
  EXPECT_TRUE(ApplyToTypesTest(kChar));
  EXPECT_TRUE(ApplyToTypesTest(kVarChar));
}

TEST_F(AggregationHandleMaxTest, ResultTypeForArgumentTypeTest) {
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kInt, kInt));
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kLong, kLong));
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kFloat, kFloat));
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kDouble, kDouble));
}

TEST_F(AggregationHandleMaxTest, GroupByTableMergeTest) {
  const Type &int_non_null_type = IntType::Instance(false);
  initializeHandle(int_non_null_type);
  storage_manager_.reset(new StorageManager("./test_max_data"));
  std::unique_ptr<AggregationStateHashTableBase> source_hash_table(
      aggregation_handle_max_->createGroupByHashTable(
          HashTableImplType::kSimpleScalarSeparateChaining,
          std::vector<const Type *>(1, &int_non_null_type),
          10,
          storage_manager_.get()));
  std::unique_ptr<AggregationStateHashTableBase> destination_hash_table(
      aggregation_handle_max_->createGroupByHashTable(
          HashTableImplType::kSimpleScalarSeparateChaining,
          std::vector<const Type *>(1, &int_non_null_type),
          10,
          storage_manager_.get()));

  AggregationStateHashTable<AggregationStateMax> *destination_hash_table_derived =
      static_cast<AggregationStateHashTable<AggregationStateMax> *>(
          destination_hash_table.get());

  AggregationStateHashTable<AggregationStateMax> *source_hash_table_derived =
      static_cast<AggregationStateHashTable<AggregationStateMax> *>(
          source_hash_table.get());

  AggregationHandleMax *aggregation_handle_max_derived =
      static_cast<AggregationHandleMax *>(aggregation_handle_max_.get());
  // We create three keys: first is present in both the hash tables, second key
  // is present only in the source hash table while the third key is present
  // the destination hash table only.
  std::vector<TypedValue> common_key;
  common_key.emplace_back(0);
  std::vector<TypedValue> exclusive_source_key, exclusive_destination_key;
  exclusive_source_key.emplace_back(1);
  exclusive_destination_key.emplace_back(2);

  const int common_key_source_max = 3000;
  TypedValue common_key_source_max_val(common_key_source_max);

  const int common_key_destination_max = 4000;
  TypedValue common_key_destination_max_val(common_key_destination_max);

  const int exclusive_key_source_max = 100;
  TypedValue exclusive_key_source_max_val(exclusive_key_source_max);

  const int exclusive_key_destination_max = 200;
  TypedValue exclusive_key_destination_max_val(exclusive_key_destination_max);

  std::unique_ptr<AggregationStateMax> common_key_source_state(
      static_cast<AggregationStateMax *>(
          aggregation_handle_max_->createInitialState()));
  std::unique_ptr<AggregationStateMax> common_key_destination_state(
      static_cast<AggregationStateMax *>(
          aggregation_handle_max_->createInitialState()));
  std::unique_ptr<AggregationStateMax> exclusive_key_source_state(
      static_cast<AggregationStateMax *>(
          aggregation_handle_max_->createInitialState()));
  std::unique_ptr<AggregationStateMax> exclusive_key_destination_state(
      static_cast<AggregationStateMax *>(
          aggregation_handle_max_->createInitialState()));

  // Create max value states for keys.
  aggregation_handle_max_derived->iterateUnaryInl(common_key_source_state.get(),
                                                  common_key_source_max_val);
  int actual_val = aggregation_handle_max_->finalize(*common_key_source_state)
                       .getLiteral<int>();
  EXPECT_EQ(common_key_source_max_val.getLiteral<int>(), actual_val);

  aggregation_handle_max_derived->iterateUnaryInl(
      common_key_destination_state.get(), common_key_destination_max_val);
  actual_val = aggregation_handle_max_->finalize(*common_key_destination_state)
                   .getLiteral<int>();
  EXPECT_EQ(common_key_destination_max_val.getLiteral<int>(), actual_val);

  aggregation_handle_max_derived->iterateUnaryInl(
      exclusive_key_destination_state.get(), exclusive_key_destination_max_val);
  actual_val =
      aggregation_handle_max_->finalize(*exclusive_key_destination_state)
          .getLiteral<int>();
  EXPECT_EQ(exclusive_key_destination_max_val.getLiteral<int>(), actual_val);

  aggregation_handle_max_derived->iterateUnaryInl(
      exclusive_key_source_state.get(), exclusive_key_source_max_val);
  actual_val = aggregation_handle_max_->finalize(*exclusive_key_source_state)
                   .getLiteral<int>();
  EXPECT_EQ(exclusive_key_source_max_val.getLiteral<int>(), actual_val);

  // Add the key-state pairs to the hash tables.
  source_hash_table_derived->putCompositeKey(common_key,
                                             *common_key_source_state);
  destination_hash_table_derived->putCompositeKey(
      common_key, *common_key_destination_state);
  source_hash_table_derived->putCompositeKey(exclusive_source_key,
                                             *exclusive_key_source_state);
  destination_hash_table_derived->putCompositeKey(
      exclusive_destination_key, *exclusive_key_destination_state);

  EXPECT_EQ(2u, destination_hash_table_derived->numEntries());
  EXPECT_EQ(2u, source_hash_table_derived->numEntries());

  aggregation_handle_max_->mergeGroupByHashTables(*source_hash_table,
                                                  destination_hash_table.get());

  EXPECT_EQ(3u, destination_hash_table_derived->numEntries());

  CheckMaxValue<int>(
      common_key_destination_max_val.getLiteral<int>(),
      *aggregation_handle_max_derived,
      *(destination_hash_table_derived->getSingleCompositeKey(common_key)));
  CheckMaxValue<int>(exclusive_key_destination_max_val.getLiteral<int>(),
                     *aggregation_handle_max_derived,
                     *(destination_hash_table_derived->getSingleCompositeKey(
                         exclusive_destination_key)));
  CheckMaxValue<int>(exclusive_key_source_max_val.getLiteral<int>(),
                     *aggregation_handle_max_derived,
                     *(source_hash_table_derived->getSingleCompositeKey(
                         exclusive_source_key)));
}

}  // namespace quickstep
