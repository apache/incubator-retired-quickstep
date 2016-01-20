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

#include "utility/textbased_test/TextBasedTestDriver.hpp"

#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "utility/Macros.hpp"
#include "utility/textbased_test/TextBasedTest.hpp"

#include "glog/logging.h"
#include "gtest/gtest.h"

#define EXPECT_TEXT_CASE(test_case,                                         \
                         comments_and_options_text,                         \
                         option_set,                                        \
                         test_input_text,                                   \
                         expected_test_output_text)                         \
  EXPECT_EQ(comments_and_options_text, (test_case)->preceding_text);        \
  EXPECT_EQ(test_input_text, (test_case)->input_text);                      \
  EXPECT_EQ(expected_test_output_text, (test_case)->expected_output_text);  \
  EXPECT_EQ(option_set, (test_case)->options)

namespace quickstep {

class TextBasedTestDriverTest : public ::testing::Test {
 protected:
  TextBasedTestDriverTest()
      : preregistered_options_{"option1", "option2", "option3", "option4", "option5", "option6", "option7", "option8",
                               "option9", "option10"} {
  }

  /**
   * @brief Resets the test driver with the text stored
   *        in \p string_stream and gets test cases.
   *
   * @param string_stream The stream where to extract the test cases.
   * @return A vector of TextBasedTestCases from the input stream.
   */
  std::vector<TextBasedTestCase*>* extractTestCasesFromStream(const std::ostringstream &string_stream) {
    std::istringstream input_stream(string_stream.str());
    test_driver_.reset(new TextBasedTestDriver(&input_stream, nullptr /* test_runner */));
    test_driver_->registerOptions(preregistered_options_);
    return test_driver_->populateAndReturnTestCases();
  }

  std::unique_ptr<TextBasedTestDriver> test_driver_;
  // Pre-registered options.
  const std::vector<std::string> preregistered_options_;

 private:
  DISALLOW_COPY_AND_ASSIGN(TextBasedTestDriverTest);
};

TEST_F(TextBasedTestDriverTest, SingleTestCase) {
  const std::string comments_and_options =
      "\n# Comment line 1\n\n"
      "# Comment line 2\r\n\n"
      "[option1 option2]\n"
      "[ option3 option4 option3 ]\n"
      "[default option5]\n";
  const std::string test_input_txt =
      "input line 1\n\n"
      "input line 2 \r\n";
  const std::string expected_test_output_text =
      "expected output line 1\n"
      "----------\n"
      "expected output line 2\n\n";

  std::ostringstream input_text_stream;

  // Empty input stream is OK.
  extractTestCasesFromStream(input_text_stream);

  // Having only comments is also OK.
  input_text_stream << "# Comment";
  extractTestCasesFromStream(input_text_stream);

  // Having options but no test cases is not allowed.
  input_text_stream.str("");
  input_text_stream << comments_and_options;
  EXPECT_DEATH(extractTestCasesFromStream(input_text_stream), "Has options but no input test text");

  // With a valid test case.
  input_text_stream << test_input_txt << "--\n" << expected_test_output_text << "==\n";
  std::vector<TextBasedTestCase*> *test_cases =
      extractTestCasesFromStream(input_text_stream);
  CHECK_EQ(test_cases->size(), 1u);
  EXPECT_TEXT_CASE((*test_cases)[0],
                   comments_and_options,
                   std::set<std::string>({"option1", "option2", "option3", "option4", "option5"}),
                   test_input_txt,
                   expected_test_output_text);

  input_text_stream.str("");
  // Do not append a test-case separator at the end.
  input_text_stream << comments_and_options << test_input_txt << "--\n" << expected_test_output_text;
  test_cases = extractTestCasesFromStream(input_text_stream);

  // The result should be the same. The last test-case separator can
  // be omitted.
  CHECK_EQ(test_cases->size(), 1u);
  EXPECT_TEXT_CASE((*test_cases)[0],
                   comments_and_options,
                   std::set<std::string>({"option1", "option2", "option3", "option4", "option5"}),
                   test_input_txt,
                   expected_test_output_text);

  input_text_stream.str("");
  // We only require that the input-output (test-case) separators
  // begin with "--" ("==").
  input_text_stream
      << comments_and_options << test_input_txt << "-------abc\n" << expected_test_output_text << "=======def\n";
  test_cases = extractTestCasesFromStream(input_text_stream);

  CHECK_EQ(test_cases->size(), 1u);
  EXPECT_TEXT_CASE((*test_cases)[0],
                   comments_and_options,
                   std::set<std::string>({"option1", "option2", "option3", "option4", "option5"}),
                   test_input_txt,
                   expected_test_output_text);

  // Without comments.
  input_text_stream.str("");
  input_text_stream << "[option1]\n" << test_input_txt << "--\n" << expected_test_output_text << "==\n";
  test_cases = extractTestCasesFromStream(input_text_stream);

  CHECK_EQ(test_cases->size(), 1u);
  EXPECT_TEXT_CASE((*test_cases)[0],
                   "[option1]\n",
                   std::set<std::string>({"option1"}),
                   test_input_txt,
                   expected_test_output_text);

  // Without options.
  input_text_stream.str("");
  input_text_stream << "# Comments\n" << test_input_txt << "--\n" << expected_test_output_text << "==\n";
  test_cases = extractTestCasesFromStream(input_text_stream);

  CHECK_EQ(test_cases->size(), 1u);
  EXPECT_TEXT_CASE((*test_cases)[0],
                   "# Comments\n",
                   std::set<std::string>(),
                   test_input_txt,
                   expected_test_output_text);

  // Without comments and options.
  input_text_stream.str("");
  input_text_stream << test_input_txt << "--\n" << expected_test_output_text << "==\n";
  test_cases = extractTestCasesFromStream(input_text_stream);

  CHECK_EQ(test_cases->size(), 1u);
  EXPECT_TEXT_CASE((*test_cases)[0],
                   "" /* comments_and_options_text */,
                   std::set<std::string>(),
                   test_input_txt,
                   expected_test_output_text);

  // With empty expected output.
  input_text_stream.str("");
  input_text_stream << test_input_txt << "--\n==";
  test_cases = extractTestCasesFromStream(input_text_stream);
  CHECK_EQ(test_cases->size(), 1u);
  EXPECT_TEXT_CASE((*test_cases)[0],
                   "" /* comments_and_options_text */,
                   std::set<std::string>(),
                   test_input_txt,
                   "" /* expected_test_output_text */);

  input_text_stream.str("");
  input_text_stream << test_input_txt << "--";
  test_cases = extractTestCasesFromStream(input_text_stream);
  CHECK_EQ(test_cases->size(), 1u);
  EXPECT_TEXT_CASE((*test_cases)[0],
                   "" /* comments_and_options_text */,
                   std::set<std::string>(),
                   test_input_txt,
                   "" /* expected_test_output_text */);
}

TEST_F(TextBasedTestDriverTest, DefaultOptionsTest) {
  const std::string comments_and_options_0 =
      "# Comments for the first case\n"
      "[default option1 option2]\n"
      "[default option3]\n";
  const std::string comments_and_options_1 =
      "# Comments for the second case\n"
      "[option4 option5]\n";
  const std::string test_input_txt =
      "input line\n";
  const std::string expected_test_output_text =
      "expected output line 1\n";

  std::ostringstream input_text_stream;
  input_text_stream << comments_and_options_0 << test_input_txt << "--\n" << expected_test_output_text << "==\n"
                    << comments_and_options_1 << test_input_txt << "--\n" << expected_test_output_text << "==\n";

  std::vector<TextBasedTestCase*> *test_cases =
      extractTestCasesFromStream(input_text_stream);

  CHECK_EQ(test_cases->size(), 2u);
  EXPECT_TEXT_CASE((*test_cases)[0],
                   comments_and_options_0,
                   std::set<std::string>({"option1", "option2", "option3"}),
                   test_input_txt,
                   expected_test_output_text);
  EXPECT_TEXT_CASE((*test_cases)[1],
                   comments_and_options_1,
                   std::set<std::string>({"option1", "option2", "option3", "option4", "option5"}),
                   test_input_txt,
                   expected_test_output_text);

  // A default option statement in a new test case clears the default
  // options set by previous test cases. The third and fourth test cases
  // do not inherit option1 -- option3.
  const std::string comments_and_options_3 =
      "# Comments for the third case\n"
      "[default option6]\n"
      "[option7 option8]\n";
  const std::string comments_and_options_4 =
      "# Comments for the third case\n"
      "[option9 option10]\n";
  input_text_stream << comments_and_options_3 << test_input_txt << "--\n" << expected_test_output_text << "==\n"
                    << comments_and_options_4 << test_input_txt << "--\n" << expected_test_output_text << "==\n";
  test_cases = extractTestCasesFromStream(input_text_stream);

  CHECK_EQ(test_cases->size(), 4u);
  EXPECT_TEXT_CASE((*test_cases)[2],
                   comments_and_options_3,
                   std::set<std::string>({"option6", "option7", "option8"}),
                   test_input_txt,
                   expected_test_output_text);
  EXPECT_TEXT_CASE((*test_cases)[3],
                   comments_and_options_4,
                   std::set<std::string>({"option6", "option9", "option10"}),
                   test_input_txt,
                   expected_test_output_text);
}

TEST_F(TextBasedTestDriverTest, SameAsAboveTest) {
  const std::string comments_and_options =
      "# Comments for the first case\n"
      "[option1 option2]\n";
  const std::string test_input_txt =
      "input line\n";
  const std::string expected_test_output_text =
      "expected output line 1\n";
  const std::string same_as_above_test_output_text =
      "[same as above]";

  std::ostringstream input_text_stream;
  input_text_stream << comments_and_options << test_input_txt << "--\n" << same_as_above_test_output_text;
  EXPECT_DEATH(extractTestCasesFromStream(input_text_stream),
               "Cannot specify \"same as above\" in the first test case");

  input_text_stream.str("");
  input_text_stream << comments_and_options << test_input_txt << "--\n" << expected_test_output_text << "==\n"
                    << comments_and_options << test_input_txt << "--\n" << same_as_above_test_output_text;
  std::vector<TextBasedTestCase*> *test_cases =
      extractTestCasesFromStream(input_text_stream);

  CHECK_EQ(test_cases->size(), 2u);
  EXPECT_TEXT_CASE((*test_cases)[1],
                   comments_and_options,
                   std::set<std::string>({"option1", "option2"}),
                   test_input_txt,
                   expected_test_output_text);
}

TEST_F(TextBasedTestDriverTest, OptionErrors) {
  const std::string test_input_txt = "input line\n";
  const std::string expected_test_output_text = "expected output line\n";

  const std::string with_unregistered_option =
      "# Comments for the first case\n"
      "[unregistered_option]\n";
  std::ostringstream input_text_stream;
  input_text_stream << with_unregistered_option << test_input_txt << "--\n" << expected_test_output_text;
  EXPECT_DEATH(
      extractTestCasesFromStream(input_text_stream),
      "Option unregistered_option is not found");

  const std::string invalid_option_line_0 = "[option1] [option2]\n";
  input_text_stream.str("");
  input_text_stream << invalid_option_line_0 << test_input_txt << "--\n" << expected_test_output_text;
  EXPECT_DEATH(
      extractTestCasesFromStream(input_text_stream),
      "Cannot have text after options");

  const std::string invalid_option_line_1 = "[option1] abc\n";
  input_text_stream.str("");
  input_text_stream << invalid_option_line_1 << test_input_txt << "--\n" << expected_test_output_text;
  EXPECT_DEATH(
      extractTestCasesFromStream(input_text_stream),
      "Cannot have text after options");

  const std::string with_no_options_0 = "[]\n";
  input_text_stream.str("");
  input_text_stream << with_no_options_0 << test_input_txt << "--\n" << expected_test_output_text;
  EXPECT_DEATH(
      extractTestCasesFromStream(input_text_stream),
      "Has no options specified in the brackets");

  const std::string with_no_options_1 = "[      \t ]\n";
  input_text_stream.str("");
  input_text_stream << with_no_options_1 << test_input_txt << "--\n" << expected_test_output_text;
  EXPECT_DEATH(
      extractTestCasesFromStream(input_text_stream),
      "Has no options specified in the brackets");

  const std::string with_no_options_2 = "[ default ]\n";
  input_text_stream.str("");
  input_text_stream << with_no_options_2 << test_input_txt << "--\n" << expected_test_output_text;
  EXPECT_DEATH(
      extractTestCasesFromStream(input_text_stream),
      "Has no options specified in the brackets");
}

}  // namespace quickstep
