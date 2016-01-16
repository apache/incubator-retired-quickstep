/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "utility/textbased_test/TextBasedTest.hpp"

#include "utility/textbased_test/TextBasedTestRunner.hpp"

#include "gtest/gtest.h"

namespace quickstep {

TEST_P(TextBasedTest, CompareOutputs) {
  test_case_->test_runner->runTestCase(test_case_->input_text, test_case_->options, &test_case_->actual_output_text);
  EXPECT_EQ(test_case_->expected_output_text, test_case_->actual_output_text);
}

}
