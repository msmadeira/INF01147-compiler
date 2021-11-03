%{
    #include "../include/types.h"
    #include "../include/tree.h"
    #include "../include/parser.tab.h"

    int current_line = 1;
    void atrib_lexico(int tipo);

%}

letra           [A-Za-z]
digito          [0-9]
alfanumerico    {letra}|{digito}
identificador   ({letra}|_)({alfanumerico}|_)*
literal_char    '[^'""\n]'
literal_string  \"[^\n"]+\"
literal_int     {digito}+
literal_float   {literal_int}(\.{digito}+)((E|e)(\+|\-)?{literal_int})?
literal    {literal_char}|{literal_string}|{literal_int}|{literal_float}
comentario_simples  \/\/.*\n
comentario_multilinha_inicio "/*"
comentario_multilinha_fim    "*/"
caractere_especial ","|";"|":"|"("|")"|"["|"]"|"{"|"}"|"+"|"-"|"|"|"*"|"/"|"<"|">"|"="|"!"|"&"|"%"|"#"|"^"|"."|"$"|"?"

%x COMENTARIO

%%  
"int"       { return TK_PR_INT; }
"float"     { return TK_PR_FLOAT; }
"bool"      { return TK_PR_BOOL; }
"char"      { return TK_PR_CHAR; }
"string"    { return TK_PR_STRING; }
"if"        { return TK_PR_IF; }
"then"      { return TK_PR_THEN; }
"else"      { return TK_PR_ELSE; }
"while"     { return TK_PR_WHILE; }
"do"        { return TK_PR_DO; }
"input"     { return TK_PR_INPUT; }
"output"    { return TK_PR_OUTPUT; }
"return"    { return TK_PR_RETURN; }
"const"     { return TK_PR_CONST; }
"static"    { return TK_PR_STATIC; }
"foreach"   { return TK_PR_FOREACH; }
"for"       { return TK_PR_FOR; }
"switch"    { return TK_PR_SWITCH; }
"case"      { return TK_PR_CASE; }
"break"     { return TK_PR_BREAK; }
"continue"  { return TK_PR_CONTINUE; }
"class"     { return TK_PR_CLASS; }
"private"   { return TK_PR_PRIVATE; }
"public"    { return TK_PR_PUBLIC; }
"protected" { return TK_PR_PROTECTED; }
"end"       { return TK_PR_END; }
"default"   { return TK_PR_DEFAULT; }
{caractere_especial} { atrib_lexico(yytext[0]); return yytext[0];}
"<="        { atrib_lexico(TK_OC_LE);  return TK_OC_LE;}
">="        { atrib_lexico(TK_OC_GE);  return TK_OC_GE;}
"=="        { atrib_lexico(TK_OC_EQ);  return TK_OC_EQ;}
"!="        { atrib_lexico(TK_OC_NE);  return TK_OC_NE;}
"&&"        { atrib_lexico(TK_OC_AND); return TK_OC_AND;}
"||"        { atrib_lexico(TK_OC_OR);  return TK_OC_OR;}
">>"        { atrib_lexico(TK_OC_SR);  return TK_OC_SR;}
"<<"        { atrib_lexico(TK_OC_SL);  return TK_OC_SL;}
{literal_char}      { atrib_lexico(TK_LIT_CHAR);   return TK_LIT_CHAR;}
{literal_string}    { atrib_lexico(TK_LIT_STRING); return TK_LIT_STRING;}
{literal_int}       { atrib_lexico(TK_LIT_INT);    return TK_LIT_INT;}
{literal_float}     { atrib_lexico(TK_LIT_FLOAT);  return TK_LIT_FLOAT;}
"true"              { atrib_lexico(TK_LIT_TRUE);   return TK_LIT_TRUE;}
"false"             { atrib_lexico(TK_LIT_FALSE);  return TK_LIT_FALSE;}
{identificador}    { atrib_lexico(TK_IDENTIFICADOR); return TK_IDENTIFICADOR;}
{comentario_simples}    {current_line++;}
{comentario_multilinha_inicio}          {BEGIN COMENTARIO;}
<COMENTARIO>[^\n]                       {}
<COMENTARIO>\n                          {current_line++;}
<COMENTARIO>{comentario_multilinha_fim} {BEGIN 0;}
" "|\t {}
\n     {current_line++;}
{literal}{identificador}   {return TOKEN_ERRO;}
.      {return TOKEN_ERRO; }

%%
int get_line_number(void){
    return current_line;
}

void atrib_lexico(int tipo)
{
    char x; 
    int i;
    yylval.lexval = (lexical_value_t*)malloc(sizeof(lexical_value_t));
    memset(yylval.lexval, 0, sizeof(lexical_value_t));
    yylval.lexval->line = get_line_number(); 
    switch(tipo)
    {
        case TK_LIT_CHAR:
            yylval.lexval->value.character = *(yytext+sizeof(char));
            yylval.lexval->category = CAT_LITERAL;
            break;
        case TK_LIT_STRING:        
            for(x = 'a', i = 0; x != '\0'; i++)
            {
                x = yytext[i];
            }
            yytext[i-2] = '\0';
            yylval.lexval->value.string = strdup(yytext + 1);
            yylval.lexval->category = CAT_LITERAL; 
            break;
        case TK_LIT_INT:
            yylval.lexval->value.integer = atoi(yytext);
            yylval.lexval->category = CAT_LITERAL; 
            break;
        case TK_LIT_FLOAT:
            yylval.lexval->value.floating = atof(yytext);
            yylval.lexval->category = CAT_LITERAL; 
            break;
        case TK_LIT_TRUE:
            yylval.lexval->value.boolean = 1; 
            yylval.lexval->category = CAT_LITERAL;
            break;
        case TK_LIT_FALSE:
            yylval.lexval->value.boolean = 0;
            yylval.lexval->category = CAT_LITERAL;
            break;
        case TK_IDENTIFICADOR:
            yylval.lexval->value.name = strdup(yytext);
            yylval.lexval->category = CAT_IDENTIFIER;
            break;
        case TK_OC_LE:
        case TK_OC_GE:
        case TK_OC_EQ:
        case TK_OC_NE:
        case TK_OC_AND:
        case TK_OC_OR:
        case TK_OC_SR:
        case TK_OC_SL:
            yylval.lexval->value.name = strdup(yytext);
            yylval.lexval->category = CAT_COMPOSITE_OPERATOR; 
            break;
        default:
            yylval.lexval->value.name = strdup(yytext); 
            yylval.lexval->category = CAT_SPECIAL_CHARACTER;
            break;
    }
}