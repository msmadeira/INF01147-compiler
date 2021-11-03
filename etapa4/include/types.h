#ifndef TYPES_H
#define TYPES_H
#include <stdlib.h>

typedef union TokenValue
{
    char *name;

    int integer;   
    float floating; 
    char *string;   
    char character;
    int boolean;

} TokenValue;

typedef enum TokenCategory
{
    CAT_SPECIAL_CHARACTER,  
    CAT_COMPOSITE_OPERATOR,
    CAT_IDENTIFIER,         
    CAT_LITERAL,            
    CAT_RESERVED          

} TokenCategory;

typedef enum LanguageType
{
    TYPE_INT,    
    TYPE_FLOAT, 
    TYPE_BOOL,
    TYPE_CHAR,  
    TYPE_STRING, 
    TYPE_TBA,   
    TYPE_NA, 
    TYPE_ANY 

} LanguageType;

typedef enum Statement_Kinds
{
    CMD_FUNCTION_DECLARATION,
    CMD_FUNCTION_CALL,        
    CMD_INIT_VARIABLE,        
    CMD_ATTRIB_VARIABLE,    
    CMD_IO,                  
    CMD_SHIFT,               
    CMD_BREAK_CONTINUE,      
    CMD_RETURN,             
    CMD_IF,                 
    CMD_FOR,               
    CMD_WHILE,             
    CMD_UNOP,                
    CMD_BINOP,              
    CMD_TERNOP,             
    CMD_OPERAND,             
    CMD_VECTOR_ACCESS      
} Statement;


typedef struct lexical_value
{
    int line;               
    TokenCategory category; 
    TokenValue value;       

} lexical_value_t;

typedef enum SymbolKind
{
    KIND_IDENTIFIER,
    KIND_VECTOR,    
    KIND_FUNCTION,   
    KIND_NONE      

} SymbolKind;

typedef struct symbol_table_entry
{
    void *data;                    
    struct symbol_table_entry *next; 

} st_entry_t;

typedef struct symbol_data
{
    char *key;
    int declaration_line;
    int argument_count;
    st_entry_t *args;
    LanguageType type;
    SymbolKind kind;
    int size;
    int count;
    lexical_value_t *data;
} symbol_t;

typedef struct symbol_table_handler
{
    int size;     
    st_entry_t *first;
} symbol_table_t;

typedef struct stack_entry
{
    void *data;              
    struct stack_entry *bot; 

} entry_t;

typedef struct stack_handle
{
    int size;  
    entry_t *top;

} stack_t;

typedef struct node
{
    lexical_value_t *lexval; 
    Statement st_kind;
    LanguageType type; 
    struct node *children;
    struct node *brothers;
    struct node *next_cmd; 
} node_t;

void free_lexical_value(lexical_value_t *lexval, LanguageType type);
int type_size(LanguageType type);
int compatible_types(LanguageType type1, LanguageType type2);
LanguageType infer_type(LanguageType type1, LanguageType type2);
char* type_name(LanguageType type);

#endif
