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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_TESTS_EXPRESSION_RULE_TEST_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_TESTS_EXPRESSION_RULE_TEST_HPP_

#include <memory>

#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/rules/Rule.hpp"
#include "query_optimizer/tests/OptimizerTest.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

/** \addtogroup QueryRules
 *  @{
 */

/**
 * @brief Basic class for tests on expression rules.
 */
class ExpressionRuleTest : public OptimizerTest {
 protected:
  /**
   * @brief Constructor.
   */
  ExpressionRuleTest() {}

  /**
   * @brief Destructor.
   */
  ~ExpressionRuleTest() override {}

  void SetUp() override {
    OptimizerTest::SetUp();
    setupRule(&rule_);
  }

  /**
   * @brief Sets up the rule.
   *
   * @param rule The rule to test.
   */
  virtual void setupRule(
      std::unique_ptr<Rule<expressions::Expression>>* rule) = 0;

  /**
   * @return The rule to test.
   */
  Rule<expressions::Expression>* rule() { return rule_.get(); }

  expressions::ExpressionPtr input_;
  expressions::ExpressionPtr expect_output_;
  expressions::ExpressionPtr actual_output_;

  std::unique_ptr<Rule<expressions::Expression>> rule_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ExpressionRuleTest);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_RULES_TESTS_EXPRESSION_RULE_TEST_HPP_ */
