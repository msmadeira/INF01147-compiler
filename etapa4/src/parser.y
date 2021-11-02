%{
    #include <stdio.h>
    #include "symbol_table_management.h" 
    #include "tree.h"                    

    int yylex(void);
    int get_line_number();
    int yyerror (char const *s);
    extern void *arvore;

    st_entry_t *local_var_symbol_list = NULL;
%}

%union{
    node_t* node;            
    lexical_value_t* lexval;
    st_entry_t* symbol; 
    LanguageType type; 
}

%define parse.error verbose
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
%token<lexval>TK_OC_LE
%token<lexval>TK_OC_GE
%token<lexval>TK_OC_EQ
%token<lexval>TK_OC_NE
%token<lexval>TK_OC_AND
%token<lexval>TK_OC_OR
%token<lexval>TK_OC_SL
%token<lexval>TK_OC_SR
%token TK_OC_FORWARD_PIPE
%token TK_OC_BASH_PIPE
%token<lexval>TK_LIT_INT
%token<lexval>TK_LIT_FLOAT
%token<lexval>TK_LIT_FALSE
%token<lexval>TK_LIT_TRUE
%token<lexval>TK_LIT_CHAR
%token<lexval>TK_LIT_STRING
%token<lexval>TK_IDENTIFICADOR
%token TOKEN_ERRO

%token<lexval> ',' 
%token<lexval> ';' 
%token<lexval> ':' 
%token<lexval> '('
%token<lexval> ')' 
%token<lexval> '+' 
%token<lexval> '-' 
%token<lexval> '|' 
%token<lexval> '#' 
%token<lexval> '<' 
%token<lexval> '>' 
%token<lexval> '}' 
%token<lexval> '{'
%token<lexval> '[' 
%token<lexval> ']' 
%token<lexval> '=' 
%token<lexval> '.' 
%token<lexval> '/' 
%token<lexval> '?'
%token<lexval> '!' 
%token<lexval> '*' 
%token<lexval> '&' 
%token<lexval> '%' 
%token<lexval> '^' 
%token<lexval> '$' 

%start programa

%type <symbol> lista_identificadores_globais
%type <symbol> identificador_global
%type <symbol> lista_parametros
%type <symbol> parametro
%type <symbol> assinatura

%type <node> bloco_comandos 
%type <node> lista_comandos_simples 
%type <node> comando_es 
%type <node> comando_simples 
%type <node> var_local 
%type <node> argumento 
%type <node> lista_identificadores_locais 
%type <node> identificador_local 
%type <node> atribuicao 
%type <node> operador_shift 
%type <node> comando_shift 
%type <node> vetor_indexado 
%type <node> comando_if 
%type <node> comando_for 
%type <node> comando_while 
%type <node> controle_fluxo 
%type <node> termo 
%type <node> expressao 
%type <node> operador_logico 
%type <node> comparador_relacional 
%type <node> operador_binario_alta_prec 
%type <node> operador_binario_baixa_prec 
%type <node> chamada_funcao 
%type <node> operando 
%type <node> fator  
%type <node> declaracao_funcao 
%type <node> definicao_funcao
%type <node> programa 
%type <node> operador_unario 
%type <node> literal
%type <node> lista_argumentos

%type<type> tipo
%type<type> tipo_estatico
%type<type> tipo_const
%type<type> tipo_const_estatico

%%


programa:   /* empty */                 {$$ = NULL; leave_scope();} 
          | declaracao_funcao programa  {$$ = create_command_chain($1,$2); arvore = $$;} 
          | var_global programa         {$$ = $2;}                                     
;

tipo:   TK_PR_INT       {$$ = TYPE_INT;}    
      | TK_PR_FLOAT     {$$ = TYPE_FLOAT;}  
      | TK_PR_CHAR      {$$ = TYPE_CHAR;}   
      | TK_PR_BOOL      {$$ = TYPE_BOOL;}   
      | TK_PR_STRING    {$$ = TYPE_STRING;}
; 

literal:   TK_LIT_INT       {$$ = create_lexical_node($1, TYPE_INT,    CMD_OPERAND, 1);} 
         | TK_LIT_FLOAT     {$$ = create_lexical_node($1, TYPE_FLOAT,  CMD_OPERAND, 1);} 
         | TK_LIT_TRUE      {$$ = create_lexical_node($1, TYPE_BOOL,   CMD_OPERAND, 1);}
         | TK_LIT_FALSE     {$$ = create_lexical_node($1, TYPE_BOOL,   CMD_OPERAND, 1);}
         | TK_LIT_CHAR      {$$ = create_lexical_node($1, TYPE_CHAR,   CMD_OPERAND, 1);}
         | TK_LIT_STRING    {$$ = create_lexical_node($1, TYPE_STRING, CMD_OPERAND, 1);}
;

tipo_estatico:   TK_PR_STATIC tipo {$$ = $2;}
               | tipo              {$$ = $1;}
;

tipo_const:   TK_PR_CONST tipo  {$$ = $2;} 
            | tipo              {$$ = $1;} 
;

tipo_const_estatico:   tipo                          {$$ = $1;} 
                     | TK_PR_STATIC tipo             {$$ = $2;} 
                     | TK_PR_CONST tipo              {$$ = $2;} 
                     | TK_PR_STATIC TK_PR_CONST tipo {$$ = $3;}
;

var_global: tipo_estatico lista_identificadores_globais ';' {declare_symbol_list($2, $1);
                                                             free_lexical_value($3, TYPE_NA);} 
;

lista_identificadores_globais:   identificador_global                                   {$$ = $1;} 
                               | identificador_global ',' lista_identificadores_globais {make_symbol_list($1, $3);
                                                                                         free_lexical_value($2, TYPE_NA);} 
; 

identificador_global:   TK_IDENTIFICADOR                    {$$ = make_symbol_entry($1, 1, KIND_IDENTIFIER);
                                                             free_lexical_value($1, TYPE_NA);}
                      | TK_IDENTIFICADOR '[' TK_LIT_INT ']' {$$ = make_symbol_entry($1, $3->value.integer, KIND_VECTOR);
                                                             free_lexical_value($1, TYPE_NA);
                                                             free_lexical_value($2, TYPE_NA);   
                                                             free_lexical_value($3, TYPE_NA);   
                                                             free_lexical_value($4, TYPE_NA);}  
;

definicao_funcao: tipo_estatico TK_IDENTIFICADOR assinatura {$$ = create_lexical_node($2, $1, CMD_FUNCTION_DECLARATION, 1);
                                                             declare_function(make_function_entry($2, $1), $3, 1);}
                                                            
;

declaracao_funcao: definicao_funcao bloco_comandos { $$ = create_function_declaration($1, $2, $1->type);} 
;

assinatura:   '(' ')'                    {$$ = NULL;
                                          free_lexical_value($1, TYPE_NA);  
                                          free_lexical_value($2, TYPE_NA);}
            | '(' lista_parametros ')'   {$$ = $2;
                                          free_lexical_value($1, TYPE_NA);  
                                          free_lexical_value($3, TYPE_NA);}
;

lista_parametros:   parametro                      {$$ = $1;}
                  | parametro ',' lista_parametros {$$ = make_param_list($1, $3);
                                                    free_lexical_value($2, TYPE_NA);}
;

parametro: tipo_const TK_IDENTIFICADOR {$$ = make_param_entry($2, $1); 
                                        free_lexical_value($2, $1);}
;

bloco_comandos_inicio: '{' {free_lexical_value($1, TYPE_NA);
                            enter_scope();}
;

bloco_comandos_fim: '}' {free_lexical_value($1, TYPE_NA); 
                         leave_scope();}
;

bloco_comandos:   bloco_comandos_inicio bloco_comandos_fim {$$ = NULL;} 
                | bloco_comandos_inicio lista_comandos_simples bloco_comandos_fim {$$ = $2;}
;

lista_comandos_simples:   comando_simples                         {$$ = $1;}                           
                        | comando_simples lista_comandos_simples  {$$ = create_command_chain($1, $2);} 
;

comando_simples:   controle_fluxo ';'         {$$ = $1; free_lexical_value($2, TYPE_NA);} 
                 | TK_PR_RETURN expressao ';' {$$ = create_return($2); free_lexical_value($3, TYPE_NA);}                   
                 | TK_PR_BREAK   ';'          {$$ = create_break(get_line_number()); free_lexical_value($2, TYPE_NA);}    
                 | TK_PR_CONTINUE ';'         {$$ = create_continue(get_line_number()); free_lexical_value($2, TYPE_NA);}  
                 | comando_shift ';'          {$$ = $1; free_lexical_value($2, TYPE_NA);} 
                 | chamada_funcao ';'         {$$ = $1; free_lexical_value($2, TYPE_NA);} 
                 | comando_es ';'             {$$ = $1; free_lexical_value($2, TYPE_NA);} 
                 | atribuicao ';'             {$$ = $1; free_lexical_value($2, TYPE_NA);} 
                 | var_local ';'              {$$ = $1; free_lexical_value($2, TYPE_NA);}
                 | bloco_comandos ';'         {$$ = $1; free_lexical_value($2, TYPE_NA);} 
;

chamada_funcao:   TK_IDENTIFICADOR '(' ')'                  {$$ = create_function_call(create_lexical_node($1, TYPE_TBA, CMD_OPERAND, 0), NULL);
                                                             free_lexical_value($2, TYPE_NA);      
                                                             free_lexical_value($3, TYPE_NA);}      
                | TK_IDENTIFICADOR '(' lista_argumentos ')' {$$ = create_function_call(create_lexical_node($1, TYPE_TBA, CMD_OPERAND, 0), $3);
                                                             free_lexical_value($2, TYPE_NA);                
                                                             free_lexical_value($4, TYPE_NA);} 
;

lista_argumentos:   argumento                      {$$ = $1;}                          
                  | argumento ',' lista_argumentos {$$ = insert_child($1, $3);        
                                                    free_lexical_value($2, TYPE_INT);}
;

argumento: expressao {$$ = $1;} 
;

vetor_indexado: TK_IDENTIFICADOR '[' expressao ']' {$$ = create_vector_access(create_lexical_node($1, TYPE_TBA, CMD_OPERAND, 0), $3);
                                                    free_lexical_value($2, TYPE_NA);   
                                                    free_lexical_value($4, TYPE_NA);} 
;

var_local: tipo_const_estatico lista_identificadores_locais {$$ = $2; 
                                                             declare_symbol_list(local_var_symbol_list, $1); 
                                                             check_init_types($2, $1);                      
                                                             local_var_symbol_list = NULL;}                  
;

lista_identificadores_locais:   identificador_local                                  {$$ = $1;}
                              | identificador_local ',' lista_identificadores_locais {$$ = insert_child($1, $3);       
                                                                                      free_lexical_value($2, TYPE_NA);} 
;

identificador_local:   TK_IDENTIFICADOR                           {$$ = NULL;
                                                                   local_var_symbol_list = make_symbol_list(make_symbol_entry($1, 1, KIND_IDENTIFIER) ,local_var_symbol_list);
                                                                   free_lexical_value($1, TYPE_NA);}
                     | TK_IDENTIFICADOR TK_OC_LE literal          {$$ = create_init(create_lexical_node($1, TYPE_TBA, CMD_OPERAND, 1),
                                                                               $3,
                                                                               create_lexical_node($2, TYPE_NA,  CMD_INIT_VARIABLE, 1));
                                                                   local_var_symbol_list = make_symbol_list(make_symbol_entry($1, 1, KIND_IDENTIFIER) ,local_var_symbol_list);}
                     | TK_IDENTIFICADOR TK_OC_LE TK_IDENTIFICADOR {$$ = create_init(create_lexical_node($1, TYPE_TBA, CMD_OPERAND, 1),
                                                                               create_lexical_node($3, TYPE_TBA, CMD_OPERAND, 0),
                                                                               create_lexical_node($2, TYPE_NA,  CMD_INIT_VARIABLE, 1));
                                                                   local_var_symbol_list = make_symbol_list(make_symbol_entry($1, 1, KIND_IDENTIFIER) ,local_var_symbol_list);}
;

atribuicao:   TK_IDENTIFICADOR '=' expressao {$$ = create_attrib(create_lexical_node($1, TYPE_TBA, CMD_OPERAND, 0), $3, create_lexical_node($2, TYPE_TBA, CMD_ATTRIB_VARIABLE, 1));}
            | vetor_indexado '=' expressao   {$$ = create_attrib($1, $3, create_lexical_node($2, TYPE_TBA, CMD_ATTRIB_VARIABLE, 0));}
;

comando_es:   TK_PR_INPUT TK_IDENTIFICADOR  {$$ = create_input(create_lexical_node($2, TYPE_TBA, CMD_OPERAND, 0));}
            | TK_PR_OUTPUT TK_IDENTIFICADOR {$$ = create_output(create_lexical_node($2, TYPE_TBA, CMD_OPERAND, 0));}
            | TK_PR_OUTPUT literal          {$$ = create_output($2);}
;

operador_shift:   TK_OC_SL {$$ = create_lexical_node($1, TYPE_TBA, CMD_SHIFT, 1);} 
                | TK_OC_SR {$$ = create_lexical_node($1, TYPE_TBA, CMD_SHIFT, 1);} 
;

comando_shift:   TK_IDENTIFICADOR operador_shift TK_LIT_INT {$$ = create_shift(create_lexical_node($1, TYPE_TBA, CMD_OPERAND, 0), 
                                                                               create_lexical_node($3, TYPE_INT, CMD_OPERAND, 1), $2);}
               | vetor_indexado operador_shift TK_LIT_INT   {$$ = create_shift($1, create_lexical_node($3, TYPE_INT, CMD_OPERAND, 1), $2);}
;

controle_fluxo:   comando_if    {$$ = $1;} 
                | comando_for   {$$ = $1;} 
                | comando_while {$$ = $1;} 
;

comando_if:   TK_PR_IF '(' expressao ')' bloco_comandos {$$ = create_if($3,$5, NULL);     
                                                         free_lexical_value($2, TYPE_NA); 
                                                         free_lexical_value($4, TYPE_NA);} 
            | TK_PR_IF '(' expressao ')' bloco_comandos TK_PR_ELSE bloco_comandos {$$ = create_if($3,$5,$7);        
                                                                                   free_lexical_value($2, TYPE_NA);  
                                                                                   free_lexical_value($4, TYPE_NA);}
;

comando_for: TK_PR_FOR '(' atribuicao ':' expressao ':' atribuicao ')' bloco_comandos {$$ = create_for($3,$5,$7,$9);     
                                                                                       free_lexical_value($2, TYPE_NA);  
                                                                                       free_lexical_value($4, TYPE_NA);  
                                                                                       free_lexical_value($6, TYPE_NA); 
                                                                                       free_lexical_value($8, TYPE_NA);} 
;

comando_while: TK_PR_WHILE '(' expressao ')' TK_PR_DO bloco_comandos {$$ = create_while($3,$6);         
                                                                      free_lexical_value($2, TYPE_NA); 
                                                                      free_lexical_value($4, TYPE_NA);} 
;

expressao:   expressao operador_binario_baixa_prec termo {$$ = create_binop($2, $1, $3);}
           | termo                                       {$$ = $1;}            
;

termo:   termo operador_binario_alta_prec fator {$$ = create_binop($2, $1, $3);} 
       | fator                                  {$$ = $1;}                       
;

fator:   '(' expressao ')' {$$ = $2;                          
                            free_lexical_value($1, TYPE_NA);  
                            free_lexical_value($3, TYPE_NA);} 
       | operando          {$$ = $1;}                         
;

operando:   TK_IDENTIFICADOR       {$$ = create_lexical_node($1, TYPE_TBA, CMD_OPERAND, 0);}   
          | vetor_indexado         {$$ = $1;}                
          | literal                {$$ = $1;}
          | chamada_funcao         {$$ = $1;}                                      
          | operador_unario fator  {$$ = create_unop($1, $2);}
; 

comparador_relacional:   TK_OC_GE {$$ = create_lexical_node($1, TYPE_INT, CMD_BINOP, 1);} 
                       | TK_OC_LE {$$ = create_lexical_node($1, TYPE_INT, CMD_BINOP, 1);} 
                       | TK_OC_EQ {$$ = create_lexical_node($1, TYPE_INT, CMD_BINOP, 1);}
                       | TK_OC_NE {$$ = create_lexical_node($1, TYPE_INT, CMD_BINOP, 1);} 
                       | '<'      {$$ = create_lexical_node($1, TYPE_INT, CMD_BINOP, 1);} 
                       | '>'      {$$ = create_lexical_node($1, TYPE_INT, CMD_BINOP, 1);}
;

operador_logico:   TK_OC_AND {$$ = create_lexical_node($1, TYPE_BOOL, CMD_BINOP, 1);}
                 | TK_OC_OR  {$$ = create_lexical_node($1, TYPE_BOOL, CMD_BINOP, 1);} 
;

operador_binario_baixa_prec:   '+'                   {$$ = create_lexical_node($1, TYPE_ANY, CMD_BINOP, 1);} 
                             | '-'                   {$$ = create_lexical_node($1, TYPE_INT, CMD_BINOP, 1);}
                             | '|'                   {$$ = create_lexical_node($1, TYPE_BOOL, CMD_BINOP, 1);} 
                             | '&'                   {$$ = create_lexical_node($1, TYPE_BOOL, CMD_BINOP, 1);} 
                             | comparador_relacional {$$ = $1;}                           
                             | operador_logico       {$$ = $1;}                           
                             | '?' expressao ':'     {$$ = create_ternop($2);         
                                                      free_lexical_value($1, TYPE_NA);    
                                                      free_lexical_value($3, TYPE_NA);}   
;

operador_binario_alta_prec:   '*' {$$ = create_lexical_node($1, TYPE_INT, CMD_BINOP, 1);} 
                            | '/' {$$ = create_lexical_node($1, TYPE_INT, CMD_BINOP, 1);} 
                            | '%' {$$ = create_lexical_node($1, TYPE_INT, CMD_BINOP, 1);} 
                            | '^' {$$ = create_lexical_node($1, TYPE_INT, CMD_BINOP, 1);} 
;

operador_unario:   '+' {$$ = create_lexical_node($1, TYPE_INT,  CMD_UNOP, 1);} 
                 | '-' {$$ = create_lexical_node($1, TYPE_INT,  CMD_UNOP, 1);} 
                 | '!' {$$ = create_lexical_node($1, TYPE_BOOL, CMD_UNOP, 1);} 
                 | '&' {$$ = create_lexical_node($1, TYPE_ANY,  CMD_UNOP, 1);}
                 | '*' {$$ = create_lexical_node($1, TYPE_ANY,  CMD_UNOP, 1);} 
                 | '?' {$$ = create_lexical_node($1, TYPE_BOOL, CMD_UNOP, 1);} 
                 | '#' {$$ = create_lexical_node($1, TYPE_ANY,  CMD_UNOP, 1);} 
;

%%
int yyerror(char const *s)
{
    int error_line = get_line_number(); 
    fprintf(stderr,"%s\nOn line %d\n", s, error_line);
    
    return 1;
}
