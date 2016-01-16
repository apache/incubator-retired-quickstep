/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/aggregation/AggregateFunction.hpp"
#include "expressions/aggregation/AggregateFunctionFactory.hpp"
#include "expressions/aggregation/AggregationHandle.hpp"
#include "expressions/aggregation/AggregationHandleCount.hpp"
#include "expressions/aggregation/AggregationID.hpp"
#include "types/CharType.hpp"
#include "types/DoubleType.hpp"
#include "types/FloatType.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/VarCharType.hpp"
#include "types/containers/ColumnVector.hpp"

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#endif

#include "gtest/gtest.h"

namespace quickstep {

class AggregationHandleCountTest : public::testing::Test {
 protected:
  const Type &dummy_type = TypeFactory::GetType(kInt);

  void iterateHandleNullary(AggregationState *state) {
    static_cast<const AggregationHandleCount<true, false>&>(
        *aggregation_handle_count_).iterateNullaryInl(
            static_cast<AggregationStateCount*>(state));
  }

  // Helper method that calls AggregationHandleCount::iterateUnaryInl() to
  // aggregate 'value' into '*state'.
  void iterateHandle(AggregationState *state, const TypedValue &value) {
    static_cast<const AggregationHandleCount<false, true>&>(
        *aggregation_handle_count_).iterateUnaryInl(
            static_cast<AggregationStateCount*>(state),
            value);
  }

  void initializeHandle(const Type *argument_type) {
    if (argument_type == nullptr) {
      aggregation_handle_count_.reset(
          AggregateFunctionFactory::Get(AggregationID::kCount).createHandle(
              std::vector<const Type*>()));
    } else {
      aggregation_handle_count_.reset(
          AggregateFunctionFactory::Get(AggregationID::kCount).createHandle(
              std::vector<const Type*>(1, argument_type)));
    }
    aggregation_handle_count_state_.reset(
        aggregation_handle_count_->createInitialState());
  }

  static bool ApplyToTypesTest(TypeID typeID) {
    const Type &type = (typeID == kChar || typeID == kVarChar) ?
        TypeFactory::GetType(typeID, static_cast<std::size_t>(10)) :
        TypeFactory::GetType(typeID);

    return AggregateFunctionFactory::Get(AggregationID::kCount).canApplyToTypes(
        std::vector<const Type*>(1, &type));
  }

  static bool ResultTypeForArgumentTypeTest(TypeID input_type_id,
                                            TypeID output_type_id) {
    const Type *result_type
        = AggregateFunctionFactory::Get(AggregationID::kCount).resultTypeForArgumentTypes(
            std::vector<const Type*>(1, &TypeFactory::GetType(input_type_id)));
    return (result_type->getTypeID() == output_type_id);
  }

  static void CheckCountValue(
      std::int64_t expected,
      const AggregationHandle &handle,
      const AggregationState &state) {
    EXPECT_EQ(expected, handle.finalize(state).getLiteral<std::int64_t>());
  }

  void checkAggregationCountNullary(int test_count) {
    initializeHandle(nullptr);
    CheckCountValue(0, *aggregation_handle_count_, *aggregation_handle_count_state_);

    for (int i = 0; i < test_count; ++i) {
      iterateHandleNullary(aggregation_handle_count_state_.get());
    }
    CheckCountValue(test_count, *aggregation_handle_count_, *aggregation_handle_count_state_);

    // Test mergeStates.
    std::unique_ptr<AggregationState> merge_state(
        aggregation_handle_count_->createInitialState());
    aggregation_handle_count_->mergeStates(*merge_state,
                                           aggregation_handle_count_state_.get());

    for (int i = 0; i < test_count; ++i) {
      iterateHandleNullary(merge_state.get());
    }

    aggregation_handle_count_->mergeStates(*merge_state,
                                           aggregation_handle_count_state_.get());
    CheckCountValue(2 * test_count, *aggregation_handle_count_, *aggregation_handle_count_state_);
  }

  void checkAggregationCountNullaryAccumulate(int test_count) {
    initializeHandle(nullptr);

    std::unique_ptr<AggregationState> accumulated_state(
        aggregation_handle_count_->accumulateNullary(test_count));

    // Test the state generated directly by accumulateNullary(), and also test
    // after merging back.
    CheckCountValue(test_count,
                    *aggregation_handle_count_,
                    *accumulated_state);

    aggregation_handle_count_->mergeStates(*accumulated_state,
                                           aggregation_handle_count_state_.get());
    CheckCountValue(test_count,
                    *aggregation_handle_count_,
                    *aggregation_handle_count_state_);
  }

  template <typename NumericType>
  void checkAggregationCountNumeric(int test_count) {
    const NumericType &type = NumericType::Instance(true);
    initializeHandle(&type);
    CheckCountValue(0, *aggregation_handle_count_, *aggregation_handle_count_state_);

    typename NumericType::cpptype val = 0;
    int count = 0;

    iterateHandle(aggregation_handle_count_state_.get(), type.makeNullValue());
    for (int i = 0; i < test_count; ++i) {
      iterateHandle(aggregation_handle_count_state_.get(), type.makeValue(&val));
      ++count;
    }
    iterateHandle(aggregation_handle_count_state_.get(), type.makeNullValue());
    CheckCountValue(count, *aggregation_handle_count_, *aggregation_handle_count_state_);

    // Test mergeStates.
    std::unique_ptr<AggregationState> merge_state(
        aggregation_handle_count_->createInitialState());
    aggregation_handle_count_->mergeStates(*merge_state,
                                           aggregation_handle_count_state_.get());

    iterateHandle(merge_state.get(), type.makeNullValue());
    for (int i = 0; i < test_count; ++i) {
      iterateHandle(merge_state.get(), type.makeValue(&val));
      ++count;
    }
    iterateHandle(merge_state.get(), type.makeNullValue());

    aggregation_handle_count_->mergeStates(*merge_state,
                                           aggregation_handle_count_state_.get());
    CheckCountValue(count, *aggregation_handle_count_, *aggregation_handle_count_state_);
  }

  template <typename NumericType>
  ColumnVector *createColumnVectorNumeric(const Type &type, int test_count) {
    NativeColumnVector *column = new NativeColumnVector(type, test_count + 3);

    typename NumericType::cpptype val = 0;
    column->appendTypedValue(type.makeNullValue());
    for (int i = 0; i < test_count; ++i) {
      column->appendTypedValue(type.makeValue(&val));
      // One NULL in the middle.
      if (i == test_count/2) {
        column->appendTypedValue(type.makeNullValue());
      }
    }
    column->appendTypedValue(type.makeNullValue());
    return column;
  }

  template <typename NumericType>
  void checkAggregationCountNumericColumnVector(int test_count) {
    const NumericType &type = NumericType::Instance(true);
    initializeHandle(&type);
    CheckCountValue(0, *aggregation_handle_count_, *aggregation_handle_count_state_);

    std::vector<std::unique_ptr<ColumnVector>> column_vectors;
    column_vectors.emplace_back(createColumnVectorNumeric<NumericType>(type, test_count));

    std::unique_ptr<AggregationState> cv_state(
        aggregation_handle_count_->accumulateColumnVectors(column_vectors));

    // Test the state generated directly by accumulateColumnVectors(), and also
    // test after merging back.
    CheckCountValue(test_count,
                    *aggregation_handle_count_,
                    *cv_state);

    aggregation_handle_count_->mergeStates(*cv_state, aggregation_handle_count_state_.get());
    CheckCountValue(test_count,
                    *aggregation_handle_count_,
                    *aggregation_handle_count_state_);
  }

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  template <typename NumericType>
  void checkAggregationCountNumericValueAccessor(int test_count) {
    const NumericType &type = NumericType::Instance(true);
    initializeHandle(&type);
    CheckCountValue(0, *aggregation_handle_count_, *aggregation_handle_count_state_);

    std::unique_ptr<ColumnVectorsValueAccessor> accessor(new ColumnVectorsValueAccessor());
    accessor->addColumn(createColumnVectorNumeric<NumericType>(type, test_count));

    std::unique_ptr<AggregationState> va_state(
        aggregation_handle_count_->accumulateValueAccessor(accessor.get(),
                                                           std::vector<attribute_id>(1, 0)));

    // Test the state generated directly by accumulateValueAccessor(), and also
    // test after merging back.
    CheckCountValue(test_count,
                    *aggregation_handle_count_,
                    *va_state);

    aggregation_handle_count_->mergeStates(*va_state, aggregation_handle_count_state_.get());
    CheckCountValue(test_count,
                    *aggregation_handle_count_,
                    *aggregation_handle_count_state_);
  }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

  template <typename StringType>
  void checkAggregationCountString(int test_count) {
    const StringType &type = StringType::Instance(10, true);
    initializeHandle(&type);
    CheckCountValue(0, *aggregation_handle_count_, *aggregation_handle_count_state_);

    std::string string_literal = "test_str";
    int count = 0;

    iterateHandle(aggregation_handle_count_state_.get(), type.makeNullValue());
    for (int i = 0; i < test_count; ++i) {
      iterateHandle(aggregation_handle_count_state_.get(),
                    type.makeValue(string_literal.c_str(), 10));
      ++count;
    }
    iterateHandle(aggregation_handle_count_state_.get(), type.makeNullValue());
    CheckCountValue(count, *aggregation_handle_count_, *aggregation_handle_count_state_);

    // Test mergeStates().
    std::unique_ptr<AggregationState> merge_state(
        aggregation_handle_count_->createInitialState());

    iterateHandle(merge_state.get(), type.makeNullValue());
    for (int i = 0; i < test_count; ++i) {
      iterateHandle(merge_state.get(), type.makeValue(string_literal.c_str(), 10));
      ++count;
    }
    iterateHandle(merge_state.get(), type.makeNullValue());

    aggregation_handle_count_->mergeStates(*merge_state,
                                           aggregation_handle_count_state_.get());
    CheckCountValue(count, *aggregation_handle_count_, *aggregation_handle_count_state_);
  }

  template <typename ColumnVectorType>
  ColumnVector *createColumnVectorString(const Type &type, int test_count) {
    ColumnVectorType *column = new ColumnVectorType(type, test_count + 3);

    std::string string_literal = "test_str";
    column->appendTypedValue(type.makeNullValue());
    for (int i = 0; i < test_count; ++i) {
      column->appendTypedValue(type.makeValue(string_literal.c_str(), 10));
      // One NULL in the middle.
      if (i == test_count/2) {
        column->appendTypedValue(type.makeNullValue());
      }
    }
    column->appendTypedValue(type.makeNullValue());

    return column;
  }

  template <typename StringType, typename ColumnVectorType>
  void checkAggregationCountStringColumnVector(int test_count) {
    const StringType &type = StringType::Instance(10, true);
    initializeHandle(&type);
    CheckCountValue(0, *aggregation_handle_count_, *aggregation_handle_count_state_);

    std::vector<std::unique_ptr<ColumnVector>> column_vectors;
    column_vectors.emplace_back(createColumnVectorString<ColumnVectorType>(type, test_count));

    std::unique_ptr<AggregationState> cv_state(
        aggregation_handle_count_->accumulateColumnVectors(column_vectors));

    // Test the state generated directly by accumulateColumnVectors(), and also
    // test after merging back.
    CheckCountValue(test_count,
                    *aggregation_handle_count_,
                    *cv_state);

    aggregation_handle_count_->mergeStates(*cv_state, aggregation_handle_count_state_.get());
    CheckCountValue(test_count,
                    *aggregation_handle_count_,
                    *aggregation_handle_count_state_);
  }

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  template <typename StringType, typename ColumnVectorType>
  void checkAggregationCountStringValueAccessor(int test_count) {
    const StringType &type = StringType::Instance(10, true);
    initializeHandle(&type);
    CheckCountValue(0, *aggregation_handle_count_, *aggregation_handle_count_state_);

    std::unique_ptr<ColumnVectorsValueAccessor> accessor(new ColumnVectorsValueAccessor());
    accessor->addColumn(createColumnVectorString<ColumnVectorType>(type, test_count));

    std::unique_ptr<AggregationState> va_state(
        aggregation_handle_count_->accumulateValueAccessor(accessor.get(),
                                                           std::vector<attribute_id>(1, 0)));

    // Test the state generated directly by accumulateValueAccessor(), and also
    // test after merging back.
    CheckCountValue(test_count,
                    *aggregation_handle_count_,
                    *va_state);

    aggregation_handle_count_->mergeStates(*va_state, aggregation_handle_count_state_.get());
    CheckCountValue(test_count,
                    *aggregation_handle_count_,
                    *aggregation_handle_count_state_);
  }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

  std::unique_ptr<AggregationHandle> aggregation_handle_count_;
  std::unique_ptr<AggregationState> aggregation_handle_count_state_;
};

typedef AggregationHandleCountTest AggregationHandleCountDeathTest;

TEST_F(AggregationHandleCountTest, CountStarTest) {
  checkAggregationCountNullary(0),
  checkAggregationCountNullary(10000);
}

TEST_F(AggregationHandleCountTest, CountStarAccumulateTest) {
  checkAggregationCountNullaryAccumulate(0),
  checkAggregationCountNullaryAccumulate(10000);
}

TEST_F(AggregationHandleCountTest, IntTypeTest) {
  checkAggregationCountNumeric<IntType>(0);
  checkAggregationCountNumeric<IntType>(10000);
}

TEST_F(AggregationHandleCountTest, LongTypeTest) {
  checkAggregationCountNumeric<LongType>(0);
  checkAggregationCountNumeric<LongType>(10000);
}

TEST_F(AggregationHandleCountTest, FloatTypeTest) {
  checkAggregationCountNumeric<FloatType>(0);
  checkAggregationCountNumeric<FloatType>(10000);
}

TEST_F(AggregationHandleCountTest, DoubleTypeTest) {
  checkAggregationCountNumeric<DoubleType>(0);
  checkAggregationCountNumeric<DoubleType>(10000);
}

TEST_F(AggregationHandleCountTest, CharTypeTest) {
  checkAggregationCountString<CharType>(0);
  checkAggregationCountString<CharType>(10000);
}

TEST_F(AggregationHandleCountTest, VarCharTypeTest) {
  checkAggregationCountString<VarCharType>(0);
  checkAggregationCountString<VarCharType>(10000);
}

TEST_F(AggregationHandleCountTest, IntTypeColumnVectorTest) {
  checkAggregationCountNumericColumnVector<IntType>(0);
  checkAggregationCountNumericColumnVector<IntType>(10000);
}

TEST_F(AggregationHandleCountTest, LongTypeColumnVectorTest) {
  checkAggregationCountNumericColumnVector<LongType>(0);
  checkAggregationCountNumericColumnVector<LongType>(10000);
}

TEST_F(AggregationHandleCountTest, FloatTypeColumnVectorTest) {
  checkAggregationCountNumericColumnVector<FloatType>(0);
  checkAggregationCountNumericColumnVector<FloatType>(10000);
}

TEST_F(AggregationHandleCountTest, DoubleTypeColumnVectorTest) {
  checkAggregationCountNumericColumnVector<DoubleType>(0);
  checkAggregationCountNumericColumnVector<DoubleType>(10000);
}

TEST_F(AggregationHandleCountTest, CharTypeColumnVectorTest) {
  checkAggregationCountStringColumnVector<CharType, NativeColumnVector>(0);
  checkAggregationCountStringColumnVector<CharType, NativeColumnVector>(10000);
}

TEST_F(AggregationHandleCountTest, VarCharTypeColumnVectorTest) {
  checkAggregationCountStringColumnVector<VarCharType, IndirectColumnVector>(0);
  checkAggregationCountStringColumnVector<VarCharType, IndirectColumnVector>(10000);
}

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
TEST_F(AggregationHandleCountTest, IntTypeValueAccessorTest) {
  checkAggregationCountNumericValueAccessor<IntType>(0);
  checkAggregationCountNumericValueAccessor<IntType>(10000);
}

TEST_F(AggregationHandleCountTest, LongTypeValueAccessorTest) {
  checkAggregationCountNumericValueAccessor<LongType>(0);
  checkAggregationCountNumericValueAccessor<LongType>(10000);
}

TEST_F(AggregationHandleCountTest, FloatTypeValueAccessorTest) {
  checkAggregationCountNumericValueAccessor<FloatType>(0);
  checkAggregationCountNumericValueAccessor<FloatType>(10000);
}

TEST_F(AggregationHandleCountTest, DoubleTypeValueAccessorTest) {
  checkAggregationCountNumericValueAccessor<DoubleType>(0);
  checkAggregationCountNumericValueAccessor<DoubleType>(10000);
}

TEST_F(AggregationHandleCountTest, CharTypeValueAccessorTest) {
  checkAggregationCountStringValueAccessor<CharType, NativeColumnVector>(0);
  checkAggregationCountStringValueAccessor<CharType, NativeColumnVector>(10000);
}

TEST_F(AggregationHandleCountTest, VarCharTypeValueAccessorTest) {
  checkAggregationCountStringValueAccessor<VarCharType, IndirectColumnVector>(0);
  checkAggregationCountStringValueAccessor<VarCharType, IndirectColumnVector>(10000);
}
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

TEST_F(AggregationHandleCountTest, canApplyToTypeTest) {
  EXPECT_TRUE(ApplyToTypesTest(kInt));
  EXPECT_TRUE(ApplyToTypesTest(kLong));
  EXPECT_TRUE(ApplyToTypesTest(kFloat));
  EXPECT_TRUE(ApplyToTypesTest(kDouble));
  EXPECT_TRUE(ApplyToTypesTest(kChar));
  EXPECT_TRUE(ApplyToTypesTest(kVarChar));
}

TEST_F(AggregationHandleCountTest, ResultTypeForArgumentTypeTest) {
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kInt, kLong));
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kLong, kLong));
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kFloat, kLong));
  EXPECT_TRUE(ResultTypeForArgumentTypeTest(kDouble, kLong));
}

}  // namespace quickstep

