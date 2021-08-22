
%{
    #include <stdio.h>
    int minha_var_global = 0;
    int get_line_number(void);
%}
%%
"lexico" { get_line_number(); }
%%
int get_line_number () {
    return 1;
}