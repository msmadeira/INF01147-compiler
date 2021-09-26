%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

int yylex(void);
int yyerror (char const *s);
%}

%union{
    tree_node_tipo* no;
    valor_lexico_tipo* valor_lexico;
}

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
%token<valor_lexico> TK_OC_LE
%token<valor_lexico> TK_OC_GE
%token<valor_lexico> TK_OC_EQ
%token<valor_lexico> TK_OC_NE
%token<valor_lexico> TK_OC_AND
%token<valor_lexico> TK_OC_OR
%token<valor_lexico> TK_OC_SL
%token<valor_lexico> TK_OC_SR
%token<valor_lexico> TK_LIT_POSINT
%token<valor_lexico> TK_LIT_NEGINT
%token<valor_lexico> TK_LIT_FLOAT
%token<valor_lexico> TK_LIT_FALSE
%token<valor_lexico> TK_LIT_TRUE
%token<valor_lexico> TK_LIT_CHAR
%token<valor_lexico> TK_LIT_STRING
%token<valor_lexico> TK_IDENTIFICADOR
%token<valor_lexico> TK_ESPECIAL_OPPAR;
%token<valor_lexico> TK_ESPECIAL_CLPAR;
%token<valor_lexico> TK_ESPECIAL_OPBRACKETS
%token<valor_lexico> TK_ESPECIAL_CLBRACKETS
%token<valor_lexico> TK_ESPECIAL_OPCURLY
%token<valor_lexico> TK_ESPECIAL_CLCURLY
%token<valor_lexico> TK_ESPECIAL_COMMA
%token<valor_lexico> TK_ESPECIAL_SEMICOLON
%token<valor_lexico> TK_ESPECIAL_COLON;
%token<valor_lexico> TK_ESPECIAL_EXCLAMATION;
%token<valor_lexico> TK_ESPECIAL_INTERROGATION;
%token<valor_lexico> TK_ESPECIAL_ADD;
%token<valor_lexico> TK_ESPECIAL_SUB;
%token<valor_lexico> TK_ESPECIAL_MULT;
%token<valor_lexico> TK_ESPECIAL_DIV;
%token<valor_lexico> TK_ESPECIAL_LTHAN;
%token<valor_lexico> TK_ESPECIAL_GTHAN;
%token<valor_lexico> TK_ESPECIAL_EQUALS;
%token<valor_lexico> TK_ESPECIAL_REM; 
%token<valor_lexico> TK_ESPECIAL_BITOR;
%token<valor_lexico> TK_ESPECIAL_BITAND;
%token<valor_lexico> TK_ESPECIAL_POW;
%token<valor_lexico> TK_ESPECIAL_HASH;
%token TOKEN_ERRO
%start programa

%type<no> bloco_comandos
entrada
saida
bloco_comandos_corpo
bloco_comandos_opcoes
condicional_if
expressao
atribuicao
declaracao_local
funcao_parametro
operacao_shift
vetor
iterativa_for
iterativa_while
op_binarios
chamada_funcao
declaracao_global
literal
funcao
expressao_unaria
funcao_parametros
programa
expressao_binaria
operacao_break
operacao_continue

%%

programa : funcao {$$ = $2;}
    | declaracao_global programa {arvore = $1; insere(&$1,$2);}
    | /* vazio */ {$$ = NULL;};

funcao : funcao_cabecalho bloco_comandos_estrutura;
funcao_cabecalho : static_opcional tipo TK_IDENTIFICADOR TK_ESPECIAL_OPPAR funcao_parametros TK_ESPECIAL_CLPAR;
funcao_parametros : funcao_parametro 
    | funcao_parametro TK_ESPECIAL_COMMA funcao_parametros 
    | /* vazio */ {};
funcao_parametro : const_opcional tipo TK_IDENTIFICADOR {};

bloco_comandos : bloco_comandos_estrutura TK_ESPECIAL_SEMICOLON {};
bloco_comandos_estrutura : TK_ESPECIAL_OPCURLY bloco_comandos_corpo TK_ESPECIAL_CLCURLY;
bloco_comandos_corpo : bloco_comandos_corpo bloco_comandos_opcoes 
    | /* vazio */ {};
bloco_comandos_opcoes : declaracao_local
    | bloco_comandos
    | entrada
    | atribuicao
    | saida
    | chamada_funcao
    | operacao_shift
    | retorno {}
    | operacao_break
    | operacao_continue
    | condicional_if
    | iterativa_for
    | iterativa_while;

declaracao_global : programa static_opcional tipo declaracao_global_nomes TK_ESPECIAL_SEMICOLON;
declaracao_global_nomes : declaracao_global_nome_variavel declaracao_global_sequencia_nomes;
declaracao_global_sequencia_nomes : TK_ESPECIAL_COMMA declaracao_global_nome_variavel declaracao_global_sequencia_nomes | /* vazio */;
declaracao_global_nome_variavel : TK_IDENTIFICADOR vetor;

declaracao_local : static_opcional const_opcional tipo declaracao_local_nomes TK_ESPECIAL_SEMICOLON {};
declaracao_local_nomes : declaracao_local_variavel declaracao_local_sequencia_nomes;
declaracao_local_sequencia_nomes : TK_ESPECIAL_COMMA declaracao_local_variavel declaracao_local_sequencia_nomes | /* vazio */;
declaracao_local_variavel : TK_IDENTIFICADOR declaracao_local_inicializacao;
declaracao_local_inicializacao : TK_OC_LE valor | /* vazio */;

entrada : TK_PR_INPUT TK_IDENTIFICADOR TK_ESPECIAL_SEMICOLON {};
saida : TK_PR_OUTPUT valor TK_ESPECIAL_SEMICOLON {};

atribuicao : atribuicao_corpo TK_ESPECIAL_SEMICOLON {};
atribuicao_corpo : TK_IDENTIFICADOR vetor_expressao TK_ESPECIAL_EQUALS literal; 

operacao_shift : TK_IDENTIFICADOR vetor_expressao operadores_shift inteiro TK_ESPECIAL_SEMICOLON {};
operadores_shift : TK_OC_SL | TK_OC_SR;

retorno : TK_PR_RETURN expressao TK_ESPECIAL_SEMICOLON;
operacao_break : TK_PR_BREAK TK_ESPECIAL_SEMICOLON {};
operacao_continue : TK_PR_CONTINUE TK_ESPECIAL_SEMICOLON {};

condicional_if : TK_PR_IF TK_ESPECIAL_OPPAR expressao TK_ESPECIAL_CLPAR bloco_comandos_estrutura condional_else TK_ESPECIAL_SEMICOLON {};
condional_else : TK_PR_ELSE bloco_comandos_estrutura | /* vazio */;

iterativa_for : TK_PR_FOR TK_ESPECIAL_OPPAR iterativa_for_cabecalho TK_ESPECIAL_CLPAR bloco_comandos_estrutura TK_ESPECIAL_SEMICOLON {};
iterativa_for_cabecalho : atribuicao_corpo TK_ESPECIAL_COLON expressao TK_ESPECIAL_COLON atribuicao_corpo;

iterativa_while : TK_PR_WHILE TK_ESPECIAL_OPPAR expressao TK_ESPECIAL_CLPAR TK_PR_DO bloco_comandos_estrutura TK_ESPECIAL_SEMICOLON {};

const_opcional : TK_PR_CONST |  /* vazio */;
static_opcional : TK_PR_STATIC |  /* vazio */;
tipo : TK_PR_INT | TK_PR_FLOAT | TK_PR_CHAR | TK_PR_BOOL | TK_PR_STRING;
literal : TK_LIT_POSINT {}
    | TK_LIT_NEGINT {}
    | TK_LIT_FLOAT {}
    | TK_LIT_FALSE {}
    | TK_LIT_TRUE {}
    | TK_LIT_CHAR {}
    | TK_LIT_STRING {};
vetor : TK_ESPECIAL_OPBRACKETS TK_LIT_POSINT TK_ESPECIAL_CLBRACKETS {}
    | /* vazio */ {};
vetor_expressao : TK_ESPECIAL_OPBRACKETS expressao TK_ESPECIAL_CLBRACKETS | /* vazio */;
inteiro : TK_LIT_POSINT | TK_LIT_NEGINT;

valor : TK_IDENTIFICADOR | literal;

chamada_funcao : TK_IDENTIFICADOR TK_ESPECIAL_OPPAR TK_ESPECIAL_CLPAR TK_ESPECIAL_SEMICOLON {}
| TK_IDENTIFICADOR TK_ESPECIAL_OPPAR lista_argumentos TK_ESPECIAL_CLPAR TK_ESPECIAL_SEMICOLON {};
lista_argumentos : argumento | argumento TK_ESPECIAL_COMMA lista_argumentos;
argumento : expressao;

expressao : TK_IDENTIFICADOR {}
    | TK_IDENTIFICADOR TK_ESPECIAL_OPBRACKETS expressao TK_ESPECIAL_CLBRACKETS {}
    | chamada_funcao
    | literal
    | expressao_ternaria {}
    | expressao_binaria 
    | expressao_unaria
    | TK_ESPECIAL_OPPAR expressao TK_ESPECIAL_CLPAR {}
;
expressao_binaria : expressao op_binarios expressao;
op_binarios : TK_ESPECIAL_ADD {}
    | TK_ESPECIAL_SUB {}
    | TK_ESPECIAL_MULT {}
    | TK_ESPECIAL_DIV {}
    | TK_ESPECIAL_REM {}
    | TK_ESPECIAL_BITOR {}
    | TK_ESPECIAL_BITAND {}
    | TK_ESPECIAL_POW {}
    | TK_OC_LE {}
    | TK_OC_GE {}
    | TK_OC_EQ {}
    | TK_OC_NE {}
    | TK_OC_AND {}
    | TK_OC_OR {};
expressao_unaria : TK_ESPECIAL_EXCLAMATION expressao {}
    | expressao TK_ESPECIAL_INTERROGATION {}
    | TK_ESPECIAL_ADD expressao {}
    | TK_ESPECIAL_SUB expressao {}
    | TK_ESPECIAL_BITAND TK_IDENTIFICADOR {}
    | TK_ESPECIAL_MULT TK_IDENTIFICADOR {}
    | TK_ESPECIAL_HASH TK_IDENTIFICADOR {};
expressao_ternaria : expressao TK_ESPECIAL_INTERROGATION expressao TK_ESPECIAL_COLON expressao;
%%

int yyerror(char const *s) {
    printf("%s\n", s);
    return 1;
}
