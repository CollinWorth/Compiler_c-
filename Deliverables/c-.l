%{
#include "tree.h"
#include "scanType.h"      // TokenData struct
#include "c-.tab.h"        // Token definitions from Bison
#include <stdio.h>
#include "globals.h"

int line = 1;
int numErrors = 0;

// Print every token before returning
//#define YY_USER_ACTION printf("Line %d: matched '%s'\n", line, yytext);

static int setValue(int linenum, int tokenClass, const char *svalue)
{
    //printf("Line %d: token %d, text: %s\n", linenum, tokenClass, svalue);
    yylval.tokenData = new TokenData;
    yylval.tokenData->linenum = linenum;
    yylval.tokenData->tokenstr = strdup(svalue);

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
        char* interpreted = (char*) malloc(n);
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
        yylval.tokenData->stringValue = strdup(svalue);
    }
    else if (tokenClass == BOOLCONST) {
        yylval.tokenData->boolValue = (strcmp(svalue, "true") == 0);
    }

    return tokenClass;
}
%}

digit       [0-9]
letter      [a-zA-Z]
identifier  {letter}({letter}|[0-9])*

%%

"//".*               { /* ignore comments */ }

"true"               { return TRUE; }
"false"              { return FALSE; }

"if"                 { return IF; }
"int"                { return INT; }
"bool"               { return BOOL; }
"char"               { return CHAR; }
"static"             { return STATIC; }
"or"                 { return OR; }
"and"                { return AND; }
"not"                { return NOT; }
"for"                { return FOR; }
"to"                 { return TO; }
"by"                 { return BY; }
"do"                 { return DO; }
"then"               { return THEN; }
"break"              { return BREAK; }
"else"               { return ELSE; }
"return"             { return RETURN; }
"while"              { return WHILE; }

">="                 { return GE; }
"<="                 { return LE; }
":="                 { return ASSIGN; }
"+="                 { return ADDASS; }
"-="                 { return SUBASS; }
"*="                 { return MULASS; }
"/="                 { return DIVASS; }
"--"                 { return DEC; }
"++"                 { return INC; }
"><"                 { return NEQ; }
"=="                 { return EQ; }
"="                 { return EQ; }
"<"                  { return LT; }
">"                  { return GT; }

[ \t\r]+             { /* skip whitespace */ }
\n                   { line++; }

{identifier}         { 
                        //printf("Lexer: matched symbol '%c' (ASCII %d)\n", *yytext, *yytext); 
                        return setValue(line, ID, yytext); }
{digit}+             { 
                        //printf("Lexer: matched symbol '%c' (ASCII %d)\n", *yytext, *yytext); 
                        return setValue(line, NUMCONST, yytext); }

\'([^\'\\\n]|\\.)+\' { 
    //printf("Lexer: matched symbol '%c' (ASCII %d)\n", *yytext, *yytext);
    return setValue(line, CHARCONST, yytext); }
\"([^"\\\n]|\\.)*\"  { 
    //printf("Lexer: matched symbol '%c' (ASCII %d)\n", *yytext, *yytext);
    return setValue(line, STRINGCONST, yytext); }

[()[\]{}:;?,%]  { 
    //printf("Lexer: matched symbol '%c' (ASCII %d)\n", *yytext, *yytext);
    return *yytext; 
}

[+\-*/=]        { 
    //printf("Lexer: matched operator '%c' (ASCII %d)\n", *yytext, *yytext);
    return *yytext; 
}

.                    { 
    //printf("Unknown character: %s\n", yytext); 
    return setValue(line, ERROR, yytext); }

%%

int yywrap() { return 1; }