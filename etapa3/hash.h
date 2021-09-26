#define HASH_SIZE 9999

typedef struct hash_node{
    int type;
    char *text;
    struct hash_node *next;
} HASH_NODE;

HASH_NODE* Table[HASH_SIZE];

void hashInit();
void hashPrint();

int hashAddress(char* text);

HASH_NODE* hashFind(char* text);
HASH_NODE* hashInsert(int type, char* text);