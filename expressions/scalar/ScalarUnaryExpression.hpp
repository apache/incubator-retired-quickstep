/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_UNARY_EXPRESSION_HPP_
#define QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_UNARY_EXPRESSION_HPP_

#include <memory>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/scalar/Scalar.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/unary_operations/UnaryOperation.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class ColumnVector;
class ValueAccessor;

struct SubBlocksReference;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief Expressions which are based on a UnaryOperation.
 **/
class ScalarUnaryExpression : public Scalar {
 public:
  /**
   * @brief Constructor.
   *
   * @param operation The unary operation to be performed.
   * @param operand The argument of the operation, which this
   *        ScalarUnaryExpression takes ownership of.
   **/
  ScalarUnaryExpression(const UnaryOperation &operation, Scalar *operand);

  /**
   * @brief Destructor.
   **/
  ~ScalarUnaryExpression() override {
  }

  serialization::Scalar getProto() const override;

  Scalar* clone() const override;

  ScalarDataSource getDataSource() const override {
    return kUnaryExpression;
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

  ColumnVector* getAllValues(ValueAccessor *accessor,
                             const SubBlocksReference *sub_blocks_ref) const override;

  ColumnVector* getAllValuesForJoin(
      const relation_id left_relation_id,
      ValueAccessor *left_accessor,
      const relation_id right_relation_id,
      ValueAccessor *right_accessor,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const override;

 private:
  void initHelper(bool own_children);

  const UnaryOperation &operation_;

  std::unique_ptr<Scalar> operand_;
  TypedValue static_value_;
  std::unique_ptr<UncheckedUnaryOperator> fast_operator_;

  friend class PredicateTest;
  friend class ScalarTest;

  DISALLOW_COPY_AND_ASSIGN(ScalarUnaryExpression);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_UNARY_EXPRESSION_HPP_
