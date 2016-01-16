/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_PARSER_SQL_PARSER_WRAPPER_HPP_
#define QUICKSTEP_PARSER_SQL_PARSER_WRAPPER_HPP_

#include <string>

#include "utility/Macros.hpp"

struct yy_buffer_state;

namespace quickstep {

class ParseStatement;

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief Struct containing the results of a parse.
 **/
struct ParseResult {
  enum Condition {
    kSuccess,
    kError,
    kEndOfInput
  };

  /**
   * @brief The outcome of the parse.
   **/
  Condition condition;

  /**
   * @brief If condition is Success, this is the parsed statement, otherwise
   *        it is NULL.
   **/
  const ParseStatement *parsed_statement;

  /**
   * @brief The error message reported during the parsing. Non-empty only when the condition
   *        is kError.
   */
  std::string error_message;
};

/**
 * @brief Wrapper encapsulating the Bison-generated parser and Flex-generated
 *        lexer.
 * @note The parser is reentrant, so an unlimited number of these wrappers can
 *       be safely created and used at the same time.
 **/
class SqlParserWrapper {
 public:
  /**
   * @brief Constructor.
   **/
  SqlParserWrapper();

  /**
   * @brief Destructor.
   **/
  ~SqlParserWrapper();

  /**
   * @brief Feed the next string buffer to parse into the parser. In normal
   *        operation, this should be called when starting parsing, and when
   *        getNextStatement() starts returning a condition other than kSuccess.
   *
   * @param buffer The next string buffer to parse. This SqlParserWrapper takes
   *        ownership of buffer and will destroy it when it is no longer
   *        needed.
   **/
  void feedNextBuffer(std::string *buffer);

  /**
   * @brief Parse the next statement from the current buffer.
   * @warning A buffer must be supplied to the SqlParserWrapper with
   *          feedNextBuffer() before calling this method.
   *
   * @return The outcome of the parse.
   **/
  ParseResult getNextStatement();

  /**
   * @brief Returns the current string buffer.
   *
   * @return The current string buffer.
   */
  const std::string* current_buffer() const {
    return current_buffer_;
  }

 private:
  void destroyCurrentBuffer();

  std::string *current_buffer_;
  yy_buffer_state *buffer_state_;
  ParseStatement *parsed_statement_;
  void *yyscanner_;

  DISALLOW_COPY_AND_ASSIGN(SqlParserWrapper);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_PARSER_SQL_PARSER_WRAPPER_HPP_
