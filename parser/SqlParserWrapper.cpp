/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "parser/SqlParserWrapper.hpp"

#include <string>

#include "parser/ParseStatement.hpp"
#include "utility/Macros.hpp"
#include "utility/SqlError.hpp"

#include "glog/logging.h"

// Forward declarations needed for preprocessed/SqlParser_gen.hpp.
namespace quickstep {

class BinaryOperation;
class Comparison;
class NumericParseLiteralValue;
class ParseAssignment;
class ParseAttribute;
class ParseAttributeDefinition;
class ParseColumnConstraint;
class ParseDataType;
class ParseFunctionCall;
class ParseLiteralValue;
class ParsePredicate;
class ParseScalarLiteral;
class ParseSelection;
class ParseSelectionItem;
class ParseSelectionList;
class ParseTableReference;
class Type;
class UnaryOperation;

struct ParseBulkInsertParams;

template<class T> class PtrList;

}  // namespace quickstep

// Needed for Bison 2.6 and higher.
typedef void* yyscan_t;

#include "parser/SqlParser_gen.hpp"
#include "parser/SqlLexer_gen.hpp"

int quickstep_yyparse(yyscan_t yyscanner, quickstep::ParseStatement **parsedStatement);
void quickstep_yyset_column(int column_no, yyscan_t yyscanner);

namespace quickstep {

SqlParserWrapper::SqlParserWrapper()
    : current_buffer_(nullptr),
      buffer_state_(nullptr),
      parsed_statement_(nullptr) {
  if (quickstep_yylex_init(&yyscanner_)) {
    FATAL_ERROR("Unable to create lexer context.");
  }
}

SqlParserWrapper::~SqlParserWrapper() {
  destroyCurrentBuffer();

  if (parsed_statement_ != nullptr) {
    delete parsed_statement_;
  }

  quickstep_yylex_destroy(yyscanner_);
}

void SqlParserWrapper::feedNextBuffer(std::string *buffer) {
  DCHECK(buffer != nullptr);
  destroyCurrentBuffer();

  current_buffer_ = buffer;
  buffer_state_ = quickstep_yy_scan_string(buffer->c_str(), yyscanner_);

  quickstep_yyset_lineno(0, yyscanner_);
  quickstep_yyset_column(0, yyscanner_);
}

ParseResult SqlParserWrapper::getNextStatement() {
  DCHECK(buffer_state_ != nullptr);
  DCHECK(current_buffer_ != nullptr);
  ParseResult retval;
  retval.parsed_statement = nullptr;

  if (parsed_statement_ != nullptr) {
    delete parsed_statement_;
    parsed_statement_ = nullptr;
  }

  int parser_return;
  try {
    parser_return = quickstep_yyparse(yyscanner_, &parsed_statement_);
  } catch (const SqlError& error) {
    parser_return = 1;
    retval.error_message = error.formatMessage(*current_buffer_);
  }

  if (parser_return == 0) {
    DCHECK(parsed_statement_ != nullptr);
    retval.condition = ParseResult::kSuccess;
    retval.parsed_statement = parsed_statement_;
  } else if (parser_return > 0) {
    retval.condition = ParseResult::kError;
  } else {
    retval.condition = ParseResult::kEndOfInput;
  }

  return retval;
}

void SqlParserWrapper::destroyCurrentBuffer() {
  if (buffer_state_ != nullptr) {
    quickstep_yyset_lineno(0, yyscanner_);
    quickstep_yy_delete_buffer(buffer_state_, yyscanner_);
    buffer_state_ = nullptr;
  }
  if (current_buffer_ != nullptr) {
    delete current_buffer_;
    current_buffer_ = nullptr;
  }
}

}  // namespace quickstep
