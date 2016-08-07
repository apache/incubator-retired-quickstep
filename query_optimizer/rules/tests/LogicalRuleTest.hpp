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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_TESTS_LOGICAL_RULE_TEST_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_TESTS_LOGICAL_RULE_TEST_HPP_

#include <memory>

#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/rules/Rule.hpp"
#include "query_optimizer/tests/OptimizerTest.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

/** \addtogroup QueryRules
 *  @{
 */

/**
 * @brief Bais class for tests on logical rules.
 */
class LogicalRuleTest : public OptimizerTest {
 protected:
  /**
   * @brief Constructor.
   */
  LogicalRuleTest() {}

  /**
   * @brief Destructor.
   */
  ~LogicalRuleTest() override {}

  void SetUp() override {
    OptimizerTest::SetUp();
    setupRule(&rule_);
  }

  /**
   * @brief Sets up the rule.
   *
   * @param rule The rule to test.
   */
  virtual void setupRule(std::unique_ptr<Rule<logical::Logical>>* rule) = 0;

  /**
   * @return The rule to test.
   */
  Rule<logical::Logical>* rule() { return rule_.get(); }

  logical::LogicalPtr input_;
  logical::LogicalPtr expect_output_;
  logical::LogicalPtr actual_output_;

 private:
  std::unique_ptr<Rule<logical::Logical>> rule_;

  DISALLOW_COPY_AND_ASSIGN(LogicalRuleTest);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_RULES_TESTS_LOGICAL_RULE_TEST_HPP_ */
