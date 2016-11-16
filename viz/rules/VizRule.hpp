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

#ifndef QUICKSTEP_VIZ_RULES_VIZ_RULE_HPP_
#define QUICKSTEP_VIZ_RULES_VIZ_RULE_HPP_

#include "utility/Macros.hpp"
#include "viz/VizContext.hpp"
#include "viz/configs/VizConfig.hpp"

namespace quickstep {
namespace viz {

/** \addtogroup Viz
 *  @{
 */

class VizRule {
 public:
  virtual ~VizRule() {}

  virtual void execute() = 0;

  inline void derive(VizRule *rule) {
    derived_rules_.emplace_back(rule);
  }

  inline void yield(VizConfig *conf) {
    result_confs_.emplace_back(conf);
  }

 protected:
  explicit VizRule(const VizContextPtr &context)
      : context_(context) {}

  const VizContextPtr context_;

 private:
  friend class VizEngine;

  std::vector<std::unique_ptr<VizRule>> derived_rules_;
  std::vector<std::unique_ptr<VizConfig>> result_confs_;

  DISALLOW_COPY_AND_ASSIGN(VizRule);
 };

/** @} */

}  // namespace viz
}  // namespace quickstep

#endif  // QUICKSTEP_VIZ_RULES_VIZ_RULE_HPP_
