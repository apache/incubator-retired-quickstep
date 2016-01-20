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
