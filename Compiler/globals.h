#ifndef GLOBALS_H
#define GLOBALS_H
#define MAX_WARNINGS 100

#include "tree.h"
#include "symbolTable.h"
#include "scanType.h" // Provides TokenData definition

// Global variables defined in main.cpp
extern int numErrors;
extern int numWarnings;
extern TreeNode *syntaxTree;
extern SymbolTable *symbolTable;
extern bool traceParser;
extern bool traceSymbolTable;
extern bool printAST;
extern bool printWithTypeInfo;
extern bool printSource;

struct SemanticMessage {
    int lineno;
    char message[256];
};

extern SemanticMessage warnings[MAX_WARNINGS];

#endif