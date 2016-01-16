/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_PARSER_PARSER_UTIL_HPP_
#define QUICKSTEP_PARSER_PARSER_UTIL_HPP_

namespace quickstep {
class ParseStatement;
}

struct YYLTYPE;

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief Report a parsing error to STDERR.
 *
 * @param yyloc The error location, which may not be the same as the location in
 *              the token currently being scanned.
 * @param yyscanner The scanner context.
 * @param parsed_statement This parameter exists purely so that yyerror() will
 *        have the type signature expected by the parser, and is ignored.
 * @param s A string describing the error encountered.
 **/
void quickstep_yyerror(const YYLTYPE *yyloc,
                       void *yyscanner,
                       quickstep::ParseStatement **statement,
                       const char *error_message);

/** @} */

#endif  // QUICKSTEP_PARSER_PARSER_UTIL_HPP_
