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

#line 154 "SqlParser_gen.cpp" /* yacc.c:339  */

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
    TOKEN_DATE = 298,
    TOKEN_DATETIME = 299,
    TOKEN_DAY = 300,
    TOKEN_DECIMAL = 301,
    TOKEN_DEFAULT = 302,
    TOKEN_DELETE = 303,
    TOKEN_DELIMITER = 304,
    TOKEN_DESC = 305,
    TOKEN_DISTINCT = 306,
    TOKEN_DOUBLE = 307,
    TOKEN_DROP = 308,
    TOKEN_ELSE = 309,
    TOKEN_END = 310,
    TOKEN_ESCAPE_STRINGS = 311,
    TOKEN_EXISTS = 312,
    TOKEN_EXTRACT = 313,
    TOKEN_FALSE = 314,
    TOKEN_FIRST = 315,
    TOKEN_FLOAT = 316,
    TOKEN_FOR = 317,
    TOKEN_FOREIGN = 318,
    TOKEN_FROM = 319,
    TOKEN_FULL = 320,
    TOKEN_GROUP = 321,
    TOKEN_HASH = 322,
    TOKEN_HAVING = 323,
    TOKEN_HOUR = 324,
    TOKEN_IN = 325,
    TOKEN_INDEX = 326,
    TOKEN_INNER = 327,
    TOKEN_INSERT = 328,
    TOKEN_INTEGER = 329,
    TOKEN_INTERVAL = 330,
    TOKEN_INTO = 331,
    TOKEN_JOIN = 332,
    TOKEN_KEY = 333,
    TOKEN_LAST = 334,
    TOKEN_LEFT = 335,
    TOKEN_LIMIT = 336,
    TOKEN_LONG = 337,
    TOKEN_MINUTE = 338,
    TOKEN_MONTH = 339,
    TOKEN_NULL = 340,
    TOKEN_NULLS = 341,
    TOKEN_OFF = 342,
    TOKEN_ON = 343,
    TOKEN_ORDER = 344,
    TOKEN_OUTER = 345,
    TOKEN_PARTITION = 346,
    TOKEN_PARTITIONS = 347,
    TOKEN_PERCENT = 348,
    TOKEN_PRIMARY = 349,
    TOKEN_QUIT = 350,
    TOKEN_RANGE = 351,
    TOKEN_REAL = 352,
    TOKEN_REFERENCES = 353,
    TOKEN_RIGHT = 354,
    TOKEN_ROW_DELIMITER = 355,
    TOKEN_SECOND = 356,
    TOKEN_SELECT = 357,
    TOKEN_SET = 358,
    TOKEN_SMA = 359,
    TOKEN_SMALLINT = 360,
    TOKEN_SUBSTRING = 361,
    TOKEN_TABLE = 362,
    TOKEN_THEN = 363,
    TOKEN_TIME = 364,
    TOKEN_TIMESTAMP = 365,
    TOKEN_TRUE = 366,
    TOKEN_TUPLESAMPLE = 367,
    TOKEN_UNIQUE = 368,
    TOKEN_UPDATE = 369,
    TOKEN_USING = 370,
    TOKEN_VALUES = 371,
    TOKEN_VARCHAR = 372,
    TOKEN_WHEN = 373,
    TOKEN_WHERE = 374,
    TOKEN_WITH = 375,
    TOKEN_YEAR = 376,
    TOKEN_YEARMONTH = 377,
    TOKEN_EOF = 378,
    TOKEN_LEX_ERROR = 379
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 118 "../SqlParser.ypp" /* yacc.c:355  */

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

  quickstep::PtrList<quickstep::ParseOrderByItem> *order_commalist_;
  quickstep::ParseOrderByItem *order_item_;

  quickstep::PtrVector<quickstep::ParseSubqueryTableReference> *with_list_;
  quickstep::ParseSubqueryTableReference *with_list_element_;

#line 411 "SqlParser_gen.cpp" /* yacc.c:355  */
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
#line 210 "../SqlParser.ypp" /* yacc.c:358  */

/* This header needs YYSTYPE, which is defined by the %union directive above */
#include "SqlLexer_gen.hpp"
void NotSupported(const YYLTYPE *location, yyscan_t yyscanner, const std::string &feature);

#line 446 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYLAST   1215

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  136
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  96
/* YYNRULES -- Number of rules.  */
#define YYNRULES  265
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  488

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   379

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     131,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   135,     2,     2,
     132,   133,    23,    21,   134,    22,    27,    24,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   130,
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
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   585,   585,   589,   593,   597,   601,   604,   611,   614,
     617,   620,   623,   626,   629,   632,   635,   638,   644,   650,
     657,   663,   670,   679,   684,   693,   698,   703,   707,   713,
     718,   721,   724,   729,   732,   735,   738,   741,   744,   747,
     750,   753,   756,   768,   771,   774,   792,   812,   815,   818,
     823,   828,   834,   840,   849,   853,   859,   862,   867,   872,
     877,   884,   891,   895,   901,   904,   909,   912,   917,   920,
     925,   928,   947,   951,   957,   961,   967,   970,   973,   978,
     981,   988,   993,  1004,  1009,  1013,  1017,  1023,  1026,  1032,
    1040,  1043,  1046,  1052,  1057,  1060,  1065,  1069,  1073,  1077,
    1083,  1088,  1093,  1097,  1103,  1109,  1112,  1117,  1122,  1126,
    1132,  1138,  1144,  1147,  1151,  1157,  1160,  1165,  1169,  1175,
    1178,  1181,  1186,  1191,  1196,  1199,  1202,  1207,  1210,  1213,
    1216,  1219,  1222,  1225,  1228,  1233,  1236,  1241,  1245,  1249,
    1252,  1256,  1259,  1264,  1267,  1272,  1275,  1280,  1284,  1290,
    1293,  1298,  1301,  1306,  1309,  1314,  1317,  1336,  1340,  1346,
    1353,  1356,  1359,  1364,  1367,  1370,  1376,  1379,  1384,  1389,
    1398,  1403,  1412,  1417,  1420,  1425,  1428,  1433,  1439,  1445,
    1448,  1451,  1454,  1457,  1460,  1466,  1475,  1478,  1483,  1486,
    1491,  1494,  1499,  1502,  1505,  1508,  1511,  1514,  1517,  1520,
    1525,  1529,  1533,  1536,  1541,  1546,  1550,  1556,  1559,  1564,
    1568,  1574,  1579,  1583,  1589,  1594,  1597,  1602,  1606,  1612,
    1615,  1618,  1621,  1633,  1637,  1656,  1669,  1684,  1687,  1690,
    1693,  1696,  1699,  1704,  1708,  1714,  1717,  1722,  1726,  1733,
    1736,  1739,  1742,  1745,  1748,  1751,  1754,  1757,  1760,  1765,
    1776,  1779,  1784,  1787,  1790,  1796,  1800,  1806,  1809,  1817,
    1820,  1823,  1826,  1832,  1837,  1842
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
  "TOKEN_CONSTRAINT", "TOKEN_COPY", "TOKEN_CREATE", "TOKEN_DATE",
  "TOKEN_DATETIME", "TOKEN_DAY", "TOKEN_DECIMAL", "TOKEN_DEFAULT",
  "TOKEN_DELETE", "TOKEN_DELIMITER", "TOKEN_DESC", "TOKEN_DISTINCT",
  "TOKEN_DOUBLE", "TOKEN_DROP", "TOKEN_ELSE", "TOKEN_END",
  "TOKEN_ESCAPE_STRINGS", "TOKEN_EXISTS", "TOKEN_EXTRACT", "TOKEN_FALSE",
  "TOKEN_FIRST", "TOKEN_FLOAT", "TOKEN_FOR", "TOKEN_FOREIGN", "TOKEN_FROM",
  "TOKEN_FULL", "TOKEN_GROUP", "TOKEN_HASH", "TOKEN_HAVING", "TOKEN_HOUR",
  "TOKEN_IN", "TOKEN_INDEX", "TOKEN_INNER", "TOKEN_INSERT",
  "TOKEN_INTEGER", "TOKEN_INTERVAL", "TOKEN_INTO", "TOKEN_JOIN",
  "TOKEN_KEY", "TOKEN_LAST", "TOKEN_LEFT", "TOKEN_LIMIT", "TOKEN_LONG",
  "TOKEN_MINUTE", "TOKEN_MONTH", "TOKEN_NULL", "TOKEN_NULLS", "TOKEN_OFF",
  "TOKEN_ON", "TOKEN_ORDER", "TOKEN_OUTER", "TOKEN_PARTITION",
  "TOKEN_PARTITIONS", "TOKEN_PERCENT", "TOKEN_PRIMARY", "TOKEN_QUIT",
  "TOKEN_RANGE", "TOKEN_REAL", "TOKEN_REFERENCES", "TOKEN_RIGHT",
  "TOKEN_ROW_DELIMITER", "TOKEN_SECOND", "TOKEN_SELECT", "TOKEN_SET",
  "TOKEN_SMA", "TOKEN_SMALLINT", "TOKEN_SUBSTRING", "TOKEN_TABLE",
  "TOKEN_THEN", "TOKEN_TIME", "TOKEN_TIMESTAMP", "TOKEN_TRUE",
  "TOKEN_TUPLESAMPLE", "TOKEN_UNIQUE", "TOKEN_UPDATE", "TOKEN_USING",
  "TOKEN_VALUES", "TOKEN_VARCHAR", "TOKEN_WHEN", "TOKEN_WHERE",
  "TOKEN_WITH", "TOKEN_YEAR", "TOKEN_YEARMONTH", "TOKEN_EOF",
  "TOKEN_LEX_ERROR", "';'", "'\\n'", "'('", "')'", "','", "'%'", "$accept",
  "start", "sql_statement", "quit_statement", "alter_table_statement",
  "create_table_statement", "create_index_statement",
  "drop_table_statement", "column_def", "column_def_commalist",
  "data_type", "column_constraint_def", "column_constraint_def_list",
  "opt_column_constraint_def_list", "table_constraint_def",
  "table_constraint_def_commalist", "opt_table_constraint_def_commalist",
  "opt_column_list", "opt_block_properties", "opt_partition_clause",
  "partition_type", "key_value_list", "key_value", "key_string_value",
  "key_string_list", "key_integer_value", "index_type",
  "opt_index_properties", "insert_statement", "copy_from_statement",
  "opt_copy_from_params", "copy_from_params", "update_statement",
  "delete_statement", "assignment_list", "assignment_item",
  "select_statement", "with_clause", "with_list", "with_list_element",
  "select_query", "opt_all_distinct", "selection",
  "selection_item_commalist", "selection_item", "from_clause",
  "subquery_expression", "opt_sample_clause", "join_type",
  "joined_table_reference", "table_reference", "table_reference_signature",
  "table_reference_signature_primary", "joined_table_reference_commalist",
  "opt_group_by_clause", "opt_having_clause", "opt_order_by_clause",
  "opt_limit_clause", "order_commalist", "order_item",
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
      59,    10,    40,    41,    44,    37
};
# endif

#define YYPACT_NINF -230

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-230)))

#define YYTABLE_NINF -128

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     162,  -230,  -230,   -75,   181,     7,    17,    23,    19,  -230,
      41,   181,   181,  -230,   151,   129,  -230,  -230,  -230,  -230,
    -230,  -230,  -230,  -230,  -230,  -230,   -38,  -230,   -36,   166,
     181,  -230,  -230,   138,   181,   181,   181,   181,   181,  -230,
    -230,   644,   111,    93,  -230,   220,   128,  -230,  -230,  -230,
     202,  -230,  -230,  -230,  -230,    65,   273,   192,   163,   172,
    -230,    11,  -230,  -230,   295,   300,  -230,  -230,  -230,   699,
     207,  -230,   233,  -230,  -230,   218,  -230,  -230,   310,  -230,
    -230,  -230,  -230,   219,  -230,  -230,   229,   245,   786,   322,
     261,   224,  -230,  -230,   182,    35,  -230,  -230,  -230,  -230,
    -230,  -230,  -230,  -230,   928,   -12,   181,   181,   235,   181,
     181,   194,   226,   238,   181,   181,   542,  -230,  -230,   237,
     181,  -230,  -230,  -230,   542,    53,   -34,  -230,   359,  -230,
     105,   105,   983,   363,  -230,   241,    40,  -230,    18,   172,
     983,  -230,  -230,   181,   983,  -230,  -230,  -230,  -230,   983,
     300,  -230,   181,   331,    82,  -230,   360,  -230,   268,  -230,
      -7,  -230,   268,   181,   146,   181,   181,   244,  -230,   246,
    -230,   143,  1088,   841,   235,   455,   369,   372,  -230,  -230,
     624,   364,  1069,   155,    15,   983,     8,  -230,   983,  -230,
     323,   252,  -230,  -230,  -230,  -230,  -230,  -230,   317,  -230,
      60,   256,  -230,  -230,    10,    90,   197,  -230,   257,    90,
     -10,   319,  -230,  -230,    35,  -230,  -230,   259,   983,  -230,
     270,   160,   181,  -230,   983,  -230,   181,  -230,  -230,   263,
     313,   315,   267,  -230,  -230,  -230,   174,   181,   281,   146,
     181,  -230,    81,  -230,  -230,    20,    34,   542,   542,    61,
    -230,  -230,  -230,  -230,  -230,  -230,  -230,  -230,   983,   271,
     983,    47,  -230,   165,   283,   983,    55,  -230,   342,   270,
    -230,  -230,   983,   398,  -230,   125,   181,  -230,  -230,   311,
    -230,   312,   318,   326,    18,  -230,   405,   407,    90,   375,
     344,  -230,   171,  -230,   983,  -230,   270,  -230,   542,   287,
     288,   181,   417,   157,   175,  -230,   177,   396,    49,  -230,
     290,   301,  -230,   335,   296,  1069,  -230,   346,   181,  -230,
    -230,    81,  -230,  -230,   372,  -230,  -230,  -230,   983,   298,
     203,   786,  -230,   270,   337,  -230,  -230,  1069,   299,   270,
     983,  -230,    42,   -25,  -230,  -230,  -230,  -230,  -230,    18,
     197,   334,   336,  -230,   983,   542,   341,  -230,   270,    21,
     181,   181,   183,  -230,  -230,  -230,  -230,  -230,  -230,  -230,
     193,  -230,   181,  -230,  -230,  -230,  -230,   307,   146,   404,
     347,  -230,   542,  -230,  -230,   316,  -230,   234,   786,  -230,
     983,   185,  -230,  -230,  1069,   270,  -230,   437,  -230,   354,
    -230,  -230,   320,   369,   409,   365,  -230,   188,   190,  -230,
     451,   157,  -230,   181,  -230,  -230,   325,   426,  -230,    31,
     181,   983,   195,   270,  -230,   198,   338,   542,   983,   460,
    -230,   366,  -230,  -230,  -230,   200,  -230,  -230,  -230,  -230,
      14,   181,    13,  -230,   339,   270,  -230,  -230,  -230,   369,
     345,  -230,   231,  -230,   181,  -230,   181,  -230,  -230,   181,
    -230,   208,  -230,  -230,   343,  -230,   983,  -230,  -230,   377,
     348,  -230,   210,  -230,   181,  -230,   113,  -230,   181,  -230,
     213,  -230,  -230,   215,   376,  -230,   471,  -230
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     6,   265,     0,     0,     0,     0,     0,     0,    18,
     112,     0,     0,     7,     0,     0,    15,     8,    10,    11,
      13,    14,     9,    17,    12,    16,     0,   105,     0,   263,
       0,   257,   258,     0,     0,     0,     0,     0,     0,   113,
     114,     0,     0,   107,   108,     0,   145,     1,     3,     2,
       0,   106,     5,     4,   264,     0,     0,     0,     0,   166,
      25,     0,   223,   220,     0,   249,   115,    40,    29,     0,
       0,    30,    31,    34,    36,     0,    37,    39,     0,    41,
     219,    35,    38,     0,    32,    33,     0,     0,     0,     0,
       0,   116,   117,   199,   121,   187,   189,   191,   194,   195,
     196,   197,   193,   192,     0,   235,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,     0,   101,   167,     0,
       0,    91,   221,   222,     0,     0,   215,   212,     0,    43,
       0,   224,     0,     0,    44,     0,     0,   226,     0,   166,
       0,   250,   251,     0,     0,   120,   253,   254,   252,     0,
       0,   190,     0,     0,   166,   103,     0,   109,     0,   110,
       0,   255,     0,     0,     0,     0,     0,     0,    93,    66,
      27,     0,     0,     0,     0,     0,   168,   170,   172,   174,
       0,   192,     0,     0,     0,     0,   215,   209,     0,   213,
       0,     0,   229,   230,   231,   228,   232,   227,     0,   225,
       0,     0,   123,   198,     0,     0,   147,   136,   122,   141,
     124,   149,   118,   119,   186,   188,   236,     0,     0,   200,
     217,     0,     0,   100,     0,   146,     0,    92,    19,     0,
       0,     0,     0,    20,    21,    22,     0,     0,     0,    64,
       0,    42,    56,   173,   181,     0,     0,     0,     0,     0,
     239,   241,   242,   243,   244,   240,   245,   247,     0,     0,
       0,     0,   233,     0,     0,     0,     0,   210,     0,   216,
     208,    45,     0,     0,    46,   127,     0,   137,   143,   133,
     128,   129,   131,     0,     0,   140,     0,     0,   139,     0,
     151,   201,     0,   202,     0,   102,   104,   256,     0,     0,
       0,     0,     0,     0,     0,   237,     0,   235,     0,    63,
      65,    68,    28,     0,     0,     0,    47,     0,     0,    49,
      55,    57,    26,   180,   169,   171,   246,   248,     0,     0,
       0,     0,   182,   179,     0,   178,    90,     0,     0,   214,
       0,   207,     0,     0,   142,   144,   134,   130,   132,     0,
     148,     0,     0,   138,     0,     0,   153,   203,   218,     0,
       0,     0,     0,    96,   261,   262,   260,   259,    97,    95,
       0,    67,     0,    83,    84,    85,    86,    87,     0,     0,
      70,    48,     0,    51,    50,     0,    54,     0,     0,   184,
       0,     0,   177,   234,     0,   211,   204,     0,   205,     0,
     125,   126,   150,   152,     0,   155,    61,     0,     0,    58,
       0,     0,   238,     0,    24,    62,     0,     0,    23,     0,
       0,     0,     0,   175,   183,     0,     0,     0,     0,     0,
     111,     0,    59,    98,    99,     0,    74,    76,    77,    78,
       0,     0,     0,    52,     0,   176,   185,    89,   206,   135,
     154,   157,   160,   156,     0,    88,     0,    82,    80,     0,
      79,     0,    72,    73,     0,    53,     0,   161,   162,   163,
       0,    75,     0,    69,     0,   158,     0,   159,     0,    81,
       0,   164,   165,     0,     0,    60,     0,    71
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -230,  -230,  -230,  -230,  -230,  -230,  -230,  -230,  -127,  -230,
     309,   161,  -230,  -230,  -229,  -230,  -230,  -230,  -230,  -230,
    -230,    43,    27,  -230,  -230,  -230,  -230,  -230,  -230,  -230,
    -230,  -230,  -230,  -230,  -230,   264,  -230,  -230,  -230,   378,
      12,  -230,  -230,  -230,   350,  -230,  -103,  -230,  -230,  -139,
     142,  -190,    -8,  -230,  -230,  -230,  -230,  -230,  -230,    26,
    -230,  -230,    44,  -230,  -121,   240,   247,   321,   -28,   349,
     351,   394,  -132,  -230,  -230,  -230,  -230,   324,  -230,   373,
     327,  -216,  -171,   370,   108,  -107,  -230,  -230,  -230,  -230,
    -230,  -119,    -4,    94,  -230,  -230
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    14,    15,    16,    17,    18,    19,    20,   170,   171,
      89,   320,   321,   322,   233,   310,   311,   238,   380,   418,
     464,   435,   436,   437,   438,   439,   377,   414,    21,    22,
     168,   304,    23,    24,   154,   155,    25,    26,    43,    44,
     135,    41,    90,    91,    92,   139,    93,   288,   283,   206,
     207,   277,   278,   208,   290,   356,   405,   430,   450,   451,
     469,   477,   117,   118,   176,   177,   178,   179,   180,    95,
      96,    97,    98,    99,   100,   101,   186,   187,   126,   127,
     190,   221,   102,   198,   263,   103,   306,   260,   104,   144,
     149,   160,   105,   368,    28,    29
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      33,   183,   292,   184,    45,   159,   209,    42,    46,   181,
     309,   262,    27,    94,    31,   152,    32,   181,    31,   285,
      32,   457,    31,   247,    32,   188,    55,   286,   247,   247,
      57,    58,    59,    60,    61,   205,   228,    30,    51,   247,
      50,   125,   397,   458,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   245,   141,   142,   334,   146,   147,
     136,   141,   142,   141,   142,   275,   181,   188,   181,    10,
      39,   244,   209,   121,   141,   142,   141,   142,   326,   327,
     328,   141,   142,    34,   373,   462,    36,   374,   375,   124,
     145,   313,    52,   111,    31,    53,    32,    40,   353,    45,
      38,   205,   156,    46,   200,   161,   162,   287,   398,   259,
     169,   172,    94,   312,   463,   391,   161,    10,    10,    35,
     153,   276,   153,   112,   314,   220,   225,   226,   265,   273,
     305,   185,   119,   315,   210,    37,   329,   335,   402,   213,
     181,   181,   204,   120,   383,   350,   459,   246,   216,   415,
     204,    47,   209,   323,   406,   192,   332,   266,   376,   172,
     269,   234,   235,     1,   443,     2,   393,   203,   340,    54,
     148,   316,   422,   203,   227,   396,   185,   359,   481,   193,
     317,   205,   362,   211,   318,    31,    31,    32,    32,   229,
     220,   181,     3,   194,   195,   279,   296,   482,   223,   319,
     210,    46,   280,   141,   142,    46,   116,    56,     4,     5,
     281,   196,   390,   143,   230,     6,   222,   209,   156,   106,
       7,   364,   297,   262,   141,   142,   389,   107,   302,   282,
     330,   197,   333,   307,   403,   303,   172,   339,   163,   164,
       8,   407,   408,   421,   342,   231,   205,   410,   181,   365,
     366,   108,   141,   142,   411,   141,   142,    48,   344,    49,
     109,   419,     9,   467,   232,   412,   358,   279,   345,    10,
     165,   166,    46,   367,   280,   181,   239,   240,   113,  -127,
     210,    11,   281,   110,    46,   114,   468,    12,   264,   226,
      13,   141,   142,   293,   294,   115,   116,   161,   336,   337,
     387,   282,   122,   220,   357,   294,   449,   123,   369,   370,
     371,   372,   395,   129,   385,   131,   409,   226,   424,   294,
     181,   431,   226,   432,   226,   134,   220,   137,   446,   294,
     138,   447,   337,   455,   456,    31,    62,    32,    63,   128,
     472,   473,   456,   479,   226,   210,   484,   226,   485,   226,
     130,   132,    64,    65,   217,   480,   161,   161,   140,   483,
     220,   133,   423,   167,    67,    68,   191,   158,   307,   182,
     201,   224,    69,    70,   202,    10,   236,   247,   237,    71,
      72,   248,    73,   270,   261,   271,   272,   218,    74,   274,
     289,   284,   291,   445,    75,   298,   299,    76,   300,   301,
     452,   308,   341,   331,   338,   343,   346,   347,   349,   440,
      77,    78,   351,   348,   352,   354,   444,   355,    79,   360,
     361,    80,   363,   152,   378,   381,   379,   392,   382,   384,
     388,   394,   400,    81,   401,   404,   460,   440,   452,   413,
     416,    82,    83,   417,   426,    84,    85,   427,   420,   428,
     470,   429,   440,    86,   294,   161,   433,   441,    87,    31,
      62,    32,    63,    88,   219,   173,   442,   453,   476,   454,
     161,   448,   465,   486,   161,   474,    64,    65,   487,   466,
     478,   242,   386,   471,   461,   157,   295,   324,    67,    68,
     212,   399,   475,   214,   243,   325,    69,    70,   151,   189,
     215,   199,   425,    71,    72,   434,    73,     0,     0,     0,
     267,     0,    74,   268,     0,     0,     0,   174,    75,     0,
       0,    76,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    77,    78,     0,     0,     0,     0,
       0,     0,    79,     0,     0,    80,    31,    62,    32,    63,
       0,     0,   173,     0,     0,     0,     0,    81,     0,     0,
       0,     0,    10,    64,    65,    82,    83,     0,     0,    84,
      85,     0,     0,     0,     0,    67,    68,    86,     0,     0,
       0,     0,    87,    69,    70,     0,     0,   175,     0,     0,
      71,    72,     0,    73,     0,     0,     0,     0,     0,    74,
       0,     0,     0,     0,   174,    75,     0,     0,    76,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    77,    78,     0,     0,     0,     0,     0,     0,    79,
       0,     0,    80,     0,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,    81,   141,   142,     0,    31,    62,
      32,    63,    82,    83,     0,     0,    84,    85,     0,     0,
       0,     0,     0,     0,    86,    64,    65,    66,     0,    87,
       0,     0,     0,     0,   175,     0,     0,    67,    68,     0,
       0,     0,     0,     0,     0,    69,    70,     0,     0,     0,
       0,     0,    71,    72,     0,    73,     0,     0,     0,   259,
       0,    74,     0,    31,    62,    32,    63,    75,     0,     0,
      76,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,    65,     0,    77,    78,     0,     0,     0,     0,     0,
       0,    79,    67,    68,    80,     0,     0,     0,     0,     0,
      69,    70,     0,     0,     0,     0,    81,    71,    72,     0,
      73,     0,     0,     0,    82,    83,    74,     0,    84,    85,
       0,     0,    75,     0,     0,    76,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     0,    88,     0,    77,    78,
       0,     0,     0,     0,     0,     0,    79,     0,     0,    80,
      31,    62,    32,    63,     0,     0,     0,     0,     0,     0,
       0,    81,     0,     0,     0,     0,     0,    64,    65,    82,
      83,     0,     0,    84,    85,     0,     0,     0,     0,    67,
      68,    86,   124,     0,     0,     0,    87,    69,    70,     0,
       0,    88,     0,     0,    71,    72,     0,    73,     0,     0,
       0,     0,     0,    74,     0,    31,    62,    32,    63,    75,
       0,     0,    76,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,    65,     0,    77,    78,     0,     0,     0,
       0,     0,     0,    79,    67,    68,    80,     0,     0,     0,
       0,     0,    69,    70,     0,     0,     0,     0,    81,    71,
      72,     0,    73,    10,     0,     0,    82,    83,    74,     0,
      84,    85,     0,   174,    75,     0,     0,    76,    86,     0,
       0,     0,     0,    87,     0,     0,     0,     0,    88,     0,
      77,    78,     0,     0,     0,     0,     0,     0,    79,     0,
       0,    80,    31,    62,    32,    63,     0,     0,     0,     0,
       0,     0,     0,    81,     0,     0,     0,     0,     0,    64,
     150,    82,    83,     0,     0,    84,    85,     0,     0,     0,
       0,    67,    68,    86,     0,     0,     0,     0,    87,    69,
      70,     0,     0,   175,     0,     0,    71,    72,     0,    73,
       0,     0,     0,     0,     0,    74,     0,    31,    62,    32,
      63,    75,     0,     0,    76,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,    65,     0,    77,    78,     0,
       0,     0,     0,     0,     0,    79,    67,    68,    80,     0,
       0,     0,     0,     0,    69,    70,     0,     0,     0,     0,
      81,    71,    72,     0,    73,     0,     0,     0,    82,    83,
      74,     0,    84,    85,     0,     0,    75,     0,     0,    76,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     0,
      88,     0,    77,    78,     0,     0,     0,     0,     0,     0,
      79,     0,     0,    80,    62,     0,    63,     0,     0,     0,
       0,     0,     0,     0,     0,    81,     0,     0,     0,     0,
      64,   150,     0,    82,    83,     0,     0,    84,    85,     0,
       0,     0,    67,    68,     0,    86,     0,     0,     0,     0,
      87,    70,     0,     0,     0,    88,     0,    71,    72,     0,
      73,    67,    68,     0,     0,     0,    74,     0,     0,     0,
      70,     0,     0,     0,     0,    76,    71,    72,     0,    73,
       0,     0,     0,     0,     0,    74,     0,     0,    77,    78,
       0,     0,     0,     0,    76,     0,    79,     0,     0,    80,
       0,     0,     0,     0,     0,     0,     0,    77,   241,     0,
       0,    81,     0,     0,     0,    79,     0,     0,     0,    82,
       0,     0,     0,    84,    85,     0,     0,     0,     0,     0,
      81,    86,     0,     0,     0,     0,    87,     0,    82,     0,
       0,     0,    84,    85,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,    87
};

static const yytype_int16 yycheck[] =
{
       4,   120,   218,   124,    12,   108,   138,    11,    12,   116,
     239,   182,     0,    41,     4,    27,     6,   124,     4,   209,
       6,     7,     4,     8,     6,    59,    30,    37,     8,     8,
      34,    35,    36,    37,    38,   138,   163,   112,    26,     8,
      78,    69,    67,    29,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,   175,    21,    22,    10,    23,    24,
      88,    21,    22,    21,    22,   204,   173,    59,   175,   107,
      29,   174,   204,    61,    21,    22,    21,    22,    17,    18,
      19,    21,    22,    76,    35,    72,    69,    38,    39,   123,
      94,    10,   128,    28,     4,   131,     6,    56,   288,   107,
      81,   204,   106,   107,   132,   109,   110,   117,   133,    75,
     114,   115,   140,   240,   101,   331,   120,   107,   107,   112,
     132,    31,   132,    58,    43,   153,   133,   134,   113,    69,
     237,   123,   121,    52,   138,   112,    75,    90,   354,   143,
     247,   248,   132,   132,   315,   284,   132,   175,   152,   378,
     132,     0,   284,   133,   133,    50,   259,   185,   109,   163,
     188,   165,   166,     1,   133,     3,   337,   133,   113,     3,
     135,    90,   388,   133,   162,   133,   123,   298,    65,    74,
      99,   284,   301,   139,   103,     4,     4,     6,     6,    43,
     218,   298,    30,    88,    89,    70,   224,    84,   154,   118,
     204,   205,    77,    21,    22,   209,   124,    69,    46,    47,
      85,   106,     9,    31,    68,    53,   134,   349,   222,   108,
      58,    64,   226,   394,    21,    22,   329,   134,    54,   104,
     258,   126,   260,   237,   355,    61,   240,   265,    44,    45,
      78,   360,   361,     9,   272,    99,   349,    54,   355,    92,
      93,    31,    21,    22,    61,    21,    22,   128,   133,   130,
     132,   382,   100,    32,   118,   372,   294,    70,   276,   107,
      44,    45,   276,   116,    77,   382,   133,   134,     5,    82,
     284,   119,    85,    81,   288,    93,    55,   125,   133,   134,
     128,    21,    22,   133,   134,   132,   124,   301,   133,   134,
     328,   104,     7,   331,   133,   134,   427,     7,   133,   134,
     133,   134,   340,    80,   318,     5,   133,   134,   133,   134,
     427,   133,   134,   133,   134,    80,   354,     5,   133,   134,
      69,   133,   134,   133,   134,     4,     5,     6,     7,   132,
     459,   133,   134,   133,   134,   349,   133,   134,   133,   134,
     132,   132,    21,    22,    23,   474,   360,   361,   134,   478,
     388,   132,   390,   125,    33,    34,     7,   132,   372,   132,
       7,    11,    41,    42,   133,   107,   132,     8,   132,    48,
      49,     9,    51,    60,    20,   133,    69,    56,    57,   133,
      71,   134,   133,   421,    63,   132,    83,    66,    83,   132,
     428,   120,    60,   132,   121,     7,    95,    95,    82,   413,
      79,    80,     7,    95,     7,    40,   420,    73,    87,   132,
     132,    90,     5,    27,   134,    90,   125,    90,   132,    83,
     132,   132,    98,   102,    98,    94,   440,   441,   466,   132,
      36,   110,   111,    96,     7,   114,   115,    93,   132,    40,
     454,    86,   456,   122,   134,   459,     5,   132,   127,     4,
       5,     6,     7,   132,   133,    10,    40,     7,    91,   103,
     474,   133,   133,    97,   478,   132,    21,    22,     7,   134,
     132,   172,   321,   456,   441,   107,   222,   247,    33,    34,
     140,   349,   466,   144,   173,   248,    41,    42,   104,   126,
     149,   131,   394,    48,    49,   411,    51,    -1,    -1,    -1,
     186,    -1,    57,   186,    -1,    -1,    -1,    62,    63,    -1,
      -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    90,     4,     5,     6,     7,
      -1,    -1,    10,    -1,    -1,    -1,    -1,   102,    -1,    -1,
      -1,    -1,   107,    21,    22,   110,   111,    -1,    -1,   114,
     115,    -1,    -1,    -1,    -1,    33,    34,   122,    -1,    -1,
      -1,    -1,   127,    41,    42,    -1,    -1,   132,    -1,    -1,
      48,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    62,    63,    -1,    -1,    66,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    90,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,   102,    21,    22,    -1,     4,     5,
       6,     7,   110,   111,    -1,    -1,   114,   115,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    21,    22,    23,    -1,   127,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,
      -1,    -1,    48,    49,    -1,    51,    -1,    -1,    -1,    75,
      -1,    57,    -1,     4,     5,     6,     7,    63,    -1,    -1,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    33,    34,    90,    -1,    -1,    -1,    -1,    -1,
      41,    42,    -1,    -1,    -1,    -1,   102,    48,    49,    -1,
      51,    -1,    -1,    -1,   110,   111,    57,    -1,   114,   115,
      -1,    -1,    63,    -1,    -1,    66,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,   132,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    90,
       4,     5,     6,     7,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    21,    22,   110,
     111,    -1,    -1,   114,   115,    -1,    -1,    -1,    -1,    33,
      34,   122,   123,    -1,    -1,    -1,   127,    41,    42,    -1,
      -1,   132,    -1,    -1,    48,    49,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    57,    -1,     4,     5,     6,     7,    63,
      -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    79,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    33,    34,    90,    -1,    -1,    -1,
      -1,    -1,    41,    42,    -1,    -1,    -1,    -1,   102,    48,
      49,    -1,    51,   107,    -1,    -1,   110,   111,    57,    -1,
     114,   115,    -1,    62,    63,    -1,    -1,    66,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,   132,    -1,
      79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    90,     4,     5,     6,     7,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    21,
      22,   110,   111,    -1,    -1,   114,   115,    -1,    -1,    -1,
      -1,    33,    34,   122,    -1,    -1,    -1,    -1,   127,    41,
      42,    -1,    -1,   132,    -1,    -1,    48,    49,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    57,    -1,     4,     5,     6,
       7,    63,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    79,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    33,    34,    90,    -1,
      -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,
     102,    48,    49,    -1,    51,    -1,    -1,    -1,   110,   111,
      57,    -1,   114,   115,    -1,    -1,    63,    -1,    -1,    66,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
     132,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    90,     5,    -1,     7,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      21,    22,    -1,   110,   111,    -1,    -1,   114,   115,    -1,
      -1,    -1,    33,    34,    -1,   122,    -1,    -1,    -1,    -1,
     127,    42,    -1,    -1,    -1,   132,    -1,    48,    49,    -1,
      51,    33,    34,    -1,    -1,    -1,    57,    -1,    -1,    -1,
      42,    -1,    -1,    -1,    -1,    66,    48,    49,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    66,    -1,    87,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    -1,
      -1,   102,    -1,    -1,    -1,    87,    -1,    -1,    -1,   110,
      -1,    -1,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,
     102,   122,    -1,    -1,    -1,    -1,   127,    -1,   110,    -1,
      -1,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,   127
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,    30,    46,    47,    53,    58,    78,   100,
     107,   119,   125,   128,   137,   138,   139,   140,   141,   142,
     143,   164,   165,   168,   169,   172,   173,   176,   230,   231,
     112,     4,     6,   228,    76,   112,    69,   112,    81,    29,
      56,   177,   228,   174,   175,   188,   228,     0,   128,   130,
      78,   176,   128,   131,     3,   228,    69,   228,   228,   228,
     228,   228,     5,     7,    21,    22,    23,    33,    34,    41,
      42,    48,    49,    51,    57,    63,    66,    79,    80,    87,
      90,   102,   110,   111,   114,   115,   122,   127,   132,   146,
     178,   179,   180,   182,   204,   205,   206,   207,   208,   209,
     210,   211,   218,   221,   224,   228,   108,   134,    31,   132,
      81,    28,    58,     5,    93,   132,   124,   198,   199,   121,
     132,   176,     7,     7,   123,   204,   214,   215,   132,    80,
     132,     5,   132,   132,    80,   176,   204,     5,    69,   181,
     134,    21,    22,    31,   225,   228,    23,    24,   135,   226,
      22,   207,    27,   132,   170,   171,   228,   175,   132,   182,
     227,   228,   228,    44,    45,    44,    45,   125,   166,   228,
     144,   145,   228,    10,    62,   132,   200,   201,   202,   203,
     204,   221,   132,   227,   200,   123,   212,   213,    59,   215,
     216,     7,    50,    74,    88,    89,   106,   126,   219,   219,
     204,     7,   133,   133,   132,   182,   185,   186,   189,   208,
     228,   198,   180,   228,   205,   206,   228,    23,    56,   133,
     204,   217,   134,   198,    11,   133,   134,   176,   144,    43,
      68,    99,   118,   150,   228,   228,   132,   132,   153,   133,
     134,    80,   146,   203,   182,   200,   204,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    75,
     223,    20,   218,   220,   133,   113,   204,   213,   216,   204,
      60,   133,    69,    69,   133,   185,    31,   187,   188,    70,
      77,    85,   104,   184,   134,   187,    37,   117,   183,    71,
     190,   133,   217,   133,   134,   171,   204,   228,   132,    83,
      83,   132,    54,    61,   167,   221,   222,   228,   120,   150,
     151,   152,   144,    10,    43,    52,    90,    99,   103,   118,
     147,   148,   149,   133,   201,   202,    17,    18,    19,    75,
     204,   132,   182,   204,    10,    90,   133,   134,   121,   204,
     113,    60,   204,     7,   133,   188,    95,    95,    95,    82,
     185,     7,     7,   187,    40,    73,   191,   133,   204,   200,
     132,   132,   227,     5,    64,    92,    93,   116,   229,   133,
     134,   133,   134,    35,    38,    39,   109,   162,   134,   125,
     154,    90,   132,   218,    83,   228,   147,   204,   132,   182,
       9,   217,    90,   218,   132,   204,   133,    67,   133,   186,
      98,    98,   217,   200,    94,   192,   133,   227,   227,   133,
      54,    61,   221,   132,   163,   150,    36,    96,   155,   200,
     132,     9,   217,   204,   133,   220,     7,    93,    40,    86,
     193,   133,   133,     5,   229,   157,   158,   159,   160,   161,
     228,   132,    40,   133,   228,   204,   133,   133,   133,   200,
     194,   195,   204,     7,   103,   133,   134,     7,    29,   132,
     228,   157,    72,   101,   156,   133,   134,    32,    55,   196,
     228,   158,   227,   133,   132,   195,    91,   197,   132,   133,
     227,    65,    84,   227,   133,   133,    97,     7
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   136,   137,   137,   137,   137,   137,   137,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   139,   140,
     140,   140,   140,   141,   142,   143,   144,   145,   145,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   147,   147,   147,
     147,   147,   147,   147,   148,   148,   149,   149,   150,   150,
     150,   150,   151,   151,   152,   152,   153,   153,   154,   154,
     155,   155,   156,   156,   157,   157,   158,   158,   158,   159,
     159,   160,   161,   162,   162,   162,   162,   163,   163,   164,
     164,   164,   164,   165,   166,   166,   167,   167,   167,   167,
     168,   169,   170,   170,   171,   172,   172,   173,   174,   174,
     175,   176,   177,   177,   177,   178,   178,   179,   179,   180,
     180,   180,   181,   182,   183,   183,   183,   184,   184,   184,
     184,   184,   184,   184,   184,   185,   185,   186,   186,   186,
     186,   186,   186,   187,   187,   188,   188,   189,   189,   190,
     190,   191,   191,   192,   192,   193,   193,   194,   194,   195,
     196,   196,   196,   197,   197,   197,   198,   198,   199,   200,
     200,   201,   201,   202,   202,   203,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   203,   204,   204,   205,   205,
     206,   206,   207,   207,   207,   207,   207,   207,   207,   207,
     208,   208,   208,   208,   209,   210,   210,   211,   211,   212,
     212,   213,   214,   214,   215,   216,   216,   217,   217,   218,
     218,   218,   218,   218,   218,   218,   218,   219,   219,   219,
     219,   219,   219,   220,   220,   221,   221,   222,   222,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   224,
     225,   225,   226,   226,   226,   227,   227,   228,   228,   229,
     229,   229,   229,   230,   231,   231
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
       3,     9,     0,     1,     1,     1,     1,     1,     3,     3,
       2,     1,     2,     3,     0,     3,     3,     0,     1,     1,
       2,     1,     2,     1,     2,     6,     1,     2,     3,     2,
       2,     1,     3,     1,     2,     1,     4,     1,     3,     0,
       3,     0,     2,     0,     3,     0,     2,     1,     3,     3,
       0,     1,     1,     0,     2,     2,     0,     1,     2,     3,
       1,     3,     1,     2,     1,     5,     6,     4,     3,     3,
       3,     2,     3,     5,     4,     6,     3,     1,     3,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       3,     4,     4,     5,     6,     6,     8,     5,     4,     1,
       2,     4,     1,     2,     4,     0,     2,     1,     3,     1,
       1,     2,     2,     1,     2,     3,     2,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     2,     2,     0
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
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1905 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_NAME  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1915 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1925 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1935 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 7: /* TOKEN_UNSIGNED_NUMVAL  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 1945 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 138: /* sql_statement  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1955 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 139: /* quit_statement  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 1965 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 140: /* alter_table_statement  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1975 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 141: /* create_table_statement  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 1985 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 142: /* create_index_statement  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1995 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 143: /* drop_table_statement  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 2005 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 144: /* column_def  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 2015 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 145: /* column_def_commalist  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 2025 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 146: /* data_type  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 2035 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 147: /* column_constraint_def  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 2045 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 148: /* column_constraint_def_list  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2055 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 149: /* opt_column_constraint_def_list  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2065 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 153: /* opt_column_list  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2075 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* opt_block_properties  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).block_properties_) != nullptr) {
    delete ((*yyvaluep).block_properties_);
  }
}
#line 2085 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* opt_partition_clause  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).partition_clause_) != nullptr) {
    delete ((*yyvaluep).partition_clause_);
  }
}
#line 2095 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* partition_type  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2105 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* key_value_list  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2115 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* key_value  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 2125 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* key_string_value  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 2135 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* key_string_list  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 2145 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* key_integer_value  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 2155 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* index_type  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2165 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 163: /* opt_index_properties  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2175 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 164: /* insert_statement  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 2185 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 165: /* copy_from_statement  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_statement_) != nullptr) {
    delete ((*yyvaluep).copy_from_statement_);
  }
}
#line 2195 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 166: /* opt_copy_from_params  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2205 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 167: /* copy_from_params  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2215 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* update_statement  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2225 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* delete_statement  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2235 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* assignment_list  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2245 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* assignment_item  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2255 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* select_statement  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_statement_) != nullptr) {
    delete ((*yyvaluep).select_statement_);
  }
}
#line 2265 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* with_clause  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2275 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* with_list  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2285 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* with_list_element  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2295 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* select_query  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2305 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* selection  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2315 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* selection_item_commalist  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2325 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* selection_item  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2335 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* from_clause  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2345 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* subquery_expression  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2355 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* opt_sample_clause  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_sample_clause_) != nullptr) {
    delete ((*yyvaluep).opt_sample_clause_);
  }
}
#line 2365 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* join_type  */
#line 574 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2371 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* joined_table_reference  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2381 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* table_reference  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2391 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* table_reference_signature  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2401 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* table_reference_signature_primary  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2411 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* joined_table_reference_commalist  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2421 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 190: /* opt_group_by_clause  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2431 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 191: /* opt_having_clause  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2441 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 192: /* opt_order_by_clause  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2451 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 193: /* opt_limit_clause  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2461 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 194: /* order_commalist  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2471 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 195: /* order_item  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2481 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 196: /* opt_order_direction  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2491 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 197: /* opt_nulls_first  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2501 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 198: /* opt_where_clause  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2511 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 199: /* where_clause  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2521 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 200: /* or_expression  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2531 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 201: /* and_expression  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2541 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 202: /* not_expression  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2551 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 203: /* predicate_expression_base  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2561 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 204: /* add_expression  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2571 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 205: /* multiply_expression  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2581 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 206: /* unary_expression  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2591 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 207: /* expression_base  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2601 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 208: /* function_call  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2611 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 209: /* extract_function  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2621 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 210: /* substr_function  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2631 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 211: /* case_expression  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2641 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 212: /* simple_when_clause_list  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_list_);
  }
}
#line 2651 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 213: /* simple_when_clause  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_);
  }
}
#line 2661 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 214: /* searched_when_clause_list  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_list_);
  }
}
#line 2671 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 215: /* searched_when_clause  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_);
  }
}
#line 2681 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 216: /* opt_else_clause  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2691 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 217: /* expression_list  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2701 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 218: /* literal_value  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2711 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 219: /* datetime_unit  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2721 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 220: /* literal_value_commalist  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2731 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 221: /* attribute_ref  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2741 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 222: /* attribute_ref_list  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2751 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 223: /* comparison_operation  */
#line 571 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2757 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 224: /* unary_operation  */
#line 572 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2763 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 225: /* add_operation  */
#line 573 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2769 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 226: /* multiply_operation  */
#line 573 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2775 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 227: /* name_commalist  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 2785 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 228: /* any_name  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2795 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 229: /* boolean_value  */
#line 570 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2801 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 230: /* command  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_) != nullptr) {
    delete ((*yyvaluep).command_);
  }
}
#line 2811 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 231: /* command_argument_list  */
#line 576 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_argument_list_) != nullptr) {
    delete ((*yyvaluep).command_argument_list_);
  }
}
#line 2821 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 585 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3118 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 589 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3127 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 593 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3136 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 5:
#line 597 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3145 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 601 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    YYABORT;
  }
#line 3153 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 604 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 3162 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 611 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3170 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 9:
#line 614 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].copy_from_statement_);
  }
#line 3178 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 10:
#line 617 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 3186 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 11:
#line 620 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3194 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 12:
#line 623 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 3202 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 13:
#line 626 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 3210 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 14:
#line 629 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 3218 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 15:
#line 632 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 3226 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 16:
#line 635 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].select_statement_);
  }
#line 3234 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 17:
#line 638 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 3242 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 18:
#line 644 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3250 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 19:
#line 650 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3262 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 20:
#line 657 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3273 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 663 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3285 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 670 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3297 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 679 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].attribute_definition_list_), (yyvsp[-1].block_properties_), (yyvsp[0].partition_clause_));
  }
#line 3305 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 684 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].key_value_list_)) {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_), (yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].key_value_list_));
    } else {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_));
    }
  }
#line 3317 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 693 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 3325 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 698 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 3333 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 703 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3342 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 707 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3351 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 713 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 3361 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 718 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3369 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 721 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3377 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 724 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3387 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 729 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3395 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 732 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3403 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 735 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3411 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 738 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3419 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 741 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3427 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 744 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3435 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 747 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3443 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 750 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3451 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 753 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3459 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 756 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3476 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 768 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3484 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 771 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3492 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 774 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3515 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 792 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3538 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 812 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3546 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 815 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3554 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 818 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3564 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 823 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3574 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 828 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3585 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 834 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3596 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 840 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3608 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 849 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3617 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 853 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3626 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 859 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3634 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 862 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3642 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 867 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3652 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 872 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3662 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 877 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3674 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 884 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3684 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 891 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3693 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 895 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3702 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 901 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3710 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 904 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = $1; */
  }
#line 3718 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 909 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = nullptr;
  }
#line 3726 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 912 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-1].attribute_list_);
  }
#line 3734 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 917 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = nullptr;
  }
#line 3742 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 920 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].key_value_list_));
  }
#line 3750 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 925 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.partition_clause_) = nullptr;
  }
#line 3758 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 928 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3780 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 947 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::PartitionSchemeHeader::PartitionType::kHash));
  }
#line 3789 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 951 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::PartitionSchemeHeader::PartitionType::kRange));
  }
#line 3798 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 957 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3807 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 961 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3816 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 967 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 3824 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 970 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 3832 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 973 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 3840 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 978 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 3848 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 981 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_),
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 3858 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 988 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 3866 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 993 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 3880 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 1004 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Defaults to BitWeavingV, but IndexProperties can change this to H.
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBitWeavingV));
  }
#line 3890 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 1009 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
#line 3899 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 1013 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
#line 3908 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 1017 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kSMA));
  }
#line 3917 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 1023 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 3925 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 1026 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 3933 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 1032 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 3946 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 1040 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertTuple((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 3954 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 1043 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-3]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), nullptr);
  }
#line 3962 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 1046 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-4]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), (yyvsp[-4].with_list_));
  }
#line 3970 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 1052 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_statement_) = new quickstep::ParseStatementCopyFrom((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].copy_from_params_));
  }
#line 3978 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 1057 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = nullptr;
  }
#line 3986 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 1060 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-1].copy_from_params_);
  }
#line 3994 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 1065 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 4003 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 1069 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 4012 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 1073 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 4021 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 1077 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 4030 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 1083 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 4038 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 1088 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 4046 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 1093 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 4055 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 1097 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 4064 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 1103 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 4072 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 1109 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].select_query_), nullptr);
  }
#line 4080 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 1112 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].select_query_), (yyvsp[-1].with_list_));
  }
#line 4088 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 1117 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 4096 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 1122 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4105 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 1126 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4114 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 1132 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 4123 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 1139 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].selection_), (yyvsp[-5].table_reference_list_), (yyvsp[-4].predicate_), (yyvsp[-3].opt_group_by_clause_), (yyvsp[-2].opt_having_clause_), (yyvsp[-1].opt_order_by_clause_), (yyvsp[0].opt_limit_clause_));
  }
#line 4131 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 1144 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 4139 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 1147 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "ALL in selection");
    YYERROR;
  }
#line 4148 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 1151 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "DISTINCT in selection");
    YYERROR;
  }
#line 4157 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 1157 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4165 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 1160 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 4173 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 1165 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4182 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 1169 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4191 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 1175 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 4199 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 1178 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 4207 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 1181 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 4215 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 1186 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[0].table_reference_list_);
  }
#line 4223 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 1191 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].select_query_));
  }
#line 4231 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 1196 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = NULL;
  }
#line 4239 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 1199 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, true, (yyvsp[-1].numeric_literal_value_));
  }
#line 4247 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 1202 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].numeric_literal_value_));
  }
#line 4255 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 1207 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4263 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 1210 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4271 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 1213 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4279 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 1216 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4287 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 1219 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4295 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 1222 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4303 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 1225 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4311 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 1228 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4319 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 1233 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseJoinedTableReference((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].join_type_), (yyvsp[-5].table_reference_), (yyvsp[-2].table_reference_), (yyvsp[0].predicate_));
  }
#line 4327 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 1236 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = (yyvsp[0].table_reference_);
  }
#line 4335 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 1241 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4344 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 1245 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].opt_sample_clause_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4353 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 1249 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].opt_sample_clause_));
  }
#line 4361 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 1252 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4370 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 1256 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].function_call_));
  }
#line 4378 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 1259 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = (yyvsp[-1].table_reference_);
  }
#line 4386 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 1264 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4394 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 1267 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4402 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 1272 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4410 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 1275 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4418 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 1280 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4427 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 1284 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4436 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 1290 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 4444 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 1293 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4452 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 1298 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4460 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 1301 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4468 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 1306 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4476 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 1309 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4484 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 1314 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4492 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 1317 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4514 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 1336 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4523 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 1340 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4532 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 1346 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 4542 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 1353 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4550 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 1356 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4558 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 1359 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4566 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 1364 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4574 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 1367 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4582 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 1370 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4590 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 1376 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 4598 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 1379 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4606 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 1384 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4614 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 1389 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4628 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 1398 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4636 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 1403 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4650 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 1412 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4658 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 1417 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4666 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 1420 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4674 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 1425 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4682 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 1428 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 4692 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 1433 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4703 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 1439 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4714 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 1445 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4722 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 1448 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 4730 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 1451 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateExists((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].subquery_expression_));
  }
#line 4738 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 1454 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-2].expression_), (yyvsp[0].subquery_expression_));
  }
#line 4746 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 1457 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-1].expression_list_));
  }
#line 4754 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 1460 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-2]).first_line,
        (yylsp[-2]).first_column,
        new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-3].expression_), (yyvsp[0].subquery_expression_)));
  }
#line 4765 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 1466 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line,
        (yylsp[-4]).first_column,
        new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-1].expression_list_)));
  }
#line 4776 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1475 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4784 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 1478 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4792 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 1483 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4800 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 1486 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4808 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 1491 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].unary_operation_), (yyvsp[0].expression_));
  }
#line 4816 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 1494 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4824 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 1499 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 4832 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 1502 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 4840 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 1505 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 4848 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 1508 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4856 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 1511 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4864 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 1514 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4872 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 1517 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 4880 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 1520 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].subquery_expression_);
  }
#line 4888 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1525 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 4897 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1529 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 4906 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1533 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, false, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4914 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1536 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-4]).first_line, (yylsp[-4]).first_column, true, (yyvsp[-4].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4922 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1541 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseExtractFunction((yylsp[-5]).first_line, (yylsp[-5]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].expression_));
  }
#line 4930 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1546 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSubstringFunction(
        (yylsp[-5]).first_line, (yylsp[-5]).first_column, (yyvsp[-3].expression_), (yyvsp[-1].numeric_literal_value_)->long_value());
  }
#line 4939 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1550 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSubstringFunction(
        (yylsp[-7]).first_line, (yylsp[-7]).first_column, (yyvsp[-5].expression_), (yyvsp[-3].numeric_literal_value_)->long_value(), (yyvsp[-1].numeric_literal_value_)->long_value());
  }
#line 4948 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1556 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSimpleCaseExpression((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].expression_), (yyvsp[-2].simple_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 4956 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1559 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSearchedCaseExpression((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].searched_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 4964 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1564 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSimpleWhenClause>;
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 4973 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1568 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_list_) = (yyvsp[-1].simple_when_clause_list_);
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 4982 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1574 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_) = new quickstep::ParseSimpleWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4990 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1579 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSearchedWhenClause>;
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 4999 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1583 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_list_) = (yyvsp[-1].searched_when_clause_list_);
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 5008 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1589 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_) = new quickstep::ParseSearchedWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].predicate_), (yyvsp[0].expression_));
  }
#line 5016 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1594 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = NULL;
  }
#line 5024 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1597 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5032 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1602 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 5041 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1606 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 5050 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1612 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 5058 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1615 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5066 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1618 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5074 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1621 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5091 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1633 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 5100 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1637 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5124 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1656 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5142 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1669 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5160 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1684 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("YEAR"));
  }
#line 5168 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1687 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("MONTH"));
  }
#line 5176 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1690 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("DAY"));
  }
#line 5184 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1693 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("HOUR"));
  }
#line 5192 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1696 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("MINUTE"));
  }
#line 5200 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1699 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("SECOND"));
  }
#line 5208 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1704 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5217 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1708 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5226 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1714 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 5234 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1717 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 5242 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1722 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = new quickstep::PtrList<quickstep::ParseAttribute>();
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5251 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1726 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-2].attribute_list_);
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5260 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1733 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 5268 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1736 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 5276 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1739 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 5284 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1742 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 5292 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1745 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 5300 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1748 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 5308 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1751 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLike);
  }
#line 5316 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1754 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotLike);
  }
#line 5324 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1757 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kRegexMatch);
  }
#line 5332 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1760 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotRegexMatch);
  }
#line 5340 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1765 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
#line 5354 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1776 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
#line 5362 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1779 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
#line 5370 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1784 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kModulo);
  }
#line 5378 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1787 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
#line 5386 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1790 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
#line 5394 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1796 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 5403 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1800 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 5412 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1806 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5420 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1809 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5431 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1817 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5439 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1820 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5447 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1823 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5455 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1826 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5463 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1832 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.command_) = new quickstep::ParseCommand((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].command_argument_list_));
  }
#line 5471 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1837 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    quickstep::PtrVector<quickstep::ParseString> *argument_list = (yyvsp[-1].command_argument_list_);
    argument_list->push_back((yyvsp[0].string_value_));
    (yyval.command_argument_list_) = argument_list;
  }
#line 5481 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1842 "../SqlParser.ypp" /* yacc.c:1661  */
    { /* Epsilon, an empy match. */
    (yyval.command_argument_list_) = new quickstep::PtrVector<quickstep::ParseString>();
  }
#line 5489 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;


#line 5493 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 1846 "../SqlParser.ypp" /* yacc.c:1906  */


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
