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
    TOKEN_BY = 291,
    TOKEN_CASE = 292,
    TOKEN_CHARACTER = 293,
    TOKEN_CHECK = 294,
    TOKEN_COLUMN = 295,
    TOKEN_CONSTRAINT = 296,
    TOKEN_COPY = 297,
    TOKEN_CREATE = 298,
    TOKEN_CSB_TREE = 299,
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
    TOKEN_EOF = 313,
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
    TOKEN_LEX_ERROR = 339,
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
    TOKEN_ROWS = 363,
    TOKEN_ROW_DELIMITER = 364,
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
    TOKEN_YEARMONTH = 391
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

#line 436 "SqlParser_gen.cpp" /* yacc.c:355  */
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

#line 471 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYLAST   1392

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  148
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  111
/* YYNRULES -- Number of rules.  */
#define YYNRULES  300
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  555

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   391

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     143,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   147,     2,     2,
     144,   145,    23,    21,   146,    22,    27,    24,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   142,
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
     140,   141
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   644,   644,   648,   652,   656,   660,   663,   670,   673,
     676,   679,   682,   685,   688,   691,   694,   697,   703,   709,
     716,   722,   729,   738,   743,   752,   757,   762,   766,   772,
     777,   780,   783,   788,   791,   794,   797,   800,   803,   806,
     809,   812,   815,   827,   830,   833,   851,   871,   874,   877,
     882,   887,   893,   899,   908,   912,   918,   921,   926,   931,
     936,   943,   950,   954,   960,   963,   968,   971,   976,   979,
     984,   987,  1006,  1009,  1014,  1018,  1024,  1027,  1030,  1033,
    1038,  1041,  1044,  1051,  1056,  1067,  1072,  1077,  1081,  1085,
    1091,  1094,  1100,  1108,  1111,  1114,  1120,  1125,  1130,  1134,
    1140,  1144,  1147,  1152,  1155,  1160,  1165,  1170,  1174,  1180,
    1189,  1192,  1197,  1200,  1219,  1224,  1228,  1234,  1240,  1249,
    1254,  1262,  1268,  1274,  1277,  1280,  1285,  1288,  1293,  1297,
    1303,  1306,  1309,  1314,  1319,  1324,  1327,  1330,  1335,  1338,
    1341,  1344,  1347,  1350,  1353,  1356,  1361,  1364,  1369,  1373,
    1377,  1380,  1384,  1387,  1392,  1395,  1400,  1403,  1408,  1412,
    1418,  1421,  1426,  1429,  1434,  1437,  1442,  1445,  1464,  1467,
    1472,  1476,  1482,  1488,  1493,  1496,  1501,  1504,  1509,  1512,
    1517,  1520,  1525,  1526,  1529,  1534,  1535,  1538,  1543,  1547,
    1553,  1560,  1563,  1566,  1571,  1574,  1577,  1583,  1586,  1591,
    1596,  1605,  1610,  1619,  1624,  1627,  1632,  1635,  1640,  1646,
    1652,  1655,  1658,  1661,  1664,  1667,  1673,  1682,  1685,  1690,
    1693,  1698,  1701,  1706,  1709,  1712,  1715,  1719,  1723,  1726,
    1729,  1732,  1735,  1740,  1744,  1748,  1751,  1756,  1761,  1765,
    1771,  1774,  1779,  1783,  1789,  1794,  1798,  1804,  1809,  1812,
    1817,  1821,  1827,  1830,  1833,  1836,  1848,  1852,  1871,  1884,
    1899,  1902,  1905,  1908,  1911,  1914,  1919,  1923,  1929,  1933,
    1939,  1942,  1947,  1951,  1958,  1961,  1964,  1967,  1970,  1973,
    1976,  1979,  1982,  1985,  1990,  2001,  2004,  2009,  2012,  2015,
    2021,  2025,  2031,  2034,  2042,  2045,  2048,  2051,  2057,  2062,
    2067
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
  "TOKEN_BLOOM_FILTER", "TOKEN_BY", "TOKEN_CASE", "TOKEN_CHARACTER",
  "TOKEN_CHECK", "TOKEN_COLUMN", "TOKEN_CONSTRAINT", "TOKEN_COPY",
  "TOKEN_CREATE", "TOKEN_CSB_TREE", "TOKEN_CURRENT", "TOKEN_DATE",
  "TOKEN_DATETIME", "TOKEN_DAY", "TOKEN_DECIMAL", "TOKEN_DEFAULT",
  "TOKEN_DELETE", "TOKEN_DESC", "TOKEN_DISTINCT", "TOKEN_DOUBLE",
  "TOKEN_DROP", "TOKEN_ELSE", "TOKEN_END", "TOKEN_EOF", "TOKEN_EXISTS",
  "TOKEN_EXTRACT", "TOKEN_FALSE", "TOKEN_FIRST", "TOKEN_FLOAT",
  "TOKEN_FOLLOWING", "TOKEN_FOR", "TOKEN_FOREIGN", "TOKEN_FROM",
  "TOKEN_FULL", "TOKEN_GROUP", "TOKEN_HASH", "TOKEN_HAVING", "TOKEN_HOUR",
  "TOKEN_IN", "TOKEN_INDEX", "TOKEN_INNER", "TOKEN_INSERT",
  "TOKEN_INTEGER", "TOKEN_INTERVAL", "TOKEN_INTO", "TOKEN_JOIN",
  "TOKEN_KEY", "TOKEN_LAST", "TOKEN_LEFT", "TOKEN_LEX_ERROR",
  "TOKEN_LIMIT", "TOKEN_LONG", "TOKEN_MINUTE", "TOKEN_MONTH", "TOKEN_NULL",
  "TOKEN_NULLS", "TOKEN_OFF", "TOKEN_ON", "TOKEN_ORDER", "TOKEN_OUTER",
  "TOKEN_OVER", "TOKEN_PARTITION", "TOKEN_PARTITIONS", "TOKEN_PERCENT",
  "TOKEN_PRECEDING", "TOKEN_PRIMARY", "TOKEN_PRIORITY", "TOKEN_QUIT",
  "TOKEN_RANGE", "TOKEN_REAL", "TOKEN_REFERENCES", "TOKEN_RIGHT",
  "TOKEN_ROW", "TOKEN_ROWS", "TOKEN_ROW_DELIMITER", "TOKEN_SECOND",
  "TOKEN_SELECT", "TOKEN_SET", "TOKEN_SMA", "TOKEN_SMALLINT",
  "TOKEN_STDERR", "TOKEN_STDOUT", "TOKEN_SUBSTRING", "TOKEN_TABLE",
  "TOKEN_THEN", "TOKEN_TIME", "TOKEN_TIMESTAMP", "TOKEN_TO", "TOKEN_TRUE",
  "TOKEN_TUPLESAMPLE", "TOKEN_UNBOUNDED", "TOKEN_UNIQUE", "TOKEN_UPDATE",
  "TOKEN_USING", "TOKEN_VALUES", "TOKEN_VARCHAR", "TOKEN_WHEN",
  "TOKEN_WHERE", "TOKEN_WINDOW", "TOKEN_WITH", "TOKEN_YEAR",
  "TOKEN_YEARMONTH", "';'", "'\\n'", "'('", "')'", "','", "'%'", "$accept",
  "start", "sql_statement", "quit_statement", "alter_table_statement",
  "create_table_statement", "create_index_statement",
  "drop_table_statement", "column_def", "column_def_commalist",
  "data_type", "column_constraint_def", "column_constraint_def_list",
  "opt_column_constraint_def_list", "table_constraint_def",
  "table_constraint_def_commalist", "opt_table_constraint_def_commalist",
  "opt_column_list", "opt_block_properties", "opt_partition_clause",
  "partition_type", "key_value_list", "key_value", "key_string_value",
  "key_string_list", "key_integer_value", "key_bool_value", "index_type",
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
     390,   391,    59,    10,    40,    41,    44,    37
};
# endif

#define YYPACT_NINF -388

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-388)))

#define YYTABLE_NINF -139

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     239,  -388,  -388,   -36,    39,   -27,    28,   -19,  -388,    23,
    -388,    30,   237,   237,   122,     7,  -388,  -388,  -388,  -388,
    -388,  -388,  -388,  -388,  -388,  -388,   154,     3,   156,  -388,
     -14,   187,   237,  -388,  -388,    53,    13,   237,   237,   237,
     237,   237,  -388,  -388,   701,   129,   103,  -388,   226,   121,
    -388,  -388,  -388,   161,   198,     3,    30,   190,  -388,   161,
    -388,  -388,  -388,    43,    16,   166,   288,   166,   211,   168,
     182,  -388,   146,  -388,  -388,   329,   334,  -388,  -388,  -388,
     795,   199,  -388,   270,  -388,  -388,   216,  -388,  -388,   359,
    -388,  -388,  -388,  -388,   235,  -388,  -388,   258,   320,   890,
     410,   352,   279,  -388,  -388,   285,    18,  -388,  -388,   326,
    -388,  -388,  -388,  -388,  -388,  1080,   -17,   237,   237,   283,
     237,    53,   237,  -388,   161,   421,  -388,   163,   177,  -388,
    -388,  -388,   286,  -388,   166,  -388,   237,   237,   607,  -388,
    -388,   287,   237,  -388,  -388,  -388,   607,    57,    62,  -388,
     422,  -388,   141,   141,  1174,   425,  -388,    -2,    29,  -388,
      19,   182,  1174,  -388,  -388,   237,  1174,  -388,  -388,  -388,
    -388,  1174,    20,   334,  -388,   237,   400,    93,  -388,   423,
    -388,   161,  -388,    80,  -388,   166,   161,   156,  -388,   237,
     153,   237,   237,   237,  -388,   293,  -388,   130,  1251,   986,
     283,   511,   430,   431,  -388,  -388,   312,   419,  1238,   298,
     170,    22,  1174,    78,  -388,  1174,  -388,   383,   301,  -388,
    -388,  -388,  -388,  -388,  -388,   375,  -388,   181,   303,  -388,
    -388,    10,    77,   269,  -388,   304,    77,   -20,   379,  -388,
    -388,    18,  -388,   348,  -388,  -388,   310,  1174,  -388,   330,
     213,   237,  -388,  1174,  -388,   237,  -388,  -388,  -388,   313,
     370,   374,   317,  -388,  -388,  -388,   222,  -388,  -388,  -388,
    -388,  -388,    87,   237,   331,   153,   237,  -388,   256,  -388,
    -388,    -1,    50,   607,   607,   219,  -388,  -388,  -388,  -388,
    -388,  -388,  -388,  -388,  1174,   318,  1174,    66,  -388,   224,
     325,   333,  1174,    26,  -388,   409,   330,  -388,  -388,  1174,
     465,  -388,   133,   237,  -388,  -388,   376,  -388,   377,   378,
     388,    19,  -388,   467,   471,    77,   438,   404,   440,   339,
     387,  -388,   227,  -388,  1174,  -388,   330,  -388,   607,   342,
     343,   237,  -388,   237,  -388,  -388,  -388,  -388,  -388,  -388,
    -388,   237,  -388,  -388,  -388,   229,   461,   180,  -388,   344,
     353,  -388,   399,   351,  1238,  -388,   411,   237,  -388,  -388,
     256,  -388,  -388,   431,  -388,  -388,  -388,  1174,   354,   292,
     890,  -388,   330,   402,  -388,  -388,  1238,  1238,   355,   330,
    1174,  -388,    32,    71,  -388,  -388,  -388,  -388,  -388,    19,
     269,   398,   401,  -388,  1174,   607,   405,  1174,  -388,   464,
     142,  -388,   330,    21,   237,   237,   231,  -388,   236,  -388,
     237,  -388,  -388,  -388,  -388,   358,   153,   468,   406,  -388,
     607,  -388,  -388,   364,  -388,   323,   890,  -388,  1174,   238,
    -388,  -388,   240,  1238,   330,  -388,   503,  -388,   414,  -388,
    -388,   367,   430,   473,   433,   367,  1174,  -388,  -388,  -388,
     501,  -388,   243,   246,  -388,  -388,  -388,   237,  -388,  -388,
     380,   486,  -388,    31,   237,  1174,   252,   330,  -388,  -388,
     254,   384,   607,  1174,   521,   393,   390,  -388,    68,    48,
     424,  -388,   263,   237,    44,  -388,   392,   330,  -388,  -388,
    -388,   430,   390,  -388,   237,  -388,   393,  -388,  1174,  -388,
    -388,   444,   436,   437,   439,   533,   237,  -388,   267,  -388,
    -388,   407,  -388,   515,  -388,  -388,   132,  -388,  -388,  -388,
    -388,    49,   408,  -388,   237,   412,  -388,  -388,   481,   443,
     488,  -388,   237,   272,   348,  -388,  -388,  -388,   274,   456,
     415,  -388,   552,  -388,  -388
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     6,   300,     0,     0,     0,     0,     0,     7,     0,
      18,   123,     0,     0,     0,     0,    15,     8,    10,    11,
      13,    14,     9,    17,    12,    16,     0,   112,   119,   121,
       0,   298,     0,   292,   293,     0,     0,     0,     0,     0,
       0,     0,   124,   125,     0,     0,   114,   115,     0,   156,
       1,     3,     2,     0,     0,   112,   123,     0,   110,     0,
       5,     4,   299,     0,     0,   103,     0,   103,     0,     0,
     197,    25,     0,   256,   253,     0,   284,   126,    40,    29,
       0,     0,    30,    31,    34,    36,     0,    37,    39,     0,
      41,   252,    35,    38,     0,    32,    33,     0,     0,     0,
       0,     0,   127,   128,   232,   132,   218,   220,   222,   225,
     228,   229,   230,   224,   223,     0,   270,     0,     0,     0,
       0,     0,     0,   111,     0,     0,   120,     0,     0,   100,
     102,   101,     0,    98,   103,    97,     0,     0,     0,   106,
     198,     0,     0,    94,   254,   255,     0,     0,   248,   245,
       0,    43,     0,   257,     0,     0,    44,     0,     0,   259,
       0,   197,     0,   285,   286,     0,     0,   131,   288,   289,
     287,     0,     0,     0,   221,     0,     0,   197,   108,     0,
     116,     0,   117,     0,   290,   103,     0,   118,   113,     0,
       0,     0,     0,     0,    96,    66,    27,     0,     0,     0,
       0,     0,   199,   201,   203,   205,     0,   223,     0,    93,
       0,     0,     0,   248,   242,     0,   246,     0,     0,   262,
     263,   264,   261,   265,   260,     0,   258,     0,     0,   134,
     231,     0,     0,   158,   147,   133,   152,   135,   160,   129,
     130,   217,   219,   174,   226,   271,     0,     0,   233,   250,
       0,     0,   105,     0,   157,     0,    99,    95,    19,     0,
       0,     0,     0,    20,    21,    22,     0,    74,    76,    77,
      78,    79,     0,     0,     0,    64,     0,    42,    56,   204,
     212,     0,     0,     0,     0,     0,   274,   276,   277,   278,
     279,   275,   280,   282,     0,     0,     0,     0,   266,     0,
       0,     0,     0,     0,   243,     0,   249,   241,    45,     0,
       0,    46,   138,     0,   148,   154,   144,   139,   140,   142,
       0,     0,   151,     0,     0,   150,     0,   162,     0,     0,
     176,   234,     0,   235,     0,   107,   109,   291,     0,     0,
       0,     0,   104,     0,    81,    84,    82,   296,   297,   295,
     294,     0,    80,    85,   272,     0,   270,     0,    63,    65,
      68,    28,     0,     0,     0,    47,     0,     0,    49,    55,
      57,    26,   211,   200,   202,   281,   283,     0,     0,     0,
       0,   213,   210,     0,   209,   268,     0,     0,     0,   247,
       0,   240,     0,     0,   153,   155,   145,   141,   143,     0,
     159,     0,     0,   149,     0,     0,   164,     0,   227,     0,
     178,   236,   251,     0,     0,     0,     0,    75,     0,    67,
       0,    86,    87,    88,    89,    90,     0,     0,    70,    48,
       0,    51,    50,     0,    54,     0,     0,   215,     0,     0,
     208,   267,     0,     0,   244,   237,     0,   238,     0,   136,
     137,   161,   163,     0,   166,   175,     0,   181,   180,   173,
       0,    61,     0,     0,    58,    83,   273,     0,    24,    62,
       0,     0,    23,     0,     0,     0,     0,   206,   214,   269,
       0,     0,     0,     0,     0,   168,   177,   188,   191,     0,
       0,    59,     0,     0,     0,    52,     0,   207,   216,    92,
     239,   146,   165,   167,     0,   122,   169,   170,     0,   192,
     193,   194,     0,     0,     0,     0,     0,    91,     0,    72,
      73,     0,    53,     0,   171,   189,     0,   190,   182,   184,
     183,     0,     0,    69,     0,     0,   195,   196,     0,     0,
       0,   179,     0,     0,   174,   185,   187,   186,     0,     0,
       0,    60,     0,   172,    71
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -388,  -388,  -388,  -388,  -388,  -388,  -388,  -388,  -145,  -388,
     363,   194,  -388,  -388,  -264,  -388,  -388,  -388,  -388,  -388,
    -388,  -387,   223,  -388,  -388,  -388,  -388,  -388,  -388,  -388,
    -388,    61,   -50,  -388,  -388,  -388,   316,  -388,   513,  -388,
    -388,   451,   257,   447,   -41,   516,  -388,  -388,   416,  -388,
    -114,  -388,  -388,  -191,   174,  -214,   -10,  -388,  -388,  -388,
    -388,  -388,  -388,  -388,    74,    33,  -388,  -388,  -388,  -388,
    -388,  -388,    91,    73,  -388,  -388,   -56,  -388,  -142,   299,
     300,   386,   -42,   417,   418,   472,  -154,  -388,  -388,  -388,
    -388,   373,  -388,   442,   382,  -232,  -195,   435,  -292,  -388,
    -126,  -388,  -388,  -388,  -388,  -388,  -141,    -4,  -388,  -388,
    -388
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    14,    15,    16,    17,    18,    19,    20,   196,   197,
     100,   369,   370,   371,   263,   359,   360,   274,   428,   472,
     521,   266,   267,   268,   269,   270,   271,   425,   468,    21,
      22,    65,   133,    23,    24,   177,   178,    25,    58,    26,
      46,    47,   157,    28,    29,    44,   101,   102,   103,   161,
     104,   325,   320,   233,   234,   314,   315,   235,   327,   406,
     454,   485,   505,   506,   507,   329,   330,   410,   459,   460,
     515,   541,   486,   487,   511,   527,   139,   140,   202,   203,
     204,   205,   206,   106,   107,   108,   109,   110,   111,   112,
     213,   214,   148,   149,   217,   250,   113,   225,   299,   209,
     114,   355,   296,   115,   166,   171,   183,   116,   353,    30,
      31
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      36,   210,   105,    48,   211,   182,   236,   283,    45,    49,
     175,   358,   207,   298,    33,   332,    34,   135,   126,   323,
     207,   129,   322,    33,    33,    34,    34,    56,    63,   283,
     283,   143,    56,    68,    69,    70,    71,    72,   147,   283,
     312,   168,   169,    33,   258,    34,   232,   163,   164,    60,
     163,   164,    37,   163,   164,   512,   538,   158,    42,   281,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
      51,   163,   164,   207,   127,   207,   383,   236,   163,   164,
     492,    33,    56,    34,   194,    66,   280,    32,    43,   163,
     164,    33,   344,    34,   345,   442,    38,    67,   513,   539,
      39,   167,   509,   128,    40,   238,   518,    41,    48,   324,
     313,   403,   227,   179,    49,   346,   184,   232,   186,   519,
     105,   252,    50,   215,   176,   510,    11,   176,   295,    61,
     400,   361,   195,   198,   249,   256,   130,   131,   184,   215,
      64,   446,    57,   229,   372,   257,   302,   354,   439,    52,
     390,   480,   520,   347,   231,    11,   237,   207,   207,   282,
     384,   240,   469,   231,   243,   170,   461,   236,   244,   431,
     303,   245,   451,   306,   230,   455,   495,   445,   514,   540,
      64,   381,   185,   348,   349,   198,    59,   264,   265,   272,
      62,   441,   298,   212,   219,   230,   413,   259,   146,   536,
     416,    53,   163,   164,   476,   249,   316,   232,   189,   190,
     418,   336,   207,   317,   212,   350,   447,   421,   220,   537,
     422,   318,   191,   192,   260,   254,   255,   237,    49,   423,
     138,   351,    49,   221,   222,    54,   375,   376,   377,   251,
       1,    33,     2,    34,   319,   236,   117,   179,   298,   118,
     457,   337,   379,   310,   382,   458,   223,    27,   261,   119,
     389,    35,    11,   452,   437,   120,   362,   392,   352,   356,
      11,     3,   198,   462,   463,   275,   276,    11,   394,   207,
     141,   224,   122,    55,   262,   232,     4,     5,   473,    33,
     142,    34,   412,   134,   466,     6,   125,   378,   424,     7,
     363,   438,     8,   395,   207,   132,   163,   164,   136,    49,
     121,   364,   137,   163,   164,   301,   255,   237,   165,   138,
       9,    49,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   475,   163,   164,   435,   144,   184,   249,   272,
     501,   145,   316,   150,   163,   164,    10,   184,   444,   317,
     365,   163,   164,   151,  -138,    11,   207,   318,   333,   334,
     152,   366,   249,   433,   153,   249,   367,   342,   343,   385,
     386,    12,   411,   334,   419,   420,   464,   255,    13,   154,
     319,   465,   255,   478,   334,   479,   386,   368,   490,   255,
     295,   491,   255,   543,   249,   237,   477,   498,   334,   499,
     386,   548,   155,   156,    33,    73,    34,    74,   517,   343,
     184,   184,   533,   343,   488,   159,   356,   549,   255,   551,
     255,    75,    76,   246,   160,   162,   172,   181,   188,   218,
     193,   208,   228,   497,   253,    78,    79,   273,   283,   297,
     284,   488,    80,    81,   300,   307,   308,   309,   311,   328,
     321,    82,    83,   326,    84,   331,   339,   338,   247,    85,
     340,   341,   380,   272,   357,    86,   488,   388,    87,   387,
     496,   391,   393,   399,   401,   396,   397,   398,   402,   404,
     405,   407,    88,    89,   408,   409,   414,   415,   175,   272,
     426,    90,   427,   429,    91,   430,   440,   432,   436,   443,
     523,   449,   467,   453,   450,   456,   470,   471,   474,    92,
     481,   482,   532,   334,   483,    33,    73,    34,    74,    93,
     489,   199,    94,   484,   493,    95,    96,   494,   503,   500,
     184,   504,    75,    76,   516,    97,   508,   522,   184,   526,
     528,    98,   531,   530,    99,   248,    78,    79,   535,   529,
     545,   534,   542,    80,    81,   546,   544,   547,   552,   554,
     553,   278,    82,    83,   434,    84,   417,   335,   123,   180,
      85,   187,   124,   448,   502,   200,    86,   550,   239,    87,
     524,   525,   373,   241,   374,   279,   304,   174,   226,   242,
     216,     0,     0,    88,    89,   305,     0,     0,     0,     0,
       0,     0,    90,     0,     0,    91,     0,     0,     0,     0,
       0,    33,    73,    34,    74,     0,     0,   199,     0,     0,
      92,     0,     0,     0,     0,     0,     0,    11,    75,    76,
      93,     0,     0,    94,     0,     0,    95,    96,     0,     0,
       0,     0,    78,    79,     0,     0,    97,     0,     0,    80,
      81,     0,    98,     0,     0,   201,     0,     0,    82,    83,
       0,    84,     0,     0,     0,     0,    85,     0,     0,     0,
       0,   200,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,     0,     0,     0,     0,     0,     0,     0,    90,     0,
       0,    91,     0,     0,     0,    33,    73,    34,    74,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,    75,    76,    77,     0,    93,     0,     0,    94,
       0,     0,    95,    96,     0,     0,    78,    79,     0,     0,
       0,     0,    97,    80,    81,     0,     0,     0,    98,     0,
       0,   201,    82,    83,     0,    84,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,    86,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,    91,     0,     0,     0,    33,
      73,    34,    74,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,    75,    76,     0,     0,
      93,     0,     0,    94,     0,     0,    95,    96,     0,     0,
      78,    79,     0,     0,     0,     0,    97,    80,    81,     0,
       0,     0,    98,     0,     0,    99,    82,    83,     0,    84,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,     0,
       0,     0,     0,     0,     0,     0,    90,     0,     0,    91,
       0,     0,     0,     0,    33,    73,    34,    74,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,    75,    76,     0,    93,     0,     0,    94,     0,     0,
      95,    96,     0,     0,     0,    78,    79,     0,     0,     0,
      97,   146,    80,    81,     0,     0,    98,     0,     0,    99,
       0,    82,    83,     0,    84,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,    91,     0,     0,     0,     0,     0,
      33,    73,    34,    74,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,    11,    75,    76,    93,
       0,     0,    94,     0,     0,    95,    96,     0,     0,     0,
       0,    78,    79,     0,     0,    97,     0,     0,    80,    81,
       0,    98,     0,     0,    99,     0,     0,    82,    83,     0,
      84,     0,     0,     0,     0,    85,     0,     0,     0,     0,
     200,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
       0,     0,     0,     0,     0,     0,     0,    90,     0,     0,
      91,     0,     0,     0,    33,    73,    34,    74,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,    75,   173,     0,     0,    93,     0,     0,    94,     0,
       0,    95,    96,     0,     0,    78,    79,     0,     0,     0,
       0,    97,    80,    81,     0,     0,     0,    98,     0,     0,
     201,    82,    83,     0,    84,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,     0,     0,     0,     0,     0,     0,
       0,    90,     0,     0,    91,     0,     0,     0,    33,    73,
      34,    74,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,    75,    76,     0,     0,    93,
       0,     0,    94,     0,     0,    95,    96,     0,     0,    78,
      79,     0,     0,     0,     0,    97,    80,    81,     0,     0,
       0,    98,     0,     0,    99,    82,    83,     0,    84,     0,
       0,     0,     0,    85,     0,     0,     0,     0,     0,    86,
       0,     0,    87,    73,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,     0,    75,
     173,     0,     0,     0,     0,    90,     0,     0,    91,     0,
       0,     0,     0,    78,    79,     0,     0,     0,     0,     0,
       0,    81,     0,    92,     0,     0,    78,    79,     0,    82,
      83,     0,    84,    93,    81,     0,    94,    85,     0,    95,
      96,     0,    82,    83,     0,    84,    87,     0,     0,    97,
      85,     0,     0,     0,     0,    98,     0,     0,    99,    87,
      88,    89,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,    91,    88,   277,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      92,     0,     0,    95,    96,     0,     0,     0,     0,     0,
      93,     0,     0,    97,     0,     0,    95,    96,     0,    98,
       0,     0,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,    98
};

static const yytype_int16 yycheck[] =
{
       4,   142,    44,    13,   146,   119,   160,     8,    12,    13,
      27,   275,   138,   208,     4,   247,     6,    67,    59,    39,
     146,     5,   236,     4,     4,     6,     6,    29,    32,     8,
       8,    72,    29,    37,    38,    39,    40,    41,    80,     8,
     231,    23,    24,     4,   189,     6,   160,    21,    22,    63,
      21,    22,    79,    21,    22,     7,     7,    99,    28,   201,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      63,    21,    22,   199,    31,   201,    10,   231,    21,    22,
     467,     4,    29,     6,   134,    72,   200,   123,    58,    21,
      22,     4,     5,     6,     7,   387,   123,    36,    50,    50,
      72,   105,    34,    60,   123,   161,   493,    84,   118,   129,
      33,   325,   154,   117,   118,    28,   120,   231,   122,    75,
     162,   177,     0,    61,   144,    57,   116,   144,    78,   143,
     321,   276,   136,   137,   176,   185,   120,   121,   142,    61,
     127,    70,   139,   145,   145,   186,   124,   273,   380,   142,
     124,   443,   108,    66,   144,   116,   160,   283,   284,   201,
      94,   165,   426,   144,   144,   147,   145,   321,   172,   364,
     212,   175,   404,   215,   145,   407,   145,   145,   130,   130,
     127,   295,   121,    96,    97,   189,    30,   191,   192,   193,
       3,   386,   387,   136,    53,   145,   338,    44,   136,    67,
     341,    47,    21,    22,   436,   247,    73,   321,    45,    46,
     351,   253,   338,    80,   136,   128,   145,    37,    77,    87,
      40,    88,    45,    46,    71,   145,   146,   231,   232,    49,
     137,   144,   236,    92,    93,    81,    17,    18,    19,   146,
       1,     4,     3,     6,   111,   399,   117,   251,   443,   146,
     108,   255,   294,    72,   296,   113,   115,     0,   105,    33,
     302,     4,   116,   405,   378,   144,    10,   309,   272,   273,
     116,    32,   276,   414,   415,   145,   146,   116,   145,   405,
     134,   140,    84,    26,   131,   399,    47,    48,   430,     4,
     144,     6,   334,     5,   420,    56,   106,    78,   118,    60,
      44,     9,    63,   313,   430,   139,    21,    22,    97,   313,
      53,    55,   144,    21,    22,   145,   146,   321,    33,   137,
      81,   325,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,     9,    21,    22,   377,     7,   341,   380,   343,
     482,     7,    73,   144,    21,    22,   107,   351,   390,    80,
      94,    21,    22,    83,    85,   116,   482,    88,   145,   146,
     144,   105,   404,   367,     5,   407,   110,   145,   146,   145,
     146,   132,   145,   146,   145,   146,   145,   146,   139,   144,
     111,   145,   146,   145,   146,   145,   146,   131,   145,   146,
      78,   145,   146,   534,   436,   399,   438,   145,   146,   145,
     146,   542,   144,    83,     4,     5,     6,     7,   145,   146,
     414,   415,   145,   146,   456,     5,   420,   145,   146,   145,
     146,    21,    22,    23,    72,   146,   100,   144,     7,     7,
     144,   144,     7,   475,    11,    35,    36,   144,     8,    20,
       9,   483,    42,    43,   146,    62,   145,    72,   145,   101,
     146,    51,    52,    74,    54,   145,    86,   144,    58,    59,
      86,   144,   144,   467,   133,    65,   508,   134,    68,   144,
     474,    62,     7,    85,     7,    99,    99,    99,     7,    41,
      76,    41,    82,    83,   145,    98,   144,   144,    27,   493,
     146,    91,   139,    94,    94,   144,    94,    86,   144,   144,
     504,   103,   144,    98,   103,    41,    38,   101,   144,   109,
       7,    97,   516,   146,    41,     4,     5,     6,     7,   119,
      19,    10,   122,    90,   144,   125,   126,    41,     7,   145,
     534,   138,    21,    22,   110,   135,   146,   145,   542,    95,
     104,   141,     9,   104,   144,   145,    35,    36,    33,   112,
      69,   144,   144,    42,    43,   112,   144,    69,   102,     7,
     145,   198,    51,    52,   370,    54,   343,   251,    55,   118,
      59,   124,    56,   399,   483,    64,    65,   544,   162,    68,
     506,   508,   283,   166,   284,   199,   213,   115,   153,   171,
     148,    -1,    -1,    82,    83,   213,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,   116,    21,    22,
     119,    -1,    -1,   122,    -1,    -1,   125,   126,    -1,    -1,
      -1,    -1,    35,    36,    -1,    -1,   135,    -1,    -1,    42,
      43,    -1,   141,    -1,    -1,   144,    -1,    -1,    51,    52,
      -1,    54,    -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,
      -1,    64,    65,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    94,    -1,    -1,    -1,     4,     5,     6,     7,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    21,    22,    23,    -1,   119,    -1,    -1,   122,
      -1,    -1,   125,   126,    -1,    -1,    35,    36,    -1,    -1,
      -1,    -1,   135,    42,    43,    -1,    -1,    -1,   141,    -1,
      -1,   144,    51,    52,    -1,    54,    -1,    -1,    -1,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,     4,
       5,     6,     7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
     119,    -1,    -1,   122,    -1,    -1,   125,   126,    -1,    -1,
      35,    36,    -1,    -1,    -1,    -1,   135,    42,    43,    -1,
      -1,    -1,   141,    -1,    -1,   144,    51,    52,    -1,    54,
      -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    -1,     4,     5,     6,     7,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,   119,    -1,    -1,   122,    -1,    -1,
     125,   126,    -1,    -1,    -1,    35,    36,    -1,    -1,    -1,
     135,   136,    42,    43,    -1,    -1,   141,    -1,    -1,   144,
      -1,    51,    52,    -1,    54,    -1,    -1,    -1,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
       4,     5,     6,     7,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    21,    22,   119,
      -1,    -1,   122,    -1,    -1,   125,   126,    -1,    -1,    -1,
      -1,    35,    36,    -1,    -1,   135,    -1,    -1,    42,    43,
      -1,   141,    -1,    -1,   144,    -1,    -1,    51,    52,    -1,
      54,    -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,
      64,    65,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      94,    -1,    -1,    -1,     4,     5,     6,     7,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,   119,    -1,    -1,   122,    -1,
      -1,   125,   126,    -1,    -1,    35,    36,    -1,    -1,    -1,
      -1,   135,    42,    43,    -1,    -1,    -1,   141,    -1,    -1,
     144,    51,    52,    -1,    54,    -1,    -1,    -1,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    94,    -1,    -1,    -1,     4,     5,
       6,     7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   119,
      -1,    -1,   122,    -1,    -1,   125,   126,    -1,    -1,    35,
      36,    -1,    -1,    -1,    -1,   135,    42,    43,    -1,    -1,
      -1,   141,    -1,    -1,   144,    51,    52,    -1,    54,    -1,
      -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,     5,    -1,     7,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    -1,    21,
      22,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    35,    36,    -1,    -1,    -1,    -1,    -1,
      -1,    43,    -1,   109,    -1,    -1,    35,    36,    -1,    51,
      52,    -1,    54,   119,    43,    -1,   122,    59,    -1,   125,
     126,    -1,    51,    52,    -1,    54,    68,    -1,    -1,   135,
      59,    -1,    -1,    -1,    -1,   141,    -1,    -1,   144,    68,
      82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    94,    82,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,
     109,    -1,    -1,   125,   126,    -1,    -1,    -1,    -1,    -1,
     119,    -1,    -1,   135,    -1,    -1,   125,   126,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
      -1,    -1,   141
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,     3,    32,    47,    48,    56,    60,    63,    81,
     107,   116,   132,   139,   149,   150,   151,   152,   153,   154,
     155,   177,   178,   181,   182,   185,   187,   190,   191,   192,
     257,   258,   123,     4,     6,   190,   255,    79,   123,    72,
     123,    84,    28,    58,   193,   255,   188,   189,   204,   255,
       0,    63,   142,    47,    81,   190,    29,   139,   186,    30,
      63,   143,     3,   255,   127,   179,    72,   179,   255,   255,
     255,   255,   255,     5,     7,    21,    22,    23,    35,    36,
      42,    43,    51,    52,    54,    59,    65,    68,    82,    83,
      91,    94,   109,   119,   122,   125,   126,   135,   141,   144,
     158,   194,   195,   196,   198,   230,   231,   232,   233,   234,
     235,   236,   237,   244,   248,   251,   255,   117,   146,    33,
     144,   190,    84,   186,   193,   106,   192,    31,    60,     5,
     120,   121,   139,   180,     5,   180,    97,   144,   137,   224,
     225,   134,   144,   192,     7,     7,   136,   230,   240,   241,
     144,    83,   144,     5,   144,   144,    83,   190,   230,     5,
      72,   197,   146,    21,    22,    33,   252,   255,    23,    24,
     147,   253,   100,    22,   233,    27,   144,   183,   184,   255,
     189,   144,   198,   254,   255,   179,   255,   191,     7,    45,
      46,    45,    46,   144,   180,   255,   156,   157,   255,    10,
      64,   144,   226,   227,   228,   229,   230,   248,   144,   247,
     254,   226,   136,   238,   239,    61,   241,   242,     7,    53,
      77,    92,    93,   115,   140,   245,   245,   230,     7,   145,
     145,   144,   198,   201,   202,   205,   234,   255,   224,   196,
     255,   231,   232,   144,   255,   255,    23,    58,   145,   230,
     243,   146,   224,    11,   145,   146,   180,   192,   156,    44,
      71,   105,   131,   162,   255,   255,   169,   170,   171,   172,
     173,   174,   255,   144,   165,   145,   146,    83,   158,   229,
     198,   226,   230,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    78,   250,    20,   244,   246,
     146,   145,   124,   230,   239,   242,   230,    62,   145,    72,
      72,   145,   201,    33,   203,   204,    73,    80,    88,   111,
     200,   146,   203,    39,   129,   199,    74,   206,   101,   213,
     214,   145,   243,   145,   146,   184,   230,   255,   144,    86,
      86,   144,   145,   146,     5,     7,    28,    66,    96,    97,
     128,   144,   255,   256,   248,   249,   255,   133,   162,   163,
     164,   156,    10,    44,    55,    94,   105,   110,   131,   159,
     160,   161,   145,   227,   228,    17,    18,    19,    78,   230,
     144,   198,   230,    10,    94,   145,   146,   144,   134,   230,
     124,    62,   230,     7,   145,   204,    99,    99,    99,    85,
     201,     7,     7,   203,    41,    76,   207,    41,   145,    98,
     215,   145,   230,   226,   144,   144,   254,   170,   254,   145,
     146,    37,    40,    49,   118,   175,   146,   139,   166,    94,
     144,   244,    86,   255,   159,   230,   144,   198,     9,   243,
      94,   244,   246,   144,   230,   145,    70,   145,   202,   103,
     103,   243,   226,    98,   208,   243,    41,   108,   113,   216,
     217,   145,   254,   254,   145,   145,   248,   144,   176,   162,
      38,   101,   167,   226,   144,     9,   243,   230,   145,   145,
     246,     7,    97,    41,    90,   209,   220,   221,   230,    19,
     145,   145,   169,   144,    41,   145,   255,   230,   145,   145,
     145,   226,   220,     7,   138,   210,   211,   212,   146,    34,
      57,   222,     7,    50,   130,   218,   110,   145,   169,    75,
     108,   168,   145,   255,   212,   221,    95,   223,   104,   112,
     104,     9,   255,   145,   144,    33,    67,    87,     7,    50,
     130,   219,   144,   254,   144,    69,   112,    69,   254,   145,
     213,   145,   102,   145,     7
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   148,   149,   149,   149,   149,   149,   149,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   151,   152,
     152,   152,   152,   153,   154,   155,   156,   157,   157,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   159,   159,   159,
     159,   159,   159,   159,   160,   160,   161,   161,   162,   162,
     162,   162,   163,   163,   164,   164,   165,   165,   166,   166,
     167,   167,   168,   168,   169,   169,   170,   170,   170,   170,
     171,   171,   171,   172,   173,   174,   175,   175,   175,   175,
     176,   176,   177,   177,   177,   177,   178,   178,   178,   178,
     179,   179,   179,   180,   180,   181,   182,   183,   183,   184,
     185,   185,   186,   186,   187,   188,   188,   189,   190,   190,
     191,   191,   192,   193,   193,   193,   194,   194,   195,   195,
     196,   196,   196,   197,   198,   199,   199,   199,   200,   200,
     200,   200,   200,   200,   200,   200,   201,   201,   202,   202,
     202,   202,   202,   202,   203,   203,   204,   204,   205,   205,
     206,   206,   207,   207,   208,   208,   209,   209,   210,   210,
     211,   211,   212,   213,   214,   214,   215,   215,   216,   216,
     217,   217,   218,   218,   218,   219,   219,   219,   220,   220,
     221,   222,   222,   222,   223,   223,   223,   224,   224,   225,
     226,   226,   227,   227,   228,   228,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   230,   230,   231,
     231,   232,   232,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   234,   234,   234,   234,   235,   236,   236,
     237,   237,   238,   238,   239,   240,   240,   241,   242,   242,
     243,   243,   244,   244,   244,   244,   244,   244,   244,   244,
     245,   245,   245,   245,   245,   245,   246,   246,   247,   247,
     248,   248,   249,   249,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   250,   251,   252,   252,   253,   253,   253,
     254,   254,   255,   255,   256,   256,   256,   256,   257,   258,
     258
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
       0,     3,    10,     5,     4,     5,     5,     4,     4,     5,
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
       1,     3,     1,     3,     4,     4,     5,     6,     6,     8,
       5,     4,     1,     2,     4,     1,     2,     4,     0,     2,
       1,     3,     1,     1,     2,     2,     1,     2,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     5,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     2,     2,
       0
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
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2014 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_NAME  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2024 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2034 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2044 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 7: /* TOKEN_UNSIGNED_NUMVAL  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2054 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 150: /* sql_statement  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2064 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 151: /* quit_statement  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 2074 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 152: /* alter_table_statement  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2084 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 153: /* create_table_statement  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 2094 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* create_index_statement  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2104 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* drop_table_statement  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 2114 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* column_def  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 2124 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* column_def_commalist  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 2134 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* data_type  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 2144 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* column_constraint_def  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 2154 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* column_constraint_def_list  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2164 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* opt_column_constraint_def_list  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2174 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 165: /* opt_column_list  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2184 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 166: /* opt_block_properties  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).block_properties_) != nullptr) {
    delete ((*yyvaluep).block_properties_);
  }
}
#line 2194 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 167: /* opt_partition_clause  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).partition_clause_) != nullptr) {
    delete ((*yyvaluep).partition_clause_);
  }
}
#line 2204 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* partition_type  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2214 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* key_value_list  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2224 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* key_value  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 2234 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* key_string_value  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 2244 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* key_string_list  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 2254 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* key_integer_value  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 2264 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* key_bool_value  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_bool_value_) != nullptr) {
    delete ((*yyvaluep).key_bool_value_);
  }
}
#line 2274 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* index_type  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2284 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* opt_index_properties  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2294 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* insert_statement  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 2304 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* copy_statement  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_statement_) != nullptr) {
    delete ((*yyvaluep).copy_statement_);
  }
}
#line 2314 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* copy_to_target  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2324 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* opt_copy_params  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2334 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* update_statement  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2344 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* delete_statement  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2354 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* assignment_list  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2364 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* assignment_item  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2374 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* set_operation_statement  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_statement_) != nullptr) {
    delete ((*yyvaluep).set_operation_statement_);
  }
}
#line 2384 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* opt_priority_clause  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_priority_clause_) != nullptr) {
    delete ((*yyvaluep).opt_priority_clause_);
  }
}
#line 2394 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* with_clause  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2404 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* with_list  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2414 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* with_list_element  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2424 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 190: /* set_operation_union  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_) != nullptr) {
    delete ((*yyvaluep).set_operation_);
  }
}
#line 2434 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 191: /* set_operation_intersect  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_) != nullptr) {
    delete ((*yyvaluep).set_operation_);
  }
}
#line 2444 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 192: /* select_query  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2454 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 193: /* opt_all_distinct  */
#line 629 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2460 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 194: /* selection  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2470 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 195: /* selection_item_commalist  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2480 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 196: /* selection_item  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2490 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 197: /* from_clause  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2500 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 198: /* subquery_expression  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2510 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 199: /* opt_sample_clause  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_sample_clause_) != nullptr) {
    delete ((*yyvaluep).opt_sample_clause_);
  }
}
#line 2520 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 200: /* join_type  */
#line 633 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2526 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 201: /* joined_table_reference  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2536 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 202: /* table_reference  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2546 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 203: /* table_reference_signature  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2556 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 204: /* table_reference_signature_primary  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2566 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 205: /* joined_table_reference_commalist  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2576 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 206: /* opt_group_by_clause  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2586 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 207: /* opt_having_clause  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2596 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 208: /* opt_order_by_clause  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2606 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 209: /* opt_limit_clause  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2616 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 210: /* opt_window_clause  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_window_clause_) != nullptr) {
    delete ((*yyvaluep).opt_window_clause_);
  }
}
#line 2626 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 211: /* window_declaration_list  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_window_clause_) != nullptr) {
    delete ((*yyvaluep).opt_window_clause_);
  }
}
#line 2636 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 212: /* window_declaration  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_definition_) != nullptr) {
    delete ((*yyvaluep).window_definition_);
  }
}
#line 2646 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 213: /* window_definition  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_definition_) != nullptr) {
    delete ((*yyvaluep).window_definition_);
  }
}
#line 2656 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 214: /* opt_window_partition  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_partition_by_list_) != nullptr) {
    delete ((*yyvaluep).window_partition_by_list_);
  }
}
#line 2666 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 215: /* opt_window_order  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_order_by_list_) != nullptr) {
    delete ((*yyvaluep).window_order_by_list_);
  }
}
#line 2676 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 216: /* opt_window_frame  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_frame_info_) != nullptr) {
    delete ((*yyvaluep).window_frame_info_);
  }
}
#line 2686 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 217: /* frame_mode  */
#line 629 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2692 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 218: /* frame_preceding  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2702 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 219: /* frame_following  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2712 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 220: /* order_commalist  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2722 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 221: /* order_item  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2732 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 222: /* opt_order_direction  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2742 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 223: /* opt_nulls_first  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2752 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 224: /* opt_where_clause  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2762 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 225: /* where_clause  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2772 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 226: /* or_expression  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2782 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 227: /* and_expression  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2792 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 228: /* not_expression  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2802 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 229: /* predicate_expression_base  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2812 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 230: /* add_expression  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2822 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 231: /* multiply_expression  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2832 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 232: /* unary_expression  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2842 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 233: /* expression_base  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2852 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 234: /* function_call  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2862 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 235: /* extract_function  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2872 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 236: /* substr_function  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2882 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 237: /* case_expression  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2892 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 238: /* simple_when_clause_list  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_list_);
  }
}
#line 2902 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 239: /* simple_when_clause  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_);
  }
}
#line 2912 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 240: /* searched_when_clause_list  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_list_);
  }
}
#line 2922 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 241: /* searched_when_clause  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_);
  }
}
#line 2932 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 242: /* opt_else_clause  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2942 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 243: /* expression_list  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2952 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 244: /* literal_value  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2962 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 245: /* datetime_unit  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2972 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 246: /* literal_value_commalist  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2982 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 247: /* literal_value_commalist_multiple  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_multiple_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_multiple_);
  }
}
#line 2992 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 248: /* attribute_ref  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 3002 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 249: /* attribute_ref_list  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 3012 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 250: /* comparison_operation  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 3018 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 251: /* unary_operation  */
#line 631 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 3024 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 252: /* add_operation  */
#line 632 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 3030 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 253: /* multiply_operation  */
#line 632 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 3036 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 254: /* name_commalist  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 3046 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 255: /* any_name  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 3056 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 256: /* boolean_value  */
#line 629 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 3062 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 257: /* command  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_) != nullptr) {
    delete ((*yyvaluep).command_);
  }
}
#line 3072 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 258: /* command_argument_list  */
#line 635 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_argument_list_) != nullptr) {
    delete ((*yyvaluep).command_argument_list_);
  }
}
#line 3082 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 644 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3379 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 648 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3388 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 652 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3397 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 5:
#line 656 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3406 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 660 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    YYABORT;
  }
#line 3414 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 663 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 3423 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 670 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3431 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 9:
#line 673 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].copy_statement_);
  }
#line 3439 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 10:
#line 676 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 3447 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 11:
#line 679 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3455 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 12:
#line 682 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 3463 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 13:
#line 685 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 3471 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 14:
#line 688 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 3479 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 15:
#line 691 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 3487 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 16:
#line 694 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].set_operation_statement_);
  }
#line 3495 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 17:
#line 697 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 3503 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 18:
#line 703 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3511 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 19:
#line 709 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3523 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 20:
#line 716 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3534 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 722 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3546 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 729 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3558 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 738 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].attribute_definition_list_), (yyvsp[-1].block_properties_), (yyvsp[0].partition_clause_));
  }
#line 3566 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 743 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].key_value_list_)) {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_), (yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].key_value_list_));
    } else {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_));
    }
  }
#line 3578 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 752 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 3586 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 757 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 3594 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 762 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3603 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 766 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3612 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 772 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 3622 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 777 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDate));
  }
#line 3630 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 780 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3638 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 783 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3648 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 788 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3656 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 791 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3664 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 794 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3672 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 797 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3680 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 800 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3688 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 803 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3696 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 806 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3704 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 809 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3712 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 812 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3720 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 815 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3737 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 827 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3745 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 830 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3753 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 833 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3776 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 851 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3799 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 871 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3807 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 874 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3815 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 877 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3825 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 882 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3835 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 887 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3846 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 893 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3857 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 899 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3869 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 908 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3878 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 912 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3887 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 918 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3895 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 921 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3903 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 926 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3913 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 931 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3923 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 936 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3935 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 943 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3945 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 950 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3954 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 954 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3963 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 960 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3971 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 963 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = $1; */
  }
#line 3979 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 968 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = nullptr;
  }
#line 3987 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 971 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-1].attribute_list_);
  }
#line 3995 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 976 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = nullptr;
  }
#line 4003 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 979 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].key_value_list_));
  }
#line 4011 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 984 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.partition_clause_) = nullptr;
  }
#line 4019 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 987 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4041 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 1006 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::kHashPartitionType);
  }
#line 4049 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 1009 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::kRangePartitionType);
  }
#line 4057 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 1014 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 4066 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 1018 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 4075 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 1024 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 4083 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 1027 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 4091 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 1030 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 4099 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 1033 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_bool_value_);
  }
#line 4107 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 1038 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 4115 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 1041 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 4123 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 1044 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_),
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 4133 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 1051 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4141 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 1056 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 4155 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 1067 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_bool_value_) = new quickstep::ParseKeyBoolValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].boolean_value_));
  }
#line 4163 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 1072 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Defaults to BitWeavingV, but IndexProperties can change this to H.
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBitWeavingV));
  }
#line 4173 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 1077 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
#line 4182 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 1081 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
#line 4191 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 1085 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kSMA));
  }
#line 4200 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 1091 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 4208 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 1094 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 4216 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 1100 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 4229 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 1108 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertTuple((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].literal_value_list_multiple_));
  }
#line 4237 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 1111 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-3]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), nullptr);
  }
#line 4245 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 1114 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-4]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), (yyvsp[-4].with_list_));
  }
#line 4253 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 1120 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-4]).first_line, (yylsp[-4]).first_column,
                                           quickstep::ParseStatementCopy::kFrom,
                                           (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4263 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 1125 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-3]).first_line, (yylsp[-3]).first_column,
                                           quickstep::ParseStatementCopy::kTo,
                                           (yyvsp[-2].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4273 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 1130 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-3]).first_line, (yylsp[-3]).first_column,
                                           (yyvsp[-2].set_operation_), nullptr, (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4282 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 1134 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-4]).first_line, (yylsp[-4]).first_column,
                                           (yyvsp[-2].set_operation_), (yyvsp[-4].with_list_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4291 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 1140 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yyvsp[0].string_value_)->line_number(), (yyvsp[0].string_value_)->column_number(), "@" + (yyvsp[0].string_value_)->value());
    delete (yyvsp[0].string_value_);
  }
#line 4300 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 1144 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "$stdout");
  }
#line 4308 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 1147 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "$stderr");
  }
#line 4316 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 1152 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 4324 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 1155 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 4332 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 1160 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 4340 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 1165 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 4348 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 1170 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 4357 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 1174 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 4366 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 1180 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 4374 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 1189 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_statement_) = new quickstep::ParseStatementSetOperation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].set_operation_), nullptr, (yyvsp[0].opt_priority_clause_));
  }
#line 4382 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 1192 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_statement_) = new quickstep::ParseStatementSetOperation((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].set_operation_), (yyvsp[-2].with_list_), (yyvsp[0].opt_priority_clause_));
  }
#line 4390 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 1197 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_priority_clause_) = nullptr;
  }
#line 4398 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 1200 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4420 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 1219 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 4428 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 1224 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4437 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 1228 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4446 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 1234 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 4455 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 1240 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-1].boolean_value_)) {
      (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-3]).first_line, (yylsp[-3]).first_column, quickstep::ParseSetOperation::kUnion);
    } else {
      (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-3]).first_line, (yylsp[-3]).first_column, quickstep::ParseSetOperation::kUnionAll);
    }
    (yyval.set_operation_)->addOperand((yyvsp[-3].set_operation_));
    (yyval.set_operation_)->addOperand((yyvsp[0].set_operation_));
  }
#line 4469 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 1249 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_) = (yyvsp[0].set_operation_);
  }
#line 4477 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 1254 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-2]).first_line, (yylsp[-2]).first_column, quickstep::ParseSetOperation::kIntersect);
    quickstep::ParseSetOperation *op = new quickstep::ParseSetOperation(
        (yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::ParseSetOperation::kSelect);
    op->addOperand((yyvsp[0].select_query_));
    (yyval.set_operation_)->addOperand((yyvsp[-2].set_operation_));
    (yyval.set_operation_)->addOperand(op);
  }
#line 4490 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 1262 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::ParseSetOperation::kSelect);
    (yyval.set_operation_)->addOperand((yyvsp[0].select_query_));
  }
#line 4499 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 1269 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-9]).first_line, (yylsp[-9]).first_column, (yyvsp[-7].selection_), (yyvsp[-6].table_reference_list_), (yyvsp[-5].predicate_), (yyvsp[-4].opt_group_by_clause_), (yyvsp[-3].opt_having_clause_), (yyvsp[-2].opt_order_by_clause_), (yyvsp[-1].opt_limit_clause_), (yyvsp[0].opt_window_clause_));
  }
#line 4507 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 1274 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;  // Distinct
  }
#line 4515 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 1277 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;  // All
  }
#line 4523 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 1280 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;  // Distinct
  }
#line 4531 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 1285 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4539 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 1288 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 4547 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 1293 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4556 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 1297 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4565 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 1303 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 4573 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 1306 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 4581 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 1309 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 4589 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 1314 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[0].table_reference_list_);
  }
#line 4597 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 1319 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].set_operation_));
  }
#line 4605 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 1324 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = NULL;
  }
#line 4613 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 1327 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, true, (yyvsp[-1].numeric_literal_value_));
  }
#line 4621 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 1330 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].numeric_literal_value_));
  }
#line 4629 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 1335 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4637 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 1338 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4645 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 1341 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4653 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 1344 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4661 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 1347 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4669 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 1350 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4677 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 1353 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4685 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 1356 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4693 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 1361 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseJoinedTableReference((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].join_type_), (yyvsp[-5].table_reference_), (yyvsp[-2].table_reference_), (yyvsp[0].predicate_));
  }
#line 4701 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 1364 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = (yyvsp[0].table_reference_);
  }
#line 4709 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 1369 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4718 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 1373 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].opt_sample_clause_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4727 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 1377 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].opt_sample_clause_));
  }
#line 4735 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 1380 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4744 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 1384 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].function_call_));
  }
#line 4752 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 1387 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = (yyvsp[-1].table_reference_);
  }
#line 4760 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 1392 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4768 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 1395 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4776 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 1400 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4784 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 1403 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4792 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 1408 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4801 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 1412 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4810 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 1418 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 4818 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 1421 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4826 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 1426 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4834 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 1429 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4842 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 1434 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4850 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 1437 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4858 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 1442 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4866 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 1445 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4888 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 1464 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = nullptr;
  }
#line 4896 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 1467 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = (yyvsp[0].opt_window_clause_);
  }
#line 4904 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 1472 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = new quickstep::PtrList<quickstep::ParseWindow>();
    (yyval.opt_window_clause_)->push_back((yyvsp[0].window_definition_));
  }
#line 4913 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 1476 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = (yyvsp[-1].opt_window_clause_);
    (yyval.opt_window_clause_)->push_back((yyvsp[0].window_definition_));
  }
#line 4922 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 1482 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_definition_) = (yyvsp[-1].window_definition_);
    (yyval.window_definition_)->setName((yyvsp[-4].string_value_));
  }
#line 4931 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 1488 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_definition_) = new quickstep::ParseWindow((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].window_partition_by_list_), (yyvsp[-1].window_order_by_list_), (yyvsp[0].window_frame_info_));
  }
#line 4939 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 1493 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_partition_by_list_) = nullptr;
  }
#line 4947 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 1496 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_partition_by_list_) = (yyvsp[0].expression_list_);
  }
#line 4955 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 1501 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_order_by_list_) = nullptr;
  }
#line 4963 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 1504 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_order_by_list_) = (yyvsp[0].order_commalist_);
  }
#line 4971 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 1509 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_frame_info_) = nullptr;
  }
#line 4979 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 1512 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_frame_info_) = new quickstep::ParseFrameInfo((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-4].boolean_value_), (yyvsp[-2].numeric_literal_value_)->long_value(), (yyvsp[0].numeric_literal_value_)->long_value());
  }
#line 4987 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 1517 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4995 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 1520 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5003 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 1526 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "-1");
  }
#line 5011 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 1529 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "0");
  }
#line 5019 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1535 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "-1");
  }
#line 5027 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 1538 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "0");
  }
#line 5035 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 1543 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 5044 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 1547 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 5053 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 1553 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 5063 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 1560 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 5071 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 1563 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 5079 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 1566 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 5087 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 1571 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 5095 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 1574 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 5103 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 1577 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 5111 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 1583 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 5119 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 1586 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5127 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 1591 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5135 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1596 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 5149 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1605 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5157 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1610 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 5171 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1619 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5179 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1624 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 5187 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1627 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5195 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1632 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5203 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1635 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 5213 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1640 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 5224 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1646 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 5235 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1652 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5243 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1655 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 5251 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1658 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateExists((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].subquery_expression_));
  }
#line 5259 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1661 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-2].expression_), (yyvsp[0].subquery_expression_));
  }
#line 5267 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1664 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-1].expression_list_));
  }
#line 5275 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1667 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-2]).first_line,
        (yylsp[-2]).first_column,
        new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-3].expression_), (yyvsp[0].subquery_expression_)));
  }
#line 5286 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1673 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line,
        (yylsp[-4]).first_column,
        new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-1].expression_list_)));
  }
#line 5297 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1682 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5305 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1685 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5313 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1690 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5321 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1693 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5329 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1698 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].unary_operation_), (yyvsp[0].expression_));
  }
#line 5337 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1701 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5345 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1706 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 5353 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1709 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 5361 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1712 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 5369 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1715 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyvsp[-2].function_call_)->setWindowName((yyvsp[0].string_value_));
    (yyval.expression_) = (yyvsp[-2].function_call_);
  }
#line 5378 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1719 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyvsp[-4].function_call_)->setWindow((yyvsp[-1].window_definition_));
    (yyval.expression_) = (yyvsp[-4].function_call_);
  }
#line 5387 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1723 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5395 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1726 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5403 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1729 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5411 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1732 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 5419 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1735 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].subquery_expression_);
  }
#line 5427 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1740 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 5436 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1744 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 5445 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1748 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, false, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 5453 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1751 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-4]).first_line, (yylsp[-4]).first_column, true, (yyvsp[-4].string_value_), (yyvsp[-1].expression_list_));
  }
#line 5461 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1756 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseExtractFunction((yylsp[-5]).first_line, (yylsp[-5]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].expression_));
  }
#line 5469 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1761 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSubstringFunction(
        (yylsp[-5]).first_line, (yylsp[-5]).first_column, (yyvsp[-3].expression_), (yyvsp[-1].numeric_literal_value_)->long_value());
  }
#line 5478 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1765 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSubstringFunction(
        (yylsp[-7]).first_line, (yylsp[-7]).first_column, (yyvsp[-5].expression_), (yyvsp[-3].numeric_literal_value_)->long_value(), (yyvsp[-1].numeric_literal_value_)->long_value());
  }
#line 5487 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1771 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSimpleCaseExpression((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].expression_), (yyvsp[-2].simple_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 5495 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1774 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSearchedCaseExpression((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].searched_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 5503 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1779 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSimpleWhenClause>;
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 5512 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1783 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_list_) = (yyvsp[-1].simple_when_clause_list_);
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 5521 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1789 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_) = new quickstep::ParseSimpleWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5529 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1794 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSearchedWhenClause>;
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 5538 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1798 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_list_) = (yyvsp[-1].searched_when_clause_list_);
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 5547 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1804 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_) = new quickstep::ParseSearchedWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].predicate_), (yyvsp[0].expression_));
  }
#line 5555 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1809 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = NULL;
  }
#line 5563 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1812 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5571 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1817 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 5580 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1821 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 5589 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1827 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 5597 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1830 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5605 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1833 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5613 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1836 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5630 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1848 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 5639 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1852 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5663 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1871 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5681 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1884 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5699 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1899 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("YEAR"));
  }
#line 5707 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1902 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("MONTH"));
  }
#line 5715 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1905 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("DAY"));
  }
#line 5723 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1908 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("HOUR"));
  }
#line 5731 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1911 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("MINUTE"));
  }
#line 5739 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1914 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("SECOND"));
  }
#line 5747 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1919 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5756 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1923 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5765 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1929 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_multiple_) = new quickstep::PtrList<quickstep::PtrList<quickstep::ParseScalarLiteral>>();
    (yyval.literal_value_list_multiple_)->push_back((yyvsp[-1].literal_value_list_));
  }
#line 5774 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1933 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_multiple_) = (yyvsp[-4].literal_value_list_multiple_);
    (yyval.literal_value_list_multiple_)->push_back((yyvsp[-1].literal_value_list_));
  }
#line 5783 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1939 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 5791 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1942 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 5799 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1947 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = new quickstep::PtrList<quickstep::ParseAttribute>();
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5808 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1951 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-2].attribute_list_);
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5817 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1958 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 5825 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1961 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 5833 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1964 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 5841 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1967 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 5849 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1970 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 5857 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1973 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 5865 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 1976 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLike);
  }
#line 5873 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1979 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotLike);
  }
#line 5881 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1982 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kRegexMatch);
  }
#line 5889 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1985 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotRegexMatch);
  }
#line 5897 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1990 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
#line 5911 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 2001 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
#line 5919 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 2004 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
#line 5927 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 2009 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kModulo);
  }
#line 5935 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 2012 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
#line 5943 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 2015 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
#line 5951 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 2021 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 5960 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 2025 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 5969 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 2031 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5977 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 2034 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5988 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 2042 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5996 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 2045 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 6004 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 2048 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 6012 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 2051 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 6020 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 2057 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.command_) = new quickstep::ParseCommand((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].command_argument_list_));
  }
#line 6028 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 2062 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    quickstep::PtrVector<quickstep::ParseString> *argument_list = (yyvsp[-1].command_argument_list_);
    argument_list->push_back((yyvsp[0].string_value_));
    (yyval.command_argument_list_) = argument_list;
  }
#line 6038 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 2067 "../SqlParser.ypp" /* yacc.c:1661  */
    { /* Epsilon, an empy match. */
    (yyval.command_argument_list_) = new quickstep::PtrVector<quickstep::ParseString>();
  }
#line 6046 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;


#line 6050 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 2071 "../SqlParser.ypp" /* yacc.c:1906  */


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
