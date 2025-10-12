#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "globals.h"
#include "scanType.h"
#include "c-.tab.hpp"
#include <cstring>

// ============================================================================
// FORWARD DECLARATIONS
// ============================================================================
static void printTreeRecursive(FILE *out, TreeNode *tree, int depth, int siblingNum, bool isChild);

// ============================================================================
// NODE CREATION FUNCTIONS (unchanged)
// ============================================================================

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
    t->isUsed = false;
    t->isInitialized = false;
    t->isStatic = false;
    t->isLHS = false;
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

TreeNode *addSibling(TreeNode *t, TreeNode *s) {
    if (s == NULL) return t;
    if (t == NULL) return s;

    TreeNode *tmp = t;
    while (tmp->sibling != NULL) {
        tmp = tmp->sibling;
    }
    tmp->sibling = s;
    return t;
}

// ============================================================================
// TYPE AND OPERATOR UTILITIES
// ============================================================================

static const char* typeToString(ExpType type) {
    switch (type) {
        case Void: return "void";
        case Integer: return "int";
        case Boolean: return "bool";
        case Char: return "char";
        case CharInt: return "char";
        case Equal: return "equal";
        default: return "undefined type";
    }
}

static void printOp(FILE *out, int op) {
    switch (op) {
        case OR: fprintf(out, "or"); break;
        case AND: fprintf(out, "and"); break;
        case NOT: fprintf(out, "not"); break;
        case EQ: fprintf(out, "="); break;
        case NEQ: fprintf(out, "><"); break;
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

// ============================================================================
// CONSTANT PRINTING FUNCTIONS
// ============================================================================

static void printStringConstant(FILE *out, TreeNode *node) {
    fprintf(out, "Const");
    if (printWithTypeInfo) {
        fprintf(out, " is array \"%s\" of type char", node->attr.string);
    } else {
        fprintf(out, " \"%s\"", node->attr.string);
    }
}

static void printCharConstant(FILE *out, TreeNode *node) {
    fprintf(out, "Const '%c'", node->attr.cvalue);
    if (printWithTypeInfo) {
        fprintf(out, " of type char");
    }
}

static void printBooleanConstant(FILE *out, TreeNode *node) {
    fprintf(out, "Const %s", node->attr.value ? "true" : "false");
    if (printWithTypeInfo) {
        fprintf(out, " of type bool");
    }
}

static void printNumericConstant(FILE *out, TreeNode *node) {
    fprintf(out, "Const %d", node->attr.value);
    if (printWithTypeInfo) {
        fprintf(out, " of type %s", typeToString(node->expType));
    }
}

static void printConstant(FILE *out, TreeNode *node) {
    switch (node->expType) {
        case CharInt:
            printStringConstant(out, node);
            break;
        case Char:
            printCharConstant(out, node);
            break;
        case Boolean:
            printBooleanConstant(out, node);
            break;
        default:
            printNumericConstant(out, node);
            break;
    }
}

// ============================================================================
// TYPE DESCRIPTOR FUNCTIONS
// ============================================================================

static bool shouldShowArrayPrefix(TreeNode *node) {
    return node->isArray && 
           ((node->nodekind == DeclK && node->subkind.decl == VarK) ||
            (node->nodekind == ExpK && node->subkind.exp == OpK));
}

static bool isFunctionDeclaration(TreeNode *node) {
    return node->nodekind == DeclK && node->subkind.decl == FuncK;
}

static bool isUndefinedType(TreeNode *node) {
    return strcmp(typeToString(node->expType), "undefined type") == 0;
}

static void printTypeDescriptor(FILE *out, TreeNode *node) {
    if (!printWithTypeInfo) return;
    
    // String constants handle their own type info
    if (node->nodekind == ExpK && node->subkind.exp == ConstantK && node->expType == CharInt) {
        return;
    }
    
    const char* typeStr = typeToString(node->expType);
    
    // Print array prefix if needed
    if (shouldShowArrayPrefix(node)) {
        fprintf(out, "is array ");
    }
    
    // Print type descriptor based on node type
    if (isFunctionDeclaration(node)) {
        fprintf(out, "returns type %s", typeStr);
    } else if (isUndefinedType(node)) {
        fprintf(out, "of %s", typeStr);
    } else {
        fprintf(out, "of type %s", typeStr);
    }
}

// ============================================================================
// NODE CONTENT PRINTING FUNCTIONS
// ============================================================================

static void printDeclNode(FILE *out, TreeNode *node) {
    switch (node->subkind.decl) {
        case FuncK:  fprintf(out, "Func: %s ", node->attr.name); break;
        case VarK:   fprintf(out, "Var: %s ", node->attr.name); break;
        case ParamK: fprintf(out, "Parm: %s ", node->attr.name); break;
    }
    printTypeDescriptor(out, node);
}

static void printStmtNode(FILE *out, TreeNode *node) {
    switch (node->subkind.stmt) {
        case CompoundK: fprintf(out, "Compound"); break;
        case IfK:       fprintf(out, "If"); break;
        case WhileK:    fprintf(out, "While"); break;
        case ForK:      fprintf(out, "For"); break;
        case RangeK:    fprintf(out, "Range"); break;
        case ReturnK:   fprintf(out, "Return"); break;
        case BreakK:    fprintf(out, "Break"); break;
        default:        fprintf(out, "Unknown Stmt"); break;
    }
}

static void printExpNode(FILE *out, TreeNode *node) {
    switch (node->subkind.exp) {
        case OpK:
            fprintf(out, "Op: ");
            printOp(out, node->attr.op);
            break;
        case ConstantK:
            printConstant(out, node);
            return; // Constants handle their own type info
        case IdK:
            fprintf(out, "Id: %s", node->attr.name);
            break;
        case AssignK:
            fprintf(out, "Assign: ");
            printOp(out, node->attr.op);
            break;
        case CallK:
            fprintf(out, "Call: %s", node->attr.name);
            break;
        default:
            fprintf(out, "Unknown Exp");
            break;
    }
    
    // Add type info for non-constant expressions
    if (printWithTypeInfo) {
        fprintf(out, " ");
        printTypeDescriptor(out, node);
    }
}

static void printNodeContent(FILE *out, TreeNode *node) {
    switch (node->nodekind) {
        case DeclK:
            printDeclNode(out, node);
            break;
        case StmtK:
            printStmtNode(out, node);
            break;
        case ExpK:
            printExpNode(out, node);
            break;
        default:
            fprintf(out, "Unknown node");
            break;
    }
}

// ============================================================================
// TREE PRINTING UTILITIES
// ============================================================================

static void printIndent(FILE *out, int depth) {
    for (int i = 0; i < depth; i++) {
        fprintf(out, ".   ");
    }
}

static void printLabel(int siblingNum, bool isChild) {
    if (isChild) {
        fprintf(stdout, "Child: %d  ", siblingNum);
    } else if (siblingNum > 0) {
        fprintf(stdout, "Sibling: %d  ", siblingNum);
    }
}

static bool shouldSkipNode(TreeNode *tree) {
    return tree->nodekind == StmtK && tree->subkind.stmt == NullK;
}

static void printChildren(FILE *out, TreeNode *tree, int depth) {
    for (int i = 0; i < MAXCHILDREN; i++) {
        if (tree->child[i]) {
            printTreeRecursive(out, tree->child[i], depth + 1, i, true);
        }
    }
}

static void printSiblings(FILE *out, TreeNode *tree, int depth, int siblingNum, bool isChild) {
    if (tree->sibling) {
        int nextSiblingNum = isChild ? 1 : siblingNum + 1;
        printTreeRecursive(out, tree->sibling, depth, nextSiblingNum, false);
    }
}

// ============================================================================
// MAIN TREE PRINTING FUNCTION
// ============================================================================

static void printTreeRecursive(FILE *out, TreeNode *tree, int depth, int siblingNum, bool isChild) {
    if (!tree || shouldSkipNode(tree)) return;

    // Print current node
    printIndent(out, depth);
    printLabel(siblingNum, isChild);
    printNodeContent(out, tree);
    fprintf(out, " [line: %d]\n", tree->lineno);

    // Print children and siblings
    printChildren(out, tree, depth);
    printSiblings(out, tree, depth, siblingNum, isChild);
}

void printTree(FILE *out, TreeNode *tree) {
    if (!tree) return;
    printTreeRecursive(out, tree, 0, 0, false);
}