#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

extern void exporta(void *arvore);
extern void libera(void *arvore);

void remove_no(void *arvore);

tree_node_tipo* insere(tree_node_tipo** pai, tree_node_tipo filhos[]);

tree_node_tipo* cria_nodo(valor_lexico_tipo* valor_lexico, Nodos tipo);

tree_node_tipo* cria_nodo_manual(int linha, char* nome, Token tipo_token, Nodos tipo_nodo);