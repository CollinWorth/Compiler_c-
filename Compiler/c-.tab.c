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

#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include "scanType.h"   // for TokenData struct
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
TreeNode *syntaxTree = NULL;  // global AST root
int printTreeFlag = 1;
static TreeNode * savedTree;


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
#line 48 "c-.y"
{
    TreeNode *tree;
    TokenData *tokenData;
    int op;
    ExpType type;   // Add this field for ExpType enum
}
/* Line 193 of yacc.c.  */
#line 230 "c-.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 243 "c-.tab.c"

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
#define YYLAST   221

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  104
/* YYNRULES -- Number of states.  */
#define YYNSTATES  163

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   295

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    51,     2,     2,
      43,    44,    49,    47,    53,    48,     2,    50,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    54,    52,
       2,     2,     2,    55,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40
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
      57,     0,    -1,    58,    -1,    58,    59,    -1,    59,    -1,
      60,    -1,    66,    -1,    65,    62,    52,    -1,    13,    65,
      62,    52,    -1,    65,    62,    52,    -1,    62,    53,    63,
      -1,    63,    -1,    64,    -1,    64,    54,    84,    -1,     3,
      -1,     3,    45,     4,    46,    -1,     9,    -1,    10,    -1,
      11,    -1,    65,     3,    43,    67,    44,    74,    -1,     3,
      43,    67,    44,    74,    -1,    68,    -1,    -1,    68,    52,
      69,    -1,    69,    -1,    65,    70,    -1,    70,    53,    71,
      -1,    71,    -1,     3,    -1,     3,    45,    46,    -1,    73,
      -1,    74,    -1,    77,    -1,    78,    -1,    80,    -1,    81,
      -1,    82,    52,    -1,    52,    -1,    41,    75,    76,    42,
      -1,    75,    61,    -1,    -1,    76,    72,    -1,    -1,    14,
      84,    15,    72,    -1,    14,    84,    15,    72,    16,    72,
      -1,    17,    84,    18,    72,    -1,    19,     3,    25,    79,
      18,    72,    -1,    84,    39,    84,    -1,    84,    39,    84,
      40,    84,    -1,    20,    52,    -1,    20,    82,    52,    -1,
      21,    52,    -1,    96,    83,    82,    -1,    96,    37,    -1,
      96,    38,    -1,    84,    -1,    25,    -1,    26,    -1,    27,
      -1,    28,    -1,    29,    -1,    84,    22,    85,    -1,    85,
      -1,    85,    23,    86,    -1,    86,    -1,    24,    86,    -1,
      87,    -1,    89,    88,    89,    -1,    89,    -1,    32,    -1,
      33,    -1,    34,    -1,    35,    -1,    30,    -1,    36,    -1,
      89,    90,    91,    -1,    91,    -1,    47,    -1,    48,    -1,
      91,    92,    93,    -1,    93,    -1,    49,    -1,    50,    -1,
      51,    -1,    94,    93,    -1,    95,    -1,    48,    -1,    49,
      -1,    55,    -1,    96,    -1,    97,    -1,     3,    -1,     3,
      45,    82,    46,    -1,    43,    82,    44,    -1,    98,    -1,
     101,    -1,     3,    43,    99,    44,    -1,   100,    -1,    -1,
     100,    53,    82,    -1,    82,    -1,     4,    -1,     5,    -1,
       6,    -1,     8,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    78,    78,    85,    89,    95,    99,   105,   132,   150,
     171,   175,   181,   185,   191,   197,   206,   207,   208,   211,
     219,   229,   234,   239,   243,   249,   269,   273,   279,   285,
     294,   298,   302,   306,   310,   314,   320,   326,   332,   338,
     344,   349,   355,   361,   370,   377,   381,   390,   394,   400,
     404,   410,   416,   421,   426,   431,   437,   438,   439,   440,
     441,   444,   449,   455,   460,   466,   471,   477,   482,   488,
     489,   490,   491,   492,   493,   496,   501,   507,   508,   511,
     516,   522,   523,   524,   527,   533,   539,   540,   541,   544,
     548,   554,   560,   571,   575,   579,   585,   593,   598,   603,
     607,   613,   619,   625,   632
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
  "'%'", "';'", "','", "':'", "'?'", "$accept", "program", "declList",
  "decl", "varDecl", "scopedVarDecl", "varDeclList", "varDeclInit",
  "varDeclId", "typeSpec", "funDecl", "parms", "parmList", "parmTypeList",
  "parmIdList", "parmId", "stmt", "expStmt", "compoundStmt", "localDecls",
  "stmtList", "selectStmt", "iterStmt", "iterRange", "returnStmt",
  "breakStmt", "exp", "assignop", "simpleExp", "andExp", "unaryRelExp",
  "relExp", "relop", "sumExp", "sumop", "mulExp", "mulop", "unaryExp",
  "unaryop", "factor", "mutable", "immutable", "call", "args", "argList",
  "constant", 0
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
      47,    37,    59,    44,    58,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    56,    57,    58,    58,    59,    59,    60,    61,    61,
      62,    62,    63,    63,    64,    64,    65,    65,    65,    66,
      66,    67,    67,    68,    68,    69,    70,    70,    71,    71,
      72,    72,    72,    72,    72,    72,    73,    73,    74,    75,
      75,    76,    76,    77,    77,    78,    78,    79,    79,    80,
      80,    81,    82,    82,    82,    82,    83,    83,    83,    83,
      83,    84,    84,    85,    85,    86,    86,    87,    87,    88,
      88,    88,    88,    88,    88,    89,    89,    90,    90,    91,
      91,    92,    92,    92,    93,    93,    94,    94,    94,    95,
      95,    96,    96,    97,    97,    97,    98,    99,    99,   100,
     100,   101,   101,   101,   101
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
#define YYPACT_NINF -107
static const yytype_int16 yypact[] =
{
     100,   -27,  -107,  -107,  -107,    31,   100,  -107,  -107,    54,
    -107,    58,  -107,  -107,     3,   -12,  -107,     8,    62,    32,
      34,  -107,    58,    80,  -107,    89,     6,    60,    55,  -107,
      71,    58,    78,    83,    93,  -107,    29,  -107,  -107,  -107,
    -107,     6,     6,  -107,  -107,  -107,   114,   120,  -107,  -107,
     -13,    91,  -107,   166,  -107,  -107,  -107,  -107,  -107,   101,
      62,  -107,  -107,  -107,    71,  -107,     6,     6,  -107,   102,
     114,   150,     6,     6,  -107,  -107,  -107,  -107,  -107,  -107,
    -107,  -107,   166,   166,  -107,  -107,  -107,   166,  -107,  -107,
    -107,   115,  -107,  -107,   105,    97,   106,  -107,  -107,  -107,
    -107,  -107,  -107,  -107,  -107,     6,   120,  -107,     4,    91,
    -107,    58,  -107,    89,    39,  -107,     6,  -107,  -107,    89,
      37,     6,     6,   152,    96,   107,  -107,  -107,  -107,  -107,
    -107,  -107,  -107,  -107,  -107,   108,  -107,    45,  -107,    14,
      15,   128,  -107,   111,  -107,  -107,  -107,   113,   113,     6,
    -107,   141,  -107,   140,    -7,   113,   113,     6,  -107,  -107,
     -16,     6,   114
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -107,  -107,  -107,   158,  -107,  -107,  -106,   142,  -107,     2,
    -107,   144,  -107,   149,  -107,   121,   -77,  -107,   -25,  -107,
    -107,  -107,  -107,  -107,  -107,  -107,   -39,  -107,   -26,   110,
     -23,  -107,  -107,   103,  -107,    90,  -107,   -49,  -107,  -107,
     -41,  -107,  -107,  -107,  -107,  -107
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      46,    71,     9,    69,    88,    62,    72,   120,     9,    36,
      37,    38,    39,   137,    40,    72,    11,    74,    68,    75,
      76,    77,    78,    79,   161,    71,    71,    93,    96,   147,
      41,    12,   157,   148,    80,    81,    72,    72,   110,    92,
      24,    25,    36,    37,    38,    39,    22,    40,    23,    42,
     107,    80,    81,   121,    43,    44,   122,    14,   123,   124,
     125,    45,    26,    41,    71,    27,   118,     2,     3,     4,
     151,   152,    66,    71,    67,    71,    30,   136,   158,   159,
      61,   126,    42,    71,    33,   143,    31,    43,    44,   138,
      25,   127,    34,   113,    45,   139,   140,   146,    25,    36,
      37,    38,    39,     1,    40,    59,    71,    71,    60,     2,
       3,     4,    61,   119,    71,    71,    36,    37,    38,    39,
      41,    40,    64,   154,     2,     3,     4,   121,   111,    65,
     122,   160,   123,   124,   125,   162,    72,    41,    23,    42,
      84,    85,    86,    73,    43,    44,    97,    89,   142,   115,
     116,    45,   117,   149,    61,   141,    42,   155,   156,   144,
     145,    43,    44,   150,    13,   127,    32,    35,    45,    36,
      37,    38,    39,   109,    40,    98,    99,   100,   101,   102,
      63,    90,   106,     0,     0,   108,     0,   103,   104,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    42,
       0,     0,     0,     0,    43,    44,     0,     0,     0,     0,
       0,    45
};

static const yytype_int16 yycheck[] =
{
      26,    42,     0,    42,    53,    30,    22,   113,     6,     3,
       4,     5,     6,   119,     8,    22,    43,    30,    41,    32,
      33,    34,    35,    36,    40,    66,    67,    66,    67,    15,
      24,     0,    39,    18,    47,    48,    22,    22,    87,    64,
      52,    53,     3,     4,     5,     6,    43,     8,    45,    43,
      73,    47,    48,    14,    48,    49,    17,     3,    19,    20,
      21,    55,    54,    24,   105,     3,   105,     9,    10,    11,
     147,   148,    43,   114,    45,   116,    44,   116,   155,   156,
      41,    42,    43,   124,     4,   124,    52,    48,    49,    52,
      53,    52,     3,    91,    55,   121,   122,    52,    53,     3,
       4,     5,     6,     3,     8,    45,   147,   148,    53,     9,
      10,    11,    41,   111,   155,   156,     3,     4,     5,     6,
      24,     8,    44,   149,     9,    10,    11,    14,    13,    46,
      17,   157,    19,    20,    21,   161,    22,    24,    45,    43,
      49,    50,    51,    23,    48,    49,    44,    46,    52,    44,
      53,    55,    46,    25,    41,     3,    43,    16,    18,    52,
      52,    48,    49,    52,     6,    52,    22,    25,    55,     3,
       4,     5,     6,    83,     8,    25,    26,    27,    28,    29,
      31,    60,    72,    -1,    -1,    82,    -1,    37,    38,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    48,    49,    -1,    -1,    -1,    -1,
      -1,    55
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     9,    10,    11,    57,    58,    59,    60,    65,
      66,    43,     0,    59,     3,    62,    63,    64,    65,    67,
      68,    69,    43,    45,    52,    53,    54,     3,    70,    71,
      44,    52,    67,     4,     3,    63,     3,     4,     5,     6,
       8,    24,    43,    48,    49,    55,    84,    85,    86,    87,
      89,    91,    93,    94,    95,    96,    97,    98,   101,    45,
      53,    41,    74,    69,    44,    46,    43,    45,    86,    82,
      84,    96,    22,    23,    30,    32,    33,    34,    35,    36,
      47,    48,    88,    90,    49,    50,    51,    92,    93,    46,
      71,    75,    74,    82,    99,   100,    82,    44,    25,    26,
      27,    28,    29,    37,    38,    83,    85,    86,    89,    91,
      93,    13,    61,    65,    76,    44,    53,    46,    82,    65,
      62,    14,    17,    19,    20,    21,    42,    52,    72,    73,
      74,    77,    78,    80,    81,    82,    82,    62,    52,    84,
      84,     3,    52,    82,    52,    52,    52,    15,    18,    25,
      52,    72,    72,    79,    84,    16,    18,    39,    72,    72,
      84,    40,    84
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
#line 79 "c-.y"
    {
                    //$$ = newDeclNode(ProgramK, 0, $1, NULL, NULL);
                    savedTree = (yyvsp[(1) - (1)].tree);   /* root of the AST */
                ;}
    break;

  case 3:
#line 86 "c-.y"
    {
                    (yyval.tree) = addSibling((yyvsp[(1) - (2)].tree), (yyvsp[(2) - (2)].tree));
                ;}
    break;

  case 4:
#line 90 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 5:
#line 96 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 6:
#line 100 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 7:
#line 106 "c-.y"
    {
                    // This rule now creates a VarK node for each item in the varDeclList.
                    // It copies the name and isArray properties from the list.
                    TreeNode * list = (yyvsp[(2) - (3)].tree);
                    TreeNode * head = NULL;
                    TreeNode * current = NULL;

                    while (list != NULL) {
                        TreeNode * newNode = newDeclNode(VarK, list->lineno, NULL, NULL, NULL);
                        newNode->attr.name = new char[strlen(list->attr.name) + 1];
                        strcpy(newNode->attr.name, list->attr.name);
                        newNode->isArray = list->isArray;

                        if (head == NULL) {
                            head = newNode;
                            current = head;
                        } else {
                            current->sibling = newNode;
                            current = newNode;
                        }
                        list = list->sibling;
                    }
                    (yyval.tree) = head;
                ;}
    break;

  case 8:
#line 133 "c-.y"
    {
                    // This rule must also create a sibling list of leaf VarK nodes.
                    TreeNode * list = (yyvsp[(3) - (4)].tree);
                    TreeNode * head = NULL;
                    while (list != NULL) {
                        TreeNode * newNode = newDeclNode(VarK, list->lineno, NULL, NULL, NULL);
                        newNode->attr.name = new char[strlen(list->attr.name) + 1];
                        strcpy(newNode->attr.name, list->attr.name);
                        newNode->isArray = list->isArray;
                        newNode->isStatic = true; // Set static flag
                        newNode->expType = (yyvsp[(2) - (4)].type);    // Capture type. $2 is now an ExpType.
                        
                        head = addSibling(head, newNode);
                        list = list->sibling;
                    }
                    (yyval.tree) = head;
                ;}
    break;

  case 9:
#line 151 "c-.y"
    {
                    // This is a local variable declaration, handled by localDecls.
                    // This rule should be part of localDecls, not a separate scopedVarDecl.
                    // For now, let's make it behave like the main varDecl.
                    TreeNode * list = (yyvsp[(2) - (3)].tree);
                    TreeNode * head = NULL;
                    while (list != NULL) {
                        TreeNode * newNode = newDeclNode(VarK, list->lineno, NULL, NULL, NULL);
                        newNode->attr.name = new char[strlen(list->attr.name) + 1];
                        strcpy(newNode->attr.name, list->attr.name);
                        newNode->isArray = list->isArray;
                        newNode->expType = (yyvsp[(1) - (3)].type); // Capture type. $1 is now an ExpType.
                        
                        head = addSibling(head, newNode);
                        list = list->sibling;
                    }
                    (yyval.tree) = head;
                ;}
    break;

  case 10:
#line 172 "c-.y"
    {
                    (yyval.tree) = addSibling((yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree));
                ;}
    break;

  case 11:
#line 176 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 12:
#line 182 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 13:
#line 186 "c-.y"
    {
                    (yyval.tree) = newExpNode(InitK, yylineno, (yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree), NULL);
                ;}
    break;

  case 14:
#line 192 "c-.y"
    {
                    (yyval.tree) = newExpNode(IdK, yylineno, NULL, NULL, NULL);
                    (yyval.tree)->attr.name = new char[strlen((yyvsp[(1) - (1)].tokenData)->tokenstr) + 1];
                    strcpy((yyval.tree)->attr.name, (yyvsp[(1) - (1)].tokenData)->tokenstr);
                ;}
    break;

  case 15:
#line 198 "c-.y"
    {
                    (yyval.tree) = newExpNode(IdK, yylineno, NULL, NULL, NULL);
                    (yyval.tree)->attr.name = new char[strlen((yyvsp[(1) - (4)].tokenData)->tokenstr) + 1];
                    strcpy((yyval.tree)->attr.name, (yyvsp[(1) - (4)].tokenData)->tokenstr);
                    (yyval.tree)->isArray = true;
                ;}
    break;

  case 16:
#line 206 "c-.y"
    { (yyval.type) = Boolean; ;}
    break;

  case 17:
#line 207 "c-.y"
    { (yyval.type) = Char; ;}
    break;

  case 18:
#line 208 "c-.y"
    { (yyval.type) = Integer; ;}
    break;

  case 19:
#line 212 "c-.y"
    {
                    // Correctly assign parms to child[0] and body to child[1]
                    (yyval.tree) = newDeclNode(FuncK, (yyvsp[(2) - (6)].tokenData)->linenum, (yyvsp[(4) - (6)].tree), (yyvsp[(6) - (6)].tree), NULL);
                    (yyval.tree)->attr.name = new char[strlen((yyvsp[(2) - (6)].tokenData)->tokenstr) + 1];
                    strcpy((yyval.tree)->attr.name, (yyvsp[(2) - (6)].tokenData)->tokenstr);
                    (yyval.tree)->expType = (yyvsp[(1) - (6)].type); // Capture type from typeSpec. $1 is now an ExpType.
                ;}
    break;

  case 20:
#line 220 "c-.y"
    {
                    // Correctly assign parms to child[0] and body to child[1]
                    (yyval.tree) = newDeclNode(FuncK, (yyvsp[(1) - (5)].tokenData)->linenum, (yyvsp[(3) - (5)].tree), (yyvsp[(5) - (5)].tree), NULL);
                    (yyval.tree)->attr.name = new char[strlen((yyvsp[(1) - (5)].tokenData)->tokenstr) + 1];
                    strcpy((yyval.tree)->attr.name, (yyvsp[(1) - (5)].tokenData)->tokenstr);
                    (yyval.tree)->expType = Void; // Default type
                ;}
    break;

  case 21:
#line 230 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 22:
#line 234 "c-.y"
    {
                    (yyval.tree) = NULL;
                ;}
    break;

  case 23:
#line 240 "c-.y"
    {
                    (yyval.tree) = addSibling((yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree));
                ;}
    break;

  case 24:
#line 244 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 25:
#line 250 "c-.y"
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
#line 270 "c-.y"
    {
                    (yyval.tree) = addSibling((yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree));
                ;}
    break;

  case 27:
#line 274 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 28:
#line 280 "c-.y"
    {
                    (yyval.tree) = newExpNode(IdK, yylineno, NULL, NULL, NULL);
                    (yyval.tree)->attr.name = new char[strlen((yyvsp[(1) - (1)].tokenData)->tokenstr) + 1];
                    strcpy((yyval.tree)->attr.name, (yyvsp[(1) - (1)].tokenData)->tokenstr);
                ;}
    break;

  case 29:
#line 286 "c-.y"
    {
                    (yyval.tree) = newExpNode(IdK, yylineno, NULL, NULL, NULL);
                    (yyval.tree)->attr.name = new char[strlen((yyvsp[(1) - (3)].tokenData)->tokenstr) + 1];
                    strcpy((yyval.tree)->attr.name, (yyvsp[(1) - (3)].tokenData)->tokenstr);
                    (yyval.tree)->isArray = true;
                ;}
    break;

  case 30:
#line 295 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 31:
#line 299 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 32:
#line 303 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 33:
#line 307 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 34:
#line 311 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 35:
#line 315 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 36:
#line 321 "c-.y"
    {
                    // Return the expression node itself as the statement.
                    // This removes the incorrect "Null Stmt" wrapper.
                    (yyval.tree) = (yyvsp[(1) - (2)].tree);
                ;}
    break;

  case 37:
#line 327 "c-.y"
    {
                    (yyval.tree) = newStmtNode(NullK, yylineno, NULL, NULL, NULL);
                ;}
    break;

  case 38:
#line 333 "c-.y"
    {
                    (yyval.tree) = newStmtNode(CompoundK, (yyvsp[(1) - (4)].tokenData)->linenum, (yyvsp[(2) - (4)].tree), (yyvsp[(3) - (4)].tree), NULL);
                ;}
    break;

  case 39:
#line 339 "c-.y"
    {
                    printf("found localDecls\n");
                    (yyval.tree) = addSibling((yyvsp[(1) - (2)].tree), (yyvsp[(2) - (2)].tree));
                ;}
    break;

  case 40:
#line 344 "c-.y"
    {
                    (yyval.tree) = NULL;
                ;}
    break;

  case 41:
#line 350 "c-.y"
    {
                    printf("found stmtList\n");
                    (yyval.tree) = addSibling((yyvsp[(1) - (2)].tree), (yyvsp[(2) - (2)].tree));
                ;}
    break;

  case 42:
#line 355 "c-.y"
    {
                    printf("found stmtList\n");
                    (yyval.tree) = NULL;
                ;}
    break;

  case 43:
#line 362 "c-.y"
    {
                    printToken((yyvsp[(1) - (4)].tokenData), "IF");

                    if ((yyvsp[(1) - (4)].tokenData)) printf("  $1->linenum = %d\n", (yyvsp[(1) - (4)].tokenData)->linenum);
                    if ((yyvsp[(2) - (4)].tree)) printf("  $2->linenum = %d\n", (yyvsp[(2) - (4)].tree)->lineno);
                    if ((yyvsp[(4) - (4)].tree)) printf("  $4->linenum = %d\n", (yyvsp[(4) - (4)].tree)->lineno);
                    (yyval.tree) = newStmtNode(IfK, (yyvsp[(1) - (4)].tokenData)->linenum, (yyvsp[(2) - (4)].tree), (yyvsp[(4) - (4)].tree), NULL);
                ;}
    break;

  case 44:
#line 371 "c-.y"
    {
                    printf("found selectStmt\n");
                    (yyval.tree) = newStmtNode(IfK, (yyvsp[(1) - (6)].tokenData)->linenum, (yyvsp[(2) - (6)].tree), (yyvsp[(4) - (6)].tree), (yyvsp[(6) - (6)].tree));
                ;}
    break;

  case 45:
#line 378 "c-.y"
    {
                    (yyval.tree) = newStmtNode(WhileK, (yyvsp[(1) - (4)].tokenData)->linenum, (yyvsp[(2) - (4)].tree), (yyvsp[(4) - (4)].tree), NULL);
                ;}
    break;

  case 46:
#line 382 "c-.y"
    {
                    TreeNode *idNode = newExpNode(IdK, (yyvsp[(2) - (6)].tokenData)->linenum, NULL, NULL, NULL);
                    idNode->attr.name = new char[strlen((yyvsp[(2) - (6)].tokenData)->tokenstr) + 1];
                    strcpy(idNode->attr.name, (yyvsp[(2) - (6)].tokenData)->tokenstr);
                    (yyval.tree) = newStmtNode(ForK, (yyvsp[(1) - (6)].tokenData)->linenum, idNode, (yyvsp[(4) - (6)].tree), (yyvsp[(6) - (6)].tree));
                ;}
    break;

  case 47:
#line 391 "c-.y"
    {
                    (yyval.tree) = newStmtNode(RangeK, (yyvsp[(2) - (3)].tokenData)->linenum, (yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree), NULL);
                ;}
    break;

  case 48:
#line 395 "c-.y"
    {
                    (yyval.tree) = newStmtNode(RangeK, (yyvsp[(2) - (5)].tokenData)->linenum, (yyvsp[(1) - (5)].tree), (yyvsp[(3) - (5)].tree), (yyvsp[(5) - (5)].tree));
                ;}
    break;

  case 49:
#line 401 "c-.y"
    {
                    (yyval.tree) = newStmtNode(ReturnK, (yyvsp[(1) - (2)].tokenData)->linenum, NULL, NULL, NULL);
                ;}
    break;

  case 50:
#line 405 "c-.y"
    {
                    (yyval.tree) = newStmtNode(ReturnK, (yyvsp[(1) - (3)].tokenData)->linenum, (yyvsp[(2) - (3)].tree), NULL, NULL);
                ;}
    break;

  case 51:
#line 411 "c-.y"
    {
                    (yyval.tree) = newStmtNode(BreakK, (yyvsp[(1) - (2)].tokenData)->linenum, NULL, NULL, NULL);
                ;}
    break;

  case 52:
#line 417 "c-.y"
    {
                    (yyval.tree) = newExpNode(AssignK, (yyvsp[(1) - (3)].tree)->lineno, (yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree), NULL); // Use lineno of the mutable
                    (yyval.tree)->attr.op = (yyvsp[(2) - (3)].op);
                ;}
    break;

  case 53:
#line 422 "c-.y"
    {
                    (yyval.tree) = newExpNode(AssignK, (yyvsp[(2) - (2)].tokenData)->linenum, (yyvsp[(1) - (2)].tree), NULL, NULL);
                    (yyval.tree)->attr.op = INC;
                ;}
    break;

  case 54:
#line 427 "c-.y"
    {
                    (yyval.tree) = newExpNode(AssignK, (yyvsp[(2) - (2)].tokenData)->linenum, (yyvsp[(1) - (2)].tree), NULL, NULL);
                    (yyval.tree)->attr.op = DEC;
                ;}
    break;

  case 55:
#line 432 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 56:
#line 437 "c-.y"
    { (yyval.op) = ASSIGN; ;}
    break;

  case 57:
#line 438 "c-.y"
    { (yyval.op) = ADDASS; ;}
    break;

  case 58:
#line 439 "c-.y"
    { (yyval.op) = SUBASS; ;}
    break;

  case 59:
#line 440 "c-.y"
    { (yyval.op) = MULASS; ;}
    break;

  case 60:
#line 441 "c-.y"
    { (yyval.op) = DIVASS; ;}
    break;

  case 61:
#line 445 "c-.y"
    {
                    (yyval.tree) = newExpNode(OpK, (yyvsp[(2) - (3)].tokenData)->linenum, (yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree), NULL);
                    (yyval.tree)->attr.op = OR;
                ;}
    break;

  case 62:
#line 450 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 63:
#line 456 "c-.y"
    {
                    (yyval.tree) = newExpNode(OpK, (yyvsp[(2) - (3)].tokenData)->linenum, (yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree), NULL);
                    (yyval.tree)->attr.op = AND;
                ;}
    break;

  case 64:
#line 461 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 65:
#line 467 "c-.y"
    {
                    (yyval.tree) = newExpNode(OpK, (yyvsp[(1) - (2)].tokenData)->linenum, (yyvsp[(2) - (2)].tree), NULL, NULL);
                    (yyval.tree)->attr.op = NOT;
                ;}
    break;

  case 66:
#line 472 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 67:
#line 478 "c-.y"
    {
                    (yyval.tree) = newExpNode(OpK, (yyvsp[(1) - (3)].tree)->lineno, (yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree), NULL); // Use lineno of first expression
                    (yyval.tree)->attr.op = (yyvsp[(2) - (3)].op);
                ;}
    break;

  case 68:
#line 483 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 69:
#line 488 "c-.y"
    { (yyval.op) = LT; ;}
    break;

  case 70:
#line 489 "c-.y"
    { (yyval.op) = LE; ;}
    break;

  case 71:
#line 490 "c-.y"
    { (yyval.op) = GT; ;}
    break;

  case 72:
#line 491 "c-.y"
    { (yyval.op) = GE; ;}
    break;

  case 73:
#line 492 "c-.y"
    { (yyval.op) = EQ; ;}
    break;

  case 74:
#line 493 "c-.y"
    { (yyval.op) = NEQ; ;}
    break;

  case 75:
#line 497 "c-.y"
    {
                    (yyval.tree) = newExpNode(OpK, (yyvsp[(1) - (3)].tree)->lineno, (yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree), NULL); // Use lineno of first expression
                    (yyval.tree)->attr.op = (yyvsp[(2) - (3)].op);
                ;}
    break;

  case 76:
#line 502 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 77:
#line 507 "c-.y"
    { (yyval.op) = '+'; ;}
    break;

  case 78:
#line 508 "c-.y"
    { (yyval.op) = '-'; ;}
    break;

  case 79:
#line 512 "c-.y"
    {
                    (yyval.tree) = newExpNode(OpK, (yyvsp[(1) - (3)].tree)->lineno, (yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree), NULL); // Use lineno of first expression
                    (yyval.tree)->attr.op = (yyvsp[(2) - (3)].op);
                ;}
    break;

  case 80:
#line 517 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 81:
#line 522 "c-.y"
    { (yyval.op) = '*'; ;}
    break;

  case 82:
#line 523 "c-.y"
    { (yyval.op) = '/'; ;}
    break;

  case 83:
#line 524 "c-.y"
    { (yyval.op) = '%'; ;}
    break;

  case 84:
#line 528 "c-.y"
    {
                    (yyval.tree) = newExpNode(OpK, (yyvsp[(2) - (2)].tree)->lineno, NULL, NULL, NULL); // Use lineno of expression
                    (yyval.tree)->attr.op = (yyvsp[(1) - (2)].op);
                    (yyval.tree)->child[0] = (yyvsp[(2) - (2)].tree);
                ;}
    break;

  case 85:
#line 534 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 86:
#line 539 "c-.y"
    { (yyval.op) = OP_CHSIGN; ;}
    break;

  case 87:
#line 540 "c-.y"
    { (yyval.op) = '*'; ;}
    break;

  case 88:
#line 541 "c-.y"
    { (yyval.op) = '?'; ;}
    break;

  case 89:
#line 545 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 90:
#line 549 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 91:
#line 555 "c-.y"
    {
                    (yyval.tree) = newExpNode(IdK, (yyvsp[(1) - (1)].tokenData)->linenum, NULL, NULL, NULL);
                    (yyval.tree)->attr.name = new char[strlen((yyvsp[(1) - (1)].tokenData)->tokenstr) + 1];
                    strcpy((yyval.tree)->attr.name, (yyvsp[(1) - (1)].tokenData)->tokenstr);
                ;}
    break;

  case 92:
#line 561 "c-.y"
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
#line 572 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(2) - (3)].tree);
                ;}
    break;

  case 94:
#line 576 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 95:
#line 580 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 96:
#line 586 "c-.y"
    {
                    (yyval.tree) = newExpNode(CallK, (yyvsp[(1) - (4)].tokenData)->linenum, (yyvsp[(3) - (4)].tree), NULL, NULL);
                    (yyval.tree)->attr.name = new char[strlen((yyvsp[(1) - (4)].tokenData)->tokenstr) + 1];
                    strcpy((yyval.tree)->attr.name, (yyvsp[(1) - (4)].tokenData)->tokenstr);
                ;}
    break;

  case 97:
#line 594 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 98:
#line 598 "c-.y"
    {
                    (yyval.tree) = NULL;
                ;}
    break;

  case 99:
#line 604 "c-.y"
    {
                    (yyval.tree) = addSibling((yyvsp[(1) - (3)].tree), (yyvsp[(3) - (3)].tree));
                ;}
    break;

  case 100:
#line 608 "c-.y"
    {
                    (yyval.tree) = (yyvsp[(1) - (1)].tree);
                ;}
    break;

  case 101:
#line 614 "c-.y"
    {
                    (yyval.tree) = newExpNode(ConstantK, yylineno, NULL, NULL, NULL);
                    (yyval.tree)->attr.value = (yyvsp[(1) - (1)].tokenData)->numValue;
                    (yyval.tree)->expType = Integer;
                ;}
    break;

  case 102:
#line 620 "c-.y"
    {
                    (yyval.tree) = newExpNode(ConstantK, yylineno, NULL, NULL, NULL);
                    (yyval.tree)->attr.cvalue = (yyvsp[(1) - (1)].tokenData)->charValue;
                    (yyval.tree)->expType = Char;
                ;}
    break;

  case 103:
#line 626 "c-.y"
    {
                    (yyval.tree) = newExpNode(ConstantK, yylineno, NULL, NULL, NULL);
                    (yyval.tree)->attr.string = new char[strlen((yyvsp[(1) - (1)].tokenData)->stringValue) + 1];
                    strcpy((yyval.tree)->attr.string, (yyvsp[(1) - (1)].tokenData)->stringValue);
                    (yyval.tree)->expType = CharInt;
                ;}
    break;

  case 104:
#line 633 "c-.y"
    {
                    (yyval.tree) = newExpNode(ConstantK, yylineno, NULL, NULL, NULL);
                    (yyval.tree)->attr.value = (yyvsp[(1) - (1)].tokenData)->boolValue;
                    (yyval.tree)->expType = Boolean;
                ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2416 "c-.tab.c"
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


#line 640 "c-.y"

#include <stdio.h>

extern FILE *yyin;

int yyparse();

int yyerror(const char *s) {

    //if($1->stringValue){
    //    printf("%s",$1->stringValue);
    //}
    
    fprintf(stderr, "Parse error: %s\n", s);
    return 0;
}

int main(int argc, char **argv) {
    if(argc > 1){
        if(strcmp(argv[1], "-p") == 0){
            // parse from file if provided as third argument
            if(argc == 3){
                yyin = fopen(argv[2], "r");
                if(!yyin){
                    perror("fopen");
                    return 1;
                }
            } else {
                yyin = stdin;
            }
        } else {
            yyin = fopen(argv[1], "r");
            if(!yyin){
                perror("fopen");
                return 1;
            }
        }
    } else {
        yyin = stdin;
    }

    yydebug = 0;

    if(yyparse() == 0){
        if(argc > 1 && strcmp(argv[1], "-p") == 0){
            printTree(stdout, savedTree);
        }
    }

    return 0;
}

