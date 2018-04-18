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

#include <iostream>
#include <fstream>
#include <memory>

#include "cli/tests/CommandExecutorTestRunner.hpp"
#include "utility/textbased_test/TextBasedTestDriver.hpp"

#include "glog/logging.h"

using quickstep::TextBasedTest;

QUICKSTEP_GENERATE_TEXT_TEST(COMMAND_EXECUTOR_TEST);

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);

  if (argc < 4) {
    LOG(ERROR) << "Must have at least 3 arguments, but " << argc - 1
               << " are provided";
  }

  std::ifstream input_file(argv[1]);
  CHECK(input_file.is_open()) << argv[1];
  std::unique_ptr<quickstep::CommandExecutorTestRunner>
      test_runner(
          new quickstep::CommandExecutorTestRunner(argv[3]));
  test_driver.reset(
      new quickstep::TextBasedTestDriver(&input_file, test_runner.get()));

  ::testing::InitGoogleTest(&argc, argv);
  const int success = RUN_ALL_TESTS();
  if (success != 0) {
    test_driver->writeActualOutputToFile(argv[2]);
  }

  return success;
}
