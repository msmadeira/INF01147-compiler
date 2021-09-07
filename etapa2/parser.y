%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
int yyerror (char const *s);
%}

%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_CHAR
%token TK_PR_STRING
%token TK_PR_IF
%token TK_PR_THEN
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_DO
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
%token TK_PR_CONST
%token TK_PR_STATIC
%token TK_PR_FOREACH
%token TK_PR_FOR
%token TK_PR_SWITCH
%token TK_PR_CASE
%token TK_PR_BREAK
%token TK_PR_CONTINUE
%token TK_PR_CLASS
%token TK_PR_PRIVATE
%token TK_PR_PUBLIC
%token TK_PR_PROTECTED
%token TK_PR_END
%token TK_PR_DEFAULT
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_OC_SL
%token TK_OC_SR
%token TK_LIT_POSINT
%token TK_LIT_NEGINT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_LIT_CHAR
%token TK_LIT_STRING
%token TK_IDENTIFICADOR
%token TK_ESPECIAL_OPPAR;
%token TK_ESPECIAL_CLPAR;
%token TK_ESPECIAL_OPBRACKETS
%token TK_ESPECIAL_CLBRACKETS
%token TK_ESPECIAL_COMMA
%token TK_ESPECIAL_SEMICOLON
%token TK_ESPECIAL_COLON;
%token TK_ESPECIAL_EXCLAMATION
%token TK_ESPECIAL_ADD;
%token TK_ESPECIAL_SUB;
%token TK_ESPECIAL_MULT;
%token TK_ESPECIAL_DIV;
%token TK_ESPECIAL_LTHAN;
%token TK_ESPECIAL_GTHAN;
%token TOKEN_ERRO
%start programa

%%

programa : funcao | declaracao_global | /* vazio */;
funcao : programa "funcao" { printf("funcao \n"); };

declaracao_global : programa static_opcional tipo declaracao_global_nomes TK_ESPECIAL_SEMICOLON { printf("declaracao_global \n"); };
declaracao_global_nomes : declaracao_global_nome_variavel declaracao_global_sequencia_nomes;
declaracao_global_sequencia_nomes : TK_ESPECIAL_COMMA declaracao_global_nome_variavel declaracao_global_sequencia_nomes | /* vazio */;
declaracao_global_nome_variavel : TK_IDENTIFICADOR vetor;

declaracao_local : static_opcional const_opcional tipo declaracao_local_nomes TK_ESPECIAL_SEMICOLON { printf("declaracao_local \n"); };
declaracao_local_nomes : declaracao_local_variavel declaracao_local_sequencia_nomes;
declaracao_local_sequencia_nomes : TK_ESPECIAL_COMMA declaracao_local_variavel declaracao_local_sequencia_nomes | /* vazio */;
declaracao_local_variavel : TK_IDENTIFICADOR declaracao_local_inicializacao;
declaracao_local_inicializacao : TK_OC_LE declaracao_local_valor | /* vazio */;
declaracao_local_valor : TK_IDENTIFICADOR | literal;

const_opcional : TK_PR_CONST |  /* vazio */;
static_opcional : TK_PR_STATIC |  /* vazio */;
tipo : TK_PR_INT | TK_PR_FLOAT | TK_PR_CHAR | TK_PR_BOOL | TK_PR_STRING;
literal : TK_LIT_POSINT | TK_LIT_NEGINT | TK_LIT_FLOAT | TK_LIT_FALSE | TK_LIT_TRUE | TK_LIT_CHAR | TK_LIT_STRING;
vetor : TK_ESPECIAL_OPBRACKETS TK_LIT_POSINT TK_ESPECIAL_CLBRACKETS | /* vazio */;

chamada_funcao : TK_IDENTIFICADOR TK_ESPECIAL_OPPAR TK_ESPECIAL_CLPAR | TK_IDENTIFICADOR TK_ESPECIAL_OPPAR lista_argumentos TK_ESPECIAL_CLPAR;
lista_argumentos : argumento | argumento TK_ESPECIAL_COMMA lista_argumentos;
argumento : expressao;

expressao : TK_IDENTIFICADOR 
    | TK_IDENTIFICADOR TK_ESPECIAL_OPBRACKETS expressao TK_ESPECIAL_CLBRACKETS
    | TK_IDENTIFICADOR TK_ESPECIAL_OPPAR lista_parametros TK_ESPECIAL_CLPAR
    | literal
    | expressao TK_ESPECIAL_ADD expressao 
    | expressao TK_ESPECIAL_SUB expressao 
    | expressao TK_ESPECIAL_MULT expressao 
    | expressao TK_ESPECIAL_DIV expressao
    | expressao TK_ESPECIAL_LTHAN expressao
    | expressao TK_ESPECIAL_GTHAN expressao 
    | TK_ESPECIAL_EXCLAMATION TK_ESPECIAL_OPPAR expressao TK_ESPECIAL_CLPAR
    | expressao OPERATOR_LE expressao
    | expressao OPERATOR_GE expressao 
    | expressao OPERATOR_EQ expressao
    | expressao OPERATOR_NE expressao
    | expressao OPERATOR_AND expressao
    | expressao OPERATOR_OR expressao
    | TK_ESPECIAL_OPPAR expressao TK_ESPECIAL_CLPAR
;

lista_parametros : parametro parametros_fim | /* vazio */;
parametros_fim : TK_ESPECIAL_COMMA parametro parametros_fim | /* vazio */;
parametro : TK_IDENTIFIER TK_ESPECIAL_COLON tipo | TK_IDENTIFIER | literal;
%%

int yyerror(char const *s) {
    printf("%s\n", s);
    return 1;
}
