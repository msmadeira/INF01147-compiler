#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define TIPO_LIT		1
#define TIPO_CHAR_ESP   2
#define TIPO_OP_CMP     3
#define TIPO_ID         4
#define TIPO_RSV_WRD    5
#define TIPO_VET		6
#define ID_INT			1
#define ID_BOOL			2
#define ID_CHAR			3	
#define ID_FLOAT		4
#define ID_STRING		5
#define INDEF		   -1
#define NOT_LIT	 	 	0
#define LIT_TIPO_INT 	1
#define LIT_TIPO_BOOL 	2
#define LIT_TIPO_CHAR 	3
#define LIT_TIPO_FLOAT 	4
#define LIT_TIPO_STRING	5
#define NO_LEX 			0
#define NO_WHILE		1
#define NO_FUN_CALL		2
#define NO_IN_OUT		3
#define NO_FOR			4
#define NO_IF			5
#define NO_ELSE			6
#define NAT_LIT			1
#define NAT_VAR			2
#define NAT_FUN			3
#define NAT_VET			4
#define ID				1
#define LIT				2
struct lexic_val_type
{
	int lineno;
	int type;
	int value_type;
	union
	{
		int vInt;
		float vFloat;
		char* vStr;
		bool vBool;
		char* vChar;
	}tk_value;
};
typedef struct lexic_val_type lexic_val_type;
struct lexic_val_type* createVal(int tipo_token, int tipo_lit, int lineno, char* yytext);
lexic_val_type* invVal( lexic_val_type* lexValue);
int getValSize(lexic_val_type* lexValue);

