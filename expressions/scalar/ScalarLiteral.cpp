/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
