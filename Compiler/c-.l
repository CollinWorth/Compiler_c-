%{
#include "tree.h"
#include "scanType.h"      // TokenData struct
#include "c-.tab.hpp"      // Token definitions from Bison
#include <stdio.h>
#include "globals.h"

int numErrors = 0;

// Print every token before returning
//#define YY_USER_ACTION printf("Line %d: matched '%s'\n", line, yytext);

static int setValue(int linenum, int tokenClass, const char *svalue)
{
    //printf("Line %d: token %d, text: %s\n", linenum, tokenClass, svalue);
    yylval.tokenData = new TokenData;
    yylval.tokenData->linenum = linenum;
    yylval.tokenData->tokenstr = new char[strlen(svalue) + 1];
    strcpy(yylval.tokenData->tokenstr, svalue);

    if (tokenClass == NUMCONST) {
        yylval.tokenData->numValue = atof(svalue);
    }
    else if (tokenClass == CHARCONST) {
        char interpreted = '\0';
        int n = strlen(svalue);
        if (n >= 3) {
            if (n == 4 && svalue[1] == '\\') {
                switch (svalue[2]) {
                    case 'n':  interpreted = '\n'; break;
                    case 't':  interpreted = '\t'; break;
                    case '0':  interpreted = '\0'; break;
                    case '\'': interpreted = '\''; break;
                    case '"':  interpreted = '"'; break;
                    case '\\': interpreted = '\\'; break;
                    default:   interpreted = svalue[2]; break;
                }
            } else {
                interpreted = svalue[1];
            }
        }
        yylval.tokenData->charValue = interpreted;
        yylval.tokenData->strLen = 1;
    }
    else if (tokenClass == STRINGCONST) {
        int n = strlen(svalue);
        char* interpreted = new char[n]; // Overallocation is safe, will be shorter
        int j = 0;
        for (int i = 1; i < n - 1; i++) {
            if (svalue[i] == '\\') {
                i++;
                switch (svalue[i]) {
                    case 'n': interpreted[j++] = '\n'; break;
                    case '0': interpreted[j++] = ' '; break;
                    case 't': interpreted[j++] = '\t'; break; 
                    case '"': interpreted[j++] = '"'; break;
                    case '\\': interpreted[j++] = '\\'; break;
                    default: interpreted[j++] = svalue[i]; break;
                }
            } else {
                interpreted[j++] = svalue[i];
            }
        }
        interpreted[j] = '\0';
        yylval.tokenData->stringValue = interpreted;
        yylval.tokenData->strLen = j;
    }
    else if (tokenClass == ID) {
        yylval.tokenData->idIndex = svalue[0] - 'a';
    }
    else if (tokenClass == ERROR) {
        numErrors++;
        // No need to copy string for error, tokenstr is already copied.
        // If a different field is needed, allocate and copy.
        // yylval.tokenData->stringValue = strdup(svalue);
    }
    else if (tokenClass == BOOLCONST) {
        yylval.tokenData->boolValue = (strcmp(svalue, "true") == 0);
    }

    return tokenClass;
}

%}

%option yylineno
%option noinput
%option nounput

digit       [0-9]
letter      [a-zA-Z]
identifier  {letter}({letter}|{digit})*

%%

"//".*               { /* ignore comments */ }

"true"               { return setValue(yylineno, BOOLCONST, yytext); }
"false"              { return setValue(yylineno, BOOLCONST, yytext); }

"if"                 { return setValue(yylineno, IF, yytext); }
"int"                { return setValue(yylineno, INT, yytext); }
"bool"               { return setValue(yylineno, BOOL, yytext); }
"char"               { return setValue(yylineno, CHAR, yytext); }
"static"             { return setValue(yylineno, STATIC, yytext); }
"or"                 { return setValue(yylineno, OR, yytext); }
"and"                { return setValue(yylineno, AND, yytext); }
"not"                { return setValue(yylineno, NOT, yytext); }
"for"                { return setValue(yylineno, FOR, yytext); }
"to"                 { return setValue(yylineno, TO, yytext); }
"by"                 { return setValue(yylineno, BY, yytext); }
"do"                 { return setValue(yylineno, DO, yytext); }
"then"               { return setValue(yylineno, THEN, yytext); }
"break"              { return setValue(yylineno, BREAK, yytext); }
"else"               { return setValue(yylineno, ELSE, yytext); }
"return"             { return setValue(yylineno, RETURN, yytext); }
"while"              { return setValue(yylineno, WHILE, yytext); }

">="                 { return setValue(yylineno, GE, yytext); }
"<="                 { return setValue(yylineno, LE, yytext); }
":="                 { return setValue(yylineno, ASSIGN, yytext); }
"+="                 { return setValue(yylineno, ADDASS, yytext); }
"-="                 { return setValue(yylineno, SUBASS, yytext); }
"*="                 { return setValue(yylineno, MULASS, yytext); }
"/="                 { return setValue(yylineno, DIVASS, yytext); }
"--"                 { return setValue(yylineno, DEC, yytext); }
"++"                 { return setValue(yylineno, INC, yytext); }
"><"                 { return setValue(yylineno, NEQ, yytext); }
"="                  { return setValue(yylineno, EQ, yytext); }
"<"                  { return setValue(yylineno, LT, yytext); }
">"                  { return setValue(yylineno, GT, yytext); }

[ \t\r]+             { /* skip whitespace */ }
\n                   { /* yylineno is automatically updated */ }

{identifier}         { 
                        //printf("Lexer: matched symbol '%c' (ASCII %d)\n", *yytext, *yytext); 
                        return setValue(yylineno, ID, yytext); }
{digit}+             { 
                        //printf("Lexer: matched symbol '%c' (ASCII %d)\n", *yytext, *yytext); 
                        return setValue(yylineno, NUMCONST, yytext); }

\'([^\'\\\n]|\\.)+\' { 
    //printf("Lexer: matched symbol '%c' (ASCII %d)\n", *yytext, *yytext);
    return setValue(yylineno, CHARCONST, yytext); }
\"([^"\\\n]|\\.)*\" { 
    //printf("Lexer: matched symbol '%c' (ASCII %d)\n", *yytext, *yytext);
    return setValue(yylineno, STRINGCONST, yytext); }

[()[\]{}:;?,%]  { 
    //printf("Lexer: matched symbol '%c' (ASCII %d)\n", *yytext, *yytext);
    // Create a TokenData struct to hold the line number for punctuation
    yylval.tokenData = new TokenData;
    yylval.tokenData->linenum = yylineno;
    yylval.tokenData->tokenstr = NULL;
    return *yytext; 
}

[+\-*/=]        { 
    //printf("Lexer: matched operator '%c' (ASCII %d)\n", *yytext, *yytext);
    // Create a TokenData struct to hold the line number for simple operators
    yylval.tokenData = new TokenData;
    yylval.tokenData->linenum = yylineno;
    yylval.tokenData->tokenstr = NULL;
    return *yytext; 
}

.                    { 
    //printf("Unknown character: %s\n", yytext); 
    return setValue(yylineno, ERROR, yytext); }

%%

int yywrap() { return 1; }