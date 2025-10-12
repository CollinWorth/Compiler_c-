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




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 43 "c-.y"
{
    TreeNode *tree;
    TokenData *tokenData;
    int op;
    ExpType type;   // Add this field for ExpType enum
}
/* Line 1529 of yacc.c.  */
#line 136 "c-.tab.hpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

