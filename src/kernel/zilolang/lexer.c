#include "lexer.h"
#include "zlibs/zmem.h"
#include "zlibs/zstr.h"
#include "zlibs/zctype.h"

static void advance(Lexer *l) {
    l->pos++;
    l->current = l->src[l->pos];
}

void lexer_init(Lexer *l, const char *src) {
    l->src = src;
    l->pos = 0;
    l->current = src[0];
}

static void skip_ws(Lexer *l) {
    while (zisspace(l->current)) advance(l);
}

static int is_ident(char c) {
    return zisalpha(c) || c == '_';
}

Token lexer_next(Lexer *l) {
    skip_ws(l);

    Token t;
    zmemset(&t, 0, sizeof(Token));

    if (l->current == '\0') {
        t.type = TOK_EOF;
        return t;
    }

    // numbers
    if (zisdigit(l->current)) {
        int val = 0;
        while (zisdigit(l->current)) {
            val = val * 10 + (l->current - '0');
            advance(l);
        }
        t.type = TOK_NUMBER;
        t.value = val;
        return t;
    }

    // identifiers / keywords
    if (is_ident(l->current)) {
        int i = 0;
        while (is_ident(l->current)) {
            t.text[i++] = l->current;
            advance(l);
        }
        t.text[i] = 0;

        if (!zstrcmp(t.text, "spawn")) t.type = TOK_KEYWORD_SPAWN;
        else if (!zstrcmp(t.text, "evolve")) t.type = TOK_KEYWORD_EVOLVE;
        else if (!zstrcmp(t.text, "fitness")) t.type = TOK_KEYWORD_FITNESS;
        else if (!zstrcmp(t.text, "pressure")) t.type = TOK_KEYWORD_PRESSURE;
        else if (!zstrcmp(t.text, "print")) t.type = TOK_KEYWORD_PRINT;
        else t.type = TOK_IDENT;

        return t;
    }

    // symbols
    switch (l->current) {
        case '[': t.type = TOK_LBRACK; advance(l); return t;
        case ']': t.type = TOK_RBRACK; advance(l); return t;
        case '(': t.type = TOK_LPAREN; advance(l); return t;
        case ')': t.type = TOK_RPAREN; advance(l); return t;
        case '+': t.type = TOK_PLUS; advance(l); return t;
        case '-': t.type = TOK_MINUS; advance(l); return t;
        case '*': t.type = TOK_STAR; advance(l); return t;
        case '/': t.type = TOK_SLASH; advance(l); return t;
        case '=': t.type = TOK_ASSIGN; advance(l); return t;
    }

    advance(l);
    t.type = TOK_EOF;
    return t;
}
