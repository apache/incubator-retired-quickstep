/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.2"

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
#include "parser/ParseKeyValue.hpp"
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

#line 144 "SqlParser_gen.cpp" /* yacc.c:339  */

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
    TOKEN_BLOCKPROPERTIES = 283,
    TOKEN_BLOOM_FILTER = 284,
    TOKEN_CSB_TREE = 285,
    TOKEN_BY = 286,
    TOKEN_CHARACTER = 287,
    TOKEN_CHECK = 288,
    TOKEN_COLUMN = 289,
    TOKEN_CONSTRAINT = 290,
    TOKEN_COPY = 291,
    TOKEN_CREATE = 292,
    TOKEN_DATE = 293,
    TOKEN_DATETIME = 294,
    TOKEN_DECIMAL = 295,
    TOKEN_DEFAULT = 296,
    TOKEN_DELETE = 297,
    TOKEN_DELIMITER = 298,
    TOKEN_DESC = 299,
    TOKEN_DISTINCT = 300,
    TOKEN_DOUBLE = 301,
    TOKEN_DROP = 302,
    TOKEN_ESCAPE_STRINGS = 303,
    TOKEN_FALSE = 304,
    TOKEN_FIRST = 305,
    TOKEN_FLOAT = 306,
    TOKEN_FOREIGN = 307,
    TOKEN_FROM = 308,
    TOKEN_FULL = 309,
    TOKEN_GROUP = 310,
    TOKEN_HAVING = 311,
    TOKEN_INDEX = 312,
    TOKEN_INNER = 313,
    TOKEN_INSERT = 314,
    TOKEN_INTEGER = 315,
    TOKEN_INTERVAL = 316,
    TOKEN_INTO = 317,
    TOKEN_JOIN = 318,
    TOKEN_KEY = 319,
    TOKEN_LAST = 320,
    TOKEN_LEFT = 321,
    TOKEN_LIMIT = 322,
    TOKEN_LONG = 323,
    TOKEN_NULL = 324,
    TOKEN_NULLS = 325,
    TOKEN_OFF = 326,
    TOKEN_ON = 327,
    TOKEN_ORDER = 328,
    TOKEN_OUTER = 329,
    TOKEN_PRIMARY = 330,
    TOKEN_QUIT = 331,
    TOKEN_REAL = 332,
    TOKEN_REFERENCES = 333,
    TOKEN_RIGHT = 334,
    TOKEN_ROW_DELIMITER = 335,
    TOKEN_SELECT = 336,
    TOKEN_SET = 337,
    TOKEN_SMALLINT = 338,
    TOKEN_TABLE = 339,
    TOKEN_TIME = 340,
    TOKEN_TIMESTAMP = 341,
    TOKEN_TRUE = 342,
    TOKEN_UNIQUE = 343,
    TOKEN_UPDATE = 344,
    TOKEN_USING = 345,
    TOKEN_VALUES = 346,
    TOKEN_VARCHAR = 347,
    TOKEN_WHERE = 348,
    TOKEN_WITH = 349,
    TOKEN_YEARMONTH = 350,
    TOKEN_EOF = 351,
    TOKEN_LEX_ERROR = 352
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 106 "../SqlParser.ypp" /* yacc.c:355  */

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

  quickstep::ParseKeyValue *key_value_;
  quickstep::PtrList<quickstep::ParseKeyValue> *key_value_list_;
  quickstep::ParseKeyStringValue *key_string_value_;
  quickstep::ParseKeyStringList *key_string_list_;
  quickstep::ParseKeyIntegerValue *key_integer_value_;

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

#line 358 "SqlParser_gen.cpp" /* yacc.c:355  */
};
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
#line 182 "../SqlParser.ypp" /* yacc.c:358  */

/* This header needs YYSTYPE, which is defined by the %union directive above */
#include "SqlLexer_gen.hpp"
void NotSupported(const YYLTYPE *location, yyscan_t yyscanner, const std::string &feature);

#line 391 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYLAST   911

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  107
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  82
/* YYNRULES -- Number of rules.  */
#define YYNRULES  211
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  410

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   352

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
     104,   105,    21,    19,   106,    20,    25,    22,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   103,
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
     100,   101,   102
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   493,   493,   497,   501,   504,   511,   514,   517,   520,
     523,   526,   529,   532,   535,   538,   544,   550,   557,   563,
     570,   579,   584,   596,   601,   606,   610,   616,   621,   624,
     627,   632,   635,   638,   641,   644,   647,   650,   653,   656,
     659,   671,   674,   677,   695,   715,   718,   721,   726,   731,
     737,   743,   752,   756,   762,   765,   770,   775,   780,   787,
     794,   798,   804,   807,   812,   815,   823,   826,   831,   835,
     841,   844,   847,   852,   857,   862,   873,   878,   885,   888,
     895,   903,   908,   913,   916,   921,   925,   929,   933,   939,
     944,   949,   953,   959,   965,   970,   973,   978,   982,   988,
     994,  1000,  1003,  1007,  1013,  1016,  1021,  1025,  1031,  1034,
    1037,  1042,  1047,  1050,  1056,  1060,  1066,  1072,  1078,  1084,
    1090,  1096,  1102,  1108,  1116,  1121,  1125,  1129,  1134,  1137,
    1142,  1145,  1150,  1154,  1160,  1163,  1168,  1171,  1176,  1179,
    1184,  1187,  1206,  1210,  1216,  1223,  1226,  1229,  1234,  1237,
    1240,  1246,  1249,  1254,  1259,  1268,  1273,  1282,  1287,  1290,
    1295,  1298,  1303,  1309,  1315,  1322,  1325,  1332,  1335,  1340,
    1343,  1348,  1351,  1356,  1359,  1362,  1365,  1370,  1374,  1378,
    1383,  1387,  1393,  1396,  1399,  1402,  1414,  1418,  1437,  1452,
    1456,  1462,  1465,  1471,  1474,  1477,  1480,  1483,  1486,  1491,
    1502,  1505,  1510,  1513,  1519,  1523,  1529,  1532,  1540,  1543,
    1546,  1549
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
  "TOKEN_BIT", "TOKEN_BLOCKPROPERTIES", "TOKEN_BLOOM_FILTER",
  "TOKEN_CSB_TREE", "TOKEN_BY", "TOKEN_CHARACTER", "TOKEN_CHECK",
  "TOKEN_COLUMN", "TOKEN_CONSTRAINT", "TOKEN_COPY", "TOKEN_CREATE",
  "TOKEN_DATE", "TOKEN_DATETIME", "TOKEN_DECIMAL", "TOKEN_DEFAULT",
  "TOKEN_DELETE", "TOKEN_DELIMITER", "TOKEN_DESC", "TOKEN_DISTINCT",
  "TOKEN_DOUBLE", "TOKEN_DROP", "TOKEN_ESCAPE_STRINGS", "TOKEN_FALSE",
  "TOKEN_FIRST", "TOKEN_FLOAT", "TOKEN_FOREIGN", "TOKEN_FROM",
  "TOKEN_FULL", "TOKEN_GROUP", "TOKEN_HAVING", "TOKEN_INDEX",
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
  "opt_column_list", "opt_block_properties", "key_value_list", "key_value",
  "key_string_value", "key_string_list", "key_integer_value", "index_type",
  "opt_index_properties", "insert_statement", "copy_from_statement",
  "opt_copy_from_params", "copy_from_params", "update_statement",
  "delete_statement", "assignment_list", "assignment_item",
  "select_statement", "opt_with_clause", "with_list", "with_list_element",
  "select_query", "opt_all_distinct", "selection",
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
     350,   351,   352,    59,    40,    41,    44
};
# endif

#define YYPACT_NINF -191

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-191)))

#define YYTABLE_NINF -96

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     101,  -191,   -77,    33,    44,    20,   -33,    61,  -191,    33,
      33,  -191,   131,   -26,  -191,  -191,  -191,  -191,  -191,  -191,
    -191,  -191,  -191,  -191,    14,    33,  -191,  -191,    96,    33,
      33,    33,    33,    33,    73,    69,  -191,   159,    92,  -191,
    -191,  -191,     4,  -191,    82,   211,   151,   127,   138,  -191,
      66,    33,    33,   141,    33,  -191,  -191,   420,    85,   166,
     165,    33,    33,   494,  -191,  -191,   146,    33,    80,  -191,
     248,  -191,    14,  -191,    16,  -191,  -191,  -191,   256,   260,
    -191,  -191,  -191,   170,  -191,   204,  -191,  -191,  -191,  -191,
     273,  -191,  -191,  -191,  -191,  -191,  -191,   175,   217,   558,
     281,   235,   196,  -191,   178,   188,  -191,  -191,  -191,  -191,
    -191,   622,     7,    33,   -20,    33,    33,   200,  -191,   205,
    -191,   106,   811,   686,   494,   301,   303,  -191,  -191,   210,
     296,   750,   108,    33,  -191,   558,   212,  -191,    33,  -191,
    -191,   309,  -191,  -191,   310,  -191,    15,  -191,    10,   138,
     558,  -191,  -191,    33,   558,  -191,  -191,  -191,   558,   260,
    -191,    33,   356,  -191,   215,   254,   258,   221,  -191,  -191,
    -191,   137,    33,   233,   -20,    33,  -191,    58,  -191,    -2,
      74,   494,   494,   312,  -191,  -191,  -191,  -191,  -191,  -191,
     326,   558,   558,     0,  -191,   129,   236,  -191,   219,  -191,
    -191,   228,   229,  -191,   144,  -191,    93,   144,   276,  -191,
    -191,   188,  -191,  -191,   234,  -191,   219,   135,   494,   240,
     241,    33,   336,   -31,   143,   148,   208,  -191,   242,   247,
    -191,   275,   246,   750,  -191,   284,    33,  -191,  -191,    58,
    -191,  -191,   303,  -191,   558,  -191,   147,   219,   280,  -191,
    -191,   750,   251,  -191,  -191,    33,  -191,  -191,   -11,   288,
      33,    47,    78,    10,  -191,   100,  -191,  -191,   321,   297,
    -191,  -191,   558,    12,    33,    33,   150,  -191,  -191,  -191,
    -191,  -191,  -191,  -191,   155,  -191,  -191,  -191,   259,   -20,
     331,  -191,  -191,   494,  -191,  -191,   261,  -191,   172,   558,
    -191,  -191,   750,  -191,    33,   298,    33,   -53,    33,   299,
      33,   306,  -191,  -191,   558,   494,   291,   219,  -191,   163,
     182,  -191,   367,   -31,    33,  -191,  -191,   274,    13,    33,
     558,   219,   184,   -44,    33,   -34,   494,   -30,    33,   -27,
      33,   277,   301,   343,   308,   302,  -191,  -191,  -191,   186,
    -191,  -191,  -191,  -191,     5,    33,  -191,   279,   219,  -191,
     494,   -14,   494,   301,   494,    -7,   494,    21,   558,   375,
    -191,    33,  -191,    33,  -191,    33,  -191,   190,  -191,   301,
     494,   301,   301,   494,   301,   494,   283,  -191,    22,  -191,
     278,  -191,   192,  -191,   301,   301,   301,   558,  -191,  -191,
     311,    33,  -191,  -191,   -15,  -191,   194,  -191,  -191,  -191
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     4,     0,     0,     0,     0,     0,     0,    16,     0,
       0,     5,     0,     0,    13,     6,     8,     9,    11,    12,
       7,    15,    10,    14,     0,     0,   206,   207,     0,     0,
       0,     0,     0,     0,     0,    96,    97,     0,   130,     1,
       3,     2,   101,    94,     0,     0,     0,     0,   151,    23,
       0,     0,     0,     0,     0,   102,   103,     0,     0,     0,
      83,     0,     0,     0,    90,   152,     0,     0,   151,    92,
       0,    98,     0,    99,     0,   204,   186,   183,     0,   199,
     104,    38,    27,     0,    28,    29,    32,    34,    35,    37,
       0,    39,   182,    33,    36,    30,    31,     0,     0,     0,
       0,     0,   105,   106,   110,   168,   170,   172,   175,   174,
     173,     0,   191,     0,     0,     0,     0,     0,    82,    64,
      25,     0,     0,     0,     0,   153,   155,   157,   159,     0,
     173,     0,     0,     0,    89,     0,     0,   131,     0,   184,
     185,     0,    41,   187,     0,    42,     0,   188,     0,   151,
       0,   200,   201,     0,     0,   109,   202,   203,     0,     0,
     171,     0,     0,    17,     0,     0,     0,     0,    18,    19,
      20,     0,     0,     0,    62,     0,    40,    54,   158,     0,
       0,     0,     0,     0,   193,   195,   196,   197,   198,   194,
       0,     0,     0,     0,   189,     0,     0,    91,    93,   124,
     205,     0,     0,   176,     0,   132,   112,   127,   134,   107,
     108,   167,   169,   192,     0,   177,   180,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,    63,    66,
      26,     0,     0,     0,    45,     0,     0,    47,    53,    55,
      24,   166,   154,   156,     0,   164,     0,   165,     0,   163,
      81,     0,     0,    43,    44,     0,   125,   128,     0,     0,
       0,     0,     0,     0,   111,   113,   115,   126,     0,   136,
     178,   179,     0,     0,     0,     0,     0,    85,   210,   211,
     209,   208,    86,    84,     0,    65,    76,    77,    78,     0,
       0,    21,    46,     0,    49,    48,     0,    52,     0,     0,
     162,   190,     0,   129,     0,     0,     0,     0,     0,     0,
       0,     0,   133,   114,     0,     0,   138,   181,    59,     0,
       0,    56,     0,     0,     0,    22,    60,     0,     0,     0,
       0,   160,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   135,   137,     0,   140,     0,    57,    87,    88,     0,
      68,    70,    71,    72,     0,     0,    50,     0,   161,    80,
       0,     0,     0,   117,     0,     0,     0,     0,     0,     0,
     100,     0,    79,     0,    75,     0,    73,     0,    51,   123,
       0,   116,   119,     0,   121,     0,   139,   142,   145,   141,
       0,    69,     0,    67,   122,   118,   120,     0,   146,   147,
     148,     0,    74,   143,     0,   144,     0,   149,   150,    58
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -191,  -191,  -191,  -191,  -191,  -191,  -191,  -191,  -109,  -191,
     268,   152,  -191,  -191,  -173,  -191,  -191,  -191,  -191,    37,
      23,  -191,  -191,  -191,  -191,  -191,  -191,  -191,  -191,  -191,
    -191,  -191,  -191,   262,  -191,  -191,  -191,   342,   325,  -191,
    -191,  -191,   252,  -191,  -191,  -191,   139,   350,   142,   199,
    -190,  -191,  -191,  -191,  -191,  -191,  -191,    11,  -191,  -191,
     -52,  -191,   -99,   230,   227,   287,   -55,   263,   255,   304,
    -191,   102,  -128,   112,   -42,  -191,  -191,  -191,  -191,   -28,
      -3,    95
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    12,    13,    14,    15,    16,    17,    18,   120,   121,
     100,   238,   239,   240,   168,   228,   229,   173,   291,   349,
     350,   351,   352,   353,   288,   325,    19,    20,   118,   224,
      21,    22,    68,    69,    23,    24,    35,    36,    43,    57,
     101,   102,   103,   149,   264,   265,   266,   204,   205,   256,
      37,   206,   269,   316,   344,   370,   386,   387,   400,   405,
      64,    65,   125,   126,   127,   128,   129,   105,   106,   107,
     108,   217,   109,   195,   110,   192,   111,   154,   158,    74,
     112,   282
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      28,   227,   104,   194,   163,   181,    34,    38,    26,   248,
      27,   374,    25,    26,   257,    27,   134,   257,   164,   181,
     181,   130,    44,   278,   336,   179,    46,    47,    48,    49,
      50,    55,   161,   360,   151,   152,    26,   165,    27,   132,
     407,   151,   152,   362,   146,   279,   280,   364,    70,    38,
     366,    75,   398,   138,    56,   408,    32,   304,   119,   122,
     166,   281,   138,   380,    75,   303,   230,   231,   305,   180,
     383,   399,   138,   167,   249,    40,   138,    41,    31,   138,
     198,   130,   130,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   138,   151,   152,   104,   232,   208,   385,   138,
      42,   155,     1,   241,   233,   294,    29,   216,    58,   375,
     122,   162,   169,   170,    72,   308,   326,   318,   356,   273,
     203,   137,   138,   301,   113,   114,   309,   138,    33,     2,
      70,    39,   234,    30,    59,   200,   246,   247,   235,   130,
     130,   236,     3,     4,   225,   207,   310,    26,     5,    27,
     210,   237,   258,     6,    45,   299,   259,   311,   213,   258,
      51,   260,    66,   259,   261,     7,   151,   152,   260,    75,
      67,   261,   122,   255,   194,    52,   130,   262,    63,   203,
     330,    26,     8,    27,   262,   222,   133,   -95,    53,   298,
     223,   151,   152,   276,   328,     9,    54,   151,   152,   263,
      10,    38,    11,   322,    38,   115,   116,   153,   323,   156,
     157,   174,   175,   196,   138,    60,   342,   317,    75,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    61,   151,
     152,    62,   307,   296,   250,   251,    63,   363,   151,   152,
     271,   272,   286,   287,   331,    72,   319,   320,   283,   284,
     131,   130,    38,   285,   138,   321,   138,    75,   135,   216,
     207,   379,   139,   381,   117,   382,   140,   384,   345,   138,
     142,    75,    75,   130,   141,   358,   333,   143,   335,   144,
     337,   394,   339,   145,   395,   147,   396,   346,   138,   359,
     251,   372,   373,   148,   130,   393,   373,   402,   138,   409,
     138,    75,   150,    75,   171,    75,   361,    75,   181,   172,
     365,   182,   367,   388,   193,   201,   202,   199,   130,   218,
     130,   354,   130,   219,   130,   221,   357,   220,   226,   244,
     245,    75,   252,   253,   254,    75,   268,    75,   130,   270,
     277,   130,   388,   130,   274,   275,   290,   392,   289,   292,
     293,   376,   354,   295,   300,   302,   306,   314,   315,    26,
      76,    27,    77,   324,   327,   329,   334,   338,   390,   343,
     354,   347,    75,   406,   340,    78,    79,   214,   355,   368,
     369,   389,   401,   272,   378,   371,   404,    81,    82,   397,
     177,   297,   377,    83,    71,   197,   391,   136,    75,    84,
      85,    86,   209,    73,   313,   312,   267,    87,   403,   243,
     178,   242,    88,   212,   332,   160,   341,   211,   348,     0,
       0,    89,    90,    26,    76,    27,    77,     0,     0,    91,
      92,     0,     0,     0,     0,     0,     0,     0,    93,    78,
      79,    80,     0,     0,    94,     0,    95,    96,     0,     0,
       0,    81,    82,    97,     0,     0,    98,    83,     0,     0,
      99,   215,     0,    84,    85,    86,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,     0,     0,     0,
       0,     0,     0,    91,    92,     0,     0,    26,    76,    27,
      77,     0,    93,   123,     0,     0,     0,     0,    94,     0,
      95,    96,     0,    78,    79,     0,     0,    97,     0,     0,
      98,     0,     0,     0,    99,    81,    82,     0,     0,     0,
       0,    83,     0,     0,     0,     0,     0,    84,    85,    86,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    26,    76,    27,    77,     0,     0,    91,    92,     0,
       0,     0,     0,     0,     0,     0,    93,    78,    79,     0,
       0,     0,    94,     0,    95,    96,     0,     0,     0,    81,
      82,    97,     0,     0,    98,    83,     0,     0,   124,     0,
       0,    84,    85,    86,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    26,    76,    27,    77,     0,
       0,    91,    92,     0,     0,     0,     0,     0,     0,     0,
      93,    78,   159,     0,     0,     0,    94,     0,    95,    96,
       0,     0,     0,    81,    82,    97,     0,     0,    98,    83,
       0,     0,    99,     0,     0,    84,    85,    86,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    26,
      76,    27,    77,     0,     0,    91,    92,     0,     0,     0,
       0,     0,     0,     0,    93,    78,    79,     0,     0,     0,
      94,     0,    95,    96,     0,     0,     0,    81,    82,    97,
       0,     0,    98,    83,     0,     0,    99,     0,     0,    84,
      85,    86,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,     0,    76,     0,    77,     0,     0,    91,
      92,     0,     0,     0,     0,     0,     0,     0,    93,    78,
     159,     0,     0,     0,    94,     0,    95,    96,     0,     0,
       0,    81,    82,    97,     0,     0,    98,    83,     0,     0,
     124,     0,     0,    84,    85,    86,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,     0,     0,     0,
       0,     0,     0,    91,    92,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,    94,     0,
      95,    96,    81,    82,     0,     0,     0,    97,    83,     0,
      98,     0,     0,     0,    84,    85,    86,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,    89,   176,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,    94,
       0,    95,    96,     0,     0,     0,     0,     0,    97,     0,
       0,    98
};

static const yytype_int16 yycheck[] =
{
       3,   174,    57,   131,   113,     7,     9,    10,     3,     9,
       5,     6,    89,     3,   204,     5,    68,   207,    38,     7,
       7,    63,    25,    54,    77,   124,    29,    30,    31,    32,
      33,    27,    25,    77,    19,    20,     3,    57,     5,    67,
      55,    19,    20,    77,    99,    76,    77,    77,    51,    52,
      77,    54,    30,   106,    50,    70,    89,    68,    61,    62,
      80,    92,   106,    77,    67,   255,   175,     9,    79,   124,
      77,    49,   106,    93,    74,   101,   106,   103,    58,   106,
     135,   123,   124,     9,    10,    11,    12,    13,    14,    15,
      16,    17,   106,    19,    20,   150,    38,   149,    77,   106,
      86,   104,     1,   105,    46,   233,    62,   162,    26,   104,
     113,   104,   115,   116,   104,    68,   289,   105,   105,   218,
     105,   105,   106,   251,    39,    40,    79,   106,    67,    28,
     133,     0,    74,    89,    52,   138,   191,   192,    80,   181,
     182,    83,    41,    42,   172,   148,    68,     3,    47,     5,
     153,    93,    59,    52,    58,     8,    63,    79,   161,    59,
      87,    68,    96,    63,    71,    64,    19,    20,    68,   172,
     104,    71,   175,    29,   302,   106,   218,    84,    98,   105,
       8,     3,    81,     5,    84,    48,   106,    86,    29,   244,
      53,    19,    20,   221,   293,    94,   104,    19,    20,   106,
      99,   204,   101,    48,   207,    39,    40,    29,    53,    21,
      22,   105,   106,   105,   106,     4,   315,   272,   221,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    77,    19,
      20,   104,   260,   236,   105,   106,    98,   336,    19,    20,
     105,   106,    34,    35,   299,   104,   274,   275,   105,   106,
     104,   293,   255,   105,   106,   105,   106,   260,    10,   314,
     263,   360,     6,   362,    99,   364,     6,   366,   105,   106,
      66,   274,   275,   315,   104,   330,   304,     4,   306,   104,
     308,   380,   310,    66,   383,     4,   385,   105,   106,   105,
     106,   105,   106,    58,   336,   105,   106,   105,   106,   105,
     106,   304,   106,   306,   104,   308,   334,   310,     7,   104,
     338,     8,   340,   368,    18,     6,     6,   105,   360,   104,
     362,   324,   364,    69,   366,   104,   329,    69,    95,    17,
       4,   334,    96,   105,   105,   338,    60,   340,   380,   105,
       4,   383,   397,   385,   104,   104,    99,   375,   106,    74,
     104,   354,   355,    69,    74,   104,    68,    36,    61,     3,
       4,     5,     6,   104,    33,   104,    68,    68,   371,    78,
     373,     4,   375,   401,    68,    19,    20,    21,   104,    36,
      72,     6,   104,   106,   105,    83,    75,    31,    32,   106,
     122,   239,   355,    37,    52,   133,   373,    72,   401,    43,
      44,    45,   150,    53,   265,   263,   207,    51,   397,   182,
     123,   181,    56,   158,   302,   111,   314,   154,   323,    -1,
      -1,    65,    66,     3,     4,     5,     6,    -1,    -1,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    19,
      20,    21,    -1,    -1,    88,    -1,    90,    91,    -1,    -1,
      -1,    31,    32,    97,    -1,    -1,   100,    37,    -1,    -1,
     104,   105,    -1,    43,    44,    45,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    73,    74,    -1,    -1,     3,     4,     5,
       6,    -1,    82,     9,    -1,    -1,    -1,    -1,    88,    -1,
      90,    91,    -1,    19,    20,    -1,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,   104,    31,    32,    -1,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    -1,    -1,    43,    44,    45,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      66,     3,     4,     5,     6,    -1,    -1,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    19,    20,    -1,
      -1,    -1,    88,    -1,    90,    91,    -1,    -1,    -1,    31,
      32,    97,    -1,    -1,   100,    37,    -1,    -1,   104,    -1,
      -1,    43,    44,    45,    -1,    -1,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    66,     3,     4,     5,     6,    -1,
      -1,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    19,    20,    -1,    -1,    -1,    88,    -1,    90,    91,
      -1,    -1,    -1,    31,    32,    97,    -1,    -1,   100,    37,
      -1,    -1,   104,    -1,    -1,    43,    44,    45,    -1,    -1,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,     3,
       4,     5,     6,    -1,    -1,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    19,    20,    -1,    -1,    -1,
      88,    -1,    90,    91,    -1,    -1,    -1,    31,    32,    97,
      -1,    -1,   100,    37,    -1,    -1,   104,    -1,    -1,    43,
      44,    45,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    -1,     4,    -1,     6,    -1,    -1,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    19,
      20,    -1,    -1,    -1,    88,    -1,    90,    91,    -1,    -1,
      -1,    31,    32,    97,    -1,    -1,   100,    37,    -1,    -1,
     104,    -1,    -1,    43,    44,    45,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      90,    91,    31,    32,    -1,    -1,    -1,    97,    37,    -1,
     100,    -1,    -1,    -1,    43,    44,    45,    -1,    -1,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    -1,    -1,
      -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,   100
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,    28,    41,    42,    47,    52,    64,    81,    94,
      99,   101,   108,   109,   110,   111,   112,   113,   114,   133,
     134,   137,   138,   141,   142,    89,     3,     5,   187,    62,
      89,    58,    89,    67,   187,   143,   144,   157,   187,     0,
     101,   103,    86,   145,   187,    58,   187,   187,   187,   187,
     187,    87,   106,    29,   104,    27,    50,   146,    26,    52,
       4,    77,   104,    98,   167,   168,    96,   104,   139,   140,
     187,   144,   104,   154,   186,   187,     4,     6,    19,    20,
      21,    31,    32,    37,    43,    44,    45,    51,    56,    65,
      66,    73,    74,    82,    88,    90,    91,    97,   100,   104,
     117,   147,   148,   149,   173,   174,   175,   176,   177,   179,
     181,   183,   187,    39,    40,    39,    40,    99,   135,   187,
     115,   116,   187,     9,   104,   169,   170,   171,   172,   173,
     181,   104,   186,   106,   167,    10,   145,   105,   106,     6,
       6,   104,    66,     4,   104,    66,   173,     4,    58,   150,
     106,    19,    20,    29,   184,   187,    21,    22,   185,    20,
     176,    25,   104,   115,    38,    57,    80,    93,   121,   187,
     187,   104,   104,   124,   105,   106,    66,   117,   172,   169,
     173,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,   182,    18,   179,   180,   105,   140,   173,   105,
     187,     6,     6,   105,   154,   155,   158,   187,   167,   149,
     187,   174,   175,   187,    21,   105,   173,   178,   104,    69,
      69,   104,    48,    53,   136,   186,    95,   121,   122,   123,
     115,     9,    38,    46,    74,    80,    83,    93,   118,   119,
     120,   105,   170,   171,    17,     4,   173,   173,     9,    74,
     105,   106,    96,   105,   105,    29,   156,   157,    59,    63,
      68,    71,    84,   106,   151,   152,   153,   156,    60,   159,
     105,   105,   106,   169,   104,   104,   186,     4,    54,    76,
      77,    92,   188,   105,   106,   105,    34,    35,   131,   106,
      99,   125,    74,   104,   179,    69,   187,   118,   173,     8,
      74,   179,   104,   157,    68,    79,    68,   186,    68,    79,
      68,    79,   155,   153,    36,    61,   160,   173,   105,   186,
     186,   105,    48,    53,   104,   132,   121,    33,   169,   104,
       8,   173,   180,   186,    68,   186,    77,   186,    68,   186,
      68,   178,   169,    78,   161,   105,   105,     4,   188,   126,
     127,   128,   129,   130,   187,   104,   105,   187,   173,   105,
      77,   186,    77,   169,    77,   186,    77,   186,    36,    72,
     162,    83,   105,   106,     6,   104,   187,   126,   105,   169,
      77,   169,   169,    77,   169,    77,   163,   164,   173,     6,
     187,   127,   186,   105,   169,   169,   169,   106,    30,    49,
     165,   104,   105,   164,    75,   166,   186,    55,    70,   105
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   107,   108,   108,   108,   108,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   110,   111,   111,   111,
     111,   112,   113,   114,   115,   116,   116,   117,   117,   117,
     117,   117,   117,   117,   117,   117,   117,   117,   117,   117,
     117,   117,   117,   117,   117,   118,   118,   118,   118,   118,
     118,   118,   119,   119,   120,   120,   121,   121,   121,   121,
     122,   122,   123,   123,   124,   124,   125,   125,   126,   126,
     127,   127,   127,   128,   129,   130,   131,   131,   132,   132,
     133,   133,   134,   135,   135,   136,   136,   136,   136,   137,
     138,   139,   139,   140,   141,   142,   142,   143,   143,   144,
     145,   146,   146,   146,   147,   147,   148,   148,   149,   149,
     149,   150,   151,   151,   152,   152,   153,   153,   153,   153,
     153,   153,   153,   153,   154,   155,   155,   155,   156,   156,
     157,   157,   158,   158,   159,   159,   160,   160,   161,   161,
     162,   162,   163,   163,   164,   165,   165,   165,   166,   166,
     166,   167,   167,   168,   169,   169,   170,   170,   171,   171,
     172,   172,   172,   172,   172,   172,   172,   173,   173,   174,
     174,   175,   175,   176,   176,   176,   176,   177,   177,   177,
     178,   178,   179,   179,   179,   179,   179,   179,   179,   180,
     180,   181,   181,   182,   182,   182,   182,   182,   182,   183,
     184,   184,   185,   185,   186,   186,   187,   187,   188,   188,
     188,   188
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     6,     6,     6,
       6,     8,     9,     3,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     4,     4,     1,     2,     1,     2,     2,
       4,     5,     2,     1,     0,     1,     4,     5,    10,     4,
       3,     1,     0,     1,     0,     3,     0,     5,     1,     3,
       1,     1,     1,     2,     4,     2,     1,     1,     0,     3,
      10,     7,     5,     0,     4,     2,     2,     4,     4,     5,
       4,     3,     1,     3,     2,     0,     2,     1,     3,     3,
       9,     0,     1,     1,     1,     1,     1,     3,     3,     2,
       1,     3,     0,     1,     2,     1,     5,     4,     6,     5,
       6,     5,     6,     5,     3,     2,     2,     1,     1,     2,
       1,     4,     1,     3,     0,     3,     0,     2,     0,     3,
       0,     2,     1,     3,     3,     0,     1,     1,     0,     2,
       2,     0,     1,     2,     3,     1,     3,     1,     2,     1,
       5,     6,     4,     3,     3,     3,     3,     3,     1,     3,
       1,     2,     1,     1,     1,     1,     3,     3,     4,     4,
       1,     3,     1,     1,     2,     2,     1,     2,     2,     1,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     1
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
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1733 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1743 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1753 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_UNSIGNED_NUMVAL  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 1763 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 109: /* sql_statement  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1773 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 110: /* quit_statement  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 1783 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 111: /* alter_table_statement  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1793 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 112: /* create_table_statement  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 1803 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 113: /* create_index_statement  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1813 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 114: /* drop_table_statement  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 1823 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 115: /* column_def  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 1833 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 116: /* column_def_commalist  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 1843 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 117: /* data_type  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 1853 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 118: /* column_constraint_def  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 1863 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 119: /* column_constraint_def_list  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1873 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 120: /* opt_column_constraint_def_list  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1883 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 124: /* opt_column_list  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 1893 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 125: /* opt_block_properties  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 1903 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 126: /* key_value_list  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 1913 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 127: /* key_value  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 1923 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 128: /* key_string_value  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 1933 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 129: /* key_string_list  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 1943 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 130: /* key_integer_value  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 1953 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 131: /* index_type  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1963 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 132: /* opt_index_properties  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 1973 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 133: /* insert_statement  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 1983 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 134: /* copy_from_statement  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_statement_) != nullptr) {
    delete ((*yyvaluep).copy_from_statement_);
  }
}
#line 1993 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 135: /* opt_copy_from_params  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2003 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 136: /* copy_from_params  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2013 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 137: /* update_statement  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2023 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 138: /* delete_statement  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2033 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 139: /* assignment_list  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2043 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 140: /* assignment_item  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2053 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 141: /* select_statement  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_statement_) != nullptr) {
    delete ((*yyvaluep).select_statement_);
  }
}
#line 2063 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 142: /* opt_with_clause  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2073 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 143: /* with_list  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2083 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 144: /* with_list_element  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2093 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 145: /* select_query  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2103 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 147: /* selection  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2113 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 148: /* selection_item_commalist  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2123 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 149: /* selection_item  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2133 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 150: /* from_clause  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2143 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* subquery_expression  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2153 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* table_reference  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2163 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* table_reference_signature  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2173 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* table_reference_signature_primary  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2183 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* table_reference_commalist  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2193 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* opt_group_by_clause  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2203 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* opt_having_clause  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2213 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* opt_order_by_clause  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2223 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* opt_limit_clause  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2233 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 163: /* order_commalist  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2243 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 164: /* order_item  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2253 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 165: /* opt_order_direction  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2263 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 166: /* opt_nulls_first  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2273 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 167: /* opt_where_clause  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2283 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* where_clause  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2293 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* or_expression  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2303 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* and_expression  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2313 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* not_expression  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2323 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* predicate_expression_base  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2333 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* add_expression  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2343 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* multiply_expression  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2353 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* unary_expression  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2363 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* expression_base  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2373 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* function_call  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2383 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* expression_list  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2393 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* literal_value  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2403 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* literal_value_commalist  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2413 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* attribute_ref  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2423 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* comparison_operation  */
#line 480 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2429 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* unary_operation  */
#line 481 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2435 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* add_operation  */
#line 482 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2441 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* multiply_operation  */
#line 482 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2447 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* name_commalist  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 2457 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* any_name  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2467 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* boolean_value  */
#line 479 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2473 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 493 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2770 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 497 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2779 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 501 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    YYABORT;
  }
#line 2787 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 504 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 2796 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 511 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 2804 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 514 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].copy_from_statement_);
  }
#line 2812 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 517 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 2820 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 520 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 2828 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 523 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 2836 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 526 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 2844 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 529 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 2852 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 532 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 2860 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 535 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].select_statement_);
  }
#line 2868 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 538 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 2876 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 544 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 2884 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 550 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2896 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 557 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2907 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 563 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2919 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 570 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2931 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 579 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-7]).first_line, (yylsp[-7]).first_column, (yyvsp[-5].string_value_), (yyvsp[-3].attribute_definition_list_), (yyvsp[0].key_value_list_));
  }
#line 2939 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 584 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-6].string_value_);
    delete (yyvsp[-4].string_value_);
    delete (yyvsp[-3].string_list_);
    delete (yyvsp[-1].string_value_);
    delete (yyvsp[0].key_value_list_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-8]), yyscanner, "CREATE INDEX statement");
    YYERROR;
  }
#line 2954 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 596 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 2962 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 601 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 2970 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 606 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 2979 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 610 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 2988 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 616 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 2998 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 621 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3006 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 624 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3014 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 627 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3024 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 632 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3032 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 635 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3040 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 638 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3048 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 641 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3056 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 644 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3064 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 647 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3072 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 650 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3080 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 653 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3088 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 656 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3096 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 659 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3113 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 671 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3121 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 674 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3129 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 677 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3152 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 695 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3175 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 715 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3183 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 718 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3191 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 721 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3201 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 726 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3211 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 731 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3222 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 737 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3233 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 743 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3245 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 752 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3254 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 756 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3263 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 762 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3271 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 765 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3279 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 770 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3289 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 775 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3299 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 780 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3311 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 787 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3321 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 794 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3330 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 798 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3339 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 804 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 3347 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 807 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = $1; */
  }
#line 3355 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 812 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = nullptr;
  }
#line 3363 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 815 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    (yyval.string_list_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "list of column names in CREATE INDEX statement");
    YYERROR;
  }
#line 3374 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 823 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 3382 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 826 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 3390 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 831 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3399 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 835 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3408 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 841 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 3416 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 844 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 3424 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 847 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 3432 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 852 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 3440 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 857 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 3448 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 862 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 3462 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 873 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Bloom Filter Index");
    YYERROR;
  }
#line 3472 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 878 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "CSB Tree Index");
    YYERROR;
  }
#line 3482 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 885 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 3490 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 888 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 3498 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 895 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 3511 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 903 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsert((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 3519 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 908 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_statement_) = new quickstep::ParseStatementCopyFrom((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].copy_from_params_));
  }
#line 3527 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 913 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = nullptr;
  }
#line 3535 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 916 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-1].copy_from_params_);
  }
#line 3543 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 921 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3552 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 925 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3561 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 929 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3570 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 933 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3579 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 939 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 3587 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 944 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 3595 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 949 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3604 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 953 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3613 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 959 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 3621 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 965 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].select_query_), (yyvsp[-1].with_list_));
  }
#line 3629 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 970 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = nullptr;
  }
#line 3637 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 973 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 3645 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 978 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3654 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 982 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3663 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 988 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 3672 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 995 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].selection_), (yyvsp[-5].table_reference_list_), (yyvsp[-4].predicate_), (yyvsp[-3].opt_group_by_clause_), (yyvsp[-2].opt_having_clause_), (yyvsp[-1].opt_order_by_clause_), (yyvsp[0].opt_limit_clause_));
  }
#line 3680 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 1000 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 3688 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 1003 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "ALL in selection");
    YYERROR;
  }
#line 3697 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 1007 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "DISTINCT in selection");
    YYERROR;
  }
#line 3706 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 1013 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3714 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 1016 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 3722 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 1021 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3731 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 1025 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3740 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 1031 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 3748 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 1034 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 3756 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 1037 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 3764 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 1042 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = (yyvsp[-1].table_reference_list_);
  }
#line 3772 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 1047 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 3780 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 1050 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3789 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 1056 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[-1]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3798 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 1060 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3807 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 1066 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3818 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 1072 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3829 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 1078 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3840 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 1084 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3851 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 1090 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3862 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 1096 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3873 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 1102 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3884 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 1108 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3895 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 1116 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].select_query_));
  }
#line 3903 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 1121 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 3912 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 1125 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 3921 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 1129 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 3929 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 1134 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 3937 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 1137 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 3945 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 1142 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 3953 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 1145 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 3961 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 1150 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 3970 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 1154 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 3979 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 1160 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 3987 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 1163 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 3995 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 1168 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4003 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 1171 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4011 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 1176 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4019 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 1179 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4027 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 1184 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4035 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 1187 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4057 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 1206 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4066 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 1210 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4075 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 1216 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 4085 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 1223 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4093 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 1226 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4101 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 1229 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4109 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 148:
#line 1234 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4117 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 149:
#line 1237 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4125 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 150:
#line 1240 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4133 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 1246 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 4141 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 1249 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4149 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 1254 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4157 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 1259 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4171 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 1268 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4179 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 1273 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4193 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 1282 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4201 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 158:
#line 1287 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4209 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 159:
#line 1290 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4217 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 1295 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4225 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 161:
#line 1298 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 4235 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 1303 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4246 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 163:
#line 1309 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4257 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 1315 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].expression_);
    delete (yyvsp[0].string_value_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "LIKE predicates");
    YYERROR;
  }
#line 4269 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 165:
#line 1322 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4277 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 166:
#line 1325 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 4285 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 1332 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4293 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 168:
#line 1335 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4301 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 169:
#line 1340 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4309 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 1343 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4317 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 171:
#line 1348 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].unary_operation_), (yyvsp[0].expression_));
  }
#line 4325 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 172:
#line 1351 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4333 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 173:
#line 1356 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 4341 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 174:
#line 1359 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 4349 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 175:
#line 1362 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 4357 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 176:
#line 1365 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 4365 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 177:
#line 1370 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 4374 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 178:
#line 1374 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 4383 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 1378 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4391 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 180:
#line 1383 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4400 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 181:
#line 1387 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4409 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 1393 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4417 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 183:
#line 1396 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4425 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 184:
#line 1399 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4433 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 185:
#line 1402 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4450 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 186:
#line 1414 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 4459 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 187:
#line 1418 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4483 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 1437 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4501 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 189:
#line 1452 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4510 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 190:
#line 1456 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4519 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 191:
#line 1462 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4527 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 192:
#line 1465 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 4535 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 193:
#line 1471 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 4543 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 194:
#line 1474 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 4551 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 195:
#line 1477 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 4559 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 196:
#line 1480 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 4567 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 197:
#line 1483 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 4575 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 198:
#line 1486 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 4583 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 199:
#line 1491 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
#line 4597 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 200:
#line 1502 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
#line 4605 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 201:
#line 1505 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
#line 4613 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 202:
#line 1510 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
#line 4621 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 203:
#line 1513 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
#line 4629 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 204:
#line 1519 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4638 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 205:
#line 1523 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4647 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 206:
#line 1529 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 4655 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 1532 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 4666 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 208:
#line 1540 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4674 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 209:
#line 1543 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4682 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 210:
#line 1546 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;
  }
#line 4690 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 211:
#line 1549 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;
  }
#line 4698 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;


#line 4702 "SqlParser_gen.cpp" /* yacc.c:1646  */
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
#line 1553 "../SqlParser.ypp" /* yacc.c:1906  */


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
