%{
#include "scanType.h"      // TokenData struct
#include "parser.tab.h"     // Token definitions from Bison

int line = 1;
int numErrors = 0;


static int setValue(int linenum, int tokenClass, char *svalue)
{
    yylval.tokenData = new TokenData;
    yylval.tokenData->linenum = linenum;
    yylval.tokenData->tokenstr = strdup(svalue);

    if (tokenClass == NUMCONST) {
        yylval.tokenData->numValue = atof(svalue);
    }
    else if (tokenClass == CHARCONST) {
        yylval.tokenData->charValue = svalue[1]; // char inside quotes
    }
    else if (tokenClass == STRINGCONST) {
        yylval.tokenData->stringValue = strdup(svalue);
    }
    else if (tokenClass == ID) {
        yylval.tokenData->idIndex = svalue[0] - 'a';
    }
    else if (tokenClass == ERROR) {
        numErrors++;
        fprintf(stderr, "ERROR(1): Invalid or misplaced input character: '%s'. Character Ignored. \n", svalue);
    }
    else if (tokenClass == SYMBOL){
        yylval.tokenData->symValue = svalue[0];
    }
    

    return tokenClass;
}
%}

digit       [0-9]
letter      [a-zA-Z]
identifier  {letter}({letter}|[0-9])*

%%




"and"    { return setValue(line, SYMBOL, yytext); }
"or"     { return setValue(line, SYMBOL, yytext); }
"not"    { return setValue(line, SYMBOL, yytext); }
"=="     { return setValue(line, SYMBOL, yytext); }
">="     { return setValue(line, SYMBOL, yytext); }
"<="     { return setValue(line, SYMBOL, yytext); }
":="     { return setValue(line, SYMBOL, yytext); }
"+="     { return setValue(line, SYMBOL, yytext); }
"-="     { return setValue(line, SYMBOL, yytext); }
"*="     { return setValue(line, SYMBOL, yytext); }
"/="     { return setValue(line, SYMBOL, yytext); }
"--"     { return setValue(line, SYMBOL, yytext); }
"++"     { return setValue(line, SYMBOL, yytext); }
"><"     { return setValue(line, SYMBOL, yytext); }
"[]"     { return setValue(line, SYMBOL, yytext); }


[ \t\r]+                  { /* skip whitespace */ }

\n                        { line++; }

{identifier}               { return setValue(line, ID, yytext); }

[0-9]+                    { return setValue(line, NUMCONST, yytext); }

\'([^\'\\]|\\[n0\\'])\'   { return setValue(line, CHARCONST, yytext); }

\"([^"\\\n]|\\.)*\"        { return setValue(line, STRINGCONST, yytext); }

[()[\]{}:;<>?,/%*+\-=?\[\]] { return setValue(line, SYMBOL, yytext); }

.                          { return setValue(line, ERROR, yytext); }

%%
