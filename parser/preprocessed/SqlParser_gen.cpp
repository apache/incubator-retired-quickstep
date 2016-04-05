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
    TOKEN_FALSE = 307,
    TOKEN_FIRST = 308,
    TOKEN_FLOAT = 309,
    TOKEN_FOREIGN = 310,
    TOKEN_FROM = 311,
    TOKEN_FULL = 312,
    TOKEN_GROUP = 313,
    TOKEN_HASH = 314,
    TOKEN_HAVING = 315,
    TOKEN_INDEX = 316,
    TOKEN_INNER = 317,
    TOKEN_INSERT = 318,
    TOKEN_INTEGER = 319,
    TOKEN_INTERVAL = 320,
    TOKEN_INTO = 321,
    TOKEN_JOIN = 322,
    TOKEN_KEY = 323,
    TOKEN_LAST = 324,
    TOKEN_LEFT = 325,
    TOKEN_LIMIT = 326,
    TOKEN_LONG = 327,
    TOKEN_NULL = 328,
    TOKEN_NULLS = 329,
    TOKEN_OFF = 330,
    TOKEN_ON = 331,
    TOKEN_ORDER = 332,
    TOKEN_OUTER = 333,
    TOKEN_PARTITION = 334,
    TOKEN_PARTITIONS = 335,
    TOKEN_PERCENT = 336,
    TOKEN_PRIMARY = 337,
    TOKEN_QUIT = 338,
    TOKEN_RANGE = 339,
    TOKEN_REAL = 340,
    TOKEN_REFERENCES = 341,
    TOKEN_RIGHT = 342,
    TOKEN_ROW_DELIMITER = 343,
    TOKEN_SELECT = 344,
    TOKEN_SET = 345,
    TOKEN_SMALLINT = 346,
    TOKEN_TABLE = 347,
    TOKEN_TIME = 348,
    TOKEN_TIMESTAMP = 349,
    TOKEN_TRUE = 350,
    TOKEN_TUPLESAMPLE = 351,
    TOKEN_UNIQUE = 352,
    TOKEN_UPDATE = 353,
    TOKEN_USING = 354,
    TOKEN_VALUES = 355,
    TOKEN_VARCHAR = 356,
    TOKEN_WHERE = 357,
    TOKEN_WITH = 358,
    TOKEN_YEARMONTH = 359,
    TOKEN_EOF = 360,
    TOKEN_LEX_ERROR = 361
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

#line 381 "SqlParser_gen.cpp" /* yacc.c:355  */
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

#line 416 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYLAST   813

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  118
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  88
/* YYNRULES -- Number of rules.  */
#define YYNRULES  235
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  453

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   361

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     113,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   117,     2,     2,
     114,   115,    23,    21,   116,    22,    27,    24,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   112,
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
     110,   111
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   536,   536,   540,   544,   548,   552,   555,   562,   565,
     568,   571,   574,   577,   580,   583,   586,   589,   595,   601,
     608,   614,   621,   630,   635,   644,   649,   654,   658,   664,
     669,   672,   675,   680,   683,   686,   689,   692,   695,   698,
     701,   704,   707,   719,   722,   725,   743,   763,   766,   769,
     774,   779,   785,   791,   800,   804,   810,   813,   818,   823,
     828,   835,   842,   846,   852,   855,   860,   863,   868,   871,
     876,   879,   898,   902,   908,   912,   918,   921,   924,   929,
     932,   939,   944,   955,   959,   965,   968,   974,   982,   985,
     988,   994,   999,  1002,  1007,  1011,  1015,  1019,  1025,  1030,
    1035,  1039,  1045,  1051,  1054,  1059,  1064,  1068,  1074,  1080,
    1086,  1089,  1093,  1099,  1102,  1107,  1111,  1117,  1120,  1123,
    1128,  1133,  1136,  1142,  1146,  1152,  1158,  1164,  1170,  1176,
    1182,  1188,  1194,  1202,  1207,  1210,  1213,  1218,  1222,  1226,
    1229,  1233,  1238,  1241,  1246,  1249,  1254,  1258,  1264,  1267,
    1272,  1275,  1280,  1283,  1288,  1291,  1310,  1314,  1320,  1327,
    1330,  1333,  1338,  1341,  1344,  1350,  1353,  1358,  1363,  1372,
    1377,  1386,  1391,  1394,  1399,  1402,  1407,  1413,  1419,  1422,
    1428,  1431,  1436,  1439,  1444,  1447,  1452,  1455,  1458,  1461,
    1466,  1470,  1474,  1477,  1482,  1486,  1492,  1495,  1498,  1501,
    1513,  1517,  1536,  1551,  1555,  1561,  1564,  1569,  1573,  1580,
    1583,  1586,  1589,  1592,  1595,  1598,  1601,  1604,  1607,  1612,
    1623,  1626,  1631,  1634,  1637,  1643,  1647,  1653,  1656,  1664,
    1667,  1670,  1673,  1679,  1684,  1689
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
  "TOKEN_ESCAPE_STRINGS", "TOKEN_FALSE", "TOKEN_FIRST", "TOKEN_FLOAT",
  "TOKEN_FOREIGN", "TOKEN_FROM", "TOKEN_FULL", "TOKEN_GROUP", "TOKEN_HASH",
  "TOKEN_HAVING", "TOKEN_INDEX", "TOKEN_INNER", "TOKEN_INSERT",
  "TOKEN_INTEGER", "TOKEN_INTERVAL", "TOKEN_INTO", "TOKEN_JOIN",
  "TOKEN_KEY", "TOKEN_LAST", "TOKEN_LEFT", "TOKEN_LIMIT", "TOKEN_LONG",
  "TOKEN_NULL", "TOKEN_NULLS", "TOKEN_OFF", "TOKEN_ON", "TOKEN_ORDER",
  "TOKEN_OUTER", "TOKEN_PARTITION", "TOKEN_PARTITIONS", "TOKEN_PERCENT",
  "TOKEN_PRIMARY", "TOKEN_QUIT", "TOKEN_RANGE", "TOKEN_REAL",
  "TOKEN_REFERENCES", "TOKEN_RIGHT", "TOKEN_ROW_DELIMITER", "TOKEN_SELECT",
  "TOKEN_SET", "TOKEN_SMALLINT", "TOKEN_TABLE", "TOKEN_TIME",
  "TOKEN_TIMESTAMP", "TOKEN_TRUE", "TOKEN_TUPLESAMPLE", "TOKEN_UNIQUE",
  "TOKEN_UPDATE", "TOKEN_USING", "TOKEN_VALUES", "TOKEN_VARCHAR",
  "TOKEN_WHERE", "TOKEN_WITH", "TOKEN_YEARMONTH", "TOKEN_EOF",
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
  "select_statement", "with_clause", "with_list", "with_list_element",
  "select_query", "opt_all_distinct", "selection",
  "selection_item_commalist", "selection_item", "from_clause",
  "opt_join_chain", "join_chain", "join", "subquery_expression",
  "opt_sample_clause", "table_reference", "table_reference_signature",
  "table_reference_signature_primary", "table_reference_commalist",
  "opt_group_by_clause", "opt_having_clause", "opt_order_by_clause",
  "opt_limit_clause", "order_commalist", "order_item",
  "opt_order_direction", "opt_nulls_first", "opt_where_clause",
  "where_clause", "or_expression", "and_expression", "not_expression",
  "predicate_expression_base", "add_expression", "multiply_expression",
  "unary_expression", "expression_base", "function_call",
  "expression_list", "literal_value", "literal_value_commalist",
  "attribute_ref", "attribute_ref_list", "comparison_operation",
  "unary_operation", "add_operation", "multiply_operation",
  "name_commalist", "any_name", "boolean_value", "command",
  "command_argument_list", YY_NULLPTR
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
     360,   361,    59,    10,    40,    41,    44,    37
};
# endif

#define YYPACT_NINF -198

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-198)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     316,  -198,  -198,    72,    62,    13,    22,    76,   108,  -198,
      24,    62,    62,  -198,    90,   107,  -198,  -198,  -198,  -198,
    -198,  -198,  -198,  -198,  -198,  -198,    65,  -198,   124,   178,
      62,  -198,  -198,   146,    62,    62,    62,    62,    62,  -198,
    -198,    15,   128,   111,  -198,   214,   135,  -198,  -198,  -198,
     158,  -198,  -198,  -198,  -198,   115,   247,   173,   142,   162,
    -198,    50,  -198,  -198,   266,   275,  -198,  -198,  -198,   170,
    -198,   218,  -198,  -198,  -198,  -198,   287,  -198,  -198,  -198,
    -198,  -198,  -198,   180,   228,   500,   298,   250,   202,  -198,
     210,    -9,  -198,  -198,  -198,  -198,  -198,   579,     1,    62,
      62,   212,    62,    62,   169,   238,   220,    62,    62,   454,
    -198,  -198,   216,    62,  -198,  -198,  -198,   328,  -198,  -198,
     335,  -198,     2,  -198,    12,   162,   500,  -198,  -198,    62,
     500,  -198,  -198,  -198,  -198,   500,   275,  -198,    62,   375,
      69,  -198,   341,  -198,   263,  -198,   171,  -198,   263,    62,
      78,    62,    62,   245,  -198,   248,  -198,   175,   658,   625,
     454,   357,   360,  -198,  -198,   774,   358,   704,   186,   268,
     270,  -198,   152,  -198,   126,   152,   -23,   323,  -198,  -198,
      -9,  -198,  -198,   272,   500,  -198,   284,   192,    62,  -198,
     500,   273,  -198,    62,  -198,  -198,   276,   318,   319,   285,
    -198,  -198,  -198,    20,    62,   297,    78,    62,  -198,   198,
    -198,     3,   249,   454,   454,   260,  -198,  -198,  -198,  -198,
    -198,  -198,  -198,  -198,   500,   500,    19,  -198,   194,   295,
    -198,  -198,    62,  -198,  -198,   -18,   317,    62,   123,   130,
      12,  -198,   163,  -198,  -198,   395,   396,   152,   366,   342,
    -198,   200,  -198,   500,  -198,   284,  -198,  -198,   454,   292,
     299,    62,   406,   -30,   205,  -198,   207,   389,   296,  -198,
     302,   312,  -198,   347,   313,   704,  -198,   359,    62,  -198,
    -198,   198,  -198,  -198,   360,  -198,  -198,  -198,   500,    51,
     284,   352,  -198,  -198,   704,   322,  -198,    62,   361,    62,
     -36,    62,   365,    62,   368,  -198,  -198,   345,   355,  -198,
     500,   454,   356,  -198,   284,    17,    62,    62,   223,  -198,
    -198,  -198,  -198,  -198,  -198,  -198,   148,  -198,    62,  -198,
    -198,   329,    78,   407,   362,  -198,   454,  -198,  -198,   333,
    -198,   141,   500,  -198,  -198,   704,   -35,    62,   -29,   454,
     -22,    62,   -14,    62,  -198,  -198,   332,   357,   410,   374,
    -198,   225,   229,  -198,   446,   -30,  -198,    62,  -198,  -198,
     340,   416,  -198,    27,    62,   500,   284,   235,   454,    56,
     454,   357,   454,    58,   454,    59,   500,   449,  -198,   371,
    -198,  -198,  -198,   239,  -198,  -198,  -198,  -198,    35,    62,
      -7,  -198,   348,   284,  -198,   357,   454,   357,   357,   454,
     357,   454,   350,  -198,   145,  -198,    62,  -198,    62,  -198,
    -198,    62,  -198,   252,  -198,  -198,   343,  -198,   357,   357,
     357,   500,  -198,  -198,   388,   354,  -198,   257,  -198,    62,
    -198,    83,  -198,    62,  -198,   259,  -198,  -198,   261,   384,
    -198,   463,  -198
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     6,   235,     0,     0,     0,     0,     0,     0,    18,
     110,     0,     0,     7,     0,     0,    15,     8,    10,    11,
      13,    14,     9,    17,    12,    16,     0,   103,     0,   233,
       0,   227,   228,     0,     0,     0,     0,     0,     0,   111,
     112,     0,     0,   105,   106,     0,   144,     1,     3,     2,
       0,   104,     5,     4,   234,     0,     0,     0,     0,   165,
      25,     0,   200,   197,     0,   219,   113,    40,    29,     0,
      30,    31,    34,    36,    37,    39,     0,    41,   196,    35,
      38,    32,    33,     0,     0,     0,     0,     0,   114,   115,
     119,   181,   183,   185,   188,   187,   186,     0,   205,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
      99,   166,     0,     0,    89,   198,   199,     0,    43,   201,
       0,    44,     0,   202,     0,   165,     0,   220,   221,     0,
       0,   118,   223,   224,   222,     0,     0,   184,     0,     0,
     165,   101,     0,   107,     0,   108,     0,   225,     0,     0,
       0,     0,     0,     0,    91,    66,    27,     0,     0,     0,
       0,   167,   169,   171,   173,     0,   186,     0,     0,     0,
       0,   189,     0,   146,   121,   141,   134,   148,   116,   117,
     180,   182,   206,     0,     0,   190,   194,     0,     0,    98,
       0,     0,   145,     0,    90,    19,     0,     0,     0,     0,
      20,    21,    22,     0,     0,     0,    64,     0,    42,    56,
     172,     0,     0,     0,     0,     0,   209,   211,   212,   213,
     214,   210,   215,   217,     0,     0,     0,   203,     0,     0,
      45,    46,     0,   137,   142,     0,     0,     0,     0,     0,
       0,   120,   122,   124,   140,     0,     0,   139,     0,   150,
     191,     0,   192,     0,   100,   102,   133,   226,     0,     0,
       0,     0,     0,     0,     0,   207,     0,   205,     0,    63,
      65,    68,    28,     0,     0,     0,    47,     0,     0,    49,
      55,    57,    26,   179,   168,   170,   216,   218,     0,     0,
     178,     0,   177,    88,     0,     0,   143,     0,     0,     0,
       0,     0,     0,     0,     0,   147,   123,     0,     0,   138,
       0,     0,   152,   193,   195,     0,     0,     0,     0,    94,
     231,   232,   230,   229,    95,    93,     0,    67,     0,    83,
      84,    85,     0,     0,    70,    48,     0,    51,    50,     0,
      54,     0,     0,   176,   204,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   135,   136,   149,   151,     0,   154,
      61,     0,     0,    58,     0,     0,   208,     0,    24,    62,
       0,     0,    23,     0,     0,     0,   174,     0,     0,     0,
       0,   126,     0,     0,     0,     0,     0,     0,   109,     0,
      59,    96,    97,     0,    74,    76,    77,    78,     0,     0,
       0,    52,     0,   175,    87,   132,     0,   125,   128,     0,
     130,     0,   153,   156,   159,   155,     0,    86,     0,    82,
      80,     0,    79,     0,    72,    73,     0,    53,   131,   127,
     129,     0,   160,   161,   162,     0,    75,     0,    69,     0,
     157,     0,   158,     0,    81,     0,   163,   164,     0,     0,
      60,     0,    71
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -198,  -198,  -198,  -198,  -198,  -198,  -198,  -198,  -132,  -198,
     314,   196,  -198,  -198,  -197,  -198,  -198,  -198,  -198,  -198,
    -198,    79,    61,  -198,  -198,  -198,  -198,  -198,  -198,  -198,
    -198,  -198,  -198,  -198,  -198,   294,  -198,  -198,  -198,   380,
     -21,  -198,  -198,  -198,   367,  -198,  -198,  -198,   241,   385,
    -198,   251,  -131,   -11,  -198,  -198,  -198,  -198,  -198,  -198,
      54,  -198,  -198,   -82,  -198,  -158,   279,   281,   337,   -38,
     369,   363,   400,  -112,  -174,  -163,   164,   -53,  -198,  -198,
    -198,  -198,  -198,  -107,    -4,   138,  -198,  -198
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    14,    15,    16,    17,    18,    19,    20,   156,   157,
      86,   280,   281,   282,   200,   270,   271,   205,   334,   372,
     426,   393,   394,   395,   396,   397,   331,   368,    21,    22,
     154,   264,    23,    24,   140,   141,    25,    26,    43,    44,
      27,    41,    87,    88,    89,   125,   241,   242,   243,   172,
     247,   173,   233,   234,   174,   249,   312,   359,   388,   412,
     413,   434,   442,   110,   111,   161,   162,   163,   164,   165,
      91,    92,    93,    94,   187,    95,   228,    96,   266,   225,
      97,   130,   135,   146,    98,   324,    28,    29
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
      33,    45,   211,    90,   227,    51,   168,    42,    46,   269,
     251,   213,   175,   245,   132,   133,    31,   195,    32,    31,
      62,    32,    63,   127,   128,   213,    55,   320,   138,   291,
      57,    58,    59,    60,    61,   213,    64,    65,    66,    31,
     114,    32,   419,   177,   244,   349,   378,   122,    67,    68,
     321,   322,   380,    39,   297,    69,   166,   424,   189,   382,
     342,    70,    71,    72,   420,   298,    31,   384,    32,    73,
     323,   262,   127,   128,    74,   272,   263,    40,   246,    34,
     193,   193,   425,    36,    75,    76,   131,   193,    90,    45,
      47,   139,    77,    78,   193,   142,    46,   292,   147,   148,
     315,   186,   193,   155,   158,    79,   166,   166,   134,   147,
      35,    80,   337,    81,    82,   139,   309,   171,   283,   196,
     176,    83,   212,   191,    84,   179,   144,   194,   175,    85,
     300,   344,   360,    50,   182,   369,   356,   406,   197,   409,
     411,   446,   401,   104,    10,   158,   186,   201,   202,   421,
     375,   265,   255,   357,   318,   112,    31,   447,    32,    10,
     166,   166,   127,   128,   113,   198,   127,   128,    46,    30,
     105,    46,   193,    37,   193,   193,   109,   432,   373,    38,
     199,    54,   227,   232,   142,   188,   289,   290,   235,   257,
     346,   381,   348,   236,   350,   301,   352,   433,   237,   364,
     267,   238,   303,   158,   365,   166,   302,    56,   273,   361,
     362,   149,   150,   304,    31,   314,    32,    48,   239,    49,
     405,   296,   407,    99,   408,   235,   410,   100,    46,   103,
     236,   127,   128,   147,    52,   237,   176,    53,   238,   274,
     379,   129,   240,    46,   383,   101,   385,   275,   428,   102,
     341,   429,   106,   430,   107,   239,   108,   147,   166,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   109,
     127,   128,   186,   115,   339,   366,   276,   286,   287,   288,
     151,   152,   116,   166,   117,   277,   192,   193,   118,   278,
     206,   207,   119,   147,   120,   147,   166,   147,   121,   147,
     279,   229,   193,   123,   376,   127,   128,   252,   253,   293,
     294,   124,   147,   147,   437,   313,   253,     1,   126,     2,
     325,   326,   327,   328,   267,   166,   144,   166,   153,   166,
     167,   166,   445,   329,   330,   169,   448,   403,   363,   193,
     389,   193,   170,   147,   390,   193,     3,   147,   414,   147,
     404,   294,   190,   166,   417,   418,   166,    10,   166,   203,
       4,     5,   204,   398,   171,   213,     6,   438,   418,   214,
     402,     7,   444,   193,   449,   193,   450,   193,   226,    31,
      62,    32,    63,   230,     8,   231,   248,   250,   256,   299,
     258,   259,   260,   414,   422,   398,    64,    65,   183,   261,
     295,   268,   307,   308,     9,   310,   316,   311,    67,    68,
      10,   319,   435,   317,   398,    69,   138,   147,   332,    11,
     333,    70,    71,    72,    12,   335,    13,   336,   184,    73,
     343,   354,   338,   347,    74,   147,   345,   351,   358,   147,
     353,   355,   370,   367,    75,    76,   371,   374,   253,   386,
     387,   391,    77,    78,   399,   400,   415,   439,    31,    62,
      32,    63,   416,   427,   159,    79,   431,   441,   443,   451,
     452,    80,   209,    81,    82,    64,    65,   340,   423,   436,
     143,    83,   254,   306,    84,   440,   145,    67,    68,    85,
     185,   305,   284,   178,    69,   285,   210,   137,   181,   180,
      70,    71,    72,   392,    31,    62,    32,    63,    73,   377,
       0,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,    64,    65,    75,    76,     0,     0,     0,     0,     0,
       0,    77,    78,    67,    68,     0,     0,     0,     0,     0,
      69,     0,     0,     0,    79,     0,    70,    71,    72,     0,
      80,     0,    81,    82,    73,     0,     0,     0,     0,    74,
      83,     0,     0,    84,     0,     0,     0,     0,   160,    75,
      76,     0,     0,     0,     0,     0,     0,    77,    78,     0,
       0,     0,     0,    31,    62,    32,    63,     0,     0,     0,
      79,     0,     0,     0,     0,     0,    80,     0,    81,    82,
      64,   136,     0,     0,     0,     0,    83,     0,     0,    84,
       0,     0,    67,    68,    85,     0,     0,     0,     0,    69,
       0,     0,     0,     0,     0,    70,    71,    72,     0,    31,
      62,    32,    63,    73,     0,     0,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,    64,    65,    75,    76,
       0,     0,     0,     0,     0,     0,    77,    78,    67,    68,
       0,     0,     0,     0,     0,    69,     0,     0,     0,    79,
       0,    70,    71,    72,     0,    80,     0,    81,    82,    73,
       0,     0,     0,     0,    74,    83,     0,     0,    84,     0,
       0,    67,    68,    85,    75,    76,     0,     0,    69,     0,
       0,     0,    77,    78,    70,    71,    72,     0,     0,    62,
       0,    63,    73,     0,     0,    79,     0,    74,     0,     0,
       0,    80,     0,    81,    82,    64,   136,    75,   208,     0,
       0,    83,     0,     0,    84,    77,     0,    67,    68,   160,
       0,     0,     0,     0,    69,     0,     0,     0,    79,     0,
      70,    71,    72,     0,    80,     0,    81,    82,    73,     0,
       0,     0,     0,    74,    83,     0,     0,    84,     0,     0,
       0,     0,     0,    75,    76,     0,     0,     0,     0,     0,
       0,    77,    78,     0,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,    79,   127,   128,     0,     0,     0,
      80,     0,    81,    82,     0,     0,     0,     0,     0,     0,
      83,     0,     0,    84
};

static const yytype_int16 yycheck[] =
{
       4,    12,   160,    41,   167,    26,   113,    11,    12,   206,
     184,     8,   124,    36,    23,    24,     4,   149,     6,     4,
       5,     6,     7,    21,    22,     8,    30,    57,    27,    10,
      34,    35,    36,    37,    38,     8,    21,    22,    23,     4,
      61,     6,     7,   125,   175,    81,    81,    85,    33,    34,
      80,    81,    81,    29,    72,    40,   109,    64,   140,    81,
       9,    46,    47,    48,    29,    83,     4,    81,     6,    54,
     100,    51,    21,    22,    59,   207,    56,    53,   101,    66,
     116,   116,    89,    61,    69,    70,    90,   116,   126,   100,
       0,   114,    77,    78,   116,    99,   100,    78,   102,   103,
     258,   139,   116,   107,   108,    90,   159,   160,   117,   113,
      97,    96,   275,    98,    99,   114,   247,   115,   115,    41,
     124,   106,   160,   144,   109,   129,   114,   148,   240,   114,
     237,   294,   115,    68,   138,   332,   310,    81,    60,    81,
      81,    58,   115,    28,    94,   149,   184,   151,   152,   114,
       9,   204,   190,   311,   261,   105,     4,    74,     6,    94,
     213,   214,    21,    22,   114,    87,    21,    22,   172,    97,
      55,   175,   116,    97,   116,   116,   107,    32,   336,    71,
     102,     3,   345,    31,   188,   116,   224,   225,    62,   193,
     297,   349,   299,    67,   301,    72,   303,    52,    72,    51,
     204,    75,    72,   207,    56,   258,    83,    61,    10,   316,
     317,    42,    43,    83,     4,   253,     6,   110,    92,   112,
     378,   232,   380,    95,   382,    62,   384,   116,   232,    71,
      67,    21,    22,   237,   110,    72,   240,   113,    75,    41,
     347,    31,   116,   247,   351,    31,   353,    49,   406,   114,
     288,   409,     5,   411,    81,    92,   114,   261,   311,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,   107,
      21,    22,   310,     7,   278,   328,    78,    17,    18,    19,
      42,    43,     7,   336,   114,    87,   115,   116,    70,    91,
     115,   116,     5,   297,   114,   299,   349,   301,    70,   303,
     102,   115,   116,     5,   342,    21,    22,   115,   116,   115,
     116,    61,   316,   317,   421,   115,   116,     1,   116,     3,
     115,   116,   115,   116,   328,   378,   114,   380,   108,   382,
     114,   384,   439,    37,    38,     7,   443,   375,   115,   116,
     115,   116,     7,   347,   115,   116,    30,   351,   386,   353,
     115,   116,    11,   406,   115,   116,   409,    94,   411,   114,
      44,    45,   114,   367,   115,     8,    50,   115,   116,     9,
     374,    55,   115,   116,   115,   116,   115,   116,    20,     4,
       5,     6,     7,   115,    68,   115,    63,   115,   115,    72,
     114,    73,    73,   431,   398,   399,    21,    22,    23,   114,
     105,   104,     7,     7,    88,    39,   114,    65,    33,    34,
      94,     5,   416,   114,   418,    40,    27,   421,   116,   103,
     108,    46,    47,    48,   108,    78,   110,   114,    53,    54,
      78,    86,    73,    72,    59,   439,   114,    72,    82,   443,
      72,    86,    35,   114,    69,    70,    84,   114,   116,    39,
      76,     5,    77,    78,   114,    39,     7,   114,     4,     5,
       6,     7,    91,   115,    10,    90,   116,    79,   114,    85,
       7,    96,   158,    98,    99,    21,    22,   281,   399,   418,
     100,   106,   188,   242,   109,   431,   101,    33,    34,   114,
     115,   240,   213,   126,    40,   214,   159,    97,   135,   130,
      46,    47,    48,   365,     4,     5,     6,     7,    54,   345,
      -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    78,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    90,    -1,    46,    47,    48,    -1,
      96,    -1,    98,    99,    54,    -1,    -1,    -1,    -1,    59,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,   114,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    -1,
      -1,    -1,    -1,     4,     5,     6,     7,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    99,
      21,    22,    -1,    -1,    -1,    -1,   106,    -1,    -1,   109,
      -1,    -1,    33,    34,   114,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    47,    48,    -1,     4,
       5,     6,     7,    54,    -1,    -1,    -1,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    90,
      -1,    46,    47,    48,    -1,    96,    -1,    98,    99,    54,
      -1,    -1,    -1,    -1,    59,   106,    -1,    -1,   109,    -1,
      -1,    33,    34,   114,    69,    70,    -1,    -1,    40,    -1,
      -1,    -1,    77,    78,    46,    47,    48,    -1,    -1,     5,
      -1,     7,    54,    -1,    -1,    90,    -1,    59,    -1,    -1,
      -1,    96,    -1,    98,    99,    21,    22,    69,    70,    -1,
      -1,   106,    -1,    -1,   109,    77,    -1,    33,    34,   114,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    90,    -1,
      46,    47,    48,    -1,    96,    -1,    98,    99,    54,    -1,
      -1,    -1,    -1,    59,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    78,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    90,    21,    22,    -1,    -1,    -1,
      96,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,   109
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,    30,    44,    45,    50,    55,    68,    88,
      94,   103,   108,   110,   119,   120,   121,   122,   123,   124,
     125,   146,   147,   150,   151,   154,   155,   158,   204,   205,
      97,     4,     6,   202,    66,    97,    61,    97,    71,    29,
      53,   159,   202,   156,   157,   171,   202,     0,   110,   112,
      68,   158,   110,   113,     3,   202,    61,   202,   202,   202,
     202,   202,     5,     7,    21,    22,    23,    33,    34,    40,
      46,    47,    48,    54,    59,    69,    70,    77,    78,    90,
      96,    98,    99,   106,   109,   114,   128,   160,   161,   162,
     187,   188,   189,   190,   191,   193,   195,   198,   202,    95,
     116,    31,   114,    71,    28,    55,     5,    81,   114,   107,
     181,   182,   105,   114,   158,     7,     7,   114,    70,     5,
     114,    70,   187,     5,    61,   163,   116,    21,    22,    31,
     199,   202,    23,    24,   117,   200,    22,   190,    27,   114,
     152,   153,   202,   157,   114,   167,   201,   202,   202,    42,
      43,    42,    43,   108,   148,   202,   126,   127,   202,    10,
     114,   183,   184,   185,   186,   187,   195,   114,   201,     7,
       7,   115,   167,   169,   172,   191,   202,   181,   162,   202,
     188,   189,   202,    23,    53,   115,   187,   192,   116,   181,
      11,   158,   115,   116,   158,   126,    41,    60,    87,   102,
     132,   202,   202,   114,   114,   135,   115,   116,    70,   128,
     186,   183,   187,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,   197,    20,   193,   194,   115,
     115,   115,    31,   170,   171,    62,    67,    72,    75,    92,
     116,   164,   165,   166,   170,    36,   101,   168,    63,   173,
     115,   192,   115,   116,   153,   187,   115,   202,   114,    73,
      73,   114,    51,    56,   149,   195,   196,   202,   104,   132,
     133,   134,   126,    10,    41,    49,    78,    87,    91,   102,
     129,   130,   131,   115,   184,   185,    17,    18,    19,   187,
     187,    10,    78,   115,   116,   105,   171,    72,    83,    72,
     201,    72,    83,    72,    83,   169,   166,     7,     7,   170,
      39,    65,   174,   115,   187,   183,   114,   114,   201,     5,
      57,    80,    81,   100,   203,   115,   116,   115,   116,    37,
      38,   144,   116,   108,   136,    78,   114,   193,    73,   202,
     129,   187,     9,    78,   193,   114,   201,    72,   201,    81,
     201,    72,   201,    72,    86,    86,   192,   183,    82,   175,
     115,   201,   201,   115,    51,    56,   195,   114,   145,   132,
      35,    84,   137,   183,   114,     9,   187,   194,    81,   201,
      81,   183,    81,   201,    81,   201,    39,    76,   176,   115,
     115,     5,   203,   139,   140,   141,   142,   143,   202,   114,
      39,   115,   202,   187,   115,   183,    81,   183,   183,    81,
     183,    81,   177,   178,   187,     7,    91,   115,   116,     7,
      29,   114,   202,   139,    64,    89,   138,   115,   183,   183,
     183,   116,    32,    52,   179,   202,   140,   201,   115,   114,
     178,    79,   180,   114,   115,   201,    58,    74,   201,   115,
     115,    85,     7
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   118,   119,   119,   119,   119,   119,   119,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   121,   122,
     122,   122,   122,   123,   124,   125,   126,   127,   127,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   129,   129,   129,
     129,   129,   129,   129,   130,   130,   131,   131,   132,   132,
     132,   132,   133,   133,   134,   134,   135,   135,   136,   136,
     137,   137,   138,   138,   139,   139,   140,   140,   140,   141,
     141,   142,   143,   144,   144,   145,   145,   146,   146,   146,
     146,   147,   148,   148,   149,   149,   149,   149,   150,   151,
     152,   152,   153,   154,   154,   155,   156,   156,   157,   158,
     159,   159,   159,   160,   160,   161,   161,   162,   162,   162,
     163,   164,   164,   165,   165,   166,   166,   166,   166,   166,
     166,   166,   166,   167,   168,   168,   168,   169,   169,   169,
     169,   169,   170,   170,   171,   171,   172,   172,   173,   173,
     174,   174,   175,   175,   176,   176,   177,   177,   178,   179,
     179,   179,   180,   180,   180,   181,   181,   182,   183,   183,
     184,   184,   185,   185,   186,   186,   186,   186,   186,   186,
     187,   187,   188,   188,   189,   189,   190,   190,   190,   190,
     191,   191,   191,   191,   192,   192,   193,   193,   193,   193,
     193,   193,   193,   194,   194,   195,   195,   196,   196,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   198,
     199,   199,   200,   200,   200,   201,   201,   202,   202,   203,
     203,   203,   203,   204,   205,   205
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
       3,     1,     3,     1,     2,     1,     1,     1,     1,     3,
       3,     4,     4,     5,     1,     3,     1,     1,     2,     2,
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
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1765 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_NAME  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1775 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1785 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1795 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 7: /* TOKEN_UNSIGNED_NUMVAL  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 1805 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 120: /* sql_statement  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1815 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 121: /* quit_statement  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 1825 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 122: /* alter_table_statement  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1835 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 123: /* create_table_statement  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 1845 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 124: /* create_index_statement  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1855 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 125: /* drop_table_statement  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 1865 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 126: /* column_def  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 1875 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 127: /* column_def_commalist  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 1885 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 128: /* data_type  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 1895 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 129: /* column_constraint_def  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 1905 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 130: /* column_constraint_def_list  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1915 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 131: /* opt_column_constraint_def_list  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1925 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 135: /* opt_column_list  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 1935 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 136: /* opt_block_properties  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).block_properties_) != nullptr) {
    delete ((*yyvaluep).block_properties_);
  }
}
#line 1945 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 137: /* opt_partition_clause  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).partition_clause_) != nullptr) {
    delete ((*yyvaluep).partition_clause_);
  }
}
#line 1955 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 138: /* partition_type  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1965 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 139: /* key_value_list  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 1975 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 140: /* key_value  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 1985 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 141: /* key_string_value  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 1995 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 142: /* key_string_list  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 2005 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 143: /* key_integer_value  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 2015 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 144: /* index_type  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2025 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 145: /* opt_index_properties  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2035 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 146: /* insert_statement  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 2045 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 147: /* copy_from_statement  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_statement_) != nullptr) {
    delete ((*yyvaluep).copy_from_statement_);
  }
}
#line 2055 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 148: /* opt_copy_from_params  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2065 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 149: /* copy_from_params  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2075 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 150: /* update_statement  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2085 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 151: /* delete_statement  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2095 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 152: /* assignment_list  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2105 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 153: /* assignment_item  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2115 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* select_statement  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_statement_) != nullptr) {
    delete ((*yyvaluep).select_statement_);
  }
}
#line 2125 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* with_clause  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2135 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* with_list  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2145 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* with_list_element  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2155 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* select_query  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2165 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* selection  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2175 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* selection_item_commalist  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2185 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* selection_item  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2195 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 163: /* from_clause  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2205 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 167: /* subquery_expression  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2215 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* opt_sample_clause  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_sample_clause_) != nullptr) {
    delete ((*yyvaluep).opt_sample_clause_);
  }
}
#line 2225 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* table_reference  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2235 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* table_reference_signature  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2245 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* table_reference_signature_primary  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2255 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* table_reference_commalist  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2265 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* opt_group_by_clause  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2275 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* opt_having_clause  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2285 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* opt_order_by_clause  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2295 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* opt_limit_clause  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2305 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* order_commalist  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2315 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* order_item  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2325 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* opt_order_direction  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2335 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* opt_nulls_first  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2345 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* opt_where_clause  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2355 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* where_clause  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2365 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* or_expression  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2375 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* and_expression  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2385 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* not_expression  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2395 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* predicate_expression_base  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2405 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* add_expression  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2415 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* multiply_expression  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2425 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* unary_expression  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2435 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 190: /* expression_base  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2445 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 191: /* function_call  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2455 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 192: /* expression_list  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2465 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 193: /* literal_value  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2475 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 194: /* literal_value_commalist  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2485 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 195: /* attribute_ref  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2495 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 196: /* attribute_ref_list  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2505 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 197: /* comparison_operation  */
#line 523 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2511 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 198: /* unary_operation  */
#line 524 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2517 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 199: /* add_operation  */
#line 525 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2523 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 200: /* multiply_operation  */
#line 525 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2529 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 201: /* name_commalist  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 2539 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 202: /* any_name  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2549 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 203: /* boolean_value  */
#line 522 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2555 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 204: /* command  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_) != nullptr) {
    delete ((*yyvaluep).command_);
  }
}
#line 2565 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 205: /* command_argument_list  */
#line 527 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_argument_list_) != nullptr) {
    delete ((*yyvaluep).command_argument_list_);
  }
}
#line 2575 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 536 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2872 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 540 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2881 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 544 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 2890 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 5:
#line 548 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 2899 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 552 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    YYABORT;
  }
#line 2907 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 555 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 2916 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 562 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 2924 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 9:
#line 565 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].copy_from_statement_);
  }
#line 2932 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 10:
#line 568 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 2940 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 11:
#line 571 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 2948 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 12:
#line 574 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 2956 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 13:
#line 577 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 2964 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 14:
#line 580 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 2972 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 15:
#line 583 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 2980 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 16:
#line 586 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].select_statement_);
  }
#line 2988 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 17:
#line 589 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 2996 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 18:
#line 595 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3004 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 19:
#line 601 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3016 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 20:
#line 608 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3027 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 614 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3039 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 621 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3051 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 630 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].attribute_definition_list_), (yyvsp[-1].block_properties_), (yyvsp[0].partition_clause_));
  }
#line 3059 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 635 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].key_value_list_)) {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_), (yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].key_value_list_));
    } else {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_));
    }
  }
#line 3071 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 644 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 3079 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 649 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 3087 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 654 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3096 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 658 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3105 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 664 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 3115 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 669 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3123 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 672 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3131 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 675 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3141 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 680 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3149 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 683 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3157 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 686 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3165 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 689 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3173 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 692 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3181 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 695 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3189 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 698 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3197 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 701 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3205 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 704 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3213 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 707 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3230 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 719 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3238 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 722 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3246 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 725 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3269 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 743 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3292 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 763 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3300 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 766 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3308 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 769 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3318 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 774 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3328 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 779 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3339 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 785 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3350 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 791 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3362 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 800 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3371 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 804 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3380 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 810 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3388 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 813 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3396 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 818 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3406 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 823 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3416 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 828 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3428 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 835 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3438 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 842 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3447 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 846 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3456 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 852 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3464 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 855 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = $1; */
  }
#line 3472 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 860 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = nullptr;
  }
#line 3480 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 863 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-1].attribute_list_);
  }
#line 3488 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 868 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = nullptr;
  }
#line 3496 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 871 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].key_value_list_));
  }
#line 3504 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 876 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.partition_clause_) = nullptr;
  }
#line 3512 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 879 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 3534 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 898 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::PartitionSchemeHeader::PartitionType::kHash));
  }
#line 3543 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 902 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::PartitionSchemeHeader::PartitionType::kRange));
  }
#line 3552 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 908 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3561 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 912 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3570 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 918 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 3578 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 921 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 3586 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 924 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 3594 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 929 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 3602 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 932 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_),
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 3612 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 939 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 3620 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 944 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 3634 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 955 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
#line 3643 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 959 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
#line 3652 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 965 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 3660 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 968 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 3668 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 974 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 3681 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 982 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertTuple((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 3689 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 985 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-3]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), nullptr);
  }
#line 3697 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 988 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-4]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), (yyvsp[-4].with_list_));
  }
#line 3705 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 994 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_statement_) = new quickstep::ParseStatementCopyFrom((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].copy_from_params_));
  }
#line 3713 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 999 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = nullptr;
  }
#line 3721 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 1002 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-1].copy_from_params_);
  }
#line 3729 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 1007 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3738 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 1011 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3747 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 1015 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3756 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 1019 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3765 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 1025 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 3773 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 1030 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 3781 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 1035 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3790 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 1039 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3799 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 1045 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 3807 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 1051 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].select_query_), nullptr);
  }
#line 3815 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 1054 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].select_query_), (yyvsp[-1].with_list_));
  }
#line 3823 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 1059 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 3831 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 1064 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3840 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 1068 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3849 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 1074 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 3858 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 1081 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].selection_), (yyvsp[-5].table_reference_list_), (yyvsp[-4].predicate_), (yyvsp[-3].opt_group_by_clause_), (yyvsp[-2].opt_having_clause_), (yyvsp[-1].opt_order_by_clause_), (yyvsp[0].opt_limit_clause_));
  }
#line 3866 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 1086 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3874 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 1089 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "ALL in selection");
    YYERROR;
  }
#line 3883 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 1093 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "DISTINCT in selection");
    YYERROR;
  }
#line 3892 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 1099 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3900 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 1102 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 3908 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 1107 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3917 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 1111 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3926 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 1117 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 3934 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 1120 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 3942 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 1123 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 3950 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 1128 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-1].table_reference_list_);
  }
#line 3958 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 1133 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /* $$ = nullptr; */
  }
#line 3966 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 1136 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3975 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 1142 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[-1]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3984 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 1146 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3993 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 1152 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4004 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 1158 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4015 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 1164 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4026 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 1170 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4037 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 1176 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4048 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 1182 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4059 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 1188 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4070 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 1194 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4081 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 1202 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].select_query_));
  }
#line 4089 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 1207 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = NULL;
  }
#line 4097 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 1210 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, true, (yyvsp[-1].numeric_literal_value_));
  }
#line 4105 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 1213 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].numeric_literal_value_));
  }
#line 4113 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 1218 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4122 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 1222 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].opt_sample_clause_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4131 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 1226 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].opt_sample_clause_));
  }
#line 4139 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 1229 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4148 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 1233 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].function_call_));
  }
#line 4156 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 1238 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4164 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 1241 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4172 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 1246 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4180 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 1249 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4188 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 1254 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4197 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 1258 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4206 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 1264 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 4214 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 1267 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4222 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 1272 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4230 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 1275 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4238 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 1280 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4246 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 1283 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4254 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 1288 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4262 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 1291 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4284 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 1310 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4293 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 1314 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4302 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 1320 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 4312 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 1327 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4320 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 1330 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4328 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 1333 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4336 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 1338 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4344 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 1341 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4352 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 1344 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4360 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 1350 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 4368 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 1353 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4376 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 1358 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4384 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 1363 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4398 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 1372 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4406 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 1377 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4420 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 1386 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4428 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 1391 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4436 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 1394 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4444 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 1399 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4452 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 1402 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 4462 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 1407 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4473 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 1413 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4484 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 1419 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4492 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 1422 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 4500 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 1428 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4508 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 1431 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4516 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 1436 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4524 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 1439 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4532 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 1444 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].unary_operation_), (yyvsp[0].expression_));
  }
#line 4540 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 1447 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4548 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1452 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 4556 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 1455 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 4564 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 1458 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 4572 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 1461 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 4580 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 1466 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 4589 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 1470 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 4598 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 1474 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, false, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4606 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 1477 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-4]).first_line, (yylsp[-4]).first_column, true, (yyvsp[-4].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4614 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 1482 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4623 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 1486 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4632 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 1492 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4640 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 1495 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4648 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 1498 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4656 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 1501 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4673 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1513 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 4682 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1517 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4706 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1536 "../SqlParser.ypp" /* yacc.c:1661  */
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
#line 4724 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1551 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4733 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1555 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4742 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1561 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4750 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1564 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 4758 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1569 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = new quickstep::PtrList<quickstep::ParseAttribute>();
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 4767 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1573 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.attribute_list_) = (yyvsp[-2].attribute_list_);
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 4776 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1580 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 4784 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1583 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 4792 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1586 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 4800 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1589 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 4808 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1592 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 4816 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1595 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 4824 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1598 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLike);
  }
#line 4832 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1601 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotLike);
  }
#line 4840 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1604 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kRegexMatch);
  }
#line 4848 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1607 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotRegexMatch);
  }
#line 4856 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1612 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
#line 4870 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1623 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
#line 4878 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1626 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
#line 4886 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1631 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kModulo);
  }
#line 4894 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1634 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
#line 4902 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1637 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
#line 4910 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1643 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4919 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1647 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4928 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1653 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 4936 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1656 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 4947 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1664 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4955 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1667 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4963 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1670 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 4971 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1673 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.boolean_value_) = false;
  }
#line 4979 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1679 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    (yyval.command_) = new quickstep::ParseCommand((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].command_argument_list_));
  }
#line 4987 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1684 "../SqlParser.ypp" /* yacc.c:1661  */
    {
    quickstep::PtrVector<quickstep::ParseString> *argument_list = (yyvsp[-1].command_argument_list_);
    argument_list->push_back((yyvsp[0].string_value_));
    (yyval.command_argument_list_) = argument_list;
  }
#line 4997 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1689 "../SqlParser.ypp" /* yacc.c:1661  */
    { /* Epsilon, an empy match. */
    (yyval.command_argument_list_) = new quickstep::PtrVector<quickstep::ParseString>();
  }
#line 5005 "SqlParser_gen.cpp" /* yacc.c:1661  */
    break;


#line 5009 "SqlParser_gen.cpp" /* yacc.c:1661  */
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
#line 1693 "../SqlParser.ypp" /* yacc.c:1906  */


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
