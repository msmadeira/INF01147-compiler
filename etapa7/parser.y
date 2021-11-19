%{
	#include <stdlib.h>
	#include <stdio.h>
	#include <string>
	#include "stackB.hpp"
	using namespace std;
	extern "C" 
	{
		int yyparse(void);
		int yylex(void);  
		int yywrap() 
		{
			return 1;
		}
	}
	int yyerror (char const *s);
	extern int get_line_number (void);
	extern char *yytext;
	extern void *tree;
	extern StackM tabelas;	
%}

%union
{
	struct lexic_val_type* lexValue;
	struct tree* node;
	int tipo;	
}

%token  TK_PR_INT
%token  TK_PR_FLOAT
%token  TK_PR_BOOL
%token  TK_PR_CHAR
%token  TK_PR_STRING
%token TK_PR_THEN
%token TK_PR_ELSE
%token TK_PR_DO
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
%token TK_PR_CONST
%token TK_PR_STATIC
%token TK_PR_FOREACH
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
%token <lexValue> TK_PR_IF
%token <lexValue> TK_PR_WHILE
%token <lexValue> TK_PR_FOR
%token <lexValue> TK_OC_LE
%token <lexValue> TK_OC_GE
%token <lexValue> TK_OC_EQ
%token <lexValue> TK_OC_NE
%token <lexValue> TK_OC_AND
%token <lexValue> TK_OC_OR
%token <lexValue> TK_OC_SL
%token <lexValue> TK_OC_SR
%token <lexValue> TK_LIT_INT
%token <lexValue> TK_LIT_FLOAT
%token <lexValue> TK_LIT_CHAR
%token <lexValue> TK_LIT_STRING
%token <lexValue> TK_LIT_TRUE
%token <lexValue> TK_LIT_FALSE
%token <lexValue> TK_IDENTIFICADOR
%token TOKEN_ERRO

%token <lexValue> ';' 
%token <lexValue> ','
%token <lexValue> '('
%token <lexValue> ')'
%token <lexValue> '['
%token <lexValue>  ']'
%token <lexValue> '{'
%token <lexValue> '}'
%token <lexValue>  ':'
%token <lexValue> '-' 
%token <lexValue> '+'
%token <lexValue> '*' 
%token <lexValue> '|' 
%token <lexValue> '<' 
%token <lexValue> '>' 
%token <lexValue> '/' 
%token <lexValue> '=' 
%token <lexValue> '?'
%token <lexValue> '!' 
%token <lexValue> '%' 
%token <lexValue> '#' 
%token <lexValue> '&' 
%token <lexValue> '$' 
%token <lexValue> '^' 
%token <lexValue> '.' 
%type <node> programa
%type <node> func
%type <node> func_header
%type <node> bloco
%type <node> seq_comando
%type <node> bloco_fun
%type <node> lista_par
%type <node> in_out
%type <node> comando
%type <node> decla_loc
%type <node> atrib
%type <node> fun_call
%type <node> shift_right
%type <node> shift_left
%type <node> literal
%type <node> comando_controle_fluxo
%type <node> lista_var_loc
%type <node> var_loc
%type <node> all_float
%type <node> pos_int
%type <node> ret_cont_break;
%type <node> op_unitario
%type <node> all_int
%type <node> literal_num_bool
%type <node> exp
%type <node> var_vet
%type <node> exp_unitaria
%type <node> lista_arg
%type <node> fun_input
%type <node> id_lit_exp
%type <node> comando_if
%type <node> comando_for
%type <node> comando_while
%type <node> id_lit
%type <tipo> tipo_stat_cons
%type <tipo> tipo_cons
%type <tipo> tipo_stat
%type <tipo> tipo_nome
%start programa

%right '?' ':'
%left TK_OC_AND TK_OC_OR
%left '|'
%left '&' 
%left TK_OC_EQ TK_OC_NE
%left '<' '>' TK_OC_LE TK_OC_GE 
%left '+' '-'
%left '*' '/' '%'
%right '^'
%right PREC_UNA

%%
//Definição básica de um programa

programa:  {$$ = NULL; tree = $$;}
| programa func  { if ($1 == NULL){ $$ = $2; tree = $2;} else{ insertChild($1,$2); $$=$2; } }
| programa decla {$$ = $1;};

decla: tipo_stat lista_var ';' { freeV($3);tabelas.updateTS($1);};

//Definição dos tipos com indicadores prefixados ou não para declaracoes globais

tipo_stat: TK_PR_STATIC tipo_nome {$$ = $2;}
| tipo_nome{$$ = $1;};
tipo_cons: TK_PR_CONST tipo_nome{$$ =$2;} 
| tipo_nome {$$ =$1;};

tipo_nome: TK_PR_INT {$$ = ID_INT;}
| TK_PR_FLOAT {$$ = ID_FLOAT;}
| TK_PR_CHAR {$$ = ID_CHAR;}
| TK_PR_STRING{$$ = ID_STRING;};
| TK_PR_BOOL {$$ = ID_BOOL;}

tipo_stat_cons: TK_PR_STATIC TK_PR_CONST tipo_nome {$$ = $3;}
| TK_PR_CONST tipo_nome {$$ = $2;}
| TK_PR_STATIC tipo_nome {$$ = $2;} 
| tipo_nome {$$ = $1;};

lista_var: lista_var ',' var 
| var ;
var: TK_IDENTIFICADOR'[' pos_int ']' {tabelas.addNewS(get_line_number(),NAT_VET,$1,INDEF,$3->lexValue->tk_value.vInt);freeV($1); freeV($2); libera($3);freeV($4);}
| TK_IDENTIFICADOR {tabelas.addNormal(get_line_number(),NAT_VAR,$1,INDEF);freeV($1);};

bloco_fun: '{' fim_bloco { $$ =NULL; }
| '{' seq_comando fim_bloco { $$ = $2;} ;

lista_par_begin: '(' {tabelas.insertNewSM(); freeV($1);};
lista_par_end: ')'{freeV($1);};

func_header:  tipo_stat  TK_IDENTIFICADOR lista_par_begin lista_par lista_par_end{tabelas.insertFun(get_line_number(),$2);$$ = insertNode(NULL,$2);tabelas.updateFun($2,$1); updateST($$,$1);}
|tipo_stat  TK_IDENTIFICADOR lista_par_begin lista_par_end{tabelas.insertFun(get_line_number(),$2);$$ = insertNode(NULL,$2);tabelas.updateFun($2,$1); updateST($$,$1);};
func: func_header bloco_fun { $$ = insertChild( $1,$2); }


lista_par: lista_par ',' tipo_cons TK_IDENTIFICADOR {tabelas.addNormal(get_line_number(),NAT_VAR,$4,$3);tabelas.pushParam($4);freeV($2); freeV($4); }
| tipo_cons TK_IDENTIFICADOR {tabelas.addNormal(get_line_number(),NAT_VAR,$2,$1); tabelas.pushParam($2);freeV($2);}; 

com_bloco: '{' {tabelas.insertNewSM();freeV($1);};
fim_bloco: '}' {freeV($1);tabelas.delSM();};
bloco: com_bloco fim_bloco   { $$ =NULL; }
| com_bloco seq_comando fim_bloco { $$ = $2;} ;

//Definição da sequencia de comandos e abaixo os diferentes tipos de comandos
seq_comando: comando seq_comando { $$ = $1; $$ = insertChild($$,$2); }
| comando { $$ = $1; };

comando: bloco ';' {$$ = NULL; freeV($2);} 
| decla_loc ';' {$$ = $1; freeV($2);}
| in_out ';' {$$ = $1; freeV($2);}
| atrib ';' {$$ = $1; freeV($2);}
| shift_left ';'{$$ = $1; freeV($2);}
| shift_right ';' {$$ = $1; freeV($2);}
| fun_call ';' {$$ = $1; freeV($2);}
| comando_controle_fluxo ';' {$$ = $1; freeV($2);};
| ret_cont_break ';' {$$ = $1; freeV($2);}

decla_loc: tipo_stat_cons lista_var_loc { $$ = $2;tabelas.updateTS($1);tabelas.fazInic();};

lista_var_loc: lista_var_loc ',' var_loc {freeV($2); $$ = insertChild($1,$3);}
| var_loc { $$ = $1; };

var_loc: TK_IDENTIFICADOR TK_OC_LE id_lit { $$ = insertNode(NULL,$2); $$= insertChild($$,insertNode(NULL,$1));$$= insertChild($$,$3);tabelas.addNormal(get_line_number(),NAT_VAR,$1,INDEF);tabelas.addIni($1,$3->lexValue);}
| TK_IDENTIFICADOR { $$ = NULL;tabelas.addNormal(get_line_number(),NAT_VAR,$1,INDEF); freeV($1);};

id_lit: literal {$$ = $1;}
| TK_IDENTIFICADOR {$$ = insertNode(NULL,$1);updateST($$,tabelas.getTypeLex($1));};

literal: all_int {$$ = $1;updateST($$,ID_INT);}
| all_float {$$ = $1;updateST($$,ID_FLOAT);}
| TK_LIT_TRUE  {$$ = insertNode(NULL,$1);updateST($$,ID_BOOL);tabelas.addNormal(get_line_number(),NAT_LIT,$1,ID_BOOL);}
| TK_LIT_FALSE {$$ = insertNode(NULL,$1);updateST($$,ID_BOOL);tabelas.addNormal(get_line_number(),NAT_LIT,$1,ID_BOOL);}
| TK_LIT_CHAR {$$ = insertNode(NULL,$1);updateST($$,ID_CHAR);tabelas.addNormal(get_line_number(),NAT_LIT,$1,ID_CHAR);}
| TK_LIT_STRING {$$ = insertNode(NULL,$1);updateST($$,ID_STRING);tabelas.addNormal(get_line_number(),NAT_LIT,$1,ID_STRING);};

literal_num_bool: TK_LIT_INT {$$ = insertNode(NULL,$1);tabelas.addNormal(get_line_number(),NAT_LIT,$1,ID_INT);updateST($$,ID_INT);}
| TK_LIT_FLOAT {$$ = insertNode(NULL,$1);tabelas.addNormal(get_line_number(),NAT_LIT,$1,ID_FLOAT);updateST($$,ID_FLOAT);}
| TK_LIT_FALSE {$$ = insertNode(NULL,$1);tabelas.addNormal(get_line_number(),NAT_LIT,$1,ID_BOOL);updateST($$,ID_BOOL);}
| TK_LIT_TRUE {$$ = insertNode(NULL,$1);tabelas.addNormal(get_line_number(),NAT_LIT,$1,ID_BOOL);updateST($$,ID_BOOL);};

pos_int: '+' TK_LIT_INT {$$ = insertNode(NULL,$2); freeV($1);tabelas.addNormal(get_line_number(),NAT_LIT,$2,ID_INT);updateST($$,ID_INT);}
| TK_LIT_INT {$$ = insertNode(NULL,$1);tabelas.addNormal(get_line_number(),NAT_LIT,$1,ID_INT);updateST($$,ID_INT);};

all_int: '-' TK_LIT_INT { $$ = insertNode(NULL,invVal($2)); freeV($1);tabelas.addNormal(get_line_number(),NAT_LIT,invVal($2),ID_INT);updateST($$,ID_INT);}
| TK_LIT_INT {$$ = insertNode(NULL,$1);tabelas.addNormal(get_line_number(),NAT_LIT,$1,ID_INT);updateST($$,ID_INT);}
| '+' TK_LIT_INT {$$ = insertNode(NULL,$2); freeV($1);tabelas.addNormal(get_line_number(),NAT_LIT,$2,ID_INT);updateST($$,ID_INT);};

all_float: '-' TK_LIT_FLOAT { $$ = insertNode(NULL,invVal($2));freeV($1);tabelas.addNormal(get_line_number(),NAT_LIT,invVal($2),ID_FLOAT);updateST($$,ID_FLOAT);}
| TK_LIT_FLOAT {$$ = insertNode(NULL,$1);tabelas.addNormal(get_line_number(),NAT_LIT,$1,ID_FLOAT);updateST($$,ID_FLOAT);}
| '+' TK_LIT_FLOAT {$$ = insertNode(NULL,$2); freeV($1);tabelas.addNormal(get_line_number(),NAT_LIT,$2,ID_FLOAT);updateST($$,ID_FLOAT);};

atrib: var_vet '=' exp { $$ = insertNode(NULL,$2); insertChild($$,$1); insertChild($$,$3);tabelas.verifyAt($1,$3);updateST($$,$1->typeSemVal);};

var_vet: TK_IDENTIFICADOR { $$= insertNode(NULL,$1);updateST($$,tabelas.getTypeLex($1));tabelas.verifyV($1);}
| TK_IDENTIFICADOR '[' exp ']' { freeV($2);freeV($4);
$$ = insertNode(NULL,createVal(TIPO_VET,NOT_LIT,get_line_number(),(char*)"[]")); insertChild($$,insertNode(NULL,$1)); insertChild($$,$3);updateST($$,tabelas.getTypeLex($1));tabelas.verifyArr($1,$3);};

ret_cont_break: TK_PR_RETURN exp {$$ = insertNode($2,createVal(TIPO_RSV_WRD,NOT_LIT,get_line_number(),(char*)"return"));tabelas.verifyRet($2->typeSemVal,get_line_number());}
| TK_PR_BREAK  {$$ = insertNode(NULL,createVal(TIPO_RSV_WRD,NOT_LIT,get_line_number(),(char*)"break"));}
| TK_PR_CONTINUE  {$$ = insertNode(NULL,createVal(TIPO_RSV_WRD,NOT_LIT,get_line_number(),(char*)"continue"));};

in_out: TK_PR_INPUT TK_IDENTIFICADOR {$$ = insertNode(insertNode(NULL,$2),createVal(TIPO_RSV_WRD,NOT_LIT,get_line_number(),(char*)"input"));tabelas.avalIn($2);}
| TK_PR_OUTPUT id_lit {$$ = insertNode($2,createVal(TIPO_RSV_WRD,NOT_LIT,get_line_number(),(char*)"output")); tabelas.avalOut($2->lexValue);};

shift_right:  var_vet TK_OC_SR pos_int { $$ = insertNode($1,$2); insertChild($$,$3);tabelas.avalShift($3->lexValue); };

shift_left: var_vet TK_OC_SL pos_int { $$ = insertNode($1,$2); insertChild($$,$3); tabelas.avalShift($3->lexValue);};

fun_call: TK_IDENTIFICADOR '(' fun_input ')' { freeV($2); freeV($4); $$ = insertNodeT($3,$1,NO_FUN_CALL);int tipo = tabelas.verifyFunc($1,$3,get_line_number()); updateST($$,tipo);} ;

lista_arg: id_lit_exp ',' lista_arg { freeV($2); $$ = insertChild($1,$3); $1->is_arg = true; }
| id_lit_exp {$$ = $1; $$->is_arg =true;};

fun_input: {$$=NULL;}
|lista_arg {$$=$1;};

id_lit_exp: TK_LIT_CHAR { $$ = insertNode(NULL,$1);updateST($$,ID_CHAR);tabelas.addNormal(get_line_number(),NAT_LIT,$1,ID_CHAR);}
| TK_LIT_STRING { $$ = insertNode(NULL,$1);updateST($$,ID_STRING);tabelas.addNormal(get_line_number(),NAT_VAR,$1,ID_STRING);} 
| exp { $$ = $1;};

//Definição das expressões

exp: literal_num_bool {$$ = $1;} 
| var_vet {$$ = $1;}
| fun_call {$$ = $1;} 
| '(' exp ')' {$$ = $2; freeV($1); freeV($3);} 
| exp_unitaria {$$ = $1;} 
| exp '+' exp { $$ = insertNode(NULL,$2); insertChild($$,$1); insertChild($$,$3);updateST($$,tabelas.typeinf($1,$3));}
| exp '-' exp { $$ = insertNode(NULL,$2); insertChild($$,$1); insertChild($$,$3);updateST($$,tabelas.typeinf($1,$3));}
| exp '/' exp { $$ = insertNode(NULL,$2); insertChild($$,$1); insertChild($$,$3);updateST($$,tabelas.typeinf($1,$3));}
| exp '*' exp { $$ = insertNode(NULL,$2); insertChild($$,$1); insertChild($$,$3);updateST($$,tabelas.typeinf($1,$3));}
| exp '|' exp { $$ = insertNode(NULL,$2); insertChild($$,$1); insertChild($$,$3);updateST($$,tabelas.typeinf($1,$3));}
| exp '&' exp { $$ = insertNode(NULL,$2); insertChild($$,$1); insertChild($$,$3);updateST($$,tabelas.typeinf($1,$3));}
| exp '%' exp { $$ = insertNode(NULL,$2); insertChild($$,$1); insertChild($$,$3);updateST($$,tabelas.typeinf($1,$3));}
| exp '^' exp { $$ = insertNode(NULL,$2); insertChild($$,$1); insertChild($$,$3);updateST($$,tabelas.typeinf($1,$3));}
| exp TK_OC_GE exp { $$ = insertNode(NULL,$2); insertChild($$,$1); insertChild($$,$3);updateST($$,ID_BOOL);}
| exp TK_OC_LE exp { $$ = insertNode(NULL,$2); insertChild($$,$1); insertChild($$,$3);updateST($$,ID_BOOL);}
| exp TK_OC_NE exp { $$ = insertNode(NULL,$2); insertChild($$,$1); insertChild($$,$3);updateST($$,ID_BOOL);}
| exp TK_OC_EQ exp { $$ = insertNode(NULL,$2); insertChild($$,$1); insertChild($$,$3);updateST($$,ID_BOOL);}
| exp TK_OC_OR exp { $$ = insertNode(NULL,$2); insertChild($$,$1); insertChild($$,$3);updateST($$,ID_BOOL);}
| exp TK_OC_AND exp { $$ = insertNode(NULL,$2); insertChild($$,$1); insertChild($$,$3);updateST($$,ID_BOOL);}
| exp '?' exp ':' exp { $$ = insertNode(NULL,createVal(TIPO_CHAR_ESP,NOT_LIT,get_line_number(),(char*)"?:")); insertChild($$,$1); insertChild($$,$3); insertChild($$,$5);freeV($2);freeV($4);updateST($$,tabelas.infTT($1,$3,$5));};
| exp '<' exp { $$ = insertNode(NULL,$2); insertChild($$,$1); insertChild($$,$3);updateST($$,tabelas.typeinf($1,$3));}
| exp '>' exp { $$ = insertNode(NULL,$2); insertChild($$,$1); insertChild($$,$3);updateST($$,tabelas.typeinf($1,$3));}

comando_controle_fluxo: comando_if {$$ = $1;}
| comando_for {$$ = $1;}
| comando_while {$$ = $1;};

comando_if: TK_PR_IF '(' exp ')' bloco  { $$ = insertNodeT(NULL,createVal(TIPO_RSV_WRD,NOT_LIT,get_line_number(),(char*)"if"),NO_IF); $$= insertChild($$,$3); $$= insertChild($$,$5); freeV($2); freeV($4);}
| TK_PR_IF '(' exp ')' bloco TK_PR_ELSE bloco { $$ = insertNodeT(NULL,createVal(TIPO_RSV_WRD,NOT_LIT,get_line_number(),(char*)"if"),NO_IF); $$ = insertChild($$,$3); $$ = insertChild($$,$5); insertChild($$,$7); freeV($2); freeV($4);};

comando_for: TK_PR_FOR '(' atrib ':' exp ':' atrib ')' bloco { $$ = insertNodeT(NULL,createVal(TIPO_RSV_WRD,NOT_LIT,get_line_number(),(char*)"for"),NO_FOR); $$ = insertChild($$,$3); $$ = insertChild($$,$5); $$ = insertChild($$,$7); $$ = insertChild($$,$9); freeV($6); freeV($4); freeV ($2);freeV($8);};

comando_while: TK_PR_WHILE '(' exp ')' TK_PR_DO bloco { $$ =  insertNodeT(NULL,createVal(TIPO_RSV_WRD,NOT_LIT,get_line_number(),(char*)"while"),NO_WHILE); $$ = insertChild($$,$3); $$ = insertChild($$,$6); freeV($2); freeV($4);}; 

op_unitario: '+' { $$ = insertNode(NULL,$1);}
|'#' { $$ = insertNode(NULL,$1);}
|'|' { $$ = insertNode(NULL,$1);}
|'!' { $$ = insertNode(NULL,$1);}
|'*' { $$ = insertNode(NULL,$1);}
|'?' { $$ = insertNode(NULL,$1);};
|'&' { $$ = insertNode(NULL,$1);}
|'-' { $$ = insertNode(NULL,$1);}
exp_unitaria: op_unitario exp { $$ = $1; insertChild($$,$2);updateST($$,$2->typeSemVal);} %prec PREC_UNA; 	
%%

int yyerror(char const *s){
	printf("%s at line %d UNEXPECTED token \"%s\" \n", s,get_line_number(), yytext);
	return 1;
}
