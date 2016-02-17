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
    TOKEN_GB = 311,
    TOKEN_HAVING = 312,
    TOKEN_INDEX = 313,
    TOKEN_INNER = 314,
    TOKEN_INSERT = 315,
    TOKEN_INTEGER = 316,
    TOKEN_INTERVAL = 317,
    TOKEN_INTO = 318,
    TOKEN_JOIN = 319,
    TOKEN_KEY = 320,
    TOKEN_LAST = 321,
    TOKEN_LEFT = 322,
    TOKEN_LIMIT = 323,
    TOKEN_LONG = 324,
    TOKEN_MB = 325,
    TOKEN_NULL = 326,
    TOKEN_NULLS = 327,
    TOKEN_OFF = 328,
    TOKEN_ON = 329,
    TOKEN_ORDER = 330,
    TOKEN_OUTER = 331,
    TOKEN_PRIMARY = 332,
    TOKEN_QUIT = 333,
    TOKEN_REAL = 334,
    TOKEN_REFERENCES = 335,
    TOKEN_RIGHT = 336,
    TOKEN_ROW_DELIMITER = 337,
    TOKEN_SELECT = 338,
    TOKEN_SET = 339,
    TOKEN_SMALLINT = 340,
    TOKEN_TABLE = 341,
    TOKEN_TIME = 342,
    TOKEN_TIMESTAMP = 343,
    TOKEN_TRUE = 344,
    TOKEN_UNIQUE = 345,
    TOKEN_UPDATE = 346,
    TOKEN_USING = 347,
    TOKEN_VALUES = 348,
    TOKEN_VARCHAR = 349,
    TOKEN_WHERE = 350,
    TOKEN_WITH = 351,
    TOKEN_YEARMONTH = 352,
    TOKEN_EOF = 353,
    TOKEN_LEX_ERROR = 354
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

#line 360 "SqlParser_gen.cpp" /* yacc.c:355  */
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

#line 393 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYLAST   871

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  109
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  82
/* YYNRULES -- Number of rules.  */
#define YYNRULES  212
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  411

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   354

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
     106,   107,    21,    19,   108,    20,    25,    22,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   105,
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
     100,   101,   102,   103,   104
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   495,   495,   499,   503,   506,   513,   516,   519,   522,
     525,   528,   531,   534,   537,   540,   546,   552,   559,   565,
     572,   581,   586,   598,   603,   608,   612,   618,   623,   626,
     629,   634,   637,   640,   643,   646,   649,   652,   655,   658,
     661,   673,   676,   679,   697,   717,   720,   723,   728,   733,
     739,   745,   754,   758,   764,   767,   772,   777,   782,   789,
     796,   800,   806,   809,   814,   817,   825,   828,   833,   837,
     843,   846,   849,   854,   857,   864,   869,   880,   885,   892,
     895,   901,   909,   914,   919,   922,   927,   931,   935,   939,
     945,   950,   955,   959,   965,   971,   976,   979,   984,   988,
     994,  1000,  1006,  1009,  1013,  1019,  1022,  1027,  1031,  1037,
    1040,  1043,  1048,  1053,  1056,  1062,  1066,  1072,  1078,  1084,
    1090,  1096,  1102,  1108,  1114,  1122,  1127,  1131,  1135,  1140,
    1143,  1148,  1151,  1156,  1160,  1166,  1169,  1174,  1177,  1182,
    1185,  1190,  1193,  1212,  1216,  1222,  1229,  1232,  1235,  1240,
    1243,  1246,  1252,  1255,  1260,  1265,  1274,  1279,  1288,  1293,
    1296,  1301,  1304,  1309,  1315,  1321,  1328,  1331,  1338,  1341,
    1346,  1349,  1354,  1357,  1362,  1365,  1368,  1371,  1376,  1380,
    1384,  1389,  1393,  1399,  1402,  1405,  1408,  1420,  1424,  1443,
    1458,  1462,  1468,  1471,  1477,  1480,  1483,  1486,  1489,  1492,
    1497,  1508,  1511,  1516,  1519,  1525,  1529,  1535,  1538,  1546,
    1549,  1552,  1555
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
  "TOKEN_FULL", "TOKEN_GROUP", "TOKEN_GB", "TOKEN_HAVING", "TOKEN_INDEX",
  "TOKEN_INNER", "TOKEN_INSERT", "TOKEN_INTEGER", "TOKEN_INTERVAL",
  "TOKEN_INTO", "TOKEN_JOIN", "TOKEN_KEY", "TOKEN_LAST", "TOKEN_LEFT",
  "TOKEN_LIMIT", "TOKEN_LONG", "TOKEN_MB", "TOKEN_NULL", "TOKEN_NULLS",
  "TOKEN_OFF", "TOKEN_ON", "TOKEN_ORDER", "TOKEN_OUTER", "TOKEN_PRIMARY",
  "TOKEN_QUIT", "TOKEN_REAL", "TOKEN_REFERENCES", "TOKEN_RIGHT",
  "TOKEN_ROW_DELIMITER", "TOKEN_SELECT", "TOKEN_SET", "TOKEN_SMALLINT",
  "TOKEN_TABLE", "TOKEN_TIME", "TOKEN_TIMESTAMP", "TOKEN_TRUE",
  "TOKEN_UNIQUE", "TOKEN_UPDATE", "TOKEN_USING", "TOKEN_VALUES",
  "TOKEN_VARCHAR", "TOKEN_WHERE", "TOKEN_WITH", "TOKEN_YEARMONTH",
  "TOKEN_EOF", "TOKEN_LEX_ERROR", "';'", "'('", "')'", "','", "$accept",
  "start", "sql_statement", "quit_statement", "alter_table_statement",
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
     350,   351,   352,   353,   354,    59,    40,    41,    44
};
# endif

#define YYPACT_NINF -185

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-185)))

#define YYTABLE_NINF -97

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      51,  -185,   -67,   124,   -13,   -16,   -45,   -15,  -185,   124,
     124,  -185,    87,    50,  -185,  -185,  -185,  -185,  -185,  -185,
    -185,  -185,  -185,  -185,     3,   124,  -185,  -185,    75,   124,
     124,   124,   124,   124,    20,    38,  -185,   120,    53,  -185,
    -185,  -185,    54,  -185,    31,   153,   111,    78,   130,  -185,
     -73,   124,   124,   110,   124,  -185,  -185,   401,    86,   137,
     155,   124,   124,   467,  -185,  -185,   115,   124,     0,  -185,
     255,  -185,     3,  -185,   100,  -185,  -185,  -185,   261,   262,
    -185,  -185,  -185,   163,  -185,   203,  -185,  -185,  -185,  -185,
     269,  -185,  -185,  -185,  -185,  -185,  -185,   171,   212,   533,
     277,   225,   177,  -185,   176,   188,  -185,  -185,  -185,  -185,
    -185,   599,   -21,   124,   -19,   124,   124,   178,  -185,   180,
    -185,   105,   769,   665,   467,   280,   281,  -185,  -185,   835,
     270,   731,   107,   124,  -185,   533,   184,  -185,   124,  -185,
    -185,   287,  -185,  -185,   289,  -185,    15,  -185,     9,   130,
     533,  -185,  -185,   124,   533,  -185,  -185,  -185,   533,   262,
    -185,   124,   335,  -185,   191,   229,   238,   209,  -185,  -185,
    -185,   144,   124,   220,   -19,   124,  -185,    56,  -185,    10,
     151,   467,   467,   292,  -185,  -185,  -185,  -185,  -185,  -185,
     314,   533,   533,     6,  -185,   112,   222,  -185,   205,  -185,
    -185,   216,   218,  -185,    85,  -185,   114,    85,   267,  -185,
    -185,   188,  -185,  -185,   221,  -185,   205,   119,   467,   223,
     224,   124,   328,   -38,   121,   127,   202,  -185,   226,   232,
    -185,   260,   236,   731,  -185,   274,   124,  -185,  -185,    56,
    -185,  -185,   281,  -185,   533,  -185,   123,   205,   271,  -185,
    -185,   731,   239,  -185,  -185,   124,  -185,  -185,    59,   284,
     124,   113,   122,     9,  -185,   116,  -185,  -185,   310,   288,
    -185,  -185,   533,    11,   124,   124,   131,  -185,  -185,  -185,
    -185,  -185,  -185,  -185,   158,  -185,  -185,  -185,   243,   -19,
     324,  -185,  -185,   467,  -185,  -185,   252,  -185,   179,   533,
    -185,  -185,   731,  -185,   124,   290,   124,   -48,   124,   291,
     124,   293,  -185,  -185,   533,   467,   283,   205,  -185,   133,
     135,  -185,   357,   -38,   124,  -185,  -185,   258,    14,   124,
     533,   205,   141,   -36,   124,   -34,   467,   -24,   124,   -12,
     124,   257,   280,   333,   298,   296,  -185,  -185,  -185,   143,
    -185,  -185,  -185,  -185,     5,   124,  -185,   268,   205,  -185,
     467,   -11,   467,   280,   467,    -9,   467,    -2,   533,   370,
    -185,   124,  -185,   124,  -185,  -185,   124,  -185,   147,  -185,
     280,   467,   280,   280,   467,   280,   467,   275,  -185,    17,
    -185,   276,  -185,   154,  -185,   280,   280,   280,   533,  -185,
    -185,   300,   124,  -185,  -185,    18,  -185,   156,  -185,  -185,
    -185
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     4,     0,     0,     0,     0,     0,     0,    16,     0,
       0,     5,     0,     0,    13,     6,     8,     9,    11,    12,
       7,    15,    10,    14,     0,     0,   207,   208,     0,     0,
       0,     0,     0,     0,     0,    97,    98,     0,   131,     1,
       3,     2,   102,    95,     0,     0,     0,     0,   152,    23,
       0,     0,     0,     0,     0,   103,   104,     0,     0,     0,
      84,     0,     0,     0,    91,   153,     0,     0,   152,    93,
       0,    99,     0,   100,     0,   205,   187,   184,     0,   200,
     105,    38,    27,     0,    28,    29,    32,    34,    35,    37,
       0,    39,   183,    33,    36,    30,    31,     0,     0,     0,
       0,     0,   106,   107,   111,   169,   171,   173,   176,   175,
     174,     0,   192,     0,     0,     0,     0,     0,    83,    64,
      25,     0,     0,     0,     0,   154,   156,   158,   160,     0,
     174,     0,     0,     0,    90,     0,     0,   132,     0,   185,
     186,     0,    41,   188,     0,    42,     0,   189,     0,   152,
       0,   201,   202,     0,     0,   110,   203,   204,     0,     0,
     172,     0,     0,    17,     0,     0,     0,     0,    18,    19,
      20,     0,     0,     0,    62,     0,    40,    54,   159,     0,
       0,     0,     0,     0,   194,   196,   197,   198,   199,   195,
       0,     0,     0,     0,   190,     0,     0,    92,    94,   125,
     206,     0,     0,   177,     0,   133,   113,   128,   135,   108,
     109,   168,   170,   193,     0,   178,   181,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,    63,    66,
      26,     0,     0,     0,    45,     0,     0,    47,    53,    55,
      24,   167,   155,   157,     0,   165,     0,   166,     0,   164,
      82,     0,     0,    43,    44,     0,   126,   129,     0,     0,
       0,     0,     0,     0,   112,   114,   116,   127,     0,   137,
     179,   180,     0,     0,     0,     0,     0,    86,   211,   212,
     210,   209,    87,    85,     0,    65,    77,    78,    79,     0,
       0,    21,    46,     0,    49,    48,     0,    52,     0,     0,
     163,   191,     0,   130,     0,     0,     0,     0,     0,     0,
       0,     0,   134,   115,     0,     0,   139,   182,    59,     0,
       0,    56,     0,     0,     0,    22,    60,     0,     0,     0,
       0,   161,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   136,   138,     0,   141,     0,    57,    88,    89,     0,
      68,    70,    71,    72,     0,     0,    50,     0,   162,    81,
       0,     0,     0,   118,     0,     0,     0,     0,     0,     0,
     101,     0,    80,     0,    76,    74,     0,    73,     0,    51,
     124,     0,   117,   120,     0,   122,     0,   140,   143,   146,
     142,     0,    69,     0,    67,   123,   119,   121,     0,   147,
     148,   149,     0,    75,   144,     0,   145,     0,   150,   151,
      58
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -185,  -185,  -185,  -185,  -185,  -185,  -185,  -185,  -100,  -185,
     263,   145,  -185,  -185,  -165,  -185,  -185,  -185,  -185,    32,
      16,  -185,  -185,  -185,  -185,  -185,  -185,  -185,  -185,  -185,
    -185,  -185,  -185,   259,  -185,  -185,  -185,   336,   318,  -185,
    -185,  -185,   244,  -185,  -185,  -185,   128,   342,   134,   189,
    -184,  -185,  -185,  -185,  -185,  -185,  -185,     2,  -185,  -185,
     -63,  -185,   -70,   217,   228,   285,   -55,   249,   254,   302,
    -185,   101,  -128,   129,   -62,  -185,  -185,  -185,  -185,   -28,
      -3,    91
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    12,    13,    14,    15,    16,    17,    18,   120,   121,
     100,   238,   239,   240,   168,   228,   229,   173,   291,   349,
     350,   351,   352,   353,   288,   325,    19,    20,   118,   224,
      21,    22,    68,    69,    23,    24,    35,    36,    43,    57,
     101,   102,   103,   149,   264,   265,   266,   204,   205,   256,
      37,   206,   269,   316,   344,   370,   387,   388,   401,   406,
      64,    65,   125,   126,   127,   128,   129,   105,   106,   107,
     108,   217,   109,   195,   110,   192,   111,   154,   158,    74,
     112,   282
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      28,   130,   104,   194,   161,   134,    34,    38,    26,   227,
      27,   374,    26,   163,    27,   248,   278,   181,   181,   164,
     257,   181,    44,   257,    25,    66,    46,    47,    48,    49,
      50,   336,   375,    67,   151,   152,   151,   152,   165,   132,
     279,   280,    31,   360,   146,   362,    32,   399,    70,    38,
      29,    75,     1,    33,   179,   364,   281,    58,   119,   122,
     138,   130,   130,   166,    75,   231,   400,   366,   381,   180,
     384,   303,   138,   408,   138,   230,   167,   386,    30,     2,
     198,    55,   249,    59,   138,   162,   208,    39,    26,   409,
      27,    42,     3,     4,   232,   104,   138,   138,     5,   138,
      63,   155,   233,     6,    56,   294,   138,   216,   133,    51,
     122,   376,   169,   170,   255,    72,     7,   241,   318,   130,
     130,   356,   203,   301,   326,   113,   114,    26,   304,    27,
      70,   299,   234,    45,     8,   200,   246,   247,   235,   -96,
     305,   236,   151,   152,   225,   207,    52,     9,   273,    53,
     210,   237,    10,    40,    11,    41,   130,    60,   213,    54,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    75,
     151,   152,   122,   258,   194,   258,   115,   116,   259,    26,
     259,    27,   308,   260,    62,   260,   261,   330,   261,   298,
      61,   310,   222,   276,   309,   151,   152,   223,   151,   152,
     262,    38,   262,   311,    38,   153,   322,   137,   138,   156,
     157,   323,   174,   175,   196,   138,    72,   317,    75,   250,
     251,   131,   263,   328,   151,   152,   271,   272,   283,   284,
      63,   130,   307,   296,   285,   138,   286,   287,   321,   138,
     345,   138,   346,   138,   331,   342,   319,   320,   359,   251,
     372,   373,    38,   130,   394,   373,   117,    75,   203,   216,
     207,   403,   138,   410,   138,   135,   363,   139,   140,   141,
     142,    75,    75,   143,   130,   358,   333,   144,   335,   145,
     337,   147,   339,   148,   171,   150,   172,   181,   193,   182,
     380,   199,   382,   201,   383,   202,   385,   218,   130,   219,
     130,    75,   130,    75,   130,    75,   361,    75,   220,   244,
     365,   395,   367,   389,   396,   221,   397,   226,   245,   130,
     252,   354,   130,   253,   130,   254,   357,   268,   270,   274,
     275,    75,   277,   290,   289,    75,   292,    75,    26,    76,
      27,    77,   293,   389,   295,   302,   314,   300,   393,   324,
     315,   377,   354,   306,    78,    79,   214,   327,   329,   334,
     338,   347,   340,   343,   355,   272,    81,    82,   391,   368,
     354,   369,    83,    75,   407,   379,   390,   405,    84,    85,
      86,   371,   402,   398,   297,   177,    87,   378,    71,   392,
     136,    88,   197,   313,   209,    73,   267,   312,   242,    75,
     404,    89,    90,   211,    26,    76,    27,    77,   178,    91,
     243,    92,   212,   160,   348,   341,     0,     0,     0,    93,
      78,    79,    80,     0,     0,    94,     0,    95,    96,     0,
       0,   332,    81,    82,    97,     0,     0,    98,    83,     0,
       0,    99,   215,     0,    84,    85,    86,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,     0,
      26,    76,    27,    77,     0,    91,   123,    92,     0,     0,
       0,     0,     0,     0,     0,    93,    78,    79,     0,     0,
       0,    94,     0,    95,    96,     0,     0,     0,    81,    82,
      97,     0,     0,    98,    83,     0,     0,    99,     0,     0,
      84,    85,    86,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,     0,    26,    76,    27,    77,
       0,    91,     0,    92,     0,     0,     0,     0,     0,     0,
       0,    93,    78,    79,     0,     0,     0,    94,     0,    95,
      96,     0,     0,     0,    81,    82,    97,     0,     0,    98,
      83,     0,     0,   124,     0,     0,    84,    85,    86,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,     0,    26,    76,    27,    77,     0,    91,     0,    92,
       0,     0,     0,     0,     0,     0,     0,    93,    78,   159,
       0,     0,     0,    94,     0,    95,    96,     0,     0,     0,
      81,    82,    97,     0,     0,    98,    83,     0,     0,    99,
       0,     0,    84,    85,    86,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,     0,    26,    76,
      27,    77,     0,    91,     0,    92,     0,     0,     0,     0,
       0,     0,     0,    93,    78,    79,     0,     0,     0,    94,
       0,    95,    96,     0,     0,     0,    81,    82,    97,     0,
       0,    98,    83,     0,     0,    99,     0,     0,    84,    85,
      86,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,     0,     0,    76,     0,    77,     0,    91,
       0,    92,     0,     0,     0,     0,     0,     0,     0,    93,
      78,   159,     0,     0,     0,    94,     0,    95,    96,     0,
       0,     0,    81,    82,    97,     0,     0,    98,    83,     0,
       0,   124,     0,     0,    84,    85,    86,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,     0,
      81,    82,     0,     0,     0,    91,    83,    92,     0,     0,
       0,     0,    84,    85,    86,    93,     0,     0,     0,     0,
      87,    94,     0,    95,    96,    88,     0,     0,     0,     0,
      97,     0,     0,    98,     0,    89,   176,     0,     0,     0,
       0,     0,     0,    91,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    93,   151,   152,     0,     0,     0,    94,
       0,    95,    96,     0,     0,     0,     0,     0,    97,     0,
       0,    98
};

static const yytype_int16 yycheck[] =
{
       3,    63,    57,   131,    25,    68,     9,    10,     3,   174,
       5,     6,     3,   113,     5,     9,    54,     7,     7,    38,
     204,     7,    25,   207,    91,    98,    29,    30,    31,    32,
      33,    79,    27,   106,    19,    20,    19,    20,    57,    67,
      78,    79,    58,    79,    99,    79,    91,    30,    51,    52,
      63,    54,     1,    68,   124,    79,    94,    26,    61,    62,
     108,   123,   124,    82,    67,     9,    49,    79,    79,   124,
      79,   255,   108,    55,   108,   175,    95,    79,    91,    28,
     135,    27,    76,    52,   108,   106,   149,     0,     3,    71,
       5,    88,    41,    42,    38,   150,   108,   108,    47,   108,
     100,   104,    46,    52,    50,   233,   108,   162,   108,    89,
     113,   106,   115,   116,    29,   106,    65,   107,   107,   181,
     182,   107,   107,   251,   289,    39,    40,     3,    69,     5,
     133,     8,    76,    58,    83,   138,   191,   192,    82,    88,
      81,    85,    19,    20,   172,   148,   108,    96,   218,    29,
     153,    95,   101,   103,   103,   105,   218,     4,   161,   106,
       9,    10,    11,    12,    13,    14,    15,    16,    17,   172,
      19,    20,   175,    59,   302,    59,    39,    40,    64,     3,
      64,     5,    69,    69,   106,    69,    72,     8,    72,   244,
      79,    69,    48,   221,    81,    19,    20,    53,    19,    20,
      86,   204,    86,    81,   207,    29,    48,   107,   108,    21,
      22,    53,   107,   108,   107,   108,   106,   272,   221,   107,
     108,   106,   108,   293,    19,    20,   107,   108,   107,   108,
     100,   293,   260,   236,   107,   108,    34,    35,   107,   108,
     107,   108,   107,   108,   299,   315,   274,   275,   107,   108,
     107,   108,   255,   315,   107,   108,   101,   260,   107,   314,
     263,   107,   108,   107,   108,    10,   336,     6,     6,   106,
      67,   274,   275,     4,   336,   330,   304,   106,   306,    67,
     308,     4,   310,    58,   106,   108,   106,     7,    18,     8,
     360,   107,   362,     6,   364,     6,   366,   106,   360,    70,
     362,   304,   364,   306,   366,   308,   334,   310,    70,    17,
     338,   381,   340,   368,   384,   106,   386,    97,     4,   381,
      98,   324,   384,   107,   386,   107,   329,    60,   107,   106,
     106,   334,     4,   101,   108,   338,    76,   340,     3,     4,
       5,     6,   106,   398,    70,   106,    36,    76,   376,   106,
      62,   354,   355,    69,    19,    20,    21,    33,   106,    69,
      69,     4,    69,    80,   106,   108,    31,    32,   371,    36,
     373,    73,    37,   376,   402,   107,     6,    77,    43,    44,
      45,    85,   106,   108,   239,   122,    51,   355,    52,   373,
      72,    56,   133,   265,   150,    53,   207,   263,   181,   402,
     398,    66,    67,   154,     3,     4,     5,     6,   123,    74,
     182,    76,   158,   111,   323,   314,    -1,    -1,    -1,    84,
      19,    20,    21,    -1,    -1,    90,    -1,    92,    93,    -1,
      -1,   302,    31,    32,    99,    -1,    -1,   102,    37,    -1,
      -1,   106,   107,    -1,    43,    44,    45,    -1,    -1,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    -1,
       3,     4,     5,     6,    -1,    74,     9,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    19,    20,    -1,    -1,
      -1,    90,    -1,    92,    93,    -1,    -1,    -1,    31,    32,
      99,    -1,    -1,   102,    37,    -1,    -1,   106,    -1,    -1,
      43,    44,    45,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    67,    -1,     3,     4,     5,     6,
      -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    19,    20,    -1,    -1,    -1,    90,    -1,    92,
      93,    -1,    -1,    -1,    31,    32,    99,    -1,    -1,   102,
      37,    -1,    -1,   106,    -1,    -1,    43,    44,    45,    -1,
      -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      67,    -1,     3,     4,     5,     6,    -1,    74,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    19,    20,
      -1,    -1,    -1,    90,    -1,    92,    93,    -1,    -1,    -1,
      31,    32,    99,    -1,    -1,   102,    37,    -1,    -1,   106,
      -1,    -1,    43,    44,    45,    -1,    -1,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    -1,     3,     4,
       5,     6,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    19,    20,    -1,    -1,    -1,    90,
      -1,    92,    93,    -1,    -1,    -1,    31,    32,    99,    -1,
      -1,   102,    37,    -1,    -1,   106,    -1,    -1,    43,    44,
      45,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,     4,    -1,     6,    -1,    74,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      19,    20,    -1,    -1,    -1,    90,    -1,    92,    93,    -1,
      -1,    -1,    31,    32,    99,    -1,    -1,   102,    37,    -1,
      -1,   106,    -1,    -1,    43,    44,    45,    -1,    -1,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    -1,
      31,    32,    -1,    -1,    -1,    74,    37,    76,    -1,    -1,
      -1,    -1,    43,    44,    45,    84,    -1,    -1,    -1,    -1,
      51,    90,    -1,    92,    93,    56,    -1,    -1,    -1,    -1,
      99,    -1,    -1,   102,    -1,    66,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    74,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    84,    19,    20,    -1,    -1,    -1,    90,
      -1,    92,    93,    -1,    -1,    -1,    -1,    -1,    99,    -1,
      -1,   102
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,    28,    41,    42,    47,    52,    65,    83,    96,
     101,   103,   110,   111,   112,   113,   114,   115,   116,   135,
     136,   139,   140,   143,   144,    91,     3,     5,   189,    63,
      91,    58,    91,    68,   189,   145,   146,   159,   189,     0,
     103,   105,    88,   147,   189,    58,   189,   189,   189,   189,
     189,    89,   108,    29,   106,    27,    50,   148,    26,    52,
       4,    79,   106,   100,   169,   170,    98,   106,   141,   142,
     189,   146,   106,   156,   188,   189,     4,     6,    19,    20,
      21,    31,    32,    37,    43,    44,    45,    51,    56,    66,
      67,    74,    76,    84,    90,    92,    93,    99,   102,   106,
     119,   149,   150,   151,   175,   176,   177,   178,   179,   181,
     183,   185,   189,    39,    40,    39,    40,   101,   137,   189,
     117,   118,   189,     9,   106,   171,   172,   173,   174,   175,
     183,   106,   188,   108,   169,    10,   147,   107,   108,     6,
       6,   106,    67,     4,   106,    67,   175,     4,    58,   152,
     108,    19,    20,    29,   186,   189,    21,    22,   187,    20,
     178,    25,   106,   117,    38,    57,    82,    95,   123,   189,
     189,   106,   106,   126,   107,   108,    67,   119,   174,   171,
     175,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,   184,    18,   181,   182,   107,   142,   175,   107,
     189,     6,     6,   107,   156,   157,   160,   189,   169,   151,
     189,   176,   177,   189,    21,   107,   175,   180,   106,    70,
      70,   106,    48,    53,   138,   188,    97,   123,   124,   125,
     117,     9,    38,    46,    76,    82,    85,    95,   120,   121,
     122,   107,   172,   173,    17,     4,   175,   175,     9,    76,
     107,   108,    98,   107,   107,    29,   158,   159,    59,    64,
      69,    72,    86,   108,   153,   154,   155,   158,    60,   161,
     107,   107,   108,   171,   106,   106,   188,     4,    54,    78,
      79,    94,   190,   107,   108,   107,    34,    35,   133,   108,
     101,   127,    76,   106,   181,    70,   189,   120,   175,     8,
      76,   181,   106,   159,    69,    81,    69,   188,    69,    81,
      69,    81,   157,   155,    36,    62,   162,   175,   107,   188,
     188,   107,    48,    53,   106,   134,   123,    33,   171,   106,
       8,   175,   182,   188,    69,   188,    79,   188,    69,   188,
      69,   180,   171,    80,   163,   107,   107,     4,   190,   128,
     129,   130,   131,   132,   189,   106,   107,   189,   175,   107,
      79,   188,    79,   171,    79,   188,    79,   188,    36,    73,
     164,    85,   107,   108,     6,    27,   106,   189,   128,   107,
     171,    79,   171,   171,    79,   171,    79,   165,   166,   175,
       6,   189,   129,   188,   107,   171,   171,   171,   108,    30,
      49,   167,   106,   107,   166,    77,   168,   188,    55,    71,
     107
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   109,   110,   110,   110,   110,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   112,   113,   113,   113,
     113,   114,   115,   116,   117,   118,   118,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   120,   120,   120,   120,   120,
     120,   120,   121,   121,   122,   122,   123,   123,   123,   123,
     124,   124,   125,   125,   126,   126,   127,   127,   128,   128,
     129,   129,   129,   130,   130,   131,   132,   133,   133,   134,
     134,   135,   135,   136,   137,   137,   138,   138,   138,   138,
     139,   140,   141,   141,   142,   143,   144,   144,   145,   145,
     146,   147,   148,   148,   148,   149,   149,   150,   150,   151,
     151,   151,   152,   153,   153,   154,   154,   155,   155,   155,
     155,   155,   155,   155,   155,   156,   157,   157,   157,   158,
     158,   159,   159,   160,   160,   161,   161,   162,   162,   163,
     163,   164,   164,   165,   165,   166,   167,   167,   167,   168,
     168,   168,   169,   169,   170,   171,   171,   172,   172,   173,
     173,   174,   174,   174,   174,   174,   174,   174,   175,   175,
     176,   176,   177,   177,   178,   178,   178,   178,   179,   179,
     179,   180,   180,   181,   181,   181,   181,   181,   181,   181,
     182,   182,   183,   183,   184,   184,   184,   184,   184,   184,
     185,   186,   186,   187,   187,   188,   188,   189,   189,   190,
     190,   190,   190
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
       1,     1,     1,     2,     2,     4,     2,     1,     1,     0,
       3,    10,     7,     5,     0,     4,     2,     2,     4,     4,
       5,     4,     3,     1,     3,     2,     0,     2,     1,     3,
       3,     9,     0,     1,     1,     1,     1,     1,     3,     3,
       2,     1,     3,     0,     1,     2,     1,     5,     4,     6,
       5,     6,     5,     6,     5,     3,     2,     2,     1,     1,
       2,     1,     4,     1,     3,     0,     3,     0,     2,     0,
       3,     0,     2,     1,     3,     3,     0,     1,     1,     0,
       2,     2,     0,     1,     2,     3,     1,     3,     1,     2,
       1,     5,     6,     4,     3,     3,     3,     3,     3,     1,
       3,     1,     2,     1,     1,     1,     1,     3,     3,     4,
       4,     1,     3,     1,     1,     2,     2,     1,     2,     2,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     1
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
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1730 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1740 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1750 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_UNSIGNED_NUMVAL  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 1760 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 111: /* sql_statement  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1770 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 112: /* quit_statement  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 1780 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 113: /* alter_table_statement  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1790 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 114: /* create_table_statement  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 1800 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 115: /* create_index_statement  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1810 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 116: /* drop_table_statement  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 1820 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 117: /* column_def  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 1830 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 118: /* column_def_commalist  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 1840 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 119: /* data_type  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 1850 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 120: /* column_constraint_def  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 1860 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 121: /* column_constraint_def_list  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1870 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 122: /* opt_column_constraint_def_list  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1880 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 126: /* opt_column_list  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 1890 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 127: /* opt_block_properties  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 1900 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 128: /* key_value_list  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 1910 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 129: /* key_value  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 1920 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 130: /* key_string_value  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 1930 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 131: /* key_string_list  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 1940 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 132: /* key_integer_value  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 1950 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 133: /* index_type  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1960 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 134: /* opt_index_properties  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 1970 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 135: /* insert_statement  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 1980 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 136: /* copy_from_statement  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_statement_) != nullptr) {
    delete ((*yyvaluep).copy_from_statement_);
  }
}
#line 1990 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 137: /* opt_copy_from_params  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2000 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 138: /* copy_from_params  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2010 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 139: /* update_statement  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2020 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 140: /* delete_statement  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2030 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 141: /* assignment_list  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2040 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 142: /* assignment_item  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2050 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 143: /* select_statement  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_statement_) != nullptr) {
    delete ((*yyvaluep).select_statement_);
  }
}
#line 2060 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 144: /* opt_with_clause  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2070 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 145: /* with_list  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2080 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 146: /* with_list_element  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2090 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 147: /* select_query  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2100 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 149: /* selection  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2110 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 150: /* selection_item_commalist  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2120 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 151: /* selection_item  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2130 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 152: /* from_clause  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2140 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* subquery_expression  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2150 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* table_reference  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2160 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* table_reference_signature  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2170 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* table_reference_signature_primary  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2180 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* table_reference_commalist  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2190 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* opt_group_by_clause  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2200 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* opt_having_clause  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2210 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 163: /* opt_order_by_clause  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2220 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 164: /* opt_limit_clause  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2230 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 165: /* order_commalist  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2240 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 166: /* order_item  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2250 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 167: /* opt_order_direction  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2260 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* opt_nulls_first  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2270 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* opt_where_clause  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2280 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* where_clause  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2290 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* or_expression  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2300 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* and_expression  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2310 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* not_expression  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2320 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* predicate_expression_base  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2330 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* add_expression  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2340 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* multiply_expression  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2350 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* unary_expression  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2360 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* expression_base  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2370 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* function_call  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2380 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* expression_list  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2390 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* literal_value  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2400 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* literal_value_commalist  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2410 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* attribute_ref  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2420 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* comparison_operation  */
#line 482 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2426 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* unary_operation  */
#line 483 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2432 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* add_operation  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2438 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* multiply_operation  */
#line 484 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2444 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* name_commalist  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 2454 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* any_name  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2464 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 190: /* boolean_value  */
#line 481 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2470 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 495 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2767 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 499 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2776 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 503 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    YYABORT;
  }
#line 2784 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 506 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 2793 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 513 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 2801 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 516 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].copy_from_statement_);
  }
#line 2809 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 519 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 2817 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 522 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 2825 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 525 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 2833 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 528 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 2841 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 531 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 2849 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 534 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 2857 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 537 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].select_statement_);
  }
#line 2865 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 540 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 2873 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 546 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 2881 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 552 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2893 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 559 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2904 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 565 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2916 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 572 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2928 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 581 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-7]).first_line, (yylsp[-7]).first_column, (yyvsp[-5].string_value_), (yyvsp[-3].attribute_definition_list_), (yyvsp[0].key_value_list_));
  }
#line 2936 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 586 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 2951 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 598 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 2959 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 603 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 2967 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 608 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 2976 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 612 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 2985 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 618 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 2995 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 623 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3003 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 626 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3011 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 629 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3021 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 634 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3029 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 637 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3037 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 640 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3045 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 643 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3053 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 646 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3061 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 649 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3069 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 652 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3077 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 655 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3085 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 658 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3093 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 661 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3110 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 673 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3118 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 676 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3126 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 679 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3149 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 697 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3172 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 717 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3180 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 720 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3188 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 723 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3198 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 728 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3208 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 733 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3219 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 739 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3230 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 745 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3242 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 754 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3251 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 758 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3260 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 764 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3268 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 767 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3276 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 772 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3286 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 777 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3296 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 782 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3308 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 789 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3318 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 796 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3327 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 800 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3336 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 806 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 3344 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 809 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = $1; */
  }
#line 3352 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 814 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = nullptr;
  }
#line 3360 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 817 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    (yyval.string_list_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "list of column names in CREATE INDEX statement");
    YYERROR;
  }
#line 3371 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 825 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 3379 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 828 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 3387 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 833 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3396 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 837 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3405 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 843 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 3413 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 846 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 3421 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 849 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 3429 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 854 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 3437 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 857 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), 
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 3447 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 864 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 3455 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 869 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 3469 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 880 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Bloom Filter Index");
    YYERROR;
  }
#line 3479 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 885 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "CSB Tree Index");
    YYERROR;
  }
#line 3489 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 892 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 3497 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 895 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 3505 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 901 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 3518 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 909 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsert((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 3526 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 914 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_statement_) = new quickstep::ParseStatementCopyFrom((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].copy_from_params_));
  }
#line 3534 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 919 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = nullptr;
  }
#line 3542 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 922 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-1].copy_from_params_);
  }
#line 3550 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 927 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3559 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 931 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3568 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 935 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3577 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 939 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3586 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 945 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 3594 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 950 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 3602 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 955 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3611 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 959 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3620 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 965 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 3628 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 971 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].select_query_), (yyvsp[-1].with_list_));
  }
#line 3636 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 976 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = nullptr;
  }
#line 3644 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 979 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 3652 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 984 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3661 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 988 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3670 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 994 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 3679 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 1001 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].selection_), (yyvsp[-5].table_reference_list_), (yyvsp[-4].predicate_), (yyvsp[-3].opt_group_by_clause_), (yyvsp[-2].opt_having_clause_), (yyvsp[-1].opt_order_by_clause_), (yyvsp[0].opt_limit_clause_));
  }
#line 3687 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 1006 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 3695 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 1009 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "ALL in selection");
    YYERROR;
  }
#line 3704 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 1013 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "DISTINCT in selection");
    YYERROR;
  }
#line 3713 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 1019 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3721 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 1022 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 3729 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 1027 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3738 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 1031 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3747 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 1037 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 3755 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 1040 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 3763 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 1043 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 3771 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 1048 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = (yyvsp[-1].table_reference_list_);
  }
#line 3779 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 1053 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 3787 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 1056 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3796 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 1062 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[-1]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3805 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 1066 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3814 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 1072 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3825 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 1078 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3836 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 1084 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3847 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 1090 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3858 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 1096 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3869 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 1102 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3880 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 1108 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3891 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 1114 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3902 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 1122 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].select_query_));
  }
#line 3910 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 1127 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 3919 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 1131 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 3928 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 1135 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 3936 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 1140 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 3944 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 1143 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 3952 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 1148 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 3960 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 1151 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 3968 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 1156 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 3977 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 1160 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 3986 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 1166 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 3994 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 1169 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4002 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 1174 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4010 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 1177 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4018 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 1182 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4026 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 1185 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4034 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 1190 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4042 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 1193 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4064 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 1212 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4073 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 1216 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4082 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 1222 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 4092 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 1229 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4100 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 1232 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4108 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 148:
#line 1235 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4116 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 149:
#line 1240 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4124 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 150:
#line 1243 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4132 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 1246 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4140 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 1252 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 4148 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 1255 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4156 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 1260 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4164 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 1265 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4178 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 1274 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4186 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 1279 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4200 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 158:
#line 1288 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4208 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 159:
#line 1293 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4216 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 1296 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4224 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 161:
#line 1301 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4232 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 1304 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 4242 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 163:
#line 1309 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4253 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 1315 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4264 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 165:
#line 1321 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].expression_);
    delete (yyvsp[0].string_value_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "LIKE predicates");
    YYERROR;
  }
#line 4276 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 166:
#line 1328 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4284 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 1331 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 4292 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 168:
#line 1338 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4300 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 169:
#line 1341 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4308 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 1346 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4316 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 171:
#line 1349 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4324 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 172:
#line 1354 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].unary_operation_), (yyvsp[0].expression_));
  }
#line 4332 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 173:
#line 1357 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4340 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 174:
#line 1362 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 4348 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 175:
#line 1365 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 4356 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 176:
#line 1368 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 4364 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 177:
#line 1371 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 4372 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 178:
#line 1376 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 4381 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 1380 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 4390 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 180:
#line 1384 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4398 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 181:
#line 1389 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4407 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 1393 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4416 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 183:
#line 1399 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4424 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 184:
#line 1402 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4432 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 185:
#line 1405 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4440 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 186:
#line 1408 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4457 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 187:
#line 1420 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 4466 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 1424 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4490 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 189:
#line 1443 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4508 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 190:
#line 1458 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4517 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 191:
#line 1462 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4526 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 192:
#line 1468 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4534 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 193:
#line 1471 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 4542 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 194:
#line 1477 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 4550 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 195:
#line 1480 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 4558 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 196:
#line 1483 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 4566 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 197:
#line 1486 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 4574 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 198:
#line 1489 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 4582 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 199:
#line 1492 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 4590 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 200:
#line 1497 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
#line 4604 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 201:
#line 1508 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
#line 4612 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 202:
#line 1511 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
#line 4620 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 203:
#line 1516 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
#line 4628 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 204:
#line 1519 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
#line 4636 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 205:
#line 1525 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4645 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 206:
#line 1529 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4654 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 1535 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 4662 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 208:
#line 1538 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 4673 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 209:
#line 1546 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4681 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 210:
#line 1549 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4689 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 211:
#line 1552 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;
  }
#line 4697 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 212:
#line 1555 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;
  }
#line 4705 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;


#line 4709 "SqlParser_gen.cpp" /* yacc.c:1646  */
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
#line 1559 "../SqlParser.ypp" /* yacc.c:1906  */


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
