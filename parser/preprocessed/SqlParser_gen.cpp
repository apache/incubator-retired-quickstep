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
    TOKEN_HAVING = 314,
    TOKEN_INDEX = 315,
    TOKEN_INNER = 316,
    TOKEN_INSERT = 317,
    TOKEN_INTEGER = 318,
    TOKEN_INTERVAL = 319,
    TOKEN_INTO = 320,
    TOKEN_JOIN = 321,
    TOKEN_KEY = 322,
    TOKEN_LAST = 323,
    TOKEN_LEFT = 324,
    TOKEN_LIMIT = 325,
    TOKEN_LONG = 326,
    TOKEN_NULL = 327,
    TOKEN_NULLS = 328,
    TOKEN_OFF = 329,
    TOKEN_ON = 330,
    TOKEN_ORDER = 331,
    TOKEN_OUTER = 332,
    TOKEN_PERCENT = 333,
    TOKEN_PRIMARY = 334,
    TOKEN_QUIT = 335,
    TOKEN_REAL = 336,
    TOKEN_REFERENCES = 337,
    TOKEN_RIGHT = 338,
    TOKEN_ROW_DELIMITER = 339,
    TOKEN_SELECT = 340,
    TOKEN_SET = 341,
    TOKEN_SMALLINT = 342,
    TOKEN_TABLE = 343,
    TOKEN_TIME = 344,
    TOKEN_TIMESTAMP = 345,
    TOKEN_TRUE = 346,
    TOKEN_TUPLESAMPLE = 347,
    TOKEN_UNIQUE = 348,
    TOKEN_UPDATE = 349,
    TOKEN_USING = 350,
    TOKEN_VALUES = 351,
    TOKEN_VARCHAR = 352,
    TOKEN_WHERE = 353,
    TOKEN_WITH = 354,
    TOKEN_YEARMONTH = 355,
    TOKEN_EOF = 356,
    TOKEN_LEX_ERROR = 357
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

#line 373 "SqlParser_gen.cpp" /* yacc.c:355  */
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

#line 406 "SqlParser_gen.cpp" /* yacc.c:358  */

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
#define YYLAST   916

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  114
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  85
/* YYNRULES -- Number of rules.  */
#define YYNRULES  226
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  429

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   357

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     109,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   113,     2,     2,
     110,   111,    23,    21,   112,    22,    27,    24,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   108,
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
     100,   101,   102,   103,   104,   105,   106,   107
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   519,   519,   523,   527,   531,   535,   538,   545,   548,
     551,   554,   557,   560,   563,   566,   569,   572,   578,   584,
     591,   597,   604,   613,   618,   623,   628,   633,   637,   643,
     648,   651,   654,   659,   662,   665,   668,   671,   674,   677,
     680,   683,   686,   698,   701,   704,   722,   742,   745,   748,
     753,   758,   764,   770,   779,   783,   789,   792,   797,   802,
     807,   814,   821,   825,   831,   834,   839,   842,   850,   853,
     858,   862,   868,   871,   874,   879,   882,   889,   894,   905,
     909,   915,   918,   924,   932,   937,   942,   945,   950,   954,
     958,   962,   968,   973,   978,   982,   988,   994,   999,  1002,
    1007,  1011,  1017,  1023,  1029,  1032,  1036,  1042,  1045,  1050,
    1054,  1060,  1063,  1066,  1071,  1076,  1079,  1085,  1089,  1095,
    1101,  1107,  1113,  1119,  1125,  1131,  1137,  1145,  1150,  1153,
    1156,  1161,  1165,  1169,  1172,  1176,  1181,  1184,  1189,  1192,
    1197,  1201,  1207,  1210,  1215,  1218,  1223,  1226,  1231,  1234,
    1253,  1257,  1263,  1270,  1273,  1276,  1281,  1284,  1287,  1293,
    1296,  1301,  1306,  1315,  1320,  1329,  1334,  1337,  1342,  1345,
    1350,  1356,  1362,  1365,  1371,  1374,  1379,  1382,  1387,  1390,
    1395,  1398,  1401,  1404,  1409,  1413,  1417,  1422,  1426,  1432,
    1435,  1438,  1441,  1453,  1457,  1476,  1491,  1495,  1501,  1504,
    1510,  1513,  1516,  1519,  1522,  1525,  1528,  1531,  1534,  1537,
    1542,  1553,  1556,  1561,  1564,  1567,  1573,  1577,  1583,  1586,
    1594,  1597,  1600,  1603,  1609,  1614,  1619
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
  "TOKEN_FOREIGN", "TOKEN_FROM", "TOKEN_FULL", "TOKEN_GROUP",
  "TOKEN_HAVING", "TOKEN_INDEX", "TOKEN_INNER", "TOKEN_INSERT",
  "TOKEN_INTEGER", "TOKEN_INTERVAL", "TOKEN_INTO", "TOKEN_JOIN",
  "TOKEN_KEY", "TOKEN_LAST", "TOKEN_LEFT", "TOKEN_LIMIT", "TOKEN_LONG",
  "TOKEN_NULL", "TOKEN_NULLS", "TOKEN_OFF", "TOKEN_ON", "TOKEN_ORDER",
  "TOKEN_OUTER", "TOKEN_PERCENT", "TOKEN_PRIMARY", "TOKEN_QUIT",
  "TOKEN_REAL", "TOKEN_REFERENCES", "TOKEN_RIGHT", "TOKEN_ROW_DELIMITER",
  "TOKEN_SELECT", "TOKEN_SET", "TOKEN_SMALLINT", "TOKEN_TABLE",
  "TOKEN_TIME", "TOKEN_TIMESTAMP", "TOKEN_TRUE", "TOKEN_TUPLESAMPLE",
  "TOKEN_UNIQUE", "TOKEN_UPDATE", "TOKEN_USING", "TOKEN_VALUES",
  "TOKEN_VARCHAR", "TOKEN_WHERE", "TOKEN_WITH", "TOKEN_YEARMONTH",
  "TOKEN_EOF", "TOKEN_LEX_ERROR", "';'", "'\\n'", "'('", "')'", "','",
  "'%'", "$accept", "start", "sql_statement", "quit_statement",
  "alter_table_statement", "create_table_statement",
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
     275,    43,    45,    42,    47,   276,   277,    46,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,    59,    10,
      40,    41,    44,    37
};
# endif

#define YYPACT_NINF -200

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-200)))

#define YYTABLE_NINF -99

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     124,  -200,  -200,   -71,   104,   -28,   -23,   -38,   -11,  -200,
     104,   104,  -200,   133,    28,  -200,  -200,  -200,  -200,  -200,
    -200,  -200,  -200,  -200,  -200,    47,    -5,   143,   104,  -200,
    -200,   135,   104,   104,   104,   104,   104,   101,    82,  -200,
     176,    95,  -200,  -200,  -200,     7,  -200,  -200,  -200,  -200,
      12,   207,   142,   115,   157,  -200,   -49,   104,   104,   119,
     104,  -200,  -200,   425,   159,   174,   127,   104,   104,   500,
    -200,  -200,   152,   104,    81,  -200,   224,  -200,    47,  -200,
     122,  -200,  -200,  -200,   235,   243,  -200,  -200,  -200,   155,
    -200,   184,  -200,  -200,  -200,  -200,   265,  -200,  -200,  -200,
    -200,  -200,  -200,   164,   209,   566,   277,   229,   181,  -200,
     182,     3,  -200,  -200,  -200,  -200,  -200,   632,   -13,   104,
     -16,   104,   104,   186,  -200,   201,  -200,   125,   798,   698,
     500,   278,   285,  -200,  -200,   894,   292,   748,   128,   104,
    -200,   566,   203,  -200,   104,  -200,  -200,   309,  -200,  -200,
     313,  -200,    -2,  -200,     9,   157,   566,  -200,  -200,   104,
     566,  -200,  -200,  -200,  -200,   566,   243,  -200,   104,   359,
    -200,   211,   251,   254,   217,  -200,  -200,  -200,   126,   104,
     230,   -16,   104,  -200,    89,  -200,     1,    74,   500,   500,
     202,  -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,   566,
     566,     2,  -200,   132,   228,  -200,   225,  -200,  -200,   220,
     221,  -200,    45,  -200,    77,    45,    34,   270,  -200,  -200,
       3,  -200,  -200,   223,  -200,   225,   137,   500,   227,   232,
     104,   330,   -22,   140,   147,   226,  -200,   231,   234,  -200,
     262,   237,   748,  -200,   268,   104,  -200,  -200,    89,  -200,
    -200,   285,  -200,  -200,  -200,   566,   136,   225,   267,  -200,
    -200,   748,   238,  -200,  -200,   104,  -200,  -200,    78,   274,
     104,    85,    99,     9,  -200,   144,  -200,  -200,   342,   347,
      45,   317,   295,  -200,  -200,   566,    10,   104,   104,   156,
    -200,  -200,  -200,  -200,  -200,  -200,  -200,   139,  -200,  -200,
    -200,   250,   -16,   326,  -200,  -200,   500,  -200,  -200,   257,
    -200,   150,   566,  -200,  -200,   748,  -200,   104,   299,   104,
     -46,   104,   300,   104,   302,  -200,  -200,   291,   293,  -200,
     566,   500,   297,   225,  -200,   169,   187,  -200,   374,   -22,
     104,  -200,  -200,   273,    13,   104,   566,   225,   189,   -41,
     104,   -37,   500,   -35,   104,   -34,   104,  -200,  -200,   272,
     278,   350,   311,   304,  -200,  -200,  -200,   191,  -200,  -200,
    -200,  -200,     4,   104,  -200,   283,   225,  -200,   500,   -18,
     500,   278,   500,    -7,   500,    20,   566,   388,  -200,   104,
    -200,   104,  -200,  -200,   104,  -200,   194,  -200,   278,   500,
     278,   278,   500,   278,   500,   284,  -200,   131,  -200,   287,
    -200,   196,  -200,   278,   278,   278,   566,  -200,  -200,   320,
     104,  -200,  -200,    55,  -200,   198,  -200,  -200,  -200
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     6,   226,     0,     0,     0,     0,     0,     0,    18,
       0,     0,     7,     0,     0,    15,     8,    10,    11,    13,
      14,     9,    17,    12,    16,     0,     0,   224,     0,   218,
     219,     0,     0,     0,     0,     0,     0,     0,    99,   100,
       0,   138,     1,     3,     2,   104,    97,     5,     4,   225,
       0,     0,     0,     0,   159,    25,     0,     0,     0,     0,
       0,   105,   106,     0,     0,     0,    86,     0,     0,     0,
      93,   160,     0,     0,   159,    95,     0,   101,     0,   102,
       0,   216,   193,   190,     0,   210,   107,    40,    29,     0,
      30,    31,    34,    36,    37,    39,     0,    41,   189,    35,
      38,    32,    33,     0,     0,     0,     0,     0,   108,   109,
     113,   175,   177,   179,   182,   181,   180,     0,   198,     0,
       0,     0,     0,     0,    85,    66,    27,     0,     0,     0,
       0,   161,   163,   165,   167,     0,   180,     0,     0,     0,
      92,     0,     0,   139,     0,   191,   192,     0,    43,   194,
       0,    44,     0,   195,     0,   159,     0,   211,   212,     0,
       0,   112,   214,   215,   213,     0,     0,   178,     0,     0,
      19,     0,     0,     0,     0,    20,    21,    22,     0,     0,
       0,    64,     0,    42,    56,   166,     0,     0,     0,     0,
       0,   200,   202,   203,   204,   205,   201,   206,   208,     0,
       0,     0,   196,     0,     0,    94,    96,   127,   217,     0,
       0,   183,     0,   140,   115,   135,   128,   142,   110,   111,
     174,   176,   199,     0,   184,   187,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    65,    68,    28,
       0,     0,     0,    47,     0,     0,    49,    55,    57,    26,
     173,   162,   164,   207,   209,     0,     0,   172,     0,   171,
      84,     0,     0,    45,    46,     0,   131,   136,     0,     0,
       0,     0,     0,     0,   114,   116,   118,   134,     0,     0,
     133,     0,   144,   185,   186,     0,     0,     0,     0,     0,
      88,   222,   223,   221,   220,    89,    87,     0,    67,    79,
      80,    81,     0,     0,    23,    48,     0,    51,    50,     0,
      54,     0,     0,   170,   197,     0,   137,     0,     0,     0,
       0,     0,     0,     0,     0,   141,   117,     0,     0,   132,
       0,     0,   146,   188,    61,     0,     0,    58,     0,     0,
       0,    24,    62,     0,     0,     0,     0,   168,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   129,   130,   143,
     145,     0,   148,     0,    59,    90,    91,     0,    70,    72,
      73,    74,     0,     0,    52,     0,   169,    83,     0,     0,
       0,   120,     0,     0,     0,     0,     0,     0,   103,     0,
      82,     0,    78,    76,     0,    75,     0,    53,   126,     0,
     119,   122,     0,   124,     0,   147,   150,   153,   149,     0,
      71,     0,    69,   125,   121,   123,     0,   154,   155,   156,
       0,    77,   151,     0,   152,     0,   157,   158,    60
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,  -102,  -200,
     275,   153,  -200,  -200,  -180,  -200,  -200,  -200,  -200,    27,
      11,  -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,
    -200,  -200,  -200,   269,  -200,  -200,  -200,   346,   331,  -200,
    -200,  -200,   255,  -200,  -200,  -200,   145,   351,  -200,   141,
    -199,    -8,  -200,  -200,  -200,  -200,  -200,  -200,    -1,  -200,
    -200,   -26,  -200,  -107,   233,   236,   288,   -58,   252,   258,
     305,  -150,    94,  -135,   111,   -27,  -200,  -200,  -200,  -200,
     -32,    -4,    80,  -200,  -200
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    13,    14,    15,    16,    17,    18,    19,   126,   127,
     106,   247,   248,   249,   175,   237,   238,   180,   304,   367,
     368,   369,   370,   371,   301,   341,    20,    21,   124,   233,
      22,    23,    74,    75,    24,    25,    38,    39,    46,    63,
     107,   108,   109,   155,   274,   275,   276,   212,   280,   213,
     266,   267,   214,   282,   332,   362,   388,   405,   406,   419,
     424,    70,    71,   131,   132,   133,   134,   135,   111,   112,
     113,   114,   226,   115,   203,   116,   200,   117,   160,   165,
      80,   118,   295,    26,    27
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      31,   236,   202,    40,   215,   110,    37,    41,    29,   188,
      30,   392,   258,    29,   168,    30,   277,   170,   188,   157,
     158,   188,    28,   186,    50,   171,   162,   163,    52,    53,
      54,    55,    56,   393,   352,   291,    61,    32,    34,   378,
      64,   138,   136,   380,   172,   382,   384,   152,   140,    29,
      40,    30,    72,    76,    41,    35,    81,   292,   293,    36,
      62,    73,   399,   125,   128,    33,   144,    65,   173,    81,
     278,   144,   187,   402,   294,   144,   265,   144,   144,   259,
     239,   329,   174,   206,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   144,   157,   158,   169,   110,   240,
     404,    47,   136,   136,    48,   144,   161,   307,    29,   211,
      30,   225,   250,   426,   394,   128,   164,   176,   177,    78,
     286,   334,   342,   215,   374,     1,   314,     2,   427,   217,
     241,   279,   144,    42,    43,    76,    44,    45,   242,   268,
     208,   256,   257,   269,   169,   312,    49,   234,   270,   317,
     216,   271,   157,   158,     3,   219,   321,   157,   158,   346,
     318,   136,   136,   417,   222,   272,   243,   322,     4,     5,
     323,   157,   158,   244,     6,    81,   245,   231,   128,     7,
     202,   324,   232,   418,    69,   211,    29,   246,    30,   273,
     338,     8,    57,   139,    58,   339,    51,   311,   289,   344,
     136,   119,   120,   157,   158,    60,   268,    59,    41,     9,
     269,    41,    66,   159,   -98,   270,   121,   122,   271,   253,
     254,   255,    67,    10,   360,    68,    81,   333,    11,    78,
      12,   123,   272,   143,   144,   141,   181,   182,   320,   204,
     144,   309,   145,   260,   261,   381,   157,   158,   284,   285,
     146,   296,   297,   148,   347,   335,   336,   316,   298,   144,
      69,    41,   137,   299,   300,   147,    81,   337,   144,   216,
     149,   398,   225,   400,   150,   401,    41,   403,   151,   136,
     363,   144,   153,    81,    81,   349,   188,   351,   376,   353,
     154,   355,   413,   156,   189,   414,   178,   415,   364,   144,
     377,   261,   390,   391,   136,   412,   391,   421,   144,   428,
     144,   179,   201,    81,   207,    81,   209,    81,   379,    81,
     210,   227,   383,   228,   385,   136,   229,   230,   407,   262,
     235,   263,   264,   281,   283,   290,   372,   287,   303,   305,
     308,   375,   288,   302,   313,   319,    81,   306,   315,   327,
      81,   136,    81,   136,   328,   136,   330,   136,   407,   331,
     340,   343,   411,    29,    82,    30,    83,   345,   395,   372,
     350,   354,   136,   356,   357,   136,   358,   136,   361,   365,
      84,    85,   223,   373,   285,   409,   387,   372,   425,   386,
      81,   389,    87,    88,   397,   408,   416,   420,   423,    89,
     396,   310,   410,   184,    77,    90,    91,    92,   205,   142,
      79,   218,   220,    93,   325,   422,    81,   185,    94,   366,
     326,   251,   167,   221,   359,   252,   348,    95,    96,    29,
      82,    30,    83,     0,     0,    97,    98,     0,     0,     0,
       0,     0,     0,     0,     0,    99,    84,    85,    86,     0,
       0,   100,     0,   101,   102,     0,     0,     0,    87,    88,
       0,   103,     0,     0,   104,    89,     0,     0,     0,   105,
     224,    90,    91,    92,     0,     0,     0,     0,     0,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,     0,     0,     0,     0,     0,
       0,    97,    98,     0,    29,    82,    30,    83,     0,     0,
     129,    99,     0,     0,     0,     0,     0,   100,     0,   101,
     102,    84,    85,     0,     0,     0,     0,   103,     0,     0,
     104,     0,     0,    87,    88,   105,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,    90,    91,    92,     0,
       0,     0,     0,     0,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    96,
      29,    82,    30,    83,     0,     0,    97,    98,     0,     0,
       0,     0,     0,     0,     0,     0,    99,    84,    85,     0,
       0,     0,   100,     0,   101,   102,     0,     0,     0,    87,
      88,     0,   103,     0,     0,   104,    89,     0,     0,     0,
     130,     0,    90,    91,    92,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    29,    82,    30,    83,
       0,     0,    97,    98,     0,     0,     0,     0,     0,     0,
       0,     0,    99,    84,   166,     0,     0,     0,   100,     0,
     101,   102,     0,     0,     0,    87,    88,     0,   103,     0,
       0,   104,    89,     0,     0,     0,   105,     0,    90,    91,
      92,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,    96,    29,    82,    30,    83,     0,     0,    97,    98,
       0,     0,     0,     0,     0,     0,     0,     0,    99,    84,
      85,     0,     0,     0,   100,     0,   101,   102,     0,     0,
       0,    87,    88,     0,   103,     0,     0,   104,    89,     0,
       0,     0,   105,     0,    90,    91,    92,     0,     0,     0,
       0,     0,    93,    82,     0,    83,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,    96,     0,    84,
     166,     0,     0,     0,    97,    98,     0,     0,     0,     0,
       0,    87,    88,     0,    99,     0,     0,     0,    89,     0,
     100,     0,   101,   102,    90,    91,    92,     0,     0,     0,
     103,     0,    93,   104,     0,     0,     0,    94,   130,     0,
       0,     0,     0,     0,     0,     0,    95,    96,     0,     0,
       0,     0,     0,     0,    97,    98,     0,     0,     0,     0,
       0,    87,    88,     0,    99,     0,     0,     0,    89,     0,
     100,     0,   101,   102,    90,    91,    92,     0,     0,     0,
     103,     0,    93,   104,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,   183,     0,     0,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,     0,     0,     0,
     100,     0,   101,   102,     0,     0,     0,     0,     0,     0,
     103,     0,     0,   104,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,     0,   157,   158
};

static const yytype_int16 yycheck[] =
{
       4,   181,   137,    11,   154,    63,    10,    11,     4,     8,
       6,     7,    10,     4,    27,     6,   215,   119,     8,    21,
      22,     8,    93,   130,    28,    41,    23,    24,    32,    33,
      34,    35,    36,    29,    80,    57,    29,    65,    61,    80,
      28,    73,    69,    80,    60,    80,    80,   105,    74,     4,
      58,     6,   101,    57,    58,    93,    60,    79,    80,    70,
      53,   110,    80,    67,    68,    93,   112,    55,    84,    73,
      36,   112,   130,    80,    96,   112,    31,   112,   112,    77,
     182,   280,    98,   141,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,   112,    21,    22,   110,   156,    10,
      80,   106,   129,   130,   109,   112,   110,   242,     4,   111,
       6,   169,   111,    58,   110,   119,   113,   121,   122,   110,
     227,   111,   302,   273,   111,     1,   261,     3,    73,   155,
      41,    97,   112,     0,   106,   139,   108,    90,    49,    62,
     144,   199,   200,    66,   110,     9,     3,   179,    71,    71,
     154,    74,    21,    22,    30,   159,    71,    21,    22,     9,
      82,   188,   189,    32,   168,    88,    77,    82,    44,    45,
      71,    21,    22,    84,    50,   179,    87,    51,   182,    55,
     315,    82,    56,    52,   103,   111,     4,    98,     6,   112,
      51,    67,    91,   112,   112,    56,    61,   255,   230,   306,
     227,    42,    43,    21,    22,   110,    62,    31,   212,    85,
      66,   215,     5,    31,    90,    71,    42,    43,    74,    17,
      18,    19,    80,    99,   331,   110,   230,   285,   104,   110,
     106,   104,    88,   111,   112,    11,   111,   112,   270,   111,
     112,   245,     7,   111,   112,   352,    21,    22,   111,   112,
       7,   111,   112,    69,   312,   287,   288,   265,   111,   112,
     103,   265,   110,    37,    38,   110,   270,   111,   112,   273,
       5,   378,   330,   380,   110,   382,   280,   384,    69,   306,
     111,   112,     5,   287,   288,   317,     8,   319,   346,   321,
      61,   323,   399,   112,     9,   402,   110,   404,   111,   112,
     111,   112,   111,   112,   331,   111,   112,   111,   112,   111,
     112,   110,    20,   317,   111,   319,     7,   321,   350,   323,
       7,   110,   354,    72,   356,   352,    72,   110,   386,   101,
     100,   111,   111,    63,   111,     5,   340,   110,   104,    77,
      72,   345,   110,   112,    77,    71,   350,   110,   110,     7,
     354,   378,   356,   380,     7,   382,    39,   384,   416,    64,
     110,    35,   394,     4,     5,     6,     7,   110,   372,   373,
      71,    71,   399,    71,    83,   402,    83,   404,    81,     5,
      21,    22,    23,   110,   112,   389,    75,   391,   420,    39,
     394,    87,    33,    34,   111,     7,   112,   110,    78,    40,
     373,   248,   391,   128,    58,    46,    47,    48,   139,    78,
      59,   156,   160,    54,   273,   416,   420,   129,    59,   339,
     275,   188,   117,   165,   330,   189,   315,    68,    69,     4,
       5,     6,     7,    -1,    -1,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    21,    22,    23,    -1,
      -1,    92,    -1,    94,    95,    -1,    -1,    -1,    33,    34,
      -1,   102,    -1,    -1,   105,    40,    -1,    -1,    -1,   110,
     111,    46,    47,    48,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    77,    -1,     4,     5,     6,     7,    -1,    -1,
      10,    86,    -1,    -1,    -1,    -1,    -1,    92,    -1,    94,
      95,    21,    22,    -1,    -1,    -1,    -1,   102,    -1,    -1,
     105,    -1,    -1,    33,    34,   110,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    47,    48,    -1,
      -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    69,
       4,     5,     6,     7,    -1,    -1,    76,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    21,    22,    -1,
      -1,    -1,    92,    -1,    94,    95,    -1,    -1,    -1,    33,
      34,    -1,   102,    -1,    -1,   105,    40,    -1,    -1,    -1,
     110,    -1,    46,    47,    48,    -1,    -1,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    68,    69,     4,     5,     6,     7,
      -1,    -1,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    21,    22,    -1,    -1,    -1,    92,    -1,
      94,    95,    -1,    -1,    -1,    33,    34,    -1,   102,    -1,
      -1,   105,    40,    -1,    -1,    -1,   110,    -1,    46,    47,
      48,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      68,    69,     4,     5,     6,     7,    -1,    -1,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    21,
      22,    -1,    -1,    -1,    92,    -1,    94,    95,    -1,    -1,
      -1,    33,    34,    -1,   102,    -1,    -1,   105,    40,    -1,
      -1,    -1,   110,    -1,    46,    47,    48,    -1,    -1,    -1,
      -1,    -1,    54,     5,    -1,     7,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    68,    69,    -1,    21,
      22,    -1,    -1,    -1,    76,    77,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    86,    -1,    -1,    -1,    40,    -1,
      92,    -1,    94,    95,    46,    47,    48,    -1,    -1,    -1,
     102,    -1,    54,   105,    -1,    -1,    -1,    59,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    77,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    86,    -1,    -1,    -1,    40,    -1,
      92,    -1,    94,    95,    46,    47,    48,    -1,    -1,    -1,
     102,    -1,    54,   105,    -1,    -1,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,
     102,    -1,    -1,   105,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    -1,    21,    22
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,    30,    44,    45,    50,    55,    67,    85,
      99,   104,   106,   115,   116,   117,   118,   119,   120,   121,
     140,   141,   144,   145,   148,   149,   197,   198,    93,     4,
       6,   195,    65,    93,    61,    93,    70,   195,   150,   151,
     165,   195,     0,   106,   108,    90,   152,   106,   109,     3,
     195,    61,   195,   195,   195,   195,   195,    91,   112,    31,
     110,    29,    53,   153,    28,    55,     5,    80,   110,   103,
     175,   176,   101,   110,   146,   147,   195,   151,   110,   161,
     194,   195,     5,     7,    21,    22,    23,    33,    34,    40,
      46,    47,    48,    54,    59,    68,    69,    76,    77,    86,
      92,    94,    95,   102,   105,   110,   124,   154,   155,   156,
     181,   182,   183,   184,   185,   187,   189,   191,   195,    42,
      43,    42,    43,   104,   142,   195,   122,   123,   195,    10,
     110,   177,   178,   179,   180,   181,   189,   110,   194,   112,
     175,    11,   152,   111,   112,     7,     7,   110,    69,     5,
     110,    69,   181,     5,    61,   157,   112,    21,    22,    31,
     192,   195,    23,    24,   113,   193,    22,   184,    27,   110,
     122,    41,    60,    84,    98,   128,   195,   195,   110,   110,
     131,   111,   112,    69,   124,   180,   177,   181,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
     190,    20,   187,   188,   111,   147,   181,   111,   195,     7,
       7,   111,   161,   163,   166,   185,   195,   175,   156,   195,
     182,   183,   195,    23,   111,   181,   186,   110,    72,    72,
     110,    51,    56,   143,   194,   100,   128,   129,   130,   122,
      10,    41,    49,    77,    84,    87,    98,   125,   126,   127,
     111,   178,   179,    17,    18,    19,   181,   181,    10,    77,
     111,   112,   101,   111,   111,    31,   164,   165,    62,    66,
      71,    74,    88,   112,   158,   159,   160,   164,    36,    97,
     162,    63,   167,   111,   111,   112,   177,   110,   110,   194,
       5,    57,    79,    80,    96,   196,   111,   112,   111,    37,
      38,   138,   112,   104,   132,    77,   110,   187,    72,   195,
     125,   181,     9,    77,   187,   110,   165,    71,    82,    71,
     194,    71,    82,    71,    82,   163,   160,     7,     7,   164,
      39,    64,   168,   181,   111,   194,   194,   111,    51,    56,
     110,   139,   128,    35,   177,   110,     9,   181,   188,   194,
      71,   194,    80,   194,    71,   194,    71,    83,    83,   186,
     177,    81,   169,   111,   111,     5,   196,   133,   134,   135,
     136,   137,   195,   110,   111,   195,   181,   111,    80,   194,
      80,   177,    80,   194,    80,   194,    39,    75,   170,    87,
     111,   112,     7,    29,   110,   195,   133,   111,   177,    80,
     177,   177,    80,   177,    80,   171,   172,   181,     7,   195,
     134,   194,   111,   177,   177,   177,   112,    32,    52,   173,
     110,   111,   172,    78,   174,   194,    58,    73,   111
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   114,   115,   115,   115,   115,   115,   115,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   117,   118,
     118,   118,   118,   119,   120,   121,   122,   123,   123,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   125,   125,   125,
     125,   125,   125,   125,   126,   126,   127,   127,   128,   128,
     128,   128,   129,   129,   130,   130,   131,   131,   132,   132,
     133,   133,   134,   134,   134,   135,   135,   136,   137,   138,
     138,   139,   139,   140,   140,   141,   142,   142,   143,   143,
     143,   143,   144,   145,   146,   146,   147,   148,   149,   149,
     150,   150,   151,   152,   153,   153,   153,   154,   154,   155,
     155,   156,   156,   156,   157,   158,   158,   159,   159,   160,
     160,   160,   160,   160,   160,   160,   160,   161,   162,   162,
     162,   163,   163,   163,   163,   163,   164,   164,   165,   165,
     166,   166,   167,   167,   168,   168,   169,   169,   170,   170,
     171,   171,   172,   173,   173,   173,   174,   174,   174,   175,
     175,   176,   177,   177,   178,   178,   179,   179,   180,   180,
     180,   180,   180,   180,   181,   181,   182,   182,   183,   183,
     184,   184,   184,   184,   185,   185,   185,   186,   186,   187,
     187,   187,   187,   187,   187,   187,   188,   188,   189,   189,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     191,   192,   192,   193,   193,   193,   194,   194,   195,   195,
     196,   196,   196,   196,   197,   198,   198
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
       3,     2,     3,     2,     2,     1,     1,     2,     1,     4,
       1,     3,     0,     3,     0,     2,     0,     3,     0,     2,
       1,     3,     3,     0,     1,     1,     0,     2,     2,     0,
       1,     2,     3,     1,     3,     1,     2,     1,     5,     6,
       4,     3,     3,     3,     3,     1,     3,     1,     2,     1,
       1,     1,     1,     3,     3,     4,     4,     1,     3,     1,
       1,     2,     2,     1,     2,     2,     1,     3,     1,     3,
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
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1760 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 4: /* TOKEN_NAME  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1770 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 5: /* TOKEN_STRING_SINGLE_QUOTED  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1780 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 6: /* TOKEN_STRING_DOUBLE_QUOTED  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 1790 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 7: /* TOKEN_UNSIGNED_NUMVAL  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).numeric_literal_value_) != nullptr) {
    delete ((*yyvaluep).numeric_literal_value_);
  }
}
#line 1800 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 116: /* sql_statement  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1810 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 117: /* quit_statement  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).quit_statement_) != nullptr) {
    delete ((*yyvaluep).quit_statement_);
  }
}
#line 1820 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 118: /* alter_table_statement  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1830 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 119: /* create_table_statement  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).create_table_statement_) != nullptr) {
    delete ((*yyvaluep).create_table_statement_);
  }
}
#line 1840 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 120: /* create_index_statement  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).statement_) != nullptr) {
    delete ((*yyvaluep).statement_);
  }
}
#line 1850 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 121: /* drop_table_statement  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).drop_table_statement_) != nullptr) {
    delete ((*yyvaluep).drop_table_statement_);
  }
}
#line 1860 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 122: /* column_def  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_);
  }
}
#line 1870 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 123: /* column_def_commalist  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_definition_list_) != nullptr) {
    delete ((*yyvaluep).attribute_definition_list_);
  }
}
#line 1880 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 124: /* data_type  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).data_type_) != nullptr) {
    delete ((*yyvaluep).data_type_);
  }
}
#line 1890 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 125: /* column_constraint_def  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_) != nullptr) {
    delete ((*yyvaluep).column_constraint_);
  }
}
#line 1900 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 126: /* column_constraint_def_list  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1910 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 127: /* opt_column_constraint_def_list  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).column_constraint_list_) != nullptr) {
    delete ((*yyvaluep).column_constraint_list_);
  }
}
#line 1920 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 131: /* opt_column_list  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 1930 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 132: /* opt_block_properties  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).block_properties_) != nullptr) {
    delete ((*yyvaluep).block_properties_);
  }
}
#line 1940 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 133: /* key_value_list  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 1950 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 134: /* key_value  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_) != nullptr) {
    delete ((*yyvaluep).key_value_);
  }
}
#line 1960 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 135: /* key_string_value  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_value_) != nullptr) {
    delete ((*yyvaluep).key_string_value_);
  }
}
#line 1970 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 136: /* key_string_list  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_string_list_) != nullptr) {
    delete ((*yyvaluep).key_string_list_);
  }
}
#line 1980 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 137: /* key_integer_value  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_integer_value_) != nullptr) {
    delete ((*yyvaluep).key_integer_value_);
  }
}
#line 1990 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 138: /* index_type  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2000 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 139: /* opt_index_properties  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).key_value_list_) != nullptr) {
    delete ((*yyvaluep).key_value_list_);
  }
}
#line 2010 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 140: /* insert_statement  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).insert_statement_) != nullptr) {
    delete ((*yyvaluep).insert_statement_);
  }
}
#line 2020 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 141: /* copy_from_statement  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_statement_) != nullptr) {
    delete ((*yyvaluep).copy_from_statement_);
  }
}
#line 2030 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 142: /* opt_copy_from_params  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2040 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 143: /* copy_from_params  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).copy_from_params_) != nullptr) {
    delete ((*yyvaluep).copy_from_params_);
  }
}
#line 2050 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 144: /* update_statement  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).update_statement_) != nullptr) {
    delete ((*yyvaluep).update_statement_);
  }
}
#line 2060 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 145: /* delete_statement  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).delete_statement_) != nullptr) {
    delete ((*yyvaluep).delete_statement_);
  }
}
#line 2070 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 146: /* assignment_list  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_list_) != nullptr) {
    delete ((*yyvaluep).assignment_list_);
  }
}
#line 2080 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 147: /* assignment_item  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).assignment_) != nullptr) {
    delete ((*yyvaluep).assignment_);
  }
}
#line 2090 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 148: /* select_statement  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_statement_) != nullptr) {
    delete ((*yyvaluep).select_statement_);
  }
}
#line 2100 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 149: /* opt_with_clause  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2110 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 150: /* with_list  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_) != nullptr) {
    delete ((*yyvaluep).with_list_);
  }
}
#line 2120 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 151: /* with_list_element  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).with_list_element_) != nullptr) {
    delete ((*yyvaluep).with_list_element_);
  }
}
#line 2130 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 152: /* select_query  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).select_query_) != nullptr) {
    delete ((*yyvaluep).select_query_);
  }
}
#line 2140 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 154: /* selection  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_) != nullptr) {
    delete ((*yyvaluep).selection_);
  }
}
#line 2150 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 155: /* selection_item_commalist  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_list_) != nullptr) {
    delete ((*yyvaluep).selection_list_);
  }
}
#line 2160 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 156: /* selection_item  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).selection_item_) != nullptr) {
    delete ((*yyvaluep).selection_item_);
  }
}
#line 2170 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 157: /* from_clause  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2180 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 161: /* subquery_expression  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).subquery_expression_) != nullptr) {
    delete ((*yyvaluep).subquery_expression_);
  }
}
#line 2190 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 162: /* opt_sample_clause  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_sample_clause_) != nullptr) {
    delete ((*yyvaluep).opt_sample_clause_);
  }
}
#line 2200 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 163: /* table_reference  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_) != nullptr) {
    delete ((*yyvaluep).table_reference_);
  }
}
#line 2210 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 164: /* table_reference_signature  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2220 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 165: /* table_reference_signature_primary  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_signature_) != nullptr) {
    delete ((*yyvaluep).table_reference_signature_);
  }
}
#line 2230 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 166: /* table_reference_commalist  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).table_reference_list_) != nullptr) {
    delete ((*yyvaluep).table_reference_list_);
  }
}
#line 2240 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 167: /* opt_group_by_clause  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_group_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_group_by_clause_);
  }
}
#line 2250 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 168: /* opt_having_clause  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_having_clause_) != nullptr) {
    delete ((*yyvaluep).opt_having_clause_);
  }
}
#line 2260 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 169: /* opt_order_by_clause  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_order_by_clause_) != nullptr) {
    delete ((*yyvaluep).opt_order_by_clause_);
  }
}
#line 2270 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 170: /* opt_limit_clause  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).opt_limit_clause_) != nullptr) {
    delete ((*yyvaluep).opt_limit_clause_);
  }
}
#line 2280 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 171: /* order_commalist  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_commalist_) != nullptr) {
    delete ((*yyvaluep).order_commalist_);
  }
}
#line 2290 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 172: /* order_item  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_item_) != nullptr) {
    delete ((*yyvaluep).order_item_);
  }
}
#line 2300 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 173: /* opt_order_direction  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2310 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 174: /* opt_nulls_first  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).order_direction_) != nullptr) {
    delete ((*yyvaluep).order_direction_);
  }
}
#line 2320 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 175: /* opt_where_clause  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2330 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 176: /* where_clause  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2340 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 177: /* or_expression  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2350 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 178: /* and_expression  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2360 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 179: /* not_expression  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2370 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 180: /* predicate_expression_base  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).predicate_) != nullptr) {
    delete ((*yyvaluep).predicate_);
  }
}
#line 2380 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 181: /* add_expression  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2390 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 182: /* multiply_expression  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2400 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 183: /* unary_expression  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2410 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 184: /* expression_base  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_) != nullptr) {
    delete ((*yyvaluep).expression_);
  }
}
#line 2420 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 185: /* function_call  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).function_call_) != nullptr) {
    delete ((*yyvaluep).function_call_);
  }
}
#line 2430 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 186: /* expression_list  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).expression_list_) != nullptr) {
    delete ((*yyvaluep).expression_list_);
  }
}
#line 2440 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 187: /* literal_value  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_) != nullptr) {
    delete ((*yyvaluep).literal_value_);
  }
}
#line 2450 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 188: /* literal_value_commalist  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).literal_value_list_) != nullptr) {
    delete ((*yyvaluep).literal_value_list_);
  }
}
#line 2460 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 189: /* attribute_ref  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).attribute_) != nullptr) {
    delete ((*yyvaluep).attribute_);
  }
}
#line 2470 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 190: /* comparison_operation  */
#line 506 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2476 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 191: /* unary_operation  */
#line 507 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2482 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 192: /* add_operation  */
#line 508 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2488 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 193: /* multiply_operation  */
#line 508 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2494 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 194: /* name_commalist  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_list_) != nullptr) {
    delete ((*yyvaluep).string_list_);
  }
}
#line 2504 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 195: /* any_name  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).string_value_) != nullptr) {
    delete ((*yyvaluep).string_value_);
  }
}
#line 2514 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 196: /* boolean_value  */
#line 505 "../SqlParser.ypp" /* yacc.c:1257  */
      { }
#line 2520 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 197: /* command  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_) != nullptr) {
    delete ((*yyvaluep).command_);
  }
}
#line 2530 "SqlParser_gen.cpp" /* yacc.c:1257  */
        break;

    case 198: /* command_argument_list  */
#line 510 "../SqlParser.ypp" /* yacc.c:1257  */
      {
  if (((*yyvaluep).command_argument_list_) != nullptr) {
    delete ((*yyvaluep).command_argument_list_);
  }
}
#line 2540 "SqlParser_gen.cpp" /* yacc.c:1257  */
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
#line 519 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2837 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 523 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].statement_);
    YYACCEPT;
  }
#line 2846 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 527 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 2855 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 531 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    *parsedStatement = (yyvsp[-1].command_);
    YYACCEPT;
  }
#line 2864 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 535 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    YYABORT;
  }
#line 2872 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 538 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
#line 2881 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 545 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 2889 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 548 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].copy_from_statement_);
  }
#line 2897 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 551 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].create_table_statement_);
  }
#line 2905 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 554 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].statement_);
  }
#line 2913 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 557 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].delete_statement_);
  }
#line 2921 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 560 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].drop_table_statement_);
  }
#line 2929 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 563 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].insert_statement_);
  }
#line 2937 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 566 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].quit_statement_);
  }
#line 2945 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 569 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].select_statement_);
  }
#line 2953 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 572 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = (yyvsp[0].update_statement_);
  }
#line 2961 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 578 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 2969 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 584 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2981 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 591 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 2992 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 597 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3004 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 604 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[0].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[-5]), yyscanner, "ALTER statements");
    YYERROR;
  }
#line 3016 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 613 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[-7]).first_line, (yylsp[-7]).first_column, (yyvsp[-5].string_value_), (yyvsp[-3].attribute_definition_list_), (yyvsp[0].block_properties_));
  }
#line 3024 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 618 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].string_value_), (yyvsp[-4].string_value_), (yyvsp[-3].string_list_), (yyvsp[-1].string_value_), (yyvsp[0].key_value_list_));
  }
#line 3032 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 623 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_));
  }
#line 3040 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 628 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].data_type_), (yyvsp[0].column_constraint_list_));
  }
#line 3048 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 633 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3057 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 637 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_definition_list_) = (yyvsp[-2].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[0].attribute_definition_));
  }
#line 3066 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 643 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "BIT data type");
    YYERROR;
  }
#line 3076 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 648 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3084 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 651 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3092 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 654 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "TIME data type");
    YYERROR;
  }
#line 3102 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 659 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
#line 3110 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 662 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3118 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 665 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3126 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 668 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
#line 3134 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 671 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
#line 3142 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 674 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3150 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 677 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
#line 3158 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 680 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3166 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 683 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
#line 3174 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 686 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3191 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 698 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
#line 3199 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 701 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
#line 3207 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 704 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3230 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 722 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 3253 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 742 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3261 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 745 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[-1]).first_line, (yylsp[-1]).first_column);
  }
#line 3269 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 748 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[0]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
#line 3279 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 753 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3289 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 758 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[0].literal_value_);
    NotSupported(&(yylsp[-1]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
#line 3300 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 764 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
#line 3311 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 770 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_value_);
    NotSupported(&(yylsp[-4]), yyscanner, "Foreign Keys");
    YYERROR;
  }
#line 3323 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 779 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = (yyvsp[-1].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3332 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 783 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[0].column_constraint_));
  }
#line 3341 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 789 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = nullptr;
  }
#line 3349 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 792 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.column_constraint_list_) = (yyvsp[0].column_constraint_list_);
  }
#line 3357 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 797 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
#line 3367 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 802 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-4]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
#line 3377 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 807 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-6].string_list_);
    delete (yyvsp[-3].string_value_);
    delete (yyvsp[-1].string_list_);
    NotSupported(&(yylsp[-9]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
#line 3389 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 814 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
#line 3399 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 821 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[-2]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3408 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 825 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "Table Constraints");
    YYERROR;
  }
#line 3417 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 831 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 3425 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 834 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = $1; */
  }
#line 3433 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 839 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = nullptr;
  }
#line 3441 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 842 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].string_list_);
    (yyval.string_list_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "list of column names in CREATE INDEX statement");
    YYERROR;
  }
#line 3452 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 850 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.block_properties_) = nullptr;
  }
#line 3460 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 853 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].key_value_list_));
  }
#line 3468 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 858 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3477 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 862 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = (yyvsp[-2].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[0].key_value_));
  }
#line 3486 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 868 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_value_);
  }
#line 3494 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 871 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_string_list_);
  }
#line 3502 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 874 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_) = (yyvsp[0].key_integer_value_);
  }
#line 3510 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 879 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].string_value_));
  }
#line 3518 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 882 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), 
        new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column, "ALL"));
  }
#line 3528 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 889 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 3536 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 894 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].numeric_literal_value_)->float_like()) {
      delete (yyvsp[0].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].numeric_literal_value_));
  }
#line 3550 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 905 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
#line 3559 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 909 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[0]).first_line, (yylsp[0]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
#line 3568 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 915 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = nullptr;
  }
#line 3576 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 918 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.key_value_list_) = (yyvsp[-1].key_value_list_);
  }
#line 3584 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 924 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-7].string_value_);
    delete (yyvsp[-5].string_list_);
    delete (yyvsp[-1].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[-6]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
#line 3597 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 932 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsert((yylsp[-6]).first_line, (yylsp[-6]).first_column, (yyvsp[-4].string_value_), (yyvsp[-1].literal_value_list_));
  }
#line 3605 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 937 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_statement_) = new quickstep::ParseStatementCopyFrom((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_value_), (yyvsp[0].copy_from_params_));
  }
#line 3613 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 942 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = nullptr;
  }
#line 3621 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 945 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-1].copy_from_params_);
  }
#line 3629 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 950 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3638 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 954 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[-1]).first_line, (yylsp[-1]).first_column);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3647 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 958 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[0].string_value_));
  }
#line 3656 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 962 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.copy_from_params_) = (yyvsp[-3].copy_from_params_);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[0].boolean_value_);
  }
#line 3665 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 968 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[-4]).first_line, (yylsp[-4]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].assignment_list_), (yyvsp[0].predicate_));
  }
#line 3673 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 973 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].predicate_));
  }
#line 3681 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 978 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_list_) = (yyvsp[-2].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3690 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 982 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[0].assignment_));
  }
#line 3699 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 988 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[0].expression_));
  }
#line 3707 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 994 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].select_query_), (yyvsp[-1].with_list_));
  }
#line 3715 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 999 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = nullptr;
  }
#line 3723 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 1002 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = (yyvsp[0].with_list_);
  }
#line 3731 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 1007 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3740 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 1011 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_) = (yyvsp[-2].with_list_);
    (yyval.with_list_)->push_back((yyvsp[0].with_list_element_));
  }
#line 3749 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 1017 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[-2].table_reference_signature_));
  }
#line 3758 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 1024 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[-8]).first_line, (yylsp[-8]).first_column, (yyvsp[-6].selection_), (yyvsp[-5].table_reference_list_), (yyvsp[-4].predicate_), (yyvsp[-3].opt_group_by_clause_), (yyvsp[-2].opt_having_clause_), (yyvsp[-1].opt_order_by_clause_), (yyvsp[0].opt_limit_clause_));
  }
#line 3766 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 1029 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 3774 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 1032 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "ALL in selection");
    YYERROR;
  }
#line 3783 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 1036 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "DISTINCT in selection");
    YYERROR;
  }
#line 3792 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 1042 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 3800 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 1045 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_) = (yyvsp[0].selection_list_);
  }
#line 3808 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 1050 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[0]).first_line, (yylsp[0]).first_column);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3817 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 1054 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_list_) = (yyvsp[-2].selection_list_);
    (yyval.selection_list_)->add((yyvsp[0].selection_item_));
  }
#line 3826 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 1060 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[0].string_value_));
  }
#line 3834 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 1063 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].expression_), (yyvsp[0].string_value_));
  }
#line 3842 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 1066 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].expression_));
  }
#line 3850 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 1071 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = (yyvsp[-1].table_reference_list_);
  }
#line 3858 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 1076 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /* $$ = nullptr; */
  }
#line 3866 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 1079 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3875 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 1085 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[-1]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3884 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 1089 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    NotSupported(&(yylsp[0]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3893 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 1095 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3904 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 1101 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-3]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
#line 3915 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 1107 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3926 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 1113 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3937 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 1119 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3948 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 1125 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3959 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 1131 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-5]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3970 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 1137 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].string_list_);
    delete (yyvsp[0].predicate_);
    NotSupported(&(yylsp[-4]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
#line 3981 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 1145 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-1].select_query_));
  }
#line 3989 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 1150 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_sample_clause_) = NULL;
  }
#line 3997 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 1153 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, true, (yyvsp[-1].numeric_literal_value_));
  }
#line 4005 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 1156 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[-2]).first_line, (yylsp[-2]).first_column, false, (yyvsp[-1].numeric_literal_value_));
  }
#line 4013 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 1161 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4022 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 1165 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), (yyvsp[-1].opt_sample_clause_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4031 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 1169 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].opt_sample_clause_));
  }
#line 4039 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 1172 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[0].table_reference_signature_));
  }
#line 4048 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 1176 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].function_call_));
  }
#line 4056 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 1181 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4064 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 1184 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = (yyvsp[0].table_reference_signature_);
  }
#line 4072 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 1189 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4080 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 1192 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].string_list_));
  }
#line 4088 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 1197 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4097 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 1201 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.table_reference_list_) = (yyvsp[-2].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[0].table_reference_));
  }
#line 4106 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 1207 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
#line 4114 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 1210 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].expression_list_));
  }
#line 4122 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 1215 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_having_clause_) = nullptr;
  }
#line 4130 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 1218 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4138 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 1223 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
#line 4146 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 1226 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].order_commalist_));
  }
#line 4154 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 148:
#line 1231 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
#line 4162 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 149:
#line 1234 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4184 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 150:
#line 1253 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4193 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 1257 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_commalist_) = (yyvsp[-2].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[0].order_item_));
  }
#line 4202 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 1263 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].expression_), (yyvsp[-1].order_direction_), (yyvsp[0].order_direction_));
    delete (yyvsp[-1].order_direction_);
    delete (yyvsp[0].order_direction_);
  }
#line 4212 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 1270 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4220 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 1273 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4228 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 1276 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4236 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 1281 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = nullptr;
  }
#line 4244 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 1284 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(true);
  }
#line 4252 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 158:
#line 1287 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.order_direction_) = new bool(false);
  }
#line 4260 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 159:
#line 1293 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = nullptr;
  }
#line 4268 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 1296 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4276 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 161:
#line 1301 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4284 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 1306 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4298 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 163:
#line 1315 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4306 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 1320 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[-2].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[-2].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[-2]).first_line, (yylsp[-2]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[-2].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[0].predicate_));
  }
#line 4320 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 165:
#line 1329 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4328 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 166:
#line 1334 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[0].predicate_));
  }
#line 4336 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 1337 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[0].predicate_);
  }
#line 4344 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 168:
#line 1342 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-4].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4352 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 169:
#line 1345 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[-4]).first_line, (yylsp[-4]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-5].expression_), (yyvsp[-2].expression_), (yyvsp[0].expression_)));
  }
#line 4362 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 1350 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-3].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-2]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4373 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 171:
#line 1356 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[-1]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
#line 4384 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 172:
#line 1362 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].comparison_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4392 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 173:
#line 1365 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.predicate_) = (yyvsp[-1].predicate_);
  }
#line 4400 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 174:
#line 1371 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4408 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 175:
#line 1374 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4416 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 176:
#line 1379 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].binary_operation_), (yyvsp[-2].expression_), (yyvsp[0].expression_));
  }
#line 4424 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 177:
#line 1382 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4432 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 178:
#line 1387 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[-1]).first_line, (yylsp[-1]).first_column, *(yyvsp[-1].unary_operation_), (yyvsp[0].expression_));
  }
#line 4440 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 1390 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].expression_);
  }
#line 4448 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 180:
#line 1395 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].attribute_);
  }
#line 4456 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 181:
#line 1398 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_));
  }
#line 4464 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 1401 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[0].function_call_);
  }
#line 4472 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 183:
#line 1404 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_) = (yyvsp[-1].expression_);
  }
#line 4480 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 184:
#line 1409 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[-2].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
#line 4489 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 185:
#line 1413 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), new quickstep::ParseStar((yylsp[-1]).first_line, (yylsp[-1]).first_column));
  }
#line 4498 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 186:
#line 1417 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[-3]).first_line, (yylsp[-3]).first_column, (yyvsp[-3].string_value_), (yyvsp[-1].expression_list_));
  }
#line 4506 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 187:
#line 1422 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4515 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 1426 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.expression_list_) = (yyvsp[-2].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[0].expression_));
  }
#line 4524 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 189:
#line 1432 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[0]).first_line, (yylsp[0]).first_column);
  }
#line 4532 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 190:
#line 1435 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4540 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 191:
#line 1438 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = (yyvsp[0].numeric_literal_value_);
  }
#line 4548 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 192:
#line 1441 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4565 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 193:
#line 1453 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[0].string_value_),
                                                nullptr);  // No explicit type.
  }
#line 4574 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 194:
#line 1457 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4598 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 195:
#line 1476 "../SqlParser.ypp" /* yacc.c:1646  */
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
#line 4616 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 196:
#line 1491 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4625 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 197:
#line 1495 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.literal_value_list_) = (yyvsp[-2].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[0].literal_value_)));
  }
#line 4634 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 198:
#line 1501 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[0]).first_line, (yylsp[0]).first_column, (yyvsp[0].string_value_));
  }
#line 4642 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 199:
#line 1504 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[-2]).first_line, (yylsp[-2]).first_column, (yyvsp[0].string_value_), (yyvsp[-2].string_value_));
  }
#line 4650 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 200:
#line 1510 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
#line 4658 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 201:
#line 1513 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
#line 4666 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 202:
#line 1516 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
#line 4674 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 203:
#line 1519 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
#line 4682 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 204:
#line 1522 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
#line 4690 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 205:
#line 1525 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
#line 4698 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 206:
#line 1528 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLike);
  }
#line 4706 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 1531 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotLike);
  }
#line 4714 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 208:
#line 1534 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kRegexMatch);
  }
#line 4722 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 209:
#line 1537 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotRegexMatch);
  }
#line 4730 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 210:
#line 1542 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
#line 4744 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 211:
#line 1553 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
#line 4752 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 212:
#line 1556 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
#line 4760 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 213:
#line 1561 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kModulo);
  }
#line 4768 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 214:
#line 1564 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
#line 4776 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 215:
#line 1567 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
#line 4784 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 216:
#line 1573 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4793 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 217:
#line 1577 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_list_) = (yyvsp[-2].string_list_);
    (yyval.string_list_)->push_back((yyvsp[0].string_value_));
  }
#line 4802 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 218:
#line 1583 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 4810 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 219:
#line 1586 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    if ((yyvsp[0].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[0]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[0].string_value_);
  }
#line 4821 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 220:
#line 1594 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4829 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 221:
#line 1597 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = true;
  }
#line 4837 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 222:
#line 1600 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;
  }
#line 4845 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 223:
#line 1603 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.boolean_value_) = false;
  }
#line 4853 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 224:
#line 1609 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    (yyval.command_) = new quickstep::ParseCommand((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yyvsp[-1].string_value_), (yyvsp[0].command_argument_list_));
  }
#line 4861 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 225:
#line 1614 "../SqlParser.ypp" /* yacc.c:1646  */
    {
    quickstep::PtrVector<quickstep::ParseString> *argument_list = (yyvsp[-1].command_argument_list_);
    argument_list->push_back((yyvsp[0].string_value_));
    (yyval.command_argument_list_) = argument_list;
  }
#line 4871 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;

  case 226:
#line 1619 "../SqlParser.ypp" /* yacc.c:1646  */
    { /* Epsilon, an empy match. */
    (yyval.command_argument_list_) = new quickstep::PtrVector<quickstep::ParseString>();
  }
#line 4879 "SqlParser_gen.cpp" /* yacc.c:1646  */
    break;


#line 4883 "SqlParser_gen.cpp" /* yacc.c:1646  */
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
#line 1623 "../SqlParser.ypp" /* yacc.c:1906  */


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
