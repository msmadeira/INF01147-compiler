#include "ast.h"

extern void exporta(void *arvore) {
}

extern void libera(void *arvore) {
}

void remove_no(void *arvore) {
    // Ponteiros auxiliares
    // node_t *aux_1 = NULL;
    // node_t *aux_2 = NULL;
    // node_t *swap = NULL;

    // // So remove nodos que existem
    // if (arvore != NULL)
    // {
    //     // Se este nodo tem filhos
    //     if (((node_t *)arvore)->filhos != NULL)
    //     {
    //         // Inicializa o ponteiro auxiliar 1 no primeiro filho
    //         aux_1 = ((node_t *)arvore)->filhos;

    //         // Inicializa o ponteiro auxiliar 2 no segundo filho
    //         aux_2 = aux_1->irmao;

    //         // Percorre todos os filhos removendo de forma recursiva
    //         while (aux_2 != NULL || aux_1 != NULL)
    //         {
    //             // Remove o filho i
    //             if (aux_1 != NULL)
    //                 remove_no(aux_1);

    //             // Atualiza o ponteiro para o filho i+1
    //             if (aux_2 != NULL)
    //                 aux_1 = aux_2->irmao;
    //             else
    //                 aux_1 = NULL;

    //             // Troca os ponteiros
    //             swap = aux_1;
    //             aux_1 = aux_2;
    //             aux_2 = swap;
    //         }
    //     }

    //     // Se este nodo tem comandos subsequentes, tambem remove
    //     if (((node_t *)arvore)->prox_comando != NULL)
    //         remove_no(((node_t *)arvore)->prox_comando);

    //     // Salva o primeiro irmao desta arvore
    //     aux_1 = ((node_t *)arvore)->irmao;

    //     // Libera a memoria usada para o valor lexico deste nodo (alocada em scanner.l)
    //     libera_valor_lexico(((node_t *)arvore)->valor_lexico, ((node_t *)arvore)->tipo);

    //     // Remove o nodo informado
    //     free(arvore);
    // }
}

tree_node_tipo* insere(tree_node_tipo** pai, tree_node_tipo filhos[]) {
	// node_t *aux = (*nodo_pai)->filhos;
	(*pai)->filhos[0] = &filhos[0];
	(*pai)->filhos[1] = &filhos[1];
	(*pai)->filhos[2] = &filhos[2];
	(*pai)->filhos[3] = &filhos[3];

	return *(pai);
}

tree_node_tipo* cria_nodo(valor_lexico_tipo* valor_lexico, Nodos tipo) {
    tree_node_tipo *nodo = (tree_node_tipo *)malloc(sizeof(tree_node_tipo));
    memset(nodo, 0, sizeof(tree_node_tipo));

    nodo->valor_lexico = valor_lexico;
    nodo->tipo = tipo;
	nodo->filhos[0] = NULL;
	nodo->filhos[1] = NULL;
	nodo->filhos[2] = NULL;
	nodo->filhos[3] = NULL;

    return nodo;
}

tree_node_tipo* cria_nodo_manual(int linha, char* nome, Token tipo_token, Nodos tipo_nodo) {
    valor_lexico_tipo *valor_lexico = (valor_lexico_tipo *)malloc(sizeof(valor_lexico_tipo));
    memset(valor_lexico, 0, sizeof(valor_lexico_tipo));
	
    valor_lexico->numero_linha = linha;
    valor_lexico->valor_token.nome_geral = strdup(nome);
    valor_lexico->tipo_token = tipo_token;

    return cria_nodo(valor_lexico, tipo_nodo);
}