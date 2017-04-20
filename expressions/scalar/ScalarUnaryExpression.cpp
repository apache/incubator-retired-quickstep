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

#include "expressions/scalar/ScalarUnaryExpression.hpp"

#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/Expressions.pb.h"
#include "storage/ValueAccessor.hpp"
#include "types/Type.hpp"
#include "types/TypeErrors.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/operations/Operation.pb.h"
#include "types/operations/unary_operations/UnaryOperation.hpp"
#include "types/operations/unary_operations/UnaryOperationID.hpp"

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN
#include "glog/logging.h"
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN

namespace quickstep {

struct SubBlocksReference;

ScalarUnaryExpression::ScalarUnaryExpression(const UnaryOperation &operation,
                                             Scalar *operand)
    : Scalar(*operation.resultTypeForArgumentType(operand->getType())),
      operation_(operation),
      operand_(operand) {
  initHelper(false);
}

serialization::Scalar ScalarUnaryExpression::getProto() const {
  serialization::Scalar proto;
  proto.set_data_source(serialization::Scalar::UNARY_EXPRESSION);
  proto.MutableExtension(serialization::ScalarUnaryExpression::operation)->CopyFrom(operation_.getProto());
  proto.MutableExtension(serialization::ScalarUnaryExpression::operand)->CopyFrom(operand_->getProto());

  return proto;
}

Scalar* ScalarUnaryExpression::clone() const {
  return new ScalarUnaryExpression(operation_, operand_->clone());
}

TypedValue ScalarUnaryExpression::getValueForSingleTuple(const ValueAccessor &accessor,
                                                         const tuple_id tuple) const {
  if (fast_operator_.get() == nullptr) {
    return static_value_.makeReferenceToThis();
  } else {
    return fast_operator_->applyToTypedValue(operand_->getValueForSingleTuple(accessor, tuple));
  }
}

TypedValue ScalarUnaryExpression::getValueForJoinedTuples(
    const ValueAccessor &left_accessor,
    const relation_id left_relation_id,
    const tuple_id left_tuple_id,
    const ValueAccessor &right_accessor,
    const relation_id right_relation_id,
    const tuple_id right_tuple_id) const {
  if (fast_operator_.get() == nullptr) {
    return static_value_.makeReferenceToThis();
  } else {
    return fast_operator_->applyToTypedValue(operand_->getValueForJoinedTuples(left_accessor,
                                                                               left_relation_id,
                                                                               left_tuple_id,
                                                                               right_accessor,
                                                                               right_relation_id,
                                                                               right_tuple_id));
  }
}

ColumnVectorPtr ScalarUnaryExpression::getAllValues(
    ValueAccessor *accessor,
    const SubBlocksReference *sub_blocks_ref,
    ColumnVectorCache *cv_cache) const {
  if (fast_operator_.get() == nullptr) {
    return ColumnVectorPtr(
        ColumnVector::MakeVectorOfValue(getType(),
                                        static_value_,
                                        accessor->getNumTuplesVirtual()));
  } else {
#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
    const attribute_id operand_attr_id = operand_->getAttributeIdForValueAccessor();
    if (operand_attr_id != -1) {
      return ColumnVectorPtr(
          fast_operator_->applyToValueAccessor(accessor, operand_attr_id));
    }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

    ColumnVectorPtr operand_result(
        operand_->getAllValues(accessor, sub_blocks_ref, cv_cache));
    return ColumnVectorPtr(
        fast_operator_->applyToColumnVector(*operand_result));
  }
}

ColumnVectorPtr ScalarUnaryExpression::getAllValuesForJoin(
    const relation_id left_relation_id,
    ValueAccessor *left_accessor,
    const relation_id right_relation_id,
    ValueAccessor *right_accessor,
    const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids,
    ColumnVectorCache *cv_cache) const {
  if (fast_operator_.get() == nullptr) {
    return ColumnVectorPtr(
        ColumnVector::MakeVectorOfValue(getType(),
                                        static_value_,
                                        joined_tuple_ids.size()));
  } else {
#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN
    const attribute_id operand_attr_id = operand_->getAttributeIdForValueAccessor();
    if (operand_attr_id != -1) {
      const relation_id operand_relation_id = operand_->getRelationIdForValueAccessor();
      DCHECK_NE(operand_relation_id, -1);
      DCHECK((operand_relation_id == left_relation_id)
             || (operand_relation_id == right_relation_id));
      const bool using_left_relation = (operand_relation_id == left_relation_id);
      ValueAccessor *operand_accessor = using_left_relation ? left_accessor
                                                            : right_accessor;
      return ColumnVectorPtr(
          fast_operator_->applyToValueAccessorForJoin(operand_accessor,
                                                      using_left_relation,
                                                      operand_attr_id,
                                                      joined_tuple_ids));
    }
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN

    ColumnVectorPtr operand_result(
        operand_->getAllValuesForJoin(left_relation_id,
                                      left_accessor,
                                      right_relation_id,
                                      right_accessor,
                                      joined_tuple_ids,
                                      cv_cache));
    return ColumnVectorPtr(
        fast_operator_->applyToColumnVector(*operand_result));
  }
}

void ScalarUnaryExpression::initHelper(bool own_children) {
  if (operation_.canApplyToType(operand_->getType())) {
    if (operand_->hasStaticValue()) {
      static_value_ = operation_.applyToChecked(operand_->getStaticValue(),
                                                operand_->getType());
    } else {
      fast_operator_.reset(operation_.makeUncheckedUnaryOperatorForType(operand_->getType()));
    }
  } else {
    const Type &operand_type = operand_->getType();
    if (!own_children) {
      operand_.release();
    }
    throw OperationInapplicableToType(operation_.getName(), 1, operand_type.getName().c_str());
  }
}

void ScalarUnaryExpression::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const Expression*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const Expression*>> *container_child_fields) const {
  Scalar::getFieldStringItems(inline_field_names,
                              inline_field_values,
                              non_container_child_field_names,
                              non_container_child_fields,
                              container_child_field_names,
                              container_child_fields);

  if (fast_operator_ == nullptr) {
    inline_field_names->emplace_back("static_value");
    if (static_value_.isNull()) {
      inline_field_values->emplace_back("NULL");
    } else {
      inline_field_values->emplace_back(type_.printValueToString(static_value_));
    }
  }

  inline_field_names->emplace_back("operation");
  inline_field_values->emplace_back(
      kUnaryOperationNames[static_cast<std::underlying_type<UnaryOperationID>::type>(
          operation_.getUnaryOperationID())]);

  non_container_child_field_names->emplace_back("operand");
  non_container_child_fields->emplace_back(operand_.get());
}

}  // namespace quickstep
