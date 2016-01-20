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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_TESTS_RULE_TEST_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_TESTS_RULE_TEST_HPP_

#include <memory>

namespace quickstep {
namespace optimizer {

class OptimizerContext;

/** \addtogroup QueryRules
 *  @{
 */

/**
 * @brief Macro that applies a rule and compares the result with the expected
 *        one.
 */
#define APPLY_RULE_AND_CHECK_OUTPUT()                                            \
  do {                                                                           \
    CHECK(input_ != nullptr);                                                    \
    CHECK(expect_output_ != nullptr);                                            \
    CHECK(rule() != nullptr);                                                    \
    actual_output_ = rule()->apply(input_);                                      \
    EXPECT_TRUE(actual_output_ != nullptr);                                      \
    EXPECT_TRUE(expect_output_->toString() == actual_output_->toString())        \
        << "Expected tree:\n" << expect_output_->toString() << "Actual tree:\n"  \
        << actual_output_->toString();                                           \
  } while (false)

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_RULES_TESTS_RULE_TEST_HPP_ */
