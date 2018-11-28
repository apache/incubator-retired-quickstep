/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int quickstep_yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOKEN_COMMAND = 258,
     TOKEN_NAME = 259,
     TOKEN_STRING_SINGLE_QUOTED = 260,
     TOKEN_STRING_DOUBLE_QUOTED = 261,
     TOKEN_UNSIGNED_NUMVAL = 262,
     TOKEN_OR = 263,
     TOKEN_AND = 264,
     TOKEN_NOT = 265,
     TOKEN_EQ = 266,
     TOKEN_NEQ = 267,
     TOKEN_GEQ = 268,
     TOKEN_GT = 269,
     TOKEN_LEQ = 270,
     TOKEN_LT = 271,
     TOKEN_REGEXP = 272,
     TOKEN_LIKE = 273,
     TOKEN_BETWEEN = 274,
     TOKEN_IS = 275,
     UNARY_MINUS = 276,
     UNARY_PLUS = 277,
     TOKEN_ADD = 278,
     TOKEN_ALL = 279,
     TOKEN_ALTER = 280,
     TOKEN_AS = 281,
     TOKEN_ASC = 282,
     TOKEN_BIGINT = 283,
     TOKEN_BIT = 284,
     TOKEN_BITWEAVING = 285,
     TOKEN_BLOCKPROPERTIES = 286,
     TOKEN_BLOCKSAMPLE = 287,
     TOKEN_BLOOM_FILTER = 288,
     TOKEN_CSB_TREE = 289,
     TOKEN_BY = 290,
     TOKEN_CASE = 291,
     TOKEN_CHARACTER = 292,
     TOKEN_CHECK = 293,
     TOKEN_COLUMN = 294,
     TOKEN_CONSTRAINT = 295,
     TOKEN_COPY = 296,
     TOKEN_CREATE = 297,
     TOKEN_CURRENT = 298,
     TOKEN_DATE = 299,
     TOKEN_DATETIME = 300,
     TOKEN_DAY = 301,
     TOKEN_DECIMAL = 302,
     TOKEN_DEFAULT = 303,
     TOKEN_DELETE = 304,
     TOKEN_DELIMITER = 305,
     TOKEN_DESC = 306,
     TOKEN_DISTINCT = 307,
     TOKEN_DOUBLE = 308,
     TOKEN_DROP = 309,
     TOKEN_ELSE = 310,
     TOKEN_END = 311,
     TOKEN_ESCAPE_STRINGS = 312,
     TOKEN_EXISTS = 313,
     TOKEN_EXTRACT = 314,
     TOKEN_FALSE = 315,
     TOKEN_FIRST = 316,
     TOKEN_FLOAT = 317,
     TOKEN_FOLLOWING = 318,
     TOKEN_FOR = 319,
     TOKEN_FOREIGN = 320,
     TOKEN_FROM = 321,
     TOKEN_FULL = 322,
     TOKEN_GROUP = 323,
     TOKEN_HASH = 324,
     TOKEN_HAVING = 325,
     TOKEN_HOUR = 326,
     TOKEN_IN = 327,
     TOKEN_INDEX = 328,
     TOKEN_INNER = 329,
     TOKEN_INSERT = 330,
     TOKEN_INTEGER = 331,
     TOKEN_INTERVAL = 332,
     TOKEN_INTO = 333,
     TOKEN_JOIN = 334,
     TOKEN_KEY = 335,
     TOKEN_LAST = 336,
     TOKEN_LEFT = 337,
     TOKEN_LIMIT = 338,
     TOKEN_LONG = 339,
     TOKEN_MINUTE = 340,
     TOKEN_MONTH = 341,
     TOKEN_NULL = 342,
     TOKEN_NULLS = 343,
     TOKEN_OFF = 344,
     TOKEN_ON = 345,
     TOKEN_ORDER = 346,
     TOKEN_OUTER = 347,
     TOKEN_OVER = 348,
     TOKEN_PARTITION = 349,
     TOKEN_PARTITIONS = 350,
     TOKEN_PERCENT = 351,
     TOKEN_PRECEDING = 352,
     TOKEN_PRIMARY = 353,
     TOKEN_PRIORITY = 354,
     TOKEN_QUIT = 355,
     TOKEN_RANGE = 356,
     TOKEN_REAL = 357,
     TOKEN_REFERENCES = 358,
     TOKEN_RIGHT = 359,
     TOKEN_ROW = 360,
     TOKEN_ROW_DELIMITER = 361,
     TOKEN_ROWS = 362,
     TOKEN_SECOND = 363,
     TOKEN_SELECT = 364,
     TOKEN_SET = 365,
     TOKEN_SMA = 366,
     TOKEN_SMALLINT = 367,
     TOKEN_SUBSTRING = 368,
     TOKEN_TABLE = 369,
     TOKEN_THEN = 370,
     TOKEN_TIME = 371,
     TOKEN_TIMESTAMP = 372,
     TOKEN_TRUE = 373,
     TOKEN_TUPLESAMPLE = 374,
     TOKEN_UNBOUNDED = 375,
     TOKEN_UNIQUE = 376,
     TOKEN_UPDATE = 377,
     TOKEN_USING = 378,
     TOKEN_VALUES = 379,
     TOKEN_VARCHAR = 380,
     TOKEN_WHEN = 381,
     TOKEN_WHERE = 382,
     TOKEN_WINDOW = 383,
     TOKEN_WITH = 384,
     TOKEN_YEAR = 385,
     TOKEN_YEARMONTH = 386,
     TOKEN_EOF = 387,
     TOKEN_LEX_ERROR = 388
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2058 of yacc.c  */
#line 120 "../SqlParser.ypp"

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
  quickstep::ParsePartitionClause *partition_clause_;
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


/* Line 2058 of yacc.c  */
#line 291 "SqlParser_gen.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int quickstep_yyparse (void *YYPARSE_PARAM);
#else
int quickstep_yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int quickstep_yyparse (yyscan_t yyscanner, quickstep::ParseStatement **parsedStatement);
#else
int quickstep_yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_QUICKSTEP_YY_SQLPARSER_GEN_HPP_INCLUDED  */
