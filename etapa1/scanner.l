
%{
    #include <stdio.h>
    #include "tokens.h"
    int get_line_number(void);
%}

WHITESPACE    [\t\n\r ]

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

%%

int get_line_number () {
    return 1;
}