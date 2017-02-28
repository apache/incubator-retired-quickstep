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

#include <fstream>
#include <memory>

#include "query_optimizer/tests/DistributedExecutionGeneratorTestRunner.hpp"
#include "utility/textbased_test/TextBasedTestDriver.hpp"
#include "utility/textbased_test/TextBasedTest.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

namespace quickstep {
namespace optimizer {

DECLARE_bool(use_lip_filters);
DECLARE_bool(use_filter_joins);

}  // namespace optimizer
}  // namespace quickstep

using quickstep::TextBasedTest;

using std::make_unique;

QUICKSTEP_GENERATE_TEXT_TEST(DISTRIBUTED_EXECUTION_GENERATOR_TEST);

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);

  // TODO(quickstep-team): Fix JIRA QUICKSTEP-76 for adding LIP filter support
  // in the distributed version.
  quickstep::optimizer::FLAGS_use_lip_filters = false;
  quickstep::optimizer::FLAGS_use_filter_joins = false;

  // Honor FLAGS_buffer_pool_slots in StorageManager.
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  if (argc < 4) {
    LOG(ERROR) << "Must have at least 3 arguments, but " << argc - 1
               << " are provided";
  }

  std::ifstream input_file(argv[1]);
  CHECK(input_file.is_open()) << argv[1];

  auto test_runner = make_unique<quickstep::optimizer::DistributedExecutionGeneratorTestRunner>(argv[3]);
  test_driver = make_unique<quickstep::TextBasedTestDriver>(&input_file, test_runner.get());
  test_driver->registerOption(
      quickstep::optimizer::DistributedExecutionGeneratorTestRunner::kResetOption);

  ::testing::InitGoogleTest(&argc, argv);
  const int success = RUN_ALL_TESTS();
  if (success != 0) {
    test_driver->writeActualOutputToFile(argv[2]);
  }

  return success;
}
