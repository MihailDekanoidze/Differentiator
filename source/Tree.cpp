#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../include/Tree.h"
#include "../include/Stack.h"
#include "../include/others.h"

Tree* tree_create(void)
{
    Tree* tree = (Tree*)calloc(1, sizeof(Tree));
    assert(tree);

    tree->root = NULL;

    /*tree->root->left = NULL;
    tree->root->right = NULL;
    tree->root->previous = NULL;*/

    FILE* tree_log = fopen("tree_log.txt", "w");
    tree->tree_log = tree_log;

    assert(tree);
    
    return tree;
}

void tree_detor(Tree* tree)
{
    node_dtor_all(tree->root);
    
    fclose(tree->tree_log);

    free(tree);

    //printf("Free ends\n");
}

void node_print(const Node* tree, FILE* tree_data)
{
    if(!tree) return;

    if((tree->left != NULL) && (tree->right != NULL)) fprintf(tree_data, "(");
    if ((tree->right == NULL) && (tree->left == NULL))
    {
        switch(tree->data_type)
        {
        case number:
        {
            if (tree->val->number < 0) 
            {
                fprintf(tree_data, "(%lg)", tree->val->number);
                break;
            }

            fprintf(tree_data, "%lg", tree->val->number);
            break;
        }
        case var:
        {
            fprintf(tree_data, "%s", tree->val->var);
            break;
        }
        case empty_node:
            printf("Empty node\n");
        case func:
        case operation:
        default:
            break;
        }
    }
    else
    {
        //printf("\nleft print\n");
        node_print(tree->left, tree_data);

        switch (tree->data_type)
        {
        case operation:
            fprintf(tree_data, "%c", get_oper_symbol(tree->val->op));
            break;
        case func:
            fprint_func(tree_data, tree->val->func);
        case var:
        case number:
        case empty_node:
        default:
            break;
        }
    
        //printf("\nright print\n");
        node_print(tree->right, tree_data);
    }

    if((tree->left != NULL) && (tree->right != NULL)) fprintf(tree_data, ")");
}


void skip_spaces(char* source, size_t* pos)
{
    while (isspace(*(source + *pos)))
    {
        (*pos)++;
    }
}

void skip_alpha(char* source, size_t* pos)
{
    printf("cur symbol is %c\n", *(source + *pos));
    while (isalpha(*(source + *pos)))
    {
        printf("alpfa symbol is %c\n", *(source + *pos));
        (*pos)++;
    }
    printf("cur symbol is %c\n", *(source + *pos));
}


void ClearBuffer(void)
{
    char ch = 0;

    while ((ch = (char)getchar()) != '\n' && ch != EOF) {}
}

void fprint_nchar(FILE* dest, char symbol, size_t count)
{
    for (size_t i = 0; i < count; i++) {fprintf(dest, "%c", symbol);}
}

Node* tree_add_node(Node* parent, Child subtree, Type tp, void* arg)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));
    
    new_node->left  = NULL;
    new_node->right = NULL;
    new_node->previous = parent;
    new_node->val = (node_data*) calloc(1, sizeof(node_data));
    
    if (tp == operation)
    {
        new_node->data_type = operation;
        new_node->val->op = *((Operation*)arg);
    }
    else
    {
        new_node->data_type = number;
        new_node->val->number = *((double*)arg);
    }

    switch (subtree)
    {
        case left:
        {
            parent->left = new_node;
            break;
        }
        case right:
        {
            parent->right = new_node;
            break;
        }
        default:
        {
            printf("Неправильный ребёнок\n");
            return NULL;
        }
    }

    return new_node;
}


/*struct Stack* way_stack(Tree* tree, double val)
{
    Stack* way_to_obj = (Stack*) calloc (1, sizeof(Stack));

    StackCtor(way_to_obj, DEFAULT_CAPACITY);

    if (way_search(tree->node_list, val, way_to_obj) == OBJECT_N_FOUND)
    {
        StackDtor(way_to_obj);
        free(way_to_obj);
        return NULL;
    }
    else
    {
        return way_to_obj;
    }
}*/

void graph_image(Node* start)
{
    FILE* tree_info = FOPEN("tree.dot", "w");

    fprintf(tree_info, "digraph G\n");
    fprintf(tree_info, "{\n");

    fprintf(tree_info, "rankdir=TB;\n");
    fprintf(tree_info, "node[color=\"red\"shape=\"rectangle\",style=\"rounded\"];\n");

    START_NODE_CHILD_PRINT(tree_info, start, start->left);
    START_NODE_CHILD_PRINT(tree_info, start, start->right);

    fprintf(tree_info, "}%c", '\0');

    FCLOSE(tree_info);
}

void node_dot_create(Node* curr_node, FILE* tree_info)
{
    if ((curr_node->left == NULL) && (curr_node->right == NULL))
    {
        fprintf(tree_info, "\"");                        
        fprint_arg(tree_info, curr_node);                     
        fprintf(tree_info, "\"");
        return;
    }

    NODE_CHILD_PRINT(tree_info, curr_node, curr_node->left);
    START_NODE_CHILD_PRINT(tree_info, curr_node, curr_node->right);

    return;
}

void node_dtor_all(Node* node)
{
    if (node == NULL) return;

    free(node->val);

    node_dtor_all(node->left);
    node_dtor_all(node->right);

    free(node);

    return;
}

void node_dtor_one(Node* node, Child saved_child)
{
    if (node == NULL) return;
    free(node->val);

    Node* saved_node = NULL;
    if (saved_child == left)
    {
        saved_node = node->left;
        free(node->right->val);
        free(node->right);
    }
    else    
    {
        saved_node = node->right;
        free(node->left->val);
        free(node->left);
    }

    node->data_type = saved_node->data_type;
    node->left = saved_node->left;
    node->right = saved_node->right;    
    node->val = saved_node->val;

    free(saved_node);

    return;
}

Operation get_oper_code(char* source)
{
    for (size_t i = 0; i < OPERATION_COUNT; i++)
        if (*source == op_info[i].symbol) return op_info[i].op;

    printf("Expected unknown oper\n");
    return null_op;
}
Function get_funct_code(char* source)
{
    for (size_t i = 0; i < FUNCTION_COUNT; i++)
    {
        printf("compare between example <<%s>> and <<", funct_info[i].name);
        for (size_t j  = 0; j < strlen(funct_info[i].name); j++)
        {
            printf("%c", *(source + j));
        }    
        printf(">>\n");
        if (!strncmp(funct_info[i].name, source, strlen(funct_info[i].name))) return funct_info[i].function;

    }
        
    printf("Expected unknown func\n");
    return null_f;
}

char get_oper_symbol(Operation op)
{
    for (size_t i = 0; i < OPERATION_COUNT; i++)
        if (op == op_info[i].op) return op_info[i].symbol;
    
    printf("Expected unknown func\n");
    return 0;
}

void fprint_arg(FILE* dest, const Node* curr_node)
{
    switch (curr_node->data_type)
    {                                       
    case number:
        fprintf(dest, " #%lg# ", curr_node->val->number);
        break;
    case var:
        fprintf(dest, " #%s# ", curr_node->val->var);
        break;
    case operation:
        fprintf(dest, " %c ", get_oper_symbol(curr_node->val->op));
        break;
    case func:
        fprint_func(dest, curr_node->val->func);
        break;
    case empty_node:
        fprintf(dest, "Empty node\n");
        break;
    default:
        fprintf(dest, "Unknown type <%lln>\n", (long long*)(curr_node->val));
        break;
    }
}

void fprint_func(FILE* dest, Function func)
{
    //printf("print_funct get %d\n", func);
    switch (func)
    {
    case sin_f:
        fprintf(dest, "sin");
        break;
    case cos_f:
        fprintf(dest, "cos");
        break;
    case ln_f:
        fprintf(dest, "ln");
        break;
    case tg_f:
        fprintf(dest, "tg");
        break;
    case ctg_f:
        fprintf(dest, "ctg");
        break;
    case sh_f:
        fprintf(dest, "sh");
        break;
    case ch_f:
        fprintf(dest, "ch");
        break;
    case th_f:
        fprintf(dest, "th");
        break;
    case cth_f:
        fprintf(dest, "cth");
        break;
    case null_f:
    default:
        printf("Unknown function\n");
        break;
    }
}

const char* get_funct_name(Function funct)
{
    for (size_t i = 0; i < FUNCTION_COUNT; i++)
        if ((const Function)funct == funct_info[i].function) return funct_info[i].name;
    
    printf("Expected unknown func\n");
    return 0;
}