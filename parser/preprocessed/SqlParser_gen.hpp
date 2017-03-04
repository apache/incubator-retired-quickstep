/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
    TOKEN_REGEXP = 273,
    TOKEN_BETWEEN = 274,
    TOKEN_IS = 275,
    UNARY_PLUS = 276,
    UNARY_MINUS = 277,
    TOKEN_ALL = 278,
    TOKEN_UNION = 279,
    TOKEN_INTERSECT = 280,
    TOKEN_ADD = 281,
    TOKEN_ALTER = 282,
    TOKEN_AS = 283,
    TOKEN_ASC = 284,
    TOKEN_BIGINT = 285,
    TOKEN_BIT = 286,
    TOKEN_BITWEAVING = 287,
    TOKEN_BLOCKPROPERTIES = 288,
    TOKEN_BLOCKSAMPLE = 289,
    TOKEN_BLOOM_FILTER = 290,
    TOKEN_CSB_TREE = 291,
    TOKEN_BY = 292,
    TOKEN_CASE = 293,
    TOKEN_CAST = 294,
    TOKEN_CHARACTER = 295,
    TOKEN_CHECK = 296,
    TOKEN_COLUMN = 297,
    TOKEN_CONSTRAINT = 298,
    TOKEN_COPY = 299,
    TOKEN_CREATE = 300,
    TOKEN_CURRENT = 301,
    TOKEN_DATE = 302,
    TOKEN_DATETIME = 303,
    TOKEN_DAY = 304,
    TOKEN_DECIMAL = 305,
    TOKEN_DEFAULT = 306,
    TOKEN_DELETE = 307,
    TOKEN_DESC = 308,
    TOKEN_DISTINCT = 309,
    TOKEN_DOUBLE = 310,
    TOKEN_DROP = 311,
    TOKEN_ELSE = 312,
    TOKEN_END = 313,
    TOKEN_EXISTS = 314,
    TOKEN_EXTRACT = 315,
    TOKEN_FALSE = 316,
    TOKEN_FIRST = 317,
    TOKEN_FLOAT = 318,
    TOKEN_FOLLOWING = 319,
    TOKEN_FOR = 320,
    TOKEN_FOREIGN = 321,
    TOKEN_FROM = 322,
    TOKEN_FULL = 323,
    TOKEN_GROUP = 324,
    TOKEN_HASH = 325,
    TOKEN_HAVING = 326,
    TOKEN_HOUR = 327,
    TOKEN_IN = 328,
    TOKEN_INDEX = 329,
    TOKEN_INNER = 330,
    TOKEN_INSERT = 331,
    TOKEN_INTEGER = 332,
    TOKEN_INTERVAL = 333,
    TOKEN_INTO = 334,
    TOKEN_JOIN = 335,
    TOKEN_KEY = 336,
    TOKEN_LAST = 337,
    TOKEN_LEFT = 338,
    TOKEN_LIMIT = 339,
    TOKEN_LONG = 340,
    TOKEN_MINUTE = 341,
    TOKEN_MONTH = 342,
    TOKEN_NULL = 343,
    TOKEN_NULLS = 344,
    TOKEN_OFF = 345,
    TOKEN_ON = 346,
    TOKEN_ORDER = 347,
    TOKEN_OUTER = 348,
    TOKEN_OVER = 349,
    TOKEN_PARTITION = 350,
    TOKEN_PARTITIONS = 351,
    TOKEN_PERCENT = 352,
    TOKEN_PRECEDING = 353,
    TOKEN_PRIMARY = 354,
    TOKEN_PRIORITY = 355,
    TOKEN_QUIT = 356,
    TOKEN_RANGE = 357,
    TOKEN_REAL = 358,
    TOKEN_REFERENCES = 359,
    TOKEN_RIGHT = 360,
    TOKEN_ROW = 361,
    TOKEN_ROW_DELIMITER = 362,
    TOKEN_ROWS = 363,
    TOKEN_SECOND = 364,
    TOKEN_SELECT = 365,
    TOKEN_SET = 366,
    TOKEN_SMA = 367,
    TOKEN_SMALLINT = 368,
    TOKEN_STDERR = 369,
    TOKEN_STDOUT = 370,
    TOKEN_SUBSTRING = 371,
    TOKEN_TABLE = 372,
    TOKEN_THEN = 373,
    TOKEN_TIME = 374,
    TOKEN_TIMESTAMP = 375,
    TOKEN_TO = 376,
    TOKEN_TRUE = 377,
    TOKEN_TUPLESAMPLE = 378,
    TOKEN_UNBOUNDED = 379,
    TOKEN_UNIQUE = 380,
    TOKEN_UPDATE = 381,
    TOKEN_USING = 382,
    TOKEN_VALUES = 383,
    TOKEN_VARCHAR = 384,
    TOKEN_WHEN = 385,
    TOKEN_WHERE = 386,
    TOKEN_WINDOW = 387,
    TOKEN_WITH = 388,
    TOKEN_YEAR = 389,
    TOKEN_YEARMONTH = 390,
    TOKEN_EOF = 391,
    TOKEN_LEX_ERROR = 392
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 115 "../SqlParser.ypp" /* yacc.c:1915  */

  quickstep::ParseString *string_value_;

  quickstep::PtrList<quickstep::ParseString> *string_list_;

  bool boolean_value_;

  quickstep::NumericParseLiteralValue *numeric_literal_value_;
  quickstep::ParseLiteralValue *literal_value_;
  quickstep::PtrList<quickstep::ParseScalarLiteral> *literal_value_list_;

  quickstep::ParseExpression *expression_;

  quickstep::ParseScalarLiteral *scalar_literal_;
  quickstep::ParseAttribute *attribute_;
  quickstep::PtrList<quickstep::ParseAttribute> *attribute_list_;

  quickstep::ParsePredicate *predicate_;

  quickstep::ParseSubqueryExpression *subquery_expression_;

  quickstep::PtrVector<quickstep::ParseSimpleWhenClause> *simple_when_clause_list_;
  quickstep::ParseSimpleWhenClause *simple_when_clause_;

  quickstep::PtrVector<quickstep::ParseSearchedWhenClause> *searched_when_clause_list_;
  quickstep::ParseSearchedWhenClause *searched_when_clause_;

  quickstep::ParseSelectionClause *selection_;
  quickstep::ParseSelectionItem *selection_item_;
  quickstep::ParseSelectionList *selection_list_;

  quickstep::ParseSetOperation *set_operation_;

  quickstep::ParseTableReference *table_reference_;
  quickstep::PtrList<quickstep::ParseTableReference> *table_reference_list_;
  quickstep::ParseTableReferenceSignature *table_reference_signature_;

  quickstep::ParseJoinedTableReference::JoinType join_type_;

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
  quickstep::ParseKeyBoolValue *key_bool_value_;

  quickstep::ParseAssignment *assignment_;
  quickstep::PtrList<quickstep::ParseAssignment> *assignment_list_;

  quickstep::ParseCommand *command_;
  quickstep::PtrVector<quickstep::ParseString> *command_argument_list_;

  quickstep::ParseStatement *statement_;
  quickstep::ParseStatementSetOperation *set_operation_statement_;
  quickstep::ParseStatementUpdate *update_statement_;
  quickstep::ParseStatementInsert *insert_statement_;
  quickstep::ParseStatementDelete *delete_statement_;
  quickstep::ParseStatementCopy *copy_statement_;
  quickstep::ParseStatementCreateTable *create_table_statement_;
  quickstep::ParsePartitionClause *partition_clause_;
  quickstep::ParseBlockProperties *block_properties_;
  quickstep::ParseStatementDropTable *drop_table_statement_;
  quickstep::ParseStatementQuit *quit_statement_;

  const quickstep::Comparison *comparison_;
  quickstep::ParseString *unary_operation_;
  quickstep::ParseString *binary_operation_;

  quickstep::ParseFunctionCall *function_call_;
  quickstep::PtrList<quickstep::ParseExpression> *expression_list_;

  quickstep::ParseSelect *select_query_;
  quickstep::ParseGroupBy *opt_group_by_clause_;
  quickstep::ParseHaving *opt_having_clause_;
  quickstep::ParseOrderBy *opt_order_by_clause_;
  bool *order_direction_;
  quickstep::ParseLimit *opt_limit_clause_;

  quickstep::ParseSample *opt_sample_clause_;

  quickstep::PtrList<quickstep::ParseWindow> *opt_window_clause_;
  quickstep::ParseWindow *window_definition_;
  quickstep::PtrList<quickstep::ParseExpression> *window_partition_by_list_;
  quickstep::PtrList<quickstep::ParseOrderByItem> *window_order_by_list_;
  quickstep::ParseFrameInfo *window_frame_info_;

  quickstep::PtrList<quickstep::ParseOrderByItem> *order_commalist_;
  quickstep::ParseOrderByItem *order_item_;

  quickstep::PtrVector<quickstep::ParseSubqueryTableReference> *with_list_;
  quickstep::ParseSubqueryTableReference *with_list_element_;

  quickstep::ParsePriority *opt_priority_clause_;

#line 293 "SqlParser_gen.hpp" /* yacc.c:1915  */
};

typedef union YYSTYPE YYSTYPE;
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
