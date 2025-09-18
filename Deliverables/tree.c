#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

TreeNode *newDeclNode(DeclKind kind, int lineno, TreeNode* c0, TreeNode* c1, TreeNode* c2) {
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    for (int i=0; i<MAXCHILDREN; i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = DeclK;
    t->subkind.decl = kind;
    t->lineno = lineno;
    t->child[0] = c0;
    t->child[1] = c1;
    t->child[2] = c2;
    t->isArray = false;
    t->isStatic = false;
    t->expType = UndefinedType;
    return t;
}

TreeNode *newStmtNode(StmtKind kind, int lineno, TreeNode* c0, TreeNode* c1, TreeNode* c2) {
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    for (int i=0; i<MAXCHILDREN; i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = StmtK;
    t->subkind.stmt = kind;
    t->lineno = lineno;
    t->child[0] = c0;
    t->child[1] = c1;
    t->child[2] = c2;
    t->isArray = false;
    t->isStatic = false;
    t->expType = UndefinedType;
    return t;
}

TreeNode *newExpNode(ExpKind kind, int lineno, TreeNode* c0, TreeNode* c1, TreeNode* c2) {
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    for (int i=0; i<MAXCHILDREN; i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ExpK;
    t->subkind.exp = kind;
    t->lineno = lineno;
    t->child[0] = c0;
    t->child[1] = c1;
    t->child[2] = c2;
    t->isArray = false;
    t->isStatic = false;
    t->expType = UndefinedType;
    return t;
}

TreeNode *addSibling(TreeNode *t, TreeNode *s) {
    if (!s) {
        fprintf(stderr, "ERROR(SYSTEM): never add a NULL to a sibling list.\n");
        exit(1);
    }
    if (t != NULL) {
        TreeNode *tmp = t;
        while (tmp->sibling != NULL) tmp = tmp->sibling;
        tmp->sibling = s;
        return t;
    }
    return s;
}

void printTree(TreeNode *tree) {
    if (!tree) return;
    switch (tree->nodekind) {
        case DeclK: printf("Decl Node: %d [line %d]\n", tree->subkind.decl, tree->lineno); break;
        case StmtK: printf("Stmt Node: %d [line %d]\n", tree->subkind.stmt, tree->lineno); break;
        case ExpK:  printf("Exp Node: %d [line %d]\n", tree->subkind.exp, tree->lineno); break;
    }
    for (int i=0; i<MAXCHILDREN; i++) if (tree->child[i]) printTree(tree->child[i]);
    if (tree->sibling) printTree(tree->sibling);
}