#ifndef ZILO_PARSER_H
#define ZILO_PARSER_H

#include "lexer.h"
#include "ast.h"

typedef struct {
    Lexer *lexer;
    Token current;
} Parser;

void parser_init(Parser *p, Lexer *l);
AST *parse_program(Parser *p);

#endif
