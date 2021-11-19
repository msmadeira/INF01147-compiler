#include <stdlib.h>
#include <stdio.h>
#include "util.h"

struct tree
{
	int tipo_no;
	bool is_arg;
	int typeSemVal; 
	lexic_val_type* lexValue;
	struct tree* child;
	struct tree* nextBro;
};
typedef struct tree a_node;

a_node* insertNode(a_node* child, lexic_val_type* lexValue);
a_node* insertChild(a_node* tree, a_node* child);
void printa(lexic_val_type* lexValue);
void exportal(a_node *tree);
void exportaAux(a_node *tree);
void exporta (void *tree);
void libera (a_node *tree);
void freeV(lexic_val_type* lexValue);
void updateST(a_node* node, int semType);
a_node* insertChildB(a_node* tree, a_node* child);
a_node* insertNodeT( a_node* child,lexic_val_type* lexValue, int tipo_nodo);
