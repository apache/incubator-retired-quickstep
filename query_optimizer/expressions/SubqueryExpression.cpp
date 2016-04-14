/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include "query_optimizer/expressions/SubqueryExpression.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"

#include "glog/logging.h"

namespace quickstep {

class CatalogAttribute;
class Scalar;

namespace optimizer {
namespace expressions {

::quickstep::Scalar* SubqueryExpression::concretize(
    const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const {
  LOG(FATAL) << "SubqueryExpression should not be concretized";
}

std::vector<AttributeReferencePtr> SubqueryExpression::getReferencedAttributes() const {
  // SubqueryExpression should be eliminated before any place that needs
  // a call of getReferencedAttributes.
  LOG(FATAL) << "SubqueryExpression::getReferencedAttributes() is not implemented";
}

void SubqueryExpression::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  non_container_child_field_names->push_back("subquery");
  non_container_child_fields->push_back(subquery_);
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
