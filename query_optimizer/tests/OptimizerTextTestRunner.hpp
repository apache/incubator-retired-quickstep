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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_TESTS_OPTIMIZER_TEXT_TEST_RUNNER_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_TESTS_OPTIMIZER_TEXT_TEST_RUNNER_HPP_

#include <set>
#include <string>
#include <vector>

#include "parser/SqlParserWrapper.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/tests/TestDatabaseLoader.hpp"
#include "utility/Macros.hpp"
#include "utility/textbased_test/TextBasedTestDriver.hpp"

namespace quickstep {

class ParseStatement;

namespace optimizer {

class OptimizerContext;

/** \addtogroup QueryOptimizer
 *  @{
 */

/**
 * @brief Runner for all text-based tests in the optimizer.
 */
class OptimizerTextTestRunner : public TextBasedTestRunner {
 public:
  /**
   * @brief Constructor.
   */
  OptimizerTextTestRunner() {
    test_database_loader_.createTestRelation(true /* allow_vchar */);
  }

  void runTestCase(const std::string &input,
                   const std::set<std::string> &options,
                   std::string *output) override;

  /**
   * @brief Test options.
   */
  static const std::vector<std::string> kTestOptions;

 private:
  logical::LogicalPtr resolveParseTree(const ParseStatement &parse_statement,
                                       OptimizerContext *optimizer_context);

  logical::LogicalPtr generateLogicalPlan(const ParseStatement &parse_statement,
                                          OptimizerContext *optimizer_context);

  physical::PhysicalPtr generatePhysicalPlan(
      const logical::LogicalPtr &logical_plan);

  SqlParserWrapper sql_parser_;
  TestDatabaseLoader test_database_loader_;

  DISALLOW_COPY_AND_ASSIGN(OptimizerTextTestRunner);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_TESTS_OPTIMIZER_TEXT_TEST_RUNNER_HPP_ */
