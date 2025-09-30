#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "scanType.h"
#include "c-.tab.hpp"

// Private helper function to create a new node and initialize common fields.
// This reduces code duplication in the public new...Node functions.
static TreeNode *newNode(NodeKind nodekind, int lineno, TreeNode* c0, TreeNode* c1, TreeNode* c2) {
    TreeNode *t = new TreeNode;
    if (t == NULL) {
        fprintf(stderr, "ERROR(SYSTEM): Out of memory\n");
        exit(1);
    }

    for (int i = 0; i < MAXCHILDREN; i++) {
        t->child[i] = NULL;
    }
    t->sibling = NULL;
    t->nodekind = nodekind;
    t->lineno = lineno;
    t->child[0] = c0;
    t->child[1] = c1;
    t->child[2] = c2;
    t->isArray = false;
    t->isStatic = false;
    t->expType = UndefinedType;
    return t;
}

TreeNode *newDeclNode(DeclKind kind, int lineno, TreeNode* c0, TreeNode* c1, TreeNode* c2) {
    TreeNode *t = newNode(DeclK, lineno, c0, c1, c2);
    t->subkind.decl = kind;
    return t;
}

TreeNode *newStmtNode(StmtKind kind, int lineno, TreeNode* c0, TreeNode* c1, TreeNode* c2) {
    TreeNode *t = newNode(StmtK, lineno, c0, c1, c2);
    t->subkind.stmt = kind;
    return t;
}

TreeNode *newExpNode(ExpKind kind, int lineno, TreeNode* c0, TreeNode* c1, TreeNode* c2) {
    TreeNode *t = newNode(ExpK, lineno, c0, c1, c2);
    t->subkind.exp = kind;
    return t;
}

// Adds a new sibling 's' to the end of the sibling list of 't'.
TreeNode *addSibling(TreeNode *t, TreeNode *s) {
    if (s == NULL) {
        // It's not an error to try to add a NULL sibling, just do nothing.
        return t;
    }
    if (t == NULL) {
        // If the original list is empty, the new sibling is the whole list.
        return s;
    }

    // Traverse to the end of the sibling list.
    TreeNode *tmp = t;
    while (tmp->sibling != NULL) {
        tmp = tmp->sibling;
    }
    // Append the new sibling.
    tmp->sibling = s;
    return t; // Return the head of the list.
}

//
// NEW AND CORRECTED PRINTING LOGIC
//

// Helper to print operator names
static void printOp(FILE *out, int op) {
    switch (op) {
        case OR: fprintf(out, "or"); break;
        case AND: fprintf(out, "and"); break;
        case NOT: fprintf(out, "not"); break;
        case EQ: fprintf(out, "="); break;
        case NEQ: fprintf(out, "!="); break;
        case LT: fprintf(out, "<"); break;
        case LE: fprintf(out, "<="); break;
        case GT: fprintf(out, ">"); break;
        case GE: fprintf(out, ">="); break;
        case ASSIGN: fprintf(out, ":="); break;
        case INC: fprintf(out, "++"); break;
        case DEC: fprintf(out, "--"); break;
        case ADDASS: fprintf(out, "+="); break;
        case SUBASS: fprintf(out, "-="); break;
        case MULASS: fprintf(out, "*="); break;
        case DIVASS: fprintf(out, "/="); break;
        case OP_CHSIGN: fprintf(out, "chsign"); break;
        case OP_SIZEOF: fprintf(out, "sizeof"); break;
        default: fprintf(out, "%c", op); break;
    }
}

// This is the recursive printing function.
static void printTreeRecursive(FILE *out, TreeNode *tree, int indent, int siblingIndex, bool isChild) {
    if (tree == NULL) return;

    // If the statement is a NullK, don't print it.
    if (tree->nodekind == StmtK && tree->subkind.stmt == NullK) {
        return;
    }

    // Indentation
    for (int i = 0; i < indent; i++) {
        fprintf(out, ".   ");
    }

    if(isChild == true){
        printf("Child: %d  ", siblingIndex);
        if(siblingIndex == 1){
            siblingIndex = 0;
        }
    }else{
        if(siblingIndex != 0){
            printf("Sibling: %d  ", siblingIndex);
        }
    }
 

    // Node-specific printing
    switch (tree->nodekind) {
        case DeclK:
            switch (tree->subkind.decl) {
                case FuncK:  fprintf(out, "Func: %s ", tree->attr.name ? tree->attr.name : "(null)"); break;
                case VarK:   fprintf(out, "Var: %s ", tree->attr.name ? tree->attr.name : "(null)"); break;
                case ParamK: fprintf(out, "Parm: %s ",tree->attr.name ? tree->attr.name : "(null)"); break;
            }
            break;
        case StmtK:
            switch (tree->subkind.stmt) {
                case CompoundK: fprintf(out, "Compound"); break;
                case IfK:       fprintf(out, "If"); break;
                case WhileK:    fprintf(out, "While"); break;
                case ForK:      fprintf(out, "For"); break;
                case RangeK:    fprintf(out, "Range"); break;
                case ReturnK:   fprintf(out, "Return"); break;
                case BreakK:    fprintf(out, "Break"); break;
                case NullK:     fprintf(out, "Null"); break;
                default:        fprintf(out, "Unknown Stmt"); break;
            }
            break;
        case ExpK:
            switch (tree->subkind.exp) {
                case OpK:       fprintf(out, "Op: "); printOp(out, tree->attr.op); break;
                case ConstantK:
                    if (tree->expType == Boolean) {
                        fprintf(out, "Const %s", tree->attr.value ? "true" : "false");
                    } else if (tree->expType == Char) {
                        // Print character constants with single quotes
                        fprintf(out, "Const '%c'", tree->attr.cvalue);
                    } else if (tree->expType == CharInt) { // For string literals
                        fprintf(out, "Const \"%s\"", tree->attr.string);
                    } else { // Default to integer
                        fprintf(out, "Const %d", tree->attr.value);
                    }
                    break;
                case IdK:     fprintf(out, "Id: %s", tree->attr.name); break;
                case AssignK: fprintf(out, "Assign: "); printOp(out, tree->attr.op); break;
                case CallK:   fprintf(out, "Call: %s", tree->attr.name); break;
                default:      fprintf(out, "Unknown Exp"); break;
            }
            break;
        default:
            fprintf(out, "Unknown node kind");
            break;
    }
    fprintf(out, " [line: %d]\n", tree->lineno);

    // --- Recurse on children (one level deeper) ---
    //int childNum = 1;
    for (int childIndex = 0; childIndex < MAXCHILDREN; childIndex++) {
        printTreeRecursive(out, tree->child[childIndex], indent + 1, childIndex, true);
    }

    // --- Recurse on siblings (same level) ---
    // This must be done *after* the node and all its children are printed.
    printTreeRecursive(out, tree->sibling, indent, siblingIndex + 1, false);
}

void printTree(FILE *out, TreeNode *tree) {
    if (tree == NULL) return;
    printTreeRecursive(out, tree, 0, 0, false);
}