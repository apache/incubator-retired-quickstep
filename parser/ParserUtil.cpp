/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "parser/ParserUtil.hpp"

#include "utility/SqlError.hpp"

// From SqlLexer
extern int quickstep_yyget_lineno(void *yyscanner);
extern int quickstep_yyget_column(void *yyscanner);

// From SqlParser
extern int quickstep_yyget_line_number(const YYLTYPE *yyloc);
extern int quickstep_yyget_column_number(const YYLTYPE *yyloc);

void quickstep_yyerror(const YYLTYPE *yyloc,
                       void *yyscanner,
                       quickstep::ParseStatement **statement,
                       const char *error_message) {
  // If the error location is explicitly provided, use it;
  // otherwise use the location stored in yyscanner.
  if (yyloc != nullptr) {
    throw ::quickstep::SqlError(quickstep_yyget_line_number(yyloc), quickstep_yyget_column_number(yyloc),
                                error_message);
  } else {
    throw ::quickstep::SqlError(quickstep_yyget_lineno(yyscanner), quickstep_yyget_column(yyscanner),
                                error_message);
  }
}
