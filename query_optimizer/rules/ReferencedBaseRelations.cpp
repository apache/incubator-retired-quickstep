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

#include "query_optimizer/rules/ReferencedBaseRelations.hpp"

#include "query_optimizer/logical/PatternMatcher.hpp"
#include "query_optimizer/logical/TableReference.hpp"

namespace quickstep {

class CatalogRelation;

namespace optimizer {

namespace L = ::quickstep::optimizer::logical;

logical::LogicalPtr ReferencedBaseRelations::applyToNode(const logical::LogicalPtr &input) {
  L::TableReferencePtr table_reference;
  const CatalogRelation* input_relation = nullptr;
  if (L::SomeTableReference::MatchesWithConditionalCast(input, &table_reference)) {
    input_relation = table_reference->catalog_relation();
    referenced_base_relations_.push_back(input_relation);
  }
  return input;
}

}  // namespace optimizer
}  // namespace quickstep
