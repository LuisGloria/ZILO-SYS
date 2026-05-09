#include "codegen.h"
#include "zlibs/zio.h"

void codegen(AST *node) {
    if (!node) return;

    switch (node->type) {
        case AST_SPAWN:
            zprintf("SPAWN ARRAY: %s\n", node->name);
            break;

        case AST_EVOLVE:
            zprintf("EVOLVE STEP EXECUTED\n");
            break;

        case AST_PRINT:
            zprintf("PRINT: %s\n", node->name);
            break;

        default:
            zprintf("UNKNOWN AST NODE\n");
    }
}
