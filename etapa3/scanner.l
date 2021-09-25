%{
    #include <stdio.h>
    #include "parser.tab.h"
    int line_number = 1;
    int get_line_number(void);
    int get_ascii(char);
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
    "," return TK_ESPECIAL_COMMA;
    ";" return TK_ESPECIAL_SEMICOLON;
    ":" return TK_ESPECIAL_COLON;
    "(" return TK_ESPECIAL_OPPAR;
    ")" return TK_ESPECIAL_CLPAR;
    "[" return TK_ESPECIAL_OPBRACKETS;
    "]" return TK_ESPECIAL_CLBRACKETS;
    "{" return TK_ESPECIAL_OPCURLY;
    "}" return TK_ESPECIAL_CLCURLY;
    "!" return TK_ESPECIAL_EXCLAMATION;
    "?" return TK_ESPECIAL_INTERROGATION;
    "+" return TK_ESPECIAL_ADD;
    "-" return TK_ESPECIAL_SUB;
    "*" return TK_ESPECIAL_MULT;
    "/" return TK_ESPECIAL_DIV;
    "<" return TK_ESPECIAL_LTHAN;
    ">" return TK_ESPECIAL_GTHAN;
    "|" return TK_ESPECIAL_BITOR;
    "=" return TK_ESPECIAL_EQUALS;
    "&" return TK_ESPECIAL_BITAND;
    "%" return TK_ESPECIAL_REM;
    "#" return TK_ESPECIAL_HASH;
    "^" return TK_ESPECIAL_POW;
    "." { return get_ascii('.'); };
    "$" { return get_ascii('$'); };

        /* Compound operators */
    "<=" return TK_OC_LE;
    ">=" return TK_OC_GE;
    "==" return TK_OC_EQ;
    "!=" return TK_OC_NE;
    "&&" return TK_OC_AND;
    "||" return TK_OC_OR;
    ">>" return TK_OC_SL;
    "<<" return TK_OC_SR;

        /* boolean Literals */
    "false" return TK_LIT_FALSE;
    "true" return TK_LIT_TRUE;

        /* Identifiers */
    {ALPHABETICAL}{ALPHANUMERIC}* return TK_IDENTIFICADOR;

        /* Literals */
    {POSINTEGER} return TK_LIT_POSINT;
    {NEGINTEGER} return TK_LIT_NEGINT;
    {FLOAT} return TK_LIT_FLOAT;
    {CHAR} return TK_LIT_CHAR;
    {STRING} return TK_LIT_STRING;
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
