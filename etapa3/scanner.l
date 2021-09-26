%{
    
    #include "hash.h"
    #include "ast.h"
    #include "y.tab.h"
    int Running = 1;
    int lineNumber = 1;

    int isRunning(void){
        return Running;
    }

    int getLineNumber(void){
        return lineNumber;
    }

    void initMe(void){
        hashInit();
    }

%}

%x COMMENT

%%

"//".*
"/*"        BEGIN(COMMENT);

"int" return TK_PR_INT;
"float" return TK_PR_FLOAT;
"bool" return TK_PR_BOOL;
"char" return TK_PR_CHAR;
"if" return TK_PR_IF;
"then" return TK_PR_THEN;
"else" return TK_PR_ELSE;
"while" return TK_PR_WHILE;
"do" return TK_PR_DO;
"if" return KW_IF;
"then" return KW_THEN;
"else" return KW_ELSE;
"while" return KW_WHILE;
"loop" return KW_LOOP;
"read" return KW_READ;
"print" return KW_PRINT;
"return" return KW_RETURN;
"<=" return TK_OC_LE;
">=" return TK_OC_GE;
"==" return TK_OC_EQ;
"!=" return TK_OC_NE;
"&&" return TK_OC_AND;
"||" return TK_OC_OR;
">>" return TK_OC_SL;
"<<" return TK_OC_SR;
"false" return TK_LIT_FALSE;
"true" return TK_LIT_TRUE;

[-,;:()\]\[{}+*/<>=|^~&$#]  return yytext[0];

[a-zA-Z_@]+[0-9A-Za-z_@]*   yylval.symbol = hashInsert(yytext, TK_IDENTIFIER); return TK_IDENTIFIER;
[0-9A-F]+                   yylval.symbol = hashInsert(yytext, LIT_INTEGER); return LIT_INTEGER;
[0-9A-F]*\.[0-9A-F]+        yylval.symbol = hashInsert(yytext, LIT_FLOAT); return LIT_FLOAT;

[ \t]
"\n"                        ++lineNumber;

'[^']*'                     yylval.symbol = hashInsert(yytext, LIT_CHAR); return LIT_CHAR;
\"[^\"]*\"                  yylval.symbol = hashInsert(yytext, LIT_STRING); return LIT_STRING;

.                           return TOKEN_ERROR;

<COMMENT>.
<COMMENT>"\n" ++lineNumber;
<COMMENT>"*/" BEGIN(INITIAL);

%%

int yywrap(){
    Running = 0;
    return 1;
}