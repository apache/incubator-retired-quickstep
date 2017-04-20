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

#ifndef QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_LITERAL_HPP_
#define QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_LITERAL_HPP_

#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/scalar/Scalar.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class ColumnVectorCache;
class Type;
class ValueAccessor;

struct SubBlocksReference;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief Scalars which are literal values from a SQL statement.
 **/
class ScalarLiteral : public Scalar {
 public:
  /**
   * @brief Constructor.
   *
   * @param lit A TypedValue to wrap.
   * @param lit_type The Type that lit belongs to.
   **/
  ScalarLiteral(const TypedValue &lit,
                const Type &lit_type)
      : Scalar(lit_type),
        internal_literal_(lit) {
    internal_literal_.ensureNotReference();
  }

  ScalarLiteral(TypedValue &&lit,
                const Type &lit_type)
      : Scalar(lit_type),
        internal_literal_(lit) {
    internal_literal_.ensureNotReference();
  }

  ~ScalarLiteral() override {
  }

  serialization::Scalar getProto() const override;

  Scalar* clone() const override;

  ScalarDataSource getDataSource() const override {
    return kLiteral;
  }

  TypedValue getValueForSingleTuple(const ValueAccessor &accessor,
                                    const tuple_id tuple) const override {
    return internal_literal_.makeReferenceToThis();
  }

  TypedValue getValueForJoinedTuples(
      const ValueAccessor &left_accessor,
      const relation_id left_relation_id,
      const tuple_id left_tuple_id,
      const ValueAccessor &right_accessor,
      const relation_id right_relation_id,
      const tuple_id right_tuple_id) const override {
    return internal_literal_.makeReferenceToThis();
  }

  bool hasStaticValue() const override {
    return true;
  }

  const TypedValue& getStaticValue() const override {
    return internal_literal_;
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
  TypedValue internal_literal_;

  DISALLOW_COPY_AND_ASSIGN(ScalarLiteral);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_LITERAL_HPP_
