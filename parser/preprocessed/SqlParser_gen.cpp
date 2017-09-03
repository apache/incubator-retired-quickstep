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
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "types/operations/unary_operations/UnaryOperation.hpp"
#include "types/operations/unary_operations/UnaryOperationFactory.hpp"
#include "types/operations/unary_operations/UnaryOperationID.hpp"
#include "utility/PtrList.hpp"
#include "utility/PtrVector.hpp"

// Needed for Bison 2.6 and higher, which do not automatically provide this typedef.
typedef void* yyscan_t;

#line 157 "SqlParser_gen.cpp" /* yacc.c:339  */

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
    TOKEN_CHARACTER = 294,
    TOKEN_CHECK = 295,
    TOKEN_COLUMN = 296,
    TOKEN_CONSTRAINT = 297,
    TOKEN_COPY = 298,
    TOKEN_CREATE = 299,
    TOKEN_CURRENT = 300,
    TOKEN_DATE = 301,
    TOKEN_DATETIME = 302,
    TOKEN_DAY = 303,
    TOKEN_DECIMAL = 304,
    TOKEN_DEFAULT = 305,
    TOKEN_DELETE = 306,
    TOKEN_DESC = 307,
    TOKEN_DISTINCT = 308,
    TOKEN_DOUBLE = 309,
    TOKEN_DROP = 310,
    TOKEN_ELSE = 311,
    TOKEN_END = 312,
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
    TOKEN_STDERR = 368,
    TOKEN_STDIN = 369,
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
#line 121 "../SqlParser.ypp" /* yacc.c:355  */

  quickstep::ParseString *string_value_;

  quickstep::PtrList<quickstep::ParseString> *string_list_;

  bool boolean_value_;

  quickstep::NumericParseLiteralValue *numeric_literal_value_;
  quickstep::ParseLiteralValue *literal_value_;
  quickstep::PtrList<quickstep::ParseScalarLiteral> *literal_value_list_;
  quickstep::PtrList<quickstep::PtrList<quickstep::ParseScalarLiteral>> *literal_value_list_multiple_;

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

#line 437 "SqlParser_gen.cpp" /* yacc.c:355  */
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
#line 223 "../SqlParser.ypp" /* yacc.c:358  */

/* This header needs YYSTYPE, which is defined by the %union directive above */
#include "SqlLexer_gen.hpp"
void NotSupported(const YYLTYPE *location, yyscan_t yyscanner, const std::string &feature);

#line 472 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYLAST   1320

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  149
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  112
/* YYNRULES -- Number of rules.  */
#define YYNRULES  302
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  557

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
       0,   646,   646,   650,   654,   658,   662,   665,   672,   675,
     678,   681,   684,   687,   690,   693,   696,   699,   705,   711,
     718,   724,   731,   740,   745,   754,   759,   764,   768,   774,
     779,   782,   785,   790,   793,   796,   799,   802,   805,   808,
     811,   814,   817,   829,   832,   835,   853,   873,   876,   879,
     884,   889,   895,   901,   910,   914,   920,   923,   928,   933,
     938,   945,   952,   956,   962,   965,   970,   973,   978,   981,
     986,   989,  1008,  1011,  1016,  1020,  1026,  1029,  1032,  1035,
    1040,  1043,  1046,  1053,  1058,  1069,  1074,  1079,  1083,  1087,
    1093,  1096,  1102,  1110,  1113,  1116,  1122,  1127,  1132,  1136,
    1142,  1146,  1151,  1155,  1158,  1163,  1166,  1171,  1176,  1181,
    1185,  1191,  1200,  1203,  1208,  1211,  1230,  1235,  1239,  1245,
    1251,  1260,  1265,  1273,  1279,  1285,  1288,  1291,  1296,  1299,
    1304,  1308,  1314,  1317,  1320,  1325,  1330,  1335,  1338,  1341,
    1346,  1349,  1352,  1355,  1358,  1361,  1364,  1367,  1372,  1375,
    1380,  1384,  1388,  1391,  1395,  1398,  1403,  1406,  1411,  1414,
    1419,  1423,  1429,  1432,  1437,  1440,  1445,  1448,  1453,  1456,
    1475,  1478,  1483,  1487,  1493,  1499,  1504,  1507,  1512,  1515,
    1520,  1523,  1528,  1531,  1536,  1537,  1540,  1545,  1546,  1549,
    1554,  1558,  1564,  1571,  1574,  1577,  1582,  1585,  1588,  1594,
    1597,  1602,  1607,  1616,  1621,  1630,  1635,  1638,  1643,  1646,
    1651,  1657,  1663,  1666,  1669,  1672,  1675,  1678,  1684,  1693,
    1696,  1701,  1704,  1709,  1712,  1717,  1720,  1723,  1726,  1730,
    1734,  1737,  1740,  1743,  1746,  1751,  1755,  1759,  1762,  1767,
    1772,  1776,  1782,  1785,  1790,  1794,  1800,  1805,  1809,  1815,
    1820,  1823,  1828,  1832,  1838,  1841,  1844,  1847,  1859,  1863,
    1882,  1895,  1910,  1913,  1916,  1919,  1922,  1925,  1930,  1934,
    1940,  1944,  1950,  1953,  1958,  1962,  1969,  1972,  1975,  1978,
    1981,  1984,  1987,  1990,  1993,  1996,  2001,  2012,  2015,  2020,
    2023,  2026,  2032,  2036,  2042,  2045,  2053,  2056,  2059,  2062,
    2068,  2073,  2078
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
  "TOKEN_CHARACTER", "TOKEN_CHECK", "TOKEN_COLUMN", "TOKEN_CONSTRAINT",
  "TOKEN_COPY", "TOKEN_CREATE", "TOKEN_CURRENT", "TOKEN_DATE",
  "TOKEN_DATETIME", "TOKEN_DAY", "TOKEN_DECIMAL", "TOKEN_DEFAULT",
  "TOKEN_DELETE", "TOKEN_DESC", "TOKEN_DISTINCT", "TOKEN_DOUBLE",
  "TOKEN_DROP", "TOKEN_ELSE", "TOKEN_END", "TOKEN_EXISTS", "TOKEN_EXTRACT",
  "TOKEN_FALSE", "TOKEN_FIRST", "TOKEN_FLOAT", "TOKEN_FOLLOWING",
  "TOKEN_FOR", "TOKEN_FOREIGN", "TOKEN_FROM", "TOKEN_FULL", "TOKEN_GROUP",
  "TOKEN_HASH", "TOKEN_HAVING", "TOKEN_HOUR", "TOKEN_IN", "TOKEN_INDEX",
  "TOKEN_INNER", "TOKEN_INSERT", "TOKEN_INTEGER", "TOKEN_INTERVAL",
  "TOKEN_INTO", "TOKEN_JOIN", "TOKEN_KEY", "TOKEN_LAST", "TOKEN_LEFT",
  "TOKEN_LIMIT", "TOKEN_LONG", "TOKEN_MINUTE", "TOKEN_MONTH", "TOKEN_NULL",
  "TOKEN_NULLS", "TOKEN_OFF", "TOKEN_ON", "TOKEN_ORDER", "TOKEN_OUTER",
  "TOKEN_OVER", "TOKEN_PARTITION", "TOKEN_PARTITIONS", "TOKEN_PERCENT",
  "TOKEN_PRECEDING", "TOKEN_PRIMARY", "TOKEN_PRIORITY", "TOKEN_QUIT",
  "TOKEN_RANGE", "TOKEN_REAL", "TOKEN_REFERENCES", "TOKEN_RIGHT",
  "TOKEN_ROW", "TOKEN_ROW_DELIMITER", "TOKEN_ROWS", "TOKEN_SECOND",
  "TOKEN_SELECT", "TOKEN_SET", "TOKEN_SMA", "TOKEN_SMALLINT",
  "TOKEN_STDERR", "TOKEN_STDIN", "TOKEN_STDOUT", "TOKEN_SUBSTRING",
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
  "copy_from_target", "copy_to_target", "opt_copy_params",
  "update_statement", "delete_statement", "assignment_list",
  "assignment_item", "set_operation_statement", "opt_priority_clause",
  "with_clause", "with_list", "with_list_element", "set_operation_union",
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
  "unary_expression", "expression_base", "function_call",
  "extract_function", "substr_function", "case_expression",
  "simple_when_clause_list", "simple_when_clause",
  "searched_when_clause_list", "searched_when_clause", "opt_else_clause",
  "expression_list", "literal_value", "datetime_unit",
  "literal_value_commalist", "literal_value_commalist_multiple",
  "attribute_ref", "attribute_ref_list", "comparison_operation",
  "unary_operation", "add_operation", "multiply_operation",
  "name_commalist", "any_name", "boolean_value", "command",
  "command_argument_list", YY_NULLPTR
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

#define YYPACT_NINF -404

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-404)))

#define YYTABLE_NINF -141

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     613,  -404,  -404,   -19,    63,   -26,    96,    -6,    41,  -404,
      67,    74,    74,  -404,   194,    81,  -404,  -404,  -404,  -404,
    -404,  -404,  -404,  -404,  -404,  -404,   148,   -12,   174,  -404,
     -40,   223,    74,  -404,  -404,    -3,   -17,    74,    74,    74,
      74,    74,  -404,  -404,   679,   122,   103,  -404,   207,   120,
    -404,  -404,  -404,   169,   210,   -12,    67,   212,  -404,   169,
    -404,  -404,  -404,    39,    93,   160,    38,   160,   160,   219,
     159,   182,  -404,   170,  -404,  -404,   313,   315,  -404,  -404,
    -404,   758,   181,  -404,   253,  -404,  -404,   197,  -404,  -404,
     341,  -404,  -404,  -404,  -404,   203,  -404,  -404,   221,   301,
     837,   368,   320,   248,  -404,  -404,   339,    -8,  -404,  -404,
     298,  -404,  -404,  -404,  -404,  -404,  1022,     5,    74,    74,
     259,    74,    -3,    74,  -404,   169,   409,  -404,   186,   201,
    -404,  -404,  -404,   281,  -404,  -404,  -404,  -404,  -404,    74,
      74,   600,  -404,  -404,   289,    74,  -404,  -404,  -404,   600,
      19,    66,  -404,   426,  -404,   155,   155,  1101,   428,  -404,
     -25,    26,  -404,     8,   182,  1101,  -404,  -404,    74,  1101,
    -404,  -404,  -404,  -404,  1101,    21,   315,  -404,    74,   363,
     -68,  -404,   425,  -404,   169,  -404,   109,  -404,   160,   169,
     174,  -404,    74,   136,    74,    74,    74,   292,  -404,   111,
     200,   943,   259,   494,   430,   431,  -404,  -404,   552,   419,
    1180,   294,   141,    15,  1101,   151,  -404,  1101,  -404,   381,
     300,  -404,  -404,  -404,  -404,  -404,  -404,   376,  -404,   258,
     302,  -404,  -404,    18,   326,   249,  -404,   304,   326,    58,
     380,  -404,  -404,    -8,  -404,   350,  -404,  -404,   308,  1101,
    -404,   288,   206,    74,  -404,  1101,  -404,    74,  -404,  -404,
    -404,   311,   372,   373,   314,  -404,  -404,  -404,   209,  -404,
    -404,  -404,  -404,  -404,    53,    74,   328,   136,    74,  -404,
      97,  -404,  -404,    12,    72,   600,   600,   198,  -404,  -404,
    -404,  -404,  -404,  -404,  -404,  -404,  1101,   316,  1101,    45,
    -404,   216,   317,   330,  1101,    55,  -404,   402,   288,  -404,
    -404,  1101,   459,  -404,   218,    74,  -404,  -404,   370,  -404,
     371,   374,   385,     8,  -404,   466,   467,   326,   434,   403,
     435,   333,   386,  -404,   229,  -404,  1101,  -404,   288,  -404,
     600,   336,   338,    74,  -404,    74,  -404,  -404,  -404,  -404,
    -404,  -404,  -404,    74,  -404,  -404,  -404,   231,   458,    71,
    -404,   342,   348,  -404,   398,   347,  1180,  -404,   408,    74,
    -404,  -404,    97,  -404,  -404,   431,  -404,  -404,  -404,  1101,
     349,   291,   837,  -404,   288,   404,  -404,  -404,  1180,  1180,
     360,   288,  1101,  -404,    29,    51,  -404,  -404,  -404,  -404,
    -404,     8,   249,   394,   405,  -404,  1101,   600,   411,  1101,
    -404,   468,   113,  -404,   288,    23,    74,    74,   233,  -404,
     235,  -404,    74,  -404,  -404,  -404,  -404,   366,   136,   474,
     414,  -404,   600,  -404,  -404,   375,  -404,   329,   837,  -404,
    1101,   241,  -404,  -404,   243,  1180,   288,  -404,   510,  -404,
     423,  -404,  -404,   377,   430,   477,   433,   377,  1101,  -404,
    -404,  -404,   503,  -404,   247,   255,  -404,  -404,  -404,    74,
    -404,  -404,   378,   483,  -404,    30,    74,  1101,   262,   288,
    -404,  -404,   264,   382,   600,  1101,   519,   390,   384,  -404,
     274,    14,   427,  -404,   273,    74,     0,  -404,   387,   288,
    -404,  -404,  -404,   430,   384,  -404,    74,  -404,   390,  -404,
    1101,  -404,  -404,   441,   437,   432,   439,   527,    74,  -404,
     278,  -404,  -404,   399,  -404,   514,  -404,  -404,    47,  -404,
    -404,  -404,  -404,    22,   406,  -404,    74,   407,  -404,  -404,
     475,   440,   481,  -404,    74,   282,   350,  -404,  -404,  -404,
     285,   454,   410,  -404,   548,  -404,  -404
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     6,   302,     0,     0,     0,     0,     0,     0,    18,
     125,     0,     0,     7,     0,     0,    15,     8,    10,    11,
      13,    14,     9,    17,    12,    16,     0,   114,   121,   123,
       0,   300,     0,   294,   295,     0,     0,     0,     0,     0,
       0,     0,   126,   127,     0,     0,   116,   117,     0,   158,
       1,     3,     2,     0,     0,   114,   125,     0,   112,     0,
       5,     4,   301,     0,     0,   105,     0,   105,   105,     0,
       0,   199,    25,     0,   258,   255,     0,   286,   128,    40,
      29,     0,     0,    30,    31,    34,    36,     0,    37,    39,
       0,    41,   254,    35,    38,     0,    32,    33,     0,     0,
       0,     0,     0,   129,   130,   234,   134,   220,   222,   224,
     227,   230,   231,   232,   226,   225,     0,   272,     0,     0,
       0,     0,     0,     0,   113,     0,     0,   122,     0,     0,
     102,   104,   103,     0,    98,   100,   101,    96,    97,     0,
       0,     0,   108,   200,     0,     0,    94,   256,   257,     0,
       0,   250,   247,     0,    43,     0,   259,     0,     0,    44,
       0,     0,   261,     0,   199,     0,   287,   288,     0,     0,
     133,   290,   291,   289,     0,     0,     0,   223,     0,     0,
     199,   110,     0,   118,     0,   119,     0,   292,   105,     0,
     120,   115,     0,     0,     0,     0,     0,    66,    27,     0,
       0,     0,     0,     0,   201,   203,   205,   207,     0,   225,
       0,    93,     0,     0,     0,   250,   244,     0,   248,     0,
       0,   264,   265,   266,   263,   267,   262,     0,   260,     0,
       0,   136,   233,     0,     0,   160,   149,   135,   154,   137,
     162,   131,   132,   219,   221,   176,   228,   273,     0,     0,
     235,   252,     0,     0,   107,     0,   159,     0,    99,    95,
      19,     0,     0,     0,     0,    20,    21,    22,     0,    74,
      76,    77,    78,    79,     0,     0,     0,    64,     0,    42,
      56,   206,   214,     0,     0,     0,     0,     0,   276,   278,
     279,   280,   281,   277,   282,   284,     0,     0,     0,     0,
     268,     0,     0,     0,     0,     0,   245,     0,   251,   243,
      45,     0,     0,    46,   140,     0,   150,   156,   146,   141,
     142,   144,     0,     0,   153,     0,     0,   152,     0,   164,
       0,     0,   178,   236,     0,   237,     0,   109,   111,   293,
       0,     0,     0,     0,   106,     0,    81,    84,    82,   298,
     299,   297,   296,     0,    80,    85,   274,     0,   272,     0,
      63,    65,    68,    28,     0,     0,     0,    47,     0,     0,
      49,    55,    57,    26,   213,   202,   204,   283,   285,     0,
       0,     0,     0,   215,   212,     0,   211,   270,     0,     0,
       0,   249,     0,   242,     0,     0,   155,   157,   147,   143,
     145,     0,   161,     0,     0,   151,     0,     0,   166,     0,
     229,     0,   180,   238,   253,     0,     0,     0,     0,    75,
       0,    67,     0,    86,    87,    88,    89,    90,     0,     0,
      70,    48,     0,    51,    50,     0,    54,     0,     0,   217,
       0,     0,   210,   269,     0,     0,   246,   239,     0,   240,
       0,   138,   139,   163,   165,     0,   168,   177,     0,   183,
     182,   175,     0,    61,     0,     0,    58,    83,   275,     0,
      24,    62,     0,     0,    23,     0,     0,     0,     0,   208,
     216,   271,     0,     0,     0,     0,     0,   170,   179,   190,
     193,     0,     0,    59,     0,     0,     0,    52,     0,   209,
     218,    92,   241,   148,   167,   169,     0,   124,   171,   172,
       0,   194,   195,   196,     0,     0,     0,     0,     0,    91,
       0,    72,    73,     0,    53,     0,   173,   191,     0,   192,
     184,   186,   185,     0,     0,    69,     0,     0,   197,   198,
       0,     0,     0,   181,     0,     0,   176,   187,   189,   188,
       0,     0,     0,    60,     0,   174,    71
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,  -150,  -404,
     359,   188,  -404,  -404,  -267,  -404,  -404,  -404,  -404,  -404,
    -404,  -403,   227,  -404,  -404,  -404,  -404,  -404,  -404,  -404,
    -404,  -404,    17,    -5,  -404,  -404,  -404,   324,  -404,   523,
    -404,  -404,   460,   238,   455,   -29,   525,  -404,  -404,   417,
    -404,  -102,  -404,  -404,  -194,   183,  -193,    -9,  -404,  -404,
    -404,  -404,  -404,  -404,  -404,    77,    42,  -404,  -404,  -404,
    -404,  -404,  -404,   102,    79,  -404,  -404,    43,  -404,  -147,
     305,   306,   392,   -35,   422,   420,   479,  -158,  -404,  -404,
    -404,  -404,   383,  -404,   445,   388,  -236,  -204,   443,  -340,
    -404,  -130,  -404,  -404,  -404,  -404,  -404,  -144,    -4,  -404,
    -404,  -404
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    14,    15,    16,    17,    18,    19,    20,   198,   199,
     101,   371,   372,   373,   265,   361,   362,   276,   430,   474,
     523,   268,   269,   270,   271,   272,   273,   427,   470,    21,
      22,    67,    65,   134,    23,    24,   180,   181,    25,    58,
      26,    46,    47,   160,    28,    29,    44,   102,   103,   104,
     164,   105,   327,   322,   235,   236,   316,   317,   237,   329,
     408,   456,   487,   507,   508,   509,   331,   332,   412,   461,
     462,   517,   543,   488,   489,   513,   529,   142,   143,   204,
     205,   206,   207,   208,   107,   108,   109,   110,   111,   112,
     113,   215,   216,   151,   152,   219,   252,   114,   227,   301,
     211,   115,   357,   298,   116,   169,   174,   186,   117,   355,
      30,    31
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      36,   212,   213,    48,    56,   238,   300,    45,    49,   106,
     360,   209,    33,   334,    34,   171,   172,    56,   185,   209,
     285,   514,    33,   285,    34,    33,    56,    34,    63,   540,
     127,   285,   178,    69,    70,    71,    72,    73,   285,   314,
     166,   167,   260,   135,   146,   324,   150,   166,   167,   444,
     166,   167,    37,    68,    66,   385,   283,    33,   346,    34,
     347,   234,   137,   138,   515,   161,   494,    33,   141,    34,
     128,   209,   541,   209,   521,   238,   166,   167,    33,   253,
      34,   348,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   520,   166,   167,    42,    38,   325,   130,   129,
     282,    60,   170,    32,    61,   482,   522,   364,   423,    64,
      48,   424,   425,   538,   182,    49,    40,   187,   349,   189,
     448,   231,   229,    64,    41,    43,    57,   217,   363,   402,
     106,   234,    10,   539,   405,   197,   200,   386,   304,   188,
     173,   187,   365,   516,   251,   356,   441,   350,   351,   297,
     179,   542,   366,   233,   214,   209,   209,   136,   374,   239,
     259,   471,   433,   233,   242,   238,   245,    39,   284,   463,
     453,   246,   232,   457,   247,   447,   497,    10,   392,   305,
     352,   261,   308,   258,   443,   300,   326,   426,   200,   367,
     266,   267,   274,   415,    50,   383,    53,   449,   353,   418,
     368,   149,   478,   179,    59,   369,   262,   240,   221,   420,
     209,   131,   217,   132,   251,   377,   378,   379,   232,   459,
     338,   234,    51,   254,    52,   460,    62,   370,    54,   239,
      49,   222,   192,   193,    49,    79,    80,   118,    27,   263,
     120,   300,    35,   238,    82,   223,   224,   194,   195,   182,
     119,    83,    84,   339,    85,   256,   257,   277,   278,    86,
     454,   381,    10,   384,    55,   121,   264,    88,   225,   391,
     354,   358,   464,   465,   200,   380,   394,   209,   439,   166,
     167,    89,   279,    10,    10,   475,   214,   303,   257,    91,
     318,   122,   468,   123,   226,   166,   167,   319,   133,   234,
     440,   414,   209,   144,   140,   320,   397,    93,   511,   166,
     167,    49,   166,   167,   139,   145,   126,    94,   141,   239,
     147,   318,   148,    49,    96,    97,   153,   321,   319,   312,
      33,   512,    34,  -140,    98,   154,   320,   503,   477,   187,
      99,   274,   155,    33,   437,    34,   156,   251,   157,   187,
     166,   167,   335,   336,   209,   344,   345,   446,   321,   315,
     166,   167,   387,   388,   396,   435,   158,    33,    74,    34,
      75,   251,   168,   162,   251,   413,   336,   421,   422,   466,
     257,   467,   257,   159,    76,    77,   248,   480,   336,   481,
     388,   163,   545,   492,   257,   165,   175,   239,    79,    80,
     550,   493,   257,   251,   184,   479,    81,    82,   500,   336,
     501,   388,   187,   187,    83,    84,   191,    85,   358,   519,
     345,   249,    86,   490,   535,   345,   196,    87,   551,   257,
      88,   553,   257,   220,   210,   230,   255,   275,   285,   299,
     286,   302,   499,   309,    89,    90,   310,   311,   313,   330,
     490,   323,    91,   328,   333,    92,   340,   341,   342,   343,
     359,   382,   389,   390,   393,   274,   395,   398,   399,   401,
      93,   400,   498,   403,   404,   490,   406,   409,   407,   410,
      94,   416,   411,   417,    95,   178,   429,    96,    97,   428,
     431,   274,   432,   434,   438,   451,   442,    98,    33,    74,
      34,    75,   525,    99,   201,   445,   452,   455,   100,   250,
     458,   469,   472,   473,   534,    76,    77,   483,   484,   485,
     476,   486,   491,   495,   336,   496,   505,   506,   502,    79,
      80,   510,   187,   524,   528,   518,   533,    81,    82,   530,
     187,   532,   531,   547,   536,    83,    84,   537,    85,   549,
     548,   544,   546,    86,   554,   556,   555,   202,    87,   280,
     436,    88,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   419,   166,   167,    89,    90,   337,   124,   183,
     190,   125,   241,    91,   450,   526,    92,   504,   552,   527,
     375,   243,   376,   281,   244,   177,   218,     0,   306,   228,
       0,    93,     0,   307,    33,    74,    34,    75,    10,     0,
     201,    94,     0,     0,     1,    95,     2,     0,    96,    97,
       0,    76,    77,     0,     0,     0,     0,     0,    98,   297,
       0,     0,     0,     0,    99,    79,    80,     0,     0,   203,
       0,     0,     0,    81,    82,     3,     0,     0,     0,     0,
       0,    83,    84,     0,    85,     0,     0,     0,     0,    86,
       0,     4,     5,   202,    87,     0,     0,    88,     0,     6,
       0,     0,     0,     7,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    33,    74,    34,    75,     0,     0,    91,
       0,     0,    92,     8,     0,     0,     0,     0,     0,     0,
      76,    77,    78,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,    79,    80,     0,    94,     9,     0,
       0,    95,    81,    82,    96,    97,     0,    10,     0,     0,
      83,    84,     0,    85,    98,     0,     0,     0,    86,     0,
      99,     0,     0,    87,    11,   203,    88,     0,     0,     0,
       0,    12,     0,     0,    13,     0,     0,     0,     0,     0,
      89,    90,    33,    74,    34,    75,     0,     0,    91,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,    76,
      77,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,    79,    80,     0,    94,     0,     0,     0,
      95,    81,    82,    96,    97,     0,     0,     0,     0,    83,
      84,     0,    85,    98,     0,     0,     0,    86,     0,    99,
       0,     0,    87,     0,   100,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    33,    74,    34,    75,     0,     0,    91,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,    76,    77,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,    79,    80,     0,    94,     0,     0,     0,    95,
      81,    82,    96,    97,     0,     0,     0,     0,    83,    84,
       0,    85,    98,   149,     0,     0,    86,     0,    99,     0,
       0,    87,     0,   100,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
       0,     0,     0,     0,     0,     0,    91,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    33,    74,    34,
      75,    10,     0,     0,    94,     0,     0,     0,    95,     0,
       0,    96,    97,     0,    76,    77,     0,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,    99,    79,    80,
       0,     0,   100,     0,     0,     0,    81,    82,     0,     0,
       0,     0,     0,     0,    83,    84,     0,    85,     0,     0,
       0,     0,    86,     0,     0,     0,   202,    87,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,    33,    74,    34,    75,
       0,     0,    91,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,    76,   176,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,    79,    80,     0,
      94,     0,     0,     0,    95,    81,    82,    96,    97,     0,
       0,     0,     0,    83,    84,     0,    85,    98,     0,     0,
       0,    86,     0,    99,     0,     0,    87,     0,   203,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    33,    74,    34,    75,     0,
       0,    91,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,    76,    77,     0,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,    79,    80,     0,    94,
       0,     0,     0,    95,    81,    82,    96,    97,     0,     0,
       0,     0,    83,    84,     0,    85,    98,     0,     0,     0,
      86,     0,    99,     0,     0,    87,     0,   100,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,     0,    74,     0,    75,     0,     0,
      91,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,    76,   176,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,    79,    80,     0,    94,     0,
       0,     0,    95,     0,    82,    96,    97,     0,     0,     0,
       0,    83,    84,     0,    85,    98,     0,     0,     0,    86,
       0,    99,     0,     0,     0,     0,   100,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,     0,     0,     0,     0,     0,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,    96,    97,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
      99
};

static const yytype_int16 yycheck[] =
{
       4,   145,   149,    12,    29,   163,   210,    11,    12,    44,
     277,   141,     4,   249,     6,    23,    24,    29,   120,   149,
       8,     7,     4,     8,     6,     4,    29,     6,    32,     7,
      59,     8,    27,    37,    38,    39,    40,    41,     8,   233,
      21,    22,   192,     5,    73,   238,    81,    21,    22,   389,
      21,    22,    78,    36,    71,    10,   203,     4,     5,     6,
       7,   163,    67,    68,    50,   100,   469,     4,   136,     6,
      31,   201,    50,   203,    74,   233,    21,    22,     4,   147,
       6,    28,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,   495,    21,    22,    28,   122,    39,     5,    60,
     202,   141,   106,   122,   144,   445,   106,    10,    37,   126,
     119,    40,    41,    66,   118,   119,   122,   121,    65,   123,
      69,   146,   157,   126,    83,    58,   138,    61,   278,   323,
     165,   233,   114,    86,   327,   139,   140,    92,   123,   122,
     148,   145,    45,   129,   179,   275,   382,    94,    95,    77,
     145,   129,    55,   145,   135,   285,   286,   119,   146,   163,
     189,   428,   366,   145,   168,   323,   145,    71,   203,   146,
     406,   175,   146,   409,   178,   146,   146,   114,   123,   214,
     127,    45,   217,   188,   388,   389,   128,   116,   192,    92,
     194,   195,   196,   340,     0,   297,    48,   146,   145,   343,
     103,   135,   438,   145,    30,   108,    70,   164,    53,   353,
     340,   118,    61,   120,   249,    17,    18,    19,   146,   106,
     255,   323,   141,   180,   143,   112,     3,   130,    80,   233,
     234,    76,    46,    47,   238,    35,    36,   115,     0,   103,
      33,   445,     4,   401,    44,    90,    91,    46,    47,   253,
     147,    51,    52,   257,    54,   146,   147,   146,   147,    59,
     407,   296,   114,   298,    26,   145,   130,    67,   113,   304,
     274,   275,   416,   417,   278,    77,   311,   407,   380,    21,
      22,    81,    82,   114,   114,   432,   135,   146,   147,    89,
      72,    53,   422,    83,   139,    21,    22,    79,   138,   401,
       9,   336,   432,   133,   145,    87,   315,   107,    34,    21,
      22,   315,    21,    22,    95,   145,   104,   117,   136,   323,
       7,    72,     7,   327,   124,   125,   145,   109,    79,    71,
       4,    57,     6,    84,   134,    82,    87,   484,     9,   343,
     140,   345,   145,     4,   379,     6,     5,   382,   145,   353,
      21,    22,   146,   147,   484,   146,   147,   392,   109,    33,
      21,    22,   146,   147,   146,   369,   145,     4,     5,     6,
       7,   406,    33,     5,   409,   146,   147,   146,   147,   146,
     147,   146,   147,    82,    21,    22,    23,   146,   147,   146,
     147,    71,   536,   146,   147,   147,    98,   401,    35,    36,
     544,   146,   147,   438,   145,   440,    43,    44,   146,   147,
     146,   147,   416,   417,    51,    52,     7,    54,   422,   146,
     147,    58,    59,   458,   146,   147,   145,    64,   146,   147,
      67,   146,   147,     7,   145,     7,    11,   145,     8,    20,
       9,   147,   477,    62,    81,    82,   146,    71,   146,    99,
     485,   147,    89,    73,   146,    92,   145,    85,    85,   145,
     132,   145,   145,   133,    62,   469,     7,    97,    97,    84,
     107,    97,   476,     7,     7,   510,    42,    42,    75,   146,
     117,   145,    96,   145,   121,    27,   138,   124,   125,   147,
      92,   495,   145,    85,   145,   101,    92,   134,     4,     5,
       6,     7,   506,   140,    10,   145,   101,    96,   145,   146,
      42,   145,    38,    99,   518,    21,    22,     7,    95,    42,
     145,    88,    19,   145,   147,    42,     7,   137,   146,    35,
      36,   147,   536,   146,    93,   108,     9,    43,    44,   102,
     544,   102,   110,    68,   145,    51,    52,    33,    54,    68,
     110,   145,   145,    59,   100,     7,   146,    63,    64,   200,
     372,    67,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,   345,    21,    22,    81,    82,   253,    55,   119,
     125,    56,   165,    89,   401,   508,    92,   485,   546,   510,
     285,   169,   286,   201,   174,   116,   151,    -1,   215,   156,
      -1,   107,    -1,   215,     4,     5,     6,     7,   114,    -1,
      10,   117,    -1,    -1,     1,   121,     3,    -1,   124,   125,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,   134,    77,
      -1,    -1,    -1,    -1,   140,    35,    36,    -1,    -1,   145,
      -1,    -1,    -1,    43,    44,    32,    -1,    -1,    -1,    -1,
      -1,    51,    52,    -1,    54,    -1,    -1,    -1,    -1,    59,
      -1,    48,    49,    63,    64,    -1,    -1,    67,    -1,    56,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,     4,     5,     6,     7,    -1,    -1,    89,
      -1,    -1,    92,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    23,    -1,    -1,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    36,    -1,   117,   105,    -1,
      -1,   121,    43,    44,   124,   125,    -1,   114,    -1,    -1,
      51,    52,    -1,    54,   134,    -1,    -1,    -1,    59,    -1,
     140,    -1,    -1,    64,   131,   145,    67,    -1,    -1,    -1,
      -1,   138,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      81,    82,     4,     5,     6,     7,    -1,    -1,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    36,    -1,   117,    -1,    -1,    -1,
     121,    43,    44,   124,   125,    -1,    -1,    -1,    -1,    51,
      52,    -1,    54,   134,    -1,    -1,    -1,    59,    -1,   140,
      -1,    -1,    64,    -1,   145,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      82,     4,     5,     6,     7,    -1,    -1,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    36,    -1,   117,    -1,    -1,    -1,   121,
      43,    44,   124,   125,    -1,    -1,    -1,    -1,    51,    52,
      -1,    54,   134,   135,    -1,    -1,    59,    -1,   140,    -1,
      -1,    64,    -1,   145,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,    -1,     4,     5,     6,
       7,   114,    -1,    -1,   117,    -1,    -1,    -1,   121,    -1,
      -1,   124,   125,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,   134,    -1,    -1,    -1,    -1,    -1,   140,    35,    36,
      -1,    -1,   145,    -1,    -1,    -1,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    52,    -1,    54,    -1,    -1,
      -1,    -1,    59,    -1,    -1,    -1,    63,    64,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,     4,     5,     6,     7,
      -1,    -1,    89,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,
     117,    -1,    -1,    -1,   121,    43,    44,   124,   125,    -1,
      -1,    -1,    -1,    51,    52,    -1,    54,   134,    -1,    -1,
      -1,    59,    -1,   140,    -1,    -1,    64,    -1,   145,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,     4,     5,     6,     7,    -1,
      -1,    89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,   117,
      -1,    -1,    -1,   121,    43,    44,   124,   125,    -1,    -1,
      -1,    -1,    51,    52,    -1,    54,   134,    -1,    -1,    -1,
      59,    -1,   140,    -1,    -1,    64,    -1,   145,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    -1,     5,    -1,     7,    -1,    -1,
      89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    36,    -1,   117,    -1,
      -1,    -1,   121,    -1,    44,   124,   125,    -1,    -1,    -1,
      -1,    51,    52,    -1,    54,   134,    -1,    -1,    -1,    59,
      -1,   140,    -1,    -1,    -1,    -1,   145,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,
     140
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,     3,    32,    48,    49,    56,    60,    80,   105,
     114,   131,   138,   141,   150,   151,   152,   153,   154,   155,
     156,   178,   179,   183,   184,   187,   189,   192,   193,   194,
     259,   260,   122,     4,     6,   192,   257,    78,   122,    71,
     122,    83,    28,    58,   195,   257,   190,   191,   206,   257,
       0,   141,   143,    48,    80,   192,    29,   138,   188,    30,
     141,   144,     3,   257,   126,   181,    71,   180,   181,   257,
     257,   257,   257,   257,     5,     7,    21,    22,    23,    35,
      36,    43,    44,    51,    52,    54,    59,    64,    67,    81,
      82,    89,    92,   107,   117,   121,   124,   125,   134,   140,
     145,   159,   196,   197,   198,   200,   232,   233,   234,   235,
     236,   237,   238,   239,   246,   250,   253,   257,   115,   147,
      33,   145,   192,    83,   188,   195,   104,   194,    31,    60,
       5,   118,   120,   138,   182,     5,   119,   182,   182,    95,
     145,   136,   226,   227,   133,   145,   194,     7,     7,   135,
     232,   242,   243,   145,    82,   145,     5,   145,   145,    82,
     192,   232,     5,    71,   199,   147,    21,    22,    33,   254,
     257,    23,    24,   148,   255,    98,    22,   235,    27,   145,
     185,   186,   257,   191,   145,   200,   256,   257,   181,   257,
     193,     7,    46,    47,    46,    47,   145,   257,   157,   158,
     257,    10,    63,   145,   228,   229,   230,   231,   232,   250,
     145,   249,   256,   228,   135,   240,   241,    61,   243,   244,
       7,    53,    76,    90,    91,   113,   139,   247,   247,   232,
       7,   146,   146,   145,   200,   203,   204,   207,   236,   257,
     226,   198,   257,   233,   234,   145,   257,   257,    23,    58,
     146,   232,   245,   147,   226,    11,   146,   147,   182,   194,
     157,    45,    70,   103,   130,   163,   257,   257,   170,   171,
     172,   173,   174,   175,   257,   145,   166,   146,   147,    82,
     159,   231,   200,   228,   232,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    77,   252,    20,
     246,   248,   147,   146,   123,   232,   241,   244,   232,    62,
     146,    71,    71,   146,   203,    33,   205,   206,    72,    79,
      87,   109,   202,   147,   205,    39,   128,   201,    73,   208,
      99,   215,   216,   146,   245,   146,   147,   186,   232,   257,
     145,    85,    85,   145,   146,   147,     5,     7,    28,    65,
      94,    95,   127,   145,   257,   258,   250,   251,   257,   132,
     163,   164,   165,   157,    10,    45,    55,    92,   103,   108,
     130,   160,   161,   162,   146,   229,   230,    17,    18,    19,
      77,   232,   145,   200,   232,    10,    92,   146,   147,   145,
     133,   232,   123,    62,   232,     7,   146,   206,    97,    97,
      97,    84,   203,     7,     7,   205,    42,    75,   209,    42,
     146,    96,   217,   146,   232,   228,   145,   145,   256,   171,
     256,   146,   147,    37,    40,    41,   116,   176,   147,   138,
     167,    92,   145,   246,    85,   257,   160,   232,   145,   200,
       9,   245,    92,   246,   248,   145,   232,   146,    69,   146,
     204,   101,   101,   245,   228,    96,   210,   245,    42,   106,
     112,   218,   219,   146,   256,   256,   146,   146,   250,   145,
     177,   163,    38,    99,   168,   228,   145,     9,   245,   232,
     146,   146,   248,     7,    95,    42,    88,   211,   222,   223,
     232,    19,   146,   146,   170,   145,    42,   146,   257,   232,
     146,   146,   146,   228,   222,     7,   137,   212,   213,   214,
     147,    34,    57,   224,     7,    50,   129,   220,   108,   146,
     170,    74,   106,   169,   146,   257,   214,   223,    93,   225,
     102,   110,   102,     9,   257,   146,   145,    33,    66,    86,
       7,    50,   129,   221,   145,   256,   145,    68,   110,    68,
     256,   146,   215,   146,   100,   146,     7
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
     180,   180,   181,   181,   181,   182,   182,   183,   184,   185,
     185,   186,   187,   187,   188,   188,   189,   190,   190,   191,
     192,   192,   193,   193,   194,   195,   195,   195,   196,   196,
     197,   197,   198,   198,   198,   199,   200,   201,   201,   201,
     202,   202,   202,   202,   202,   202,   202,   202,   203,   203,
     204,   204,   204,   204,   204,   204,   205,   205,   206,   206,
     207,   207,   208,   208,   209,   209,   210,   210,   211,   211,
     212,   212,   213,   213,   214,   215,   216,   216,   217,   217,
     218,   218,   219,   219,   220,   220,   220,   221,   221,   221,
     222,   222,   223,   224,   224,   224,   225,   225,   225,   226,
     226,   227,   228,   228,   229,   229,   230,   230,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   232,
     232,   233,   233,   234,   234,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   236,   236,   236,   236,   237,
     238,   238,   239,   239,   240,   240,   241,   242,   242,   243,
     244,   244,   245,   245,   246,   246,   246,   246,   246,   246,
     246,   246,   247,   247,   247,   247,   247,   247,   248,   248,
     249,   249,   250,   250,   251,   251,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   253,   254,   254,   255,
     255,   255,   256,   256,   257,   257,   258,   258,   258,   258,
     259,   260,   260
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
       0,     3,    10,     5,     4,     5,     4,     4,     4,     5,
       2,     2,     2,     2,     2,     0,     4,     5,     4,     3,
       1,     3,     2,     3,     0,     3,     2,     1,     3,     3,
       4,     1,     3,     1,    10,     0,     1,     1,     1,     1,
       1,     3,     3,     2,     1,     2,     3,     0,     3,     3,
       0,     1,     1,     2,     1,     2,     1,     2,     6,     1,
       2,     3,     2,     2,     1,     3,     1,     2,     1,     4,
       1,     3,     0,     3,     0,     2,     0,     3,     0,     2,
       0,     1,     1,     2,     6,     3,     0,     3,     0,     3,
       0,     5,     1,     1,     2,     2,     2,     2,     2,     2,
       1,     3,     3,     0,     1,     1,     0,     2,     2,     0,
       1,     2,     3,     1,     3,     1,     2,     1,     5,     6,
       4,     3,     3,     3,     2,     3,     5,     4,     6,     3,
       1,     3,     1,     2,     1,     1,     1,     1,     3,     5,
       1,     1,     1,     3,     1,     3,     4,     4,     5,     6,
       6,     8,     5,     4,     1,     2,     4,     1,     2,     4,
       0,     2,     1,     3,     1,     1,     2,     2,     1,     2,
       3,     2,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     5,     1,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       2,     2,     0
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
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2001 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_NAME  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2011 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2021 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2031 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 7: /* TOKEN_UNSIGNED_NUMVAL  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2041 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 151: /* sql_statement  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2051 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 152: /* quit_statement  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 2061 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 153: /* alter_table_statement  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2071 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* create_table_statement  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 2081 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* create_index_statement  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2091 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* drop_table_statement  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 2101 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* column_def  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 2111 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* column_def_commalist  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 2121 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* data_type  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 2131 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* column_constraint_def  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 2141 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* column_constraint_def_list  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2151 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* opt_column_constraint_def_list  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2161 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 166: /* opt_column_list  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2171 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 167: /* opt_block_properties  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).block_properties_) != nullptr) {
    delete ((*yyvaluep).block_properties_);
  }
}
#line 2181 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* opt_partition_clause  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).partition_clause_) != nullptr) {
    delete ((*yyvaluep).partition_clause_);
  }
}
#line 2191 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* partition_type  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2201 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* key_value_list  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2211 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* key_value  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 2221 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* key_string_value  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 2231 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* key_string_list  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 2241 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* key_integer_value  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 2251 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* key_bool_value  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_bool_value_) != nullptr) {
    delete ((*yyvaluep).key_bool_value_);
  }
}
#line 2261 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* index_type  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2271 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* opt_index_properties  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2281 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* insert_statement  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 2291 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* copy_statement  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_statement_) != nullptr) {
    delete ((*yyvaluep).copy_statement_);
  }
}
#line 2301 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* copy_from_target  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2311 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* copy_to_target  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2321 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* opt_copy_params  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2331 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* update_statement  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2341 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* delete_statement  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2351 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* assignment_list  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2361 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* assignment_item  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2371 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* set_operation_statement  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_statement_) != nullptr) {
    delete ((*yyvaluep).set_operation_statement_);
  }
}
#line 2381 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* opt_priority_clause  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_priority_clause_) != nullptr) {
    delete ((*yyvaluep).opt_priority_clause_);
  }
}
#line 2391 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* with_clause  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2401 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 190: /* with_list  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2411 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 191: /* with_list_element  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2421 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 192: /* set_operation_union  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_) != nullptr) {
    delete ((*yyvaluep).set_operation_);
  }
}
#line 2431 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 193: /* set_operation_intersect  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_) != nullptr) {
    delete ((*yyvaluep).set_operation_);
  }
}
#line 2441 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 194: /* select_query  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2451 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 195: /* opt_all_distinct  */
#line 631 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2457 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 196: /* selection  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2467 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 197: /* selection_item_commalist  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2477 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 198: /* selection_item  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2487 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 199: /* from_clause  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2497 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 200: /* subquery_expression  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2507 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 201: /* opt_sample_clause  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_sample_clause_) != nullptr) {
    delete ((*yyvaluep).opt_sample_clause_);
  }
}
#line 2517 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 202: /* join_type  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2523 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 203: /* joined_table_reference  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2533 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 204: /* table_reference  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2543 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 205: /* table_reference_signature  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2553 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 206: /* table_reference_signature_primary  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2563 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 207: /* joined_table_reference_commalist  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2573 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 208: /* opt_group_by_clause  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2583 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 209: /* opt_having_clause  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2593 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 210: /* opt_order_by_clause  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2603 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 211: /* opt_limit_clause  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2613 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 212: /* opt_window_clause  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_window_clause_) != nullptr) {
    delete ((*yyvaluep).opt_window_clause_);
  }
}
#line 2623 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 213: /* window_declaration_list  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_window_clause_) != nullptr) {
    delete ((*yyvaluep).opt_window_clause_);
  }
}
#line 2633 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 214: /* window_declaration  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_definition_) != nullptr) {
    delete ((*yyvaluep).window_definition_);
  }
}
#line 2643 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 215: /* window_definition  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_definition_) != nullptr) {
    delete ((*yyvaluep).window_definition_);
  }
}
#line 2653 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 216: /* opt_window_partition  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_partition_by_list_) != nullptr) {
    delete ((*yyvaluep).window_partition_by_list_);
  }
}
#line 2663 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 217: /* opt_window_order  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_order_by_list_) != nullptr) {
    delete ((*yyvaluep).window_order_by_list_);
  }
}
#line 2673 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 218: /* opt_window_frame  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_frame_info_) != nullptr) {
    delete ((*yyvaluep).window_frame_info_);
  }
}
#line 2683 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 219: /* frame_mode  */
#line 631 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2689 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 220: /* frame_preceding  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2699 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 221: /* frame_following  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2709 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 222: /* order_commalist  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2719 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 223: /* order_item  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2729 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 224: /* opt_order_direction  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2739 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 225: /* opt_nulls_first  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2749 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 226: /* opt_where_clause  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2759 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 227: /* where_clause  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2769 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 228: /* or_expression  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2779 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 229: /* and_expression  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2789 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 230: /* not_expression  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2799 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 231: /* predicate_expression_base  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2809 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 232: /* add_expression  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2819 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 233: /* multiply_expression  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2829 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 234: /* unary_expression  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2839 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 235: /* expression_base  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2849 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 236: /* function_call  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2859 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 237: /* extract_function  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2869 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 238: /* substr_function  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2879 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 239: /* case_expression  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2889 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 240: /* simple_when_clause_list  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_list_);
  }
}
#line 2899 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 241: /* simple_when_clause  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_);
  }
}
#line 2909 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 242: /* searched_when_clause_list  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_list_);
  }
}
#line 2919 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 243: /* searched_when_clause  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_);
  }
}
#line 2929 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 244: /* opt_else_clause  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2939 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 245: /* expression_list  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2949 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 246: /* literal_value  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2959 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 247: /* datetime_unit  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2969 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 248: /* literal_value_commalist  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2979 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 249: /* literal_value_commalist_multiple  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_multiple_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_multiple_);
  }
}
#line 2989 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 250: /* attribute_ref  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2999 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 251: /* attribute_ref_list  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 3009 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 252: /* comparison_operation  */
#line 632 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 3015 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 253: /* unary_operation  */
#line 633 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 3021 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 254: /* add_operation  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 3027 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 255: /* multiply_operation  */
#line 634 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 3033 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 256: /* name_commalist  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 3043 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 257: /* any_name  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 3053 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 258: /* boolean_value  */
#line 631 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 3059 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 259: /* command  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_) != nullptr) {
    delete ((*yyvaluep).command_);
  }
}
#line 3069 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 260: /* command_argument_list  */
#line 637 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_argument_list_) != nullptr) {
    delete ((*yyvaluep).command_argument_list_);
  }
}
#line 3079 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 646 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3376 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 650 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3385 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 654 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3394 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 5:
#line 658 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3403 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 662 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    YYABORT;
  }
#line 3411 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 665 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 3420 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 672 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3428 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 9:
#line 675 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].copy_statement_);
  }
#line 3436 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 10:
#line 678 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 3444 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 11:
#line 681 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3452 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 12:
#line 684 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 3460 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 13:
#line 687 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 3468 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 14:
#line 690 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 3476 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 15:
#line 693 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 3484 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 16:
#line 696 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].set_operation_statement_);
  }
#line 3492 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 17:
#line 699 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 3500 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 18:
#line 705 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3508 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 19:
#line 711 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3520 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 20:
#line 718 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3531 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 724 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3543 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 731 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3555 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 740 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].attribute_definition_list_), (yyvsp[-1].block_properties_), (yyvsp[0].partition_clause_));
  }
#line 3563 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 745 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].key_value_list_)) {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_), (yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].key_value_list_));
    } else {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_));
    }
  }
#line 3575 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 754 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 3583 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 759 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 3591 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 764 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3600 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 768 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3609 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 774 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 3619 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 779 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDate));
  }
#line 3627 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 782 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3635 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 785 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3645 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 790 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3653 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 793 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3661 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 796 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3669 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 799 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3677 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 802 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3685 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 805 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3693 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 808 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3701 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 811 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3709 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 814 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3717 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 817 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3734 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 829 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3742 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 832 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3750 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 835 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3773 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 853 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3796 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 873 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3804 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 876 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3812 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 879 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3822 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 884 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3832 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 889 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3843 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 895 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3854 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 901 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3866 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 910 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3875 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 914 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3884 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 920 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3892 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 923 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3900 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 928 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3910 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 933 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3920 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 938 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3932 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 945 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3942 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 952 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3951 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 956 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3960 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 962 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3968 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 965 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = $1; */
  }
#line 3976 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 970 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = nullptr;
  }
#line 3984 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 973 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-1].attribute_list_);
  }
#line 3992 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 978 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = nullptr;
  }
#line 4000 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 981 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].key_value_list_));
  }
#line 4008 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 986 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.partition_clause_) = nullptr;
  }
#line 4016 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 989 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4038 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 1008 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::kHashPartitionType);
  }
#line 4046 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 1011 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::kRangePartitionType);
  }
#line 4054 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 1016 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 4063 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 1020 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 4072 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 1026 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 4080 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 1029 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 4088 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 1032 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 4096 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 1035 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_bool_value_);
  }
#line 4104 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 1040 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 4112 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 1043 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 4120 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 1046 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_),
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 4130 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 1053 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4138 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 1058 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 4152 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 1069 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_bool_value_) = new quickstep::ParseKeyBoolValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].boolean_value_));
  }
#line 4160 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 1074 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Defaults to BitWeavingV, but IndexProperties can change this to H.
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBitWeavingV));
  }
#line 4170 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 1079 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
#line 4179 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 1083 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
#line 4188 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 1087 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kSMA));
  }
#line 4197 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 1093 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 4205 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 1096 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 4213 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 1102 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 4226 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 1110 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertTuple((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].literal_value_list_multiple_));
  }
#line 4234 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 1113 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-3]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), nullptr);
  }
#line 4242 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 1116 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-4]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), (yyvsp[-4].with_list_));
  }
#line 4250 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 1122 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-3]).first_line, (yylsp[-3]).first_column,
                                           quickstep::ParseStatementCopy::kFrom,
                                           (yyvsp[-2].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4260 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 1127 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-3]).first_line, (yylsp[-3]).first_column,
                                           quickstep::ParseStatementCopy::kTo,
                                           (yyvsp[-2].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4270 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 1132 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-3]).first_line, (yylsp[-3]).first_column,
                                           (yyvsp[-2].set_operation_), nullptr, (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4279 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 1136 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-4]).first_line, (yylsp[-4]).first_column,
                                           (yyvsp[-2].set_operation_), (yyvsp[-4].with_list_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4288 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 1142 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yyvsp[0].string_value_)->line_number(), (yyvsp[0].string_value_)->column_number(), "@" + (yyvsp[0].string_value_)->value());
    delete (yyvsp[0].string_value_);
  }
#line 4297 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 1146 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "$stdin");
  }
#line 4305 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 1151 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yyvsp[0].string_value_)->line_number(), (yyvsp[0].string_value_)->column_number(), "@" + (yyvsp[0].string_value_)->value());
    delete (yyvsp[0].string_value_);
  }
#line 4314 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 1155 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "$stdout");
  }
#line 4322 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 1158 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "$stderr");
  }
#line 4330 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 1163 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 4338 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 1166 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 4346 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 1171 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 4354 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 1176 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 4362 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 1181 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 4371 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 1185 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 4380 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 1191 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 4388 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 1200 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_statement_) = new quickstep::ParseStatementSetOperation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].set_operation_), nullptr, (yyvsp[0].opt_priority_clause_));
  }
#line 4396 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 1203 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_statement_) = new quickstep::ParseStatementSetOperation((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].set_operation_), (yyvsp[-2].with_list_), (yyvsp[0].opt_priority_clause_));
  }
#line 4404 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 1208 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_priority_clause_) = nullptr;
  }
#line 4412 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 1211 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4434 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 1230 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 4442 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 1235 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4451 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 1239 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4460 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 1245 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 4469 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 1251 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-1].boolean_value_)) {
      (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-3]).first_line, (yylsp[-3]).first_column, quickstep::ParseSetOperation::kUnion);
    } else {
      (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-3]).first_line, (yylsp[-3]).first_column, quickstep::ParseSetOperation::kUnionAll);
    }
    (yyval.set_operation_)->addOperand((yyvsp[-3].set_operation_));
    (yyval.set_operation_)->addOperand((yyvsp[0].set_operation_));
  }
#line 4483 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 1260 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_) = (yyvsp[0].set_operation_);
  }
#line 4491 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 1265 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-2]).first_line, (yylsp[-2]).first_column, quickstep::ParseSetOperation::kIntersect);
    quickstep::ParseSetOperation *op = new quickstep::ParseSetOperation(
        (yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::ParseSetOperation::kSelect);
    op->addOperand((yyvsp[0].select_query_));
    (yyval.set_operation_)->addOperand((yyvsp[-2].set_operation_));
    (yyval.set_operation_)->addOperand(op);
  }
#line 4504 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 1273 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::ParseSetOperation::kSelect);
    (yyval.set_operation_)->addOperand((yyvsp[0].select_query_));
  }
#line 4513 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 1280 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-9]).first_line, (yylsp[-9]).first_column, (yyvsp[-7].selection_), (yyvsp[-6].table_reference_list_), (yyvsp[-5].predicate_), (yyvsp[-4].opt_group_by_clause_), (yyvsp[-3].opt_having_clause_), (yyvsp[-2].opt_order_by_clause_), (yyvsp[-1].opt_limit_clause_), (yyvsp[0].opt_window_clause_));
  }
#line 4521 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 1285 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;  // Distinct
  }
#line 4529 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 1288 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;  // All
  }
#line 4537 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 1291 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;  // Distinct
  }
#line 4545 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 1296 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4553 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 1299 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 4561 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 1304 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4570 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 1308 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4579 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 1314 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 4587 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 1317 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 4595 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 1320 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 4603 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 1325 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[0].table_reference_list_);
  }
#line 4611 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 1330 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].set_operation_));
  }
#line 4619 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 1335 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = NULL;
  }
#line 4627 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 1338 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, true, (yyvsp[-1].numeric_literal_value_));
  }
#line 4635 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 1341 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].numeric_literal_value_));
  }
#line 4643 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 1346 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4651 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 1349 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4659 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 1352 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4667 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 1355 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4675 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 1358 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4683 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 1361 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4691 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 1364 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4699 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 1367 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4707 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 1372 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseJoinedTableReference((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].join_type_), (yyvsp[-5].table_reference_), (yyvsp[-2].table_reference_), (yyvsp[0].predicate_));
  }
#line 4715 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 1375 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = (yyvsp[0].table_reference_);
  }
#line 4723 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 1380 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4732 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 1384 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].opt_sample_clause_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4741 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 1388 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].opt_sample_clause_));
  }
#line 4749 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 1391 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4758 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 1395 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].function_call_));
  }
#line 4766 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 1398 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = (yyvsp[-1].table_reference_);
  }
#line 4774 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 1403 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4782 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 1406 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4790 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 1411 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4798 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 1414 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4806 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 1419 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4815 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 1423 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4824 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 1429 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 4832 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 1432 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4840 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 1437 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4848 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 1440 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4856 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 1445 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4864 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 1448 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4872 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 1453 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4880 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 1456 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4902 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 1475 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = nullptr;
  }
#line 4910 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 1478 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = (yyvsp[0].opt_window_clause_);
  }
#line 4918 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 1483 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = new quickstep::PtrList<quickstep::ParseWindow>();
    (yyval.opt_window_clause_)->push_back((yyvsp[0].window_definition_));
  }
#line 4927 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 1487 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = (yyvsp[-1].opt_window_clause_);
    (yyval.opt_window_clause_)->push_back((yyvsp[0].window_definition_));
  }
#line 4936 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 1493 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_definition_) = (yyvsp[-1].window_definition_);
    (yyval.window_definition_)->setName((yyvsp[-4].string_value_));
  }
#line 4945 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 1499 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_definition_) = new quickstep::ParseWindow((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].window_partition_by_list_), (yyvsp[-1].window_order_by_list_), (yyvsp[0].window_frame_info_));
  }
#line 4953 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 1504 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_partition_by_list_) = nullptr;
  }
#line 4961 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 1507 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_partition_by_list_) = (yyvsp[0].expression_list_);
  }
#line 4969 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 1512 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_order_by_list_) = nullptr;
  }
#line 4977 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 1515 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_order_by_list_) = (yyvsp[0].order_commalist_);
  }
#line 4985 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 1520 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_frame_info_) = nullptr;
  }
#line 4993 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 1523 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_frame_info_) = new quickstep::ParseFrameInfo((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-4].boolean_value_), (yyvsp[-2].numeric_literal_value_)->long_value(), (yyvsp[0].numeric_literal_value_)->long_value());
  }
#line 5001 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 1528 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5009 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 1531 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5017 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 1537 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "-1");
  }
#line 5025 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1540 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "0");
  }
#line 5033 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 1546 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "-1");
  }
#line 5041 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 1549 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "0");
  }
#line 5049 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 1554 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 5058 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 1558 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 5067 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 1564 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 5077 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 1571 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 5085 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 1574 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 5093 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 1577 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 5101 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 1582 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 5109 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 1585 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 5117 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 1588 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 5125 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 1594 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 5133 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1597 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5141 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1602 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5149 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1607 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 5163 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1616 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5171 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1621 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 5185 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1630 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5193 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1635 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 5201 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1638 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5209 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1643 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5217 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1646 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 5227 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1651 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 5238 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1657 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 5249 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1663 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5257 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1666 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 5265 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1669 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateExists((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].subquery_expression_));
  }
#line 5273 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1672 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-2].expression_), (yyvsp[0].subquery_expression_));
  }
#line 5281 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1675 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-1].expression_list_));
  }
#line 5289 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1678 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-2]).first_line,
        (yylsp[-2]).first_column,
        new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-3].expression_), (yyvsp[0].subquery_expression_)));
  }
#line 5300 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1684 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line,
        (yylsp[-4]).first_column,
        new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-1].expression_list_)));
  }
#line 5311 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1693 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5319 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1696 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5327 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1701 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5335 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1704 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5343 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1709 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].unary_operation_), (yyvsp[0].expression_));
  }
#line 5351 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1712 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5359 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1717 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 5367 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1720 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 5375 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1723 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 5383 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1726 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyvsp[-2].function_call_)->setWindowName((yyvsp[0].string_value_));
    (yyval.expression_) = (yyvsp[-2].function_call_);
  }
#line 5392 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1730 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyvsp[-4].function_call_)->setWindow((yyvsp[-1].window_definition_));
    (yyval.expression_) = (yyvsp[-4].function_call_);
  }
#line 5401 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1734 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5409 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1737 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5417 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1740 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5425 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1743 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 5433 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1746 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].subquery_expression_);
  }
#line 5441 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1751 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 5450 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1755 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 5459 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1759 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, false, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 5467 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1762 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-4]).first_line, (yylsp[-4]).first_column, true, (yyvsp[-4].string_value_), (yyvsp[-1].expression_list_));
  }
#line 5475 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1767 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseExtractFunction((yylsp[-5]).first_line, (yylsp[-5]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].expression_));
  }
#line 5483 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1772 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSubstringFunction(
        (yylsp[-5]).first_line, (yylsp[-5]).first_column, (yyvsp[-3].expression_), (yyvsp[-1].numeric_literal_value_)->long_value());
  }
#line 5492 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1776 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSubstringFunction(
        (yylsp[-7]).first_line, (yylsp[-7]).first_column, (yyvsp[-5].expression_), (yyvsp[-3].numeric_literal_value_)->long_value(), (yyvsp[-1].numeric_literal_value_)->long_value());
  }
#line 5501 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1782 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSimpleCaseExpression((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].expression_), (yyvsp[-2].simple_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 5509 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1785 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSearchedCaseExpression((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].searched_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 5517 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1790 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSimpleWhenClause>;
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 5526 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1794 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_list_) = (yyvsp[-1].simple_when_clause_list_);
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 5535 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1800 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_) = new quickstep::ParseSimpleWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5543 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1805 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSearchedWhenClause>;
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 5552 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1809 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_list_) = (yyvsp[-1].searched_when_clause_list_);
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 5561 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1815 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_) = new quickstep::ParseSearchedWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].predicate_), (yyvsp[0].expression_));
  }
#line 5569 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1820 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = NULL;
  }
#line 5577 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1823 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5585 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1828 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 5594 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1832 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 5603 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1838 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 5611 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1841 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5619 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1844 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5627 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1847 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5644 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1859 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 5653 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1863 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5677 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1882 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5695 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1895 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5713 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1910 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("YEAR"));
  }
#line 5721 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1913 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("MONTH"));
  }
#line 5729 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1916 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("DAY"));
  }
#line 5737 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1919 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("HOUR"));
  }
#line 5745 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1922 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("MINUTE"));
  }
#line 5753 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1925 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("SECOND"));
  }
#line 5761 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1930 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5770 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1934 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5779 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1940 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_multiple_) = new quickstep::PtrList<quickstep::PtrList<quickstep::ParseScalarLiteral>>();
    (yyval.literal_value_list_multiple_)->push_back((yyvsp[-1].literal_value_list_));
  }
#line 5788 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1944 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_multiple_) = (yyvsp[-4].literal_value_list_multiple_);
    (yyval.literal_value_list_multiple_)->push_back((yyvsp[-1].literal_value_list_));
  }
#line 5797 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1950 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 5805 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1953 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 5813 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1958 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = new quickstep::PtrList<quickstep::ParseAttribute>();
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5822 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1962 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-2].attribute_list_);
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5831 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1969 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 5839 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1972 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 5847 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1975 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 5855 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1978 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 5863 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 1981 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 5871 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1984 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 5879 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1987 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLike);
  }
#line 5887 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1990 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotLike);
  }
#line 5895 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1993 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kRegexMatch);
  }
#line 5903 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1996 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotRegexMatch);
  }
#line 5911 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 2001 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
#line 5925 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 2012 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
#line 5933 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 2015 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
#line 5941 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 2020 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kModulo);
  }
#line 5949 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 2023 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
#line 5957 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 2026 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
#line 5965 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 2032 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 5974 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 2036 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 5983 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 2042 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5991 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 2045 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 6002 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 2053 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 6010 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 2056 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 6018 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 2059 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 6026 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 2062 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 6034 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 2068 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.command_) = new quickstep::ParseCommand((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].command_argument_list_));
  }
#line 6042 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 2073 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    quickstep::PtrVector<quickstep::ParseString> *argument_list = (yyvsp[-1].command_argument_list_);
    argument_list->push_back((yyvsp[0].string_value_));
    (yyval.command_argument_list_) = argument_list;
  }
#line 6052 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 2078 "../SqlParser.ypp" /* yacc.c:1661  */
    { /* Epsilon, an empy match. */
    (yyval.command_argument_list_) = new quickstep::PtrVector<quickstep::ParseString>();
  }
#line 6060 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;


#line 6064 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 2082 "../SqlParser.ypp" /* yacc.c:1906  */


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
