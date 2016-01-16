/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_UTILITY_TEXTBASED_TEST_TEXTBASED_TEST_DRIVER_HPP_
#define QUICKSTEP_UTILITY_TEXTBASED_TEST_TEXTBASED_TEST_DRIVER_HPP_

#include <istream>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "utility/textbased_test/TextBasedTest.hpp"
#include "utility/Macros.hpp"

/**
 * @brief Macro to create a text-based test.
 */
#define QUICKSTEP_GENERATE_TEXT_TEST(TEST_CASE_NAME)                                           \
    std::unique_ptr<::quickstep::TextBasedTestDriver> test_driver;                             \
    INSTANTIATE_TEST_CASE_P(TEST_CASE_NAME, TextBasedTest,                                     \
                            ::testing::ValuesIn(*test_driver->populateAndReturnTestCases()))

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief Extracts text-based test cases from an input text stream.
 *
 * Each test case may contain four sections:
 * comments, options, input text and expected output text.
 *
 * 1) The comment line starts with # and is ignored by the test.
 *    Comments are optional.
 *
 * 2) Options are used to tell the test runner how to generate the output and
 *    what should be included in the output. For now, each option is just a string.
 *    A list of options can be specified in a pair of brackets and separated
 *    by whitespaces. An option list should occupy exactly one single line.
 *    Options in an option list with "default" at the very beginning
 *    will be included for all the remaining tests unless they are overridden
 *    by another default option list for anther test. The options section is optional.
 *
 *    Two examples of options:
 *      a) [initial_logical_plan optimized_logical_plan]
 *      b) [default initial_logical_plan]
 *    The first option line specifies two options 'initial_logical_plan'
 *    and 'optimized_logical_plan'. The second line specifies a default
 *    option 'initial_logical_plan'.
 *
 * 3) The input text is required and separated from the expected output
 *    text by "--". The expected output text may be empty.
 *
 * 4) The test cases are separated by "==".
 */
class TextBasedTestDriver {
 public:
  /**
   * @brief Constructor. Does not take ownership of \p test_runner.
   * @param input_stream The stream where to read text-based tests.
   * @param test_runner The test runner that generates actual output text
   *                    for each test case.
   */
  TextBasedTestDriver(std::istream *input_stream, TextBasedTestRunner *test_runner);

  virtual ~TextBasedTestDriver();

  /**
   * @brief Reads test cases from the input stream.
   *        The returned vector and the pointers in the vector are owned by the
   *        driver.
   *
   * @return A vector of test cases.
   */
  std::vector<TextBasedTestCase*>* populateAndReturnTestCases();

  /**
   * @brief Appends the input content with expected outputs
   *        being replaced by actual outputs to \p out.
   *
   * @param out The output stream.
   */
  void writeActualOutput(std::ostream *out) const;

  /**
   * @brief Writes the input content with expected outputs
   *        being replaced by actual outputs to file. It can be
   *        compared with the input file to see the diff result.
   *
   * @param output_filename The output file name.
   */
  void writeActualOutputToFile(const std::string &output_filename) const;

  /**
   * @brief Registers a vector of options.
   *
   * @param option_names The vector of option names to be registered.
   */
  void registerOptions(const std::vector<std::string> &option_names);

  /**
   * @brief Registers a option.
   *
   * @param option_name The option name to be registered.
   */
  void registerOption(const std::string &option_name);

  /**
   * @brief Checks if an option has been registered.
   *
   * @param option_name the option to check.
   * @return True if it is registered.
   */
  bool isRegisteredOption(const std::string &option_name) const;

  /**
   * @brief Checks whether an option is valid. A valid option can
   *        only contain alphanumeric characters or underscores.
   *
   * @param option_name The option to check.
   * @return True if the option is valid.
   */
  bool isValidOption(const std::string &option_name) const;

 private:
  /**
   * @brief Populates <test_cases_> with tests in the input file.
   */
  void populateTestCases();

  /**
   * @return Reads next test case from the input file.
   *         NULL if the end of the file is reached.
   */
  TextBasedTestCase* readNextTestCase();

  /**
   * @brief Clear all test cases.
   */
  void clearTestCases();

  /**
   * @brief Reads options from one line and inserts the options
   *        to \p options_out. If they are default options,
   *        also appends them to \p default_options_out.
   *
   * @param line The input line.
   * @param options_out The set that the new options are inserted to.
   * @param default_options_out The set that the new options are inserted
   *                            to if default is specified.
   */
  void readOptions(const std::string &line,
                   std::set<std::string> *options_out,
                   std::set<std::string> *default_options_out);

  /**
   * @brief Checks whether \p str is a separator between test cases (i.e. "==").
   */
  static bool IsTestCaseSeparator(const std::string &str);

  /**
   * @brief Checks whether \p str is a separator between input and output (i.e. "--").
   */
  static bool IsInputOutputSeparator(const std::string &str);

  std::istream *input_stream_;

  std::set<std::string> registered_options_;
  std::set<std::string> default_options_;
  std::vector<TextBasedTestCase*> test_cases_;

  TextBasedTestRunner *test_runner_;

  DISALLOW_COPY_AND_ASSIGN(TextBasedTestDriver);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_UTILITY_TEXTBASED_TEST_TEXTBASED_TEST_DRIVER_HPP_ */
