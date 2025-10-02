#include "semantic.h"
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "c-.tab.hpp"

static void traverse(TreeNode *t, SymbolTable *st);
static void insertNode(TreeNode *t, SymbolTable *st);
static void checkNode(TreeNode *t, SymbolTable *st);
static void checkUse(TreeNode *t, SymbolTable *st);

void semanticError(TreeNode *t, const char *message) {
    fprintf(stderr, "ERROR(%d): %s\n", t->lineno, message);
    numErrors++;
}

void semanticWarning(TreeNode *t, const char *message) {
    if (numWarnings < MAX_WARNINGS) {
        warnings[numWarnings].lineno = t->lineno;
        strncpy(warnings[numWarnings].message, message, 255);
        warnings[numWarnings].message[255] = '\0';
        numWarnings++;
    }
}

void semanticAnalysis(TreeNode *t, SymbolTable *st) {
    numErrors = numWarnings = 0;
    traverse(t, st);
    
    for (int i = 0; i < numWarnings; i++)
        printf("WARNING(%d): %s\n", warnings[i].lineno, warnings[i].message);
    
    printf("Number of warnings: %d\n", numWarnings);
    printf("Number of errors: %d\n", numErrors);
}

static void traverse(TreeNode *t, SymbolTable *st) {
    if (!t) return;
    
    insertNode(t, st);
    for (int i = 0; i < MAXCHILDREN; i++)
        traverse(t->child[i], st);
    checkNode(t, st);
    traverse(t->sibling, st);
}

static void checkUse(TreeNode *t, SymbolTable *st) {
    if (t->nodekind != ExpK || (t->subkind.exp != IdK && t->subkind.exp != CallK))
        return;
    
    TreeNode *declNode = (TreeNode *)st->lookup(t->attr.name);
    if (!declNode) {
        char buf[256];
        snprintf(buf, sizeof(buf), "Symbol '%s' is not declared.", t->attr.name);
        semanticError(t, buf);
    } else {
        declNode->isUsed = true;
        t->expType = declNode->expType;
        t->isArray = declNode->isArray;
        
        // Check if variable is being used before initialization
        if (!declNode->isInitialized && (t->subkind.exp == IdK)) {
            char buf[256];
            snprintf(buf, sizeof(buf), "Variable '%s' may be uninitialized when used here.", t->attr.name);
            semanticWarning(t, buf);
        }
    }
}

static void insertNode(TreeNode *t, SymbolTable *st) {
    if (!t) return;
    
    if (t->nodekind == StmtK && t->subkind.stmt == CompoundK) {
        st->enter("compound");
    }
    else if (t->nodekind == DeclK) {
        char buf[256];
        bool inserted = st->insert(t->attr.name, t);
        
        if (t->subkind.decl == FuncK) {
            if (!inserted)
                snprintf(buf, sizeof(buf), "Function '%s' is already declared in this scope.", t->attr.name);
            else {
                st->enter(t->attr.name);
                return;
            }
        } else { // VarK or ParamK
            if (!inserted) {
                snprintf(buf, sizeof(buf), "Symbol '%s' is already declared in this scope.", t->attr.name);
            } else {
                TreeNode *outerDecl = (TreeNode *)st->lookupGlobal(t->attr.name);
                if (outerDecl && outerDecl != t) {
                    snprintf(buf, sizeof(buf), "Symbol '%s' shadows a variable from an outer scope.", t->attr.name);
                    semanticWarning(t, buf);
                }
                // Parameters are considered initialized
                if (t->subkind.decl == ParamK) {
                    t->isInitialized = true;
                }
                // Variables with initializers are initialized
                if (t->child[0] != NULL) {
                    t->isInitialized = true;
                }
                return;
            }
        }
        semanticError(t, buf);
    }
    else if (t->nodekind == ExpK) {
        checkUse(t, st);
    }
}

static void checkNode(TreeNode *t, SymbolTable *st) {
    if (!t) return;
    
    if (t->nodekind == ExpK && t->subkind.exp == OpK) {
        if (t->child[0] && t->child[1]) {
            ExpType t1 = t->child[0]->expType, t2 = t->child[1]->expType;
            
            // Array subscript operator
            if (t->attr.op == '[') {
                // Result type is the element type of the array
                t->expType = t->child[0]->expType;
                t->isArray = false; // subscript result is not an array
            }
            else if (t->attr.op == AND || t->attr.op == OR) {
                if (t1 != Boolean || t2 != Boolean)
                    semanticError(t, "Boolean operator applied to non-boolean operands.");
                t->expType = Boolean;
            }
            else if (t->attr.op == NEQ) { // ><
                t->expType = Boolean;
            }
            else if (t->attr.op >= EQ && t->attr.op <= GE) {
                if (t1 != Integer || t2 != Integer)
                    semanticError(t, "Relational operator applied to non-integer operands.");
                t->expType = Boolean;
            }
            else {
                if (t1 != Integer || t2 != Integer)
                    semanticError(t, "Arithmetic operator applied to non-integer operands.");
                t->expType = Integer;
            }
        }
        else if (t->child[0]) {
            ExpType t1 = t->child[0]->expType;
            if (t->attr.op == NOT) {
                if (t1 != Boolean) semanticError(t, "Boolean operator 'not' applied to a non-boolean operand.");
                t->expType = Boolean;
            } else if (t->attr.op == OP_CHSIGN) {
                if (t1 != Integer) semanticError(t, "Unary operator '-' applied to a non-integer operand.");
                t->expType = Integer;
            } else if (t->attr.op == OP_SIZEOF) {
                if (!t->child[0]->isArray) semanticError(t, "Operator 'sizeof' applied to a non-array operand.");
                t->expType = Integer;
            } else if (t->attr.op == '?') {
                t->expType = Integer;
            }
        }
    }
    else if (t->nodekind == ExpK && t->subkind.exp == AssignK) {
        // Assignments should have the type of the left side
        if (t->child[0]) {
            t->expType = t->child[0]->expType;
            
            // Mark left-hand side as initialized
            if (t->child[0]->nodekind == ExpK && t->child[0]->subkind.exp == IdK) {
                TreeNode *declNode = (TreeNode *)st->lookup(t->child[0]->attr.name);
                if (declNode) {
                    declNode->isInitialized = true;
                }
            }
            // Also handle array subscript assignments
            else if (t->child[0]->nodekind == ExpK && t->child[0]->subkind.exp == OpK && t->child[0]->attr.op == '[') {
                if (t->child[0]->child[0] && t->child[0]->child[0]->nodekind == ExpK && 
                    t->child[0]->child[0]->subkind.exp == IdK) {
                    TreeNode *declNode = (TreeNode *)st->lookup(t->child[0]->child[0]->attr.name);
                    if (declNode) {
                        declNode->isInitialized = true;
                    }
                }
            }
            
            // Type checking
            if (t->child[1] && t->child[0]->expType != t->child[1]->expType)
                semanticWarning(t, "Type mismatch in assignment.");
        }
    }
    else if (t->nodekind == DeclK) {
        if ((t->subkind.decl == VarK || t->subkind.decl == ParamK) && !t->isUsed) {
            char buf[256];
            snprintf(buf, sizeof(buf), "The variable '%s' seems not to be used.", t->attr.name);
            semanticWarning(t, buf);
        }
        if (t->subkind.decl == FuncK) st->leave();
    }
    else if (t->nodekind == StmtK && t->subkind.stmt == CompoundK) {
        st->leave();
    }
}