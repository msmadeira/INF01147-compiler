#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_report.h"

error_t *create_error(int type)
{
    error_t *error = (error_t *)malloc(sizeof(error_t));
    error->error_type = type;
    error->data1 = NULL;
    error->data2 = NULL;
    error->data3 = NULL;
    return error;
}

char *literal_name(node_t *node)
{
    char *name = calloc(2048, sizeof(char));
    switch (node->type)
    {
    case TYPE_INT:
        sprintf(name, "%d", node->lexval->value.integer);
        break;
    case TYPE_FLOAT:
        sprintf(name, "%f", node->lexval->value.floating);
        break;
    case TYPE_BOOL:
        sprintf(name, "%s", node->lexval->value.boolean ? "true" : "false");
        break;
    case TYPE_CHAR:
        sprintf(name, "\'%c\'", node->lexval->value.character);
        break;
    case TYPE_STRING:
        sprintf(name, "\"%s\"", node->lexval->value.string);
        break;
    default:
        sprintf(name, "%s", "\% ERROR UNKOWN LITERAL \%");
    }

    return name;
}

char *reconstruct_function_declaration(symbol_t *func)
{
    char *rec = malloc(2048);
    int size = 0;
    st_entry_t *aux = NULL;
    aux = func->args;
    sprintf(rec, "%s %s (", type_name(func->type), func->key);
    size = size + strlen(func->key) + strlen(type_name(func->type)) + 3;
    while (aux != NULL && aux->next != NULL)
    {
        sprintf(rec + size, "%s %s, ", type_name(((symbol_t *)(aux->data))->type), ((symbol_t *)(aux->data))->key);
        size = size + strlen(((symbol_t *)(aux->data))->key) + strlen(type_name(((symbol_t *)(aux->data))->type)) + 3;
        aux = aux->next;
    }
    if (aux != NULL)
    {
        sprintf(rec + size, "%s %s", type_name(((symbol_t *)(aux->data))->type), ((symbol_t *)(aux->data))->key);
        size += strlen(type_name(((symbol_t *)(aux->data))->type)) + strlen(((symbol_t *)(aux->data))->key) + 1;
    }
    sprintf(rec + size, ")");

    return rec;
}

char *reconstruct_function_call(node_t *func)
{
    char *rec = calloc(2048, 1);
    char *temp = NULL;
    int size = 0;
    node_t *aux = NULL;
    node_t *aux_2 = NULL;
    aux = func->children;
    sprintf(rec, "%s (", func->lexval->value.name);
    size = size + strlen(func->lexval->value.name) + 2;

    while (aux != NULL)
    {
        aux_2 = aux->children;
        temp = reconstruct_node(aux);
        sprintf(rec + size, "%s", temp);
        size = size + strlen(temp);
        while (aux_2 != NULL && aux_2->brothers != NULL)
        {
            aux_2 = aux_2->brothers;
        }
        aux = aux_2;
        if (aux != NULL)
        {
            sprintf(rec + size, ", ");
            size = size + 2;
        }
    }
    sprintf(rec + size, ")");

    return rec;
}

char *get_expected_param_type(symbol_t *func, int par_index)
{
    st_entry_t *aux = NULL;
    int count = 0;
    LanguageType type;
    aux = func->args;
    while (aux != NULL && aux->next != NULL && count < par_index)
    {
        aux = aux->next;
        count++;
    }
    if (aux != NULL)
    {
        type = ((symbol_t *)(aux->data))->type;
    }
    else
    {
        type = TYPE_TBA;
    }

    return type_name(type);
}

char *get_argument_type(node_t *func, int par_index)
{
    node_t *aux = NULL;
    node_t *aux_2 = NULL;
    int current = 0;
    LanguageType type;
    aux = func->children;
    while (aux != NULL && current < par_index - 1)
    {
        aux_2 = aux->children;
        while (aux_2 != NULL && aux_2->brothers != NULL)
        {
            aux_2 = aux_2->brothers;
        }
        aux = aux_2;

        current++;
    }

    if (aux != NULL)
    {
        type = aux->type;
    }
    else
    {
        type = TYPE_NA;
    }

    return type_name(type);
}

char *reconstruct_node(node_t *node)
{
    char *code = malloc(2048);
    char *temp1 = NULL;
    char *temp2 = NULL;
    char *temp3 = NULL;
    int size = 0;

    switch (node->st_kind)
    {
        case CMD_OPERAND:
            if (node->lexval->category == CAT_IDENTIFIER)
            {
                sprintf(code + size, "%s", node->lexval->value.name);
                size += strlen(node->lexval->value.name);
            }
            else
            {
                temp1 = literal_name(node);
                sprintf(code + size, "%s", temp1);
                size += strlen(temp1);
            }
            break;
        case CMD_VECTOR_ACCESS:
            temp1 = reconstruct_node(node->children);
            temp2 = reconstruct_node(node->children->brothers);
            sprintf(code + size, "%s[%s]", temp1, temp2);
            size += strlen(temp1) + strlen(temp2) + 2;
            break;
        case CMD_UNOP: 
            temp1 = reconstruct_node(node->children);
            sprintf(code + size, "%s%s", node->lexval->value.name, temp1);
            size += strlen(temp1) + 1;
            break;
        case CMD_BINOP:
            temp1 = reconstruct_node(node->children);
            temp2 = node->lexval->value.name;
            temp3 = reconstruct_node(node->children->brothers);
            sprintf(code + size, "%s %s %s", temp1, temp2, temp3);
            size += strlen(temp1) + strlen(temp2) + strlen(temp3) + 2;
            break;
        case CMD_TERNOP: 
            temp1 = reconstruct_node(node->children->brothers);
            temp2 = reconstruct_node(node->children);
            temp3 = reconstruct_node(node->children->brothers->brothers);
            sprintf(code + size, "%s ? %s : %s", temp1, temp2, temp3);
            size += strlen(temp1) + strlen(temp2) + strlen(temp3) + 6;
            break;
        case CMD_FUNCTION_CALL:
            temp1 = reconstruct_function_call(node);
            sprintf(code + size, "%s", temp1);
            size += strlen(temp1);
            break;
        case CMD_ATTRIB_VARIABLE: 
            temp1 = reconstruct_node(node->children);
            temp2 = reconstruct_node(node->children->brothers);
            sprintf(code + size, "%s = %s", temp1, temp2);
            size += strlen(temp1) + strlen(temp2) + 3;
            break;
        case CMD_INIT_VARIABLE:
            temp1 = reconstruct_node(node->children);
            temp2 = reconstruct_node(node->children->brothers);
            sprintf(code + size, "%s <= %s", temp1, temp2);
            size += strlen(temp1) + strlen(temp2) + 4;
            break;
        case CMD_RETURN: 
            temp1 = reconstruct_node(node->children);
            sprintf(code + size, "return %s", temp1);
            size += strlen(temp1) + 7;
            break;
        case CMD_IF: 
            temp1 = reconstruct_node(node->children);
            sprintf(code + size, "if ( %s ) ...", temp1);
            size += strlen(temp1) + 11;
            break;
        case CMD_FOR:
            temp1 = reconstruct_node(node->children);
            temp2 = reconstruct_node(node->children->brothers);
            temp3 = reconstruct_node(node->children->brothers->brothers);
            sprintf(code + size, "for (%s : %s : %s) ...", temp1, temp2, temp3);
            size += strlen(temp1) + strlen(temp2) + strlen(temp3) + 16;
            break;
        case CMD_WHILE:
            temp1 = reconstruct_node(node->children);
            sprintf(code + size, "while ( %s ) ...", temp1);
            size += strlen(temp1) + 14;
            break;
        case CMD_IO:
            temp1 = node->lexval->value.name;
            temp2 = reconstruct_node(node->children);
            sprintf(code + size, "%s %s", temp1, temp2);
            size += strlen(temp1) + strlen(temp2) + 1;
            break;
        default:
            sprintf(code + size, "< OTHER NODES >");
            size += 15;
            break;
    }

    return code;
}

void print_error(error_t *error)
{
    printf("\n\n[ERROR] On line %d: ", get_line_number());

    if (error != NULL)
    {
        switch (error->error_type)
        {
            case ERR_UNDECLARED:
                printf("Identifier \"%s\" undeclared (first use in this function)\n", (char *)error->data1);
                break;
            case ERR_DECLARED:
                printf("Redeclaration of \"%s\"\n", ((symbol_t *)(error->data1))->key);
                printf("[ERROR] Previous declaration was on line %d\n", ((symbol_t *)(error->data2))->declaration_line);
                break;
            case ERR_VARIABLE:
                printf("Incorrect use of variable \"%s\":\n", ((symbol_t *)(error->data1))->key);
                printf(" %s\n", reconstruct_node((node_t *)(error->data2)));
                printf("[ERROR] Declared on line %d\n", ((symbol_t *)(error->data1))->declaration_line);
                break;
            case ERR_VECTOR:
                printf("Incorrect use of vector \"%s\":\n", ((symbol_t *)(error->data1))->key);
                printf(" %s\n", reconstruct_node((node_t *)(error->data2)));
                printf("[ERROR] Declared on line %d\n", ((symbol_t *)(error->data1))->declaration_line);
                break;
            case ERR_FUNCTION:
                printf("Incorrect use of function \"%s\":\n", ((symbol_t *)(error->data1))->key);
                printf(" %s\n", reconstruct_node((node_t *)(error->data2)));
                printf("[ERROR] Declared on line %d:\n", ((symbol_t *)(error->data1))->declaration_line);
                printf(" %s\n", reconstruct_function_declaration((symbol_t *)(error->data1)));
                break;
            case ERR_WRONG_TYPE:
                if (((node_t *)(error->data1))->st_kind == CMD_TERNOP)
                {
                    printf("Expression types in ternary operator are incompatible:\n");
                    printf(" %s : %s\n", reconstruct_node(((node_t *)(error->data1))->children), reconstruct_node(((node_t *)(error->data1))->children->brothers->brothers));
                    printf("True statement has type %s, but false statement has type %s\n", type_name(((node_t *)(error->data1))->children->type), type_name(((node_t *)(error->data1))->children->brothers->brothers->type));
                }
                else
                {
                    printf("Assignment from incompatible types:\n");
                    printf(" %s\n", reconstruct_node(((node_t *)(error->data1))));
                    printf("[ERROR] lval has type %s, but rval has type %s\n", type_name(((node_t *)(error->data1))->children->type), type_name(((node_t *)(error->data1))->children->brothers->type));
                }
                break;
            case ERR_STRING_TO_X:
                printf("No implicit conversion for STRING:\n");
                printf(" %s\n", reconstruct_node((node_t *)(error->data1)));
                break;
            case ERR_CHAR_TO_X:
                printf("No implicit conversion for CHAR:\n");
                printf(" %s\n", reconstruct_node((node_t *)(error->data1)));
                break;
            case ERR_STRING_SIZE:
                printf("Incorrect size being attributed to STRING:\n");
                printf(" %s\n", reconstruct_node((node_t *)(error->data2)));
                printf("[ERROR] string \"%s\" has defined size %d, but rval expression has string size %d\n", ((symbol_t *)(error->data1))->key, ((symbol_t *)(error->data1))->size, *((int *)(error->data3)));
                printf("[ERROR] string \"%s\" was declared on line %d\n", ((symbol_t *)(error->data1))->key, ((symbol_t *)(error->data1))->declaration_line);
                break;
            case ERR_MISSING_ARGS:
                printf("Too few arguments in function call:\n");
                printf(" %s\n", reconstruct_function_call((node_t *)(error->data2)));
                printf("[ERROR] Function was declared on line %d:\n", ((symbol_t *)(error->data1))->declaration_line);
                printf(" %s\n", reconstruct_function_declaration((symbol_t *)(error->data1)));
                break;
            case ERR_EXCESS_ARGS:
                printf("Too many arguments in function call:\n");
                printf(" %s\n", reconstruct_function_call((node_t *)(error->data2)));
                printf("[ERROR] Function was declared on line %d:\n", ((symbol_t *)(error->data1))->declaration_line);
                printf(" %s\n", reconstruct_function_declaration((symbol_t *)(error->data1)));
                break;
            case ERR_WRONG_TYPE_ARGS:
                printf("Incorrect argument type in argument %d of function call:\n", *((int *)(error->data3)));
                printf(" %s\n", reconstruct_function_call((node_t *)(error->data2)));
                printf("[ERROR] Exepected %s, but got %s\n", get_expected_param_type((symbol_t *)(error->data1), *((int *)(error->data3))), get_argument_type((node_t *)(error->data2), *((int *)(error->data3))));
                printf("[ERROR] Function was declared on line %d:\n", ((symbol_t *)(error->data1))->declaration_line);
                printf(" %s\n", reconstruct_function_declaration((symbol_t *)(error->data1)));
                break;
            case ERR_WRONG_PAR_INPUT:
                printf("Incompatible types for INPUT statement:\n");
                printf(" %s\n", reconstruct_node((node_t *)(error->data1)));
                printf("[ERROR] Expected INT or FLOAT, but got %s\n", type_name(((node_t *)(error->data1))->children->type));
                break;
            case ERR_WRONG_PAR_OUTPUT:
                printf("Incompatible type for OUTPUT statement:\n");
                printf(" %s\n", reconstruct_node((node_t *)(error->data1)));
                printf("[ERROR] Expected INT or FLOAT, but got %s\n", type_name(((node_t *)(error->data1))->children->type));
                break;
            case ERR_WRONG_PAR_RETURN:
                printf("Incompatible type for RETURN statement:\n");
                printf(" return %s\n", reconstruct_node((node_t *)(error->data2)));
                printf("[ERROR] Expected %s, but got expression of type %s\n", type_name(((symbol_t *)(error->data1))->type), type_name(((node_t *)(error->data2))->type));
                printf("[ERROR] Function was declared on line %d:\n", ((symbol_t *)(error->data1))->declaration_line);
                printf(" %s\n", reconstruct_function_declaration((symbol_t *)(error->data1)));
                break;
            case ERR_WRONG_PAR_SHIFT:
                printf("Incorrect index for SHIFT\n");
                printf("[ERROR] Index must be less than 16, but got %d\n", *((int *)(error->data1)));
                break;
            default:
                printf("Invalid error code received from parser: %d\n", error->error_type);
                break;
        }
        printf("Exiting with status code %d \n\n", error->error_type);
        exit(error->error_type);
    }
    else
    {
        printf("Received NULL error descriptor.\n This should never happen, aborting.\n\n");
        exit(-1);
    }
}
