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

#include "viz/VizEngine.hpp"

#include "utility/PlanVisualizer.hpp"
#include "utility/ExecutionDAGVisualizer.hpp"
#include "viz/VizAnalyzer.hpp"
#include "viz/configs/DumpRelation.hpp"
#include "viz/rules/Grouping.hpp"
#include "viz/rules/VizRule.hpp"

#include "json.hpp"

namespace quickstep {
namespace viz {

using nlohmann::json;

void VizEngine::execute() {
  rules_.emplace_back(new Grouping(context_base_));

  while (!rules_.empty()) {
    std::unique_ptr<VizRule> rule(rules_.front().release());
    rules_.pop_front();

    rule->execute();

    for (auto &derived_rule : rule->derived_rules_) {
      rules_.emplace_back(derived_rule.release());
    }
    for (auto &result_conf : rule->result_confs_) {
      confs_.emplace_back(result_conf.release());
    }
  }

  json viz = json::array();
  for (auto &conf : confs_) {
    viz.push_back(conf->toJSON());
  }

  const VizAnalyzer *analyzer = context_base_->get<VizAnalyzer>("VizAnalyzer");
  PlanVisualizer plan_visualizer;
  ExecutionDAGVisualizer dag_visualizer(*analyzer->getExecutionPlan());
  if (analyzer->getProfilingStats() != nullptr) {
    dag_visualizer.bindProfilingStats(*analyzer->getProfilingStats());
  }

  json output = json::object();
  output["plan"] = plan_visualizer.visualize(analyzer->getQueryPlan());
  output["dag"] = dag_visualizer.toDOT();
  output["results"] = DumpRelation(context_base_).toJSON();
  output["viz"] = viz;
  std::cerr << output.dump(2) << "\n";
}

}  // namespace viz
}  // namespace quickstep
