#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../include/Tree.h"
#include "../include/Stack.h"
#include "../include/others.h"


#define PRINT_ARG(curr_node) if(curr_node->data_type == number) printf(" %lg ", curr_node->val->number);         \
                        else printf(" %c ", get_oper_symbol(curr_node->val->op))


Tree* tree_create(size_t node_count)
{
    Tree* tree = (Tree*)calloc(1, sizeof(Tree));

    if (!tree)
        return TREE_CALLOC_ERROR;
    
    Node* root = (Node*) calloc(node_count, sizeof(Node));

    if (!root)
        return TREE_CALLOC_ERROR;

    tree->node_list = root;

    tree->capacity = node_count;
    tree->node_list->left = NULL;
    tree->node_list->right = NULL;
    tree->node_list->previous = NULL;

    FILE* tree_log = FOPEN("tree_log.cpp", "w+");
    tree->tree_log = tree_log;
    
    return tree;
}

void node_list_print(Tree* tree)
{
    for (size_t i = 0; i < tree->node_count; i++)
    {
        printf("Node %zu has arg ", i);
        PRINT_ARG((tree->node_list + i));
        printf("\n");
    }
}

Tree* tree_increase_capasity(Tree* tree)
{
    //printf("increase capacity!\n");
    //printf("old capacity = %zu\n", tree->capacity);

    tree->node_list = (Node*)realloc(tree->node_list, sizeof(Node) * tree->capacity * TREE_NODE_INCREASE_COEF);
    tree->capacity *= TREE_NODE_INCREASE_COEF;

    //printf("new capacity = %zu\n", tree->capacity);

    return tree; 
}

void tree_detor(Tree* tree)
{
    for (size_t i = 0; i < tree->node_count; i++)
    {
        printf("Attempt to free node[%zu]\n", i);

        printf("I plan to free a node_data to address %p, val is ",
        &((tree->node_list + i)->val));
        PRINT_ARG(((tree->node_list + i)));

        free((tree->node_list + i)->val);
        printf("Sucsess!\n");
    }
    
    free(tree->node_list);
    
    FCLOSE(tree->tree_log);

    tree->capacity = 0;
    tree->node_count = 0;

    free(tree);
}

void tree_print(const Node* tree, FILE* tree_data, size_t* level)
{
    //printf("I am in print \n");

    if (tree == NULL) return;

    //fprint_nchar(tree_data, '\t', *level);

    if(tree->left == NULL)
    {
        printf("I plan to write a node_data to address %p, val is ", tree->val);
        PRINT_ARG(tree);

        fprintf(tree_data, "%lg", tree->val->number);
        return;        
    }

    fprintf(tree_data, "{");
    (*level)++;

    tree_print(tree->left, tree_data, level);

    //fprint_nchar(tree_data, '\t', *level);

    fprintf(tree_data, "%c", get_oper_symbol(tree->val->op));

    tree_print(tree->right, tree_data, level);

    (*level)--;

    //fprint_nchar(tree_data, '\t', *level);
    fprintf(tree_data, "}");
}

void skip_spaces(char* source, size_t* pos)
{
    while (isspace(*(source + *pos)))
    {
        (*pos)++;
    }
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
    if (curr_tree->node_count == curr_tree->capacity)
        tree_increase_capasity(curr_tree);

    Node* new_node = &(curr_tree->node_list)[curr_tree->node_count++];

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

/*int way_search(Node* curr_node, double val, Stack* way_to_obj)
{
    if ((curr_node->left == NULL) && (curr_node->right == NULL))
    {
        int diff = CmpDbl(val, curr_node->val->number);

        if (!diff) 
        {
            return OBJECT_FOUND;
        }

        return OBJECT_N_FOUND;
    }

    int buffer = 0;

    StackPush(way_to_obj, left);
    
    if (way_search(curr_node->left, val, way_to_obj) == OBJECT_FOUND)
    {
        return OBJECT_FOUND;
    }
    StackPop(way_to_obj, &buffer);

    StackPush(way_to_obj, right);
    if  (way_search(curr_node->right, val, way_to_obj) == OBJECT_FOUND) return OBJECT_FOUND;
    StackPop(way_to_obj, &buffer);

    return OBJECT_N_FOUND;
}

struct Stack* way_stack(Tree* tree, double val)
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
    case add:
        return '+';
    case sub:
        return '-';
    case mul:
        return '*';
    case divis:
        return '/';
    case null_operator:
    default:
        return '?';
        break;
    }

    return 0;
}


void node_dtor(Node* node)
{
    if (node == NULL) return;

    free(node->val);

    node_dtor(node->left);
    node_dtor(node->right);

    free(node);

    return;
}