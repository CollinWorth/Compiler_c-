#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "globals.h"
#include "scanType.h"
#include "c-.tab.hpp"

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

// Separate helper functions

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

static const char* typeToString(ExpType type) {
    switch (type) {
        case Void: return "void";
        case Integer: return "int";
        case Boolean: return "bool";
        case Char: return "char";
        case CharInt: return "char";
        case Equal: return "equal";
        default: return "undefined";
    }
}

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

// Separated type descriptor building
static void printTypeDescriptor(FILE *out, TreeNode *node) {
    if (!printWithTypeInfo) return;
    
    const char* typeStr = typeToString(node->expType);
    
    // Handle arrays
    if (node->isArray) {
        fprintf(out, "is array ");
    }
    
    // Different format based on node kind
    if (node->nodekind == DeclK && node->subkind.decl == FuncK) {
        fprintf(out, "returns type %s", typeStr);
    } else {
        fprintf(out, "of type %s", typeStr);
    }
}

static void printNodeContent(FILE *out, TreeNode *node) {
    switch (node->nodekind) {
        case DeclK:
            switch (node->subkind.decl) {
                case FuncK:  fprintf(out, "Func: %s ", node->attr.name); break;
                case VarK:   fprintf(out, "Var: %s ", node->attr.name); break;
                case ParamK: fprintf(out, "Parm: %s ", node->attr.name); break;
            }
            printTypeDescriptor(out, node);
            break;
            
        case StmtK:
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
            break;
            
        case ExpK:
            switch (node->subkind.exp) {
                case OpK:
                    fprintf(out, "Op: ");
                    printOp(out, node->attr.op);
                    break;
                case ConstantK:
                    if (node->expType == Boolean) {
                        fprintf(out, "Const %s", node->attr.value ? "true" : "false");
                    } else if (node->expType == Char) {
                        fprintf(out, "Const '%c'", node->attr.cvalue);
                    } else if (node->expType == CharInt) {
                        fprintf(out, "Const \"%s\"", node->attr.string);
                    } else {
                        fprintf(out, "Const %d", node->attr.value);
                    }
                    if (printWithTypeInfo) {
                        fprintf(out, " ");
                        printTypeDescriptor(out, node);
                    }
                    return; // Early return to avoid double type printing
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
            if (printWithTypeInfo && node->subkind.exp != ConstantK) {
                fprintf(out, " ");
                printTypeDescriptor(out, node);
            }
            break;
            
        default:
            fprintf(out, "Unknown node");
            break;
    }
}

static void printTreeRecursive(FILE *out, TreeNode *tree, int depth, int siblingNum, bool isChild) {
    if (!tree) return;
    if (tree->nodekind == StmtK && tree->subkind.stmt == NullK) return;

    printIndent(out, depth);
    printLabel(siblingNum, isChild);
    printNodeContent(out, tree);
    fprintf(out, " [line: %d]\n", tree->lineno);

    // Print children
    for (int i = 0; i < MAXCHILDREN; i++) {
        if (tree->child[i]) {
            printTreeRecursive(out, tree->child[i], depth + 1, i, true);
        }
    }

    // Print siblings - they always count from 1, 2, 3...
    if (tree->sibling) {
        int nextSiblingNum = isChild ? 1 : siblingNum + 1;
        printTreeRecursive(out, tree->sibling, depth, nextSiblingNum, false);
    }
}

void printTree(FILE *out, TreeNode *tree) {
    if (!tree) return;
    printTreeRecursive(out, tree, 0, 0, false);
}