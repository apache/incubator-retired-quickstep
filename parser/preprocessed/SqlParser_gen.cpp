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
    TOKEN_LEFT = 339,
    TOKEN_LIMIT = 340,
    TOKEN_LONG = 341,
    TOKEN_MINUTE = 342,
    TOKEN_MONTH = 343,
    TOKEN_NULL = 344,
    TOKEN_NULLS = 345,
    TOKEN_OFF = 346,
    TOKEN_ON = 347,
    TOKEN_ORDER = 348,
    TOKEN_OUTER = 349,
    TOKEN_OVER = 350,
    TOKEN_PARTITION = 351,
    TOKEN_PARTITIONS = 352,
    TOKEN_PERCENT = 353,
    TOKEN_PRECEDING = 354,
    TOKEN_PRIMARY = 355,
    TOKEN_PRIORITY = 356,
    TOKEN_QUIT = 357,
    TOKEN_RANGE = 358,
    TOKEN_REAL = 359,
    TOKEN_REFERENCES = 360,
    TOKEN_RIGHT = 361,
    TOKEN_ROW = 362,
    TOKEN_ROW_DELIMITER = 363,
    TOKEN_ROWS = 364,
    TOKEN_SECOND = 365,
    TOKEN_SELECT = 366,
    TOKEN_SET = 367,
    TOKEN_SMA = 368,
    TOKEN_SMALLINT = 369,
    TOKEN_STDERR = 370,
    TOKEN_STDOUT = 371,
    TOKEN_SUBSTRING = 372,
    TOKEN_TABLE = 373,
    TOKEN_THEN = 374,
    TOKEN_TIME = 375,
    TOKEN_TIMESTAMP = 376,
    TOKEN_TO = 377,
    TOKEN_TRUE = 378,
    TOKEN_TUPLESAMPLE = 379,
    TOKEN_UNBOUNDED = 380,
    TOKEN_UNIQUE = 381,
    TOKEN_UPDATE = 382,
    TOKEN_USING = 383,
    TOKEN_VALUES = 384,
    TOKEN_VARCHAR = 385,
    TOKEN_WHEN = 386,
    TOKEN_WHERE = 387,
    TOKEN_WINDOW = 388,
    TOKEN_WITH = 389,
    TOKEN_YEAR = 390,
    TOKEN_YEARMONTH = 391,
    TOKEN_EOF = 392,
    TOKEN_LEX_ERROR = 393
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

#line 431 "SqlParser_gen.cpp" /* yacc.c:355  */
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

#line 466 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYLAST   1282

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  150
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  111
/* YYNRULES -- Number of rules.  */
#define YYNRULES  303
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  562

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   393

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     145,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   149,     2,     2,
     146,   147,    23,    21,   148,    22,    27,    24,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   144,
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
     140,   141,   142,   143
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   635,   635,   639,   643,   647,   651,   654,   661,   664,
     667,   670,   673,   676,   679,   682,   685,   688,   694,   700,
     707,   713,   720,   729,   734,   743,   748,   753,   757,   763,
     768,   771,   774,   779,   782,   785,   788,   791,   794,   797,
     800,   803,   806,   818,   821,   824,   842,   862,   865,   868,
     873,   878,   884,   890,   899,   903,   909,   912,   917,   922,
     927,   934,   941,   945,   951,   954,   959,   962,   967,   970,
     975,   978,   997,  1000,  1005,  1009,  1015,  1018,  1021,  1024,
    1029,  1032,  1035,  1042,  1047,  1058,  1063,  1068,  1072,  1076,
    1082,  1085,  1091,  1099,  1102,  1105,  1111,  1116,  1121,  1125,
    1131,  1135,  1138,  1143,  1146,  1151,  1156,  1161,  1165,  1171,
    1180,  1183,  1188,  1191,  1210,  1215,  1219,  1225,  1231,  1240,
    1245,  1253,  1259,  1265,  1268,  1271,  1276,  1279,  1284,  1288,
    1294,  1297,  1300,  1305,  1310,  1315,  1318,  1321,  1326,  1329,
    1332,  1335,  1338,  1341,  1344,  1347,  1352,  1355,  1360,  1364,
    1368,  1371,  1375,  1378,  1383,  1386,  1391,  1394,  1399,  1403,
    1409,  1412,  1417,  1420,  1425,  1428,  1433,  1436,  1455,  1458,
    1463,  1467,  1473,  1479,  1484,  1487,  1492,  1495,  1500,  1503,
    1508,  1511,  1516,  1517,  1520,  1525,  1526,  1529,  1534,  1538,
    1544,  1551,  1554,  1557,  1562,  1565,  1568,  1574,  1577,  1582,
    1587,  1596,  1601,  1610,  1615,  1618,  1623,  1626,  1631,  1637,
    1643,  1646,  1649,  1652,  1655,  1658,  1664,  1673,  1679,  1684,
    1690,  1695,  1700,  1705,  1708,  1711,  1714,  1718,  1722,  1725,
    1728,  1731,  1734,  1737,  1742,  1746,  1750,  1753,  1758,  1772,
    1781,  1795,  1806,  1817,  1825,  1836,  1839,  1844,  1848,  1854,
    1859,  1863,  1869,  1874,  1877,  1882,  1886,  1892,  1895,  1898,
    1901,  1913,  1917,  1936,  1949,  1964,  1967,  1970,  1973,  1976,
    1979,  1984,  1988,  1994,  1997,  2002,  2006,  2013,  2016,  2019,
    2022,  2025,  2028,  2031,  2034,  2037,  2040,  2045,  2056,  2059,
    2064,  2067,  2070,  2076,  2080,  2086,  2089,  2097,  2100,  2103,
    2106,  2112,  2117,  2122
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
  "TOKEN_JOIN", "TOKEN_KEY", "TOKEN_LAST", "TOKEN_LEFT", "TOKEN_LIMIT",
  "TOKEN_LONG", "TOKEN_MINUTE", "TOKEN_MONTH", "TOKEN_NULL", "TOKEN_NULLS",
  "TOKEN_OFF", "TOKEN_ON", "TOKEN_ORDER", "TOKEN_OUTER", "TOKEN_OVER",
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
     390,   391,   392,   393,    59,    10,    40,    41,    44,    37
};
# endif

#define YYPACT_NINF -370

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-370)))

#define YYTABLE_NINF -139

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     318,  -370,  -370,   -80,    69,   -19,    41,   -25,    43,  -370,
      42,   242,   242,  -370,   137,   148,  -370,  -370,  -370,  -370,
    -370,  -370,  -370,  -370,  -370,  -370,   173,    70,   114,  -370,
     132,   215,   242,  -370,  -370,    11,     5,   242,   242,   242,
     242,   242,  -370,  -370,   662,   111,    99,  -370,   233,   122,
    -370,  -370,  -370,   186,   221,    70,    42,   206,  -370,   186,
    -370,  -370,  -370,    74,    67,   175,   321,   175,   231,   195,
     205,  -370,   -34,  -370,  -370,   337,   342,  -370,  -370,  -370,
     730,   208,   228,  -370,   268,  -370,  -370,   244,  -370,  -370,
     353,  -370,  -370,  -370,  -370,   246,  -370,  -370,   247,   299,
     825,   386,   322,   255,  -370,  -370,   314,    27,  -370,   333,
     304,  -370,  -370,  -370,  -370,  -370,  -370,   988,    -5,   242,
     242,   259,   242,    11,   242,  -370,   186,   399,  -370,   155,
     236,  -370,  -370,  -370,   261,  -370,   175,  -370,   242,   242,
     567,  -370,  -370,   265,   242,  -370,  -370,  -370,   567,    25,
     -15,  -370,  1056,   405,  -370,   146,   146,  1056,   409,  -370,
       3,    32,  -370,    19,   205,  1056,  -370,  -370,   242,  1056,
    -370,  -370,  -370,  -370,  1056,   635,    23,   342,   333,   242,
     392,    60,  -370,   408,  -370,   186,  -370,   -68,  -370,   175,
     186,   114,  -370,   242,   165,   242,   242,   242,  -370,   274,
    -370,   112,   798,   893,   259,   499,   413,   415,  -370,  -370,
    1201,   402,  1141,   150,    33,  1056,   149,  -370,  1056,  -370,
     362,   220,   283,  -370,  -370,  -370,  -370,  -370,  -370,   358,
    -370,    46,   285,  -370,  -370,     7,   190,   248,  -370,   290,
     190,    38,   364,  -370,  -370,    27,  -370,  -370,  -370,  -370,
     332,  -370,  -370,   293,  1056,  -370,   279,   163,   242,  -370,
    1056,  -370,   242,  -370,  -370,  -370,   295,   355,   356,   300,
    -370,  -370,  -370,   169,  -370,  -370,  -370,  -370,  -370,    14,
     242,   316,   165,   242,   194,  -370,  -370,     4,    73,   567,
     567,   245,  -370,  -370,  -370,  -370,  -370,  -370,  -370,  -370,
    1056,   307,  1056,    53,  -370,   183,   320,  1056,    75,  -370,
     395,   279,  -370,   635,  -370,  1056,   449,  -370,   124,   242,
    -370,  -370,   363,  -370,   365,   366,   377,    19,  -370,   459,
     460,   190,   426,   393,   427,   325,   375,  -370,   185,  -370,
    1056,  -370,   279,  -370,   567,   328,   331,   242,  -370,   242,
    -370,  -370,  -370,  -370,  -370,  -370,  -370,   242,  -370,  -370,
    -370,   191,   452,   189,  -370,   334,   345,  -370,   387,   339,
    1141,  -370,   400,   242,  -370,  -370,   194,  -370,  -370,   415,
    -370,  -370,  -370,  1056,   343,   212,   825,  -370,   279,   394,
    -370,  -370,  1141,   344,   279,  1056,  -370,   346,   347,    34,
     -22,  -370,  -370,  -370,  -370,  -370,    19,   248,   388,   389,
    -370,  1056,   567,   397,  1056,  -370,   455,   135,  -370,   279,
      16,   242,   242,   209,  -370,   214,  -370,   242,  -370,  -370,
    -370,  -370,   352,   165,   461,   401,  -370,   567,  -370,  -370,
     354,  -370,   235,   825,  -370,  1056,   216,  -370,  -370,  1141,
     279,  -370,  -370,  -370,   501,  -370,   416,  -370,  -370,   367,
     413,   468,   422,   367,  1056,  -370,  -370,  -370,   497,  -370,
     218,   223,  -370,  -370,  -370,   242,  -370,  -370,   376,   481,
    -370,    31,   242,  1056,   225,   279,  -370,   230,   378,   567,
    1056,   517,   390,   381,  -370,   193,     9,   420,  -370,   234,
     242,     0,  -370,   379,   279,  -370,  -370,  -370,   413,   381,
    -370,   242,  -370,   390,  -370,  1056,  -370,  -370,   436,   428,
     424,   437,   531,   242,  -370,   237,  -370,  -370,   403,  -370,
     513,  -370,  -370,    39,  -370,  -370,  -370,  -370,    52,   404,
    -370,   242,   407,  -370,  -370,   477,   443,   478,  -370,   242,
     239,   332,  -370,  -370,  -370,   241,   454,   410,  -370,   551,
    -370,  -370
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     6,   303,     0,     0,     0,     0,     0,     0,    18,
     123,     0,     0,     7,     0,     0,    15,     8,    10,    11,
      13,    14,     9,    17,    12,    16,     0,   112,   119,   121,
       0,   301,     0,   295,   296,     0,     0,     0,     0,     0,
       0,     0,   124,   125,     0,     0,   114,   115,     0,   156,
       1,     3,     2,     0,     0,   112,   123,     0,   110,     0,
       5,     4,   302,     0,     0,   103,     0,   103,     0,     0,
     197,    25,     0,   261,   258,     0,   287,   126,    40,    29,
       0,     0,     0,    30,    31,    34,    36,     0,    37,    39,
       0,    41,   257,    35,    38,     0,    32,    33,     0,     0,
       0,     0,     0,   127,   128,   233,   132,   218,   220,   222,
     225,   228,   229,   230,   231,   224,   223,     0,   273,     0,
       0,     0,     0,     0,     0,   111,     0,     0,   120,     0,
       0,   100,   102,   101,     0,    98,   103,    97,     0,     0,
       0,   106,   198,     0,     0,    94,   259,   260,     0,     0,
     253,   250,     0,     0,    43,     0,   262,     0,     0,    44,
       0,     0,   264,     0,   197,     0,   288,   289,     0,     0,
     131,   291,   292,   290,     0,     0,     0,     0,   221,     0,
       0,   197,   108,     0,   116,     0,   117,     0,   293,   103,
       0,   118,   113,     0,     0,     0,     0,     0,    96,    66,
      27,     0,     0,     0,     0,     0,   199,   201,   203,   205,
       0,   223,     0,     0,     0,     0,   253,   247,     0,   251,
       0,     0,     0,   267,   268,   269,   266,   270,   265,     0,
     263,     0,     0,   134,   232,     0,     0,   158,   147,   133,
     152,   135,   160,   129,   130,   217,   219,    42,   240,   241,
     174,   226,   274,     0,     0,   234,   255,     0,     0,   105,
       0,   157,     0,    99,    95,    19,     0,     0,     0,     0,
      20,    21,    22,     0,    74,    76,    77,    78,    79,     0,
       0,     0,    64,     0,    56,   204,   212,     0,     0,     0,
       0,     0,   277,   279,   280,   281,   282,   278,   283,   285,
       0,     0,     0,     0,   271,     0,     0,     0,     0,   248,
       0,   254,   246,     0,    45,     0,     0,    46,   138,     0,
     148,   154,   144,   139,   140,   142,     0,     0,   151,     0,
       0,   150,     0,   162,     0,     0,   176,   235,     0,   236,
       0,   107,   109,   294,     0,     0,     0,     0,   104,     0,
      81,    84,    82,   299,   300,   298,   297,     0,    80,    85,
     275,     0,   273,     0,    63,    65,    68,    28,     0,     0,
       0,    47,     0,     0,    49,    55,    57,    26,   211,   200,
     202,   284,   286,     0,     0,     0,     0,   213,   210,     0,
     209,    93,     0,     0,   252,     0,   245,     0,     0,     0,
       0,   153,   155,   145,   141,   143,     0,   159,     0,     0,
     149,     0,     0,   164,     0,   227,     0,   178,   237,   256,
       0,     0,     0,     0,    75,     0,    67,     0,    86,    87,
      88,    89,    90,     0,     0,    70,    48,     0,    51,    50,
       0,    54,     0,     0,   215,     0,     0,   208,   272,     0,
     249,   238,   239,   242,     0,   243,     0,   136,   137,   161,
     163,     0,   166,   175,     0,   181,   180,   173,     0,    61,
       0,     0,    58,    83,   276,     0,    24,    62,     0,     0,
      23,     0,     0,     0,     0,   206,   214,     0,     0,     0,
       0,     0,   168,   177,   188,   191,     0,     0,    59,     0,
       0,     0,    52,     0,   207,   216,    92,   244,   146,   165,
     167,     0,   122,   169,   170,     0,   192,   193,   194,     0,
       0,     0,     0,     0,    91,     0,    72,    73,     0,    53,
       0,   171,   189,     0,   190,   182,   184,   183,     0,     0,
      69,     0,     0,   195,   196,     0,     0,     0,   179,     0,
       0,   174,   185,   187,   186,     0,     0,     0,    60,     0,
     172,    71
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -370,  -370,  -370,  -370,  -370,  -370,  -370,  -370,  -135,  -370,
    -145,   184,  -370,  -370,  -279,  -370,  -370,  -370,  -370,  -370,
    -370,  -369,   213,  -370,  -370,  -370,  -370,  -370,  -370,  -370,
    -370,   -10,    -3,  -370,  -370,  -370,   303,  -370,   508,  -370,
    -370,   446,   287,   441,   -28,   514,  -370,  -370,   411,  -370,
    -111,  -370,  -370,  -169,   172,  -202,   -11,  -370,  -370,  -370,
    -370,  -370,  -370,  -370,    56,    24,  -370,  -370,  -370,  -370,
    -370,  -370,    89,    65,  -370,  -370,   -38,  -370,  -143,   292,
     294,   382,   -35,   417,   418,   470,  -161,  -370,  -370,  -370,
    -370,  -370,   380,  -370,   444,   383,  -237,  -197,   435,   151,
    -134,  -370,  -370,  -370,  -370,  -370,  -140,    -4,  -370,  -370,
    -370
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    14,    15,    16,    17,    18,    19,    20,   200,   201,
     101,   375,   376,   377,   270,   365,   366,   281,   435,   480,
     528,   273,   274,   275,   276,   277,   278,   432,   476,    21,
      22,    65,   135,    23,    24,   181,   182,    25,    58,    26,
      46,    47,   160,    28,    29,    44,   102,   103,   104,   164,
     105,   331,   326,   237,   238,   320,   321,   239,   333,   413,
     462,   492,   512,   513,   514,   335,   336,   417,   467,   468,
     522,   548,   493,   494,   518,   534,   141,   142,   206,   207,
     208,   209,   210,   107,   108,   109,   110,   111,   112,   113,
     114,   216,   217,   150,   151,   220,   257,   115,   229,   305,
     116,   361,   302,   117,   169,   174,   187,   118,   359,    30,
      31
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      36,    48,   240,   364,   213,   214,   211,    45,    49,   106,
     186,    33,   289,    34,   211,   304,   519,   338,    33,   350,
      34,   351,   179,    33,   289,    34,    67,    33,    63,    34,
     248,   128,    56,    68,    69,    70,    71,    72,   328,   289,
      56,   289,   352,    32,   145,   149,   166,   167,   218,   454,
     171,   172,   236,   166,   167,   166,   167,   284,   265,   545,
     520,    37,   287,   389,   137,   161,   318,   166,   167,   211,
      42,   211,   131,    33,   240,    34,   526,   329,    66,   261,
     262,   353,    10,   291,   292,   293,   294,   295,   296,   297,
     298,   299,   300,   286,   166,   167,   166,   167,    40,    56,
     143,    43,   170,   546,    38,   129,   499,   543,   527,    48,
     354,   355,   144,   189,    39,   183,    49,   221,   188,   316,
     190,   148,   231,    10,   236,   455,   242,   544,    41,   410,
     106,   525,    64,   198,   199,   202,   130,    50,    64,   521,
     188,   180,   356,   259,    59,   256,   360,   390,   367,   446,
     233,   378,   301,   235,   477,   211,   211,   307,   407,   241,
     357,   215,   264,   469,   244,   235,   240,   330,   397,   250,
     288,   249,   251,   438,   459,   252,   173,   463,   502,   234,
     308,   453,   547,   311,   180,    10,   263,   132,   133,   202,
     387,   271,   272,   279,    33,   448,    34,   140,   322,   395,
     223,   420,   193,   194,   368,   323,   484,   423,   258,    57,
     211,   266,   218,   324,   166,   167,   236,   425,    62,   256,
     234,   445,    53,   319,   224,   342,   428,   516,   119,   429,
     430,   241,    49,   166,   167,   325,    49,   267,   225,   226,
     369,   166,   167,   465,   483,   240,    33,   120,    34,   466,
     370,   517,   304,   313,   183,    54,   166,   167,   343,   282,
     283,   227,   381,   382,   383,   385,   121,   388,   122,   460,
     268,   401,   394,   444,    60,   358,   362,    61,   211,   202,
     399,   470,   471,   195,   196,   215,   228,    27,   371,    10,
      51,    35,    52,   474,   481,   236,   269,   306,   262,   372,
     166,   167,    10,   211,   373,   419,   124,   431,   402,   398,
     339,   340,   127,    55,   134,    49,   348,   349,    33,     1,
      34,     2,   322,   241,   384,   374,   136,    49,   138,   323,
     391,   392,   418,   340,  -138,   166,   167,   324,   426,   427,
     123,   139,   140,   188,   146,   279,   508,   168,   442,   147,
       3,   256,   154,   188,   152,   211,   472,   262,   156,   325,
     450,   473,   262,   486,   340,   497,   262,     4,     5,   440,
     498,   262,   505,   340,   153,     6,   256,   506,   392,   256,
       7,   524,   349,   159,   540,   349,   556,   262,   558,   262,
     155,   162,   157,   158,   175,   163,    33,    73,    34,    74,
       8,   550,   241,   165,   176,   185,   192,   197,   256,   555,
     485,   212,   222,    75,    76,   253,   232,   188,   188,   260,
     280,   289,   303,   362,   290,     9,   312,    78,    79,   495,
     314,   315,   317,   334,    10,    80,    81,    82,   327,   332,
     337,   344,   345,   346,    83,    84,   347,    85,   504,   363,
      11,   254,    86,   386,   393,   495,   400,    12,    87,   396,
      13,    88,   403,   406,   404,   405,   408,   409,   411,   414,
     412,   279,   415,   416,   421,    89,    90,   422,   503,   179,
     495,   436,   433,    91,   434,   437,    92,   439,   447,   443,
     449,   457,   458,   451,   452,   461,   279,   464,   475,   478,
     482,    93,   479,    33,    73,    34,    74,   530,   488,   203,
     490,    94,   491,   489,    95,   340,   496,    96,    97,   539,
      75,    76,   500,   501,   510,   507,   529,    98,   511,   515,
     523,   533,   535,    99,    78,    79,   536,   188,   100,   255,
     538,   537,    80,    81,    82,   188,   542,   552,   554,   541,
     549,    83,    84,   551,    85,   553,   559,   560,   561,    86,
     441,   341,   424,   125,   204,    87,   184,   191,    88,   531,
     126,    33,    73,    34,    74,   557,   243,   203,   456,   509,
     532,   379,    89,    90,   380,   285,   245,   178,    75,    76,
      91,   230,   246,    92,   219,     0,   309,     0,     0,   310,
     487,     0,    78,    79,     0,     0,     0,     0,    93,     0,
      80,    81,    82,     0,     0,    10,     0,     0,    94,    83,
      84,    95,    85,     0,    96,    97,     0,    86,     0,     0,
       0,     0,   204,    87,    98,     0,    88,     0,     0,    33,
      99,    34,     0,     0,     0,   205,     0,     0,     0,     0,
      89,    90,     0,     0,     0,     0,     0,     0,    91,     0,
       0,    92,     0,     0,     0,     0,    33,    73,    34,    74,
      78,    79,     0,     0,     0,     0,    93,     0,     0,     0,
      82,     0,     0,    75,    76,    77,    94,    83,    84,    95,
      85,     0,    96,    97,     0,    86,     0,    78,    79,     0,
       0,     0,    98,     0,    88,    80,    81,    82,    99,     0,
       0,     0,     0,   205,    83,    84,     0,    85,    89,   247,
       0,     0,    86,     0,     0,     0,    91,     0,    87,     0,
       0,    88,     0,     0,    33,    73,    34,    74,     0,     0,
       0,     0,     0,     0,    93,    89,    90,     0,     0,     0,
       0,    75,    76,    91,    94,     0,    92,     0,     0,     0,
      96,    97,     0,     0,     0,    78,    79,     0,     0,     0,
      98,    93,     0,    80,    81,    82,    99,     0,     0,     0,
       0,    94,    83,    84,    95,    85,     0,    96,    97,     0,
      86,     0,     0,     0,     0,     0,    87,    98,     0,    88,
       0,     0,     0,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,    89,    90,     0,     0,     0,     0,     0,
       0,    91,     0,     0,    92,     0,     0,     0,     0,    33,
      73,    34,    74,    78,    79,     0,     0,     0,     0,    93,
       0,     0,     0,    82,     0,     0,    75,    76,     0,    94,
      83,    84,    95,    85,     0,    96,    97,     0,    86,     0,
      78,    79,     0,     0,     0,    98,   148,    88,    80,    81,
      82,    99,     0,     0,     0,     0,   100,    83,    84,     0,
      85,    89,   247,     0,     0,    86,     0,     0,     0,    91,
       0,    87,     0,     0,    88,     0,     0,    33,    73,    34,
      74,     0,     0,     0,     0,     0,     0,    93,    89,    90,
       0,     0,     0,     0,    75,    76,    91,    94,     0,    92,
       0,     0,     0,    96,    97,     0,     0,     0,    78,    79,
       0,     0,     0,    98,    93,     0,    80,    81,    82,    99,
       0,    10,     0,     0,    94,    83,    84,    95,    85,     0,
      96,    97,     0,    86,     0,     0,     0,     0,   204,    87,
      98,     0,    88,     0,     0,     0,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,    89,    90,     0,     0,
       0,     0,     0,     0,    91,     0,     0,    92,     0,     0,
       0,     0,    33,    73,    34,    74,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,    75,
     177,     0,    94,     0,     0,    95,     0,     0,    96,    97,
       0,     0,     0,    78,    79,     0,     0,     0,    98,     0,
       0,    80,    81,    82,    99,     0,     0,     0,     0,   205,
      83,    84,     0,    85,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,    87,     0,     0,    88,     0,     0,
      33,    73,    34,    74,     0,     0,     0,     0,     0,     0,
       0,    89,    90,     0,     0,     0,     0,    75,    76,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,    78,    79,     0,     0,     0,     0,    93,     0,    80,
      81,    82,     0,     0,     0,     0,     0,    94,    83,    84,
      95,    85,     0,    96,    97,     0,    86,     0,     0,     0,
       0,     0,    87,    98,     0,    88,     0,     0,     0,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,    89,
      90,     0,     0,     0,     0,     0,    73,    91,    74,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,   177,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,    78,    79,    95,     0,
       0,    96,    97,     0,     0,     0,    82,     0,     0,     0,
       0,    98,     0,    83,    84,     0,    85,    99,     0,     0,
       0,    86,   100,     0,     0,     0,     0,     0,     0,     0,
      88,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,     0,   166,   167,    89,    90,     0,     0,     0,     0,
       0,     0,    91,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,    96,    97,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,     0,
     301,     0,    99
};

static const yytype_int16 yycheck[] =
{
       4,    12,   163,   282,   144,   148,   140,    11,    12,    44,
     121,     4,     8,     6,   148,   212,     7,   254,     4,     5,
       6,     7,    27,     4,     8,     6,    36,     4,    32,     6,
     175,    59,    29,    37,    38,    39,    40,    41,   240,     8,
      29,     8,    28,   123,    72,    80,    21,    22,    63,    71,
      23,    24,   163,    21,    22,    21,    22,   202,   193,     7,
      51,    80,   205,    10,    67,   100,   235,    21,    22,   203,
      28,   205,     5,     4,   235,     6,    76,    39,    73,   147,
     148,    67,   116,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,   204,    21,    22,    21,    22,   123,    29,
     134,    59,   106,    51,   123,    31,   475,    68,   108,   120,
      96,    97,   146,   123,    73,   119,   120,   152,   122,    73,
     124,   136,   157,   116,   235,   147,   164,    88,    85,   331,
     165,   500,   127,   136,   138,   139,    62,     0,   127,   130,
     144,   146,   128,   181,    30,   180,   280,    94,   283,   386,
     147,   147,    79,   146,   433,   289,   290,   124,   327,   163,
     146,   136,   190,   147,   168,   146,   327,   129,   313,   146,
     205,   175,   176,   370,   411,   179,   149,   414,   147,   147,
     215,   147,   130,   218,   146,   116,   189,   120,   121,   193,
     301,   195,   196,   197,     4,   392,     6,   137,    74,   124,
      54,   344,    47,    48,    10,    81,   443,   347,   148,   139,
     344,    46,    63,    89,    21,    22,   327,   357,     3,   254,
     147,     9,    49,    33,    78,   260,    37,    34,   117,    40,
      41,   235,   236,    21,    22,   111,   240,    72,    92,    93,
      46,    21,    22,   108,     9,   406,     4,   148,     6,   114,
      56,    58,   449,    33,   258,    82,    21,    22,   262,   147,
     148,   115,    17,    18,    19,   300,    33,   302,   146,   412,
     105,   147,   307,   384,   142,   279,   280,   145,   412,   283,
     315,   421,   422,    47,    48,   136,   140,     0,    94,   116,
     142,     4,   144,   427,   437,   406,   131,   147,   148,   105,
      21,    22,   116,   437,   110,   340,    85,   118,   319,   313,
     147,   148,   106,    26,   139,   319,   147,   148,     4,     1,
       6,     3,    74,   327,    79,   131,     5,   331,    97,    81,
     147,   148,   147,   148,    86,    21,    22,    89,   147,   148,
      53,   146,   137,   347,     7,   349,   489,    33,   383,     7,
      32,   386,    84,   357,   146,   489,   147,   148,     5,   111,
     395,   147,   148,   147,   148,   147,   148,    49,    50,   373,
     147,   148,   147,   148,   146,    57,   411,   147,   148,   414,
      62,   147,   148,    84,   147,   148,   147,   148,   147,   148,
     146,     5,   146,   146,    61,    73,     4,     5,     6,     7,
      82,   541,   406,   148,   100,   146,     7,   146,   443,   549,
     445,   146,     7,    21,    22,    23,     7,   421,   422,    11,
     146,     8,    20,   427,     9,   107,    64,    35,    36,   464,
     147,    73,   147,   101,   116,    43,    44,    45,   148,    75,
     147,   146,    87,    87,    52,    53,   146,    55,   483,   133,
     132,    59,    60,   146,   134,   490,     7,   139,    66,    64,
     142,    69,    99,    86,    99,    99,     7,     7,    42,    42,
      77,   475,   147,    98,   146,    83,    84,   146,   482,    27,
     515,    94,   148,    91,   139,   146,    94,    87,    94,   146,
     146,   103,   103,   147,   147,    98,   500,    42,   146,    38,
     146,   109,   101,     4,     5,     6,     7,   511,     7,    10,
      42,   119,    90,    97,   122,   148,    19,   125,   126,   523,
      21,    22,   146,    42,     7,   147,   147,   135,   138,   148,
     110,    95,   104,   141,    35,    36,   112,   541,   146,   147,
       9,   104,    43,    44,    45,   549,    33,    70,    70,   146,
     146,    52,    53,   146,    55,   112,   102,   147,     7,    60,
     376,   258,   349,    55,    65,    66,   120,   126,    69,   513,
      56,     4,     5,     6,     7,   551,   165,    10,   406,   490,
     515,   289,    83,    84,   290,   203,   169,   117,    21,    22,
      91,   156,   174,    94,   150,    -1,   216,    -1,    -1,   216,
     449,    -1,    35,    36,    -1,    -1,    -1,    -1,   109,    -1,
      43,    44,    45,    -1,    -1,   116,    -1,    -1,   119,    52,
      53,   122,    55,    -1,   125,   126,    -1,    60,    -1,    -1,
      -1,    -1,    65,    66,   135,    -1,    69,    -1,    -1,     4,
     141,     6,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    94,    -1,    -1,    -1,    -1,     4,     5,     6,     7,
      35,    36,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      45,    -1,    -1,    21,    22,    23,   119,    52,    53,   122,
      55,    -1,   125,   126,    -1,    60,    -1,    35,    36,    -1,
      -1,    -1,   135,    -1,    69,    43,    44,    45,   141,    -1,
      -1,    -1,    -1,   146,    52,    53,    -1,    55,    83,    84,
      -1,    -1,    60,    -1,    -1,    -1,    91,    -1,    66,    -1,
      -1,    69,    -1,    -1,     4,     5,     6,     7,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    83,    84,    -1,    -1,    -1,
      -1,    21,    22,    91,   119,    -1,    94,    -1,    -1,    -1,
     125,   126,    -1,    -1,    -1,    35,    36,    -1,    -1,    -1,
     135,   109,    -1,    43,    44,    45,   141,    -1,    -1,    -1,
      -1,   119,    52,    53,   122,    55,    -1,   125,   126,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    66,   135,    -1,    69,
      -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,     4,
       5,     6,     7,    35,    36,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,    45,    -1,    -1,    21,    22,    -1,   119,
      52,    53,   122,    55,    -1,   125,   126,    -1,    60,    -1,
      35,    36,    -1,    -1,    -1,   135,   136,    69,    43,    44,
      45,   141,    -1,    -1,    -1,    -1,   146,    52,    53,    -1,
      55,    83,    84,    -1,    -1,    60,    -1,    -1,    -1,    91,
      -1,    66,    -1,    -1,    69,    -1,    -1,     4,     5,     6,
       7,    -1,    -1,    -1,    -1,    -1,    -1,   109,    83,    84,
      -1,    -1,    -1,    -1,    21,    22,    91,   119,    -1,    94,
      -1,    -1,    -1,   125,   126,    -1,    -1,    -1,    35,    36,
      -1,    -1,    -1,   135,   109,    -1,    43,    44,    45,   141,
      -1,   116,    -1,    -1,   119,    52,    53,   122,    55,    -1,
     125,   126,    -1,    60,    -1,    -1,    -1,    -1,    65,    66,
     135,    -1,    69,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,   146,    -1,    -1,    -1,    -1,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,
      -1,    -1,     4,     5,     6,     7,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,   119,    -1,    -1,   122,    -1,    -1,   125,   126,
      -1,    -1,    -1,    35,    36,    -1,    -1,    -1,   135,    -1,
      -1,    43,    44,    45,   141,    -1,    -1,    -1,    -1,   146,
      52,    53,    -1,    55,    -1,    -1,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
       4,     5,     6,     7,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    84,    -1,    -1,    -1,    -1,    21,    22,    91,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    36,    -1,    -1,    -1,    -1,   109,    -1,    43,
      44,    45,    -1,    -1,    -1,    -1,    -1,   119,    52,    53,
     122,    55,    -1,   125,   126,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    66,   135,    -1,    69,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,     5,    91,     7,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    35,    36,   122,    -1,
      -1,   125,   126,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      -1,   135,    -1,    52,    53,    -1,    55,   141,    -1,    -1,
      -1,    60,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    -1,    21,    22,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,    -1,    -1,    -1,    -1,   125,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
      79,    -1,   141
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,     3,    32,    49,    50,    57,    62,    82,   107,
     116,   132,   139,   142,   151,   152,   153,   154,   155,   156,
     157,   179,   180,   183,   184,   187,   189,   192,   193,   194,
     259,   260,   123,     4,     6,   192,   257,    80,   123,    73,
     123,    85,    28,    59,   195,   257,   190,   191,   206,   257,
       0,   142,   144,    49,    82,   192,    29,   139,   188,    30,
     142,   145,     3,   257,   127,   181,    73,   181,   257,   257,
     257,   257,   257,     5,     7,    21,    22,    23,    35,    36,
      43,    44,    45,    52,    53,    55,    60,    66,    69,    83,
      84,    91,    94,   109,   119,   122,   125,   126,   135,   141,
     146,   160,   196,   197,   198,   200,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   247,   250,   253,   257,   117,
     148,    33,   146,   192,    85,   188,   195,   106,   194,    31,
      62,     5,   120,   121,   139,   182,     5,   182,    97,   146,
     137,   226,   227,   134,   146,   194,     7,     7,   136,   232,
     243,   244,   146,   146,    84,   146,     5,   146,   146,    84,
     192,   232,     5,    73,   199,   148,    21,    22,    33,   254,
     257,    23,    24,   149,   255,    61,   100,    22,   235,    27,
     146,   185,   186,   257,   191,   146,   200,   256,   257,   181,
     257,   193,     7,    47,    48,    47,    48,   146,   182,   257,
     158,   159,   257,    10,    65,   146,   228,   229,   230,   231,
     232,   250,   146,   256,   228,   136,   241,   242,    63,   244,
     245,   232,     7,    54,    78,    92,    93,   115,   140,   248,
     248,   232,     7,   147,   147,   146,   200,   203,   204,   207,
     236,   257,   226,   198,   257,   233,   234,    84,   160,   257,
     146,   257,   257,    23,    59,   147,   232,   246,   148,   226,
      11,   147,   148,   182,   194,   158,    46,    72,   105,   131,
     164,   257,   257,   171,   172,   173,   174,   175,   176,   257,
     146,   167,   147,   148,   160,   231,   200,   228,   232,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    79,   252,    20,   247,   249,   147,   124,   232,   242,
     245,   232,    64,    33,   147,    73,    73,   147,   203,    33,
     205,   206,    74,    81,    89,   111,   202,   148,   205,    39,
     129,   201,    75,   208,   101,   215,   216,   147,   246,   147,
     148,   186,   232,   257,   146,    87,    87,   146,   147,   148,
       5,     7,    28,    67,    96,    97,   128,   146,   257,   258,
     250,   251,   257,   133,   164,   165,   166,   158,    10,    46,
      56,    94,   105,   110,   131,   161,   162,   163,   147,   229,
     230,    17,    18,    19,    79,   232,   146,   200,   232,    10,
      94,   147,   148,   134,   232,   124,    64,   160,   257,   232,
       7,   147,   206,    99,    99,    99,    86,   203,     7,     7,
     205,    42,    77,   209,    42,   147,    98,   217,   147,   232,
     228,   146,   146,   256,   172,   256,   147,   148,    37,    40,
      41,   118,   177,   148,   139,   168,    94,   146,   247,    87,
     257,   161,   232,   146,   200,     9,   246,    94,   247,   146,
     232,   147,   147,   147,    71,   147,   204,   103,   103,   246,
     228,    98,   210,   246,    42,   108,   114,   218,   219,   147,
     256,   256,   147,   147,   250,   146,   178,   164,    38,   101,
     169,   228,   146,     9,   246,   232,   147,   249,     7,    97,
      42,    90,   211,   222,   223,   232,    19,   147,   147,   171,
     146,    42,   147,   257,   232,   147,   147,   147,   228,   222,
       7,   138,   212,   213,   214,   148,    34,    58,   224,     7,
      51,   130,   220,   110,   147,   171,    76,   108,   170,   147,
     257,   214,   223,    95,   225,   104,   112,   104,     9,   257,
     147,   146,    33,    68,    88,     7,    51,   130,   221,   146,
     256,   146,    70,   112,    70,   256,   147,   215,   147,   102,
     147,     7
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   150,   151,   151,   151,   151,   151,   151,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   153,   154,
     154,   154,   154,   155,   156,   157,   158,   159,   159,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   161,   161,   161,
     161,   161,   161,   161,   162,   162,   163,   163,   164,   164,
     164,   164,   165,   165,   166,   166,   167,   167,   168,   168,
     169,   169,   170,   170,   171,   171,   172,   172,   172,   172,
     173,   173,   173,   174,   175,   176,   177,   177,   177,   177,
     178,   178,   179,   179,   179,   179,   180,   180,   180,   180,
     181,   181,   181,   182,   182,   183,   184,   185,   185,   186,
     187,   187,   188,   188,   189,   190,   190,   191,   192,   192,
     193,   193,   194,   195,   195,   195,   196,   196,   197,   197,
     198,   198,   198,   199,   200,   201,   201,   201,   202,   202,
     202,   202,   202,   202,   202,   202,   203,   203,   204,   204,
     204,   204,   204,   204,   205,   205,   206,   206,   207,   207,
     208,   208,   209,   209,   210,   210,   211,   211,   212,   212,
     213,   213,   214,   215,   216,   216,   217,   217,   218,   218,
     219,   219,   220,   220,   220,   221,   221,   221,   222,   222,
     223,   224,   224,   224,   225,   225,   225,   226,   226,   227,
     228,   228,   229,   229,   230,   230,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   232,   232,   233,
     233,   234,   234,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   236,   236,   236,   236,   237,   237,
     237,   237,   238,   239,   239,   240,   240,   241,   241,   242,
     243,   243,   244,   245,   245,   246,   246,   247,   247,   247,
     247,   247,   247,   247,   247,   248,   248,   248,   248,   248,
     248,   249,   249,   250,   250,   251,   251,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   253,   254,   254,
     255,   255,   255,   256,   256,   257,   257,   258,   258,   258,
     258,   259,   260,   260
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
       3,     3,     6,     6,     8,     5,     4,     1,     2,     4,
       1,     2,     4,     0,     2,     1,     3,     1,     1,     2,
       2,     1,     2,     3,     2,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     2,     2,     0
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
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1990 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_NAME  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2000 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2010 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2020 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 7: /* TOKEN_UNSIGNED_NUMVAL  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2030 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 152: /* sql_statement  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2040 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 153: /* quit_statement  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 2050 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* alter_table_statement  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2060 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* create_table_statement  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 2070 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* create_index_statement  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2080 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* drop_table_statement  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 2090 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* column_def  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 2100 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* column_def_commalist  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 2110 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* data_type  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 2120 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* column_constraint_def  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 2130 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* column_constraint_def_list  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2140 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 163: /* opt_column_constraint_def_list  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2150 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 167: /* opt_column_list  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2160 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* opt_block_properties  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).block_properties_) != nullptr) {
    delete ((*yyvaluep).block_properties_);
  }
}
#line 2170 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* opt_partition_clause  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).partition_clause_) != nullptr) {
    delete ((*yyvaluep).partition_clause_);
  }
}
#line 2180 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* partition_type  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2190 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* key_value_list  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2200 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* key_value  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 2210 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* key_string_value  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 2220 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* key_string_list  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 2230 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* key_integer_value  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 2240 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* key_bool_value  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_bool_value_) != nullptr) {
    delete ((*yyvaluep).key_bool_value_);
  }
}
#line 2250 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* index_type  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2260 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* opt_index_properties  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2270 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* insert_statement  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 2280 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* copy_statement  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_statement_) != nullptr) {
    delete ((*yyvaluep).copy_statement_);
  }
}
#line 2290 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* copy_to_target  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2300 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* opt_copy_params  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2310 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* update_statement  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2320 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* delete_statement  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2330 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* assignment_list  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2340 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* assignment_item  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2350 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* set_operation_statement  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_statement_) != nullptr) {
    delete ((*yyvaluep).set_operation_statement_);
  }
}
#line 2360 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* opt_priority_clause  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_priority_clause_) != nullptr) {
    delete ((*yyvaluep).opt_priority_clause_);
  }
}
#line 2370 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* with_clause  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2380 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 190: /* with_list  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2390 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 191: /* with_list_element  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2400 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 192: /* set_operation_union  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_) != nullptr) {
    delete ((*yyvaluep).set_operation_);
  }
}
#line 2410 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 193: /* set_operation_intersect  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_) != nullptr) {
    delete ((*yyvaluep).set_operation_);
  }
}
#line 2420 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 194: /* select_query  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2430 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 195: /* opt_all_distinct  */
#line 622 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2436 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 196: /* selection  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2446 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 197: /* selection_item_commalist  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2456 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 198: /* selection_item  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2466 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 199: /* from_clause  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2476 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 200: /* subquery_expression  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2486 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 201: /* opt_sample_clause  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_sample_clause_) != nullptr) {
    delete ((*yyvaluep).opt_sample_clause_);
  }
}
#line 2496 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 202: /* join_type  */
#line 624 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2502 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 203: /* joined_table_reference  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2512 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 204: /* table_reference  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2522 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 205: /* table_reference_signature  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2532 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 206: /* table_reference_signature_primary  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2542 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 207: /* joined_table_reference_commalist  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2552 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 208: /* opt_group_by_clause  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2562 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 209: /* opt_having_clause  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2572 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 210: /* opt_order_by_clause  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2582 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 211: /* opt_limit_clause  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2592 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 212: /* opt_window_clause  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_window_clause_) != nullptr) {
    delete ((*yyvaluep).opt_window_clause_);
  }
}
#line 2602 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 213: /* window_declaration_list  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_window_clause_) != nullptr) {
    delete ((*yyvaluep).opt_window_clause_);
  }
}
#line 2612 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 214: /* window_declaration  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_definition_) != nullptr) {
    delete ((*yyvaluep).window_definition_);
  }
}
#line 2622 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 215: /* window_definition  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_definition_) != nullptr) {
    delete ((*yyvaluep).window_definition_);
  }
}
#line 2632 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 216: /* opt_window_partition  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_partition_by_list_) != nullptr) {
    delete ((*yyvaluep).window_partition_by_list_);
  }
}
#line 2642 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 217: /* opt_window_order  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_order_by_list_) != nullptr) {
    delete ((*yyvaluep).window_order_by_list_);
  }
}
#line 2652 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 218: /* opt_window_frame  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_frame_info_) != nullptr) {
    delete ((*yyvaluep).window_frame_info_);
  }
}
#line 2662 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 219: /* frame_mode  */
#line 622 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2668 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 220: /* frame_preceding  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2678 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 221: /* frame_following  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2688 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 222: /* order_commalist  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2698 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 223: /* order_item  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2708 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 224: /* opt_order_direction  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2718 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 225: /* opt_nulls_first  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2728 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 226: /* opt_where_clause  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2738 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 227: /* where_clause  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2748 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 228: /* or_expression  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2758 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 229: /* and_expression  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2768 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 230: /* not_expression  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2778 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 231: /* predicate_expression_base  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2788 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 232: /* add_expression  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2798 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 233: /* multiply_expression  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2808 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 234: /* unary_expression  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2818 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 235: /* expression_base  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2828 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 236: /* function_call  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2838 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 237: /* cast_function  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2848 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 238: /* extract_function  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2858 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 239: /* substr_function  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2868 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 240: /* case_expression  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2878 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 241: /* simple_when_clause_list  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_list_);
  }
}
#line 2888 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 242: /* simple_when_clause  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_);
  }
}
#line 2898 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 243: /* searched_when_clause_list  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_list_);
  }
}
#line 2908 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 244: /* searched_when_clause  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_);
  }
}
#line 2918 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 245: /* opt_else_clause  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2928 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 246: /* expression_list  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2938 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 247: /* literal_value  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2948 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 248: /* datetime_unit  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2958 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 249: /* literal_value_commalist  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2968 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 250: /* attribute_ref  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2978 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 251: /* attribute_ref_list  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2988 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 252: /* comparison_operation  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2994 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 253: /* unary_operation  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).unary_operation_) != nullptr) {
    delete ((*yyvaluep).unary_operation_);
  }
}
#line 3004 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 254: /* add_operation  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).binary_operation_) != nullptr) {
    delete ((*yyvaluep).binary_operation_);
  }
}
#line 3014 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 255: /* multiply_operation  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).binary_operation_) != nullptr) {
    delete ((*yyvaluep).binary_operation_);
  }
}
#line 3024 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 256: /* name_commalist  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 3034 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 257: /* any_name  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 3044 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 258: /* boolean_value  */
#line 622 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 3050 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 259: /* command  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_) != nullptr) {
    delete ((*yyvaluep).command_);
  }
}
#line 3060 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 260: /* command_argument_list  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_argument_list_) != nullptr) {
    delete ((*yyvaluep).command_argument_list_);
  }
}
#line 3070 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 635 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3367 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 639 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3376 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 643 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3385 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 5:
#line 647 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3394 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 651 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    YYABORT;
  }
#line 3402 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 654 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 3411 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 661 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3419 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 9:
#line 664 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].copy_statement_);
  }
#line 3427 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 10:
#line 667 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 3435 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 11:
#line 670 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3443 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 12:
#line 673 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 3451 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 13:
#line 676 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 3459 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 14:
#line 679 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 3467 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 15:
#line 682 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 3475 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 16:
#line 685 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].set_operation_statement_);
  }
#line 3483 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 17:
#line 688 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 3491 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 18:
#line 694 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3499 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 19:
#line 700 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3511 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 20:
#line 707 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3522 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 713 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3534 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 720 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3546 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 729 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].attribute_definition_list_), (yyvsp[-1].block_properties_), (yyvsp[0].partition_clause_));
  }
#line 3554 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 734 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].key_value_list_)) {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_), (yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].key_value_list_));
    } else {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_));
    }
  }
#line 3566 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 743 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 3574 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 748 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 3582 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 753 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3591 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 757 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3600 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 763 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 3610 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 768 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDate));
  }
#line 3618 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 771 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3626 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 774 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3636 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 779 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3644 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 782 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3652 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 785 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3660 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 788 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3668 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 791 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3676 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 794 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3684 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 797 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3692 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 800 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3700 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 803 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3708 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 806 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3725 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 818 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3733 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 821 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3741 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 824 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3764 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 842 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3787 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 862 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3795 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 865 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3803 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 868 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3813 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 873 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3823 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 878 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3834 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 884 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3845 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 890 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3857 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 899 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3866 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 903 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3875 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 909 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3883 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 912 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3891 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 917 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3901 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 922 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3911 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 927 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3923 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 934 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3933 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 941 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3942 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 945 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3951 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 951 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3959 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 954 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = $1; */
  }
#line 3967 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 959 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = nullptr;
  }
#line 3975 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 962 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-1].attribute_list_);
  }
#line 3983 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 967 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = nullptr;
  }
#line 3991 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 970 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].key_value_list_));
  }
#line 3999 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 975 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.partition_clause_) = nullptr;
  }
#line 4007 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 978 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4029 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 997 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::kHashPartitionType);
  }
#line 4037 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 1000 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::kRangePartitionType);
  }
#line 4045 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 1005 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 4054 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 1009 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 4063 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 1015 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 4071 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 1018 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 4079 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 1021 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 4087 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 1024 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_bool_value_);
  }
#line 4095 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 1029 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 4103 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 1032 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 4111 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 1035 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_),
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 4121 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 1042 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4129 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 1047 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 4143 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 1058 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_bool_value_) = new quickstep::ParseKeyBoolValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].boolean_value_));
  }
#line 4151 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 1063 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Defaults to BitWeavingV, but IndexProperties can change this to H.
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBitWeavingV));
  }
#line 4161 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 1068 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
#line 4170 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 1072 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
#line 4179 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 1076 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kSMA));
  }
#line 4188 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 1082 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 4196 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 1085 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 4204 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 1091 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 4217 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 1099 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertTuple((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 4225 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 1102 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-3]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), nullptr);
  }
#line 4233 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 1105 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-4]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), (yyvsp[-4].with_list_));
  }
#line 4241 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 1111 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-4]).first_line, (yylsp[-4]).first_column,
                                           quickstep::ParseStatementCopy::kFrom,
                                           (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4251 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 1116 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-3]).first_line, (yylsp[-3]).first_column,
                                           quickstep::ParseStatementCopy::kTo,
                                           (yyvsp[-2].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4261 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 1121 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-3]).first_line, (yylsp[-3]).first_column,
                                           (yyvsp[-2].set_operation_), nullptr, (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4270 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 1125 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-4]).first_line, (yylsp[-4]).first_column,
                                           (yyvsp[-2].set_operation_), (yyvsp[-4].with_list_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4279 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 1131 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yyvsp[0].string_value_)->line_number(), (yyvsp[0].string_value_)->column_number(), "@" + (yyvsp[0].string_value_)->value());
    delete (yyvsp[0].string_value_);
  }
#line 4288 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 1135 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "$stdout");
  }
#line 4296 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 1138 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "$stderr");
  }
#line 4304 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 1143 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 4312 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 1146 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 4320 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 1151 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 4328 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 1156 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 4336 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 1161 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 4345 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 1165 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 4354 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 1171 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 4362 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 1180 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_statement_) = new quickstep::ParseStatementSetOperation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].set_operation_), nullptr, (yyvsp[0].opt_priority_clause_));
  }
#line 4370 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 1183 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_statement_) = new quickstep::ParseStatementSetOperation((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].set_operation_), (yyvsp[-2].with_list_), (yyvsp[0].opt_priority_clause_));
  }
#line 4378 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 1188 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_priority_clause_) = nullptr;
  }
#line 4386 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 1191 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4408 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 1210 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 4416 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 1215 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4425 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 1219 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4434 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 1225 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 4443 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 1231 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-1].boolean_value_)) {
      (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-3]).first_line, (yylsp[-3]).first_column, quickstep::ParseSetOperation::kUnion);
    } else {
      (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-3]).first_line, (yylsp[-3]).first_column, quickstep::ParseSetOperation::kUnionAll);
    }
    (yyval.set_operation_)->addOperand((yyvsp[-3].set_operation_));
    (yyval.set_operation_)->addOperand((yyvsp[0].set_operation_));
  }
#line 4457 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 1240 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_) = (yyvsp[0].set_operation_);
  }
#line 4465 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 1245 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-2]).first_line, (yylsp[-2]).first_column, quickstep::ParseSetOperation::kIntersect);
    quickstep::ParseSetOperation *op = new quickstep::ParseSetOperation(
        (yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::ParseSetOperation::kSelect);
    op->addOperand((yyvsp[0].select_query_));
    (yyval.set_operation_)->addOperand((yyvsp[-2].set_operation_));
    (yyval.set_operation_)->addOperand(op);
  }
#line 4478 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 1253 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::ParseSetOperation::kSelect);
    (yyval.set_operation_)->addOperand((yyvsp[0].select_query_));
  }
#line 4487 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 1260 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-9]).first_line, (yylsp[-9]).first_column, (yyvsp[-7].selection_), (yyvsp[-6].table_reference_list_), (yyvsp[-5].predicate_), (yyvsp[-4].opt_group_by_clause_), (yyvsp[-3].opt_having_clause_), (yyvsp[-2].opt_order_by_clause_), (yyvsp[-1].opt_limit_clause_), (yyvsp[0].opt_window_clause_));
  }
#line 4495 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 1265 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;  // Distinct
  }
#line 4503 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 1268 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;  // All
  }
#line 4511 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 1271 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;  // Distinct
  }
#line 4519 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 1276 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4527 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 1279 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 4535 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 1284 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4544 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 1288 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4553 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 1294 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 4561 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 1297 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 4569 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 1300 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 4577 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 1305 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[0].table_reference_list_);
  }
#line 4585 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 1310 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].set_operation_));
  }
#line 4593 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 1315 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = NULL;
  }
#line 4601 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 1318 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, true, (yyvsp[-1].numeric_literal_value_));
  }
#line 4609 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 1321 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].numeric_literal_value_));
  }
#line 4617 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 1326 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4625 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 1329 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4633 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 1332 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4641 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 1335 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4649 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 1338 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4657 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 1341 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4665 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 1344 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4673 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 1347 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4681 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 1352 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseJoinedTableReference((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].join_type_), (yyvsp[-5].table_reference_), (yyvsp[-2].table_reference_), (yyvsp[0].predicate_));
  }
#line 4689 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 1355 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = (yyvsp[0].table_reference_);
  }
#line 4697 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 1360 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4706 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 1364 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].opt_sample_clause_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4715 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 1368 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].opt_sample_clause_));
  }
#line 4723 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 1371 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4732 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 1375 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].function_call_));
  }
#line 4740 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 1378 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = (yyvsp[-1].table_reference_);
  }
#line 4748 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 1383 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4756 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 1386 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4764 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 1391 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4772 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 1394 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4780 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 1399 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4789 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 1403 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4798 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 1409 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 4806 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 1412 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4814 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 1417 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4822 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 1420 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4830 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 1425 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4838 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 1428 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4846 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 1433 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4854 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 1436 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4876 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 1455 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = nullptr;
  }
#line 4884 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 1458 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = (yyvsp[0].opt_window_clause_);
  }
#line 4892 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 1463 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = new quickstep::PtrList<quickstep::ParseWindow>();
    (yyval.opt_window_clause_)->push_back((yyvsp[0].window_definition_));
  }
#line 4901 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 1467 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = (yyvsp[-1].opt_window_clause_);
    (yyval.opt_window_clause_)->push_back((yyvsp[0].window_definition_));
  }
#line 4910 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 1473 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_definition_) = (yyvsp[-1].window_definition_);
    (yyval.window_definition_)->setName((yyvsp[-4].string_value_));
  }
#line 4919 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 1479 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_definition_) = new quickstep::ParseWindow((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].window_partition_by_list_), (yyvsp[-1].window_order_by_list_), (yyvsp[0].window_frame_info_));
  }
#line 4927 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 1484 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_partition_by_list_) = nullptr;
  }
#line 4935 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 1487 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_partition_by_list_) = (yyvsp[0].expression_list_);
  }
#line 4943 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 1492 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_order_by_list_) = nullptr;
  }
#line 4951 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 1495 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_order_by_list_) = (yyvsp[0].order_commalist_);
  }
#line 4959 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 1500 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_frame_info_) = nullptr;
  }
#line 4967 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 1503 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_frame_info_) = new quickstep::ParseFrameInfo((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-4].boolean_value_), (yyvsp[-2].numeric_literal_value_)->long_value(), (yyvsp[0].numeric_literal_value_)->long_value());
  }
#line 4975 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 1508 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4983 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 1511 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 4991 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 1517 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "-1");
  }
#line 4999 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 1520 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "0");
  }
#line 5007 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1526 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "-1");
  }
#line 5015 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 1529 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "0");
  }
#line 5023 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 1534 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 5032 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 1538 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 5041 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 1544 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 5051 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 1551 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 5059 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 1554 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 5067 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 1557 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 5075 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 1562 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 5083 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 1565 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 5091 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 1568 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 5099 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 1574 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 5107 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 1577 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5115 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 1582 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5123 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1587 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 5137 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1596 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5145 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1601 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 5159 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1610 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5167 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1615 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 5175 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1618 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5183 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1623 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5191 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1626 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 5201 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1631 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 5212 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1637 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 5223 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1643 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5231 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1646 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 5239 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1649 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateExists((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].subquery_expression_));
  }
#line 5247 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1652 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-2].expression_), (yyvsp[0].subquery_expression_));
  }
#line 5255 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1655 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-1].expression_list_));
  }
#line 5263 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1658 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-2]).first_line,
        (yylsp[-2]).first_column,
        new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-3].expression_), (yyvsp[0].subquery_expression_)));
  }
#line 5274 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1664 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line,
        (yylsp[-4]).first_column,
        new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-1].expression_list_)));
  }
#line 5285 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1673 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-2].expression_));
    arguments->push_back((yyvsp[0].expression_));
    (yyval.expression_) = new quickstep::ParseFunctionCall((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].binary_operation_), arguments);
  }
#line 5296 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1679 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5304 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1684 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-2].expression_));
    arguments->push_back((yyvsp[0].expression_));
    (yyval.expression_) = new quickstep::ParseFunctionCall((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].binary_operation_), arguments);
  }
#line 5315 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1690 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5323 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1695 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[0].expression_));
    (yyval.expression_) = new quickstep::ParseFunctionCall((yylsp[-1]).first_line, (yylsp[-1]).first_column, false, (yyvsp[-1].unary_operation_), arguments);
  }
#line 5333 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1700 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5341 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1705 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 5349 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1708 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 5357 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1711 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 5365 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1714 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyvsp[-2].function_call_)->setWindowName((yyvsp[0].string_value_));
    (yyval.expression_) = (yyvsp[-2].function_call_);
  }
#line 5374 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1718 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyvsp[-4].function_call_)->setWindow((yyvsp[-1].window_definition_));
    (yyval.expression_) = (yyvsp[-4].function_call_);
  }
#line 5383 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1722 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5391 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1725 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5399 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1728 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5407 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1731 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5415 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1734 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 5423 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1737 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].subquery_expression_);
  }
#line 5431 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1742 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 5440 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1746 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 5449 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1750 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, false, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 5457 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1753 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-4]).first_line, (yylsp[-4]).first_column, true, (yyvsp[-4].string_value_), (yyvsp[-1].expression_list_));
  }
#line 5465 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1758 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5484 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1772 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-3].expression_));
    arguments->push_back(new quickstep::ParseScalarLiteral(
        new quickstep::StringParseLiteralValue((yyvsp[-1].string_value_), nullptr)));
    auto *name = new quickstep::ParseString((yylsp[-5]).first_line, (yylsp[-5]).first_column, "cast");
    (yyval.expression_) = new quickstep::ParseFunctionCall(
        (yylsp[-5]).first_line, (yylsp[-5]).first_column, false, name, arguments);
  }
#line 5498 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1781 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5517 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1795 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-2].expression_));
    arguments->push_back(new quickstep::ParseScalarLiteral(
        new quickstep::StringParseLiteralValue((yyvsp[0].string_value_), nullptr)));
    auto *name = new quickstep::ParseString((yylsp[-1]).first_line, (yylsp[-1]).first_column, "cast");
    (yyval.expression_) = new quickstep::ParseFunctionCall(
        (yylsp[-1]).first_line, (yylsp[-1]).first_column, false, name, arguments);
  }
#line 5531 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1806 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-1].expression_));
    arguments->push_back(new quickstep::ParseScalarLiteral(
        new quickstep::StringParseLiteralValue((yyvsp[-3].string_value_), nullptr)));
    auto *name = new quickstep::ParseString((yylsp[-5]).first_line, (yylsp[-5]).first_column, "extract");
    (yyval.expression_) = new quickstep::ParseFunctionCall(
        (yylsp[-5]).first_line, (yylsp[-5]).first_column, false, name, arguments);
  }
#line 5545 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1817 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-3].expression_));
    arguments->push_back(new quickstep::ParseScalarLiteral((yyvsp[-1].numeric_literal_value_)));
    auto *name = new quickstep::ParseString((yylsp[-5]).first_line, (yylsp[-5]).first_column, "substring");
    (yyval.expression_) = new quickstep::ParseFunctionCall(
        (yylsp[-5]).first_line, (yylsp[-5]).first_column, false, name, arguments);
  }
#line 5558 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1825 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-5].expression_));
    arguments->push_back(new quickstep::ParseScalarLiteral((yyvsp[-3].numeric_literal_value_)));
    arguments->push_back(new quickstep::ParseScalarLiteral((yyvsp[-1].numeric_literal_value_)));
    auto *name = new quickstep::ParseString((yylsp[-7]).first_line, (yylsp[-7]).first_column, "substring");
    (yyval.expression_) = new quickstep::ParseFunctionCall(
        (yylsp[-7]).first_line, (yylsp[-7]).first_column, false, name, arguments);
  }
#line 5572 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1836 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSimpleCaseExpression((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].expression_), (yyvsp[-2].simple_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 5580 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1839 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSearchedCaseExpression((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].searched_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 5588 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1844 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSimpleWhenClause>;
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 5597 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1848 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_list_) = (yyvsp[-1].simple_when_clause_list_);
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 5606 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1854 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_) = new quickstep::ParseSimpleWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5614 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1859 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSearchedWhenClause>;
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 5623 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1863 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_list_) = (yyvsp[-1].searched_when_clause_list_);
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 5632 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1869 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_) = new quickstep::ParseSearchedWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].predicate_), (yyvsp[0].expression_));
  }
#line 5640 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1874 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = NULL;
  }
#line 5648 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1877 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5656 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1882 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 5665 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1886 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 5674 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1892 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 5682 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1895 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5690 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1898 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5698 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1901 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5715 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1913 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 5724 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1917 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5748 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1936 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5766 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1949 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5784 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1964 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("YEAR"));
  }
#line 5792 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1967 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("MONTH"));
  }
#line 5800 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1970 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("DAY"));
  }
#line 5808 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1973 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("HOUR"));
  }
#line 5816 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1976 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("MINUTE"));
  }
#line 5824 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1979 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("SECOND"));
  }
#line 5832 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1984 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5841 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1988 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5850 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1994 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 5858 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1997 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 5866 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 2002 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = new quickstep::PtrList<quickstep::ParseAttribute>();
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5875 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 2006 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-2].attribute_list_);
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5884 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 2013 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 5892 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 2016 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 5900 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 2019 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 5908 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 2022 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 5916 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 2025 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 5924 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 2028 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 5932 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 2031 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLike);
  }
#line 5940 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 2034 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotLike);
  }
#line 5948 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 2037 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kRegexMatch);
  }
#line 5956 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 2040 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotRegexMatch);
  }
#line 5964 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 2045 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("-"));
  }
#line 5978 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 2056 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("+"));
  }
#line 5986 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 2059 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("-"));
  }
#line 5994 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 2064 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("%"));
  }
#line 6002 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 2067 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("*"));
  }
#line 6010 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 2070 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("/"));
  }
#line 6018 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 2076 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 6027 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 2080 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 6036 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 2086 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 6044 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 2089 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 6055 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 2097 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 6063 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 2100 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 6071 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 2103 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 6079 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 2106 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 6087 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 2112 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.command_) = new quickstep::ParseCommand((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].command_argument_list_));
  }
#line 6095 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 2117 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    quickstep::PtrVector<quickstep::ParseString> *argument_list = (yyvsp[-1].command_argument_list_);
    argument_list->push_back((yyvsp[0].string_value_));
    (yyval.command_argument_list_) = argument_list;
  }
#line 6105 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 2122 "../SqlParser.ypp" /* yacc.c:1661  */
    { /* Epsilon, an empy match. */
    (yyval.command_argument_list_) = new quickstep::PtrVector<quickstep::ParseString>();
  }
#line 6113 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;


#line 6117 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 2126 "../SqlParser.ypp" /* yacc.c:1906  */


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
