#ifndef STACK_H
#define STACK_H
#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "symbol_table.h"

stack_t *stack;
void stack_init();
void push(void *data);
void *pop();

#endif