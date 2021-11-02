#include "stack.h"

void stack_init()
{
    symbol_table_t *global_scope = create_symbol_table();
    entry_t *stack_entry = (entry_t *)malloc(sizeof(entry_t));
    stack = (stack_t *)malloc(sizeof(stack_t));
    stack->size = 1;
    stack->top = stack_entry;
    stack->top->data = (void *)global_scope;
    stack->top->bot = NULL;
}

void push(void *data)
{
    entry_t *new_entry = (entry_t *)malloc(sizeof(entry_t));
    new_entry->data = data;
    new_entry->bot = stack->top;
    stack->top = new_entry;
    stack->size++;
}

void *pop()
{
    void *data = NULL;   
    entry_t *aux = NULL; 
    if (stack->size > 0)
    {
        aux = stack->top;
        data = aux->data;
        stack->top = aux->bot;
        free(aux);
        stack->size--;
        if (stack->size == 0)
        {
            free(stack);
            stack = NULL;
        }
    }

    return data;
}