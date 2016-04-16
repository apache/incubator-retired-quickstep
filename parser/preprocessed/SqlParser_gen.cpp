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

#line 153 "SqlParser_gen.cpp" /* yacc.c:339  */

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
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 117 "../SqlParser.ypp" /* yacc.c:355  */

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

#line 400 "SqlParser_gen.cpp" /* yacc.c:355  */
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
#line 207 "../SqlParser.ypp" /* yacc.c:358  */

/* This header needs YYSTYPE, which is defined by the %union directive above */
#include "SqlLexer_gen.hpp"
void NotSupported(const YYLTYPE *location, yyscan_t yyscanner, const std::string &feature);

#line 433 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYLAST   1147

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  128
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  95
/* YYNRULES -- Number of rules.  */
#define YYNRULES  255
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  497

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
       0,   570,   570,   574,   578,   582,   586,   589,   596,   599,
     602,   605,   608,   611,   614,   617,   620,   623,   629,   635,
     642,   648,   655,   664,   669,   678,   683,   688,   692,   698,
     703,   706,   709,   714,   717,   720,   723,   726,   729,   732,
     735,   738,   741,   753,   756,   759,   777,   797,   800,   803,
     808,   813,   819,   825,   834,   838,   844,   847,   852,   857,
     862,   869,   876,   880,   886,   889,   894,   897,   902,   905,
     910,   913,   932,   936,   942,   946,   952,   955,   958,   963,
     966,   973,   978,   989,   994,   998,  1002,  1008,  1011,  1017,
    1025,  1028,  1031,  1037,  1042,  1045,  1050,  1054,  1058,  1062,
    1068,  1073,  1078,  1082,  1088,  1094,  1097,  1102,  1107,  1111,
    1117,  1123,  1129,  1132,  1136,  1142,  1145,  1150,  1154,  1160,
    1163,  1166,  1171,  1176,  1179,  1185,  1189,  1195,  1201,  1207,
    1213,  1219,  1225,  1231,  1237,  1245,  1250,  1253,  1256,  1261,
    1265,  1269,  1272,  1276,  1281,  1284,  1289,  1292,  1297,  1301,
    1307,  1310,  1315,  1318,  1323,  1326,  1331,  1334,  1353,  1357,
    1363,  1370,  1373,  1376,  1381,  1384,  1387,  1393,  1396,  1401,
    1406,  1415,  1420,  1429,  1434,  1437,  1442,  1445,  1450,  1456,
    1462,  1465,  1468,  1471,  1474,  1477,  1483,  1492,  1495,  1500,
    1503,  1508,  1511,  1516,  1519,  1522,  1525,  1528,  1531,  1536,
    1540,  1544,  1547,  1552,  1557,  1560,  1565,  1569,  1575,  1580,
    1584,  1590,  1595,  1598,  1603,  1607,  1613,  1616,  1619,  1622,
    1634,  1638,  1657,  1672,  1676,  1682,  1685,  1690,  1694,  1701,
    1704,  1707,  1710,  1713,  1716,  1719,  1722,  1725,  1728,  1733,
    1744,  1747,  1752,  1755,  1758,  1764,  1768,  1774,  1777,  1785,
    1788,  1791,  1794,  1800,  1805,  1810
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
  "opt_join_chain", "join_chain", "join", "subquery_expression",
  "opt_sample_clause", "table_reference", "table_reference_signature",
  "table_reference_signature_primary", "table_reference_commalist",
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

#define YYPACT_NINF -209

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-209)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     394,  -209,  -209,   -35,   190,    72,   133,   120,    53,  -209,
      69,   190,   190,  -209,    94,   127,  -209,  -209,  -209,  -209,
    -209,  -209,  -209,  -209,  -209,  -209,    91,  -209,   -34,   207,
     190,  -209,  -209,   166,   190,   190,   190,   190,   190,  -209,
    -209,   602,   134,   113,  -209,   235,   144,  -209,  -209,  -209,
     197,  -209,  -209,  -209,  -209,    62,   289,   208,   176,   192,
    -209,   137,  -209,  -209,   308,   321,  -209,  -209,  -209,   635,
     206,  -209,   255,  -209,  -209,   210,  -209,  -209,   331,  -209,
    -209,  -209,  -209,  -209,  -209,   214,   272,   844,   346,   285,
     227,  -209,   236,    -2,  -209,  -209,  -209,  -209,  -209,  -209,
    -209,   877,    41,   190,   190,   230,   190,   190,   123,   220,
     237,   190,   190,   514,  -209,  -209,   233,   190,  -209,  -209,
    -209,   514,    23,   -33,  -209,   349,  -209,   190,  -209,   352,
    -209,     7,  -209,     9,   192,   844,  -209,  -209,   190,   844,
    -209,  -209,  -209,  -209,   844,   321,  -209,   190,   426,   126,
    -209,   350,  -209,   261,  -209,   146,  -209,   261,   190,     3,
     190,   190,   239,  -209,   241,  -209,   148,   960,   723,   230,
     514,   358,   359,  -209,  -209,  1075,   354,   965,   152,    15,
     844,   -20,  -209,   844,  -209,   311,   247,   309,   254,  -209,
      81,  -209,   105,    81,   -32,   312,  -209,  -209,    -2,  -209,
    -209,   260,   844,  -209,   258,   156,   190,  -209,   844,   262,
    -209,   190,  -209,  -209,   256,   303,   313,   265,  -209,  -209,
    -209,   184,   190,   278,     3,   190,  -209,   118,  -209,  -209,
       1,    37,   514,   514,    98,  -209,  -209,  -209,  -209,  -209,
    -209,  -209,  -209,   844,   268,   844,    26,  -209,   165,   280,
     844,    52,  -209,   337,   258,  -209,  -209,   844,  -209,   190,
    -209,  -209,   116,   319,   190,   129,   132,     9,  -209,   106,
    -209,  -209,   395,   399,    81,   361,   336,  -209,   167,  -209,
     844,  -209,   258,  -209,  -209,   514,   284,   286,   190,   404,
      88,   173,  -209,   177,   385,    27,  -209,   287,   296,  -209,
     330,   292,   965,  -209,   338,   190,  -209,  -209,   118,  -209,
    -209,   359,  -209,  -209,  -209,   844,   293,    55,   756,  -209,
     258,   334,  -209,  -209,   965,   297,   258,   844,  -209,    39,
    -209,   190,   341,   190,   -47,   190,   343,   190,   344,  -209,
    -209,   332,   333,  -209,   844,   514,   339,  -209,   258,    10,
     190,   190,   180,  -209,  -209,  -209,  -209,  -209,  -209,  -209,
     209,  -209,   190,  -209,  -209,  -209,  -209,   305,     3,   391,
     345,  -209,   514,  -209,  -209,   310,  -209,   232,   756,  -209,
     844,   182,  -209,  -209,   965,   258,  -209,   -45,   190,   -25,
     514,   -16,   190,    18,   190,  -209,  -209,   316,   358,   403,
     356,  -209,   185,   187,  -209,   430,    88,  -209,   190,  -209,
    -209,   320,   410,  -209,    16,   190,   844,   191,   258,  -209,
     193,   514,    64,   514,   358,   514,    67,   514,    75,   844,
     445,  -209,   355,  -209,  -209,  -209,   195,  -209,  -209,  -209,
    -209,    13,   190,   121,  -209,   329,   258,  -209,  -209,   358,
     514,   358,   358,   514,   358,   514,   340,  -209,   202,  -209,
     190,  -209,   190,  -209,  -209,   190,  -209,   198,  -209,  -209,
     347,  -209,   358,   358,   358,   844,  -209,  -209,   369,   348,
    -209,   200,  -209,   190,  -209,   117,  -209,   190,  -209,   217,
    -209,  -209,   219,   365,  -209,   455,  -209
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
     114,     0,     0,   107,   108,     0,   146,     1,     3,     2,
       0,   106,     5,     4,   254,     0,     0,     0,     0,   167,
      25,     0,   220,   217,     0,   239,   115,    40,    29,     0,
       0,    30,    31,    34,    36,     0,    37,    39,     0,    41,
     216,    35,    38,    32,    33,     0,     0,     0,     0,     0,
     116,   117,   121,   188,   190,   192,   195,   196,   197,   194,
     193,     0,   225,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,     0,   101,   168,     0,     0,    91,   218,
     219,     0,     0,   212,   209,     0,    43,     0,   221,     0,
      44,     0,   222,     0,   167,     0,   240,   241,     0,     0,
     120,   243,   244,   242,     0,     0,   191,     0,     0,   167,
     103,     0,   109,     0,   110,     0,   245,     0,     0,     0,
       0,     0,     0,    93,    66,    27,     0,     0,     0,     0,
       0,   169,   171,   173,   175,     0,   193,     0,     0,     0,
       0,   212,   206,     0,   210,     0,     0,     0,     0,   198,
       0,   148,   123,   143,   136,   150,   118,   119,   187,   189,
     226,     0,     0,   199,   214,     0,     0,   100,     0,     0,
     147,     0,    92,    19,     0,     0,     0,     0,    20,    21,
      22,     0,     0,     0,    64,     0,    42,    56,   174,   182,
       0,     0,     0,     0,     0,   229,   231,   232,   233,   234,
     230,   235,   237,     0,     0,     0,     0,   223,     0,     0,
       0,     0,   207,     0,   213,   205,    45,     0,    46,     0,
     139,   144,     0,     0,     0,     0,     0,     0,   122,   124,
     126,   142,     0,     0,   141,     0,   152,   200,     0,   201,
       0,   102,   104,   135,   246,     0,     0,     0,     0,     0,
       0,     0,   227,     0,   225,     0,    63,    65,    68,    28,
       0,     0,     0,    47,     0,     0,    49,    55,    57,    26,
     181,   170,   172,   236,   238,     0,     0,     0,     0,   183,
     180,     0,   179,    90,     0,     0,   211,     0,   204,     0,
     145,     0,     0,     0,     0,     0,     0,     0,     0,   149,
     125,     0,     0,   140,     0,     0,   154,   202,   215,     0,
       0,     0,     0,    96,   251,   252,   250,   249,    97,    95,
       0,    67,     0,    83,    84,    85,    86,    87,     0,     0,
      70,    48,     0,    51,    50,     0,    54,     0,     0,   185,
       0,     0,   178,   224,     0,   208,   203,     0,     0,     0,
       0,     0,     0,     0,     0,   137,   138,   151,   153,     0,
     156,    61,     0,     0,    58,     0,     0,   228,     0,    24,
      62,     0,     0,    23,     0,     0,     0,     0,   176,   184,
       0,     0,     0,     0,   128,     0,     0,     0,     0,     0,
       0,   111,     0,    59,    98,    99,     0,    74,    76,    77,
      78,     0,     0,     0,    52,     0,   177,   186,    89,   134,
       0,   127,   130,     0,   132,     0,   155,   158,   161,   157,
       0,    88,     0,    82,    80,     0,    79,     0,    72,    73,
       0,    53,   133,   129,   131,     0,   162,   163,   164,     0,
      75,     0,    69,     0,   159,     0,   160,     0,    81,     0,
     165,   166,     0,     0,    60,     0,    71
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -209,  -209,  -209,  -209,  -209,  -209,  -209,  -209,  -121,  -209,
     298,   155,  -209,  -209,  -208,  -209,  -209,  -209,  -209,  -209,
    -209,    22,     8,  -209,  -209,  -209,  -209,  -209,  -209,  -209,
    -209,  -209,  -209,  -209,  -209,   267,  -209,  -209,  -209,   373,
      14,  -209,  -209,  -209,   351,  -209,  -209,  -209,   211,  -102,
    -209,   218,  -183,   -11,  -209,  -209,  -209,  -209,  -209,  -209,
      12,  -209,  -209,   -54,  -209,   -50,   246,   251,   322,   -30,
     353,   357,   392,  -129,  -209,  -209,  -209,   315,  -209,   371,
     317,  -196,  -175,   115,   -86,  -209,  -209,  -209,  -209,  -209,
    -105,    -4,    99,  -209,  -209
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    14,    15,    16,    17,    18,    19,    20,   165,   166,
      88,   307,   308,   309,   218,   297,   298,   223,   370,   413,
     470,   436,   437,   438,   439,   440,   367,   409,    21,    22,
     163,   291,    23,    24,   149,   150,    25,    26,    43,    44,
     209,    41,    89,    90,    91,   134,   268,   269,   270,   190,
     274,   191,   260,   261,   192,   276,   346,   400,   431,   456,
     457,   478,   486,   114,   115,   171,   172,   173,   174,   175,
      93,    94,    95,    96,    97,    98,   181,   182,   123,   124,
     185,   205,    99,   248,   100,   293,   245,   101,   139,   144,
     155,   102,   358,    28,    29
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
      33,    45,   247,   154,   193,   272,   278,    42,    46,   232,
     271,    92,   178,    31,    27,    32,   296,    31,   232,    32,
     463,   141,   142,   232,   232,   183,    55,   176,   136,   137,
      57,    58,    59,    60,    61,   176,   321,   213,   183,   122,
      51,   390,   464,   421,   136,   137,   214,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   131,   136,   137,
     136,   137,   363,   423,   380,   364,   365,   229,   147,   215,
      30,   179,   425,   136,   137,   118,   136,   137,   273,   211,
     195,   211,   176,   121,   176,    31,    52,    32,   140,    53,
     108,   343,   148,    45,    47,   207,   180,   216,    39,   151,
      46,   211,   156,   157,   299,    92,   427,   164,   167,   244,
     211,   322,   259,   156,   217,   313,   314,   315,   204,   109,
     230,   250,   381,   187,    40,   143,   310,   373,   300,   194,
     366,    38,   189,   153,   197,   401,   292,   465,   193,   180,
     231,   444,   319,   200,   211,    34,   176,   176,   397,   383,
     251,   354,   450,   254,   167,   453,   219,   220,   327,   334,
     410,   301,   189,   455,   386,   148,    50,   158,   159,   302,
     316,   212,   204,   262,   262,   355,   356,    35,   282,   263,
     263,   490,   417,   352,   264,   264,    46,   265,   265,    46,
     211,   468,    10,   211,    31,   331,    32,   357,   491,   176,
      36,   211,   151,   303,   266,   266,   332,   284,   335,   247,
      54,   337,   304,   317,   379,   320,   305,   469,   294,   336,
     326,   167,   338,   136,   137,    37,   387,   329,   389,   306,
     391,   267,   393,    56,   476,   349,   103,   289,    10,   104,
      31,   416,    32,   113,   290,   402,   403,    48,   330,    49,
     348,   116,   206,   136,   137,    46,   477,   136,   137,   176,
     156,   117,   405,   194,   160,   161,   105,   138,   106,   406,
      46,   210,   211,   224,   225,   107,   407,   249,   211,   136,
     137,   279,   280,   422,   156,   377,   176,   426,   204,   428,
     323,   324,   347,   280,   110,   398,   111,   385,   359,   360,
     112,   375,   361,   362,   176,   404,   211,   419,   280,   113,
     432,   211,   433,   211,   204,   119,   447,   280,   448,   324,
     461,   462,   414,   482,   462,   488,   211,   156,   120,   156,
     125,   156,   126,   156,   127,   176,   128,   176,   129,   176,
     424,   176,   493,   211,   494,   211,   156,   156,   204,   130,
     418,   132,   133,   135,   153,   162,   186,   177,   294,   188,
     481,   208,    10,   221,   176,   222,   232,   176,   233,   176,
     255,   449,   256,   451,   246,   452,   257,   454,   489,   258,
     285,   275,   492,   286,   156,   277,   446,   283,   156,   288,
     156,   295,   318,   287,   325,     1,   328,     2,   333,   458,
     472,   344,   341,   473,   441,   474,   342,   345,   350,   353,
     351,   445,   147,   368,   369,   371,   372,   378,   374,   382,
     388,   384,   392,   394,     3,   395,   396,   411,   399,   408,
      31,    62,    32,    63,   415,   434,   412,   466,   441,   430,
       4,     5,   280,   429,   442,   458,     6,    64,    65,   201,
     443,     7,   459,   460,   471,   485,   479,   495,   441,    67,
      68,   156,   496,   376,   467,   227,   475,    69,    70,     8,
     480,   483,   487,   281,    71,    72,    73,   152,   311,   156,
     340,   202,    74,   156,   312,   339,   196,   484,    75,     9,
     228,    76,   198,   146,   184,    10,   252,     0,   253,   420,
       0,   199,    77,    78,     0,   435,    11,     0,     0,     0,
      79,    80,    12,     0,    13,     0,     0,     0,    31,    62,
      32,    63,     0,    81,   168,     0,     0,     0,     0,     0,
      82,     0,     0,    83,    84,    64,    65,     0,     0,     0,
       0,    85,     0,     0,     0,    86,     0,    67,    68,     0,
      87,   203,     0,     0,     0,    69,    70,     0,     0,     0,
       0,     0,    71,    72,    73,     0,     0,     0,     0,     0,
      74,     0,     0,     0,     0,   169,    75,     0,     0,    76,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      77,    78,     0,     0,     0,     0,     0,     0,    79,    80,
       0,     0,     0,     0,     0,     0,    31,    62,    32,    63,
       0,    81,     0,     0,     0,     0,     0,     0,    82,     0,
       0,    83,    84,    64,    65,    66,     0,     0,     0,    85,
       0,     0,     0,    86,     0,    67,    68,     0,   170,    31,
      62,    32,    63,    69,    70,     0,     0,     0,     0,     0,
      71,    72,    73,     0,     0,     0,    64,    65,    74,     0,
       0,     0,     0,     0,    75,     0,     0,    76,    67,    68,
       0,     0,     0,     0,     0,     0,    69,    70,    77,    78,
       0,     0,     0,    71,    72,    73,    79,    80,     0,     0,
       0,    74,     0,     0,     0,     0,     0,    75,     0,    81,
      76,     0,     0,     0,     0,     0,    82,     0,     0,    83,
      84,    77,    78,     0,     0,     0,     0,    85,     0,    79,
      80,    86,     0,     0,     0,     0,    87,    31,    62,    32,
      63,     0,    81,     0,     0,     0,     0,     0,     0,    82,
       0,     0,    83,    84,    64,    65,     0,     0,     0,     0,
      85,   121,     0,     0,    86,     0,    67,    68,     0,    87,
      31,    62,    32,    63,    69,    70,     0,     0,     0,     0,
       0,    71,    72,    73,     0,     0,     0,    64,    65,    74,
       0,     0,     0,     0,   169,    75,     0,     0,    76,    67,
      68,     0,     0,     0,     0,     0,     0,    69,    70,    77,
      78,     0,     0,     0,    71,    72,    73,    79,    80,     0,
       0,     0,    74,     0,     0,     0,     0,     0,    75,     0,
      81,    76,     0,     0,     0,     0,     0,    82,     0,     0,
      83,    84,    77,    78,     0,     0,     0,     0,    85,     0,
      79,    80,    86,     0,     0,     0,     0,   170,    31,    62,
      32,    63,     0,    81,     0,     0,     0,    10,     0,     0,
      82,     0,     0,    83,    84,    64,    65,     0,     0,     0,
       0,    85,     0,     0,     0,    86,     0,    67,    68,     0,
      87,    31,    62,    32,    63,    69,    70,     0,     0,     0,
       0,     0,    71,    72,    73,     0,     0,     0,    64,   145,
      74,     0,     0,     0,     0,     0,    75,     0,     0,    76,
      67,    68,     0,     0,     0,     0,     0,     0,    69,    70,
      77,    78,     0,     0,     0,    71,    72,    73,    79,    80,
       0,     0,     0,    74,     0,     0,     0,     0,     0,    75,
       0,    81,    76,     0,     0,     0,     0,     0,    82,     0,
       0,    83,    84,    77,    78,     0,     0,     0,     0,    85,
       0,    79,    80,    86,     0,     0,     0,     0,    87,     0,
      62,     0,    63,     0,    81,     0,     0,     0,     0,     0,
       0,    82,     0,     0,    83,    84,    64,   145,     0,     0,
       0,     0,    85,    67,    68,     0,    86,     0,    67,    68,
       0,    87,    70,     0,     0,     0,     0,    70,    71,    72,
      73,     0,     0,    71,    72,    73,    74,     0,     0,     0,
       0,    74,     0,     0,     0,    76,     0,     0,     0,     0,
      76,     0,     0,     0,     0,     0,    77,   226,     0,     0,
       0,    77,    78,     0,    79,     0,     0,     0,     0,    79,
      80,     0,     0,     0,     0,     0,     0,    81,     0,     0,
       0,     0,    81,     0,    82,     0,     0,    83,    84,    82,
       0,     0,    83,    84,     0,    85,     0,     0,     0,    86,
      85,     0,     0,     0,    86,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,     0,   136,   137,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   244
};

static const yytype_int16 yycheck[] =
{
       4,    12,   177,   105,   133,    37,   202,    11,    12,     8,
     193,    41,   117,     4,     0,     6,   224,     4,     8,     6,
       7,    23,    24,     8,     8,    58,    30,   113,    21,    22,
      34,    35,    36,    37,    38,   121,    10,   158,    58,    69,
      26,    88,    29,    88,    21,    22,    43,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    87,    21,    22,
      21,    22,    35,    88,     9,    38,    39,   169,    27,    66,
     105,   121,    88,    21,    22,    61,    21,    22,   110,   126,
     134,   126,   168,   116,   170,     4,   120,     6,    92,   123,
      28,   274,   124,   104,     0,   149,   116,    94,    29,   103,
     104,   126,   106,   107,   225,   135,    88,   111,   112,    72,
     126,    85,    31,   117,   111,    17,    18,    19,   148,    57,
     170,   106,   318,   127,    55,   127,   125,   302,    10,   133,
     103,    78,   125,   124,   138,   125,   222,   124,   267,   116,
     170,   125,   244,   147,   126,    73,   232,   233,   344,   324,
     180,    63,    88,   183,   158,    88,   160,   161,   106,   264,
     368,    43,   125,    88,   125,   124,    75,    44,    45,    51,
      72,   157,   202,    68,    68,    87,    88,   105,   208,    74,
      74,    64,   378,   288,    79,    79,   190,    82,    82,   193,
     126,    70,   101,   126,     4,    79,     6,   109,    81,   285,
      67,   126,   206,    85,    99,    99,    90,   211,    79,   384,
       3,    79,    94,   243,   316,   245,    98,    96,   222,    90,
     250,   225,    90,    21,    22,   105,   331,   257,   333,   111,
     335,   126,   337,    67,    32,   285,   102,    53,   101,   126,
       4,     9,     6,   117,    60,   350,   351,   120,   259,   122,
     280,   114,   126,    21,    22,   259,    54,    21,    22,   345,
     264,   124,    53,   267,    44,    45,    31,    31,   124,    60,
     274,   125,   126,   125,   126,    78,   362,   125,   126,    21,
      22,   125,   126,   388,   288,   315,   372,   392,   318,   394,
     125,   126,   125,   126,     5,   345,    88,   327,   125,   126,
     124,   305,   125,   126,   390,   125,   126,   125,   126,   117,
     125,   126,   125,   126,   344,     7,   125,   126,   125,   126,
     125,   126,   372,   125,   126,   125,   126,   331,     7,   333,
     124,   335,    77,   337,   124,   421,     5,   423,   124,   425,
     390,   427,   125,   126,   125,   126,   350,   351,   378,    77,
     380,     5,    67,   126,   124,   118,     7,   124,   362,     7,
     465,    11,   101,   124,   450,   124,     8,   453,     9,   455,
      59,   421,   125,   423,    20,   425,    67,   427,   483,   125,
     124,    69,   487,    80,   388,   125,   416,   125,   392,   124,
     394,   113,   124,    80,   114,     1,    59,     3,    79,   429,
     450,    40,     7,   453,   408,   455,     7,    71,   124,     5,
     124,   415,    27,   126,   118,    85,   124,   124,    80,    85,
      79,   124,    79,    79,    30,    93,    93,    36,    89,   124,
       4,     5,     6,     7,   124,     5,    91,   441,   442,    83,
      46,    47,   126,    40,   124,   475,    52,    21,    22,    23,
      40,    57,     7,    98,   125,    86,   460,    92,   462,    33,
      34,   465,     7,   308,   442,   167,   126,    41,    42,    75,
     462,   124,   124,   206,    48,    49,    50,   104,   232,   483,
     269,    55,    56,   487,   233,   267,   135,   475,    62,    95,
     168,    65,   139,   101,   123,   101,   181,    -1,   181,   384,
      -1,   144,    76,    77,    -1,   406,   112,    -1,    -1,    -1,
      84,    85,   118,    -1,   120,    -1,    -1,    -1,     4,     5,
       6,     7,    -1,    97,    10,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,   107,   108,    21,    22,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,   119,    -1,    33,    34,    -1,
     124,   125,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    -1,    -1,    -1,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    61,    62,    -1,    -1,    65,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,     4,     5,     6,     7,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,   107,   108,    21,    22,    23,    -1,    -1,    -1,   115,
      -1,    -1,    -1,   119,    -1,    33,    34,    -1,   124,     4,
       5,     6,     7,    41,    42,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    -1,    -1,    -1,    21,    22,    56,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    65,    33,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    42,    76,    77,
      -1,    -1,    -1,    48,    49,    50,    84,    85,    -1,    -1,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    62,    -1,    97,
      65,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,   107,
     108,    76,    77,    -1,    -1,    -1,    -1,   115,    -1,    84,
      85,   119,    -1,    -1,    -1,    -1,   124,     4,     5,     6,
       7,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,    -1,   107,   108,    21,    22,    -1,    -1,    -1,    -1,
     115,   116,    -1,    -1,   119,    -1,    33,    34,    -1,   124,
       4,     5,     6,     7,    41,    42,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    -1,    -1,    -1,    21,    22,    56,
      -1,    -1,    -1,    -1,    61,    62,    -1,    -1,    65,    33,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    41,    42,    76,
      77,    -1,    -1,    -1,    48,    49,    50,    84,    85,    -1,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      97,    65,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
     107,   108,    76,    77,    -1,    -1,    -1,    -1,   115,    -1,
      84,    85,   119,    -1,    -1,    -1,    -1,   124,     4,     5,
       6,     7,    -1,    97,    -1,    -1,    -1,   101,    -1,    -1,
     104,    -1,    -1,   107,   108,    21,    22,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,   119,    -1,    33,    34,    -1,
     124,     4,     5,     6,     7,    41,    42,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    -1,    -1,    -1,    21,    22,
      56,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    65,
      33,    34,    -1,    -1,    -1,    -1,    -1,    -1,    41,    42,
      76,    77,    -1,    -1,    -1,    48,    49,    50,    84,    85,
      -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    97,    65,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,   107,   108,    76,    77,    -1,    -1,    -1,    -1,   115,
      -1,    84,    85,   119,    -1,    -1,    -1,    -1,   124,    -1,
       5,    -1,     7,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,   107,   108,    21,    22,    -1,    -1,
      -1,    -1,   115,    33,    34,    -1,   119,    -1,    33,    34,
      -1,   124,    42,    -1,    -1,    -1,    -1,    42,    48,    49,
      50,    -1,    -1,    48,    49,    50,    56,    -1,    -1,    -1,
      -1,    56,    -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    -1,    -1,    -1,    76,    77,    -1,    -1,
      -1,    76,    77,    -1,    84,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,
      -1,    -1,    97,    -1,   104,    -1,    -1,   107,   108,   104,
      -1,    -1,   107,   108,    -1,   115,    -1,    -1,    -1,   119,
     115,    -1,    -1,    -1,   119,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    -1,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,    30,    46,    47,    52,    57,    75,    95,
     101,   112,   118,   120,   129,   130,   131,   132,   133,   134,
     135,   156,   157,   160,   161,   164,   165,   168,   221,   222,
     105,     4,     6,   219,    73,   105,    67,   105,    78,    29,
      55,   169,   219,   166,   167,   181,   219,     0,   120,   122,
      75,   168,   120,   123,     3,   219,    67,   219,   219,   219,
     219,   219,     5,     7,    21,    22,    23,    33,    34,    41,
      42,    48,    49,    50,    56,    62,    65,    76,    77,    84,
      85,    97,   104,   107,   108,   115,   119,   124,   138,   170,
     171,   172,   197,   198,   199,   200,   201,   202,   203,   210,
     212,   215,   219,   102,   126,    31,   124,    78,    28,    57,
       5,    88,   124,   117,   191,   192,   114,   124,   168,     7,
       7,   116,   197,   206,   207,   124,    77,   124,     5,   124,
      77,   197,     5,    67,   173,   126,    21,    22,    31,   216,
     219,    23,    24,   127,   217,    22,   200,    27,   124,   162,
     163,   219,   167,   124,   177,   218,   219,   219,    44,    45,
      44,    45,   118,   158,   219,   136,   137,   219,    10,    61,
     124,   193,   194,   195,   196,   197,   212,   124,   218,   193,
     116,   204,   205,    58,   207,   208,     7,   219,     7,   125,
     177,   179,   182,   201,   219,   191,   172,   219,   198,   199,
     219,    23,    55,   125,   197,   209,   126,   191,    11,   168,
     125,   126,   168,   136,    43,    66,    94,   111,   142,   219,
     219,   124,   124,   145,   125,   126,    77,   138,   196,   177,
     193,   197,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    72,   214,    20,   210,   211,   125,
     106,   197,   205,   208,   197,    59,   125,    67,   125,    31,
     180,   181,    68,    74,    79,    82,    99,   126,   174,   175,
     176,   180,    37,   110,   178,    69,   183,   125,   209,   125,
     126,   163,   197,   125,   219,   124,    80,    80,   124,    53,
      60,   159,   212,   213,   219,   113,   142,   143,   144,   136,
      10,    43,    51,    85,    94,    98,   111,   139,   140,   141,
     125,   194,   195,    17,    18,    19,    72,   197,   124,   177,
     197,    10,    85,   125,   126,   114,   197,   106,    59,   197,
     181,    79,    90,    79,   218,    79,    90,    79,    90,   179,
     176,     7,     7,   180,    40,    71,   184,   125,   197,   193,
     124,   124,   218,     5,    63,    87,    88,   109,   220,   125,
     126,   125,   126,    35,    38,    39,   103,   154,   126,   118,
     146,    85,   124,   210,    80,   219,   139,   197,   124,   177,
       9,   209,    85,   210,   124,   197,   125,   218,    79,   218,
      88,   218,    79,   218,    79,    93,    93,   209,   193,    89,
     185,   125,   218,   218,   125,    53,    60,   212,   124,   155,
     142,    36,    91,   147,   193,   124,     9,   209,   197,   125,
     211,    88,   218,    88,   193,    88,   218,    88,   218,    40,
      83,   186,   125,   125,     5,   220,   149,   150,   151,   152,
     153,   219,   124,    40,   125,   219,   197,   125,   125,   193,
      88,   193,   193,    88,   193,    88,   187,   188,   197,     7,
      98,   125,   126,     7,    29,   124,   219,   149,    70,    96,
     148,   125,   193,   193,   193,   126,    32,    54,   189,   219,
     150,   218,   125,   124,   188,    86,   190,   124,   125,   218,
      64,    81,   218,   125,   125,    92,     7
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
     172,   172,   173,   174,   174,   175,   175,   176,   176,   176,
     176,   176,   176,   176,   176,   177,   178,   178,   178,   179,
     179,   179,   179,   179,   180,   180,   181,   181,   182,   182,
     183,   183,   184,   184,   185,   185,   186,   186,   187,   187,
     188,   189,   189,   189,   190,   190,   190,   191,   191,   192,
     193,   193,   194,   194,   195,   195,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   197,   197,   198,
     198,   199,   199,   200,   200,   200,   200,   200,   200,   201,
     201,   201,   201,   202,   203,   203,   204,   204,   205,   206,
     206,   207,   208,   208,   209,   209,   210,   210,   210,   210,
     210,   210,   210,   211,   211,   212,   212,   213,   213,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   215,
     216,   216,   217,   217,   217,   218,   218,   219,   219,   220,
     220,   220,   220,   221,   222,   222
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
       2,     1,     3,     0,     1,     2,     1,     5,     4,     6,
       5,     6,     5,     6,     5,     3,     0,     3,     3,     2,
       3,     2,     2,     1,     1,     2,     1,     4,     1,     3,
       0,     3,     0,     2,     0,     3,     0,     2,     1,     3,
       3,     0,     1,     1,     0,     2,     2,     0,     1,     2,
       3,     1,     3,     1,     2,     1,     5,     6,     4,     3,
       3,     3,     2,     3,     5,     4,     6,     3,     1,     3,
       1,     2,     1,     1,     1,     1,     1,     1,     3,     3,
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
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1874 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_NAME  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1884 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1894 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1904 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 7: /* TOKEN_UNSIGNED_NUMVAL  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 1914 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 130: /* sql_statement  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1924 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 131: /* quit_statement  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 1934 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 132: /* alter_table_statement  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1944 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 133: /* create_table_statement  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 1954 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 134: /* create_index_statement  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1964 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 135: /* drop_table_statement  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 1974 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 136: /* column_def  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 1984 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 137: /* column_def_commalist  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 1994 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 138: /* data_type  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 2004 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 139: /* column_constraint_def  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 2014 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 140: /* column_constraint_def_list  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2024 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 141: /* opt_column_constraint_def_list  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2034 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 145: /* opt_column_list  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2044 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 146: /* opt_block_properties  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).block_properties_) != nullptr) {
    delete ((*yyvaluep).block_properties_);
  }
}
#line 2054 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 147: /* opt_partition_clause  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).partition_clause_) != nullptr) {
    delete ((*yyvaluep).partition_clause_);
  }
}
#line 2064 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 148: /* partition_type  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2074 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 149: /* key_value_list  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2084 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 150: /* key_value  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 2094 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 151: /* key_string_value  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 2104 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 152: /* key_string_list  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 2114 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 153: /* key_integer_value  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 2124 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* index_type  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2134 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* opt_index_properties  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2144 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* insert_statement  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 2154 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* copy_from_statement  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_statement_) != nullptr) {
    delete ((*yyvaluep).copy_from_statement_);
  }
}
#line 2164 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* opt_copy_from_params  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2174 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* copy_from_params  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2184 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* update_statement  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2194 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* delete_statement  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2204 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* assignment_list  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2214 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 163: /* assignment_item  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2224 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 164: /* select_statement  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_statement_) != nullptr) {
    delete ((*yyvaluep).select_statement_);
  }
}
#line 2234 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 165: /* with_clause  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2244 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 166: /* with_list  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2254 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 167: /* with_list_element  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2264 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* select_query  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2274 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* selection  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2284 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* selection_item_commalist  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2294 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* selection_item  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2304 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* from_clause  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2314 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* subquery_expression  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2324 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* opt_sample_clause  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_sample_clause_) != nullptr) {
    delete ((*yyvaluep).opt_sample_clause_);
  }
}
#line 2334 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* table_reference  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2344 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* table_reference_signature  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2354 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* table_reference_signature_primary  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2364 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* table_reference_commalist  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2374 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* opt_group_by_clause  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2384 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* opt_having_clause  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2394 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* opt_order_by_clause  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2404 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* opt_limit_clause  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2414 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* order_commalist  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2424 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* order_item  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2434 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* opt_order_direction  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2444 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 190: /* opt_nulls_first  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2454 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 191: /* opt_where_clause  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2464 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 192: /* where_clause  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2474 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 193: /* or_expression  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2484 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 194: /* and_expression  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2494 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 195: /* not_expression  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2504 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 196: /* predicate_expression_base  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2514 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 197: /* add_expression  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2524 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 198: /* multiply_expression  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2534 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 199: /* unary_expression  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2544 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 200: /* expression_base  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2554 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 201: /* function_call  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2564 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 202: /* extract_function  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2574 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 203: /* case_expression  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2584 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 204: /* simple_when_clause_list  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_list_);
  }
}
#line 2594 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 205: /* simple_when_clause  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_);
  }
}
#line 2604 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 206: /* searched_when_clause_list  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_list_);
  }
}
#line 2614 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 207: /* searched_when_clause  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_);
  }
}
#line 2624 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 208: /* opt_else_clause  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2634 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 209: /* expression_list  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2644 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 210: /* literal_value  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2654 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 211: /* literal_value_commalist  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2664 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 212: /* attribute_ref  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2674 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 213: /* attribute_ref_list  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2684 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 214: /* comparison_operation  */
#line 557 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2690 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 215: /* unary_operation  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2696 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 216: /* add_operation  */
#line 559 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2702 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 217: /* multiply_operation  */
#line 559 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2708 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 218: /* name_commalist  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 2718 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 219: /* any_name  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2728 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 220: /* boolean_value  */
#line 556 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2734 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 221: /* command  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_) != nullptr) {
    delete ((*yyvaluep).command_);
  }
}
#line 2744 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 222: /* command_argument_list  */
#line 561 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_argument_list_) != nullptr) {
    delete ((*yyvaluep).command_argument_list_);
  }
}
#line 2754 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 570 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3051 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 574 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3060 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 578 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3069 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 582 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3078 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 586 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    YYABORT;
  }
#line 3086 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 589 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 3095 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 596 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3103 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 599 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].copy_from_statement_);
  }
#line 3111 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 602 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 3119 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 605 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3127 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 608 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 3135 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 611 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 3143 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 614 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 3151 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 617 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 3159 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 620 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].select_statement_);
  }
#line 3167 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 623 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 3175 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 629 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3183 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 635 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3195 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 642 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3206 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 648 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3218 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 655 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3230 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 664 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].attribute_definition_list_), (yyvsp[-1].block_properties_), (yyvsp[0].partition_clause_));
  }
#line 3238 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 669 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].key_value_list_)) {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_), (yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].key_value_list_));
    } else {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_));
    }
  }
#line 3250 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 678 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 3258 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 683 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 3266 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 688 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3275 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 692 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3284 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 698 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 3294 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 703 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3302 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 706 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3310 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 709 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3320 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 714 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3328 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 717 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3336 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 720 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3344 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 723 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3352 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 726 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3360 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 729 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3368 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 732 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3376 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 735 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3384 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 738 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3392 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 741 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3409 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 753 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3417 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 756 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3425 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 759 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3448 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 777 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3471 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 797 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3479 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 800 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3487 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 803 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3497 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 808 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3507 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 813 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3518 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 819 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3529 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 825 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3541 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 834 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3550 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 838 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3559 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 844 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3567 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 847 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3575 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 852 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3585 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 857 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3595 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 862 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3607 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 869 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3617 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 876 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3626 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 880 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3635 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 886 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 3643 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 889 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = $1; */
  }
#line 3651 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 894 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_list_) = nullptr;
  }
#line 3659 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 897 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_list_) = (yyvsp[-1].attribute_list_);
  }
#line 3667 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 902 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.block_properties_) = nullptr;
  }
#line 3675 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 905 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].key_value_list_));
  }
#line 3683 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 910 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.partition_clause_) = nullptr;
  }
#line 3691 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 913 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3713 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 932 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::PartitionSchemeHeader::PartitionType::kHash));
  }
#line 3722 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 936 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::PartitionSchemeHeader::PartitionType::kRange));
  }
#line 3731 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 942 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3740 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 946 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3749 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 952 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 3757 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 955 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 3765 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 958 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 3773 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 963 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 3781 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 966 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_),
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 3791 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 973 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 3799 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 978 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 3813 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 989 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    // Defaults to BitWeavingV, but IndexProperties can change this to H.
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBitWeavingV));
  }
#line 3823 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 994 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
#line 3832 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 998 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
#line 3841 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 1002 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kSMA));
  }
#line 3850 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 1008 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 3858 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 1011 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 3866 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 1017 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 3879 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 1025 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertTuple((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 3887 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 1028 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-3]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), nullptr);
  }
#line 3895 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 1031 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-4]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), (yyvsp[-4].with_list_));
  }
#line 3903 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 1037 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_statement_) = new quickstep::ParseStatementCopyFrom((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].copy_from_params_));
  }
#line 3911 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 1042 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = nullptr;
  }
#line 3919 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 1045 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-1].copy_from_params_);
  }
#line 3927 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 1050 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3936 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 1054 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3945 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 1058 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3954 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 1062 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3963 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 1068 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 3971 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 1073 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 3979 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 1078 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3988 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 1082 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3997 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 1088 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 4005 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 1094 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].select_query_), nullptr);
  }
#line 4013 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 1097 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].select_query_), (yyvsp[-1].with_list_));
  }
#line 4021 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 1102 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 4029 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 1107 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4038 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 1111 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4047 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 1117 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 4056 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 1124 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].selection_), (yyvsp[-5].table_reference_list_), (yyvsp[-4].predicate_), (yyvsp[-3].opt_group_by_clause_), (yyvsp[-2].opt_having_clause_), (yyvsp[-1].opt_order_by_clause_), (yyvsp[0].opt_limit_clause_));
  }
#line 4064 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 1129 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 4072 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 1132 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "ALL in selection");
    YYERROR;
  }
#line 4081 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 1136 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "DISTINCT in selection");
    YYERROR;
  }
#line 4090 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 1142 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4098 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 1145 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 4106 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 1150 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4115 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 1154 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4124 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 1160 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 4132 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 1163 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 4140 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 1166 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 4148 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 1171 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = (yyvsp[-1].table_reference_list_);
  }
#line 4156 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 1176 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 4164 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 1179 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4173 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 1185 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[-1]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4182 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 1189 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4191 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 1195 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4202 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 1201 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4213 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 1207 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4224 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 1213 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4235 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 1219 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4246 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 1225 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4257 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 1231 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4268 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 1237 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4279 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 1245 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].select_query_));
  }
#line 4287 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 1250 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_sample_clause_) = NULL;
  }
#line 4295 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 1253 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, true, (yyvsp[-1].numeric_literal_value_));
  }
#line 4303 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 1256 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].numeric_literal_value_));
  }
#line 4311 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 1261 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4320 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 1265 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].opt_sample_clause_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4329 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 1269 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].opt_sample_clause_));
  }
#line 4337 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 1272 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4346 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 1276 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].function_call_));
  }
#line 4354 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 1281 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4362 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 1284 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4370 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 1289 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4378 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 1292 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4386 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 148:
#line 1297 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4395 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 149:
#line 1301 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4404 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 150:
#line 1307 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 4412 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 1310 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4420 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 1315 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4428 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 1318 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4436 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 1323 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4444 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 1326 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4452 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 1331 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4460 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 1334 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4482 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 158:
#line 1353 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4491 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 159:
#line 1357 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4500 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 1363 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 4510 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 161:
#line 1370 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4518 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 1373 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4526 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 163:
#line 1376 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4534 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 1381 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4542 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 165:
#line 1384 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4550 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 166:
#line 1387 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4558 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 1393 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 4566 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 168:
#line 1396 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4574 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 169:
#line 1401 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4582 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 1406 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4596 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 171:
#line 1415 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4604 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 172:
#line 1420 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4618 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 173:
#line 1429 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4626 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 174:
#line 1434 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4634 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 175:
#line 1437 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4642 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 176:
#line 1442 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4650 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 177:
#line 1445 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 4660 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 178:
#line 1450 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4671 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 1456 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4682 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 180:
#line 1462 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4690 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 181:
#line 1465 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 4698 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 1468 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateExists((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].subquery_expression_));
  }
#line 4706 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 183:
#line 1471 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-2].expression_), (yyvsp[0].subquery_expression_));
  }
#line 4714 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 184:
#line 1474 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-1].expression_list_));
  }
#line 4722 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 185:
#line 1477 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-2]).first_line,
        (yylsp[-2]).first_column,
        new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-3].expression_), (yyvsp[0].subquery_expression_)));
  }
#line 4733 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 186:
#line 1483 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line,
        (yylsp[-4]).first_column,
        new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-1].expression_list_)));
  }
#line 4744 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 187:
#line 1492 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4752 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 1495 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4760 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 189:
#line 1500 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4768 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 190:
#line 1503 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4776 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 191:
#line 1508 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].unary_operation_), (yyvsp[0].expression_));
  }
#line 4784 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 192:
#line 1511 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4792 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 193:
#line 1516 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 4800 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 194:
#line 1519 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 4808 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 195:
#line 1522 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 4816 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 196:
#line 1525 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4824 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 197:
#line 1528 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4832 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 198:
#line 1531 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 4840 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 199:
#line 1536 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 4849 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 200:
#line 1540 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 4858 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 201:
#line 1544 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, false, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4866 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 202:
#line 1547 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-4]).first_line, (yylsp[-4]).first_column, true, (yyvsp[-4].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4874 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 203:
#line 1552 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseExtractFunction((yylsp[-5]).first_line, (yylsp[-5]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].expression_));
  }
#line 4882 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 204:
#line 1557 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseSimpleCaseExpression((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].expression_), (yyvsp[-2].simple_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 4890 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 205:
#line 1560 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseSearchedCaseExpression((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].searched_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 4898 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 206:
#line 1565 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.simple_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSimpleWhenClause>;
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 4907 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 1569 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.simple_when_clause_list_) = (yyvsp[-1].simple_when_clause_list_);
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 4916 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 208:
#line 1575 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.simple_when_clause_) = new quickstep::ParseSimpleWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4924 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 209:
#line 1580 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.searched_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSearchedWhenClause>;
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 4933 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 210:
#line 1584 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.searched_when_clause_list_) = (yyvsp[-1].searched_when_clause_list_);
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 4942 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 211:
#line 1590 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.searched_when_clause_) = new quickstep::ParseSearchedWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].predicate_), (yyvsp[0].expression_));
  }
#line 4950 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 212:
#line 1595 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = NULL;
  }
#line 4958 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 213:
#line 1598 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4966 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 214:
#line 1603 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4975 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 215:
#line 1607 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4984 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 216:
#line 1613 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4992 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 217:
#line 1616 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5000 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 218:
#line 1619 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5008 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 219:
#line 1622 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 5025 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 220:
#line 1634 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 5034 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 221:
#line 1638 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 5058 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 222:
#line 1657 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 5076 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 223:
#line 1672 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5085 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 224:
#line 1676 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5094 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 225:
#line 1682 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 5102 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 226:
#line 1685 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 5110 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 227:
#line 1690 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_list_) = new quickstep::PtrList<quickstep::ParseAttribute>();
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5119 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 228:
#line 1694 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_list_) = (yyvsp[-2].attribute_list_);
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5128 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 229:
#line 1701 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 5136 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 230:
#line 1704 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 5144 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 231:
#line 1707 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 5152 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 232:
#line 1710 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 5160 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 233:
#line 1713 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 5168 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 234:
#line 1716 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 5176 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 235:
#line 1719 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLike);
  }
#line 5184 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 236:
#line 1722 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotLike);
  }
#line 5192 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 237:
#line 1725 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kRegexMatch);
  }
#line 5200 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 238:
#line 1728 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotRegexMatch);
  }
#line 5208 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 239:
#line 1733 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
#line 5222 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 240:
#line 1744 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
#line 5230 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 241:
#line 1747 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
#line 5238 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 242:
#line 1752 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kModulo);
  }
#line 5246 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 243:
#line 1755 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
#line 5254 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 244:
#line 1758 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
#line 5262 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 245:
#line 1764 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 5271 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 246:
#line 1768 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 5280 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 247:
#line 1774 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5288 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 248:
#line 1777 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5299 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 249:
#line 1785 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5307 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 250:
#line 1788 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5315 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 251:
#line 1791 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5323 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 252:
#line 1794 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5331 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 253:
#line 1800 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.command_) = new quickstep::ParseCommand((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].command_argument_list_));
  }
#line 5339 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 254:
#line 1805 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    quickstep::PtrVector<quickstep::ParseString> *argument_list = (yyvsp[-1].command_argument_list_);
    argument_list->push_back((yyvsp[0].string_value_));
    (yyval.command_argument_list_) = argument_list;
  }
#line 5349 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 255:
#line 1810 "../SqlParser.ypp" /* yacc.c:1646  */
    { /* Epsilon, an empy match. */
    (yyval.command_argument_list_) = new quickstep::PtrVector<quickstep::ParseString>();
  }
#line 5357 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;


#line 5361 "SqlParser_gen.cpp" /* yacc.c:1646  */
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
#line 1814 "../SqlParser.ypp" /* yacc.c:1906  */


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
