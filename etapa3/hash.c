#include "hash.h"

HASH_NODE*Table[HASH_SIZE];

void hashInit(void){
    int i;
    for(i = 0; i < HASH_SIZE; ++i)
    {
        Table[i] = 0;
    }
}

int hashAddress(char *text){
    int addr = 1;
    int i;
    for(i = 0; i < strlen(text); ++i)
    {
        addr = (addr * text[i]) % HASH_SIZE + 1;
    }

    return addr - 1;
}

HASH_NODE *hashFind(char *text){
    HASH_NODE *node;
    int addr = hashAddress(text);
    for(node = Table[addr]; node; node = node->next)
    {
        if(strcmp(node->text, text) == 0)
        {
            return node;
        }
    }

    return 0;
}

void hashPrint(void){
    int i;
    HASH_NODE *node;
    for(i = 0; i < HASH_SIZE; ++i)
    {
        for(node = Table[i]; node; node = node->next)
        {
            printf("Tabela[%d]: ", i);
            printf("Type: %d ", node->type);
            printf("Text: %s\n", node->text);
        }
    }
}

HASH_NODE *hashInsert(char *text, int type){
    HASH_NODE *newNode;
    int addr = hashAddress(text);

    if((newNode = hashFind(text)) != 0)
    {
        return newNode;
    }

    newNode = (HASH_NODE*) calloc(1, sizeof(HASH_NODE));
    newNode->type = type;
    newNode->text = (char*) calloc(strlen(text) + 1, sizeof(char));
    strcpy(newNode->text, text);
    newNode->next = Table[addr];
    Table[addr] = newNode;

    return newNode;
}