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

#line 148 "SqlParser_gen.cpp" /* yacc.c:339  */

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
    TOKEN_BETWEEN = 273,
    TOKEN_IS = 274,
    UNARY_PLUS = 275,
    UNARY_MINUS = 276,
    TOKEN_ADD = 277,
    TOKEN_ALL = 278,
    TOKEN_ALTER = 279,
    TOKEN_AS = 280,
    TOKEN_ASC = 281,
    TOKEN_BIGINT = 282,
    TOKEN_BIT = 283,
    TOKEN_BLOCKPROPERTIES = 284,
    TOKEN_BLOCKSAMPLE = 285,
    TOKEN_BLOOM_FILTER = 286,
    TOKEN_CSB_TREE = 287,
    TOKEN_BY = 288,
    TOKEN_CHARACTER = 289,
    TOKEN_CHECK = 290,
    TOKEN_COLUMN = 291,
    TOKEN_CONSTRAINT = 292,
    TOKEN_COPY = 293,
    TOKEN_CREATE = 294,
    TOKEN_DATE = 295,
    TOKEN_DATETIME = 296,
    TOKEN_DECIMAL = 297,
    TOKEN_DEFAULT = 298,
    TOKEN_DELETE = 299,
    TOKEN_DELIMITER = 300,
    TOKEN_DESC = 301,
    TOKEN_DISTINCT = 302,
    TOKEN_DOUBLE = 303,
    TOKEN_DROP = 304,
    TOKEN_ESCAPE_STRINGS = 305,
    TOKEN_FALSE = 306,
    TOKEN_FIRST = 307,
    TOKEN_FLOAT = 308,
    TOKEN_FOREIGN = 309,
    TOKEN_FROM = 310,
    TOKEN_FULL = 311,
    TOKEN_GROUP = 312,
    TOKEN_HAVING = 313,
    TOKEN_INDEX = 314,
    TOKEN_INNER = 315,
    TOKEN_INSERT = 316,
    TOKEN_INTEGER = 317,
    TOKEN_INTERVAL = 318,
    TOKEN_INTO = 319,
    TOKEN_JOIN = 320,
    TOKEN_KEY = 321,
    TOKEN_LAST = 322,
    TOKEN_LEFT = 323,
    TOKEN_LIMIT = 324,
    TOKEN_LONG = 325,
    TOKEN_NULL = 326,
    TOKEN_NULLS = 327,
    TOKEN_OFF = 328,
    TOKEN_ON = 329,
    TOKEN_ORDER = 330,
    TOKEN_OUTER = 331,
    TOKEN_PERCENT = 332,
    TOKEN_PRIMARY = 333,
    TOKEN_QUIT = 334,
    TOKEN_REAL = 335,
    TOKEN_REFERENCES = 336,
    TOKEN_RIGHT = 337,
    TOKEN_ROW_DELIMITER = 338,
    TOKEN_SELECT = 339,
    TOKEN_SET = 340,
    TOKEN_SMALLINT = 341,
    TOKEN_TABLE = 342,
    TOKEN_TIME = 343,
    TOKEN_TIMESTAMP = 344,
    TOKEN_TRUE = 345,
    TOKEN_TUPLESAMPLE = 346,
    TOKEN_UNIQUE = 347,
    TOKEN_UPDATE = 348,
    TOKEN_USING = 349,
    TOKEN_VALUES = 350,
    TOKEN_VARCHAR = 351,
    TOKEN_WHERE = 352,
    TOKEN_WITH = 353,
    TOKEN_YEARMONTH = 354,
    TOKEN_EOF = 355,
    TOKEN_LEX_ERROR = 356
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 110 "../SqlParser.ypp" /* yacc.c:355  */

  quickstep::ParseString *string_value_;

  quickstep::PtrList<quickstep::ParseString> *string_list_;

  bool boolean_value_;

  quickstep::NumericParseLiteralValue *numeric_literal_value_;
  quickstep::ParseLiteralValue *literal_value_;
  quickstep::PtrList<quickstep::ParseScalarLiteral> *literal_value_list_;

  quickstep::ParseExpression *expression_;

  quickstep::ParseScalarLiteral *scalar_literal_;
  quickstep::ParseAttribute *attribute_;

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

#line 372 "SqlParser_gen.cpp" /* yacc.c:355  */
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
#line 192 "../SqlParser.ypp" /* yacc.c:358  */

/* This header needs YYSTYPE, which is defined by the %union directive above */
#include "SqlLexer_gen.hpp"
void NotSupported(const YYLTYPE *location, yyscan_t yyscanner, const std::string &feature);

#line 405 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYLAST   858

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  112
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  85
/* YYNRULES -- Number of rules.  */
#define YYNRULES  222
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  426

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   356

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     108,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     109,   110,    22,    20,   111,    21,    26,    23,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   107,
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
      15,    16,    17,    18,    19,    24,    25,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   518,   518,   522,   526,   530,   534,   537,   544,   547,
     550,   553,   556,   559,   562,   565,   568,   571,   577,   583,
     590,   596,   603,   612,   617,   622,   627,   632,   636,   642,
     647,   650,   653,   658,   661,   664,   667,   670,   673,   676,
     679,   682,   685,   697,   700,   703,   721,   741,   744,   747,
     752,   757,   763,   769,   778,   782,   788,   791,   796,   801,
     806,   813,   820,   824,   830,   833,   838,   841,   849,   852,
     857,   861,   867,   870,   873,   878,   881,   888,   893,   904,
     908,   914,   917,   923,   931,   936,   941,   944,   949,   953,
     957,   961,   967,   972,   977,   981,   987,   993,   998,  1001,
    1006,  1010,  1016,  1022,  1028,  1031,  1035,  1041,  1044,  1049,
    1053,  1059,  1062,  1065,  1070,  1075,  1078,  1084,  1088,  1094,
    1100,  1106,  1112,  1118,  1124,  1130,  1136,  1144,  1149,  1152,
    1169,  1188,  1192,  1196,  1200,  1203,  1208,  1211,  1216,  1219,
    1224,  1228,  1234,  1237,  1242,  1245,  1250,  1253,  1258,  1261,
    1280,  1284,  1290,  1297,  1300,  1303,  1308,  1311,  1314,  1320,
    1323,  1328,  1333,  1342,  1347,  1356,  1361,  1364,  1369,  1372,
    1377,  1383,  1389,  1396,  1399,  1406,  1409,  1414,  1417,  1422,
    1425,  1430,  1433,  1436,  1439,  1444,  1448,  1452,  1457,  1461,
    1467,  1470,  1473,  1476,  1488,  1492,  1511,  1526,  1530,  1536,
    1539,  1545,  1548,  1551,  1554,  1557,  1560,  1565,  1576,  1579,
    1584,  1587,  1593,  1597,  1603,  1606,  1614,  1617,  1620,  1623,
    1629,  1634,  1639
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
  "TOKEN_NEQ", "TOKEN_LIKE", "TOKEN_BETWEEN", "TOKEN_IS", "'+'", "'-'",
  "'*'", "'/'", "UNARY_PLUS", "UNARY_MINUS", "'.'", "TOKEN_ADD",
  "TOKEN_ALL", "TOKEN_ALTER", "TOKEN_AS", "TOKEN_ASC", "TOKEN_BIGINT",
  "TOKEN_BIT", "TOKEN_BLOCKPROPERTIES", "TOKEN_BLOCKSAMPLE",
  "TOKEN_BLOOM_FILTER", "TOKEN_CSB_TREE", "TOKEN_BY", "TOKEN_CHARACTER",
  "TOKEN_CHECK", "TOKEN_COLUMN", "TOKEN_CONSTRAINT", "TOKEN_COPY",
  "TOKEN_CREATE", "TOKEN_DATE", "TOKEN_DATETIME", "TOKEN_DECIMAL",
  "TOKEN_DEFAULT", "TOKEN_DELETE", "TOKEN_DELIMITER", "TOKEN_DESC",
  "TOKEN_DISTINCT", "TOKEN_DOUBLE", "TOKEN_DROP", "TOKEN_ESCAPE_STRINGS",
  "TOKEN_FALSE", "TOKEN_FIRST", "TOKEN_FLOAT", "TOKEN_FOREIGN",
  "TOKEN_FROM", "TOKEN_FULL", "TOKEN_GROUP", "TOKEN_HAVING", "TOKEN_INDEX",
  "TOKEN_INNER", "TOKEN_INSERT", "TOKEN_INTEGER", "TOKEN_INTERVAL",
  "TOKEN_INTO", "TOKEN_JOIN", "TOKEN_KEY", "TOKEN_LAST", "TOKEN_LEFT",
  "TOKEN_LIMIT", "TOKEN_LONG", "TOKEN_NULL", "TOKEN_NULLS", "TOKEN_OFF",
  "TOKEN_ON", "TOKEN_ORDER", "TOKEN_OUTER", "TOKEN_PERCENT",
  "TOKEN_PRIMARY", "TOKEN_QUIT", "TOKEN_REAL", "TOKEN_REFERENCES",
  "TOKEN_RIGHT", "TOKEN_ROW_DELIMITER", "TOKEN_SELECT", "TOKEN_SET",
  "TOKEN_SMALLINT", "TOKEN_TABLE", "TOKEN_TIME", "TOKEN_TIMESTAMP",
  "TOKEN_TRUE", "TOKEN_TUPLESAMPLE", "TOKEN_UNIQUE", "TOKEN_UPDATE",
  "TOKEN_USING", "TOKEN_VALUES", "TOKEN_VARCHAR", "TOKEN_WHERE",
  "TOKEN_WITH", "TOKEN_YEARMONTH", "TOKEN_EOF", "TOKEN_LEX_ERROR", "';'",
  "'\\n'", "'('", "')'", "','", "$accept", "start", "sql_statement",
  "quit_statement", "alter_table_statement", "create_table_statement",
  "create_index_statement", "drop_table_statement", "column_def",
  "column_def_commalist", "data_type", "column_constraint_def",
  "column_constraint_def_list", "opt_column_constraint_def_list",
  "table_constraint_def", "table_constraint_def_commalist",
  "opt_table_constraint_def_commalist", "opt_column_list",
  "opt_block_properties", "key_value_list", "key_value",
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
  "opt_limit_clause", "order_commalist", "order_item",
  "opt_order_direction", "opt_nulls_first", "opt_where_clause",
  "where_clause", "or_expression", "and_expression", "not_expression",
  "predicate_expression_base", "add_expression", "multiply_expression",
  "unary_expression", "expression_base", "function_call",
  "expression_list", "literal_value", "literal_value_commalist",
  "attribute_ref", "comparison_operation", "unary_operation",
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
      43,    45,    42,    47,   275,   276,    46,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,    59,    10,    40,
      41,    44
};
# endif

#define YYPACT_NINF -189

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-189)))

#define YYTABLE_NINF -99

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      43,  -189,  -189,   -35,   156,   -15,    45,    36,    44,  -189,
     156,   156,  -189,   131,   105,  -189,  -189,  -189,  -189,  -189,
    -189,  -189,  -189,  -189,  -189,    77,   -58,   173,   156,  -189,
    -189,   134,   156,   156,   156,   156,   156,   114,    96,  -189,
     181,   129,  -189,  -189,  -189,    10,  -189,  -189,  -189,  -189,
      49,   210,   183,   151,   189,  -189,   -27,   156,   156,   184,
     156,  -189,  -189,   422,   102,   160,   167,   156,   156,   472,
    -189,  -189,   188,   156,    -7,  -189,   287,  -189,    77,  -189,
     106,  -189,  -189,  -189,   293,   294,  -189,  -189,  -189,   195,
    -189,   234,  -189,  -189,  -189,  -189,   301,  -189,  -189,  -189,
    -189,  -189,  -189,   199,   243,   538,   308,   255,   206,  -189,
     193,   196,  -189,  -189,  -189,  -189,  -189,   588,   -21,   156,
     -18,   156,   156,   209,  -189,   213,  -189,   110,   754,   654,
     472,   311,   312,  -189,  -189,   230,   306,   704,   117,   156,
    -189,   538,   216,  -189,   156,  -189,  -189,   321,  -189,  -189,
     323,  -189,     6,  -189,    11,   189,   538,  -189,  -189,   156,
     538,  -189,  -189,  -189,   538,   294,  -189,   156,   356,  -189,
     222,   263,   264,   227,  -189,  -189,  -189,   148,   156,   238,
     -18,   156,  -189,    50,  -189,     2,   172,   472,   472,   322,
    -189,  -189,  -189,  -189,  -189,  -189,   334,   538,   538,     8,
    -189,   119,   241,  -189,   212,  -189,  -189,   232,   235,  -189,
      55,  -189,    69,    55,   -16,   282,  -189,  -189,   196,  -189,
    -189,   244,  -189,   212,   124,   472,   242,   246,   156,   351,
      73,   126,   143,   228,  -189,   248,   254,  -189,   288,   259,
     704,  -189,   298,   156,  -189,  -189,    50,  -189,  -189,   312,
    -189,   538,  -189,    90,   212,   295,  -189,  -189,   704,   265,
    -189,  -189,   156,  -189,  -189,    21,   303,   156,    97,   100,
      11,  -189,    84,  -189,  -189,   368,   372,    55,   342,   318,
    -189,  -189,   538,    12,   156,   156,   158,  -189,  -189,  -189,
    -189,  -189,  -189,  -189,   150,  -189,  -189,  -189,   274,   -18,
     352,  -189,  -189,   472,  -189,  -189,   276,  -189,   152,   538,
    -189,  -189,   704,  -189,   156,   320,   156,   -56,   156,   324,
     156,   326,  -189,  -189,   309,   310,  -189,   538,   472,   313,
     212,  -189,   161,   165,  -189,   392,    73,   156,  -189,  -189,
     289,    13,   156,   538,   212,   168,   -45,   156,   -44,   472,
     -43,   156,   -40,   156,  -189,  -189,   296,   311,   361,   330,
     314,  -189,  -189,  -189,   174,  -189,  -189,  -189,  -189,     5,
     156,  -189,   300,   212,  -189,   472,   -37,   472,   311,   472,
     -36,   472,   -28,   538,   398,  -189,   156,  -189,   156,  -189,
    -189,   156,  -189,   176,  -189,   311,   472,   311,   311,   472,
     311,   472,   297,  -189,   144,  -189,   302,  -189,   179,  -189,
     311,   311,   311,   538,  -189,  -189,   329,   156,  -189,  -189,
     -20,  -189,   185,  -189,  -189,  -189
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     6,   222,     0,     0,     0,     0,     0,     0,    18,
       0,     0,     7,     0,     0,    15,     8,    10,    11,    13,
      14,     9,    17,    12,    16,     0,     0,   220,     0,   214,
     215,     0,     0,     0,     0,     0,     0,     0,    99,   100,
       0,   138,     1,     3,     2,   104,    97,     5,     4,   221,
       0,     0,     0,     0,   159,    25,     0,     0,     0,     0,
       0,   105,   106,     0,     0,     0,    86,     0,     0,     0,
      93,   160,     0,     0,   159,    95,     0,   101,     0,   102,
       0,   212,   194,   191,     0,   207,   107,    40,    29,     0,
      30,    31,    34,    36,    37,    39,     0,    41,   190,    35,
      38,    32,    33,     0,     0,     0,     0,     0,   108,   109,
     113,   176,   178,   180,   183,   182,   181,     0,   199,     0,
       0,     0,     0,     0,    85,    66,    27,     0,     0,     0,
       0,   161,   163,   165,   167,     0,   181,     0,     0,     0,
      92,     0,     0,   139,     0,   192,   193,     0,    43,   195,
       0,    44,     0,   196,     0,   159,     0,   208,   209,     0,
       0,   112,   210,   211,     0,     0,   179,     0,     0,    19,
       0,     0,     0,     0,    20,    21,    22,     0,     0,     0,
      64,     0,    42,    56,   166,     0,     0,     0,     0,     0,
     201,   203,   204,   205,   206,   202,     0,     0,     0,     0,
     197,     0,     0,    94,    96,   127,   213,     0,     0,   184,
       0,   140,   115,   134,   128,   142,   110,   111,   175,   177,
     200,     0,   185,   188,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    65,    68,    28,     0,     0,
       0,    47,     0,     0,    49,    55,    57,    26,   174,   162,
     164,     0,   172,     0,   173,     0,   171,    84,     0,     0,
      45,    46,     0,   131,   136,     0,     0,     0,     0,     0,
       0,   114,   116,   118,   133,     0,     0,   135,     0,   144,
     186,   187,     0,     0,     0,     0,     0,    88,   218,   219,
     217,   216,    89,    87,     0,    67,    79,    80,    81,     0,
       0,    23,    48,     0,    51,    50,     0,    54,     0,     0,
     170,   198,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,   141,   117,     0,     0,   132,     0,     0,   146,
     189,    61,     0,     0,    58,     0,     0,     0,    24,    62,
       0,     0,     0,     0,   168,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   129,   130,   143,   145,     0,   148,
       0,    59,    90,    91,     0,    70,    72,    73,    74,     0,
       0,    52,     0,   169,    83,     0,     0,     0,   120,     0,
       0,     0,     0,     0,     0,   103,     0,    82,     0,    78,
      76,     0,    75,     0,    53,   126,     0,   119,   122,     0,
     124,     0,   147,   150,   153,   149,     0,    71,     0,    69,
     125,   121,   123,     0,   154,   155,   156,     0,    77,   151,
       0,   152,     0,   157,   158,    60
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,  -103,  -189,
     284,   169,  -189,  -189,  -178,  -189,  -189,  -189,  -189,    46,
      29,  -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,
    -189,  -189,  -189,   279,  -189,  -189,  -189,   362,   341,  -189,
    -189,  -189,   266,  -189,  -189,  -189,   149,   366,  -189,   163,
    -188,   -10,  -189,  -189,  -189,  -189,  -189,  -189,    17,  -189,
    -189,   -61,  -189,   -72,   247,   249,   307,   -60,   275,   281,
     331,  -146,   111,  -133,   127,   -29,  -189,  -189,  -189,  -189,
     -59,    -4,   104,  -189,  -189
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    13,    14,    15,    16,    17,    18,    19,   126,   127,
     106,   245,   246,   247,   174,   235,   236,   179,   301,   364,
     365,   366,   367,   368,   298,   338,    20,    21,   124,   231,
      22,    23,    74,    75,    24,    25,    38,    39,    46,    63,
     107,   108,   109,   155,   271,   272,   273,   210,   277,   211,
     263,   264,   212,   279,   329,   359,   385,   402,   403,   416,
     421,    70,    71,   131,   132,   133,   134,   135,   111,   112,
     113,   114,   224,   115,   201,   116,   198,   117,   160,   164,
      80,   118,   292,    26,    27
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      31,    40,   234,   110,   200,   167,    37,    41,   213,    29,
     187,    30,   389,   140,   138,    29,   169,    30,   255,   275,
     187,   187,   170,   349,    50,   274,   157,   158,    52,    53,
      54,    55,    56,   390,   375,   377,   379,   423,    61,   381,
     136,   171,   396,   399,     1,   152,     2,    47,    40,    32,
      48,   401,   424,    76,    41,   144,    81,    28,   185,    29,
     238,    30,    62,   125,   128,   172,   144,   144,   144,    81,
     186,   144,     3,    72,   144,   144,    64,    33,   237,   173,
     276,   204,    73,   144,   256,   262,     4,     5,   168,   326,
     239,   314,     6,   168,   215,    69,   110,     7,   240,   309,
     136,   136,   315,    65,   139,    34,   161,   304,   223,     8,
     157,   158,   248,    36,   391,   128,   209,   175,   176,   232,
      78,   339,   331,   371,   213,   311,   241,     9,    35,   288,
     265,    42,   -98,   242,   266,    76,   243,   253,   254,   267,
     206,    10,   268,   119,   120,   265,    11,   244,    12,   266,
     214,   289,   290,   283,   267,   217,   269,   268,   136,   136,
      29,   343,    30,   220,   157,   158,    45,   318,   291,   286,
     320,   269,   157,   158,    81,   414,    49,   128,   319,   200,
     270,   321,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   308,   157,   158,    51,   415,   136,    29,   229,    30,
     335,   121,   122,   230,    57,   336,    41,    58,   317,    41,
      43,    59,    44,   157,   158,    66,   143,   144,   162,   163,
     180,   181,   330,   159,    81,   332,   333,   202,   144,   257,
     258,   341,   157,   158,   281,   282,   293,   294,    60,   306,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   344,
     157,   158,   313,   295,   144,   346,   357,   348,    41,   350,
      68,   352,    67,    81,   296,   297,   214,   223,   334,   144,
     123,   360,   144,    41,   136,   361,   144,   378,   374,   258,
      81,    81,   209,   373,   387,   388,   409,   388,   376,   418,
     144,    69,   380,    78,   382,   425,   144,   137,   141,   136,
     145,   146,   148,   395,   147,   397,   149,   398,   150,   400,
      81,   151,    81,   153,    81,   154,    81,   156,   177,   187,
     136,   188,   178,   404,   410,   199,   205,   411,   207,   412,
     208,   225,   408,   369,   226,   227,   228,   233,   372,   252,
     251,   259,   260,    81,   278,   261,   136,    81,   136,    81,
     136,   284,   136,   404,   280,   285,   287,   300,   422,   299,
      29,    82,    30,    83,   302,   392,   369,   136,   303,   305,
     136,   310,   136,   316,   312,   324,    84,    85,   221,   325,
     327,   328,   406,   337,   369,   342,   340,    81,    87,    88,
     347,   354,   355,   358,   351,    89,   353,   362,   370,   383,
     386,    90,    91,    92,   384,   405,   420,   282,   413,    93,
     394,   417,   183,    81,    94,   307,   393,   407,   203,   142,
      77,   323,   216,    95,    96,    79,    29,    82,    30,    83,
     419,    97,    98,   322,   249,   218,   184,   250,   356,   345,
     363,    99,    84,    85,    86,   219,     0,   100,   166,   101,
     102,     0,     0,     0,    87,    88,     0,   103,     0,     0,
     104,    89,     0,     0,     0,   105,   222,    90,    91,    92,
       0,     0,     0,     0,     0,    93,    29,    82,    30,    83,
      94,     0,   129,     0,     0,     0,     0,     0,     0,    95,
      96,     0,    84,    85,     0,     0,     0,    97,    98,     0,
       0,     0,     0,     0,    87,    88,     0,    99,     0,     0,
       0,    89,     0,   100,     0,   101,   102,    90,    91,    92,
       0,     0,     0,   103,     0,    93,   104,     0,     0,     0,
      94,   105,     0,     0,     0,     0,     0,     0,     0,    95,
      96,     0,    29,    82,    30,    83,     0,    97,    98,     0,
       0,     0,     0,     0,     0,     0,     0,    99,    84,    85,
       0,     0,     0,   100,     0,   101,   102,     0,     0,     0,
      87,    88,     0,   103,     0,     0,   104,    89,     0,     0,
       0,   130,     0,    90,    91,    92,     0,     0,     0,     0,
       0,    93,    29,    82,    30,    83,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,     0,    84,   165,
       0,     0,     0,    97,    98,     0,     0,     0,     0,     0,
      87,    88,     0,    99,     0,     0,     0,    89,     0,   100,
       0,   101,   102,    90,    91,    92,     0,     0,     0,   103,
       0,    93,   104,     0,     0,     0,    94,   105,     0,     0,
       0,     0,     0,     0,     0,    95,    96,     0,    29,    82,
      30,    83,     0,    97,    98,     0,     0,     0,     0,     0,
       0,     0,     0,    99,    84,    85,     0,     0,     0,   100,
       0,   101,   102,     0,     0,     0,    87,    88,     0,   103,
       0,     0,   104,    89,     0,     0,     0,   105,     0,    90,
      91,    92,     0,     0,     0,     0,     0,    93,     0,    82,
       0,    83,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,    96,     0,    84,   165,     0,     0,     0,    97,
      98,     0,     0,     0,     0,     0,    87,    88,     0,    99,
       0,     0,     0,    89,     0,   100,     0,   101,   102,    90,
      91,    92,     0,     0,     0,   103,     0,    93,   104,     0,
       0,     0,    94,   130,     0,     0,     0,     0,     0,     0,
       0,    95,    96,     0,     0,     0,     0,     0,     0,    97,
      98,     0,     0,     0,     0,     0,    87,    88,     0,    99,
       0,     0,     0,    89,     0,   100,     0,   101,   102,    90,
      91,    92,     0,     0,     0,   103,     0,    93,   104,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,   182,     0,     0,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,     0,     0,     0,   100,     0,   101,   102,     0,
       0,     0,     0,     0,     0,   103,     0,     0,   104
};

static const yytype_int16 yycheck[] =
{
       4,    11,   180,    63,   137,    26,    10,    11,   154,     4,
       8,     6,     7,    74,    73,     4,   119,     6,    10,    35,
       8,     8,    40,    79,    28,   213,    20,    21,    32,    33,
      34,    35,    36,    28,    79,    79,    79,    57,    28,    79,
      69,    59,    79,    79,     1,   105,     3,   105,    58,    64,
     108,    79,    72,    57,    58,   111,    60,    92,   130,     4,
      10,     6,    52,    67,    68,    83,   111,   111,   111,    73,
     130,   111,    29,   100,   111,   111,    27,    92,   181,    97,
      96,   141,   109,   111,    76,    30,    43,    44,   109,   277,
      40,    70,    49,   109,   155,   102,   156,    54,    48,     9,
     129,   130,    81,    54,   111,    60,   110,   240,   168,    66,
      20,    21,   110,    69,   109,   119,   110,   121,   122,   178,
     109,   299,   110,   110,   270,   258,    76,    84,    92,    56,
      61,     0,    89,    83,    65,   139,    86,   197,   198,    70,
     144,    98,    73,    41,    42,    61,   103,    97,   105,    65,
     154,    78,    79,   225,    70,   159,    87,    73,   187,   188,
       4,     9,     6,   167,    20,    21,    89,    70,    95,   228,
      70,    87,    20,    21,   178,    31,     3,   181,    81,   312,
     111,    81,    10,    11,    12,    13,    14,    15,    16,    17,
      18,   251,    20,    21,    60,    51,   225,     4,    50,     6,
      50,    41,    42,    55,    90,    55,   210,   111,   267,   213,
     105,    30,   107,    20,    21,     5,   110,   111,    22,    23,
     110,   111,   282,    30,   228,   284,   285,   110,   111,   110,
     111,   303,    20,    21,   110,   111,   110,   111,   109,   243,
      10,    11,    12,    13,    14,    15,    16,    17,    18,   309,
      20,    21,   262,   110,   111,   314,   328,   316,   262,   318,
     109,   320,    79,   267,    36,    37,   270,   327,   110,   111,
     103,   110,   111,   277,   303,   110,   111,   349,   110,   111,
     284,   285,   110,   343,   110,   111,   110,   111,   347,   110,
     111,   102,   351,   109,   353,   110,   111,   109,    11,   328,
       7,     7,    68,   375,   109,   377,     5,   379,   109,   381,
     314,    68,   316,     5,   318,    60,   320,   111,   109,     8,
     349,     9,   109,   383,   396,    19,   110,   399,     7,   401,
       7,   109,   391,   337,    71,    71,   109,    99,   342,     5,
      18,   100,   110,   347,    62,   110,   375,   351,   377,   353,
     379,   109,   381,   413,   110,   109,     5,   103,   417,   111,
       4,     5,     6,     7,    76,   369,   370,   396,   109,    71,
     399,    76,   401,    70,   109,     7,    20,    21,    22,     7,
      38,    63,   386,   109,   388,   109,    34,   391,    32,    33,
      70,    82,    82,    80,    70,    39,    70,     5,   109,    38,
      86,    45,    46,    47,    74,     7,    77,   111,   111,    53,
     110,   109,   128,   417,    58,   246,   370,   388,   139,    78,
      58,   272,   156,    67,    68,    59,     4,     5,     6,     7,
     413,    75,    76,   270,   187,   160,   129,   188,   327,   312,
     336,    85,    20,    21,    22,   164,    -1,    91,   117,    93,
      94,    -1,    -1,    -1,    32,    33,    -1,   101,    -1,    -1,
     104,    39,    -1,    -1,    -1,   109,   110,    45,    46,    47,
      -1,    -1,    -1,    -1,    -1,    53,     4,     5,     6,     7,
      58,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    -1,    20,    21,    -1,    -1,    -1,    75,    76,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    85,    -1,    -1,
      -1,    39,    -1,    91,    -1,    93,    94,    45,    46,    47,
      -1,    -1,    -1,   101,    -1,    53,   104,    -1,    -1,    -1,
      58,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    -1,     4,     5,     6,     7,    -1,    75,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    20,    21,
      -1,    -1,    -1,    91,    -1,    93,    94,    -1,    -1,    -1,
      32,    33,    -1,   101,    -1,    -1,   104,    39,    -1,    -1,
      -1,   109,    -1,    45,    46,    47,    -1,    -1,    -1,    -1,
      -1,    53,     4,     5,     6,     7,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    20,    21,
      -1,    -1,    -1,    75,    76,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    85,    -1,    -1,    -1,    39,    -1,    91,
      -1,    93,    94,    45,    46,    47,    -1,    -1,    -1,   101,
      -1,    53,   104,    -1,    -1,    -1,    58,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    -1,     4,     5,
       6,     7,    -1,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    20,    21,    -1,    -1,    -1,    91,
      -1,    93,    94,    -1,    -1,    -1,    32,    33,    -1,   101,
      -1,    -1,   104,    39,    -1,    -1,    -1,   109,    -1,    45,
      46,    47,    -1,    -1,    -1,    -1,    -1,    53,    -1,     5,
      -1,     7,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    -1,    20,    21,    -1,    -1,    -1,    75,
      76,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    85,
      -1,    -1,    -1,    39,    -1,    91,    -1,    93,    94,    45,
      46,    47,    -1,    -1,    -1,   101,    -1,    53,   104,    -1,
      -1,    -1,    58,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    85,
      -1,    -1,    -1,    39,    -1,    91,    -1,    93,    94,    45,
      46,    47,    -1,    -1,    -1,   101,    -1,    53,   104,    -1,
      -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,   104
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,    29,    43,    44,    49,    54,    66,    84,
      98,   103,   105,   113,   114,   115,   116,   117,   118,   119,
     138,   139,   142,   143,   146,   147,   195,   196,    92,     4,
       6,   193,    64,    92,    60,    92,    69,   193,   148,   149,
     163,   193,     0,   105,   107,    89,   150,   105,   108,     3,
     193,    60,   193,   193,   193,   193,   193,    90,   111,    30,
     109,    28,    52,   151,    27,    54,     5,    79,   109,   102,
     173,   174,   100,   109,   144,   145,   193,   149,   109,   159,
     192,   193,     5,     7,    20,    21,    22,    32,    33,    39,
      45,    46,    47,    53,    58,    67,    68,    75,    76,    85,
      91,    93,    94,   101,   104,   109,   122,   152,   153,   154,
     179,   180,   181,   182,   183,   185,   187,   189,   193,    41,
      42,    41,    42,   103,   140,   193,   120,   121,   193,    10,
     109,   175,   176,   177,   178,   179,   187,   109,   192,   111,
     173,    11,   150,   110,   111,     7,     7,   109,    68,     5,
     109,    68,   179,     5,    60,   155,   111,    20,    21,    30,
     190,   193,    22,    23,   191,    21,   182,    26,   109,   120,
      40,    59,    83,    97,   126,   193,   193,   109,   109,   129,
     110,   111,    68,   122,   178,   175,   179,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,   188,    19,
     185,   186,   110,   145,   179,   110,   193,     7,     7,   110,
     159,   161,   164,   183,   193,   173,   154,   193,   180,   181,
     193,    22,   110,   179,   184,   109,    71,    71,   109,    50,
      55,   141,   192,    99,   126,   127,   128,   120,    10,    40,
      48,    76,    83,    86,    97,   123,   124,   125,   110,   176,
     177,    18,     5,   179,   179,    10,    76,   110,   111,   100,
     110,   110,    30,   162,   163,    61,    65,    70,    73,    87,
     111,   156,   157,   158,   162,    35,    96,   160,    62,   165,
     110,   110,   111,   175,   109,   109,   192,     5,    56,    78,
      79,    95,   194,   110,   111,   110,    36,    37,   136,   111,
     103,   130,    76,   109,   185,    71,   193,   123,   179,     9,
      76,   185,   109,   163,    70,    81,    70,   192,    70,    81,
      70,    81,   161,   158,     7,     7,   162,    38,    63,   166,
     179,   110,   192,   192,   110,    50,    55,   109,   137,   126,
      34,   175,   109,     9,   179,   186,   192,    70,   192,    79,
     192,    70,   192,    70,    82,    82,   184,   175,    80,   167,
     110,   110,     5,   194,   131,   132,   133,   134,   135,   193,
     109,   110,   193,   179,   110,    79,   192,    79,   175,    79,
     192,    79,   192,    38,    74,   168,    86,   110,   111,     7,
      28,   109,   193,   131,   110,   175,    79,   175,   175,    79,
     175,    79,   169,   170,   179,     7,   193,   132,   192,   110,
     175,   175,   175,   111,    31,    51,   171,   109,   110,   170,
      77,   172,   192,    57,    72,   110
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   112,   113,   113,   113,   113,   113,   113,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   114,   115,   116,
     116,   116,   116,   117,   118,   119,   120,   121,   121,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   123,   123,   123,
     123,   123,   123,   123,   124,   124,   125,   125,   126,   126,
     126,   126,   127,   127,   128,   128,   129,   129,   130,   130,
     131,   131,   132,   132,   132,   133,   133,   134,   135,   136,
     136,   137,   137,   138,   138,   139,   140,   140,   141,   141,
     141,   141,   142,   143,   144,   144,   145,   146,   147,   147,
     148,   148,   149,   150,   151,   151,   151,   152,   152,   153,
     153,   154,   154,   154,   155,   156,   156,   157,   157,   158,
     158,   158,   158,   158,   158,   158,   158,   159,   160,   160,
     160,   161,   161,   161,   161,   161,   162,   162,   163,   163,
     164,   164,   165,   165,   166,   166,   167,   167,   168,   168,
     169,   169,   170,   171,   171,   171,   172,   172,   172,   173,
     173,   174,   175,   175,   176,   176,   177,   177,   178,   178,
     178,   178,   178,   178,   178,   179,   179,   180,   180,   181,
     181,   182,   182,   182,   182,   183,   183,   183,   184,   184,
     185,   185,   185,   185,   185,   185,   185,   186,   186,   187,
     187,   188,   188,   188,   188,   188,   188,   189,   190,   190,
     191,   191,   192,   192,   193,   193,   194,   194,   194,   194,
     195,   196,   196
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
       1,     3,     3,     9,     0,     1,     1,     1,     1,     1,
       3,     3,     2,     1,     3,     0,     1,     2,     1,     5,
       4,     6,     5,     6,     5,     6,     5,     3,     0,     3,
       3,     2,     3,     2,     1,     2,     1,     2,     1,     4,
       1,     3,     0,     3,     0,     2,     0,     3,     0,     2,
       1,     3,     3,     0,     1,     1,     0,     2,     2,     0,
       1,     2,     3,     1,     3,     1,     2,     1,     5,     6,
       4,     3,     3,     3,     3,     3,     1,     3,     1,     2,
       1,     1,     1,     1,     3,     3,     4,     4,     1,     3,
       1,     1,     2,     2,     1,     2,     2,     1,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       2,     2,     0
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
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1746 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_NAME  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1756 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1766 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1776 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 7: /* TOKEN_UNSIGNED_NUMVAL  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 1786 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 114: /* sql_statement  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1796 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 115: /* quit_statement  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 1806 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 116: /* alter_table_statement  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1816 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 117: /* create_table_statement  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 1826 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 118: /* create_index_statement  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1836 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 119: /* drop_table_statement  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 1846 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 120: /* column_def  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 1856 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 121: /* column_def_commalist  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 1866 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 122: /* data_type  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 1876 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 123: /* column_constraint_def  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 1886 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 124: /* column_constraint_def_list  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1896 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 125: /* opt_column_constraint_def_list  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1906 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 129: /* opt_column_list  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 1916 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 130: /* opt_block_properties  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).block_properties_) != nullptr) {
    delete ((*yyvaluep).block_properties_);
  }
}
#line 1926 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 131: /* key_value_list  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 1936 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 132: /* key_value  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 1946 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 133: /* key_string_value  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 1956 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 134: /* key_string_list  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 1966 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 135: /* key_integer_value  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 1976 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 136: /* index_type  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1986 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 137: /* opt_index_properties  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 1996 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 138: /* insert_statement  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 2006 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 139: /* copy_from_statement  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_statement_) != nullptr) {
    delete ((*yyvaluep).copy_from_statement_);
  }
}
#line 2016 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 140: /* opt_copy_from_params  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2026 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 141: /* copy_from_params  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2036 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 142: /* update_statement  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2046 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 143: /* delete_statement  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2056 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 144: /* assignment_list  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2066 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 145: /* assignment_item  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2076 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 146: /* select_statement  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_statement_) != nullptr) {
    delete ((*yyvaluep).select_statement_);
  }
}
#line 2086 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 147: /* opt_with_clause  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2096 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 148: /* with_list  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2106 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 149: /* with_list_element  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2116 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 150: /* select_query  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2126 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 152: /* selection  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2136 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 153: /* selection_item_commalist  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2146 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* selection_item  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2156 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* from_clause  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2166 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 159: /* subquery_expression  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2176 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 160: /* opt_sample_clause  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_sample_clause_) != nullptr) {
    delete ((*yyvaluep).opt_sample_clause_);
  }
}
#line 2186 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* table_reference  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2196 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* table_reference_signature  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2206 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 163: /* table_reference_signature_primary  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2216 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 164: /* table_reference_commalist  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2226 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 165: /* opt_group_by_clause  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2236 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 166: /* opt_having_clause  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2246 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 167: /* opt_order_by_clause  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2256 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* opt_limit_clause  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2266 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* order_commalist  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2276 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* order_item  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2286 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* opt_order_direction  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2296 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* opt_nulls_first  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2306 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* opt_where_clause  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2316 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* where_clause  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2326 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* or_expression  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2336 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* and_expression  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2346 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* not_expression  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2356 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* predicate_expression_base  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2366 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* add_expression  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2376 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* multiply_expression  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2386 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* unary_expression  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2396 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* expression_base  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2406 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* function_call  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2416 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* expression_list  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2426 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* literal_value  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2436 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* literal_value_commalist  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2446 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* attribute_ref  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2456 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* comparison_operation  */
#line 505 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2462 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* unary_operation  */
#line 506 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2468 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 190: /* add_operation  */
#line 507 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2474 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 191: /* multiply_operation  */
#line 507 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2480 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 192: /* name_commalist  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 2490 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 193: /* any_name  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2500 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 194: /* boolean_value  */
#line 504 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2506 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 195: /* command  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_) != nullptr) {
    delete ((*yyvaluep).command_);
  }
}
#line 2516 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 196: /* command_argument_list  */
#line 509 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_argument_list_) != nullptr) {
    delete ((*yyvaluep).command_argument_list_);
  }
}
#line 2526 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 518 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2823 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 522 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2832 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 526 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 2841 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 530 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 2850 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 534 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    YYABORT;
  }
#line 2858 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 537 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 2867 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 544 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 2875 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 547 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].copy_from_statement_);
  }
#line 2883 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 550 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 2891 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 553 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 2899 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 556 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 2907 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 559 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 2915 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 562 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 2923 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 565 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 2931 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 568 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].select_statement_);
  }
#line 2939 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 571 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 2947 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 577 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 2955 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 583 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2967 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 590 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2978 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 596 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2990 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 603 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3002 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 612 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-7]).first_line, (yylsp[-7]).first_column, (yyvsp[-5].string_value_), (yyvsp[-3].attribute_definition_list_), (yyvsp[0].block_properties_));
  }
#line 3010 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 617 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].string_list_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 3018 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 622 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 3026 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 627 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 3034 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 632 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3043 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 636 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3052 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 642 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 3062 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 647 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3070 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 650 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3078 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 653 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3088 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 658 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3096 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 661 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3104 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 664 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3112 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 667 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3120 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 670 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3128 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 673 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3136 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 676 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3144 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 679 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3152 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 682 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3160 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 685 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3177 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 697 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3185 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 700 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3193 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 703 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3216 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 721 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3239 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 741 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3247 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 744 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3255 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 747 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3265 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 752 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3275 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 757 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3286 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 763 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3297 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 769 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3309 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 778 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3318 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 782 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3327 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 788 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3335 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 791 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3343 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 796 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3353 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 801 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3363 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 806 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3375 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 813 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3385 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 820 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3394 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 824 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3403 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 830 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 3411 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 833 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = $1; */
  }
#line 3419 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 838 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = nullptr;
  }
#line 3427 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 841 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    (yyval.string_list_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "list of column names in CREATE INDEX statement");
    YYERROR;
  }
#line 3438 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 849 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.block_properties_) = nullptr;
  }
#line 3446 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 852 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].key_value_list_));
  }
#line 3454 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 857 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3463 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 861 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3472 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 867 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 3480 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 870 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 3488 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 873 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 3496 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 878 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 3504 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 881 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), 
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 3514 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 888 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 3522 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 893 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 3536 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 904 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
#line 3545 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 908 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
#line 3554 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 914 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 3562 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 917 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 3570 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 923 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 3583 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 931 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsert((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 3591 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 936 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_statement_) = new quickstep::ParseStatementCopyFrom((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].copy_from_params_));
  }
#line 3599 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 941 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = nullptr;
  }
#line 3607 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 944 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-1].copy_from_params_);
  }
#line 3615 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 949 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3624 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 953 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3633 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 957 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3642 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 961 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3651 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 967 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 3659 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 972 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 3667 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 977 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3676 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 981 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3685 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 987 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 3693 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 993 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].select_query_), (yyvsp[-1].with_list_));
  }
#line 3701 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 998 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = nullptr;
  }
#line 3709 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 1001 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 3717 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 1006 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3726 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 1010 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3735 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 1016 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 3744 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 1023 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].selection_), (yyvsp[-5].table_reference_list_), (yyvsp[-4].predicate_), (yyvsp[-3].opt_group_by_clause_), (yyvsp[-2].opt_having_clause_), (yyvsp[-1].opt_order_by_clause_), (yyvsp[0].opt_limit_clause_));
  }
#line 3752 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 1028 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 3760 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 1031 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "ALL in selection");
    YYERROR;
  }
#line 3769 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 1035 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "DISTINCT in selection");
    YYERROR;
  }
#line 3778 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 1041 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3786 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 1044 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 3794 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 1049 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3803 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 1053 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3812 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 1059 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 3820 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 1062 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 3828 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 1065 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 3836 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 1070 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = (yyvsp[-1].table_reference_list_);
  }
#line 3844 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 1075 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 3852 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 1078 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3861 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 1084 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[-1]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3870 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 1088 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3879 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 1094 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3890 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 1100 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3901 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 1106 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3912 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 1112 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3923 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 1118 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3934 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 1124 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3945 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 1130 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3956 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 1136 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3967 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 1144 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].select_query_));
  }
#line 3975 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 1149 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_sample_clause_) = NULL;
  }
#line 3983 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 1152 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-1].numeric_literal_value_)->float_like()) {
      delete (yyvsp[-1].numeric_literal_value_);
      (yyval.opt_sample_clause_) = NULL;
      quickstep_yyerror(&(yylsp[-1]), yyscanner, NULL, "SAMPLE value must be an integer");
      YYERROR;
    } else {
      if ((yyvsp[-1].numeric_literal_value_)->long_value() <= 0 || (yyvsp[-1].numeric_literal_value_)->long_value() >100) {
        delete (yyvsp[-1].numeric_literal_value_);
        (yyval.opt_sample_clause_) = NULL;
        quickstep_yyerror(&(yylsp[-1]), yyscanner, NULL, "SAMPLE PERCENT must be between 1 and 100");
        YYERROR;
      } else {
        (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column,true, (yyvsp[-1].numeric_literal_value_));
      }
    }
  }
#line 4005 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 1169 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-1].numeric_literal_value_)->float_like()) {
      delete (yyvsp[-1].numeric_literal_value_);
      (yyval.opt_sample_clause_) = NULL;
      quickstep_yyerror(&(yylsp[-1]), yyscanner, NULL, "SAMPLE value must be an integer");
      YYERROR;
    } else {
      if ((yyvsp[-1].numeric_literal_value_)->long_value() <= 0 || (yyvsp[-1].numeric_literal_value_)->long_value() >100) {
        delete (yyvsp[-1].numeric_literal_value_);
        (yyval.opt_sample_clause_) = NULL;
        quickstep_yyerror(&(yylsp[-1]), yyscanner, NULL, "SAMPLE PERCENT must be between 1 and 100");
        YYERROR;
      } else {
        (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column,false, (yyvsp[-1].numeric_literal_value_));
        
      }
    }
  }
#line 4028 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 1188 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4037 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 1192 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_),(yyvsp[-1].opt_sample_clause_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_)  );
  }
#line 4046 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 1196 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4055 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 1200 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].function_call_));
  }
#line 4063 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 1203 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_),(yyvsp[0].opt_sample_clause_));
  }
#line 4071 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 1208 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4079 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 1211 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4087 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 1216 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4095 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 1219 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4103 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 1224 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4112 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 1228 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4121 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 1234 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 4129 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 1237 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4137 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 1242 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4145 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 1245 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4153 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 1250 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4161 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 1253 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4169 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 148:
#line 1258 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4177 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 149:
#line 1261 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4199 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 150:
#line 1280 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4208 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 1284 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4217 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 1290 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 4227 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 1297 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4235 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 1300 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4243 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 1303 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4251 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 1308 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4259 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 1311 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4267 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 158:
#line 1314 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4275 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 159:
#line 1320 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 4283 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 1323 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4291 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 161:
#line 1328 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4299 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 1333 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4313 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 163:
#line 1342 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4321 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 1347 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4335 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 165:
#line 1356 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4343 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 166:
#line 1361 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4351 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 1364 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4359 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 168:
#line 1369 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4367 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 169:
#line 1372 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 4377 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 1377 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4388 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 171:
#line 1383 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4399 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 172:
#line 1389 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].expression_);
    delete (yyvsp[0].string_value_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "LIKE predicates");
    YYERROR;
  }
#line 4411 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 173:
#line 1396 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4419 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 174:
#line 1399 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 4427 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 175:
#line 1406 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4435 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 176:
#line 1409 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4443 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 177:
#line 1414 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4451 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 178:
#line 1417 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4459 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 1422 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].unary_operation_), (yyvsp[0].expression_));
  }
#line 4467 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 180:
#line 1425 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4475 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 181:
#line 1430 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 4483 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 1433 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 4491 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 183:
#line 1436 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 4499 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 184:
#line 1439 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 4507 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 185:
#line 1444 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 4516 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 186:
#line 1448 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 4525 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 187:
#line 1452 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4533 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 1457 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4542 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 189:
#line 1461 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4551 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 190:
#line 1467 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4559 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 191:
#line 1470 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4567 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 192:
#line 1473 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4575 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 193:
#line 1476 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4592 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 194:
#line 1488 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 4601 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 195:
#line 1492 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4625 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 196:
#line 1511 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4643 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 197:
#line 1526 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4652 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 198:
#line 1530 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4661 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 199:
#line 1536 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4669 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 200:
#line 1539 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 4677 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 201:
#line 1545 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 4685 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 202:
#line 1548 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 4693 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 203:
#line 1551 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 4701 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 204:
#line 1554 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 4709 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 205:
#line 1557 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 4717 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 206:
#line 1560 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 4725 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 1565 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
#line 4739 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 208:
#line 1576 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
#line 4747 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 209:
#line 1579 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
#line 4755 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 210:
#line 1584 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
#line 4763 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 211:
#line 1587 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
#line 4771 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 212:
#line 1593 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4780 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 213:
#line 1597 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4789 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 214:
#line 1603 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 4797 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 215:
#line 1606 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 4808 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 216:
#line 1614 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4816 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 217:
#line 1617 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4824 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 218:
#line 1620 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;
  }
#line 4832 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 219:
#line 1623 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;
  }
#line 4840 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 220:
#line 1629 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.command_) = new quickstep::ParseCommand((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].command_argument_list_));
  }
#line 4848 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 221:
#line 1634 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    quickstep::PtrVector<quickstep::ParseString> *argument_list = (yyvsp[-1].command_argument_list_);
    argument_list->push_back((yyvsp[0].string_value_));
    (yyval.command_argument_list_) = argument_list;
  }
#line 4858 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 222:
#line 1639 "../SqlParser.ypp" /* yacc.c:1646  */
    { /* Epsilon, an empy match. */
    (yyval.command_argument_list_) = new quickstep::PtrVector<quickstep::ParseString>();
  }
#line 4866 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;


#line 4870 "SqlParser_gen.cpp" /* yacc.c:1646  */
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
#line 1643 "../SqlParser.ypp" /* yacc.c:1906  */


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
