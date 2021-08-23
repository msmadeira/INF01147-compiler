
%{
    #include <stdio.h>
    #include "tokens.h"
    int get_line_number(void);
    int get_ascii(char);
%}

WHITESPACE  [ \t]+
NUMBER   [0-9]
ALPHABETICAL    [A-Za-z_]
INTEGER    [+-]?{NUMBER}+
CHAR   '.'
STRING   \".*\"

%%

    /* ignores */
{WHITESPACE} {}

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
"," { return get_ascii(','); };
";" { return get_ascii(';'); };
":" { return get_ascii(':'); };
"(" { return get_ascii('('); };
")" { return get_ascii(')'); };
"[" { return get_ascii('['); };
"]" { return get_ascii(']'); };
"{" { return get_ascii('{'); };
"}" { return get_ascii('}'); };
"+" { return get_ascii('+'); };
"-" { return get_ascii('-'); };
"|" { return get_ascii('|'); };
"*" { return get_ascii('*'); };
"/" { return get_ascii('/'); };
"<" { return get_ascii('<'); };
">" { return get_ascii('>'); };
"=" { return get_ascii('='); };
"!" { return get_ascii('!'); };
"&" { return get_ascii('&'); };
"%" { return get_ascii('%'); };
"#" { return get_ascii('#'); };
"^" { return get_ascii('^'); };
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

    /* Identifiers */
{ALPHABETICAL}{NUMBER}* return TK_IDENTIFICADOR;

    /* Literals */
{INTEGER} return TK_LIT_INT;
"false" return TK_LIT_FALSE;
"true" return TK_LIT_TRUE;
{CHAR} return TK_LIT_CHAR;
{STRING} return TK_LIT_STRING;
    /* TODO: TK_LIT_FLOAT */

%%

int get_line_number () {
    return 1;
}

int get_ascii(char c) {
    int code = c;
    return code;
}