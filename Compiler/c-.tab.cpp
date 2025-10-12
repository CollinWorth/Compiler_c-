/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ID = 258,
     NUMCONST = 259,
     CHARCONST = 260,
     STRINGCONST = 261,
     ERROR = 262,
     BOOLCONST = 263,
     BOOL = 264,
     CHAR = 265,
     INT = 266,
     VOID = 267,
     STATIC = 268,
     IF = 269,
     THEN = 270,
     ELSE = 271,
     WHILE = 272,
     DO = 273,
     FOR = 274,
     RETURN = 275,
     BREAK = 276,
     OR = 277,
     AND = 278,
     NOT = 279,
     ASSIGN = 280,
     ADDASS = 281,
     SUBASS = 282,
     MULASS = 283,
     DIVASS = 284,
     EQ = 285,
     NE = 286,
     LT = 287,
     LE = 288,
     GT = 289,
     GE = 290,
     NEQ = 291,
     INC = 292,
     DEC = 293,
     TO = 294,
     BY = 295
   };
#endif
/* Tokens.  */
#define ID 258
#define NUMCONST 259
#define CHARCONST 260
#define STRINGCONST 261
#define ERROR 262
#define BOOLCONST 263
#define BOOL 264
#define CHAR 265
#define INT 266
#define VOID 267
#define STATIC 268
#define IF 269
#define THEN 270
#define ELSE 271
#define WHILE 272
#define DO 273
#define FOR 274
#define RETURN 275
#define BREAK 276
#define OR 277
#define AND 278
#define NOT 279
#define ASSIGN 280
#define ADDASS 281
#define SUBASS 282
#define MULASS 283
#define DIVASS 284
#define EQ 285
#define NE 286
#define LT 287
#define LE 288
#define GT 289
#define GE 290
#define NEQ 291
#define INC 292
#define DEC 293
#define TO 294
#define BY 295




/* Copy the first part of user declarations.  */
#line 1 "c-.y"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "globals.h"
#include <string.h>

void freeTokenData(struct TokenData* tok) {
    if (!tok) return;

    if (tok->tokenstr) {
        delete[] tok->tokenstr;
        tok->tokenstr = NULL;
    }

    // This assumes stringValue points to memory managed by tokenstr or another mechanism
    // If stringValue is separately allocated, it needs its own delete[].
    // Based on c-.l, it is separately allocated.
    // if (tok->stringValue) { delete[] tok->stringValue; }
    delete tok; // finally free the struct itself
}

char* formatNumValue(double x) {
    static char buf[64];  // static buffer for printing
    if (floor(x) == x) {
        snprintf(buf, sizeof(buf), "%d", (int)x);  // integer
    } else {
        snprintf(buf, sizeof(buf), "%g", x);      // float, drops trailing zeros
    }
    return buf;
}

void printToken(TokenData* t, const char* name) {
    if (!t) return;
    printf("[Parser] Token %s at line %d, addr=%p\n", name, t->linenum, (void*)t);
}

extern int yylineno;
int yylex(void);
int yyerror(const char *s);


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
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

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 43 "c-.y"
{
    TreeNode *tree;
    TokenData *tokenData;
    int op;
    ExpType type;   // Add this field for ExpType enum
}
/* Line 193 of yacc.c.  */
#line 225 "c-.tab.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 238 "c-.tab.cpp"

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
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  12
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   232

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  104
/* YYNRULES -- Number of states.  */
#define YYNSTATES  163

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   296

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    51,     2,     2,
      43,    44,    49,    47,    54,    48,     2,    50,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    55,    53,
       2,     2,     2,    56,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    45,     2,    46,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    41,     2,    42,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    52
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    18,    23,
      27,    31,    33,    35,    39,    41,    46,    48,    50,    52,
      59,    65,    67,    68,    72,    74,    77,    81,    83,    85,
      89,    91,    93,    95,    97,    99,   101,   104,   106,   111,
     114,   115,   118,   119,   124,   131,   136,   143,   147,   153,
     156,   160,   163,   167,   170,   173,   175,   177,   179,   181,
     183,   185,   189,   191,   195,   197,   200,   202,   206,   208,
     210,   212,   214,   216,   218,   220,   224,   226,   228,   230,
     234,   236,   238,   240,   242,   245,   247,   249,   251,   253,
     255,   257,   259,   264,   268,   270,   272,   277,   279,   280,
     284,   286,   288,   290,   292
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      58,     0,    -1,    59,    -1,    59,    60,    -1,    60,    -1,
      61,    -1,    67,    -1,    66,    63,    53,    -1,    13,    66,
      63,    53,    -1,    66,    63,    53,    -1,    63,    54,    64,
      -1,    64,    -1,    65,    -1,    65,    55,    85,    -1,     3,
      -1,     3,    45,     4,    46,    -1,     9,    -1,    10,    -1,
      11,    -1,    66,     3,    43,    68,    44,    75,    -1,     3,
      43,    68,    44,    75,    -1,    69,    -1,    -1,    69,    53,
      70,    -1,    70,    -1,    66,    71,    -1,    71,    54,    72,
      -1,    72,    -1,     3,    -1,     3,    45,    46,    -1,    74,
      -1,    75,    -1,    78,    -1,    79,    -1,    81,    -1,    82,
      -1,    83,    53,    -1,    53,    -1,    41,    76,    77,    42,
      -1,    76,    62,    -1,    -1,    77,    73,    -1,    -1,    14,
      85,    15,    73,    -1,    14,    85,    15,    73,    16,    73,
      -1,    17,    85,    18,    73,    -1,    19,     3,    25,    80,
      18,    73,    -1,    85,    39,    85,    -1,    85,    39,    85,
      40,    85,    -1,    20,    53,    -1,    20,    83,    53,    -1,
      21,    53,    -1,    97,    84,    83,    -1,    97,    37,    -1,
      97,    38,    -1,    85,    -1,    25,    -1,    26,    -1,    27,
      -1,    28,    -1,    29,    -1,    85,    22,    86,    -1,    86,
      -1,    86,    23,    87,    -1,    87,    -1,    24,    87,    -1,
      88,    -1,    90,    89,    90,    -1,    90,    -1,    32,    -1,
      33,    -1,    34,    -1,    35,    -1,    30,    -1,    36,    -1,
      90,    91,    92,    -1,    92,    -1,    47,    -1,    48,    -1,
      92,    93,    94,    -1,    94,    -1,    49,    -1,    50,    -1,
      51,    -1,    95,    94,    -1,    96,    -1,    48,    -1,    49,
      -1,    56,    -1,    97,    -1,    98,    -1,     3,    -1,     3,
      45,    83,    46,    -1,    43,    83,    44,    -1,    99,    -1,
     102,    -1,     3,    43,   100,    44,    -1,   101,    -1,    -1,
     101,    54,    83,    -1,    83,    -1,     4,    -1,     5,    -1,
       6,    -1,     8,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    76,    76,    83,    87,    93,    97,   103,   140,   164,
     190,   194,   200,   204,   213,   220,   232,   233,   234,   237,
     245,   255,   260,   265,   269,   275,   295,   299,   305,   311,
     320,   324,   328,   332,   336,   340,   346,   352,   358,   364,
     370,   375,   382,   388,   403,   415,   419,   429,   433,   439,
     443,   449,   455,   460,   465,   470,   476,   477,   478,   479,
     480,   483,   488,   494,   499,   505,   510,   516,   521,   527,
     528,   529,   530,   531,   532,   535,   540,   546,   547,   550,
     555,   561,   562,   563,   566,   572,   578,   579,   580,   583,
     587,   593,   599,   610,   614,   618,   624,   632,   637,   642,
     646,   652,   658,   664,   672
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "NUMCONST", "CHARCONST",
  "STRINGCONST", "ERROR", "BOOLCONST", "BOOL", "CHAR", "INT", "VOID",
  "STATIC", "IF", "THEN", "ELSE", "WHILE", "DO", "FOR", "RETURN", "BREAK",
  "OR", "AND", "NOT", "ASSIGN", "ADDASS", "SUBASS", "MULASS", "DIVASS",
  "EQ", "NE", "LT", "LE", "GT", "GE", "NEQ", "INC", "DEC", "TO", "BY",
  "'{'", "'}'", "'('", "')'", "'['", "']'", "'+'", "'-'", "'*'", "'/'",
  "'%'", "\"then\"", "';'", "','", "':'", "'?'", "$accept", "program",
  "declList", "decl", "varDecl", "scopedVarDecl", "varDeclList",
  "varDeclInit", "varDeclId", "typeSpec", "funDecl", "parms", "parmList",
  "parmTypeList", "parmIdList", "parmId", "stmt", "expStmt",
  "compoundStmt", "localDecls", "stmtList", "selectStmt", "iterStmt",
  "iterRange", "returnStmt", "breakStmt", "exp", "assignop", "simpleExp",
  "andExp", "unaryRelExp", "relExp", "relop", "sumExp", "sumop", "mulExp",
  "mulop", "unaryExp", "unaryop", "factor", "mutable", "immutable", "call",
  "args", "argList", "constant", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   123,   125,    40,    41,    91,    93,    43,    45,    42,
      47,    37,   296,    59,    44,    58,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    57,    58,    59,    59,    60,    60,    61,    62,    62,
      63,    63,    64,    64,    65,    65,    66,    66,    66,    67,
      67,    68,    68,    69,    69,    70,    71,    71,    72,    72,
      73,    73,    73,    73,    73,    73,    74,    74,    75,    76,
      76,    77,    77,    78,    78,    79,    79,    80,    80,    81,
      81,    82,    83,    83,    83,    83,    84,    84,    84,    84,
      84,    85,    85,    86,    86,    87,    87,    88,    88,    89,
      89,    89,    89,    89,    89,    90,    90,    91,    91,    92,
      92,    93,    93,    93,    94,    94,    95,    95,    95,    96,
      96,    97,    97,    98,    98,    98,    99,   100,   100,   101,
     101,   102,   102,   102,   102
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     3,     4,     3,
       3,     1,     1,     3,     1,     4,     1,     1,     1,     6,
       5,     1,     0,     3,     1,     2,     3,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     2,     1,     4,     2,
       0,     2,     0,     4,     6,     4,     6,     3,     5,     2,
       3,     2,     3,     2,     2,     1,     1,     1,     1,     1,
       1,     3,     1,     3,     1,     2,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     3,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     1,     1,     4,     1,     0,     3,
       1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    16,    17,    18,     0,     2,     4,     5,     0,
       6,    22,     1,     3,    14,     0,    11,    12,     0,     0,
      21,    24,    22,     0,     7,     0,     0,    28,    25,    27,
       0,     0,     0,     0,    14,    10,    91,   101,   102,   103,
     104,     0,     0,    86,    87,    88,    13,    62,    64,    66,
      68,    76,    80,     0,    85,    89,    90,    94,    95,     0,
       0,    40,    20,    23,     0,    15,    98,     0,    65,     0,
      55,    89,     0,     0,    73,    69,    70,    71,    72,    74,
      77,    78,     0,     0,    81,    82,    83,     0,    84,    29,
      26,    42,    19,   100,     0,    97,     0,    93,    56,    57,
      58,    59,    60,    53,    54,     0,    61,    63,    67,    75,
      79,     0,    39,     0,     0,    96,     0,    92,    52,     0,
       0,     0,     0,     0,     0,     0,    38,    37,    41,    30,
      31,    32,    33,    34,    35,     0,    99,     0,     9,     0,
       0,     0,    49,     0,    51,    36,     8,     0,     0,     0,
      50,    43,    45,     0,     0,     0,     0,     0,    44,    46,
      47,     0,    48
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,     7,     8,   112,    15,    16,    17,    18,
      10,    19,    20,    21,    28,    29,   128,   129,   130,    91,
     114,   131,   132,   153,   133,   134,   135,   105,    70,    47,
      48,    49,    82,    50,    83,    51,    87,    52,    53,    54,
      55,    56,    57,    94,    95,    58
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -88
static const yytype_int16 yypact[] =
{
      79,   -20,   -88,   -88,   -88,    32,    79,   -88,   -88,    51,
     -88,    11,   -88,   -88,    20,   -44,   -88,     1,    64,    34,
      39,   -88,    11,    80,   -88,   100,   167,    60,    54,   -88,
      68,    11,    66,    65,    67,   -88,    31,   -88,   -88,   -88,
     -88,   167,   167,   -88,   -88,   -88,   102,    99,   -88,   -88,
     -18,    21,   -88,   176,   -88,   -88,   -88,   -88,   -88,    82,
      64,   -88,   -88,   -88,    68,   -88,   167,   167,   -88,    81,
     102,   160,   167,   167,   -88,   -88,   -88,   -88,   -88,   -88,
     -88,   -88,   176,   176,   -88,   -88,   -88,   176,   -88,   -88,
     -88,    40,   -88,   -88,    85,    72,    90,   -88,   -88,   -88,
     -88,   -88,   -88,   -88,   -88,   167,    99,   -88,   -13,    21,
     -88,    11,   -88,   100,    38,   -88,   167,   -88,   -88,   100,
      -6,   167,   167,   135,    96,    87,   -88,   -88,   -88,   -88,
     -88,   -88,   -88,   -88,   -88,    88,   -88,    44,   -88,    18,
     -11,   117,   -88,    93,   -88,   -88,   -88,   113,   113,   167,
     -88,   127,   -88,   129,    -3,   113,   113,   167,   -88,   -88,
     -16,   167,   102
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -88,   -88,   -88,   142,   -88,   -88,   -82,   125,   -88,     2,
     -88,   131,   -88,   120,   -88,    95,   -87,   -88,   -25,   -88,
     -88,   -88,   -88,   -88,   -88,   -88,   -39,   -88,   -26,    86,
     -28,   -88,   -88,    75,   -88,    76,   -88,   -49,   -88,   -88,
     -41,   -88,   -88,   -88,   -88,   -88
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      46,    71,     9,    69,    88,    62,    72,   148,     9,    24,
      25,    72,    74,    68,    75,    76,    77,    78,    79,    72,
       2,     3,     4,    11,   161,    71,    71,    93,    96,    80,
      81,   120,    12,   147,    80,    81,   157,   137,   110,    92,
      72,    36,    37,    38,    39,   107,    40,   138,    25,     2,
       3,     4,   121,   111,    14,   122,    26,   123,   124,   125,
     151,   152,    41,    22,    71,    23,   118,    27,   158,   159,
      84,    85,    86,    71,    66,    71,    67,   136,    30,    61,
     126,    42,     1,    71,    33,   143,    43,    44,     2,     3,
       4,   127,    31,   113,    45,   139,   140,   146,    25,    36,
      37,    38,    39,    34,    40,    59,    71,    71,    60,    61,
      64,    65,    23,   119,    71,    71,    36,    37,    38,    39,
      41,    40,    73,   154,    72,    97,   116,   121,    89,   115,
     122,   160,   123,   124,   125,   162,   117,    41,   141,    42,
     144,   145,   149,   155,    43,    44,   150,   156,    13,   142,
      35,    63,    45,    32,    61,    90,    42,   108,   106,   109,
       0,    43,    44,     0,     0,     0,   127,     0,     0,    45,
      36,    37,    38,    39,     0,    40,     0,     0,     0,    36,
      37,    38,    39,     0,    40,    98,    99,   100,   101,   102,
       0,    41,     0,     0,     0,     0,     0,   103,   104,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      42,     0,     0,     0,     0,    43,    44,     0,     0,    42,
       0,     0,     0,    45,    43,    44,     0,     0,     0,     0,
       0,     0,    45
};

static const yytype_int16 yycheck[] =
{
      26,    42,     0,    42,    53,    30,    22,    18,     6,    53,
      54,    22,    30,    41,    32,    33,    34,    35,    36,    22,
       9,    10,    11,    43,    40,    66,    67,    66,    67,    47,
      48,   113,     0,    15,    47,    48,    39,   119,    87,    64,
      22,     3,     4,     5,     6,    73,     8,    53,    54,     9,
      10,    11,    14,    13,     3,    17,    55,    19,    20,    21,
     147,   148,    24,    43,   105,    45,   105,     3,   155,   156,
      49,    50,    51,   114,    43,   116,    45,   116,    44,    41,
      42,    43,     3,   124,     4,   124,    48,    49,     9,    10,
      11,    53,    53,    91,    56,   121,   122,    53,    54,     3,
       4,     5,     6,     3,     8,    45,   147,   148,    54,    41,
      44,    46,    45,   111,   155,   156,     3,     4,     5,     6,
      24,     8,    23,   149,    22,    44,    54,    14,    46,    44,
      17,   157,    19,    20,    21,   161,    46,    24,     3,    43,
      53,    53,    25,    16,    48,    49,    53,    18,     6,    53,
      25,    31,    56,    22,    41,    60,    43,    82,    72,    83,
      -1,    48,    49,    -1,    -1,    -1,    53,    -1,    -1,    56,
       3,     4,     5,     6,    -1,     8,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,     8,    25,    26,    27,    28,    29,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    -1,    -1,    48,    49,    -1,    -1,    43,
      -1,    -1,    -1,    56,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    56
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     9,    10,    11,    58,    59,    60,    61,    66,
      67,    43,     0,    60,     3,    63,    64,    65,    66,    68,
      69,    70,    43,    45,    53,    54,    55,     3,    71,    72,
      44,    53,    68,     4,     3,    64,     3,     4,     5,     6,
       8,    24,    43,    48,    49,    56,    85,    86,    87,    88,
      90,    92,    94,    95,    96,    97,    98,    99,   102,    45,
      54,    41,    75,    70,    44,    46,    43,    45,    87,    83,
      85,    97,    22,    23,    30,    32,    33,    34,    35,    36,
      47,    48,    89,    91,    49,    50,    51,    93,    94,    46,
      72,    76,    75,    83,   100,   101,    83,    44,    25,    26,
      27,    28,    29,    37,    38,    84,    86,    87,    90,    92,
      94,    13,    62,    66,    77,    44,    54,    46,    83,    66,
      63,    14,    17,    19,    20,    21,    42,    53,    73,    74,
      75,    78,    79,    81,    82,    83,    83,    63,    53,    85,
      85,     3,    53,    83,    53,    53,    53,    15,    18,    25,
      53,    73,    73,    80,    85,    16,    18,    39,    73,    73,
      85,    40,    85
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
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
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
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
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
		  Type, Value); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
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
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
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
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

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
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



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
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

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


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

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
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 77 "c-.y"
    {
                    //$$ = newDeclNode(ProgramK, 0, $1, NULL, NULL);
                    syntaxTree = (yyvsp[(1) - (1)].tree);   /* root of the AST */
                ;}
    break;

  case 3:
#line 84 "c-.y"
    {
                    (yyval.tree) = addSibling((yyvsp[(1) - (2)].tree), (yyvsp[(2) - (2)].tree));
                ;}
    break;

  case 4:
#line 88 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 5:
#line 94 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 6:
#line 98 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 7:
#line 104 "c-.y"
    {
                    // This rule now creates a VarK node for each item in the varDeclList.
                    // It copies the name and isArray properties from the list.
                    TreeNode * list = (yyvsp[(2) - (3)].tree); // This is a list of IdK or InitK nodes
                    TreeNode * head = NULL;

                    while (list != NULL) {
                        TreeNode * varNode;
                        // If it's an initialization, the name is in the child.
                        if (list->subkind.exp == InitK) {
                            // Create a VarK node. Its child should be the initialization
                            // expression from the InitK node (list->child[1]), not the InitK node itself.
                            varNode = newDeclNode(VarK, list->lineno, list->child[1], NULL, NULL);
                            varNode->attr.name = strdup(list->child[0]->attr.name);
                            // The IdK node (list->child[0]) is now processed, so we can free it
                            // to prevent memory leaks. The InitK node (list) is also processed.
                            varNode->isArray = list->child[0]->isArray;
                        } 
                        // Otherwise, it's a simple ID declaration.
                        else { // IdK
                            // For a simple declaration, the VarK node is a leaf.
                            // It holds the name and type; it doesn't need a child.
                            varNode = newDeclNode(VarK, list->lineno, NULL, NULL, NULL);
                            varNode->attr.name = strdup(list->attr.name);
                            varNode->isArray = list->isArray;
                        }

                        varNode->expType = (yyvsp[(1) - (3)].type);
                        head = addSibling(head, varNode);
                        
                        list = list->sibling;
                    }
                    (yyval.tree) = head;
                ;}
    break;

  case 8:
#line 141 "c-.y"
    {
                    // This rule must also create a sibling list of leaf VarK nodes.
                    TreeNode * list = (yyvsp[(3) - (4)].tree);
                    TreeNode * head = NULL;
                    while (list != NULL) {
                        TreeNode * varNode;
                        if (list->subkind.exp == InitK) {
                            varNode = newDeclNode(VarK, list->lineno, list->child[1], NULL, NULL);
                            varNode->attr.name = strdup(list->child[0]->attr.name);
                            varNode->isArray = list->child[0]->isArray;
                        } else { // IdK
                            varNode = newDeclNode(VarK, list->lineno, NULL, NULL, NULL);
                            varNode->attr.name = strdup(list->attr.name);
                            varNode->isArray = list->isArray;
                        }
                        varNode->isStatic = true; // Set static flag
                        varNode->expType = (yyvsp[(2) - (4)].type);    // Capture type.
                        
                        head = addSibling(head, varNode);
                        list = list->sibling;
                    }
                    (yyval.tree) = head;
                ;}
    break;

  case 9:
#line 165 "c-.y"
    {
                    // This is a local variable declaration.
                    TreeNode * list = (yyvsp[(2) - (3)].tree);
                    TreeNode * head = NULL;
                    while (list != NULL) {
                        TreeNode * varNode;
                        if (list->subkind.exp == InitK) {
                            varNode = newDeclNode(VarK, list->lineno, list->child[1], NULL, NULL);
                            varNode->attr.name = strdup(list->child[0]->attr.name);
                            varNode->isArray = list->child[0]->isArray;
                        } else { // IdK
                            varNode = newDeclNode(VarK, list->lineno, NULL, NULL, NULL);
                            varNode->attr.name = strdup(list->attr.name);
                            varNode->isArray = list->isArray;
                        }

                        varNode->expType = (yyvsp[(1) - (3)].type); // Capture type.
                        
                        head = addSibling(head, varNode);
                        list = list->sibling;
                    }
                    (yyval.tree) = head;
                ;}
    break;

  case 10:
#line 191 "c-.y"
    {
                    (yyval.tree) = addSibling((yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree));
                ;}
    break;

  case 11:
#line 195 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 12:
#line 201 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 13:
#line 205 "c-.y"
    {
                    (yyval.tree) = newExpNode(InitK, yylineno, (yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree), NULL);
                    if ((yyvsp[(1) - (3)].tree)->attr.name != NULL) {
                        (yyval.tree)->attr.name = strdup((yyvsp[(1) - (3)].tree)->attr.name);
                    }
                ;}
    break;

  case 14:
#line 214 "c-.y"
    {
                    (yyval.tree) = newExpNode(IdK, (yyvsp[(1) - (1)].tokenData)->linenum, NULL, NULL, NULL);
                    (yyval.tree)->attr.name = new char[strlen((yyvsp[(1) - (1)].tokenData)->tokenstr) + 1];
                    strcpy((yyval.tree)->attr.name, (yyvsp[(1) - (1)].tokenData)->tokenstr);
                    (yyval.tree)->isArray = false;
                ;}
    break;

  case 15:
#line 221 "c-.y"
    {
                    TreeNode *sizeNode = newExpNode(ConstantK, (yyvsp[(3) - (4)].tokenData)->linenum, NULL, NULL, NULL);
                    sizeNode->attr.value = (yyvsp[(3) - (4)].tokenData)->numValue;
                    sizeNode->expType = Integer;
                    (yyval.tree) = newExpNode(IdK, (yyvsp[(1) - (4)].tokenData)->linenum, sizeNode, NULL, NULL);
                    (yyval.tree)->attr.name = new char[strlen((yyvsp[(1) - (4)].tokenData)->tokenstr) + 1];
                    strcpy((yyval.tree)->attr.name, (yyvsp[(1) - (4)].tokenData)->tokenstr);
                    (yyval.tree)->isArray = true;
                ;}
    break;

  case 16:
#line 232 "c-.y"
    { (yyval.type) = Boolean; ;}
    break;

  case 17:
#line 233 "c-.y"
    { (yyval.type) = Char; ;}
    break;

  case 18:
#line 234 "c-.y"
    { (yyval.type) = Integer; ;}
    break;

  case 19:
#line 238 "c-.y"
    {
                    // Correctly assign parms to child[0] and body to child[1]
                    (yyval.tree) = newDeclNode(FuncK, (yyvsp[(2) - (6)].tokenData)->linenum, (yyvsp[(4) - (6)].tree), (yyvsp[(6) - (6)].tree), NULL);
                    (yyval.tree)->attr.name = new char[strlen((yyvsp[(2) - (6)].tokenData)->tokenstr) + 1];
                    strcpy((yyval.tree)->attr.name, (yyvsp[(2) - (6)].tokenData)->tokenstr);
                    (yyval.tree)->expType = (yyvsp[(1) - (6)].type); // Capture type from typeSpec. $1 is now an ExpType.
                ;}
    break;

  case 20:
#line 246 "c-.y"
    {
                    // Correctly assign parms to child[0] and body to child[1]
                    (yyval.tree) = newDeclNode(FuncK, (yyvsp[(1) - (5)].tokenData)->linenum, (yyvsp[(3) - (5)].tree), (yyvsp[(5) - (5)].tree), NULL);
                    (yyval.tree)->attr.name = new char[strlen((yyvsp[(1) - (5)].tokenData)->tokenstr) + 1];
                    strcpy((yyval.tree)->attr.name, (yyvsp[(1) - (5)].tokenData)->tokenstr);
                    (yyval.tree)->expType = Void; // Default type
                ;}
    break;

  case 21:
#line 256 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 22:
#line 260 "c-.y"
    {
                    (yyval.tree) = NULL;
                ;}
    break;

  case 23:
#line 266 "c-.y"
    {
                    (yyval.tree) = addSibling((yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree));
                ;}
    break;

  case 24:
#line 270 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 25:
#line 276 "c-.y"
    {
                    // Iterate through the parmIdList ($2) and create a ParamK node for each one.
                    // Each new ParamK node gets its type from typeSpec ($1).
                    TreeNode * list = (yyvsp[(2) - (2)].tree);
                    TreeNode * head = NULL;
                    while (list != NULL) {
                        TreeNode * newNode = newDeclNode(ParamK, list->lineno, NULL, NULL, NULL);
                        newNode->attr.name = new char[strlen(list->attr.name) + 1];
                        strcpy(newNode->attr.name, list->attr.name);
                        newNode->isArray = list->isArray;
                        newNode->expType = (yyvsp[(1) - (2)].type); // Assign the ExpType to the node's field
                        
                        head = addSibling(head, newNode);
                        list = list->sibling;
                    }
                    (yyval.tree) = head;
                ;}
    break;

  case 26:
#line 296 "c-.y"
    {
                    (yyval.tree) = addSibling((yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree));
                ;}
    break;

  case 27:
#line 300 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 28:
#line 306 "c-.y"
    {
                    (yyval.tree) = newExpNode(IdK, (yyvsp[(1) - (1)].tokenData)->linenum, NULL, NULL, NULL);
                    (yyval.tree)->attr.name = new char[strlen((yyvsp[(1) - (1)].tokenData)->tokenstr) + 1];
                    strcpy((yyval.tree)->attr.name, (yyvsp[(1) - (1)].tokenData)->tokenstr);
                ;}
    break;

  case 29:
#line 312 "c-.y"
    {
                    (yyval.tree) = newExpNode(IdK, (yyvsp[(1) - (3)].tokenData)->linenum, NULL, NULL, NULL);
                    (yyval.tree)->attr.name = new char[strlen((yyvsp[(1) - (3)].tokenData)->tokenstr) + 1];
                    strcpy((yyval.tree)->attr.name, (yyvsp[(1) - (3)].tokenData)->tokenstr);
                    (yyval.tree)->isArray = true;
                ;}
    break;

  case 30:
#line 321 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 31:
#line 325 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 32:
#line 329 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 33:
#line 333 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 34:
#line 337 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 35:
#line 341 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 36:
#line 347 "c-.y"
    {
                    // Return the expression node itself as the statement.
                    // This removes the incorrect "Null Stmt" wrapper.
                    (yyval.tree) = (yyvsp[(1) - (2)].tree);
                ;}
    break;

  case 37:
#line 353 "c-.y"
    {
                    (yyval.tree) = newStmtNode(NullK, yylineno, NULL, NULL, NULL);
                ;}
    break;

  case 38:
#line 359 "c-.y"
    {
                    (yyval.tree) = newStmtNode(CompoundK, (yyvsp[(1) - (4)].tokenData)->linenum, (yyvsp[(2) - (4)].tree), (yyvsp[(3) - (4)].tree), NULL);
                ;}
    break;

  case 39:
#line 365 "c-.y"
    {
                    //printf("found localDecls\n");
                    (yyval.tree) = addSibling((yyvsp[(1) - (2)].tree), (yyvsp[(2) - (2)].tree));
                ;}
    break;

  case 40:
#line 370 "c-.y"
    {
                    (yyval.tree) = NULL;
                ;}
    break;

  case 41:
#line 376 "c-.y"
    {
                    //printf("found stmtList\n");
                    // Correctly append to the list. addSibling handles the case where $1 is NULL.
                    (yyval.tree) = addSibling((yyvsp[(1) - (2)].tree), (yyvsp[(2) - (2)].tree));
                ;}
    break;

  case 42:
#line 382 "c-.y"
    {
                    //printf("found stmtList\n");
                    (yyval.tree) = NULL;
                ;}
    break;

  case 43:
#line 389 "c-.y"
    {
                    //printToken($1, "IF");

                    //if ($1) printf("  $1->linenum = %d\n", $1->linenum);
                    //if ($2) printf("  $2->linenum = %d\n", $2->lineno);
                    //if ($4) printf("  $4->linenum = %d\n", $4->lineno);

                    // If the 'then' part is an empty statement, don't create a node for it.
                    TreeNode *thenStmt = (yyvsp[(4) - (4)].tree);
                    //if (thenStmt != NULL && thenStmt->nodekind == StmtK && thenStmt->subkind.stmt == NullK) {
                    //    thenStmt = NULL; // This will prevent the Null node from being added to the tree
                    //}
                    (yyval.tree) = newStmtNode(IfK, (yyvsp[(1) - (4)].tokenData)->linenum, (yyvsp[(2) - (4)].tree), thenStmt, NULL);
                ;}
    break;

  case 44:
#line 404 "c-.y"
    {
                    //printf("found selectStmt\n");
                    TreeNode *thenStmt = (yyvsp[(4) - (6)].tree);
                    TreeNode *elseStmt = (yyvsp[(6) - (6)].tree);
                    if (thenStmt != NULL && thenStmt->nodekind == StmtK && thenStmt->subkind.stmt == NullK) thenStmt = NULL;
                    if (elseStmt != NULL && elseStmt->nodekind == StmtK && elseStmt->subkind.stmt == NullK) elseStmt = NULL;

                    (yyval.tree) = newStmtNode(IfK, (yyvsp[(1) - (6)].tokenData)->linenum, (yyvsp[(2) - (6)].tree), thenStmt, elseStmt);
                ;}
    break;

  case 45:
#line 416 "c-.y"
    {
                    (yyval.tree) = newStmtNode(WhileK, (yyvsp[(1) - (4)].tokenData)->linenum, (yyvsp[(2) - (4)].tree), (yyvsp[(4) - (4)].tree), NULL);
                ;}
    break;

  case 46:
#line 420 "c-.y"
    {
                    TreeNode *varNode = newDeclNode(VarK, (yyvsp[(2) - (6)].tokenData)->linenum, NULL, NULL, NULL);
                    varNode->attr.name = new char[strlen((yyvsp[(2) - (6)].tokenData)->tokenstr) + 1];
                    strcpy(varNode->attr.name, (yyvsp[(2) - (6)].tokenData)->tokenstr);
                    varNode->expType = Integer; // For loop variables are implicitly integers
                    (yyval.tree) = newStmtNode(ForK, (yyvsp[(1) - (6)].tokenData)->linenum, varNode, (yyvsp[(4) - (6)].tree), (yyvsp[(6) - (6)].tree));
                ;}
    break;

  case 47:
#line 430 "c-.y"
    {
                    (yyval.tree) = newStmtNode(RangeK, (yyvsp[(2) - (3)].tokenData)->linenum, (yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree), NULL);
                ;}
    break;

  case 48:
#line 434 "c-.y"
    {
                    (yyval.tree) = newStmtNode(RangeK, (yyvsp[(2) - (5)].tokenData)->linenum, (yyvsp[(1) - (5)].tree), (yyvsp[(3) - (5)].tree), (yyvsp[(5) - (5)].tree));
                ;}
    break;

  case 49:
#line 440 "c-.y"
    {
                    (yyval.tree) = newStmtNode(ReturnK, (yyvsp[(1) - (2)].tokenData)->linenum, NULL, NULL, NULL);
                ;}
    break;

  case 50:
#line 444 "c-.y"
    {
                    (yyval.tree) = newStmtNode(ReturnK, (yyvsp[(1) - (3)].tokenData)->linenum, (yyvsp[(2) - (3)].tree), NULL, NULL);
                ;}
    break;

  case 51:
#line 450 "c-.y"
    {
                    (yyval.tree) = newStmtNode(BreakK, (yyvsp[(1) - (2)].tokenData)->linenum, NULL, NULL, NULL);
                ;}
    break;

  case 52:
#line 456 "c-.y"
    {
                    (yyval.tree) = newExpNode(AssignK, (yyvsp[(1) - (3)].tree)->lineno, (yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree), NULL); // Use lineno of the mutable
                    (yyval.tree)->attr.op = (yyvsp[(2) - (3)].op);
                ;}
    break;

  case 53:
#line 461 "c-.y"
    {
                    (yyval.tree) = newExpNode(AssignK, (yyvsp[(2) - (2)].tokenData)->linenum, (yyvsp[(1) - (2)].tree), NULL, NULL);
                    (yyval.tree)->attr.op = INC;
                ;}
    break;

  case 54:
#line 466 "c-.y"
    {
                    (yyval.tree) = newExpNode(AssignK, (yyvsp[(2) - (2)].tokenData)->linenum, (yyvsp[(1) - (2)].tree), NULL, NULL);
                    (yyval.tree)->attr.op = DEC;
                ;}
    break;

  case 55:
#line 471 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 56:
#line 476 "c-.y"
    { (yyval.op) = ASSIGN; ;}
    break;

  case 57:
#line 477 "c-.y"
    { (yyval.op) = ADDASS; ;}
    break;

  case 58:
#line 478 "c-.y"
    { (yyval.op) = SUBASS; ;}
    break;

  case 59:
#line 479 "c-.y"
    { (yyval.op) = MULASS; ;}
    break;

  case 60:
#line 480 "c-.y"
    { (yyval.op) = DIVASS; ;}
    break;

  case 61:
#line 484 "c-.y"
    {
                    (yyval.tree) = newExpNode(OpK, (yyvsp[(2) - (3)].tokenData)->linenum, (yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree), NULL);
                    (yyval.tree)->attr.op = OR;
                ;}
    break;

  case 62:
#line 489 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 63:
#line 495 "c-.y"
    {
                    (yyval.tree) = newExpNode(OpK, (yyvsp[(2) - (3)].tokenData)->linenum, (yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree), NULL);
                    (yyval.tree)->attr.op = AND;
                ;}
    break;

  case 64:
#line 500 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 65:
#line 506 "c-.y"
    {
                    (yyval.tree) = newExpNode(OpK, (yyvsp[(1) - (2)].tokenData)->linenum, (yyvsp[(2) - (2)].tree), NULL, NULL);
                    (yyval.tree)->attr.op = NOT;
                ;}
    break;

  case 66:
#line 511 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 67:
#line 517 "c-.y"
    {
                    (yyval.tree) = newExpNode(OpK, (yyvsp[(1) - (3)].tree)->lineno, (yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree), NULL); // Use lineno of first expression
                    (yyval.tree)->attr.op = (yyvsp[(2) - (3)].op);
                ;}
    break;

  case 68:
#line 522 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 69:
#line 527 "c-.y"
    { (yyval.op) = LT; ;}
    break;

  case 70:
#line 528 "c-.y"
    { (yyval.op) = LE; ;}
    break;

  case 71:
#line 529 "c-.y"
    { (yyval.op) = GT; ;}
    break;

  case 72:
#line 530 "c-.y"
    { (yyval.op) = GE; ;}
    break;

  case 73:
#line 531 "c-.y"
    { (yyval.op) = EQ; ;}
    break;

  case 74:
#line 532 "c-.y"
    { (yyval.op) = NEQ; ;}
    break;

  case 75:
#line 536 "c-.y"
    {
                    (yyval.tree) = newExpNode(OpK, (yyvsp[(1) - (3)].tree)->lineno, (yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree), NULL); // Use lineno of first expression
                    (yyval.tree)->attr.op = (yyvsp[(2) - (3)].op);
                ;}
    break;

  case 76:
#line 541 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 77:
#line 546 "c-.y"
    { (yyval.op) = '+'; ;}
    break;

  case 78:
#line 547 "c-.y"
    { (yyval.op) = '-'; ;}
    break;

  case 79:
#line 551 "c-.y"
    {
                    (yyval.tree) = newExpNode(OpK, (yyvsp[(1) - (3)].tree)->lineno, (yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree), NULL); // Use lineno of first expression
                    (yyval.tree)->attr.op = (yyvsp[(2) - (3)].op);
                ;}
    break;

  case 80:
#line 556 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 81:
#line 561 "c-.y"
    { (yyval.op) = '*'; ;}
    break;

  case 82:
#line 562 "c-.y"
    { (yyval.op) = '/'; ;}
    break;

  case 83:
#line 563 "c-.y"
    { (yyval.op) = '%'; ;}
    break;

  case 84:
#line 567 "c-.y"
    {
                    (yyval.tree) = newExpNode(OpK, (yyvsp[(2) - (2)].tree)->lineno, NULL, NULL, NULL); // Use lineno of expression
                    (yyval.tree)->attr.op = (yyvsp[(1) - (2)].op);
                    (yyval.tree)->child[0] = (yyvsp[(2) - (2)].tree);
                ;}
    break;

  case 85:
#line 573 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 86:
#line 578 "c-.y"
    { (yyval.op) = OP_CHSIGN; ;}
    break;

  case 87:
#line 579 "c-.y"
    { (yyval.op) = OP_SIZEOF; ;}
    break;

  case 88:
#line 580 "c-.y"
    { (yyval.op) = '?'; ;}
    break;

  case 89:
#line 584 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 90:
#line 588 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 91:
#line 594 "c-.y"
    {
                    (yyval.tree) = newExpNode(IdK, (yyvsp[(1) - (1)].tokenData)->linenum, NULL, NULL, NULL);
                    (yyval.tree)->attr.name = new char[strlen((yyvsp[(1) - (1)].tokenData)->tokenstr) + 1];
                    strcpy((yyval.tree)->attr.name, (yyvsp[(1) - (1)].tokenData)->tokenstr);
                ;}
    break;

  case 92:
#line 600 "c-.y"
    {
                    // Create an OpK node for array subscripting.
                    TreeNode *idNode = newExpNode(IdK, (yyvsp[(1) - (4)].tokenData)->linenum, NULL, NULL, NULL);
                    idNode->attr.name = new char[strlen((yyvsp[(1) - (4)].tokenData)->tokenstr) + 1];
                    strcpy(idNode->attr.name, (yyvsp[(1) - (4)].tokenData)->tokenstr);
                    (yyval.tree) = newExpNode(OpK, (yyvsp[(2) - (4)].tokenData)->linenum, idNode, (yyvsp[(3) - (4)].tree), NULL);
                    (yyval.tree)->attr.op = '[';
                ;}
    break;

  case 93:
#line 611 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(2) - (3)].tree);
                ;}
    break;

  case 94:
#line 615 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 95:
#line 619 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 96:
#line 625 "c-.y"
    {
                    (yyval.tree) = newExpNode(CallK, (yyvsp[(1) - (4)].tokenData)->linenum, (yyvsp[(3) - (4)].tree), NULL, NULL);
                    (yyval.tree)->attr.name = new char[strlen((yyvsp[(1) - (4)].tokenData)->tokenstr) + 1];
                    strcpy((yyval.tree)->attr.name, (yyvsp[(1) - (4)].tokenData)->tokenstr);
                ;}
    break;

  case 97:
#line 633 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 98:
#line 637 "c-.y"
    {
                    (yyval.tree) = NULL;
                ;}
    break;

  case 99:
#line 643 "c-.y"
    {
                    (yyval.tree) = addSibling((yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree));
                ;}
    break;

  case 100:
#line 647 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 101:
#line 653 "c-.y"
    {
                    (yyval.tree) = newExpNode(ConstantK, yylineno, NULL, NULL, NULL);
                    (yyval.tree)->attr.value = (yyvsp[(1) - (1)].tokenData)->numValue;
                    (yyval.tree)->expType = Integer;
                ;}
    break;

  case 102:
#line 659 "c-.y"
    {
                    (yyval.tree) = newExpNode(ConstantK, yylineno, NULL, NULL, NULL);
                    (yyval.tree)->attr.cvalue = (yyvsp[(1) - (1)].tokenData)->charValue;
                    (yyval.tree)->expType = Char;
                ;}
    break;

  case 103:
#line 665 "c-.y"
    {
                    (yyval.tree) = newExpNode(ConstantK, yylineno, NULL, NULL, NULL);
                    (yyval.tree)->attr.string = new char[strlen((yyvsp[(1) - (1)].tokenData)->stringValue) + 1];
                    strcpy((yyval.tree)->attr.string, (yyvsp[(1) - (1)].tokenData)->stringValue);
                    (yyval.tree)->expType = CharInt;
                    (yyval.tree)->isArray = true;
                ;}
    break;

  case 104:
#line 673 "c-.y"
    {
                    (yyval.tree) = newExpNode(ConstantK, yylineno, NULL, NULL, NULL);
                    (yyval.tree)->attr.value = (yyvsp[(1) - (1)].tokenData)->boolValue;
                    (yyval.tree)->expType = Boolean;
                ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2455 "c-.tab.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


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
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
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
      if (yyn != YYPACT_NINF)
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


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
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


#line 680 "c-.y"

#include <stdio.h>

extern FILE *yyin;

