/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1

/* Substitute the variable and function names.  */
#define yyparse         quickstep_yyparse
#define yylex           quickstep_yylex
#define yyerror         quickstep_yyerror
#define yylval          quickstep_yylval
#define yychar          quickstep_yychar
#define yydebug         quickstep_yydebug
#define yynerrs         quickstep_yynerrs
#define yylloc          quickstep_yylloc

/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 33 "../SqlParser.ypp"


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


/* Line 268 of yacc.c  */
#line 62 "../SqlParser.ypp"

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


/* Line 268 of yacc.c  */
#line 158 "SqlParser_gen.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOKEN_COMMAND = 258,
     TOKEN_NAME = 259,
     TOKEN_STRING_SINGLE_QUOTED = 260,
     TOKEN_STRING_DOUBLE_QUOTED = 261,
     TOKEN_UNSIGNED_NUMVAL = 262,
     TOKEN_OR = 263,
     TOKEN_AND = 264,
     TOKEN_NOT = 265,
     TOKEN_EQ = 266,
     TOKEN_NEQ = 267,
     TOKEN_GEQ = 268,
     TOKEN_GT = 269,
     TOKEN_LEQ = 270,
     TOKEN_LT = 271,
     TOKEN_REGEXP = 272,
     TOKEN_LIKE = 273,
     TOKEN_BETWEEN = 274,
     TOKEN_IS = 275,
     UNARY_MINUS = 276,
     UNARY_PLUS = 277,
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



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 110 "../SqlParser.ypp"

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



/* Line 293 of yacc.c  */
#line 381 "SqlParser_gen.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */

/* Line 343 of yacc.c  */
#line 193 "../SqlParser.ypp"

/* This header needs YYSTYPE, which is defined by the %union directive above */
#include "SqlLexer_gen.hpp"
void NotSupported(const YYLTYPE *location, yyscan_t yyscanner, const std::string &feature);


/* Line 343 of yacc.c  */
#line 413 "SqlParser_gen.cpp"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  42
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   919

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  114
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  86
/* YYNRULES -- Number of rules.  */
#define YYNRULES  228
/* YYNRULES -- Number of states.  */
#define YYNSTATES  433

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   357

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
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
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     9,    12,    15,    17,    19,    21,
      23,    25,    27,    29,    31,    33,    35,    37,    39,    41,
      48,    55,    62,    69,    78,    88,    92,    96,    98,   102,
     104,   106,   108,   110,   112,   114,   116,   118,   120,   122,
     124,   126,   128,   130,   133,   136,   141,   146,   148,   151,
     153,   156,   159,   164,   170,   173,   175,   176,   178,   183,
     189,   200,   205,   209,   211,   212,   214,   215,   219,   220,
     226,   228,   232,   234,   236,   238,   241,   244,   249,   252,
     254,   256,   257,   261,   272,   280,   286,   287,   292,   295,
     298,   303,   308,   314,   319,   323,   325,   329,   332,   333,
     336,   338,   342,   346,   356,   357,   359,   361,   363,   365,
     367,   371,   375,   378,   380,   384,   385,   387,   390,   392,
     398,   403,   410,   416,   423,   429,   436,   442,   446,   447,
     451,   455,   458,   462,   465,   468,   470,   472,   475,   477,
     482,   484,   488,   489,   493,   494,   497,   498,   502,   503,
     506,   508,   512,   516,   517,   519,   521,   522,   525,   528,
     529,   531,   534,   538,   540,   544,   546,   549,   551,   557,
     564,   569,   573,   577,   581,   585,   587,   591,   593,   596,
     598,   600,   602,   604,   608,   612,   617,   622,   624,   628,
     630,   632,   635,   638,   640,   643,   646,   648,   652,   654,
     658,   660,   664,   666,   668,   670,   672,   674,   676,   678,
     681,   683,   686,   688,   690,   692,   694,   696,   698,   700,
     704,   706,   708,   710,   712,   714,   716,   719,   722
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     115,     0,    -1,   116,   108,    -1,   116,   106,    -1,   198,
     109,    -1,   198,   106,    -1,     1,    -1,   106,    -1,   118,
      -1,   141,    -1,   119,    -1,   120,    -1,   145,    -1,   121,
      -1,   140,    -1,   117,    -1,   148,    -1,   144,    -1,    85,
      -1,    30,    93,   196,    28,    42,   122,    -1,    30,    93,
     196,    28,    43,   128,    -1,    30,    93,   196,    55,    42,
     196,    -1,    30,    93,   196,    55,    43,   196,    -1,    45,
      93,   196,   110,   123,   111,   130,   132,    -1,    45,    65,
     196,    80,   196,   131,   100,   138,   139,    -1,    55,    93,
     196,    -1,   196,   124,   127,    -1,   122,    -1,   123,   112,
     122,    -1,    34,    -1,    46,    -1,    47,    -1,    94,    -1,
      95,    -1,    48,    -1,    86,    -1,    54,    -1,    59,    -1,
      92,    -1,    68,    -1,    33,    -1,    76,    -1,    69,    -1,
      47,    69,    -1,   105,    69,    -1,    40,   110,     7,   111,
      -1,   102,   110,     7,   111,    -1,    77,    -1,    10,    77,
      -1,    98,    -1,    84,    72,    -1,    49,   187,    -1,    41,
     110,   177,   111,    -1,    87,   196,   110,   196,   111,    -1,
     126,   125,    -1,   125,    -1,    -1,   126,    -1,    98,   110,
     195,   111,    -1,    84,    72,   110,   195,   111,    -1,    60,
      72,   110,   195,   111,    87,   196,   110,   195,   111,    -1,
      41,   110,   177,   111,    -1,   129,   112,   128,    -1,   128,
      -1,    -1,   129,    -1,    -1,   110,   190,   111,    -1,    -1,
     104,    35,   110,   133,   111,    -1,   134,    -1,   133,   112,
     134,    -1,   135,    -1,   136,    -1,   137,    -1,   196,   196,
      -1,   196,    29,    -1,   196,   110,   195,   111,    -1,   196,
       7,    -1,    37,    -1,    38,    -1,    -1,   110,   133,   111,
      -1,    67,    70,   196,   110,   195,   111,   101,   110,   188,
     111,    -1,    67,    70,   196,   101,   110,   188,   111,    -1,
      44,   196,    61,     5,   142,    -1,    -1,   104,   110,   143,
     111,    -1,    51,     5,    -1,    56,   197,    -1,   143,   112,
      51,     5,    -1,   143,   112,    56,   197,    -1,    99,   196,
      91,   146,   175,    -1,    50,    61,   196,   175,    -1,   146,
     112,   147,    -1,   147,    -1,   196,    11,   181,    -1,   149,
     152,    -1,    -1,   104,   150,    -1,   151,    -1,   150,   112,
     151,    -1,   165,    31,   161,    -1,    90,   153,   154,   157,
     175,   167,   168,   169,   170,    -1,    -1,    29,    -1,    53,
      -1,    23,    -1,   155,    -1,   156,    -1,   155,   112,   156,
      -1,   181,    31,   196,    -1,   181,   196,    -1,   181,    -1,
      61,   166,   158,    -1,    -1,   159,    -1,   159,   160,    -1,
     160,    -1,    66,    71,   195,    80,   177,    -1,    71,   195,
      80,   177,    -1,    74,    82,    71,   195,    80,   177,    -1,
      74,    71,   195,    80,   177,    -1,    88,    82,    71,   195,
      80,   177,    -1,    88,    71,   195,    80,   177,    -1,    62,
      82,    71,   195,    80,   177,    -1,    62,    71,   195,    80,
     177,    -1,   110,   152,   111,    -1,    -1,    36,     7,    83,
      -1,    97,     7,    83,    -1,   161,   164,    -1,   196,   162,
     164,    -1,   196,   162,    -1,   185,   164,    -1,   185,    -1,
     165,    -1,    31,   165,    -1,   196,    -1,   196,   110,   195,
     111,    -1,   163,    -1,   166,   112,   163,    -1,    -1,    63,
      39,   186,    -1,    -1,    64,   177,    -1,    -1,    81,    39,
     171,    -1,    -1,    75,     7,    -1,   172,    -1,   171,   112,
     172,    -1,   181,   173,   174,    -1,    -1,    32,    -1,    52,
      -1,    -1,    78,    58,    -1,    78,    73,    -1,    -1,   176,
      -1,   103,   177,    -1,   177,     8,   178,    -1,   178,    -1,
     178,     9,   179,    -1,   179,    -1,    10,   180,    -1,   180,
      -1,   181,    19,   181,     9,   181,    -1,   181,    10,    19,
     181,     9,   181,    -1,   189,    20,    10,    77,    -1,   189,
      20,    77,    -1,   181,   191,   181,    -1,   110,   177,   111,
      -1,   181,   193,   182,    -1,   182,    -1,   182,   194,   183,
      -1,   183,    -1,   192,   184,    -1,   184,    -1,   189,    -1,
     187,    -1,   185,    -1,   110,   181,   111,    -1,   196,   110,
     111,    -1,   196,   110,    23,   111,    -1,   196,   110,   186,
     111,    -1,   181,    -1,   186,   112,   181,    -1,    77,    -1,
       7,    -1,    21,     7,    -1,    22,     7,    -1,     5,    -1,
      69,     5,    -1,   124,     5,    -1,   187,    -1,   188,   112,
     187,    -1,   196,    -1,   196,    27,   196,    -1,   189,    -1,
     190,   112,   189,    -1,    11,    -1,    12,    -1,    16,    -1,
      15,    -1,    14,    -1,    13,    -1,    18,    -1,    10,    18,
      -1,    17,    -1,    10,    17,    -1,    22,    -1,    21,    -1,
      22,    -1,   113,    -1,    23,    -1,    24,    -1,   196,    -1,
     195,   112,   196,    -1,     4,    -1,     6,    -1,    96,    -1,
      80,    -1,    57,    -1,    79,    -1,     3,   199,    -1,   199,
       3,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   523,   523,   527,   531,   535,   539,   542,   549,   552,
     555,   558,   561,   564,   567,   570,   573,   576,   582,   588,
     595,   601,   608,   617,   622,   631,   636,   641,   645,   651,
     656,   659,   662,   667,   670,   673,   676,   679,   682,   685,
     688,   691,   694,   706,   709,   712,   730,   750,   753,   756,
     761,   766,   772,   778,   787,   791,   797,   800,   805,   810,
     815,   822,   829,   833,   839,   842,   847,   850,   855,   858,
     863,   867,   873,   876,   879,   884,   887,   894,   899,   910,
     914,   920,   923,   929,   937,   942,   947,   950,   955,   959,
     963,   967,   973,   978,   983,   987,   993,   999,  1004,  1007,
    1012,  1016,  1022,  1028,  1034,  1037,  1041,  1047,  1050,  1055,
    1059,  1065,  1068,  1071,  1076,  1081,  1084,  1090,  1094,  1100,
    1106,  1112,  1118,  1124,  1130,  1136,  1142,  1150,  1155,  1158,
    1161,  1166,  1170,  1174,  1177,  1181,  1186,  1189,  1194,  1197,
    1202,  1206,  1212,  1215,  1220,  1223,  1228,  1231,  1236,  1239,
    1258,  1262,  1268,  1275,  1278,  1281,  1286,  1289,  1292,  1298,
    1301,  1306,  1311,  1320,  1325,  1334,  1339,  1342,  1347,  1350,
    1355,  1361,  1367,  1370,  1376,  1379,  1384,  1387,  1392,  1395,
    1400,  1403,  1406,  1409,  1414,  1418,  1422,  1427,  1431,  1437,
    1440,  1443,  1446,  1458,  1462,  1481,  1496,  1500,  1506,  1509,
    1514,  1518,  1525,  1528,  1531,  1534,  1537,  1540,  1543,  1546,
    1549,  1552,  1557,  1568,  1571,  1576,  1579,  1582,  1588,  1592,
    1598,  1601,  1609,  1612,  1615,  1618,  1624,  1629,  1634
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOKEN_COMMAND", "TOKEN_NAME",
  "TOKEN_STRING_SINGLE_QUOTED", "TOKEN_STRING_DOUBLE_QUOTED",
  "TOKEN_UNSIGNED_NUMVAL", "TOKEN_OR", "TOKEN_AND", "TOKEN_NOT",
  "TOKEN_EQ", "TOKEN_NEQ", "TOKEN_GEQ", "TOKEN_GT", "TOKEN_LEQ",
  "TOKEN_LT", "TOKEN_REGEXP", "TOKEN_LIKE", "TOKEN_BETWEEN", "TOKEN_IS",
  "'+'", "'-'", "'*'", "'/'", "UNARY_MINUS", "UNARY_PLUS", "'.'",
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
  "attribute_ref", "attribute_ref_list", "comparison_operation",
  "unary_operation", "add_operation", "multiply_operation",
  "name_commalist", "any_name", "boolean_value", "command",
  "command_argument_list", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
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
     190,   190,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   192,   193,   193,   194,   194,   194,   195,   195,
     196,   196,   197,   197,   197,   197,   198,   199,   199
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
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
       1,     3,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     2,     2,     0
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     6,   228,     0,     0,     0,     0,     0,     0,    18,
       0,     0,     7,     0,     0,    15,     8,    10,    11,    13,
      14,     9,    17,    12,    16,     0,     0,   226,     0,   220,
     221,     0,     0,     0,     0,     0,     0,     0,    99,   100,
       0,   138,     1,     3,     2,   104,    97,     5,     4,   227,
       0,     0,     0,     0,   159,    25,     0,     0,     0,     0,
       0,   105,   106,     0,     0,     0,    86,     0,     0,     0,
      93,   160,     0,     0,   159,    95,     0,   101,     0,   102,
       0,   218,   193,   190,     0,   212,   107,    40,    29,     0,
      30,    31,    34,    36,    37,    39,     0,    41,   189,    35,
      38,    32,    33,     0,     0,     0,     0,     0,   108,   109,
     113,   175,   177,   179,   182,   181,   180,     0,   198,     0,
       0,     0,     0,     0,    85,    66,    27,     0,     0,     0,
       0,   161,   163,   165,   167,     0,   180,     0,     0,     0,
      92,     0,     0,   139,     0,   191,   192,     0,    43,   194,
       0,    44,     0,   195,     0,   159,     0,   213,   214,     0,
       0,   112,   216,   217,   215,     0,     0,   178,     0,     0,
      19,     0,     0,     0,     0,    20,    21,    22,     0,     0,
       0,    64,     0,    42,    56,   166,     0,     0,     0,     0,
       0,   202,   203,   207,   206,   205,   204,   210,   208,     0,
       0,     0,   196,     0,     0,    94,    96,   127,   219,     0,
       0,   183,     0,   140,   115,   135,   128,   142,   110,   111,
     174,   176,   199,     0,   184,   187,     0,     0,     0,     0,
       0,     0,     0,     0,   200,     0,   198,     0,    63,    65,
      68,    28,     0,     0,     0,    47,     0,     0,    49,    55,
      57,    26,   173,   162,   164,   211,   209,     0,     0,   172,
       0,   171,    84,     0,     0,    45,    46,     0,   131,   136,
       0,     0,     0,     0,     0,     0,   114,   116,   118,   134,
       0,     0,   133,     0,   144,   185,   186,     0,     0,     0,
       0,     0,    88,   224,   225,   223,   222,    89,    87,     0,
      67,     0,    79,    80,    81,     0,     0,    23,    48,     0,
      51,    50,     0,    54,     0,     0,   170,   197,     0,   137,
       0,     0,     0,     0,     0,     0,     0,     0,   141,   117,
       0,     0,   132,     0,     0,   146,   188,    61,     0,     0,
      58,     0,     0,   201,     0,    24,    62,     0,     0,     0,
       0,   168,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   129,   130,   143,   145,     0,   148,     0,    59,    90,
      91,     0,    70,    72,    73,    74,     0,     0,    52,     0,
     169,    83,     0,     0,     0,   120,     0,     0,     0,     0,
       0,     0,   103,     0,    82,     0,    78,    76,     0,    75,
       0,    53,   126,     0,   119,   122,     0,   124,     0,   147,
     150,   153,   149,     0,    71,     0,    69,   125,   121,   123,
       0,   154,   155,   156,     0,    77,   151,     0,   152,     0,
     157,   158,    60
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    13,    14,    15,    16,    17,    18,    19,   126,   127,
     106,   249,   250,   251,   175,   239,   240,   180,   307,   371,
     372,   373,   374,   375,   304,   345,    20,    21,   124,   233,
      22,    23,    74,    75,    24,    25,    38,    39,    46,    63,
     107,   108,   109,   155,   276,   277,   278,   212,   282,   213,
     268,   269,   214,   284,   335,   366,   392,   409,   410,   423,
     428,    70,    71,   131,   132,   133,   134,   135,   111,   112,
     113,   114,   226,   115,   203,   116,   235,   200,   117,   160,
     165,    80,   118,   297,    26,    27
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -194
static const yytype_int16 yypact[] =
{
      40,  -194,  -194,   -55,   206,    -5,    30,    20,    35,  -194,
     206,   206,  -194,   147,   108,  -194,  -194,  -194,  -194,  -194,
    -194,  -194,  -194,  -194,  -194,    19,   -69,   157,   206,  -194,
    -194,   137,   206,   206,   206,   206,   206,   114,    79,  -194,
     171,    99,  -194,  -194,  -194,    -2,  -194,  -194,  -194,  -194,
      66,   212,   170,   142,   145,  -194,   -49,   206,   206,   144,
     206,  -194,  -194,   396,   181,   185,   152,   206,   206,   503,
    -194,  -194,   148,   206,   -46,  -194,   251,  -194,    19,  -194,
      42,  -194,  -194,  -194,   260,   274,  -194,  -194,  -194,   174,
    -194,   219,  -194,  -194,  -194,  -194,   288,  -194,  -194,  -194,
    -194,  -194,  -194,   186,   229,   569,   294,   239,   189,  -194,
     182,    -3,  -194,  -194,  -194,  -194,  -194,   635,   -10,   206,
     -16,   206,   206,   192,  -194,   193,  -194,   109,   801,   701,
     503,   296,   297,  -194,  -194,   897,   285,   751,   121,   206,
    -194,   569,   197,  -194,   206,  -194,  -194,   302,  -194,  -194,
     303,  -194,    12,  -194,    10,   145,   569,  -194,  -194,   206,
     569,  -194,  -194,  -194,  -194,   569,   274,  -194,   206,   392,
    -194,   201,   240,   242,   205,  -194,  -194,  -194,    75,   206,
     217,   -16,   206,  -194,   138,  -194,     1,   155,   503,   503,
     178,  -194,  -194,  -194,  -194,  -194,  -194,  -194,  -194,   569,
     569,    16,  -194,   123,   218,  -194,   216,  -194,  -194,   210,
     213,  -194,    70,  -194,   118,    70,   -18,   262,  -194,  -194,
      -3,  -194,  -194,   215,  -194,   216,   128,   503,   220,   221,
     206,   318,   -21,   130,  -194,   133,   301,   209,  -194,   222,
     231,  -194,   256,   226,   751,  -194,   265,   206,  -194,  -194,
     138,  -194,  -194,   297,  -194,  -194,  -194,   569,   115,   216,
     264,  -194,  -194,   751,   232,  -194,  -194,   206,  -194,  -194,
      -9,   268,   206,    67,   112,    10,  -194,   119,  -194,  -194,
     336,   337,    70,   307,   283,  -194,  -194,   569,     3,   206,
     206,   149,  -194,  -194,  -194,  -194,  -194,  -194,  -194,   100,
    -194,   206,  -194,  -194,   238,   -16,   314,  -194,  -194,   503,
    -194,  -194,   241,  -194,   136,   569,  -194,  -194,   751,  -194,
     206,   281,   206,   -41,   206,   287,   206,   292,  -194,  -194,
     282,   284,  -194,   569,   503,   289,   216,  -194,   153,   158,
    -194,   359,   -21,  -194,   206,  -194,  -194,   258,    11,   206,
     569,   216,   161,   -35,   206,   -34,   503,   -32,   206,   -31,
     206,  -194,  -194,   254,   296,   330,   299,   290,  -194,  -194,
    -194,   165,  -194,  -194,  -194,  -194,     6,   206,  -194,   267,
     216,  -194,   503,   -25,   503,   296,   503,   -15,   503,   -13,
     569,   364,  -194,   206,  -194,   206,  -194,  -194,   206,  -194,
     168,  -194,   296,   503,   296,   296,   503,   296,   503,   263,
    -194,   111,  -194,   270,  -194,   179,  -194,   296,   296,   296,
     569,  -194,  -194,   304,   206,  -194,  -194,    46,  -194,   183,
    -194,  -194,  -194
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -194,  -194,  -194,  -194,  -194,  -194,  -194,  -194,   -96,  -194,
     255,   134,  -194,  -194,  -177,  -194,  -194,  -194,  -194,     8,
       0,  -194,  -194,  -194,  -194,  -194,  -194,  -194,  -194,  -194,
    -194,  -194,  -194,   248,  -194,  -194,  -194,   335,   326,  -194,
    -194,  -194,   249,  -194,  -194,  -194,   129,   349,  -194,   141,
    -193,    -8,  -194,  -194,  -194,  -194,  -194,  -194,   -11,  -194,
    -194,   -59,  -194,     4,   223,   233,   295,   -58,   261,   266,
     306,  -146,    94,  -136,   110,   -27,  -194,  -194,  -194,  -194,
    -194,   -71,    -4,    91,  -194,  -194
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -99
static const yytype_int16 yytable[] =
{
      31,   202,   138,    40,   238,   110,    37,    41,   215,   188,
      29,   188,    30,   396,    29,   140,    30,   168,   280,   188,
     162,   163,   279,   170,    50,   171,   260,    61,    52,    53,
      54,    55,    56,   157,   158,   397,   293,    47,    28,   356,
      48,     1,   136,     2,   172,   382,   384,   152,   386,   388,
      40,    62,    72,    76,    41,   403,    81,    69,   294,   295,
      32,    73,   320,   125,   128,   406,   139,   408,   173,    81,
       3,   144,   187,   321,    29,   296,    30,   144,   144,   281,
     144,   144,   174,   206,     4,     5,   241,   144,    33,   332,
       6,    34,   169,   261,    64,     7,   217,   144,   110,   144,
     169,   267,   136,   136,   430,    36,   161,     8,   310,    45,
     164,   225,   252,    35,   337,   128,   398,   176,   177,   431,
      78,    65,   378,   211,   315,     9,   231,   317,   346,   215,
     -98,   232,   157,   158,   186,    76,   157,   158,   324,    10,
     208,   258,   259,   421,    11,   350,    12,    42,   242,   325,
     216,   341,   234,   143,   144,   219,   342,   157,   158,   291,
      49,   136,   136,   422,   222,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   236,   157,   158,   128,   243,
     270,   270,   202,   326,   271,   271,    29,   244,    30,   272,
     272,    58,   273,   273,   327,   255,   256,   257,    51,   314,
     136,   323,    59,   157,   158,    57,   274,   274,    41,    60,
      29,    41,    30,   159,    43,   245,    44,    66,   338,   339,
     181,   182,   246,   119,   120,   247,    81,   121,   122,   336,
     275,   288,   204,   144,   262,   263,   248,   157,   158,   286,
     287,   298,   299,   312,   300,   301,   302,   303,    69,   353,
      67,   355,    68,   357,    78,   359,   123,   351,   137,   319,
     340,   144,   141,    41,   367,   144,   211,   145,    81,   368,
     144,   216,   381,   263,   343,   225,   394,   395,    41,   416,
     395,   146,   136,   383,   147,    81,    81,   387,   148,   389,
     425,   144,   380,   149,   432,   144,   150,   236,   151,   153,
     154,   156,   178,   179,   188,   201,   189,   136,   207,   209,
     210,   227,   228,   348,   229,   230,    81,   237,    81,   264,
      81,   265,    81,   292,   266,   283,   285,   415,   168,   136,
     289,   290,   411,   308,   305,   306,   309,   311,   364,   322,
     376,   316,   318,   330,   331,   379,   333,   334,   344,   347,
      81,   349,   354,   429,    81,   136,    81,   136,   358,   136,
     385,   136,   411,   360,   369,   361,   287,   362,   377,   390,
     365,   412,   399,   376,   391,   420,   136,   393,   401,   136,
     424,   136,   427,   184,   313,   400,   402,   205,   404,   413,
     405,   376,   407,    77,    81,   414,    29,    82,    30,    83,
      29,    82,    30,    83,   142,   218,   329,   417,    79,   426,
     418,   253,   419,    84,    85,   223,   328,    84,    85,    86,
      81,   220,   254,   167,   185,    87,    88,   363,   352,    87,
      88,   221,    89,   370,     0,     0,    89,     0,    90,    91,
      92,     0,    90,    91,    92,     0,    93,     0,     0,     0,
      93,    94,     0,     0,     0,    94,     0,     0,     0,     0,
      95,    96,     0,     0,    95,    96,     0,     0,    97,    98,
       0,     0,    97,    98,     0,     0,     0,     0,    99,     0,
       0,     0,    99,     0,   100,     0,   101,   102,   100,     0,
     101,   102,     0,     0,   103,     0,     0,   104,   103,     0,
       0,   104,   105,   224,     0,     0,   105,    29,    82,    30,
      83,     0,     0,   129,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    84,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,    90,
      91,    92,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,    96,    29,    82,    30,    83,     0,     0,    97,
      98,     0,     0,     0,     0,     0,     0,     0,     0,    99,
      84,    85,     0,     0,     0,   100,     0,   101,   102,     0,
       0,     0,    87,    88,     0,   103,     0,     0,   104,    89,
       0,     0,     0,   130,     0,    90,    91,    92,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,    94,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    29,
      82,    30,    83,     0,     0,    97,    98,     0,     0,     0,
       0,     0,     0,     0,     0,    99,    84,   166,     0,     0,
       0,   100,     0,   101,   102,     0,     0,     0,    87,    88,
       0,   103,     0,     0,   104,    89,     0,     0,     0,   105,
       0,    90,    91,    92,     0,     0,     0,     0,     0,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    29,    82,    30,    83,     0,
       0,    97,    98,     0,     0,     0,     0,     0,     0,     0,
       0,    99,    84,    85,     0,     0,     0,   100,     0,   101,
     102,     0,     0,     0,    87,    88,     0,   103,     0,     0,
     104,    89,     0,     0,     0,   105,     0,    90,    91,    92,
       0,     0,     0,     0,     0,    93,    82,     0,    83,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,     0,    84,   166,     0,     0,     0,    97,    98,     0,
       0,     0,     0,     0,    87,    88,     0,    99,     0,     0,
       0,    89,     0,   100,     0,   101,   102,    90,    91,    92,
       0,     0,     0,   103,     0,    93,   104,     0,     0,     0,
      94,   130,     0,     0,     0,     0,     0,     0,     0,    95,
      96,     0,     0,     0,     0,     0,     0,    97,    98,     0,
       0,     0,     0,     0,    87,    88,     0,    99,     0,     0,
       0,    89,     0,   100,     0,   101,   102,    90,    91,    92,
       0,     0,     0,   103,     0,    93,   104,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
     183,     0,     0,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
       0,     0,     0,   100,     0,   101,   102,     0,     0,     0,
       0,     0,     0,   103,     0,     0,   104,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,     0,   157,   158
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-194))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       4,   137,    73,    11,   181,    63,    10,    11,   154,     8,
       4,     8,     6,     7,     4,    74,     6,    27,    36,     8,
      23,    24,   215,   119,    28,    41,    10,    29,    32,    33,
      34,    35,    36,    21,    22,    29,    57,   106,    93,    80,
     109,     1,    69,     3,    60,    80,    80,   105,    80,    80,
      58,    53,   101,    57,    58,    80,    60,   103,    79,    80,
      65,   110,    71,    67,    68,    80,   112,    80,    84,    73,
      30,   112,   130,    82,     4,    96,     6,   112,   112,    97,
     112,   112,    98,   141,    44,    45,   182,   112,    93,   282,
      50,    61,   110,    77,    28,    55,   155,   112,   156,   112,
     110,    31,   129,   130,    58,    70,   110,    67,   244,    90,
     113,   169,   111,    93,   111,   119,   110,   121,   122,    73,
     110,    55,   111,   111,     9,    85,    51,   263,   305,   275,
      90,    56,    21,    22,   130,   139,    21,    22,    71,    99,
     144,   199,   200,    32,   104,     9,   106,     0,    10,    82,
     154,    51,   179,   111,   112,   159,    56,    21,    22,   230,
       3,   188,   189,    52,   168,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,   179,    21,    22,   182,    41,
      62,    62,   318,    71,    66,    66,     4,    49,     6,    71,
      71,   112,    74,    74,    82,    17,    18,    19,    61,   257,
     227,   272,    31,    21,    22,    91,    88,    88,   212,   110,
       4,   215,     6,    31,   106,    77,   108,     5,   289,   290,
     111,   112,    84,    42,    43,    87,   230,    42,    43,   287,
     112,   227,   111,   112,   111,   112,    98,    21,    22,   111,
     112,   111,   112,   247,   111,   112,    37,    38,   103,   320,
      80,   322,   110,   324,   110,   326,   104,   315,   110,   267,
     111,   112,    11,   267,   111,   112,   111,     7,   272,   111,
     112,   275,   111,   112,   301,   333,   111,   112,   282,   111,
     112,     7,   309,   354,   110,   289,   290,   358,    69,   360,
     111,   112,   350,     5,   111,   112,   110,   301,    69,     5,
      61,   112,   110,   110,     8,    20,     9,   334,   111,     7,
       7,   110,    72,   309,    72,   110,   320,   100,   322,   101,
     324,   111,   326,     5,   111,    63,   111,   398,    27,   356,
     110,   110,   390,    77,   112,   104,   110,    72,   334,    71,
     344,    77,   110,     7,     7,   349,    39,    64,   110,    35,
     354,   110,    71,   424,   358,   382,   360,   384,    71,   386,
     356,   388,   420,    71,     5,    83,   112,    83,   110,    39,
      81,     7,   376,   377,    75,   112,   403,    87,   111,   406,
     110,   408,    78,   128,   250,   377,   382,   139,   384,   393,
     386,   395,   388,    58,   398,   395,     4,     5,     6,     7,
       4,     5,     6,     7,    78,   156,   277,   403,    59,   420,
     406,   188,   408,    21,    22,    23,   275,    21,    22,    23,
     424,   160,   189,   117,   129,    33,    34,   333,   318,    33,
      34,   165,    40,   342,    -1,    -1,    40,    -1,    46,    47,
      48,    -1,    46,    47,    48,    -1,    54,    -1,    -1,    -1,
      54,    59,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,
      68,    69,    -1,    -1,    68,    69,    -1,    -1,    76,    77,
      -1,    -1,    76,    77,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    86,    -1,    92,    -1,    94,    95,    92,    -1,
      94,    95,    -1,    -1,   102,    -1,    -1,   105,   102,    -1,
      -1,   105,   110,   111,    -1,    -1,   110,     4,     5,     6,
       7,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      47,    48,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    68,    69,     4,     5,     6,     7,    -1,    -1,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      21,    22,    -1,    -1,    -1,    92,    -1,    94,    95,    -1,
      -1,    -1,    33,    34,    -1,   102,    -1,    -1,   105,    40,
      -1,    -1,    -1,   110,    -1,    46,    47,    48,    -1,    -1,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    69,     4,
       5,     6,     7,    -1,    -1,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    21,    22,    -1,    -1,
      -1,    92,    -1,    94,    95,    -1,    -1,    -1,    33,    34,
      -1,   102,    -1,    -1,   105,    40,    -1,    -1,    -1,   110,
      -1,    46,    47,    48,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    68,    69,     4,     5,     6,     7,    -1,
      -1,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    21,    22,    -1,    -1,    -1,    92,    -1,    94,
      95,    -1,    -1,    -1,    33,    34,    -1,   102,    -1,    -1,
     105,    40,    -1,    -1,    -1,   110,    -1,    46,    47,    48,
      -1,    -1,    -1,    -1,    -1,    54,     5,    -1,     7,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      69,    -1,    21,    22,    -1,    -1,    -1,    76,    77,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    86,    -1,    -1,
      -1,    40,    -1,    92,    -1,    94,    95,    46,    47,    48,
      -1,    -1,    -1,   102,    -1,    54,   105,    -1,    -1,    -1,
      59,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    77,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    86,    -1,    -1,
      -1,    40,    -1,    92,    -1,    94,    95,    46,    47,    48,
      -1,    -1,    -1,   102,    -1,    54,   105,    -1,    -1,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    94,    95,    -1,    -1,    -1,
      -1,    -1,    -1,   102,    -1,    -1,   105,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    -1,    21,    22
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,    30,    44,    45,    50,    55,    67,    85,
      99,   104,   106,   115,   116,   117,   118,   119,   120,   121,
     140,   141,   144,   145,   148,   149,   198,   199,    93,     4,
       6,   196,    65,    93,    61,    93,    70,   196,   150,   151,
     165,   196,     0,   106,   108,    90,   152,   106,   109,     3,
     196,    61,   196,   196,   196,   196,   196,    91,   112,    31,
     110,    29,    53,   153,    28,    55,     5,    80,   110,   103,
     175,   176,   101,   110,   146,   147,   196,   151,   110,   161,
     195,   196,     5,     7,    21,    22,    23,    33,    34,    40,
      46,    47,    48,    54,    59,    68,    69,    76,    77,    86,
      92,    94,    95,   102,   105,   110,   124,   154,   155,   156,
     181,   182,   183,   184,   185,   187,   189,   192,   196,    42,
      43,    42,    43,   104,   142,   196,   122,   123,   196,    10,
     110,   177,   178,   179,   180,   181,   189,   110,   195,   112,
     175,    11,   152,   111,   112,     7,     7,   110,    69,     5,
     110,    69,   181,     5,    61,   157,   112,    21,    22,    31,
     193,   196,    23,    24,   113,   194,    22,   184,    27,   110,
     122,    41,    60,    84,    98,   128,   196,   196,   110,   110,
     131,   111,   112,    69,   124,   180,   177,   181,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
     191,    20,   187,   188,   111,   147,   181,   111,   196,     7,
       7,   111,   161,   163,   166,   185,   196,   175,   156,   196,
     182,   183,   196,    23,   111,   181,   186,   110,    72,    72,
     110,    51,    56,   143,   189,   190,   196,   100,   128,   129,
     130,   122,    10,    41,    49,    77,    84,    87,    98,   125,
     126,   127,   111,   178,   179,    17,    18,    19,   181,   181,
      10,    77,   111,   112,   101,   111,   111,    31,   164,   165,
      62,    66,    71,    74,    88,   112,   158,   159,   160,   164,
      36,    97,   162,    63,   167,   111,   111,   112,   177,   110,
     110,   195,     5,    57,    79,    80,    96,   197,   111,   112,
     111,   112,    37,    38,   138,   112,   104,   132,    77,   110,
     187,    72,   196,   125,   181,     9,    77,   187,   110,   165,
      71,    82,    71,   195,    71,    82,    71,    82,   163,   160,
       7,     7,   164,    39,    64,   168,   181,   111,   195,   195,
     111,    51,    56,   189,   110,   139,   128,    35,   177,   110,
       9,   181,   188,   195,    71,   195,    80,   195,    71,   195,
      71,    83,    83,   186,   177,    81,   169,   111,   111,     5,
     197,   133,   134,   135,   136,   137,   196,   110,   111,   196,
     181,   111,    80,   195,    80,   177,    80,   195,    80,   195,
      39,    75,   170,    87,   111,   112,     7,    29,   110,   196,
     133,   111,   177,    80,   177,   177,    80,   177,    80,   171,
     172,   181,     7,   196,   134,   195,   111,   177,   177,   177,
     112,    32,    52,   173,   110,   111,   172,    78,   174,   195,
      58,    73,   111
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (&yylloc, yyscanner, parsedStatement, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc, yyscanner)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location, yyscanner, parsedStatement); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t yyscanner, quickstep::ParseStatement **parsedStatement)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, yyscanner, parsedStatement)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    yyscan_t yyscanner;
    quickstep::ParseStatement **parsedStatement;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (yyscanner);
  YYUSE (parsedStatement);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t yyscanner, quickstep::ParseStatement **parsedStatement)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, yyscanner, parsedStatement)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    yyscan_t yyscanner;
    quickstep::ParseStatement **parsedStatement;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, yyscanner, parsedStatement);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, yyscan_t yyscanner, quickstep::ParseStatement **parsedStatement)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, yyscanner, parsedStatement)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    yyscan_t yyscanner;
    quickstep::ParseStatement **parsedStatement;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , yyscanner, parsedStatement);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, yyscanner, parsedStatement); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
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
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
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

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

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

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, yyscan_t yyscanner, quickstep::ParseStatement **parsedStatement)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, yyscanner, parsedStatement)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    yyscan_t yyscanner;
    quickstep::ParseStatement **parsedStatement;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (yyscanner);
  YYUSE (parsedStatement);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {
      case 3: /* "TOKEN_COMMAND" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->string_value_) != nullptr) {
    delete (yyvaluep->string_value_);
  }
};

/* Line 1391 of yacc.c  */
#line 1924 "SqlParser_gen.cpp"
	break;
      case 4: /* "TOKEN_NAME" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->string_value_) != nullptr) {
    delete (yyvaluep->string_value_);
  }
};

/* Line 1391 of yacc.c  */
#line 1937 "SqlParser_gen.cpp"
	break;
      case 5: /* "TOKEN_STRING_SINGLE_QUOTED" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->string_value_) != nullptr) {
    delete (yyvaluep->string_value_);
  }
};

/* Line 1391 of yacc.c  */
#line 1950 "SqlParser_gen.cpp"
	break;
      case 6: /* "TOKEN_STRING_DOUBLE_QUOTED" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->string_value_) != nullptr) {
    delete (yyvaluep->string_value_);
  }
};

/* Line 1391 of yacc.c  */
#line 1963 "SqlParser_gen.cpp"
	break;
      case 7: /* "TOKEN_UNSIGNED_NUMVAL" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->numeric_literal_value_) != nullptr) {
    delete (yyvaluep->numeric_literal_value_);
  }
};

/* Line 1391 of yacc.c  */
#line 1976 "SqlParser_gen.cpp"
	break;
      case 116: /* "sql_statement" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->statement_) != nullptr) {
    delete (yyvaluep->statement_);
  }
};

/* Line 1391 of yacc.c  */
#line 1989 "SqlParser_gen.cpp"
	break;
      case 117: /* "quit_statement" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->quit_statement_) != nullptr) {
    delete (yyvaluep->quit_statement_);
  }
};

/* Line 1391 of yacc.c  */
#line 2002 "SqlParser_gen.cpp"
	break;
      case 118: /* "alter_table_statement" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->statement_) != nullptr) {
    delete (yyvaluep->statement_);
  }
};

/* Line 1391 of yacc.c  */
#line 2015 "SqlParser_gen.cpp"
	break;
      case 119: /* "create_table_statement" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->create_table_statement_) != nullptr) {
    delete (yyvaluep->create_table_statement_);
  }
};

/* Line 1391 of yacc.c  */
#line 2028 "SqlParser_gen.cpp"
	break;
      case 120: /* "create_index_statement" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->statement_) != nullptr) {
    delete (yyvaluep->statement_);
  }
};

/* Line 1391 of yacc.c  */
#line 2041 "SqlParser_gen.cpp"
	break;
      case 121: /* "drop_table_statement" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->drop_table_statement_) != nullptr) {
    delete (yyvaluep->drop_table_statement_);
  }
};

/* Line 1391 of yacc.c  */
#line 2054 "SqlParser_gen.cpp"
	break;
      case 122: /* "column_def" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->attribute_definition_) != nullptr) {
    delete (yyvaluep->attribute_definition_);
  }
};

/* Line 1391 of yacc.c  */
#line 2067 "SqlParser_gen.cpp"
	break;
      case 123: /* "column_def_commalist" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->attribute_definition_list_) != nullptr) {
    delete (yyvaluep->attribute_definition_list_);
  }
};

/* Line 1391 of yacc.c  */
#line 2080 "SqlParser_gen.cpp"
	break;
      case 124: /* "data_type" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->data_type_) != nullptr) {
    delete (yyvaluep->data_type_);
  }
};

/* Line 1391 of yacc.c  */
#line 2093 "SqlParser_gen.cpp"
	break;
      case 125: /* "column_constraint_def" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->column_constraint_) != nullptr) {
    delete (yyvaluep->column_constraint_);
  }
};

/* Line 1391 of yacc.c  */
#line 2106 "SqlParser_gen.cpp"
	break;
      case 126: /* "column_constraint_def_list" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->column_constraint_list_) != nullptr) {
    delete (yyvaluep->column_constraint_list_);
  }
};

/* Line 1391 of yacc.c  */
#line 2119 "SqlParser_gen.cpp"
	break;
      case 127: /* "opt_column_constraint_def_list" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->column_constraint_list_) != nullptr) {
    delete (yyvaluep->column_constraint_list_);
  }
};

/* Line 1391 of yacc.c  */
#line 2132 "SqlParser_gen.cpp"
	break;
      case 131: /* "opt_column_list" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->attribute_list_) != nullptr) {
    delete (yyvaluep->attribute_list_);
  }
};

/* Line 1391 of yacc.c  */
#line 2145 "SqlParser_gen.cpp"
	break;
      case 132: /* "opt_block_properties" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->block_properties_) != nullptr) {
    delete (yyvaluep->block_properties_);
  }
};

/* Line 1391 of yacc.c  */
#line 2158 "SqlParser_gen.cpp"
	break;
      case 133: /* "key_value_list" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->key_value_list_) != nullptr) {
    delete (yyvaluep->key_value_list_);
  }
};

/* Line 1391 of yacc.c  */
#line 2171 "SqlParser_gen.cpp"
	break;
      case 134: /* "key_value" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->key_value_) != nullptr) {
    delete (yyvaluep->key_value_);
  }
};

/* Line 1391 of yacc.c  */
#line 2184 "SqlParser_gen.cpp"
	break;
      case 135: /* "key_string_value" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->key_string_value_) != nullptr) {
    delete (yyvaluep->key_string_value_);
  }
};

/* Line 1391 of yacc.c  */
#line 2197 "SqlParser_gen.cpp"
	break;
      case 136: /* "key_string_list" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->key_string_list_) != nullptr) {
    delete (yyvaluep->key_string_list_);
  }
};

/* Line 1391 of yacc.c  */
#line 2210 "SqlParser_gen.cpp"
	break;
      case 137: /* "key_integer_value" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->key_integer_value_) != nullptr) {
    delete (yyvaluep->key_integer_value_);
  }
};

/* Line 1391 of yacc.c  */
#line 2223 "SqlParser_gen.cpp"
	break;
      case 138: /* "index_type" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->string_value_) != nullptr) {
    delete (yyvaluep->string_value_);
  }
};

/* Line 1391 of yacc.c  */
#line 2236 "SqlParser_gen.cpp"
	break;
      case 139: /* "opt_index_properties" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->key_value_list_) != nullptr) {
    delete (yyvaluep->key_value_list_);
  }
};

/* Line 1391 of yacc.c  */
#line 2249 "SqlParser_gen.cpp"
	break;
      case 140: /* "insert_statement" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->insert_statement_) != nullptr) {
    delete (yyvaluep->insert_statement_);
  }
};

/* Line 1391 of yacc.c  */
#line 2262 "SqlParser_gen.cpp"
	break;
      case 141: /* "copy_from_statement" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->copy_from_statement_) != nullptr) {
    delete (yyvaluep->copy_from_statement_);
  }
};

/* Line 1391 of yacc.c  */
#line 2275 "SqlParser_gen.cpp"
	break;
      case 142: /* "opt_copy_from_params" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->copy_from_params_) != nullptr) {
    delete (yyvaluep->copy_from_params_);
  }
};

/* Line 1391 of yacc.c  */
#line 2288 "SqlParser_gen.cpp"
	break;
      case 143: /* "copy_from_params" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->copy_from_params_) != nullptr) {
    delete (yyvaluep->copy_from_params_);
  }
};

/* Line 1391 of yacc.c  */
#line 2301 "SqlParser_gen.cpp"
	break;
      case 144: /* "update_statement" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->update_statement_) != nullptr) {
    delete (yyvaluep->update_statement_);
  }
};

/* Line 1391 of yacc.c  */
#line 2314 "SqlParser_gen.cpp"
	break;
      case 145: /* "delete_statement" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->delete_statement_) != nullptr) {
    delete (yyvaluep->delete_statement_);
  }
};

/* Line 1391 of yacc.c  */
#line 2327 "SqlParser_gen.cpp"
	break;
      case 146: /* "assignment_list" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->assignment_list_) != nullptr) {
    delete (yyvaluep->assignment_list_);
  }
};

/* Line 1391 of yacc.c  */
#line 2340 "SqlParser_gen.cpp"
	break;
      case 147: /* "assignment_item" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->assignment_) != nullptr) {
    delete (yyvaluep->assignment_);
  }
};

/* Line 1391 of yacc.c  */
#line 2353 "SqlParser_gen.cpp"
	break;
      case 148: /* "select_statement" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->select_statement_) != nullptr) {
    delete (yyvaluep->select_statement_);
  }
};

/* Line 1391 of yacc.c  */
#line 2366 "SqlParser_gen.cpp"
	break;
      case 149: /* "opt_with_clause" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->with_list_) != nullptr) {
    delete (yyvaluep->with_list_);
  }
};

/* Line 1391 of yacc.c  */
#line 2379 "SqlParser_gen.cpp"
	break;
      case 150: /* "with_list" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->with_list_) != nullptr) {
    delete (yyvaluep->with_list_);
  }
};

/* Line 1391 of yacc.c  */
#line 2392 "SqlParser_gen.cpp"
	break;
      case 151: /* "with_list_element" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->with_list_element_) != nullptr) {
    delete (yyvaluep->with_list_element_);
  }
};

/* Line 1391 of yacc.c  */
#line 2405 "SqlParser_gen.cpp"
	break;
      case 152: /* "select_query" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->select_query_) != nullptr) {
    delete (yyvaluep->select_query_);
  }
};

/* Line 1391 of yacc.c  */
#line 2418 "SqlParser_gen.cpp"
	break;
      case 154: /* "selection" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->selection_) != nullptr) {
    delete (yyvaluep->selection_);
  }
};

/* Line 1391 of yacc.c  */
#line 2431 "SqlParser_gen.cpp"
	break;
      case 155: /* "selection_item_commalist" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->selection_list_) != nullptr) {
    delete (yyvaluep->selection_list_);
  }
};

/* Line 1391 of yacc.c  */
#line 2444 "SqlParser_gen.cpp"
	break;
      case 156: /* "selection_item" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->selection_item_) != nullptr) {
    delete (yyvaluep->selection_item_);
  }
};

/* Line 1391 of yacc.c  */
#line 2457 "SqlParser_gen.cpp"
	break;
      case 157: /* "from_clause" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->table_reference_list_) != nullptr) {
    delete (yyvaluep->table_reference_list_);
  }
};

/* Line 1391 of yacc.c  */
#line 2470 "SqlParser_gen.cpp"
	break;
      case 161: /* "subquery_expression" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->subquery_expression_) != nullptr) {
    delete (yyvaluep->subquery_expression_);
  }
};

/* Line 1391 of yacc.c  */
#line 2483 "SqlParser_gen.cpp"
	break;
      case 162: /* "opt_sample_clause" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->opt_sample_clause_) != nullptr) {
    delete (yyvaluep->opt_sample_clause_);
  }
};

/* Line 1391 of yacc.c  */
#line 2496 "SqlParser_gen.cpp"
	break;
      case 163: /* "table_reference" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->table_reference_) != nullptr) {
    delete (yyvaluep->table_reference_);
  }
};

/* Line 1391 of yacc.c  */
#line 2509 "SqlParser_gen.cpp"
	break;
      case 164: /* "table_reference_signature" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->table_reference_signature_) != nullptr) {
    delete (yyvaluep->table_reference_signature_);
  }
};

/* Line 1391 of yacc.c  */
#line 2522 "SqlParser_gen.cpp"
	break;
      case 165: /* "table_reference_signature_primary" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->table_reference_signature_) != nullptr) {
    delete (yyvaluep->table_reference_signature_);
  }
};

/* Line 1391 of yacc.c  */
#line 2535 "SqlParser_gen.cpp"
	break;
      case 166: /* "table_reference_commalist" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->table_reference_list_) != nullptr) {
    delete (yyvaluep->table_reference_list_);
  }
};

/* Line 1391 of yacc.c  */
#line 2548 "SqlParser_gen.cpp"
	break;
      case 167: /* "opt_group_by_clause" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->opt_group_by_clause_) != nullptr) {
    delete (yyvaluep->opt_group_by_clause_);
  }
};

/* Line 1391 of yacc.c  */
#line 2561 "SqlParser_gen.cpp"
	break;
      case 168: /* "opt_having_clause" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->opt_having_clause_) != nullptr) {
    delete (yyvaluep->opt_having_clause_);
  }
};

/* Line 1391 of yacc.c  */
#line 2574 "SqlParser_gen.cpp"
	break;
      case 169: /* "opt_order_by_clause" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->opt_order_by_clause_) != nullptr) {
    delete (yyvaluep->opt_order_by_clause_);
  }
};

/* Line 1391 of yacc.c  */
#line 2587 "SqlParser_gen.cpp"
	break;
      case 170: /* "opt_limit_clause" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->opt_limit_clause_) != nullptr) {
    delete (yyvaluep->opt_limit_clause_);
  }
};

/* Line 1391 of yacc.c  */
#line 2600 "SqlParser_gen.cpp"
	break;
      case 171: /* "order_commalist" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->order_commalist_) != nullptr) {
    delete (yyvaluep->order_commalist_);
  }
};

/* Line 1391 of yacc.c  */
#line 2613 "SqlParser_gen.cpp"
	break;
      case 172: /* "order_item" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->order_item_) != nullptr) {
    delete (yyvaluep->order_item_);
  }
};

/* Line 1391 of yacc.c  */
#line 2626 "SqlParser_gen.cpp"
	break;
      case 173: /* "opt_order_direction" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->order_direction_) != nullptr) {
    delete (yyvaluep->order_direction_);
  }
};

/* Line 1391 of yacc.c  */
#line 2639 "SqlParser_gen.cpp"
	break;
      case 174: /* "opt_nulls_first" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->order_direction_) != nullptr) {
    delete (yyvaluep->order_direction_);
  }
};

/* Line 1391 of yacc.c  */
#line 2652 "SqlParser_gen.cpp"
	break;
      case 175: /* "opt_where_clause" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->predicate_) != nullptr) {
    delete (yyvaluep->predicate_);
  }
};

/* Line 1391 of yacc.c  */
#line 2665 "SqlParser_gen.cpp"
	break;
      case 176: /* "where_clause" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->predicate_) != nullptr) {
    delete (yyvaluep->predicate_);
  }
};

/* Line 1391 of yacc.c  */
#line 2678 "SqlParser_gen.cpp"
	break;
      case 177: /* "or_expression" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->predicate_) != nullptr) {
    delete (yyvaluep->predicate_);
  }
};

/* Line 1391 of yacc.c  */
#line 2691 "SqlParser_gen.cpp"
	break;
      case 178: /* "and_expression" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->predicate_) != nullptr) {
    delete (yyvaluep->predicate_);
  }
};

/* Line 1391 of yacc.c  */
#line 2704 "SqlParser_gen.cpp"
	break;
      case 179: /* "not_expression" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->predicate_) != nullptr) {
    delete (yyvaluep->predicate_);
  }
};

/* Line 1391 of yacc.c  */
#line 2717 "SqlParser_gen.cpp"
	break;
      case 180: /* "predicate_expression_base" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->predicate_) != nullptr) {
    delete (yyvaluep->predicate_);
  }
};

/* Line 1391 of yacc.c  */
#line 2730 "SqlParser_gen.cpp"
	break;
      case 181: /* "add_expression" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->expression_) != nullptr) {
    delete (yyvaluep->expression_);
  }
};

/* Line 1391 of yacc.c  */
#line 2743 "SqlParser_gen.cpp"
	break;
      case 182: /* "multiply_expression" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->expression_) != nullptr) {
    delete (yyvaluep->expression_);
  }
};

/* Line 1391 of yacc.c  */
#line 2756 "SqlParser_gen.cpp"
	break;
      case 183: /* "unary_expression" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->expression_) != nullptr) {
    delete (yyvaluep->expression_);
  }
};

/* Line 1391 of yacc.c  */
#line 2769 "SqlParser_gen.cpp"
	break;
      case 184: /* "expression_base" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->expression_) != nullptr) {
    delete (yyvaluep->expression_);
  }
};

/* Line 1391 of yacc.c  */
#line 2782 "SqlParser_gen.cpp"
	break;
      case 185: /* "function_call" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->function_call_) != nullptr) {
    delete (yyvaluep->function_call_);
  }
};

/* Line 1391 of yacc.c  */
#line 2795 "SqlParser_gen.cpp"
	break;
      case 186: /* "expression_list" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->expression_list_) != nullptr) {
    delete (yyvaluep->expression_list_);
  }
};

/* Line 1391 of yacc.c  */
#line 2808 "SqlParser_gen.cpp"
	break;
      case 187: /* "literal_value" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->literal_value_) != nullptr) {
    delete (yyvaluep->literal_value_);
  }
};

/* Line 1391 of yacc.c  */
#line 2821 "SqlParser_gen.cpp"
	break;
      case 188: /* "literal_value_commalist" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->literal_value_list_) != nullptr) {
    delete (yyvaluep->literal_value_list_);
  }
};

/* Line 1391 of yacc.c  */
#line 2834 "SqlParser_gen.cpp"
	break;
      case 189: /* "attribute_ref" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->attribute_) != nullptr) {
    delete (yyvaluep->attribute_);
  }
};

/* Line 1391 of yacc.c  */
#line 2847 "SqlParser_gen.cpp"
	break;
      case 190: /* "attribute_ref_list" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->attribute_list_) != nullptr) {
    delete (yyvaluep->attribute_list_);
  }
};

/* Line 1391 of yacc.c  */
#line 2860 "SqlParser_gen.cpp"
	break;
      case 191: /* "comparison_operation" */

/* Line 1391 of yacc.c  */
#line 510 "../SqlParser.ypp"
	{ };

/* Line 1391 of yacc.c  */
#line 2869 "SqlParser_gen.cpp"
	break;
      case 192: /* "unary_operation" */

/* Line 1391 of yacc.c  */
#line 511 "../SqlParser.ypp"
	{ };

/* Line 1391 of yacc.c  */
#line 2878 "SqlParser_gen.cpp"
	break;
      case 193: /* "add_operation" */

/* Line 1391 of yacc.c  */
#line 512 "../SqlParser.ypp"
	{ };

/* Line 1391 of yacc.c  */
#line 2887 "SqlParser_gen.cpp"
	break;
      case 194: /* "multiply_operation" */

/* Line 1391 of yacc.c  */
#line 512 "../SqlParser.ypp"
	{ };

/* Line 1391 of yacc.c  */
#line 2896 "SqlParser_gen.cpp"
	break;
      case 195: /* "name_commalist" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->string_list_) != nullptr) {
    delete (yyvaluep->string_list_);
  }
};

/* Line 1391 of yacc.c  */
#line 2909 "SqlParser_gen.cpp"
	break;
      case 196: /* "any_name" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->string_value_) != nullptr) {
    delete (yyvaluep->string_value_);
  }
};

/* Line 1391 of yacc.c  */
#line 2922 "SqlParser_gen.cpp"
	break;
      case 197: /* "boolean_value" */

/* Line 1391 of yacc.c  */
#line 509 "../SqlParser.ypp"
	{ };

/* Line 1391 of yacc.c  */
#line 2931 "SqlParser_gen.cpp"
	break;
      case 198: /* "command" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->command_) != nullptr) {
    delete (yyvaluep->command_);
  }
};

/* Line 1391 of yacc.c  */
#line 2944 "SqlParser_gen.cpp"
	break;
      case 199: /* "command_argument_list" */

/* Line 1391 of yacc.c  */
#line 514 "../SqlParser.ypp"
	{
  if ((yyvaluep->command_argument_list_) != nullptr) {
    delete (yyvaluep->command_argument_list_);
  }
};

/* Line 1391 of yacc.c  */
#line 2957 "SqlParser_gen.cpp"
	break;

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (yyscan_t yyscanner, quickstep::ParseStatement **parsedStatement);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (yyscan_t yyscanner, quickstep::ParseStatement **parsedStatement)
#else
int
yyparse (yyscanner, parsedStatement)
    yyscan_t yyscanner;
    quickstep::ParseStatement **parsedStatement;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
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
  int yytoken;
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

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

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
      yychar = YYLEX;
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
  *++yyvsp = yylval;
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
     `$$ = $1'.

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

/* Line 1806 of yacc.c  */
#line 523 "../SqlParser.ypp"
    {
    *parsedStatement = (yyvsp[(1) - (2)].statement_);
    YYACCEPT;
  }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 527 "../SqlParser.ypp"
    {
    *parsedStatement = (yyvsp[(1) - (2)].statement_);
    YYACCEPT;
  }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 531 "../SqlParser.ypp"
    {
    *parsedStatement = (yyvsp[(1) - (2)].command_);
    YYACCEPT;
  }
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 535 "../SqlParser.ypp"
    {
    *parsedStatement = (yyvsp[(1) - (2)].command_);
    YYACCEPT;
  }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 539 "../SqlParser.ypp"
    {
    YYABORT;
  }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 542 "../SqlParser.ypp"
    {
    // Regular yyparse() return codes are non-negative, so use a negative one here.
    return -1;
  }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 549 "../SqlParser.ypp"
    {
    (yyval.statement_) = (yyvsp[(1) - (1)].statement_);
  }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 552 "../SqlParser.ypp"
    {
    (yyval.statement_) = (yyvsp[(1) - (1)].copy_from_statement_);
  }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 555 "../SqlParser.ypp"
    {
    (yyval.statement_) = (yyvsp[(1) - (1)].create_table_statement_);
  }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 558 "../SqlParser.ypp"
    {
    (yyval.statement_) = (yyvsp[(1) - (1)].statement_);
  }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 561 "../SqlParser.ypp"
    {
    (yyval.statement_) = (yyvsp[(1) - (1)].delete_statement_);
  }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 564 "../SqlParser.ypp"
    {
    (yyval.statement_) = (yyvsp[(1) - (1)].drop_table_statement_);
  }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 567 "../SqlParser.ypp"
    {
    (yyval.statement_) = (yyvsp[(1) - (1)].insert_statement_);
  }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 570 "../SqlParser.ypp"
    {
    (yyval.statement_) = (yyvsp[(1) - (1)].quit_statement_);
  }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 573 "../SqlParser.ypp"
    {
    (yyval.statement_) = (yyvsp[(1) - (1)].select_statement_);
  }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 576 "../SqlParser.ypp"
    {
    (yyval.statement_) = (yyvsp[(1) - (1)].update_statement_);
  }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 582 "../SqlParser.ypp"
    {
    (yyval.quit_statement_) = new quickstep::ParseStatementQuit((yylsp[(1) - (1)]).first_line, (yylsp[(1) - (1)]).first_column);
  }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 588 "../SqlParser.ypp"
    {
    delete (yyvsp[(3) - (6)].string_value_);
    delete (yyvsp[(6) - (6)].attribute_definition_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[(1) - (6)]), yyscanner, "ALTER statements");
    YYERROR;
  }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 595 "../SqlParser.ypp"
    {
    delete (yyvsp[(3) - (6)].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[(1) - (6)]), yyscanner, "ALTER statements");
    YYERROR;
  }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 601 "../SqlParser.ypp"
    {
    delete (yyvsp[(3) - (6)].string_value_);
    delete (yyvsp[(6) - (6)].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[(1) - (6)]), yyscanner, "ALTER statements");
    YYERROR;
  }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 608 "../SqlParser.ypp"
    {
    delete (yyvsp[(3) - (6)].string_value_);
    delete (yyvsp[(6) - (6)].string_value_);
    (yyval.statement_) = nullptr;
    NotSupported(&(yylsp[(1) - (6)]), yyscanner, "ALTER statements");
    YYERROR;
  }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 617 "../SqlParser.ypp"
    {
    (yyval.create_table_statement_) = new quickstep::ParseStatementCreateTable((yylsp[(1) - (8)]).first_line, (yylsp[(1) - (8)]).first_column, (yyvsp[(3) - (8)].string_value_), (yyvsp[(5) - (8)].attribute_definition_list_), (yyvsp[(8) - (8)].block_properties_));
  }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 622 "../SqlParser.ypp"
    {
    if ((yyvsp[(9) - (9)].key_value_list_)) {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[(1) - (9)]).first_line, (yylsp[(1) - (9)]).first_column, (yyvsp[(3) - (9)].string_value_), (yyvsp[(5) - (9)].string_value_), (yyvsp[(6) - (9)].attribute_list_), (yyvsp[(8) - (9)].string_value_), (yylsp[(9) - (9)]).first_line, (yylsp[(9) - (9)]).first_column, (yyvsp[(9) - (9)].key_value_list_));
    } else {
      (yyval.statement_) = new quickstep::ParseStatementCreateIndex((yylsp[(1) - (9)]).first_line, (yylsp[(1) - (9)]).first_column, (yyvsp[(3) - (9)].string_value_), (yyvsp[(5) - (9)].string_value_), (yyvsp[(6) - (9)].attribute_list_), (yyvsp[(8) - (9)].string_value_));
    }
  }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 631 "../SqlParser.ypp"
    {
    (yyval.drop_table_statement_) = new quickstep::ParseStatementDropTable((yylsp[(1) - (3)]).first_line, (yylsp[(1) - (3)]).first_column, (yyvsp[(3) - (3)].string_value_));
  }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 636 "../SqlParser.ypp"
    {
    (yyval.attribute_definition_) = new quickstep::ParseAttributeDefinition((yylsp[(1) - (3)]).first_line, (yylsp[(1) - (3)]).first_column, (yyvsp[(1) - (3)].string_value_), (yyvsp[(2) - (3)].data_type_), (yyvsp[(3) - (3)].column_constraint_list_));
  }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 641 "../SqlParser.ypp"
    {
    (yyval.attribute_definition_list_) = new quickstep::PtrList<quickstep::ParseAttributeDefinition>();
    (yyval.attribute_definition_list_)->push_back((yyvsp[(1) - (1)].attribute_definition_));
  }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 645 "../SqlParser.ypp"
    {
    (yyval.attribute_definition_list_) = (yyvsp[(1) - (3)].attribute_definition_list_);
    (yyval.attribute_definition_list_)->push_back((yyvsp[(3) - (3)].attribute_definition_));
  }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 651 "../SqlParser.ypp"
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[(1) - (1)]), yyscanner, "BIT data type");
    YYERROR;
  }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 656 "../SqlParser.ypp"
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 659 "../SqlParser.ypp"
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 662 "../SqlParser.ypp"
    {
    (yyval.data_type_) = nullptr;
    NotSupported(&(yylsp[(1) - (1)]), yyscanner, "TIME data type");
    YYERROR;
  }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 667 "../SqlParser.ypp"
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetime));
  }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 670 "../SqlParser.ypp"
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 673 "../SqlParser.ypp"
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 676 "../SqlParser.ypp"
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDouble));
  }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 679 "../SqlParser.ypp"
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kFloat));
  }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 682 "../SqlParser.ypp"
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 685 "../SqlParser.ypp"
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kInt));
  }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 688 "../SqlParser.ypp"
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 691 "../SqlParser.ypp"
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kLong));
  }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 694 "../SqlParser.ypp"
    {
    /**
     * NOTE(chasseur): This pattern exhibits a shift/reduce conflict with the
     * TOKEN_INTERVAL case in 'literal_value'. Bison prefers to shift rather
     * than reduce, so the case in 'literal_value' has precedence over this.
     **/
    (yyval.data_type_) = nullptr;
    quickstep_yyerror(&(yylsp[(1) - (1)]), yyscanner, nullptr,
        "INTERVAL is ambiguous as a column type. Specify either DATETIME INTERVAL "
        "or YEARMONTH INTERVAL");
    YYERROR;
  }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 706 "../SqlParser.ypp"
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kDatetimeInterval));
  }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 709 "../SqlParser.ypp"
    {
    (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kYearMonthInterval));
  }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 712 "../SqlParser.ypp"
    {
    if ((yyvsp[(3) - (4)].numeric_literal_value_)->float_like()) {
      delete (yyvsp[(3) - (4)].numeric_literal_value_);
      (yyval.data_type_) = NULL;
      quickstep_yyerror(&(yylsp[(3) - (4)]), yyscanner, nullptr, "Non-integer length supplied for CHAR type");
      YYERROR;
    } else {
      if ((yyvsp[(3) - (4)].numeric_literal_value_)->long_value() <= 0) {
        delete (yyvsp[(3) - (4)].numeric_literal_value_);
        (yyval.data_type_) = NULL;
        quickstep_yyerror(&(yylsp[(3) - (4)]), yyscanner, nullptr, "Length for CHAR type must be at least 1");
        YYERROR;
      } else {
        (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kChar, (yyvsp[(3) - (4)].numeric_literal_value_)->long_value(), false));
        delete (yyvsp[(3) - (4)].numeric_literal_value_);
      }
    }
  }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 730 "../SqlParser.ypp"
    {
    if ((yyvsp[(3) - (4)].numeric_literal_value_)->float_like()) {
      delete (yyvsp[(3) - (4)].numeric_literal_value_);
      (yyval.data_type_) = NULL;
      quickstep_yyerror(&(yylsp[(3) - (4)]), yyscanner, nullptr, "Non-integer length supplied for VARCHAR type");
      YYERROR;
    } else {
      if ((yyvsp[(3) - (4)].numeric_literal_value_)->long_value() < 0) {
        delete (yyvsp[(3) - (4)].numeric_literal_value_);
        (yyval.data_type_) = NULL;
        quickstep_yyerror(&(yylsp[(3) - (4)]), yyscanner, nullptr, "Negative length supplied for VARCHAR type");
        YYERROR;
      } else {
        (yyval.data_type_) = new quickstep::ParseDataType(quickstep::TypeFactory::GetType(quickstep::kVarChar, (yyvsp[(3) - (4)].numeric_literal_value_)->long_value(), false));
        delete (yyvsp[(3) - (4)].numeric_literal_value_);
      }
    }
  }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 750 "../SqlParser.ypp"
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNull((yylsp[(1) - (1)]).first_line, (yylsp[(1) - (1)]).first_column);
  }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 753 "../SqlParser.ypp"
    {
    (yyval.column_constraint_) = new quickstep::ParseColumnConstraintNotNull((yylsp[(1) - (2)]).first_line, (yylsp[(1) - (2)]).first_column);
  }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 756 "../SqlParser.ypp"
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[(1) - (1)]), yyscanner, "Column Constraints (UNIQUE)");
    YYERROR;
  }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 761 "../SqlParser.ypp"
    {
    (yyval.column_constraint_) = nullptr;
    NotSupported(&(yylsp[(1) - (2)]), yyscanner, "Column Constraints (PRIMARY KEY)");
    YYERROR;
  }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 766 "../SqlParser.ypp"
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[(2) - (2)].literal_value_);
    NotSupported(&(yylsp[(1) - (2)]), yyscanner, "Column Constraints (DEFAULT)");
    YYERROR;
  }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 772 "../SqlParser.ypp"
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[(3) - (4)].predicate_);
    NotSupported(&(yylsp[(1) - (4)]), yyscanner, "Column Constraints (CHECK)");
    YYERROR;
  }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 778 "../SqlParser.ypp"
    {
    (yyval.column_constraint_) = nullptr;
    delete (yyvsp[(2) - (5)].string_value_);
    delete (yyvsp[(4) - (5)].string_value_);
    NotSupported(&(yylsp[(1) - (5)]), yyscanner, "Foreign Keys");
    YYERROR;
  }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 787 "../SqlParser.ypp"
    {
    (yyval.column_constraint_list_) = (yyvsp[(1) - (2)].column_constraint_list_);
    (yyval.column_constraint_list_)->push_back((yyvsp[(2) - (2)].column_constraint_));
  }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 791 "../SqlParser.ypp"
    {
    (yyval.column_constraint_list_) = new quickstep::PtrList<quickstep::ParseColumnConstraint>();
    (yyval.column_constraint_list_)->push_back((yyvsp[(1) - (1)].column_constraint_));
  }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 797 "../SqlParser.ypp"
    {
    (yyval.column_constraint_list_) = nullptr;
  }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 800 "../SqlParser.ypp"
    {
    (yyval.column_constraint_list_) = (yyvsp[(1) - (1)].column_constraint_list_);
  }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 805 "../SqlParser.ypp"
    {
    delete (yyvsp[(3) - (4)].string_list_);
    NotSupported(&(yylsp[(1) - (4)]), yyscanner, "Table Constraints (UNIQUE)");
    YYERROR;
  }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 810 "../SqlParser.ypp"
    {
    delete (yyvsp[(4) - (5)].string_list_);
    NotSupported(&(yylsp[(1) - (5)]), yyscanner, "Table Constraints (PRIMARY KEY)");
    YYERROR;
  }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 815 "../SqlParser.ypp"
    {
    delete (yyvsp[(4) - (10)].string_list_);
    delete (yyvsp[(7) - (10)].string_value_);
    delete (yyvsp[(9) - (10)].string_list_);
    NotSupported(&(yylsp[(1) - (10)]), yyscanner, "Table Constraints (FOREIGN KEY)");
    YYERROR;
  }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 822 "../SqlParser.ypp"
    {
    delete (yyvsp[(3) - (4)].predicate_);
    NotSupported(&(yylsp[(1) - (4)]), yyscanner, "Table Constraints (CHECK)");
    YYERROR;
  }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 829 "../SqlParser.ypp"
    {
    NotSupported(&(yylsp[(1) - (3)]), yyscanner, "Table Constraints");
    YYERROR;
  }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 833 "../SqlParser.ypp"
    {
    NotSupported(&(yylsp[(1) - (1)]), yyscanner, "Table Constraints");
    YYERROR;
  }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 839 "../SqlParser.ypp"
    {
    /* $$ = nullptr; */
  }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 842 "../SqlParser.ypp"
    {
    /* $$ = $1; */
  }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 847 "../SqlParser.ypp"
    {
    (yyval.attribute_list_) = nullptr;
  }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 850 "../SqlParser.ypp"
    {
    (yyval.attribute_list_) = (yyvsp[(2) - (3)].attribute_list_);
  }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 855 "../SqlParser.ypp"
    {
    (yyval.block_properties_) = nullptr;
  }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 858 "../SqlParser.ypp"
    {
    (yyval.block_properties_) = new quickstep::ParseBlockProperties((yylsp[(2) - (5)]).first_line, (yylsp[(2) - (5)]).first_column, (yyvsp[(4) - (5)].key_value_list_));
  }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 863 "../SqlParser.ypp"
    {
    (yyval.key_value_list_) = new quickstep::PtrList<quickstep::ParseKeyValue>();
    (yyval.key_value_list_)->push_back((yyvsp[(1) - (1)].key_value_));
  }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 867 "../SqlParser.ypp"
    {
    (yyval.key_value_list_) = (yyvsp[(1) - (3)].key_value_list_);
    (yyval.key_value_list_)->push_back((yyvsp[(3) - (3)].key_value_));
  }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 873 "../SqlParser.ypp"
    {
    (yyval.key_value_) = (yyvsp[(1) - (1)].key_string_value_);
  }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 876 "../SqlParser.ypp"
    {
    (yyval.key_value_) = (yyvsp[(1) - (1)].key_string_list_);
  }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 879 "../SqlParser.ypp"
    {
    (yyval.key_value_) = (yyvsp[(1) - (1)].key_integer_value_);
  }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 884 "../SqlParser.ypp"
    {
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[(1) - (2)]).first_line, (yylsp[(1) - (2)]).first_column, (yyvsp[(1) - (2)].string_value_), (yyvsp[(2) - (2)].string_value_));
  }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 887 "../SqlParser.ypp"
    {
    // This is a special case to handle the COMPRESS ALL option of the BLOCK PROPERTIES.
    (yyval.key_string_value_) = new quickstep::ParseKeyStringValue((yylsp[(1) - (2)]).first_line, (yylsp[(1) - (2)]).first_column, (yyvsp[(1) - (2)].string_value_),
        new quickstep::ParseString((yylsp[(2) - (2)]).first_line, (yylsp[(2) - (2)]).first_column, "ALL"));
  }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 894 "../SqlParser.ypp"
    {
    (yyval.key_string_list_) = new quickstep::ParseKeyStringList((yylsp[(1) - (4)]).first_line, (yylsp[(1) - (4)]).first_column, (yyvsp[(1) - (4)].string_value_), (yyvsp[(3) - (4)].string_list_));
  }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 899 "../SqlParser.ypp"
    {
    if ((yyvsp[(2) - (2)].numeric_literal_value_)->float_like()) {
      delete (yyvsp[(2) - (2)].numeric_literal_value_);
      (yyval.key_integer_value_) = nullptr;
      quickstep_yyerror(&(yylsp[(2) - (2)]), yyscanner, nullptr, "Value must be an integer");
      YYERROR;
    }
    (yyval.key_integer_value_) = new quickstep::ParseKeyIntegerValue((yylsp[(1) - (2)]).first_line, (yylsp[(1) - (2)]).first_column, (yyvsp[(1) - (2)].string_value_), (yyvsp[(2) - (2)].numeric_literal_value_));
  }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 910 "../SqlParser.ypp"
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[(1) - (1)]).first_line, (yylsp[(1) - (1)]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kBloomFilter));
  }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 914 "../SqlParser.ypp"
    {
    (yyval.string_value_) = new quickstep::ParseString((yylsp[(1) - (1)]).first_line, (yylsp[(1) - (1)]).first_column,
           std::to_string(quickstep::IndexSubBlockType::kCSBTree));
  }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 920 "../SqlParser.ypp"
    {
    (yyval.key_value_list_) = nullptr;
  }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 923 "../SqlParser.ypp"
    {
    (yyval.key_value_list_) = (yyvsp[(2) - (3)].key_value_list_);
  }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 929 "../SqlParser.ypp"
    {
    delete (yyvsp[(3) - (10)].string_value_);
    delete (yyvsp[(5) - (10)].string_list_);
    delete (yyvsp[(9) - (10)].literal_value_list_);
    (yyval.insert_statement_) = nullptr;
    NotSupported(&(yylsp[(4) - (10)]), yyscanner, "list of column names in INSERT statement");
    YYERROR;
  }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 937 "../SqlParser.ypp"
    {
    (yyval.insert_statement_) = new quickstep::ParseStatementInsert((yylsp[(1) - (7)]).first_line, (yylsp[(1) - (7)]).first_column, (yyvsp[(3) - (7)].string_value_), (yyvsp[(6) - (7)].literal_value_list_));
  }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 942 "../SqlParser.ypp"
    {
    (yyval.copy_from_statement_) = new quickstep::ParseStatementCopyFrom((yylsp[(1) - (5)]).first_line, (yylsp[(1) - (5)]).first_column, (yyvsp[(2) - (5)].string_value_), (yyvsp[(4) - (5)].string_value_), (yyvsp[(5) - (5)].copy_from_params_));
  }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 947 "../SqlParser.ypp"
    {
    (yyval.copy_from_params_) = nullptr;
  }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 950 "../SqlParser.ypp"
    {
    (yyval.copy_from_params_) = (yyvsp[(3) - (4)].copy_from_params_);
  }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 955 "../SqlParser.ypp"
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[(1) - (2)]).first_line, (yylsp[(1) - (2)]).first_column);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[(2) - (2)].string_value_));
  }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 959 "../SqlParser.ypp"
    {
    (yyval.copy_from_params_) = new quickstep::ParseCopyFromParams((yylsp[(1) - (2)]).first_line, (yylsp[(1) - (2)]).first_column);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[(2) - (2)].boolean_value_);
  }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 963 "../SqlParser.ypp"
    {
    (yyval.copy_from_params_) = (yyvsp[(1) - (4)].copy_from_params_);
    (yyval.copy_from_params_)->set_delimiter((yyvsp[(4) - (4)].string_value_));
  }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 967 "../SqlParser.ypp"
    {
    (yyval.copy_from_params_) = (yyvsp[(1) - (4)].copy_from_params_);
    (yyval.copy_from_params_)->escape_strings = (yyvsp[(4) - (4)].boolean_value_);
  }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 973 "../SqlParser.ypp"
    {
    (yyval.update_statement_) = new quickstep::ParseStatementUpdate((yylsp[(1) - (5)]).first_line, (yylsp[(1) - (5)]).first_column, (yyvsp[(2) - (5)].string_value_), (yyvsp[(4) - (5)].assignment_list_), (yyvsp[(5) - (5)].predicate_));
  }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 978 "../SqlParser.ypp"
    {
    (yyval.delete_statement_) = new quickstep::ParseStatementDelete((yylsp[(1) - (4)]).first_line, (yylsp[(1) - (4)]).first_column, (yyvsp[(3) - (4)].string_value_), (yyvsp[(4) - (4)].predicate_));
  }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 983 "../SqlParser.ypp"
    {
    (yyval.assignment_list_) = (yyvsp[(1) - (3)].assignment_list_);
    (yyval.assignment_list_)->push_back((yyvsp[(3) - (3)].assignment_));
  }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 987 "../SqlParser.ypp"
    {
    (yyval.assignment_list_) = new quickstep::PtrList<quickstep::ParseAssignment>();
    (yyval.assignment_list_)->push_back((yyvsp[(1) - (1)].assignment_));
  }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 993 "../SqlParser.ypp"
    {
    (yyval.assignment_) = new quickstep::ParseAssignment((yylsp[(1) - (3)]).first_line, (yylsp[(1) - (3)]).first_column, (yyvsp[(1) - (3)].string_value_), (yyvsp[(3) - (3)].expression_));
  }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 999 "../SqlParser.ypp"
    {
    (yyval.select_statement_) = new quickstep::ParseStatementSelect((yylsp[(1) - (2)]).first_line, (yylsp[(1) - (2)]).first_column, (yyvsp[(2) - (2)].select_query_), (yyvsp[(1) - (2)].with_list_));
  }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 1004 "../SqlParser.ypp"
    {
    (yyval.with_list_) = nullptr;
  }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 1007 "../SqlParser.ypp"
    {
    (yyval.with_list_) = (yyvsp[(2) - (2)].with_list_);
  }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 1012 "../SqlParser.ypp"
    {
    (yyval.with_list_) = new quickstep::PtrVector<quickstep::ParseSubqueryTableReference>();
    (yyval.with_list_)->push_back((yyvsp[(1) - (1)].with_list_element_));
  }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 1016 "../SqlParser.ypp"
    {
    (yyval.with_list_) = (yyvsp[(1) - (3)].with_list_);
    (yyval.with_list_)->push_back((yyvsp[(3) - (3)].with_list_element_));
  }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 1022 "../SqlParser.ypp"
    {
    (yyval.with_list_element_) = new quickstep::ParseSubqueryTableReference((yylsp[(1) - (3)]).first_line, (yylsp[(1) - (3)]).first_column, (yyvsp[(3) - (3)].subquery_expression_));
    (yyval.with_list_element_)->set_table_reference_signature((yyvsp[(1) - (3)].table_reference_signature_));
  }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 1029 "../SqlParser.ypp"
    {
    (yyval.select_query_) = new quickstep::ParseSelect((yylsp[(1) - (9)]).first_line, (yylsp[(1) - (9)]).first_column, (yyvsp[(3) - (9)].selection_), (yyvsp[(4) - (9)].table_reference_list_), (yyvsp[(5) - (9)].predicate_), (yyvsp[(6) - (9)].opt_group_by_clause_), (yyvsp[(7) - (9)].opt_having_clause_), (yyvsp[(8) - (9)].opt_order_by_clause_), (yyvsp[(9) - (9)].opt_limit_clause_));
  }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 1034 "../SqlParser.ypp"
    {
    /* $$ = nullptr; */
  }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 1037 "../SqlParser.ypp"
    {
    NotSupported(&(yylsp[(1) - (1)]), yyscanner, "ALL in selection");
    YYERROR;
  }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 1041 "../SqlParser.ypp"
    {
    NotSupported(&(yylsp[(1) - (1)]), yyscanner, "DISTINCT in selection");
    YYERROR;
  }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 1047 "../SqlParser.ypp"
    {
    (yyval.selection_) = new quickstep::ParseSelectionStar((yylsp[(1) - (1)]).first_line, (yylsp[(1) - (1)]).first_column);
  }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 1050 "../SqlParser.ypp"
    {
    (yyval.selection_) = (yyvsp[(1) - (1)].selection_list_);
  }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 1055 "../SqlParser.ypp"
    {
    (yyval.selection_list_) = new quickstep::ParseSelectionList((yylsp[(1) - (1)]).first_line, (yylsp[(1) - (1)]).first_column);
    (yyval.selection_list_)->add((yyvsp[(1) - (1)].selection_item_));
  }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 1059 "../SqlParser.ypp"
    {
    (yyval.selection_list_) = (yyvsp[(1) - (3)].selection_list_);
    (yyval.selection_list_)->add((yyvsp[(3) - (3)].selection_item_));
  }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 1065 "../SqlParser.ypp"
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[(1) - (3)]).first_line, (yylsp[(1) - (3)]).first_column, (yyvsp[(1) - (3)].expression_), (yyvsp[(3) - (3)].string_value_));
  }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 1068 "../SqlParser.ypp"
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[(1) - (2)]).first_line, (yylsp[(1) - (2)]).first_column, (yyvsp[(1) - (2)].expression_), (yyvsp[(2) - (2)].string_value_));
  }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 1071 "../SqlParser.ypp"
    {
    (yyval.selection_item_) = new quickstep::ParseSelectionItem((yylsp[(1) - (1)]).first_line, (yylsp[(1) - (1)]).first_column, (yyvsp[(1) - (1)].expression_));
  }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 1076 "../SqlParser.ypp"
    {
    (yyval.table_reference_list_) = (yyvsp[(2) - (3)].table_reference_list_);
  }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 1081 "../SqlParser.ypp"
    {
    /* $$ = nullptr; */
  }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 1084 "../SqlParser.ypp"
    {
    NotSupported(&(yylsp[(1) - (1)]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 1090 "../SqlParser.ypp"
    {
    NotSupported(&(yylsp[(1) - (2)]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 1094 "../SqlParser.ypp"
    {
    NotSupported(&(yylsp[(1) - (1)]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 1100 "../SqlParser.ypp"
    {
    delete (yyvsp[(3) - (5)].string_list_);
    delete (yyvsp[(5) - (5)].predicate_);
    NotSupported(&(yylsp[(1) - (5)]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 1106 "../SqlParser.ypp"
    {
    delete (yyvsp[(2) - (4)].string_list_);
    delete (yyvsp[(4) - (4)].predicate_);
    NotSupported(&(yylsp[(1) - (4)]), yyscanner, "alternate JOIN syntax (specify in WHERE clause instead)");
    YYERROR;
  }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 1112 "../SqlParser.ypp"
    {
    delete (yyvsp[(4) - (6)].string_list_);
    delete (yyvsp[(6) - (6)].predicate_);
    NotSupported(&(yylsp[(1) - (6)]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 1118 "../SqlParser.ypp"
    {
    delete (yyvsp[(3) - (5)].string_list_);
    delete (yyvsp[(5) - (5)].predicate_);
    NotSupported(&(yylsp[(1) - (5)]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 1124 "../SqlParser.ypp"
    {
    delete (yyvsp[(4) - (6)].string_list_);
    delete (yyvsp[(6) - (6)].predicate_);
    NotSupported(&(yylsp[(1) - (6)]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 1130 "../SqlParser.ypp"
    {
    delete (yyvsp[(3) - (5)].string_list_);
    delete (yyvsp[(5) - (5)].predicate_);
    NotSupported(&(yylsp[(1) - (5)]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 1136 "../SqlParser.ypp"
    {
    delete (yyvsp[(4) - (6)].string_list_);
    delete (yyvsp[(6) - (6)].predicate_);
    NotSupported(&(yylsp[(1) - (6)]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 1142 "../SqlParser.ypp"
    {
    delete (yyvsp[(3) - (5)].string_list_);
    delete (yyvsp[(5) - (5)].predicate_);
    NotSupported(&(yylsp[(1) - (5)]), yyscanner, "OUTER JOIN");
    YYERROR;
  }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 1150 "../SqlParser.ypp"
    {
    (yyval.subquery_expression_) = new quickstep::ParseSubqueryExpression((yylsp[(1) - (3)]).first_line, (yylsp[(1) - (3)]).first_column, (yyvsp[(2) - (3)].select_query_));
  }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 1155 "../SqlParser.ypp"
    {
    (yyval.opt_sample_clause_) = NULL;
  }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 1158 "../SqlParser.ypp"
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[(1) - (3)]).first_line, (yylsp[(1) - (3)]).first_column, true, (yyvsp[(2) - (3)].numeric_literal_value_));
  }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 1161 "../SqlParser.ypp"
    {
    (yyval.opt_sample_clause_) = new quickstep::ParseSample((yylsp[(1) - (3)]).first_line, (yylsp[(1) - (3)]).first_column, false, (yyvsp[(2) - (3)].numeric_literal_value_));
  }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 1166 "../SqlParser.ypp"
    {
    (yyval.table_reference_) = new quickstep::ParseSubqueryTableReference((yylsp[(1) - (2)]).first_line, (yylsp[(1) - (2)]).first_column, (yyvsp[(1) - (2)].subquery_expression_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[(2) - (2)].table_reference_signature_));
  }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 1170 "../SqlParser.ypp"
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[(1) - (3)]).first_line, (yylsp[(1) - (3)]).first_column, (yyvsp[(1) - (3)].string_value_), (yyvsp[(2) - (3)].opt_sample_clause_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[(3) - (3)].table_reference_signature_));
  }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 1174 "../SqlParser.ypp"
    {
    (yyval.table_reference_) = new quickstep::ParseSimpleTableReference((yylsp[(1) - (2)]).first_line, (yylsp[(1) - (2)]).first_column, (yyvsp[(1) - (2)].string_value_), (yyvsp[(2) - (2)].opt_sample_clause_));
  }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 1177 "../SqlParser.ypp"
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[(1) - (2)]).first_line, (yylsp[(1) - (2)]).first_column, (yyvsp[(1) - (2)].function_call_));
    (yyval.table_reference_)->set_table_reference_signature((yyvsp[(2) - (2)].table_reference_signature_));
  }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 1181 "../SqlParser.ypp"
    {
    (yyval.table_reference_) = new quickstep::ParseGeneratorTableReference((yylsp[(1) - (1)]).first_line, (yylsp[(1) - (1)]).first_column, (yyvsp[(1) - (1)].function_call_));
  }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 1186 "../SqlParser.ypp"
    {
    (yyval.table_reference_signature_) = (yyvsp[(1) - (1)].table_reference_signature_);
  }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 1189 "../SqlParser.ypp"
    {
    (yyval.table_reference_signature_) = (yyvsp[(2) - (2)].table_reference_signature_);
  }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 1194 "../SqlParser.ypp"
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[(1) - (1)]).first_line, (yylsp[(1) - (1)]).first_column, (yyvsp[(1) - (1)].string_value_));
  }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 1197 "../SqlParser.ypp"
    {
    (yyval.table_reference_signature_) = new ::quickstep::ParseTableReferenceSignature((yylsp[(1) - (4)]).first_line, (yylsp[(1) - (4)]).first_column, (yyvsp[(1) - (4)].string_value_), (yyvsp[(3) - (4)].string_list_));
  }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 1202 "../SqlParser.ypp"
    {
    (yyval.table_reference_list_) = new quickstep::PtrList<quickstep::ParseTableReference>();
    (yyval.table_reference_list_)->push_back((yyvsp[(1) - (1)].table_reference_));
  }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 1206 "../SqlParser.ypp"
    {
    (yyval.table_reference_list_) = (yyvsp[(1) - (3)].table_reference_list_);
    (yyval.table_reference_list_)->push_back((yyvsp[(3) - (3)].table_reference_));
  }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 1212 "../SqlParser.ypp"
    {
    (yyval.opt_group_by_clause_) = nullptr;
  }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 1215 "../SqlParser.ypp"
    {
    (yyval.opt_group_by_clause_) = new quickstep::ParseGroupBy((yylsp[(1) - (3)]).first_line, (yylsp[(1) - (3)]).first_column, (yyvsp[(3) - (3)].expression_list_));
  }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 1220 "../SqlParser.ypp"
    {
    (yyval.opt_having_clause_) = nullptr;
  }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 1223 "../SqlParser.ypp"
    {
    (yyval.opt_having_clause_) = new quickstep::ParseHaving((yylsp[(1) - (2)]).first_line, (yylsp[(1) - (2)]).first_column, (yyvsp[(2) - (2)].predicate_));
  }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 1228 "../SqlParser.ypp"
    {
    (yyval.opt_order_by_clause_) = nullptr;
  }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 1231 "../SqlParser.ypp"
    {
    (yyval.opt_order_by_clause_) = new quickstep::ParseOrderBy((yylsp[(1) - (3)]).first_line, (yylsp[(1) - (3)]).first_column, (yyvsp[(3) - (3)].order_commalist_));
  }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 1236 "../SqlParser.ypp"
    {
    (yyval.opt_limit_clause_) = nullptr;
  }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 1239 "../SqlParser.ypp"
    {
    if ((yyvsp[(2) - (2)].numeric_literal_value_)->float_like()) {
      delete (yyvsp[(2) - (2)].numeric_literal_value_);
      (yyval.opt_limit_clause_) = nullptr;
      quickstep_yyerror(&(yylsp[(2) - (2)]), yyscanner, nullptr, "LIMIT value must be an integer");
      YYERROR;
    } else {
      if ((yyvsp[(2) - (2)].numeric_literal_value_)->long_value() <= 0) {
        delete (yyvsp[(2) - (2)].numeric_literal_value_);
        (yyval.opt_limit_clause_) = nullptr;
        quickstep_yyerror(&(yylsp[(2) - (2)]), yyscanner, nullptr, "LIMIT value must be positive");
        YYERROR;
      } else {
        (yyval.opt_limit_clause_) = new quickstep::ParseLimit((yylsp[(1) - (2)]).first_line, (yylsp[(1) - (2)]).first_column, (yyvsp[(2) - (2)].numeric_literal_value_));
      }
    }
  }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 1258 "../SqlParser.ypp"
    {
    (yyval.order_commalist_) = new quickstep::PtrList<quickstep::ParseOrderByItem>();
    (yyval.order_commalist_)->push_back((yyvsp[(1) - (1)].order_item_));
  }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 1262 "../SqlParser.ypp"
    {
    (yyval.order_commalist_) = (yyvsp[(1) - (3)].order_commalist_);
    (yyval.order_commalist_)->push_back((yyvsp[(3) - (3)].order_item_));
  }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 1268 "../SqlParser.ypp"
    {
    (yyval.order_item_) = new quickstep::ParseOrderByItem((yylsp[(1) - (3)]).first_line, (yylsp[(1) - (3)]).first_column, (yyvsp[(1) - (3)].expression_), (yyvsp[(2) - (3)].order_direction_), (yyvsp[(3) - (3)].order_direction_));
    delete (yyvsp[(2) - (3)].order_direction_);
    delete (yyvsp[(3) - (3)].order_direction_);
  }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 1275 "../SqlParser.ypp"
    {
    (yyval.order_direction_) = nullptr;
  }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 1278 "../SqlParser.ypp"
    {
    (yyval.order_direction_) = new bool(true);
  }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 1281 "../SqlParser.ypp"
    {
    (yyval.order_direction_) = new bool(false);
  }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 1286 "../SqlParser.ypp"
    {
    (yyval.order_direction_) = nullptr;
  }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 1289 "../SqlParser.ypp"
    {
    (yyval.order_direction_) = new bool(true);
  }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 1292 "../SqlParser.ypp"
    {
    (yyval.order_direction_) = new bool(false);
  }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 1298 "../SqlParser.ypp"
    {
    (yyval.predicate_) = nullptr;
  }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 1301 "../SqlParser.ypp"
    {
    (yyval.predicate_) = (yyvsp[(1) - (1)].predicate_);
  }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 1306 "../SqlParser.ypp"
    {
    (yyval.predicate_) = (yyvsp[(2) - (2)].predicate_);
  }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 1311 "../SqlParser.ypp"
    {
    if ((yyvsp[(1) - (3)].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kDisjunction) {
      (yyval.predicate_) = (yyvsp[(1) - (3)].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateDisjunction((yylsp[(1) - (3)]).first_line, (yylsp[(1) - (3)]).first_column);
      static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[(1) - (3)].predicate_));
    }
    static_cast<quickstep::ParsePredicateDisjunction *>((yyval.predicate_))->addPredicate((yyvsp[(3) - (3)].predicate_));
  }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 1320 "../SqlParser.ypp"
    {
    (yyval.predicate_) = (yyvsp[(1) - (1)].predicate_);
  }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 1325 "../SqlParser.ypp"
    {
    if ((yyvsp[(1) - (3)].predicate_)->getParsePredicateType() == quickstep::ParsePredicate::kConjunction) {
      (yyval.predicate_) = (yyvsp[(1) - (3)].predicate_);
    } else {
      (yyval.predicate_) = new quickstep::ParsePredicateConjunction((yylsp[(1) - (3)]).first_line, (yylsp[(1) - (3)]).first_column);
      static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[(1) - (3)].predicate_));
    }
    static_cast<quickstep::ParsePredicateConjunction *>((yyval.predicate_))->addPredicate((yyvsp[(3) - (3)].predicate_));
  }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 1334 "../SqlParser.ypp"
    {
    (yyval.predicate_) = (yyvsp[(1) - (1)].predicate_);
  }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 1339 "../SqlParser.ypp"
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation((yylsp[(1) - (2)]).first_line, (yylsp[(1) - (2)]).first_column, (yyvsp[(2) - (2)].predicate_));
  }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 1342 "../SqlParser.ypp"
    {
    (yyval.predicate_) = (yyvsp[(1) - (1)].predicate_);
  }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 1347 "../SqlParser.ypp"
    {
    (yyval.predicate_) = new quickstep::ParsePredicateBetween((yylsp[(2) - (5)]).first_line, (yylsp[(2) - (5)]).first_column, (yyvsp[(1) - (5)].expression_), (yyvsp[(3) - (5)].expression_), (yyvsp[(5) - (5)].expression_));
  }
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 1350 "../SqlParser.ypp"
    {
    (yyval.predicate_) = new quickstep::ParsePredicateNegation(
        (yylsp[(2) - (6)]).first_line, (yylsp[(2) - (6)]).first_column,
        new quickstep::ParsePredicateBetween((yylsp[(3) - (6)]).first_line, (yylsp[(3) - (6)]).first_column, (yyvsp[(1) - (6)].expression_), (yyvsp[(4) - (6)].expression_), (yyvsp[(6) - (6)].expression_)));
  }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 1355 "../SqlParser.ypp"
    {
    delete (yyvsp[(1) - (4)].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[(2) - (4)]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 1361 "../SqlParser.ypp"
    {
    delete (yyvsp[(1) - (3)].attribute_);
    (yyval.predicate_) = nullptr;
    NotSupported(&(yylsp[(2) - (3)]), yyscanner, "NULL comparison predicates");
    YYERROR;
  }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 1367 "../SqlParser.ypp"
    {
    (yyval.predicate_) = new quickstep::ParsePredicateComparison((yylsp[(2) - (3)]).first_line, (yylsp[(2) - (3)]).first_column, *(yyvsp[(2) - (3)].comparison_), (yyvsp[(1) - (3)].expression_), (yyvsp[(3) - (3)].expression_));
  }
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 1370 "../SqlParser.ypp"
    {
    (yyval.predicate_) = (yyvsp[(2) - (3)].predicate_);
  }
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 1376 "../SqlParser.ypp"
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[(2) - (3)]).first_line, (yylsp[(2) - (3)]).first_column, *(yyvsp[(2) - (3)].binary_operation_), (yyvsp[(1) - (3)].expression_), (yyvsp[(3) - (3)].expression_));
  }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 1379 "../SqlParser.ypp"
    {
    (yyval.expression_) = (yyvsp[(1) - (1)].expression_);
  }
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 1384 "../SqlParser.ypp"
    {
    (yyval.expression_) = new quickstep::ParseBinaryExpression((yylsp[(2) - (3)]).first_line, (yylsp[(2) - (3)]).first_column, *(yyvsp[(2) - (3)].binary_operation_), (yyvsp[(1) - (3)].expression_), (yyvsp[(3) - (3)].expression_));
  }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 1387 "../SqlParser.ypp"
    {
    (yyval.expression_) = (yyvsp[(1) - (1)].expression_);
  }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 1392 "../SqlParser.ypp"
    {
    (yyval.expression_) = new quickstep::ParseUnaryExpression((yylsp[(1) - (2)]).first_line, (yylsp[(1) - (2)]).first_column, *(yyvsp[(1) - (2)].unary_operation_), (yyvsp[(2) - (2)].expression_));
  }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 1395 "../SqlParser.ypp"
    {
    (yyval.expression_) = (yyvsp[(1) - (1)].expression_);
  }
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 1400 "../SqlParser.ypp"
    {
    (yyval.expression_) = (yyvsp[(1) - (1)].attribute_);
  }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 1403 "../SqlParser.ypp"
    {
    (yyval.expression_) = new quickstep::ParseScalarLiteral((yyvsp[(1) - (1)].literal_value_));
  }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 1406 "../SqlParser.ypp"
    {
    (yyval.expression_) = (yyvsp[(1) - (1)].function_call_);
  }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 1409 "../SqlParser.ypp"
    {
    (yyval.expression_) = (yyvsp[(2) - (3)].expression_);
  }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 1414 "../SqlParser.ypp"
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[(1) - (3)]).first_line, (yylsp[(1) - (3)]).first_column, (yyvsp[(1) - (3)].string_value_), new quickstep::PtrList<quickstep::ParseExpression>());
  }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 1418 "../SqlParser.ypp"
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall(
        (yylsp[(1) - (4)]).first_line, (yylsp[(1) - (4)]).first_column, (yyvsp[(1) - (4)].string_value_), new quickstep::ParseStar((yylsp[(3) - (4)]).first_line, (yylsp[(3) - (4)]).first_column));
  }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 1422 "../SqlParser.ypp"
    {
    (yyval.function_call_) = new quickstep::ParseFunctionCall((yylsp[(1) - (4)]).first_line, (yylsp[(1) - (4)]).first_column, (yyvsp[(1) - (4)].string_value_), (yyvsp[(3) - (4)].expression_list_));
  }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 1427 "../SqlParser.ypp"
    {
    (yyval.expression_list_) = new quickstep::PtrList<quickstep::ParseExpression>();
    (yyval.expression_list_)->push_back((yyvsp[(1) - (1)].expression_));
  }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 1431 "../SqlParser.ypp"
    {
    (yyval.expression_list_) = (yyvsp[(1) - (3)].expression_list_);
    (yyval.expression_list_)->push_back((yyvsp[(3) - (3)].expression_));
  }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 1437 "../SqlParser.ypp"
    {
    (yyval.literal_value_) = new quickstep::NullParseLiteralValue((yylsp[(1) - (1)]).first_line, (yylsp[(1) - (1)]).first_column);
  }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 1440 "../SqlParser.ypp"
    {
    (yyval.literal_value_) = (yyvsp[(1) - (1)].numeric_literal_value_);
  }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 1443 "../SqlParser.ypp"
    {
    (yyval.literal_value_) = (yyvsp[(2) - (2)].numeric_literal_value_);
  }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 1446 "../SqlParser.ypp"
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * minus character as a 'unary_operation' followed by a numeric literal.
     * Because Bison prefers to shift rather than reduce, this case has
     * precedence (i.e. the parser will prefer to interpret the ambiguous
     * pattern as a negative number literal rather than a unary minus operation
     * applied to a non-negative number literal).
     **/
    (yyvsp[(2) - (2)].numeric_literal_value_)->prependMinus();
    (yyval.literal_value_) = (yyvsp[(2) - (2)].numeric_literal_value_);
  }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 1458 "../SqlParser.ypp"
    {
    (yyval.literal_value_) = new quickstep::StringParseLiteralValue((yyvsp[(1) - (1)].string_value_),
                                                nullptr);  // No explicit type.
  }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 1462 "../SqlParser.ypp"
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
    if (quickstep::StringParseLiteralValue::ParseAmbiguousInterval((yyvsp[(2) - (2)].string_value_), &parse_value)) {
      (yyval.literal_value_) = parse_value;
    } else {
      (yyval.literal_value_) = nullptr;
      quickstep_yyerror(&(yylsp[(2) - (2)]), yyscanner, nullptr, "Failed to parse literal as specified type");
      YYERROR;
    }
  }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 1481 "../SqlParser.ypp"
    {
    quickstep::StringParseLiteralValue *parse_value
        = new quickstep::StringParseLiteralValue((yyvsp[(2) - (2)].string_value_), &((yyvsp[(1) - (2)].data_type_)->getType()));
    delete (yyvsp[(1) - (2)].data_type_);
    if (!parse_value->tryExplicitTypeParse()) {
      delete parse_value;
      (yyval.literal_value_) = nullptr;
      quickstep_yyerror(&(yylsp[(2) - (2)]), yyscanner, nullptr, "Failed to parse literal as specified type");
      YYERROR;
    } else {
      (yyval.literal_value_) = parse_value;
    }
  }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 1496 "../SqlParser.ypp"
    {
    (yyval.literal_value_list_) = new quickstep::PtrList<quickstep::ParseScalarLiteral>();
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[(1) - (1)].literal_value_)));
  }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 1500 "../SqlParser.ypp"
    {
    (yyval.literal_value_list_) = (yyvsp[(1) - (3)].literal_value_list_);
    (yyval.literal_value_list_)->push_back(new quickstep::ParseScalarLiteral((yyvsp[(3) - (3)].literal_value_)));
  }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 1506 "../SqlParser.ypp"
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[(1) - (1)]).first_line, (yylsp[(1) - (1)]).first_column, (yyvsp[(1) - (1)].string_value_));
  }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 1509 "../SqlParser.ypp"
    {
    (yyval.attribute_) = new quickstep::ParseAttribute((yylsp[(1) - (3)]).first_line, (yylsp[(1) - (3)]).first_column, (yyvsp[(3) - (3)].string_value_), (yyvsp[(1) - (3)].string_value_));
  }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 1514 "../SqlParser.ypp"
    {
    (yyval.attribute_list_) = new quickstep::PtrList<quickstep::ParseAttribute>();
    (yyval.attribute_list_)->push_back((yyvsp[(1) - (1)].attribute_));
  }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 1518 "../SqlParser.ypp"
    {
    (yyval.attribute_list_) = (yyvsp[(1) - (3)].attribute_list_);
    (yyval.attribute_list_)->push_back((yyvsp[(3) - (3)].attribute_));
  }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 1525 "../SqlParser.ypp"
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual);
  }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 1528 "../SqlParser.ypp"
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotEqual);
  }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 1531 "../SqlParser.ypp"
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLess);
  }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 1534 "../SqlParser.ypp"
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLessOrEqual);
  }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 1537 "../SqlParser.ypp"
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreater);
  }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 1540 "../SqlParser.ypp"
    {
    (yyval.comparison_) = &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kGreaterOrEqual);
  }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 1543 "../SqlParser.ypp"
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kLike);
  }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 1546 "../SqlParser.ypp"
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotLike);
  }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 1549 "../SqlParser.ypp"
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kRegexMatch);
  }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 1552 "../SqlParser.ypp"
    {
    (yyval.comparison_) =  &quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kNotRegexMatch);
  }
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 1557 "../SqlParser.ypp"
    {
    /**
     * NOTE(chasseur): This case exhibits a shift/reduce conflict with the
     * '-' TOKEN_UNSIGNED_NUMVAL case in 'literal_value'. Because Bison prefers
     * to shift rather than reduce, the case in 'literal_value' has precedence
     * over this one.
     **/
    (yyval.unary_operation_) = &quickstep::UnaryOperationFactory::GetUnaryOperation(quickstep::UnaryOperationID::kNegate);
  }
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 1568 "../SqlParser.ypp"
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kAdd);
  }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 1571 "../SqlParser.ypp"
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kSubtract);
  }
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 1576 "../SqlParser.ypp"
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kModulo);
  }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 1579 "../SqlParser.ypp"
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kMultiply);
  }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 1582 "../SqlParser.ypp"
    {
    (yyval.binary_operation_) = &quickstep::BinaryOperationFactory::GetBinaryOperation(quickstep::BinaryOperationID::kDivide);
  }
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 1588 "../SqlParser.ypp"
    {
    (yyval.string_list_) = new quickstep::PtrList<quickstep::ParseString>();
    (yyval.string_list_)->push_back((yyvsp[(1) - (1)].string_value_));
  }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 1592 "../SqlParser.ypp"
    {
    (yyval.string_list_) = (yyvsp[(1) - (3)].string_list_);
    (yyval.string_list_)->push_back((yyvsp[(3) - (3)].string_value_));
  }
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 1598 "../SqlParser.ypp"
    {
    (yyval.string_value_) = (yyvsp[(1) - (1)].string_value_);
  }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 1601 "../SqlParser.ypp"
    {
    if ((yyvsp[(1) - (1)].string_value_)->value().empty()) {
      quickstep_yyerror(&(yylsp[(1) - (1)]), yyscanner, nullptr, "Zero-length identifier");
    }
    (yyval.string_value_) = (yyvsp[(1) - (1)].string_value_);
  }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 1609 "../SqlParser.ypp"
    {
    (yyval.boolean_value_) = true;
  }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 1612 "../SqlParser.ypp"
    {
    (yyval.boolean_value_) = true;
  }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 1615 "../SqlParser.ypp"
    {
    (yyval.boolean_value_) = false;
  }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 1618 "../SqlParser.ypp"
    {
    (yyval.boolean_value_) = false;
  }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 1624 "../SqlParser.ypp"
    {
    (yyval.command_) = new quickstep::ParseCommand((yylsp[(1) - (2)]).first_line, (yylsp[(1) - (2)]).first_column, (yyvsp[(1) - (2)].string_value_), (yyvsp[(2) - (2)].command_argument_list_));
  }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 1629 "../SqlParser.ypp"
    {
    quickstep::PtrVector<quickstep::ParseString> *argument_list = (yyvsp[(1) - (2)].command_argument_list_);
    argument_list->push_back((yyvsp[(2) - (2)].string_value_));
    (yyval.command_argument_list_) = argument_list;
  }
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 1634 "../SqlParser.ypp"
    { /* Epsilon, an empy match. */
    (yyval.command_argument_list_) = new quickstep::PtrVector<quickstep::ParseString>();
  }
    break;



/* Line 1806 of yacc.c  */
#line 5584 "SqlParser_gen.cpp"
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

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
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
  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

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

  *++yyvsp = yylval;

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

#if !defined(yyoverflow) || YYERROR_VERBOSE
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
  /* Do not reclaim the symbols of the rule which action triggered
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 1638 "../SqlParser.ypp"


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

