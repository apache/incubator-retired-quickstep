/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_UTILITY_SQL_ERROR_HPP_
#define QUICKSTEP_UTILITY_SQL_ERROR_HPP_

#include <exception>
#include <sstream>
#include <string>

#include "glog/logging.h"

/**
 * @brief Macros to construct and throw an error message.
 *
 * @details If the parser node where the error happens is available, use
 *          THROW_SQL_ERROR_AT(parser_node) << "Foo message";
 *          otherwise, use THROW_SQL_ERROR() << "Foo message".
 */
#define THROW_SQL_ERROR_AT(parser_node) \
  DCHECK((parser_node) != nullptr);  \
  throw ::quickstep::SqlError((parser_node)->line_number(), (parser_node)->column_number())

#define THROW_SQL_ERROR() \
  throw ::quickstep::SqlError()

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief Encapsulates an error message and optionally
 *        the error location in a SQL query.
 **/
class SqlError : public std::exception {
 public:
  /**
   * @brief The number of characters preceding to the error location
   *        that should be shown in the error message.
   */
  static const int kLeftCharSizeInErrorMessage = 40;
  /**
   * @brief The number of characters following the error location
   *        that should be shown in the error message.
   */
  static const int kRightCharSizeInErrorMessage = 30;

  /**
   * @brief Constructor.
   */
  SqlError(): line_number_(-1), column_number_(-1) {}

  /**
   * @brief Constructor.
   *
   * @param line_number The 0-based line number of the error location.
   * @param column_number The 0-based column number of the error location.
   * @param message The error message.
   */
  SqlError(int line_number, int column_number, const std::string &message = "")
      : line_number_(line_number),
        column_number_(column_number),
        error_stream_(message) {
  }

  /**
   * @brief Makes a copy of \p source.
   *
   * @param source The SqlError to make a copy from.
   */
  SqlError(const SqlError &source) {
    line_number_ = source.line_number_;
    column_number_ = source.column_number_;
    error_stream_ << source.error_stream_.str();
  }

  /**
   * @brief Destructor.
   */
  virtual ~SqlError() {}

  int line_number() const { return line_number_; }
  int column_number() const { return column_number_; }
  std::string getErrorMessage() const { return error_stream_.str(); }

  /**
   *
   * @brief Overrides the output stream operator to make it easy to construct
   *        error message. We currently only support string as the argument.
   *
   * @param str The string to be appended to the message.
   * @return The reference to this SqlError.
   */
  SqlError& operator<<(const std::string &str);

  /**
   * @brief Generates an error message with a caret pointing to the error location in \p sql_query.
   *
   * @param sql_query The SQL query that the error is associated with.
   * @return An error message with a caret pointing to the error location in \p sql_query.
   */
  std::string formatMessage(const std::string &sql_query) const;

 private:
  /**
   * @brief Computes the beginning position of the snippet text to show in the
   *        error message. It first computes the position by decreasing
   *        'center_pos' by kLeftCharSizeInErrorMessage. If the position is beyond
   *        the line of the error location, we use the start of the line.
   *        If the position points to the middle of a word, we move the position
   *        backward until reaching a non-alphanumeric character.
   *
   * @param center_pos The error position.
   * @param sql_statement The SQL query.
   * @return 0-based index in \p sql_query.
   */
  int getStartErrorPos(int center_pos, const std::string &sql_query) const;

  /**
   * @brief Computes the ending position of the snippet text to shown in the
   *        error message. It first computes the position by increasing
   *        'center_pos' by kRightCharSizeInErrorMessage. If the position is beyond
   *        the line of the error location, we use the end of the line.
   *        If the position points to the middle of a word, we move the position
   *        forward until reaching a non-alphanumeric character.
   *
   * @param center_pos The error position.
   * @param sql_query The SQL query.
   * @return 0-based index in \p sql_query.
   */
  int getEndErrorPos(int center_pos, const std::string &sql_query) const;

  int line_number_;
  int column_number_;
  std::ostringstream error_stream_;

  // Copyable.
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_UTILITY_SQL_ERROR_HPP_ */
