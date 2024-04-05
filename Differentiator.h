#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <stdio.h>
#include "./include/Tree.h"

#define PRINT_ARG(curr_node) if(curr_node->data_type == number) printf(" %lg \n", curr_node->val->number);         \
                        else printf(" %c \n", get_oper_symbol(curr_node->val->op))

#define _NUM(num) create_node(number, val_double((double) num), NULL, NULL)

#define _ADD(left_node, right_node) create_node(operation, Add,   left_node, right_node)
#define _SUB(left_node, right_node) create_node(operation, Sub,   left_node, right_node)
#define _MUL(left_node, right_node) create_node(operation, Mul,   left_node, right_node)
#define _DIV(left_node, right_node) create_node(operation, Div,   left_node, right_node)

#define _SIN(right_node_node) create_node(func, Sin, NULL, right_node_node)
#define _COS(right_node_node) create_node(func, Cos, NULL, right_node_node)

#define DL diff_the_tree(node->left)
#define DR diff_the_tree(node->right)

#define CL copy_tree(node->left)
#define CR copy_tree(node->right)


#define VAL_FILLING(type, data, key) node_data* val_##type(type data)                                   \
{                                                                                                       \
    union node_data* val = (node_data*)calloc(1, sizeof(node_data));                                    \
    key;                                                                                                \
    printf("I am crete a new node_data to address %p, val is %c\n", &val, get_oper_symbol(val->op));    \
                                                                                                        \
    return val;                                                                                         \
}




int file_read (const char* file_name, Tree* tree);
void file_write(const char* file_name, Node* root);

Node* differentiator_tree_read(char* source, Tree* akinator_tree, size_t* pos);
void operation_scan(char* source, size_t* pos, Node* curr_node);
void arg_scan(char* source, size_t* pos, Node* curr_node);

Operation get_oper_code(char* source);
Function get_funct_code(char* func);
void skip_arg(char* source, size_t *pos);
size_t nsymbol_in_str(char* source, char symbol);

Node* diff_the_tree(const Node* node);
Node* create_node(Type, const node_data*, Node*, Node*);
Node* copy_tree(const Node*);


node_data* val_double           (double);
node_data* val_Operation        (Operation);
node_data* val_char             (char); 
node_data* val_Function         (Function); 

#endif