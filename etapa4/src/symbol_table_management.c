#include "symbol_table_management.h"

#include <stdio.h>
#include <stdlib.h>

int initialized = 0;

void init()
{
    stack_init();
    initialized = 1;
    current_function = NULL;
    func_depth = 0;
}

void enter_scope()
{
    if (!initialized)
    {
        init();
    }

    symbol_table_t *st = create_symbol_table();
    push((void *)st);
    if (current_function != NULL && func_depth == 0)
    {
        declare_params(((symbol_t *)(current_function->data))->args);
        func_depth++;
    }
}

void leave_scope()
{
    if (!initialized)
    {
        init();
    }

    symbol_table_t *st = (symbol_table_t *)pop();
    if (st != NULL)
    {
        free_symbol_table(st);
    }

    if (current_function != NULL && func_depth > 0)
    {
        free(current_function);
        func_depth--;
        if (func_depth == 0)
        {
            current_function = NULL;
        }
    }
}

error_t *find_id(char *key, int global)
{
    if (!initialized)
        init();

    symbol_t *symbol = NULL;                              
    error_t *status = (error_t *)malloc(sizeof(error_t));
    status->error_type = -1;
    entry_t *entry;
    if ((symbol = retrieve_symbol((symbol_table_t *)(stack->top->data), key, 0)) != NULL)
    {
        status->error_type = 0;
        status->data1 = (void *)symbol;
    }
    else
    {
        if (global)
        {
            entry = stack->top->bot;
            for (int i = 1; i < stack->size && (status->error_type == -1); i++, entry = entry->bot)
            {
                if ((symbol = retrieve_symbol((symbol_table_t *)(entry->data), key, 0)) != NULL)
                {
                    status->error_type = 0;
                    status->data1 = (void *)symbol;
                }
            }
        }
        if (status->error_type == -1)
        {
            status->error_type = ERR_UNDECLARED;
            status->data1 = (void *)key;
        }
    }

    return status;
}

st_entry_t *make_symbol_entry(lexical_value_t *lexval, int count, SymbolKind kind)
{
    if (!initialized)
    {
        init();
    }

    symbol_t *new_symbol = create_symbol(lexval, TYPE_TBA, kind, count, 0, NULL);
    st_entry_t *new_entry = (st_entry_t *)malloc(sizeof(st_entry_t));
    new_entry->data = new_symbol;
    new_entry->next = NULL;

    return new_entry;
}

st_entry_t *make_symbol_list(st_entry_t *symbol_entry, st_entry_t *list)
{
    if (!initialized)
    {
        init();
    }
    st_entry_t *aux = NULL;
    if (list != NULL)
    {
        aux = list;
        while (aux->next != NULL)
        {
            aux = aux->next;
        }
        aux->next = symbol_entry;
        return list;
    }
    else
    {
        return symbol_entry;
    }
}

void declare_symbol_list(st_entry_t *list, LanguageType type)
{
    if (!initialized)
    {
        init();
    }

    st_entry_t *aux = NULL; 
    error_t *status = NULL; 

    if (list != NULL)
    {
        for (aux = list; aux != NULL && status == NULL;)
        {
            ((symbol_t *)(aux->data))->type = type;
            if ((status = insert_symbol(((entry_t *)(stack->top))->data, aux->data)) == NULL)
            {
                list = aux;
                aux = aux->next;
                free(list);
            }
            else
            {
                print_error(status);
            }
        }
    }
}

void check_init_types(node_t *vars, LanguageType type)
{
    LanguageType new_type; 
    node_t *aux = NULL;     
    error_t *status = NULL; 
    symbol_t *initializer_symbol = NULL;
    node_t *identifier = NULL; 
    node_t *initializer = NULL;

    if (vars != NULL)
    {
        aux = vars->children->brothers->brothers;
        do
        {
            identifier = vars->children;
            initializer = vars->children->brothers;
            vars->type = type;
            identifier->type = type;
            if (initializer->lexval->category == CAT_IDENTIFIER)
            {
                status = find_id(initializer->lexval->value.name, 1);
                if (status->error_type != 0)
                {
                    print_error(status);
                }
                else
                {
                    initializer->type = ((symbol_t *)(status->data1))->type;
                    initializer_symbol = ((symbol_t *)(status->data1));
                    free(status);
                    status = NULL;
                }
            }
            if ((new_type = infer_type(identifier->type, initializer->type)) != -1)
            {
                identifier->type = new_type;
                vars->type = new_type;
                status = find_id(identifier->lexval->value.name, 1);
                if (status->error_type == 0)
                {
                    ((symbol_t *)(status->data1))->type = new_type;
                    if (type == TYPE_STRING)
                    {
                        if (initializer->lexval->category == CAT_LITERAL)
                        {
                            ((symbol_t *)(status->data1))->count = strlen(initializer->lexval->value.string);
                            ((symbol_t *)(status->data1))->size = ((symbol_t *)(status->data1))->count * type_size(TYPE_STRING);
                        }
                        else
                        {
                            ((symbol_t *)(status->data1))->count = initializer_symbol->count;
                            ((symbol_t *)(status->data1))->size = ((symbol_t *)(status->data1))->count * type_size(TYPE_STRING);
                        }
                    }
                }
                else
                {
                    print_error(status);
                }

                free(status);
            }
            else
            {
                status = create_error(ERR_WRONG_TYPE);
                status->data1 = (void *)vars;
                print_error(status);
            }
            vars = aux;
            if (aux != NULL)
            {
                aux = aux->children->brothers->brothers;
            }

        } while (vars != NULL);
    }
}

st_entry_t *make_function_entry(lexical_value_t *lexval, LanguageType type)
{
    if (!initialized)
    {
        init();
    }

    symbol_t *new_symbol = create_symbol(lexval, type, KIND_FUNCTION, 1, 0, NULL);
    st_entry_t *new_entry = (st_entry_t *)malloc(sizeof(st_entry_t));
    new_entry->data = new_symbol;
    new_entry->next = NULL;

    return new_entry;
}

st_entry_t *make_param_entry(lexical_value_t *param, LanguageType type)
{
    if (!initialized)
    {
        init();
    }

    symbol_t *new_symbol = create_symbol(param, type, KIND_IDENTIFIER, 1, 0, NULL);
    st_entry_t *new_param = (st_entry_t *)malloc(sizeof(st_entry_t));
    new_param->data = new_symbol;
    new_param->next = NULL;

    return new_param;
}

st_entry_t *make_param_list(st_entry_t *param, st_entry_t *list)
{
    if (!initialized)
    {
        init();
    }

    if (list != NULL)
    {
        param->next = list;
        ((symbol_t *)(param->data))->args = list;
        ((symbol_t *)(param->data))->argument_count = ((symbol_t *)(list->data))->argument_count + 1;

        return param;
    }
    else
    {
        if (param != NULL)
        {
            ((symbol_t *)(param->data))->argument_count = 1;
        }
        return param;
    }
}

st_entry_t *declare_function(st_entry_t *function, st_entry_t *params, int global)
{
    if (!initialized)
        init();

    symbol_t *function_symbol = NULL;
    error_t *status = NULL;           

    if (function != NULL && function->data != NULL)
    {
        function_symbol = ((symbol_t *)(function->data));
        function_symbol->args = params;
        if (params != NULL && params->data != NULL)
        {
            function_symbol->argument_count = ((symbol_t *)(params->data))->argument_count + 1;
        }
        if ((status = insert_symbol(((entry_t *)(stack->top))->data, function_symbol)) != NULL)
        {
            print_error(status);
        }
    }
    current_function = function;

    return function;
}

void declare_params(st_entry_t *params)
{
    if (!initialized)
        init();

    st_entry_t *aux = NULL;
    error_t *status = NULL; 
    symbol_t *param = NULL; 
    aux = params;

    while (aux != NULL)
    {
        param = create_symbol(((symbol_t *)(aux->data))->data,
                              ((symbol_t *)(aux->data))->type,
                              ((symbol_t *)(aux->data))->kind,
                              ((symbol_t *)(aux->data))->count,
                              ((symbol_t *)(aux->data))->argument_count,
                              ((symbol_t *)(aux->data))->args);
        if ((status = insert_symbol(((entry_t *)(stack->top))->data, param)) != NULL)
        {
            print_error(status);
        }
        aux = aux->next;
    }
}
