#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

AST *createAST(int type, HASH_NODE *symbol, AST* son_0, AST* son_1, AST* son_2, AST* son_3){
    AST* newNode;
    
    newNode = (AST*) calloc(1, sizeof(AST));
    newNode->type = type;
    newNode->symbol = symbol;
    newNode->son[0] = son_0;
    newNode->son[1] = son_1;
    newNode->son[2] = son_2;
    newNode->son[3] = son_3;
    
    return newNode;
}

void astPrint(AST *node, int level){
    int i = 0;
    
    if (node == 0)
        return;
    
    for(i=0; i < level; ++i)
        fprintf(stderr, "  ");
    
    fprintf(stderr, "ast(");
    switch(node->type){
        case AST_SYMBOL: fprintf(stderr, "AST_SYMBOL"); break;
		case AST_ADD: fprintf(stderr, "AST_ADD"); break;
		case AST_SUB: fprintf(stderr, "AST_SUB"); break;
		case AST_MUL: fprintf(stderr, "AST_MUL"); break;
		case AST_DIV: fprintf(stderr, "AST_DIV"); break;
		case AST_L: fprintf(stderr, "AST_L"); break;
		case AST_G: fprintf(stderr, "AST_G"); break;
		case AST_LE: fprintf(stderr, "AST_LE"); break;
		case AST_GE: fprintf(stderr, "AST_GE"); break;		
		case AST_EQ: fprintf(stderr, "AST_EQ"); break;
		case AST_NE: fprintf(stderr, "AST_NE"); break;
		case AST_AND: fprintf(stderr, "AST_AND"); break;
		case AST_OR: fprintf(stderr, "AST_OR"); break;		
		case AST_ATR: fprintf(stderr, "AST_ATR"); break;
		case AST_PARAM: fprintf(stderr, "AST_PARAM"); break;
		case AST_FUNDEF: fprintf(stderr, "AST_FUNDEF"); break;
		case AST_FUNCALL: fprintf(stderr, "AST_FUNCALL"); break;
		case AST_CMDL: fprintf(stderr, "AST_CMDL"); break;
		case AST_DFNL: fprintf(stderr, "AST_DFNL"); break;		
		case AST_PARL: fprintf(stderr, "AST_PARL"); break;
		case AST_INTL: fprintf(stderr, "AST_INTL"); break;
		case AST_BYTE: fprintf(stderr, "AST_BYTE"); break;
		case AST_SHORT: fprintf(stderr, "AST_SHORT"); break;
		case AST_LONG: fprintf(stderr, "AST_LONG"); break;		
		case AST_FLOAT: fprintf(stderr, "AST_FLOAT"); break;
		case AST_DOUBLE: fprintf(stderr, "AST_DOUBLE"); break;
		case AST_PRINT: fprintf(stderr, "AST_PRINT"); break;
		case AST_PRINTL: fprintf(stderr, "AST_PRINTL"); break;
		case AST_READ: fprintf(stderr, "AST_READ"); break;	
		case AST_RETURN: fprintf(stderr, "AST_RETURN"); break;
		case AST_IF: fprintf(stderr, "AST_IF"); break;
		case AST_WHILE: fprintf(stderr, "AST_WHILE"); break;
		case AST_PARENTHESIS: fprintf(stderr, "AST_PARENTHESIS"); break;
		case AST_BRACES: fprintf(stderr, "AST_BRACES"); break;
		case AST_AATR: fprintf(stderr, "AST_AATR"); break;
		case AST_AACC: fprintf(stderr, "AST_AACC"); break;		
		case AST_VARINI: fprintf(stderr, "AST_VARINI"); break;
		case AST_ARRINI: fprintf(stderr, "AST_ARRINI"); break;
		case AST_NOT: fprintf(stderr, "AST_NOT"); break;
		case AST_ELSE: fprintf(stderr, "AST_ELSE"); break;
		default: fprintf(stderr, "Node Type: %d", node->type); break;
    }
    
    if(node->symbol != 0)
        fprintf(stderr, ",%s\n", node->symbol->text);
    else
        fprintf(stderr, ",0\n");
  
    for (i=0; i < MAX_SONS; i++)
        astPrint(node->son[i], level+1);
}