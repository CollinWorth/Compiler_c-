#include "semantic.h"
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "c-.tab.hpp"
#include <algorithm>
#include <set>

static void traverse(TreeNode *t, SymbolTable *st, TreeNode *parent = nullptr);
static void insertNode(TreeNode *t, SymbolTable *st, TreeNode *parent = nullptr);
static void checkNode(TreeNode *t, SymbolTable *st, TreeNode *parent = nullptr);
static void checkUse(TreeNode *t, SymbolTable *st, TreeNode *parent = nullptr);
static void checkCall(TreeNode *t, TreeNode *declNode);

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

static const char* opToString(int op) {
    switch (op) {
        case OR: return "or";
        case AND: return "and";
        case NOT: return "not";
        case EQ: return "=";
        case NEQ: return "><";
        case LT: return "<";
        case LE: return "<=";
        case GT: return ">";
        case GE: return ">=";
        case ASSIGN: return ":=";
        case ADDASS: return "+=";
        case SUBASS: return "-=";
        case MULASS: return "*=";
        case DIVASS: return "/=";
        case INC: return "++";
        case DEC: return "--";
        case OP_CHSIGN: return "chsign";
        case OP_SIZEOF: return "sizeof";
        case '?': return "?";
        default: {
            static char buf[2];
            buf[0] = (char)op;
            buf[1] = '\0';
            return buf;
        }
    }
}

void semanticError(TreeNode *t, const char *message) {
    if (numErrors < MAX_WARNINGS) {
        errors[numErrors].lineno = t->lineno;
        strncpy(errors[numErrors].message, message, 511);  // Change to 511
        errors[numErrors].message[511] = '\0';              // Change to 511
        errors[numErrors].isError = true;
        numErrors++;
    }
}

void semanticWarning(TreeNode *t, const char *message) {
    if (numWarnings < MAX_WARNINGS) {
        warnings[numWarnings].lineno = t->lineno;
        strncpy(warnings[numWarnings].message, message, 511);  // Change to 511
        warnings[numWarnings].message[511] = '\0';              // Change to 511
        warnings[numWarnings].isError = false;
        numWarnings++;
    }
}

static std::set<std::string> warnedUninitVars;
static std::vector<TreeNode*> allDeclarations;
static std::vector<TreeNode*> functionStack;

static TreeNode* createIOLibrary() {
    TreeNode* ioFunctions = nullptr;
    
    // Create parameter nodes (reusable dummy parameter)
    TreeNode* intParam = newDeclNode(ParamK, -1, nullptr, nullptr, nullptr);
    intParam->attr.name = strdup("*dummy*");
    intParam->expType = Integer;
    intParam->isArray = false;
    
    TreeNode* boolParam = newDeclNode(ParamK, -1, nullptr, nullptr, nullptr);
    boolParam->attr.name = strdup("*dummy*");
    boolParam->expType = Boolean;
    boolParam->isArray = false;
    
    TreeNode* charParam = newDeclNode(ParamK, -1, nullptr, nullptr, nullptr);
    charParam->attr.name = strdup("*dummy*");
    charParam->expType = Char;
    charParam->isArray = false;
    
    // void output(int)
    TreeNode* output = newDeclNode(FuncK, -1, intParam, nullptr, nullptr);
    output->attr.name = strdup("output");
    output->expType = Void;
    ioFunctions = addSibling(ioFunctions, output);
    
    // void outputb(bool)
    TreeNode* outputb = newDeclNode(FuncK, -1, boolParam, nullptr, nullptr);
    outputb->attr.name = strdup("outputb");
    outputb->expType = Void;
    ioFunctions = addSibling(ioFunctions, outputb);
    
    // void outputc(char)
    TreeNode* outputc = newDeclNode(FuncK, -1, charParam, nullptr, nullptr);
    outputc->attr.name = strdup("outputc");
    outputc->expType = Void;
    ioFunctions = addSibling(ioFunctions, outputc);
    
    // int input()
    TreeNode* input = newDeclNode(FuncK, -1, nullptr, nullptr, nullptr);
    input->attr.name = strdup("input");
    input->expType = Integer;
    ioFunctions = addSibling(ioFunctions, input);
    
    // bool inputb()
    TreeNode* inputb = newDeclNode(FuncK, -1, nullptr, nullptr, nullptr);
    inputb->attr.name = strdup("inputb");
    inputb->expType = Boolean;
    ioFunctions = addSibling(ioFunctions, inputb);
    
    // char inputc()
    TreeNode* inputc = newDeclNode(FuncK, -1, nullptr, nullptr, nullptr);
    inputc->attr.name = strdup("inputc");
    inputc->expType = Char;
    ioFunctions = addSibling(ioFunctions, inputc);
    
    // void outnl()
    TreeNode* outnl = newDeclNode(FuncK, -1, nullptr, nullptr, nullptr);
    outnl->attr.name = strdup("outnl");
    outnl->expType = Void;
    ioFunctions = addSibling(ioFunctions, outnl);
    
    return ioFunctions;
}

void semanticAnalysis(TreeNode *t, SymbolTable *st) {
    numErrors = numWarnings = 0;
    warnedUninitVars.clear();
    allDeclarations.clear();

    // First, load I/O library functions into symbol table
    TreeNode* ioLibrary = createIOLibrary();
    traverse(ioLibrary, st, nullptr);
    
    // Then process the actual program
    traverse(t, st, nullptr);

    // Check for unused variables
    for (TreeNode* decl : allDeclarations) {
        if ((decl->subkind.decl == VarK || decl->subkind.decl == ParamK || decl->subkind.decl == FuncK) && !decl->isUsed) {
            // Skip warnings for I/O library functions and their parameters (line -1)
            if (decl->lineno == -1) {
                continue;
            }
            
            // Skip warnings for main function
            if (decl->subkind.decl == FuncK && strcmp(decl->attr.name, "main") == 0) {
                continue;
            }
            
            bool hasRedeclarationError = false;
            for (int i = 0; i < numErrors; i++) {
                if (errors[i].lineno == decl->lineno && 
                    strstr(errors[i].message, "already declared") != nullptr) {
                    hasRedeclarationError = true;
                    break;
                }
            }
            
            if (!hasRedeclarationError) {
                char buf[256];
                if (decl->subkind.decl == ParamK) {
                    snprintf(buf, sizeof(buf), "The parameter '%s' seems not to be used.", decl->attr.name);
                } else if (decl->subkind.decl == FuncK) {
                    snprintf(buf, sizeof(buf), "The function '%s' seems not to be used.", decl->attr.name);
                } else {
                    snprintf(buf, sizeof(buf), "The variable '%s' seems not to be used.", decl->attr.name);
                }
                semanticWarning(decl, buf);
            }
        }
    }

    // Print regular errors and warnings first
    std::vector<SemanticMessage> allMessages;
    allMessages.insert(allMessages.end(), errors, errors + numErrors);
    allMessages.insert(allMessages.end(), warnings, warnings + numWarnings);

    for (const auto& msg : allMessages) {
        if (msg.isError) {
            printf("ERROR(%d): %s\n", msg.lineno, msg.message);
        } else {
            printf("WARNING(%d): %s\n", msg.lineno, msg.message);
        }
    }

    
    TreeNode *mainNode = (TreeNode *)st->lookupGlobal("main");
    if (!mainNode || mainNode->subkind.decl != FuncK) {
        printf("ERROR(LINKER): A function named 'main' with no parameters must be defined.\n");
        numErrors++; // Still count it for the total
    } else {
        bool hasParameters = (mainNode->child[0] != nullptr);
        if (hasParameters) {
            printf("ERROR(LINKER): A function named 'main' with no parameters must be defined.\n");
            numErrors++; // Still count it for the total
        }
    }
}

static void traverse(TreeNode *t, SymbolTable *st, TreeNode *parent) {
    if (!t) return;
    
    insertNode(t, st, parent);
    
    for (int i = 0; i < MAXCHILDREN; i++) {
        traverse(t->child[i], st, t);
    }
    
    checkUse(t, st, parent);
    checkNode(t, st, parent);

    traverse(t->sibling, st, parent);
}

static void checkUse(TreeNode *t, SymbolTable *st, TreeNode *parent) {
    if (t->nodekind != ExpK || (t->subkind.exp != IdK && t->subkind.exp != CallK))
        return;
    
    TreeNode *declNode = (TreeNode *)st->lookup(t->attr.name);
    if (!declNode) {
        char buf[256];
        snprintf(buf, sizeof(buf), "Symbol '%s' is not declared.", t->attr.name);
        semanticError(t, buf);
        t->expType = UndefinedType;
        return;
    } else {
        declNode->isUsed = true;
        
        TreeNode *globalDecl = (TreeNode *)st->lookupGlobal(t->attr.name);
        if (globalDecl && globalDecl != declNode) {
            globalDecl->isUsed = true;
        }
        
        if (declNode->subkind.decl == FuncK && t->subkind.exp == IdK) {
            char buf[256];
            snprintf(buf, sizeof(buf), "Cannot use function '%s' as a variable.", t->attr.name);
            semanticError(t, buf);
        }
        
        bool isLHS = (parent && parent->nodekind == ExpK && parent->subkind.exp == AssignK && parent->child[0] == t);
        
        bool isBeingSubscripted = (parent && parent->nodekind == ExpK && parent->subkind.exp == OpK && parent->attr.op == '[' && declNode->isArray);
        
        if (!declNode->isInitialized && declNode->subkind.decl != FuncK && !isLHS && !isBeingSubscripted && t->subkind.exp == IdK) {
            TreeNode *globalCheck = (TreeNode *)st->lookupGlobal(t->attr.name);
            if (globalCheck != declNode) {
                std::string varKey = std::string(t->attr.name) + "_" + std::to_string(declNode->lineno);
                if (warnedUninitVars.find(varKey) == warnedUninitVars.end()) {
                    warnedUninitVars.insert(varKey);
                    char buf[256];
                    snprintf(buf, sizeof(buf), "Variable '%s' may be uninitialized when used here.", t->attr.name);
                    semanticWarning(t, buf);
                }
            }
        }
        
        t->expType = declNode->expType;
        t->isArray = declNode->isArray;
        
        if (t->subkind.exp == CallK) {
            if (declNode->subkind.decl != FuncK) {
                char buf[256];
                snprintf(buf, sizeof(buf), "'%s' is a simple variable and cannot be called.", t->attr.name);
                semanticError(t, buf);
            } else {
                checkCall(t, declNode);
            }
        }
    }
}

static void insertNode(TreeNode *t, SymbolTable *st, TreeNode *parent) {
    if (!t) return;
    
    if (t->nodekind == StmtK && t->subkind.stmt == CompoundK) {
        bool isForLoopBody = (parent && parent->nodekind == StmtK && parent->subkind.stmt == ForK && parent->child[2] == t);
        if (!isForLoopBody) {
            st->enter("compound");
        } 
    }
    else if (t->nodekind == StmtK && t->subkind.stmt == ForK) {
        st->enter("for");
        if (t->child[0] && t->child[0]->nodekind == DeclK && t->child[0]->subkind.decl == VarK) {
            TreeNode *forVarDecl = t->child[0];
            forVarDecl->expType = Integer;
            forVarDecl->isInitialized = true;
            forVarDecl->isUsed = false;
            forVarDecl->isArray = false;
            st->insert(forVarDecl->attr.name, forVarDecl);
        }
    }
    else if (t->nodekind == DeclK) {
        bool isForLoopVar = (parent && parent->nodekind == StmtK && parent->subkind.stmt == ForK && parent->child[0] == t);
        
        if (t->subkind.decl == VarK || t->subkind.decl == ParamK || t->subkind.decl == FuncK) {
            allDeclarations.push_back(t);
        }
        
        if (isForLoopVar) {
            return;
        }
        
        char buf[256];
        bool errorReported = false;
        bool isGlobal = (st->depth() == 1);
        
        if (t->subkind.decl == ParamK && parent && parent->nodekind == DeclK && parent->subkind.decl == FuncK) {
            TreeNode *current = parent;
            if (current->child[0]) {
                TreeNode *param = current->child[0];
                while (param && param != t) {
                    if (param->attr.name && t->attr.name && strcmp(param->attr.name, t->attr.name) == 0) {
                        snprintf(buf, sizeof(buf), "Symbol '%s' is already declared at line %d.", 
                                t->attr.name, param->lineno);
                        semanticError(t, buf);
                        errorReported = true;
                        break;
                    }
                    param = param->sibling;
                }
            }
        }
        
        bool inserted;
        if (!errorReported) {
            if (isGlobal && (t->subkind.decl == VarK || t->subkind.decl == FuncK)) {
                inserted = st->insertGlobal(t->attr.name, t);
            } else {
                inserted = st->insert(t->attr.name, t);
            }
        } else {
            inserted = false;
        }
        
        if (t->subkind.decl == FuncK) {
            if (!inserted) {
                TreeNode *existing;
                if (isGlobal) {
                    existing = (TreeNode*)st->lookupGlobal(t->attr.name);
                } else {
                    existing = (TreeNode*)st->lookup(t->attr.name);
                }
                snprintf(buf, sizeof(buf), "Symbol '%s' is already declared at line %d.", 
                        t->attr.name, existing->lineno);
                semanticError(t, buf);
            }
            if (inserted) {
                functionStack.push_back(t);  // Push current function
                st->enter(t->attr.name);
            }
        } else {
            if (!inserted && !errorReported) {
                TreeNode *existing;
                if (isGlobal) {
                    existing = (TreeNode*)st->lookupGlobal(t->attr.name);
                } else {
                    existing = (TreeNode*)st->lookup(t->attr.name);
                }
                if (existing) {
                    snprintf(buf, sizeof(buf), "Symbol '%s' is already declared at line %d.", t->attr.name, existing->lineno);
                    semanticError(t, buf);
                }
            }
            
            if (t->subkind.decl == ParamK) {
                t->isInitialized = true;  // Parameters are always initialized
            }
            if (t->child[0] != NULL) { // Has explicit initializer
                t->isInitialized = true;
                
                // Check initializer compatibility
                if (t->isArray != t->child[0]->isArray) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "Initializer for variable '%s' requires both operands be arrays or not but variable is%s an array and rhs is%s an array.",
                            t->attr.name,
                            t->isArray ? "" : " not",
                            t->child[0]->isArray ? "" : " not");
                    semanticError(t, buf);
                }
                
                // Check if initializer is constant (simple check for variables)
                if (t->child[0]->nodekind == ExpK && t->child[0]->subkind.exp == IdK) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "Initializer for variable '%s' is not a constant expression.", t->attr.name);
                    semanticError(t, buf);
                }
                
                // Check type compatibility
                if (t->expType != t->child[0]->expType && t->child[0]->expType != UndefinedType) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "Initializer for variable '%s' of type %s is of type %s",
                            t->attr.name, typeToString(t->expType), typeToString(t->child[0]->expType));
                    semanticError(t, buf);
                }
            }
            if (isGlobal && t->subkind.decl == VarK) {
                t->isInitialized = true; // Global variables are auto-initialized
            }
            if (t->isStatic) {
                t->isInitialized = true; // Static variables are auto-initialized to zero
            }
        }
    }
}

static void checkNode(TreeNode *t, SymbolTable *st, TreeNode *parent) {
    if (!t) return;
    
    // Handle array indexing
    if (t->nodekind == ExpK && t->subkind.exp == OpK && t->attr.op == '[') {
        if (t->child[0] && t->child[1]) {
            // Check if left operand is an array
            if (!t->child[0]->isArray) {
                char buf[256];
                snprintf(buf, sizeof(buf), "Cannot index nonarray '%s'.", 
                        t->child[0]->attr.name ? t->child[0]->attr.name : "expression");
                semanticError(t, buf);
            }
            
            // Check if index is an integer
            if (t->child[1]->expType != Integer && t->child[1]->expType != UndefinedType) {
                char buf[256];
                snprintf(buf, sizeof(buf), "Array '%s' should be indexed by type int but got type %s.", 
                        t->child[0]->attr.name ? t->child[0]->attr.name : "expression",
                        typeToString(t->child[1]->expType));
                semanticError(t, buf);
            }
            
            // Check if index is itself an unindexed array
            if (t->child[1]->isArray && t->child[1]->nodekind == ExpK && t->child[1]->subkind.exp == IdK) {
                char buf[256];
                snprintf(buf, sizeof(buf), "Array index is the unindexed array '%s'.", t->child[1]->attr.name);
                semanticError(t, buf);
            }
            
            // Set result type and properties
            if (t->child[0]->isArray) {
                t->expType = t->child[0]->expType;
                t->isArray = false; // Result of indexing is not an array
            }
        }
        return;
    }
    
    // Handle return statements
    if (t->nodekind == StmtK && t->subkind.stmt == ReturnK) {
        if (t->child[0] && t->child[0]->isArray) {
            char buf[256];
            snprintf(buf, sizeof(buf), "Cannot return an array.");
            semanticError(t, buf);
        }
        
        if (!functionStack.empty()) {
            TreeNode* currentFunc = functionStack.back();
            
            if (currentFunc->expType == Void) {
                if (t->child[0]) {
                    char buf[512];  // Use larger buffer
                    snprintf(buf, sizeof(buf), "Function '%s' at line %d is expecting no return value, but return has a value.",
                            currentFunc->attr.name, currentFunc->lineno);
                    semanticError(t, buf);
                }
            } else {
                if (!t->child[0]) {
                    char buf[512];  // Use larger buffer
                    snprintf(buf, sizeof(buf), "Function '%s' at line %d is expecting to return type %s but return has no value.",
                            currentFunc->attr.name, currentFunc->lineno, typeToString(currentFunc->expType));
                    semanticError(t, buf);
                } else if (t->child[0]->expType != currentFunc->expType && t->child[0]->expType != UndefinedType) {
                    char buf[512];  // Use larger buffer
                    snprintf(buf, sizeof(buf), "Function '%s' at line %d is expecting to return type %s but returns type %s.",
                            currentFunc->attr.name, currentFunc->lineno, 
                            typeToString(currentFunc->expType), typeToString(t->child[0]->expType));
                    semanticError(t, buf);
                }
            }
        }
        return;
    }
    
    // Handle break statements
    if (t->nodekind == StmtK && t->subkind.stmt == BreakK) {
        // Check if we're inside a loop
        TreeNode *current = parent;
        bool insideLoop = false;
        while (current) {
            if (current->nodekind == StmtK && 
                (current->subkind.stmt == WhileK || current->subkind.stmt == ForK)) {
                insideLoop = true;
                break;
            }
            // Move up the parent chain - you'll need to track this in traversal
            current = nullptr; // This needs proper parent tracking
        }
        if (!insideLoop) {
            char buf[256];
            snprintf(buf, sizeof(buf), "Cannot have a break statement outside of loop.");
            semanticError(t, buf);
        }
        return;
    }
    
    // Handle if/while statements with test conditions
    if (t->nodekind == StmtK && (t->subkind.stmt == IfK || t->subkind.stmt == WhileK)) {
        if (t->child[0]) {
            if (t->child[0]->isArray) {
                char buf[256];
                const char* stmtType = (t->subkind.stmt == IfK) ? "if" : "while";
                snprintf(buf, sizeof(buf), "Cannot use array as test condition in %s statement.", stmtType);
                semanticError(t, buf);
            } else if (t->child[0]->expType != Boolean && t->child[0]->expType != UndefinedType) {
                char buf[256];
                const char* stmtType = (t->subkind.stmt == IfK) ? "if" : "while";
                snprintf(buf, sizeof(buf), "Expecting Boolean test condition in %s statement but got type %s.",
                        stmtType, typeToString(t->child[0]->expType));
                semanticError(t, buf);
            }
        }
    }
    
    // Handle for statements
    if (t->nodekind == StmtK && t->subkind.stmt == ForK) {
        // Check start expression (child[1])
        if (t->child[1]) {
            if (t->child[1]->isArray) {
                char buf[256];
                snprintf(buf, sizeof(buf), "Cannot use array in position 1 in range of for statement.");
                semanticError(t, buf);
            } else if (t->child[1]->expType != Integer && t->child[1]->expType != UndefinedType) {
                char buf[256];
                snprintf(buf, sizeof(buf), "Expecting type int in position 1 in range of for statement but got type %s.",
                        typeToString(t->child[1]->expType));
                semanticError(t, buf);
            }
        }
        
        // Check stop expression - child[2] if it's not the body
        TreeNode* stopExpr = t->child[2];
        if (stopExpr && stopExpr->nodekind != StmtK) { // Make sure it's not the body
            if (stopExpr->isArray) {
                char buf[256];
                snprintf(buf, sizeof(buf), "Cannot use array in position 2 in range of for statement.");
                semanticError(t, buf);
            } else if (stopExpr->expType != Integer && stopExpr->expType != UndefinedType) {
                char buf[256];
                snprintf(buf, sizeof(buf), "Expecting type int in position 2 in range of for statement but got type %s.",
                        typeToString(stopExpr->expType));
                semanticError(t, buf);
            }
        }
    }
    
    // Handle unary operators (check before binary to avoid conflicts)
    if (t->nodekind == ExpK && t->subkind.exp == OpK && t->child[1] == nullptr) {
        switch (t->attr.op) {
            case NOT:
                if (t->child[0] && t->child[0]->expType != Boolean && t->child[0]->expType != UndefinedType) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "Unary 'not' requires an operand of type bool but was given %s.",
                            typeToString(t->child[0]->expType));
                    semanticError(t, buf);
                }
                t->expType = Boolean;
                break;
                
            case OP_CHSIGN:
                if (t->child[0] && t->child[0]->expType != Integer && t->child[0]->expType != UndefinedType) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "Unary 'chsign' requires an operand of type int but was given %s.",
                            typeToString(t->child[0]->expType));
                    semanticError(t, buf);
                }
                t->expType = Integer;
                break;
                
            case '?':
                if (t->child[0] && t->child[0]->expType != Integer && t->child[0]->expType != UndefinedType) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "Unary '?' requires an operand of type int but was given %s.",
                            typeToString(t->child[0]->expType));
                    semanticError(t, buf);
                }
                if (t->child[0] && t->child[0]->isArray) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "The operation '?' does not work with arrays.");
                    semanticError(t, buf);
                }
                t->expType = Integer;
                break;
                
            case OP_SIZEOF:
                if (t->child[0] && !t->child[0]->isArray) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "The operation 'sizeof' only works with arrays.");
                    semanticError(t, buf);
                }
                t->expType = Integer;
                break;
                
            case '*': // Unary dereference operator
                if (t->child[0] && !t->child[0]->isArray) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "The operation '*' only works with arrays.");
                    semanticError(t, buf);
                }
                t->expType = t->child[0] ? t->child[0]->expType : UndefinedType;
                break;
        }
        return;
    }
    
    // Handle assignments (including compound assignments)
    if (t->nodekind == ExpK && t->subkind.exp == AssignK) {
        if (t->child[0] && t->child[1]) {
            // For compound assignments (+=, -=, *=, /=), check specific type requirements
            if (t->attr.op == ADDASS || t->attr.op == SUBASS || t->attr.op == MULASS || t->attr.op == DIVASS) {
                if (t->child[0]->expType != Integer && t->child[0]->expType != UndefinedType) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "'%s' requires operands of type int but lhs is of type %s.",
                            opToString(t->attr.op), typeToString(t->child[0]->expType));  // Added "type"
                    semanticError(t, buf);
                }
                if (t->child[1]->expType != Integer && t->child[1]->expType != UndefinedType) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "'%s' requires operands of type int but rhs is of type %s.",
                            opToString(t->attr.op), typeToString(t->child[1]->expType));  // Added "type"
                    semanticError(t, buf);
                }
            } else {
                // Regular assignment - check for type compatibility
                if (t->child[0]->expType != UndefinedType && t->child[1]->expType != UndefinedType) {
                    // Check if types are compatible (Char and CharInt are compatible)
                    bool compatible = (t->child[0]->expType == t->child[1]->expType) ||
                                     (t->child[0]->expType == Char && t->child[1]->expType == CharInt) ||
                                     (t->child[0]->expType == CharInt && t->child[1]->expType == Char);
                    
                    if (!compatible) {
                        char buf[256];
                        snprintf(buf, sizeof(buf), "':=' requires operands of the same type but lhs is type %s and rhs is type %s.",
                                typeToString(t->child[0]->expType), typeToString(t->child[1]->expType));
                        semanticError(t, buf);
                    }
                    
                    // Check array compatibility
                    if (t->child[0]->isArray != t->child[1]->isArray) {
                        char buf[256];
                        snprintf(buf, sizeof(buf), "':=' requires both operands be arrays or not but lhs is%s an array and rhs is%s an array.",
                                t->child[0]->isArray ? "" : " not", t->child[1]->isArray ? "" : " not");
                        semanticError(t, buf);
                    }
                }
            }
            
            // Mark LHS as initialized if it's a variable
            if (t->child[0]->nodekind == ExpK && t->child[0]->subkind.exp == IdK) {
                TreeNode *declNode = (TreeNode *)st->lookup(t->child[0]->attr.name);
                if (declNode) {
                    declNode->isInitialized = true;
                    declNode->isUsed = true;
                }
            }
            
            // ADD: Handle assignment to array element
            if (t->child[0]->nodekind == ExpK && t->child[0]->subkind.exp == OpK && t->child[0]->attr.op == '[') {
                // This is an assignment to an array element - mark array as initialized
                TreeNode *arrayId = t->child[0]->child[0];
                if (arrayId && arrayId->nodekind == ExpK && arrayId->subkind.exp == IdK) {
                    TreeNode *declNode = (TreeNode *)st->lookup(arrayId->attr.name);
                    if (declNode) {
                        declNode->isInitialized = true;
                    }
                }
            }
            
            t->expType = t->child[0]->expType;
        } 
        else if (t->child[0] && !t->child[1]) {
            // Unary assignment operators (++, --)
            if (t->attr.op == INC || t->attr.op == DEC) {
                if (t->child[0]->expType != Integer && t->child[0]->expType != UndefinedType) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "'%s' requires operand of type int but was given %s.",
                            opToString(t->attr.op), typeToString(t->child[0]->expType));  // Added "type"
                    semanticError(t, buf);
                }
                
                // Mark LHS as initialized if it's a variable
                if (t->child[0]->nodekind == ExpK && t->child[0]->subkind.exp == IdK) {
                    TreeNode *declNode = (TreeNode *)st->lookup(t->child[0]->attr.name);
                    if (declNode) {
                        declNode->isInitialized = true;
                        declNode->isUsed = true;
                    }
                }
                
                t->expType = Integer;  // ++ and -- always result in int type
            }
        }
        return;
    }
    
    // Handle binary operators
    if (t->nodekind == ExpK && t->subkind.exp == OpK && t->child[0] && t->child[1]) {
        switch (t->attr.op) {
            case OR:
            case AND:
                // Check for array operands in logical operations
                if (t->child[0]->isArray || t->child[1]->isArray) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "The operation '%s' does not work with arrays.", opToString(t->attr.op));
                    semanticError(t, buf);
                }
                if (t->child[0]->expType != Boolean && t->child[0]->expType != UndefinedType) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "'%s' requires operands of type bool but lhs is of type %s.",
                            opToString(t->attr.op), typeToString(t->child[0]->expType));
                    semanticError(t, buf);
                }
                if (t->child[1]->expType != Boolean && t->child[1]->expType != UndefinedType) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "'%s' requires operands of type bool but rhs is of type %s.",
                            opToString(t->attr.op), typeToString(t->child[1]->expType));
                    semanticError(t, buf);
                }
                t->expType = Boolean;
                break;
                
            case EQ:
            case NEQ:
            case LT:
            case LE:
            case GT:
            case GE:
                // Check array compatibility first
                if (t->child[0]->isArray != t->child[1]->isArray) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "'%s' requires both operands be arrays or not but lhs is%s an array and rhs is%s an array.",
                            opToString(t->attr.op), 
                            t->child[0]->isArray ? "" : " not", 
                            t->child[1]->isArray ? "" : " not");
                    semanticError(t, buf);
                }
                if (t->child[0]->expType != UndefinedType && t->child[1]->expType != UndefinedType &&
                    t->child[0]->expType != t->child[1]->expType) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "'%s' requires operands of the same type but lhs is type %s and rhs is type %s.",
                            opToString(t->attr.op), typeToString(t->child[0]->expType), typeToString(t->child[1]->expType));
                    semanticError(t, buf);
                }
                t->expType = Boolean;
                break;
                
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
                // Add array checking first
                if (t->child[0]->isArray || t->child[1]->isArray) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "The operation '%s' does not work with arrays.", opToString(t->attr.op));
                    semanticError(t, buf);
                }
                if (t->child[0]->expType != Integer && t->child[0]->expType != UndefinedType) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "'%s' requires operands of type int but lhs is of type %s.",
                            opToString(t->attr.op), typeToString(t->child[0]->expType));
                    semanticError(t, buf);
                }
                if (t->child[1]->expType != Integer && t->child[1]->expType != UndefinedType) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "'%s' requires operands of type int but rhs is of type %s.",
                            opToString(t->attr.op), typeToString(t->child[1]->expType));
                    semanticError(t, buf);
                }
                t->expType = Integer;
                break;
        }
    }
    
    // Leave scopes
    if (t->nodekind == DeclK && t->subkind.decl == FuncK) {
        if (!functionStack.empty()) {
            functionStack.pop_back();  // Pop current function
        }
        st->leave();
    } else if (t->nodekind == StmtK && t->subkind.stmt == CompoundK) {
        bool isForLoopBody = (parent && parent->nodekind == StmtK && parent->subkind.stmt == ForK && parent->child[2] == t);
        if (!isForLoopBody) {
            st->leave();
        }
    } else if (t->nodekind == StmtK && t->subkind.stmt == ForK) {
        st->leave();
    }
}

static void checkCall(TreeNode *t, TreeNode *declNode) {
    if (!t || !declNode || t->subkind.exp != CallK) return;
    
    // Count parameters in call
    int callParamCount = 0;
    TreeNode *callParam = t->child[0];
    while (callParam) {
        callParamCount++;
        callParam = callParam->sibling;
    }
    
    // Count parameters in declaration
    int declParamCount = 0;
    TreeNode *declParam = declNode->child[0];
    while (declParam) {
        declParamCount++;
        declParam = declParam->sibling;
    }
    
    // Check parameter count
    if (callParamCount < declParamCount) {
        char buf[256];
        snprintf(buf, sizeof(buf), "Too few parameters passed for function '%s' declared on line %d.",
                declNode->attr.name, declNode->lineno);
        semanticError(t, buf);
        return;
    } else if (callParamCount > declParamCount) {
        char buf[256];
        snprintf(buf, sizeof(buf), "Too many parameters passed for function '%s' declared on line %d.",
                declNode->attr.name, declNode->lineno);
        semanticError(t, buf);
        return;
    }
    
    // Check parameter types
    callParam = t->child[0];
    declParam = declNode->child[0];
    int paramIndex = 1;
    
    while (callParam && declParam) {
        // Check type compatibility
        if (callParam->expType != UndefinedType && declParam->expType != UndefinedType &&
            callParam->expType != declParam->expType) {
            char buf[256];
            snprintf(buf, sizeof(buf), "Expecting type %s in parameter %d of call to '%s' declared on line %d but got type %s.",
                    typeToString(declParam->expType), paramIndex, declNode->attr.name, 
                    declNode->lineno, typeToString(callParam->expType));  // Added "type"
            semanticError(t, buf);
        }
        
        // Check array compatibility
        if (declParam->isArray && !callParam->isArray) {
            char buf[256];
            snprintf(buf, sizeof(buf), "Expecting array in parameter %d of call to '%s' declared on line %d.",
                    paramIndex, declNode->attr.name, declNode->lineno);
            semanticError(t, buf);
        } else if (!declParam->isArray && callParam->isArray) {
            char buf[256];
            snprintf(buf, sizeof(buf), "Not expecting array in parameter %d of call to '%s' declared on line %d.",
                    paramIndex, declNode->attr.name, declNode->lineno);
            semanticError(t, buf);
        }
        
        callParam = callParam->sibling;
        declParam = declParam->sibling;
        paramIndex++;
    }
}