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
#line 23 "../SqlParser.ypp" /* yacc.c:339  */


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

#line 52 "../SqlParser.ypp" /* yacc.c:339  */

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
    TOKEN_BY = 283,
    TOKEN_CHARACTER = 284,
    TOKEN_CHECK = 285,
    TOKEN_COLUMN = 286,
    TOKEN_CONSTRAINT = 287,
    TOKEN_COPY = 288,
    TOKEN_CREATE = 289,
    TOKEN_DATE = 290,
    TOKEN_DATETIME = 291,
    TOKEN_DECIMAL = 292,
    TOKEN_DEFAULT = 293,
    TOKEN_DELETE = 294,
    TOKEN_DELIMITER = 295,
    TOKEN_DESC = 296,
    TOKEN_DISTINCT = 297,
    TOKEN_DOUBLE = 298,
    TOKEN_DROP = 299,
    TOKEN_ESCAPE_STRINGS = 300,
    TOKEN_FALSE = 301,
    TOKEN_FIRST = 302,
    TOKEN_FLOAT = 303,
    TOKEN_FOREIGN = 304,
    TOKEN_FROM = 305,
    TOKEN_FULL = 306,
    TOKEN_GROUP = 307,
    TOKEN_HAVING = 308,
    TOKEN_INNER = 309,
    TOKEN_INSERT = 310,
    TOKEN_INTEGER = 311,
    TOKEN_INTERVAL = 312,
    TOKEN_INTO = 313,
    TOKEN_JOIN = 314,
    TOKEN_KEY = 315,
    TOKEN_LAST = 316,
    TOKEN_LEFT = 317,
    TOKEN_LIMIT = 318,
    TOKEN_LONG = 319,
    TOKEN_NULL = 320,
    TOKEN_NULLS = 321,
    TOKEN_OFF = 322,
    TOKEN_ON = 323,
    TOKEN_ORDER = 324,
    TOKEN_OUTER = 325,
    TOKEN_PRIMARY = 326,
    TOKEN_QUIT = 327,
    TOKEN_REAL = 328,
    TOKEN_REFERENCES = 329,
    TOKEN_RIGHT = 330,
    TOKEN_ROW_DELIMITER = 331,
    TOKEN_SELECT = 332,
    TOKEN_SET = 333,
    TOKEN_SMALLINT = 334,
    TOKEN_TABLE = 335,
    TOKEN_TIME = 336,
    TOKEN_TIMESTAMP = 337,
    TOKEN_TRUE = 338,
    TOKEN_UNIQUE = 339,
    TOKEN_UPDATE = 340,
    TOKEN_VALUES = 341,
    TOKEN_VARCHAR = 342,
    TOKEN_WHERE = 343,
    TOKEN_WITH = 344,
    TOKEN_YEARMONTH = 345,
    TOKEN_EOF = 346,
    TOKEN_LEX_ERROR = 347
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 95 "../SqlParser.ypp" /* yacc.c:355  */

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

#line 346 "SqlParser_gen.cpp" /* yacc.c:355  */
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
#line 165 "../SqlParser.ypp" /* yacc.c:358  */

/* This header needs YYSTYPE, which is defined by the %union directive above */
#include "SqlLexer_gen.hpp"
void NotSupported(const YYLTYPE *location, yyscan_t yyscanner, const std::string &feature);

#line 381 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYFINAL  37
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   760

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  102
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  72
/* YYNRULES -- Number of rules.  */
#define YYNRULES  193
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  375

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   347

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
      99,   100,    21,    19,   101,    20,    25,    22,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    98,
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
      90,    91,    92,    93,    94,    95,    96,    97
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   449,   449,   453,   457,   460,   467,   470,   473,   476,
     479,   482,   485,   488,   491,   497,   503,   510,   516,   523,
     532,   537,   542,   547,   551,   557,   562,   565,   568,   573,
     576,   579,   582,   585,   588,   591,   594,   597,   600,   612,
     615,   618,   636,   656,   659,   662,   667,   672,   678,   684,
     693,   697,   703,   706,   711,   716,   721,   728,   735,   739,
     745,   748,   754,   762,   767,   772,   775,   780,   784,   788,
     792,   798,   803,   808,   812,   818,   824,   829,   832,   837,
     841,   847,   853,   859,   862,   866,   872,   875,   880,   884,
     890,   893,   896,   901,   906,   909,   915,   919,   925,   931,
     937,   943,   949,   955,   961,   967,   975,   980,   984,   988,
     993,   996,  1001,  1004,  1009,  1013,  1019,  1022,  1027,  1030,
    1035,  1038,  1043,  1046,  1065,  1069,  1075,  1082,  1085,  1088,
    1093,  1096,  1099,  1105,  1108,  1113,  1118,  1127,  1132,  1141,
    1146,  1149,  1154,  1157,  1162,  1168,  1174,  1181,  1184,  1191,
    1194,  1199,  1202,  1207,  1210,  1215,  1218,  1221,  1224,  1229,
    1233,  1237,  1242,  1246,  1252,  1255,  1258,  1261,  1273,  1277,
    1296,  1311,  1315,  1321,  1324,  1330,  1333,  1336,  1339,  1342,
    1345,  1350,  1361,  1364,  1369,  1372,  1378,  1382,  1388,  1391,
    1399,  1402,  1405,  1408
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
  "TOKEN_BIT", "TOKEN_BY", "TOKEN_CHARACTER", "TOKEN_CHECK",
  "TOKEN_COLUMN", "TOKEN_CONSTRAINT", "TOKEN_COPY", "TOKEN_CREATE",
  "TOKEN_DATE", "TOKEN_DATETIME", "TOKEN_DECIMAL", "TOKEN_DEFAULT",
  "TOKEN_DELETE", "TOKEN_DELIMITER", "TOKEN_DESC", "TOKEN_DISTINCT",
  "TOKEN_DOUBLE", "TOKEN_DROP", "TOKEN_ESCAPE_STRINGS", "TOKEN_FALSE",
  "TOKEN_FIRST", "TOKEN_FLOAT", "TOKEN_FOREIGN", "TOKEN_FROM",
  "TOKEN_FULL", "TOKEN_GROUP", "TOKEN_HAVING", "TOKEN_INNER",
  "TOKEN_INSERT", "TOKEN_INTEGER", "TOKEN_INTERVAL", "TOKEN_INTO",
  "TOKEN_JOIN", "TOKEN_KEY", "TOKEN_LAST", "TOKEN_LEFT", "TOKEN_LIMIT",
  "TOKEN_LONG", "TOKEN_NULL", "TOKEN_NULLS", "TOKEN_OFF", "TOKEN_ON",
  "TOKEN_ORDER", "TOKEN_OUTER", "TOKEN_PRIMARY", "TOKEN_QUIT",
  "TOKEN_REAL", "TOKEN_REFERENCES", "TOKEN_RIGHT", "TOKEN_ROW_DELIMITER",
  "TOKEN_SELECT", "TOKEN_SET", "TOKEN_SMALLINT", "TOKEN_TABLE",
  "TOKEN_TIME", "TOKEN_TIMESTAMP", "TOKEN_TRUE", "TOKEN_UNIQUE",
  "TOKEN_UPDATE", "TOKEN_VALUES", "TOKEN_VARCHAR", "TOKEN_WHERE",
  "TOKEN_WITH", "TOKEN_YEARMONTH", "TOKEN_EOF", "TOKEN_LEX_ERROR", "';'",
  "'('", "')'", "','", "$accept", "start", "sql_statement",
  "quit_statement", "alter_table_statement", "create_table_statement",
  "drop_table_statement", "column_def", "column_def_commalist",
  "data_type", "column_constraint_def", "column_constraint_def_list",
  "opt_column_constraint_def_list", "table_constraint_def",
  "table_constraint_def_commalist", "opt_table_constraint_def_commalist",
  "insert_statement", "copy_from_statement", "opt_copy_from_params",
  "copy_from_params", "update_statement", "delete_statement",
  "assignment_list", "assignment_item", "select_statement",
  "opt_with_clause", "with_list", "with_list_element", "select_query",
  "opt_all_distinct", "selection", "selection_item_commalist",
  "selection_item", "from_clause", "opt_join_chain", "join_chain", "join",
  "subquery_expression", "table_reference", "table_reference_signature",
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
     340,   341,   342,   343,   344,   345,   346,   347,    59,    40,
      41,    44
};
# endif

#define YYPACT_NINF -166

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-166)))

#define YYTABLE_NINF -78

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      39,  -166,   -41,   152,   -32,     1,   -15,    29,  -166,   152,
     152,  -166,   105,    60,  -166,  -166,  -166,  -166,  -166,  -166,
    -166,  -166,  -166,    52,   152,  -166,  -166,    91,   152,   152,
     152,   152,    68,    58,  -166,   146,    81,  -166,  -166,  -166,
      48,  -166,    13,   183,    92,   119,  -166,   -48,   152,   152,
     106,   152,  -166,  -166,   389,   147,   162,   129,   152,   429,
    -166,  -166,   120,   152,   -35,  -166,   192,  -166,    52,  -166,
      95,  -166,  -166,  -166,   224,   226,  -166,  -166,  -166,   141,
    -166,   180,  -166,  -166,  -166,  -166,   242,  -166,  -166,  -166,
    -166,  -166,  -166,   148,   187,   491,   248,   200,   158,  -166,
     159,   179,  -166,  -166,  -166,  -166,  -166,   531,   -13,   152,
      14,   152,   152,   165,  -166,  -166,   107,   665,   593,   429,
     254,   257,  -166,  -166,   325,   249,   625,   109,   152,  -166,
     491,   166,  -166,   152,  -166,  -166,   264,  -166,  -166,   265,
    -166,    -4,  -166,     5,   119,   491,  -166,  -166,   152,   491,
    -166,  -166,  -166,   491,   226,  -166,   152,   327,  -166,   173,
     208,   209,   177,  -166,  -166,  -166,   -21,    14,   152,  -166,
      -5,  -166,    -2,   157,   429,   429,   260,  -166,  -166,  -166,
    -166,  -166,  -166,   274,   491,   491,     9,  -166,   113,   188,
    -166,   196,  -166,  -166,   181,   182,  -166,   114,  -166,    80,
     114,   223,  -166,  -166,   179,  -166,  -166,   184,  -166,   196,
     117,   429,   195,   197,   152,   279,   -31,   121,  -166,   194,
    -166,  -166,   227,   199,   625,  -166,   236,   152,  -166,  -166,
      -5,  -166,  -166,   257,  -166,   491,  -166,   103,   196,   233,
    -166,  -166,   625,   207,  -166,  -166,   152,  -166,  -166,    49,
     243,   152,    51,    73,     5,  -166,   126,  -166,  -166,   276,
     253,  -166,  -166,   491,     7,   152,   152,   125,  -166,  -166,
    -166,  -166,  -166,  -166,  -166,    35,    14,  -166,   429,  -166,
    -166,   216,  -166,   112,   491,  -166,  -166,   625,  -166,   152,
     258,   152,   -54,   152,   259,   152,   261,  -166,  -166,   491,
     429,   244,   196,  -166,   134,   136,  -166,   317,   -31,  -166,
      10,   152,   491,   196,   138,   -51,   152,   -42,   429,   -40,
     152,   -37,   152,   225,   254,   295,   281,   271,  -166,  -166,
    -166,  -166,   251,   196,  -166,   429,   -19,   429,   254,   429,
     -10,   429,     0,   491,   346,  -166,   152,  -166,   254,   429,
     254,   254,   429,   254,   429,   252,  -166,   108,  -166,   255,
     254,   254,   254,   491,  -166,  -166,   284,   152,  -166,   -29,
    -166,   144,  -166,  -166,  -166
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     4,     0,     0,     0,     0,     0,     0,    15,     0,
       0,     5,     0,     0,    12,     6,     8,    10,    11,     7,
      14,     9,    13,     0,     0,   188,   189,     0,     0,     0,
       0,     0,     0,    78,    79,     0,   112,     1,     3,     2,
      83,    76,     0,     0,     0,   133,    21,     0,     0,     0,
       0,     0,    84,    85,     0,     0,     0,    65,     0,     0,
      72,   134,     0,     0,   133,    74,     0,    80,     0,    81,
       0,   186,   168,   165,     0,   181,    86,    36,    25,     0,
      26,    27,    30,    32,    33,    35,     0,    37,   164,    31,
      34,    28,    29,     0,     0,     0,     0,     0,    87,    88,
      92,   150,   152,   154,   157,   156,   155,     0,   173,     0,
       0,     0,     0,     0,    64,    23,     0,     0,     0,     0,
     135,   137,   139,   141,     0,   155,     0,     0,     0,    71,
       0,     0,   113,     0,   166,   167,     0,    39,   169,     0,
      40,     0,   170,     0,   133,     0,   182,   183,     0,     0,
      91,   184,   185,     0,     0,   153,     0,     0,    16,     0,
       0,     0,     0,    17,    18,    19,     0,    60,     0,    38,
      52,   140,     0,     0,     0,     0,     0,   175,   177,   178,
     179,   180,   176,     0,     0,     0,     0,   171,     0,     0,
      73,    75,   106,   187,     0,     0,   158,     0,   114,    94,
     109,   116,    89,    90,   149,   151,   174,     0,   159,   162,
       0,     0,     0,     0,     0,     0,     0,     0,    59,    61,
      20,    24,     0,     0,     0,    43,     0,     0,    45,    51,
      53,    22,   148,   136,   138,     0,   146,     0,   147,     0,
     145,    63,     0,     0,    41,    42,     0,   107,   110,     0,
       0,     0,     0,     0,     0,    93,    95,    97,   108,     0,
     118,   160,   161,     0,     0,     0,     0,     0,    67,   192,
     193,   191,   190,    68,    66,     0,     0,    44,     0,    47,
      46,     0,    50,     0,     0,   144,   172,     0,   111,     0,
       0,     0,     0,     0,     0,     0,     0,   115,    96,     0,
       0,   120,   163,    57,     0,     0,    54,     0,     0,    58,
       0,     0,     0,   142,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   117,   119,     0,   122,     0,    55,    69,
      70,    48,     0,   143,    62,     0,     0,     0,    99,     0,
       0,     0,     0,     0,     0,    82,     0,    49,   105,     0,
      98,   101,     0,   103,     0,   121,   124,   127,   123,     0,
     104,   100,   102,     0,   128,   129,   130,     0,   125,     0,
     126,     0,   131,   132,    56
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -166,  -166,  -166,  -166,  -166,  -166,  -166,   -96,  -166,   239,
     127,  -166,  -166,  -164,  -166,  -166,  -166,  -166,  -166,  -166,
    -166,  -166,  -166,   232,  -166,  -166,  -166,   313,   297,  -166,
    -166,  -166,   218,  -166,  -166,  -166,   110,   320,   118,   171,
    -165,  -166,  -166,  -166,  -166,  -166,  -166,    11,  -166,  -166,
     -55,  -166,   -50,   202,   198,   263,   -43,   228,   229,   272,
    -166,    79,  -124,    96,   -25,  -166,  -166,  -166,  -166,   -62,
      -3,    76
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    12,    13,    14,    15,    16,    17,   115,   116,    96,
     229,   230,   231,   163,   219,   220,    18,    19,   114,   217,
      20,    21,    64,    65,    22,    23,    33,    34,    41,    54,
      97,    98,    99,   144,   255,   256,   257,   197,   198,   247,
      35,   199,   260,   301,   326,   345,   355,   356,   366,   370,
      60,    61,   120,   121,   122,   123,   124,   101,   102,   103,
     104,   210,   105,   188,   106,   185,   107,   149,   153,    70,
     108,   273
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      27,   127,   187,   218,   222,   174,    32,    36,    25,   129,
      26,   100,   156,   158,   174,   146,   147,   174,   239,   318,
     269,    42,   335,   372,   215,    44,    45,    46,    47,   216,
     223,   337,   248,   339,   125,   248,   341,   373,   224,    55,
       1,   270,   271,    62,    24,    66,    36,   133,    71,   159,
     133,    63,   141,    28,   349,   117,    29,   272,    59,   133,
      71,   133,    56,   352,   133,   225,   128,     2,   160,   172,
      30,   226,   221,   354,   227,    52,   173,     3,     4,   240,
     307,   288,   133,     5,   228,   308,   157,   191,     6,   201,
     161,   133,    31,   125,   125,    53,   196,   150,   232,     7,
     279,   133,   100,   162,    68,    37,   117,   303,   164,   165,
     331,   284,   309,   289,   209,   293,     8,    25,   286,    26,
     312,   -77,   146,   147,   290,    66,   294,   146,   147,     9,
     193,   146,   147,    10,    40,    11,   249,   295,   364,   250,
     200,   237,   238,   246,   251,   203,    43,   252,   296,   125,
     125,    48,   267,   206,   365,    25,    38,    26,    39,    49,
     253,   264,    25,   187,    26,   117,   176,   177,   178,   179,
     180,   181,   182,   183,   184,    50,   146,   147,   146,   147,
      51,   254,   249,   109,   110,   250,   125,    57,   148,   292,
     251,    58,   283,   252,    36,   132,   133,    36,   111,   112,
     151,   152,   130,   304,   305,    68,   253,   167,   168,   189,
     133,    71,    59,   241,   242,   146,   147,   262,   263,   126,
     302,   274,   275,   113,   281,   306,   133,   315,   310,   317,
     134,   319,   135,   321,   327,   133,   328,   133,   334,   242,
     136,   313,   137,    36,   374,   133,   138,   139,    71,   140,
     324,   200,   142,   125,   336,   143,   209,   196,   340,   145,
     342,   174,    71,    71,   166,   175,   192,   186,   338,   333,
     194,   195,   211,   212,   213,   125,   214,   235,   236,   243,
     259,   244,   245,   268,   261,   348,    71,   350,    71,   351,
      71,   353,    71,   125,   265,   276,   266,   277,   278,   360,
     357,   280,   361,   285,   362,   371,   287,   291,   332,   299,
     125,   300,   125,    71,   125,   311,   125,    71,   325,    71,
     357,   329,   316,   320,   125,   322,   263,   125,   343,   125,
      25,    72,    26,    73,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   359,   146,   147,    74,    75,   207,   344,
     346,   347,   358,   363,   367,   369,   170,   282,    77,    78,
     190,    79,    67,   202,    71,   131,   298,    80,    81,    82,
      69,   258,   297,   234,   368,    83,   233,   204,   323,   155,
      84,   171,   205,   314,   330,     0,     0,     0,    85,    86,
       0,     0,    25,    72,    26,    73,    87,    88,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,    74,    75,
      76,    90,     0,    91,    92,     0,     0,     0,     0,    93,
      77,    78,    94,    79,     0,     0,    95,   208,     0,    80,
      81,    82,    25,    72,    26,    73,     0,    83,   118,     0,
       0,     0,    84,     0,     0,     0,     0,     0,    74,    75,
      85,    86,     0,     0,     0,     0,     0,     0,    87,    88,
      77,    78,     0,    79,     0,     0,     0,    89,     0,    80,
      81,    82,     0,    90,     0,    91,    92,    83,     0,     0,
       0,    93,    84,     0,    94,     0,     0,     0,    95,     0,
      85,    86,     0,     0,    25,    72,    26,    73,    87,    88,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
      74,    75,     0,    90,     0,    91,    92,     0,     0,     0,
       0,    93,    77,    78,    94,    79,     0,     0,   119,     0,
       0,    80,    81,    82,    25,    72,    26,    73,     0,    83,
       0,     0,     0,     0,    84,     0,     0,     0,     0,     0,
      74,   154,    85,    86,     0,     0,     0,     0,     0,     0,
      87,    88,    77,    78,     0,    79,     0,     0,     0,    89,
       0,    80,    81,    82,     0,    90,     0,    91,    92,    83,
       0,     0,     0,    93,    84,     0,    94,     0,     0,     0,
      95,     0,    85,    86,     0,     0,    25,    72,    26,    73,
      87,    88,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,    74,    75,     0,    90,     0,    91,    92,     0,
       0,     0,     0,    93,    77,    78,    94,    79,     0,    72,
      95,    73,     0,    80,    81,    82,     0,     0,     0,     0,
       0,    83,     0,     0,    74,   154,    84,     0,     0,     0,
       0,     0,     0,     0,    85,    86,    77,    78,     0,    79,
       0,     0,    87,    88,     0,    80,    81,    82,     0,     0,
       0,    89,     0,    83,     0,     0,     0,    90,    84,    91,
      92,     0,     0,     0,     0,    93,    85,    86,    94,     0,
       0,     0,   119,     0,    87,    88,    77,    78,     0,    79,
       0,     0,     0,    89,     0,    80,    81,    82,     0,    90,
       0,    91,    92,    83,     0,     0,     0,    93,    84,     0,
      94,     0,     0,     0,     0,     0,    85,   169,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,    90,
       0,    91,    92,     0,     0,     0,     0,    93,     0,     0,
      94
};

static const yytype_int16 yycheck[] =
{
       3,    63,   126,   167,     9,     7,     9,    10,     3,    64,
       5,    54,    25,   109,     7,    19,    20,     7,     9,    73,
      51,    24,    73,    52,    45,    28,    29,    30,    31,    50,
      35,    73,   197,    73,    59,   200,    73,    66,    43,    26,
       1,    72,    73,    91,    85,    48,    49,   101,    51,    35,
     101,    99,    95,    85,    73,    58,    55,    88,    93,   101,
      63,   101,    49,    73,   101,    70,   101,    28,    54,   119,
      85,    76,   168,    73,    79,    27,   119,    38,    39,    70,
      45,   246,   101,    44,    89,    50,    99,   130,    49,   144,
      76,   101,    63,   118,   119,    47,   100,   100,   100,    60,
     224,   101,   145,    89,    99,     0,   109,   100,   111,   112,
     100,     8,   276,    64,   157,    64,    77,     3,   242,     5,
       8,    82,    19,    20,    75,   128,    75,    19,    20,    90,
     133,    19,    20,    94,    82,    96,    56,    64,    30,    59,
     143,   184,   185,    29,    64,   148,    55,    67,    75,   174,
     175,    83,   214,   156,    46,     3,    96,     5,    98,   101,
      80,   211,     3,   287,     5,   168,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    29,    19,    20,    19,    20,
      99,   101,    56,    36,    37,    59,   211,     4,    29,   251,
      64,    99,   235,    67,   197,   100,   101,   200,    36,    37,
      21,    22,    10,   265,   266,    99,    80,   100,   101,   100,
     101,   214,    93,   100,   101,    19,    20,   100,   101,    99,
     263,   100,   101,    94,   227,   100,   101,   289,   278,   291,
       6,   293,     6,   295,   100,   101,   100,   101,   100,   101,
      99,   284,    62,   246,   100,   101,     4,    99,   251,    62,
     300,   254,     4,   278,   316,    55,   299,   100,   320,   101,
     322,     7,   265,   266,    99,     8,   100,    18,   318,   312,
       6,     6,    99,    65,    65,   300,    99,    17,     4,    91,
      57,   100,   100,     4,   100,   335,   289,   337,   291,   339,
     293,   341,   295,   318,    99,   101,    99,    70,    99,   349,
     343,    65,   352,    70,   354,   367,    99,    64,   311,    33,
     335,    58,   337,   316,   339,    99,   341,   320,    74,   322,
     363,     4,    64,    64,   349,    64,   101,   352,    33,   354,
       3,     4,     5,     6,     9,    10,    11,    12,    13,    14,
      15,    16,    17,   346,    19,    20,    19,    20,    21,    68,
      79,   100,     6,   101,    99,    71,   117,   230,    31,    32,
     128,    34,    49,   145,   367,    68,   256,    40,    41,    42,
      50,   200,   254,   175,   363,    48,   174,   149,   299,   107,
      53,   118,   153,   287,   308,    -1,    -1,    -1,    61,    62,
      -1,    -1,     3,     4,     5,     6,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    19,    20,
      21,    84,    -1,    86,    87,    -1,    -1,    -1,    -1,    92,
      31,    32,    95,    34,    -1,    -1,    99,   100,    -1,    40,
      41,    42,     3,     4,     5,     6,    -1,    48,     9,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    19,    20,
      61,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      31,    32,    -1,    34,    -1,    -1,    -1,    78,    -1,    40,
      41,    42,    -1,    84,    -1,    86,    87,    48,    -1,    -1,
      -1,    92,    53,    -1,    95,    -1,    -1,    -1,    99,    -1,
      61,    62,    -1,    -1,     3,     4,     5,     6,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      19,    20,    -1,    84,    -1,    86,    87,    -1,    -1,    -1,
      -1,    92,    31,    32,    95,    34,    -1,    -1,    99,    -1,
      -1,    40,    41,    42,     3,     4,     5,     6,    -1,    48,
      -1,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      19,    20,    61,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    31,    32,    -1,    34,    -1,    -1,    -1,    78,
      -1,    40,    41,    42,    -1,    84,    -1,    86,    87,    48,
      -1,    -1,    -1,    92,    53,    -1,    95,    -1,    -1,    -1,
      99,    -1,    61,    62,    -1,    -1,     3,     4,     5,     6,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    19,    20,    -1,    84,    -1,    86,    87,    -1,
      -1,    -1,    -1,    92,    31,    32,    95,    34,    -1,     4,
      99,     6,    -1,    40,    41,    42,    -1,    -1,    -1,    -1,
      -1,    48,    -1,    -1,    19,    20,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    31,    32,    -1,    34,
      -1,    -1,    69,    70,    -1,    40,    41,    42,    -1,    -1,
      -1,    78,    -1,    48,    -1,    -1,    -1,    84,    53,    86,
      87,    -1,    -1,    -1,    -1,    92,    61,    62,    95,    -1,
      -1,    -1,    99,    -1,    69,    70,    31,    32,    -1,    34,
      -1,    -1,    -1,    78,    -1,    40,    41,    42,    -1,    84,
      -1,    86,    87,    48,    -1,    -1,    -1,    92,    53,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    61,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    86,    87,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      95
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,    28,    38,    39,    44,    49,    60,    77,    90,
      94,    96,   103,   104,   105,   106,   107,   108,   118,   119,
     122,   123,   126,   127,    85,     3,     5,   172,    85,    55,
      85,    63,   172,   128,   129,   142,   172,     0,    96,    98,
      82,   130,   172,    55,   172,   172,   172,   172,    83,   101,
      29,    99,    27,    47,   131,    26,    49,     4,    99,    93,
     152,   153,    91,    99,   124,   125,   172,   129,    99,   139,
     171,   172,     4,     6,    19,    20,    21,    31,    32,    34,
      40,    41,    42,    48,    53,    61,    62,    69,    70,    78,
      84,    86,    87,    92,    95,    99,   111,   132,   133,   134,
     158,   159,   160,   161,   162,   164,   166,   168,   172,    36,
      37,    36,    37,    94,   120,   109,   110,   172,     9,    99,
     154,   155,   156,   157,   158,   166,    99,   171,   101,   152,
      10,   130,   100,   101,     6,     6,    99,    62,     4,    99,
      62,   158,     4,    55,   135,   101,    19,    20,    29,   169,
     172,    21,    22,   170,    20,   161,    25,    99,   109,    35,
      54,    76,    89,   115,   172,   172,    99,   100,   101,    62,
     111,   157,   154,   158,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,   167,    18,   164,   165,   100,
     125,   158,   100,   172,     6,     6,   100,   139,   140,   143,
     172,   152,   134,   172,   159,   160,   172,    21,   100,   158,
     163,    99,    65,    65,    99,    45,    50,   121,   115,   116,
     117,   109,     9,    35,    43,    70,    76,    79,    89,   112,
     113,   114,   100,   155,   156,    17,     4,   158,   158,     9,
      70,   100,   101,    91,   100,   100,    29,   141,   142,    56,
      59,    64,    67,    80,   101,   136,   137,   138,   141,    57,
     144,   100,   100,   101,   154,    99,    99,   171,     4,    51,
      72,    73,    88,   173,   100,   101,   101,    70,    99,   164,
      65,   172,   112,   158,     8,    70,   164,    99,   142,    64,
      75,    64,   171,    64,    75,    64,    75,   140,   138,    33,
      58,   145,   158,   100,   171,   171,   100,    45,    50,   115,
     154,    99,     8,   158,   165,   171,    64,   171,    73,   171,
      64,   171,    64,   163,   154,    74,   146,   100,   100,     4,
     173,   100,   172,   158,   100,    73,   171,    73,   154,    73,
     171,    73,   171,    33,    68,   147,    79,   100,   154,    73,
     154,   154,    73,   154,    73,   148,   149,   158,     6,   172,
     154,   154,   154,   101,    30,    46,   150,    99,   149,    71,
     151,   171,    52,    66,   100
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   102,   103,   103,   103,   103,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   105,   106,   106,   106,   106,
     107,   108,   109,   110,   110,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   111,   111,   111,
     111,   111,   111,   112,   112,   112,   112,   112,   112,   112,
     113,   113,   114,   114,   115,   115,   115,   115,   116,   116,
     117,   117,   118,   118,   119,   120,   120,   121,   121,   121,
     121,   122,   123,   124,   124,   125,   126,   127,   127,   128,
     128,   129,   130,   131,   131,   131,   132,   132,   133,   133,
     134,   134,   134,   135,   136,   136,   137,   137,   138,   138,
     138,   138,   138,   138,   138,   138,   139,   140,   140,   140,
     141,   141,   142,   142,   143,   143,   144,   144,   145,   145,
     146,   146,   147,   147,   148,   148,   149,   150,   150,   150,
     151,   151,   151,   152,   152,   153,   154,   154,   155,   155,
     156,   156,   157,   157,   157,   157,   157,   157,   157,   158,
     158,   159,   159,   160,   160,   161,   161,   161,   161,   162,
     162,   162,   163,   163,   164,   164,   164,   164,   164,   164,
     164,   165,   165,   166,   166,   167,   167,   167,   167,   167,
     167,   168,   169,   169,   170,   170,   171,   171,   172,   172,
     173,   173,   173,   173
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     6,     6,     6,     6,
       7,     3,     3,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     4,     4,     1,     2,     1,     2,     2,     4,     5,
       2,     1,     0,     1,     4,     5,    10,     4,     3,     1,
       0,     1,    10,     7,     5,     0,     4,     2,     2,     4,
       4,     5,     4,     3,     1,     3,     2,     0,     2,     1,
       3,     3,     9,     0,     1,     1,     1,     1,     1,     3,
       3,     2,     1,     3,     0,     1,     2,     1,     5,     4,
       6,     5,     6,     5,     6,     5,     3,     2,     2,     1,
       1,     2,     1,     4,     1,     3,     0,     3,     0,     2,
       0,     3,     0,     2,     1,     3,     3,     0,     1,     1,
       0,     2,     2,     0,     1,     2,     3,     1,     3,     1,
       2,     1,     5,     6,     4,     3,     3,     3,     3,     3,
       1,     3,     1,     2,     1,     1,     1,     1,     3,     3,
       4,     4,     1,     3,     1,     1,     2,     2,     1,     2,
       2,     1,     3,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     1,
       1,     1,     1,     1
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
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1670 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1680 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1690 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_UNSIGNED_NUMVAL  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 1700 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 104: /* sql_statement  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1710 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 105: /* quit_statement  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 1720 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 106: /* alter_table_statement  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1730 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 107: /* create_table_statement  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 1740 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 108: /* drop_table_statement  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 1750 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 109: /* column_def  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 1760 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 110: /* column_def_commalist  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 1770 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 111: /* data_type  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 1780 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 112: /* column_constraint_def  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 1790 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 113: /* column_constraint_def_list  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1800 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 114: /* opt_column_constraint_def_list  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1810 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 118: /* insert_statement  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 1820 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 119: /* copy_from_statement  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_statement_) != nullptr) {
    delete ((*yyvaluep).copy_from_statement_);
  }
}
#line 1830 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 120: /* opt_copy_from_params  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 1840 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 121: /* copy_from_params  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 1850 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 122: /* update_statement  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 1860 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 123: /* delete_statement  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 1870 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 124: /* assignment_list  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 1880 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 125: /* assignment_item  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 1890 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 126: /* select_statement  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_statement_) != nullptr) {
    delete ((*yyvaluep).select_statement_);
  }
}
#line 1900 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 127: /* opt_with_clause  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 1910 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 128: /* with_list  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 1920 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 129: /* with_list_element  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 1930 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 130: /* select_query  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 1940 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 132: /* selection  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 1950 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 133: /* selection_item_commalist  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 1960 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 134: /* selection_item  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 1970 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 135: /* from_clause  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 1980 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 139: /* subquery_expression  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 1990 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 140: /* table_reference  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2000 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 141: /* table_reference_signature  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2010 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 142: /* table_reference_signature_primary  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2020 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 143: /* table_reference_commalist  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2030 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 144: /* opt_group_by_clause  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2040 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 145: /* opt_having_clause  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2050 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 146: /* opt_order_by_clause  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2060 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 147: /* opt_limit_clause  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2070 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 148: /* order_commalist  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2080 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 149: /* order_item  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2090 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 150: /* opt_order_direction  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2100 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 151: /* opt_nulls_first  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2110 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 152: /* opt_where_clause  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2120 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 153: /* where_clause  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2130 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* or_expression  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2140 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* and_expression  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2150 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* not_expression  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2160 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* predicate_expression_base  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2170 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* add_expression  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2180 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* multiply_expression  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2190 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* unary_expression  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2200 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* expression_base  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2210 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* function_call  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2220 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 163: /* expression_list  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2230 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 164: /* literal_value  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2240 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 165: /* literal_value_commalist  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2250 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 166: /* attribute_ref  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2260 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 167: /* comparison_operation  */
#line 436 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2266 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* unary_operation  */
#line 437 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2272 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* add_operation  */
#line 438 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2278 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* multiply_operation  */
#line 438 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2284 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* name_commalist  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 2294 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* any_name  */
#line 440 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2304 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* boolean_value  */
#line 435 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2310 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 449 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2607 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 453 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2616 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 457 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    YYABORT;
  }
#line 2624 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 5:
#line 460 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 2633 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 467 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 2641 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 470 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].copy_from_statement_);
  }
#line 2649 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 473 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 2657 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 9:
#line 476 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 2665 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 10:
#line 479 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 2673 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 11:
#line 482 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 2681 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 12:
#line 485 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 2689 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 13:
#line 488 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].select_statement_);
  }
#line 2697 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 14:
#line 491 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 2705 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 15:
#line 497 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 2713 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 16:
#line 503 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2725 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 17:
#line 510 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2736 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 18:
#line 516 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2748 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 19:
#line 523 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2760 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 20:
#line 532 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-2].attribute_definition_list_));
  }
#line 2768 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 537 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 2776 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 542 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 2784 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 547 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 2793 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 551 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 2802 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 557 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 2812 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 562 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 2820 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 565 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 2828 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 568 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 2838 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 573 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 2846 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 576 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 2854 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 579 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 2862 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 582 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 2870 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 585 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 2878 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 588 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 2886 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 591 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 2894 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 594 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 2902 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 597 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 2910 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 600 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 2927 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 612 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 2935 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 615 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 2943 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 618 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 2966 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 636 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 2989 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 656 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 2997 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 659 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3005 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 662 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3015 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 667 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3025 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 672 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3036 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 678 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3047 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 684 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3059 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 693 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3068 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 697 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3077 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 703 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3085 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 706 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3093 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 711 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3103 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 716 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3113 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 721 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3125 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 728 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3135 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 735 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3144 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 739 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3153 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 745 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3161 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 748 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = $1; */
  }
#line 3169 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 754 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 3182 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 762 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsert((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 3190 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 767 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_statement_) = new quickstep::ParseStatementCopyFrom((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].copy_from_params_));
  }
#line 3198 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 772 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = nullptr;
  }
#line 3206 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 775 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-1].copy_from_params_);
  }
#line 3214 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 780 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3223 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 784 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3232 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 788 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3241 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 792 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3250 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 798 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 3258 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 803 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 3266 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 808 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3275 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 812 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3284 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 818 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 3292 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 824 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].select_query_), (yyvsp[-1].with_list_));
  }
#line 3300 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 829 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = nullptr;
  }
#line 3308 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 832 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 3316 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 837 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3325 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 841 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3334 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 847 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 3343 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 854 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].selection_), (yyvsp[-5].table_reference_list_), (yyvsp[-4].predicate_), (yyvsp[-3].opt_group_by_clause_), (yyvsp[-2].opt_having_clause_), (yyvsp[-1].opt_order_by_clause_), (yyvsp[0].opt_limit_clause_));
  }
#line 3351 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 859 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3359 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 862 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "ALL in selection");
    YYERROR;
  }
#line 3368 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 866 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "DISTINCT in selection");
    YYERROR;
  }
#line 3377 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 872 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3385 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 875 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 3393 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 880 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3402 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 884 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3411 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 890 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 3419 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 893 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 3427 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 896 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 3435 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 901 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-1].table_reference_list_);
  }
#line 3443 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 906 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3451 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 909 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3460 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 915 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-1]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3469 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 919 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3478 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 925 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3489 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 931 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3500 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 937 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3511 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 943 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3522 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 949 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3533 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 955 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3544 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 961 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3555 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 967 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3566 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 975 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].select_query_));
  }
#line 3574 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 980 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 3583 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 984 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 3592 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 988 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 3600 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 993 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 3608 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 996 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 3616 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 1001 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 3624 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 1004 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 3632 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 1009 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 3641 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 1013 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 3650 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 1019 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 3658 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 1022 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 3666 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 1027 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 3674 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 1030 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 3682 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 1035 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 3690 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 1038 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 3698 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 1043 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 3706 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 1046 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3728 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 1065 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 3737 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 1069 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 3746 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 1075 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 3756 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 1082 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 3764 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 1085 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 3772 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 1088 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 3780 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 1093 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 3788 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 1096 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 3796 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 1099 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 3804 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 1105 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 3812 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 1108 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 3820 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 1113 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 3828 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 1118 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 3842 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 1127 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 3850 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 1132 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 3864 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 1141 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 3872 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 1146 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 3880 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 1149 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 3888 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 1154 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 3896 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 1157 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 3906 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 1162 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 3917 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 1168 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 3928 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 1174 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].expression_);
    delete (yyvsp[0].string_value_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "LIKE predicates");
    YYERROR;
  }
#line 3940 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 1181 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 3948 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 1184 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 3956 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 1191 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 3964 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 1194 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 3972 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 1199 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 3980 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 1202 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 3988 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 1207 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].unary_operation_), (yyvsp[0].expression_));
  }
#line 3996 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 1210 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4004 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 1215 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 4012 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 1218 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 4020 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 1221 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 4028 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 1224 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 4036 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 1229 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 4045 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 1233 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 4054 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 1237 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4062 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 1242 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4071 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 1246 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4080 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 1252 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4088 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 1255 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4096 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 1258 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4104 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 1261 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4121 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 1273 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 4130 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 1277 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4154 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 1296 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4172 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 1311 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4181 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 1315 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4190 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 1321 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4198 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 1324 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 4206 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 1330 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 4214 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 1333 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 4222 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 1336 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 4230 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 1339 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 4238 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 1342 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 4246 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 1345 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 4254 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 1350 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
#line 4268 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 1361 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
#line 4276 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 1364 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
#line 4284 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 1369 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
#line 4292 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 1372 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
#line 4300 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1378 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4309 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 1382 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4318 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 1388 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 4326 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 1391 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 4337 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 1399 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4345 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 1402 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4353 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 1405 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 4361 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 1408 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 4369 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;


#line 4373 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 1412 "../SqlParser.ypp" /* yacc.c:1906  */


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
