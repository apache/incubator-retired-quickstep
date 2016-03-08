/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_QUICKSTEP_YY_SQLPARSER_GEN_HPP_INCLUDED
# define YY_QUICKSTEP_YY_SQLPARSER_GEN_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int quickstep_yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOKEN_COMMAND = 258,
    TOKEN_NAME = 259,
    TOKEN_STRING_SINGLE_QUOTED = 260,
    TOKEN_STRING_DOUBLE_QUOTED = 261,
    TOKEN_UNSIGNED_NUMVAL = 262,
    TOKEN_OR = 263,
    TOKEN_AND = 264,
    TOKEN_NOT = 265,
    TOKEN_EQ = 266,
    TOKEN_LT = 267,
    TOKEN_LEQ = 268,
    TOKEN_GT = 269,
    TOKEN_GEQ = 270,
    TOKEN_NEQ = 271,
    TOKEN_LIKE = 272,
    TOKEN_BETWEEN = 273,
    TOKEN_IS = 274,
    UNARY_PLUS = 275,
    UNARY_MINUS = 276,
    TOKEN_ADD = 277,
    TOKEN_ALL = 278,
    TOKEN_ALTER = 279,
    TOKEN_AS = 280,
    TOKEN_ASC = 281,
    TOKEN_BIGINT = 282,
    TOKEN_BIT = 283,
    TOKEN_BLOCKPROPERTIES = 284,
    TOKEN_BLOCKSAMPLE = 285,
    TOKEN_BLOOM_FILTER = 286,
    TOKEN_CSB_TREE = 287,
    TOKEN_BY = 288,
    TOKEN_CHARACTER = 289,
    TOKEN_CHECK = 290,
    TOKEN_COLUMN = 291,
    TOKEN_CONSTRAINT = 292,
    TOKEN_COPY = 293,
    TOKEN_CREATE = 294,
    TOKEN_DATE = 295,
    TOKEN_DATETIME = 296,
    TOKEN_DECIMAL = 297,
    TOKEN_DEFAULT = 298,
    TOKEN_DELETE = 299,
    TOKEN_DELIMITER = 300,
    TOKEN_DESC = 301,
    TOKEN_DISTINCT = 302,
    TOKEN_DOUBLE = 303,
    TOKEN_DROP = 304,
    TOKEN_ESCAPE_STRINGS = 305,
    TOKEN_FALSE = 306,
    TOKEN_FIRST = 307,
    TOKEN_FLOAT = 308,
    TOKEN_FOREIGN = 309,
    TOKEN_FROM = 310,
    TOKEN_FULL = 311,
    TOKEN_GROUP = 312,
    TOKEN_HAVING = 313,
    TOKEN_INDEX = 314,
    TOKEN_INNER = 315,
    TOKEN_INSERT = 316,
    TOKEN_INTEGER = 317,
    TOKEN_INTERVAL = 318,
    TOKEN_INTO = 319,
    TOKEN_JOIN = 320,
    TOKEN_KEY = 321,
    TOKEN_LAST = 322,
    TOKEN_LEFT = 323,
    TOKEN_LIMIT = 324,
    TOKEN_LONG = 325,
    TOKEN_NULL = 326,
    TOKEN_NULLS = 327,
    TOKEN_OFF = 328,
    TOKEN_ON = 329,
    TOKEN_ORDER = 330,
    TOKEN_OUTER = 331,
    TOKEN_PERCENT = 332,
    TOKEN_PRIMARY = 333,
    TOKEN_QUIT = 334,
    TOKEN_REAL = 335,
    TOKEN_REFERENCES = 336,
    TOKEN_RIGHT = 337,
    TOKEN_ROW_DELIMITER = 338,
    TOKEN_SELECT = 339,
    TOKEN_SET = 340,
    TOKEN_SMALLINT = 341,
    TOKEN_TABLE = 342,
    TOKEN_TIME = 343,
    TOKEN_TIMESTAMP = 344,
    TOKEN_TRUE = 345,
    TOKEN_TUPLESAMPLE = 346,
    TOKEN_UNIQUE = 347,
    TOKEN_UPDATE = 348,
    TOKEN_USING = 349,
    TOKEN_VALUES = 350,
    TOKEN_VARCHAR = 351,
    TOKEN_WHERE = 352,
    TOKEN_WITH = 353,
    TOKEN_YEARMONTH = 354,
    TOKEN_EOF = 355,
    TOKEN_LEX_ERROR = 356
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 110 "../SqlParser.ypp" /* yacc.c:1909  */

  quickstep::ParseString *string_value_;

  quickstep::PtrList<quickstep::ParseString> *string_list_;

  bool boolean_value_;

  quickstep::NumericParseLiteralValue *numeric_literal_value_;
  quickstep::ParseLiteralValue *literal_value_;
  quickstep::PtrList<quickstep::ParseScalarLiteral> *literal_value_list_;

  quickstep::ParseExpression *expression_;

  quickstep::ParseScalarLiteral *scalar_literal_;
  quickstep::ParseAttribute *attribute_;

  quickstep::ParsePredicate *predicate_;

  quickstep::ParseSubqueryExpression *subquery_expression_;

  quickstep::ParseSelectionClause *selection_;
  quickstep::ParseSelectionItem *selection_item_;
  quickstep::ParseSelectionList *selection_list_;

  quickstep::ParseTableReference *table_reference_;
  quickstep::PtrList<quickstep::ParseTableReference> *table_reference_list_;
  quickstep::ParseTableReferenceSignature *table_reference_signature_;

  quickstep::ParseDataType *data_type_;
  quickstep::ParseAttributeDefinition *attribute_definition_;
  quickstep::ParseColumnConstraint *column_constraint_;
  quickstep::PtrList<quickstep::ParseColumnConstraint> *column_constraint_list_;
  quickstep::PtrList<quickstep::ParseAttributeDefinition> *attribute_definition_list_;

  quickstep::ParseKeyValue *key_value_;
  quickstep::PtrList<quickstep::ParseKeyValue> *key_value_list_;
  quickstep::ParseKeyStringValue *key_string_value_;
  quickstep::ParseKeyStringList *key_string_list_;
  quickstep::ParseKeyIntegerValue *key_integer_value_;

  quickstep::ParseCopyFromParams *copy_from_params_;

  quickstep::ParseAssignment *assignment_;
  quickstep::PtrList<quickstep::ParseAssignment> *assignment_list_;

  quickstep::ParseCommand *command_;
  quickstep::PtrVector<quickstep::ParseString> *command_argument_list_;

  quickstep::ParseStatement *statement_;
  quickstep::ParseStatementSelect *select_statement_;
  quickstep::ParseStatementUpdate *update_statement_;
  quickstep::ParseStatementInsert *insert_statement_;
  quickstep::ParseStatementDelete *delete_statement_;
  quickstep::ParseStatementCopyFrom *copy_from_statement_;
  quickstep::ParseStatementCreateTable *create_table_statement_;
  quickstep::ParseBlockProperties *block_properties_;
  quickstep::ParseStatementDropTable *drop_table_statement_;
  quickstep::ParseStatementQuit *quit_statement_;

  const quickstep::Comparison *comparison_;
  const quickstep::UnaryOperation *unary_operation_;
  const quickstep::BinaryOperation *binary_operation_;

  quickstep::ParseFunctionCall *function_call_;
  quickstep::PtrList<quickstep::ParseExpression> *expression_list_;

  quickstep::ParseSelect *select_query_;
  quickstep::ParseGroupBy *opt_group_by_clause_;
  quickstep::ParseHaving *opt_having_clause_;
  quickstep::ParseOrderBy *opt_order_by_clause_;
  bool *order_direction_;
  quickstep::ParseLimit *opt_limit_clause_;
  
  quickstep::ParseSample *opt_sample_clause_;

  quickstep::PtrList<quickstep::ParseOrderByItem> *order_commalist_;
  quickstep::ParseOrderByItem *order_item_;

  quickstep::PtrVector<quickstep::ParseSubqueryTableReference> *with_list_;
  quickstep::ParseSubqueryTableReference *with_list_element_;

#line 238 "SqlParser_gen.hpp" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int quickstep_yyparse (yyscan_t yyscanner, quickstep::ParseStatement **parsedStatement);

#endif /* !YY_QUICKSTEP_YY_SQLPARSER_GEN_HPP_INCLUDED  */
