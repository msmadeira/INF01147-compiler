typedef enum Token
{
    LITERAL,
    IDENTIFICADOR,
    COMPOSTO,
    ESPECIAL
} Token;

union Valor_Token
{
    // definição de valor para nomes_gerais
    char* nome_geral;
    // definição de valores para cada tipo de literal
    int inteiro;
    float ponto_flutuante;
    char* palavra;
    char caractere;
    int booleano;
};

typedef struct valor_lexico
{
    int numero_linha;
    int tipo_token;
    union Valor_Token valor_token;

} valor_lexico_tipo;

typedef enum Nodos
{
    LISTA_FUNCAO,
    LISTA_COMANDOS,
    LISTA_EXPRESSOES,
    ID,
    INICIA_VAR,
    ATRIBUICAO_VAR,
    ENTRADA_SAIDA,
    CHAMADA_FUNCAO,
    COMANDO_SHIFT,
    COMANDO_RETURN,
    BREAK_CONTINUE,
    COMANDO_IF,
    COMANDO_FOR,
    COMANDO_WHILE,
    VETOR_ACESSO,
    LITERAL_INT,
    LITERAL_FLOAT,
    LITERAL_STRING,
    LITERAL_CHAR,
    LITERAL_BOOL,
    DELIMITADOR
} Nodos;

typedef struct tree_node {
    valor_lexico_tipo* valor_lexico;
    Nodos tipo;
    struct tree_node *filhos[4];
    struct tree_node *prox_comando; // usado para distinção em blocos de comando if/else

} tree_node_tipo;