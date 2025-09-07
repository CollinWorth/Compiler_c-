%{
#include "scanType.h"      // TokenData struct
#include "c-.tab.h"     // Token definitions from Bison

int line = 1;
int numErrors = 0;

static int setValue(int linenum, int tokenClass, const char *svalue)
{
    yylval.tokenData = new TokenData;
    yylval.tokenData->linenum = linenum;
    yylval.tokenData->tokenstr = strdup(svalue);

    if (tokenClass == NUMCONST) {
        yylval.tokenData->tokenstr = strdup(svalue);  // copies "123"
        yylval.tokenData->numValue = atof(svalue);

    }
    else if (tokenClass == CHARCONST) {
        yylval.tokenData->tokenstr = strdup(svalue); // raw input, e.g. `'a'` or `'\\n'`

        // Allocate interpreted char
        char interpreted = '\0';

        int n = strlen(svalue);
        if (n >= 3) {
            if (n == 4 && svalue[1] == '\\') {
                switch (svalue[2]) {
                    case 'n':  interpreted = '\n'; break;
                    case 't':  interpreted = 't'; break;
                    case '0':  interpreted = '\0'; break;
                    case '\'': interpreted = '\''; break;
                    case '"':  interpreted = '"'; break;
                    case '\\': interpreted = '\\'; break;
                    default:   interpreted = svalue[2]; break;
                }
            } else {
                interpreted = svalue[1];
                if (n > 3) {
                    char buf[256];
                    snprintf(buf, sizeof(buf),
                            "WARNING(%d): character is %d characters long and not a single character: '%s'. The first char will be used.",
                            linenum, n - 2, svalue);
                    yylval.tokenData->warning = strdup(buf); 
                }
            }
        }

        yylval.tokenData->charValue = interpreted; // store interpreted char
        yylval.tokenData->strLen = 1;
    }
    else if (tokenClass == STRINGCONST) {
        yylval.tokenData->tokenstr = strdup(svalue); // raw input, with quotes

        int n = strlen(svalue);
        char* interpreted = (char*) malloc(n); // max possible size
        int j = 0;

        for (int i = 1; i < n - 1; i++) { // skip quotes
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
        yylval.tokenData->stringValue = interpreted; // store interpreted string
        yylval.tokenData->strLen = j;               // actual length
    }
    else if (tokenClass == ID) {
        yylval.tokenData->idIndex = svalue[0] - 'a';
    }
    else if (tokenClass == ERROR) {
        numErrors++;
        yylval.tokenData->stringValue = strdup(svalue);
    }
    else if (tokenClass == SYMBOL){
        yylval.tokenData->stringValue = strdup(svalue);
    }  
    else if (tokenClass == KEYWORD){
        yylval.tokenData->stringValue = strdup(svalue);
    }
    else if (tokenClass == BOOLCONST){
        if(strcmp(svalue, "true") == 0){
            yylval.tokenData->boolValue = 1;
        }else{
            yylval.tokenData->boolValue = 0;
        }
    }
    
    

    return tokenClass;
}
%}

digit       [0-9]
letter      [a-zA-Z]
identifier  {letter}({letter}|[0-9])*

%%

"//".*   { /* Do nothing */ }

"true"      { return setValue(line, BOOLCONST, "true"); }
"false"     { return setValue(line, BOOLCONST, "false"); }

"if"        { return setValue(line, KEYWORD, "IF"); }
"int"       { return setValue(line, KEYWORD, "INT"); }
"bool"      { return setValue(line, KEYWORD, "BOOL"); }
"char"      { return setValue(line, KEYWORD, "CHAR"); }
"static"    { return setValue(line, KEYWORD, "STATIC"); }
"or"        { return setValue(line, KEYWORD, "OR"); }
"and"       { return setValue(line, KEYWORD, "AND"); }
"not"       { return setValue(line, KEYWORD, "NOT"); }
"for"       { return setValue(line, KEYWORD, "FOR"); }
"to"        { return setValue(line, KEYWORD, "TO"); }
"by"        { return setValue(line, KEYWORD, "BY"); }
"do"        { return setValue(line, KEYWORD, "DO"); }
"then"      { return setValue(line, KEYWORD, "THEN"); }
"break"     { return setValue(line, KEYWORD, "BREAK"); }
"else"      { return setValue(line, KEYWORD, "ELSE"); }
"return"    { return setValue(line, KEYWORD, "RETURN"); }
"while"     { return setValue(line, KEYWORD, "WHILE"); }




">="     { return setValue(line, SYMBOL, "GEQ"); }
"<="     { return setValue(line, SYMBOL, "LEQ"); }
":="     { return setValue(line, SYMBOL, "ASGN"); }
"+="     { return setValue(line, SYMBOL, "ADDASS"); }
"-="     { return setValue(line, SYMBOL, yytext); }
"*="     { return setValue(line, SYMBOL, yytext); }
"/="     { return setValue(line, SYMBOL, yytext); }
"--"     { return setValue(line, SYMBOL, "DEC"); }
"++"     { return setValue(line, SYMBOL, "INC"); }
"><"     { return setValue(line, SYMBOL, "NEQ"); }


[ \t\r]+                    { /* skip whitespace */ }

\n                          { line++; }

{identifier}                { return setValue(line, ID, yytext); }

[0-9]+                      { return setValue(line, NUMCONST, yytext); }

\'([^\'\\\n]|\\.)+\'      { return setValue(line, CHARCONST, yytext); }

\"([^"\\\n]|\\.)*\"         { return setValue(line, STRINGCONST, yytext); }

[()[\]{}:;<>?,/%*+\-=?\[\]] { return setValue(line, SYMBOL, yytext); }

.                           { return setValue(line, ERROR, yytext); }

%%

int yywrap() { return 1; }