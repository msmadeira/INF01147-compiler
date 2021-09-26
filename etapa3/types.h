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