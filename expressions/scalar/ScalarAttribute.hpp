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

#ifndef QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_ATTRIBUTE_HPP_
#define QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_ATTRIBUTE_HPP_

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

class CatalogAttribute;
class ColumnVectorCache;
class ValueAccessor;

struct SubBlocksReference;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief Scalars which are attribute values from tuples.
 **/
class ScalarAttribute : public Scalar {
 public:
  /**
   * @brief Constructor.
   *
   * @param attribute The attribute to use.
   * @param join_side The join side of which this attribute belongs to.
   **/
  explicit ScalarAttribute(const CatalogAttribute &attribute,
                           const JoinSide join_side = kNone);

  serialization::Scalar getProto() const override;

  Scalar* clone() const override;

  ScalarDataSource getDataSource() const override {
    return kAttribute;
  }

  TypedValue getValueForSingleTuple(const ValueAccessor &accessor,
                                    const tuple_id tuple) const override;

  TypedValue getValueForJoinedTuples(
      const ValueAccessor &left_accessor,
      const tuple_id left_tuple_id,
      const ValueAccessor &right_accessor,
      const tuple_id right_tuple_id) const override;

  attribute_id getAttributeIdForValueAccessor() const override;

  ColumnVectorPtr getAllValues(ValueAccessor *accessor,
                               const SubBlocksReference *sub_blocks_ref,
                               ColumnVectorCache *cv_cache) const override;

  ColumnVectorPtr getAllValuesForJoin(
      ValueAccessor *left_accessor,
      ValueAccessor *right_accessor,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids,
      ColumnVectorCache *cv_cache) const override;

  const CatalogAttribute& getAttribute() const {
    return attribute_;
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const Expression*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const Expression*>> *container_child_fields) const override;

  const CatalogAttribute &attribute_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ScalarAttribute);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_ATTRIBUTE_HPP_
