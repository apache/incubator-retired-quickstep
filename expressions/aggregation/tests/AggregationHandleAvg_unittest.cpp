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

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/aggregation/AggregateFunction.hpp"
#include "expressions/aggregation/AggregateFunctionFactory.hpp"
#include "expressions/aggregation/AggregationHandle.hpp"
#include "expressions/aggregation/AggregationHandleAvg.hpp"
#include "expressions/aggregation/AggregationID.hpp"
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

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#endif

#include "gtest/gtest.h"

namespace quickstep {

class AggregationHandleAvgTest : public::testing::Test {
 protected:
  static const int kNumSamples = 100;

  // Helper method that calls AggregationHandleAvg::iterateUnaryInl() to
  // aggregate 'value' into '*state'.
  void iterateHandle(AggregationState *state, const TypedValue &value) {
    static_cast<const AggregationHandleAvg&>(*aggregation_handle_avg_).iterateUnaryInl(
        static_cast<AggregationStateAvg*>(state),
        value);
  }

  void initializeHandle(const Type &type) {
    aggregation_handle_avg_.reset(
        AggregateFunctionFactory::Get(AggregationID::kAvg).createHandle(
            std::vector<const Type*>(1, &type)));
    aggregation_handle_avg_state_.reset(
        aggregation_handle_avg_->createInitialState());
  }

  static bool ApplyToTypesTest(TypeID typeID) {
    const Type &type = (typeID == kChar || typeID == kVarChar) ?
        TypeFactory::GetType(typeID, static_cast<std::size_t>(10)) :
        TypeFactory::GetType(typeID);

    return AggregateFunctionFactory::Get(AggregationID::kAvg).canApplyToTypes(
        std::vector<const Type*>(1, &type));
  }

  static bool ResultTypeForArgumentTypeTest(TypeID input_type_id,
                                            TypeID output_type_id) {
    const Type *result_type
        = AggregateFunctionFactory::Get(AggregationID::kAvg).resultTypeForArgumentTypes(
            std::vector<const Type*>(1, &TypeFactory::GetType(input_type_id)));
    return (result_type->getTypeID() == output_type_id);
  }

  template <typename CppType>
  static void CheckAvgValue(
      CppType expected,
      const AggregationHandle &handle,
      const AggregationState &state) {
    EXPECT_EQ(expected, handle.finalize(state).getLiteral<CppType>());
  }

  // Static templated method for set a meaningful value to data types.
  template <typename CppType>
  static void SetDataType(int value, CppType *data) {
    *data = value;
  }

  template <typename GenericType, typename OutputType = DoubleType>
  void checkAggregationAvgGeneric() {
    const GenericType &type = GenericType::Instance(true);
    initializeHandle(type);
    EXPECT_TRUE(aggregation_handle_avg_->finalize(*aggregation_handle_avg_state_).isNull());

    typename GenericType::cpptype val;
    typename GenericType::cpptype sum;
    SetDataType(0, &sum);

    iterateHandle(aggregation_handle_avg_state_.get(), type.makeNullValue());
    for (int i = 0; i < kNumSamples; ++i) {
      if (type.getTypeID() == kInt || type.getTypeID() == kLong) {
        SetDataType(i - 10, &val);
      } else {
        SetDataType(static_cast<float>(i - 10)/10, &val);
      }
      iterateHandle(aggregation_handle_avg_state_.get(), type.makeValue(&val));
      sum += val;
    }
    iterateHandle(aggregation_handle_avg_state_.get(), type.makeNullValue());
    CheckAvgValue<typename OutputType::cpptype>(static_cast<typename OutputType::cpptype>(sum) / kNumSamples,
                                                *aggregation_handle_avg_,
                                                *aggregation_handle_avg_state_);

    // Test mergeStates().
    std::unique_ptr<AggregationState> merge_state(
        aggregation_handle_avg_->createInitialState());
    aggregation_handle_avg_->mergeStates(*merge_state,
                                         aggregation_handle_avg_state_.get());

    iterateHandle(merge_state.get(), type.makeNullValue());
    for (int i = 0; i < kNumSamples; ++i) {
      if (type.getTypeID() == kInt || type.getTypeID() == kLong) {
        SetDataType(i - 10, &val);
      } else {
        SetDataType(static_cast<float>(i - 10)/10, &val);
      }
      iterateHandle(merge_state.get(), type.makeValue(&val));
      sum += val;
    }

    aggregation_handle_avg_->mergeStates(*merge_state,
                                         aggregation_handle_avg_state_.get());
    CheckAvgValue<typename OutputType::cpptype>(
        static_cast<typename OutputType::cpptype>(sum) / (2 * kNumSamples),
        *aggregation_handle_avg_,
        *aggregation_handle_avg_state_);
  }

  template <typename GenericType>
  ColumnVector *createColumnVectorGeneric(const Type &type, typename GenericType::cpptype *sum) {
    NativeColumnVector *column = new NativeColumnVector(type, kNumSamples + 3);

    typename GenericType::cpptype val;
    SetDataType(0, sum);

    column->appendTypedValue(type.makeNullValue());
    for (int i = 0; i < kNumSamples; ++i) {
      if (type.getTypeID() == kInt || type.getTypeID() == kLong) {
        SetDataType(i - 10, &val);
      } else {
        SetDataType(static_cast<float>(i - 10)/10, &val);
      }
      column->appendTypedValue(type.makeValue(&val));
      *sum += val;
      // One NULL in the middle.
      if (i == kNumSamples/2) {
        column->appendTypedValue(type.makeNullValue());
      }
    }
    column->appendTypedValue(type.makeNullValue());

    return column;
  }

  template <typename GenericType, typename OutputType = DoubleType>
  void checkAggregationAvgGenericColumnVector() {
    const GenericType &type = GenericType::Instance(true);
    initializeHandle(type);
    EXPECT_TRUE(aggregation_handle_avg_->finalize(*aggregation_handle_avg_state_).isNull());

    typename GenericType::cpptype sum;
    SetDataType(0, &sum);
    std::vector<std::unique_ptr<ColumnVector>> column_vectors;
    column_vectors.emplace_back(createColumnVectorGeneric<GenericType>(type, &sum));

    std::unique_ptr<AggregationState> cv_state(
        aggregation_handle_avg_->accumulateColumnVectors(column_vectors));

    // Test the state generated directly by accumulateColumnVectors(), and also
    // test after merging back.
    CheckAvgValue<typename OutputType::cpptype>(
        static_cast<typename OutputType::cpptype>(sum) / kNumSamples,
        *aggregation_handle_avg_,
        *cv_state);

    aggregation_handle_avg_->mergeStates(*cv_state, aggregation_handle_avg_state_.get());
    CheckAvgValue<typename OutputType::cpptype>(
        static_cast<typename OutputType::cpptype>(sum) / kNumSamples,
        *aggregation_handle_avg_,
        *aggregation_handle_avg_state_);
  }

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  template <typename GenericType, typename OutputType = DoubleType>
  void checkAggregationAvgGenericValueAccessor() {
    const GenericType &type = GenericType::Instance(true);
    initializeHandle(type);
    EXPECT_TRUE(aggregation_handle_avg_->finalize(*aggregation_handle_avg_state_).isNull());

    typename GenericType::cpptype sum;
    SetDataType(0, &sum);
    std::unique_ptr<ColumnVectorsValueAccessor> accessor(new ColumnVectorsValueAccessor());
    accessor->addColumn(createColumnVectorGeneric<GenericType>(type, &sum));

    std::unique_ptr<AggregationState> va_state(
        aggregation_handle_avg_->accumulateValueAccessor(accessor.get(),
                                                         std::vector<attribute_id>(1, 0)));

    // Test the state generated directly by accumulateValueAccessor(), and also
    // test after merging back.
    CheckAvgValue<typename OutputType::cpptype>(
        static_cast<typename OutputType::cpptype>(sum) / kNumSamples,
        *aggregation_handle_avg_,
        *va_state);

    aggregation_handle_avg_->mergeStates(*va_state, aggregation_handle_avg_state_.get());
    CheckAvgValue<typename OutputType::cpptype>(
        static_cast<typename OutputType::cpptype>(sum) / kNumSamples,
        *aggregation_handle_avg_,
        *aggregation_handle_avg_state_);
  }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

  std::unique_ptr<AggregationHandle> aggregation_handle_avg_;
  std::unique_ptr<AggregationState> aggregation_handle_avg_state_;
};

const int AggregationHandleAvgTest::kNumSamples;

template <>
void AggregationHandleAvgTest::CheckAvgValue<double>(
    double expected,
    const AggregationHandle &handle,
    const AggregationState &state) {
  EXPECT_DOUBLE_EQ(expected, handle.finalize(state).getLiteral<double>());
}

template <>
void AggregationHandleAvgTest::SetDataType<DatetimeIntervalLit>(int value, DatetimeIntervalLit *data) {
  data->interval_ticks = value;
}

template <>
void AggregationHandleAvgTest::SetDataType<YearMonthIntervalLit>(int value, YearMonthIntervalLit *data) {
  data->months = value;
}

typedef AggregationHandleAvgTest AggregationHandleAvgDeathTest;

TEST_F(AggregationHandleAvgTest, IntTypeTest) {
  checkAggregationAvgGeneric<IntType>();
}

TEST_F(AggregationHandleAvgTest, LongTypeTest) {
  checkAggregationAvgGeneric<LongType>();
}

TEST_F(AggregationHandleAvgTest, FloatTypeTest) {
  checkAggregationAvgGeneric<FloatType>();
}

TEST_F(AggregationHandleAvgTest, DoubleTypeTest) {
  checkAggregationAvgGeneric<DoubleType>();
}

TEST_F(AggregationHandleAvgTest, DatetimeIntervalTypeTest) {
  checkAggregationAvgGeneric<DatetimeIntervalType, DatetimeIntervalType>();
}

TEST_F(AggregationHandleAvgTest, YearMonthIntervalTypeTest) {
  checkAggregationAvgGeneric<YearMonthIntervalType, YearMonthIntervalType>();
}

TEST_F(AggregationHandleAvgTest, IntTypeColumnVectorTest) {
  checkAggregationAvgGenericColumnVector<IntType>();
}

TEST_F(AggregationHandleAvgTest, LongTypeColumnVectorTest) {
  checkAggregationAvgGenericColumnVector<LongType>();
}

TEST_F(AggregationHandleAvgTest, FloatTypeColumnVectorTest) {
  checkAggregationAvgGenericColumnVector<FloatType>();
}

TEST_F(AggregationHandleAvgTest, DoubleTypeColumnVectorTest) {
  checkAggregationAvgGenericColumnVector<DoubleType>();
}

TEST_F(AggregationHandleAvgTest, DatetimeIntervalTypeColumnVectorTest) {
  checkAggregationAvgGenericColumnVector<DatetimeIntervalType, DatetimeIntervalType>();
}

TEST_F(AggregationHandleAvgTest, YearMonthIntervalTypeColumnVectorTest) {
  checkAggregationAvgGenericColumnVector<YearMonthIntervalType, YearMonthIntervalType>();
}

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
TEST_F(AggregationHandleAvgTest, IntTypeValueAccessorTest) {
  checkAggregationAvgGenericValueAccessor<IntType>();
}

TEST_F(AggregationHandleAvgTest, LongTypeValueAccessorTest) {
  checkAggregationAvgGenericValueAccessor<LongType>();
}

TEST_F(AggregationHandleAvgTest, FloatTypeValueAccessorTest) {
  checkAggregationAvgGenericValueAccessor<FloatType>();
}

TEST_F(AggregationHandleAvgTest, DoubleTypeValueAccessorTest) {
  checkAggregationAvgGenericValueAccessor<DoubleType>();
}

TEST_F(AggregationHandleAvgTest, DatetimeIntervalTypeValueAccessorTest) {
  checkAggregationAvgGenericValueAccessor<DatetimeIntervalType, DatetimeIntervalType>();
}

TEST_F(AggregationHandleAvgTest, YearMonthIntervalTypeValueAccessorTest) {
  checkAggregationAvgGenericValueAccessor<YearMonthIntervalType, YearMonthIntervalType>();
}
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

#ifdef QUICKSTEP_DEBUG
TEST_F(AggregationHandleAvgDeathTest, CharTypeTest) {
  const Type &type = CharType::Instance(true, 10);
  EXPECT_DEATH(initializeHandle(type), "");
}

TEST_F(AggregationHandleAvgDeathTest, VarTypeTest) {
  const Type &type = VarCharType::Instance(true, 10);
  EXPECT_DEATH(initializeHandle(type), "");
}

TEST_F(AggregationHandleAvgDeathTest, WrongTypeTest) {
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

  iterateHandle(aggregation_handle_avg_state_.get(), int_non_null_type.makeValue(&int_val));

  EXPECT_DEATH(iterateHandle(aggregation_handle_avg_state_.get(), long_type.makeValue(&long_val)), "");
  EXPECT_DEATH(iterateHandle(aggregation_handle_avg_state_.get(), double_type.makeValue(&double_val)), "");
  EXPECT_DEATH(iterateHandle(aggregation_handle_avg_state_.get(), float_type.makeValue(&float_val)), "");
  EXPECT_DEATH(iterateHandle(aggregation_handle_avg_state_.get(), char_type.makeValue("asdf", 5)), "");
  EXPECT_DEATH(iterateHandle(aggregation_handle_avg_state_.get(), varchar_type.makeValue("asdf", 5)), "");

  // Test mergeStates() with incorrectly typed handles.
  std::unique_ptr<AggregationHandle> aggregation_handle_avg_double(
      AggregateFunctionFactory::Get(AggregationID::kAvg).createHandle(
          std::vector<const Type*>(1, &double_type)));
  std::unique_ptr<AggregationState> aggregation_state_avg_merge_double(
      aggregation_handle_avg_double->createInitialState());
  static_cast<const AggregationHandleAvg&>(*aggregation_handle_avg_double).iterateUnaryInl(
      static_cast<AggregationStateAvg*>(aggregation_state_avg_merge_double.get()),
      double_type.makeValue(&double_val));
  EXPECT_DEATH(aggregation_handle_avg_->mergeStates(*aggregation_state_avg_merge_double,
                                                    aggregation_handle_avg_state_.get()),
               "");

  std::unique_ptr<AggregationHandle> aggregation_handle_avg_float(
      AggregateFunctionFactory::Get(AggregationID::kAvg).createHandle(
          std::vector<const Type*>(1, &float_type)));
  std::unique_ptr<AggregationState> aggregation_state_avg_merge_float(
      aggregation_handle_avg_float->createInitialState());
  static_cast<const AggregationHandleAvg&>(*aggregation_handle_avg_float).iterateUnaryInl(
      static_cast<AggregationStateAvg*>(aggregation_state_avg_merge_float.get()),
      float_type.makeValue(&float_val));
  EXPECT_DEATH(aggregation_handle_avg_->mergeStates(*aggregation_state_avg_merge_float,
                                                    aggregation_handle_avg_state_.get()),
               "");
}
#endif

TEST_F(AggregationHandleAvgTest, canApplyToTypeTest) {
  EXPECT_TRUE(ApplyToTypesTest(kInt));
  EXPECT_TRUE(ApplyToTypesTest(kLong));
  EXPECT_TRUE(ApplyToTypesTest(kFloat));
  EXPECT_TRUE(ApplyToTypesTest(kDouble));
  EXPECT_FALSE(ApplyToTypesTest(kChar));
  EXPECT_FALSE(ApplyToTypesTest(kVarChar));
  EXPECT_FALSE(ApplyToTypesTest(kDatetime));
  EXPECT_TRUE(ApplyToTypesTest(kDatetimeInterval));
  EXPECT_TRUE(ApplyToTypesTest(kYearMonthInterval));
}

TEST_F(AggregationHandleAvgTest, ResultTypeForArgumentTypeTest) {
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kInt, kDouble));
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kLong, kDouble));
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kFloat, kDouble));
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kDouble, kDouble));
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kDatetimeInterval, kDatetimeInterval));
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kYearMonthInterval, kYearMonthInterval));
}

}  // namespace quickstep
