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

#ifndef QUICKSTEP_VIZ_CONFIGS_DUMP_RELATION_HPP_
#define QUICKSTEP_VIZ_CONFIGS_DUMP_RELATION_HPP_

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "utility/Macros.hpp"
#include "viz/configs/VizConfig.hpp"
#include "viz/VizAnalyzer.hpp"
#include "viz/VizContext.hpp"

#include "json.hpp"

namespace quickstep {

class CatalogRelation;
class StorageManager;

namespace viz {

/** \addtogroup Viz
 *  @{
 */

class DumpRelation : public VizConfig {
 public:
  DumpRelation(const VizContextPtr &context)
      : VizConfig(context) {}

  ~DumpRelation() override {}

  nlohmann::json toJSON() override {
    const CatalogRelation *relation =
        context_->get<VizAnalyzer>("VizAnalyzer")->getRelation();
    std::vector<attribute_id> attr_ids;

    nlohmann::json data = nlohmann::json::array();
    for (const CatalogAttribute &attr : *relation) {
      data.push_back(copyColumn(attr.getID()));
      attr_ids.emplace_back(attr.getID());
    }

    nlohmann::json ret;
    ret["type"] = "DumpRelation";
    ret["schema"] = copySchema(attr_ids);
    ret["data"] = data;

    return ret;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(DumpRelation);
 };

/** @} */

}  // namespace viz
}  // namespace quickstep

#endif  // QUICKSTEP_VIZ_CONFIGS_DUMP_RELATION_HPP_
