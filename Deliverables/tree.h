#ifndef TREE_H
#define TREE_H

#include <stdbool.h>

#define MAXCHILDREN 3

typedef enum {DeclK, StmtK, ExpK} NodeKind;
typedef enum {VarK, FuncK, ParamK, ProgramK} DeclKind;   // added ProgramK
typedef enum {NullK, IfK, WhileK, ForK, CompoundK, ReturnK, BreakK, RangeK} StmtKind;
typedef enum {OpK, ConstantK, IdK, AssignK, InitK, CallK} ExpKind;
typedef enum {Void, Integer, Boolean, Char, CharInt, Equal, UndefinedType} ExpType;

typedef struct treeNode {
    struct treeNode *child[MAXCHILDREN];
    struct treeNode *sibling;
    int lineno;
    NodeKind nodekind;
    union { DeclKind decl; StmtKind stmt; ExpKind exp; } subkind;
    union { int value; char cvalue; char *string; char *name; int op; } attr;
    ExpType expType;
    bool isArray;
    bool isStatic;
} TreeNode;

TreeNode *addSibling(TreeNode *t, TreeNode *s);
TreeNode *newDeclNode(DeclKind kind, int lineno, TreeNode* c0, TreeNode* c1, TreeNode* c2);
TreeNode *newStmtNode(StmtKind kind, int lineno, TreeNode* c0, TreeNode* c1, TreeNode* c2);
TreeNode *newExpNode(ExpKind kind, int lineno, TreeNode* c0, TreeNode* c1, TreeNode* c2);
void printTree(TreeNode *tree);

#endif