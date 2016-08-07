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

#include "utility/SqlError.hpp"

#include <memory>
#include <string>

#include "utility/Macros.hpp"

#include "gtest/gtest.h"

#define EXPECT_SQL_ERROR_WITH_EXPECTED_MESSAGE(line_number, column_number)       \
  sql_error_.reset(new SqlError(line_number, column_number, error_message_));    \
  EXPECT_EQ(expected_formatted_message_, sql_error_->formatMessage(sql_query_))

#ifdef QUICKSTEP_DEBUG
#define DEATH_TEST_IN_DEBUG_MODE(statement, regex) EXPECT_DEBUG_DEATH(statement, regex)
#else
#define DEATH_TEST_IN_DEBUG_MODE(statement, regex) DLOG(DFATAL)
#endif

namespace quickstep {

class SqlErrorTest : public ::testing::Test {
 protected:
  SqlErrorTest()
      : sql_query_("0123456789\nabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"),
        error_message_("Test error message") {
  }

  void setSqlErrorAtPosition(int line_number, int column_number) {
    sql_error_.reset(new SqlError(line_number, column_number, error_message_));
  }

  const std::string sql_query_;

  std::unique_ptr<const SqlError> sql_error_;
  const std::string error_message_;

  std::string expected_formatted_message_;

 private:
  DISALLOW_COPY_AND_ASSIGN(SqlErrorTest);
};

TEST_F(SqlErrorTest, SimpleTests) {
  expected_formatted_message_ =
      "ERROR: Test error message (1 : 1)\n"
      "0123456789\n"
      "^\n";
  EXPECT_SQL_ERROR_WITH_EXPECTED_MESSAGE(0, 0);

  expected_formatted_message_ =
      "ERROR: Test error message (1 : 10)\n"
      "0123456789\n"
      "         ^\n";
  EXPECT_SQL_ERROR_WITH_EXPECTED_MESSAGE(0, 9);

  expected_formatted_message_ =
      "ERROR: Test error message (2 : 1)\n"
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXY\n"
      "^\n";
  EXPECT_SQL_ERROR_WITH_EXPECTED_MESSAGE(1, 0);

  expected_formatted_message_ =
      "ERROR: Test error message (2 : 52)\n"
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
      "                                                   ^\n";
  EXPECT_SQL_ERROR_WITH_EXPECTED_MESSAGE(1, 51);

  // This is a weird case. We are pointing at newline '\n'.
  expected_formatted_message_ =
      "ERROR: Test error message (1 : 11)\n"
      "0123456789\n"
      "          ^\n";
  EXPECT_SQL_ERROR_WITH_EXPECTED_MESSAGE(0, 10);
  setSqlErrorAtPosition(1, 52);
  DEATH_TEST_IN_DEBUG_MODE(sql_error_->formatMessage(sql_query_), "Invalid line and column number")
      << sql_error_->formatMessage(sql_query_);

  setSqlErrorAtPosition(2, 0);
  DEATH_TEST_IN_DEBUG_MODE(sql_error_->formatMessage(sql_query_), "Invalid line number")
      << sql_error_->formatMessage(sql_query_);

  setSqlErrorAtPosition(0, 11);
  DEATH_TEST_IN_DEBUG_MODE(sql_error_->formatMessage(sql_query_), "Invalid line and column number");

  setSqlErrorAtPosition(0, -1);
  DEATH_TEST_IN_DEBUG_MODE(sql_error_->formatMessage(sql_query_), "Invalid column number");
}

TEST_F(SqlErrorTest, ErrorBeyondTheLastLine) {
  const std::string sql_query_with_newline = sql_query_ + "\n";

  // We allow this case, since BISON may generate such an error.
  // We move the original reported error location backward.
  expected_formatted_message_ =
      "ERROR: Test error message (2 : 53)\n"
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
      "                                                    ^\n";
  setSqlErrorAtPosition(2, 0);
  EXPECT_EQ(expected_formatted_message_, sql_error_->formatMessage(sql_query_with_newline));

  setSqlErrorAtPosition(2, 1);
  DEATH_TEST_IN_DEBUG_MODE(sql_error_->formatMessage(sql_query_with_newline), "Invalid line and column number");
}

TEST_F(SqlErrorTest, ErrorShallNotShowPartialWord) {
  const std::string sql_query_with_words =
      "aaa bbb ccc ddd eee fff ggg hhh iii jjj aaa bbb ccc ddd eee fff ggg hhh iii jjj aaa bbb ccc ddd eee fff ggg";

  setSqlErrorAtPosition(0, 45);
  std::string expected_error_message =
      "ERROR: Test error message (1 : 46)\n"
      "...bbb ccc ddd eee fff ggg hhh iii jjj aaa bbb ccc ddd eee fff ggg hhh iii...\n"
      "                                            ^\n";
  EXPECT_EQ(expected_error_message, sql_error_->formatMessage(sql_query_with_words));
}

TEST_F(SqlErrorTest, SqlStringWithTab) {
  const std::string sql_query_with_tab = "0123\t567890";

  setSqlErrorAtPosition(0, 5);
  std::ostringstream expected_error_message;
  expected_error_message << "ERROR: Test error message (1 : 6)\n";
  expected_error_message << "0123\t567890\n";
  expected_error_message << "    \t^\n";
  EXPECT_EQ(expected_error_message.str(), sql_error_->formatMessage(sql_query_with_tab));
}

}  // namespace quickstep
