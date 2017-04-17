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

#include <numeric>
#include <string>
#include <vector>

#include "cli/LineReaderBuffered.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

namespace quickstep {

/**
 * Create a string of several SQL statements and expect that they are individually parsed out.
 */
TEST(NetworkIOTest, TestLineReaderBuffered) {
  LineReaderBuffered linereader;
  EXPECT_TRUE(linereader.bufferEmpty());

  std::vector<std::string> statements_orig = {"select * from foo;", "select 1;", "select 2;", "quit;"};
  std::string statements_str;
  statements_str = accumulate(statements_orig.begin(), statements_orig.end(), statements_str);
  linereader.setBuffer(statements_str);
  ASSERT_FALSE(linereader.bufferEmpty());

  std::vector<std::string> statements_parsed;
  std::size_t parsed_commands;
  for (parsed_commands = 0;
       parsed_commands < statements_orig.size() + 1 && !linereader.bufferEmpty();
       parsed_commands++) {
    std::string command = linereader.getNextCommand();
    if (!command.empty()) {
      statements_parsed.push_back(command);
    }
  }

  ASSERT_EQ(statements_parsed.size(), statements_orig.size());
  for (std::size_t i = 0; i < statements_parsed.size(); ++i) {
    EXPECT_EQ(statements_orig[i], statements_parsed[i]);
  }
  EXPECT_TRUE(linereader.bufferEmpty());
}

}  // namespace quickstep

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  ::testing::InitGoogleTest(&argc, argv);
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  return RUN_ALL_TESTS();
}
