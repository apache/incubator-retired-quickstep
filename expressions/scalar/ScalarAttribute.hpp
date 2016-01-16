/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_ATTRIBUTE_HPP_
#define QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_ATTRIBUTE_HPP_

#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/scalar/Scalar.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogAttribute;
class ColumnVector;
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
   **/
  explicit ScalarAttribute(const CatalogAttribute &attribute);

  serialization::Scalar getProto() const override;

  Scalar* clone() const override;

  ScalarDataSource getDataSource() const override {
    return kAttribute;
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

  attribute_id getAttributeIdForValueAccessor() const override;

  relation_id getRelationIdForValueAccessor() const override;

  ColumnVector* getAllValues(ValueAccessor *accessor,
                             const SubBlocksReference *sub_blocks_ref) const override;

  ColumnVector* getAllValuesForJoin(
      const relation_id left_relation_id,
      ValueAccessor *left_accessor,
      const relation_id right_relation_id,
      ValueAccessor *right_accessor,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const override;

  const CatalogAttribute& getAttribute() const {
    return attribute_;
  }

 protected:
  const CatalogAttribute &attribute_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ScalarAttribute);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_ATTRIBUTE_HPP_
