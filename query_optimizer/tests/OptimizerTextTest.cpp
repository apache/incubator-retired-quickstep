/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include <fstream>
#include <memory>
#include <iostream>

#include "query_optimizer/tests/OptimizerTextTestRunner.hpp"
#include "utility/textbased_test/TextBasedTestDriver.hpp"

#include "glog/logging.h"

using quickstep::TextBasedTest;

QUICKSTEP_GENERATE_TEXT_TEST(OPTIMIZER_TEST);

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);

  if (argc < 3) {
    LOG(ERROR) << "Must have at least 2 arguments, but " << argc - 1
               << " are provided";
  }

  std::ifstream input_file(argv[1]);
  CHECK(input_file.is_open()) << argv[1];
  std::unique_ptr<quickstep::optimizer::OptimizerTextTestRunner> test_runner(
      new quickstep::optimizer::OptimizerTextTestRunner);
  test_driver.reset(
      new quickstep::TextBasedTestDriver(&input_file, test_runner.get()));
  test_driver->registerOptions(
      quickstep::optimizer::OptimizerTextTestRunner::kTestOptions);

  ::testing::InitGoogleTest(&argc, argv);
  int success = RUN_ALL_TESTS();
  if (success != 0) {
    test_driver->writeActualOutputToFile(argv[2]);
  }

  return success;
}
