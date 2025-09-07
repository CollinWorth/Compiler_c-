%{
#include <stdio.h>
#include <stdlib.h>
#include "scanType.h"   // for TokenData struct
#include <math.h>

void freeTokenData(struct TokenData* tok) {
    if (!tok) return;
    
    if (tok->tokenstr) {
        free(tok->tokenstr);
        tok->tokenstr = NULL;
    }

    if (tok->stringValue) {
        free(tok->stringValue);
        tok->stringValue = NULL;
    }
    free(tok); // finally free the struct itself
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

int yylex(void);
int yyerror(const char *s);
%}


%union {
    TokenData *tokenData;
}
%token <tokenData> NUMCONST ID CHARCONST STRINGCONST BOOLCONST SYMBOL KEYWORD ERROR
%%
/* Minimal grammar */
input:
      /* empty */
    | input token
    ;

token:
      ID {
          printf("Line %d Token: ID Value: %s\n", $1->linenum, $1->tokenstr);
          freeTokenData($1);
      }
    | NUMCONST {
            printf("Line %d Token: NUMCONST Value: %s Input: %s\n",
            $1->linenum,
            formatNumValue($1->numValue),
            $1->tokenstr);
            freeTokenData($1);
      }
    | CHARCONST {
            if ($1->warning) {
                printf("%s\n", $1->warning);
                free($1->warning);
                $1->warning = NULL;
            }
            printf("Line %d Token: CHARCONST Value: \'%c\' Input: %s\n",
                $1->linenum,
                $1->charValue,
                $1->tokenstr);
            freeTokenData($1);
      }
    | STRINGCONST {
          printf("Line %d Token: STRINGCONST Value: \"%s\" Len: %d Input: %s\n",
                 $1->linenum,
                 $1->stringValue,
                 $1->strLen,
                 $1->tokenstr);
          freeTokenData($1);
      }
    | BOOLCONST {
        printf("Line %d Token: BOOLCONST Value: %d Input: %s\n",
                $1->linenum,
                $1->boolValue,
                $1->tokenstr);
        freeTokenData($1);
    }
    | SYMBOL {
          printf("Line %d Token: %s\n", $1->linenum, $1->tokenstr);
          freeTokenData($1);
      }
    | KEYWORD {
          printf("Line %d Token: %s\n", $1->linenum, $1->tokenstr);
          freeTokenData($1);
      }
    | ERROR {
          printf("ERROR(%d): Invalid or misplaced input character: '%s'. Character Ignored. \n", $1->linenum, $1->tokenstr);
          freeTokenData($1);
      }
;

%%
#include <stdio.h>

extern FILE *yyin;
int yyparse();

int yyerror(const char *s) {
    
    fprintf(stderr, "Parse error: %s\n", s);
    return 0;
}

int main(int argc, char ** argv) {
    if(argc > 1){
        yyin = (fopen(argv[1], "r"));
        if(!yyin){
            perror("fopen");
            return 1;
        }
    }else{
        yyin = stdin; // This is fallback 
    }   
    
    return yyparse();
    return 0;
}

