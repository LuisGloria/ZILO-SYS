#ifndef ZILO_AST_H
#define ZILO_AST_H

typedef enum {
    AST_SPAWN,
    AST_EVOLVE,
    AST_FITNESS,
    AST_PRESSURE,
    AST_PRINT
} ASTType;

typedef struct AST {
    ASTType type;
    char name[64];

    int value;

    struct AST *left;
    struct AST *right;
} AST;

AST *ast_new(ASTType type);

#endif
