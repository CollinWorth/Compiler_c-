/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUMCONST = 258,
     ID = 259,
     CHARCONST = 260,
     STRINGCONST = 261,
     BOOLCONST = 262,
     SYMBOL = 263,
     KEYWORD = 264,
     ERROR = 265,
     BOOL = 266,
     CHAR = 267,
     INT = 268,
     STATIC = 269,
     IF = 270,
     THEN = 271,
     ELSE = 272,
     WHILE = 273,
     DO = 274,
     FOR = 275,
     RETURN = 276,
     BREAK = 277,
     TRUE = 278,
     FALSE = 279,
     OR = 280,
     AND = 281,
     NOT = 282,
     ASSIGN = 283,
     ADDASS = 284,
     SUBASS = 285,
     MULASS = 286,
     DIVASS = 287,
     EQ = 288,
     NE = 289,
     LT = 290,
     LE = 291,
     GT = 292,
     GE = 293,
     NEQ = 294,
     INC = 295,
     DEC = 296,
     TO = 297,
     BY = 298
   };
#endif
/* Tokens.  */
#define NUMCONST 258
#define ID 259
#define CHARCONST 260
#define STRINGCONST 261
#define BOOLCONST 262
#define SYMBOL 263
#define KEYWORD 264
#define ERROR 265
#define BOOL 266
#define CHAR 267
#define INT 268
#define STATIC 269
#define IF 270
#define THEN 271
#define ELSE 272
#define WHILE 273
#define DO 274
#define FOR 275
#define RETURN 276
#define BREAK 277
#define TRUE 278
#define FALSE 279
#define OR 280
#define AND 281
#define NOT 282
#define ASSIGN 283
#define ADDASS 284
#define SUBASS 285
#define MULASS 286
#define DIVASS 287
#define EQ 288
#define NE 289
#define LT 290
#define LE 291
#define GT 292
#define GE 293
#define NEQ 294
#define INC 295
#define DEC 296
#define TO 297
#define BY 298




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 42 "c-.y"
{
    TreeNode* tree;
    TokenData* tokenData;
    ExpType type;
}
/* Line 1529 of yacc.c.  */
#line 141 "c-.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

