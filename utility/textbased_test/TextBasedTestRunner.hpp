/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_UTILITY_TEXTBASED_TEST_TEXTBASED_TEST_RUNNER_HPP_
#define QUICKSTEP_UTILITY_TEXTBASED_TEST_TEXTBASED_TEST_RUNNER_HPP_

#include <set>
#include <string>

#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief Interface for a test runner in text-based tests.
 */
class TextBasedTestRunner {
 public:
  virtual ~TextBasedTestRunner() {
  }

  /**
   * @brief Generate an \p output from \p input given \p options.
   *
   * @param input The input text.
   * @param options Options provided in the text test file for the input.
   * @param output The output text.
   */
  virtual void runTestCase(const std::string &input, const std::set<std::string> &options,
                           std::string *output) = 0;
 protected:
  TextBasedTestRunner() {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(TextBasedTestRunner);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_UTILITY_TEXTBASED_TEST_TEXTBASED_TEST_RUNNER_HPP_ */
