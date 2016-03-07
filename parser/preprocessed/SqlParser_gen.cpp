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
#include "parser/ParseBlockProperties.hpp"
#include "parser/ParseExpression.hpp"
#include "parser/ParseGeneratorTableReference.hpp"
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

#line 147 "SqlParser_gen.cpp" /* yacc.c:339  */

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
    TOKEN_REGEXP = 272,
    TOKEN_BETWEEN = 273,
    TOKEN_IS = 274,
    UNARY_PLUS = 275,
    UNARY_MINUS = 276,
    TOKEN_ADD = 277,
    TOKEN_ALL = 278,
    TOKEN_ALTER = 279,
    TOKEN_AS = 280,
    TOKEN_ASC = 281,
    TOKEN_BIGINT = 282,
    TOKEN_BIT = 283,
    TOKEN_BLOCKPROPERTIES = 284,
    TOKEN_BLOOM_FILTER = 285,
    TOKEN_CSB_TREE = 286,
    TOKEN_BY = 287,
    TOKEN_CHARACTER = 288,
    TOKEN_CHECK = 289,
    TOKEN_COLUMN = 290,
    TOKEN_CONSTRAINT = 291,
    TOKEN_COPY = 292,
    TOKEN_CREATE = 293,
    TOKEN_DATE = 294,
    TOKEN_DATETIME = 295,
    TOKEN_DECIMAL = 296,
    TOKEN_DEFAULT = 297,
    TOKEN_DELETE = 298,
    TOKEN_DELIMITER = 299,
    TOKEN_DESC = 300,
    TOKEN_DISTINCT = 301,
    TOKEN_DOUBLE = 302,
    TOKEN_DROP = 303,
    TOKEN_ESCAPE_STRINGS = 304,
    TOKEN_FALSE = 305,
    TOKEN_FIRST = 306,
    TOKEN_FLOAT = 307,
    TOKEN_FOREIGN = 308,
    TOKEN_FROM = 309,
    TOKEN_FULL = 310,
    TOKEN_GROUP = 311,
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
    TOKEN_NULL = 325,
    TOKEN_NULLS = 326,
    TOKEN_OFF = 327,
    TOKEN_ON = 328,
    TOKEN_ORDER = 329,
    TOKEN_OUTER = 330,
    TOKEN_PRIMARY = 331,
    TOKEN_QUIT = 332,
    TOKEN_REAL = 333,
    TOKEN_REFERENCES = 334,
    TOKEN_RIGHT = 335,
    TOKEN_ROW_DELIMITER = 336,
    TOKEN_SELECT = 337,
    TOKEN_SET = 338,
    TOKEN_SMALLINT = 339,
    TOKEN_TABLE = 340,
    TOKEN_TIME = 341,
    TOKEN_TIMESTAMP = 342,
    TOKEN_TRUE = 343,
    TOKEN_UNIQUE = 344,
    TOKEN_UPDATE = 345,
    TOKEN_USING = 346,
    TOKEN_VALUES = 347,
    TOKEN_VARCHAR = 348,
    TOKEN_WHERE = 349,
    TOKEN_WITH = 350,
    TOKEN_YEARMONTH = 351,
    TOKEN_EOF = 352,
    TOKEN_LEX_ERROR = 353
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 109 "../SqlParser.ypp" /* yacc.c:355  */

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

  quickstep::PtrList<quickstep::ParseOrderByItem> *order_commalist_;
  quickstep::ParseOrderByItem *order_item_;

  quickstep::PtrVector<quickstep::ParseSubqueryTableReference> *with_list_;
  quickstep::ParseSubqueryTableReference *with_list_element_;

#line 363 "SqlParser_gen.cpp" /* yacc.c:355  */
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
#line 186 "../SqlParser.ypp" /* yacc.c:358  */

/* This header needs YYSTYPE, which is defined by the %union directive above */
#include "SqlLexer_gen.hpp"
void NotSupported(const YYLTYPE *location, yyscan_t yyscanner, const std::string &feature);

#line 398 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYLAST   986

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  108
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  82
/* YYNRULES -- Number of rules.  */
#define YYNRULES  217
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  415

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   353

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
     105,   106,    22,    20,   107,    21,    26,    23,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   104,
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
      15,    16,    17,    18,    19,    24,    25,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   500,   500,   504,   508,   511,   518,   521,   524,   527,
     530,   533,   536,   539,   542,   545,   551,   557,   564,   570,
     577,   586,   591,   596,   601,   606,   610,   616,   621,   624,
     627,   632,   635,   638,   641,   644,   647,   650,   653,   656,
     659,   671,   674,   677,   695,   715,   718,   721,   726,   731,
     737,   743,   752,   756,   762,   765,   770,   775,   780,   787,
     794,   798,   804,   807,   812,   815,   823,   826,   831,   835,
     841,   844,   847,   852,   855,   862,   867,   878,   882,   888,
     891,   897,   905,   910,   915,   918,   923,   927,   931,   935,
     941,   946,   951,   955,   961,   967,   972,   975,   980,   984,
     990,   996,  1002,  1005,  1009,  1015,  1018,  1023,  1027,  1033,
    1036,  1039,  1044,  1049,  1052,  1058,  1062,  1068,  1074,  1080,
    1086,  1092,  1098,  1104,  1110,  1118,  1123,  1127,  1131,  1135,
    1138,  1143,  1146,  1151,  1154,  1159,  1163,  1169,  1172,  1177,
    1180,  1185,  1188,  1193,  1196,  1215,  1219,  1225,  1232,  1235,
    1238,  1243,  1246,  1249,  1255,  1258,  1263,  1268,  1277,  1282,
    1291,  1296,  1299,  1304,  1307,  1312,  1318,  1324,  1327,  1333,
    1336,  1341,  1344,  1349,  1352,  1357,  1360,  1363,  1366,  1371,
    1375,  1379,  1384,  1388,  1394,  1397,  1400,  1403,  1415,  1419,
    1438,  1453,  1457,  1463,  1466,  1472,  1475,  1478,  1481,  1484,
    1487,  1490,  1493,  1496,  1499,  1504,  1515,  1518,  1523,  1526,
    1532,  1536,  1542,  1545,  1553,  1556,  1559,  1562
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
  "TOKEN_NEQ", "TOKEN_LIKE", "TOKEN_REGEXP", "TOKEN_BETWEEN", "TOKEN_IS",
  "'+'", "'-'", "'*'", "'/'", "UNARY_PLUS", "UNARY_MINUS", "'.'",
  "TOKEN_ADD", "TOKEN_ALL", "TOKEN_ALTER", "TOKEN_AS", "TOKEN_ASC",
  "TOKEN_BIGINT", "TOKEN_BIT", "TOKEN_BLOCKPROPERTIES",
  "TOKEN_BLOOM_FILTER", "TOKEN_CSB_TREE", "TOKEN_BY", "TOKEN_CHARACTER",
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
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
      43,    45,    42,    47,   275,   276,    46,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,    59,    40,    41,    44
};
# endif

#define YYPACT_NINF -174

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-174)))

#define YYTABLE_NINF -97

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      32,  -174,   -49,   165,   -27,    -7,    43,    37,  -174,   165,
     165,  -174,    62,    94,  -174,  -174,  -174,  -174,  -174,  -174,
    -174,  -174,  -174,  -174,    59,   165,  -174,  -174,    97,   165,
     165,   165,   165,   165,    71,    64,  -174,   133,    72,  -174,
    -174,  -174,    93,  -174,    33,   187,   126,   102,   114,  -174,
      46,   165,   165,   116,   165,  -174,  -174,   463,    96,   160,
     148,   165,   165,   538,  -174,  -174,   118,   165,    -4,  -174,
     226,  -174,    59,  -174,   112,  -174,  -174,  -174,   245,   265,
    -174,  -174,  -174,   169,  -174,   210,  -174,  -174,  -174,  -174,
     275,  -174,  -174,  -174,  -174,  -174,  -174,   191,   231,   612,
     296,   243,   197,  -174,   144,   203,  -174,  -174,  -174,  -174,
    -174,   686,    -5,   165,   -16,   165,   165,   201,  -174,   205,
    -174,   121,   885,   760,   538,   301,   304,  -174,  -174,   272,
     297,   824,   123,   165,  -174,   612,   211,  -174,   165,  -174,
    -174,   309,  -174,  -174,   313,  -174,    -2,  -174,    11,   114,
     612,  -174,  -174,   165,   612,  -174,  -174,  -174,   612,   265,
    -174,   165,   388,  -174,   215,   251,   252,   218,  -174,  -174,
    -174,    39,   165,   228,   -16,   165,  -174,     8,  -174,     1,
     172,   538,   538,   199,  -174,  -174,  -174,  -174,  -174,  -174,
    -174,  -174,   612,   612,    15,  -174,   125,   229,  -174,   213,
    -174,  -174,   222,   223,  -174,    68,  -174,    88,    68,    10,
     271,  -174,  -174,   203,  -174,  -174,   232,  -174,   213,   132,
     538,   235,   237,   165,   333,   -24,   134,   136,   209,  -174,
     236,   244,  -174,   270,   241,   824,  -174,   277,   165,  -174,
    -174,     8,  -174,  -174,   304,  -174,  -174,  -174,   612,   158,
     213,   274,  -174,  -174,   824,   246,  -174,  -174,   165,  -174,
    -174,   -34,   281,   165,    60,    73,    11,  -174,   139,  -174,
    -174,  -174,   315,   291,  -174,  -174,   612,    18,   165,   165,
     140,  -174,  -174,  -174,  -174,  -174,  -174,  -174,   113,  -174,
    -174,  -174,   249,   -16,   325,  -174,  -174,   538,  -174,  -174,
     256,  -174,   189,   612,  -174,  -174,   824,  -174,   165,   294,
     165,   -41,   165,   298,   165,   299,  -174,  -174,   612,   538,
     286,   213,  -174,   147,   150,  -174,   362,   -24,   165,  -174,
    -174,   266,    25,   165,   612,   213,   152,   -40,   165,   -39,
     538,   -35,   165,   -28,   165,   263,   301,   336,   302,   292,
    -174,  -174,  -174,   155,  -174,  -174,  -174,  -174,     6,   165,
    -174,   280,   213,  -174,   538,    16,   538,   301,   538,    47,
     538,    48,   612,   372,  -174,   165,  -174,   165,  -174,  -174,
     165,  -174,   159,  -174,   301,   538,   301,   301,   538,   301,
     538,   273,  -174,    56,  -174,   283,  -174,   163,  -174,   301,
     301,   301,   612,  -174,  -174,   314,   165,  -174,  -174,    28,
    -174,   166,  -174,  -174,  -174
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     4,     0,     0,     0,     0,     0,     0,    16,     0,
       0,     5,     0,     0,    13,     6,     8,     9,    11,    12,
       7,    15,    10,    14,     0,     0,   212,   213,     0,     0,
       0,     0,     0,     0,     0,    97,    98,     0,   133,     1,
       3,     2,   102,    95,     0,     0,     0,     0,   154,    23,
       0,     0,     0,     0,     0,   103,   104,     0,     0,     0,
      84,     0,     0,     0,    91,   155,     0,     0,   154,    93,
       0,    99,     0,   100,     0,   210,   188,   185,     0,   205,
     105,    38,    27,     0,    28,    29,    32,    34,    35,    37,
       0,    39,   184,    33,    36,    30,    31,     0,     0,     0,
       0,     0,   106,   107,   111,   170,   172,   174,   177,   176,
     175,     0,   193,     0,     0,     0,     0,     0,    83,    64,
      25,     0,     0,     0,     0,   156,   158,   160,   162,     0,
     175,     0,     0,     0,    90,     0,     0,   134,     0,   186,
     187,     0,    41,   189,     0,    42,     0,   190,     0,   154,
       0,   206,   207,     0,     0,   110,   208,   209,     0,     0,
     173,     0,     0,    17,     0,     0,     0,     0,    18,    19,
      20,     0,     0,     0,    62,     0,    40,    54,   161,     0,
       0,     0,     0,     0,   195,   197,   198,   199,   200,   196,
     201,   203,     0,     0,     0,   191,     0,     0,    92,    94,
     125,   211,     0,     0,   178,     0,   135,   113,   129,   130,
     137,   108,   109,   169,   171,   194,     0,   179,   182,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
      63,    66,    26,     0,     0,     0,    45,     0,     0,    47,
      53,    55,    24,   168,   157,   159,   202,   204,     0,     0,
     167,     0,   166,    82,     0,     0,    43,    44,     0,   126,
     131,     0,     0,     0,     0,     0,     0,   112,   114,   116,
     128,   127,     0,   139,   180,   181,     0,     0,     0,     0,
       0,    86,   216,   217,   215,   214,    87,    85,     0,    65,
      77,    78,    79,     0,     0,    21,    46,     0,    49,    48,
       0,    52,     0,     0,   165,   192,     0,   132,     0,     0,
       0,     0,     0,     0,     0,     0,   136,   115,     0,     0,
     141,   183,    59,     0,     0,    56,     0,     0,     0,    22,
      60,     0,     0,     0,     0,   163,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   138,   140,     0,   143,     0,
      57,    88,    89,     0,    68,    70,    71,    72,     0,     0,
      50,     0,   164,    81,     0,     0,     0,   118,     0,     0,
       0,     0,     0,     0,   101,     0,    80,     0,    76,    74,
       0,    73,     0,    51,   124,     0,   117,   120,     0,   122,
       0,   142,   145,   148,   144,     0,    69,     0,    67,   123,
     119,   121,     0,   149,   150,   151,     0,    75,   146,     0,
     147,     0,   152,   153,    58
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -174,  -174,  -174,  -174,  -174,  -174,  -174,  -174,   -93,  -174,
     267,   156,  -174,  -174,  -173,  -174,  -174,  -174,  -174,    40,
      19,  -174,  -174,  -174,  -174,  -174,  -174,  -174,  -174,  -174,
    -174,  -174,  -174,   268,  -174,  -174,  -174,   346,   328,  -174,
    -174,  -174,   254,  -174,  -174,  -174,   138,   358,   146,    86,
      -8,  -174,  -174,  -174,  -174,  -174,  -174,    12,  -174,  -174,
     -58,  -174,    17,   234,   240,   290,   -54,   262,   259,   307,
    -144,   101,  -126,   117,    -6,  -174,  -174,  -174,  -174,   -11,
      -3,    98
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    12,    13,    14,    15,    16,    17,    18,   120,   121,
     100,   240,   241,   242,   168,   230,   231,   173,   295,   353,
     354,   355,   356,   357,   292,   329,    19,    20,   118,   226,
      21,    22,    68,    69,    23,    24,    35,    36,    43,    57,
     101,   102,   103,   149,   267,   268,   269,   205,   206,   259,
     260,   207,   273,   320,   348,   374,   391,   392,   405,   410,
      64,    65,   125,   126,   127,   128,   129,   105,   106,   107,
     108,   219,   109,   196,   110,   193,   111,   154,   158,    74,
     112,   286
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      28,   229,    37,   104,   208,   195,    34,    38,   181,    26,
     134,    27,   378,    26,    26,    27,    27,   233,   151,   152,
     163,   161,    44,   164,   251,   181,    46,    47,    48,    49,
      50,   282,   181,     1,   379,   308,    29,   340,   364,   366,
     258,    25,   165,   368,    37,   146,   309,   234,    70,    38,
     370,    75,    31,   283,   284,   235,   132,   130,   119,   122,
      58,     2,    39,    30,    75,   166,   138,   138,   138,   285,
     180,    26,   138,    27,     3,     4,   151,   152,   167,   138,
       5,   199,   232,   236,   412,     6,    59,   403,   224,   237,
     252,   210,   238,   225,   385,    63,   104,     7,   258,   413,
     162,   155,   239,   133,   204,    33,   404,   243,   218,   298,
     122,   380,   169,   170,     8,   162,    72,   130,   130,   -96,
     330,    55,   208,   138,   322,   388,   390,     9,   305,   312,
      70,   360,    10,    32,    11,   201,   113,   114,   249,   250,
     313,   179,   314,    66,    56,   209,    42,    26,   261,    27,
     212,    67,   262,   315,   138,   138,    45,   263,   215,    51,
     264,   227,   326,    53,   151,   152,   303,   327,    26,    75,
      27,    52,   122,   265,   153,   130,   130,    54,   151,   152,
     195,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,    60,   151,   152,   302,   266,    40,   334,    41,   261,
     115,   116,    38,   262,    61,    38,    38,    62,   263,   151,
     152,   264,   280,    63,   130,   246,   247,   248,   137,   138,
      75,    72,   321,   131,   265,   156,   157,   174,   175,   197,
     138,   253,   254,   151,   152,   300,   135,   277,   275,   276,
     287,   288,   289,   138,   290,   291,   325,   138,   117,   335,
     307,   139,   311,   349,   138,    38,   350,   138,   363,   254,
      75,   376,   377,   209,   218,   398,   377,   323,   324,   407,
     138,   140,   414,   138,   141,    75,    75,   142,   204,   143,
     362,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   130,   151,   152,   270,   271,   144,   337,   145,   339,
     147,   341,   148,   343,   150,    75,   171,    75,   181,    75,
     172,    75,   182,   130,   332,   202,   194,   200,   393,   203,
     220,   221,   222,   223,   228,   358,   255,   365,   256,   257,
     361,   369,   272,   371,   130,    75,   346,   281,   274,    75,
     278,    75,   279,   293,   294,   296,   297,   299,   393,   304,
     310,   306,   318,   319,   328,   381,   358,   367,   130,   331,
     130,   333,   130,   338,   130,   347,   351,   342,   344,   397,
     276,   359,   395,   372,   358,   373,   375,    75,   394,   130,
     402,   384,   130,   386,   130,   387,   383,   389,   406,   177,
     409,    26,    76,    27,    77,   411,   396,   301,    71,   382,
     136,   198,   399,    75,   211,   400,   317,   401,    78,    79,
     216,    73,   316,   178,   408,   244,   213,   214,   160,   345,
      81,    82,   245,   336,     0,   352,    83,     0,     0,     0,
       0,     0,    84,    85,    86,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,     0,     0,     0,     0,
       0,     0,    91,    92,     0,     0,    26,    76,    27,    77,
       0,    93,     0,     0,     0,     0,     0,    94,     0,    95,
      96,     0,     0,    78,    79,    80,    97,     0,     0,    98,
       0,     0,     0,    99,   217,    81,    82,     0,     0,     0,
       0,    83,     0,     0,     0,     0,     0,    84,    85,    86,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,     0,     0,     0,     0,     0,     0,    91,    92,     0,
       0,    26,    76,    27,    77,     0,    93,   123,     0,     0,
       0,     0,    94,     0,    95,    96,     0,     0,    78,    79,
       0,    97,     0,     0,    98,     0,     0,     0,    99,     0,
      81,    82,     0,     0,     0,     0,    83,     0,     0,     0,
       0,     0,    84,    85,    86,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,     0,     0,     0,     0,
       0,     0,    91,    92,     0,    26,    76,    27,    77,     0,
       0,    93,     0,     0,     0,     0,     0,    94,     0,    95,
      96,     0,    78,    79,     0,     0,    97,     0,     0,    98,
       0,     0,     0,   124,    81,    82,     0,     0,     0,     0,
      83,     0,     0,     0,     0,     0,    84,    85,    86,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
       0,     0,     0,     0,     0,     0,    91,    92,     0,    26,
      76,    27,    77,     0,     0,    93,     0,     0,     0,     0,
       0,    94,     0,    95,    96,     0,    78,   159,     0,     0,
      97,     0,     0,    98,     0,     0,     0,    99,    81,    82,
       0,     0,     0,     0,    83,     0,     0,     0,     0,     0,
      84,    85,    86,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,     0,     0,     0,     0,     0,     0,
      91,    92,     0,    26,    76,    27,    77,     0,     0,    93,
       0,     0,     0,     0,     0,    94,     0,    95,    96,     0,
      78,    79,     0,     0,    97,     0,     0,    98,     0,     0,
       0,    99,    81,    82,     0,     0,     0,     0,    83,     0,
       0,     0,     0,     0,    84,    85,    86,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    76,     0,
      77,     0,     0,     0,    91,    92,     0,     0,     0,     0,
       0,     0,     0,    93,    78,   159,     0,     0,     0,    94,
       0,    95,    96,     0,     0,     0,    81,    82,    97,     0,
       0,    98,    83,     0,     0,   124,     0,     0,    84,    85,
      86,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,     0,     0,     0,     0,     0,     0,    91,    92,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,    94,     0,    95,    96,    81,    82,     0,
       0,     0,    97,    83,     0,    98,     0,     0,     0,    84,
      85,    86,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,    89,   176,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,    94,     0,    95,    96,     0,     0,
       0,     0,     0,    97,     0,     0,    98
};

static const yytype_int16 yycheck[] =
{
       3,   174,    10,    57,   148,   131,     9,    10,     7,     3,
      68,     5,     6,     3,     3,     5,     5,     9,    20,    21,
     113,    26,    25,    39,     9,     7,    29,    30,    31,    32,
      33,    55,     7,     1,    28,    69,    63,    78,    78,    78,
      30,    90,    58,    78,    52,    99,    80,    39,    51,    52,
      78,    54,    59,    77,    78,    47,    67,    63,    61,    62,
      27,    29,     0,    90,    67,    81,   107,   107,   107,    93,
     124,     3,   107,     5,    42,    43,    20,    21,    94,   107,
      48,   135,   175,    75,    56,    53,    53,    31,    49,    81,
      75,   149,    84,    54,    78,    99,   150,    65,    30,    71,
     105,   104,    94,   107,   106,    68,    50,   106,   162,   235,
     113,   105,   115,   116,    82,   105,   105,   123,   124,    87,
     293,    28,   266,   107,   106,    78,    78,    95,   254,    69,
     133,   106,   100,    90,   102,   138,    40,    41,   192,   193,
      80,   124,    69,    97,    51,   148,    87,     3,    60,     5,
     153,   105,    64,    80,   107,   107,    59,    69,   161,    88,
      72,   172,    49,    30,    20,    21,     8,    54,     3,   172,
       5,   107,   175,    85,    30,   181,   182,   105,    20,    21,
     306,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,     4,    20,    21,   248,   107,   102,     8,   104,    60,
      40,    41,   205,    64,    78,   208,   209,   105,    69,    20,
      21,    72,   223,    99,   220,    16,    17,    18,   106,   107,
     223,   105,   276,   105,    85,    22,    23,   106,   107,   106,
     107,   106,   107,    20,    21,   238,    10,   220,   106,   107,
     106,   107,   106,   107,    35,    36,   106,   107,   100,   303,
     258,     6,   263,   106,   107,   258,   106,   107,   106,   107,
     263,   106,   107,   266,   318,   106,   107,   278,   279,   106,
     107,     6,   106,   107,   105,   278,   279,    67,   106,     4,
     334,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,   297,    20,    21,   208,   209,   105,   308,    67,   310,
       4,   312,    59,   314,   107,   308,   105,   310,     7,   312,
     105,   314,     8,   319,   297,     6,    19,   106,   372,     6,
     105,    70,    70,   105,    96,   328,    97,   338,   106,   106,
     333,   342,    61,   344,   340,   338,   319,     4,   106,   342,
     105,   344,   105,   107,   100,    75,   105,    70,   402,    75,
      69,   105,    37,    62,   105,   358,   359,   340,   364,    34,
     366,   105,   368,    69,   370,    79,     4,    69,    69,   380,
     107,   105,   375,    37,   377,    73,    84,   380,     6,   385,
     107,   364,   388,   366,   390,   368,   106,   370,   105,   122,
      76,     3,     4,     5,     6,   406,   377,   241,    52,   359,
      72,   133,   385,   406,   150,   388,   268,   390,    20,    21,
      22,    53,   266,   123,   402,   181,   154,   158,   111,   318,
      32,    33,   182,   306,    -1,   327,    38,    -1,    -1,    -1,
      -1,    -1,    44,    45,    46,    -1,    -1,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    74,    75,    -1,    -1,     3,     4,     5,     6,
      -1,    83,    -1,    -1,    -1,    -1,    -1,    89,    -1,    91,
      92,    -1,    -1,    20,    21,    22,    98,    -1,    -1,   101,
      -1,    -1,    -1,   105,   106,    32,    33,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    45,    46,
      -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    -1,
      -1,     3,     4,     5,     6,    -1,    83,     9,    -1,    -1,
      -1,    -1,    89,    -1,    91,    92,    -1,    -1,    20,    21,
      -1,    98,    -1,    -1,   101,    -1,    -1,    -1,   105,    -1,
      32,    33,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    -1,    44,    45,    46,    -1,    -1,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    74,    75,    -1,     3,     4,     5,     6,    -1,
      -1,    83,    -1,    -1,    -1,    -1,    -1,    89,    -1,    91,
      92,    -1,    20,    21,    -1,    -1,    98,    -1,    -1,   101,
      -1,    -1,    -1,   105,    32,    33,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    45,    46,    -1,
      -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    -1,     3,
       4,     5,     6,    -1,    -1,    83,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    91,    92,    -1,    20,    21,    -1,    -1,
      98,    -1,    -1,   101,    -1,    -1,    -1,   105,    32,    33,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      44,    45,    46,    -1,    -1,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    75,    -1,     3,     4,     5,     6,    -1,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    91,    92,    -1,
      20,    21,    -1,    -1,    98,    -1,    -1,   101,    -1,    -1,
      -1,   105,    32,    33,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    44,    45,    46,    -1,    -1,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    67,     4,    -1,
       6,    -1,    -1,    -1,    74,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    20,    21,    -1,    -1,    -1,    89,
      -1,    91,    92,    -1,    -1,    -1,    32,    33,    98,    -1,
      -1,   101,    38,    -1,    -1,   105,    -1,    -1,    44,    45,
      46,    -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    74,    75,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    91,    92,    32,    33,    -1,
      -1,    -1,    98,    38,    -1,   101,    -1,    -1,    -1,    44,
      45,    46,    -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    91,    92,    -1,    -1,
      -1,    -1,    -1,    98,    -1,    -1,   101
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,    29,    42,    43,    48,    53,    65,    82,    95,
     100,   102,   109,   110,   111,   112,   113,   114,   115,   134,
     135,   138,   139,   142,   143,    90,     3,     5,   188,    63,
      90,    59,    90,    68,   188,   144,   145,   158,   188,     0,
     102,   104,    87,   146,   188,    59,   188,   188,   188,   188,
     188,    88,   107,    30,   105,    28,    51,   147,    27,    53,
       4,    78,   105,    99,   168,   169,    97,   105,   140,   141,
     188,   145,   105,   155,   187,   188,     4,     6,    20,    21,
      22,    32,    33,    38,    44,    45,    46,    52,    57,    66,
      67,    74,    75,    83,    89,    91,    92,    98,   101,   105,
     118,   148,   149,   150,   174,   175,   176,   177,   178,   180,
     182,   184,   188,    40,    41,    40,    41,   100,   136,   188,
     116,   117,   188,     9,   105,   170,   171,   172,   173,   174,
     182,   105,   187,   107,   168,    10,   146,   106,   107,     6,
       6,   105,    67,     4,   105,    67,   174,     4,    59,   151,
     107,    20,    21,    30,   185,   188,    22,    23,   186,    21,
     177,    26,   105,   116,    39,    58,    81,    94,   122,   188,
     188,   105,   105,   125,   106,   107,    67,   118,   173,   170,
     174,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,   183,    19,   180,   181,   106,   141,   174,
     106,   188,     6,     6,   106,   155,   156,   159,   178,   188,
     168,   150,   188,   175,   176,   188,    22,   106,   174,   179,
     105,    70,    70,   105,    49,    54,   137,   187,    96,   122,
     123,   124,   116,     9,    39,    47,    75,    81,    84,    94,
     119,   120,   121,   106,   171,   172,    16,    17,    18,   174,
     174,     9,    75,   106,   107,    97,   106,   106,    30,   157,
     158,    60,    64,    69,    72,    85,   107,   152,   153,   154,
     157,   157,    61,   160,   106,   106,   107,   170,   105,   105,
     187,     4,    55,    77,    78,    93,   189,   106,   107,   106,
      35,    36,   132,   107,   100,   126,    75,   105,   180,    70,
     188,   119,   174,     8,    75,   180,   105,   158,    69,    80,
      69,   187,    69,    80,    69,    80,   156,   154,    37,    62,
     161,   174,   106,   187,   187,   106,    49,    54,   105,   133,
     122,    34,   170,   105,     8,   174,   181,   187,    69,   187,
      78,   187,    69,   187,    69,   179,   170,    79,   162,   106,
     106,     4,   189,   127,   128,   129,   130,   131,   188,   105,
     106,   188,   174,   106,    78,   187,    78,   170,    78,   187,
      78,   187,    37,    73,   163,    84,   106,   107,     6,    28,
     105,   188,   127,   106,   170,    78,   170,   170,    78,   170,
      78,   164,   165,   174,     6,   188,   128,   187,   106,   170,
     170,   170,   107,    31,    50,   166,   105,   106,   165,    76,
     167,   187,    56,    71,   106
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   108,   109,   109,   109,   109,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   111,   112,   112,   112,
     112,   113,   114,   115,   116,   117,   117,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   119,   119,   119,   119,   119,
     119,   119,   120,   120,   121,   121,   122,   122,   122,   122,
     123,   123,   124,   124,   125,   125,   126,   126,   127,   127,
     128,   128,   128,   129,   129,   130,   131,   132,   132,   133,
     133,   134,   134,   135,   136,   136,   137,   137,   137,   137,
     138,   139,   140,   140,   141,   142,   143,   143,   144,   144,
     145,   146,   147,   147,   147,   148,   148,   149,   149,   150,
     150,   150,   151,   152,   152,   153,   153,   154,   154,   154,
     154,   154,   154,   154,   154,   155,   156,   156,   156,   156,
     156,   157,   157,   158,   158,   159,   159,   160,   160,   161,
     161,   162,   162,   163,   163,   164,   164,   165,   166,   166,
     166,   167,   167,   167,   168,   168,   169,   170,   170,   171,
     171,   172,   172,   173,   173,   173,   173,   173,   173,   174,
     174,   175,   175,   176,   176,   177,   177,   177,   177,   178,
     178,   178,   179,   179,   180,   180,   180,   180,   180,   180,
     180,   181,   181,   182,   182,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   184,   185,   185,   186,   186,
     187,   187,   188,   188,   189,   189,   189,   189
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
       5,     6,     5,     6,     5,     3,     2,     2,     2,     1,
       1,     1,     2,     1,     4,     1,     3,     0,     3,     0,
       2,     0,     3,     0,     2,     1,     3,     3,     0,     1,
       1,     0,     2,     2,     0,     1,     2,     3,     1,     3,
       1,     2,     1,     5,     6,     4,     3,     3,     3,     3,
       1,     3,     1,     2,     1,     1,     1,     1,     3,     3,
       4,     4,     1,     3,     1,     1,     2,     2,     1,     2,
       2,     1,     3,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     1,     1,     1
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
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1757 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1767 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1777 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_UNSIGNED_NUMVAL  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 1787 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 110: /* sql_statement  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1797 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 111: /* quit_statement  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 1807 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 112: /* alter_table_statement  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1817 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 113: /* create_table_statement  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 1827 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 114: /* create_index_statement  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1837 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 115: /* drop_table_statement  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 1847 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 116: /* column_def  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 1857 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 117: /* column_def_commalist  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 1867 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 118: /* data_type  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 1877 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 119: /* column_constraint_def  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 1887 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 120: /* column_constraint_def_list  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1897 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 121: /* opt_column_constraint_def_list  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1907 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 125: /* opt_column_list  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 1917 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 126: /* opt_block_properties  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).block_properties_) != nullptr) {
    delete ((*yyvaluep).block_properties_);
  }
}
#line 1927 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 127: /* key_value_list  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 1937 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 128: /* key_value  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 1947 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 129: /* key_string_value  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 1957 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 130: /* key_string_list  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 1967 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 131: /* key_integer_value  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 1977 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 132: /* index_type  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1987 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 133: /* opt_index_properties  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 1997 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 134: /* insert_statement  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 2007 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 135: /* copy_from_statement  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_statement_) != nullptr) {
    delete ((*yyvaluep).copy_from_statement_);
  }
}
#line 2017 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 136: /* opt_copy_from_params  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2027 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 137: /* copy_from_params  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2037 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 138: /* update_statement  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2047 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 139: /* delete_statement  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2057 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 140: /* assignment_list  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2067 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 141: /* assignment_item  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2077 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 142: /* select_statement  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_statement_) != nullptr) {
    delete ((*yyvaluep).select_statement_);
  }
}
#line 2087 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 143: /* opt_with_clause  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2097 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 144: /* with_list  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2107 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 145: /* with_list_element  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2117 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 146: /* select_query  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2127 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 148: /* selection  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2137 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 149: /* selection_item_commalist  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2147 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 150: /* selection_item  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2157 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 151: /* from_clause  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2167 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* subquery_expression  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2177 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* table_reference  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2187 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* table_reference_signature  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2197 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* table_reference_signature_primary  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2207 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* table_reference_commalist  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2217 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* opt_group_by_clause  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2227 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* opt_having_clause  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2237 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* opt_order_by_clause  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2247 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 163: /* opt_limit_clause  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2257 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 164: /* order_commalist  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2267 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 165: /* order_item  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2277 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 166: /* opt_order_direction  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2287 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 167: /* opt_nulls_first  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2297 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* opt_where_clause  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2307 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* where_clause  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2317 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* or_expression  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2327 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* and_expression  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2337 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* not_expression  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2347 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* predicate_expression_base  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2357 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* add_expression  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2367 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* multiply_expression  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2377 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* unary_expression  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2387 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* expression_base  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2397 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* function_call  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2407 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* expression_list  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2417 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* literal_value  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2427 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* literal_value_commalist  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2437 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* attribute_ref  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2447 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* comparison_operation  */
#line 487 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2453 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* unary_operation  */
#line 488 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2459 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* add_operation  */
#line 489 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2465 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* multiply_operation  */
#line 489 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2471 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* name_commalist  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 2481 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* any_name  */
#line 491 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2491 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* boolean_value  */
#line 486 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2497 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 500 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2794 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 504 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2803 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 508 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    YYABORT;
  }
#line 2811 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 5:
#line 511 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 2820 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 518 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 2828 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 521 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].copy_from_statement_);
  }
#line 2836 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 524 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 2844 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 9:
#line 527 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 2852 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 10:
#line 530 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 2860 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 11:
#line 533 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 2868 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 12:
#line 536 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 2876 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 13:
#line 539 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 2884 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 14:
#line 542 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].select_statement_);
  }
#line 2892 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 15:
#line 545 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 2900 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 16:
#line 551 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 2908 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 17:
#line 557 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2920 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 18:
#line 564 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2931 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 19:
#line 570 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2943 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 20:
#line 577 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2955 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 586 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-7]).first_line, (yylsp[-7]).first_column, (yyvsp[-5].string_value_), (yyvsp[-3].attribute_definition_list_), (yyvsp[0].block_properties_));
  }
#line 2963 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 591 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].string_list_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 2971 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 596 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 2979 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 601 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 2987 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 606 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 2996 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 610 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3005 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 616 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 3015 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 621 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3023 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 624 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3031 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 627 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3041 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 632 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3049 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 635 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3057 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 638 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3065 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 641 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3073 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 644 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3081 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 647 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3089 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 650 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3097 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 653 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3105 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 656 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3113 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 659 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3130 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 671 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3138 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 674 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3146 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 677 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3169 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 695 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3192 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 715 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3200 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 718 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3208 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 721 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3218 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 726 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3228 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 731 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3239 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 737 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3250 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 743 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3262 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 752 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3271 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 756 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3280 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 762 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3288 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 765 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3296 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 770 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3306 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 775 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3316 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 780 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3328 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 787 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3338 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 794 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3347 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 798 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3356 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 804 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3364 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 807 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = $1; */
  }
#line 3372 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 812 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = nullptr;
  }
#line 3380 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 815 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    (yyval.string_list_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "list of column names in CREATE INDEX statement");
    YYERROR;
  }
#line 3391 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 823 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = nullptr;
  }
#line 3399 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 826 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].key_value_list_));
  }
#line 3407 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 831 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3416 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 835 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3425 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 841 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 3433 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 844 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 3441 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 847 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 3449 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 852 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 3457 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 855 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), 
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 3467 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 862 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 3475 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 867 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 3489 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 878 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
#line 3498 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 882 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
#line 3507 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 888 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 3515 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 891 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 3523 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 897 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 3536 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 905 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsert((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 3544 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 910 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_statement_) = new quickstep::ParseStatementCopyFrom((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].copy_from_params_));
  }
#line 3552 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 915 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = nullptr;
  }
#line 3560 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 918 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-1].copy_from_params_);
  }
#line 3568 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 923 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3577 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 927 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3586 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 931 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3595 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 935 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3604 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 941 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 3612 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 946 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 3620 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 951 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3629 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 955 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3638 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 961 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 3646 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 967 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].select_query_), (yyvsp[-1].with_list_));
  }
#line 3654 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 972 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = nullptr;
  }
#line 3662 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 975 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 3670 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 980 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3679 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 984 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3688 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 990 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 3697 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 997 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].selection_), (yyvsp[-5].table_reference_list_), (yyvsp[-4].predicate_), (yyvsp[-3].opt_group_by_clause_), (yyvsp[-2].opt_having_clause_), (yyvsp[-1].opt_order_by_clause_), (yyvsp[0].opt_limit_clause_));
  }
#line 3705 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 1002 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3713 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 1005 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "ALL in selection");
    YYERROR;
  }
#line 3722 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 1009 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "DISTINCT in selection");
    YYERROR;
  }
#line 3731 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 1015 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3739 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 1018 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 3747 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 1023 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3756 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 1027 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3765 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 1033 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 3773 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 1036 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 3781 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 1039 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 3789 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 1044 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-1].table_reference_list_);
  }
#line 3797 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 1049 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3805 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 1052 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3814 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 1058 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-1]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3823 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 1062 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3832 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 1068 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3843 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 1074 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3854 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 1080 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3865 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 1086 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3876 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 1092 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3887 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 1098 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3898 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 1104 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3909 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 1110 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3920 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 1118 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].select_query_));
  }
#line 3928 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 1123 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 3937 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 1127 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 3946 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 1131 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 3955 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 1135 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].function_call_));
  }
#line 3963 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 1138 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 3971 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 1143 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 3979 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 1146 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 3987 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 1151 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 3995 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 1154 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4003 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 1159 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4012 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 1163 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4021 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 1169 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 4029 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 1172 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4037 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 1177 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4045 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 1180 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4053 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 1185 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4061 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 1188 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4069 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 1193 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4077 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 1196 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4099 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 1215 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4108 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 1219 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4117 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 1225 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 4127 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 1232 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4135 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 1235 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4143 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 1238 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4151 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 1243 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4159 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 1246 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4167 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 1249 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4175 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 1255 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 4183 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 1258 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4191 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 1263 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4199 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 1268 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4213 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 1277 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4221 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 1282 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4235 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 1291 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4243 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 1296 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4251 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 1299 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4259 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 1304 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4267 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 1307 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 4277 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 1312 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4288 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 1318 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4299 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 1324 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4307 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 1327 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 4315 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 1333 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4323 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 1336 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4331 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 1341 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4339 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 1344 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4347 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 1349 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].unary_operation_), (yyvsp[0].expression_));
  }
#line 4355 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 1352 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4363 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 1357 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 4371 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 1360 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 4379 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 1363 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 4387 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 1366 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 4395 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 1371 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 4404 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 1375 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 4413 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 1379 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4421 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 1384 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4430 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 1388 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4439 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 1394 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4447 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 1397 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4455 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1400 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4463 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 1403 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4480 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 1415 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 4489 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 1419 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4513 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 1438 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4531 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 1453 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4540 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 1457 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4549 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 1463 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4557 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 1466 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 4565 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 1472 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 4573 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 1475 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 4581 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 1478 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 4589 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 1481 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 4597 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 1484 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 4605 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1487 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 4613 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1490 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLike);
  }
#line 4621 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1493 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotLike);
  }
#line 4629 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1496 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kRegexMatch);
  }
#line 4637 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1499 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotRegexMatch);
  }
#line 4645 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1504 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
#line 4659 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1515 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
#line 4667 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1518 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
#line 4675 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1523 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
#line 4683 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1526 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
#line 4691 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1532 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4700 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1536 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4709 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1542 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 4717 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1545 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 4728 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1553 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4736 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1556 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4744 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1559 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 4752 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1562 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 4760 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;


#line 4764 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 1566 "../SqlParser.ypp" /* yacc.c:1906  */


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
