#include "symbol_table.h"

symbol_table_t *create_symbol_table()
{
    symbol_table_t *new_symbol_table = (symbol_table_t *)malloc(sizeof(symbol_table_t));
    new_symbol_table->size = 0;
    new_symbol_table->first = NULL;

    return new_symbol_table;
}

symbol_t *create_symbol(lexical_value_t *lv, LanguageType type, SymbolKind kind, int amount, int arg_count, st_entry_t *args)
{
    lexical_value_t *symbol_lexval = (lexical_value_t *)malloc(sizeof(lexical_value_t));
    symbol_lexval->category = lv->category;

    char *new_name = NULL;

    if (lv->category == CAT_LITERAL)
    {
        if (type == TYPE_STRING)
        {
            new_name = strdup(lv->value.string);
            symbol_lexval->value.string = new_name;
        }
        else
            symbol_lexval->value = lv->value;
    }
    else
    {
        new_name = strdup(lv->value.name);
        symbol_lexval->value.name = new_name;
    }

    symbol_lexval->line = lv->line;
    symbol_t *symbol = (symbol_t *)malloc(sizeof(symbol_t));
    symbol->key = get_symbol_name(lv, type);
    symbol->count = amount;
    symbol->data = symbol_lexval;
    symbol->kind = kind;
    symbol->size = amount * type_size(type);
    symbol->type = type;
    symbol->declaration_line = symbol_lexval->line;
    symbol->args = args;
    symbol->argument_count = arg_count;

    return symbol;
}

error_t *insert_symbol(symbol_table_t *st, symbol_t *symbol)
{
    error_t *status = NULL; 
    st_entry_t *new_entry = (st_entry_t *)malloc(sizeof(st_entry_t));
    new_entry->next = NULL;
    new_entry->data = (void *)symbol;

    st_entry_t *entry = NULL;     
    st_entry_t *entry_aux = NULL; 

    if (st != NULL)
    {
        entry = st->first;
        entry_aux = entry;

        if (entry != NULL)
        {
            do
            {
                if (!strcmp(((symbol_t *)(entry->data))->key, symbol->key))
                {
                    status = create_error(ERR_DECLARED);
                    status->data1 = (void *)symbol; 
                    status->data2 = entry->data; 
                }
                entry_aux = entry;
                entry = entry->next;
            } while (entry != NULL && status == NULL);
            if (status == NULL)
            {
                entry_aux->next = new_entry;
                st->size++;
            }
        }
        else
        {
            st->first = new_entry;
            st->size++;
        }
    }

    return status;
}

symbol_t *retrieve_symbol(symbol_table_t *st, char *key, int f_par)
{
    symbol_t *symbol = NULL;  
    st_entry_t *entry = NULL; 
    st_entry_t *aux = NULL;

    if (st != NULL)
    {
        entry = st->first;
        while (entry != NULL && symbol == NULL)
        {
            if (!strcmp(key, ((symbol_t *)(entry->data))->key))
            {
                symbol = (symbol_t *)(entry->data);
            }
            else
            {
                if (f_par == 1 && ((symbol_t *)(entry->data))->kind == KIND_FUNCTION)
                {
                    aux = ((symbol_t *)(entry->data))->args;
                    while (aux != NULL && symbol == NULL)
                    {
                        if (!strcmp(key, ((symbol_t *)(aux->data))->key))
                        {
                            symbol = (symbol_t *)(aux->data);
                        }
                        else
                        {
                            aux = aux->next;
                        }
                    }
                }
            }
            entry = entry->next;
        }
    }

    return symbol;
}

void free_symbol_table(symbol_table_t *st)
{
    st_entry_t *entry;  
    st_entry_t *entry_aux; 
    st_entry_t *param_aux;
    st_entry_t *param_aux_next;

    entry = st->first;

    for (int i = 0; i < st->size; i++, entry = entry_aux)
    {
        entry_aux = entry->next;
        free(((symbol_t *)(entry->data))->key);
        if (((symbol_t *)(entry->data))->kind == KIND_FUNCTION)
        {
            param_aux = ((symbol_t *)(entry->data))->args;
            while (param_aux != NULL)
            {
                free(((symbol_t *)(param_aux->data))->key);
                free_lexical_value(((symbol_t *)(param_aux->data))->data, ((symbol_t *)(param_aux->data))->type);
                free((symbol_t *)(param_aux->data));
                param_aux_next = param_aux->next;
                free(param_aux);
                param_aux = param_aux_next;
            }
        }
        free_lexical_value(((symbol_t *)(entry->data))->data, ((symbol_t *)(entry->data))->type);
        free(entry->data);
        free(entry);
    }

    free(st);
}

void print_symbol_table(symbol_table_t *st)
{

    st_entry_t *aux = st->first;
    st_entry_t *par_aux = NULL;
    while (aux != NULL)
    {
        print_symbol((symbol_t *)(aux->data));
        if (((symbol_t *)(aux->data))->kind == KIND_FUNCTION)
        {
            printf("Arguments:\n");
            par_aux = ((symbol_t *)(aux->data))->args;
            while (par_aux != NULL)
            {
                print_symbol((symbol_t *)(par_aux->data));
                par_aux = par_aux->next;
            }
        }
        aux = aux->next;
    }
}

void print_symbol(symbol_t *symbol)
{
    printf("========================\n");
    printf("Symbol key: %s\n", symbol->key);
    printf("Symbol type: %d\n", symbol->type);
    printf("Declared on line: %d\n", symbol->declaration_line);
    printf("Symbol kind: %d\n", symbol->kind);
    printf("Symbol argument count: %d\n", symbol->argument_count);
    printf("========================\n");
}

char *get_symbol_name(lexical_value_t *lexval, LanguageType type)
{
    char *name = NULL;

    if (lexval->category == CAT_LITERAL)
    {
        switch (type)
        {
        case TYPE_INT:
            name = calloc(20, 1);
            sprintf(name, "\"%d\"", lexval->value.integer);
            return name;
        case TYPE_FLOAT:
            name = calloc(20, 1);
            sprintf(name, "\"%f\"", lexval->value.floating);
            return name;
        case TYPE_STRING:
            name = calloc(strlen(lexval->value.string) + 3, 1);
            sprintf(name, "\"%s\"", lexval->value.string);
            return name;
        case TYPE_CHAR:
            name = calloc(3, 1);
            sprintf(name, "\"%c\"", lexval->value.character);
            return name;
        case TYPE_BOOL:
            name = calloc(6, 1);
            lexval->value.boolean ? sprintf(name, "true") : sprintf(name, "false");
            return name;
        default:
            return NULL;
        }
    }
    else
    {
        name = strdup(lexval->value.name);
        return name;
    }
}