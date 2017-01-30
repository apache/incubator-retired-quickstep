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
#include "expressions/aggregation/AggregationHandleMin.hpp"
#include "expressions/aggregation/AggregationID.hpp"
#include "storage/AggregationOperationState.hpp"
#include "storage/PackedPayloadHashTable.hpp"
#include "storage/StorageManager.hpp"
#include "storage/ValueAccessorMultiplexer.hpp"
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
#include "types/containers/ColumnVectorsValueAccessor.hpp"

#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"

#include "gtest/gtest.h"

namespace quickstep {

class AggregationHandleMinTest : public ::testing::Test {
 protected:
  static const int kNumSamples = 200;
  static const int kIterations = 5;

  // Helper method that calls AggregationHandleMin::iterateUnaryInl() to
  // aggregate 'value' into '*state'.
  void iterateHandle(AggregationState *state, const TypedValue &value) {
    static_cast<const AggregationHandleMin &>(*aggregation_handle_min_)
        .iterateUnaryInl(static_cast<AggregationStateMin *>(state), value);
  }

  void initializeHandle(const Type &type) {
    aggregation_handle_min_.reset(
        AggregateFunctionFactory::Get(AggregationID::kMin)
            .createHandle(std::vector<const Type *>(1, &type)));
    aggregation_handle_min_state_.reset(
        aggregation_handle_min_->createInitialState());
  }

  static bool ApplyToTypesTest(TypeID typeID) {
    const Type &type =
        (typeID == kChar || typeID == kVarChar)
            ? TypeFactory::GetType(typeID, static_cast<std::size_t>(10))
            : TypeFactory::GetType(typeID);

    return AggregateFunctionFactory::Get(AggregationID::kMin)
        .canApplyToTypes(std::vector<const Type *>(1, &type));
  }

  static bool ResultTypeForArgumentTypeTest(TypeID input_type_id,
                                            TypeID output_type_id) {
    const Type *result_type =
        AggregateFunctionFactory::Get(AggregationID::kMin)
            .resultTypeForArgumentTypes(std::vector<const Type *>(
                1, &TypeFactory::GetType(input_type_id)));
    return (result_type->getTypeID() == output_type_id);
  }

  template <typename CppType>
  static void CheckMinValue(CppType expected,
                            const AggregationHandle &handle,
                            const AggregationState &state) {
    EXPECT_EQ(expected, handle.finalize(state).getLiteral<CppType>());
  }

  template <typename CppType>
  static void CheckMinValue(CppType expected, const TypedValue &value) {
    EXPECT_EQ(expected, value.getLiteral<CppType>());
  }

  static void CheckMinString(const std::string &expected,
                             const AggregationHandle &handle,
                             const AggregationState &state) {
    TypedValue value = handle.finalize(state);

    ASSERT_EQ(expected.length(), value.getAsciiStringLength());
    EXPECT_EQ(0,
              std::strncmp(expected.c_str(),
                           static_cast<const char *>(value.getDataPtr()),
                           value.getAsciiStringLength()));
  }

  // Static templated method to initialize data types.
  template <typename CppType>
  void SetDataType(int value, CppType *data) {
    *data = value;
  }

  template <typename GenericType>
  void checkAggregationMinGeneric() {
    const GenericType &type = GenericType::Instance(true);
    initializeHandle(type);
    EXPECT_TRUE(
        aggregation_handle_min_->finalize(*aggregation_handle_min_state_)
            .isNull());

    typename GenericType::cpptype val;
    typename GenericType::cpptype min;
    SetDataType(1000, &min);

    iterateHandle(aggregation_handle_min_state_.get(), type.makeNullValue());
    for (int i = 0; i < kIterations; ++i) {
      for (int j = kNumSamples - 1; j >= 0; --j) {
        if (type.getTypeID() == kInt || type.getTypeID() == kLong) {
          SetDataType(i * kNumSamples + j - 10, &val);
        } else {
          SetDataType(static_cast<float>(i * kNumSamples + j - 10) / 10, &val);
        }
        iterateHandle(aggregation_handle_min_state_.get(),
                      type.makeValue(&val));
        if (min > val) {
          min = val;
        }
      }
    }
    iterateHandle(aggregation_handle_min_state_.get(), type.makeNullValue());
    CheckMinValue<typename GenericType::cpptype>(
        min, *aggregation_handle_min_, *aggregation_handle_min_state_);

    // Test mergeStates().
    std::unique_ptr<AggregationState> merge_state(
        aggregation_handle_min_->createInitialState());
    aggregation_handle_min_->mergeStates(*merge_state,
                                         aggregation_handle_min_state_.get());

    iterateHandle(merge_state.get(), type.makeNullValue());
    for (int i = 0; i < kIterations; ++i) {
      for (int j = kNumSamples - 1; j >= 0; --j) {
        if (type.getTypeID() == kInt || type.getTypeID() == kLong) {
          SetDataType(i * kNumSamples + j - 20, &val);
        } else {
          SetDataType(static_cast<float>(i * kNumSamples + j - 20) / 10, &val);
        }
        iterateHandle(merge_state.get(), type.makeValue(&val));
        if (min > val) {
          min = val;
        }
      }
    }
    aggregation_handle_min_->mergeStates(*merge_state,
                                         aggregation_handle_min_state_.get());
    CheckMinValue<typename GenericType::cpptype>(
        min, *aggregation_handle_min_, *aggregation_handle_min_state_);
  }

  template <typename GenericType>
  ColumnVector* createColumnVectorGeneric(const Type &type,
                                          typename GenericType::cpptype *min) {
    NativeColumnVector *column =
        new NativeColumnVector(type, kIterations * kNumSamples + 3);

    typename GenericType::cpptype val;
    SetDataType(1000, min);

    column->appendTypedValue(type.makeNullValue());
    for (int i = 0; i < kIterations; ++i) {
      for (int j = kNumSamples - 1; j >= 0; --j) {
        if (type.getTypeID() == kInt || type.getTypeID() == kLong) {
          SetDataType(i * kNumSamples + j - 10, &val);
        } else {
          SetDataType(static_cast<float>(i * kNumSamples + j - 10) / 10, &val);
        }
        column->appendTypedValue(type.makeValue(&val));
        if (*min > val) {
          *min = val;
        }
      }
      // One NULL in the middle.
      if (i == kIterations / 2) {
        column->appendTypedValue(type.makeNullValue());
      }
    }
    column->appendTypedValue(type.makeNullValue());

    return column;
  }

  template <typename GenericType>
  void checkAggregationMinGenericValueAccessor() {
    const GenericType &type = GenericType::Instance(true);
    initializeHandle(type);
    EXPECT_TRUE(
        aggregation_handle_min_->finalize(*aggregation_handle_min_state_)
            .isNull());

    std::unique_ptr<ColumnVectorsValueAccessor> accessor(
        new ColumnVectorsValueAccessor());

    typename GenericType::cpptype min;
    accessor->addColumn(createColumnVectorGeneric<GenericType>(type, &min));

    std::unique_ptr<AggregationState> va_state(
        aggregation_handle_min_->accumulateValueAccessor(
            {MultiSourceAttributeId(ValueAccessorSource::kBase, 0)},
            ValueAccessorMultiplexer(accessor.get())));

    // Test the state generated directly by accumulateValueAccessor(), and also
    // test after merging back.
    CheckMinValue<typename GenericType::cpptype>(
        min, *aggregation_handle_min_, *va_state);

    aggregation_handle_min_->mergeStates(*va_state,
                                         aggregation_handle_min_state_.get());
    CheckMinValue<typename GenericType::cpptype>(
        min, *aggregation_handle_min_, *aggregation_handle_min_state_);
  }

  template <typename StringType>
  void checkAggregationMinString() {
    const StringType &type = StringType::Instance(10, true);
    initializeHandle(type);
    EXPECT_TRUE(
        aggregation_handle_min_->finalize(*aggregation_handle_min_state_)
            .isNull());

    std::unique_ptr<UncheckedComparator> fast_comparator_;
    fast_comparator_.reset(ComparisonFactory::GetComparison(ComparisonID::kLess)
                               .makeUncheckedComparatorForTypes(type, type));
    std::string string_literal;
    std::string min = "z";
    int val;
    iterateHandle(aggregation_handle_min_state_.get(), type.makeNullValue());
    for (int i = 0; i < kIterations; ++i) {
      for (int j = kNumSamples - 1; j >= 0; --j) {
        val = i * kNumSamples + j;
        std::ostringstream oss;
        oss << "test" << val;
        string_literal = oss.str();

        iterateHandle(
            aggregation_handle_min_state_.get(),
            type.makeValue(string_literal.c_str(), string_literal.length() + 1)
                .ensureNotReference());
        if (fast_comparator_->compareDataPtrs(string_literal.c_str(),
                                              min.c_str())) {
          min = string_literal;
        }
      }
    }
    iterateHandle(aggregation_handle_min_state_.get(), type.makeNullValue());
    CheckMinString(
        min, *aggregation_handle_min_, *aggregation_handle_min_state_);

    // Test mergeStates().
    std::unique_ptr<AggregationState> merge_state(
        aggregation_handle_min_->createInitialState());
    aggregation_handle_min_->mergeStates(*merge_state,
                                         aggregation_handle_min_state_.get());

    iterateHandle(merge_state.get(), type.makeNullValue());
    for (int i = 0; i < kIterations; ++i) {
      for (int j = kNumSamples - 1; j >= 0; --j) {
        val = i * kNumSamples + j;
        std::ostringstream oss;
        oss << "min" << val;
        string_literal = oss.str();

        iterateHandle(
            merge_state.get(),
            type.makeValue(string_literal.c_str(), string_literal.length() + 1)
                .ensureNotReference());
        if (fast_comparator_->compareDataPtrs(string_literal.c_str(),
                                              min.c_str())) {
          min = string_literal;
        }
      }
    }
    aggregation_handle_min_->mergeStates(*merge_state,
                                         aggregation_handle_min_state_.get());
    CheckMinString(
        min, *aggregation_handle_min_, *aggregation_handle_min_state_);
  }

  template <typename ColumnVectorType>
  ColumnVector* createColumnVectorString(const Type &type, std::string *min) {
    ColumnVectorType *column =
        new ColumnVectorType(type, kIterations * kNumSamples + 3);
    std::unique_ptr<UncheckedComparator> fast_comparator_;
    fast_comparator_.reset(ComparisonFactory::GetComparison(ComparisonID::kLess)
                               .makeUncheckedComparatorForTypes(type, type));
    std::string string_literal;
    *min = "z";
    int val;
    column->appendTypedValue(type.makeNullValue());
    for (int i = 0; i < kIterations; ++i) {
      for (int j = kNumSamples - 1; j >= 0; --j) {
        val = i * kNumSamples + j;
        std::ostringstream oss;
        oss << "test" << val;
        string_literal = oss.str();

        column->appendTypedValue(
            type.makeValue(string_literal.c_str(), string_literal.length() + 1)
                .ensureNotReference());
        if (fast_comparator_->compareDataPtrs(string_literal.c_str(),
                                              min->c_str())) {
          *min = string_literal;
        }
      }
      // One NULL in the middle.
      if (i == kIterations / 2) {
        column->appendTypedValue(type.makeNullValue());
      }
    }
    column->appendTypedValue(type.makeNullValue());

    return column;
  }

  template <typename StringType, typename ColumnVectorType>
  void checkAggregationMinStringValueAccessor() {
    const StringType &type = StringType::Instance(10, true);
    initializeHandle(type);
    EXPECT_TRUE(
        aggregation_handle_min_->finalize(*aggregation_handle_min_state_)
            .isNull());

    std::string min;
    std::unique_ptr<ColumnVectorsValueAccessor> accessor(
        new ColumnVectorsValueAccessor());
    accessor->addColumn(createColumnVectorString<ColumnVectorType>(type, &min));

    std::unique_ptr<AggregationState> va_state(
        aggregation_handle_min_->accumulateValueAccessor(
            {MultiSourceAttributeId(ValueAccessorSource::kBase, 0)},
            ValueAccessorMultiplexer(accessor.get())));

    // Test the state generated directly by accumulateValueAccessor(), and also
    // test after merging back.
    CheckMinString(min, *aggregation_handle_min_, *va_state);

    aggregation_handle_min_->mergeStates(*va_state,
                                         aggregation_handle_min_state_.get());
    CheckMinString(
        min, *aggregation_handle_min_, *aggregation_handle_min_state_);
  }

  std::unique_ptr<AggregationHandle> aggregation_handle_min_;
  std::unique_ptr<AggregationState> aggregation_handle_min_state_;
  std::unique_ptr<StorageManager> storage_manager_;
};

template <>
void AggregationHandleMinTest::CheckMinValue<float>(
    float val, const AggregationHandle &handle, const AggregationState &state) {
  EXPECT_FLOAT_EQ(val, handle.finalize(state).getLiteral<float>());
}

template <>
void AggregationHandleMinTest::CheckMinValue<double>(
    double val,
    const AggregationHandle &handle,
    const AggregationState &state) {
  EXPECT_DOUBLE_EQ(val, handle.finalize(state).getLiteral<double>());
}

template <>
void AggregationHandleMinTest::SetDataType<DatetimeLit>(int value,
                                                        DatetimeLit *data) {
  data->ticks = value;
}

template <>
void AggregationHandleMinTest::SetDataType<DatetimeIntervalLit>(
    int value, DatetimeIntervalLit *data) {
  data->interval_ticks = value;
}

template <>
void AggregationHandleMinTest::SetDataType<YearMonthIntervalLit>(
    int value, YearMonthIntervalLit *data) {
  data->months = value;
}

typedef AggregationHandleMinTest AggregationHandleMinDeathTest;

TEST_F(AggregationHandleMinTest, IntTypeTest) {
  checkAggregationMinGeneric<IntType>();
}

TEST_F(AggregationHandleMinTest, LongTypeTest) {
  checkAggregationMinGeneric<LongType>();
}

TEST_F(AggregationHandleMinTest, FloatTypeTest) {
  checkAggregationMinGeneric<FloatType>();
}

TEST_F(AggregationHandleMinTest, DoubleTypeTest) {
  checkAggregationMinGeneric<DoubleType>();
}

TEST_F(AggregationHandleMinTest, DatetimeType) {
  checkAggregationMinGeneric<DatetimeType>();
}

TEST_F(AggregationHandleMinTest, DatetimeIntervalType) {
  checkAggregationMinGeneric<DatetimeIntervalType>();
}

TEST_F(AggregationHandleMinTest, YearMonthIntervalType) {
  checkAggregationMinGeneric<YearMonthIntervalType>();
}

TEST_F(AggregationHandleMinTest, CharTypeTest) {
  checkAggregationMinString<CharType>();
}

TEST_F(AggregationHandleMinTest, VarCharTypeTest) {
  checkAggregationMinString<VarCharType>();
}

TEST_F(AggregationHandleMinTest, IntTypeValueAccessorTest) {
  checkAggregationMinGenericValueAccessor<IntType>();
}

TEST_F(AggregationHandleMinTest, LongTypeValueAccessorTest) {
  checkAggregationMinGenericValueAccessor<LongType>();
}

TEST_F(AggregationHandleMinTest, FloatTypeValueAccessorTest) {
  checkAggregationMinGenericValueAccessor<FloatType>();
}

TEST_F(AggregationHandleMinTest, DoubleTypeValueAccessorTest) {
  checkAggregationMinGenericValueAccessor<DoubleType>();
}

TEST_F(AggregationHandleMinTest, DatetimeTypeValueAccessorTest) {
  checkAggregationMinGenericValueAccessor<DatetimeType>();
}

TEST_F(AggregationHandleMinTest, DatetimeIntervalTypeValueAccessorTest) {
  checkAggregationMinGenericValueAccessor<DatetimeIntervalType>();
}

TEST_F(AggregationHandleMinTest, YearMonthIntervalTypeValueAccessorTest) {
  checkAggregationMinGenericValueAccessor<YearMonthIntervalType>();
}

TEST_F(AggregationHandleMinTest, CharTypeValueAccessorTest) {
  checkAggregationMinStringValueAccessor<CharType, NativeColumnVector>();
}

TEST_F(AggregationHandleMinTest, VarCharTypeValueAccessorTest) {
  checkAggregationMinStringValueAccessor<VarCharType, IndirectColumnVector>();
}

#ifdef QUICKSTEP_DEBUG
TEST_F(AggregationHandleMinDeathTest, WrongTypeTest) {
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

  iterateHandle(aggregation_handle_min_state_.get(),
                int_non_null_type.makeValue(&int_val));

  EXPECT_DEATH(iterateHandle(aggregation_handle_min_state_.get(),
                             long_type.makeValue(&long_val)),
               "");
  EXPECT_DEATH(iterateHandle(aggregation_handle_min_state_.get(),
                             double_type.makeValue(&double_val)),
               "");
  EXPECT_DEATH(iterateHandle(aggregation_handle_min_state_.get(),
                             float_type.makeValue(&float_val)),
               "");
  EXPECT_DEATH(iterateHandle(aggregation_handle_min_state_.get(),
                             char_type.makeValue("asdf", 5)),
               "");
  EXPECT_DEATH(iterateHandle(aggregation_handle_min_state_.get(),
                             varchar_type.makeValue("asdf", 5)),
               "");

  // Test mergeStates() with incorrectly typed handles.
  std::unique_ptr<AggregationHandle> aggregation_handle_min_long(
      AggregateFunctionFactory::Get(AggregationID::kMin)
          .createHandle(std::vector<const Type *>(1, &long_type)));
  std::unique_ptr<AggregationState> aggregation_state_min_merge_long(
      aggregation_handle_min_long->createInitialState());
  static_cast<const AggregationHandleMin &>(*aggregation_handle_min_long)
      .iterateUnaryInl(static_cast<AggregationStateMin *>(
                           aggregation_state_min_merge_long.get()),
                       long_type.makeValue(&long_val));
  EXPECT_DEATH(
      aggregation_handle_min_->mergeStates(*aggregation_state_min_merge_long,
                                           aggregation_handle_min_state_.get()),
      "");

  std::unique_ptr<AggregationHandle> aggregation_handle_min_double(
      AggregateFunctionFactory::Get(AggregationID::kMin)
          .createHandle(std::vector<const Type *>(1, &double_type)));
  std::unique_ptr<AggregationState> aggregation_state_min_merge_double(
      aggregation_handle_min_double->createInitialState());
  static_cast<const AggregationHandleMin &>(*aggregation_handle_min_double)
      .iterateUnaryInl(static_cast<AggregationStateMin *>(
                           aggregation_state_min_merge_double.get()),
                       double_type.makeValue(&double_val));
  EXPECT_DEATH(
      aggregation_handle_min_->mergeStates(*aggregation_state_min_merge_double,
                                           aggregation_handle_min_state_.get()),
      "");

  std::unique_ptr<AggregationHandle> aggregation_handle_min_float(
      AggregateFunctionFactory::Get(AggregationID::kMin)
          .createHandle(std::vector<const Type *>(1, &float_type)));
  std::unique_ptr<AggregationState> aggregation_state_min_merge_float(
      aggregation_handle_min_float->createInitialState());
  static_cast<const AggregationHandleMin &>(*aggregation_handle_min_float)
      .iterateUnaryInl(static_cast<AggregationStateMin *>(
                           aggregation_state_min_merge_float.get()),
                       float_type.makeValue(&float_val));
  EXPECT_DEATH(
      aggregation_handle_min_->mergeStates(*aggregation_state_min_merge_float,
                                           aggregation_handle_min_state_.get()),
      "");
}
#endif

TEST_F(AggregationHandleMinTest, canApplyToTypeTest) {
  EXPECT_TRUE(ApplyToTypesTest(kInt));
  EXPECT_TRUE(ApplyToTypesTest(kLong));
  EXPECT_TRUE(ApplyToTypesTest(kFloat));
  EXPECT_TRUE(ApplyToTypesTest(kDouble));
  EXPECT_TRUE(ApplyToTypesTest(kChar));
  EXPECT_TRUE(ApplyToTypesTest(kVarChar));
}

TEST_F(AggregationHandleMinTest, ResultTypeForArgumentTypeTest) {
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kInt, kInt));
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kLong, kLong));
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kFloat, kFloat));
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kDouble, kDouble));
}

TEST_F(AggregationHandleMinTest, GroupByTableMergeTest) {
  const Type &int_non_null_type = IntType::Instance(false);
  initializeHandle(int_non_null_type);
  storage_manager_.reset(new StorageManager("./test_min_data"));
  std::unique_ptr<AggregationStateHashTableBase> source_hash_table(
      AggregationStateHashTableFactory::CreateResizable(
          HashTableImplType::kSeparateChaining,
          std::vector<const Type *>(1, &int_non_null_type),
          10,
          {aggregation_handle_min_.get()},
          storage_manager_.get()));
  std::unique_ptr<AggregationStateHashTableBase> destination_hash_table(
      AggregationStateHashTableFactory::CreateResizable(
          HashTableImplType::kSeparateChaining,
          std::vector<const Type *>(1, &int_non_null_type),
          10,
          {aggregation_handle_min_.get()},
          storage_manager_.get()));

  PackedPayloadHashTable *destination_hash_table_derived =
      static_cast<PackedPayloadHashTable *>(destination_hash_table.get());

  PackedPayloadHashTable *source_hash_table_derived =
      static_cast<PackedPayloadHashTable *>(source_hash_table.get());

  AggregationHandleMin *aggregation_handle_min_derived =
      static_cast<AggregationHandleMin *>(aggregation_handle_min_.get());
  // We create three keys: first is present in both the hash tables, second key
  // is present only in the source hash table while the third key is present
  // the destination hash table only.
  std::vector<TypedValue> common_key;
  common_key.emplace_back(0);
  std::vector<TypedValue> exclusive_source_key, exclusive_destination_key;
  exclusive_source_key.emplace_back(1);
  exclusive_destination_key.emplace_back(2);

  const int common_key_source_min = 3000;
  TypedValue common_key_source_min_val(common_key_source_min);

  const int common_key_destination_min = 4000;
  TypedValue common_key_destination_min_val(common_key_destination_min);

  const int exclusive_key_source_min = 100;
  TypedValue exclusive_key_source_min_val(exclusive_key_source_min);

  const int exclusive_key_destination_min = 200;
  TypedValue exclusive_key_destination_min_val(exclusive_key_destination_min);

  std::unique_ptr<AggregationStateMin> common_key_source_state(
      static_cast<AggregationStateMin *>(
          aggregation_handle_min_->createInitialState()));
  std::unique_ptr<AggregationStateMin> common_key_destination_state(
      static_cast<AggregationStateMin *>(
          aggregation_handle_min_->createInitialState()));
  std::unique_ptr<AggregationStateMin> exclusive_key_source_state(
      static_cast<AggregationStateMin *>(
          aggregation_handle_min_->createInitialState()));
  std::unique_ptr<AggregationStateMin> exclusive_key_destination_state(
      static_cast<AggregationStateMin *>(
          aggregation_handle_min_->createInitialState()));

  // Create min value states for keys.
  aggregation_handle_min_derived->iterateUnaryInl(common_key_source_state.get(),
                                                  common_key_source_min_val);
  int actual_val = aggregation_handle_min_->finalize(*common_key_source_state)
                       .getLiteral<int>();
  EXPECT_EQ(common_key_source_min_val.getLiteral<int>(), actual_val);

  aggregation_handle_min_derived->iterateUnaryInl(
      common_key_destination_state.get(), common_key_destination_min_val);
  actual_val = aggregation_handle_min_->finalize(*common_key_destination_state)
                   .getLiteral<int>();
  EXPECT_EQ(common_key_destination_min_val.getLiteral<int>(), actual_val);

  aggregation_handle_min_derived->iterateUnaryInl(
      exclusive_key_destination_state.get(), exclusive_key_destination_min_val);
  actual_val =
      aggregation_handle_min_->finalize(*exclusive_key_destination_state)
          .getLiteral<int>();
  EXPECT_EQ(exclusive_key_destination_min_val.getLiteral<int>(), actual_val);

  aggregation_handle_min_derived->iterateUnaryInl(
      exclusive_key_source_state.get(), exclusive_key_source_min_val);
  actual_val = aggregation_handle_min_->finalize(*exclusive_key_source_state)
                   .getLiteral<int>();
  EXPECT_EQ(exclusive_key_source_min_val.getLiteral<int>(), actual_val);

  // Add the key-state pairs to the hash tables.
  unsigned char buffer[100];
  buffer[0] = '\0';
  memcpy(buffer + 1,
         common_key_source_state.get()->getPayloadAddress(),
         aggregation_handle_min_.get()->getPayloadSize());
  source_hash_table_derived->upsertCompositeKey(common_key, buffer);

  memcpy(buffer + 1,
         common_key_destination_state.get()->getPayloadAddress(),
         aggregation_handle_min_.get()->getPayloadSize());
  destination_hash_table_derived->upsertCompositeKey(common_key, buffer);

  memcpy(buffer + 1,
         exclusive_key_source_state.get()->getPayloadAddress(),
         aggregation_handle_min_.get()->getPayloadSize());
  source_hash_table_derived->upsertCompositeKey(exclusive_source_key, buffer);

  memcpy(buffer + 1,
         exclusive_key_destination_state.get()->getPayloadAddress(),
         aggregation_handle_min_.get()->getPayloadSize());
  destination_hash_table_derived->upsertCompositeKey(exclusive_destination_key,
                                                     buffer);

  EXPECT_EQ(2u, destination_hash_table_derived->numEntries());
  EXPECT_EQ(2u, source_hash_table_derived->numEntries());

  HashTableMerger merger(destination_hash_table_derived);
  source_hash_table_derived->forEachCompositeKey(&merger);

  EXPECT_EQ(3u, destination_hash_table_derived->numEntries());

  CheckMinValue<int>(
      common_key_source_min_val.getLiteral<int>(),
      aggregation_handle_min_derived->finalizeHashTableEntry(
          destination_hash_table_derived->getSingleCompositeKey(common_key) +
          1));
  CheckMinValue<int>(
      exclusive_key_destination_min_val.getLiteral<int>(),
      aggregation_handle_min_derived->finalizeHashTableEntry(
          destination_hash_table_derived->getSingleCompositeKey(
              exclusive_destination_key) + 1));
  CheckMinValue<int>(
      exclusive_key_source_min_val.getLiteral<int>(),
      aggregation_handle_min_derived->finalizeHashTableEntry(
          source_hash_table_derived->getSingleCompositeKey(
              exclusive_source_key) + 1));
}

}  // namespace quickstep
