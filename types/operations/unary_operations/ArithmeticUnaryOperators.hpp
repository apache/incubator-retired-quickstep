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

#ifndef QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_ARITHMETIC_UNARY_OPERATORS_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_ARITHMETIC_UNARY_OPERATORS_HPP_

#include <cstddef>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/operations/unary_operations/UnaryOperation.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief The UncheckedUnaryOperator for negation.
 **/
template <class ResultType, bool argument_nullable>
class NegateUncheckedUnaryOperator : public UncheckedUnaryOperator {
 public:
  NegateUncheckedUnaryOperator() : UncheckedUnaryOperator() {
  }

  inline TypedValue applyToTypedValue(const TypedValue &argument) const override {
    return applyToTypedValueInl(argument);
  }

  inline TypedValue applyToTypedValueInl(const TypedValue &argument) const {
    if (argument_nullable && argument.isNull()) {
      return argument;
    }
    return TypedValue(-argument.getLiteral<typename ResultType::cpptype>());
  }

  inline TypedValue applyToDataPtr(const void *argument) const override {
    return applyToDataPtrInl(argument);
  }

  inline TypedValue applyToDataPtrInl(const void *argument) const {
    if (argument_nullable && (argument == nullptr)) {
      return TypedValue(ResultType::kStaticTypeID);
    }
    return TypedValue(-*static_cast<const typename ResultType::cpptype*>(argument));
  }

  ColumnVector* applyToColumnVector(const ColumnVector &argument) const override {
    DCHECK(NativeColumnVector::UsableForType(ResultType::Instance(argument_nullable)));
    // All arithmetic types (numbers, datetime, and intervals) are usable with
    // NativeColumnVector, so 'argument' should always be native.
    DCHECK(argument.isNative());
    const NativeColumnVector &native_argument = static_cast<const NativeColumnVector&>(argument);
    NativeColumnVector *result = new NativeColumnVector(
        ResultType::Instance(argument_nullable),
        native_argument.size());
    for (std::size_t pos = 0;
         pos < native_argument.size();
         ++pos) {
      const typename ResultType::cpptype *scalar_arg
          = static_cast<const typename ResultType::cpptype*>(
              native_argument.getUntypedValue<argument_nullable>(pos));
      if (argument_nullable && (scalar_arg == nullptr)) {
        result->appendNullValue();
      } else {
        *static_cast<typename ResultType::cpptype*>(result->getPtrForDirectWrite())
            = -(*scalar_arg);
      }
    }
    return result;
  }

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  ColumnVector* applyToValueAccessor(ValueAccessor *accessor,
                                     const attribute_id argument_attr_id) const override {
    DCHECK(NativeColumnVector::UsableForType(ResultType::Instance(argument_nullable)));
    return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
        accessor,
        [&](auto *accessor) -> ColumnVector* {  // NOLINT(build/c++11)
      NativeColumnVector *result = new NativeColumnVector(
          ResultType::Instance(argument_nullable),
          accessor->getNumTuples());
      accessor->beginIteration();
      while (accessor->next()) {
        const typename ResultType::cpptype *scalar_arg
            = static_cast<const typename ResultType::cpptype*>(
                accessor->template getUntypedValue<argument_nullable>(argument_attr_id));
        if (argument_nullable && (scalar_arg == nullptr)) {
          result->appendNullValue();
        } else {
          *static_cast<typename ResultType::cpptype*>(result->getPtrForDirectWrite())
              = -(*scalar_arg);
        }
      }
      return result;
    });
  }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN
  ColumnVector* applyToValueAccessorForJoin(
      ValueAccessor *accessor,
      const bool use_left_relation,
      const attribute_id argument_attr_id,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const override {
    DCHECK(NativeColumnVector::UsableForType(ResultType::Instance(argument_nullable)));
    NativeColumnVector *result = new NativeColumnVector(ResultType::Instance(argument_nullable),
                                                        joined_tuple_ids.size());
    InvokeOnValueAccessorNotAdapter(
        accessor,
        [&](auto *accessor) -> void {  // NOLINT(build/c++11)
      for (const std::pair<tuple_id, tuple_id> &joined_pair : joined_tuple_ids) {
        const typename ResultType::cpptype *scalar_arg
            = static_cast<const typename ResultType::cpptype*>(
                accessor->template getUntypedValueAtAbsolutePosition<argument_nullable>(
                    argument_attr_id,
                    use_left_relation ? joined_pair.first : joined_pair.second));
        if (argument_nullable && (scalar_arg == nullptr)) {
          result->appendNullValue();
        } else {
          *static_cast<typename ResultType::cpptype*>(result->getPtrForDirectWrite())
              = -(*scalar_arg);
        }
      }
    });
    return result;
  }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN

 private:
  DISALLOW_COPY_AND_ASSIGN(NegateUncheckedUnaryOperator);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_ARITHMETIC_UNARY_OPERATORS_HPP_
