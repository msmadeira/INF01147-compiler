#ifndef TREE_H
#define TREE_H
#include "types.h"
#include "symbol_table_management.h"

extern void exporta(void *arvore);
extern void libera(void *arvore);
node_t *create_lexical_node(lexical_value_t *lexval, LanguageType type, Statement st_kind, int declare);
node_t *create_command_chain(node_t *first, node_t *second);
node_t *insert_child(node_t *father, node_t *child);
node_t *create_if(node_t *condition, node_t *true_statement, node_t *false_statement);
node_t *create_for(node_t *init_atrib, node_t *condition, node_t *loop_atrib, node_t *statement);
node_t *create_while(node_t *condition, node_t *statement);
node_t *create_vector_access(node_t *identifier, node_t *index);
node_t *create_input(node_t *identifier);
node_t *create_output(node_t *operand);
node_t *create_continue(int line);
node_t *create_break(int line);
node_t *create_return(node_t *retval);
node_t *create_shift(node_t *identifier, node_t *amount, node_t *operator);
node_t *create_attrib(node_t *lval, node_t *rval, node_t *operator);
node_t *create_init(node_t *identifier, node_t *rval, node_t *operator);
node_t *create_unop(node_t *operation, node_t *operand);
node_t *create_binop(node_t *operation, node_t *lval, node_t *rval);
node_t *create_ternop(node_t *true_statement);
node_t *create_function_declaration(node_t *identifier, node_t *body, LanguageType type);
node_t *create_function_call(node_t *identifier, node_t *args);
void check_correct_id_usage(node_t *usage, node_t *id, SymbolKind mode);
int get_resulting_string_size(node_t *node);

#endif