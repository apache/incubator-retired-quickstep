/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
