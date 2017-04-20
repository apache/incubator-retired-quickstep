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

#ifndef QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_BINARY_EXPRESSION_HPP_
#define QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_BINARY_EXPRESSION_HPP_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/scalar/Scalar.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class ColumnVectorCache;
class ValueAccessor;

struct SubBlocksReference;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief Expressions which are based on a BinaryOperation.
 **/
class ScalarBinaryExpression : public Scalar {
 public:
  /**
   * @brief Constructor.
   *
   * @param operation The binary operation to be performed.
   * @param left_operand The left argument of the operation, which this
   *        ScalarBinaryExpression takes ownership of.
   * @param right_operand The right argument of the operation, which this
   *        ScalarBinaryExpression takes ownership of.
   **/
  ScalarBinaryExpression(const BinaryOperation &operation,
                         Scalar *left_operand,
                         Scalar *right_operand);

  /**
   * @brief Destructor
   **/
  ~ScalarBinaryExpression() override {
  }

  serialization::Scalar getProto() const override;

  Scalar* clone() const override;

  ScalarDataSource getDataSource() const override {
    return kBinaryExpression;
  }

  TypedValue getValueForSingleTuple(const ValueAccessor &accessor,
                                    const tuple_id tuple) const override;

  TypedValue getValueForJoinedTuples(
      const ValueAccessor &left_accessor,
      const relation_id left_relation_id,
      const tuple_id left_tuple_id,
      const ValueAccessor &right_accessor,
      const relation_id right_relation_id,
      const tuple_id right_tuple_id) const override;

  bool hasStaticValue() const override {
    return fast_operator_.get() == nullptr;
  }

  const TypedValue& getStaticValue() const override {
    DCHECK(hasStaticValue());
    return static_value_;
  }

  ColumnVectorPtr getAllValues(ValueAccessor *accessor,
                               const SubBlocksReference *sub_blocks_ref,
                               ColumnVectorCache *cv_cache) const override;

  ColumnVectorPtr getAllValuesForJoin(
      const relation_id left_relation_id,
      ValueAccessor *left_accessor,
      const relation_id right_relation_id,
      ValueAccessor *right_accessor,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids,
      ColumnVectorCache *cv_cache) const override;

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const Expression*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const Expression*>> *container_child_fields) const override;

 private:
  void initHelper(bool own_children);

  const BinaryOperation &operation_;

  std::unique_ptr<Scalar> left_operand_;
  std::unique_ptr<Scalar> right_operand_;
  TypedValue static_value_;
  std::unique_ptr<UncheckedBinaryOperator> fast_operator_;

  friend class PredicateTest;
  friend class ScalarTest;

  DISALLOW_COPY_AND_ASSIGN(ScalarBinaryExpression);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_BINARY_EXPRESSION_HPP_
