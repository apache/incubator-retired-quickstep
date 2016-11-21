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

#ifndef QUICKSTEP_VIZ_CONFIGS_LINE_CHART_HPP_
#define QUICKSTEP_VIZ_CONFIGS_LINE_CHART_HPP_

#include <string>
#include <map>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "utility/Macros.hpp"
#include "viz/configs/VizConfig.hpp"
#include "viz/VizContext.hpp"

#include "json.hpp"

namespace quickstep {

class CatalogRelation;
class StorageManager;

namespace viz {

/** \addtogroup Viz
 *  @{
 */

class LineChart : public VizConfig {
 public:
  LineChart(const attribute_id dimension_attr_id,
            const std::vector<attribute_id> &measure_attr_ids,
            const VizContextPtr &context)
      : VizConfig(context),
        dimension_attr_id_(dimension_attr_id),
        measure_attr_ids_(measure_attr_ids) {}

  ~LineChart() override {}

  nlohmann::json toJSON() override {
    std::vector<attribute_id> all_attr_ids;
    all_attr_ids.emplace_back(dimension_attr_id_);
    for (const attribute_id id : measure_attr_ids_) {
      all_attr_ids.emplace_back(id);
    }
    nlohmann::json schema = copySchema(all_attr_ids);
    nlohmann::json columns = nlohmann::json::array();
    for (std::size_t i = 1; i < all_attr_ids.size(); i++) {
      std::string header = schema[i]["name"];
      columns.push_back(copyColumn(all_attr_ids[i], &header));
    }

    nlohmann::json data;
    data["columns"] = columns;

    nlohmann::json x_axis;
    x_axis["x"]["type"] = "category";
    x_axis["x"]["categories"] = copyColumn(all_attr_ids[0], nullptr);

    nlohmann::json c3;
    c3["data"] = data;
    c3["axis"] = x_axis;

    nlohmann::json ret;
    ret["type"] = "LineChart";
    ret["trace"] = copyTrace();
    ret["schema"] = schema;
    ret["c3"] = c3;

    return ret;
  }

 private:
  const attribute_id dimension_attr_id_;
  const std::vector<attribute_id> measure_attr_ids_;

  DISALLOW_COPY_AND_ASSIGN(LineChart);
 };

/** @} */

}  // namespace viz
}  // namespace quickstep

#endif  // QUICKSTEP_VIZ_CONFIGS_LINE_CHART_HPP_
