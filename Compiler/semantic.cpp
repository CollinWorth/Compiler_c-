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
static void checkChildren(TreeNode *t, SymbolTable *st);
static void checkBinaryOp(TreeNode *t, SymbolTable *st, ExpType requiredType, ExpType resultType, bool allowArrays = false);
static void checkUnaryOp(TreeNode *t, ExpType requiredType, ExpType resultType, bool allowArrays = false);
static void debugPrint(TreeNode *t, const char *);
static void markExpressionAsUndefined(TreeNode *t);
static void debugSymbolTable(SymbolTable *st, const char* location, const char* varName = "x");

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
    if (numErrors < MAX_WARNINGS) { // Using MAX_WARNINGS for both for simplicity
        errors[numErrors].lineno = t->lineno;
        strncpy(errors[numErrors].message, message, 255);
        errors[numErrors].message[255] = '\0';
        errors[numErrors].isError = true;
        numErrors++;
    }
}

void semanticWarning(TreeNode *t, const char *message) {
    if (numWarnings < MAX_WARNINGS) {
        warnings[numWarnings].lineno = t->lineno;
        strncpy(warnings[numWarnings].message, message, 255);
        warnings[numWarnings].message[255] = '\0';
        warnings[numWarnings].isError = false;
        numWarnings++;
    }
}

static std::set<std::string> warnedUninitVars;
static std::vector<TreeNode*> allDeclarations;

void semanticAnalysis(TreeNode *t, SymbolTable *st) {
    numErrors = numWarnings = 0;
    warnedUninitVars.clear();  // Clear previous warnings
    allDeclarations.clear();   // Clear previous declarations

    traverse(t, st, nullptr);  // Start traversal from the root of the AST

    // Check for main function using lookupGlobal - main must be in global scope
    TreeNode *mainNode = (TreeNode *)st->lookupGlobal("main");
    if (!mainNode || mainNode->subkind.decl != FuncK) {
        // Add linker error to the buffer with high line number so it sorts to the end
        if (numErrors < MAX_WARNINGS) {
            errors[numErrors].lineno = 99999; // High line number to sort to the end
            strncpy(errors[numErrors].message, "A function named 'main()' must be defined.", 255);
            errors[numErrors].isError = true;
            numErrors++;
        }
    }

    // NOW check for unused variables after entire tree is processed
    // But don't warn about variables that had redeclaration errors
    for (TreeNode* decl : allDeclarations) {
        if ((decl->subkind.decl == VarK || decl->subkind.decl == ParamK) && !decl->isUsed) {
            // Check if this variable had a redeclaration error
            bool hasRedeclarationError = false;
            for (int i = 0; i < numErrors; i++) {
                if (errors[i].lineno == decl->lineno && 
                    strstr(errors[i].message, "already declared") != nullptr) {
                    hasRedeclarationError = true;
                    break;
                }
            }
            
            // Only warn if no redeclaration error
            if (!hasRedeclarationError) {
                char buf[256];
                snprintf(buf, sizeof(buf), "The variable '%s' seems not to be used.", decl->attr.name);
                semanticWarning(decl, buf);
            }
        }
    }

    // Combine, sort, and print errors and warnings
    std::vector<SemanticMessage> allMessages;
    allMessages.insert(allMessages.end(), errors, errors + numErrors);
    allMessages.insert(allMessages.end(), warnings, warnings + numWarnings);

    std::stable_sort(allMessages.begin(), allMessages.end(), 
          [](const SemanticMessage& a, const SemanticMessage& b) {
    // Special case: group unused variable warnings with their related redeclaration errors
    // If this is a "not used" warning for a for-loop variable, it should come after
    // any redeclaration error in the same for-loop context
    
    bool aIsUnusedWarning = !a.isError && strstr(a.message, "seems not to be used") != nullptr;
    bool bIsRedeclError = b.isError && strstr(b.message, "already declared") != nullptr;
    
    // If a is an unused warning and b is a redeclaration error, and they're close in line numbers
    if (aIsUnusedWarning && bIsRedeclError && abs(a.lineno - b.lineno) <= 2) {
        // Check if the redeclaration error mentions the same line as the unused warning
        char lineStr[10];
        snprintf(lineStr, sizeof(lineStr), "line %d", a.lineno);
        if (strstr(b.message, lineStr) != nullptr) {
            return false; // Put the unused warning after the related redeclaration error
        }
    }
    
    // Normal sorting: by line number first
    if (a.lineno != b.lineno) return a.lineno < b.lineno;
    
    // For same line, errors before warnings  
    return a.isError && !b.isError;
});

    for (const auto& msg : allMessages) {
        if (msg.isError) {
            if (msg.lineno == 99999) {
                printf("ERROR(LINKER): %s\n", msg.message);
            } else {
                printf("ERROR(%d): %s\n", msg.lineno, msg.message);
            }
        } else {
            printf("WARNING(%d): %s\n", msg.lineno, msg.message);
        }
    }
    
}

static void checkChildren(TreeNode *t, SymbolTable *st) {
    if (!t) return;
    for (int i = 0; i < MAXCHILDREN; i++) {
        traverse(t->child[i], st, t);
    }
}

static void traverse(TreeNode *t, SymbolTable *st, TreeNode *parent) {
    if (!t) return;
    
    // Pre-order actions: enter scopes, insert symbols
    insertNode(t, st, parent);
    
    // Traverse children FIRST
    for (int i = 0; i < MAXCHILDREN; i++) {
        traverse(t->child[i], st, t);
    }
    
    // Post-order actions: check uses, do type checking, leave scopes
    checkUse(t, st, parent);  // MOVED HERE - after children are processed
    checkNode(t, st, parent);

    // Traverse siblings
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
        // Set type to undefined for error recovery
        t->expType = UndefinedType;
        return;
    } else {
        // Mark the original declaration node as used - ALWAYS do this
        declNode->isUsed = true;
        
        // ALSO mark any global variable with the same name as used
        TreeNode *globalDecl = (TreeNode *)st->lookupGlobal(t->attr.name);
        if (globalDecl && globalDecl != declNode) {
            globalDecl->isUsed = true;
        }
        
        // Check if this is a function being used as a variable
        if (declNode->subkind.decl == FuncK && t->subkind.exp == IdK) {
            char buf[256];
            snprintf(buf, sizeof(buf), "Cannot use function '%s' as a variable.", t->attr.name);
            semanticError(t, buf);
        }
        
        // Check for uninitialized variable usage - only for variables (not functions)
        // Don't warn if this is the LHS of an assignment
        bool isLHS = (parent && parent->nodekind == ExpK && parent->subkind.exp == AssignK && parent->child[0] == t);
        
        if (!declNode->isInitialized && declNode->subkind.decl != FuncK && !isLHS && t->subkind.exp == IdK) {
            // Global variables are implicitly initialized to 0/false/etc.
            TreeNode *globalCheck = (TreeNode *)st->lookupGlobal(t->attr.name);
            if (globalCheck != declNode) { // Not a global variable
                // Create a unique key for this variable + line combination
                std::string varKey = std::string(t->attr.name) + "_" + std::to_string(declNode->lineno);
                if (warnedUninitVars.find(varKey) == warnedUninitVars.end()) {
                    warnedUninitVars.insert(varKey);
                    char buf[256];
                    snprintf(buf, sizeof(buf), "Variable '%s' may be uninitialized when used here.", t->attr.name);
                    semanticWarning(t, buf);
                }
            }
        }
        
        // Copy type information from the declaration to the use.
        t->expType = declNode->expType;
        t->isArray = declNode->isArray;
        
        // If this is a function call, do basic call checking
        if (t->subkind.exp == CallK) {
            if (declNode->subkind.decl != FuncK) {
                char buf[256];
                snprintf(buf, sizeof(buf), "'%s' is a simple variable and cannot be called.", t->attr.name);
                semanticError(t, buf);
            }
        }
    }
}

static void checkCall(TreeNode *t, TreeNode *declNode) {
    // This function is now a placeholder.
    // We can add back argument/parameter checking logic here later.
}

static void insertNode(TreeNode *t, SymbolTable *st, TreeNode *parent) {
    if (!t) return;
    
    if (t->nodekind == StmtK && t->subkind.stmt == CompoundK) {
        // Check if this compound is the direct body of a for-loop
        bool isForLoopBody = (parent && parent->nodekind == StmtK && parent->subkind.stmt == ForK && parent->child[2] == t);
        
        if (!isForLoopBody) {
            // Only create a new scope if this is NOT a for-loop body
            st->enter("compound");
        } 
    }
    // Handle for loop scopes - create separate for-loop scope
    else if (t->nodekind == StmtK && t->subkind.stmt == ForK) {
        st->enter("for");
        // The for loop variable is in child[0] as a Var declaration node
        if (t->child[0] && t->child[0]->nodekind == DeclK && t->child[0]->subkind.decl == VarK) {
            TreeNode *forVarDecl = t->child[0];
            
            // Set properties for the for-loop variable
            forVarDecl->expType = Integer;
            forVarDecl->isInitialized = true;
            forVarDecl->isUsed = false;
            forVarDecl->isArray = false;
            
            // Insert the for-loop variable into the for-loop scope
            bool inserted = st->insert(forVarDecl->attr.name, forVarDecl);
        }
    }
    else if (t->nodekind == DeclK) {
        // Check if this is a for-loop variable (parent is ForK)
        bool isForLoopVar = (parent && parent->nodekind == StmtK && parent->subkind.stmt == ForK && parent->child[0] == t);
        
        // Add declaration to the list for later unused checking
        if (t->subkind.decl == VarK || t->subkind.decl == ParamK) {
            allDeclarations.push_back(t);
        }
        
        // Skip insertion logic for for-loop variables (already inserted by ForK handling)
        if (isForLoopVar) {
            return;
        }
        
        char buf[256];
        bool errorReported = false;
        bool isGlobal = (st->depth() == 1); // Global scope has depth 1
        
        // For function parameters, check against other parameters in the same parameter list
        if (t->subkind.decl == ParamK && parent && parent->nodekind == DeclK && parent->subkind.decl == FuncK) {
            // Walk through the parameter list to find duplicates
            TreeNode *current = parent;
            if (current->child[0]) { // child[0] is the parameter list
                TreeNode *param = current->child[0];
                while (param && param != t) {
                    if (param->attr.name && strcmp(param->attr.name, t->attr.name) == 0) {
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
        
        // Try to insert into symbol table - use insertGlobal for global declarations
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
            st->enter(t->attr.name);
        } else { // VarK or ParamK
            // If the symbol is already in the table, insertion will fail.
            if (!inserted && !errorReported) {
                TreeNode *existing;
                if (isGlobal) {
                    existing = (TreeNode*)st->lookupGlobal(t->attr.name);
                } else {
                    existing = (TreeNode*)st->lookup(t->attr.name);
                }
                if (existing) { // Make sure lookup returned something
                    snprintf(buf, sizeof(buf), "Symbol '%s' is already declared at line %d.", t->attr.name, existing->lineno);
                    semanticError(t, buf);
                }
            }
            
            // Parameters are considered initialized, variables with initializers too
            if (t->subkind.decl == ParamK) {
                t->isInitialized = true;
            }
            if (t->child[0] != NULL) { // Has initializer
                t->isInitialized = true;
            }
            // Global variables are implicitly initialized
            if (isGlobal && t->subkind.decl == VarK) {
                t->isInitialized = true;
            }
            // Array variables are considered initialized upon declaration
            if (t->isArray) {
                t->isInitialized = true;
            }
        }
    }
}

static void checkNode(TreeNode *t, SymbolTable *st, TreeNode *parent) {
    if (!t) return;
    
    // Basic operator type checking
    if (t->nodekind == ExpK && t->subkind.exp == OpK) {
        switch (t->attr.op) {
            case OR:
            case AND:
                // Boolean operators require bool operands
                if (t->child[0] && t->child[0]->expType != Boolean) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "'%s' requires operands of type bool but lhs is of type %s.", 
                            opToString(t->attr.op), typeToString(t->child[0]->expType));
                    semanticError(t, buf);
                }
                if (t->child[1] && t->child[1]->expType != Boolean) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "'%s' requires operands of type bool but rhs is of type %s.", 
                            opToString(t->attr.op), typeToString(t->child[1]->expType));
                    semanticError(t, buf);
                }
                t->expType = Boolean;
                break;
        }
    }
    
    // Handle assignments to mark variables as initialized
    if (t->nodekind == ExpK && t->subkind.exp == AssignK) {
        if (t->child[0] && t->child[0]->nodekind == ExpK && t->child[0]->subkind.exp == IdK) {
            TreeNode *declNode = (TreeNode *)st->lookup(t->child[0]->attr.name);
            if (declNode) {
                declNode->isInitialized = true;
                declNode->isUsed = true;
            }
        }
    }
    
    // Leave scopes for functions, compound statements (except for-loop bodies), AND for loops
    if (t->nodekind == DeclK && t->subkind.decl == FuncK) {
        st->leave();
    } else if (t->nodekind == StmtK && t->subkind.stmt == CompoundK) {
        // Check if this compound is the direct body of a for-loop
        bool isForLoopBody = (parent && parent->nodekind == StmtK && parent->subkind.stmt == ForK && parent->child[2] == t);
        
        if (!isForLoopBody) {
            // Only leave scope if this is NOT a for-loop body
            st->leave();
        }
    } else if (t->nodekind == StmtK && t->subkind.stmt == ForK) {
        st->leave();  // Leave the for-loop scope
    }
}

// Helper function for checking binary operators
static void checkBinaryOp(TreeNode *t, SymbolTable *st, ExpType requiredType, ExpType resultType, bool allowArrays) {
    // This function is now a placeholder.
    // We can add back type checking for binary operators here.
}

// Helper for unary operators
static void checkUnaryOp(TreeNode *t, ExpType requiredType, ExpType resultType, bool allowArrays) {
    // This function is now a placeholder.
    // We can add back type checking for unary operators here.
}

// Add this helper function at the top with your other static functions
static void debugPrint(TreeNode *t, const char* location) {
    if (!t) {
        printf("DEBUG [%s]: TreeNode is NULL\n", location);
        return;
    }
    
    printf("DEBUG [%s]: Node at line %d\n", location, t->lineno);
    printf("  - nodekind: %d ", t->nodekind);
    if (t->nodekind == ExpK) printf("(ExpK)");
    else if (t->nodekind == DeclK) printf("(DeclK)");
    else if (t->nodekind == StmtK) printf("(StmtK)");
    printf("\n");
    
    if (t->nodekind == ExpK) {
        printf("  - subkind.exp: %d ", t->subkind.exp);
        if (t->subkind.exp == IdK) printf("(IdK)");
        else if (t->subkind.exp == OpK) printf("(OpK)");
        else if (t->subkind.exp == CallK) printf("(CallK)");
        printf("\n");
    }
    
    printf("  - attr.name: %s\n", t->attr.name ? t->attr.name : "NULL");
    printf("  - expType: %d (%s)\n", t->expType, typeToString(t->expType));
    printf("  - isArray: %s\n", t->isArray ? "true" : "false");
    printf("  - isUsed: %s\n", t->isUsed ? "true" : "false");
    printf("  - isInitialized: %s\n", t->isInitialized ? "true" : "false");
    printf("---\n");
}

static void debugSymbolTable(SymbolTable *st, const char* location, const char* varName) {
    printf("=== SYMBOL TABLE DEBUG [%s] ===\n", location);
    printf("Current depth: %d\n", st->depth());
    
    TreeNode *node = (TreeNode*)st->lookup(varName);
    if (node) {
        printf("Found '%s' at line %d\n", varName, node->lineno);
    } else {
        printf("'%s' not found in symbol table\n", varName);
    }
    printf("===================================\n");
}