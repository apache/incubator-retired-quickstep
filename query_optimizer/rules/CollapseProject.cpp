/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#include "query_optimizer/rules/CollapseProject.hpp"

#include <vector>

#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/PatternMatcher.hpp"
#include "query_optimizer/logical/Project.hpp"
#include "query_optimizer/rules/Rule.hpp"
#include "query_optimizer/rules/RuleHelper.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace L = ::quickstep::optimizer::logical;

L::LogicalPtr CollapseProject::applyToNode(const L::LogicalPtr &input) {
  L::ProjectPtr project;
  L::ProjectPtr child_project;

  if (L::SomeProject::MatchesWithConditionalCast(input, &project) &&
      L::SomeProject::MatchesWithConditionalCast(project->input(), &child_project)) {
    std::vector<E::NamedExpressionPtr> project_expressions =
        project->project_expressions();
    PullUpProjectExpressions(child_project->project_expressions(),
                             {} /* non_project_expression_lists */,
                             {&project_expressions} /* project_expression_lists */);
    L::LogicalPtr output = L::Project::Create(child_project->input(), project_expressions);
    LOG_APPLYING_RULE(input, output);
    return output;
  }

  LOG_IGNORING_RULE(input);
  return input;
}

}  // namespace optimizer
}  // namespace quickstep
