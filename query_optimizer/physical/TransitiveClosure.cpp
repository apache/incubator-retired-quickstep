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

#include "query_optimizer/physical/TransitiveClosure.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/physical/Physical.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace physical {

namespace E = ::quickstep::optimizer::expressions;

PhysicalPtr TransitiveClosure::copyWithNewChildren(
    const std::vector<PhysicalPtr> &new_children) const {
  DCHECK_EQ(2u, new_children.size());
  return Create(new_children[0],
                new_children[1],
                start_attr_,
                source_attr_,
                destination_attr_);
}

std::vector<E::AttributeReferencePtr> TransitiveClosure::getOutputAttributes() const {
  return { start_attr_, destination_attr_ };
}

std::vector<E::AttributeReferencePtr> TransitiveClosure::getReferencedAttributes() const {
  return { start_attr_, source_attr_, destination_attr_ };
}

void TransitiveClosure::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  non_container_child_field_names->push_back("start");
  non_container_child_fields->push_back(start_);

  non_container_child_field_names->push_back("edge");
  non_container_child_fields->push_back(edge_);

  non_container_child_field_names->push_back("start_attr");
  non_container_child_fields->push_back(start_attr_);

  non_container_child_field_names->push_back("source_attr");
  non_container_child_fields->push_back(source_attr_);

  non_container_child_field_names->push_back("destination_attr");
  non_container_child_fields->push_back(destination_attr_);
}

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

