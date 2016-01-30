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
#line 33 "../SqlParser.ypp" /* yacc.c:339  */


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

#line 62 "../SqlParser.ypp" /* yacc.c:339  */

#include <cstdlib>
#include <string>
#include <utility>

#include "parser/ParseAssignment.hpp"
#include "parser/ParseAttributeDefinition.hpp"
#include "parser/ParseBasicExpressions.hpp"
#include "parser/ParseExpression.hpp"
#include "parser/ParseGroupBy.hpp"
#include "parser/ParseHaving.hpp"
#include "parser/ParseLimit.hpp"
#include "parser/ParseLiteralValue.hpp"
#include "parser/ParseOrderBy.hpp"
#include "parser/ParsePredicate.hpp"
#include "parser/ParserUtil.hpp"
#include "parser/ParseSelect.hpp"
#include "parser/ParseSelectionClause.hpp"
#include "parser/ParseSimpleTableReference.hpp"
#include "parser/ParseStatement.hpp"
#include "parser/ParseString.hpp"
#include "parser/ParseSubqueryExpression.hpp"
#include "parser/ParseSubqueryTableReference.hpp"
#include "parser/ParseTableReference.hpp"
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

#line 143 "SqlParser_gen.cpp" /* yacc.c:339  */

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
    TOKEN_NAME = 258,
    TOKEN_STRING_SINGLE_QUOTED = 259,
    TOKEN_STRING_DOUBLE_QUOTED = 260,
    TOKEN_UNSIGNED_NUMVAL = 261,
    TOKEN_OR = 262,
    TOKEN_AND = 263,
    TOKEN_NOT = 264,
    TOKEN_EQ = 265,
    TOKEN_LT = 266,
    TOKEN_LEQ = 267,
    TOKEN_GT = 268,
    TOKEN_GEQ = 269,
    TOKEN_NEQ = 270,
    TOKEN_LIKE = 271,
    TOKEN_BETWEEN = 272,
    TOKEN_IS = 273,
    UNARY_PLUS = 274,
    UNARY_MINUS = 275,
    TOKEN_ADD = 276,
    TOKEN_ALL = 277,
    TOKEN_ALTER = 278,
    TOKEN_AS = 279,
    TOKEN_ASC = 280,
    TOKEN_BIGINT = 281,
    TOKEN_BIT = 282,
    TOKEN_BLOOM_FILTER_INDEX = 283,
    TOKEN_BY = 284,
    TOKEN_CHARACTER = 285,
    TOKEN_CHECK = 286,
    TOKEN_COLUMN = 287,
    TOKEN_CONSTRAINT = 288,
    TOKEN_COPY = 289,
    TOKEN_CREATE = 290,
    TOKEN_DATE = 291,
    TOKEN_DATETIME = 292,
    TOKEN_DECIMAL = 293,
    TOKEN_DEFAULT = 294,
    TOKEN_DELETE = 295,
    TOKEN_DELIMITER = 296,
    TOKEN_DESC = 297,
    TOKEN_DISTINCT = 298,
    TOKEN_DOUBLE = 299,
    TOKEN_DROP = 300,
    TOKEN_ESCAPE_STRINGS = 301,
    TOKEN_FALSE = 302,
    TOKEN_FIRST = 303,
    TOKEN_FLOAT = 304,
    TOKEN_FOREIGN = 305,
    TOKEN_FROM = 306,
    TOKEN_FULL = 307,
    TOKEN_GROUP = 308,
    TOKEN_HAVING = 309,
    TOKEN_INDEX = 310,
    TOKEN_INNER = 311,
    TOKEN_INSERT = 312,
    TOKEN_INTEGER = 313,
    TOKEN_INTERVAL = 314,
    TOKEN_INTO = 315,
    TOKEN_JOIN = 316,
    TOKEN_KEY = 317,
    TOKEN_LAST = 318,
    TOKEN_LEFT = 319,
    TOKEN_LIMIT = 320,
    TOKEN_LONG = 321,
    TOKEN_NULL = 322,
    TOKEN_NULLS = 323,
    TOKEN_OFF = 324,
    TOKEN_ON = 325,
    TOKEN_ORDER = 326,
    TOKEN_OUTER = 327,
    TOKEN_PRIMARY = 328,
    TOKEN_QUIT = 329,
    TOKEN_REAL = 330,
    TOKEN_REFERENCES = 331,
    TOKEN_RIGHT = 332,
    TOKEN_ROW_DELIMITER = 333,
    TOKEN_SELECT = 334,
    TOKEN_SET = 335,
    TOKEN_SMALLINT = 336,
    TOKEN_TABLE = 337,
    TOKEN_TIME = 338,
    TOKEN_TIMESTAMP = 339,
    TOKEN_TRUE = 340,
    TOKEN_UNIQUE = 341,
    TOKEN_UPDATE = 342,
    TOKEN_USING = 343,
    TOKEN_VALUES = 344,
    TOKEN_VARCHAR = 345,
    TOKEN_WHERE = 346,
    TOKEN_WITH = 347,
    TOKEN_YEARMONTH = 348,
    TOKEN_EOF = 349,
    TOKEN_LEX_ERROR = 350
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 105 "../SqlParser.ypp" /* yacc.c:355  */

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

  quickstep::ParseCopyFromParams *copy_from_params_;

  quickstep::ParseAssignment *assignment_;
  quickstep::PtrList<quickstep::ParseAssignment> *assignment_list_;

  quickstep::ParseStatement *statement_;
  quickstep::ParseStatementSelect *select_statement_;
  quickstep::ParseStatementUpdate *update_statement_;
  quickstep::ParseStatementInsert *insert_statement_;
  quickstep::ParseStatementDelete *delete_statement_;
  quickstep::ParseStatementCopyFrom *copy_from_statement_;
  quickstep::ParseStatementCreateTable *create_table_statement_;
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

  quickstep::PtrList<quickstep::ParseOrderByItem> *order_commalist_;
  quickstep::ParseOrderByItem *order_item_;

  quickstep::PtrVector<quickstep::ParseSubqueryTableReference> *with_list_;
  quickstep::ParseSubqueryTableReference *with_list_element_;

#line 349 "SqlParser_gen.cpp" /* yacc.c:355  */
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
#line 175 "../SqlParser.ypp" /* yacc.c:358  */

/* This header needs YYSTYPE, which is defined by the %union directive above */
#include "SqlLexer_gen.hpp"
void NotSupported(const YYLTYPE *location, yyscan_t yyscanner, const std::string &feature);

#line 384 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYFINAL  39
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   872

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  105
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  78
/* YYNRULES -- Number of rules.  */
#define YYNRULES  204
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  399

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   350

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     102,   103,    21,    19,   104,    20,    25,    22,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   101,
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
      15,    16,    17,    18,    23,    24,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   468,   468,   472,   476,   479,   486,   489,   492,   495,
     498,   501,   504,   507,   510,   513,   519,   525,   532,   538,
     545,   554,   559,   568,   573,   578,   582,   588,   593,   596,
     599,   604,   607,   610,   613,   616,   619,   622,   625,   628,
     631,   643,   646,   649,   667,   687,   690,   693,   698,   703,
     709,   715,   724,   728,   734,   737,   742,   747,   752,   759,
     766,   770,   776,   779,   783,   785,   792,   797,   799,   805,
     809,   815,   821,   830,   838,   843,   848,   851,   856,   860,
     864,   868,   874,   879,   884,   888,   894,   900,   905,   908,
     913,   917,   923,   929,   935,   938,   942,   948,   951,   956,
     960,   966,   969,   972,   977,   982,   985,   991,   995,  1001,
    1007,  1013,  1019,  1025,  1031,  1037,  1043,  1051,  1056,  1060,
    1064,  1069,  1072,  1077,  1080,  1085,  1089,  1095,  1098,  1103,
    1106,  1111,  1114,  1119,  1122,  1141,  1145,  1151,  1158,  1161,
    1164,  1169,  1172,  1175,  1181,  1184,  1189,  1194,  1203,  1208,
    1217,  1222,  1225,  1230,  1233,  1238,  1244,  1250,  1257,  1260,
    1267,  1270,  1275,  1278,  1283,  1286,  1291,  1294,  1297,  1300,
    1305,  1309,  1313,  1318,  1322,  1328,  1331,  1334,  1337,  1349,
    1353,  1372,  1387,  1391,  1397,  1400,  1406,  1409,  1412,  1415,
    1418,  1421,  1426,  1437,  1440,  1445,  1448,  1454,  1458,  1464,
    1467,  1475,  1478,  1481,  1484
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOKEN_NAME",
  "TOKEN_STRING_SINGLE_QUOTED", "TOKEN_STRING_DOUBLE_QUOTED",
  "TOKEN_UNSIGNED_NUMVAL", "TOKEN_OR", "TOKEN_AND", "TOKEN_NOT",
  "TOKEN_EQ", "TOKEN_LT", "TOKEN_LEQ", "TOKEN_GT", "TOKEN_GEQ",
  "TOKEN_NEQ", "TOKEN_LIKE", "TOKEN_BETWEEN", "TOKEN_IS", "'+'", "'-'",
  "'*'", "'/'", "UNARY_PLUS", "UNARY_MINUS", "'.'", "TOKEN_ADD",
  "TOKEN_ALL", "TOKEN_ALTER", "TOKEN_AS", "TOKEN_ASC", "TOKEN_BIGINT",
  "TOKEN_BIT", "TOKEN_BLOOM_FILTER_INDEX", "TOKEN_BY", "TOKEN_CHARACTER",
  "TOKEN_CHECK", "TOKEN_COLUMN", "TOKEN_CONSTRAINT", "TOKEN_COPY",
  "TOKEN_CREATE", "TOKEN_DATE", "TOKEN_DATETIME", "TOKEN_DECIMAL",
  "TOKEN_DEFAULT", "TOKEN_DELETE", "TOKEN_DELIMITER", "TOKEN_DESC",
  "TOKEN_DISTINCT", "TOKEN_DOUBLE", "TOKEN_DROP", "TOKEN_ESCAPE_STRINGS",
  "TOKEN_FALSE", "TOKEN_FIRST", "TOKEN_FLOAT", "TOKEN_FOREIGN",
  "TOKEN_FROM", "TOKEN_FULL", "TOKEN_GROUP", "TOKEN_HAVING", "TOKEN_INDEX",
  "TOKEN_INNER", "TOKEN_INSERT", "TOKEN_INTEGER", "TOKEN_INTERVAL",
  "TOKEN_INTO", "TOKEN_JOIN", "TOKEN_KEY", "TOKEN_LAST", "TOKEN_LEFT",
  "TOKEN_LIMIT", "TOKEN_LONG", "TOKEN_NULL", "TOKEN_NULLS", "TOKEN_OFF",
  "TOKEN_ON", "TOKEN_ORDER", "TOKEN_OUTER", "TOKEN_PRIMARY", "TOKEN_QUIT",
  "TOKEN_REAL", "TOKEN_REFERENCES", "TOKEN_RIGHT", "TOKEN_ROW_DELIMITER",
  "TOKEN_SELECT", "TOKEN_SET", "TOKEN_SMALLINT", "TOKEN_TABLE",
  "TOKEN_TIME", "TOKEN_TIMESTAMP", "TOKEN_TRUE", "TOKEN_UNIQUE",
  "TOKEN_UPDATE", "TOKEN_USING", "TOKEN_VALUES", "TOKEN_VARCHAR",
  "TOKEN_WHERE", "TOKEN_WITH", "TOKEN_YEARMONTH", "TOKEN_EOF",
  "TOKEN_LEX_ERROR", "';'", "'('", "')'", "','", "$accept", "start",
  "sql_statement", "quit_statement", "alter_table_statement",
  "create_table_statement", "create_index_statement",
  "drop_table_statement", "column_def", "column_def_commalist",
  "data_type", "column_constraint_def", "column_constraint_def_list",
  "opt_column_constraint_def_list", "table_constraint_def",
  "table_constraint_def_commalist", "opt_table_constraint_def_commalist",
  "opt_column_list", "index_type", "opt_index_properties",
  "index_properties_list", "index_property", "insert_statement",
  "copy_from_statement", "opt_copy_from_params", "copy_from_params",
  "update_statement", "delete_statement", "assignment_list",
  "assignment_item", "select_statement", "opt_with_clause", "with_list",
  "with_list_element", "select_query", "opt_all_distinct", "selection",
  "selection_item_commalist", "selection_item", "from_clause",
  "opt_join_chain", "join_chain", "join", "subquery_expression",
  "table_reference", "table_reference_signature",
  "table_reference_signature_primary", "table_reference_commalist",
  "opt_group_by_clause", "opt_having_clause", "opt_order_by_clause",
  "opt_limit_clause", "order_commalist", "order_item",
  "opt_order_direction", "opt_nulls_first", "opt_where_clause",
  "where_clause", "or_expression", "and_expression", "not_expression",
  "predicate_expression_base", "add_expression", "multiply_expression",
  "unary_expression", "expression_base", "function_call",
  "expression_list", "literal_value", "literal_value_commalist",
  "attribute_ref", "comparison_operation", "unary_operation",
  "add_operation", "multiply_operation", "name_commalist", "any_name",
  "boolean_value", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,    43,
      45,    42,    47,   274,   275,    46,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,    59,    40,    41,    44
};
# endif

#define YYPACT_NINF -284

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-284)))

#define YYTABLE_NINF -89

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      35,  -284,   -19,    47,    16,    31,    50,    28,  -284,    47,
      47,  -284,    96,    24,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,    55,    47,  -284,  -284,    66,    47,
      47,    47,    47,    47,    56,    60,  -284,   115,    57,  -284,
    -284,  -284,    -2,  -284,    27,   187,   103,    74,    97,  -284,
      94,    47,    47,    95,    47,  -284,  -284,   420,   124,   137,
     110,    47,    47,   462,  -284,  -284,   111,    47,    99,  -284,
     206,  -284,    55,  -284,   -69,  -284,  -284,  -284,   222,   234,
    -284,  -284,  -284,   141,  -284,   185,  -284,  -284,  -284,  -284,
     250,  -284,  -284,  -284,  -284,  -284,  -284,   154,   199,   524,
     261,   213,   163,  -284,   195,   184,  -284,  -284,  -284,  -284,
    -284,   566,   -10,    47,   -13,    47,    47,   189,  -284,   197,
    -284,   107,   774,   628,   462,   290,   293,  -284,  -284,   273,
     285,   732,   116,    47,  -284,   524,   207,  -284,    47,  -284,
    -284,   303,  -284,  -284,   305,  -284,    -9,  -284,     0,    97,
     524,  -284,  -284,    47,   524,  -284,  -284,  -284,   524,   234,
    -284,    47,   347,  -284,   210,   246,   247,   215,  -284,  -284,
    -284,    38,    47,   226,   -13,    47,  -284,    82,  -284,     1,
     170,   462,   462,   304,  -284,  -284,  -284,  -284,  -284,  -284,
     316,   524,   524,     9,  -284,   119,   229,  -284,   211,  -284,
    -284,   223,   225,  -284,    40,  -284,    67,    40,   267,  -284,
    -284,   184,  -284,  -284,   227,  -284,   211,   142,   462,   230,
     232,    47,   325,   -35,   144,   155,   302,  -284,   233,  -284,
    -284,   271,   237,   732,  -284,   277,    47,  -284,  -284,    82,
    -284,  -284,   293,  -284,   524,  -284,    80,   211,   274,  -284,
    -284,   732,   243,  -284,  -284,    47,  -284,  -284,    76,   281,
      47,    90,    91,     0,  -284,   160,  -284,  -284,   314,   296,
    -284,  -284,   524,     6,    47,    47,   158,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,    92,  -284,  -284,   254,   -13,  -284,
     462,  -284,  -284,   256,  -284,   132,   524,  -284,  -284,   732,
    -284,    47,   283,    47,   -44,    47,   295,    47,   297,  -284,
    -284,   524,   462,   288,   211,  -284,   172,   174,  -284,   361,
     -35,    47,  -284,  -284,    12,    47,   524,   211,   176,   -43,
      47,   -42,   462,   -38,    47,   -37,    47,   265,   290,   336,
     301,   291,  -284,  -284,  -284,   191,  -284,   670,  -284,   270,
     211,  -284,   462,   -34,   462,   290,   462,   -31,   462,   -18,
     524,   368,  -284,    47,  -284,    47,   732,  -284,  -284,   290,
     462,   290,   290,   462,   290,   462,   272,  -284,   101,  -284,
     275,  -284,   202,   290,   290,   290,   524,  -284,  -284,   307,
      47,  -284,  -284,     3,  -284,   204,  -284,  -284,  -284
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     4,     0,     0,     0,     0,     0,     0,    16,     0,
       0,     5,     0,     0,    13,     6,     8,     9,    11,    12,
       7,    15,    10,    14,     0,     0,   199,   200,     0,     0,
       0,     0,     0,     0,     0,    89,    90,     0,   123,     1,
       3,     2,    94,    87,     0,     0,     0,     0,   144,    23,
       0,     0,     0,     0,     0,    95,    96,     0,     0,     0,
      76,     0,     0,     0,    83,   145,     0,     0,   144,    85,
       0,    91,     0,    92,     0,   197,   179,   176,     0,   192,
      97,    38,    27,     0,    28,    29,    32,    34,    35,    37,
       0,    39,   175,    33,    36,    30,    31,     0,     0,     0,
       0,     0,    98,    99,   103,   161,   163,   165,   168,   167,
     166,     0,   184,     0,     0,     0,     0,     0,    75,    64,
      25,     0,     0,     0,     0,   146,   148,   150,   152,     0,
     166,     0,     0,     0,    82,     0,     0,   124,     0,   177,
     178,     0,    41,   180,     0,    42,     0,   181,     0,   144,
       0,   193,   194,     0,     0,   102,   195,   196,     0,     0,
     164,     0,     0,    17,     0,     0,     0,     0,    18,    19,
      20,     0,     0,     0,    62,     0,    40,    54,   151,     0,
       0,     0,     0,     0,   186,   188,   189,   190,   191,   187,
       0,     0,     0,     0,   182,     0,     0,    84,    86,   117,
     198,     0,     0,   169,     0,   125,   105,   120,   127,   100,
     101,   160,   162,   185,     0,   170,   173,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,    63,    21,
      26,     0,     0,     0,    45,     0,     0,    47,    53,    55,
      24,   159,   147,   149,     0,   157,     0,   158,     0,   156,
      74,     0,     0,    43,    44,     0,   118,   121,     0,     0,
       0,     0,     0,     0,   104,   106,   108,   119,     0,   129,
     171,   172,     0,     0,     0,     0,     0,    78,   203,   204,
     202,   201,    79,    77,     0,    65,    66,    67,     0,    46,
       0,    49,    48,     0,    52,     0,     0,   155,   183,     0,
     122,     0,     0,     0,     0,     0,     0,     0,     0,   126,
     107,     0,     0,   131,   174,    59,     0,     0,    56,     0,
       0,     0,    22,    60,     0,     0,     0,   153,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   128,   130,     0,
     133,     0,    57,    80,    81,     0,    69,     0,    50,     0,
     154,    73,     0,     0,     0,   110,     0,     0,     0,     0,
       0,     0,    93,     0,    68,     0,     0,    71,    51,   116,
       0,   109,   112,     0,   114,     0,   132,   135,   138,   134,
       0,    70,     0,   115,   111,   113,     0,   139,   140,   141,
       0,    72,   136,     0,   137,     0,   142,   143,    58
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,   -93,  -284,
     253,   145,  -284,  -284,  -172,  -284,  -284,  -284,  -284,  -284,
    -284,    18,  -284,  -284,  -284,  -284,  -284,  -284,  -284,   248,
    -284,  -284,  -284,   333,   319,  -284,  -284,  -284,   236,  -284,
    -284,  -284,   127,   340,   131,   188,  -183,  -284,  -284,  -284,
    -284,  -284,  -284,    11,  -284,  -284,   -54,  -284,  -120,   217,
     218,   276,   -45,   249,   244,   294,  -284,    93,  -122,  -283,
     -16,  -284,  -284,  -284,  -284,   -66,    -3,    86
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    12,    13,    14,    15,    16,    17,    18,   120,   121,
     100,   238,   239,   240,   168,   228,   229,   173,   287,   322,
     345,   346,    19,    20,   118,   224,    21,    22,    68,    69,
      23,    24,    35,    36,    43,    57,   101,   102,   103,   149,
     264,   265,   266,   204,   205,   256,    37,   206,   269,   313,
     340,   362,   376,   377,   389,   394,    64,    65,   125,   126,
     127,   128,   129,   105,   106,   107,   108,   217,   109,   195,
     110,   192,   111,   154,   158,    74,   112,   282
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      28,   132,   227,    26,   179,    27,    34,    38,   181,   194,
     151,   152,   104,   181,   134,   161,   328,   278,   248,   181,
     163,   257,    44,   164,   257,    55,    46,    47,    48,    49,
      50,   332,   352,   354,   137,   138,     1,   356,   358,   279,
     280,   370,   165,    26,   373,    27,    56,   130,    70,    38,
      26,    75,    27,    58,   146,   281,   396,   375,   119,   122,
     138,   138,   138,     2,    75,   166,   138,   138,    25,   255,
     138,   397,   300,   138,     3,     4,    29,    59,   167,   180,
       5,   249,   230,   382,   222,     6,   138,    31,   296,   223,
     198,   231,   162,    33,   203,   208,    39,     7,   273,   151,
     152,   155,    72,    30,   241,   104,   225,   130,   130,   315,
     122,   291,   169,   170,     8,   348,   323,   216,   232,   -88,
     151,   152,    45,    40,   258,    41,   233,     9,   259,   298,
      70,   387,    10,   260,    11,   200,   261,    32,   319,    42,
     326,    51,   301,   320,    53,   207,   246,   247,   388,   262,
     210,   151,   152,   302,   234,   276,   305,   307,   213,    54,
     235,   113,   114,   236,    52,   130,   130,   306,   308,    75,
     324,   263,   122,   237,   115,   116,    62,   194,    61,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    66,   151,
     152,    60,   338,    63,   304,    63,    67,    72,    26,   295,
      27,    38,   130,   133,    38,   156,   157,   117,   316,   317,
     174,   175,   355,   131,   151,   152,   135,   258,    75,   196,
     138,   259,   250,   251,   153,   367,   260,   314,   139,   261,
     151,   152,   369,   293,   371,   329,   372,   331,   374,   333,
     140,   335,   262,   141,   194,   271,   272,   283,   284,   142,
     383,   327,    38,   384,   143,   385,   144,    75,   285,   138,
     207,   318,   138,   145,   353,   147,   216,   150,   357,   148,
     359,    75,    75,   203,   130,   341,   138,   342,   138,   351,
     251,   350,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   171,   151,   152,   364,   365,   130,   181,    75,   172,
      75,   182,    75,   193,    75,   391,   251,   398,   138,   201,
     199,   202,   218,   219,   220,   378,   130,   221,   347,   226,
     245,   244,   349,   252,   395,   268,   253,    75,   254,   277,
     270,    75,   274,    75,   275,   286,   130,   288,   130,   290,
     130,   378,   130,   289,   292,   299,   297,   303,   311,   330,
      26,    76,    27,    77,   130,   312,   321,   130,   325,   130,
     380,   334,   347,   336,   339,   343,    78,    79,   214,   272,
     360,   361,   363,   368,   379,   177,   386,   390,    81,    82,
     393,   197,    83,   381,   294,    71,   209,    75,    84,    85,
      86,   136,   310,    73,   309,   267,    87,   392,   242,   178,
     243,    88,   212,   211,   337,   160,   344,     0,     0,     0,
      89,    90,     0,     0,     0,     0,     0,     0,    91,    92,
       0,     0,     0,    26,    76,    27,    77,    93,     0,     0,
       0,     0,     0,    94,     0,    95,    96,     0,     0,    78,
      79,    80,    97,     0,     0,    98,     0,     0,     0,    99,
     215,    81,    82,     0,     0,    83,     0,     0,     0,     0,
       0,    84,    85,    86,     0,    26,    76,    27,    77,    87,
       0,   123,     0,     0,    88,     0,     0,     0,     0,     0,
       0,    78,    79,    89,    90,     0,     0,     0,     0,     0,
       0,    91,    92,    81,    82,     0,     0,    83,     0,     0,
      93,     0,     0,    84,    85,    86,    94,     0,    95,    96,
       0,    87,     0,     0,     0,    97,    88,     0,    98,     0,
       0,     0,    99,     0,     0,    89,    90,    26,    76,    27,
      77,     0,     0,    91,    92,     0,     0,     0,     0,     0,
       0,     0,    93,    78,    79,     0,     0,     0,    94,     0,
      95,    96,     0,     0,     0,    81,    82,    97,     0,    83,
      98,     0,     0,     0,   124,    84,    85,    86,     0,    26,
      76,    27,    77,    87,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,    78,   159,    89,    90,     0,
       0,     0,     0,     0,     0,    91,    92,    81,    82,     0,
       0,    83,     0,     0,    93,     0,     0,    84,    85,    86,
      94,     0,    95,    96,     0,    87,     0,     0,     0,    97,
      88,     0,    98,     0,     0,     0,    99,     0,     0,    89,
      90,    26,    76,    27,    77,     0,     0,    91,    92,     0,
       0,     0,     0,     0,     0,     0,    93,    78,    79,     0,
       0,     0,    94,     0,    95,    96,     0,     0,     0,    81,
      82,    97,     0,    83,    98,     0,     0,     0,    99,    84,
      85,    86,     0,     0,    76,     0,    77,    87,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,    78,
     159,    89,    90,     0,     0,     0,     0,     0,     0,    91,
      92,    81,    82,     0,     0,    83,     0,     0,    93,     0,
       0,    84,    85,    86,    94,     0,    95,    96,     0,    87,
       0,     0,     0,    97,    88,     0,    98,     0,     0,     0,
     124,     0,     0,    89,    90,     0,    76,     0,    77,     0,
       0,    91,    92,     0,     0,     0,     0,     0,     0,     0,
      93,    78,   159,     0,     0,     0,    94,     0,    95,    96,
       0,     0,     0,    81,    82,    97,     0,    83,    98,     0,
       0,     0,   366,    84,    85,    86,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,     0,     0,     0,
       0,     0,     0,    91,    92,    81,    82,     0,     0,    83,
       0,     0,    93,     0,     0,    84,    85,    86,    94,     0,
      95,    96,     0,    87,     0,     0,     0,    97,    88,     0,
      98,     0,     0,     0,     0,     0,     0,    89,   176,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
      94,     0,    95,    96,     0,     0,     0,     0,     0,    97,
       0,     0,    98
};

static const yytype_int16 yycheck[] =
{
       3,    67,   174,     3,   124,     5,     9,    10,     7,   131,
      19,    20,    57,     7,    68,    25,   299,    52,     9,     7,
     113,   204,    25,    36,   207,    27,    29,    30,    31,    32,
      33,    75,    75,    75,   103,   104,     1,    75,    75,    74,
      75,    75,    55,     3,    75,     5,    48,    63,    51,    52,
       3,    54,     5,    26,    99,    90,    53,    75,    61,    62,
     104,   104,   104,    28,    67,    78,   104,   104,    87,    29,
     104,    68,   255,   104,    39,    40,    60,    50,    91,   124,
      45,    72,   175,   366,    46,    50,   104,    56,     8,    51,
     135,     9,   102,    65,   103,   149,     0,    62,   218,    19,
      20,   104,   102,    87,   103,   150,   172,   123,   124,   103,
     113,   233,   115,   116,    79,   103,   288,   162,    36,    84,
      19,    20,    56,    99,    57,   101,    44,    92,    61,   251,
     133,    30,    97,    66,    99,   138,    69,    87,    46,    84,
       8,    85,    66,    51,    29,   148,   191,   192,    47,    82,
     153,    19,    20,    77,    72,   221,    66,    66,   161,   102,
      78,    37,    38,    81,   104,   181,   182,    77,    77,   172,
     290,   104,   175,    91,    37,    38,   102,   299,    75,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    94,    19,
      20,     4,   312,    96,   260,    96,   102,   102,     3,   244,
       5,   204,   218,   104,   207,    21,    22,    97,   274,   275,
     103,   104,   332,   102,    19,    20,    10,    57,   221,   103,
     104,    61,   103,   104,    29,   347,    66,   272,     6,    69,
      19,    20,   352,   236,   354,   301,   356,   303,   358,   305,
       6,   307,    82,   102,   366,   103,   104,   103,   104,    64,
     370,   296,   255,   373,     4,   375,   102,   260,   103,   104,
     263,   103,   104,    64,   330,     4,   311,   104,   334,    56,
     336,   274,   275,   103,   290,   103,   104,   103,   104,   103,
     104,   326,     9,    10,    11,    12,    13,    14,    15,    16,
      17,   102,    19,    20,   103,   104,   312,     7,   301,   102,
     303,     8,   305,    18,   307,   103,   104,   103,   104,     6,
     103,     6,   102,    67,    67,   360,   332,   102,   321,    93,
       4,    17,   325,    94,   390,    58,   103,   330,   103,     4,
     103,   334,   102,   336,   102,    33,   352,   104,   354,   102,
     356,   386,   358,    72,    67,   102,    72,    66,    34,    66,
       3,     4,     5,     6,   370,    59,   102,   373,   102,   375,
     363,    66,   365,    66,    76,     4,    19,    20,    21,   104,
      34,    70,    81,   103,     6,   122,   104,   102,    31,    32,
      73,   133,    35,   365,   239,    52,   150,   390,    41,    42,
      43,    72,   265,    53,   263,   207,    49,   386,   181,   123,
     182,    54,   158,   154,   311,   111,   320,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      -1,    -1,    -1,     3,     4,     5,     6,    80,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    88,    89,    -1,    -1,    19,
      20,    21,    95,    -1,    -1,    98,    -1,    -1,    -1,   102,
     103,    31,    32,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      -1,    41,    42,    43,    -1,     3,     4,     5,     6,    49,
      -1,     9,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    20,    63,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    31,    32,    -1,    -1,    35,    -1,    -1,
      80,    -1,    -1,    41,    42,    43,    86,    -1,    88,    89,
      -1,    49,    -1,    -1,    -1,    95,    54,    -1,    98,    -1,
      -1,    -1,   102,    -1,    -1,    63,    64,     3,     4,     5,
       6,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    19,    20,    -1,    -1,    -1,    86,    -1,
      88,    89,    -1,    -1,    -1,    31,    32,    95,    -1,    35,
      98,    -1,    -1,    -1,   102,    41,    42,    43,    -1,     3,
       4,     5,     6,    49,    -1,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    31,    32,    -1,
      -1,    35,    -1,    -1,    80,    -1,    -1,    41,    42,    43,
      86,    -1,    88,    89,    -1,    49,    -1,    -1,    -1,    95,
      54,    -1,    98,    -1,    -1,    -1,   102,    -1,    -1,    63,
      64,     3,     4,     5,     6,    -1,    -1,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    19,    20,    -1,
      -1,    -1,    86,    -1,    88,    89,    -1,    -1,    -1,    31,
      32,    95,    -1,    35,    98,    -1,    -1,    -1,   102,    41,
      42,    43,    -1,    -1,     4,    -1,     6,    49,    -1,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      20,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    31,    32,    -1,    -1,    35,    -1,    -1,    80,    -1,
      -1,    41,    42,    43,    86,    -1,    88,    89,    -1,    49,
      -1,    -1,    -1,    95,    54,    -1,    98,    -1,    -1,    -1,
     102,    -1,    -1,    63,    64,    -1,     4,    -1,     6,    -1,
      -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    19,    20,    -1,    -1,    -1,    86,    -1,    88,    89,
      -1,    -1,    -1,    31,    32,    95,    -1,    35,    98,    -1,
      -1,    -1,   102,    41,    42,    43,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    31,    32,    -1,    -1,    35,
      -1,    -1,    80,    -1,    -1,    41,    42,    43,    86,    -1,
      88,    89,    -1,    49,    -1,    -1,    -1,    95,    54,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    88,    89,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    -1,    98
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,    28,    39,    40,    45,    50,    62,    79,    92,
      97,    99,   106,   107,   108,   109,   110,   111,   112,   127,
     128,   131,   132,   135,   136,    87,     3,     5,   181,    60,
      87,    56,    87,    65,   181,   137,   138,   151,   181,     0,
      99,   101,    84,   139,   181,    56,   181,   181,   181,   181,
     181,    85,   104,    29,   102,    27,    48,   140,    26,    50,
       4,    75,   102,    96,   161,   162,    94,   102,   133,   134,
     181,   138,   102,   148,   180,   181,     4,     6,    19,    20,
      21,    31,    32,    35,    41,    42,    43,    49,    54,    63,
      64,    71,    72,    80,    86,    88,    89,    95,    98,   102,
     115,   141,   142,   143,   167,   168,   169,   170,   171,   173,
     175,   177,   181,    37,    38,    37,    38,    97,   129,   181,
     113,   114,   181,     9,   102,   163,   164,   165,   166,   167,
     175,   102,   180,   104,   161,    10,   139,   103,   104,     6,
       6,   102,    64,     4,   102,    64,   167,     4,    56,   144,
     104,    19,    20,    29,   178,   181,    21,    22,   179,    20,
     170,    25,   102,   113,    36,    55,    78,    91,   119,   181,
     181,   102,   102,   122,   103,   104,    64,   115,   166,   163,
     167,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,   176,    18,   173,   174,   103,   134,   167,   103,
     181,     6,     6,   103,   148,   149,   152,   181,   161,   143,
     181,   168,   169,   181,    21,   103,   167,   172,   102,    67,
      67,   102,    46,    51,   130,   180,    93,   119,   120,   121,
     113,     9,    36,    44,    72,    78,    81,    91,   116,   117,
     118,   103,   164,   165,    17,     4,   167,   167,     9,    72,
     103,   104,    94,   103,   103,    29,   150,   151,    57,    61,
      66,    69,    82,   104,   145,   146,   147,   150,    58,   153,
     103,   103,   104,   163,   102,   102,   180,     4,    52,    74,
      75,    90,   182,   103,   104,   103,    33,   123,   104,    72,
     102,   173,    67,   181,   116,   167,     8,    72,   173,   102,
     151,    66,    77,    66,   180,    66,    77,    66,    77,   149,
     147,    34,    59,   154,   167,   103,   180,   180,   103,    46,
      51,   102,   124,   119,   163,   102,     8,   167,   174,   180,
      66,   180,    75,   180,    66,   180,    66,   172,   163,    76,
     155,   103,   103,     4,   182,   125,   126,   181,   103,   181,
     167,   103,    75,   180,    75,   163,    75,   180,    75,   180,
      34,    70,   156,    81,   103,   104,   102,   173,   103,   163,
      75,   163,   163,    75,   163,    75,   157,   158,   167,     6,
     181,   126,   174,   163,   163,   163,   104,    30,    47,   159,
     102,   103,   158,    73,   160,   180,    53,    68,   103
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   105,   106,   106,   106,   106,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   108,   109,   109,   109,
     109,   110,   111,   112,   113,   114,   114,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   116,   116,   116,   116,   116,
     116,   116,   117,   117,   118,   118,   119,   119,   119,   119,
     120,   120,   121,   121,   122,   122,   123,   124,   124,   125,
     125,   126,   126,   127,   127,   128,   129,   129,   130,   130,
     130,   130,   131,   132,   133,   133,   134,   135,   136,   136,
     137,   137,   138,   139,   140,   140,   140,   141,   141,   142,
     142,   143,   143,   143,   144,   145,   145,   146,   146,   147,
     147,   147,   147,   147,   147,   147,   147,   148,   149,   149,
     149,   150,   150,   151,   151,   152,   152,   153,   153,   154,
     154,   155,   155,   156,   156,   157,   157,   158,   159,   159,
     159,   160,   160,   160,   161,   161,   162,   163,   163,   164,
     164,   165,   165,   166,   166,   166,   166,   166,   166,   166,
     167,   167,   168,   168,   169,   169,   170,   170,   170,   170,
     171,   171,   171,   172,   172,   173,   173,   173,   173,   173,
     173,   173,   174,   174,   175,   175,   176,   176,   176,   176,
     176,   176,   177,   178,   178,   179,   179,   180,   180,   181,
     181,   182,   182,   182,   182
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     6,     6,     6,
       6,     7,     9,     3,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     4,     4,     1,     2,     1,     2,     2,
       4,     5,     2,     1,     0,     1,     4,     5,    10,     4,
       3,     1,     0,     1,     0,     3,     1,     0,     3,     1,
       3,     2,     4,    10,     7,     5,     0,     4,     2,     2,
       4,     4,     5,     4,     3,     1,     3,     2,     0,     2,
       1,     3,     3,     9,     0,     1,     1,     1,     1,     1,
       3,     3,     2,     1,     3,     0,     1,     2,     1,     5,
       4,     6,     5,     6,     5,     6,     5,     3,     2,     2,
       1,     1,     2,     1,     4,     1,     3,     0,     3,     0,
       2,     0,     3,     0,     2,     1,     3,     3,     0,     1,
       1,     0,     2,     2,     0,     1,     2,     3,     1,     3,
       1,     2,     1,     5,     6,     4,     3,     3,     3,     3,
       3,     1,     3,     1,     2,     1,     1,     1,     1,     3,
       3,     4,     4,     1,     3,     1,     1,     2,     2,     1,
       2,     2,     1,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       1,     1,     1,     1,     1
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
          case 3: /* TOKEN_NAME  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1708 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1718 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1728 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_UNSIGNED_NUMVAL  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 1738 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 107: /* sql_statement  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1748 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 108: /* quit_statement  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 1758 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 109: /* alter_table_statement  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1768 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 110: /* create_table_statement  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 1778 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 111: /* create_index_statement  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1788 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 112: /* drop_table_statement  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 1798 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 113: /* column_def  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 1808 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 114: /* column_def_commalist  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 1818 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 115: /* data_type  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 1828 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 116: /* column_constraint_def  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 1838 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 117: /* column_constraint_def_list  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1848 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 118: /* opt_column_constraint_def_list  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1858 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 127: /* insert_statement  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 1868 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 128: /* copy_from_statement  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_statement_) != nullptr) {
    delete ((*yyvaluep).copy_from_statement_);
  }
}
#line 1878 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 129: /* opt_copy_from_params  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 1888 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 130: /* copy_from_params  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 1898 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 131: /* update_statement  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 1908 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 132: /* delete_statement  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 1918 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 133: /* assignment_list  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 1928 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 134: /* assignment_item  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 1938 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 135: /* select_statement  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_statement_) != nullptr) {
    delete ((*yyvaluep).select_statement_);
  }
}
#line 1948 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 136: /* opt_with_clause  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 1958 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 137: /* with_list  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 1968 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 138: /* with_list_element  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 1978 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 139: /* select_query  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 1988 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 141: /* selection  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 1998 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 142: /* selection_item_commalist  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2008 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 143: /* selection_item  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2018 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 144: /* from_clause  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2028 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 148: /* subquery_expression  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2038 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 149: /* table_reference  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2048 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 150: /* table_reference_signature  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2058 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 151: /* table_reference_signature_primary  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2068 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 152: /* table_reference_commalist  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2078 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 153: /* opt_group_by_clause  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2088 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* opt_having_clause  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2098 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* opt_order_by_clause  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2108 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* opt_limit_clause  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2118 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* order_commalist  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2128 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* order_item  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2138 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* opt_order_direction  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2148 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* opt_nulls_first  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2158 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* opt_where_clause  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2168 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* where_clause  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2178 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 163: /* or_expression  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2188 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 164: /* and_expression  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2198 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 165: /* not_expression  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2208 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 166: /* predicate_expression_base  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2218 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 167: /* add_expression  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2228 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* multiply_expression  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2238 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* unary_expression  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2248 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* expression_base  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2258 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* function_call  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2268 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* expression_list  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2278 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* literal_value  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2288 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* literal_value_commalist  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2298 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* attribute_ref  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2308 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* comparison_operation  */
#line 455 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2314 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* unary_operation  */
#line 456 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2320 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* add_operation  */
#line 457 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2326 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* multiply_operation  */
#line 457 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2332 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* name_commalist  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 2342 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* any_name  */
#line 459 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2352 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* boolean_value  */
#line 454 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2358 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 468 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2655 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 472 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2664 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 476 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    YYABORT;
  }
#line 2672 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 5:
#line 479 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 2681 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 486 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 2689 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 489 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].copy_from_statement_);
  }
#line 2697 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 492 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 2705 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 9:
#line 495 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 2713 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 10:
#line 498 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 2721 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 11:
#line 501 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 2729 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 12:
#line 504 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 2737 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 13:
#line 507 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 2745 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 14:
#line 510 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].select_statement_);
  }
#line 2753 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 15:
#line 513 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 2761 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 16:
#line 519 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 2769 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 17:
#line 525 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2781 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 18:
#line 532 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2792 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 19:
#line 538 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2804 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 20:
#line 545 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2816 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 554 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-2].attribute_definition_list_));
  }
#line 2824 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 559 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-6].string_value_);
    delete (yyvsp[-4].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-8]), yyscanner, "CREATE INDEX statements");
    YYERROR;
  }
#line 2836 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 568 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 2844 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 573 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 2852 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 578 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 2861 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 582 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 2870 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 588 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 2880 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 593 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 2888 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 596 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 2896 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 599 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 2906 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 604 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 2914 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 607 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 2922 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 610 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 2930 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 613 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 2938 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 616 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 2946 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 619 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 2954 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 622 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 2962 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 625 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 2970 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 628 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 2978 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 631 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 2995 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 643 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3003 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 646 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3011 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 649 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3034 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 667 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3057 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 687 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3065 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 690 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3073 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 693 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3083 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 698 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3093 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 703 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3104 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 709 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3115 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 715 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3127 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 724 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3136 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 728 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3145 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 734 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3153 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 737 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3161 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 742 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3171 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 747 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3181 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 752 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3193 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 759 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3203 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 766 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3212 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 770 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3221 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 776 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3229 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 779 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = $1; */
  }
#line 3237 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 785 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-2]), yyscanner, "list of column names in CREATE INDEX statement");
    YYERROR;
  }
#line 3247 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 792 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Bloom Filter Index");
    YYERROR;
  }
#line 3256 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 799 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "list of index properties in CREATE INDEX statement");
    YYERROR;
  }
#line 3265 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 805 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "index property in CREATE INDEX statement");
    YYERROR;
  }
#line 3274 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 809 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "list of index properties in CREATE INDEX statement");
    YYERROR;
  }
#line 3283 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 815 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_value_);
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "index property in CREATE INDEX statement");
    YYERROR;
  }
#line 3294 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 821 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].literal_value_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "index property list in CREATE INDEX statement");
    YYERROR;
  }
#line 3305 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 830 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 3318 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 838 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsert((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 3326 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 843 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_statement_) = new quickstep::ParseStatementCopyFrom((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].copy_from_params_));
  }
#line 3334 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 848 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = nullptr;
  }
#line 3342 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 851 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-1].copy_from_params_);
  }
#line 3350 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 856 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3359 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 860 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3368 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 864 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3377 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 868 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3386 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 874 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 3394 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 879 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 3402 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 884 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3411 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 888 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3420 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 894 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 3428 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 900 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].select_query_), (yyvsp[-1].with_list_));
  }
#line 3436 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 905 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = nullptr;
  }
#line 3444 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 908 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 3452 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 913 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3461 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 917 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3470 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 923 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 3479 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 930 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].selection_), (yyvsp[-5].table_reference_list_), (yyvsp[-4].predicate_), (yyvsp[-3].opt_group_by_clause_), (yyvsp[-2].opt_having_clause_), (yyvsp[-1].opt_order_by_clause_), (yyvsp[0].opt_limit_clause_));
  }
#line 3487 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 935 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3495 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 938 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "ALL in selection");
    YYERROR;
  }
#line 3504 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 942 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "DISTINCT in selection");
    YYERROR;
  }
#line 3513 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 948 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3521 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 951 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 3529 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 956 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3538 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 960 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3547 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 966 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 3555 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 969 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 3563 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 972 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 3571 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 977 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-1].table_reference_list_);
  }
#line 3579 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 982 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3587 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 985 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3596 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 991 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-1]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3605 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 995 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3614 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 1001 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3625 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 1007 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3636 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 1013 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3647 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 1019 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3658 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 1025 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3669 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 1031 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3680 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 1037 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3691 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 1043 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3702 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 1051 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].select_query_));
  }
#line 3710 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 1056 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 3719 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 1060 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 3728 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 1064 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 3736 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 1069 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 3744 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 1072 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 3752 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 1077 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 3760 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 1080 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 3768 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 1085 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 3777 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 1089 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 3786 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 1095 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 3794 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 1098 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 3802 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 1103 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 3810 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 1106 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 3818 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 1111 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 3826 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 1114 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 3834 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 1119 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 3842 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 1122 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3864 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 1141 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 3873 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 1145 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 3882 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 1151 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 3892 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 1158 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 3900 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 1161 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 3908 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 1164 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 3916 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 1169 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 3924 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 1172 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 3932 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 1175 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 3940 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 1181 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 3948 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 1184 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 3956 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 1189 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 3964 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 1194 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 3978 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 1203 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 3986 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 1208 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4000 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 1217 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4008 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 1222 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4016 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 1225 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4024 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 1230 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4032 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 1233 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 4042 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 1238 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4053 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 1244 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4064 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 1250 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].expression_);
    delete (yyvsp[0].string_value_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "LIKE predicates");
    YYERROR;
  }
#line 4076 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 1257 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4084 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 1260 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 4092 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 1267 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4100 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 1270 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4108 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 1275 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4116 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 1278 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4124 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 1283 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].unary_operation_), (yyvsp[0].expression_));
  }
#line 4132 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 1286 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4140 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 1291 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 4148 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 1294 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 4156 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 1297 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 4164 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 1300 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 4172 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 1305 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 4181 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 1309 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 4190 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 1313 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4198 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 1318 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4207 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 1322 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4216 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 1328 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4224 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 1331 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4232 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 1334 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4240 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 1337 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4257 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 1349 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 4266 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 1353 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4290 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 1372 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4308 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 1387 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4317 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 1391 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4326 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 1397 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4334 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 1400 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 4342 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1406 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 4350 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 1409 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 4358 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 1412 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 4366 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 1415 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 4374 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 1418 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 4382 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 1421 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 4390 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 1426 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
#line 4404 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 1437 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
#line 4412 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 1440 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
#line 4420 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 1445 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
#line 4428 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 1448 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
#line 4436 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 1454 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4445 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 1458 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4454 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 1464 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 4462 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1467 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 4473 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1475 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4481 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1478 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4489 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1481 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 4497 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1484 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 4505 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;


#line 4509 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 1488 "../SqlParser.ypp" /* yacc.c:1906  */


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
