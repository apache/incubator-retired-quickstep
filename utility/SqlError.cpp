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

#include <cstddef>
#include <cctype>
#include <limits>
#include <sstream>
#include <string>

#include "glog/logging.h"

namespace quickstep {

SqlError& SqlError::operator<<(const std::string &str) {
  error_stream_ << str;
  return *this;
}

std::string SqlError::formatMessage(const std::string &sql_query) const {
  CHECK(!sql_query.empty()) << "SQL query is empty";
  CHECK_LT(sql_query.size(), static_cast<std::size_t>(std::numeric_limits<int>::max()));

  int error_line_number = line_number_;
  int error_column_number = column_number_;

  if (error_line_number == -1) {
    // If the error location is not available,
    // just append the error message directly.
    return std::string("ERROR: ").append(error_stream_.str()).append("\n");
  }

  DCHECK_GT(error_column_number, -1) << "Invalid column number";

  int current_line_number = 0;
  int current_line_begin_pos = 0;
  int current_line_end_pos = -1;

  // Find the ending index of the (<error_line_number>-1)-th line
  // of <sql_query>.
  if (current_line_number < error_line_number) {
    while (current_line_number < error_line_number) {
      current_line_begin_pos = sql_query.find('\n', current_line_begin_pos);
      DCHECK_GE(current_line_begin_pos, 0) << "Invalid line number";
      ++current_line_number;
      ++current_line_begin_pos;
    }
  }

  /*
   * The BISON may point the error to a position beyond the last line of
   * the SQL query. We move it to the position to the end of the last line.
   */
  if (current_line_begin_pos == static_cast<int>(sql_query.size()) && column_number_ == 0) {
    current_line_end_pos = current_line_begin_pos - 1;
    current_line_begin_pos = sql_query.rfind('\n', current_line_end_pos - 1) + 1;

    // Move the error line to the previous line.
    --error_line_number;
    error_column_number = current_line_end_pos - current_line_begin_pos;
  } else {
    current_line_end_pos = sql_query.find('\n', current_line_begin_pos);
    if (current_line_end_pos == -1) {
      current_line_end_pos = sql_query.size() - 1;
    }
    DCHECK(current_line_end_pos - current_line_begin_pos + 1 > column_number_) << "Invalid line and column number";
  }

  std::ostringstream error_stream;
  const int start_pos = getStartErrorPos(error_column_number + current_line_begin_pos, sql_query);
  const int end_pos = getEndErrorPos(error_column_number + current_line_begin_pos, sql_query);

  DCHECK_LE(start_pos, error_column_number + current_line_begin_pos);
  DCHECK_LE(error_column_number + current_line_begin_pos, end_pos);

  error_stream << "ERROR: " << getErrorMessage();
  error_stream << " (" << error_line_number + 1 << " : " << error_column_number + 1 << ")\n";

  // Append the snippet text.
  bool has_omitted_text = false;
  if (start_pos > current_line_begin_pos) {
    error_stream << "...";
    has_omitted_text = true;
  }
  error_stream << sql_query.substr(start_pos, end_pos - start_pos);
  if (end_pos < current_line_end_pos) {
    error_stream << "...";
  }
  error_stream << "\n";

  // Append a caret.
  if (has_omitted_text) {
    error_stream << "   ";
  }
  for (int i = start_pos; i < error_column_number + current_line_begin_pos; ++i) {
    if (sql_query.at(i) == '\t') {
      error_stream << "\t";
    } else {
      error_stream << " ";
    }
  }
  error_stream << "^\n";

  return error_stream.str();
}

int SqlError::getStartErrorPos(int center_pos, const std::string &sql_statement) const {
  if (center_pos == 0) {
    return 0;
  }

  int start_pos = center_pos - kLeftCharSizeInErrorMessage;
  const int current_line_begin_pos = sql_statement.rfind('\n', center_pos - 1) + 1;
  if (start_pos < current_line_begin_pos) {
    start_pos = current_line_begin_pos;
  }

  if (isalnum(sql_statement.at(start_pos))) {
    while (start_pos > 0 && isalnum(sql_statement.at(start_pos - 1))) {
      --start_pos;
    }
  }

  return start_pos;
}

int SqlError::getEndErrorPos(int center_pos, const std::string &sql_statement) const {
  int end_pos = center_pos + kRightCharSizeInErrorMessage;

  int current_line_end_pos = sql_statement.find('\n', center_pos);
  if (current_line_end_pos == -1) {
    current_line_end_pos = sql_statement.size();
  }

  if (end_pos > current_line_end_pos) {
    end_pos = current_line_end_pos;
  }

  if (end_pos < static_cast<int>(sql_statement.size()) && std::isalnum(sql_statement.at(end_pos))) {
    while (end_pos < static_cast<int>(sql_statement.size()) - 1 && std::isalnum(sql_statement.at(end_pos + 1))) {
      ++end_pos;
    }
  }

  return end_pos;
}

}  // namespace quickstep
