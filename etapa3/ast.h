#include "hash.h"

#ifndef AST_HEADER
#define AST_HEADER

#define MAX_SONS 4

#define AST_SYMBOL 0
#define AST_ADD 1
#define AST_SUB 2
#define AST_MUL 3
#define AST_DIV 4
#define AST_L 5
#define AST_G 6
#define AST_LE 7
#define AST_GE 8
#define AST_EQ 9 
#define AST_NE 10
#define AST_AND 11
#define AST_OR 12
#define AST_PARAM 13
#define AST_FUNDEF 14
#define AST_FUNCALL 15
#define AST_BYTE 16
#define AST_SHORT 17
#define AST_LONG 18
#define AST_FLOAT 19
#define AST_DOUBLE 20
#define AST_PARENTHESIS 21
#define AST_BRACES 22
#define AST_ATR 23
#define AST_AATR 24
#define AST_AACC 25
#define AST_VARINI 26
#define AST_ARRINI 27
#define AST_PRINT 28
#define AST_PRINTL 29
#define AST_READ 30
#define AST_RETURN 31
#define AST_WHILE 32
#define AST_IF 33
#define AST_ELSE 34
#define AST_NOT 35
#define AST_CMDL 100
#define AST_DFNL 101
#define AST_PARL 102
#define AST_INTL 104
#define AST_QUOTES 105

typedef struct astnode{
    int type;
    HASH_NODE *symbol;
    struct astnode *son[MAX_SONS];
} AST;

AST *createAST(int type, HASH_NODE *symbol, AST* son_0, AST* son_1, AST* son_2, AST* son_3);

void astPrint(AST *node, int level);

#endif