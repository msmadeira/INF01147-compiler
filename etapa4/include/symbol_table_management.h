#ifndef ST_MANAGEMENT_H
#define ST_MANAGEMENT_H
#include "symbol_table.h"
#include "stack.h"
#include "types.h"

st_entry_t *current_function; 
int func_depth;            
void init();
void enter_scope();
void leave_scope();
error_t *find_id(char *key, int global);
st_entry_t *make_symbol_entry(lexical_value_t *lexval, int count, SymbolKind kind);
st_entry_t *make_symbol_list(st_entry_t *symbol_entry, st_entry_t *list);
void declare_symbol_list(st_entry_t *list, LanguageType type);
void check_init_types(node_t *vars, LanguageType type);
st_entry_t *make_function_entry(lexical_value_t *lexval, LanguageType type);
st_entry_t *make_param_entry(lexical_value_t *param, LanguageType type);
st_entry_t *make_param_list(st_entry_t *param, st_entry_t *list);
st_entry_t *declare_function(st_entry_t *function, st_entry_t *params, int global);
void declare_params(st_entry_t *params);

#endif