/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/expressions/AttributeReference.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#include "expressions/scalar/ScalarAttribute.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace expressions {

ExpressionPtr AttributeReference::copyWithNewChildren(
    const std::vector<ExpressionPtr> &new_children) const {
  DCHECK_EQ(new_children.size(), children().size());
  return AttributeReference::Create(id(),
                                    attribute_name(),
                                    attribute_alias(),
                                    relation_name(),
                                    getValueType());
}

std::vector<AttributeReferencePtr> AttributeReference::getReferencedAttributes()
    const {
  return { Create(id(), attribute_name(), attribute_alias(), relation_name(), type_) };
}

::quickstep::Scalar *AttributeReference::concretize(
    const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const {
  const std::unordered_map<ExprId, const CatalogAttribute*>::const_iterator found_it =
      substitution_map.find(id());
  DCHECK(found_it != substitution_map.end()) << toString();
  return new ::quickstep::ScalarAttribute(*found_it->second);
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
