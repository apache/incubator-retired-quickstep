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
    TOKEN_CASE = 290,
    TOKEN_CHARACTER = 291,
    TOKEN_CHECK = 292,
    TOKEN_COLUMN = 293,
    TOKEN_CONSTRAINT = 294,
    TOKEN_COPY = 295,
    TOKEN_CREATE = 296,
    TOKEN_DATE = 297,
    TOKEN_DATETIME = 298,
    TOKEN_DECIMAL = 299,
    TOKEN_DEFAULT = 300,
    TOKEN_DELETE = 301,
    TOKEN_DELIMITER = 302,
    TOKEN_DESC = 303,
    TOKEN_DISTINCT = 304,
    TOKEN_DOUBLE = 305,
    TOKEN_DROP = 306,
    TOKEN_ELSE = 307,
    TOKEN_END = 308,
    TOKEN_ESCAPE_STRINGS = 309,
    TOKEN_EXISTS = 310,
    TOKEN_EXTRACT = 311,
    TOKEN_FALSE = 312,
    TOKEN_FIRST = 313,
    TOKEN_FLOAT = 314,
    TOKEN_FOREIGN = 315,
    TOKEN_FROM = 316,
    TOKEN_FULL = 317,
    TOKEN_GROUP = 318,
    TOKEN_HASH = 319,
    TOKEN_HAVING = 320,
    TOKEN_INDEX = 321,
    TOKEN_INNER = 322,
    TOKEN_INSERT = 323,
    TOKEN_INTEGER = 324,
    TOKEN_INTERVAL = 325,
    TOKEN_INTO = 326,
    TOKEN_JOIN = 327,
    TOKEN_KEY = 328,
    TOKEN_LAST = 329,
    TOKEN_LEFT = 330,
    TOKEN_LIMIT = 331,
    TOKEN_LONG = 332,
    TOKEN_NULL = 333,
    TOKEN_NULLS = 334,
    TOKEN_OFF = 335,
    TOKEN_ON = 336,
    TOKEN_ORDER = 337,
    TOKEN_OUTER = 338,
    TOKEN_PARTITION = 339,
    TOKEN_PARTITIONS = 340,
    TOKEN_PERCENT = 341,
    TOKEN_PRIMARY = 342,
    TOKEN_QUIT = 343,
    TOKEN_RANGE = 344,
    TOKEN_REAL = 345,
    TOKEN_REFERENCES = 346,
    TOKEN_RIGHT = 347,
    TOKEN_ROW_DELIMITER = 348,
    TOKEN_SELECT = 349,
    TOKEN_SET = 350,
    TOKEN_SMA = 351,
    TOKEN_SMALLINT = 352,
    TOKEN_TABLE = 353,
    TOKEN_THEN = 354,
    TOKEN_TIME = 355,
    TOKEN_TIMESTAMP = 356,
    TOKEN_TRUE = 357,
    TOKEN_TUPLESAMPLE = 358,
    TOKEN_UNIQUE = 359,
    TOKEN_UPDATE = 360,
    TOKEN_USING = 361,
    TOKEN_VALUES = 362,
    TOKEN_VARCHAR = 363,
    TOKEN_WHEN = 364,
    TOKEN_WHERE = 365,
    TOKEN_WITH = 366,
    TOKEN_YEARMONTH = 367,
    TOKEN_EOF = 368,
    TOKEN_LEX_ERROR = 369
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
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

#line 397 "SqlParser_gen.cpp" /* yacc.c:355  */
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
#line 206 "../SqlParser.ypp" /* yacc.c:358  */

/* This header needs YYSTYPE, which is defined by the %union directive above */
#include "SqlLexer_gen.hpp"
void NotSupported(const YYLTYPE *location, yyscan_t yyscanner, const std::string &feature);

#line 430 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYLAST   1038

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  126
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  95
/* YYNRULES -- Number of rules.  */
#define YYNRULES  250
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  486

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   369

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     121,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   125,     2,     2,
     122,   123,    23,    21,   124,    22,    27,    24,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   120,
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
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   567,   567,   571,   575,   579,   583,   586,   593,   596,
     599,   602,   605,   608,   611,   614,   617,   620,   626,   632,
     639,   645,   652,   661,   666,   675,   680,   685,   689,   695,
     700,   703,   706,   711,   714,   717,   720,   723,   726,   729,
     732,   735,   738,   750,   753,   756,   774,   794,   797,   800,
     805,   810,   816,   822,   831,   835,   841,   844,   849,   854,
     859,   866,   873,   877,   883,   886,   891,   894,   899,   902,
     907,   910,   929,   933,   939,   943,   949,   952,   955,   960,
     963,   970,   975,   986,   990,   994,  1000,  1003,  1009,  1017,
    1020,  1023,  1029,  1034,  1037,  1042,  1046,  1050,  1054,  1060,
    1065,  1070,  1074,  1080,  1086,  1089,  1094,  1099,  1103,  1109,
    1115,  1121,  1124,  1128,  1134,  1137,  1142,  1146,  1152,  1155,
    1158,  1163,  1168,  1171,  1177,  1181,  1187,  1193,  1199,  1205,
    1211,  1217,  1223,  1229,  1237,  1242,  1245,  1248,  1253,  1257,
    1261,  1264,  1268,  1273,  1276,  1281,  1284,  1289,  1293,  1299,
    1302,  1307,  1310,  1315,  1318,  1323,  1326,  1345,  1349,  1355,
    1362,  1365,  1368,  1373,  1376,  1379,  1385,  1388,  1393,  1398,
    1407,  1412,  1421,  1426,  1429,  1434,  1437,  1442,  1448,  1454,
    1457,  1460,  1468,  1471,  1476,  1479,  1484,  1487,  1492,  1495,
    1498,  1501,  1504,  1507,  1512,  1516,  1520,  1523,  1528,  1533,
    1536,  1541,  1545,  1551,  1556,  1560,  1566,  1571,  1574,  1579,
    1583,  1589,  1592,  1595,  1598,  1610,  1614,  1633,  1648,  1652,
    1658,  1661,  1666,  1670,  1677,  1680,  1683,  1686,  1689,  1692,
    1695,  1698,  1701,  1704,  1709,  1720,  1723,  1728,  1731,  1734,
    1740,  1744,  1750,  1753,  1761,  1764,  1767,  1770,  1776,  1781,
    1786
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
  "TOKEN_CASE", "TOKEN_CHARACTER", "TOKEN_CHECK", "TOKEN_COLUMN",
  "TOKEN_CONSTRAINT", "TOKEN_COPY", "TOKEN_CREATE", "TOKEN_DATE",
  "TOKEN_DATETIME", "TOKEN_DECIMAL", "TOKEN_DEFAULT", "TOKEN_DELETE",
  "TOKEN_DELIMITER", "TOKEN_DESC", "TOKEN_DISTINCT", "TOKEN_DOUBLE",
  "TOKEN_DROP", "TOKEN_ELSE", "TOKEN_END", "TOKEN_ESCAPE_STRINGS",
  "TOKEN_EXISTS", "TOKEN_EXTRACT", "TOKEN_FALSE", "TOKEN_FIRST",
  "TOKEN_FLOAT", "TOKEN_FOREIGN", "TOKEN_FROM", "TOKEN_FULL",
  "TOKEN_GROUP", "TOKEN_HASH", "TOKEN_HAVING", "TOKEN_INDEX",
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
      59,    10,    40,    41,    44,    37
};
# endif

#define YYPACT_NINF -223

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-223)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     367,  -223,  -223,   -29,   233,    -6,    18,    34,    35,  -223,
     138,   233,   233,  -223,    94,   132,  -223,  -223,  -223,  -223,
    -223,  -223,  -223,  -223,  -223,  -223,   -20,  -223,   111,   165,
     233,  -223,  -223,    98,   233,   233,   233,   233,   233,  -223,
    -223,   564,    72,    52,  -223,   185,   103,  -223,  -223,  -223,
     118,  -223,  -223,  -223,  -223,    68,   231,   167,   125,   142,
    -223,    76,  -223,  -223,   256,   260,  -223,  -223,  -223,   627,
     139,  -223,   213,  -223,  -223,   160,  -223,  -223,   287,  -223,
    -223,  -223,  -223,  -223,  -223,   179,   228,   753,   300,   241,
     187,  -223,   234,    33,  -223,  -223,  -223,  -223,  -223,  -223,
    -223,   816,   -16,   233,   233,   204,   233,   233,   199,   237,
     205,   233,   233,   501,  -223,  -223,   206,   233,  -223,  -223,
    -223,   501,    49,   -28,  -223,   302,  -223,   233,  -223,   329,
    -223,    30,  -223,    17,   142,   753,  -223,  -223,   233,   753,
    -223,  -223,  -223,  -223,   753,   260,  -223,   233,   415,    81,
    -223,   330,  -223,   248,  -223,   153,  -223,   248,   233,    27,
     233,   233,   226,  -223,   245,  -223,   161,   908,   690,   204,
     501,   342,   344,  -223,  -223,  1016,   335,   879,   163,    16,
     753,    -2,  -223,   753,  -223,   299,   246,   305,   249,  -223,
      56,  -223,   102,    56,   -18,   306,  -223,  -223,    33,  -223,
    -223,   250,   753,  -223,   269,   171,   233,  -223,   753,   252,
    -223,   233,  -223,  -223,   255,   301,   303,   261,  -223,  -223,
    -223,   121,   233,   267,    27,   233,  -223,    26,  -223,  -223,
       7,    28,   501,   501,   254,  -223,  -223,  -223,  -223,  -223,
    -223,  -223,  -223,   753,   753,    15,  -223,   173,   272,   753,
      51,  -223,   327,   269,  -223,  -223,   753,  -223,   233,  -223,
    -223,   -13,   309,   233,     5,   113,    17,  -223,   147,  -223,
    -223,   381,   382,    56,   351,   321,  -223,   175,  -223,   753,
    -223,   269,  -223,  -223,   501,   270,   271,   233,   389,    99,
     189,  -223,   191,   368,    43,  -223,   275,   284,  -223,   318,
     280,   879,  -223,   326,   233,  -223,  -223,    26,  -223,  -223,
     344,  -223,  -223,  -223,   753,   224,   269,   322,  -223,  -223,
     879,   285,   269,   753,  -223,    37,  -223,   233,   331,   233,
     -19,   233,   332,   233,   333,  -223,  -223,   315,   320,  -223,
     753,   501,   328,  -223,   269,     8,   233,   233,   193,  -223,
    -223,  -223,  -223,  -223,  -223,  -223,   141,  -223,   233,  -223,
    -223,  -223,   292,    27,   390,   337,  -223,   501,  -223,  -223,
     294,  -223,   257,   753,  -223,  -223,   879,   269,  -223,    -8,
     233,    -3,   501,    24,   233,    54,   233,  -223,  -223,   293,
     342,   385,   348,  -223,   207,   210,  -223,   425,    99,  -223,
     233,  -223,  -223,   310,   392,  -223,     9,   233,   753,   269,
     216,   501,    59,   501,   342,   501,    79,   501,    80,   753,
     426,  -223,   338,  -223,  -223,  -223,   221,  -223,  -223,  -223,
    -223,     6,   233,   101,  -223,   312,   269,  -223,   342,   501,
     342,   342,   501,   342,   501,   317,  -223,   188,  -223,   233,
    -223,   233,  -223,  -223,   233,  -223,   235,  -223,  -223,   324,
    -223,   342,   342,   342,   753,  -223,  -223,   355,   336,  -223,
     238,  -223,   233,  -223,    22,  -223,   233,  -223,   240,  -223,
    -223,   242,   352,  -223,   437,  -223
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     6,   250,     0,     0,     0,     0,     0,     0,    18,
     111,     0,     0,     7,     0,     0,    15,     8,    10,    11,
      13,    14,     9,    17,    12,    16,     0,   104,     0,   248,
       0,   242,   243,     0,     0,     0,     0,     0,     0,   112,
     113,     0,     0,   106,   107,     0,   145,     1,     3,     2,
       0,   105,     5,     4,   249,     0,     0,     0,     0,   166,
      25,     0,   215,   212,     0,   234,   114,    40,    29,     0,
       0,    30,    31,    34,    36,     0,    37,    39,     0,    41,
     211,    35,    38,    32,    33,     0,     0,     0,     0,     0,
     115,   116,   120,   183,   185,   187,   190,   191,   192,   189,
     188,     0,   220,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,   100,   167,     0,     0,    90,   213,
     214,     0,     0,   207,   204,     0,    43,     0,   216,     0,
      44,     0,   217,     0,   166,     0,   235,   236,     0,     0,
     119,   238,   239,   237,     0,     0,   186,     0,     0,   166,
     102,     0,   108,     0,   109,     0,   240,     0,     0,     0,
       0,     0,     0,    92,    66,    27,     0,     0,     0,     0,
       0,   168,   170,   172,   174,     0,   188,     0,     0,     0,
       0,   207,   201,     0,   205,     0,     0,     0,     0,   193,
       0,   147,   122,   142,   135,   149,   117,   118,   182,   184,
     221,     0,     0,   194,   209,     0,     0,    99,     0,     0,
     146,     0,    91,    19,     0,     0,     0,     0,    20,    21,
      22,     0,     0,     0,    64,     0,    42,    56,   173,   181,
       0,     0,     0,     0,     0,   224,   226,   227,   228,   229,
     225,   230,   232,     0,     0,     0,   218,     0,     0,     0,
       0,   202,     0,   208,   200,    45,     0,    46,     0,   138,
     143,     0,     0,     0,     0,     0,     0,   121,   123,   125,
     141,     0,     0,   140,     0,   151,   195,     0,   196,     0,
     101,   103,   134,   241,     0,     0,     0,     0,     0,     0,
       0,   222,     0,   220,     0,    63,    65,    68,    28,     0,
       0,     0,    47,     0,     0,    49,    55,    57,    26,   180,
     169,   171,   231,   233,     0,     0,   179,     0,   178,    89,
       0,     0,   206,     0,   199,     0,   144,     0,     0,     0,
       0,     0,     0,     0,     0,   148,   124,     0,     0,   139,
       0,     0,   153,   197,   210,     0,     0,     0,     0,    95,
     246,   247,   245,   244,    96,    94,     0,    67,     0,    83,
      84,    85,    86,     0,     0,    70,    48,     0,    51,    50,
       0,    54,     0,     0,   177,   219,     0,   203,   198,     0,
       0,     0,     0,     0,     0,     0,     0,   136,   137,   150,
     152,     0,   155,    61,     0,     0,    58,     0,     0,   223,
       0,    24,    62,     0,     0,    23,     0,     0,     0,   175,
       0,     0,     0,     0,   127,     0,     0,     0,     0,     0,
       0,   110,     0,    59,    97,    98,     0,    74,    76,    77,
      78,     0,     0,     0,    52,     0,   176,    88,   133,     0,
     126,   129,     0,   131,     0,   154,   157,   160,   156,     0,
      87,     0,    82,    80,     0,    79,     0,    72,    73,     0,
      53,   132,   128,   130,     0,   161,   162,   163,     0,    75,
       0,    69,     0,   158,     0,   159,     0,    81,     0,   164,
     165,     0,     0,    60,     0,    71
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -223,  -223,  -223,  -223,  -223,  -223,  -223,  -223,  -136,  -223,
     286,   144,  -223,  -223,  -222,  -223,  -223,  -223,  -223,  -223,
    -223,    20,     3,  -223,  -223,  -223,  -223,  -223,  -223,  -223,
    -223,  -223,  -223,  -223,  -223,   251,  -223,  -223,  -223,   357,
      -7,  -223,  -223,  -223,   339,  -223,  -223,  -223,   197,   -78,
    -223,   201,  -156,    -9,  -223,  -223,  -223,  -223,  -223,  -223,
      -5,  -223,  -223,    63,  -223,   -93,   239,   247,   307,   -21,
     334,   340,   377,  -128,  -223,  -223,  -223,   311,  -223,   358,
     313,  -198,  -168,   106,  -107,  -223,  -223,  -223,  -223,  -223,
    -116,    -4,    88,  -223,  -223
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    14,    15,    16,    17,    18,    19,    20,   165,   166,
      88,   306,   307,   308,   218,   296,   297,   223,   365,   405,
     459,   426,   427,   428,   429,   430,   362,   401,    21,    22,
     163,   290,    23,    24,   149,   150,    25,    26,    43,    44,
      27,    41,    89,    90,    91,   134,   267,   268,   269,   190,
     273,   191,   259,   260,   192,   275,   342,   392,   421,   445,
     446,   467,   475,   114,   115,   171,   172,   173,   174,   175,
      93,    94,    95,    96,    97,    98,   181,   182,   123,   124,
     185,   205,    99,   247,   100,   292,   244,   101,   139,   144,
     155,   102,   354,    28,    29
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
      33,   178,   295,    45,   277,   193,   176,    42,    46,   246,
      31,   147,    32,   452,   176,   232,   232,   232,   271,    51,
      92,    31,   213,    32,   232,   317,    55,   154,   179,   183,
      57,    58,    59,    60,    61,   453,   299,   270,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   122,   136,
     137,   136,   137,    50,   118,   183,   141,   142,   136,   137,
      31,   176,    32,   176,   327,    34,   131,   382,   300,   214,
     136,   137,   136,   137,    30,   328,   301,   230,   411,    10,
     359,   360,   331,   413,    36,   479,   121,   258,   140,   298,
     272,   229,   215,   332,    47,    45,   108,    35,   318,   151,
      46,   480,   156,   157,   148,   211,   148,   164,   167,   302,
     415,    38,   180,   156,    92,   291,   211,   339,   303,   216,
     249,   211,   304,   187,   109,   176,   176,   204,   454,   194,
     309,   393,   434,   368,   197,   305,   217,    37,   193,   153,
     417,   402,   389,   200,   361,   439,   209,   330,   211,   231,
     212,   189,   375,   189,   167,   323,   219,   220,   143,   250,
     378,   350,   253,   180,    56,   442,   444,    39,    54,   261,
     457,   348,   103,   288,   262,    10,   104,   176,   211,   263,
     289,   204,   264,   211,   351,   352,    46,   281,   116,    46,
     333,   345,    40,   397,   107,   458,   113,   195,   117,   265,
     398,   334,   151,   211,   211,   206,   353,   283,   246,   136,
     137,   379,   207,   381,   261,   383,   105,   385,   293,   262,
     465,   167,   315,   316,   263,   106,   266,   264,   322,    52,
     394,   395,    53,   373,   176,   325,   110,    31,    31,    32,
      32,   466,   158,   159,   265,   136,   137,   112,   390,   326,
      48,   399,    49,   111,    46,   136,   137,   113,   344,   156,
     176,   125,   194,   119,   412,   138,   408,   120,   416,    46,
     418,   312,   313,   314,   406,   176,   210,   211,   136,   137,
     160,   161,   127,   156,   224,   225,   248,   211,   126,   414,
     136,   137,   128,   372,   278,   279,   319,   320,   343,   279,
     370,   129,   377,   130,   176,   132,   176,   133,   176,   186,
     176,   135,   355,   356,   357,   358,   396,   211,   438,   204,
     440,   162,   441,   156,   443,   156,   153,   156,   177,   156,
     422,   211,   176,   423,   211,   176,   188,   176,   470,   437,
     320,   208,   156,   156,   450,   451,   461,    10,   221,   462,
     232,   463,   409,   233,   293,   245,   478,   254,   471,   451,
     481,   477,   211,   482,   211,   483,   211,   222,     1,   255,
       2,   256,   257,   276,   274,   282,   156,   284,   294,   285,
     156,   286,   156,   287,   321,   324,   329,   436,   337,   338,
     340,   341,   346,   347,   349,   147,   431,     3,   447,   363,
     364,   366,   367,   435,   369,   374,   387,   376,   380,   384,
     386,   388,     4,     5,   400,   391,   407,   279,     6,    31,
      62,    32,    63,     7,   419,   403,   404,   455,   431,   420,
     424,   433,   432,   448,   449,   460,    64,    65,   201,   474,
       8,   464,   484,   447,   485,   468,   472,   431,    67,    68,
     156,   371,   456,   227,   469,    69,    70,   280,   476,   473,
       9,   152,    71,    72,    73,   336,    10,   335,   156,   202,
      74,   310,   156,   198,   196,   228,    75,    11,   146,    76,
     311,   184,   410,    12,   199,    13,   425,     0,     0,    77,
      78,     0,   251,     0,   252,     0,     0,    79,    80,     0,
       0,     0,     0,     0,     0,    31,    62,    32,    63,     0,
      81,   168,     0,     0,     0,     0,     0,    82,     0,     0,
      83,    84,    64,    65,     0,     0,     0,     0,    85,     0,
       0,     0,    86,     0,    67,    68,     0,    87,   203,     0,
       0,    69,    70,     0,     0,     0,     0,     0,    71,    72,
      73,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,   169,    75,     0,     0,    76,     0,     0,    31,    62,
      32,    63,     0,     0,     0,    77,    78,     0,     0,     0,
       0,     0,     0,    79,    80,    64,    65,    66,     0,     0,
       0,     0,     0,     0,     0,     0,    81,    67,    68,     0,
       0,     0,     0,    82,    69,    70,    83,    84,     0,     0,
       0,    71,    72,    73,    85,     0,     0,     0,    86,    74,
       0,     0,     0,   170,     0,    75,     0,     0,    76,     0,
       0,    31,    62,    32,    63,     0,     0,     0,    77,    78,
       0,     0,     0,     0,     0,     0,    79,    80,    64,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    81,
      67,    68,     0,     0,     0,     0,    82,    69,    70,    83,
      84,     0,     0,     0,    71,    72,    73,    85,     0,     0,
       0,    86,    74,     0,     0,     0,    87,     0,    75,     0,
       0,    76,     0,     0,    31,    62,    32,    63,     0,     0,
       0,    77,    78,     0,     0,     0,     0,     0,     0,    79,
      80,    64,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    81,    67,    68,     0,     0,     0,     0,    82,
      69,    70,    83,    84,     0,     0,     0,    71,    72,    73,
      85,   121,     0,     0,    86,    74,     0,     0,     0,    87,
     169,    75,     0,     0,    76,     0,     0,    31,    62,    32,
      63,     0,     0,     0,    77,    78,     0,     0,     0,     0,
       0,     0,    79,    80,    64,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    81,    67,    68,     0,     0,
       0,     0,    82,    69,    70,    83,    84,     0,     0,     0,
      71,    72,    73,    85,     0,     0,     0,    86,    74,     0,
       0,     0,   170,     0,    75,     0,     0,    76,     0,     0,
      31,    62,    32,    63,     0,     0,     0,    77,    78,     0,
       0,     0,     0,     0,     0,    79,    80,    64,   145,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    81,    67,
      68,     0,     0,     0,     0,    82,    69,    70,    83,    84,
       0,     0,     0,    71,    72,    73,    85,     0,     0,     0,
      86,    74,     0,     0,     0,    87,     0,    75,     0,     0,
      76,     0,     0,     0,    62,     0,    63,     0,     0,     0,
      77,    78,     0,     0,     0,     0,     0,     0,    79,    80,
      64,   145,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    81,    67,    68,     0,     0,     0,     0,    82,     0,
      70,    83,    84,     0,     0,     0,    71,    72,    73,    85,
       0,     0,     0,    86,    74,     0,     0,     0,    87,     0,
       0,    67,    68,    76,     0,     0,     0,     0,     0,    70,
       0,     0,     0,    77,    78,    71,    72,    73,     0,     0,
       0,    79,    80,    74,     0,     0,     0,     0,     0,     0,
       0,     0,    76,     0,    81,     0,     0,     0,     0,     0,
       0,    82,    77,   226,    83,    84,     0,     0,     0,     0,
      79,     0,    85,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,    81,     0,     0,     0,     0,     0,     0,
      82,     0,     0,    83,    84,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,    86,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,     0,   136,   137
};

static const yytype_int16 yycheck[] =
{
       4,   117,   224,    12,   202,   133,   113,    11,    12,   177,
       4,    27,     6,     7,   121,     8,     8,     8,    36,    26,
      41,     4,   158,     6,     8,    10,    30,   105,   121,    57,
      34,    35,    36,    37,    38,    29,    10,   193,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    69,    21,
      22,    21,    22,    73,    61,    57,    23,    24,    21,    22,
       4,   168,     6,   170,    77,    71,    87,    86,    42,    42,
      21,    22,    21,    22,   103,    88,    50,   170,    86,    99,
      37,    38,    77,    86,    66,    63,   114,    31,    92,   225,
     108,   169,    65,    88,     0,   104,    28,   103,    83,   103,
     104,    79,   106,   107,   122,   124,   122,   111,   112,    83,
      86,    76,   114,   117,   135,   222,   124,   273,    92,    92,
     104,   124,    96,   127,    56,   232,   233,   148,   122,   133,
     123,   123,   123,   301,   138,   109,   109,   103,   266,   122,
      86,   363,   340,   147,   101,    86,   153,   263,   124,   170,
     157,   123,   320,   123,   158,   104,   160,   161,   125,   180,
     123,    62,   183,   114,    66,    86,    86,    29,     3,    67,
      69,   287,   100,    52,    72,    99,   124,   284,   124,    77,
      59,   202,    80,   124,    85,    86,   190,   208,   112,   193,
      77,   284,    54,    52,    76,    94,   115,   134,   122,    97,
      59,    88,   206,   124,   124,   124,   107,   211,   376,    21,
      22,   327,   149,   329,    67,   331,    31,   333,   222,    72,
      32,   225,   243,   244,    77,   122,   124,    80,   249,   118,
     346,   347,   121,     9,   341,   256,     5,     4,     4,     6,
       6,    53,    43,    44,    97,    21,    22,   122,   341,   258,
     118,   358,   120,    86,   258,    21,    22,   115,   279,   263,
     367,   122,   266,     7,   380,    31,     9,     7,   384,   273,
     386,    17,    18,    19,   367,   382,   123,   124,    21,    22,
      43,    44,   122,   287,   123,   124,   123,   124,    75,   382,
      21,    22,     5,   314,   123,   124,   123,   124,   123,   124,
     304,   122,   323,    75,   411,     5,   413,    66,   415,     7,
     417,   124,   123,   124,   123,   124,   123,   124,   411,   340,
     413,   116,   415,   327,   417,   329,   122,   331,   122,   333,
     123,   124,   439,   123,   124,   442,     7,   444,   454,   123,
     124,    11,   346,   347,   123,   124,   439,    99,   122,   442,
       8,   444,   373,     9,   358,    20,   472,    58,   123,   124,
     476,   123,   124,   123,   124,   123,   124,   122,     1,   123,
       3,    66,   123,   123,    68,   123,   380,   122,   111,    78,
     384,    78,   386,   122,   112,    58,    77,   408,     7,     7,
      39,    70,   122,   122,     5,    27,   400,    30,   419,   124,
     116,    83,   122,   407,    78,    83,    91,   122,    77,    77,
      77,    91,    45,    46,   122,    87,   122,   124,    51,     4,
       5,     6,     7,    56,    39,    35,    89,   431,   432,    81,
       5,    39,   122,     7,    96,   123,    21,    22,    23,    84,
      73,   124,    90,   464,     7,   449,   122,   451,    33,    34,
     454,   307,   432,   167,   451,    40,    41,   206,   122,   464,
      93,   104,    47,    48,    49,   268,    99,   266,   472,    54,
      55,   232,   476,   139,   135,   168,    61,   110,   101,    64,
     233,   123,   376,   116,   144,   118,   398,    -1,    -1,    74,
      75,    -1,   181,    -1,   181,    -1,    -1,    82,    83,    -1,
      -1,    -1,    -1,    -1,    -1,     4,     5,     6,     7,    -1,
      95,    10,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,
     105,   106,    21,    22,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,   117,    -1,    33,    34,    -1,   122,   123,    -1,
      -1,    40,    41,    -1,    -1,    -1,    -1,    -1,    47,    48,
      49,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    -1,     4,     5,
       6,     7,    -1,    -1,    -1,    74,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    21,    22,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    33,    34,    -1,
      -1,    -1,    -1,   102,    40,    41,   105,   106,    -1,    -1,
      -1,    47,    48,    49,   113,    -1,    -1,    -1,   117,    55,
      -1,    -1,    -1,   122,    -1,    61,    -1,    -1,    64,    -1,
      -1,     4,     5,     6,     7,    -1,    -1,    -1,    74,    75,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      33,    34,    -1,    -1,    -1,    -1,   102,    40,    41,   105,
     106,    -1,    -1,    -1,    47,    48,    49,   113,    -1,    -1,
      -1,   117,    55,    -1,    -1,    -1,   122,    -1,    61,    -1,
      -1,    64,    -1,    -1,     4,     5,     6,     7,    -1,    -1,
      -1,    74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    33,    34,    -1,    -1,    -1,    -1,   102,
      40,    41,   105,   106,    -1,    -1,    -1,    47,    48,    49,
     113,   114,    -1,    -1,   117,    55,    -1,    -1,    -1,   122,
      60,    61,    -1,    -1,    64,    -1,    -1,     4,     5,     6,
       7,    -1,    -1,    -1,    74,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    33,    34,    -1,    -1,
      -1,    -1,   102,    40,    41,   105,   106,    -1,    -1,    -1,
      47,    48,    49,   113,    -1,    -1,    -1,   117,    55,    -1,
      -1,    -1,   122,    -1,    61,    -1,    -1,    64,    -1,    -1,
       4,     5,     6,     7,    -1,    -1,    -1,    74,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    83,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    33,
      34,    -1,    -1,    -1,    -1,   102,    40,    41,   105,   106,
      -1,    -1,    -1,    47,    48,    49,   113,    -1,    -1,    -1,
     117,    55,    -1,    -1,    -1,   122,    -1,    61,    -1,    -1,
      64,    -1,    -1,    -1,     5,    -1,     7,    -1,    -1,    -1,
      74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    33,    34,    -1,    -1,    -1,    -1,   102,    -1,
      41,   105,   106,    -1,    -1,    -1,    47,    48,    49,   113,
      -1,    -1,    -1,   117,    55,    -1,    -1,    -1,   122,    -1,
      -1,    33,    34,    64,    -1,    -1,    -1,    -1,    -1,    41,
      -1,    -1,    -1,    74,    75,    47,    48,    49,    -1,    -1,
      -1,    82,    83,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    74,    75,   105,   106,    -1,    -1,    -1,    -1,
      82,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
     102,    -1,    -1,   105,   106,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,   117,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    -1,    21,    22
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,    30,    45,    46,    51,    56,    73,    93,
      99,   110,   116,   118,   127,   128,   129,   130,   131,   132,
     133,   154,   155,   158,   159,   162,   163,   166,   219,   220,
     103,     4,     6,   217,    71,   103,    66,   103,    76,    29,
      54,   167,   217,   164,   165,   179,   217,     0,   118,   120,
      73,   166,   118,   121,     3,   217,    66,   217,   217,   217,
     217,   217,     5,     7,    21,    22,    23,    33,    34,    40,
      41,    47,    48,    49,    55,    61,    64,    74,    75,    82,
      83,    95,   102,   105,   106,   113,   117,   122,   136,   168,
     169,   170,   195,   196,   197,   198,   199,   200,   201,   208,
     210,   213,   217,   100,   124,    31,   122,    76,    28,    56,
       5,    86,   122,   115,   189,   190,   112,   122,   166,     7,
       7,   114,   195,   204,   205,   122,    75,   122,     5,   122,
      75,   195,     5,    66,   171,   124,    21,    22,    31,   214,
     217,    23,    24,   125,   215,    22,   198,    27,   122,   160,
     161,   217,   165,   122,   175,   216,   217,   217,    43,    44,
      43,    44,   116,   156,   217,   134,   135,   217,    10,    60,
     122,   191,   192,   193,   194,   195,   210,   122,   216,   191,
     114,   202,   203,    57,   205,   206,     7,   217,     7,   123,
     175,   177,   180,   199,   217,   189,   170,   217,   196,   197,
     217,    23,    54,   123,   195,   207,   124,   189,    11,   166,
     123,   124,   166,   134,    42,    65,    92,   109,   140,   217,
     217,   122,   122,   143,   123,   124,    75,   136,   194,   175,
     191,   195,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,   212,    20,   208,   209,   123,   104,
     195,   203,   206,   195,    58,   123,    66,   123,    31,   178,
     179,    67,    72,    77,    80,    97,   124,   172,   173,   174,
     178,    36,   108,   176,    68,   181,   123,   207,   123,   124,
     161,   195,   123,   217,   122,    78,    78,   122,    52,    59,
     157,   210,   211,   217,   111,   140,   141,   142,   134,    10,
      42,    50,    83,    92,    96,   109,   137,   138,   139,   123,
     192,   193,    17,    18,    19,   195,   195,    10,    83,   123,
     124,   112,   195,   104,    58,   195,   179,    77,    88,    77,
     216,    77,    88,    77,    88,   177,   174,     7,     7,   178,
      39,    70,   182,   123,   195,   191,   122,   122,   216,     5,
      62,    85,    86,   107,   218,   123,   124,   123,   124,    37,
      38,   101,   152,   124,   116,   144,    83,   122,   208,    78,
     217,   137,   195,     9,    83,   208,   122,   195,   123,   216,
      77,   216,    86,   216,    77,   216,    77,    91,    91,   207,
     191,    87,   183,   123,   216,   216,   123,    52,    59,   210,
     122,   153,   140,    35,    89,   145,   191,   122,     9,   195,
     209,    86,   216,    86,   191,    86,   216,    86,   216,    39,
      81,   184,   123,   123,     5,   218,   147,   148,   149,   150,
     151,   217,   122,    39,   123,   217,   195,   123,   191,    86,
     191,   191,    86,   191,    86,   185,   186,   195,     7,    96,
     123,   124,     7,    29,   122,   217,   147,    69,    94,   146,
     123,   191,   191,   191,   124,    32,    53,   187,   217,   148,
     216,   123,   122,   186,    84,   188,   122,   123,   216,    63,
      79,   216,   123,   123,    90,     7
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   126,   127,   127,   127,   127,   127,   127,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   129,   130,
     130,   130,   130,   131,   132,   133,   134,   135,   135,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   137,   137,   137,
     137,   137,   137,   137,   138,   138,   139,   139,   140,   140,
     140,   140,   141,   141,   142,   142,   143,   143,   144,   144,
     145,   145,   146,   146,   147,   147,   148,   148,   148,   149,
     149,   150,   151,   152,   152,   152,   153,   153,   154,   154,
     154,   154,   155,   156,   156,   157,   157,   157,   157,   158,
     159,   160,   160,   161,   162,   162,   163,   164,   164,   165,
     166,   167,   167,   167,   168,   168,   169,   169,   170,   170,
     170,   171,   172,   172,   173,   173,   174,   174,   174,   174,
     174,   174,   174,   174,   175,   176,   176,   176,   177,   177,
     177,   177,   177,   178,   178,   179,   179,   180,   180,   181,
     181,   182,   182,   183,   183,   184,   184,   185,   185,   186,
     187,   187,   187,   188,   188,   188,   189,   189,   190,   191,
     191,   192,   192,   193,   193,   194,   194,   194,   194,   194,
     194,   194,   195,   195,   196,   196,   197,   197,   198,   198,
     198,   198,   198,   198,   199,   199,   199,   199,   200,   201,
     201,   202,   202,   203,   204,   204,   205,   206,   206,   207,
     207,   208,   208,   208,   208,   208,   208,   208,   209,   209,
     210,   210,   211,   211,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   213,   214,   214,   215,   215,   215,
     216,   216,   217,   217,   218,   218,   218,   218,   219,   220,
     220
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
       2,     4,     2,     1,     1,     1,     0,     3,    10,     7,
       4,     5,     5,     0,     4,     2,     2,     4,     4,     5,
       4,     3,     1,     3,     1,     2,     2,     1,     3,     3,
       9,     0,     1,     1,     1,     1,     1,     3,     3,     2,
       1,     3,     0,     1,     2,     1,     5,     4,     6,     5,
       6,     5,     6,     5,     3,     0,     3,     3,     2,     3,
       2,     2,     1,     1,     2,     1,     4,     1,     3,     0,
       3,     0,     2,     0,     3,     0,     2,     1,     3,     3,
       0,     1,     1,     0,     2,     2,     0,     1,     2,     3,
       1,     3,     1,     2,     1,     5,     6,     4,     3,     3,
       3,     2,     3,     1,     3,     1,     2,     1,     1,     1,
       1,     1,     1,     3,     3,     4,     4,     5,     6,     5,
       4,     1,     2,     4,     1,     2,     4,     0,     2,     1,
       3,     1,     1,     2,     2,     1,     2,     2,     1,     3,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     2,     2,
       0
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
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1845 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_NAME  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1855 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1865 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1875 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 7: /* TOKEN_UNSIGNED_NUMVAL  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 1885 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 128: /* sql_statement  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1895 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 129: /* quit_statement  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 1905 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 130: /* alter_table_statement  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1915 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 131: /* create_table_statement  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 1925 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 132: /* create_index_statement  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1935 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 133: /* drop_table_statement  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 1945 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 134: /* column_def  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 1955 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 135: /* column_def_commalist  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 1965 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 136: /* data_type  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 1975 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 137: /* column_constraint_def  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 1985 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 138: /* column_constraint_def_list  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1995 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 139: /* opt_column_constraint_def_list  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 2005 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 143: /* opt_column_list  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2015 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 144: /* opt_block_properties  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).block_properties_) != nullptr) {
    delete ((*yyvaluep).block_properties_);
  }
}
#line 2025 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 145: /* opt_partition_clause  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).partition_clause_) != nullptr) {
    delete ((*yyvaluep).partition_clause_);
  }
}
#line 2035 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 146: /* partition_type  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2045 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 147: /* key_value_list  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2055 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 148: /* key_value  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 2065 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 149: /* key_string_value  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 2075 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 150: /* key_string_list  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 2085 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 151: /* key_integer_value  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 2095 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 152: /* index_type  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2105 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 153: /* opt_index_properties  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2115 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* insert_statement  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 2125 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* copy_from_statement  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_statement_) != nullptr) {
    delete ((*yyvaluep).copy_from_statement_);
  }
}
#line 2135 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* opt_copy_from_params  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2145 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* copy_from_params  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2155 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* update_statement  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2165 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* delete_statement  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2175 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* assignment_list  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2185 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* assignment_item  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2195 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* select_statement  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_statement_) != nullptr) {
    delete ((*yyvaluep).select_statement_);
  }
}
#line 2205 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 163: /* with_clause  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2215 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 164: /* with_list  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2225 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 165: /* with_list_element  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2235 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 166: /* select_query  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2245 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* selection  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2255 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* selection_item_commalist  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2265 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* selection_item  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2275 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* from_clause  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2285 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* subquery_expression  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2295 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* opt_sample_clause  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_sample_clause_) != nullptr) {
    delete ((*yyvaluep).opt_sample_clause_);
  }
}
#line 2305 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* table_reference  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2315 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* table_reference_signature  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2325 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* table_reference_signature_primary  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2335 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* table_reference_commalist  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2345 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* opt_group_by_clause  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2355 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* opt_having_clause  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2365 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* opt_order_by_clause  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2375 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* opt_limit_clause  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2385 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* order_commalist  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2395 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* order_item  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2405 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* opt_order_direction  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2415 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* opt_nulls_first  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2425 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* opt_where_clause  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2435 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 190: /* where_clause  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2445 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 191: /* or_expression  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2455 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 192: /* and_expression  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2465 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 193: /* not_expression  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2475 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 194: /* predicate_expression_base  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2485 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 195: /* add_expression  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2495 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 196: /* multiply_expression  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2505 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 197: /* unary_expression  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2515 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 198: /* expression_base  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2525 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 199: /* function_call  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2535 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 200: /* extract_function  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2545 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 201: /* case_expression  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2555 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 202: /* simple_when_clause_list  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_list_);
  }
}
#line 2565 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 203: /* simple_when_clause  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).simple_when_clause_) != nullptr) {
    delete ((*yyvaluep).simple_when_clause_);
  }
}
#line 2575 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 204: /* searched_when_clause_list  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_list_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_list_);
  }
}
#line 2585 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 205: /* searched_when_clause  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).searched_when_clause_) != nullptr) {
    delete ((*yyvaluep).searched_when_clause_);
  }
}
#line 2595 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 206: /* opt_else_clause  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2605 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 207: /* expression_list  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2615 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 208: /* literal_value  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2625 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 209: /* literal_value_commalist  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2635 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 210: /* attribute_ref  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2645 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 211: /* attribute_ref_list  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2655 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 212: /* comparison_operation  */
#line 554 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2661 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 213: /* unary_operation  */
#line 555 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2667 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 214: /* add_operation  */
#line 556 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2673 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 215: /* multiply_operation  */
#line 556 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2679 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 216: /* name_commalist  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 2689 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 217: /* any_name  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2699 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 218: /* boolean_value  */
#line 553 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2705 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 219: /* command  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_) != nullptr) {
    delete ((*yyvaluep).command_);
  }
}
#line 2715 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 220: /* command_argument_list  */
#line 558 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_argument_list_) != nullptr) {
    delete ((*yyvaluep).command_argument_list_);
  }
}
#line 2725 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 567 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3022 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 571 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 3031 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 575 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3040 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 579 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 3049 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 583 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    YYABORT;
  }
#line 3057 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 586 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 3066 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 593 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3074 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 596 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].copy_from_statement_);
  }
#line 3082 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 599 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 3090 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 602 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3098 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 605 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 3106 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 608 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 3114 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 611 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 3122 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 614 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 3130 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 617 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].select_statement_);
  }
#line 3138 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 620 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 3146 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 626 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3154 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 632 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3166 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 639 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3177 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 645 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3189 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 652 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3201 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 661 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].attribute_definition_list_), (yyvsp[-1].block_properties_), (yyvsp[0].partition_clause_));
  }
#line 3209 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 666 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].key_value_list_)) {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_), (yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].key_value_list_));
    } else {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_));
    }
  }
#line 3221 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 675 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 3229 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 680 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 3237 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 685 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3246 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 689 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3255 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 695 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 3265 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 700 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3273 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 703 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3281 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 706 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3291 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 711 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3299 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 714 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3307 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 717 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3315 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 720 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3323 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 723 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3331 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 726 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3339 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 729 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3347 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 732 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3355 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 735 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3363 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 738 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3380 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 750 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3388 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 753 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3396 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 756 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3419 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 774 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3442 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 794 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3450 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 797 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3458 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 800 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3468 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 805 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3478 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 810 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3489 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 816 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3500 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 822 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3512 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 831 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3521 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 835 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3530 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 841 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3538 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 844 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3546 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 849 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3556 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 854 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3566 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 859 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3578 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 866 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3588 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 873 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3597 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 877 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3606 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 883 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 3614 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 886 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = $1; */
  }
#line 3622 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 891 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_list_) = nullptr;
  }
#line 3630 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 894 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_list_) = (yyvsp[-1].attribute_list_);
  }
#line 3638 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 899 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.block_properties_) = nullptr;
  }
#line 3646 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 902 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].key_value_list_));
  }
#line 3654 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 907 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.partition_clause_) = nullptr;
  }
#line 3662 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 910 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3684 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 929 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::PartitionSchemeHeader::PartitionType::kHash));
  }
#line 3693 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 933 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::PartitionSchemeHeader::PartitionType::kRange));
  }
#line 3702 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 939 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3711 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 943 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3720 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 949 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 3728 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 952 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 3736 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 955 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 3744 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 960 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 3752 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 963 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_),
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 3762 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 970 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 3770 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 975 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 3784 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 986 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
#line 3793 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 990 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
#line 3802 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 994 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kSMA));
  }
#line 3811 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 1000 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 3819 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 1003 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 3827 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 1009 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 3840 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 1017 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertTuple((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 3848 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 1020 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-3]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), nullptr);
  }
#line 3856 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 1023 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsertSelection((yylsp[-4]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].select_query_), (yyvsp[-4].with_list_));
  }
#line 3864 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 1029 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_statement_) = new quickstep::ParseStatementCopyFrom((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].copy_from_params_));
  }
#line 3872 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 1034 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = nullptr;
  }
#line 3880 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 1037 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-1].copy_from_params_);
  }
#line 3888 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 1042 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3897 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 1046 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3906 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 1050 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3915 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 1054 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3924 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 1060 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 3932 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 1065 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 3940 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 1070 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3949 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 1074 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3958 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 1080 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 3966 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 1086 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].select_query_), nullptr);
  }
#line 3974 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 1089 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].select_query_), (yyvsp[-1].with_list_));
  }
#line 3982 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 1094 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 3990 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 1099 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3999 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 1103 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 4008 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 1109 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 4017 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 1116 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].selection_), (yyvsp[-5].table_reference_list_), (yyvsp[-4].predicate_), (yyvsp[-3].opt_group_by_clause_), (yyvsp[-2].opt_having_clause_), (yyvsp[-1].opt_order_by_clause_), (yyvsp[0].opt_limit_clause_));
  }
#line 4025 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 1121 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 4033 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 1124 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "ALL in selection");
    YYERROR;
  }
#line 4042 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 1128 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "DISTINCT in selection");
    YYERROR;
  }
#line 4051 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 1134 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4059 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 1137 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 4067 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 1142 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4076 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 1146 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 4085 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 1152 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 4093 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 1155 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 4101 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 1158 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 4109 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 1163 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = (yyvsp[-1].table_reference_list_);
  }
#line 4117 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 1168 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 4125 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 1171 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4134 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 1177 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[-1]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4143 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 1181 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4152 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 1187 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4163 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 1193 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4174 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 1199 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4185 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 1205 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4196 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 1211 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4207 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 1217 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4218 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 1223 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4229 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 1229 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4240 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 1237 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].select_query_));
  }
#line 4248 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 1242 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_sample_clause_) = NULL;
  }
#line 4256 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 1245 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, true, (yyvsp[-1].numeric_literal_value_));
  }
#line 4264 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 1248 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].numeric_literal_value_));
  }
#line 4272 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 1253 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4281 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 1257 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].opt_sample_clause_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4290 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 1261 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].opt_sample_clause_));
  }
#line 4298 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 1264 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4307 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 1268 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].function_call_));
  }
#line 4315 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 1273 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4323 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 1276 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4331 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 1281 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4339 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 1284 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4347 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 1289 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4356 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 148:
#line 1293 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4365 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 149:
#line 1299 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 4373 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 150:
#line 1302 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4381 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 1307 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4389 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 1310 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4397 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 1315 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4405 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 1318 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4413 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 1323 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4421 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 1326 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4443 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 1345 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4452 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 158:
#line 1349 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4461 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 159:
#line 1355 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 4471 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 1362 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4479 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 161:
#line 1365 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4487 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 1368 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4495 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 163:
#line 1373 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4503 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 1376 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4511 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 165:
#line 1379 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4519 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 166:
#line 1385 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 4527 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 1388 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4535 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 168:
#line 1393 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4543 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 169:
#line 1398 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4557 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 1407 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4565 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 171:
#line 1412 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4579 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 172:
#line 1421 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4587 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 173:
#line 1426 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4595 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 174:
#line 1429 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4603 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 175:
#line 1434 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4611 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 176:
#line 1437 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 4621 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 177:
#line 1442 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4632 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 178:
#line 1448 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4643 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 1454 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4651 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 180:
#line 1457 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 4659 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 181:
#line 1460 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateExists((yylsp[-1]).first_line,
                                             (yylsp[-1]).first_column,
                                             (yyvsp[0].subquery_expression_));
  }
#line 4669 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 1468 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4677 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 183:
#line 1471 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4685 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 184:
#line 1476 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4693 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 185:
#line 1479 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4701 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 186:
#line 1484 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].unary_operation_), (yyvsp[0].expression_));
  }
#line 4709 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 187:
#line 1487 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4717 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 1492 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 4725 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 189:
#line 1495 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 4733 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 190:
#line 1498 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 4741 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 191:
#line 1501 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4749 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 192:
#line 1504 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4757 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 193:
#line 1507 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 4765 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 194:
#line 1512 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 4774 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 195:
#line 1516 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 4783 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 196:
#line 1520 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, false, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4791 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 197:
#line 1523 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-4]).first_line, (yylsp[-4]).first_column, true, (yyvsp[-4].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4799 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 198:
#line 1528 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseExtractFunction((yylsp[-5]).first_line, (yylsp[-5]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].expression_));
  }
#line 4807 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 199:
#line 1533 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseSimpleCaseExpression((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].expression_), (yyvsp[-2].simple_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 4815 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 200:
#line 1536 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseSearchedCaseExpression((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].searched_when_clause_list_), (yyvsp[-1].expression_));
  }
#line 4823 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 201:
#line 1541 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.simple_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSimpleWhenClause>;
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 4832 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 202:
#line 1545 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.simple_when_clause_list_) = (yyvsp[-1].simple_when_clause_list_);
    (yyval.simple_when_clause_list_)->push_back((yyvsp[0].simple_when_clause_));
  }
#line 4841 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 203:
#line 1551 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.simple_when_clause_) = new quickstep::ParseSimpleWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4849 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 204:
#line 1556 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.searched_when_clause_list_) = new quickstep::PtrVector<quickstep::ParseSearchedWhenClause>;
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 4858 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 205:
#line 1560 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.searched_when_clause_list_) = (yyvsp[-1].searched_when_clause_list_);
    (yyval.searched_when_clause_list_)->push_back((yyvsp[0].searched_when_clause_));
  }
#line 4867 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 206:
#line 1566 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.searched_when_clause_) = new quickstep::ParseSearchedWhenClause((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-2].predicate_), (yyvsp[0].expression_));
  }
#line 4875 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 1571 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = NULL;
  }
#line 4883 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 208:
#line 1574 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4891 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 209:
#line 1579 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4900 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 210:
#line 1583 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4909 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 211:
#line 1589 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4917 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 212:
#line 1592 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4925 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 213:
#line 1595 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4933 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 214:
#line 1598 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4950 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 215:
#line 1610 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 4959 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 216:
#line 1614 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4983 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 217:
#line 1633 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 5001 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 218:
#line 1648 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5010 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 219:
#line 1652 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 5019 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 220:
#line 1658 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 5027 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 221:
#line 1661 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 5035 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 222:
#line 1666 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_list_) = new quickstep::PtrList<quickstep::ParseAttribute>();
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5044 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 223:
#line 1670 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_list_) = (yyvsp[-2].attribute_list_);
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 5053 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 224:
#line 1677 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 5061 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 225:
#line 1680 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 5069 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 226:
#line 1683 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 5077 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 227:
#line 1686 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 5085 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 228:
#line 1689 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 5093 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 229:
#line 1692 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 5101 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 230:
#line 1695 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLike);
  }
#line 5109 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 231:
#line 1698 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotLike);
  }
#line 5117 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 232:
#line 1701 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kRegexMatch);
  }
#line 5125 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 233:
#line 1704 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotRegexMatch);
  }
#line 5133 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 234:
#line 1709 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
#line 5147 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 235:
#line 1720 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
#line 5155 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 236:
#line 1723 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
#line 5163 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 237:
#line 1728 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kModulo);
  }
#line 5171 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 238:
#line 1731 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
#line 5179 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 239:
#line 1734 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
#line 5187 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 240:
#line 1740 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 5196 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 241:
#line 1744 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 5205 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 242:
#line 1750 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5213 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 243:
#line 1753 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 5224 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 244:
#line 1761 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5232 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 245:
#line 1764 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5240 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 246:
#line 1767 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5248 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 247:
#line 1770 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5256 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 248:
#line 1776 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.command_) = new quickstep::ParseCommand((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].command_argument_list_));
  }
#line 5264 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 249:
#line 1781 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    quickstep::PtrVector<quickstep::ParseString> *argument_list = (yyvsp[-1].command_argument_list_);
    argument_list->push_back((yyvsp[0].string_value_));
    (yyval.command_argument_list_) = argument_list;
  }
#line 5274 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 250:
#line 1786 "../SqlParser.ypp" /* yacc.c:1646  */
    { /* Epsilon, an empy match. */
    (yyval.command_argument_list_) = new quickstep::PtrVector<quickstep::ParseString>();
  }
#line 5282 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;


#line 5286 "SqlParser_gen.cpp" /* yacc.c:1646  */
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
