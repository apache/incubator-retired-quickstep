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
