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

    FILE* tree_log = FOPEN("tree_log.cpp", "w+");
    tree->tree_log = tree_log;

    assert(tree);
    
    return tree;
}



void tree_detor(Tree* tree)
{
    node_dtor(tree->root);
    
    FCLOSE(tree->tree_log);

    free(tree);

    printf("Free ends\n");
}

void tree_print(const Node* tree, FILE* tree_data)
{
    if (tree == NULL) return;

    fprintf(tree_data, "(");

    if ((tree->right == NULL) && (tree->left == NULL))
    {
        switch(tree->data_type)
        {
        case number:
        {
            //printf("I plan to write a number, val is ");
            //print_arg(tree);
            //printf("\n");

            fprintf(tree_data, "%lg", tree->val->number);
            break;
        }
        case var:
        {
            //printf("I plan to write a var, val is ");
            //print_arg(tree);
            //printf("\n");

            fprintf(tree_data, "%c", tree->val->var);
            break;
        }
        case empty_node:
            //printf("Empty node\n");
        case func:
        case operation:
        default:
            break;
        }
    }
    else
    {
        //printf("\nleft print\n");
        tree_print(tree->left, tree_data);

        switch (tree->data_type)
        {
        case operation:
            fprintf(tree_data, "%c", get_oper_symbol(tree->val->op));
            break;
        case func:
            print_func(tree_data, tree->val->func);
        case var:
        case number:
        case empty_node:
        default:
            break;
        }
    
        //printf("\nright print\n");
        tree_print(tree->right, tree_data);
    }

    fprintf(tree_data, ")");
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

Node* tree_add_node(Node* parent, Child subtree, Tree* curr_tree, Type tp, void* arg)
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

/*void graph_image(Node* start)
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
        fprintf(tree_info, "->\"%s\";\n", curr_node->val);
        return;
    }

    NODE_CHILD_PRINT(tree_info, curr_node, curr_node->left);
    START_NODE_CHILD_PRINT(tree_info, curr_node, curr_node->right);

    return;
}*/

char get_oper_symbol(Operation op)
{
    switch (op)
    {
    case add_op:
        return '+';
    case sub_op:
        return '-';
    case mul_op:
        return '*';
    case divis_op:
        return '/';
    case null_op:
    default:
        return '?';
        break;
    }

    return 0;
}


void node_dtor(Node* node)
{
    if (node == NULL) return;

    printf("I plan to clean val ");
    print_arg(node);
    printf("\n");

    free(node->val);

    node_dtor(node->left);
    node_dtor(node->right);

    free(node);

    return;
}

void print_arg(const Node* curr_node)
{
    switch (curr_node->data_type)
    {                                       
    case number:
        printf(" #%lg# ", curr_node->val->number);
        break;
    case var:
        printf(" #%c# ", curr_node->val->var);
        break;
    case operation:
        printf(" %c ", get_oper_symbol(curr_node->val->op));
        break;
    case func:
        print_func(stdout, curr_node->val->func);
        break;
    case empty_node:
        printf("Empty node\n");
        break;
    default:
        printf("Unknown type <%lln>\n", (long long*)(curr_node->val));
        break;
    }
}

void print_func(FILE* dest, Function func)
{
    switch (func)
    {
    case sin_f:
        fprintf(dest, "SIN");
        break;
    case cos_f:
        fprintf(dest, "COS");
        break;
    case ln_f:
        fprintf(dest, "LN");
        break;
    case tg_f:
        fprintf(dest, "TG");
        break;
    case null_f:
    default:
        printf("Unknown function\n");
        break;
    }
}