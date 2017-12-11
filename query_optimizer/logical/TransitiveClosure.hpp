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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_TRANSITIVE_CLOSURE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_TRANSITIVE_CLOSURE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class TransitiveClosure;
typedef std::shared_ptr<const TransitiveClosure> TransitiveClosurePtr;

class TransitiveClosure : public Logical {
 public:
  ~TransitiveClosure() override {}

  LogicalType getLogicalType() const override {
    return LogicalType::kTransitiveClosure;
  }

  std::string getName() const override {
    return "TransitiveClosure";
  }

  const LogicalPtr& start() const {
    return start_;
  }

  const LogicalPtr& edge() const {
    return edge_;
  }

  const expressions::AttributeReferencePtr& start_attr() const {
    return start_attr_;
  }

  const expressions::AttributeReferencePtr& source_attr() const {
    return source_attr_;
  }

  const expressions::AttributeReferencePtr& destination_attr() const {
    return destination_attr_;
  }

  LogicalPtr copyWithNewChildren(const std::vector<LogicalPtr> &new_children) const override;

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override;

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override;

  static TransitiveClosurePtr Create(
      const LogicalPtr &start,
      const LogicalPtr &edge,
      const expressions::AttributeReferencePtr &start_attr,
      const expressions::AttributeReferencePtr &source_attr,
      const expressions::AttributeReferencePtr &destination_attr) {
    return TransitiveClosurePtr(
        new TransitiveClosure(start,
                              edge,
                              start_attr,
                              source_attr,
                              destination_attr));
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

 private:
  TransitiveClosure(const LogicalPtr &start,
                    const LogicalPtr &edge,
                    const expressions::AttributeReferencePtr &start_attr,
                    const expressions::AttributeReferencePtr &source_attr,
                    const expressions::AttributeReferencePtr &destination_attr)
      : start_(start),
        edge_(edge),
        start_attr_(start_attr),
        source_attr_(source_attr),
        destination_attr_(destination_attr) {
    addChild(start_);
    addChild(edge_);
  }

  const LogicalPtr start_;
  const LogicalPtr edge_;
  const expressions::AttributeReferencePtr start_attr_;
  const expressions::AttributeReferencePtr source_attr_;
  const expressions::AttributeReferencePtr destination_attr_;

  DISALLOW_COPY_AND_ASSIGN(TransitiveClosure);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_TRANSITIVE_CLOSURE_HPP_
