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

#ifndef QUICKSTEP_VIZ_RULES_GROUPING_HPP_
#define QUICKSTEP_VIZ_RULES_GROUPING_HPP_

#include <memory>
#include <unordered_set>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "utility/Macros.hpp"
#include "viz/VizAnalyzer.hpp"
#include "viz/rules/OneDimensionOneMeasure.hpp"

namespace quickstep {
namespace viz {

/** \addtogroup Viz
 *  @{
 */

class Grouping : public VizRule {
 public:
  Grouping(const VizContextPtr &context)
      : VizRule(context) {}

  ~Grouping() override {}

  void execute() override {
    const VizAnalyzer *analyzer =
        context_->get<VizAnalyzer>("VizAnalyzer");

    std::vector<attribute_id> group_by_attribute_ids;
    if (analyzer->findGroupByAttributes(&group_by_attribute_ids)) {
      std::unordered_set<attribute_id> group_by_attr_id_set(
          group_by_attribute_ids.begin(),
          group_by_attribute_ids.end());
      std::vector<attribute_id> residual_attribute_ids;
      for (const CatalogAttribute &attr : *analyzer->getRelation()) {
        if (group_by_attr_id_set.find(attr.getID()) == group_by_attr_id_set.end()) {
          residual_attribute_ids.emplace_back(attr.getID());
        }
      }

      std::unique_ptr<VizContext> new_context(new VizContext(context_));
      new_context->set("trace", new StringValue("Grouping"));
      new_context->set("Dimensions",
                       new AttributeIdVector(group_by_attribute_ids));
      new_context->set("Measures",
                       new AttributeIdVector(residual_attribute_ids));

      dispatchWithGrouping(analyzer,
                           VizContextPtr(new_context.release()),
                           group_by_attribute_ids.size(),
                           residual_attribute_ids.size());
    } else {
      // TODO
    }
  }

 private:
  void dispatchWithGrouping(const VizAnalyzer *analyzer,
                            const VizContextPtr new_context,
                            const std::size_t num_dimension_attrs,
                            const std::size_t num_measure_attrs) {
    if (num_dimension_attrs == 1) {
      if (num_measure_attrs == 1) {
        derive(new OneDimensionOneMeasure(new_context));
      }
    }
  }

  DISALLOW_COPY_AND_ASSIGN(Grouping);
 };

/** @} */

}  // namespace viz
}  // namespace quickstep

#endif  // QUICKSTEP_VIZ_RULES_GROUPING_HPP_
