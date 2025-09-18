%{
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include "scanType.h"   // for TokenData struct
#include <math.h>
#include "globals.h"

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

extern int yylineno;
int yylex(void);
int yyerror(const char *s);
TreeNode *syntaxTree = NULL;  // global AST root
int printTreeFlag = 1;
static TreeNode * savedTree;
%}

%union {
    TreeNode* tree;
    TokenData* tokenData;
    ExpType type;
}
%token <tokenData> NUMCONST ID CHARCONST STRINGCONST BOOLCONST SYMBOL KEYWORD ERROR
%token <type> BOOL CHAR INT
%token STATIC
%token IF THEN ELSE WHILE DO FOR RETURN BREAK
%token TRUE FALSE
%token OR AND NOT
%token ASSIGN ADDASS SUBASS MULASS DIVASS  
%token EQ NE LT LE GT GE NEQ               
%token INC DEC 
%token TO BY
%type <tree> program declList
%debug

%%

program         : declList
                {
                    $$ = newDeclNode(ProgramK, 0, $1, NULL, NULL);
                    savedTree = $$;   /* root of the AST */
                }
                ;

declList        : declList decl
                {
                    $$ = addSibling($1, $2);
                }
                | decl
                {
                    $$ = $1;
                }
                ;

decl            : varDecl
                {
                    $$ = $1;
                }
                | funDecl
                {
                    $$ = $1;
                }
                ;

varDecl         : typeSpec varDeclList ';'
                ;

scopedVarDecl   : STATIC typeSpec varDeclList ';'
                | typeSpec varDeclList ';'
                ;

varDeclList     : varDeclList ',' varDeclInit
                | varDeclInit
                ;

varDeclInit     : varDeclId
                | varDeclId ':' simpleExp
                ;

varDeclId       : ID
                | ID '[' NUMCONST ']'
                ;

typeSpec        : BOOL
                | CHAR
                | INT
                ;

funDecl         : typeSpec ID '(' parms ')' compoundStmt
                | ID '(' parms ')' compoundStmt
                ;

parms           : parmList
                | /* empty */
                ;

parmList        : parmList ';' parmTypeList
                | parmTypeList
                ;

parmTypeList    : typeSpec parmIdList
                ;

parmIdList      : parmIdList ',' parmId
                | parmId
                ;

parmId          : ID
                | ID '[' ']'
                ;

stmt            : expStmt
                | compoundStmt
                | selectStmt
                | iterStmt
                | returnStmt
                | breakStmt
                ;

expStmt         : exp ';'
                | ';'
                ;

compoundStmt    : '{' localDecls stmtList '}'
                ;

localDecls      : localDecls scopedVarDecl
                | /* empty */
                ;

stmtList        : stmtList stmt
                {
                    $$ = addSibling($1, $2);
                }
                | /* empty */
                {
                    $$ = NULL;
                }
                ;

selectStmt      : IF simpleExp THEN stmt
                | IF simpleExp THEN stmt ELSE stmt
                ;

iterStmt        : WHILE simpleExp DO stmt
                | FOR ID ASSIGN iterRange DO stmt
                ;

iterRange       : simpleExp TO simpleExp
                | simpleExp TO simpleExp BY simpleExp
                ;

returnStmt      : RETURN ';'
                | RETURN exp ';'
                ;

breakStmt       : BREAK ';'
                ;

exp             : mutable assignop exp
                | mutable INC
                | mutable DEC
                | simpleExp
                ;

assignop        : ASSIGN
                | ADDASS
                | SUBASS
                | MULASS
                | DIVASS
                ;

simpleExp       : simpleExp OR andExp
                | andExp
                ;

andExp          : andExp AND unaryRelExp
                | unaryRelExp
                ;

unaryRelExp     : NOT unaryRelExp
                | relExp
                ;

relExp          : sumExp relop sumExp
                | sumExp
                ;

relop           : LT
                | LE
                | GT
                | GE
                | EQ
                | NEQ
                ;

sumExp          : sumExp sumop mulExp
                | mulExp
                ;

sumop           : '+'
                | '-'
                ;

mulExp          : mulExp mulop unaryExp
                | unaryExp
                ;

mulop           : '*'
                | '/'
                | '%'
                ;

unaryExp        : unaryop unaryExp
                | factor
                ;

unaryop         : '-'
                | '*'
                | '?'
                ;

factor          : mutable
                | immutable
                ;

mutable         : ID
                | ID '[' exp ']'
                ;

immutable       : '(' exp ')'
                | call
                | constant
                ;

call            : ID '(' args ')'
                ;

args            : argList
                | /* empty */
                ;

argList         : argList ',' exp
                | exp
                ;

constant        : NUMCONST
                | CHARCONST
                | STRINGCONST
                | TRUE
                | FALSE
                ;

%%
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
    yydebug = 0;
    return yyparse();
    return 0;
}

