/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "expressions/scalar/ScalarAttribute.hpp"

#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/Expressions.pb.h"
#include "storage/StorageBlockInfo.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

ScalarAttribute::ScalarAttribute(const CatalogAttribute &attribute)
    : Scalar(attribute.getType()),
      attribute_(attribute) {
}

serialization::Scalar ScalarAttribute::getProto() const {
  serialization::Scalar proto;
  proto.set_data_source(serialization::Scalar::ATTRIBUTE);
  proto.SetExtension(serialization::ScalarAttribute::relation_id, attribute_.getParent().getID());
  proto.SetExtension(serialization::ScalarAttribute::attribute_id, attribute_.getID());

  return proto;
}

Scalar* ScalarAttribute::clone() const {
  return new ScalarAttribute(attribute_);
}

TypedValue ScalarAttribute::getValueForSingleTuple(const ValueAccessor &accessor,
                                                   const tuple_id tuple) const {
  return accessor.getTypedValueAtAbsolutePositionVirtual(attribute_.getID(), tuple);
}

TypedValue ScalarAttribute::getValueForJoinedTuples(
    const ValueAccessor &left_accessor,
    const relation_id left_relation_id,
    const tuple_id left_tuple_id,
    const ValueAccessor &right_accessor,
    const relation_id right_relation_id,
    const tuple_id right_tuple_id) const {
  // FIXME(chasseur): This can get confused and break for self-joins.
  DCHECK((attribute_.getParent().getID() == left_relation_id)
         || (attribute_.getParent().getID() == right_relation_id));
  if (attribute_.getParent().getID() == left_relation_id) {
    return left_accessor.getTypedValueAtAbsolutePositionVirtual(attribute_.getID(),
                                                                left_tuple_id);
  } else {
    return right_accessor.getTypedValueAtAbsolutePositionVirtual(attribute_.getID(),
                                                                 right_tuple_id);
  }
}

attribute_id ScalarAttribute::getAttributeIdForValueAccessor() const {
  return attribute_.getID();
}

relation_id ScalarAttribute::getRelationIdForValueAccessor() const {
  return attribute_.getParent().getID();
}

ColumnVector* ScalarAttribute::getAllValues(ValueAccessor *accessor,
                                            const SubBlocksReference *sub_blocks_ref) const {
  const attribute_id attr_id = attribute_.getID();
  const Type &result_type = attribute_.getType();
  return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
      accessor,
      [&](auto *accessor) -> ColumnVector* {  // NOLINT(build/c++11)
    if (NativeColumnVector::UsableForType(result_type)) {
      NativeColumnVector *result = new NativeColumnVector(result_type,
                                                          accessor->getNumTuples());
      accessor->beginIteration();
      if (result_type.isNullable()) {
        while (accessor->next()) {
          const void *value = accessor->template getUntypedValue<true>(attr_id);
          if (value == nullptr) {
            result->appendNullValue();
          } else {
            result->appendUntypedValue(value);
          }
        }
      } else {
        while (accessor->next()) {
          result->appendUntypedValue(
              accessor->template getUntypedValue<false>(attr_id));
        }
      }
      return result;
    } else {
      IndirectColumnVector *result = new IndirectColumnVector(result_type,
                                                              accessor->getNumTuples());
      accessor->beginIteration();
      while (accessor->next()) {
        result->appendTypedValue(accessor->getTypedValue(attr_id));
      }
      return result;
    }
  });
}

ColumnVector* ScalarAttribute::getAllValuesForJoin(
    const relation_id left_relation_id,
    ValueAccessor *left_accessor,
    const relation_id right_relation_id,
    ValueAccessor *right_accessor,
    const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const {
  DCHECK((attribute_.getParent().getID() == left_relation_id)
         || (attribute_.getParent().getID() == right_relation_id));

  const attribute_id attr_id = attribute_.getID();
  const Type &result_type = attribute_.getType();

  const bool using_left_relation = (attribute_.getParent().getID() == left_relation_id);
  ValueAccessor *accessor = using_left_relation ? left_accessor
                                                : right_accessor;

  return InvokeOnValueAccessorNotAdapter(
      accessor,
      [&](auto *accessor) -> ColumnVector* {  // NOLINT(build/c++11)
    if (NativeColumnVector::UsableForType(result_type)) {
      NativeColumnVector *result = new NativeColumnVector(result_type,
                                                          joined_tuple_ids.size());
      if (result_type.isNullable()) {
        for (const std::pair<tuple_id, tuple_id> &joined_pair : joined_tuple_ids) {
          const void *value = accessor->template getUntypedValueAtAbsolutePosition<true>(
              attr_id,
              using_left_relation ? joined_pair.first : joined_pair.second);
          if (value == nullptr) {
            result->appendNullValue();
          } else {
            result->appendUntypedValue(value);
          }
        }
      } else {
        for (const std::pair<tuple_id, tuple_id> &joined_pair : joined_tuple_ids) {
          result->appendUntypedValue(
              accessor->template getUntypedValueAtAbsolutePosition<false>(
                  attr_id,
                  using_left_relation ? joined_pair.first : joined_pair.second));
        }
      }
      return result;
    } else {
      IndirectColumnVector *result = new IndirectColumnVector(result_type,
                                                              joined_tuple_ids.size());
      for (const std::pair<tuple_id, tuple_id> &joined_pair : joined_tuple_ids) {
        result->appendTypedValue(
              accessor->getTypedValueAtAbsolutePosition(
                  attr_id,
                  using_left_relation ? joined_pair.first : joined_pair.second));
      }
      return result;
    }
  });
}

}  // namespace quickstep
