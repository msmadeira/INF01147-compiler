#ifndef ERROR_H
#define ERROR_H
#include "errors.h"
#include "types.h"

typedef struct error_return_format
{
    int error_type;
    void *data1;
    void *data2;
    void *data3;
} error_t;

#include "symbol_table.h" 

int get_line_number();
error_t *create_error(int type);
void print_error(error_t *error);
char *reconstruct_node(node_t *node);

#endif