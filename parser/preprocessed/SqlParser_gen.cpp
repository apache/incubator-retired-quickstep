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

#line 151 "SqlParser_gen.cpp" /* yacc.c:339  */

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
    TOKEN_BLOCKPROPERTIES = 285,
    TOKEN_BLOCKSAMPLE = 286,
    TOKEN_BLOOM_FILTER = 287,
    TOKEN_CSB_TREE = 288,
    TOKEN_BY = 289,
    TOKEN_CHARACTER = 290,
    TOKEN_CHECK = 291,
    TOKEN_COLUMN = 292,
    TOKEN_CONSTRAINT = 293,
    TOKEN_COPY = 294,
    TOKEN_CREATE = 295,
    TOKEN_DATE = 296,
    TOKEN_DATETIME = 297,
    TOKEN_DECIMAL = 298,
    TOKEN_DEFAULT = 299,
    TOKEN_DELETE = 300,
    TOKEN_DELIMITER = 301,
    TOKEN_DESC = 302,
    TOKEN_DISTINCT = 303,
    TOKEN_DOUBLE = 304,
    TOKEN_DROP = 305,
    TOKEN_ESCAPE_STRINGS = 306,
    TOKEN_EXISTS = 307,
    TOKEN_EXTRACT = 308,
    TOKEN_FALSE = 309,
    TOKEN_FIRST = 310,
    TOKEN_FLOAT = 311,
    TOKEN_FOREIGN = 312,
    TOKEN_FROM = 313,
    TOKEN_FULL = 314,
    TOKEN_GROUP = 315,
    TOKEN_HASH = 316,
    TOKEN_HAVING = 317,
    TOKEN_INDEX = 318,
    TOKEN_INNER = 319,
    TOKEN_INSERT = 320,
    TOKEN_INTEGER = 321,
    TOKEN_INTERVAL = 322,
    TOKEN_INTO = 323,
    TOKEN_JOIN = 324,
    TOKEN_KEY = 325,
    TOKEN_LAST = 326,
    TOKEN_LEFT = 327,
    TOKEN_LIMIT = 328,
    TOKEN_LONG = 329,
    TOKEN_NULL = 330,
    TOKEN_NULLS = 331,
    TOKEN_OFF = 332,
    TOKEN_ON = 333,
    TOKEN_ORDER = 334,
    TOKEN_OUTER = 335,
    TOKEN_PARTITION = 336,
    TOKEN_PARTITIONS = 337,
    TOKEN_PERCENT = 338,
    TOKEN_PRIMARY = 339,
    TOKEN_QUIT = 340,
    TOKEN_RANGE = 341,
    TOKEN_REAL = 342,
    TOKEN_REFERENCES = 343,
    TOKEN_RIGHT = 344,
    TOKEN_ROW_DELIMITER = 345,
    TOKEN_SELECT = 346,
    TOKEN_SET = 347,
    TOKEN_SMALLINT = 348,
    TOKEN_TABLE = 349,
    TOKEN_TIME = 350,
    TOKEN_TIMESTAMP = 351,
    TOKEN_TRUE = 352,
    TOKEN_TUPLESAMPLE = 353,
    TOKEN_UNIQUE = 354,
    TOKEN_UPDATE = 355,
    TOKEN_USING = 356,
    TOKEN_VALUES = 357,
    TOKEN_VARCHAR = 358,
    TOKEN_WHERE = 359,
    TOKEN_WITH = 360,
    TOKEN_YEARMONTH = 361,
    TOKEN_EOF = 362,
    TOKEN_LEX_ERROR = 363
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 115 "../SqlParser.ypp" /* yacc.c:355  */

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

#line 384 "SqlParser_gen.cpp" /* yacc.c:355  */
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
#line 199 "../SqlParser.ypp" /* yacc.c:358  */

/* This header needs YYSTYPE, which is defined by the %union directive above */
#include "SqlLexer_gen.hpp"
void NotSupported(const YYLTYPE *location, yyscan_t yyscanner, const std::string &feature);

#line 419 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYLAST   943

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  120
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  89
/* YYNRULES -- Number of rules.  */
#define YYNRULES  238
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  462

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   363

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     115,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   119,     2,     2,
     116,   117,    23,    21,   118,    22,    27,    24,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   114,
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
     110,   111,   112,   113
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   540,   540,   544,   548,   552,   556,   559,   566,   569,
     572,   575,   578,   581,   584,   587,   590,   593,   599,   605,
     612,   618,   625,   634,   639,   648,   653,   658,   662,   668,
     673,   676,   679,   684,   687,   690,   693,   696,   699,   702,
     705,   708,   711,   723,   726,   729,   747,   767,   770,   773,
     778,   783,   789,   795,   804,   808,   814,   817,   822,   827,
     832,   839,   846,   850,   856,   859,   864,   867,   872,   875,
     880,   883,   902,   906,   912,   916,   922,   925,   928,   933,
     936,   943,   948,   959,   963,   969,   972,   978,   986,   989,
     992,   998,  1003,  1006,  1011,  1015,  1019,  1023,  1029,  1034,
    1039,  1043,  1049,  1055,  1058,  1063,  1068,  1072,  1078,  1084,
    1090,  1093,  1097,  1103,  1106,  1111,  1115,  1121,  1124,  1127,
    1132,  1137,  1140,  1146,  1150,  1156,  1162,  1168,  1174,  1180,
    1186,  1192,  1198,  1206,  1211,  1214,  1217,  1222,  1226,  1230,
    1233,  1237,  1242,  1245,  1250,  1253,  1258,  1262,  1268,  1271,
    1276,  1279,  1284,  1287,  1292,  1295,  1314,  1318,  1324,  1331,
    1334,  1337,  1342,  1345,  1348,  1354,  1357,  1362,  1367,  1376,
    1381,  1390,  1395,  1398,  1403,  1406,  1411,  1417,  1423,  1426,
    1429,  1437,  1440,  1445,  1448,  1453,  1456,  1461,  1464,  1467,
    1470,  1473,  1478,  1482,  1486,  1489,  1494,  1499,  1503,  1509,
    1512,  1515,  1518,  1530,  1534,  1553,  1568,  1572,  1578,  1581,
    1586,  1590,  1597,  1600,  1603,  1606,  1609,  1612,  1615,  1618,
    1621,  1624,  1629,  1640,  1643,  1648,  1651,  1654,  1660,  1664,
    1670,  1673,  1681,  1684,  1687,  1690,  1696,  1701,  1706
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
  "TOKEN_BIGINT", "TOKEN_BIT", "TOKEN_BLOCKPROPERTIES",
  "TOKEN_BLOCKSAMPLE", "TOKEN_BLOOM_FILTER", "TOKEN_CSB_TREE", "TOKEN_BY",
  "TOKEN_CHARACTER", "TOKEN_CHECK", "TOKEN_COLUMN", "TOKEN_CONSTRAINT",
  "TOKEN_COPY", "TOKEN_CREATE", "TOKEN_DATE", "TOKEN_DATETIME",
  "TOKEN_DECIMAL", "TOKEN_DEFAULT", "TOKEN_DELETE", "TOKEN_DELIMITER",
  "TOKEN_DESC", "TOKEN_DISTINCT", "TOKEN_DOUBLE", "TOKEN_DROP",
  "TOKEN_ESCAPE_STRINGS", "TOKEN_EXISTS", "TOKEN_EXTRACT", "TOKEN_FALSE",
  "TOKEN_FIRST", "TOKEN_FLOAT", "TOKEN_FOREIGN", "TOKEN_FROM",
  "TOKEN_FULL", "TOKEN_GROUP", "TOKEN_HASH", "TOKEN_HAVING", "TOKEN_INDEX",
  "TOKEN_INNER", "TOKEN_INSERT", "TOKEN_INTEGER", "TOKEN_INTERVAL",
  "TOKEN_INTO", "TOKEN_JOIN", "TOKEN_KEY", "TOKEN_LAST", "TOKEN_LEFT",
  "TOKEN_LIMIT", "TOKEN_LONG", "TOKEN_NULL", "TOKEN_NULLS", "TOKEN_OFF",
  "TOKEN_ON", "TOKEN_ORDER", "TOKEN_OUTER", "TOKEN_PARTITION",
  "TOKEN_PARTITIONS", "TOKEN_PERCENT", "TOKEN_PRIMARY", "TOKEN_QUIT",
  "TOKEN_RANGE", "TOKEN_REAL", "TOKEN_REFERENCES", "TOKEN_RIGHT",
  "TOKEN_ROW_DELIMITER", "TOKEN_SELECT", "TOKEN_SET", "TOKEN_SMALLINT",
  "TOKEN_TABLE", "TOKEN_TIME", "TOKEN_TIMESTAMP", "TOKEN_TRUE",
  "TOKEN_TUPLESAMPLE", "TOKEN_UNIQUE", "TOKEN_UPDATE", "TOKEN_USING",
  "TOKEN_VALUES", "TOKEN_VARCHAR", "TOKEN_WHERE", "TOKEN_WITH",
  "TOKEN_YEARMONTH", "TOKEN_EOF", "TOKEN_LEX_ERROR", "';'", "'\\n'", "'('",
  "')'", "','", "'%'", "$accept", "start", "sql_statement",
  "quit_statement", "alter_table_statement", "create_table_statement",
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
  "extract_function", "expression_list", "literal_value",
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
     360,   361,   362,   363,    59,    10,    40,    41,    44,    37
};
# endif

#define YYPACT_NINF -206

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-206)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     535,  -206,  -206,   -32,   139,    11,    21,   -25,    47,  -206,
      54,   139,   139,  -206,   164,   112,  -206,  -206,  -206,  -206,
    -206,  -206,  -206,  -206,  -206,  -206,   -28,  -206,   127,   182,
     139,  -206,  -206,   130,   139,   139,   139,   139,   139,  -206,
    -206,   541,   114,    96,  -206,   197,   106,  -206,  -206,  -206,
     168,  -206,  -206,  -206,  -206,    57,   243,   169,   145,   154,
    -206,   102,  -206,  -206,   247,   260,  -206,  -206,  -206,   157,
    -206,   262,  -206,  -206,   161,  -206,  -206,   287,  -206,  -206,
    -206,  -206,  -206,  -206,   183,   269,   703,   314,   280,   228,
    -206,   138,    -3,  -206,  -206,  -206,  -206,  -206,  -206,   784,
      -2,   139,   139,   229,   139,   139,   217,   223,   237,   139,
     139,   460,  -206,  -206,   232,   139,  -206,  -206,  -206,   342,
    -206,   139,  -206,   343,  -206,     1,  -206,    13,   154,   703,
    -206,  -206,   139,   703,  -206,  -206,  -206,  -206,   703,   260,
    -206,   139,   361,   -48,  -206,   340,  -206,   257,  -206,    84,
    -206,   257,   139,    99,   139,   139,   238,  -206,   241,  -206,
     116,   827,   622,   229,   460,   351,   352,  -206,  -206,   272,
     349,   832,   152,   245,   300,   253,  -206,    60,  -206,    98,
      60,   -22,   306,  -206,  -206,    -3,  -206,  -206,   256,   703,
    -206,   254,   162,   139,  -206,   703,   258,  -206,   139,  -206,
    -206,   261,   299,   301,   264,  -206,  -206,  -206,    90,   139,
     275,    99,   139,  -206,   158,  -206,  -206,     2,    37,   460,
     460,   227,  -206,  -206,  -206,  -206,  -206,  -206,  -206,  -206,
     703,   703,    19,  -206,   178,   278,  -206,   703,  -206,   139,
    -206,  -206,    62,   304,   139,    92,   121,    13,  -206,   110,
    -206,  -206,   379,   380,    60,   350,   321,  -206,   180,  -206,
     703,  -206,   254,  -206,  -206,   460,   274,   276,   139,   386,
     -20,   194,  -206,   196,   366,   271,  -206,   279,   286,  -206,
     318,   283,   832,  -206,   327,   139,  -206,  -206,   158,  -206,
    -206,   352,  -206,  -206,  -206,   703,   149,   254,   325,  -206,
    -206,   832,   290,    22,  -206,   139,   336,   139,   -46,   139,
     337,   139,   338,  -206,  -206,   328,   329,  -206,   703,   460,
     334,  -206,   254,     4,   139,   139,   198,  -206,  -206,  -206,
    -206,  -206,  -206,  -206,   159,  -206,   139,  -206,  -206,   297,
      99,   385,   339,  -206,   460,  -206,  -206,   308,  -206,   191,
     703,  -206,  -206,   832,  -206,   -42,   139,   -38,   460,   -26,
     139,   -23,   139,  -206,  -206,   309,   351,   389,   353,  -206,
     200,   205,  -206,   424,   -20,  -206,   139,  -206,  -206,   319,
     391,  -206,    10,   139,   703,   254,   207,   460,   -14,   460,
     351,   460,   -10,   460,    -5,   703,   427,  -206,   344,  -206,
    -206,  -206,   209,  -206,  -206,  -206,  -206,     9,   139,    -1,
    -206,   322,   254,  -206,   351,   460,   351,   351,   460,   351,
     460,   320,  -206,   131,  -206,   139,  -206,   139,  -206,  -206,
     139,  -206,   212,  -206,  -206,   326,  -206,   351,   351,   351,
     703,  -206,  -206,   355,   330,  -206,   218,  -206,   139,  -206,
     105,  -206,   139,  -206,   220,  -206,  -206,   222,   356,  -206,
     438,  -206
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     6,   238,     0,     0,     0,     0,     0,     0,    18,
     110,     0,     0,     7,     0,     0,    15,     8,    10,    11,
      13,    14,     9,    17,    12,    16,     0,   103,     0,   236,
       0,   230,   231,     0,     0,     0,     0,     0,     0,   111,
     112,     0,     0,   105,   106,     0,   144,     1,     3,     2,
       0,   104,     5,     4,   237,     0,     0,     0,     0,   165,
      25,     0,   203,   200,     0,   222,   113,    40,    29,     0,
      30,    31,    34,    36,     0,    37,    39,     0,    41,   199,
      35,    38,    32,    33,     0,     0,     0,     0,     0,   114,
     115,   119,   182,   184,   186,   189,   190,   188,   187,     0,
     208,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,    99,   166,     0,     0,    89,   201,   202,     0,
      43,     0,   204,     0,    44,     0,   205,     0,   165,     0,
     223,   224,     0,     0,   118,   226,   227,   225,     0,     0,
     185,     0,     0,   165,   101,     0,   107,     0,   108,     0,
     228,     0,     0,     0,     0,     0,     0,    91,    66,    27,
       0,     0,     0,     0,     0,   167,   169,   171,   173,     0,
     187,     0,     0,     0,     0,     0,   191,     0,   146,   121,
     141,   134,   148,   116,   117,   181,   183,   209,     0,     0,
     192,   197,     0,     0,    98,     0,     0,   145,     0,    90,
      19,     0,     0,     0,     0,    20,    21,    22,     0,     0,
       0,    64,     0,    42,    56,   172,   180,     0,     0,     0,
       0,     0,   212,   214,   215,   216,   217,   213,   218,   220,
       0,     0,     0,   206,     0,     0,    45,     0,    46,     0,
     137,   142,     0,     0,     0,     0,     0,     0,   120,   122,
     124,   140,     0,     0,   139,     0,   150,   193,     0,   194,
       0,   100,   102,   133,   229,     0,     0,     0,     0,     0,
       0,     0,   210,     0,   208,     0,    63,    65,    68,    28,
       0,     0,     0,    47,     0,     0,    49,    55,    57,    26,
     179,   168,   170,   219,   221,     0,     0,   178,     0,   177,
      88,     0,     0,     0,   143,     0,     0,     0,     0,     0,
       0,     0,     0,   147,   123,     0,     0,   138,     0,     0,
     152,   195,   198,     0,     0,     0,     0,    94,   234,   235,
     233,   232,    95,    93,     0,    67,     0,    83,    84,    85,
       0,     0,    70,    48,     0,    51,    50,     0,    54,     0,
       0,   176,   207,     0,   196,     0,     0,     0,     0,     0,
       0,     0,     0,   135,   136,   149,   151,     0,   154,    61,
       0,     0,    58,     0,     0,   211,     0,    24,    62,     0,
       0,    23,     0,     0,     0,   174,     0,     0,     0,     0,
     126,     0,     0,     0,     0,     0,     0,   109,     0,    59,
      96,    97,     0,    74,    76,    77,    78,     0,     0,     0,
      52,     0,   175,    87,   132,     0,   125,   128,     0,   130,
       0,   153,   156,   159,   155,     0,    86,     0,    82,    80,
       0,    79,     0,    72,    73,     0,    53,   131,   127,   129,
       0,   160,   161,   162,     0,    75,     0,    69,     0,   157,
       0,   158,     0,    81,     0,   163,   164,     0,     0,    60,
       0,    71
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -206,  -206,  -206,  -206,  -206,  -206,  -206,  -206,  -124,  -206,
     288,   163,  -206,  -206,  -205,  -206,  -206,  -206,  -206,  -206,
    -206,    39,    23,  -206,  -206,  -206,  -206,  -206,  -206,  -206,
    -206,  -206,  -206,  -206,  -206,   259,  -206,  -206,  -206,   354,
     -21,  -206,  -206,  -206,   331,  -206,  -206,  -206,   206,   -67,
    -206,   210,  -145,    -9,  -206,  -206,  -206,  -206,  -206,  -206,
      14,  -206,  -206,   -57,  -206,  -162,   239,   248,   311,   -40,
     341,   333,   364,  -116,  -206,  -180,  -167,   122,   -87,  -206,
    -206,  -206,  -206,  -206,   -88,    -4,   109,  -206,  -206
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    14,    15,    16,    17,    18,    19,    20,   159,   160,
      87,   287,   288,   289,   205,   277,   278,   210,   342,   381,
     435,   402,   403,   404,   405,   406,   339,   377,    21,    22,
     157,   271,    23,    24,   143,   144,    25,    26,    43,    44,
      27,    41,    88,    89,    90,   128,   248,   249,   250,   177,
     254,   178,   240,   241,   179,   256,   320,   368,   397,   421,
     422,   443,   451,   112,   113,   165,   166,   167,   168,   169,
      92,    93,    94,    95,    96,   192,    97,   234,    98,   273,
     231,    99,   133,   138,   149,   100,   332,    28,    29
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
      33,    91,   217,    45,   233,    51,   276,    42,    46,   258,
     219,   180,   219,    31,   252,    32,   428,    31,   219,    32,
     135,   136,   130,   131,   170,   141,    55,   172,   200,   298,
      57,    58,    59,    60,    61,   251,   148,   358,   429,   328,
     116,   387,    50,   130,   131,   389,   125,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   391,   130,   131,
     393,   111,   329,   330,    31,   433,    32,    30,    10,   415,
     193,   182,   198,   418,    37,   170,   198,   170,   420,    34,
     198,   253,   331,    39,    36,   106,   194,   134,   279,    91,
     434,   239,   198,    45,   142,   198,   216,   145,    46,   299,
     150,   151,   191,   323,   198,   158,   161,    40,   198,   317,
      35,   150,   107,   198,   142,   345,   137,   174,   176,   290,
      38,   369,   272,   181,   218,   430,   196,   410,   184,   147,
     199,   180,   170,   170,   352,   378,   305,   187,   365,   354,
     201,   269,    31,    31,    32,    32,   270,   306,   161,   191,
     206,   207,   130,   131,   176,   262,   308,   366,   350,   130,
     131,   202,   242,   441,    47,   455,   309,   243,   280,   132,
     130,   131,   244,    46,   242,   245,    46,   310,   170,   243,
     326,   456,   382,   442,   244,    54,   233,   245,   203,   145,
     296,   297,   246,    56,   264,   311,   390,   303,    10,   281,
     384,   197,   198,   204,   246,   274,   312,   282,   161,   114,
     373,   101,   130,   131,   102,   374,   247,   355,   115,   357,
     322,   359,   104,   361,    48,   414,    49,   416,   103,   417,
     304,   419,   170,   211,   212,    46,   370,   371,   283,    52,
     150,   105,    53,   181,   293,   294,   295,   284,   108,   375,
      46,   285,   109,   437,   117,   349,   438,   170,   439,   152,
     153,   110,   286,   111,   150,   154,   155,   118,   388,   235,
     198,   170,   392,   119,   394,   130,   131,   121,   191,   259,
     260,   347,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   122,   130,   131,   300,   301,   321,   260,   123,
     170,   150,   170,   150,   170,   150,   170,   150,   337,   338,
     385,   333,   334,   335,   336,   372,   198,   398,   198,   126,
     150,   150,   399,   198,   413,   301,   426,   427,   170,   447,
     427,   170,   274,   170,   120,   453,   198,   458,   198,   459,
     198,   124,   446,   127,   412,   147,   129,   156,   171,   173,
     175,   195,   150,    10,   208,   423,   150,   209,   150,   219,
     454,   220,   236,   237,   457,    31,    62,    32,    63,   232,
     238,   255,   407,   257,   266,   263,   267,   265,   307,   411,
     268,   275,    64,    65,   188,   302,   315,   316,   319,   318,
     324,   327,   325,   141,    67,    68,   341,   340,   343,   344,
     423,    69,   346,   431,   407,   351,   353,    70,    71,    72,
     356,   360,   362,   376,   189,    73,   363,   364,   367,    74,
     379,   444,    75,   407,   383,   380,   150,   260,   395,   400,
     409,   396,    76,    77,   424,   408,   450,   425,   440,   436,
      78,    79,   448,   460,   150,   461,   452,   432,   150,   214,
     445,   348,   261,    80,   449,   314,   146,   313,   291,    81,
     183,    82,    83,   140,    31,    62,    32,    63,   292,    84,
     162,   186,    85,   215,   185,   386,     0,    86,   190,     0,
       0,    64,    65,   401,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,    68,     0,     0,     0,     0,     0,
      69,     0,     0,     0,     0,     0,    70,    71,    72,     0,
       0,     0,     0,     0,    73,     0,     0,   163,    74,     0,
       0,    75,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    76,    77,     0,     0,     0,     1,     0,     2,    78,
      79,     0,     0,     0,     0,    31,    62,    32,    63,     0,
       0,     0,    80,     0,     0,     0,     0,     0,    81,     0,
      82,    83,    64,    65,    66,     3,     0,     0,    84,     0,
       0,    85,     0,     0,    67,    68,   164,     0,     0,     4,
       5,    69,     0,     0,     0,     6,     0,    70,    71,    72,
       7,     0,     0,     0,     0,    73,     0,     0,     0,    74,
       0,     0,    75,     0,     0,     8,     0,     0,     0,     0,
       0,     0,    76,    77,     0,     0,     0,     0,     0,     0,
      78,    79,     0,     0,     0,     9,    31,    62,    32,    63,
       0,    10,     0,    80,     0,     0,     0,     0,     0,    81,
      11,    82,    83,    64,    65,    12,     0,    13,     0,    84,
       0,     0,    85,     0,     0,    67,    68,    86,     0,     0,
       0,     0,    69,     0,     0,     0,     0,     0,    70,    71,
      72,     0,     0,     0,     0,     0,    73,     0,     0,   163,
      74,     0,     0,    75,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    76,    77,     0,     0,     0,     0,     0,
       0,    78,    79,     0,     0,     0,     0,    31,    62,    32,
      63,     0,     0,     0,    80,     0,     0,     0,     0,     0,
      81,     0,    82,    83,    64,    65,     0,     0,     0,     0,
      84,     0,     0,    85,     0,     0,    67,    68,   164,     0,
       0,     0,     0,    69,     0,     0,     0,     0,     0,    70,
      71,    72,     0,     0,     0,     0,     0,    73,     0,     0,
       0,    74,     0,     0,    75,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    76,    77,     0,     0,     0,     0,
       0,     0,    78,    79,     0,     0,     0,     0,    31,    62,
      32,    63,     0,     0,     0,    80,     0,     0,     0,     0,
       0,    81,     0,    82,    83,    64,   139,     0,     0,     0,
       0,    84,     0,     0,    85,     0,     0,    67,    68,    86,
       0,     0,     0,     0,    69,     0,     0,     0,     0,     0,
      70,    71,    72,     0,     0,     0,     0,    62,    73,    63,
       0,     0,    74,     0,     0,    75,     0,     0,     0,     0,
       0,     0,     0,    64,   139,    76,    77,     0,     0,     0,
      67,    68,     0,    78,    79,    67,    68,    69,     0,     0,
       0,     0,    69,    70,    71,    72,    80,     0,    70,    71,
      72,    73,    81,     0,    82,    83,    73,     0,    75,     0,
       0,     0,    84,    75,     0,    85,     0,     0,    76,   213,
      86,     0,     0,    76,    77,     0,    78,     0,     0,     0,
       0,    78,    79,     0,     0,     0,     0,     0,     0,    80,
       0,     0,     0,     0,    80,    81,     0,    82,    83,     0,
      81,     0,    82,    83,     0,    84,     0,     0,    85,     0,
      84,     0,     0,    85
};

static const yytype_int16 yycheck[] =
{
       4,    41,   164,    12,   171,    26,   211,    11,    12,   189,
       8,   127,     8,     4,    36,     6,     7,     4,     8,     6,
      23,    24,    21,    22,   111,    27,    30,   115,   152,    10,
      34,    35,    36,    37,    38,   180,   103,    83,    29,    59,
      61,    83,    70,    21,    22,    83,    86,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    83,    21,    22,
      83,   109,    82,    83,     4,    66,     6,    99,    96,    83,
     118,   128,   118,    83,    99,   162,   118,   164,    83,    68,
     118,   103,   102,    29,    63,    28,   143,    91,   212,   129,
      91,    31,   118,   102,   116,   118,   163,   101,   102,    80,
     104,   105,   142,   265,   118,   109,   110,    53,   118,   254,
      99,   115,    55,   118,   116,   282,   119,   121,   117,   117,
      73,   117,   209,   127,   164,   116,   147,   117,   132,   116,
     151,   247,   219,   220,   301,   340,    74,   141,   318,   117,
      41,    51,     4,     4,     6,     6,    56,    85,   152,   189,
     154,   155,    21,    22,   117,   195,   244,   319,     9,    21,
      22,    62,    64,    32,     0,    60,    74,    69,    10,    31,
      21,    22,    74,   177,    64,    77,   180,    85,   265,    69,
     268,    76,   344,    52,    74,     3,   353,    77,    89,   193,
     230,   231,    94,    63,   198,    74,   358,   237,    96,    41,
       9,   117,   118,   104,    94,   209,    85,    49,   212,   107,
      51,    97,    21,    22,   118,    56,   118,   305,   116,   307,
     260,   309,   116,   311,   112,   387,   114,   389,    31,   391,
     239,   393,   319,   117,   118,   239,   324,   325,    80,   112,
     244,    73,   115,   247,    17,    18,    19,    89,     5,   336,
     254,    93,    83,   415,     7,   295,   418,   344,   420,    42,
      43,   116,   104,   109,   268,    42,    43,     7,   356,   117,
     118,   358,   360,   116,   362,    21,    22,   116,   318,   117,
     118,   285,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,     5,    21,    22,   117,   118,   117,   118,   116,
     387,   305,   389,   307,   391,   309,   393,   311,    37,    38,
     350,   117,   118,   117,   118,   117,   118,   117,   118,     5,
     324,   325,   117,   118,   117,   118,   117,   118,   415,   117,
     118,   418,   336,   420,    72,   117,   118,   117,   118,   117,
     118,    72,   430,    63,   384,   116,   118,   110,   116,     7,
       7,    11,   356,    96,   116,   395,   360,   116,   362,     8,
     448,     9,   117,    63,   452,     4,     5,     6,     7,    20,
     117,    65,   376,   117,    75,   117,    75,   116,    74,   383,
     116,   106,    21,    22,    23,   107,     7,     7,    67,    39,
     116,     5,   116,    27,    33,    34,   110,   118,    80,   116,
     440,    40,    75,   407,   408,    80,   116,    46,    47,    48,
      74,    74,    74,   116,    53,    54,    88,    88,    84,    58,
      35,   425,    61,   427,   116,    86,   430,   118,    39,     5,
      39,    78,    71,    72,     7,   116,    81,    93,   118,   117,
      79,    80,   116,    87,   448,     7,   116,   408,   452,   161,
     427,   288,   193,    92,   440,   249,   102,   247,   219,    98,
     129,   100,   101,    99,     4,     5,     6,     7,   220,   108,
      10,   138,   111,   162,   133,   353,    -1,   116,   117,    -1,
      -1,    21,    22,   374,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    47,    48,    -1,
      -1,    -1,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    -1,    -1,    -1,     1,    -1,     3,    79,
      80,    -1,    -1,    -1,    -1,     4,     5,     6,     7,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    98,    -1,
     100,   101,    21,    22,    23,    30,    -1,    -1,   108,    -1,
      -1,   111,    -1,    -1,    33,    34,   116,    -1,    -1,    44,
      45,    40,    -1,    -1,    -1,    50,    -1,    46,    47,    48,
      55,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,    58,
      -1,    -1,    61,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    -1,    -1,    -1,    90,     4,     5,     6,     7,
      -1,    96,    -1,    92,    -1,    -1,    -1,    -1,    -1,    98,
     105,   100,   101,    21,    22,   110,    -1,   112,    -1,   108,
      -1,    -1,   111,    -1,    -1,    33,    34,   116,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    47,
      48,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    57,
      58,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    -1,    -1,    -1,    -1,     4,     5,     6,
       7,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      98,    -1,   100,   101,    21,    22,    -1,    -1,    -1,    -1,
     108,    -1,    -1,   111,    -1,    -1,    33,    34,   116,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      47,    48,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    58,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    -1,    -1,    -1,    -1,     4,     5,
       6,     7,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    98,    -1,   100,   101,    21,    22,    -1,    -1,    -1,
      -1,   108,    -1,    -1,   111,    -1,    -1,    33,    34,   116,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    47,    48,    -1,    -1,    -1,    -1,     5,    54,     7,
      -1,    -1,    58,    -1,    -1,    61,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    71,    72,    -1,    -1,    -1,
      33,    34,    -1,    79,    80,    33,    34,    40,    -1,    -1,
      -1,    -1,    40,    46,    47,    48,    92,    -1,    46,    47,
      48,    54,    98,    -1,   100,   101,    54,    -1,    61,    -1,
      -1,    -1,   108,    61,    -1,   111,    -1,    -1,    71,    72,
     116,    -1,    -1,    71,    72,    -1,    79,    -1,    -1,    -1,
      -1,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    92,    98,    -1,   100,   101,    -1,
      98,    -1,   100,   101,    -1,   108,    -1,    -1,   111,    -1,
     108,    -1,    -1,   111
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,    30,    44,    45,    50,    55,    70,    90,
      96,   105,   110,   112,   121,   122,   123,   124,   125,   126,
     127,   148,   149,   152,   153,   156,   157,   160,   207,   208,
      99,     4,     6,   205,    68,    99,    63,    99,    73,    29,
      53,   161,   205,   158,   159,   173,   205,     0,   112,   114,
      70,   160,   112,   115,     3,   205,    63,   205,   205,   205,
     205,   205,     5,     7,    21,    22,    23,    33,    34,    40,
      46,    47,    48,    54,    58,    61,    71,    72,    79,    80,
      92,    98,   100,   101,   108,   111,   116,   130,   162,   163,
     164,   189,   190,   191,   192,   193,   194,   196,   198,   201,
     205,    97,   118,    31,   116,    73,    28,    55,     5,    83,
     116,   109,   183,   184,   107,   116,   160,     7,     7,   116,
      72,   116,     5,   116,    72,   189,     5,    63,   165,   118,
      21,    22,    31,   202,   205,    23,    24,   119,   203,    22,
     192,    27,   116,   154,   155,   205,   159,   116,   169,   204,
     205,   205,    42,    43,    42,    43,   110,   150,   205,   128,
     129,   205,    10,    57,   116,   185,   186,   187,   188,   189,
     198,   116,   204,     7,   205,     7,   117,   169,   171,   174,
     193,   205,   183,   164,   205,   190,   191,   205,    23,    53,
     117,   189,   195,   118,   183,    11,   160,   117,   118,   160,
     128,    41,    62,    89,   104,   134,   205,   205,   116,   116,
     137,   117,   118,    72,   130,   188,   169,   185,   189,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,   200,    20,   196,   197,   117,   117,    63,   117,    31,
     172,   173,    64,    69,    74,    77,    94,   118,   166,   167,
     168,   172,    36,   103,   170,    65,   175,   117,   195,   117,
     118,   155,   189,   117,   205,   116,    75,    75,   116,    51,
      56,   151,   198,   199,   205,   106,   134,   135,   136,   128,
      10,    41,    49,    80,    89,    93,   104,   131,   132,   133,
     117,   186,   187,    17,    18,    19,   189,   189,    10,    80,
     117,   118,   107,   189,   173,    74,    85,    74,   204,    74,
      85,    74,    85,   171,   168,     7,     7,   172,    39,    67,
     176,   117,   189,   185,   116,   116,   204,     5,    59,    82,
      83,   102,   206,   117,   118,   117,   118,    37,    38,   146,
     118,   110,   138,    80,   116,   196,    75,   205,   131,   189,
       9,    80,   196,   116,   117,   204,    74,   204,    83,   204,
      74,   204,    74,    88,    88,   195,   185,    84,   177,   117,
     204,   204,   117,    51,    56,   198,   116,   147,   134,    35,
      86,   139,   185,   116,     9,   189,   197,    83,   204,    83,
     185,    83,   204,    83,   204,    39,    78,   178,   117,   117,
       5,   206,   141,   142,   143,   144,   145,   205,   116,    39,
     117,   205,   189,   117,   185,    83,   185,   185,    83,   185,
      83,   179,   180,   189,     7,    93,   117,   118,     7,    29,
     116,   205,   141,    66,    91,   140,   117,   185,   185,   185,
     118,    32,    52,   181,   205,   142,   204,   117,   116,   180,
      81,   182,   116,   117,   204,    60,    76,   204,   117,   117,
      87,     7
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   120,   121,   121,   121,   121,   121,   121,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   123,   124,
     124,   124,   124,   125,   126,   127,   128,   129,   129,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   131,   131,   131,
     131,   131,   131,   131,   132,   132,   133,   133,   134,   134,
     134,   134,   135,   135,   136,   136,   137,   137,   138,   138,
     139,   139,   140,   140,   141,   141,   142,   142,   142,   143,
     143,   144,   145,   146,   146,   147,   147,   148,   148,   148,
     148,   149,   150,   150,   151,   151,   151,   151,   152,   153,
     154,   154,   155,   156,   156,   157,   158,   158,   159,   160,
     161,   161,   161,   162,   162,   163,   163,   164,   164,   164,
     165,   166,   166,   167,   167,   168,   168,   168,   168,   168,
     168,   168,   168,   169,   170,   170,   170,   171,   171,   171,
     171,   171,   172,   172,   173,   173,   174,   174,   175,   175,
     176,   176,   177,   177,   178,   178,   179,   179,   180,   181,
     181,   181,   182,   182,   182,   183,   183,   184,   185,   185,
     186,   186,   187,   187,   188,   188,   188,   188,   188,   188,
     188,   189,   189,   190,   190,   191,   191,   192,   192,   192,
     192,   192,   193,   193,   193,   193,   194,   195,   195,   196,
     196,   196,   196,   196,   196,   196,   197,   197,   198,   198,
     199,   199,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   201,   202,   202,   203,   203,   203,   204,   204,
     205,   205,   206,   206,   206,   206,   207,   208,   208
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
       2,     4,     2,     1,     1,     0,     3,    10,     7,     4,
       5,     5,     0,     4,     2,     2,     4,     4,     5,     4,
       3,     1,     3,     1,     2,     2,     1,     3,     3,     9,
       0,     1,     1,     1,     1,     1,     3,     3,     2,     1,
       3,     0,     1,     2,     1,     5,     4,     6,     5,     6,
       5,     6,     5,     3,     0,     3,     3,     2,     3,     2,
       2,     1,     1,     2,     1,     4,     1,     3,     0,     3,
       0,     2,     0,     3,     0,     2,     1,     3,     3,     0,
       1,     1,     0,     2,     2,     0,     1,     2,     3,     1,
       3,     1,     2,     1,     5,     6,     4,     3,     3,     3,
       2,     3,     1,     3,     1,     2,     1,     1,     1,     1,
       1,     3,     3,     4,     4,     5,     6,     1,     3,     1,
       1,     2,     2,     1,     2,     2,     1,     3,     1,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     2,     2,     0
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
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1797 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_NAME  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1807 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1817 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1827 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 7: /* TOKEN_UNSIGNED_NUMVAL  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 1837 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 122: /* sql_statement  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1847 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 123: /* quit_statement  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 1857 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 124: /* alter_table_statement  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1867 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 125: /* create_table_statement  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 1877 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 126: /* create_index_statement  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1887 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 127: /* drop_table_statement  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 1897 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 128: /* column_def  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 1907 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 129: /* column_def_commalist  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 1917 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 130: /* data_type  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 1927 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 131: /* column_constraint_def  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 1937 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 132: /* column_constraint_def_list  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1947 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 133: /* opt_column_constraint_def_list  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1957 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 137: /* opt_column_list  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 1967 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 138: /* opt_block_properties  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).block_properties_) != nullptr) {
    delete ((*yyvaluep).block_properties_);
  }
}
#line 1977 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 139: /* opt_partition_clause  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).partition_clause_) != nullptr) {
    delete ((*yyvaluep).partition_clause_);
  }
}
#line 1987 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 140: /* partition_type  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1997 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 141: /* key_value_list  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2007 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 142: /* key_value  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 2017 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 143: /* key_string_value  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 2027 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 144: /* key_string_list  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 2037 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 145: /* key_integer_value  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 2047 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 146: /* index_type  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2057 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 147: /* opt_index_properties  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2067 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 148: /* insert_statement  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 2077 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 149: /* copy_from_statement  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_statement_) != nullptr) {
    delete ((*yyvaluep).copy_from_statement_);
  }
}
#line 2087 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 150: /* opt_copy_from_params  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2097 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 151: /* copy_from_params  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2107 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 152: /* update_statement  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2117 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 153: /* delete_statement  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2127 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* assignment_list  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2137 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* assignment_item  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2147 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* select_statement  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_statement_) != nullptr) {
    delete ((*yyvaluep).select_statement_);
  }
}
#line 2157 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* with_clause  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2167 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* with_list  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2177 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* with_list_element  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2187 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* select_query  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2197 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* selection  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2207 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 163: /* selection_item_commalist  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2217 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 164: /* selection_item  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2227 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 165: /* from_clause  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2237 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* subquery_expression  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2247 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* opt_sample_clause  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_sample_clause_) != nullptr) {
    delete ((*yyvaluep).opt_sample_clause_);
  }
}
#line 2257 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* table_reference  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2267 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* table_reference_signature  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2277 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* table_reference_signature_primary  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2287 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* table_reference_commalist  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2297 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* opt_group_by_clause  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2307 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* opt_having_clause  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2317 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* opt_order_by_clause  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2327 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* opt_limit_clause  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2337 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* order_commalist  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2347 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* order_item  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2357 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* opt_order_direction  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2367 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* opt_nulls_first  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2377 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* opt_where_clause  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2387 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* where_clause  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2397 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* or_expression  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2407 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* and_expression  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2417 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* not_expression  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2427 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* predicate_expression_base  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2437 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* add_expression  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2447 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 190: /* multiply_expression  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2457 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 191: /* unary_expression  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2467 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 192: /* expression_base  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2477 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 193: /* function_call  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2487 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 194: /* extract_function  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2497 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 195: /* expression_list  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2507 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 196: /* literal_value  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2517 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 197: /* literal_value_commalist  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2527 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 198: /* attribute_ref  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2537 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 199: /* attribute_ref_list  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2547 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 200: /* comparison_operation  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2553 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 201: /* unary_operation  */
#line 528 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2559 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 202: /* add_operation  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2565 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 203: /* multiply_operation  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2571 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 204: /* name_commalist  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 2581 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 205: /* any_name  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2591 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 206: /* boolean_value  */
#line 526 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2597 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 207: /* command  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_) != nullptr) {
    delete ((*yyvaluep).command_);
  }
}
#line 2607 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 208: /* command_argument_list  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_argument_list_) != nullptr) {
    delete ((*yyvaluep).command_argument_list_);
  }
}
#line 2617 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 540 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2914 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 544 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2923 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 548 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 2932 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 5:
#line 552 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 2941 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 556 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    YYABORT;
  }
#line 2949 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 559 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 2958 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 566 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 2966 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 9:
#line 569 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].copy_from_statement_);
  }
#line 2974 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 10:
#line 572 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 2982 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 11:
#line 575 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 2990 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 12:
#line 578 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 2998 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 13:
#line 581 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 3006 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 14:
#line 584 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 3014 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 15:
#line 587 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 3022 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 16:
#line 590 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].select_statement_);
  }
#line 3030 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 17:
#line 593 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 3038 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 18:
#line 599 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3046 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 19:
#line 605 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3058 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 20:
#line 612 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3069 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 618 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3081 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 625 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3093 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 634 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].attribute_definition_list_), (yyvsp[-1].block_properties_), (yyvsp[0].partition_clause_));
  }
#line 3101 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 639 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].key_value_list_)) {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_), (yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].key_value_list_));
    } else {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_));
    }
  }
#line 3113 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 648 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 3121 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 653 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 3129 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 658 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3138 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 662 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3147 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 668 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 3157 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 673 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3165 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 676 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3173 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 679 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3183 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 684 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3191 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 687 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3199 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 690 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3207 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 693 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3215 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 696 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3223 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 699 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3231 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 702 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3239 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 705 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3247 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 708 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3255 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 711 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3272 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 723 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3280 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 726 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3288 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 729 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3311 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 747 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3334 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 767 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3342 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 770 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3350 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 773 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3360 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 778 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3370 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 783 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3381 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 789 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3392 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 795 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3404 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 804 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3413 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 808 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3422 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 814 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3430 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 817 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3438 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 822 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3448 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 827 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3458 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 832 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3470 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 839 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3480 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 846 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3489 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 850 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3498 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 856 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3506 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 859 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = $1; */
  }
#line 3514 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 864 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = nullptr;
  }
#line 3522 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 867 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-1].attribute_list_);
  }
#line 3530 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 872 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = nullptr;
  }
#line 3538 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 875 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].key_value_list_));
  }
#line 3546 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 880 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.partition_clause_) = nullptr;
  }
#line 3554 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 883 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3576 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 902 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::PartitionSchemeHeader::PartitionType::kHash));
  }
#line 3585 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 906 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::PartitionSchemeHeader::PartitionType::kRange));
  }
#line 3594 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 912 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3603 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 916 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3612 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 922 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 3620 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 925 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 3628 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 928 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 3636 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 933 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 3644 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 936 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_),
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 3654 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 943 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 3662 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 948 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 3676 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 959 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
#line 3685 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 963 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
#line 3694 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 969 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 3702 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 972 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 3710 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 978 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 3723 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 986 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertTuple((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 3731 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 989 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-3]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), nullptr);
  }
#line 3739 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 992 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-4]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), (yyvsp[-4].with_list_));
  }
#line 3747 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 998 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_statement_) = new quickstep::ParseStatementCopyFrom((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].copy_from_params_));
  }
#line 3755 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 1003 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = nullptr;
  }
#line 3763 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 1006 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-1].copy_from_params_);
  }
#line 3771 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 1011 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3780 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 1015 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3789 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 1019 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3798 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 1023 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3807 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 1029 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 3815 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 1034 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 3823 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 1039 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3832 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 1043 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3841 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 1049 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 3849 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 1055 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].select_query_), nullptr);
  }
#line 3857 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 1058 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].select_query_), (yyvsp[-1].with_list_));
  }
#line 3865 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 1063 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 3873 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 1068 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3882 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 1072 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3891 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 1078 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 3900 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 1085 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].selection_), (yyvsp[-5].table_reference_list_), (yyvsp[-4].predicate_), (yyvsp[-3].opt_group_by_clause_), (yyvsp[-2].opt_having_clause_), (yyvsp[-1].opt_order_by_clause_), (yyvsp[0].opt_limit_clause_));
  }
#line 3908 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 1090 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3916 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 1093 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "ALL in selection");
    YYERROR;
  }
#line 3925 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 1097 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "DISTINCT in selection");
    YYERROR;
  }
#line 3934 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 1103 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3942 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 1106 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 3950 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 1111 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3959 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 1115 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3968 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 1121 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 3976 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 1124 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 3984 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 1127 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 3992 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 1132 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-1].table_reference_list_);
  }
#line 4000 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 1137 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 4008 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 1140 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4017 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 1146 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-1]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4026 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 1150 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4035 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 1156 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4046 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 1162 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4057 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 1168 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4068 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 1174 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4079 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 1180 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4090 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 1186 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4101 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 1192 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4112 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 1198 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4123 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 1206 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].select_query_));
  }
#line 4131 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 1211 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = NULL;
  }
#line 4139 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 1214 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, true, (yyvsp[-1].numeric_literal_value_));
  }
#line 4147 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 1217 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].numeric_literal_value_));
  }
#line 4155 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 1222 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4164 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 1226 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].opt_sample_clause_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4173 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 1230 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].opt_sample_clause_));
  }
#line 4181 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 1233 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4190 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 1237 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].function_call_));
  }
#line 4198 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 1242 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4206 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 1245 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4214 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 1250 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4222 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 1253 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4230 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 1258 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4239 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 1262 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4248 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 1268 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 4256 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 1271 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4264 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 1276 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4272 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 1279 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4280 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 1284 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4288 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 1287 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4296 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 1292 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4304 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 1295 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4326 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 1314 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4335 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 1318 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4344 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 1324 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 4354 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 1331 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4362 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 1334 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4370 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 1337 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4378 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 1342 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4386 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 1345 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4394 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 1348 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4402 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 1354 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 4410 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 1357 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4418 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 1362 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4426 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 1367 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4440 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 1376 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4448 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 1381 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4462 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 1390 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4470 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 1395 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4478 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 1398 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4486 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 1403 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4494 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 1406 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 4504 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 1411 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4515 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 1417 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4526 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 1423 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4534 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 1426 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 4542 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 1429 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateExists((yylsp[-1]).first_line,
                                             (yylsp[-1]).first_column,
                                             (yyvsp[0].subquery_expression_));
  }
#line 4552 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 1437 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4560 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 1440 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4568 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 1445 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4576 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 1448 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4584 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 1453 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].unary_operation_), (yyvsp[0].expression_));
  }
#line 4592 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1456 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4600 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 1461 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 4608 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 1464 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 4616 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 1467 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 4624 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 1470 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4632 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 1473 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 4640 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 1478 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 4649 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 1482 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 4658 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 1486 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, false, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4666 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 1489 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-4]).first_line, (yylsp[-4]).first_column, true, (yyvsp[-4].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4674 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 1494 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseExtractFunction((yylsp[-5]).first_line, (yylsp[-5]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].expression_));
  }
#line 4682 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 1499 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4691 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 1503 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4700 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 1509 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4708 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1512 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4716 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1515 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4724 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1518 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4741 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1530 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 4750 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1534 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4774 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1553 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4792 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1568 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4801 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1572 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4810 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1578 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4818 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1581 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 4826 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1586 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = new quickstep::PtrList<quickstep::ParseAttribute>();
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 4835 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1590 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-2].attribute_list_);
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 4844 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1597 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 4852 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1600 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 4860 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1603 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 4868 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1606 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 4876 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1609 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 4884 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1612 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 4892 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1615 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLike);
  }
#line 4900 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1618 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotLike);
  }
#line 4908 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1621 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kRegexMatch);
  }
#line 4916 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1624 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotRegexMatch);
  }
#line 4924 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1629 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
#line 4938 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1640 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
#line 4946 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1643 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
#line 4954 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1648 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kModulo);
  }
#line 4962 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1651 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
#line 4970 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1654 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
#line 4978 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1660 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4987 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1664 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4996 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1670 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5004 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1673 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5015 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1681 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5023 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1684 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5031 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1687 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5039 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1690 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5047 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1696 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.command_) = new quickstep::ParseCommand((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].command_argument_list_));
  }
#line 5055 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1701 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    quickstep::PtrVector<quickstep::ParseString> *argument_list = (yyvsp[-1].command_argument_list_);
    argument_list->push_back((yyvsp[0].string_value_));
    (yyval.command_argument_list_) = argument_list;
  }
#line 5065 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1706 "../SqlParser.ypp" /* yacc.c:1661  */
    { /* Epsilon, an empy match. */
    (yyval.command_argument_list_) = new quickstep::PtrVector<quickstep::ParseString>();
  }
#line 5073 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;


#line 5077 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 1710 "../SqlParser.ypp" /* yacc.c:1906  */


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
