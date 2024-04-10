#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <stdio.h>
#include "./include/Tree.h"

#define PRINT_ARG(curr_node) if(curr_node->data_type == number) printf(" %lg \n", curr_node->val->number);         \
                        else printf(" %c \n", get_oper_symbol(curr_node->val->op))

#define _NUM(num) create_node(number, val_double(num), NULL, NULL)

#define _ADD(left_node, right_node) create_node(operation, Add,   left_node, right_node)
#define _SUB(left_node, right_node) create_node(operation, Sub,   left_node, right_node)
#define _MUL(left_node, right_node) create_node(operation, Mul,   left_node, right_node)
#define _DIV(left_node, right_node) create_node(operation, Div,   left_node, right_node)
#define _POW(left_node, right_node) create_node(operation, Pow,   left_node, right_node)

#define _SIN(right_node) create_node(func, Sin, NULL, right_node)
#define _COS(right_node) create_node(func, Cos, NULL, right_node)
#define  _TG(right_node) create_node(func, Tg,  NULL, right_node)
#define _CTG(right_node) create_node(func, Ctg, NULL, right_node)
#define  _SH(right_node) create_node(func, Sh,  NULL, right_node)
#define  _CH(right_node) create_node(func, Ch,  NULL, right_node)
#define  _TH(right_node) create_node(func, Th,  NULL, right_node)
#define _CTH(right_node) create_node(func, Cth, NULL, right_node)
#define  _LN(right_node) create_node(func, Ln,  NULL, right_node)



#define DL diff_the_tree(node->left)
#define DR diff_the_tree(node->right)

#define CL copy_tree(node->left)
#define CR copy_tree(node->right)


#define VAL_FILLING(type, data, key) node_data* val_##type(type data)                                   \
{                                                                                                       \
    union node_data* val = (node_data*)calloc(1, sizeof(node_data));                                    \
    key;                                                                                                \
    /*printf("I am crete a new node_data to address %p, val is %c\n", &val, get_oper_symbol(val->op));*/\  
                                                                                                        \
    return val;                                                                                         \
}




int file_read (const char* file_name, Tree* tree);
void file_write(const char* file_name, Node* root);

Operation get_oper_code(char* source);
Function get_funct_code(char* func);
size_t nsymbol_in_str(char* source, char symbol);

Node* diff_the_tree(const Node* node);
Node* create_node(Type, node_data*, Node*, Node*);
Node* copy_tree(const Node*);

int var_search(Node* curr_node);
void const_calculation(Node* node, size_t* changes);


void       syntax_error(void);
Node*      get_G(char* str);
Node*      get_N(char** S);
Node*      get_E(char** S);
Node*      get_T(char** S);
Node*      get_P(char** S);
double     get_C(char** S);
Node*      get_F(char** S);
double     get_constant(char* source);
Node*      get_D(char** S);


double     get_G(const char* str, double var);
double     get_N(void);
double     get_E(void);
double     get_T(void);
double     get_P(void);
double     get_C(void);
double     get_F(void);
double     get_D(void);

node_data* val_double           (double);
node_data* val_Operation        (Operation);
node_data* val_char             (char); 
node_data* val_Function         (Function); 


#define Null        val_Operation(null_op)                                      // in main filling or use constructor
#define Add         val_Operation(add_op)
#define Sub         val_Operation(sub_op)     //array with funct node_datas
#define Mul         val_Operation(mul_op)
#define Div         val_Operation(divis_op)
#define Pow         val_Operation(pow_op)
#define Sin         val_Function(sin_f)
#define Cos         val_Function(cos_f)
#define Tg          val_Function(tg_f)
#define Ctg         val_Function(ctg_f)
#define Sh          val_Function(sh_f)
#define Ch          val_Function(ch_f)
#define Th          val_Function(th_f)
#define Cth         val_Function(cth_f)     
#define Ln          val_Function(ln_f)



#endif