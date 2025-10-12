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

void semanticAnalysis(TreeNode *t, SymbolTable *st) {
    numErrors = numWarnings = 0;
    warnedUninitVars.clear();  // Clear previous warnings
    traverse(t, st, nullptr);  // Start with no parent

    // Combine, sort, and print errors and warnings
    std::vector<SemanticMessage> allMessages;
    allMessages.insert(allMessages.end(), errors, errors + numErrors);
    allMessages.insert(allMessages.end(), warnings, warnings + numWarnings);

    std::sort(allMessages.begin(), allMessages.end(), 
              [](const SemanticMessage& a, const SemanticMessage& b) {
        return a.lineno < b.lineno;
    });

    for (const auto& msg : allMessages) {
        if (msg.isError) {
            printf("ERROR(%s): %s\n", msg.lineno == 0 ? "LINKER" : std::to_string(msg.lineno).c_str(), msg.message);
        } else {
            printf("WARNING(%d): %s\n", msg.lineno, msg.message);
        }
    }
    
    // Check for main function
    TreeNode *mainNode = (TreeNode *)st->lookup("main");
    if (!mainNode || mainNode->subkind.decl != FuncK) {
        // Add linker error to the buffer
        if (numErrors < MAX_WARNINGS) {
            errors[numErrors].lineno = 0; // Linker error has no line number
            strncpy(errors[numErrors].message, "A function named 'main()' must be defined.", 255);
            errors[numErrors].isError = true;
            numErrors++;
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
    
    insertNode(t, st, parent);
    checkUse(t, st, parent); // Check for symbol use before traversing children
    
    // Pass t as parent to children
    for (int i = 0; i < MAXCHILDREN; i++) {
        traverse(t->child[i], st, t);
    }
    
    checkNode(t, st, parent);
    traverse(t->sibling, st, parent);  // Siblings have same parent
}

static void checkUse(TreeNode *t, SymbolTable *st, TreeNode *parent) {
    if (t->nodekind != ExpK || (t->subkind.exp != IdK && t->subkind.exp != CallK))
        return;
    
    TreeNode *declNode = (TreeNode *)st->lookup(t->attr.name);
    if (!declNode) {
        char buf[256];
        snprintf(buf, sizeof(buf), "Symbol '%s' is not declared.", t->attr.name);
        semanticError(t, buf);
    } else {
        // The pointer returned by lookup IS the original declaration node.
        // Mark it as used.
        declNode->isUsed = true;
        
        // Check if this is a function being used as a variable
        if (declNode->subkind.decl == FuncK && t->subkind.exp == IdK) {
            char buf[256];
            snprintf(buf, sizeof(buf), "Cannot use function '%s' as a variable.", t->attr.name);
            semanticError(t, buf);
        }

        // Check if parent is a Range node
        bool inRangeContext = (parent && parent->nodekind == StmtK && 
                              parent->subkind.stmt == RangeK);
        
        if (declNode->isArray && inRangeContext && t->subkind.exp == IdK) {
            t->expType = UndefinedType;  // Arrays in ranges are undefined
        } else {
            t->expType = declNode->expType;  // Normal case
        }
        
        t->isArray = declNode->isArray;
        
        if (t->subkind.exp == CallK) {
            // Check if trying to call a non-function
            if (declNode->subkind.decl != FuncK) {
                char buf[256];
                snprintf(buf, sizeof(buf), "'%s' is a simple variable and cannot be called.", t->attr.name);
                semanticError(t, buf);
            } else {
                // Re-enable function call parameter checking
                checkCall(t, declNode);
            }
        }
    }
}

static void checkCall(TreeNode *t, TreeNode *declNode) {
    TreeNode *args = t->child[0];
    TreeNode *params = declNode->child[0];
    char buf[256];
    int paramNum = 1;

    while (args && params) {
        if (args->expType != params->expType) {
            snprintf(buf, sizeof(buf), 
                    "Expecting type %s in parameter %d of call to '%s' but got type %s.", 
                    typeToString(params->expType), paramNum, 
                    declNode->attr.name, typeToString(args->expType));
            semanticError(t, buf);
        }
        if (args->isArray != params->isArray) {
            snprintf(buf, sizeof(buf), "Expecting %s in parameter %d of call to '%s' but got %s.",
                     params->isArray ? "an array" : "a simple value", paramNum, declNode->attr.name, args->isArray ? "an array" : "a simple value");
            semanticError(t, buf);
        }
        args = args->sibling;
        params = params->sibling;
        paramNum++;
    }

    if (args) {
        snprintf(buf, sizeof(buf), "Too many arguments to function '%s'.", declNode->attr.name);
        semanticError(t, buf);
    }

    if (params) {
        snprintf(buf, sizeof(buf), "Too few arguments to function '%s'.", declNode->attr.name);
        semanticError(t, buf);
    }
}

static void insertNode(TreeNode *t, SymbolTable *st, TreeNode *parent) {
    if (!t) return;
    
    if (t->nodekind == StmtK && t->subkind.stmt == CompoundK) {
        st->enter("compound");
    }
    else if (t->nodekind == DeclK) {
        char buf[256];
        bool errorReported = false;
        
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
        
        // Try to insert into symbol table
        bool inserted = errorReported ? false : st->insert(t->attr.name, t);
        
        if (t->subkind.decl == FuncK) {
            if (!inserted) {
                TreeNode *existing = (TreeNode*)st->lookup(t->attr.name);
                snprintf(buf, sizeof(buf), "Symbol '%s' is already declared at line %d.", 
                        t->attr.name, existing->lineno);
                semanticError(t, buf);
            } else {
                st->enter(t->attr.name);
            }
        } else { // VarK or ParamK
            // If the symbol is already in the table, insertion will fail.
            if (!inserted) {
                TreeNode *existing = (TreeNode*)st->lookup(t->attr.name);
                if (existing) { // Make sure lookup returned something
                    snprintf(buf, sizeof(buf), "Symbol '%s' is already declared at line %d.", t->attr.name, existing->lineno);
                    semanticError(t, buf);
                }
            }
        }
    }
}

static void checkNode(TreeNode *t, SymbolTable *st, TreeNode *parent) {
    if (!t) return;
    
    // Handle operations inside Range nodes - they should be undefined type
    if (t->nodekind == ExpK && t->subkind.exp == OpK && 
        parent && parent->nodekind == StmtK && parent->subkind.stmt == RangeK) {
        
        // Set operation type to undefined if it's in a range context
        switch (t->attr.op) {
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
                t->expType = UndefinedType;
                return; // Skip normal operator processing
        }
    }
    
    if (t->nodekind == DeclK && t->subkind.decl == FuncK) st->leave();
    
    else if (t->nodekind == ExpK && t->subkind.exp == OpK) {
        // Handle binary operators
        switch (t->attr.op) {
            case '[': // Array indexing
                if (!t->child[0]->isArray) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "Cannot index nonarray '%s'.", 
                            t->child[0]->attr.name ? t->child[0]->attr.name : "");
                    semanticError(t, buf);
                }
                if (t->child[1]->isArray) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "Array index is the unindexed array '%s'.", 
                            t->child[1]->attr.name ? t->child[1]->attr.name : "");
                    semanticError(t, buf);
                }
                else if (t->child[1]->expType != Integer) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "Array '%s' should be indexed by type int but got type %s.", 
                            t->child[0]->attr.name ? t->child[0]->attr.name : "", 
                            typeToString(t->child[1]->expType));
                    semanticError(t, buf);
                }
                t->expType = t->child[0]->expType;
                t->isArray = false;
                break;
                
            // Boolean operators
            case AND:
            case OR:
                checkBinaryOp(t, st, Boolean, Boolean);
                break;
                
            // Comparison operators - Add array checking
            case EQ:
            case NEQ:
            case LT:
            case LE:
            case GT:
            case GE:
                // Add array operation errors
                if (t->child[0]->isArray || t->child[1]->isArray) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "The operation '%s' does not work with arrays.", 
                            opToString(t->attr.op));
                    semanticError(t, buf);
                }
                checkBinaryOp(t, st, UndefinedType, Boolean);
                break;
                
            // Arithmetic operators
            case '+':
            case '-':
            case '/':
            case '%':
                checkBinaryOp(t, st, Integer, Integer);
                if (t->attr.op == '/' || t->attr.op == '%') {
                    // Add special message about division/modulo with arrays
                    if (t->child[0]->isArray || t->child[1]->isArray) {
                        char buf[256];
                        snprintf(buf, sizeof(buf), "The operation '%s' does not work with arrays.", 
                                opToString(t->attr.op));
                        semanticError(t, buf);
                    }
                }
                break;
                
            case '*': // Multiplication OR Dereference - handle both cases
                if (t->child[1]) {
                    // Binary multiplication
                    checkBinaryOp(t, st, Integer, Integer);
                } else {
                    // Unary dereference
                    if (!t->child[0]->isArray) {
                        char buf[256];
                        snprintf(buf, sizeof(buf), "The operation '*' only works with arrays.");
                        semanticError(t, buf);
                    }
                    t->expType = t->child[0]->expType;
                    t->isArray = false;
                }
                break;
                
            // Compound assignment
            case ADDASS:
            case SUBASS:
            case MULASS:
            case DIVASS:
                checkBinaryOp(t, st, Integer, Integer);
                break;
                
            // Unary operators
            case NOT:
                checkUnaryOp(t, Boolean, Boolean);
                break;
                
            case OP_CHSIGN:
                checkUnaryOp(t, Integer, Integer);
                break;
                
            case '?':
                // Add array operation error
                if (t->child[0]->isArray) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "The operation '?' does not work with arrays.");
                    semanticError(t, buf);
                }
                checkUnaryOp(t, Integer, Integer);
                break;
                
            case OP_SIZEOF:
                if (!t->child[0]->isArray) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "The operation 'sizeof' only works with arrays.");
                    semanticError(t, buf);
                }
                t->expType = Integer;
                break;
        }
    }
    else if (t->nodekind == ExpK && t->subkind.exp == AssignK) {
        // Assignments should have the type of the left side
        if (t->child[0]) {
            // Type checking must happen first
            if (t->child[1] && t->child[0]->expType != t->child[1]->expType) {
                char buf[256];
                snprintf(buf, sizeof(buf), "'%s' requires operands of the same type but lhs is type %s and rhs is type %s.",
                         opToString(t->attr.op), typeToString(t->child[0]->expType), typeToString(t->child[1]->expType));
                semanticError(t, buf);
            }
            t->expType = t->child[0]->expType;
            
            // Mark left-hand side as initialized
            if (t->child[0]->nodekind == ExpK && t->child[0]->subkind.exp == IdK) {
                TreeNode *declNode = (TreeNode *)st->lookup(t->child[0]->attr.name);
                if (declNode) {
                    declNode->isUsed = true;
                    declNode->isInitialized = true;
                }
            }
            // Also handle array subscript assignments
            else if (t->child[0]->nodekind == ExpK && t->child[0]->subkind.exp == OpK && t->child[0]->attr.op == '[') {
                if (t->child[0]->child[0] && t->child[0]->child[0]->nodekind == ExpK && 
                    t->child[0]->child[0]->subkind.exp == IdK) {
                    TreeNode *declNode = (TreeNode *)st->lookup(t->child[0]->child[0]->attr.name);
                    if (declNode) {
                        declNode->isUsed = true;
                        declNode->isInitialized = true;
                    }
                } 
            }
        }
    }
    else if (t->nodekind == StmtK && t->subkind.stmt == ReturnK) {
        TreeNode* func = (TreeNode*)st->lookup(st->scopeName());
        if (func) {
            ExpType returnType = t->child[0] ? t->child[0]->expType : Void;
            if (func->expType != returnType && !(func->expType == Char && returnType == Integer) && !(func->expType == Integer && returnType == Char)) {
                char buf[256];
                snprintf(buf, sizeof(buf), "Function '%s' should return type %s but returns type %s.", func->attr.name, typeToString(func->expType), typeToString(returnType));
                semanticError(t, buf);
            }
        }
        if (t->child[0] && t->child[0]->isArray) {
            char buf[256];
            snprintf(buf, sizeof(buf), "Cannot return an array.");
            semanticError(t, buf);
        }
    }
    else if (t->nodekind == StmtK && t->subkind.stmt == CompoundK) {
        st->leave();
    }

    // Check for unused variables at the end of their scope
    if (t->nodekind == DeclK) {
        if ((t->subkind.decl == VarK || t->subkind.decl == ParamK) && !t->isUsed) {
            char buf[256];
            snprintf(buf, sizeof(buf), "The variable '%s' seems not to be used.", t->attr.name);
            semanticWarning(t, buf);
        }
    }
}

// Helper function for checking binary operators
static void checkBinaryOp(TreeNode *t, SymbolTable *st, ExpType requiredType, ExpType resultType, bool allowArrays) {
    if (!t->child[0] || !t->child[1]) return;
    
    ExpType t1 = t->child[0]->expType;
    ExpType t2 = t->child[1]->expType;
    const char* opStr = opToString(t->attr.op);
    char buf[256];
    
    // Check arrays first
    if (!allowArrays && (t->child[0]->isArray || t->child[1]->isArray)) {
        snprintf(buf, sizeof(buf), "The operation '%s' does not work with arrays.", opStr);
        semanticError(t, buf);
        t->expType = resultType; // Set type even with error to reduce cascading
        return;
    }
    
    // Check operand types
    bool typesMatch = (requiredType == UndefinedType) ? (t1 == t2) : 
                     (t1 == requiredType && t2 == requiredType);
    
    if (!typesMatch) {
        if (t->attr.op == ADDASS || t->attr.op == SUBASS || 
            t->attr.op == MULASS || t->attr.op == DIVASS) {
            // Compound assignment operators should say "same type"
            snprintf(buf, sizeof(buf), "'%s' requires operands of the same type but lhs is type %s and rhs is type %s.", 
                    opStr, typeToString(t1), typeToString(t2));
        } 
        else if (requiredType == UndefinedType) { // Types need to match but don't
            snprintf(buf, sizeof(buf), "'%s' requires operands of the same type but lhs is type %s and rhs is type %s.", 
                    opStr, typeToString(t1), typeToString(t2));
        }
        else if (t1 != requiredType && t2 != requiredType) { // Both wrong type
            snprintf(buf, sizeof(buf), "'%s' requires operands of type %s.", 
                    opStr, typeToString(requiredType));
        }
        else if (t1 != requiredType && t2 == requiredType) { // Error on left operand
            snprintf(buf, sizeof(buf), "'%s' requires operands of type %s but lhs is of type %s.", 
                    opStr, typeToString(requiredType), typeToString(t1));
        } 
        else if (t2 != requiredType && t1 == requiredType) { // Error on right operand
            snprintf(buf, sizeof(buf), "'%s' requires operands of type %s but rhs is of type %s.", 
                    opStr, typeToString(requiredType), typeToString(t2));
        }
        semanticError(t, buf);
    }
    
    t->expType = resultType;
}

// Helper for unary operators
static void checkUnaryOp(TreeNode *t, ExpType requiredType, ExpType resultType, bool allowArrays) {
    if (!t->child[0]) return;
    
    const char* opStr = opToString(t->attr.op);
    char buf[256];
    
    if (!allowArrays && t->child[0]->isArray) {
        snprintf(buf, sizeof(buf), "The operation '%s' does not work with arrays.", opStr);
        semanticError(t, buf);
    } 
    else if (t->child[0]->expType != requiredType) {
        snprintf(buf, sizeof(buf), "Unary '%s' requires an operand of type %s but was given type %s.", 
                opStr, typeToString(requiredType), typeToString(t->child[0]->expType));
        semanticError(t, buf);
    }
    
    t->expType = resultType;
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