%{
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
    TreeNode *tree;
    TokenData *tokenData;
    int op;
    ExpType type;   // Add this field for ExpType enum
}

%token <tokenData> ID NUMCONST CHARCONST STRINGCONST ERROR BOOLCONST
%token BOOL CHAR INT VOID
%token STATIC
%token IF THEN ELSE WHILE DO FOR RETURN BREAK
%token OR AND NOT
%token ASSIGN ADDASS SUBASS MULASS DIVASS  
%token EQ NE LT LE GT GE NEQ               
%token INC DEC 
%token TO BY

%type <tree> program declList decl varDecl scopedVarDecl varDeclList varDeclInit varDeclId funDecl parms parmList parmTypeList parmIdList parmId stmt expStmt compoundStmt localDecls stmtList selectStmt iterStmt iterRange returnStmt breakStmt exp simpleExp andExp unaryRelExp relExp sumExp mulExp unaryExp factor mutable immutable call args argList constant
%type <op> assignop relop sumop mulop unaryop
%type <type> typeSpec // Change typeSpec to use the new 'type' field

%debug
%start program

%%


program         : declList
                {
                    //$$ = newDeclNode(ProgramK, 0, $1, NULL, NULL);
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
                {
                    // This rule now creates a VarK node for each item in the varDeclList.
                    // It copies the name and isArray properties from the list.
                    TreeNode * list = $2;
                    TreeNode * head = NULL;
                    TreeNode * current = NULL;

                    while (list != NULL) {
                        TreeNode * newNode = newDeclNode(VarK, list->lineno, NULL, NULL, NULL);
                        newNode->attr.name = strdup(list->attr.name);
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
                    $$ = head;
                }
                ;

scopedVarDecl   : STATIC typeSpec varDeclList ';'
                {
                    // This rule must also create a sibling list of leaf VarK nodes.
                    TreeNode * list = $3;
                    TreeNode * head = NULL;
                    while (list != NULL) {
                        TreeNode * newNode = newDeclNode(VarK, list->lineno, NULL, NULL, NULL);
                        newNode->attr.name = strdup(list->attr.name);
                        newNode->isArray = list->isArray;
                        newNode->isStatic = true; // Set static flag
                        newNode->expType = $2;    // Capture type. $2 is now an ExpType.
                        
                        head = addSibling(head, newNode);
                        list = list->sibling;
                    }
                    $$ = head;
                }
                | typeSpec varDeclList ';'
                {
                    // This is a local variable declaration, handled by localDecls.
                    // This rule should be part of localDecls, not a separate scopedVarDecl.
                    // For now, let's make it behave like the main varDecl.
                    TreeNode * list = $2;
                    TreeNode * head = NULL;
                    while (list != NULL) {
                        TreeNode * newNode = newDeclNode(VarK, list->lineno, NULL, NULL, NULL);
                        newNode->attr.name = strdup(list->attr.name);
                        newNode->isArray = list->isArray;
                        newNode->expType = $1; // Capture type. $1 is now an ExpType.
                        
                        head = addSibling(head, newNode);
                        list = list->sibling;
                    }
                    $$ = head;
                }
                ;

varDeclList     : varDeclList ',' varDeclInit
                {
                    $$ = addSibling($1, $3);
                }
                | varDeclInit
                {
                    $$ = $1;
                }
                ;

varDeclInit     : varDeclId
                {
                    $$ = $1;
                }
                | varDeclId ':' simpleExp
                {
                    $$ = newExpNode(InitK, yylineno, $1, $3, NULL);
                }
                ;

varDeclId       : ID
                {
                    $$ = newExpNode(IdK, yylineno, NULL, NULL, NULL);
                    $$->attr.name = strdup($1->tokenstr);
                }
                | ID '[' NUMCONST ']'
                {
                    $$ = newExpNode(IdK, yylineno, NULL, NULL, NULL);
                    $$->attr.name = strdup($1->tokenstr);
                    $$->isArray = true;
                }
                ;

typeSpec        : BOOL              { $$ = Boolean; }
                | CHAR              { $$ = Char; }
                | INT               { $$ = Integer; }
                ;

funDecl         : typeSpec ID '(' parms ')' compoundStmt
                {
                    // Correctly assign parms to child[0] and body to child[1]
                    $$ = newDeclNode(FuncK, @2.first_line, $4, $6, NULL);
                    $$->attr.name = strdup($2->tokenstr);
                    $$->expType = $1; // Capture type from typeSpec. $1 is now an ExpType.
                }
                | ID '(' parms ')' compoundStmt
                {
                    // Correctly assign parms to child[0] and body to child[1]
                    $$ = newDeclNode(FuncK, @1.first_line, $3, $5, NULL);
                    $$->attr.name = strdup($1->tokenstr);
                    $$->expType = Void; // Default type
                }
                ;

parms           : parmList
                {
                    $$ = $1;
                }
                | /* empty */
                {
                    $$ = NULL;
                }
                ;

parmList        : parmList ';' parmTypeList
                {
                    $$ = addSibling($1, $3);
                }
                | parmTypeList
                {
                    $$ = $1;
                }
                ;

parmTypeList    : typeSpec parmIdList
                {
                    // Iterate through the parmIdList ($2) and create a ParamK node for each one.
                    // Each new ParamK node gets its type from typeSpec ($1).
                    TreeNode * list = $2;
                    TreeNode * head = NULL;
                    while (list != NULL) {
                        TreeNode * newNode = newDeclNode(ParamK, list->lineno, NULL, NULL, NULL);
                        newNode->attr.name = strdup(list->attr.name);
                        newNode->isArray = list->isArray;
                        newNode->expType = $1; // Assign the ExpType to the node's field
                        
                        head = addSibling(head, newNode);
                        list = list->sibling;
                    }
                    $$ = head;
                }
                ;

parmIdList      : parmIdList ',' parmId
                {
                    $$ = addSibling($1, $3);
                }
                | parmId
                {
                    $$ = $1;
                }
                ;

parmId          : ID
                {
                    $$ = newExpNode(IdK, yylineno, NULL, NULL, NULL);
                    $$->attr.name = strdup($1->tokenstr);
                }
                | ID '[' ']'
                {
                    $$ = newExpNode(IdK, yylineno, NULL, NULL, NULL);
                    $$->attr.name = strdup($1->tokenstr);
                    $$->isArray = true;
                }
                ;

stmt            : expStmt
                {
                    $$ = $1;
                }
                | compoundStmt
                {
                    $$ = $1;
                }
                | selectStmt
                {
                    $$ = $1;
                }
                | iterStmt
                {
                    $$ = $1;
                }
                | returnStmt
                {
                    $$ = $1;
                }
                | breakStmt
                {
                    $$ = $1;
                }
                ;

expStmt         : exp ';'
                {
                    // Return the expression node itself as the statement.
                    // This removes the incorrect "Null Stmt" wrapper.
                    $$ = $1;
                }
                | ';'
                {
                    $$ = newStmtNode(NullK, yylineno, NULL, NULL, NULL);
                }
                ;

compoundStmt    : '{' localDecls stmtList '}'
                {
                    $$ = newStmtNode(CompoundK, yylineno, $2, $3, NULL);
                }
                ;

localDecls      : localDecls scopedVarDecl
                {
                    $$ = addSibling($1, $2);
                }
                | /* empty */
                {
                    $$ = NULL;
                }
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
                {
                    $$ = newStmtNode(IfK, yylineno, $2, $4, NULL);
                }
                | IF simpleExp THEN stmt ELSE stmt
                {
                    $$ = newStmtNode(IfK, yylineno, $2, $4, $6);
                }
                ;

iterStmt        : WHILE simpleExp DO stmt
                {
                    $$ = newStmtNode(WhileK, yylineno, $2, $4, NULL);
                }
                | FOR ID ASSIGN iterRange DO stmt
                {
                    TreeNode *idNode = newExpNode(IdK, @2.first_line, NULL, NULL, NULL);
                    idNode->attr.name = strdup($2->tokenstr);
                    $$ = newStmtNode(ForK, yylineno, idNode, $4, $6);
                }
                ;

iterRange       : simpleExp TO simpleExp
                {
                    $$ = newStmtNode(RangeK, yylineno, $1, $3, NULL);
                }
                | simpleExp TO simpleExp BY simpleExp
                {
                    $$ = newStmtNode(RangeK, yylineno, $1, $3, $5);
                }
                ;

returnStmt      : RETURN ';'
                {
                    $$ = newStmtNode(ReturnK, yylineno, NULL, NULL, NULL);
                }
                | RETURN exp ';'
                {
                    $$ = newStmtNode(ReturnK, yylineno, $2, NULL, NULL);
                }
                ;

breakStmt       : BREAK ';'
                {
                    $$ = newStmtNode(BreakK, yylineno, NULL, NULL, NULL);
                }
                ;

exp             : mutable assignop exp
                {
                    $$ = newExpNode(AssignK, yylineno, $1, $3, NULL);
                    $$->attr.op = $2;
                }
                | mutable INC
                {
                    $$ = newExpNode(AssignK, yylineno, $1, NULL, NULL);
                    $$->attr.op = INC;
                }
                | mutable DEC
                {
                    $$ = newExpNode(AssignK, yylineno, $1, NULL, NULL);
                    $$->attr.op = DEC;
                }
                | simpleExp
                {
                    $$ = $1;
                }
                ;

assignop        : ASSIGN                { $$ = ASSIGN; }
                | ADDASS                { $$ = ADDASS; }
                | SUBASS                { $$ = SUBASS; }
                | MULASS                { $$ = MULASS; }
                | DIVASS                { $$ = DIVASS; }
                ;

simpleExp       : simpleExp OR andExp
                {
                    $$ = newExpNode(OpK, @2.first_line, $1, $3, NULL);
                    $$->attr.op = OR;
                }
                | andExp
                {
                    $$ = $1;
                }
                ;

andExp          : andExp AND unaryRelExp
                {
                    $$ = newExpNode(OpK, @2.first_line, $1, $3, NULL);
                    $$->attr.op = AND;
                }
                | unaryRelExp
                {
                    $$ = $1;
                }
                ;

unaryRelExp     : NOT unaryRelExp
                {
                    $$ = newExpNode(OpK, yylineno, $2, NULL, NULL);
                    $$->attr.op = NOT;
                }
                | relExp
                {
                    $$ = $1;
                }
                ;

relExp          : sumExp relop sumExp
                {
                    $$ = newExpNode(OpK, @2.first_line, $1, $3, NULL);
                    $$->attr.op = $2;
                }
                | sumExp
                {
                    $$ = $1;
                }
                ;

relop           : LT                    { $$ = LT; }
                | LE                    { $$ = LE; }
                | GT                    { $$ = GT; }
                | GE                    { $$ = GE; }
                | EQ                    { $$ = EQ; }
                | NEQ                   { $$ = NEQ; }
                ;

sumExp          : sumExp sumop mulExp
                {
                    $$ = newExpNode(OpK, @2.first_line, $1, $3, NULL);
                    $$->attr.op = $2;
                }
                | mulExp
                {
                    $$ = $1;
                }
                ;

sumop           : '+'                   { $$ = '+'; }
                | '-'                   { $$ = '-'; }
                ;

mulExp          : mulExp mulop unaryExp
                {
                    $$ = newExpNode(OpK, @2.first_line, $1, $3, NULL);
                    $$->attr.op = $2;
                }
                | unaryExp
                {
                    $$ = $1;
                }
                ;

mulop           : '*'               { $$ = '*'; }
                | '/'               { $$ = '/'; }
                | '%'               { $$ = '%'; }
                ;

unaryExp        : unaryop unaryExp
                {
                    $$ = newExpNode(OpK, yylineno, NULL, NULL, NULL);
                    $$->attr.op = $1;
                    $$->child[0] = $2;
                }
                | factor
                {
                    $$ = $1;
                }
                ;

unaryop         : '-'               { $$ = OP_CHSIGN; } // Use the constant for chsign
                | '*'               { $$ = '*'; }
                | '?'               { $$ = '?'; }
                ;

factor          : mutable
                {
                    $$ = $1;
                }
                | immutable
                {
                    $$ = $1;
                }
                ;

mutable         : ID
                {
                    $$ = newExpNode(IdK, yylineno, NULL, NULL, NULL);
                    $$->attr.name = strdup($1->tokenstr);
                }
                | ID '[' exp ']'
                {
                    // Create an OpK node for array subscripting.
                    TreeNode *idNode = newExpNode(IdK, @1.first_line, NULL, NULL, NULL);
                    idNode->attr.name = strdup($1->tokenstr);
                    $$ = newExpNode(OpK, @2.first_line, idNode, $3, NULL);
                    $$->attr.op = '[';
                }
                ;

immutable       : '(' exp ')'
                {
                    $$ = $2;
                }
                | call
                {
                    $$ = $1;
                }
                | constant
                {
                    $$ = $1;
                }
                ;

call            : ID '(' args ')'
                {
                    $$ = newExpNode(CallK, yylineno, $3, NULL, NULL);
                    $$->attr.name = strdup($1->tokenstr);
                }
                ;

args            : argList
                {
                    $$ = $1;
                }
                | /* empty */
                {
                    $$ = NULL;
                }
                ;

argList         : argList ',' exp
                {
                    $$ = addSibling($1, $3);
                }
                | exp
                {
                    $$ = $1;
                }
                ;

constant        : NUMCONST
                {
                    $$ = newExpNode(ConstantK, yylineno, NULL, NULL, NULL);
                    $$->attr.value = $1->numValue;
                    $$->expType = Integer;
                }
                | CHARCONST
                {
                    $$ = newExpNode(ConstantK, yylineno, NULL, NULL, NULL);
                    $$->attr.cvalue = $1->charValue;
                    $$->expType = Char;
                }
                | STRINGCONST
                {
                    $$ = newExpNode(ConstantK, yylineno, NULL, NULL, NULL);
                    $$->attr.string = strdup($1->stringValue);
                    $$->expType = CharInt;
                }
                | BOOLCONST
                {
                    $$ = newExpNode(ConstantK, yylineno, NULL, NULL, NULL);
                    $$->attr.value = $1->boolValue;
                    $$->expType = Boolean;
                }
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
    if (yyparse() == 0) { // A return value of 0 indicates success
        if (savedTree != NULL) {
            printTree(stdout, savedTree);
        }
    }
    return 0;
}

