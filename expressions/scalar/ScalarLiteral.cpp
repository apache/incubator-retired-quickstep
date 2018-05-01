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

#include "expressions/scalar/ScalarLiteral.hpp"

#include <string>
#include <utility>
#include <vector>

#include "expressions/Expressions.pb.h"
#include "storage/ValueAccessor.hpp"
#include "types/Type.hpp"
#include "types/Type.pb.h"
#include "types/TypedValue.hpp"
#include "types/TypedValue.pb.h"
#include "types/containers/ColumnVector.hpp"

namespace quickstep {

serialization::Scalar ScalarLiteral::getProto() const {
  serialization::Scalar proto;
  proto.set_data_source(serialization::Scalar::LITERAL);
  proto.MutableExtension(serialization::ScalarLiteral::literal)
      ->CopyFrom(internal_literal_.getProto());
  proto.MutableExtension(serialization::ScalarLiteral::literal_type)
      ->CopyFrom(type_.getProto());

  return proto;
}

Scalar* ScalarLiteral::clone() const {
  return new ScalarLiteral(internal_literal_, type_);
}

ColumnVectorPtr ScalarLiteral::getAllValues(
    ValueAccessor *accessor,
    const SubBlocksReference *sub_blocks_ref,
    ColumnVectorCache *cv_cache) const {
  return ColumnVectorPtr(
      ColumnVector::MakeVectorOfValue(type_,
                                      internal_literal_,
                                      accessor->getNumTuplesVirtual()));
}

ColumnVectorPtr ScalarLiteral::getAllValuesForJoin(
    ValueAccessor *left_accessor,
    ValueAccessor *right_accessor,
    const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids,
    ColumnVectorCache *cv_cache) const {
  return ColumnVectorPtr(
      ColumnVector::MakeVectorOfValue(type_,
                                      internal_literal_,
                                      joined_tuple_ids.size()));
}

void ScalarLiteral::getFieldStringItems(
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

  inline_field_names->emplace_back("internal_literal");
  if (internal_literal_.isNull()) {
    inline_field_values->emplace_back("NULL");
  } else {
    inline_field_values->emplace_back(type_.printValueToString(internal_literal_));
  }
}

}  // namespace quickstep
