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
#include "parser/ParseDataType.hpp"
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
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/PtrList.hpp"
#include "utility/PtrVector.hpp"

// Needed for Bison 2.6 and higher, which do not automatically provide this typedef.
typedef void* yyscan_t;

#line 152 "SqlParser_gen.cpp" /* yacc.c:339  */

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
    TOKEN_BIT = 285,
    TOKEN_BITWEAVING = 286,
    TOKEN_BLOCKPROPERTIES = 287,
    TOKEN_BLOCKSAMPLE = 288,
    TOKEN_BLOOM_FILTER = 289,
    TOKEN_CSB_TREE = 290,
    TOKEN_BY = 291,
    TOKEN_CASE = 292,
    TOKEN_CAST = 293,
    TOKEN_CHECK = 294,
    TOKEN_COLUMN = 295,
    TOKEN_CONSTRAINT = 296,
    TOKEN_COPY = 297,
    TOKEN_CREATE = 298,
    TOKEN_CURRENT = 299,
    TOKEN_DAY = 300,
    TOKEN_DEFAULT = 301,
    TOKEN_DELETE = 302,
    TOKEN_DESC = 303,
    TOKEN_DISTINCT = 304,
    TOKEN_DOUBLECOLON = 305,
    TOKEN_DROP = 306,
    TOKEN_ELSE = 307,
    TOKEN_END = 308,
    TOKEN_EXISTS = 309,
    TOKEN_EXTRACT = 310,
    TOKEN_FALSE = 311,
    TOKEN_FIRST = 312,
    TOKEN_FOLLOWING = 313,
    TOKEN_FOR = 314,
    TOKEN_FOREIGN = 315,
    TOKEN_FROM = 316,
    TOKEN_FULL = 317,
    TOKEN_GROUP = 318,
    TOKEN_HASH = 319,
    TOKEN_HAVING = 320,
    TOKEN_HOUR = 321,
    TOKEN_IN = 322,
    TOKEN_INDEX = 323,
    TOKEN_INNER = 324,
    TOKEN_INSERT = 325,
    TOKEN_INTERVAL = 326,
    TOKEN_INTO = 327,
    TOKEN_JOIN = 328,
    TOKEN_KEY = 329,
    TOKEN_LAST = 330,
    TOKEN_LBRACE = 331,
    TOKEN_LEFT = 332,
    TOKEN_LIMIT = 333,
    TOKEN_MINUTE = 334,
    TOKEN_MONTH = 335,
    TOKEN_NULL = 336,
    TOKEN_NULLS = 337,
    TOKEN_OFF = 338,
    TOKEN_ON = 339,
    TOKEN_ORDER = 340,
    TOKEN_OUTER = 341,
    TOKEN_OVER = 342,
    TOKEN_PARTITION = 343,
    TOKEN_PARTITIONS = 344,
    TOKEN_PERCENT = 345,
    TOKEN_PRECEDING = 346,
    TOKEN_PRIMARY = 347,
    TOKEN_PRIORITY = 348,
    TOKEN_QUIT = 349,
    TOKEN_RANGE = 350,
    TOKEN_RBRACE = 351,
    TOKEN_REAL = 352,
    TOKEN_REFERENCES = 353,
    TOKEN_RIGHT = 354,
    TOKEN_ROW = 355,
    TOKEN_ROW_DELIMITER = 356,
    TOKEN_ROWS = 357,
    TOKEN_SECOND = 358,
    TOKEN_SELECT = 359,
    TOKEN_SET = 360,
    TOKEN_SMA = 361,
    TOKEN_STDERR = 362,
    TOKEN_STDOUT = 363,
    TOKEN_SUBSTRING = 364,
    TOKEN_TABLE = 365,
    TOKEN_THEN = 366,
    TOKEN_TO = 367,
    TOKEN_TRUE = 368,
    TOKEN_TUPLESAMPLE = 369,
    TOKEN_UNBOUNDED = 370,
    TOKEN_UNIQUE = 371,
    TOKEN_UPDATE = 372,
    TOKEN_USING = 373,
    TOKEN_VALUES = 374,
    TOKEN_WHEN = 375,
    TOKEN_WHERE = 376,
    TOKEN_WINDOW = 377,
    TOKEN_WITH = 378,
    TOKEN_YEAR = 379,
    TOKEN_EOF = 380,
    TOKEN_LEX_ERROR = 381
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 116 "../SqlParser.ypp" /* yacc.c:355  */

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
  quickstep::ParseKeyBoolValue *key_bool_value_;

  quickstep::ParseAssignment *assignment_;
  quickstep::PtrList<quickstep::ParseAssignment> *assignment_list_;

  quickstep::ParseCommand *command_;
  quickstep::PtrVector<quickstep::ParseString> *command_argument_list_;

  quickstep::ParseStatement *statement_;
  quickstep::ParseStatementSetOperation *set_operation_statement_;
  quickstep::ParseStatementUpdate *update_statement_;
  quickstep::ParseStatementInsert *insert_statement_;
  quickstep::ParseStatementDelete *delete_statement_;
  quickstep::ParseStatementCopy *copy_statement_;
  quickstep::ParseStatementCreateTable *create_table_statement_;
  quickstep::ParsePartitionClause *partition_clause_;
  quickstep::ParseBlockProperties *block_properties_;
  quickstep::ParseStatementDropTable *drop_table_statement_;
  quickstep::ParseStatementQuit *quit_statement_;

  const quickstep::Comparison *comparison_;
  quickstep::ParseString *unary_operation_;
  quickstep::ParseString *binary_operation_;

  quickstep::ParseArray *array_expression_;
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

#line 421 "SqlParser_gen.cpp" /* yacc.c:355  */
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
#line 218 "../SqlParser.ypp" /* yacc.c:358  */

/* This header needs YYSTYPE, which is defined by the %union directive above */
#include "SqlLexer_gen.hpp"
void NotSupported(const YYLTYPE *location, yyscan_t yyscanner, const std::string &feature);

#line 456 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYFINAL  50
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   792

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  138
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  115
/* YYNRULES -- Number of rules.  */
#define YYNRULES  296
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  554

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   381

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     133,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   137,     2,     2,
     134,   135,    23,    21,   136,    22,    27,    24,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   132,
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
     130,   131
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   632,   632,   636,   640,   644,   648,   651,   658,   661,
     664,   667,   670,   673,   676,   679,   682,   685,   691,   697,
     704,   710,   717,   726,   731,   740,   745,   750,   754,   760,
     763,   768,   774,   780,   785,   792,   795,   798,   803,   806,
     809,   814,   819,   825,   831,   840,   844,   850,   853,   858,
     863,   868,   875,   882,   886,   892,   895,   900,   903,   908,
     911,   916,   919,   938,   941,   946,   950,   956,   959,   962,
     965,   970,   973,   976,   983,   988,   999,  1004,  1009,  1013,
    1017,  1023,  1026,  1032,  1040,  1043,  1046,  1052,  1057,  1062,
    1066,  1072,  1076,  1079,  1084,  1087,  1092,  1097,  1102,  1106,
    1112,  1121,  1124,  1129,  1132,  1151,  1156,  1160,  1166,  1172,
    1181,  1186,  1194,  1200,  1206,  1209,  1212,  1217,  1220,  1225,
    1229,  1235,  1238,  1241,  1246,  1251,  1256,  1259,  1262,  1267,
    1270,  1273,  1276,  1279,  1282,  1285,  1288,  1293,  1296,  1301,
    1305,  1309,  1312,  1316,  1319,  1324,  1327,  1332,  1335,  1340,
    1344,  1350,  1353,  1358,  1361,  1366,  1369,  1374,  1377,  1396,
    1399,  1404,  1408,  1414,  1420,  1425,  1428,  1433,  1436,  1441,
    1444,  1449,  1452,  1457,  1458,  1461,  1466,  1467,  1470,  1475,
    1479,  1485,  1492,  1495,  1498,  1503,  1506,  1509,  1515,  1518,
    1523,  1528,  1537,  1542,  1551,  1556,  1559,  1564,  1567,  1572,
    1578,  1584,  1587,  1590,  1593,  1596,  1599,  1605,  1614,  1620,
    1625,  1631,  1636,  1641,  1646,  1649,  1652,  1655,  1658,  1662,
    1666,  1669,  1672,  1675,  1678,  1681,  1686,  1689,  1695,  1699,
    1706,  1710,  1714,  1717,  1722,  1725,  1730,  1741,  1749,  1760,
    1763,  1768,  1772,  1778,  1783,  1787,  1793,  1798,  1801,  1806,
    1810,  1816,  1819,  1822,  1825,  1837,  1841,  1860,  1875,  1878,
    1881,  1884,  1887,  1890,  1895,  1899,  1905,  1908,  1913,  1917,
    1924,  1927,  1930,  1933,  1936,  1939,  1942,  1945,  1948,  1951,
    1956,  1967,  1970,  1975,  1978,  1981,  1987,  1991,  1997,  2000,
    2008,  2011,  2014,  2017,  2023,  2028,  2033
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
  "TOKEN_ALTER", "TOKEN_AS", "TOKEN_ASC", "TOKEN_BIT", "TOKEN_BITWEAVING",
  "TOKEN_BLOCKPROPERTIES", "TOKEN_BLOCKSAMPLE", "TOKEN_BLOOM_FILTER",
  "TOKEN_CSB_TREE", "TOKEN_BY", "TOKEN_CASE", "TOKEN_CAST", "TOKEN_CHECK",
  "TOKEN_COLUMN", "TOKEN_CONSTRAINT", "TOKEN_COPY", "TOKEN_CREATE",
  "TOKEN_CURRENT", "TOKEN_DAY", "TOKEN_DEFAULT", "TOKEN_DELETE",
  "TOKEN_DESC", "TOKEN_DISTINCT", "TOKEN_DOUBLECOLON", "TOKEN_DROP",
  "TOKEN_ELSE", "TOKEN_END", "TOKEN_EXISTS", "TOKEN_EXTRACT",
  "TOKEN_FALSE", "TOKEN_FIRST", "TOKEN_FOLLOWING", "TOKEN_FOR",
  "TOKEN_FOREIGN", "TOKEN_FROM", "TOKEN_FULL", "TOKEN_GROUP", "TOKEN_HASH",
  "TOKEN_HAVING", "TOKEN_HOUR", "TOKEN_IN", "TOKEN_INDEX", "TOKEN_INNER",
  "TOKEN_INSERT", "TOKEN_INTERVAL", "TOKEN_INTO", "TOKEN_JOIN",
  "TOKEN_KEY", "TOKEN_LAST", "TOKEN_LBRACE", "TOKEN_LEFT", "TOKEN_LIMIT",
  "TOKEN_MINUTE", "TOKEN_MONTH", "TOKEN_NULL", "TOKEN_NULLS", "TOKEN_OFF",
  "TOKEN_ON", "TOKEN_ORDER", "TOKEN_OUTER", "TOKEN_OVER",
  "TOKEN_PARTITION", "TOKEN_PARTITIONS", "TOKEN_PERCENT",
  "TOKEN_PRECEDING", "TOKEN_PRIMARY", "TOKEN_PRIORITY", "TOKEN_QUIT",
  "TOKEN_RANGE", "TOKEN_RBRACE", "TOKEN_REAL", "TOKEN_REFERENCES",
  "TOKEN_RIGHT", "TOKEN_ROW", "TOKEN_ROW_DELIMITER", "TOKEN_ROWS",
  "TOKEN_SECOND", "TOKEN_SELECT", "TOKEN_SET", "TOKEN_SMA", "TOKEN_STDERR",
  "TOKEN_STDOUT", "TOKEN_SUBSTRING", "TOKEN_TABLE", "TOKEN_THEN",
  "TOKEN_TO", "TOKEN_TRUE", "TOKEN_TUPLESAMPLE", "TOKEN_UNBOUNDED",
  "TOKEN_UNIQUE", "TOKEN_UPDATE", "TOKEN_USING", "TOKEN_VALUES",
  "TOKEN_WHEN", "TOKEN_WHERE", "TOKEN_WINDOW", "TOKEN_WITH", "TOKEN_YEAR",
  "TOKEN_EOF", "TOKEN_LEX_ERROR", "';'", "'\\n'", "'('", "')'", "','",
  "'%'", "$accept", "start", "sql_statement", "quit_statement",
  "alter_table_statement", "create_table_statement",
  "create_index_statement", "drop_table_statement", "column_def",
  "column_def_commalist", "data_type", "data_type_parameter_commalist",
  "opt_nullable", "column_constraint_def", "column_constraint_def_list",
  "opt_column_constraint_def_list", "table_constraint_def",
  "table_constraint_def_commalist", "opt_table_constraint_def_commalist",
  "opt_column_list", "opt_block_properties", "opt_partition_clause",
  "partition_type", "key_value_list", "key_value", "key_string_value",
  "key_string_list", "key_integer_value", "key_bool_value", "index_type",
  "opt_index_properties", "insert_statement", "copy_statement",
  "copy_to_target", "opt_copy_params", "update_statement",
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
  "unary_expression", "expression_base", "array_expression",
  "array_element_commalist", "function_call", "cast_function",
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
     380,   381,    59,    10,    40,    41,    44,    37
};
# endif

#define YYPACT_NINF -257

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-257)))

#define YYTABLE_NINF -130

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     653,  -257,  -257,    37,    86,     4,    58,    84,   157,  -257,
      55,   286,   286,  -257,   245,   185,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,   169,   -17,   226,  -257,
     144,   276,   286,  -257,  -257,    10,   -22,   286,   286,   286,
     286,   286,  -257,  -257,   479,   189,   125,  -257,   271,   178,
    -257,  -257,  -257,   217,   252,   -17,    55,   254,  -257,   217,
    -257,  -257,  -257,   174,    79,   207,   354,   207,   281,   251,
     277,  -257,   -35,  -257,  -257,   393,   403,  -257,   491,   280,
     294,   416,   503,  -257,   316,   569,   379,   319,  -257,  -257,
     386,     7,  -257,   396,  -257,   -39,   360,  -257,  -257,  -257,
    -257,  -257,  -257,   626,    48,   286,   286,   320,   286,    10,
     286,  -257,   217,   449,  -257,    26,   278,  -257,  -257,  -257,
     323,  -257,   207,  -257,   286,   286,   472,  -257,  -257,   324,
     286,  -257,  -257,  -257,   472,    72,   101,  -257,   637,   156,
     156,  -257,   310,   637,    35,    21,    14,   277,   637,  -257,
    -257,   286,   637,  -257,  -257,  -257,  -257,   637,   286,  -257,
     637,    17,   403,   396,   286,   339,   162,  -257,   450,  -257,
     217,  -257,   198,  -257,   207,   217,   226,  -257,   286,   152,
     286,   286,   286,  -257,   326,  -257,   206,   286,   576,   320,
     315,   455,   456,  -257,  -257,   720,   444,   232,   212,    16,
     637,   147,  -257,   637,  -257,   409,   274,  -257,  -257,  -257,
    -257,  -257,  -257,   402,  -257,    39,  -257,  -257,     9,    74,
     335,  -257,   334,    74,    53,   404,  -257,  -257,     7,  -257,
    -257,   215,   337,   310,   382,  -257,  -257,   345,   637,  -257,
     310,   228,   286,  -257,   637,  -257,   286,  -257,  -257,  -257,
     347,   408,   410,   356,  -257,  -257,  -257,   237,  -257,  -257,
    -257,  -257,  -257,   133,   286,   368,   152,   286,   149,  -257,
    -257,     6,    36,   472,   472,   283,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,   637,   369,   637,    30,  -257,   241,
     380,   637,    91,  -257,   447,   310,  -257,   286,   637,   499,
     151,   286,  -257,  -257,   425,  -257,   426,   427,   441,    14,
    -257,   513,   516,    74,   485,   457,  -257,   362,   362,   487,
     395,   445,  -257,   243,  -257,   637,  -257,   310,  -257,   472,
     406,   407,   286,  -257,   286,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,   286,  -257,  -257,  -257,   253,   509,   192,  -257,
     411,   414,  -257,   452,   415,   232,  -257,   464,   286,  -257,
    -257,   149,  -257,  -257,   456,  -257,  -257,  -257,   637,   418,
     180,   569,  -257,   310,   458,  -257,  -257,   232,   420,   310,
     637,  -257,   421,    44,   122,  -257,  -257,  -257,  -257,  -257,
      14,   335,   462,   466,  -257,   637,   472,   460,    77,  -257,
      77,  -257,   637,  -257,   518,   175,  -257,   310,    11,   286,
     286,   259,  -257,   262,  -257,   286,  -257,  -257,  -257,  -257,
     428,   152,   527,   473,  -257,   472,  -257,  -257,   434,  -257,
     318,   569,  -257,   637,   287,  -257,  -257,   232,   310,  -257,
    -257,   562,  -257,   481,  -257,  -257,   435,   455,   537,   502,
     501,  -257,  -257,  -257,   435,   637,  -257,  -257,  -257,   573,
    -257,   291,   295,  -257,  -257,  -257,   286,  -257,  -257,   454,
     553,  -257,    18,   286,   637,   297,   310,  -257,   299,   461,
     472,   637,   588,   474,  -257,   463,  -257,   190,    49,   497,
    -257,   301,   286,     0,  -257,   467,   310,  -257,  -257,  -257,
     455,   463,  -257,   286,  -257,   474,  -257,   637,  -257,  -257,
     520,   507,   504,   512,   601,   286,  -257,   306,  -257,  -257,
     480,  -257,   582,  -257,  -257,    23,  -257,  -257,  -257,  -257,
      61,   486,  -257,   286,   488,  -257,  -257,   558,   519,   560,
    -257,   286,   308,   382,  -257,  -257,  -257,   311,   532,   492,
    -257,   621,  -257,  -257
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     6,   296,     0,     0,     0,     0,     0,     0,    18,
     114,     0,     0,     7,     0,     0,    15,     8,    10,    11,
      13,    14,     9,    17,    12,    16,     0,   103,   110,   112,
       0,   294,     0,   288,   289,     0,     0,     0,     0,     0,
       0,     0,   115,   116,     0,     0,   105,   106,     0,   147,
       1,     3,     2,     0,     0,   103,   114,     0,   101,     0,
       5,     4,   295,     0,     0,    94,     0,    94,     0,     0,
     188,    25,     0,   255,   252,     0,   280,   117,     0,     0,
       0,     0,     0,   251,     0,     0,     0,   118,   119,   225,
     123,   209,   211,   213,   216,     0,   217,   220,   221,   222,
     223,   215,   214,     0,   266,     0,     0,     0,     0,     0,
       0,   102,     0,     0,   111,     0,     0,    91,    93,    92,
       0,    89,    94,    88,     0,     0,     0,    97,   189,     0,
       0,    85,   253,   254,     0,     0,   247,   244,     0,     0,
     256,   227,   229,     0,     0,     0,     0,   188,     0,   281,
     282,     0,     0,   122,   284,   285,   283,     0,     0,   226,
       0,     0,     0,   212,     0,     0,   188,    99,     0,   107,
       0,   108,     0,   286,    94,     0,   109,   104,     0,     0,
       0,     0,     0,    87,    57,    27,     0,     0,     0,     0,
       0,   190,   192,   194,   196,     0,   214,     0,     0,     0,
       0,   247,   241,     0,   245,     0,     0,   260,   261,   262,
     259,   263,   258,     0,   257,     0,   125,   224,     0,     0,
     149,   138,   124,   143,   126,   151,   120,   121,   208,   210,
     235,     0,    29,   228,   165,   218,   267,     0,     0,   230,
     249,     0,     0,    96,     0,   148,     0,    90,    86,    19,
       0,     0,     0,     0,    20,    21,    22,     0,    65,    67,
      68,    69,    70,     0,     0,     0,    55,     0,    47,   195,
     203,     0,     0,     0,     0,     0,   270,   272,   273,   274,
     275,   271,   276,   278,     0,     0,     0,     0,   264,     0,
       0,     0,     0,   242,     0,   248,   240,     0,     0,     0,
     129,     0,   139,   145,   135,   130,   131,   133,     0,     0,
     142,     0,     0,   141,     0,   153,    30,     0,     0,     0,
       0,   167,   231,     0,   232,     0,    98,   100,   287,     0,
       0,     0,     0,    95,     0,    72,    75,    73,   292,   293,
     291,   290,     0,    71,    76,   268,     0,   266,     0,    54,
      56,    59,    28,     0,     0,     0,    38,     0,     0,    40,
      46,    48,    26,   202,   191,   193,   277,   279,     0,     0,
       0,     0,   204,   201,     0,   200,    84,     0,     0,   246,
       0,   239,     0,     0,     0,   144,   146,   136,   132,   134,
       0,   150,     0,     0,   140,     0,     0,   155,    35,    34,
      35,    33,     0,   219,     0,   169,   233,   250,     0,     0,
       0,     0,    66,     0,    58,     0,    77,    78,    79,    80,
      81,     0,     0,    61,    39,     0,    42,    41,     0,    45,
       0,     0,   206,     0,     0,   199,   265,     0,   243,   234,
     236,     0,   237,     0,   127,   128,   152,   154,     0,   157,
       0,    36,    32,    31,   166,     0,   172,   171,   164,     0,
      52,     0,     0,    49,    74,   269,     0,    24,    53,     0,
       0,    23,     0,     0,     0,     0,   197,   205,     0,     0,
       0,     0,     0,   159,    37,   168,   179,   182,     0,     0,
      50,     0,     0,     0,    43,     0,   198,   207,    83,   238,
     137,   156,   158,     0,   113,   160,   161,     0,   183,   184,
     185,     0,     0,     0,     0,     0,    82,     0,    63,    64,
       0,    44,     0,   162,   180,     0,   181,   173,   175,   174,
       0,     0,    60,     0,     0,   186,   187,     0,     0,     0,
     170,     0,     0,   165,   176,   178,   177,     0,     0,     0,
      51,     0,   163,    62
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -105,  -257,
    -182,  -257,   234,   279,  -257,  -257,  -255,  -257,  -257,  -257,
    -257,  -257,  -257,  -256,   304,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,     5,   -40,  -257,  -257,  -257,   397,  -257,
     591,  -257,  -257,   543,   258,   539,   -50,   597,  -257,  -257,
     515,  -257,  -101,  -257,  -257,  -180,   270,  -191,   -10,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,   159,   118,  -257,  -257,
    -257,  -257,  -257,  -257,   184,   160,  -257,  -257,   100,  -257,
    -131,   398,   392,   482,   -15,   521,   517,   572,  -257,  -257,
    -142,  -257,  -257,  -257,  -257,  -257,   471,  -257,   540,   476,
    -222,  -187,   541,   247,  -109,  -257,  -257,  -257,  -257,  -257,
    -129,    -4,  -257,  -257,  -257
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    14,    15,    16,    17,    18,    19,    20,   185,   186,
     230,   231,   452,   360,   361,   362,   254,   350,   351,   265,
     423,   471,   520,   257,   258,   259,   260,   261,   262,   420,
     467,    21,    22,    65,   121,    23,    24,   166,   167,    25,
      58,    26,    46,    47,   144,    28,    29,    44,    86,    87,
      88,   147,    89,   313,   308,   220,   221,   302,   303,   222,
     315,   397,   449,   483,   504,   505,   506,   320,   321,   405,
     458,   459,   514,   540,   485,   486,   510,   526,   127,   128,
     191,   192,   193,   194,   195,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   201,   202,   136,   137,   205,
     241,   101,   213,   289,   102,   346,   286,   103,   152,   157,
     172,   104,   344,    30,    31
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      36,   198,    48,   199,   223,   268,   171,    45,    49,   114,
     288,   349,    56,    33,   273,    34,   323,   196,    33,   273,
      34,    33,   131,    34,   273,   196,   273,   123,    63,    90,
     154,   155,   310,    68,    69,    70,    71,    72,   300,    56,
     374,    67,   149,   150,    66,   219,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   511,   149,   150,   271,
     149,   150,   159,   135,    56,   149,   150,   142,   537,   518,
     145,   178,   179,   249,    10,   164,   223,    37,    33,   196,
      34,   196,   183,    42,   117,   535,   153,   450,   270,   129,
      33,   311,    34,   149,   150,    64,    48,   160,   512,   130,
     519,   168,    49,   536,   173,   299,   175,   301,   285,    43,
     538,    57,   149,   150,   174,   382,   375,   219,    10,    38,
     184,   187,   394,   206,    39,   248,   173,    64,   215,   391,
     399,   401,   291,    90,   247,   398,   400,    33,   335,    34,
     336,   363,   224,   218,   156,   233,   460,   227,   218,   434,
     240,   234,    32,   494,   232,   345,   217,   235,   203,   353,
     236,   337,   352,   451,   196,   196,   468,   223,   426,   513,
     216,   217,   312,   446,   187,   272,   255,   256,   263,   440,
     454,   539,   165,   232,   372,   292,   441,   165,   295,   433,
     436,   118,   119,   354,   338,    10,   250,   200,   408,    40,
     355,   149,   150,   411,   203,   115,   207,   380,   219,   475,
     491,   149,   150,   413,   224,    49,    53,   251,   304,    49,
     196,   339,   340,   240,   508,   305,   134,   208,   416,   327,
     116,   417,   418,   306,    41,   356,   517,    73,   168,    74,
     209,   210,   328,   509,    54,    50,   357,   225,   223,   252,
     288,   341,   358,    75,   162,   307,    59,   442,    27,   343,
     347,   106,    35,   187,   211,   447,   243,   342,   432,   370,
     359,   373,   200,   253,    60,   456,   379,    61,    10,    62,
     461,   462,   457,   383,    55,   212,   385,   196,   126,   219,
      33,   386,    34,   232,   472,   149,   150,    49,   242,   105,
     366,   367,   368,   419,   107,   224,   465,   297,    81,    49,
     407,   109,   108,   232,   232,    51,   196,    52,    83,    33,
      73,    34,    74,   180,   181,   188,    10,   474,   173,   110,
     263,   149,   150,   245,   246,   120,    75,    76,   173,   149,
     150,   266,   267,    33,    73,    34,    74,   290,   246,   500,
     316,   317,   113,   430,   428,   369,   240,    78,    79,   122,
      75,    76,   237,   324,   325,   438,    33,    73,    34,    74,
     124,   196,   333,   334,   189,    80,   376,   377,   406,   325,
     240,    78,    79,    75,   162,   125,   224,   240,   414,   415,
      33,    81,    34,   238,   463,   246,    82,   464,   246,    80,
     132,    83,   304,   126,   542,   173,   173,   149,   150,   305,
     133,   347,   547,  -129,   138,    81,   240,   306,   476,   151,
      82,   140,   477,   325,    10,    83,   489,   246,   139,    84,
     490,   246,   497,   325,   498,   377,   516,   334,    81,   307,
     487,   532,   334,   548,   246,   146,   550,   246,    83,   190,
     143,   158,   161,    84,   170,   148,   177,   182,   197,   496,
     264,   244,   263,   273,   287,   274,   487,   296,   298,   495,
     309,   318,   314,    85,   239,   319,    33,    73,    34,    74,
     322,   329,   188,    33,    73,    34,    74,   330,   263,   331,
     332,   348,   487,    75,    76,    33,    73,    34,    74,   522,
      75,    76,    77,   371,   378,   381,   384,    33,    73,    34,
      74,   531,    75,    76,    78,    79,   387,   388,   389,   390,
     392,    78,    79,   393,    75,    76,   395,   396,   402,   173,
     403,   189,    80,    78,    79,   404,   164,   173,   424,    80,
     409,   410,   422,   427,   435,    78,    79,   421,    81,   425,
     448,    80,   431,    82,   437,    81,   439,   444,    83,   455,
      82,   445,   466,    80,   469,    83,   470,    81,   473,   479,
     480,   325,    82,    33,    73,    34,    74,    83,   481,    81,
      33,    73,    34,    74,    82,   482,    84,   484,   492,    83,
      75,    76,   488,    84,   493,   502,   499,    75,    76,   507,
     515,   503,   521,   527,   141,    84,   190,   525,   529,   528,
     530,    78,    79,    85,   533,   534,   134,    84,    78,    79,
     541,   544,   543,   546,   545,    85,   551,   552,   553,    80,
      33,    73,    34,    74,   453,   189,    80,    85,   412,   326,
     429,    33,    73,    34,    74,    81,   111,    75,   162,   169,
      82,   176,    81,   112,     1,    83,     2,    82,    75,    76,
     443,   549,    83,   226,   523,   501,   365,   524,    78,    79,
     269,   364,   293,   228,   229,   163,   204,   294,    10,    78,
      79,   214,     0,    84,   478,     3,    80,     0,     0,     0,
      84,     0,     0,     0,     0,     0,     0,    80,     0,     0,
       4,     5,    81,    85,     0,     6,     0,    82,     0,     7,
     190,     0,    83,    81,     0,     0,     0,     0,    82,     0,
       0,     0,     0,    83,     0,     0,     0,     0,     8,     0,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
      84,   149,   150,     0,     0,     0,     0,     0,     0,     0,
       0,    84,     9,     0,     0,     0,     0,     0,     0,     0,
      85,     0,    10,     0,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,    11,     0,     0,     0,     0,
       0,    12,     0,    13,     0,     0,     0,     0,     0,     0,
       0,     0,   285
};

static const yytype_int16 yycheck[] =
{
       4,   130,    12,   134,   146,   187,   107,    11,    12,    59,
     197,   266,    29,     4,     8,     6,   238,   126,     4,     8,
       6,     4,    72,     6,     8,   134,     8,    67,    32,    44,
      23,    24,   223,    37,    38,    39,    40,    41,   218,    29,
      10,    36,    21,    22,    66,   146,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,     7,    21,    22,   190,
      21,    22,   101,    78,    29,    21,    22,    82,     7,    69,
      85,    45,    46,   178,   109,    27,   218,    73,     4,   188,
       6,   190,   122,    28,     5,    62,    90,    10,   189,   124,
       4,    38,     6,    21,    22,   117,   106,   136,    49,   134,
     100,   105,   106,    80,   108,    66,   110,    33,    72,    54,
      49,   128,    21,    22,   109,   297,    86,   218,   109,   115,
     124,   125,   313,   138,    66,   175,   130,   117,   143,   309,
     317,   318,   116,   148,   174,   317,   318,     4,     5,     6,
       7,   135,   146,   134,   137,   160,   135,   151,   134,   371,
     165,   134,   115,   135,   158,   264,   135,   161,    57,    10,
     164,    28,   267,    86,   273,   274,   421,   309,   355,   120,
     135,   135,   119,   395,   178,   190,   180,   181,   182,   135,
     402,   120,   134,   187,   285,   200,    64,   134,   203,     9,
     377,   112,   113,    44,    61,   109,    44,   125,   329,   115,
      51,    21,    22,   332,    57,    31,    50,   116,   309,   431,
     466,    21,    22,   342,   218,   219,    47,    65,    67,   223,
     329,    88,    89,   238,    34,    74,   125,    71,    36,   244,
      56,    39,    40,    82,    77,    86,   492,     5,   242,     7,
      84,    85,   246,    53,    75,     0,    97,   147,   390,    97,
     437,   118,   103,    21,    22,   104,    30,   135,     0,   263,
     264,   136,     4,   267,   108,   396,   166,   134,   369,   284,
     121,   286,   125,   121,   130,   100,   291,   133,   109,     3,
     409,   410,   107,   298,    26,   129,   135,   396,   126,   390,
       4,   301,     6,   297,   425,    21,    22,   301,   136,   110,
      17,    18,    19,   111,    33,   309,   415,    33,    76,   313,
     325,    53,   134,   317,   318,   130,   425,   132,    86,     4,
       5,     6,     7,    45,    46,    10,   109,     9,   332,    77,
     334,    21,    22,   135,   136,   128,    21,    22,   342,    21,
      22,   135,   136,     4,     5,     6,     7,   135,   136,   480,
     135,   136,    98,   368,   358,    72,   371,    42,    43,     5,
      21,    22,    23,   135,   136,   380,     4,     5,     6,     7,
      89,   480,   135,   136,    59,    60,   135,   136,   135,   136,
     395,    42,    43,    21,    22,   134,   390,   402,   135,   136,
       4,    76,     6,    54,   135,   136,    81,   135,   136,    60,
       7,    86,    67,   126,   533,   409,   410,    21,    22,    74,
       7,   415,   541,    78,   134,    76,   431,    82,   433,    33,
      81,     5,   135,   136,   109,    86,   135,   136,   134,   114,
     135,   136,   135,   136,   135,   136,   135,   136,    76,   104,
     455,   135,   136,   135,   136,    66,   135,   136,    86,   134,
     134,    55,    92,   114,   134,   136,     7,   134,   134,   474,
     134,    11,   466,     8,    20,     9,   481,    58,    66,   473,
     136,   134,    68,   134,   135,    93,     4,     5,     6,     7,
     135,   134,    10,     4,     5,     6,     7,    79,   492,    79,
     134,   123,   507,    21,    22,     4,     5,     6,     7,   503,
      21,    22,    23,   134,   124,    58,     7,     4,     5,     6,
       7,   515,    21,    22,    42,    43,    91,    91,    91,    78,
       7,    42,    43,     7,    21,    22,    41,    70,    41,   533,
     135,    59,    60,    42,    43,    90,    27,   541,    86,    60,
     134,   134,   128,    79,    86,    42,    43,   136,    76,   134,
      90,    60,   134,    81,   134,    76,   135,    95,    86,    41,
      81,    95,   134,    60,    37,    86,    93,    76,   134,     7,
      89,   136,    81,     4,     5,     6,     7,    86,    41,    76,
       4,     5,     6,     7,    81,    83,   114,    86,   134,    86,
      21,    22,    19,   114,    41,     7,   135,    21,    22,   136,
     103,   127,   135,    96,   101,   114,   134,    87,    96,   105,
       9,    42,    43,   134,   134,    33,   125,   114,    42,    43,
     134,    63,   134,    63,   105,   134,    94,   135,     7,    60,
       4,     5,     6,     7,   400,    59,    60,   134,   334,   242,
     361,     4,     5,     6,     7,    76,    55,    21,    22,   106,
      81,   112,    76,    56,     1,    86,     3,    81,    21,    22,
     390,   543,    86,   148,   505,   481,   274,   507,    42,    43,
     188,   273,   201,   152,   157,   103,   136,   201,   109,    42,
      43,   140,    -1,   114,   437,    32,    60,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,
      47,    48,    76,   134,    -1,    52,    -1,    81,    -1,    56,
     134,    -1,    86,    76,    -1,    -1,    -1,    -1,    81,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    75,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
     114,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,   128,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,    32,    47,    48,    52,    56,    75,    99,
     109,   122,   128,   130,   139,   140,   141,   142,   143,   144,
     145,   169,   170,   173,   174,   177,   179,   182,   183,   184,
     251,   252,   115,     4,     6,   182,   249,    73,   115,    66,
     115,    77,    28,    54,   185,   249,   180,   181,   196,   249,
       0,   130,   132,    47,    75,   182,    29,   128,   178,    30,
     130,   133,     3,   249,   117,   171,    66,   171,   249,   249,
     249,   249,   249,     5,     7,    21,    22,    23,    42,    43,
      60,    76,    81,    86,   114,   134,   186,   187,   188,   190,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   239,   242,   245,   249,   110,   136,    33,   134,   182,
      77,   178,   185,    98,   184,    31,    56,     5,   112,   113,
     128,   172,     5,   172,    89,   134,   126,   216,   217,   124,
     134,   184,     7,     7,   125,   222,   235,   236,   134,   134,
       5,   101,   222,   134,   182,   222,    66,   189,   136,    21,
      22,    33,   246,   249,    23,    24,   137,   247,    55,   101,
     136,    92,    22,   225,    27,   134,   175,   176,   249,   181,
     134,   190,   248,   249,   171,   249,   183,     7,    45,    46,
      45,    46,   134,   172,   249,   146,   147,   249,    10,    59,
     134,   218,   219,   220,   221,   222,   242,   134,   248,   218,
     125,   233,   234,    57,   236,   237,   222,    50,    71,    84,
      85,   108,   129,   240,   240,   222,   135,   135,   134,   190,
     193,   194,   197,   228,   249,   216,   188,   249,   223,   224,
     148,   149,   249,   222,   134,   249,   249,    23,    54,   135,
     222,   238,   136,   216,    11,   135,   136,   172,   184,   146,
      44,    65,    97,   121,   154,   249,   249,   161,   162,   163,
     164,   165,   166,   249,   134,   157,   135,   136,   148,   221,
     190,   218,   222,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    72,   244,    20,   239,   241,
     135,   116,   222,   234,   237,   222,    58,    33,    66,    66,
     193,    33,   195,   196,    67,    74,    82,   104,   192,   136,
     195,    38,   119,   191,    68,   198,   135,   136,   134,    93,
     205,   206,   135,   238,   135,   136,   176,   222,   249,   134,
      79,    79,   134,   135,   136,     5,     7,    28,    61,    88,
      89,   118,   134,   249,   250,   242,   243,   249,   123,   154,
     155,   156,   146,    10,    44,    51,    86,    97,   103,   121,
     151,   152,   153,   135,   219,   220,    17,    18,    19,    72,
     222,   134,   190,   222,    10,    86,   135,   136,   124,   222,
     116,    58,   148,   222,     7,   135,   196,    91,    91,    91,
      78,   193,     7,     7,   195,    41,    70,   199,   148,   239,
     148,   239,    41,   135,    90,   207,   135,   222,   218,   134,
     134,   248,   162,   248,   135,   136,    36,    39,    40,   111,
     167,   136,   128,   158,    86,   134,   239,    79,   249,   151,
     222,   134,   190,     9,   238,    86,   239,   134,   222,   135,
     135,    64,   135,   194,    95,    95,   238,   218,    90,   200,
      10,    86,   150,   150,   238,    41,   100,   107,   208,   209,
     135,   248,   248,   135,   135,   242,   134,   168,   154,    37,
      93,   159,   218,   134,     9,   238,   222,   135,   241,     7,
      89,    41,    83,   201,    86,   212,   213,   222,    19,   135,
     135,   161,   134,    41,   135,   249,   222,   135,   135,   135,
     218,   212,     7,   127,   202,   203,   204,   136,    34,    53,
     214,     7,    49,   120,   210,   103,   135,   161,    69,   100,
     160,   135,   249,   204,   213,    87,   215,    96,   105,    96,
       9,   249,   135,   134,    33,    62,    80,     7,    49,   120,
     211,   134,   248,   134,    63,   105,    63,   248,   135,   205,
     135,    94,   135,     7
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   138,   139,   139,   139,   139,   139,   139,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   141,   142,
     142,   142,   142,   143,   144,   145,   146,   147,   147,   148,
     148,   149,   149,   149,   149,   150,   150,   150,   151,   151,
     151,   151,   151,   151,   151,   152,   152,   153,   153,   154,
     154,   154,   154,   155,   155,   156,   156,   157,   157,   158,
     158,   159,   159,   160,   160,   161,   161,   162,   162,   162,
     162,   163,   163,   163,   164,   165,   166,   167,   167,   167,
     167,   168,   168,   169,   169,   169,   169,   170,   170,   170,
     170,   171,   171,   171,   172,   172,   173,   174,   175,   175,
     176,   177,   177,   178,   178,   179,   180,   180,   181,   182,
     182,   183,   183,   184,   185,   185,   185,   186,   186,   187,
     187,   188,   188,   188,   189,   190,   191,   191,   191,   192,
     192,   192,   192,   192,   192,   192,   192,   193,   193,   194,
     194,   194,   194,   194,   194,   195,   195,   196,   196,   197,
     197,   198,   198,   199,   199,   200,   200,   201,   201,   202,
     202,   203,   203,   204,   205,   206,   206,   207,   207,   208,
     208,   209,   209,   210,   210,   210,   211,   211,   211,   212,
     212,   213,   214,   214,   214,   215,   215,   215,   216,   216,
     217,   218,   218,   219,   219,   220,   220,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   222,   222,
     223,   223,   224,   224,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   226,   226,   227,   227,
     228,   228,   228,   228,   229,   229,   230,   231,   231,   232,
     232,   233,   233,   234,   235,   235,   236,   237,   237,   238,
     238,   239,   239,   239,   239,   239,   239,   239,   240,   240,
     240,   240,   240,   240,   241,   241,   242,   242,   243,   243,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     245,   246,   246,   247,   247,   247,   248,   248,   249,   249,
     250,   250,   250,   250,   251,   252,   252
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     6,
       6,     6,     6,     9,     9,     3,     3,     1,     3,     1,
       2,     4,     4,     3,     3,     0,     1,     2,     1,     2,
       1,     2,     2,     4,     5,     2,     1,     0,     1,     4,
       5,    10,     4,     3,     1,     0,     1,     0,     3,     0,
       5,     0,     8,     1,     1,     1,     3,     1,     1,     1,
       1,     2,     2,     2,     4,     2,     2,     1,     1,     1,
       1,     0,     3,    10,     7,     4,     5,     5,     4,     4,
       5,     2,     2,     2,     0,     4,     5,     4,     3,     1,
       3,     2,     3,     0,     3,     2,     1,     3,     3,     4,
       1,     3,     1,    10,     0,     1,     1,     1,     1,     1,
       3,     3,     2,     1,     2,     3,     0,     3,     3,     0,
       1,     1,     2,     1,     2,     1,     2,     6,     1,     2,
       3,     2,     2,     1,     3,     1,     2,     1,     4,     1,
       3,     0,     3,     0,     2,     0,     3,     0,     2,     0,
       1,     1,     2,     6,     3,     0,     3,     0,     3,     0,
       5,     1,     1,     2,     2,     2,     2,     2,     2,     1,
       3,     3,     0,     1,     1,     0,     2,     2,     0,     1,
       2,     3,     1,     3,     1,     2,     1,     5,     6,     4,
       3,     3,     3,     2,     3,     5,     4,     6,     3,     1,
       3,     1,     2,     1,     1,     1,     1,     1,     3,     5,
       1,     1,     1,     1,     3,     1,     2,     2,     3,     2,
       3,     4,     4,     5,     6,     3,     6,     6,     8,     5,
       4,     1,     2,     4,     1,     2,     4,     0,     2,     1,
       3,     1,     1,     2,     2,     1,     2,     3,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     1,
       1,     1,     1,     1,     2,     2,     0
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
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1872 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_NAME  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1882 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1892 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1902 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 7: /* TOKEN_UNSIGNED_NUMVAL  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 1912 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 140: /* sql_statement  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1922 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 141: /* quit_statement  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 1932 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 142: /* alter_table_statement  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1942 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 143: /* create_table_statement  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 1952 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 144: /* create_index_statement  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1962 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 145: /* drop_table_statement  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 1972 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 146: /* column_def  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 1982 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 147: /* column_def_commalist  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 1992 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 148: /* data_type  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 2002 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 149: /* data_type_parameter_commalist  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 2012 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 150: /* opt_nullable  */
#line 619 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2018 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 151: /* column_constraint_def  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 2028 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 152: /* column_constraint_def_list  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2038 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 153: /* opt_column_constraint_def_list  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2048 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* opt_column_list  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2058 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* opt_block_properties  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).block_properties_) != nullptr) {
    delete ((*yyvaluep).block_properties_);
  }
}
#line 2068 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* opt_partition_clause  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).partition_clause_) != nullptr) {
    delete ((*yyvaluep).partition_clause_);
  }
}
#line 2078 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* partition_type  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2088 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* key_value_list  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2098 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* key_value  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 2108 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 163: /* key_string_value  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 2118 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 164: /* key_string_list  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 2128 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 165: /* key_integer_value  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 2138 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 166: /* key_bool_value  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_bool_value_) != nullptr) {
    delete ((*yyvaluep).key_bool_value_);
  }
}
#line 2148 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 167: /* index_type  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2158 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* opt_index_properties  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2168 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* insert_statement  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 2178 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* copy_statement  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_statement_) != nullptr) {
    delete ((*yyvaluep).copy_statement_);
  }
}
#line 2188 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* copy_to_target  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2198 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* opt_copy_params  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2208 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* update_statement  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2218 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* delete_statement  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2228 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* assignment_list  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2238 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* assignment_item  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2248 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* set_operation_statement  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_statement_) != nullptr) {
    delete ((*yyvaluep).set_operation_statement_);
  }
}
#line 2258 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* opt_priority_clause  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_priority_clause_) != nullptr) {
    delete ((*yyvaluep).opt_priority_clause_);
  }
}
#line 2268 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* with_clause  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2278 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* with_list  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2288 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* with_list_element  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2298 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* set_operation_union  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_) != nullptr) {
    delete ((*yyvaluep).set_operation_);
  }
}
#line 2308 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* set_operation_intersect  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).set_operation_) != nullptr) {
    delete ((*yyvaluep).set_operation_);
  }
}
#line 2318 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* select_query  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2328 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* opt_all_distinct  */
#line 619 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2334 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* selection  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2344 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* selection_item_commalist  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2354 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* selection_item  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2364 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* from_clause  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2374 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 190: /* subquery_expression  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2384 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 191: /* opt_sample_clause  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_sample_clause_) != nullptr) {
    delete ((*yyvaluep).opt_sample_clause_);
  }
}
#line 2394 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 192: /* join_type  */
#line 621 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2400 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 193: /* joined_table_reference  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2410 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 194: /* table_reference  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2420 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 195: /* table_reference_signature  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2430 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 196: /* table_reference_signature_primary  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2440 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 197: /* joined_table_reference_commalist  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2450 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 198: /* opt_group_by_clause  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2460 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 199: /* opt_having_clause  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2470 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 200: /* opt_order_by_clause  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2480 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 201: /* opt_limit_clause  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2490 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 202: /* opt_window_clause  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_window_clause_) != nullptr) {
    delete ((*yyvaluep).opt_window_clause_);
  }
}
#line 2500 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 203: /* window_declaration_list  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_window_clause_) != nullptr) {
    delete ((*yyvaluep).opt_window_clause_);
  }
}
#line 2510 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 204: /* window_declaration  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_definition_) != nullptr) {
    delete ((*yyvaluep).window_definition_);
  }
}
#line 2520 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 205: /* window_definition  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_definition_) != nullptr) {
    delete ((*yyvaluep).window_definition_);
  }
}
#line 2530 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 206: /* opt_window_partition  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_partition_by_list_) != nullptr) {
    delete ((*yyvaluep).window_partition_by_list_);
  }
}
#line 2540 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 207: /* opt_window_order  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_order_by_list_) != nullptr) {
    delete ((*yyvaluep).window_order_by_list_);
  }
}
#line 2550 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 208: /* opt_window_frame  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_frame_info_) != nullptr) {
    delete ((*yyvaluep).window_frame_info_);
  }
}
#line 2560 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 209: /* frame_mode  */
#line 619 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2566 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 210: /* frame_preceding  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2576 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 211: /* frame_following  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 2586 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 212: /* order_commalist  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2596 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 213: /* order_item  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2606 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 214: /* opt_order_direction  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2616 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 215: /* opt_nulls_first  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2626 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 216: /* opt_where_clause  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2636 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 217: /* where_clause  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2646 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 218: /* or_expression  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2656 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 219: /* and_expression  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2666 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 220: /* not_expression  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2676 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 221: /* predicate_expression_base  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2686 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 222: /* add_expression  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2696 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 223: /* multiply_expression  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2706 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 224: /* unary_expression  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2716 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 225: /* expression_base  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2726 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 226: /* array_expression  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).array_expression_) != nullptr) {
    delete ((*yyvaluep).array_expression_);
  }
}
#line 2736 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 227: /* array_element_commalist  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).array_expression_) != nullptr) {
    delete ((*yyvaluep).array_expression_);
  }
}
#line 2746 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 228: /* function_call  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2756 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 229: /* cast_function  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2766 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 230: /* extract_function  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2776 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 231: /* substr_function  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2786 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 232: /* case_expression  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2796 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 233: /* simple_when_clause_list  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_list_);
  }
}
#line 2806 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 234: /* simple_when_clause  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_);
  }
}
#line 2816 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 235: /* searched_when_clause_list  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_list_);
  }
}
#line 2826 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 236: /* searched_when_clause  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_);
  }
}
#line 2836 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 237: /* opt_else_clause  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2846 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 238: /* expression_list  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2856 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 239: /* literal_value  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2866 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 240: /* datetime_unit  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2876 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 241: /* literal_value_commalist  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2886 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 242: /* attribute_ref  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2896 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 243: /* attribute_ref_list  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2906 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 244: /* comparison_operation  */
#line 620 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2912 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 245: /* unary_operation  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).unary_operation_) != nullptr) {
    delete ((*yyvaluep).unary_operation_);
  }
}
#line 2922 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 246: /* add_operation  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).binary_operation_) != nullptr) {
    delete ((*yyvaluep).binary_operation_);
  }
}
#line 2932 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 247: /* multiply_operation  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).binary_operation_) != nullptr) {
    delete ((*yyvaluep).binary_operation_);
  }
}
#line 2942 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 248: /* name_commalist  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 2952 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 249: /* any_name  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2962 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 250: /* boolean_value  */
#line 619 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2968 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 251: /* command  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_) != nullptr) {
    delete ((*yyvaluep).command_);
  }
}
#line 2978 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 252: /* command_argument_list  */
#line 623 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_argument_list_) != nullptr) {
    delete ((*yyvaluep).command_argument_list_);
  }
}
#line 2988 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 632 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3285 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 636 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3294 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 640 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3303 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 5:
#line 644 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3312 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 648 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    YYABORT;
  }
#line 3320 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 651 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 3329 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 658 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3337 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 9:
#line 661 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].copy_statement_);
  }
#line 3345 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 10:
#line 664 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 3353 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 11:
#line 667 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3361 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 12:
#line 670 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 3369 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 13:
#line 673 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 3377 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 14:
#line 676 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 3385 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 15:
#line 679 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 3393 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 16:
#line 682 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].set_operation_statement_);
  }
#line 3401 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 17:
#line 685 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 3409 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 18:
#line 691 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3417 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 19:
#line 697 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3429 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 20:
#line 704 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3440 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 710 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3452 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 717 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3464 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 726 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].attribute_definition_list_), (yyvsp[-1].block_properties_), (yyvsp[0].partition_clause_));
  }
#line 3472 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 731 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].key_value_list_)) {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_), (yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].key_value_list_));
    } else {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_));
    }
  }
#line 3484 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 740 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 3492 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 745 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 3500 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 750 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3509 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 754 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3518 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 760 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 3526 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 763 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = (yyvsp[-1].data_type_);
  }
#line 3534 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 768 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_));
    (yyvsp[-1].data_type_)->setNullable((yyvsp[0].boolean_value_));
    (yyval.data_type_)->addParameter(
        new quickstep::ParseDataTypeParameterDataType((yyvsp[-1].data_type_)->line_number(), (yyvsp[-1].data_type_)->column_number(), (yyvsp[-1].data_type_)));
  }
#line 3545 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 774 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = (yyvsp[-3].data_type_);
    (yyvsp[-1].data_type_)->setNullable((yyvsp[0].boolean_value_));
    (yyval.data_type_)->addParameter(
        new quickstep::ParseDataTypeParameterDataType((yyvsp[-1].data_type_)->line_number(), (yyvsp[-1].data_type_)->column_number(), (yyvsp[-1].data_type_)));
  }
#line 3556 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 780 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_));
    (yyval.data_type_)->addParameter(
        new quickstep::ParseDataTypeParameterLiteralValue((yyvsp[0].literal_value_)->line_number(), (yyvsp[0].literal_value_)->column_number(), (yyvsp[0].literal_value_)));
  }
#line 3566 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 785 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = (yyvsp[-2].data_type_);
    (yyval.data_type_)->addParameter(
        new quickstep::ParseDataTypeParameterLiteralValue((yyvsp[0].literal_value_)->line_number(), (yyvsp[0].literal_value_)->column_number(), (yyvsp[0].literal_value_)));
  }
#line 3576 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 792 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;  // NOT NULL
  }
#line 3584 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 795 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;  // NULL
  }
#line 3592 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 798 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;  // NOT NULL
  }
#line 3600 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 803 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3608 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 806 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3616 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 809 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3626 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 814 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3636 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 819 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3647 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 825 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3658 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 831 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3670 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 840 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3679 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 844 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3688 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 850 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3696 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 853 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3704 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 858 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3714 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 863 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3724 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 868 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3736 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 875 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3746 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 882 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3755 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 886 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3764 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 892 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3772 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 895 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = $1; */
  }
#line 3780 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 900 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = nullptr;
  }
#line 3788 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 903 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-1].attribute_list_);
  }
#line 3796 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 908 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = nullptr;
  }
#line 3804 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 911 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].key_value_list_));
  }
#line 3812 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 916 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.partition_clause_) = nullptr;
  }
#line 3820 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 919 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3842 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 938 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::kHashPartitionType);
  }
#line 3850 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 941 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::kRangePartitionType);
  }
#line 3858 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 946 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3867 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 950 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3876 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 956 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 3884 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 959 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 3892 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 962 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 3900 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 965 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_bool_value_);
  }
#line 3908 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 970 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 3916 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 973 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 3924 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 976 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_),
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 3934 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 983 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 3942 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 988 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 3956 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 999 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_bool_value_) = new quickstep::ParseKeyBoolValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].boolean_value_));
  }
#line 3964 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 1004 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Defaults to BitWeavingV, but IndexProperties can change this to H.
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBitWeavingV));
  }
#line 3974 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 1009 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
#line 3983 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 1013 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
#line 3992 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 1017 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kSMA));
  }
#line 4001 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 1023 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 4009 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 1026 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 4017 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 1032 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 4030 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 1040 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertTuple((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 4038 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 1043 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-3]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), nullptr);
  }
#line 4046 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 1046 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-4]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), (yyvsp[-4].with_list_));
  }
#line 4054 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 1052 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-4]).first_line, (yylsp[-4]).first_column,
                                           quickstep::ParseStatementCopy::kFrom,
                                           (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4064 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 1057 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-3]).first_line, (yylsp[-3]).first_column,
                                           quickstep::ParseStatementCopy::kTo,
                                           (yyvsp[-2].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4074 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 1062 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-3]).first_line, (yylsp[-3]).first_column,
                                           (yyvsp[-2].set_operation_), nullptr, (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4083 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 1066 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_statement_) = new quickstep::ParseStatementCopy((yylsp[-4]).first_line, (yylsp[-4]).first_column,
                                           (yyvsp[-2].set_operation_), (yyvsp[-4].with_list_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 4092 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 1072 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yyvsp[0].string_value_)->line_number(), (yyvsp[0].string_value_)->column_number(), "@" + (yyvsp[0].string_value_)->value());
    delete (yyvsp[0].string_value_);
  }
#line 4101 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 1076 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "$stdout");
  }
#line 4109 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 1079 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "$stderr");
  }
#line 4117 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 1084 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 4125 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 1087 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 4133 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 1092 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 4141 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 1097 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 4149 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 1102 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 4158 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 1106 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 4167 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 1112 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 4175 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 1121 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_statement_) = new quickstep::ParseStatementSetOperation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].set_operation_), nullptr, (yyvsp[0].opt_priority_clause_));
  }
#line 4183 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 1124 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_statement_) = new quickstep::ParseStatementSetOperation((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].set_operation_), (yyvsp[-2].with_list_), (yyvsp[0].opt_priority_clause_));
  }
#line 4191 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 1129 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_priority_clause_) = nullptr;
  }
#line 4199 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 1132 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4221 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 1151 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 4229 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 1156 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4238 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 1160 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4247 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 1166 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 4256 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 1172 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-1].boolean_value_)) {
      (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-3]).first_line, (yylsp[-3]).first_column, quickstep::ParseSetOperation::kUnion);
    } else {
      (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-3]).first_line, (yylsp[-3]).first_column, quickstep::ParseSetOperation::kUnionAll);
    }
    (yyval.set_operation_)->addOperand((yyvsp[-3].set_operation_));
    (yyval.set_operation_)->addOperand((yyvsp[0].set_operation_));
  }
#line 4270 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 1181 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_) = (yyvsp[0].set_operation_);
  }
#line 4278 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 1186 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[-2]).first_line, (yylsp[-2]).first_column, quickstep::ParseSetOperation::kIntersect);
    quickstep::ParseSetOperation *op = new quickstep::ParseSetOperation(
        (yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::ParseSetOperation::kSelect);
    op->addOperand((yyvsp[0].select_query_));
    (yyval.set_operation_)->addOperand((yyvsp[-2].set_operation_));
    (yyval.set_operation_)->addOperand(op);
  }
#line 4291 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 1194 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.set_operation_) = new quickstep::ParseSetOperation((yylsp[0]).first_line, (yylsp[0]).first_column, quickstep::ParseSetOperation::kSelect);
    (yyval.set_operation_)->addOperand((yyvsp[0].select_query_));
  }
#line 4300 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 1201 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-9]).first_line, (yylsp[-9]).first_column, (yyvsp[-7].selection_), (yyvsp[-6].table_reference_list_), (yyvsp[-5].predicate_), (yyvsp[-4].opt_group_by_clause_), (yyvsp[-3].opt_having_clause_), (yyvsp[-2].opt_order_by_clause_), (yyvsp[-1].opt_limit_clause_), (yyvsp[0].opt_window_clause_));
  }
#line 4308 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 1206 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;  // Distinct
  }
#line 4316 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 1209 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;  // All
  }
#line 4324 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 1212 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;  // Distinct
  }
#line 4332 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 1217 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4340 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 1220 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 4348 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 1225 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4357 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 1229 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4366 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 1235 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 4374 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 1238 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 4382 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 1241 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 4390 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 1246 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[0].table_reference_list_);
  }
#line 4398 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 1251 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].set_operation_));
  }
#line 4406 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 1256 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = NULL;
  }
#line 4414 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 1259 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, true, (yyvsp[-1].numeric_literal_value_));
  }
#line 4422 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 1262 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].numeric_literal_value_));
  }
#line 4430 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 1267 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4438 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 1270 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kInnerJoin;
  }
#line 4446 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 1273 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4454 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 1276 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kLeftOuterJoin;
  }
#line 4462 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 1279 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4470 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 1282 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kRightOuterJoin;
  }
#line 4478 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 1285 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4486 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 1288 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.join_type_) = quickstep::ParseJoinedTableReference::JoinType::kFullOuterJoin;
  }
#line 4494 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 1293 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseJoinedTableReference((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].join_type_), (yyvsp[-5].table_reference_), (yyvsp[-2].table_reference_), (yyvsp[0].predicate_));
  }
#line 4502 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 1296 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = (yyvsp[0].table_reference_);
  }
#line 4510 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 1301 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4519 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 1305 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].opt_sample_clause_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4528 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 1309 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].opt_sample_clause_));
  }
#line 4536 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 1312 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4545 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 1316 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].function_call_));
  }
#line 4553 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 1319 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = (yyvsp[-1].table_reference_);
  }
#line 4561 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 1324 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4569 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 1327 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4577 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 1332 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4585 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 1335 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4593 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 1340 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4602 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 1344 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4611 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 1350 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 4619 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 1353 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4627 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 1358 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4635 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 1361 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4643 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 1366 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4651 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 1369 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4659 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 1374 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4667 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 1377 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4689 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 1396 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = nullptr;
  }
#line 4697 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 1399 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = (yyvsp[0].opt_window_clause_);
  }
#line 4705 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 1404 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = new quickstep::PtrList<quickstep::ParseWindow>();
    (yyval.opt_window_clause_)->push_back((yyvsp[0].window_definition_));
  }
#line 4714 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 1408 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_window_clause_) = (yyvsp[-1].opt_window_clause_);
    (yyval.opt_window_clause_)->push_back((yyvsp[0].window_definition_));
  }
#line 4723 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 1414 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_definition_) = (yyvsp[-1].window_definition_);
    (yyval.window_definition_)->setName((yyvsp[-4].string_value_));
  }
#line 4732 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 1420 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_definition_) = new quickstep::ParseWindow((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].window_partition_by_list_), (yyvsp[-1].window_order_by_list_), (yyvsp[0].window_frame_info_));
  }
#line 4740 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 1425 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_partition_by_list_) = nullptr;
  }
#line 4748 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 1428 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_partition_by_list_) = (yyvsp[0].expression_list_);
  }
#line 4756 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 1433 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_order_by_list_) = nullptr;
  }
#line 4764 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 1436 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_order_by_list_) = (yyvsp[0].order_commalist_);
  }
#line 4772 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 1441 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_frame_info_) = nullptr;
  }
#line 4780 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 1444 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.window_frame_info_) = new quickstep::ParseFrameInfo((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-4].boolean_value_), (yyvsp[-2].numeric_literal_value_)->long_value(), (yyvsp[0].numeric_literal_value_)->long_value());
  }
#line 4788 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 1449 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4796 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 1452 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 4804 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 1458 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "-1");
  }
#line 4812 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 1461 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "0");
  }
#line 4820 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 1467 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "-1");
  }
#line 4828 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 1470 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.numeric_literal_value_) = new quickstep::NumericParseLiteralValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, "0");
  }
#line 4836 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 1475 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4845 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 1479 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4854 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 1485 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 4864 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 1492 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4872 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 1495 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4880 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 1498 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4888 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 1503 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4896 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1506 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4904 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 1509 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4912 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 1515 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 4920 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 1518 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4928 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 1523 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4936 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 1528 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4950 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 1537 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4958 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 1542 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4972 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 1551 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4980 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 1556 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4988 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 1559 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4996 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 1564 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5004 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 1567 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 5014 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 1572 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 5025 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1578 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 5036 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1584 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5044 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1587 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 5052 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1590 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateExists((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].subquery_expression_));
  }
#line 5060 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1593 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-2].expression_), (yyvsp[0].subquery_expression_));
  }
#line 5068 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1596 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-1].expression_list_));
  }
#line 5076 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1599 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-2]).first_line,
        (yylsp[-2]).first_column,
        new quickstep::ParsePredicateInTableQuery((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-3].expression_), (yyvsp[0].subquery_expression_)));
  }
#line 5087 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1605 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line,
        (yylsp[-4]).first_column,
        new quickstep::ParsePredicateInValueList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-1].expression_list_)));
  }
#line 5098 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1614 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-2].expression_));
    arguments->push_back((yyvsp[0].expression_));
    (yyval.expression_) = new quickstep::ParseFunctionCall((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].binary_operation_), arguments);
  }
#line 5109 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1620 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5117 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1625 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-2].expression_));
    arguments->push_back((yyvsp[0].expression_));
    (yyval.expression_) = new quickstep::ParseFunctionCall((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].binary_operation_), arguments);
  }
#line 5128 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1631 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5136 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1636 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[0].expression_));
    (yyval.expression_) = new quickstep::ParseFunctionCall((yylsp[-1]).first_line, (yylsp[-1]).first_column, false, (yyvsp[-1].unary_operation_), arguments);
  }
#line 5146 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1641 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5154 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1646 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 5162 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1649 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 5170 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1652 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].array_expression_);
  }
#line 5178 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1655 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 5186 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1658 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyvsp[-2].function_call_)->setWindowName((yyvsp[0].string_value_));
    (yyval.expression_) = (yyvsp[-2].function_call_);
  }
#line 5195 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1662 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyvsp[-4].function_call_)->setWindow((yyvsp[-1].window_definition_));
    (yyval.expression_) = (yyvsp[-4].function_call_);
  }
#line 5204 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1666 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5212 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1669 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5220 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1672 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5228 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1675 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5236 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1678 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 5244 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1681 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].subquery_expression_);
  }
#line 5252 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1686 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.array_expression_) = (yyvsp[-1].array_expression_);
  }
#line 5260 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1689 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.array_expression_) = new quickstep::ParseArray((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 5268 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1695 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.array_expression_) = (yyvsp[-2].array_expression_);
    (yyval.array_expression_)->add((yyvsp[0].expression_));
  }
#line 5277 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1699 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.array_expression_) = new quickstep::ParseArray((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.array_expression_)->add((yyvsp[0].expression_));
  }
#line 5286 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1706 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 5295 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1710 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 5304 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1714 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, false, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 5312 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1717 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-4]).first_line, (yylsp[-4]).first_column, true, (yyvsp[-4].string_value_), (yyvsp[-1].expression_list_));
  }
#line 5320 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1722 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseTypeCast((yylsp[-5]).first_line, (yylsp[-5]).first_column, (yyvsp[-3].expression_), (yyvsp[-1].data_type_));
  }
#line 5328 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1725 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseTypeCast((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-2].expression_), (yyvsp[0].data_type_));
  }
#line 5336 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1730 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-1].expression_));
    arguments->push_back(new quickstep::ParseScalarLiteral(
        new quickstep::StringParseLiteralValue((yyvsp[-3].string_value_), nullptr)));
    auto *name = new quickstep::ParseString((yylsp[-5]).first_line, (yylsp[-5]).first_column, "extract");
    (yyval.expression_) = new quickstep::ParseFunctionCall(
        (yylsp[-5]).first_line, (yylsp[-5]).first_column, false, name, arguments);
  }
#line 5350 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1741 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-3].expression_));
    arguments->push_back(new quickstep::ParseScalarLiteral((yyvsp[-1].numeric_literal_value_)));
    auto *name = new quickstep::ParseString((yylsp[-5]).first_line, (yylsp[-5]).first_column, "substring");
    (yyval.expression_) = new quickstep::ParseFunctionCall(
        (yylsp[-5]).first_line, (yylsp[-5]).first_column, false, name, arguments);
  }
#line 5363 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1749 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    auto *arguments = new quickstep::PtrList<quickstep::ParseExpression>();
    arguments->push_back((yyvsp[-5].expression_));
    arguments->push_back(new quickstep::ParseScalarLiteral((yyvsp[-3].numeric_literal_value_)));
    arguments->push_back(new quickstep::ParseScalarLiteral((yyvsp[-1].numeric_literal_value_)));
    auto *name = new quickstep::ParseString((yylsp[-7]).first_line, (yylsp[-7]).first_column, "substring");
    (yyval.expression_) = new quickstep::ParseFunctionCall(
        (yylsp[-7]).first_line, (yylsp[-7]).first_column, false, name, arguments);
  }
#line 5377 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1760 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSimpleCaseExpression((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].expression_), (yyvsp[-2].simple_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 5385 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1763 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseSearchedCaseExpression((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].searched_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 5393 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1768 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSimpleWhenClause>;
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 5402 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1772 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_list_) = (yyvsp[-1].simple_when_clause_list_);
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 5411 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1778 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.simple_when_clause_) = new quickstep::ParseSimpleWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 5419 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1783 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSearchedWhenClause>;
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 5428 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1787 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_list_) = (yyvsp[-1].searched_when_clause_list_);
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 5437 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1793 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.searched_when_clause_) = new quickstep::ParseSearchedWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].predicate_), (yyvsp[0].expression_));
  }
#line 5445 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1798 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = NULL;
  }
#line 5453 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1801 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 5461 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1806 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 5470 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1810 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 5479 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1816 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 5487 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1819 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5495 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1822 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 5503 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1825 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5520 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1837 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 5529 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1841 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5553 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1860 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 5571 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1875 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("YEAR"));
  }
#line 5579 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1878 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("MONTH"));
  }
#line 5587 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1881 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("DAY"));
  }
#line 5595 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1884 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("HOUR"));
  }
#line 5603 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1887 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("MINUTE"));
  }
#line 5611 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1890 "../SqlParser.ypp" /* yacc.c:1661  */
    {
     (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("SECOND"));
  }
#line 5619 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1895 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5628 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1899 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5637 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1905 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 5645 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1908 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 5653 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1913 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = new quickstep::PtrList<quickstep::ParseAttribute>();
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5662 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1917 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-2].attribute_list_);
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5671 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1924 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 5679 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1927 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 5687 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1930 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 5695 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1933 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 5703 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1936 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 5711 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1939 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 5719 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1942 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLike);
  }
#line 5727 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1945 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotLike);
  }
#line 5735 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1948 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kRegexMatch);
  }
#line 5743 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1951 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotRegexMatch);
  }
#line 5751 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 1956 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("-"));
  }
#line 5765 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1967 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("+"));
  }
#line 5773 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1970 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("-"));
  }
#line 5781 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1975 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("%"));
  }
#line 5789 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1978 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("*"));
  }
#line 5797 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1981 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, std::string("/"));
  }
#line 5805 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 1987 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 5814 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1991 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 5823 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1997 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5831 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 2000 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5842 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 2008 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5850 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 2011 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5858 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 2014 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5866 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 2017 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5874 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 2023 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.command_) = new quickstep::ParseCommand((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].command_argument_list_));
  }
#line 5882 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 2028 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    quickstep::PtrVector<quickstep::ParseString> *argument_list = (yyvsp[-1].command_argument_list_);
    argument_list->push_back((yyvsp[0].string_value_));
    (yyval.command_argument_list_) = argument_list;
  }
#line 5892 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 2033 "../SqlParser.ypp" /* yacc.c:1661  */
    { /* Epsilon, an empy match. */
    (yyval.command_argument_list_) = new quickstep::PtrVector<quickstep::ParseString>();
  }
#line 5900 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;


#line 5904 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 2037 "../SqlParser.ypp" /* yacc.c:1906  */


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
