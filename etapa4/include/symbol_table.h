#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"
#include "types.h"
#include "error_report.h"

symbol_table_t *create_symbol_table();
symbol_t *create_symbol(lexical_value_t *lv, LanguageType type, SymbolKind kind, int amount, int arg_count, st_entry_t *args);
error_t *insert_symbol(symbol_table_t *st, symbol_t *symbol);
symbol_t *retrieve_symbol(symbol_table_t *st, char *key, int f_par);
void free_symbol_table(symbol_table_t *st);
void print_symbol_table(symbol_table_t *st);
void print_symbol(symbol_t *symbol);
char *get_symbol_name(lexical_value_t *lexval, LanguageType type);

#endif