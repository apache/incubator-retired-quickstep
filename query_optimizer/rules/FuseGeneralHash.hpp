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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_FUSE_GENERAL_HASH_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_FUSE_GENERAL_HASH_HPP_

#include <cstdint>
#include <memory>
#include <string>

#include "query_optimizer/physical/GeneralizedHashJoin.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/rules/TopDownRule.hpp"

namespace quickstep {
namespace optimizer {

/** \addtogroup OptimizerRules
 *  @{
 */

/**
 * @brief Rule that applies to a physical plan to fuse a select node in the 
 *        build predicate of a hash join to the join.
 */
class FuseGeneralHash : public TopDownRule<physical::Physical> {
 public:
    /**
     * @brief Constructor.
     */
    FuseGeneralHash() {}

    ~FuseGeneralHash() override {}

    std::string getName() const override {
      return "FuseGeneralHash";
    }

 protected:
    physical::PhysicalPtr applyToNode(const physical::PhysicalPtr &node) override;

 private:
    DISALLOW_COPY_AND_ASSIGN(FuseGeneralHash);
};

}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_RULES_FUSE_GENERAL_HASH_
