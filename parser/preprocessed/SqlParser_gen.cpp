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
#define YYLAST   1072

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  128
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  94
/* YYNRULES -- Number of rules.  */
#define YYNRULES  255
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  471

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
    1481,  1484,  1489,  1492,  1495,  1498,  1501,  1504,  1507,  1512,
    1516,  1520,  1523,  1528,  1533,  1536,  1541,  1545,  1551,  1556,
    1560,  1566,  1571,  1574,  1579,  1583,  1589,  1592,  1595,  1598,
    1610,  1614,  1633,  1648,  1652,  1658,  1661,  1666,  1670,  1677,
    1680,  1683,  1686,  1689,  1692,  1695,  1698,  1701,  1704,  1709,
    1720,  1723,  1728,  1731,  1734,  1740,  1744,  1750,  1753,  1761,
    1764,  1767,  1770,  1776,  1781,  1786
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

#define YYPACT_NINF -216

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-216)))

#define YYTABLE_NINF -128

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     198,  -216,  -216,   -46,   175,   -19,    46,   -27,    20,  -216,
      32,   175,   175,  -216,    88,   127,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,    -8,  -216,   115,   114,
     175,  -216,  -216,    54,   175,   175,   175,   175,   175,  -216,
    -216,   527,    52,    34,  -216,   135,    67,  -216,  -216,  -216,
     191,  -216,  -216,  -216,  -216,    37,   197,   117,   152,   180,
    -216,    86,  -216,  -216,   293,   294,  -216,  -216,  -216,   560,
     188,  -216,   236,  -216,  -216,   202,  -216,  -216,   312,  -216,
    -216,  -216,  -216,  -216,  -216,   212,   260,   648,   334,   274,
     214,  -216,  -216,   164,    -1,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,   769,    23,   175,   175,   224,   175,   175,    28,
     171,   233,   175,   175,   439,  -216,  -216,   228,   175,  -216,
    -216,  -216,   439,    27,   -17,  -216,   346,  -216,   175,  -216,
     347,  -216,   231,     7,  -216,    15,   180,   802,  -216,  -216,
     175,   802,  -216,  -216,  -216,  -216,   802,   294,  -216,   175,
     273,    56,  -216,   348,  -216,   272,  -216,   101,  -216,   272,
     175,   118,   175,   175,   250,  -216,   251,  -216,   111,   885,
     681,   224,   406,   370,   374,  -216,  -216,  1000,   359,   890,
     131,    17,   802,   -11,  -216,   802,  -216,   325,   261,   318,
     262,  -216,  -216,    40,    75,   124,  -216,   263,    75,   -13,
     321,  -216,  -216,    -1,  -216,  -216,   266,   802,  -216,   219,
     133,   175,  -216,   802,  -216,   175,  -216,  -216,   269,   315,
     316,   275,  -216,  -216,  -216,   144,   175,   287,   118,   175,
    -216,    82,  -216,  -216,    -3,   350,   439,   439,    58,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,   802,   278,   802,
      25,  -216,   137,   289,   802,    47,  -216,   345,   219,  -216,
    -216,   802,  -216,    89,   175,  -216,  -216,   317,  -216,   319,
     324,   326,    15,  -216,   399,   401,    75,   375,   352,  -216,
     139,  -216,   802,  -216,   219,  -216,   439,   300,   301,   175,
     412,     3,   141,  -216,   145,   391,    45,  -216,   303,   308,
    -216,   349,   306,   890,  -216,   351,   175,  -216,  -216,    82,
    -216,  -216,   374,  -216,  -216,  -216,   802,   313,   187,   648,
    -216,   219,   356,  -216,  -216,   890,   327,   219,   802,  -216,
      21,  -216,  -216,  -216,  -216,  -216,    15,   124,   339,   343,
    -216,   802,   439,   353,  -216,   219,    12,   175,   175,   149,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,   193,  -216,   175,
    -216,  -216,  -216,  -216,   328,   118,   414,   367,  -216,   439,
    -216,  -216,   335,  -216,   221,   648,  -216,   802,   156,  -216,
    -216,   890,   219,  -216,   376,  -216,  -216,   337,   370,   425,
     383,  -216,   158,   162,  -216,   464,     3,  -216,   175,  -216,
    -216,   354,   430,  -216,    19,   175,   802,   166,   219,  -216,
     179,   439,   802,   467,  -216,   378,  -216,  -216,  -216,   183,
    -216,  -216,  -216,  -216,    11,   175,   -14,  -216,   360,   219,
    -216,  -216,   370,   358,  -216,    31,  -216,   175,  -216,   175,
    -216,  -216,   175,  -216,   194,  -216,  -216,   355,  -216,   802,
    -216,  -216,   400,   368,  -216,   199,  -216,   175,  -216,    38,
    -216,   175,  -216,   205,  -216,  -216,   208,   385,  -216,   486,
    -216
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     6,   255,     0,     0,     0,     0,     0,     0,    18,
     112,     0,     0,     7,     0,     0,    15,     8,    10,    11,
      13,    14,     9,    17,    12,    16,     0,   105,     0,   253,
       0,   247,   248,     0,     0,     0,     0,     0,     0,   113,
     114,     0,     0,   107,   108,     0,   145,     1,     3,     2,
       0,   106,     5,     4,   254,     0,     0,     0,     0,   166,
      25,     0,   220,   217,     0,   239,   115,    40,    29,     0,
       0,    30,    31,    34,    36,     0,    37,    39,     0,    41,
     216,    35,    38,    32,    33,     0,     0,     0,     0,     0,
     116,   117,   198,   121,   187,   189,   191,   194,   195,   196,
     193,   192,     0,   225,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,     0,   101,   167,     0,     0,    91,
     218,   219,     0,     0,   212,   209,     0,    43,     0,   221,
       0,    44,     0,     0,   222,     0,   166,     0,   240,   241,
       0,     0,   120,   243,   244,   242,     0,     0,   190,     0,
       0,   166,   103,     0,   109,     0,   110,     0,   245,     0,
       0,     0,     0,     0,     0,    93,    66,    27,     0,     0,
       0,     0,     0,   168,   170,   172,   174,     0,   192,     0,
       0,     0,     0,   212,   206,     0,   210,     0,     0,     0,
       0,   123,   197,     0,     0,   147,   136,   122,   141,   124,
     149,   118,   119,   186,   188,   226,     0,     0,   199,   214,
       0,     0,   100,     0,   146,     0,    92,    19,     0,     0,
       0,     0,    20,    21,    22,     0,     0,     0,    64,     0,
      42,    56,   173,   181,     0,     0,     0,     0,     0,   229,
     231,   232,   233,   234,   230,   235,   237,     0,     0,     0,
       0,   223,     0,     0,     0,     0,   207,     0,   213,   205,
      45,     0,    46,   127,     0,   137,   143,   133,   128,   129,
     131,     0,     0,   140,     0,     0,   139,     0,   151,   200,
       0,   201,     0,   102,   104,   246,     0,     0,     0,     0,
       0,     0,     0,   227,     0,   225,     0,    63,    65,    68,
      28,     0,     0,     0,    47,     0,     0,    49,    55,    57,
      26,   180,   169,   171,   236,   238,     0,     0,     0,     0,
     182,   179,     0,   178,    90,     0,     0,   211,     0,   204,
       0,   142,   144,   134,   130,   132,     0,   148,     0,     0,
     138,     0,     0,   153,   202,   215,     0,     0,     0,     0,
      96,   251,   252,   250,   249,    97,    95,     0,    67,     0,
      83,    84,    85,    86,    87,     0,     0,    70,    48,     0,
      51,    50,     0,    54,     0,     0,   184,     0,     0,   177,
     224,     0,   208,   203,     0,   125,   126,   150,   152,     0,
     155,    61,     0,     0,    58,     0,     0,   228,     0,    24,
      62,     0,     0,    23,     0,     0,     0,     0,   175,   183,
       0,     0,     0,     0,   111,     0,    59,    98,    99,     0,
      74,    76,    77,    78,     0,     0,     0,    52,     0,   176,
     185,    89,   135,   154,   157,   160,   156,     0,    88,     0,
      82,    80,     0,    79,     0,    72,    73,     0,    53,     0,
     161,   162,   163,     0,    75,     0,    69,     0,   158,     0,
     159,     0,    81,     0,   164,   165,     0,     0,    60,     0,
      71
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -102,  -216,
     329,   185,  -216,  -216,  -215,  -216,  -216,  -216,  -216,  -216,
    -216,    71,    60,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,   286,  -216,  -216,  -216,   397,
      10,  -216,  -216,  -216,   369,  -216,   -97,  -216,  -216,  -138,
     169,  -161,   -10,  -216,  -216,  -216,  -216,  -216,  -216,    59,
    -216,  -216,   -91,  -216,  -121,   276,   280,   341,   -30,   377,
     363,   417,  -123,  -216,  -216,  -216,   344,  -216,   396,   357,
    -203,  -163,   147,  -108,  -216,  -216,  -216,  -216,  -216,  -115,
      -4,   126,  -216,  -216
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    14,    15,    16,    17,    18,    19,    20,   167,   168,
      88,   308,   309,   310,   222,   298,   299,   227,   367,   403,
     447,   419,   420,   421,   422,   423,   364,   399,    21,    22,
     165,   292,    23,    24,   151,   152,    25,    26,    43,    44,
     132,    41,    89,    90,    91,   136,    92,   276,   271,   195,
     196,   265,   266,   197,   278,   343,   390,   414,   433,   434,
     452,   460,   115,   116,   173,   174,   175,   176,   177,    94,
      95,    96,    97,    98,    99,   183,   184,   124,   125,   187,
     210,   100,   252,   101,   294,   249,   102,   141,   146,   157,
     103,   355,    28,    29
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      33,   181,    45,   180,   280,   236,   178,    42,    46,   156,
      27,    93,   198,   297,   178,    31,   251,    32,   440,    31,
     236,    32,   143,   144,   274,   236,    55,   236,   138,   139,
      57,    58,    59,    60,    61,   322,    51,   273,   194,   123,
     441,   185,   138,   139,    31,   200,    32,   185,   138,   139,
     149,   234,   138,   139,    34,   263,   445,   133,   217,    30,
     212,    39,   178,   450,   178,   109,   351,    50,   138,   139,
     198,   119,   160,   161,   233,   314,   315,   316,    37,    31,
     360,    32,   446,   361,   362,   451,    35,    40,    47,   142,
     352,   353,   301,    10,   110,    45,   194,   275,    38,   122,
     153,    46,   464,   158,   159,   182,   264,    93,   166,   169,
     323,   150,   354,    36,   158,   340,   378,    54,   293,   465,
     209,    56,   311,   254,   189,   302,   145,   300,   178,   178,
     317,   199,   192,   303,   337,   442,   202,   391,   387,   193,
     370,    10,   235,   182,   427,   205,   383,   150,   363,   198,
     400,   320,   255,   328,   104,   258,   169,   267,   223,   224,
     105,   218,   380,   268,   193,   346,   106,   304,    31,   216,
      32,   269,   407,   114,   349,   194,   305,   209,   178,    31,
     306,    32,   211,   284,   219,   138,   139,    10,   270,   199,
      46,   107,   267,   307,    46,   140,   377,   290,   268,     1,
     117,     2,   111,  -127,   291,   112,   269,   153,   138,   139,
     118,   285,   220,   198,   331,   162,   163,   318,   251,   321,
     376,   388,   295,   270,   327,   169,   214,   215,     3,   221,
     406,   330,   392,   393,   178,    52,   228,   229,    53,   194,
     138,   139,   138,   139,     4,     5,   395,    48,   404,    49,
       6,   397,   345,   396,   332,     7,   253,   215,   281,   282,
      46,   178,   324,   325,   344,   282,   356,   357,   199,   108,
     358,   359,    46,     8,   394,   215,   113,    31,    62,    32,
      63,   409,   282,   415,   215,   158,   374,   416,   215,   209,
     432,   430,   282,     9,    64,    65,   206,   114,   382,    10,
     120,   121,   372,   178,   431,   325,    67,    68,   438,   439,
      11,   209,   126,   127,    69,    70,    12,   129,    13,   456,
     439,    71,    72,    73,   462,   215,   128,   455,   207,    74,
     467,   215,   199,   468,   215,    75,   130,   131,    76,   134,
     137,   135,   463,   158,   158,   209,   466,   408,   155,    77,
      78,   164,   179,   188,   190,   295,   191,    79,    80,   213,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
      81,   138,   139,    10,   225,   226,   429,    82,   236,   250,
      83,    84,   435,   237,   259,   261,   260,   262,    85,   272,
     277,   279,    86,   286,   424,   287,   288,    87,   208,   289,
     296,   428,   319,   326,   329,   336,   338,   333,   339,   334,
      31,    62,    32,    63,   335,   341,   170,   350,   149,   435,
     443,   424,   248,   342,   347,   348,   366,    64,    65,   365,
     369,   371,   385,   453,   368,   424,   386,   375,   158,    67,
      68,   379,   389,    31,    62,    32,    63,    69,    70,   170,
     401,   381,   398,   158,    71,    72,    73,   158,   402,   405,
      64,    65,    74,   282,   411,   412,   413,   171,    75,   417,
     426,    76,    67,    68,   436,   192,   437,   469,   425,   457,
      69,    70,    77,    78,   449,   448,   459,    71,    72,    73,
      79,    80,   461,   470,   373,    74,   444,   283,   231,   454,
     171,    75,   154,    81,    76,   384,   201,    10,   458,   204,
      82,   232,   312,    83,    84,    77,    78,   313,   203,   148,
     186,    85,   418,    79,    80,    86,     0,   256,   410,     0,
     172,    31,    62,    32,    63,     0,    81,     0,     0,     0,
     257,     0,     0,    82,     0,     0,    83,    84,    64,    65,
      66,     0,     0,     0,    85,     0,     0,     0,    86,     0,
      67,    68,     0,   172,    31,    62,    32,    63,    69,    70,
       0,     0,     0,     0,     0,    71,    72,    73,     0,     0,
       0,    64,    65,    74,     0,     0,     0,     0,     0,    75,
       0,     0,    76,    67,    68,     0,     0,     0,     0,     0,
       0,    69,    70,    77,    78,     0,     0,     0,    71,    72,
      73,    79,    80,     0,     0,     0,    74,     0,     0,     0,
       0,     0,    75,     0,    81,    76,     0,     0,     0,     0,
       0,    82,     0,     0,    83,    84,    77,    78,     0,     0,
       0,     0,    85,     0,    79,    80,    86,     0,     0,     0,
       0,    87,    31,    62,    32,    63,     0,    81,     0,     0,
       0,     0,     0,     0,    82,     0,     0,    83,    84,    64,
      65,     0,     0,     0,     0,    85,   122,     0,     0,    86,
       0,    67,    68,     0,    87,    31,    62,    32,    63,    69,
      70,     0,     0,     0,     0,     0,    71,    72,    73,     0,
       0,     0,    64,    65,    74,     0,     0,     0,     0,     0,
      75,     0,     0,    76,    67,    68,     0,     0,     0,     0,
       0,     0,    69,    70,    77,    78,     0,     0,     0,    71,
      72,    73,    79,    80,     0,     0,     0,    74,     0,     0,
       0,     0,   171,    75,     0,    81,    76,     0,     0,    10,
       0,     0,    82,     0,     0,    83,    84,    77,    78,     0,
       0,     0,     0,    85,     0,    79,    80,    86,     0,     0,
       0,     0,    87,    31,    62,    32,    63,     0,    81,     0,
       0,     0,     0,     0,     0,    82,     0,     0,    83,    84,
      64,   147,     0,     0,     0,     0,    85,     0,     0,     0,
      86,     0,    67,    68,     0,   172,    31,    62,    32,    63,
      69,    70,     0,     0,     0,     0,     0,    71,    72,    73,
       0,     0,     0,    64,    65,    74,     0,     0,     0,     0,
       0,    75,     0,     0,    76,    67,    68,     0,     0,     0,
       0,     0,     0,    69,    70,    77,    78,     0,     0,     0,
      71,    72,    73,    79,    80,     0,     0,     0,    74,     0,
       0,     0,     0,     0,    75,     0,    81,    76,     0,     0,
       0,     0,     0,    82,     0,     0,    83,    84,    77,    78,
       0,     0,     0,     0,    85,     0,    79,    80,    86,     0,
       0,     0,     0,    87,     0,    62,     0,    63,     0,    81,
       0,     0,     0,     0,     0,     0,    82,     0,     0,    83,
      84,    64,   147,     0,     0,     0,     0,    85,    67,    68,
       0,    86,     0,    67,    68,     0,    87,    70,     0,     0,
       0,     0,    70,    71,    72,    73,     0,     0,    71,    72,
      73,    74,     0,     0,     0,     0,    74,     0,     0,     0,
      76,     0,     0,     0,     0,    76,     0,     0,     0,     0,
       0,    77,   230,     0,     0,     0,    77,    78,     0,    79,
       0,     0,     0,     0,    79,    80,     0,     0,     0,     0,
       0,     0,    81,     0,     0,     0,     0,    81,     0,    82,
       0,     0,    83,    84,    82,     0,     0,    83,    84,     0,
      85,     0,     0,     0,    86,    85,     0,     0,     0,    86,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
       0,   138,   139,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   248
};

static const yytype_int16 yycheck[] =
{
       4,   122,    12,   118,   207,     8,   114,    11,    12,   106,
       0,    41,   135,   228,   122,     4,   179,     6,     7,     4,
       8,     6,    23,    24,    37,     8,    30,     8,    21,    22,
      34,    35,    36,    37,    38,    10,    26,   198,   135,    69,
      29,    58,    21,    22,     4,   136,     6,    58,    21,    22,
      27,   172,    21,    22,    73,   193,    70,    87,   160,   105,
     151,    29,   170,    32,   172,    28,    63,    75,    21,    22,
     193,    61,    44,    45,   171,    17,    18,    19,   105,     4,
      35,     6,    96,    38,    39,    54,   105,    55,     0,    93,
      87,    88,    10,   101,    57,   105,   193,   110,    78,   116,
     104,   105,    64,   107,   108,   116,    31,   137,   112,   113,
      85,   124,   109,    67,   118,   276,   319,     3,   226,    81,
     150,    67,   125,   106,   128,    43,   127,   229,   236,   237,
      72,   135,   125,    51,   272,   124,   140,   125,   341,   124,
     303,   101,   172,   116,   125,   149,   125,   124,   103,   272,
     365,   248,   182,   106,   102,   185,   160,    68,   162,   163,
     126,    43,   325,    74,   124,   286,    31,    85,     4,   159,
       6,    82,   375,   117,   289,   272,    94,   207,   286,     4,
      98,     6,   126,   213,    66,    21,    22,   101,    99,   193,
     194,   124,    68,   111,   198,    31,     9,    53,    74,     1,
     114,     3,     5,    79,    60,    88,    82,   211,    21,    22,
     124,   215,    94,   336,   125,    44,    45,   247,   381,   249,
     317,   342,   226,    99,   254,   229,   125,   126,    30,   111,
       9,   261,   347,   348,   342,   120,   125,   126,   123,   336,
      21,    22,    21,    22,    46,    47,    53,   120,   369,   122,
      52,   359,   282,    60,   264,    57,   125,   126,   125,   126,
     264,   369,   125,   126,   125,   126,   125,   126,   272,    78,
     125,   126,   276,    75,   125,   126,   124,     4,     5,     6,
       7,   125,   126,   125,   126,   289,   316,   125,   126,   319,
     411,   125,   126,    95,    21,    22,    23,   117,   328,   101,
       7,     7,   306,   411,   125,   126,    33,    34,   125,   126,
     112,   341,   124,    77,    41,    42,   118,     5,   120,   125,
     126,    48,    49,    50,   125,   126,   124,   442,    55,    56,
     125,   126,   336,   125,   126,    62,   124,    77,    65,     5,
     126,    67,   457,   347,   348,   375,   461,   377,   124,    76,
      77,   118,   124,     7,     7,   359,   125,    84,    85,    11,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      97,    21,    22,   101,   124,   124,   406,   104,     8,    20,
     107,   108,   412,     9,    59,    67,   125,   125,   115,   126,
      69,   125,   119,   124,   398,    80,    80,   124,   125,   124,
     113,   405,   124,   114,    59,    79,     7,    90,     7,    90,
       4,     5,     6,     7,    90,    40,    10,     5,    27,   449,
     424,   425,    72,    71,   124,   124,   118,    21,    22,   126,
     124,    80,    93,   437,    85,   439,    93,   124,   442,    33,
      34,    85,    89,     4,     5,     6,     7,    41,    42,    10,
      36,   124,   124,   457,    48,    49,    50,   461,    91,   124,
      21,    22,    56,   126,    88,    40,    83,    61,    62,     5,
      40,    65,    33,    34,     7,   125,    98,    92,   124,   124,
      41,    42,    76,    77,   126,   125,    86,    48,    49,    50,
      84,    85,   124,     7,   309,    56,   425,   211,   169,   439,
      61,    62,   105,    97,    65,   336,   137,   101,   449,   146,
     104,   170,   236,   107,   108,    76,    77,   237,   141,   102,
     124,   115,   396,    84,    85,   119,    -1,   183,   381,    -1,
     124,     4,     5,     6,     7,    -1,    97,    -1,    -1,    -1,
     183,    -1,    -1,   104,    -1,    -1,   107,   108,    21,    22,
      23,    -1,    -1,    -1,   115,    -1,    -1,    -1,   119,    -1,
      33,    34,    -1,   124,     4,     5,     6,     7,    41,    42,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    -1,    -1,
      -1,    21,    22,    56,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    -1,    65,    33,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    42,    76,    77,    -1,    -1,    -1,    48,    49,
      50,    84,    85,    -1,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    97,    65,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,   107,   108,    76,    77,    -1,    -1,
      -1,    -1,   115,    -1,    84,    85,   119,    -1,    -1,    -1,
      -1,   124,     4,     5,     6,     7,    -1,    97,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,   107,   108,    21,
      22,    -1,    -1,    -1,    -1,   115,   116,    -1,    -1,   119,
      -1,    33,    34,    -1,   124,     4,     5,     6,     7,    41,
      42,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    -1,
      -1,    -1,    21,    22,    56,    -1,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    65,    33,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    41,    42,    76,    77,    -1,    -1,    -1,    48,
      49,    50,    84,    85,    -1,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    61,    62,    -1,    97,    65,    -1,    -1,   101,
      -1,    -1,   104,    -1,    -1,   107,   108,    76,    77,    -1,
      -1,    -1,    -1,   115,    -1,    84,    85,   119,    -1,    -1,
      -1,    -1,   124,     4,     5,     6,     7,    -1,    97,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,   107,   108,
      21,    22,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
     119,    -1,    33,    34,    -1,   124,     4,     5,     6,     7,
      41,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      -1,    -1,    -1,    21,    22,    56,    -1,    -1,    -1,    -1,
      -1,    62,    -1,    -1,    65,    33,    34,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    42,    76,    77,    -1,    -1,    -1,
      48,    49,    50,    84,    85,    -1,    -1,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    97,    65,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,   107,   108,    76,    77,
      -1,    -1,    -1,    -1,   115,    -1,    84,    85,   119,    -1,
      -1,    -1,    -1,   124,    -1,     5,    -1,     7,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,   107,
     108,    21,    22,    -1,    -1,    -1,    -1,   115,    33,    34,
      -1,   119,    -1,    33,    34,    -1,   124,    42,    -1,    -1,
      -1,    -1,    42,    48,    49,    50,    -1,    -1,    48,    49,
      50,    56,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,
      65,    -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,
      -1,    76,    77,    -1,    -1,    -1,    76,    77,    -1,    84,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    -1,    -1,    -1,    -1,    97,    -1,   104,
      -1,    -1,   107,   108,   104,    -1,    -1,   107,   108,    -1,
     115,    -1,    -1,    -1,   119,   115,    -1,    -1,    -1,   119,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72
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
     171,   172,   174,   196,   197,   198,   199,   200,   201,   202,
     209,   211,   214,   218,   102,   126,    31,   124,    78,    28,
      57,     5,    88,   124,   117,   190,   191,   114,   124,   168,
       7,     7,   116,   196,   205,   206,   124,    77,   124,     5,
     124,    77,   168,   196,     5,    67,   173,   126,    21,    22,
      31,   215,   218,    23,    24,   127,   216,    22,   199,    27,
     124,   162,   163,   218,   167,   124,   174,   217,   218,   218,
      44,    45,    44,    45,   118,   158,   218,   136,   137,   218,
      10,    61,   124,   192,   193,   194,   195,   196,   211,   124,
     217,   192,   116,   203,   204,    58,   206,   207,     7,   218,
       7,   125,   125,   124,   174,   177,   178,   181,   200,   218,
     190,   172,   218,   197,   198,   218,    23,    55,   125,   196,
     208,   126,   190,    11,   125,   126,   168,   136,    43,    66,
      94,   111,   142,   218,   218,   124,   124,   145,   125,   126,
      77,   138,   195,   174,   192,   196,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    72,   213,
      20,   209,   210,   125,   106,   196,   204,   207,   196,    59,
     125,    67,   125,   177,    31,   179,   180,    68,    74,    82,
      99,   176,   126,   179,    37,   110,   175,    69,   182,   125,
     208,   125,   126,   163,   196,   218,   124,    80,    80,   124,
      53,    60,   159,   211,   212,   218,   113,   142,   143,   144,
     136,    10,    43,    51,    85,    94,    98,   111,   139,   140,
     141,   125,   193,   194,    17,    18,    19,    72,   196,   124,
     174,   196,    10,    85,   125,   126,   114,   196,   106,    59,
     196,   125,   180,    90,    90,    90,    79,   177,     7,     7,
     179,    40,    71,   183,   125,   196,   192,   124,   124,   217,
       5,    63,    87,    88,   109,   219,   125,   126,   125,   126,
      35,    38,    39,   103,   154,   126,   118,   146,    85,   124,
     209,    80,   218,   139,   196,   124,   174,     9,   208,    85,
     209,   124,   196,   125,   178,    93,    93,   208,   192,    89,
     184,   125,   217,   217,   125,    53,    60,   211,   124,   155,
     142,    36,    91,   147,   192,   124,     9,   208,   196,   125,
     210,    88,    40,    83,   185,   125,   125,     5,   219,   149,
     150,   151,   152,   153,   218,   124,    40,   125,   218,   196,
     125,   125,   192,   186,   187,   196,     7,    98,   125,   126,
       7,    29,   124,   218,   149,    70,    96,   148,   125,   126,
      32,    54,   188,   218,   150,   217,   125,   124,   187,    86,
     189,   124,   125,   217,    64,    81,   217,   125,   125,    92,
       7
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
     198,   198,   199,   199,   199,   199,   199,   199,   199,   200,
     200,   200,   200,   201,   202,   202,   203,   203,   204,   205,
     205,   206,   207,   207,   208,   208,   209,   209,   209,   209,
     209,   209,   209,   210,   210,   211,   211,   212,   212,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   214,
     215,   215,   216,   216,   216,   217,   217,   218,   218,   219,
     219,   219,   219,   220,   221,   221
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
       2,     1,     1,     1,     1,     1,     1,     3,     1,     3,
       4,     4,     5,     6,     5,     4,     1,     2,     4,     1,
       2,     4,     0,     2,     1,     3,     1,     1,     2,     2,
       1,     2,     2,     1,     3,     1,     3,     1,     3,     1,
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
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1859 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_NAME  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1869 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1879 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1889 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 7: /* TOKEN_UNSIGNED_NUMVAL  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 1899 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 130: /* sql_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1909 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 131: /* quit_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 1919 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 132: /* alter_table_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1929 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 133: /* create_table_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 1939 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 134: /* create_index_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1949 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 135: /* drop_table_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 1959 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 136: /* column_def  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 1969 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 137: /* column_def_commalist  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 1979 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 138: /* data_type  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 1989 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 139: /* column_constraint_def  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 1999 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 140: /* column_constraint_def_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2009 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 141: /* opt_column_constraint_def_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2019 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 145: /* opt_column_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2029 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 146: /* opt_block_properties  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).block_properties_) != nullptr) {
    delete ((*yyvaluep).block_properties_);
  }
}
#line 2039 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 147: /* opt_partition_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).partition_clause_) != nullptr) {
    delete ((*yyvaluep).partition_clause_);
  }
}
#line 2049 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 148: /* partition_type  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2059 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 149: /* key_value_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2069 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 150: /* key_value  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 2079 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 151: /* key_string_value  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 2089 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 152: /* key_string_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 2099 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 153: /* key_integer_value  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 2109 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* index_type  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2119 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* opt_index_properties  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2129 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* insert_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 2139 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* copy_from_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_statement_) != nullptr) {
    delete ((*yyvaluep).copy_from_statement_);
  }
}
#line 2149 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* opt_copy_from_params  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2159 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* copy_from_params  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2169 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* update_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2179 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* delete_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2189 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* assignment_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2199 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 163: /* assignment_item  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2209 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 164: /* select_statement  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_statement_) != nullptr) {
    delete ((*yyvaluep).select_statement_);
  }
}
#line 2219 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 165: /* with_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2229 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 166: /* with_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2239 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 167: /* with_list_element  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2249 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* select_query  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2259 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* selection  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2269 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* selection_item_commalist  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2279 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* selection_item  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2289 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* from_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2299 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* subquery_expression  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2309 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* opt_sample_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_sample_clause_) != nullptr) {
    delete ((*yyvaluep).opt_sample_clause_);
  }
}
#line 2319 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* join_type  */
#line 564 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2325 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* joined_table_reference  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2335 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* table_reference  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2345 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* table_reference_signature  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2355 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* table_reference_signature_primary  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2365 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* joined_table_reference_commalist  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2375 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* opt_group_by_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2385 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* opt_having_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2395 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* opt_order_by_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2405 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* opt_limit_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2415 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* order_commalist  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2425 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* order_item  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2435 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* opt_order_direction  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2445 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* opt_nulls_first  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2455 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 190: /* opt_where_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2465 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 191: /* where_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2475 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 192: /* or_expression  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2485 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 193: /* and_expression  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2495 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 194: /* not_expression  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2505 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 195: /* predicate_expression_base  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2515 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 196: /* add_expression  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2525 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 197: /* multiply_expression  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2535 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 198: /* unary_expression  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2545 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 199: /* expression_base  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2555 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 200: /* function_call  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2565 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 201: /* extract_function  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2575 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 202: /* case_expression  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2585 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 203: /* simple_when_clause_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_list_);
  }
}
#line 2595 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 204: /* simple_when_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_);
  }
}
#line 2605 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 205: /* searched_when_clause_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_list_);
  }
}
#line 2615 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 206: /* searched_when_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_);
  }
}
#line 2625 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 207: /* opt_else_clause  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2635 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 208: /* expression_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2645 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 209: /* literal_value  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2655 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 210: /* literal_value_commalist  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2665 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 211: /* attribute_ref  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2675 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 212: /* attribute_ref_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2685 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 213: /* comparison_operation  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2691 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 214: /* unary_operation  */
#line 562 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2697 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 215: /* add_operation  */
#line 563 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2703 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 216: /* multiply_operation  */
#line 563 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2709 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 217: /* name_commalist  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 2719 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 218: /* any_name  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2729 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 219: /* boolean_value  */
#line 560 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2735 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 220: /* command  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_) != nullptr) {
    delete ((*yyvaluep).command_);
  }
}
#line 2745 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 221: /* command_argument_list  */
#line 566 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_argument_list_) != nullptr) {
    delete ((*yyvaluep).command_argument_list_);
  }
}
#line 2755 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 3052 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 579 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3061 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 583 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3070 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 5:
#line 587 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3079 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 591 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    YYABORT;
  }
#line 3087 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 594 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 3096 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 601 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3104 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 9:
#line 604 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].copy_from_statement_);
  }
#line 3112 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 10:
#line 607 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 3120 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 11:
#line 610 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3128 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 12:
#line 613 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 3136 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 13:
#line 616 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 3144 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 14:
#line 619 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 3152 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 15:
#line 622 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 3160 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 16:
#line 625 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].select_statement_);
  }
#line 3168 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 17:
#line 628 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 3176 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 18:
#line 634 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3184 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 3196 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 20:
#line 647 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3207 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 3219 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 3231 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 669 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].attribute_definition_list_), (yyvsp[-1].block_properties_), (yyvsp[0].partition_clause_));
  }
#line 3239 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 3251 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 683 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 3259 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 688 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 3267 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 693 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3276 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 697 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3285 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 703 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 3295 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 708 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3303 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 711 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3311 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 714 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3321 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 719 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3329 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 722 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3337 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 725 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3345 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 728 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3353 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 731 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3361 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 734 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3369 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 737 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3377 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 740 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3385 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 743 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3393 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 3410 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 758 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3418 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 761 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3426 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 3449 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 3472 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 802 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3480 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 805 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3488 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 808 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3498 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 813 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3508 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 818 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3519 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 824 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3530 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 3542 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 839 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3551 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 843 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3560 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 849 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3568 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 852 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3576 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 857 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3586 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 862 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3596 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 3608 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 874 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3618 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 881 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3627 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 885 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3636 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 891 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3644 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 894 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = $1; */
  }
#line 3652 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 899 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = nullptr;
  }
#line 3660 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 902 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-1].attribute_list_);
  }
#line 3668 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 907 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = nullptr;
  }
#line 3676 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 910 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].key_value_list_));
  }
#line 3684 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 915 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.partition_clause_) = nullptr;
  }
#line 3692 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 3714 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 937 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::PartitionSchemeHeader::PartitionType::kHash));
  }
#line 3723 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 941 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::PartitionSchemeHeader::PartitionType::kRange));
  }
#line 3732 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 947 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3741 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 951 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3750 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 957 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 3758 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 960 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 3766 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 963 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 3774 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 968 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 3782 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 971 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_),
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 3792 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 978 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 3800 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 3814 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 994 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Defaults to BitWeavingV, but IndexProperties can change this to H.
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBitWeavingV));
  }
#line 3824 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 999 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
#line 3833 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 1003 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
#line 3842 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 1007 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kSMA));
  }
#line 3851 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 1013 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 3859 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 1016 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 3867 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 3880 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 1030 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertTuple((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 3888 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 1033 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-3]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), nullptr);
  }
#line 3896 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 1036 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-4]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), (yyvsp[-4].with_list_));
  }
#line 3904 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 1042 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_statement_) = new quickstep::ParseStatementCopyFrom((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].copy_from_params_));
  }
#line 3912 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 1047 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = nullptr;
  }
#line 3920 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 1050 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-1].copy_from_params_);
  }
#line 3928 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 1055 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3937 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 1059 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3946 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 1063 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3955 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 1067 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3964 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 1073 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 3972 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 1078 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 3980 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 1083 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3989 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 1087 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3998 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 1093 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 4006 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 1099 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].select_query_), nullptr);
  }
#line 4014 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 1102 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].select_query_), (yyvsp[-1].with_list_));
  }
#line 4022 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 1107 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 4030 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 1112 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4039 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 1116 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4048 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 1122 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 4057 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 1129 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].selection_), (yyvsp[-5].table_reference_list_), (yyvsp[-4].predicate_), (yyvsp[-3].opt_group_by_clause_), (yyvsp[-2].opt_having_clause_), (yyvsp[-1].opt_order_by_clause_), (yyvsp[0].opt_limit_clause_));
  }
#line 4065 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 1134 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 4073 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 1137 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "ALL in selection");
    YYERROR;
  }
#line 4082 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 1141 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "DISTINCT in selection");
    YYERROR;
  }
#line 4091 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 1147 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4099 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 1150 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 4107 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 1155 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4116 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 1159 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4125 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 1165 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 4133 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 1168 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 4141 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 1171 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 4149 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 1176 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[0].table_reference_list_);
  }
#line 4157 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 1181 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].select_query_));
  }
#line 4165 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 1186 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = NULL;
  }
#line 4173 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 1189 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, true, (yyvsp[-1].numeric_literal_value_));
  }
#line 4181 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 1192 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].numeric_literal_value_));
  }
#line 4189 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 1197 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4197 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 1200 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4205 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 1203 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4213 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 1206 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4221 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 1209 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4229 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 1212 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4237 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 1215 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4245 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 1218 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4253 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 1223 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseJoinedTableReference((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].join_type_), (yyvsp[-5].table_reference_), (yyvsp[-2].table_reference_), (yyvsp[0].predicate_));
  }
#line 4261 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 1226 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = (yyvsp[0].table_reference_);
  }
#line 4269 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 1231 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4278 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 1235 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].opt_sample_clause_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4287 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 1239 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].opt_sample_clause_));
  }
#line 4295 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 1242 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4304 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 1246 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].function_call_));
  }
#line 4312 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 1249 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = (yyvsp[-1].table_reference_);
  }
#line 4320 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 1254 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4328 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 1257 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4336 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 1262 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4344 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 1265 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4352 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 1270 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4361 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 1274 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4370 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 1280 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 4378 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 1283 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4386 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 1288 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4394 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 1291 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4402 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 1296 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4410 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 1299 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4418 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 1304 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4426 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 4448 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 1326 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4457 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 1330 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4466 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 1336 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 4476 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 1343 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4484 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 1346 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4492 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 1349 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4500 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 1354 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4508 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 1357 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4516 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 1360 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4524 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 1366 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 4532 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 1369 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4540 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 1374 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4548 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 4562 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 1388 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4570 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 4584 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 1402 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4592 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 1407 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4600 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 1410 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4608 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 1415 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4616 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 1418 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 4626 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 1423 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4637 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 1429 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4648 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 1435 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4656 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 1438 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 4664 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 1441 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateExists((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].subquery_expression_));
  }
#line 4672 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 1444 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-2].expression_), (yyvsp[0].subquery_expression_));
  }
#line 4680 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 1447 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-1].expression_list_));
  }
#line 4688 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 1450 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-2]).first_line,
        (yylsp[-2]).first_column,
        new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-3].expression_), (yyvsp[0].subquery_expression_)));
  }
#line 4699 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 1456 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line,
        (yylsp[-4]).first_column,
        new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-1].expression_list_)));
  }
#line 4710 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1465 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4718 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 1468 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4726 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 1473 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4734 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 1476 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4742 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 1481 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].unary_operation_), (yyvsp[0].expression_));
  }
#line 4750 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 1484 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4758 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 1489 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 4766 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 1492 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 4774 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 1495 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 4782 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 1498 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4790 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 1501 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4798 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 1504 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 4806 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 1507 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].subquery_expression_);
  }
#line 4814 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 1512 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 4823 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1516 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 4832 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1520 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, false, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4840 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1523 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-4]).first_line, (yylsp[-4]).first_column, true, (yyvsp[-4].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4848 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1528 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseExtractFunction((yylsp[-5]).first_line, (yylsp[-5]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].expression_));
  }
#line 4856 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1533 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSimpleCaseExpression((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].expression_), (yyvsp[-2].simple_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 4864 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1536 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSearchedCaseExpression((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].searched_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 4872 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1541 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSimpleWhenClause>;
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 4881 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1545 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_list_) = (yyvsp[-1].simple_when_clause_list_);
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 4890 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1551 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_) = new quickstep::ParseSimpleWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4898 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1556 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSearchedWhenClause>;
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 4907 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1560 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_list_) = (yyvsp[-1].searched_when_clause_list_);
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 4916 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1566 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_) = new quickstep::ParseSearchedWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].predicate_), (yyvsp[0].expression_));
  }
#line 4924 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1571 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = NULL;
  }
#line 4932 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1574 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4940 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1579 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4949 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1583 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4958 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1589 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4966 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1592 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4974 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1595 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4982 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1598 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4999 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1610 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 5008 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1614 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5032 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1633 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5050 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1648 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5059 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1652 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5068 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1658 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 5076 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1661 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 5084 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1666 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = new quickstep::PtrList<quickstep::ParseAttribute>();
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5093 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1670 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-2].attribute_list_);
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5102 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1677 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 5110 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1680 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 5118 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1683 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 5126 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1686 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 5134 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1689 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 5142 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1692 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 5150 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1695 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLike);
  }
#line 5158 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1698 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotLike);
  }
#line 5166 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1701 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kRegexMatch);
  }
#line 5174 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1704 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotRegexMatch);
  }
#line 5182 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1709 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
#line 5196 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1720 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
#line 5204 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1723 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
#line 5212 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1728 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kModulo);
  }
#line 5220 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1731 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
#line 5228 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1734 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
#line 5236 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1740 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 5245 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1744 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 5254 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1750 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5262 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1753 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5273 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1761 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5281 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1764 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5289 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1767 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5297 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1770 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5305 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1776 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.command_) = new quickstep::ParseCommand((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].command_argument_list_));
  }
#line 5313 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1781 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    quickstep::PtrVector<quickstep::ParseString> *argument_list = (yyvsp[-1].command_argument_list_);
    argument_list->push_back((yyvsp[0].string_value_));
    (yyval.command_argument_list_) = argument_list;
  }
#line 5323 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1786 "../SqlParser.ypp" /* yacc.c:1661  */
    { /* Epsilon, an empy match. */
    (yyval.command_argument_list_) = new quickstep::PtrVector<quickstep::ParseString>();
  }
#line 5331 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;


#line 5335 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 1790 "../SqlParser.ypp" /* yacc.c:1906  */


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
