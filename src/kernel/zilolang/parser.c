#include "parser.h"
#include "zlibs/ztypes.h"
#include "zlibs/zstr.h"

static void advance(Parser *p) {
    p->current = lexer_next(p->lexer);
}

void parser_init(Parser *p, Lexer *l) {
    p->lexer = l;
    advance(p);
}

static AST *parse_stmt(Parser *p) {
    if (p->current.type == TOK_KEYWORD_SPAWN) {
        advance(p);
        AST *n = ast_new(AST_SPAWN);
        zstrcpy(n->name, p->current.text);
        advance(p);
        return n;
    }

    if (p->current.type == TOK_KEYWORD_EVOLVE) {
        advance(p);
        AST *n = ast_new(AST_EVOLVE);
        return n;
    }

    if (p->current.type == TOK_KEYWORD_PRINT) {
        advance(p);
        AST *n = ast_new(AST_PRINT);
        zstrcpy(n->name, p->current.text);
        advance(p);
        return n;
    }

    return NULL;
}

AST *parse_program(Parser *p) {
    return parse_stmt(p);
}
