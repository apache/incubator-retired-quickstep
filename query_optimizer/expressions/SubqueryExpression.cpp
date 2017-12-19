/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#include "query_optimizer/expressions/SubqueryExpression.hpp"

#include <string>
#include <unordered_map>
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
  // Note(jianqiao): Here simply return an empty set so that we skip the validation
  // for this expression at the end of the resolving phase (otherwise we need to
  // revise the Validate() function to deal with OUTER scoped attributes). Note
  // that SubqueryExpression will always be eliminated by UnnestSubqueries as the
  // first logical optimization pass in LogicalGenerator. So any dangling attribute
  // will still be detected by Validate() at the end of the logical optimization
  // phase.
  return {};
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
