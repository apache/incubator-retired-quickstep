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

#ifndef QUICKSTEP_VIZ_CONFIGS_HEAT_MAP_HPP_
#define QUICKSTEP_VIZ_CONFIGS_HEAT_MAP_HPP_

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

class HeatMap : public VizConfig {
 public:
  HeatMap(const attribute_id row_attr_id,
          const attribute_id col_attr_id,
          const attribute_id measure_attr_id,
          const VizContextPtr &context)
      : VizConfig(context),
        row_attr_id_(row_attr_id),
        col_attr_id_(col_attr_id),
        measure_attr_id_(measure_attr_id) {}

  ~HeatMap() override {}

  nlohmann::json toJSON() override {
    nlohmann::json columns = nlohmann::json::array();
    columns.push_back(copyColumn(row_attr_id_));
    columns.push_back(copyColumn(col_attr_id_));
    columns.push_back(copyColumn(measure_attr_id_));

    nlohmann::json data;
    data["columns"] = columns;

    nlohmann::json ret;
    ret["type"] = "HeatMap";
    ret["trace"] = copyTrace();
    ret["schema"] = copySchema({row_attr_id_, col_attr_id_, measure_attr_id_});
    ret["data"] = data;

    return ret;
  }

 private:
  const attribute_id row_attr_id_;
  const attribute_id col_attr_id_;
  const attribute_id measure_attr_id_;

  DISALLOW_COPY_AND_ASSIGN(HeatMap);
 };

/** @} */

}  // namespace viz
}  // namespace quickstep

#endif  // QUICKSTEP_VIZ_CONFIGS_HEAT_MAP_HPP_
