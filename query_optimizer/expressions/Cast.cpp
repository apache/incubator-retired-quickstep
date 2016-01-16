/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/expressions/Cast.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarUnaryExpression.hpp"
#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "types/Type.hpp"
#include "types/operations/unary_operations/NumericCastOperation.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace expressions {

ExpressionPtr Cast::copyWithNewChildren(
    const std::vector<ExpressionPtr> &new_children) const {
  DCHECK_EQ(getNumChildren(), new_children.size());
  ScalarPtr scalar;
  CHECK(SomeScalar::MatchesWithConditionalCast(new_children[0], &scalar))
      << new_children[0]->toString();
  return Create(scalar, target_type_);
}

::quickstep::Scalar *Cast::concretize(
    const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const {
  return new ::quickstep::ScalarUnaryExpression(::quickstep::NumericCastOperation::Instance(target_type_),
                                                operand_->concretize(substitution_map));
}

void Cast::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  inline_field_names->push_back("target_type");
  inline_field_values->push_back(target_type_.getName());

  non_container_child_field_names->push_back("operand");
  non_container_child_fields->push_back(operand_);
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
