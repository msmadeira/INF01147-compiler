
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

%%

int get_line_number () {
    return 1;
}