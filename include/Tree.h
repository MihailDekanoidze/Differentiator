#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <ctype.h>
#include <stdint.h>


#define BASE_NODE_COUNT 100
#define TREE_NODE_INCREASE_COEF   20     
#define STR_LEN         100
#define OBJECT_FOUND    1 << 7
#define OBJECT_N_FOUND  1 << 8
#define RUS_CHAR_SIZE   sizeof('ф')

#define TREE_CALLOC_ERROR  NULL;

#define TREE_PRINT(node_list, file)     size_t level = 0;                                    \
                                        tree_print(node_list, file, &level)


#define NODE_CHILD_PRINT(file, node, child)             fprintf(file, "->\"%s\"", node->val);         \
                                                        node_dot_create(child, file)
#define START_NODE_CHILD_PRINT(file, node, child)       fprintf(file, "\"%s\"", node->val);         \
                                                        node_dot_create(child, file)

#define FOPEN(file_name, mode) fopen(file_name, mode); /*fprintf(stderr, "open: %s on line: %d %s\n", #file_name, __LINE__, __PRETTY_FUNCTION__);*/ 
#define FCLOSE(file_name) int file_close = fclose(file_name); /*fprintf(stderr, "close: %s  on line: %d %s\n", #file_name, __LINE__, __PRETTY_FUNCTION__);*/ \
    if (file_close != 0) {fprintf(stderr, "meow!!\n");}


typedef unsigned char Data;
typedef struct Node Node;

enum Operation
{
    null_operator = 0,
    add = 1,
    sub = 2,
    divis = 3,
    mul = 4
};

union node_data
{
    Operation op;
    double number;
};


enum Type
{
    operation = 1,
    number    = 2
};


typedef struct Node {
    Type data_type;
    node_data* val;            
    Node* left;
    Node* right;
    Node* previous;
} Node;

enum Child
{
    left  = 1,
    right = 2
};

struct Tree {
    Node* node_list;             //rename
    size_t node_count;
    size_t capacity;
    FILE* tree_log;
};

Tree*               tree_create(size_t node_count);
void                tree_print(Node* tree, FILE* tree_log, size_t* level);
Tree*               tree_increase_capasity(Tree* tree);
void                tree_detor(Tree* tree);

Node*               tree_add_node(Node* parent, Child subtree, Tree* curr_tree, Type tp, void* arg);

void                node_list_print(Tree* main_tree);
void                skip_spaces(char* source, size_t* pos);
void                ClearBuffer(void);
void                fprint_nchar(FILE* dest, char symbol, size_t count);
char                get_oper_symbol(Node* tree);

//struct Stack*       way_stack(Tree* tree, char* val);
//int                 way_search(Node* curr_node, double val, struct Stack* way_to_obj);

//void                graph_image(Node* start);
//void                node_dot_create(Node* curr_node, FILE* tree_info);

#endif