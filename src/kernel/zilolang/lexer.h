#ifndef ZILO_LEXER_H
#define ZILO_LEXER_H

typedef enum {
    TOK_EOF,
    TOK_IDENT,
    TOK_NUMBER,

    TOK_LBRACK,
    TOK_RBRACK,
    TOK_LPAREN,
    TOK_RPAREN,

    TOK_ASSIGN,
    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_SLASH,

    TOK_KEYWORD_SPAWN,
    TOK_KEYWORD_EVOLVE,
    TOK_KEYWORD_FITNESS,
    TOK_KEYWORD_PRESSURE,
    TOK_KEYWORD_PRINT
} TokenType;

typedef struct {
    TokenType type;
    char text[64];
    int value;
} Token;

typedef struct {
    const char *src;
    int pos;
    char current;
} Lexer;

void lexer_init(Lexer *l, const char *src);
Token lexer_next(Lexer *l);

#endif
