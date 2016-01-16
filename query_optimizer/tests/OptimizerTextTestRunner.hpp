/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
