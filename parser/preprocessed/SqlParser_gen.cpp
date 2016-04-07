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

#line 150 "SqlParser_gen.cpp" /* yacc.c:339  */

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
    TOKEN_EXTRACT = 307,
    TOKEN_FALSE = 308,
    TOKEN_FIRST = 309,
    TOKEN_FLOAT = 310,
    TOKEN_FOREIGN = 311,
    TOKEN_FROM = 312,
    TOKEN_FULL = 313,
    TOKEN_GROUP = 314,
    TOKEN_HASH = 315,
    TOKEN_HAVING = 316,
    TOKEN_INDEX = 317,
    TOKEN_INNER = 318,
    TOKEN_INSERT = 319,
    TOKEN_INTEGER = 320,
    TOKEN_INTERVAL = 321,
    TOKEN_INTO = 322,
    TOKEN_JOIN = 323,
    TOKEN_KEY = 324,
    TOKEN_LAST = 325,
    TOKEN_LEFT = 326,
    TOKEN_LIMIT = 327,
    TOKEN_LONG = 328,
    TOKEN_NULL = 329,
    TOKEN_NULLS = 330,
    TOKEN_OFF = 331,
    TOKEN_ON = 332,
    TOKEN_ORDER = 333,
    TOKEN_OUTER = 334,
    TOKEN_PARTITION = 335,
    TOKEN_PARTITIONS = 336,
    TOKEN_PERCENT = 337,
    TOKEN_PRIMARY = 338,
    TOKEN_QUIT = 339,
    TOKEN_RANGE = 340,
    TOKEN_REAL = 341,
    TOKEN_REFERENCES = 342,
    TOKEN_RIGHT = 343,
    TOKEN_ROW_DELIMITER = 344,
    TOKEN_SELECT = 345,
    TOKEN_SET = 346,
    TOKEN_SMALLINT = 347,
    TOKEN_TABLE = 348,
    TOKEN_TIME = 349,
    TOKEN_TIMESTAMP = 350,
    TOKEN_TRUE = 351,
    TOKEN_TUPLESAMPLE = 352,
    TOKEN_UNIQUE = 353,
    TOKEN_UPDATE = 354,
    TOKEN_USING = 355,
    TOKEN_VALUES = 356,
    TOKEN_VARCHAR = 357,
    TOKEN_WHERE = 358,
    TOKEN_WITH = 359,
    TOKEN_YEARMONTH = 360,
    TOKEN_EOF = 361,
    TOKEN_LEX_ERROR = 362
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 114 "../SqlParser.ypp" /* yacc.c:355  */

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

#line 382 "SqlParser_gen.cpp" /* yacc.c:355  */
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
#line 198 "../SqlParser.ypp" /* yacc.c:358  */

/* This header needs YYSTYPE, which is defined by the %union directive above */
#include "SqlLexer_gen.hpp"
void NotSupported(const YYLTYPE *location, yyscan_t yyscanner, const std::string &feature);

#line 417 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYLAST   982

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  119
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  89
/* YYNRULES -- Number of rules.  */
#define YYNRULES  237
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  460

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   362

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     114,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   118,     2,     2,
     115,   116,    23,    21,   117,    22,    27,    24,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   113,
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
     110,   111,   112
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   538,   538,   542,   546,   550,   554,   557,   564,   567,
     570,   573,   576,   579,   582,   585,   588,   591,   597,   603,
     610,   616,   623,   632,   637,   646,   651,   656,   660,   666,
     671,   674,   677,   682,   685,   688,   691,   694,   697,   700,
     703,   706,   709,   721,   724,   727,   745,   765,   768,   771,
     776,   781,   787,   793,   802,   806,   812,   815,   820,   825,
     830,   837,   844,   848,   854,   857,   862,   865,   870,   873,
     878,   881,   900,   904,   910,   914,   920,   923,   926,   931,
     934,   941,   946,   957,   961,   967,   970,   976,   984,   987,
     990,   996,  1001,  1004,  1009,  1013,  1017,  1021,  1027,  1032,
    1037,  1041,  1047,  1053,  1056,  1061,  1066,  1070,  1076,  1082,
    1088,  1091,  1095,  1101,  1104,  1109,  1113,  1119,  1122,  1125,
    1130,  1135,  1138,  1144,  1148,  1154,  1160,  1166,  1172,  1178,
    1184,  1190,  1196,  1204,  1209,  1212,  1215,  1220,  1224,  1228,
    1231,  1235,  1240,  1243,  1248,  1251,  1256,  1260,  1266,  1269,
    1274,  1277,  1282,  1285,  1290,  1293,  1312,  1316,  1322,  1329,
    1332,  1335,  1340,  1343,  1346,  1352,  1355,  1360,  1365,  1374,
    1379,  1388,  1393,  1396,  1401,  1404,  1409,  1415,  1421,  1424,
    1430,  1433,  1438,  1441,  1446,  1449,  1454,  1457,  1460,  1463,
    1466,  1471,  1475,  1479,  1482,  1487,  1492,  1496,  1502,  1505,
    1508,  1511,  1523,  1527,  1546,  1561,  1565,  1571,  1574,  1579,
    1583,  1590,  1593,  1596,  1599,  1602,  1605,  1608,  1611,  1614,
    1617,  1622,  1633,  1636,  1641,  1644,  1647,  1653,  1657,  1663,
    1666,  1674,  1677,  1680,  1683,  1689,  1694,  1699
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
  "TOKEN_ESCAPE_STRINGS", "TOKEN_EXTRACT", "TOKEN_FALSE", "TOKEN_FIRST",
  "TOKEN_FLOAT", "TOKEN_FOREIGN", "TOKEN_FROM", "TOKEN_FULL",
  "TOKEN_GROUP", "TOKEN_HASH", "TOKEN_HAVING", "TOKEN_INDEX",
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
     360,   361,   362,    59,    10,    40,    41,    44,    37
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
     303,  -209,  -209,    -9,   150,   -10,    65,    46,    37,  -209,
       7,   150,   150,  -209,   176,    47,  -209,  -209,  -209,  -209,
    -209,  -209,  -209,  -209,  -209,  -209,    76,  -209,   -21,   178,
     150,  -209,  -209,   122,   150,   150,   150,   150,   150,  -209,
    -209,   463,   103,    86,  -209,   204,    94,  -209,  -209,  -209,
     153,  -209,  -209,  -209,  -209,    84,   222,   147,   135,   144,
    -209,   145,  -209,  -209,   254,   257,  -209,  -209,  -209,   163,
    -209,   213,  -209,  -209,   173,  -209,  -209,   285,  -209,  -209,
    -209,  -209,  -209,  -209,   177,   224,   623,   295,   240,   203,
    -209,   215,     1,  -209,  -209,  -209,  -209,  -209,  -209,   703,
      -8,   150,   150,   201,   150,   150,    10,    53,   217,   150,
     150,   543,  -209,  -209,   212,   150,  -209,  -209,  -209,   321,
    -209,   150,  -209,   322,  -209,     6,  -209,     5,   144,   623,
    -209,  -209,   150,   623,  -209,  -209,  -209,  -209,   623,   257,
    -209,   150,   383,   -14,  -209,   320,  -209,   237,  -209,    73,
    -209,   237,   150,    22,   150,   150,   219,  -209,   226,  -209,
     106,   872,   783,   543,   329,   334,  -209,  -209,   943,   324,
     817,   138,   229,   284,   233,  -209,   143,  -209,    89,   143,
     -16,   287,  -209,  -209,     1,  -209,  -209,   236,   623,  -209,
     235,   142,   150,  -209,   623,   241,  -209,   150,  -209,  -209,
     244,   286,   288,   246,  -209,  -209,  -209,    -1,   150,   258,
      22,   150,  -209,   136,  -209,     2,    54,   543,   543,   199,
    -209,  -209,  -209,  -209,  -209,  -209,  -209,  -209,   623,   623,
      25,  -209,   154,   259,  -209,   623,  -209,   150,  -209,  -209,
     107,   298,   150,   113,   169,     5,  -209,   137,  -209,  -209,
     361,   366,   143,   335,   309,  -209,   156,  -209,   623,  -209,
     235,  -209,  -209,   543,   261,   263,   150,   374,    82,   158,
    -209,   160,   353,   243,  -209,   264,   273,  -209,   304,   269,
     817,  -209,   311,   150,  -209,  -209,   136,  -209,  -209,   334,
    -209,  -209,  -209,   623,   157,   235,   307,  -209,  -209,   817,
     276,    16,  -209,   150,   323,   150,   -59,   150,   326,   150,
     327,  -209,  -209,   306,   308,  -209,   623,   543,   314,  -209,
     235,     8,   150,   150,   166,  -209,  -209,  -209,  -209,  -209,
    -209,  -209,     0,  -209,   150,  -209,  -209,   279,    22,   368,
     325,  -209,   543,  -209,  -209,   293,  -209,   223,   623,  -209,
    -209,   817,  -209,   -43,   150,   -40,   543,   -39,   150,   -38,
     150,  -209,  -209,   292,   329,   376,   336,  -209,   170,   181,
    -209,   413,    82,  -209,   150,  -209,  -209,   305,   386,  -209,
      13,   150,   623,   235,   192,   543,   -37,   543,   329,   543,
     -35,   543,   -33,   623,   415,  -209,   340,  -209,  -209,  -209,
     194,  -209,  -209,  -209,  -209,    11,   150,   -36,  -209,   310,
     235,  -209,   329,   543,   329,   329,   543,   329,   543,   316,
    -209,   121,  -209,   150,  -209,   150,  -209,  -209,   150,  -209,
     196,  -209,  -209,   312,  -209,   329,   329,   329,   623,  -209,
    -209,   348,   319,  -209,   198,  -209,   150,  -209,    33,  -209,
     150,  -209,   206,  -209,  -209,   208,   349,  -209,   431,  -209
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     6,   237,     0,     0,     0,     0,     0,     0,    18,
     110,     0,     0,     7,     0,     0,    15,     8,    10,    11,
      13,    14,     9,    17,    12,    16,     0,   103,     0,   235,
       0,   229,   230,     0,     0,     0,     0,     0,     0,   111,
     112,     0,     0,   105,   106,     0,   144,     1,     3,     2,
       0,   104,     5,     4,   236,     0,     0,     0,     0,   165,
      25,     0,   202,   199,     0,   221,   113,    40,    29,     0,
      30,    31,    34,    36,     0,    37,    39,     0,    41,   198,
      35,    38,    32,    33,     0,     0,     0,     0,     0,   114,
     115,   119,   181,   183,   185,   188,   189,   187,   186,     0,
     207,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,    99,   166,     0,     0,    89,   200,   201,     0,
      43,     0,   203,     0,    44,     0,   204,     0,   165,     0,
     222,   223,     0,     0,   118,   225,   226,   224,     0,     0,
     184,     0,     0,   165,   101,     0,   107,     0,   108,     0,
     227,     0,     0,     0,     0,     0,     0,    91,    66,    27,
       0,     0,     0,     0,   167,   169,   171,   173,     0,   186,
       0,     0,     0,     0,     0,   190,     0,   146,   121,   141,
     134,   148,   116,   117,   180,   182,   208,     0,     0,   191,
     196,     0,     0,    98,     0,     0,   145,     0,    90,    19,
       0,     0,     0,     0,    20,    21,    22,     0,     0,     0,
      64,     0,    42,    56,   172,     0,     0,     0,     0,     0,
     211,   213,   214,   215,   216,   212,   217,   219,     0,     0,
       0,   205,     0,     0,    45,     0,    46,     0,   137,   142,
       0,     0,     0,     0,     0,     0,   120,   122,   124,   140,
       0,     0,   139,     0,   150,   192,     0,   193,     0,   100,
     102,   133,   228,     0,     0,     0,     0,     0,     0,     0,
     209,     0,   207,     0,    63,    65,    68,    28,     0,     0,
       0,    47,     0,     0,    49,    55,    57,    26,   179,   168,
     170,   218,   220,     0,     0,   178,     0,   177,    88,     0,
       0,     0,   143,     0,     0,     0,     0,     0,     0,     0,
       0,   147,   123,     0,     0,   138,     0,     0,   152,   194,
     197,     0,     0,     0,     0,    94,   233,   234,   232,   231,
      95,    93,     0,    67,     0,    83,    84,    85,     0,     0,
      70,    48,     0,    51,    50,     0,    54,     0,     0,   176,
     206,     0,   195,     0,     0,     0,     0,     0,     0,     0,
       0,   135,   136,   149,   151,     0,   154,    61,     0,     0,
      58,     0,     0,   210,     0,    24,    62,     0,     0,    23,
       0,     0,     0,   174,     0,     0,     0,     0,   126,     0,
       0,     0,     0,     0,     0,   109,     0,    59,    96,    97,
       0,    74,    76,    77,    78,     0,     0,     0,    52,     0,
     175,    87,   132,     0,   125,   128,     0,   130,     0,   153,
     156,   159,   155,     0,    86,     0,    82,    80,     0,    79,
       0,    72,    73,     0,    53,   131,   127,   129,     0,   160,
     161,   162,     0,    75,     0,    69,     0,   157,     0,   158,
       0,    81,     0,   163,   164,     0,     0,    60,     0,    71
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -209,  -209,  -209,  -209,  -209,  -209,  -209,  -209,  -130,  -209,
     278,   155,  -209,  -209,  -208,  -209,  -209,  -209,  -209,  -209,
    -209,    38,    20,  -209,  -209,  -209,  -209,  -209,  -209,  -209,
    -209,  -209,  -209,  -209,  -209,   255,  -209,  -209,  -209,   346,
     -13,  -209,  -209,  -209,   328,  -209,  -209,  -209,   202,   347,
    -209,   207,  -167,   -11,  -209,  -209,  -209,  -209,  -209,  -209,
      17,  -209,  -209,   -82,  -209,  -122,   234,   238,   296,   -27,
     330,   333,   360,  -124,  -209,  -183,  -164,   109,   -49,  -209,
    -209,  -209,  -209,  -209,  -111,    -4,    92,  -209,  -209
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    14,    15,    16,    17,    18,    19,    20,   159,   160,
      87,   285,   286,   287,   204,   275,   276,   209,   340,   379,
     433,   400,   401,   402,   403,   404,   337,   375,    21,    22,
     157,   269,    23,    24,   143,   144,    25,    26,    43,    44,
      27,    41,    88,    89,    90,   128,   246,   247,   248,   176,
     252,   177,   238,   239,   178,   254,   318,   366,   395,   419,
     420,   441,   449,   112,   113,   164,   165,   166,   167,   168,
      92,    93,    94,    95,    96,   191,    97,   232,    98,   271,
     229,    99,   133,   138,   149,   100,   330,    28,    29
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
      33,    45,   274,   179,   171,   256,   231,    42,    46,    31,
     217,    32,   249,    51,    91,    31,   217,    32,   426,   141,
     250,   217,   199,   356,   135,   136,    55,   130,   131,   431,
      57,    58,    59,    60,    61,   296,    39,   130,   131,   385,
     427,   215,   387,   389,   391,   413,   181,   416,   116,   418,
     267,   371,   152,   153,   432,   268,   372,    34,   197,   125,
      40,   193,   169,   200,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   197,   130,   131,   197,   197,   197,
     197,   277,   197,   201,   197,   315,   251,   134,    35,    30,
      52,    45,   453,    53,   111,   154,   155,   145,    46,   142,
     150,   151,    91,   192,   297,   158,   161,   142,   454,    38,
     202,   150,   106,   169,   169,   190,   343,   173,   288,   137,
     147,   179,   175,   180,   367,   203,   428,    36,   183,   408,
     376,   306,   352,   363,   195,   350,   216,   186,   198,   107,
     326,   321,   130,   131,    37,    50,   278,    31,   161,    32,
     205,   206,   240,   439,    31,   324,    32,   241,    48,   270,
      49,   190,   242,   327,   328,   243,   348,   260,   169,   169,
     175,    10,    46,   440,   237,    46,    47,   279,   130,   131,
     303,    54,   244,   329,    56,   280,   307,   231,   145,   196,
     197,   304,   353,   262,   355,   364,   357,   308,   359,   101,
     240,   294,   295,   102,   272,   241,   245,   161,   301,   104,
     242,   368,   369,   243,   169,   281,   291,   292,   293,    31,
     380,    32,   210,   211,   282,   105,   302,   108,   283,   109,
     244,   320,   382,    46,   388,   103,   130,   131,   150,   284,
      10,   180,   309,   386,   130,   131,   132,   390,    46,   392,
     110,   114,   111,   310,   233,   197,   130,   131,   257,   258,
     115,   117,   150,   412,   118,   414,   347,   415,   169,   417,
     298,   299,   319,   258,   331,   332,   333,   334,   119,   345,
     335,   336,   370,   197,   120,   373,   396,   197,   121,   190,
     122,   435,   123,   169,   436,   124,   437,   397,   197,   150,
     126,   150,   127,   150,     1,   150,     2,   169,   411,   299,
     424,   425,   445,   425,   451,   197,   147,   444,   150,   150,
     129,   383,   456,   197,   457,   197,   156,   170,   172,   174,
     272,   194,    10,     3,   207,   452,   169,   217,   169,   455,
     169,   208,   169,   218,   230,   234,   235,     4,     5,   236,
     150,   253,   255,     6,   150,   410,   150,   261,     7,   263,
     264,   266,   265,   273,   169,   300,   421,   169,   313,   169,
     405,   305,     8,   314,   316,   317,   322,   409,   323,   325,
     141,   338,   339,   341,   342,   344,   349,    31,    62,    32,
      63,   351,     9,   361,   374,   362,   354,   365,    10,   358,
     360,   429,   405,   377,    64,    65,   187,    11,   381,   258,
     378,   421,    12,   394,    13,   393,    67,    68,   398,   442,
     406,   405,   422,    69,   150,   407,   434,   446,   448,    70,
      71,    72,   423,   438,   450,   458,   188,    73,   459,   213,
      74,   346,   150,    75,   430,   443,   150,   259,   146,   312,
     148,   289,   311,    76,    77,   447,   290,   182,   214,   140,
     384,    78,    79,   184,   399,     0,     0,    31,    62,    32,
      63,   185,     0,     0,    80,     0,     0,     0,     0,     0,
      81,     0,    82,    83,    64,    65,    66,     0,     0,     0,
      84,     0,     0,    85,     0,     0,    67,    68,    86,   189,
       0,     0,     0,    69,     0,     0,     0,     0,     0,    70,
      71,    72,     0,     0,     0,     0,     0,    73,     0,     0,
      74,     0,     0,    75,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    76,    77,     0,     0,     0,     0,     0,
       0,    78,    79,     0,     0,     0,     0,    31,    62,    32,
      63,     0,     0,   162,    80,     0,     0,     0,     0,     0,
      81,     0,    82,    83,    64,    65,     0,     0,     0,     0,
      84,     0,     0,    85,     0,     0,    67,    68,    86,     0,
       0,     0,     0,    69,     0,     0,     0,     0,     0,    70,
      71,    72,     0,     0,     0,     0,     0,    73,     0,     0,
      74,     0,     0,    75,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    76,    77,     0,     0,     0,     0,     0,
       0,    78,    79,     0,     0,     0,     0,    31,    62,    32,
      63,     0,     0,     0,    80,     0,     0,     0,     0,     0,
      81,     0,    82,    83,    64,    65,     0,     0,     0,     0,
      84,     0,     0,    85,     0,     0,    67,    68,   163,     0,
       0,     0,     0,    69,     0,     0,     0,     0,     0,    70,
      71,    72,     0,     0,     0,     0,     0,    73,     0,     0,
      74,     0,     0,    75,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    76,    77,     0,     0,     0,     0,     0,
       0,    78,    79,     0,     0,     0,     0,    31,    62,    32,
      63,     0,     0,     0,    80,     0,     0,     0,     0,     0,
      81,     0,    82,    83,    64,   139,     0,     0,     0,     0,
      84,     0,     0,    85,     0,     0,    67,    68,    86,     0,
       0,     0,     0,    69,     0,     0,     0,     0,     0,    70,
      71,    72,     0,     0,     0,     0,     0,    73,     0,     0,
      74,     0,     0,    75,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    76,    77,     0,     0,     0,     0,     0,
       0,    78,    79,     0,     0,     0,     0,    31,    62,    32,
      63,     0,     0,     0,    80,     0,     0,     0,     0,     0,
      81,     0,    82,    83,    64,    65,     0,     0,     0,     0,
      84,     0,     0,    85,     0,     0,    67,    68,    86,     0,
       0,     0,    62,    69,    63,     0,     0,     0,     0,    70,
      71,    72,     0,     0,     0,     0,     0,    73,    64,   139,
      74,     0,     0,    75,     0,     0,     0,     0,     0,     0,
      67,    68,     0,    76,    77,     0,     0,    69,     0,     0,
       0,    78,    79,    70,    71,    72,     0,     0,     0,     0,
       0,    73,     0,     0,    80,     0,     0,    75,     0,     0,
      81,     0,    82,    83,     0,     0,     0,    76,    77,     0,
      84,     0,     0,    85,     0,    78,    79,     0,   163,     0,
       0,     0,     0,     0,     0,    67,    68,     0,    80,     0,
       0,     0,    69,     0,    81,     0,    82,    83,    70,    71,
      72,     0,     0,     0,    84,     0,    73,    85,     0,     0,
       0,     0,    75,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    76,   212,     0,     0,     0,     0,     0,     0,
      78,     0,     0,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    80,   130,   131,     0,     0,     0,    81,
       0,    82,    83,     0,     0,     0,     0,     0,     0,    84,
       0,     0,    85
};

static const yytype_int16 yycheck[] =
{
       4,    12,   210,   127,   115,   188,   170,    11,    12,     4,
       8,     6,   179,    26,    41,     4,     8,     6,     7,    27,
      36,     8,   152,    82,    23,    24,    30,    21,    22,    65,
      34,    35,    36,    37,    38,    10,    29,    21,    22,    82,
      29,   163,    82,    82,    82,    82,   128,    82,    61,    82,
      51,    51,    42,    43,    90,    56,    56,    67,   117,    86,
      53,   143,   111,    41,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,   117,    21,    22,   117,   117,   117,
     117,   211,   117,    61,   117,   252,   102,    91,    98,    98,
     111,   102,    59,   114,   108,    42,    43,   101,   102,   115,
     104,   105,   129,   117,    79,   109,   110,   115,    75,    72,
      88,   115,    28,   162,   163,   142,   280,   121,   116,   118,
     115,   245,   116,   127,   116,   103,   115,    62,   132,   116,
     338,   242,   116,   316,   147,   299,   163,   141,   151,    55,
      58,   263,    21,    22,    98,    69,    10,     4,   152,     6,
     154,   155,    63,    32,     4,   266,     6,    68,   111,   208,
     113,   188,    73,    81,    82,    76,     9,   194,   217,   218,
     116,    95,   176,    52,    31,   179,     0,    41,    21,    22,
      73,     3,    93,   101,    62,    49,    73,   351,   192,   116,
     117,    84,   303,   197,   305,   317,   307,    84,   309,    96,
      63,   228,   229,   117,   208,    68,   117,   211,   235,   115,
      73,   322,   323,    76,   263,    79,    17,    18,    19,     4,
     342,     6,   116,   117,    88,    72,   237,     5,    92,    82,
      93,   258,     9,   237,   356,    31,    21,    22,   242,   103,
      95,   245,    73,   354,    21,    22,    31,   358,   252,   360,
     115,   106,   108,    84,   116,   117,    21,    22,   116,   117,
     115,     7,   266,   385,     7,   387,   293,   389,   317,   391,
     116,   117,   116,   117,   116,   117,   116,   117,   115,   283,
      37,    38,   116,   117,    71,   334,   116,   117,   115,   316,
       5,   413,   115,   342,   416,    71,   418,   116,   117,   303,
       5,   305,    62,   307,     1,   309,     3,   356,   116,   117,
     116,   117,   116,   117,   116,   117,   115,   428,   322,   323,
     117,   348,   116,   117,   116,   117,   109,   115,     7,     7,
     334,    11,    95,    30,   115,   446,   385,     8,   387,   450,
     389,   115,   391,     9,    20,   116,    62,    44,    45,   116,
     354,    64,   116,    50,   358,   382,   360,   116,    55,   115,
      74,   115,    74,   105,   413,   106,   393,   416,     7,   418,
     374,    73,    69,     7,    39,    66,   115,   381,   115,     5,
      27,   117,   109,    79,   115,    74,    79,     4,     5,     6,
       7,   115,    89,    87,   115,    87,    73,    83,    95,    73,
      73,   405,   406,    35,    21,    22,    23,   104,   115,   117,
      85,   438,   109,    77,   111,    39,    33,    34,     5,   423,
     115,   425,     7,    40,   428,    39,   116,   115,    80,    46,
      47,    48,    92,   117,   115,    86,    53,    54,     7,   161,
      57,   286,   446,    60,   406,   425,   450,   192,   102,   247,
     103,   217,   245,    70,    71,   438,   218,   129,   162,    99,
     351,    78,    79,   133,   372,    -1,    -1,     4,     5,     6,
       7,   138,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      97,    -1,    99,   100,    21,    22,    23,    -1,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,    33,    34,   115,   116,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      47,    48,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    -1,    -1,    -1,    -1,     4,     5,     6,
       7,    -1,    -1,    10,    91,    -1,    -1,    -1,    -1,    -1,
      97,    -1,    99,   100,    21,    22,    -1,    -1,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,    33,    34,   115,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      47,    48,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    -1,    -1,    -1,    -1,     4,     5,     6,
       7,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      97,    -1,    99,   100,    21,    22,    -1,    -1,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,    33,    34,   115,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      47,    48,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    -1,    -1,    -1,    -1,     4,     5,     6,
       7,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      97,    -1,    99,   100,    21,    22,    -1,    -1,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,    33,    34,   115,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      47,    48,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    -1,    -1,    -1,    -1,     4,     5,     6,
       7,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      97,    -1,    99,   100,    21,    22,    -1,    -1,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,    33,    34,   115,    -1,
      -1,    -1,     5,    40,     7,    -1,    -1,    -1,    -1,    46,
      47,    48,    -1,    -1,    -1,    -1,    -1,    54,    21,    22,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    70,    71,    -1,    -1,    40,    -1,    -1,
      -1,    78,    79,    46,    47,    48,    -1,    -1,    -1,    -1,
      -1,    54,    -1,    -1,    91,    -1,    -1,    60,    -1,    -1,
      97,    -1,    99,   100,    -1,    -1,    -1,    70,    71,    -1,
     107,    -1,    -1,   110,    -1,    78,    79,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    91,    -1,
      -1,    -1,    40,    -1,    97,    -1,    99,   100,    46,    47,
      48,    -1,    -1,    -1,   107,    -1,    54,   110,    -1,    -1,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    91,    21,    22,    -1,    -1,    -1,    97,
      -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,    -1,   110
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,    30,    44,    45,    50,    55,    69,    89,
      95,   104,   109,   111,   120,   121,   122,   123,   124,   125,
     126,   147,   148,   151,   152,   155,   156,   159,   206,   207,
      98,     4,     6,   204,    67,    98,    62,    98,    72,    29,
      53,   160,   204,   157,   158,   172,   204,     0,   111,   113,
      69,   159,   111,   114,     3,   204,    62,   204,   204,   204,
     204,   204,     5,     7,    21,    22,    23,    33,    34,    40,
      46,    47,    48,    54,    57,    60,    70,    71,    78,    79,
      91,    97,    99,   100,   107,   110,   115,   129,   161,   162,
     163,   188,   189,   190,   191,   192,   193,   195,   197,   200,
     204,    96,   117,    31,   115,    72,    28,    55,     5,    82,
     115,   108,   182,   183,   106,   115,   159,     7,     7,   115,
      71,   115,     5,   115,    71,   188,     5,    62,   164,   117,
      21,    22,    31,   201,   204,    23,    24,   118,   202,    22,
     191,    27,   115,   153,   154,   204,   158,   115,   168,   203,
     204,   204,    42,    43,    42,    43,   109,   149,   204,   127,
     128,   204,    10,   115,   184,   185,   186,   187,   188,   197,
     115,   203,     7,   204,     7,   116,   168,   170,   173,   192,
     204,   182,   163,   204,   189,   190,   204,    23,    53,   116,
     188,   194,   117,   182,    11,   159,   116,   117,   159,   127,
      41,    61,    88,   103,   133,   204,   204,   115,   115,   136,
     116,   117,    71,   129,   187,   184,   188,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,   199,
      20,   195,   196,   116,   116,    62,   116,    31,   171,   172,
      63,    68,    73,    76,    93,   117,   165,   166,   167,   171,
      36,   102,   169,    64,   174,   116,   194,   116,   117,   154,
     188,   116,   204,   115,    74,    74,   115,    51,    56,   150,
     197,   198,   204,   105,   133,   134,   135,   127,    10,    41,
      49,    79,    88,    92,   103,   130,   131,   132,   116,   185,
     186,    17,    18,    19,   188,   188,    10,    79,   116,   117,
     106,   188,   172,    73,    84,    73,   203,    73,    84,    73,
      84,   170,   167,     7,     7,   171,    39,    66,   175,   116,
     188,   184,   115,   115,   203,     5,    58,    81,    82,   101,
     205,   116,   117,   116,   117,    37,    38,   145,   117,   109,
     137,    79,   115,   195,    74,   204,   130,   188,     9,    79,
     195,   115,   116,   203,    73,   203,    82,   203,    73,   203,
      73,    87,    87,   194,   184,    83,   176,   116,   203,   203,
     116,    51,    56,   197,   115,   146,   133,    35,    85,   138,
     184,   115,     9,   188,   196,    82,   203,    82,   184,    82,
     203,    82,   203,    39,    77,   177,   116,   116,     5,   205,
     140,   141,   142,   143,   144,   204,   115,    39,   116,   204,
     188,   116,   184,    82,   184,   184,    82,   184,    82,   178,
     179,   188,     7,    92,   116,   117,     7,    29,   115,   204,
     140,    65,    90,   139,   116,   184,   184,   184,   117,    32,
      52,   180,   204,   141,   203,   116,   115,   179,    80,   181,
     115,   116,   203,    59,    75,   203,   116,   116,    86,     7
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   119,   120,   120,   120,   120,   120,   120,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   121,   122,   123,
     123,   123,   123,   124,   125,   126,   127,   128,   128,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   130,   130,   130,
     130,   130,   130,   130,   131,   131,   132,   132,   133,   133,
     133,   133,   134,   134,   135,   135,   136,   136,   137,   137,
     138,   138,   139,   139,   140,   140,   141,   141,   141,   142,
     142,   143,   144,   145,   145,   146,   146,   147,   147,   147,
     147,   148,   149,   149,   150,   150,   150,   150,   151,   152,
     153,   153,   154,   155,   155,   156,   157,   157,   158,   159,
     160,   160,   160,   161,   161,   162,   162,   163,   163,   163,
     164,   165,   165,   166,   166,   167,   167,   167,   167,   167,
     167,   167,   167,   168,   169,   169,   169,   170,   170,   170,
     170,   170,   171,   171,   172,   172,   173,   173,   174,   174,
     175,   175,   176,   176,   177,   177,   178,   178,   179,   180,
     180,   180,   181,   181,   181,   182,   182,   183,   184,   184,
     185,   185,   186,   186,   187,   187,   187,   187,   187,   187,
     188,   188,   189,   189,   190,   190,   191,   191,   191,   191,
     191,   192,   192,   192,   192,   193,   194,   194,   195,   195,
     195,   195,   195,   195,   195,   196,   196,   197,   197,   198,
     198,   199,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   200,   201,   201,   202,   202,   202,   203,   203,   204,
     204,   205,   205,   205,   205,   206,   207,   207
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
       3,     1,     3,     1,     2,     1,     1,     1,     1,     1,
       3,     3,     4,     4,     5,     6,     1,     3,     1,     1,
       2,     2,     1,     2,     2,     1,     3,     1,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     2,     2,     0
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
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1800 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_NAME  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1810 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1820 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1830 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 7: /* TOKEN_UNSIGNED_NUMVAL  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 1840 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 121: /* sql_statement  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1850 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 122: /* quit_statement  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 1860 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 123: /* alter_table_statement  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1870 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 124: /* create_table_statement  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 1880 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 125: /* create_index_statement  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1890 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 126: /* drop_table_statement  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 1900 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 127: /* column_def  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 1910 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 128: /* column_def_commalist  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 1920 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 129: /* data_type  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 1930 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 130: /* column_constraint_def  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 1940 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 131: /* column_constraint_def_list  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1950 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 132: /* opt_column_constraint_def_list  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1960 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 136: /* opt_column_list  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 1970 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 137: /* opt_block_properties  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).block_properties_) != nullptr) {
    delete ((*yyvaluep).block_properties_);
  }
}
#line 1980 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 138: /* opt_partition_clause  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).partition_clause_) != nullptr) {
    delete ((*yyvaluep).partition_clause_);
  }
}
#line 1990 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 139: /* partition_type  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2000 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 140: /* key_value_list  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2010 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 141: /* key_value  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 2020 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 142: /* key_string_value  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 2030 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 143: /* key_string_list  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 2040 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 144: /* key_integer_value  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 2050 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 145: /* index_type  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2060 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 146: /* opt_index_properties  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2070 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 147: /* insert_statement  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 2080 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 148: /* copy_from_statement  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_statement_) != nullptr) {
    delete ((*yyvaluep).copy_from_statement_);
  }
}
#line 2090 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 149: /* opt_copy_from_params  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2100 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 150: /* copy_from_params  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2110 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 151: /* update_statement  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2120 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 152: /* delete_statement  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2130 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 153: /* assignment_list  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2140 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* assignment_item  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2150 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* select_statement  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_statement_) != nullptr) {
    delete ((*yyvaluep).select_statement_);
  }
}
#line 2160 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* with_clause  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2170 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* with_list  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2180 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* with_list_element  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2190 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* select_query  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2200 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* selection  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2210 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* selection_item_commalist  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2220 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 163: /* selection_item  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2230 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 164: /* from_clause  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2240 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* subquery_expression  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2250 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* opt_sample_clause  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_sample_clause_) != nullptr) {
    delete ((*yyvaluep).opt_sample_clause_);
  }
}
#line 2260 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* table_reference  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2270 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* table_reference_signature  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2280 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* table_reference_signature_primary  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2290 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* table_reference_commalist  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2300 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* opt_group_by_clause  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2310 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* opt_having_clause  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2320 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* opt_order_by_clause  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2330 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* opt_limit_clause  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2340 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* order_commalist  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2350 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* order_item  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2360 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* opt_order_direction  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2370 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* opt_nulls_first  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2380 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* opt_where_clause  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2390 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* where_clause  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2400 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* or_expression  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2410 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* and_expression  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2420 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* not_expression  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2430 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* predicate_expression_base  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2440 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* add_expression  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2450 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* multiply_expression  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2460 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 190: /* unary_expression  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2470 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 191: /* expression_base  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2480 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 192: /* function_call  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2490 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 193: /* extract_function  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2500 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 194: /* expression_list  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2510 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 195: /* literal_value  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2520 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 196: /* literal_value_commalist  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2530 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 197: /* attribute_ref  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2540 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 198: /* attribute_ref_list  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2550 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 199: /* comparison_operation  */
#line 525 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2556 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 200: /* unary_operation  */
#line 526 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2562 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 201: /* add_operation  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2568 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 202: /* multiply_operation  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2574 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 203: /* name_commalist  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 2584 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 204: /* any_name  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2594 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 205: /* boolean_value  */
#line 524 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2600 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 206: /* command  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_) != nullptr) {
    delete ((*yyvaluep).command_);
  }
}
#line 2610 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 207: /* command_argument_list  */
#line 529 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_argument_list_) != nullptr) {
    delete ((*yyvaluep).command_argument_list_);
  }
}
#line 2620 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 538 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2917 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 542 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2926 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 546 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 2935 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 5:
#line 550 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 2944 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 554 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    YYABORT;
  }
#line 2952 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 557 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 2961 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 564 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 2969 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 9:
#line 567 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].copy_from_statement_);
  }
#line 2977 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 10:
#line 570 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 2985 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 11:
#line 573 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 2993 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 12:
#line 576 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 3001 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 13:
#line 579 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 3009 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 14:
#line 582 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 3017 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 15:
#line 585 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 3025 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 16:
#line 588 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].select_statement_);
  }
#line 3033 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 17:
#line 591 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 3041 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 18:
#line 597 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3049 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 19:
#line 603 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3061 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 20:
#line 610 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3072 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 616 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3084 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 623 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3096 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 632 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].attribute_definition_list_), (yyvsp[-1].block_properties_), (yyvsp[0].partition_clause_));
  }
#line 3104 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 637 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].key_value_list_)) {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_), (yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].key_value_list_));
    } else {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_));
    }
  }
#line 3116 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 646 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 3124 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 651 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 3132 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 656 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3141 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 660 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3150 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 666 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 3160 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 671 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3168 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 674 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3176 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 677 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3186 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 682 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3194 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 685 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3202 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 688 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3210 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 691 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3218 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 694 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3226 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 697 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3234 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 700 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3242 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 703 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3250 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 706 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3258 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 709 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3275 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 721 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3283 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 724 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3291 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 727 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3314 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 745 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3337 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 765 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3345 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 768 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3353 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 771 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3363 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 776 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3373 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 781 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3384 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 787 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3395 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 793 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3407 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 802 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3416 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 806 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3425 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 812 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3433 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 815 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3441 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 820 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3451 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 825 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3461 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 830 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3473 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 837 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3483 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 844 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3492 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 848 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3501 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 854 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3509 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 857 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = $1; */
  }
#line 3517 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 862 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = nullptr;
  }
#line 3525 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 865 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-1].attribute_list_);
  }
#line 3533 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 870 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = nullptr;
  }
#line 3541 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 873 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].key_value_list_));
  }
#line 3549 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 878 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.partition_clause_) = nullptr;
  }
#line 3557 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 881 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3579 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 900 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::PartitionSchemeHeader::PartitionType::kHash));
  }
#line 3588 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 904 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::PartitionSchemeHeader::PartitionType::kRange));
  }
#line 3597 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 910 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3606 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 914 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3615 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 920 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 3623 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 923 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 3631 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 926 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 3639 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 931 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 3647 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 934 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_),
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 3657 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 941 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 3665 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 946 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 3679 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 957 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
#line 3688 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 961 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
#line 3697 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 967 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 3705 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 970 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 3713 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 976 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 3726 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 984 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertTuple((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 3734 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 987 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-3]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), nullptr);
  }
#line 3742 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 990 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-4]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), (yyvsp[-4].with_list_));
  }
#line 3750 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 996 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_statement_) = new quickstep::ParseStatementCopyFrom((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].copy_from_params_));
  }
#line 3758 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 1001 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = nullptr;
  }
#line 3766 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 1004 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-1].copy_from_params_);
  }
#line 3774 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 1009 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3783 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 1013 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3792 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 1017 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3801 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 1021 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3810 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 1027 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 3818 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 1032 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 3826 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 1037 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3835 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 1041 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3844 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 1047 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 3852 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 1053 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].select_query_), nullptr);
  }
#line 3860 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 1056 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].select_query_), (yyvsp[-1].with_list_));
  }
#line 3868 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 1061 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 3876 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 1066 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3885 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 1070 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3894 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 1076 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 3903 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 1083 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].selection_), (yyvsp[-5].table_reference_list_), (yyvsp[-4].predicate_), (yyvsp[-3].opt_group_by_clause_), (yyvsp[-2].opt_having_clause_), (yyvsp[-1].opt_order_by_clause_), (yyvsp[0].opt_limit_clause_));
  }
#line 3911 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 1088 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3919 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 1091 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "ALL in selection");
    YYERROR;
  }
#line 3928 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 1095 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "DISTINCT in selection");
    YYERROR;
  }
#line 3937 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 1101 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3945 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 1104 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 3953 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 1109 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3962 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 1113 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3971 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 1119 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 3979 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 1122 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 3987 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 1125 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 3995 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 1130 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-1].table_reference_list_);
  }
#line 4003 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 1135 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 4011 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 1138 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4020 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 1144 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-1]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4029 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 1148 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4038 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 1154 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4049 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 1160 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4060 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 1166 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4071 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 1172 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4082 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 1178 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4093 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 1184 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4104 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 1190 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4115 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 1196 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4126 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 1204 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].select_query_));
  }
#line 4134 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 1209 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = NULL;
  }
#line 4142 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 1212 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, true, (yyvsp[-1].numeric_literal_value_));
  }
#line 4150 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 1215 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].numeric_literal_value_));
  }
#line 4158 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 1220 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4167 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 1224 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].opt_sample_clause_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4176 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 1228 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].opt_sample_clause_));
  }
#line 4184 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 1231 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4193 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 1235 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].function_call_));
  }
#line 4201 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 1240 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4209 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 1243 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4217 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 1248 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4225 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 1251 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4233 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 1256 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4242 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 1260 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4251 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 1266 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 4259 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 1269 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4267 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 1274 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4275 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 1277 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4283 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 1282 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4291 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 1285 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4299 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 1290 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4307 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 1293 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4329 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 1312 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4338 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 1316 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4347 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 1322 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 4357 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 1329 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4365 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 1332 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4373 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 1335 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4381 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 1340 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4389 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 1343 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4397 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 1346 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4405 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 1352 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 4413 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 1355 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4421 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 1360 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4429 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 1365 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4443 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 1374 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4451 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 1379 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4465 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 1388 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4473 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 1393 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4481 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 1396 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4489 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 1401 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4497 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 1404 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 4507 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 1409 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4518 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 1415 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4529 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 1421 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4537 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 1424 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 4545 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 1430 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4553 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 1433 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4561 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 1438 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4569 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 1441 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4577 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 1446 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].unary_operation_), (yyvsp[0].expression_));
  }
#line 4585 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 1449 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4593 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1454 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 4601 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 1457 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 4609 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 1460 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 4617 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 1463 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4625 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 1466 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 4633 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 1471 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 4642 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 1475 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 4651 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 1479 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, false, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4659 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 1482 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-4]).first_line, (yylsp[-4]).first_column, true, (yyvsp[-4].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4667 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 1487 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseExtractFunction((yylsp[-5]).first_line, (yylsp[-5]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].expression_));
  }
#line 4675 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 1492 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4684 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 1496 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4693 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 1502 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4701 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 1505 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4709 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1508 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4717 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1511 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4734 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1523 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 4743 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1527 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4767 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1546 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4785 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1561 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4794 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1565 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4803 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1571 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4811 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1574 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 4819 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1579 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = new quickstep::PtrList<quickstep::ParseAttribute>();
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 4828 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1583 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-2].attribute_list_);
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 4837 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1590 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 4845 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1593 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 4853 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1596 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 4861 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1599 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 4869 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1602 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 4877 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1605 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 4885 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1608 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLike);
  }
#line 4893 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1611 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotLike);
  }
#line 4901 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1614 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kRegexMatch);
  }
#line 4909 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1617 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotRegexMatch);
  }
#line 4917 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1622 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
#line 4931 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1633 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
#line 4939 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1636 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
#line 4947 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1641 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kModulo);
  }
#line 4955 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1644 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
#line 4963 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1647 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
#line 4971 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1653 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4980 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1657 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4989 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1663 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 4997 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1666 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5008 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1674 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5016 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1677 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5024 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1680 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5032 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1683 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5040 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1689 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.command_) = new quickstep::ParseCommand((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].command_argument_list_));
  }
#line 5048 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1694 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    quickstep::PtrVector<quickstep::ParseString> *argument_list = (yyvsp[-1].command_argument_list_);
    argument_list->push_back((yyvsp[0].string_value_));
    (yyval.command_argument_list_) = argument_list;
  }
#line 5058 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1699 "../SqlParser.ypp" /* yacc.c:1661  */
    { /* Epsilon, an empy match. */
    (yyval.command_argument_list_) = new quickstep::PtrVector<quickstep::ParseString>();
  }
#line 5066 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;


#line 5070 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 1703 "../SqlParser.ypp" /* yacc.c:1906  */


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
