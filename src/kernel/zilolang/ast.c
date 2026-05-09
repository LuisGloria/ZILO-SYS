#include "ast.h"
#include "zlibs/zmem.h"
#include "zlibs/zalloc.h"

AST *ast_new(ASTType type) {
    AST *n = (AST*)zmalloc(sizeof(AST));
    zmemset(n, 0, sizeof(AST));
    n->type = type;
    return n;
}
