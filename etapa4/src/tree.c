#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

void free_node(void *root)
{
    node_t *node = (node_t *)root;
    node_t *aux_1 = NULL;
    node_t *aux_2 = NULL;
    node_t *swap = NULL;
    if (root != NULL)
    {
        if (node->children != NULL)
        {
            aux_1 = node->children;
            aux_2 = aux_1->brothers;
            while (aux_2 != NULL || aux_1 != NULL)
            {
                if (aux_1 != NULL)
                {
                    free_node(aux_1);
                }

                if (aux_2 != NULL)
                {
                    aux_1 = aux_2->brothers;
                }
                else
                {
                    aux_1 = NULL;
                }

                swap = aux_1;
                aux_1 = aux_2;
                aux_2 = swap;
            }
        }

        if (node->next_cmd != NULL)
        {
            free_node(node->next_cmd);
        }

        if (node->lexval != NULL)
        {
            free_lexical_value(node->lexval, node->type);
        }

        free(root);
    }
}

extern void libera(void *arvore)
{
    free_node(arvore);
}

void print_node(node_t *root)
{
    if (root != NULL)
    {
        printf("%p, [label=\"", root);

        switch (root->lexval->category)
        {
            case CAT_SPECIAL_CHARACTER:
            case CAT_COMPOSITE_OPERATOR:
            case CAT_RESERVED:
            case CAT_IDENTIFIER:

            if (root->st_kind == CMD_FUNCTION_CALL)
            {
                printf("call ");
            }

            printf("%s", root->lexval->value.name);
            break;
            case CAT_LITERAL:

            switch (root->type)
            {
                case TYPE_INT:
                    printf("%d", root->lexval->value.integer);
                    break;
                case TYPE_FLOAT:
                    printf("%f", root->lexval->value.floating);
                    break;
                case TYPE_CHAR:
                    printf("%c", root->lexval->value.character);
                    break;
                case TYPE_BOOL:
                    root->lexval->value.boolean ? printf("true") : printf("false");
                    break;
                case TYPE_STRING:
                    printf("%s", root->lexval->value.string);
                    break;
                default: // Error
                    printf("%p", root->lexval);
                    break;
            }
            break;
        }
        printf("\"];\n");
    }
}

void print_connections(node_t *root)
{
    node_t *aux;

    if (root != NULL)
    {
        aux = root->children;
        while (aux != NULL)
        {
            printf("%p, %p\n", root, aux);
            aux = aux->brothers;
        }

        if (root->next_cmd != NULL)
        {
            printf("%p, %p\n", root, root->next_cmd);
        }
    }
}

void export_ast_nodes(node_t *root)
{
    if (root != NULL)
    {
        print_node(root);
        export_ast_nodes(root->children);
        export_ast_nodes(root->brothers);
        export_ast_nodes(root->next_cmd);
    }
}

void export_ast_connections(node_t *root)
{
    if (root != NULL)
    {
        print_connections(root);
        export_ast_connections(root->children);
        export_ast_connections(root->brothers);
        export_ast_connections(root->next_cmd);
    }
}

extern void exporta(void *arvore)
{
    if (arvore != NULL)
    {
        export_ast_connections((node_t *)arvore);
        export_ast_nodes((node_t *)arvore);
    }
}

node_t *create_lexical_node(lexical_value_t *lexval, LanguageType type, Statement st_kind, int declare)
{
    error_t *status = NULL; 
    char *name = NULL;
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->lexval = lexval;
    new_node->st_kind = st_kind;

    if (!declare && st_kind == CMD_OPERAND && type == TYPE_TBA)
    {
        status = find_id(lexval->value.name, 1);
        if (status->error_type != 0)
        {
            print_error(status);
        }
        new_node->type = ((symbol_t *)(status->data1))->type;
        free(status);
    }
    else
    {
        new_node->type = type;
    }

    if (lexval != NULL && lexval->category == CAT_LITERAL)
    {
        name = get_symbol_name(lexval, type);
        status = find_id(name, 0);

        if (status->error_type != 0)
        {
            insert_symbol((symbol_table_t *)(stack->top->data), create_symbol(lexval, type, KIND_NONE, 1, 0, NULL));
        }
        else
        {
            ((symbol_t *)(status->data1))->declaration_line = lexval->line;
        }

        free(status);
        free(name);
    }
    new_node->brothers = NULL;
    new_node->children = NULL;
    new_node->next_cmd = NULL;

    return new_node;
}

node_t *create_command_chain(node_t *first, node_t *second)
{
    if (first != NULL)
    {
        first->next_cmd = second;
        return first;
    }
    else
    {
        return second;
    }
}

node_t *insert_child(node_t *father, node_t *child)
{
    node_t *aux = NULL;
    if (father != NULL)
    {
        if ((aux = father->children) != NULL)
        {
            while (aux->brothers != NULL)
            {
                aux = aux->brothers;
            }
            aux->brothers = child;
        }
        else
        {
            father->children = child;
        }

        return father;
    }
    else
    {
        return child;
    }
}

node_t *create_artificial_node(TokenCategory category, TokenValue value, LanguageType type, Statement st_kind, int line)
{
    lexical_value_t *lexval = (lexical_value_t *)malloc(sizeof(lexical_value_t));
    lexval->value = value;
    lexval->line = line;
    lexval->category = category;

    return create_lexical_node(lexval, type, st_kind, 1);
}

node_t *create_if(node_t *condition, node_t *true_statement, node_t *false_statement)
{
    error_t *status = NULL;
    node_t *if_node = NULL;

    TokenValue value;
    value.name = "if";
    if_node = create_artificial_node(CAT_RESERVED, value, TYPE_NA, CMD_IF, condition->lexval->line);
    if_node = insert_child(if_node, condition);
    if_node = insert_child(if_node, true_statement);
    if_node = insert_child(if_node, false_statement);
    if (condition->lexval->category == CAT_IDENTIFIER)
    {
        check_correct_id_usage(if_node, condition, condition->st_kind == CMD_FUNCTION_CALL ? KIND_FUNCTION : KIND_IDENTIFIER);
    }

    if (!compatible_types(TYPE_BOOL, condition->type))
    {
        if (condition->type == TYPE_CHAR)
        {
            status = create_error(ERR_CHAR_TO_X);
        }
        else if (condition->type == TYPE_STRING)
        {
            status = create_error(ERR_STRING_TO_X);
        }
        else
        {
            status = create_error(-1);
        }
        status->data1 = (void *)if_node;
        print_error(status);
    }

    return if_node;
}

node_t *create_for(node_t *init_atrib, node_t *condition, node_t *loop_atrib, node_t *statement)
{
    node_t *for_node = NULL;
    error_t *status = NULL;

    TokenValue value;
    value.name = "for";
    for_node = create_artificial_node(CAT_RESERVED, value, TYPE_NA, CMD_FOR, init_atrib->lexval->line);
    for_node = insert_child(for_node, init_atrib);
    for_node = insert_child(for_node, condition);
    for_node = insert_child(for_node, loop_atrib);
    for_node = insert_child(for_node, statement);
    if (condition->lexval->category == CAT_IDENTIFIER)
    {
        check_correct_id_usage(for_node, condition, condition->st_kind == CMD_FUNCTION_CALL ? KIND_FUNCTION : KIND_IDENTIFIER);
    }
    if (!compatible_types(TYPE_BOOL, condition->type))
    {
        if (condition->type == TYPE_CHAR)
        {
            status = create_error(ERR_CHAR_TO_X);
        }
        else if (condition->type == TYPE_STRING)
        {
            status = create_error(ERR_STRING_TO_X);
        }
        else
        {
            status = create_error(-1);
        }

        status->data1 = (void *)for_node;
        print_error(status);
    }

    return for_node;
}

node_t *create_while(node_t *condition, node_t *statement)
{
    node_t *while_node = NULL;
    error_t *status = NULL;
    TokenValue value;
    value.name = "while";
    while_node = create_artificial_node(CAT_RESERVED, value, TYPE_NA, CMD_WHILE, condition->lexval->line);
    while_node = insert_child(while_node, condition);
    while_node = insert_child(while_node, statement);
    if (condition->lexval->category == CAT_IDENTIFIER)
    {
        check_correct_id_usage(while_node, condition, condition->st_kind == CMD_FUNCTION_CALL ? KIND_FUNCTION : KIND_IDENTIFIER);
    }
    if (!compatible_types(TYPE_BOOL, condition->type))
    {
        if (condition->type == TYPE_CHAR)
        {
            status = create_error(ERR_CHAR_TO_X);
        }
        else if (condition->type == TYPE_STRING)
        {
            status = create_error(ERR_STRING_TO_X);
        }
        else
        {
            status = create_error(-1);
        }
        status->data1 = (void *)while_node;
        print_error(status);
    }

    return while_node;
}

node_t *create_vector_access(node_t *identifier, node_t *index)
{
    node_t *vector_access_node = NULL;
    error_t *status = NULL;
    TokenValue value;
    value.name = "[]";
    vector_access_node = create_artificial_node(CAT_RESERVED, value, identifier->type, CMD_VECTOR_ACCESS, identifier->lexval->line);
    vector_access_node = insert_child(vector_access_node, identifier);
    vector_access_node = insert_child(vector_access_node, index);
    vector_access_node->type = identifier->type;
    check_correct_id_usage(vector_access_node, identifier, KIND_VECTOR);
    free(status);
    status = NULL;
    if (!compatible_types(TYPE_INT, index->type))
    {
        if (index->type == TYPE_CHAR)
        {
            status = create_error(ERR_CHAR_TO_X);
        }
        else if (index->type == TYPE_STRING)
        {
            status = create_error(ERR_STRING_TO_X);
        }
        else
        {
            status = create_error(-1);
        }
        status->data1 = (void *)vector_access_node;
        print_error(status);
    }

    return vector_access_node;
}

node_t *create_input(node_t *identifier)
{
    node_t *input_node = NULL;
    error_t *status = NULL;

    TokenValue value;
    value.name = "input";
    input_node = create_artificial_node(CAT_RESERVED, value, TYPE_NA, CMD_IO, identifier->lexval->line);
    input_node = insert_child(input_node, identifier);
    check_correct_id_usage(input_node, identifier, identifier->st_kind == CMD_FUNCTION_CALL ? KIND_FUNCTION : KIND_IDENTIFIER);
    if (!compatible_types(identifier->type, TYPE_INT))
    {
        status = create_error(ERR_WRONG_PAR_INPUT);
        status->data1 = (void *)input_node;
        print_error(status);
    }
    return input_node;
}

node_t *create_output(node_t *operand)
{
    node_t *output_node = NULL;
    error_t *status = NULL;
    TokenValue value;
    value.name = "output";
    output_node = create_artificial_node(CAT_RESERVED, value, TYPE_NA, CMD_IO, operand->lexval->line);
    output_node = insert_child(output_node, operand);
    if (operand->lexval->category == CAT_IDENTIFIER)
    {
        check_correct_id_usage(output_node, operand, operand->st_kind == CMD_FUNCTION_CALL ? KIND_FUNCTION : KIND_IDENTIFIER);
    }
    if (!compatible_types(operand->type, TYPE_INT))
    {
        status = create_error(ERR_WRONG_PAR_OUTPUT);
        status->data1 = (void *)output_node;
        print_error(status);
    }
    return output_node;
}

node_t *create_continue(int line)
{
    TokenValue value;
    value.name = "continue";
    return create_artificial_node(CAT_RESERVED, value, TYPE_NA, CMD_BREAK_CONTINUE, line);
}

node_t *create_break(int line)
{
    TokenValue value;
    value.name = "break";
    return create_artificial_node(CAT_RESERVED, value, TYPE_NA, CMD_BREAK_CONTINUE, line);
}

node_t *create_return(node_t *retval)
{
    node_t *return_node = NULL;
    error_t *status = NULL;
    TokenValue value;
    value.name = "return";
    return_node = create_artificial_node(CAT_RESERVED, value, TYPE_NA, CMD_RETURN, retval->lexval->line);
    return_node = insert_child(return_node, retval);
    if (retval->lexval->category == CAT_IDENTIFIER)
    {
        check_correct_id_usage(return_node, retval, retval->st_kind == CMD_FUNCTION_CALL ? KIND_FUNCTION : KIND_IDENTIFIER);
    }
    if (current_function != NULL)
    {
        if (!compatible_types(((symbol_t *)(current_function->data))->type, retval->type))
        {
            status = create_error(ERR_WRONG_PAR_RETURN);
            status->data1 = current_function->data; 
            status->data2 = (void *)retval;      
            print_error(status);
        }
    }
    return return_node;
}

node_t *create_shift(node_t *identifier, node_t *amount, node_t *operator)
{
    node_t *shift_node = NULL;
    error_t *status = NULL;
    shift_node = insert_child(operator, identifier);
    shift_node = insert_child(shift_node, amount);
    shift_node->type = identifier->type;
    if (identifier->lexval->category == CAT_IDENTIFIER)
    {
        check_correct_id_usage(shift_node, identifier, identifier->st_kind == CMD_FUNCTION_CALL ? KIND_FUNCTION : KIND_IDENTIFIER);
    }
    if (amount->lexval->value.integer > 16)
    {
        status = create_error(ERR_WRONG_PAR_SHIFT);
        status->data1 = (void *)(&(amount->lexval->value.integer));
        print_error(status);
    }

    return shift_node;
}

node_t *create_attrib(node_t *lval, node_t *rval, node_t *operator)
{
    node_t *attrib_node = NULL;
    error_t *status = NULL;
    symbol_t *lval_symbol = NULL;
    int string_size = 0;
    attrib_node = insert_child(operator, lval);
    attrib_node = insert_child(attrib_node, rval);
    attrib_node->type = lval->type;

    if (rval->lexval->category == CAT_IDENTIFIER)
    {
        check_correct_id_usage(attrib_node, rval, rval->st_kind == CMD_FUNCTION_CALL ? KIND_FUNCTION : KIND_IDENTIFIER);
    }

    if (lval->lexval->category == CAT_IDENTIFIER)
    {
        check_correct_id_usage(attrib_node, lval, lval->st_kind == CMD_FUNCTION_CALL ? KIND_FUNCTION : KIND_IDENTIFIER);
    }

    if (!compatible_types(lval->type, rval->type))
    {
        status = create_error(ERR_WRONG_TYPE);
        status->data1 = (void *)attrib_node;
        print_error(status);
    }

    if (lval->type == TYPE_STRING)
    {
        status = find_id(lval->lexval->value.name, 1);
        lval_symbol = ((symbol_t *)(status->data1));
        free(status);
        status = NULL;
        string_size = get_resulting_string_size(rval);
        if (lval_symbol->count == 0)
        {
            lval_symbol->count = string_size;
            lval_symbol->size = lval_symbol->count * type_size(lval_symbol->type);
        }
        else
        {
            if (lval_symbol->count < string_size)
            {
                status = create_error(ERR_STRING_SIZE);
                status->data1 = (void *)lval_symbol;
                status->data2 = (void *)attrib_node;
                status->data3 = (void *)&string_size;
                print_error(status);
            }
        }
    }

    return attrib_node;
}

node_t *create_init(node_t *identifier, node_t *rval, node_t *operator)
{
    node_t *init_node = NULL;
    init_node = insert_child(operator, identifier);
    init_node = insert_child(init_node, rval);
    return init_node;
}

node_t *create_unop(node_t *operation, node_t *operand)
{
    LanguageType new_type;
    node_t *unop_node = NULL;
    error_t *status = NULL;
    new_type = infer_type(operation->type, operand->type);
    unop_node = insert_child(operation, operand);
    if (operand->lexval->category == CAT_IDENTIFIER)
    {
        check_correct_id_usage(unop_node, operand, operand->st_kind == CMD_FUNCTION_CALL ? KIND_FUNCTION : KIND_IDENTIFIER);
    }
    unop_node->type = new_type;
    if (new_type == -1)
    {
        if (operand->type == TYPE_CHAR)
        {
            status = create_error(ERR_CHAR_TO_X);
        }
        else if (operand->type == TYPE_STRING)
        {
            status = create_error(ERR_STRING_TO_X);
        }
        else
        {
            status = create_error(-1);
        }
        status->data1 = (void *)unop_node;
        print_error(status);
    }

    return unop_node;
}

node_t *create_binop(node_t *operation, node_t *lval, node_t *rval)
{
    LanguageType new_type;
    error_t *status = NULL;
    node_t *binop_node = NULL;
    binop_node = insert_child(operation, lval);
    binop_node = insert_child(binop_node, rval);
    if (lval->lexval->category == CAT_IDENTIFIER)
    {
        check_correct_id_usage(binop_node, lval, lval->st_kind == CMD_FUNCTION_CALL ? KIND_FUNCTION : KIND_IDENTIFIER);
    }
    if (rval->lexval->category == CAT_IDENTIFIER)
    {
        check_correct_id_usage(binop_node, rval, rval->st_kind == CMD_FUNCTION_CALL ? KIND_FUNCTION : KIND_IDENTIFIER);
    }

    if (operation->st_kind == CMD_TERNOP)
    {
        if (!compatible_types(TYPE_BOOL, lval->type))
        {
            if (lval->type == TYPE_CHAR)
            {
                status = create_error(ERR_CHAR_TO_X);
            }
            else if (lval->type == TYPE_STRING)
            {
                status = create_error(ERR_STRING_TO_X);
            }
            else
            {
                status = create_error(-1);
            }
            status->data1 = (void *)binop_node;
            print_error(status);
        }
        if ((new_type = infer_type(operation->children->type, rval->type)) == -1)
        {
            status = create_error(ERR_WRONG_TYPE);
            status->data1 = (void *)binop_node;
            print_error(status);
        }
        binop_node->type = new_type;
    }
    else
    {
        new_type = infer_type(lval->type, operation->type);
        if (new_type == -1)
        {
            if (lval->type == TYPE_CHAR)
            {
                status = create_error(ERR_CHAR_TO_X);
            }
            else if (lval->type == TYPE_STRING)
            {
                status = create_error(ERR_STRING_TO_X);
            }
            else
            {
                status = create_error(-1);
            }
            status->data1 = (void *)binop_node;
            print_error(status);
        }
        new_type = infer_type(rval->type, operation->type);
        if (new_type == -1)
        {
            if (rval->type == TYPE_CHAR)
            {
                status = create_error(ERR_CHAR_TO_X);
            }
            else if (rval->type == TYPE_STRING)
            {
                status = create_error(ERR_STRING_TO_X);
            }
            else
            {
                status = create_error(-1);
            }
            status->data1 = (void *)binop_node;
            print_error(status);
        }
        if ((lval->type == TYPE_STRING || rval->type == TYPE_STRING) && lval->type != rval->type)
        {
            status = create_error(ERR_STRING_TO_X);
            status->data1 = binop_node;
            print_error(status);
        }
        binop_node->type = infer_type(lval->type, rval->type);
    }

    return binop_node;
}

node_t *create_ternop(node_t *true_statement)
{
    node_t *ternop_node = NULL;
    TokenValue value;
    value.name = ":?";
    ternop_node = create_artificial_node(CAT_RESERVED, value, true_statement->type, CMD_TERNOP, true_statement->lexval->line);
    ternop_node = insert_child(ternop_node, true_statement);
    if (true_statement->lexval->category == CAT_IDENTIFIER)
    {
        check_correct_id_usage(true_statement, true_statement, true_statement->st_kind == CMD_FUNCTION_CALL ? KIND_FUNCTION : KIND_IDENTIFIER);
    }

    return ternop_node;
}

node_t *create_function_declaration(node_t *identifier, node_t *body, LanguageType type)
{
    node_t *function_declaration_node = NULL;
    function_declaration_node = insert_child(identifier, body);
    function_declaration_node->type = type;

    return function_declaration_node;
}

node_t *create_function_call(node_t *identifier, node_t *args)
{
    node_t *function_call_node = NULL;
    error_t *status = NULL;
    symbol_t *symbol = NULL;
    st_entry_t *aux_param = NULL;
    node_t *aux_args = NULL;      
    function_call_node = insert_child(identifier, args);
    function_call_node->st_kind = CMD_FUNCTION_CALL;
    status = find_id(identifier->lexval->value.name, 1);
    if (status->error_type != 0)
    {
        print_error(status);
    }
    symbol = ((symbol_t *)(status->data1));
    function_call_node->type = symbol->type;
    free(status);
    status = NULL;
    check_correct_id_usage(function_call_node, function_call_node, KIND_FUNCTION);
    aux_param = symbol->args;
    aux_args = args;
    int arg_count = 0;
    while (aux_args != NULL)
    {
        switch (aux_args->st_kind)
        {
            case CMD_OPERAND: 
                if (status == NULL && aux_param != NULL && aux_args->lexval->category == CAT_IDENTIFIER)
                {
                
                    check_correct_id_usage(function_call_node, aux_args, aux_args->st_kind == CMD_FUNCTION_CALL ? KIND_FUNCTION : KIND_IDENTIFIER);
                }
                if (aux_param != NULL && !compatible_types(aux_args->type, ((symbol_t *)(aux_param->data))->type))
                {
                    status = create_error(ERR_WRONG_TYPE_ARGS);
                    status->data1 = (void *)symbol;
                    status->data2 = (void *)function_call_node;
                    status->data3 = &arg_count;
                }
                aux_args = aux_args->children;
                break;
            case CMD_VECTOR_ACCESS:

                if (status == NULL && aux_param != NULL)
                {
                    if (!compatible_types(aux_args->type, ((symbol_t *)(aux_param->data))->type))
                    {
                        status = create_error(ERR_WRONG_TYPE_ARGS);
                        status->data1 = (void *)symbol;
                        status->data2 = (void *)function_call_node;
                        status->data3 = &arg_count;
                    }
                }
                aux_args = aux_args->children->brothers->brothers;
                break;
            case CMD_FUNCTION_CALL:
                if (status == NULL && aux_param != NULL)
                {
                    if (!compatible_types(aux_args->type, ((symbol_t *)(aux_param->data))->type))
                    {
                        status = create_error(ERR_WRONG_TYPE_ARGS);
                        status->data1 = (void *)symbol;
                        status->data2 = (void *)function_call_node;
                        status->data3 = &arg_count;
                    }
                }
                aux_args = aux_args->children->brothers;
                break;
            case CMD_UNOP:
                if (status == NULL && aux_param != NULL)
                {
                    if (!compatible_types(aux_args->type, ((symbol_t *)(aux_param->data))->type))
                    {
                        status = create_error(ERR_WRONG_TYPE_ARGS);
                        status->data1 = (void *)symbol;
                        status->data2 = (void *)function_call_node;
                        status->data3 = &arg_count;
                    }
                }
                aux_args = aux_args->children->brothers;
                break;
            case CMD_BINOP:
                if (status == NULL && aux_param != NULL)
                {
                    if (!compatible_types(aux_args->type, ((symbol_t *)(aux_param->data))->type))
                    {
                        status = create_error(ERR_WRONG_TYPE_ARGS);
                        status->data1 = (void *)symbol;
                        status->data2 = (void *)function_call_node;
                        status->data3 = &arg_count;
                    }
                }
                aux_args = aux_args->children->brothers->brothers;
                break;
            case CMD_TERNOP:
                if (status == NULL && aux_param != NULL)
                {
                    if (!compatible_types(aux_args->type, ((symbol_t *)(aux_param->data))->type))
                    {
                        status = create_error(ERR_WRONG_TYPE_ARGS);
                        status->data1 = (void *)symbol;
                        status->data2 = (void *)function_call_node;
                        status->data3 = &arg_count;
                    }
                }
                aux_args = aux_args->children->brothers->brothers->brothers;
                break;
            default:
                status = create_error(-1);
                print_error(status);
                break;
        }
        if (aux_param != NULL)
        {
            aux_param = aux_param->next;
        }
        arg_count++;
    }
    if (arg_count != symbol->argument_count)
    {
        status = create_error(arg_count < symbol->argument_count ? ERR_MISSING_ARGS : ERR_EXCESS_ARGS);
        status->data1 = (void *)symbol;            
        status->data2 = (void *)function_call_node;
        print_error(status);
    }
    if (status != NULL)
    {
        print_error(status);
    }

    return function_call_node;
}

void check_correct_id_usage(node_t *usage, node_t *id, SymbolKind mode)
{
    error_t *status = NULL;
    symbol_t *symbol = NULL;
    status = find_id(id->lexval->value.name, 1);
    if (status->error_type != 0)
    {
        print_error(status);
    }
    symbol = ((symbol_t *)(status->data1));
    free(status);
    status = NULL;
    switch (symbol->kind)
    {
        case KIND_IDENTIFIER:
            if (mode != KIND_IDENTIFIER)
                status = create_error(ERR_VARIABLE);
            break;
        case KIND_VECTOR:
            if (mode != KIND_VECTOR)
                status = create_error(ERR_VECTOR);
            break;
        case KIND_FUNCTION:
            if (mode != KIND_FUNCTION)
                status = create_error(ERR_FUNCTION);
            break;
        default:
            break;
    }
    if (status != NULL)
    {
        status->data1 = (void *)symbol;
        status->data2 = (void *)usage;
        print_error(status);
    }
}

int get_resulting_string_size(node_t *node)
{
    int length = 0;
    error_t *status = NULL;
    switch (node->st_kind)
    {
        case CMD_OPERAND:
            if (node->lexval->category == CAT_IDENTIFIER)
            {
                status = find_id(node->lexval->value.name, 1);
                length = ((symbol_t *)(status->data1))->count;
            }
            else
            {
                length = strlen(node->lexval->value.string);
            }
            break;
        case CMD_BINOP:
            length = get_resulting_string_size(node->children) + get_resulting_string_size(node->children->brothers);
            break;
        default:
            break;
    }

    return length;
}
