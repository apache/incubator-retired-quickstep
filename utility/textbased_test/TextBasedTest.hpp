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

#ifndef QUICKSTEP_UTILITY_TEXTBASED_TEST_TEXTBASED_TEST_HPP_
#define QUICKSTEP_UTILITY_TEXTBASED_TEST_TEXTBASED_TEST_HPP_

#include <set>
#include <string>

#include "utility/textbased_test/TextBasedTestRunner.hpp"
#include "utility/Macros.hpp"

#include "gtest/gtest.h"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief Info for a text-based test case. See TextBasedTestDriver for
 *        a description of the format of such a test.
 */
struct TextBasedTestCase {
  /**
   * @brief Constructor. Does not take ownership of \p test_runner_in.
   *        We copy all the constant strings because they have lifetime
   *        requirements beyond the original strings they are copied from.
   */
  TextBasedTestCase(std::string &&preceding_text_in,        // NOLINT(whitespace/operators)
                    std::string &&input_text_in,            // NOLINT(whitespace/operators)
                    std::string &&expected_output_text_in,  // NOLINT(whitespace/operators)
                    std::set<std::string> &&options_in,
                    TextBasedTestRunner *test_runner_in)
      : preceding_text(preceding_text_in),
        input_text(input_text_in),
        expected_output_text(expected_output_text_in),
        options(options_in),
        test_runner(test_runner_in) {
  }

  /**
   * @brief The text (comments and options) preceding the input testing text.
   */
  const std::string preceding_text;

  /**
   * @brief The test input used to generate the output.
   */
  const std::string input_text;

  /**
   * @brief The expected test output for the test input.
   */
  const std::string expected_output_text;

  /**
   * @brief The test options that can be used to specify how and what the
   *        test runner generates the output from an input.
   */
  const std::set<std::string> options;

  /**
   * @brief Test runner that takes <input_text> as input and generates
   *        <actual_output_text> according to <options> (if given).
   */
  TextBasedTestRunner *test_runner;

  /**
   * @brief Text output by <test_runner>.
   */
  std::string actual_output_text;
};

/**
 * @brief Value-parameterized test that takes a test case as input, calls the
 *        associated test runner and compares the expected output and the
 *        actual output.
 */
class TextBasedTest : public ::testing::TestWithParam<TextBasedTestCase*> {
 public:
  /**
   * @brief Destructor.
   */
  ~TextBasedTest() {
  }

  void SetUp() override {
    test_case_ = GetParam();
  }

 protected:
  /**
   * @brief Constructor.
   */
  TextBasedTest() : test_case_(nullptr) {}

  TextBasedTestCase *test_case_;

 private:
  DISALLOW_COPY_AND_ASSIGN(TextBasedTest);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_UTILITY_TEXTBASED_TEST_TEXTBASED_TEST_HPP_ */
