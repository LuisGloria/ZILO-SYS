#include "lexer.h"
#include "parser.h"
#include "codegen.h"

int main() {
    const char *src =
    "spawn genome\n"
    "evolve\n"
    "print genome\n";

    Lexer lex;
    lexer_init(&lex, src);

    Parser p;
    parser_init(&p, &lex);

    AST *ast = parse_program(&p);
    codegen(ast);

    return 0;
}
