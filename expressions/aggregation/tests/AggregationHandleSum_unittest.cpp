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
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/aggregation/AggregateFunction.hpp"
#include "expressions/aggregation/AggregateFunctionFactory.hpp"
#include "expressions/aggregation/AggregationHandle.hpp"
#include "expressions/aggregation/AggregationHandleSum.hpp"
#include "expressions/aggregation/AggregationID.hpp"
#include "storage/StorageManager.hpp"
#include "types/CharType.hpp"
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

class AggregationHandleSumTest : public::testing::Test {
 protected:
  static const int kNumSamples = 1000;

  // Helper method that calls AggregationHandleSum::iterateUnaryInl() to
  // aggregate 'value' into '*state'.
  void iterateHandle(AggregationState *state, const TypedValue &value) {
    static_cast<const AggregationHandleSum&>(*aggregation_handle_sum_).iterateUnaryInl(
        static_cast<AggregationStateSum*>(state),
        value);
  }

  void initializeHandle(const Type &type) {
    aggregation_handle_sum_.reset(
        AggregateFunctionFactory::Get(AggregationID::kSum).createHandle(
            std::vector<const Type*>(1, &type)));
    aggregation_handle_sum_state_.reset(
        aggregation_handle_sum_->createInitialState());
  }

  static bool ApplyToTypesTest(TypeID typeID) {
    const Type &type = (typeID == kChar || typeID == kVarChar) ?
        TypeFactory::GetType(typeID, static_cast<std::size_t>(10)) :
        TypeFactory::GetType(typeID);

    return AggregateFunctionFactory::Get(AggregationID::kSum).canApplyToTypes(
        std::vector<const Type*>(1, &type));
  }

  static bool ResultTypeForArgumentTypeTest(TypeID input_type_id,
                                            TypeID output_type_id) {
    const Type *result_type
        = AggregateFunctionFactory::Get(AggregationID::kSum).resultTypeForArgumentTypes(
            std::vector<const Type*>(1, &TypeFactory::GetType(input_type_id)));
    return (result_type->getTypeID() == output_type_id);
  }

  template <typename CppType>
  static void CheckSumValue(
      CppType expected,
      const AggregationHandle &target,
      const AggregationState &state) {
    EXPECT_EQ(expected, target.finalize(state).getLiteral<CppType>());
  }

  // Static templated method to set a meaningful to data types.
  template <typename CppType>
  static void SetDataType(int value, CppType *data) {
    *data = value;
  }

  template <typename GenericType, typename PrecisionType>
  void checkAggregationSumGeneric() {
    const GenericType &type = GenericType::Instance(true);

    initializeHandle(type);
    EXPECT_TRUE(aggregation_handle_sum_->finalize(*aggregation_handle_sum_state_).isNull());

    typename GenericType::cpptype val;
    typename PrecisionType::cpptype sum;
    SetDataType(0, &sum);

    iterateHandle(aggregation_handle_sum_state_.get(), type.makeNullValue());
    for (int i = 0; i < kNumSamples; ++i) {
      if (type.getTypeID() == kInt || type.getTypeID() == kLong) {
        SetDataType(i - 10, &val);
      } else {
        SetDataType(static_cast<float>(i - 10)/10, &val);
      }
      iterateHandle(aggregation_handle_sum_state_.get(), type.makeValue(&val));
      sum += val;
    }
    iterateHandle(aggregation_handle_sum_state_.get(), type.makeNullValue());
    CheckSumValue<typename PrecisionType::cpptype>(sum, *aggregation_handle_sum_, *aggregation_handle_sum_state_);

    // Test mergeStates().
    std::unique_ptr<AggregationState> merge_state(
        aggregation_handle_sum_->createInitialState());
    aggregation_handle_sum_->mergeStates(*merge_state,
                                         aggregation_handle_sum_state_.get());

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
    aggregation_handle_sum_->mergeStates(*merge_state,
                                         aggregation_handle_sum_state_.get());
    CheckSumValue<typename PrecisionType::cpptype>(
        sum,
        *aggregation_handle_sum_,
        *aggregation_handle_sum_state_);
  }

  template <typename GenericType, typename Output>
  ColumnVector *createColumnVectorGeneric(const Type &type, Output *sum) {
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

  template <typename GenericType, typename PrecisionType>
  void checkAggregationSumGenericColumnVector() {
    const GenericType &type = GenericType::Instance(true);

    initializeHandle(type);
    EXPECT_TRUE(aggregation_handle_sum_->finalize(*aggregation_handle_sum_state_).isNull());

    typename PrecisionType::cpptype sum;
    std::vector<std::unique_ptr<ColumnVector>> column_vectors;
    column_vectors.emplace_back(
        createColumnVectorGeneric<GenericType, typename PrecisionType::cpptype>(type, &sum));

    std::unique_ptr<AggregationState> cv_state(
        aggregation_handle_sum_->accumulateColumnVectors(column_vectors));

    // Test the state generated directly by accumulateColumnVectors(), and also
    // test after merging back.
    CheckSumValue<typename PrecisionType::cpptype>(
        sum,
        *aggregation_handle_sum_,
        *cv_state);

    aggregation_handle_sum_->mergeStates(*cv_state, aggregation_handle_sum_state_.get());
    CheckSumValue<typename PrecisionType::cpptype>(
        sum,
        *aggregation_handle_sum_,
        *aggregation_handle_sum_state_);
  }

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  template <typename GenericType, typename PrecisionType>
  void checkAggregationSumGenericValueAccessor() {
    const GenericType &type = GenericType::Instance(true);

    initializeHandle(type);
    EXPECT_TRUE(aggregation_handle_sum_->finalize(*aggregation_handle_sum_state_).isNull());

    typename PrecisionType::cpptype sum;
    std::unique_ptr<ColumnVectorsValueAccessor> accessor(new ColumnVectorsValueAccessor());
    accessor->addColumn(
        createColumnVectorGeneric<GenericType, typename PrecisionType::cpptype>(type, &sum));

    std::unique_ptr<AggregationState> va_state(
        aggregation_handle_sum_->accumulateValueAccessor(accessor.get(),
                                                         std::vector<attribute_id>(1, 0)));

    // Test the state generated directly by accumulateValueAccessor(), and also
    // test after merging back.
    CheckSumValue<typename PrecisionType::cpptype>(
        sum,
        *aggregation_handle_sum_,
        *va_state);

    aggregation_handle_sum_->mergeStates(*va_state, aggregation_handle_sum_state_.get());
    CheckSumValue<typename PrecisionType::cpptype>(
        sum,
        *aggregation_handle_sum_,
        *aggregation_handle_sum_state_);
  }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

  std::unique_ptr<AggregationHandle> aggregation_handle_sum_;
  std::unique_ptr<AggregationState> aggregation_handle_sum_state_;
  std::unique_ptr<StorageManager> storage_manager_;
};

const int AggregationHandleSumTest::kNumSamples;

template <>
void AggregationHandleSumTest::CheckSumValue<float>(
    float val,
    const AggregationHandle &handle,
    const AggregationState &state) {
  EXPECT_FLOAT_EQ(val, handle.finalize(state).getLiteral<float>());
}

template <>
void AggregationHandleSumTest::CheckSumValue<double>(
    double val,
    const AggregationHandle &handle,
    const AggregationState &state) {
  EXPECT_DOUBLE_EQ(val, handle.finalize(state).getLiteral<double>());
}

template <>
void AggregationHandleSumTest::SetDataType<DatetimeIntervalLit>(int value, DatetimeIntervalLit *data) {
  data->interval_ticks = value;
}

template <>
void AggregationHandleSumTest::SetDataType<YearMonthIntervalLit>(int value, YearMonthIntervalLit *data) {
  data->months = value;
}

typedef AggregationHandleSumTest AggregationHandleSumDeathTest;

TEST_F(AggregationHandleSumTest, IntTypeTest) {
  checkAggregationSumGeneric<IntType, LongType>();
}

TEST_F(AggregationHandleSumTest, LongTypeTest) {
  checkAggregationSumGeneric<LongType, LongType>();
}

TEST_F(AggregationHandleSumTest, FloatTypeTest) {
  checkAggregationSumGeneric<FloatType, DoubleType>();
}

TEST_F(AggregationHandleSumTest, DoubleTypeTest) {
  checkAggregationSumGeneric<DoubleType, DoubleType>();
}

TEST_F(AggregationHandleSumTest, DatetimeIntervalTypeTest) {
  checkAggregationSumGeneric<DatetimeIntervalType, DatetimeIntervalType>();
}

TEST_F(AggregationHandleSumTest, YearMonthIntervalTypeTest) {
  checkAggregationSumGeneric<YearMonthIntervalType, YearMonthIntervalType>();
}

TEST_F(AggregationHandleSumTest, IntTypeColumnVectorTest) {
  checkAggregationSumGenericColumnVector<IntType, LongType>();
}

TEST_F(AggregationHandleSumTest, LongTypeColumnVectorTest) {
  checkAggregationSumGenericColumnVector<LongType, LongType>();
}

TEST_F(AggregationHandleSumTest, FloatTypeColumnVectorTest) {
  checkAggregationSumGenericColumnVector<FloatType, DoubleType>();
}

TEST_F(AggregationHandleSumTest, DoubleTypeColumnVectorTest) {
  checkAggregationSumGenericColumnVector<DoubleType, DoubleType>();
}

TEST_F(AggregationHandleSumTest, DatetimeIntervalTypeColumnVectorTest) {
  checkAggregationSumGenericColumnVector<DatetimeIntervalType, DatetimeIntervalType>();
}

TEST_F(AggregationHandleSumTest, YearMonthIntervalTypeColumnVectorTest) {
  checkAggregationSumGenericColumnVector<YearMonthIntervalType, YearMonthIntervalType>();
}

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
TEST_F(AggregationHandleSumTest, IntTypeValueAccessorTest) {
  checkAggregationSumGenericValueAccessor<IntType, LongType>();
}

TEST_F(AggregationHandleSumTest, LongTypeValueAccessorTest) {
  checkAggregationSumGenericValueAccessor<LongType, LongType>();
}

TEST_F(AggregationHandleSumTest, FloatTypeValueAccessorTest) {
  checkAggregationSumGenericValueAccessor<FloatType, DoubleType>();
}

TEST_F(AggregationHandleSumTest, DoubleTypeValueAccessorTest) {
  checkAggregationSumGenericValueAccessor<DoubleType, DoubleType>();
}

TEST_F(AggregationHandleSumTest, DatetimeIntervalTypeValueAccessorTest) {
  checkAggregationSumGenericValueAccessor<DatetimeIntervalType, DatetimeIntervalType>();
}

TEST_F(AggregationHandleSumTest, YearMonthIntervalTypeValueAccessorTest) {
  checkAggregationSumGenericValueAccessor<YearMonthIntervalType, YearMonthIntervalType>();
}
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

#ifdef QUICKSTEP_DEBUG
TEST_F(AggregationHandleSumDeathTest, CharTypeTest) {
  const Type &type = CharType::Instance(true, 10);
  EXPECT_DEATH(initializeHandle(type), "");
}

TEST_F(AggregationHandleSumDeathTest, VarTypeTest) {
  const Type &type = VarCharType::Instance(true, 10);
  EXPECT_DEATH(initializeHandle(type), "");
}

TEST_F(AggregationHandleSumDeathTest, WrongTypeTest) {
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
  iterateHandle(aggregation_handle_sum_state_.get(), int_non_null_type.makeValue(&int_val));

  EXPECT_DEATH(iterateHandle(aggregation_handle_sum_state_.get(), long_type.makeValue(&long_val)), "");
  EXPECT_DEATH(iterateHandle(aggregation_handle_sum_state_.get(), double_type.makeValue(&double_val)), "");
  EXPECT_DEATH(iterateHandle(aggregation_handle_sum_state_.get(), float_type.makeValue(&float_val)), "");
  EXPECT_DEATH(iterateHandle(aggregation_handle_sum_state_.get(), char_type.makeValue("asdf", 5)), "");
  EXPECT_DEATH(iterateHandle(aggregation_handle_sum_state_.get(), varchar_type.makeValue("asdf", 5)), "");

  // Test mergeStates() with incorrectly typed handles.
  std::unique_ptr<AggregationHandle> aggregation_handle_sum_double(
      AggregateFunctionFactory::Get(AggregationID::kSum).createHandle(
          std::vector<const Type*>(1, &double_type)));
  std::unique_ptr<AggregationState> aggregation_state_sum_merge_double(
      aggregation_handle_sum_double->createInitialState());
  static_cast<const AggregationHandleSum&>(*aggregation_handle_sum_double).iterateUnaryInl(
      static_cast<AggregationStateSum*>(aggregation_state_sum_merge_double.get()),
      double_type.makeValue(&double_val));
  EXPECT_DEATH(aggregation_handle_sum_->mergeStates(*aggregation_state_sum_merge_double,
                                                    aggregation_handle_sum_state_.get()),
               "");

  std::unique_ptr<AggregationHandle> aggregation_handle_sum_float(
      AggregateFunctionFactory::Get(AggregationID::kSum).createHandle(
          std::vector<const Type*>(1, &float_type)));
  std::unique_ptr<AggregationState> aggregation_state_sum_merge_float(
      aggregation_handle_sum_float->createInitialState());
  static_cast<const AggregationHandleSum&>(*aggregation_handle_sum_float).iterateUnaryInl(
      static_cast<AggregationStateSum*>(aggregation_state_sum_merge_float.get()),
      float_type.makeValue(&float_val));
  EXPECT_DEATH(aggregation_handle_sum_->mergeStates(*aggregation_state_sum_merge_float,
                                                    aggregation_handle_sum_state_.get()),
               "");
}
#endif

TEST_F(AggregationHandleSumTest, canApplyToTypeTest) {
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

TEST_F(AggregationHandleSumTest, ResultTypeForArgumentTypeTest) {
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kInt, kLong));
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kLong, kLong));
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kFloat, kDouble));
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kDouble, kDouble));
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kDatetimeInterval, kDatetimeInterval));
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kYearMonthInterval, kYearMonthInterval));
}

TEST_F(AggregationHandleSumTest, GroupByTableMergeTest) {
  const Type &long_non_null_type = LongType::Instance(false);
  initializeHandle(long_non_null_type);
  storage_manager_.reset(new StorageManager("./test_sum_data"));
  std::unique_ptr<AggregationStateHashTableBase> source_hash_table(
      aggregation_handle_sum_->createGroupByHashTable(
          HashTableImplType::kSimpleScalarSeparateChaining,
          std::vector<const Type *>(1, &long_non_null_type),
          10,
          storage_manager_.get()));
  std::unique_ptr<AggregationStateHashTableBase> destination_hash_table(
      aggregation_handle_sum_->createGroupByHashTable(
          HashTableImplType::kSimpleScalarSeparateChaining,
          std::vector<const Type *>(1, &long_non_null_type),
          10,
          storage_manager_.get()));

  AggregationStateHashTable<AggregationStateSum> *destination_hash_table_derived =
      static_cast<AggregationStateHashTable<AggregationStateSum> *>(
          destination_hash_table.get());

  AggregationStateHashTable<AggregationStateSum> *source_hash_table_derived =
      static_cast<AggregationStateHashTable<AggregationStateSum> *>(
          source_hash_table.get());

  AggregationHandleSum *aggregation_handle_sum_derived =
      static_cast<AggregationHandleSum *>(aggregation_handle_sum_.get());
  // We create three keys: first is present in both the hash tables, second key
  // is present only in the source hash table while the third key is present
  // the destination hash table only.
  std::vector<TypedValue> common_key;
  common_key.emplace_back(static_cast<std::int64_t>(0));
  std::vector<TypedValue> exclusive_source_key, exclusive_destination_key;
  exclusive_source_key.emplace_back(static_cast<std::int64_t>(1));
  exclusive_destination_key.emplace_back(static_cast<std::int64_t>(2));

  const std::int64_t common_key_source_sum = 3000;
  TypedValue common_key_source_sum_val(common_key_source_sum);

  const std::int64_t common_key_destination_sum = 4000;
  TypedValue common_key_destination_sum_val(common_key_destination_sum);

  const std::int64_t merged_common_key = common_key_source_sum + common_key_destination_sum;
  TypedValue common_key_merged_val(merged_common_key);

  const std::int64_t exclusive_key_source_sum = 100;
  TypedValue exclusive_key_source_sum_val(exclusive_key_source_sum);

  const std::int64_t exclusive_key_destination_sum = 200;
  TypedValue exclusive_key_destination_sum_val(exclusive_key_destination_sum);

  std::unique_ptr<AggregationStateSum> common_key_source_state(
      static_cast<AggregationStateSum *>(
          aggregation_handle_sum_->createInitialState()));
  std::unique_ptr<AggregationStateSum> common_key_destination_state(
      static_cast<AggregationStateSum *>(
          aggregation_handle_sum_->createInitialState()));
  std::unique_ptr<AggregationStateSum> exclusive_key_source_state(
      static_cast<AggregationStateSum *>(
          aggregation_handle_sum_->createInitialState()));
  std::unique_ptr<AggregationStateSum> exclusive_key_destination_state(
      static_cast<AggregationStateSum *>(
          aggregation_handle_sum_->createInitialState()));

  // Create sum value states for keys.
  aggregation_handle_sum_derived->iterateUnaryInl(common_key_source_state.get(),
                                                  common_key_source_sum_val);
  std::int64_t actual_val = aggregation_handle_sum_->finalize(*common_key_source_state)
                       .getLiteral<std::int64_t>();
  EXPECT_EQ(common_key_source_sum_val.getLiteral<std::int64_t>(), actual_val);

  aggregation_handle_sum_derived->iterateUnaryInl(
      common_key_destination_state.get(), common_key_destination_sum_val);
  actual_val = aggregation_handle_sum_->finalize(*common_key_destination_state)
                   .getLiteral<std::int64_t>();
  EXPECT_EQ(common_key_destination_sum_val.getLiteral<std::int64_t>(), actual_val);

  aggregation_handle_sum_derived->iterateUnaryInl(
      exclusive_key_destination_state.get(), exclusive_key_destination_sum_val);
  actual_val =
      aggregation_handle_sum_->finalize(*exclusive_key_destination_state)
          .getLiteral<std::int64_t>();
  EXPECT_EQ(exclusive_key_destination_sum_val.getLiteral<std::int64_t>(), actual_val);

  aggregation_handle_sum_derived->iterateUnaryInl(
      exclusive_key_source_state.get(), exclusive_key_source_sum_val);
  actual_val = aggregation_handle_sum_->finalize(*exclusive_key_source_state)
                   .getLiteral<std::int64_t>();
  EXPECT_EQ(exclusive_key_source_sum_val.getLiteral<std::int64_t>(), actual_val);

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

  aggregation_handle_sum_->mergeGroupByHashTables(*source_hash_table,
                                                  destination_hash_table.get());

  EXPECT_EQ(3u, destination_hash_table_derived->numEntries());

  CheckSumValue<std::int64_t>(
      common_key_merged_val.getLiteral<std::int64_t>(),
      *aggregation_handle_sum_derived,
      *(destination_hash_table_derived->getSingleCompositeKey(common_key)));
  CheckSumValue<std::int64_t>(exclusive_key_destination_sum_val.getLiteral<std::int64_t>(),
                     *aggregation_handle_sum_derived,
                     *(destination_hash_table_derived->getSingleCompositeKey(
                         exclusive_destination_key)));
  CheckSumValue<std::int64_t>(exclusive_key_source_sum_val.getLiteral<std::int64_t>(),
                     *aggregation_handle_sum_derived,
                     *(source_hash_table_derived->getSingleCompositeKey(
                         exclusive_source_key)));
}

}  // namespace quickstep
