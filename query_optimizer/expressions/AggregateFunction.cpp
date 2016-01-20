/**
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

#include "query_optimizer/expressions/AggregateFunction.hpp"

#include <string>
#include <utility>
#include <vector>

#include "expressions/aggregation/AggregateFunction.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "types/Type.hpp"
#include "utility/Cast.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace expressions {

bool AggregateFunction::isNullable() const {
  std::vector<const Type*> argument_types;
  for (const ScalarPtr &argument : arguments_) {
    argument_types.emplace_back(&argument->getValueType());
  }

  const Type *return_type = aggregate_.resultTypeForArgumentTypes(argument_types);
  DCHECK(return_type != nullptr);
  return return_type->isNullable();
}

const Type& AggregateFunction::getValueType() const {
  std::vector<const Type*> argument_types;
  for (const ScalarPtr &argument : arguments_) {
    argument_types.emplace_back(&argument->getValueType());
  }

  const Type *return_type = aggregate_.resultTypeForArgumentTypes(argument_types);
  DCHECK(return_type != nullptr);
  return *return_type;
}

AggregateFunctionPtr AggregateFunction::Create(
    const ::quickstep::AggregateFunction &aggregate,
    const std::vector<ScalarPtr> &arguments,
    const bool is_vector_aggregate) {
#ifdef QUICKSTEP_DEBUG
  std::vector<const Type*> argument_types;
  for (const ScalarPtr &argument : arguments) {
    argument_types.emplace_back(&argument->getValueType());
  }
  DCHECK(aggregate.canApplyToTypes(argument_types));
#endif  // QUICKSTEP_DEBUG

  return AggregateFunctionPtr(
      new AggregateFunction(aggregate, arguments, is_vector_aggregate));
}

ExpressionPtr AggregateFunction::copyWithNewChildren(
    const std::vector<ExpressionPtr> &new_children) const {
  std::vector<ScalarPtr> new_arguments;
  for (const ExpressionPtr &expression_ptr : new_children) {
    ScalarPtr expr_as_scalar;
    CHECK(SomeScalar::MatchesWithConditionalCast(expression_ptr, &expr_as_scalar))
        << expression_ptr->toString();
    new_arguments.emplace_back(std::move(expr_as_scalar));
  }

  return Create(aggregate_, new_arguments, is_vector_aggregate_);
}

std::vector<AttributeReferencePtr> AggregateFunction::getReferencedAttributes() const {
  std::vector<AttributeReferencePtr> referenced_attributes;
  for (const ScalarPtr &argument : arguments_) {
    const std::vector<AttributeReferencePtr> referenced_attributes_in_argument =
        argument->getReferencedAttributes();
    referenced_attributes.insert(referenced_attributes.end(),
                                 referenced_attributes_in_argument.begin(),
                                 referenced_attributes_in_argument.end());
  }
  return referenced_attributes;
}

void AggregateFunction::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  inline_field_names->push_back("function");
  inline_field_values->push_back(aggregate_.getName());

  container_child_field_names->push_back("");
  container_child_fields->emplace_back(CastSharedPtrVector<OptimizerTreeBase>(arguments_));
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
