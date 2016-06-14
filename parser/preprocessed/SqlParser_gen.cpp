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
#include "parser/ParserUtil.hpp"
#include "parser/ParseSample.hpp"
#include "parser/ParseSelect.hpp"
#include "parser/ParseSelectionClause.hpp"
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

#line 155 "SqlParser_gen.cpp" /* yacc.c:339  */

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
    TOKEN_QUIT = 354,
    TOKEN_RANGE = 355,
    TOKEN_REAL = 356,
    TOKEN_REFERENCES = 357,
    TOKEN_RIGHT = 358,
    TOKEN_ROW = 359,
    TOKEN_ROW_DELIMITER = 360,
    TOKEN_ROWS = 361,
    TOKEN_SECOND = 362,
    TOKEN_SELECT = 363,
    TOKEN_SET = 364,
    TOKEN_SMA = 365,
    TOKEN_SMALLINT = 366,
    TOKEN_SUBSTRING = 367,
    TOKEN_TABLE = 368,
    TOKEN_THEN = 369,
    TOKEN_TIME = 370,
    TOKEN_TIMESTAMP = 371,
    TOKEN_TRUE = 372,
    TOKEN_TUPLESAMPLE = 373,
    TOKEN_UNBOUNDED = 374,
    TOKEN_UNIQUE = 375,
    TOKEN_UPDATE = 376,
    TOKEN_USING = 377,
    TOKEN_VALUES = 378,
    TOKEN_VARCHAR = 379,
    TOKEN_WHEN = 380,
    TOKEN_WHERE = 381,
    TOKEN_WINDOW = 382,
    TOKEN_WITH = 383,
    TOKEN_YEAR = 384,
    TOKEN_YEARMONTH = 385,
    TOKEN_EOF = 386,
    TOKEN_LEX_ERROR = 387
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 119 "../SqlParser.ypp" /* yacc.c:355  */

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

#line 426 "SqlParser_gen.cpp" /* yacc.c:355  */
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

#line 461 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYFINAL  47
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1347

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  144
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  106
/* YYNRULES -- Number of rules.  */
#define YYNRULES  287
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  531

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   387

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     139,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   143,     2,     2,
     140,   141,    23,    21,   142,    22,    27,    24,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   138,
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
     130,   131,   132,   133,   134,   135,   136,   137
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   622,   622,   626,   630,   634,   638,   641,   648,   651,
     654,   657,   660,   663,   666,   669,   672,   675,   681,   687,
     694,   700,   707,   716,   721,   730,   735,   740,   744,   750,
     755,   758,   761,   766,   769,   772,   775,   778,   781,   784,
     787,   790,   793,   805,   808,   811,   829,   849,   852,   855,
     860,   865,   871,   877,   886,   890,   896,   899,   904,   909,
     914,   921,   928,   932,   938,   941,   946,   949,   954,   957,
     962,   965,   984,   988,   994,   998,  1004,  1007,  1010,  1015,
    1018,  1025,  1030,  1041,  1046,  1050,  1054,  1060,  1063,  1069,
    1077,  1080,  1083,  1089,  1094,  1097,  1102,  1106,  1110,  1114,
    1120,  1125,  1130,  1134,  1140,  1146,  1149,  1154,  1159,  1163,
    1169,  1175,  1181,  1184,  1188,  1194,  1197,  1202,  1206,  1212,
    1215,  1218,  1223,  1228,  1233,  1236,  1239,  1244,  1247,  1250,
    1253,  1256,  1259,  1262,  1265,  1270,  1273,  1278,  1282,  1286,
    1289,  1293,  1296,  1301,  1304,  1309,  1312,  1317,  1321,  1327,
    1330,  1335,  1338,  1343,  1346,  1351,  1354,  1373,  1376,  1379,
    1383,  1389,  1395,  1400,  1403,  1408,  1411,  1416,  1419,  1424,
    1427,  1432,  1433,  1436,  1441,  1442,  1445,  1450,  1454,  1460,
    1467,  1470,  1473,  1478,  1481,  1484,  1490,  1493,  1498,  1503,
    1512,  1517,  1526,  1531,  1534,  1539,  1542,  1547,  1553,  1559,
    1562,  1565,  1568,  1571,  1574,  1580,  1589,  1592,  1597,  1600,
    1605,  1608,  1613,  1616,  1619,  1622,  1626,  1630,  1633,  1636,
    1639,  1642,  1647,  1651,  1655,  1658,  1663,  1668,  1672,  1678,
    1681,  1686,  1690,  1696,  1701,  1705,  1711,  1716,  1719,  1724,
    1728,  1734,  1737,  1740,  1743,  1755,  1759,  1778,  1791,  1806,
    1809,  1812,  1815,  1818,  1821,  1826,  1830,  1836,  1839,  1844,
    1848,  1855,  1858,  1861,  1864,  1867,  1870,  1873,  1876,  1879,
    1882,  1887,  1898,  1901,  1906,  1909,  1912,  1918,  1922,  1928,
    1931,  1939,  1942,  1945,  1948,  1954,  1959,  1964
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
  "TOKEN_ADD", "TOKEN_ALL", "TOKEN_ALTER", "TOKEN_AS", "TOKEN_ASC",
  "TOKEN_BIGINT", "TOKEN_BIT", "TOKEN_BITWEAVING", "TOKEN_BLOCKPROPERTIES",
  "TOKEN_BLOCKSAMPLE", "TOKEN_BLOOM_FILTER", "TOKEN_CSB_TREE", "TOKEN_BY",
  "TOKEN_CASE", "TOKEN_CHARACTER", "TOKEN_CHECK", "TOKEN_COLUMN",
  "TOKEN_CONSTRAINT", "TOKEN_COPY", "TOKEN_CREATE", "TOKEN_CURRENT",
  "TOKEN_DATE", "TOKEN_DATETIME", "TOKEN_DAY", "TOKEN_DECIMAL",
  "TOKEN_DEFAULT", "TOKEN_DELETE", "TOKEN_DELIMITER", "TOKEN_DESC",
  "TOKEN_DISTINCT", "TOKEN_DOUBLE", "TOKEN_DROP", "TOKEN_ELSE",
  "TOKEN_END", "TOKEN_ESCAPE_STRINGS", "TOKEN_EXISTS", "TOKEN_EXTRACT",
  "TOKEN_FALSE", "TOKEN_FIRST", "TOKEN_FLOAT", "TOKEN_FOLLOWING",
  "TOKEN_FOR", "TOKEN_FOREIGN", "TOKEN_FROM", "TOKEN_FULL", "TOKEN_GROUP",
  "TOKEN_HASH", "TOKEN_HAVING", "TOKEN_HOUR", "TOKEN_IN", "TOKEN_INDEX",
  "TOKEN_INNER", "TOKEN_INSERT", "TOKEN_INTEGER", "TOKEN_INTERVAL",
  "TOKEN_INTO", "TOKEN_JOIN", "TOKEN_KEY", "TOKEN_LAST", "TOKEN_LEFT",
  "TOKEN_LIMIT", "TOKEN_LONG", "TOKEN_MINUTE", "TOKEN_MONTH", "TOKEN_NULL",
  "TOKEN_NULLS", "TOKEN_OFF", "TOKEN_ON", "TOKEN_ORDER", "TOKEN_OUTER",
  "TOKEN_OVER", "TOKEN_PARTITION", "TOKEN_PARTITIONS", "TOKEN_PERCENT",
  "TOKEN_PRECEDING", "TOKEN_PRIMARY", "TOKEN_QUIT", "TOKEN_RANGE",
  "TOKEN_REAL", "TOKEN_REFERENCES", "TOKEN_RIGHT", "TOKEN_ROW",
  "TOKEN_ROW_DELIMITER", "TOKEN_ROWS", "TOKEN_SECOND", "TOKEN_SELECT",
  "TOKEN_SET", "TOKEN_SMA", "TOKEN_SMALLINT", "TOKEN_SUBSTRING",
  "TOKEN_TABLE", "TOKEN_THEN", "TOKEN_TIME", "TOKEN_TIMESTAMP",
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
  "key_integer_value", "index_type", "opt_index_properties",
  "insert_statement", "copy_from_statement", "opt_copy_from_params",
  "copy_from_params", "update_statement", "delete_statement",
  "assignment_list", "assignment_item", "select_statement", "with_clause",
  "with_list", "with_list_element", "select_query", "opt_all_distinct",
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
     380,   381,   382,   383,   384,   385,   386,   387,    59,    10,
      40,    41,    44,    37
};
# endif

#define YYPACT_NINF -239

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-239)))

#define YYTABLE_NINF -128

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      71,  -239,  -239,   -52,   270,   -19,    22,   -43,    31,  -239,
      16,   270,   270,  -239,    97,   162,  -239,  -239,  -239,  -239,
    -239,  -239,  -239,  -239,  -239,  -239,   -24,  -239,    98,   117,
     270,  -239,  -239,    58,   270,   270,   270,   270,   270,  -239,
    -239,   619,    69,    29,  -239,   155,    52,  -239,  -239,  -239,
     126,  -239,  -239,  -239,  -239,    36,   233,   151,    60,   127,
    -239,    68,  -239,  -239,   255,   260,  -239,  -239,  -239,   700,
     131,  -239,   217,  -239,  -239,   137,  -239,  -239,   288,  -239,
    -239,  -239,  -239,   172,  -239,  -239,   176,   241,   792,   320,
     297,   189,  -239,  -239,   286,    -1,  -239,  -239,   271,  -239,
    -239,  -239,  -239,  -239,   954,     0,   270,   270,   234,   270,
     270,   184,   210,   240,   270,   270,   538,  -239,  -239,   239,
     270,  -239,  -239,  -239,   538,    47,    -9,  -239,   373,  -239,
     154,   154,  1035,   374,  -239,   242,    26,  -239,    33,   127,
    1035,  -239,  -239,   270,  1035,  -239,  -239,  -239,  -239,  1035,
      38,   260,  -239,   270,   354,    48,  -239,   371,  -239,   272,
    -239,   140,  -239,   272,   270,   -18,   270,   270,   244,  -239,
     246,  -239,   163,  1144,   873,   234,   457,   382,   383,  -239,
    -239,  1270,   377,  1116,   169,    46,  1035,    -2,  -239,  1035,
    -239,   332,   253,  -239,  -239,  -239,  -239,  -239,  -239,   327,
    -239,    65,   259,  -239,  -239,    34,   220,   177,  -239,   263,
     220,    -8,   328,  -239,  -239,    -1,  -239,   303,  -239,  -239,
     266,  1035,  -239,   293,   180,   270,  -239,  1035,  -239,   270,
    -239,  -239,   268,   325,   329,   273,  -239,  -239,  -239,   125,
     270,   290,   -18,   270,  -239,   188,  -239,  -239,     2,   201,
     538,   538,   105,  -239,  -239,  -239,  -239,  -239,  -239,  -239,
    -239,  1035,   279,  1035,    43,  -239,   185,   287,  1035,    57,
    -239,   359,   293,  -239,  -239,  1035,   415,  -239,   178,   270,
    -239,  -239,   326,  -239,   330,   331,   340,    33,  -239,   419,
     422,   220,   390,   356,   393,   296,   338,  -239,   187,  -239,
    1035,  -239,   293,  -239,   538,   298,   299,   270,   435,    -3,
     192,  -239,   195,   414,    42,  -239,   300,   311,  -239,   353,
     307,  1116,  -239,   364,   270,  -239,  -239,   188,  -239,  -239,
     383,  -239,  -239,  -239,  1035,   310,   275,   792,  -239,   293,
     360,  -239,  -239,  1116,   313,   293,  1035,  -239,    28,   -33,
    -239,  -239,  -239,  -239,  -239,    33,   177,   350,   357,  -239,
    1035,   538,   358,  1035,  -239,   425,   -17,  -239,   293,     4,
     270,   270,   197,  -239,  -239,  -239,  -239,  -239,  -239,  -239,
     148,  -239,   270,  -239,  -239,  -239,  -239,   315,   -18,   423,
     367,  -239,   538,  -239,  -239,   333,  -239,   280,   792,  -239,
    1035,   199,  -239,  -239,  1116,   293,  -239,   462,  -239,   381,
    -239,  -239,   335,   382,   432,   396,   335,  1035,  -239,  -239,
    -239,   467,  -239,   202,   204,  -239,   476,    -3,  -239,   270,
    -239,  -239,   347,   448,  -239,    11,   270,  1035,   206,   293,
    -239,   208,   351,   538,  1035,   486,   365,   361,  -239,   238,
       7,   389,  -239,  -239,  -239,   211,  -239,  -239,  -239,  -239,
      14,   270,    -7,  -239,   363,   293,  -239,  -239,  -239,   382,
     361,  -239,   270,  -239,   365,  -239,  1035,  -239,  -239,   407,
     399,   401,   400,   499,   270,  -239,   270,  -239,  -239,   270,
    -239,   213,  -239,  -239,   372,  -239,   480,  -239,  -239,   102,
    -239,  -239,  -239,  -239,    17,   376,  -239,   215,  -239,   270,
     378,  -239,  -239,   446,   408,   451,  -239,   270,  -239,   221,
     303,  -239,  -239,  -239,   223,   426,   384,  -239,   515,  -239,
    -239
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     6,   287,     0,     0,     0,     0,     0,     0,    18,
     112,     0,     0,     7,     0,     0,    15,     8,    10,    11,
      13,    14,     9,    17,    12,    16,     0,   105,     0,   285,
       0,   279,   280,     0,     0,     0,     0,     0,     0,   113,
     114,     0,     0,   107,   108,     0,   145,     1,     3,     2,
       0,   106,     5,     4,   286,     0,     0,     0,     0,   186,
      25,     0,   245,   242,     0,   271,   115,    40,    29,     0,
       0,    30,    31,    34,    36,     0,    37,    39,     0,    41,
     241,    35,    38,     0,    32,    33,     0,     0,     0,     0,
       0,   116,   117,   221,   121,   207,   209,   211,   214,   217,
     218,   219,   213,   212,     0,   257,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,     0,   101,   187,     0,
       0,    91,   243,   244,     0,     0,   237,   234,     0,    43,
       0,   246,     0,     0,    44,     0,     0,   248,     0,   186,
       0,   272,   273,     0,     0,   120,   275,   276,   274,     0,
       0,     0,   210,     0,     0,   186,   103,     0,   109,     0,
     110,     0,   277,     0,     0,     0,     0,     0,     0,    93,
      66,    27,     0,     0,     0,     0,     0,   188,   190,   192,
     194,     0,   212,     0,     0,     0,     0,   237,   231,     0,
     235,     0,     0,   251,   252,   253,   250,   254,   249,     0,
     247,     0,     0,   123,   220,     0,     0,   147,   136,   122,
     141,   124,   149,   118,   119,   206,   208,   163,   215,   258,
       0,     0,   222,   239,     0,     0,   100,     0,   146,     0,
      92,    19,     0,     0,     0,     0,    20,    21,    22,     0,
       0,     0,    64,     0,    42,    56,   193,   201,     0,     0,
       0,     0,     0,   261,   263,   264,   265,   266,   262,   267,
     269,     0,     0,     0,     0,   255,     0,     0,     0,     0,
     232,     0,   238,   230,    45,     0,     0,    46,   127,     0,
     137,   143,   133,   128,   129,   131,     0,     0,   140,     0,
       0,   139,     0,   151,     0,     0,   165,   223,     0,   224,
       0,   102,   104,   278,     0,     0,     0,     0,     0,     0,
       0,   259,     0,   257,     0,    63,    65,    68,    28,     0,
       0,     0,    47,     0,     0,    49,    55,    57,    26,   200,
     189,   191,   268,   270,     0,     0,     0,     0,   202,   199,
       0,   198,    90,     0,     0,   236,     0,   229,     0,     0,
     142,   144,   134,   130,   132,     0,   148,     0,     0,   138,
       0,     0,   153,     0,   216,     0,   167,   225,   240,     0,
       0,     0,     0,    96,   283,   284,   282,   281,    97,    95,
       0,    67,     0,    83,    84,    85,    86,    87,     0,     0,
      70,    48,     0,    51,    50,     0,    54,     0,     0,   204,
       0,     0,   197,   256,     0,   233,   226,     0,   227,     0,
     125,   126,   150,   152,     0,   155,   164,     0,   170,   169,
     162,     0,    61,     0,     0,    58,     0,     0,   260,     0,
      24,    62,     0,     0,    23,     0,     0,     0,     0,   195,
     203,     0,     0,     0,     0,     0,   157,   166,   177,   180,
       0,     0,    59,    98,    99,     0,    74,    76,    77,    78,
       0,     0,     0,    52,     0,   196,   205,    89,   228,   135,
     154,   156,     0,   111,   158,   159,     0,   181,   182,   183,
       0,     0,     0,     0,     0,    88,     0,    82,    80,     0,
      79,     0,    72,    73,     0,    53,     0,   160,   178,     0,
     179,   171,   173,   172,     0,     0,    75,     0,    69,     0,
       0,   184,   185,     0,     0,     0,   168,     0,    81,     0,
     163,   174,   176,   175,     0,     0,     0,    60,     0,   161,
      71
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,   -88,  -239,
     355,   196,  -239,  -239,  -238,  -239,  -239,  -239,  -239,  -239,
    -239,    66,    44,  -239,  -239,  -239,  -239,  -239,  -239,  -239,
    -239,  -239,  -239,  -239,  -239,   304,  -239,  -239,  -239,   424,
       9,  -239,  -239,  -239,   392,  -239,   -92,  -239,  -239,  -134,
     179,  -182,   -11,  -239,  -239,  -239,  -239,  -239,  -239,  -239,
      59,    15,  -239,  -239,  -239,  -239,  -239,  -239,    92,    61,
    -239,  -239,   -55,  -239,  -119,   291,   289,   379,   -28,   403,
     402,   450,  -123,  -239,  -239,  -239,  -239,   368,  -239,   430,
     370,  -204,  -177,   421,   146,  -113,  -239,  -239,  -239,  -239,
    -239,  -118,    -4,   134,  -239,  -239
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    14,    15,    16,    17,    18,    19,    20,   171,   172,
      89,   326,   327,   328,   236,   316,   317,   241,   390,   434,
     494,   455,   456,   457,   458,   459,   387,   430,    21,    22,
     169,   310,    23,    24,   155,   156,    25,    26,    43,    44,
     135,    41,    90,    91,    92,   139,    93,   291,   286,   207,
     208,   280,   281,   209,   293,   362,   415,   446,   473,   474,
     475,   295,   296,   366,   420,   421,   483,   516,   447,   448,
     479,   500,   117,   118,   177,   178,   179,   180,   181,    95,
      96,    97,    98,    99,   100,   101,   187,   188,   126,   127,
     191,   224,   102,   199,   266,   103,   312,   263,   104,   144,
     149,   161,   105,   378,    28,    29
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      33,    45,   184,   182,   315,   185,   265,    42,    46,    27,
     250,   182,   250,    94,   480,   210,   160,   298,    31,   250,
      32,   487,   146,   147,   513,   232,    55,   153,   288,   289,
      57,    58,    59,    60,    61,    51,   407,    31,    31,    32,
      32,   125,    31,   488,    32,    39,   206,   141,   142,   141,
     142,   189,   233,   340,   250,   481,    50,   248,   189,    34,
     136,   182,   374,   182,   111,   514,    30,   492,   141,   142,
     121,   278,     1,    40,     2,    37,   231,   383,   141,   142,
     384,   385,   210,   247,   212,   234,   141,   142,   418,    10,
     145,   375,   376,    36,   419,   112,    45,    47,   493,    35,
     226,     3,   157,    46,   201,   162,   163,   235,   408,   359,
     170,   173,    94,   206,    38,   290,   162,     4,     5,   377,
      54,   124,   332,   333,   334,     6,   223,   311,   186,    56,
       7,   482,   154,   401,   211,   341,   276,   182,   182,   214,
     154,   515,   148,   329,   393,   422,   218,    10,   249,   219,
     431,     8,   463,   356,   489,   318,   412,   386,   269,   416,
     173,   272,   237,   238,   210,   268,   403,   204,   511,   406,
     338,   107,   230,   205,   205,     9,   346,   186,   217,   116,
     308,    10,   335,   106,    10,   369,   108,   309,   512,   372,
     225,   182,   109,   223,   438,   206,   119,    11,   319,   302,
     115,   211,    46,   426,    12,   193,    46,    13,   120,   110,
     427,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   157,   141,   142,    31,   303,    32,   265,   164,   165,
     194,   320,   210,   336,    52,   339,   313,    53,   113,   173,
     345,   321,   413,   399,   195,   196,   114,   348,   182,   282,
     282,   279,   423,   424,   166,   167,   283,   283,   116,   141,
     142,  -127,   122,   206,   284,   284,   197,   123,   351,   428,
     477,   128,   368,   435,    31,    46,    32,   130,   262,   182,
     322,   228,   229,   211,   400,   285,   285,    46,   198,   437,
      31,   323,    32,   131,   478,   324,   141,   142,    48,   129,
      49,   141,   142,   162,   242,   243,   397,   141,   142,   223,
     267,   229,   132,   325,   141,   142,   133,   143,   405,   350,
     395,   299,   300,   134,   469,   137,   342,   343,   367,   300,
     182,   140,   223,   379,   380,   223,   381,   382,   425,   229,
     440,   300,   204,   451,   229,   452,   229,   466,   300,   467,
     343,   211,   485,   486,   508,   486,   518,   229,    31,    62,
      32,    63,   525,   229,   527,   229,   162,   162,   138,   150,
     223,   507,   439,   168,   159,    64,    65,   220,   313,   183,
     192,   202,   227,   203,   239,    10,   240,    67,    68,   449,
     250,   519,   251,   273,   274,    69,    70,   264,   275,   524,
     277,   292,   294,    71,    72,   287,    73,   297,   304,   465,
     305,   221,    74,   307,   306,   344,   449,   314,    75,   337,
     347,    76,   349,   352,   355,   460,   357,   353,   354,   358,
     360,   361,   464,   363,   365,    77,    78,   364,   370,   371,
     373,   153,   388,    79,   389,   391,    80,   392,   449,   394,
     398,   410,   402,   404,   414,   429,   490,   460,   411,   432,
      81,    31,    62,    32,    63,   417,   433,   174,   496,   442,
      82,    83,   444,   436,    84,    85,   443,   300,    64,    65,
     505,   453,   460,    86,   445,   162,   450,   461,   462,    87,
      67,    68,   468,   471,    88,   222,   484,   472,    69,    70,
     499,   501,   503,   476,   495,   162,    71,    72,   504,    73,
     502,   510,   509,   162,   521,    74,   517,   522,   520,   523,
     175,    75,   530,   396,    76,   529,   528,   491,   245,   301,
     506,   158,   213,   497,   409,   526,   470,   498,    77,    78,
     331,   330,    31,    62,    32,    63,    79,   215,   174,    80,
     441,   216,   200,   246,   152,   270,   190,   271,     0,    64,
      65,   454,     0,    81,     0,     0,     0,     0,     0,     0,
      10,    67,    68,    82,    83,     0,     0,    84,    85,    69,
      70,     0,     0,     0,     0,     0,    86,    71,    72,     0,
      73,     0,    87,     0,     0,     0,    74,   176,     0,     0,
       0,   175,    75,     0,     0,    76,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    77,
      78,     0,     0,    31,    62,    32,    63,    79,     0,     0,
      80,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,    65,    66,     0,    81,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    82,    83,     0,     0,    84,    85,
      69,    70,     0,     0,     0,     0,     0,    86,    71,    72,
       0,    73,     0,    87,     0,     0,     0,    74,   176,     0,
       0,     0,     0,    75,     0,     0,    76,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      77,    78,     0,     0,    31,    62,    32,    63,    79,     0,
       0,    80,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,    65,     0,     0,    81,     0,     0,     0,     0,
       0,     0,     0,    67,    68,    82,    83,     0,     0,    84,
      85,    69,    70,     0,     0,     0,     0,     0,    86,    71,
      72,     0,    73,     0,    87,     0,     0,     0,    74,    88,
       0,     0,     0,     0,    75,     0,     0,    76,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    77,    78,     0,     0,     0,     0,     0,     0,    79,
       0,     0,    80,     0,     0,     0,    31,    62,    32,    63,
       0,     0,     0,     0,     0,     0,    81,     0,     0,     0,
       0,     0,     0,    64,    65,     0,    82,    83,     0,     0,
      84,    85,     0,     0,     0,    67,    68,     0,     0,    86,
     124,     0,     0,    69,    70,    87,     0,     0,     0,     0,
      88,    71,    72,     0,    73,     0,     0,     0,     0,     0,
      74,     0,     0,     0,     0,     0,    75,     0,     0,    76,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    77,    78,     0,     0,    31,    62,    32,
      63,    79,     0,     0,    80,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,    65,     0,     0,    81,     0,
       0,     0,     0,     0,     0,    10,    67,    68,    82,    83,
       0,     0,    84,    85,    69,    70,     0,     0,     0,     0,
       0,    86,    71,    72,     0,    73,     0,    87,     0,     0,
       0,    74,    88,     0,     0,     0,   175,    75,     0,     0,
      76,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    77,    78,     0,     0,    31,    62,
      32,    63,    79,     0,     0,    80,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,   151,     0,     0,    81,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    82,
      83,     0,     0,    84,    85,    69,    70,     0,     0,     0,
       0,     0,    86,    71,    72,     0,    73,     0,    87,     0,
       0,     0,    74,   176,     0,     0,     0,     0,    75,     0,
       0,    76,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    77,    78,     0,     0,    31,
      62,    32,    63,    79,     0,     0,    80,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,    65,     0,     0,
      81,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      82,    83,     0,     0,    84,    85,    69,    70,     0,     0,
       0,     0,     0,    86,    71,    72,     0,    73,     0,    87,
       0,     0,     0,    74,    88,     0,     0,     0,     0,    75,
       0,     0,    76,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    77,    78,     0,     0,
       0,    62,     0,    63,    79,     0,     0,    80,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    64,   151,     0,
       0,    81,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    82,    83,     0,     0,    84,    85,     0,    70,     0,
       0,     0,     0,     0,    86,    71,    72,     0,    73,     0,
      87,     0,     0,     0,    74,    88,     0,    67,    68,     0,
       0,     0,     0,    76,     0,     0,    70,     0,     0,     0,
       0,     0,     0,    71,    72,     0,    73,    77,    78,     0,
       0,     0,    74,     0,     0,    79,     0,     0,    80,     0,
       0,    76,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    81,     0,     0,    77,   244,     0,     0,     0,
       0,     0,    82,    79,     0,     0,    84,    85,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
      81,    87,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,    84,    85,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,    87,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
       0,   141,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   262
};

static const yytype_int16 yycheck[] =
{
       4,    12,   120,   116,   242,   124,   183,    11,    12,     0,
       8,   124,     8,    41,     7,   138,   108,   221,     4,     8,
       6,     7,    23,    24,     7,    43,    30,    27,   210,    37,
      34,    35,    36,    37,    38,    26,    69,     4,     4,     6,
       6,    69,     4,    29,     6,    29,   138,    21,    22,    21,
      22,    60,    70,    10,     8,    48,    80,   176,    60,    78,
      88,   174,    65,   176,    28,    48,   118,    74,    21,    22,
      61,   205,     1,    57,     3,   118,   164,    35,    21,    22,
      38,    39,   205,   175,   139,   103,    21,    22,   105,   113,
      94,    94,    95,    71,   111,    59,   107,     0,   105,   118,
     155,    30,   106,   107,   132,   109,   110,   125,   141,   291,
     114,   115,   140,   205,    83,   123,   120,    46,    47,   122,
       3,   130,    17,    18,    19,    54,   154,   240,   130,    71,
      59,   124,   140,   337,   138,    92,    71,   250,   251,   143,
     140,   124,   143,   141,   321,   141,   150,   113,   176,   153,
     388,    80,   141,   287,   140,   243,   360,   115,   186,   363,
     164,   189,   166,   167,   287,   119,   343,   141,    66,   141,
     262,   142,   163,   140,   140,   104,   119,   130,   140,   131,
      55,   113,    77,   114,   113,   304,    31,    62,    86,   307,
     142,   304,   140,   221,   398,   287,   128,   126,    10,   227,
     140,   205,   206,    55,   133,    51,   210,   136,   140,    83,
      62,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,   225,    21,    22,     4,   229,     6,   404,    44,    45,
      76,    43,   355,   261,   136,   263,   240,   139,     5,   243,
     268,    53,   361,   335,    90,    91,    95,   275,   361,    72,
      72,    31,   370,   371,    44,    45,    79,    79,   131,    21,
      22,    84,     7,   355,    87,    87,   112,     7,   279,   382,
      32,   140,   300,   392,     4,   279,     6,   140,    77,   392,
      92,   141,   142,   287,     9,   108,   108,   291,   134,     9,
       4,   103,     6,     5,    56,   107,    21,    22,   136,    82,
     138,    21,    22,   307,   141,   142,   334,    21,    22,   337,
     141,   142,   140,   125,    21,    22,   140,    31,   346,   141,
     324,   141,   142,    82,   443,     5,   141,   142,   141,   142,
     443,   142,   360,   141,   142,   363,   141,   142,   141,   142,
     141,   142,   141,   141,   142,   141,   142,   141,   142,   141,
     142,   355,   141,   142,   141,   142,   141,   142,     4,     5,
       6,     7,   141,   142,   141,   142,   370,   371,    71,    98,
     398,   489,   400,   133,   140,    21,    22,    23,   382,   140,
       7,     7,    11,   141,   140,   113,   140,    33,    34,   417,
       8,   509,     9,    61,   141,    41,    42,    20,    71,   517,
     141,    73,    99,    49,    50,   142,    52,   141,   140,   437,
      85,    57,    58,   140,    85,   128,   444,   127,    64,   140,
      61,    67,     7,    97,    84,   429,     7,    97,    97,     7,
      40,    75,   436,    40,    96,    81,    82,   141,   140,   140,
       5,    27,   142,    89,   133,    92,    92,   140,   476,    85,
     140,   101,    92,   140,    96,   140,   460,   461,   101,    36,
     106,     4,     5,     6,     7,    40,    99,    10,   472,     7,
     116,   117,    40,   140,   120,   121,    95,   142,    21,    22,
     484,     5,   486,   129,    88,   489,    19,   140,    40,   135,
      33,    34,   141,     7,   140,   141,   107,   132,    41,    42,
      93,   102,   102,   142,   141,   509,    49,    50,     9,    52,
     109,    31,   140,   517,    68,    58,   140,   109,   140,    68,
      63,    64,     7,   327,    67,   141,   100,   461,   173,   225,
     486,   107,   140,   474,   355,   520,   444,   476,    81,    82,
     251,   250,     4,     5,     6,     7,    89,   144,    10,    92,
     404,   149,   131,   174,   104,   187,   126,   187,    -1,    21,
      22,   427,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    33,    34,   116,   117,    -1,    -1,   120,   121,    41,
      42,    -1,    -1,    -1,    -1,    -1,   129,    49,    50,    -1,
      52,    -1,   135,    -1,    -1,    -1,    58,   140,    -1,    -1,
      -1,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      82,    -1,    -1,     4,     5,     6,     7,    89,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    23,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    34,   116,   117,    -1,    -1,   120,   121,
      41,    42,    -1,    -1,    -1,    -1,    -1,   129,    49,    50,
      -1,    52,    -1,   135,    -1,    -1,    -1,    58,   140,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    82,    -1,    -1,     4,     5,     6,     7,    89,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    34,   116,   117,    -1,    -1,   120,
     121,    41,    42,    -1,    -1,    -1,    -1,    -1,   129,    49,
      50,    -1,    52,    -1,   135,    -1,    -1,    -1,    58,   140,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,     4,     5,     6,     7,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,   116,   117,    -1,    -1,
     120,   121,    -1,    -1,    -1,    33,    34,    -1,    -1,   129,
     130,    -1,    -1,    41,    42,   135,    -1,    -1,    -1,    -1,
     140,    49,    50,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    -1,    -1,     4,     5,     6,
       7,    89,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    33,    34,   116,   117,
      -1,    -1,   120,   121,    41,    42,    -1,    -1,    -1,    -1,
      -1,   129,    49,    50,    -1,    52,    -1,   135,    -1,    -1,
      -1,    58,   140,    -1,    -1,    -1,    63,    64,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    -1,    -1,     4,     5,
       6,     7,    89,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,   116,
     117,    -1,    -1,   120,   121,    41,    42,    -1,    -1,    -1,
      -1,    -1,   129,    49,    50,    -1,    52,    -1,   135,    -1,
      -1,    -1,    58,   140,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,     4,
       5,     6,     7,    89,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,
     116,   117,    -1,    -1,   120,   121,    41,    42,    -1,    -1,
      -1,    -1,    -1,   129,    49,    50,    -1,    52,    -1,   135,
      -1,    -1,    -1,    58,   140,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,
      -1,     5,    -1,     7,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,   116,   117,    -1,    -1,   120,   121,    -1,    42,    -1,
      -1,    -1,    -1,    -1,   129,    49,    50,    -1,    52,    -1,
     135,    -1,    -1,    -1,    58,   140,    -1,    33,    34,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    50,    -1,    52,    81,    82,    -1,
      -1,    -1,    58,    -1,    -1,    89,    -1,    -1,    92,    -1,
      -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    81,    82,    -1,    -1,    -1,
      -1,    -1,   116,    89,    -1,    -1,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,
     106,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    -1,   120,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,   135,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,    30,    46,    47,    54,    59,    80,   104,
     113,   126,   133,   136,   145,   146,   147,   148,   149,   150,
     151,   172,   173,   176,   177,   180,   181,   184,   248,   249,
     118,     4,     6,   246,    78,   118,    71,   118,    83,    29,
      57,   185,   246,   182,   183,   196,   246,     0,   136,   138,
      80,   184,   136,   139,     3,   246,    71,   246,   246,   246,
     246,   246,     5,     7,    21,    22,    23,    33,    34,    41,
      42,    49,    50,    52,    58,    64,    67,    81,    82,    89,
      92,   106,   116,   117,   120,   121,   129,   135,   140,   154,
     186,   187,   188,   190,   222,   223,   224,   225,   226,   227,
     228,   229,   236,   239,   242,   246,   114,   142,    31,   140,
      83,    28,    59,     5,    95,   140,   131,   216,   217,   128,
     140,   184,     7,     7,   130,   222,   232,   233,   140,    82,
     140,     5,   140,   140,    82,   184,   222,     5,    71,   189,
     142,    21,    22,    31,   243,   246,    23,    24,   143,   244,
      98,    22,   225,    27,   140,   178,   179,   246,   183,   140,
     190,   245,   246,   246,    44,    45,    44,    45,   133,   174,
     246,   152,   153,   246,    10,    63,   140,   218,   219,   220,
     221,   222,   239,   140,   245,   218,   130,   230,   231,    60,
     233,   234,     7,    51,    76,    90,    91,   112,   134,   237,
     237,   222,     7,   141,   141,   140,   190,   193,   194,   197,
     226,   246,   216,   188,   246,   223,   224,   140,   246,   246,
      23,    57,   141,   222,   235,   142,   216,    11,   141,   142,
     184,   152,    43,    70,   103,   125,   158,   246,   246,   140,
     140,   161,   141,   142,    82,   154,   221,   190,   218,   222,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    77,   241,    20,   236,   238,   141,   119,   222,
     231,   234,   222,    61,   141,    71,    71,   141,   193,    31,
     195,   196,    72,    79,    87,   108,   192,   142,   195,    37,
     123,   191,    73,   198,    99,   205,   206,   141,   235,   141,
     142,   179,   222,   246,   140,    85,    85,   140,    55,    62,
     175,   239,   240,   246,   127,   158,   159,   160,   152,    10,
      43,    53,    92,   103,   107,   125,   155,   156,   157,   141,
     219,   220,    17,    18,    19,    77,   222,   140,   190,   222,
      10,    92,   141,   142,   128,   222,   119,    61,   222,     7,
     141,   196,    97,    97,    97,    84,   193,     7,     7,   195,
      40,    75,   199,    40,   141,    96,   207,   141,   222,   218,
     140,   140,   245,     5,    65,    94,    95,   122,   247,   141,
     142,   141,   142,    35,    38,    39,   115,   170,   142,   133,
     162,    92,   140,   236,    85,   246,   155,   222,   140,   190,
       9,   235,    92,   236,   140,   222,   141,    69,   141,   194,
     101,   101,   235,   218,    96,   200,   235,    40,   105,   111,
     208,   209,   141,   245,   245,   141,    55,    62,   239,   140,
     171,   158,    36,    99,   163,   218,   140,     9,   235,   222,
     141,   238,     7,    95,    40,    88,   201,   212,   213,   222,
      19,   141,   141,     5,   247,   165,   166,   167,   168,   169,
     246,   140,    40,   141,   246,   222,   141,   141,   141,   218,
     212,     7,   132,   202,   203,   204,   142,    32,    56,   214,
       7,    48,   124,   210,   107,   141,   142,     7,    29,   140,
     246,   165,    74,   105,   164,   141,   246,   204,   213,    93,
     215,   102,   109,   102,     9,   246,   166,   245,   141,   140,
      31,    66,    86,     7,    48,   124,   211,   140,   141,   245,
     140,    68,   109,    68,   245,   141,   205,   141,   100,   141,
       7
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   144,   145,   145,   145,   145,   145,   145,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   147,   148,
     148,   148,   148,   149,   150,   151,   152,   153,   153,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   155,   155,   155,
     155,   155,   155,   155,   156,   156,   157,   157,   158,   158,
     158,   158,   159,   159,   160,   160,   161,   161,   162,   162,
     163,   163,   164,   164,   165,   165,   166,   166,   166,   167,
     167,   168,   169,   170,   170,   170,   170,   171,   171,   172,
     172,   172,   172,   173,   174,   174,   175,   175,   175,   175,
     176,   177,   178,   178,   179,   180,   180,   181,   182,   182,
     183,   184,   185,   185,   185,   186,   186,   187,   187,   188,
     188,   188,   189,   190,   191,   191,   191,   192,   192,   192,
     192,   192,   192,   192,   192,   193,   193,   194,   194,   194,
     194,   194,   194,   195,   195,   196,   196,   197,   197,   198,
     198,   199,   199,   200,   200,   201,   201,   202,   202,   203,
     203,   204,   205,   206,   206,   207,   207,   208,   208,   209,
     209,   210,   210,   210,   211,   211,   211,   212,   212,   213,
     214,   214,   214,   215,   215,   215,   216,   216,   217,   218,
     218,   219,   219,   220,   220,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   222,   222,   223,   223,
     224,   224,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   226,   226,   226,   226,   227,   228,   228,   229,
     229,   230,   230,   231,   232,   232,   233,   234,   234,   235,
     235,   236,   236,   236,   236,   236,   236,   236,   236,   237,
     237,   237,   237,   237,   237,   238,   238,   239,   239,   240,
     240,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   242,   243,   243,   244,   244,   244,   245,   245,   246,
     246,   247,   247,   247,   247,   248,   249,   249
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
       0,     8,     1,     1,     1,     3,     1,     1,     1,     2,
       2,     4,     2,     1,     1,     1,     1,     0,     3,    10,
       7,     4,     5,     5,     0,     4,     2,     2,     4,     4,
       5,     4,     3,     1,     3,     1,     2,     2,     1,     3,
       3,    10,     0,     1,     1,     1,     1,     1,     3,     3,
       2,     1,     2,     3,     0,     3,     3,     0,     1,     1,
       2,     1,     2,     1,     2,     6,     1,     2,     3,     2,
       2,     1,     3,     1,     2,     1,     4,     1,     3,     0,
       3,     0,     2,     0,     3,     0,     2,     0,     1,     1,
       2,     6,     3,     0,     3,     0,     3,     0,     5,     1,
       1,     2,     2,     2,     2,     2,     2,     1,     3,     3,
       0,     1,     1,     0,     2,     2,     0,     1,     2,     3,
       1,     3,     1,     2,     1,     5,     6,     4,     3,     3,
       3,     2,     3,     5,     4,     6,     3,     1,     3,     1,
       2,     1,     1,     1,     1,     3,     5,     1,     1,     1,
       3,     1,     3,     4,     4,     5,     6,     6,     8,     5,
       4,     1,     2,     4,     1,     2,     4,     0,     2,     1,
       3,     1,     1,     2,     2,     1,     2,     3,     2,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     2,     2,     0
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
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1975 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_NAME  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1985 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1995 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2005 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 7: /* TOKEN_UNSIGNED_NUMVAL  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2015 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 146: /* sql_statement  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2025 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 147: /* quit_statement  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 2035 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 148: /* alter_table_statement  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2045 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 149: /* create_table_statement  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 2055 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 150: /* create_index_statement  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2065 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 151: /* drop_table_statement  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 2075 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 152: /* column_def  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 2085 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 153: /* column_def_commalist  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 2095 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* data_type  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 2105 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* column_constraint_def  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 2115 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* column_constraint_def_list  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2125 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* opt_column_constraint_def_list  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2135 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* opt_column_list  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2145 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* opt_block_properties  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).block_properties_) != nullptr) {
    delete ((*yyvaluep).block_properties_);
  }
}
#line 2155 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 163: /* opt_partition_clause  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).partition_clause_) != nullptr) {
    delete ((*yyvaluep).partition_clause_);
  }
}
#line 2165 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 164: /* partition_type  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2175 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 165: /* key_value_list  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2185 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 166: /* key_value  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 2195 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 167: /* key_string_value  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 2205 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* key_string_list  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 2215 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* key_integer_value  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 2225 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* index_type  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2235 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* opt_index_properties  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2245 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* insert_statement  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 2255 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* copy_from_statement  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_statement_) != nullptr) {
    delete ((*yyvaluep).copy_from_statement_);
  }
}
#line 2265 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* opt_copy_from_params  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2275 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* copy_from_params  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2285 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* update_statement  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2295 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* delete_statement  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2305 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* assignment_list  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2315 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* assignment_item  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2325 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* select_statement  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_statement_) != nullptr) {
    delete ((*yyvaluep).select_statement_);
  }
}
#line 2335 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* with_clause  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2345 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* with_list  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2355 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* with_list_element  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2365 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* select_query  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2375 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* selection  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2385 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* selection_item_commalist  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2395 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* selection_item  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2405 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* from_clause  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2415 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 190: /* subquery_expression  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2425 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 191: /* opt_sample_clause  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_sample_clause_) != nullptr) {
    delete ((*yyvaluep).opt_sample_clause_);
  }
}
#line 2435 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 192: /* join_type  */
#line 611 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2441 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 193: /* joined_table_reference  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2451 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 194: /* table_reference  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2461 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 195: /* table_reference_signature  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2471 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 196: /* table_reference_signature_primary  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2481 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 197: /* joined_table_reference_commalist  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2491 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 198: /* opt_group_by_clause  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2501 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 199: /* opt_having_clause  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2511 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 200: /* opt_order_by_clause  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2521 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 201: /* opt_limit_clause  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2531 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 202: /* opt_window_clause  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_window_clause_) != nullptr) {
    delete ((*yyvaluep).opt_window_clause_);
  }
}
#line 2541 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 203: /* window_declaration_list  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_window_clause_) != nullptr) {
    delete ((*yyvaluep).opt_window_clause_);
  }
}
#line 2551 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 204: /* window_declaration  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_definition_) != nullptr) {
    delete ((*yyvaluep).window_definition_);
  }
}
#line 2561 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 205: /* window_definition  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_definition_) != nullptr) {
    delete ((*yyvaluep).window_definition_);
  }
}
#line 2571 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 206: /* opt_window_partition  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_partition_by_list_) != nullptr) {
    delete ((*yyvaluep).window_partition_by_list_);
  }
}
#line 2581 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 207: /* opt_window_order  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_order_by_list_) != nullptr) {
    delete ((*yyvaluep).window_order_by_list_);
  }
}
#line 2591 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 208: /* opt_window_frame  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_frame_info_) != nullptr) {
    delete ((*yyvaluep).window_frame_info_);
  }
}
#line 2601 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 209: /* frame_mode  */
#line 607 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2607 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 210: /* frame_preceding  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2617 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 211: /* frame_following  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2627 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 212: /* order_commalist  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2637 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 213: /* order_item  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2647 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 214: /* opt_order_direction  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2657 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 215: /* opt_nulls_first  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2667 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 216: /* opt_where_clause  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2677 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 217: /* where_clause  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2687 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 218: /* or_expression  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2697 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 219: /* and_expression  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2707 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 220: /* not_expression  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2717 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 221: /* predicate_expression_base  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2727 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 222: /* add_expression  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2737 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 223: /* multiply_expression  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2747 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 224: /* unary_expression  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2757 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 225: /* expression_base  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2767 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 226: /* function_call  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2777 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 227: /* extract_function  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2787 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 228: /* substr_function  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2797 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 229: /* case_expression  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2807 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 230: /* simple_when_clause_list  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_list_);
  }
}
#line 2817 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 231: /* simple_when_clause  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_);
  }
}
#line 2827 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 232: /* searched_when_clause_list  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_list_);
  }
}
#line 2837 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 233: /* searched_when_clause  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_);
  }
}
#line 2847 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 234: /* opt_else_clause  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2857 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 235: /* expression_list  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2867 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 236: /* literal_value  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2877 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 237: /* datetime_unit  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2887 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 238: /* literal_value_commalist  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2897 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 239: /* attribute_ref  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2907 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 240: /* attribute_ref_list  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2917 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 241: /* comparison_operation  */
#line 608 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2923 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 242: /* unary_operation  */
#line 609 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2929 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 243: /* add_operation  */
#line 610 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2935 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 244: /* multiply_operation  */
#line 610 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2941 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 245: /* name_commalist  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 2951 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 246: /* any_name  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2961 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 247: /* boolean_value  */
#line 607 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2967 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 248: /* command  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_) != nullptr) {
    delete ((*yyvaluep).command_);
  }
}
#line 2977 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 249: /* command_argument_list  */
#line 613 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_argument_list_) != nullptr) {
    delete ((*yyvaluep).command_argument_list_);
  }
}
#line 2987 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 622 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3284 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 626 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3293 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 630 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3302 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 634 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3311 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 638 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    YYABORT;
  }
#line 3319 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 641 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 3328 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 648 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3336 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 651 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].copy_from_statement_);
  }
#line 3344 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 654 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 3352 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 657 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3360 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 660 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 3368 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 663 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 3376 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 666 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 3384 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 669 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 3392 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 672 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].select_statement_);
  }
#line 3400 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 675 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 3408 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 681 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3416 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 687 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3428 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 694 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3439 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 700 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3451 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 707 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3463 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 716 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].attribute_definition_list_), (yyvsp[-1].block_properties_), (yyvsp[0].partition_clause_));
  }
#line 3471 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 721 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].key_value_list_)) {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_), (yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].key_value_list_));
    } else {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_));
    }
  }
#line 3483 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 730 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 3491 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 735 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 3499 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 740 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3508 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 744 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3517 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 750 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 3527 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 755 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3535 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 758 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3543 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 761 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3553 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 766 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3561 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 769 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3569 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 772 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3577 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 775 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3585 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 778 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3593 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 781 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3601 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 784 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3609 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 787 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3617 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 790 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3625 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 793 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3642 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 805 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3650 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 808 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3658 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 811 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3681 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 829 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3704 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 849 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3712 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 852 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3720 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 855 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3730 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 860 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3740 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 865 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3751 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 871 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3762 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 877 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3774 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 886 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3783 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 890 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3792 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 896 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3800 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 899 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3808 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 904 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3818 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 909 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3828 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 914 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3840 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 921 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3850 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 928 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3859 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 932 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3868 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 938 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 3876 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 941 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = $1; */
  }
#line 3884 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 946 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_list_) = nullptr;
  }
#line 3892 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 949 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_list_) = (yyvsp[-1].attribute_list_);
  }
#line 3900 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 954 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.block_properties_) = nullptr;
  }
#line 3908 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 957 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].key_value_list_));
  }
#line 3916 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 962 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.partition_clause_) = nullptr;
  }
#line 3924 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 965 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3946 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 984 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::PartitionSchemeHeader::PartitionType::kHash));
  }
#line 3955 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 988 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::PartitionSchemeHeader::PartitionType::kRange));
  }
#line 3964 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 994 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3973 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 998 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3982 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 1004 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 3990 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 1007 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 3998 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 1010 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 4006 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 1015 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 4014 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 1018 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_),
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 4024 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 1025 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4032 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 1030 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 4046 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 1041 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    // Defaults to BitWeavingV, but IndexProperties can change this to H.
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBitWeavingV));
  }
#line 4056 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 1046 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
#line 4065 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 1050 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
#line 4074 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 1054 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kSMA));
  }
#line 4083 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 1060 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 4091 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 1063 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 4099 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 1069 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 4112 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 1077 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertTuple((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 4120 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 1080 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-3]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), nullptr);
  }
#line 4128 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 1083 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-4]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), (yyvsp[-4].with_list_));
  }
#line 4136 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 1089 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_statement_) = new quickstep::ParseStatementCopyFrom((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].copy_from_params_));
  }
#line 4144 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 1094 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = nullptr;
  }
#line 4152 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 1097 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-1].copy_from_params_);
  }
#line 4160 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 1102 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 4169 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 1106 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 4178 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 1110 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 4187 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 1114 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 4196 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 1120 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 4204 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 1125 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 4212 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 1130 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 4221 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 1134 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 4230 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 1140 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 4238 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 1146 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].select_query_), nullptr);
  }
#line 4246 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 1149 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].select_query_), (yyvsp[-1].with_list_));
  }
#line 4254 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 1154 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 4262 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 1159 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4271 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 1163 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4280 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 1169 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 4289 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 1176 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-9]).first_line, (yylsp[-9]).first_column, (yyvsp[-7].selection_), (yyvsp[-6].table_reference_list_), (yyvsp[-5].predicate_), (yyvsp[-4].opt_group_by_clause_), (yyvsp[-3].opt_having_clause_), (yyvsp[-2].opt_order_by_clause_), (yyvsp[-1].opt_limit_clause_), (yyvsp[0].opt_window_clause_));
  }
#line 4297 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 1181 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 4305 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 1184 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "ALL in selection");
    YYERROR;
  }
#line 4314 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 1188 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "DISTINCT in selection");
    YYERROR;
  }
#line 4323 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 1194 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4331 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 1197 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 4339 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 1202 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4348 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 1206 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4357 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 1212 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 4365 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 1215 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 4373 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 1218 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 4381 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 1223 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = (yyvsp[0].table_reference_list_);
  }
#line 4389 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 1228 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].select_query_));
  }
#line 4397 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 1233 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_sample_clause_) = NULL;
  }
#line 4405 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 1236 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, true, (yyvsp[-1].numeric_literal_value_));
  }
#line 4413 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 1239 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].numeric_literal_value_));
  }
#line 4421 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 1244 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4429 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 1247 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4437 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 1250 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4445 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 1253 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4453 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 1256 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4461 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 1259 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4469 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 1262 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4477 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 1265 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4485 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 1270 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseJoinedTableReference((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].join_type_), (yyvsp[-5].table_reference_), (yyvsp[-2].table_reference_), (yyvsp[0].predicate_));
  }
#line 4493 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 1273 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = (yyvsp[0].table_reference_);
  }
#line 4501 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 1278 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4510 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 1282 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].opt_sample_clause_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4519 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 1286 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].opt_sample_clause_));
  }
#line 4527 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 1289 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4536 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 1293 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].function_call_));
  }
#line 4544 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 1296 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = (yyvsp[-1].table_reference_);
  }
#line 4552 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 1301 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4560 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 1304 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4568 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 1309 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4576 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 1312 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4584 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 1317 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4593 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 148:
#line 1321 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4602 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 149:
#line 1327 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 4610 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 150:
#line 1330 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4618 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 1335 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4626 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 1338 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4634 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 1343 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4642 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 1346 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4650 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 1351 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4658 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 1354 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4680 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 1373 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_window_clause_) = nullptr;
  }
#line 4688 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 159:
#line 1379 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_window_clause_) = new quickstep::PtrList<quickstep::ParseWindow>();
    (yyval.opt_window_clause_)->push_back((yyvsp[0].window_definition_));
  }
#line 4697 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 1383 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_window_clause_) = (yyvsp[-1].opt_window_clause_);
    (yyval.opt_window_clause_)->push_back((yyvsp[0].window_definition_));
  }
#line 4706 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 161:
#line 1389 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.window_definition_) = (yyvsp[-1].window_definition_);
    (yyval.window_definition_)->setName((yyvsp[-4].string_value_));
  }
#line 4715 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 1395 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.window_definition_) = new quickstep::ParseWindow((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].window_partition_by_list_), (yyvsp[-1].window_order_by_list_), (yyvsp[0].window_frame_info_));
  }
#line 4723 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 163:
#line 1400 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.window_partition_by_list_) = nullptr;
  }
#line 4731 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 1403 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.window_partition_by_list_) = (yyvsp[0].expression_list_);
  }
#line 4739 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 165:
#line 1408 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.window_order_by_list_) = nullptr;
  }
#line 4747 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 166:
#line 1411 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.window_order_by_list_) = (yyvsp[0].order_commalist_);
  }
#line 4755 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 1416 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.window_frame_info_) = nullptr;
  }
#line 4763 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 168:
#line 1419 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.window_frame_info_) = new quickstep::ParseFrameInfo((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-4].boolean_value_), (yyvsp[-2].numeric_literal_value_)->long_value(), (yyvsp[0].numeric_literal_value_)->long_value());
  }
#line 4771 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 169:
#line 1424 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4779 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 1427 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;
  }
#line 4787 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 172:
#line 1433 "../SqlParser.ypp" /* yacc.c:1646  */
    { 
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "-1");
  }
#line 4795 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 173:
#line 1436 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "0");
  }
#line 4803 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 175:
#line 1442 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "-1");
  }
#line 4811 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 176:
#line 1445 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "0");
  }
#line 4819 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 177:
#line 1450 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4828 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 178:
#line 1454 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4837 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 1460 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 4847 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 180:
#line 1467 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4855 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 181:
#line 1470 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4863 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 1473 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4871 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 183:
#line 1478 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4879 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 184:
#line 1481 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4887 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 185:
#line 1484 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4895 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 186:
#line 1490 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 4903 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 187:
#line 1493 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4911 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 1498 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4919 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 189:
#line 1503 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4933 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 190:
#line 1512 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4941 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 191:
#line 1517 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4955 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 192:
#line 1526 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4963 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 193:
#line 1531 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4971 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 194:
#line 1534 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4979 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 195:
#line 1539 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4987 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 196:
#line 1542 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 4997 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 197:
#line 1547 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 5008 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 198:
#line 1553 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 5019 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 199:
#line 1559 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5027 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 200:
#line 1562 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 5035 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 201:
#line 1565 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateExists((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].subquery_expression_));
  }
#line 5043 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 202:
#line 1568 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-2].expression_), (yyvsp[0].subquery_expression_));
  }
#line 5051 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 203:
#line 1571 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-1].expression_list_));
  }
#line 5059 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 204:
#line 1574 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-2]).first_line,
        (yylsp[-2]).first_column,
        new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-3].expression_), (yyvsp[0].subquery_expression_)));
  }
#line 5070 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 205:
#line 1580 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line,
        (yylsp[-4]).first_column,
        new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-1].expression_list_)));
  }
#line 5081 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 206:
#line 1589 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5089 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 1592 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5097 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 208:
#line 1597 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5105 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 209:
#line 1600 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5113 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 210:
#line 1605 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].unary_operation_), (yyvsp[0].expression_));
  }
#line 5121 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 211:
#line 1608 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5129 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 212:
#line 1613 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 5137 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 213:
#line 1616 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 5145 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 214:
#line 1619 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 5153 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 215:
#line 1622 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyvsp[-2].function_call_)->setWindowName((yyvsp[0].string_value_));
    (yyval.expression_) = (yyvsp[-2].function_call_);
  }
#line 5162 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 216:
#line 1626 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyvsp[-4].function_call_)->setWindow((yyvsp[-1].window_definition_));
    (yyval.expression_) = (yyvsp[-4].function_call_);
  }
#line 5171 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 217:
#line 1630 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5179 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 218:
#line 1633 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5187 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 219:
#line 1636 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5195 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 220:
#line 1639 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 5203 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 221:
#line 1642 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].subquery_expression_);
  }
#line 5211 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 222:
#line 1647 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 5220 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 223:
#line 1651 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 5229 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 224:
#line 1655 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, false, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 5237 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 225:
#line 1658 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-4]).first_line, (yylsp[-4]).first_column, true, (yyvsp[-4].string_value_), (yyvsp[-1].expression_list_));
  }
#line 5245 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 226:
#line 1663 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseExtractFunction((yylsp[-5]).first_line, (yylsp[-5]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].expression_));
  }
#line 5253 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 227:
#line 1668 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseSubstringFunction(
        (yylsp[-5]).first_line, (yylsp[-5]).first_column, (yyvsp[-3].expression_), (yyvsp[-1].numeric_literal_value_)->long_value());
  }
#line 5262 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 228:
#line 1672 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseSubstringFunction(
        (yylsp[-7]).first_line, (yylsp[-7]).first_column, (yyvsp[-5].expression_), (yyvsp[-3].numeric_literal_value_)->long_value(), (yyvsp[-1].numeric_literal_value_)->long_value());
  }
#line 5271 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 229:
#line 1678 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseSimpleCaseExpression((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].expression_), (yyvsp[-2].simple_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 5279 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 230:
#line 1681 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseSearchedCaseExpression((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].searched_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 5287 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 231:
#line 1686 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.simple_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSimpleWhenClause>;
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 5296 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 232:
#line 1690 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.simple_when_clause_list_) = (yyvsp[-1].simple_when_clause_list_);
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 5305 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 233:
#line 1696 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.simple_when_clause_) = new quickstep::ParseSimpleWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5313 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 234:
#line 1701 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.searched_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSearchedWhenClause>;
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 5322 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 235:
#line 1705 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.searched_when_clause_list_) = (yyvsp[-1].searched_when_clause_list_);
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 5331 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 236:
#line 1711 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.searched_when_clause_) = new quickstep::ParseSearchedWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].predicate_), (yyvsp[0].expression_));
  }
#line 5339 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 237:
#line 1716 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = NULL;
  }
#line 5347 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 238:
#line 1719 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5355 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 239:
#line 1724 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 5364 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 240:
#line 1728 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 5373 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 241:
#line 1734 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 5381 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 242:
#line 1737 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5389 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 243:
#line 1740 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5397 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 244:
#line 1743 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 5414 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 245:
#line 1755 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 5423 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 246:
#line 1759 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 5447 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 247:
#line 1778 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 5465 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 248:
#line 1791 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 5483 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 249:
#line 1806 "../SqlParser.ypp" /* yacc.c:1646  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("YEAR"));
  }
#line 5491 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 250:
#line 1809 "../SqlParser.ypp" /* yacc.c:1646  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("MONTH"));
  }
#line 5499 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 251:
#line 1812 "../SqlParser.ypp" /* yacc.c:1646  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("DAY"));
  }
#line 5507 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 252:
#line 1815 "../SqlParser.ypp" /* yacc.c:1646  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("HOUR"));
  }
#line 5515 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 253:
#line 1818 "../SqlParser.ypp" /* yacc.c:1646  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("MINUTE"));
  }
#line 5523 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 254:
#line 1821 "../SqlParser.ypp" /* yacc.c:1646  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("SECOND"));
  }
#line 5531 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 255:
#line 1826 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5540 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 256:
#line 1830 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5549 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 257:
#line 1836 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 5557 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 258:
#line 1839 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 5565 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 259:
#line 1844 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_list_) = new quickstep::PtrList<quickstep::ParseAttribute>();
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5574 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 260:
#line 1848 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_list_) = (yyvsp[-2].attribute_list_);
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5583 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 261:
#line 1855 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 5591 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 262:
#line 1858 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 5599 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 263:
#line 1861 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 5607 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 264:
#line 1864 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 5615 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 265:
#line 1867 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 5623 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 266:
#line 1870 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 5631 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 267:
#line 1873 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLike);
  }
#line 5639 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 268:
#line 1876 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotLike);
  }
#line 5647 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 269:
#line 1879 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kRegexMatch);
  }
#line 5655 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 270:
#line 1882 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotRegexMatch);
  }
#line 5663 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 271:
#line 1887 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
#line 5677 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 272:
#line 1898 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
#line 5685 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 273:
#line 1901 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
#line 5693 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 274:
#line 1906 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kModulo);
  }
#line 5701 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 275:
#line 1909 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
#line 5709 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 276:
#line 1912 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
#line 5717 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 277:
#line 1918 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 5726 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 278:
#line 1922 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 5735 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 279:
#line 1928 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5743 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 280:
#line 1931 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5754 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 281:
#line 1939 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5762 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 282:
#line 1942 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5770 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 283:
#line 1945 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5778 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 284:
#line 1948 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5786 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 285:
#line 1954 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.command_) = new quickstep::ParseCommand((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].command_argument_list_));
  }
#line 5794 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 286:
#line 1959 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    quickstep::PtrVector<quickstep::ParseString> *argument_list = (yyvsp[-1].command_argument_list_);
    argument_list->push_back((yyvsp[0].string_value_));
    (yyval.command_argument_list_) = argument_list;
  }
#line 5804 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 287:
#line 1964 "../SqlParser.ypp" /* yacc.c:1646  */
    { /* Epsilon, an empy match. */
    (yyval.command_argument_list_) = new quickstep::PtrVector<quickstep::ParseString>();
  }
#line 5812 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;


#line 5816 "SqlParser_gen.cpp" /* yacc.c:1646  */
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
#line 1968 "../SqlParser.ypp" /* yacc.c:1906  */


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
