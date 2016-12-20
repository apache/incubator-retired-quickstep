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
#include "parser/ParsePriority.hpp"
#include "parser/ParserUtil.hpp"
#include "parser/ParseSample.hpp"
#include "parser/ParseSelect.hpp"
#include "parser/ParseSelectionClause.hpp"
#include "parser/ParseSetOperation.hpp"
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

#line 157 "SqlParser_gen.cpp" /* yacc.c:339  */

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
    TOKEN_ALL = 278,
    TOKEN_UNION = 279,
    TOKEN_INTERSECT = 280,
    TOKEN_ADD = 281,
    TOKEN_ALTER = 282,
    TOKEN_AS = 283,
    TOKEN_ASC = 284,
    TOKEN_BIGINT = 285,
    TOKEN_BIT = 286,
    TOKEN_BITWEAVING = 287,
    TOKEN_BLOCKPROPERTIES = 288,
    TOKEN_BLOCKSAMPLE = 289,
    TOKEN_BLOOM_FILTER = 290,
    TOKEN_CSB_TREE = 291,
    TOKEN_BY = 292,
    TOKEN_CASE = 293,
    TOKEN_CHARACTER = 294,
    TOKEN_CHECK = 295,
    TOKEN_COLUMN = 296,
    TOKEN_CONSTRAINT = 297,
    TOKEN_COPY = 298,
    TOKEN_CREATE = 299,
    TOKEN_CURRENT = 300,
    TOKEN_DATE = 301,
    TOKEN_DATETIME = 302,
    TOKEN_DAY = 303,
    TOKEN_DECIMAL = 304,
    TOKEN_DEFAULT = 305,
    TOKEN_DELETE = 306,
    TOKEN_DELIMITER = 307,
    TOKEN_DESC = 308,
    TOKEN_DISTINCT = 309,
    TOKEN_DOUBLE = 310,
    TOKEN_DROP = 311,
    TOKEN_ELSE = 312,
    TOKEN_END = 313,
    TOKEN_ESCAPE_STRINGS = 314,
    TOKEN_EXISTS = 315,
    TOKEN_EXTRACT = 316,
    TOKEN_FALSE = 317,
    TOKEN_FIRST = 318,
    TOKEN_FLOAT = 319,
    TOKEN_FOLLOWING = 320,
    TOKEN_FOR = 321,
    TOKEN_FOREIGN = 322,
    TOKEN_FROM = 323,
    TOKEN_FULL = 324,
    TOKEN_GROUP = 325,
    TOKEN_HASH = 326,
    TOKEN_HAVING = 327,
    TOKEN_HOUR = 328,
    TOKEN_IN = 329,
    TOKEN_INDEX = 330,
    TOKEN_INNER = 331,
    TOKEN_INSERT = 332,
    TOKEN_INTEGER = 333,
    TOKEN_INTERVAL = 334,
    TOKEN_INTO = 335,
    TOKEN_JOIN = 336,
    TOKEN_KEY = 337,
    TOKEN_LAST = 338,
    TOKEN_LEFT = 339,
    TOKEN_LIMIT = 340,
    TOKEN_LONG = 341,
    TOKEN_MINUTE = 342,
    TOKEN_MONTH = 343,
    TOKEN_NULL = 344,
    TOKEN_NULLS = 345,
    TOKEN_OFF = 346,
    TOKEN_ON = 347,
    TOKEN_ORDER = 348,
    TOKEN_OUTER = 349,
    TOKEN_OVER = 350,
    TOKEN_PARTITION = 351,
    TOKEN_PARTITIONS = 352,
    TOKEN_PERCENT = 353,
    TOKEN_PRECEDING = 354,
    TOKEN_PRIMARY = 355,
    TOKEN_PRIORITY = 356,
    TOKEN_QUIT = 357,
    TOKEN_RANGE = 358,
    TOKEN_REAL = 359,
    TOKEN_REFERENCES = 360,
    TOKEN_RIGHT = 361,
    TOKEN_ROW = 362,
    TOKEN_ROW_DELIMITER = 363,
    TOKEN_ROWS = 364,
    TOKEN_SECOND = 365,
    TOKEN_SELECT = 366,
    TOKEN_SET = 367,
    TOKEN_SMA = 368,
    TOKEN_SMALLINT = 369,
    TOKEN_SUBSTRING = 370,
    TOKEN_TABLE = 371,
    TOKEN_THEN = 372,
    TOKEN_TIME = 373,
    TOKEN_TIMESTAMP = 374,
    TOKEN_TRUE = 375,
    TOKEN_TUPLESAMPLE = 376,
    TOKEN_UNBOUNDED = 377,
    TOKEN_UNIQUE = 378,
    TOKEN_UPDATE = 379,
    TOKEN_USING = 380,
    TOKEN_VALUES = 381,
    TOKEN_VARCHAR = 382,
    TOKEN_WHEN = 383,
    TOKEN_WHERE = 384,
    TOKEN_WINDOW = 385,
    TOKEN_WITH = 386,
    TOKEN_YEAR = 387,
    TOKEN_YEARMONTH = 388,
    TOKEN_EOF = 389,
    TOKEN_LEX_ERROR = 390
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 121 "../SqlParser.ypp" /* yacc.c:355  */

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

  quickstep::ParseSetOperation *set_operation_;

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
  quickstep::ParseStatementSetOperation *set_operation_statement_;
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

  quickstep::ParsePriority *opt_priority_clause_;

#line 435 "SqlParser_gen.cpp" /* yacc.c:355  */
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
#line 223 "../SqlParser.ypp" /* yacc.c:358  */

/* This header needs YYSTYPE, which is defined by the %union directive above */
#include "SqlLexer_gen.hpp"
void NotSupported(const YYLTYPE *location, yyscan_t yyscanner, const std::string &feature);

#line 470 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYFINAL  49
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1207

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  147
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  109
/* YYNRULES -- Number of rules.  */
#define YYNRULES  293
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  543

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   390

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     142,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   146,     2,     2,
     143,   144,    23,    21,   145,    22,    27,    24,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   141,
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
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   639,   639,   643,   647,   651,   655,   658,   665,   668,
     671,   674,   677,   680,   683,   686,   689,   692,   698,   704,
     711,   717,   724,   733,   738,   747,   752,   757,   761,   767,
     772,   775,   778,   783,   786,   789,   792,   795,   798,   801,
     804,   807,   810,   822,   825,   828,   846,   866,   869,   872,
     877,   882,   888,   894,   903,   907,   913,   916,   921,   926,
     931,   938,   945,   949,   955,   958,   963,   966,   971,   974,
     979,   982,  1001,  1004,  1009,  1013,  1019,  1022,  1025,  1030,
    1033,  1040,  1045,  1056,  1061,  1065,  1069,  1075,  1078,  1084,
    1092,  1095,  1098,  1104,  1109,  1112,  1117,  1121,  1125,  1129,
    1135,  1140,  1145,  1149,  1155,  1164,  1167,  1172,  1175,  1194,
    1199,  1203,  1209,  1215,  1224,  1229,  1237,  1243,  1249,  1252,
    1255,  1260,  1263,  1268,  1272,  1278,  1281,  1284,  1289,  1294,
    1299,  1302,  1305,  1310,  1313,  1316,  1319,  1322,  1325,  1328,
    1331,  1336,  1339,  1344,  1348,  1352,  1355,  1359,  1362,  1367,
    1370,  1375,  1378,  1383,  1387,  1393,  1396,  1401,  1404,  1409,
    1412,  1417,  1420,  1439,  1442,  1447,  1451,  1457,  1463,  1468,
    1471,  1476,  1479,  1484,  1487,  1492,  1495,  1500,  1501,  1504,
    1509,  1510,  1513,  1518,  1522,  1528,  1535,  1538,  1541,  1546,
    1549,  1552,  1558,  1561,  1566,  1571,  1580,  1585,  1594,  1599,
    1602,  1607,  1610,  1615,  1621,  1627,  1630,  1633,  1636,  1639,
    1642,  1648,  1657,  1660,  1665,  1668,  1673,  1676,  1681,  1684,
    1687,  1690,  1694,  1698,  1701,  1704,  1707,  1710,  1715,  1719,
    1723,  1726,  1731,  1736,  1740,  1746,  1749,  1754,  1758,  1764,
    1769,  1773,  1779,  1784,  1787,  1792,  1796,  1802,  1805,  1808,
    1811,  1823,  1827,  1846,  1859,  1874,  1877,  1880,  1883,  1886,
    1889,  1894,  1898,  1904,  1907,  1912,  1916,  1923,  1926,  1929,
    1932,  1935,  1938,  1941,  1944,  1947,  1950,  1955,  1966,  1969,
    1974,  1977,  1980,  1986,  1990,  1996,  1999,  2007,  2010,  2013,
    2016,  2022,  2027,  2032
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
  "TOKEN_ALL", "TOKEN_UNION", "TOKEN_INTERSECT", "TOKEN_ADD",
  "TOKEN_ALTER", "TOKEN_AS", "TOKEN_ASC", "TOKEN_BIGINT", "TOKEN_BIT",
  "TOKEN_BITWEAVING", "TOKEN_BLOCKPROPERTIES", "TOKEN_BLOCKSAMPLE",
  "TOKEN_BLOOM_FILTER", "TOKEN_CSB_TREE", "TOKEN_BY", "TOKEN_CASE",
  "TOKEN_CHARACTER", "TOKEN_CHECK", "TOKEN_COLUMN", "TOKEN_CONSTRAINT",
  "TOKEN_COPY", "TOKEN_CREATE", "TOKEN_CURRENT", "TOKEN_DATE",
  "TOKEN_DATETIME", "TOKEN_DAY", "TOKEN_DECIMAL", "TOKEN_DEFAULT",
  "TOKEN_DELETE", "TOKEN_DELIMITER", "TOKEN_DESC", "TOKEN_DISTINCT",
  "TOKEN_DOUBLE", "TOKEN_DROP", "TOKEN_ELSE", "TOKEN_END",
  "TOKEN_ESCAPE_STRINGS", "TOKEN_EXISTS", "TOKEN_EXTRACT", "TOKEN_FALSE",
  "TOKEN_FIRST", "TOKEN_FLOAT", "TOKEN_FOLLOWING", "TOKEN_FOR",
  "TOKEN_FOREIGN", "TOKEN_FROM", "TOKEN_FULL", "TOKEN_GROUP", "TOKEN_HASH",
  "TOKEN_HAVING", "TOKEN_HOUR", "TOKEN_IN", "TOKEN_INDEX", "TOKEN_INNER",
  "TOKEN_INSERT", "TOKEN_INTEGER", "TOKEN_INTERVAL", "TOKEN_INTO",
  "TOKEN_JOIN", "TOKEN_KEY", "TOKEN_LAST", "TOKEN_LEFT", "TOKEN_LIMIT",
  "TOKEN_LONG", "TOKEN_MINUTE", "TOKEN_MONTH", "TOKEN_NULL", "TOKEN_NULLS",
  "TOKEN_OFF", "TOKEN_ON", "TOKEN_ORDER", "TOKEN_OUTER", "TOKEN_OVER",
  "TOKEN_PARTITION", "TOKEN_PARTITIONS", "TOKEN_PERCENT",
  "TOKEN_PRECEDING", "TOKEN_PRIMARY", "TOKEN_PRIORITY", "TOKEN_QUIT",
  "TOKEN_RANGE", "TOKEN_REAL", "TOKEN_REFERENCES", "TOKEN_RIGHT",
  "TOKEN_ROW", "TOKEN_ROW_DELIMITER", "TOKEN_ROWS", "TOKEN_SECOND",
  "TOKEN_SELECT", "TOKEN_SET", "TOKEN_SMA", "TOKEN_SMALLINT",
  "TOKEN_SUBSTRING", "TOKEN_TABLE", "TOKEN_THEN", "TOKEN_TIME",
  "TOKEN_TIMESTAMP", "TOKEN_TRUE", "TOKEN_TUPLESAMPLE", "TOKEN_UNBOUNDED",
  "TOKEN_UNIQUE", "TOKEN_UPDATE", "TOKEN_USING", "TOKEN_VALUES",
  "TOKEN_VARCHAR", "TOKEN_WHEN", "TOKEN_WHERE", "TOKEN_WINDOW",
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
  "set_operation_statement", "opt_priority_clause", "with_clause",
  "with_list", "with_list_element", "set_operation_union",
  "set_operation_intersect", "select_query", "opt_all_distinct",
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
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,    59,    10,    40,    41,    44,    37
};
# endif

#define YYPACT_NINF -244

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-244)))

#define YYTABLE_NINF -134

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     807,  -244,  -244,   -55,   203,   -26,    -2,    -6,    43,  -244,
      86,   203,   203,  -244,   136,   125,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,   -22,    -3,   128,  -244,
     -78,   182,   203,  -244,  -244,   130,   203,   203,   203,   203,
     203,  -244,  -244,   633,   107,    89,  -244,   209,   120,  -244,
    -244,  -244,   197,    -3,    86,   161,  -244,   137,  -244,  -244,
    -244,    27,   265,   180,   141,   170,  -244,   -25,  -244,  -244,
     301,   313,  -244,  -244,  -244,   660,   188,  -244,   249,  -244,
    -244,   206,  -244,  -244,   357,  -244,  -244,  -244,  -244,   221,
    -244,  -244,   222,   282,   783,   366,   299,   231,  -244,  -244,
     272,     7,  -244,  -244,   277,  -244,  -244,  -244,  -244,  -244,
     933,    -5,   203,   203,   237,   203,   203,  -244,   137,   374,
    -244,   233,   239,   246,   203,   203,   551,  -244,  -244,   244,
     203,  -244,  -244,  -244,   551,    47,    40,  -244,   376,  -244,
     153,   153,   960,   381,  -244,     8,    24,  -244,    23,   170,
     960,  -244,  -244,   203,   960,  -244,  -244,  -244,  -244,   960,
      36,   313,  -244,   203,   363,    94,  -244,   378,  -244,   137,
    -244,   144,  -244,   137,   128,  -244,   203,   -21,   203,   203,
     248,  -244,   250,  -244,   147,  1069,   810,   237,   469,   387,
     388,  -244,  -244,   329,   380,  1042,   152,    45,   960,    48,
    -244,   960,  -244,   333,   257,  -244,  -244,  -244,  -244,  -244,
    -244,   330,  -244,   168,   260,  -244,  -244,    19,    66,   146,
    -244,   264,    66,    51,   335,  -244,  -244,     7,  -244,   304,
    -244,  -244,   267,   960,  -244,   279,   158,   203,  -244,   960,
    -244,   203,  -244,  -244,   270,   331,   334,   273,  -244,  -244,
    -244,   165,   203,   290,   -21,   203,  -244,    77,  -244,  -244,
       2,    64,   551,   551,   193,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,   960,   281,   960,    31,  -244,   162,   294,
     960,    71,  -244,   364,   279,  -244,  -244,   960,   419,  -244,
     110,   203,  -244,  -244,   332,  -244,   336,   337,   342,    23,
    -244,   423,   426,    66,   392,   361,   397,   296,   343,  -244,
     166,  -244,   960,  -244,   279,  -244,   551,   300,   302,   203,
     437,   119,   169,  -244,   172,   421,    60,  -244,   305,   316,
    -244,   355,   310,  1042,  -244,   368,   203,  -244,  -244,    77,
    -244,  -244,   388,  -244,  -244,  -244,   960,   315,    95,   783,
    -244,   279,   362,  -244,  -244,  1042,   317,   279,   960,  -244,
      26,   -15,  -244,  -244,  -244,  -244,  -244,    23,   146,   356,
     358,  -244,   960,   551,   365,   960,  -244,   420,   111,  -244,
     279,     4,   203,   203,   174,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,   201,  -244,   203,  -244,  -244,  -244,  -244,   321,
     -21,   427,   369,  -244,   551,  -244,  -244,   323,  -244,   238,
     783,  -244,   960,   179,  -244,  -244,  1042,   279,  -244,   460,
    -244,   384,  -244,  -244,   326,   387,   435,   394,   326,   960,
    -244,  -244,  -244,   459,  -244,   181,   183,  -244,   480,   119,
    -244,   203,  -244,  -244,   345,   447,  -244,     6,   203,   960,
     185,   279,  -244,   192,   349,   551,   960,   489,   367,   353,
    -244,   240,    12,   389,  -244,  -244,  -244,   208,  -244,  -244,
    -244,  -244,    11,   203,    22,  -244,   359,   279,  -244,  -244,
    -244,   387,   353,  -244,   203,  -244,   367,  -244,   960,  -244,
    -244,   405,   404,   398,   407,   500,   203,  -244,   203,  -244,
    -244,   203,  -244,   210,  -244,  -244,   371,  -244,   482,  -244,
    -244,   129,  -244,  -244,  -244,  -244,    13,   373,  -244,   212,
    -244,   203,   375,  -244,  -244,   449,   410,   454,  -244,   203,
    -244,   214,   304,  -244,  -244,  -244,   216,   424,   383,  -244,
     521,  -244,  -244
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     6,   293,     0,     0,     0,     0,     0,     0,    18,
     118,     0,     0,     7,     0,     0,    15,     8,    10,    11,
      13,    14,     9,    17,    12,    16,     0,   107,   114,   116,
       0,   291,     0,   285,   286,     0,     0,     0,     0,     0,
       0,   119,   120,     0,     0,   109,   110,     0,   151,     1,
       3,     2,     0,   107,   118,     0,   105,     0,     5,     4,
     292,     0,     0,     0,     0,   192,    25,     0,   251,   248,
       0,   277,   121,    40,    29,     0,     0,    30,    31,    34,
      36,     0,    37,    39,     0,    41,   247,    35,    38,     0,
      32,    33,     0,     0,     0,     0,     0,   122,   123,   227,
     127,   213,   215,   217,   220,   223,   224,   225,   219,   218,
       0,   263,     0,     0,     0,     0,     0,   106,     0,     0,
     115,     0,     0,    94,     0,     0,     0,   101,   193,     0,
       0,    91,   249,   250,     0,     0,   243,   240,     0,    43,
       0,   252,     0,     0,    44,     0,     0,   254,     0,   192,
       0,   278,   279,     0,     0,   126,   281,   282,   280,     0,
       0,     0,   216,     0,     0,   192,   103,     0,   111,     0,
     112,     0,   283,     0,   113,   108,     0,     0,     0,     0,
       0,    93,    66,    27,     0,     0,     0,     0,     0,   194,
     196,   198,   200,     0,   218,     0,     0,     0,     0,   243,
     237,     0,   241,     0,     0,   257,   258,   259,   256,   260,
     255,     0,   253,     0,     0,   129,   226,     0,     0,   153,
     142,   128,   147,   130,   155,   124,   125,   212,   214,   169,
     221,   264,     0,     0,   228,   245,     0,     0,   100,     0,
     152,     0,    92,    19,     0,     0,     0,     0,    20,    21,
      22,     0,     0,     0,    64,     0,    42,    56,   199,   207,
       0,     0,     0,     0,     0,   267,   269,   270,   271,   272,
     268,   273,   275,     0,     0,     0,     0,   261,     0,     0,
       0,     0,   238,     0,   244,   236,    45,     0,     0,    46,
     133,     0,   143,   149,   139,   134,   135,   137,     0,     0,
     146,     0,     0,   145,     0,   157,     0,     0,   171,   229,
       0,   230,     0,   102,   104,   284,     0,     0,     0,     0,
       0,     0,     0,   265,     0,   263,     0,    63,    65,    68,
      28,     0,     0,     0,    47,     0,     0,    49,    55,    57,
      26,   206,   195,   197,   274,   276,     0,     0,     0,     0,
     208,   205,     0,   204,    90,     0,     0,   242,     0,   235,
       0,     0,   148,   150,   140,   136,   138,     0,   154,     0,
       0,   144,     0,     0,   159,     0,   222,     0,   173,   231,
     246,     0,     0,     0,     0,    96,   289,   290,   288,   287,
      97,    95,     0,    67,     0,    83,    84,    85,    86,    87,
       0,     0,    70,    48,     0,    51,    50,     0,    54,     0,
       0,   210,     0,     0,   203,   262,     0,   239,   232,     0,
     233,     0,   131,   132,   156,   158,     0,   161,   170,     0,
     176,   175,   168,     0,    61,     0,     0,    58,     0,     0,
     266,     0,    24,    62,     0,     0,    23,     0,     0,     0,
       0,   201,   209,     0,     0,     0,     0,     0,   163,   172,
     183,   186,     0,     0,    59,    98,    99,     0,    74,    76,
      77,    78,     0,     0,     0,    52,     0,   202,   211,    89,
     234,   141,   160,   162,     0,   117,   164,   165,     0,   187,
     188,   189,     0,     0,     0,     0,     0,    88,     0,    82,
      80,     0,    79,     0,    72,    73,     0,    53,     0,   166,
     184,     0,   185,   177,   179,   178,     0,     0,    75,     0,
      69,     0,     0,   190,   191,     0,     0,     0,   174,     0,
      81,     0,   169,   180,   182,   181,     0,     0,     0,    60,
       0,   167,    71
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -132,  -244,
     346,   191,  -244,  -244,  -243,  -244,  -244,  -244,  -244,  -244,
    -244,    59,    35,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,   303,  -244,   483,  -244,  -244,
     428,   200,   425,    -8,   485,  -244,  -244,   395,  -244,   -98,
    -244,  -244,  -165,   175,  -179,   -10,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,    58,     5,  -244,  -244,  -244,  -244,  -244,
    -244,    90,    61,  -244,  -244,    39,  -244,  -133,   285,   287,
     379,   -37,   400,   403,   438,  -144,  -244,  -244,  -244,  -244,
     352,  -244,   430,   360,  -212,  -186,   429,   148,  -121,  -244,
    -244,  -244,  -244,  -244,  -127,    -4,   132,  -244,  -244
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    14,    15,    16,    17,    18,    19,    20,   183,   184,
      95,   338,   339,   340,   248,   328,   329,   253,   402,   446,
     506,   467,   468,   469,   470,   471,   399,   442,    21,    22,
     181,   322,    23,    24,   165,   166,    25,    56,    26,    45,
      46,   145,    28,    29,    43,    96,    97,    98,   149,    99,
     303,   298,   219,   220,   292,   293,   221,   305,   374,   427,
     458,   485,   486,   487,   307,   308,   378,   432,   433,   495,
     528,   459,   460,   491,   512,   127,   128,   189,   190,   191,
     192,   193,   101,   102,   103,   104,   105,   106,   107,   199,
     200,   136,   137,   203,   236,   108,   211,   278,   109,   324,
     275,   110,   154,   159,   171,   111,   390,    30,    31
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      35,   197,    47,   196,   222,   194,   100,    44,    48,   277,
     262,   327,   262,   194,   262,    33,   170,    34,   499,   492,
     525,   310,   163,    33,   244,    34,    54,    33,    61,    34,
     156,   157,    63,    64,    65,    66,    67,    54,   135,   500,
      33,   352,    34,   300,   243,   151,   152,   151,   152,   120,
     218,   245,   290,   262,    36,   260,   419,   146,   121,   131,
      52,    58,   493,   526,    59,   194,    32,   194,   151,   152,
      33,    38,    34,   222,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   246,   151,   152,   331,   122,   259,
     301,    10,   151,   152,    10,    37,   155,   395,   504,   291,
     396,   397,   201,    47,   412,   213,   129,   247,   167,    48,
     201,   172,   173,   100,    41,    39,   151,   152,   130,   218,
     182,   185,   332,   330,   371,   353,   172,   235,    40,   420,
     505,   323,   333,    55,   368,    10,    49,   413,   164,   494,
     527,   194,   194,   274,   223,    42,   341,   405,   434,   226,
     475,   261,   215,   158,   501,   222,   230,   443,    57,   231,
     424,   281,   217,   428,   284,   242,   217,   280,   216,   415,
     418,   334,   185,   134,   249,   250,   350,   302,   398,   229,
     198,   198,   335,   381,   294,    60,   386,   336,   224,   151,
     152,   295,   384,   358,   164,   194,   235,   523,   450,   296,
      27,   218,   314,    62,   238,   337,   205,    33,   216,    34,
     344,   345,   346,   223,    48,   387,   388,   524,    48,   430,
     294,   297,   320,   222,   112,   431,    53,   295,   126,   321,
     277,   206,  -133,   167,   113,   296,   348,   315,   351,   237,
     425,   288,   114,   357,   389,   207,   208,   449,   325,   411,
     360,   185,   194,    10,   362,   435,   436,   297,   438,   151,
     152,   151,   152,   115,    50,   439,    51,   119,   209,   218,
     123,   447,   347,   440,   489,   380,    33,   124,    34,   176,
     177,   363,   116,   194,   125,   178,   179,    48,   240,   241,
     210,   254,   255,   151,   152,   223,   279,   241,   490,    48,
     151,   152,   311,   312,   126,   153,   354,   355,   132,   409,
     379,   312,   235,   391,   392,   172,   393,   394,   437,   241,
     133,   417,   481,   452,   312,   463,   241,   464,   241,   478,
     312,   138,   407,   139,   194,   235,   479,   355,   235,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   140,
     151,   152,   497,   498,   520,   498,   530,   241,   537,   241,
     539,   241,   141,   223,   142,   143,   144,    33,    68,    34,
      69,   147,   148,   235,   519,   451,   150,   160,   172,   172,
     169,   175,   180,   204,    70,    71,   232,   195,   214,   239,
     325,   251,   461,   252,   531,   262,   285,   263,    73,    74,
     276,   286,   536,   287,   289,   306,    75,    76,   274,   299,
     304,   309,   477,   316,    77,    78,   319,    79,   317,   461,
     326,   318,   233,    80,   349,   356,   361,   359,   367,    81,
     369,   364,    82,   370,   372,   365,   366,   472,   373,   375,
     376,   377,   385,   382,   476,   383,    83,    84,   163,   403,
     400,   461,   401,   404,    85,   406,   414,    86,   410,   422,
     416,   423,   429,   426,   441,   444,   448,   454,   502,   472,
     445,   312,    87,    33,    68,    34,    69,   456,   462,   186,
     508,   455,    88,    89,   457,   465,    90,    91,   473,   474,
      70,    71,   517,   480,   472,    92,   483,   172,   488,   496,
     511,    93,   484,   507,    73,    74,    94,   234,   513,   516,
     514,   515,    75,    76,   521,   522,   529,   172,   532,   533,
      77,    78,   534,    79,   535,   172,   540,   541,   542,    80,
     408,   257,   503,   518,   187,    81,   117,   538,    82,   118,
     313,   168,   421,   174,   509,   225,   482,   342,   162,   510,
     343,   282,    83,    84,   227,    33,    68,    34,    69,   283,
      85,   186,   228,    86,   453,   258,   202,     0,     0,     0,
     212,   466,    70,    71,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,    10,    73,    74,    88,    89,
       0,     0,    90,    91,    75,    76,     0,     0,     0,     0,
       0,    92,    77,    78,     0,    79,     0,    93,     0,     0,
       0,    80,   188,     0,     0,     0,   187,    81,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,     0,    33,    68,    34,
      69,     0,    85,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,     0,     0,     0,
      87,     0,     0,     0,    33,    68,    34,    69,    73,    74,
      88,    89,     0,     0,    90,    91,    75,    76,     0,     0,
       0,    70,    71,    92,    77,    78,     0,    79,     0,    93,
       0,     0,     0,    80,   188,    73,    74,     0,     0,    81,
       0,     0,    82,    75,    76,     0,     0,     0,     0,     0,
       0,    77,    78,     0,    79,     0,    83,    84,     0,     0,
      80,     0,     0,     0,    85,     0,    81,    86,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    83,    84,     0,     0,     0,     0,     0,
       0,    85,    88,    89,    86,     0,    90,    91,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,    87,
       0,    93,     0,     0,     0,     0,    94,     0,     0,    88,
      89,     0,     0,    90,    91,     0,     0,    33,    68,    34,
      69,     0,    92,   134,     0,     0,     0,     0,    93,     0,
       0,     0,     0,    94,    70,    71,     0,     0,     1,     0,
       2,     0,     0,     0,    33,    68,    34,    69,    73,    74,
       0,     0,     0,     0,     0,     0,    75,    76,     0,     0,
       0,    70,    71,     0,    77,    78,     0,    79,     0,     3,
       0,     0,     0,    80,     0,    73,    74,     0,     0,    81,
       0,     0,    82,    75,    76,     4,     5,     0,     0,     0,
       0,    77,    78,     6,    79,     0,    83,    84,     7,     0,
      80,     0,     0,     0,    85,   187,    81,    86,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     8,
       0,     0,    87,    83,    84,     0,     0,     0,     0,    10,
       0,    85,    88,    89,    86,     0,    90,    91,     0,     0,
       0,     0,     0,     0,     9,    92,     0,     0,     0,    87,
       0,    93,     0,    10,     0,     0,    94,     0,     0,    88,
      89,     0,     0,    90,    91,     0,    11,    33,    68,    34,
      69,     0,    92,    12,     0,     0,    13,     0,    93,     0,
       0,     0,     0,   188,    70,   161,     0,     0,     0,     0,
       0,     0,     0,     0,    33,    68,    34,    69,    73,    74,
       0,     0,     0,     0,     0,     0,    75,    76,     0,     0,
       0,    70,    71,     0,    77,    78,     0,    79,     0,     0,
       0,     0,     0,    80,     0,    73,    74,     0,     0,    81,
       0,     0,    82,    75,    76,     0,     0,     0,     0,     0,
       0,    77,    78,     0,    79,     0,    83,    84,     0,     0,
      80,     0,     0,     0,    85,     0,    81,    86,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    83,    84,     0,     0,    68,     0,    69,
       0,    85,    88,    89,    86,     0,    90,    91,     0,     0,
       0,     0,     0,    70,   161,    92,     0,     0,     0,    87,
       0,    93,     0,     0,     0,     0,    94,    73,    74,    88,
      89,     0,     0,    90,    91,     0,    76,     0,     0,     0,
       0,     0,    92,    77,    78,     0,    79,     0,    93,     0,
       0,     0,    80,    94,    73,    74,     0,     0,     0,     0,
       0,    82,     0,    76,     0,     0,     0,     0,     0,     0,
      77,    78,     0,    79,     0,    83,    84,     0,     0,    80,
       0,     0,     0,    85,     0,     0,    86,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    83,   256,     0,     0,     0,     0,     0,     0,
      85,    88,     0,     0,     0,    90,    91,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,    87,     0,
      93,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,    90,    91,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,    93
};

static const yytype_int16 yycheck[] =
{
       4,   134,    12,   130,   148,   126,    43,    11,    12,   195,
       8,   254,     8,   134,     8,     4,   114,     6,     7,     7,
       7,   233,    27,     4,    45,     6,    29,     4,    32,     6,
      23,    24,    36,    37,    38,    39,    40,    29,    75,    28,
       4,    10,     6,   222,   176,    21,    22,    21,    22,    57,
     148,    72,   217,     8,    80,   188,    71,    94,    31,    67,
      82,   139,    50,    50,   142,   186,   121,   188,    21,    22,
       4,    73,     6,   217,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,   105,    21,    22,    10,    61,   187,
      39,   116,    21,    22,   116,   121,   100,    37,    76,    33,
      40,    41,    62,   113,     9,   142,   131,   128,   112,   113,
      62,   115,   116,   150,    28,   121,    21,    22,   143,   217,
     124,   125,    45,   255,   303,    94,   130,   164,    85,   144,
     108,   252,    55,   136,   299,   116,     0,   349,   143,   127,
     127,   262,   263,    79,   148,    59,   144,   333,   144,   153,
     144,   188,   144,   146,   143,   299,   160,   400,    30,   163,
     372,   198,   143,   375,   201,   173,   143,   122,   144,   355,
     144,    94,   176,   133,   178,   179,   274,   126,   118,   143,
     133,   133,   105,   316,    74,     3,    67,   110,   149,    21,
      22,    81,   319,   122,   143,   316,   233,    68,   410,    89,
       0,   299,   239,    73,   165,   128,    53,     4,   144,     6,
      17,    18,    19,   217,   218,    96,    97,    88,   222,   108,
      74,   111,    57,   367,   117,   114,    26,    81,   134,    64,
     416,    78,    86,   237,   145,    89,   273,   241,   275,   145,
     373,    73,    33,   280,   125,    92,    93,     9,   252,   347,
     287,   255,   373,   116,   144,   382,   383,   111,    57,    21,
      22,    21,    22,   143,   139,    64,   141,   106,   115,   367,
       5,   404,    79,   394,    34,   312,     4,    97,     6,    46,
      47,   291,    85,   404,   143,    46,    47,   291,   144,   145,
     137,   144,   145,    21,    22,   299,   144,   145,    58,   303,
      21,    22,   144,   145,   134,    33,   144,   145,     7,   346,
     144,   145,   349,   144,   145,   319,   144,   145,   144,   145,
       7,   358,   455,   144,   145,   144,   145,   144,   145,   144,
     145,   143,   336,    84,   455,   372,   144,   145,   375,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,   143,
      21,    22,   144,   145,   144,   145,   144,   145,   144,   145,
     144,   145,     5,   367,   143,   143,    84,     4,     5,     6,
       7,     5,    73,   410,   501,   412,   145,   100,   382,   383,
     143,     7,   136,     7,    21,    22,    23,   143,     7,    11,
     394,   143,   429,   143,   521,     8,    63,     9,    35,    36,
      20,   144,   529,    73,   144,   101,    43,    44,    79,   145,
      75,   144,   449,   143,    51,    52,   143,    54,    87,   456,
     130,    87,    59,    60,   143,   131,     7,    63,    86,    66,
       7,    99,    69,     7,    42,    99,    99,   441,    77,    42,
     144,    98,     5,   143,   448,   143,    83,    84,    27,    94,
     145,   488,   136,   143,    91,    87,    94,    94,   143,   103,
     143,   103,    42,    98,   143,    38,   143,     7,   472,   473,
     101,   145,   109,     4,     5,     6,     7,    42,    19,    10,
     484,    97,   119,   120,    90,     5,   123,   124,   143,    42,
      21,    22,   496,   144,   498,   132,     7,   501,   145,   110,
      95,   138,   135,   144,    35,    36,   143,   144,   104,     9,
     112,   104,    43,    44,   143,    33,   143,   521,   143,    70,
      51,    52,   112,    54,    70,   529,   102,   144,     7,    60,
     339,   185,   473,   498,    65,    66,    53,   532,    69,    54,
     237,   113,   367,   118,   486,   150,   456,   262,   110,   488,
     263,   199,    83,    84,   154,     4,     5,     6,     7,   199,
      91,    10,   159,    94,   416,   186,   136,    -1,    -1,    -1,
     141,   439,    21,    22,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    35,    36,   119,   120,
      -1,    -1,   123,   124,    43,    44,    -1,    -1,    -1,    -1,
      -1,   132,    51,    52,    -1,    54,    -1,   138,    -1,    -1,
      -1,    60,   143,    -1,    -1,    -1,    65,    66,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    83,    84,    -1,     4,     5,     6,
       7,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    23,    -1,    -1,    -1,
     109,    -1,    -1,    -1,     4,     5,     6,     7,    35,    36,
     119,   120,    -1,    -1,   123,   124,    43,    44,    -1,    -1,
      -1,    21,    22,   132,    51,    52,    -1,    54,    -1,   138,
      -1,    -1,    -1,    60,   143,    35,    36,    -1,    -1,    66,
      -1,    -1,    69,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    52,    -1,    54,    -1,    83,    84,    -1,    -1,
      60,    -1,    -1,    -1,    91,    -1,    66,    94,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    91,   119,   120,    94,    -1,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,   109,
      -1,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,   119,
     120,    -1,    -1,   123,   124,    -1,    -1,     4,     5,     6,
       7,    -1,   132,   133,    -1,    -1,    -1,    -1,   138,    -1,
      -1,    -1,    -1,   143,    21,    22,    -1,    -1,     1,    -1,
       3,    -1,    -1,    -1,     4,     5,     6,     7,    35,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    43,    44,    -1,    -1,
      -1,    21,    22,    -1,    51,    52,    -1,    54,    -1,    32,
      -1,    -1,    -1,    60,    -1,    35,    36,    -1,    -1,    66,
      -1,    -1,    69,    43,    44,    48,    49,    -1,    -1,    -1,
      -1,    51,    52,    56,    54,    -1,    83,    84,    61,    -1,
      60,    -1,    -1,    -1,    91,    65,    66,    94,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      -1,    -1,   109,    83,    84,    -1,    -1,    -1,    -1,   116,
      -1,    91,   119,   120,    94,    -1,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,   107,   132,    -1,    -1,    -1,   109,
      -1,   138,    -1,   116,    -1,    -1,   143,    -1,    -1,   119,
     120,    -1,    -1,   123,   124,    -1,   129,     4,     5,     6,
       7,    -1,   132,   136,    -1,    -1,   139,    -1,   138,    -1,
      -1,    -1,    -1,   143,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     4,     5,     6,     7,    35,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    43,    44,    -1,    -1,
      -1,    21,    22,    -1,    51,    52,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    35,    36,    -1,    -1,    66,
      -1,    -1,    69,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    52,    -1,    54,    -1,    83,    84,    -1,    -1,
      60,    -1,    -1,    -1,    91,    -1,    66,    94,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    83,    84,    -1,    -1,     5,    -1,     7,
      -1,    91,   119,   120,    94,    -1,   123,   124,    -1,    -1,
      -1,    -1,    -1,    21,    22,   132,    -1,    -1,    -1,   109,
      -1,   138,    -1,    -1,    -1,    -1,   143,    35,    36,   119,
     120,    -1,    -1,   123,   124,    -1,    44,    -1,    -1,    -1,
      -1,    -1,   132,    51,    52,    -1,    54,    -1,   138,    -1,
      -1,    -1,    60,   143,    35,    36,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      51,    52,    -1,    54,    -1,    83,    84,    -1,    -1,    60,
      -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      91,   119,    -1,    -1,    -1,   123,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,   109,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
      -1,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,   138
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,    32,    48,    49,    56,    61,    82,   107,
     116,   129,   136,   139,   148,   149,   150,   151,   152,   153,
     154,   175,   176,   179,   180,   183,   185,   188,   189,   190,
     254,   255,   121,     4,     6,   252,    80,   121,    73,   121,
      85,    28,    59,   191,   252,   186,   187,   202,   252,     0,
     139,   141,    82,   188,    29,   136,   184,    30,   139,   142,
       3,   252,    73,   252,   252,   252,   252,   252,     5,     7,
      21,    22,    23,    35,    36,    43,    44,    51,    52,    54,
      60,    66,    69,    83,    84,    91,    94,   109,   119,   120,
     123,   124,   132,   138,   143,   157,   192,   193,   194,   196,
     228,   229,   230,   231,   232,   233,   234,   235,   242,   245,
     248,   252,   117,   145,    33,   143,    85,   184,   191,   106,
     190,    31,    61,     5,    97,   143,   134,   222,   223,   131,
     143,   190,     7,     7,   133,   228,   238,   239,   143,    84,
     143,     5,   143,   143,    84,   188,   228,     5,    73,   195,
     145,    21,    22,    33,   249,   252,    23,    24,   146,   250,
     100,    22,   231,    27,   143,   181,   182,   252,   187,   143,
     196,   251,   252,   252,   189,     7,    46,    47,    46,    47,
     136,   177,   252,   155,   156,   252,    10,    65,   143,   224,
     225,   226,   227,   228,   245,   143,   251,   224,   133,   236,
     237,    62,   239,   240,     7,    53,    78,    92,    93,   115,
     137,   243,   243,   228,     7,   144,   144,   143,   196,   199,
     200,   203,   232,   252,   222,   194,   252,   229,   230,   143,
     252,   252,    23,    59,   144,   228,   241,   145,   222,    11,
     144,   145,   190,   155,    45,    72,   105,   128,   161,   252,
     252,   143,   143,   164,   144,   145,    84,   157,   227,   196,
     224,   228,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    79,   247,    20,   242,   244,   144,
     122,   228,   237,   240,   228,    63,   144,    73,    73,   144,
     199,    33,   201,   202,    74,    81,    89,   111,   198,   145,
     201,    39,   126,   197,    75,   204,   101,   211,   212,   144,
     241,   144,   145,   182,   228,   252,   143,    87,    87,   143,
      57,    64,   178,   245,   246,   252,   130,   161,   162,   163,
     155,    10,    45,    55,    94,   105,   110,   128,   158,   159,
     160,   144,   225,   226,    17,    18,    19,    79,   228,   143,
     196,   228,    10,    94,   144,   145,   131,   228,   122,    63,
     228,     7,   144,   202,    99,    99,    99,    86,   199,     7,
       7,   201,    42,    77,   205,    42,   144,    98,   213,   144,
     228,   224,   143,   143,   251,     5,    67,    96,    97,   125,
     253,   144,   145,   144,   145,    37,    40,    41,   118,   173,
     145,   136,   165,    94,   143,   242,    87,   252,   158,   228,
     143,   196,     9,   241,    94,   242,   143,   228,   144,    71,
     144,   200,   103,   103,   241,   224,    98,   206,   241,    42,
     108,   114,   214,   215,   144,   251,   251,   144,    57,    64,
     245,   143,   174,   161,    38,   101,   166,   224,   143,     9,
     241,   228,   144,   244,     7,    97,    42,    90,   207,   218,
     219,   228,    19,   144,   144,     5,   253,   168,   169,   170,
     171,   172,   252,   143,    42,   144,   252,   228,   144,   144,
     144,   224,   218,     7,   135,   208,   209,   210,   145,    34,
      58,   220,     7,    50,   127,   216,   110,   144,   145,     7,
      28,   143,   252,   168,    76,   108,   167,   144,   252,   210,
     219,    95,   221,   104,   112,   104,     9,   252,   169,   251,
     144,   143,    33,    68,    88,     7,    50,   127,   217,   143,
     144,   251,   143,    70,   112,    70,   251,   144,   211,   144,
     102,   144,     7
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   147,   148,   148,   148,   148,   148,   148,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   150,   151,
     151,   151,   151,   152,   153,   154,   155,   156,   156,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   158,   158,   158,
     158,   158,   158,   158,   159,   159,   160,   160,   161,   161,
     161,   161,   162,   162,   163,   163,   164,   164,   165,   165,
     166,   166,   167,   167,   168,   168,   169,   169,   169,   170,
     170,   171,   172,   173,   173,   173,   173,   174,   174,   175,
     175,   175,   175,   176,   177,   177,   178,   178,   178,   178,
     179,   180,   181,   181,   182,   183,   183,   184,   184,   185,
     186,   186,   187,   188,   188,   189,   189,   190,   191,   191,
     191,   192,   192,   193,   193,   194,   194,   194,   195,   196,
     197,   197,   197,   198,   198,   198,   198,   198,   198,   198,
     198,   199,   199,   200,   200,   200,   200,   200,   200,   201,
     201,   202,   202,   203,   203,   204,   204,   205,   205,   206,
     206,   207,   207,   208,   208,   209,   209,   210,   211,   212,
     212,   213,   213,   214,   214,   215,   215,   216,   216,   216,
     217,   217,   217,   218,   218,   219,   220,   220,   220,   221,
     221,   221,   222,   222,   223,   224,   224,   225,   225,   226,
     226,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   228,   228,   229,   229,   230,   230,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   232,   232,
     232,   232,   233,   234,   234,   235,   235,   236,   236,   237,
     238,   238,   239,   240,   240,   241,   241,   242,   242,   242,
     242,   242,   242,   242,   242,   243,   243,   243,   243,   243,
     243,   244,   244,   245,   245,   246,   246,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   248,   249,   249,
     250,   250,   250,   251,   251,   252,   252,   253,   253,   253,
     253,   254,   255,   255
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
       5,     4,     3,     1,     3,     2,     3,     0,     3,     2,
       1,     3,     3,     4,     1,     3,     1,    10,     0,     1,
       1,     1,     1,     1,     3,     3,     2,     1,     2,     3,
       0,     3,     3,     0,     1,     1,     2,     1,     2,     1,
       2,     6,     1,     2,     3,     2,     2,     1,     3,     1,
       2,     1,     4,     1,     3,     0,     3,     0,     2,     0,
       3,     0,     2,     0,     1,     1,     2,     6,     3,     0,
       3,     0,     3,     0,     5,     1,     1,     2,     2,     2,
       2,     2,     2,     1,     3,     3,     0,     1,     1,     0,
       2,     2,     0,     1,     2,     3,     1,     3,     1,     2,
       1,     5,     6,     4,     3,     3,     3,     2,     3,     5,
       4,     6,     3,     1,     3,     1,     2,     1,     1,     1,
       1,     3,     5,     1,     1,     1,     3,     1,     3,     4,
       4,     5,     6,     6,     8,     5,     4,     1,     2,     4,
       1,     2,     4,     0,     2,     1,     3,     1,     1,     2,
       2,     1,     2,     3,     2,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     2,     2,     0
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
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1966 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_NAME  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1976 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1986 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1996 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 7: /* TOKEN_UNSIGNED_NUMVAL  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2006 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 149: /* sql_statement  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2016 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 150: /* quit_statement  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 2026 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 151: /* alter_table_statement  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2036 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 152: /* create_table_statement  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 2046 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 153: /* create_index_statement  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 2056 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* drop_table_statement  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 2066 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* column_def  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 2076 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* column_def_commalist  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 2086 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* data_type  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 2096 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* column_constraint_def  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 2106 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* column_constraint_def_list  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2116 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* opt_column_constraint_def_list  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2126 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 164: /* opt_column_list  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2136 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 165: /* opt_block_properties  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).block_properties_) != nullptr) {
    delete ((*yyvaluep).block_properties_);
  }
}
#line 2146 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 166: /* opt_partition_clause  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).partition_clause_) != nullptr) {
    delete ((*yyvaluep).partition_clause_);
  }
}
#line 2156 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 167: /* partition_type  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2166 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* key_value_list  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2176 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* key_value  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 2186 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* key_string_value  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 2196 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* key_string_list  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 2206 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* key_integer_value  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 2216 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* index_type  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2226 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* opt_index_properties  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2236 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* insert_statement  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 2246 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* copy_from_statement  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_statement_) != nullptr) {
    delete ((*yyvaluep).copy_from_statement_);
  }
}
#line 2256 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* opt_copy_from_params  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2266 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* copy_from_params  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2276 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* update_statement  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2286 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* delete_statement  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2296 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* assignment_list  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2306 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* assignment_item  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2316 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* set_operation_statement  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_statement_) != nullptr) {
    delete ((*yyvaluep).set_operation_statement_);
  }
}
#line 2326 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* opt_priority_clause  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_priority_clause_) != nullptr) {
    delete ((*yyvaluep).opt_priority_clause_);
  }
}
#line 2336 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* with_clause  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2346 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* with_list  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2356 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* with_list_element  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2366 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* set_operation_union  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_) != nullptr) {
    delete ((*yyvaluep).set_operation_);
  }
}
#line 2376 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* set_operation_intersect  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_) != nullptr) {
    delete ((*yyvaluep).set_operation_);
  }
}
#line 2386 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 190: /* select_query  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2396 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 191: /* opt_all_distinct  */
#line 624 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2402 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 192: /* selection  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2412 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 193: /* selection_item_commalist  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2422 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 194: /* selection_item  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2432 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 195: /* from_clause  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2442 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 196: /* subquery_expression  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2452 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 197: /* opt_sample_clause  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_sample_clause_) != nullptr) {
    delete ((*yyvaluep).opt_sample_clause_);
  }
}
#line 2462 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 198: /* join_type  */
#line 628 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2468 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 199: /* joined_table_reference  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2478 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 200: /* table_reference  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2488 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 201: /* table_reference_signature  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2498 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 202: /* table_reference_signature_primary  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2508 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 203: /* joined_table_reference_commalist  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2518 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 204: /* opt_group_by_clause  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2528 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 205: /* opt_having_clause  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2538 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 206: /* opt_order_by_clause  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2548 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 207: /* opt_limit_clause  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2558 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 208: /* opt_window_clause  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_window_clause_) != nullptr) {
    delete ((*yyvaluep).opt_window_clause_);
  }
}
#line 2568 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 209: /* window_declaration_list  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_window_clause_) != nullptr) {
    delete ((*yyvaluep).opt_window_clause_);
  }
}
#line 2578 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 210: /* window_declaration  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_definition_) != nullptr) {
    delete ((*yyvaluep).window_definition_);
  }
}
#line 2588 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 211: /* window_definition  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_definition_) != nullptr) {
    delete ((*yyvaluep).window_definition_);
  }
}
#line 2598 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 212: /* opt_window_partition  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_partition_by_list_) != nullptr) {
    delete ((*yyvaluep).window_partition_by_list_);
  }
}
#line 2608 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 213: /* opt_window_order  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_order_by_list_) != nullptr) {
    delete ((*yyvaluep).window_order_by_list_);
  }
}
#line 2618 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 214: /* opt_window_frame  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_frame_info_) != nullptr) {
    delete ((*yyvaluep).window_frame_info_);
  }
}
#line 2628 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 215: /* frame_mode  */
#line 624 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2634 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 216: /* frame_preceding  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2644 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 217: /* frame_following  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2654 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 218: /* order_commalist  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2664 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 219: /* order_item  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2674 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 220: /* opt_order_direction  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2684 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 221: /* opt_nulls_first  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2694 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 222: /* opt_where_clause  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2704 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 223: /* where_clause  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2714 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 224: /* or_expression  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2724 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 225: /* and_expression  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2734 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 226: /* not_expression  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2744 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 227: /* predicate_expression_base  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2754 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 228: /* add_expression  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2764 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 229: /* multiply_expression  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2774 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 230: /* unary_expression  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2784 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 231: /* expression_base  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2794 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 232: /* function_call  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2804 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 233: /* extract_function  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2814 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 234: /* substr_function  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2824 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 235: /* case_expression  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2834 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 236: /* simple_when_clause_list  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_list_);
  }
}
#line 2844 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 237: /* simple_when_clause  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_);
  }
}
#line 2854 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 238: /* searched_when_clause_list  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_list_);
  }
}
#line 2864 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 239: /* searched_when_clause  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_);
  }
}
#line 2874 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 240: /* opt_else_clause  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2884 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 241: /* expression_list  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2894 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 242: /* literal_value  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2904 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 243: /* datetime_unit  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2914 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 244: /* literal_value_commalist  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2924 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 245: /* attribute_ref  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2934 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 246: /* attribute_ref_list  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2944 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 247: /* comparison_operation  */
#line 625 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2950 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 248: /* unary_operation  */
#line 626 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2956 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 249: /* add_operation  */
#line 627 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2962 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 250: /* multiply_operation  */
#line 627 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2968 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 251: /* name_commalist  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 2978 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 252: /* any_name  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2988 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 253: /* boolean_value  */
#line 624 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2994 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 254: /* command  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_) != nullptr) {
    delete ((*yyvaluep).command_);
  }
}
#line 3004 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 255: /* command_argument_list  */
#line 630 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_argument_list_) != nullptr) {
    delete ((*yyvaluep).command_argument_list_);
  }
}
#line 3014 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 639 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3311 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 643 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3320 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 647 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3329 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 651 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3338 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 655 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    YYABORT;
  }
#line 3346 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 658 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 3355 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 665 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3363 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 668 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].copy_from_statement_);
  }
#line 3371 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 671 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 3379 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 674 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3387 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 677 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 3395 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 680 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 3403 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 683 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 3411 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 686 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 3419 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 689 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].set_operation_statement_);
  }
#line 3427 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 692 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 3435 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 698 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3443 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 704 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3455 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 711 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3466 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 717 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3478 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 724 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3490 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 733 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].attribute_definition_list_), (yyvsp[-1].block_properties_), (yyvsp[0].partition_clause_));
  }
#line 3498 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 738 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].key_value_list_)) {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_), (yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].key_value_list_));
    } else {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_));
    }
  }
#line 3510 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 747 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 3518 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 752 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 3526 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 757 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3535 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 761 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3544 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 767 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 3554 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 772 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDate));
  }
#line 3562 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 775 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3570 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 778 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3580 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 783 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3588 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 786 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3596 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 789 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3604 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 792 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3612 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 795 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3620 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 798 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3628 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 801 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3636 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 804 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3644 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 807 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3652 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 810 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3669 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 822 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3677 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 825 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3685 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 828 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3708 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 846 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3731 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 866 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3739 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 869 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3747 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 872 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3757 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 877 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3767 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 882 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3778 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 888 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3789 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 894 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3801 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 903 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3810 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 907 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3819 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 913 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3827 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 916 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3835 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 921 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3845 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 926 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3855 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 931 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3867 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 938 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3877 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 945 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3886 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 949 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3895 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 955 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 3903 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 958 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = $1; */
  }
#line 3911 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 963 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_list_) = nullptr;
  }
#line 3919 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 966 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_list_) = (yyvsp[-1].attribute_list_);
  }
#line 3927 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 971 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.block_properties_) = nullptr;
  }
#line 3935 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 974 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].key_value_list_));
  }
#line 3943 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 979 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.partition_clause_) = nullptr;
  }
#line 3951 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 982 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3973 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 1001 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::kHashPartitionType);
  }
#line 3981 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 1004 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::kRangePartitionType);
  }
#line 3989 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 1009 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3998 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 1013 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 4007 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 1019 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 4015 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 1022 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 4023 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 1025 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 4031 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 1030 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 4039 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 1033 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_),
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 4049 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 1040 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4057 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 1045 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 4071 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 1056 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    // Defaults to BitWeavingV, but IndexProperties can change this to H.
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBitWeavingV));
  }
#line 4081 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 1061 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
#line 4090 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 1065 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
#line 4099 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 1069 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kSMA));
  }
#line 4108 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 1075 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 4116 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 1078 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 4124 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 1084 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 4137 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 1092 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertTuple((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 4145 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 1095 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-3]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), nullptr);
  }
#line 4153 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 1098 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-4]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), (yyvsp[-4].with_list_));
  }
#line 4161 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 1104 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_statement_) = new quickstep::ParseStatementCopyFrom((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].copy_from_params_));
  }
#line 4169 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 1109 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = nullptr;
  }
#line 4177 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 1112 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-1].copy_from_params_);
  }
#line 4185 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 1117 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 4194 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 1121 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 4203 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 1125 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 4212 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 1129 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 4221 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 1135 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 4229 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 1140 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 4237 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 1145 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 4246 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 1149 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 4255 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 1155 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 4263 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 1164 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.set_operation_statement_) = new quickstep::ParseStatementSetOperation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].set_operation_), nullptr, (yyvsp[0].opt_priority_clause_));
  }
#line 4271 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 1167 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.set_operation_statement_) = new quickstep::ParseStatementSetOperation((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].set_operation_), (yyvsp[-2].with_list_), (yyvsp[0].opt_priority_clause_));
  }
#line 4279 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 1172 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_priority_clause_) = nullptr;
  }
#line 4287 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 1175 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.opt_priority_clause_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "PRIORITY value must be an integer");
      YYERROR;
    } else {
      if ((yyvsp[0].numeric_literal_value_)->long_value() <= 0) {
        delete (yyvsp[0].numeric_literal_value_);
        (yyval.opt_priority_clause_) = nullptr;
        quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "PRIORITY value must be positive");
        YYERROR;
      } else {
        (yyval.opt_priority_clause_) = new quickstep::ParsePriority((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].numeric_literal_value_));
      }
    }
  }
#line 4309 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 1194 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 4317 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 1199 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4326 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 1203 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4335 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 1209 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 4344 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 1215 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-1].boolean_value_)) {
      (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-3]).first_line, (yylsp[-3]).first_column, quickstep::ParseSetOperation::kUnion);
    } else {
      (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-3]).first_line, (yylsp[-3]).first_column, quickstep::ParseSetOperation::kUnionAll);
    }
    (yyval.set_operation_)->addOperand((yyvsp[-3].set_operation_));
    (yyval.set_operation_)->addOperand((yyvsp[0].set_operation_));
  }
#line 4358 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 1224 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.set_operation_) = (yyvsp[0].set_operation_);
  }
#line 4366 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 1229 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-2]).first_line, (yylsp[-2]).first_column, quickstep::ParseSetOperation::kIntersect);
    quickstep::ParseSetOperation *op = new quickstep::ParseSetOperation(
        (yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::ParseSetOperation::kSelect);
    op->addOperand((yyvsp[0].select_query_));
    (yyval.set_operation_)->addOperand((yyvsp[-2].set_operation_));
    (yyval.set_operation_)->addOperand(op);
  }
#line 4379 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 1237 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::ParseSetOperation::kSelect);
    (yyval.set_operation_)->addOperand((yyvsp[0].select_query_));
  }
#line 4388 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 1244 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-9]).first_line, (yylsp[-9]).first_column, (yyvsp[-7].selection_), (yyvsp[-6].table_reference_list_), (yyvsp[-5].predicate_), (yyvsp[-4].opt_group_by_clause_), (yyvsp[-3].opt_having_clause_), (yyvsp[-2].opt_order_by_clause_), (yyvsp[-1].opt_limit_clause_), (yyvsp[0].opt_window_clause_));
  }
#line 4396 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 1249 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;  // Distinct
  }
#line 4404 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 1252 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;  // All
  }
#line 4412 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 1255 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;  // Distinct
  }
#line 4420 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 1260 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4428 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 1263 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 4436 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 1268 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4445 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 1272 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4454 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 1278 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 4462 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 1281 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 4470 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 1284 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 4478 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 1289 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = (yyvsp[0].table_reference_list_);
  }
#line 4486 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 1294 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].set_operation_));
  }
#line 4494 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 1299 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_sample_clause_) = NULL;
  }
#line 4502 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 1302 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, true, (yyvsp[-1].numeric_literal_value_));
  }
#line 4510 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 1305 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].numeric_literal_value_));
  }
#line 4518 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 1310 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4526 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 1313 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4534 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 1316 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4542 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 1319 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4550 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 1322 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4558 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 1325 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4566 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 1328 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4574 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 1331 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4582 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 1336 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseJoinedTableReference((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].join_type_), (yyvsp[-5].table_reference_), (yyvsp[-2].table_reference_), (yyvsp[0].predicate_));
  }
#line 4590 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 1339 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = (yyvsp[0].table_reference_);
  }
#line 4598 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 1344 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4607 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 1348 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].opt_sample_clause_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4616 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 1352 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].opt_sample_clause_));
  }
#line 4624 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 1355 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4633 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 1359 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].function_call_));
  }
#line 4641 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 148:
#line 1362 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = (yyvsp[-1].table_reference_);
  }
#line 4649 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 149:
#line 1367 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4657 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 150:
#line 1370 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4665 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 1375 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4673 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 1378 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4681 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 1383 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4690 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 1387 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4699 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 1393 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 4707 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 1396 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4715 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 1401 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4723 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 158:
#line 1404 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4731 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 159:
#line 1409 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4739 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 1412 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4747 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 161:
#line 1417 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4755 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 1420 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4777 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 163:
#line 1439 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_window_clause_) = nullptr;
  }
#line 4785 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 1442 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_window_clause_) = (yyvsp[0].opt_window_clause_);
  }
#line 4793 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 165:
#line 1447 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_window_clause_) = new quickstep::PtrList<quickstep::ParseWindow>();
    (yyval.opt_window_clause_)->push_back((yyvsp[0].window_definition_));
  }
#line 4802 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 166:
#line 1451 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_window_clause_) = (yyvsp[-1].opt_window_clause_);
    (yyval.opt_window_clause_)->push_back((yyvsp[0].window_definition_));
  }
#line 4811 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 1457 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.window_definition_) = (yyvsp[-1].window_definition_);
    (yyval.window_definition_)->setName((yyvsp[-4].string_value_));
  }
#line 4820 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 168:
#line 1463 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.window_definition_) = new quickstep::ParseWindow((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].window_partition_by_list_), (yyvsp[-1].window_order_by_list_), (yyvsp[0].window_frame_info_));
  }
#line 4828 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 169:
#line 1468 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.window_partition_by_list_) = nullptr;
  }
#line 4836 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 1471 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.window_partition_by_list_) = (yyvsp[0].expression_list_);
  }
#line 4844 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 171:
#line 1476 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.window_order_by_list_) = nullptr;
  }
#line 4852 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 172:
#line 1479 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.window_order_by_list_) = (yyvsp[0].order_commalist_);
  }
#line 4860 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 173:
#line 1484 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.window_frame_info_) = nullptr;
  }
#line 4868 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 174:
#line 1487 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.window_frame_info_) = new quickstep::ParseFrameInfo((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-4].boolean_value_), (yyvsp[-2].numeric_literal_value_)->long_value(), (yyvsp[0].numeric_literal_value_)->long_value());
  }
#line 4876 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 175:
#line 1492 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4884 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 176:
#line 1495 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;
  }
#line 4892 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 178:
#line 1501 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "-1");
  }
#line 4900 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 1504 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "0");
  }
#line 4908 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 181:
#line 1510 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "-1");
  }
#line 4916 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 1513 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "0");
  }
#line 4924 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 183:
#line 1518 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4933 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 184:
#line 1522 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4942 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 185:
#line 1528 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 4952 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 186:
#line 1535 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4960 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 187:
#line 1538 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4968 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 1541 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4976 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 189:
#line 1546 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4984 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 190:
#line 1549 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4992 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 191:
#line 1552 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 5000 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 192:
#line 1558 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 5008 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 193:
#line 1561 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5016 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 194:
#line 1566 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5024 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 195:
#line 1571 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 5038 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 196:
#line 1580 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5046 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 197:
#line 1585 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 5060 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 198:
#line 1594 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5068 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 199:
#line 1599 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 5076 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 200:
#line 1602 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 5084 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 201:
#line 1607 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5092 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 202:
#line 1610 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 5102 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 203:
#line 1615 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 5113 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 204:
#line 1621 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 5124 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 205:
#line 1627 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5132 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 206:
#line 1630 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 5140 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 1633 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateExists((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].subquery_expression_));
  }
#line 5148 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 208:
#line 1636 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-2].expression_), (yyvsp[0].subquery_expression_));
  }
#line 5156 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 209:
#line 1639 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-1].expression_list_));
  }
#line 5164 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 210:
#line 1642 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-2]).first_line,
        (yylsp[-2]).first_column,
        new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-3].expression_), (yyvsp[0].subquery_expression_)));
  }
#line 5175 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 211:
#line 1648 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line,
        (yylsp[-4]).first_column,
        new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-1].expression_list_)));
  }
#line 5186 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 212:
#line 1657 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5194 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 213:
#line 1660 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5202 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 214:
#line 1665 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5210 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 215:
#line 1668 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5218 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 216:
#line 1673 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].unary_operation_), (yyvsp[0].expression_));
  }
#line 5226 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 217:
#line 1676 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5234 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 218:
#line 1681 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 5242 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 219:
#line 1684 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 5250 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 220:
#line 1687 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 5258 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 221:
#line 1690 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyvsp[-2].function_call_)->setWindowName((yyvsp[0].string_value_));
    (yyval.expression_) = (yyvsp[-2].function_call_);
  }
#line 5267 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 222:
#line 1694 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyvsp[-4].function_call_)->setWindow((yyvsp[-1].window_definition_));
    (yyval.expression_) = (yyvsp[-4].function_call_);
  }
#line 5276 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 223:
#line 1698 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5284 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 224:
#line 1701 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5292 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 225:
#line 1704 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5300 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 226:
#line 1707 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 5308 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 227:
#line 1710 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].subquery_expression_);
  }
#line 5316 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 228:
#line 1715 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 5325 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 229:
#line 1719 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 5334 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 230:
#line 1723 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, false, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 5342 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 231:
#line 1726 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-4]).first_line, (yylsp[-4]).first_column, true, (yyvsp[-4].string_value_), (yyvsp[-1].expression_list_));
  }
#line 5350 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 232:
#line 1731 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseExtractFunction((yylsp[-5]).first_line, (yylsp[-5]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].expression_));
  }
#line 5358 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 233:
#line 1736 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseSubstringFunction(
        (yylsp[-5]).first_line, (yylsp[-5]).first_column, (yyvsp[-3].expression_), (yyvsp[-1].numeric_literal_value_)->long_value());
  }
#line 5367 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 234:
#line 1740 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseSubstringFunction(
        (yylsp[-7]).first_line, (yylsp[-7]).first_column, (yyvsp[-5].expression_), (yyvsp[-3].numeric_literal_value_)->long_value(), (yyvsp[-1].numeric_literal_value_)->long_value());
  }
#line 5376 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 235:
#line 1746 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseSimpleCaseExpression((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].expression_), (yyvsp[-2].simple_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 5384 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 236:
#line 1749 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseSearchedCaseExpression((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].searched_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 5392 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 237:
#line 1754 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.simple_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSimpleWhenClause>;
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 5401 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 238:
#line 1758 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.simple_when_clause_list_) = (yyvsp[-1].simple_when_clause_list_);
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 5410 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 239:
#line 1764 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.simple_when_clause_) = new quickstep::ParseSimpleWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5418 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 240:
#line 1769 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.searched_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSearchedWhenClause>;
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 5427 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 241:
#line 1773 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.searched_when_clause_list_) = (yyvsp[-1].searched_when_clause_list_);
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 5436 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 242:
#line 1779 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.searched_when_clause_) = new quickstep::ParseSearchedWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].predicate_), (yyvsp[0].expression_));
  }
#line 5444 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 243:
#line 1784 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = NULL;
  }
#line 5452 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 244:
#line 1787 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5460 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 245:
#line 1792 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 5469 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 246:
#line 1796 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 5478 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 247:
#line 1802 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 5486 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 248:
#line 1805 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5494 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 249:
#line 1808 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5502 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 250:
#line 1811 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 5519 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 251:
#line 1823 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 5528 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 252:
#line 1827 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 5552 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 253:
#line 1846 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 5570 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 254:
#line 1859 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 5588 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 255:
#line 1874 "../SqlParser.ypp" /* yacc.c:1646  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("YEAR"));
  }
#line 5596 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 256:
#line 1877 "../SqlParser.ypp" /* yacc.c:1646  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("MONTH"));
  }
#line 5604 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 257:
#line 1880 "../SqlParser.ypp" /* yacc.c:1646  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("DAY"));
  }
#line 5612 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 258:
#line 1883 "../SqlParser.ypp" /* yacc.c:1646  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("HOUR"));
  }
#line 5620 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 259:
#line 1886 "../SqlParser.ypp" /* yacc.c:1646  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("MINUTE"));
  }
#line 5628 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 260:
#line 1889 "../SqlParser.ypp" /* yacc.c:1646  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("SECOND"));
  }
#line 5636 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 261:
#line 1894 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5645 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 262:
#line 1898 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5654 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 263:
#line 1904 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 5662 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 264:
#line 1907 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 5670 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 265:
#line 1912 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_list_) = new quickstep::PtrList<quickstep::ParseAttribute>();
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5679 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 266:
#line 1916 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_list_) = (yyvsp[-2].attribute_list_);
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5688 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 267:
#line 1923 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 5696 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 268:
#line 1926 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 5704 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 269:
#line 1929 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 5712 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 270:
#line 1932 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 5720 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 271:
#line 1935 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 5728 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 272:
#line 1938 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 5736 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 273:
#line 1941 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLike);
  }
#line 5744 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 274:
#line 1944 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotLike);
  }
#line 5752 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 275:
#line 1947 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kRegexMatch);
  }
#line 5760 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 276:
#line 1950 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotRegexMatch);
  }
#line 5768 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 277:
#line 1955 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
#line 5782 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 278:
#line 1966 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
#line 5790 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 279:
#line 1969 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
#line 5798 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 280:
#line 1974 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kModulo);
  }
#line 5806 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 281:
#line 1977 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
#line 5814 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 282:
#line 1980 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
#line 5822 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 283:
#line 1986 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 5831 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 284:
#line 1990 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 5840 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 285:
#line 1996 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5848 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 286:
#line 1999 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5859 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 287:
#line 2007 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5867 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 288:
#line 2010 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5875 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 289:
#line 2013 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5883 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 290:
#line 2016 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5891 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 291:
#line 2022 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.command_) = new quickstep::ParseCommand((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].command_argument_list_));
  }
#line 5899 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 292:
#line 2027 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    quickstep::PtrVector<quickstep::ParseString> *argument_list = (yyvsp[-1].command_argument_list_);
    argument_list->push_back((yyvsp[0].string_value_));
    (yyval.command_argument_list_) = argument_list;
  }
#line 5909 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 293:
#line 2032 "../SqlParser.ypp" /* yacc.c:1646  */
    { /* Epsilon, an empy match. */
    (yyval.command_argument_list_) = new quickstep::PtrVector<quickstep::ParseString>();
  }
#line 5917 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;


#line 5921 "SqlParser_gen.cpp" /* yacc.c:1646  */
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
#line 2036 "../SqlParser.ypp" /* yacc.c:1906  */


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
