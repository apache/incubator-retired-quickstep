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

#include "expressions/scalar/Scalar.hpp"

#include <string>
#include <vector>

#include "utility/Macros.hpp"

namespace quickstep {

const char *Scalar::kScalarDataSourceNames[] = {
  "Literal",
  "Attribute",
  "UnaryExpression",
  "BinaryExpression",
  "SimpleCase"
  "SharedExpression",
};

const TypedValue& Scalar::getStaticValue() const {
  FATAL_ERROR("Called getStaticValue() on a Scalar which does not have a static value");
}

void Scalar::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const Expression*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const Expression*>> *container_child_fields) const {
  inline_field_names->emplace_back("result_type");
  inline_field_values->emplace_back(type_.getName());
}

}  // namespace quickstep
