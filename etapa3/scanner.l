%{
    #include <stdio.h>
    #include "types.h"
    #include "parser.tab.h"
    int line_number = 1;
    int get_line_number(void);
    int get_ascii(char);
    void set_valor_lexico(int);
%}
%s IN_COMMENT

WHITESPACE    [ \t]+
NUMBER    [0-9]
ALPHABETICAL    [A-Za-z_]
ALPHANUMERIC    {NUMBER}|{ALPHABETICAL}
POSINTEGER    [+]?{NUMBER}+
NEGINTEGER    "-"{NUMBER}+
INTEGER    [+-]?{NUMBER}+
FLOAT    {INTEGER}("."{INTEGER}+)?([eE][+-]?{INTEGER}+)?
CHAR    '.'
STRING    \".*\"
SINGLE_LINE_COMMENT    "//".*

%%
<INITIAL>{
        /* line counting */
    \n ++line_number;

        /* ignores */
    "/*" BEGIN(IN_COMMENT);
    {WHITESPACE} {}
    {SINGLE_LINE_COMMENT} {}

        /* reserved words */
    "int" return TK_PR_INT;
    "float" return TK_PR_FLOAT;
    "bool" return TK_PR_BOOL;
    "char" return TK_PR_CHAR;
    "string" return TK_PR_STRING;
    "if" return TK_PR_IF;
    "then" return TK_PR_THEN;
    "else" return TK_PR_ELSE;
    "while" return TK_PR_WHILE;
    "do" return TK_PR_DO;
    "input" return TK_PR_INPUT;
    "output" return TK_PR_OUTPUT;
    "return" return TK_PR_RETURN;
    "const" return TK_PR_CONST;
    "static" return TK_PR_STATIC;
    "foreach" return TK_PR_FOREACH;
    "for" return TK_PR_FOR;
    "switch" return TK_PR_SWITCH;
    "case" return TK_PR_CASE;
    "break" return TK_PR_BREAK;
    "continue" return TK_PR_CONTINUE;
    "class" return TK_PR_CLASS;
    "private" return TK_PR_PRIVATE;
    "public" return TK_PR_PUBLIC;
    "protected" return TK_PR_PROTECTED;
    "end" return TK_PR_END;
    "default" return TK_PR_DEFAULT;

        /* special characters */
    "," { set_valor_lexico(TK_ESPECIAL_COMMA); return TK_ESPECIAL_COMMA; };
    ";" { set_valor_lexico(TK_ESPECIAL_SEMICOLON); return TK_ESPECIAL_SEMICOLON; };
    ":" { set_valor_lexico(TK_ESPECIAL_COLON); return TK_ESPECIAL_COLON; };
    "(" { set_valor_lexico(TK_ESPECIAL_OPPAR); return TK_ESPECIAL_OPPAR; };
    ")" { set_valor_lexico(TK_ESPECIAL_CLPAR); return TK_ESPECIAL_CLPAR; };
    "[" { set_valor_lexico(TK_ESPECIAL_OPBRACKETS); return TK_ESPECIAL_OPBRACKETS; };
    "]" { set_valor_lexico(TK_ESPECIAL_CLBRACKETS); return TK_ESPECIAL_CLBRACKETS; };
    "{" { set_valor_lexico(TK_ESPECIAL_OPCURLY); return TK_ESPECIAL_OPCURLY; };
    "}" { set_valor_lexico(TK_ESPECIAL_CLCURLY); return TK_ESPECIAL_CLCURLY; };
    "!" { set_valor_lexico(TK_ESPECIAL_EXCLAMATION); return TK_ESPECIAL_EXCLAMATION; };
    "?" { set_valor_lexico(TK_ESPECIAL_INTERROGATION); return TK_ESPECIAL_INTERROGATION; };
    "+" { set_valor_lexico(TK_ESPECIAL_ADD); return TK_ESPECIAL_ADD; };
    "-" { set_valor_lexico(TK_ESPECIAL_SUB); return TK_ESPECIAL_SUB; };
    "*" { set_valor_lexico(TK_ESPECIAL_MULT); return TK_ESPECIAL_MULT; };
    "/" { set_valor_lexico(TK_ESPECIAL_DIV); return TK_ESPECIAL_DIV; };
    "<" { set_valor_lexico(TK_ESPECIAL_LTHAN); return TK_ESPECIAL_LTHAN; };
    ">" { set_valor_lexico(TK_ESPECIAL_GTHAN); return TK_ESPECIAL_GTHAN; };
    "|" { set_valor_lexico(TK_ESPECIAL_BITOR); return TK_ESPECIAL_BITOR; };
    "=" { set_valor_lexico(TK_ESPECIAL_EQUALS); return TK_ESPECIAL_EQUALS; };
    "&" { set_valor_lexico(TK_ESPECIAL_BITAND); return TK_ESPECIAL_BITAND; };
    "%" { set_valor_lexico(TK_ESPECIAL_REM); return TK_ESPECIAL_REM; };
    "#" { set_valor_lexico(TK_ESPECIAL_HASH); return TK_ESPECIAL_HASH; };
    "^" { set_valor_lexico(TK_ESPECIAL_POW); return TK_ESPECIAL_POW; };
    "." { int code = get_ascii('.'); set_valor_lexico(code); return code; };
    "$" { int code = get_ascii('$'); set_valor_lexico(code); return code; };

        /* Compound operators */
    "<=" { set_valor_lexico(TK_OC_LE); return TK_OC_LE; };
    ">=" { set_valor_lexico(TK_OC_GE); return TK_OC_GE; };
    "==" { set_valor_lexico(TK_OC_EQ); return TK_OC_EQ; };
    "!=" { set_valor_lexico(TK_OC_NE); return TK_OC_NE; };
    "&&" { set_valor_lexico(TK_OC_AND); return TK_OC_AND; };
    "||" { set_valor_lexico(TK_OC_OR); return TK_OC_OR; };
    ">>" { set_valor_lexico(TK_OC_SL); return TK_OC_SL; };
    "<<" { set_valor_lexico(TK_OC_SR); return TK_OC_SR; };

        /* boolean Literals */
    "false" { set_valor_lexico(TK_LIT_FALSE); return TK_LIT_FALSE; };
    "true" { set_valor_lexico(TK_LIT_TRUE); return TK_LIT_TRUE; };

        /* Identifiers */
    {ALPHABETICAL}{ALPHANUMERIC}* return TK_IDENTIFICADOR;

        /* Literals */
    {POSINTEGER} { set_valor_lexico(TK_LIT_POSINT); return TK_LIT_POSINT; };
    {NEGINTEGER} { set_valor_lexico(TK_LIT_NEGINT); return TK_LIT_NEGINT; };
    {FLOAT} { set_valor_lexico(TK_LIT_FLOAT); return TK_LIT_FLOAT; };
    {CHAR} { set_valor_lexico(TK_LIT_CHAR); return TK_LIT_CHAR; };
    {STRING} { set_valor_lexico(TK_LIT_STRING); return TK_LIT_STRING; };
    . return TOKEN_ERRO;
}

<IN_COMMENT>{
    "*/" BEGIN(INITIAL);
    [^*\n]+ {}
    "*" {}
    \n ++line_number;
}

%%

int get_line_number () {
    return line_number;
}

int get_ascii(char c) {
    int code = c;
    return code;
}

void set_valor_lexico(int token) {
    char x; 
    int i;

    yylval.valor_lexico = (valor_lexico_tipo*)malloc(sizeof(valor_lexico_tipo));
    memset(yylval.valor_lexico, 0, sizeof(valor_lexico_tipo));

    yylval.valor_lexico->numero_linha = get_line_number();
    yylval.valor_lexico->tipo_token = LITERAL; // define como tipo padrão para evitar duplicação de código no switch

    switch(token) {
        case TK_LIT_CHAR:
            yylval.valor_lexico->valor_token.caractere = *(yytext + sizeof(char));
            break;
        case TK_LIT_STRING:        
            for(x = 'a', i = 0; x != '\0'; i++)
                x = yytext[i];
            yytext[i-2] = '\0';
            yylval.valor_lexico->valor_token.palavra = strdup(yytext + 1);
            break;
        case TK_LIT_POSINT:
        case TK_LIT_NEGINT:
            yylval.valor_lexico->valor_token.inteiro = atoi(yytext);
            break;
        case TK_LIT_FLOAT:
            yylval.valor_lexico->valor_token.ponto_flutuante = atof(yytext);
            break;
        case TK_LIT_TRUE:
            yylval.valor_lexico->valor_token.booleano = 1;
            break;
        case TK_LIT_FALSE:
            yylval.valor_lexico->valor_token.booleano = 0;
            break;
        case TK_IDENTIFICADOR:
            yylval.valor_lexico->valor_token.nome_geral = strdup(yytext);
            yylval.valor_lexico->tipo_token = IDENTIFICADOR;
            break;
        case TK_OC_LE:
        case TK_OC_GE:
        case TK_OC_EQ:
        case TK_OC_NE:
        case TK_OC_AND:
        case TK_OC_OR:
        case TK_OC_SR:
        case TK_OC_SL:
            yylval.valor_lexico->valor_token.nome_geral = strdup(yytext);
            yylval.valor_lexico->tipo_token = COMPOSTO;
            break;
        default:
            yylval.valor_lexico->valor_token.nome_geral = strdup(yytext);
            yylval.valor_lexico->tipo_token = ESPECIAL;
            break;

    }
}
