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
#include <set>
#include <string>

#include "parser/ParseStatement.hpp"
#include "parser/SqlParserWrapper.hpp"
#include "utility/Macros.hpp"
#include "utility/textbased_test/TextBasedTest.hpp"
#include "utility/textbased_test/TextBasedTestDriver.hpp"
#include "utility/textbased_test/TextBasedTestRunner.hpp"

#include "glog/logging.h"

#include "gtest/gtest.h"

namespace quickstep {

class SqlParserTestRunner : public TextBasedTestRunner {
 public:
  SqlParserTestRunner() {}

  void runTestCase(const std::string& input, const std::set<std::string> &options, std::string *output) override {
    CHECK(options.empty()) << "ParserTestRunner does not accept any options";

    VLOG(4) << "Parse SQL: " << input;

    sql_parser_.feedNextBuffer(new std::string(input));
    ParseResult result = sql_parser_.getNextStatement();

    if (result.condition != ParseResult::kSuccess) {
      *output = result.error_message;
    } else {
      *output = result.parsed_statement->toString();
    }
  }

 private:
  SqlParserWrapper sql_parser_;

  DISALLOW_COPY_AND_ASSIGN(SqlParserTestRunner);
};

QUICKSTEP_GENERATE_TEXT_TEST(PARSE_SQL_TEST);

}  // namespace quickstep

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);

  if (argc < 3) {
    LOG(ERROR) << "Must have at least 2 arguments, but " << argc-1 << " are provided";
  }

  std::ifstream input_file(argv[1]);
  CHECK(input_file.is_open()) << argv[1];
  std::unique_ptr<quickstep::TextBasedTestRunner> test_runner(new quickstep::SqlParserTestRunner);
  quickstep::test_driver.reset(new quickstep::TextBasedTestDriver(&input_file, test_runner.get()));

  ::testing::InitGoogleTest(&argc, argv);
  const int success = RUN_ALL_TESTS();
  if (success != 0) {
    quickstep::test_driver->writeActualOutputToFile(argv[2]);
  }

  return success;
}
