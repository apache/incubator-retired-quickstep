/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_PUSH_DOWN_SEMI_ANTI_JOIN_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_PUSH_DOWN_SEMI_ANTI_JOIN_HPP_

#include <string>

#include "query_optimizer/logical/HashJoin.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/rules/TopDownRule.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

class PushDownSemiAntiJoin : public TopDownRule<logical::Logical> {
 public:
  PushDownSemiAntiJoin() {}

  std::string getName() const override { return "PushDownSemiAntiJoin"; }

 protected:
  logical::LogicalPtr applyToNode(const logical::LogicalPtr &input) override;

 private:
  logical::LogicalPtr pushDownSemiAntiJoin(const logical::HashJoinPtr &input);

  DISALLOW_COPY_AND_ASSIGN(PushDownSemiAntiJoin);
};

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_RULES_PUSH_DOWN_SEMI_ANTI_JOIN_HPP_ */
