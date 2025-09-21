#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "scanType.h"
#include "c-.tab.h"

// Private helper function to create a new node and initialize common fields.
// This reduces code duplication in the public new...Node functions.
static TreeNode *newNode(NodeKind nodekind, int lineno, TreeNode* c0, TreeNode* c1, TreeNode* c2) {
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
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
        case OP_CHSIGN: fprintf(out, "chsign"); break;
        default: fprintf(out, "%c", op); break;
    }
}

// This is the recursive printing function.
static void printTreeRecursive(FILE *out, TreeNode *tree, int indent, int isChild, int childNum) {
    if (tree == NULL) return;

    // Indentation
    for (int i = 0; i < indent; i++) fprintf(out, ".   ");

    // Sibling/Child Prefix
    if (isChild) {
        fprintf(out, "Child: %d  ", childNum);
    } else if (indent > 0) { // It's a sibling
        fprintf(out, "Sibling: %d  ", childNum);
    }

    // Node-specific printing
    switch (tree->nodekind) {
        case DeclK:
            switch (tree->subkind.decl) {
                case FuncK: fprintf(out, "Func: %s", tree->attr.name); break;
                case VarK: fprintf(out, "Var: %s ", tree->attr.name); break;
                case ParamK: fprintf(out, "Param: %s", tree->attr.name); break;
            }
            break;
        case StmtK:
            switch (tree->subkind.stmt) {
                case CompoundK: fprintf(out, "Compound"); break;
                case IfK: fprintf(out, "If"); break;
                default: fprintf(out, "Stmt"); break;
            }
            break;
        case ExpK:
            switch (tree->subkind.exp) {
                case OpK: fprintf(out, "Op: "); printOp(out, tree->attr.op); break;
                case ConstantK:
                    if (tree->expType == Boolean) fprintf(out, "Const %s", tree->attr.value ? "true" : "false");
                    else fprintf(out, "Const %d", tree->attr.value);
                    break;
                case IdK: fprintf(out, "Id: %s", tree->attr.name); break;
                case AssignK: fprintf(out, "Assign: "); printOp(out, tree->attr.op); break;
                case CallK: fprintf(out, "Call: %s", tree->attr.name); break;
                default: fprintf(out, "Exp"); break;
            }
            break;
        default:
            fprintf(out, "Unknown node kind");
            break;
    }
    fprintf(out, "  [line: %d]\n", tree->lineno); // two spaces before [line:] to match expected formatting

    // Recurse on children
    for (int i = 0; i < MAXCHILDREN; i++) {
        printTreeRecursive(out, tree->child[i], indent + 1, 1, i);     // pass 0-based child index
    }

    // After printing the node and all its children, move to its sibling
    // The sibling is NOT a child of the current node, so isChild is 0.
    // The sibling number is incremented from the current node's sibling number.
    if (tree->sibling) {
        // The sibling's number should be 1 greater than the current node's sibling number.
        // If the current node is a child, its sibling number is effectively 0, so its sibling is 1.
        int nextSiblingNum = isChild ? 1 : childNum + 1;
        printTreeRecursive(out, tree->sibling, indent, 0, nextSiblingNum);
    }
}

// Public entry point for printing the tree
void printTree(FILE *out, TreeNode *tree) {
    if (tree == NULL) return;
    // Start with the root node. It is not a child or a sibling.
    printTreeRecursive(out, tree, 0, 0, 0);
}