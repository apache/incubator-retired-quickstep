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

#include "expressions/scalar/ScalarLiteral.hpp"

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

ColumnVector* ScalarLiteral::getAllValues(
    ValueAccessor *accessor,
    const SubBlocksReference *sub_blocks_ref) const {
  return ColumnVector::MakeVectorOfValue(
      type_,
      internal_literal_,
      accessor->getNumTuplesVirtual());
}

ColumnVector* ScalarLiteral::getAllValuesForJoin(
    const relation_id left_relation_id,
    ValueAccessor *left_accessor,
    const relation_id right_relation_id,
    ValueAccessor *right_accessor,
    const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const {
  return ColumnVector::MakeVectorOfValue(type_,
                                         internal_literal_,
                                         joined_tuple_ids.size());
}

}  // namespace quickstep
