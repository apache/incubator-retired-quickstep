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

#ifndef QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_DATE_EXTRACT_OPERATION_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_DATE_EXTRACT_OPERATION_HPP_

#include <cstdint>
#include <string>

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN
#include <utility>
#include <vector>

#include "storage/StorageBlockInfo.hpp"
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN

#include "catalog/CatalogTypedefs.hpp"
#include "types/LongType.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/Operation.pb.h"
#include "types/operations/unary_operations/UnaryOperation.hpp"
#include "types/operations/unary_operations/UnaryOperationID.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class ColumnVector;
class ValueAccessor;

struct DatetimeLit;

/** \addtogroup Types
 *  @{
 */

enum class DateExtractUnit {
  kYear = 0,
  kMonth,
  kDay,
  kHour,
  kMinute,
  kSecond
};

/**
 * @brief UncheckedUnaryOperator for Datetime Extract.
 */
template <DateExtractUnit unit, bool argument_nullable>
class DatetimeExtractUncheckedOperator : public UncheckedUnaryOperator {
 public:
  DatetimeExtractUncheckedOperator()
      : UncheckedUnaryOperator() {}

  TypedValue applyToTypedValue(const TypedValue &argument) const override;

  TypedValue applyToDataPtr(const void *argument) const override;

  ColumnVector* applyToColumnVector(const ColumnVector &argument) const override;

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  ColumnVector* applyToValueAccessor(ValueAccessor *accessor,
                                     const attribute_id argument_attr_id) const override;
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN
  ColumnVector* applyToValueAccessorForJoin(
      ValueAccessor *accessor,
      const bool use_left_relation,
      const attribute_id argument_attr_id,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const override;
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN

 private:
  inline std::int64_t dateExtract(const DatetimeLit &argument) const;

  DISALLOW_COPY_AND_ASSIGN(DatetimeExtractUncheckedOperator);
};

/**
 * @brief UncheckedUnaryOperator for Date Extract.
 */
template <DateExtractUnit unit, bool argument_nullable>
class DateExtractUncheckedOperator : public UncheckedUnaryOperator {
 public:
  DateExtractUncheckedOperator()
      : UncheckedUnaryOperator() {}

  TypedValue applyToTypedValue(const TypedValue &argument) const override;

  TypedValue applyToDataPtr(const void *argument) const override;

  ColumnVector* applyToColumnVector(const ColumnVector &argument) const override;

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  ColumnVector* applyToValueAccessor(ValueAccessor *accessor,
                                     const attribute_id argument_attr_id) const override;
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN
  ColumnVector* applyToValueAccessorForJoin(
      ValueAccessor *accessor,
      const bool use_left_relation,
      const attribute_id argument_attr_id,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const override;
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN

 private:
  inline std::int32_t dateExtract(const DateLit &argument) const;

  DISALLOW_COPY_AND_ASSIGN(DateExtractUncheckedOperator);
};

/**
 * @brief UnaryOperation for Extract.
 */
class DateExtractOperation : public UnaryOperation {
 public:
  /**
   * @brief Get a reference to the singleton instance of this Operation for a
   *        particular DateExtractUnit.
   *
   * @param unit The date unit to extract.
   * @return A reference to the singleton instance of this Operation for the
   *         specified DateExtractUnit.
   **/
  static const DateExtractOperation& Instance(const DateExtractUnit unit);

  serialization::UnaryOperation getProto() const override;

  std::string getName() const override;

  bool canApplyToType(const Type &type) const override {
    return type.getTypeID() == TypeID::kDatetime || type.getTypeID() == kDate;
  }

  const Type* resultTypeForArgumentType(const Type &type) const override;

  const Type* fixedNullableResultType() const override {
    return nullptr;
  }

  bool resultTypeIsPlausible(const Type &result_type) const override {
    return result_type.getTypeID() == kLong || result_type.getTypeID() == kInt;
  }

  const Type* pushDownTypeHint(const Type *type_hint) const override;

  TypedValue applyToChecked(const TypedValue &argument,
                            const Type &argument_type) const override;

  UncheckedUnaryOperator* makeUncheckedUnaryOperatorForType(const Type &type) const override {
    DCHECK(canApplyToType(type));

    return makeUncheckedUnaryOperatorForTypeHelper(type);
  }

 private:
  explicit DateExtractOperation(const DateExtractUnit unit)
      : UnaryOperation(UnaryOperationID::kDateExtract),
        unit_(unit) {}

  UncheckedUnaryOperator* makeUncheckedUnaryOperatorForTypeHelper(const Type &type) const;

  const DateExtractUnit unit_;

 private:
  DISALLOW_COPY_AND_ASSIGN(DateExtractOperation);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_DATE_EXTRACT_OPERATION_HPP_ */
