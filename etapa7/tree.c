#include "tree.h"
a_node* insertNode(a_node* child,lexic_val_type* lexValue)
{
	a_node* node;
	node = (a_node*)malloc(sizeof(a_node));
	node->tipo_no = NO_LEX;
	node->is_arg = false;
	node->child = child;
	node->nextBro = NULL;
	node->lexValue = lexValue;	
	return node;
}

a_node* insertChild(a_node* tree, a_node* child)
{
	a_node* aux;
	if(tree != NULL)
	{
		aux = tree->child;
		if(aux == NULL)
		{
			tree->child = child;
		}
		else
		{
			while(aux->nextBro != NULL)
			{
				aux = aux->nextBro;
			}
			aux->nextBro = child;
		}
	}else
	{
		tree=child;
	}
	return tree;
}

a_node* insertChildB(a_node* tree, a_node* child)
{
	a_node* aux;
	aux = tree->child;
	if(aux == NULL)
	{
		tree->child = child;
	}
	else{
		do
		{
			while(aux->nextBro != NULL)
			{
				aux = aux->nextBro;
			}
			if(aux->child != NULL)
			{
				aux = aux->child;
			}
		}while(aux->child != NULL);
		while(aux->nextBro != NULL)
		{
			aux = aux->nextBro;
		}
		aux->nextBro = child;
	}
	return tree;
}

void updateST(a_node* node, int semType)
{
	node->typeSemVal = semType;
}
a_node* insertNodeT( a_node* child,lexic_val_type* lexValue, int tipo_nodo)
{
	a_node* node;
	node = (a_node*)malloc(sizeof(a_node));
	node->tipo_no = tipo_nodo;
	node->is_arg = false;
	node->child = child;
	node->nextBro = NULL;
	node->lexValue = lexValue;	
	return node;

}

a_node* insere_filho_prefix(a_node* tree, a_node* child){
	a_node* aux;
	a_node* filho_atual;
	filho_atual = tree->child;
	if(filho_atual == NULL)
	{
		tree->child = child;
	}
	else
	{
		aux = child;
		while(aux->nextBro != NULL)
		{
			aux = aux->nextBro;
		}
		aux->nextBro = filho_atual;
		tree->child = child;
	}
	return tree;
}

void exporta(void *tree)
{
	if(tree !=NULL)
	{
		exportaAux((a_node*) tree);
		exportal((a_node*) tree);
	}
}
void exportal(a_node *tree){
	if(tree->nextBro != NULL)
	{
		exportal(tree->nextBro);
	}
	if(tree->child!=NULL)
	{
		exportal(tree->child);
	}
	if( tree->lexValue != NULL)
	{
		printf("semType: %d -> ", tree->typeSemVal);
		printf("%p [label =\"",tree);
		if(tree->tipo_no == NO_FUN_CALL)
		{
			printf("call ");
		}
		printa(tree->lexValue);
	}
}
void exportaAux(a_node *tree){
	if(tree != NULL)
	{
		a_node *aux;
		aux = tree->child;
		while(aux != NULL)
		{
			printf("%p, %p\n",tree,aux);
			aux = aux->nextBro;
		}
		if(tree->nextBro != NULL)
		{
			exportaAux(tree->nextBro);
		}
		if(tree->child!=NULL)
		{
			exportaAux(tree->child);
		}
	}

}
void libera (a_node *tree){
	if(tree != NULL)
	{
		if(tree->nextBro != NULL)
		{
			libera(tree->nextBro);
		}
		if(tree->child!=NULL)
		{
			libera(tree->child);
		}
		if(tree->lexValue !=NULL)
		{
			freeV(tree->lexValue);
		}
		free(tree);	
	}
}
void printa(lexic_val_type* lexValue){
	switch(lexValue->type)
	{
		case (TIPO_LIT):
			switch(lexValue->value_type)
			{
				case(LIT_TIPO_INT):
					printf("%d\"]\n",lexValue->tk_value.vInt);
					break;
				case(LIT_TIPO_FLOAT):
					printf("%f\"]\n",lexValue->tk_value.vFloat);
					break;
				case(LIT_TIPO_BOOL):
					if(lexValue->tk_value.vBool)
					{
						printf("true\"]\n");
					}
					else
					{
						printf("false\"]\n");
					}
					break;
				case(LIT_TIPO_STRING):
					printf("%s\"]\n",lexValue->tk_value.vStr);
					break;
				case(LIT_TIPO_CHAR):
					printf("%s\"]\n",lexValue->tk_value.vChar);
					break;
				default:
					printf("%s\"]\n",lexValue->tk_value.vStr);
					break;
			}
			break;
	    case (TIPO_CHAR_ESP):
	    case (TIPO_OP_CMP):
		case (TIPO_ID):
		default:
			printf("%s\"]\n",lexValue->tk_value.vStr);
			break;
	}
}

void freeV(lexic_val_type* lexValue){
	if(lexValue != NULL)
	{
		switch(lexValue->value_type)
		{
			case(LIT_TIPO_STRING):
				free(lexValue->tk_value.vStr);
				break;
			case(LIT_TIPO_CHAR):
				free(lexValue->tk_value.vChar);
				break;
			case (NOT_LIT):
				free(lexValue->tk_value.vStr);
			default:
				break;
		}
		free(lexValue);
	}

}