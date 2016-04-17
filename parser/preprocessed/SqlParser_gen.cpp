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
    TOKEN_DECIMAL = 300,
    TOKEN_DEFAULT = 301,
    TOKEN_DELETE = 302,
    TOKEN_DELIMITER = 303,
    TOKEN_DESC = 304,
    TOKEN_DISTINCT = 305,
    TOKEN_DOUBLE = 306,
    TOKEN_DROP = 307,
    TOKEN_ELSE = 308,
    TOKEN_END = 309,
    TOKEN_ESCAPE_STRINGS = 310,
    TOKEN_EXISTS = 311,
    TOKEN_EXTRACT = 312,
    TOKEN_FALSE = 313,
    TOKEN_FIRST = 314,
    TOKEN_FLOAT = 315,
    TOKEN_FOREIGN = 316,
    TOKEN_FROM = 317,
    TOKEN_FULL = 318,
    TOKEN_GROUP = 319,
    TOKEN_HASH = 320,
    TOKEN_HAVING = 321,
    TOKEN_IN = 322,
    TOKEN_INDEX = 323,
    TOKEN_INNER = 324,
    TOKEN_INSERT = 325,
    TOKEN_INTEGER = 326,
    TOKEN_INTERVAL = 327,
    TOKEN_INTO = 328,
    TOKEN_JOIN = 329,
    TOKEN_KEY = 330,
    TOKEN_LAST = 331,
    TOKEN_LEFT = 332,
    TOKEN_LIMIT = 333,
    TOKEN_LONG = 334,
    TOKEN_NULL = 335,
    TOKEN_NULLS = 336,
    TOKEN_OFF = 337,
    TOKEN_ON = 338,
    TOKEN_ORDER = 339,
    TOKEN_OUTER = 340,
    TOKEN_PARTITION = 341,
    TOKEN_PARTITIONS = 342,
    TOKEN_PERCENT = 343,
    TOKEN_PRIMARY = 344,
    TOKEN_QUIT = 345,
    TOKEN_RANGE = 346,
    TOKEN_REAL = 347,
    TOKEN_REFERENCES = 348,
    TOKEN_RIGHT = 349,
    TOKEN_ROW_DELIMITER = 350,
    TOKEN_SELECT = 351,
    TOKEN_SET = 352,
    TOKEN_SMA = 353,
    TOKEN_SMALLINT = 354,
    TOKEN_TABLE = 355,
    TOKEN_THEN = 356,
    TOKEN_TIME = 357,
    TOKEN_TIMESTAMP = 358,
    TOKEN_TRUE = 359,
    TOKEN_TUPLESAMPLE = 360,
    TOKEN_UNIQUE = 361,
    TOKEN_UPDATE = 362,
    TOKEN_USING = 363,
    TOKEN_VALUES = 364,
    TOKEN_VARCHAR = 365,
    TOKEN_WHEN = 366,
    TOKEN_WHERE = 367,
    TOKEN_WITH = 368,
    TOKEN_YEARMONTH = 369,
    TOKEN_EOF = 370,
    TOKEN_LEX_ERROR = 371
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

#line 403 "SqlParser_gen.cpp" /* yacc.c:355  */
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

#line 438 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYLAST   975

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  128
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  94
/* YYNRULES -- Number of rules.  */
#define YYNRULES  254
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  470

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   371

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     123,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   127,     2,     2,
     124,   125,    23,    21,   126,    22,    27,    24,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   122,
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
     120,   121
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   575,   575,   579,   583,   587,   591,   594,   601,   604,
     607,   610,   613,   616,   619,   622,   625,   628,   634,   640,
     647,   653,   660,   669,   674,   683,   688,   693,   697,   703,
     708,   711,   714,   719,   722,   725,   728,   731,   734,   737,
     740,   743,   746,   758,   761,   764,   782,   802,   805,   808,
     813,   818,   824,   830,   839,   843,   849,   852,   857,   862,
     867,   874,   881,   885,   891,   894,   899,   902,   907,   910,
     915,   918,   937,   941,   947,   951,   957,   960,   963,   968,
     971,   978,   983,   994,   999,  1003,  1007,  1013,  1016,  1022,
    1030,  1033,  1036,  1042,  1047,  1050,  1055,  1059,  1063,  1067,
    1073,  1078,  1083,  1087,  1093,  1099,  1102,  1107,  1112,  1116,
    1122,  1128,  1134,  1137,  1141,  1147,  1150,  1155,  1159,  1165,
    1168,  1171,  1176,  1181,  1186,  1189,  1192,  1197,  1200,  1203,
    1206,  1209,  1212,  1215,  1218,  1223,  1226,  1231,  1235,  1239,
    1242,  1246,  1249,  1254,  1257,  1262,  1265,  1270,  1274,  1280,
    1283,  1288,  1291,  1296,  1299,  1304,  1307,  1326,  1330,  1336,
    1343,  1346,  1349,  1354,  1357,  1360,  1366,  1369,  1374,  1379,
    1388,  1393,  1402,  1407,  1410,  1415,  1418,  1423,  1429,  1435,
    1438,  1441,  1444,  1447,  1450,  1456,  1465,  1468,  1473,  1476,
    1481,  1484,  1489,  1492,  1495,  1498,  1501,  1504,  1509,  1513,
    1517,  1520,  1525,  1530,  1533,  1538,  1542,  1548,  1553,  1557,
    1563,  1568,  1571,  1576,  1580,  1586,  1589,  1592,  1595,  1607,
    1611,  1630,  1645,  1649,  1655,  1658,  1663,  1667,  1674,  1677,
    1680,  1683,  1686,  1689,  1692,  1695,  1698,  1701,  1706,  1717,
    1720,  1725,  1728,  1731,  1737,  1741,  1747,  1750,  1758,  1761,
    1764,  1767,  1773,  1778,  1783
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
  "TOKEN_DATETIME", "TOKEN_DECIMAL", "TOKEN_DEFAULT", "TOKEN_DELETE",
  "TOKEN_DELIMITER", "TOKEN_DESC", "TOKEN_DISTINCT", "TOKEN_DOUBLE",
  "TOKEN_DROP", "TOKEN_ELSE", "TOKEN_END", "TOKEN_ESCAPE_STRINGS",
  "TOKEN_EXISTS", "TOKEN_EXTRACT", "TOKEN_FALSE", "TOKEN_FIRST",
  "TOKEN_FLOAT", "TOKEN_FOREIGN", "TOKEN_FROM", "TOKEN_FULL",
  "TOKEN_GROUP", "TOKEN_HASH", "TOKEN_HAVING", "TOKEN_IN", "TOKEN_INDEX",
  "TOKEN_INNER", "TOKEN_INSERT", "TOKEN_INTEGER", "TOKEN_INTERVAL",
  "TOKEN_INTO", "TOKEN_JOIN", "TOKEN_KEY", "TOKEN_LAST", "TOKEN_LEFT",
  "TOKEN_LIMIT", "TOKEN_LONG", "TOKEN_NULL", "TOKEN_NULLS", "TOKEN_OFF",
  "TOKEN_ON", "TOKEN_ORDER", "TOKEN_OUTER", "TOKEN_PARTITION",
  "TOKEN_PARTITIONS", "TOKEN_PERCENT", "TOKEN_PRIMARY", "TOKEN_QUIT",
  "TOKEN_RANGE", "TOKEN_REAL", "TOKEN_REFERENCES", "TOKEN_RIGHT",
  "TOKEN_ROW_DELIMITER", "TOKEN_SELECT", "TOKEN_SET", "TOKEN_SMA",
  "TOKEN_SMALLINT", "TOKEN_TABLE", "TOKEN_THEN", "TOKEN_TIME",
  "TOKEN_TIMESTAMP", "TOKEN_TRUE", "TOKEN_TUPLESAMPLE", "TOKEN_UNIQUE",
  "TOKEN_UPDATE", "TOKEN_USING", "TOKEN_VALUES", "TOKEN_VARCHAR",
  "TOKEN_WHEN", "TOKEN_WHERE", "TOKEN_WITH", "TOKEN_YEARMONTH",
  "TOKEN_EOF", "TOKEN_LEX_ERROR", "';'", "'\\n'", "'('", "')'", "','",
  "'%'", "$accept", "start", "sql_statement", "quit_statement",
  "alter_table_statement", "create_table_statement",
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
  "opt_limit_clause", "order_commalist", "order_item",
  "opt_order_direction", "opt_nulls_first", "opt_where_clause",
  "where_clause", "or_expression", "and_expression", "not_expression",
  "predicate_expression_base", "add_expression", "multiply_expression",
  "unary_expression", "expression_base", "function_call",
  "extract_function", "case_expression", "simple_when_clause_list",
  "simple_when_clause", "searched_when_clause_list",
  "searched_when_clause", "opt_else_clause", "expression_list",
  "literal_value", "literal_value_commalist", "attribute_ref",
  "attribute_ref_list", "comparison_operation", "unary_operation",
  "add_operation", "multiply_operation", "name_commalist", "any_name",
  "boolean_value", "command", "command_argument_list", YY_NULLPTR
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
     370,   371,    59,    10,    40,    41,    44,    37
};
# endif

#define YYPACT_NINF -223

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-223)))

#define YYTABLE_NINF -128

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      63,  -223,  -223,   -56,   229,   -15,     4,   -51,    14,  -223,
      36,   229,   229,  -223,    96,   124,  -223,  -223,  -223,  -223,
    -223,  -223,  -223,  -223,  -223,  -223,    97,  -223,    48,   101,
     229,  -223,  -223,   133,   229,   229,   229,   229,   229,  -223,
    -223,   493,    23,    79,  -223,   199,    60,  -223,  -223,  -223,
     179,  -223,  -223,  -223,  -223,    27,   260,   181,   165,   158,
    -223,   123,  -223,  -223,   289,   293,  -223,  -223,  -223,   526,
     178,  -223,   232,  -223,  -223,   180,  -223,  -223,   306,  -223,
    -223,  -223,  -223,  -223,  -223,   188,   242,   735,   320,   273,
     248,  -223,   221,    22,  -223,  -223,  -223,  -223,  -223,  -223,
    -223,   768,    -5,   229,   229,   223,   229,   229,   115,   194,
     237,   229,   229,   405,  -223,  -223,   234,   229,  -223,  -223,
    -223,   405,    51,   -22,  -223,   377,  -223,   229,  -223,   378,
    -223,     7,  -223,    21,   158,   735,  -223,  -223,   229,   735,
    -223,  -223,  -223,  -223,   735,   293,  -223,   229,   272,   -64,
    -223,   375,  -223,   287,  -223,   138,  -223,   287,   229,    58,
     229,   229,   265,  -223,   266,  -223,   148,   851,   614,   223,
     405,   384,   385,  -223,  -223,   349,   379,   856,   155,    16,
     735,    -2,  -223,   735,  -223,   336,   276,   331,   277,  -223,
      15,   195,   111,  -223,   278,   195,    52,   334,  -223,  -223,
      22,  -223,  -223,   280,   735,  -223,   261,   161,   229,  -223,
     735,   281,  -223,   229,  -223,  -223,   283,   328,   333,   290,
    -223,  -223,  -223,   117,   229,   303,    58,   229,  -223,   151,
    -223,  -223,     5,    64,   405,   405,    25,  -223,  -223,  -223,
    -223,  -223,  -223,  -223,  -223,   735,   298,   735,    13,  -223,
     166,   309,   735,    38,  -223,   358,   261,  -223,  -223,   735,
    -223,   129,   229,  -223,  -223,   335,  -223,   338,   339,   345,
      21,  -223,   423,   424,   195,   393,   364,  -223,   173,  -223,
     735,  -223,   261,  -223,  -223,   405,   312,   316,   229,   436,
      24,   182,  -223,   190,   415,   157,  -223,   317,   326,  -223,
     360,   324,   856,  -223,   369,   229,  -223,  -223,   151,  -223,
    -223,   385,  -223,  -223,  -223,   735,   327,   246,   647,  -223,
     261,   365,  -223,  -223,   856,   340,   261,   735,  -223,    26,
    -223,  -223,  -223,  -223,  -223,    21,   111,   366,   367,  -223,
     735,   405,   368,  -223,   261,    12,   229,   229,   192,  -223,
    -223,  -223,  -223,  -223,  -223,  -223,   153,  -223,   229,  -223,
    -223,  -223,  -223,   341,    58,   422,   371,  -223,   405,  -223,
    -223,   344,  -223,   250,   647,  -223,   735,   198,  -223,  -223,
     856,   261,  -223,   381,  -223,  -223,   337,   384,   431,   389,
    -223,   204,   207,  -223,   468,    24,  -223,   229,  -223,  -223,
     350,   435,  -223,    30,   229,   735,   210,   261,  -223,   213,
     405,   735,   469,  -223,   380,  -223,  -223,  -223,   225,  -223,
    -223,  -223,  -223,    11,   229,   118,  -223,   352,   261,  -223,
    -223,   384,   353,  -223,   186,  -223,   229,  -223,   229,  -223,
    -223,   229,  -223,   227,  -223,  -223,   356,  -223,   735,  -223,
    -223,   397,   361,  -223,   247,  -223,   229,  -223,   -13,  -223,
     229,  -223,   252,  -223,  -223,   257,   392,  -223,   479,  -223
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     6,   254,     0,     0,     0,     0,     0,     0,    18,
     112,     0,     0,     7,     0,     0,    15,     8,    10,    11,
      13,    14,     9,    17,    12,    16,     0,   105,     0,   252,
       0,   246,   247,     0,     0,     0,     0,     0,     0,   113,
     114,     0,     0,   107,   108,     0,   145,     1,     3,     2,
       0,   106,     5,     4,   253,     0,     0,     0,     0,   166,
      25,     0,   219,   216,     0,   238,   115,    40,    29,     0,
       0,    30,    31,    34,    36,     0,    37,    39,     0,    41,
     215,    35,    38,    32,    33,     0,     0,     0,     0,     0,
     116,   117,   121,   187,   189,   191,   194,   195,   196,   193,
     192,     0,   224,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,     0,   101,   167,     0,     0,    91,   217,
     218,     0,     0,   211,   208,     0,    43,     0,   220,     0,
      44,     0,   221,     0,   166,     0,   239,   240,     0,     0,
     120,   242,   243,   241,     0,     0,   190,     0,     0,   166,
     103,     0,   109,     0,   110,     0,   244,     0,     0,     0,
       0,     0,     0,    93,    66,    27,     0,     0,     0,     0,
       0,   168,   170,   172,   174,     0,   192,     0,     0,     0,
       0,   211,   205,     0,   209,     0,     0,     0,     0,   197,
       0,     0,   147,   136,   122,   141,   124,   149,   118,   119,
     186,   188,   225,     0,     0,   198,   213,     0,     0,   100,
       0,     0,   146,     0,    92,    19,     0,     0,     0,     0,
      20,    21,    22,     0,     0,     0,    64,     0,    42,    56,
     173,   181,     0,     0,     0,     0,     0,   228,   230,   231,
     232,   233,   229,   234,   236,     0,     0,     0,     0,   222,
       0,     0,     0,     0,   206,     0,   212,   204,    45,     0,
      46,   127,     0,   137,   143,   133,   128,   129,   131,     0,
       0,   140,     0,     0,   139,     0,   151,   199,     0,   200,
       0,   102,   104,   123,   245,     0,     0,     0,     0,     0,
       0,     0,   226,     0,   224,     0,    63,    65,    68,    28,
       0,     0,     0,    47,     0,     0,    49,    55,    57,    26,
     180,   169,   171,   235,   237,     0,     0,     0,     0,   182,
     179,     0,   178,    90,     0,     0,   210,     0,   203,     0,
     142,   144,   134,   130,   132,     0,   148,     0,     0,   138,
       0,     0,   153,   201,   214,     0,     0,     0,     0,    96,
     250,   251,   249,   248,    97,    95,     0,    67,     0,    83,
      84,    85,    86,    87,     0,     0,    70,    48,     0,    51,
      50,     0,    54,     0,     0,   184,     0,     0,   177,   223,
       0,   207,   202,     0,   125,   126,   150,   152,     0,   155,
      61,     0,     0,    58,     0,     0,   227,     0,    24,    62,
       0,     0,    23,     0,     0,     0,     0,   175,   183,     0,
       0,     0,     0,   111,     0,    59,    98,    99,     0,    74,
      76,    77,    78,     0,     0,     0,    52,     0,   176,   185,
      89,   135,   154,   157,   160,   156,     0,    88,     0,    82,
      80,     0,    79,     0,    72,    73,     0,    53,     0,   161,
     162,   163,     0,    75,     0,    69,     0,   158,     0,   159,
       0,    81,     0,   164,   165,     0,     0,    60,     0,    71
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,  -121,  -223,
     321,   183,  -223,  -223,  -222,  -223,  -223,  -223,  -223,  -223,
    -223,    68,    49,  -223,  -223,  -223,  -223,  -223,  -223,  -223,
    -223,  -223,  -223,  -223,  -223,   285,  -223,  -223,  -223,   390,
       9,  -223,  -223,  -223,   370,  -223,  -100,  -223,  -223,  -149,
     160,  -143,    -9,  -223,  -223,  -223,  -223,  -223,  -223,    53,
    -223,  -223,   107,  -223,  -120,   262,   268,   342,   -30,   372,
     362,   403,  -123,  -223,  -223,  -223,   347,  -223,   394,   355,
    -192,  -161,   127,  -107,  -223,  -223,  -223,  -223,  -223,  -115,
      -4,   113,  -223,  -223
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    14,    15,    16,    17,    18,    19,    20,   165,   166,
      88,   307,   308,   309,   220,   297,   298,   225,   366,   402,
     446,   418,   419,   420,   421,   422,   363,   398,    21,    22,
     163,   291,    23,    24,   149,   150,    25,    26,    43,    44,
     211,    41,    89,    90,    91,   134,   191,   274,   269,   192,
     193,   263,   264,   194,   276,   342,   389,   413,   432,   433,
     451,   459,   114,   115,   171,   172,   173,   174,   175,    93,
      94,    95,    96,    97,    98,   181,   182,   123,   124,   185,
     207,    99,   250,   100,   293,   247,   101,   139,   144,   155,
     102,   354,    28,    29
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      33,   179,   178,    45,   296,   154,   176,    42,    46,    27,
     195,    92,   278,   234,   176,    31,   249,    32,   439,    31,
     234,    32,   147,   321,   234,    31,    55,    32,   136,   137,
      57,    58,    59,    60,    61,    51,   183,   215,   234,   122,
     440,   261,   313,   314,   315,   141,   142,   136,   137,    30,
     232,   463,   271,   113,    37,   108,   183,   131,    34,   136,
     137,   176,   208,   176,     1,    39,     2,   195,   464,   231,
     118,    36,   136,   137,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   109,   136,   137,   350,   140,   272,
      35,    40,    38,     3,   121,    45,    47,   316,   322,   151,
      46,   216,   156,   157,    54,    92,   299,   164,   167,     4,
       5,   351,   352,   156,   180,     6,    10,   292,   206,   148,
       7,   336,   252,   187,   217,   103,   377,   176,   176,   196,
     310,   339,   189,   353,   199,   441,   246,   390,     8,   190,
     233,   369,   399,   202,   327,   190,   319,   195,   386,   143,
     253,   382,   218,   256,   167,   426,   221,   222,     9,   158,
     159,   300,   273,   379,    10,   345,   214,   180,    52,   219,
     289,    53,    50,   348,   206,    11,   148,   290,   176,   265,
     282,    12,   406,    13,   106,   266,   196,    46,   444,   189,
    -127,    46,   359,   267,   301,   360,   361,   265,    10,    31,
      56,    32,   302,   266,   151,   104,   394,   136,   137,   284,
     268,   267,   195,   395,   445,   317,   375,   320,   449,   249,
     294,   387,   326,   167,    10,    31,   262,    32,   268,   329,
     105,   391,   392,    31,   176,    32,   303,   116,   160,   161,
     450,   197,   136,   137,    48,   304,    49,   117,   403,   305,
     344,   396,   138,   331,   330,   376,   209,   107,    46,   405,
     362,   176,   306,   212,   213,   110,   196,   136,   137,   111,
      46,   136,   137,   226,   227,   113,    31,    62,    32,    63,
     251,   213,   136,   137,   156,   373,   279,   280,   206,   112,
     431,   323,   324,    64,    65,   203,   119,   381,   343,   280,
     120,   371,   125,   176,   127,    67,    68,   355,   356,   126,
     206,   128,   129,    69,    70,   357,   358,   393,   213,   130,
      71,    72,    73,   408,   280,   132,   454,   204,    74,   414,
     213,   196,   415,   213,    75,   429,   280,    76,   430,   324,
     133,   462,   156,   156,   206,   465,   407,   153,    77,    78,
     437,   438,   455,   438,   294,   162,    79,    80,   177,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,    81,
     136,   137,   461,   213,   135,   428,    82,   466,   213,    83,
      84,   434,   467,   213,   186,   188,   210,    85,    10,   223,
     224,    86,   234,   423,   235,   257,    87,   205,   259,   248,
     427,   258,   260,   275,   270,   277,   283,   285,   286,    31,
      62,    32,    63,   287,   288,   168,   295,   328,   434,   442,
     423,   246,   318,   325,   335,   332,    64,    65,   333,   334,
     337,   338,   452,   340,   423,   341,   346,   156,    67,    68,
     347,   349,   147,   364,   365,   367,    69,    70,   368,   370,
     378,   374,   156,    71,    72,    73,   156,   388,   400,   384,
     385,    74,   401,   280,   380,   397,   169,    75,   404,   410,
      76,   411,   412,   416,   424,   425,   435,   447,   436,   448,
     456,    77,    78,   458,   468,   460,   469,   453,   229,    79,
      80,   372,   443,   281,   152,   383,   311,    31,    62,    32,
      63,   457,    81,   312,   146,   198,   201,   409,   417,    82,
     230,   200,    83,    84,    64,    65,    66,   184,     0,     0,
      85,     0,     0,     0,    86,     0,    67,    68,   254,   170,
      31,    62,    32,    63,    69,    70,   255,     0,     0,     0,
       0,    71,    72,    73,     0,     0,     0,    64,    65,    74,
       0,     0,     0,     0,     0,    75,     0,     0,    76,    67,
      68,     0,     0,     0,     0,     0,     0,    69,    70,    77,
      78,     0,     0,     0,    71,    72,    73,    79,    80,     0,
       0,     0,    74,     0,     0,     0,     0,     0,    75,     0,
      81,    76,     0,     0,     0,     0,     0,    82,     0,     0,
      83,    84,    77,    78,     0,     0,     0,     0,    85,     0,
      79,    80,    86,     0,     0,     0,     0,    87,    31,    62,
      32,    63,     0,    81,     0,     0,     0,     0,     0,     0,
      82,     0,     0,    83,    84,    64,    65,     0,     0,     0,
       0,    85,   121,     0,     0,    86,     0,    67,    68,     0,
      87,    31,    62,    32,    63,    69,    70,     0,     0,     0,
       0,     0,    71,    72,    73,     0,     0,     0,    64,    65,
      74,     0,     0,     0,     0,   169,    75,     0,     0,    76,
      67,    68,     0,     0,     0,     0,     0,     0,    69,    70,
      77,    78,     0,     0,     0,    71,    72,    73,    79,    80,
       0,     0,     0,    74,     0,     0,     0,     0,     0,    75,
       0,    81,    76,     0,     0,     0,     0,     0,    82,     0,
       0,    83,    84,    77,    78,     0,     0,     0,     0,    85,
       0,    79,    80,    86,     0,     0,     0,     0,   170,    31,
      62,    32,    63,     0,    81,     0,     0,     0,    10,     0,
       0,    82,     0,     0,    83,    84,    64,    65,     0,     0,
       0,     0,    85,     0,     0,     0,    86,     0,    67,    68,
       0,    87,    31,    62,    32,    63,    69,    70,     0,     0,
       0,     0,     0,    71,    72,    73,     0,     0,     0,    64,
     145,    74,     0,     0,     0,     0,     0,    75,     0,     0,
      76,    67,    68,     0,     0,     0,     0,     0,     0,    69,
      70,    77,    78,     0,     0,     0,    71,    72,    73,    79,
      80,     0,     0,     0,    74,     0,     0,     0,     0,     0,
      75,     0,    81,    76,     0,     0,     0,     0,     0,    82,
       0,     0,    83,    84,    77,    78,     0,     0,     0,     0,
      85,     0,    79,    80,    86,     0,     0,     0,     0,    87,
       0,    62,     0,    63,     0,    81,     0,     0,     0,     0,
       0,     0,    82,     0,     0,    83,    84,    64,   145,     0,
       0,     0,     0,    85,    67,    68,     0,    86,     0,    67,
      68,     0,    87,    70,     0,     0,     0,     0,    70,    71,
      72,    73,     0,     0,    71,    72,    73,    74,     0,     0,
       0,     0,    74,     0,     0,     0,    76,     0,     0,     0,
       0,    76,     0,     0,     0,     0,     0,    77,   228,     0,
       0,     0,    77,    78,     0,    79,     0,     0,     0,     0,
      79,    80,     0,     0,     0,     0,     0,     0,    81,     0,
       0,     0,     0,    81,     0,    82,     0,     0,    83,    84,
      82,     0,     0,    83,    84,     0,    85,     0,     0,     0,
      86,    85,     0,     0,     0,    86
};

static const yytype_int16 yycheck[] =
{
       4,   121,   117,    12,   226,   105,   113,    11,    12,     0,
     133,    41,   204,     8,   121,     4,   177,     6,     7,     4,
       8,     6,    27,    10,     8,     4,    30,     6,    21,    22,
      34,    35,    36,    37,    38,    26,    58,   158,     8,    69,
      29,   190,    17,    18,    19,    23,    24,    21,    22,   105,
     170,    64,   195,   117,   105,    28,    58,    87,    73,    21,
      22,   168,   126,   170,     1,    29,     3,   190,    81,   169,
      61,    67,    21,    22,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    57,    21,    22,    63,    92,    37,
     105,    55,    78,    30,   116,   104,     0,    72,    85,   103,
     104,    43,   106,   107,     3,   135,   227,   111,   112,    46,
      47,    87,    88,   117,   116,    52,   101,   224,   148,   124,
      57,   270,   106,   127,    66,   102,   318,   234,   235,   133,
     125,   274,   125,   109,   138,   124,    72,   125,    75,   124,
     170,   302,   364,   147,   106,   124,   246,   270,   340,   127,
     180,   125,    94,   183,   158,   125,   160,   161,    95,    44,
      45,    10,   110,   324,   101,   285,   157,   116,   120,   111,
      53,   123,    75,   288,   204,   112,   124,    60,   285,    68,
     210,   118,   374,   120,   124,    74,   190,   191,    70,   125,
      79,   195,    35,    82,    43,    38,    39,    68,   101,     4,
      67,     6,    51,    74,   208,   126,    53,    21,    22,   213,
      99,    82,   335,    60,    96,   245,   316,   247,    32,   380,
     224,   341,   252,   227,   101,     4,    31,     6,    99,   259,
      31,   346,   347,     4,   341,     6,    85,   114,    44,    45,
      54,   134,    21,    22,   120,    94,   122,   124,   368,    98,
     280,   358,    31,   262,   125,     9,   149,    78,   262,     9,
     103,   368,   111,   125,   126,     5,   270,    21,    22,    88,
     274,    21,    22,   125,   126,   117,     4,     5,     6,     7,
     125,   126,    21,    22,   288,   315,   125,   126,   318,   124,
     410,   125,   126,    21,    22,    23,     7,   327,   125,   126,
       7,   305,   124,   410,   124,    33,    34,   125,   126,    77,
     340,     5,   124,    41,    42,   125,   126,   125,   126,    77,
      48,    49,    50,   125,   126,     5,   441,    55,    56,   125,
     126,   335,   125,   126,    62,   125,   126,    65,   125,   126,
      67,   456,   346,   347,   374,   460,   376,   124,    76,    77,
     125,   126,   125,   126,   358,   118,    84,    85,   124,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    97,
      21,    22,   125,   126,   126,   405,   104,   125,   126,   107,
     108,   411,   125,   126,     7,     7,    11,   115,   101,   124,
     124,   119,     8,   397,     9,    59,   124,   125,    67,    20,
     404,   125,   125,    69,   126,   125,   125,   124,    80,     4,
       5,     6,     7,    80,   124,    10,   113,    59,   448,   423,
     424,    72,   124,   114,    79,    90,    21,    22,    90,    90,
       7,     7,   436,    40,   438,    71,   124,   441,    33,    34,
     124,     5,    27,   126,   118,    85,    41,    42,   124,    80,
      85,   124,   456,    48,    49,    50,   460,    89,    36,    93,
      93,    56,    91,   126,   124,   124,    61,    62,   124,    88,
      65,    40,    83,     5,   124,    40,     7,   125,    98,   126,
     124,    76,    77,    86,    92,   124,     7,   438,   167,    84,
      85,   308,   424,   208,   104,   335,   234,     4,     5,     6,
       7,   448,    97,   235,   101,   135,   144,   380,   395,   104,
     168,   139,   107,   108,    21,    22,    23,   123,    -1,    -1,
     115,    -1,    -1,    -1,   119,    -1,    33,    34,   181,   124,
       4,     5,     6,     7,    41,    42,   181,    -1,    -1,    -1,
      -1,    48,    49,    50,    -1,    -1,    -1,    21,    22,    56,
      -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    65,    33,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    41,    42,    76,
      77,    -1,    -1,    -1,    48,    49,    50,    84,    85,    -1,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      97,    65,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
     107,   108,    76,    77,    -1,    -1,    -1,    -1,   115,    -1,
      84,    85,   119,    -1,    -1,    -1,    -1,   124,     4,     5,
       6,     7,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,   107,   108,    21,    22,    -1,    -1,    -1,
      -1,   115,   116,    -1,    -1,   119,    -1,    33,    34,    -1,
     124,     4,     5,     6,     7,    41,    42,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    -1,    -1,    -1,    21,    22,
      56,    -1,    -1,    -1,    -1,    61,    62,    -1,    -1,    65,
      33,    34,    -1,    -1,    -1,    -1,    -1,    -1,    41,    42,
      76,    77,    -1,    -1,    -1,    48,    49,    50,    84,    85,
      -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    97,    65,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,   107,   108,    76,    77,    -1,    -1,    -1,    -1,   115,
      -1,    84,    85,   119,    -1,    -1,    -1,    -1,   124,     4,
       5,     6,     7,    -1,    97,    -1,    -1,    -1,   101,    -1,
      -1,   104,    -1,    -1,   107,   108,    21,    22,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,   119,    -1,    33,    34,
      -1,   124,     4,     5,     6,     7,    41,    42,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    -1,    -1,    -1,    21,
      22,    56,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,
      65,    33,    34,    -1,    -1,    -1,    -1,    -1,    -1,    41,
      42,    76,    77,    -1,    -1,    -1,    48,    49,    50,    84,
      85,    -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,
      62,    -1,    97,    65,    -1,    -1,    -1,    -1,    -1,   104,
      -1,    -1,   107,   108,    76,    77,    -1,    -1,    -1,    -1,
     115,    -1,    84,    85,   119,    -1,    -1,    -1,    -1,   124,
      -1,     5,    -1,     7,    -1,    97,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,   107,   108,    21,    22,    -1,
      -1,    -1,    -1,   115,    33,    34,    -1,   119,    -1,    33,
      34,    -1,   124,    42,    -1,    -1,    -1,    -1,    42,    48,
      49,    50,    -1,    -1,    48,    49,    50,    56,    -1,    -1,
      -1,    -1,    56,    -1,    -1,    -1,    65,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    -1,    -1,    -1,    76,    77,    -1,
      -1,    -1,    76,    77,    -1,    84,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    97,    -1,   104,    -1,    -1,   107,   108,
     104,    -1,    -1,   107,   108,    -1,   115,    -1,    -1,    -1,
     119,   115,    -1,    -1,    -1,   119
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,    30,    46,    47,    52,    57,    75,    95,
     101,   112,   118,   120,   129,   130,   131,   132,   133,   134,
     135,   156,   157,   160,   161,   164,   165,   168,   220,   221,
     105,     4,     6,   218,    73,   105,    67,   105,    78,    29,
      55,   169,   218,   166,   167,   180,   218,     0,   120,   122,
      75,   168,   120,   123,     3,   218,    67,   218,   218,   218,
     218,   218,     5,     7,    21,    22,    23,    33,    34,    41,
      42,    48,    49,    50,    56,    62,    65,    76,    77,    84,
      85,    97,   104,   107,   108,   115,   119,   124,   138,   170,
     171,   172,   196,   197,   198,   199,   200,   201,   202,   209,
     211,   214,   218,   102,   126,    31,   124,    78,    28,    57,
       5,    88,   124,   117,   190,   191,   114,   124,   168,     7,
       7,   116,   196,   205,   206,   124,    77,   124,     5,   124,
      77,   196,     5,    67,   173,   126,    21,    22,    31,   215,
     218,    23,    24,   127,   216,    22,   199,    27,   124,   162,
     163,   218,   167,   124,   174,   217,   218,   218,    44,    45,
      44,    45,   118,   158,   218,   136,   137,   218,    10,    61,
     124,   192,   193,   194,   195,   196,   211,   124,   217,   192,
     116,   203,   204,    58,   206,   207,     7,   218,     7,   125,
     124,   174,   177,   178,   181,   200,   218,   190,   172,   218,
     197,   198,   218,    23,    55,   125,   196,   208,   126,   190,
      11,   168,   125,   126,   168,   136,    43,    66,    94,   111,
     142,   218,   218,   124,   124,   145,   125,   126,    77,   138,
     195,   174,   192,   196,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    72,   213,    20,   209,
     210,   125,   106,   196,   204,   207,   196,    59,   125,    67,
     125,   177,    31,   179,   180,    68,    74,    82,    99,   176,
     126,   179,    37,   110,   175,    69,   182,   125,   208,   125,
     126,   163,   196,   125,   218,   124,    80,    80,   124,    53,
      60,   159,   211,   212,   218,   113,   142,   143,   144,   136,
      10,    43,    51,    85,    94,    98,   111,   139,   140,   141,
     125,   193,   194,    17,    18,    19,    72,   196,   124,   174,
     196,    10,    85,   125,   126,   114,   196,   106,    59,   196,
     125,   180,    90,    90,    90,    79,   177,     7,     7,   179,
      40,    71,   183,   125,   196,   192,   124,   124,   217,     5,
      63,    87,    88,   109,   219,   125,   126,   125,   126,    35,
      38,    39,   103,   154,   126,   118,   146,    85,   124,   209,
      80,   218,   139,   196,   124,   174,     9,   208,    85,   209,
     124,   196,   125,   178,    93,    93,   208,   192,    89,   184,
     125,   217,   217,   125,    53,    60,   211,   124,   155,   142,
      36,    91,   147,   192,   124,     9,   208,   196,   125,   210,
      88,    40,    83,   185,   125,   125,     5,   219,   149,   150,
     151,   152,   153,   218,   124,    40,   125,   218,   196,   125,
     125,   192,   186,   187,   196,     7,    98,   125,   126,     7,
      29,   124,   218,   149,    70,    96,   148,   125,   126,    32,
      54,   188,   218,   150,   217,   125,   124,   187,    86,   189,
     124,   125,   217,    64,    81,   217,   125,   125,    92,     7
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   128,   129,   129,   129,   129,   129,   129,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   131,   132,
     132,   132,   132,   133,   134,   135,   136,   137,   137,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   139,   139,   139,
     139,   139,   139,   139,   140,   140,   141,   141,   142,   142,
     142,   142,   143,   143,   144,   144,   145,   145,   146,   146,
     147,   147,   148,   148,   149,   149,   150,   150,   150,   151,
     151,   152,   153,   154,   154,   154,   154,   155,   155,   156,
     156,   156,   156,   157,   158,   158,   159,   159,   159,   159,
     160,   161,   162,   162,   163,   164,   164,   165,   166,   166,
     167,   168,   169,   169,   169,   170,   170,   171,   171,   172,
     172,   172,   173,   174,   175,   175,   175,   176,   176,   176,
     176,   176,   176,   176,   176,   177,   177,   178,   178,   178,
     178,   178,   178,   179,   179,   180,   180,   181,   181,   182,
     182,   183,   183,   184,   184,   185,   185,   186,   186,   187,
     188,   188,   188,   189,   189,   189,   190,   190,   191,   192,
     192,   193,   193,   194,   194,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   196,   196,   197,   197,
     198,   198,   199,   199,   199,   199,   199,   199,   200,   200,
     200,   200,   201,   202,   202,   203,   203,   204,   205,   205,
     206,   207,   207,   208,   208,   209,   209,   209,   209,   209,
     209,   209,   210,   210,   211,   211,   212,   212,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   214,   215,
     215,   216,   216,   216,   217,   217,   218,   218,   219,   219,
     219,   219,   220,   221,   221
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
       2,     1,     1,     1,     1,     1,     1,     3,     3,     4,
       4,     5,     6,     5,     4,     1,     2,     4,     1,     2,
       4,     0,     2,     1,     3,     1,     1,     2,     2,     1,
       2,     2,     1,     3,     1,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     2,     2,     0
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
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1836 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_NAME  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1846 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1856 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1866 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 7: /* TOKEN_UNSIGNED_NUMVAL  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 1876 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 130: /* sql_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1886 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 131: /* quit_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 1896 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 132: /* alter_table_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1906 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 133: /* create_table_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 1916 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 134: /* create_index_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1926 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 135: /* drop_table_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 1936 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 136: /* column_def  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 1946 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 137: /* column_def_commalist  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 1956 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 138: /* data_type  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 1966 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 139: /* column_constraint_def  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 1976 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 140: /* column_constraint_def_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1986 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 141: /* opt_column_constraint_def_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1996 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 145: /* opt_column_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2006 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 146: /* opt_block_properties  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).block_properties_) != nullptr) {
    delete ((*yyvaluep).block_properties_);
  }
}
#line 2016 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 147: /* opt_partition_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).partition_clause_) != nullptr) {
    delete ((*yyvaluep).partition_clause_);
  }
}
#line 2026 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 148: /* partition_type  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2036 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 149: /* key_value_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2046 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 150: /* key_value  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 2056 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 151: /* key_string_value  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 2066 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 152: /* key_string_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 2076 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 153: /* key_integer_value  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 2086 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* index_type  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2096 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* opt_index_properties  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2106 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* insert_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 2116 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* copy_from_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_statement_) != nullptr) {
    delete ((*yyvaluep).copy_from_statement_);
  }
}
#line 2126 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* opt_copy_from_params  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2136 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* copy_from_params  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2146 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* update_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2156 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* delete_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2166 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* assignment_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2176 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 163: /* assignment_item  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2186 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 164: /* select_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_statement_) != nullptr) {
    delete ((*yyvaluep).select_statement_);
  }
}
#line 2196 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 165: /* with_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2206 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 166: /* with_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2216 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 167: /* with_list_element  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2226 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* select_query  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2236 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* selection  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2246 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* selection_item_commalist  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2256 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* selection_item  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2266 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* from_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2276 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* subquery_expression  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2286 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* opt_sample_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_sample_clause_) != nullptr) {
    delete ((*yyvaluep).opt_sample_clause_);
  }
}
#line 2296 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* join_type  */
#line 564 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2302 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* joined_table_reference  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2312 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* table_reference  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2322 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* table_reference_signature  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2332 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* table_reference_signature_primary  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2342 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* joined_table_reference_commalist  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2352 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* opt_group_by_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2362 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* opt_having_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2372 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* opt_order_by_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2382 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* opt_limit_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2392 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* order_commalist  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2402 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* order_item  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2412 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* opt_order_direction  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2422 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* opt_nulls_first  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2432 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 190: /* opt_where_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2442 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 191: /* where_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2452 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 192: /* or_expression  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2462 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 193: /* and_expression  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2472 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 194: /* not_expression  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2482 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 195: /* predicate_expression_base  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2492 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 196: /* add_expression  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2502 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 197: /* multiply_expression  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2512 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 198: /* unary_expression  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2522 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 199: /* expression_base  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2532 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 200: /* function_call  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2542 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 201: /* extract_function  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2552 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 202: /* case_expression  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2562 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 203: /* simple_when_clause_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_list_);
  }
}
#line 2572 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 204: /* simple_when_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_);
  }
}
#line 2582 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 205: /* searched_when_clause_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_list_);
  }
}
#line 2592 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 206: /* searched_when_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_);
  }
}
#line 2602 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 207: /* opt_else_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2612 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 208: /* expression_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2622 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 209: /* literal_value  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2632 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 210: /* literal_value_commalist  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2642 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 211: /* attribute_ref  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2652 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 212: /* attribute_ref_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2662 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 213: /* comparison_operation  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2668 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 214: /* unary_operation  */
#line 562 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2674 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 215: /* add_operation  */
#line 563 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2680 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 216: /* multiply_operation  */
#line 563 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2686 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 217: /* name_commalist  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 2696 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 218: /* any_name  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2706 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 219: /* boolean_value  */
#line 560 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2712 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 220: /* command  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_) != nullptr) {
    delete ((*yyvaluep).command_);
  }
}
#line 2722 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 221: /* command_argument_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_argument_list_) != nullptr) {
    delete ((*yyvaluep).command_argument_list_);
  }
}
#line 2732 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 575 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3029 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 579 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3038 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 583 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3047 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 5:
#line 587 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3056 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 591 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    YYABORT;
  }
#line 3064 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 594 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 3073 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 601 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3081 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 9:
#line 604 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].copy_from_statement_);
  }
#line 3089 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 10:
#line 607 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 3097 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 11:
#line 610 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3105 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 12:
#line 613 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 3113 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 13:
#line 616 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 3121 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 14:
#line 619 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 3129 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 15:
#line 622 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 3137 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 16:
#line 625 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].select_statement_);
  }
#line 3145 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 17:
#line 628 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 3153 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 18:
#line 634 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3161 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 19:
#line 640 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3173 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 20:
#line 647 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3184 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 653 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3196 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 660 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3208 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 669 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].attribute_definition_list_), (yyvsp[-1].block_properties_), (yyvsp[0].partition_clause_));
  }
#line 3216 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 674 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].key_value_list_)) {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_), (yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].key_value_list_));
    } else {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_));
    }
  }
#line 3228 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 683 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 3236 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 688 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 3244 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 693 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3253 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 697 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3262 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 703 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 3272 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 708 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3280 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 711 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3288 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 714 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3298 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 719 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3306 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 722 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3314 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 725 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3322 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 728 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3330 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 731 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3338 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 734 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3346 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 737 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3354 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 740 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3362 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 743 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3370 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 746 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3387 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 758 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3395 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 761 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3403 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 764 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3426 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 782 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3449 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 802 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3457 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 805 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3465 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 808 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3475 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 813 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3485 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 818 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3496 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 824 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3507 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 830 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3519 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 839 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3528 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 843 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3537 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 849 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3545 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 852 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3553 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 857 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3563 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 862 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3573 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 867 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3585 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 874 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3595 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 881 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3604 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 885 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3613 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 891 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3621 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 894 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = $1; */
  }
#line 3629 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 899 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = nullptr;
  }
#line 3637 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 902 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-1].attribute_list_);
  }
#line 3645 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 907 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = nullptr;
  }
#line 3653 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 910 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].key_value_list_));
  }
#line 3661 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 915 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.partition_clause_) = nullptr;
  }
#line 3669 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 918 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3691 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 937 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::PartitionSchemeHeader::PartitionType::kHash));
  }
#line 3700 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 941 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::PartitionSchemeHeader::PartitionType::kRange));
  }
#line 3709 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 947 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3718 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 951 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3727 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 957 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 3735 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 960 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 3743 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 963 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 3751 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 968 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 3759 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 971 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_),
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 3769 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 978 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 3777 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 983 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 3791 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 994 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Defaults to BitWeavingV, but IndexProperties can change this to H.
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBitWeavingV));
  }
#line 3801 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 999 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
#line 3810 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 1003 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
#line 3819 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 1007 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kSMA));
  }
#line 3828 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 1013 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 3836 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 1016 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 3844 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 1022 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 3857 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 1030 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertTuple((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 3865 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 1033 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-3]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), nullptr);
  }
#line 3873 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 1036 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-4]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), (yyvsp[-4].with_list_));
  }
#line 3881 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 1042 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_statement_) = new quickstep::ParseStatementCopyFrom((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].copy_from_params_));
  }
#line 3889 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 1047 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = nullptr;
  }
#line 3897 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 1050 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-1].copy_from_params_);
  }
#line 3905 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 1055 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3914 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 1059 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3923 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 1063 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3932 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 1067 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3941 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 1073 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 3949 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 1078 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 3957 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 1083 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3966 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 1087 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3975 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 1093 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 3983 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 1099 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].select_query_), nullptr);
  }
#line 3991 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 1102 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].select_query_), (yyvsp[-1].with_list_));
  }
#line 3999 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 1107 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 4007 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 1112 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4016 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 1116 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4025 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 1122 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 4034 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 1129 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].selection_), (yyvsp[-5].table_reference_list_), (yyvsp[-4].predicate_), (yyvsp[-3].opt_group_by_clause_), (yyvsp[-2].opt_having_clause_), (yyvsp[-1].opt_order_by_clause_), (yyvsp[0].opt_limit_clause_));
  }
#line 4042 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 1134 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 4050 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 1137 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "ALL in selection");
    YYERROR;
  }
#line 4059 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 1141 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "DISTINCT in selection");
    YYERROR;
  }
#line 4068 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 1147 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4076 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 1150 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 4084 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 1155 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4093 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 1159 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4102 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 1165 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 4110 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 1168 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 4118 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 1171 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 4126 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 1176 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[0].table_reference_list_);
  }
#line 4134 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 1181 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].select_query_));
  }
#line 4142 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 1186 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = NULL;
  }
#line 4150 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 1189 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, true, (yyvsp[-1].numeric_literal_value_));
  }
#line 4158 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 1192 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].numeric_literal_value_));
  }
#line 4166 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 1197 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4174 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 1200 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4182 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 1203 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4190 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 1206 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4198 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 1209 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4206 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 1212 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4214 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 1215 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4222 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 1218 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4230 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 1223 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseJoinedTableReference((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].join_type_), (yyvsp[-5].table_reference_), (yyvsp[-2].table_reference_), (yyvsp[0].predicate_));
  }
#line 4238 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 1226 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = (yyvsp[0].table_reference_);
  }
#line 4246 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 1231 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4255 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 1235 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].opt_sample_clause_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4264 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 1239 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].opt_sample_clause_));
  }
#line 4272 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 1242 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4281 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 1246 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].function_call_));
  }
#line 4289 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 1249 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = (yyvsp[-1].table_reference_);
  }
#line 4297 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 1254 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4305 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 1257 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4313 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 1262 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4321 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 1265 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4329 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 1270 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4338 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 1274 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4347 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 1280 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 4355 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 1283 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4363 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 1288 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4371 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 1291 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4379 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 1296 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4387 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 1299 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4395 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 1304 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4403 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 1307 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4425 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 1326 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4434 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 1330 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4443 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 1336 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 4453 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 1343 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4461 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 1346 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4469 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 1349 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4477 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 1354 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4485 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 1357 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4493 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 1360 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4501 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 1366 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 4509 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 1369 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4517 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 1374 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4525 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 1379 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4539 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 1388 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4547 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 1393 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4561 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 1402 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4569 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 1407 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4577 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 1410 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4585 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 1415 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4593 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 1418 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 4603 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 1423 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4614 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 1429 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4625 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 1435 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4633 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 1438 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 4641 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 1441 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateExists((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].subquery_expression_));
  }
#line 4649 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 1444 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-2].expression_), (yyvsp[0].subquery_expression_));
  }
#line 4657 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 1447 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-1].expression_list_));
  }
#line 4665 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 1450 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-2]).first_line,
        (yylsp[-2]).first_column,
        new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-3].expression_), (yyvsp[0].subquery_expression_)));
  }
#line 4676 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 1456 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line,
        (yylsp[-4]).first_column,
        new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-1].expression_list_)));
  }
#line 4687 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1465 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4695 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 1468 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4703 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 1473 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4711 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 1476 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4719 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 1481 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].unary_operation_), (yyvsp[0].expression_));
  }
#line 4727 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 1484 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4735 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 1489 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 4743 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 1492 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 4751 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 1495 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 4759 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 1498 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4767 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 1501 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4775 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 1504 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 4783 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 1509 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 4792 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 1513 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 4801 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1517 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, false, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4809 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1520 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-4]).first_line, (yylsp[-4]).first_column, true, (yyvsp[-4].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4817 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1525 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseExtractFunction((yylsp[-5]).first_line, (yylsp[-5]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].expression_));
  }
#line 4825 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1530 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSimpleCaseExpression((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].expression_), (yyvsp[-2].simple_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 4833 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1533 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSearchedCaseExpression((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].searched_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 4841 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1538 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSimpleWhenClause>;
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 4850 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1542 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_list_) = (yyvsp[-1].simple_when_clause_list_);
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 4859 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1548 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_) = new quickstep::ParseSimpleWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4867 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1553 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSearchedWhenClause>;
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 4876 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1557 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_list_) = (yyvsp[-1].searched_when_clause_list_);
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 4885 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1563 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_) = new quickstep::ParseSearchedWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].predicate_), (yyvsp[0].expression_));
  }
#line 4893 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1568 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = NULL;
  }
#line 4901 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1571 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4909 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1576 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4918 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1580 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4927 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1586 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4935 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1589 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4943 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1592 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4951 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1595 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4968 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1607 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 4977 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1611 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5001 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1630 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5019 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1645 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5028 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1649 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5037 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1655 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 5045 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1658 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 5053 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1663 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = new quickstep::PtrList<quickstep::ParseAttribute>();
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5062 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1667 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-2].attribute_list_);
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5071 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1674 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 5079 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1677 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 5087 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1680 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 5095 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1683 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 5103 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1686 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 5111 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1689 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 5119 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1692 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLike);
  }
#line 5127 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1695 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotLike);
  }
#line 5135 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1698 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kRegexMatch);
  }
#line 5143 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1701 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotRegexMatch);
  }
#line 5151 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1706 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
#line 5165 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1717 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
#line 5173 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1720 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
#line 5181 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1725 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kModulo);
  }
#line 5189 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1728 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
#line 5197 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1731 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
#line 5205 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1737 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 5214 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1741 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 5223 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1747 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5231 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1750 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5242 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1758 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5250 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1761 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5258 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1764 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5266 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1767 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5274 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1773 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.command_) = new quickstep::ParseCommand((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].command_argument_list_));
  }
#line 5282 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1778 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    quickstep::PtrVector<quickstep::ParseString> *argument_list = (yyvsp[-1].command_argument_list_);
    argument_list->push_back((yyvsp[0].string_value_));
    (yyval.command_argument_list_) = argument_list;
  }
#line 5292 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1783 "../SqlParser.ypp" /* yacc.c:1661  */
    { /* Epsilon, an empy match. */
    (yyval.command_argument_list_) = new quickstep::PtrVector<quickstep::ParseString>();
  }
#line 5300 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;


#line 5304 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 1787 "../SqlParser.ypp" /* yacc.c:1906  */


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
