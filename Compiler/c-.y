%{
#include <stdio.h>
#include <stdlib.h>
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
%}

%union {
    TreeNode *tree;
    TokenData *tokenData;
    int op;
    ExpType type;   // Add this field for ExpType enum
}

%token <tokenData> ID NUMCONST CHARCONST STRINGCONST ERROR BOOLCONST
%token <tokenData> BOOL CHAR INT VOID
%token <tokenData> STATIC
%token <tokenData> IF THEN ELSE WHILE DO FOR RETURN BREAK
%token <tokenData> OR AND NOT
%token <tokenData> ASSIGN ADDASS SUBASS MULASS DIVASS  
%token <tokenData> EQ NE LT LE GT GE NEQ               
%token <tokenData> INC DEC 
%token <tokenData> TO BY
// Add tokenData type to punctuation/operators used for line numbers
%token <tokenData> '{' '}' '(' ')' '[' ']'
%token <tokenData> '+' '-' '*' '/' '%'

%type <tree> program declList decl varDecl scopedVarDecl varDeclList varDeclInit varDeclId funDecl parms parmList parmTypeList parmIdList parmId stmt expStmt compoundStmt localDecls stmtList selectStmt iterStmt iterRange returnStmt breakStmt exp simpleExp andExp unaryRelExp relExp sumExp mulExp unaryExp factor mutable immutable call args argList constant
%type <op> assignop relop sumop mulop unaryop
%type <type> typeSpec // Change typeSpec to use the new 'type' field

%nonassoc "then"
%nonassoc ELSE

%debug
%start program

%%


program         : declList
                {
                    //$$ = newDeclNode(ProgramK, 0, $1, NULL, NULL);
                    syntaxTree = $1;   /* root of the AST */
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
                    TreeNode * list = $2; // This is a list of IdK or InitK nodes
                    TreeNode * head = NULL;

                    while (list != NULL) {
                        TreeNode * varNode;
                        // If it's an initialization, the name is in the child.
                        if (list->subkind.exp == InitK) {
                            // Create a VarK node. Its child should be the initialization
                            // expression from the InitK node (list->child[1]), not the InitK node itself.
                            varNode = newDeclNode(VarK, list->lineno, list->child[1], NULL, NULL);
                            varNode->attr.name = strdup(list->child[0]->attr.name);
                            // The IdK node (list->child[0]) is now processed, so we can free it
                            // to prevent memory leaks. The InitK node (list) is also processed.
                            varNode->isArray = list->child[0]->isArray;
                        } 
                        // Otherwise, it's a simple ID declaration.
                        else { // IdK
                            // For a simple declaration, the VarK node is a leaf.
                            // It holds the name and type; it doesn't need a child.
                            varNode = newDeclNode(VarK, list->lineno, NULL, NULL, NULL);
                            varNode->attr.name = strdup(list->attr.name);
                            //varNode->isArray = list->isArray;
                        }

                        varNode->expType = $1;
                        head = addSibling(head, varNode);
                        
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
                        TreeNode * varNode;
                        if (list->subkind.exp == InitK) {
                            varNode = newDeclNode(VarK, list->lineno, list->child[1], NULL, NULL);
                            varNode->attr.name = strdup(list->child[0]->attr.name);
                            varNode->isArray = list->child[0]->isArray;
                        } else { // IdK
                            varNode = newDeclNode(VarK, list->lineno, NULL, NULL, NULL);
                            varNode->attr.name = strdup(list->attr.name);
                            //varNode->isArray = list->isArray;
                        }
                        varNode->isStatic = true; // Set static flag
                        varNode->expType = $2;    // Capture type.
                        
                        head = addSibling(head, varNode);
                        list = list->sibling;
                    }
                    $$ = head;
                }
                | typeSpec varDeclList ';'
                {
                    // This is a local variable declaration.
                    TreeNode * list = $2;
                    TreeNode * head = NULL;
                    while (list != NULL) {
                        TreeNode * varNode;
                        if (list->subkind.exp == InitK) {
                            varNode = newDeclNode(VarK, list->lineno, list->child[1], NULL, NULL);
                            varNode->attr.name = strdup(list->child[0]->attr.name);
                            varNode->isArray = list->child[0]->isArray;
                        } else { // IdK
                            varNode = newDeclNode(VarK, list->lineno, NULL, NULL, NULL);
                            varNode->attr.name = strdup(list->attr.name);
                            //varNode->isArray = list->isArray;
                        }

                        varNode->expType = $1; // Capture type.
                        
                        head = addSibling(head, varNode);
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
                    if ($1->attr.name != NULL) {
                        $$->attr.name = strdup($1->attr.name);
                    }
                }
                ;

varDeclId       : ID
                {
                    $$ = newExpNode(IdK, $1->linenum, NULL, NULL, NULL);
                    $$->attr.name = new char[strlen($1->tokenstr) + 1];
                    strcpy($$->attr.name, $1->tokenstr);
                    $$->isArray = false;
                }
                | ID '[' NUMCONST ']'
                {
                    TreeNode *sizeNode = newExpNode(ConstantK, $3->linenum, NULL, NULL, NULL);
                    sizeNode->attr.value = $3->numValue;
                    sizeNode->expType = Integer;
                    $$ = newExpNode(IdK, $1->linenum, sizeNode, NULL, NULL);
                    $$->attr.name = new char[strlen($1->tokenstr) + 1];
                    strcpy($$->attr.name, $1->tokenstr);
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
                    $$ = newDeclNode(FuncK, $2->linenum, $4, $6, NULL);
                    $$->attr.name = new char[strlen($2->tokenstr) + 1];
                    strcpy($$->attr.name, $2->tokenstr);
                    $$->expType = $1; // Capture type from typeSpec. $1 is now an ExpType.
                }
                | ID '(' parms ')' compoundStmt
                {
                    // Correctly assign parms to child[0] and body to child[1]
                    $$ = newDeclNode(FuncK, $1->linenum, $3, $5, NULL);
                    $$->attr.name = new char[strlen($1->tokenstr) + 1];
                    strcpy($$->attr.name, $1->tokenstr);
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
                        newNode->attr.name = new char[strlen(list->attr.name) + 1];
                        strcpy(newNode->attr.name, list->attr.name);
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
                    $$ = newExpNode(IdK, $1->linenum, NULL, NULL, NULL);
                    $$->attr.name = new char[strlen($1->tokenstr) + 1];
                    strcpy($$->attr.name, $1->tokenstr);
                }
                | ID '[' ']'
                {
                    $$ = newExpNode(IdK, $1->linenum, NULL, NULL, NULL);
                    $$->attr.name = new char[strlen($1->tokenstr) + 1];
                    strcpy($$->attr.name, $1->tokenstr);
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
                    $$ = newStmtNode(CompoundK, $1->linenum, $2, $3, NULL);
                }
                ;

localDecls      : localDecls scopedVarDecl
                {
                    //printf("found localDecls\n");
                    $$ = addSibling($1, $2);
                }
                | /* empty */
                {
                    $$ = NULL;
                }
                ;

stmtList        : stmtList stmt
                {
                    //printf("found stmtList\n");
                    // Correctly append to the list. addSibling handles the case where $1 is NULL.
                    $$ = addSibling($1, $2);
                }
                | /* empty */
                {
                    //printf("found stmtList\n");
                    $$ = NULL;
                }
                ;

selectStmt      : IF simpleExp THEN stmt %prec "then"
                {
                    //printToken($1, "IF");

                    //if ($1) printf("  $1->linenum = %d\n", $1->linenum);
                    //if ($2) printf("  $2->linenum = %d\n", $2->lineno);
                    //if ($4) printf("  $4->linenum = %d\n", $4->lineno);

                    // If the 'then' part is an empty statement, don't create a node for it.
                    TreeNode *thenStmt = $4;
                    //if (thenStmt != NULL && thenStmt->nodekind == StmtK && thenStmt->subkind.stmt == NullK) {
                    //    thenStmt = NULL; // This will prevent the Null node from being added to the tree
                    //}
                    $$ = newStmtNode(IfK, $1->linenum, $2, thenStmt, NULL);
                }
                | IF simpleExp THEN stmt ELSE stmt
                {
                    //printf("found selectStmt\n");
                    TreeNode *thenStmt = $4;
                    TreeNode *elseStmt = $6;
                    if (thenStmt != NULL && thenStmt->nodekind == StmtK && thenStmt->subkind.stmt == NullK) thenStmt = NULL;
                    if (elseStmt != NULL && elseStmt->nodekind == StmtK && elseStmt->subkind.stmt == NullK) elseStmt = NULL;

                    $$ = newStmtNode(IfK, $1->linenum, $2, thenStmt, elseStmt);
                }
                ;

iterStmt        : WHILE simpleExp DO stmt
                {
                    $$ = newStmtNode(WhileK, $1->linenum, $2, $4, NULL);
                }
                | FOR ID ASSIGN iterRange DO stmt
                {
                    TreeNode *varNode = newDeclNode(VarK, $2->linenum, NULL, NULL, NULL);
                    varNode->attr.name = new char[strlen($2->tokenstr) + 1];
                    strcpy(varNode->attr.name, $2->tokenstr);
                    varNode->expType = Integer; // For loop variables are implicitly integers
                    $$ = newStmtNode(ForK, $1->linenum, varNode, $4, $6);
                }
                ;

iterRange       : simpleExp TO simpleExp
                {
                    $$ = newStmtNode(RangeK, $2->linenum, $1, $3, NULL);
                }
                | simpleExp TO simpleExp BY simpleExp
                {
                    $$ = newStmtNode(RangeK, $2->linenum, $1, $3, $5);
                }
                ;

returnStmt      : RETURN ';'
                {
                    $$ = newStmtNode(ReturnK, $1->linenum, NULL, NULL, NULL);
                }
                | RETURN exp ';'
                {
                    $$ = newStmtNode(ReturnK, $1->linenum, $2, NULL, NULL);
                }
                ;

breakStmt       : BREAK ';'
                {
                    $$ = newStmtNode(BreakK, $1->linenum, NULL, NULL, NULL);
                }
                ;

exp             : mutable assignop exp
                {
                    $$ = newExpNode(AssignK, $1->lineno, $1, $3, NULL); // Use lineno of the mutable
                    $$->attr.op = $2;
                }
                | mutable INC
                {
                    $$ = newExpNode(AssignK, $2->linenum, $1, NULL, NULL);
                    $$->attr.op = INC;
                }
                | mutable DEC
                {
                    $$ = newExpNode(AssignK, $2->linenum, $1, NULL, NULL);
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
                    $$ = newExpNode(OpK, $2->linenum, $1, $3, NULL);
                    $$->attr.op = OR;
                }
                | andExp
                {
                    $$ = $1;
                }
                ;

andExp          : andExp AND unaryRelExp
                {
                    $$ = newExpNode(OpK, $2->linenum, $1, $3, NULL);
                    $$->attr.op = AND;
                }
                | unaryRelExp
                {
                    $$ = $1;
                }
                ;

unaryRelExp     : NOT unaryRelExp
                {
                    $$ = newExpNode(OpK, $1->linenum, $2, NULL, NULL);
                    $$->attr.op = NOT;
                }
                | relExp
                {
                    $$ = $1;
                }
                ;

relExp          : sumExp relop sumExp
                {
                    $$ = newExpNode(OpK, $1->lineno, $1, $3, NULL); // Use lineno of first expression
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
                    $$ = newExpNode(OpK, $1->lineno, $1, $3, NULL); // Use lineno of first expression
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
                    $$ = newExpNode(OpK, $1->lineno, $1, $3, NULL); // Use lineno of first expression
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
                    $$ = newExpNode(OpK, $2->lineno, NULL, NULL, NULL); // Use lineno of expression
                    $$->attr.op = $1;
                    $$->child[0] = $2;
                }
                | factor
                {
                    $$ = $1;
                }
                ;

unaryop         : '-'               { $$ = OP_CHSIGN; } // Use the constant for chsign
                | '*'               { $$ = OP_SIZEOF; }
                | '?'               { $$ = '?'; } // This is the "random" operator
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
                    $$ = newExpNode(IdK, $1->linenum, NULL, NULL, NULL);
                    $$->attr.name = new char[strlen($1->tokenstr) + 1];
                    strcpy($$->attr.name, $1->tokenstr);
                }
                | ID '[' exp ']'
                {
                    // Create an OpK node for array subscripting.
                    TreeNode *idNode = newExpNode(IdK, $1->linenum, NULL, NULL, NULL);
                    idNode->attr.name = new char[strlen($1->tokenstr) + 1];
                    strcpy(idNode->attr.name, $1->tokenstr);
                    $$ = newExpNode(OpK, $2->linenum, idNode, $3, NULL);
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
                    $$ = newExpNode(CallK, $1->linenum, $3, NULL, NULL);
                    $$->attr.name = new char[strlen($1->tokenstr) + 1];
                    strcpy($$->attr.name, $1->tokenstr);
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
                    $$->attr.string = new char[strlen($1->stringValue) + 1];
                    strcpy($$->attr.string, $1->stringValue);
                    $$->expType = CharInt;
                    $$->isArray = true;
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
