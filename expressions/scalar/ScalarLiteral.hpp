/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_LITERAL_HPP_
#define QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_LITERAL_HPP_

#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/scalar/Scalar.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class ColumnVector;
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

  ColumnVector* getAllValues(ValueAccessor *accessor,
                             const SubBlocksReference *sub_blocks_ref) const override;

  ColumnVector* getAllValuesForJoin(
      const relation_id left_relation_id,
      ValueAccessor *left_accessor,
      const relation_id right_relation_id,
      ValueAccessor *right_accessor,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const override;

 private:
  TypedValue internal_literal_;

  DISALLOW_COPY_AND_ASSIGN(ScalarLiteral);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_LITERAL_HPP_
