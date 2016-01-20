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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_STRATEGY_TESTS_STRATEGY_TEST_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_STRATEGY_TESTS_STRATEGY_TEST_HPP_

#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/strategy/Strategy.hpp"
#include "query_optimizer/tests/OptimizerTest.hpp"
#include "utility/Macros.hpp"

/**
 * @brief Macro that applies a strategy to the input logical plan and
 *        compares the generated physical plan with the expected one.
 */
#define EXPECT_CORRECT_PHYSICAL()                                                \
  do {                                                                           \
    CHECK(input_logical_ != nullptr);                                            \
    CHECK(expected_physical_ != nullptr);                                        \
    CHECK(strategy_ != nullptr);                                                 \
    actual_physical_.reset();                                                    \
    strategy_->generatePlan(input_logical_, &actual_physical_);                  \
    EXPECT_TRUE(actual_physical_ != nullptr);                                    \
    EXPECT_TRUE(expected_physical_->toString() == actual_physical_->toString())  \
        << "Expected tree:\n" << expected_physical_->toString()                  \
        << "Actual tree:\n" << actual_physical_->toString();                     \
  } while (false)

/**
 * @brief Macro that applies a strategy to the input logical plan and
 *        verifies that the output physical plan is NULL (i.e. the strategy
 *        is not applicable).
 */
#define EXPECT_STRATEGY_NOT_APPLIED()                                            \
  do {                                                                           \
    CHECK(input_logical_ != nullptr);                                            \
    CHECK(strategy_ != nullptr);                                                 \
    actual_physical_.reset();                                                    \
    strategy_->generatePlan(input_logical_, &actual_physical_);                  \
    EXPECT_TRUE(actual_physical_ == nullptr);                                    \
  } while (false)

namespace quickstep {
namespace optimizer {
namespace strategy {

/** \addtogroup OptimizerStrategy
 *  @{
 */

/**
 * @brief Base class for strategy tests.
 */
class StrategyTest : public OptimizerTest {
 public:
  /**
   * @brief Destructor.
   */
  ~StrategyTest() override {}

  /**
   * @brief Sets the strategy to test.
   *
   * @param strategy The strategy to test.
   */
  virtual void setupStrategy(std::unique_ptr<Strategy> *strategy) = 0;

  void SetUp() override {
    OptimizerTest::SetUp();
    setupStrategy(&strategy_);
  }

 protected:
  /**
   * @brief Constructor.
   */
  StrategyTest() {}

  std::unique_ptr<Strategy> strategy_;

  logical::LogicalPtr input_logical_;
  physical::PhysicalPtr expected_physical_;
  physical::PhysicalPtr actual_physical_;

 private:
  DISALLOW_COPY_AND_ASSIGN(StrategyTest);
};

/** @} */

}  // namespace strategy
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_STRATEGY_TESTS_STRATEGYTEST_HPP_ */
