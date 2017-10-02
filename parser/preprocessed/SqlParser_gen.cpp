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
    TOKEN_DOUBLECOLON = 311,
    TOKEN_DROP = 312,
    TOKEN_ELSE = 313,
    TOKEN_END = 314,
    TOKEN_EXISTS = 315,
    TOKEN_EXTRACT = 316,
    TOKEN_FALSE = 317,
    TOKEN_FIRST = 318,
    TOKEN_FLOAT = 319,
    TOKEN_FOLLOWING = 320,
    TOKEN_FOR = 321,
    TOKEN_FOREIGN = 322,
    TOKEN_FROM = 323,
    TOKEN_FULL = 324,
    TOKEN_GROUP = 325,
    TOKEN_HASH = 326,
    TOKEN_HAVING = 327,
    TOKEN_HOUR = 328,
    TOKEN_IN = 329,
    TOKEN_INDEX = 330,
    TOKEN_INNER = 331,
    TOKEN_INSERT = 332,
    TOKEN_INTEGER = 333,
    TOKEN_INTERVAL = 334,
    TOKEN_INTO = 335,
    TOKEN_JOIN = 336,
    TOKEN_KEY = 337,
    TOKEN_LAST = 338,
    TOKEN_LBRACE = 339,
    TOKEN_LEFT = 340,
    TOKEN_LIMIT = 341,
    TOKEN_LONG = 342,
    TOKEN_MINUTE = 343,
    TOKEN_MONTH = 344,
    TOKEN_NULL = 345,
    TOKEN_NULLS = 346,
    TOKEN_OFF = 347,
    TOKEN_ON = 348,
    TOKEN_ORDER = 349,
    TOKEN_OUTER = 350,
    TOKEN_OVER = 351,
    TOKEN_PARTITION = 352,
    TOKEN_PARTITIONS = 353,
    TOKEN_PERCENT = 354,
    TOKEN_PRECEDING = 355,
    TOKEN_PRIMARY = 356,
    TOKEN_PRIORITY = 357,
    TOKEN_QUIT = 358,
    TOKEN_RANGE = 359,
    TOKEN_RBRACE = 360,
    TOKEN_REAL = 361,
    TOKEN_REFERENCES = 362,
    TOKEN_RIGHT = 363,
    TOKEN_ROW = 364,
    TOKEN_ROW_DELIMITER = 365,
    TOKEN_ROWS = 366,
    TOKEN_SECOND = 367,
    TOKEN_SELECT = 368,
    TOKEN_SET = 369,
    TOKEN_SMA = 370,
    TOKEN_SMALLINT = 371,
    TOKEN_STDERR = 372,
    TOKEN_STDOUT = 373,
    TOKEN_SUBSTRING = 374,
    TOKEN_TABLE = 375,
    TOKEN_THEN = 376,
    TOKEN_TIME = 377,
    TOKEN_TIMESTAMP = 378,
    TOKEN_TO = 379,
    TOKEN_TRUE = 380,
    TOKEN_TUPLESAMPLE = 381,
    TOKEN_UNBOUNDED = 382,
    TOKEN_UNIQUE = 383,
    TOKEN_UPDATE = 384,
    TOKEN_USING = 385,
    TOKEN_VALUES = 386,
    TOKEN_VARCHAR = 387,
    TOKEN_WHEN = 388,
    TOKEN_WHERE = 389,
    TOKEN_WINDOW = 390,
    TOKEN_WITH = 391,
    TOKEN_YEAR = 392,
    TOKEN_YEARMONTH = 393,
    TOKEN_EOF = 394,
    TOKEN_LEX_ERROR = 395
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

  quickstep::ParseArray *array_expression_;
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

#line 434 "SqlParser_gen.cpp" /* yacc.c:355  */
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
#line 217 "../SqlParser.ypp" /* yacc.c:358  */

/* This header needs YYSTYPE, which is defined by the %union directive above */
#include "SqlLexer_gen.hpp"
void NotSupported(const YYLTYPE *location, yyscan_t yyscanner, const std::string &feature);

#line 469 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYLAST   1626

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  152
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  113
/* YYNRULES -- Number of rules.  */
#define YYNRULES  308
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  570

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   395

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     147,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   151,     2,     2,
     148,   149,    23,    21,   150,    22,    27,    24,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   146,
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
     140,   141,   142,   143,   144,   145
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   643,   643,   647,   651,   655,   659,   662,   669,   672,
     675,   678,   681,   684,   687,   690,   693,   696,   702,   708,
     715,   721,   728,   737,   742,   751,   756,   761,   765,   771,
     776,   779,   782,   787,   790,   793,   796,   799,   802,   805,
     808,   811,   814,   826,   829,   832,   850,   870,   873,   876,
     881,   886,   892,   898,   907,   911,   917,   920,   925,   930,
     935,   942,   949,   953,   959,   962,   967,   970,   975,   978,
     983,   986,  1005,  1008,  1013,  1017,  1023,  1026,  1029,  1032,
    1037,  1040,  1043,  1050,  1055,  1066,  1071,  1076,  1080,  1084,
    1090,  1093,  1099,  1107,  1110,  1113,  1119,  1124,  1129,  1133,
    1139,  1143,  1146,  1151,  1154,  1159,  1164,  1169,  1173,  1179,
    1188,  1191,  1196,  1199,  1218,  1223,  1227,  1233,  1239,  1248,
    1253,  1261,  1267,  1273,  1276,  1279,  1284,  1287,  1292,  1296,
    1302,  1305,  1308,  1313,  1318,  1323,  1326,  1329,  1334,  1337,
    1340,  1343,  1346,  1349,  1352,  1355,  1360,  1363,  1368,  1372,
    1376,  1379,  1383,  1386,  1391,  1394,  1399,  1402,  1407,  1411,
    1417,  1420,  1425,  1428,  1433,  1436,  1441,  1444,  1463,  1466,
    1471,  1475,  1481,  1487,  1492,  1495,  1500,  1503,  1508,  1511,
    1516,  1519,  1524,  1525,  1528,  1533,  1534,  1537,  1542,  1546,
    1552,  1559,  1562,  1565,  1570,  1573,  1576,  1582,  1585,  1590,
    1595,  1604,  1609,  1618,  1623,  1626,  1631,  1634,  1639,  1645,
    1651,  1654,  1657,  1660,  1663,  1666,  1672,  1681,  1687,  1692,
    1698,  1703,  1708,  1713,  1716,  1719,  1722,  1725,  1729,  1733,
    1736,  1739,  1742,  1745,  1748,  1753,  1756,  1762,  1766,  1773,
    1777,  1781,  1784,  1789,  1803,  1812,  1826,  1837,  1848,  1856,
    1867,  1870,  1875,  1879,  1885,  1890,  1894,  1900,  1905,  1908,
    1913,  1917,  1923,  1926,  1929,  1932,  1944,  1948,  1967,  1980,
    1995,  1998,  2001,  2004,  2007,  2010,  2015,  2019,  2025,  2028,
    2033,  2037,  2044,  2047,  2050,  2053,  2056,  2059,  2062,  2065,
    2068,  2071,  2076,  2087,  2090,  2095,  2098,  2101,  2107,  2111,
    2117,  2120,  2128,  2131,  2134,  2137,  2143,  2148,  2153
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
  "TOKEN_DOUBLE", "TOKEN_DOUBLECOLON", "TOKEN_DROP", "TOKEN_ELSE",
  "TOKEN_END", "TOKEN_EXISTS", "TOKEN_EXTRACT", "TOKEN_FALSE",
  "TOKEN_FIRST", "TOKEN_FLOAT", "TOKEN_FOLLOWING", "TOKEN_FOR",
  "TOKEN_FOREIGN", "TOKEN_FROM", "TOKEN_FULL", "TOKEN_GROUP", "TOKEN_HASH",
  "TOKEN_HAVING", "TOKEN_HOUR", "TOKEN_IN", "TOKEN_INDEX", "TOKEN_INNER",
  "TOKEN_INSERT", "TOKEN_INTEGER", "TOKEN_INTERVAL", "TOKEN_INTO",
  "TOKEN_JOIN", "TOKEN_KEY", "TOKEN_LAST", "TOKEN_LBRACE", "TOKEN_LEFT",
  "TOKEN_LIMIT", "TOKEN_LONG", "TOKEN_MINUTE", "TOKEN_MONTH", "TOKEN_NULL",
  "TOKEN_NULLS", "TOKEN_OFF", "TOKEN_ON", "TOKEN_ORDER", "TOKEN_OUTER",
  "TOKEN_OVER", "TOKEN_PARTITION", "TOKEN_PARTITIONS", "TOKEN_PERCENT",
  "TOKEN_PRECEDING", "TOKEN_PRIMARY", "TOKEN_PRIORITY", "TOKEN_QUIT",
  "TOKEN_RANGE", "TOKEN_RBRACE", "TOKEN_REAL", "TOKEN_REFERENCES",
  "TOKEN_RIGHT", "TOKEN_ROW", "TOKEN_ROW_DELIMITER", "TOKEN_ROWS",
  "TOKEN_SECOND", "TOKEN_SELECT", "TOKEN_SET", "TOKEN_SMA",
  "TOKEN_SMALLINT", "TOKEN_STDERR", "TOKEN_STDOUT", "TOKEN_SUBSTRING",
  "TOKEN_TABLE", "TOKEN_THEN", "TOKEN_TIME", "TOKEN_TIMESTAMP", "TOKEN_TO",
  "TOKEN_TRUE", "TOKEN_TUPLESAMPLE", "TOKEN_UNBOUNDED", "TOKEN_UNIQUE",
  "TOKEN_UPDATE", "TOKEN_USING", "TOKEN_VALUES", "TOKEN_VARCHAR",
  "TOKEN_WHEN", "TOKEN_WHERE", "TOKEN_WINDOW", "TOKEN_WITH", "TOKEN_YEAR",
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
  "unary_expression", "expression_base", "array_expression",
  "array_element_commalist", "function_call", "cast_function",
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
     390,   391,   392,   393,   394,   395,    59,    10,    40,    41,
      44,    37
};
# endif

#define YYPACT_NINF -280

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-280)))

#define YYTABLE_NINF -139

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     185,  -280,  -280,   -26,    38,     6,    29,    16,    26,  -280,
      47,   210,   210,  -280,   215,    84,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,   173,    66,   201,  -280,
     -54,   222,   210,  -280,  -280,     1,   135,   210,   210,   210,
     210,   210,  -280,  -280,   768,   138,   113,  -280,   236,   117,
    -280,  -280,  -280,   154,   189,    66,    47,   187,  -280,   154,
    -280,  -280,  -280,    50,    22,   151,   294,   151,   214,   160,
     177,  -280,   -14,  -280,  -280,   314,   317,  -280,  -280,  -280,
     865,   170,   174,  -280,   246,  -280,  -280,   190,  -280,  -280,
     337,   962,  -280,  -280,  -280,  -280,   204,  -280,  -280,   209,
     279,  1059,   355,   292,   217,  -280,  -280,   358,    40,  -280,
     313,  -280,   -18,   277,  -280,  -280,  -280,  -280,  -280,  -280,
    1265,     5,   210,   210,   235,   210,     1,   210,  -280,   154,
     379,  -280,    81,   162,  -280,  -280,  -280,   242,  -280,   151,
    -280,   210,   210,   671,  -280,  -280,   260,   210,  -280,  -280,
    -280,   671,    63,    51,  -280,  1362,   387,  -280,   183,   183,
    -280,   276,  1362,   410,  -280,    -9,    46,  -280,     9,   177,
    1362,  -280,  -280,   210,  1362,  -280,  -280,  -280,  -280,  1362,
     340,  -280,  1362,    19,   317,   313,   210,   437,    73,  -280,
     424,  -280,   154,  -280,   191,  -280,   151,   154,   201,  -280,
     210,   172,   210,   210,   210,  -280,   288,  -280,   200,  1483,
    1168,   235,   562,   430,   431,  -280,  -280,   525,   419,  1442,
     205,    52,  1362,    54,  -280,  1362,  -280,   381,   315,   297,
    -280,  -280,  -280,  -280,  -280,  -280,   374,  -280,   198,   299,
    -280,  -280,    18,   220,   253,  -280,   300,   220,     2,   377,
    -280,  -280,    40,  -280,  -280,  -280,  -280,   276,   347,  -280,
    -280,   304,  1362,  -280,   276,   221,   210,  -280,  1362,  -280,
     210,  -280,  -280,  -280,   306,   368,   370,   316,  -280,  -280,
    -280,   223,  -280,  -280,  -280,  -280,  -280,    12,   210,   327,
     172,   210,   195,  -280,  -280,    21,    36,   671,   671,   231,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  1362,   318,
    1362,    86,  -280,   232,   329,  1362,    67,  -280,   405,   276,
    -280,   340,  -280,  1362,   463,  -280,   155,   210,  -280,  -280,
     371,  -280,   375,   376,   388,     9,  -280,   471,   477,   220,
     443,   414,   445,   344,   395,  -280,   239,  -280,  1362,  -280,
     276,  -280,   671,   350,   351,   210,  -280,   210,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,   210,  -280,  -280,  -280,   247,
     468,    60,  -280,   352,   359,  -280,   406,   357,  1442,  -280,
     420,   210,  -280,  -280,   195,  -280,  -280,   431,  -280,  -280,
    -280,  1362,   360,   237,  1059,  -280,   276,   415,  -280,  -280,
    1442,   361,   276,  1362,  -280,   362,   363,    55,    -1,  -280,
    -280,  -280,  -280,  -280,     9,   253,   409,   412,  -280,  1362,
     671,   418,  1362,  -280,   472,   -22,  -280,   276,    23,   210,
     210,   249,  -280,   252,  -280,   210,  -280,  -280,  -280,  -280,
     380,   172,   480,   417,  -280,   671,  -280,  -280,   382,  -280,
     293,  1059,  -280,  1362,   254,  -280,  -280,  1442,   276,  -280,
    -280,  -280,   515,  -280,   425,  -280,  -280,   383,   430,   482,
     434,   383,  1362,  -280,  -280,  -280,   512,  -280,   257,   262,
    -280,  -280,  -280,   210,  -280,  -280,   386,   507,  -280,    35,
     210,  1362,   265,   276,  -280,   270,   401,   671,  1362,   544,
     416,   402,  -280,   311,    31,   442,  -280,   272,   210,    15,
    -280,   408,   276,  -280,  -280,  -280,   430,   402,  -280,   210,
    -280,   416,  -280,  1362,  -280,  -280,   459,   454,   446,   457,
     554,   210,  -280,   278,  -280,  -280,   422,  -280,   538,  -280,
    -280,    37,  -280,  -280,  -280,  -280,    62,   427,  -280,   210,
     428,  -280,  -280,   503,   464,   509,  -280,   210,   280,   347,
    -280,  -280,  -280,   284,   474,   432,  -280,   575,  -280,  -280
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     6,   308,     0,     0,     0,     0,     0,     0,    18,
     123,     0,     0,     7,     0,     0,    15,     8,    10,    11,
      13,    14,     9,    17,    12,    16,     0,   112,   119,   121,
       0,   306,     0,   300,   301,     0,     0,     0,     0,     0,
       0,     0,   124,   125,     0,     0,   114,   115,     0,   156,
       1,     3,     2,     0,     0,   112,   123,     0,   110,     0,
       5,     4,   307,     0,     0,   103,     0,   103,     0,     0,
     197,    25,     0,   266,   263,     0,   292,   126,    40,    29,
       0,     0,     0,    30,    31,    34,    36,     0,    37,    39,
       0,     0,    41,   262,    35,    38,     0,    32,    33,     0,
       0,     0,     0,     0,   127,   128,   234,   132,   218,   220,
     222,   225,     0,   226,   229,   230,   231,   232,   224,   223,
       0,   278,     0,     0,     0,     0,     0,     0,   111,     0,
       0,   120,     0,     0,   100,   102,   101,     0,    98,   103,
      97,     0,     0,     0,   106,   198,     0,     0,    94,   264,
     265,     0,     0,   258,   255,     0,     0,    43,     0,   267,
     236,   238,     0,     0,    44,     0,     0,   269,     0,   197,
       0,   293,   294,     0,     0,   131,   296,   297,   295,     0,
       0,   235,     0,     0,     0,   221,     0,     0,   197,   108,
       0,   116,     0,   117,     0,   298,   103,     0,   118,   113,
       0,     0,     0,     0,     0,    96,    66,    27,     0,     0,
       0,     0,     0,   199,   201,   203,   205,     0,   223,     0,
       0,     0,     0,   258,   252,     0,   256,     0,     0,     0,
     272,   273,   274,   271,   275,   270,     0,   268,     0,     0,
     134,   233,     0,     0,   158,   147,   133,   152,   135,   160,
     129,   130,   217,   219,    42,   245,   246,   237,   174,   227,
     279,     0,     0,   239,   260,     0,     0,   105,     0,   157,
       0,    99,    95,    19,     0,     0,     0,     0,    20,    21,
      22,     0,    74,    76,    77,    78,    79,     0,     0,     0,
      64,     0,    56,   204,   212,     0,     0,     0,     0,     0,
     282,   284,   285,   286,   287,   283,   288,   290,     0,     0,
       0,     0,   276,     0,     0,     0,     0,   253,     0,   259,
     251,     0,    45,     0,     0,    46,   138,     0,   148,   154,
     144,   139,   140,   142,     0,     0,   151,     0,     0,   150,
       0,   162,     0,     0,   176,   240,     0,   241,     0,   107,
     109,   299,     0,     0,     0,     0,   104,     0,    81,    84,
      82,   304,   305,   303,   302,     0,    80,    85,   280,     0,
     278,     0,    63,    65,    68,    28,     0,     0,     0,    47,
       0,     0,    49,    55,    57,    26,   211,   200,   202,   289,
     291,     0,     0,     0,     0,   213,   210,     0,   209,    93,
       0,     0,   257,     0,   250,     0,     0,     0,     0,   153,
     155,   145,   141,   143,     0,   159,     0,     0,   149,     0,
       0,   164,     0,   228,     0,   178,   242,   261,     0,     0,
       0,     0,    75,     0,    67,     0,    86,    87,    88,    89,
      90,     0,     0,    70,    48,     0,    51,    50,     0,    54,
       0,     0,   215,     0,     0,   208,   277,     0,   254,   243,
     244,   247,     0,   248,     0,   136,   137,   161,   163,     0,
     166,   175,     0,   181,   180,   173,     0,    61,     0,     0,
      58,    83,   281,     0,    24,    62,     0,     0,    23,     0,
       0,     0,     0,   206,   214,     0,     0,     0,     0,     0,
     168,   177,   188,   191,     0,     0,    59,     0,     0,     0,
      52,     0,   207,   216,    92,   249,   146,   165,   167,     0,
     122,   169,   170,     0,   192,   193,   194,     0,     0,     0,
       0,     0,    91,     0,    72,    73,     0,    53,     0,   171,
     189,     0,   190,   182,   184,   183,     0,     0,    69,     0,
       0,   195,   196,     0,     0,     0,   179,     0,     0,   174,
     185,   187,   186,     0,     0,     0,    60,     0,   172,    71
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,  -126,  -280,
    -170,   203,  -280,  -280,  -279,  -280,  -280,  -280,  -280,  -280,
    -280,  -229,   233,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
    -280,    42,   -41,  -280,  -280,  -280,   322,  -280,   534,  -280,
    -280,   469,   256,   462,     0,   537,  -280,  -280,   426,  -280,
    -103,  -280,  -280,  -181,   180,  -164,    -7,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,    74,    41,  -280,  -280,  -280,  -280,
    -280,  -280,   101,    78,  -280,  -280,   -62,  -280,  -150,   305,
     310,   393,   -35,   435,   433,   490,  -280,  -280,  -162,  -280,
    -280,  -280,  -280,  -280,   390,  -280,   458,   396,  -248,  -217,
     461,   159,  -139,  -280,  -280,  -280,  -280,  -280,  -144,    -4,
    -280,  -280,  -280
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    14,    15,    16,    17,    18,    19,    20,   207,   208,
     102,   383,   384,   385,   278,   373,   374,   289,   443,   488,
     536,   281,   282,   283,   284,   285,   286,   440,   484,    21,
      22,    65,   138,    23,    24,   188,   189,    25,    58,    26,
      46,    47,   165,    28,    29,    44,   103,   104,   105,   169,
     106,   339,   334,   244,   245,   328,   329,   246,   341,   421,
     470,   500,   520,   521,   522,   343,   344,   425,   475,   476,
     530,   556,   501,   502,   526,   542,   144,   145,   213,   214,
     215,   216,   217,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   223,   224,   153,   154,   227,   265,   118,
     236,   313,   119,   369,   310,   120,   174,   179,   194,   121,
     367,    30,    31
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      36,   221,   312,   220,   218,    48,   247,    45,    49,   107,
     255,   372,   218,    33,   346,    34,    33,   358,    34,   359,
      56,   193,    33,    33,    34,    34,   140,   134,    63,   297,
      56,   297,   186,    68,    69,    70,    71,    72,   527,   292,
     360,   337,    33,   297,    34,   152,   299,   300,   301,   302,
     303,   304,   305,   306,   307,   308,   161,   171,   172,   131,
     297,   326,   295,   176,   177,   243,   166,   171,   172,   553,
     462,   218,   148,   218,   273,    42,   171,   172,    67,   361,
     247,   132,   528,   336,   171,   172,    37,   473,   171,   172,
      60,   534,   181,    61,   474,    56,   397,   436,   205,    32,
     437,   438,    39,   175,    10,   551,    43,   249,   294,   362,
     363,    41,   133,   554,   225,   309,    48,   225,   190,    49,
     228,   195,   146,   197,   535,   552,   267,   238,   200,   201,
      64,    38,   182,   338,   147,   107,    10,   206,   209,   243,
     240,    40,   364,   195,   135,   136,   454,   257,   463,   368,
     187,   405,   264,   187,   415,   271,    10,   242,   218,   218,
     365,   446,   485,   529,   248,   375,   242,   258,   196,   251,
     386,   467,   477,   247,   471,   418,   256,   296,   315,   259,
     439,   398,   260,   456,   510,   241,     1,   316,     2,   151,
     319,   178,   222,   403,   555,   241,   209,   272,   279,   280,
     287,   222,   428,   492,   461,   376,   395,    57,    66,   202,
     203,   431,   143,   218,    33,    50,    34,     3,   274,   171,
     172,   433,    53,   266,    33,    62,    34,   264,    51,   330,
      52,    59,   243,   350,     4,     5,   331,   230,   248,    49,
     312,   377,     6,    49,   275,   332,   453,     7,   389,   390,
     391,   378,   247,   327,   507,    54,    27,   122,   171,   172,
      35,   231,   190,   123,    64,   125,   351,     8,   333,   124,
     468,   324,    10,   393,   127,   396,   232,   233,   276,   533,
     402,   218,    55,   366,   370,   478,   479,   209,   407,   452,
     379,    10,   137,     9,   130,   489,   482,   171,   172,   139,
     234,   380,   491,    10,   409,   277,   218,   381,   142,   126,
     392,   243,   141,   427,   171,   172,   143,   406,   155,    11,
     410,   149,   156,    49,   150,   235,    12,   330,   382,    13,
     157,   248,   171,   172,   331,    49,   171,   172,   158,  -138,
     269,   270,   159,   332,    33,   524,    34,   516,   321,   290,
     291,   195,   162,   287,   314,   270,   450,   163,   218,   264,
     167,   195,    33,   164,    34,   168,   333,   170,   458,   525,
     347,   348,   356,   357,   180,    78,    79,   448,   183,   171,
     172,   399,   400,   192,   264,    82,   199,   264,   426,   348,
     204,   173,    83,    84,   229,    85,   434,   435,   480,   270,
      86,   481,   270,   494,   348,   558,   505,   270,   219,    88,
     248,   506,   270,   563,   513,   348,   264,   239,   493,   514,
     400,   532,   357,    89,   254,   195,   195,   548,   357,   564,
     270,   370,    92,   566,   270,   268,   288,   503,   297,   311,
     298,    33,    73,    34,    74,   320,   322,   323,   325,   342,
     335,    94,   340,   345,   352,   353,   512,   354,    75,    76,
     261,    95,   371,   503,   355,   401,   394,    97,    98,   404,
     408,   411,    78,    79,   414,   412,   413,    99,   416,   287,
      80,    81,    82,   100,   417,   419,   511,   422,   503,    83,
      84,   420,    85,   423,   424,   186,   262,    86,   429,   430,
     442,   444,   441,    87,   287,   445,    88,   447,   451,   457,
     455,   459,   460,   465,   472,   538,   466,   469,   486,   487,
      89,    90,   496,   497,   498,   499,    91,   547,   483,    92,
     490,   504,    93,   348,   508,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   195,   171,   172,    94,   509,
     515,   518,   523,   195,   531,   541,   519,   537,    95,   543,
     544,    96,   545,   546,    97,    98,    33,    73,    34,    74,
     549,   550,   210,   560,    99,   557,   559,   567,   561,   562,
     100,   568,   569,    75,    76,   101,   263,   449,   349,   128,
     432,   198,   191,   129,   464,   539,   250,    78,    79,   517,
     565,   540,   387,   293,   309,    80,    81,    82,   388,   252,
     185,   226,   253,   317,    83,    84,   495,    85,     0,   318,
     237,     0,    86,     0,     0,     0,     0,   211,    87,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,     0,     0,     0,
       0,    91,     0,     0,    92,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,    33,    73,    34,    74,     0,
      10,   210,     0,    95,     0,     0,    96,     0,     0,    97,
      98,     0,    75,    76,     0,     0,     0,     0,     0,    99,
       0,     0,     0,     0,     0,   100,    78,    79,     0,     0,
     212,     0,     0,     0,    80,    81,    82,     0,     0,     0,
       0,     0,     0,    83,    84,     0,    85,     0,     0,     0,
       0,    86,     0,     0,     0,     0,   211,    87,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,     0,     0,     0,     0,
      91,     0,     0,    92,     0,     0,    93,     0,     0,     0,
       0,     0,    33,    73,    34,    74,     0,     0,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    95,     0,     0,    96,     0,     0,    97,    98,
       0,     0,     0,    78,    79,     0,     0,     0,    99,     0,
       0,    80,    81,    82,   100,     0,     0,     0,     0,   212,
      83,    84,     0,    85,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,    87,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,     0,     0,     0,     0,    91,     0,     0,
      92,     0,     0,    93,     0,     0,     0,     0,     0,    33,
      73,    34,    74,     0,     0,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,    75,    76,     0,    95,
       0,     0,    96,     0,     0,    97,    98,     0,     0,     0,
      78,    79,     0,     0,     0,    99,     0,     0,    80,    81,
      82,   100,     0,     0,     0,     0,   101,    83,    84,     0,
      85,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,    87,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
       0,     0,     0,     0,    91,     0,     0,    92,     0,     0,
      93,     0,     0,     0,     0,     0,    33,    73,    34,    74,
       0,     0,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,    75,    76,     0,    95,     0,     0,    96,
       0,     0,    97,    98,     0,     0,     0,    78,    79,     0,
       0,     0,    99,   151,     0,    80,    81,    82,   100,     0,
       0,     0,     0,   101,    83,    84,     0,    85,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,    87,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,     0,     0,     0,
       0,    91,     0,     0,    92,     0,     0,    93,     0,     0,
       0,     0,     0,    33,    73,    34,    74,     0,     0,     0,
       0,     0,   160,    94,     0,     0,     0,     0,     0,     0,
      75,    76,     0,    95,     0,     0,    96,     0,     0,    97,
      98,     0,     0,     0,    78,    79,     0,     0,     0,    99,
       0,     0,    80,    81,    82,   100,     0,     0,     0,     0,
     101,    83,    84,     0,    85,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,    87,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,     0,     0,     0,     0,    91,     0,
       0,    92,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,    33,    73,    34,    74,     0,    10,     0,     0,
      95,     0,     0,    96,     0,     0,    97,    98,     0,    75,
      76,     0,     0,     0,     0,     0,    99,     0,     0,     0,
       0,     0,   100,    78,    79,     0,     0,   101,     0,     0,
       0,    80,    81,    82,     0,     0,     0,     0,     0,     0,
      83,    84,     0,    85,     0,     0,     0,     0,    86,     0,
       0,     0,     0,   211,    87,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,     0,     0,     0,     0,    91,     0,     0,
      92,     0,     0,    93,     0,     0,     0,     0,     0,    33,
      73,    34,    74,     0,     0,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,    75,   184,     0,    95,
       0,     0,    96,     0,     0,    97,    98,     0,     0,     0,
      78,    79,     0,     0,     0,    99,     0,     0,    80,    81,
      82,   100,     0,     0,     0,     0,   212,    83,    84,     0,
      85,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,    87,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
       0,     0,     0,     0,    91,     0,     0,    92,     0,     0,
      93,     0,     0,     0,     0,     0,    33,    73,    34,    74,
       0,     0,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,    75,    76,     0,    95,     0,     0,    96,
       0,     0,    97,    98,     0,     0,     0,    78,    79,     0,
       0,     0,    99,     0,     0,    80,    81,    82,   100,     0,
       0,     0,     0,   101,    83,    84,     0,    85,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,    87,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    73,     0,    74,
       0,    91,     0,     0,    92,     0,     0,    93,     0,     0,
       0,     0,     0,    75,   184,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,     0,    78,    79,     0,
       0,     0,     0,    95,     0,     0,    96,    82,     0,    97,
      98,     0,     0,     0,    83,    84,     0,    85,     0,    99,
       0,     0,    86,     0,     0,   100,     0,     0,     0,     0,
     101,    88,     0,     0,     0,     0,     0,     0,    78,    79,
       0,     0,     0,     0,     0,    89,    90,     0,    82,     0,
       0,     0,     0,     0,    92,    83,    84,    93,    85,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    94,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    89,   254,     0,    97,
      98,     0,     0,     0,     0,    92,     0,     0,     0,    99,
       0,     0,     0,     0,     0,   100,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
      97,    98,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,     0,     0,     0,   100
};

static const yytype_int16 yycheck[] =
{
       4,   151,   219,   147,   143,    12,   168,    11,    12,    44,
     180,   290,   151,     4,   262,     6,     4,     5,     6,     7,
      29,   124,     4,     4,     6,     6,    67,     5,    32,     8,
      29,     8,    27,    37,    38,    39,    40,    41,     7,   209,
      28,    39,     4,     8,     6,    80,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    91,    21,    22,    59,
       8,   242,   212,    23,    24,   168,   101,    21,    22,     7,
      71,   210,    72,   212,   200,    28,    21,    22,    36,    67,
     242,    31,    51,   247,    21,    22,    80,   109,    21,    22,
     144,    76,   110,   147,   116,    29,    10,    37,   139,   125,
      40,    41,    73,   107,   118,    68,    59,   169,   211,    97,
      98,    85,    62,    51,    63,    79,   123,    63,   122,   123,
     155,   125,   136,   127,   109,    88,   188,   162,    47,    48,
     129,   125,   150,   131,   148,   170,   118,   141,   142,   242,
     149,   125,   130,   147,   122,   123,   394,   182,   149,   288,
     148,   321,   187,   148,   335,   196,   118,   148,   297,   298,
     148,   378,   441,   132,   168,   291,   148,   148,   126,   173,
     149,   419,   149,   335,   422,   339,   180,   212,   126,   183,
     120,    95,   186,   400,   149,   149,     1,   222,     3,   138,
     225,   151,   138,   126,   132,   149,   200,   197,   202,   203,
     204,   138,   352,   451,   149,    10,   309,   141,    73,    47,
      48,   355,   139,   352,     4,     0,     6,    32,    46,    21,
      22,   365,    49,   150,     4,     3,     6,   262,   144,    74,
     146,    30,   335,   268,    49,    50,    81,    54,   242,   243,
     457,    46,    57,   247,    72,    90,     9,    62,    17,    18,
      19,    56,   414,    33,   483,    82,     0,   119,    21,    22,
       4,    78,   266,   150,   129,   148,   270,    82,   113,    33,
     420,    73,   118,   308,    85,   310,    93,    94,   106,   508,
     315,   420,    26,   287,   288,   429,   430,   291,   323,   392,
      95,   118,   141,   108,   107,   445,   435,    21,    22,     5,
     117,   106,     9,   118,   149,   133,   445,   112,   148,    53,
      79,   414,    98,   348,    21,    22,   139,   321,   148,   134,
     327,     7,   148,   327,     7,   142,   141,    74,   133,   144,
      84,   335,    21,    22,    81,   339,    21,    22,   148,    86,
     149,   150,     5,    90,     4,    34,     6,   497,    33,   149,
     150,   355,   148,   357,   149,   150,   391,   148,   497,   394,
       5,   365,     4,    84,     6,    73,   113,   150,   403,    58,
     149,   150,   149,   150,    61,    35,    36,   381,   101,    21,
      22,   149,   150,   148,   419,    45,     7,   422,   149,   150,
     148,    33,    52,    53,     7,    55,   149,   150,   149,   150,
      60,   149,   150,   149,   150,   549,   149,   150,   148,    69,
     414,   149,   150,   557,   149,   150,   451,     7,   453,   149,
     150,   149,   150,    83,    84,   429,   430,   149,   150,   149,
     150,   435,    92,   149,   150,    11,   148,   472,     8,    20,
       9,     4,     5,     6,     7,    64,   149,    73,   149,   102,
     150,   111,    75,   149,   148,    87,   491,    87,    21,    22,
      23,   121,   135,   498,   148,   136,   148,   127,   128,    64,
       7,   100,    35,    36,    86,   100,   100,   137,     7,   483,
      43,    44,    45,   143,     7,    42,   490,    42,   523,    52,
      53,    77,    55,   149,    99,    27,    59,    60,   148,   148,
     141,    95,   150,    66,   508,   148,    69,    87,   148,   148,
      95,   149,   149,   104,    42,   519,   104,    99,    38,   102,
      83,    84,     7,    98,    42,    91,    89,   531,   148,    92,
     148,    19,    95,   150,   148,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,   549,    21,    22,   111,    42,
     149,     7,   150,   557,   112,    96,   140,   149,   121,   105,
     114,   124,   105,     9,   127,   128,     4,     5,     6,     7,
     148,    33,    10,    70,   137,   148,   148,   103,   114,    70,
     143,   149,     7,    21,    22,   148,   149,   384,   266,    55,
     357,   129,   123,    56,   414,   521,   170,    35,    36,   498,
     559,   523,   297,   210,    79,    43,    44,    45,   298,   174,
     120,   153,   179,   223,    52,    53,   457,    55,    -1,   223,
     159,    -1,    60,    -1,    -1,    -1,    -1,    65,    66,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,    -1,     4,     5,     6,     7,    -1,
     118,    10,    -1,   121,    -1,    -1,   124,    -1,    -1,   127,
     128,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,   137,
      -1,    -1,    -1,    -1,    -1,   143,    35,    36,    -1,    -1,
     148,    -1,    -1,    -1,    43,    44,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    53,    -1,    55,    -1,    -1,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    65,    66,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,     4,     5,     6,     7,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    23,   121,    -1,    -1,   124,    -1,    -1,   127,   128,
      -1,    -1,    -1,    35,    36,    -1,    -1,    -1,   137,    -1,
      -1,    43,    44,    45,   143,    -1,    -1,    -1,    -1,   148,
      52,    53,    -1,    55,    -1,    -1,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    84,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,     4,
       5,     6,     7,    -1,    -1,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,   121,
      -1,    -1,   124,    -1,    -1,   127,   128,    -1,    -1,    -1,
      35,    36,    -1,    -1,    -1,   137,    -1,    -1,    43,    44,
      45,   143,    -1,    -1,    -1,    -1,   148,    52,    53,    -1,
      55,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    92,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,     4,     5,     6,     7,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,   121,    -1,    -1,   124,
      -1,    -1,   127,   128,    -1,    -1,    -1,    35,    36,    -1,
      -1,    -1,   137,   138,    -1,    43,    44,    45,   143,    -1,
      -1,    -1,    -1,   148,    52,    53,    -1,    55,    -1,    -1,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,     4,     5,     6,     7,    -1,    -1,    -1,
      -1,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,   121,    -1,    -1,   124,    -1,    -1,   127,
     128,    -1,    -1,    -1,    35,    36,    -1,    -1,    -1,   137,
      -1,    -1,    43,    44,    45,   143,    -1,    -1,    -1,    -1,
     148,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,     4,     5,     6,     7,    -1,   118,    -1,    -1,
     121,    -1,    -1,   124,    -1,    -1,   127,   128,    -1,    21,
      22,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
      -1,    -1,   143,    35,    36,    -1,    -1,   148,    -1,    -1,
      -1,    43,    44,    45,    -1,    -1,    -1,    -1,    -1,    -1,
      52,    53,    -1,    55,    -1,    -1,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    65,    66,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    84,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,     4,
       5,     6,     7,    -1,    -1,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,   121,
      -1,    -1,   124,    -1,    -1,   127,   128,    -1,    -1,    -1,
      35,    36,    -1,    -1,    -1,   137,    -1,    -1,    43,    44,
      45,   143,    -1,    -1,    -1,    -1,   148,    52,    53,    -1,
      55,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    92,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,     4,     5,     6,     7,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,   121,    -1,    -1,   124,
      -1,    -1,   127,   128,    -1,    -1,    -1,    35,    36,    -1,
      -1,    -1,   137,    -1,    -1,    43,    44,    45,   143,    -1,
      -1,    -1,    -1,   148,    52,    53,    -1,    55,    -1,    -1,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    84,     5,    -1,     7,
      -1,    89,    -1,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,    -1,    -1,    -1,    35,    36,    -1,
      -1,    -1,    -1,   121,    -1,    -1,   124,    45,    -1,   127,
     128,    -1,    -1,    -1,    52,    53,    -1,    55,    -1,   137,
      -1,    -1,    60,    -1,    -1,   143,    -1,    -1,    -1,    -1,
     148,    69,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,
      -1,    -1,    -1,    -1,    -1,    83,    84,    -1,    45,    -1,
      -1,    -1,    -1,    -1,    92,    52,    53,    95,    55,    -1,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    83,    84,    -1,   127,
     128,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,   137,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
     127,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,    -1,    -1,   143
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,     3,    32,    49,    50,    57,    62,    82,   108,
     118,   134,   141,   144,   153,   154,   155,   156,   157,   158,
     159,   181,   182,   185,   186,   189,   191,   194,   195,   196,
     263,   264,   125,     4,     6,   194,   261,    80,   125,    73,
     125,    85,    28,    59,   197,   261,   192,   193,   208,   261,
       0,   144,   146,    49,    82,   194,    29,   141,   190,    30,
     144,   147,     3,   261,   129,   183,    73,   183,   261,   261,
     261,   261,   261,     5,     7,    21,    22,    23,    35,    36,
      43,    44,    45,    52,    53,    55,    60,    66,    69,    83,
      84,    89,    92,    95,   111,   121,   124,   127,   128,   137,
     143,   148,   162,   198,   199,   200,   202,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   251,   254,
     257,   261,   119,   150,    33,   148,   194,    85,   190,   197,
     107,   196,    31,    62,     5,   122,   123,   141,   184,     5,
     184,    98,   148,   139,   228,   229,   136,   148,   196,     7,
       7,   138,   234,   247,   248,   148,   148,    84,   148,     5,
     110,   234,   148,   148,    84,   194,   234,     5,    73,   201,
     150,    21,    22,    33,   258,   261,    23,    24,   151,   259,
      61,   110,   150,   101,    22,   237,    27,   148,   187,   188,
     261,   193,   148,   202,   260,   261,   183,   261,   195,     7,
      47,    48,    47,    48,   148,   184,   261,   160,   161,   261,
      10,    65,   148,   230,   231,   232,   233,   234,   254,   148,
     260,   230,   138,   245,   246,    63,   248,   249,   234,     7,
      54,    78,    93,    94,   117,   142,   252,   252,   234,     7,
     149,   149,   148,   202,   205,   206,   209,   240,   261,   228,
     200,   261,   235,   236,    84,   162,   261,   234,   148,   261,
     261,    23,    59,   149,   234,   250,   150,   228,    11,   149,
     150,   184,   196,   160,    46,    72,   106,   133,   166,   261,
     261,   173,   174,   175,   176,   177,   178,   261,   148,   169,
     149,   150,   162,   233,   202,   230,   234,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    79,
     256,    20,   251,   253,   149,   126,   234,   246,   249,   234,
      64,    33,   149,    73,    73,   149,   205,    33,   207,   208,
      74,    81,    90,   113,   204,   150,   207,    39,   131,   203,
      75,   210,   102,   217,   218,   149,   250,   149,   150,   188,
     234,   261,   148,    87,    87,   148,   149,   150,     5,     7,
      28,    67,    97,    98,   130,   148,   261,   262,   254,   255,
     261,   135,   166,   167,   168,   160,    10,    46,    56,    95,
     106,   112,   133,   163,   164,   165,   149,   231,   232,    17,
      18,    19,    79,   234,   148,   202,   234,    10,    95,   149,
     150,   136,   234,   126,    64,   162,   261,   234,     7,   149,
     208,   100,   100,   100,    86,   205,     7,     7,   207,    42,
      77,   211,    42,   149,    99,   219,   149,   234,   230,   148,
     148,   260,   174,   260,   149,   150,    37,    40,    41,   120,
     179,   150,   141,   170,    95,   148,   251,    87,   261,   163,
     234,   148,   202,     9,   250,    95,   251,   148,   234,   149,
     149,   149,    71,   149,   206,   104,   104,   250,   230,    99,
     212,   250,    42,   109,   116,   220,   221,   149,   260,   260,
     149,   149,   254,   148,   180,   166,    38,   102,   171,   230,
     148,     9,   250,   234,   149,   253,     7,    98,    42,    91,
     213,   224,   225,   234,    19,   149,   149,   173,   148,    42,
     149,   261,   234,   149,   149,   149,   230,   224,     7,   140,
     214,   215,   216,   150,    34,    58,   226,     7,    51,   132,
     222,   112,   149,   173,    76,   109,   172,   149,   261,   216,
     225,    96,   227,   105,   114,   105,     9,   261,   149,   148,
      33,    68,    88,     7,    51,   132,   223,   148,   260,   148,
      70,   114,    70,   260,   149,   217,   149,   103,   149,     7
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   152,   153,   153,   153,   153,   153,   153,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   155,   156,
     156,   156,   156,   157,   158,   159,   160,   161,   161,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   163,   163,   163,
     163,   163,   163,   163,   164,   164,   165,   165,   166,   166,
     166,   166,   167,   167,   168,   168,   169,   169,   170,   170,
     171,   171,   172,   172,   173,   173,   174,   174,   174,   174,
     175,   175,   175,   176,   177,   178,   179,   179,   179,   179,
     180,   180,   181,   181,   181,   181,   182,   182,   182,   182,
     183,   183,   183,   184,   184,   185,   186,   187,   187,   188,
     189,   189,   190,   190,   191,   192,   192,   193,   194,   194,
     195,   195,   196,   197,   197,   197,   198,   198,   199,   199,
     200,   200,   200,   201,   202,   203,   203,   203,   204,   204,
     204,   204,   204,   204,   204,   204,   205,   205,   206,   206,
     206,   206,   206,   206,   207,   207,   208,   208,   209,   209,
     210,   210,   211,   211,   212,   212,   213,   213,   214,   214,
     215,   215,   216,   217,   218,   218,   219,   219,   220,   220,
     221,   221,   222,   222,   222,   223,   223,   223,   224,   224,
     225,   226,   226,   226,   227,   227,   227,   228,   228,   229,
     230,   230,   231,   231,   232,   232,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   234,   234,   235,
     235,   236,   236,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   238,   238,   239,   239,   240,
     240,   240,   240,   241,   241,   241,   241,   242,   243,   243,
     244,   244,   245,   245,   246,   247,   247,   248,   249,   249,
     250,   250,   251,   251,   251,   251,   251,   251,   251,   251,
     252,   252,   252,   252,   252,   252,   253,   253,   254,   254,
     255,   255,   256,   256,   256,   256,   256,   256,   256,   256,
     256,   256,   257,   258,   258,   259,   259,   259,   260,   260,
     261,   261,   262,   262,   262,   262,   263,   264,   264
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
       1,     2,     1,     1,     1,     1,     1,     3,     5,     1,
       1,     1,     1,     3,     1,     2,     2,     3,     2,     3,
       4,     4,     5,     6,     6,     3,     3,     6,     6,     8,
       5,     4,     1,     2,     4,     1,     2,     4,     0,     2,
       1,     3,     1,     1,     2,     2,     1,     2,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     2,     2,     0
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
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2063 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_NAME  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2073 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2083 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2093 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 7: /* TOKEN_UNSIGNED_NUMVAL  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2103 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* sql_statement  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2113 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* quit_statement  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 2123 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* alter_table_statement  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2133 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* create_table_statement  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 2143 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* create_index_statement  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2153 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* drop_table_statement  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 2163 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* column_def  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 2173 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* column_def_commalist  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 2183 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* data_type  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 2193 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 163: /* column_constraint_def  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 2203 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 164: /* column_constraint_def_list  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2213 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 165: /* opt_column_constraint_def_list  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2223 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* opt_column_list  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2233 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* opt_block_properties  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).block_properties_) != nullptr) {
    delete ((*yyvaluep).block_properties_);
  }
}
#line 2243 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* opt_partition_clause  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).partition_clause_) != nullptr) {
    delete ((*yyvaluep).partition_clause_);
  }
}
#line 2253 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* partition_type  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2263 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* key_value_list  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2273 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* key_value  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 2283 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* key_string_value  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 2293 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* key_string_list  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 2303 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* key_integer_value  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 2313 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* key_bool_value  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_bool_value_) != nullptr) {
    delete ((*yyvaluep).key_bool_value_);
  }
}
#line 2323 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* index_type  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2333 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* opt_index_properties  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2343 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* insert_statement  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 2353 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* copy_statement  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_statement_) != nullptr) {
    delete ((*yyvaluep).copy_statement_);
  }
}
#line 2363 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* copy_to_target  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2373 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* opt_copy_params  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2383 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* update_statement  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2393 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* delete_statement  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2403 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* assignment_list  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2413 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* assignment_item  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2423 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* set_operation_statement  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_statement_) != nullptr) {
    delete ((*yyvaluep).set_operation_statement_);
  }
}
#line 2433 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 190: /* opt_priority_clause  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_priority_clause_) != nullptr) {
    delete ((*yyvaluep).opt_priority_clause_);
  }
}
#line 2443 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 191: /* with_clause  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2453 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 192: /* with_list  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2463 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 193: /* with_list_element  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2473 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 194: /* set_operation_union  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_) != nullptr) {
    delete ((*yyvaluep).set_operation_);
  }
}
#line 2483 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 195: /* set_operation_intersect  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_) != nullptr) {
    delete ((*yyvaluep).set_operation_);
  }
}
#line 2493 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 196: /* select_query  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2503 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 197: /* opt_all_distinct  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2509 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 198: /* selection  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2519 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 199: /* selection_item_commalist  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2529 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 200: /* selection_item  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2539 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 201: /* from_clause  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2549 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 202: /* subquery_expression  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2559 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 203: /* opt_sample_clause  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_sample_clause_) != nullptr) {
    delete ((*yyvaluep).opt_sample_clause_);
  }
}
#line 2569 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 204: /* join_type  */
#line 632 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2575 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 205: /* joined_table_reference  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2585 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 206: /* table_reference  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2595 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 207: /* table_reference_signature  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2605 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 208: /* table_reference_signature_primary  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2615 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 209: /* joined_table_reference_commalist  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2625 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 210: /* opt_group_by_clause  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2635 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 211: /* opt_having_clause  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2645 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 212: /* opt_order_by_clause  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2655 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 213: /* opt_limit_clause  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2665 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 214: /* opt_window_clause  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_window_clause_) != nullptr) {
    delete ((*yyvaluep).opt_window_clause_);
  }
}
#line 2675 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 215: /* window_declaration_list  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_window_clause_) != nullptr) {
    delete ((*yyvaluep).opt_window_clause_);
  }
}
#line 2685 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 216: /* window_declaration  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_definition_) != nullptr) {
    delete ((*yyvaluep).window_definition_);
  }
}
#line 2695 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 217: /* window_definition  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_definition_) != nullptr) {
    delete ((*yyvaluep).window_definition_);
  }
}
#line 2705 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 218: /* opt_window_partition  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_partition_by_list_) != nullptr) {
    delete ((*yyvaluep).window_partition_by_list_);
  }
}
#line 2715 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 219: /* opt_window_order  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_order_by_list_) != nullptr) {
    delete ((*yyvaluep).window_order_by_list_);
  }
}
#line 2725 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 220: /* opt_window_frame  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_frame_info_) != nullptr) {
    delete ((*yyvaluep).window_frame_info_);
  }
}
#line 2735 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 221: /* frame_mode  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2741 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 222: /* frame_preceding  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2751 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 223: /* frame_following  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2761 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 224: /* order_commalist  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2771 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 225: /* order_item  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2781 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 226: /* opt_order_direction  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2791 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 227: /* opt_nulls_first  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2801 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 228: /* opt_where_clause  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2811 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 229: /* where_clause  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2821 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 230: /* or_expression  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2831 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 231: /* and_expression  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2841 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 232: /* not_expression  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2851 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 233: /* predicate_expression_base  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2861 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 234: /* add_expression  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2871 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 235: /* multiply_expression  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2881 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 236: /* unary_expression  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2891 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 237: /* expression_base  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2901 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 238: /* array_expression  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).array_expression_) != nullptr) {
    delete ((*yyvaluep).array_expression_);
  }
}
#line 2911 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 239: /* array_element_commalist  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).array_expression_) != nullptr) {
    delete ((*yyvaluep).array_expression_);
  }
}
#line 2921 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 240: /* function_call  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2931 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 241: /* cast_function  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2941 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 242: /* extract_function  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2951 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 243: /* substr_function  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2961 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 244: /* case_expression  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2971 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 245: /* simple_when_clause_list  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_list_);
  }
}
#line 2981 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 246: /* simple_when_clause  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_);
  }
}
#line 2991 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 247: /* searched_when_clause_list  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_list_);
  }
}
#line 3001 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 248: /* searched_when_clause  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_);
  }
}
#line 3011 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 249: /* opt_else_clause  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 3021 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 250: /* expression_list  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 3031 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 251: /* literal_value  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 3041 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 252: /* datetime_unit  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 3051 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 253: /* literal_value_commalist  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 3061 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 254: /* attribute_ref  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 3071 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 255: /* attribute_ref_list  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 3081 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 256: /* comparison_operation  */
#line 631 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 3087 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 257: /* unary_operation  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).unary_operation_) != nullptr) {
    delete ((*yyvaluep).unary_operation_);
  }
}
#line 3097 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 258: /* add_operation  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).binary_operation_) != nullptr) {
    delete ((*yyvaluep).binary_operation_);
  }
}
#line 3107 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 259: /* multiply_operation  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).binary_operation_) != nullptr) {
    delete ((*yyvaluep).binary_operation_);
  }
}
#line 3117 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 260: /* name_commalist  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 3127 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 261: /* any_name  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 3137 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 262: /* boolean_value  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 3143 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 263: /* command  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_) != nullptr) {
    delete ((*yyvaluep).command_);
  }
}
#line 3153 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 264: /* command_argument_list  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_argument_list_) != nullptr) {
    delete ((*yyvaluep).command_argument_list_);
  }
}
#line 3163 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 643 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3460 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 647 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3469 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 651 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3478 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 5:
#line 655 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3487 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 659 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    YYABORT;
  }
#line 3495 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 662 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 3504 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 669 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3512 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 9:
#line 672 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].copy_statement_);
  }
#line 3520 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 10:
#line 675 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 3528 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 11:
#line 678 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3536 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 12:
#line 681 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 3544 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 13:
#line 684 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 3552 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 14:
#line 687 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 3560 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 15:
#line 690 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 3568 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 16:
#line 693 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].set_operation_statement_);
  }
#line 3576 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 17:
#line 696 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 3584 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 18:
#line 702 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3592 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 19:
#line 708 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3604 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 20:
#line 715 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3615 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 721 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3627 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 728 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3639 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 737 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].attribute_definition_list_), (yyvsp[-1].block_properties_), (yyvsp[0].partition_clause_));
  }
#line 3647 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 742 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].key_value_list_)) {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_), (yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].key_value_list_));
    } else {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_));
    }
  }
#line 3659 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 751 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 3667 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 756 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 3675 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 761 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3684 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 765 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3693 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 771 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 3703 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 776 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDate));
  }
#line 3711 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 779 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3719 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 782 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3729 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 787 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3737 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 790 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3745 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 793 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3753 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 796 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3761 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 799 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3769 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 802 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3777 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 805 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3785 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 808 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3793 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 811 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3801 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 814 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3818 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 826 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3826 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 829 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3834 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 832 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3857 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 850 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3880 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 870 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3888 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 873 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3896 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 876 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3906 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 881 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3916 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 886 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3927 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 892 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3938 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 898 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3950 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 907 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3959 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 911 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3968 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 917 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3976 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 920 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3984 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 925 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3994 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 930 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 4004 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 935 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 4016 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 942 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 4026 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 949 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 4035 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 953 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 4044 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 959 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 4052 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 962 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = $1; */
  }
#line 4060 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 967 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = nullptr;
  }
#line 4068 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 970 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-1].attribute_list_);
  }
#line 4076 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 975 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = nullptr;
  }
#line 4084 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 978 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].key_value_list_));
  }
#line 4092 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 983 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.partition_clause_) = nullptr;
  }
#line 4100 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 986 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4122 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 1005 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::kHashPartitionType);
  }
#line 4130 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 1008 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::kRangePartitionType);
  }
#line 4138 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 1013 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 4147 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 1017 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 4156 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 1023 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 4164 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 1026 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 4172 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 1029 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 4180 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 1032 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_bool_value_);
  }
#line 4188 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 1037 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 4196 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 1040 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 4204 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 1043 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_),
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 4214 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 1050 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4222 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 1055 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 4236 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 1066 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_bool_value_) = new quickstep::ParseKeyBoolValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].boolean_value_));
  }
#line 4244 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 1071 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Defaults to BitWeavingV, but IndexProperties can change this to H.
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBitWeavingV));
  }
#line 4254 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 1076 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
#line 4263 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 1080 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
#line 4272 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 1084 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kSMA));
  }
#line 4281 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 1090 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 4289 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 1093 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 4297 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 1099 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 4310 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 1107 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertTuple((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 4318 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 1110 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-3]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), nullptr);
  }
#line 4326 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 1113 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-4]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), (yyvsp[-4].with_list_));
  }
#line 4334 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 1119 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-4]).first_line, (yylsp[-4]).first_column,
                                           quickstep::ParseStatementCopy::kFrom,
                                           (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4344 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 1124 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-3]).first_line, (yylsp[-3]).first_column,
                                           quickstep::ParseStatementCopy::kTo,
                                           (yyvsp[-2].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4354 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 1129 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-3]).first_line, (yylsp[-3]).first_column,
                                           (yyvsp[-2].set_operation_), nullptr, (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4363 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 1133 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-4]).first_line, (yylsp[-4]).first_column,
                                           (yyvsp[-2].set_operation_), (yyvsp[-4].with_list_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4372 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 1139 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yyvsp[0].string_value_)->line_number(), (yyvsp[0].string_value_)->column_number(), "@" + (yyvsp[0].string_value_)->value());
    delete (yyvsp[0].string_value_);
  }
#line 4381 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 1143 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "$stdout");
  }
#line 4389 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 1146 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "$stderr");
  }
#line 4397 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 1151 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 4405 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 1154 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 4413 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 1159 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 4421 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 1164 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 4429 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 1169 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 4438 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 1173 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 4447 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 1179 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 4455 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 1188 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_statement_) = new quickstep::ParseStatementSetOperation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].set_operation_), nullptr, (yyvsp[0].opt_priority_clause_));
  }
#line 4463 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 1191 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_statement_) = new quickstep::ParseStatementSetOperation((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].set_operation_), (yyvsp[-2].with_list_), (yyvsp[0].opt_priority_clause_));
  }
#line 4471 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 1196 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_priority_clause_) = nullptr;
  }
#line 4479 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 1199 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4501 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 1218 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 4509 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 1223 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4518 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 1227 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4527 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 1233 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 4536 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 1239 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-1].boolean_value_)) {
      (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-3]).first_line, (yylsp[-3]).first_column, quickstep::ParseSetOperation::kUnion);
    } else {
      (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-3]).first_line, (yylsp[-3]).first_column, quickstep::ParseSetOperation::kUnionAll);
    }
    (yyval.set_operation_)->addOperand((yyvsp[-3].set_operation_));
    (yyval.set_operation_)->addOperand((yyvsp[0].set_operation_));
  }
#line 4550 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 1248 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_) = (yyvsp[0].set_operation_);
  }
#line 4558 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 1253 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-2]).first_line, (yylsp[-2]).first_column, quickstep::ParseSetOperation::kIntersect);
    quickstep::ParseSetOperation *op = new quickstep::ParseSetOperation(
        (yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::ParseSetOperation::kSelect);
    op->addOperand((yyvsp[0].select_query_));
    (yyval.set_operation_)->addOperand((yyvsp[-2].set_operation_));
    (yyval.set_operation_)->addOperand(op);
  }
#line 4571 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 1261 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::ParseSetOperation::kSelect);
    (yyval.set_operation_)->addOperand((yyvsp[0].select_query_));
  }
#line 4580 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 1268 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-9]).first_line, (yylsp[-9]).first_column, (yyvsp[-7].selection_), (yyvsp[-6].table_reference_list_), (yyvsp[-5].predicate_), (yyvsp[-4].opt_group_by_clause_), (yyvsp[-3].opt_having_clause_), (yyvsp[-2].opt_order_by_clause_), (yyvsp[-1].opt_limit_clause_), (yyvsp[0].opt_window_clause_));
  }
#line 4588 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 1273 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;  // Distinct
  }
#line 4596 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 1276 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;  // All
  }
#line 4604 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 1279 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;  // Distinct
  }
#line 4612 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 1284 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4620 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 1287 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 4628 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 1292 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4637 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 1296 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4646 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 1302 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 4654 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 1305 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 4662 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 1308 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 4670 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 1313 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[0].table_reference_list_);
  }
#line 4678 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 1318 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].set_operation_));
  }
#line 4686 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 1323 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = NULL;
  }
#line 4694 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 1326 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, true, (yyvsp[-1].numeric_literal_value_));
  }
#line 4702 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 1329 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].numeric_literal_value_));
  }
#line 4710 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 1334 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4718 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 1337 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4726 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 1340 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4734 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 1343 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4742 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 1346 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4750 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 1349 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4758 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 1352 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4766 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 1355 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4774 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 1360 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseJoinedTableReference((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].join_type_), (yyvsp[-5].table_reference_), (yyvsp[-2].table_reference_), (yyvsp[0].predicate_));
  }
#line 4782 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 1363 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = (yyvsp[0].table_reference_);
  }
#line 4790 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 1368 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4799 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 1372 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].opt_sample_clause_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4808 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 1376 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].opt_sample_clause_));
  }
#line 4816 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 1379 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4825 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 1383 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].function_call_));
  }
#line 4833 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 1386 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = (yyvsp[-1].table_reference_);
  }
#line 4841 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 1391 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4849 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 1394 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4857 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 1399 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4865 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 1402 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4873 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 1407 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4882 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 1411 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4891 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 1417 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 4899 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 1420 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4907 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 1425 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4915 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 1428 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4923 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 1433 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4931 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 1436 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4939 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 1441 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4947 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 1444 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4969 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 1463 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = nullptr;
  }
#line 4977 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 1466 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = (yyvsp[0].opt_window_clause_);
  }
#line 4985 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 1471 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = new quickstep::PtrList<quickstep::ParseWindow>();
    (yyval.opt_window_clause_)->push_back((yyvsp[0].window_definition_));
  }
#line 4994 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 1475 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = (yyvsp[-1].opt_window_clause_);
    (yyval.opt_window_clause_)->push_back((yyvsp[0].window_definition_));
  }
#line 5003 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 1481 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_definition_) = (yyvsp[-1].window_definition_);
    (yyval.window_definition_)->setName((yyvsp[-4].string_value_));
  }
#line 5012 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 1487 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_definition_) = new quickstep::ParseWindow((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].window_partition_by_list_), (yyvsp[-1].window_order_by_list_), (yyvsp[0].window_frame_info_));
  }
#line 5020 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 1492 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_partition_by_list_) = nullptr;
  }
#line 5028 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 1495 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_partition_by_list_) = (yyvsp[0].expression_list_);
  }
#line 5036 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 1500 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_order_by_list_) = nullptr;
  }
#line 5044 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 1503 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_order_by_list_) = (yyvsp[0].order_commalist_);
  }
#line 5052 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 1508 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_frame_info_) = nullptr;
  }
#line 5060 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 1511 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_frame_info_) = new quickstep::ParseFrameInfo((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-4].boolean_value_), (yyvsp[-2].numeric_literal_value_)->long_value(), (yyvsp[0].numeric_literal_value_)->long_value());
  }
#line 5068 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 1516 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5076 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 1519 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5084 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 1525 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "-1");
  }
#line 5092 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 1528 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "0");
  }
#line 5100 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1534 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "-1");
  }
#line 5108 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 1537 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "0");
  }
#line 5116 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 1542 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 5125 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 1546 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 5134 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 1552 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 5144 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 1559 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 5152 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 1562 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 5160 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 1565 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 5168 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 1570 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 5176 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 1573 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 5184 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 1576 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 5192 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 1582 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 5200 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 1585 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5208 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 1590 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5216 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1595 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 5230 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1604 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5238 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1609 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 5252 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1618 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5260 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1623 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 5268 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1626 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5276 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1631 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5284 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1634 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 5294 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1639 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 5305 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1645 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 5316 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1651 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5324 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1654 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 5332 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1657 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateExists((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].subquery_expression_));
  }
#line 5340 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1660 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-2].expression_), (yyvsp[0].subquery_expression_));
  }
#line 5348 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1663 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-1].expression_list_));
  }
#line 5356 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1666 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-2]).first_line,
        (yylsp[-2]).first_column,
        new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-3].expression_), (yyvsp[0].subquery_expression_)));
  }
#line 5367 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1672 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line,
        (yylsp[-4]).first_column,
        new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-1].expression_list_)));
  }
#line 5378 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1681 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-2].expression_));
    arguments->push_back((yyvsp[0].expression_));
    (yyval.expression_) = new quickstep::ParseFunctionCall((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].binary_operation_), arguments);
  }
#line 5389 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1687 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5397 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1692 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-2].expression_));
    arguments->push_back((yyvsp[0].expression_));
    (yyval.expression_) = new quickstep::ParseFunctionCall((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].binary_operation_), arguments);
  }
#line 5408 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1698 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5416 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1703 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[0].expression_));
    (yyval.expression_) = new quickstep::ParseFunctionCall((yylsp[-1]).first_line, (yylsp[-1]).first_column, false, (yyvsp[-1].unary_operation_), arguments);
  }
#line 5426 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1708 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5434 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1713 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 5442 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1716 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 5450 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1719 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].array_expression_);
  }
#line 5458 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1722 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 5466 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1725 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyvsp[-2].function_call_)->setWindowName((yyvsp[0].string_value_));
    (yyval.expression_) = (yyvsp[-2].function_call_);
  }
#line 5475 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1729 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyvsp[-4].function_call_)->setWindow((yyvsp[-1].window_definition_));
    (yyval.expression_) = (yyvsp[-4].function_call_);
  }
#line 5484 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1733 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5492 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1736 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5500 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1739 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5508 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1742 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5516 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1745 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 5524 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1748 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].subquery_expression_);
  }
#line 5532 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1753 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.array_expression_) = (yyvsp[-1].array_expression_);
  }
#line 5540 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1756 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.array_expression_) = new quickstep::ParseArray((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 5548 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1762 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.array_expression_) = (yyvsp[-2].array_expression_);
    (yyval.array_expression_)->add((yyvsp[0].expression_));
  }
#line 5557 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1766 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.array_expression_) = new quickstep::ParseArray((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.array_expression_)->add((yyvsp[0].expression_));
  }
#line 5566 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1773 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 5575 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1777 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 5584 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1781 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, false, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 5592 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1784 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-4]).first_line, (yylsp[-4]).first_column, true, (yyvsp[-4].string_value_), (yyvsp[-1].expression_list_));
  }
#line 5600 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1789 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5619 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1803 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-3].expression_));
    arguments->push_back(new quickstep::ParseScalarLiteral(
        new quickstep::StringParseLiteralValue((yyvsp[-1].string_value_), nullptr)));
    auto *name = new quickstep::ParseString((yylsp[-5]).first_line, (yylsp[-5]).first_column, "cast");
    (yyval.expression_) = new quickstep::ParseFunctionCall(
        (yylsp[-5]).first_line, (yylsp[-5]).first_column, false, name, arguments);
  }
#line 5633 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1812 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-2].expression_));
    arguments->push_back(new quickstep::ParseScalarLiteral(
        new quickstep::StringParseLiteralValue(
            new quickstep::ParseString((yylsp[0]).first_line,
                                       (yylsp[0]).first_column,
                                       (yyvsp[0].data_type_)->getType().getName()),
            nullptr)));
    delete (yyvsp[0].data_type_);
    auto *name = new quickstep::ParseString((yylsp[-1]).first_line, (yylsp[-1]).first_column, "cast");
    (yyval.expression_) = new quickstep::ParseFunctionCall(
        (yylsp[-1]).first_line, (yylsp[-1]).first_column, false, name, arguments);
  }
#line 5652 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1826 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-2].expression_));
    arguments->push_back(new quickstep::ParseScalarLiteral(
        new quickstep::StringParseLiteralValue((yyvsp[0].string_value_), nullptr)));
    auto *name = new quickstep::ParseString((yylsp[-1]).first_line, (yylsp[-1]).first_column, "cast");
    (yyval.expression_) = new quickstep::ParseFunctionCall(
        (yylsp[-1]).first_line, (yylsp[-1]).first_column, false, name, arguments);
  }
#line 5666 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1837 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-1].expression_));
    arguments->push_back(new quickstep::ParseScalarLiteral(
        new quickstep::StringParseLiteralValue((yyvsp[-3].string_value_), nullptr)));
    auto *name = new quickstep::ParseString((yylsp[-5]).first_line, (yylsp[-5]).first_column, "extract");
    (yyval.expression_) = new quickstep::ParseFunctionCall(
        (yylsp[-5]).first_line, (yylsp[-5]).first_column, false, name, arguments);
  }
#line 5680 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1848 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-3].expression_));
    arguments->push_back(new quickstep::ParseScalarLiteral((yyvsp[-1].numeric_literal_value_)));
    auto *name = new quickstep::ParseString((yylsp[-5]).first_line, (yylsp[-5]).first_column, "substring");
    (yyval.expression_) = new quickstep::ParseFunctionCall(
        (yylsp[-5]).first_line, (yylsp[-5]).first_column, false, name, arguments);
  }
#line 5693 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1856 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-5].expression_));
    arguments->push_back(new quickstep::ParseScalarLiteral((yyvsp[-3].numeric_literal_value_)));
    arguments->push_back(new quickstep::ParseScalarLiteral((yyvsp[-1].numeric_literal_value_)));
    auto *name = new quickstep::ParseString((yylsp[-7]).first_line, (yylsp[-7]).first_column, "substring");
    (yyval.expression_) = new quickstep::ParseFunctionCall(
        (yylsp[-7]).first_line, (yylsp[-7]).first_column, false, name, arguments);
  }
#line 5707 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1867 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSimpleCaseExpression((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].expression_), (yyvsp[-2].simple_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 5715 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1870 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSearchedCaseExpression((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].searched_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 5723 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1875 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSimpleWhenClause>;
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 5732 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1879 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_list_) = (yyvsp[-1].simple_when_clause_list_);
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 5741 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1885 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_) = new quickstep::ParseSimpleWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5749 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1890 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSearchedWhenClause>;
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 5758 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1894 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_list_) = (yyvsp[-1].searched_when_clause_list_);
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 5767 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1900 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_) = new quickstep::ParseSearchedWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].predicate_), (yyvsp[0].expression_));
  }
#line 5775 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1905 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = NULL;
  }
#line 5783 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1908 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5791 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1913 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 5800 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1917 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 5809 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1923 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 5817 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1926 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5825 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1929 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5833 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1932 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5850 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1944 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 5859 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1948 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5883 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1967 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5901 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1980 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5919 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1995 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("YEAR"));
  }
#line 5927 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1998 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("MONTH"));
  }
#line 5935 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 2001 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("DAY"));
  }
#line 5943 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 2004 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("HOUR"));
  }
#line 5951 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 2007 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("MINUTE"));
  }
#line 5959 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 2010 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("SECOND"));
  }
#line 5967 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 2015 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5976 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 2019 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5985 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 2025 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 5993 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 2028 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 6001 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 2033 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = new quickstep::PtrList<quickstep::ParseAttribute>();
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 6010 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 2037 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-2].attribute_list_);
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 6019 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 2044 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 6027 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 2047 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 6035 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 2050 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 6043 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 2053 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 6051 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 2056 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 6059 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 2059 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 6067 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 2062 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLike);
  }
#line 6075 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 2065 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotLike);
  }
#line 6083 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 2068 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kRegexMatch);
  }
#line 6091 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 2071 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotRegexMatch);
  }
#line 6099 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 2076 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("-"));
  }
#line 6113 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 2087 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("+"));
  }
#line 6121 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 2090 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("-"));
  }
#line 6129 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 2095 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("%"));
  }
#line 6137 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 2098 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("*"));
  }
#line 6145 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 2101 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("/"));
  }
#line 6153 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 2107 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 6162 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 2111 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 6171 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 2117 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 6179 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 2120 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 6190 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 2128 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 6198 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 2131 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 6206 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 304:
#line 2134 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 6214 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 305:
#line 2137 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 6222 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 306:
#line 2143 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.command_) = new quickstep::ParseCommand((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].command_argument_list_));
  }
#line 6230 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 307:
#line 2148 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    quickstep::PtrVector<quickstep::ParseString> *argument_list = (yyvsp[-1].command_argument_list_);
    argument_list->push_back((yyvsp[0].string_value_));
    (yyval.command_argument_list_) = argument_list;
  }
#line 6240 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 308:
#line 2153 "../SqlParser.ypp" /* yacc.c:1661  */
    { /* Epsilon, an empy match. */
    (yyval.command_argument_list_) = new quickstep::PtrVector<quickstep::ParseString>();
  }
#line 6248 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;


#line 6252 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 2157 "../SqlParser.ypp" /* yacc.c:1906  */


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
