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
#include "parser/ParseWindow.hpp"
#include "parser/ParseLiteralValue.hpp"
#include "parser/ParseOrderBy.hpp"
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

#line 149 "SqlParser_gen.cpp" /* yacc.c:339  */

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
    TOKEN_AGE = 279,
    TOKEN_ALL = 280,
    TOKEN_ALTER = 281,
    TOKEN_AS = 282,
    TOKEN_ASC = 283,
    TOKEN_BIGINT = 284,
    TOKEN_BIT = 285,
    TOKEN_BLOCKPROPERTIES = 286,
    TOKEN_BLOCKSAMPLE = 287,
    TOKEN_BLOOM_FILTER = 288,
    TOKEN_CSB_TREE = 289,
    TOKEN_BY = 290,
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
    TOKEN_ESCAPE_STRINGS = 307,
    TOKEN_EMIT = 308,
    TOKEN_FALSE = 309,
    TOKEN_FIRST = 310,
    TOKEN_FLOAT = 311,
    TOKEN_FOREIGN = 312,
    TOKEN_FROM = 313,
    TOKEN_FULL = 314,
    TOKEN_GROUP = 315,
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
    TOKEN_PERCENT = 335,
    TOKEN_PRIMARY = 336,
    TOKEN_PARTITION = 337,
    TOKEN_QUIT = 338,
    TOKEN_REAL = 339,
    TOKEN_REFERENCES = 340,
    TOKEN_RIGHT = 341,
    TOKEN_ROW_DELIMITER = 342,
    TOKEN_SELECT = 343,
    TOKEN_SET = 344,
    TOKEN_SMALLINT = 345,
    TOKEN_TABLE = 346,
    TOKEN_TIME = 347,
    TOKEN_TIMESTAMP = 348,
    TOKEN_TRUE = 349,
    TOKEN_TUPLESAMPLE = 350,
    TOKEN_UNIQUE = 351,
    TOKEN_UPDATE = 352,
    TOKEN_USING = 353,
    TOKEN_VALUES = 354,
    TOKEN_VARCHAR = 355,
    TOKEN_WHERE = 356,
    TOKEN_WITH = 357,
    TOKEN_WINDOW = 358,
    TOKEN_YEARMONTH = 359,
    TOKEN_EOF = 360,
    TOKEN_LEX_ERROR = 361
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 111 "../SqlParser.ypp" /* yacc.c:355  */

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
  quickstep::ParseBlockProperties *block_properties_;
  quickstep::ParseStatementDropTable *drop_table_statement_;
  quickstep::ParseStatementQuit *quit_statement_;

  const quickstep::Comparison *comparison_;
  const quickstep::UnaryOperation *unary_operation_;
  const quickstep::BinaryOperation *binary_operation_;

  quickstep::ParseFunctionCall *function_call_;
  quickstep::PtrList<quickstep::ParseExpression> *expression_list_;
  quickstep::PtrList<quickstep::ParseExpression> *window_partition_by_list_;

  quickstep::ParseSelect *select_query_;
  quickstep::ParseGroupBy *opt_group_by_clause_;
  quickstep::ParseHaving *opt_having_clause_;
  quickstep::ParseOrderBy *opt_order_by_clause_;
  bool *order_direction_;
  quickstep::ParseLimit *opt_limit_clause_;
  quickstep::ParseSample *opt_sample_clause_;
  quickstep::ParseWindow *opt_window_clause_;

  quickstep::PtrList<quickstep::ParseOrderByItem> *order_commalist_;
  quickstep::ParseOrderByItem *order_item_;

  quickstep::PtrVector<quickstep::ParseSubqueryTableReference> *with_list_;
  quickstep::ParseSubqueryTableReference *with_list_element_;

#line 380 "SqlParser_gen.cpp" /* yacc.c:355  */
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
#line 195 "../SqlParser.ypp" /* yacc.c:358  */

/* This header needs YYSTYPE, which is defined by the %union directive above */
#include "SqlLexer_gen.hpp"
void NotSupported(const YYLTYPE *location, yyscan_t yyscanner, const std::string &feature);

#line 413 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYFINAL  42
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   906

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  117
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  92
/* YYNRULES -- Number of rules.  */
#define YYNRULES  235
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  449

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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
       0,   544,   544,   548,   552,   556,   560,   563,   570,   573,
     576,   579,   582,   585,   588,   591,   594,   597,   603,   609,
     616,   622,   629,   638,   643,   652,   657,   662,   666,   672,
     677,   680,   683,   688,   691,   694,   697,   700,   703,   706,
     709,   712,   715,   727,   730,   733,   751,   771,   774,   777,
     782,   787,   793,   799,   808,   812,   818,   821,   826,   831,
     836,   843,   850,   854,   860,   863,   868,   871,   876,   879,
     884,   888,   894,   897,   900,   905,   908,   915,   920,   931,
     935,   941,   944,   950,   958,   963,   968,   971,   976,   980,
     984,   988,   994,   999,  1004,  1008,  1014,  1020,  1025,  1028,
    1033,  1037,  1043,  1049,  1055,  1058,  1062,  1068,  1071,  1076,
    1080,  1086,  1089,  1092,  1097,  1102,  1105,  1111,  1115,  1121,
    1127,  1133,  1139,  1145,  1151,  1157,  1163,  1171,  1176,  1179,
    1182,  1187,  1191,  1195,  1198,  1202,  1207,  1210,  1215,  1218,
    1223,  1227,  1233,  1236,  1241,  1244,  1249,  1252,  1257,  1260,
    1279,  1282,  1287,  1292,  1297,  1300,  1305,  1310,  1315,  1319,
    1325,  1332,  1335,  1338,  1343,  1346,  1349,  1355,  1358,  1363,
    1368,  1377,  1382,  1391,  1396,  1399,  1404,  1407,  1412,  1418,
    1424,  1427,  1433,  1436,  1441,  1444,  1449,  1452,  1457,  1460,
    1463,  1466,  1471,  1475,  1479,  1484,  1488,  1494,  1497,  1500,
    1503,  1515,  1519,  1538,  1553,  1557,  1563,  1566,  1571,  1575,
    1582,  1585,  1588,  1591,  1594,  1597,  1600,  1603,  1606,  1609,
    1614,  1625,  1628,  1633,  1636,  1642,  1646,  1652,  1655,  1663,
    1666,  1669,  1672,  1678,  1683,  1688
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
  "TOKEN_ADD", "TOKEN_AGE", "TOKEN_ALL", "TOKEN_ALTER", "TOKEN_AS",
  "TOKEN_ASC", "TOKEN_BIGINT", "TOKEN_BIT", "TOKEN_BLOCKPROPERTIES",
  "TOKEN_BLOCKSAMPLE", "TOKEN_BLOOM_FILTER", "TOKEN_CSB_TREE", "TOKEN_BY",
  "TOKEN_CHARACTER", "TOKEN_CHECK", "TOKEN_COLUMN", "TOKEN_CONSTRAINT",
  "TOKEN_COPY", "TOKEN_CREATE", "TOKEN_DATE", "TOKEN_DATETIME",
  "TOKEN_DECIMAL", "TOKEN_DEFAULT", "TOKEN_DELETE", "TOKEN_DELIMITER",
  "TOKEN_DESC", "TOKEN_DISTINCT", "TOKEN_DOUBLE", "TOKEN_DROP",
  "TOKEN_ESCAPE_STRINGS", "TOKEN_EMIT", "TOKEN_FALSE", "TOKEN_FIRST",
  "TOKEN_FLOAT", "TOKEN_FOREIGN", "TOKEN_FROM", "TOKEN_FULL",
  "TOKEN_GROUP", "TOKEN_HAVING", "TOKEN_INDEX", "TOKEN_INNER",
  "TOKEN_INSERT", "TOKEN_INTEGER", "TOKEN_INTERVAL", "TOKEN_INTO",
  "TOKEN_JOIN", "TOKEN_KEY", "TOKEN_LAST", "TOKEN_LEFT", "TOKEN_LIMIT",
  "TOKEN_LONG", "TOKEN_NULL", "TOKEN_NULLS", "TOKEN_OFF", "TOKEN_ON",
  "TOKEN_ORDER", "TOKEN_OUTER", "TOKEN_PERCENT", "TOKEN_PRIMARY",
  "TOKEN_PARTITION", "TOKEN_QUIT", "TOKEN_REAL", "TOKEN_REFERENCES",
  "TOKEN_RIGHT", "TOKEN_ROW_DELIMITER", "TOKEN_SELECT", "TOKEN_SET",
  "TOKEN_SMALLINT", "TOKEN_TABLE", "TOKEN_TIME", "TOKEN_TIMESTAMP",
  "TOKEN_TRUE", "TOKEN_TUPLESAMPLE", "TOKEN_UNIQUE", "TOKEN_UPDATE",
  "TOKEN_USING", "TOKEN_VALUES", "TOKEN_VARCHAR", "TOKEN_WHERE",
  "TOKEN_WITH", "TOKEN_WINDOW", "TOKEN_YEARMONTH", "TOKEN_EOF",
  "TOKEN_LEX_ERROR", "';'", "'\\n'", "'('", "')'", "','", "$accept",
  "start", "sql_statement", "quit_statement", "alter_table_statement",
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
  "opt_sample_clause", "table_reference", "table_reference_signature",
  "table_reference_signature_primary", "table_reference_commalist",
  "opt_group_by_clause", "opt_having_clause", "opt_order_by_clause",
  "opt_limit_clause", "opt_window_clause", "window_duration",
  "window_attribute", "opt_window_partition", "emit_duration",
  "age_duration", "order_commalist", "order_item", "opt_order_direction",
  "opt_nulls_first", "opt_where_clause", "where_clause", "or_expression",
  "and_expression", "not_expression", "predicate_expression_base",
  "add_expression", "multiply_expression", "unary_expression",
  "expression_base", "function_call", "expression_list", "literal_value",
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
     360,   361,    59,    10,    40,    41,    44
};
# endif

#define YYPACT_NINF -325

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-325)))

#define YYTABLE_NINF -99

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      34,  -325,  -325,    24,   209,    -8,   -22,    55,    22,  -325,
     209,   209,  -325,   162,   106,  -325,  -325,  -325,  -325,  -325,
    -325,  -325,  -325,  -325,  -325,    66,   -55,   170,   209,  -325,
    -325,   122,   209,   209,   209,   209,   209,   126,    62,  -325,
     190,   103,  -325,  -325,  -325,    94,  -325,  -325,  -325,  -325,
      86,   222,   155,   110,   149,  -325,    43,   209,   209,   147,
     209,  -325,  -325,   444,   102,   123,   141,   209,   209,   522,
    -325,  -325,   151,   209,    54,  -325,   255,  -325,    66,  -325,
      84,  -325,  -325,  -325,   261,   267,  -325,  -325,  -325,   164,
    -325,   217,  -325,  -325,  -325,  -325,   285,  -325,  -325,  -325,
    -325,  -325,  -325,   183,   228,   574,   295,   238,   188,  -325,
     150,   185,  -325,  -325,  -325,  -325,  -325,   643,   -17,   209,
      -2,   209,   209,   191,  -325,   192,  -325,   115,   797,   695,
     522,   294,   302,  -325,  -325,   866,   292,   764,   117,   209,
    -325,   574,   199,  -325,   209,  -325,  -325,   309,  -325,  -325,
     311,  -325,    -3,  -325,     5,   149,   574,  -325,  -325,   209,
     574,  -325,  -325,  -325,   574,   267,  -325,   209,   392,  -325,
     206,   248,   249,   211,  -325,  -325,  -325,   -14,   209,   224,
      -2,   209,  -325,   133,  -325,    13,   176,   522,   522,   186,
    -325,  -325,  -325,  -325,  -325,  -325,  -325,  -325,   574,   574,
       7,  -325,   120,   227,  -325,   219,  -325,  -325,   218,   221,
    -325,   105,  -325,    85,   105,   -23,   269,  -325,  -325,   185,
    -325,  -325,   223,  -325,   219,   129,   522,   226,   229,   209,
     327,    -7,   131,  -325,   134,   310,   214,  -325,   225,   235,
    -325,   272,   232,   764,  -325,   278,   209,  -325,  -325,   133,
    -325,  -325,   302,  -325,  -325,  -325,   574,   143,   219,   275,
    -325,  -325,   764,   242,  -325,  -325,   209,  -325,  -325,   -37,
     286,   209,   -27,    20,     5,  -325,   138,  -325,  -325,   351,
     353,   105,   321,   297,  -325,  -325,   574,    18,   209,   209,
     142,  -325,  -325,  -325,  -325,  -325,  -325,  -325,    50,  -325,
     209,  -325,  -325,   250,    -2,   329,  -325,  -325,   522,  -325,
    -325,   252,  -325,   159,   574,  -325,  -325,   764,  -325,   209,
     296,   209,   -49,   209,   300,   209,   301,  -325,  -325,   282,
     283,  -325,   574,   522,   293,   219,  -325,   144,   148,  -325,
     370,    -7,  -325,   209,  -325,  -325,   263,    19,   209,   574,
     219,   165,   -48,   209,   -40,   522,   -38,   209,   -33,   209,
    -325,  -325,   262,   294,   341,   305,   304,  -325,  -325,  -325,
     167,  -325,  -325,  -325,  -325,     9,   209,  -325,   271,   219,
    -325,   522,   -21,   522,   294,   522,   -16,   522,   -11,   574,
     377,   284,   209,  -325,   209,  -325,  -325,   209,  -325,   171,
    -325,   294,   522,   294,   294,   522,   294,   522,   279,  -325,
      29,  -325,   764,  -325,   318,   277,  -325,   177,  -325,   294,
     294,   294,   574,  -325,  -325,   322,  -325,   209,   314,   209,
    -325,  -325,    38,  -325,  -325,   363,   347,   179,  -325,  -325,
     574,   764,   379,  -325,   262,  -325,   764,  -325,  -325
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     6,   235,     0,     0,     0,     0,     0,     0,    18,
       0,     0,     7,     0,     0,    15,     8,    10,    11,    13,
      14,     9,    17,    12,    16,     0,     0,   233,     0,   227,
     228,     0,     0,     0,     0,     0,     0,     0,    99,   100,
       0,   138,     1,     3,     2,   104,    97,     5,     4,   234,
       0,     0,     0,     0,   167,    25,     0,     0,     0,     0,
       0,   105,   106,     0,     0,     0,    86,     0,     0,     0,
      93,   168,     0,     0,   167,    95,     0,   101,     0,   102,
       0,   225,   201,   198,     0,   220,   107,    40,    29,     0,
      30,    31,    34,    36,    37,    39,     0,    41,   197,    35,
      38,    32,    33,     0,     0,     0,     0,     0,   108,   109,
     113,   183,   185,   187,   190,   189,   188,     0,   206,     0,
       0,     0,     0,     0,    85,    66,    27,     0,     0,     0,
       0,   169,   171,   173,   175,     0,   188,     0,     0,     0,
      92,     0,     0,   139,     0,   199,   200,     0,    43,   202,
       0,    44,     0,   203,     0,   167,     0,   221,   222,     0,
       0,   112,   223,   224,     0,     0,   186,     0,     0,    19,
       0,     0,     0,     0,    20,    21,    22,     0,     0,     0,
      64,     0,    42,    56,   174,     0,     0,     0,     0,     0,
     210,   212,   213,   214,   215,   211,   216,   218,     0,     0,
       0,   204,     0,     0,    94,    96,   127,   226,     0,     0,
     191,     0,   140,   115,   135,   128,   142,   110,   111,   182,
     184,   207,     0,   192,   195,     0,     0,     0,     0,     0,
       0,     0,     0,   208,     0,   206,     0,    63,    65,    68,
      28,     0,     0,     0,    47,     0,     0,    49,    55,    57,
      26,   181,   170,   172,   217,   219,     0,     0,   180,     0,
     179,    84,     0,     0,    45,    46,     0,   131,   136,     0,
       0,     0,     0,     0,     0,   114,   116,   118,   134,     0,
       0,   133,     0,   144,   193,   194,     0,     0,     0,     0,
       0,    88,   231,   232,   230,   229,    89,    87,     0,    67,
       0,    79,    80,    81,     0,     0,    23,    48,     0,    51,
      50,     0,    54,     0,     0,   178,   205,     0,   137,     0,
       0,     0,     0,     0,     0,     0,     0,   141,   117,     0,
       0,   132,     0,     0,   146,   196,    61,     0,     0,    58,
       0,     0,   209,     0,    24,    62,     0,     0,     0,     0,
     176,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     129,   130,   143,   145,     0,   148,     0,    59,    90,    91,
       0,    70,    72,    73,    74,     0,     0,    52,     0,   177,
      83,     0,     0,     0,   120,     0,     0,     0,     0,     0,
       0,   150,     0,    82,     0,    78,    76,     0,    75,     0,
      53,   126,     0,   119,   122,     0,   124,     0,   147,   158,
     161,   149,     0,   103,     0,     0,    71,     0,    69,   125,
     121,   123,     0,   162,   163,   164,   152,     0,   154,     0,
      77,   159,     0,   160,   153,     0,     0,     0,   165,   166,
       0,     0,     0,    60,   155,   156,     0,   151,   157
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -325,  -325,  -325,  -325,  -325,  -325,  -325,  -325,   -94,  -325,
     288,   157,  -325,  -325,  -178,  -325,  -325,  -325,  -325,    35,
      16,  -325,  -325,  -325,  -325,  -325,  -325,  -325,  -325,  -325,
    -325,  -325,  -325,   273,  -325,  -325,  -325,   359,   340,  -325,
    -325,  -325,   264,  -325,  -325,  -325,   145,   360,  -325,   154,
    -192,   -10,  -325,  -325,  -325,  -325,  -325,  -325,  -325,  -325,
    -325,  -325,  -325,  -325,     0,  -325,  -325,   -54,  -325,     2,
     237,   241,   303,   -60,   270,   274,   317,  -149,  -324,  -133,
     114,   -57,  -325,  -325,  -325,  -325,  -325,   -50,    -4,    95,
    -325,  -325
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    13,    14,    15,    16,    17,    18,    19,   126,   127,
     106,   248,   249,   250,   174,   238,   239,   179,   306,   370,
     371,   372,   373,   374,   303,   344,    20,    21,   124,   232,
      22,    23,    74,    75,    24,    25,    38,    39,    46,    63,
     107,   108,   109,   155,   275,   276,   277,   211,   281,   212,
     267,   268,   213,   283,   334,   365,   391,   413,   414,   428,
     436,   442,   447,   408,   409,   425,   433,    70,    71,   131,
     132,   133,   134,   135,   111,   112,   113,   114,   225,   115,
     202,   116,   234,   199,   117,   160,   164,    80,   118,   296,
      26,    27
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      31,    40,   237,   110,   201,   214,    37,    41,   362,    29,
     167,    30,   136,    29,   279,    30,   395,   259,   157,   158,
     140,   187,   278,   138,    50,   169,   187,   187,    52,    53,
      54,    55,    56,   355,   381,     1,   319,     2,   230,   396,
     170,    34,   383,   231,   385,   152,   323,   320,    40,   387,
     157,   158,   292,    76,    41,    47,    81,   324,    48,    32,
     171,   402,   423,   125,   128,     3,   405,   144,   144,    81,
     186,   407,   136,   136,   293,   294,   144,   280,   144,     4,
       5,   205,   424,   144,   172,     6,   260,   240,    33,   331,
       7,   168,   295,   325,    36,   144,   110,   168,   438,   173,
     144,   216,   340,     8,   326,   144,   161,   341,   224,    29,
     309,    30,   210,   439,    64,   128,   444,   175,   176,    78,
      28,   233,     9,   397,    61,   214,   345,   -98,   251,   316,
     136,   136,   185,   336,   377,    76,    10,   266,   257,   258,
     207,    11,    65,   241,    12,   119,   120,    72,    62,   269,
     215,    35,   314,   270,    29,   218,    30,    73,   271,    45,
      69,   272,    42,   221,   157,   158,   121,   122,   349,   136,
     139,   157,   158,    49,   235,   242,   273,   128,    58,   290,
     157,   158,   159,   243,   201,    51,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   313,   157,   158,   143,
     144,   274,   269,   254,   255,   256,   270,    41,   162,   163,
      41,   271,   244,    29,   272,    30,    43,    60,    44,   245,
      57,   322,    59,   246,    68,    81,   335,    66,   287,   273,
     180,   181,   203,   144,   247,   261,   262,    67,   337,   338,
     157,   158,   311,   342,   285,   286,   297,   298,   123,   299,
     300,   136,   301,   302,   350,    69,   318,   339,   144,   366,
     144,    78,    41,   367,   144,   137,   141,    81,   145,   352,
     215,   354,   224,   356,   146,   358,   136,    41,   147,   426,
     380,   262,   393,   394,    81,    81,   418,   394,   148,   379,
     149,   210,   430,   144,   443,   144,   235,   150,   136,   151,
     153,   154,   187,   382,   156,   177,   178,   386,   445,   388,
     347,   188,   200,   448,   206,    81,   208,    81,   209,    81,
     226,    81,   227,   228,   136,   229,   136,   236,   136,   410,
     136,   263,   291,   264,   282,   363,   265,   167,   284,   375,
     288,   304,   305,   289,   378,   136,   308,   417,   136,    81,
     136,   307,   310,    81,   315,    81,   317,   384,   329,   321,
     330,   332,   410,   333,   343,   346,   348,   360,   361,   353,
     434,   398,   375,   357,   359,   368,   364,   376,   286,   437,
     224,   389,   390,   401,   411,   403,   400,   404,   415,   406,
     375,   429,   412,    81,   392,   422,    29,    82,    30,    83,
     427,   435,   432,   440,   419,   441,   312,   420,   446,   421,
     416,   399,   204,    84,    85,   222,   183,    77,   142,    79,
     217,   328,   431,   235,   252,    81,    87,    88,   327,   253,
     219,   351,   184,    89,   166,     0,   369,     0,   220,    90,
      91,    92,     0,     0,     0,     0,     0,    93,    29,    82,
      30,    83,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,     0,    84,    85,    86,     0,     0,
      97,    98,     0,     0,     0,     0,     0,     0,    87,    88,
       0,    99,     0,     0,     0,    89,     0,   100,     0,   101,
     102,    90,    91,    92,     0,     0,     0,   103,     0,    93,
       0,   104,     0,     0,     0,    94,   105,   223,     0,     0,
       0,     0,     0,     0,    95,    96,     0,     0,     0,     0,
       0,     0,    97,    98,     0,     0,    29,    82,    30,    83,
       0,     0,   129,    99,     0,     0,     0,     0,     0,   100,
       0,   101,   102,    84,    85,     0,     0,     0,     0,   103,
       0,     0,     0,   104,     0,     0,    87,    88,   105,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,    90,
      91,    92,     0,     0,     0,     0,     0,    93,    29,    82,
      30,    83,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,     0,    84,    85,     0,     0,     0,
      97,    98,     0,     0,     0,     0,     0,     0,    87,    88,
       0,    99,     0,     0,     0,    89,     0,   100,     0,   101,
     102,    90,    91,    92,     0,     0,     0,   103,     0,    93,
       0,   104,     0,     0,     0,    94,   130,     0,     0,     0,
       0,     0,     0,     0,    95,    96,     0,    29,    82,    30,
      83,     0,    97,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,    84,   165,     0,     0,     0,   100,
       0,   101,   102,     0,     0,     0,     0,    87,    88,   103,
       0,     0,     0,   104,    89,     0,     0,     0,   105,     0,
      90,    91,    92,     0,     0,     0,     0,     0,    93,    29,
      82,    30,    83,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,     0,    84,    85,     0,     0,
       0,    97,    98,     0,     0,     0,     0,     0,     0,    87,
      88,     0,    99,     0,     0,     0,    89,     0,   100,     0,
     101,   102,    90,    91,    92,     0,     0,     0,   103,     0,
      93,     0,   104,     0,     0,     0,    94,   105,     0,     0,
       0,     0,     0,     0,     0,    95,    96,     0,     0,    82,
       0,    83,     0,    97,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,    84,   165,     0,     0,     0,
     100,     0,   101,   102,     0,     0,     0,     0,    87,    88,
     103,     0,     0,     0,   104,    89,     0,     0,     0,   130,
       0,    90,    91,    92,     0,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,    87,    88,     0,    95,    96,     0,     0,    89,     0,
       0,     0,    97,    98,    90,    91,    92,     0,     0,     0,
       0,     0,    93,    99,     0,     0,     0,     0,    94,   100,
       0,   101,   102,     0,     0,     0,     0,    95,   182,   103,
       0,     0,     0,   104,     0,    97,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,    99,   157,   158,     0,
       0,     0,   100,     0,   101,   102,     0,     0,     0,     0,
       0,     0,   103,     0,     0,     0,   104
};

static const yytype_int16 yycheck[] =
{
       4,    11,   180,    63,   137,   154,    10,    11,   332,     4,
      27,     6,    69,     4,    37,     6,     7,    10,    21,    22,
      74,     8,   214,    73,    28,   119,     8,     8,    32,    33,
      34,    35,    36,    82,    82,     1,    73,     3,    52,    30,
      42,    63,    82,    57,    82,   105,    73,    84,    58,    82,
      21,    22,    59,    57,    58,   110,    60,    84,   113,    67,
      62,    82,    33,    67,    68,    31,    82,   116,   116,    73,
     130,    82,   129,   130,    81,    82,   116,   100,   116,    45,
      46,   141,    53,   116,    86,    51,    79,   181,    96,   281,
      56,   114,    99,    73,    72,   116,   156,   114,    60,   101,
     116,   155,    52,    69,    84,   116,   110,    57,   168,     4,
     243,     6,   115,    75,    28,   119,   440,   121,   122,   114,
      96,   178,    88,   114,    30,   274,   304,    93,   115,   262,
     187,   188,   130,   115,   115,   139,   102,    32,   198,   199,
     144,   107,    56,    10,   110,    43,    44,   104,    54,    64,
     154,    96,     9,    68,     4,   159,     6,   114,    73,    93,
     106,    76,     0,   167,    21,    22,    43,    44,     9,   226,
     116,    21,    22,     3,   178,    42,    91,   181,   116,   229,
      21,    22,    32,    50,   317,    63,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,   256,    21,    22,   115,
     116,   116,    64,    17,    18,    19,    68,   211,    23,    24,
     214,    73,    79,     4,    76,     6,   110,   114,   112,    86,
      94,   271,    32,    90,   114,   229,   286,     5,   226,    91,
     115,   116,   115,   116,   101,   115,   116,    82,   288,   289,
      21,    22,   246,   300,   115,   116,   115,   116,   107,   115,
     116,   308,    38,    39,   314,   106,   266,   115,   116,   115,
     116,   114,   266,   115,   116,   114,    11,   271,     7,   319,
     274,   321,   332,   323,     7,   325,   333,   281,   114,   412,
     115,   116,   115,   116,   288,   289,   115,   116,    71,   349,
       5,   115,   115,   116,   115,   116,   300,   114,   355,    71,
       5,    63,     8,   353,   116,   114,   114,   357,   441,   359,
     308,     9,    20,   446,   115,   319,     7,   321,     7,   323,
     114,   325,    74,    74,   381,   114,   383,   103,   385,   389,
     387,   104,     5,   115,    65,   333,   115,    27,   115,   343,
     114,   116,   107,   114,   348,   402,   114,   397,   405,   353,
     407,    79,    74,   357,    79,   359,   114,   355,     7,    73,
       7,    40,   422,    66,   114,    36,   114,    85,    85,    73,
     427,   375,   376,    73,    73,     5,    83,   114,   116,   429,
     440,    40,    77,   381,     7,   383,   115,   385,   392,   387,
     394,   114,   108,   397,    90,   116,     4,     5,     6,     7,
      82,    87,    80,    40,   402,    58,   249,   405,    29,   407,
     394,   376,   139,    21,    22,    23,   128,    58,    78,    59,
     156,   276,   422,   427,   187,   429,    34,    35,   274,   188,
     160,   317,   129,    41,   117,    -1,   341,    -1,   164,    47,
      48,    49,    -1,    -1,    -1,    -1,    -1,    55,     4,     5,
       6,     7,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    21,    22,    23,    -1,    -1,
      78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    34,    35,
      -1,    89,    -1,    -1,    -1,    41,    -1,    95,    -1,    97,
      98,    47,    48,    49,    -1,    -1,    -1,   105,    -1,    55,
      -1,   109,    -1,    -1,    -1,    61,   114,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    -1,    -1,     4,     5,     6,     7,
      -1,    -1,    10,    89,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    97,    98,    21,    22,    -1,    -1,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,    34,    35,   114,    -1,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    47,
      48,    49,    -1,    -1,    -1,    -1,    -1,    55,     4,     5,
       6,     7,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    21,    22,    -1,    -1,    -1,
      78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    34,    35,
      -1,    89,    -1,    -1,    -1,    41,    -1,    95,    -1,    97,
      98,    47,    48,    49,    -1,    -1,    -1,   105,    -1,    55,
      -1,   109,    -1,    -1,    -1,    61,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,     4,     5,     6,
       7,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    21,    22,    -1,    -1,    -1,    95,
      -1,    97,    98,    -1,    -1,    -1,    -1,    34,    35,   105,
      -1,    -1,    -1,   109,    41,    -1,    -1,    -1,   114,    -1,
      47,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,     4,
       5,     6,     7,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    21,    22,    -1,    -1,
      -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      35,    -1,    89,    -1,    -1,    -1,    41,    -1,    95,    -1,
      97,    98,    47,    48,    49,    -1,    -1,    -1,   105,    -1,
      55,    -1,   109,    -1,    -1,    -1,    61,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,     5,
      -1,     7,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    21,    22,    -1,    -1,    -1,
      95,    -1,    97,    98,    -1,    -1,    -1,    -1,    34,    35,
     105,    -1,    -1,    -1,   109,    41,    -1,    -1,    -1,   114,
      -1,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    70,    71,    -1,    -1,    41,    -1,
      -1,    -1,    78,    79,    47,    48,    49,    -1,    -1,    -1,
      -1,    -1,    55,    89,    -1,    -1,    -1,    -1,    61,    95,
      -1,    97,    98,    -1,    -1,    -1,    -1,    70,    71,   105,
      -1,    -1,    -1,   109,    -1,    78,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    89,    21,    22,    -1,
      -1,    -1,    95,    -1,    97,    98,    -1,    -1,    -1,    -1,
      -1,    -1,   105,    -1,    -1,    -1,   109
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,    31,    45,    46,    51,    56,    69,    88,
     102,   107,   110,   118,   119,   120,   121,   122,   123,   124,
     143,   144,   147,   148,   151,   152,   207,   208,    96,     4,
       6,   205,    67,    96,    63,    96,    72,   205,   153,   154,
     168,   205,     0,   110,   112,    93,   155,   110,   113,     3,
     205,    63,   205,   205,   205,   205,   205,    94,   116,    32,
     114,    30,    54,   156,    28,    56,     5,    82,   114,   106,
     184,   185,   104,   114,   149,   150,   205,   154,   114,   164,
     204,   205,     5,     7,    21,    22,    23,    34,    35,    41,
      47,    48,    49,    55,    61,    70,    71,    78,    79,    89,
      95,    97,    98,   105,   109,   114,   127,   157,   158,   159,
     190,   191,   192,   193,   194,   196,   198,   201,   205,    43,
      44,    43,    44,   107,   145,   205,   125,   126,   205,    10,
     114,   186,   187,   188,   189,   190,   198,   114,   204,   116,
     184,    11,   155,   115,   116,     7,     7,   114,    71,     5,
     114,    71,   190,     5,    63,   160,   116,    21,    22,    32,
     202,   205,    23,    24,   203,    22,   193,    27,   114,   125,
      42,    62,    86,   101,   131,   205,   205,   114,   114,   134,
     115,   116,    71,   127,   189,   186,   190,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,   200,
      20,   196,   197,   115,   150,   190,   115,   205,     7,     7,
     115,   164,   166,   169,   194,   205,   184,   159,   205,   191,
     192,   205,    23,   115,   190,   195,   114,    74,    74,   114,
      52,    57,   146,   198,   199,   205,   103,   131,   132,   133,
     125,    10,    42,    50,    79,    86,    90,   101,   128,   129,
     130,   115,   187,   188,    17,    18,    19,   190,   190,    10,
      79,   115,   116,   104,   115,   115,    32,   167,   168,    64,
      68,    73,    76,    91,   116,   161,   162,   163,   167,    37,
     100,   165,    65,   170,   115,   115,   116,   186,   114,   114,
     204,     5,    59,    81,    82,    99,   206,   115,   116,   115,
     116,    38,    39,   141,   116,   107,   135,    79,   114,   196,
      74,   205,   128,   190,     9,    79,   196,   114,   168,    73,
      84,    73,   204,    73,    84,    73,    84,   166,   163,     7,
       7,   167,    40,    66,   171,   190,   115,   204,   204,   115,
      52,    57,   198,   114,   142,   131,    36,   186,   114,     9,
     190,   197,   204,    73,   204,    82,   204,    73,   204,    73,
      85,    85,   195,   186,    83,   172,   115,   115,     5,   206,
     136,   137,   138,   139,   140,   205,   114,   115,   205,   190,
     115,    82,   204,    82,   186,    82,   204,    82,   204,    40,
      77,   173,    90,   115,   116,     7,    30,   114,   205,   136,
     115,   186,    82,   186,   186,    82,   186,    82,   180,   181,
     190,     7,   108,   174,   175,   205,   137,   204,   115,   186,
     186,   186,   116,    33,    53,   182,   196,    82,   176,   114,
     115,   181,    80,   183,   198,    87,   177,   204,    60,    75,
      40,    58,   178,   115,   195,   196,    29,   179,   196
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   117,   118,   118,   118,   118,   118,   118,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   120,   121,
     121,   121,   121,   122,   123,   124,   125,   126,   126,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   128,   128,   128,
     128,   128,   128,   128,   129,   129,   130,   130,   131,   131,
     131,   131,   132,   132,   133,   133,   134,   134,   135,   135,
     136,   136,   137,   137,   137,   138,   138,   139,   140,   141,
     141,   142,   142,   143,   143,   144,   145,   145,   146,   146,
     146,   146,   147,   148,   149,   149,   150,   151,   152,   152,
     153,   153,   154,   155,   156,   156,   156,   157,   157,   158,
     158,   159,   159,   159,   160,   161,   161,   162,   162,   163,
     163,   163,   163,   163,   163,   163,   163,   164,   165,   165,
     165,   166,   166,   166,   166,   166,   167,   167,   168,   168,
     169,   169,   170,   170,   171,   171,   172,   172,   173,   173,
     174,   174,   175,   176,   177,   177,   178,   179,   180,   180,
     181,   182,   182,   182,   183,   183,   183,   184,   184,   185,
     186,   186,   187,   187,   188,   188,   189,   189,   189,   189,
     189,   189,   190,   190,   191,   191,   192,   192,   193,   193,
     193,   193,   194,   194,   194,   195,   195,   196,   196,   196,
     196,   196,   196,   196,   197,   197,   198,   198,   199,   199,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     201,   202,   202,   203,   203,   204,   204,   205,   205,   206,
     206,   206,   206,   207,   208,   208
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     6,
       6,     6,     6,     8,     9,     3,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     4,     4,     1,     2,     1,
       2,     2,     4,     5,     2,     1,     0,     1,     4,     5,
      10,     4,     3,     1,     0,     1,     0,     3,     0,     5,
       1,     3,     1,     1,     1,     2,     2,     4,     2,     1,
       1,     0,     3,    10,     7,     5,     0,     4,     2,     2,
       4,     4,     5,     4,     3,     1,     3,     2,     0,     2,
       1,     3,     3,    10,     0,     1,     1,     1,     1,     1,
       3,     3,     2,     1,     3,     0,     1,     2,     1,     5,
       4,     6,     5,     6,     5,     6,     5,     3,     0,     3,
       3,     2,     3,     2,     2,     1,     1,     2,     1,     4,
       1,     3,     0,     3,     0,     2,     0,     3,     0,     2,
       0,     5,     2,     2,     0,     3,     2,     2,     1,     3,
       3,     0,     1,     1,     0,     2,     2,     0,     1,     2,
       3,     1,     3,     1,     2,     1,     5,     6,     4,     3,
       3,     3,     3,     1,     3,     1,     2,     1,     1,     1,
       1,     3,     3,     4,     4,     1,     3,     1,     1,     2,
       2,     1,     2,     2,     1,     3,     1,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     2,
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
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1779 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_NAME  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1789 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1799 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1809 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 7: /* TOKEN_UNSIGNED_NUMVAL  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 1819 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 119: /* sql_statement  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1829 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 120: /* quit_statement  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 1839 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 121: /* alter_table_statement  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1849 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 122: /* create_table_statement  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 1859 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 123: /* create_index_statement  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1869 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 124: /* drop_table_statement  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 1879 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 125: /* column_def  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 1889 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 126: /* column_def_commalist  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 1899 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 127: /* data_type  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 1909 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 128: /* column_constraint_def  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 1919 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 129: /* column_constraint_def_list  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1929 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 130: /* opt_column_constraint_def_list  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1939 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 134: /* opt_column_list  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 1949 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 135: /* opt_block_properties  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).block_properties_) != nullptr) {
    delete ((*yyvaluep).block_properties_);
  }
}
#line 1959 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 136: /* key_value_list  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 1969 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 137: /* key_value  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 1979 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 138: /* key_string_value  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 1989 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 139: /* key_string_list  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 1999 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 140: /* key_integer_value  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 2009 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 141: /* index_type  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2019 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 142: /* opt_index_properties  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2029 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 143: /* insert_statement  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 2039 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 144: /* copy_from_statement  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_statement_) != nullptr) {
    delete ((*yyvaluep).copy_from_statement_);
  }
}
#line 2049 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 145: /* opt_copy_from_params  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2059 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 146: /* copy_from_params  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2069 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 147: /* update_statement  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2079 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 148: /* delete_statement  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2089 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 149: /* assignment_list  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2099 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 150: /* assignment_item  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2109 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 151: /* select_statement  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_statement_) != nullptr) {
    delete ((*yyvaluep).select_statement_);
  }
}
#line 2119 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 152: /* opt_with_clause  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2129 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 153: /* with_list  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2139 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* with_list_element  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2149 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* select_query  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2159 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* selection  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2169 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 158: /* selection_item_commalist  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2179 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* selection_item  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2189 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* from_clause  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2199 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 164: /* subquery_expression  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2209 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 165: /* opt_sample_clause  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_sample_clause_) != nullptr) {
    delete ((*yyvaluep).opt_sample_clause_);
  }
}
#line 2219 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 166: /* table_reference  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2229 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 167: /* table_reference_signature  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2239 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* table_reference_signature_primary  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2249 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* table_reference_commalist  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2259 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* opt_group_by_clause  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2269 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* opt_having_clause  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2279 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* opt_order_by_clause  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2289 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* opt_limit_clause  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2299 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* opt_window_clause  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_window_clause_) != nullptr) {
    delete ((*yyvaluep).opt_window_clause_);
  }
}
#line 2309 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* window_duration  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2319 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* window_attribute  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2329 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* opt_window_partition  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).window_partition_by_list_) != nullptr) {
    delete ((*yyvaluep).window_partition_by_list_);
  }
}
#line 2339 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* emit_duration  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2349 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* age_duration  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2359 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* order_commalist  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2369 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* order_item  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2379 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* opt_order_direction  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2389 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* opt_nulls_first  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2399 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* opt_where_clause  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2409 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* where_clause  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2419 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* or_expression  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2429 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* and_expression  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2439 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* not_expression  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2449 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* predicate_expression_base  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2459 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 190: /* add_expression  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2469 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 191: /* multiply_expression  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2479 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 192: /* unary_expression  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2489 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 193: /* expression_base  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2499 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 194: /* function_call  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2509 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 195: /* expression_list  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2519 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 196: /* literal_value  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2529 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 197: /* literal_value_commalist  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2539 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 198: /* attribute_ref  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2549 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 199: /* attribute_ref_list  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_list_) != nullptr) {
    delete ((*yyvaluep).attribute_list_);
  }
}
#line 2559 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 200: /* comparison_operation  */
#line 531 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2565 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 201: /* unary_operation  */
#line 532 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2571 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 202: /* add_operation  */
#line 533 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2577 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 203: /* multiply_operation  */
#line 533 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2583 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 204: /* name_commalist  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 2593 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 205: /* any_name  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2603 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 206: /* boolean_value  */
#line 530 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2609 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 207: /* command  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_) != nullptr) {
    delete ((*yyvaluep).command_);
  }
}
#line 2619 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 208: /* command_argument_list  */
#line 535 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_argument_list_) != nullptr) {
    delete ((*yyvaluep).command_argument_list_);
  }
}
#line 2629 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 544 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2926 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 548 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2935 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 552 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 2944 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 556 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 2953 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 560 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    YYABORT;
  }
#line 2961 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 563 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 2970 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 570 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 2978 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 573 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].copy_from_statement_);
  }
#line 2986 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 576 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 2994 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 579 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 3002 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 582 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 3010 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 585 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 3018 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 588 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 3026 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 591 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 3034 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 594 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].select_statement_);
  }
#line 3042 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 597 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 3050 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 603 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3058 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 609 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3070 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 616 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3081 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 622 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3093 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 629 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3105 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 638 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-7]).first_line, (yylsp[-7]).first_column, (yyvsp[-5].string_value_), (yyvsp[-3].attribute_definition_list_), (yyvsp[0].block_properties_));
  }
#line 3113 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 643 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].key_value_list_)) {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_), (yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].key_value_list_));
    } else {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].attribute_list_), (yyvsp[-1].string_value_));
    }
  }
#line 3125 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 652 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 3133 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 657 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 3141 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 662 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3150 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 666 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3159 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 672 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 3169 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 677 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3177 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 680 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3185 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 683 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3195 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 688 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3203 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 691 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3211 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 694 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3219 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 697 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3227 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 700 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3235 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 703 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3243 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 706 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3251 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 709 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3259 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 712 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3267 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 715 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3284 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 727 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3292 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 730 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3300 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 733 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3323 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 751 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3346 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 771 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3354 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 774 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3362 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 777 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3372 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 782 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3382 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 787 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3393 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 793 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3404 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 799 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3416 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 808 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3425 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 812 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3434 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 818 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3442 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 821 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3450 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 826 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3460 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 831 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3470 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 836 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3482 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 843 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3492 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 850 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3501 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 854 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3510 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 860 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 3518 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 863 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = $1; */
  }
#line 3526 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 868 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_list_) = nullptr;
  }
#line 3534 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 871 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_list_) = (yyvsp[-1].attribute_list_);
  }
#line 3542 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 876 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.block_properties_) = nullptr;
  }
#line 3550 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 879 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].key_value_list_));
  }
#line 3558 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 884 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3567 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 888 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3576 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 894 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 3584 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 897 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 3592 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 900 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 3600 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 905 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 3608 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 908 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_),
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 3618 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 915 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 3626 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 920 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 3640 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 931 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
#line 3649 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 935 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
#line 3658 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 941 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 3666 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 944 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 3674 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 950 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 3687 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 958 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsert((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 3695 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 963 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_statement_) = new quickstep::ParseStatementCopyFrom((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].copy_from_params_));
  }
#line 3703 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 968 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = nullptr;
  }
#line 3711 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 971 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-1].copy_from_params_);
  }
#line 3719 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 976 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3728 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 980 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3737 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 984 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3746 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 988 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3755 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 994 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 3763 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 999 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 3771 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 1004 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3780 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 1008 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3789 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 1014 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 3797 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 1020 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].select_query_), (yyvsp[-1].with_list_));
  }
#line 3805 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 1025 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = nullptr;
  }
#line 3813 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 1028 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 3821 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 1033 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3830 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 1037 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3839 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 1043 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 3848 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 1050 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-9]).first_line, (yylsp[-9]).first_column, (yyvsp[-7].selection_), (yyvsp[-6].table_reference_list_), (yyvsp[-5].predicate_), (yyvsp[-4].opt_group_by_clause_), (yyvsp[-3].opt_having_clause_), (yyvsp[-2].opt_order_by_clause_), (yyvsp[-1].opt_limit_clause_), (yyvsp[0].opt_window_clause_));
  }
#line 3856 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 1055 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 3864 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 1058 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "ALL in selection");
    YYERROR;
  }
#line 3873 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 1062 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "DISTINCT in selection");
    YYERROR;
  }
#line 3882 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 1068 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3890 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 1071 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 3898 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 1076 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3907 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 1080 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3916 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 1086 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 3924 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 1089 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 3932 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 1092 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 3940 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 1097 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = (yyvsp[-1].table_reference_list_);
  }
#line 3948 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 1102 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 3956 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 1105 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3965 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 1111 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[-1]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3974 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 1115 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3983 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 1121 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3994 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 1127 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 4005 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 1133 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4016 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 1139 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4027 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 1145 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4038 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 1151 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4049 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 1157 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4060 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 1163 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 4071 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 1171 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].select_query_));
  }
#line 4079 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 1176 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_sample_clause_) = NULL;
  }
#line 4087 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 1179 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, true, (yyvsp[-1].numeric_literal_value_));
  }
#line 4095 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 1182 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].numeric_literal_value_));
  }
#line 4103 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 1187 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4112 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 1191 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].opt_sample_clause_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4121 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 1195 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].opt_sample_clause_));
  }
#line 4129 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 1198 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4138 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 1202 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].function_call_));
  }
#line 4146 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 1207 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4154 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 1210 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4162 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 1215 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4170 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 1218 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4178 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 1223 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4187 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 1227 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4196 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 1233 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 4204 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 1236 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4212 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 1241 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4220 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 1244 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4228 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 1249 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4236 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 1252 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4244 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 148:
#line 1257 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4252 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 149:
#line 1260 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4274 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 150:
#line 1279 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_window_clause_) = nullptr;
  }
#line 4282 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 1282 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_window_clause_) = new quickstep::ParseWindow((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].attribute_), (yyvsp[-2].window_partition_by_list_), (yyvsp[-4].literal_value_), (yyvsp[-1].literal_value_), (yyvsp[0].literal_value_));
  }
#line 4290 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 1287 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].literal_value_);
  }
#line 4298 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 1292 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_) = (yyvsp[0].attribute_);
  }
#line 4306 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 1297 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.window_partition_by_list_) = nullptr;
  }
#line 4314 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 1300 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.window_partition_by_list_) = (yyvsp[0].expression_list_);
  }
#line 4322 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 1305 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].literal_value_);
  }
#line 4330 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 1310 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].literal_value_);
  }
#line 4338 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 158:
#line 1315 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4347 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 159:
#line 1319 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4356 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 1325 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 4366 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 161:
#line 1332 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4374 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 1335 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4382 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 163:
#line 1338 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4390 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 1343 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4398 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 165:
#line 1346 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4406 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 166:
#line 1349 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4414 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 1355 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 4422 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 168:
#line 1358 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4430 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 169:
#line 1363 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4438 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 1368 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4452 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 171:
#line 1377 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4460 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 172:
#line 1382 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4474 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 173:
#line 1391 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4482 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 174:
#line 1396 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4490 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 175:
#line 1399 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4498 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 176:
#line 1404 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4506 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 177:
#line 1407 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 4516 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 178:
#line 1412 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4527 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 1418 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4538 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 180:
#line 1424 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4546 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 181:
#line 1427 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 4554 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 1433 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4562 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 183:
#line 1436 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4570 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 184:
#line 1441 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4578 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 185:
#line 1444 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4586 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 186:
#line 1449 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].unary_operation_), (yyvsp[0].expression_));
  }
#line 4594 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 187:
#line 1452 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4602 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 1457 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 4610 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 189:
#line 1460 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 4618 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 190:
#line 1463 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 4626 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 191:
#line 1466 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 4634 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 192:
#line 1471 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 4643 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 193:
#line 1475 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 4652 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 194:
#line 1479 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4660 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 195:
#line 1484 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4669 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 196:
#line 1488 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4678 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 197:
#line 1494 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4686 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 198:
#line 1497 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4694 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 199:
#line 1500 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4702 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 200:
#line 1503 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4719 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 201:
#line 1515 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 4728 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 202:
#line 1519 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4752 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 203:
#line 1538 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4770 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 204:
#line 1553 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4779 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 205:
#line 1557 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4788 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 206:
#line 1563 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4796 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 1566 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 4804 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 208:
#line 1571 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_list_) = new quickstep::PtrList<quickstep::ParseAttribute>();
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 4813 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 209:
#line 1575 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_list_) = (yyvsp[-2].attribute_list_);
    (yyval.attribute_list_)->push_back((yyvsp[0].attribute_));
  }
#line 4822 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 210:
#line 1582 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 4830 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 211:
#line 1585 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 4838 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 212:
#line 1588 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 4846 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 213:
#line 1591 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 4854 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 214:
#line 1594 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 4862 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 215:
#line 1597 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 4870 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 216:
#line 1600 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLike);
  }
#line 4878 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 217:
#line 1603 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotLike);
  }
#line 4886 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 218:
#line 1606 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kRegexMatch);
  }
#line 4894 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 219:
#line 1609 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotRegexMatch);
  }
#line 4902 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 220:
#line 1614 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
#line 4916 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 221:
#line 1625 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
#line 4924 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 222:
#line 1628 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
#line 4932 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 223:
#line 1633 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
#line 4940 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 224:
#line 1636 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
#line 4948 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 225:
#line 1642 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4957 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 226:
#line 1646 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4966 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 227:
#line 1652 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 4974 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 228:
#line 1655 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 4985 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 229:
#line 1663 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4993 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 230:
#line 1666 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;
  }
#line 5001 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 231:
#line 1669 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5009 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 232:
#line 1672 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;
  }
#line 5017 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 233:
#line 1678 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.command_) = new quickstep::ParseCommand((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].command_argument_list_));
  }
#line 5025 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 234:
#line 1683 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    quickstep::PtrVector<quickstep::ParseString> *argument_list = (yyvsp[-1].command_argument_list_);
    argument_list->push_back((yyvsp[0].string_value_));
    (yyval.command_argument_list_) = argument_list;
  }
#line 5035 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 235:
#line 1688 "../SqlParser.ypp" /* yacc.c:1646  */
    { /* Epsilon, an empy match. */
    (yyval.command_argument_list_) = new quickstep::PtrVector<quickstep::ParseString>();
  }
#line 5043 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;


#line 5047 "SqlParser_gen.cpp" /* yacc.c:1646  */
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
#line 1692 "../SqlParser.ypp" /* yacc.c:1906  */


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
