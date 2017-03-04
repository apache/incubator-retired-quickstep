/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         quickstep_yyparse
#define yylex           quickstep_yylex
#define yyerror         quickstep_yyerror
#define yydebug         quickstep_yydebug
#define yynerrs         quickstep_yynerrs


/* Copy the first part of user declarations.  */
#line 35 "../SqlParser.ypp" /* yacc.c:339  */


/* Override the default definition, as we only need <first_line> and <first_column>. */
typedef struct YYLTYPE {
  int first_line;
  int first_column;
} YYLTYPE;

#define YYLTYPE_IS_DECLARED 1

/*
 * Modified from the default YYLLOC_DEFAULT
 * (http://www.gnu.org/software/bison/manual/html_node/Location-Default-Action.html).
 * The assignments for last_line and last_column are removed as they are not used.
 */
#define YYLLOC_DEFAULT(current, rhs, n)                         \
  do {                                                          \
    if (n) {                                                    \
      (current).first_line   = YYRHSLOC(rhs, 1).first_line;     \
      (current).first_column = YYRHSLOC(rhs, 1).first_column;   \
    } else {                                                    \
      /* empty RHS */                                           \
      (current).first_line = YYRHSLOC(rhs, 0).first_line;       \
      (current).first_column = YYRHSLOC(rhs, 0).first_column;   \
    }                                                           \
  } while (0)

#line 64 "../SqlParser.ypp" /* yacc.c:339  */

#include <cstdlib>
#include <string>
#include <utility>

#include "catalog/PartitionSchemeHeader.hpp"
#include "parser/ParseAssignment.hpp"
#include "parser/ParseAttributeDefinition.hpp"
#include "parser/ParseBasicExpressions.hpp"
#include "parser/ParseBlockProperties.hpp"
#include "parser/ParseCaseExpressions.hpp"
#include "parser/ParseExpression.hpp"
#include "parser/ParseGeneratorTableReference.hpp"
#include "parser/ParseGroupBy.hpp"
#include "parser/ParseHaving.hpp"
#include "parser/ParseJoinedTableReference.hpp"
#include "parser/ParseKeyValue.hpp"
#include "parser/ParseLimit.hpp"
#include "parser/ParseLiteralValue.hpp"
#include "parser/ParseOrderBy.hpp"
#include "parser/ParsePartitionClause.hpp"
#include "parser/ParsePredicate.hpp"
#include "parser/ParsePredicateExists.hpp"
#include "parser/ParsePredicateInTableQuery.hpp"
#include "parser/ParsePriority.hpp"
#include "parser/ParserUtil.hpp"
#include "parser/ParseSample.hpp"
#include "parser/ParseSelect.hpp"
#include "parser/ParseSelectionClause.hpp"
#include "parser/ParseSetOperation.hpp"
#include "parser/ParseSimpleTableReference.hpp"
#include "parser/ParseStatement.hpp"
#include "parser/ParseString.hpp"
#include "parser/ParseSubqueryExpression.hpp"
#include "parser/ParseSubqueryTableReference.hpp"
#include "parser/ParseTableReference.hpp"
#include "parser/ParseWindow.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/PtrList.hpp"
#include "utility/PtrVector.hpp"

// Needed for Bison 2.6 and higher, which do not automatically provide this typedef.
typedef void* yyscan_t;

#line 151 "SqlParser_gen.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "SqlParser_gen.hpp".  */
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
#line 115 "../SqlParser.ypp" /* yacc.c:355  */

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

#line 430 "SqlParser_gen.cpp" /* yacc.c:355  */
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

/* Copy the second part of user declarations.  */
#line 216 "../SqlParser.ypp" /* yacc.c:358  */

/* This header needs YYSTYPE, which is defined by the %union directive above */
#include "SqlLexer_gen.hpp"
void NotSupported(const YYLTYPE *location, yyscan_t yyscanner, const std::string &feature);

#line 465 "SqlParser_gen.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  50
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1389

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  149
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  111
/* YYNRULES -- Number of rules.  */
#define YYNRULES  301
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  559

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   392

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     144,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   148,     2,     2,
     145,   146,    23,    21,   147,    22,    27,    24,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   143,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    25,    26,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   634,   634,   638,   642,   646,   650,   653,   660,   663,
     666,   669,   672,   675,   678,   681,   684,   687,   693,   699,
     706,   712,   719,   728,   733,   742,   747,   752,   756,   762,
     767,   770,   773,   778,   781,   784,   787,   790,   793,   796,
     799,   802,   805,   817,   820,   823,   841,   861,   864,   867,
     872,   877,   883,   889,   898,   902,   908,   911,   916,   921,
     926,   933,   940,   944,   950,   953,   958,   961,   966,   969,
     974,   977,   996,   999,  1004,  1008,  1014,  1017,  1020,  1023,
    1028,  1031,  1034,  1041,  1046,  1057,  1062,  1067,  1071,  1075,
    1081,  1084,  1090,  1098,  1101,  1104,  1110,  1115,  1120,  1124,
    1130,  1134,  1137,  1142,  1145,  1150,  1155,  1160,  1164,  1170,
    1179,  1182,  1187,  1190,  1209,  1214,  1218,  1224,  1230,  1239,
    1244,  1252,  1258,  1264,  1267,  1270,  1275,  1278,  1283,  1287,
    1293,  1296,  1299,  1304,  1309,  1314,  1317,  1320,  1325,  1328,
    1331,  1334,  1337,  1340,  1343,  1346,  1351,  1354,  1359,  1363,
    1367,  1370,  1374,  1377,  1382,  1385,  1390,  1393,  1398,  1402,
    1408,  1411,  1416,  1419,  1424,  1427,  1432,  1435,  1454,  1457,
    1462,  1466,  1472,  1478,  1483,  1486,  1491,  1494,  1499,  1502,
    1507,  1510,  1515,  1516,  1519,  1524,  1525,  1528,  1533,  1537,
    1543,  1550,  1553,  1556,  1561,  1564,  1567,  1573,  1576,  1581,
    1586,  1595,  1600,  1609,  1614,  1617,  1622,  1625,  1630,  1636,
    1642,  1645,  1648,  1651,  1654,  1657,  1663,  1672,  1678,  1683,
    1689,  1694,  1699,  1704,  1707,  1710,  1713,  1717,  1721,  1724,
    1727,  1730,  1733,  1736,  1741,  1745,  1749,  1752,  1757,  1771,
    1782,  1793,  1801,  1812,  1815,  1820,  1824,  1830,  1835,  1839,
    1845,  1850,  1853,  1858,  1862,  1868,  1871,  1874,  1877,  1889,
    1893,  1912,  1925,  1940,  1943,  1946,  1949,  1952,  1955,  1960,
    1964,  1970,  1973,  1978,  1982,  1989,  1992,  1995,  1998,  2001,
    2004,  2007,  2010,  2013,  2016,  2021,  2032,  2035,  2040,  2043,
    2046,  2052,  2056,  2062,  2065,  2073,  2076,  2079,  2082,  2088,
    2093,  2098
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOKEN_COMMAND", "TOKEN_NAME",
  "TOKEN_STRING_SINGLE_QUOTED", "TOKEN_STRING_DOUBLE_QUOTED",
  "TOKEN_UNSIGNED_NUMVAL", "TOKEN_OR", "TOKEN_AND", "TOKEN_NOT",
  "TOKEN_EQ", "TOKEN_LT", "TOKEN_LEQ", "TOKEN_GT", "TOKEN_GEQ",
  "TOKEN_NEQ", "TOKEN_LIKE", "TOKEN_REGEXP", "TOKEN_BETWEEN", "TOKEN_IS",
  "'+'", "'-'", "'*'", "'/'", "UNARY_PLUS", "UNARY_MINUS", "'.'",
  "TOKEN_ALL", "TOKEN_UNION", "TOKEN_INTERSECT", "TOKEN_ADD",
  "TOKEN_ALTER", "TOKEN_AS", "TOKEN_ASC", "TOKEN_BIGINT", "TOKEN_BIT",
  "TOKEN_BITWEAVING", "TOKEN_BLOCKPROPERTIES", "TOKEN_BLOCKSAMPLE",
  "TOKEN_BLOOM_FILTER", "TOKEN_CSB_TREE", "TOKEN_BY", "TOKEN_CASE",
  "TOKEN_CAST", "TOKEN_CHARACTER", "TOKEN_CHECK", "TOKEN_COLUMN",
  "TOKEN_CONSTRAINT", "TOKEN_COPY", "TOKEN_CREATE", "TOKEN_CURRENT",
  "TOKEN_DATE", "TOKEN_DATETIME", "TOKEN_DAY", "TOKEN_DECIMAL",
  "TOKEN_DEFAULT", "TOKEN_DELETE", "TOKEN_DESC", "TOKEN_DISTINCT",
  "TOKEN_DOUBLE", "TOKEN_DROP", "TOKEN_ELSE", "TOKEN_END", "TOKEN_EXISTS",
  "TOKEN_EXTRACT", "TOKEN_FALSE", "TOKEN_FIRST", "TOKEN_FLOAT",
  "TOKEN_FOLLOWING", "TOKEN_FOR", "TOKEN_FOREIGN", "TOKEN_FROM",
  "TOKEN_FULL", "TOKEN_GROUP", "TOKEN_HASH", "TOKEN_HAVING", "TOKEN_HOUR",
  "TOKEN_IN", "TOKEN_INDEX", "TOKEN_INNER", "TOKEN_INSERT",
  "TOKEN_INTEGER", "TOKEN_INTERVAL", "TOKEN_INTO", "TOKEN_JOIN",
  "TOKEN_KEY", "TOKEN_LAST", "TOKEN_LEFT", "TOKEN_LIMIT", "TOKEN_LONG",
  "TOKEN_MINUTE", "TOKEN_MONTH", "TOKEN_NULL", "TOKEN_NULLS", "TOKEN_OFF",
  "TOKEN_ON", "TOKEN_ORDER", "TOKEN_OUTER", "TOKEN_OVER",
  "TOKEN_PARTITION", "TOKEN_PARTITIONS", "TOKEN_PERCENT",
  "TOKEN_PRECEDING", "TOKEN_PRIMARY", "TOKEN_PRIORITY", "TOKEN_QUIT",
  "TOKEN_RANGE", "TOKEN_REAL", "TOKEN_REFERENCES", "TOKEN_RIGHT",
  "TOKEN_ROW", "TOKEN_ROW_DELIMITER", "TOKEN_ROWS", "TOKEN_SECOND",
  "TOKEN_SELECT", "TOKEN_SET", "TOKEN_SMA", "TOKEN_SMALLINT",
  "TOKEN_STDERR", "TOKEN_STDOUT", "TOKEN_SUBSTRING", "TOKEN_TABLE",
  "TOKEN_THEN", "TOKEN_TIME", "TOKEN_TIMESTAMP", "TOKEN_TO", "TOKEN_TRUE",
  "TOKEN_TUPLESAMPLE", "TOKEN_UNBOUNDED", "TOKEN_UNIQUE", "TOKEN_UPDATE",
  "TOKEN_USING", "TOKEN_VALUES", "TOKEN_VARCHAR", "TOKEN_WHEN",
  "TOKEN_WHERE", "TOKEN_WINDOW", "TOKEN_WITH", "TOKEN_YEAR",
  "TOKEN_YEARMONTH", "TOKEN_EOF", "TOKEN_LEX_ERROR", "';'", "'\\n'", "'('",
  "')'", "','", "'%'", "$accept", "start", "sql_statement",
  "quit_statement", "alter_table_statement", "create_table_statement",
  "create_index_statement", "drop_table_statement", "column_def",
  "column_def_commalist", "data_type", "column_constraint_def",
  "column_constraint_def_list", "opt_column_constraint_def_list",
  "table_constraint_def", "table_constraint_def_commalist",
  "opt_table_constraint_def_commalist", "opt_column_list",
  "opt_block_properties", "opt_partition_clause", "partition_type",
  "key_value_list", "key_value", "key_string_value", "key_string_list",
  "key_integer_value", "key_bool_value", "index_type",
  "opt_index_properties", "insert_statement", "copy_statement",
  "copy_to_target", "opt_copy_params", "update_statement",
  "delete_statement", "assignment_list", "assignment_item",
  "set_operation_statement", "opt_priority_clause", "with_clause",
  "with_list", "with_list_element", "set_operation_union",
  "set_operation_intersect", "select_query", "opt_all_distinct",
  "selection", "selection_item_commalist", "selection_item", "from_clause",
  "subquery_expression", "opt_sample_clause", "join_type",
  "joined_table_reference", "table_reference", "table_reference_signature",
  "table_reference_signature_primary", "joined_table_reference_commalist",
  "opt_group_by_clause", "opt_having_clause", "opt_order_by_clause",
  "opt_limit_clause", "opt_window_clause", "window_declaration_list",
  "window_declaration", "window_definition", "opt_window_partition",
  "opt_window_order", "opt_window_frame", "frame_mode", "frame_preceding",
  "frame_following", "order_commalist", "order_item",
  "opt_order_direction", "opt_nulls_first", "opt_where_clause",
  "where_clause", "or_expression", "and_expression", "not_expression",
  "predicate_expression_base", "add_expression", "multiply_expression",
  "unary_expression", "expression_base", "function_call", "cast_function",
  "extract_function", "substr_function", "case_expression",
  "simple_when_clause_list", "simple_when_clause",
  "searched_when_clause_list", "searched_when_clause", "opt_else_clause",
  "expression_list", "literal_value", "datetime_unit",
  "literal_value_commalist", "attribute_ref", "attribute_ref_list",
  "comparison_operation", "unary_operation", "add_operation",
  "multiply_operation", "name_commalist", "any_name", "boolean_value",
  "command", "command_argument_list", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,    43,    45,    42,    47,   276,   277,    46,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,    59,    10,    40,    41,    44,    37
};
# endif

#define YYPACT_NINF -286

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-286)))

#define YYTABLE_NINF -139

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     254,  -286,  -286,   -65,    71,   -18,   133,   -19,    28,  -286,
      40,   190,   190,  -286,   209,    79,  -286,  -286,  -286,  -286,
    -286,  -286,  -286,  -286,  -286,  -286,   168,    45,   183,  -286,
     150,   238,   190,  -286,  -286,    13,    77,   190,   190,   190,
     190,   190,  -286,  -286,   704,   118,   105,  -286,   225,   120,
    -286,  -286,  -286,   148,   186,    45,    40,   179,  -286,   148,
    -286,  -286,  -286,   164,    18,   159,   285,   159,   204,   162,
     174,  -286,   -37,  -286,  -286,   307,   312,  -286,  -286,  -286,
     771,   178,   194,  -286,   258,  -286,  -286,   198,  -286,  -286,
     344,  -286,  -286,  -286,  -286,   207,  -286,  -286,   211,   282,
     865,   353,   300,   227,  -286,  -286,   349,    24,  -286,  -286,
     276,  -286,  -286,  -286,  -286,  -286,  -286,  1026,   -13,   190,
     190,   236,   190,    13,   190,  -286,   148,   347,  -286,   248,
     270,  -286,  -286,  -286,   239,  -286,   159,  -286,   190,   190,
     610,  -286,  -286,   246,   190,  -286,  -286,  -286,   610,    50,
     -16,  -286,  1093,   379,  -286,   146,   146,  1093,   387,  -286,
       0,    29,  -286,    20,   174,  1093,  -286,  -286,   190,  1093,
    -286,  -286,  -286,  -286,  1093,    21,   312,  -286,   190,   406,
     -81,  -286,   389,  -286,   148,  -286,   175,  -286,   159,   148,
     183,  -286,   190,    -7,   190,   190,   190,  -286,   274,  -286,
     215,  1249,   932,   236,   543,   408,   416,  -286,  -286,   315,
     420,  1237,   217,    10,  1093,     1,  -286,  1093,  -286,   380,
     226,   298,  -286,  -286,  -286,  -286,  -286,  -286,   374,  -286,
      85,   301,  -286,  -286,    11,   220,   228,  -286,   306,   220,
      56,   381,  -286,  -286,    24,  -286,   348,  -286,  -286,   308,
    1093,  -286,   346,   231,   190,  -286,  1093,  -286,   190,  -286,
    -286,  -286,   311,   371,   376,   318,  -286,  -286,  -286,   233,
    -286,  -286,  -286,  -286,  -286,    15,   190,   328,    -7,   190,
    -286,   152,  -286,  -286,     2,    72,   610,   610,   210,  -286,
    -286,  -286,  -286,  -286,  -286,  -286,  -286,  1093,   319,  1093,
      60,  -286,   241,   334,  1093,    58,  -286,   407,   346,  -286,
    1153,  -286,  1093,   462,  -286,   163,   190,  -286,  -286,   375,
    -286,   378,   382,   393,    20,  -286,   465,   472,   220,   439,
     409,   440,   337,   390,  -286,   243,  -286,  1093,  -286,   346,
    -286,   610,   339,   341,   190,  -286,   190,  -286,  -286,  -286,
    -286,  -286,  -286,  -286,   190,  -286,  -286,  -286,   250,   463,
     170,  -286,   345,   356,  -286,   398,   350,  1237,  -286,   411,
     190,  -286,  -286,   152,  -286,  -286,   416,  -286,  -286,  -286,
    1093,   355,    92,   865,  -286,   346,   405,  -286,  -286,  1237,
     357,   346,  1093,  -286,   359,   360,    31,    51,  -286,  -286,
    -286,  -286,  -286,    20,   228,   399,   401,  -286,  1093,   610,
     410,  1093,  -286,   466,   126,  -286,   346,    22,   190,   190,
     255,  -286,   257,  -286,   190,  -286,  -286,  -286,  -286,   364,
      -7,   473,   412,  -286,   610,  -286,  -286,   365,  -286,   259,
     865,  -286,  1093,   262,  -286,  -286,  1237,   346,  -286,  -286,
    -286,   506,  -286,   419,  -286,  -286,   370,   408,   476,   430,
     370,  1093,  -286,  -286,  -286,   501,  -286,   271,   275,  -286,
    -286,  -286,   190,  -286,  -286,   377,   479,  -286,    30,   190,
    1093,   277,   346,  -286,   284,   383,   610,  1093,   516,   388,
     385,  -286,   325,    25,   417,  -286,   286,   190,    17,  -286,
     391,   346,  -286,  -286,  -286,   408,   385,  -286,   190,  -286,
     388,  -286,  1093,  -286,  -286,   434,   432,   422,   433,   529,
     190,  -286,   288,  -286,  -286,   394,  -286,   508,  -286,  -286,
      38,  -286,  -286,  -286,  -286,    49,   400,  -286,   190,   413,
    -286,  -286,   474,   443,   475,  -286,   190,   290,   348,  -286,
    -286,  -286,   292,   454,   414,  -286,   549,  -286,  -286
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     6,   301,     0,     0,     0,     0,     0,     0,    18,
     123,     0,     0,     7,     0,     0,    15,     8,    10,    11,
      13,    14,     9,    17,    12,    16,     0,   112,   119,   121,
       0,   299,     0,   293,   294,     0,     0,     0,     0,     0,
       0,     0,   124,   125,     0,     0,   114,   115,     0,   156,
       1,     3,     2,     0,     0,   112,   123,     0,   110,     0,
       5,     4,   300,     0,     0,   103,     0,   103,     0,     0,
     197,    25,     0,   259,   256,     0,   285,   126,    40,    29,
       0,     0,     0,    30,    31,    34,    36,     0,    37,    39,
       0,    41,   255,    35,    38,     0,    32,    33,     0,     0,
       0,     0,     0,   127,   128,   233,   132,   218,   220,   222,
     225,   228,   229,   230,   231,   224,   223,     0,   271,     0,
       0,     0,     0,     0,     0,   111,     0,     0,   120,     0,
       0,   100,   102,   101,     0,    98,   103,    97,     0,     0,
       0,   106,   198,     0,     0,    94,   257,   258,     0,     0,
     251,   248,     0,     0,    43,     0,   260,     0,     0,    44,
       0,     0,   262,     0,   197,     0,   286,   287,     0,     0,
     131,   289,   290,   288,     0,     0,     0,   221,     0,     0,
     197,   108,     0,   116,     0,   117,     0,   291,   103,     0,
     118,   113,     0,     0,     0,     0,     0,    96,    66,    27,
       0,     0,     0,     0,     0,   199,   201,   203,   205,     0,
     223,     0,     0,     0,     0,   251,   245,     0,   249,     0,
       0,     0,   265,   266,   267,   264,   268,   263,     0,   261,
       0,     0,   134,   232,     0,     0,   158,   147,   133,   152,
     135,   160,   129,   130,   217,   219,   174,   226,   272,     0,
       0,   234,   253,     0,     0,   105,     0,   157,     0,    99,
      95,    19,     0,     0,     0,     0,    20,    21,    22,     0,
      74,    76,    77,    78,    79,     0,     0,     0,    64,     0,
      42,    56,   204,   212,     0,     0,     0,     0,     0,   275,
     277,   278,   279,   280,   276,   281,   283,     0,     0,     0,
       0,   269,     0,     0,     0,     0,   246,     0,   252,   244,
       0,    45,     0,     0,    46,   138,     0,   148,   154,   144,
     139,   140,   142,     0,     0,   151,     0,     0,   150,     0,
     162,     0,     0,   176,   235,     0,   236,     0,   107,   109,
     292,     0,     0,     0,     0,   104,     0,    81,    84,    82,
     297,   298,   296,   295,     0,    80,    85,   273,     0,   271,
       0,    63,    65,    68,    28,     0,     0,     0,    47,     0,
       0,    49,    55,    57,    26,   211,   200,   202,   282,   284,
       0,     0,     0,     0,   213,   210,     0,   209,    93,     0,
       0,   250,     0,   243,     0,     0,     0,     0,   153,   155,
     145,   141,   143,     0,   159,     0,     0,   149,     0,     0,
     164,     0,   227,     0,   178,   237,   254,     0,     0,     0,
       0,    75,     0,    67,     0,    86,    87,    88,    89,    90,
       0,     0,    70,    48,     0,    51,    50,     0,    54,     0,
       0,   215,     0,     0,   208,   270,     0,   247,   238,   239,
     240,     0,   241,     0,   136,   137,   161,   163,     0,   166,
     175,     0,   181,   180,   173,     0,    61,     0,     0,    58,
      83,   274,     0,    24,    62,     0,     0,    23,     0,     0,
       0,     0,   206,   214,     0,     0,     0,     0,     0,   168,
     177,   188,   191,     0,     0,    59,     0,     0,     0,    52,
       0,   207,   216,    92,   242,   146,   165,   167,     0,   122,
     169,   170,     0,   192,   193,   194,     0,     0,     0,     0,
       0,    91,     0,    72,    73,     0,    53,     0,   171,   189,
       0,   190,   182,   184,   183,     0,     0,    69,     0,     0,
     195,   196,     0,     0,     0,   179,     0,     0,   174,   185,
     187,   186,     0,     0,     0,    60,     0,   172,    71
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,  -132,  -286,
    -152,   184,  -286,  -286,  -275,  -286,  -286,  -286,  -286,  -286,
    -286,  -285,   213,  -286,  -286,  -286,  -286,  -286,  -286,  -286,
    -286,     4,    -8,  -286,  -286,  -286,   309,  -286,   507,  -286,
    -286,   441,   240,   442,   -28,   510,  -286,  -286,   402,  -286,
    -105,  -286,  -286,  -193,   166,  -185,   -11,  -286,  -286,  -286,
    -286,  -286,  -286,  -286,    61,    26,  -286,  -286,  -286,  -286,
    -286,  -286,    83,    63,  -286,  -286,    52,  -286,  -142,   287,
     289,   392,   -35,   403,   415,   460,  -161,  -286,  -286,  -286,
    -286,  -286,   366,  -286,   435,   367,  -238,  -200,   424,   137,
    -135,  -286,  -286,  -286,  -286,  -286,  -140,    -4,  -286,  -286,
    -286
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    14,    15,    16,    17,    18,    19,    20,   199,   200,
     101,   372,   373,   374,   266,   362,   363,   277,   432,   477,
     525,   269,   270,   271,   272,   273,   274,   429,   473,    21,
      22,    65,   135,    23,    24,   180,   181,    25,    58,    26,
      46,    47,   160,    28,    29,    44,   102,   103,   104,   164,
     105,   328,   323,   236,   237,   317,   318,   238,   330,   410,
     459,   489,   509,   510,   511,   332,   333,   414,   464,   465,
     519,   545,   490,   491,   515,   531,   141,   142,   205,   206,
     207,   208,   209,   107,   108,   109,   110,   111,   112,   113,
     114,   215,   216,   150,   151,   219,   253,   115,   228,   302,
     116,   358,   299,   117,   169,   174,   186,   118,   356,    30,
      31
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      36,    48,   239,   361,   212,   210,   213,    45,    49,   106,
     286,   301,   335,   210,   178,    33,   185,    34,   286,    33,
     347,    34,   348,   131,    33,    33,    34,    34,    63,    56,
     286,   128,   516,    68,    69,    70,    71,    72,   286,   262,
      67,   315,    56,   349,   145,   149,   217,   171,   172,   281,
     166,   167,   166,   167,   325,   140,   542,    32,   235,   137,
     261,    37,   284,   217,   263,   161,   254,   210,    42,   210,
     386,   166,   167,   239,    56,    33,   517,    34,    10,   166,
     167,   350,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   523,   166,   167,   326,   143,   264,   283,    43,
     543,   442,   170,    40,    38,   540,   166,   167,   144,    48,
     351,   352,    41,   166,   167,   182,    49,   220,   187,   148,
     189,   451,   230,   265,   524,   541,    10,   188,   197,   235,
     106,   404,   179,   304,   198,   201,   214,   132,   133,    64,
     187,   357,   353,   407,   252,   443,   232,   364,   375,    66,
     298,   210,   210,   387,   518,   474,   234,   313,   394,   240,
     354,   260,   365,   239,   243,   234,   246,   435,   466,   285,
     456,   247,   173,   460,   248,   233,   499,   450,   544,   305,
     259,   392,   308,    57,   327,   214,    10,   496,   201,   445,
     267,   268,   275,   384,    33,   129,    34,   452,   366,   417,
     222,   179,   481,    64,   420,    39,   210,   425,   367,    50,
     426,   427,   522,    59,   422,   252,   241,    53,   233,   235,
      51,   339,    52,   223,    33,   130,    34,   378,   379,   380,
     240,    49,   255,   462,   119,    49,   319,   224,   225,   463,
      27,    62,   239,   320,    35,   368,   301,   166,   167,    54,
     182,   321,   120,   316,   340,     1,   369,     2,   121,   310,
     226,   370,   382,    10,   385,   122,    55,   457,   480,   391,
     124,   355,   359,   322,   210,   201,   441,   396,   467,   468,
     166,   167,   371,    10,   127,   227,     3,   428,   381,   471,
     136,    60,   478,   123,    61,   192,   193,   134,   235,   210,
     138,   319,   416,     4,     5,   399,   395,   139,   320,   398,
     140,     6,    49,  -138,   146,     7,   321,   194,   195,   147,
     240,   257,   258,   152,    49,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,     8,   166,   167,   322,   153,
     187,   154,   275,   155,   505,   439,   166,   167,   252,   156,
     187,   210,   157,    33,   191,    34,   158,   447,   162,   513,
       9,   278,   279,   303,   258,   159,   437,   166,   167,    10,
     166,   167,   163,   252,   165,   175,   252,   336,   337,   345,
     346,   184,   168,   514,   196,    11,   221,   388,   389,   415,
     337,   211,    12,   298,   231,    13,   423,   424,   547,   240,
     256,   469,   258,   470,   258,   252,   552,   482,   483,   337,
      33,    73,    34,    74,   187,   187,   286,   494,   258,   276,
     359,   495,   258,   502,   337,   287,   492,    75,    76,   249,
     503,   389,   521,   346,   537,   346,   553,   258,   555,   258,
     300,    78,    79,   309,   311,   501,   312,   314,   331,    80,
      81,    82,   492,   324,   334,   329,   341,   342,    83,    84,
     360,    85,   343,   344,   383,   250,    86,   390,   275,   397,
     393,    87,   405,   400,    88,   500,   401,   492,   403,   406,
     402,   408,   411,   412,   418,   409,   419,   413,    89,    90,
     178,   433,   430,   275,   431,   434,    91,   436,   444,    92,
     440,   454,   446,   455,   527,   448,   449,   458,   461,   472,
     479,   475,   476,   485,    93,   486,   536,   337,   487,   488,
     493,   498,   497,   507,    94,   508,   520,    95,   530,   504,
      96,    97,   512,   533,   187,   532,   534,   526,   535,   538,
      98,   539,   187,   549,   551,   546,    99,    33,    73,    34,
      74,   100,   251,   202,   550,   556,   558,   438,   548,   421,
     557,   183,   125,   338,    75,    76,   126,   242,   190,   453,
     506,   528,   244,   376,   554,   529,   377,   177,    78,    79,
     229,   306,   307,   484,     0,   218,    80,    81,    82,   245,
       0,     0,     0,     0,   282,    83,    84,     0,    85,     0,
       0,     0,     0,    86,     0,     0,     0,   203,    87,     0,
       0,    88,     0,     0,    33,    73,    34,    74,     0,     0,
     202,     0,     0,     0,     0,    89,    90,     0,     0,     0,
       0,    75,    76,    91,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    78,    79,     0,     0,     0,
       0,    93,     0,    80,    81,    82,     0,     0,    10,     0,
       0,    94,    83,    84,    95,    85,     0,    96,    97,     0,
      86,     0,     0,     0,   203,    87,     0,    98,    88,     0,
       0,     0,     0,    99,     0,     0,     0,     0,   204,     0,
       0,     0,    89,    90,     0,     0,     0,     0,     0,     0,
      91,     0,     0,    92,     0,     0,     0,     0,    33,    73,
      34,    74,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    94,     0,
       0,    95,     0,     0,    96,    97,     0,     0,     0,    78,
      79,     0,     0,     0,    98,     0,     0,    80,    81,    82,
      99,     0,     0,     0,     0,   204,    83,    84,     0,    85,
       0,     0,     0,     0,    86,     0,     0,     0,     0,    87,
       0,     0,    88,     0,     0,    33,    73,    34,    74,     0,
       0,     0,     0,     0,     0,     0,    89,    90,     0,     0,
       0,     0,    75,    76,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,    78,    79,     0,     0,
       0,     0,    93,     0,    80,    81,    82,     0,     0,     0,
       0,     0,    94,    83,    84,    95,    85,     0,    96,    97,
       0,    86,     0,     0,     0,     0,    87,     0,    98,    88,
       0,     0,     0,     0,    99,     0,     0,     0,     0,   100,
       0,     0,     0,    89,    90,     0,     0,     0,     0,     0,
       0,    91,     0,     0,    92,     0,     0,     0,     0,    33,
      73,    34,    74,     0,     0,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,    75,    76,     0,    94,
       0,     0,    95,     0,     0,    96,    97,     0,     0,     0,
      78,    79,     0,     0,     0,    98,   148,     0,    80,    81,
      82,    99,     0,     0,     0,     0,   100,    83,    84,     0,
      85,     0,     0,     0,     0,    86,     0,     0,     0,     0,
      87,     0,     0,    88,     0,     0,    33,    73,    34,    74,
       0,     0,     0,     0,     0,     0,     0,    89,    90,     0,
       0,     0,     0,    75,    76,    91,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    78,    79,     0,
       0,     0,     0,    93,     0,    80,    81,    82,     0,     0,
      10,     0,     0,    94,    83,    84,    95,    85,     0,    96,
      97,     0,    86,     0,     0,     0,   203,    87,     0,    98,
      88,     0,     0,     0,     0,    99,     0,     0,     0,     0,
     100,     0,     0,     0,    89,    90,     0,     0,     0,     0,
       0,     0,    91,     0,     0,    92,     0,     0,     0,     0,
      33,    73,    34,    74,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,    75,   176,     0,
      94,     0,     0,    95,     0,     0,    96,    97,     0,     0,
       0,    78,    79,     0,     0,     0,    98,     0,     0,    80,
      81,    82,    99,     0,     0,     0,     0,   204,    83,    84,
       0,    85,     0,     0,     0,     0,    86,     0,     0,     0,
       0,    87,     0,     0,    88,     0,     0,    33,    73,    34,
      74,     0,     0,     0,     0,     0,     0,     0,    89,    90,
       0,     0,     0,     0,    75,    76,    91,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    78,    79,
       0,     0,     0,     0,    93,     0,    80,    81,    82,     0,
       0,     0,     0,     0,    94,    83,    84,    95,    85,     0,
      96,    97,     0,    86,     0,     0,     0,    33,    87,    34,
      98,    88,     0,     0,     0,     0,    99,     0,     0,     0,
       0,   100,     0,     0,     0,    89,    90,     0,     0,     0,
       0,     0,     0,    91,     0,     0,    92,     0,    78,    79,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,    93,     0,     0,     0,    83,    84,     0,    85,     0,
       0,    94,     0,    86,    95,     0,     0,    96,    97,     0,
       0,    88,     0,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,    99,     0,    89,   280,     0,   100,     0,
       0,     0,    73,    91,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,   176,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    78,    79,     0,     0,     0,    96,    97,     0,
       0,     0,    82,     0,    78,    79,     0,    98,     0,    83,
      84,     0,    85,    99,    82,     0,     0,    86,     0,     0,
       0,    83,    84,     0,    85,    88,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,    89,
      90,     0,     0,     0,     0,     0,     0,    91,     0,     0,
      92,    89,   280,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,    93,     0,     0,
       0,    96,    97,     0,     0,     0,     0,    94,     0,     0,
       0,    98,     0,    96,    97,     0,     0,    99,     0,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,    99
};

static const yytype_int16 yycheck[] =
{
       4,    12,   163,   278,   144,   140,   148,    11,    12,    44,
       8,   211,   250,   148,    27,     4,   121,     6,     8,     4,
       5,     6,     7,     5,     4,     4,     6,     6,    32,    29,
       8,    59,     7,    37,    38,    39,    40,    41,     8,    46,
      36,   234,    29,    28,    72,    80,    62,    23,    24,   201,
      21,    22,    21,    22,   239,   136,     7,   122,   163,    67,
     192,    79,   204,    62,    71,   100,   147,   202,    28,   204,
      10,    21,    22,   234,    29,     4,    51,     6,   115,    21,
      22,    66,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    75,    21,    22,    39,   133,   104,   203,    59,
      51,     9,   106,   122,   122,    67,    21,    22,   145,   120,
      95,    96,    84,    21,    22,   119,   120,   152,   122,   135,
     124,    70,   157,   130,   107,    87,   115,   123,   136,   234,
     165,   324,   145,   123,   138,   139,   135,   119,   120,   126,
     144,   276,   127,   328,   179,   383,   146,   279,   146,    72,
      78,   286,   287,    93,   129,   430,   145,    72,   310,   163,
     145,   189,    10,   324,   168,   145,   145,   367,   146,   204,
     408,   175,   148,   411,   178,   146,   146,   146,   129,   214,
     188,   123,   217,   138,   128,   135,   115,   472,   192,   389,
     194,   195,   196,   298,     4,    31,     6,   146,    46,   341,
      54,   145,   440,   126,   344,    72,   341,    37,    56,     0,
      40,    41,   497,    30,   354,   250,   164,    49,   146,   324,
     141,   256,   143,    77,     4,    61,     6,    17,    18,    19,
     234,   235,   180,   107,   116,   239,    73,    91,    92,   113,
       0,     3,   403,    80,     4,    93,   446,    21,    22,    81,
     254,    88,   147,    33,   258,     1,   104,     3,    33,    33,
     114,   109,   297,   115,   299,   145,    26,   409,     9,   304,
      84,   275,   276,   110,   409,   279,   381,   312,   418,   419,
      21,    22,   130,   115,   105,   139,    32,   117,    78,   424,
       5,   141,   434,    53,   144,    47,    48,   138,   403,   434,
      96,    73,   337,    49,    50,   316,   310,   145,    80,   146,
     136,    57,   316,    85,     7,    61,    88,    47,    48,     7,
     324,   146,   147,   145,   328,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    81,    21,    22,   110,   145,
     344,    83,   346,   145,   486,   380,    21,    22,   383,     5,
     354,   486,   145,     4,     7,     6,   145,   392,     5,    34,
     106,   146,   147,   146,   147,    83,   370,    21,    22,   115,
      21,    22,    72,   408,   147,    99,   411,   146,   147,   146,
     147,   145,    33,    58,   145,   131,     7,   146,   147,   146,
     147,   145,   138,    78,     7,   141,   146,   147,   538,   403,
      11,   146,   147,   146,   147,   440,   546,   442,   146,   147,
       4,     5,     6,     7,   418,   419,     8,   146,   147,   145,
     424,   146,   147,   146,   147,     9,   461,    21,    22,    23,
     146,   147,   146,   147,   146,   147,   146,   147,   146,   147,
      20,    35,    36,    63,   146,   480,    72,   146,   100,    43,
      44,    45,   487,   147,   146,    74,   145,    86,    52,    53,
     132,    55,    86,   145,   145,    59,    60,   133,   472,     7,
      63,    65,     7,    98,    68,   479,    98,   512,    85,     7,
      98,    42,    42,   146,   145,    76,   145,    97,    82,    83,
      27,    93,   147,   497,   138,   145,    90,    86,    93,    93,
     145,   102,   145,   102,   508,   146,   146,    97,    42,   145,
     145,    38,   100,     7,   108,    96,   520,   147,    42,    89,
      19,    42,   145,     7,   118,   137,   109,   121,    94,   146,
     124,   125,   147,   111,   538,   103,   103,   146,     9,   145,
     134,    33,   546,    69,    69,   145,   140,     4,     5,     6,
       7,   145,   146,    10,   111,   101,     7,   373,   145,   346,
     146,   120,    55,   254,    21,    22,    56,   165,   126,   403,
     487,   510,   169,   286,   548,   512,   287,   117,    35,    36,
     156,   215,   215,   446,    -1,   150,    43,    44,    45,   174,
      -1,    -1,    -1,    -1,   202,    52,    53,    -1,    55,    -1,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    64,    65,    -1,
      -1,    68,    -1,    -1,     4,     5,     6,     7,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    82,    83,    -1,    -1,    -1,
      -1,    21,    22,    90,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    36,    -1,    -1,    -1,
      -1,   108,    -1,    43,    44,    45,    -1,    -1,   115,    -1,
      -1,   118,    52,    53,   121,    55,    -1,   124,   125,    -1,
      60,    -1,    -1,    -1,    64,    65,    -1,   134,    68,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    93,    -1,    -1,    -1,    -1,     4,     5,
       6,     7,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    23,   118,    -1,
      -1,   121,    -1,    -1,   124,   125,    -1,    -1,    -1,    35,
      36,    -1,    -1,    -1,   134,    -1,    -1,    43,    44,    45,
     140,    -1,    -1,    -1,    -1,   145,    52,    53,    -1,    55,
      -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    -1,    -1,     4,     5,     6,     7,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    -1,    -1,
      -1,    -1,    21,    22,    90,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,    -1,
      -1,    -1,   108,    -1,    43,    44,    45,    -1,    -1,    -1,
      -1,    -1,   118,    52,    53,   121,    55,    -1,   124,   125,
      -1,    60,    -1,    -1,    -1,    -1,    65,    -1,   134,    68,
      -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    82,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,     4,
       5,     6,     7,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,   118,
      -1,    -1,   121,    -1,    -1,   124,   125,    -1,    -1,    -1,
      35,    36,    -1,    -1,    -1,   134,   135,    -1,    43,    44,
      45,   140,    -1,    -1,    -1,    -1,   145,    52,    53,    -1,
      55,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    -1,    -1,     4,     5,     6,     7,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    -1,
      -1,    -1,    -1,    21,    22,    90,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,
      -1,    -1,    -1,   108,    -1,    43,    44,    45,    -1,    -1,
     115,    -1,    -1,   118,    52,    53,   121,    55,    -1,   124,
     125,    -1,    60,    -1,    -1,    -1,    64,    65,    -1,   134,
      68,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    82,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,
       4,     5,     6,     7,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
     118,    -1,    -1,   121,    -1,    -1,   124,   125,    -1,    -1,
      -1,    35,    36,    -1,    -1,    -1,   134,    -1,    -1,    43,
      44,    45,   140,    -1,    -1,    -1,    -1,   145,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    -1,    -1,     4,     5,     6,
       7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,
      -1,    -1,    -1,    -1,    21,    22,    90,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,
      -1,    -1,    -1,    -1,   108,    -1,    43,    44,    45,    -1,
      -1,    -1,    -1,    -1,   118,    52,    53,   121,    55,    -1,
     124,   125,    -1,    60,    -1,    -1,    -1,     4,    65,     6,
     134,    68,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    93,    -1,    35,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,   108,    -1,    -1,    -1,    52,    53,    -1,    55,    -1,
      -1,   118,    -1,    60,   121,    -1,    -1,   124,   125,    -1,
      -1,    68,    -1,    -1,    -1,    -1,    -1,   134,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    82,    83,    -1,   145,    -1,
      -1,    -1,     5,    90,     7,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    35,    36,    -1,    -1,    -1,   124,   125,    -1,
      -1,    -1,    45,    -1,    35,    36,    -1,   134,    -1,    52,
      53,    -1,    55,   140,    45,    -1,    -1,    60,    -1,    -1,
      -1,    52,    53,    -1,    55,    68,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      93,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,   108,    -1,    -1,
      -1,   124,   125,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,   134,    -1,   124,   125,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,   140
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,     3,    32,    49,    50,    57,    61,    81,   106,
     115,   131,   138,   141,   150,   151,   152,   153,   154,   155,
     156,   178,   179,   182,   183,   186,   188,   191,   192,   193,
     258,   259,   122,     4,     6,   191,   256,    79,   122,    72,
     122,    84,    28,    59,   194,   256,   189,   190,   205,   256,
       0,   141,   143,    49,    81,   191,    29,   138,   187,    30,
     141,   144,     3,   256,   126,   180,    72,   180,   256,   256,
     256,   256,   256,     5,     7,    21,    22,    23,    35,    36,
      43,    44,    45,    52,    53,    55,    60,    65,    68,    82,
      83,    90,    93,   108,   118,   121,   124,   125,   134,   140,
     145,   159,   195,   196,   197,   199,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   246,   249,   252,   256,   116,
     147,    33,   145,   191,    84,   187,   194,   105,   193,    31,
      61,     5,   119,   120,   138,   181,     5,   181,    96,   145,
     136,   225,   226,   133,   145,   193,     7,     7,   135,   231,
     242,   243,   145,   145,    83,   145,     5,   145,   145,    83,
     191,   231,     5,    72,   198,   147,    21,    22,    33,   253,
     256,    23,    24,   148,   254,    99,    22,   234,    27,   145,
     184,   185,   256,   190,   145,   199,   255,   256,   180,   256,
     192,     7,    47,    48,    47,    48,   145,   181,   256,   157,
     158,   256,    10,    64,   145,   227,   228,   229,   230,   231,
     249,   145,   255,   227,   135,   240,   241,    62,   243,   244,
     231,     7,    54,    77,    91,    92,   114,   139,   247,   247,
     231,     7,   146,   146,   145,   199,   202,   203,   206,   235,
     256,   225,   197,   256,   232,   233,   145,   256,   256,    23,
      59,   146,   231,   245,   147,   225,    11,   146,   147,   181,
     193,   157,    46,    71,   104,   130,   163,   256,   256,   170,
     171,   172,   173,   174,   175,   256,   145,   166,   146,   147,
      83,   159,   230,   199,   227,   231,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    78,   251,
      20,   246,   248,   146,   123,   231,   241,   244,   231,    63,
      33,   146,    72,    72,   146,   202,    33,   204,   205,    73,
      80,    88,   110,   201,   147,   204,    39,   128,   200,    74,
     207,   100,   214,   215,   146,   245,   146,   147,   185,   231,
     256,   145,    86,    86,   145,   146,   147,     5,     7,    28,
      66,    95,    96,   127,   145,   256,   257,   249,   250,   256,
     132,   163,   164,   165,   157,    10,    46,    56,    93,   104,
     109,   130,   160,   161,   162,   146,   228,   229,    17,    18,
      19,    78,   231,   145,   199,   231,    10,    93,   146,   147,
     133,   231,   123,    63,   159,   256,   231,     7,   146,   205,
      98,    98,    98,    85,   202,     7,     7,   204,    42,    76,
     208,    42,   146,    97,   216,   146,   231,   227,   145,   145,
     255,   171,   255,   146,   147,    37,    40,    41,   117,   176,
     147,   138,   167,    93,   145,   246,    86,   256,   160,   231,
     145,   199,     9,   245,    93,   246,   145,   231,   146,   146,
     146,    70,   146,   203,   102,   102,   245,   227,    97,   209,
     245,    42,   107,   113,   217,   218,   146,   255,   255,   146,
     146,   249,   145,   177,   163,    38,   100,   168,   227,   145,
       9,   245,   231,   146,   248,     7,    96,    42,    89,   210,
     221,   222,   231,    19,   146,   146,   170,   145,    42,   146,
     256,   231,   146,   146,   146,   227,   221,     7,   137,   211,
     212,   213,   147,    34,    58,   223,     7,    51,   129,   219,
     109,   146,   170,    75,   107,   169,   146,   256,   213,   222,
      94,   224,   103,   111,   103,     9,   256,   146,   145,    33,
      67,    87,     7,    51,   129,   220,   145,   255,   145,    69,
     111,    69,   255,   146,   214,   146,   101,   146,     7
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   149,   150,   150,   150,   150,   150,   150,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   152,   153,
     153,   153,   153,   154,   155,   156,   157,   158,   158,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   160,   160,   160,
     160,   160,   160,   160,   161,   161,   162,   162,   163,   163,
     163,   163,   164,   164,   165,   165,   166,   166,   167,   167,
     168,   168,   169,   169,   170,   170,   171,   171,   171,   171,
     172,   172,   172,   173,   174,   175,   176,   176,   176,   176,
     177,   177,   178,   178,   178,   178,   179,   179,   179,   179,
     180,   180,   180,   181,   181,   182,   183,   184,   184,   185,
     186,   186,   187,   187,   188,   189,   189,   190,   191,   191,
     192,   192,   193,   194,   194,   194,   195,   195,   196,   196,
     197,   197,   197,   198,   199,   200,   200,   200,   201,   201,
     201,   201,   201,   201,   201,   201,   202,   202,   203,   203,
     203,   203,   203,   203,   204,   204,   205,   205,   206,   206,
     207,   207,   208,   208,   209,   209,   210,   210,   211,   211,
     212,   212,   213,   214,   215,   215,   216,   216,   217,   217,
     218,   218,   219,   219,   219,   220,   220,   220,   221,   221,
     222,   223,   223,   223,   224,   224,   224,   225,   225,   226,
     227,   227,   228,   228,   229,   229,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   231,   231,   232,
     232,   233,   233,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   235,   235,   235,   235,   236,   236,
     237,   238,   238,   239,   239,   240,   240,   241,   242,   242,
     243,   244,   244,   245,   245,   246,   246,   246,   246,   246,
     246,   246,   246,   247,   247,   247,   247,   247,   247,   248,
     248,   249,   249,   250,   250,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   252,   253,   253,   254,   254,
     254,   255,   255,   256,   256,   257,   257,   257,   257,   258,
     259,   259
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     6,
       6,     6,     6,     9,     9,     3,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     4,     4,     1,     2,     1,
       2,     2,     4,     5,     2,     1,     0,     1,     4,     5,
      10,     4,     3,     1,     0,     1,     0,     3,     0,     5,
       0,     8,     1,     1,     1,     3,     1,     1,     1,     1,
       2,     2,     2,     4,     2,     2,     1,     1,     1,     1,
       0,     3,    10,     7,     4,     5,     5,     4,     4,     5,
       2,     2,     2,     0,     4,     5,     4,     3,     1,     3,
       2,     3,     0,     3,     2,     1,     3,     3,     4,     1,
       3,     1,    10,     0,     1,     1,     1,     1,     1,     3,
       3,     2,     1,     2,     3,     0,     3,     3,     0,     1,
       1,     2,     1,     2,     1,     2,     6,     1,     2,     3,
       2,     2,     1,     3,     1,     2,     1,     4,     1,     3,
       0,     3,     0,     2,     0,     3,     0,     2,     0,     1,
       1,     2,     6,     3,     0,     3,     0,     3,     0,     5,
       1,     1,     2,     2,     2,     2,     2,     2,     1,     3,
       3,     0,     1,     1,     0,     2,     2,     0,     1,     2,
       3,     1,     3,     1,     2,     1,     5,     6,     4,     3,
       3,     3,     2,     3,     5,     4,     6,     3,     1,     3,
       1,     2,     1,     1,     1,     1,     3,     5,     1,     1,
       1,     1,     3,     1,     3,     4,     4,     5,     6,     6,
       6,     6,     8,     5,     4,     1,     2,     4,     1,     2,
       4,     0,     2,     1,     3,     1,     1,     2,     2,     1,
       2,     3,     2,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     3,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     2,
       2,     0
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (&yylloc, yyscanner, parsedStatement, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, yyscanner, parsedStatement); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t yyscanner, quickstep::ParseStatement **parsedStatement)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (yyscanner);
  YYUSE (parsedStatement);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t yyscanner, quickstep::ParseStatement **parsedStatement)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, yyscanner, parsedStatement);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, yyscan_t yyscanner, quickstep::ParseStatement **parsedStatement)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , yyscanner, parsedStatement);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, yyscanner, parsedStatement); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, yyscan_t yyscanner, quickstep::ParseStatement **parsedStatement)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (yyscanner);
  YYUSE (parsedStatement);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yytype)
    {
          case 3: /* TOKEN_COMMAND  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2006 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_NAME  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2016 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2026 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2036 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 7: /* TOKEN_UNSIGNED_NUMVAL  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2046 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 151: /* sql_statement  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2056 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 152: /* quit_statement  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 2066 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 153: /* alter_table_statement  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2076 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* create_table_statement  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 2086 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* create_index_statement  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2096 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* drop_table_statement  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 2106 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* column_def  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 2116 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* column_def_commalist  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 2126 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* data_type  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 2136 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* column_constraint_def  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 2146 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* column_constraint_def_list  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2156 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* opt_column_constraint_def_list  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2166 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 166: /* opt_column_list  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2176 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 167: /* opt_block_properties  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).block_properties_) != nullptr) {
    delete ((*yyvaluep).block_properties_);
  }
}
#line 2186 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* opt_partition_clause  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).partition_clause_) != nullptr) {
    delete ((*yyvaluep).partition_clause_);
  }
}
#line 2196 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* partition_type  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2206 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* key_value_list  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2216 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* key_value  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 2226 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* key_string_value  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 2236 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* key_string_list  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 2246 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* key_integer_value  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 2256 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* key_bool_value  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_bool_value_) != nullptr) {
    delete ((*yyvaluep).key_bool_value_);
  }
}
#line 2266 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* index_type  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2276 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* opt_index_properties  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2286 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* insert_statement  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 2296 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* copy_statement  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_statement_) != nullptr) {
    delete ((*yyvaluep).copy_statement_);
  }
}
#line 2306 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* copy_to_target  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2316 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* opt_copy_params  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2326 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* update_statement  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2336 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* delete_statement  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2346 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* assignment_list  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2356 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* assignment_item  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2366 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* set_operation_statement  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_statement_) != nullptr) {
    delete ((*yyvaluep).set_operation_statement_);
  }
}
#line 2376 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* opt_priority_clause  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_priority_clause_) != nullptr) {
    delete ((*yyvaluep).opt_priority_clause_);
  }
}
#line 2386 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* with_clause  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2396 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* with_list  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2406 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 190: /* with_list_element  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2416 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 191: /* set_operation_union  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_) != nullptr) {
    delete ((*yyvaluep).set_operation_);
  }
}
#line 2426 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 192: /* set_operation_intersect  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_) != nullptr) {
    delete ((*yyvaluep).set_operation_);
  }
}
#line 2436 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 193: /* select_query  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2446 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 194: /* opt_all_distinct  */
#line 621 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2452 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 195: /* selection  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2462 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 196: /* selection_item_commalist  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2472 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 197: /* selection_item  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2482 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 198: /* from_clause  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2492 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 199: /* subquery_expression  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2502 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 200: /* opt_sample_clause  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_sample_clause_) != nullptr) {
    delete ((*yyvaluep).opt_sample_clause_);
  }
}
#line 2512 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 201: /* join_type  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2518 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 202: /* joined_table_reference  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2528 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 203: /* table_reference  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2538 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 204: /* table_reference_signature  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2548 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 205: /* table_reference_signature_primary  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2558 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 206: /* joined_table_reference_commalist  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2568 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 207: /* opt_group_by_clause  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2578 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 208: /* opt_having_clause  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2588 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 209: /* opt_order_by_clause  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2598 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 210: /* opt_limit_clause  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2608 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 211: /* opt_window_clause  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_window_clause_) != nullptr) {
    delete ((*yyvaluep).opt_window_clause_);
  }
}
#line 2618 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 212: /* window_declaration_list  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_window_clause_) != nullptr) {
    delete ((*yyvaluep).opt_window_clause_);
  }
}
#line 2628 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 213: /* window_declaration  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_definition_) != nullptr) {
    delete ((*yyvaluep).window_definition_);
  }
}
#line 2638 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 214: /* window_definition  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_definition_) != nullptr) {
    delete ((*yyvaluep).window_definition_);
  }
}
#line 2648 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 215: /* opt_window_partition  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_partition_by_list_) != nullptr) {
    delete ((*yyvaluep).window_partition_by_list_);
  }
}
#line 2658 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 216: /* opt_window_order  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_order_by_list_) != nullptr) {
    delete ((*yyvaluep).window_order_by_list_);
  }
}
#line 2668 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 217: /* opt_window_frame  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_frame_info_) != nullptr) {
    delete ((*yyvaluep).window_frame_info_);
  }
}
#line 2678 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 218: /* frame_mode  */
#line 621 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2684 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 219: /* frame_preceding  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2694 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 220: /* frame_following  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2704 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 221: /* order_commalist  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2714 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 222: /* order_item  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2724 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 223: /* opt_order_direction  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2734 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 224: /* opt_nulls_first  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2744 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 225: /* opt_where_clause  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2754 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 226: /* where_clause  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2764 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 227: /* or_expression  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2774 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 228: /* and_expression  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2784 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 229: /* not_expression  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2794 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 230: /* predicate_expression_base  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2804 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 231: /* add_expression  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2814 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 232: /* multiply_expression  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2824 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 233: /* unary_expression  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2834 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 234: /* expression_base  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2844 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 235: /* function_call  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2854 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 236: /* cast_function  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2864 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 237: /* extract_function  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2874 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 238: /* substr_function  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2884 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 239: /* case_expression  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2894 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 240: /* simple_when_clause_list  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_list_);
  }
}
#line 2904 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 241: /* simple_when_clause  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_);
  }
}
#line 2914 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 242: /* searched_when_clause_list  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_list_);
  }
}
#line 2924 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 243: /* searched_when_clause  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_);
  }
}
#line 2934 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 244: /* opt_else_clause  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2944 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 245: /* expression_list  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2954 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 246: /* literal_value  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2964 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 247: /* datetime_unit  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2974 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 248: /* literal_value_commalist  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2984 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 249: /* attribute_ref  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2994 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 250: /* attribute_ref_list  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 3004 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 251: /* comparison_operation  */
#line 622 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 3010 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 252: /* unary_operation  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).unary_operation_) != nullptr) {
    delete ((*yyvaluep).unary_operation_);
  }
}
#line 3020 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 253: /* add_operation  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).binary_operation_) != nullptr) {
    delete ((*yyvaluep).binary_operation_);
  }
}
#line 3030 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 254: /* multiply_operation  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).binary_operation_) != nullptr) {
    delete ((*yyvaluep).binary_operation_);
  }
}
#line 3040 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 255: /* name_commalist  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 3050 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 256: /* any_name  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 3060 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 257: /* boolean_value  */
#line 621 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 3066 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 258: /* command  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_) != nullptr) {
    delete ((*yyvaluep).command_);
  }
}
#line 3076 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 259: /* command_argument_list  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_argument_list_) != nullptr) {
    delete ((*yyvaluep).command_argument_list_);
  }
}
#line 3086 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;


      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (yyscan_t yyscanner, quickstep::ParseStatement **parsedStatement)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, &yylloc, yyscanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 634 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3383 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 638 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3392 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 642 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3401 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 5:
#line 646 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3410 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 650 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    YYABORT;
  }
#line 3418 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 653 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 3427 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 660 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3435 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 9:
#line 663 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].copy_statement_);
  }
#line 3443 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 10:
#line 666 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 3451 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 11:
#line 669 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3459 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 12:
#line 672 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 3467 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 13:
#line 675 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 3475 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 14:
#line 678 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 3483 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 15:
#line 681 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 3491 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 16:
#line 684 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].set_operation_statement_);
  }
#line 3499 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 17:
#line 687 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 3507 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 18:
#line 693 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3515 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 19:
#line 699 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3527 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 20:
#line 706 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3538 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 712 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3550 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 719 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3562 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 728 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].attribute_definition_list_), (yyvsp[-1].block_properties_), (yyvsp[0].partition_clause_));
  }
#line 3570 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 733 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].key_value_list_)) {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_), (yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].key_value_list_));
    } else {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_));
    }
  }
#line 3582 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 742 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 3590 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 747 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 3598 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 752 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3607 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 756 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3616 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 762 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 3626 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 767 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDate));
  }
#line 3634 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 770 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3642 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 773 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3652 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 778 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3660 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 781 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3668 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 784 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3676 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 787 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3684 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 790 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3692 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 793 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3700 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 796 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3708 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 799 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3716 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 802 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3724 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 805 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /**
     * NOTE(chasseur): This pattern exhibits a shift/reduce conflict with the
     * TOKEN_INTERVAL case in 'literal_value'. Bison prefers to shift rather
     * than reduce, so the case in 'literal_value' has precedence over this.
     **/
    (yyval.data_type_) = nullptr;
    quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr,
        "INTERVAL is ambiguous as a column type. Specify either DATETIME INTERVAL "
        "or YEARMONTH INTERVAL");
    YYERROR;
  }
#line 3741 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 817 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3749 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 820 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3757 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 823 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-1].numeric_literal_value_)->float_like()) {
      delete (yyvsp[-1].numeric_literal_value_);
      (yyval.data_type_) = NULL;
      quickstep_yyerror(&(yylsp[-1]), yyscanner, nullptr, "Non-integer length supplied for CHAR type");
      YYERROR;
    } else {
      if ((yyvsp[-1].numeric_literal_value_)->long_value() <= 0) {
        delete (yyvsp[-1].numeric_literal_value_);
        (yyval.data_type_) = NULL;
        quickstep_yyerror(&(yylsp[-1]), yyscanner, nullptr, "Length for CHAR type must be at least 1");
        YYERROR;
      } else {
        (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kChar, (yyvsp[-1].numeric_literal_value_)->long_value(), false));
        delete (yyvsp[-1].numeric_literal_value_);
      }
    }
  }
#line 3780 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 841 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-1].numeric_literal_value_)->float_like()) {
      delete (yyvsp[-1].numeric_literal_value_);
      (yyval.data_type_) = NULL;
      quickstep_yyerror(&(yylsp[-1]), yyscanner, nullptr, "Non-integer length supplied for VARCHAR type");
      YYERROR;
    } else {
      if ((yyvsp[-1].numeric_literal_value_)->long_value() < 0) {
        delete (yyvsp[-1].numeric_literal_value_);
        (yyval.data_type_) = NULL;
        quickstep_yyerror(&(yylsp[-1]), yyscanner, nullptr, "Negative length supplied for VARCHAR type");
        YYERROR;
      } else {
        (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kVarChar, (yyvsp[-1].numeric_literal_value_)->long_value(), false));
        delete (yyvsp[-1].numeric_literal_value_);
      }
    }
  }
#line 3803 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 861 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3811 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 864 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3819 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 867 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3829 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 872 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3839 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 877 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3850 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 883 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3861 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 889 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3873 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 898 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3882 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 902 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3891 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 908 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3899 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 911 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3907 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 916 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3917 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 921 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3927 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 926 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3939 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 933 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3949 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 940 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3958 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 944 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3967 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 950 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3975 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 953 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = $1; */
  }
#line 3983 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 958 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = nullptr;
  }
#line 3991 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 961 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-1].attribute_list_);
  }
#line 3999 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 966 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = nullptr;
  }
#line 4007 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 969 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].key_value_list_));
  }
#line 4015 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 974 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.partition_clause_) = nullptr;
  }
#line 4023 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 977 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.partition_clause_) = NULL;
      quickstep_yyerror(&(yylsp[0]), yyscanner, NULL, "NUMBER OF PARTITIONS must be an integer");
      YYERROR;
    } else {
      if ((yyvsp[0].numeric_literal_value_)->long_value() <= 0 || (yyvsp[0].numeric_literal_value_)->long_value() > 64) {
        delete (yyvsp[0].numeric_literal_value_);
        (yyval.partition_clause_) = NULL;
        quickstep_yyerror(&(yylsp[0]), yyscanner, NULL, "NUMBER OF PARITIONS must be between 1 and 64");
        YYERROR;
      } else {
        (yyval.partition_clause_) = new quickstep::ParsePartitionClause((yylsp[-7]).first_line, (yylsp[-7]).first_column, (yyvsp[-5].string_value_), (yyvsp[-3].string_list_), (yyvsp[0].numeric_literal_value_));
      }
    }
  }
#line 4045 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 996 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::kHashPartitionType);
  }
#line 4053 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 999 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::kRangePartitionType);
  }
#line 4061 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 1004 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 4070 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 1008 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 4079 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 1014 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 4087 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 1017 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 4095 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 1020 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 4103 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 1023 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_bool_value_);
  }
#line 4111 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 1028 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 4119 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 1031 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 4127 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 1034 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_),
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 4137 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 1041 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4145 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 1046 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 4159 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 1057 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_bool_value_) = new quickstep::ParseKeyBoolValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].boolean_value_));
  }
#line 4167 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 1062 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Defaults to BitWeavingV, but IndexProperties can change this to H.
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBitWeavingV));
  }
#line 4177 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 1067 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
#line 4186 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 1071 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
#line 4195 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 1075 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kSMA));
  }
#line 4204 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 1081 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 4212 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 1084 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 4220 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 1090 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 4233 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 1098 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertTuple((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 4241 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 1101 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-3]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), nullptr);
  }
#line 4249 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 1104 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-4]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), (yyvsp[-4].with_list_));
  }
#line 4257 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 1110 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-4]).first_line, (yylsp[-4]).first_column,
                                           quickstep::ParseStatementCopy::kFrom,
                                           (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4267 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 1115 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-3]).first_line, (yylsp[-3]).first_column,
                                           quickstep::ParseStatementCopy::kTo,
                                           (yyvsp[-2].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4277 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 1120 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-3]).first_line, (yylsp[-3]).first_column,
                                           (yyvsp[-2].set_operation_), nullptr, (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4286 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 1124 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-4]).first_line, (yylsp[-4]).first_column,
                                           (yyvsp[-2].set_operation_), (yyvsp[-4].with_list_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4295 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 1130 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yyvsp[0].string_value_)->line_number(), (yyvsp[0].string_value_)->column_number(), "@" + (yyvsp[0].string_value_)->value());
    delete (yyvsp[0].string_value_);
  }
#line 4304 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 1134 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "$stdout");
  }
#line 4312 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 1137 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "$stderr");
  }
#line 4320 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 1142 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 4328 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 1145 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 4336 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 1150 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 4344 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 1155 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 4352 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 1160 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 4361 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 1164 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 4370 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 1170 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 4378 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 1179 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_statement_) = new quickstep::ParseStatementSetOperation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].set_operation_), nullptr, (yyvsp[0].opt_priority_clause_));
  }
#line 4386 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 1182 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_statement_) = new quickstep::ParseStatementSetOperation((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].set_operation_), (yyvsp[-2].with_list_), (yyvsp[0].opt_priority_clause_));
  }
#line 4394 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 1187 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_priority_clause_) = nullptr;
  }
#line 4402 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 1190 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.opt_priority_clause_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "PRIORITY value must be an integer");
      YYERROR;
    } else {
      if ((yyvsp[0].numeric_literal_value_)->long_value() <= 0) {
        delete (yyvsp[0].numeric_literal_value_);
        (yyval.opt_priority_clause_) = nullptr;
        quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "PRIORITY value must be positive");
        YYERROR;
      } else {
        (yyval.opt_priority_clause_) = new quickstep::ParsePriority((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].numeric_literal_value_));
      }
    }
  }
#line 4424 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 1209 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 4432 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 1214 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4441 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 1218 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4450 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 1224 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 4459 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 1230 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-1].boolean_value_)) {
      (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-3]).first_line, (yylsp[-3]).first_column, quickstep::ParseSetOperation::kUnion);
    } else {
      (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-3]).first_line, (yylsp[-3]).first_column, quickstep::ParseSetOperation::kUnionAll);
    }
    (yyval.set_operation_)->addOperand((yyvsp[-3].set_operation_));
    (yyval.set_operation_)->addOperand((yyvsp[0].set_operation_));
  }
#line 4473 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 1239 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_) = (yyvsp[0].set_operation_);
  }
#line 4481 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 1244 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-2]).first_line, (yylsp[-2]).first_column, quickstep::ParseSetOperation::kIntersect);
    quickstep::ParseSetOperation *op = new quickstep::ParseSetOperation(
        (yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::ParseSetOperation::kSelect);
    op->addOperand((yyvsp[0].select_query_));
    (yyval.set_operation_)->addOperand((yyvsp[-2].set_operation_));
    (yyval.set_operation_)->addOperand(op);
  }
#line 4494 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 1252 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::ParseSetOperation::kSelect);
    (yyval.set_operation_)->addOperand((yyvsp[0].select_query_));
  }
#line 4503 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 1259 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-9]).first_line, (yylsp[-9]).first_column, (yyvsp[-7].selection_), (yyvsp[-6].table_reference_list_), (yyvsp[-5].predicate_), (yyvsp[-4].opt_group_by_clause_), (yyvsp[-3].opt_having_clause_), (yyvsp[-2].opt_order_by_clause_), (yyvsp[-1].opt_limit_clause_), (yyvsp[0].opt_window_clause_));
  }
#line 4511 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 1264 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;  // Distinct
  }
#line 4519 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 1267 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;  // All
  }
#line 4527 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 1270 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;  // Distinct
  }
#line 4535 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 1275 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4543 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 1278 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 4551 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 1283 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4560 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 1287 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4569 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 1293 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 4577 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 1296 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 4585 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 1299 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 4593 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 1304 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[0].table_reference_list_);
  }
#line 4601 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 1309 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].set_operation_));
  }
#line 4609 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 1314 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = NULL;
  }
#line 4617 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 1317 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, true, (yyvsp[-1].numeric_literal_value_));
  }
#line 4625 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 1320 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].numeric_literal_value_));
  }
#line 4633 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 1325 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4641 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 1328 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4649 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 1331 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4657 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 1334 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4665 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 1337 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4673 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 1340 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4681 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 1343 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4689 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 1346 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4697 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 1351 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseJoinedTableReference((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].join_type_), (yyvsp[-5].table_reference_), (yyvsp[-2].table_reference_), (yyvsp[0].predicate_));
  }
#line 4705 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 1354 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = (yyvsp[0].table_reference_);
  }
#line 4713 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 1359 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4722 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 1363 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].opt_sample_clause_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4731 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 1367 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].opt_sample_clause_));
  }
#line 4739 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 1370 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4748 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 1374 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].function_call_));
  }
#line 4756 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 1377 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = (yyvsp[-1].table_reference_);
  }
#line 4764 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 1382 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4772 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 1385 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4780 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 1390 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4788 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 1393 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4796 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 1398 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4805 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 1402 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4814 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 1408 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 4822 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 1411 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4830 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 1416 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4838 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 1419 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4846 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 1424 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4854 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 1427 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4862 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 1432 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4870 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 1435 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.opt_limit_clause_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "LIMIT value must be an integer");
      YYERROR;
    } else {
      if ((yyvsp[0].numeric_literal_value_)->long_value() <= 0) {
        delete (yyvsp[0].numeric_literal_value_);
        (yyval.opt_limit_clause_) = nullptr;
        quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "LIMIT value must be positive");
        YYERROR;
      } else {
        (yyval.opt_limit_clause_) = new quickstep::ParseLimit((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].numeric_literal_value_));
      }
    }
  }
#line 4892 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 1454 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = nullptr;
  }
#line 4900 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 1457 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = (yyvsp[0].opt_window_clause_);
  }
#line 4908 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 1462 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = new quickstep::PtrList<quickstep::ParseWindow>();
    (yyval.opt_window_clause_)->push_back((yyvsp[0].window_definition_));
  }
#line 4917 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 1466 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = (yyvsp[-1].opt_window_clause_);
    (yyval.opt_window_clause_)->push_back((yyvsp[0].window_definition_));
  }
#line 4926 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 1472 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_definition_) = (yyvsp[-1].window_definition_);
    (yyval.window_definition_)->setName((yyvsp[-4].string_value_));
  }
#line 4935 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 1478 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_definition_) = new quickstep::ParseWindow((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].window_partition_by_list_), (yyvsp[-1].window_order_by_list_), (yyvsp[0].window_frame_info_));
  }
#line 4943 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 1483 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_partition_by_list_) = nullptr;
  }
#line 4951 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 1486 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_partition_by_list_) = (yyvsp[0].expression_list_);
  }
#line 4959 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 1491 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_order_by_list_) = nullptr;
  }
#line 4967 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 1494 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_order_by_list_) = (yyvsp[0].order_commalist_);
  }
#line 4975 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 1499 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_frame_info_) = nullptr;
  }
#line 4983 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 1502 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_frame_info_) = new quickstep::ParseFrameInfo((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-4].boolean_value_), (yyvsp[-2].numeric_literal_value_)->long_value(), (yyvsp[0].numeric_literal_value_)->long_value());
  }
#line 4991 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 1507 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4999 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 1510 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5007 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 1516 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "-1");
  }
#line 5015 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 1519 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "0");
  }
#line 5023 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1525 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "-1");
  }
#line 5031 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 1528 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "0");
  }
#line 5039 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 1533 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 5048 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 1537 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 5057 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 1543 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 5067 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 1550 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 5075 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 1553 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 5083 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 1556 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 5091 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 1561 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 5099 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 1564 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 5107 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 1567 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 5115 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 1573 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 5123 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 1576 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5131 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 1581 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5139 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1586 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 5153 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1595 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5161 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1600 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 5175 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1609 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5183 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1614 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 5191 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1617 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5199 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1622 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5207 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1625 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 5217 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1630 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 5228 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1636 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 5239 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1642 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5247 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1645 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 5255 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1648 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateExists((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].subquery_expression_));
  }
#line 5263 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1651 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-2].expression_), (yyvsp[0].subquery_expression_));
  }
#line 5271 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1654 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-1].expression_list_));
  }
#line 5279 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1657 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-2]).first_line,
        (yylsp[-2]).first_column,
        new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-3].expression_), (yyvsp[0].subquery_expression_)));
  }
#line 5290 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1663 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line,
        (yylsp[-4]).first_column,
        new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-1].expression_list_)));
  }
#line 5301 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1672 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-2].expression_));
    arguments->push_back((yyvsp[0].expression_));
    (yyval.expression_) = new quickstep::ParseFunctionCall((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].binary_operation_), arguments);
  }
#line 5312 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1678 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5320 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1683 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-2].expression_));
    arguments->push_back((yyvsp[0].expression_));
    (yyval.expression_) = new quickstep::ParseFunctionCall((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].binary_operation_), arguments);
  }
#line 5331 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1689 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5339 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1694 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[0].expression_));
    (yyval.expression_) = new quickstep::ParseFunctionCall((yylsp[-1]).first_line, (yylsp[-1]).first_column, false, (yyvsp[-1].unary_operation_), arguments);
  }
#line 5349 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1699 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5357 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1704 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 5365 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1707 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 5373 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1710 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 5381 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1713 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyvsp[-2].function_call_)->setWindowName((yyvsp[0].string_value_));
    (yyval.expression_) = (yyvsp[-2].function_call_);
  }
#line 5390 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1717 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyvsp[-4].function_call_)->setWindow((yyvsp[-1].window_definition_));
    (yyval.expression_) = (yyvsp[-4].function_call_);
  }
#line 5399 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1721 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5407 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1724 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5415 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1727 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5423 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1730 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5431 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1733 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 5439 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1736 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].subquery_expression_);
  }
#line 5447 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1741 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 5456 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1745 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 5465 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1749 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, false, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 5473 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1752 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-4]).first_line, (yylsp[-4]).first_column, true, (yyvsp[-4].string_value_), (yyvsp[-1].expression_list_));
  }
#line 5481 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1757 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-3].expression_));
    arguments->push_back(new quickstep::ParseScalarLiteral(
        new quickstep::StringParseLiteralValue(
            new quickstep::ParseString((yylsp[-1]).first_line,
                                       (yylsp[-1]).first_column,
                                       (yyvsp[-1].data_type_)->getType().getName()),
            nullptr)));
    delete (yyvsp[-1].data_type_);
    auto *name = new quickstep::ParseString((yylsp[-5]).first_line, (yylsp[-5]).first_column, "cast");
    (yyval.expression_) = new quickstep::ParseFunctionCall(
        (yylsp[-5]).first_line, (yylsp[-5]).first_column, false, name, arguments);
  }
#line 5500 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1771 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-3].expression_));
    arguments->push_back(new quickstep::ParseScalarLiteral(
        new quickstep::StringParseLiteralValue((yyvsp[-1].string_value_), nullptr)));
    auto *name = new quickstep::ParseString((yylsp[-5]).first_line, (yylsp[-5]).first_column, "cast");
    (yyval.expression_) = new quickstep::ParseFunctionCall(
        (yylsp[-5]).first_line, (yylsp[-5]).first_column, false, name, arguments);
  }
#line 5514 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1782 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-1].expression_));
    arguments->push_back(new quickstep::ParseScalarLiteral(
        new quickstep::StringParseLiteralValue((yyvsp[-3].string_value_), nullptr)));
    auto *name = new quickstep::ParseString((yylsp[-5]).first_line, (yylsp[-5]).first_column, "extract");
    (yyval.expression_) = new quickstep::ParseFunctionCall(
        (yylsp[-5]).first_line, (yylsp[-5]).first_column, false, name, arguments);
  }
#line 5528 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1793 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-3].expression_));
    arguments->push_back(new quickstep::ParseScalarLiteral((yyvsp[-1].numeric_literal_value_)));
    auto *name = new quickstep::ParseString((yylsp[-5]).first_line, (yylsp[-5]).first_column, "substring");
    (yyval.expression_) = new quickstep::ParseFunctionCall(
        (yylsp[-5]).first_line, (yylsp[-5]).first_column, false, name, arguments);
  }
#line 5541 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1801 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-5].expression_));
    arguments->push_back(new quickstep::ParseScalarLiteral((yyvsp[-3].numeric_literal_value_)));
    arguments->push_back(new quickstep::ParseScalarLiteral((yyvsp[-1].numeric_literal_value_)));
    auto *name = new quickstep::ParseString((yylsp[-7]).first_line, (yylsp[-7]).first_column, "substring");
    (yyval.expression_) = new quickstep::ParseFunctionCall(
        (yylsp[-7]).first_line, (yylsp[-7]).first_column, false, name, arguments);
  }
#line 5555 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1812 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSimpleCaseExpression((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].expression_), (yyvsp[-2].simple_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 5563 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1815 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSearchedCaseExpression((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].searched_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 5571 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1820 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSimpleWhenClause>;
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 5580 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1824 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_list_) = (yyvsp[-1].simple_when_clause_list_);
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 5589 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1830 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_) = new quickstep::ParseSimpleWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5597 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1835 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSearchedWhenClause>;
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 5606 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1839 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_list_) = (yyvsp[-1].searched_when_clause_list_);
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 5615 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1845 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_) = new quickstep::ParseSearchedWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].predicate_), (yyvsp[0].expression_));
  }
#line 5623 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1850 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = NULL;
  }
#line 5631 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1853 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5639 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1858 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 5648 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1862 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 5657 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1868 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 5665 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1871 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5673 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1874 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5681 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1877 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * minus character as a 'unary_operation' followed by a numeric literal.
     * Because Bison prefers to shift rather than reduce, this case has
     * precedence (i.e. the parser will prefer to interpret the ambiguous
     * pattern as a negative number literal rather than a unary minus operation
     * applied to a non-negative number literal).
     **/
    (yyvsp[0].numeric_literal_value_)->prependMinus();
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5698 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1889 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 5707 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1893 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * plain TOKEN_INTERVAL case in 'data_type' reduced and used in the case
     * below. Because Bison prefers to shift rather than reduce, this case has
     * precedence (i.e. the special
     * StringParseLiteralValue::ParseAmbiguousInterval() method will be used to
     * parse the string as either one of the interval types, rather than an
     * error being emitted because of an ambiguous type).
     **/
    quickstep::StringParseLiteralValue *parse_value;
    if (quickstep::StringParseLiteralValue::ParseAmbiguousInterval((yyvsp[0].string_value_), &parse_value)) {
      (yyval.literal_value_) = parse_value;
    } else {
      (yyval.literal_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Failed to parse literal as specified type");
      YYERROR;
    }
  }
#line 5731 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1912 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    quickstep::StringParseLiteralValue *parse_value;
    const std::string &datetime_type_value = (yyvsp[0].string_value_)->value();
    if (quickstep::StringParseLiteralValue::ParseAmbiguousInterval(
        &((yyvsp[-1].string_value_)->append((" " + datetime_type_value).c_str(), datetime_type_value.length() + 1)),
        &parse_value)) {
      (yyval.literal_value_) = parse_value;
    } else {
      (yyval.literal_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Failed to parse literal as specified type");
      YYERROR;
    }
  }
#line 5749 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1925 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    quickstep::StringParseLiteralValue *parse_value
        = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_), &((yyvsp[-1].data_type_)->getType()));
    delete (yyvsp[-1].data_type_);
    if (!parse_value->tryExplicitTypeParse()) {
      delete parse_value;
      (yyval.literal_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Failed to parse literal as specified type");
      YYERROR;
    } else {
      (yyval.literal_value_) = parse_value;
    }
  }
#line 5767 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1940 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("YEAR"));
  }
#line 5775 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1943 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("MONTH"));
  }
#line 5783 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1946 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("DAY"));
  }
#line 5791 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1949 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("HOUR"));
  }
#line 5799 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1952 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("MINUTE"));
  }
#line 5807 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1955 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("SECOND"));
  }
#line 5815 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1960 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5824 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1964 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5833 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1970 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 5841 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1973 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 5849 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1978 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = new quickstep::PtrList<quickstep::ParseAttribute>();
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5858 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1982 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-2].attribute_list_);
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5867 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1989 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 5875 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1992 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 5883 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1995 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 5891 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1998 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 5899 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 2001 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 5907 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 2004 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 5915 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 2007 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLike);
  }
#line 5923 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 2010 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotLike);
  }
#line 5931 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 2013 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kRegexMatch);
  }
#line 5939 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 2016 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotRegexMatch);
  }
#line 5947 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 2021 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("-"));
  }
#line 5961 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 2032 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("+"));
  }
#line 5969 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 2035 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("-"));
  }
#line 5977 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 2040 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("%"));
  }
#line 5985 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 2043 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("*"));
  }
#line 5993 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 2046 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("/"));
  }
#line 6001 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 2052 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 6010 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 2056 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 6019 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 2062 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 6027 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 2065 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 6038 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 2073 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 6046 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 2076 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 6054 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 2079 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 6062 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 2082 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 6070 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 2088 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.command_) = new quickstep::ParseCommand((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].command_argument_list_));
  }
#line 6078 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 2093 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    quickstep::PtrVector<quickstep::ParseString> *argument_list = (yyvsp[-1].command_argument_list_);
    argument_list->push_back((yyvsp[0].string_value_));
    (yyval.command_argument_list_) = argument_list;
  }
#line 6088 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 2098 "../SqlParser.ypp" /* yacc.c:1661  */
    { /* Epsilon, an empy match. */
    (yyval.command_argument_list_) = new quickstep::PtrVector<quickstep::ParseString>();
  }
#line 6096 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;


#line 6100 "SqlParser_gen.cpp" /* yacc.c:1661  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, yyscanner, parsedStatement, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, yyscanner, parsedStatement, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, yyscanner, parsedStatement);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, yyscanner, parsedStatement);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, yyscanner, parsedStatement, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, yyscanner, parsedStatement);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, yyscanner, parsedStatement);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 2102 "../SqlParser.ypp" /* yacc.c:1906  */


void NotSupported(const YYLTYPE *location, yyscan_t yyscanner, const std::string &feature) {
  std::string msg;
  msg.append(feature);
  msg.append(" is not supported yet");

  quickstep_yyerror(location, yyscanner, nullptr, msg.c_str());
}

int quickstep_yyget_line_number(const YYLTYPE *yyloc) {
  return yyloc->first_line;
}

int quickstep_yyget_column_number(const YYLTYPE *yyloc) {
  return yyloc->first_column;
}
