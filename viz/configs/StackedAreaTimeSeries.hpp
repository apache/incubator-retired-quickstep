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

#ifndef QUICKSTEP_VIZ_CONFIGS_STACKED_AREA_TIME_SERIES_HPP_
#define QUICKSTEP_VIZ_CONFIGS_STACKED_AREA_TIME_SERIES_HPP_

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

class StackedAreaTimeSeries : public VizConfig {
 public:
  StackedAreaTimeSeries(const attribute_id time_attr_id,
                        const std::string &time_format,
                        const attribute_id group_attr_id,
                        const attribute_id measure_attr_id,
                        const VizContextPtr &context)
      : VizConfig(context),
        time_attr_id_(time_attr_id),
        time_format_(time_format),
        group_attr_id_(group_attr_id),
        measure_attr_id_(measure_attr_id) {}

  ~StackedAreaTimeSeries() override {}

  nlohmann::json toJSON() override {
    std::vector<attribute_id> all_attr_ids;
    all_attr_ids.emplace_back(time_attr_id_);
    if (group_attr_id_ != kInvalidAttributeID) {
      all_attr_ids.emplace_back(group_attr_id_);
    }
    all_attr_ids.emplace_back(measure_attr_id_);

    nlohmann::json schema = copySchema(all_attr_ids);
    nlohmann::json columns = nlohmann::json::array();
    for (std::size_t i = 0; i < all_attr_ids.size(); ++i) {
      const std::string &header = schema[i]["name"];
      columns.push_back(copyColumn(all_attr_ids[i], &header));
    }

    nlohmann::json data;
    data["xFormat"] = time_format_;
    if (group_attr_id_ == kInvalidAttributeID) {
      data["x"] = schema[0]["name"];
      data["columns"] = columns;

      const std::string &m_name = schema[1]["name"];
      data["types"][m_name] = "area";
      data["groups"] = nlohmann::json::array({ nlohmann::json::array({ m_name }) });
    } else {
      const nlohmann::json &group_col = columns.at(1);
      nlohmann::json &time_col = columns.at(0);
      nlohmann::json &measure_col = columns.at(2);

      std::map<nlohmann::json, std::vector<nlohmann::json>> time_groups;
      std::map<nlohmann::json, std::vector<nlohmann::json>> measure_groups;
      for (std::size_t i = 1; i < group_col.size(); ++i) {
        const nlohmann::json &key = group_col.at(i);
        time_groups[key].emplace_back(std::move(time_col.at(i)));
        measure_groups[key].emplace_back(std::move(measure_col.at(i)));
      }

      nlohmann::json xs;
      nlohmann::json pivot_columns = nlohmann::json::array();
      nlohmann::json types;
      nlohmann::json group = nlohmann::json::array();
      for (auto &pair : time_groups) {
        const auto &key = pair.first;
        const std::string str_key_measure = RemoveQuotes(key.dump());
        const std::string str_key_time = "_$x_" + str_key_measure;
        xs[str_key_measure] = str_key_time;
        types[str_key_measure] = "area";
        group.push_back(str_key_measure);

        auto &time_vec = pair.second;
        auto &measure_vec = measure_groups.at(key);
        nlohmann::json pivot_time_col = { str_key_time };
        nlohmann::json pivot_measure_col = { str_key_measure };
        for (std::size_t i = 0; i < time_vec.size(); ++i) {
          pivot_time_col.push_back(std::move(time_vec.at(i)));
          pivot_measure_col.push_back(std::move(measure_vec.at(i)));
        }
        pivot_columns.push_back(pivot_time_col);
        pivot_columns.push_back(pivot_measure_col);
      }

      data["xs"] = xs;
      data["columns"] = pivot_columns;
      data["types"] = types;
      data["groups"] = nlohmann::json::array({ group });
    }

    nlohmann::json x_axis;
    x_axis["label"]["text"] = schema[0]["name"];
    x_axis["label"]["position"] = "outer-right";
    x_axis["type"] = "timeseries";
    x_axis["tick"]["format"] = time_format_;

    nlohmann::json c3;
    c3["data"] = data;
    c3["axis"]["x"] = x_axis;

    nlohmann::json ret;
    ret["type"] = "StackedAreaTimeSeries";
    ret["trace"] = copyTrace();
    ret["schema"] = schema;
    ret["c3"] = c3;

    return ret;
  }

 private:
  std::string RemoveQuotes(const std::string &input) {
    std::string ret;
    for (std::size_t i = 0; i < input.size(); ++i) {
      const char c = input[i];
      if (c != '"') {
        ret.push_back(c);
      }
    }
    return ret;
  }

  const attribute_id time_attr_id_;
  const std::string time_format_;
  const attribute_id group_attr_id_;
  const attribute_id measure_attr_id_;

  DISALLOW_COPY_AND_ASSIGN(StackedAreaTimeSeries);
 };

/** @} */

}  // namespace viz
}  // namespace quickstep

#endif  // QUICKSTEP_VIZ_CONFIGS_STACKED_AREA_TIME_SERIES_HPP_
