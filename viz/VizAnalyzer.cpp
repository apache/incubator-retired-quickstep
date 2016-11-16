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

#include "viz/VizAnalyzer.hpp"

#include <cstddef>
#include <unordered_map>
#include <vector>

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace viz {

namespace E = ::quickstep::optimizer::expressions;
namespace P = ::quickstep::optimizer::physical;

bool VizAnalyzer::findGroupByAttributes(
    std::vector<attribute_id> *group_by_attribute_ids) const {
  std::vector<E::AttributeReferencePtr> group_by_attributes;
  if (findGroupByAttributesInternal(physical_plan_, &group_by_attributes)) {
    CHECK(!group_by_attributes.empty());

    std::unordered_map<E::ExprId, attribute_id> attr_id_map;
    const std::vector<E::AttributeReferencePtr> query_result_attributes =
        physical_plan_->getOutputAttributes();
    for (std::size_t i = 0; i < query_result_attributes.size(); ++i) {
      attr_id_map.emplace(query_result_attributes[i]->id(), i);
    }

    for (const auto &attr : group_by_attributes) {
      const auto attr_it = attr_id_map.find(attr->id());
      CHECK(attr_it != attr_id_map.end());
      group_by_attribute_ids->emplace_back(attr_it->second);
    }
    return true;
  }
  return false;
}

bool VizAnalyzer::findGroupByAttributesInternal(
    const P::PhysicalPtr &physical_plan,
    std::vector<E::AttributeReferencePtr> *group_by_attributes) const {
  switch (physical_plan->getPhysicalType()) {
    case P::PhysicalType::kTopLevelPlan:  // Fall through
    case P::PhysicalType::kSort:
    case P::PhysicalType::kSelection: {
      if (!findGroupByAttributesInternal(physical_plan->children().at(0),
                                         group_by_attributes)) {
        return false;
      }
      if (!E::SubsetOfExpressions(*group_by_attributes,
                                  physical_plan->getOutputAttributes())) {
        return false;
      }
      return true;
    }
    case P::PhysicalType::kAggregate: {
      const P::AggregatePtr &aggregate =
          std::static_pointer_cast<const P::Aggregate>(physical_plan);
      if (aggregate->grouping_expressions().empty()) {
        return false;
      }
      *group_by_attributes = E::ToRefVector(aggregate->grouping_expressions());
      return true;
    }
    default:
      return false;
  }
}



}  // namespace viz
}  // namespace quickstep
