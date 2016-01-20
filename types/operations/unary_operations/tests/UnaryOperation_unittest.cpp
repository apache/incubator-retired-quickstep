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
#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include "types/DoubleType.hpp"
#include "types/FloatType.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/operations/Operation.pb.h"
#include "types/operations/unary_operations/ArithmeticUnaryOperations.hpp"
#include "types/operations/unary_operations/NumericCastOperation.hpp"
#include "types/operations/unary_operations/UnaryOperation.hpp"
#include "types/operations/unary_operations/UnaryOperationFactory.hpp"
#include "types/operations/unary_operations/UnaryOperationID.hpp"
#include "utility/Macros.hpp"

#include "gtest/gtest.h"

using std::int64_t;
using std::numeric_limits;
using std::pair;
using std::unique_ptr;
using std::vector;

namespace quickstep {

class ArithmeticUnaryOperationTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    int_null_.reset(new TypedValue(kInt));
    int_zero_.reset(new TypedValue(0));
    int_positive_.reset(new TypedValue(12345));
    int_negative_.reset(new TypedValue(- int_positive_->getLiteral<int>()));
    int_max_.reset(new TypedValue(numeric_limits<int>::max()));
    int_min_.reset(new TypedValue(numeric_limits<int>::min()));

    numeric_typed_values_.emplace_back(int_null_.get(),
                                       &TypeFactory::GetType(kInt, true));
    numeric_typed_values_.emplace_back(int_zero_.get(),
                                       &TypeFactory::GetType(kInt, false));
    numeric_typed_values_.emplace_back(int_positive_.get(),
                                       &TypeFactory::GetType(kInt, false));
    numeric_typed_values_.emplace_back(int_negative_.get(),
                                       &TypeFactory::GetType(kInt, false));
    numeric_typed_values_.emplace_back(int_max_.get(),
                                       &TypeFactory::GetType(kInt, false));
    numeric_typed_values_.emplace_back(int_min_.get(),
                                       &TypeFactory::GetType(kInt, false));

    long_null_.reset(new TypedValue(kLong));
    long_zero_.reset(new TypedValue(static_cast<int64_t>(0)));
    long_positive_.reset(new TypedValue(static_cast<int64_t>(12345)));
    long_negative_.reset(new TypedValue(- long_positive_->getLiteral<int64_t>()));
    long_max_.reset(new TypedValue(numeric_limits<int64_t>::max()));
    long_min_.reset(new TypedValue(numeric_limits<int64_t>::min()));

    numeric_typed_values_.emplace_back(long_null_.get(),
                                       &TypeFactory::GetType(kLong, true));
    numeric_typed_values_.emplace_back(long_zero_.get(),
                                       &TypeFactory::GetType(kLong, false));
    numeric_typed_values_.emplace_back(long_positive_.get(),
                                       &TypeFactory::GetType(kLong, false));
    numeric_typed_values_.emplace_back(long_negative_.get(),
                                       &TypeFactory::GetType(kLong, false));
    numeric_typed_values_.emplace_back(long_max_.get(),
                                       &TypeFactory::GetType(kLong, false));
    numeric_typed_values_.emplace_back(long_min_.get(),
                                       &TypeFactory::GetType(kLong, false));

    float_null_.reset(new TypedValue(kFloat));
    float_zero_.reset(new TypedValue(static_cast<float>(0.0)));
    float_positive_.reset(new TypedValue(static_cast<float>(123.45)));
    float_negative_.reset(new TypedValue(- float_positive_->getLiteral<float>()));
    float_max_.reset(new TypedValue(numeric_limits<float>::max()));
    float_min_.reset(new TypedValue(numeric_limits<float>::min()));

    numeric_typed_values_.emplace_back(float_null_.get(),
                                       &TypeFactory::GetType(kFloat, true));
    numeric_typed_values_.emplace_back(float_zero_.get(),
                                       &TypeFactory::GetType(kFloat, false));
    numeric_typed_values_.emplace_back(float_positive_.get(),
                                       &TypeFactory::GetType(kFloat, false));
    numeric_typed_values_.emplace_back(float_negative_.get(),
                                       &TypeFactory::GetType(kFloat, false));
    numeric_typed_values_.emplace_back(float_max_.get(),
                                       &TypeFactory::GetType(kFloat, false));
    numeric_typed_values_.emplace_back(float_min_.get(),
                                       &TypeFactory::GetType(kFloat, false));

    double_null_.reset(new TypedValue(kDouble));
    double_zero_.reset(new TypedValue(static_cast<double>(0)));
    double_positive_.reset(new TypedValue(static_cast<double>(123.45)));
    double_negative_.reset(new TypedValue(- double_positive_->getLiteral<double>()));
    double_max_.reset(new TypedValue(numeric_limits<double>::max()));
    double_min_.reset(new TypedValue(numeric_limits<double>::min()));

    numeric_typed_values_.emplace_back(double_null_.get(),
                                       &TypeFactory::GetType(kDouble, true));
    numeric_typed_values_.emplace_back(double_zero_.get(),
                                       &TypeFactory::GetType(kDouble, false));
    numeric_typed_values_.emplace_back(double_positive_.get(),
                                       &TypeFactory::GetType(kDouble, false));
    numeric_typed_values_.emplace_back(double_negative_.get(),
                                       &TypeFactory::GetType(kDouble, false));
    numeric_typed_values_.emplace_back(double_max_.get(),
                                       &TypeFactory::GetType(kDouble, false));
    numeric_typed_values_.emplace_back(double_min_.get(),
                                       &TypeFactory::GetType(kDouble, false));
  }

  void checkUnaryOperation(const ArithmeticUnaryOperation &operation) {
    checkArithmeticUnaryOperation(operation);
  }

 private:
  void checkArithmeticUnaryOperation(const ArithmeticUnaryOperation &operation) {
    for (const pair<const TypedValue*, const Type*> &item : numeric_typed_values_) {
      checkArithmeticUnaryOperationChecked(operation, *item.first, *item.second);
      checkNumericUnaryOperatorUnchecked(operation, *item.first, *item.second);
    }
  }

  void checkArithmeticUnaryOperationChecked(const ArithmeticUnaryOperation &operation,
                                            const TypedValue &checked,
                                            const Type &type) {
    if (checked.isNull()) {
      EXPECT_TRUE(operation.applyToChecked(checked, type).isNull());
      return;
    }

    const Type *expected_type = operation.resultTypeForArgumentType(type);
    ASSERT_NE(expected_type, nullptr);
    EXPECT_TRUE(expected_type->equals(type));

    switch (expected_type->getTypeID()) {
      case kInt:
        checkArithmeticUnaryOperationCheckedHelper<int>(operation, checked, type);
        break;
      case kLong:
        checkArithmeticUnaryOperationCheckedHelper<int64_t>(operation, checked, type);
        break;
      case kFloat:
        checkArithmeticUnaryOperationCheckedHelper<float>(operation, checked, type);
        break;
      case kDouble:
        checkArithmeticUnaryOperationCheckedHelper<double>(operation, checked, type);
        break;
      default:
        FATAL_ERROR("Unsupported result type from one operand.");
    }
  }

  template <typename NumericType>
  void checkArithmeticUnaryOperationCheckedHelper(const ArithmeticUnaryOperation &operation,
                                                  const TypedValue &checked,
                                                  const Type &type) {
    NumericType expected = checkArithmeticUnaryOperationHelper<NumericType>(operation, checked);
    EXPECT_EQ(expected, operation.applyToChecked(checked, type).getLiteral<NumericType>());
  }

  void checkNumericUnaryOperatorUnchecked(const ArithmeticUnaryOperation &operation,
                                          const TypedValue &checked,
                                          const Type &type) {
    unique_ptr<UncheckedUnaryOperator> unary_operator(
        operation.makeUncheckedUnaryOperatorForType(type));

    if (checked.isNull()) {
      EXPECT_TRUE(unary_operator->applyToTypedValue(checked).isNull());
      EXPECT_TRUE(unary_operator->applyToDataPtr(nullptr).isNull());
      return;
    }

    const Type *expected_type = operation.resultTypeForArgumentType(type);
    ASSERT_NE(expected_type, nullptr);
    EXPECT_TRUE(expected_type->equals(type));

    switch (expected_type->getTypeID()) {
      case kInt:
        checkNumericUnaryOperatorUncheckedHelper<int>(operation, *unary_operator, checked);
        break;
      case kLong:
        checkNumericUnaryOperatorUncheckedHelper<int64_t>(operation, *unary_operator, checked);
        break;
      case kFloat:
        checkNumericUnaryOperatorUncheckedHelper<float>(operation, *unary_operator, checked);
        break;
      case kDouble:
        checkNumericUnaryOperatorUncheckedHelper<double>(operation, *unary_operator, checked);
        break;
      default:
        FATAL_ERROR("Unsupported result type from one operand.");
    }
  }

  template <typename NumericType>
  void checkNumericUnaryOperatorUncheckedHelper(const ArithmeticUnaryOperation &operation,
                                                const UncheckedUnaryOperator &unary_operator,
                                                const TypedValue &checked) {
    NumericType expected = checkArithmeticUnaryOperationHelper<NumericType>(operation, checked);
    EXPECT_EQ(expected, unary_operator.applyToTypedValue(checked).getLiteral<NumericType>());
    EXPECT_EQ(expected, unary_operator.applyToDataPtr(checked.getDataPtr()).getLiteral<NumericType>());
  }

  template <typename NumericType>
  NumericType checkArithmeticUnaryOperationHelper(const ArithmeticUnaryOperation &operation,
                                                  const TypedValue &checked) {
    switch (operation.getUnaryOperationID()) {
      case UnaryOperationID::kNegate:
        return -checked.getLiteral<NumericType>();
      default:
        FATAL_ERROR("Unsupported numeric unary operation.");
    }
  }

  unique_ptr<TypedValue>
      int_null_, int_zero_, int_positive_, int_negative_, int_max_, int_min_,
      long_null_, long_zero_, long_positive_, long_negative_, long_max_, long_min_,
      float_null_, float_zero_, float_positive_, float_negative_, float_max_, float_min_,
      double_null_, double_zero_, double_positive_, double_negative_, double_max_, double_min_;

  vector<pair<const TypedValue*, const Type*>> numeric_typed_values_;
};

TEST_F(ArithmeticUnaryOperationTest, CheckedNegateUnaryOperationTest) {
  checkUnaryOperation(NegateUnaryOperation::Instance());
}

void CheckNegateOperationSerialization(const UnaryOperation &operation) {
  serialization::UnaryOperation proto = operation.getProto();
  EXPECT_EQ(serialization::UnaryOperation::NEGATE, proto.operation_id());
  EXPECT_TRUE(operation.equals(UnaryOperationFactory::ReconstructFromProto(proto)));
}

TEST(NegateOperationTest, ProtoSerializationTest) {
  CheckNegateOperationSerialization(UnaryOperationFactory::GetUnaryOperation(UnaryOperationID::kNegate));
}


template <class TemplateSourceType, class TemplateTargetType>
struct CastOperationTestTypeTraits {
  typedef TemplateSourceType SourceType;
  typedef TemplateTargetType TargetType;
};

template <class CastOperationTestTypeTraits>
class NumericCastOperationTest : public ::testing::Test {
 protected:
  typedef typename CastOperationTestTypeTraits::SourceType SourceType;
  typedef typename CastOperationTestTypeTraits::TargetType TargetType;
  typedef typename CastOperationTestTypeTraits::SourceType::cpptype source_cpp_type;
  typedef typename CastOperationTestTypeTraits::TargetType::cpptype target_cpp_type;

  NumericCastOperationTest()
      : test_values_({std::numeric_limits<source_cpp_type>::max(),
                      std::numeric_limits<source_cpp_type>::min(),
                      0,
                      static_cast<source_cpp_type>(2.718281828)}),
        source_type_instance_(SourceType::InstanceNullable()),
        target_type_instance_(TargetType::InstanceNullable()),
        cast_operation_(NumericCastOperation::Instance(target_type_instance_)),
        unchecked_cast_operator_(
            cast_operation_.makeUncheckedUnaryOperatorForType(source_type_instance_)) {}

  virtual ~NumericCastOperationTest() {}

  // The maximum value, the minimum value, zero, and the euler number.
  std::vector<source_cpp_type> test_values_;

  const Type &source_type_instance_;
  const Type &target_type_instance_;

  const NumericCastOperation &cast_operation_;
  unique_ptr<UncheckedUnaryOperator> unchecked_cast_operator_;

 private:
  DISALLOW_COPY_AND_ASSIGN(NumericCastOperationTest);
};

typedef ::testing::Types<CastOperationTestTypeTraits<IntType, IntType>,
                         CastOperationTestTypeTraits<IntType, LongType>,
                         CastOperationTestTypeTraits<IntType, FloatType>,
                         CastOperationTestTypeTraits<IntType, DoubleType>,
                         CastOperationTestTypeTraits<LongType, IntType>,
                         CastOperationTestTypeTraits<LongType, LongType>,
                         CastOperationTestTypeTraits<LongType, FloatType>,
                         CastOperationTestTypeTraits<LongType, DoubleType>,
                         CastOperationTestTypeTraits<FloatType, IntType>,
                         CastOperationTestTypeTraits<FloatType, LongType>,
                         CastOperationTestTypeTraits<FloatType, FloatType>,
                         CastOperationTestTypeTraits<FloatType, DoubleType>,
                         CastOperationTestTypeTraits<DoubleType, IntType>,
                         CastOperationTestTypeTraits<DoubleType, LongType>,
                         CastOperationTestTypeTraits<DoubleType, FloatType>,
                         CastOperationTestTypeTraits<DoubleType, DoubleType>> CastOperationTestTypes;
TYPED_TEST_CASE(NumericCastOperationTest, CastOperationTestTypes);

TYPED_TEST(NumericCastOperationTest, CheckedNumericCastOperationTest) {
  EXPECT_TRUE(this->cast_operation_.canApplyToType(this->source_type_instance_));

  const Type *result_type = this->cast_operation_.resultTypeForArgumentType(this->source_type_instance_);
  ASSERT_TRUE(result_type != nullptr);
  EXPECT_TRUE(this->target_type_instance_.equals(*result_type));

  for (const typename TestFixture::source_cpp_type test_value : this->test_values_) {
    const TypedValue input_typed_value(test_value);
    const TypedValue output_typed_value =
        this->cast_operation_.applyToChecked(input_typed_value,
                                             this->source_type_instance_);
    EXPECT_EQ(static_cast<typename TestFixture::target_cpp_type>(test_value),
              output_typed_value.getLiteral<typename TestFixture::target_cpp_type>());
  }

  const TypedValue null_input(TestFixture::SourceType::kStaticTypeID);
  const TypedValue output_value_from_null_input =
      this->cast_operation_.applyToChecked(null_input,
                                           this->source_type_instance_);
  EXPECT_EQ(TestFixture::TargetType::kStaticTypeID,
            output_value_from_null_input.getTypeID());
  EXPECT_TRUE(output_value_from_null_input.isNull());
}

TYPED_TEST(NumericCastOperationTest, ApplyToTypedValueTest) {
  for (const typename TestFixture::source_cpp_type test_value : this->test_values_) {
    const TypedValue input_typed_value(test_value);
    const TypedValue output_typed_value =
        this->unchecked_cast_operator_->applyToTypedValue(input_typed_value);
    EXPECT_EQ(static_cast<typename TestFixture::target_cpp_type>(test_value),
              output_typed_value.getLiteral<typename TestFixture::target_cpp_type>());
  }

  const TypedValue null_input(TestFixture::SourceType::kStaticTypeID);
  const TypedValue output_value_from_null_input =
      this->unchecked_cast_operator_->applyToTypedValue(null_input);
  EXPECT_EQ(TestFixture::TargetType::kStaticTypeID,
            output_value_from_null_input.getTypeID());
  EXPECT_TRUE(output_value_from_null_input.isNull());
}

TYPED_TEST(NumericCastOperationTest, ApplyToDataPtrTest) {
  for (const typename TestFixture::source_cpp_type test_value : this->test_values_) {
    const TypedValue output_typed_value =
        this->unchecked_cast_operator_->applyToDataPtr(&test_value);
    EXPECT_EQ(static_cast<typename TestFixture::target_cpp_type>(test_value),
              output_typed_value.getLiteral<typename TestFixture::target_cpp_type>());
  }

  const TypedValue output_value_from_null_input =
      this->unchecked_cast_operator_->applyToDataPtr(nullptr);
  EXPECT_EQ(TestFixture::TargetType::kStaticTypeID,
            output_value_from_null_input.getTypeID());
  EXPECT_TRUE(output_value_from_null_input.isNull());
}

TYPED_TEST(NumericCastOperationTest, ApplyToColumnVectorTest) {
  NativeColumnVector column_vector(this->source_type_instance_, this->test_values_.size() + 1);
  for (const typename TestFixture::source_cpp_type test_value : this->test_values_) {
    column_vector.appendUntypedValue(&test_value);
  }
  column_vector.appendNullValue();

  unique_ptr<ColumnVector> result_column_vector(
      this->unchecked_cast_operator_->applyToColumnVector(column_vector));
  ASSERT_TRUE(result_column_vector->isNative());

  const NativeColumnVector *typed_result_column_vector =
      static_cast<const NativeColumnVector*>(result_column_vector.get());
  ASSERT_EQ(typed_result_column_vector->size(), this->test_values_.size() + 1);
  std::size_t pos;
  for (pos = 0; pos < this->test_values_.size(); ++pos) {
    EXPECT_EQ(static_cast<typename TestFixture::target_cpp_type>(this->test_values_[pos]),
              *static_cast<const typename TestFixture::target_cpp_type*>(
                  typed_result_column_vector->getUntypedValue<true>(pos)));
  }
  EXPECT_TRUE(typed_result_column_vector->getUntypedValue(pos) == nullptr);
}

TYPED_TEST(NumericCastOperationTest, ProtoSerializationTest) {
  serialization::UnaryOperation proto = this->cast_operation_.getProto();
  EXPECT_EQ(serialization::UnaryOperation::CAST, proto.operation_id());
  EXPECT_TRUE(UnaryOperationFactory::ProtoIsValid(proto));
  EXPECT_TRUE(this->cast_operation_.equals(UnaryOperationFactory::ReconstructFromProto(proto)));
}

}  // namespace quickstep
